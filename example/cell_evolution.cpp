#include "finite_volume.hpp"

void FINITE_VOLUME::cell_evolution(double& new_z, double& new_Qx, double& new_Qy, const triplet& u_c, const interfaces_values& intern_interfaces, const interfaces_values& extern_interfaces, const double& dx, const double& dy, const double& dt)
{
	using namespace FINITE_VOLUME;
	double3 C0;

	C_flux(u_c, intern_interfaces, extern_interfaces, dx, dy, C0);

	// forward Euler
	new_z = u_c.z + dt*C0.first;
	new_Qx = u_c.Qx + dt*C0.second;
	new_Qy = u_c.Qy + dt*C0.third;
}

