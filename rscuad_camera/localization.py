import socket
import cv2
from localization.montecarlo import Montecarlo
import numpy as np


UDP_IP = "127.0.0.1"
UDP_PORT = 5005

print("UDP target IP: %s" % UDP_IP)
 
sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
width_loc = 600
height_loc = 900
loc = Montecarlo(width_loc, height_loc)

x_result =0
y_result =0
while(1):

	data_send = str(int(x_result))+" "+str(int(y_result))
	sock.sendto(data_send.encode(), (UDP_IP, UDP_PORT))
	data, addr = sock.recvfrom(1024)

	numbers = data.decode().split()
	# Convert the substrings to integers
	point = int(numbers[0])
	x = float(numbers[1])
	y = float(numbers[2])

	print("send message: ", data_send)  
	print("received message: ", numbers)

	x_result, y_result = loc.process(point,x,y)
	if cv2.waitKey(50) & 0xFF == 27:
		break