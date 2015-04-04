/*
 * EpochStopwatch.cpp
 *
 *  Created on: Apr 4, 2015
 *      Author: ajl2612
 */

#include "EpochStopwatch.h"

EpochStopwatch::EpochStopwatch(time_t* p_time) : Actor(p_time)  {
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

	printf("### START RUN PROCESS : EPOCH STOPWATCH\n");
	int curr_cmd;
	bool shouldRun = true;

	self->sendMessage(ESW_ACTOR_START);
	while( shouldRun ){
		if( self->msgQueue.empty() ){
			usleep(1*SECONDS_T);
		}else{
			curr_cmd = self->msgQueue.front();
			self->msgQueue.pop();

			switch(curr_cmd){
			case ESW_ACTOR_START:
				printf("EPOCH STOPWATCH   : Epoch time enabled.\n");
				self->sendMessage(ESW_TICK);
				break;

			case ESW_TICK:
				self->secsSinceEpoch++;
				usleep(1*SECONDS_T);
				self->sendMessage(SSW_TICK);
				break;

			case ESW_SHUTDOWN:
				printf("EPOCH STOPWATCH   : Epoch time disabled.\n");
				shouldRun = false;
				break;


			default:
				printf("EPOCH STOPWATCH : INVALID MESSAGE CODE :%d\n", curr_cmd);
				break;

			}
		}
	}
	printf("EPOCH STOPWATCH : Epoch time turned off.\n");
	printf("### END RUN PROCESS : EPOCH STOPWATCH\n");
	pthread_exit(NULL);
}
