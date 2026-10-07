cmake_minimum_required(VERSION 3.20)
project(RUNTIME VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ─── Dependencies via FetchContent ───────────────────────────────────────────
include(FetchContent)

# Raylib
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# nlohmann/json
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

# ─── Source files ────────────────────────────────────────────────────────────
set(RUNTIME_SOURCES
    # Main
    src/main.cpp

    # Core
    src/core/Game.cpp
    src/core/GBAEngine.cpp
    src/core/SceneManager.cpp
    src/core/EventBus.cpp
    src/core/TimeManager.cpp
    src/core/Config.cpp

    # Entities
    src/entities/Entity.cpp
    src/entities/Character.cpp
    src/entities/Player.cpp
    src/entities/NPC.cpp
    src/entities/Enemy.cpp

    # World
    src/world/World.cpp
    src/world/Room.cpp
    src/world/Interactable.cpp
    src/world/Tilemap.cpp

    # DSA
    src/dsa/ArrayDS.cpp
    src/dsa/LinkedList.cpp
    src/dsa/Stack.cpp
    src/dsa/Queue.cpp
    src/dsa/SearchAlgorithms.cpp
    src/dsa/SortAlgorithms.cpp
    src/dsa/ComplexityAnalyzer.cpp

    # Gameplay
    src/gameplay/CombatSystem.cpp
    src/gameplay/PuzzleSystem.cpp
    src/gameplay/QuestSystem.cpp
    src/gameplay/ProgressionSystem.cpp
    src/gameplay/InventorySystem.cpp
    src/gameplay/MemoryDistrictScene.cpp
    src/gameplay/GBABattleScene.cpp
    src/gameplay/GBAOverworldScene.cpp

    # Dialogue
    src/dialogue/DialogueSystem.cpp
    src/dialogue/DialogueNode.cpp

    # UI
    src/ui/UISystem.cpp
    src/ui/MainMenu.cpp
    src/ui/DemoScene.cpp
    src/ui/HUD.cpp
    src/ui/DialogueUI.cpp
    src/ui/DSAVisualizer.cpp
    src/ui/MasteryScreen.cpp
    src/ui/PauseMenu.cpp
    src/ui/SettingsMenu.cpp
    src/ui/GBADialogueUI.cpp
    src/ui/GBAMainMenu.cpp
    src/ui/CodedexChallengeUI.cpp
    src/ui/CodedexCodeEditorUI.cpp

    # Audio
    src/audio/AudioManager.cpp
    src/audio/MusicManager.cpp
    src/audio/SFXManager.cpp
    src/audio/VoiceManager.cpp

    # Save
    src/save/SaveManager.cpp
    src/save/SaveData.cpp

    # Utils
    src/utils/Logger.cpp
    src/utils/Random.cpp
    src/utils/MathUtils.cpp
)

# ─── Main executable ────────────────────────────────────────────────────────
add_executable(runtime ${RUNTIME_SOURCES})

target_include_directories(runtime PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(runtime PRIVATE
    raylib
    nlohmann_json::nlohmann_json
)

# Platform-specific linking
if(UNIX AND NOT APPLE)
    target_link_libraries(runtime PRIVATE m pthread dl)
endif()

# ─── Copy assets and data to build directory ─────────────────────────────────
add_custom_command(TARGET runtime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:runtime>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:runtime>/data
    COMMENT "Copying assets and data to build directory"
)

# ─── Tests ───────────────────────────────────────────────────────────────────
option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    enable_testing()

    # DSA tests
    set(DSA_TEST_SOURCES
        src/dsa/ArrayDS.cpp
        src/dsa/LinkedList.cpp
        src/dsa/Stack.cpp
        src/dsa/Queue.cpp
        src/dsa/SearchAlgorithms.cpp
        src/dsa/SortAlgorithms.cpp
        src/dsa/ComplexityAnalyzer.cpp
        src/ui/UISystem.cpp
        src/audio/SFXManager.cpp
        src/audio/AudioManager.cpp
        src/utils/Logger.cpp
        src/utils/Random.cpp
        src/utils/MathUtils.cpp
    )

    add_executable(test_dsa
        tests/dsa/test_array.cpp
        tests/dsa/test_stack.cpp
        tests/dsa/test_queue.cpp
        tests/dsa/test_linked_list.cpp
        tests/dsa/test_algorithms.cpp
        tests/dsa/test_main.cpp
        ${DSA_TEST_SOURCES}
    )
    target_include_directories(test_dsa PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(test_dsa PRIVATE raylib nlohmann_json::nlohmann_json)
    if(UNIX AND NOT APPLE)
        target_link_libraries(test_dsa PRIVATE m pthread dl)
    endif()
    add_test(NAME DSATests COMMAND test_dsa)

    # Save system tests
    add_executable(test_save
        tests/save/test_save.cpp
        src/save/SaveManager.cpp
        src/save/SaveData.cpp
        src/utils/Logger.cpp
        ${CMAKE_SOURCE_DIR}/src/utils/MathUtils.cpp
    )
    target_include_directories(test_save PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(test_save PRIVATE raylib nlohmann_json::nlohmann_json)
    if(UNIX AND NOT APPLE)
        target_link_libraries(test_save PRIVATE m pthread dl)
    endif()
    add_test(NAME SaveTests COMMAND test_save)
endif()

# ─── Compiler warnings ──────────────────────────────────────────────────────
target_compile_options(runtime PRIVATE
    -Wall -Wextra -Wpedantic
)

# ─── Debug/Release configurations ───────────────────────────────────────────
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(runtime PRIVATE RUNTIME_DEBUG=1)
endif()
