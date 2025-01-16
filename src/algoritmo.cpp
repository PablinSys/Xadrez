#include "../include/algoritmo.hpp"
#include <iostream>

float Algoritmo::minimax(Tabuleiro* tabuleiro, const sf::Vector2i& pos_peça, bool maximizando, float alpha, float beta, int profundidade, bool usageCache)
{
    if (GameController::getPossiveisMovimentos(*tabuleiro, maximizando ? tabuleiro->brancasPrimeiro : !tabuleiro->brancasPrimeiro).size() == 0)
        return 999;
    else if (profundidade == 0)
        return Algoritmo::avaliacao(tabuleiro, pos_peça, (maximizando ? false : true), 4, false);

    if (maximizando)
    {
        float maxPoint = -999;
        for (Jogada& movimento : GameController::getPossiveisMovimentos(*tabuleiro, !tabuleiro->brancasPrimeiro))
        {
            Tabuleiro test_tab = *tabuleiro;
            test_tab.moverPeça(movimento.peça_pos, movimento.new_pos);
            float point = minimax(&test_tab, movimento.new_pos, false, alpha, beta, profundidade-1, false);
            maxPoint = std::max(maxPoint, point);
            alpha = std::max(alpha, point);

            if (usageCache) 
                cache[pos_peça][movimento] = point;

            if (beta <= alpha)
                break;
        }
        return maxPoint;
    }
    else 
    {
        float minPoint = 999;  
        for (Jogada& movimento : GameController::getPossiveisMovimentos(*tabuleiro, tabuleiro->brancasPrimeiro))
        {
            Tabuleiro test_tab = *tabuleiro;
            test_tab.moverPeça(movimento.peça_pos, movimento.new_pos);

            float point = minimax(&test_tab, movimento.new_pos, true, alpha, beta, profundidade-1, usageCache);
            minPoint = std::min(minPoint, point);
            beta = std::min(beta, point);

            if (beta <= alpha)
                break;
        }
        return minPoint;
    }
}

float Algoritmo::avaliacao(Tabuleiro* tabuleiro, sf::Vector2i pos_peça, bool isJogador, int nivel, bool debug)
{
    if ((pos_peça.x < 0 || pos_peça.x > 7) || (pos_peça.y < 0 || pos_peça.y > 7))
        return -99;
    if (tabuleiro == nullptr || tabuleiro->getTabuleiro().empty())
        return -99;

    std::vector<std::vector<Peça*>> tab = tabuleiro->getTabuleiro();
    float point = 0;
    if (nivel >= 0)
    {
        int diff =  (isJogador ? 1 : -1);
        for (const std::vector<Peça*>& linha : tab)
            for (Peça* peça : linha)
                if (peça != nullptr)
                {
                    if (peça->isWhite == tabuleiro->brancasPrimeiro) 
                        point += peça->movimentosPossiveis(tabuleiro).size() * 0.05 * diff;
                    else 
                        point += peça->movimentosPossiveis(tabuleiro).size() * 0.05 * (diff * -1);

                    if (dynamic_cast<Peao*>(peça)){
                        if (peça->isWhite == tabuleiro->brancasPrimeiro)
                            point += 1 * diff;
                        else 
                            point -= 1 * diff;
                    }
                    else if (dynamic_cast<Torre*>(peça)){
                        if (peça->isWhite == tabuleiro->brancasPrimeiro)
                            point += 5 * diff;
                        else 
                            point -= 5 * diff;
                    }
                    else if (dynamic_cast<Cavalo*>(peça)){
                        if (peça->isWhite == tabuleiro->brancasPrimeiro)
                            point += 3 * diff;
                        else 
                            point -= 3 * diff;
                    }
                    else if (dynamic_cast<Bispo*>(peça)){
                        if (peça->isWhite == tabuleiro->brancasPrimeiro)
                            point += 3 * diff;
                        else 
                            point -= 3 * diff;
                    }
                    else if (dynamic_cast<Rainha*>(peça)){
                        if (peça->isWhite == tabuleiro->brancasPrimeiro)
                            point += 9 * diff;
                        else 
                            point -= 9 * diff;
                    }
                }
    }
    if (nivel >= 1)
    {
        Peça* peça = tab[pos_peça.y][pos_peça.x];
        if (dynamic_cast<Peao*>(peça))
        {
            if (peça->positionIndex.x == 3 || peça->positionIndex.x == 4)
            {
                if (isJogador)
                    if (peça->positionIndex.y < 5)
                    {
                        point += 0.5;
                        if (debug)
                            std::cout << "Nivel 1 -> " << 0.5 << 
                                        "Peão Branco na Coluna 4 ou 5" << std::endl;
                    }
                else if (peça->positionIndex.y > 3)
                {
                    point += 0.5;
                    if (debug)
                        std::cout << "Nivel 1 -> " << 0.5 << 
                            "Peão Preto na Coluna 4 ou 5" << std::endl;
                }
            }
            if (peça->positionIndex.x == 0 || peça->positionIndex.x == 7)
            {
                point -= 0.5;
                if (debug)
                    std::cout << "Nivel 1 -> " << -0.5 << 
                        "Peão Branco na Coluna 0 ou 7" << std::endl;
            }
        }
        else if (Bispo* bispo = dynamic_cast<Bispo*>(peça)) {
            float bispoPoints = bispo->movimentosPossiveis(tabuleiro).size() * 0.05;
            point += bispoPoints;
            if (debug)
                std::cout << "Nivel 1 -> " << bispoPoints << " Bispo Movimentos: " << bispoPoints << std::endl;
        } else if (Torre* torre = dynamic_cast<Torre*>(peça)) {
            float torrePoints = torre->movimentosPossiveis(tabuleiro).size() * 0.05;
            point += torrePoints;
            if (debug)
                std::cout << "Nivel 1 -> " << torrePoints << " Torre Movimentos: " << torrePoints << std::endl;
        } else if (dynamic_cast<Cavalo*>(peça))
        {
            if (isJogador)
            {
                if (peça->positionIndex.y < 5)
                {
                    point += 0.5;
                    if (debug)
                        std::cout << "Nivel 1 -> " << 0.5 << " Cavalo Branco avançado" << std::endl;
                }
                else if (peça->positionIndex.y == 5)
                {
                    point += 0.25;
                    if (debug)
                        std::cout << "Nivel 1 -> " << 0.25 << " Cavalo Branco no meio" << std::endl;
                }
            }
            else 
            {
                if (peça->positionIndex.y > 2)
                {
                    point += 0.25;
                    if (debug)
                        std::cout << "Nivel 1 -> " << point << " Cavalo Preto avançado" << std::endl;
                }
                else if (peça->positionIndex.y == 2)
                {
                    point += 0.25;
                    if (debug)
                        std::cout << "Nivel 1 -> " << point << " Cavalo Preto no meio" << std::endl;
                }
            }
            if (peça->positionIndex.x == 0 || peça->positionIndex.x == 7)
            {
                point -= 0.5;
                if (debug)
                    std::cout << "Nivel 1 -> " << -0.5 << " Cavalo Branco nos cantos" << std::endl;
            }
        }
        
    }
    if (debug)
        std::cout << "Pontos Totais: " << point << std::endl;
    return point;
}
