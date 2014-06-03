import serial
import time
from bitstring import BitArray

portNum = 15
baudrate = 9600


def main():
    ser = serial.Serial(portNum, baudrate)
    while True:
        if ser.inWaiting() > 100:
            number = ser.inWaiting()
            data = ser.read(number)
            for x in xrange(0, number):
                # print '123'
                if x < number - 16 and  data[x] == 'S' and data[x+15] == 'E':
                    ax = BitArray('0x' +(data[x+1]+data[x+2]).encode('hex')).int
                    ay = BitArray('0x' +(data[x+3]+data[x+4]).encode('hex')).int
                    az = BitArray('0x' +(data[x+5]+data[x+6]).encode('hex')).int
                    gx = BitArray('0x' +(data[x+9]+data[x+10]).encode('hex')).int
                    gy = BitArray('0x' +(data[x+11]+data[x+12]).encode('hex')).int
                    gz = BitArray('0x' +(data[x+13]+data[x+14]).encode('hex')).int
                    print str(ax) + ',' + str(ay) + ',' + str(az) + ',' + str(gx) + ',' + str(gy) + ',' + str(gz)

if __name__ == '__main__':
	main()
    # a = '11111111'
    # b = '10001111'
    # print BitArray(bin = a).int
