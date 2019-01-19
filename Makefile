PROJECT_DIR = ./Shallow_waters

I_DIR = ./include

SRC_DIR = ./src
LIB_DIR = ./lib

LIBNAME = libgrid.a
LIB = $(LIB_DIR)/$(LIBNAME)

all:
	make dirs
	make lib

dirs:
	./setup_dirs.sh $(PROJECT_DIR)

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
	-rm -rf $(PROJECT_DIR)/data

clean: clean_data clean_libgrid
