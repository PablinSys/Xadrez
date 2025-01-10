#ifndef PEÇAS_H
#define PEÇAS_H

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <vector>
#include <../include/tabuleiro.hpp>
#include <../include/structs.hpp>

class Tabuleiro;
class Array;
class Peça //classe abstrata
{
	public :
		bool isWhite;
		sf::Vector2i positionIndex;
		sf::Texture texture;
		sf::Sprite objectUI;
		Peça(const std::filesystem::path& path_img, Tabuleiro& tab, const sf::Vector2i& positionIndex, const bool& isWhite);
		virtual bool analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const = 0;
		virtual Peça* clone() const = 0;
		virtual std::vector<Jogada> movimentosPossiveis(Tabuleiro* tab) const = 0;
		virtual ~Peça() = default;
};
class Peao : public Peça
{
	public :
		bool primeiroLance;
		Peao(const std::filesystem::path& path_img, Tabuleiro& tab, const sf::Vector2i& positionIndex, const bool& isWhite, bool primeiroLance);
		bool analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const override;
		std::vector<Jogada> movimentosPossiveis(Tabuleiro* tab) const override;
		Peça* clone() const override;
};
class Torre : virtual public Peça
{
	public :
		Torre(const std::filesystem::path& path_img, Tabuleiro& tab, const sf::Vector2i& positionIndex, const bool& isWhite);
		bool analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const override;
		std::vector<Jogada> movimentosPossiveis(Tabuleiro* tab) const override;
		Peça* clone() const override;
};
class Cavalo : public Peça
{
	public :
		Cavalo(const std::filesystem::path& path_img, Tabuleiro& tab, const sf::Vector2i& positionIndex, const bool& isWhite);
		bool analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const override;
		std::vector<Jogada> movimentosPossiveis(Tabuleiro* tab) const override;
		Peça* clone() const override;
};
class Bispo : virtual public Peça
{
	public :
		Bispo(const std::filesystem::path& path_img, Tabuleiro& tab, const sf::Vector2i& positionIndex, const bool& isWhite);
		bool analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const override;
		std::vector<Jogada> movimentosPossiveis(Tabuleiro* tab) const override;
		Peça* clone() const override;
};
class Rainha : public Bispo, public Torre
{
	public :
		Rainha(const std::filesystem::path& path_img, Tabuleiro& tab, const sf::Vector2i& positionIndex, const bool& isWhite);
		bool analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const override;
		std::vector<Jogada> movimentosPossiveis(Tabuleiro* tab) const override;
		Peça* clone() const override;
};
class Rei : public Peça
{
	public :
		Rei(const std::filesystem::path& path_img, Tabuleiro& tab, const sf::Vector2i& positionIndex, const bool& isWhite);
		bool analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const override;
		std::vector<Jogada> movimentosPossiveis(Tabuleiro* tab) const override;
		Peça* clone() const override;
		int contarPecasMarcando(Tabuleiro* tabuleiro, const bool& isJogador);
};
#endif
