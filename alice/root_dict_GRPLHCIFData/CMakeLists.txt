# 2026-11-26
# https://root.cern/manual/integrate_root_into_my_cmake_project/
# https://root.cern/manual/io_custom_classes/#using-cmake
# https://github.com/root-project/root/tree/master/tutorials/io/tree/dictionary
# https://cliutils.gitlab.io/modern-cmake/root/

# cmake -S . -B build/ # -Wdev -Wdeprecated --warn-uninitialized
# cmake -S . -B build/ -DCMAKE_BUILD_TYPE=Release   # -O3 -DNDEBUG
# cmake -S . -B build/ -DWITH_BunchFilling_cxx=OFF
# cmake --build build/ --verbose

cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(GRPLHCIFData LANGUAGES CXX)

find_package(ROOT REQUIRED COMPONENTS Core)

# https://root.cern/doc/v610/release-notes.html#build-configuration-and-testing-infrastructure (2017-06)
# https://root.cern/doc/v620/release-notes.html#build-configuration-and-testing-infrastructure (2020-04)
# include(${ROOT_USE_FILE}) # legacy

# # To ensure compatibility between ROOT's C++ interpreter, Cling, and compiled
# # code, your application (target in CMake) must be compiled with the same
# # C++ standard with which ROOT was compiled.
# #
# # root/cmake/ROOTConfig.cmake  => set(ROOT_CXX_STANDARD 20)
# # root/cmake/ROOTUseFile.cmake => set(CMAKE_CXX_STANDARD ${ROOT_CXX_STANDARD})
# set(CMAKE_CXX_STANDARD ${ROOT_CXX_STANDARD})
# set(CMAKE_CXX_STANDARD 23)
# message(NOTICE "ROOT_CXX_STANDARD =  ${ROOT_CXX_STANDARD}")
# message(NOTICE "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")
# set(CMAKE_CXX_STANDARD_REQUIRED ON) # enforce the specified C++ standard
# set(CMAKE_CXX_EXTENSIONS OFF) # compiler specific extensions (gnu++ vs c++)
# #
# # CMake, resp. ROOTConfig.cmake automaticky nastavi potrebny C++ standard.
# #
# # POZOR
# # https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
# # CMake interpretuje CXX_STANDARD ako pozadovany minimalny C++ standard,
# # nie ako presny prikaz pre dany standard. Ak kompilator uz splna pozadovany
# # C++ standard, nemusi pridat ziaden '-std=' parameter navyse.

option(WITH_BunchFilling_cxx "Build shared library including BunchFilling.cxx" OFF)
message(STATUS ">>> With BunchFilling.cxx: ${WITH_BunchFilling_cxx}")
message(STATUS ">>> Build type: ${CMAKE_BUILD_TYPE}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/AliceO2_GRPLHCIFData)

set(DICT_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/AliceO2_GRPLHCIFData/GRPLHCIFData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AliceO2_GRPLHCIFData/BunchFilling.h)

ROOT_GENERATE_DICTIONARY(G__${PROJECT_NAME}.only.dict ${DICT_HDRS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h)

set(LIB_SRCS G__${PROJECT_NAME}.only.dict.cxx) # library with dictionaries only
if(WITH_BunchFilling_cxx) # library with dictionaries and BunchFilling.cxx (specific case)
  list(PREPEND LIB_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/AliceO2_GRPLHCIFData/BunchFilling.cxx)
endif()

add_library(${PROJECT_NAME}.only.dict SHARED ${LIB_SRCS})
target_link_libraries(${PROJECT_NAME}.only.dict PUBLIC ROOT::Core)
