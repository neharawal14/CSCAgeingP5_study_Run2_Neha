cmake_minimum_required(VERSION 3.21)
#cmake_minimum_required(VERSION 3.19)

#Project name
project(TTGammaAnalysis VERSION 1.0)
#C++ Standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(CMAKE_PREFIX_PATH "/cvmfs/cms.cern.ch/el8_amd64_gcc12/cms/cmssw/CMSSW_13_3_0/external/el8_amd64_gcc12/bin/root")
# This package uses ROOT
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(ROOT CONFIG REQUIRED)


#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# This is required for ROOT < 6.16
string(REPLACE "-L " "-L" ROOT_EXE_LINKER_FLAGS "${ROOT_EXE_LINKER_FLAGS}")

# This is required on if there is more than one flag 
separate_arguments(ROOT_EXE_LINKER_FLAGS)

root_generate_dictionary(G__Analysis 
	#${CMAKE_CURRENT_SOURCE_DIR}/AnalysisGasGain.h
	${CMAKE_CURRENT_SOURCE_DIR}/old_AnalysisGasGain.h
  ${CMAKE_CURRENT_SOURCE_DIR}/HistMan.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ChargeORIGandInstL.h
	${CMAKE_CURRENT_SOURCE_DIR}/me11.h
	${CMAKE_CURRENT_SOURCE_DIR}/nonme11_first.h
	${CMAKE_CURRENT_SOURCE_DIR}/nonme11_second.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/pressurecsc_2016.h
 	# ${CMAKE_CURRENT_SOURCE_DIR}/pressurecsc_2017.h
 	# ${CMAKE_CURRENT_SOURCE_DIR}/pressurecsc_2018.h
 	# ${CMAKE_CURRENT_SOURCE_DIR}/IntegrateLumi_2016.h
 	# ${CMAKE_CURRENT_SOURCE_DIR}/IntegrateLumi_2017.h
 	# ${CMAKE_CURRENT_SOURCE_DIR}/IntegrateLumi_2018.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h
  )

add_library(Analysis SHARED 
	#${CMAKE_CURRENT_SOURCE_DIR}/AnalysisGasGain.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/old_AnalysisGasGain.cxx
	#${CMAKE_CURRENT_SOURCE_DIR}/old_code.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/HistMan.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/ChargeORIGandInstL.C
	${CMAKE_CURRENT_SOURCE_DIR}/me11.C
	${CMAKE_CURRENT_SOURCE_DIR}/nonme11_first.C
	${CMAKE_CURRENT_SOURCE_DIR}/nonme11_second.C

	G__Analysis)

target_link_libraries(Analysis PUBLIC 
	${ROOT_LIBRARIES} ${ROOT_EXE_LINKER_FLAGS} ROOT::Physics ROOT::Core ROOT::RooFit)

target_include_directories(Analysis PUBLIC 
				"${CMAKE_CURRENT_SOURCE_DIR}/" 
				)

			#add_executable(Analysis_exe ${CMAKE_CURRENT_SOURCE_DIR}/main.C)
add_executable(Analysis_exe ${CMAKE_CURRENT_SOURCE_DIR}/main.C)
target_link_libraries(Analysis_exe  Analysis)
