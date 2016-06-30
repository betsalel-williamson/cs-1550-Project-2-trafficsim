cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

include_directories(".")

add_subdirectory(lib)
add_subdirectory(test)

project(Main)

include_directories(${SysCallLibs_SOURCE_DIR}/include
        ${SysCallLibs_SOURCE_DIR})

add_executable(Project2_${PROJECT_NAME} trafficsim.c)
target_link_libraries(Project2_${PROJECT_NAME} SysCallLibs)

#add_executable(Single_${PROJECT_NAME} main_single.c library.c library.h iso_font.h)



