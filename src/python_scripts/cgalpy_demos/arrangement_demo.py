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
MN2 = CGALPY.MN2

Arrangement_2 = Arr2.Arrangement_2
Point_2 = Ker.Point_2
Segment_2 = Ker.Segment_2
Curve_2 = Arr2.Curve_2

from gui_demo import GUI_demo, QtWidgets, QtCore


def point_to_tuple(p):
    return p.x().to_double(), p.y().to_double()


def set_up_scene_arr():
    gui.clear_scene()
    gui.empty_queue()
    p0 = Point_2(0, 0)
    p1 = Point_2(100, 100)
    p2 = Point_2(0, 100)
    p3 = Point_2(100, 0)
    s0 = Segment_2(p0, p1)
    c0 = Curve_2(s0)
    c1 = Curve_2(Segment_2(p2, p3))
    arr = Arrangement_2()
    Arr2.insert(arr, c0)
    Arr2.insert(arr, c1)

    for edge in arr.edges():
        p0x = edge.source().point().x().to_double()
        p0y = edge.source().point().y().to_double()
        p1x = edge.target().point().x().to_double()
        p1y = edge.target().point().y().to_double()
        gui.add_segment(p0x, p0y, p1x, p1y, QtCore.Qt.red)

    disc = gui.add_disc(4, 0, 0)
    for face in arr.faces():
        for ccb in face.inner_ccbs():
            for halfedge in ccb:
                x0 = halfedge.source().point().x().to_double()
                y0 = halfedge.source().point().y().to_double()
                x1 = halfedge.target().point().x().to_double()
                y1 = halfedge.target().point().y().to_double()
                anim = gui.linear_translation_animation(disc, x0, y0, x1, y1, 300)
                gui.queue_animation(anim)


def play_animation():
    gui.play_queue()


poly2 = None
if __name__ == "__main__":
    import sys

    app = QtWidgets.QApplication(sys.argv)
    gui = GUI_demo()
    gui.set_logic(0, set_up_scene_arr)
    gui.set_button_text(0, "arrangement scene")
    gui.set_logic(2, play_animation)
    gui.set_button_text(2, "play animation")
    gui.set_animation_finished_action(lambda: None)
    gui.mainWindow.show()
    sys.exit(app.exec_())
