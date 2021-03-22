import numpy as np
import matplotlib.pyplot as plt
import sys
import scipy.stats

data_set = np.loadtxt(
    fname=sys.argv[1],
    dtype="float",
    delimiter=",",
)

#散布図を描画 → scatterを使用する
#1行ずつ取り出して描画
#plt.scatter(x座標の値, y座標の値)

a = []
b = []

for data in data_set:
    #print(data[0])

    a.append(data[0])
    b.append(data[1])

plt.scatter(scipy.stats.zscore(a), scipy.stats.zscore(b))

plt.title("sourceIP,sourcePort,destIP,destPort")
plt.xlabel("counts")
plt.ylabel("bytes")
plt.grid()

plt.show()

