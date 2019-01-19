#include "finite_volume.hpp"

void FINITE_VOLUME::reconstruction(grid& g, cell_unknown& z, cell_unknown& Qx, \
		cell_unknown& Qy, const std::vector<IMMERSED_BOUNDARY::label>& WC)
{

	using namespace IMMERSED_BOUNDARY;
	using namespace FINITE_VOLUME;
	using namespace std;

	for (std::vector<label>::const_iterator it=WC.begin(); it!=WC.end(); it++)
	{
		// import delle medie di cella tramite referenze;
		const stencil_values z_sv = z.central.get_stencil_values(*it);
		/*
		cout <<"display z_sv in recostruction\n";
		cout<<"wet cell: ";
		it->print();
		cout<<"central: " <<z_sv.central <<", E: " <<z_sv.E  <<", W: " <<z_sv.W <<", N: " <<z_sv.N <<", S: " <<z_sv.S <<endl;
		cout<<"valori teorici\n";
		cout<<z.central(*it) <<", " <<z.central.get_stencil_value(*it,unknown_position::E) \
				<<", " <<z.central.get_stencil_value(*it,unknown_position::W) \
				<<", " <<z.central.get_stencil_value(*it,unknown_position::N) \
				<<", " <<z.central.get_stencil_value(*it,unknown_position::S) <<endl;
		*/

		// calcolo valori interfaccia interna;
		cell_reconstruction(z_sv, z.E(*it), z.W(*it), z.N(*it), z.S(*it));

		const stencil_values Qx_sv = Qx.central.get_stencil_values(*it);
		cell_reconstruction(Qx_sv, Qx.E(*it), Qx.W(*it), Qx.N(*it), Qx.S(*it));

		const stencil_values Qy_sv = Qy.central.get_stencil_values(*it);
		cell_reconstruction(Qy_sv, Qy.E(*it), Qy.W(*it), Qy.N(*it), Qy.S(*it));
	}
}
