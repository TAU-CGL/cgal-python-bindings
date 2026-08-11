// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Graphics_scene.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/draw_polyhedron.h>

#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Point = Kernel::Point_3;
using Polyhedron = CGAL::Polyhedron_3<Kernel>;

constexpr int width = 192;
constexpr int height = 192;
constexpr int margin = 12;

constexpr double u_min = -0.50;
constexpr double u_max = 1.05;
constexpr double v_min = -0.05;
constexpr double v_max = 1.05;

std::pair<int, int> project(double x, double y, double z) {
  const double u = x - 0.45 * y;
  const double v = z + 0.35 * x + 0.35 * y;

  if (u < u_min || u > u_max || v < v_min || v > v_max) {
    throw std::runtime_error(
      "projected point outside fixed viewport");
  }

  const int px = margin + static_cast<int>(std::floor(
    ((u - u_min) / (u_max - u_min))
      * static_cast<double>(width - 1 - 2 * margin) + 0.5));

  const int py = height - 1 - margin - static_cast<int>(std::floor(
    ((v - v_min) / (v_max - v_min))
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
      std::cerr
        << "usage: pol3_visual_buffer_raster <output.ppm>\n";
      return 2;
    }

    Polyhedron polyhedron;
    polyhedron.make_tetrahedron(
      Point(1, 0, 0),
      Point(0, 1, 0),
      Point(0, 0, 1),
      Point(0, 0, 0));

    CGAL::Graphics_scene scene;
    CGAL::add_to_graphics_scene(polyhedron, scene);

    const auto& buffer =
      scene.get_array_of_index(CGAL::Graphics_scene::POS_SEGMENTS);

    if (buffer.empty()) {
      throw std::runtime_error(
        "Polyhedron graphics scene has no POS_SEGMENTS");
    }
    if (buffer.size() % 6 != 0) {
      throw std::runtime_error(
        "POS_SEGMENTS float count is not divisible by 6");
    }

    std::vector<unsigned char> image(
      static_cast<std::size_t>(width * height * 3),
      static_cast<unsigned char>(255));

    for (std::size_t offset = 0; offset < buffer.size(); offset += 6) {
      const auto p0 = project(
        buffer[offset],
        buffer[offset + 1],
        buffer[offset + 2]);
      const auto p1 = project(
        buffer[offset + 3],
        buffer[offset + 4],
        buffer[offset + 5]);

      draw_line(
        image,
        p0.first,
        p0.second,
        p1.first,
        p1.second);
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
