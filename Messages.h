/*
 * Messages.h
 *
 *  Created on: Apr 1, 2015
 *      Author: ajl2612
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_


// units below are in micro seconds for usleep function
#define	SECONDS_T				1666
#define MINUTES_T				100000
#define	FOR_NEXT_MESSAGE_T		100000
#define MAX_POSSIBLE_CUSTOMERS	480

#define NUM_TELLERS		3

//note that for this simulation 1 second = 100 ms

#define TIME_EPOCH				0
#define TIME_OPEN				60
#define TIME_CLOSE				480

#define TIME_EPOCH				0
#define DEMO_OPEN				5
#define DEMO_CLOSE				30



#define ALL_ACTOR_START			10
#define ALL_ACTOR_INIT			11

#define SEC_ACTOR_START			20
#define SEC_WAIT_OPEN			21
#define SEC_OPEN_BANK			22
#define SEC_GREET_CUSTOMERS		23
#define SEC_WAIT_CLOSE			24
#define SEC_CLOSE_BANK			25
#define SEC_SIGNAL_CLOSE		26

#define CQU_ACTOR_START			30
#define CQU_WAIT_OPEN			31
#define CQU_OPEN_BANK			32
#define CQU_WAIT_CLOSE			33
#define CQU_CLOSE_QUEUE			34
#define CQU_SERVE_TEL0			35
#define CQU_SERVE_TEL1			36
#define CQU_SERVE_TEL2			37
#define CQU_SIGNAL_CLOSE		38

#define TEL_ACTOR_START			40
#define TEL_WAIT_OPEN			41
#define TEL_OPEN_BANK			42
#define TEL_SERVE_CUSTOMER		43
#define TEL_CHECK_BREAK			44
#define TEL_WAIT_NEXT			45
#define TEL_CAN_BREAK			46
#define TEL_CLOSE_TERMINAL		47

#define BSW_ACTOR_START			50
#define BSW_START				51
#define BSW_SHUTDOWN			52

#define SSW_ACTOR_START			60
#define SSW_START				61
#define SSW_STOP				62
#define SSW_TICK				63
#define SSW_SHUTDOWN			64

#define ESW_ACTOR_START			70
#define ESW_TICK				72
#define ESW_SHUTDOWN			73



#endif /* MESSAGES_H_ */
