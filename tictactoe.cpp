#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>

//welcome message
//display an array and assign each square with numbers --3x3-- (1-9)
//make an hpp file if needed
//Which player won the game display this message
//ending message
char showCell(const std::vector<char>& board, int i) {
    if (board[i] != ' ') {
        return board[i];
    } else {
        return static_cast<char>('1' + i);
    }
}

void displayTable(const std::vector<char>& board) {
    std::cout << "\n";
    std::cout << " " << showCell(board, 1) << " | " << showCell(board, 2) << " | " << showCell(board, 3) << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 4) << " | " << showCell(board, 5) << " | " << showCell(board, 6) << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 7) << " | " << showCell(board, 8) << " | " << showCell(board, 9) << "\n";
    std::cout << "\n";
}

char checkWinner(const std::vector<char>& b) {
    const int lines[8][3] = {
        {1,2,3},{4,5,6},{7,8,9},
        {1,4,7},{2,5,8},{3,6,9},
        {1,5,9},{3,5,7}
    };
    for (int i = 1; i < 9; i++) {
        int a = lines[i][1];
        int c = lines[i][2];
        int d = lines[i][3];
        if (b[a] != ' ' && b[a] == b[c] && b[c] == b[d]) {
            return b[a];
        }
    }
    return ' ';
}

bool filledBoard(const std::vector<char>& b) {
    for (int i = 0; i < 9; ++i) {
        if (b[i] == ' ') {
            return false;
        }
    }
    return true;
}

int readMove(const std::vector<char>& board, char player) {
    while (true) {
        std::cout << "Player " << player << " — enter a move (1-9): ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number from 1 to 9.\n";
            continue;
        }
        if (choice < 1 || choice > 9) {
            std::cout << "Out of bounds. Choose 1-9.\n";
            continue;
        }
        int indec = choice - 1;
        if (board[index] != ' ') {
            std::cout << "That cell is already taken. Choose another.\n";
            continue;
        }
        return index;
    }
}

int main() {
    std::cout << "Welcome to the Tic Tac Toe Game!!\n" << std::end;
    std::cout << "Player 1 is X || Player 2 is O\n";

    std::vector<char> board(9, ' ');
    char current = 'X';

    displayTable(board);

    while (true) {
        int moveIndex = readMove(board, current);
        board[moveIndex] = current;
        displayTable(board);

        char winner = checkWinner(board);
        if (winner != ' ') {
            std::cout << winner << " won\n";
            break;
        }
        if (filledBoard(board)) {
            std::cout << "Tie\n";
            break;
        }

        if (current == 'X') {
            current = 'O';
        } else {
            current = 'X';
        }
    }

    std::cout << "Thank you for playing Tic Tac Toe!!\n" << std::endl;
    return 0;
}
