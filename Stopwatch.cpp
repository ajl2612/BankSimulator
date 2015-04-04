/*
 * Stopwatch.cpp
 *
 *  Created on: Apr 4, 2015
 *      Author: ajl2612
 */

#include "Stopwatch.h"

Stopwatch::Stopwatch(time_t* p_time) : Actor(p_time) {
	this->secsWaiting = 0;

}

Stopwatch::~Stopwatch() {
	// TODO Auto-generated destructor stub
}

int Stopwatch::getSecsWaiting(){
	return this->secsWaiting;
}

void *Stopwatch::runProcess(void *threadid){

	Stopwatch *self;
	self = (Stopwatch *) threadid;

	printf("### START RUN PROCESS : STOPWATCH %d\n", self->p_teller->id);
	//printf("### START RUN PROCESS : STOPWATCH DEBUG %d\n", self->secsWaiting);
	int curr_cmd;
	bool shouldRun = true;
	bool timerOn = false;

	self->sendMessage(SSW_ACTOR_START);
	while( shouldRun ){
		if( self->msgQueue.empty() ){
			usleep(1*SECONDS_T);
		}else{
			curr_cmd = self->msgQueue.front();
			self->msgQueue.pop();

			switch(curr_cmd){
			case SSW_ACTOR_START:
				printf("STOPWATCH %d   : Stop watch turned on.\n", self->p_teller->id);
				self->sendMessage(SSW_TICK);
				break;

			case SSW_START:
				printf("STOPWATCH %d   : Counting time.\n", self->p_teller->id);
				timerOn = true;
				break;

			case SSW_STOP:
				printf("STOPWATCH %d   : Counting time.\n", self->p_teller->id);
				timerOn = false;
				break;

			case SSW_TICK:
				if(timerOn){
					self->secsWaiting++;
				}
				usleep(1*SECONDS_T);
				self->sendMessage(SSW_TICK);
				break;

			case SSW_SHUTDOWN:
				printf("B STOPWATCH %d : Shutting down stop watch timer.\n", self->p_teller->id);
				printf("STOPWATCH %d   : Counted %d seconds\n", self->p_teller->id, self->secsWaiting);
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

