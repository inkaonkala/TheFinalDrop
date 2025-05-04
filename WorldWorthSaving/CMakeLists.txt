cmake_minimum_required(VERSION 3.16)
project(JamsEscape)

set(CMAKE_CXX_STANDARD 17)

# === Add SFML as a subdirectory ===
# Disable macOS frameworks inside SFML
set(SFML_USE_SYSTEM_DEPS ON)
set(SFML_USE_SYSTEM_FREETYPE ON)
set(SFML_USE_SYSTEM_OPENAL ON)

add_subdirectory(SFML)


# === Include SFML headers ===
include_directories(SFML/include)

# === Your source files ===
file(GLOB_RECURSE SOURCES "src/*.cpp")

# === Create the game executable ===
add_executable(JamsEscape ${SOURCES})


# === Link to SFML statically ===
target_link_libraries(JamsEscape
    sfml-graphics
    sfml-window
    sfml-system
	sfml-audio
)

set_target_properties(JamsEscape PROPERTIES
    BUILD_RPATH "$ORIGIN"
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
)


# === Copy SFML .so libraries and assets after build ===
add_custom_command(TARGET JamsEscape POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:JamsEscape>/assets
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/SFML/lib/libsfml-graphics.so.2.6 $<TARGET_FILE_DIR:JamsEscape>
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/SFML/lib/libsfml-window.so.2.6 $<TARGET_FILE_DIR:JamsEscape>
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/SFML/lib/libsfml-system.so.2.6 $<TARGET_FILE_DIR:JamsEscape>
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/SFML/lib/libsfml-audio.so.2.6 $<TARGET_FILE_DIR:JamsEscape>
)
