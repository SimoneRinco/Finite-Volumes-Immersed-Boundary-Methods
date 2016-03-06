#include "grid.hpp"

double sincos(const Point2d<double>& P) {return 1.0 + 0.2*sin(P(0))*cos(P(1));}
double xsquare(const Point2d<double>& P) {return P(0)*P(0);}

int main(int argc, char **argv) {

	using namespace std;
	using namespace IMMERSED_BOUNDARY;

	grid G;
	const vector<label>& wet_cells(G.get_wet_cells());
	const double dx = G.get_dx(), dy = G.get_dy();
	const unsigned int Nx = G.get_Nx(), Ny = G.get_Ny();
	double z, z_E, z_W, z_N, z_S;

	// stampa a video e salvataggio su file informazioni relative al dominio
	G.info();
	G.writeout_domain();

	// impostare i valori alle incognite tramite funzione
	G.set_unknown_values(sincos, 1);

	// impostare i valori alle incognite tramite ciclo sulle wet cells
	for (vector<label>::const_iterator it=wet_cells.begin(); \
		it!=wet_cells.end(); it++)
	{
		G.set_unknown_values(*it, 2, xsquare(G.get_cell_center(*it)));
	}

	// impostare i valori tramite ciclo su tutte le celle (sconsigliato)
	for (unsigned int i=0; i<Nx+2; i++)
	{
		for (unsigned int j=0; j<Ny+2; j++)
		{
			if (G.is_wet(i,j))
				G.set_unknown_values(label(i,j), 3, 4.5);
		}
	}

	// applicazione delle condizioni al contorno
	// ==> calcolo valori incognite nei ghost points
	G.boundary_conditions();

	// estrarre i valori delle incognite
	z = G.get_unknown_value(*wet_cells.begin(), 1);
	G.get_stencil_values(*wet_cells.begin(), 1, z_E, z_W, z_N, z_S);

	// salvare su file i valori delle incognite
	G.writeout_unknowns("./data/unknowns/the_unknowns");

	return 0;
}
