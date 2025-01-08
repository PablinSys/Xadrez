#include "../include/game.hpp"
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
                        vez_jogador = !vez_jogador;
                        vez_cpu = !vez_cpu;
                        std::cout << "--------------------------" << std::endl;
                        if (gameController->analisarCheck(true))
                        {
                            std::cout << "CHECK" << std::endl;
                        }
                        std::cout << "--------------------------" << std::endl;
                    }
                }
        }
        if (!vez_jogador)
        {
            //CPU();
            vez_jogador = !vez_jogador;
            vez_cpu = !vez_cpu;
            sf::sleep(sf::seconds(1));
        }
        gameUI->update(gameController);
        sf::sleep(sf::milliseconds(100));
    }
}