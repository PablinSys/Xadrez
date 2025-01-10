#include "../include/gameController.hpp"
#include <iostream>
#include <cassert>

GameController::GameController(Tabuleiro& tabuleiro, const float& tamanho)
    : tabuleiro(tabuleiro), tamanho_casas(tamanho)
{
    posReiJogador = sf::Vector2i(4, 7);
    posReiOponente = sf::Vector2i(4, 0);
}
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
                movimentos.insert(movimentos.end(), movimentos_pecas.begin(), movimentos_pecas.end());
            }
        }
    }
    return movimentos;
}
bool GameController::analisarCheckmate(const bool& isWhite)
{
    std::vector<Jogada> movimentos = getPossiveisMovimentos(tabuleiro, isWhite);
    Rei* rei = dynamic_cast<Rei*>(tabuleiro.brancasPrimeiro == isWhite ? &*tabuleiro.getTabuleiro()[posReiJogador.y][posReiJogador.x] : 
                                                                        &*tabuleiro.getTabuleiro()[posReiOponente.y][posReiOponente.x]);
    Tabuleiro* test_tab = new Tabuleiro(tabuleiro.brancasPrimeiro, tamanho_casas);
    for (Jogada& movimento : movimentos)
    {
        if (movimento.new_pos == (tabuleiro.brancasPrimeiro == isWhite ? posReiJogador : posReiOponente))
            continue;

        // Copiar tabuleiro
        test_tab->setTabuleiro(tabuleiro.getTabuleiro());
        // Mover peça
        test_tab->moverPeça(movimento.peça_pos, movimento.new_pos);

        // Verificar se a peça movida é o Rei
        auto& peça = test_tab->getTabuleiro()[movimento.peça_pos.y][movimento.peça_pos.x];

        if (peça != nullptr && typeid(*peça) == typeid(Rei)) {
            rei = dynamic_cast<Rei*>(test_tab->getTabuleiro()[movimento.new_pos.y][movimento.new_pos.x]);
        }

        // Verificar se o Rei esta em check
        //std::cout << "\x1B[2J\x1B[H"; // clear terminal
        if (rei->contarPecasMarcando(test_tab, isWhite) == 0)
        {
            std::cout << "Movimento que evita o checkmate " << " em (" << movimento.peça_pos.x << ", " << movimento.peça_pos.y << ") para (" << movimento.new_pos.x << ", " << movimento.new_pos.y << std::endl;
            delete test_tab;
            return false;
        }
    }
    return true;
}
bool GameController::analisarCheck(const bool& isWhite)
{
    if (isWhite == tabuleiro.brancasPrimeiro)
    {
        Rei* rei = dynamic_cast<Rei*>(&*tabuleiro.getTabuleiro()[posReiOponente.y][posReiOponente.x]);
        if (rei->contarPecasMarcando(&tabuleiro, false) == 0)
            return false;
        else 
        {
            //analisarCheckmate(false);
            return true;
        }
    }
    else
    {
        Rei* rei = dynamic_cast<Rei*>(&*tabuleiro.getTabuleiro()[posReiJogador.y][posReiJogador.x]);
        if (rei->contarPecasMarcando(&tabuleiro, false) == 0)
            return false;
        else 
        {
            //analisarCheckmate(false);
            return true;
        }
    }
}
int GameController::moverPeça(sf::Vector2i peça_pos, sf::Vector2f new_pos, bool emMovimento)
{
    if (tabuleiro.getTabuleiro()[peça_pos.y][peça_pos.x] != nullptr)
    {
        Peça* (*tab)[8] = this->tabuleiro.getTabuleiro();
        if (emMovimento)
            tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({new_pos.x-20, new_pos.y-20});
            
        else if (tab[peça_pos.y][peça_pos.x]->analisarMovimento(&tabuleiro, {(int)new_pos.x, (int)new_pos.y}))
        {
            int index_y = (int)(new_pos.y / tamanho_casas), index_x = (int)(new_pos.x / tamanho_casas);
            if (tab[index_y][index_x] != nullptr)
            {
                std::cout << "Analisando cor da peça inimiga" << std::endl;
                if (tab[index_y][index_x]->isWhite == tab[peça_pos.y][peça_pos.x]->isWhite)
                {
                    std::cout << "Peça amiga detectada" << std::endl;
                    tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({
                        (float)(peça_pos.x * tamanho_casas + (tamanho_casas-60)/2), 
                        (float)(peça_pos.y * tamanho_casas + (tamanho_casas-60)/2)
                    });
                    return - 1;
                }
            }
            if (Rei* rei = dynamic_cast<Rei*>(tab[peça_pos.y][peça_pos.x]->isWhite == tabuleiro.brancasPrimeiro ? 
                                                tab[posReiJogador.y][posReiJogador.x] :
                                                tab[posReiOponente.y][posReiOponente.x]))
            {
                std::cout << "Rei detectado" << std::endl;
                Tabuleiro* test_tab = new Tabuleiro(tabuleiro.brancasPrimeiro, tamanho_casas);
                test_tab->setTabuleiro(tab);
                test_tab->moverPeça(peça_pos, {index_x, index_y});
                if (typeid(*tab[peça_pos.y][peça_pos.x]) == typeid(Rei))
                    rei = dynamic_cast<Rei*>(test_tab->getTabuleiro()[index_y][index_x]);

                std::cout << "TESTANDO A PROTEÇÃO DO REI" << std::endl;
                if (rei->contarPecasMarcando(test_tab, tab[peça_pos.y][peça_pos.x]->isWhite == tabuleiro.brancasPrimeiro) == 0)
                {
                    if (typeid(*tab[peça_pos.y][peça_pos.x]) == typeid(Rei))
                    {
                        std::cout << "Rei movido, mudando posição : (" << index_x << ", " << index_y << ")" << std::endl;
                        if (tab[peça_pos.y][peça_pos.x]->isWhite == tabuleiro.brancasPrimeiro)
                            posReiJogador = {index_x, index_y};
                        else 
                            posReiOponente = {index_x, index_y};
                    }

                    this->tabuleiro.moverPeça(peça_pos, {index_x, index_y});
                    std::cout << "MOVIMENTO EFETUADO" << std::endl;
                }
                else 
                {
                    assert(test_tab->getTabuleiro()[index_y][index_x] != tab[index_y][index_x]);
                    std::cout << "ERRO : Rei nao protegido -> ";
                    if (test_tab->getTabuleiro()[index_y][index_x] != nullptr) std::cout << typeid(*test_tab->getTabuleiro()[index_y][index_x]).name() << std::endl;
                    else 
                        std::cout << "NULL" << std::endl;
                    tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({
                        (float)(peça_pos.x * tamanho_casas + (tamanho_casas-60)/2), 
                        (float)(peça_pos.y * tamanho_casas + (tamanho_casas-60)/2)
                    });
                    return - 1;
                }
            }
            else 
            {
                std::cout << "ERRO : Rei não encontrado : ( " << peça_pos.x << " , " << peça_pos.y << " )" << std::endl; 
                return - 1;
            }
        }
        else
        {
            std::cout << "ERRO : Movimento nao permitido" << std::endl;
            tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({
                (float)(peça_pos.x * tamanho_casas + (tamanho_casas-60)/2), 
                (float)(peça_pos.y * tamanho_casas + (tamanho_casas-60)/2)
             });            
             return - 1;
        }
        return 0;
    }
    return -1;
}
Peça* (*GameController::getTabuleiro())[8] { return tabuleiro.getTabuleiro(); }