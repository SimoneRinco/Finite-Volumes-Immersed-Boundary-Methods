/*
 * wet_unknown.cpp
 *
 *  Created on: 18/gen/2012
 *      Author: ubuntu
 */

#include "wet_unknown.hpp"

wet_unknown::wet_unknown(grid& g, const unsigned int& n_unknown, const p_comp& comp_function) :
	unknown_map(comp_function)
{

	double u_value;
	std::vector<label> wcl = g.get_wet_cells();

	if (n_unknown < 1 || n_unknown > g.get_N_equations())
	{
		std::cout<< "ERRORE in wet_unknown constructor: numero dell'incognita fornito errato.\n";
	}
	else
	{
		for (std::vector<label>::iterator it = wcl.begin(); it != wcl.end(); it++)
		{
			u_value = g.get_unknown_value(*it, n_unknown);
			unknown_map.insert(std::pair<label, double>(*it, u_value));
		}
	}
} // end constructor

wet_unknown::wet_unknown(const wet_unknown& lhs) : unknown_map(lhs.unknown_map){}

double& wet_unknown::operator ()(const unsigned int& i, const unsigned int& j)
{
  if (unknown_map.find(label(i,j)) == unknown_map.end())
  {
    std::cout<< "ERRORE in wet_unknown::operator(): la cella fornita non è bagnata. Non è stato ritornato nessun valore.\n";
    throw 1;
  }

  return unknown_map[label(i,j)];
}

double& wet_unknown::operator ()(const label& l)
{
	return operator ()(l(0), l(1));
}

void wet_unknown::export_unknown(grid& g, const unsigned int& n_unknown)
{
	for (std::map<label, double>::iterator it = unknown_map.begin(); it != unknown_map.end(); it++)
	{
		g.set_unknown_values(it->first, n_unknown, it->second);
	}
}
void wet_unknown::import_unknown(grid & g, const unsigned int& n_unknown)
{
	for (std::map<label, double>::iterator it = unknown_map.begin(); it != unknown_map.end(); it++)
	{
		it->second = g.get_unknown_value(it->first, n_unknown);
	}
}
