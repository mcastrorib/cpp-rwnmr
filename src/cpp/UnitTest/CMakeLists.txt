add_library(UnitTest UnitTest.cpp)

target_include_directories(UnitTest
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )
target_compile_options(UnitTest PUBLIC -O3)
install(TARGETS UnitTest DESTINATION lib)
install(FILES UnitTest.h UnitTest_defs.h DESTINATION include)