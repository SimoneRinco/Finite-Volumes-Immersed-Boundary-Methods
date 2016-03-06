/*
 * grid_public_getters.cpp
 *
 *  Created on: 09/gen/2012
 *      Author: ubuntu
 */

#include "grid.hpp"

unsigned int grid::get_Nx() const
{
	return Nx;
}
unsigned int grid::get_Ny() const
{
	return Ny;
}
unsigned int grid::get_N_equations() const
{
	return N_equations;
}
unsigned int grid::get_N_wet_cells() const
{
	return wet_cells.size();
}
unsigned int grid::get_N_ghost_cells() const
{
	return ghost_cells.size();
}

bool grid::is_wet(const unsigned int& i, const unsigned int& j) const
{
	label L(i,j);
	if (i>=Nx+2 || j>=Ny+2)
	{
		std::cout<<"ERRORE: non esiste la cella ";
		L.print();
		return false;
	}
	else
	{
		std::map<label, cell, p_comp>::const_iterator it = cells.find(L);
		return (it->second.get_type() == CELL::wet);
	}
}
std::vector<double> grid::get_x() const
{
	return x;
}
std::vector<double> grid::get_y() const
{
	return y;
}
double grid::get_dx() const
{
	return dx;
}
double grid::get_dy() const
{
	return dy;
}
const std::vector<IMMERSED_BOUNDARY::label>& grid::get_wet_cells() const
{
	return wet_cells;
}
std::vector<IMMERSED_BOUNDARY::label> grid::get_ghost_cells() const
{
	std::vector<label> gcl;
	for (std::map<label, ghost_cell>::const_iterator it = ghost_cells.begin(); it != ghost_cells.end(); it++)
	{
		gcl.push_back(it->first);
	}
	return gcl;
}

Point2d<double> grid::get_cell_center(const unsigned int& i, const unsigned int& j) const
{
	label L(i,j);
	if (i>=Nx || j>=Ny)
	{
		std::cout<<"ERRORE: non esiste la cella ";
		L.print();
		return Point2d<double>(0.0,0.0);
	}
	else
	{
		std::map<label, cell, p_comp>::const_iterator it = cells.find(L);
		return (it->second).get_center();
	}
}
Point2d<double> grid::get_cell_center(const IMMERSED_BOUNDARY::label& l) const
{
	return get_cell_center(l(0), l(1));
}

unsigned int grid::get_N_edges()
{
	return ccb.get_N_edges();
}

bool grid::get_stencil_values(const label& computation_cell, const unsigned int& unknown_number, double& E_value, double& W_value, double& N_value, double& S_value)
{
	unsigned int lx = computation_cell(0);
	unsigned int ly = computation_cell(1);

	label E_label(lx+1, ly);
	label W_label(lx-1, ly);
	label N_label(lx, ly+1);
	label S_label(lx, ly-1);

	int associated_GP_number;

	if (cells[computation_cell].get_type() == CELL::dry)
	{
		return false;
	}
	else
	{

		E_value = compute_single_stencil_value(computation_cell, E_label, unknown_number);
		W_value = compute_single_stencil_value(computation_cell, W_label, unknown_number);
		N_value = compute_single_stencil_value(computation_cell, N_label, unknown_number);
		S_value = compute_single_stencil_value(computation_cell, S_label, unknown_number);
		return true;
	}
}

bool grid::get_stencil_values(const label& computation_cell, const unsigned int& un, IMMERSED_BOUNDARY::stencil_values& u)
{
	double u_c = get_unknown_value(computation_cell, un);
	double u_E, u_W, u_N, u_S;
	get_stencil_values(computation_cell, un, u_E, u_W, u_N, u_S);
	u.central = u_c;
	u.E = u_E;
	u.W = u_W;
	u.N = u_N;
	u.S = u_S;
}

double grid::get_unknown_value(const label& wet_label, const unsigned int& n_unknown)
{
	if (n_unknown<1 || n_unknown>N_equations)
	{
		std::cout<<"ERRORE: non esiste l'incognita numero " <<n_unknown <<std::endl;
		return 0.0;
	}
	else if (cells[wet_label].get_type() != CELL::wet)
	{
		std::cout<<"ERRORE: la cella ";
		wet_label.print();
		std::cout<<"non è bagnata.\n";
		return 0.0;
	}
	else
	{
		return cells[wet_label].get_unknown(n_unknown);
	}
}

double grid::get_unknown_value(const unsigned int& wet_i, const unsigned int& wet_j, const unsigned int& n_unknown)
{
	return get_unknown_value(label(wet_i, wet_j), n_unknown);
}


// this is a private member function
double grid::compute_single_stencil_value(const label& current_label, const label& adj_label, const unsigned int& unknown_number)
{

	double value;
	int associated_GP_number;

	if (cells[adj_label].get_type() == CELL::wet)
	{
		value = cells[adj_label].get_unknown(unknown_number);
	}
	else
	{
		if (ghost_cells.find(adj_label) == ghost_cells.end())
		{
			std::cout<< "ERRORE: ";
			adj_label.print();
			std::cout<< "isn't a ghost cell.\n";
		}

		associated_GP_number = ghost_cells[adj_label].get_associated_GP(current_label);

		// visualizzazioni per debug
		/*
		std::cout<< "ghost cell: ";
		adj_label.print();
		std::cout<<"computational cell: ";
		current_label.print();
		std::cout<<"associated GP number: " <<associated_GP_number <<std::endl;
		/**/

		if (associated_GP_number == -1)
		{
			std::cout<<"ERROR: ghost cell ";
			adj_label.print();
			std::cout<<"hasn't associated RP to cell ";
			current_label.print();
			std::cout<< std::endl;
		}
		else
		{
			value = ghost_cells[adj_label].get_GP_unknown(associated_GP_number, unknown_number);
		}
	}

	return value;
}
