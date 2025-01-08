#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include "../include/tabuleiro.hpp"
#include "../include/structs.hpp"
#include "../include/array.hpp"

class Tabuleiro;
class GameController
{
    private:
        float tamanho_casas;
        sf::Vector2i posReiJogador, posReiOponente;
        Tabuleiro tabuleiro;
        //Array<Jogada, 1> getPossiveisMovimentos(Peça* peça);
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