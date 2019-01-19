/*
 * ghost_unknown.cpp
 *
 *  Created on: 18/gen/2012
 *      Author: ubuntu
 */

#include "ghost_unknown.hpp"

ghost_unknown::ghost_unknown(grid& g, const unsigned int& n_unknown, const p_comp& comp_function) : unknown_map(comp_function)
{

	std::vector<label> gcl = g.get_ghost_cells();
	std::vector<ghost_value> gv;

	for (std::vector<label>::iterator it = gcl.begin(); it!= gcl.end(); it++)
	{
		unknown_map.insert(std::pair<label, std::vector<ghost_value> >(*it, gv));
	}
	import_unknown(g, n_unknown);
}

ghost_unknown::ghost_unknown(const ghost_unknown& lhs): unknown_map(lhs.unknown_map) {}

void ghost_unknown::import_unknown(grid& g, const unsigned int& n_unknown)
{
	std::vector<ghost_quantities> gq;
	ghost_value gv;

	for (std::map<label, std::vector<ghost_value> >::iterator it = unknown_map.begin(); it != unknown_map.end(); it++)
	{
		it->second.clear();
		gq = g.ghost_cells[it->first].get_GQs();

		for (std::vector<ghost_quantities>::iterator itv = gq.begin(); itv != gq.end(); itv++)
		{
			gv.value = itv->RP_and_GP_unknowns.at(n_unknown-1)(1);
			gv.ass_wet_cell = itv->associated_WC;
			it->second.push_back(gv);
		}
	}
}
