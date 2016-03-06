/*
 * boundary.cpp
 *
 *  Created on: 17/nov/2011
 *      Author: ubuntu
 */

#include "boundary.hpp"


boundary::boundary(): N_edges(0){}
boundary::boundary(const boundary& lhs): vertices_and_labels(lhs.vertices_and_labels), N_edges(lhs.N_edges){}

// getters


Point2d<double> boundary::get_edge_vertex(const BOUNDARY::side& s, unsigned int n_edge) const
{
	std::list<std::pair<Point2d<double>, IMMERSED_BOUNDARY::label> >::const_iterator it=vertices_and_labels.begin();

	if (N_edges==0) throw ERRORS::errors(1);
	if (n_edge<1 || n_edge>N_edges)
		{
		std::cout<<"ERRORE in boundary::get_edge_vertex. n_edge = " <<n_edge <<std::endl;
		std::cout<<"Numero di lati: " <<N_edges <<std::endl;
		throw ERRORS::errors(2);
		}

	if (s==BOUNDARY::right)
	{
		std::advance(it,n_edge-1);
		return it->first;
	}
	else
	{
		if(n_edge==1)
		{
			std::advance(it,N_edges-1);
			return it->first;
		}
		else
		{
			std::advance(it,n_edge-2);
			return it->first;
		}
	}
}


Point2d<double>& boundary::get_last_vertex()
{
	// Restituisce una referenza al vertice destro dell'ultimo lato

	return vertices_and_labels.rbegin()->first;
}


IMMERSED_BOUNDARY::label& boundary::get_label(const unsigned int& n_edge)
{
	// Ritorna l'etichetta associata al lato n_edge. Il lato N_edges+1 è considerato in questo metodo
	// uguale al primo lato.

	std::list<std::pair<Point2d<double>, IMMERSED_BOUNDARY::label> >::iterator it=vertices_and_labels.begin();
	if (n_edge==1 || n_edge==N_edges+1)
	{
		std::advance(it, N_edges-1);
		return it->second;
	}
	else if(n_edge<=N_edges)
	{
		std::advance(it, n_edge-2);
		return it->second;
	}
	else throw ERRORS::errors(3);
}

 int boundary::get_n_edge(const IMMERSED_BOUNDARY::label& l)
{
	 // Ritorna il numero del lato associato all'etichetta l. Se l non è presente viene ritornato -1.

	int n_edge=-1;
	int counter=0;
	std::list<std::pair<Point2d<double>, IMMERSED_BOUNDARY::label> >::iterator it=vertices_and_labels.begin();
	if (vertices_and_labels.rbegin()->second==l)
		{
		n_edge=1;
		return n_edge;
		}
	while (it!=vertices_and_labels.end())
	{
		if ((it->second)==l)
		{
			n_edge = counter+2;
			return n_edge;
		}
		else
		{
			counter++;
			it++;
		}
	}

	return n_edge;

}

 unsigned int boundary::get_N_edges()
 {
	 // Ritorna il numero di lati del dominio.

	 return N_edges;
 }

 // setters

void boundary::set_edge_vertex(const BOUNDARY::side& s, const Point2d<double>& vertex, unsigned int n_edge)
{
	std::list<std::pair<Point2d<double>, IMMERSED_BOUNDARY::label> >::iterator it=vertices_and_labels.begin();

	if (n_edge <1 || n_edge > N_edges)
	{
		throw ERRORS::errors(4);
	}
	else
	{
	if (s==BOUNDARY::right)
		{
		std::advance(it,n_edge-1);
		it->first = vertex;
		}
	else
	{
		if (n_edge==1)
		{
		std::advance(it,N_edges-1);
		it->first =vertex;
		}
		else
		{
		std::advance(it,n_edge-2);
		it->first =vertex;
		}
	}
	}
}

void boundary::set_label(const IMMERSED_BOUNDARY::label& L, const unsigned int& n_edge)
{
	std::list<std::pair<Point2d<double>, IMMERSED_BOUNDARY::label> >::iterator it=vertices_and_labels.begin();
	if (n_edge<1 || n_edge>N_edges)
		throw ERRORS::errors(5);
	else
	{
		if (n_edge==1)
		{
			vertices_and_labels.rbegin()->second=L;
		}
		else
		{
			std::advance(it,n_edge-2);
			it->second=L;
		}
	}

}

// other methods

void boundary::add_edge(const Point2d<double>& last_vertex, const IMMERSED_BOUNDARY::label& l)
{
	// aggiunge un nuovo lato con l'etichietta l inserendo un nuovo vertice in coda al vettore dei vertici.
	// Di conseguenza viene modificato il vertice sinistro del primo lato. Gli altri lati rimangono inalterati.

	if (N_edges==0)
	{
		vertices_and_labels.push_back(std::pair<Point2d<double>, IMMERSED_BOUNDARY::label>(last_vertex,l));
	}
	else
	{
	IMMERSED_BOUNDARY::label first_label=vertices_and_labels.rbegin()->second;
	vertices_and_labels.rbegin()->second = l;
	vertices_and_labels.push_back(std::pair<Point2d<double>, IMMERSED_BOUNDARY::label>(last_vertex,first_label));
	}
	N_edges++;

}

void boundary::add_edge(const double& x_right_vertex, const double& y_right_vertex, const IMMERSED_BOUNDARY::label& l)
{
	Point2d<double> nv(x_right_vertex, y_right_vertex);
	add_edge(nv, l);
}


void boundary::remove_edge(unsigned int n_edge, const BOUNDARY::side& s)
{
	// Rimuove un lato: l'etichietta corrispondente al lato da rimuovere viene persa, i lati vengono
	// uniti eliminando il vertice sinistro o destro del lato da rimuovere.

	IMMERSED_BOUNDARY::label l;
	std::list<std::pair<Point2d<double>, IMMERSED_BOUNDARY::label> >::iterator it=vertices_and_labels.begin();

	if (n_edge==N_edges+1)
		n_edge=1;

	else if (n_edge<1 || n_edge > N_edges+1)
		std::cout<<"Errore: il poligono non ha il lato " <<n_edge <<".\n";
	else
	{
	if (n_edge==1)
		{
			if (s==BOUNDARY::right)
			{
				l=it->second;
				vertices_and_labels.erase(it);
				vertices_and_labels.rbegin()->second=l;
			}
			else
			{
				std::advance(it,N_edges-1);
				vertices_and_labels.erase(it);
			}

		}
		else
		{
		if (s==BOUNDARY::right)
			{
				std::advance(it,n_edge-1);
				l=it->second;
				it=vertices_and_labels.erase(it);
				it--;
				it->second=l;
			}
			else
			{
			std::advance(it,n_edge-2);
			vertices_and_labels.erase(it);
			}
		}
	N_edges--;
	}
}

void boundary::print_edge(const unsigned int& edge_number)
{
	if (edge_number>0 && edge_number<=N_edges)
	{
		std::cout<<"Edge number " <<edge_number <<":\n";
		get_edge_vertex(BOUNDARY::left,edge_number).print();
		get_edge_vertex(BOUNDARY::right,edge_number).print();
		std::cout<<"The edge is in cell:\n";
		get_label(edge_number).print();
	}
	else
	{
		std::cout<<"Non esiste il lato " <<edge_number <<"\n";
	}
}

void boundary::print_edges()
{
	std::cout<<"### Number of edges: " <<N_edges <<"\n";

	for (int i=1; i<=N_edges; i++)
	{
		print_edge(i);
	}
}




