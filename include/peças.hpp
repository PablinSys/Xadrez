#ifndef PEÇAS_H
#define PEÇAS_H

#include <SFML/Graphics.hpp>
#include <../include/tabuleiro.hpp>

class Tabuleiro;
class Peça //classe abstrata
{
	public :
		bool isWhite;
		sf::Vector2i positionIndex;
		sf::Vector2f positionUI;
		Tabuleiro* tab;
		Peça(const sf::Vector2i& positionIndex, Tabuleiro& tab, const bool& isWhite);
		virtual bool analisarMovimento(const sf::Vector2i& new_pos) const = 0;
		virtual ~Peça();
};
class Peao : public Peça
{
	public :
		bool primeiroLance;
		Peao(const sf::Vector2i& positionIndex, Tabuleiro& tabuleiro, const bool& isWhite, bool primeiroLance);
		bool analisarMovimento(const sf::Vector2i& new_pos) const override;
};
class Torre : virtual public Peça
{
	public :
		Torre(const sf::Vector2i& positionIndex, Tabuleiro& tabuleiro, const bool& isWhite);
		bool analisarMovimento(const sf::Vector2i& new_pos) const override;
};
class Cavalo : public Peça
{
	public :
		Cavalo(const sf::Vector2i& positionIndex, Tabuleiro& tabuleiro, const bool& isWhite);
		bool analisarMovimento(const sf::Vector2i& new_pos) const override;
};
class Bispo : virtual public Peça
{
	public :
		Bispo(const sf::Vector2i& positionIndex, Tabuleiro& tabuleiro, const bool& isWhite);
		bool analisarMovimento(const sf::Vector2i& new_pos) const override;
};
class Rainha : public Bispo, public Torre
{
	public :
		Rainha(const sf::Vector2i& positionIndex, Tabuleiro& tabuleiro, const bool& isWhite);
		bool analisarMovimento(const sf::Vector2i& new_pos) const override;
};
class Rei : public Peça
{
	public :
		Rei(const sf::Vector2i& positionIndex, Tabuleiro& tabuleiro, const bool& isWhite);
		bool analisarMovimento(const sf::Vector2i& new_pos) const override;
};
#endif
