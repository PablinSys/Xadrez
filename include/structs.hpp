#ifndef STRUCTS_H
#define STRUCTS_H
#include <SFML/Graphics.hpp>
struct Jogada
{
    sf::Vector2i peça_pos;
    sf::Vector2i new_pos;
    Jogada(const sf::Vector2i& peça_pos, const sf::Vector2i& new_pos) : peça_pos(peça_pos), new_pos(new_pos) {}
};

#endif // STRUCTS_H
