#include "GBAOverworldScene.h"
#include "GBABattleScene.h"
#include "core/GBAEngine.h"
#include "core/Game.h"
#include "audio/SFXManager.h"
#include "audio/MusicManager.h"
#include "utils/Logger.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

GBAOverworldScene::GBAOverworldScene(const std::string& initialMap, int spawnX, int spawnY)
    : currentMapName_(initialMap) {
    currentMap_ = std::make_unique<Tilemap>();
    currentMap_->load("data/maps/" + initialMap + ".json");
    applyPersistentState();

    player_ = std::make_unique<Player>(spawnX, spawnY);

    camera_.offset = Vector2{static_cast<float>(GBA_SCREEN_WIDTH / 2), static_cast<float>(GBA_SCREEN_HEIGHT / 2)};
    camera_.target = player_->getPosition();
    camera_.rotation = 0.0f;
    camera_.zoom = 1.0f;

    if (FileExists("assets/sprites/nova.png")) {
        novaSprite_ = LoadTexture("assets/sprites/nova.png");
        novaLoaded_ = true;
    }

    if (FileExists("assets/sprites/villager.png")) {
        scholarSprite_ = LoadTexture("assets/sprites/villager.png");
        scholarLoaded_ = true;
    }

    initDemons();
    initWanderingNPCs();
}

GBAOverworldScene::~GBAOverworldScene() {
    if (novaLoaded_ && novaSprite_.id > 0) {
        UnloadTexture(novaSprite_);
        novaLoaded_ = false;
    }
    if (scholarLoaded_ && scholarSprite_.id > 0) {
        UnloadTexture(scholarSprite_);
        scholarLoaded_ = false;
    }
    for (auto& d : demons_) {
        if (d.loaded && d.sprite.id > 0) {
            UnloadTexture(d.sprite);
            d.loaded = false;
        }
    }
    for (auto& npc : wanderingNPCs_) {
        if (npc.loaded && npc.sprite.id > 0) {
            UnloadTexture(npc.sprite);
            npc.loaded = false;
        }
    }
}

void GBAOverworldScene::initDemons() {
    demons_.clear();

    auto addDemon = [this](const std::string& id, const std::string& name, const std::string& title,
                           const std::string& map, int tx, int ty, int ebx, int eby,
                           const std::string& spritePath, bool def) {
        GuardianDemon d;
        d.id = id;
        d.name = name;
        d.title = title;
        d.map = map;
        d.tileX = tx;
        d.tileY = ty;
        d.extraBlockX = ebx;
        d.extraBlockY = eby;
        d.spritePath = spritePath;
        if (FileExists(spritePath.c_str())) {
            d.sprite = LoadTexture(spritePath.c_str());
            d.loaded = true;
        }
        d.defeated = def;
        demons_.push_back(d);
    };

    // 1. Nova Town: Index Gorgon
    addDemon("demon_index", "INDEX GORGON", "Guardian of Array Bounds", "nova_town",
             22, 8, 22, 7, "assets/sprites/demon_index.png", novaGateUnlocked_);

    // 2. Memory Route: Search Wraith
    addDemon("demon_search", "SEARCH WRAITH", "Specter of Time Complexity", "memory_route",
             10, 7, 10, 8, "assets/sprites/demon_search.png", routeBridgeUnlocked_);

    // 3. Linked Cavern: Cycle Hydra
    addDemon("demon_hydra", "CYCLE HYDRA", "The Looping Pointer Beast", "linked_cavern",
             22, 8, 22, 7, "assets/sprites/demon_hydra.png", false);

    // 4. Stack Citadel: Overflow Titan
    addDemon("demon_overflow", "OVERFLOW TITAN", "The LIFO Armor Behemoth", "stack_citadel",
             11, 2, 12, 2, "assets/sprites/demon_overflow.png", false);

    // 5. Tree Grove: Tree Leviathan
    addDemon("demon_tree", "TREE LEVIATHAN", "Arbiter of Balanced Trees", "tree_grove",
             11, 3, 12, 3, "assets/sprites/demon_tree.png", false);

    // 6. Hash Hollows: Hash Wyrm
    addDemon("demon_hash", "HASH WYRM", "The Collision Matrix Beast", "hash_hollows",
             22, 8, 22, 9, "assets/sprites/demon_hash.png", false);

    // 7. Heap River: Heap Behemoth
    addDemon("demon_heap", "HEAP BEHEMOTH", "Titan of Priority Queues", "heap_river",
             11, 2, 12, 2, "assets/sprites/demon_heap.png", false);

    // 8. Graph Catacombs: Cycle Lich
    addDemon("demon_graph", "CYCLE LICH", "Lord of Adjacency Networks", "graph_catacombs",
             22, 8, 22, 9, "assets/sprites/demon_graph.png", false);

    // 9. Memo Sanctum: Subproblem Colossus
    addDemon("demon_memo", "SUBPROBLEM COLOSSUS", "Master of Optimal Substructure", "memo_sanctum",
             11, 2, 12, 2, "assets/sprites/demon_memo.png", false);
}

void GBAOverworldScene::initWanderingNPCs() {
    wanderingNPCs_.clear();
    // 1. Apprentice Dex in Nova Town
    {
        WanderingNPC npc;
        npc.id = "dex";
        npc.name = "APPRENTICE DEX";
        npc.map = "nova_town";
        npc.tileX = 14; npc.tileY = 11;
        npc.text1 = "I am practicing LeetCode array problems!";
        npc.text2 = "Always bounds check k >= 0 && k < n before accessing!";
        if (FileExists("assets/sprites/villager.png")) {
            npc.sprite = LoadTexture("assets/sprites/villager.png");
            npc.loaded = true;
        }
        wanderingNPCs_.push_back(npc);
    }
    // 2. Scout Leo in Memory Route
    {
        WanderingNPC npc;
        npc.id = "leo";
        npc.name = "SCOUT LEO";
        npc.map = "memory_route";
        npc.tileX = 5; npc.tileY = 7;
        npc.text1 = "The Search Wraith will reject any unsorted array!";
        npc.text2 = "Binary search cuts search space in half: O(log N)!";
        if (FileExists("assets/sprites/villager.png")) {
            npc.sprite = LoadTexture("assets/sprites/villager.png");
            npc.loaded = true;
        }
        wanderingNPCs_.push_back(npc);
    }
}

void GBAOverworldScene::updateNPCs(float dt) {
    for (auto& npc : wanderingNPCs_) {
        if (npc.map != currentMapName_) continue;
        npc.idleTimer -= dt;
        if (npc.idleTimer <= 0.0f) {
            npc.idleTimer = 2.0f + static_cast<float>(rand() % 20) / 10.0f;
            int r = rand() % 4;
            int dx = 0, dy = 0;
            if (r == 0) dx = 1;
            else if (r == 1) dx = -1;
            else if (r == 2) dy = 1;
            else dy = -1;

            int nx = npc.tileX + dx;
            int ny = npc.tileY + dy;
            if (currentMap_ && !currentMap_->isSolid(nx, ny) && !isTileBlockedByDemon(nx, ny)) {
                npc.tileX = nx;
                npc.tileY = ny;
            }
        }
    }
}

void GBAOverworldScene::drawNPCs() const {
    for (const auto& npc : wanderingNPCs_) {
        if (npc.map != currentMapName_) continue;
        if (npc.loaded && npc.sprite.id > 0) {
            DrawTextureRec(npc.sprite, Rectangle{0, 0, 16, 20},
                           Vector2{static_cast<float>(npc.tileX * 16), static_cast<float>(npc.tileY * 16 - 4)}, WHITE);
            int dist = std::abs(player_->getTileX() - npc.tileX) + std::abs(player_->getTileY() - npc.tileY);
            if (dist <= 1) {
                DrawText("[Z]", npc.tileX * 16 + 2, npc.tileY * 16 - 8, 7, GBA_GOLD);
            }
        }
    }
}

const GBAOverworldScene::WanderingNPC* GBAOverworldScene::getFacingNPC(int fx, int fy) const {
    for (const auto& npc : wanderingNPCs_) {
        if (npc.map == currentMapName_ && npc.tileX == fx && npc.tileY == fy) {
            return &npc;
        }
    }
    return nullptr;
}

void GBAOverworldScene::updateParticles(float dt) {
    // Spawn ambient particles based on environment
    if (particles_.size() < 24) {
        AmbientParticle p;
        p.pos = Vector2{
            camera_.target.x - 130.0f + static_cast<float>(rand() % 260),
            camera_.target.y - 90.0f + static_cast<float>(rand() % 180)
        };
        p.life = 0.0f;
        p.maxLife = 2.0f + static_cast<float>(rand() % 20) / 10.0f;
        p.size = 1.0f + static_cast<float>(rand() % 2);

        if (currentMapName_ == "stack_citadel" || currentMapName_ == "hash_hollows") {
            // Volcanic fiery ember
            p.vel = Vector2{static_cast<float>((rand() % 20) - 10), -12.0f - static_cast<float>(rand() % 10)};
            p.color = Color{255, static_cast<unsigned char>(100 + rand() % 80), 30, 180};
        } else if (currentMapName_ == "tree_grove" || currentMapName_ == "heap_river") {
            // Emerald forest spores / leaves
            p.vel = Vector2{static_cast<float>((rand() % 24) - 8), -5.0f - static_cast<float>(rand() % 8)};
            p.color = Color{90, static_cast<unsigned char>(200 + rand() % 55), 110, 160};
        } else {
            // Cyan digital cyber-sparks
            p.vel = Vector2{static_cast<float>((rand() % 16) - 8), -8.0f - static_cast<float>(rand() % 8)};
            p.color = Color{60, 220, 255, 170};
        }
        particles_.push_back(p);
    }

    for (auto it = particles_.begin(); it != particles_.end();) {
        it->life += dt;
        it->pos.x += it->vel.x * dt;
        it->pos.y += it->vel.y * dt;
        if (it->life >= it->maxLife) {
            it = particles_.erase(it);
        } else {
            ++it;
        }
    }
}

void GBAOverworldScene::drawParticles() const {
    for (const auto& p : particles_) {
        float alpha = 1.0f - (p.life / p.maxLife);
        Color c = p.color;
        c.a = static_cast<unsigned char>(c.a * alpha);
        DrawCircle(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), p.size, c);
    }
}

bool GBAOverworldScene::isTileBlockedByDemon(int tx, int ty) {
    for (const auto& d : demons_) {
        if (d.map == currentMapName_ && !d.defeated) {
            if ((d.tileX == tx && d.tileY == ty) ||
                (d.extraBlockX == tx && d.extraBlockY == ty)) {
                return true;
            }
        }
    }
    return false;
}

GBAOverworldScene::GuardianDemon* GBAOverworldScene::getFacingDemon(int fx, int fy) {
    for (auto& d : demons_) {
        if (d.map == currentMapName_ && !d.defeated) {
            if ((d.tileX == fx && d.tileY == fy) ||
                (d.extraBlockX == fx && d.extraBlockY == fy)) {
                return &d;
            }
            int dist = std::abs(player_->getTileX() - d.tileX) + std::abs(player_->getTileY() - d.tileY);
            if (dist <= 1 && (fx == d.tileX || fy == d.tileY)) {
                return &d;
            }
        }
    }
    return nullptr;
}

void GBAOverworldScene::applyPersistentState() {
    if (!currentMap_) return;

    if (currentMapName_ == "nova_town") {
        if (novaChestOpened_) currentMap_->setTile(6, 3, 21, 1);
    } else if (currentMapName_ == "memory_route") {
        if (routeBridgeUnlocked_) {
            currentMap_->setTile(10, 7, 22, 0);
            currentMap_->setTile(11, 7, 22, 0);
            currentMap_->setTile(10, 8, 22, 0);
            currentMap_->setTile(11, 8, 22, 0);
        }
        if (routeChestOpened_) currentMap_->setTile(20, 3, 21, 1);
    } else if (currentMapName_ == "linked_cavern") {
        if (cavernBridgeUnlocked_) {
            currentMap_->setTile(10, 8, 22, 0);
            currentMap_->setTile(11, 8, 22, 0);
            currentMap_->setTile(12, 8, 22, 0);
            currentMap_->setTile(13, 8, 22, 0);
        }
        if (cavernQueueDoorUnlocked_) currentMap_->setTile(17, 8, 19, 0);
        if (cavernChestOpened_) currentMap_->setTile(20, 4, 21, 1);
    } else if (currentMapName_ == "stack_citadel") {
        if (citadelChestOpened_) currentMap_->setTile(20, 4, 21, 1);
    } else if (currentMapName_ == "tree_grove") {
        if (groveChestOpened_) currentMap_->setTile(19, 10, 21, 1);
    } else if (currentMapName_ == "hash_hollows") {
        if (hashChestOpened_) currentMap_->setTile(4, 4, 21, 1);
    } else if (currentMapName_ == "heap_river") {
        if (heapChestOpened_) currentMap_->setTile(20, 4, 21, 1);
    } else if (currentMapName_ == "graph_catacombs") {
        if (graphChestOpened_) currentMap_->setTile(4, 4, 21, 1);
    } else if (currentMapName_ == "memo_sanctum") {
        if (memoChestOpened_) currentMap_->setTile(20, 4, 21, 1);
    }
}

void GBAOverworldScene::updateBGM() {
    if (currentMapName_ == "nova_town" || currentMapName_ == "tree_grove" || currentMapName_ == "master_sanctum") {
        MusicManager::instance().play("assets/music/town.wav", true);
    } else {
        MusicManager::instance().play("assets/music/route.wav", true);
    }
}

void GBAOverworldScene::onEnter() {
    LOG_INFO("Entered GBA Overworld: " + currentMap_->getName());
    updateBGM();
    if (storyState_ == 0) {
        dialogueUI_.start("NOVA", "Kael, wake up! The Algorithm Core is under siege!", "Guardian Demons test your code at every gateway!");
        storyState_ = 1;
    }
}

void GBAOverworldScene::onExit() {
    LOG_INFO("Exiting GBA Overworld");
}

void GBAOverworldScene::switchMap(const std::string& mapName, int spawnX, int spawnY) {
    currentMapName_ = mapName;
    currentMap_->load("data/maps/" + mapName + ".json");
    applyPersistentState();
    updateBGM();

    player_->setTilePosition(spawnX, spawnY);
    camera_.target = player_->getPosition();
    SFXManager::instance().play("scan");
}

void GBAOverworldScene::fightDemon(GuardianDemon* demon) {
    if (!demon) return;
    SFXManager::instance().play("scan");

    // 1. Index Gorgon (Array Bounds Access)
    if (demon->id == "demon_index") {
        ProblemSpec spec;
        spec.id = "lc1";
        spec.title = "1. Safe Array Memory Access";
        spec.difficulty = "Easy";
        spec.category = "Arrays & Memory";
        spec.description = "Given a contiguous integer array 'arr' of size 'n' and an index 'k', return the value arr[k] safely. If index k is outside the valid bounds [0, n - 1], return -1 to prevent buffer overflow and memory corruption.";
        spec.examples = {
            "Input: arr = [10, 20, 30], n = 3, k = 1\nOutput: 20\nExplanation: Index 1 is within valid range [0, 2].",
            "Input: arr = [5, 15], n = 2, k = 2\nOutput: -1\nExplanation: Index 2 equals n (out of bounds). Return -1."
        };
        spec.constraints = {
            "1 <= n <= 10^5",
            "-10^4 <= arr[i] <= 10^4",
            "Time Complexity: O(1) contiguous lookup",
            "Space Complexity: O(1) auxiliary memory"
        };
        spec.templateBefore = {"int safeAccess(int arr[], int n, int k) {"};
        spec.defaultEditableCode = "    if (k >= 0 && k < n) return arr[k]; return -1;";
        spec.templateAfter = {"}"};
        spec.testRunner = [](const std::string& code) -> std::vector<CodeTestCase> {
            bool hasBounds = (code.find("k < n") != std::string::npos || code.find("k<n") != std::string::npos ||
                              code.find("k <= n - 1") != std::string::npos);
            bool hasLower = (code.find("k >= 0") != std::string::npos || code.find("k>=0") != std::string::npos ||
                             code.find("0 <= k") != std::string::npos);
            bool returnsNeg = (code.find("-1") != std::string::npos);
            bool returnsVal = (code.find("arr[k]") != std::string::npos);

            bool valid = hasBounds && hasLower && returnsNeg && returnsVal;
            return {
                {"Standard Index", "arr=[10, 20, 30], n=3, k=1", "20", valid ? "20" : "ERR", valid},
                {"Upper OOB Bound", "arr=[5, 15], n=2, k=2", "-1", valid ? "-1" : "5", valid},
                {"Negative OOB Bound", "arr=[42], n=1, k=-1", "-1", valid ? "-1" : "42", valid}
            };
        };
        spec.onComplete = [this, demon](bool success) {
            if (success) {
                SFXManager::instance().play("hit");
                SFXManager::instance().play("success");
                demon->hp = 0; demon->defeated = true; demon->defeatAnim = 1.5f;
                novaGateUnlocked_ = true;
                applyPersistentState();
                dialogueUI_.start("INDEX GORGON", "TESTS PASSED! ZERO MEMORY LEAKS!", "Your safe indexing breached my armor! Route 1 is open!");
            } else {
                SFXManager::instance().play("hover");
                dialogueUI_.start("INDEX GORGON", "WRONG ANSWER! Bounds violated!", "Bounds checking [0, n-1] is required before accessing!");
            }
        };
        codeEditorUI_.startEditor(spec);
    }
    // 2. Search Wraith (Binary Search Bounds)
    else if (demon->id == "demon_search") {
        ProblemSpec spec;
        spec.id = "lc2";
        spec.title = "2. Binary Search Midpoint Pivot";
        spec.difficulty = "Easy";
        spec.category = "Binary Search";
        spec.description = "Given lower index bound 'low' and upper bound 'high', compute the safe middle pivot index to avoid 32-bit integer arithmetic overflow when low + high exceeds INT_MAX.";
        spec.examples = {
            "Input: low = 0, high = 10\nOutput: 5\nExplanation: Midpoint is (0 + 10) / 2 = 5.",
            "Input: low = 1000, high = 2000\nOutput: 1500\nExplanation: low + (high - low) / 2 = 1000 + 500 = 1500."
        };
        spec.constraints = {
            "0 <= low <= high <= 2 * 10^9",
            "Time Complexity: O(1)",
            "Space Complexity: O(1)"
        };
        spec.templateBefore = {"int getMid(int low, int high) {"};
        spec.defaultEditableCode = "    return low + (high - low) / 2;";
        spec.templateAfter = {"}"};
        spec.testRunner = [](const std::string& code) -> std::vector<CodeTestCase> {
            bool safeMid = (code.find("low + (high - low) / 2") != std::string::npos ||
                            code.find("(high - low) / 2") != std::string::npos ||
                            code.find("low + (high - low)/2") != std::string::npos ||
                            code.find("(low + high) / 2") != std::string::npos);
            return {
                {"Standard Range", "low=0, high=10", "5", safeMid ? "5" : "0", safeMid},
                {"Single Element", "low=4, high=4", "4", safeMid ? "4" : "0", safeMid},
                {"Large Int Range", "low=1000, high=2000", "1500", safeMid ? "1500" : "0", safeMid}
            };
        };
        spec.onComplete = [this, demon](bool success) {
            if (success) {
                SFXManager::instance().play("hit");
                SFXManager::instance().play("success");
                demon->hp = 0; demon->defeated = true; demon->defeatAnim = 1.5f;
                routeBridgeUnlocked_ = true;
                applyPersistentState();
                dialogueUI_.start("SEARCH WRAITH", "TESTS PASSED! LOGARITHMIC CONQUEST!", "The bridge across the chasm is complete!");
            } else {
                SFXManager::instance().play("hover");
                dialogueUI_.start("SEARCH WRAITH", "WRONG ANSWER! Pivot calculation failed!", "Ensure mid = low + (high - low) / 2;");
            }
        };
        codeEditorUI_.startEditor(spec);
    }
    // 3. Cycle Hydra (Floyd Cycle Detection)
    else if (demon->id == "demon_hydra") {
        ProblemSpec spec;
        spec.id = "lc3";
        spec.title = "3. Floyd Cycle Detection (Tortoise & Hare)";
        spec.difficulty = "Medium";
        spec.category = "Two Pointers";
        spec.description = "Advance the slow and fast pointers to detect pointer loops in a singly-linked chain. The tortoise advances by 1 node while the hare advances by 2 nodes simultaneously.";
        spec.examples = {
            "Input: head = [3, 2, 0, -4], cycle at index 1\nOutput: true\nExplanation: Fast and slow meet at node 2.",
            "Input: head = [1, 2], no cycle\nOutput: false\nExplanation: Fast reaches nullptr."
        };
        spec.constraints = {
            "0 <= nodes <= 10^4",
            "Time Complexity: O(N)",
            "Space Complexity: O(1) Auxiliary Space"
        };
        spec.templateBefore = {"void stepPointers(Node*& slow, Node*& fast) {"};
        spec.defaultEditableCode = "    slow = slow->next; fast = fast->next->next;";
        spec.templateAfter = {"}"};
        spec.testRunner = [](const std::string& code) -> std::vector<CodeTestCase> {
            bool validSlow = (code.find("slow = slow->next") != std::string::npos || code.find("slow=slow->next") != std::string::npos);
            bool validFast = (code.find("fast->next->next") != std::string::npos);
            bool valid = validSlow && validFast;
            return {
                {"Tortoise Step", "slow->val=1 -> next", "2", valid ? "2" : "1", valid},
                {"Hare 2-Step", "fast->val=1 -> next->next", "3", valid ? "3" : "1", valid},
                {"Relative Delta", "fast_speed - slow_speed", "1 node/step", valid ? "1 node/step" : "0", valid}
            };
        };
        spec.onComplete = [this, demon](bool success) {
            if (success) {
                SFXManager::instance().play("hit");
                SFXManager::instance().play("success");
                demon->hp = 0; demon->defeated = true; demon->defeatAnim = 1.5f;
                dialogueUI_.start("CYCLE HYDRA", "TESTS PASSED! THE LOOP IS BROKEN!", "Portal to Stack Citadel unsealed!");
            } else {
                SFXManager::instance().play("hover");
                dialogueUI_.start("CYCLE HYDRA", "WRONG ANSWER! Pointer step mismatch!", "Tortoise steps 1 node; Hare steps 2 nodes!");
            }
        };
        codeEditorUI_.startEditor(spec);
    }
    // 4. Overflow Titan (LIFO Call Stack Pop)
    else if (demon->id == "demon_overflow") {
        ProblemSpec spec;
        spec.id = "lc4";
        spec.title = "4. Call Stack LIFO Frame Pop";
        spec.difficulty = "Easy";
        spec.category = "Stack (LIFO)";
        spec.description = "Manage activation records on the execution stack. Extract and pop the most recently pushed call frame from stack 's' and return its value while maintaining LIFO ordering.";
        spec.examples = {
            "Input: stack = [10, 20, 30] (30 on top)\nOutput: 30\nExplanation: 30 was popped and stack now contains [10, 20]."
        };
        spec.constraints = {
            "Stack is guaranteed non-empty",
            "Time Complexity: O(1)",
            "Space Complexity: O(1)"
        };
        spec.templateBefore = {"int popFrame(std::stack<int>& s) {"};
        spec.defaultEditableCode = "    int top = s.top(); s.pop(); return top;";
        spec.templateAfter = {"}"};
        spec.testRunner = [](const std::string& code) -> std::vector<CodeTestCase> {
            bool hasTop = (code.find("s.top()") != std::string::npos);
            bool hasPop = (code.find("s.pop()") != std::string::npos);
            bool returns = (code.find("return") != std::string::npos);
            bool valid = hasTop && hasPop && returns;
            return {
                {"Top Peek", "stack=[10, 20, 30]", "30", valid ? "30" : "10", valid},
                {"LIFO Invariant", "size_after_pop", "2", valid ? "2" : "3", valid},
                {"Return Value", "popped_element", "30", valid ? "30" : "0", valid}
            };
        };
        spec.onComplete = [this, demon](bool success) {
            if (success) {
                SFXManager::instance().play("hit");
                SFXManager::instance().play("success");
                demon->hp = 0; demon->defeated = true; demon->defeatAnim = 1.5f;
                dialogueUI_.start("OVERFLOW TITAN", "TESTS PASSED! STACK FRAME CLEANED!", "The stairs to Binary Tree Grove are open!");
            } else {
                SFXManager::instance().play("hover");
                dialogueUI_.start("OVERFLOW TITAN", "WRONG ANSWER! Memory stack corrupted!", "Store s.top(), call s.pop(), then return top!");
            }
        };
        codeEditorUI_.startEditor(spec);
    }
    // 5. Tree Leviathan (BST In-Order Traversal)
    else if (demon->id == "demon_tree") {
        ProblemSpec spec;
        spec.id = "lc5";
        spec.title = "5. Binary Search Tree In-Order Traversal";
        spec.difficulty = "Medium";
        spec.category = "Binary Trees";
        spec.description = "Traverse a Binary Search Tree (BST) in-order so that keys are processed in strictly ascending sorted order: recursively visit root->left, record root->val, then visit root->right.";
        spec.examples = {
            "Input: root = [20, 10, 30]\nOutput: [10, 20, 30]\nExplanation: Left child 10, root 20, right child 30."
        };
        spec.constraints = {
            "0 <= node count <= 10^4",
            "Time Complexity: O(N)",
            "Space Complexity: O(H) recursion stack"
        };
        spec.templateBefore = {"void inorder(Node* root, vector<int>& out) {"};
        spec.defaultEditableCode = "    inorder(root->left, out); out.push_back(root->val); inorder(root->right, out);";
        spec.templateAfter = {"}"};
        spec.testRunner = [](const std::string& code) -> std::vector<CodeTestCase> {
            bool hasLeft = (code.find("root->left") != std::string::npos);
            bool hasRight = (code.find("root->right") != std::string::npos);
            bool hasRoot = (code.find("root->val") != std::string::npos || code.find("out.push") != std::string::npos);
            bool valid = hasLeft && hasRight && hasRoot;
            return {
                {"Sorted Root", "BST: root=20, left=10, right=30", "[10, 20, 30]", valid ? "[10, 20, 30]" : "[20]", valid},
                {"Left Subtree", "BST Left Subtree", "visited first", valid ? "visited first" : "root first", valid},
                {"Right Subtree", "BST Right Subtree", "visited last", valid ? "visited last" : "none", valid}
            };
        };
        spec.onComplete = [this, demon](bool success) {
            if (success) {
                SFXManager::instance().play("hit");
                SFXManager::instance().play("success");
                demon->hp = 0; demon->defeated = true; demon->defeatAnim = 1.5f;
                dialogueUI_.start("TREE LEVIATHAN", "TESTS PASSED! TREE INVARIANT HARMONIZED!", "The portal to Hash Hollows is unsealed!");
            } else {
                SFXManager::instance().play("hover");
                dialogueUI_.start("TREE LEVIATHAN", "WRONG ANSWER! Traversal not in-order!", "In-order requires: Left -> Root -> Right!");
            }
        };
        codeEditorUI_.startEditor(spec);
    }
    // 6. Hash Wyrm (Hash Function & Chaining)
    else if (demon->id == "demon_hash") {
        ProblemSpec spec;
        spec.id = "lc6";
        spec.title = "6. Hash Table Bucket Index";
        spec.difficulty = "Easy";
        spec.category = "Hash Map";
        spec.description = "Implement bucket distribution for a hash map. Map an integer key into a valid bucket slot index within [0, cap - 1] using modulo arithmetic: ((key % cap) + cap) % cap.";
        spec.examples = {
            "Input: key = 15, cap = 8\nOutput: 7\nExplanation: 15 % 8 = 7.",
            "Input: key = 8, cap = 8\nOutput: 0\nExplanation: 8 % 8 = 0."
        };
        spec.constraints = {
            "1 <= cap <= 10^5",
            "Time Complexity: O(1)",
            "Space Complexity: O(1)"
        };
        spec.templateBefore = {"int getBucket(int key, int cap) {"};
        spec.defaultEditableCode = "    return ((key % cap) + cap) % cap;";
        spec.templateAfter = {"}"};
        spec.testRunner = [](const std::string& code) -> std::vector<CodeTestCase> {
            bool hasMod = (code.find("% cap") != std::string::npos || code.find("%cap") != std::string::npos ||
                           code.find("key %") != std::string::npos);
            return {
                {"Positive Key", "key=15, cap=8", "7", hasMod ? "7" : "0", hasMod},
                {"Zero Key", "key=0, cap=8", "0", hasMod ? "0" : "-1", hasMod},
                {"Exact Capacity", "key=8, cap=8", "0", hasMod ? "0" : "8", hasMod}
            };
        };
        spec.onComplete = [this, demon](bool success) {
            if (success) {
                SFXManager::instance().play("hit");
                SFXManager::instance().play("success");
                demon->hp = 0; demon->defeated = true; demon->defeatAnim = 1.5f;
                dialogueUI_.start("HASH WYRM", "TESTS PASSED! COLLISION MATRIX STABILIZED!", "Path to Priority Delta / Heap River is open!");
            } else {
                SFXManager::instance().play("hover");
                dialogueUI_.start("HASH WYRM", "WRONG ANSWER! Hash bucket out of bounds!", "Compute bucket using modulo: key % cap;");
            }
        };
        codeEditorUI_.startEditor(spec);
    }
    // 7. Heap Behemoth (Min-Heap Invariant)
    else if (demon->id == "demon_heap") {
        ProblemSpec spec;
        spec.id = "lc7";
        spec.title = "7. Min-Heap Parent-Child Invariant";
        spec.difficulty = "Easy";
        spec.category = "Priority Queue";
        spec.description = "Verify the min-heap ordering invariant: each parent node's key must be less than or equal to both of its children keys. Return true if parentVal <= childVal.";
        spec.examples = {
            "Input: parentVal = 5, childVal = 10\nOutput: true\nExplanation: 5 <= 10 satisfies min-heap property.",
            "Input: parentVal = 20, childVal = 10\nOutput: false\nExplanation: Parent is larger than child (invariant violated)."
        };
        spec.constraints = {
            "-10^6 <= key <= 10^6",
            "Time Complexity: O(1)",
            "Space Complexity: O(1)"
        };
        spec.templateBefore = {"bool isMinHeapValid(int parentVal, int childVal) {"};
        spec.defaultEditableCode = "    return parentVal <= childVal;";
        spec.templateAfter = {"}"};
        spec.testRunner = [](const std::string& code) -> std::vector<CodeTestCase> {
            bool validCheck = (code.find("parentVal <= childVal") != std::string::npos ||
                               code.find("parentVal < childVal") != std::string::npos ||
                               code.find("childVal >= parentVal") != std::string::npos);
            return {
                {"Valid Parent", "parent=5, child=10", "true", validCheck ? "true" : "false", validCheck},
                {"Invalid Parent", "parent=20, child=10", "false", validCheck ? "false" : "true", validCheck},
                {"Equal Keys", "parent=7, child=7", "true", validCheck ? "true" : "false", validCheck}
            };
        };
        spec.onComplete = [this, demon](bool success) {
            if (success) {
                SFXManager::instance().play("hit");
                SFXManager::instance().play("success");
                demon->hp = 0; demon->defeated = true; demon->defeatAnim = 1.5f;
                dialogueUI_.start("HEAP BEHEMOTH", "TESTS PASSED! MIN-HEAP INVARIANT SECURED!", "North pass to Graph Catacombs is cleared!");
            } else {
                SFXManager::instance().play("hover");
                dialogueUI_.start("HEAP BEHEMOTH", "WRONG ANSWER! Heap ordering violated!", "Min-heap requires parent <= child!");
            }
        };
        codeEditorUI_.startEditor(spec);
    }
    // 8. Cycle Lich (BFS Level Order)
    else if (demon->id == "demon_graph") {
        ProblemSpec spec;
        spec.id = "lc8";
        spec.title = "8. Breadth-First Search (BFS) Traversal";
        spec.difficulty = "Medium";
        spec.category = "Graphs & Queues";
        spec.description = "Explore graph vertices level by level to compute shortest paths. When examining an unvisited neighbor 'u', mark vis[u] = true and push into queue q: q.push(u);.";
        spec.examples = {
            "Input: u = 2, vis[2] = false\nOutput: vis[2] becomes true, 2 pushed to queue.",
            "Input: u = 1, vis[1] = true\nOutput: duplicate skipped."
        };
        spec.constraints = {
            "1 <= V <= 10^4",
            "1 <= E <= 10^5",
            "Time Complexity: O(V + E)",
            "Space Complexity: O(V)"
        };
        spec.templateBefore = {"void visitNeighbor(queue<int>& q, vector<bool>& vis, int u) {"};
        spec.defaultEditableCode = "    if (!vis[u]) { vis[u] = true; q.push(u); }";
        spec.templateAfter = {"}"};
        spec.testRunner = [](const std::string& code) -> std::vector<CodeTestCase> {
            bool hasPush = (code.find("q.push(u)") != std::string::npos || code.find("q.push") != std::string::npos);
            bool hasVis = (code.find("vis[u] = true") != std::string::npos || code.find("vis[u]=true") != std::string::npos ||
                           code.find("!vis[u]") != std::string::npos);
            bool valid = hasPush && hasVis;
            return {
                {"Unvisited Node", "vis[2]=false, u=2", "pushed, vis=true", valid ? "pushed, vis=true" : "err", valid},
                {"Visited Node", "vis[1]=true, u=1", "skip duplicate", valid ? "skip duplicate" : "pushed duplicate", valid},
                {"FIFO Order", "queue invariant", "level-order", valid ? "level-order" : "err", valid}
            };
        };
        spec.onComplete = [this, demon](bool success) {
            if (success) {
                SFXManager::instance().play("hit");
                SFXManager::instance().play("success");
                demon->hp = 0; demon->defeated = true; demon->defeatAnim = 1.5f;
                dialogueUI_.start("CYCLE LICH", "TESTS PASSED! BFS REACHED ALL VERTICES!", "Gateway to Memoization Sanctum is open!");
            } else {
                SFXManager::instance().play("hover");
                dialogueUI_.start("CYCLE LICH", "WRONG ANSWER! Infinite cycle detected!", "Mark visited: vis[u] = true; before q.push(u);");
            }
        };
        codeEditorUI_.startEditor(spec);
    }
    // 9. Subproblem Colossus (DP Memoization)
    else if (demon->id == "demon_memo") {
        ProblemSpec spec;
        spec.id = "lc9";
        spec.title = "9. Dynamic Programming Subproblem Memoization";
        spec.difficulty = "Medium";
        spec.category = "Dynamic Programming";
        spec.description = "Eliminate exponential recomputation by caching optimal subproblems. If state 'n' is already computed (dp[n] != -1), immediately return cached dp[n]; otherwise return -1 to signal computation.";
        spec.examples = {
            "Input: n = 5, dp[5] = 8\nOutput: 8\nExplanation: Cache hit! Returns in O(1) time without re-branching.",
            "Input: n = 6, dp[6] = -1\nOutput: -1\nExplanation: Cache miss. Compute and store."
        };
        spec.constraints = {
            "0 <= n <= 10^5",
            "Time Complexity: O(1) lookup",
            "Space Complexity: O(N) memo table"
        };
        spec.templateBefore = {"int checkMemo(vector<int>& dp, int n) {"};
        spec.defaultEditableCode = "    if (dp[n] != -1) return dp[n]; return -1;";
        spec.templateAfter = {"}"};
        spec.testRunner = [](const std::string& code) -> std::vector<CodeTestCase> {
            bool hasCheck = (code.find("dp[n] != -1") != std::string::npos || code.find("dp[n]!=-1") != std::string::npos ||
                             code.find("dp[n] >= 0") != std::string::npos);
            bool hasReturn = (code.find("return dp[n]") != std::string::npos);
            bool valid = hasCheck && hasReturn;
            return {
                {"Memo Cache Hit", "dp[5]=8 (computed)", "8 [O(1)]", valid ? "8 [O(1)]" : "recompute", valid},
                {"Memo Cache Miss", "dp[6]=-1 (unvisited)", "-1 [compute]", valid ? "-1 [compute]" : "invalid", valid},
                {"Subproblem Invariant", "time complexity", "O(N) total", valid ? "O(N) total" : "O(2^N)", valid}
            };
        };
        spec.onComplete = [this, demon](bool success) {
            if (success) {
                SFXManager::instance().play("hit");
                SFXManager::instance().play("success");
                demon->hp = 0; demon->defeated = true; demon->defeatAnim = 1.5f;
                dialogueUI_.start("SUBPROBLEM COLOSSUS", "TESTS PASSED! OPTIMAL SUBSTRUCTURE UNLOCKED!", "The doors to the Master Sanctum open!");
            } else {
                SFXManager::instance().play("hover");
                dialogueUI_.start("SUBPROBLEM COLOSSUS", "WRONG ANSWER! Subproblem recomputed!", "If dp[n] != -1, return cached dp[n];");
            }
        };
        codeEditorUI_.startEditor(spec);
    }
}

void GBAOverworldScene::handleInteraction() {
    int fx, fy;
    player_->getFacingTile(fx, fy);

    // 1. Guardian Demon Fight Interaction
    GuardianDemon* demon = getFacingDemon(fx, fy);
    if (demon) {
        fightDemon(demon);
        return;
    }

    // 2. Wandering NPCs
    const WanderingNPC* wnpc = getFacingNPC(fx, fy);
    if (wnpc) {
        SFXManager::instance().play("click");
        dialogueUI_.start(wnpc->name, wnpc->text1, wnpc->text2);
        return;
    }

    // 3. Scholar NPC in Nova Town
    if (currentMapName_ == "nova_town" && fx == scholarTileX_ && fy == scholarTileY_) {
        SFXManager::instance().play("click");
        if (!novaGateUnlocked_) {
            dialogueUI_.start("SCHOLAR ADA",
                "Greetings, Kael! The Index Gorgon blocks the eastern pass.",
                "Confront the demon [Z] and test your C++ algorithm in the IDE!");
        } else {
            dialogueUI_.start("SCHOLAR ADA",
                "Brilliant code execution! The Gorgon was banished.",
                "Travel east across all 10 sectors to restore the Algorithmic Core!");
        }
        return;
    }

    // 4. Golden Code Chests (Tile 20)
    int groundTile = currentMap_->getGroundTile(fx, fy);
    if (groundTile == 20) {
        ProblemSpec spec;
        spec.id = "chest1";
        spec.title = "Algorithmic Matrix Validation";
        spec.difficulty = "Easy";
        spec.category = "Capacity Preconditions";
        spec.description = "Validate dynamic memory allocation preconditions. Verify if the container capacity 'cap' is strictly positive (> 0) before reserving memory buffers.";
        spec.examples = {
            "Input: cap = 10\nOutput: true\nExplanation: Positive capacity allows valid allocation.",
            "Input: cap = 0\nOutput: false\nExplanation: Zero capacity cannot allocate buffer."
        };
        spec.constraints = {
            "-10^6 <= cap <= 10^6",
            "Time Complexity: O(1)",
            "Space Complexity: O(1)"
        };
        spec.templateBefore = {"bool isValidCapacity(int cap) {"};
        spec.defaultEditableCode = "    return cap > 0;";
        spec.templateAfter = {"}"};
        spec.testRunner = [](const std::string& code) -> std::vector<CodeTestCase> {
            bool valid = (code.find("cap > 0") != std::string::npos || code.find("cap>0") != std::string::npos ||
                          code.find("0 < cap") != std::string::npos);
            return {
                {"Positive Cap", "cap=10", "true", valid ? "true" : "false", valid},
                {"Zero Cap", "cap=0", "false", valid ? "false" : "true", valid},
                {"Negative Cap", "cap=-5", "false", valid ? "false" : "true", valid}
            };
        };
        spec.onComplete = [this](bool success) {
            if (success) {
                if (currentMapName_ == "nova_town") novaChestOpened_ = true;
                else if (currentMapName_ == "memory_route") routeChestOpened_ = true;
                else if (currentMapName_ == "linked_cavern") cavernChestOpened_ = true;
                else if (currentMapName_ == "stack_citadel") citadelChestOpened_ = true;
                else if (currentMapName_ == "tree_grove") groveChestOpened_ = true;
                else if (currentMapName_ == "hash_hollows") hashChestOpened_ = true;
                else if (currentMapName_ == "heap_river") heapChestOpened_ = true;
                else if (currentMapName_ == "graph_catacombs") graphChestOpened_ = true;
                else if (currentMapName_ == "memo_sanctum") memoChestOpened_ = true;

                applyPersistentState();
                dialogueUI_.start("CHEST", "OBTAINED: [ALGORITHM MATRIX GEM] +50 XP!", "Equipped core data structure logic.");
            } else {
                dialogueUI_.start("CHEST", "CHEST LOCKED: Logic error.", "Capacity must be strictly positive (cap > 0).");
            }
        };
        codeEditorUI_.startEditor(spec);
        return;
    }

    // 5. Map Interactables (Altar, Save Crystal, Well, Lantern, Signs)
    const MapInteractable* it = currentMap_->checkInteractable(fx, fy);
    if (it) {
        if (it->type == "ALTAR") {
            bool allDemonsDefeated = true;
            for (const auto& d : demons_) {
                if (!d.defeated) { allDemonsDefeated = false; break; }
            }
            if (allDemonsDefeated) {
                SFXManager::instance().play("success");
                sanctumMasterCleared_ = true;
                dialogueUI_.start("MASTER ARCHIVE",
                    "ALL 9 GUARDIAN DEMONS BANISHED! COMPILATION COMPLETE!",
                    "Kael & Nova have restored all DSA foundations! YOU WIN RUNTIME!");
            } else {
                SFXManager::instance().play("click");
                dialogueUI_.start("MASTER ARCHIVE",
                    "The Algorithmic Core requires all 9 Guardian Demons to be banished.",
                    "Explore all 10 sectors and test your code against each guardian!");
            }
        } else if (it->type == "SAVE_CRYSTAL") {
            SFXManager::instance().play("success");
            dialogueUI_.start("SYSTEM", "ARCHIVE SAVED: Progress synchronized.", "Memory state committed to cartridge memory!");
        } else if (it->type == "WELL") {
            SFXManager::instance().play("success");
            dialogueUI_.start("CRYSTAL WELL",
                "Deep pure crystal water resonates with memory registers.",
                "Your party health and CPU cycles are fully restored!");
        } else if (it->type == "LANTERN") {
            SFXManager::instance().play("hover");
            dialogueUI_.start("STREET LANTERN",
                "Emitting a warm amber algorithmic beacon.",
                "Illuminating memory addresses across Sector Alpha.");
        } else {
            SFXManager::instance().play("click");
            dialogueUI_.start("SIGN", it->text);
        }
        return;
    }
}

void GBAOverworldScene::checkEncounter() {
    if (!player_->isMoving()) return;

    if (currentMap_->isTallGrass(player_->getTileX(), player_->getTileY())) {
        grassEncounterAccum_ += 0.08f;
        if (grassEncounterAccum_ >= 1.0f) {
            grassEncounterAccum_ = 0.0f;
            SFXManager::instance().play("scan");
            Game::getSceneManager().pushScene(std::make_unique<GBABattleScene>("GLITCH NODE", 80));
        }
    }
}

void GBAOverworldScene::update(float dt) {
    MusicManager::instance().update(dt);

    // 1. Code Editor UI takes first priority
    if (codeEditorUI_.isActive()) {
        codeEditorUI_.update(dt);
        return;
    }

    // 2. Dialogue takes second priority
    if (dialogueUI_.isActive()) {
        dialogueUI_.update(dt);
        return;
    }

    // 3. Update demon animations & particles & wandering NPCs
    for (auto& d : demons_) {
        if (d.defeatAnim > 0.0f) {
            d.defeatAnim -= dt;
        }
    }
    updateParticles(dt);
    updateNPCs(dt);

    // 4. Interaction check (Z, SPACE, ENTER)
    if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        handleInteraction();
    }

    // 5. Player movement with demon collision check
    player_->handleInput(currentMap_.get(), dt, [this](int tx, int ty) {
        return isTileBlockedByDemon(tx, ty);
    });
    player_->update(dt);

    // 6. Check warps when player finishes step
    if (!player_->isMoving()) {
        const WarpPoint* warp = currentMap_->checkWarp(player_->getTileX(), player_->getTileY());
        if (warp) {
            switchMap(warp->targetMap, warp->targetX, warp->targetY);
            return;
        }
        checkEncounter();
    }

    // 7. Camera follows player
    Vector2 pPos = player_->getPosition();
    camera_.target.x += (pPos.x + 8.0f - camera_.target.x) * 10.0f * dt;
    camera_.target.y += (pPos.y + 8.0f - camera_.target.y) * 10.0f * dt;

    // 8. Nova floating follower
    float time = static_cast<float>(GetTime());
    float floatOffset = sinf(time * 4.0f) * 3.0f;
    Vector2 targetNova = {pPos.x - 12.0f, pPos.y - 10.0f + floatOffset};
    novaPos_.x += (targetNova.x - novaPos_.x) * 6.0f * dt;
    novaPos_.y += (targetNova.y - novaPos_.y) * 6.0f * dt;
}

void GBAOverworldScene::drawHUD() const {
    // Current Quest Tracker at top-right
    std::string demonTarget = "Index Gorgon";
    for (const auto& d : demons_) {
        if (d.map == currentMapName_ && !d.defeated) {
            demonTarget = d.name;
            break;
        }
    }
    std::string hudText = TextFormat("QUEST: Defeat %s", demonTarget.c_str());
    int textW = MeasureText(hudText.c_str(), 6);
    DrawRectangleRounded(Rectangle{static_cast<float>(GBA_SCREEN_WIDTH - textW - 12), 4.0f, static_cast<float>(textW + 8), 11.0f}, 0.2f, 2, GBA_FRAME_DARK);
    DrawText(hudText.c_str(), GBA_SCREEN_WIDTH - textW - 8, 6, 6, GBA_GOLD);
}

void GBAOverworldScene::draw() {
    ClearBackground(GBA_BLACK);

    // 1. Render Tilemap, Entities, Particles under Camera
    BeginMode2D(camera_);
    currentMap_->draw(camera_);

    // Draw Particles
    drawParticles();

    // Draw Wandering NPCs
    drawNPCs();

    // Draw Scholar NPC (in Nova Town)
    if (currentMapName_ == "nova_town" && scholarLoaded_ && scholarSprite_.id > 0) {
        float time = static_cast<float>(GetTime());
        int frame = (static_cast<int>(time * 2.0f) % 2 == 0) ? 0 : 1;
        DrawTextureRec(scholarSprite_, Rectangle{static_cast<float>(frame * 16), 0.0f, 16.0f, 20.0f},
                       Vector2{static_cast<float>(scholarTileX_ * 16), static_cast<float>(scholarTileY_ * 16 - 4)}, WHITE);
        int pDistX = std::abs(player_->getTileX() - scholarTileX_);
        int pDistY = std::abs(player_->getTileY() - scholarTileY_);
        if (pDistX <= 1 && pDistY <= 1) {
            DrawText("[Z]", scholarTileX_ * 16 + 2, scholarTileY_ * 16 - 8, 7, GBA_GOLD);
        }
    }

    // Draw Guardian Demons
    for (auto& d : demons_) {
        if (d.map == currentMapName_) {
            if (!d.defeated && d.loaded && d.sprite.id > 0) {
                float time = static_cast<float>(GetTime());
                float hover = sinf(time * 5.0f) * 2.5f;

                DrawEllipse(d.tileX * 16 + 8, d.tileY * 16 + 14, 11, 4, Color{190, 25, 45, 140});
                DrawTexture(d.sprite, d.tileX * 16 - 10, d.tileY * 16 - 16 + static_cast<int>(hover), WHITE);

                // Demon HP Bar
                DrawRectangle(d.tileX * 16 - 2, d.tileY * 16 - 20 + static_cast<int>(hover), 20, 4, GBA_FRAME_DARK);
                DrawRectangle(d.tileX * 16 - 1, d.tileY * 16 - 19 + static_cast<int>(hover), static_cast<int>(18 * (d.hp / 100.0f)), 2, RED);

                int pDist = std::abs(player_->getTileX() - d.tileX) + std::abs(player_->getTileY() - d.tileY);
                if (pDist <= 2) {
                    DrawRectangleRounded(Rectangle{static_cast<float>(d.tileX * 16 - 14), static_cast<float>(d.tileY * 16 - 31 + hover), 44.0f, 9.0f}, 0.2f, 2, GBA_FRAME_DARK);
                    DrawText("[Z] CODE!", d.tileX * 16 - 11, static_cast<int>(d.tileY * 16 - 30 + hover), 6, GBA_GOLD);
                }
            } else if (d.defeatAnim > 0.0f) {
                float r = (1.5f - d.defeatAnim) * 22.0f;
                DrawCircleLines(d.tileX * 16 + 8, d.tileY * 16 + 8, r, GBA_GOLD);
                DrawCircleLines(d.tileX * 16 + 8, d.tileY * 16 + 8, r * 0.7f, RED);
            }
        }
    }

    // Draw Nova Companion
    if (novaLoaded_ && novaSprite_.id > 0) {
        DrawTexture(novaSprite_, static_cast<int>(novaPos_.x), static_cast<int>(novaPos_.y), WHITE);
    }

    // Draw Player
    player_->draw();

    EndMode2D();

    // 2. Overlays: Dialogue UI and Code Editor UI
    dialogueUI_.draw();
    codeEditorUI_.draw();

    // 3. Location banner and HUD
    if (!dialogueUI_.isActive() && !codeEditorUI_.isActive()) {
        DrawRectangleRounded(Rectangle{4, 4, static_cast<float>(MeasureText(currentMap_->getName().c_str(), 7) + 8), 11}, 0.2f, 2, GBA_FRAME_DARK);
        DrawText(currentMap_->getName().c_str(), 7, 6, 7, GBA_WHITE);

        drawHUD();

        int fx, fy;
        player_->getFacingTile(fx, fy);
        int facingTile = currentMap_->getGroundTile(fx, fy);
        const MapInteractable* it = currentMap_->checkInteractable(fx, fy);
        bool isNearScholar = (currentMapName_ == "nova_town" && fx == scholarTileX_ && fy == scholarTileY_);
        GuardianDemon* facingDemon = getFacingDemon(fx, fy);
        const WanderingNPC* facingNPC = getFacingNPC(fx, fy);

        if (facingDemon != nullptr || facingTile == 20 || it != nullptr || isNearScholar || facingNPC != nullptr) {
            const char* prompt = "[Z] TALK";
            if (facingDemon != nullptr) prompt = "[Z] CODE TRIAL";
            else if (facingTile == 20) prompt = "[Z] CODE CHEST";
            else if (it != nullptr && it->type == "ALTAR") prompt = "[Z] MASTER ALTAR";
            DrawRectangleRounded(Rectangle{4.0f, GBA_SCREEN_HEIGHT - 14.0f, 74.0f, 10.0f}, 0.2f, 2, GBA_FRAME_DARK);
            DrawText(prompt, 6, GBA_SCREEN_HEIGHT - 12, 6, GBA_GOLD);
        }
    }
}

void GBAOverworldScene::drawScreenOverlay() {
    if (codeEditorUI_.isActive()) {
        codeEditorUI_.drawOverlay();
    }
}
