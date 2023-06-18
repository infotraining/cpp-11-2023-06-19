# Set VCPKG_ROOT to your vcpkg installation directory or add the following to your cmake call:
# -DCMAKE_TOOLCHAIN=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake

cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")  
endif()
if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

project(cpp-modern LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

########################################################################
# Catch2
find_package(Catch2 3)

if (NOT Catch2_FOUND)
  Include(FetchContent)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.3.2 # or a later release
  )

  FetchContent_MakeAvailable(Catch2)
endif()

enable_testing()

add_test(AllTests ...)

add_subdirectory(nullptr)
add_subdirectory(enums)
add_subdirectory(auto-decltype)
add_subdirectory(class-features)
add_subdirectory(move-semantics)
add_subdirectory(range-based-for)
add_subdirectory(smart-pointers)
add_subdirectory(string-literals)
add_subdirectory(tuples)
add_subdirectory(uniform-initialization)
add_subdirectory(variadic-templates)

add_subdirectory(_exercises/find-null-ex)
add_subdirectory(_exercises/auto-declarations-ex)



