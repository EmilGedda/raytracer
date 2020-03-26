#include "vector.hpp"

#include <cmath>

#include <algorithm>
#include <chrono>
#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

constexpr static auto pi = static_cast<f32>(M_PI);

struct ray {
  vector pos;
  vector dir;
};

struct options {
  u32 width, height;
  f32 scale, aspect_ratio, fov;
  options(u32 width, u32 height, f32 fov = 30)
      : width(width), height(height), fov(fov) {
    scale        = tanf(fov * pi / 180);
    aspect_ratio = static_cast<f32>(width) / height;
  }
};

struct camera {
  options            opts;
  [[nodiscard]] auto ray(f32 y, f32 x) const -> ray {
    auto angle = tanf(pi / 2 * opts.fov / 180);
    f32  px    = (2 * ((x + 0.5) / opts.width) - 1) * angle * opts.aspect_ratio;
    f32  py    = (1 - 2 * ((y + 0.5) / opts.height)) * angle;
    auto direction = vector{ px, py, -1.0F }.normalize();
    return { position::origo, direction };
  }
};

using rgb = vector;

namespace color {
constexpr auto max    = 255;
constexpr rgb  orange = { 0.8, 0.5, 0.1 };
constexpr rgb  blue   = { 0.1, 0.2, 1 };
constexpr rgb  black  = { 0, 0, 0 };
auto           mix(const rgb& a, const rgb& b, const f32& strength) -> rgb {
  return a * (1 - strength) + b * strength;
}
};  // namespace color

struct shape {
  vector center;
  rgb    color;

  shape(vector center, rgb color) : center(center), color(color) {
  }

  [[nodiscard]] virtual auto intersect(const ray& ray) const noexcept
      -> std::optional<f32> = 0;
  [[nodiscard]] virtual auto bounce(const vector& phit, vector& nhit) const
      noexcept -> std::pair<f32, f32> = 0;
};

struct sphere : virtual shape {
  f32 radius;
  f32 r2;
  sphere(vector center, rgb color, f32 radius)
      : shape{ center, color }, radius(radius), r2(radius * radius) {
  }

  [[nodiscard]] auto intersect(const ray& ray) const noexcept
      -> std::optional<f32> override {
    auto r2  = radius * radius;
    auto l   = center - ray.pos;
    auto tca = l * ray.dir;
    if (tca < 0) return std::nullopt;
    auto d2 = l * l - tca * tca;
    if (d2 > r2) return std::nullopt;
    auto thc = std::sqrt(r2 - d2);
    return tca - thc;
    // todo add more stuff
  }

  [[nodiscard]] auto bounce(const vector& phit, vector& nhit) const noexcept
      -> std::pair<f32, f32> override {
    nhit = (phit - center).normalize();
    return {
      (1 + std::atan2(nhit.z, nhit.x) / pi) * 0.5,
      acosf(nhit.y) / pi,
    };
  }
};

struct scene {
  u32                 width, height;
  std::vector<sphere> objects;
  camera              view;
};

struct bitmap {
  u32              width, height;
  std::vector<rgb> pixels;
};


auto render(const scene& s) -> bitmap {
  std::vector<rgb> image(s.width * s.height, color::black);
  const auto  infinity = std::numeric_limits<f32>::numeric_limits::infinity();
  const auto& objs     = s.objects;
  for (auto i = 0; i < s.height; i++) {
    for (auto j = 0; j < s.width; j++) {
      auto ray      = s.view.ray(i, j);
      auto hit      = &objs[0];
      auto distance = infinity;
      for (const auto& object: objs) {
        auto opt = object.intersect(ray);
        if (opt && *opt < distance) {
          hit      = &object;
          distance = *opt;
        }
      }
      if (distance != infinity) {
        auto   phit = ray.pos + ray.dir * distance;
        vector normal;  // nhit
        auto [x, y]  = hit->bounce(phit, normal);
        auto scale   = 4.0F;
        auto pattern = static_cast<f32>((std::fmodf(x * scale, 1) > 0.5)
                                        ^ (fmodf(y * scale, 1) > 0.5));
        image[i * s.width + j] =
            color::mix(hit->color, hit->color * 0.8F, pattern)
            * std::max(0.f, -ray.dir * normal);
      }
    }
  }
  return { s.width, s.height, std::move(image) };
}

auto load(std::string_view /* path */) -> scene {
  return { 1920,
           1080,
           {
               sphere{ { 0, 0, -70 }, color::orange, 500 },
               sphere{ { 400, 0, -200 }, color::orange, 30 },
           },
           options{ 1920, 1080 } };
}

auto load(options opts) -> scene {
  return {
    opts.width,
    opts.height,
    {
        sphere{ { 0, 0, -70 }, color::orange, 10 },
        sphere{ { 50, 40, -200 }, color::blue, 5 },
        sphere{ { -5, -5, -40 }, color::blue, 3 },
    },
    opts,
  };
}

auto operator<<(std::ostream& stream, const rgb& pixel) -> std::ostream& {
  auto [r, g, b] = pixel;
  auto max       = color::max;
  u8   red       = r * max;
  u8   green     = g * max;
  u8   blue      = b * max;
  return stream << red << green << blue;
}

void save(const bitmap& image, std::ostream& stream) {
  stream << "P6 " << image.width << ' ' << image.height << ' ' << color::max
         << '\n';
  for (const auto& pixel: image.pixels) {
    stream << pixel;
  }
}

int main(int argc, char** argv) {  // NOLINT
  namespace chrono = std::chrono;

  std::string path = "scene.csv";

  if (argc == 2) {
    path = argv[1];
  }

  bitmap img;
  auto   scene = load({ 1920, 1080 });

  constexpr auto iterations = 1; //50;

  std::cout << "Rendering...\n";
  auto a = chrono::steady_clock::now();
  for (int i = 0; i < iterations; i++) {
    img = render(scene);
  }
  auto b = chrono::steady_clock::now();
  std::cout << "Finished raytracing\n"
            << "Elapsed time: "
            << chrono::duration_cast<chrono::milliseconds>(b - a).count()
                   / iterations
            << "ms\n";

  std::ofstream output{ "scene.ppm", std::ios::binary };
  save(img, output);
}
