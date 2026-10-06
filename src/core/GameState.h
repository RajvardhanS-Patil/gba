#pragma once
// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// GameState — High-level game state enumeration
// ═══════════════════════════════════════════════════════════════════════════

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    DIALOGUE,
    COMBAT,
    PUZZLE,
    VISUALIZER,
    LOADING,
    CUTSCENE,
    DEMO,
    QUIT
};
