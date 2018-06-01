/*
 * Box.h
 *
 *  Created on: 1 Jun 2018
 *      Author: martin
 */

#ifndef SEARCHER_INCLUDE_BOX_H_
#define SEARCHER_INCLUDE_BOX_H_

#include "Base.h"
#include "Entity.h"

class Base;

class Box : public Entity {
public:

	Box(Base & base, Location loc);
	virtual ~Box();

};

#endif /* SEARCHER_INCLUDE_BOX_H_ */
