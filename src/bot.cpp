#include "../include/bot.hpp"
#include <iostream>

Bot::Bot(Game& game) : game(game), algoritmo(game.gameController)
{
    jogadas = 0;
}

void Bot::Animaçao(sf::Vector2i pos_peça, sf::Vector2i new_pos)
{
    Peça* peça = game.gameController->tabuleiro.getTabuleiro()[new_pos.y][new_pos.x];
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
    Jogada movimento = analisarMelhorMovimento(&game.gameController->tabuleiro, game.gameController->jogador_jogada);    
    float tamanho = game.tabuleiro->tamanho_casas;
    if (!game.gameController->moverPeça(movimento.peça_pos, sf::Vector2f(movimento.new_pos.x * tamanho + (tamanho-60)/2, movimento.new_pos.y * tamanho + (tamanho-60)/2), false))
        jogar();
    else 
    {
        Animaçao(movimento.peça_pos, movimento.new_pos);
        jogadas++;
    }
}

Jogada Bot::analisarMelhorMovimento(Tabuleiro* tabuleiro, const Jogada& jogador_movimento)
{
    Jogada melhorMovimento;
    float melhorPontuacao = -999;
    int max_depth = 2;

    float points;
    if (!algoritmo.cache.empty())
    {
        std::cout << "Usando cache" << std::endl;
        if (auto it_key = algoritmo.cache.find(jogador_movimento); it_key != algoritmo.cache.end())
        {
            auto jogadas = it_key->second;
            // Isso retorna o maior elemento
            auto it_max = std::max_element(jogadas.begin(), jogadas.end(), [](const std::pair<Jogada, float>& a, const std::pair<Jogada, float>& b) {
                return a.second < b.second;
            });
            melhorPontuacao = it_max->second;
            melhorMovimento = it_max->first;
            // Atualizando o cache
            algoritmo.Esvaziar_cache();
            algoritmo.minimax(tabuleiro, melhorMovimento.peça_pos, melhorMovimento.new_pos, false, -999, 999, max_depth, 1);
        }
    }
    else 
    {
        std::vector<Jogada> movimentos = GameController::getPossiveisMovimentos(*tabuleiro, !tabuleiro->brancasPrimeiro);
        for (Jogada& movimento : movimentos)
        {
            points = algoritmo.minimax(tabuleiro, movimento.peça_pos, movimento.new_pos, true, -999, 999, max_depth, 2);
            if (points > melhorPontuacao)
            {
                //std::cout << "Pontuacao do movimento: " << points << " || Movimento: (" << movimento.peça_pos.x << ", " << movimento.peça_pos.y << ") -> (" << movimento.new_pos.x << ", " << movimento.new_pos.y << ")" << std::endl;
                melhorPontuacao = points;
                melhorMovimento = movimento;
            }
        }
    }
    // Debugging the cache content

    std::string pieceType = typeid(*tabuleiro->getTabuleiro()[melhorMovimento.peça_pos.y][melhorMovimento.peça_pos.x]).name();
    std::cout << "Melhor movimento com " << melhorPontuacao << " pontos: " << pieceType << " em (" << melhorMovimento.peça_pos.x << ", " << melhorMovimento.peça_pos.y << ") -> (" << melhorMovimento.new_pos.x << ", " << melhorMovimento.new_pos.y << ")" << std::endl;
    Algoritmo::avaliacao(tabuleiro, melhorMovimento.peça_pos, melhorMovimento.new_pos, false, 4, true);
    return melhorMovimento;
}