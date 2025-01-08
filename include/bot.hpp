#ifndef BOT_H
#define BOT_H

#include <SFML/Graphics.hpp>
#include "../include/game.hpp"
#include "../include/algoritmo.hpp"
#include "../include/peças.hpp"
#include "../include/tabuleiro.hpp"

class Bot {
        Game game;
        void Animaçao(sf::Vector2i pos_peça, sf::Vector2f new_pos);
    public:
        Bot(Game& game);
        void jogar(Tabuleiro* tabuleiro);
        void analisarMelhorMovimento(Tabuleiro* tabuleiro);
};

#endif