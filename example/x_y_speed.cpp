/*
 * x_speed.cpp
 *
 *  Created on: 03/feb/2012
 *      Author: ubuntu
 */

#include "finite_volume.hpp"

void FINITE_VOLUME::x_speed(const triplet& plus, const triplet& minus, double& a)
{

	// formula non desingolarizzata
	const double eps = 10e-5;
	double u_p, u_m;
	const double g = 9.81;

	if (plus.z < eps || minus.z < eps)
	{
		std::cout<< "WARNING: Altezza minore di " <<eps <<" in x_speed." <<std::endl;
	}
	u_p = plus.Qx/plus.z;
	u_m = minus.Qx/minus.z;

	a = std::max(u_p + sqrt(g*plus.z), u_m + sqrt(g*minus.z));
}

void FINITE_VOLUME::y_speed(const triplet& plus, const triplet& minus, double& a)
{

	// formula non desingolarizzata
	const double eps = 10e-5;
	double v_p, v_m;
	const double g = 9.81;

	if (plus.z < eps || minus.z < eps)
	{
		std::cout<< "WARNING: Altezza minore di " <<eps <<" in y_speed." <<std::endl;
	}
	v_p = plus.Qy/plus.z;
	v_m = minus.Qy/minus.z;

	a = std::max(v_p + sqrt(g*plus.z), v_m + sqrt(g*minus.z));

}

