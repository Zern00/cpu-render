#pragma once
#include <vector>
#include <cstdint>
#include <algorithm>
#include <filesystem>

namespace egn {
    class Texture {
    private:
        int width_, height_;
        std::vector<uint32_t> data_;
    public:
        Texture(int width, int height) : width_(width), height_(height) {
            data_.resize(width_ * height_);
        }

        inline int width() noexcept {
            return width_;
        }

        inline int height() noexcept {
            return height_;
        }

        static Texture loadFromFile(const std::filesystem::path path) {
            int w, h, channels;
            unsigned char* img = stbi_load(path.c_str(), &w, &h, &channels, 4);
            if (!img) {
                throw std::runtime_error("Failed to load texture: " + std::string(stbi_failure_reason()));
            }

            Texture tex(w, h);
            for (int i = 0; i < w * h; ++i) {
                uint8_t r = img[i * 4 + 0];
                uint8_t g = img[i * 4 + 1];
                uint8_t b = img[i * 4 + 2];
                uint8_t a = img[i * 4 + 3];
                tex.data_[i] = (a << 24) | (b << 16) | (g << 8) | r;
            }
            stbi_image_free(img);
            return tex;
        }

        //returns chess-board texture
        static Texture empty(int width, int height, int cellSize = 32) {
            Texture tex(width, height);
            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    bool isWhite = ((x / cellSize) + (y / cellSize)) % 2 == 0;
                    tex.setPixel(x, y, isWhite ? packColor(255, 255, 255) : packColor(0, 0, 0));
                }
            }
            return tex;
        }

        void setPixel(int x, int y, uint32_t color) noexcept {
            if (x < 0 || x >= width_ || y < 0 || y >= height_) return;
            data_[width_ * y + x] = color;
        }

        uint32_t getPixel(int x, int y) noexcept {
            if (x < 0 || x >= width_ || y < 0 || y >= height_) return 0;
            return data_[width_ * y + x];
        }

        static uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
            return (a << 24) | (b << 16) | (g << 8) | r;
        }

        uint32_t sample(float u, float v) const {
            u = std::clamp(u, 0.0f, 1.0f);
            v = std::clamp(v, 0.0f, 1.0f);
            int x = static_cast<int>(u * (width_ - 1));
            int y = static_cast<int>(v * (height_ - 1));
            return data_[y * width_ + x];
        }
    };
}