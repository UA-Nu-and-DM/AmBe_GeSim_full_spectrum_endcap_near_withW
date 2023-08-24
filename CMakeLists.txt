#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(GeSim)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
find_package(Geant4 REQUIRED ui_all vis_all)
else()
 
find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find AIDA (optional package)
#
find_package(AIDA)
if(AIDA_FOUND)
  add_definitions(-DG4ANALYSIS_USE)   
endif()

#----------------------------------------------------------------------------
# Find ROOT (optional package)
#
find_package(ROOT)
if(ROOT_FOUND)
  add_definitions(-DG4ANALYSIS_USE_ROOT)
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${AIDA_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(GeSim GeSim.cc ${sources} ${headers})
target_link_libraries(GeSim ${Geant4_LIBRARIES} ${AIDA_LIBRARIES} ${ROOT_LIBRARIES} )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build GeSim. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(GeSim_SCRIPTS
    GeSim.in GeSim.out
  )

foreach(_script ${GeSim_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS GeSim DESTINATION bin)

