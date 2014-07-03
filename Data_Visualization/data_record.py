import serial
import time
from bitstring import BitArray

portNum_1 = 8
# portNum_1 = 17
# portNum_2 = 8
baudrate = 115200

def receive_data(receive_port, record_file, now):
    receive_time = now
    number = receive_port.inWaiting()

    if number > 100:
        data = receive_port.read(number)
        receive_time = time.time()
        byte_count = 0

        while byte_count < number - 16:
            if data[byte_count] == 'S' and data[byte_count+15] == 'E':
                ax = BitArray('0x' +(data[byte_count+1]+data[byte_count+2]).encode('hex')).int
                ay = BitArray('0x' +(data[byte_count+3]+data[byte_count+4]).encode('hex')).int
                az = BitArray('0x' +(data[byte_count+5]+data[byte_count+6]).encode('hex')).int
                gx = BitArray('0x' +(data[byte_count+9]+data[byte_count+10]).encode('hex')).int
                gy = BitArray('0x' +(data[byte_count+11]+data[byte_count+12]).encode('hex')).int
                gz = BitArray('0x' +(data[byte_count+13]+data[byte_count+14]).encode('hex')).int
                datastr =  str(ax) + ',' + str(ay) + ',' + str(az) + ',' + str(gx) + ',' + str(gy) + ',' + str(gz)
                # record_file.write(datastr+'\n')
                print datastr
                byte_count += 16
            else:
                byte_count += 1

    elif time.time() - now > 2 and number == 0:
        print receive_port.name + " cannot receive data"
        receive_port.close()
        exit()

    return receive_time


def main():

    ser_1 = serial.Serial(portNum_1, baudrate)
    f_1 = open('sensor_1_record', 'w')
    count_1 = 0
    if ser_1 != None:
        print 'connecting to port' + ser_1.name + ' success!!'

    # ser_2 = serial.Serial(portNum_2, baudrate)
    # f_2 = open('sensor_2_record', 'w')
    # count_2 = 0
    # if ser_2 != None:
    #     print 'connecting to port' + ser_2.name + ' success!!'

    now_1 = now_2 = time.time()


    while True:

        now_1 = receive_data(ser_1, f_1, now_1)

        # now_2 = receive_data(ser_2, f_2, now_2)


if __name__ == '__main__':
	main()
