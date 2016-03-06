/*
 * grid_build_interpolation_matrices.cpp
 *
 *  Created on: 16/dic/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

void grid::build_interpolation_matrices()

{
	label current_SW_label, adj_label;
	std::vector<ghost_quantities> current_gq;

	IMMERSED_BOUNDARY::M3_LU M3x3_LU;
	IMMERSED_BOUNDARY::M4_LU M4x4_LU;
	IMMERSED_BOUNDARY::M6_LU M6x6_LU;
	IMMERSED_BOUNDARY::M8_LU M8x8_LU;

	IMMERSED_BOUNDARY::M6M3_LU_non_cc M6M3_and_ncc;

	unsigned int GP_counter;
	unsigned int dry_no_GP_counter;
	unsigned int non_considered_corner; // caso matrici 6x6 e 3x3


	for (std::map<label,ghost_cell>::iterator it=ghost_cells.begin(); it!=ghost_cells.end(); it++)
	{
		current_gq = it->second.get_GQs();

		for (std::vector<ghost_quantities>::iterator itgq = current_gq.begin(); itgq != current_gq.end(); itgq++)
		{
			GP_counter = 0;
			dry_no_GP_counter = 0;

			current_SW_label = itgq->SW_corner;

			// conteggio dei GPs e dry-non-GPs nel quadrato di interpolazione

			if (cells[current_SW_label].get_type() != CELL::wet)
			{
				(ghost_cells.find(current_SW_label) != ghost_cells.end()) ? (GP_counter++) : (dry_no_GP_counter++);
			}

			adj_label(0) = current_SW_label(0) + 1;
			adj_label(1) = current_SW_label(1);

			if (cells[adj_label].get_type() != CELL::wet)
			{
				(ghost_cells.find(adj_label) != ghost_cells.end()) ? (GP_counter++) : (dry_no_GP_counter++);
			}

			adj_label(0) = current_SW_label(0) + 1;
			adj_label(1) = current_SW_label(1) + 1;

			if (cells[adj_label].get_type() != CELL::wet)
			{
				(ghost_cells.find(adj_label) != ghost_cells.end()) ? (GP_counter++) : (dry_no_GP_counter++);
			}

			adj_label(0) = current_SW_label(0);
			adj_label(1) = current_SW_label(1) + 1;

			if (cells[adj_label].get_type() != CELL::wet)
			{
				(ghost_cells.find(adj_label) != ghost_cells.end()) ? (GP_counter++) : (dry_no_GP_counter++);
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////////

			// costruzione delle matrici

			if (GP_counter == 0 && dry_no_GP_counter == 0 && all_wet_matrices.find(current_SW_label) == all_wet_matrices.end())
			{
				M4x4_LU = build_all_wet_4x4_matrix(current_SW_label);
				all_wet_matrices.insert(std::pair<label,IMMERSED_BOUNDARY::M4_LU>(current_SW_label,M4x4_LU));
			}


			else if (GP_counter > 0 && dry_no_GP_counter == 0 && GPs_Matrices.find(current_SW_label) == GPs_Matrices.end())
			{
				M8x8_LU = build_GPs_8x8_matrix(current_SW_label);
				M4x4_LU = build_GPs_4x4_matrix(current_SW_label);
				GPs_Matrices.insert(std::pair<label,IMMERSED_BOUNDARY::M8M4_LU>(current_SW_label,IMMERSED_BOUNDARY::M8M4_LU(M8x8_LU,M4x4_LU)));
			}


			else if (dry_no_GP_counter > 0 && noGPs_Matrices.find(current_SW_label) == noGPs_Matrices.end())
			{
				if (dry_no_GP_counter > 1)
				{
					std::cout<< "#\nWARNING: più di un dry-non-ghost point nel quadrato di interpolazione di vertice SW ";
					current_SW_label.print();
					std::cout<<"Non è stata costruita nessuna matrice\n#\n";
					//throw ERRORS::errors(300);
				}

				else
				{
					M6x6_LU = build_noGPs_6x6_matrix(current_SW_label, non_considered_corner);
					M3x3_LU = build_noGPs_3x3_matrix(current_SW_label);

					M6M3_and_ncc.matrices = IMMERSED_BOUNDARY::M6M3_LU(M6x6_LU, M3x3_LU);
					M6M3_and_ncc.non_cc = non_considered_corner;

					noGPs_Matrices.insert(std::pair<label,IMMERSED_BOUNDARY::M6M3_LU_non_cc>(current_SW_label,M6M3_and_ncc));

				}
			}
		} // end for
	} // end for
} // end method


////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IMMERSED_BOUNDARY::M4_LU grid::build_all_wet_4x4_matrix(const label& SW_label)
{
	IMMERSED_BOUNDARY::M4 M4x4;
	unsigned int SWlx(SW_label(0));
	unsigned int SWly(SW_label(1));
	unsigned int row_index;

	Point2d<double> C1(cells[SW_label].get_center());
	Point2d<double> C2(cells[label(SWlx+1, SWly)].get_center());
	Point2d<double> C3(cells[label(SWlx+1, SWly+1)].get_center());
	Point2d<double> C4(cells[label(SWlx, SWly+1)].get_center());

	std::vector<Point2d<double> > centers;
	centers.push_back(C1);
	centers.push_back(C2);
	centers.push_back(C3);
	centers.push_back(C4);

	row_index = 0;

	for (std::vector<Point2d<double> >::iterator it=centers.begin(); it!=centers.end(); it++)
	{
		M4x4(row_index,0) = (*it)(0) * (*it)(1);
		M4x4(row_index,1) = (*it)(0);
		M4x4(row_index,2) = (*it)(1);
		M4x4(row_index,3) = 1.0;

		row_index++;
	}

	IMMERSED_BOUNDARY::M4_LU return_M(M4x4);
	if (return_M.isInvertible() == false)
	{

		std::cout<<"#\nWARNING: Matrix 4x4 associated to SW corner ";
		SW_label.print();
		std::cout<<"is near singular\n#\n";
	}

	return return_M;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMMERSED_BOUNDARY::M8_LU grid::build_GPs_8x8_matrix(const label& SW_label)
{
	IMMERSED_BOUNDARY::M8 M8x8;

	unsigned int SWlx(SW_label(0));
	unsigned int SWly(SW_label(1));
	unsigned int row_index;

	ghost_cell GC;
	label GP_label;
	std::vector<ghost_quantities> GQs;


	std::vector<unsigned int> wet_corners;
	std::vector<unsigned int> GP_corners;
	std::vector<label> corner_labels;
	std::vector<Point2d<double> > square_corners; // vertici del quadrato di interpolazione (centri cella)

	Point2d<double> IP; // interpolation point (un centro cella o un RP)
	Point2d<double> n; // normale uscente

	corner_labels.push_back(label(SWlx,SWly)); // SW
	square_corners.push_back(cells[label(SWlx,SWly)].get_center());
	corner_labels.push_back(label(SWlx+1,SWly)); // SE
	square_corners.push_back(cells[label(SWlx+1,SWly)].get_center());
	corner_labels.push_back(label(SWlx+1,SWly+1)); // NE
	square_corners.push_back(cells[label(SWlx+1,SWly+1)].get_center());
	corner_labels.push_back(label(SWlx,SWly+1)); // NW
	square_corners.push_back(cells[label(SWlx,SWly+1)].get_center());

	for (unsigned int i=0; i<4; i++)
	{
		(ghost_cells.find(corner_labels.at(i)) == ghost_cells.end()) ? (wet_corners.push_back(i+1)) : (GP_corners.push_back(i+1));
	}

	for (std::vector<unsigned int>::iterator it = wet_corners.begin(); it!=wet_corners.end(); it++)
	{

		IP = square_corners.at(*it-1);

		row_index = 2*(*it-1);

		M8x8(row_index,0) = IP(0)*IP(1);
		M8x8(row_index,1) = 0.0;
		M8x8(row_index,2) = IP(0);
		M8x8(row_index,3) = 0.0;
		M8x8(row_index,4) = IP(1);
		M8x8(row_index,5) = 0.0;
		M8x8(row_index,6) = 1.0;
		M8x8(row_index,7) = 0.0;

		row_index = 2*(*it-1) + 1;

		M8x8(row_index,0) = 0.0;
		M8x8(row_index,1) = IP(0)*IP(1);
		M8x8(row_index,2) = 0.0;
		M8x8(row_index,3) = IP(0);
		M8x8(row_index,4) = 0.0;
		M8x8(row_index,5) = IP(1);
		M8x8(row_index,6) = 0.0;
		M8x8(row_index,7) = 1.0;

	}

	for (std::vector<unsigned int>::iterator it = GP_corners.begin(); it!=GP_corners.end(); it++)
	{
		GP_label = corner_labels.at(*it-1);
		GC = ghost_cells[GP_label];
		GQs = GC.get_GQs();

		if (GQs.size() == 1)
		{
			IP = GQs.at(0).BP;
			n = GQs.at(0).normal;
		}
		else if (GQs.size() == 2)
		{
		// l'IP considerato è quello che cade nel quadrato di interpolazione oppure (se non esiste) l'altro

		if (GQs.at(0).SW_corner == SW_label)
			{
			IP = GQs.at(0).BP;
			n = GQs.at(0).normal;
			}
		else
			{
			IP = GQs.at(1).BP;
			n = GQs.at(1).normal;
			}
		}

		row_index = 2*(*it-1);

		M8x8(row_index,0) = IP(0)*IP(1)*n(0);
		M8x8(row_index,1) = IP(0)*IP(1)*n(1);
		M8x8(row_index,2) = IP(0)*n(0);
		M8x8(row_index,3) = IP(0)*n(1);
		M8x8(row_index,4) = IP(1)*n(0);
		M8x8(row_index,5) = IP(1)*n(1);
		M8x8(row_index,6) = n(0);
		M8x8(row_index,7) = n(1);

		row_index = 2*(*it-1) + 1;

		M8x8(row_index,0) = -IP(1)*n(0)*n(1)-IP(0)*n(1)*n(1);
		M8x8(row_index,1) = IP(1)*n(0)*n(0)+IP(0)*n(0)*n(1);
		M8x8(row_index,2) = -n(0)*n(1);
		M8x8(row_index,3) = n(0)*n(0);
		M8x8(row_index,4) = -n(1)*n(1);
		M8x8(row_index,5) = n(0)*n(1);
		M8x8(row_index,6) = 0.0;
		M8x8(row_index,7) = 0.0;


	}

	IMMERSED_BOUNDARY::M8_LU return_M(M8x8);
	if (return_M.isInvertible() == false)
	{

		std::cout<<"#\nWARNING: Matrix 8x8 associated to SW corner ";
		SW_label.print();
		std::cout<<"is near singular\n#\n";
	}

	return return_M;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IMMERSED_BOUNDARY::M4_LU grid::build_GPs_4x4_matrix(const label& SW_label)
{
	IMMERSED_BOUNDARY::M4 M4x4;

	unsigned int SWlx(SW_label(0));
	unsigned int SWly(SW_label(1));
	unsigned int row_index;

	ghost_cell GC;
	label GP_label;
	std::vector<ghost_quantities> GQs;


	std::vector<unsigned int> wet_corners;
	std::vector<unsigned int> GP_corners;
	std::vector<label> corner_labels;
	std::vector<Point2d<double> > square_corners; // vertici del quadrato di interpolazione (centri cella)

	Point2d<double> IP; // interpolation point (un centro cella o un RP)
	Point2d<double> n; // normale uscente

	corner_labels.push_back(label(SWlx,SWly)); // SW
	square_corners.push_back(cells[label(SWlx,SWly)].get_center());
	corner_labels.push_back(label(SWlx+1,SWly)); // SE
	square_corners.push_back(cells[label(SWlx+1,SWly)].get_center());
	corner_labels.push_back(label(SWlx+1,SWly+1)); // NE
	square_corners.push_back(cells[label(SWlx+1,SWly+1)].get_center());
	corner_labels.push_back(label(SWlx,SWly+1)); // NW
	square_corners.push_back(cells[label(SWlx,SWly+1)].get_center());

	for (unsigned int i=0; i<4; i++)
	{
		(ghost_cells.find(corner_labels.at(i)) == ghost_cells.end()) ? (wet_corners.push_back(i+1)) : (GP_corners.push_back(i+1));
	}

	for (std::vector<unsigned int>::iterator it = wet_corners.begin(); it!=wet_corners.end(); it++)
	{

		IP = square_corners.at(*it-1);

		row_index = (*it)-1;

		M4x4(row_index,0) = IP(0)*IP(1);
		M4x4(row_index,1) = IP(0);
		M4x4(row_index,2) = IP(1);
		M4x4(row_index,3) = 1.0;

	}

	for (std::vector<unsigned int>::iterator it = GP_corners.begin(); it!=GP_corners.end(); it++)
	{
		GP_label = corner_labels.at(*it-1);
		GC = ghost_cells[GP_label];
		GQs = GC.get_GQs();

		if (GQs.size() == 1)
		{
			IP = GQs.at(0).BP;
			n = GQs.at(0).normal;
		}
		else if (GQs.size() == 2)
		{
		// l'IP considerato è quello che cade nel quadrato di interpolazione oppure (se non esiste) l'altro

		if (GQs.at(0).SW_corner == SW_label)
			{
			IP = GQs.at(0).BP;
			n = GQs.at(0).normal;
			}
		else
			{
			IP = GQs.at(1).BP;
			n = GQs.at(1).normal;
			}
		}

		row_index = (*it)-1;

		M4x4(row_index,0) = IP(0)*n(1) + IP(1)*n(0);
		M4x4(row_index,1) = n(0);
		M4x4(row_index,2) = n(1);
		M4x4(row_index,3) = 0.0;

	}

	IMMERSED_BOUNDARY::M4_LU return_M(M4x4);
	if (return_M.isInvertible() == false)
	{

		std::cout<<"#\nWARNING: Matrix 4x4 associated to SW corner ";
		SW_label.print();
		std::cout<<"is near singular\n#\n";
	}

	return return_M;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMMERSED_BOUNDARY::M6_LU grid::build_noGPs_6x6_matrix(const label& SW_label, unsigned int& non_considered_corner)
{

	// In questo caso bisogna salvare l'ulteriore informazione di quale vertice non viene considerato

	IMMERSED_BOUNDARY::M6 M6x6;

	unsigned int SWlx(SW_label(0));
	unsigned int SWly(SW_label(1));
	unsigned int row_index;

	ghost_cell GC;
	label GP_label;
	std::vector<ghost_quantities> GQs;


	std::vector<unsigned int> wet_corners;
	std::vector<unsigned int> GP_corners;
	std::vector<unsigned int>::iterator wet_corners_iterator;
	std::vector<unsigned int>::iterator GP_corners_iterator;

	unsigned int gap=0;

	std::vector<label> corner_labels;
	std::vector<Point2d<double> > square_corners; // vertici del quadrato di interpolazione (centri cella)

	Point2d<double> IP; // interpolation point (un centro cella o un RP)
	Point2d<double> n; // normale uscente

	corner_labels.push_back(label(SWlx,SWly)); // SW
	square_corners.push_back(cells[label(SWlx,SWly)].get_center());
	corner_labels.push_back(label(SWlx+1,SWly)); // SE
	square_corners.push_back(cells[label(SWlx+1,SWly)].get_center());
	corner_labels.push_back(label(SWlx+1,SWly+1)); // NE
	square_corners.push_back(cells[label(SWlx+1,SWly+1)].get_center());
	corner_labels.push_back(label(SWlx,SWly+1)); // NW
	square_corners.push_back(cells[label(SWlx,SWly+1)].get_center());

	for (unsigned int i=0; i<4; i++)
	{

		if (ghost_cells.find(corner_labels.at(i)) != ghost_cells.end())
		{
			GP_corners.push_back(i+1);
		}

		else if (cells[corner_labels.at(i)].get_type() == CELL::wet)
		{
			wet_corners.push_back(i+1);
		}


	}

	wet_corners_iterator = wet_corners.begin();
	GP_corners_iterator = GP_corners.begin();

	non_considered_corner = 4;

	for (int i=0; i<3; i++)
	{

		if (wet_corners_iterator != wet_corners.end() && *wet_corners_iterator == i+1+gap)
		{
		IP = square_corners.at(i+gap);

		row_index = 2*i;

		M6x6(row_index,0) = IP(0);
		M6x6(row_index,1) = 0.0;
		M6x6(row_index,2) = IP(1);
		M6x6(row_index,3) = 0.0;
		M6x6(row_index,4) = 1.0;
		M6x6(row_index,5) = 0.0;

		row_index = 2*i + 1;

		M6x6(row_index,0) = 0.0;
		M6x6(row_index,1) = IP(0);
		M6x6(row_index,2) = 0.0;
		M6x6(row_index,3) = IP(1);
		M6x6(row_index,4) = 0.0;
		M6x6(row_index,5) = 1.0;

		wet_corners_iterator++;
		}

		else if (GP_corners_iterator != GP_corners.end() && *GP_corners_iterator == i+1+gap)
		{

		GP_label = corner_labels.at(i+gap);
		GC = ghost_cells[GP_label];
		GQs = GC.get_GQs();

			if (GQs.size() == 1)
			{
				IP = GQs.at(0).BP;
				n = GQs.at(0).normal;
			}
			else if (GQs.size() == 2)
			{
				// l'IP considerato è quello che cade nel quadrato di interpolazione oppure (se non esiste) l'altro

				if (GQs.at(0).SW_corner == SW_label)
				{
					IP = GQs.at(0).BP;
					n = GQs.at(0).normal;
				}
				else
				{
					IP = GQs.at(1).BP;
					n = GQs.at(1).normal;
				}
			}

			row_index = 2*i;

			M6x6(row_index,0) = IP(0)*n(0);
			M6x6(row_index,1) = IP(0)*n(1);
			M6x6(row_index,2) = IP(1)*n(0);
			M6x6(row_index,3) = IP(1)*n(1);
			M6x6(row_index,4) = n(0);
			M6x6(row_index,5) = n(1);

			row_index = 2*i + 1;

			M6x6(row_index,0) = -n(0)*n(1);
			M6x6(row_index,1) = n(0)*n(0);
			M6x6(row_index,2) = -n(1)*n(1);
			M6x6(row_index,3) = n(0)*n(1);
			M6x6(row_index,4) = 0.0;
			M6x6(row_index,5) = 0.0;

			GP_corners_iterator++;

		}
		else
		{
			non_considered_corner = i+1;
			gap=1;
			i--;
		}

	} // end for

	IMMERSED_BOUNDARY::M6_LU return_M(M6x6);
	if (return_M.isInvertible() == false)
	{

		std::cout<<"#\nWARNING: Matrix 6x6 associated to SW corner ";
		SW_label.print();
		std::cout<<"is near singular\n#\n";
	}

	return return_M;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IMMERSED_BOUNDARY::M3_LU grid::build_noGPs_3x3_matrix(const label& SW_label)
{

	IMMERSED_BOUNDARY::M3 M3x3;

	unsigned int SWlx(SW_label(0));
	unsigned int SWly(SW_label(1));
	unsigned int row_index;

	ghost_cell GC;
	label GP_label;
	std::vector<ghost_quantities> GQs;


	std::vector<unsigned int> wet_corners;
	std::vector<unsigned int> GP_corners;
	std::vector<unsigned int>::iterator wet_corners_iterator;
	std::vector<unsigned int>::iterator GP_corners_iterator;

	unsigned int gap=0;

	std::vector<label> corner_labels;
	std::vector<Point2d<double> > square_corners; // vertici del quadrato di interpolazione (centri cella)

	Point2d<double> IP; // interpolation point (un centro cella o un RP)
	Point2d<double> n; // normale uscente

	corner_labels.push_back(label(SWlx,SWly)); // SW
	square_corners.push_back(cells[label(SWlx,SWly)].get_center());
	corner_labels.push_back(label(SWlx+1,SWly)); // SE
	square_corners.push_back(cells[label(SWlx+1,SWly)].get_center());
	corner_labels.push_back(label(SWlx+1,SWly+1)); // NE
	square_corners.push_back(cells[label(SWlx+1,SWly+1)].get_center());
	corner_labels.push_back(label(SWlx,SWly+1)); // NW
	square_corners.push_back(cells[label(SWlx,SWly+1)].get_center());

	for (unsigned int i=0; i<4; i++)
	{

		if (ghost_cells.find(corner_labels.at(i)) != ghost_cells.end())
		{
			GP_corners.push_back(i+1);
		}

		else if (cells[corner_labels.at(i)].get_type() == CELL::wet)
		{
			wet_corners.push_back(i+1);
		}

	}
	wet_corners_iterator = wet_corners.begin();
	GP_corners_iterator = GP_corners.begin();


	for (int i=0; i<3; i++)
	{

		if (wet_corners_iterator != wet_corners.end() && *wet_corners_iterator == i+1+gap)
		{

		IP = square_corners.at(i+gap);

		row_index = i;

		M3x3(row_index,0) = IP(0);
		M3x3(row_index,1) = IP(1);
		M3x3(row_index,2) = 1.0;

		wet_corners_iterator++;

		}

		else if(GP_corners_iterator != GP_corners.end() && *GP_corners_iterator == i+1+gap)
		{
		GP_label = corner_labels.at(i+gap);
		GC = ghost_cells[GP_label];
		GQs = GC.get_GQs();

		if (GQs.size() == 1)
		{
			IP = GQs.at(0).BP;
			n = GQs.at(0).normal;
		}
		else if (GQs.size() == 2)
		{
		// l'IP considerato è quello che cade nel quadrato di interpolazione oppure (se non esiste) l'altro

		if (GQs.at(0).SW_corner == SW_label)
			{
			IP = GQs.at(0).BP;
			n = GQs.at(0).normal;
			}
		else
			{
			IP = GQs.at(1).BP;
			n = GQs.at(1).normal;
			}
		}

		row_index = i;

		M3x3(row_index,0) = n(0);
		M3x3(row_index,1) = n(1);
		M3x3(row_index,2) = 0.0;

		GP_corners_iterator++;

	}
		else
		{
			gap=1;
			i--;
		}
	}

	IMMERSED_BOUNDARY::M3_LU return_M(M3x3);
	if (return_M.isInvertible() == false)
	{

		std::cout<<"#\nWARNING: Matrix 3x3 associated to SW corner ";
		SW_label.print();
		std::cout<<"is near singular\n#\n";
	}

	return return_M;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////




