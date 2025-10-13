cmake_minimum_required(VERSION 3.30)
project(SFML-Base)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML COMPONENTS graphics window system audio REQUIRED)

add_executable(SFML-Base
        src/main.cpp
)

target_link_libraries(SFML-Base sfml-graphics sfml-window sfml-system sfml-audio)
target_include_directories(SFML-Base PRIVATE ${SFML_INCLUDE_DIR})
target_compile_definitions(SFML-Base PRIVATE SFML_STATIC)