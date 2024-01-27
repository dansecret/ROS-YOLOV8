# add dependency
- package.xml

		<depend>rscuad</depend>

- CMakelists.txt

		find_package(catkin REQUIRED COMPONENTS
			...
			...
			rscuad
		)

-  add library

		#include "rscuad.h"

- code is normal you can see in the testing.cpp

## Note
> we disable the camera in the library for ROS until we implement the new method

## Copyright
> RSCUAD TEAM
