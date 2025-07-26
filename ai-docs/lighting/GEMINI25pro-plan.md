# Lighting Effects Implementation Plan for DeerPortal

**Author**: Gemini 2.5 Pro
**Date**: July 21, 2024

## 1. Overview

This document outlines a comprehensive plan to implement a dynamic 2D lighting effect in DeerPortal using SFML 3.0.1. The goal is to enhance the visual experience during the board initialization sequence.

The desired effect is as follows:
- When the `state_board_animation` begins, the screen is covered by a dark, semi-transparent overlay.
- As the 112 diamonds animate from the screen edges to their final positions on the board, each diamond will emit a soft, localized light.
- This will create a visually engaging effect of the diamonds "lighting up the darkness" as they assemble on the board.
- The implementation must be highly performant to avoid any frame rate drops during the animation.

## 2. Analysis of Existing Codebase

A thorough analysis of the existing codebase has identified the key components and considerations for this implementation:

-   **`BoardInitializationAnimator`**: This is the central class (`src/board-initialization-animator.h/cpp`) that controls the entire board setup animation. It manages the position, scale, and rotation of each diamond. It is highly optimized, using a single `sf::VertexArray` to render all diamonds in one draw call. **Any lighting solution must integrate cleanly with this class and maintain this high level of performance.**
-   **`GameStateManager`**: The game flow is controlled by a state machine (`src/game-state-manager.h/cpp`). The new lighting effect must be strictly confined to the `Game::state_board_animation` state.
-   **Game Render Loop**: The main loop in `game.cpp` will need to be adapted to handle the new lighting render pass.
-   **Asset Management**: A new asset for the light texture will need to be managed by the existing asset loading systems.

## 3. Proposed Solution: The Light Map Technique

To achieve the desired effect with maximum performance, this plan proposes the **Light Map** (also known as a light texture) technique. This is a standard and highly efficient method for 2D lighting.

### How It Works:

1.  **Create a Light Map**: A `sf::RenderTexture` is created, the same size as the game window. This texture will act as our canvas for drawing lights.
2.  **Darken the Scene**: In each frame of the animation, the light map is first cleared to a dark color (e.g., black or dark gray). This color will define the ambient light level of the scene.
3.  **Draw Lights**: For each animating diamond, a soft, glowing sprite (a pre-made texture of a radial gradient) is drawn onto the light map at the diamond's position. These lights are drawn using an **additive blend mode (`sf::BlendAdd`)**, which allows lights to overlap and brightly combine their intensity.
4.  **Combine with Scene**: The main game scene (the diamonds) is rendered normally to the main window. Afterwards, the completed light map is drawn on top of the scene using a **multiplicative blend mode (`sf::BlendMultiply`)**. This final step darkens the scene based on the light map, allowing the bright areas (where we drew our lights) to shine through.

### Why This Approach?

-   **Performance**: This method is extremely fast. It leverages the GPU for all blending operations and requires only one additional full-screen texture draw per frame. It avoids complex, per-pixel CPU calculations or expensive ray-casting geometry.
-   **Scalability**: It handles a large number of lights (112 in our case) with negligible performance impact, as all lights are simply blended sprites on a texture.
-   **Visual Quality**: It produces a soft, atmospheric lighting effect that perfectly matches the user's request.
-   **Simplicity**: It integrates well with SFML's existing render states and drawing capabilities.

## 4. Step-by-Step Implementation Plan

### Task 1: Create the Light Source Asset

-   **Action**: Create a new image file for the light's appearance.
-   **Details**: This should be a grayscale image of a soft-edged circle, white in the center and fading smoothly to black at the edges. A PNG with an alpha channel is ideal.
-   **Location**: Save the file as `assets/img/light_source.png`.

### Task 2: Create the `LightingManager` Class

-   **Action**: To encapsulate all lighting logic, create a new `LightingManager` class. This promotes modularity and reusability.
-   **Files**: `src/lighting-manager.h` and `src/lighting-manager.cpp`.
-   **`lighting-manager.h` Definition**:
    ```cpp
    #pragma once
    #include <SFML/Graphics.hpp>

    class LightingManager {
    public:
        LightingManager();
        void initialize(const sf::Vector2u& windowSize);
        void setLightTexture(const sf::Texture& texture);

        void beginFrame(const sf::Color& ambientColor);
        void addLight(sf::Vector2f position, float radius, const sf::Color& color);
        void render(sf::RenderTarget& target);

    private:
        sf::RenderTexture m_lightMap;
        sf::Sprite m_lightSprite;
        bool m_initialized = false;
    };
    ```
-   **`lighting-manager.cpp` Implementation**:
    -   `initialize`: Creates the `sf::RenderTexture`.
    -   `setLightTexture`: Assigns the loaded texture to the `m_lightSprite`.
    -   `beginFrame`: Clears the `m_lightMap` with the given ambient color.
    -   `addLight`: Sets the sprite's position, scale (based on radius), and color, then draws it to the `m_lightMap` with `sf::BlendAdd`.
    -   `render`: Draws the `m_lightMap`'s texture to the final target with `sf::BlendMultiply`.

### Task 3: Integrate `LightingManager` into the Game

-   **Action**: Add the `LightingManager` to the core game logic.
-   **Files to Modify**: `src/game.h`, `src/game.cpp`, `src/game-assets.h`, `src/game-assets.cpp`.
-   **Steps**:
    1.  In `game-assets.h`, add `sf::Texture lightSourceTexture;` and a method to load it in `game-assets.cpp`.
    2.  In `game.h`, add a `LightingManager m_lightingManager;` member.
    3.  In `game.cpp` (e.g., in the constructor or `init` method), call `m_lightingManager.initialize(window.getSize())` and `m_lightingManager.setLightTexture(assets.lightSourceTexture)`.

### Task 4: Connect `BoardInitializationAnimator` to the Lighting System

-   **Action**: The animator needs to tell the `LightingManager` where the diamonds are each frame.
-   **Files to Modify**: `src/board-initialization-animator.h`, `src/board-initialization-animator.cpp`.
-   **Steps**:
    1.  In `board-initialization-animator.h`, add a new public method declaration:
        ```cpp
        #include "lighting-manager.h" // Forward declaration might be better
        void updateLights(LightingManager& lightingManager) const;
        ```
    2.  In `board-initialization-animator.cpp`, implement this method. It should iterate through the `animatedItems` vector and call `lightingManager.addLight()` for each one, passing the item's current position.

### Task 5: Update the Main Render Loop

-   **Action**: Modify the main rendering logic to use the lighting system during the correct game state.
-   **File to Modify**: `src/game.cpp`.
-   **Logic**:
    ```cpp
    // Inside the main game loop's rendering section
    window.clear(sf::Color::Black);

    if (gameStateManager.getCurrentState() == Game::state_board_animation) {
        // 1. Prepare the light map
        m_lightingManager.beginFrame(sf::Color(20, 20, 30, 255)); // Dark blue ambient light

        // 2. Tell the animator to add its lights to the map
        boardAnimator.updateLights(m_lightingManager);

        // 3. Render the game scene (the diamonds) as normal
        boardAnimator.render(window, assets.diamondTexture);

        // 4. Apply the lighting effect by blending the light map
        m_lightingManager.render(window);

    } else {
        // ... existing rendering logic for other game states ...
        // For example:
        // window.draw(gameBoard);
        // window.draw(playerHud);
    }

    // Render UI elements on top of everything
    // ...

    window.display();
    ```

## 5. Conclusion

This plan provides a clear, efficient, and modular path to implementing the desired lighting effects. By encapsulating logic within a `LightingManager` and using the performant light map technique, we can create a stunning visual introduction to the game without compromising performance. The implementation respects the existing architecture and promotes clean, maintainable code. 