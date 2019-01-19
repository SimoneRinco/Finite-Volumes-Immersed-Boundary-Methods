/*
 * grid_ctr_level_set.cpp
 *
 *  Created on: 30/nov/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

grid::grid(const IMMERSED_BOUNDARY::p_fun& p_f, const IMMERSED_BOUNDARY::p_grad_fun& p_grad_f, const std::string& input_f, const p_comp& comp_function):
	if_parameters(input_f.c_str()),
	Nx(if_parameters("grid_parameters/Nx",0)),
	Ny(if_parameters("grid_parameters/Ny",0)),
	N_equations(if_parameters("equations_parameters/N_equations",0)),
	x(Nx+3),
	y(Ny+3),
	cells(comp_function),
	ghost_cells(comp_function),
	all_wet_matrices(comp_function),
	GPs_Matrices(comp_function),
	noGPs_Matrices(comp_function)
{

using namespace IMMERSED_BOUNDARY;


std::map<label, IMMERSED_BOUNDARY::Edge, p_comp> single_edges(comp_function);
std::map<label, IMMERSED_BOUNDARY::Edge, p_comp> first_edge(comp_function);

const double k = if_parameters("grid_parameters/k",0.25);

const double x_min = if_parameters("grid_parameters/x_min",0.0);
const double x_max = if_parameters("grid_parameters/x_max",0.0);
const double y_min = if_parameters("grid_parameters/y_min",0.0);
// const double y_max = if_parameters("grid_parameters/y_max",0.0);

Point2d<double> P, wet_center, dry_center1, dry_center2;
Point2d<double> zero;
Point2d<double>* first_Vertex;

label wet_cell, dry_cell;
label actual_label, next_label;
label Key, first_wet, first_dry, first_cutted_cell, first_label;
IMMERSED_BOUNDARY::Edge edge;
std::map<label,IMMERSED_BOUNDARY::Edge>::iterator it_single_edges;
int N_single_edges;

cell C(N_equations);
double fun_value;

dx = (x_max-x_min)/Nx;
dy = dx; // imposizione celle quadrate
//dy = (y_max-y_min)/Ny;

const double toll_x = k*dx;
const double toll_y = k*dy;

// tolleranze iniziali per creazione bordo tramite level set

this->toll_x = 0.01*dx;
this->toll_y = 0.01*dy;

//inizializzazione vettori x e y (griglia)

for(unsigned int nx=0; nx<Nx+3; nx++)
{
  x.at(nx)=x_min + (nx-1)*dx;
}
for(unsigned int ny=0; ny<Ny+3; ny++)
{
  y.at(ny)=y_min + (ny-1)*dy;
}

// inizializzazione centri cella e impostazione iniziale asciutto/bagnato
for(unsigned int nx=0; nx<Nx+2; nx++)
{
	for(unsigned int ny=0; ny<Ny+2; ny++)
	{
		Key.set_values(nx,ny);
		P(0)=x_min+0.5*dx+(nx-1)*dx;
		P(1)=y_min+0.5*dy+(ny-1)*dy;
		C.set_center(P);
		fun_value = p_f(P);

		(fun_value < 0) ? (C.set_type(CELL::wet)) : (C.set_type(CELL::dry));

		cells.insert(std::pair<label,cell>(Key,C));
	}
}

label wet_label, Nl, El, Sl, Ol;
int w;
bool valid_wet_cells = true; // diventa non valido se una cella bagnata confina con più di 2 celle asciutte

// creazione lati singoli

for (std::map<label,cell>::iterator it=cells.begin(); it!=cells.end(); it++)
{
	w=0;
	if (it->second.get_type()==CELL::wet)
	{
		wet_label = it->first;
		Nl(0)=wet_label(0);
		Nl(1)=wet_label(1)+1;

		El(0)=wet_label(0)+1;
		El(1)=wet_label(1);

		Sl(0)=wet_label(0);
		Sl(1)=wet_label(1)-1;

		Ol(0)=wet_label(0)-1;
		Ol(1)=wet_label(1);

		// controllo NORD
		if (cells[Nl].get_type()==CELL::dry)
			w+=1;
		// controllo EST
		if (cells[El].get_type()==CELL::dry)
			w+=2;
		// controllo SUD
		if (cells[Sl].get_type()==CELL::dry)
			w+=4;
		// controllo OVEST
		if (cells[Ol].get_type()==CELL::dry)
			w+=8;

		switch (w)
		{

		// casi singola cella asciutta confinante

		case 0: // cella bagnata interna al dominio
			break;
		case 1:
			build_and_add_edge(wet_label, Nl, true, 0.01*dy, p_f, p_grad_f, single_edges);
			break;
		case 2:
			build_and_add_edge(wet_label, El, false, 0.01*dx, p_f, p_grad_f, single_edges);
			break;
		case 4:
			build_and_add_edge(wet_label, Sl, true, 0.01*dy, p_f, p_grad_f, single_edges);
			break;
		case 8:
			build_and_add_edge(wet_label, Ol, false, 0.01*dx, p_f, p_grad_f, single_edges);
			break;

			// casi due celle asciutte confinanti

		case 3:
			build_and_add_edge2(wet_label, Nl, El, p_f, p_grad_f, single_edges);
			break;
		case 6:
			build_and_add_edge2(wet_label, Sl, El, p_f, p_grad_f, single_edges);
			break;
		case 12:
			build_and_add_edge2(wet_label, Sl, Ol, p_f, p_grad_f, single_edges);
			break;
		case 9:
			build_and_add_edge2(wet_label, Nl, Ol, p_f, p_grad_f, single_edges);
			break;
		default:
			std::cout<<"Cella bagnata ";
			wet_label.print();
			std::cout<<"confina con più di 2 celle asciutte.\n";
			valid_wet_cells = false;
		} // end switch
	} // end if
} // end for


	// Salvataggio lati singoli (solo per visualizzazione e debug)
	std::ofstream OutFile;

	OutFile.open("./data/domain/initial_edges", std::ios::out);
	for (std::map<label,IMMERSED_BOUNDARY::Edge>::iterator it=single_edges.begin(); it!=single_edges.end(); it++)
	{
		OutFile<< it->first(0) <<" " <<it->first(1) <<" " <<(it->second).first(0) << " " <<(it->second).first(1) << " " <<(it->second).second(0) << " " <<(it->second).second(1) <<std::endl;
	}
	OutFile.close();

	// errore nel caso di almeno una cella bagnata confinante con più di 2 celle asciutte
	if (!valid_wet_cells)
	{
		// è necessario salvare qui i vettori x e y per la visualizzazione con Matlab
		OutFile.open("./data/domain/x", std::ios::out);
		for (unsigned int i=0; i<Nx+3; i++)
		{
			OutFile<<x.at(i)<<std::endl;
		}
		OutFile.close();

		OutFile.open("./data/domain/y", std::ios::out);
		for (unsigned int i=0; i<Ny+3; i++)
		{
			OutFile<<y.at(i)<<std::endl;
		}
		OutFile.close();
		std::cout<<"ERRORE: almeno una cella bagnata confina con più di 2 celle asciutte. Consultare il file initiali edges.\n";
		throw ERRORS::errors(12);
	}

	// end if


// Creazione dominio: unione dei single edges

first_label = single_edges.begin()->first;
first_Vertex = &(single_edges.begin()->second.second);

N_single_edges = single_edges.size();
p_comp p_c = compare;
std::set<label,p_comp> considered_labels(p_c);
join_edges(first_label, first_Vertex, single_edges, N_single_edges, considered_labels);

// impostazione tolleranze reali
this->toll_x = toll_x;
this->toll_y = toll_y;


// Creazione del bordo

Point2d<double> right_vertex;

for (unsigned int i=1; i<=ccb.get_N_edges(); i++)
{
	right_vertex = ccb.get_edge_vertex(BOUNDARY::right, i);
	V_position(ccb.get_label(i), right_vertex);
	ccb.set_edge_vertex(BOUNDARY::right, right_vertex, i);
}

last_common_ctrs_part();



} // end constructor


void grid::build_and_add_edge(const label& wet_label, const label& dry_label, const bool& move_vertical, const double& toll, const IMMERSED_BOUNDARY::p_fun& p_f, const IMMERSED_BOUNDARY::p_grad_fun p_grad_f, std::map<label, IMMERSED_BOUNDARY::Edge, p_comp>& single_edges)
{
	Point2d<double> wet_center(cells[wet_label].get_center());
	Point2d<double> dry_center(cells[dry_label].get_center());
	double t;
	Point2d<double> zero;
	IMMERSED_BOUNDARY::Edge edge;

	t = IMMERSED_BOUNDARY::find_zero(wet_center,dry_center, p_f, move_vertical, toll);

	if (move_vertical)
	{
		zero(0) = wet_center(0);
		zero(1) = t;
	}
	else
	{
		zero(0) = t;
		zero(1) = wet_center(1);
	}

	if (is_point_in_cell(zero, wet_label))
	{
		edge= build_edge(zero, wet_label, p_f, p_grad_f);
		single_edges.insert(std::pair<label,IMMERSED_BOUNDARY::Edge>(wet_label,edge));
	}
	else
	{
		edge= build_edge(zero, dry_label, p_f, p_grad_f);
		single_edges.insert(std::pair<label,IMMERSED_BOUNDARY::Edge>(dry_label,edge));
	}
}


void grid::build_and_add_edge2(const label& wet_label, const label& vertical_dry_label, const label& orizontal_dry_label, const IMMERSED_BOUNDARY::p_fun& p_f, const IMMERSED_BOUNDARY::p_grad_fun& p_grad_f, std::map<label, IMMERSED_BOUNDARY::Edge, p_comp>& single_edges)
{
	Point2d<double> wet_center(cells[wet_label].get_center());
	Point2d<double> vertical_dry_center(cells[vertical_dry_label].get_center());
	Point2d<double> orizontal_dry_center(cells[orizontal_dry_label].get_center());
	double t;
	Point2d<double> vertical_zero, orizontal_zero;

	IMMERSED_BOUNDARY::Edge edge;

	t = IMMERSED_BOUNDARY::find_zero(wet_center,vertical_dry_center, p_f, true, 0.01*dy);
	vertical_zero(0) = wet_center(0);
	vertical_zero(1) = t;

	t = IMMERSED_BOUNDARY::find_zero(wet_center,orizontal_dry_center, p_f, false, 0.01*dx);
	orizontal_zero(0) = t;
	orizontal_zero(1) = wet_center(1);

	if (is_point_in_cell(vertical_zero, wet_label) && is_point_in_cell(orizontal_zero, wet_label))
	{
		//std::cout<<"Due zeri nella cella ";
		//wet_label.print();
		edge= build_edge(vertical_zero, wet_label, p_f, p_grad_f);
		single_edges.insert(std::pair<label,IMMERSED_BOUNDARY::Edge>(wet_label,edge));
	}
	else
	{
		if (is_point_in_cell(vertical_zero, wet_label))
		{
			edge= build_edge(vertical_zero, wet_label, p_f, p_grad_f);
			single_edges.insert(std::pair<label,IMMERSED_BOUNDARY::Edge>(wet_label,edge));
		}
		else
		{
			edge= build_edge(vertical_zero, vertical_dry_label, p_f, p_grad_f);
			single_edges.insert(std::pair<label,IMMERSED_BOUNDARY::Edge>(vertical_dry_label,edge));
		}

		if (is_point_in_cell(orizontal_zero, wet_label))
		{
			edge= build_edge(orizontal_zero, wet_label, p_f, p_grad_f);
			single_edges.insert(std::pair<label,IMMERSED_BOUNDARY::Edge>(wet_label,edge));
		}
		else
		{
			edge= build_edge(orizontal_zero, orizontal_dry_label, p_f, p_grad_f);
			single_edges.insert(std::pair<label,IMMERSED_BOUNDARY::Edge>(orizontal_dry_label,edge));
		}
	}
}

int grid::closest_vertex(const Point2d<double>& current_Vertex, const Point2d<double>& V1, const Point2d<double>& V2)
{
	if (std::max(fabs(V1(0)-current_Vertex(0)), fabs(V1(1)-current_Vertex(1))) < std::max(fabs(V2(0)-current_Vertex(0)), fabs(V2(1)-current_Vertex(1))))
		return 1;
	else return 2;
}
