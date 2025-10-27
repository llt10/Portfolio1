#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>
#include "battle.hpp"

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


//implementing the battlefield game 

void playBattlefield() 
    std::cout << "Starting Battle Mode" << srd::endl;

    char p1Move = promptForMove(1,'\0');
    char p2Move = promptForMove(2, p1Move);

    std::string p1Arche = promptArchetype(1);
    std::string p2Arche = promptArchetype(2);

    std::vector<char> board (9, ' ');
    char current = p1Move;

    std::cout << "Player 1: " << p1Move << " (" << p1Arche << ")\n" << std::endl;
    std::cout << "Player 2: " << p2Move << " (" << p2Arche << ")\n" << std::endl;

    displayTable(board);

    while (true) {
        int playerNum = (current == p1Move) ? 1: 2;
        std::string arche = (playerNum == 1) ? p1Arche : p2Arche;
        std::cout << " Player " << playerNum << " (" << current << ")'s turn. Archetype: " << arche << "\n" << std::endl;

        bool didAction = false;
        if (arche == "alchemist" || arche == "paladin" || arche == "chronomage" ) {
            std::cout << "Choose action:\n Regular move\n Special move\n" << std::endl;
            std::cout << "Enter 1 or 2: " << std::endl;
            int choice;
            if (!(std::cin>> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Wrong input (Default = Move)\n" <<std::endl;
                choice = 1;
            }
            if (choice ==1) {
                int idx = readMove(board, current);
                board[idx] = current;
                didAction = true;
            } else {
                if (arche == "alchemist") {
                    if (countMoves(board) < 2) {
                        std::cout << "Not enough moves to swap." << std::endl;
                        int idx = readMove(board, current);
                        board[idx] = current;
                        didAction = true;
                    } else {
                        didAction = true;
                        } 
                    }
                } else if (arche == "paladin") {
                    if (countMarksOnBoard(board) < 1) {
                        std::cout << "No marks on board to shift. Choose a regular move instead.\n";
                        int idx = readMove(board, current);
                        board[idx] = current;
                        didAction = true;
                    } else {
                        bool shifted = paladinShift(board);
                        if (!shifted) {
                            std::cout << "Shift was not successful; performing regular move instead.\n";
                            int idx = readMove(board, current);
                            board[idx] = current;
                            didAction = true;
                        } else {
                            didAction = true;
                        }
                    }
                } else {
                    std::cout << "Chronomage ability not yet implemented. Performing regular move.\n";
                    int idx = readMove(board, current);
                    board[idx] = current;
                    didAction = true;
                }
            }
        } else {
            int idx = readMove(board, current);
            board[idx] = current;
            didAction = true;
        }

        displayTable(board);
        char winner = checkWinner(board);
        if (winner != ' ') {
            int who = whichPlayerForMark(winner, p1Mark, p2Mark);
            if (who == 1) {
                std::cout << "Player 1 (" << p1Mark << ") won\n" << std::endl;
            } else if (who == 2) {
                std::cout << "Player 2 (" << p2Mark << ") won\n" << std::endl;
            } else {
                std::cout << winner << " won\n" << std::endl;
            }
            break;
        }
        if (boardFull(board)) {
            std::cout << "It's a draw!!\n" << std::endl;
            break;
        }
        if (current == p1Mark) current = p2Mark;
        else current = p1Mark;
    }


int main() {
    std::cout << "Welcome to the Tic Tac Toe Game!!\n" << std::endl;

    while (true) {
        std::cout << "Choose game type:\n1) regular\n2) battle\nEnter 1 or 2: ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again.\n" << std::endl;
            continue;
        }

        if (choice == 1) {
            playRegularGame();
        } else if (choice == 2) {
            playBattleGame();
        } else {
            std::cout << "Please enter 1 or 2.\n" << std::endl;
            continue;
        }

        
        std::cout << "Play again? (y/n): ";
        char again;
        if (!(std::cin >> again)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
        if (again == 'y' || again == 'Y') {
            std::cout << "\nRestarting...\n" << std::endl;
            continue;
        } else {
            break;
        }
    }

    std::cout << "Thank you for playing Tic Tac Toe/Battlefield!!\n"<< std::endl;
    return 0;
}

