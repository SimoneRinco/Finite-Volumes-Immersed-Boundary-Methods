/*
 * relative_position.cpp
 *
 *  Created on: 15/nov/2011
 *      Author: ubuntu
 */

#include "immersed_boundary.hpp"

IMMERSED_BOUNDARY::position IMMERSED_BOUNDARY::relative_position(const Point2d<double> &V_n, const Point2d<double> &V, const double& toll_x, const double& toll_y)
{

	// Ritorna la posizione di Vn rispetto a V
	// Se i nodi sono molto vicini ritorna N o S

	if (fabs(V_n(0)-V(0))<toll_x)
	{
		if (V_n(1)-V(1)> 0) return N;
		else return S;
	}

	else if (fabs(V_n(1)-V(1))<toll_y)
	{
		if (V_n(0)-V(0)> 0) return E;
		else return W;
	}

	else if (V_n(0)>V(0))
	{
		if (V_n(1)>V(1)) return NE;
		else return SE;
	}

	else
	{
		if (V_n(1)>V(1)) return NW;
		else return SW;
	}





}
