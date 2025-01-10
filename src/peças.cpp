#include "../include/peças.hpp"
#include "../include/structs.hpp"
#include <iostream>

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
    // objectUI.setOrigin(objectUI.getPosition());
}

Peao::Peao(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite, bool primeiroLance) : 
    Peça(path_img, tab , positionIndex, isWhite), primeiroLance(primeiroLance) {}
Torre::Torre(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite) : 
     Peça(path_img, tab , positionIndex, isWhite) {}
Bispo::Bispo(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite) : 
     Peça(path_img, tab , positionIndex, isWhite) {}
Cavalo::Cavalo(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite) : 
     Peça(path_img, tab , positionIndex, isWhite) {}
Rainha::Rainha(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite) : 
     Peça(path_img, tab , positionIndex, isWhite), Bispo(path_img, tab , positionIndex, isWhite), Torre(path_img, tab , positionIndex, isWhite) {}
Rei::Rei(const std::filesystem::path& path_img, Tabuleiro& tab , const sf::Vector2i& positionIndex, const bool& isWhite) : 
     Peça(path_img, tab , positionIndex, isWhite) {}


Peça* Peao::clone() const
{
    return new Peao(*this);
}

Peça* Torre::clone() const
{
    return new Torre(*this);
}

Peça* Bispo::clone() const
{
    return new Bispo(*this);
}

Peça* Cavalo::clone() const
{
    return new Cavalo(*this);
}

Peça* Rainha::clone() const
{
    return new Rainha(*this);
}

Peça* Rei::clone() const
{
    return new Rei(*this);
}


bool Peao::analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const
{ 
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    int new_index_x, new_index_y;
    index_x = positionIndex.x, index_y = positionIndex.y;
    new_index_x = (int)(new_pos.x / tab->tamanho_casas); new_index_y = (int)(new_pos.y / tab->tamanho_casas);
    
    if (isAdversario)
    {
        index_y = 7 - positionIndex.y;
        new_index_y = 7 - new_index_y;
    }

    if (new_index_x >= 0 && new_index_y >= 0)
    {
        int pos_x, pos_y;
        pos_x = new_index_x, pos_y = new_index_y;
        if (isAdversario) pos_y = 7 - new_index_y;

        // Primeira regra : o peão não pode andar mais de uma casa se for o primeiro lance
        if (primeiroLance && new_index_x == index_x && 
            (index_y - 2 == new_index_y && index_y - 2 >= 0) &&
            tab->getTabuleiro()[pos_y][pos_x] == nullptr) {}
        // Segunda regra : caso andar 1 casa a frente, tenha nenhuma peça a frente
        else if (index_x == new_index_x && 
            (index_y - 1 == new_index_y && index_y - 1 >= 0) &&
            tab->getTabuleiro()[pos_y][pos_x] == nullptr){}
        // Quarta regra : o peão pode comer peças nas diagonais caso tiver um
        else if((index_x - 1 == new_index_x || index_x + 1 == new_index_x) && 
                (index_x+1 < 8 || index_x-1 >= 0) && 
                index_y - 1 == new_index_y && 
                index_y - 1 >= 0 && 
                tab->getTabuleiro()[pos_y][pos_x] != nullptr)
        {
            if (tab->getTabuleiro()[pos_y][pos_x]->isWhite == isWhite)
                return false;
            std::cout << "Capturing piece : " << typeid(*tab->getTabuleiro()[pos_y][pos_x]).name() << std::endl;
        }
        else 
        {
            std::cout << "Unpermitted movement : " << new_index_x << " " << new_index_y << std::endl;
            std::cout << "Conditions : " << index_x << " " << index_y << "≃≃" << new_index_x << " " << new_index_y << std::endl;
            return false;
        }
    }
    else
    { 
        std::cout << "Invalid position : " << new_pos.x << " " << new_pos.y << std::endl;
        return false;
    }
    return true; 
}
bool Torre::analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const 
{ 
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    int new_index_x, new_index_y; 

    index_x = positionIndex.x, index_y = positionIndex.y;
    new_index_x = (int)(new_pos.x / tab->tamanho_casas); new_index_y = (int)(new_pos.y / tab->tamanho_casas);
    
    if (isAdversario)
    {
        index_y = 7 - positionIndex.y;  
        new_index_y = 7 - new_index_y;
    }
    
    // Primeira regra : enquanto tiver casas na frente disponíveis sem nenhuma peça amiga ou inimiga, pode andar em qualquer uma -> HORIZONTAL
    int pos_x, pos_y;
    if (index_x == new_index_x)
    {
        //  CIMA ou BAIXO :
        if (index_y > new_index_y)
            for (int i = 1; i < index_y; i++) // index_y - i 
            {
                if (isAdversario) pos_y = 7 - index_y + i;
                else pos_y = index_y - i;
                pos_x = index_x;

                if (index_y - i == new_index_y)
                    break;
                else if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
                {
                    std::cout << "Torre: Encontrou uma peça em " << pos_x << " " << pos_y << " do tipo " << typeid(*tab->getTabuleiro()[pos_y][pos_x]).name() << std::endl;
                    return false;
                }
            }
        //  BAIXO ou CIMA : 
        if (index_y < new_index_y)
            for (int i = 1; i < 8 - index_y; i++) // 8 - index_y + i
            {
                if (isAdversario) pos_y = 7 - index_y - i;
                else pos_y = index_y - i;
                pos_x = index_x;

                if (index_y + i == new_index_y)
                    break;
                else if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
                {
                    std::cout << "Torre: Encontrou uma peça em " << pos_x << " " << pos_y << " do tipo " << typeid(*tab->getTabuleiro()[pos_y][pos_x]).name() << std::endl;
                    return false;
                }
            }
    }
    // Segunda Regra : enquanto tiver casas nas laterais disponíveis sem nenhuma peça amiga ou inimiga, pode andar em qualquer uma -> VERTICAL
    //  ESQUERDA :
    else if (index_y == new_index_y)
    {
        std::cout << "Torre: Pos : " << positionIndex.x << " " << positionIndex.y << " New Pos : " << new_index_x << " " << new_index_y << std::endl;
        if (index_x > new_index_x)
            for (int i = 1; i < index_x; i++) // index_x - i
            {
                if (isAdversario) pos_y = 7 - index_y;
                else pos_y = index_y;
                pos_x = index_x - i;

                if (index_x - i == new_index_x)
                    break;
                else if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
                {
                    std::cout << "Torre: Encontrou uma peça em " << pos_x << " " << pos_y << " do tipo " << typeid(*tab->getTabuleiro()[pos_y][pos_x]).name() << std::endl;
                    return false;
                }
            }
        //  DIREITA : 
        if (index_x < new_index_x)
            for (int i = 1; i < 8 - index_x; i++) // 8 - index_x + i
            {
                if (isAdversario) pos_y = 7 - index_y;
                else pos_y = index_y;
                pos_x = index_x + i;

                if (index_x + i == new_index_x)
                    break;
                else if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
                {
                    std::cout << "Torre: Encontrou uma peça em " << pos_x << " " << pos_y << " do tipo " << typeid(*tab->getTabuleiro()[pos_y][pos_x]).name() << std::endl;
                    return false;
                }
            }
    }
    else 
        return false;
    return true;
}
bool Bispo::analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const 
{
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    int new_index_x, new_index_y; 
        
    index_x = positionIndex.x, index_y = positionIndex.y;
    new_index_x = (int)(new_pos.x / tab->tamanho_casas); new_index_y = (int)(new_pos.y / tab->tamanho_casas);

    if (isAdversario) 
    {
        index_y = 7 - index_y;
        new_index_y = 7 - new_index_y;
    }
    
    if (std::abs(index_x - new_index_x) != std::abs(index_y - new_index_y))
        return false;

    int pos_x, pos_y;
    // Primeira regra : enquanto tiver casas na diagonal disponiveis sem nenhuma peça, pode andar em qualquer uma
    if (index_y > new_index_y && index_x != new_index_x && index_y)
    {
        //  SUPERIOR ESQUERDO :
        if (index_x > new_index_x)
        {
            for (int i = 1; i < index_y; i++)
            {
                if (isAdversario) pos_y = 7 - index_y + i;
                else pos_y = index_y - i;
                pos_x = index_x - i;

                if (index_y - i == new_index_y && index_x - i == new_index_x && index_x - i >= 0)
                    break;
                else if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
                    return false;
            }
        }
        //  SUPERIOR DIREITO :
        else if (index_x < new_index_x)
        {
            for (int i = 1; i < index_y; i++)
            {
                if (isAdversario) pos_y = 7 - index_y + i;
                else pos_y = index_y - i;
                pos_x = index_x + i;

                if (index_y - i == new_index_y && index_x + i == new_index_x && index_x + i < 8)
                    break;
                else if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
                    return false;
            }
        }
    }
    //  INFERIOR ESQUERDO :
    else if (index_y < new_index_y && index_x != new_index_x)
    {
        if (index_x > new_index_x)
        {
            for (int i = 1; i < 8 - index_y; i++)
            {
                if (isAdversario) pos_y = 7 - index_y - i;
                else pos_y = index_y - i;
                pos_x = index_x - i;

                if (index_y + i == new_index_y && index_x - i == new_index_x && index_x - i >= 0)
                    break;
                else if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
                    return false;
            }
        }
        else if (index_x > new_index_x)
        {
            for (int i = 1; i < 8 - index_y; i++)
            {
                if (isAdversario) pos_y = 7 - index_y - i;
                else pos_y = index_y - i;
                pos_x = index_x + i;

                if (index_y + i == new_index_y && index_x + i == new_index_x && index_x + i < 8)
                    break;
                else if (tab->getTabuleiro()[pos_y][pos_x] != nullptr)
                    return false;
            }
        }
    }
    else 
        return false;
    return true; 
}
bool Cavalo::analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const  
{ 
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    int new_index_x, new_index_y;

    index_x = positionIndex.x, index_y = positionIndex.y;
    new_index_x = (int)(new_pos.x / tab->tamanho_casas), new_index_y = (int)(new_pos.y / tab->tamanho_casas);
    
    if (isAdversario)   
    {
        index_y = 7 - index_y;
        new_index_y = 7 - new_index_y;
    }

    
    // Primeira regra : pode se posicionar dentre os 8 movimentos possiveis entre os limites da tabela e sem posicionar nenhuma posição de peça amiga 
    //  HORIZONTAL :
    //      CIMA :
    if ((std::abs(index_y - new_index_y) == 2 && std::abs(index_x - new_index_x) == 1) || 
        (std::abs(index_x - new_index_x) == 2 && std::abs(index_y - new_index_y) == 1))
    {
        if (index_y - 2 == new_index_y && index_y - 2 >= 0)
        {
            if ((index_x - 1 == new_index_x && index_x - 1 >= 0) || (index_x + 1 == new_index_x && index_x + 1 < 8))
                return true;
            else 
                return false;
        }
        //      BAIXO :
        else if (index_y + 2 == new_index_y && index_y + 2 < 8)
        {
            if ((index_x - 1 == new_index_x && index_x - 1 >= 0) || (index_x + 1 == new_index_x && index_x + 1 < 8))
                return true;
            else 
                return false;
        }
        //  VERTICAL :
        //      ESQUERDA :
        else if (index_x - 2 == new_index_x && index_x - 2 >= 0)
        {
            if ((index_y - 1 == new_index_y && index_y - 1 >= 0) || (index_y + 1 == new_index_y && index_y + 1 < 8))
                return true;
            else 
                return false;
        }
        //      DIREITA :
        else if (index_x + 2 == new_index_x && index_x + 2 < 8)
        {
            if ((index_y - 1 == new_index_y && index_y - 1 >= 0) || (index_y + 1 == new_index_y && index_y + 1 < 8))
                return true;
            else 
                return false;
            
        }
    }
    return false; 
}
bool Rainha::analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const 
{
    // Analisando movimentos diagonais, horizontais e verticais da Peça
    return Torre::analisarMovimento(tab, new_pos) || Bispo::analisarMovimento(tab, new_pos);
}
bool Rei::analisarMovimento(Tabuleiro* tab, const sf::Vector2i& new_pos) const 
{ 
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    int new_index_x, new_index_y;

    index_x = positionIndex.x, index_y = positionIndex.y;
    new_index_x = (int)(new_pos.x / tab->tamanho_casas), new_index_y = (int)(new_pos.y / tab->tamanho_casas);

    // Primeira regra : pode se posicionar entre as casas ao redor do rei e sem posicionar nenhuma peça amiga
    int init_x = index_x-1, init_y = index_y-1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            if (init_x + j == index_x && init_y + i == index_y)
                continue;
            else if (init_x + j == new_index_x && init_y + i == new_index_y && 
                     init_x + j < 8 && init_y + i < 8)
                return true;
        }
    return false; 
}
std::vector<Jogada> Peao::movimentosPossiveis(Tabuleiro* tab) const
{
    std::vector<Jogada> movimentos;
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    index_x = positionIndex.x, index_y = positionIndex.y;
    
    if (isAdversario)
        index_y = 7 - positionIndex.y;

    if (primeiroLance && index_y - 2 >= 0)
        if (tab->getTabuleiro()[!isAdversario ? index_y - 2 : 7 - (index_y - 2)][index_x] == nullptr)
            movimentos.push_back(Jogada(positionIndex, sf::Vector2i(index_x, !isAdversario ? index_y - 2 : 7 - (index_y - 2))));
        // Segunda regra : caso andar 1 casa a frente, tenha nenhuma peça a frente
    
    if (index_y - 1 >= 0)
        if (tab->getTabuleiro()[!isAdversario ? index_y - 1 : 7 - (index_y - 1)][index_x] == nullptr)
            movimentos.push_back(Jogada(positionIndex, sf::Vector2i(index_x, !isAdversario ? index_y - 1 : 7 - (index_y - 1))));

    // Quarta regra : o peão pode comer peças nas diagonais caso tiver um
    if (index_y - 1 >= 0)
    {
        if (index_x+1 < 8)
            if (tab->getTabuleiro()[!isAdversario ? index_y - 1 : 7 - (index_y - 1)][index_x+1] != nullptr) 
                if (tab->getTabuleiro()[!isAdversario ? index_y - 1 : 7 - (index_y - 1)][index_x+1]->isWhite != isWhite)
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(index_x+1, !isAdversario ? index_y - 1 : 7 - (index_y - 1))));
        else if (index_x-1 >= 0)
            if (tab->getTabuleiro()[!isAdversario ? index_y - 1 : 7 - (index_y - 1)][index_x+1] != nullptr)
                if (tab->getTabuleiro()[!isAdversario ? index_y - 1 : 7 - (index_y - 1)][index_x+1]->isWhite != isWhite)
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(index_x+1, !isAdversario ? index_y - 1 : 7 - (index_y - 1))));
    }
    return movimentos;
}
std::vector<Jogada> Bispo::movimentosPossiveis(Tabuleiro* tab) const
{
    std::vector<Jogada> movimentos;
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;       
    index_x = positionIndex.x, index_y = positionIndex.y;
    if (isAdversario) 
        index_y = 7 - index_y;
    
    int pos_x, pos_y;
    bool passouLimite1 = false, passouLimite2 = false;
    for (int y = 1; y < index_y; y++)
    {
        pos_y = index_y - y; pos_x = index_x - y;

        if (pos_x >= 0 && pos_x < 8 && pos_y >= 0 && pos_y < 8 && !passouLimite1)
        {
            if (isAdversario) pos_y = 7 - index_y + y;

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
        
        pos_y = index_y - y; pos_x = index_x + y;

        if (pos_x < 8 && pos_x >= 0 && pos_y >= 0 && pos_y < 8 && !passouLimite2)
        {
            if (isAdversario) pos_y = 7 - index_y + y;

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
    for (int y = 1; y < 8 - index_y; y++)
    {
        pos_y = index_y + y; pos_x = index_x - y;

        if (pos_x >= 0 && pos_y < 8 && pos_y >= 0 && !passouLimite1)
        {
            if (isAdversario) pos_y = 7 - index_y - y;

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
        
        pos_y = index_y + y; pos_x = index_x + y;

        if (pos_x < 8 && pos_y < 8 && pos_y >= 0 && pos_x >= 0 && !passouLimite2)
        {
            if (isAdversario) pos_y = 7 - index_y - y;

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
std::vector<Jogada> Torre::movimentosPossiveis(Tabuleiro* tab) const
{
    std::vector<Jogada> movimentos;
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    index_x = positionIndex.x, index_y = positionIndex.y;
    if (isAdversario)
        index_y = 7 - positionIndex.y;  
    
    // Primeira regra : enquanto tiver casas na frente disponíveis sem nenhuma peça amiga ou inimiga, pode andar em qualquer uma -> HORIZONTAL
    int pos_x, pos_y;
    bool passouLimite1 = false, passouLimite2 = false;
    for (int y = 1; y < 8; y++)
    {
        pos_y = index_y - y; pos_x = index_x;

        if (pos_x >= 0 && pos_x < 8 && pos_y >= 0 && pos_y < 8 && !passouLimite1)
        {
            if (isAdversario) pos_y = 7 - index_y + y;

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
        
        pos_y = index_y + y; pos_x = index_x;

        if (pos_x < 8 && pos_x >= 0 && pos_y >= 0 && pos_y < 8 && !passouLimite2)
        {
            if (isAdversario) pos_y = 7 - index_y + y;

            std::cout << "Torre: Verificando se pode andar para " << pos_x << " " << pos_y << std::endl;
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
                std::cout << "Torre: Pode andar para " << pos_x << " " << pos_y << std::endl;
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
        pos_x = index_x - x; pos_y = index_y;

        if (pos_x >= 0 && pos_x < 8 && pos_y >= 0 && pos_y < 8 && !passouLimite1)
        {
            if (isAdversario) pos_x = 7 - index_x;

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
        
        pos_x = index_x + x; pos_y = index_y;

        if (pos_x < 8 && pos_y < 8 && pos_y >= 0 && pos_x >= 0 && !passouLimite2)
        {
            if (isAdversario) pos_x = 7 - index_x;

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
std::vector<Jogada> Cavalo::movimentosPossiveis(Tabuleiro* tab) const
{
    std::vector<Jogada> movimentos;
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    index_x = positionIndex.x, index_y = positionIndex.y;
    if (isAdversario)
        index_y = 7 - positionIndex.y; 
    const std::pair<int, int> knight_moves[] = {
        {-1, 2}, {1, 2}, {-1, -2}, {1, -2},
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}
    };
    int pos_x, pos_y;
    for (const auto& move : knight_moves) {
        pos_x = index_x + move.first;
        pos_y = index_y + move.second;
        
        if (isAdversario) pos_y = 7 - index_y - move.second;
        
        if (pos_y >= 0 && pos_y < 8 && pos_x >= 0 && pos_x < 8)
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
std::vector<Jogada> Rainha::movimentosPossiveis(Tabuleiro* tab) const 
{
    std::vector<Jogada> movimentos;
    std::vector<Jogada> movimentosBispo = Bispo::movimentosPossiveis(tab);
    std::vector<Jogada> movimentosTorre = Torre::movimentosPossiveis(tab);
    movimentos.insert(movimentos.end(), movimentosBispo.begin(), movimentosBispo.end());
    movimentos.insert(movimentos.end(), movimentosTorre.begin(), movimentosTorre.end());
    return movimentos;
}
std::vector<Jogada> Rei::movimentosPossiveis(Tabuleiro* tab) const
{
    std::vector<Jogada> movimentos;
    int index_x, index_y; bool isAdversario = isWhite != tab->brancasPrimeiro;
    index_x = positionIndex.x, index_y = positionIndex.y;
    // Primeira regra : pode se posicionar entre as casas ao redor do rei e sem posicionar nenhuma peça amiga
    int init_x = index_x-1, init_y = index_y-1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            if (init_x + j == index_x && init_y + i == index_y)
                continue;
            else if (init_x + j < 8 && init_y + i < 8 && init_x + j >= 0 && init_y + i >= 0)
            {
                if (tab->getTabuleiro()[init_y + i][init_x + j] != nullptr)
                {
                    if (tab->getTabuleiro()[init_y + i][init_x + j]->isWhite != isWhite)
                        movimentos.push_back(Jogada(positionIndex, sf::Vector2i(init_x + j, init_y + i)));
                }
                else 
                    movimentos.push_back(Jogada(positionIndex, sf::Vector2i(init_x + j, init_y + i)));
            }
        }
    return movimentos;
}
int Rei::contarPecasMarcando(Tabuleiro* tabuleiro, const bool& isJogador)
{
    Peça* (*tab)[8] = tabuleiro->getTabuleiro();
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
                if (tab[pos_y][pos_x]->isWhite != isJogador)
                {
                    if ((typeid(*tab[pos_y][pos_x]) == typeid(Peao) && y == 1) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Bispo) || 
                        typeid(*tab[pos_y][pos_x]) == typeid(Rainha))
                    {
                        std::cout << "Piece marking the king: " 
                                  << typeid(*tab[pos_y][pos_x]).name() 
                                  << " at position (" << pos_x << ", " 
                                  << pos_y << ")" << std::endl;
                        count++;
                        passouLimite1 = true;
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
                std::cout << "Rei: Verificando se nenhuma peça diagonal em " << pos_x << " " << pos_y << " : " << typeid(*tab[pos_y][pos_x]).name() << std::endl;
                if (tab[pos_y][pos_x]->isWhite != isJogador)
                {
                    if ((typeid(*tab[pos_y][pos_x]) == typeid(Peao) && y == 1) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Bispo) || 
                        typeid(*tab[pos_y][pos_x]) == typeid(Rainha))
                    {
                        std::cout << "[DIAGONALMENTE A ESQUERDA E DIREITA SUPERIOR] Peça marcando o rei: " 
                                  << typeid(*tab[pos_y][pos_x]).name() 
                                  << " na posição (" << pos_x << ", " 
                                  << pos_y << ")" << std::endl;
                        count++;
                        passouLimite2 = true;
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
                if (tab[pos_y][pos_x]->isWhite != isJogador)
                {
                    if ((typeid(*tab[pos_y][pos_x]) == typeid(Peao) && y == 1) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Bispo) || 
                        typeid(*tab[pos_y][pos_x]) == typeid(Rainha))
                    {
                        std::cout << "Piece marking the king: " 
                                  << typeid(*tab[pos_y][pos_x]).name() 
                                  << " at position (" << pos_x << ", " 
                                  << pos_y << ")" << std::endl;
                        count++;
                        passouLimite1 = true;
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
                if (tab[pos_y][pos_x]->isWhite != isJogador)
                {
                    if ((typeid(*tab[pos_y][pos_x]) == typeid(Peao) && y == 1) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Bispo) || 
                        typeid(*tab[pos_y][pos_x]) == typeid(Rainha))
                    {
                        std::cout << "[DIAGONALMENTE A ESQUERDA E DIREITA SUPERIOR]Peça marcando o rei: " 
                                  << typeid(*tab[pos_y][pos_x]).name() 
                                  << " na posição (" << pos_x << ", " 
                                  << pos_y << ")" << std::endl;
                        count++;
                        passouLimite2 = true;
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
    for (int x = 1; x < index_x; x++)
    {
        if (isJogador) pos_x = index_x + x, pos_y = index_y;
        else pos_x = index_x + x, pos_y = 7 - index_y;

        if (pos_x < 8 && pos_x >= 0 && pos_y >= 0 && pos_y < 8 && !passouLimite1) 
        {
            if (tab[pos_y][pos_x] != nullptr) 
            {
                if (tab[pos_y][pos_x]->isWhite != isJogador) 
                {
                    if (typeid(*tab[pos_y][pos_x]) == typeid(Rainha) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Torre)) 
                    {
                        std::cout << "Peça marcando o rei: "
                                  << typeid(*tab[pos_y][pos_x]).name()
                                  << " na posição (" << pos_x << ", "
                                  << pos_y << ")" << std::endl;
                        count++;
                        passouLimite1 = true;
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
                if (tab[pos_y][pos_x]->isWhite != isJogador) 
                {
                    if (typeid(*tab[pos_y][pos_x]) == typeid(Rainha) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Torre)) 
                    {
                        std::cout << "Peça marcando o rei: "
                                  << typeid(*tab[pos_y][pos_x]).name()
                                  << " na posição (" << pos_x << ", "
                                  << pos_y << ")" << std::endl;
                        count++;
                        passouLimite2 = true;
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
    for (int y = 1; y < index_y; y++)
    {
        if (isJogador) pos_x = index_x, pos_y = index_y + y;
        else pos_x = index_x, pos_y = 7 - index_y - y;
        
        if (pos_y < 8 && pos_y >= 0 && pos_x >= 0 && pos_x < 8 && !passouLimite1)
            if (tab[pos_y][pos_x] != nullptr)
            {
                if (tab[pos_y][pos_x]->isWhite != isJogador)
                {
                    if (typeid(*tab[pos_y][pos_x]) == typeid(Rainha) ||
                        typeid(*tab[pos_y][pos_x]) == typeid(Torre))
                    {
                        std::cout << "Peça marcando o rei: " 
                                  << typeid(*tab[pos_y][pos_x]).name() 
                                  << " na posição (" << pos_x << ", " 
                                  << pos_y << ")" << std::endl;
                        count++;
                        passouLimite1 = true;
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
                if (tab[pos_y][pos_x]->isWhite != isJogador)
                {
                    if (typeid(*tab[pos_y][pos_x]) == typeid(Rainha) ||
                    typeid(*tab[pos_y][pos_x]) == typeid(Torre))
                    {
                        std::cout << "Peça marcando o rei: " 
                                  << typeid(*tab[pos_y][pos_x]).name() 
                                  << " na posição (" << pos_x << ", " 
                                  << pos_y << ")" << std::endl;
                        count++;
                        passouLimite2 = true;
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
            if (tab[pos_y][pos_x] != nullptr && typeid(*tab[pos_y][pos_x]) == typeid(Cavalo) &&
                tab[pos_y][pos_x]->isWhite != isJogador) {
                std::cout << "Cavalo marcando o rei: " << typeid(*tab[pos_y][pos_x]).name()
                          << " na posição (" << pos_x << ", " << pos_y << ")"
                          << " cor: " << (tab[pos_y][pos_x]->isWhite ? "Branco" : "Preto") << std::endl;
                count++;
            }
        }
    }
    
    return count;
}
