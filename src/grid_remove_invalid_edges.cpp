/*
 * grid_remove_invalid_edges.cpp
 *
 *  Created on: 05/dic/2011
 *      Author: ubuntu
 */


#include "grid.hpp"

void grid::remove_invalid_edges(bool& all_valid)
{

unsigned int valid_counter, initial_N_edges, edge_counter;
label L;
Point2d<double> v;
std::vector<label> L_adj(4,label());
BOUNDARY::side v_extreme;

  valid_counter=1;
  initial_N_edges=ccb.get_N_edges();

  for (unsigned int i=1; i<=initial_N_edges; i++)
  {
    label l = ccb.get_label(valid_counter);
    Point2d<double> lv = ccb.get_edge_vertex(BOUNDARY::left, valid_counter);
    Point2d<double> rv = ccb.get_edge_vertex(BOUNDARY::right, valid_counter);
    bool valid = valid_edge(l, lv, rv, toll_x, toll_y, v_extreme);
    if (valid)
    {
      valid_counter++;
    }
    else
    {
      all_valid = false;
      (v_extreme == BOUNDARY::left) ? (ccb.remove_edge(valid_counter,BOUNDARY::right)) : (ccb.remove_edge(valid_counter,BOUNDARY::left));
    }
  }

  // 2) fusione di due lati consecutivi appartenenti alla stessa cella

  initial_N_edges=ccb.get_N_edges();
  valid_counter=1;

  for (unsigned int i=1; i<=initial_N_edges; i++)
  {
    if (ccb.get_label(valid_counter)==ccb.get_label(valid_counter+1))
    {
      all_valid=false;
      ccb.remove_edge(valid_counter,BOUNDARY::right);
    }
    else
    {
      valid_counter++;
    }
  }

  // 3) soddisfacimento proprietà celle adiacenti

  edge_counter=1;
  unsigned int edge_adj;
  // distanze "adiacente-corrente"
  int right_edge_dist, max_right_edge_dist;
  int left_edge_dist, max_left_edge_dist;
  unsigned int v_index;

  while(edge_counter<=ccb.get_N_edges())
  {
    max_left_edge_dist=0;
    max_right_edge_dist=0;
    L=ccb.get_label(edge_counter);
    set_L_adj(L,L_adj);
    for (int li=0; li<4; li++)
    {
      if (ccb.get_n_edge(L_adj.at(li))!=-1)
      {
        edge_adj=ccb.get_n_edge(L_adj.at(li));

        if (edge_counter<edge_adj)
          {right_edge_dist = edge_adj-edge_counter;}
        else
          {right_edge_dist = edge_adj-edge_counter+ccb.get_N_edges();}

        left_edge_dist = ccb.get_N_edges()-right_edge_dist;

        if (right_edge_dist<left_edge_dist)
          {max_right_edge_dist=std::max(max_right_edge_dist,right_edge_dist);}
        else
          {max_left_edge_dist=std::max(max_left_edge_dist,left_edge_dist);}
      }
    }
    if (max_right_edge_dist>max_left_edge_dist && max_right_edge_dist>1)
    {
      all_valid = false;

      v_index=edge_counter+max_right_edge_dist;
      if (v_index > ccb.get_N_edges())
                        {
                          v_index-=ccb.get_N_edges();
                        }
      v=ccb.get_edge_vertex(BOUNDARY::left, v_index);
      v=closest_corner(L,v);
      ccb.set_edge_vertex(BOUNDARY::left,v,v_index);
      ccb.set_edge_vertex(BOUNDARY::right,v,edge_counter);
      for (int i=0; i<max_right_edge_dist-1; i++)
      {
                          ccb.remove_edge(edge_counter+1,BOUNDARY::right);
      }
    }
  edge_counter++;
  } // end while (edge_counter<=ccb.get_N_edges())

} // end method
