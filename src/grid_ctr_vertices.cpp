/*
 * grid_ctr_vertices.cpp
 *
 *  Created on: 23/dic/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

grid::grid(const std::string& input_f, const std::string& vertices_f, const p_comp& comp_function):
    if_parameters(input_f.c_str()), if_vertices(vertices_f.c_str()),
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

  Point2d<double> P,v;
  std::pair<label,Point2d<double> >* V;
  std::pair<label,Point2d<double> >* V_n; //considered vertex and next vertex

  label Key, L;
  std::vector<label> L_adj(4,label());
  cell C(N_equations);
  std::map<label,Point2d<double> >::iterator it;
  std::vector<unsigned int>::iterator itui;
  std::list<std::pair<Point2d<double>, label> >::iterator itl, nitl;

  double xv,yv;

  unsigned int N_vertices = if_vertices("Number_of_vertices",0);
  std::vector<std::pair<label, Point2d<double>>> vertices(N_vertices+1);

  const double k = if_parameters("grid_parameters/k",0.25);

  const double x_min = if_parameters("grid_parameters/x_min",0.0);
  const double x_max = if_parameters("grid_parameters/x_max",0.0);
  const double y_min = if_parameters("grid_parameters/y_min",0.0);

  // acquisizione vertici del dominio originale da file
  for(unsigned int i=0; i<N_vertices; i++)
  {
    xv = if_vertices("vertices", 0.0, 2*i);
    yv = if_vertices("vertices", 0.0, 2*i+1);
    (vertices.at(i)).second = Point2d<double>(xv,yv);
  }
  vertices.at(N_vertices).second=vertices.at(0).second; // ultimo vertice coincide col primo

  // Scrittura su file importabile da matlab del dominio reale
  writeout_real_domain(vertices);

  dx = (x_max-x_min) / Nx;
  dy = dx;

  toll_x = k*dx;
  toll_y = k*dy;

  //inizializzazione vettori x e y (griglia)
  for(std::size_t nx=0; nx < Nx + 3; nx++)
  {
    x.at(nx)=x_min - dx + nx*dx;
  }

  for(std::size_t ny=0; ny < Ny + 3; ny++)
  {
    y.at(ny)=y_min - dy + ny*dy;
  }

  for (unsigned int iv=0; iv < N_vertices; iv++)
  {
    Key = IMMERSED_BOUNDARY::find_label(x,y,(vertices.at(iv)).second);
    (vertices.at(iv)).first = Key;
  }
  vertices.at(N_vertices).first=vertices.at(0).first;

  // inizializzazione centri cella
  for(unsigned int nx=0; nx<Nx+2; nx++)
  {
    for(unsigned int ny=0; ny<Ny+2; ny++)
    {
      Key.set_values(nx,ny);
      C.set_center(x_min+0.5*dx+(nx-1)*dx,y_min+0.5*dy+(ny-1)*dy);
      cells.insert(std::pair<label,cell>(Key,C));
    }
  }

  // riposizionamento di tutti i vertici originali vicini ai bordi cella
  for(unsigned int iv=0; iv<N_vertices+1; iv++)
  {
  V = &vertices.at(iv);
  V_position(V->first, V->second);
  }

  // generazione dei lati

  for (unsigned int iv=0; iv<N_vertices; iv++)
  {
    V = &vertices.at(iv);
    V_n = &vertices.at(iv+1);

    if (!((*V).first == (*V_n).first)) // se i vertici non sono nella stessa cella
    {
      cut_cells(V->first, V->second, V_n->first, V_n->second, toll_x, toll_y, false);
    }
  }

  last_common_ctrs_part();

} // end constructor
