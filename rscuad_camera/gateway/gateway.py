# 
# 	Title	: AI Gateway
# 
# 	Author	: Danu Andrean
# 	Create	: 2023
# 	Update 	: 2024
# 	
# 	Note	: gateway for data python
# 

USE_ROS = True

import socket
import sys
import numpy as np
import cv2 as cv
import threading
import time

if USE_ROS:
	import rospy
	from std_msgs.msg import String
	from rscuad.msg import DataCamera

	import rospy 
	from sensor_msgs.msg import Image 
	from cv_bridge import CvBridge 


class Gateway:
	
	ip = "127.0.0.1"
	port_data = 9131
	port_image = 9132
	data = "0 0 0 0 0"
	frame = 0

	rospy.init_node('camera_module', anonymous=True)
	pub_image = rospy.Publisher('video_camera', Image, queue_size=10)
	pub_data = rospy.Publisher('data_camera', DataCamera, queue_size=10)

	rate = rospy.Rate(10) # 10hz
	br = CvBridge()

	def __init__(self):
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

	def init(self):
		data = threading.Thread(target=Gateway.async_data,  args = (self,), daemon=True)
		image = threading.Thread(target=Gateway.async_image, args = (self,), daemon=True)
 
		data.start()
		image.start()
		print("Running in {}".format(self.ip))

	def async_data(self):
		while(1):
			self.sock.sendto(self.data.encode('utf-8'), (self.ip, self.port_data))
			time.sleep(0.1)
	def async_image(self):
		while(1):
			_, encoded_frame = cv.imencode(".jpg", self.frame, [int(cv.IMWRITE_JPEG_QUALITY), 70])
			self.sock.sendto(encoded_frame.tobytes(), (self.ip, self.port_image))
			time.sleep(0.1)

	def update_data(self, is_ball, ball_x, ball_y, goal_x=0, goal_y=0, is_goal=False, is_robot=False, robot_x=0, robot_y=0):
		if USE_ROS:
			dc = DataCamera()
			dc.ball = is_ball
			dc.ball_x = ball_x
			dc.ball_y = ball_y
			dc.point = is_goal
			dc.loc_x = goal_x
			dc.loc_y = goal_y
			dc.target = is_robot
			dc.target_x = robot_x
			dc.target_y = robot_y
			
			self.pub_data.publish(dc)
		else:
			self.data = str(is_ball)+" "+str(ball_x)+" "+str(ball_y)+" "+str(loc_x)+" "+str(loc_y)+" "+str(point)+" "+ str(is_target)+" "+str(target_x)+" "+str(target_y)

	def update_image(self, frame):
		if USE_ROS:
			self.pub_image.publish(self.br.cv2_to_imgmsg(frame))
		else:
			self.frame = frame

	def get_ros(self):
		return USE_ROS
		
