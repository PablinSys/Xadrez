#include <cassert>
#include <iostream>
#include "../include/peças.hpp"
#include "../include/tabuleiro.hpp"
#include "../include/structs.hpp"
#include "../include/gameController.hpp"

Peça* (*convert_string_list_to_board_(const std::array<std::array<std::string, 8>, 8>& tab))[8]
{
    static Peça* board[8][8] = {}; 
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            board[y][x] = nullptr;

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            bool isWhite = tab[y][x][0] == 'W';

            if (tab[y][x].length() > 1)
                switch (tab[y][x][1])
                {
                case 'T':
                    board[y][x] = new Torre(isWhite, sf::Vector2i(x, y));
                    break;
                case 'C':
                    board[y][x] = new Cavalo(isWhite, sf::Vector2i(x, y));
                    break;
                case 'B':
                    board[y][x] = new Bispo(isWhite, sf::Vector2i(x, y));
                    break;
                case 'P':
                    board[y][x] = new Peao(isWhite, sf::Vector2i(x, y), isWhite ? y == 6 : y == 1);
                    break; 
                default:
                    if (tab[y][x].length() > 2)
                    {
                        if (tab[y][x][2] == 'E')
                            board[y][x] = new Rei(isWhite, sf::Vector2i(x, y));
                        else if (tab[y][x][2] == 'A')
                            board[y][x] = new Rainha(isWhite, sf::Vector2i(x, y));
                    }
                    else 
                        board[y][x] = nullptr;
                    break;
                }
        }
    }
    return board;
}
void TestarMovimentos()
{
    std::vector<Jogada> movimentos;
    Tabuleiro* tab;
    std::array<std::array<std::string, 8>, 8> tabuleiro = 
    {{
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", "WT", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " }
    }};
    Peça* (*board)[8] = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, true);
    assert(movimentos.size() == 4+4+3+3);
    system("clear");

    tabuleiro =
    {{
        { "BT", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, false);
    assert(movimentos.size() == 7+7);
    system("clear");

    tabuleiro =
    {{
        { "BT", " ", " ", " ", " ", " ", " ", "WT" },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { "WP", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, false);
    assert(movimentos.size() == 7+4);
    system("clear");

    tabuleiro =
    {{
        { " ", " ", " ", " ", "BT", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", "BP", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", "BB", " ", " ", "WT", " ", " ", " " }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, true);
    assert(movimentos.size() == 3+4+3);
    system("clear");

    tabuleiro =
    {{
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", "BP", " ", "BP", " ", " " },
        { " ", " ", " ", "WP", "WP", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, true);
    assert(movimentos.size() == 2+2);
    system("clear");

    tabuleiro =
    {{
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", "BP", " ", "BP", " ", " ", " " },
        { " ", " ", " ", "WP", " ", "WB", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, false);
    assert(movimentos.size() == 2+1+1+2+1);
    system("clear");

    tabuleiro = 
    {{
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", "WRE", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, true);
    assert(movimentos.size() == 2+2+2+2);
    system("clear");

    tabuleiro = 
    {{
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", "WP", " ", " ", " " },
        { " ", " ", " ", "WP", "WRE", "WP", " ", " " },
        { " ", " ", " ", " ", "WP", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, true);
    assert(movimentos.size() == 2+2+3);
    system("clear");

    tabuleiro = 
    {{
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", "WRE" }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, true);
    assert(movimentos.size() == 3);
    system("clear");

        tabuleiro = 
    {{
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", "BB" }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, false);
    assert(movimentos.size() == 7);
    system("clear");

    tabuleiro = 
    {{
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", "WB", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", "BB", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, true);
    assert(movimentos.size() == 3+3+3+2);
    movimentos = GameController::getPossiveisMovimentos(*tab, false);
    assert(movimentos.size() == 2+2+2+2);
    system("clear");

    tabuleiro = 
    {{
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", "BRA", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", "WRA" }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, true);
    assert(movimentos.size() == 7+7+4);
    movimentos = GameController::getPossiveisMovimentos(*tab, false);
    assert(movimentos.size() == 7+7+7+6);
    system("clear");

    tabuleiro = 
    {{
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", "WC", " ", " " },
        { " ", " ", " ", "BC", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", "WC", " ", " " },
        { " ", " ", " ", "WP", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", "" }
    }};
    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, true);
    assert(movimentos.size() == 7+8+2);
    movimentos = GameController::getPossiveisMovimentos(*tab, false);
    assert(movimentos.size() == 8);
    system("clear");

}
int main() {
    TestarMovimentos();
}