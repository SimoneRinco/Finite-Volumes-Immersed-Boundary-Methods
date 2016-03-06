/*
 * grid_public_setters.cpp
 *
 *  Created on: 09/gen/2012
 *      Author: ubuntu
 */

#include "grid.hpp"

void grid::set_BC_type(const unsigned int& BC_type)
{
	for (std::vector<unsigned int>::iterator it= type_of_boundary_conditions.begin(); it!=type_of_boundary_conditions.end(); it++)
	{
		*it = BC_type;
	}
}

void grid::set_BC_type(const unsigned int& first_edge_number, const unsigned int& last_edge_number, const unsigned int& BC_type)
{
	// assegna BC_type ai lati [first_edge_number, last_edge_number)

	for (unsigned int i = first_edge_number; i<last_edge_number; i++)
	{
		type_of_boundary_conditions.at(i-1) = BC_type;
	}
}

void grid::set_unknown_values(IMMERSED_BOUNDARY::p_fun pf, const unsigned int& unknown_number)
{
	Point2d<double> cc; // cell_center
	label current_label;
	double unknown_value;

	if (unknown_number<1 || unknown_number>N_equations)
	{
		std::cout<<"ERRORE: numero dell'incognita fornito errato: nessun valore è stato assegnato.\n";
	}
	else
	{
		for (unsigned int i=0; i<Nx+2; i++)
		{
			for (unsigned int j=0; j<Ny+2; j++)
			{
				current_label(0)=i;
				current_label(1)=j;
				if (cells[current_label].get_type() == CELL::wet)
				{
					cc = cells[current_label].get_center();
					unknown_value = pf(cc);
					cells[current_label].set_unknown(unknown_number, unknown_value);
				}
			}
		}
	}
}

void grid::set_unknown_values(const label& cell_label, const unsigned int& unknown_number, const double& unknown_value)
{
	if (unknown_number<1 || unknown_number>N_equations)
	{
		std::cout<<"ERRORE: numero dell'incognita fornito errato: nessun valore è stato assegnato.\n";
	}
	else if (cells[cell_label].get_type() != CELL::wet)
	{
		std::cout<<"WARNING: si sta cercando di impostare il valore " <<unknown_value <<"all'incognita " <<unknown_number <<" alla cella ";
		cell_label.print();
		std::cout<<"che non è bagnata. Nessuna operazione eseguita.\n";
	}
	else
	{
		cells[cell_label].set_unknown(unknown_number, unknown_value);
	}
}

