cmake_minimum_required(VERSION 3.29)
project(Synth_project)

set(CMAKE_CXX_STANDARD  20)

add_executable(main main.cpp
        input_base.h
        MyStream.cpp
        MyStream.h
        note_data.h
        sound_modifier_data.h
        MyStream.cpp
        main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML/SFML-3.0.0/lib/cmake/SFML")
set(Ogg_DIR "${CMAKE_SOURCE_DIR}/SFML/SFML-3.0.0/lib/cmake/Ogg")

find_package(SFML 3 COMPONENTS System Window Graphics Audio REQUIRED)

target_link_libraries(main SFML::System SFML::Window SFML::Graphics SFML::Audio)

