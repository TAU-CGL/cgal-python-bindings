#!/usr/bin/python
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import importlib
import math
import sys
from pathlib import Path

WIDTH = 192
HEIGHT = 192
MARGIN = 12

U_MIN = -0.50
U_MAX = 1.05
V_MIN = -0.05
V_MAX = 1.05


def project(x, y, z):
    u = x - 0.45 * y
    v = z + 0.35 * x + 0.35 * y
    if not (U_MIN <= u <= U_MAX and V_MIN <= v <= V_MAX):
        raise ValueError(
            f"projected point outside fixed viewport: ({u}, {v})"
        )

    px = MARGIN + int(
        math.floor(
            ((u - U_MIN) / (U_MAX - U_MIN))
            * (WIDTH - 1 - 2 * MARGIN)
            + 0.5
        )
    )
    py = HEIGHT - 1 - MARGIN - int(
        math.floor(
            ((v - V_MIN) / (V_MAX - V_MIN))
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
        "usage: pol3_visual_buffer_raster.py "
        "<binding-library> <output.ppm>"
    )

library = sys.argv[1]
output = Path(sys.argv[2])

CGALPY = importlib.import_module(library)
Ker = CGALPY.Ker
Pol3 = CGALPY.Pol3
Bvr = CGALPY.Bvr

polyhedron = Pol3.Polyhedron_3()
polyhedron.make_tetrahedron(
    Ker.Point_3(1, 0, 0),
    Ker.Point_3(0, 1, 0),
    Ker.Point_3(0, 0, 1),
    Ker.Point_3(0, 0, 0),
)

scene = Bvr.Graphics_scene()
Bvr.add_to_graphics_scene(polyhedron, scene)

buffer = scene.buffer(Bvr.POS_SEGMENTS)
if not buffer:
    raise RuntimeError("Polyhedron graphics scene has no POS_SEGMENTS")
if len(buffer) % 6 != 0:
    raise RuntimeError(
        f"POS_SEGMENTS float count is not divisible by 6: {len(buffer)}"
    )

image = bytearray([255]) * (WIDTH * HEIGHT * 3)

for offset in range(0, len(buffer), 6):
    x0, y0, z0, x1, y1, z1 = buffer[offset:offset + 6]
    p0 = project(float(x0), float(y0), float(z0))
    p1 = project(float(x1), float(y1), float(z1))
    draw_line(image, p0[0], p0[1], p1[0], p1[1])

write_ppm(output, image)
print("raster_width", WIDTH)
print("raster_height", HEIGHT)
print("raster_segments", len(buffer) // 6)
