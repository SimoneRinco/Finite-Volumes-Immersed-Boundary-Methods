#include "finite_volume.hpp"

void FINITE_VOLUME::cell_reconstruction( \
		const IMMERSED_BOUNDARY::stencil_values& u, \
		double& u_Em, double& u_Wp, double& u_Nm, double& u_Sp)
{

	using namespace FINITE_VOLUME;

	// u.E --> u(j+1,k); u.S --> u(j,k-1)
	double u_x = minmod(u.E-u.central, u.central-u.W, 0.5*(u.E-u.W));
	double u_y = minmod(u.N-u.central, u.central-u.S, 0.5*(u.N-u.S));


	u_Em = u.central + 0.5*u_x; // u_(j+1/2,k)^-
	u_Wp = u.central - 0.5*u_x; // u_(j-1/2,k)^+
	u_Nm = u.central + 0.5*u_y; // u_(j,k+1/2)^-
	u_Sp = u.central - 0.5*u_y; // u_(j,k-1/2)^+
}

