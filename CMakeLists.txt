cmake_minimum_required(VERSION 3.16)

project(
    "ekizu"
    VERSION 0.1.0
    LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
        FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()

if(NOT DEFINED ${PROJECT_NAME}_MASTER_PROJECT)
    set(${PROJECT_NAME}_MASTER_PROJECT OFF)

    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(${PROJECT_NAME}_MASTER_PROJECT ON)
        message(STATUS "CMake version: ${CMAKE_VERSION}")
    endif()
endif()

if(UNIX)
    add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG>")
endif()

string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(${PROJECT_NAME}_INSTALL "Generate the install target" ON)

include(CMakeDependentOption)

cmake_dependent_option(
    ${PROJECT_NAME}_BUILD_EXAMPLES "Build the examples" ON
    "${PROJECT_NAME}_MASTER_PROJECT" OFF
)

if(BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
endif()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/CPM.cmake")
    file(
        DOWNLOAD
        "https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake"
        "${CMAKE_BINARY_DIR}/CPM.cmake"
        TLS_VERIFY ON
    )
endif()
include(${CMAKE_BINARY_DIR}/CPM.cmake)

cpmfindpackage(NAME "fmt" GITHUB_REPOSITORY "fmtlib/fmt" GIT_TAG "10.1.1")

cpmfindpackage(
    NAME
    "net"
    GITHUB_REPOSITORY
    "Xminent/net"
    GIT_TAG
    "main"
    OPTIONS
    "BUILD_SHARED_LIBS OFF"
)

cpmaddpackage(
    NAME
    "nlohmann_json"
    VERSION
    "3.11.1"
    URL
    "https://github.com/nlohmann/json/releases/download/v3.11.1/include.zip"
    URL_HASH
    "SHA256=9c15ca7806f863872452bfbc85fee6d1c9868117e3ea5e00a204ae961a2e1ae7"
)

add_library(nlohmann_json INTERFACE)
target_include_directories(
    nlohmann_json
    INTERFACE $<BUILD_INTERFACE:${nlohmann_json_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>
)

find_package(ZLIB)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "include/*.hpp" "src/*.cpp")

add_library(${PROJECT_NAME} ${sources})

include(GenerateExportHeader)

generate_export_header(
    ${PROJECT_NAME} EXPORT_FILE_NAME export/${PROJECT_NAME_LOWERCASE}/export.h
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
        $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${PROJECT_NAME_UPPERCASE}_STATIC_DEFINE>
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/export>
           $<INSTALL_INTERFACE:include>
)

set(${PROJECT_NAME}_TARGETS ${PROJECT_NAME} nlohmann_json)
set(SEPARATE_DEPENDENCIES "")

if(fmt_ADDED)
    list(APPEND ${PROJECT_NAME}_TARGETS fmt)
else()
    list(APPEND SEPARATE_DEPENDENCIES "fmt")
endif()

if(net_ADDED)
    list(APPEND ${PROJECT_NAME}_TARGETS net)
else()
    list(APPEND SEPARATE_DEPENDENCIES "net")
endif()

if(ZLIB_FOUND)
    list(APPEND SEPARATE_DEPENDENCIES "ZLIB")
else()
    cpmfindpackage(
        NAME
        "ZLIB"
        GITHUB_REPOSITORY
        "zlib-ng/zlib-ng"
        GIT_TAG
        "2.0.6"
        OPTIONS
        "ZLIB_COMPAT ON"
        "ZLIB_ENABLE_TESTS OFF"
        "SKIP_INSTALL_ALL ON"
    )
    add_library(ZLIB::ZLIB ALIAS zlib)
    list(APPEND ${PROJECT_NAME}_TARGETS zlib)
endif()

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC fmt::fmt net::net nlohmann_json
    PRIVATE ZLIB::ZLIB
)
# target_precompile_headers( ${PROJECT_NAME} PRIVATE
# "${nlohmann_json_SOURCE_DIR}/include/nlohmann/json.hpp" )

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(CLANG_TIDY_EXE)
    # set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}"
    # "-checks=bugprone-*,cert-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,google-build-using-namespace,google-explicit-constructor,google-global-names-in-headers,google-readability-casting,google-runtime-int,google-runtime-operator,hicpp-*,llvm-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-type-vararg,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-hicpp-signed-bitwise,-hicpp-uppercase-literal-suffix,-hicpp-vararg,-llvm-header-guard,-modernize-use-trailing-return-type,-readability-identifier-length,-readability-uppercase-literal-suffix,-cppcoreguidelines-avoid-const-or-ref-data-members,-cppcoreguidelines-avoid-do-while,-cppcoreguidelines-pro-type-reinterpret-cast,-misc-non-private-member-variables-in-classes"
    # ) set_target_properties( ${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY
    # "${CLANG_TIDY_COMMAND}" )

    message(
        STATUS
            "clang-tidy found: ${CLANG_TIDY_EXE} and applied to ${PROJECT_NAME}."
    )
else()
    message(STATUS "clang-tidy not found.")
endif()

if(${PROJECT_NAME}_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(${PROJECT_NAME}_INSTALL)
    include(GNUInstallDirs)

    install(
        TARGETS ${${PROJECT_NAME}_TARGETS}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    install(DIRECTORY ${nlohmann_json_SOURCE_DIR}/include/nlohmann
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
        include/${PROJECT_NAME_LOWERCASE}/version.h @ONLY
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/export/${PROJECT_NAME_LOWERCASE}/export.h
            ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME_LOWERCASE}/version.h
        DESTINATION include/${PROJECT_NAME_LOWERCASE}
    )

    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME_LOWERCASE}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(
        ${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
                  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    set(CPACK_PACKAGE_VENDOR Xminent)
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A networking library for C++")
    set(CPACK_PACKAGE_DESCRIPTION "A networking library for C++")
    set(CPACK_PACKAGE_CONTACT "mail@xminent.com")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A networking library for C++")

    if(WIN32)
        set(CPACK_GENERATOR ZIP)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CPACK_GENERATOR "DEB;RPM")
    else()
        set(CPACK_GENERATOR "TBZ2")
    endif()

    include(CPack)
endif()

target_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=thread)
target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=thread)
