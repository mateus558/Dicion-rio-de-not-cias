set(EXP exp)

add_executable(${EXP} experiment.cpp)
target_link_libraries(${EXP} ${LIBGNUPLOT} ${LIBJSON} ${LIBHASHDICT} ${LIBTRIEDICT})
