import math
import shapely
import numpy as np
class Point():
    def __init__(self,x,y):
        self.x = x
        self.y = y

def PolyArea(x,y):
    return 0.5*np.abs(np.dot(x,np.roll(y,1))-np.dot(y,np.roll(x,1)))

def GetAreaOfPolyGonbyVector(points):
    # 基于向量叉乘计算多边形面积
    area = 0
    if(len(points)<3):

         raise Exception("error")

    for i in range(0,len(points)-1):
        p1 = points[i]
        p2 = points[i + 1]

        triArea = (p1.x*p2.y - p2.x*p1.y)/2
        area += triArea
    return abs(area)

def main():

    points = []

  #  x = [17.105219999999832,19.51894999999986,13.591930000000474,18.523999999999816]
   # y = [-108.22027453314553,-108.24582195502866,-105.45265321230615,-108.48080901967194]
    x=[9.80969000000016,21.318769999999958,40.90459999999979,24.40625000000047,18.163750000000363,18.16914000000004,18.174419999999998,18.688669999999874,18.262089999999844 ]
    y=[-109.21506784387132,-108.94022031255818,-109.41133507675173,-107.27859183850549,-109.84731041492198,-109.84779092393053,-109.84782563019218,-109.80716107151578,-109.79973403513048]
    x = [1,2,0,2]
    y = [0,0,0,1]
    for index in range(len(x)):
        points.append(Point(x[index],y[index]))

    area = GetAreaOfPolyGonbyVector(points)
    print(area)
   # print(math.ceil(area))

    dst = PolyArea(x,y)
    print(dst)

if __name__ == '__main__':
    main()
    print("OK")