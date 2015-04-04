/*
 * Stopwatch.h
 *
 *  Created on: Apr 4, 2015
 *      Author: ajl2612
 */

#ifndef STOPWATCH_H_
#define STOPWATCH_H_

class Teller;

#include "Actor.h"
#include "Teller.h"

class Stopwatch : public Actor {
public:
	int secsWaiting;
	Teller* p_teller;

	Stopwatch();
	virtual ~Stopwatch();
	static void *runProcess(void*);
	int getSecsWaiting();
};

#endif /* STOPWATCH_H_ */
