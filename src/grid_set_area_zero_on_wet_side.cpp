/*
 * grid_set_area_zero_on_wet_side.cpp
 *
 *  Created on: 29/nov/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

	void grid::set_area_zero_on_wet_side()
	{
		int N_edges=ccb.get_N_edges();
		label L, L_adj;

		Point2d<double> v1,v2;
		for (int i=1; i<=N_edges; i++)
		{
			L=ccb.get_label(i);
			v1=ccb.get_edge_vertex(BOUNDARY::right,i);
			v2=ccb.get_edge_vertex(BOUNDARY::left,i);

			// controllo area zero a nord
			if (fabs(v1(1)- y.at(L(1)+1))<toll_y && fabs(v2(1)- y.at(L(1)+1))<toll_y)
			{
				L_adj(0)=L(0);
				L_adj(1)=L(1)+1;
				if (cells[L_adj].get_type()==CELL::wet)
					ccb.set_label(L_adj,i);
			}
			// controllo area zero a est
			else if (fabs(v1(0)- x.at(L(0)+1))<toll_x && fabs(v2(0)- x.at(L(0)+1))<toll_x)
			{
				L_adj(0)=L(0)+1;
				L_adj(1)=L(1);
				if (cells[L_adj].get_type()==CELL::wet)
					ccb.set_label(L_adj,i);
			}
			// controllo area zero a sud
			else if (fabs(v1(1)- y.at(L(1)))<toll_y && fabs(v2(1)- y.at(L(1)))<toll_y)
			{
				L_adj(0)=L(0);
				L_adj(1)=L(1)-1;
				if (cells[L_adj].get_type()==CELL::wet)
					ccb.set_label(L_adj,i);
			}
			// controllo area zero a ovest
			else if (fabs(v1(0)- x.at(L(0)))<toll_x && fabs(v2(0)- x.at(L(0)))<toll_x)
			{
				L_adj(0)=L(0)-1;
				L_adj(1)=L(1);
				if (cells[L_adj].get_type()==CELL::wet)
					ccb.set_label(L_adj,i);
			}
		}
	}
