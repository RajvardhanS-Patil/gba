# RUNTIME: GBA Asset & Audio Research Ledger

## 1. Design & Platform Philosophy
- **Logical Display Target:** 240 × 160 pixels (Authentic Game Boy Advance resolution).
- **Presentation:** Rendered to a 240×160 `RenderTexture2D` with nearest-neighbor texture filtering (`TEXTURE_FILTER_POINT`), integer upscaled to window sizes (960×640 at 4x scale or 1200×800 at 5x scale).
- **Grid / Tile Standard:** 16 × 16 px tiles.
- **Palette Aesthetic:** Warm GBA handheld colors (nostalgic earthen greens, stone slates, warm wood creams, golden yellows, crimson accents). No modern neon bloom.

---

## 2. Visual Candidates Evaluation

| Asset ID | Category | Source / Creator | License | Dimensions | Style / Evaluation | Compatibility Score |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **T-GBA-01** | Overworld Tiles | Kenney (Roguelike/RPG 1-Bit & Micro RPG) | CC0 1.0 Universal | 16×16 | Clean 16x16 tileable nature, paths, houses, trees | 9.5 / 10 |
| **T-GBA-02** | Town & Indoors | OpenGameArt (Puny World / Buch) | CC0 1.0 Universal | 16×16 | Authentic 16x16 JRPG town & interior furniture | 9.0 / 10 |
| **C-GBA-01** | Characters | Custom Procedural Pixel Generator / CC0 OpenGameArt | CC0 1.0 Universal | 16×20 / 16×24 | Handheld 4-directional walking sprites (Kael, Nova, NPCs, Vex) | 10 / 10 |
| **M-GBA-01** | Algorithmic Creatures | Original Pixel Art Specifications (Arrix, Linko, Stackra, Queura, Glitch Fiends) | CC0 / Dedicated | 32×32 / 40×40 | Expressive front/back battle combatant sprites | 10 / 10 |
| **UI-GBA-01** | Handheld UI & Dialogue | Classic 9-Slice Handheld RPG Frame | CC0 1.0 Universal | 240×160 native | Crisp high-contrast 1px border dialogue boxes, HP gauges, arrow cursors | 10 / 10 |
| **F-GBA-01** | Handheld Pixel Font | Built-in Raylib pixel font & 8px Monospace bitmap | Permissive / SIL OFL | 8×8 / 5×7 | Flawlessly legible at 240×160 without anti-aliasing blur | 10 / 10 |

---

## 3. Audio & Chiptune Candidates Evaluation

| Sound ID | Type | Source & Technique | License | Role / Context | Score |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **BGM-TOWN** | Chiptune Music | Procedural Chiptune Pulse/Triangle Synthesizer / CC0 OpenGameArt | CC0 | Nova Town Peaceful Overworld Theme | 9.5 / 10 |
| **BGM-ROUTE** | Chiptune Music | Melodic square wave looping arrangement | CC0 | Memory Route Exploration | 9.5 / 10 |
| **BGM-BATTLE**| Chiptune Music | Fast-paced bass arpeggio + drum pulse | CC0 | Turn-based Creature Battle | 10 / 10 |
| **BGM-VICTORY**| Chiptune Fanfare| 4-bar uplifting brass chiptune fanfare | CC0 | Battle Win & Mastery Acquired | 10 / 10 |
| **SFX-CURSOR** | Sound Effect | High-pitch pulse blip (1200Hz, 30ms) | CC0 | Menu navigation & selection | 10 / 10 |
| **SFX-CONFIRM**| Sound Effect | Two-tone rising chime (600Hz -> 1000Hz) | CC0 | Interaction / Confirm | 10 / 10 |
| **SFX-BUMP**   | Sound Effect | Low thud (80Hz, 50ms) | CC0 | Walking into wall/obstacle | 9.5 / 10 |
| **SFX-TEXT**   | Sound Effect | Soft typewriter chirp (800Hz, 15ms) | CC0 | Dialogue character spooling | 10 / 10 |
| **SFX-ENCOUNTER**| Sound Effect | Glissando sweep & flash effect | CC0 | Battle transition trigger | 10 / 10 |
