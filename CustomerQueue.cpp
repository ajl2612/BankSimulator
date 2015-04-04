/*
 * CustomerQueue.cpp
 *
 *  Created on: Mar 31, 2015
 *      Author: ajl2612
 */

#include "CustomerQueue.h"

using namespace std;

CustomerQueue::CustomerQueue(pthread_mutex_t* p_mutex, int numTellers){
	this->mutex = p_mutex;
	this->maxDepth = 0;
	this->numTellers = numTellers;
	this->numTellersClosed = 0;
	this->timeToClose = false;
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
	pthread_mutex_lock(this->mutex);
	this->p_cQueue.push(c);
	pthread_mutex_unlock(this->mutex);
}

int CustomerQueue::getMaxDepth(){
	return maxDepth;
}

void *CustomerQueue::runProcess(void *threadid){

	CustomerQueue *self;
	self = (CustomerQueue *) threadid;

	printf("### START RUN PROCESS CUSTOMER QUEUE\n");

	int iter;
	bool bankOpen = false;
	Customer* p_next;

	self->sendMessage(CQU_ACTOR_START);
	while( self->shouldRun ){
		if( self->msgQueue.empty() ){
			usleep(1*SECONDS_T);
		}else{
			self->currentState = self->msgQueue.front();
			self->msgQueue.pop();

			switch(self->currentState){
			case CQU_ACTOR_START:
				printf("CUSTOMERQUEUE  : Queue made in front of tellers.\n");
				self->sendMessage(CQU_WAIT_OPEN);
				break;

			case CQU_WAIT_OPEN:
				printf("CUSTOMERQUEUE  : Ready and waiting for day to begin.\n");
				usleep(1*SECONDS_T);
				if( bankOpen ){
					printf("CUSTOMERQUEUE  : The queue is open.\n");
					self->sendMessage(CQU_WAIT_CLOSE);
				}
				else{
					self->sendMessage(CQU_WAIT_OPEN);
				}
				break;

			case CQU_OPEN_BANK:
				printf("CUSTOMERQUEUE  : The queue is signaled to open.\n");
				for(iter = 0; iter<NUM_TELLERS;iter++){
					self->p_tellers[iter]->sendMessage(TEL_OPEN_BANK);
				}
				bankOpen = true;
				break;

			case CQU_WAIT_CLOSE:
				//Wait to close bank until customer queue is empty and signal from security guard recieved.

				if( !(self->timeToClose) || ( !self->p_cQueue.empty()) || (self->numTellersClosed != NUM_TELLERS ) ){
					usleep(1*SECONDS_T);
					self->sendMessage(CQU_WAIT_CLOSE);
				}else{
					//do nothing
				}
				break;

			case CQU_SERVE_TEL0:
				pthread_mutex_lock(self->mutex);
				if( !self->p_cQueue.empty() ){
					//people in queue need to be processed.
					p_next = self->p_cQueue.front();
					self->p_cQueue.pop();

					printf("CUSTOMERQUEUE  : Sending customer %d to Teller 1.\n", p_next->getID());
					p_next->leaveQueue();
					self->p_tellers[0]->setCustomer(p_next);
					self->p_tellers[0]->sendMessage(TEL_SERVE_CUSTOMER);
					pthread_mutex_unlock(self->mutex);

				}else{
					pthread_mutex_unlock(self->mutex);
					if(!(self->timeToClose)){
						//queue is empty but bank is still open. delay 10 seconds to avoid deadlock resend the message to self to preserve it.
						usleep(TELL_RESEND_DELAY*SECONDS_T);
						self->sendMessage(CQU_SERVE_TEL0);
					}else{
						//bank is closed and queue is empty, send this teller home.
						self->p_tellers[0]->sendMessage(TEL_CLOSE_TERMINAL);
						self->numTellersClosed++;
						printf("%d tellers told to close\n", self->numTellersClosed);
						if(self->numTellersClosed == NUM_TELLERS ){
							self->sendMessage(CQU_CLOSE_QUEUE);
						}
					}
				}
				break;

			case CQU_SERVE_TEL1:
				pthread_mutex_lock(self->mutex);
				if( !self->p_cQueue.empty() ){
					//people in queue need to be processed.
					p_next = self->p_cQueue.front();
					self->p_cQueue.pop();

					printf("CUSTOMERQUEUE  : Sending customer %d to Teller 2.\n", p_next->getID());
					p_next->leaveQueue();
					self->p_tellers[1]->setCustomer(p_next);
					self->p_tellers[1]->sendMessage(TEL_SERVE_CUSTOMER);
					pthread_mutex_unlock(self->mutex);

				}else{
					pthread_mutex_unlock(self->mutex);
					if(!(self->timeToClose)){
						//queue is empty but bank is still open. delay 10 seconds to avoid deadlock resend the message to self to preserve it.
						usleep(TELL_RESEND_DELAY*SECONDS_T);
						self->sendMessage(CQU_SERVE_TEL1);
					}else{
						//bank is closed and queue is empty, send this teller home.
						self->p_tellers[1]->sendMessage(TEL_CLOSE_TERMINAL);
						self->numTellersClosed++;
						printf("%d tellers told to close\n", self->numTellersClosed);
						if(self->numTellersClosed == NUM_TELLERS ){
							self->sendMessage(CQU_CLOSE_QUEUE);
						}
					}
				}
			break;

			case CQU_SERVE_TEL2:
				pthread_mutex_lock(self->mutex);
				if( !self->p_cQueue.empty() ){
				//people in queue need to be processed.
					p_next = self->p_cQueue.front();
					self->p_cQueue.pop();

					printf("CUSTOMERQUEUE  : Sending customer %d to Teller 3.\n", p_next->getID());
					p_next->leaveQueue();
					self->p_tellers[2]->setCustomer(p_next);
					self->p_tellers[2]->sendMessage(TEL_SERVE_CUSTOMER);
					pthread_mutex_unlock(self->mutex);

				}else{
				pthread_mutex_unlock(self->mutex);
					if(!(self->timeToClose)){
						//queue is empty but bank is still open. delay 10 seconds to avoid deadlock resend the message to self to preserve it.
						usleep(TELL_RESEND_DELAY*SECONDS_T);
						self->sendMessage(CQU_SERVE_TEL2);
					}else{
						//bank is closed and queue is empty, send this teller home.
						self->p_tellers[2]->sendMessage(TEL_CLOSE_TERMINAL);
						self->numTellersClosed++;
						printf("%d tellers told to close\n", self->numTellersClosed);
						if(self->numTellersClosed == NUM_TELLERS ){
							self->sendMessage(CQU_CLOSE_QUEUE);
						}
					}
				}
				break;

			case CQU_SIGNAL_CLOSE:
				printf("CUSTOMERQUEUE  : Queue signaled to close.\n");
				bankOpen = false;
				self->timeToClose = true;
				break;

			case CQU_CLOSE_QUEUE:
				printf("CUSTOMERQUEUE  : Closing queue.\n");
				self->shouldRun = false;
				self->p_guard->sendMessage(SEC_CLOSE_BANK);
				break;

			default:
				printf("CUSTOMERQUEUE : INVALID MESSAGE CODE :%d\n",self->currentState);
				break;

			}
		}
	}
	printf("CUSTOMERQUEUE  : Today is over, but the queue excitedly awaiting tomorrow's customers.\n");
	printf("### END RUN PROCESS CUSTOMERQUEUE\n");
	pthread_exit(NULL);
}
