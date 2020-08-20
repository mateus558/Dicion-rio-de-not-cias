add_library(${LIBHASHDICT} STATIC
            HashDictionary.cpp
)

set_target_properties(${LIBHASHDICT} PROPERTIES PUBLIC_HEADER 
                        "HashDictionary.hpp"
                    )
set_target_properties(${LIBHASHDICT} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${LIBHASHDICT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

string(TOUPPER ${LIBHASHDICT} LIBNAME)
target_compile_definitions(${LIBHASHDICT} PUBLIC ${LIBNAME}_VERSION=1.0)
target_compile_features(${LIBHASHDICT} PRIVATE cxx_std_11)
target_link_libraries(${LIBHASHDICT} ${LIBDICT})