cmake_minimum_required(VERSION 3.29)
Include(FetchContent)
set(CMAKE_CXX_STANDARD 20)

project(main)

#FetchContent_Declare(
#        Catch2
#        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#        GIT_TAG        v3.4.0 # or a later release
#)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY
        https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL SYSTEM )

#FetchContent_MakeAvailable(Catch2)

FetchContent_MakeAvailable(SFML)

#add_executable(tests test.cpp input_base.h MyStream.cpp MyStream.h note_data.h sound_modifier_data.h MyStream.cpp)
add_executable(main main.cpp input_base.h MyStream.cpp MyStream.h note_data.h sound_modifier_data.h MyStream.cpp
        Soundwave.cpp
        Soundwave.h
)

target_link_libraries(main SFML::System SFML::Window SFML::Graphics SFML::Network SFML::Audio)
#target_link_libraries(tests PRIVATE Catch2::Catch2WithMain  SFML::System SFML::Window SFML::Graphics SFML::Network SFML::Audio)


#include(CTest)
#include(Catch)
#catch_discover_tests(tests)