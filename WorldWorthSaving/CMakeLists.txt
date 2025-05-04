cmake_minimum_required(VERSION 3.16)
project(TheFinalDrops)

set(CMAKE_CXX_STANDARD 17)

# === Source files ===
file(GLOB_RECURSE SOURCES "src/*.cpp")

# === Create executable ===
add_executable(TheFinalDrops ${SOURCES})

# === macOS SFML Framework setup ===
# === macOS SFML Framework setup ===
if(APPLE)
    message(STATUS "Configuring for macOS with SFML Frameworks")

    set(SFML_FRAMEWORKS_DIR "/Users/inka/Downloads/SFML-2.5.1-macOS-clang/Frameworks")

    # Make includes like <SFML/Graphics.hpp> work
    include_directories("${CMAKE_SOURCE_DIR}/sfml-shim")

    # Tell the linker where to find SFML frameworks
    target_link_options(TheFinalDrops PRIVATE -F "${SFML_FRAMEWORKS_DIR}")

    # Link to the SFML frameworks
    target_link_libraries(TheFinalDrops
        "-framework SFML"
        "-framework SFMLGraphics"
        "-framework SFMLWindow"
        "-framework SFMLSystem"
        "-framework SFMLAudio"
        "-framework SFMLNetwork"
    )
endif()

# === Copy assets after build (for local dev runs) ===
add_custom_command(TARGET TheFinalDrops POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:TheFinalDrops>/assets
)

