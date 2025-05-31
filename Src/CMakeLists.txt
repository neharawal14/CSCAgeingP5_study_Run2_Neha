cmake_minimum_required(VERSION 3.21)

# Project information
project(TTGammaAnalysis VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# Find ROOT using modern CONFIG mode
find_package(ROOT REQUIRED CONFIG)

# Generate ROOT dictionary
root_generate_dictionary(G__Analysis
  AnalysisGasGain.h
  HistMan.h
  ChargeORIGandInstL.h
  me11.h
  nonme11_first.h
  nonme11_second.h
  LINKDEF LinkDef.h
)

# Create shared library
add_library(Analysis SHARED
  AnalysisGasGain.cxx
  HistMan.cxx
  ChargeORIGandInstL.C
  me11.C
  nonme11_first.C
  nonme11_second.C
  G__Analysis
)

# Link ROOT libraries
target_link_libraries(Analysis PUBLIC ROOT::Core ROOT::Physics ROOT::RooFit)

# Include current directory for headers
target_include_directories(Analysis PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Build main executable
add_executable(Analysis_exe main.C)
target_link_libraries(Analysis_exe Analysis)
