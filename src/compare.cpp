/*
 * compare.cpp
 *
 *  Created on: 14/nov/2011
 *      Author: ubuntu
 */

#include "immersed_boundary.hpp"

bool IMMERSED_BOUNDARY::compare(const label& l, const label& r)
{
	if (l(0)==r(0)) return l(1)<r(1);
	else return l(0)<r(0);
}
