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

auto vector::operator*(type rhs) const -> vector {
  return { x * rhs, y * rhs, z * rhs };
}

auto vector::operator+(type rhs) const -> vector {
  return { x + rhs, y + rhs, z + rhs };
}

auto vector::operator-(type rhs) const -> vector {
  return { x - rhs, y - rhs, z - rhs };
}

auto vector::operator/(type rhs) const -> vector {
  return { x / rhs, y / rhs, z / rhs };
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

auto vector::normalize() -> vector& {
  return *this /= this->magnitude();
}

auto vector::magnitude() -> type {
  return std::hypot(x, y, z);
}
