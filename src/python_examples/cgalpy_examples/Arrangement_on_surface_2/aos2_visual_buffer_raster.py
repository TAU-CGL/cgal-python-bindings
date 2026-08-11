#!/usr/bin/python
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import importlib
import math
import sys
from pathlib import Path

WIDTH = 192
HEIGHT = 160
MARGIN = 8
X_MIN = 0.0
X_MAX = 6.0
Y_MIN = 0.0
Y_MAX = 5.0


def map_point(x, y):
    if not (X_MIN <= x <= X_MAX and Y_MIN <= y <= Y_MAX):
        raise ValueError(f"point outside fixed viewport: ({x}, {y})")
    px = MARGIN + int(
        math.floor(
            ((x - X_MIN) / (X_MAX - X_MIN))
            * (WIDTH - 1 - 2 * MARGIN)
            + 0.5
        )
    )
    py = HEIGHT - 1 - MARGIN - int(
        math.floor(
            ((y - Y_MIN) / (Y_MAX - Y_MIN))
            * (HEIGHT - 1 - 2 * MARGIN)
            + 0.5
        )
    )
    return px, py


def set_black(image, x, y):
    if not (0 <= x < WIDTH and 0 <= y < HEIGHT):
        raise ValueError(f"pixel outside raster: ({x}, {y})")
    offset = (y * WIDTH + x) * 3
    image[offset] = 0
    image[offset + 1] = 0
    image[offset + 2] = 0


def draw_line(image, x0, y0, x1, y1):
    dx = abs(x1 - x0)
    sx = 1 if x0 < x1 else -1
    dy = -abs(y1 - y0)
    sy = 1 if y0 < y1 else -1
    error = dx + dy

    while True:
        set_black(image, x0, y0)
        if x0 == x1 and y0 == y1:
            break
        twice_error = 2 * error
        if twice_error >= dy:
            error += dy
            x0 += sx
        if twice_error <= dx:
            error += dx
            y0 += sy


def write_ppm(path, image):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(
        f"P6\n{WIDTH} {HEIGHT}\n255\n".encode("ascii")
        + bytes(image)
    )


if len(sys.argv) != 3:
    raise SystemExit(
        "usage: aos2_visual_buffer_raster.py "
        "<binding-library> <output.ppm>"
    )

library = sys.argv[1]
output = Path(sys.argv[2])

CGALPY = importlib.import_module(library)
Aos2 = CGALPY.Aos2
Bvr = CGALPY.Bvr

Arrangement = Aos2.Arrangement_2
Point = Arrangement.Geometry_traits_2.Point_2
Segment = Arrangement.Geometry_traits_2.X_monotone_curve_2

segments = [
    Segment(Point(1, 0), Point(2, 4)),
    Segment(Point(5, 0), Point(5, 5)),
    Segment(Point(1, 0), Point(5, 3)),
    Segment(Point(0, 2), Point(6, 0)),
    Segment(Point(3, 0), Point(5, 5)),
]

arrangement = Arrangement()
Aos2.insert(arrangement, segments)

scene = Bvr.Graphics_scene()
Bvr.add_to_graphics_scene(arrangement, scene)

buffer = scene.buffer(Bvr.POS_SEGMENTS)
if len(buffer) % 6 != 0:
    raise RuntimeError(
        f"POS_SEGMENTS float count is not divisible by 6: {len(buffer)}"
    )

image = bytearray([255]) * (WIDTH * HEIGHT * 3)

for offset in range(0, len(buffer), 6):
    x0, y0, z0, x1, y1, z1 = buffer[offset:offset + 6]
    if abs(z0) > 1e-6 or abs(z1) > 1e-6:
        raise RuntimeError("expected the Aos2 scene to lie in the XY plane")
    p0 = map_point(float(x0), float(y0))
    p1 = map_point(float(x1), float(y1))
    draw_line(image, p0[0], p0[1], p1[0], p1[1])

write_ppm(output, image)
print("raster_width", WIDTH)
print("raster_height", HEIGHT)
print("raster_segments", len(buffer) // 6)
