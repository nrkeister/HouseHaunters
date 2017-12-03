#include <SFML/Graphics.hpp>
#include <memory>
#include "engine/Engine.hpp"


class CharacterScreen: public GameScreen
{
public:
    void init();
    void onDraw(sf::RenderTarget& ctx, sf::RenderStates states) const;
    void onUpdate(float dt);
protected:
    void onGamepadEvent(GamepadEvent e);
    bool changed;
    sf::Sprite chara1;
    sf::Sprite chara2;
    sf::Sprite chara3;
    sf::Sprite chara4;
    sf::RectangleShape background;
    sf::Text teamFont;
    sf::Texture title;
    sf::RectangleShape blackness;
    int trans = 255;
  // sf::Font font;
  // sf::Text t;
};
