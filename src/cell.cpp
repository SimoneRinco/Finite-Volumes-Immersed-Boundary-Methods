/*
 * cell.cpp
 *
 *  Created on: 08/nov/2011
 *      Author: ubuntu
 */

#include "cell.hpp"

cell::cell(): center(0.0,0.0), type(CELL::dry), boundary(CELL::not_cut), cutting_edge_number(0), N_unknowns(0) {}
cell::cell(const unsigned int& N_unknowns): center(0.0,0.0), type(CELL::dry), boundary(CELL::not_cut), cutting_edge_number(0), N_unknowns(N_unknowns), unknowns(N_unknowns, 0.0) {}
cell::cell( const Point2d<double>& center, const CELL::type& type, const CELL::boundary& boundary, const unsigned int& cutting_edge_number, const unsigned int& N_unknowns):
    center(center),
    type(type),
    boundary(boundary),
    cutting_edge_number(cutting_edge_number),
    N_unknowns(N_unknowns),
    unknowns(N_unknowns, 0.0)

{}
cell::cell(const double& x_center, const double& y_center): center(x_center,y_center), type(CELL::dry), boundary(CELL::not_cut), cutting_edge_number(0){}
cell::cell(const cell& lhs): center(lhs.center), type(lhs.type), boundary(lhs.boundary), cutting_edge_number(lhs.cutting_edge_number), N_unknowns(lhs.N_unknowns), unknowns(lhs.unknowns){}
cell::~cell(){}


cell& cell::operator=(const cell& rhs)
{
  center = rhs.center;
  type = rhs.type;
  boundary = rhs.boundary;
  cutting_edge_number = rhs.cutting_edge_number;
  N_unknowns = rhs.N_unknowns;
  unknowns = rhs.unknowns;
  return *this;
}



void cell::print()
{
  std::cout<<"Cell data" <<std::endl;
  std::cout<<"center: ";
  center.print();

  if (type == CELL::dry)
    std::cout<<"; dry cell";
  else if (type == CELL::wet)
    std::cout<<"; wet cell";

  if (boundary == CELL::cut)
  {
    std::cout<<"; cell cutted by edge number " <<cutting_edge_number <<std::endl;
  }
  else if (boundary == CELL::not_cut)
    std::cout<<"; cell not cutted";
  std::cout<<std::endl;

  std::cout<<"Number of unknowns: " <<N_unknowns <<std::endl;
  std::cout<<"values:\n";
  for (unsigned int i=0; i<N_unknowns; i++)
  {
    std::cout<<unknowns.at(i) <<" \n";
  }


}

//getters
double cell::get_unknown(const unsigned int& N_unknown)
{
  return unknowns.at(N_unknown-1);
}

CELL::type cell::get_type() const
{
  return type;
}
CELL::boundary cell::get_boundary()
{
  return boundary;
}

Point2d<double> cell::get_center()
{
  return center;
}

Point2d<double> cell::get_center() const
{
  return center;
}

unsigned int cell::get_cutting_edge_number()
{
  return cutting_edge_number;
}

// setters

void cell::set_unknown(const unsigned int& N_unknown, const double& value)
{
  unknowns.at(N_unknown-1) = value;
}

void cell::set_unknowns(const std::vector<double>& new_unknowns)
{
  if (new_unknowns.size()!= N_unknowns)
  {
    throw ERRORS::errors(400);
  }
  else
  {
    unknowns = new_unknowns;
  }
}


void cell::set_center(const Point2d<double>& P)
{
  center=P;
}
void cell::set_center(const double& first, const double& second)
{
  center.set_values(first,second);
}

void cell::set_cutting_edge_number(const unsigned int& cen)
{
  cutting_edge_number=cen;
}

void cell::set_type(const CELL::type& t)
{
  type=t;
}

void cell::set_boundary(const CELL::boundary& b)
{
  boundary = b;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

ghost_cell::ghost_cell(): number_adj_wet_cells(0){}

ghost_cell& ghost_cell::operator=(const ghost_cell& rhs)
{
  cell::operator=(rhs);
  number_adj_wet_cells = rhs.number_adj_wet_cells;
  GQs = rhs.GQs;
  return *this;
}

void ghost_cell::set_GQs(const std::vector<ghost_quantities>& gqs)
{
  GQs = gqs;
}

void ghost_cell::set_RP_unknown(const unsigned int& RP_number, const unsigned int& unknown_number, const double& unknown_value)
{
  (GQs.at(RP_number).RP_and_GP_unknowns).at(unknown_number-1)(0) = unknown_value;
}

void ghost_cell::set_GP_unknown(const unsigned int& GP_number, const unsigned int& unknown_number, const double& unknown_value)
{
  (GQs.at(GP_number).RP_and_GP_unknowns).at(unknown_number-1)(1) = unknown_value;
}

double ghost_cell::get_RP_unknown(const unsigned int& RP_number, const unsigned int& unknown_number)
{
  return (GQs.at(RP_number).RP_and_GP_unknowns).at(unknown_number-1)(0);
}

double ghost_cell::get_GP_unknown(const unsigned int& GP_number, const unsigned int& unknown_number)
{
  //std::cout <<"GQs size: " <<GQs.size() <<", GP_number: " <<GP_number <<", unknown number: " <<unknown_number <<"\n";
  return (GQs.at(GP_number).RP_and_GP_unknowns).at(unknown_number-1)(1);
}


unsigned int ghost_cell::get_number_of_adj_wet_cells() const
{
  return GQs.size();
}


std::vector<ghost_quantities> ghost_cell::get_GQs() const
{
  return GQs;
}

int ghost_cell::get_associated_GP(const IMMERSED_BOUNDARY::label& adj_cell)
{
  int associated_GP_number = -1;
  std::size_t cicle_counter = 0;
  bool exit = false;

  while (exit == false && cicle_counter < GQs.size())
  {
    if (GQs.at(cicle_counter).associated_WC == adj_cell)
    {
      associated_GP_number = cicle_counter;
      exit = true;
    }
    else
    {
      cicle_counter++;
    }
  }

  return associated_GP_number;
}
