#include "grid.hpp"

double quadratic(const Point2d<double>& P)
{
	double x = P(0);
	double y = P(1);

	return x*x + y*y - 1;
}

Point2d<double> grad_quadratic(const Point2d<double>& P)
{
	double x = P(0);
	double y = P(1);

	double grad_x = 2*x;
	double grad_y = 2*y;

	Point2d<double> grad(grad_x, grad_y);

	return grad;
}

int main(int argc, char **argv)
{

	IMMERSED_BOUNDARY::p_comp p_c = IMMERSED_BOUNDARY::compare;
	IMMERSED_BOUNDARY::p_fun p_f = quadratic;
	IMMERSED_BOUNDARY::p_grad_fun p_gf = grad_quadratic;

	std::string input_file("./input_parameters.pot");
	std::string vertices_file("vertices.pot");
	bool save_interp_matrices = true;

	grid G1; // costruzione dominio "per vertici"
	G1.writeout_domain(); // => G1.writeout_domain(true);

	grid G2(input_file, vertices_file, p_c); // come G1
	G2.writeout_domain(save_interp_matrices);

	grid G3(p_f, p_gf); // costruzione dominio per "level set"

	grid G4(p_f, p_gf, input_file, p_c); // come G3

	return 0;
}

