/*
 * Boss.h
 *
 *  Created on: Apr 3, 2015
 *      Author: ajl2612
 */

#ifndef BOSS_H_
#define BOSS_H_

#include "Actor.h"
#include "Securityguard.h"

class Boss : public Actor {
public:
	Securityguard* p_guard;

	Boss();
	virtual ~Boss();

	static void *runProcess(void*);
};

#endif /* BOSS_H_ */
