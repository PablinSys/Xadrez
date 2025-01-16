#include "../include/game.hpp"
#include "../include/bot.hpp"
#include <iostream>

Game::Game(sf::RenderWindow* window) 
{
    this->window = window;
    float tamanho_casas = (window->getSize().x + window->getSize().y)/2/8;
    tabuleiro = new Tabuleiro(true, tamanho_casas);
    gameUI = new GameUI(*window, tabuleiro);
    gameController = new GameController(tabuleiro, gameUI->tamanho_casas);
}

void Game::run()
{
    bool vez_jogador = tabuleiro->brancasPrimeiro;
    bool vez_cpu = !tabuleiro->brancasPrimeiro;
    Bot CPU = Bot(*this);
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event) && vez_jogador)
        {
            if (event.type == sf::Event::Closed)
                window->close();
            else if (event.type == sf::Event::MouseButtonPressed)
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if (gameUI->OnMouseButtonLeftPressed(gameController))
                    {
                        gameUI->update(gameController);
                        if (auto qtd_movimentos = GameController::getPossiveisMovimentos(*gameController->tabuleiro, false).size(); qtd_movimentos < 12)
                            if (gameController->analisarCheck(true))
                            {
                                if (qtd_movimentos == 0)
                                {
                                    std::cout << "CHECKMATE!!!!\nVITORIA DO JOGADOR" << std::endl;
                                    break;
                                }
                                else 
                                    std::cout << "CHECK" << std::endl;
                            }
                        vez_jogador = !vez_jogador;
                        vez_cpu = !vez_cpu;
                    }
                }
        }
        if (!vez_jogador)
        {
            CPU.jogar();
            if (auto qtd_movimentos = GameController::getPossiveisMovimentos(*gameController->tabuleiro, true).size(); qtd_movimentos < 12)
                if (gameController->analisarCheck(false))
                {
                    if (qtd_movimentos == 0)
                    {
                        std::cout << "CHECKMATE??!!\nVITORIA DA IA" << std::endl;
                        break;
                    }
                    else 
                        std::cout << "VOCÊ LEVOU CHECK" << std::endl;
                }
            vez_jogador = !vez_jogador;
            vez_cpu = !vez_cpu;
        }
        gameUI->update(gameController);
        sf::sleep(sf::milliseconds(100));
    }
    window->close();
    //window->display();
    return;
}