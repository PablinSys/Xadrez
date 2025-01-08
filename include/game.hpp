#ifndef GAME_H
#define GAME_H

#include "gameController.hpp"
#include "gameUI.hpp"

class Game 
{
    public:
        Tabuleiro* tabuleiro;
        sf::RenderWindow* window;
        GameController* gameController;
        GameUI* gameUI;
        Game(sf::RenderWindow* window);
        void run();
        ~Game() = default;
};



#endif // GAME_H