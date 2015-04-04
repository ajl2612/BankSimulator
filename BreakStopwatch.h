/*
 * BreakStopwatch.h
 *
 *  Created on: Apr 3, 2015
 *      Author: ajl2612
 */

#ifndef BREAKSTOPWATCH_H_
#define BREAKSTOPWATCH_H_


class Teller;

#include "Actor.h"
#include "Teller.h"

class BreakStopwatch : public Actor  {
public:
	Teller* p_teller;

	BreakStopwatch();
	virtual ~BreakStopwatch();
	static void *runProcess(void*);
};

#endif /* BREAKSTOPWATCH_H_ */
