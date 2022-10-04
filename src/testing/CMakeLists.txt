file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
add_library(testing ${SOURCES})

include_directories(../rwnmr)
target_include_directories(testing INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(testing PUBLIC -O3)

install(TARGETS testing DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)
