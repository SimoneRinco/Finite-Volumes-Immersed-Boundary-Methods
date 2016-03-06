#include "finite_volume.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// cell_unknown
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

FINITE_VOLUME::cell_unknown::cell_unknown(grid& G, const unsigned int& n_unknown):
	central(G, n_unknown),
	E(G, n_unknown),
	W(G, n_unknown),
	N(G, n_unknown),
	S(G, n_unknown)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// triplet
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

FINITE_VOLUME::triplet::triplet(unknown& z, unknown& Qx, unknown& Qy, const IMMERSED_BOUNDARY::label& wc):
	z(z(wc)), Qx(Qx(wc)), Qy(Qy(wc))
{}

FINITE_VOLUME::triplet::triplet(unknown& uz, unknown& uQx, unknown& uQy, const IMMERSED_BOUNDARY::label& wc, const unknown_position::position& pos):
	z(uz.get_stencil_value(wc, pos, false)),
	Qx(uQx.get_stencil_value(wc, pos, false)),
	Qy(uQy.get_stencil_value(wc, pos, false))
{}

FINITE_VOLUME::triplet::triplet(double& _z, double& _Qx, double& _Qy):
	z(_z), Qx(_Qx), Qy(_Qy)
{}

FINITE_VOLUME::triplet::triplet(triplet& lhs):
	z(lhs.z), Qx(lhs.Qx), Qy(lhs.Qy)
{}

FINITE_VOLUME::triplet::triplet(const triplet& lhs):
	z(lhs.z), Qx(lhs.Qx), Qy(lhs.Qy)
{}

void FINITE_VOLUME::triplet::print() const
{
	std::cout<< "z: " <<z <<", Qx: " <<Qx <<", Qy: " <<Qy <<std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// interfaces_values
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

FINITE_VOLUME::interfaces_values::interfaces_values(triplet& Et, triplet& Wt, triplet& Nt, triplet& St):
	E(Et),
	W(Wt),
	N(Nt),
	S(St)
{}

FINITE_VOLUME::interfaces_values::interfaces_values(const triplet& Et, const triplet& Wt, const triplet& Nt, const triplet& St):
	E(Et),
	W(Wt),
	N(Nt),
	S(St)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// double3
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////


FINITE_VOLUME::double3::double3()
{}
FINITE_VOLUME::double3::double3(const double& f, const double& s, const double& t):
		first(f), second(s), third(t)
{}

FINITE_VOLUME::double3& FINITE_VOLUME::double3::operator =(const double3& lhs)
{
	first = lhs.first;
	second = lhs.second;
	third = lhs.third;
	return (*this);
}

FINITE_VOLUME::double3 FINITE_VOLUME::double3::operator +(const double3& lhs)
{
	return double3(first+lhs.first, second+lhs.second, third+lhs.third);
}

FINITE_VOLUME::double3 FINITE_VOLUME::double3::operator -(const double3& lhs)
{
	return double3(first-lhs.first, second-lhs.second, third-lhs.third);
}

FINITE_VOLUME::double3 FINITE_VOLUME::double3::operator *(const double& c)
{
	return double3(first*c, second*c, third*c);
}

void FINITE_VOLUME::double3::print()
{
	std::cout<< "[" << first <<", " <<second <<", " <<third <<"]" <<std::endl;
}

