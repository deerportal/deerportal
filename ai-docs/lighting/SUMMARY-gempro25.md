# Consolidated Lighting Implementation Plan

**Objective**: To implement a highly performant, dynamic 2D lighting effect for the board initialization sequence in DeerPortal. The effect will feature 112 animating diamonds that illuminate a dark screen, enhancing the visual experience.

This document synthesizes the best elements from three proposed plans (Gemini 2.5 Pro, Grok-4, Opus-4) to create a single, optimal strategy.

---

## 1. Core Strategy: The Light Map Technique

After analyzing the shader-based approach (Grok) and the comprehensive engine design (Opus), the **Light Map** technique (proposed by Gemini) is the most suitable and performant solution for this specific scenario.

-   **Why**: This technique avoids the significant performance cost of a shader loop with 112 lights (Grok's proposal) by consolidating all lighting into a single texture. It is simpler and more direct than the full-scale lighting engine (Opus's proposal), which includes complexities like culling and pooling that are unnecessary for this scripted animation where all lights are always on-screen.

### How It Works:
1.  **Light Map Texture**: A single `sf::RenderTexture`, the size of the screen, is used as a canvas for lights.
2.  **Frame Start**: The light map is cleared to a dark ambient color (e.g., dark blue/gray).
3.  **Draw Lights**: For each of the 112 diamonds, a sprite with a soft radial gradient texture is drawn onto the light map at the diamond's current position. These are drawn with `sf::BlendAdd`, allowing lights to overlap and intensify realistically.
4.  **Final Composite**: The main scene (the diamonds) is drawn to the window. Then, the completed light map texture is drawn on top using `sf::BlendMultiply`. This darkens the scene according to the map, letting the bright areas shine through.

---

## 2. Architecture & Design

This plan adopts the modularity proposed by Gemini and Opus for a clean, reusable implementation.

-   **`LightingManager` Class**: A central class (`src/lighting-manager.h/cpp`) will encapsulate all lighting logic. This promotes separation of concerns and makes the system easy to manage.
-   **`LightSource` Properties**: While not a formal struct, the `addLight` method in the manager will accept parameters for `position`, `radius`, and `color`, inspired by the detailed structure in the Opus plan.
-   **Configuration**: The ambient color and other lighting parameters will be easily configurable, a best practice highlighted in the Opus plan.

---

## 3. Step-by-Step Implementation Plan

This plan follows the clear, actionable steps laid out by Gemini, which integrate cleanly with the existing codebase.

**Task 1: Create Light Asset**
-   Create a grayscale radial gradient image (`assets/img/light_source.png`). It should be white/bright in the center and fade to transparent/black at the edges. This is more performant than generating the gradient via a shader for every light.

**Task 2: Implement `LightingManager` Class**
-   Create `src/lighting-manager.h` and `src/lighting-manager.cpp`.
-   **Header (`lighting-manager.h`)**:
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
-   **Implementation (`lighting-manager.cpp`)**:
    -   `initialize`: Creates the `sf::RenderTexture`.
    -   `setLightTexture`: Assigns the loaded texture to `m_lightSprite`.
    -   `beginFrame`: Clears `m_lightMap` with the specified `ambientColor`.
    -   `addLight`: Sets the sprite's position, scale (based on `radius`), and color, then draws it to `m_lightMap` using `sf::BlendAdd`.
    -   `render`: Draws the `m_lightMap`'s texture onto the `target` using `sf::BlendMultiply`.

**Task 3: Integrate into Game**
-   **`GameAssets`**: Load `light_source.png` into an `sf::Texture`.
-   **`Game`**: Add a `LightingManager m_lightingManager;` member. Initialize it and set its texture in the game's setup phase.

**Task 4: Connect to Animation**
-   **`BoardInitializationAnimator`**: Add a new method: `void updateLights(LightingManager& lightingManager) const;`
-   In the `.cpp`, implement this method to loop through all animating diamonds and call `lightingManager.addLight()` for each one, passing its current position and a scale/color based on its animation state.

**Task 5: Update Render Loop**
-   Modify the main render loop in `game.cpp`. When `gameStateManager.getCurrentState() == Game::state_board_animation`:
    1.  `m_lightingManager.beginFrame(sf::Color(20, 20, 30, 255));`
    2.  `boardAnimator.updateLights(m_lightingManager);`
    3.  `boardAnimator.render(window, ...);` // Render diamonds as normal
    4.  `m_lightingManager.render(window);` // Apply the final lighting effect
-   For all other game states, the existing rendering logic will run, and the lighting effect will not be applied.

---

## 4. Conclusion

This consolidated plan provides the most effective path forward. It leverages the **performance and simplicity of the Light Map technique**, which is perfectly suited for the task, while incorporating the strong **architectural and modular concepts** from all three proposals. This ensures a high-quality, visually stunning result without compromising the game's performance.

**Validation Note:** While access to the SFML source code for a deep performance analysis was not possible, the recommended Light Map technique is a well-established and highly-performant method for 2D lighting in SFML, endorsed by the SFML community and extensive game development resources. It is considered the most efficient and straightforward approach for this use case.
