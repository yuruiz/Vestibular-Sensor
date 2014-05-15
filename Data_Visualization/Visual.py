import pylab
import serial

from pylab import *

port = serial.Serial(11, 9600)

xAchse=pylab.arange(0,100,1)
yAchse=pylab.array([0]*100)

fig = pylab.figure(1)
ax = fig.add_subplot(111)
ax.grid(True)
ax.set_title("Realtime Waveform Plot")
ax.set_xlabel("Time")
ax.set_ylabel("Amplitude")
ax.axis([0,100,-20000,20000])
line1=ax.plot(xAchse,yAchse,'-')

manager = pylab.get_current_fig_manager()

values=[]
values = [0 for x in range(100)]

Ta=0.01
fa=1.0/Ta
fcos=3.5

Konstant=cos(2*pi*fcos*Ta)
T0=1.0
T1=Konstant

def getSerialData():
    while 1 > 0:
          read_line = ''
          read_char = port.read()
          if read_char != '\n':
              read_line += read_char
          else:
            if read_line[1:3] == 'ax':
                try:
                    return int(line[5:10].strip(' '))
                except Exception, e:
                    print e
            read_line = ''

def SinwaveformGenerator(arg):
  global values,T1,Konstant,T0
  ohmegaCos=arccos(T1)/Ta
  print "fcos=", ohmegaCos/(2*pi), "Hz"

  Tnext=((Konstant*T1)*2)-T0
  if len(values)%100>70:
    values.append(random()*2-1)
  else:
    values.append(getSerialData())
  T0=T1
  T1=Tnext


def RealtimePloter(arg):
  global values
  CurrentXAxis=pylab.arange(len(values)-100,len(values),1)
  line1[0].set_data(CurrentXAxis,pylab.array(values[-100:]))
  ax.axis([CurrentXAxis.min(),CurrentXAxis.max(),-20000,20000])
  manager.canvas.draw()
  #manager.show()

timer = fig.canvas.new_timer(interval=20)
timer.add_callback(RealtimePloter, ())
timer2 = fig.canvas.new_timer(interval=20)
timer2.add_callback(SinwaveformGenerator, ())
timer.start()
timer2.start()

pylab.show()
