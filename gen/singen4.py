# -*- coding: utf-8 -*-
from scipy import arange, sin
from scipy import pi as mpi
from matplotlib import pylab as pl

# ===============
#  サイン波を作る
# ===============
def make_sine(F, A, fs, sec = 2.):
    ret = A * sin(2. * mpi * F * arange(int(fs * sec)) / fs)
    return ret
  
# ========
#  テスト
# ========
F_default = 2.5
A_default = 1
fs_default = 125
def test():
    sine_wave = make_sine(F_default, A_default, fs_default)

    fig = pl.figure()
    fig.add_subplot(111)
    pl.plot(sine_wave)
    pl.xlim([0, len(sine_wave)])
    pl.show()

if __name__ == "__main__":
    test()
