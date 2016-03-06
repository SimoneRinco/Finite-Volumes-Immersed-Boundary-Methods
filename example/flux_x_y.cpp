/*
 * flux_x.cpp
 *
 *  Created on: 03/feb/2012
 *      Author: ubuntu
 */

#include "finite_volume.hpp"

void FINITE_VOLUME::flux_x(const triplet& u, double3& f)
{
	double eps = 10e-5;
	const double g = 9.81;

	if (u.z < eps)
	{
		std::cout<<"h circa 0 in flux_x\n";
	}

	f.first = u.Qx;
	f.second = (u.Qx)*(u.Qx)/u.z + 0.5*g*u.z*u.z;
	f.third = u.Qx*u.Qy/u.z;
}

void FINITE_VOLUME::flux_y(const triplet& u, double3& f)
{
	double eps = 10e-5;
	const double g = 9.81;

	if (u.z < eps)
	{
		std::cout<<"h circa 0 in flux_y\n";
	}

	f.first = u.Qy;
	f.second = u.Qx*u.Qy/u.z;
	f.third = (u.Qy)*(u.Qy)/u.z + 0.5*g*u.z*u.z;
}
