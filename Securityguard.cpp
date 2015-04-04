/*
 * Securityguard.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: ajl2612
 */

#include "Securityguard.h"

using namespace std;

Securityguard::Securityguard(time_t* p_time, CustomerQueue* p_cq, pthread_mutex_t* p_mutex, Customer** p_customerList, EpochStopwatch* p_epoch ) : Actor( p_time ) {
	this->p_customers = p_customerList;
	this->p_custQueue = p_cq;
	this->numCustomers = 0;
	this->mutex = p_mutex;
	this->p_esw = p_epoch;
	this->timeToClose = false;
}

Securityguard::~Securityguard() {
	// TODO Auto-generated destructor stub
}

int Securityguard::getNumCustomers(){
	return this->numCustomers;
}

Customer* Securityguard::getFirstCustomer(){
	return this->p_customers[0];
}

void *Securityguard::runProcess(void *threadid){

	printf("### START RUN PROCESS : SECURITYGUARD\n");

	srand (time(NULL));



	Customer* p_newCust;
	Securityguard *self;
	self = (Securityguard *) threadid;

	int curr_cmd;
	bool shouldRun = true;
	double curr_time;

	self->sendMessage(SEC_ACTOR_START);
	while( shouldRun ){
		if( self->msgQueue.empty() ){
			usleep(1*SECONDS_T);
		}else{
			curr_cmd = self->msgQueue.front();
			self->msgQueue.pop();

			switch(curr_cmd){
			case SEC_ACTOR_START:
				printf("SECURITYGUARD : Arrives at work.\n");
				self->sendMessage(SEC_WAIT_OPEN);
				break;

			case SEC_WAIT_OPEN:

				printf("SECURITYGUARD : Waiting to open bank doors.\n");
				usleep(10*SECONDS_T);

				break;

			case SEC_OPEN_BANK:
				printf("SECURITYGUARD : The bank doors are open.\n");
				self->p_custQueue->sendMessage(CQU_OPEN_BANK);
				self->sendMessage(SEC_GREET_CUSTOMERS);

				break;

			case SEC_GREET_CUSTOMERS:
				/*
				 * Get the current time. If it is not time to close the bank,
				 * greet a new customer and direct them to the queue.
				 *
				 * If it is time to close the bank, signal so.
				 */

				curr_time = difftime( time(0), self->epoch );
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
					printf("SECURITYGUARD : Hello customer %d, step this way.\n", p_newCust->getID());

					//wait a random amount of time for the next customer

					int range = C_UPPB - C_LOWB + 1;
					int wait = ((rand() % (range ) )+ C_LOWB);
					int mins = wait/60;
					int secs = wait%60;

					if(secs<10){
						printf( "SECURITYGUARD : Next customer arrives %d:0%d minutes later.\n",mins, secs);
					}
					else{
						printf( "SECURITYGUARD : Next customer arrives %d:%d minutes later.\n",mins, secs);
					}
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
				printf("SECURITYGUARD : Waiting to close bank doors.\n");
				usleep(10*SECONDS_T);
				self->sendMessage(SEC_WAIT_CLOSE);


				//break;

			case SEC_CLOSE_BANK:
				printf("SECURITYGUARD : The bank door is closed.\n");
				shouldRun = false;
				//self->p_custQueue->sendMessage(CQU_CLOSE_BANK);
				break;

			default:
				printf("SECURITYGUARD : INVALID MESSAGE CODE :%d\n",curr_cmd);
				break;

			}
		}
	}
	printf("SECURITYGUARD : I'm going home. See you all tomorrow.\n");
	printf("### END RUN PROCESS : SECURITYGUARD\n");
	pthread_exit(NULL);
}

