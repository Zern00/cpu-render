#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>
#include <type_traits>

namespace gll {

    struct Gfloat {
        static constexpr float EPSILON = 1e-5f;

        float value;
        constexpr Gfloat() noexcept : value(0.0f) {}

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        constexpr Gfloat(T v) noexcept : value(static_cast<float>(v)) {}

        constexpr operator float() const noexcept { return value; }

        constexpr bool operator==(const Gfloat& other) const noexcept {
            float diff = value - other.value;
            return (diff < 0.0f ? -diff : diff) < EPSILON;
        }

        constexpr bool operator!=(const Gfloat& other) const noexcept {
            return !(*this == other);
        }
        constexpr bool operator<(const Gfloat& other) const noexcept {
            return value < other.value;
        }
        constexpr bool operator>(const Gfloat& other) const noexcept {
            return value > other.value;
        }
        constexpr bool operator<=(const Gfloat& other) const noexcept {
            return value <= other.value;
        }
        constexpr bool operator>=(const Gfloat& other) const noexcept {
            return value >= other.value;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        constexpr bool operator==(T other) const noexcept { return *this == Gfloat(other); }
        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        constexpr bool operator!=(T other) const noexcept { return *this != Gfloat(other); }
        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        constexpr bool operator<(T other) const noexcept { return *this < Gfloat(other); }
        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        constexpr bool operator>(T other) const noexcept { return *this > Gfloat(other); }
        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        constexpr bool operator<=(T other) const noexcept { return *this <= Gfloat(other); }
        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        constexpr bool operator>=(T other) const noexcept { return *this >= Gfloat(other); }

        constexpr Gfloat operator+() const noexcept { return *this; }
        constexpr Gfloat operator-() const noexcept { return Gfloat(-value); }

        constexpr Gfloat& operator+=(const Gfloat& other) noexcept {
            value += other.value;
            return *this;
        }
        constexpr Gfloat& operator-=(const Gfloat& other) noexcept {
            value -= other.value;
            return *this;
        }
        constexpr Gfloat& operator*=(const Gfloat& other) noexcept {
            value *= other.value;
            return *this;
        }
        constexpr Gfloat& operator/=(const Gfloat& other) noexcept {
            value /= other.value;
            return *this;
        }

        constexpr Gfloat& operator++() noexcept { ++value; return *this; }
        constexpr Gfloat operator++(int) noexcept { Gfloat tmp = *this; ++value; return tmp; }
        constexpr Gfloat& operator--() noexcept { --value; return *this; }
        constexpr Gfloat operator--(int) noexcept { Gfloat tmp = *this; --value; return tmp; }
    };

    constexpr Gfloat operator+(const Gfloat& a, const Gfloat& b) noexcept {
        Gfloat result = a;
        result += b;
        return result;
    }
    constexpr Gfloat operator-(const Gfloat& a, const Gfloat& b) noexcept {
        Gfloat result = a;
        result -= b;
        return result;
    }
    constexpr Gfloat operator*(const Gfloat& a, const Gfloat& b) noexcept {
        Gfloat result = a;
        result *= b;
        return result;
    }
    constexpr Gfloat operator/(const Gfloat& a, const Gfloat& b) noexcept {
        Gfloat result = a;
        result /= b;
        return result;
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr Gfloat operator+(Gfloat a, T b) noexcept { return a + Gfloat(b); }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr Gfloat operator+(T a, Gfloat b) noexcept { return Gfloat(a) + b; }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr Gfloat operator-(Gfloat a, T b) noexcept { return a - Gfloat(b); }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr Gfloat operator-(T a, Gfloat b) noexcept { return Gfloat(a) - b; }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr Gfloat operator*(Gfloat a, T b) noexcept { return a * Gfloat(b); }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr Gfloat operator*(T a, Gfloat b) noexcept { return Gfloat(a) * b; }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr Gfloat operator/(Gfloat a, T b) noexcept { return a / Gfloat(b); }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr Gfloat operator/(T a, Gfloat b) noexcept { return Gfloat(a) / b; }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr bool operator==(T left, Gfloat right) noexcept { return Gfloat(left) == right; }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr bool operator!=(T left, Gfloat right) noexcept { return Gfloat(left) != right; }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr bool operator<(T left, Gfloat right) noexcept { return Gfloat(left) < right; }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr bool operator>(T left, Gfloat right) noexcept { return Gfloat(left) > right; }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr bool operator<=(T left, Gfloat right) noexcept { return Gfloat(left) <= right; }
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    constexpr bool operator>=(T left, Gfloat right) noexcept { return Gfloat(left) >= right; }

    inline std::ostream& operator<<(std::ostream& os, const Gfloat& g) {
        os << g.value;
        return os;
    }
    inline std::istream& operator>>(std::istream& is, Gfloat& g) {
        is >> g.value;
        return is;
    }

}
