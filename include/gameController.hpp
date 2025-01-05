#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include "../include/tabuleiro.hpp"

class Tabuleiro;
class GameController
{
    private:
        int tamanho_casas = 100;
        Tabuleiro tabuleiro;
    public:
        GameController(Tabuleiro& tabuleiro, const int& tamanho);
        int moverPeça(sf::Vector2i peça_pos, sf::Vector2f new_pos, bool emMovimento);
        bool analisarProtecaoRei(const sf::Vector2i& posRei);
        Peça* (*getTabuleiro())[8];
        //sf::Vector2i getPossiveisMovimentos(Peça* peça);
        ~GameController() = default;
};

#endif // GAME_CONTROL_H