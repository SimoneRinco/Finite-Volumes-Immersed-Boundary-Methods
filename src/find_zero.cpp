/*
 * find_zero.cpp
 *
 *  Created on: 30/nov/2011
 *      Author: ubuntu
 */

#include "immersed_boundary.hpp"


double IMMERSED_BOUNDARY::find_zero(const Point2d<double>& c1, const Point2d<double>& c2, const p_fun& p_f, const bool& move_vertical, const double& toll)
{
	Point2d<double> P;
	double t, t_plus, t_minus, delta;
	const double& x1=c1(0);
	const double& x2=c2(0);
	const double& y1=c1(1);
	const double& y2=c2(1);

	double f_value;

	if (p_f(c1)*p_f(c2)>0) throw ERRORS::errors(10);

	if (move_vertical)
	{
		if (fabs(x2-x1)>toll) throw ERRORS::errors(10);

		P(0)=x1;

		if(p_f(c1)>0)
		{
			t_plus=y1;
			t_minus=y2;
		}
		else
		{
			t_plus=y2;
			t_minus=y1;
		}

		delta = fabs(t_minus-t_plus);


		do
		{

		t= 0.5*(t_plus+t_minus);
		P(1)=t;
		f_value=p_f(P);

		if (f_value>0)
			t_plus=t;
		else
			t_minus=t;

		delta=0.5*delta;

		}
		while (delta>toll);

	}


	else
	{
		if (fabs(y2-y1)>toll) throw ERRORS::errors(10);

		P(1)=y1;

		if(p_f(c1)>0)
		{
			t_plus=x1;
			t_minus=x2;
		}
		else
		{
			t_plus=x2;
			t_minus=x1;
		}

		delta = fabs(t_minus-t_plus);


		do
		{

		t= 0.5*(t_plus+t_minus);
		P(0)=t;
		f_value=p_f(P);

		if (f_value>0)
			t_plus=t;
		else
			t_minus=t;

		delta=0.5*delta;

		}
		while (delta>toll);
	}


	return t;

}
