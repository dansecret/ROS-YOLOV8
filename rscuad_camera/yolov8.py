#
# yolo v8 ball detection
# Ham dandih
# 2023
# 

from gateway import Gateway
from ultralytics import YOLO
import numpy as np
import cv2 as cv
import time
import torch

clases = ["ball", "goal", "robot"]
torch.device('cuda')
# Load the YOLOv8 model
model = YOLO('best.pt')
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

frameRate = 0
rate = 10.0
prev_frame_time = 0
new_frame_time = 0
framecount = 0
framecount = 0
fps = 0
fpsLimit = 1# throttle limit
startTime = time.time()

if not cap.isOpened():
  print("Cannot open camera")
  exit()
frameRate = 0

x_center = 0
y_center = 0

while cap.isOpened():
    # Read a frame from the video
	success, frame = cap.read()
	is_detected = False
	x_cor = 0
	y_cor = 0
	w_cor = 0
	h_cor = 0
	ball = [0,0,0]
	goal = [0,0,0]
	robot = [0,0,0]
	if success:
		results = model(frame)
		annotated_frame = results[0].plot()


		for result in results:
			boxes = result.boxes.cpu().numpy() 

			for box in boxes: 
				r = box.xyxy[0].astype(int)
				x_cor = r[:2][0]
				y_cor = r[:2][1] 
				w_cor = r[2:][0] - r[:2][0]
				h_cor = r[2:][1] - r[:2][1] 
				x_center = x_cor + w_cor /2
				y_center = y_cor + h_cor /2

				class_name = box.cls[0].astype(int)

				if class_name == 0:
					ball.insert(0, x_center)
					ball.insert(1, y_center)
					ball.insert(2, 1)
				
				if class_name == 1:
					goal.insert(0, x_center)
					goal.insert(1, y_center)
					goal.insert(2, 1)
				
				if class_name == 2:
					robot.insert(0, x_center)
					robot.insert(1, y_center)
					robot.insert(2, 1)


				cv.circle(annotated_frame, (int(x_center), int(y_center)), 5, (0,0,255), -1)
				cv.putText(annotated_frame, clases[class_name], (r[0],r[1]),cv.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
				cv.rectangle(annotated_frame, r[:2], r[2:], (255, 255, 255), 2)  
			
		# Display the annotated frame
        # fps = int(cap.get(cv.CAP_PROP_FPS))
		nowTime = time.time()
		frameRate += rate
		times = int(nowTime - startTime)
		if times > fpsLimit:
			fps = frameRate/times
			startTime = time.time()
			frameRate = 0
		cv.putText(annotated_frame,str(fps),(50,50), cv.FONT_HERSHEY_SIMPLEX, 2,(255, 0, 0),2,cv.LINE_AA)
		cv.imshow("YOLOv8 Inference",annotated_frame)
		print("ball", ball[2])
		print("ballx", ball[0])
		print("bally", ball[1])

		# self, is_ball, ball_x, ball_y, goal_x=0, goal_y=0, is_goal=False, is_robot=False, robot_x=0, robot_y=0
		gateway.update_data(ball[2], ball[0],ball[1], goal[0],goal[1],goal[2],robot[2],robot[0],robot[1]) # isdetected, x , y
		gateway.update_image(frame) 
		
		# Break the loop if 'q' is pressed
		if cv.waitKey(1) & 0xFF == ord("q"):
			break
	else:
		# Break the loop if the end of the video is reached
		break

# Release the video capture object and close the display window
cap.release()
cv.destroyAllWindows()