# Particles System Documentation

**Source Knowledge**: Commit 60a939ef8d8de62a00d82cdf3da244d8a17463ea

## Overview

The DeerPortal game implements a sophisticated particle system through the `GameAnimationSystem` class (`src/game-animation-system.h/cpp`). The system provides visual feedback for game events through particle bursts and animated effects.

## Core Architecture

### Particle Structure
The system uses `CircleParticle` structs with the following properties:
- `position`: Current particle position
- `velocity`: Movement vector
- `lifetime`: Time remaining for particle
- `totalLifetime`: Original lifetime for fade calculations
- `scale`: Size multiplier (default 0.5f)
- `textureRect`: Texture atlas coordinates
- `fadeOut`: Enable alpha fade over lifetime
- `gravity`: Downward acceleration

### Performance Optimizations

The system implements advanced optimization patterns:

#### Object Pooling
- `PARTICLE_POOL_SIZE`: 2048 particles pre-allocated
- `m_availableParticleIndices`: O(1) allocation queue
- `m_activeParticleIndices`: Active particle tracking

#### Trigonometry Lookup Tables
- `TRIG_TABLE_SIZE`: 360 entries (1-degree precision)
- `s_sinTable`/`s_cosTable`: Pre-calculated values
- `fastSin()`/`fastCos()`: O(1) trigonometry operations

#### Vertex Array Optimization
- Double buffering: `m_particleVertices` and `m_particleVerticesBack`
- Dirty flag system: `m_particlesDirty` prevents unnecessary rebuilds
- Throttled rebuilds: Maximum 120 rebuilds per second

#### Spatial Partitioning
- 8x8 grid for frustum culling
- `SpatialCell` structure for particle grouping
- View bounds culling for off-screen particles

## Particle Configurations

### Predefined Presets (ParticlePresets namespace)

#### DIAMOND_BURST
- Count: 120 particles (10x enhanced)
- Speed: 180.0f pixels/second
- Lifetime: 1.8 seconds
- Scale: 0.8f
- Pattern: Circular burst
- Texture: Diamond sprite from atlas

#### CARD_COLLECT  
- Count: 80 particles
- Speed: 140.0f pixels/second
- Lifetime: 1.4 seconds
- Scale: 0.6f
- Pattern: Circular burst
- Features: Scale-down effect

#### CARD_COLLECT_RANDOM
- Count: 80 particles
- Speed: 140.0f pixels/second
- Pattern: Explosion (random directions)
- Same properties as CARD_COLLECT

#### STOP_CARD
- Count: 160 particles
- Speed: 100.0f pixels/second
- Lifetime: 1.6 seconds
- Scale: 0.7f
- Gravity: 98.0f (falling effect)
- Pattern: Directional burst

## Burst Patterns

### BurstPattern Enum
- `CIRCLE`: Particles spread in uniform circle
- `EXPLOSION`: Random directional spread
- `DIRECTIONAL`: Particles follow specific trajectory

## Implementation Details

### Particle Creation
The `createCollectionBurst()` method accepts `ParticleConfig` structs and spawns particles at specified positions. The system supports:
- Custom textures through `customTexture` parameter
- Texture atlas regions via `textureRect`
- Per-effect configuration through `ParticleConfig`

### Rendering Pipeline
1. Update particle positions and properties
2. Apply spatial culling
3. Rebuild vertex array if marked dirty
4. Render all particles in single draw call

### Memory Management
- Pool-based allocation prevents runtime allocations
- Automatic cleanup of finished particles
- Vertex array reuse across frames

## Integration Points

The particle system integrates with:
- Card collection events
- Diamond collection mechanics
- Stop card effects
- Other game events requiring visual feedback

The system is called through `game->animationSystem->createCollectionBurst()` with appropriate configuration presets.