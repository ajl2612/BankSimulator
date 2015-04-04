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


	printf("///////////////////////// START BANK SIMULATION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
	int iter, tcs, mctq, mtwc, mtt, mcqd, actq, actt, atwc, temp1, temp2 = 0;

	/*
	 * Create mutex for threads that require shared mutable state.
	 */

	pthread_mutex_t mutex;
	int pthread_mutex_init(pthread_mutex_t* mutex,const pthread_mutexattr_t* attr );


	/*
	 * Allocate pointers for all used objects/threads.
	 */

	Customer* p_allCustomers[MAX_POSSIBLE_CUSTOMERS];
	EpochStopwatch* p_esw;
	Boss* p_boss;
	Securityguard* p_guard;
	CustomerQueue* p_cQueue;
	Teller* p_tellers[NUM_TELLERS];
	BreakStopwatch* p_bwatches[NUM_TELLERS];
	Stopwatch* p_swatches[NUM_TELLERS];


	/*
	 * Instantiate all used objects/threads.
	 */

	p_esw = new EpochStopwatch();
	p_boss = new Boss();
	p_guard = new Securityguard(&mutex);
	p_cQueue = new CustomerQueue(&mutex, NUM_TELLERS);

	for(iter = 0; iter < NUM_TELLERS; iter++){
		p_tellers[iter] = new Teller(iter);
		p_bwatches[iter] = new BreakStopwatch();
		p_swatches[iter] = new Stopwatch();
	}


	/*
	 * Assign all inter-object links.
	 */

	p_boss->p_guard = p_guard;

	p_guard->p_customers = p_allCustomers;
	p_guard->p_custQueue = p_cQueue;
	p_guard->p_esw = p_esw;

	p_cQueue->p_tellers = p_tellers;
	p_cQueue->p_guard = p_guard;

	for(iter=0; iter < NUM_TELLERS; iter++){
		p_tellers[iter]->p_custQueue = p_cQueue;
		p_tellers[iter]->p_bsw = p_bwatches[iter];
		p_tellers[iter]->p_ssw = p_swatches[iter];
	}

	for(iter=0; iter < NUM_TELLERS; iter++){
		p_bwatches[iter]->p_teller = p_tellers[iter];
		p_swatches[iter]->p_teller = p_tellers[iter];
	}


	/*
	 *  Allocate all pthread runner threads.
	 */

	pthread_t epochRunner;
	pthread_t bossRunner;
	pthread_t guardRunner;
	pthread_t queueRunner;
	pthread_t tellRunners[NUM_TELLERS];
	pthread_t bWatchRunners[NUM_TELLERS];
	pthread_t sWatchRunners[NUM_TELLERS];


	/*
	 * Execute all pthread runner threads.
	 * Executed in reverse order of communication with epoch timer first to minimize system time with some threads not started.
	 */

	int rc;
	rc = pthread_create(&epochRunner, NULL, EpochStopwatch::runProcess, (void *)p_esw);
	if (rc){
		cout << "Error:unable to create SecurityGuard thread," << rc << endl;
		exit(-1);
	}

	for(iter = 0; iter < NUM_TELLERS; iter++){

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
		rc = pthread_create(&tellRunners[iter], NULL, Teller::runProcess, (void *)p_tellers[iter]);
		if (rc){
			cout << "Error:unable to create Teller thread," << rc << endl;
			exit(-1);
		}
	}

	rc = pthread_create(&queueRunner, NULL, CustomerQueue::runProcess, (void *)p_cQueue);
	if (rc){
		cout << "Error:unable to create CustomerQueue thread," << rc << endl;
		exit(-1);
	}

	rc = pthread_create(&guardRunner, NULL, Securityguard::runProcess, (void *)p_guard);
	if (rc){
		cout << "Error:unable to create SecurityGuard thread," << rc << endl;
		exit(-1);
	}

	rc = pthread_create(&bossRunner, NULL, Boss::runProcess, (void *)p_boss);
	if (rc){
		cout << "Error:unable to create Boss thread," << rc << endl;
		exit(-1);
	}


	/*
	 * Wait for all threads to complete.
	 */

	pthread_join(epochRunner, 0);
	pthread_join(bossRunner, 0);
	pthread_join(guardRunner, 0);
	pthread_join(queueRunner, 0);
	for(iter=0; iter<NUM_TELLERS;iter++){
		pthread_join(tellRunners[iter], 0);
		pthread_join(bWatchRunners[iter], 0);
		pthread_join(sWatchRunners[iter], 0);
	}

	printf("////////////////// ALL SYSTEM THREADS TERMINALED SAFELY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n \n");


	/*
	 *  Post system statistics calculation and display begins here.
	 */

	printf("////////////////////// CALCULATING SYSTEM STATISTICS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");

	tcs = p_guard->getNumCustomers();

	for(iter = 0; iter < tcs; iter++){
		temp1 += ( p_allCustomers[iter]->getLeaveQueue() - p_allCustomers[iter]->getEnterQueue() );
		temp2 += ( p_allCustomers[iter]->getEndTeller() - p_allCustomers[iter]->getStartTeller() );

		if(p_allCustomers[iter]->getLeaveQueue() - p_allCustomers[iter]->getEnterQueue() > mctq ){
			mctq = p_allCustomers[iter]->getLeaveQueue() - p_allCustomers[iter]->getEnterQueue();
		}

		if(( p_allCustomers[iter]->getEndTeller() - p_allCustomers[iter]->getStartTeller() ) > mtt){
			mtt = ( p_allCustomers[iter]->getEndTeller() - p_allCustomers[iter]->getStartTeller() );
		}
	}

	actq = (int) temp1/tcs;
	actt = (int) temp2/tcs;

	for(iter=0; iter < NUM_TELLERS;iter++){
		temp1+= p_tellers[iter]->p_ssw->secsWaiting;
		if( p_tellers[iter]->p_ssw->secsWaiting > mtwc ){
			mtwc = p_tellers[iter]->p_ssw->secsWaiting;
		}
	}

	atwc = (int)temp1/NUM_TELLERS;

	mcqd = p_cQueue->getMaxDepth();

	printf("\n////////////////////// PRINTING SYSTEM STATISTICS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
	printf("//\tTOTAL CUSTOMERS SERVICED	\t: %d\t\t\t\\\\\n", tcs );
	printf("//\tAVG CUSTOMER TIME IN QUEUE\t\t: %.2d:%.2d(MINUTES)\t\\\\\n", (actq / 60) % 60 , actq % 60);
	printf("//\tAVG CUSTOMER TIME AT TELLER\t\t: %.2d:%.2d(MINUTES)\t\\\\\n", (actt / 60) % 60 , actt % 60);
	printf("//\tAVG TELLER WAIT TIME FOR CUSTOMERS\t: %.2d:%.2d:%.2d(HOURS)\t\\\\\n", atwc / 3600, (atwc / 60) % 60 , atwc % 60);
	printf("//\tMAX CUSTOMER TIME IN QUEUE\t\t: %.2d:%.2d:%.2d(HOURS)\t\\\\\n", mctq / 3600, (mctq / 60) % 60 , mctq % 60);
	printf("//\tMAX TELLER WAIT TIME FOR CUSTOMERS\t: %.2d:%.2d:%.2d(HOURS)\t\\\\\n", mtwc / 3600, (mtwc / 60) % 60 , mtwc % 60);
	printf("//\tMAX TRANSACTION TIME\t\t\t: %.2d:%.2d(MINUTES)\t\\\\\n", (mtt / 60) % 60 , mtt % 60);
	printf("//\tMAX CUSTOMER QUEUE DEPTH\t\t: %d\t\t\t\\\\\n", mcqd);
	printf("/////////////////////////////////////^\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");


	return EXIT_SUCCESS;
}
