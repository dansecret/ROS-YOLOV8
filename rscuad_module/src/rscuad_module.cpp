#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>
#include "rscuad.h"

bool isDetected = false;

void module_init()
{
	/*
		Initial
    camera init not necessary in ROS
	*/
	RSCUAD::Get()->communication->SetIPGC("192.168.43.103");
	RSCUAD::Get()->serial->Init(1, "/dev/ttyUSB0");
	RSCUAD::Get()->communication->Init("192.168.43.103", 1, true, 3, true);

}

void module_test()
{
    /* com */
    // RSCUAD::Get()->communication->Robot(isDetected,0,0,360,0,7,200, 300, 400);
    // RSCUAD_PRINT(_YELLOW "value return com : %d\n", 	Communication::s_robot);
    // RSCUAD_SUCCESS("value return state : %d\n", 		Communication::s_GC->STATE);
    // RSCUAD_SUCCESS("value return kickoff : %d\n", 		Communication::s_GC->KICKOFF);
    // RSCUAD_SUCCESS("value return second state : %d\n", 	Communication::s_GC->SECOND_STATE);
    // RSCUAD_SUCCESS("value return team : %d\n", 			Communication::s_GC->SECOND_STATE_TEAM);
    // RSCUAD_SUCCESS("value return condition : %d\n", 	Communication::s_GC->SECOND_STATE_CONDITION);	
    // RSCUAD_SUCCESS("value is pickup : %d\n", 			Communication::s_GC->IS_PICKUP);
    // RSCUAD_SUCCESS("value is dropball : %d\n", 			Communication::s_GC->IS_DROP_BALL);	

    // example to use
    // if(Communication::s_GC->STATE == GC_PLAY)
    // {
    //     RSCUAD_INFO("playing \n");
    // }
    // if(Communication::s_GC->SECOND_STATE == GC_PENALTY && Communication::s_GC->SECOND_STATE_TEAM == Communication::s_team)
    // {
    //     RSCUAD_INFO("penalty \n");
    // }


    /* gyro */
    // RSCUAD::Get()->serial->Process();
    // RSCUAD_INFO("data sensor gyro  : %d\n", RSCUAD::Get()->serial->Gyro());
    // RSCUAD_INFO("data sensor button: %d\n", RSCUAD::Get()->serial->Button());

    /* camera */
    bool ball = RSCUAD::Get()->camera->Process();
    float ball_x = RSCUAD::Get()->camera->GetX();
    float ball_y = RSCUAD::Get()->camera->GetY();
    bool goal =  RSCUAD::Get()->camera->GetPoint();
    float goal_x = RSCUAD::Get()->camera->GetLocX();
    float goal_y = RSCUAD::Get()->camera->GetLocY();
    bool robot = RSCUAD::Get()->camera->GetRobot();
    float robot_x = RSCUAD::Get()->camera->GetRobotX();
    float robot_y = RSCUAD::Get()->camera->GetRobotY();

    RSCUAD_PRINT("ball %d \n", ball);
    RSCUAD_PRINT("ball X %f \n", ball_x);
		RSCUAD_PRINT("ball Y %f \n", ball_y);
		RSCUAD_PRINT("goal %d \n", goal);
		RSCUAD_PRINT("goal X %f \n", goal_x);
		RSCUAD_PRINT("goal Y %f \n", goal_y);
    RSCUAD_PRINT("robot %d \n", robot);
    RSCUAD_PRINT("robot Y %f \n", robot_y);
    RSCUAD_PRINT("robot Y %f \n", robot_y);


    // RSCUAD::Get()->communication->Camera(RSCUAD::Get()->camera->GetFrame());

}

int main(int argc, char **argv)
{
 
  ros::init(argc, argv, "rscuad_module");
  ros::NodeHandle nh;

  image_transport::ImageTransport it(nh);
   
  image_transport::Subscriber imageSub = it.subscribe("video_camera", 1, &Camera::ImageCallback, RSCUAD::Get()->camera.get());

  ros::Subscriber dataSub = nh.subscribe("data_camera", 1000, &Camera::DataCallback, RSCUAD::Get()->camera.get());

  ros::Rate loop_rate(10);


  module_init();

  while (ros::ok())
  {
    module_test();

    ros::spinOnce();
    loop_rate.sleep();
  }


  return 0;
}

