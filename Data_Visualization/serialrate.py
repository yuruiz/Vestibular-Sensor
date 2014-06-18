import serial, time

def main():
	ser = serial.Serial(12, 115200)
	start = time.time()
	now = start
	count = 0
	while True:
	    if ser.inWaiting() > 100:
	        number = ser.inWaiting()
	        data = ser.read(number)
	        for x in xrange(0, number):
	            if x < number - 16 and  data[x] == 'S' and data[x+15] == 'E':
	                count += 1
	    if time.time() - now > 1:
	        now = time.time()
	        print count/(time.time() - start)

if __name__ == '__main__':
	main()
