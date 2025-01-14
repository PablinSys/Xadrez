#ifndef TABULEIRO_H
#define TABULEIRO_H


#include "../include/peças.hpp"

class Peça;
class Tabuleiro
{
    private :
        std::vector<std::vector<Peça*>> tabuleiro;
    public:
        bool brancasPrimeiro; 
        float tamanho_casas;
        Tabuleiro(const bool& brancasPrimeiro, const float& tamanho_casas);
        Tabuleiro(const Tabuleiro& other);
        void moverPeça(sf::Vector2i peça_pos, sf::Vector2i new_pos);
        static sf::Vector2i getReiPosition(Tabuleiro* tabuleiro, const bool& isWhite);
        std::vector<std::vector<Peça*>> getTabuleiro() const;
        ~Tabuleiro();
};
#endif
