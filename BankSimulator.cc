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
#include "Boss.h"
#include "BreakStopwatch.h"
#include "Stopwatch.h"

using namespace std;

int main(int argc, char *argv[]) {
	cout << "Welcome to the QNX Momentics IDE 1" << endl;
	int iter;

	pthread_mutex_t mutex;
	int pthread_mutex_init(pthread_mutex_t* mutex,const pthread_mutexattr_t* attr );

	time_t start;
	start = time(0);

	//Allocate pointers for all used objects/threads
	CustomerQueue* p_cQueue;
	Securityguard* p_guard;
	Boss* p_boss;
	Teller* p_tellers[NUM_TELLERS];
	BreakStopwatch* p_bwatches[NUM_TELLERS];
	Stopwatch* p_swatches[NUM_TELLERS];
	Customer* p_allCustomers[MAX_POSSIBLE_CUSTOMERS];
	EpochStopwatch* p_esw;


	//instantiate all used objects/threads

	//replace here
	p_esw = new EpochStopwatch(&start);


	p_cQueue = new CustomerQueue(&start, &mutex, p_tellers, 0);
	p_guard = new Securityguard(&start, p_cQueue, &mutex, p_allCustomers, p_esw);
	p_boss = new Boss(&start, p_guard);


	for(iter = 0; iter < NUM_TELLERS; iter++){
		p_tellers[iter] = new Teller(&start, p_cQueue, iter);
		p_bwatches[iter] = new BreakStopwatch(&start);
		p_swatches[iter] = new Stopwatch(&start);

		p_tellers[iter]->p_bsw = p_bwatches[iter];
		p_tellers[iter]->p_ssw = p_swatches[iter];

		p_bwatches[iter]->p_teller = p_tellers[iter];
		p_swatches[iter]->p_teller = p_tellers[iter];
	}

	// assign all inter-object links

	// replace here

	//printf("Teller 1 ID:%d stopwatch 1 ID: %d Bstopwatch 1 ID : %d \n", p_tellers[0]->id, p_swatches[0]->p_teller->id, p_bwatches[0]->p_teller->id  );

	// temp bug fix, to avoid garbage being set to p_guard value in CustomerQueue constructor;
	p_cQueue->p_guard = p_guard;


	// allocate all pthread runner threads
	pthread_t queueRunner;
	pthread_t guardRunner;
	pthread_t bossRunner;
	pthread_t tellRunners[NUM_TELLERS];
	pthread_t bWatchRunners[NUM_TELLERS];
	pthread_t sWatchRunners[NUM_TELLERS];


	// execute all pthread runner threads
	int rc;
	rc = pthread_create(&guardRunner, NULL, Securityguard::runProcess, (void *)p_guard);
	if (rc){
		cout << "Error:unable to create SecurityGuard thread," << rc << endl;
		exit(-1);
	}

	rc = pthread_create(&queueRunner, NULL, CustomerQueue::runProcess, (void *)p_cQueue);
	if (rc){
		cout << "Error:unable to create CustomerQueue thread," << rc << endl;
		exit(-1);
	}

	rc = pthread_create(&bossRunner, NULL, Boss::runProcess, (void *)p_boss);
	if (rc){
		cout << "Error:unable to create Boss thread," << rc << endl;
		exit(-1);
	}

	for(iter = 0; iter < NUM_TELLERS; iter++){
		rc = pthread_create(&tellRunners[iter], NULL, Teller::runProcess, (void *)p_tellers[iter]);
		if (rc){
			cout << "Error:unable to create Teller thread," << rc << endl;
			exit(-1);
		}
		rc = pthread_create(&bWatchRunners[iter], NULL, BreakStopwatch::runProcess, (void *)p_bwatches[iter]);
		if (rc){
			cout << "Error:unable to create BreakStopWatch thread," << rc << endl;
			exit(-1);
		}
		rc = pthread_create(&sWatchRunners[iter], NULL, Stopwatch::runProcess, (void *)p_swatches[iter]);
		if (rc){
			cout << "Error:unable to create StopWatch thread," << rc << endl;
			exit(-1);
		}
	}


	// wait for all threads to complete
	pthread_join(bossRunner, 0);
	pthread_join(guardRunner, 0);
	pthread_join(queueRunner, 0);
	for(iter=0; iter<NUM_TELLERS;iter++){
		pthread_join(tellRunners[iter], 0);
		pthread_join(bWatchRunners[iter], 0);
		pthread_join(sWatchRunners[iter], 0);
	}

	// post system statistics calculation and display begins here
	printf("\n### PRINTING SYSTEM STATISTICS ###\n\n");

	printf("\n	...beep boop I'm a computer...\n\n");
	for(iter = 0; iter < NUM_TELLERS; iter++){
		printf("Teller %d waited %d seconds\n", p_tellers[iter]->id, p_tellers[iter]->p_ssw->secsWaiting);
	}


	printf("\n### PRINTING SYSTEM STATISTICS ###\n\n");
	printf("TOTAL CUSTOMERS SERVICED			: %d\n", p_guard->getNumCustomers() );
	printf("AVG CUSTOMER TIME IN QUEUE			: $$$(MINUTES)\n");
	printf("AVG CUSTOMER TIME AT TELLER			: $$$(MINUTES)\n");
	printf("AVG TELLER WAIT TIME FOR CUSTOMERS		: $$$(MINUTES)\n");
	printf("MAX CUSTOMER TIME IN QUEUE			: $$$(MINUTES)\n");
	printf("MAX TELLER WAIT TIME FOR CUSTOMERS		: $$$(MINUTES)\n");
	printf("MAX TRANSACTION TIME				: $$$(MINUTES)\n");
	printf("MAX CUSTOMER QUEUE DEPTH			: %d\n", p_cQueue->getMaxDepth());


	return EXIT_SUCCESS;
}
