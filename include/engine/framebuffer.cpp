#include "framebuffer.hpp"
#include <algorithm>

namespace egn {
    Framebuffer::Framebuffer(int width, int height) : width_(width), height_(height) {
        data_.assign(width_ * height_, 0xFF000000);
        depth_.assign(width_ * height_, 1.0f);
    }

    void Framebuffer::clear(uint32_t rgba) {
        std::fill(data_.begin(), data_.end(), rgba);
        std::fill(depth_.begin(), depth_.end(), 1.0f);
    }

    int Framebuffer::width() noexcept {
        return width_;
    }

    int Framebuffer::height() noexcept {
        return height_;
    }

    uint32_t* Framebuffer::color() {
        return data_.data();
    }

    gll::Gfloat* Framebuffer::depth() {
        return depth_.data();
    }

    void Framebuffer::setDepth(int x, int y, gll::Gfloat depth) noexcept {
        if (x < 0 || x >= width_ || y < 0 || y >= height_) return;
        depth_[width_ * y + x] = depth;
    }

    gll::Gfloat Framebuffer::getDepth(int x, int y) noexcept {
        if (x < 0 || x >= width_ || y < 0 || y >= height_) return 0;
        return depth_[width_ * y + x];
    }

    void Framebuffer::setPixel(int x, int y, uint32_t color) noexcept {
        if (x < 0 || x >= width_ || y < 0 || y >= height_) return;
        data_[width_ * y + x] = color;
    }

    uint32_t Framebuffer::getPixel(int x, int y) noexcept {
        if (x < 0 || x >= width_ || y < 0 || y >= height_) return 0;
        return data_[width_ * y + x];
    }

    uint32_t Framebuffer::packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept {
        return (a << 24) | (b << 16) | (g << 8) | r;
    }
}
