#include "../include/tabuleiro.hpp"
#include "../include/peças.hpp"

Tabuleiro::Tabuleiro(const bool& brancasPrimeiro)
    : brancasPrimeiro(brancasPrimeiro)
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            tabuleiro[y][x] = nullptr;
    
    // inicializando o tabuleiro
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (y < 2)
            {
                if (y == 0)
                {
                    if (x == 0 || x == 7)
                        tabuleiro[y][x] = new Torre(sf::Vector2i({x, y}), *this, !brancasPrimeiro);
                    else if (x == 1 || x == 6)
                        tabuleiro[y][x] = new Cavalo(sf::Vector2i({x, y}), *this, !brancasPrimeiro);
                    else if (x == 2 || x == 5)
                        tabuleiro[y][x] = new Bispo(sf::Vector2i({x, y}), *this, !brancasPrimeiro);
                    else if (x == 3)
                        tabuleiro[y][x] = new Rainha(sf::Vector2i({x, y}), *this, !brancasPrimeiro);
                    else if (x == 4)
                        tabuleiro[y][x] = new Rei(sf::Vector2i({x, y}), *this, !brancasPrimeiro);
                }
                else 
                    tabuleiro[y][x] = new Peao(sf::Vector2i({x, y}), *this, !brancasPrimeiro, true);
            }
            else if (y > 5)
            {
                if (y == 7)
                {
                    if (x == 0 || x == 7)
                        tabuleiro[y][x] = new Torre(sf::Vector2i({x, y}), *this, brancasPrimeiro);
                    else if (x == 1 || x == 6)
                        tabuleiro[y][x] = new Cavalo(sf::Vector2i({x, y}), *this, brancasPrimeiro);
                    else if (x == 2 || x == 5)
                        tabuleiro[y][x] = new Bispo(sf::Vector2i({x, y}), *this, brancasPrimeiro);
                    else if (x == 3)
                        tabuleiro[y][x] = new Rainha(sf::Vector2i({x, y}), *this, brancasPrimeiro);
                    else if (x == 4)
                        tabuleiro[y][x] = new Rei(sf::Vector2i({x, y}), *this, brancasPrimeiro);
                }
                else 
                    tabuleiro[y][x] = new Peao(sf::Vector2i({x, y}), *this, brancasPrimeiro, true);
            }
            else 
                tabuleiro[y][x] = nullptr;
        }
    }
}
Tabuleiro::Tabuleiro(Peça* (*tabuleiro)[8], const bool& brancasPrimeiro)
    : brancasPrimeiro(brancasPrimeiro)
{
    setTabuleiro(tabuleiro);
}

Peça* (*Tabuleiro::getTabuleiro())[8]
{
    return tabuleiro;
}

void Tabuleiro::setTabuleiro(Peça* (*tabuleiro)[8])
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            this->tabuleiro[y][x] = tabuleiro[y][x];
}
Tabuleiro::~Tabuleiro()
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (tabuleiro[y][x] != nullptr)
                delete tabuleiro[y][x];
        }
    }
}