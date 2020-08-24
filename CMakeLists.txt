cmake_minimum_required(VERSION 3.14)
project(DOCS_INDEXING VERSION 1.0)

set(LIBDICT dictionary)
set(LIBTRIEDICT trie_dictionary)
set(LIBHASHDICT hash_dictionary)
set(LIBJSON jsonparser)
set(LIBGNUPLOT gp_wrapper)

message(STATUS "Documents indexing and search Version: ${PROJECT_VERSION}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wno-multichar -pedantic-errors -Wall -Wextra -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O1")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

add_subdirectory(Utils)
add_subdirectory(Dictionary)
add_subdirectory(Experiments)
add_subdirectory(CLI)
add_executable(teste main.cpp)
target_link_libraries(teste ${LIBGNUPLOT} ${LIBJSON} ${LIBHASHDICT} ${LIBTRIEDICT})