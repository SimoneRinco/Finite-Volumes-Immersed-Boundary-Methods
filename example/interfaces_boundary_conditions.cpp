#include "finite_volume.hpp"

void FINITE_VOLUME::interfaces_boundary_conditions(grid& G, cell_unknown& z, cell_unknown& Qx, cell_unknown& Qy)
{
// applicazione delle condizioni al contorno per i valori di interfaccia

interfaces_boundary_conditions(G, z.E, Qx.E, Qy.E);
interfaces_boundary_conditions(G, z.W, Qx.W, Qy.W);
interfaces_boundary_conditions(G, z.N, Qx.N, Qy.N);
interfaces_boundary_conditions(G, z.S, Qx.S, Qy.S);

}

void FINITE_VOLUME::interfaces_boundary_conditions(grid& G, unknown& z, unknown& Qx, unknown& Qy)
{
	z.export_unknown();
	Qx.export_unknown();
	Qy.export_unknown();
	G.boundary_conditions();
	z.import_unknown();
	Qx.import_unknown();
	Qy.import_unknown();
}
