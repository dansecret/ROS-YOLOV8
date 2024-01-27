import math

class Estimation:

	def __init__(self, w, h):
		self.width = w
		self.height = h
		self.array_loc = [	[self.width/5 *1, self.height/6 * 1],
							[self.width/5 *2, self.height/6 * 1],
							[self.width/5 *3, self.height/6 * 1],
							[self.width/5 *4, self.height/6 * 1],
							[self.width/5 *1, self.height/6 * 2],
							[self.width/5 *2, self.height/6 * 2],
							[self.width/5 *3, self.height/6 * 2],
							[self.width/5 *4, self.height/6 * 2],
							[self.width/5 *1, self.height/6 * 3],
							[self.width/5 *2, self.height/6 * 3],
							[self.width/5 *3, self.height/6 * 3],
							[self.width/5 *4, self.height/6 * 3]]

	def point_to_coor(self, point):
		return self.array_loc[point-1]

	def gradien(self, x1, y1, x2, y2):
		
		y = y2-y1
		x = x2-x1

		m = 0
		if y != x:
			m = y/x

		return m

	def calculation_position(self, p, r1, r2, l1, l2):
		tresh = 1
		point = 0
		global prev_point
		rm = self.gradien( r2[0], 400-r2[1], r1[0], 400-r1[1])
		lm = self.gradien( l2[0], 400-l2[1], l1[0], 400-l1[1])
		lmp = self.gradien( p[0], 400-p[1], l2[0], 400-l2[1])
		rmp = self.gradien( p[0], 400-p[1], r2[0], 400-r2[1])

		rm = abs(rm)
		lm = abs(lm)

		# all actived
		if p[0]>0 and r1[0]>0 and r2[0]>0:
			if rm > 1 and rmp < 0.1 and rmp > 0.0 :
				point = 12
			elif rm > 0.5 and rmp > 0.1 :
				point = 11
			elif rm > 0.1 and rmp > 0.2:
				point = 10
			# -------------------------------------------
			elif rm > 0.05 and rmp < -2:
				point = 6
			elif rm < 0.05 and rmp > -2 :
				point = 5

		elif p[0]>0 and l1[0]>0 and l2[0]>0:
			if lm > 1 and lmp > -0.1 and lmp < 0.0:
				point = 9
			elif lm > 0.5 and lmp <-0.1 :
				point = 10
			elif lm > 0.1 and lmp <-0.2 :
				point = 11
			# -------------------------------------------
			elif lm > 0.05 and lmp > 2:
				point = 7
			elif lm < 0.1 and lmp < 2:
				point = 8

		elif p[0]>0  and l2[0]>0:
			if lmp > -0.1 and lmp < 0.0:
				point = 9
			elif lmp > 2:
				point = 7
			elif lmp <-0.2 :
				point = 11
			elif lmp <-0.1 :
				point = 10
			elif lmp < 2 :
				point = 8
		
		elif p[0]>0  and r2[0]>0:
			if rmp < 0.1 and rmp > 0.0:
				point = 12
			elif rmp < -2:
				point = 6
			elif rmp > 0.2 :
				point = 10
			elif rmp > 0.1 :
				point = 11
			elif rmp > -2 :
				point = 5
		


		elif l1[0]>0 and l2[0]> 0: 
			if lm > 2:
				point = 1
			elif lm > 0.5:
				point = 6
			elif lm > 0.2:
				point = 2
			elif lm > 0.03:
				point = 3

		elif r1[0]>0 and r2[0]> 0: 
			if rm > 2:
				point = 4
			elif rm > 0.5:
				point = 7
			elif rm > 0.2:
				point = 3
			elif rm > 0.03:
				point = 2
		
		print("--------------------------------")
		print("Gradien rm {}| lm {}| rmp {}| lmp {}".format(rm, lm, rmp, lmp))
		print("Point estimate : ",point)
		return point

	def euclidean_distance(self, point1, point2):
		return math.sqrt((point1[0] - point2[0])**2 + (point1[1] - point2[1])**2)

	def closest_index(self, x, y):
		distances = [self.euclidean_distance([x, y], point) for point in self.array_loc]
		min_distance = min(distances)
		closest_index = distances.index(min_distance)
		return closest_index+1
