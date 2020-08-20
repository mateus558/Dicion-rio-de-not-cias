add_library(${LIBTRIEDICT} STATIC
                TRIEDictionary.cpp
            )

set_target_properties(${LIBTRIEDICT} PROPERTIES PUBLIC_HEADER 
                        "TRIEDictionary.hpp"
                    )
set_target_properties(${LIBTRIEDICT} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${LIBTRIEDICT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


string(TOUPPER ${LIBTRIEDICT} LIBNAME)
target_compile_definitions(${LIBTRIEDICT} PUBLIC ${LIBNAME}_VERSION=1.0)
target_compile_features(${LIBTRIEDICT} PRIVATE cxx_std_11)

target_link_libraries(${LIBTRIEDICT} ${LIBDICT})