# RUNTIME: Gameplay Guide & Player Manual

## 1. Control Scheme

| Key | Context | Action |
| :--- | :--- | :--- |
| **W, A, S, D** | Exploration | Move protagonist Kael across room |
| **E** | Exploration | Interact with Memory Nodes / NPCs / Terminals |
| **SPACE / ENTER** | Dialogue | Advance dialogue / Skip typewriter text |
| **UP / DOWN** | Dialogue | Select dialogue choices |
| **1 / ENTER** | Combat | Select Array Scan attack during player turn |
| **TAB** | In-Game | Open / Close Polymorphic DSA Visualizer |
| **F1 / V** | In-Game | Toggle Architecture Inspector (Viva Mode) |
| **ESC** | In-Game | Open Pause Menu (Resume, Settings, Save, Quit) |
| **P / N** | Demo Mode | Cycle Previous / Next feature demonstration |
| **SPACE** | Demo Mode | Execute live operation on active data structure |

---

## 2. Chapter 1: Memory District Walkthrough

### Act I: The Awakening
1. Kael awakens in damaged Sector 7-Alpha following a runtime glitch.
2. Nova introduces herself and explains the system architecture: data is stored in contiguous sequences called **Arrays**.

### Act II: The Investigation
1. Move Kael east toward the central Memory Bank.
2. Eight memory nodes are aligned sequentially representing indices `0` through `7`.
3. Inspect each node with **[E]** to perform a real **Linear Search**.
4. Node `4` contains corrupted address `999`. Interacting with it repairs the corrupted sector and advances the quest.

### Act III: The Anomaly Encounter
1. A **Memory Eater** entity detects the memory stabilization and attacks.
2. Enter the combat arena. The Memory Eater is vulnerable to index-targeted disruption.
3. Use **Array Scan** ([1]) to exploit the anomaly's memory vulnerability and neutralize it.

### Act IV: Sector Recovery
1. Kael acquires the **Array Core**, granting direct indexed access capability.
2. District stabilizes; dialogue concludes and progress is saved to slot 1.

---

## 3. Interactive DSA Visualizer ([TAB])

Pressing **[TAB]** opens the in-game DSA Visualizer overlay:
- View live element values, index counters, and current operations count.
- Click **INSERT**, **REMOVE**, **SEARCH**, or **RESET** to see elements animate in real time.
- View real Big-O complexity ratings: `Access: O(1) | Search: O(n) | Insert: O(n) | Delete: O(n)`.

---

## 4. Presentation & Viva Demo Mode

From the Main Menu, click **DEMO MODE (VIVA)** to open a guided 10-step presentation tool:
- **1/10**: Narrative Arc & Setting
- **2/10**: Character Relationship & Dialogue
- **3/10**: Array Data Structure & Linear Search
- **4/10**: Stack LIFO Push / Pop Simulation
- **5/10**: Queue FIFO Enqueue / Dequeue Simulation
- **6/10**: Bubble Sort Step-by-Step Visualization
- **7/10**: Big-O Complexity Comparisons Bar Chart
- **8/10**: C++ Object-Oriented Architecture Blueprint
- **9/10**: Persistent JSON Save / Load Demonstration
- **10/10**: Final Boss Architect Concept Breakdown
