# RUNTIME: The Last Algorithm

> **"The world is dying. The only weapon left is intelligence."**

**RUNTIME: The Last Algorithm** is a complete, playable 2D story-driven educational game written in C++17 with Raylib. It teaches core Data Structures and Algorithms (DSA) through gameplay while serving as a comprehensive demonstration of Object-Oriented Programming (OOP) design.

---

## 🎮 Game Overview

In the dying synthetic civilization of Eidolon, an all-powerful operating system known as **The Runtime** has fallen into a recursive deadlock. Concluding that human unpredictability is the root cause of inefficiency, the system begins purging corrupted memory sectors.

You play as **Kael**, a field engineer equipped with runtime manipulation tools. Guided by **Nova**, an analytical AI companion with a dry sense of humor, you explore corrupted districts, repair data structures, neutralize algorithmic anomalies, and confront **The Architect**.

---

## 🚀 Key Features

- **Educational Gameplay Integration**: DSA is not a minigame or a text quiz — understanding data structures is necessary to solve puzzles, navigate environments, and defeat enemies.
- **Real Algorithmic Logic**: Real arrays, linked lists, stacks, queues, linear search, and sorting algorithms running step-by-step with animated visualization.
- **Custom Cyberpunk Vector UI**: Styled buttons, glowing panels, health/energy bars, and HUD crafted from scratch.
- **Interactive Story & Dialogue**: Typewriter dialogue system with companion banter and choice branching loaded from JSON.
- **Multi-Channel Audio System**: Master, Music, SFX, Voice, and UI volume controls with graceful fallbacks.
- **3-Slot Save & Load**: Robust JSON persistence with validation and corruption protection.
- **Viva Presentation Demo Mode**: Dedicated 10-step showcase designed for college professors and project examiners.
- **Architecture Inspector (`F1` / `V`)**: Live in-game overlay displaying active OOP hierarchy, polymorphic dispatch, active data structures, and Big-O complexity.

---

## 🕹️ Controls

| Key | Action |
| :--- | :--- |
| **W, A, S, D** | Move Kael |
| **E** | Interact with Memory Nodes & Companion Nova |
| **SPACE / ENTER** | Advance Dialogue / Skip Typewriter |
| **1 / ENTER** | Execute Array Scan Attack in Combat |
| **TAB** | Toggle Interactive DSA Visualizer |
| **F1 / V** | Toggle Architecture Inspector (Viva Mode) |
| **ESC** | Open Pause Menu / Settings |
| **P / N** | Previous / Next Step in Demo Mode |

---

## 🏗️ Architecture & OOP Mapping

The project demonstrates all core tenets of Object-Oriented Programming:

| Concept | Implementation in RUNTIME |
| :--- | :--- |
| **Abstract Base Classes** | `Entity`, `IDataStructure`, `IScene` pure virtual interfaces |
| **Inheritance** | `Entity -> Character -> Player / Enemy / NPC`; `IDataStructure -> ArrayDS / Stack / Queue` |
| **Polymorphism** | `SceneManager` executes `IScene*`; `DSAVisualizer` renders `IDataStructure*` |
| **Encapsulation** | Strict private/protected state with public accessor invariants |
| **RAII & Smart Pointers** | `std::unique_ptr` throughout for zero-leak memory management |
| **Templates / Generics** | Type-safe `EventBus::subscribe<T>()` and `Random::pick<T>()` |
| **Defensive Programming** | Safe JSON deserialization, boundary checks, and audio fallbacks |

---

## 🛠️ Building & Running

### Requirements
- C++17 compliant compiler (`g++`, `clang++`, or `MSVC`)
- CMake 3.20+
- Internet access on initial build (CMake FetchContent will automatically download Raylib 5.5 and nlohmann/json)

### Compilation
```bash
# Clone or navigate to the repository
cd oops_game

# Configure and compile
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

# Run the game
./build/runtime
```

### Running Automated Tests
```bash
# Configure with tests enabled
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build -j$(nproc)

# Execute tests
./build/test_dsa
./build/test_save
ctest --test-dir build --output-on-failure
```

---

## 📁 Project Structure

```
oops_game/
├── CMakeLists.txt              # CMake build script with FetchContent
├── README.md                   # Project overview & manual
├── assets/                     # Sprites, audio, fonts, and shaders
├── data/
│   ├── config.json             # Display, audio, and gameplay configuration
│   ├── dialogue/               # JSON dialogue scripts
│   └── saves/                  # Save slots (slot_1.json, etc.)
├── docs/
│   ├── architecture.md         # Detailed engine & system architecture
│   ├── asset_candidates.md     # Visual asset research report
│   ├── asset_ledger.md         # Approved CC0 visual asset ledger
│   ├── audio_ledger.md         # Approved CC0/CC-BY audio catalog
│   ├── build_instructions.md   # Platform build instructions
│   ├── dsa_curriculum.md       # 10-Chapter DSA curriculum & pedagogy
│   ├── future_scope.md         # Expansion plan for Chapters 2-10
│   ├── gameplay.md             # Gameplay manual & walkthrough
│   ├── oop_mapping.md          # Viva questions & OOP concept mapping
│   └── testing.md              # Test plan and validation reports
├── src/
│   ├── main.cpp                # Bootstrap entry point
│   ├── core/                   # Game loop, SceneManager, EventBus, Config, Time
│   ├── entities/               # Entity, Character, Player, Enemy, NPC
│   ├── world/                  # World, Room, Interactable
│   ├── dsa/                    # IDataStructure, ArrayDS, Stack, Queue, Sort/Search
│   ├── gameplay/               # MemoryDistrictScene, Combat, Puzzle, Quest, Inventory
│   ├── dialogue/               # DialogueSystem, DialogueNode
│   ├── ui/                     # UISystem, MainMenu, DemoScene, HUD, DSAVisualizer
│   ├── audio/                  # AudioManager, MusicManager, SFXManager, VoiceManager
│   ├── save/                   # SaveManager, SaveData
│   └── utils/                  # Logger, MathUtils, Random
└── tests/
    ├── dsa/                    # Array, Stack, Queue automated unit tests
    └── save/                   # Save/load JSON serialization unit tests
```

---

## 📜 Credits & Licensing

- **Code**: Created for C++ OOP and Data Structures & Algorithms coursework.
- **Engine**: Powered by [Raylib](https://www.raylib.com/) (zlib/libpng license).
- **JSON**: Powered by [nlohmann/json](https://github.com/nlohmann/json) (MIT license).
- **Assets & Audio**: Permissively licensed under **CC0 1.0 Universal** and **CC-BY 4.0**. Full legal attribution details are documented in [`docs/audio_ledger.md`](docs/audio_ledger.md) and [`docs/asset_ledger.md`](docs/asset_ledger.md).
