/*
 * wet_unknown.hpp
 *
 *  Created on: 18/gen/2012
 *      Author: ubuntu
 */

#ifndef WET_UNKNOWN_HPP_
#define WET_UNKNOWN_HPP_

#include "grid.hpp"


class wet_unknown
{
public:
	typedef Point2d<unsigned int> label;
	typedef bool (*p_comp)(const label&, const label&);

	wet_unknown(grid&, const unsigned int& n_unknown, const p_comp& comp_function = IMMERSED_BOUNDARY::compare);
	wet_unknown(const wet_unknown&);
	double& operator()(const unsigned int& i, const unsigned int& j);
	double& operator()(const label& l);
	void export_unknown(grid&, const unsigned int& n_unknown);
	void import_unknown(grid&, const unsigned int& n_unknown);

	friend class unknown;

private:
	std::map<label, double,  p_comp> unknown_map;

};


#endif /* WET_UNKNOWN_HPP_ */
