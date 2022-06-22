cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(ddfusion_nbkgd)

find_package(Geant4 REQUIRED ui_all vis_all multithreaded)
find_package(ROOT REQUIRED Core)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/inc)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.hh)

file(GLOB MACROS "*.mac" "NeutronAna.h" "Ana.C" "excitationEnergyScan.sh" "*.csv")

file(COPY ${MACROS} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(ddfusion sim.cc ${sources} ${headers})
target_link_libraries(ddfusion ${Geant4_LIBRARIES})
target_link_libraries(ddfusion ${ROOT_LIBRARIES})

add_custom_target(ddfusion_nbkgd DEPENDS ddfusion)
