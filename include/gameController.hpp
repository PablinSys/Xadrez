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
    public:
        Tabuleiro* tabuleiro;
        Jogada jogador_jogada = Jogada();
        GameController(Tabuleiro* tabuleiro, const float& tamanho);
        GameController(Tabuleiro& tabuleiro, const float& tamanho, sf::Vector2i posReiJogador, sf::Vector2i posReiOponente);
        bool moverPeça(sf::Vector2i peça_pos, sf::Vector2f new_pos, bool emMovimento);
        static std::vector<Jogada> getPossiveisMovimentos(Tabuleiro& tab, const bool& isWhite);
        static std::vector<Jogada> getPossiveisMovimentos(Tabuleiro& tab, const bool& isWhite, bool isSorted, bool isFiltred, int nivel);
        bool analisarCheckmate(const bool& isWhite); // responsavel por verificar se o rei esta em checkmate, testando todos os movimentos possiveis das peças para ver se ele esta em check todas
        bool analisarCheck(const bool& isWhite);
        bool analisarEmpate() const;
        ~GameController() = default;
};

#endif // GAME_CONTROL_H