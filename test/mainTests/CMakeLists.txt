include_directories(${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        ${SysCallLibs_SOURCE_DIR}/include
        ${SysCallLibs_SOURCE_DIR})

add_executable(TestSuite
        mother.cpp
        library_tests.cpp)

target_link_libraries(TestSuite SysCallLibs gtest gtest_main)

add_test(NAME TestSuite
        COMMAND TestSuite)