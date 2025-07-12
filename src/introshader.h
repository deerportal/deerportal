#ifndef INTROSHADER_H
#define INTROSHADER_H

#include <SFML/Graphics.hpp>

namespace DP {

/*!
 * \brief IntroShader handles the animated shader intro sequence
 *
 * Based on "Creation" by Danilo Guanabara (ShaderToy: XsXXDn)
 * Used with permission: "If you intend to reuse this shader, please add credits to 'Danilo
 * Guanabara'"
 */
class IntroShader {
public:
  IntroShader();
  ~IntroShader();

  bool initialize(sf::Vector2u screenSize);
  bool initialize(sf::Vector2u screenSize, const sf::Texture* backgroundTexture);
  void setIntroTexture(const sf::Texture* introTexture);
  void update(float deltaTime);
  void render(sf::RenderWindow& window);

  bool isFinished() const { return finished; }
  void reset();

private:
  sf::Shader shader;
  sf::RenderTexture renderTexture;
  std::unique_ptr<sf::Sprite> sprite;
  sf::Clock clock;

  bool initialized;
  bool finished;
  float duration;
  float currentTime;

  sf::Vector2u screenSize;
};

} // namespace DP

#endif // INTROSHADER_H