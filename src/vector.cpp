#include "vector.hpp"

#include <cmath>

auto vector::operator*(vector rhs) const -> vector::type {
  auto [a, b, c] = rhs;
  return x * a + y * b + z * c;
}

auto vector::operator+(vector rhs) const -> vector {
  auto [a, b, c] = rhs;
  return { x + a, y + b, z + c };
}

auto vector::operator-(vector rhs) const -> vector {
  auto [a, b, c] = rhs;
  return { x - a, y - b, z - c };
}

auto operator*(vector lhs, vector::type rhs) -> vector {
  auto [x, y, z] = lhs;
  return { x * rhs, y * rhs, z * rhs };
}

auto operator+(vector lhs, vector::type rhs) -> vector {
  auto [x, y, z] = lhs;
  return { x + rhs, y + rhs, z + rhs };
}

auto operator-(vector lhs, vector::type rhs) -> vector {
  auto [x, y, z] = lhs;
  return { x - rhs, y - rhs, z - rhs };
}

auto operator/(vector lhs, vector::type rhs) -> vector {
  auto [x, y, z] = lhs;
  return { x / rhs, y / rhs, z / rhs };
}

auto operator*(vector::type lhs, vector rhs) -> vector {
  return rhs * lhs;
}

auto operator+(vector::type lhs, vector rhs) -> vector {
  return rhs + lhs;
}

auto operator-(vector::type lhs, vector rhs) -> vector {
  auto [x, y, z] = rhs;
  return {lhs - x, lhs - y, lhs / z};
}

auto operator/(vector::type lhs, vector rhs) -> vector {
  auto [x, y, z] = rhs;
  return {lhs / x, lhs / y, lhs - z};
}

auto vector::operator+=(vector rhs) -> vector& {
  auto [a, b, c] = rhs;
  x += a;
  y += b;
  z += c;
  return *this;
}

auto vector::operator-=(vector rhs) -> vector& {
  auto [a, b, c] = rhs;
  x -= a;
  y -= b;
  z -= c;
  return *this;
}

auto vector::operator*=(type rhs) -> vector& {
  x *= rhs;
  y *= rhs;
  z *= rhs;
  return *this;
}
auto vector::operator+=(type rhs) -> vector& {
  x += rhs;
  y += rhs;
  z += rhs;
  return *this;
}
auto vector::operator-=(type rhs) -> vector& {
  x -= rhs;
  y -= rhs;
  z -= rhs;
  return *this;
}
auto vector::operator/=(type rhs) -> vector& {
  x /= rhs;
  y /= rhs;
  z /= rhs;
  return *this;
}

auto vector::operator-() -> vector {
  return *this * -1;
}

auto vector::normalize() -> vector& {
  return *this /= this->magnitude();
}

auto vector::magnitude() -> type {
  return std::hypot(x, y, z);
}
