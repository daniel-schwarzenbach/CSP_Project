# set c++ 20
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)

project(Heisenberg3d)

add_subdirectory(matplotplusplus)

set(CMAKE_BUILD_TYPE Debug)


# enable vectorization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")


# include/
include_directories(${CMAKE_SOURCE_DIR}/include)
# include_directories(${CMAKE_SOURCE_DIR}/include/Ippl)
# include_directories(${CMAKE_SOURCE_DIR}/include/Heffte)
# include_directories(${CMAKE_SOURCE_DIR}/include/Kokkos)
# lib/
link_directories(${CMAKE_SOURCE_DIR}/lib)



# add open mp
# find_package(OpenMP REQUIRED)
# if (OPENMP_FOUND)
#     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
# endif()


#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "-ldl")

add_executable(test code/test/test.c++)
add_executable(Heisenberg code/main.c++)

# lib/
# target_link_libraries(test heffte)
# target_link_libraries(test ippl)
# target_link_libraries(test kokkoscore)
# target_link_libraries(test kokkoscontainers)
# target_link_libraries(test kokkossimd)
target_link_libraries(test dl)
target_link_libraries(test matplot)
target_link_libraries(Heisenberg dl)
target_link_libraries(Heisenberg matplot)

# add MPI
#find_package(MPI REQUIRED)
#include_directories(SYSTEM ${MPI_INCLUDE_PATH})
#target_link_libraries(test ${MPI_CXX_LIBRARIES})