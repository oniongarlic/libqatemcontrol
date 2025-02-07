cmake_minimum_required(VERSION 3.16)

project(qatemcontrol VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_EXAMPLE "Build example" OFF)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOVERSION 1)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Network)

set(qatemcontrol_headers
    libqatemcontrol_global.h
    qatemcameracontrol.h
    qatemconnection.h
    qatemdownstreamkey.h
    qatemfairlight.h
    qatemmixeffect.h
    qatemrecording.h
    qatemstreaming.h
    qatemsubsystembase.h
    qatemsupersourcebox.h
    qatemsupersource.h
    qatemtypes.h
    qupstreamkeysettings.h
)

set(qatemcontrol_sources
    qatemcameracontrol.cpp
    qatemconnection.cpp
    qatemdownstreamkey.cpp
    qatemfairlight.cpp
    qatemmixeffect.cpp
    qatemrecording.cpp
    qatemstreaming.cpp
    qatemsubsystembase.cpp
    qatemsupersourcebox.cpp
    qatemsupersource.cpp
)
add_library(qatemcontrol SHARED ${qatemcontrol_headers} ${qatemcontrol_sources})

target_include_directories(qatemcontrol PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)

target_link_libraries(qatemcontrol PRIVATE Qt6::Core Qt6::Gui Qt6::Network)

target_compile_definitions(qatemcontrol PRIVATE LIBQATEMCONTROL_LIBRARY)

set_target_properties(qatemcontrol PROPERTIES
                              VERSION ${PROJECT_VERSION}
                            SOVERSION ${PROJECT_VERSION_MAJOR})

if (${BUILD_EXAMPLE})
    add_subdirectory(example)
endif()

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(config_export_name "${PROJECT_NAME}Config")
set(targets_export_name "${PROJECT_NAME}Targets")
set(project_config "${CMAKE_CURRENT_BINARY_DIR}/${config_export_name}.cmake")
set(version_config "${CMAKE_CURRENT_BINARY_DIR}/${config_export_name}Version.cmake")
set(namespace "${PROJECT_NAME}::")

include(GNUInstallDirs)
include (CMakePackageConfigHelpers)

configure_package_config_file(
    "config.cmake.in"
    ${project_config}
    INSTALL_DESTINATION ${config_install_dir}
)
write_basic_package_version_file(
    ${version_config}
    VERSION ${VERSION}
    COMPATIBILITY SameMajorVersion
)

install(TARGETS qatemcontrol
    EXPORT ${targets_export_name}
    BUNDLE DESTINATION .
    INCLUDES DESTINATION "include"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${include_install_dir}
)

install(EXPORT ${targets_export_name}
    NAMESPACE ${namespace}
    DESTINATION ${config_install_dir})

install(FILES ${project_config} ${version_config}
    DESTINATION ${config_install_dir})

install(FILES ${qatemcontrol_headers} DESTINATION include/QAtemControl)
