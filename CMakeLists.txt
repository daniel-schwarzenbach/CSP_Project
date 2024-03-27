# set c++ 20
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)

project(Heisenberg3d)

add_subdirectory(libraries/matplotplusplus)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)



# enable vectorization
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")


# include/
include_directories(${CMAKE_SOURCE_DIR}/libraries/matplotplusplus/source)
include_directories(${CMAKE_SOURCE_DIR}/code)

# include/Ippl
#include_directories(${CMAKE_SOURCE_DIR}/libraries/Ippl/build/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/Ippl/build/ippl/src)
include_directories(${CMAKE_SOURCE_DIR}/libraries/Ippl/build/Heffte/Heffte_mixed/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/Ippl/build/Kokkos/Kokkos_mixed/include)

# lib/
link_directories(${CMAKE_SOURCE_DIR}/libraries/Ippl/build/ippl/build_mixed/src)
link_directories(${CMAKE_SOURCE_DIR}/libraries/Ippl/build/Heffte/Heffte_mixed/lib)
link_directories(${CMAKE_SOURCE_DIR}/libraries/Ippl/build/Kokkos/Kokkos_mixed/lib)

add_definitions(-DIPPL_ENABLE_TIMER_FENCES=false -DOMP_PROC_BIND=false)

# 
file(GLOB_RECURSE SRC_FILES code/*.c++ CMAKE_CONFIGURE_DEPENDS)


# add open mp
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (OMP_PROC_BIND=false and OMP_PLACES=threads)
endif()

# add MPI
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})


#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "-ldl")

# programs
add_executable(test ${SRC_FILES} ./programs/test.c++)
add_executable(Heisenberg ${SRC_FILES} ./programs/heisenberg.c++)
add_executable(IpplTest ${SRC_FILES} ./programs/test-ippl.c++)

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


target_link_libraries(IpplTest matplot)
target_link_libraries(IpplTest ippl)
target_link_libraries(IpplTest kokkoscore kokkoscontainers kokkossimd)
target_link_libraries(IpplTest heffte)
target_link_libraries(IpplTest dl)
target_link_libraries(IpplTest ${MPI_CXX_LIBRARIES})