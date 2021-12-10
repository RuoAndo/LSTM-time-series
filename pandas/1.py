import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt

import sys
args = sys.argv 

df = pd.read_csv('wdbc.data', index_col=0)

print(len(df))
print(len(df.columns))

dataExchange = df.iloc[:, [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,0]]
print(dataExchange)

dataExchange.to_csv('tmp.csv')