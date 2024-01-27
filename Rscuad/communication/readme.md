# Communication
> udp side client

## how to use?
- recommended                     
       
        RSCUAD::Get()->communication->Init("192.168.43.60", 4, true, 7, true);		// ip , robot , referee, team, realtime cam

        // loop area
        RSCUAD::Get()->communication->Robot(0,0,0,360,0,count,200);					// send data to server
        RSCUAD::Get()->communication->Camera(RSCUAD::Get()->camera->GetFrame());	// to send video stream to Basestation

- not recomended

        com.Init("192.168.43.60", 2, true, 7, true);	// ip , robot , referee, team[see config in GameController], realtime cam


        //loop area
        com.Robot(1,0,0,360,0,200,200);                // send data to server
        com.Camera(camera.GetFrame());                 // to send video stream to Basestation

- get data

        Communication::s_robot              			// get data robot return from server
        Communication::s_GC->STATE            			// get data state 
		Communication::s_GC->KICKOFF            		// get data kickoff 
		Communication::s_GC->SECOND_STATE		        // get data second state (penalty, corner etc. see gamecontroller.h) 
		Communication::s_GC->SECOND_STATE_TEAM          // get data team for second state 
		Communication::s_GC->SECOND_STATE_CONDITION     // get data condition state, (start, stop, end)


## very important for you to know
- New GameController maybe use different version, [change in referee.h]
- SPL and HL is very different after 2017 version
- Now we are included in HL not SPL
- HL used Port 3939 to return value from robot
- After 2016 SPL change Port return value to 3939
> Clarification from Arne Hasselbring. [maintenance GameController SPL].


## Architecture

![alt text](https://github.com/R-SCUAD-PROGRESS/Rscuad/blob/master/communication/img/flow%20basestation.drawio.png)
