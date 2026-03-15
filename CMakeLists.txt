cmake_minimum_required(VERSION 3.16)
project(mato LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Re-configure when levels/ directory changes (files added/removed)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/levels")
file(GLOB LEVEL_FILES "${CMAKE_SOURCE_DIR}/levels/*.level")

set(GENERATED_LEVEL_CPP "${CMAKE_BINARY_DIR}/generated/LevelData_generated.cpp")

add_custom_command(
    OUTPUT "${GENERATED_LEVEL_CPP}"
    COMMAND Python3::Interpreter
            "${CMAKE_SOURCE_DIR}/scripts/generate_levels.py"
            --input-dir "${CMAKE_SOURCE_DIR}/levels"
            --output "${GENERATED_LEVEL_CPP}"
    DEPENDS ${LEVEL_FILES} "${CMAKE_SOURCE_DIR}/scripts/generate_levels.py"
    COMMENT "Generating LevelData from level files"
    VERBATIM
)

add_executable(mato
    src/main.cpp
    src/render/RenderAssetsFactory.cpp
    src/input/KeyboardControl.cpp
    src/game/GameState.cpp
    src/game/FoodSpawner.cpp
    ${GENERATED_LEVEL_CPP}
    src/render/GameText.cpp
    src/render/WindowTitle.cpp
    src/render/CellRenderer.cpp
    src/render/StatusRenderer.cpp
    src/effects/EffectEngine.cpp
    src/effects/CompositeEffect.cpp
    src/effects/gfx/FoodEatEffect.cpp
    src/effects/gfx/DeathEffect.cpp
    src/effects/gfx/CelebrationEffect.cpp
    src/effects/sfx/SoundEffect.cpp
    src/audio/SoundPlayer.cpp
    src/game/FoodReward.cpp
)

target_include_directories(mato PRIVATE src)
target_link_libraries(mato PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

add_custom_command(TARGET mato POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:mato>/assets
)

add_executable(mato-editor
    src/editor/main.cpp
    src/editor/Editor.cpp
    src/editor/EditorRenderer.cpp
    src/editor/LevelFileIO.cpp
)
target_include_directories(mato-editor PRIVATE src)
target_link_libraries(mato-editor PRIVATE sfml-graphics sfml-window sfml-system)

add_custom_command(TARGET mato-editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:mato-editor>/assets
)
