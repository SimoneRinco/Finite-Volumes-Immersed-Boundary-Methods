/*
 * time_step.cpp
 *
 *  Created on: 23/gen/2012
 *      Author: ubuntu
 */

#include "finite_volume.hpp"

double FINITE_VOLUME::time_step( cell_unknown& z, cell_unknown& Qx, cell_unknown& Qy, const std::vector<IMMERSED_BOUNDARY::label>& WC, const double& dx, const double& dy, const double& CFL)
{

	using namespace std;
	double dt;
	double hc, Qxc, Qyc; // c = "central" (valori a centro cella)
	double u, v;
	double max_lambda_x = 0.0;
	double max_lambda_y = 0.0;
	const double g=9.81;

	// fac simile della funzione che calcola il passo temporale
	// max_lambda = max ( |u| + sqrt(g*h), |v| + sqrt(g*h) )

	for (std::vector<IMMERSED_BOUNDARY::label>::const_iterator it = WC.begin(); it != WC.end(); it++)
	{
		// ipotizzo fondo costante = 0
		hc = z.central(*it); // h = z
		Qxc = Qx.central(*it);
		Qyc = Qy.central(*it);
		u = Qxc/hc;
		v = Qyc/hc;

		max_lambda_x = max(max_lambda_x, fabs(u) + sqrt(g*hc));
		max_lambda_y = max(max_lambda_y, fabs(v) + sqrt(g*hc));
	}

	dt = CFL*min(dx/max_lambda_x, dy/max_lambda_y);
	return dt;
}
