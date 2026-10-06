# RUNTIME: DSA Curriculum & Pedagogical Design

## 1. Pedagogical Philosophy

In **RUNTIME: The Last Algorithm**, educational mechanics are intertwined with game progression:
1. **Mechanic First**: The player encounters a gameplay bottleneck that cannot be solved by brute force.
2. **Concept Discovery**: Nova explains the underlying structural principle naturally through narrative and dialogue.
3. **Interactive Visualization**: The player visualizes the in-memory representation and inspects individual steps.
4. **Gameplay Mastery**: The player applies the concept to bypass firewalls, defeat anomalies, or solve environmental challenges.
5. **Formal Takeaway**: The game presents operations cost, Big-O classification, and trade-off summaries.

---

## 2. 10-Chapter Curriculum Roadmap

| Chapter | Sector Name | Core DSA Topic | Narrative Conflict | In-Game Ability / Weapon |
| :---: | :--- | :--- | :--- | :--- |
| **1** | **Memory District** | **Arrays & Linear Search** | Corrupted memory cells in contiguous buffer | **Array Core** (Direct indexing $O(1)$) |
| **2** | **Pointer Warrens** | **Linked Lists** | Memory fragmentation; nodes scattered in heap | **Link Anchor** (Dynamic splicing $O(1)$) |
| **3** | **Recursion Abyss** | **Stack & Call Stack** | Cascading memory collapse; system overflow | **Stack Shield** (LIFO reversal) |
| **4** | **Pipeline Hub** | **Queue & Deque** | Packet congestion in high-traffic bus | **Queue Conduit** (FIFO flow buffer) |
| **5** | **Cache Citadel** | **Hash Tables & Collisions** | Identity authentication deadlock | **Hash Decryptor** ($O(1)$ average lookup) |
| **6** | **Archive Spire** | **Sorting Algorithms** | Disordered library indexes causing timeouts | **Sort Pulse** (Merge / Quick sort) |
| **7** | **Decision Canopy** | **Trees & BST** | Hierarchical permission tree lockouts | **Tree Traversal Beam** |
| **8** | **Priority Forge** | **Binary Heaps & Priority Queues**| Overloaded event scheduler | **Heap Overclock** (Max/Min priority dispatch) |
| **9** | **The Mesh** | **Graphs & Pathfinding (BFS/DFS/Dijkstra)** | Network grid rerouting during total blackout | **Shortest Path Router** |
| **10** | **The Runtime Core** | **Dynamic Programming & Complexity Grand Finale** | The Architect's master optimization deadlock | **Optimal Substructure Synthesis** |

---

## 3. Chapter 1 Deep Dive: Memory District

### Core Concept: Contiguous Memory & Array Indexing
- **In-Memory Reality**: An array allocates sequential memory addresses:
  $$\text{Address}(A[i]) = \text{BaseAddress} + i \times \text{ElementSize}$$
- **Discovery**: Kael observes that knowing the target index allows immediate, instant retrieval in constant time $O(1)$.

### Algorithmic Challenge: Linear Search
- **The Problem**: When the memory buffer is unsorted and the index is unknown, every cell must be inspected sequentially from index $0$ to $n-1$.
- **Best Case**: $O(1)$ (Element at index 0)
- **Worst Case**: $O(n)$ (Element at last index or absent)
- **Average Case**: $O(n)$
- **Gameplay Lesson**: As $n$ grows, linear search becomes unviable against fast enemies, establishing the need for sorted arrays and binary search in later chapters.

---

## 4. Big-O Complexity Reference

| Data Structure / Algorithm | Access | Search | Insertion | Deletion | Space Complexity |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Array** | $O(1)$ | $O(n)$ | $O(n)$ | $O(n)$ | $O(n)$ |
| **Linked List** | $O(n)$ | $O(n)$ | $O(1)$ (at head) | $O(1)$ (at head) | $O(n)$ |
| **Stack** | $O(n)$ | $O(n)$ | $O(1)$ (push) | $O(1)$ (pop) | $O(n)$ |
| **Queue** | $O(n)$ | $O(n)$ | $O(1)$ (enqueue)| $O(1)$ (dequeue)| $O(n)$ |
| **Bubble Sort** | — | — | — | — | Worst: $O(n^2)$, Best: $O(n)$, Space: $O(1)$ |
| **Merge Sort** | — | — | — | — | Worst: $O(n \log n)$, Best: $O(n \log n)$, Space: $O(n)$ |
| **Quick Sort** | — | — | — | — | Worst: $O(n^2)$, Best: $O(n \log n)$, Space: $O(\log n)$ |
| **Linear Search**| — | $O(n)$ | — | — | Space: $O(1)$ |
| **Binary Search**| — | $O(\log n)$ | — | — | Space: $O(1)$ |
