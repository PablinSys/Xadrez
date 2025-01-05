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
        Tabuleiro(Peça* (*tabuleiro)[8], const bool& brancasPrimeiro);
        Peça* (*getTabuleiro())[8];
        void setTabuleiro(Peça* (*tabuleiro)[8]);
        ~Tabuleiro();
};
#endif
