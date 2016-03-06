/*
 * grid_boundary_conditions.cpp
 *
 *  Created on: 10/gen/2012
 *      Author: ubuntu
 */

#include "grid.hpp"

void grid::boundary_conditions()
{
	calculate_RP_quantities();
	calculate_GP_quantities();
}
