cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(ddfusion_nbkgd)

find_package(Geant4 REQUIRED ui_all vis_all multithreaded)
find_package(ROOT)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

add_executable(ddfusion sim.cc ${sources} ${headers})
target_link_libraries(ddfusion ${Geant4_LIBRARIES})
target_link_libraries(ddfusion ${ROOT_LIBRARIES})

add_custom_target(ddfusion_nbkgd DEPENDS ddfusion)
