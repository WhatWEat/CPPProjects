PROJECT(Project2)
include_directories(./include)
aux_source_directory(./cpp DIR_SRCS)
add_executable(Project2 ${DIR_SRCS}  Calculator.cpp)