#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SFML/Graphics.hpp>

class EventHandler {
  private:
    sf::Event event;

    void key_press_mapping(sf::Event& event, sf::RenderWindow& window);

  public:
    EventHandler();

    void check(sf::RenderWindow& window);
};

#endif