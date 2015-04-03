/*
 * Securityguard.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: ajl2612
 */

#include "Securityguard.h"

using namespace std;

Securityguard::Securityguard(time_t* p_time, CustomerQueue* p_cq, pthread_mutex_t* p_mutex, Customer** customerList ) : Actor( p_time ) {
	this->customers = customerList;
	this->p_custQueue = p_cq;
	this->numCustomers = 0;
	this->mutex = p_mutex;
}

Securityguard::~Securityguard() {
	// TODO Auto-generated destructor stub
}

int Securityguard::getNumCustomers(){
	return this->numCustomers;
}

Customer* Securityguard::getFirstCustomer(){
	return this->customers[0];
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
	self->p_custQueue->sendMessage(CQU_ACTOR_START);
	while( shouldRun ){
		if( self->msgQueue.empty() ){
			printf("### SECURITY GUARD EMPTY QUEUE\n");
			usleep(FOR_NEXT_MESSAGE_T);
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
				//TODO this is incorrect.

				curr_time = difftime( time(0), self->epoch );
				if( curr_time < DEMO_OPEN){
					usleep(1*SECONDS_T);
					self->sendMessage(SEC_WAIT_OPEN);
				}
				else{
					//time to open bank
					self->sendMessage(SEC_OPEN_BANK);
				}
				break;

			case SEC_OPEN_BANK:
				printf("SECURITYGUARD : The bank doors are open.\n");
				self->sendMessage(SEC_GREET_CUSTOMERS);
				self->p_custQueue->sendMessage(CQU_OPEN_BANK);
				break;

			case SEC_GREET_CUSTOMERS:
				/*
				 * Get the current time. If it is not time to close the bank,
				 * greet a new customer and direct them to the queue.
				 *
				 * If it is time to close the bank, signal so.
				 */

				curr_time = difftime( time(0), self->epoch );
				if( curr_time < DEMO_CLOSE ){


					p_newCust = new Customer(self->numCustomers + 1);
					self->customers[self->numCustomers] = p_newCust;

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
					printf( "SECURITYGUARD : Next customer arrives %d minutes later.\n",wait);
					usleep(wait*SECONDS_T);

					self->sendMessage(SEC_GREET_CUSTOMERS);
				}
				else{
					self->sendMessage(SEC_WAIT_CLOSE);
				}
				break;

			case SEC_WAIT_CLOSE:
				//Wait to close bank until customer queue is empty.
				printf("SECURITYGUARD : Waiting to close bank doors.\n");
				if( !self->p_custQueue->empty()){
					//TODO remove this
					printf("$$$$$$$DEMO guard removing all customers from queue\n");
					while( ! self->p_custQueue->empty() ){
						self->p_custQueue->pop();
					}
					self->sendMessage(SEC_WAIT_CLOSE);
				}
				else{
					self->sendMessage(SEC_CLOSE_BANK);
				}
				break;

			case SEC_CLOSE_BANK:
				printf("SECURITYGUARD : The bank door is closed.\n");
				shouldRun = false;
				self->p_custQueue->sendMessage(CQU_CLOSE_BANK);
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

