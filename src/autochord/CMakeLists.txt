cmake_minimum_required(VERSION 3.10)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/arp_modes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chord.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scale.cpp   
    ${CMAKE_CURRENT_SOURCE_DIR}/autochord.cpp
)