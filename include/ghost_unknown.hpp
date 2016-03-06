/*
 * ghost_unknown.hpp
 *
 *  Created on: 18/gen/2012
 *      Author: ubuntu
 */

#ifndef GHOST_UNKNOWN_HPP_
#define GHOST_UNKNOWN_HPP_

#include "grid.hpp"


struct ghost_value
{
	double value;
	Point2d<unsigned int> ass_wet_cell; // associated wet cell
};

class ghost_unknown
{
public:
	typedef Point2d<unsigned int> label;
	typedef bool (*p_comp)(const label&, const label&);

	ghost_unknown(grid&, const unsigned int& n_unknown, const p_comp& comp_function = IMMERSED_BOUNDARY::compare);
	ghost_unknown(const ghost_unknown&);
	void import_unknown(grid&, const unsigned int& n_unknown);

	friend class unknown;

private:
	std::map<label, std::vector<ghost_value>,  p_comp> unknown_map;
};

#endif /* GHOST_UNKNOWN_HPP_ */
