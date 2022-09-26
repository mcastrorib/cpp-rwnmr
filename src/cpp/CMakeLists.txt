cmake_minimum_required(VERSION 3.17 FATAL_ERROR)


# set the project name
project(RWNMR VERSION 1.1 LANGUAGES C CXX CUDA)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# specify mpi compilers
SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(RWNMR_Config.h.in RWNMR_Config.h)

# Require MPI package
find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(MPI_INCLUDE_PATH)

# include OpenCV package
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# include CUDA package
find_package(CUDAToolkit REQUIRED)

# include OpenMP package
find_package( OpenMP REQUIRED )

if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

#include Eigen package
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(/usr/include/eigen3)


# add and list libraries (it must respect libs building order)
add_subdirectory(UnitTest)
add_subdirectory(App)
add_subdirectory(Framework)
add_subdirectory(Math)
add_subdirectory(ConfigFiles)
add_subdirectory(Utils)
list(APPEND EXTRA_LIBS UnitTest App Framework Math ConfigFiles Utils)


# add the executable
add_executable(rwnmr main.cpp)
target_compile_options(rwnmr PUBLIC -O3)
target_link_libraries(rwnmr PUBLIC ${EXTRA_LIBS} ${MPI_LIBRARIES} ${OpenCV_LIBS} OpenMP::OpenMP_CXX Eigen3::Eigen)
target_link_libraries(rwnmr PRIVATE CUDA::cudart)


# add the binary tree to the search path for include files
# so that we will find RandomWalkConfig.h
target_include_directories(rwnmr PUBLIC "${PROJECT_BINARY_DIR}")


# Testing
# add the executable
add_executable(test-rwnmr testing.cpp)
target_compile_options(test-rwnmr PUBLIC -O3)
target_link_libraries(test-rwnmr PUBLIC ${EXTRA_LIBS} ${MPI_LIBRARIES} ${OpenCV_LIBS} OpenMP::OpenMP_CXX Eigen3::Eigen)
target_link_libraries(test-rwnmr PRIVATE CUDA::cudart)


# add the binary tree to the search path for include files
# so that we will find RandomWalkConfig.h
target_include_directories(test-rwnmr PUBLIC "${PROJECT_BINARY_DIR}")

# Installation
install(TARGETS rwnmr DESTINATION bin)
install(TARGETS test-rwnmr DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/RWNMR_Config.h"
  DESTINATION include
  )

# Enabling testing
enable_testing()

# does the application run
add_test(NAME Runs COMMAND rwnmr)
add_test(NAME CPMG-smooth-run COMMAND rwnmr cpmg)
add_test(NAME PFGSE-smooth-run COMMAND rwnmr pfgse)
add_test(NAME MultiTau-smooth-run COMMAND rwnmr multitau)
add_test(NAME ArgsParserTest COMMAND test-rwnmr argsparser)
add_test(NAME ConfigFilesTest COMMAND test-rwnmr configfiles)