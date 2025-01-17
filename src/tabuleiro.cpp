#include "../include/tabuleiro.hpp"
#include "../include/peças.hpp"
#include <iostream>
#include <cassert>

/**
 * @brief Construtor da classe Tabuleiro
 * 
 * Inicializa o tabuleiro com as peças iniciais do jogo, 
 * com as peças do oponente na parte superior e as peças do jogador na parte inferior
 * 
 * @param brancasPrimeiro True as peças brancas serão posicionados na visão do jogador, False as peças pretas serão posicionadas na visão do jogador
 * @param tamanho_casas Tamanho das casas do tabuleiro
 * 
 */
Tabuleiro::Tabuleiro(const bool& brancasPrimeiro, const float& tamanho_casas)
    : brancasPrimeiro(brancasPrimeiro), tamanho_casas(tamanho_casas)
{
    tabuleiro.resize(8);
    for (int y = 0; y < 8; y++)
    {
        tabuleiro[y].resize(8);
        for (int x = 0; x < 8; x++)
            tabuleiro[y][x] = nullptr;
    }

    // inicializando o tabuleiro
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (y < 2)
            {
                std::string cor = brancasPrimeiro ? "black" : "white";
                if (y == 0)
                {
                    if (x == 0 || x == 7)
                        tabuleiro[y][x] = new Torre(std::filesystem::current_path() / ("assets/" + cor + "/" + "torre" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro);
                    else if (x == 1 || x == 6)
                        tabuleiro[y][x] = new Cavalo(std::filesystem::current_path() / ("assets/" + cor + "/" + "cavalo" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro);
                    else if (x == 2 || x == 5)
                        tabuleiro[y][x] = new Bispo(std::filesystem::current_path() / ("assets/" + cor + "/" + "bispo" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro);
                    else if (x == 3)
                        tabuleiro[y][x] = new Rainha(std::filesystem::current_path() / ("assets/" + cor + "/" + "rainha" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro);
                    else if (x == 4)
                        tabuleiro[y][x] = new Rei(std::filesystem::current_path() / ("assets/" + cor + "/" + "rei" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro);
                }
                else 
                    tabuleiro[y][x] = new Peao(std::filesystem::current_path() / ("assets/" + cor + "/" + "peao" + ".png"), *this ,sf::Vector2i({x, y}), !brancasPrimeiro, true);
            }
            else if (y > 5)
            {
                std::string cor = brancasPrimeiro ? "white" : "black";
                if (y == 7)
                {
                    if (x == 0 || x == 7)
                        tabuleiro[y][x] = new Torre(std::filesystem::current_path() / ("assets/" + cor + "/" + "torre" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro);
                    else if (x == 1 || x == 6)
                        tabuleiro[y][x] = new Cavalo(std::filesystem::current_path() / ("assets/" + cor + "/" + "cavalo" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro);
                    else if (x == 2 || x == 5)
                        tabuleiro[y][x] = new Bispo(std::filesystem::current_path() / ("assets/" + cor + "/" + "bispo" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro);
                    else if (x == 3)
                        tabuleiro[y][x] = new Rainha(std::filesystem::current_path() / ("assets/" + cor + "/" + "rainha" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro);
                    else if (x == 4)
                        tabuleiro[y][x] = new Rei(std::filesystem::current_path() / ("assets/" + cor + "/" + "rei" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro);
                }
                else 
                    tabuleiro[y][x] = new Peao(std::filesystem::current_path() / ("assets/" + cor + "/" + "peao" + ".png"), *this , sf::Vector2i({x, y}), brancasPrimeiro, true);

            }
        }
    }
}

/**
 * @brief Construtor de cópia do tabuleiro
 * 
 * @param other Tabuleiro a ser copiado
 * 
 */
Tabuleiro::Tabuleiro(const Tabuleiro& other)
    : brancasPrimeiro(other.brancasPrimeiro), tamanho_casas(other.tamanho_casas)
{
    tabuleiro.resize(8);
    for (int y = 0; y < 8; y++) {
        tabuleiro[y].resize(8);
        for (int x = 0; x < 8; x++) {
            if (other.tabuleiro[y][x])
                tabuleiro[y][x] = other.tabuleiro[y][x]->clone(); // Cria uma nova peça
            else
                tabuleiro[y][x] = nullptr;
        }
    }
}

/**
 * @brief Obtem o tabuleiro
 * 
 * @return std::vector<std::vector<Peça*>> Tabuleiro
 */
std::vector<std::vector<Peça*>> Tabuleiro::getTabuleiro() const
{
    return tabuleiro;
}

/**
 * @brief Move uma peça do tabuleiro
 * 
 * Move uma peça do tabuleiro e atualiza sua posição UI
 * 
 * @param peça_pos Posição da peça a ser movida
 * @param new_pos Nova posição da peça
 */
void Tabuleiro::moverPeça(sf::Vector2i peça_pos, sf::Vector2i new_pos)
{
    Peça* peça = &*tabuleiro[peça_pos.y][peça_pos.x];

    // Se a peça for um Peao, desativa o primeiro lance ja que ele foi movido
    if (Peao* peao = dynamic_cast<Peao*>(peça))
        peao->primeiroLance = false;

    // Atualiza a posicao da peça no UI
    peça->objectUI.setPosition( 
    {
        (float)new_pos.x * tamanho_casas + (tamanho_casas-60)/2 , 
        (float)new_pos.y * tamanho_casas + (tamanho_casas-60)/2
    });
    peça->positionIndex = new_pos;

    // Move a peça
    tabuleiro[new_pos.y][new_pos.x] = peça;
    tabuleiro[peça_pos.y][peça_pos.x] = nullptr;
}

/**
 * @brief Retorna a posicao do Rei
 * 
 * @param tabuleiro Tabuleiro a ser verificado
 * @param isWhite True se for o Rei branco, False se for o Rei preto
 * 
 * @return sf::Vector2i Posicao do Rei
 */
sf::Vector2i Tabuleiro::getReiPosition(Tabuleiro* tabuleiro, const bool& isWhite)
{
    for (const auto& y : tabuleiro->tabuleiro)
        for (const auto& x : y)
        {
            if (x == nullptr)
                continue;
            if (Rei* rei = dynamic_cast<Rei*>(x))
                if (rei->isWhite == isWhite)
                    return rei->positionIndex;
        }
    throw std::runtime_error("Erro: Rei nao encontrado");
}

/**
 * @brief Destrutor do tabuleiro
 * 
 */
Tabuleiro::~Tabuleiro()
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (tabuleiro[y][x] != nullptr)
                delete tabuleiro[y][x];
        }
    }
}