set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS iostreams system filesystem) 
find_package(Gnuplot)

message(STATUS "Boost version: ${Boost_VERSION}")

if(Boost_FOUND AND GNUPLOT_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 

    add_library(${LIBGNUPLOT} STATIC
                    gnuplot-iostream.h
                )

    set_target_properties(${LIBGNUPLOT} PROPERTIES PUBLIC_HEADER 
                            "gnuplot-iostream.h"
                        )
    set_target_properties(${LIBGNUPLOT} PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(${LIBGNUPLOT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


    string(TOUPPER ${LIBGNUPLOT} LIBNAME)
    target_compile_definitions(${LIBGNUPLOT} PUBLIC ${LIBNAME}_VERSION=1.0)
    target_compile_features(${LIBGNUPLOT} PRIVATE cxx_std_17)
    target_link_libraries(${LIBGNUPLOT} ${Boost_LIBRARIES})
endif()

add_library(${LIBJSON} STATIC
                json.hpp
                utils.cpp
            )

set_target_properties(${LIBJSON} PROPERTIES PUBLIC_HEADER 
                        "json.hpp;utils.hpp"
                    )
set_target_properties(${LIBJSON} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${LIBJSON} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

string(TOUPPER ${LIBJSON} LIBNAME)
target_compile_definitions(${LIBJSON} PUBLIC ${LIBNAME}_VERSION=1.0)
target_compile_features(${LIBJSON} PRIVATE cxx_std_17)
