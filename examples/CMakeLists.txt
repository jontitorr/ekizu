cmake_minimum_required(VERSION 3.15)
project(${CMAKE_PROJECT_NAME}_examples LANGUAGES CXX)

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

cpmfindpackage(
    NAME "spdlog" GITHUB_REPOSITORY "gabime/spdlog" GIT_TAG "v1.12.0" OPTIONS "SPDLOG_FMT_EXTERNAL ON"
)

file(GLOB_RECURSE examples "*.cpp")

foreach(example ${examples})
    get_filename_component(example_name ${example} NAME_WE)
    add_executable(${example_name} ${example})
    target_link_libraries(
        ${example_name} PRIVATE ${CMAKE_PROJECT_NAME} dotenv spdlog::spdlog
    )
endforeach()

file(COPY .env DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
