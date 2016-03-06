/*
 * grid_set_cut.cpp
 *
 *  Created on: 29/nov/2011
 *      Author: ubuntu
 */

#include "grid.hpp"


void grid::set_cut()
{
	label L;
	unsigned int N_edges = ccb.get_N_edges();
	for (unsigned int i=1; i<=N_edges; i++)
	{
		L=ccb.get_label(i);
		cells[L].set_boundary(CELL::cut);
	}
}
