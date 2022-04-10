#"python.pythonPath": "F:\\programFile_project\\Anaconda3\\envs\\mcm212\\python.exe"
import sys
import getopt
import xlrd
from datetime import date,datetime
import numpy as np 
import cv2 as cv 
import argparse
import imutils
import math

import pandas as pd
import geopandas as gpd
import matplotlib.pyplot as plt
from mpl_toolkits.basemap import Basemap
from shapely.geometry import Polygon
from shapely.geometry import LineString,Point

import pickle
datac_path = 'E:\workspace_project\mcm\data\mcm_all.xlsx'
img_path = 'E:\workspace_project\mcm\data\img_data'
class Usage(Exception):
    def __init__(self, msg):
        self.msg = msg

def save_variable(v,filename):
  f=open(filename,'wb')
  pickle.dump(v,f)
  f.close()
  return filename

def load_variavle(filename):
  f=open(filename,'rb')
  r=pickle.load(f)
  f.close()
  return r

def template_demo(img):
    imshow("demo",img)

def erode_img(img): #腐蚀
    print(img.shape)
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    ret, binary = cv.threshold(gray, 0, 255, cv.THRESH_BINARY_INV | cv.THRESH_OTSU)
    #cv.imshow("binary", binary)
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (15,15))
    dst = cv.erode(binary, kernel)
    cv.imshow("erode_demo", dst)
    return dst

def dilate_img(img): #膨胀
    print(img.shape)
#    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
#    ret, binary = cv.threshold(gray, 0, 255, cv.THRESH_BINARY_INV | cv.THRESH_OTSU)
    #cv.imshow("binary", binary)
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (15,15))
    dst = cv.dilate(img, kernel)
    cv.imshow("erode_demo", dst)
    return dst

def open_img(img):
    #print(img.shape)
    #img_gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    #ret, binary = cv.threshold(img_gray, 0, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)
    #cv.imshow("img_binary", binary)
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (5,5))
    dst = cv.morphologyEx(img, cv.MORPH_OPEN, kernel)
    #cv.imshow("img_open",dst)
    return dst

def close_img(img):
    print(img.shape)
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    ret, binary = cv.threshold(gray, 0, 255, cv.THRESH_BINARY_INV | cv.THRESH_OTSU)
    #cv.imshow("binary", binary)
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (20,20))
    dst = cv.morphologyEx(binary, cv.MORPH_CLOSE, kernel)
    cv.imshow("close-demo",binary)
    return dst

def openAndClose_img(img):
    #kernel = cv.getStructuringElement(cv.MORPH_RECT, (2,2))
    #dst = cv.morphologyEx(img, cv.MORPH_OPEN, kernel)
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (5,5))
    dst = cv.morphologyEx(img, cv.MORPH_CLOSE, kernel)
    #dst = cv.morphologyEx(dst, cv.MORPH_CLOSE, kernel)
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (5,5))
    dst = cv.morphologyEx(dst, cv.MORPH_OPEN, kernel)
    dst = cv.morphologyEx(dst, cv.MORPH_CLOSE, kernel)
    dst = cv.morphologyEx(dst, cv.MORPH_CLOSE, kernel)

    return dst

def find_rectangle(contour):
    """ 寻找矩形轮廓 """
    y, x = [], []
    for p in contour:
        y.append(p[0][0])
        x.append(p[0][1])
    return [min(y), min(x), max(y), max(x)]

def find_waves(threshold, histogram):
    """ 根据设定的阈值和图片直方图，找出波峰，用于分隔字符 """
    up_point = -1    # 上升点
    is_peak = False
    if histogram[0] > threshold:
        up_point = 0
        is_peak = True
    wave_peaks = []
    for i, x in enumerate(histogram):
        if is_peak and x < threshold:
            if i - up_point > 2:
                is_peak = False
                wave_peaks.append((up_point, i))
        elif not is_peak and x >= threshold:
            is_peak = True
            up_point = i
    if is_peak and up_point != -1 and i - up_point > 4:
        wave_peaks.append((up_point, i))
    return wave_peaks

def remove_upanddown_border(img):
    """ 去除上下无用的边缘部分，确定上下边界 """
    plate_gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    ret, plate_binary_img = cv.threshold(plate_gray, 0, 255, cv.THRESH_BINARY + cv.THRESH_OTSU)
    row_histogram = np.sum(plate_binary_img, axis=1)    # 数组的每一行求和
    row_min = np.min(row_histogram)
    row_average = np.sum(row_histogram) / plate_binary_img.shape[0]
    row_threshold = (row_min + row_average) / 2
    wave_peaks = find_waves(row_threshold, row_histogram)
    # 挑选跨度最大的波峰
    wave_span = 0.0
    selected_wave = []
    for wave_peak in wave_peaks:
        span = wave_peak[1] - wave_peak[0]
        if span > wave_span:
            wave_span = span
            selected_wave = wave_peak
    plate_binary_img = plate_binary_img[selected_wave[0]:selected_wave[1], :]
    return  plate_binary_img

def find_end(start, arg, black, white, width, black_max, white_max):
    end = start + 1
    for m in range(start + 1, width - 1):
        if (black[m] if arg else white[m]) > (0.95*black_max if arg else 0.95*white_max):
            end = m
            break
    return end

def find_license(img,re_img):
    contours, hierarchy = cv.findContours(re_img, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_NONE)
    #img_license = cv.drawContours(img, contours, -1, (256,0,0), 3)
    #for cnt in range(len(hierarchy)):
    #    epsilon = 0.1*cv.arcLength(int(cnt),True)
    #    print(epsilon)
    #    approx = cv.approxPolyDP(cnt,epsilon,True)
    img_license0 = cv.drawContours(img, contours, -1, (0,255,0), 9)
    cv.imshow("img_license0",img_license0)
    for cnt in range(len(contours)):
        #epsilon = 0.001*cv.arcLength(contours[cnt],True)
        #print(epsilon)
        k = cv.isContourConvex(contours[cnt])
        #find_hull(len(contours))
        if(k==False):
            #hull = cv.cornerHarris(contours[cnt])
            x,y,w,h = cv.boundingRect(contours[cnt])
            img_license = cv.rectangle(img,(x,y),(x+w,y+h),(0,0,255),2)
            print("Cnt is %s\n"%cnt)
            print(contours[cnt])
            print("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n")

    print("**************************************************************\n")
    print(len(contours))
    print("**************************************************************\n")
    print(contours)
    cv.imshow("img_license",img_license)
    return img_license

def cut_license(original, rect):
    """ 裁剪 """
    license_img = original[rect[1]:rect[3], rect[0]:rect[2]]
    return license_img

def Get_Background():
    capture = cv.VideoCapture(0)
    if capture.isOpened():
        ret_flag, frame = cap.read()
        cv.imshow("frame",frame)
    else:
        print("Get_photo is error!!!")
    return frame

def binary_img(img):
    ret_b, img_binarization = cv.threshold(img,0,255,cv.THRESH_BINARY+cv.THRESH_OTSU)
    #img_binarization = cv.adaptiveThreshold(gray, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY_INV, 25, 10)
    #   ret, binary01 = cv.threshold(gray, 150, 255, cv.THRESH_BINARY)
    #   dst = cv.medianBlur(binary_img)
    #cv.imshow("img_binarization",img_binarization)
    return img_binarization

def resize_img(img):
    imgInfo = img.shape
    height = imgInfo[0]
    width  = imgInfo[1]
    mode = imgInfo[2]
    height = int(height*1)
    width  = int(width*1)
    img_resized = cv.resize(img, (width,height), interpolation=cv.INTER_CUBIC)
    #cv.imshow("img_resize",img_binary)
    return img_resized

def hisEqu_img(img):
    imgInfo = img.shape
    height = imgInfo[0]
    width  = imgInfo[1]
    count =np.zeros(256,np.float)
    for i in range(0,height):
        for j in range(0,width):
            pixel = img[i,j]
            index = int(pixel)
            count[index] = count[index]+1
    for i in range(0,255):
        count[i] = count[i]/(height*width)
    x = np.linspace(0,255,256)
    y = count
    plt.bar(x,y,alpha=1,color='b')
    plt.show()
    #计算累积概率
    sum1 = float(0)
    for i in range(0,256):
        sum1 = sum1+count[i]
        count[i] = sum1
    #print(count)
    #计算映射表
    map1 = np.zeros(256,np.uint16)
    for i in range(0,256):
        map1[i] = np.uint16(count[i]*255)
    #映射
    for i in range(0,height):
        for j in range(0, width):
            pixel = img[i,j]
            img[i,j] = map1[pixel]
    #cv.imshow('map1',img)
    return img

def Recognition():
    capture = cv.VideoCapture(0)
    while(True):
        t1 = cv.getTickCount()
        ret_flag, frame = capture.read()
        w, h = frame.shape[0:2]
        if ret_flag == False:
            print("capture.read is error!!!")
            break
        cv.imshow("frame",frame)
        ##########################################################################
        img_resize = resize_img(frame)
        cv.imshow("img_resize",img_resize)
        img_gray = cv.cvtColor(img_resize, cv.COLOR_BGR2GRAY)
        cv.imshow("img_gray",img_gray)

        img_gaussian = cv.GaussianBlur(img_gray, (10,10), 0)
        cv.imshow("img_gaussian",img_gaussian)
        img_opened = open_img(img_gaussian)
        cv.imshow("img_opened",img_opened)

        ##########################################################################
        t2 = cv.getTickCount()
        fps = 1/((t2-t1)/cv.getTickFrequency())
        print("fps=%s"%fps)
        c = cv.waitKey(20)
        if c==27:
            break

def map_make(list_p):

    map = Basemap(llcrnrlon=-124.,llcrnrlat=48.,urcrnrlon=-121.,urcrnrlat=50., lat_0=40, lon_0=-110,resolution='l', area_thresh=5000.0)
    map.drawmapboundary()   # 绘制边界
    map.drawcountries()     # 绘制国家
    map.drawcoastlines()    # 绘制海岸线
#    map.drawmapboundary(fill_color='skyblue')
#    map.fillcontinents(color='white', lake_color='lightskyblue')

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
  #   map.scatter(yy19, xx19, marker = '.', color = 'm')
    map.scatter(yy20, xx20, marker = '.', color = 'b')

    plt.show()
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

def get_s():
    xy =[
        [ 17.105219999999832, -108.22027453314553],
        [ 19.51894999999986 , -108.24582195502866],
        [ 13.591930000000474, -105.45265321230615],
        [ 18.523999999999816, -108.48080901967194]
    ]
    xy2=[
       [9.80969000000016   ,-109.21506784387132],
       [21.318769999999958 ,-108.94022031255818],
       [40.90459999999979  ,-109.41133507675173],
       [24.40625000000047  ,-107.27859183850549],
       [18.163750000000363 ,-109.84731041492198],
       [18.16914000000004  ,-109.84779092393053],
       [18.174419999999998 ,-109.84782563019218],
       [18.688669999999874 ,-109.80716107151578],
       [18.262089999999844 ,-109.79973403513048]
    ]

    x=[9.80969000000016,21.318769999999958,40.90459999999979,24.40625000000047,18.163750000000363,18.16914000000004,18.174419999999998,18.688669999999874,18.262089999999844 ]
    y=[-109.21506784387132,-108.94022031255818,-109.41133507675173,-107.27859183850549,-109.84731041492198,-109.84779092393053,-109.84782563019218,-109.80716107151578,-109.79973403513048]
    points = []

    dst1 = Polygon(xy).area
    dst2 = Polygon(xy2).area

    print(dst1)
    print(dst2)

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
 #   print(list[0])
    list=np.array(list)
    list_p = list[0:,[1, 6, 7, 8]]
    list_p2 = sorted(list_p, key = lambda x:x[0])
    list_p2 = np.array(list_p2)    
    list_p2019 = list_p2[0:5,[1, 2]]
    list_p2020 = list_p2[5:,[1, 2]]
 #   print(list_p2)
    return list_p2
    #data_img_extract(list[1][8])

def data_img_extract(img_name):
    img = cv.imread('E:\workspace_project\mcm\data\img_data' + f'\{img_name}')
    #cv.imshow("img", img)
    return img
all_end_g = np.zeros((4500,1))
def dsp_imgAll(data):
    (rowNum, colNum) = np.array(data).shape
    all_end_g = np.zeros((rowNum,1))
    for i in np.arange(1,rowNum):
        img = data_img_extract(data[i][3])
        #img = data_img_extract("ATT422_image001.jpg")
        all_end_g[i,0] = dsp(img)
        print(all_end_g[i,0])


def calculate(hist1, hist2):
    # 灰度直方图算法
    # 计算直方图的重合度
    degree = 0
    for i in range(len(hist1)):
        if hist1[i] != hist2[i]:
            degree = degree + \
                (1 - abs(hist1[i] - hist2[i]) / max(hist1[i], hist2[i]))
        else:
            degree = degree + 1
    degree = degree / len(hist1)
    return degree

def calculate2(histb, histg, histr, z_histb, z_histg, z_histr):
    Similar1 = calculate(histb, z_histb)
    Similar2 = calculate(histg, z_histg)
    Similar3 = calculate(histr, z_histr)
    Similar_all = Similar1 + Similar2 + Similar3
    return Similar_all/3



def dsp(src):
    try:
        src4 = src.copy()
    except:
        pass
        return 0
    (h,w) = src.shape[0:2]
    #cv.imshow("src", src)
    img_blur=cv.boxFilter(src,-1,(3,3))
    #cv.imshow("img_blur", img_blur)
    hsv = cv.cvtColor(img_blur, cv.COLOR_BGR2HSV)
    low_hsv = np.array([11,130,43])
    high_hsv = np.array([34,255,255])
    img_y = cv.inRange(hsv,lowerb=low_hsv,upperb=high_hsv)
    img_open=open_img(img_y)
    img_open=open_img(img_open)
    img_open=open_img(img_open)
    cnts = cv.findContours(img_open.copy(), cv.RETR_EXTERNAL,
                            cv.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)
    # 遍历轮廓集
    cxy = np.zeros((len(cnts),4))
    i = 0
    for c in cnts:
        # 计算轮廓区域的图像矩。 在计算机视觉和图像处理中，图像矩通常用于表征图像中对象的形状。这些力矩捕获了形状的基本统计特性，包括对象的面积，质心（即，对象的中心（x，y）坐标），方向以及其他所需的特性。
        M = cv.moments(c)
        cX = int(M["m10"] / M["m00"])
        cY = int(M["m01"] / M["m00"])
        # 在图像上绘制轮廓及中心
        cxy[i,0:2] = [cX, cY]
        i = i + 1
        cv.drawContours(src, [c], -1, (0, 255, 0), 2)
        cv.circle(src, (cX, cY), 3, (255, 0, 255), -1)
        cv.putText(src, "c"+str(i), (cX - 20, cY - 20),
                    cv.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)
        # 展示图像
    src2 = src.copy()
    src3 = src.copy()

    #cv.imshow("dst", src)

#   len(cxy)
    jx_all = np.zeros((len(cxy), 5, 2))

    for i in np.arange(len(cxy)):
        dis = 0
        dis_min = 100000
        for j in np.arange(len(cxy)):
            if i != j:
                dis = np.sqrt((cxy[i,0]-cxy[j,0])**2 + (cxy[i,1] - cxy[j,1])**2)
                if dis_min > dis:
                    dis_min = dis
                    cxy[i, 2] = cxy[j, 0]
                    cxy[i, 3] = cxy[j, 1]
        (x1, y1, x2, y2) = (int(cxy[i, 0]),int(cxy[i, 1]),int(cxy[i, 2]),int(cxy[i, 3]))              
        cX = abs(cxy[i, 2] + cxy[i, 0])/2
        cY = abs(cxy[i, 3] + cxy[i, 1])/2
        cv.circle(src2, (int(cX), int(cY)), 3, (255, 0, 255), -1)
        cv.line(src2, (int(cxy[i, 0]), int(cxy[i, 1])), ( int(cxy[i, 2]), int(cxy[i, 3])), (0, 0, 255),1, 4)
        if (y1!=y2) & (x1!=x2):
            k1 = (x1-x2)/(y1-y2)
            k2 = -1/k1
        else:
            k1 = 0
            k2 = 0
        y1_list = np.arange(y1-200, y1+200, 1)
        y2_list = np.arange(y2-200, y2+200, 1)
        x1_list = y1_list*0
        x2_list = y2_list*0
        y1_list_len = len(x1_list)
        y2_list_len = len(x2_list)
        if (np.min(y1_list) < 0) | (np.max(y1_list) >= h):
            for m in range(y1_list_len):
                if y1_list[m] < 0:
                    y1_list[m] = 0
                if y1_list[m] >= w:
                    y1_list[m] = w-1
        if (np.min(y2_list) < 0) | (np.max(y2_list) >= h):
            for m in range(y2_list_len):
                if y2_list[m] < 0:
                    y2_list[m] = 0
                if y2_list[m] >= h:
                    y2_list[m] = h-1
        for m in range(y1_list_len):
            x1_list[m] = k2*(y1_list[m] - y1) + x1      
        for m in range(y2_list_len):
            x2_list[m] = k2*(y2_list[m] - y2) + x2

        if (x1_list[y1_list_len-1] - x1_list[0]) < 0:
            for n in range(y1_list_len-1):
                if (x1_list[n] < (w-1)) & (y1_list[n] > 0):
                    xy1_list_s = n
                    break
            for n in range(y1_list_len-1,0,-1):
                if (x1_list[n] > 0) & (y1_list[n] < (h-1)):
                    xy1_list_e = n
                    break
        else:
            for n in range(y1_list_len-1):
                if (x1_list[n] > 0) & (y1_list[n] > 0):
                    xy1_list_s = n
                    break
            for n in range(y1_list_len-1,0,-1):
                if (x1_list[n] < (w-1)) & (y1_list[n] < (h-1)):
                    xy1_list_e = n
                    break

        if (x1_list[y2_list_len-1] - x2_list[0]) < 0:
            for n in range(y2_list_len-1):
                if (x2_list[n] < (w-1)) & (y2_list[n] > 0):
                    xy2_list_s = n
                    break
            for n in range(y2_list_len-1,0,-1):
                if (x2_list[n] > 0) & (y2_list[n] < (h-1)):
                    xy2_list_e = n
                    break
        else:
            for n in range(y2_list_len-1):
                if (x2_list[n] > 0 ) & (y2_list[n] > 0):
                    xy2_list_s = n
                    break
            for n in range(y1_list_len-1,0,-1):
                if (x2_list[n] < (w-1)) & (y2_list[n] < (h-1)):
                    xy2_list_e = n
                    break   
        try:           
            if k2 == 0:
                continue
            else:              
                cv.line(src2, ( int(x1_list[xy1_list_s]), int(y1_list[xy1_list_s])), (int(x1_list[xy2_list_e]), int(y1_list[xy2_list_e])), (255, 0, 0),2, 4)
                cv.line(src2, ( int(x2_list[xy2_list_s]), int(y2_list[xy2_list_s])), (int(x2_list[xy2_list_e]), int(y2_list[xy2_list_e])), (255, 0, 0),2, 4)
        except:
            pass
        try:
            y1_list_len_z=y1_list.tolist()
            y1_list_len_z = y1_list_len_z.index(y1)

            y2_list_len_z=y2_list.tolist()
            y2_list_len_z = y2_list_len_z.index(y2)
        except:
            pass
        try:
            for m in range(int(y1_list_len_z),xy1_list_s,-1):
                if img_open[y1_list[m], x1_list[m]] != 0:
                    pass
                else:
                    jx1 = [x1_list[m], y1_list[m]]
                    cv.circle(src2, (int(jx1[0]), int(jx1[1])), 3, (0, 0, 255), -1)
                    break
            
            for m in range(int(y1_list_len_z), xy1_list_e-1,1):
                if img_open[y1_list[m], x1_list[m]] != 0:
                    pass

                else:
                    jx2 = [x1_list[m],y1_list[m]]
                    cv.circle(src2, (int(jx2[0]), int(jx2[1])), 3, (0, 0, 255), -1)
                    break
        except:
            pass                
        try:
            for m in range(int(y2_list_len_z), xy2_list_s,-1):
                if img_open[y2_list[m], x2_list[m]] != 0:
                    pass
                else:
                    jx3 = [x2_list[m],y2_list[m]]
                    cv.circle(src2, (int(jx3[0]), int(jx3[1])), 3, (0, 0, 255), -1)
                    break
            for m in range(int(y2_list_len_z), xy2_list_e-1,1):
                if img_open[y2_list[m], x2_list[m]] != 0:
                    pass
                else:
                    jx4 = [x2_list[m],y2_list[m]]
                    cv.circle(src2, (int(jx4[0]), int(jx4[1])), 3, (0, 0, 255), -1)
                    break
        except:
            pass
        try:
            cv.line(src3, tuple(jx1), tuple(jx2), (0, 0, 255),2, 4)
            cv.line(src3, tuple(jx2), tuple(jx4), (0, 0, 255),2, 4)
            cv.line(src3, tuple(jx3), tuple(jx4), (0, 0, 255),2, 4)
            cv.line(src3, tuple(jx3), tuple(jx1), (0, 0, 255),2, 4)
        except:
            pass
        try:
            cXY = [cX, cY]
            jx_all[i,:,:] = [jx1,jx2,jx4,jx3,cXY]
        except:
            pass

    #cv.imshow("dst2",src2)
    #cv.imshow("dst3", src3)

    #hist2 = load_variavle('hist8.txt')
    grade = np.zeros(len(cxy))
    for l in np.arange(len(cxy)):
        pts = np.array([    [int(jx_all[l, 0, 0]), int(jx_all[l, 0, 1])],
                            [int(jx_all[l, 1, 0]), int(jx_all[l, 1, 1])],
                            [int(jx_all[l, 2, 0]), int(jx_all[l, 2, 1])],
                            [int(jx_all[l, 3, 0]), int(jx_all[l, 3, 1])]
                    ])
        pts = np.array([pts])
        mask = np.zeros(src4.shape[:2], np.uint8)
        cv.polylines(mask, pts, 1, 255)    # 描绘边缘
        cv.fillPoly(mask, pts, 255)        # 填充
        dst = cv.bitwise_and(src4, src4, mask=mask)
        bg = np.ones_like(src4, np.uint8) * 255
        cv.bitwise_not(bg, bg, mask=mask)  # bg的多边形区域为0，背景区域为255
        dst_white = bg + dst

        b, g, r = cv.split(dst_white)
        clahe = cv.createCLAHE(clipLimit=2.0, tileGridSize=(5, 5))
        b = clahe.apply(b)
        g = clahe.apply(g)
        r = clahe.apply(r)
        dst_white = cv.merge([b, g, r])

        histb = cv.calcHist([dst_white], [0], None, [255], [1, 255])
        histg = cv.calcHist([dst_white], [1], None, [255], [1, 255])
        histr = cv.calcHist([dst_white], [2], None, [255], [1, 255])
        hist1 = [histb, histg, histr]
        h = (histb[:].sum() + histg[:].sum() + histr[:].sum())
        try:
            k1 = histb[11:130].sum() + histg[130:255].sum() + histr[43: 255].sum()
            k2 = histb[0:11].sum() + histg[0:60].sum() + histr[0: 43].sum()
#            grade[l] = (10*(k1/k2)/h) / abs(k1/k2 - 1)
            grade[l] = (10*k1+k2)/h - abs(10*math.log10(k1/k2))

            pass
        except:
            return 0
        #grade[l] = calculate2(hist1[0], hist1[1], hist1[2], hist2[0], hist2[1], hist2[2])
        #if l== 8:
        #    save_variable(hist1,"hist"+str(l)+".txt")
        #plt.plot(histb, color="b")
        #plt.plot(histg, color="g")
        #plt.plot(histr, color="r")
        #plt.title("c"+str(l))   
        #cv.imshow("c"+str(l)+".jpg", dst_white)
        #plt.show()
    try:
        grade_end = sum(grade) / len(cxy)
    except:
        grade_end = 0
    '''
    low_hsv_red = np.array([0,0,255])
    high_hsv_red = np.array([0,0,255])
    img_r = cv.inRange(src4,lowerb=low_hsv_red,upperb=high_hsv_red)
    '''
    return grade_end



#   low_hsv  = np.array([11,130,43])
#   high_hsv = np.array([34,255,255])

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
    print("ready Go!!!")
    datac = data_excel_read(datac_path)
    list_p = data_excel_extract(datac)
    dsp_imgAll(list_p)
    cv.waitKey(0)
    cv.destroyAllWindows()
#    map_make(list_p)
#    get_s()

if __name__ == "__main__":
    sys.exit(main())
