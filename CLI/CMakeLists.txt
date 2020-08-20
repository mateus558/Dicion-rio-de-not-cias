set(CLIEXEC news_index)

add_executable(${CLIEXEC} main.cpp)
target_link_libraries(${CLIEXEC} ${LIBJSON} ${LIBTRIEDICT} ${LIBHASHDICT})

