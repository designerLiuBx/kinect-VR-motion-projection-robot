# 1) y1=24710000./(1+(24710000/12.566-1)*exp(-c*(td-1)))
# 2) 地图上花圈
# 3）2019 几何图形 2020
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from pandas import DataFrame,Series
def logic(flip=1):
    x = np.linspace(1,3,100)
    c = 1.8133
    y = 24710000./(1+(24710000/12.566-1)*np.exp(-c*(x-1)))        
    plt.plot(x, y)
    plt.xlabel("years")
    plt.ylabel("km^2")
    xticklabes = ['2018', '2019', '2020']
  #  plt.xticks(x, xticklabes)
    plt.xticks([1, 2, 3], xticklabes )

logic()
plt.show()