// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include "arr_exact_construction_segments.h"

#include <CGAL/draw_arrangement_2.h>
#include <CGAL/Graphics_scene.h>

#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

constexpr int width = 192;
constexpr int height = 160;
constexpr int margin = 8;
constexpr double x_min = 0.0;
constexpr double x_max = 6.0;
constexpr double y_min = 0.0;
constexpr double y_max = 5.0;

std::pair<int, int> map_point(double x, double y) {
  if (x < x_min || x > x_max || y < y_min || y > y_max) {
    throw std::runtime_error("point outside fixed viewport");
  }
  const int px = margin + static_cast<int>(std::floor(
    ((x - x_min) / (x_max - x_min))
      * static_cast<double>(width - 1 - 2 * margin) + 0.5));
  const int py = height - 1 - margin - static_cast<int>(std::floor(
    ((y - y_min) / (y_max - y_min))
      * static_cast<double>(height - 1 - 2 * margin) + 0.5));
  return {px, py};
}

void set_black(std::vector<unsigned char>& image, int x, int y) {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    throw std::runtime_error("pixel outside raster");
  }
  const std::size_t offset =
    static_cast<std::size_t>((y * width + x) * 3);
  image[offset] = 0;
  image[offset + 1] = 0;
  image[offset + 2] = 0;
}

void draw_line(std::vector<unsigned char>& image,
               int x0, int y0, int x1, int y1) {
  const int dx = std::abs(x1 - x0);
  const int sx = x0 < x1 ? 1 : -1;
  const int dy = -std::abs(y1 - y0);
  const int sy = y0 < y1 ? 1 : -1;
  int error = dx + dy;

  while (true) {
    set_black(image, x0, y0);
    if (x0 == x1 && y0 == y1) {
      break;
    }
    const int twice_error = 2 * error;
    if (twice_error >= dy) {
      error += dy;
      x0 += sx;
    }
    if (twice_error <= dx) {
      error += dx;
      y0 += sy;
    }
  }
}

void write_ppm(const std::string& path,
               const std::vector<unsigned char>& image) {
  std::ofstream output(path, std::ios::binary);
  if (!output) {
    throw std::runtime_error("unable to open raster output");
  }
  output << "P6\n" << width << ' ' << height << "\n255\n";
  output.write(
    reinterpret_cast<const char*>(image.data()),
    static_cast<std::streamsize>(image.size()));
  if (!output) {
    throw std::runtime_error("failed while writing raster output");
  }
}

} // namespace

int main(int argc, char** argv) {
  try {
    if (argc != 2) {
      std::cerr << "usage: aos2_visual_buffer_raster <output.ppm>\n";
      return 2;
    }

    Segment segments[] = {
      Segment(Point(1, 0), Point(2, 4)),
      Segment(Point(5, 0), Point(5, 5)),
      Segment(Point(1, 0), Point(5, 3)),
      Segment(Point(0, 2), Point(6, 0)),
      Segment(Point(3, 0), Point(5, 5))
    };

    Arrangement arrangement;
    insert(
      arrangement,
      segments,
      segments + sizeof(segments) / sizeof(Segment));

    CGAL::Graphics_scene scene;
    CGAL::add_to_graphics_scene(arrangement, scene);

    const auto& buffer =
      scene.get_array_of_index(CGAL::Graphics_scene::POS_SEGMENTS);
    if (buffer.size() % 6 != 0) {
      throw std::runtime_error(
        "POS_SEGMENTS float count is not divisible by 6");
    }

    std::vector<unsigned char> image(
      static_cast<std::size_t>(width * height * 3),
      static_cast<unsigned char>(255));

    for (std::size_t offset = 0; offset < buffer.size(); offset += 6) {
      const double x0 = buffer[offset];
      const double y0 = buffer[offset + 1];
      const double z0 = buffer[offset + 2];
      const double x1 = buffer[offset + 3];
      const double y1 = buffer[offset + 4];
      const double z1 = buffer[offset + 5];

      if (std::abs(z0) > 1e-6 || std::abs(z1) > 1e-6) {
        throw std::runtime_error(
          "expected the Aos2 scene to lie in the XY plane");
      }

      const auto p0 = map_point(x0, y0);
      const auto p1 = map_point(x1, y1);
      draw_line(image, p0.first, p0.second, p1.first, p1.second);
    }

    write_ppm(argv[1], image);
    std::cout << "raster_width " << width << '\n';
    std::cout << "raster_height " << height << '\n';
    std::cout << "raster_segments " << buffer.size() / 6 << '\n';
    return 0;
  }
  catch (const std::exception& error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
