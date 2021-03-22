import numpy as np
import matplotlib.pyplot as plt
import sys

data_set = np.loadtxt(
    fname=sys.argv[1],
    dtype="float",
    delimiter=",",
)

#散布図を描画 → scatterを使用する
#1行ずつ取り出して描画
#plt.scatter(x座標の値, y座標の値)
for data in data_set:
    plt.scatter(data[0], data[1])

plt.title("sourceIP,sourcePort,destIP,destPort")
plt.xlabel("counts")
plt.ylabel("bytes")
plt.grid()

plt.show()

