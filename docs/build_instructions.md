# RUNTIME: Build Instructions & Setup Guide

## 1. Prerequisites & Toolchain

| Tool / Library | Minimum Required | Recommended |
| :--- | :--- | :--- |
| **Compiler** | C++17 compliant (GCC 9+, Clang 10+, MSVC 2019+) | GCC 13+ / Clang 16+ |
| **Build System** | CMake 3.20+ | CMake 3.25+ |
| **Git** | Git 2.20+ (for FetchContent dependencies) | Latest |
| **Graphics** | OpenGL 3.3 compatible GPU / drivers | Any modern GPU |

> [!NOTE]
> Raylib and nlohmann/json are managed automatically via **CMake FetchContent**. You do NOT need to install external libraries manually to build the project.

---

## 2. Quick Start (Linux / Arch Linux)

```bash
# 1. Navigate to the project root
cd /home/raj/Documents/oops_game

# 2. Configure the build directory
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# 3. Compile the game
cmake --build build -j$(nproc)

# 4. Launch the game
./build/runtime
```

---

## 3. Building and Running Automated Tests

To build with the full automated test suite enabled:

```bash
# Configure with tests enabled
cmake -S . -B build -DBUILD_TESTS=ON

# Compile everything including tests
cmake --build build -j$(nproc)

# Run DSA tests directly
./build/test_dsa

# Run Save System tests directly
./build/test_save

# Run full test suite via CTest
ctest --test-dir build --output-on-failure
```

---

## 4. Cross-Platform Instructions

### Windows (Visual Studio / MSVC)
```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.\build\Release\runtime.exe
```

### macOS (Clang)
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(sysctl -n hw.ncpu)
./build/runtime
```

---

## 5. Troubleshooting

- **Audio device failure**: If running on a headless or virtualized Linux environment without audio output, the game gracefully logs a warning and continues in subtitle-only mode.
- **Window scaling issues**: Adjust display resolution in `data/config.json` or through the in-game Settings menu.
