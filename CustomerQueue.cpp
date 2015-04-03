/*
 * CustomerQueue.cpp
 *
 *  Created on: Mar 31, 2015
 *      Author: ajl2612
 */

#include "CustomerQueue.h"

using namespace std;

CustomerQueue::CustomerQueue(time_t* p_time, pthread_mutex_t* p_mutex) : Actor(p_time) {
	this->mutex = p_mutex;
}

CustomerQueue::~CustomerQueue() {
	Customer* c;
	while( !this->empty() ){
		c = this->front();
		this->pop();
		delete c;
	}
}

bool CustomerQueue::empty(){
	return this->p_cQueue.empty();
}

int CustomerQueue::size(){
	return this->p_cQueue.size();
}

Customer* CustomerQueue::front(){
	return this->p_cQueue.front();
}

void CustomerQueue::pop(){
	this->p_cQueue.pop();
}

void CustomerQueue::push(Customer* c){
	//int pthread_mutex_lock( pthread_mutex_t* mutex );
	this->p_cQueue.push(c);
	//int pthread_mutex_unlock( pthread_mutex_t* mutex );
}

pthread_mutex_t* CustomerQueue::getMutex(){
	return mutex;
}

int CustomerQueue::getMaxDepth(){
	return maxDepth;
}

void *CustomerQueue::runProcess(void *threadid){

	printf("### START RUN PROCESS : CUSTOMER QUEUE\n");

	CustomerQueue *self;
	self = (CustomerQueue *) threadid;

	int curr_cmd;
	bool shouldRun = true;
	bool bankOpen = false;
	double curr_time;

	while( shouldRun ){
		if( self->msgQueue.empty() ){
			usleep(FOR_NEXT_MESSAGE_T);
		}else{
			curr_cmd = self->msgQueue.front();
			self->msgQueue.pop();

			switch(curr_cmd){
			case CQU_ACTOR_START:
				printf("CUSTOMERQUEUE : Queue made in front of tellers.\n");
				self->sendMessage(CQU_WAIT_OPEN);
				break;

			case CQU_WAIT_OPEN:
				printf("CUSTOMERQUEUE : Ready and waiting for day to begin.\n");
				usleep(1*SECONDS_T);
				if( bankOpen ){
					printf("CUSTOMERQUEUE : The queue is open.\n");
					self->sendMessage(CQU_WAIT_CLOSE);
				}
				else{
					self->sendMessage(CQU_WAIT_OPEN);
				}
				break;

			case CQU_OPEN_BANK:
				printf("CUSTOMERQUEUE : The queue is signaled to open.\n");
				bankOpen = true;
				break;

			case CQU_WAIT_CLOSE:
				//Wait to close bank until customer queue is empty and signal from security guard recieved.

				if( bankOpen || ( !self->p_cQueue.empty() ) ){
					usleep(1*SECONDS_T);
					self->sendMessage(CQU_WAIT_CLOSE);
				}else{
					printf("CUSTOMERQUEUE : Closing queue.\n");
					shouldRun = false;
				}
				/*
				printf("CUSTOMERQUEUE : Waiting for bank to close.\n");
				if( !self->p_cQueue.empty()){
					printf("CUSTOMERQUEUE : Can not close queue while it contains customers.\n");
					usleep(1*SECONDS_T);
					self->sendMessage(CQU_WAIT_CLOSE);
				}
				else if( bankOpen ){
					printf("CUSTOMERQUEUE : Awaiting shutdown signal from Security Guard.\n");
					usleep(1*SECONDS_T);
					self->sendMessage(CQU_WAIT_CLOSE);
				}
				else{
					printf("CUSTOMERQUEUE : Closing queue.\n");
					shouldRun = false;
				}
				*/
				break;

			case CQU_CLOSE_BANK:
				printf("CUSTOMERQUEUE : Queue signaled to close.\n");
				bankOpen = false;
				break;

			default:
				printf("CUSTOMERQUEUE : INVALID MESSAGE CODE :%d\n",curr_cmd);
				break;

			}
		}
	}
	printf("CUSTOMERQUEUE : Today is over, but the queue excitedly awaiting tomorrow's customers.\n");
	printf("### END RUN PROCESS : CUSTOMERQUEUE\n");
	pthread_exit(NULL);
}
