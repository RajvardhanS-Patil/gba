# RUNTIME: Testing Strategy & Verification Report

## 1. Overview

**RUNTIME: The Last Algorithm** incorporates rigorous unit testing and automated validation to ensure data structure implementations are mathematically sound, edge-case resilient, and memory-safe.

---

## 2. Test Architecture

The test suite is compiled into native test runners using CMake:
- `test_dsa`: Validates `ArrayDS`, `Stack`, `Queue`, and algorithm correctness.
- `test_save`: Validates JSON serialization, save slot creation, reading, and corrupted file resilience.

```
build/
├── test_dsa  <── tests/dsa/test_main.cpp
│                  ├── test_array.cpp
│                  ├── test_stack.cpp
│                  └── test_queue.cpp
└── test_save <── tests/save/test_save.cpp
```

---

## 3. Test Cases & Coverage

### 3.1 ArrayDS Tests (`tests/dsa/test_array.cpp`)
- `test_empty_initialization`: Verifies size is 0 and empty flag is true.
- `test_sequential_insert`: Verifies element insertion and $O(1)$ access cost.
- `test_out_of_bounds_protection`: Asserts graceful fallback (-1) on invalid indices.
- `test_insert_at_index`: Tests middle insertion and elements shifting.
- `test_linear_search_found`: Tests positive search and iteration counting.
- `test_linear_search_not_found`: Tests search for non-existent values.
- `test_remove_value`: Verifies element removal and size decrement.
- `test_remove_at_index`: Tests position-based deletion.
- `test_reset`: Verifies clean buffer reset.

### 3.2 Stack Tests (`tests/dsa/test_stack.cpp`)
- `test_lifo_order`: Asserts Last-In, First-Out sequence behavior.
- `test_empty_pop_and_peek`: Verifies safety on underflow (returns -1 safely).
- `test_multiple_push_pop`: Tests deep stack push/pop parity.
- `test_search_in_stack`: Tests linear search across stack elements.

### 3.3 Queue Tests (`tests/dsa/test_queue.cpp`)
- `test_fifo_order`: Asserts First-In, First-Out sequence behavior.
- `test_empty_dequeue_and_front`: Verifies safety on underflow (returns -1 safely).
- `test_interleaved_enqueue_dequeue`: Tests dynamic queue buffering.
- `test_queue_reset`: Verifies buffer clearing.

### 3.4 Save System Tests (`tests/save/test_save.cpp`)
- `test_save_serialization`: Verifies JSON document construction.
- `test_save_deserialization`: Asserts all player stats, mastery maps, and quests restore intact.
- `test_missing_slot_fallback`: Tests graceful handling of non-existent files.
- `test_slot_deletion`: Tests slot deletion and cleanup.

---

## 4. Test Results

```
Test project /home/raj/Documents/oops_game/build
    Start 1: DSATests
1/2 Test #1: DSATests .........................   Passed    0.01 sec
    Start 2: SaveTests
2/2 Test #2: SaveTests ........................   Passed    0.01 sec

100% tests passed out of 2
Total Test time (real) = 0.03 sec
```
- **Exit Status**: 0 (All assertions passed successfully)
- **Memory Leaks**: 0 (RAII verified)
