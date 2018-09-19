project(zhminer)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") #  -Wall

## Enable solvers here
option(USE_CPU_TROMP "USE CPU_TROMP" ON)
option(USE_CUDA_TROMP "USE CUDA_TROMP" ON)

## Add solvers here
if (USE_CPU_TROMP)
    add_definitions(-DUSE_CPU_TROMP)
    message("-- USE_CPU_TROMP DEFINED")
endif()
if (USE_CUDA_TROMP)
    add_definitions(-DUSE_CUDA_TROMP)
    message("-- USE_CUDA_TROMP DEFINED")
endif()


########
# LINUX
if(CMAKE_COMPILER_IS_GNUCXX)
#    # use native cpu features
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -fPIC")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -fPIC")

#    # optimizations
#    add_definitions(-O3)

    # use
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64 -msse2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -msse2")
    # optimizations
    add_definitions(-O2)
endif()

# Common
include_directories(${zhminer_SOURCE_DIR}/zhminer)

# BOOST
#find_package(Threads REQUIRED COMPONENTS)
# compile boost staticaly
set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#set(BUILD_SHARED_LIBRARIES OFF)
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
find_package(Boost REQUIRED COMPONENTS system log_setup log date_time filesystem thread)

if (Boost_FOUND)
  # From the offical documentation:
  # Add include directories to the build. [...] If the SYSTEM option is given,
  # the compiler will be told the directories are meant as system include
  # directories on some platforms (signalling this setting might achieve effects
  # such as the compiler skipping warnings [...])."
  include_directories (SYSTEM ${Boost_INCLUDE_DIR})

  # From the offical documentation:
  # "Specify directories in which the linker will look for libraries. [...] Note
  # that this command is rarely necessary. Library locations returned by
  # find_package() and find_library() are absolute paths. Pass these absolute
  # library file paths directly to the target_link_libraries() command. CMake
  # will ensure the linker finds them."
  link_directories (${Boost_LIBRARY_DIRS})
else()
    message("Boost_FOUND NOT FOUND")
endif ()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

set(SOURCE_FILES
    # sources
    zhminer/amount.cpp
    zhminer/api.cpp
    zhminer/arith_uint256.cpp
    zhminer/crypto/sha256.cpp
    zhminer/json/json_spirit_reader.cpp
    zhminer/json/json_spirit_value.cpp
    zhminer/json/json_spirit_writer.cpp
    zhminer/libstratum/ZcashStratum.cpp
    zhminer/main.cpp
    zhminer/primitives/block.cpp
    zhminer/speed.cpp
    zhminer/uint256.cpp
    zhminer/utilstrencodings.cpp
    # headers
    zhminer/amount.h
    zhminer/api.hpp
    zhminer/arith_uint256.h
    zhminer/crypto/sha256.h
    zhminer/hash.h
    zhminer/json/json_spirit.h
    zhminer/json/json_spirit_error_position.h
    zhminer/json/json_spirit_reader.h
    zhminer/json/json_spirit_reader_template.h
    zhminer/json/json_spirit_stream_reader.h
    zhminer/json/json_spirit_utils.h
    zhminer/json/json_spirit_value.h
    zhminer/json/json_spirit_writer.h
    zhminer/json/json_spirit_writer_template.h
    zhminer/libstratum/StratumClient.cpp
    zhminer/libstratum/StratumClient.h
    zhminer/libstratum/ZcashStratum.cpp
    zhminer/libstratum/ZcashStratum.h
    zhminer/primitives/block.h
    zhminer/primitives/transaction.h
    zhminer/script/script.h
    zhminer/serialize.h
    zhminer/speed.hpp
    zhminer/streams.h
    zhminer/support/allocators/zeroafterfree.h
    zhminer/tinyformat.h
    zhminer/uint252.h
    zhminer/uint256.h
    zhminer/utilstrencodings.h
    zhminer/version.h
    zhminer/zcash/JoinSplit.hpp
    zhminer/zcash/NoteEncryption.hpp
    zhminer/zcash/Proof.hpp
    zhminer/zcash/Zcash.h
    zhminer/SolverStub.h # just a stub

    zhminer/AvailableSolvers.h
    zhminer/ISolver.h
    zhminer/Solver.h
    zhminer/MinerFactory.h
    zhminer/MinerFactory.cpp

    # make same path on windows
    #blake shared
    # src
    blake2/blake2bx.cpp
    # headers
    blake2/blake2.h
    blake2/blake2b-load-sse2.h
    blake2/blake2b-load-sse41.h
    blake2/blake2b-round.h
    blake2/blake2-config.h
    blake2/blake2-impl.h
    blake2/blake2-round.h
    )

#set(LIBS ${LIBS} ${Threads_LIBRARIES} ${Boost_LIBRARIES})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

message("-- CXXFLAGS: ${CMAKE_CXX_FLAGS}")
message("-- LIBS: ${LIBS}")

if (USE_CPU_TROMP)
    add_subdirectory(cpu_tromp)
endif()
if (USE_CUDA_TROMP)
    add_subdirectory(cuda_tromp)
endif()

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

#target_link_libraries(${PROJECT_NAME} ${LIBS} ${CUDA_LIBRARIES} )
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${LIBS} )

# link libs
if (USE_CPU_TROMP)
    target_link_libraries(${PROJECT_NAME} cpu_tromp)
endif()
if (USE_CUDA_TROMP)
    target_link_libraries(${PROJECT_NAME} cuda_tromp)
endif()

