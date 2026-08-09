#pragma once
#include <vector>
#include <cstdint>
#include <algorithm>
#include <filesystem>
#include "glm-like-lib/Gfloat.hpp"

namespace egn {
    class Texture {
    private:
        int width_, height_;
        std::vector<uint32_t> data_;
    public:
        Texture(int width, int height);

        int width() noexcept;

        int height() noexcept;

        static Texture loadFromFile(const std::filesystem::path path);

        //returns chess-board texture
        static Texture empty(int width, int height, int cellSize = 32);

        void setPixel(int x, int y, uint32_t color) noexcept;

        uint32_t getPixel(int x, int y) noexcept;

        static uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

        uint32_t sample(gll::Gfloat u, gll::Gfloat v) const;
    };
}
