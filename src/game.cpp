#include "../include/game.hpp"
#include <iostream>

Game::Game(sf::RenderWindow* window) 
{
    std::cout << "Game constructor called" << std::endl;
    this->window = window;
    tabuleiro = new Tabuleiro(true);
    std::cout << "Tabuleiro constructor called" << std::endl;
    gameUI = new GameUI(*window, *tabuleiro);
    std::cout << "GameUI constructor called" << std::endl;
    gameController = new GameController(*tabuleiro, gameUI->tamanho_casas);
    std::cout << "GameController constructor called" << std::endl;
}

void Game::run()
{
    sf::Clock clock;
    sf::Time elapsed;
    while (window->isOpen())
    {
        sf::Event event;
        clock.restart();
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    sf::sleep(sf::milliseconds(500));
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        gameUI->OnMouseButtonLeftPressed(gameController);
                    else 
                        gameUI->OnMouseButtonLeftClicked(gameController);
                }
            }
        }
        gameUI->update();
        sf::sleep(sf::milliseconds(500));
    }
}