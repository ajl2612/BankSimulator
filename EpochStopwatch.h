/*
 * EpochStopwatch.h
 *
 *  Created on: Apr 4, 2015
 *      Author: ajl2612
 */

#ifndef EPOCHSTOPWATCH_H_
#define EPOCHSTOPWATCH_H_

#include "Actor.h"

class EpochStopwatch : public Actor {
public:
	int secsSinceEpoch;

	EpochStopwatch();
	virtual ~EpochStopwatch();
	static void *runProcess(void*);
	int getSecsSinceEpoch();
};

#endif /* EPOCHSTOPWATCH_H_ */
