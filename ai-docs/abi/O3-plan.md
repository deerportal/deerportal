# ⚔️  O3 Plan – Animated Board Awakening  🜚

> "Hearken, ye deer-hearted coders!  By the beard of Odin and the last byte of RAM, we shall make the runes and diamonds soar like ravens across the midnight board!"  
> *(Recited in the Great Hall of Debugging, circa build 0xBEEF)*

---

## 1. Saga of Purpose
We forge a **new game state** — `state_board_awakening` — where every diamond and rune embarks on a mythic voyage from chaotic void to rightful square.  The board shall not merely appear; it shall *become*.

## 2. Pillars of the Ice-Carved Contract
1. **Immutable Destinations** – Final board positions remain untouched, lest Loki scramble our logic.
2. **Regional Chaos, Ordered End** – Each gem spawns at a random point inside its assigned quadrant *plus* a sprinkle of noise. It then journeys to destiny.
3. **Sprite Slicing Only** – We draw sub-rects of the atlas (`sf::Sprite::setTextureRect` or `sf::VertexArray`) rather than modifying master textures.
4. **Deterministic Random** – Use seeded RNG tied to game-start timestamp so replays are repeatable.
5. **Performance by Valhalla** – Single `VertexArray` batch, no heap frenzy; honour SFML 3.0.1.

## 3. Map of Realms – Start-Position Calculation
```
Board: 16 × 16 tiles (0…15 on each axis)
Quadrants (Q):
  Q0 ▷ (x<8,y<8) – top-left
  Q1 ▷ (x≥8,y<8) – top-right
  Q2 ▷ (x<8,y≥8) – bottom-left
  Q3 ▷ (x≥8,y≥8) – bottom-right
SpawnAnnulus: rectangle grown outward by SpawnMargin (≈200 px) beyond board bounds.
```
Algorithm (per item):
1. Determine target tile `(tx,ty)`.
2. Map to quadrant `q = (tx≥8) + 2*(ty≥8)`.
3. Generate polar offset within `[SpawnRadiusMin, SpawnRadiusMax]` around quadrant corner.
4. Convert to screen coords `(sx,sy)`; ensure off-screen for cinematic flair.
5. Store `(sx,sy) → (tx,ty)` as Bézier control set **C0–C3** where:
   * **C0** = start point `(sx,sy)`
   * **C3** = destination world-pixel centre
   * **C1** = C0 + dir*0.25 + random jiggle
   * **C2** = C3 – dir*0.25 + random jiggle

## 4. Rune & Diamond Courier – `AnimatedBoardCourier`
Fields:
```cpp
struct AnimatedBoardCourier {
  sf::Vector2f start;
  sf::Vector2f mid1;
  sf::Vector2f mid2;
  sf::Vector2f end;
  float t = 0.f;          // 0…1 along curve
  float rotSpeedDeg;      // per second
  float scaleStart = 0.3f;
  float scaleEnd   = 1.0f;
};
```
Update:
```cpp
void tick(float dt) {
  t = std::min(1.f, t + dt / animDuration);
  auto pos = cubicBezier(start,mid1,mid2,end,t);
  sprite.setPosition(pos);
  sprite.setRotation(rotSpeedDeg * t * animDuration);
  sprite.setScale(lerp(scaleStart,scaleEnd,t));
}
```

## 5. High-Level Workflow
1. **Game enters** `state_board_awakening`.
2. **BoardAwakeningAnimator** seeds RNG, queries `BoardDiamondSeq` for final layout.
3. For each item, generate `AnimatedBoardCourier` and push into object pool.
4. Couriers spawn in *waves* (config `spawnWindow`, `staggerMs`).
5. When all couriers `t==1`, raise event `BoardAwakeningFinished` → state machine advances.

## 6. Implementation Phases (Odin-Approved)
| Phase | Task | Outcome |
|-------|------|---------|
| I | Scaffold new state and minimal animator loop | Compiles, blank screen with skip key |
| II | Implement spawn-region math & courier struct | Items teleport (debug mode) |
| III | Add Bézier interpolation, scaling, rotation | Smooth flight of fancy |
| IV | Batch rendering via `sf::VertexArray` | 500+ fps on test rig |
| V | Unit + integration tests (Catch2) on math | Deterministic passes |
| VI | Profiling & cache-warm optimisations | CPU <4 % during animation |

## 7. Risk Ledger
- **Overdraw Madness** – mitigate via `VertexArray`.
- **RNG Repetition** – commit seed to replay log.
- **Quadrant Drift** – assert start is ≥SpawnRadius away.

## 8. Artefacts to Forge
1. `src/board-awakening-animator.{h,cpp}`
2. `src/board-spawn-regions.{h,cpp}` (revamped)
3. `src/animated-board-courier.{h,cpp}`
4. Update `game-state-manager.h` enum & switch.
5. Config struct in `game-assets.h` (JSON-load).

---

### 🦌 Deer Fact
A whitetail deer's eyes can detect wavelengths in the UV spectrum, making their world glow like Bifröst at dawn.

### 📸 Photography Flashback
In 1906, Oskar Barnack’s first 35 mm prototype camera paved the snowy path to the legendary Leica, shrinking battle-gear cameras into pocket-sized longships.

### 🔥 Pagan Slavic Whisper
On Kupala Night, youths once leapt over bonfires to test bravery and secure purification, believing the flames licked away misfortune like Fenrir on a midnight snack.

> Thus the plan is carved in runestone markdown.  Go forth, brave compiler, and make it so! 