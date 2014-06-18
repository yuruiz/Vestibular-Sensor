import serial
import time
from bitstring import BitArray

# portNum_1 = 12
portNum_1 = 10
portNum_2 = 12
baudrate = 115200




def main():
    f_1 = open('sensor_1_record', 'w')
    ser_1 = serial.Serial(portNum_1, baudrate)
    count_1 = 0

    f_2 = open('sensor_2_record', 'w')
    ser_2 = serial.Serial(portNum_2, baudrate)
    count_2 = 0


    start = time.time()
    now = start

    if ser_1 != None:
        print 'connecting to port 1 success!!'

    if ser_2 != None:
        print 'connecting to port 2 success!!'

    while True:
        if ser_1.inWaiting() > 100:
            number_1 = ser_1.inWaiting()
            data_1 = ser_1.read(number_1)
            for x in xrange(0, number_1):
                if x < number_1 - 16 and  data_1[x] == 'S' and data_1[x+15] == 'E':
                    ax = BitArray('0x' +(data_1[x+1]+data_1[x+2]).encode('hex')).int
                    ay = BitArray('0x' +(data_1[x+3]+data_1[x+4]).encode('hex')).int
                    az = BitArray('0x' +(data_1[x+5]+data_1[x+6]).encode('hex')).int
                    gx = BitArray('0x' +(data_1[x+9]+data_1[x+10]).encode('hex')).int
                    gy = BitArray('0x' +(data_1[x+11]+data_1[x+12]).encode('hex')).int
                    gz = BitArray('0x' +(data_1[x+13]+data_1[x+14]).encode('hex')).int
                    str_1 =  str(ax) + ',' + str(ay) + ',' + str(az) + ',' + str(gx) + ',' + str(gy) + ',' + str(gz)
                    f_1.write(str_1+'\n')
                    # count_1 = count_1 + 1
                    now_1 = time.time()
                    # print "port1:" + str(count_1)

        elif time.time() - now_1 > 1 and ser_1.inWaiting() < 10:
            exit()

        if ser_2.inWaiting()> 100:
            number_2 = ser_2.inWaiting()
            data_2 = ser_2.read(number_2)
            for x in xrange(0, number_2):
                # print '123'
                if x < number_2 - 16 and  data_2[x] == 'S' and data_2[x+15] == 'E':
                    ax = BitArray('0x' +(data_2[x+1]+data_2[x+2]).encode('hex')).int
                    ay = BitArray('0x' +(data_2[x+3]+data_2[x+4]).encode('hex')).int
                    az = BitArray('0x' +(data_2[x+5]+data_2[x+6]).encode('hex')).int
                    gx = BitArray('0x' +(data_2[x+9]+data_2[x+10]).encode('hex')).int
                    gy = BitArray('0x' +(data_2[x+11]+data_2[x+12]).encode('hex')).int
                    gz = BitArray('0x' +(data_2[x+13]+data_2[x+14]).encode('hex')).int
                    str_2 = str(ax) + ',' + str(ay) + ',' + str(az) + ',' + str(gx) + ',' + str(gy) + ',' + str(gz)
                    f_2.write(str_2+'\n')
                    # count_2 = count_2 + 1
                    now_2 = time.time()
                    # print "port2:" + str(count_2)
        elif time.time() - now_2 > 1 and ser_2.inWaiting() < 10:
            exit()

if __name__ == '__main__':
	main()
