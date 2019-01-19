/*
 * grid_build_ghost_cells.cpp
 *
 *  Created on: 13/dic/2011
 *      Author: ubuntu
 */

#include "grid.hpp"


void set_gq(ghost_quantities& gq, const Point2d<double>& BP, Point2d<double>& RP, const Point2d<double>& normal, const IMMERSED_BOUNDARY::label& associated_WC, const IMMERSED_BOUNDARY::label& SWcorner, const unsigned int& number_ghost_corners, const unsigned int& edge_number)
{
	gq.BP = BP;
	gq.RP = RP;
	gq.SW_corner = SWcorner;
	gq.associated_WC = associated_WC;
	gq.edge_number = edge_number;
	gq.normal = normal;
	gq.number_ghost_corners = number_ghost_corners;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMMERSED_BOUNDARY::label grid::find_SW_label(const Point2d<double>& RP)
{
	// restituisce la cella il cui centro è il vertice di SW del quadrato di interpolazione in cui cade RP
	unsigned int i=0;
	unsigned int j=0;
	while (x.at(i)<RP(0)-0.5*dx) {i++;}
	while (y.at(j)<RP(1)-0.5*dy) {j++;}
	return label(i-1,j-1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int grid::find_number_ghost_corners(const label& SW_label)
{
	// restituisce il numero di vertici asciutti presenti nel quadrato di interpolazione
	unsigned int ngc=0;
	unsigned int xl = SW_label(0);
	unsigned int yl = SW_label(1);

	if (cells[SW_label].get_type() == CELL::dry) {ngc++;}
	if (cells[label(xl+1,yl)].get_type() == CELL::dry) {ngc++;}
	if (cells[label(xl+1,yl+1)].get_type() == CELL::dry) {ngc++;}
	if (cells[label(xl,yl+1)].get_type() == CELL::dry) {ngc++;}
	return ngc;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void grid::build_single_BP_quantities(const label& ghost_label, const label& wet_label, ghost_quantities CGgq, std::vector<ghost_quantities>& gqs)
{
	// date la ghost cell e la wet cell (tagliata) restituisce le quantità ghost associate alla cella bagnata.
	// Se il BP trovato non è valido allora la GC è tagliata e quindi possiede un BP valido che viene assegnato
	// alla wet cell (è il BP da considerare quando viene processata la wet cell).

	// I parametri calcolati aggiornano il vettore gqs

	using namespace IMMERSED_BOUNDARY;

	Point2d<double> V1, V2, cV, cell_center;
	Point2d<double> BP, RP, normal_vector;
	label SWlabel;
	unsigned int ngc;
	unsigned int edge_number;
	bool BP_ok, BP_ok_x, BP_ok_y;
	bool cV_SW, cV_SE, cV_NE, cV_NW;
	bool BP_and_cV_very_close;
	int close_vertex;

	ghost_quantities gq;

	edge_number = ccb.get_n_edge(wet_label);
	V1 = ccb.get_edge_vertex(BOUNDARY::left, edge_number);
	V2 = ccb.get_edge_vertex(BOUNDARY::right, edge_number);
	cell_center = cells[ghost_label].get_center();
	find_BP_RP_normal(V1, V2, cell_center, BP, RP, normal_vector);
	//////////////////////////////////
	// controllo BP valido

	// controllo BP

	if (fabs(V1(0)-V2(0))<toll_x/10.0 && fabs(V1(0)-BP(0))<toll_x/10.0 && fabs(V2(0)-BP(0))<toll_x/10.0)
	{
		BP_ok_x = true;
	}
	else
	{
		BP_ok_x = std::min(V1(0),V2(0))< BP(0) && BP(0) < std::max(V1(0),V2(0));
	}

	if (fabs(V1(1)-V2(1))<toll_y/10.0 && fabs(V1(1)-BP(1))<toll_y/10.0 && fabs(V2(1)-BP(1))<toll_y/10.0)
	{
		BP_ok_y = true;
	}
	else
	{
		BP_ok_y = std::min(V1(1),V2(1))< BP(1) && BP(1) < std::max(V1(1),V2(1));
	}

	BP_ok = BP_ok_x && BP_ok_y;

	// caso in cui il BP sia molto vicino ad un vertice del lato che coincide con un vertice di cella
	close_vertex = closest_vertex(BP, V1, V2);
	(close_vertex == 1) ? (cV = V1) : (cV=V2);
	cV_SW = fabs(cV(0)-x.at(wet_label(0)))<toll_x && fabs(cV(1)-y.at(wet_label(1)))<toll_y;
	cV_SE = fabs(cV(0)-x.at(wet_label(0)+1))<toll_x && fabs(cV(1)-y.at(wet_label(1)))<toll_y;
	cV_NE = fabs(cV(0)-x.at(wet_label(0)+1))<toll_x && fabs(cV(1)-y.at(wet_label(1)+1))<toll_y;
	cV_NW = fabs(cV(0)-x.at(wet_label(0)))<toll_x && fabs(cV(1)-y.at(wet_label(1)+1))<toll_y;

	BP_and_cV_very_close = fabs(BP(0)-cV(0)) < toll_x && fabs(BP(1)-cV(1)) < toll_y;

	if (BP_ok || ( (cV_SW || cV_SE || cV_NE || cV_NW) && BP_and_cV_very_close ) )
	{
		SWlabel = find_SW_label(RP);
		ngc = find_number_ghost_corners(SWlabel);
		set_gq(gq, BP, RP, normal_vector, wet_label, SWlabel, ngc, edge_number);
		gq.RP_and_GP_unknowns.resize(N_equations);
		gqs.push_back(gq);
	}
	else if (cells[ghost_label].get_boundary() == CELL:: cut)
	{
		CGgq.associated_WC = wet_label;
		gqs.push_back(CGgq);
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void grid::build_ghost_cells()
{

	label current_ghost_label;
	label adj_wet_label;

	ghost_cell current_ghost_cell;
	ghost_cell new_ghost_cell;

	std::vector<label> Adj_labels;

	std::vector<ghost_quantities> gqs;
	ghost_quantities gq, CGgq;


	unsigned int adj_wet_counter;
	unsigned int edge_number;

	// GC quantities nel caso in cui GC sia una cella tagliata
	Point2d<double> GCBP; // ghost cell boundary point (se la ghost cell è tagliata)
	Point2d<double> GCRP; // ghost cell reflected point (se la ghost cell è tagliata)
	Point2d<double> GCnormal;
	label GC_SWlabel;
	unsigned int GC_ngc;
	unsigned int GC_edge_number;
	//

	Point2d<double> V1, V2, cell_center;

	////////////////////////////////////
	// identificazione delle ghost cells

	for (unsigned int i=0; i<Nx+2; i++)
	{
		for (unsigned int j=0; j<Ny+2; j++)
		{
			if (cells[label(i,j)].get_type() == CELL::dry)
		{
			Adj_labels.clear();
			adj_wet_counter=0;

			if (i == 0)
			{
				Adj_labels.push_back(label(1,j));
			}
			else if (i == Nx+1)
			{
				Adj_labels.push_back(label(Nx,j));
			}
			else
			{
				Adj_labels.push_back(label(i-1,j));
				Adj_labels.push_back(label(i+1,j));
			}

			if (j == 0)
			{
				Adj_labels.push_back(label(i,1));
			}
			else if (j == Ny+1)
			{
				Adj_labels.push_back(label(i,Ny));
			}
			else
			{
				Adj_labels.push_back(label(i,j+1));
				Adj_labels.push_back(label(i,j-1));
			}

			for (std::vector<label>::iterator it = Adj_labels.begin(); it!=Adj_labels.end(); it++)
			{
				if (cells[*it].get_type() == CELL::wet)
				{
				adj_wet_counter++;
				}
			}

			if (adj_wet_counter>0)
			{

				if (adj_wet_counter > 2)
				{
					std::cout<< "WARNING: ghost cell ";
					label(i,j).print();
					std::cout<< "ha più di 2 wet cells adiacenti. Provare ad infittire la griglia o semplificare il dominio.\n";
					std::cout<<"Tento di costruire ugualmente le matrici di interpolazione.\n";
					//throw ERRORS::errors(500);
				}

				new_ghost_cell.cell::operator=(cells[label(i,j)]);
				/*
				std::cout<<"aggiunta ghost cell ";
				label(i,j).print();
				*/
				ghost_cells.insert(std::pair<label,ghost_cell>(label(i,j),new_ghost_cell));

			}

		} // end if

		}
	} // fine identificazione delle ghost cells

	///////////////////////////////////////////////////////////////////////////////////////////////////

	// costruzione delle ghost quantities per ogni ghost cell

	for (std::map<label,ghost_cell>::iterator it=ghost_cells.begin(); it!=ghost_cells.end(); it++)
	{
		current_ghost_label = it->first;
		current_ghost_cell = it->second;

		gqs.clear();


		//current_ghost_label.print();

		// calcolo di BP e RP nel caso la ghost cell sia tagliata

		if (current_ghost_cell.get_boundary() == CELL::cut)
		{
			edge_number = ccb.get_n_edge(current_ghost_label);
			GC_edge_number = edge_number;
			V1 = ccb.get_edge_vertex(BOUNDARY::left, edge_number);
			V2 = ccb.get_edge_vertex(BOUNDARY::right, edge_number);
			cell_center=current_ghost_cell.get_center();

			IMMERSED_BOUNDARY::find_BP_RP_normal(V1, V2, cell_center, GCBP, GCRP, GCnormal);
			GC_SWlabel=find_SW_label(GCRP);
			GC_ngc = find_number_ghost_corners(GC_SWlabel);

			CGgq.BP = GCBP;
			CGgq.RP = GCRP;
			CGgq.SW_corner = GC_SWlabel;
			CGgq.edge_number = GC_edge_number;
			CGgq.normal = GCnormal;
			CGgq.number_ghost_corners = GC_ngc;
			CGgq.RP_and_GP_unknowns.resize(N_equations);
			// manca da assegnare CGgq.associated_WC

		} // end if GC tagliata

		///////////////////////////////////////////////////////////////////////////////////////////////////

		// NORD BAGNATO

		adj_wet_label(0)=current_ghost_label(0);
		adj_wet_label(1)=current_ghost_label(1)+1;

		if (adj_wet_label(1)<Ny+1 && cells[adj_wet_label].get_type() == CELL::wet)
		{
			if (cells[adj_wet_label].get_boundary() == CELL::cut)
			{
				build_single_BP_quantities(current_ghost_label, adj_wet_label, CGgq, gqs);
			}
			else
			{
				CGgq.associated_WC = adj_wet_label;
				gqs.push_back(CGgq);
			}
		}
		//
		// EST BAGNATO

		adj_wet_label(0)=current_ghost_label(0)+1;
		adj_wet_label(1)=current_ghost_label(1);

		if (adj_wet_label(0)<Nx+1 && cells[adj_wet_label].get_type() == CELL::wet)
		{
			//std::cout<<"EST bagnato\n";
			if (cells[adj_wet_label].get_boundary() == CELL::cut)
			{
				build_single_BP_quantities(current_ghost_label, adj_wet_label, CGgq, gqs);
			}
			else
			{
				CGgq.associated_WC = adj_wet_label;
				gqs.push_back(CGgq);
			}
		}
		//
		// SUD BAGNATO

		adj_wet_label(0)=current_ghost_label(0);
		adj_wet_label(1)=current_ghost_label(1)-1;

		if (adj_wet_label(1)>0 && cells[adj_wet_label].get_type() == CELL::wet)
		{
			if (cells[adj_wet_label].get_boundary() == CELL::cut)
			{
				build_single_BP_quantities(current_ghost_label, adj_wet_label, CGgq, gqs);
			}
			else
			{
				CGgq.associated_WC = adj_wet_label;
				gqs.push_back(CGgq);
			}
		}
		//
		// OVEST BAGNATO

		adj_wet_label(0)=current_ghost_label(0)-1;
		adj_wet_label(1)=current_ghost_label(1);

		if (adj_wet_label(0)>0 && cells[adj_wet_label].get_type() == CELL::wet)
		{
			if (cells[adj_wet_label].get_boundary() == CELL::cut)
			{
				build_single_BP_quantities(current_ghost_label, adj_wet_label, CGgq, gqs);
			}
			else
			{
				CGgq.associated_WC = adj_wet_label;
				gqs.push_back(CGgq);
			}
		}
		//

	if (gqs.size() == 0)
	{
		std::cout<< "WARNING: Ghost cell ";
		current_ghost_label.print();
		std::cout<<"non ha nessun punto riflesso\n";
		//throw ERRORS::errors(200);
	}
	(it->second).set_GQs(gqs);

	} // end for






}
