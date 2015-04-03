/*
 * Messages.h
 *
 *  Created on: Apr 1, 2015
 *      Author: ajl2612
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#define	SECONDS_T				1000000
#define	FOR_NEXT_MESSAGE_T		100000
#define MAX_POSSIBLE_CUSTOMERS	480

//note that for this simulation 1 second = 1 hour

#define TIME_EPOCH				0
#define TIME_OPEN				60
#define TIME_CLOSE				480

#define TIME_EPOCH				0
#define DEMO_OPEN				5
#define DEMO_CLOSE				30



#define ALL_ACTOR_START		10
#define ALL_ACTOR_INIT		11

#define SEC_ACTOR_START			20
#define SEC_WAIT_OPEN		21
#define SEC_OPEN_BANK			22
#define SEC_GREET_CUSTOMERS		23
#define SEC_WAIT_CLOSE			24
#define SEC_CLOSE_BANK			25

#define CQU_ACTOR_START			30
#define CQU_WAIT_OPEN			31
#define CQU_OPEN_BANK			32
#define CQU_WAIT_CLOSE			33
#define CQU_CLOSE_BANK			34


#endif /* MESSAGES_H_ */
