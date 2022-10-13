# CMake version specification
cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# Build
# Set the project name
project(RWNMR VERSION 1.1 LANGUAGES C CXX CUDA)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Configure a header file to pass some of the CMake settings to the source code
configure_file(config.h.in config.h)

# Includes
# Include OpenCV package
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Include CUDA package
find_package(CUDAToolkit REQUIRED)

# Include OpenMP package
find_package( OpenMP REQUIRED )
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Include Eigen package
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(/usr/include/eigen3)

# Add and list libraries (respecting libs building order)
add_subdirectory(testing)
add_subdirectory(rwnmr)
list(APPEND EXTRA_LIBS rwnmr)

# Add the executable
add_executable(rwnmr-app rwnmr/main.cpp)
target_compile_options(rwnmr-app PUBLIC -O3)
target_link_libraries(rwnmr-app PUBLIC ${EXTRA_LIBS} ${OpenCV_LIBS} OpenMP::OpenMP_CXX Eigen3::Eigen)
target_link_libraries(rwnmr-app PRIVATE CUDA::cudart)

# Add the binary tree to the search path for include files so that it will find RandomWalkConfig.h
target_include_directories(rwnmr PUBLIC "${PROJECT_BINARY_DIR}")

## Testing
# Add the test executable
list(APPEND TEST_LIBS testing ${EXTRA_LIBS})
add_executable(rwnmr-test testing/test.cpp)
target_compile_options(rwnmr-test PUBLIC -O3)
target_link_libraries(rwnmr-test PUBLIC ${TEST_LIBS} ${OpenCV_LIBS} OpenMP::OpenMP_CXX Eigen3::Eigen)
target_link_libraries(rwnmr-test PRIVATE CUDA::cudart)

# Add the binary tree to the search path for include files so that it will find RandomWalkConfig.h
target_include_directories(rwnmr-test PUBLIC "${PROJECT_BINARY_DIR}")

# Enabling testing
enable_testing()

# List test commands
add_test(NAME Runs COMMAND rwnmr-app -rwconfig ./input/testing/trwnmr.config -uctconfig ./input/testing/tuct.config)
add_test(NAME CPMG-smooth-run COMMAND rwnmr-app -rwconfig ./input/testing/trwnmr.config -uctconfig ./input/testing/tuct.config cpmg -config ./input/testing/tcpmg.config)
add_test(NAME CPMG_uniform_gradient-smooth-run COMMAND rwnmr-app cpmg -config ./input/testing/tcpmg_uniform_field.config)
add_test(NAME PFGSE-smooth-run COMMAND rwnmr-app pfgse -config ./input/testing/tpfgse.config)
add_test(NAME MultiTau-smooth-run COMMAND rwnmr-app multitau -config ./input/testing/tmultitau.config ./input/testing/tcpmg_uniform_field.config)
add_test(NAME ArgsParser-test COMMAND rwnmr-test ArgsParser)
add_test(NAME RwnmrConfig-test COMMAND rwnmr-test RwnmrConfig)
add_test(NAME UctConfig-test COMMAND rwnmr-test UctConfig)
add_test(NAME CpmgConfig-test COMMAND rwnmr-test CpmgConfig)
add_test(NAME PfgseConfig-test COMMAND rwnmr-test PfgseConfig)
add_test(NAME MultitauConfig-test COMMAND rwnmr-test MultitauConfig)
# add_test(NAME ConfigFilesTest COMMAND rwnmr-test configfiles)

## Installation
install(TARGETS rwnmr-app DESTINATION bin)
install(TARGETS rwnmr-test DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)