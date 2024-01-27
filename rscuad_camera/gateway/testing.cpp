#include "gateway.h"
#include <unistd.h>

int main()
{
	Gateway gate;

	gate.Init();

	while(1)
	{
		bool isBall = 1;

		float ballX = 88;
		float ballY =66;	

		int locX = 10;
		int locY = 100;
		int point = 6;
		int isTarget = 1;
		int targetX = 90;
		int targetY = 80;

		
		gate.Update(isBall, ballX, ballY, locX, locY, point, isTarget, targetX, targetY);
		sleep(1);
	}

}