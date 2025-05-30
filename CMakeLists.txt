cmake_minimum_required (VERSION 3.21)

project(termhack
  VERSION 0.1.0
  LANGUAGES CXX
)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

option(TERMHACK_CLANG_TIDY "Set to ON to use clang tidy" OFF)

# Build
file(GLOB_RECURSE LIB_SOURCES
    src/*.cpp
)

add_library(${PROJECT_NAME}-static STATIC ${LIB_SOURCES})
set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

add_library(${PROJECT_NAME}-shared SHARED ${LIB_SOURCES})
set_target_properties(${PROJECT_NAME}-shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}-static
  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(${PROJECT_NAME}-shared
  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy" DOC "Path to clang-tidy executable")

if(TERMHACK_CLANG_TIDY)
  if(NOT CLANG_TIDY_EXE)
    message(FATAL_ERROR "clang-tidy requested but executable not found")
  endif()

  set_target_properties(${PROJECT_NAME}-static PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
  set_target_properties(${PROJECT_NAME}-shared PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

option(ENABLE_TESTS "Enable tests" ON) 

if(ENABLE_TESTS)
    message(STATUS "Tests enabled")
    add_subdirectory(tests)
endif()