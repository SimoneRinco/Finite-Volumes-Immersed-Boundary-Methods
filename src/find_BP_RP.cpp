/*
 * find_BP_RP.cpp
 *
 *  Created on: 13/dic/2011
 *      Author: ubuntu
 */

#include "immersed_boundary.hpp"


void IMMERSED_BOUNDARY::find_BP_RP_normal(const Point2d<double>& V1, const Point2d<double>& V2, const Point2d<double>& cc, Point2d<double>& BP, Point2d<double>& RP, Point2d<double>& n)

{
	// Dati i vertici del lato e il centro cella (della ghost cell) restituisce BP, RP e la normale uscente

	double x1, y1, x2, y2, xc, yc, xt, yt, xn, yn, xb, yb, xr, yr;
	double euc_norm;
	double alpha, beta;

	x1 = V1(0);
	y1 = V1(1);
	x2 = V2(0);
	y2 = V2(1);

	xc = cc(0);
	yc = cc(1);

	xt = x1-x2;
	yt = y1-y2;

	euc_norm = std::sqrt(xt*xt+yt*yt);
	xt = xt/euc_norm;
	yt = yt/euc_norm;

	xn = yt;
	yn = -xt;

	alpha = xt*(xc-x1) + yt*(yc-y1);
	beta = -yt*(xc-x1) + xt*(yc-y1);

	xb = x1 + alpha*xt;
	yb = y1 + alpha*yt;

	// impostazione della normale uscente
	if (beta>0.0)
	{
		xn=-xn;
		yn=-yn;
		xr = xc - 2*beta*xn;
		yr = yc - 2*beta*yn;
	}

	else
	{
	xr = xc + 2*beta*xn;
	yr = yc + 2*beta*yn;
	}

	BP(0)=xb;
	BP(1)=yb;

	RP(0)=xr;
	RP(1)=yr;

	n(0)=xn;
	n(1)=yn;

}
