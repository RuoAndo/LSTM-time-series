from datetime import datetime

#import matplotlib.pyplot as plt
#import matplotlib.dates as mdates

import matplotlib.pyplot as plt
import matplotlib.dates as md
import dateutil

import sys
args = sys.argv

x = []
y = []
z = []
w = []

#f = open('1.csv', 'r')
f = open(args[1], 'r')

line = f.readline()
line = f.readline()

while line:
    tmp = line.split(",")
    dt_object = datetime.fromtimestamp(int(tmp[0]))

    x.append(str(dt_object))
    y.append(float(str(str(tmp[1]).replace('\n',''))))
    z.append(float(str(str(tmp[2]).replace('\n',''))))
    w.append(float(str(str(tmp[3]).replace('\n',''))))
    line = f.readline()    
f.close()

counter = 0
for i in x:
    print(x[counter] + "," + str(y[counter]))
    counter = counter + 1

dates = [dateutil.parser.parse(s) for s in x]

plt.subplots_adjust(bottom=0.1)

ax=plt.gca()
#xfmt = md.DateFormatter('%Y-%m-%d %H:%M')
days = md.DayLocator()
daysFmt = md.DateFormatter('%m/%d %H:%M')
ax.xaxis.set_major_locator(days)
ax.xaxis.set_major_formatter(daysFmt)
#plt.plot(dates[0:10],plt_data[0:10], "o-")

plt.subplot(3, 1, 1)
plt.title("real traffic")
plt.xticks( rotation= 20 )
plt.plot(dates,y)
plt.subplots_adjust(hspace=1.3)

plt.subplot(3, 1, 2)
plt.title("predicted")
plt.xticks( rotation= 20 )
plt.plot(dates,z)
plt.subplots_adjust(hspace=1.3)

plt.subplot(3, 1, 3)
plt.title("MSE")
plt.xticks( rotation= 20 )
plt.plot(dates,w)
plt.subplots_adjust(hspace=1.3)

plt.show()
