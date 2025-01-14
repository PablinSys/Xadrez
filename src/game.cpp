#include "../include/game.hpp"
#include "../include/bot.hpp"
#include <iostream>

Game::Game(sf::RenderWindow* window) 
{
    std::cout << "Game constructor called" << std::endl;
    this->window = window;
    float tamanho_casas = (window->getSize().x + window->getSize().y)/2/8;
    tabuleiro = new Tabuleiro(true, tamanho_casas);
    std::cout << "Tabuleiro constructor called" << std::endl;
    gameUI = new GameUI(*window, *tabuleiro);
    std::cout << "GameUI constructor called" << std::endl;
    gameController = new GameController(*tabuleiro, gameUI->tamanho_casas);
    std::cout << "GameController constructor called" << std::endl;
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
                        std::cout << "--------------------------" << std::endl;
                        if (GameController::getPossiveisMovimentos(gameController->tabuleiro, false).size() < 10)
                            if (gameController->analisarCheck(true))
                            {
                                std::cout << "CHECK" << std::endl;
                                if (gameController->analisarCheckmate(!true))
                                {
                                    std::cout << "MOVIMENTOS POSSIVEIS DO OPONENTE: " << GameController::getPossiveisMovimentos(gameController->tabuleiro, false).size() << std::endl;
                                    std::cout << "CHECKMATE!!!!\nVITORIA DO JOGADOR" << std::endl;
                                    window->close();
                                    break;
                                }
                            }
                        vez_jogador = !vez_jogador;
                        vez_cpu = !vez_cpu;
                        std::cout << "--------------------------" << std::endl;
                    }
                }
        }
        if (!vez_jogador)
        {
            CPU.jogar();
            if (GameController::getPossiveisMovimentos(gameController->tabuleiro, true).size() < 20)
                if (gameController->analisarCheck(false))
                {
                    std::cout << "VOCÊ LEVOU CHECK" << std::endl;
                    if (gameController->analisarCheckmate(true))
                    {
                        std::cout << "CHECKMATE??!!\nVITORIA DA IA" << std::endl;
                        window->close();
                        break;
                    }
                }
            vez_jogador = !vez_jogador;
            vez_cpu = !vez_cpu;
        }
        gameUI->update(gameController);
        sf::sleep(sf::milliseconds(100));
    }
}