/*
 * grid.cpp
 *
 *  Created on: 08/nov/2011
 *      Author: ubuntu
 */

#include "grid.hpp"
#include "immersed_boundary.hpp"


// destructor
grid::~grid()
{}

void grid::print_cells()
{
	using namespace std;

	map<label,cell>::iterator it;
	for (it= cells.begin(); it!=cells.end(); it++)
	{
		cout<<"Cell label:" <<endl;
		(it->first).print();
		cout<<endl <<"Cell:" <<endl;
		(it->second).print();
	}
}



void grid::print_cutting_edges()
{
	ccb.print_edges();
}


void grid::info()
{
	using namespace std;
	cout <<"########## Global info ##########" <<endl;
	cout <<"Total number of x cells: " <<Nx <<"+2; " <<"Number of y cells: " <<Ny <<"+2; "  <<endl;
	cout <<"Total rectangular domain: (" <<x.at(0) <<"," <<x.at(Nx+2) <<") X ("<<y.at(0) <<"," <<y.at(Ny+2) <<")" <<endl;
	cout <<"dx= " <<dx <<"; dy= " <<dy <<endl;
	cout<<"Number of edges: " <<get_N_edges() <<endl;
	cout<<"Number of wet cells: " <<get_N_wet_cells() <<endl;
	cout<<"Number of ghost cells: " <<get_N_ghost_cells() <<endl;
	cout <<"#################################" <<endl;
}

void grid::set_all_dry()
{

	for (std::map<label,cell>::iterator it=cells.begin(); it!=cells.end(); it++)
	{
	it->second.set_type(CELL::dry);
	}
}

void grid::set_all_not_cut()
{

	for (std::map<label,cell>::iterator it=cells.begin(); it!=cells.end(); it++)
	{
	it->second.set_boundary(CELL::not_cut);
	}
}

bool grid::is_point_in_cell(const Point2d<double>& P, const label& L)
{
	Point2d<double> center=cells[L].get_center();

	if ( (fabs(P(0)-center(0))<0.5*dx) && (fabs(P(1)-center(1))<0.5*dy) )
		return true;
	else return false;
}

void grid::print_stencil_values(const label& computational_cell, const unsigned int& n_unknown)
{
	using namespace std;
	double central_value;
	double E_v, W_v, N_v, S_v;

	get_stencil_values(computational_cell, n_unknown, E_v, W_v, N_v, S_v);
	central_value = get_unknown_value(computational_cell, n_unknown);

	cout<<"#########################" <<endl;
	cout<<"# Wet cell: ";
	computational_cell.print();
	cout<<"# Values for unknown number " <<n_unknown <<endl;
	cout<<"# central value: " <<central_value <<endl;
	cout<<"# E value: " <<E_v <<endl;
	cout<<"# W value: " <<W_v <<endl;
	cout<<"# N value: " <<N_v <<endl;
	cout<<"# S value: " <<S_v <<endl;
	cout<<"#########################" <<endl;

}
