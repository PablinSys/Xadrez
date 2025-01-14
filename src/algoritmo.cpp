#include "../include/algoritmo.hpp"
#include <iostream>

float Algoritmo::minimax(Tabuleiro* tabuleiro, const sf::Vector2i& pos_peça, const sf::Vector2i& new_pos, bool maximizando, float alpha, float beta, int profundidade, int nivel)
{
    //sf::sleep(sf::milliseconds(1000));
    // std::cout << std::boolalpha << "Minimax called with params: "
    //           << "maximizando=" << maximizando << ", "
    //           << "alpha=" << alpha << ", "
    //           << "beta=" << beta << ", "
    //           << "profundidade=" << profundidade << std::endl;

    std::unique_ptr<GameController> gameC = std::make_unique<GameController>(
        *tabuleiro, tabuleiro->tamanho_casas,
        Tabuleiro::getReiPosition(tabuleiro, tabuleiro->brancasPrimeiro),
        Tabuleiro::getReiPosition(tabuleiro, !tabuleiro->brancasPrimeiro)
    );
    if (profundidade == 0)
    {
        float points = Algoritmo::avaliacao(tabuleiro, pos_peça, new_pos, (maximizando ? false : true), 4, false);
        points *= maximizando ? 1 : -1;
        //std::cout << "Para " << (maximizando ? "pretas" : "brancas") << " | pontos: " << points << std::endl;
        //system("clear");
        // std::cout << "Peça: " << typeid(*tabuleiro->getTabuleiro()[pos_peça.y][pos_peça.x]).name() << " (pos: {" << pos_peça.x << "," << pos_peça.y << "}) para " 
        //         << (tabuleiro->getTabuleiro()[new_pos.y][new_pos.x] != nullptr 
        //             ? std::string("atacar a peça: ") + typeid(*tabuleiro->getTabuleiro()[new_pos.y][new_pos.x]).name() + " (pos: {" + std::to_string(new_pos.x) + "," + std::to_string(new_pos.y) + "})"
        //             : std::string("posicionar na casa: {") + std::to_string(new_pos.x) + "," + std::to_string(new_pos.y) + "}")
        //         << std::endl;
        return points;
    }
    if (maximizando)
    {
        if (gameC->analisarCheck(!tabuleiro->brancasPrimeiro))
        {
            if (gameC->analisarCheckmate(tabuleiro->brancasPrimeiro))
                return -999;
            return -12;
        }
    }
    else
    {
        if (gameC->analisarCheck(tabuleiro->brancasPrimeiro))
        {
            if (gameC->analisarCheckmate(!tabuleiro->brancasPrimeiro))
                return 999;
            return 12;
        }
    }
    /*
    if (GameController::isEmpate())
        return 0;
    */
    if (maximizando)
    {
        float maxPoint = -999;
        Tabuleiro test_tab = *tabuleiro;
        test_tab.moverPeça(pos_peça, new_pos);
        for (Jogada& movimento : GameController::getPossiveisMovimentos(test_tab, tabuleiro->brancasPrimeiro, true, false, 1))
        {
            float point = minimax(&test_tab, movimento.peça_pos, movimento.new_pos, false, alpha, beta, profundidade-1, nivel);
            maxPoint = std::max(maxPoint, point);
            alpha = std::max(alpha, point);
            if (beta <= alpha)
            {
                break;
            }
        }
        return maxPoint;
    }
    else 
    {
        float minPoint = 999;    
        Tabuleiro test_tab = *tabuleiro;
        test_tab.moverPeça(pos_peça, new_pos);
        Jogada brancasJogada = Jogada(pos_peça, new_pos);     
        for (Jogada& movimento : GameController::getPossiveisMovimentos(test_tab, !tabuleiro->brancasPrimeiro, true, false, 1))
        {
            float point = minimax(&test_tab, movimento.peça_pos, movimento.new_pos, true, alpha, beta, profundidade-1, nivel);
            minPoint = std::min(minPoint, point);
            beta = std::min(beta, point);
            if (beta <= alpha)
            {
                break;
            }
            else 
            {
                cache[brancasJogada].insert(std::pair<Jogada, float>(movimento, point));
            }
        }
        return minPoint;
    }
}

float Algoritmo::avaliacao(Tabuleiro* tabuleiro, sf::Vector2i pos_peça, sf::Vector2i new_pos, bool isJogador, int nivel, bool debug)
{
    if ((pos_peça.x < 0 || pos_peça.x > 7) || (pos_peça.y < 0 || pos_peça.y > 7) || 
        (new_pos.x < 0 || new_pos.x > 7) || (new_pos.y < 0 || new_pos.y > 7))
        return -99;
    if (tabuleiro == nullptr || tabuleiro->getTabuleiro().empty())
        return -99;

    std::vector<std::vector<Peça*>> tab = tabuleiro->getTabuleiro();
    float point = 0;
    if (nivel >= 0)
    {
        if (tab[new_pos.y][new_pos.x] != nullptr)
        {
            Peça* peça = tab[new_pos.y][new_pos.x];
            if (peça->isWhite != tabuleiro->brancasPrimeiro)
            {
                if (dynamic_cast<Peao*>(peça)) {
                    point += 1;
                } else if (dynamic_cast<Torre*>(peça)) {
                    point += 5;
                } else if (dynamic_cast<Cavalo*>(peça)) {
                    point += 3;
                } else if (dynamic_cast<Bispo*>(peça)) {
                    point += 3;
                } else if (dynamic_cast<Rainha*>(peça)) {
                    point += 9;
                } else if (dynamic_cast<Rei*>(peça)) {
                    point += 90;
                } else {
                    point += 0;
                }
                if (debug)
                    std::cout << "Nivel 0 -> " << point << 
                            "Peça Comida: " << typeid(*peça).name() << std::endl; 
            }
            else 
            {
                if (debug)
                    std::cout << "Nivel 0 -> " << point
                              << "Peça Protegida: " << typeid(*peça).name() << std::endl;
                point += 0.25;
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
                {
                    if (peça->positionIndex.y < 5)
                    {
                        point += 0.5;
                        if (debug)
                            std::cout << "Nivel 1 -> " << point << 
                                        "Peão Branco na Coluna 4 ou 5" << std::endl;
                    }
                    else if (peça->positionIndex.x == 0 || peça->positionIndex.x == 7)
                    {
                        point -= 0.5;
                        if (debug)
                            std::cout << "Nivel 1 -> " << point << 
                                        "Peão Branco na Coluna 0 ou 7" << std::endl;
                    }
                }
                else if (peça->positionIndex.y > 3)
                {
                    point += 0.5;
                    if (debug)
                        std::cout << "Nivel 1 -> " << point << 
                                    "Peão Preto na Coluna 4 ou 5" << std::endl;
                }
                else if (peça->positionIndex.x == 0 || peça->positionIndex.x == 7)
                {
                    point -= 0.5;
                    if (debug)
                        std::cout << "Nivel 1 -> " << point << 
                                    "Peão Preto na Coluna 0 ou 7" << std::endl;
                }
            }
        }
        else if (Bispo* bispo = dynamic_cast<Bispo*>(peça)) {
            float bispoPoints = bispo->movimentosPossiveis(tabuleiro).size() * 0.15;
            point += bispoPoints;
            if (debug)
                std::cout << "Nivel 1 -> " << point << " Bispo Movimentos: " << bispoPoints << std::endl;
        } else if (Torre* torre = dynamic_cast<Torre*>(peça)) {
            float torrePoints = torre->movimentosPossiveis(tabuleiro).size() * 0.05;
            point += torrePoints;
            if (debug)
                std::cout << "Nivel 1 -> " << point << " Torre Movimentos: " << torrePoints << std::endl;
        } else if (dynamic_cast<Cavalo*>(peça))
        {
            if (isJogador)
            {
                if (peça->positionIndex.y < 4)
                {
                    point += 0.25;
                    if (debug)
                        std::cout << "Nivel 1 -> " << point << " Cavalo Branco avançado" << std::endl;
                }
                else if (peça->positionIndex.x == 0 || peça->positionIndex.x == 7)
                {
                    point -= 0.5;
                    if (debug)
                        std::cout << "Nivel 1 -> " << point << " Cavalo Branco nos cantos" << std::endl;
                }
            }
            else 
            {
                if (peça->positionIndex.y > 3)
                {
                    point += 0.25;
                    if (debug)
                        std::cout << "Nivel 1 -> " << point << " Cavalo Preto avançado" << std::endl;
                }
                else if (peça->positionIndex.x == 0 || peça->positionIndex.x == 7)
                {
                    point -= 0.5;
                    if (debug)
                        std::cout << "Nivel 1 -> " << point << " Cavalo Preto nos cantos" << std::endl;
                }
            }
        }
        
    }
    if (nivel >= 2)
    {
        std::vector<Jogada> movimentos_brancas;
        std::vector<Jogada> movimentos_pretas;
        if (isJogador || nivel >= 2) 
        {
            const std::vector<Jogada>& movimentos_brancas_tmp = GameController::getPossiveisMovimentos(*tabuleiro, tabuleiro->brancasPrimeiro);
            movimentos_brancas = movimentos_brancas_tmp;
            if (isJogador) 
                point += movimentos_brancas.size() * 0.025;
            if (debug)
                std::cout << "Nivel 2 -> " << point << " Movimentos Brancas: " << movimentos_brancas.size() << std::endl;
        }
        if (!isJogador || nivel >= 2) 
        {
            const std::vector<Jogada>& movimentos_pretas_tmp = GameController::getPossiveisMovimentos(*tabuleiro, !tabuleiro->brancasPrimeiro, true, true, 2);
            movimentos_pretas = movimentos_pretas_tmp;
            if (!isJogador) 
                point += movimentos_pretas.size() * 0.025;
            if (debug)
                std::cout << "Nivel 2 -> " << point << " Movimentos Pretas: " << movimentos_pretas.size() << std::endl;
        }
        if (nivel >= 3)
        {
            float avaliacaoBranca;
            float avaliacaoPreta;
            for (const auto& movimento_branca : movimentos_brancas) {
                avaliacaoBranca = Algoritmo::avaliacao(tabuleiro, movimento_branca.peça_pos, movimento_branca.new_pos, isJogador, 1, false);
                point += isJogador ? avaliacaoBranca : (-1)*avaliacaoBranca;
            }
            if (debug)
                std::cout << "Nivel 3 -> Movimento Branca Avaliacao: " << avaliacaoBranca << std::endl;
            for (const auto& movimento_preta : movimentos_pretas) {
                avaliacaoPreta = Algoritmo::avaliacao(tabuleiro, movimento_preta.peça_pos, movimento_preta.new_pos, !isJogador, 1, false);
                point += !isJogador ? avaliacaoPreta : (-1)*avaliacaoPreta;
            }
            if (debug)
                std::cout << "Nivel 3 -> Movimento Preta Avaliacao: " << avaliacaoPreta << std::endl;
        }
    }
    // if (nivel >= 4)
    // {
    //     Tabuleiro board_tmp = *tabuleiro;
    //     board_tmp.moverPeça(pos_peça, new_pos);
    //     sf::Vector2i rei_pos = board_tmp.getReiPosition(&board_tmp, isJogador ? !tabuleiro->brancasPrimeiro : tabuleiro->brancasPrimeiro);
    //     if (Rei* rei = dynamic_cast<Rei*>(board_tmp.getTabuleiro()[rei_pos.y][rei_pos.x]))
    //         if (GameController::getPossiveisMovimentos(board_tmp, isJogador ? !tabuleiro->brancasPrimeiro : tabuleiro->brancasPrimeiro).size() == 0)
    //             point += 999;
    // }
    if (debug)
        std::cout << "Pontos Totais: " << point << std::endl;
    return point;
}
