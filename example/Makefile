###################################
# MAKEFILE nella cartella example #
###################################

I_DIR1 = ../include
I_DIR2 = ./
LIB_DIR = ../lib

LIBNAME = grid
LIB = $(LIB_DIR)/lib$(LIBNAME).a

FLAGS = -O0 -g -Wall -Werror

# Nome del sorgente di prova (il main): l'eseguibile avrà lo stesso nome
EXAMPLE = example


EXAMPLE_OBJ = $(EXAMPLE).o
OTHER_OBJS = set_initial_conditions.o reconstruction.o time_step.o evolution.o \
	 struct_definitions.o cell_reconstruction.o minmod.o interfaces_boundary_conditions.o \
	 cell_evolution.o C_flux.o H_flux.o x_y_speed.o flux_x_y.o

all: $(EXAMPLE)

$(EXAMPLE): $(LIB) $(EXAMPLE_OBJ) $(OTHER_OBJS)
	g++ $(EXAMPLE_OBJ) $(OTHER_OBJS) -o $(EXAMPLE) -L $(LIB_DIR) -l $(LIBNAME) $(FLAGS)

$(EXAMPLE_OBJ): %.o : %.cpp
	g++ -c $< -I$(I_DIR1) -I$(I_DIR2) $(FLAGS)

$(OTHER_OBJS) : %.o : %.cpp
	g++ -c $< -I$(I_DIR1) -I$(I_DIR2) $(FLAGS)

$(LIB):
	(cd ../src; make lib)

lib:
	(cd ../src; make lib)


clean_interp_matrices:
	-rm  -rf ./data/domain/interp_matrices/all_wet/M4x4/*
	-rm  -rf ./data/domain/interp_matrices/all_wet/SW_labels/*
	     -rf
	-rm  -rf ./data/domain/interp_matrices/dry_no_GP/M3x3/*
	-rm  -rf ./data/domain/interp_matrices/dry_no_GP/M6x6/*
	-rm  -rf ./data/domain/interp_matrices/dry_no_GP/non_considered_corner/*
	-rm  -rf ./data/domain/interp_matrices/dry_no_GP/SW_labels/*
	     -rf
	-rm  -rf ./data/domain/interp_matrices/GPs/M4x4/*
	-rm  -rf ./data/domain/interp_matrices/GPs/M8x8/*
	-rm  -rf ./data/domain/interp_matrices/GPs/SW_labels/*

clean_data:
	-rm -rf ./data/domain/*
	-rm -rf ./data/unknowns/*
	-rm -rf ./data/z_frames/*
	-rm -rf ./data/z_frames_contour/*
	-rm -rf ./data/Qx_frames/*
	-rm -rf ./data/Qy_frames/*
	-rm -rf ./data/time

clean: clean_interp_matrices clean_data
	-rm -rf $(EXAMPLE)
	-rm -rf *.o
