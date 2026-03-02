cmake_minimum_required(VERSION 3.16)
project(mato LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(snake
    src/main.cpp
    src/render/RenderAssetsFactory.cpp
    src/input/KeyboardControl.cpp
    src/game/GameState.cpp
    src/game/FoodSpawner.cpp
    src/game/LevelData.cpp
)

target_include_directories(snake PRIVATE src)
target_link_libraries(snake PRIVATE sfml-graphics sfml-window sfml-system)

add_custom_command(TARGET snake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:snake>/assets
)
