#!/usr/bin/python3.9
# export PYTHONPATH=...
import os
import sys
import importlib

#import CGALPY_kerEpecInt_arr2SegExPl_bso2_bv_ch2_pol2_ms2_pp_ss_tri2

if len(sys.argv) < 2:
    print('Library name missing, assuming CGALPY')
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Arr2 = CGALPY.Arr2

Arrangement_2 = Arr2.Arrangement_2
Point_2 = Ker.Point_2
Segment_2 = Ker.Segment_2
Curve_2 = Arr2.Curve_2

from gui_demo import GUI_demo, QtWidgets, QtCore


def plus(x, y):
    return x + y


def set_up_arrangement():
    arr0 = Arrangement_2()
    p0 = Point_2(0, 1)
    p1 = Point_2(1, 2)
    p2 = Point_2(2, 1)
    p3 = Point_2(1, 0)
    c0 = Curve_2(Segment_2(p0, p1))
    c1 = Curve_2(Segment_2(p1, p2))
    c2 = Curve_2(Segment_2(p2, p3))
    c3 = Curve_2(Segment_2(p3, p0))

    Arr2.insert(arr0, [c0, c1, c2, c3])

    for face in arr0.faces():
        face.set_data(0)
        if not face.is_unbounded():
            face.set_data(1)

    arr1 = Arrangement_2()
    p0 = Point_2(0.5, 1)
    p1 = Point_2(1.5, 2)
    p2 = Point_2(2.5, 1)
    p3 = Point_2(1.5, 0)
    c0 = Curve_2(Segment_2(p0, p1))
    c1 = Curve_2(Segment_2(p1, p2))
    c2 = Curve_2(Segment_2(p2, p3))
    c3 = Curve_2(Segment_2(p3, p0))

    Arr2.insert(arr1, [c0, c1, c2, c3])

    for face in arr1.faces():
        face.set_data(0)
        if not face.is_unbounded():
            face.set_data(2)

    traits = Arr2.Arr_face_overlay_traits(plus)  # Define how a face data will be determined (sum of the data of the
    # two overlaid faces)
    arr2 = Arrangement_2()
    Arr2.overlay(arr0, arr1, arr2, traits)

    # splitting a preexisting face
    p1 = Point_2(2, 1)
    p2 = Point_2(2.5, 1)
    c = Curve_2(Segment_2(p1, p2))
    he = Arr2.insert_non_intersecting_curve(arr2, c)
    face_data = he.face().data() if he.face().data() is not None else he.twin().face().data()
    he.face().set_data(face_data)
    he.twin().face().set_data(face_data)

    for face in arr2.faces():
        print(face.data())
        lst = []
        if not face.is_unbounded():
            for edge in face.outer_ccb():
                lst.append(edge.source().point())
            lst = [(p.x().to_double(), p.y().to_double()) for p in lst]
            if face.data() == 3:
                gui.add_polygon(lst, QtCore.Qt.darkMagenta)
            if face.data() == 2:
                gui.add_polygon(lst, QtCore.Qt.blue)
            if face.data() == 1:
                gui.add_polygon(lst, QtCore.Qt.red)


if __name__ == "__main__":
    import sys

    app = QtWidgets.QApplication(sys.argv)
    gui = GUI_demo()
    gui.set_button_text(0, 'Display Result')
    gui.set_logic(0, set_up_arrangement)
    gui.mainWindow.show()
    sys.exit(app.exec_())
