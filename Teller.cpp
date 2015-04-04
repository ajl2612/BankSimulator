/*
 * Teller.cpp
 *
 *  Created on: Apr 2, 2015
 *      Author: ajl2612
 */

#include "Teller.h"


Teller::Teller(time_t* p_time, CustomerQueue* p_cq, int myID) : Actor(p_time) {
	this->p_custQueue = p_cq;
	this-> p_custQueue = p_cq;
	this->id = myID;
	this->timeForBreak = false;
	this->secsWaitingForCustomers = 0;
}

Teller::~Teller() {

}

void Teller::setCustomer(Customer* p_newCust){
	this->p_cust = p_newCust;
}

void *Teller::runProcess(void *threadid){



	srand (time(NULL));

	Teller *self;
	self = (Teller *) threadid;

	printf("### START RUN PROCESS : TELLER %d\n", self->id);

	int curr_cmd;
	int range,wait,mins,secs, breakID;
	bool shouldRun = true;
	bool bankOpen = false;

	srand (time(NULL));

	self->sendMessage(TEL_ACTOR_START);
	while( shouldRun ){
		if( self->msgQueue.empty() ){
			usleep(1*SECONDS_T);
		}else{
			curr_cmd = self->msgQueue.front();
			self->msgQueue.pop();

			switch(curr_cmd){
			case TEL_ACTOR_START:
				printf("TELLER %d     : Arrives at work.\n", self->id);
				self->sendMessage(TEL_WAIT_OPEN);
				break;

			case TEL_WAIT_OPEN:

				printf("TELLER %d     : Waiting for bank doors to open.\n", self->id);
				usleep(10*SECONDS_T);

				break;

			case TEL_OPEN_BANK:
				printf("TELLER %d     : Terminal is open and waiting for customers.\n");
				bankOpen = true;


				//this part of code needs to be changed based on number of tellers
				switch(self->id){
				case 0:
					self->p_custQueue->sendMessage(CQU_SERVE_TEL0);
					break;
				case 1:
					self->p_custQueue->sendMessage(CQU_SERVE_TEL1);
					break;
				case 2:
					self->p_custQueue->sendMessage(CQU_SERVE_TEL2);
					break;
				default:
					break;
				}
				self->p_ssw->sendMessage(SSW_START);
				self->p_bsw->sendMessage(BSW_START);
				break;

			case TEL_SERVE_CUSTOMER:
				self->p_ssw->sendMessage(SSW_STOP);
				//self->secsWaitingForCustomers += difftime(time(NULL), self->lastCust);
				printf( "TELLER %d      : Hello Customer %d.\n",self->id, self->p_cust->getID());
				self->p_cust->startTeller();
				range = MAX_CUST_TIME - MIN_CUST_TIME + 1;
				wait = ((rand() % (range ) )+ MIN_CUST_TIME);
				mins = wait/60;
				secs = wait%60;

				usleep(wait*SECONDS_T);
				if(secs<10){
					printf( "TELLER %d      : Customer %d completed transactions in %d:0%d minutes.\n",self->id, self->p_cust->getID(),mins, secs);
				}
				else{
					printf( "TELLER %d      : Customer %d completed transactions in %d:%d minutes.\n",self->id, self->p_cust->getID(),mins, secs);
				}
				self->p_cust->endTeller();

				self->sendMessage(TEL_CHECK_BREAK);
				self->lastCust = time(NULL);
				break;

			case TEL_CHECK_BREAK:

				if( self->timeForBreak){
					range = MAX_TEL_BREAK - MIN_TEL_BREAK + 1;
					wait = ((rand() % (range ) )+ MIN_TEL_BREAK);
					mins = wait/60;
					secs = wait%60;

					breakID = (rand() % NUM_EXCUSES );

					string e = self->excuses[breakID];


					if(secs<10){
						printf( "TELLER %d      : %s back in %d:0%d minutes.\n", self->id, e.c_str(), mins, secs);
					}
					else{
						printf( "TELLER %d      : %s back in %d:%d minutes.\n", self->id, e.c_str(), mins, secs);
					}

					usleep(wait*SECONDS_T);

					self->timeForBreak = false;
					self->p_bsw->sendMessage(BSW_START);
				}



				switch(self->id){
				case 0:
					self->p_custQueue->sendMessage(CQU_SERVE_TEL0);
					break;
				case 1:
					self->p_custQueue->sendMessage(CQU_SERVE_TEL1);
					break;
				case 2:
					self->p_custQueue->sendMessage(CQU_SERVE_TEL2);
					break;
				default:
					break;
				}

				self->p_ssw->sendMessage(SSW_START);

				break;


//			case TEL_CAN_BREAK:
//				printf("TELLER %d       : FIX BREAK TEXT\n");
//				self->timeForBreak = true;
//				break;

			case TEL_CLOSE_TERMINAL:
				//self->secsWaitingForCustomers += difftime(time(NULL), self->lastCust);
				printf("TELLER %d      : Cashing out my terminal.\n", self->id);
				self->p_bsw->sendMessage(BSW_SHUTDOWN);
				self->p_ssw->sendMessage(SSW_SHUTDOWN);
				shouldRun = false;
				break;

			default:
				printf("TELLER %d       : INVALID MESSAGE CODE :%d\n",curr_cmd);
				break;

			}
		}
	}
	printf("TELLER%d       : I'm going home. See you all tomorrow.\n", self->id);
	printf("### END RUN PROCESS : TELLER %d\n", self->id);
	pthread_exit(NULL);
}

const string Teller::excuses[NUM_EXCUSES] =
	{	"I need to make a phone call, ",
		"I need to make a phone call, ",
		"I need to make a phone call, ",
		"I need to make a phone call, ",
		"I need to make a phone call, ",
		"I need to make a phone call, ",
		"I need to make a phone call, ",
		"I need to make a phone call, ",
		"I need to make a phone call, ",
		"I need to make a phone call, ",
		"I need a cup of coffee, ",
		"I need a cup of coffee, ",
		"I need a cup of coffee, ",
		"I need a cup of coffee, ",
		"I need a cup of coffee, ",
		"I need a cup of coffee, ",
		"I need a cup of coffee, ",
		"I need a cup of coffee, ",
		"I need a cup of coffee, ",
		"I need a cup of coffee, ",
		"I want a snack, ",
		"I want a snack, ",
		"I want a snack, ",
		"I want a snack, ",
		"I want a snack, ",
		"I want a snack, ",
		"I want a snack, ",
		"I want a snack, ",
		"I want a snack, ",
		"I want a snack, ",
		"Need to go listen to some music to relax, ",
		"Need to go listen to some music to relax, ",
		"Need to go listen to some music to relax, ",
		"Need to go listen to some music to relax, ",
		"Need to go listen to some music to relax, ",
		"Need to go listen to some music to relax, ",
		"Need to go listen to some music to relax, ",
		"Need to go listen to some music to relax, ",
		"Need to go listen to some music to relax, ",
		"Need to go listen to some music to relax, ",
		"I need to make an appointment, ",
		"I need to make an appointment, ",
		"I need to make an appointment, ",
		"I need to make an appointment, ",
		"I need to make an appointment, ",
		"I need to make an appointment, ",
		"I need to make an appointment, ",
		"I need to make an appointment, ",
		"I need to make an appointment, ",
		"I need to make an appointment, ",
		"My friend is outside, "
		"My friend is outside, "
		"My friend is outside, "
		"My friend is outside, "
		"My friend is outside, "
		"Need to check my instagram, ",
		"Need to check my instagram, ",
		"Need to check my instagram, ",
		"Need to check my instagram, ",
		"Need to check my instagram, ",
		"Need to check my facebook, ",
		"Need to check my facebook, ",
		"Need to check my facebook, ",
		"Need to check my facebook, ",
		"Need to check my facebook, ",
		"Need to check my tumblr,  ",
		"Need to check my tumblr, ",
		"Need to check my tumblr, ",
		"Need to check my myspace.... yes people still use it, ",
		"Need to check my myspace.... yes people still use it, ",
		"Some money in the back needs to be borrowed.... I mean counted, ",
		"I LITERALLY JUST CANT EVEN, ",
		"Uh, lemme take a selfie, ",
		"So I have a phone interview for another job right now.... ",
		"I think I just saw an alien outside AYY LMAO, ",
		"Yo my home dawgz are just outside, ",
		"I feel like taking a quick nap, ",
		"Taking a quick smoke break, ",
		"Are those guys outside in sunglasses watching me?, ",
		"Jet fuel doesn't melt steel beams, "};

