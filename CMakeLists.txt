# set c++ 23
cmake_minimum_required(VERSION 3.16.6)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)

enable_testing()
set(DOWNLOAD_EXTRACT_TIMESTAMP true)


project(Heisenberg3d)

add_subdirectory(libraries/matplotplusplus)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)



# include files
include_directories(${CMAKE_SOURCE_DIR}/libraries/matplotplusplus/source)
include_directories(${CMAKE_SOURCE_DIR}/code)
# include Eigen
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# source files
file(GLOB_RECURSE SRC_FILES code/*.c++ CMAKE_CONFIGURE_DEPENDS)

# programs
add_executable(Plot_Lattices ${SRC_FILES} ./programs/plot/plot-lattices.c++)
add_executable(Magnetization ${SRC_FILES} ./programs/magnetization.c++)
add_executable(Plot_Speed_Up_Metro ${SRC_FILES} ./programs/omp/plot_speed_up_metropolis.c++)
add_executable(Get_Speed_Up_Data ${SRC_FILES} ./programs/omp/get_metropolis_speed_up_data.c++)
add_executable(Plot_Speed_Up_Wolff ${SRC_FILES} ./programs/omp/plot_speed_up_wolff.c++)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (OMP_PROC_BIND=false and OMP_PLACES=threads)
    add_definitions(-DWITH_OPENMP)
endif()


###################################### MPI 
option(MPI "Use MPI" OFF)
if(MPI)
    message(STATUS "MPI option is set to ${MPI}")
    set(CMAKE_CXX_COMPILER mpic++)

    ##add_definitions(-DIPPL_ENABLE_TIMER_FENCES=false -DOMP_PROC_BIND=false)

    # add open mp
    find_package(OpenMP REQUIRED)
    if (OPENMP_FOUND)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set (OMP_PROC_BIND=false and OMP_PLACES=threads)
        add_definitions(-DWITH_OPENMP)
    endif()

    # add MPI
    find_package(MPI REQUIRED)
    include_directories(${MPI_INCLUDE_PATH})
    #include_directories(SYSTEM ${MPI_INCLUDE_PATH})

    # programs
    add_executable(MPI_Seed ${SRC_FILES}
        ./programs/MPI/MPI_seed.c++)
    add_executable(HeisenbergMPI ${SRC_FILES}
        ./programs/MPI/HeisenbergMPI.c++)
    add_executable(MPI_Speed_Up ${SRC_FILES}
        ./programs/MPI/MPI_metropolis_speed_up.c++)
    add_executable(Lattice4MPI ${SRC_FILES}
        ./programs/MPI/Lattice4MPI.c++)

    # lib/
    set(LIBRARIES
    matplot
    Eigen3::Eigen
    MPI::MPI_CXX
    dl
    )

    # link libraries
    target_link_libraries(MPI_Speed_Up ${LIBRARIES})
    target_link_libraries(HeisenbergMPI ${LIBRARIES})
    target_link_libraries(MPI_Seed ${LIBRARIES})
    target_link_libraries(Lattice4MPI ${LIBRARIES})
endif()


if(NOT MPI)# without MPI and IPPL
message(STATUS "compiling gtests")
    # google-tests
    add_executable(Test_Lattice ${SRC_FILES} ./programs/gtests/test-latttice.c++)
    set_target_properties(Test_Lattice PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gtests"
    )
    add_executable(Test_Algorithm ${SRC_FILES} ./programs/gtests/test-algorithm.c++)
    set_target_properties(Test_Algorithm PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gtests"
    )
    add_executable(Test_Data ${SRC_FILES} ./programs/gtests/test-data.c++)
    set_target_properties(Test_Data PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gtests"
    )

    # get all libraries
    set(LIBRARIES
    dl
    matplot
    Eigen3::Eigen
    )

    # google test stuff
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    include(GoogleTest)
    gtest_discover_tests(Test_Lattice)
    gtest_discover_tests(Test_Algorithm)
    gtest_discover_tests(Test_Data)

    # link libraries for test
    target_link_libraries(Test_Lattice ${LIBRARIES} gtest_main)
    target_link_libraries(Test_Algorithm ${LIBRARIES} gtest_main)
    target_link_libraries(Test_Data ${LIBRARIES} gtest_main)
endif()

add_definitions(-DDEBUG)




# link libraries
target_link_libraries(Plot_Lattices ${LIBRARIES})
target_link_libraries(Magnetization ${LIBRARIES})
target_link_libraries(Plot_Speed_Up_Wolff ${LIBRARIES})
target_link_libraries(Get_Speed_Up_Data ${LIBRARIES})
target_link_libraries(Plot_Speed_Up_Metro ${LIBRARIES})



set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



# export compile commands for intellisense
