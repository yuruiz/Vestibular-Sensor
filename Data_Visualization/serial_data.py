import serial
port = serial.Serial(11, 9600)
print port
line = ""
while 1 > 0:
	line = port.readline()
	if '*' not in line[5:10]:
		try:
			print int(line[5:10].strip(' '))
		except Exception, e:
			pass
	line = ""

