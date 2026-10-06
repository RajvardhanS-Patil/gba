# RUNTIME: Future Scope & Expansion Roadmap

## 1. Chapters 2–10 Implementation Plan

With the core game loop, entity hierarchy, polymorphic visualizer, and persistence verified in Slice 1, future chapters will plug directly into the architecture:

### Chapter 2: Pointer Warrens (Linked Lists)
- **Mechanic**: Memory blocks are scattered non-contiguously across heap alleys.
- **Gameplay**: Player manipulates pointer chains (`next` pointers) to bridge physical gaps and bypass corrupted nodes in $O(1)$ time without shifting elements.

### Chapter 3: Recursion Abyss (Call Stack & DFS)
- **Mechanic**: Reversing state corruption using stack push/pop mechanics.
- **Gameplay**: Player records traversal steps and unravels recursion trees, confronting Stack Overflow phantoms.

### Chapter 4: Pipeline Hub (Queues & Buffers)
- **Mechanic**: Packet congestion routing in asynchronous I/O hubs.
- **Gameplay**: Priority queues and circular buffers regulate data traffic to prevent bus lockups.

### Chapter 5: Cache Citadel (Hash Tables)
- **Mechanic**: Direct hash key mapping and collision resolution (chaining vs. open addressing).
- **Gameplay**: Fast security credential verification against rogue security monitors.

### Chapter 6: Archive Spire (Sorting Algorithms)
- **Mechanic**: Reordering corrupt archives using Merge Sort ($O(n \log n)$) and Quick Sort.
- **Gameplay**: Time-attack sorting mini-games under real-time cycle deadlines.

### Chapter 7: Decision Canopy (Binary Trees & BST)
- **Mechanic**: Navigating hierarchical decision branches and balancing AVL trees.

### Chapter 8: Priority Forge (Heaps)
- **Mechanic**: Task scheduling based on emergency priority levels.

### Chapter 9: The Mesh (Graphs & Pathfinding)
- **Mechanic**: BFS / DFS / Dijkstra shortest-path navigation across degraded power grids.

### Chapter 10: The Runtime Core (Dynamic Programming & Finale)
- **Mechanic**: Multi-phase boss confrontation against The Architect using optimal substructure synthesis.

---

## 2. Technical Enhancements

- **Custom GLSL Post-Processing Shaders**: CRT scanline simulation, chromatic aberration during anomaly encounters, and neon bloom.
- **Asset Replacement Pipeline**: Seamlessly replacing procedural vector graphics with the vetted CC0 spritesheets documented in `docs/asset_ledger.md`.
- **Dialogue Voice Acting**: Full voice pack recording mapped directly to `VoiceManager` IDs.
