#pragma once

#include <map>
#include <set>
#include <vector>
#include <list>
#include <string>
#include "algorithm"
#include "cell.hpp"
#include "Point2d.hpp"
#include "Point2d_def.hpp"
#include "boundary.hpp"
#include "Getpot.hpp"
#include "immersed_boundary.hpp"
#include "find_in_list_def.hpp"

class grid
{
public:

  using label = Point2d<unsigned int>;
  using p_comp = bool (*)(const label&, const label&);

  friend class ghost_unknown;

  /*!
   * Constructs the grid given the domain vertices
   */
  grid(
    const std::string& input_f,
    const std::string& vertices_f,
    const p_comp& comp_function = IMMERSED_BOUNDARY::compare);

  grid(
    const IMMERSED_BOUNDARY::p_fun& p_f,
    const IMMERSED_BOUNDARY::p_grad_fun& p_grad_f,
    const std::string& input_f,
    const p_comp& comp_function = IMMERSED_BOUNDARY::compare);

  grid(const grid&);
  ~grid();

  // writeout functions (write on files)

  void writeout_domain(const bool& save_interp_matrices = true);
  void writeout_BC_type();
  //void writeout_unknowns(const std::string& file_name = "./data/unknowns/unknowns");
  void writeout_unknowns(const std::string& file_name);

  // print functions

  void print_cells();
  void print_cutting_edges();
  void info();
  void print_stencil_values(const label& computational_cell, const unsigned int& n_unknown);

  // most useful public members

  void boundary_conditions();

  unsigned int get_Nx() const;
  unsigned int get_Ny() const;
  unsigned int get_N_wet_cells() const;
  unsigned int get_N_ghost_cells() const;
  unsigned int get_N_equations() const;
  std::vector<double> get_x() const;
  std::vector<double> get_y() const;
  double get_dx() const;
  double get_dy() const;

  const std::vector<label>& get_wet_cells() const;
  std::vector<label> get_ghost_cells() const;
  Point2d<double> get_cell_center(const unsigned int& i, const unsigned int& j) const;
  Point2d<double> get_cell_center(const label& l) const;
  bool is_wet(const unsigned int& i, const unsigned int& j) const;
  unsigned int get_N_edges();
  unsigned int get_BC_type(const unsigned int& edge_number);
  bool get_stencil_values(const label& computation_cell, const unsigned int& unknown_number, double& EST_value, double& WEST_value, double& NORD_value, double& SUD_value);
  bool get_stencil_values(const label& computation_cell, const unsigned int& un, IMMERSED_BOUNDARY::stencil_values& u);
  double get_unknown_value(const label& wet_cell_label, const unsigned int& n_unknown);
  double get_unknown_value(const unsigned int& wet_i, const unsigned int& wet_j, const unsigned int& n_unknown);

  void set_unknown_values(const label& cell_label, const unsigned int& unknown_number, const double& unknown_value);
  void set_unknown_values(IMMERSED_BOUNDARY::p_fun pf, const unsigned int& unknown_number);
  void set_unknown_values(const std::map<label, std::vector<double> >& wet_unknowns);
  void set_BC_type(const unsigned int& BC_type);
  void set_BC_type(const unsigned int& first_edge_number, const unsigned int& last_edge_number, const unsigned int& BC_type);

private:

  GetPot if_parameters;
  GetPot if_vertices;

  unsigned int Nx; // number of effective cells in x direction (total cells: Nx + 2 not cutted and dry cells)
  unsigned int Ny;
  double dx, dy;
  double toll_x, toll_y;
  unsigned int N_equations;
  std::vector<double> x;
  std::vector<double> y;

  std::vector<unsigned int> type_of_boundary_conditions;
  std::map<label, cell, p_comp> cells; // all cells
  std::map<label, ghost_cell, p_comp> ghost_cells;
  std::vector<label> wet_cells; // wet cells labels
  std::map<label, IMMERSED_BOUNDARY::M4_LU, p_comp, Eigen::aligned_allocator<std::pair<const label, IMMERSED_BOUNDARY::M4_LU> > > all_wet_matrices;
  std::map<label, IMMERSED_BOUNDARY::M8M4_LU, p_comp, Eigen::aligned_allocator<std::pair<const label, IMMERSED_BOUNDARY::M8M4_LU> > > GPs_Matrices;
  std::map<label, IMMERSED_BOUNDARY::M6M3_LU_non_cc, p_comp, Eigen::aligned_allocator<std::pair<const label, IMMERSED_BOUNDARY::M6M3_LU_non_cc> > > noGPs_Matrices;

  boundary ccb; // Cut Cell Boundary

  // private member functions

  void last_common_ctrs_part();
  IMMERSED_BOUNDARY::position V_position(const label& cell_label, Point2d<double>& V);
  void cut_cells(const label& actual_label, Point2d<double>& actual_vertex, const label& finish_label, Point2d<double>& finish_vertex, const double& toll_x, const double& toll_y, bool exit);
  bool valid_edge(const label& l, const Point2d<double>& lv, const Point2d<double>& rv, const double& toll_x, const double& toll_y, BOUNDARY::side&);
  void set_L_adj(const label&, std::vector<label>&);
  Point2d<double> closest_corner(const label& L, const Point2d<double>& V);
  void remove_invalid_edges(bool& all_valid);
  void move_vertices();
  void set_cut();
  void set_area_zero_on_wet_side();
  void set_all_dry();
  void set_all_not_cut();
  void set_wet(label* &);
  void set_wet2();
  void set_wet_cells();
  void set_wet_cutted_centers();
  bool is_point_in_cell(const Point2d<double>&, const label &);
  bool cutted_center(const label&);
  bool cutted_center_by_diagonal(const label&);
  IMMERSED_BOUNDARY::Edge build_edge(const Point2d<double>& zero, const label& wet_cell, const IMMERSED_BOUNDARY::p_fun& p_f, const IMMERSED_BOUNDARY::p_grad_fun& p_grad_f);
  void build_and_add_edge(const label& wet_label, const label& dry_label, const bool& move_vertical, const double& toll, const IMMERSED_BOUNDARY::p_fun& p_f, const IMMERSED_BOUNDARY::p_grad_fun p_grad_f, std::map<label, IMMERSED_BOUNDARY::Edge, p_comp>& single_edges);
  void build_and_add_edge2(const label& wet_label, const label& vertical_dry_label, const label& orizontal_dry_label, const IMMERSED_BOUNDARY::p_fun& p_f, const IMMERSED_BOUNDARY::p_grad_fun& p_grad_f, std::map<label, IMMERSED_BOUNDARY::Edge, p_comp>& single_edges);
  int closest_vertex(const Point2d<double>& current_Vertex, const Point2d<double>& V1, const Point2d<double>& V2);
  void join_edges(const label& current_label, Point2d<double>*& current_Vertex, std::map<label, IMMERSED_BOUNDARY::Edge, p_comp>& single_edges, int& N_single_edges, std::set<label,p_comp>& considered_labels);
  void next_and_new_vertex(const Point2d<double>& current_V, Point2d<double>& V1, Point2d<double>& V2, Point2d<double>*& new_V, Point2d<double>*& next_V);
  void closest_cell_pos_and_vertices(const label& current_label, Point2d<double>*& current_Vertex, Point2d<double>*& next_Vertex, Point2d<double>*& new_Vertex, IMMERSED_BOUNDARY::position& next_cell_pos, IMMERSED_BOUNDARY::position& next_Vertex_pos, std::map<label, IMMERSED_BOUNDARY::Edge, p_comp>& single_edges, std::set<label,p_comp>& considered_labels);
  void build_ghost_cells();
  label find_SW_label(const Point2d<double>& RP);
  unsigned int find_number_ghost_corners(const label& SW_label);
  void build_single_BP_quantities(const label& ghost_cell, const label& wet_label, ghost_quantities CGgq, std::vector<ghost_quantities>& gqs);
  void build_interpolation_matrices();
  IMMERSED_BOUNDARY::M4_LU build_all_wet_4x4_matrix(const label&);
  IMMERSED_BOUNDARY::M8_LU build_GPs_8x8_matrix(const label&);
  IMMERSED_BOUNDARY::M4_LU build_GPs_4x4_matrix(const label&);
  IMMERSED_BOUNDARY::M6_LU build_noGPs_6x6_matrix(const label&, unsigned int& non_considered_corner);
  IMMERSED_BOUNDARY::M3_LU build_noGPs_3x3_matrix(const label&);
  void calculate_RP_quantities();
  void calculate_GP_quantities();
  double compute_single_stencil_value(const label& current_label, const label& adj_label, const unsigned int& unknown_number);
  void writeout_real_domain(const std::vector<std::pair<label, Point2d<double> > >& real_vertices);

};
