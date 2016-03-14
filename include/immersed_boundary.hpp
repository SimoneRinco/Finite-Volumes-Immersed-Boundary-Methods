#ifndef IMMERSED_BOUNDARY_HPP_
#define IMMERSED_BOUNDARY_HPP_

#include "Point2d.hpp"
#include "Point2d_def.hpp"
#include "errors.hpp"
#include <list>
#include <vector>
#include <cmath>
#include <libs/Eigen/Eigen/Dense>

namespace IMMERSED_BOUNDARY
{
typedef Point2d<unsigned int> label;
typedef double (*p_fun)(const Point2d<double>&);
typedef Point2d<double> (*p_grad_fun)(const Point2d<double>&);
typedef std::pair<Point2d<double>, Point2d<double> > Edge;
typedef bool (*p_comp)(const label&, const label&);

typedef bool (*p_comp)(const label&, const label&);

enum position{Intern, N, NE, E, SE, S, SW, W, NW};
enum BC_type{open, closed};

typedef Eigen::Matrix<double, 3, 3> M3;
typedef Eigen::Matrix<double, 4, 4> M4;
typedef Eigen::Matrix<double, 6, 6> M6;
typedef Eigen::Matrix<double, 8, 8> M8;

typedef Eigen::Matrix<double, 3, 1> V3;
typedef Eigen::Matrix<double, 4, 1> V4;
typedef Eigen::Matrix<double, 6, 1> V6;
typedef Eigen::Matrix<double, 8, 1> V8;

typedef std::pair<M8,M4> M8M4;
typedef std::pair<M6,M3> M6M3;

typedef Eigen::FullPivLU<M3> M3_LU;
typedef Eigen::FullPivLU<M4> M4_LU;
typedef Eigen::FullPivLU<M6> M6_LU;
typedef Eigen::FullPivLU<M8> M8_LU;

typedef std::pair<M8_LU,M4_LU> M8M4_LU;
typedef std::pair<M6_LU,M3_LU> M6M3_LU;

struct M6M3_LU_non_cc
{
	M6M3_LU matrices;
	unsigned int non_cc; // non considered corner (dry and non ghost corner)
};

struct stencil_values
{
	double& central;
	double& E;
	double& W;
	double& N;
	double& S;

	stencil_values(double& c, double& e, double& w, double& n, double& s):
		central(c), E(e), W(w), N(n), S(s)
	{}
	stencil_values(const stencil_values& lhs):
	central(lhs.central), E(lhs.E), W(lhs.W), N(lhs.N), S(lhs.S)
	{}
};


bool compare(const label&, const label&);
label find_label(const std::vector<double>& x, const std::vector<double>& y, const Point2d<double>& P);
position relative_position(const Point2d<double> &V_n, const Point2d<double> &V, const double& toll_x, const double& toll_y);


template <typename T>
unsigned int find_in_list(const T& element, const std::list<T>& l);
double find_zero(const Point2d<double>& c1, const Point2d<double>& c2, const p_fun& p_f, const bool& move_vertical, const double& toll);
void find_BP_RP_normal(const Point2d<double>& V1, const Point2d<double>& V2, const Point2d<double>& cc, Point2d<double>& BP, Point2d<double>& RP, Point2d<double>& n);

}



#endif /* IMMERSED_BOUNDARY_HPP_ */


