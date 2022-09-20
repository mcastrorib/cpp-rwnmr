add_library(Math LeastSquareAdjust.cpp 
                Vector3D.cpp
                ./RNG/xorshift.cpp 
                ./Laplace/tikhonov.cpp 
                ./Laplace/src/nmrinv_core.cpp 
                ./Laplace/src/nmrinv.cpp 
                ./Laplace/src/TikhonovSolver.cpp 
                ./Laplace/src/vector_funcs.cpp) 

include_directories(./Laplace/include)
include_directories(/usr/include/eigen3)    

target_include_directories(Math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(Math PUBLIC -O3)
install(TARGETS Math DESTINATION lib)
install(FILES MathFunctions.h
              LeastSquareAdjust.h 
              Vector3D.h 
              ./RNG/mRNG.h
              ./RNG/randomIndex.h 
              ./RNG/xorshift.h
              ./Laplace/tikhonov.h 
              ./Laplace/include/nmrinv_core.h 
              ./Laplace/include/nmrinv.h 
              ./Laplace/include/TikhonovSolver.h 
              ./Laplace/include/vector_funcs.h
              DESTINATION include)

install(FILES  DESTINATION include)