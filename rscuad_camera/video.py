import cv2
import time
from ultralytics import YOLO
import threading

import socket
from estimation import Estimation

UDP_IP = "127.0.0.1"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

g_point = 0
width_loc = 600
height_loc = 900
x_coor = 0
y_coor = 0
estimate = Estimation(width_loc, height_loc)

def resize_image(img, scale_percent) :
    width = int(img.shape[1] * scale_percent / 100)
    height = int(img.shape[0] * scale_percent / 100)
    dim = (600, 400)
    resized = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)
    return resized

def send_localization():
	# recv message and decode here 1024 is buffer size.    \
	global g_point, x_coor, y_coor
	while(1):
		
		data_recv, addr = sock.recvfrom(1024) # buffer size is 1024 bytes

		numbers = data_recv.decode().split()
		x_recv = int(numbers[0])
		y_recv = int(numbers[1])

		predict_point = estimate.closest_index(x_recv, y_recv)
		print("prediction point: ", predict_point)
		print("received message: ", data_recv)  
		print("--------------------------------------")

		if g_point>0:
			x_coor, y_coor = estimate.point_to_coor( g_point )
		data_send = str(g_point)+" "+str(x_coor)+" "+str(y_coor)
		sock.sendto(bytes(data_send.encode()), addr)
		
def image_processing():
	global g_point
	start_time = time.time()
	# FPS update time in seconds
	display_time = 1
	fc = 0
	FPS = 0
	clases = ["pinalty", "right1", "right2", "left1", "left2"]

	model = YOLO('best.pt')
	video_path = "coba.webm"

	cap = cv2.VideoCapture(0)
	# cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
	# cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)


	while cap.isOpened():
		success, frame = cap.read()
		frame = cv2.flip(frame, flipCode=1)
		frame = resize_image(frame, 70)

		if success:
			fc += 1
			if fc % 4 != 0:
				continue

			# frame = cv2.resize(frame, (640, 480))
			results = model(frame)
			pinalty_area = [-1, -1]
			right1_area = [-1, -1]
			right2_area = [-1, -1]
			left1_area = [-1, -1]
			left2_area = [-1, -1]

			# annotated_frame = results[0].plot()
			# results = model.predict(results, stream=True)                 # run prediction on img
			for result in results:                                         # iterate results
				boxes = result.boxes.cpu().numpy()                         # get boxes on cpu in numpy
				for box in boxes:                                          # iterate boxes
					r = box.xyxy[0].astype(int)                            # get corner points as int
					x_cor = r[:2][0]
					y_cor = r[:2][1] 
					w_cor = r[2:][0] - r[:2][0]
					h_cor = r[2:][1] - r[:2][1] 
					x_center = x_cor + w_cor /2
					y_center = y_cor + h_cor /2

					# print(r)
					class_name = box.cls[0].astype(int)

					if class_name == 0:
						pinalty_area.insert(0, x_center)
						pinalty_area.insert(1, y_center)

					if class_name == 1:
						right1_area.insert(0, x_center)
						right1_area.insert(1, y_center)

					if class_name == 2:
						right2_area.insert(0, x_center)
						right2_area.insert(1, y_center)
					
					if class_name == 3:
						left1_area.insert(0, x_center)
						left1_area.insert(1, y_center)
					
					if class_name == 4:
						left2_area.insert(0, x_center)
						left2_area.insert(1, y_center)

					cv2.circle(frame, (int(x_center), int(y_center)), 5, (0,0,255), -1)
					cv2.putText(frame, clases[class_name], (r[0],r[1]),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
					cv2.rectangle(frame, r[:2], r[2:], (255, 255, 255), 2)  
			g_point = estimate.calculation_position(pinalty_area, right1_area, right2_area, left1_area, left2_area)
			TIME = time.time() - start_time

			if (TIME) >= display_time :
				FPS = fc / (TIME)
				fc = 0
				start_time = time.time()

			fps_disp = "FPS: "+str(FPS)[:5]

			cv2.putText(frame, fps_disp, (10, 25),cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

			cv2.imshow("YOLOv8 Inference", frame)

			if cv2.waitKey(1) & 0xFF == ord("q"):
				break
		else:
			break

	cap.release()
	cv2.destroyAllWindows()

def main():

	processing = threading.Thread(target=image_processing,  daemon=None	)
	loc = threading.Thread(target=send_localization, daemon=None	)
	processing.start()
	loc.start()

if __name__ == "__main__":
    main()