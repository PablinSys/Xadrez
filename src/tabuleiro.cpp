#include "../include/tabuleiro.hpp"
#include "../include/peças.hpp"
#include <iostream>

Tabuleiro::Tabuleiro(const bool& brancasPrimeiro, const float& tamanho_casas)
    : brancasPrimeiro(brancasPrimeiro), tamanho_casas(tamanho_casas)
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
                std::string cor = brancasPrimeiro ? "black" : "white";
                if (y == 0)
                {
                    if (x == 0 || x == 7)
                        tabuleiro[y][x] = new Torre(std::filesystem::current_path() / ("assets/" + cor + "/" + "torre" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro);
                    else if (x == 1 || x == 6)
                        tabuleiro[y][x] = new Cavalo(std::filesystem::current_path() / ("assets/" + cor + "/" + "cavalo" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro);
                    else if (x == 2 || x == 5)
                        tabuleiro[y][x] = new Bispo(std::filesystem::current_path() / ("assets/" + cor + "/" + "bispo" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro);
                    else if (x == 3)
                        tabuleiro[y][x] = new Rainha(std::filesystem::current_path() / ("assets/" + cor + "/" + "rainha" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro);
                    else if (x == 4)
                        tabuleiro[y][x] = new Rei(std::filesystem::current_path() / ("assets/" + cor + "/" + "rei" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro);
                }
                else 
                    tabuleiro[y][x] = new Peao(std::filesystem::current_path() / ("assets/" + cor + "/" + "peao" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro, true);
            }
            else if (y > 5)
            {
                std::string cor = brancasPrimeiro ? "white" : "black";
                if (y == 7)
                {
                    if (x == 0 || x == 7)
                        tabuleiro[y][x] = new Torre(std::filesystem::current_path() / ("assets/" + cor + "/" + "torre" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro);
                    else if (x == 1 || x == 6)
                        tabuleiro[y][x] = new Cavalo(std::filesystem::current_path() / ("assets/" + cor + "/" + "cavalo" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro);
                    else if (x == 2 || x == 5)
                        tabuleiro[y][x] = new Bispo(std::filesystem::current_path() / ("assets/" + cor + "/" + "bispo" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro);
                    else if (x == 3)
                        tabuleiro[y][x] = new Rainha(std::filesystem::current_path() / ("assets/" + cor + "/" + "rainha" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro);
                    else if (x == 4)
                        tabuleiro[y][x] = new Rei(std::filesystem::current_path() / ("assets/" + cor + "/" + "rei" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro);
                }
                else 
                    tabuleiro[y][x] = new Peao(std::filesystem::current_path() / ("assets/" + cor + "/" + "peao" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro, true);

            }
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
            this->tabuleiro[y][x] = *(tabuleiro[y] + x);
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