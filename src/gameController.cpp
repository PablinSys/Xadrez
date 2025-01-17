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

// TODO: Implementar a funcionalidade de filtrar e ordenar os movimentos
/**
 * @brief Obtem todos os movimentos possiveis de todas as peças do tabuleiro
 * 
 * Faz um loop por todas as peças do tabuleiro e obtem todos os movimentos possiveis para cada peça
 * com sua cor especificada e analisando se o movimento possibilita o Rei nao estar em check
 * 
 * @param tab Tabuleiro 
 * @param isWhite True se as peças forem brancas, False se forem pretas
 * 
 * @return std::vector<Jogada> Todos os movimentos possiveis de todas as peças pretas ou brancas
 */
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
                    if (movimento.new_pos != movimento.peça_pos && !(dynamic_cast<Rei*>(tab.getTabuleiro()[movimento.new_pos.y][movimento.new_pos.x])))
                    {
                        // Verificando se o movimento possibilita o Rei nao estar em check
                        Tabuleiro test_tab = tab;
                        test_tab.moverPeça({x, y}, movimento.new_pos);

                        sf::Vector2i rei_pos = dynamic_cast<Rei*>(test_tab.getTabuleiro()[movimento.new_pos.y][movimento.new_pos.x]) != nullptr ? movimento.new_pos : Tabuleiro::getReiPosition(&tab, isWhite);

                        if (Rei* rei = dynamic_cast<Rei*>(test_tab.getTabuleiro()[rei_pos.y][rei_pos.x]))
                            if (!rei->isCheck(&test_tab, isWhite))
                                // Se o Rei nao estiver em check, adiciona o movimento
                                movimentos.push_back(movimento);
                    }
            }
        }
    }
    return movimentos;
}

/**
 * @brief Verifica se o Rei esta em checkmate
 * 
 * Verifica todos os movimentos possíveis das peças que possibilitam o Rei nao estar em check
 * 
 * @param isWhite True se a peça for branca, False se for preta
 * 
 * @return True se o Rei estiver em checkmate False se nao
 */
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

        if (dynamic_cast<Rei*>(peça)) {
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

/**
 * @brief Verifica se o Rei esta em check
 * 
 * Chama a funcao isCheck para verificar se o Rei estiver em check
 * 
 * @param isWhite True se a peça for branca, False se for preta
 *  
 * @return True se o Rei estiver em check False se nao
 * 
 */
bool GameController::analisarCheck(const bool& isWhite)
{
    if (isWhite != tabuleiro->brancasPrimeiro)
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

/**
 * @brief Mover peça no tabuleiro
 * 
 * Faz varias verificacoes e movimenta a peça no tabuleiro
 * 
 * @param peça_pos Posicao da peça
 * @param new_pos Posicao para mover a peça
 * @param emMovimento True se a peça estiver sendo movida, False se nao
 * 
 * @return True se a peça foi movida False se nao
 * 
 */
bool GameController::moverPeça(sf::Vector2i peça_pos, sf::Vector2f new_pos, bool emMovimento)
{
    if (tabuleiro->getTabuleiro()[peça_pos.y][peça_pos.x] != nullptr)
    {
        std::vector<std::vector<Peça*>> tab = this->tabuleiro->getTabuleiro();
        int index_y = (int)(new_pos.y / tamanho_casas), index_x = (int)(new_pos.x / tamanho_casas);

        if (emMovimento)
            tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({new_pos.x-20, new_pos.y-20});
        else
        {
            // Verificar se na nova posicao tem uma peça
            if (tab[index_y][index_x] != nullptr)
            {
                // Se tiver e é da mesma cor, então essa nova posição é invalida
                if (tab[index_y][index_x]->isWhite == tab[peça_pos.y][peça_pos.x]->isWhite)
                {
                    tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({
                        (float)(peça_pos.x * tamanho_casas + (tamanho_casas-60)/2), 
                        (float)(peça_pos.y * tamanho_casas + (tamanho_casas-60)/2)
                    });
                    return false;
                }
            }

            // Caso a peça movida for rei, atualizar as informacoes de posicao do rei
            if (typeid(*tab[peça_pos.y][peça_pos.x]) == typeid(Rei))
                if (tab[peça_pos.y][peça_pos.x]->isWhite == tabuleiro->brancasPrimeiro)
                    posReiJogador = {index_x, index_y};
                else 
                    posReiOponente = {index_x, index_y};
            
            // Apenas para testes
            if (tab[peça_pos.y][peça_pos.x]->isWhite == tabuleiro->brancasPrimeiro){
                system("clear");
                Algoritmo::avaliacao(tabuleiro, {index_x, index_y}, true, 4, true);
            }

            // Movendo a peça
            this->tabuleiro->moverPeça(peça_pos, {index_x, index_y});
            // Atualizando jogador_jogada
            jogador_jogada = Jogada(peça_pos, {index_x, index_y});
        }
        return true;
    }
    return false;
}
