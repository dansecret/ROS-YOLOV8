/**
 *	Title	: Testing Module RSCUAD
 *
 *	Author	: Danu Andrean
 *	Create	: 2020
 *	Update 	: 2023
 *	
 *	Note	:
 */

#include "rscuad.h"
#include <signal.h>
#include <stdarg.h>

void sighandler(int sig) {
    printf(_RESET);
    exit(0);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, sighandler);

    /*
        Initial
    */
   	RSCUAD::Get()->communication->SetIPGC("192.168.0.17");

	RSCUAD::Get()->camera->Init();
    RSCUAD::Get()->serial->Init(1, "/dev/ttyUSB0");
    RSCUAD::Get()->communication->Init("192.168.43.103", 1, true, 7, true);

    int count = 0;
    
    for(;;)
    {
        /*
            Loop Area
        */
        ++count;

        /* com */
        RSCUAD::Get()->communication->Robot(1,0,0,360,0,count,200, 300, 400);
        RSCUAD_PRINT(_YELLOW "value return com : %d\n", 	Communication::s_robot);
        RSCUAD_SUCCESS("value return state : %d\n", 		Communication::s_GC->STATE);
		RSCUAD_SUCCESS("value return kickoff : %d\n", 		Communication::s_GC->KICKOFF);
		RSCUAD_SUCCESS("value return second state : %d\n", 	Communication::s_GC->SECOND_STATE);
		RSCUAD_SUCCESS("value return team : %d\n", 			Communication::s_GC->SECOND_STATE_TEAM);
		RSCUAD_SUCCESS("value return condition : %d\n", 	Communication::s_GC->SECOND_STATE_CONDITION);	
		RSCUAD_SUCCESS("value is pickup : %d\n", 			Communication::s_GC->IS_PICKUP);
		RSCUAD_SUCCESS("value is dropball : %d\n", 			Communication::s_GC->IS_DROP_BALL);	

		// example to use
		if(Communication::s_GC->STATE == GC_PLAY)
		{
			RSCUAD_INFO("playing \n");
		}
		if(Communication::s_GC->SECOND_STATE == GC_PENALTY && Communication::s_GC->SECOND_STATE_TEAM == Communication::s_team)
		{
			RSCUAD_INFO("penalty \n");
		}


        /* gyro */
        RSCUAD::Get()->serial->Process();
        RSCUAD_INFO("data sensor gyro  : %d\n", RSCUAD::Get()->serial->Gyro());
        RSCUAD_INFO("data sensor button: %d\n", RSCUAD::Get()->serial->Button());

        /* camera */
		bool isDetected = RSCUAD::Get()->camera->Process(true);
		RSCUAD_PRINT("Is detected %d \n", isDetected);
		RSCUAD_PRINT("Target X %f \n", RSCUAD::Get()->camera->GetX());
		RSCUAD_PRINT("Target Y %f \n", RSCUAD::Get()->camera->GetY());
		RSCUAD_PRINT("Robot X %f \n", RSCUAD::Get()->camera->GetLocX());
		RSCUAD_PRINT("Robot Y %f \n", RSCUAD::Get()->camera->GetLocY());
		RSCUAD_PRINT("Point %d \n", RSCUAD::Get()->camera->GetPoint());

		RSCUAD::Get()->communication->Camera(RSCUAD::Get()->camera->GetFrame());

        // if(count > 100) break;
        usleep(1000);
    }

	//if you want to very clear
	RSCUAD::Get()->~RSCUAD();
    return 0;
}