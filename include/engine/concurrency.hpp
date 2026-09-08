#pragma once
#include <algorithm>
#include <condition_variable>
#include <cstddef>
#include <future>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace egn {
    class Threadpool {
    public:
        explicit Threadpool(size_t count_workers = std::thread::hardware_concurrency()) : isActive_(true) {
            count_workers = std::max(count_workers, size_t{1});
            workers_.reserve(count_workers);

            try {
                for (size_t i = 0; i < count_workers; ++i) {
                    workers_.emplace_back([this]{worker_task();});
                }
            } catch (...) {
                isActive_.store(false);
                cv_.notify_all();
                throw;
            }
        }

        Threadpool(const Threadpool&) = delete;
        Threadpool& operator=(const Threadpool&) = delete;
        
        template<typename F, typename... Args>
        auto PushTask(F&& func, Args&&... args) {
            using return_type = std::invoke_result_t<F, Args...>;
            auto task = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(func), std::forward<Args>(args)...)
            );

            std::future<return_type> fut = task->get_future();
            {
                std::lock_guard<std::mutex> lock(mtx_);
                if (!isActive_) {
                    throw std::runtime_error("Threadpool is already terminated");
                }
                queue_.emplace([task]{(*task)();});
            }
            cv_.notify_one();
            return fut;
        }

        size_t workersCount() const noexcept {
            return workers_.size();
        }

        ~Threadpool() {
            isActive_.store(false);
            cv_.notify_all();
            for (auto& t : workers_) {
                if (t.joinable()) {
                    t.join();
                }
            }
        }
    private:
        void worker_task() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx_);
                    cv_.wait(lock, [this]{return !queue_.empty() || !isActive_;});

                    if (queue_.empty() && !isActive_) {
                        return;
                    }
                    task = std::move(queue_.front());
                    queue_.pop();
                }
                task();
            }
        }

        std::mutex mtx_;
        std::vector<std::jthread> workers_;
        std::condition_variable cv_;
        std::atomic<bool> isActive_;
        std::queue<std::function<void()>> queue_;
    };

} //namespace egn
