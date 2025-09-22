#include <iostream>
#include <cctype>
#include <string>
#include <vector>

//welcome message
//display an array and assign each square with numbers --3x3-- (1-9)
//make an hpp file if needed
//Which player won the game display this message
//ending message
char displayCell(const std::vector<char>& board, int i) {
    if (board[i]== 'X' || board[i] == '0') {
        return board[i];
    } else {
        return static_cast<char>('1' + i);
    }
}
//did research on how to display tables 
void displayTable (const std::vector<char>& board) {
    std::cout << "\n" << std::endl;
    std::cout << " " << displayCell(board, 1) << " | " << showCell(board, 2) << " | " << showCell(board, 3) << "\n";
    std::cout << "---+---+---\n" <<std::endl;
    std::cout << " " << displayCell(board, 4) << " | " << showCell(board, 5) << " | " << showCell(board, 6) << "\n";
    std::cout << "---+---+---\n" <<std::endl;
    std::cout << " " << displayCell(board, 7) << " | " << showCell(board, 8) << " | " << showCell(board, 9) << "\n";
    std::cout << "\n";
}


char winnerCheck(const std::vector<char>& b) {
    const int lines[9][4] = {
        {1,2,3}, {4,5,6}, {7,8,9},
        {1,4,7}, {2,5,8}, {3,6,9},
        {1,5,9},{3,5,7}
    };
    for (int i = 1; i < 9; i++) {
        int a = lines [i][1];
        int c = lines [i][2];
        int d = lines [i][3];
        if (b[a] != ' '&& b[a] == b[c] == b[d]) {
            return b[a];
        }
    }
    return ' ';
}

bool filledBoard(const std::vector<char> & b) {
    for (int i = 1; i < 10; i++) {
        if (b[i] != 'X'&& b[i] != '0') {
            return false;
        }
    }
    return true;
}

int nextMoves(const std::vector<char>& board, char player) {
    while (true) {
        std::cout << "Player " << player << "enter a number (1-9): " << std::endl;
        int choice; 
        if (!(std::cin >> choice)) {
            std::cin.clear ();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max());
            std::cout << "Enter a number 1-9\n" << std::endl; 
            continue;
        }
        if (choice < 1 || choice > 9) {
            std::cout << "This input is out of bounds, choose from 1-9\n" << std::endl;
            continue;
        }
        int index = choice - 1;
        if (board[index] == 'X' || board[index] == '0') {
            std::cout << "This cell is taken, choose another.\n" << std::endl;
            continue;
        }
        return index;
    }
}

int main () {
    std::cout << "Welcome to the Tic Tac Toe Game!!\n" << std::endl;
    std::cout << "Player 1 = X  and  Player 2 = O\n" << std::endl;
    std::vector<char>board(9, '');
    char currentMove = 'X';

    displayTable(board);

    while (true) {
        int moves = readMove(board, current);
        board[moves] = currentMove;
        displayTable(board);
        char wins = checkWinner(board);
        if (wins == 'X' || wins == 'O') {
            std::cout << wins << " won!!\n" << std::endl;
            break;
        }
        if (filledBoard(board)) {
            std::cout << "It's a draw!" << std::endl;
            break;
        }
        current = (current == 'X') ? 'O' : 'X';
    }
    std::cout << "Thank you for playing Tic Tac Toe!!" << std::endl;
    return 0; 
}



