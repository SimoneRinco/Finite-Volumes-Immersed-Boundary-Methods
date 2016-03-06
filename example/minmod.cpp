/*
 * minmod.cpp
 *
 *  Created on: 30/gen/2012
 *      Author: ubuntu
 */

#include "finite_volume.hpp"

double FINITE_VOLUME::minmod(const double& x1, const double& x2, const double& x3)
{
	// ritorna il minimo modulo tra x1, x2 e x3 se tutti i numeri hanno lo stesso segno
	// altrimenti ritorna 0.0

	double m;

	if (x1>0 && x2>0 && x3>0)
		return std::min(std::min(x1,x2), x3);
	else if (x1<0 && x2<0 && x3<0)
		return std::max(std::max(x1, x2), x3);
	else
		return 0.0;
}

