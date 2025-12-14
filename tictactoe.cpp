#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include <ctime>
#include "battle.hpp"


int readMove(const std::vector<char>& board, char mark) {
    while (true) {
        std::cout << "Player '" << mark << "' — enter a move (1-9): ";
        int n;
        if (!(std::cin >> n)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a NUMBER.\n";
            continue;
        }
        if (n < 1 || n > 9 || board[n-1] != ' ') {
            std::cout << "Invalid move.\n";
            continue;
        }
        return n - 1;
    }
}

void display(const std::vector<char>& b) {
    for (int i = 0; i < 9; i++) {
        std::cout << (b[i] == ' ' ? char('1'+i) : b[i]);
        if (i % 3 != 2) std::cout << " | ";
        if (i % 3 == 2 && i != 8) std::cout << "\n--+---+--\n";
    }
    std::cout << "\n\n";
}

char winner(const std::vector<char>& b) {
    int w[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
    for (auto &l : w)
        if (b[l[0]]!=' ' && b[l[0]]==b[l[1]] && b[l[1]]==b[l[2]])
            return b[l[0]];
    return ' ';
}

// campaign

void playCampaign() {
    std::string name;
    std::cout << "Enter name: ";
    std::cin >> name;

    std::cout << "Choose your mark (single character): ";
    char mark = promptForMove(1, '\0');

    std::vector<char> board(9,' ');
    char current = mark;

    while (true) {
        display(board);
        int move = readMove(board, current);
        board[move] = current;

        if (winner(board) != ' ') {
            display(board);
            std::cout << "You won the match!\n";
            break;
        }

        current = (current == mark ? 'X' : mark);
    }
}

//main

int main() {
    while (true) {
        std::cout << "1) campaign\nChoose: ";
        int c;
        std::cin >> c;

        if (c == 1) playCampaign();

        std::cout << "Play again? (y/n): ";
        char again;
        std::cin >> again;
        if (again != 'y' && again != 'Y') break;
    }
}