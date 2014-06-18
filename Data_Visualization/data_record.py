import serial
import time
from bitstring import BitArray

# portNum_1 = 12
portNum_1 = 12
portNum_2 = 8
baudrate = 115200

def receive_data(receive_port, record_file, now):
    finish_time = now
    number = receive_port.inWaiting()

    if number > 100:
        data = receive_port.read(number)
        finish_time = time.time()
        byte_count = 0

        while byte_count < number - 16:
            if data[byte_count] == 'S' and data[byte_count+15] == 'E':
                ax = BitArray('0x' +(data[byte_count+1]+data[byte_count+2]).encode('hex')).int
                ay = BitArray('0x' +(data[byte_count+3]+data[byte_count+4]).encode('hex')).int
                az = BitArray('0x' +(data[byte_count+5]+data[byte_count+6]).encode('hex')).int
                gx = BitArray('0x' +(data[byte_count+9]+data[byte_count+10]).encode('hex')).int
                gy = BitArray('0x' +(data[byte_count+11]+data[byte_count+12]).encode('hex')).int
                gz = BitArray('0x' +(data[byte_count+13]+data[byte_count+14]).encode('hex')).int
                str_1 =  str(ax) + ',' + str(ay) + ',' + str(az) + ',' + str(gx) + ',' + str(gy) + ',' + str(gz)
                record_file.write(str_1+'\n')
                byte_count += 16
            else:
                byte_count += 1

    elif time.time() - now > 1 and receive_port.inWaiting() == 10:
        print receive_port.name + " cannot receive data"
        exit()

    return finish_time


def main():
    f_1 = open('sensor_1_record', 'w')
    ser_1 = serial.Serial(portNum_1, baudrate)
    count_1 = 0
    if ser_1 != None:
        print 'connecting to port 1 success!!'


    f_2 = open('sensor_2_record', 'w')
    ser_2 = serial.Serial(portNum_2, baudrate)
    count_2 = 0
    if ser_2 != None:
        print 'connecting to port 2 success!!'

    now_1 = now_2 = time.time()


    while True:

        now_1 = receive_data(ser_1, f_1, now_1)

        now_2 = receive_data(ser_2, f_2, now_2)


if __name__ == '__main__':
	main()
