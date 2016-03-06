/*
 * grid_build_edge.cpp
 *
 *  Created on: 30/nov/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

IMMERSED_BOUNDARY::Edge grid::build_edge(const Point2d<double>& zero, const label& cell_label, const IMMERSED_BOUNDARY::p_fun& p_f, const IMMERSED_BOUNDARY::p_grad_fun& p_grad_f)
{
	const double x_min = x.at(cell_label(0));
	const double x_max = x.at(cell_label(0)+1);
	const double y_min = y.at(cell_label(1));
	const double y_max = y.at(cell_label(1)+1);

	const double x0 = zero(0);
	const double y0 = zero(1);

	//std::cout<<"x_min, x_max, y_min, t_max, x0, y0\n";
	//std::cout <<x_min <<" " <<x_max <<" " <<y_min <<" " <<y_max <<" " <<x0 <<" " <<y0 <<"\n"; //ok
	double m,x,y;

	Point2d<double> grad = p_grad_f(zero);
	Point2d<double> tangent(grad(1),-grad(0));
	//tangent.print(); //ok
	Point2d<double> V1,V2;
	IMMERSED_BOUNDARY::Edge edge;

	if (fabs(tangent(1))<fabs(tangent(0)))
	{
		// considero retta nella forma y = m*x+q

		m= tangent(1)/tangent(0);

		// provo x_min

		y = y0 + m*(x_min-x0); // se m~=0 y~=y0 e non si deve calcolare 1/m
		if (y>y_max)
		{
			x= x0 + (1.0/m)*(y_max-y0);
			V1(0)= x;
			V1(1)= y_max;
		}
		else if (y<y_min)
		{
			x= x0 + (1.0/m)*(y_min-y0);
			V1(0)= x;
			V1(1)= y_min;
		}
		else
		{
			V1(0)= x_min;
			V1(1)= y;
		}

		// provo x_max

		y = y0 + m*(x_max-x0);
		if (y>y_max)
		{
			x= x0 + (1.0/m)*(y_max-y0);
			V2(0)= x;
			V2(1)= y_max;
		}
		else if (y<y_min)
		{
			x= x0 + (1.0/m)*(y_min-y0);
			V2(0)= x;
			V2(1)= y_min;
		}
		else
		{
			V2(0)= x_max;
			V2(1)= y;
		}


	}

	else
	{
		// considero retta nella forma x = m*y+q

		m= tangent(0)/tangent(1);
		// provo y_min


		x = x0 + m*(y_min-y0);
		if (x>x_max)
		{
			y= y0 + (1.0/m)*(x_max-x0);
			V1(0)= x_max;
			V1(1)= y;
		}
		else if (x<x_min)
		{
			y= y0 + (1.0/m)*(x_min-x0);
			V1(0)= x_min;
			V1(1)= y;
		}
		else
		{
			V1(0)= x;
			V1(1)= y_min;
		}

		// provo y_max

		x = x0 + m*(y_max-y0);
		if (x>x_max)
		{
			y= y0 + (1.0/m)*(x_max-x0);
			V2(0)= x_max;
			V2(1)= y;
		}
		else if (x<x_min)
		{
			y= y0 + (1.0/m)*(x_min-x0);
			V2(0)= x_min;
			V2(1)= y;
		}
		else
		{
			V2(0)= x;
			V2(1)= y_max;
		}


	}

	edge.first=V1;
	edge.second=V2;
	return edge;

}



