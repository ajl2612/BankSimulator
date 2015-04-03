#include <stdlib.h>     //for using the function sleep
#include <iostream>
#include <time.h>		// for using time object
#include <unistd.h>		//for using usleep

#include <sys/time.h>

#include "Customer.h"
#include "CustomerQueue.h"
#include "Securityguard.h"
#include "Teller.h"
#include "Messages.h"

#define	SECONDS_T		1000000
#define NUM_THREADS     5
#define NUM_TELLERS		3

using namespace std;

void *demoProcess(void *threadid){

	printf("### START RUN PROCESS : SECURITYGUARD\n");

	Securityguard *self;
	self = (Securityguard *) threadid;



	printf("### BEFORE FOR\n");
	bool shouldRun = true;
	double duration;

	while( shouldRun ){

		duration = difftime( time(0), self->epoch );
		printf( "Time Difference : %f\n", duration);
		if( duration < 10){
			usleep(1*SECONDS_T);
		}
		else{
			shouldRun = false;
		}
	}
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	cout << "Welcome to the QNX Momentics IDE 1" << endl;

	Customer* allCustomers[MAX_POSSIBLE_CUSTOMERS];


	pthread_mutex_t mutex;
	int pthread_mutex_init(pthread_mutex_t* mutex,const pthread_mutexattr_t* attr );

	CustomerQueue* p_cQueue;

	time_t start;
	start = time(0);

	Teller* tellers[NUM_TELLERS];
	int iter;
	for(iter = 0; iter < NUM_TELLERS; iter++){
		tellers[iter] = new Teller(&start);
	}

	p_cQueue = new CustomerQueue(&start, &mutex);

	Securityguard* p_guard;
	p_guard = new Securityguard(&start, p_cQueue, &mutex, allCustomers);


	int rc;
	pthread_t guardRunner;
	pthread_t queueRunner;

	rc = pthread_create(&guardRunner, NULL, Securityguard::runProcess, (void *)p_guard);
	//rc = pthread_create(&guardRunner, NULL, demoProcess, (void *)g);
	if (rc){
		cout << "Error:unable to create SecurityGuard thread," << rc << endl;
		exit(-1);
	}

	rc = pthread_create(&queueRunner, NULL, CustomerQueue::runProcess, (void *)p_cQueue);
	//rc = pthread_create(&guardRunner, NULL, demoProcess, (void *)g);
	if (rc){
		cout << "Error:unable to create SecurityGuard thread," << rc << endl;
		exit(-1);
	}

	pthread_join(guardRunner, 0);
	pthread_join(queueRunner, 0);


	printf("\n### PRINTING SYSTEM STATISTICS ###\n\n");

	int numCustomers = p_guard->getNumCustomers();

	for(iter = 0; iter < numCustomers; iter++){
		printf("%d\n", allCustomers[iter]->getID());
	}


	printf("\n	...beep boop I'm a computer...\n\n");


	printf("\n### PRINTING SYSTEM STATISTICS ###\n\n");
	printf("TOTAL CUSTOMERS SERVICED			: %d\n", p_guard->getNumCustomers() );
	printf("AVG CUSTOMER TIME IN QUEUE			: $$$(MINUTES)\n");
	printf("AVG CUSTOMER TIME AT TELLER			: $$$(MINUTES)\n");
	printf("AVG TELLER WAIT TIME FOR CUSTOMERS		: $$$(MINUTES)\n");
	printf("MAX CUSTOMER TIME IN QUEUE			: $$$(MINUTES)\n");
	printf("MAX TELLER WAIT TIME FOR CUSTOMERS		: $$$(MINUTES)\n");
	printf("MAX TRANSACTION TIME				: $$$(MINUTES)\n");
	printf("MAX CUSTOMER QUEUE DEPTH			: %d\n", p_cQueue->getMaxDepth());


	/**
	for(int i = 0; i<NUM_THREADS; i++){
		c = new Customer(i);
		cust_arr[i] = c;
		cQueue->push(c);
	}

	printf("BEFORE Queue Size : %d\n",cQueue->size());
	printf("BEFORE Is Empty : %d\n",cQueue->empty());

	for(int i = 0; i<NUM_THREADS; i++){
		c = cQueue->front();
		cQueue->pop();

		printf("Current Customer : %d\n",c->getID());
		printf("Queue Size : %d\n",cQueue->size());
	}

	printf("AFTER Queue Size : %d\n",cQueue->size());
	printf("AFTER Is Empty : %d\n",cQueue->empty());


	pthread_t threads[NUM_THREADS];
	int rc;
	int i;
	for( i=0; i < NUM_THREADS; i++ ){
		cout << "main() : creating thread, " << i << endl;
		rc = pthread_create(&threads[i], NULL, CustomerQueue::runProcess, (void *)cust_arr[i]);
		if (rc){
			cout << "Error:unable to create thread," << rc << endl;
			exit(-1);
		}
	}
	pthread_exit(NULL);
	**/

	/**
	time_t start;
	time_t end;

	double duration;


	start = time(NULL);
	usleep(10000);
	end = time(NULL);




	duration = difftime( end, start ) *1000;

	printf("Printing timers\n");
	cout << duration << endl;
**/
	return EXIT_SUCCESS;
}
