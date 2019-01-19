/*
 * unknown.cpp
 *
 *  Created on: 18/gen/2012
 *      Author: ubuntu
 */

#include "unknown.hpp"

unknown::unknown(grid& g, const unsigned int& n_unknown, const p_comp& comp_function):
  Wu(g, n_unknown, comp_function),
  Gu(g, n_unknown, comp_function),
  N_unknown(n_unknown),
  G(g)
{}

unknown::unknown(const unknown& lhs) :
    Wu(lhs.Wu),
    Gu(lhs.Gu),
    N_unknown(lhs.N_unknown),
    G(lhs.G)
{}

double& unknown::operator ()(const unsigned int & wet_i, const unsigned int & wet_j)
{
  if (Wu.unknown_map.find(label(wet_i, wet_j)) != Wu.unknown_map.end())
  {
    return Wu.unknown_map[label(wet_i, wet_j)];
  }
  std::cout<< "ERRORE in unknown::operator(): la cella fornita non è bagnata. Non è stato ritornato nessun valore.\n";
  throw 1;
}

double& unknown::operator ()(const label& l)
{
  return operator ()(l(0), l(1));
}

const double& unknown::operator ()(const label& wc) const
{
  std::map<label, double>::const_iterator it = Wu.unknown_map.find(wc);
  if (it != Wu.unknown_map.end())
  {
    return it->second;
  }
  std::cout<< "ERRORE in unknown::operator(): la cella fornita non è bagnata. Non è stato ritornato nessun valore.\n";

  throw 1;
}

double& unknown::operator ()(const unsigned int& ghost_i, const unsigned int& ghost_j, const unsigned int& adj_wet_i, const unsigned int& adj_wet_j)
{

  label ghost(ghost_i, ghost_j);
  label adj_wet(adj_wet_i, adj_wet_j);


  if (Gu.unknown_map.find(ghost) == Gu.unknown_map.end())
  {
    std::cout<< "ERRORE in unknown::operator(): la cella fornita non è ghost. Non è stato ritornato nessun valore.\n";
  }
  else
  {
    std::vector<ghost_value>& gv = Gu.unknown_map[ghost];
    for (std::vector<ghost_value>::iterator it = gv.begin(); it != gv.end(); it++)
    {
      if (it->ass_wet_cell == adj_wet)
      {
        return it->value;
      }
    }

    std::cout<<"ERRORE in unknown::operator(): la ghost cell ";
    ghost.print();
    std::cout<<"non ha la cella adiacente bagnata ";
    adj_wet.print();
  }

  throw 1;
}

double& unknown::operator ()(const label& ghost, const label& adj_wet)
{
  return operator ()(ghost(0), ghost(1), adj_wet(0), adj_wet(1));
}

IMMERSED_BOUNDARY::stencil_values unknown::get_stencil_values(const label& wet_cell)
{
  double& u_c = (*this)(wet_cell);
  double& u_E = get_stencil_value(wet_cell, unknown_position::E, false);
  double& u_W = get_stencil_value(wet_cell, unknown_position::W, false);
  double& u_N = get_stencil_value(wet_cell, unknown_position::N, false);
  double& u_S = get_stencil_value(wet_cell, unknown_position::S, false);

  return IMMERSED_BOUNDARY::stencil_values(u_c, u_E, u_W, u_N, u_S);

}

void unknown::get_stencil_values(const label& wet_label, double& u_E, double& u_W, double& u_N, double& u_S)
{

  unsigned int lx = wet_label(0);
  unsigned int ly = wet_label(1);

  label E_label(lx+1, ly);
  label W_label(lx-1, ly);
  label N_label(lx, ly+1);
  label S_label(lx, ly-1);

  u_E = get_stencil_value(wet_label, unknown_position::E, true);
  u_W = get_stencil_value(wet_label, unknown_position::W, false);
  u_N = get_stencil_value(wet_label, unknown_position::N, false);
  u_S = get_stencil_value(wet_label, unknown_position::S, false);

}

double& unknown::get_stencil_value(const label& wet_label, const unknown_position::position& pos, const bool& wet_control)
{

  unsigned int lx = wet_label(0);
  unsigned int ly = wet_label(1);
  label adj_label;

  if (wet_control && (Wu.unknown_map.find(wet_label) == Wu.unknown_map.end()))
  {
    std::cout<< "ERRORE: la cella ";
    wet_label.print();
    std::cout<<"non è bagnata.\n";
  }
  else
  {
    switch(pos)
    {
    case unknown_position::E:
      adj_label.set_values(lx+1, ly);
      break;
    case unknown_position::W:
      adj_label.set_values(lx-1, ly);
      break;
    case unknown_position::N:
      adj_label.set_values(lx, ly+1);
      break;
    case unknown_position::S:
      adj_label.set_values(lx, ly-1);
      break;
    }
  }
    if (Wu.unknown_map.find(adj_label) == Wu.unknown_map.end())
      return (*this)(adj_label, wet_label);
    else
      return (*this)(adj_label);
}

void unknown::export_unknown()
{
  Wu.export_unknown(G, N_unknown);
}

void unknown::import_unknown()
{
  Wu.import_unknown(G, N_unknown);
  Gu.import_unknown(G, N_unknown);
}
