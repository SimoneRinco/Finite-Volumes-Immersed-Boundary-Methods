/*
 * unknown.hpp
 *
 *  Created on: 18/gen/2012
 *      Author: ubuntu
 */

#ifndef UNKNOWN_HPP_
#define UNKNOWN_HPP_

#include "grid.hpp"
#include "wet_unknown.hpp"
#include "ghost_unknown.hpp"

namespace unknown_position
{
enum position{E,W,N,S};
}

class unknown
{
public:

	typedef Point2d<unsigned int> label;
	typedef bool (*p_comp)(const label&, const label&);

	unknown(grid&, const unsigned int& n_unknown, const p_comp& comp_function = IMMERSED_BOUNDARY::compare);
	unknown(const unknown&);

	double& operator()(const unsigned int& wet_i, const unsigned int& wet_j);
	double& operator()(const label& l);
	const double& operator()(const label&) const;
	double& operator()(const unsigned int& ghost_i, const unsigned int& ghost_j, const unsigned int& adj_wet_i, const unsigned int& adj_wet_j);
	double& operator()(const label& ghost_cell, const label& adj_wet_cell);

	double& get_stencil_value(const label& computational_wet_cell, const unknown_position::position& pos, const bool& wet_control = true);
	void get_stencil_values(const label& computational_wet_cell, double& u_E, double& u_W, double& u_N, double& u_S);
	IMMERSED_BOUNDARY::stencil_values get_stencil_values(const label& computational_wet_cell);
	void export_unknown();
	void import_unknown();

private:
	wet_unknown Wu;
	ghost_unknown Gu;
	const unsigned int N_unknown;
	grid& G;
};

#endif /* UNKNOWN_HPP_ */
