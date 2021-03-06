#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "engine/Engine.hpp"
#include "engine/ResourceManager.hpp"


class GametitleScreen: public GameScreen
{
public:
    void init();
    void onDraw(sf::RenderTarget& ctx, sf::RenderStates states) const;
    void onUpdate(float dt);
protected:
    sf::Music music;
    void onGamepadEvent(GamepadEvent e);
    bool changed;
    bool pressed;
    sf::Sprite sprite;
    sf::Texture title;
    sf::RectangleShape blackness;
    int trans = 255;
  // sf::Font font;
  // sf::Text t;
};
