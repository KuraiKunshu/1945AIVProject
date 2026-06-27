cmake_minimum_required(VERSION 3.20)
project(Exam1945 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

add_library(aiv-collections STATIC
    external/aiv-collections/src/aiv_dict.c
    external/aiv-collections/src/aiv_list.c
    external/aiv-collections/src/aiv_vector.c
    external/aiv-collections/include/aiv_dict.h
    external/aiv-collections/include/aiv_list.h
    external/aiv-collections/include/aiv_vector.h
)

target_include_directories(aiv-collections PUBLIC
    external/aiv-collections/include
)

set(ENGINE_SOURCES
    src/Engine/Animation/animation.c
    src/Engine/Audio/audio.c
    src/Engine/Collision/collision.c
    src/Engine/Input/input.c
    src/Engine/Resources/assets.c
    src/Engine/Utils/utils.c
)

set(ENGINE_HEADERS
    src/Engine/Animation/animation.h
    src/Engine/Audio/audio.h
    src/Engine/Collision/collision.h
    src/Engine/Input/input.h
    src/Engine/Resources/assets.h
    src/Engine/Utils/utils.h
)

set(GAME_OBJECT_SOURCES
    src/GameObjects/Actors/actor.c
    src/GameObjects/Actors/player.c
    src/GameObjects/Actors/enemy.c
    src/GameObjects/Projectiles/bullet.c
    src/GameObjects/Effects/explosion.c
)

set(GAME_OBJECT_HEADERS
    src/GameObjects/Actors/actor.h
    src/GameObjects/Actors/player.h
    src/GameObjects/Actors/enemy.h
    src/GameObjects/Projectiles/bullet.h
    src/GameObjects/Effects/explosion.h
)

set(SCENE_SOURCES
    src/Scene/Game/game.c
    src/Scene/Level/level.c
    src/Scene/UI/ui.c
)

set(SCENE_HEADERS
    src/Scene/Game/game.h
    src/Scene/Level/level.h
    src/Scene/UI/ui.h
)

add_executable(1945
    src/Program/main.c
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
    ${GAME_OBJECT_SOURCES}
    ${GAME_OBJECT_HEADERS}
    ${SCENE_SOURCES}
    ${SCENE_HEADERS}
)

target_include_directories(1945 PRIVATE
    src/Program
    src/Engine/Animation
    src/Engine/Audio
    src/Engine/Collision
    src/Engine/Input
    src/Engine/Resources
    src/Engine/Utils
    src/GameObjects/Actors
    src/GameObjects/Projectiles
    src/GameObjects/Effects
    src/Scene/Game
    src/Scene/Level
    src/Scene/UI
)

target_link_libraries(1945 PRIVATE aiv-collections)

if (EXISTS "${CMAKE_SOURCE_DIR}/external/raylib/CMakeLists.txt")
    message(STATUS "Using local raylib at external/raylib")
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
    add_subdirectory(external/raylib)
    target_link_libraries(1945 PRIVATE raylib)
else()
    message(STATUS "raylib not found locally, downloading with FetchContent")
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
    )
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(raylib)
    target_link_libraries(1945 PRIVATE raylib)
endif()

add_custom_command(
    TARGET 1945
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Assets
            $<TARGET_FILE_DIR:1945>/assets
)

include(CTest)

if(BUILD_TESTING)
    set(CLOVE_UNIT_LOCAL_DIR "${CMAKE_SOURCE_DIR}/external/_deps/clove_unit-src")

    if(EXISTS "${CLOVE_UNIT_LOCAL_DIR}/CMakeLists.txt")
        message(STATUS "Using local CLove-Unit at external/_deps/clove_unit-src")
        add_subdirectory(
            "${CLOVE_UNIT_LOCAL_DIR}"
            "${CMAKE_BINARY_DIR}/_deps/clove_unit-build"
        )
    else()
        message(STATUS "CLove-Unit not found locally, downloading with FetchContent")
        FetchContent_Declare(
            clove_unit
            GIT_REPOSITORY https://github.com/fdefelici/clove-unit.git
            GIT_TAG ef6676f9f4ee870970b0626ae8eae4831dd7ada0
        )
        FetchContent_MakeAvailable(clove_unit)
    endif()

    add_executable(unit_tests
        tests/test_main.c
        tests/test_collision.c
        tests/test_collections.c
        tests/test_animation.c
        src/Engine/Collision/collision.c
        src/Engine/Animation/animation.c
        src/GameObjects/Effects/explosion.c
    )

    target_include_directories(unit_tests PRIVATE
        src/Engine/Animation
        src/Engine/Collision
        src/Engine/Utils
        src/GameObjects/Effects
        external/aiv-collections/include
    )

    target_link_libraries(unit_tests PRIVATE
        aiv-collections
        clove-unit
        raylib
    )

    add_test(
        NAME unit_tests
        COMMAND unit_tests --error-on-test-fail
    )
endif()
