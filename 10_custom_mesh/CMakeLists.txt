cmake_minimum_required(VERSION 3.1)
SET(CMAKE_BUILD_TYPE release)
project(mesh_decimation)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)

find_package(LIBIGL REQUIRED)

# 包含src目录
include_directories("../src/")

# 显示包含的目录
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

# 添加源文件
file(GLOB SRCFILES *.cpp ../src/*.cpp)
add_executable(${PROJECT_NAME} ${SRCFILES})

target_link_libraries(${PROJECT_NAME} igl::core igl::opengl igl::opengl_glfw)
