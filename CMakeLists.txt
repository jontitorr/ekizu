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

include(cmake/CPM.cmake)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "include/*.hpp" "src/*.cpp")

add_library(${PROJECT_NAME} ${sources})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
        $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${PROJECT_NAME_UPPERCASE}_STATIC_DEFINE>
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(
    ${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->"
                           "$<$<PLATFORM_ID:Windows>:/bigobj>"
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/export>
           $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_definitions(
        ${PROJECT_NAME}
        PUBLIC -D_WIN32_WINNT=0x0601
               -D_SCL_SECURE_NO_WARNINGS=1
               -D_CRT_SECURE_NO_WARNINGS=1
               -D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
               -D_SILENCE_CXX17_ADAPTOR_TYPEDEFS_DEPRECATION_WARNING
    )
endif()

set(${PROJECT_NAME}_WARNINGS_AS_ERRORS ON)
include(cmake/CompilerWarnings.cmake)

set_project_warnings(${PROJECT_NAME})

set(TRY_BOOST_VERSION "1.81.0")
set(BOOST_NOT_HEADER_ONLY_COMPONENTS_THAT_YOU_NEED
    "coroutine;filesystem;thread;url"
)
set(BOOST_HEADER_ONLY_COMPONENTS_THAT_YOU_NEED "asio;beast;outcome")

set(IS_BOOST_LOCAL OFF)
if(${CPM_LOCAL_PACKAGES_ONLY})
    message(STATUS "Trying to find Boost...")
    find_package(
        Boost ${TRY_BOOST_VERSION} REQUIRED
        COMPONENTS ${BOOST_NOT_HEADER_ONLY_COMPONENTS_THAT_YOU_NEED}
    )
    set(IS_BOOST_LOCAL ON)
elseif(${CPM_USE_LOCAL_PACKAGES} OR NOT ${CPM_DOWNLOAD_ALL})
    message(STATUS "Trying to use local Boost...")
    find_package(
        Boost ${TRY_BOOST_VERSION}
        COMPONENTS ${BOOST_NOT_HEADER_ONLY_COMPONENTS_THAT_YOU_NEED}
    )
    if(${BOOST_FOUND})
        set(IS_BOOST_LOCAL ON)
        message(DEBUG "boost include dir: ${Boost_INCLUDE_DIRS}")
    endif()
endif()

if(NOT (${BOOST_FOUND}) OR (NOT DEFINED BOOST_FOUND))
    message(STATUS "Trying to download Boost...")
    set(BOOST_INCLUDE_LIBRARIES
        "${BOOST_NOT_HEADER_ONLY_COMPONENTS_THAT_YOU_NEED};${BOOST_HEADER_ONLY_COMPONENTS_THAT_YOU_NEED}"
    )
    set(BOOST_SKIP_INSTALL_RULES OFF)
    cpmaddpackage(
        NAME
        Boost
        URL
        "https://github.com/Xminent/boost-cmake/releases/download/1.84.0-release/boost-1.84.0.tar.xz"
    )
    set(IS_BOOST_LOCAL OFF)
endif()

cpmfindpackage(
    NAME
    "OpenSSL"
    GITHUB_REPOSITORY
    "Xminent/openssl-cmake"
    GIT_TAG
    "master"
    OPTIONS
    "WITH_APPS OFF"
)

if(OpenSSL_ADDED)
    add_library(OpenSSL::Crypto ALIAS crypto)
    add_library(OpenSSL::SSL ALIAS ssl)
    install(TARGETS crypto ssl EXPORT ${PROJECT_NAME}Targets)
else()
    set(SEPARATE_DEPENDENCIES "OpenSSL;SSL;")
endif()

cpmfindpackage(NAME "fmt" GITHUB_REPOSITORY "fmtlib/fmt" GIT_TAG "13156e5")

if(fmt_ADDED)
    install(TARGETS fmt EXPORT ${PROJECT_NAME}Targets)

    if(WIN32)
        target_compile_definitions(
            fmt PUBLIC _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
        )
    endif()
else()
    set(SEPARATE_DEPENDENCIES "${SEPARATE_DEPENDENCIES}fmt;;")
endif()

cpmaddpackage(NAME certify GITHUB_REPOSITORY "Xminent/certify" GIT_TAG "master")

set(SEPARATE_DEPENDENCIES
    "${SEPARATE_DEPENDENCIES}Threads;;Boost;coroutine,date_time,filesystem,system,thread,url"
)

if(${IS_BOOST_LOCAL})
    target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)
else()
    target_link_libraries(
        ${PROJECT_NAME} PUBLIC Boost::asio Boost::beast Boost::outcome
    )
endif()

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

install(TARGETS core nlohmann_json EXPORT ${PROJECT_NAME}Targets)

find_package(ZLIB)

if(ZLIB_FOUND)
    set(SEPARATE_DEPENDENCIES "${SEPARATE_DEPENDENCIES}ZLIB;;")
else()
    cpmaddpackage(
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
    install(TARGETS zlib EXPORT ${PROJECT_NAME}Targets)
endif()

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC certify::core Boost::coroutine Boost::thread fmt::fmt nlohmann_json
    PRIVATE Boost::url ZLIB::ZLIB
)

include(GenerateExportHeader)

generate_export_header(
    ${PROJECT_NAME} EXPORT_FILE_NAME export/${PROJECT_NAME_LOWERCASE}/export.h
)

if(${PROJECT_NAME}_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(${PROJECT_NAME}_INSTALL)
    include(GNUInstallDirs)

    install(
        TARGETS ${PROJECT_NAME}
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
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Discord library for C++")
    set(CPACK_PACKAGE_DESCRIPTION "A Discord library for C++")
    set(CPACK_PACKAGE_CONTACT "mail@xminent.com")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A Discord library for C++")

    if(WIN32)
        set(CPACK_GENERATOR ZIP)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CPACK_GENERATOR "DEB;RPM")
    else()
        set(CPACK_GENERATOR "TBZ2")
    endif()

    include(CPack)
endif()

# target_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=thread)
# target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=thread)
