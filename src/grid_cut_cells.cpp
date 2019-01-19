/*
 * grid_cut_cells.cpp
 *
 *  Created on: 23/dic/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

void grid::cut_cells( const label& actual_label, Point2d<double>& actual_vertex, const label& finish_label, Point2d<double>& finish_vertex, const double& toll_x, const double& toll_y, bool exit)
{
	using namespace IMMERSED_BOUNDARY;

	if (exit==false)
	{
	//std::cout<<"edge++\n";
	label new_label;
	Point2d<double> new_Vertex;
	double t;
	position V_pos, V_n_pos;

	// posizione del vertice V nella sua cella: eventuale riposizionamento sul bordo
	V_pos = V_position(actual_label, actual_vertex);

	// controllo vicinanza vertici attuale e finale
	if (fabs(actual_vertex(0)-finish_vertex(0)) < toll_x && fabs(actual_vertex(1)-finish_vertex(1)) < toll_y)
	{
		//std::cout<<"Nodi vicini\n";
		ccb.add_edge(actual_vertex, actual_label);
		exit=true;
	}

	else {

		// posizione di Vn rispetto a V
		V_n_pos = relative_position(finish_vertex, actual_vertex, toll_x, toll_y);
		/*
		std::cout<<"### cut_cells ###\n";
		std::cout<<"actual label:\n";
		actual_label.print();
		std::cout<<"finish label:\n";
		finish_label.print();
		std::cout<<"actual vertex:\n";
		actual_vertex.print();
		std::cout<<"finish vertex:\n";
		finish_vertex.print();
		std::cout<<"posizione del vertice attuale nella propria cella: " <<V_pos <<"\n";
		std::cout<<"mi muovo a ";
		std::cout<<V_n_pos <<"\n";
		std::cout<<"### end cut_cells ###\n";
		*/
		switch (V_n_pos)
		{
		case (N):
			new_label(0)=actual_label(0);
			new_label(1)=actual_label(1)+1;
			new_Vertex(0)=actual_vertex(0);
			new_Vertex(1)=y.at(new_label(1));

			ccb.add_edge(new_Vertex, actual_label);
			if (new_label(1)==finish_label(1)) {exit = true;}
		break;
		case (E):
			new_label(0)=actual_label(0)+1;
			new_label(1)=actual_label(1);
			new_Vertex(0)=x.at(new_label(0));
			new_Vertex(1)=actual_vertex(1);

			ccb.add_edge(new_Vertex, actual_label);
			if (new_label(0)==finish_label(0)) {exit = true;}
		break;
		case (S):
			new_label(0)=actual_label(0);
			new_label(1)=actual_label(1)-1;
			new_Vertex(0)=actual_vertex(0);
			new_Vertex(1)=y.at(actual_label(1));

			ccb.add_edge(new_Vertex, actual_label);
			if (new_label(1)==finish_label(1)) {exit=true;}
		break;
		case (W):
			new_label(0)=actual_label(0)-1;
			new_label(1)=actual_label(1);
			new_Vertex(0)=x.at(actual_label(0));
			new_Vertex(1)=actual_vertex(1);

			ccb.add_edge(new_Vertex, actual_label);
			if (new_label(0)==finish_label(0)) {exit=true;}
		break;
		case (NE):
			if (V_pos==NE)
			{
				new_label(0)=actual_label(0)+1;
				new_label(1)=actual_label(1)+1;
				new_Vertex=actual_vertex;
			}

			else if (V_pos==E || V_pos==SE)
			{
				new_label(0)=actual_label(0)+1;
				new_label(1)=actual_label(1);
				new_Vertex=actual_vertex;
			}

			else if (V_pos==N || V_pos==NW)
			{
				new_label(0)=actual_label(0);
				new_label(1)=actual_label(1)+1;
				new_Vertex=actual_vertex;
			}

			else
			{
				Point2d<double> Vp(x.at(actual_label(0)+1),y.at(actual_label(1)+1));
				if (fabs(Vp(0)-finish_vertex(0))<toll_x && fabs(Vp(1)-finish_vertex(1))<toll_y)
				{
					new_label=finish_label;
					new_Vertex=finish_vertex;
				}
				else
				{
					if ((Vp(1)-actual_vertex(1))*(finish_vertex(0)-actual_vertex(0))> (finish_vertex(1)-actual_vertex(1))*(Vp(0)-actual_vertex(0)))
					{
					//std::cout<<"esco a destra\n";
					new_label(0)=actual_label(0)+1;
					new_label(1)=actual_label(1);
					new_Vertex(0)=x.at(new_label(0));
					t = (new_Vertex(0)-actual_vertex(0))/(finish_vertex(0)-actual_vertex(0));
					new_Vertex(1)=actual_vertex(1)+t*(finish_vertex(1)-actual_vertex(1));
					}
					else
					{
					//std::cout<<"esco sopra\n";
					new_label(0)=actual_label(0);
					new_label(1)=actual_label(1)+1;
					new_Vertex(1)=y.at(new_label(1));
					t = (new_Vertex(1)-actual_vertex(1))/(finish_vertex(1)-actual_vertex(1));
					new_Vertex(0)=actual_vertex(0)+t*(finish_vertex(0)-actual_vertex(0));
					}
				}
			}

			ccb.add_edge(new_Vertex, actual_label);
			if (new_label==finish_label) {exit=true;}
		break;

		case (NW):
			if (V_pos==NW)
			{
				new_label(0)=actual_label(0)-1;
				new_label(1)=actual_label(1)+1;
				new_Vertex=actual_vertex;
			}

			else if (V_pos==W || V_pos==SW)
			{
				new_label(0)=actual_label(0)-1;
				new_label(1)=actual_label(1);
				new_Vertex=actual_vertex;
			}

			else if (V_pos==N || V_pos==NE)
			{
				new_label(0)=actual_label(0);
				new_label(1)=actual_label(1)+1;
				new_Vertex=actual_vertex;
			}

			else
			{
				Point2d<double> Vp(x.at(actual_label(0)),y.at(actual_label(1)+1));
				if (fabs(Vp(0)-finish_vertex(0))<toll_x && fabs(Vp(1)-finish_vertex(1))<toll_y)
				{
					new_label=finish_label;
					new_Vertex=finish_vertex;
				}
				else
				{
				if ((Vp(1)-actual_vertex(1))*(finish_vertex(0)-actual_vertex(0))> (finish_vertex(1)-actual_vertex(1))*(Vp(0)-actual_vertex(0)))
				{
					//std::cout<<"esco a nord\n";
					new_label(0)=actual_label(0);
					new_label(1)=actual_label(1)+1;
					new_Vertex(1)=y.at(new_label(1));
					t = (new_Vertex(1)-actual_vertex(1))/(finish_vertex(1)-actual_vertex(1));
					new_Vertex(0)=actual_vertex(0)+t*(finish_vertex(0)-actual_vertex(0));
				}
				else
				{
					//std::cout<<"esco a sinistra\n";
					new_label(0)=actual_label(0)-1;
					new_label(1)=actual_label(1);
					new_Vertex(0)=x.at(actual_label(0));
					t = (new_Vertex(0)-actual_vertex(0))/(finish_vertex(0)-actual_vertex(0));
					new_Vertex(1)=actual_vertex(1)+t*(finish_vertex(1)-actual_vertex(1));
				}

			}
			}
			ccb.add_edge(new_Vertex, actual_label);
			if (new_label==finish_label) {exit=true;}
		break;

		case (SW):
			if (V_pos==SW)
			{
				new_label(0)=actual_label(0)-1;
				new_label(1)=actual_label(1)-1;
				new_Vertex=actual_vertex;
			}

			else if (V_pos==S || V_pos==SE)
			{
				new_label(0)=actual_label(0);
				new_label(1)=actual_label(1)-1;
				new_Vertex=actual_vertex;
			}

			else if (V_pos==W || V_pos==NW)
			{
				new_label(0)=actual_label(0)-1;
				new_label(1)=actual_label(1);
				new_Vertex=actual_vertex;
			}

			else
			{
				Point2d<double> Vp(x.at(actual_label(0)),y.at(actual_label(1)));
				if (fabs(Vp(0)-finish_vertex(0))<toll_x && fabs(Vp(1)-finish_vertex(1))<toll_y)
				{
					new_label=finish_label;
					new_Vertex=finish_vertex;
				}
				else
				{
				if ((Vp(1)-actual_vertex(1))*(finish_vertex(0)-actual_vertex(0))> (finish_vertex(1)-actual_vertex(1))*(Vp(0)-actual_vertex(0)))
				{
					new_label(0)=actual_label(0)-1;
					new_label(1)=actual_label(1);
					new_Vertex(0)=x.at(actual_label(0));
					t = (new_Vertex(0)-actual_vertex(0))/(finish_vertex(0)-actual_vertex(0));
					new_Vertex(1)=actual_vertex(1)+t*(finish_vertex(1)-actual_vertex(1));
				}
				else
				{
					new_label(0)=actual_label(0);
					new_label(1)=actual_label(1)-1;
					new_Vertex(1)=y.at(actual_label(1));
					t = (new_Vertex(1)-actual_vertex(1))/(finish_vertex(1)-actual_vertex(1));
					new_Vertex(0)=actual_vertex(0)+t*(finish_vertex(0)-actual_vertex(0));
				}

			}
			}
			ccb.add_edge(new_Vertex, actual_label);
			if (new_label==finish_label) {exit=true;}
		break;

		case (SE):
			if (V_pos==SE)
			{
				new_label(0)=actual_label(0)+1;
				new_label(1)=actual_label(1)-1;
				new_Vertex=actual_vertex;
			}

			else if (V_pos==S || V_pos==SW)
			{
				new_label(0)=actual_label(0);
				new_label(1)=actual_label(1)-1;
				new_Vertex=actual_vertex;
			}

			else if (V_pos==E || V_pos==NE)
			{
				new_label(0)=actual_label(0)+1;
				new_label(1)=actual_label(1);
				new_Vertex=actual_vertex;
			}

			else
			{
				Point2d<double> Vp(x.at(actual_label(0)+1),y.at(actual_label(1)));
				if (fabs(Vp(0)-finish_vertex(0))<toll_x && fabs(Vp(1)-finish_vertex(1))<toll_y)
				{
					new_label=finish_label;
					new_Vertex=finish_vertex;
				}
				else
				{
				if ((Vp(1)-actual_vertex(1))*(finish_vertex(0)-actual_vertex(0))> (finish_vertex(1)-actual_vertex(1))*(Vp(0)-actual_vertex(0)))
				{
					new_label(0)=actual_label(0);
					new_label(1)=actual_label(1)-1;
					new_Vertex(1)=y.at(actual_label(1));
					t = (new_Vertex(1)-actual_vertex(1))/(finish_vertex(1)-actual_vertex(1));
					new_Vertex(0)=actual_vertex(0)+t*(finish_vertex(0)-actual_vertex(0));
				}
				else
				{
					new_label(0)=actual_label(0)+1;
					new_label(1)=actual_label(1);
					new_Vertex(0)=x.at(new_label(0));
					t = (new_Vertex(0)-actual_vertex(0))/(finish_vertex(0)-actual_vertex(0));
					new_Vertex(1)=actual_vertex(1)+t*(finish_vertex(1)-actual_vertex(1));
				}

			}
			}
			ccb.add_edge(new_Vertex, actual_label);
			if (new_label==finish_label) {exit=true;}
		break;

                case Intern:
                  throw 1;

		} // end switch


		//std::cout<<"Richiamo cut_cells...\n";
		cut_cells(new_label, ccb.get_last_vertex(), finish_label, finish_vertex, toll_x, toll_y, exit);
	}
	} // end if (exit==false)
	else
	{
		V_position(finish_label, actual_vertex);
		ccb.set_edge_vertex(BOUNDARY::right, actual_vertex, ccb.get_N_edges());
	}
} // end method
