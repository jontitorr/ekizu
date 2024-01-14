cmake_minimum_required(VERSION 3.15)
project(${CMAKE_PROJECT_NAME}_examples LANGUAGES C CXX)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/_deps/dotenv-src/dotenv/dotenv.h")
    file(
        DOWNLOAD
        "https://raw.githubusercontent.com/laserpants/dotenv-cpp/master/include/laserpants/dotenv/dotenv.h"
        "${CMAKE_BINARY_DIR}/_deps/dotenv-src/dotenv/dotenv.h"
        TLS_VERIFY ON
    )
endif()

add_library(dotenv INTERFACE)
target_sources(
    dotenv
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/_deps/dotenv-src/dotenv/dotenv.h>
)
target_include_directories(
    dotenv INTERFACE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/_deps/dotenv-src>
                     $<INSTALL_INTERFACE:include>
)

if(WIN32)
    # Disable getenv() warning
    target_compile_definitions(dotenv INTERFACE _CRT_SECURE_NO_WARNINGS)
endif()

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
        ${example_name} PRIVATE ${CMAKE_PROJECT_NAME} dotenv portaudio_static spdlog::spdlog sodium
    )
endforeach()

file(COPY airhorn.dca DESTINATION ${CMAKE_CURRENT_BINARY_DIR})