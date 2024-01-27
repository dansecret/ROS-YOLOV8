import numpy as np
import scipy as scipy
from numpy.random import uniform
import scipy.stats


np.set_printoptions(threshold=3)
np.set_printoptions(suppress=True)
import cv2
import random

class Draw:
	def drawLines(self, img, points, r, g, b):
		cv2.polylines(img, [np.int32(points)], isClosed=False, color=(r, g, b))

	def drawCross(self, img, center, r, g, b):
		d = 5
		t = 2
		LINE_AAa = cv2.LINE_AA 
		color = (r, g, b)
		ctrx = center[0,0]
		ctry = center[0,1]
		cv2.line(img, (ctrx - d, ctry - d), (ctrx + d, ctry + d), color, t, LINE_AAa)
		cv2.line(img, (ctrx + d, ctry - d), (ctrx - d, ctry + d), color, t, LINE_AAa)
		

class Montecarlo:
	#Number of partciles
	NL = 0
	N=400
	previous_x=-1
	previous_y=-1
	particles= np.empty((N, 2))
	landmarks=np.array([ [144,73], [410,13], [336,175], [718,159], [178,484], [665,464]  ])
	sensor_std_err=5
	trajectory=np.zeros(shape=(0,2))
	draw = Draw()

	def __init__(self, w, h ):
		self.width = w
		self.height = h
		self.x_range=np.array([0, self.width])
		self.y_range=np.array([0, self.height])

		self.NL = len(self.landmarks)
		self.particles = self.create_uniform_particles(self.x_range, self.y_range, self.N)

		self.weights = np.array([1.0]*self.N)
		self.img = np.zeros((self.height, self.width, 3), np.uint8)
		self.image = cv2.imread('localization/lapangan.png')


	def create_uniform_particles(self, x_range, y_range, N):
		self.particles = np.empty((N, 2))
		self.particles[:, 0] = uniform(x_range[0], x_range[1], size=N)
		self.particles[:, 1] = uniform(y_range[0], y_range[1], size=N)
		return self.particles



	def predict(self, particles, u, std, dt=1.):
		N = len(particles)
		dist = (u[0] * dt) + (np.random.randn(N) * std[1])
		particles[:, 0] += np.cos(u[0]) * dist
		particles[:, 1] += np.sin(u[0]) * dist
		return particles
	
	def update(self, particles, weights, z, R, landmarks):
		weights.fill(1.)
		for i, landmark in enumerate(landmarks):
		
			distance=np.power((particles[:,0] - landmark[0])**2 +(particles[:,1] - landmark[1])**2,0.5)
			weights *= scipy.stats.norm(distance, R).pdf(z[i])

	
		weights += 1.e-300 # avoid round-off to zero
		weights /= sum(weights)
		return weights
		
	def neff(self, weights):
		return 1. / np.sum(np.square(weights))

	def systematic_resample(self, weights):
		N = len(weights)
		positions = (np.arange(N) + np.random.random()) / N

		indexes = np.zeros(N, 'i')
		cumulative_sum = np.cumsum(weights)
		i, j = 0, 0
		while i < N and j<N:
			if positions[i] < cumulative_sum[j]:
				indexes[i] = j
				i += 1
			else:
				j += 1
		return indexes

	def get_center(self, particles):
		x = [item[0] for item in particles]
		y = [item[1] for item in particles]
		x_val = np.average(x, axis=0)
		y_val = np.average(y, axis=0)

		return x_val, y_val
		
	def estimate(self, particles, weights):
		pos = particles[:, 0:1]
		mean = np.average(pos, weights=weights, axis=0)
		var = np.average((pos - mean)**2, weights=weights, axis=0)
		return mean, var

	def resample_from_index(self, particles, weights, indexes):
		particles[:] = particles[indexes]
		weights[:] = weights[indexes]
		weights /= np.sum(weights)
		return particles, weights

    
	def mcl(self, ispoint, x, y):

		# print("particle ", self.particles)
		

		center=np.array([[x,y]])
		self.trajectory=np.vstack((self.trajectory,np.array([x,y])))
		#noise=sensorSigma * np.random.randn(1,2) + sensorMu

		if self.previous_x >0:
			heading=np.arctan2(np.array([y-self.previous_y]), np.array([self.previous_x-x ]))

			if heading>0:
				heading=-(heading-random.uniform(0.0, 3.0))
			else:
				heading=-(random.uniform(0.0, 3.0)+heading) 

			print("heading", heading)
				
			distance=np.linalg.norm(np.array([[self.previous_x,self.previous_y]])-np.array([[x,y]]) ,axis=1)
			
			std=np.array([2,5])
			u=np.array([heading,distance])
			self.particles = self.predict(self.particles, u, std, dt=1.)
			zs = (np.linalg.norm(self.landmarks - center, axis=1) + (np.random.randn(self.NL) * self.sensor_std_err))
			self.weights = self.update(self.particles, self.weights, z=zs, R=50, landmarks=self.landmarks)
			
			indexes = self.systematic_resample(self.weights)
			self.particles, self.weight = self.resample_from_index(self.particles, self.weights, indexes)

		self.previous_x=x
		self.previous_y=y


		for particle in self.particles:
			cv2.circle(self.img,tuple((int(particle[0]),int(particle[1]))),1,(0,0,255),-1)

		x_result, y_result = self.get_center(self.particles)
		# print(">>>>" ,x_result, y_result)
		cv2.circle(self.img, (int(x_result), int(y_result)) , 3,(0,255,255), 5)
		return center, x_result, y_result
		
	def process(self, ispoint,  x, y):

		# print("x {} y{}".format(x,y))

		gambar_jpeg_resized = cv2.resize(self.image, ( self.width, self.height))
		self.img = cv2.addWeighted(self.img, 1, gambar_jpeg_resized, 1, 0)
		
		center, x_result, y_result = self.mcl(ispoint, int(x), int(y))

		cv2.imshow("monte carlo",self.img)
		self.img = np.zeros((self.height,self.width,3), np.uint8)
		# drawLines(img, trajectory,   0,   255, 0)
		self.draw.drawCross(self.img, center, r=255, g=0, b=0)
		return x_result, y_result


