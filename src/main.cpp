#include "vector.hpp"

#include <cmath>

#include <chrono>
#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
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

struct rgb {
  f32 r, g, b;
};

namespace color {
inline constexpr static auto max    = 255;
inline constexpr static rgb  orange = { 0.8, 0.5, 0.1 };
inline constexpr static rgb  blue   = { 0.1, 0.2, 1 };
inline constexpr static rgb  black  = { 0, 0, 0 };
};  // namespace color

struct shape {
  vector center;
  rgb    color;
};

struct sphere : shape {
  u32 radius;
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

auto intersect(const ray& ray, const sphere& sphere)
    -> std::optional<std::pair<f32, f32>> {
  auto r2  = sphere.radius * sphere.radius;
  auto l   = sphere.center - ray.pos;
  auto tca = l * ray.dir;
  if (tca < 0) return std::nullopt;
  auto d2 = l * l - tca * tca;
  if (d2 > r2) return std::nullopt;
  auto thc = std::sqrt(r2 - d2);
  return std::pair{ tca - thc, tca + thc };
}

auto render(const scene& s) -> bitmap {
  std::vector<rgb> image(s.width * s.height, color::black);
  for (auto i = 0; i < s.height; i++) {
    for (auto j = 0; j < s.width; j++) {
      auto ray = s.view.ray(i, j);
      for (const auto& object: s.objects) {
        auto opt = intersect(ray, object);
        if (opt.has_value()) {
          image[i * s.width + j] = object.color;
        }
      }
    }
  }
  return { s.width, s.height, std::move(image) };
}

auto load(std::string_view /* path */) -> scene {
  return { 1920,
           1080,
           {
               sphere{ shape{ { 0, 0, -70 }, color::orange }, 500 },
               sphere{ shape{ { 400, 0, -200 }, color::orange }, 30 },
           },
           options{ 1920, 1080 } };
}

auto load(options opts) -> scene {
  u32 w = 1920;
  u32 h = 1080;
  return {
    w,
    h,
    {
        sphere{ shape{ { 0, 0, -70 }, color::orange }, 10 },
        sphere{ shape{ { 50, 40, -200 }, color::blue }, 5 },
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

  auto scene = load({ 1920, 1080 });

  std::cout << "Rendering...\n";
  auto a   = chrono::steady_clock::now();
  auto img = render(scene);
  auto b   = chrono::steady_clock::now();
  std::cout << "Finished raytracing\n"
            << "Elapsed time: "
            << chrono::duration_cast<chrono::milliseconds>(b - a).count()
            << "ms\n";

  std::ofstream output{ "scene.ppm", std::ios::binary };
  save(img, output);
}
