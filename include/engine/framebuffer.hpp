#pragma once
#include <vector>
#include <cstdint>
#include "glm-like-lib/Gfloat.hpp"

namespace egn {
    class Framebuffer {
    private:
        int width_, height_;
        std::vector<uint32_t> data_;
        std::vector<gll::Gfloat> depth_;
    public:
        Framebuffer(int width, int height);

        void clear(uint32_t rgba = 0xFF000000);

        int width() noexcept;

        int height() noexcept;

        uint32_t* color();

        gll::Gfloat* depth();

        void setDepth(int x, int y, gll::Gfloat depth) noexcept;

        gll::Gfloat getDepth(int x, int y) noexcept;

        void setPixel(int x, int y, uint32_t color) noexcept;

        uint32_t getPixel(int x, int y) noexcept;

        static uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept;
    };
}
