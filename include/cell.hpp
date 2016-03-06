/*
 * cell.hpp
 *
 *  Created on: 07/nov/2011
 *      Author: Simone Rinco
 */

#ifndef CELL_HPP_
#define CELL_HPP_

#include <iostream>
#include "Point2d.hpp"
#include "Point2d_def.hpp"
#include "immersed_boundary.hpp"


/*
 * class cell crea oggetti adatti a rappresentare una cella per l'applicazione dei metodi immersed boundary.
 * Ogni cella si caratterizza per il tipo (wet or dry) a seconda del fatto che il centro cella sia interno (wet)
 * o esterno al dominio e per il bordo (cut or not_cut) a seconda del fatto che la cella sia tagliata da un lato
 * o meno.
 * Il membro unknowns contiene il vettore con i valori delle incognite.
 *
 * class ghost cell eredita pubblicamente da cell e crea oggetti adatti a rappresentare le ghost cell (celle asciutte
 * che confinano con almeno una cella bagnata). Di conseguenza è necessario salvare in un vettore di dimensioni
 * pari al numero di celle bagnate confinanti tutte le informazioni necessarie, riassunte in oggetti di tipo
 * ghost_quantities.
 *
 */

namespace CELL
{
	enum type {wet, dry};
	enum boundary {cut, not_cut};
}

namespace GHOST_CELL
{
	enum interp_matrix_type {all_wet, GPs, noGP};

	// all_wet: tutti i vertici del quadrato di interpolazione sono bagnati: unica matrice 4x4 per ogni variabile
	// GPs: almeno un vertice è un GP. Ogni vertice è bagnato o è un GP: una matrice 8x8 per Qx,Qy e una matrice 4x4 per Z
	// noGP: un vertice non è nè bagnato nè GP: una matrice 6x6 per Qx,Qy e una matrice 3x3 per Z
}

struct ghost_quantities
{
	Point2d<double> RP; // punto riflesso
	Point2d<double> BP; // punto sul bordo
	Point2d<double> normal; // versore normale uscente al lato considerato
	IMMERSED_BOUNDARY::label associated_WC; // cella bagnata associata al BP
	IMMERSED_BOUNDARY::label SW_corner; // cella il cui centro è il vertice SW del quadrato di interpolazione
	unsigned int number_ghost_corners; // numero di ghost points nel quadrato di interpolazione
	unsigned int edge_number; // numero del lato associato al BP

	std::vector<Point2d<double> > RP_and_GP_unknowns;

};

class cell{

protected:
	Point2d<double> center;
	CELL::type type;
	CELL::boundary boundary;
	unsigned int cutting_edge_number;
	unsigned int N_unknowns;
	std::vector<double> unknowns;

public:
	cell();
	cell(const unsigned int& N_unknowns);
	cell(const Point2d<double>& center, const CELL::type&, const CELL::boundary&, const unsigned int& edge_number, const unsigned int& N_unknowns);
	cell(const double& x_center, const double& y_center);
	cell(const cell&);
	virtual ~cell();

	cell& operator=(const cell&);
	virtual void print();

	// getters
	Point2d<double> get_center();
	Point2d<double> get_center() const;
	CELL::type get_type() const;
	CELL::boundary get_boundary();
	unsigned int get_cutting_edge_number();
	double get_unknown(const unsigned int& N_unknown);
	std::vector<double> get_unknowns();

	// setters
	void set_center(const Point2d<double>&);
	void set_center(const double&, const double&);
	void set_type(const CELL::type&);
	void set_boundary(const CELL::boundary&);
	void set_cutting_edge_number(const unsigned int& cen);
	void set_unknown(const unsigned int& N_unknown, const double& value);
	void set_unknowns(const std::vector<double>& values);

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ghost_cell : public cell
{
	unsigned int number_adj_wet_cells;
	std::vector<ghost_quantities> GQs;


public:
	ghost_cell();

	ghost_cell& operator=(const ghost_cell&);

	//getters
	unsigned int get_number_of_adj_wet_cells() const;
	std::vector<ghost_quantities> get_GQs() const;

	double get_RP_unknown(const unsigned int& RP_number, const unsigned int& unknown_number);
	double get_GP_unknown(const unsigned int& GP_number, const unsigned int& unknown_number);
	int get_associated_GP(const IMMERSED_BOUNDARY::label& adj_cell);

	// setters
	void set_GQs(const std::vector<ghost_quantities>& gqs);
	void set_RP_unknown(const unsigned int& RP_number, const unsigned int& unknown_number, const double& unknown_value);
	void set_GP_unknown(const unsigned int& GP_number, const unsigned int& unknown_number, const double& unknown_value);

};

#endif /* CELL_HPP_ */
