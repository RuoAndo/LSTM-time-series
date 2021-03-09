# -*- coding: utf-8 -*-

import wave
import numpy as np
from matplotlib import pylab as plt
import struct

import random

a = 1     #振幅
fs = 4000 #サンプリング周波数
f0 = 22 #周波数
sec = 5   #秒
 
swav=[]

for n in np.arange(fs * sec):
    s = a * np.sin(2.0 * np.pi * f0 * n / fs)
    s = s + random.random()
    swav.append(s)

#print(swav[0:1000])

for i in swav[0:2000]:
    print(i) 
              
#サイン波を表示
#plt.plot(swav[0:1000])
#plt.show()
 
