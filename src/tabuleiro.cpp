#include "../include/tabuleiro.hpp"
#include "../include/peças.hpp"
#include <iostream>
#include <cassert>

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
Tabuleiro::Tabuleiro(Peça* (*tabuleiro)[8], const bool& brancasPrimeiro, const float& tamanho_casas)
    : brancasPrimeiro(brancasPrimeiro), tamanho_casas(tamanho_casas)
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
        {
            if (tabuleiro[y][x] != nullptr)
                this->tabuleiro[y][x] = tabuleiro[y][x]->clone();
            else 
                this->tabuleiro[y][x] = nullptr;
        }
}
void Tabuleiro::moverPeça(sf::Vector2i peça_pos, sf::Vector2i new_pos)
{
    Peça* peça = &*tabuleiro[peça_pos.y][peça_pos.x];

    if (Peao* peao = dynamic_cast<Peao*>(peça))
        peao->primeiroLance = false;

    peça->objectUI.setPosition( 
    {
        (float)new_pos.x * tamanho_casas + (tamanho_casas-60)/2 , 
        (float)new_pos.y * tamanho_casas + (tamanho_casas-60)/2
    });
    peça->positionIndex = {new_pos.x, new_pos.y};

    tabuleiro[new_pos.y][new_pos.x] = peça;
    tabuleiro[peça_pos.y][peça_pos.x] = nullptr;

    assert(tabuleiro[peça_pos.y][peça_pos.x] == nullptr);
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