cmake_minimum_required(VERSION 3.29)
project(Synth_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp
        input_base.h
        MyStream.cpp
        MyStream.h
        note_data.h
        sound_modifier_data.h
        MyStream.cpp
        main.cpp)

include(FetchContent)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL SYSTEM
)

FetchContent_MakeAvailable(SFML)

target_link_libraries(main SFML::System SFML::Window SFML::Graphics SFML::Network SFML::Audio)


