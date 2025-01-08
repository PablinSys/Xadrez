#ifndef GAME_UI_H
#define GAME_UI_H

#include <SFML/Graphics.hpp>
#include "../include/array.hpp"
#include "../include/tabuleiro.hpp"

class Tabuleiro;
class GameController;

class GameUI
{
    private:
        Tabuleiro tabuleiro;
        sf::RenderWindow* window;
        sf::VertexArray tabuleiroUI[64];
        void renderTab();
    public:
        float tamanho_casas;
        GameUI(sf::RenderWindow& window, Tabuleiro& tabuleiro);
        void update(GameController* gameC);
        bool OnMouseButtonLeftPressed(GameController* gameC);
        void OnMouseButtonLeftClicked(GameController* gameC);
        ~GameUI();
};


#endif // GAME_UI_H