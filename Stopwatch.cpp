/*
 * Stopwatch.cpp
 *
 *  Created on: Apr 4, 2015
 *      Author: ajl2612
 */

#include "Stopwatch.h"

Stopwatch::Stopwatch(){
	this->secsWaiting = 0;
}

Stopwatch::~Stopwatch() {
	// default destructor
}

int Stopwatch::getSecsWaiting(){
	return this->secsWaiting;
}

void *Stopwatch::runProcess(void *threadid){

	Stopwatch *self;
	self = (Stopwatch *) threadid;

	printf("### START RUN PROCESS STOPWATCH %d\n", self->p_teller->id);

	bool timerOn = false;

	self->sendMessage(SSW_ACTOR_START);
	while( self->shouldRun ){
		if( self->msgQueue.empty() ){
			usleep(1*SECONDS_T);
		}else{
			self->currentState = self->msgQueue.front();
			self->msgQueue.pop();

			switch(self->currentState){
			case SSW_ACTOR_START:
				self->sendMessage(SSW_TICK);
				break;

			case SSW_START:
				timerOn = true;
				break;

			case SSW_STOP:
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
				printf("STOPWATCH %d    : Shutting down stop watch timer.\n", self->p_teller->id);
				self->shouldRun = false;
				break;


			default:
				printf("STOPWATCH %d    : INVALID MESSAGE CODE :%d\n", self->p_teller->id, self->currentState);
				break;
			}
		}
	}
	printf("STOPWATCH %d    :  Stop watch turned off.\n", self->p_teller->id);
	printf("### END RUN PROCESS STOPWATCH %d\n", self->p_teller->id);
	pthread_exit(NULL);
}

