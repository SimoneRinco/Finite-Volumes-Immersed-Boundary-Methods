/*
 * grid_last_common_ctrs_part.cpp
 *
 *  Created on: 25/dic/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

void grid::last_common_ctrs_part()

{
	bool all_valid = false;

	while (all_valid==false)
	{
	set_all_dry();
	set_all_not_cut();
	all_valid = true;
	remove_invalid_edges(all_valid);
	move_vertices();
	set_cut();
	set_wet2();
	set_area_zero_on_wet_side();
	}

	set_all_not_cut();
	set_cut();

	set_wet_cutted_centers();
	set_wet_cells();
	build_ghost_cells();
	build_interpolation_matrices();
	type_of_boundary_conditions.assign(ccb.get_N_edges(), 0);
}
