
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
	-mkdir $(PROJECT_DIR)
	-mkdir $(PROJECT_DIR)/data 
	-mkdir $(PROJECT_DIR)/data/domain
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/all_wet
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/all_wet/M4x4
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/all_wet/SW_labels
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/GPs
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/GPs/M4x4
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/GPs/M8x8
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/GPs/SW_labels
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/M3x3
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/M6x6
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/non_considered_corner
	-mkdir $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/SW_labels
	-mkdir $(PROJECT_DIR)/data/unknowns
	
	-mkdir $(PROJECT_DIR)/m_files
	cp m_files/* $(PROJECT_DIR)/m_files
	cp ./input_files/* $(PROJECT_DIR)

lib: $(LIB)  
	
$(LIB):
	(cd ./src; make lib) 
			

clean_libgrid: 
	-rm ./lib/*	
	-rm ./src/*.o
	
clean_data:
	-rm $(PROJECT_DIR)/data/domain/*
	-rm	$(PROJECT_DIR)/data/domain/interp_matrices/all_wet/M4x4/*
	-rm	$(PROJECT_DIR)/data/domain/interp_matrices/all_wet/SW_labels/*
	-rm $(PROJECT_DIR)/data/domain/interp_matrices/GPs/M4x4/*
	-rm $(PROJECT_DIR)/data/domain/interp_matrices/GPs/M8x8/*
	-rm $(PROJECT_DIR)/data/domain/interp_matrices/GPs/SW_labels/*
	-rm $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/M3x3/*
	-rm $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/M6x6/*
	-rm $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/non_considered_corner/*
	-rm $(PROJECT_DIR)/data/domain/interp_matrices/dry_no_GP/SW_labels/*
	-rm $(PROJECT_DIR)/data/unknowns/*
	
clean: clean_data clean_libgrid	
	
	
		
