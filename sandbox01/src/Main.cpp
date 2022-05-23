/**
 * @brief バブル
 */
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

class Circle : public sf::CircleShape {
public:
  Circle() : sf::CircleShape(rand() % 23 + 10) {
    setFillColor({static_cast<sf::Uint8>(rand() % 256),
                  static_cast<sf::Uint8>(rand() % 256),
                  static_cast<sf::Uint8>(rand() % 256), 255});
    float speed = rand() % 80 + 20;
    vx = rand() % 2 == 0 ? speed : -speed;
    vy = rand() % 2 == 0 ? speed : -speed;
  }

  void update(const sf::Time &dt) {
    auto s = dt.asSeconds();
    move(vx * s, vy * s);
  }

private:
  float vx{0.f}, vy{0.f};
};

int main() {
  constexpr unsigned int sw = 800, sh = 600;
  sf::RenderWindow window(sf::VideoMode(sw, sh), "sandbox01");

  unsigned int maxCircles = 100;
  std::vector<Circle> circles;
  sf::Clock clock;
  sf::Font font;
  sf::Text fpsText, countText;
  if (!font.loadFromFile("Mplus1-Regular.ttf")) {
    std::cout << "font load error" << std::endl;
  }
  fpsText.setFont(font);
  fpsText.setCharacterSize(14);
  fpsText.setPosition(4, 4);
  countText.setFont(font);
  countText.setCharacterSize(14);
  countText.setPosition(4, 22);

  auto processEvents = [&]() {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
  };

  auto update = [&](const sf::Time &dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      if (maxCircles > 0)
        maxCircles--;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      maxCircles++;
    }

    while (circles.size() < maxCircles) {
      Circle circle;
      circle.setPosition(rand() % sw, rand() % sh);
      circles.emplace_back(std::move(circle));
    }

    for (auto &circle : circles) {
      circle.update(dt);
    }

    std::erase_if(circles, [](const auto &circle) {
      auto x = circle.getPosition().x;
      auto y = circle.getPosition().y;
      auto d = circle.getRadius() * 2;
      return x > sw || y > sh || (x + d) < 0 || (y + d) < 0;
    });

    fpsText.setString("FPS: " +
                      std::to_string(1000000.f / dt.asMicroseconds()));
    countText.setString("circles: " + std::to_string(circles.size()) + " / " +
                        std::to_string(maxCircles));
  };

  auto render = [&]() {
    window.clear();
    for (auto &circle : circles) {
      window.draw(circle);
    }
    window.draw(fpsText);
    window.draw(countText);
    window.display();
  };

  while (window.isOpen()) {
    auto dt = clock.restart();
    processEvents();
    update(dt);
    render();
  }

  return 0;
}
