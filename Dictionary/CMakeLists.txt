add_library(${LIBDICT} STATIC 
		Dictionary.cpp
)

set_target_properties(${LIBDICT} PROPERTIES PUBLIC_HEADERS
				"Dictionary.hpp; Document.hpp; DictNode.hpp"
			)

set_target_properties(${LIBDICT} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${LIBDICT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

string(TOUPPER ${LIBDICT} LIBNAME)
target_compile_definitions(${LIBDICT} PUBLIC ${LIBNAME}_VERSION=1.0)
target_compile_features(${LIBDICT} PRIVATE cxx_std_11)
target_link_libraries(${LIBDICT} ${LIBJSON} ${LIBGNUPLOT})

add_subdirectory(TRIEDictionary)
add_subdirectory(HashDictionary)