/*
 * Hx_flux.cpp
 *
 *  Created on: 02/feb/2012
 *      Author: ubuntu
 */

#include "finite_volume.hpp"

void FINITE_VOLUME::H_flux(const triplet& plus, const triplet& minus, double3& H, const p_speed& speed, const p_flux& flux)
{
	// u_p = valori "+" all'interfaccia (limiti da destra)
	// u_m = valori "-" all'interfaccia (limiti da sinistra)

	using namespace std;

	double3 f_p; // f_p = fx(u_p); flusso calcolato nei valori di interfaccia "+"
	double3 f_m;
	double3 u_p(plus.z, plus.Qx, plus.Qy);
	double3 u_m(minus.z, minus.Qx, minus.Qy);

	double a; // massima velocità all'interfaccia

	speed(plus, minus, a);
	flux(plus, f_p);
	flux(minus, f_m);

	/*
	cout<<"f_p: ";
	f_p.print();
	cout<<"f_m n";
	f_m.print();
	/**/
	H = (f_p + f_m)*0.5 - (u_p - u_m)*(0.5*a);
}




