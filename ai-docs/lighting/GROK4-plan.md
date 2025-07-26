
# Lighting Effects Implementation Plan for Board Initialization Animation

## 1. Current Implementation Analysis

### Board Initialization Animation
- Handled by `BoardInitializationAnimator` in `src/board-initialization-animator.cpp/h`.
- Animates 112 diamonds from spawn points to target positions using Bezier curves.
- Uses sf::VertexArray for efficient batch rendering of all diamonds in a single draw call.
- Each diamond has scaling (from 0.3 to 1.0), rotation, and staggered timing.
- Rendered via `render(sf::RenderTarget& target, const sf::Texture& texture)` which draws the vertex array.
- Integrated into game states via `state_board_animation`.
- Animation duration ~2.5s per diamond with 0.05s stagger.

### Rendering Pipeline
- `GameRenderer` in `src/game-renderer.cpp/h` handles rendering.
- Uses direct rendering to window for performance in gameplay states.
- For animation state: `renderStateBoardAnimation()` likely draws background and then the animated diamonds.
- Existing shaders: blur.frag, dark.frag, pixelate.frag.
  - dark.frag: Darkens image based on luminance (clamps luminance -0.5, multiplies by 2).
- RenderTexture used for some states, but optimized to direct rendering where possible.

### Relevant Documentation (ai-docs/final-docs/)
- **animation-diamonds-before-game.md**: Details animation mechanics, Bezier paths, vertex array optimization. No existing lighting.
- **game_states.md**: Describes `state_board_animation` for the initialization sequence.
- **sprites.md**: Documents diamond sprites (44x44 from atlas), backgrounds. No lighting-specific assets.

### Existing Effects
- Some shader usage in rendering (blur, pixelate) for backgrounds/menus.
- Dark overlay potential via `background_dark.png` or dark.frag.
- No current dynamic lighting; rendering is flat.

## 2. Requirements
- Dark transparent overlay on entire scene during animation.
- Each diamond acts as a light source, illuminating a small area around it (radial gradient).
- Lights move with diamonds during animation.
- Reveal/illuminate the board progressively as diamonds animate in.
- Super efficient: Target 60 FPS, handle 112 dynamic lights without significant overhead.
- SFML 3.0.1 compatible.

## 3. Research: GameDev Patterns for 2D Lighting in SFML
Based on SFML forums, gamedev.stackexchange, and common 2D lighting techniques:

### Common Efficient Patterns
1. **Additive Blending Lights**:
   - Render scene normally.
   - Render black overlay (sf::RectangleShape with alpha).
   - Draw light sprites (radial gradient circles) with sf::BlendAdd to "add" light, brightening areas.

2. **Light Map Technique** (More advanced/efficient for multiple lights):
   - Create a sf::RenderTexture light map (same size as view, clear to black/ambient color).
   - Draw soft white circles (or sprites) to light map at each light position with BlendAdd.
   - Use a shader to multiply light map with scene texture (dark areas stay dark, lit areas brighten).
   - Efficient for many lights as it's single pass per light + one shader pass.

3. **Shader-Based Lighting**:
   - Pass light positions to fragment shader.
   - For each pixel, calculate distance to nearest lights and compute intensity.
   - Pros: GPU-efficient for simple lights.
   - Cons: For 112 lights, need to pass array of positions (SFML shaders support uniforms arrays).

4. **Efficiency Considerations**:
   - Avoid per-pixel CPU operations; use GPU via shaders or batch drawing.
   - Vertex arrays for batching multiple lights.
   - Reuse existing dark.frag as base for lighting shader.
   - SFML 3.0.1 supports GLSL shaders with uniform arrays (up to ~1024 elements).
   - Benchmark: Additive blending is simplest/fastest for <100 lights; shader better for dynamic/many.

Recommended: Shader-based for efficiency with 112 moving lights. Modify dark.frag to include point lights.

## 4. Proposed Implementation
- Use shader-based approach: Extend dark.frag to a lighting shader that applies darkness but adds point light contributions.
- During animation, pass array of current diamond positions and light parameters to shader.
- Render scene to RenderTexture, apply lighting shader when drawing to window.
- Lights: Simple point lights with radius ~50-100px, intensity falloff (inverse square or linear).
- Dark overlay: Global darkness factor (e.g., 0.2 ambient light).
- Integrate into `BoardInitializationAnimator` for position data.

### Key Changes
- New shader: `lighting.frag` based on dark.frag, adding point light calculations.
- In `GameRenderer`: For `state_board_animation`, render scene to texture, apply shader.
- Pass uniforms: vec2 lightPositions[112], float lightRadii[112], etc.
- Efficiency: Single shader pass, no extra textures needed.

### Shader Sketch (lighting.frag)
```
uniform sampler2D texture;
uniform vec2 lightPositions[112];
uniform float lightRadii[112];
uniform float ambient = 0.1;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    vec2 fragPos = gl_FragCoord.xy; // Need to pass resolution or use normalized coords
    float lighting = ambient;
    for(int i = 0; i < 112; i++) {
        float dist = distance(fragPos, lightPositions[i]);
        lighting += max(0.0, 1.0 - dist / lightRadii[i]); // Simple linear falloff
    }
    lighting = clamp(lighting, 0.0, 1.0);
    gl_FragColor = color * lighting;
}
```
- Note: Need to normalize positions or pass resolution.

## 5. Step-by-Step Plan
1. **Setup Folder and Assets**:
   - Create `assets/shaders/lighting.frag` with the shader code.
   - (Optional) Create radial gradient texture if needed for sprite-based fallback.

2. **Modify BoardInitializationAnimator**:
   - Add method to get current positions: `std::vector<sf::Vector2f> getCurrentLightPositions()`.
   - In `update()`, collect positions from animatedItems.

3. **Update GameRenderer**:
   - Load new sf::Shader lightingShader.
   - In `renderStateBoardAnimation()`:
     - Render background and other elements to RenderTexture.
     - Draw animated diamonds normally.
     - Get positions from boardAnimator.
     - Set shader uniforms with positions array.
     - Draw RenderTexture to window with lightingShader.

4. **Integrate into State Manager**:
   - Ensure `state_board_animation` uses the new rendering path.
   - Add config for light radius/intensity in BoardAnimationConfig.

5. **Optimization and Testing**:
   - Profile FPS during animation.
   - If slow, reduce light count or simplify shader (e.g., nearest light only).
   - Test on target hardware (Darwin 24.5.0).
   - Add skip option that disables lights.

6. **Documentation**:
   - Update ai-docs/final-docs/animation-diamonds-before-game.md with lighting details.
   - Add to ai-docs/lighting/ folder.

## 6. Potential Issues and Mitigations
- **Performance**: 112 lights in shader loop may be slow on low-end GPU. Mitigate: Cluster lights or use light map with additive blending.
- **Position Coordinates**: Ensure light positions are in screen space. Use sf::Transform to convert.
- **SFML Limits**: Uniform array size; 112 is fine.
- **Edge Cases**: Animation skip, fullscreen toggle during animation.
- **Efficiency Gains**: Batch uniform updates, minimize state changes.

This plan ensures an efficient, visually appealing lighting effect integrated seamlessly with existing animation. 