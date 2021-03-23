import pandas as pd
df = pd.read_csv("month.csv", header=None, names=["time","counts","span"])

print(df)
import datetime
df['time'] = pd.to_datetime(df['time'].astype(int), unit='s')
print(df['time'])

import matplotlib.pyplot as plt
import matplotlib.dates as md
import dateutil

from statsmodels.tsa.arima_model import ARIMA
from sklearn.metrics import mean_squared_error

Q = df['counts']

P = Q.dropna()

size = int(len(P) * 0.5)
train, test = P[0:size], P[size:len(P)]
history = [p for p in train]
predictions = list()

size = int(len(P) * 0.5)
train, test = P[0:size], P[size:len(P)]
history = [p for p in train]
predictions = list()

a = []
for i in P:
    a.append(i)
    
P = []
for i in a:
    P.append(i)
    
size = int(len(P) * 0.66)
train, test = P[0:size], P[size:len(P)]
history = [p for p in train]
predictions = list()

for t in range(len(test)):
	model = ARIMA(history, order=(5,1,0))
	model_fit = model.fit(disp=0)
	output = model_fit.forecast()
	yhat = output[0]
	predictions.append(yhat)
	obs = test[t]
	history.append(obs)
	print('predicted=%f, expected=%f' % (yhat, obs))

import numpy as np

err = []
counter = 0 
for i in predictions:
        err.append(int(test[counter]-i))
        counter = counter + 1
                                      
error = mean_squared_error(test, predictions)

plt.subplots_adjust(wspace=0.4, hspace=0.6)

plt.subplot(3, 1, 1)
plt.title("traffic")
plt.plot(test)

plt.subplot(3, 1, 2)
plt.title("Predicted")
plt.plot(predictions)

plt.subplot(3, 1, 3)
plt.title("Error (MSE)")
plt.plot(err)

#pyplot.plot(test)
#pyplot.plot(predictions, color='red')
plt.show()
