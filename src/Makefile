I_DIR = ../include
LIB_DIR = ../lib
LIB_NAME = libgrid.a

FLAGS = -O0 -g -Wall -Werror

GRID_OBJS = compare.o cell.o boundary.o grid.o find_label.o relative_position.o errors.o \
		grid_writeout.o grid_valid_edge.o grid_move_vertices.o grid_set_wet.o grid_set_cut.o \
		grid_set_area_zero_on_wet_side.o grid_ctr_level_set.o find_zero.o grid_build_edge.o \
		grid_join_edges.o grid_remove_invalid_edges.o grid_set_wet2.o grid_set_wet_cells.o \
		grid_build_ghost_cells.o \
		find_BP_RP.o grid_build_interpolation_matrices.o int2string.o grid_cutted_center.o \
		grid_V_position.o grid_cut_cells.o grid_ctr_vertices.o grid_set_L_adj.o \
		grid_last_common_ctrs_part.o \
		grid_public_getters.o grid_public_setters.o \
		grid_calculate_RP_quantities.o grid_calculate_GP_quantities.o \
		grid_boundary_conditions.o \
		wet_unknown.o ghost_unknown.o unknown.o

lib: $(GRID_OBJS)
	mkdir -p $(LIB_DIR)
	ar -cr $(LIB_DIR)/$(LIB_NAME) $^

clean:
	-rm -rf *.o
	-rm -rf $(LIB_DIR)/*

$(GRID_OBJS): %.o : %.cpp
	g++ -c $< -I$(I_DIR) $(FLAGS)
