import pandas as pd
import matplotlib.pyplot as plt
import datetime

dtypes = {'date': 'str', 'us': 'int', 'world': 'int', 'uk':int}
parse_dates = ['date']
data = pd.read_csv("1.csv", index_col=0, dtype=dtypes, parse_dates=parse_dates)

data.plot()
#plt.savefig("Graph03.png")
plt.show()
