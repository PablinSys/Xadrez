#ifndef ALGORITMO_H
#define ALGORITMO_H

#include "../include/peças.hpp"
#include "../include/array.hpp"
#include "../include/tabuleiro.hpp"

class Algoritmo
{
    public:
        static int minimax(Tabuleiro* tabuleiro, const sf::Vector2i& pos_peça, const sf::Vector2i& new_pos, bool maximizando, int alpha, int beta, int profundidade);
};

#endif