/*
 * grid_set_wet2.cpp
 *
 *  Created on: 06/dic/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

void grid::set_wet2()
{

	bool vsjccc; // vertical segment joining cell centers cutted

	CELL::type current_cell_type = CELL::dry;
	label current_label, next_label;
	double x1,x2, y1, y2; // ascisse e ordinate dei vertici del lato considerato
	double xcv; // coordinate del vertice centrale (caso particolare entrambe tagliate con vertice centrale sul segmento)
	Point2d<double> cc; // cell center
	double xc, yc;
	double y, m;
	double temp;

	bool processed;

	for (unsigned int i=0; i<Nx+2; i++)
	{
		current_cell_type = CELL::dry;
		current_label(0)=i;
		next_label(0)=i;

		for (unsigned int j=0; j<Ny+1; j++)
		{
			current_label(1)=j;
			next_label(1)=j+1; // cella a NORD

			vsjccc = false;

			// analisi cella corrente

			if (cells[current_label].get_boundary()==CELL::cut)
			{
				x1=ccb.get_edge_vertex(BOUNDARY::left,ccb.get_n_edge(current_label))(0);
				x2=ccb.get_edge_vertex(BOUNDARY::right,ccb.get_n_edge(current_label))(0);
				y1=ccb.get_edge_vertex(BOUNDARY::left,ccb.get_n_edge(current_label))(1);
				y2=ccb.get_edge_vertex(BOUNDARY::right,ccb.get_n_edge(current_label))(1);

				cc=cells[current_label].get_center();
				xc=cc(0);
				yc=cc(1);



				if (((x1<xc) && (x2<xc)) || ((x1>xc) && (x2>xc)))
				{
					vsjccc=false;
				}
				else if (fabs(x1-x2)<toll_x)
				{
					if (x2<x1)
					{
						temp = x1;
						x1 = x2;
						x2 = temp;

						temp = y1;
						y1 = y2;
						y2 = temp;
					}

					(((xc-x1 > x2-xc) && (y1 < y2)) || ((xc-x1 < x2-xc) && (y1 > y2)) ) ? (vsjccc=true) : (vsjccc=false);
				}

				else
				{
					m = (y2-y1)/(x2-x1);
					y = y1 + m*(xc-x1);
					(y>yc) ? (vsjccc=true) : (vsjccc=false);
				}
			}

			//
			if (vsjccc == true)
			{
				processed = false;
				if (cells[next_label].get_boundary()==CELL::cut)
				{
					if (ccb.get_n_edge(current_label) > ccb.get_n_edge(next_label) && ccb.get_n_edge(next_label) !=1)
					{
						xcv = ccb.get_edge_vertex(BOUNDARY::left, ccb.get_n_edge(current_label))(0);
						x1 = ccb.get_edge_vertex(BOUNDARY::right, ccb.get_n_edge(current_label))(0);
						x2 = ccb.get_edge_vertex(BOUNDARY::left, ccb.get_n_edge(next_label))(0);

					}

					else
					{
						xcv = ccb.get_edge_vertex(BOUNDARY::right, ccb.get_n_edge(current_label))(0);
						x1 = ccb.get_edge_vertex(BOUNDARY::left, ccb.get_n_edge(current_label))(0);
						x2 = ccb.get_edge_vertex(BOUNDARY::right, ccb.get_n_edge(next_label))(0);
					}

					if (fabs(xcv-xc)<0.1*toll_x && ((x1>xc && x2>xc)||(x1<xc && x2<xc)) )
					{
						cells[next_label].set_type(cells[current_label].get_type());
						current_cell_type = cells[current_label].get_type();
						processed = true;
					}
				}

				if (processed == false)
				{
					if (current_cell_type == CELL::dry)
					{
						cells[next_label].set_type(CELL::wet);
						current_cell_type = CELL::wet;
					}
					else
					{
						cells[next_label].set_type(CELL::dry);
						current_cell_type = CELL::dry;
					}
				}
			}



			// se la cella corrente non è tagliata e se la next lo è
			else if (cells[next_label].get_boundary()==CELL::cut)
			{

				// vsjjc false e next cell tagliata

				x1=ccb.get_edge_vertex(BOUNDARY::left,ccb.get_n_edge(next_label))(0);
				x2=ccb.get_edge_vertex(BOUNDARY::right,ccb.get_n_edge(next_label))(0);
				y1=ccb.get_edge_vertex(BOUNDARY::left,ccb.get_n_edge(next_label))(1);
				y2=ccb.get_edge_vertex(BOUNDARY::right,ccb.get_n_edge(next_label))(1);

				cc=cells[next_label].get_center();
				xc=cc(0);
				yc=cc(1);

				if (((x1<xc) && (x2<xc)) || ((x1>xc) && (x2>xc)))
				{
					vsjccc=false;
				}
				else if (fabs(x1-x2)<toll_x)
				{
					if (x2<x1)
					{
						temp = x1;
						x1 = x2;
						x2 = temp;

						temp = y1;
						y1 = y2;
						y2 = temp;
					}

					(((xc-x1 > x2-xc) && (y1 < y2)) || ((xc-x1 < x2-xc) && (y1 > y2)) ) ? (vsjccc=false) : (vsjccc=true);
				}
				else
				{
					m = (y2-y1)/(x2-x1);
					y = y1 + m*(xc-x1);
					(y>yc) ? (vsjccc=false) : (vsjccc=true);
				}

				if (vsjccc == true)
				{
					if (current_cell_type == CELL::dry)
					{
						cells[next_label].set_type(CELL::wet);
						current_cell_type = CELL::wet;
					}
					else
					{
						cells[next_label].set_type(CELL::dry);
						current_cell_type = CELL::dry;
					}
				}
				else
				{
					cells[next_label].set_type(current_cell_type);
				}
			}
			else
			{
				cells[next_label].set_type(current_cell_type);
			}

		} // end for
	} // end for






} // end method


void grid::set_wet_cutted_centers()
{
	// imposta bagnate le celle il cui centro del lato sta in un rettangolo di dimensioni toll_x/10 x toll_y/10
	// centrato sul centro cella

	Point2d<double> V1,V2, Center, edge_center;
	int n_edge;
	label cell_lab;


	for (std::map<label,cell>::iterator it=cells.begin(); it!=cells.end(); it++)
	{
		if ((it->second.get_boundary() == CELL::cut) && (it->second.get_type() == CELL::dry))
		{
			cell_lab = it->first;
			n_edge = ccb.get_n_edge(cell_lab);
			V1 = ccb.get_edge_vertex(BOUNDARY::left, n_edge);
			V2 = ccb.get_edge_vertex(BOUNDARY::right, n_edge);

			Center = it->second.get_center();
			edge_center(0) = 0.5*(V1(0)+V2(0));
			edge_center(1) = 0.5*(V1(1)+V2(1));

			if (fabs(Center(0)-edge_center(0))<toll_x/10 && fabs(Center(1)-edge_center(1))<toll_y/10)
			{
				it->second.set_type(CELL::wet);
			}
		}
	}
}
