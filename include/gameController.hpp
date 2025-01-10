#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <array>
#include "../include/tabuleiro.hpp"
#include "../include/structs.hpp"

class Tabuleiro;
class GameController
{
    private:
        float tamanho_casas;
        sf::Vector2i posReiJogador, posReiOponente;
        Tabuleiro tabuleiro;
        std::vector<Jogada> getPossiveisMovimentos(Tabuleiro& tab, const bool& isWhite);
    public:
        GameController(Tabuleiro& tabuleiro, const float& tamanho);
        int moverPeça(sf::Vector2i peça_pos, sf::Vector2f new_pos, bool emMovimento);
        Peça* (*getTabuleiro())[8];
        bool analisarEmpate();
        bool analisarCheck(const bool& isWhite);
        bool analisarCheckmate(const bool& isWhite); // responsavel por verificar se o rei esta em checkmate, testando todos os movimentos possiveis das peças para ver se ele esta em check todas
        ~GameController() = default;
};

#endif // GAME_CONTROL_H