#
# Example Camera code for python
# Danu Andrean
# 2023
# 


from gateway import Gateway
import numpy as np
import cv2 as cv

gateway = Gateway()

USE_ROS = gateway.get_ros()
if not USE_ROS:
	gateway.init()
	print("USING INIT")
else :
	import rospy

cap = cv.VideoCapture(0)  
cap.set(cv.CAP_PROP_FRAME_WIDTH, 320) 
cap.set(cv.CAP_PROP_FRAME_HEIGHT, 240)
cap.set(cv.CAP_PROP_FPS, 30) 			 # Sesuaikan frame rate

if not cap.isOpened():
	print("Cannot open camera")
	exit()

while not rospy.is_shutdown():
# while True:
	ret, frame = cap.read()
	if not ret:
		break

	# gateway
	gateway.update_data(1, 900, 1234, 70,6,0, 1,80,90) # isball, x , y, loc x, loc y, point, istarget, target x, target y
	gateway.update_image(frame)

	cv.imshow('frame', frame)
	if cv.waitKey(1) == ord('q'):
		break
	
cap.release()
cv.destroyAllWindows()
