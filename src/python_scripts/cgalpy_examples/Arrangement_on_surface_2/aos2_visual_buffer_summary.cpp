#include "arr_exact_construction_segments.h"

#include <CGAL/draw_arrangement_2.h>
#include <CGAL/Graphics_scene.h>

#include <iomanip>
#include <iostream>
#include <numeric>

namespace {

const char* py_bool(bool value) { return value ? "True" : "False"; }

void print_buffer_summary(const char* name,
                          CGAL::Graphics_scene::Buffers index,
                          const CGAL::Graphics_scene& scene) {
  const auto buffer = scene.get_array_of_index(index);
  const double sum = std::accumulate(buffer.begin(), buffer.end(), 0.0);

  std::cout << name
            << " elements " << scene.number_of_elements(index)
            << " size " << scene.get_size_of_index(index)
            << " floats " << buffer.size()
            << " sum " << std::fixed << std::setprecision(6) << sum
            << '\n';
}

} // namespace

int main() {
  Segment segments[] = {
    Segment(Point(1, 0), Point(2, 4)),
    Segment(Point(5, 0), Point(5, 5)),
    Segment(Point(1, 0), Point(5, 3)),
    Segment(Point(0, 2), Point(6, 0)),
    Segment(Point(3, 0), Point(5, 5))
  };

  Arrangement arr;
  insert(arr, segments, segments + sizeof(segments) / sizeof(Segment));

  CGAL::Graphics_scene scene;
  std::cout << "empty_before " << py_bool(scene.empty()) << '\n';

  CGAL::add_to_graphics_scene(arr, scene);

  std::cout << "empty_after " << py_bool(scene.empty()) << '\n';
  std::cout << "is_two_dimensional " << py_bool(scene.is_two_dimensional()) << '\n';

  // Only geometry buffers are compared. Color buffers can depend on
  // random/default coloring and are intentionally excluded.
  print_buffer_summary("POS_POINTS", CGAL::Graphics_scene::POS_POINTS, scene);
  print_buffer_summary("POS_SEGMENTS", CGAL::Graphics_scene::POS_SEGMENTS, scene);
  print_buffer_summary("POS_RAYS", CGAL::Graphics_scene::POS_RAYS, scene);
  print_buffer_summary("POS_LINES", CGAL::Graphics_scene::POS_LINES, scene);
  print_buffer_summary("POS_FACES", CGAL::Graphics_scene::POS_FACES, scene);
  print_buffer_summary("SMOOTH_NORMAL_FACES", CGAL::Graphics_scene::SMOOTH_NORMAL_FACES, scene);
  print_buffer_summary("FLAT_NORMAL_FACES", CGAL::Graphics_scene::FLAT_NORMAL_FACES, scene);

  return 0;
}
