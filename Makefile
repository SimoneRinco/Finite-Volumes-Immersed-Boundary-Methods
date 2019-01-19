######################################################################
# INSERIRE QUI IL NOME DELLA CARTELLA CHE CONTIENE IL PROPRIO PROGETTO
PROJECT_DIR = ./Shallow_waters
######################################################################

I_DIR = ./include

SRC_DIR = ./src
LIB_DIR = ./lib

LIBNAME = libgrid.a
LIB = $(LIB_DIR)/$(LIBNAME)


all:
	make dirs
	make lib

dirs:
	mkdir -p $(PROJECT_DIR)
	mkdir -p $(PROJECT_DIR)/data
	mkdir -p $(PROJECT_DIR)/data/domain
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/all_wet
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/all_wet/M4x4
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/all_wet/SW_labels
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/GPs
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/GPs/M4x4
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/GPs/M8x8
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/GPs/SW_labels
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/M3x3
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/M6x6
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/non_considered_corner
	mkdir -p $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/SW_labels
	mkdir -p $(PROJECT_DIR)/data/unknowns

	mkdir -p $(PROJECT_DIR)/m_files
	cp m_files/* $(PROJECT_DIR)/m_files
	cp ./input_files/* $(PROJECT_DIR)

lib: $(LIB)

$(LIB):
	(cd ./src; make lib)


clean_libgrid:
	-rm -rf ./lib/*
	-rm -rf ./src/*.o

clean_data:
	-rm -rf $(PROJECT_DIR)/data/domain/*
	-rm -rf $(PROJECT_DIR)/data/domain/interp_matrices/all_wet/M4x4/*
	-rm -rf $(PROJECT_DIR)/data/domain/interp_matrices/all_wet/SW_labels/*
	-rm -rf $(PROJECT_DIR)/data/domain/interp_matrices/GPs/M4x4/*
	-rm -rf $(PROJECT_DIR)/data/domain/interp_matrices/GPs/M8x8/*
	-rm -rf $(PROJECT_DIR)/data/domain/interp_matrices/GPs/SW_labels/*
	-rm -rf $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/M3x3/*
	-rm -rf $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/M6x6/*
	-rm -rf $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/non_considered_corner/*
	-rm -rf $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/SW_labels/*
	-rm -rf $(PROJECT_DIR)/data/unknowns/*

clean: clean_data clean_libgrid
