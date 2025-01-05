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
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            else if (event.type == sf::Event::MouseButtonPressed)
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    gameUI->OnMouseButtonLeftPressed(gameController);
        }
        gameUI->update(gameController);
        sf::sleep(sf::milliseconds(100));
    }
}