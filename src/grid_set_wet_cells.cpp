/*
 * grid_set_wet_cells.cpp
 *
 *  Created on: 17/gen/2012
 *      Author: ubuntu
 */

#include "grid.hpp"

void grid::set_wet_cells()
{
	for (std::map<label, cell>::iterator it = cells.begin(); it != cells.end(); it++)
	{
		if (it->second.get_type() == CELL::wet)
		{
			wet_cells.push_back(it->first);
		}
	}
}
