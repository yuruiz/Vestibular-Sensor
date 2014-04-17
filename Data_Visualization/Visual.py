import numpy as np
import matplotlib.pyplot as plt

def get_peaks(x, y, n, x0=None, x1=None):
    if x0 is None:
        x0 = x[0]
    if x1 is None:
        x1 = x[-1]
    index0 = np.searchsorted(x, x0)
    index1 = np.searchsorted(x, x1, side="right")
    step = (index1 - index0) // n
    if step == 0:
        step = 1
    index1 += 2 * step
    if index0 < 0:
        index0 = 0
    if index1 > len(x) - 1:
        index1 = len(x) - 1
    x = x[index0:index1+1]
    y = y[index0:index1+1]
    y = y[:len(y)//step*step]
    yy = y.reshape(-1, step)
    index = np.c_[np.argmin(yy, axis=1), np.argmax(yy, axis=1)]
    index.sort(axis=1)
    index += np.arange(0, len(y), step).reshape(-1, 1)
    index = index.reshape(-1)
    return x[index], y[index]

def get_sample_data(N):
    x = np.linspace(-14, 14, N)
    y = np.sin(x) * x**3 * np.sin(100*x)
    np.random.seed(1)
    y[np.random.randint(0, N, 100)] += np.random.randint(100, 300, 100)
    return x, y

x, y = get_sample_data(1000000)

ax = plt.subplot(111)
line, = plt.plot(*get_peaks(x, y, 500))

ax = plt.gca()

def update_data(ax):
    x0, x1 = ax.get_xlim()
    line.set_data(*get_peaks(x, y, 500, x0, x1))
    ax.figure.canvas.draw_idle()

ax.callbacks.connect('xlim_changed', update_data)
plt.show()
