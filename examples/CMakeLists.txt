cmake_minimum_required(VERSION 3.15)
project(${CMAKE_PROJECT_NAME}_examples LANGUAGES C CXX)

cpmfindpackage(
    NAME portaudio GITHUB_REPOSITORY "PortAudio/portaudio" GIT_TAG "v19.7.0"
)

cpmfindpackage(
    NAME
    "spdlog"
    GITHUB_REPOSITORY
    "gabime/spdlog"
    GIT_TAG
    "v1.12.0"
    OPTIONS
    "SPDLOG_FMT_EXTERNAL ON"
)

file(GLOB_RECURSE examples "*.cpp")

foreach(example ${examples})
    get_filename_component(example_name ${example} NAME_WE)
    add_executable(${example_name} ${example})
    target_link_libraries(
        ${example_name} PRIVATE ${CMAKE_PROJECT_NAME} portaudio_static
                                spdlog::spdlog sodium
    )
endforeach()

file(COPY airhorn.dca DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
