/*
 * grid_calculate_GP_quantities.cpp
 *
 *  Created on: 10/gen/2012
 *      Author: ubuntu
 */

#include "grid.hpp"

void grid::calculate_GP_quantities()
{

	double Z_RP, Z_GP;
	double Qn_RP, Qn_GP;
	double Qt_RP, Qt_GP;
	double Qx_RP, Qx_GP;
	double Qy_RP, Qy_GP;
	double nx, ny;


	double q = 0.0;


	for (std::map<label, ghost_cell>::iterator it = ghost_cells.begin(); it != ghost_cells.end(); it++)
	{
		for (unsigned int RP_counter=0; RP_counter < it->second.get_number_of_adj_wet_cells(); RP_counter++)
		{

			Z_RP = it->second.get_RP_unknown(RP_counter, 1);
			Qx_RP = it->second.get_RP_unknown(RP_counter, 2);
			Qy_RP = it->second.get_RP_unknown(RP_counter, 3);

			nx = it->second.get_GQs().at(RP_counter).normal(0);
			ny = it->second.get_GQs().at(RP_counter).normal(1);

			Qn_RP = Qx_RP*nx + Qy_RP*ny;
			Qt_RP = Qx_RP*ny - Qy_RP*nx;

			////////////////////////////////////////////////////////////////////////////////////////////
			// APPLICAZIONE DELLE CONDIZIONI AL BORDO

			// ** // BOUNDARY CONDITION FOR Z
			Z_GP = Z_RP;
			// ** // ** // ** // ** //

			// ** // BOUNDARY CONDITION FOR Qt
			Qt_GP = Qt_RP;
			// ** // ** // ** // ** //

			// ** // BOUNDARY CONDITION FOR Qn
			Qn_GP = 2.0*q - Qn_RP;
			// ** // ** // ** // ** //
			////////////////////////////////////////////////////////////////////////////////////////////

			Qx_GP = Qn_GP*nx + Qt_GP*ny;
			Qy_GP = Qn_GP*ny - Qt_GP*nx;

			it->second.set_GP_unknown(RP_counter, 1, Z_GP);
			it->second.set_GP_unknown(RP_counter, 2, Qx_GP);
			it->second.set_GP_unknown(RP_counter, 3, Qy_GP);

		} // fine scorrimento RPs
	} // fine scorrimento ghost cells
} // end method

