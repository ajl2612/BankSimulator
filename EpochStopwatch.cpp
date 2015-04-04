/*
 * EpochStopwatch.cpp
 *
 *  Created on: Apr 4, 2015
 *      Author: ajl2612
 */

#include "EpochStopwatch.h"

EpochStopwatch::EpochStopwatch(){
	this->secsSinceEpoch = 0;
}

EpochStopwatch::~EpochStopwatch() {
	// TODO Auto-generated destructor stub
}


int EpochStopwatch::getSecsSinceEpoch(){
	return this->secsSinceEpoch;
}

void *EpochStopwatch::runProcess(void *threadid){

	EpochStopwatch *self;
	self = (EpochStopwatch *) threadid;

	printf("### START RUN PROCESS EPOCH STOPWATCH\n");

	self->sendMessage(ESW_ACTOR_START);
	while( self->shouldRun ){
		if( self->msgQueue.empty() ){
			usleep(1*SECONDS_T);
		}else{
			self->currentState = self->msgQueue.front();
			self->msgQueue.pop();

			switch(self->currentState){
			case ESW_ACTOR_START:
				printf("EPOCH STOPWATCH: Epoch time enabled.\n");
				self->sendMessage(ESW_TICK);
				break;

			case ESW_TICK:
				self->secsSinceEpoch++;
				usleep(1*SECONDS_T);
				self->sendMessage(ESW_TICK);
				break;

			case ESW_SHUTDOWN:
				printf("EPOCH STOPWATCH: Epoch time disabled.\n");
				self->shouldRun = false;
				break;

			default:
				printf("EPOCH STOPWATCH: INVALID MESSAGE CODE :%d\n", self->currentState);
				break;
			}
		}
	}
	printf("EPOCH STOPWATCH: Epoch time turned off.\n");
	printf("### END RUN PROCESS EPOCH STOPWATCH\n");
	pthread_exit(NULL);
}
