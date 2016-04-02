#include "grid.hpp"
#include "Config.hpp"
#include "Domain.hpp"

grid::grid(const std::string& input_f, const std::string& vertices_f, const p_comp& comp_function):
		cells(comp_function),
		ghost_cells(comp_function),
		all_wet_matrices(comp_function),
		GPs_Matrices(comp_function),
		noGPs_Matrices(comp_function)

{
    Config config;
    config.load(input_f);

    Nx = config.nx;
    Ny = config.ny;
    N_equations = config.n_equations;
    const double x_min = config.x_min;
    const double x_max = config.x_max;
    const double y_min = config.y_min;
    const double k = config.k;
    x.resize(Nx + 3);
    y.resize(Ny + 3);

	using namespace IMMERSED_BOUNDARY;

	Point2d<double> P,v;
	std::pair<label,Point2d<double> >* V;
	std::pair<label,Point2d<double> >* V_n; //considered vertex and next vertex

	label Key,L;
	std::vector<label> L_adj(4,label());
	cell C(N_equations);
	std::map<label,Point2d<double> >::iterator it;
	std::vector<unsigned int>::iterator itui;
	std::list<std::pair<Point2d<double>, label> >::iterator itl, nitl;

	double xv,yv;

    Domain domain;
    domain.load(vertices_f);

	unsigned int N_vertices = domain.vertices.size();
	std::vector<std::pair<label, Point2d<double> > > vertices(N_vertices+1);

	// acquisizione vertici del dominio originale da file
	for(int i=0; i<N_vertices; i++)
	{
		(vertices.at(i)).second=domain.vertices[i];
	}
	vertices.at(N_vertices).second=vertices.at(0).second; // ultimo vertice coincide col primo

	// Scrittura su file importabile da matlab del dominio reale
	writeout_real_domain(vertices);

	dx = (x_max-x_min)/Nx;
	dy = dx;
	//dy = (y_max-y_min)/Ny;

	const double toll_x = k*dx;
	const double toll_y = k*dy;
	this->toll_x = toll_x;
	this->toll_y = toll_y;

	//inizializzazione vettori x e y (griglia)
	for(int nx=0; nx<Nx+3; nx++) {x.at(nx)=x_min + (nx-1)*dx;}
	for(int ny=0; ny<Ny+3; ny++) {y.at(ny)=y_min + (ny-1)*dy;}

	for (int iv=0; iv<N_vertices; iv++)
	{
		Key = IMMERSED_BOUNDARY::find_label(x,y,(vertices.at(iv)).second);
		(vertices.at(iv)).first = Key;
	}
	vertices.at(N_vertices).first=vertices.at(0).first;

	// inizializzazione centri cella
	for(int nx=0; nx<Nx+2; nx++)
	{
		for(int ny=0; ny<Ny+2; ny++)
		{
			Key.set_values(nx,ny);
			C.set_center(x_min+0.5*dx+(nx-1)*dx,y_min+0.5*dy+(ny-1)*dy);
			cells.insert(std::pair<label,cell>(Key,C));
		}
	}

	// riposizionamento di tutti i vertici originali vicini ai bordi cella
	for(int iv=0; iv<N_vertices+1; iv++)
	{
	V = &vertices.at(iv);
	V_position(V->first, V->second);
	}

	// generazione dei lati

	for (int iv=0; iv<N_vertices; iv++)
	{
		V = &vertices.at(iv);
		V_n = &vertices.at(iv+1);

		if (!((*V).first == (*V_n).first)) // se i vertici non sono nella stessa cella
		{
		cut_cells(V->first, V->second, V_n->first, V_n->second, toll_x, toll_y, false);
		}
	}

	///////////

	last_common_ctrs_part();

} // end constructor
