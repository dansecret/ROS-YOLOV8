#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#define HL_BALL_MANIPULATION                30
#define HL_PHYSICAL_CONTACT                 31
#define HL_ILLEGAL_ATTACK                   32
#define HL_ILLEGAL_DEFENSE                  33
#define HL_PICKUP_OR_INCAPABLE              34
#define HL_SERVICE                          35

#define GC_TEAM_COUNT						266
#define GC_PLAYER_DATA 						6
#define GC_PLAYER_NUMBER 					11
#define GC_PLAYER_COUT  					GC_PLAYER_DATA * GC_PLAYER_NUMBER
#define GC_DROP_BALL						128

enum GCDATA
{	GCD_HEADER 								= 0,	// BUFF
	GCD_VERSION 							= 4, // BUFF
	GCD_PACKET_NUMBER 						= 6,
	GCD_PLAYER_PER_TEAM 					= 7,
	GCD_TYPE 								= 8,
	GCD_STATE 								= 9,
	GCD_FIRST_HALF 							= 10,
	GCD_KICK_OF_TEAM 						= 11,
	GCD_SEC0NDARY_STATE 					= 12,
	GCD_SECONDARY_STATE_INFO 				= 13,	// 4 byte
	GCD_DROP_IN_TEAM 						= 17,
	GCD_DROP_IN_TIME 						= 18,	// 2
	GCD_SECONDS_REMAINING 					= 20, // 2
	GCD_SECONDARY_SECONDS_REMAINING 		= 22, // 2

	// TEAM 266x2 -> 24+266+66  = second team
	GCD_TEAM_NUMBER 						= 24,
	GCD_TEAM_COLOR 							= 25,
	GCD_SCORE 								= 26,
	GCD_PENALTY_SHOT 						= 27,
	GCD_SINGLE_SHOT 						= 28,	// 2 byte
	GCD_COACH_SEQUENCE 						= 30,
	GCD_COACH_MESSAGE 						= 31,	// 258

	// PLAYER 6x11 -> secon team + 266 =  second player first
	GCD_PENALTY 							= 290,
	GCD_SECS_TILT_UNPENALIZED 				= 192,
	GCD_NUMBER_OF_WARNING 					= 193,
	GCD_NUMBER_OF_YELLOW_CARD 				= 194,
	GCD_NUMBER_OF_RED_CARD 					= 195,
	GCD_GOAL_KEEPER 						= 296,

};

enum GCState
{
	GC_INIT 				= 0,
	GC_READY 				= 1,
	GC_SET 					= 2,
	GC_PLAY					= 3,
	GC_FINISH				= 4,
};

enum GCSecondState
{
	GC_PENALTY_SHOOT		= 1,
	GC_UNDEFINED			= 2,
	GC_TIMEOUT				= 3,
	GC_DIRECT_FREE_KICK 	= 4,
	GC_INDIRECT_FREE_KICK 	= 5,
	GC_PENALTY 				= 6,
	GC_CORNER_KICK 			= 7,
	GC_GOAL_KICK 			= 8,
	GC_THROW_IN 			= 9,
};

enum GCCondition
{
	GC_STOP 				= 0,
	GC_START 				= 1,
	GC_END 					= 2,
};

struct GameController
{
	int STATE;
	int KICKOFF;
	int SECOND_STATE;
	int SECOND_STATE_TEAM;
	int SECOND_STATE_CONDITION;
	bool IS_PICKUP;
	bool IS_DROP_BALL;
};

#endif