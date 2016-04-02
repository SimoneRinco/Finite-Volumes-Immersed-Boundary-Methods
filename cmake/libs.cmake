# the directory from where cmake is invoked (usally with the "cmake .." command)
set(LIB_BUILD_DIR imbo5_build CACHE STRING "fillme") 

#todo automate this reading the .tar.gz files in the libs directory
set(CEREAL_DIR cereal-1.2.2 CACHE STRING "fillme")
set(EIGEN_DIR eigen-3.2.8 CACHE STRING "fillme")
set(GOOGLE_TEST_DIR googletest-release-1.7.0 CACHE STRING "fillme")

macro(generate_lib_path dir_name_variable)
    set(${dir_name_variable}_PATH ${PROJECT_SOURCE_DIR}/libs/${${dir_name_variable}} CACHE STRING "fillme")
endmacro()

generate_lib_path(CEREAL_DIR)
generate_lib_path(EIGEN_DIR)
generate_lib_path(GOOGLE_TEST_DIR)

set(GOOGLE_TEST_LIB_DIR ${GOOGLE_TEST_DIR_PATH}/${LIB_BUILD_DIR} CACHE STRING "fillme")
set(GOOGLE_TEST_LIB_NAME libgtest.a CACHE STRING "fillme")
set(GOOGLE_TEST_LIB_PATH ${GOOGLE_TEST_LIB_DIR}/${GOOGLE_TEST_LIB_NAME} CACHE STRING "fillme")
