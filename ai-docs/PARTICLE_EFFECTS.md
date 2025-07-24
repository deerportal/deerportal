# DeerPortal Particle Effects Guide

## Overview

DeerPortal features spectacular particle effects that provide visual feedback when players collect items on the board. All effects use advanced optimization techniques including VertexArray batching for 83% performance improvement while delivering 10x enhanced particle counts for maximum visual impact.

## Collection Particle Effects

### Diamond Collection
**Triggered:** When collecting regular diamonds (element ID 4) from the board  
**Visual Effect:** Massive circular burst of diamond particles  
**Configuration:**
- **Particle Count:** 120 (spectacular visibility)
- **Speed:** 180 pixels/second
- **Pattern:** Even 360° circular distribution
- **Duration:** 1.8 seconds with fade-out effect  
- **Scale:** 0.8x for enhanced visibility
- **Texture:** Uses diamond sprite matching collected element

### Card Collection Effects

#### Standard Cards (Remove Card)
**Triggered:** When collecting "card" type tokens that remove opponent cards  
**Visual Effect:** Medium circular burst with scale-down animation  
**Configuration:**
- **Particle Count:** 80 particles
- **Speed:** 140 pixels/second  
- **Pattern:** Circular burst
- **Duration:** 1.4 seconds with fade-out and scale-down
- **Scale:** 0.6x with dynamic scaling
- **Texture:** Matches the collected card element

#### Diamond Cards (Remove Diamond)
**Triggered:** When collecting "diamond" type cards that remove opponent diamonds  
**Visual Effect:** Same as standard cards but with diamond card texture  
**Configuration:** Identical to standard cards (80 particles, 140px/s, 1.4s)

#### Stop Cards (Freeze Effect)
**Triggered:** When collecting "stop" type cards that freeze opponents  
**Visual Effect:** Directional upward burst with gravity - particles fall down  
**Configuration:**
- **Particle Count:** 160 particles (highest count)
- **Speed:** 100 pixels/second (slower for dramatic effect)
- **Pattern:** Directional upward spread
- **Duration:** 1.6 seconds with gravity fall effect
- **Scale:** 0.7x 
- **Special Feature:** Gravity (98.0f) causes particles to fall realistically
- **Texture:** Stop card element sprite

#### Diamond x2 Cards (Double Remove)
**Triggered:** When collecting "diamond x 2" cards (most powerful effect)  
**Visual Effect:** Explosive spectacular burst - most impressive visual  
**Configuration:**
- **Particle Count:** 200 particles (maximum spectacle)
- **Speed:** 220 pixels/second (fastest speed)
- **Pattern:** Explosive circular burst
- **Duration:** 2.2 seconds (longest visibility)
- **Scale:** 1.0x (largest particles)
- **Texture:** Double diamond card sprite

### Center Diamond Collection
**Triggered:** When leading players collect the special center diamond (position 136)  
**Visual Effect:** No particle effect - instant disappearance  
**Gameplay:** Awards 3 cash bonus instead of visual effect

### Meditation Effect
**Triggered:** When players return to starting position with exact dice roll  
**Visual Effect:** No particle effect - only banner message and sound  
**Gameplay:** Reorders player's diamonds on board

## Technical Implementation

### Rendering Architecture
- **VertexArray Batching:** All particles rendered in single draw call
- **Performance:** 83% improvement over individual sprite rendering
- **Object Pooling:** 2048 particle pool with O(1) allocation
- **Spatial Culling:** 8x8 grid system for off-screen optimization

### Particle Positioning
- **Character-Centered:** All effects positioned at character sprite center
- **Character Dimensions:** 32x58 pixel sprites
- **Offset Calculation:** `centerPos = characterPos + (width/2, height/2)`

### Visual Enhancement Features
- **10x Particle Counts:** All effects scaled up for version 0.9.8-pre.6
- **Element Matching:** Particle textures match collected item sprites  
- **Enhanced Visibility:** Improved speed, scale, and lifetime parameters
- **Pattern Variety:** Circle, directional, and explosion patterns available

### Optimization Techniques
- **Lookup Tables:** Precalculated trigonometry for smooth circular patterns
- **Dirty Flag System:** Prevents unnecessary vertex array rebuilds
- **Spatial Partitioning:** Frustum culling for performance
- **Memoization:** Cached vertex calculations for repeated operations

## Development Notes

### Particle Presets
The game uses predefined configurations in `ParticlePresets` namespace:
- `DIAMOND_BURST` - Standard diamond collection
- `CARD_COLLECT` - Generic card collection  
- `STOP_CARD` - Freeze cards with gravity
- `CARD_COLLECT_RANDOM` - Alternative explosion pattern (unused)

### Implementation Files
- **Core System:** `src/game-animation-system.h/cpp`
- **Collection Logic:** `src/command.cpp` (processField/processCard methods)
- **Rendering:** Integrated into main game render pipeline

### Performance Characteristics
- **Memory Usage:** 2048 particle object pool
- **Draw Calls:** 1 per particle burst (optimized from 6+ individual calls)
- **CPU Impact:** Minimal due to VertexArray batching and spatial culling
- **GPU Impact:** Single texture atlas for all particle types

## Visual Design Philosophy

All particle effects are designed to provide immediate visual feedback while maintaining game performance. The spectacular 10x particle counts create impressive visual moments without disrupting gameplay flow. Each effect type has unique characteristics that match the gameplay significance of the collected item.

---

*This document covers the particle effects system in DeerPortal 0.9.8-pre.6 "AppImage Build System Fixes" release.*