#include "../include/gameUI.hpp"
#include "../include/gameController.hpp"
#include "../include/array.hpp"
#include <SFML/System.hpp>
#include <filesystem>
#include <iostream>

GameUI::GameUI(sf::RenderWindow& window, Tabuleiro& tabuleiro)
    : window(&window), tabuleiro(tabuleiro)
{
    renderTab();
}

void GameUI::renderTab()
{
    int qtdVertices = 4, pos_x = 0, pos_y = 0, index = 0;
    tamanho_casas = (window->getSize().x + window->getSize().y)/2/8; int tamanho = (int)tamanho_casas;
    tabuleiro.tamanho_casas = tamanho;
    bool casasPretas = true;
    for(int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++){
            pos_x = x*tamanho; pos_y = y*tamanho;
            tabuleiroUI[index].setPrimitiveType(sf::PrimitiveType::Quads); tabuleiroUI[index].resize(qtdVertices);

            for (int i = 0; i < qtdVertices; i++)
            {
                int distance_x = i == 1 || i == 2 ? tamanho : 0, distance_y = i == 3 || i == 2 ? tamanho : 0;

                tabuleiroUI[index][i].position = sf::Vector2f(pos_x + distance_x, pos_y + distance_y);
                if (casasPretas)
                    tabuleiroUI[index][i].color = sf::Color(111, 149, 81); // Cor das casas pretas *Chess.com
                else
                    tabuleiroUI[index][i].color = sf::Color::White;
            }
            casasPretas = !casasPretas;
            index++;
        }
        casasPretas = !casasPretas;
    }
}
void GameUI::update(GameController* gameC)
{
    if ((int)tamanho_casas != (int)((window->getSize().x + window->getSize().y)/2/8))
        renderTab();

    window->clear(sf::Color::Black);
    Peça* (*peças)[8] = gameC->getTabuleiro();
    for (int i = 0; i < 64; i++)
        window->draw(tabuleiroUI[i]);
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (peças[y][x] != nullptr)
                window->draw(peças[y][x]->objectUI);

    window->display();
}
bool GameUI::OnMouseButtonLeftPressed(GameController* gameC)
{
    int pos_x = (int)( sf::Mouse::getPosition(*window).x/tamanho_casas), pos_y = (int)(sf::Mouse::getPosition(*window).y/tamanho_casas);

	while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
        gameC->moverPeça({pos_x, pos_y}, {(float) sf::Mouse::getPosition(*window).x, (float) sf::Mouse::getPosition(*window).y}, sf::Mouse::isButtonPressed(sf::Mouse::Left));
        update(gameC);
	}
    int result = gameC->moverPeça({pos_x, pos_y}, {(float) sf::Mouse::getPosition(*window).x, (float) sf::Mouse::getPosition(*window).y}, sf::Mouse::isButtonPressed(sf::Mouse::Left));
    update(gameC);
    if (result >= 0)
        return true;
    else 
        return false;
}
void GameUI::OnMouseButtonLeftClicked(GameController* gameC)
{
    return;

    int pos_x = (int)( sf::Mouse::getPosition(*window).x/tamanho_casas), pos_y = (int)(sf::Mouse::getPosition(*window).y/tamanho_casas);
    while (!sf::Mouse::isButtonPressed(sf::Mouse::Left)){}
    gameC->moverPeça({pos_x, pos_y}, {(float) sf::Mouse::getPosition(*window).x, (float) sf::Mouse::getPosition(*window).y}, sf::Mouse::isButtonPressed(sf::Mouse::Left));
    update(gameC);
}
GameUI::~GameUI()
{
    delete window;
}