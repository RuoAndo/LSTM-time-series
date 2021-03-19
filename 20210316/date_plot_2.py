import pandas as pd
import matplotlib.pyplot as plt
import datetime
import sys

dtypes = {'date': 'str', 'count': 'int'}
parse_dates = ['date']
data = pd.read_csv(sys.argv[1], index_col=0, dtype=dtypes, parse_dates=parse_dates)

data.plot()
#plt.savefig("Graph03.png")
plt.show()
