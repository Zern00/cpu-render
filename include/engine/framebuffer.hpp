#pragma once
#include <vector>
#include <cstdint>

namespace egn {
    class Framebuffer {
    private:
        int width_, height_;
        std::vector<uint32_t> data_;
        std::vector<float> depth_;
    public:
        Framebuffer(int width, int height) : width_(width), height_(height) {
            data_.assign(width_ * height_, 0xFF000000);
            depth_.assign(width_ * height_, 1.0f);
        }

        void clear(uint32_t rgba = 0xFF000000) {
            std::fill(data_.begin(), data_.end(), rgba);
            std::fill(depth_.begin(), depth_.end(), 1.0f);
        }

        inline int width() noexcept {
            return width_;
        }

        inline int height() noexcept {
            return height_;
        }

        inline uint32_t* color() {
            return data_.data();
        }

        inline float* depth() {
            return depth_.data();
        }

        void setDepth(int x, int y, float depth) noexcept {
            if (x < 0 || x >= width_ || y < 0 || y >= height_) return;
            depth_[width_ * y + x] = depth;
        }

        float getDepth(int x, int y) noexcept {
            if (x < 0 || x >= width_ || y < 0 || y >= height_) return 0;
            return depth_[width_ * y + x];
        }

        void setPixel(int x, int y, uint32_t color) noexcept {
            if (x < 0 || x >= width_ || y < 0 || y >= height_) return;
            data_[width_ * y + x] = color;
        }

        uint32_t getPixel(int x, int y) noexcept {
            if (x < 0 || x >= width_ || y < 0 || y >= height_) return 0;
            return data_[width_ * y + x];
        }

        static uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept {
            return (a << 24) | (b << 16) | (g << 8) | r;
        }
    };
}