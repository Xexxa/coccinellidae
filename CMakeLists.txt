cmake_minimum_required(VERSION 3.16...3.22)

project(coccinellidae
    VERSION 0.0.1
    LANGUAGES CXX
    DESCRIPTION "Coccinellidae Web Browser"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# See slide 100 of the following ppt :^)
# https://crascit.com/wp-content/uploads/2019/09/Deep-CMake-For-Library-Authors-Craig-Scott-CppCon-2019.pdf
if (NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include(cmake/EnableLLD.cmake)

# Lagom
include(FetchContent)
include(cmake/FetchLagom.cmake)

# Lagom warnings
include(${Lagom_SOURCE_DIR}/../CMake/lagom_compile_options.cmake)
add_compile_options(-Wno-expansion-to-defined)
add_compile_options(-Wno-user-defined-literals)

set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Network Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Network Widgets )
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Network Widgets )

set(SOURCES
    BrowserWindow.cpp
    ConsoleClient.cpp
    ConsoleGlobalObject.cpp
    CookieJar.cpp
    RequestManagerQt.cpp
    main.cpp
    WebView.cpp
    History.cpp
    Settings.cpp
    SettingsDialog.cpp
    Tab.cpp
)

add_executable(coccinellidae ${SOURCES})
target_link_libraries(coccinellidae PRIVATE Qt5::Widgets Qt5::Network Lagom::Web Lagom::WebSocket Lagom::Main)
#target_link_libraries(coccinellidae PRIVATE Qt::Widgets Qt::Network Lagom::Web Lagom::WebSocket Lagom::Main)

get_filename_component(
    SERENITY_SOURCE_DIR "${Lagom_SOURCE_DIR}/../.."
    ABSOLUTE
)

add_custom_target(run
    COMMAND "${CMAKE_COMMAND}" -E env "SERENITY_SOURCE_DIR=${SERENITY_SOURCE_DIR}" "$<TARGET_FILE:coccinellidae>"
    USES_TERMINAL
)

add_custom_target(debug
    COMMAND "${CMAKE_COMMAND}" -E env "SERENITY_SOURCE_DIR=${SERENITY_SOURCE_DIR}" gdb "$<TARGET_FILE:coccinellidae>"
    USES_TERMINAL
)

