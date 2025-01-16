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
                case 'Q':
                    board[y][x] = new Rainha(isWhite, sf::Vector2i(x, y));
                    break;
                case 'R':
                    board[y][x] = new Rei(isWhite, sf::Vector2i(x, y));
                    break;
                default:
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
        { " ", " ", " ", " ", "WR", " ", " ", " " },
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
        { " ", " ", " ", "WP", "WR", "WP", " ", " " },
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
      { " ", " ", " ", " ", " ", " ", "BP", "BP" },
     { " ", " ", " ", " ", " ", "BP", "BR", "BP" },
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
    assert(movimentos.size() == 2+2+4);
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
        { " ", " ", " ", " ", " ", " ", " ", "WR" }
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
        { " ", " ", " ", "BQ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", "WQ" }
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

    tabuleiro = 
    {{
        { "BT", "BC", "BB", "BR", "BR", "BB", "BC", "BT" },
        { "BP", "BP", "BP", "BP", "BP", "BP", "BP", "BP" },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", "WP", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { "WP", " ", "WP", "WP", "WP", "WP", "WP", "WP" },
        { "WT", "WC", "WB", "WR", "WR", "WB", "WC", "WT" }
    }};

    board = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    movimentos = GameController::getPossiveisMovimentos(*tab, true);
    assert(movimentos.size() == 26);
    system("clear");

}
void TestarCheques()
{
    std::vector<Jogada> movimentos;
    Tabuleiro* tab;
    std::array<std::array<std::string, 8>, 8> tabuleiro = {{
        { "BT", "BC", "BB", "BQ", "BR", "BB", "BC", "BT" },
        { "BP", "BP", "BP", "BP", "BP", "BP", "BP", "BP" },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { " ", " ", " ", " ", "WP", " ", " ", " " },
        { " ", " ", " ", " ", " ", " ", " ", " " },
        { "WP", "WP", "WP", "WP", " ", "WP", "WP", "WP" },
        { "WT", "WC", "WB", "WQ", "WR", "WB", "WC", "WT" }
    }};
    Peça* (*board)[8] = convert_string_list_to_board_(tabuleiro);
    tab = new Tabuleiro(board, true, 100);
    GameController* gameController = new GameController(*tab, 100, sf::Vector2i(4, 7), sf::Vector2i(4, 0));
    bool isCheckForWhite = gameController->analisarCheck(true);
    assert(isCheckForWhite == false);
    bool isCheckForBlack = gameController->analisarCheck(false);
    assert(isCheckForBlack == false); 
    bool isCheckmateForWhite = gameController->analisarCheckmate(true);
    assert(isCheckmateForWhite == false);
    bool isCheckmateForBlack = gameController->analisarCheckmate(false);
    assert(isCheckmateForBlack == false);
}
int main() {
    TestarMovimentos();
    //TestarCheques();
}