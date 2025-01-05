#include "../include/peças.hpp"
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
            return false;
    }
    else 
        return false;
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
    std::cout << "Pos : " << positionIndex.x << " " << positionIndex.y << " New Pos : " << new_index_x << " " << new_index_y << std::endl;
    
    // Primeira regra : enquanto tiver casas na frente disponíveis sem nenhuma peça amiga ou inimiga, pode andar em qualquer uma -> HORIZONTAL
    int pos_x, pos_y;
    if (index_x == new_index_x)
    {
        std::cout << "Torre: Pos : " << positionIndex.x << " " << positionIndex.y << " New Pos : " << new_index_x << " " << new_index_y << std::endl;
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

    if (isAdversario) 
    {
        index_x = 7 - positionIndex.x, index_y = 7 - positionIndex.y;
        new_index_x = 7 - new_index_x, new_index_y = 7 - new_index_y;
    }


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