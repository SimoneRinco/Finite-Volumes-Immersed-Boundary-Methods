#ifndef FV_HPP_
#define FV_HPP_

#include "grid.hpp"
#include "unknown.hpp"

namespace FINITE_VOLUME
{

	struct cell_unknown
	{
		unknown central;
		unknown E;
		unknown W;
		unknown N;
		unknown S;

		cell_unknown(grid& G, const unsigned int& n_unknown);
	};

	struct triplet
	{
		double& z;
		double& Qx;
		double& Qy;

		triplet(unknown& z, unknown& Qx, unknown& Qy, const IMMERSED_BOUNDARY::label& wc);
		triplet(unknown& z, unknown& Qx, unknown& Qy, const IMMERSED_BOUNDARY::label& wc, const unknown_position::position& pos);
		triplet(double& z, double& Qx, double& Qy);
		triplet(triplet&);
		triplet(const triplet&);
		void print() const;
	};

	struct interfaces_values
	{
		triplet E;
		triplet W;
		triplet N;
		triplet S;

		interfaces_values(triplet& Et, triplet& Wt, triplet& Nt, triplet& St);
		interfaces_values(const triplet& Et, const triplet& Wt, const triplet& Nt, const triplet& St);
		interfaces_values(interfaces_values&);
		interfaces_values(const interfaces_values&);
	};

	struct double3
	{
		double first;
		double second;
		double third;

		double3();
		double3(const double&, const double&, const double&);
		double3& operator=(const double3& lhs);
		double3 operator+(const double3& lhs);
		double3 operator-(const double3& lhs);
		double3 operator*(const double& c);
		void print();
	};

	typedef void (*p_speed)(const triplet&, const triplet&, double&);
	typedef void (*p_flux)(const triplet&, double3&);


	void initial_conditions(grid&);

	void reconstruction(grid& g, cell_unknown& z, cell_unknown& Qx, cell_unknown& Qy, const std::vector<IMMERSED_BOUNDARY::label>& WC);
	void cell_reconstruction(const IMMERSED_BOUNDARY::stencil_values& u, double& u_Em, double& u_Wp, double& u_Nm, double& u_Sp);
	double minmod(const double& x1, const double& x2, const double& x3);

	void interfaces_boundary_conditions(grid&, cell_unknown& z, cell_unknown& Qx, cell_unknown& Qy);
	void interfaces_boundary_conditions(grid&, unknown& z, unknown& Qx, unknown& Qy);

	double time_step(cell_unknown& z, cell_unknown& Qx, cell_unknown& Qy, const std::vector<IMMERSED_BOUNDARY::label>& WC, const double& dx, const double& dy, const double& CFL);

	void evolution(cell_unknown& z, cell_unknown& Qx, cell_unknown& Qy, unknown& new_z, unknown& new_Qx, unknown& new_Qy, const std::vector<IMMERSED_BOUNDARY::label>& WC, const double& dx, const double& dy, const double& dt);
	void cell_evolution(double& new_z, double& new_Qx, double& new_Qy, const triplet& u_c, const interfaces_values& minus_interf, const interfaces_values& plus_interf, const double& dx, const double& dy, const double& dt);

	void C_flux(const triplet&, const interfaces_values& intern_interf, const interfaces_values& extern_interf, const double& dx, const double& dy, double3& C);
	void H_flux(const triplet& u_p, const triplet& u_m, double3& H, const p_speed&, const p_flux&);

	void x_speed(const triplet& u_p, const triplet& u_m, double& a);
	void y_speed(const triplet& u_p, const triplet& u_m, double& a);
	void flux_x(const triplet& u, double3&);
	void flux_y(const triplet& u, double3&);

}

#endif /* FV_HPP_ */
