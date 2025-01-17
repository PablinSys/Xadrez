#include "../include/bot.hpp"
#include <iostream>

Bot::Bot(Game& game) : game(game), algoritmo(game.gameController){}

/**
 * @brief Função para animar a movimentação da peça
 * 
 * @param pos_peça Posição da peça posicionada
 * @param new_pos Nova posição da peça
 */
void Bot::Animaçao(sf::Vector2i pos_peça, sf::Vector2i new_pos)
{
    Peça* peça = game.gameController->tabuleiro->getTabuleiro()[new_pos.y][new_pos.x];
    int sleep_time_ms = 10; int seconds = 1;
    float repeats = (seconds*1000)/sleep_time_ms;

    float tamanho = game.tabuleiro->tamanho_casas;
    float pos_x = pos_peça.x * tamanho + (tamanho-60)/2,
            pos_y = pos_peça.y * tamanho + (tamanho-60)/2;
    float new_pos_x = peça->objectUI.getPosition().x, 
            new_pos_y = peça->objectUI.getPosition().y;
    
    float x = (new_pos_x - pos_x); float y = (new_pos_y - pos_y);

    for (int i = 0; i < repeats; i++)
    {
        pos_x += x/repeats, pos_y += y/repeats;
        peça->objectUI.setPosition(pos_x, pos_y);
        game.gameUI->update(game.gameController);
        sf::sleep(sf::milliseconds(sleep_time_ms));
    }
}

/**
 * @brief Função que permite o bot(IA) movimentar peças
 * 
 * A função pega o melhor movimento e o executa
 * 
 */
void Bot::jogar()
{
    Jogada movimento = analisarMelhorMovimento(game.gameController->tabuleiro, game.gameController->jogador_jogada);    
    float tamanho = game.tabuleiro->tamanho_casas;

    if (!game.gameController->moverPeça(movimento.peça_pos, sf::Vector2f(movimento.new_pos.x * tamanho + (tamanho-60)/2, movimento.new_pos.y * tamanho + (tamanho-60)/2), false))
        jogar();
    else 
        Animaçao(movimento.peça_pos, movimento.new_pos);
}

// TODO: Melhorar o uso do cache 
/**
 * @brief Função que retorna o melhor movimento para o bot(IA)
 * 
 * A função retorna o melhor movimento para o bot(IA) usando o algoritmo minimax ou usando o cache 
 * gerado pelo Algoritmo com os possíveis movimentos do jogador e os respectivos possíveis movimentos contra.
 * Caso não encontre nenhum movimento, ele retorna um movimento avaliado com a heuristica
 * 
 * @param tabuleiro Tabuleiro atual
 * @param jogador_movimento Jogada anterior do jogador(humano)
 * 
 * @return Jogada Melhor movimento
 * 
 */
Jogada Bot::analisarMelhorMovimento(Tabuleiro* tabuleiro, const Jogada& jogador_movimento)
{
    Jogada melhorMovimento;
    float melhorPontuacao = -999, points;
    int max_depth = 2;

    // Verificando se o cache contém o movimento do jogador armazenado
    auto it_key = algoritmo.cache.find(jogador_movimento.new_pos);
    if (it_key != algoritmo.cache.end())
    {
        auto jogadas = it_key->second;

        algoritmo.Esvaziar_cache();
        for (const auto& [jogada, pontuacao] : jogadas)
        {
            // Verificando se a peça da jogada é valida
            if (tabuleiro->getTabuleiro()[jogada.peça_pos.y][jogada.peça_pos.x] != nullptr)
            {
                // Testando o movimento
                Tabuleiro test_tab = *tabuleiro;
                test_tab.moverPeça(jogada.peça_pos, jogada.new_pos);

                // Calculando a pontuação do movimento
                points = algoritmo.minimax(tabuleiro, jogada.peça_pos, false, -999, 999, max_depth, true);

                // Verificando se o movimento é melhor
                if (points > melhorPontuacao)
                {
                    melhorPontuacao = points;
                    melhorMovimento = jogada;
                }
            }
        }
    }
    else 
    {
        algoritmo.Esvaziar_cache();
        std::vector<Jogada> movimentos = GameController::getPossiveisMovimentos(*tabuleiro, !tabuleiro->brancasPrimeiro);
        for (Jogada& movimento : movimentos)
        {
            // Testando o movimento
            Tabuleiro test_tab = *tabuleiro;
            test_tab.moverPeça(movimento.peça_pos, movimento.new_pos);

            // Calculando a pontuação do movimento
            points = algoritmo.minimax(tabuleiro, movimento.new_pos, false, -999, 999, max_depth, true);

            // Verificando se o movimento é melhor
            if (points > melhorPontuacao)
            {
                melhorPontuacao = points;
                melhorMovimento = movimento;
            }
        }
    }
    // Verificando se o movimento fornecido pelo bot(IA) é invalido
    if (tabuleiro->getTabuleiro()[melhorMovimento.peça_pos.y][melhorMovimento.peça_pos.x] == nullptr || melhorMovimento.new_pos == melhorMovimento.peça_pos)
    {
        for (Jogada& movimento : GameController::getPossiveisMovimentos(*tabuleiro, !tabuleiro->brancasPrimeiro))
        {
            // Testando o movimento
            Tabuleiro test_tab = *tabuleiro;
            test_tab.moverPeça(movimento.peça_pos, movimento.new_pos);

            // Calculando a pontuação do movimento usando a heuristica sem analise profundas
            points = Algoritmo::avaliacao(&test_tab, movimento.new_pos, false, 4, false);

            // Verificando se o movimento é melhor
            if (points > melhorPontuacao)
            {
                melhorPontuacao = points;
                melhorMovimento = movimento;
            }
        }
    }
    return melhorMovimento;
}