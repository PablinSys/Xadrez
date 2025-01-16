#include "../include/bot.hpp"
#include <iostream>

Bot::Bot(Game& game) : game(game), algoritmo(game.gameController)
{
    jogadas = 0;
}

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
void Bot::jogar()
{
    Jogada movimento = analisarMelhorMovimento(game.gameController->tabuleiro, game.gameController->jogador_jogada);    
    float tamanho = game.tabuleiro->tamanho_casas;

    if (!game.gameController->moverPeça(movimento.peça_pos, sf::Vector2f(movimento.new_pos.x * tamanho + (tamanho-60)/2, movimento.new_pos.y * tamanho + (tamanho-60)/2), false))
        jogar();
    else 
        Animaçao(movimento.peça_pos, movimento.new_pos);
}

Jogada Bot::analisarMelhorMovimento(Tabuleiro* tabuleiro, const Jogada& jogador_movimento)
{
    Jogada melhorMovimento;
    float melhorPontuacao = -999;
    int max_depth = 2;

    float points;
    auto it_key = algoritmo.cache.find(jogador_movimento.new_pos);
    if (it_key != algoritmo.cache.end())
    {
        auto jogadas = it_key->second;

        algoritmo.Esvaziar_cache();
        for (const auto& [jogada, pontuacao] : jogadas)
        {
            if (tabuleiro->getTabuleiro()[jogada.peça_pos.y][jogada.peça_pos.x] != nullptr)
            {
                Tabuleiro test_tab = *tabuleiro;
                test_tab.moverPeça(jogada.peça_pos, jogada.new_pos);
                points = algoritmo.minimax(tabuleiro, jogada.peça_pos, false, -999, 999, max_depth, true);
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
            Tabuleiro test_tab = *tabuleiro;
            test_tab.moverPeça(movimento.peça_pos, movimento.new_pos);
            points = algoritmo.minimax(tabuleiro, movimento.new_pos, false, -999, 999, max_depth, true);
            if (points > melhorPontuacao)
            {
                melhorPontuacao = points;
                melhorMovimento = movimento;
            }
        }
    }
    // Debugging the cache content
    if (tabuleiro->getTabuleiro()[melhorMovimento.peça_pos.y][melhorMovimento.peça_pos.x] == nullptr || melhorMovimento.new_pos == melhorMovimento.peça_pos)
    {
        for (Jogada& movimento : GameController::getPossiveisMovimentos(*tabuleiro, !tabuleiro->brancasPrimeiro))
        {
            Tabuleiro test_tab = *tabuleiro;
            test_tab.moverPeça(movimento.peça_pos, movimento.new_pos);
            points = Algoritmo::avaliacao(&test_tab, movimento.new_pos, false, 4, false);
            if (points > melhorPontuacao)
            {
                melhorPontuacao = points;
                melhorMovimento = movimento;
            }
        }
    }
    return melhorMovimento;
}