#include "hsv.h"
#include "gateway.h"

int main()
{
	HSV hsv;
	Gateway gate;

	hsv.SetLighting(0,50);
	hsv.SetField(0, 255, 0, 255, 0,255,0);
	hsv.SetBall(0, 21, 47, 255, 170, 255);
	hsv.Init();
	gate.Init();
    
	while(1)
	{
		bool isBall = hsv.Process();

		float ballX = hsv.GetX();
		float ballY = hsv.GetY();	

		int locX = 10;
		int locY = 100;
		int point = 0;
		int isTarget = 1;
		int targetX = 90;
		int targetY = 80;

		
		gate.Update(isBall, ballX, ballY, locX, locY, point, isTarget, targetX, targetY);
		gate.Update(hsv.GetFrame());

		hsv.EndProsess();
	}

}
