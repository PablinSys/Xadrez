#include "../include/gameController.hpp"

GameController::GameController(Tabuleiro& tabuleiro, const int& tamanho)
    : tabuleiro(tabuleiro), tamanho_casas(tamanho) {}
    
void GameController::moverPeça(sf::Vector2i peça_pos, sf::Vector2f new_pos, bool emMovimento)
{
    if (tabuleiro.getTabuleiro()[peça_pos.y][peça_pos.x] != nullptr)
    {
        Peça* (*tab)[8] = tabuleiro.getTabuleiro();
        if (emMovimento)
            tab[peça_pos.y][peça_pos.x]->positionUI = {new_pos.x-60, new_pos.y-60};

        else if (tab[peça_pos.y][peça_pos.x]->analisarMovimento({(int)new_pos.x, (int)new_pos.y}))
        {
            if (tab[(int)(new_pos.y / tamanho_casas)][(int)(new_pos.x / tamanho_casas)] != nullptr)
                if (tab[(int)(new_pos.y / tamanho_casas)][(int)(new_pos.x / tamanho_casas)]->isWhite == tab[peça_pos.y][peça_pos.x]->isWhite)
                {
                    tab[peça_pos.y][peça_pos.x]->positionUI = {(float)(peça_pos.x * tamanho_casas - tamanho_casas/2), (float)(peça_pos.y * tamanho_casas - tamanho_casas/8)};
                    return tabuleiro.setTabuleiro(tab);
                }

            Peça* peça = tab[peça_pos.y][peça_pos.x];
            tamanho_casas = tabuleiro.tamanho_casas;

            if (Peao* peao = dynamic_cast<Peao*>(peça))
                peao->primeiroLance = false;

            peça->positionUI = 
            {
                (float)((int)(new_pos.x / tamanho_casas)) * tamanho_casas - tamanho_casas/2 , 
                (float)((int)(new_pos.y / tamanho_casas)) * tamanho_casas - tamanho_casas/8
            };
            peça->positionIndex = {(int)(new_pos.x / tamanho_casas), (int)(new_pos.y / tamanho_casas)};
            
            tab[(int)(new_pos.y / tamanho_casas)][(int)(new_pos.x / tamanho_casas)] = &*peça;
            tab[peça_pos.y][peça_pos.x] = nullptr;
        }
        else
            tab[peça_pos.y][peça_pos.x]->positionUI = {(float)(peça_pos.x * tamanho_casas - tamanho_casas/2), (float)(peça_pos.y * tamanho_casas - tamanho_casas/8)};
        return tabuleiro.setTabuleiro(tab);
    }
}
bool GameController::analisarProtecaoRei(const sf::Vector2i& posRei)
{
    return true;
}