/*
 * grid_set_L_adj.cpp
 *
 *  Created on: 23/dic/2011
 *      Author: ubuntu
 */

#include "grid.hpp"

void grid::set_L_adj(const label& L, std::vector<label>& L_adj)
{
	const unsigned int& lx=L(0);
	const unsigned int& ly=L(1);
	L_adj.at(0)=label(lx,ly+1); // N
	L_adj.at(1)=label(lx+1,ly); // E
	L_adj.at(2)=label(lx,ly-1); // S
	L_adj.at(3)=label(lx-1,ly); // W
	//std::cout<<"end set L adj\n";
}

Point2d<double> grid::closest_corner(const label& L, const Point2d<double>& V)
{
  int d=1; // d=1: il minimo è SW, d=2: il minimo è SE, d=3: il minimo è NE, d=2: il minimo è NW,
  double xc,yc, dist, min_dist;
  // SW
  xc=x.at(L(0));
  yc=y.at(L(1));
  dist = std::max(fabs(xc-V(0)),fabs(yc-V(1)));
  min_dist=dist;
  // SE
  xc=x.at(L(0)+1);
  yc=y.at(L(1));
  dist=std::max(fabs(xc-V(0)),fabs(yc-V(1)));
  if (dist<min_dist)
  {
    min_dist=dist;
    d=2;
  }
  // NE
  xc=x.at(L(0)+1);
  yc=y.at(L(1)+1);
  dist=std::max(fabs(xc-V(0)),fabs(yc-V(1)));
  if (dist<min_dist)
  {
    min_dist=dist;
    d=3;
  }
  // NW
  xc=x.at(L(0));
  yc=y.at(L(1)+1);
  dist=std::max(fabs(xc-V(0)),fabs(yc-V(1)));
  if (dist<min_dist)
  {
    min_dist=dist;
    d=4;
  }
  switch (d)
  {
  case 1:
    return Point2d<double>(x.at(L(0)),y.at(L(1)));
  case 2:
    return Point2d<double>(x.at(L(0)+1),y.at(L(1)));
  case 3:
    return Point2d<double>(x.at(L(0)+1),y.at(L(1)+1));
  case 4:
    return Point2d<double>(x.at(L(0)),y.at(L(1)+1));
  }

  throw 1;
}
