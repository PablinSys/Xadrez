#include "../include/gameController.hpp"
#include <iostream>
#include <cassert>

GameController::GameController(Tabuleiro& tabuleiro, const float& tamanho)
    : tabuleiro(tabuleiro), tamanho_casas(tamanho)
{
    posReiJogador = sf::Vector2i(4, 7);
    posReiOponente = sf::Vector2i(4, 0);
}
bool GameController::analisarCheckmate(const bool& isJogador)
{
    int countChecks = 0;
    int countProteçao = 0;
    if (isJogador)
    {
        Rei* rei = dynamic_cast<Rei*>(&*tabuleiro.getTabuleiro()[posReiJogador.y][posReiJogador.x]);
        int index_x = posReiJogador.x, index_y = posReiJogador.y;
        int init_x = index_x-1, init_y = index_y-1;
        for (int y = 0; y < 3; y++)
            for (int x = 0; x < 3; x++)
            {
                if (init_x + x < 8 && init_y + y < 8 && init_x + x >= 0 && init_y + y >= 0)
                {
                    //std::cout << "Pos Possible rei(jogador) : " << init_x + x << " " << init_y + y << std::endl;
                    if (tabuleiro.getTabuleiro()[init_y + y][init_x + x] == nullptr)
                    {
                        rei->positionIndex = {init_x + x, init_y + y};
                        if (rei->isProtegido(&tabuleiro, true))
                        {
                            //gameUI->PrintMessage("YOU WIN!");
                            return false;
                        }
                        else
                            countChecks++;
                    }
                    else if (tabuleiro.getTabuleiro()[init_y + y][init_x + x]->isWhite != rei->isWhite ||
                            typeid(*tabuleiro.getTabuleiro()[init_y + y][init_x + x]) == typeid(Rei))
                    {
                        rei->positionIndex = {init_x + x, init_y + y};
                        if (rei->isProtegido(&tabuleiro, true))
                        {
                            //gameUI->PrintMessage("YOU WIN!");
                            return false;
                        }
                        else
                            countChecks++;
                    }
                    else 
                        countProteçao++;
                }
            }
    }
    else
    {
        Rei* rei = dynamic_cast<Rei*>(&*tabuleiro.getTabuleiro()[posReiOponente.y][posReiOponente.x]);
        int index_x = posReiOponente.x, index_y = posReiOponente.y;
        int init_x = index_x-1, init_y = index_y-1;
        for (int y = 0; y < 3; y++)
            for (int x = 0; x < 3; x++)
            {
                if (init_x + x < 8 && init_y + y < 8 && init_x + x >= 0 && init_y + y >= 0)
                {
                    std::cout << "Pos Possible rei(oponente) : " << init_x + x << " " << init_y + y << std::endl;
                    
                    if (tabuleiro.getTabuleiro()[init_y + y][init_x + x] == nullptr)
                    {
                        rei->positionIndex = {init_x + x, init_y + y};
                        if (rei->isProtegido(&tabuleiro, false))
                        {
                            //gameUI->PrintMessage("YOU WIN!");
                            return false;
                        }
                        else
                            countChecks++;
                    }
                    else if (tabuleiro.getTabuleiro()[init_y + y][init_x + x]->isWhite != rei->isWhite ||
                            typeid(*tabuleiro.getTabuleiro()[init_y + y][init_x + x]) == typeid(Rei))
                    {
                        std::cout << "Posição de analise : " << init_x + x << " " << init_y + y << " , tipo : " << (typeid(*tabuleiro.getTabuleiro()[init_y + y][init_x + x]) == typeid(Rei) ? "Rei" : "Outra") << std::endl;
                        rei->positionIndex = {init_x + x, init_y + y};
                        if (rei->isProtegido(&tabuleiro, false))
                        {
                            //gameUI->PrintMessage("YOU WIN!");
                            return false;
                        }
                        else
                            countChecks++;
                    }
                    else 
                        countProteçao++;
                }
            }
    }
    if (countChecks > 1)
        return true; 
    else if (countChecks == 1 && countProteçao == 4)
        return true;
    return false;

}
bool GameController::analisarCheck(const bool& isWhite)
{
    if (isWhite == tabuleiro.brancasPrimeiro)
    {
        Rei* rei = dynamic_cast<Rei*>(&*tabuleiro.getTabuleiro()[posReiOponente.y][posReiOponente.x]);
        if (!rei->isProtegido(&tabuleiro, false))
            return true;
        else 
            return false;
    }
    else
    {
        Rei* rei = dynamic_cast<Rei*>(&*tabuleiro.getTabuleiro()[posReiJogador.y][posReiJogador.x]);
        if (!rei->isProtegido(&tabuleiro, true))
            return true;
        else 
            return false;
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
                if (tab[index_y][index_x]->isWhite == tab[peça_pos.y][peça_pos.x]->isWhite)
                {
                    tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({
                        (float)(peça_pos.x * tamanho_casas + (tamanho_casas-60)/2), 
                        (float)(peça_pos.y * tamanho_casas + (tamanho_casas-60)/2)
                    });
                    return - 1;
                }
            }
            if (Rei* rei = dynamic_cast<Rei*>(tab[posReiJogador.y][posReiJogador.x]))
            {
                if (!rei->isProtegido(&tabuleiro, true))
                {
                    tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({
                        (float)(peça_pos.x * tamanho_casas + (tamanho_casas-60)/2), 
                        (float)(peça_pos.y * tamanho_casas + (tamanho_casas-60)/2)
                    });
                    return - 1;
                }
            }

            if (typeid(*tab[peça_pos.y][peça_pos.x]) == typeid(Rei))
                if (tab[peça_pos.y][peça_pos.x]->isWhite == tabuleiro.brancasPrimeiro)
                    posReiJogador = {index_x, index_y};
                else 
                    posReiOponente = {index_x, index_y};

            this->tabuleiro.moverPeça(peça_pos, {index_x, index_y});
        }
        else
        {
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