cmake_minimum_required(VERSION 3.5)
if(POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif()
project(cgalpy)

# Define options
option(BUILD_SHARED_LIBS "Build shared libs instead of static libs" ON)
option(SNAPFIXT_DEF_PYBINDINGS "Define Python bindings" OFF)

# General
set(SNAPFIXT_MODULES_REL_DIR cmake/modules)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/${SNAPFIXT_MODULES_REL_DIR})

# SONAME, SOVERSION
#
# The rule is that each new release or bug fix release should increse the soversion.
#
# SOVERSION is $(SONAME_VERSION).$(SOVERSION_MINOR).$(SOVERSION_RELEASE)
#
# - If the binary interface of libraries do not change from previous release
#   (example: most bug fix releases), increase SOVERSION_RELEASE (this third number).
# - If the binary interface is changed, but remains compatible with
#   previous release (example: only addition of new functions), then increase
#   SOVERSION_MINOR (second number) and set SOVERSION_RELEASE to 0.
# - If the binary interface is changed in an incompatible way to previous
#   release, then increase the SONAME_VERSION, and set the two other
#   numbers to 0.
#
# SOVERSION history:
#   SNAPFIXT-0.1  : 0.1.0
#
set(SNAPFIXT_SONAME_VERSION "0")
set(SNAPFIXT_SOVERSION "0.1.0")
message(STATUS "SNAPFIXT_SONAME_VERSION=${SNAPFIXT_SONAME_VERSION}")
message(STATUS "SNAPFIXT_SOVERSION=${SNAPFIXT_SOVERSION}")

# Add src subdirectory
add_subdirectory(src)

include (InstallRequiredSystemLibraries)

set (CPACK_GENERATOR TGZ)
set (CPACK_PACKAGE_VENDOR "Xenia Optimal Ltd")
set (CPACK_PACKAGE_VERSION_MAJOR "0")
set (CPACK_PACKAGE_VERSION_MINOR "1")
set (CPACK_PACKAGE_VERSION_PATCH "0")
include (CPack)
