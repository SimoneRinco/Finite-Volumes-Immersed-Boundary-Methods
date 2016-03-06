/*
 * grid_set_wet.cpp
 *
 *  Created on: 25/nov/2011
 *      Author: ubuntu
 */

#include "grid.hpp"


void grid::set_wet(label* &L)
{

	// si presuppone che le celle bagnate e non tagliate siano comunicanti tra loro. Due celle sono comunicanti
	// se hanno uno dei quattro lati in comune.
	static int a=1;
	static Point2d<double> V1,V2;
	static Point2d<double> ncc; // cell center of the next cell

	label* next_L;
	next_L = new label;
	if (next_L == 0)
	{
		std::cout<<"Error assigning memory in function set_wet\n";
		throw ERRORS::errors(6);
	}
	//static char c;
	//if (a%50==0) {std::cin>>c;}

	std::cout<<"a " <<a <<"\n";
	a++;
	std::cout<<"cella attuale:\n";
	L->print();

	cells[*L].set_type(CELL::wet);

	// esamina NORD

	(*next_L)(0)=(*L)(0);
	(*next_L)(1)=(*L)(1)+1;

	ncc=cells[*next_L].get_center();


	// non si considera il caso di cella successiva non tagliata e bagnata: vuol dire che è già stata esaminata

	if ((cells[*next_L].get_boundary()==CELL::not_cut) && cells[*next_L].get_type()==CELL::dry)
		{set_wet(next_L);}

	else if (cells[*next_L].get_boundary()==CELL::cut)
	{
		V1=ccb.get_edge_vertex(BOUNDARY::left,ccb.get_n_edge(*next_L));
		V2=ccb.get_edge_vertex(BOUNDARY::right,ccb.get_n_edge(*next_L));

		if ((V1(0)<ncc(0) && V2(0)<ncc(0)) || (V1(0)>ncc(0) && V2(0)>ncc(0)) || std::min(V1(0),V2(0))>x.at((*L)(0)))
		{
			cells[*next_L].set_type(CELL::wet);
		}
		else if (V1(1)+((V2(1)-V1(1))/(V2(0)-V1(0)))*(ncc(0)-V1(0))>ncc(1))
			{cells[*next_L].set_type(CELL::wet);}
		else
			{cells[*next_L].set_type(CELL::dry);}
	}

	// esamina EST

	(*next_L)(0)=(*L)(0)+1;
	(*next_L)(1)=(*L)(1);
	ncc=cells[*next_L].get_center();


	if ((cells[*next_L].get_boundary()==CELL::not_cut) && cells[*next_L].get_type()==CELL::dry)
		{set_wet(next_L);}

	else if (cells[*next_L].get_boundary()==CELL::cut)
	{
		V1=ccb.get_edge_vertex(BOUNDARY::left,ccb.get_n_edge(*next_L));
		V2=ccb.get_edge_vertex(BOUNDARY::right,ccb.get_n_edge(*next_L));

		if ((V1(1)<ncc(1) && V2(1)<ncc(1)) || (V1(1)>ncc(1) && V2(1)>ncc(1)) || std::max(V1(1),V2(1))<y.at((*L)(1)+1))
		{
			cells[*next_L].set_type(CELL::wet);
		}
		else if (V1(0)+((V2(0)-V1(0))/(V2(1)-V1(1)))*(ncc(1)-V1(1))>ncc(0))
			{cells[*next_L].set_type(CELL::wet);}
		else
			{cells[*next_L].set_type(CELL::dry);}
	}

	// esamina SUD

	(*next_L)(0)=(*L)(0);
	(*next_L)(1)=(*L)(1)-1;
	ncc=cells[*next_L].get_center();


	if ((cells[*next_L].get_boundary()==CELL::not_cut) && cells[*next_L].get_type()==CELL::dry)
		{set_wet(next_L);}

	else if (cells[*next_L].get_boundary()==CELL::cut)
	{
		V1=ccb.get_edge_vertex(BOUNDARY::left,ccb.get_n_edge(*next_L));
		V2=ccb.get_edge_vertex(BOUNDARY::right,ccb.get_n_edge(*next_L));

		if ((V1(0)<ncc(0) && V2(0)<ncc(0)) || (V1(0)>ncc(0) && V2(0)>ncc(0)) || std::max(V1(0),V2(0))<x.at((*L)(0)+1))
		{
			cells[*next_L].set_type(CELL::wet);
		}
		else if (V1(1)+((V2(1)-V1(1))/(V2(0)-V1(0)))*(ncc(0)-V1(0))<ncc(1))
			{cells[*next_L].set_type(CELL::wet);}
		else
			{cells[*next_L].set_type(CELL::dry);}
	}


	// esamina OVEST

	(*next_L)(0)=(*L)(0)-1;
	(*next_L)(1)=(*L)(1);
	ncc=cells[*next_L].get_center();


	if ((cells[*next_L].get_boundary()==CELL::not_cut) && cells[*next_L].get_type()==CELL::dry)
		{set_wet(next_L);}

	else if (cells[*next_L].get_boundary()==CELL::cut)
	{
		V1=ccb.get_edge_vertex(BOUNDARY::left,ccb.get_n_edge(*next_L));
		V2=ccb.get_edge_vertex(BOUNDARY::right,ccb.get_n_edge(*next_L));

		if ((V1(1)<ncc(1) && V2(1)<ncc(1)) || (V1(1)>ncc(1) && V2(1)>ncc(1)) || std::min(V1(1),V2(1))>y.at((*L)(1)))
		{
			cells[*next_L].set_type(CELL::wet);
		}
		else if (V1(0)+((V2(0)-V1(0))/(V2(1)-V1(1)))*(ncc(1)-V1(1))<ncc(0))
			{cells[*next_L].set_type(CELL::wet);}
		else
			{cells[*next_L].set_type(CELL::dry);}
	}

	//std::cout<<"Esaurite le direzioni per  ";
	//L->print();
	delete next_L;


}

