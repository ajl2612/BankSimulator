/*
 * BreakStopwatch.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: ajl2612
 */

#include "BreakStopwatch.h"

BreakStopwatch::BreakStopwatch(time_t* p_time) : Actor(p_time)  {
	//this->num = -1;
}

BreakStopwatch::~BreakStopwatch() {
	// TODO Auto-generated destructor stub
}

void *BreakStopwatch::runProcess(void *threadid){

	BreakStopwatch *self;
	self = (BreakStopwatch *) threadid;

	printf("### START RUN PROCESS : BREAK STOPWATCH %d\n", self->p_teller->id);
	//printf("### START RUN PROCESS : BREAK DEBUGG %d\n", 0);

	int curr_cmd;
	bool shouldRun = true;

	self->sendMessage(BSW_ACTOR_START);
	while( shouldRun ){
		if( self->msgQueue.empty() ){
			usleep(1*SECONDS_T);
		}else{
			curr_cmd = self->msgQueue.front();
			self->msgQueue.pop();

			switch(curr_cmd){
			case BSW_ACTOR_START:
				printf("B STOPWATCH %d : Break stop watch turned on.\n", self->p_teller->id);
				break;

			case BSW_START:
				printf("B STOPWATCH %d : Starting new Break timer.\n", self->p_teller->id);
				//wait for 30 secs system time then tell teller they can go on break.

				int minutes;
				for(minutes=0; minutes < 30; minutes++){
					usleep(1*MINUTES_T);
				}

				printf("B STOPWATCH %d : Break timer complete.\n", self->p_teller->id);
				self->p_teller->timeForBreak = true;
				break;


			case BSW_SHUTDOWN:
				printf("B STOPWATCH %d : Shutting down break stop watch timer.\n", self->p_teller->id);
				shouldRun = false;
				break;


			default:
				printf("B STOPWATCH %d : INVALID MESSAGE CODE :%d\n", self->p_teller->id, curr_cmd);
				break;

			}
		}
	}
	printf("B STOPWATCH %d : Break stop watch turned off.\n", self->p_teller->id);
	printf("### END RUN PROCESS : B STOPWATCH %d\n", self->p_teller->id);
	pthread_exit(NULL);
}
