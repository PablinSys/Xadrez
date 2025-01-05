#include "../include/gameController.hpp"
#include <iostream>
#include <cassert>

GameController::GameController(Tabuleiro& tabuleiro, const int& tamanho)
    : tabuleiro(tabuleiro), tamanho_casas(tamanho) {}
    
int GameController::moverPeça(sf::Vector2i peça_pos, sf::Vector2f new_pos, bool emMovimento)
{
    if (tabuleiro.getTabuleiro()[peça_pos.y][peça_pos.x] != nullptr)
    {
        int points = 0;
        Tabuleiro* tabuleiro = &(this->tabuleiro);
        Peça* (*tab)[8] = tabuleiro->getTabuleiro();
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
                    return 0;
                }
                else if (typeid(*tab[index_y][index_x]) == typeid(Peao))
                    points += 1;
                else if (typeid(*tab[index_y][index_x]) == typeid(Torre))
                    points += 5;
                else if (typeid(*tab[index_y][index_x]) == typeid(Cavalo))
                    points += 3;
                else if (typeid(*tab[index_y][index_x]) == typeid(Bispo))
                    points += 3;
                else if (typeid(*tab[index_y][index_x]) == typeid(Rainha))
                    points += 9;
                else if (typeid(*tab[index_y][index_x]) == typeid(Rei))
                    points += 100;
            }

            Peça* peça = &*tab[peça_pos.y][peça_pos.x];
            tamanho_casas = tabuleiro->tamanho_casas;

            if (Peao* peao = dynamic_cast<Peao*>(peça))
                peao->primeiroLance = false;

            peça->objectUI.setPosition( 
            {
                (float)index_x * tamanho_casas + (tamanho_casas-60)/2 , 
                (float)index_y * tamanho_casas + (tamanho_casas-60)/2
            });
            peça->positionIndex = {index_x, index_y};

            tab[index_y][index_x] = &*peça;
            tab[peça_pos.y][peça_pos.x] = nullptr;
            assert(tab[peça_pos.y][peça_pos.x] == nullptr);
        }
        else
        {
            tab[peça_pos.y][peça_pos.x]->objectUI.setPosition({
                (float)(peça_pos.x * tamanho_casas + (tamanho_casas-60)/2), 
                (float)(peça_pos.y * tamanho_casas + (tamanho_casas-60)/2)
             });            
             return 0;
        }
        //tabuleiro.setTabuleiro(tab);
        return points;
    }
}
Peça* (*GameController::getTabuleiro())[8] { return tabuleiro.getTabuleiro(); }
bool GameController::analisarProtecaoRei(const sf::Vector2i& posRei)
{
    return true;
}