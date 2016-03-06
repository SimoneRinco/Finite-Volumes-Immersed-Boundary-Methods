/*
 * grid_move_vertices.cpp
 *
 *  Created on: 25/nov/2011
 *      Author: ubuntu
 */


#include "grid.hpp"
#include "immersed_boundary.hpp"

void grid::move_vertices()
{

	/*
	 * Date 2 celle adiacenti tagliate, se il segmento congiungente i centri viene tagliato più di una volta
	 * viene spostato il vertice centrale sul segmento.
	 * Inoltre vengono spostati i vertici in modo che i lati non taglino il centro cella tranne che nel caso diagonale.
	 * Si evita la situazione in cui 2 celle tagliate adiacenti siano entrambe tagliate da diagonali e la situazione
	 * limite per la regola 2 con una delle due celle tagliate dalla diagonale
	 *
	 */


	IMMERSED_BOUNDARY::label aL, nL; // actual and next label
	double xl,yl,xc,yc,xr,yr; // left,central,right vertices
	double x_center, y_center; // aL cell center
	double gap;


	for (unsigned int i=1; i<ccb.get_N_edges(); i++)
	{
		aL=ccb.get_label(i);
		nL=ccb.get_label(i+1);
		x_center=cells[aL].get_center()(0);
		y_center=cells[aL].get_center()(1);

		// vertice centrale
		xc=ccb.get_edge_vertex(BOUNDARY::right,i)(0);
		yc=ccb.get_edge_vertex(BOUNDARY::right,i)(1);


		if ((nL(0)==aL(0)+1 && nL(1)==aL(1)) || (nL(0)==aL(0)-1 && nL(1)==aL(1)))
		{
			// case EST and WEST
			yl=ccb.get_edge_vertex(BOUNDARY::left,i)(1);
			yr=ccb.get_edge_vertex(BOUNDARY::right,i+1)(1);
			gap=yc-y_center;
			if ((gap>0.0 && yl<y_center-gap && yr<y_center-gap) || (gap<0.0 && yl>y_center-gap && yr>y_center-gap))
				{ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(xc,y_center),i);}

			// caso celle adiacenti tagliate entrambe al centro
			if (cutted_center(aL) && cutted_center(nL))
			{

				yc = ccb.get_edge_vertex(BOUNDARY::right, i)(1);

				// casi di celle adiacenti tagliate da diagonali
				if (fabs(yc-y.at(aL(1)+1))<toll_y)
				{
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(xc,y.at(aL(1))),i);
				}
				else if (fabs(yc-y.at(aL(1)))<toll_y)
				{
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(xc,y.at(aL(1)+1)),i);
				}
				//
				else if (y.at(aL(1)+1)-yc < yc-y.at(aL(1)))
				{
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(xc,0.1*y.at(aL(1)+1)+0.9*yc),i);
				}
				else
				{
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(xc,0.1*y.at(aL(1))+0.9*yc),i);
				}

			}

			// spostamento dei lati che tagliano il centro (tranne caso diagonale)
			if (cutted_center(aL) && !cutted_center_by_diagonal(aL))
			{
				yc = ccb.get_edge_vertex(BOUNDARY::right, i)(1);
				if (yc > y_center)
				{
					yc = 0.1*y.at(aL(1)+1)+0.9*yc;
					if (fabs(y.at(aL(1)+1))<toll_y) {yc = y.at(aL(1)+1);}
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(xc,yc),i);
				}
				else
				{
					yc = 0.1*y.at(aL(1))+0.9*yc;
					if (fabs(y.at(aL(1)))<toll_y) {yc = y.at(aL(1));}
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(xc,yc),i);
				}
			}

			// eliminazione situazione limite per la regola 2
			if (cutted_center_by_diagonal(aL) || cutted_center_by_diagonal(nL))
			{
				if ((fabs(yl-y.at(aL(1)))<toll_y && fabs(yr-y.at(aL(1)))<toll_y) || (fabs(yl-y.at(aL(1)+1))<toll_y && fabs(yr-y.at(aL(1)+1))<toll_y))
				{
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(xc,y_center),i);
				}
			}

			/**/
		}


		else if ((nL(0)==aL(0) && nL(1)==aL(1)-1) || (nL(0)==aL(0) && nL(1)==aL(1)+1))
		{
			// case SUD and NORD
			xl=ccb.get_edge_vertex(BOUNDARY::left,i)(0);
			xr=ccb.get_edge_vertex(BOUNDARY::right,i+1)(0);
			gap=xc-x_center;
			if ((gap>0.0 && xl<x_center-gap && xr<x_center-gap) || (gap<0.0 && xl>x_center-gap && xr>x_center-gap))
				{ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(x_center,yc),i);}

			// caso celle adiacenti tagliate entrambe al centro
			if (cutted_center(aL) && cutted_center(nL))
			{

				xc = ccb.get_edge_vertex(BOUNDARY::right, i)(0);

				// casi di celle adiacenti tagliate da diagonali
				if (fabs(xc-x.at(aL(0)+1))<toll_x)
				{
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(x.at(aL(0)),yc),i);
				}
				else if (fabs(xc-x.at(aL(0)))<toll_x)
				{
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(x.at(aL(0)+1),yc),i);
				}
				//
				else if (x.at(aL(0)+1)-xc < xc-x.at(aL(0)))
				{
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(0.9*xc+0.1*x.at(aL(0)+1),yc),i);
				}
				else
				{
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(0.9*xc+0.1*x.at(aL(0)),yc),i);
				}
			}

			// spostamento dei lati che tagliano il centro (tranne caso diagonale)
			if (cutted_center(aL) && !cutted_center_by_diagonal(aL))
			{
				xc = ccb.get_edge_vertex(BOUNDARY::right, i)(0);
				if (xc > x_center)
				{
					xc = 0.1*x.at(aL(0)+1)+0.9*xc;
					if (fabs(x.at(aL(0)+1))<toll_x) {xc = x.at(aL(0)+1);}
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(xc,yc),i);
				}
				else
				{
					xc = 0.1*x.at(aL(0))+0.9*xc;
					if (fabs(x.at(aL(0)))<toll_x) {xc = x.at(aL(0));}
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(xc,yc),i);
				}
			}

			// eliminazione situazione limite per la regola 2
			if (cutted_center_by_diagonal(aL) || cutted_center_by_diagonal(nL))
			{
				if ((fabs(xl-x.at(aL(0)))<toll_x && fabs(xr-x.at(aL(0)))<toll_x) || (fabs(xl-x.at(aL(0)+1))<toll_x && fabs(xr-x.at(aL(0)+1))<toll_x))
				{
					ccb.set_edge_vertex(BOUNDARY::right,Point2d<double>(x_center,yc),i);
				}
			}
			/**/
		}



	} // end for
}
