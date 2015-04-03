/*
 * Actor.h
 *
 *  Created on: Mar 31, 2015
 *      Author: ajl2612
 */

#ifndef ACTOR_H_
#define ACTOR_H_

#include <queue>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>		// for using time object
#include <unistd.h>		//for using usleep

#include "Messages.h"

using namespace std;

class Actor {
public:
	time_t epoch;
	queue<int> msgQueue;
	int currentState;

	Actor(time_t*);
	virtual ~Actor();

	static void *runProcess(void*);
	void sendMessage(int);
};

#endif /* ACTOR_H_ */
