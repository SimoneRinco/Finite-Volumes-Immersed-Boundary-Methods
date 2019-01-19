#include "finite_volume.hpp"

void FINITE_VOLUME::evolution(cell_unknown& z, cell_unknown& Qx, \
	cell_unknown& Qy, unknown& new_z, unknown& new_Qx, unknown& new_Qy, \
	const std::vector<IMMERSED_BOUNDARY::label>& WC, \
	const double& dx, const double& dy, const double& dt)
{

	using namespace IMMERSED_BOUNDARY;
	using namespace FINITE_VOLUME;

	// u_c: medie di cella u(j,k)
	// u_Em = u(j+1/2,k)^-, ... , u_Sp = u(j,k-1/2)^+
	for (std::vector<label>::const_iterator it=WC.begin(); it!=WC.end(); it++)
	{

		/*
		//std::cin >> a;
		std::cout<<"Computational cell: ";
		it->print();
		*/
		// medie di cella
		const triplet u_c(z.central, Qx.central, Qy.central, *it);

		//interfacce EST, OVEST, NORD, SUD interne (u_Nm --> u(j,k+1/2)^-)
		const triplet u_Em(z.E, Qx.E, Qy.E, *it);
		const triplet u_Wp(z.W, Qx.W, Qy.W, *it);
		const triplet u_Nm(z.N, Qx.N, Qy.N, *it);
		const triplet u_Sp(z.S, Qx.S, Qy.S, *it);
		const interfaces_values intern_interf(u_Em, u_Wp, u_Nm, u_Sp);

		// interfacce EST, OVEST, NORD, SUD esterne (u_Ep --> u(j+1/2,k)^+)

		const triplet u_Ep(z.W, Qx.W, Qy.W, *it, unknown_position::E);
		const triplet u_Wm(z.E, Qx.E, Qy.E, *it, unknown_position::W);
		const triplet u_Np(z.S, Qx.S, Qy.S, *it, unknown_position::N);
		const triplet u_Sm(z.N, Qx.N, Qy.N, *it, unknown_position::S);
		const interfaces_values extern_interf(u_Ep, u_Wm, u_Np, u_Sm);


		// --> EVOLUZIONE <--

		cell_evolution(new_z(*it), new_Qx(*it), new_Qy(*it), u_c, \
			intern_interf, extern_interf, dx, dy, dt);
	}
	new_z.export_unknown();
	new_Qx.export_unknown();
	new_Qy.export_unknown();
}
