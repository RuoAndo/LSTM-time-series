from pandas import read_csv
from pandas import datetime
#from matplotlib import pyplot
import matplotlib.pyplot as plt
import numpy as np

from statsmodels.tsa.arima_model import ARIMA
from sklearn.metrics import mean_squared_error

def parser(p):
	return datetime.strptime(p,'%Y/%m/%d')

series = read_csv('4.csv', header=0, parse_dates=[0], index_col=0, squeeze=True, date_parser=parser)
P = series.values

print(P)

#size = int(len(P) * 0.66)
size = int(len(P) * 0.5)
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

err = []
counter = 0 
for i in predictions:
        err.append(int(test[counter]-i))
        counter = counter + 1
                   
                   
error = mean_squared_error(test, predictions)
print('Test MSE: %.3f' % error)

print(test)

err2 = np.array(err)
print(err2)

# plot

plt.subplots_adjust(wspace=0.4, hspace=0.6)


plt.subplot(3, 1, 1)
plt.title("The number of daily deaths")
plt.plot(test)

plt.subplot(3, 1, 2)
plt.title("Predicted")
plt.plot(predictions)

plt.subplot(3, 1, 3)
plt.title("Error")
plt.plot(err)

#pyplot.plot(test)
#pyplot.plot(predictions, color='red')
plt.show()
