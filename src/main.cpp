#include "../include/game.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "XADREZ", sf::Style::Titlebar | sf::Style::Close);
	Game game(&window);
	game.run();
	return 0;
}
