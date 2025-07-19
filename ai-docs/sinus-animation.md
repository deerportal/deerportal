# Plan: Sinusoidal Wave Distortion for Title Text

**Date:** 2025-07-19
**Author:** Gemini

## 1. Objective

To create a dynamic, visually appealing effect for the main menu's "Deer Portal" title. The text, rendered with the game's primary font, will be distorted by a horizontal sine wave, making it appear as if it's gently waving. This will be achieved using a vertex shader for maximum performance.

## 2. The Core Concept: Vertex Shaders

Instead of manipulating pixels (a fragment shader), we will manipulate the geometry of the text itself. A vertex shader allows us to change the position of each vertex (corner) of the letters in the `sf::Text` object. By displacing the `y` coordinate of each vertex based on its `x` position and the current time, we can create a smooth, flowing wave.

## 3. Implementation Steps

### Step 1: Create the Vertex Shader File

A new file named `sinus-wave.vert` will be created in the `assets/shaders/` directory.

**File: `assets/shaders/sinus-wave.vert`**
```glsl
// Uniforms: Values passed from the C++ code to the shader
uniform float time;      // Time, to animate the wave
uniform float amplitude; // How high the waves are
uniform float frequency; // How many waves are on screen
uniform float speed;     // How fast the waves move

void main()
{
    // 1. Copy the original vertex position
    vec4 newPosition = gl_Vertex;

    // 2. Calculate the wave displacement
    // We use the original x-position of the vertex to determine its place in the sine wave.
    // We add (time * speed) to make the wave move horizontally.
    float wave = sin(newPosition.x * frequency + time * speed) * amplitude;

    // 3. Apply the displacement to the Y coordinate
    newPosition.y += wave;

    // 4. Set the final transformed position of the vertex
    // gl_ModelViewProjectionMatrix is a standard SFML variable that handles camera and view transformations.
    gl_Position = gl_ModelViewProjectionMatrix * newPosition;

    // 5. Pass along the original color and texture coordinates without modification
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}
```

### Step 2: Integrate into the Game Engine

The following changes will be made, likely within the `Game` class or a more focused `GameRenderer` or `Menu` class.

1.  **Declare the Shader and Clock:**
    Add a `sf::Shader` and a `sf::Clock` as member variables.

    ```cpp
    // In your class header (e.g., game.h)
    sf::Shader m_titleWaveShader;
    sf::Clock m_waveClock;
    ```

2.  **Load the Shader:**
    During the asset loading phase, load the vertex shader.

    ```cpp
    // In your asset loading function
    if (!m_titleWaveShader.loadFromFile(get_full_path("assets/shaders/sinus-wave.vert"), sf::Shader::Vertex)) {
        // Handle error: Log that the shader failed to load
        std::cerr << "Failed to load title wave shader!" << std::endl;
    }
    ```

3.  **Update Shader Uniforms in the Game Loop:**
    In the `update` method, continuously pass the elapsed time and other parameters to the shader.

    ```cpp
    // In your update(sf::Time deltaTime) method
    m_titleWaveShader.setUniform("time", m_waveClock.getElapsedTime().asSeconds());
    m_titleWaveShader.setUniform("amplitude", 10.f); // 10 pixels high
    m_titleWaveShader.setUniform("frequency", 0.02f);
    m_titleWaveShader.setUniform("speed", 3.f);
    ```

4.  **Apply the Shader During Rendering:**
    When drawing the menu title, pass the shader in the `sf::RenderStates` object.

    ```cpp
    // In your render() method, when drawing the menu
    // Assuming menuTxt is your sf::Text object for the title
    sf::RenderStates states;
    states.shader = &m_titleWaveShader;
    window.draw(*menuTxt, states);
    ```

## 4. Tuning the Effect

The visual appearance of the wave can be easily adjusted by changing the uniform values:

*   **`amplitude`:** Increase for higher waves, decrease for a more subtle effect.
*   **`frequency`:** Increase for more, tighter waves. Decrease for fewer, wider waves.
*   **`speed`:** Increase to make the waves travel faster across the text.

## 5. Cross-Platform Compatibility

This approach is highly portable. Vertex shaders are a fundamental part of OpenGL and are fully supported by SFML on all its target platforms (Windows, macOS, Linux, and even mobile). The GLSL shader code is simple and uses core features that will work everywhere without modification.
