cmake_minimum_required(VERSION 2.6)
project(neural_terrain)

set(FANN_INCLUDE_DIR "PATH/TO/FANN/src/include" CACHE PATH "Include path for FANN")
set(FANN_LIBRARY_DIR "PATH/TO/FANN/bin" CACHE PATH "Library path for FANN")

include_directories(include ${FANN_INCLUDE_DIR})
link_directories(${FANN_LIBRARY_DIR})
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(neural_terrain ${SOURCES})
set_property(TARGET neural_terrain PROPERTY CXX_STANDARD 14)
target_link_libraries(neural_terrain debug fannfloatd)
target_link_libraries(neural_terrain optimized fannfloat)

install(TARGETS neural_terrain DESTINATION bin)
