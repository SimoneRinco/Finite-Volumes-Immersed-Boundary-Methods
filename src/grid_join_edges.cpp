/*
 * grid_join_edges.cpp
 *
 *  Created on: 02/dic/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

void set_min_and_positions (const IMMERSED_BOUNDARY::label& adj_label, const IMMERSED_BOUNDARY::position& adj_pos, IMMERSED_BOUNDARY::position& next_cell_pos, const Point2d<double>& current_Vertex, Point2d<double>*& next_Vertex, Point2d<double>*& new_Vertex, double& min_dist, std::map<IMMERSED_BOUNDARY::label, IMMERSED_BOUNDARY::Edge, IMMERSED_BOUNDARY::p_comp>& single_edges, std::set<IMMERSED_BOUNDARY::label, IMMERSED_BOUNDARY::p_comp>& considered_labels);

void grid::join_edges(const label& current_label, Point2d<double>*& current_Vertex, std::map<label, IMMERSED_BOUNDARY::Edge, p_comp>& single_edges, int& N_single_edges, std::set<label,p_comp>& considered_labels)
{

using namespace IMMERSED_BOUNDARY;

char a; // variabile per debug

position current_vertex_pos; // posizione all'interno della propria cella (current_label) del vertice da unire
position next_cutted_cell_pos; // posizione della cella contenente il vertice più vicino rispetto alla cella corrente
position next_vertex_pos = Intern; // posizione all'interno della propria cella (adj_label) del vertice più vicino

label next_label;
Point2d<double>* next_Vertex = 0; // vertice da unire al current_Vertex
Point2d<double>* new_Vertex = 0; // nuovo vertice che dovrà essere unito (altro vertice della cella che contiene next_Vertex)
std::map<label,Edge>::iterator it_single_edges;
int clV;

// coordinate x e y della cella corrente
const double& Lx = x.at(current_label(0));
const double& Rx = x.at(current_label(0)+1);
const double& By = y.at(current_label(1));
const double& Ty = y.at(current_label(1)+1);

double xm, ym;

// coordinate vertice corrente e successivo (da unire)
double cx, cy, nx, ny;

// spigoli della cella corrente
Point2d<double> NEc(Rx,Ty);
Point2d<double> NWc(Lx,Ty);
Point2d<double> SWc(Lx,By);
Point2d<double> SEc(Rx,By);

// celle confinanti
unsigned int clx = current_label(0);
unsigned int cly = current_label(1);

// celle considerate


label Nl(clx, cly+1);
label NEl(clx+1, cly+1);
label El(clx+1, cly);
label SEl(clx+1, cly-1);
label Sl(clx, cly-1);
label SWl(clx-1, cly-1);
label Wl(clx-1, cly);
label NWl(clx-1, cly+1);

////////////////////////////////////////////////////////////////////////////////////////////////////////

if (N_single_edges > 0)
{

//std::cout<<N_single_edges <<std::endl;
N_single_edges--;

current_vertex_pos = V_position(current_label, *current_Vertex);
closest_cell_pos_and_vertices(current_label, current_Vertex, next_Vertex, new_Vertex, next_cutted_cell_pos, next_vertex_pos, single_edges, considered_labels);

cx = (*current_Vertex)(0);
cy = (*current_Vertex)(1);
nx = (*next_Vertex)(0);
ny = (*next_Vertex)(1);

/*
// visualizzazioni per debug
std::cout<< "Cella corrente: ";
current_label.print();
std::cout<< "di centro ";
cells[current_label].get_center().print();
std::cout<< "current vertex: ";
current_Vertex->print();
std::cout<< "next vertex: ";
next_Vertex->print();
std::cout<< "new vertex: ";
new_Vertex->print();
std::cout<<"current vertex pos: " <<current_vertex_pos << "; next cutted cell pos: " <<next_cutted_cell_pos <<"; next vertex pos: " <<next_vertex_pos <<"\n";
//std::cin>> a;
/**/

switch(current_vertex_pos)
{

// (1)
case N:
	switch(next_cutted_cell_pos)
	{
	case N:
		switch(next_vertex_pos)
		{
		case W: case SW: ccb.add_edge(NWc, current_label); join_edges(Nl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case S: xm = 0.5*(cx+nx); ccb.add_edge(xm, Ty, current_label); join_edges(Nl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case SE: case E: ccb.add_edge(NEc, current_label); join_edges(Nl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(101);
		}
	break;

	case E:
		ccb.add_edge(NEc, current_label); join_edges(El,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;

	case W:
		ccb.add_edge(NWc, current_label); join_edges(Wl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;

	case NE:
		switch(next_vertex_pos)
		{
		case W:
			ccb.add_edge(cx, cy, current_label);
			ccb.add_edge(nx, ny, Nl);
			join_edges(NEl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case SW: case S: ccb.add_edge(NEc, current_label); join_edges(NEl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(102);
		}
	break;

	case NW:
		switch(next_vertex_pos)
		{
		case E:
			ccb.add_edge(cx, cy, current_label);
			ccb.add_edge(nx, ny, Nl);
			join_edges(NWl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case SE: case S: ccb.add_edge(NWc, current_label); join_edges(NWl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(103);
		}
	break;

	default:;
	}

break;

// (2)
case NE:
	ccb.add_edge(NEc, current_label);
	switch(next_cutted_cell_pos)
	{
	case N: join_edges(Nl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	case NE: join_edges(NEl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	case E: join_edges(El,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	default:
		std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
		throw ERRORS::errors(104);
	}
break;
//////////////////////////////////////////////////////////////


// (3)
case E:
	switch(next_cutted_cell_pos)
	{
	case E:
		switch(next_vertex_pos)
		{
		case N: case NW: ccb.add_edge(NEc, current_label); join_edges(El,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case W: ym = 0.5*(cy+ny); ccb.add_edge(Rx, ym, current_label); join_edges(El,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case SW: case S: ccb.add_edge(SEc, current_label); join_edges(El,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(105);
		}
	break;

	case S:
		ccb.add_edge(SEc, current_label); join_edges(Sl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;

	case N:
		ccb.add_edge(NEc, current_label); join_edges(Nl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;

	case SE:
		switch(next_vertex_pos)
		{
		case N:
			ccb.add_edge(cx, cy, current_label);
			ccb.add_edge(nx, ny, El);
			join_edges(SEl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case NW: case W: ccb.add_edge(SEc, current_label); join_edges(SEl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(106);
		}
	break;

	case NE:
		switch(next_vertex_pos)
		{
		case S:
			ccb.add_edge(cx, cy, current_label);
			ccb.add_edge(nx, ny, El);
			join_edges(NEl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case W: case SW: ccb.add_edge(NEc, current_label); join_edges(NEl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(107);
		}
	break;

	default:
		std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
		throw ERRORS::errors(108);
	}

break;

// (4)
case SE:
	ccb.add_edge(SEc, current_label);
	switch(next_cutted_cell_pos)
	{
	case E: join_edges(El,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	case SE: join_edges(SEl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	case S: join_edges(Sl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	default:
		std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
		throw ERRORS::errors(109);
	}
break;
//////////////////////////////////////////////////////////////

// (5)
case S:
	switch(next_cutted_cell_pos)
	{
	case S:
		switch(next_vertex_pos)
		{
		case E: case NE: ccb.add_edge(SEc, current_label); join_edges(Sl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case N: xm = 0.5*(cx+nx); ccb.add_edge(xm, By, current_label); join_edges(Sl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case W: case NW: ccb.add_edge(SWc, current_label); join_edges(Sl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(110);
		}
	break;

	case W:
		ccb.add_edge(SWc, current_label); join_edges(Wl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;

	case E:
		ccb.add_edge(SEc, current_label); join_edges(El,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;

	case SW:
		switch(next_vertex_pos)
		{
		case E:
			ccb.add_edge(cx, cy, current_label);
			ccb.add_edge(nx, ny, Sl);
			join_edges(SWl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case NE: case N: ccb.add_edge(SWc, current_label); join_edges(SWl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(111);
		}
	break;

	case SE:
		switch(next_vertex_pos)
		{
		case W:
			ccb.add_edge(cx, cy, current_label);
			ccb.add_edge(nx, ny, Sl);
			join_edges(SEl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case NW: case N: ccb.add_edge(SEc, current_label); join_edges(SEl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(112);
		}
	break;

	default:
		std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
		throw ERRORS::errors(113);
	}

break;

// (6)
case SW:
	ccb.add_edge(SWc, current_label);
	switch(next_cutted_cell_pos)
	{
	case S: join_edges(Sl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	case SW: join_edges(SWl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	case W: join_edges(Wl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	default:
		std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
		throw ERRORS::errors(114);
	}
break;
//////////////////////////////////////////////////////////////

// (7)
case W:
	switch(next_cutted_cell_pos)
	{
	case W:
		switch(next_vertex_pos)
		{
		case S: case SE: ccb.add_edge(SWc, current_label); join_edges(Wl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case E: ym = 0.5*(cy+ny); ccb.add_edge(Lx, ym, current_label); join_edges(Wl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case N: case NE: ccb.add_edge(NWc, current_label); join_edges(Wl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(115);
		}
	break;

	case N:
		ccb.add_edge(NWc, current_label); join_edges(Nl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;

	case S:
		ccb.add_edge(SWc, current_label); join_edges(Sl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;

	case NW:
		switch(next_vertex_pos)
		{
		case S:
			ccb.add_edge(cx, cy, current_label);
			ccb.add_edge(nx, ny, Wl);
			join_edges(NWl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case E: case SE: ccb.add_edge(NWc, current_label); join_edges(NWl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(116);
		}
	break;

	case SW:
		switch(next_vertex_pos)
		{
		case N:
			ccb.add_edge(cx, cy, current_label);
			ccb.add_edge(nx, ny, Wl);
			join_edges(SWl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		case NE: case E: ccb.add_edge(SWc, current_label); join_edges(SWl,new_Vertex, single_edges, N_single_edges, considered_labels);
		break;
		default:
			std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
			throw ERRORS::errors(117);
		}
	break;

	default:
		std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
		throw ERRORS::errors(118);
	}

break;

// (8)
case NW:
	ccb.add_edge(NWc, current_label);
	switch(next_cutted_cell_pos)
	{
	case W: join_edges(Wl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	case NW: join_edges(NWl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	case N: join_edges(Nl,new_Vertex, single_edges, N_single_edges, considered_labels);
	break;
	default:
		std::cout<<"Errore durante la creazione del dominio: provare a raffinare la griglia.\n";
		throw ERRORS::errors(119);
	}
break;
//////////////////////////////////////////////////////////////



} // end switch

} // end if

} // end method


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void grid::closest_cell_pos_and_vertices(const label& current_label, Point2d<double>*& current_Vertex, Point2d<double>*& next_Vertex, Point2d<double>*& new_Vertex, IMMERSED_BOUNDARY::position& next_cell_pos, IMMERSED_BOUNDARY::position& next_Vertex_pos, std::map<label, IMMERSED_BOUNDARY::Edge, IMMERSED_BOUNDARY::p_comp>& single_edges, std::set<label, p_comp>& considered_labels)
{

	// Data la cella corrente ritorna la posizione della cella confinante (tra le 8) che contiene il vertice più
	// vicino al current_Vertex. Ritorna anche next_Vertex (il vertice più vicino che è quello da unire) e la sua
	// posizione nella cella di appartenenza e new_Vertex che è l'altro vertice del lato al quale appartiene next_Vertex

	using namespace IMMERSED_BOUNDARY;


	label adj_label;
	double min_dist = +INFINITY;
	const unsigned int& clx = current_label(0);
	const unsigned int& cly = current_label(1);

	label Nl(clx, cly+1);
	label NEl(clx+1, cly+1);
	label El(clx+1, cly);
	label SEl(clx+1, cly-1);
	label Sl(clx, cly-1);
	label SWl(clx-1, cly-1);
	label Wl(clx-1, cly);
	label NWl(clx-1, cly+1);

	set_min_and_positions(Nl, N, next_cell_pos, *current_Vertex, next_Vertex, new_Vertex, min_dist, single_edges, considered_labels);
	set_min_and_positions(NEl, NE, next_cell_pos, *current_Vertex, next_Vertex, new_Vertex, min_dist, single_edges, considered_labels);
	set_min_and_positions(El, E, next_cell_pos, *current_Vertex, next_Vertex, new_Vertex, min_dist, single_edges, considered_labels);
	set_min_and_positions(SEl, SE, next_cell_pos, *current_Vertex, next_Vertex, new_Vertex, min_dist, single_edges, considered_labels);
	set_min_and_positions(Sl, S, next_cell_pos, *current_Vertex, next_Vertex, new_Vertex, min_dist, single_edges, considered_labels);
	set_min_and_positions(SWl, SW, next_cell_pos, *current_Vertex, next_Vertex, new_Vertex, min_dist, single_edges, considered_labels);
	set_min_and_positions(Wl, W, next_cell_pos, *current_Vertex, next_Vertex, new_Vertex, min_dist, single_edges, considered_labels);
	set_min_and_positions(NWl, NW, next_cell_pos, *current_Vertex, next_Vertex, new_Vertex, min_dist, single_edges, considered_labels);

	// posizionamento nella cella adiacente del vertice da unire (next_vertex)

	switch (next_cell_pos)
	{
	case N: considered_labels.insert(Nl); next_Vertex_pos = V_position(Nl, *next_Vertex); break;
	case NE: considered_labels.insert(NEl); next_Vertex_pos = V_position(NEl, *next_Vertex); break;
	case E: considered_labels.insert(El); next_Vertex_pos = V_position(El, *next_Vertex); break;
	case SE: considered_labels.insert(SEl); next_Vertex_pos = V_position(SEl, *next_Vertex); break;
	case S: considered_labels.insert(Sl); next_Vertex_pos = V_position(Sl, *next_Vertex); break;
	case SW: considered_labels.insert(SWl); next_Vertex_pos = V_position(SWl, *next_Vertex); break;
	case W: considered_labels.insert(Wl); next_Vertex_pos = V_position(Wl, *next_Vertex); break;
	case NW: considered_labels.insert(NWl); next_Vertex_pos = V_position(NWl, *next_Vertex); break;
	default:
		std::cout<<"Errore: la cella";
		current_label.print();
		std::cout<<"non confina con nessuna nuova cella tagliata rispetto a quelle già esaminate per la costruzione del dominio finale. Consultare il file initial_edges.\n";
		// è necessario salvare qui i vettori x e y per la visualizzazione con Matlab
		std::ofstream OutFile;
		OutFile.open("./data/domain/x", std::ios::out);
		for (int i=0; i<Nx+3; i++)
		{
			OutFile<<x.at(i)<<std::endl;
		}
		OutFile.close();

		OutFile.open("./data/domain/y", std::ios::out);
		for (int i=0; i<Ny+3; i++)
		{
			OutFile<<y.at(i)<<std::endl;
		}
		OutFile.close();

		OutFile.open("./data/domain/considered_cells", std::ios::out);
		for (std::set<label>::iterator it = considered_labels.begin(); it != considered_labels.end(); it++)
		{
			OutFile<< (*it)(0) <<' ' <<(*it)(1) <<std::endl;
		}
		OutFile.close();
		throw ERRORS::errors(100);
	}

}

double distance(const Point2d<double>& V1, const Point2d<double>& V2)
{
	return std::max(fabs(V1(0)-V2(0)), fabs(V1(1)-V2(1))); // distanza in norma infinito
}

void set_min_and_positions (const IMMERSED_BOUNDARY::label& adj_label, const IMMERSED_BOUNDARY::position& adj_pos, IMMERSED_BOUNDARY::position& next_cell_pos, const Point2d<double>& current_Vertex, Point2d<double>*& next_Vertex, Point2d<double>*& new_Vertex, double& min_dist, std::map<IMMERSED_BOUNDARY::label, IMMERSED_BOUNDARY::Edge, IMMERSED_BOUNDARY::p_comp>& single_edges, std::set<IMMERSED_BOUNDARY::label, IMMERSED_BOUNDARY::p_comp>& considered_labels)
{

// Se la cella adj_label è tagliata e contiene un vertice a distanza minore di min_dist dal current_Vertex
// la funzione ritorna (tramite le referenze passate): next_cell_pos, next_Vertex e new_Vertex.

using namespace IMMERSED_BOUNDARY;

std::map<label,Edge>::iterator it_single_edges;
std::set<label>::iterator it_considered_labels;

double dist1, dist2;

it_single_edges = single_edges.find(adj_label);
it_considered_labels = considered_labels.find(adj_label);

if ((single_edges.find(adj_label) != single_edges.end()) && (considered_labels.find(adj_label) == considered_labels.end()))
{

	dist1 = distance(current_Vertex, it_single_edges->second.first);
	dist2 = distance(current_Vertex, it_single_edges->second.second);

	if ((dist1 < dist2) && (dist1 < min_dist))
	{
		min_dist = dist1;
		next_cell_pos = adj_pos;
		next_Vertex = &(it_single_edges->second.first);
		new_Vertex = &(it_single_edges->second.second);
	}

	else if ((dist2 < dist1) && (dist2 < min_dist))
	{
		min_dist = dist2;
		next_cell_pos = adj_pos;
		next_Vertex = &(it_single_edges->second.second);
		new_Vertex = &(it_single_edges->second.first);
	}

} // end if

} // end function

