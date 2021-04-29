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


def set_up_arrangement():
    arr0 = Arrangement_2()
    p0 = Point_2(0, 100)
    p1 = Point_2(100, 200)
    p2 = Point_2(200, 100)
    p3 = Point_2(100, 0)
    c0 = Curve_2(Segment_2(p0, p1))
    c1 = Curve_2(Segment_2(p1, p2))
    c2 = Curve_2(Segment_2(p2, p3))
    c3 = Curve_2(Segment_2(p3, p0))

    Arr2.insert(arr0, [c0, c1, c2, c3])

# Set the data for the faces. The data can be any python object
    for f in arr0.faces():
        if f.is_unbounded():
            f.set_data(0)
        else:
            f.set_data(1)

    # Alternative
    # ubf: Face = arr0.unbounded_face()
    # ubf.set_data(0)
    # f: Face
    # for ccb in ubf.inner_ccbs():
    #     next(next(ubf.inner_ccbs())).twin().face().set_data(1)

    arr1 = Arrangement_2()
    p0 = Point_2(50, 100)
    p1 = Point_2(150, 200)
    p2 = Point_2(250, 100)
    p3 = Point_2(150, 0)
    c0 = Curve_2(Segment_2(p0, p1))
    c1 = Curve_2(Segment_2(p1, p2))
    c2 = Curve_2(Segment_2(p2, p3))
    c3 = Curve_2(Segment_2(p3, p0))

    Arr2.insert(arr1, [c0, c1, c2, c3])

    for f in arr1.faces():
        if f.is_unbounded():
            f.set_data(0)
        else:
            f.set_data(1)

    result = Arrangement_2()
    # Set the data of a face that is the intersection of two faces
    # from the original arrangements to be the sum of their data
    overlay_traits = Arr2.Arr_face_overlay_traits(lambda x, y: x+y)
    Arr2.overlay(arr0, arr1, result, overlay_traits)

    # Splitting a preexisting face setting the data of the two new faces to match the old face data
    p1 = Point_2(200, 100)
    p2 = Point_2(250, 100)
    c = Curve_2(Segment_2(p1, p2))
    he = Arr2.insert_non_intersecting_curve(result, c)
    face_data = he.face().data() if he.face().data() is not None else he.twin().face().data()
    he.face().set_data(face_data)
    he.twin().face().set_data(face_data)

    # Drawing
    for face in result.faces():
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
