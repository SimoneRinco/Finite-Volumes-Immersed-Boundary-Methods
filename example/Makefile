###################################
# MAKEFILE nella cartella example #
###################################

I_DIR1 = ../include
I_DIR2 = ./ 
I_DIR3 = ../libs/eigen-3.2.8
LIB_DIR = ../lib

LIBNAME = grid
LIB = $(LIB_DIR)/lib$(LIBNAME).a 

FLAGS = -O2 -pg -g -std=c++11

# Nome del sorgente di prova (il main): l'eseguibile avrà lo stesso nome
EXAMPLE = example
CONFIG_GENERATOR = build_config
DOMAIN_GENERATOR = build_domain
					
EXAMPLE_OBJ = $(EXAMPLE).o
OTHER_OBJS = set_initial_conditions.o reconstruction.o time_step.o evolution.o \
	 struct_definitions.o cell_reconstruction.o minmod.o interfaces_boundary_conditions.o \
	 cell_evolution.o C_flux.o H_flux.o x_y_speed.o flux_x_y.o 	
	
all: $(EXAMPLE)

$(EXAMPLE): $(LIB) $(CONFIG_GENERATOR) $(DOMAIN_GENERATOR) $(EXAMPLE_OBJ) $(OTHER_OBJS)
	g++ $(EXAMPLE_OBJ) $(OTHER_OBJS) -o $(EXAMPLE) -L $(LIB_DIR) -l $(LIBNAME) $(FLAGS)

$(CONFIG_GENERATOR): $(CONFIG_GENERATOR).cpp
	g++ $(CONFIG_GENERATOR).cpp -I $(I_DIR1) -o $(CONFIG_GENERATOR) -L $(LIB_DIR) -l $(LIBNAME) $(FLAGS)
	
$(DOMAIN_GENERATOR): $(DOMAIN_GENERATOR).cpp
	g++ $(DOMAIN_GENERATOR).cpp -I $(I_DIR1) -o $(DOMAIN_GENERATOR) -L $(LIB_DIR) -l $(LIBNAME) $(FLAGS)

$(EXAMPLE_OBJ): %.o : %.cpp
	g++ -c $< -I$(I_DIR1) -I$(I_DIR2) -I$(I_DIR3) $(FLAGS)
		
$(OTHER_OBJS) : %.o : %.cpp
	g++ -c $< -I$(I_DIR1) -I$(I_DIR2) -I$(I_DIR3) $(FLAGS)	 	
		
$(LIB): 
	(cd ../src; make lib) 
	
lib:
	(cd ../src; make lib) 
	
		
clean_interp_matrices:
	-rm ./data/domain/interp_matrices/all_wet/M4x4/*	
	-rm ./data/domain/interp_matrices/all_wet/SW_labels/*	

	-rm ./data/domain/interp_matrices/dry_no_GP/M3x3/*	
	-rm ./data/domain/interp_matrices/dry_no_GP/M6x6/*	
	-rm ./data/domain/interp_matrices/dry_no_GP/non_considered_corner/*	
	-rm ./data/domain/interp_matrices/dry_no_GP/SW_labels/*		
	
	-rm ./data/domain/interp_matrices/GPs/M4x4/*	
	-rm ./data/domain/interp_matrices/GPs/M8x8/*
	-rm ./data/domain/interp_matrices/GPs/SW_labels/*	
	
clean_data:
	-rm ./data/domain/*
	-rm ./data/unknowns/*
	-rm ./data/z_frames/*
	-rm ./data/z_frames_contour/*
	-rm ./data/Qx_frames/*
	-rm ./data/Qy_frames/*
	-rm ./data/time		
		
clean: clean_interp_matrices clean_data
	-rm $(EXAMPLE) 
	-rm *.o	

	
	
	
	
