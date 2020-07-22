cmake_minimum_required(VERSION 3.5)
project(scopeTest)
set(Boost_USE_STATIC_LIBS   ON)
find_package(PythonLibs ${PY_VERSION} REQUIRED)
find_package(Boost 1.71.0 REQUIRED COMPONENTS system python)
# include directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

# create the lib
add_library(scopeTest SHARED src/bindings.cpp)

set_target_properties(scopeTest PROPERTIES SUFFIX ".pyd")

# link
target_link_libraries(scopeTest ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})