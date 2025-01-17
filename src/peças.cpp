#include "../include/peças.hpp"
#include "../include/structs.hpp"
#include <iostream>

/**
 * @brief Construtor da classe Peça
 * 
 * @param path_img Caminho para a imagem da peça
 * @param tab Tabuleiro
 * @param positionIndex Posição da peça no tabuleiro
 * @param isWhite Cor da peça
 * 
 */
Peça::Peça(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite)
    :positionIndex(positionIndex), isWhite(isWhite)
{
    if (!texture.loadFromFile(path_img)) {
        throw std::runtime_error("Failed to load texture");
    }
    texture.setSmooth(true);
    objectUI.setTexture(texture);
    objectUI.setPosition(
        positionIndex.x * tab.tamanho_casas + (tab.tamanho_casas - 60)/2,
        positionIndex.y * tab.tamanho_casas + (tab.tamanho_casas - 60)/2
    );
}

// Construtor da classe Peao
Peao::Peao(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite, bool primeiroLance) : 
    Peça(path_img, tab , positionIndex, isWhite), primeiroLance(primeiroLance) {}
// Construtor da classe Torre
Torre::Torre(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite) : 
     Peça(path_img, tab , positionIndex, isWhite) {}
// Construtor da classe Bispo
Bispo::Bispo(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite) : 
     Peça(path_img, tab , positionIndex, isWhite) {}
// Construtor da classe Cavalo
Cavalo::Cavalo(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite) : 
     Peça(path_img, tab , positionIndex, isWhite) {}
// Construtor da classe Rainha
Rainha::Rainha(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite) : 
     Peça(path_img, tab , positionIndex, isWhite), Bispo(path_img, tab , positionIndex, isWhite), Torre(path_img, tab , positionIndex, isWhite) {}
// Construtor da classe Rei
Rei::Rei(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite) : 
     Peça(path_img, tab , positionIndex, isWhite) {}


Peça* Peao::clone() const
{
    return new Peao(isWhite, positionIndex, primeiroLance);
}

Peça* Torre::clone() const
{
    return new Torre(isWhite, positionIndex);
}

Peça* Bispo::clone() const
{
    return new Bispo(isWhite, positionIndex);
}

Peça* Cavalo::clone() const
{
    return new Cavalo(isWhite, positionIndex);
}

Peça* Rainha::clone() const
{
    return new Rainha(isWhite, positionIndex);
}

Peça* Rei::clone() const
{
    return new Rei(isWhite, positionIndex);
}

/**
 * @brief Função que retorna todas as jogadas possíveis para uma peça peao
 * 
 * Primeira regra: o peão pode andar 2 casas para frente, caso seja o primeiro lance
 * Segunda regra: o peão pode andar 1 casa para frente
 * Terceira regra: o peão pode comer uma peça adversaria nas diagonais
 * 
 * @param tab Tabuleiro
 * @return std::vector<Jogada>
 * 
 */
std::vector<Jogada> Peao::movimentosPossiveis(Tabuleiro* tab) const
{
    std::vector<Jogada> movimentos;
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    index_x = positionIndex.x, index_y = positionIndex.y;
    int pos_x, pos_y;
    
    pos_y = !isAdversario ? index_y - 2 : index_y + 2;
    pos_x = index_x;

    if (primeiroLance && pos_y >= 0 && pos_y < 8)
        if (tab->getTabuleiro()[pos_y][pos_x] == nullptr && tab->getTabuleiro()[!isAdversario ? pos_y + 1 : pos_y - 1][pos_x] == nullptr)
            movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));

    // Segunda regra : caso andar 1 casa a frente, tenha nenhuma peça a frente
    pos_y = !isAdversario ? index_y - 1 : index_y + 1;
    if (pos_y >= 0 && pos_y < 8)
        if (tab->getTabuleiro()[pos_y][pos_x] == nullptr)
            movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
    // Quarta regra : o peão pode comer peças nas diagonais caso tiver um
    if (pos_y >= 0 && pos_y < 8)
    {
        pos_x = index_x + 1;
        if (pos_x < 8)
            if (tab->getTabuleiro()[pos_y][pos_x] != nullptr) 
                if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));

        pos_x = index_x - 1;
        if (index_x-1 >= 0)
            if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
                if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y))); 
    }
    return movimentos;
}

/**
 * @brief Função que retorna todas as jogadas possíveis para uma peça bispo
 * 
 * Enquanto tiver casas disponíveis nas diagonais sem nenhuma peça amiga ou inimiga, pode andar em qualquer uma
 * 
 * @param tab Tabuleiro
 * @return std::vector<Jogada>
 */
std::vector<Jogada> Bispo::movimentosPossiveis(Tabuleiro* tab) const
{
    std::vector<Jogada> movimentos;
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;       
    index_x = positionIndex.x, index_y = positionIndex.y;
    
    int pos_x, pos_y;
    bool passouLimite1 = false, passouLimite2 = false;
    for (int y = 1; y < 8; y++)
    {
        pos_x = index_x - y;
        if (isAdversario) 
            pos_y = index_y + y;
        else 
            pos_y = index_y - y; 

        if (pos_x >= 0 && pos_x < 8 && pos_y >= 0 && pos_y < 8 && !passouLimite1)
        {
            if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
            {
                if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
                passouLimite1 = true;
            }
            else 
                movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
        }
        else 
            passouLimite1 = true;
        
        pos_x = index_x + y;

        if (pos_x < 8 && pos_x >= 0 && pos_y >= 0 && pos_y < 8 && !passouLimite2)
        {
            if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
            {
                if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
                passouLimite2 = true;
            }
            else 
                movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
        }
        else
            passouLimite2 = true;
        
        if (passouLimite1 && passouLimite2) break;
    }

    passouLimite1 = false, passouLimite2 = false;
    for (int y = 1; y < 8; y++)
    {
        pos_x = index_x - y;
        if (isAdversario) 
            pos_y = index_y - y;
        else 
            pos_y = index_y + y; 

        if (pos_x >= 0 && pos_y < 8 && pos_y >= 0 && !passouLimite1)
        {
            if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
            {
                if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
                passouLimite1 = true;
            }
            else 
                movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
        }
        else 
            passouLimite1 = true;
        
        pos_x = index_x + y;

        if (pos_x < 8 && pos_y < 8 && pos_y >= 0 && pos_x >= 0 && !passouLimite2)
        {
            if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
            {
                if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
                passouLimite2 = true;
            }
            else 
                movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
        }
        else
            passouLimite2 = true;
        
        if (passouLimite1 && passouLimite2) break;
    }
    return movimentos;
}

/**
 * @brief Função que retorna todas as jogadas possíveis para uma peça torre
 * 
 * Enquanto tiver casas nas horizontais e verticais disponiveis sem nenhuma peça amiga ou inimiga, pode andar em qualquer uma
 * 
 * @param tab Tabuleiro onde a peça se encontra
 * @return std::vector<Jogada> com todas as jogadas possíveis
 * 
 */
std::vector<Jogada> Torre::movimentosPossiveis(Tabuleiro* tab) const
{
    std::vector<Jogada> movimentos;
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    index_x = positionIndex.x, index_y = positionIndex.y;
    
    // Primeira regra : enquanto tiver casas na frente disponíveis sem nenhuma peça amiga ou inimiga, pode andar em qualquer uma -> HORIZONTAL
    int pos_x, pos_y;
    bool passouLimite1 = false, passouLimite2 = false;
    for (int y = 1; y < 8; y++)
    {
        pos_x = index_x;
        pos_y = index_y - y; 

        if (pos_x >= 0 && pos_x < 8 && pos_y >= 0 && pos_y < 8 && !passouLimite1)
        {
            if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
            {
                if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
                passouLimite1 = true;
            }
            else 
                movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
        }
        else 
            passouLimite1 = true;

        pos_y = index_y + y;

        if (pos_x < 8 && pos_x >= 0 && pos_y >= 0 && pos_y < 8 && !passouLimite2)
        {
            if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
            {
                if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                {
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
                }
                passouLimite2 = true;
            }
            else 
            {
                movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
            }
        }
        else
            passouLimite2 = true;
        
        if (passouLimite1 && passouLimite2) break;
    }

    // Segunda regra : enquanto tiver casas na frente disponíveis sem nenhuma peça amiga ou inimiga, pode andar em qualquer uma -> VERTICAL
    passouLimite1 = false, passouLimite2 = false;
    for (int x = 1; x < 8; x++)
    {
        pos_x = index_x - x; 
        pos_y = index_y;

        if (pos_x >= 0 && pos_x < 8 && pos_y >= 0 && pos_y < 8 && !passouLimite1)
        {
            if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
            {
                if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
                passouLimite1 = true;
            }
            else 
                movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
        }
        else 
            passouLimite1 = true;
        
        pos_x = index_x + x; 
        pos_y = index_y;

        if (pos_x < 8 && pos_y < 8 && pos_y >= 0 && pos_x >= 0 && !passouLimite2)
        {
            if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
            {
                if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
                passouLimite2 = true;
            }
            else 
                movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
        }
        else
            passouLimite2 = true;
        
        if (passouLimite1 && passouLimite2) break;
    }
    return movimentos;
}

/**
 * @brief Função que retorna os movimentos possíveis de um cavalo
 * 
 * Verifica se o cavalo pode se mover entre as 8 posições possíveis em L
 * 
 * @param tab Tabuleiro onde a peça se encontra
 * @return std::vector<Jogada> com todas as jogadas possíveis
 * 
 */
std::vector<Jogada> Cavalo::movimentosPossiveis(Tabuleiro* tab) const
{
    std::vector<Jogada> movimentos;
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    index_x = positionIndex.x, index_y = positionIndex.y;
    const std::pair<int, int> knight_moves[] = {
        {-1, 2}, {1, 2}, {-1, -2}, {1, -2},
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}
    };
    int pos_x, pos_y;    
    for (const auto& move : knight_moves) {
        pos_x = index_x + move.first;
        pos_y = index_y + move.second;
        
        if (isAdversario) pos_y = index_y - move.second;
        
        if (pos_y >= 0 && pos_y < 8 && pos_x >= 0 && pos_x < 8 && (std::abs(index_x - pos_x) + std::abs(index_y - pos_y) == 3))
        {
            if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
            {
                if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                {
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
                }
            }
            else 
                movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
        }
    }
    return movimentos;
}

/**
 * @brief Função que retorna os movimentos possíveis de uma rainha
 * 
 * É uma combinação de movimentos possíveis de bispo e torre
 * 
 * @param tab Tabuleiro onde a peça se encontra
 * @return std::vector<Jogada> com todas as jogadas possíveis
 * 
 */
std::vector<Jogada> Rainha::movimentosPossiveis(Tabuleiro* tab) const 
{
    std::vector<Jogada> movimentos;
    std::vector<Jogada> movimentosBispo = Bispo::movimentosPossiveis(tab);
    std::vector<Jogada> movimentosTorre = Torre::movimentosPossiveis(tab);
    movimentos.insert(movimentos.end(), movimentosBispo.begin(), movimentosBispo.end());
    movimentos.insert(movimentos.end(), movimentosTorre.begin(), movimentosTorre.end());
    return movimentos;
}

/**
 * @brief Função que retorna os movimentos possíveis de um rei
 * 
 * Verifica se o rei pode se mover entre as casas ao redor dele
 * 
 * @param tab Tabuleiro onde a peça se encontra
 * @return std::vector<Jogada> com todas as jogadas possíveis
 * 
 */
std::vector<Jogada> Rei::movimentosPossiveis(Tabuleiro* tab) const
{
    std::vector<Jogada> movimentos;
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    int pos_x, pos_y;
    index_x = positionIndex.x, index_y = positionIndex.y;
    int init_x = index_x-1, init_y = index_y-1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            pos_x = init_x + j, pos_y = init_y + i;
            if (init_x + j == index_x && init_y + i == index_y)
                continue;
            else if (pos_x < 8 && pos_y < 8 && pos_x >= 0 && pos_y >= 0)
            {            
                if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
                {
                    if (tab->getTabuleiro()[pos_y][pos_x]->isWhite != isWhite)
                        movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
                }
                else 
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(pos_x, pos_y)));
            }
        }
    return movimentos;
}

/**
 * @brief Função que verifica se o rei está em check
 * 
 * Verifica entre posições adversarias, se alguma peça estiver marcando o rei
 * 
 * @param tabuleiro Tabuleiro onde a peça se encontra
 * @param isJogador True se for o jogador, False se for o oponente
 * @return True se o rei estiver em check False se o rei não estiver em check
 */
bool Rei::isCheck(Tabuleiro* tabuleiro, const bool& isWhite)
{
    std::vector<std::vector<Peça*>> tab = tabuleiro->getTabuleiro();
    bool isJogador = this->isWhite == tabuleiro->brancasPrimeiro;
    int count = 0;
    int index_x = positionIndex.x, index_y = positionIndex.y;
    if (!isJogador) index_y = 7 - index_y;


    // Analisar se nenhuma peça marcando diagonalmente o rei
    //  DIAGONALMENTE A ESQUERDA E DIREITA SUPERIOR
    
    int pos_x, pos_y;
    bool passouLimite1 = false;
    bool passouLimite2 = false;
    for (int y = 1; y < 8; y++)
    {
        if (isJogador) pos_x = index_x + y, pos_y = index_y - y;
        else pos_x = index_x + y, pos_y = 7 - index_y + y;
        

        if (pos_x < 8 && pos_y >= 0 && pos_y < 8 && !passouLimite1)
        {
            if (tab[pos_y][pos_x] != nullptr)
            {
                if (tab[pos_y][pos_x]->isWhite != isWhite)
                {
                    if ((typeid(*tab[pos_y][pos_x]) == typeid(Peao) && y == 1) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Bispo) || 
                        typeid(*tab[pos_y][pos_x]) == typeid(Rainha))
                    {
                        return true;
                    }
                    else 
                        passouLimite1 = true;
                }
                else 
                    passouLimite1 = true;
            }
        }
        else passouLimite1 = true;

        if (isJogador) pos_x = index_x - y, pos_y = index_y - y;
        else pos_x = index_x - y, pos_y = 7 - index_y + y;

        if (pos_x >= 0 && pos_y >= 0 && pos_y < 8 && !passouLimite2)
        {
            if (tab[pos_y][pos_x] != nullptr)
            {
                if (tab[pos_y][pos_x]->isWhite != isWhite)
                {
                    if ((typeid(*tab[pos_y][pos_x]) == typeid(Peao) && y == 1) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Bispo) || 
                        typeid(*tab[pos_y][pos_x]) == typeid(Rainha))
                    {
                        return true;
                    }
                    else 
                        passouLimite2 = true;
                }
                else 
                    passouLimite2 = true;
            }
        }
        else if (passouLimite1 && passouLimite2) break;
    }
    //  DIAGONALMENTE A ESQUERDA E DIREITA SUPERIOR
    passouLimite1 = false;
    passouLimite2 = false;
    for (int y = 1; y < index_y; y++)
    {
        if (isJogador) pos_x = index_x + y, pos_y = index_y - y;
        else pos_x = index_x + y, pos_y = 7 - index_y + y;

        if (pos_x < 8 && pos_x >= 0 && pos_y >= 0 && pos_y < 8 && !passouLimite1)
            if (tab[pos_y][pos_x] != nullptr)
            {
                if (tab[pos_y][pos_x]->isWhite != isWhite)
                {
                    if ((typeid(*tab[pos_y][pos_x]) == typeid(Peao) && y == 1) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Bispo) || 
                        typeid(*tab[pos_y][pos_x]) == typeid(Rainha))
                    {
                        return true;
                    }
                    else 
                        passouLimite1 = true;
                }
                else passouLimite1 = true;
            }
        
        if (isJogador) pos_x = index_x - y, pos_y = index_y - y;
        else pos_x = index_x - y, pos_y = 7 - index_y + y;

        if (pos_x >= 0 && pos_x < 8 &&  pos_y >= 0 && pos_y < 8 && !passouLimite2)
            if (tab[pos_y][pos_x] != nullptr)
            {
                if (tab[pos_y][pos_x]->isWhite != isWhite)
                {
                    if ((typeid(*tab[pos_y][pos_x]) == typeid(Peao) && y == 1) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Bispo) || 
                        typeid(*tab[pos_y][pos_x]) == typeid(Rainha))
                    {
                        return true;
                    }
                    else 
                        passouLimite2 = true;
                }
                else passouLimite2 = true;
            }
        else if (passouLimite1 && passouLimite2) break;
    }

    // ANALISAR SE NENHUMA PECA MARCANDO HORIZONTALMENTE E VERTICALMENTE O REI
    //  VERTICALMENTE ESQUERDA E DIREITA
    passouLimite1 = false;
    passouLimite2 = false;
    for (int x = 1; x < 8; x++)
    {
        if (isJogador) pos_x = index_x + x, pos_y = index_y;
        else pos_x = index_x + x, pos_y = 7 - index_y;

        if (pos_x < 8 && pos_x >= 0 && pos_y >= 0 && pos_y < 8 && !passouLimite1) 
        {
            if (tab[pos_y][pos_x] != nullptr) 
            {
                if (tab[pos_y][pos_x]->isWhite != isWhite) 
                {
                    if (typeid(*tab[pos_y][pos_x]) == typeid(Rainha) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Torre)) 
                    {
                        return true;
                    }
                    else 
                        passouLimite1 = true;
                } 
                else 
                    passouLimite1 = true;
            }
        } 

        pos_x = index_x - x;

        if (pos_x >= 0 && pos_y >= 0 && pos_y < 8 && !passouLimite2) 
        {
            if (tab[pos_y][pos_x] != nullptr) 
            {
                if (tab[pos_y][pos_x]->isWhite != isWhite) 
                {
                    if (typeid(*tab[pos_y][pos_x]) == typeid(Rainha) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Torre)) 
                    {
                        return true;
                    }
                    else 
                        passouLimite2 = true;
                } 
                else 
                    passouLimite2 = true;
            }
        } 
        else if (passouLimite1 && passouLimite2) break;
    }
    //  HORIZONTALMENTE SUPERIOR E INFERIOR
    passouLimite1 = false;
    passouLimite2 = false;
    for (int y = 1; y < 8; y++)
    {
        if (isJogador) pos_x = index_x, pos_y = index_y + y;
        else pos_x = index_x, pos_y = 7 - index_y - y;
        
        if (pos_y < 8 && pos_y >= 0 && pos_x >= 0 && pos_x < 8 && !passouLimite1)
            if (tab[pos_y][pos_x] != nullptr)
            {
                if (tab[pos_y][pos_x]->isWhite != isWhite)
                {
                    if (typeid(*tab[pos_y][pos_x]) == typeid(Rainha) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Torre))
                    {
                        return true;
                    }
                    else passouLimite1 = true;
                }
                else passouLimite1 = true;
            }

        if (isJogador) pos_y = index_y - y;
        else pos_y = 7 - index_y + y;

        if (pos_y >= 0 && pos_y < 8 && pos_x >= 0 && pos_x < 8 && !passouLimite2)
            if (tab[pos_y][pos_x] != nullptr)
            {
                if (tab[pos_y][pos_x]->isWhite != isWhite)
                {
                    if (typeid(*tab[pos_y][pos_x]) == typeid(Rainha) ||
                    typeid(*tab[pos_y][pos_x]) == typeid(Torre))
                    {
                        return true;
                    }
                    else passouLimite2 = true;
                }
                else passouLimite2 = true;
            }
        else if (passouLimite1 && passouLimite2) break;
    }
    // ANALISAR SE NENHUM CAVALO MARCA O REI
    const std::pair<int, int> knight_moves[] = {
        {-1, 2}, {1, 2}, {-1, -2}, {1, -2},
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}
    };
    
    for (const auto& move : knight_moves) {
        pos_x = index_x + move.first;
        pos_y = index_y + move.second;
        
        if (!isJogador) pos_y = 7 - index_y - move.second;
        
        if (pos_y >= 0 && pos_y < 8 && pos_x >= 0 && pos_x < 8) {
            if (tab[pos_y][pos_x] != nullptr)
                if (typeid(*tab[pos_y][pos_x]) == typeid(Cavalo) &&
                    tab[pos_y][pos_x]->isWhite != isWhite) 
                {
                    return true;
                }
        }
    }
    
    // ANALISAR SE NENHUM REI MARCA O REI
    index_x = index_x, index_y = positionIndex.y;
    int init_x = index_x-1, init_y = index_y-1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            pos_x = init_x + j;
            pos_y = init_y + i;
            if (init_x + j == index_x && init_y + i == index_y)
                continue;

            else if (pos_x < 8 && pos_y < 8 && pos_x >= 0 && pos_y >= 0)
            {
                if (tab[pos_y][pos_x] != nullptr)
                {
                    if (tab[pos_y][pos_x]->isWhite != isWhite)
                    {
                        if (typeid(*tab[pos_y][pos_x]) == typeid(Rei))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    return count;
}