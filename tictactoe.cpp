#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>

//welcome message (done)
//display an array and assign each square with numbers --3x3-- (1-9) (done)
//make an hpp file if needed? (did not need)
//Which player won the game display this message (done)
//ending message (done)
char showCell(const std::vector<char>& board, int i) {
    if (board[i] != ' ') {
        return board[i];
    } else {
        return static_cast<char>('1' + i);
    }
}

void displayTable(const std::vector<char>& board) {

    std::cout << " " << showCell(board, 0) << " | " << showCell(board, 1) << " | " << showCell(board, 2) << std::endl;
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 3) << " | " << showCell(board, 4) << " | " << showCell(board, 5) << std::endl;
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 6) << " | " << showCell(board, 7) << " | " << showCell(board, 8) << std::endl;
    std::cout << "\n" << std::endl;
}

char checkWinner(const std::vector<char>& b) {
    const int lines[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
    for (int i = 0; i < 8; ++i) {
        int a = lines[i][0];
        int c = lines[i][1];
        int d = lines[i][2];
        if (b[a] != ' ' && b[a] == b[c] && b[c] == b[d]) {
            return b[a];
        }
    }
    return ' ';
}


int readMove(const std::vector<char>& board, char player) {
    while (true) {
        std::cout << "Player " << player << " — enter a move (1-9): "<< std::endl;
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Error: Please enter a number from 1 to 9.\n" << std::endl;
            continue;
        }
        if (choice < 1 || choice > 9) {
            std::cout << "Sorry out of bounds, choose 1-9.\n" << std::endl;
            continue;
        }
        int index = choice - 1;
        if (board[index] != ' ') {
            std::cout << "That cell is already taken. Choose another.\n" << std::endl;
            continue;
        }
        return index;
    }
}
bool boardFull(const std::vector<char>& b) {
    for (int i = 0; i < 9; ++i) {
        if (b[i] == ' ') {
            return false;
        }
    }
    return true;
}

int main() {
    std::cout << "Welcome to the Tic Tac Toe Game!!\n" << std::endl;
    std::cout << "Player 1 is X || Player 2 is O\n" << std::endl;

    std::vector<char> board(9, ' ');
    char current = 'X';

    displayTable(board);

    while (true) {
        int idx = readMove(board, current);
        board[idx] = current;
        displayTable(board);

        char winner = checkWinner(board);
        if (winner != ' ') {
            std::cout << winner << " won\n"<< std::endl;
            break;
        }
        if (boardFull(board)) {
            std::cout << "It's a draw!!\n" << std::endl;
            break;
        }
        if (current == 'X') {
            current = 'O';
        } else {
            current = 'X';
        }
    }

    std::cout << "Thank you for playing Tic Tac Toe!!\n"<< std::endl;
    return 0;
}