cmake_minimum_required(VERSION 3.16)

project(OperatorEditor LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>/)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Sql)

include(cmake/find_assets.cmake)
GENERATE_ICONS_QRC(${CMAKE_CURRENT_SOURCE_DIR}/icons icons/icons.qrc)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)
file(GLOB_RECURSE forms *.ui)
file(GLOB_RECURSE resources *.qrc)

set(PROJECT_SOURCES
        ${sources}
        ${headers}
        ${resources}
        ${forms}
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Gui Qt5::Widgets Qt5::Sql)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}" REQUIRED)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${WINDEPLOYQT_EXECUTABLE} --no-translations --no-compiler-runtime "$<TARGET_FILE:${PROJECT_NAME}>"
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/db/system.db"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying system.db"
)
