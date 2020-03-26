#pragma once
#include <cstdint>

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8  = std::uint8_t;

using f32 = float;
using f64 = double;

struct vector {
  using type = float;
  type x, y, z;

  auto operator*(vector rhs) const -> type;
  auto operator+(vector rhs) const -> vector;
  auto operator-(vector rhs) const -> vector;

  auto operator*=(type rhs) -> vector&;
  auto operator+=(type rhs) -> vector&;
  auto operator-=(type rhs) -> vector&;
  auto operator/=(type rhs) -> vector&;

  auto operator+=(vector rhs) -> vector&;
  auto operator-=(vector rhs) -> vector&;

  auto magnitude() -> type;
  auto normalize() -> vector&;

  auto operator-() -> vector;
};

namespace position {
constexpr vector origo = { 0, 0, 0 };
};


auto operator*(vector lhs, vector::type rhs) -> vector;
auto operator+(vector lhs, vector::type rhs) -> vector;
auto operator-(vector lhs, vector::type rhs) -> vector;
auto operator/(vector lhs, vector::type rhs) -> vector;

auto operator*(vector::type lhs, vector rhs) -> vector;
auto operator+(vector::type lhs, vector rhs) -> vector;
auto operator-(vector::type lhs, vector rhs) -> vector;
auto operator/(vector::type lhs, vector rhs) -> vector;

auto intersect(const vector& a, const vector& b) -> bool;
