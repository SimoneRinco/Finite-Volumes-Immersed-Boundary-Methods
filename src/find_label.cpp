/*
 * find_label.cpp
 *
 *  Created on: 14/nov/2011
 *      Author: ubuntu
 */

#include "immersed_boundary.hpp"

IMMERSED_BOUNDARY::label IMMERSED_BOUNDARY::find_label(const std::vector<double>& x, const std::vector<double>& y, const Point2d<double>& P)
{
	unsigned int i=0;
	unsigned int j=0;

	while (P(0)>x.at(i))
	{
		i++;
		if (i>= x.size())
		{
			std::cout<< "Errore: vertice ";
			P.print();
			std::cout<< "fuori dai limiti indicati\n";
		}
	}
	while (P(1)>y.at(j))
	{
		j++;
		if (j>= y.size())
		{
			std::cout<< "Errore: vertice ";
			P.print();
			std::cout<< "fuori dai limiti indicati\n";
		}
	}

	return label(i-1,j-1);


}

