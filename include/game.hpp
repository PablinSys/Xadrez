#ifndef GAME_H
#define GAME_H

#include "gameController.hpp"
#include "gameUI.hpp"

class Game 
{
    private:
        Tabuleiro* tabuleiro;
        sf::RenderWindow* window;
        GameController* gameController;
        GameUI* gameUI;
    public:
        Game(sf::RenderWindow* window);
        void run();
        ~Game() = default;
};



#endif // GAME_H