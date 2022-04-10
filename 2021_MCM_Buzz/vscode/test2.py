from shapely.geometry import Polygon
import numpy as np

def PolyArea(x,y):
    return 0.5*np.abs(np.dot(x,np.roll(y,1))-np.dot(y,np.roll(x,1)))

coords = np.random.rand(6, 2)
x, y = coords[:, 0], coords[:, 1]

print(x)
print(y)

dst = PolyArea(x,y)-Polygon().area
print(coords)

    map = Basemap()
    map.drawcoastlines()

    map = Basemap(projection='stere',lat_0=90,lon_0=-105,\
                llcrnrlat=23.41 ,urcrnrlat=45.44,\
                llcrnrlon=-118.67,urcrnrlon=-64.52,\
                rsphere=6371200.,resolution='l',area_thresh=10000)
    map.drawmapboundary()   # 绘制边界
    #map.fillcontinents()   # 填充大陆，发现填充之后无法显示散点图，应该是被覆盖了
    map.drawstates()        # 绘制州
    map.drawcoastlines()    # 绘制海岸线
    map.drawcountries()     # 绘制国家
    map.drawcounties()      # 绘制县
    map.drawcoastlines()


        map = Basemap(projection='stere',lat_0=90,lon_0=-105,\
                llcrnrlat=23.41 ,urcrnrlat=45.44,\
                llcrnrlon=-118.67,urcrnrlon=-64.52,\
                rsphere=6371200.,resolution='l',area_thresh=10000)



#"python.pythonPath": "F:\\programFile_project\\Anaconda3\\envs\\mcm212\\python.exe"
import sys
import getopt
import xlrd
from datetime import date,datetime
import numpy as np 
import cv2 as cv 

import pandas as pd
import geopandas as gpd
import matplotlib.pyplot as plt
from mpl_toolkits.basemap import Basemap
from shapely.geometry import Polygon
from shapely.geometry import LineString,Point

datac_path = 'E:\workspace_project\mcm\data\mcm_all.xlsx'
img_path = 'E:\workspace_project\mcm\data\img_data'
class Usage(Exception):
    def __init__(self, msg):
        self.msg = msg

def map_make(list_p):


    map = Basemap(llcrnrlon=-124.,llcrnrlat=48.,urcrnrlon=-121.,urcrnrlat=50., lat_0=40, lon_0=-110,resolution='l', area_thresh=5000.0)
    map.drawmapboundary()   # 绘制边界
    map.drawcountries()     # 绘制国家
    map.drawcoastlines()    # 绘制海岸线
    map.drawmapboundary(fill_color='skyblue')
    map.fillcontinents(color='white', lake_color='lightskyblue')

#    map.fillcontinents(color='white', lake_color='lightskyblue')
#    map.drawmapboundary(fill_color='skyblue')

#  list_p = np.mat(list_p)
    #list_p.asnumpy()
    x = list_p[0:, 1]
    y = list_p[0:, 2]
    x = np.squeeze(x)
    y = np.squeeze(y)  
    (list_rowNum, list_colNum) = np.array(list_p).shape

    xx = np.zeros(list_rowNum)
    yy = np.zeros(list_rowNum)

    for i in range(list_rowNum):
        xx[i] = x[i]
        yy[i] = y[i]

    xx19 = xx[0:5]
    xx20 = xx[5:]
    yy19 = yy[0:5]
    yy20 = yy[5:]
    print(xx19)
    map.scatter(yy19, xx19, marker = '.', color = 'm')
    map.scatter(yy20, xx20, marker = '.', color = 'b')

    plt.show()

def data_excel_read(path):
    wb = xlrd.open_workbook(path)
    print(wb.sheet_names())#获取所有表格名字
    data_sheet=wb.sheet_by_index(0)
    data_sheet1=wb.sheets()[0]
    rowNum = data_sheet.nrows
    colNum = data_sheet.ncols
    print("rowNum =", rowNum, end='')
    print(" colNum =", colNum)
    list = []
    for i in range(rowNum):
        rowlist = []
        for j in range(colNum):
            rowlist.append(data_sheet.cell_value(i,j))
        list.append(rowlist)
    return list

def data_excel_extract(data):
    (rowNum, colNum) = np.array(data).shape
    Positive_ID = "Positive ID"
    list = []
    for i in range(rowNum):
        if data[i][3]==Positive_ID:
            list.append(data[i])
    (list_rowNum, list_colNum) = np.array(list).shape
     
    #for i in range(list_rowNum):
    #    for j in range(list_colNum):
    #        print(list[i][j], end="")
    #    print()
    #print(list_rowNum, list_colNum)
    list=np.array(list)
    list_p = list[0:,[1, 6, 7]]
    list_p2 = sorted(list_p, key = lambda x:x[0])
    list_p2 = np.array(list_p2)    
    list_p2019 = list_p2[0:5,[1, 2]]
    list_p2020 = list_p2[5:,[1, 2]]

    print(list_p)
    print(list_p2)
    print(list_p2019)
    print(list_p2020)
 #   print(list[1,1])
    print(Polygon(list_p2019).area)    
    print(Polygon(list_p2020).area)    

    return list_p2
    
    #data_img_extract(list[1][8])

def data_img_extract(img_name):
    img = cv.imread('E:\workspace_project\mcm\data\img_data' + f'\{img_name}')
    cv.imshow("img", img)

def main(argv=None):
    if argv is None:
        argv = sys.argv
    try:
        try:
            opts, args = getopt.getopt(argv[1:], "h", ["help"])
        except getopt.error as msg:
            raise Usage(msg)
    except Usage as err:
        print >>sys.stderr, err.msg
        print >>sys.stderr, "for help use --help"
        return 2
#######################################################################
    print("ready Go!!!")
    datac = data_excel_read(datac_path)
    print(datac[1][3])
    list_p = data_excel_extract(datac)
    map_make(list_p)

######################################################################
    cv.waitKey(0)
    cv.destroyAllWindows()

if __name__ == "__main__":
    sys.exit(main())
    