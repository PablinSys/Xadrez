#ifndef TABULEIRO_H
#define TABULEIRO_H


#include "../include/peças.hpp"

class Peça;
class Tabuleiro
{
    private :
        Peça* tabuleiro[8][8];
    public:
        bool brancasPrimeiro; 
        float tamanho_casas;
        Tabuleiro(const bool& brancasPrimeiro, const float& tamanho_casas);
        Tabuleiro(Peça* (*tabuleiro)[8], const bool& brancasPrimeiro, const float& tamanho_casas);
        Peça* (*getTabuleiro())[8];
        void setTabuleiro(Peça* (*tabuleiro)[8]);
        void moverPeça(sf::Vector2i peça_pos, sf::Vector2i new_pos);
        ~Tabuleiro();
};
#endif
