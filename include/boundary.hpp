/*
 * boundary.hpp
 *
 *  Created on: 17/nov/2011
 *      Author: Simone Rinco
 */

#ifndef BOUNDARY_HPP_
#define BOUNDARY_HPP_

#include <iostream>
#include <list>
#include "Point2d.hpp"
#include "immersed_boundary.hpp"
#include "errors.hpp"

/*
 * class boundary rappresenta un dominio poligonale inserito in una griglia cartesiana. Ogni lato è contenuto in
 * una e una sola cella e ogni cella può contenere al massimo un lato. Di conseguenza i vertici di ogni lato
 * cadono sul bordo della cella. Ogni lato possiede una label che rappresenta la cella nella quale è contenuto.
 * I lati sono ordianati in modo che il vertice chiamato "destro" del lato n sia coincidente col vertice "sinistro"
 * del lato n+1, ad eccezione del vertice destro dell'ultimo lato che coincide col vertice sinistro del primo.
 *
 * È previsto il solo costruttore di default che crea un dominio vuoto (numero di lati = 0).
 *
 * I membri pubblici permettono di aggiungere un lato (in coda, modificando di conseguenza il solo primo lato del dominio originale),
 * rimuovere un lato, spostare i vertici, rinominare e assegnare etichette ai lati, stampare a video uno o tutti i lati.
 *
 */

namespace BOUNDARY
{
	enum side{left,right};
}

class boundary
{
	std::list<std::pair<Point2d<double>,IMMERSED_BOUNDARY::label> > vertices_and_labels;
	unsigned int N_edges;

public:

	boundary();
	boundary(const boundary&);

	// getters
	Point2d<double> get_edge_vertex(const BOUNDARY::side& s, unsigned int n_edge) const;
	Point2d<double>& get_last_vertex();
	IMMERSED_BOUNDARY::label& get_label(const unsigned int& n_edge);
	int get_n_edge(const IMMERSED_BOUNDARY::label& l);
	unsigned int get_N_edges();

	// setters
	void set_edge_vertex(const BOUNDARY::side& s, const Point2d<double>& vertex, unsigned int n_edge);
	void set_label(const IMMERSED_BOUNDARY::label& L, const unsigned int& n_edge);

	// other methods
	void add_edge(const Point2d<double>& right_vertex, const IMMERSED_BOUNDARY::label& l);
	void add_edge(const double& x_right_vertex, const double& y_right_vertex, const IMMERSED_BOUNDARY::label& l);
	void remove_edge(unsigned int n_edge, const BOUNDARY::side& s);
	void print_edge(const unsigned int& edge_number);
	void print_edges();
};

#endif /* BOUNDARY_HPP_ */
