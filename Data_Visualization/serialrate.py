import serial, time

def main():
	ser = serial.Serial(15, 9600)

	while True:
		ser.flushInput()
		now = time.time()
		future = now + 10
		while time.time() < future:
			pass
		number = ser.inWaiting()
		data = ser.read(number)
		count = 0
		for x in xrange(0,number):
			if x < number-16:
				if data[x] == 'S' and data[x+15] == 'E':
					count += 1
		print count/10.0

if __name__ == '__main__':
	main()
