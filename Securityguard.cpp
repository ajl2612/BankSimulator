/*
 * Securityguard.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: ajl2612
 */

#include "Securityguard.h"

using namespace std;

Securityguard::Securityguard(pthread_mutex_t* p_mutex){
	this->numCustomers = 0;
	this->timeToClose = false;
	this->mutex = p_mutex;
}

Securityguard::~Securityguard() {
	// default destructor
}

int Securityguard::getNumCustomers(){
	return this->numCustomers;
}

void *Securityguard::runProcess(void *threadid){

	Securityguard *self;
	self = (Securityguard *) threadid;

	printf("### START RUN PROCESS SECURITYGUARD\n");

	Customer* p_newCust;

	srand (time(NULL));

	self->sendMessage(SEC_ACTOR_START);
	while( self->shouldRun ){
		if( self->msgQueue.empty() ){
			usleep(1*SECONDS_T);
		}else{
			self->currentState = self->msgQueue.front();
			self->msgQueue.pop();

			switch(self->currentState){
			case SEC_ACTOR_START:
				printf("SECURITYGUARD  : Arrives at work.\n");
				self->sendMessage(SEC_WAIT_OPEN);
				break;

			case SEC_WAIT_OPEN:

				printf("SECURITYGUARD  : Waiting to open bank doors.\n");
				usleep(10*SECONDS_T);

				break;

			case SEC_OPEN_BANK:
				printf("SECURITYGUARD  : The bank doors are open.\n");
				self->p_custQueue->sendMessage(CQU_OPEN_BANK);
				self->sendMessage(SEC_GREET_CUSTOMERS);

				break;

			case SEC_GREET_CUSTOMERS:

				if( !self->timeToClose ){


					p_newCust = new Customer(self->numCustomers + 1, self->p_esw);
					self->p_customers[self->numCustomers] = p_newCust;

					pthread_mutex_lock(self->mutex);
					p_newCust->enterQueue();
					self->p_custQueue->push(p_newCust);
					if(self->p_custQueue->size() > self->p_custQueue->maxDepth){
						self->p_custQueue->maxDepth = self->p_custQueue->size();
					}
					pthread_mutex_unlock(self->mutex);
					self->numCustomers++;
					printf("SECURITYGUARD  : Hello customer %d, step this way.\n", p_newCust->getID());

					//wait a random amount of time for the next customer

					int range = C_UPPB - C_LOWB + 1;
					int wait = ((rand() % (range ) )+ C_LOWB);
					int mins = wait/60;
					int secs = wait%60;

					printf( "SECURITYGUARD  : Next customer arrives %d:%.2d minutes later.\n",mins, secs);
					usleep(wait*SECONDS_T);

					self->sendMessage(SEC_GREET_CUSTOMERS);
				}
				else{
					self->sendMessage(SEC_WAIT_CLOSE);
				}
				break;

			case SEC_SIGNAL_CLOSE:
				self->timeToClose = true;
				self->p_custQueue->sendMessage(CQU_SIGNAL_CLOSE);
				break;

			case SEC_WAIT_CLOSE:
				//Wait to close bank until customer queue is empty.
				printf("SECURITYGUARD  : Waiting to close bank doors.\n");
				usleep(1*SECONDS_T);
				self->sendMessage(SEC_WAIT_CLOSE);
				break;

			case SEC_CLOSE_BANK:
				printf("SECURITYGUARD  : The bank door is closed.\n");
				self->shouldRun = false;
				self->p_esw->sendMessage(ESW_SHUTDOWN);
				break;

			default:
				printf("SECURITYGUARD  : INVALID MESSAGE CODE :%d\n",self->currentState);
				break;
			}
		}
	}
	printf("SECURITYGUARD  : I'm going home. See you all tomorrow.\n");
	printf("### END RUN PROCESS SECURITYGUARD\n");
	pthread_exit(NULL);
}

