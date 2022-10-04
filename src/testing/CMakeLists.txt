file(GLOB_RECURSE SOURCES ./*.cpp)
file(GLOB_RECURSE HEADERS ./*.h)
add_library(testing ${SOURCES})
include_directories(../rwnmr)
target_include_directories(testing
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )

target_compile_options(testing PUBLIC -O3)

install(TARGETS testing DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)
