#include "grid.hpp"
#include "unknown.hpp"

double sincos(const Point2d<double>& P) {return 1.0 + 0.2*sin(P(0))*cos(P(1));}

int main(int argc, char **argv) {

	using namespace std;
	using namespace IMMERSED_BOUNDARY;

	grid G;
	unknown u(G,1);
	const vector<label>& WC(G.get_wet_cells()); // wet cells
	label a_wet_label(*WC.begin());
	double u_central, u_E, u_W, u_N, u_S;

	G.set_unknown_values(sincos, 1);

	G.boundary_conditions();

	// importo da G il valore della prima incognita
	// (valori nelle wet cells e nelle ghost cells)
	u.import_unknown();

	// importo valori dell'incognita nella cella centrale
	// e nelle quattro celle adiacenti
	u_central = u(a_wet_label);
	u.get_stencil_values(a_wet_label, u_E, u_W, u_N, u_S);

	// importo PER REFERENZA i valori dell'incognita nella cella centrale
	// e nelle quattro celle adiacenti
	stencil_values u_sv_ref = u.get_stencil_values(a_wet_label);

	// importo solo il valore EST per valore
	u_E = u.get_stencil_value(a_wet_label, unknown_position::E, false);

	// importo solo il valore NORD per referenza con controllo cella bagnata
	double& u_N_ref = u.get_stencil_value(a_wet_label, unknown_position::N,true);


	// assegno il valore 1.0 ad u in tutto il dominio
	for (vector<label>::const_iterator it = WC.begin(); it != WC.end(); it++)
	{
		u(*it) = 1.0;
	}

	// esporto i valori di u in G (prima incognita) nelle sole celle bagnate
	u.export_unknown();
}




