import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

if __name__ == '__main__':
    fname = "1.csv"
    df = pd.read_csv(fname,header=None, names=['value'], index_col=0)
    df.index = pd.to_datetime(df.index)
    # print(df)

    fig, ax = plt.subplots()
    ax.xaxis.set_major_formatter(mdates.DateFormatter('%m/%d\n%H:%M'))

    plt.plot(df.index, df['value'])
    plt.savefig(f"{fname}.png")
