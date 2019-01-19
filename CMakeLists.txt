cmake_minimum_required(VERSION 3.10.2)


add_executable(
  ibfv_example
  example/example.cpp
  example/cell_evolution.cpp
  example/cell_reconstruction.cpp
  example/C_flux.cpp
  example/evolution.cpp
  example/example.cpp
  example/flux_x_y.cpp
  example/H_flux.cpp
  example/interfaces_boundary_conditions.cpp
  example/minmod.cpp
  example/reconstruction.cpp
  example/set_initial_conditions.cpp
  example/struct_definitions.cpp
  example/time_step.cpp
  example/x_y_speed.cpp
  src/boundary.cpp
  src/cell.cpp
  src/compare.cpp
  src/errors.cpp
  src/find_BP_RP.cpp
  src/find_label.cpp
  src/find_zero.cpp
  src/ghost_unknown.cpp
  src/grid_boundary_conditions.cpp
  src/grid_build_edge.cpp
  src/grid_build_ghost_cells.cpp
  src/grid_build_interpolation_matrices.cpp
  src/grid_calculate_GP_quantities.cpp
  src/grid_calculate_RP_quantities.cpp
  src/grid.cpp
  src/grid_ctr_level_set.cpp
  src/grid_ctr_vertices.cpp
  src/grid_cut_cells.cpp
  src/grid_cutted_center.cpp
  src/grid_join_edges.cpp
  src/grid_last_common_ctrs_part.cpp
  src/grid_move_vertices.cpp
  src/grid_public_getters.cpp
  src/grid_public_setters.cpp
  src/grid_remove_invalid_edges.cpp
  src/grid_set_area_zero_on_wet_side.cpp
  src/grid_set_cut.cpp
  src/grid_set_L_adj.cpp
  src/grid_set_wet2.cpp
  src/grid_set_wet_cells.cpp
  src/grid_set_wet.cpp
  src/grid_valid_edge.cpp
  src/grid_V_position.cpp
  src/grid_writeout.cpp
  src/int2string.cpp
  src/relative_position.cpp
  src/unknown.cpp
  src/wet_unknown.cpp
  )

target_include_directories(
  ibfv_example
  PUBLIC example
  PUBLIC include
  )

target_compile_options(
  ibfv_example
  PUBLIC "-Wall"
  PUBLIC "-Werror"
  PUBLIC "-g"
  PUBLIC "-O0"

  )
