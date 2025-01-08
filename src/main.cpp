#include "../include/game.hpp"
#include <csignal>
#include <iostream>

void signalHandler(int signal) {
    if (signal == SIGSEGV) {
        std::cerr << "Segmentação falhou." << std::endl;
    }
}

int main() {
    std::signal(SIGSEGV, signalHandler);

    sf::RenderWindow window(sf::VideoMode(800, 800), "XADREZ", sf::Style::Titlebar | sf::Style::Close);
    Game game(&window);
    game.run();

    return 0;
}
