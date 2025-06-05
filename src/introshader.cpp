#include "introshader.h"
#include "tilemap.h"  // For get_full_path
#include <iostream>

namespace DP {

IntroShader::IntroShader()
    : initialized(false), finished(false), duration(8.0f), currentTime(0.0f)
{
}

IntroShader::~IntroShader()
{
}

bool IntroShader::initialize(sf::Vector2u screenSize)
{
    this->screenSize = screenSize;
    
    // Try GLSL version 120 first (OpenGL 2.1)
    const std::string fragmentShader120 = R"(
#version 120

// Shader based on "Creation" by Danilo Guanabara (ShaderToy: XsXXDn)
// Credits to 'Danilo Guanabara'
// Original: https://www.shadertoy.com/view/XsXXDn

uniform float iTime;
uniform vec2 iResolution;

void main()
{
    vec3 c;
    float l, z = iTime;
    for(int i = 0; i < 3; i++) {
        vec2 uv, p = gl_FragCoord.xy / iResolution.xy;
        uv = p;
        p -= 0.5;
        p.x *= iResolution.x / iResolution.y;
        z += 0.07;
        l = length(p);
        uv += p / l * (sin(z) + 1.0) * abs(sin(l * 9.0 - z - z));
        c[i] = 0.01 / length(mod(uv, 1.0) - 0.5);
    }
    gl_FragColor = vec4(c / l, 1.0);
}
)";

    // Fallback: Legacy GLSL without version declaration
    const std::string fragmentShaderLegacy = R"(
// Shader based on "Creation" by Danilo Guanabara (ShaderToy: XsXXDn)
// Credits to 'Danilo Guanabara'
// Original: https://www.shadertoy.com/view/XsXXDn

uniform float iTime;
uniform vec2 iResolution;

void main()
{
    vec3 c;
    float l, z = iTime;
    for(int i = 0; i < 3; i++) {
        vec2 uv, p = gl_FragCoord.xy / iResolution.xy;
        uv = p;
        p -= 0.5;
        p.x *= iResolution.x / iResolution.y;
        z += 0.07;
        l = length(p);
        uv += p / l * (sin(z) + 1.0) * abs(sin(l * 9.0 - z - z));
        c[i] = 0.01 / length(mod(uv, 1.0) - 0.5);
    }
    gl_FragColor = vec4(c / l, 1.0);
}
)";

    // Try GLSL 120 first
    if (shader.loadFromMemory(fragmentShader120, sf::Shader::Type::Fragment))
    {
        std::cout << "Intro shader loaded successfully with GLSL 120" << std::endl;
    }
    // Fallback to legacy GLSL
    else if (shader.loadFromMemory(fragmentShaderLegacy, sf::Shader::Type::Fragment))
    {
        std::cout << "Intro shader loaded successfully with legacy GLSL" << std::endl;
    }
    else
    {
        std::cerr << "Failed to load intro shader with any GLSL version" << std::endl;
        return false;
    }

    // Create render texture
    if (!renderTexture.resize(screenSize))
    {
        std::cerr << "Failed to create intro shader render texture" << std::endl;
        return false;
    }

    // Setup sprite - initialize after render texture is created
    sprite = std::make_unique<sf::Sprite>(renderTexture.getTexture());
    
    initialized = true;
    return true;
}

void IntroShader::update(float deltaTime)
{
    if (!initialized || finished) return;
    
    currentTime += deltaTime;
    
    // Finish after duration
    if (currentTime >= duration)
    {
        finished = true;
    }
}

void IntroShader::render(sf::RenderWindow& window)
{
    if (!initialized || finished || !sprite) return;
    
    // Set shader uniforms
    shader.setUniform("iTime", currentTime);
    shader.setUniform("iResolution", sf::Vector2f(screenSize.x, screenSize.y));
    
    // Clear render texture and render fullscreen quad with shader
    renderTexture.clear(sf::Color::Black);
    
    // Create a fullscreen rectangle for the shader
    sf::RectangleShape fullscreenQuad(sf::Vector2f(screenSize.x, screenSize.y));
    fullscreenQuad.setPosition(sf::Vector2f(0.0f, 0.0f));
    
    // Render with shader
    renderTexture.draw(fullscreenQuad, &shader);
    renderTexture.display();
    
    // Draw to window
    window.draw(*sprite);
}

void IntroShader::reset()
{
    currentTime = 0.0f;
    finished = false;
    clock.restart();
}

} // namespace DP 