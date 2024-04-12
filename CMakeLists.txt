# set c++ 23
cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 23)


project(Heisenberg3d)

add_subdirectory(libraries/matplotplusplus)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)



# include files
include_directories(${CMAKE_SOURCE_DIR}/libraries/matplotplusplus/source)
include_directories(${CMAKE_SOURCE_DIR}/code)
# include Eigen
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# source files
file(GLOB_RECURSE SRC_FILES code/*.c++ CMAKE_CONFIGURE_DEPENDS)

# programs
add_executable(Heisenberg ${SRC_FILES} ./programs/heisenberg.c++)

# precompile header
target_precompile_headers(Heisenberg
    PUBLIC
    code/LatticeSerial.h++
)

# IPPL STUFF
option(IPPL "Use IPPL" OFF)
if(IPPL)
    message(STATUS "IPPL option is set to ${IPPL}")
    set(CMAKE_CXX_COMPILER mpic++)
    # include/ippl
    include_directories(${CMAKE_SOURCE_DIR}/libraries/Ippl/build/include/)
    include_directories(${CMAKE_SOURCE_DIR}/libraries/Ippl/build/Heffte/Heffte_omp/include)
    include_directories(${CMAKE_SOURCE_DIR}/libraries/Ippl/build/Kokkos/Kokkos_omp/include)

    add_definitions(-DIPPL_ENABLE_TIMER_FENCES=false -DOMP_PROC_BIND=false)

    # lib/ippl
    set(IPPL_PATH ${CMAKE_SOURCE_DIR}/libraries/Ippl/build/lib/)
    set(HEFFTE_PATH ${CMAKE_SOURCE_DIR}/libraries/Ippl/build/Heffte/Heffte_omp/lib/)
    set(KOKKOS_PATH ${CMAKE_SOURCE_DIR}/libraries/Ippl/build/Kokkos/Kokkos_omp/lib/)

    find_library(IPPL_LIBRARY ippl PATHS ${IPPL_PATH})
    find_library(KOKKOS_CORE_LIBRARY kokkoscore PATHS ${KOKKOS_PATH})
    find_library(KOKKOS_CONTAINERS_LIBRARY kokkoscontainers PATHS ${KOKKOS_PATH})
    find_library(KOKKOS_SIMD_LIBRARY kokkossimd PATHS ${KOKKOS_PATH})
    find_library(HEFFTE_LIBRARY heffte PATHS ${HEFFTE_PATH})
   

    # add open mp
    find_package(OpenMP REQUIRED)
    if (OPENMP_FOUND)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set (OMP_PROC_BIND=false and OMP_PLACES=threads)
    endif()

    # add MPI
    find_package(MPI REQUIRED)
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})

    # programs
    add_executable(HeisenbergIppl ${SRC_FILES} 
        ./programs/heisenbergIppl.c++)
    # add_executable(IpplTest ${SRC_FILES} ./programs/test-ippl.c++)

    # precompile header
    target_precompile_headers(HeisenbergIppl
        PUBLIC
        code/LatticeIppl.h++
    )

    # lib/
    set(LIBRARIES
    matplot
    Eigen3::Eigen
    ${MPI_CXX_LIBRARIES}
    ${IPPL_LIBRARY}
    ${KOKKOS_CORE_LIBRARY}
    ${KOKKOS_CONTAINERS_LIBRARY}
    ${KOKKOS_SIMD_LIBRARY}
    ${HEFFTE_LIBRARY}
    dl
    )

    # link libraries
    target_link_libraries(HeisenbergIppl ${LIBRARIES})
    # target_link_libraries(IpplTest ${LIBRARIES})

else()# without IPPL

    # get all libraries
    set(LIBRARIES
    dl
    matplot
    Eigen3::Eigen
    )

endif()

# link libraries
target_link_libraries(Heisenberg ${LIBRARIES})

# export compile commands for intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)