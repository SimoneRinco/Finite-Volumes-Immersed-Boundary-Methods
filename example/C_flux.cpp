/*
 * C_flux.cpp
 *
 *  Created on: 02/feb/2012
 *      Author: ubuntu
 */

#include "finite_volume.hpp"

void FINITE_VOLUME::C_flux(const triplet& uc, const interfaces_values& intern_interf, const interfaces_values& extern_interf, const double& dx, const double& dy, double3& C)
{
	using namespace FINITE_VOLUME;
	using namespace std;

	// caricamento valori di interfaccia

	const triplet u_Em(intern_interf.E);
	const triplet u_Ep(extern_interf.E);

	const triplet u_Wp(intern_interf.W);
	const triplet u_Wm(extern_interf.W);

	const triplet u_Nm(intern_interf.N);
	const triplet u_Np(extern_interf.N);

	const triplet u_Sp(intern_interf.S);
	const triplet u_Sm(extern_interf.S);

	// visualizzazioni debug

	/*
	cout<< "##### C_flux input:\n";
	cout<<"-->u_Em\n";
	u_Em.print();
	cout<<"-->u_Ep\n";
	u_Ep.print();
	cout<<"-->u_Wp\n";
	u_Wp.print();
	cout<<"-->u_Wm\n";
	u_Wm.print();
	cout<<"-->u_Nm\n";
	u_Nm.print();
	cout<<"-->u_Np\n";
	u_Np.print();
	cout<<"-->u_Sp\n";
	u_Sp.print();
	cout<<"-->u_Sm\n";
	u_Sm.print();
	cout<< "#########################\n";
	*/

	double3 Hx_p, Hx_m, Hy_p, Hy_m;


	// calcolo flussi H

	// Hx fluxes

	H_flux(u_Ep, u_Em, Hx_p, x_speed, flux_x); // flusso (Hx)^+ calcolato con i valori di intefaccia EST
	H_flux(u_Wp, u_Wm, Hx_m, x_speed, flux_x);

	// Hy fluxes
	H_flux(u_Np, u_Nm, Hy_p, y_speed, flux_y);
	H_flux(u_Sp, u_Sm, Hy_m, y_speed, flux_y);


	C = (Hx_p-Hx_m)*(1.0/dx) + (Hy_p-Hy_m)*(1.0/dy);
	C = C*(-1);

	// visualizzazioni debug

	/*
	cout <<"## H fluxes\n";
	cout<<"Hx_p:\n";
	Hx_p.print();
	cout<<"Hx_m\n:";
	Hx_m.print();
	cout<<"Hy_p\n:";
	Hy_p.print();
	cout<<"Hy_m\n:";
	Hy_m.print();
	cout<<"##\n";
	cout<<"C flux:\n";
	C.print();
	*/
}
