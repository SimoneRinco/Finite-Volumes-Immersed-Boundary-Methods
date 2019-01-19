####################################
# MAKEFILE nella cartella tutorial #
####################################

I_DIR = ../include
LIB_DIR = ../lib

LIBNAME = grid
LIB = $(LIB_DIR)/lib$(LIBNAME).a

T1 = use_grid
T2 = use_grid2
T3 = use_unknown

all:
	g++ -c  $(T1).cpp -I $(I_DIR)
	g++ -o $(T1) $(T1).o -L $(LIB_DIR) -l $(LIBNAME)
	g++ -c  $(T2).cpp -I $(I_DIR)
	g++ -o $(T2) $(T2).o -L $(LIB_DIR) -l $(LIBNAME)
	g++ -c  $(T3).cpp -I $(I_DIR)
	g++ -o $(T3) $(T3).o -L $(LIB_DIR) -l $(LIBNAME)

clean:
	-rm $(T1) $(T1).o
	-rm $(T2) $(T2).o
	-rm $(T3) $(T3).o

	-rm ./data/domain/*
	-rm ./data/domain/interp_matrices/all_wet/M4x4/*
	-rm ./data/domain/interp_matrices/all_wet/SW_labels/*
	-rm ./data/domain/interp_matrices/GPs/M4x4/*
	-rm ./data/domain/interp_matrices/GPs/M8x8/*
	-rm ./data/domain/interp_matrices/GPs/SW_labels/*
	-rm ./data/domain/interp_matrices/dry_no_GP/M3x3/*
	-rm ./data/domain/interp_matrices/dry_no_GP/M6x6/*
	-rm ./data/domain/interp_matrices/dry_no_GP/non_considered_corner/*
	-rm ./data/domain/interp_matrices/dry_no_GP/SW_labels/*
	-rm ./data/unknowns/*
