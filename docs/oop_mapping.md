# RUNTIME: OOP Concept Mapping & Viva Reference

This document maps all core Object-Oriented Programming (OOP) concepts to their exact C++ implementations in **RUNTIME: The Last Algorithm**, serving as a comprehensive viva guide for professors and examiners.

---

## 1. Master OOP Mapping Table

| OOP Concept | Project Location / Class | Description & Implementation Evidence |
| :--- | :--- | :--- |
| **Abstract Base Classes** | `src/entities/Entity.h`, `src/dsa/IDataStructure.h`, `src/core/SceneManager.h` | Contains pure virtual functions (`= 0`) and virtual destructors (`= default;`). Cannot be instantiated directly. |
| **Encapsulation** | `src/dsa/ArrayDS.h`, `src/core/Config.h`, `src/entities/Character.h` | Member variables (`data_`, `health_`, etc.) are protected/private; access is controlled via public member methods and invariants. |
| **Inheritance** | `Character : public Entity`, `Player : public Character`, `ArrayDS : public IDataStructure` | Hierarchical code reuse and specialized behavior. |
| **Runtime Polymorphism** | `SceneManager::update()`, `DSAVisualizer::draw()`, `Room::draw()` | Dynamic method dispatch through base class pointers (`IScene*`, `IDataStructure*`, `Entity*`). |
| **Interface Segregation** | `IDataStructure.h`, `IScene.h` | Clean, minimal contracts tailored specifically to subsystem roles. |
| **Operator Overloading** | Vector operations, math calculations | Mathematical ease in movement and bounding checks. |
| **Templates / Generics** | `src/core/EventBus.h`, `src/utils/Random.h` | Type-safe generic event dispatch (`EventBus::subscribe<T>()`) and random element selection (`pick<T>()`). |
| **RAII (Resource Acquisition Is Initialization)** | `Game::~Game()`, `AudioManager::~AudioManager()`, `LinkedList::~LinkedList()` | Resource lifecycles (windows, audio streams, dynamically allocated nodes) tied directly to object lifetime. |
| **Smart Pointers** | `std::unique_ptr` used throughout `Room`, `SceneManager`, `UISystem` | Ownership semantics enforced; zero memory leaks without manual `delete` calls. |
| **Standard Template Library (STL)** | Vectors, maps, sets, algorithms across all modules | `std::vector`, `std::map`, `std::unordered_map`, `std::string`, `std::function`, `std::type_index`. |
| **Exception Handling & Defensive Programming** | `src/save/SaveManager.cpp`, `src/save/SaveData.cpp` | Graceful JSON corruption handling, null checks, safe file I/O operations. |

---

## 2. In-Depth Viva Questions & Answers

### Q1: How is polymorphism used in the DSA visualizer?
**Answer:**  
In `src/ui/DSAVisualizer.h`, the visualization method accepts a pointer to the abstract base interface:
```cpp
void draw(IDataStructure* ds, float x, float y, float w, float h);
```
The visualizer does not know or care whether it is rendering an `ArrayDS`, `Stack`, `Queue`, or `LinkedList`. It calls:
```cpp
ds->visualize(x + 20, y + 100, w - 40, h - 200);
```
The compiler looks up the virtual method table (vtable) and calls the specific data structure's visualization routine at runtime.

### Q2: Why did you use `std::unique_ptr` instead of raw pointers for Scene and Entity management?
**Answer:**  
`std::unique_ptr` provides exclusive ownership semantics and automated deterministic memory deallocation (RAII). When a scene is popped from `SceneManager` or an entity is destroyed upon level unload, its memory is guaranteed to be freed without manual `delete` statements, preventing memory leaks, double free errors, and dangling pointers.

### Q3: How is the Open/Closed Principle (OCP) demonstrated?
**Answer:**  
The engine is open for extension but closed for modification. If a new data structure (e.g., `BinarySearchTree` or `Graph`) is added in a future chapter, we simply derive from `IDataStructure` and implement its pure virtual methods. We do not need to modify `DSAVisualizer`, `MasteryScreen`, or `SceneManager` to support it.

### Q4: How is the EventBus decoupled using modern C++?
**Answer:**  
`EventBus` uses `std::type_index` with C++ templates:
```cpp
template<typename EventType>
SubscriptionId subscribe(std::function<void(const EventType&)> callback);
```
Publishers and subscribers do not have compile-time dependencies on each other. When a dialogue line finishes and unlocks a DSA core, it publishes `DSAUnlockEvent`, which the progression and inventory systems receive without `DialogueSystem` needing pointers to either.

---

## 3. Architecture Inspector (Viva Mode) Guide

During viva presentations, the **Architecture Inspector** overlay can be activated anytime by pressing **[F1]** or **[V]**. It provides a live HUD showing:
1. **Active Scene Class & Inheritance Path**
2. **Current Polymorphic Dispatch Status**
3. **Active Data Structure and In-Memory Buffer Capacity**
4. **Current Algorithmic Complexity Class ($O(1)$ vs $O(n)$)**
5. **Memory Management Strategy (Smart Pointer Lifecycle)**
