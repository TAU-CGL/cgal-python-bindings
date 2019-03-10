from tkinter import *
import pickle
import functools
from Polygon_2 import *

class Point_2_old:
    x = 0.0
    y = 0.0
    def __init__(self, x = 0.0, y = 0.0):
        self.x = x
        self.y = y
    def __repr__(self):
        return "(" + str(self.x) + "," + str(self.y) + ")"

def pointCompare(p1, p2):
    xDiff = p1[0]-p2[0]
    if(xDiff != 0):
        return xDiff
    else:
        return p1[1]-p2[1]


class Polygon(Frame):
    polygon = Polygon_2()
    vertexCount = 0
    vertexLst = []
    vertexTup = ()
    pointSize = 8
    lineWidth = 2
    scale = 1
    filename = ""
    center = (400, 300)
    xOffset = 0
    yOffset = 0
    try:
        d = pickle.load(open("maxAreas.db.pickle", "rb"))
    except FileNotFoundError:
        d = {}

    def getVertices(self):
        self.clear()
        self.filename = self.inputBox.get()
        try:
            s = open(self.filename, "r")
        except FileNotFoundError:
            print("invalid file name")
            return
        self.vertexCount = int(s.read(1))
        s = s.read().split(" ")
        s.pop(0)
        for i in range(self.vertexCount):
            x = int(s[i * 2])
            y = int(s[i * 2 + 1])
            self.vertexLst.append((x, y))
        tmp = sorted(self.vertexLst, key=functools.cmp_to_key(pointCompare))
        self.vertexTup = tuple(tmp)
        self.createPolygon()

    def createPolygon(self):
        temp = []
        for p in self.vertexLst:
            x = p[0]
            y = p[1]
            p2 = Point_2(x,y)
            temp.append((x,y))
            self.polygon.push_back(p2)
        self.vertexLst = temp
        bbox = self.polygon.bbox()
        center = ((bbox.xmax()+bbox.xmin())/2, (bbox.ymax()+bbox.ymin())/2)

        self.xOffset = self.center[0] - center[0]
        self.yOffset = self.center[1] - center[1]

    def displayInfo(self):
        simple = self.polygon.is_simple()
        area = abs(self.polygon.area())
        currentMax = 0
        currentMaxFileName = ""
        if self.vertexTup in self.d:
            currentMax = self.d[self.vertexTup][0]
            currentMaxFileName = self.d[self.vertexTup][1]
            if simple and area > currentMax:
                currentMax = area
                currentMaxFileName = self.filename
                self.d[self.vertexTup] = (area, self.filename, tuple(self.vertexLst))
        else:
            currentMax = area
            currentMaxFileName = self.filename
            self.d[self.vertexTup] = (area, self.filename, tuple(self.vertexLst))
        s = ""
        s += "simple: " + str(simple) + "\n"
        s += "area: " + str(area) + "\n"
        s += "current max area for these vertices: " + str(int(currentMax)) + "\n"
        s += "found in: " + currentMaxFileName + "\n"
        self.clearText()
        self.infoText.config(state=NORMAL)
        self.infoText.insert(END, s, "center")
        self.infoText.config(state=DISABLED)


    def drawVertices(self):
        if self.vertexCount == 0: return
        x, y = self.center[0], self.center[1]
        #self.canvas.create_oval(x, y, x + self.pointSize, y + self.pointSize, fill="green", width=1)
        for p in self.vertexLst:
            x, y = (p[0]+self.xOffset) * self.scale, (p[1]+self.yOffset) * self.scale
            self.canvas.create_oval(x, y, x + self.pointSize, y + self.pointSize, fill="red", width=1)


    def drawPolygon(self):
        if self.vertexCount == 0: return
        lst = []
        for p in self.vertexLst:
            lst.append((p[0]+self.xOffset) * self.scale + self.pointSize / 2)
            lst.append((p[1]+self.yOffset) * self.scale + self.pointSize / 2)
        lst = tuple(lst)
        polygon = self.canvas.create_polygon(lst, fill="yellow", outline="blue", width=self.lineWidth)
        self.canvas.tag_lower(polygon)

    def initialize(self):
        self.getVertices()
        self.drawVertices()

    def clear(self):
        self.canvas.delete("all")
        self.clearText()
        self.vertexLst = []
        self.vertexCount = 0
        self.polygon = Polygon_2()
        self.xOffset = 0
        self.yOffset = 0

    def clearText(self):
        self.infoText.config(state=NORMAL)
        self.infoText.delete("1.0", END)
        self.infoText.config(state=DISABLED)

    def onClosing(self):
        pickle.dump(self.d, open("maxAreas.db.pickle", "wb"))
        root.destroy()

    def createWidgets(self):
        self.winfo_toplevel().title("Polygon")

        self.inputLabel = Label(self, text = "File name:")
        self.inputLabel.pack()

        self.inputBox = Entry(self, justify='center')
        self.inputBox.insert(END, "input.txt")

        self.inputBox.pack()

        self.initializeButton = Button(self)
        self.initializeButton["text"] = "Initialize"
        self.initializeButton["command"] = self.initialize

        self.initializeButton.pack()

        self.drawButton = Button(self)
        self.drawButton["text"] = "Draw Polygon"
        self.drawButton["command"] = self.drawPolygon

        self.drawButton.pack()

        self.infoButton = Button(self)
        self.infoButton["text"] = "Information"
        self.infoButton["command"] = self.displayInfo

        self.infoButton.pack()

        self.infoText = Text(self, height=4, width=70)
        self.infoText.pack()
        self.infoText.insert(END, "", "center")
        self.infoText.tag_configure("center", justify='center')
        self.infoText.config(state=DISABLED)

        self.clearButton = Button(self)
        self.clearButton["text"] = "Clear"
        self.clearButton["command"] = self.clear

        self.clearButton.pack()

        self.canvas = Canvas(self, height=600, width=800, bg = "white")
        self.canvas.pack(fill=BOTH, expand=1)

    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.pack()
        self.createWidgets()

root = Tk()
app = Polygon(master=root)
root.protocol("WM_DELETE_WINDOW", app.onClosing)
app.mainloop()