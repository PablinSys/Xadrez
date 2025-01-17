#include "../include/gameUI.hpp"
#include "../include/gameController.hpp"
#include "../include/algoritmo.hpp"
#include <SFML/System.hpp>
#include <filesystem>
#include <iostream>

GameUI::GameUI(sf::RenderWindow& window, Tabuleiro* tabuleiro)
    : window(&window), tabuleiro(tabuleiro)
{
    renderTab();
}

/**
 * @brief Renderiza as casas do tabuleiro
 * 
 * Desenha os vertices de vários quadrados na tela. E esses quadrados são
 * preenchidos com cores verdes e brancas para representar as casas pretas e brancas respectivamente
 * 
 */
void GameUI::renderTab()
{
    int qtdVertices = 4, pos_x = 0, pos_y = 0;
    // Calculo da dimensao das casas de acordo com a relação entre largura, altura da janela e a quantidade de casas do tabuleiro
    tamanho_casas = (window->getSize().x + window->getSize().y) / 2 / 8;

    int tamanho = (int)(tamanho_casas);
    tabuleiro->tamanho_casas = tamanho;
    bool casasPretas = true;

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            pos_x = x * tamanho;
            pos_y = y * tamanho;
            tabuleiroUI[y][x].setPrimitiveType(sf::PrimitiveType::Quads);
            tabuleiroUI[y][x].resize(qtdVertices);

            for (int i = 0; i < qtdVertices; i++)
            {
                int distance_x = (i == 1 || i == 2) ? tamanho : 0;
                int distance_y = (i == 3 || i == 2) ? tamanho : 0;

                tabuleiroUI[y][x][i].position = sf::Vector2f(pos_x + distance_x, pos_y + distance_y);
                tabuleiroUI[y][x][i].color = casasPretas ? sf::Color(111, 149, 81) : sf::Color::White;
            }
            casasPretas = !casasPretas;
        }
        casasPretas = !casasPretas;
    }
}

/**
 * @brief Atualiza o tabuleiro na tela
 * 
 * Desenha as peças e o tabuleiro na tela
 * 
 * @param gameC GameController responsavel pelo tabuleiro
 */
void GameUI::update(GameController* gameC)
{
    if (window != nullptr)
    {
        // Renderiza o tabuleiro se a dimensao da janela mudar
        if ((int)tamanho_casas != (int)((window->getSize().x + window->getSize().y)/2/8))
            renderTab();

        window->clear(sf::Color::Black);
        auto peças = gameC->tabuleiro->getTabuleiro();
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                window->draw(tabuleiroUI[i][j]);
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
                if (peças[y][x] != nullptr)
                    window->draw(peças[y][x]->objectUI);

        window->display();
    }
}

/**
 * @brief Mostra as casas possiveis para uma peça visualmente
 * 
 * @param gameC GameController responsavel pelo tabuleiro
 * @param movimentosPossiveis Vetor com as casas possiveis
 * 
 */
void GameUI::MostrarCasasPossiveis(GameController* gameC, std::vector<Jogada> movimentosPossiveis)
{
    sf::Color color;
    for (int i = 0; i < movimentosPossiveis.size(); i++)
    {
        int pos_x = movimentosPossiveis[i].new_pos.x, pos_y = movimentosPossiveis[i].new_pos.y;
        if (gameC->tabuleiro->getTabuleiro()[pos_y][pos_x] == nullptr)
            for (int j = 0; j < 4; j++)
            {
                if ((pos_y+1) % 2 == 0)
                    color = (pos_x+1) % 2 != 0 ? sf::Color(200, 200, 200) : sf::Color(89, 121, 64);
                else 
                    color = (pos_x+1) % 2 != 0 ? sf::Color(89, 121, 64) : sf::Color(200, 200, 200);
                tabuleiroUI[pos_y][pos_x][j].color = color;
            }
        else
            for (int j = 0; j < 4; j++)
                tabuleiroUI[pos_y][pos_x][j].color = sf::Color(209, 59, 59);
    }
}

/**
 * @brief Verifica se a peça foi movida com sucesso
 * 
 * Armazena a posicao incial da peça clicada, e enquanto o jogador pressionar 
 * e mover o mouse, atualiza o tabuleiro com a peça sendo movida
 * 
 * @param gameC GameController responsavel pelo tabuleiro
 * @return true se o jogador clicou em uma peça
 * @return True se o status de jogada foi sucedida False se nao
 * 
 */
bool GameUI::OnMouseButtonLeftPressed(GameController* gameC)
{
    // Converte a posicao do mouse para a posicao do tabuleiro
    int pos_x = (int)( sf::Mouse::getPosition(*window).x/tamanho_casas), pos_y = (int)(sf::Mouse::getPosition(*window).y/tamanho_casas);
    int new_pos_x, new_pos_y;

    if (gameC->tabuleiro->getTabuleiro()[pos_y][pos_x] == nullptr)
        return false;
    else if (gameC->tabuleiro->getTabuleiro()[pos_y][pos_x]->isWhite != gameC->tabuleiro->brancasPrimeiro)
        return false;

    // Filtrando os movimentos possiveis que nao deixam o rei em check
    std::vector<Jogada> movimentos = gameC->tabuleiro->getTabuleiro()[pos_y][pos_x]->movimentosPossiveis(gameC->tabuleiro), aux;
    std::copy_if(movimentos.begin(), movimentos.end(), std::back_inserter(aux), [gameC, pos_x, pos_y](Jogada movimento)
    {
        Tabuleiro test_tab = *gameC->tabuleiro;
        test_tab.moverPeça({pos_x, pos_y}, movimento.new_pos);

        sf::Vector2i rei_pos = dynamic_cast<Rei*>(test_tab.getTabuleiro()[movimento.new_pos.y][movimento.new_pos.x]) ? movimento.new_pos : gameC->posReiJogador;
        bool Check = (dynamic_cast<Rei*>(test_tab.getTabuleiro()[rei_pos.y][rei_pos.x])->isCheck(&test_tab, gameC->tabuleiro->brancasPrimeiro));
        return !Check;
    });

    // Se nao houver movimentos possiveis retorna false
    if (aux.empty())
    {
        renderTab();
        return false;
    }

    MostrarCasasPossiveis(gameC, aux);
	while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
        gameC->moverPeça({pos_x, pos_y}, {(float) sf::Mouse::getPosition(*window).x, (float) sf::Mouse::getPosition(*window).y}, sf::Mouse::isButtonPressed(sf::Mouse::Left));
        update(gameC);
	}

    // Convertendo a posicao do mouse para a posicao do tabuleiro
    new_pos_x = (int)( sf::Mouse::getPosition(*window).x/tamanho_casas), new_pos_y = (int)(sf::Mouse::getPosition(*window).y/tamanho_casas);

    // Verificando se o movimento foi valido
    if (auto it = std::find(aux.begin(), aux.end(), Jogada({pos_x, pos_y}, {new_pos_x, new_pos_y})); it != aux.end())
    {
        renderTab();
        return gameC->moverPeça({pos_x, pos_y}, {(float) sf::Mouse::getPosition(*window).x, (float) sf::Mouse::getPosition(*window).y}, sf::Mouse::isButtonPressed(sf::Mouse::Left));
    }
    
    // Se o movimento nao foi valido retorna false
    renderTab();
    gameC->tabuleiro->getTabuleiro()[pos_y][pos_x]->objectUI.setPosition({
            (float)(pos_x * tamanho_casas + (tamanho_casas-60)/2), 
            (float)(pos_y * tamanho_casas + (tamanho_casas-60)/2)
             });            
    return false;
}

/**
 * @brief Destrutor da classe GameUI
 */
GameUI::~GameUI()
{
    delete window;
}