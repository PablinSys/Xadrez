#ifndef BOT_H
#define BOT_H

#include <SFML/Graphics.hpp>
#include "../include/game.hpp"
#include "../include/algoritmo.hpp"
#include "../include/peças.hpp"
#include "../include/tabuleiro.hpp"

class Bot {
        Game game;
        Algoritmo algoritmo;
        int jogadas;
        void Animaçao(sf::Vector2i pos_peça, sf::Vector2i new_pos);
        Jogada analisarMelhorMovimento(Tabuleiro* tabuleiro, const Jogada& jogador_movimento);
    public:
        Bot(Game& game);
        void jogar();
};

#endif