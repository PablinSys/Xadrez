#include "../include/algoritmo.hpp"
#include <iostream>

/**
 * @brief Algoritmo minimax com alpha-beta pruning
 * 
 * Minimax é uma busca em profundidade que considera a maximização 
 * como ganho mínimo (Jogador) e minimização como perda mínima (Computador)  
 * 
 * @param tabuleiro Tabuleiro atual
 * @param pos_peça Posição da peça posicionada
 * @param maximizando True se maximizando (Jogador), False se minimizando (Computador)
 * @param alpha Alpha
 * @param beta Beta
 * @param profundidade Profundidade da busca
 * @param usageCache True para atualizar o cache com os possíveis movimentos do oponente
 * @return float Pontuação
 */
float Algoritmo::minimax(Tabuleiro* tabuleiro, const sf::Vector2i& pos_peça, bool maximizando, float alpha, float beta, int profundidade, bool usageCache)
{
    // Verificando se o oponente não tem movimentos possiveis e retornando uma pontuação boa
    if (GameController::getPossiveisMovimentos(*tabuleiro, maximizando ? tabuleiro->brancasPrimeiro : !tabuleiro->brancasPrimeiro).size() == 0)
        return 999;
    // Se a profundidade for 0, retorna a avaliação da posição da peça
    else if (profundidade == 0)
        return Algoritmo::avaliacao(tabuleiro, pos_peça, (maximizando ? false : true), 4, false);

    // Procura todos os melhores movimentos possiveis 
    if (maximizando)
    {
        float maxPoint = -999;
        // Percorrendo todos os movimentos possiveis da (IA)
        for (Jogada& movimento : GameController::getPossiveisMovimentos(*tabuleiro, !tabuleiro->brancasPrimeiro))
        {
            // Simulando o movimento
            Tabuleiro test_tab = *tabuleiro;
            test_tab.moverPeça(movimento.peça_pos, movimento.new_pos);

            // Avaliando o movimento com chamada recursiva
            float point = minimax(&test_tab, movimento.new_pos, false, alpha, beta, profundidade-1, false);
            maxPoint = std::max(maxPoint, point);
            alpha = std::max(alpha, point);

            // Atualizando o cache para possivel uso 
            if (usageCache) 
                cache[pos_peça][movimento] = point;

            // Cortando o algoritmo se beta for menor ou igual a alpha
            if (beta <= alpha)
                break;
        }
        return maxPoint;
    }
    // Procura todos os piores movimentos possiveis
    else 
    {
        float minPoint = 999;  
        // Percorrendo todos os movimentos possiveis do jogador
        for (Jogada& movimento : GameController::getPossiveisMovimentos(*tabuleiro, tabuleiro->brancasPrimeiro))
        {
            // Simulando o movimento
            Tabuleiro test_tab = *tabuleiro;
            test_tab.moverPeça(movimento.peça_pos, movimento.new_pos);

            // Avaliando o movimento com chamada recursiva
            float point = minimax(&test_tab, movimento.new_pos, true, alpha, beta, profundidade-1, usageCache);
            minPoint = std::min(minPoint, point);
            beta = std::min(beta, point);

            // Cortando o algoritmo se beta for menor ou igual a alpha
            if (beta <= alpha)
                break;
        }
        return minPoint;
    }
}

// TODO: Melhorar a avaliação com diferentes tipos de heurísticas. Utilizar os niveis para filtração de movimentos
/**
 * @brief Avalia a posição da peça no tabuleiro
 * 
 * Avalia a peça no tabuleiro de acordo com o nível de avaliação 
 *  Nivel 0 : 
 *    - Contagem de peças e seus movimentos
 *  Nivel 1 :
 *    - Pontua as posições de determinadas peças
 * 
 * @param tabuleiro Tabuleiro atual
 * @param pos_peça Posição da peça posicionada
 * @param isJogador True se for o jogador, False se for o oponente(IA)
 * @param nivel Nível de avaliação
 * @return float Pontuação
 * 
 */
float Algoritmo::avaliacao(Tabuleiro* tabuleiro, sf::Vector2i pos_peça, bool isJogador, int nivel, bool debug)
{
    // Verificando se a posição da peça é valida
    if ((pos_peça.x < 0 || pos_peça.x > 7) || (pos_peça.y < 0 || pos_peça.y > 7))
        return -99;
    // Verificando se o tabuleiro nao esta vazio
    if (tabuleiro == nullptr || tabuleiro->getTabuleiro().empty())
        return -99;

    std::vector<std::vector<Peça*>> tab = tabuleiro->getTabuleiro();
    float point = 0;
    if (nivel >= 0)
    {
        // Variavel responsável pela diferenca de pontuação entre as peças
        int diff =  (isJogador ? 1 : -1);
        // Percorrendo todas as peças
        for (const std::vector<Peça*>& linha : tab)
            for (Peça* peça : linha)
                // Verificando se a peça não é uma casa vazia   
                if (peça != nullptr)
                {
                    // Contagem de peças e seus movimentos

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
        // Avaliando a posição de especificas peças no tabuleiro
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
        else if (dynamic_cast<Cavalo*>(peça))
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
