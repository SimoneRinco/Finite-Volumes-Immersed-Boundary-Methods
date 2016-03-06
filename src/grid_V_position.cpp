/*
 * grid_V_position.cpp
 *
 *  Created on: 23/dic/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

IMMERSED_BOUNDARY::position grid::V_position(const label& cell_label, Point2d<double>& V)
{

	// Riposizionamento del vertice V all'interno della sua cella nel caso in cui sia vicino ai bordi cella.
	// Viene restituita la posizione di V relativamente alla propria cella dopo l'eventuale riposizionamento.
	// NB: non viene modificata la cella di appartenenza.

	using namespace IMMERSED_BOUNDARY;

	double& xv = V(0);
	double& yv = V(1);
	const double& xW = x.at(cell_label(0));
	const double& xE = x.at(cell_label(0)+1);
	const double& yS = y.at(cell_label(1));
	const double& yN = y.at(cell_label(1)+1);

	if (fabs(xv-xW)<toll_x)
	{
		xv = xW;
		if (fabs(yv-yS)<toll_y)
			{yv = yS; return SW;}
		else if (fabs(yv-yN)<toll_y)
			{yv = yN; return NW;}
		else return W;
	}
	//
	else if (fabs(xv-xE)<toll_x)
	{
		xv = xE;
		if (fabs(yv-yS)<toll_y)
			{yv = yS; return SE;}
		else if (fabs(yv-yN)<toll_y)
			{yv = yN; return NE;}
		else return E;
	}
	//
	else
	{
		if (fabs(yv-yS)<toll_y)
			{yv = yS; return S;}
		else if (fabs(yv-yN)<toll_y)
			{yv = yN; return N;}
		else return Intern;
	}
}
