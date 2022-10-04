file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE CUDA_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*_cuda.*") # Always indicate CUDA files by "_cuda" sufix before format (e.g. mysrcfile_cuda.cxx or myheaderfile_cuda.h)
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}./*.h")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
add_library(rwnmr ${SOURCES})

include_directories(math/Laplace/include)
include_directories(/usr/include/eigen3)    
target_include_directories(rwnmr INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Request the library to be built with -std=c++11
# As this is public compile feature, anything that links to
# This lib will also be build with -std=c++11
target_compile_features(rwnmr PUBLIC cxx_std_11)

# Explicitly state that all CUDA files in the 
# library needs to be built with -dc as the member functions 
# could be called by other libraries and executables
set(CMAKE_CUDA_FLAGS "-Xcompiler -fopenmp -O3 -w -Wno-deprecated-gpu-targets")
foreach(CUDA_SRC ${CUDA_SRC_FILES})
    set_source_files_properties(${CUDA_SRC} PROPERTIES LANGUAGE "CUDA")
endforeach()

# Set CUDA architecture and link to CUDA runtime environment
# For better results, add current device architecture
find_package(CUDA)
if (CUDA_FOUND)
  #Get CUDA compute capability
  set(OUTPUTFILE ${CMAKE_CURRENT_SOURCE_DIR}/utils/cudaCheck/cuda_script) # No suffix required
  set(CUDAFILE ${CMAKE_CURRENT_SOURCE_DIR}/utils/cudaCheck/check_cuda.cu)
  execute_process(COMMAND nvcc -lcuda ${CUDAFILE} -o ${OUTPUTFILE})
  execute_process(COMMAND ${OUTPUTFILE}
                  RESULT_VARIABLE CUDA_RETURN_CODE
                  OUTPUT_VARIABLE ARCH)
  execute_process(COMMAND rm ${OUTPUTFILE})

  if(${CUDA_RETURN_CODE} EQUAL 0)
    set(CUDA_SUCCESS "TRUE")
  else()
    set(CUDA_SUCCESS "FALSE")
  endif()

  if (${CUDA_SUCCESS})
    message(STATUS "CUDA Architecture: ${ARCH}")
    message(STATUS "CUDA Version: ${CUDA_VERSION_STRING}")
    message(STATUS "CUDA Path: ${CUDA_TOOLKIT_ROOT_DIR}")
    set(CUDA_NVCC_FLAGS "${ARCH}")
	  set_property(TARGET rwnmr PROPERTY CUDA_ARCHITECTURES "${ARCH}")
    add_definitions(-DGPU) #You may not require this
  
  else()
    message(WARNING ${ARCH})

  endif()
endif()

target_compile_options(rwnmr PUBLIC -O3)
target_link_libraries(rwnmr PRIVATE CUDA::cudart)

install(TARGETS rwnmr DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)