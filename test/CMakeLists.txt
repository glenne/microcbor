# Standard CMake header config.
cmake_minimum_required(VERSION 3.11.4)
set(CMAKE_CXX_STANDARD 11)

#==============================================================================
# Dependencies when enable_testing
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # Prevent GoogleTest from using PThreads
    set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)

    # adds the targers: gtest, gtest_main, gmock, gmock_main
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
  # Silence std::tr1 warning on MSVC
    if(MSVC)
      foreach(_tgt gtest gtest_main gmock gmock_main)
        target_compile_definitions(${_tgt}
          PRIVATE
            "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"
          )
      endforeach()
    endif()
endif()
include_directories (INTERFACE ${googletest_SOURCE_DIR}/googletest/include)

#==============================================================================
# Project definition.
project(microcbortest VERSION 0.0.1)

add_compile_options(-Wall -Wvla -Wshadow -DCONFIG_MICROCBOR_STD_VECTOR -g)
add_executable(microcbortest
               MicroCborTest.cpp
              )

# Set project target dependencies.
# This requires that the target is built and will use it as a library.
target_link_libraries( microcbortest
    # other dependencies go here
    gtest_main
)

target_include_directories(microcbortest
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${googletest_SOURCE_DIR}
)

#==============================================================================
# test config
add_test(
    NAME  microcbortest_UNIT_TEST
    COMMAND  "$<TARGET_FILE:cborTest>" --gtest_output=xml:${CMAKE_BINARY_DIR}/microcbortest_UnitTest_Results.xml
)
