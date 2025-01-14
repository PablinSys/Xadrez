#ifndef ALGORITMO_H
#define ALGORITMO_H

#include "../include/peças.hpp"
#include "../include/tabuleiro.hpp"
#include "../include/gameController.hpp"
#include <unordered_map>

class Algoritmo
{
    struct JogadasLess {
        bool operator()(const Jogada& a, const Jogada& b) const {
            return a.new_pos.x > b.new_pos.x || (a.new_pos.x == b.new_pos.x && a.new_pos.y > b.new_pos.y);
        }
    };
    GameController* gameC;
    public:
        std::map<Jogada, std::map<Jogada, float, JogadasLess>, JogadasLess> cache;
        Algoritmo(GameController* gameC) : gameC(gameC) {};
        float minimax(Tabuleiro* tabuleiro, const sf::Vector2i& pos_peça, const sf::Vector2i& new_pos, bool maximizando, float alpha, float beta, int profundidade, int nivel);
        inline void Esvaziar_cache(){cache.clear();}
        static float avaliacao(Tabuleiro* tabuleiro, sf::Vector2i pos_peça, sf::Vector2i new_pos, bool isJogador, int nivel, bool debug);
};

#endif