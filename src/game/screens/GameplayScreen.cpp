#include "game/screens/GameplayScreen.hpp"
#include "engine/Random.hpp"
#include "game/characters/Character.hpp"
#include <iostream>

void GameplayScreen::init()
{
    group.generateRoomGrid(8);
    for(int i=0; i < numplayers; i++)
    {
        view = std::unique_ptr<PlayerView>(new PlayerView());
        character = std::shared_ptr<Character>(new Character());
        character->setGroup(&group);
        character->init();
        this->activeCharacters.push_back(std::move(character));
        view->setTotalPlayers(numplayers);
        view->setPlayerNumber(i);
        view->setCharacter(activeCharacters[i]);
        view->setCharacterList(&activeCharacters);
        this->addChild(std::move(view));
    }
}
void GameplayScreen::onDraw(sf::RenderTarget& ctx, sf::RenderStates states) const
{
    // ctx.draw(group, states);
}
