#!/usr/bin/python3.9
# export PYTHONPATH=...
import os
import sys
import importlib

if len(sys.argv) < 2:
    print('Library name missing, assuming CGALPY')
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Arr2 = CGALPY.Arr2
Pol2 = CGALPY.Pol2
MN2 = CGALPY.MN2

Arrangement_2 = Arr2.Arrangement_2
Point_2 = Ker.Point_2
Vector_2 = Ker.Vector_2
Segment_2 = Ker.Segment_2
Curve_2 = Arr2.Curve_2
Polygon_2 = Pol2.Polygon_2

from gui_demo import GUI_demo, QtWidgets, QtCore


def point_to_tuple(p):
    return p.x().to_double(), p.y().to_double()


def add_polygon(gui, poly, color):
    lst0 = []
    for point in poly.vertices():
        lst0.append(point_to_tuple(point))
    return gui.add_polygon(lst0, color)


def set_up_scene_ms():
    gui.clear_scene()
    gui.empty_queue()
    bounding_rect = [(0, 0), (0, 1000), (1000, 1000), (1000, 0)]
    gui.add_polygon(bounding_rect, QtCore.Qt.transparent)
    poly = Polygon_2()
    poly.push_back(Point_2(0, 0))
    poly.push_back(Point_2(100, 0))
    poly.push_back(Point_2(100, 200))

    poly1 = Polygon_2()
    poly1.push_back(Point_2(300, 0))
    poly1.push_back(Point_2(350, 0))
    poly1.push_back(Point_2(350, 50))
    poly1.push_back(Point_2(300, 50))

    p1 = add_polygon(gui, poly1, QtCore.Qt.red)
    p0 = add_polygon(gui, poly, QtCore.Qt.blue)



    poly2 = MN2.minkowski_sum_2(poly, poly1).outer_boundary()
    p2 = add_polygon(gui, poly2, QtCore.Qt.green)
    p2.visible = 0
    anim = gui.visibility_animation(p2, False)
    gui.queue_animation(anim)

    anim = gui.linear_translation_animation(p0, *point_to_tuple(poly[0]), *point_to_tuple(poly1[0]), 0)
    gui.queue_animation(anim)
    for i in range(11):
        start = point_to_tuple(poly1[i%2] + Vector_2(0, i * 5))
        end = point_to_tuple(poly1[((i + 1) % 2)] + Vector_2(0, i * 5))
        anim = gui.linear_translation_animation(p0, *start, *end)
        gui.queue_animation(anim)
    gui.queue_animation(gui.pause_animation(500))

    anim = gui.visibility_animation(p2, True)
    gui.queue_animation(anim)


def play_animation():
    gui.play_queue()


poly2 = None
if __name__ == "__main__":
    import sys

    app = QtWidgets.QApplication(sys.argv)
    gui = GUI_demo()
    gui.set_logic(0, set_up_scene_ms)
    gui.set_button_text(0, "ms scene")
    gui.set_logic(2, play_animation)
    gui.set_button_text(2, "play animation")
    gui.set_animation_finished_action(lambda: None)
    gui.mainWindow.show()
    sys.exit(app.exec_())
