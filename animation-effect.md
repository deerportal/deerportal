# Detailed Plan: High-Performance Animation System

This document provides a detailed, step-by-step guide for implementing a high-performance, pluggable animation system for item collection effects, leveraging advanced optimization patterns.

## 1. In-Depth Codebase Analysis

*   **State Management:** The game's flow is controlled by `Game::currentState`. The key states for this implementation are `state_game` and `state_roll_dice`.
*   **Rendering Pipeline:** `GameRenderer` handles all drawing. It has a direct-to-window rendering path (`renderDirectToWindow`) for gameplay, which is highly efficient. The animation system should be integrated into this direct path to maintain performance.
*   **Item Collection Logic:**
    *   **Diamonds:** `CommandManager::processField` is the entry point for player-board interactions. Diamond collection is triggered from here. `BoardDiamond` objects are stored in `Game::boardDiamonds`.
    *   **Cards:** `CardsDeck` and `CommandManager` handle card drawing.
*   **Drawing Order:** The rendering order in `GameRenderer::renderStateGame` and `renderDirectToWindow` is crucial. Animations must be drawn after the game board and before the UI elements (`GroupHud`, `PlayerHud`).

## 2. Advanced Architecture & Optimization Patterns

To ensure the animation system is highly optimized and does not impact game performance, we will use the following patterns:

*   **Particle System with `sf::VertexArray`:** Instead of drawing individual sprites for each animation, we will treat the animations as particles. All active animation particles will be stored in a single `sf::VertexArray` and drawn with one draw call per texture. This is the most significant optimization.
*   **Object Pooling:** We will pre-allocate a pool of `AnimationParticle` objects. When an animation is triggered, we will activate a particle from the pool. When the animation finishes, the particle is returned to the pool for reuse. This completely avoids dynamic memory allocation (`new`/`delete`) during gameplay, preventing frame stutters.

### 2.1. Revised Core Components

*   **`AnimationParticle` (Struct):** A lightweight struct to hold the state of a single animation particle (position, velocity, color, lifetime, etc.).
*   **`AnimationPool` (Class):** An object pool for `AnimationParticle` objects.
*   **`AnimationSystem` (New Class):** A central manager that owns the `AnimationPool` and the `sf::VertexArray`. It will be responsible for updating particle states, rebuilding the vertex array each frame, and drawing it.

## 3. Revised Step-by-Step Implementation Plan

### Phase 1: Core System Implementation

#### Step 1.1: Create `AnimationParticle` Struct

**File:** `src/animation-particle.h`
```cpp
#ifndef ANIMATION_PARTICLE_H
#define ANIMATION_PARTICLE_H

#include <SFML/Graphics.hpp>

struct AnimationParticle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Color color;
    sf::Time lifetime;
    sf::Time totalLifetime;
    bool active = false;
};

#endif // ANIMATION_PARTICLE_H
```

#### Step 1.2: Create `AnimationSystem`

This class will manage the particles and the vertex array.

**File:** `src/animation-system.h`
```cpp
#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "animation-particle.h"

class AnimationSystem : public sf::Drawable {
public:
    AnimationSystem(unsigned int maxParticles, sf::Texture& texture);

    void addEffect(sf::IntRect textureRect, sf::Vector2f startPos, sf::Vector2f endPos, sf::Time duration);
    void update(sf::Time dt);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<AnimationParticle> m_pool;
    sf::VertexArray m_vertices;
    sf::Texture& m_texture;
    std::size_t m_firstAvailable;
};

#endif // ANIMATION_SYSTEM_H
```

**File:** `src/animation-system.cpp`
```cpp
#include "animation-system.h"

AnimationSystem::AnimationSystem(unsigned int maxParticles, sf::Texture& texture)
: m_pool(maxParticles), m_vertices(sf::Quads, 0), m_texture(texture), m_firstAvailable(0) {}

void AnimationSystem::addEffect(sf::IntRect textureRect, sf::Vector2f startPos, sf::Vector2f endPos, sf::Time duration) {
    // Find an available particle
    std::size_t particleIdx = m_firstAvailable;
    if (particleIdx >= m_pool.size()) return; // Pool is full

    AnimationParticle& p = m_pool[particleIdx];
    p.active = true;
    p.pos = startPos;
    p.vel = (endPos - startPos) / duration.asSeconds();
    p.lifetime = duration;
    p.totalLifetime = duration;
    p.color = sf::Color::White;

    // Update the first available index
    m_firstAvailable++;
}

void AnimationSystem::update(sf::Time dt) {
    m_vertices.clear();

    for (std::size_t i = 0; i < m_pool.size(); ++i) {
        AnimationParticle& p = m_pool[i];
        if (!p.active) continue;

        p.lifetime -= dt;
        if (p.lifetime <= sf::Time::Zero) {
            p.active = false;
            // This particle is now available, move it to the front of the available list
            std::swap(m_pool[i], m_pool[m_firstAvailable - 1]);
            m_firstAvailable--;
            continue;
        }

        p.pos += p.vel * dt.asSeconds();

        float ratio = p.lifetime.asSeconds() / p.totalLifetime.asSeconds();
        p.color.a = static_cast<sf::Uint8>(255 * ratio);

        // Add to vertex array
        // (Implementation will be to add 4 vertices for a quad)
    }
}

void AnimationSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &m_texture;
    target.draw(m_vertices, states);
}
```

### Phase 2: Integration

#### Step 2.1: Update `CMakeLists.txt`

Add `src/animation-system.cpp` to the list of source files in `CMakeLists.txt`.

#### Step 2.2: Integrate `AnimationSystem` into `Game`

**File:** `src/game.h`
1.  Include `animation-system.h`.
2.  Add a private member: `std::unique_ptr<AnimationSystem> m_diamondAnimationSystem;`

**File:** `src/game.cpp`
1.  In the `Game` constructor, after textures are loaded, initialize the animation system:
    `m_diamondAnimationSystem = std::make_unique<AnimationSystem>(100, textures.textureBoardDiamond);`
2.  In `Game::update`, call `m_diamondAnimationSystem->update(frameTime);`.

#### Step 2.3: Integrate into `GameRenderer`

**File:** `src/game-renderer.cpp`
In `renderDirectToWindow`, after drawing the board and before drawing the UI, add:
`game->window.draw(*game->getDiamondAnimationSystem());`

#### Step 2.4: Trigger Animation

**File:** `src/command.cpp` (or wherever `processField` is)
When a diamond is collected:
1.  Get the diamond's texture rect, start position, and the player's HUD position.
2.  Call `game.getDiamondAnimationSystem()->addEffect(...)`.
3.  Immediately hide the original `BoardDiamond` sprite.

## 4. Knowledge Base & References (SFML 3 Compatible)

*   **SFML 3 Vertex Array Examples:**
    *   [Simple SFML 3 VertexArray Example (Triangle)](https://www.sfml-dev.org/documentation/3.0/classsf_1_1VertexArray.php)
    *   [Terminal Root: SFML 3 Vertex Array Primitives](https://terminalroot.com/how-to-create-geometric-primitives-with-sfml-and-c/)

*   **Object Pooling:**
    *   [Game Programming Patterns: Object Pool](http://gameprogrammingpatterns.com/object-pool.html)
    *   [Refactoring Game Development: Object Pools](https://refactoring.guru/design-patterns/object-pool)

*   **SFML 3 Particle System Design:**
    *   [SFML-ParticleSystem Library (Data-Oriented)](https://github.com/MarioLiebisch/SFML-ParticleSystem)
    *   [Terminal Root: Simple Particle Animation in SFML](https://terminalroot.com/a-simple-particle-animation-with-c-and-sfml/)