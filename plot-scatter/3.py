import numpy as np
import matplotlib.pyplot as plt
import sys
import scipy.stats

from matplotlib import pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d.axes3d import Axes3D

data_set = np.loadtxt(
    fname=sys.argv[1],
    dtype="float",
    delimiter=",",
)

#散布図を描画 → scatterを使用する
#1行ずつ取り出して描画
#plt.scatter(x座標の値, y座標の値)

x = []
y = []
z = []

for data in data_set:
    #print(data[0])

    x.append(data[0])
    y.append(data[1])
    z.append(data[2])

fig = plt.figure()
 
ax = Axes3D(fig)
 
ax.scatter(x, y, c='b')

plt.title("sourceIP,sourcePort,destIP,destPort")

ax.set_xlabel("bytes")
ax.set_ylabel("bytes_sent")
ax.set_zlabel("bytes_received")
#plt.zlabel("bytes_sent")
plt.grid()

plt.show()

#plt.scatter(scipy.stats.zscore(a), scipy.stats.zscore(b),)

#plt.title("sourceIP,sourcePort,destIP,destPort")
#plt.xlabel("counts")
#plt.ylabel("bytes")
#plt.grid()

#plt.show()

