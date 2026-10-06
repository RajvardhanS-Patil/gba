#include "save/SaveManager.h"
#include "save/SaveData.h"
#include <iostream>
#include <cassert>
#include <filesystem>

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << " RUNTIME — SAVE SYSTEM UNIT TESTS" << std::endl;
    std::cout << "========================================" << std::endl;

    // Ensure save dir exists
    std::filesystem::create_directories("data/saves");

    SaveManager sm;

    // Setup dummy save data
    SaveData original;
    original.chapter = 1;
    original.currentRoomId = "memory_district_01";
    original.playerHealth = 85;
    original.playerEnergy = 90;
    original.playerXP = 150;
    original.playerLevel = 2;
    original.playerPosition = {320.0f, 240.0f};
    original.dsaMastery["Array"] = 75.0f;
    original.unlockedAbilities.push_back("ARRAY_CORE");
    original.completedQuests.push_back("quest_find_memory");
    original.masterVolume = 0.8f;
    original.musicVolume = 0.7f;
    original.timestamp = "2026-10-07 01:30:00";

    const int testSlot = 99; // Use slot 99 for test isolation

    // 1. Test save
    std::cout << "[TEST] Saving data to slot " << testSlot << "..." << std::endl;
    sm.save(testSlot, original);
    assert(sm.hasSave(testSlot) == true);

    // 2. Test load
    std::cout << "[TEST] Loading data from slot " << testSlot << "..." << std::endl;
    auto loadedOpt = sm.load(testSlot);
    assert(loadedOpt.has_value());
    const SaveData& loaded = loadedOpt.value();

    assert(loaded.chapter == 1);
    assert(loaded.currentRoomId == "memory_district_01");
    assert(loaded.playerHealth == 85);
    assert(loaded.playerEnergy == 90);
    assert(loaded.playerXP == 150);
    assert(loaded.playerLevel == 2);
    assert(loaded.playerPosition.x == 320.0f);
    assert(loaded.playerPosition.y == 240.0f);
    assert(loaded.dsaMastery.at("Array") == 75.0f);
    assert(!loaded.unlockedAbilities.empty() && loaded.unlockedAbilities[0] == "ARRAY_CORE");
    assert(!loaded.completedQuests.empty() && loaded.completedQuests[0] == "quest_find_memory");

    // 3. Test non-existent slot
    std::cout << "[TEST] Testing non-existent slot handling..." << std::endl;
    auto missing = sm.load(9999);
    assert(!missing.has_value());

    // 4. Test delete
    std::cout << "[TEST] Deleting save in slot " << testSlot << "..." << std::endl;
    sm.deleteSave(testSlot);
    assert(sm.hasSave(testSlot) == false);
    auto afterDelete = sm.load(testSlot);
    assert(!afterDelete.has_value());

    std::cout << "\n>>> ALL SAVE TESTS PASSED SUCCESSFULLY! <<<\n" << std::endl;
    return 0;
}
