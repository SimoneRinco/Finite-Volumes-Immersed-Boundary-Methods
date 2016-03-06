#include "finite_volume.hpp"

namespace INITIAL_CONDITIONS
{
	double z_init(const Point2d<double>& P)
	{
		/*
		 // dam break circolare
		if(P(0)*P(0)+P(1)*P(1) < 0.0625)
			return 2.0;
		else
			return 1.0;
			/**/

		if(P(0)<-5.0)
			return 2.0;
		else
			return 1.0;

	}

	double Qx_init(const Point2d<double>& P) {return 0.0;}

	double Qy_init(const Point2d<double>& P) {return 0.0;}
}

void FINITE_VOLUME::initial_conditions(grid& G)
{

	G.set_unknown_values(INITIAL_CONDITIONS::z_init, 1);
	G.set_unknown_values(INITIAL_CONDITIONS::Qx_init, 2);
	G.set_unknown_values(INITIAL_CONDITIONS::Qy_init, 3);
}
