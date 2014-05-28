import sys, serial
import numpy as np
from time import sleep
from collections import deque
from matplotlib import pyplot as plt

DATA_LEN = 6
# class that holds analog data for N samples
class AnalogData:
  # constr
  def __init__(self, maxLen):
    self.ax = deque([0.0]*maxLen)
    self.ay = deque([0.0]*maxLen)
    self.az = deque([0.0]*maxLen)
    self.gx = deque([0.0]*maxLen)
    self.gy = deque([0.0]*maxLen)
    self.gz = deque([0.0]*maxLen)
    self.maxLen = maxLen

  # ring buffer
  def addToBuf(self, buf, val):
    if len(buf) < self.maxLen:
      buf.append(val)
    else:
      buf.pop()
      buf.appendleft(val)

  # add data
  def add(self, data):
    assert(len(data) == DATA_LEN)
    self.addToBuf(self.ax, data[0])
    self.addToBuf(self.ay, data[1])
    self.addToBuf(self.az, data[2])
    self.addToBuf(self.gx, data[3])
    self.addToBuf(self.gy, data[4])
    self.addToBuf(self.gz, data[5])

# plot class
class AnalogPlot:
  # constr
  def __init__(self, analogData):
    # set plot to animated
    plt.ion()
    plt.subplot(3, 2, 1)
    self.ayline_ax, = plt.plot(analogData.ax)
    plt.title('AX')
    plt.ylim([-20000, 20000])
    plt.subplot(3, 2, 3)
    self.ayline_ay, = plt.plot(analogData.ay)
    plt.title('AY')
    plt.ylim([-20000, 20000])
    plt.subplot(3, 2, 5)
    self.ayline_az, = plt.plot(analogData.az)
    plt.title('AZ')
    plt.ylim([-20000, 20000])
    plt.subplot(3, 2, 2)
    self.ayline_gx, = plt.plot(analogData.gx)
    plt.title('GX')
    plt.ylim([-20000, 20000])
    plt.subplot(3, 2, 4)
    self.ayline_gy, = plt.plot(analogData.gy)
    plt.title('GY')
    plt.ylim([-20000, 20000])
    plt.subplot(3, 2, 6)
    self.ayline_gz, = plt.plot(analogData.gz)
    plt.title('GZ')
    plt.ylim([-20000, 20000])

  # update plot
  def update(self, analogData):
    self.ayline_ax.set_ydata(analogData.ax)
    self.ayline_ay.set_ydata(analogData.ay)
    self.ayline_az.set_ydata(analogData.az)
    self.ayline_gx.set_ydata(analogData.gx)
    self.ayline_gy.set_ydata(analogData.gy)
    self.ayline_gz.set_ydata(analogData.gz)
    plt.draw()

# main() function
def main():


  # plot parameters
  # analogData = AnalogData(100)
  # analogPlot = AnalogPlot(analogData)
  data = [0,0,0,0,0,0]
  # print "here"

  # open serial port
  ser = serial.Serial(13, 9600)
  while True:
    try:
      read_line = ser.readline()
      # print read_line
      # data = [float(val) for val in line.split()]
      if 'ax' in read_line:
        try:
            data[0] = int(read_line[4:11].strip(' '))
        except Exception, e:
          print e
      elif 'ay' in read_line:
        try:
            data[1] = int(read_line[5:11].strip(' '))
        except Exception, e:
          print e
      elif 'az' in read_line:
        try:
            data[2] = int(read_line[5:11].strip(' '))
        except Exception, e:
          print e
      elif 'gx' in read_line:
        try:
            data[3] = int(read_line[5:11].strip(' '))
        except Exception, e:
          print e
      elif 'gy' in read_line:
        try:
            data[4] = int(read_line[5:11].strip(' '))
        except Exception, e:
          print e
      elif 'gz' in read_line:
        try:
            data[5] = int(read_line[5:11].strip(' '))
        except Exception, e:
          print e
      print data
      # analogData.add(data)
      # analogPlot.update(analogData)
    except KeyboardInterrupt:
      print 'exiting'
      break
    # if ser.inWaiting() > 1000:
    #   ser.flushInput()
  # close serial
  ser.flush()
  ser.close()

# call main
if __name__ == '__main__':
  main()
