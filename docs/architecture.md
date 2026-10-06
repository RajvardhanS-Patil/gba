# RUNTIME: Engine Architecture & System Design

## 1. Architectural Philosophy

**RUNTIME: The Last Algorithm** is engineered in modern C++17 with Raylib, following professional software engineering principles:
- **Clean separation of concerns**: Engine core, world representation, entity logic, algorithmic visualization, UI, audio, and persistence are decoupled into modular subsystems.
- **Robust Object-Oriented Design**: Full adherence to Encapsulation, Abstraction via pure virtual interfaces, Polymorphic dispatch, and Composition over inheritance where appropriate.
- **Zero-Stub Policy**: Every algorithm, data structure, visualizer, and gameplay state is fully implemented with real executable logic.
- **Fail-Safe Persistence & Resilience**: Robust JSON serialization with fallback error recovery for damaged save states or missing assets.

```
┌─────────────────────────────────────────────────────────────┐
│                         Game Loop                           │
│     (TimeManager -> Input -> SceneManager -> Renderer)      │
└──────────────┬──────────────────────────────┬───────────────┘
               │                              │
       ┌───────▼────────┐             ┌───────▼────────┐
       │  SceneManager  │             │   AudioManager │
       │  (Scene Stack) │             │ (Multi-Channel)│
       └───────┬────────┘             └────────────────┘
               │
    ┌──────────┴──────────────────────────────────────┐
    │                                                 │
┌───▼────────────────┐                     ┌──────────▼─────────┐
│     Gameplay       │                     │    DSA Subsystem   │
│ ┌────────────────┐ │                     │ ┌────────────────┐ │
│ │ World & Rooms  │ │   [Polymorphism]    │ │ IDataStructure │ │
│ └───────┬────────┘ │ <─────────────────> │ └───────┬────────┘ │
│ ┌───────▼────────┐ │                     │ ┌───────▼────────┐ │
│ │ Entity System  │ │                     │ │ ArrayDS, Stack │ │
│ └────────────────┘ │                     │ │ Queue, Lists   │ │
│ ┌────────────────┐ │                     │ └────────────────┘ │
│ │ Combat/Quests  │ │                     │ ┌────────────────┐ │
│ └────────────────┘ │                     │ │ DSAVisualizer  │ │
└────────────────────┘                     │ └────────────────┘ │
                                           └────────────────────┘
```

---

## 2. Core Engine Subsystems

### 2.1 The Game Class (`src/core/Game.h`)
- Central coordinating singleton owning the window lifecycle, main loop, audio device, configuration, and top-level managers.
- Controls the fixed/variable frame rate update via `TimeManager` (`dt`), delegating scene logic to `SceneManager`.

### 2.2 Scene Stack (`src/core/SceneManager.h`)
- Implements the **State Pattern** and **Scene Stack**:
  - `IScene` defines the pure virtual contract: `onEnter()`, `onExit()`, `update(float dt)`, `draw()`, `getName()`.
  - Supports non-destructive overlays (e.g., pushing `PauseMenu` on top of `MemoryDistrictScene` preserves gameplay state without teardown).
  - Handles screen transitions (fade-in, fade-out).

### 2.3 Event Bus (`src/core/EventBus.h`)
- Type-safe publish/subscribe event dispatcher using `std::type_index` and `std::function`.
- Emits events such as `DSAUnlockEvent`, `QuestUpdateEvent`, `DialogueStartEvent`, avoiding tight coupling between dialogue triggers and gameplay progression.

### 2.4 Configuration Manager (`src/core/Config.h`)
- Reads user preferences from `data/config.json`: display resolution, target FPS, fullscreen toggle, volume mix, and typewriter text speed.

---

## 3. Entity & World Subsystem

### 3.1 Inheritance Hierarchy
```
           Entity (Abstract Base Class)
             │  - Position, Velocity, Bounding Box
             │  - pure virtual: update(), draw(), getType()
             ▼
         Character
             │  - Health, Energy, State Machine
             ├───────────────────┬───────────────────┐
             ▼                   ▼                   ▼
           Player               Enemy               NPC
       (WASD Movement,    (Patrol AI, Weakness   (Dialogue Trigger,
        Interaction,       DSA Combat Phase)      Companion Nova)
        Abilities)
```

### 3.2 Tile-Based World (`src/world/World.h` & `src/world/Room.h`)
- `World` manages the active room and smooth 2D camera tracking (`Camera2D`) centered on the player.
- `Room` maintains collision data, floor tiles, accent markers, and collections of `std::unique_ptr<Entity>` and `std::unique_ptr<Interactable>`.

---

## 4. DSA Subsystem & Visualization Engine

### 4.1 `IDataStructure` Interface (`src/dsa/IDataStructure.h`)
```cpp
class IDataStructure {
public:
    virtual void insert(int value) = 0;
    virtual bool remove(int value) = 0;
    virtual bool search(int value) const = 0;
    virtual void visualize(float x, float y, float w, float h) const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getComplexityInfo() const = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual void reset() = 0;
    virtual ~IDataStructure() = default;
};
```
Every concrete data structure (`ArrayDS`, `Stack`, `Queue`, `LinkedList`) implements this interface, allowing the universal `DSAVisualizer` to render and interact with any structure polymorphically.

### 4.2 Algorithmic Accuracy
- **Real Memory Array (`ArrayDS`)**: Uses contiguous vectors with indexed random access ($O(1)$) and step-by-step linear search comparisons ($O(n)$).
- **Stack & Queue**: Maintain internal buffers with animated element transitions, enforcing LIFO and FIFO constraints respectively.
- **Sorting Algorithms**: `BubbleSort`, `SelectionSort`, `InsertionSort`, `MergeSort`, `QuickSort` record atomic `SortStep` snapshots (comparison indices, swaps, descriptions) for animated playback.
- **Complexity Analyzer**: Tracks execution operations against theoretical curves ($O(1)$, $O(\log n)$, $O(n)$, $O(n \log n)$, $O(n^2)$) and renders dynamic bar charts.

---

## 5. UI Pipeline & Cyberpunk Design System

- Custom-drawn vector widgets built with Raylib primitives (`DrawRectangleRounded`, `DrawRectangleRoundedLinesEx`, `DrawTriangle`).
- **Cohesive Cyberpunk Palette**:
  - `CYBER_CYAN` (`#00E5FF`): Interactive focal points, player energy, headers
  - `CYBER_VIOLET` (`#7C4DFF`): Companion Nova accents, secondary nodes
  - `CYBER_MAGENTA` (`#FF4081`): Enemies, corrupted anomalies
  - `CYBER_DARK` (`#0A0E17`): Deep carbon slate background
  - `CYBER_PANEL` (`#1A1E2E`): Semi-transparent terminal container panels
  - `CYBER_TEXT` (`#DCE6FF`): Primary readable typography

---

## 6. Persistence & Audio Pipeline

- **SaveManager**: Manages 3 save slots under `data/saves/`. Persists player stats, level, unlocked abilities, quest completions, and DSA concept mastery percentages in structured JSON.
- **AudioManager**: Independent volume sliders for Master, Music, SFX, Voice, and UI channels. Fully decoupled with graceful fallbacks if audio hardware or files are unavailable.
