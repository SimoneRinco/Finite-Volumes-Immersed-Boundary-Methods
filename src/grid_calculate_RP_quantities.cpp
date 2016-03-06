/*
 * grid_calculate_RP_quantities.cpp
 *
 *  Created on: 09/gen/2012
 *      Author: ubuntu
 */

#include "grid.hpp"

void grid::calculate_RP_quantities()
{

	label SW_corner, SE_corner, NE_corner, NW_corner;
	CELL::type SW_type, SE_type, NE_type, NW_type;
	unsigned int SWx, SWy;
	unsigned int vector_component; // da usare nel caso delle matrici 6x6 e 3x3
	Point2d<double> current_RP;
	double RPx, RPy;
	double unknown_RP_value;

	// Valori dipendenti dalle condizioni al bordo

	double dZ_dn = 0.0;
	// valore della derivata normale dell'altezza totale. Interviene nei casi GPs e dry-non-GP.
	double q = 0.0;
	// Valore della portata in uscita/entrata al bordo. Interviene nei casi GPs e dry-non-GP.

	//

	std::vector<ghost_quantities> GQs_vector;

	std::map<label, IMMERSED_BOUNDARY::M4_LU>::iterator it_M4_LU;
	std::map<label, IMMERSED_BOUNDARY::M8M4_LU>::iterator it_M8M4_LU;
	std::map<label, IMMERSED_BOUNDARY::M6M3_LU_non_cc>::iterator it_M6M3_LU_non_cc;

	IMMERSED_BOUNDARY::V3 icv3, rsv3; // interpolation coefficients vector and right side vector
	IMMERSED_BOUNDARY::V4 icv4, rsv4;
	IMMERSED_BOUNDARY::V6 icv6, rsv6;
	IMMERSED_BOUNDARY::V8 icv8, rsv8;

	for (std::map<label,ghost_cell>::iterator it=ghost_cells.begin(); it!=ghost_cells.end(); it++)
	{
		GQs_vector = it->second.get_GQs();

		for (unsigned int RPcounter=0; RPcounter < it->second.get_number_of_adj_wet_cells(); RPcounter++)
		{

			current_RP = GQs_vector.at(RPcounter).RP;
			RPx = current_RP(0);
			RPy = current_RP(1);

			SW_corner = GQs_vector.at(RPcounter).SW_corner;

			SWx = SW_corner(0);
			SWy = SW_corner(1);
			SW_type = cells[SW_corner].get_type();

			SE_corner(0) = SWx+1;
			SE_corner(1) = SWy;
			SE_type = cells[SE_corner].get_type();

			NE_corner(0) = SWx+1;
			NE_corner(1) = SWy+1;
			NE_type = cells[NE_corner].get_type();

			NW_corner(0) = SWx;
			NW_corner(1) = SWy+1;
			NW_type = cells[NW_corner].get_type();

			////////////////////////////////////////////////////////////////////////////////////////////

			// ** // ** // ** // ** // ** //
			// test caso all_wet
			// ** // ** // ** // ** // ** //

			it_M4_LU = all_wet_matrices.find(SW_corner);

			if (it_M4_LU != all_wet_matrices.end())
			{
				// Il caso "all wet" è il più semplice. Per ogni variabile si trovano i coefficienti interpolanti
				// dati i valori noti nei vertici del quadrato di interpolazione.
				// É l'unico caso in cui il valore in RP è indipendente dal tipo di condizioni al bordo.

				for (unsigned int n_unknown=1; n_unknown<=N_equations; n_unknown++)
				{
					rsv4[0] = cells[SW_corner].get_unknown(n_unknown);
					rsv4[1] = cells[SE_corner].get_unknown(n_unknown);
					rsv4[2] = cells[NE_corner].get_unknown(n_unknown);
					rsv4[3] = cells[NW_corner].get_unknown(n_unknown);

					icv4 = it_M4_LU->second.solve(rsv4);
					unknown_RP_value = icv4[0]*RPx*RPy + icv4[1]*RPx + icv4[2]*RPy + icv4[3];
					it->second.set_RP_unknown(RPcounter, n_unknown, unknown_RP_value);
				}
			}

			// ** // ** // ** // ** // ** //
			// test caso vertici di interpolazione ghost points
			// intervengono direttamente le condizioni al bordo
			// ** // ** // ** // ** // ** //

			it_M8M4_LU = GPs_Matrices.find(SW_corner);
			if (it_M8M4_LU != GPs_Matrices.end())
			{
				// Qui si assume N_equations = 3
				// La prima incognita è la grandezza scalare, le altre 2 la grandezza vettoriale (portata o velocità)

				// costruzione del termine noto per l'altezza e calcolo dell'altezza in RP

				(SW_type == CELL::wet) ? (rsv4[0] = cells[SW_corner].get_unknown(1)) : (rsv4[0] = dZ_dn);
				(SE_type == CELL::wet) ? (rsv4[1] = cells[SE_corner].get_unknown(1)) : (rsv4[1] = dZ_dn);
				(NE_type == CELL::wet) ? (rsv4[2] = cells[NE_corner].get_unknown(1)) : (rsv4[2] = dZ_dn);
				(NW_type == CELL::wet) ? (rsv4[3] = cells[NW_corner].get_unknown(1)) : (rsv4[3] = dZ_dn);

				icv4 = it_M8M4_LU->second.second.solve(rsv4);
				unknown_RP_value = icv4[0]*RPx*RPy + icv4[1]*RPx + icv4[2]*RPy + icv4[3];
				it->second.set_RP_unknown(RPcounter, 1, unknown_RP_value);

				// costruzione del termine noto per la portata e calcolo di Qx e Qy in RP

				if (SW_type == CELL::wet)
				{
					rsv8[0] = cells[SW_corner].get_unknown(2);
					rsv8[1] = cells[SW_corner].get_unknown(3);
				}
				else
				{
					rsv8[0] = q;
					rsv8[1] = 0.0;
				}
				//
				if (SE_type == CELL::wet)
				{
					rsv8[2] = cells[SE_corner].get_unknown(2);
					rsv8[3] = cells[SE_corner].get_unknown(3);
				}
				else
				{
					rsv8[2] = q;
					rsv8[3] = 0.0;
				}
				//
				if (NE_type == CELL::wet)
				{
					rsv8[4] = cells[NE_corner].get_unknown(2);
					rsv8[5] = cells[NE_corner].get_unknown(3);
				}
				else
				{
					rsv8[4] = q;
					rsv8[5] = 0.0;
				}
				//
				if (NW_type == CELL::wet)
				{
					rsv8[6] = cells[NW_corner].get_unknown(2);
					rsv8[7] = cells[NW_corner].get_unknown(3);
				}
				else
				{
					rsv8[6] = q;
					rsv8[7] = 0.0;
				}
				//
				icv8 = it_M8M4_LU->second.first.solve(rsv8);

				unknown_RP_value = icv8[0]*RPx*RPy + icv8[2]*RPx + icv8[4]*RPy + icv8[6];
				it->second.set_RP_unknown(RPcounter, 2, unknown_RP_value);

				unknown_RP_value = icv8[1]*RPx*RPy + icv8[3]*RPx + icv8[5]*RPy + icv8[7];
				it->second.set_RP_unknown(RPcounter, 3, unknown_RP_value);
			}

			// ** // ** // ** // ** // ** //
			// test caso dry-non-ghost
			// intervengono direttamente le condizioni al bordo
			// ** // ** // ** // ** // ** //

			it_M6M3_LU_non_cc = noGPs_Matrices.find(SW_corner);
			if (it_M6M3_LU_non_cc != noGPs_Matrices.end())
			{
				vector_component = 0;

				// costruzione del termine noto per l'altezza e calcolo dell'altezza in RP

				if (SW_type == CELL::wet)
				{
					rsv3[vector_component] = cells[SW_corner].get_unknown(1);
					vector_component++;
				}
				else if (ghost_cells.find(SW_corner) != ghost_cells.end())
				{
					rsv3[vector_component] = dZ_dn;
					vector_component++;
				}
				//

				if (SE_type == CELL::wet)
				{
					rsv3[vector_component] = cells[SE_corner].get_unknown(1);
					vector_component++;
				}
				else if (ghost_cells.find(SE_corner) != ghost_cells.end())
				{
					rsv3[vector_component] = dZ_dn;
					vector_component++;
				}
				//

				if (NE_type == CELL::wet)
				{
					rsv3[vector_component] = cells[NE_corner].get_unknown(1);
					vector_component++;
				}
				else if (ghost_cells.find(NE_corner) != ghost_cells.end())
				{
					rsv3[vector_component] = dZ_dn;
					vector_component++;
				}
				//

				if (NW_type == CELL::wet)
				{
					rsv3[vector_component] = cells[NW_corner].get_unknown(1);
					vector_component++;
				}
				else if (ghost_cells.find(NW_corner) != ghost_cells.end())
				{
					rsv3[vector_component] = dZ_dn;
					vector_component++;
				}
				//

				icv3 = it_M6M3_LU_non_cc->second.matrices.second.solve(rsv3);

				unknown_RP_value = icv3[0]*RPx + icv3[1]*RPy + icv3[2];
				it->second.set_RP_unknown(RPcounter, 1, unknown_RP_value);

				// costruzione del termine noto per la portata e calcolo di Qx e Qy in RP

				vector_component = 0;

				if (SW_type == CELL::wet)
				{
					rsv6[vector_component] = cells[SW_corner].get_unknown(2);
					vector_component++;
					rsv6[vector_component] = cells[SW_corner].get_unknown(3);
					vector_component++;
				}
				else if (ghost_cells.find(SW_corner) != ghost_cells.end())
				{
					rsv6[vector_component] = q;
					vector_component++;
					rsv6[vector_component] = 0.0;
					vector_component++;
				}
				//
				if (SE_type == CELL::wet)
				{
					rsv6[vector_component] = cells[SE_corner].get_unknown(2);
					vector_component++;
					rsv6[vector_component] = cells[SE_corner].get_unknown(3);
					vector_component++;
				}
				else if (ghost_cells.find(SE_corner) != ghost_cells.end())
				{
					rsv6[vector_component] = q;
					vector_component++;
					rsv6[vector_component] = 0.0;
					vector_component++;
				}
				//
				if (NE_type == CELL::wet)
				{
					rsv6[vector_component] = cells[NE_corner].get_unknown(2);
					vector_component++;
					rsv6[vector_component] = cells[NE_corner].get_unknown(3);
					vector_component++;
				}
				else if (ghost_cells.find(NE_corner) != ghost_cells.end())
				{
					rsv6[vector_component] = q;
					vector_component++;
					rsv6[vector_component] = 0.0;
					vector_component++;
				}
				//
				if (NW_type == CELL::wet)
				{
					rsv6[vector_component] = cells[NW_corner].get_unknown(2);
					vector_component++;
					rsv6[vector_component] = cells[NW_corner].get_unknown(3);
					vector_component++;
				}
				else if (ghost_cells.find(NW_corner) != ghost_cells.end())
				{
					rsv6[vector_component] = q;
					vector_component++;
					rsv6[vector_component] = 0.0;
					vector_component++;
				}
				//

				icv6 = it_M6M3_LU_non_cc->second.matrices.first.solve(rsv6);

				unknown_RP_value = icv6[0]*RPx + icv6[2]*RPy + icv6[4];
				it->second.set_RP_unknown(RPcounter, 2, unknown_RP_value);

				unknown_RP_value = icv6[1]*RPx + icv6[3]*RPy + icv6[5];
				it->second.set_RP_unknown(RPcounter, 3, unknown_RP_value);

			} // end test dty non ghost
		} // fine scorrimento RP associati alla ghost cell
	} // fine scorrimento ghost cells
} // end method

