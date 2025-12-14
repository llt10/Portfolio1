#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "battle.hpp"

/* ===================== DISPLAY / BOARD ===================== */

char showCell(const std::vector<char>& board, int i) {
    if (board[i] != ' ') return board[i];
    return static_cast<char>('1' + i);
}

void displayTable(const std::vector<char>& board) {
    std::cout << " " << showCell(board,0) << " | " << showCell(board,1) << " | " << showCell(board,2) << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board,3) << " | " << showCell(board,4) << " | " << showCell(board,5) << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board,6) << " | " << showCell(board,7) << " | " << showCell(board,8) << "\n\n";
}

char checkWinner(const std::vector<char>& b) {
    const int lines[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
    for (int i = 0; i < 8; i++) {
        if (b[lines[i][0]] != ' ' &&
            b[lines[i][0]] == b[lines[i][1]] &&
            b[lines[i][1]] == b[lines[i][2]])
            return b[lines[i][0]];
    }
    return ' ';
}

bool boardFull(const std::vector<char>& b) {
    for (char c : b) if (c == ' ') return false;
    return true;
}

/* ===================== INPUT ===================== */

int readMove(const std::vector<char>& board, char player) {
    while (true) {
        std::cout << "Player " << player << " enter move (1-9): ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (choice < 1 || choice > 9) continue;
        if (board[choice-1] != ' ') continue;
        return choice - 1;
    }
}

/* ===================== REGULAR MODE ===================== */

void playRegular() {
    std::vector<char> board(9, ' ');
    char current = 'X';

    displayTable(board);

    while (true) {
        int idx = readMove(board, current);
        board[idx] = current;
        displayTable(board);

        char w = checkWinner(board);
        if (w != ' ') {
            std::cout << w << " wins!\n";
            break;
        }
        if (boardFull(board)) {
            std::cout << "Draw!\n";
            break;
        }
        current = (current == 'X') ? 'O' : 'X';
    }
}

/* ===================== ORIGINAL BATTLE MODE (UNCHANGED) ===================== */

void playBattle() {
    std::cout << "Starting Battle Mode\n";
    char p1Move = promptForMove(1,'\0');
    char p2Move = promptForMove(2,p1Move);
    std::string p1Arch = promptArchetype(1);
    std::string p2Arch = promptArchetype(2);

    std::vector<char> board(9,' ');
    char current = p1Move;

    displayTable(board);

    while (true) {
        int idx = readMove(board,current);
        board[idx] = current;
        displayTable(board);

        char w = checkWinner(board);
        if (w != ' ') {
            std::cout << w << " wins!\n";
            break;
        }
        if (boardFull(board)) {
            std::cout << "Draw!\n";
            break;
        }
        current = (current == p1Move) ? p2Move : p1Move;
    }
}

// campaign implement

struct Fighter {
    std::string name;
    int health;
    int attack;
    int defense;
};

int damageCalc(int atk, int def) {
    int dmg = atk - def;
    if (dmg < 0) dmg = 0;
    return dmg;
}

int randomMoveAI(const std::vector<char>& board) {
    std::vector<int> open;
    for (int i = 0; i < 9; i++)
        if (board[i] == ' ') open.push_back(i);
    return open[rand() % open.size()];
}

void playCampaign() {
    srand(static_cast<unsigned>(time(0)));

    Fighter player;
    std::cout << "Enter hero name: ";
    std::cin >> player.name;

    std::string arch = promptArchetype(1);
    if (arch == "paladin") {
        player.health = 35; player.attack = 6; player.defense = 4;
    } else {
        player.health = 28; player.attack = 7; player.defense = 3;
    }

    std::vector<Fighter> enemies = {
        {"Ares' Champion",18,5,2},
        {"Medusa",22,6,3},
        {"Minotaur",26,7,4},
        {"Hades' Shade",30,8,4}
    };

    for (size_t e = 0; e < enemies.size(); e++) {
        Fighter& enemy = enemies[e];
        std::cout << "\nTrial " << e+1 << ": " << enemy.name << "\n";

        while (player.health > 0 && enemy.health > 0) {
            std::vector<char> board(9,' ');
            char current = 'X';
            displayTable(board);

            while (true) {
                if (current == 'X') {
                    int idx = readMove(board,'X');
                    board[idx] = 'X';
                } else {
                    board[randomMoveAI(board)] = 'O';
                }

                displayTable(board);
                char w = checkWinner(board);

                if (w == 'X') {
                    enemy.health -= damageCalc(player.attack, enemy.defense);
                    break;
                }
                if (w == 'O') {
                    player.health -= damageCalc(enemy.attack, player.defense);
                    break;
                }
                if (boardFull(board)) break;

                current = (current == 'X') ? 'O' : 'X';
            }
        }

        if (player.health <= 0) {
            std::cout << "You fall. Olympus rejects you.\n";
            return;
        }

        std::cout << enemy.name << " defeated!\n";

        if (e < enemies.size()-1) {
            std::cout << "Godly blessing:\n";
            std::cout << "1) +2 Attack (Ares)\n2) +2 Defense (Athena)\n3) +8 Health (Apollo)\n";
            int c; std::cin >> c;
            if (c == 1) player.attack += 2;
            else if (c == 2) player.defense += 2;
            else if (c == 3) player.health += 8;
        }
    }

    Fighter zeus = {"Zeus, King of Olympus",40,9,5};
    std::cout << "\nFinal Trial: Zeus\n";

    while (player.health > 0 && zeus.health > 0) {
        if (rand()%2 == 0) {
            zeus.attack += 2;
            std::cout << "Zeus calls lightning!\n";
        } else {
            zeus.defense += 2;
            std::cout << "Zeus hardens his defense!\n";
        }

        std::vector<char> board(9,' ');
        char current = 'X';
        displayTable(board);

        while (true) {
            if (current == 'X') {
                board[readMove(board,'X')] = 'X';
            } else {
                board[randomMoveAI(board)] = 'O';
            }

            displayTable(board);
            char w = checkWinner(board);

            if (w == 'X') { zeus.health -= damageCalc(player.attack, zeus.defense); break; }
            if (w == 'O') { player.health -= damageCalc(zeus.attack, player.defense); break; }
            if (boardFull(board)) break;

            current = (current == 'X') ? 'O' : 'X';
        }
    }

    if (player.health > 0)
        std::cout << "You have earned the favor of Olympus!\n";
    else
        std::cout << "Zeus strikes you down.\n";
}

//main

int main() {
    while (true) {
        std::cout << "\nChoose game type:\n";
        std::cout << "1) Regular \n2) Battle\n3) Campaign\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) playRegular();
        else if (choice == 2) playBattle();
        else if (choice == 3) playCampaign();
        else continue;

        std::cout << "Play again? (y/n): ";
        char again; std::cin >> again;
        if (again != 'y' && again != 'Y') break;
    }

    std::cout << "Thanks for playing!\n";
    return 0;
}
