/*
 * grid_valid_edge.cpp
 *
 *  Created on: 21/nov/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

bool grid::valid_edge(const label& l, const Point2d<double>& lv, const Point2d<double>& rv, const double& toll_x, const double& toll_y, BOUNDARY::side& v_extreme)
{
	// un lato è considerato non valido se i due estremi coincidono oppure se il lato appartiene strettamente ad uno dei bordi cella.
	// Un lato coincidente con un bordo cella è considerato valido.

	const double& xlv=lv(0);
	const double& ylv=lv(1);
	const double& xrv=rv(0);
	const double& yrv=rv(1);
	const unsigned int& lx=l(0);
	const unsigned int& ly=l(1);

	if (fabs(xlv-xrv)<toll_x && fabs(ylv-yrv)<toll_y) return false; // vertici vicini

	else if (fabs(xlv-x.at(lx))<toll_x && (fabs(ylv-y.at(ly))<toll_y || fabs(ylv-y.at(ly+1))<toll_y) ||
		fabs(xlv-x.at(lx+1))<toll_x && (fabs(ylv-y.at(ly))<toll_y || fabs(ylv-y.at(ly+1))<toll_y))
	{
		v_extreme=BOUNDARY::left;
		if ((fabs(xlv-xrv)<toll_x && fabs(ylv-yrv)<dy-toll_y) || (fabs(ylv-yrv)<toll_y && fabs(xlv-xrv)<dx-toll_x))
			return false;
		else return true;
	}

	else if (fabs(xrv-x.at(lx))<toll_x && (fabs(yrv-y.at(ly))<toll_y || fabs(yrv-y.at(ly+1))<toll_y) ||
		fabs(xrv-x.at(lx+1))<toll_x && (fabs(yrv-y.at(ly))<toll_y || fabs(yrv-y.at(ly+1))<toll_y))
	{
		v_extreme=BOUNDARY::right;
		if ((fabs(xlv-xrv)<toll_x && fabs(ylv-yrv)<dy-toll_y) || (fabs(ylv-yrv)<toll_y && fabs(xlv-xrv)<dx-toll_x))
			return false;
		else return true;
	}

	else if ((fabs(xlv-x.at(lx))<toll_x && fabs(xrv-x.at(lx))<toll_x && fabs(ylv-yrv)<dy-toll_y) ||
			(fabs(xlv-x.at(lx+1))<toll_x && fabs(xrv-x.at(lx+1))<toll_x && fabs(ylv-yrv)<dy-toll_y) ||
			(fabs(ylv-y.at(ly))<toll_y && fabs(yrv-y.at(ly))<toll_y && fabs(xlv-xrv)<dx-toll_x) ||
			(fabs(ylv-y.at(ly+1))<toll_y && fabs(yrv-y.at(ly+1))<toll_y && fabs(xlv-xrv)<dx-toll_x))
	{
		v_extreme=BOUNDARY::left;
		return false;
	}

	else return true;

}
