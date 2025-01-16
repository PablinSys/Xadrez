#include "../include/gameController.hpp"
#include "../include/algoritmo.hpp"
#include <iostream>
#include <cassert>
#include <functional>

GameController::GameController(Tabuleiro* tabuleiro, const float& tamanho)
    : tabuleiro(tabuleiro), tamanho_casas(tamanho)
{
    posReiJogador = sf::Vector2i(4, 7);
    posReiOponente = sf::Vector2i(4, 0);
}
GameController::GameController(Tabuleiro& tabuleiro, const float& tamanho, sf::Vector2i posReiJogador, sf::Vector2i posReiOponente)
    : tabuleiro(&tabuleiro), tamanho_casas(tamanho), posReiJogador(posReiJogador), posReiOponente(posReiOponente) {}
std::vector<Jogada> GameController::getPossiveisMovimentos(Tabuleiro& tab, const bool& isWhite)
{
    std::vector<Jogada> movimentos;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (tab.getTabuleiro()[y][x] != nullptr && tab.getTabuleiro()[y][x]->isWhite == isWhite)
            {
                std::vector<Jogada> movimentos_pecas = tab.getTabuleiro()[y][x]->movimentosPossiveis(&tab);
                for (Jogada& movimento : movimentos_pecas)
                    if (movimento.new_pos != movimento.peça_pos)
                    {
                        Tabuleiro test_tab = tab;
                        test_tab.moverPeça({x, y}, movimento.new_pos);
                        sf::Vector2i rei_pos;
                        try {
                            rei_pos = (typeid(*tab.getTabuleiro()[y][x]) == typeid(Rei)) ? sf::Vector2i(x, y) : test_tab.getReiPosition(&test_tab, isWhite);
                        } catch (const std::runtime_error& e) {
                            continue;
                        }
                        if (Rei* rei = dynamic_cast<Rei*>(test_tab.getTabuleiro()[rei_pos.y][rei_pos.x]))
                            if (!rei->isCheck(&test_tab, isWhite))
                                movimentos.push_back(movimento);
                    }
            }
        }
    }
    return movimentos;
}
bool GameController::analisarCheckmate(const bool& isWhite)
{
    std::vector<Jogada> movimentos = GameController::getPossiveisMovimentos(*tabuleiro, isWhite);
    Rei* rei;
    Tabuleiro test_tab = *tabuleiro;
    for (Jogada& movimento : movimentos)
    {
        rei = dynamic_cast<Rei*>(tabuleiro->brancasPrimeiro == isWhite ? &*tabuleiro->getTabuleiro()[posReiJogador.y][posReiJogador.x] : 
                                                                        &*tabuleiro->getTabuleiro()[posReiOponente.y][posReiOponente.x]);
        if (movimento.new_pos == (tabuleiro->brancasPrimeiro == isWhite ? posReiJogador : posReiOponente))
            continue;
        // Mover peça
        test_tab.moverPeça(movimento.peça_pos, movimento.new_pos);

        // Verificar se a peça movida é o Rei
        Peça* peça = tabuleiro->getTabuleiro()[movimento.peça_pos.y][movimento.peça_pos.x];

        if (Rei* rei2 = dynamic_cast<Rei*>(peça)) {
            rei = dynamic_cast<Rei*>(test_tab.getTabuleiro()[movimento.new_pos.y][movimento.new_pos.x]);
        }
        // Verificar se o Rei esta em check
        if (!rei->isCheck(&test_tab, isWhite))
        {
            return false;
        }
    }
    return true;
}
bool GameController::analisarCheck(const bool& isWhite)
{
    if (isWhite == tabuleiro->brancasPrimeiro)
    {
        Rei* rei = dynamic_cast<Rei*>(&*tabuleiro->getTabuleiro()[posReiOponente.y][posReiOponente.x]);
        if (!rei->isCheck(tabuleiro, !tabuleiro->brancasPrimeiro))
            return false;
        else 
        {
            //analisarCheckmate(false);
            return true;
        }
    }
    else
    {
        Rei* rei = dynamic_cast<Rei*>(&*tabuleiro->getTabuleiro()[posReiJogador.y][posReiJogador.x]);
        if (!rei->isCheck(tabuleiro, tabuleiro->brancasPrimeiro))
            return false;
        else 
            return true;
    }
}
bool GameController::moverPeça(sf::Vector2i peça_pos, sf::Vector2f new_pos, bool emMovimento)
{
    if (tabuleiro->getTabuleiro()[peça_pos.y][peça_pos.x] != nullptr)
    {
        std::vector<std::vector<Peça*>> tab = this->tabuleiro->getTabuleiro();
        if (emMovimento)
            tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({new_pos.x-20, new_pos.y-20});
        else if (tab[peça_pos.y][peça_pos.x]->analisarMovimento(tabuleiro, {(int)new_pos.x, (int)new_pos.y}))
        {
            int index_y = (int)(new_pos.y / tamanho_casas), index_x = (int)(new_pos.x / tamanho_casas);
            if (tab[index_y][index_x] != nullptr)
            {
                if (tab[index_y][index_x]->isWhite == tab[peça_pos.y][peça_pos.x]->isWhite)
                {
                    tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({
                        (float)(peça_pos.x * tamanho_casas + (tamanho_casas-60)/2), 
                        (float)(peça_pos.y * tamanho_casas + (tamanho_casas-60)/2)
                    });
                    return false;
                }
            }
            if (Rei* rei = dynamic_cast<Rei*>(tab[peça_pos.y][peça_pos.x]->isWhite == tabuleiro->brancasPrimeiro ? 
                                                tab[posReiJogador.y][posReiJogador.x] :
                                                tab[posReiOponente.y][posReiOponente.x]))
            {
                Tabuleiro test_tab = *tabuleiro;
                test_tab.moverPeça(peça_pos, {index_x, index_y});

                if (typeid(*tab[peça_pos.y][peça_pos.x]) == typeid(Rei))
                    rei = dynamic_cast<Rei*>(test_tab.getTabuleiro()[index_y][index_x]);

                if (!rei->isCheck(&test_tab, tab[peça_pos.y][peça_pos.x]->isWhite == tabuleiro->brancasPrimeiro))
                {
                    if (typeid(*tab[peça_pos.y][peça_pos.x]) == typeid(Rei))
                    {
                        if (tab[peça_pos.y][peça_pos.x]->isWhite == tabuleiro->brancasPrimeiro)
                            posReiJogador = {index_x, index_y};
                        else 
                            posReiOponente = {index_x, index_y};
                    }
                    if (tab[peça_pos.y][peça_pos.x]->isWhite == tabuleiro->brancasPrimeiro){
                        system("clear");
                        Algoritmo::avaliacao(tabuleiro, {index_x, index_y}, true, 4, true);
                    }
                    this->tabuleiro->moverPeça(peça_pos, {index_x, index_y});
                    jogador_jogada = Jogada(peça_pos, {index_x, index_y});
                }
                else 
                {
                    tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({
                        (float)(peça_pos.x * tamanho_casas + (tamanho_casas-60)/2), 
                        (float)(peça_pos.y * tamanho_casas + (tamanho_casas-60)/2)
                    });
                    return false;
                }
            }
            else 
            {
                std::cerr << "ERRO: Rei nao encontrado" << std::endl;
                return false;
            }
        }
        else
        {
            tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({
                (float)(peça_pos.x * tamanho_casas + (tamanho_casas-60)/2), 
                (float)(peça_pos.y * tamanho_casas + (tamanho_casas-60)/2)
             });            
             return false;
        }
        return true;
    }
    return false;
}
