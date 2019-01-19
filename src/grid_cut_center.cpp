/*
 * grid_cutted_center.cpp
 *
 *  Created on: 21/dic/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

bool grid::cutted_center(const label& L)
{
	Point2d<double> V1, V2, edge_center, cell_center;
	int n_edge;

	n_edge = ccb.get_n_edge(L);

	if (n_edge == -1)
	{
		return false;
	}
	else
	{
		V1 = ccb.get_edge_vertex(BOUNDARY::right, n_edge);
		V2 = ccb.get_edge_vertex(BOUNDARY::left, n_edge);
		cell_center = cells[L].get_center();
		edge_center(0) = 0.5*(V1(0)+V2(0));
		edge_center(1) = 0.5*(V1(1)+V2(1));

		return (fabs(edge_center(0)-cell_center(0))<0.1*toll_x && fabs(edge_center(1)-cell_center(1))<0.1*toll_y);
	}
}

bool grid::cutted_center_by_diagonal(const label& L)
{
	unsigned int n_edge;
	Point2d<double> V1, V2, temp;
	bool d1, d2;

	if (cutted_center(L))
	{
		n_edge = ccb.get_n_edge(L);
		V1 = ccb.get_edge_vertex(BOUNDARY::right, n_edge);
		V2 = ccb.get_edge_vertex(BOUNDARY::left, n_edge);

		if (V2(0)<V1(0))
		{
			temp=V1;
			V1=V2;
			V2=temp;
		}

		d1 = fabs(V1(0)-x.at(L(0)))<toll_x && fabs(V1(1)-y.at(L(1)))<toll_y && fabs(V2(0)-x.at(L(0)+1))<toll_y && fabs(V2(1)-y.at(L(1)+1))<toll_y;
		d2 = fabs(V1(0)-x.at(L(0)))<toll_x && fabs(V1(1)-y.at(L(1)+1))<toll_y && fabs(V2(0)-x.at(L(0)+1))<toll_y && fabs(V2(1)-y.at(L(1)))<toll_y;
		return (d1 || d2);
	}

	else return false;
}
