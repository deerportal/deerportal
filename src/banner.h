#ifndef BANNER_H
#define BANNER_H
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

class Banner : public sf::Drawable, public sf::Transformable {
public:
  explicit Banner(sf::Font* gameFont);
  void setText(const std::string& newText);
  bool active;

  std::string textStr;
  std::unique_ptr<sf::Text> text;
  float timeDownCounter;
  void update(sf::Time frameTime);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // BANNER_H
