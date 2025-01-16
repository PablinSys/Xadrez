#ifndef ALGORITMO_H
#define ALGORITMO_H

#include "../include/peças.hpp"
#include "../include/tabuleiro.hpp"
#include "../include/gameController.hpp"
#include <unordered_map>

class Algoritmo
{
    GameController* gameC;
    struct Vector2iHash {
        std::size_t operator()(const sf::Vector2i& v) const {
            return std::hash<int>{}(v.x) ^ std::hash<int>{}(v.y);
        }
    };

    struct JogadaHash {
        std::size_t operator()(const Jogada& j) const {
            return std::hash<int>{}(j.new_pos.x) ^ std::hash<int>{}(j.new_pos.y) ^
                std::hash<int>{}(j.peça_pos.x) ^ std::hash<int>{}(j.peça_pos.y);
        }
    };
    public:
        // armazenando possíveis movimentos do oponente com os possíveis movimentos contra e suas pontuaç
        // estrutura: <posição do oponente, <movimento contra, pontuação>>
        std::unordered_map<sf::Vector2i, std::unordered_map<Jogada, float, JogadaHash>, Vector2iHash> cache;
        Algoritmo(GameController* gameC) : gameC(gameC) {}
        float minimax(Tabuleiro* tabuleiro, const sf::Vector2i& pos_peça, bool maximizando, float alpha, float beta, int profundidade, bool usageCache);
        inline void Esvaziar_cache(){cache.clear();}
        static float avaliacao(Tabuleiro* tabuleiro, sf::Vector2i pos_peça, bool isJogador, int nivel, bool debug);
};

#endif