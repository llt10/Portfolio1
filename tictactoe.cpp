#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>

#include "battle.hpp"

struct Character {
    std::string name;
    std::string archetype;
    int health;
    int attack;
    int defense;
};

char showCell(const std::vector<char>& board, int i) {
    return board[i] != ' ' ? board[i] : static_cast<char>('1' + i);
}

void displayTable(const std::vector<char>& board) {
    std::cout << " " << showCell(board, 0) << " | " << showCell(board, 1) << " | " << showCell(board, 2) << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 3) << " | " << showCell(board, 4) << " | " << showCell(board, 5) << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 6) << " | " << showCell(board, 7) << " | " << showCell(board, 8) << "\n\n";
}

char checkWinner(const std::vector<char>& b) {
    const int lines[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
    for (int i = 0; i < 8; ++i) {
        int a = lines[i][0], c = lines[i][1], d = lines[i][2];
        if (b[a] != ' ' && b[a] == b[c] && b[c] == b[d]) return b[a];
    }
    return ' ';
}

int readMove(const std::vector<char>& board, char player) {
    while (true) {
        std::cout << "Player " << player << " enter move (1-9): ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter a number 1-9.\n";
            continue;
        }
        if (choice < 1 || choice > 9) {
            std::cout << "Out of bounds. Choose 1-9.\n";
            continue;
        }
        int idx = choice - 1;
        if (board[idx] != ' ') {
            std::cout << "Cell occupied. Choose another.\n";
            continue;
        }
        return idx;
    }
}

bool boardFull(const std::vector<char>& b) {
    for (char c : b) if (c == ' ') return false;
    return true;
}

int whichPlayerForMove(char mark, char p1Move, char p2Move) {
    if (mark == p1Move) return 1;
    if (mark == p2Move) return 2;
    return 0;
}

bool validMarkChar(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    std::string allowed = "?!*~$%#";
    return allowed.find(c) != std::string::npos;
}

char promptForMove(int playerNumber, char otherPlayerMark) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose a single-character mark: ";
        std::string s;
        if (!(std::cin >> s)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (s.size() != 1) {
            std::cout << "Enter exactly one character.\n";
            continue;
        }
        char c = s[0];
        if (!validMarkChar(c)) {
            std::cout << "Invalid mark. Choose A-Z, a-z, or one of ? ! * ~ $ % #\n";
            continue;
        }
        if (otherPlayerMark != '\0' && c == otherPlayerMark) {
            std::cout << "That mark is already taken by the other player.\n";
            continue;
        }
        return c;
    }
}

std::string toLower(const std::string& s) {
    std::string out = s;
    for (char &c : out) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

std::string promptArchetype(int playerNumber) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose archetype (Paladin, Alchemist, Chronomage): ";
        std::string s;
        if (!(std::cin >> s)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::string low = toLower(s);
        if (low == "paladin" || low == "alchemist" || low == "chronomage") return low;
        std::cout << "Invalid archetype.\n";
    }
}

int countMoves(const std::vector<char>& board) {
    int cnt = 0;
    for (char c : board) if (c != ' ') ++cnt;
    return cnt;
}

bool isAdjacent(int from, int to) {
    int fr = from / 3, fc = from % 3;
    int tr = to / 3, tc = to % 3;
    int dr = abs(fr - tr), dc = abs(fc - tc);
    return (dr <= 1 && dc <= 1) && !(dr == 0 && dc == 0);
}

bool paladinShift(std::vector<char>& board) {
    std::cout << "Enter the position of the mark to shift (1-9): ";
    int a;
    if (!(std::cin >> a)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (a < 1 || a > 9) return false;
    int from = a - 1;
    if (board[from] == ' ') { std::cout << "No mark at that position.\n"; return false; }
    std::cout << "Enter the destination position (1-9): ";
    int b;
    if (!(std::cin >> b)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (b < 1 || b > 9) return false;
    int to = b - 1;
    if (board[to] != ' ') { std::cout << "Destination not empty.\n"; return false; }
    if (!isAdjacent(from, to)) { std::cout << "Destination not adjacent.\n"; return false; }
    std::swap(board[from], board[to]);
    return true;
}

bool alchemistSwap(std::vector<char>& board) {
    std::cout << "Enter first position to swap (1-9): ";
    int a; if (!(std::cin >> a)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (a < 1 || a > 9) return false;
    int i = a - 1;
    if (board[i] == ' ') { std::cout << "No mark at first position.\n"; return false; }
    std::cout << "Enter second position to swap (1-9): ";
    int b; if (!(std::cin >> b)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (b < 1 || b > 9) return false;
    int j = b - 1;
    if (board[j] == ' ') { std::cout << "No mark at second position.\n"; return false; }
    if (board[i] == board[j]) { std::cout << "Cannot swap identical marks.\n"; return false; }
    std::swap(board[i], board[j]);
    return true;
}

// -- Regular Tic Tac Toe --
void playRegular() {
    std::vector<char> board(9, ' ');
    char current = 'X';
    displayTable(board);
    while (true) {
        int idx = readMove(board, current);
        board[idx] = current;
        displayTable(board);
        char winner = checkWinner(board);
        if (winner != ' ') {
            std::cout << winner << " wins!\n";
            break;
        }
        if (boardFull(board)) {
            std::cout << "It's a draw!\n";
            break;
        }
        current = (current == 'X') ? 'O' : 'X';
    }
}

// -- Battle & Campaign modes can be added below (following previous corrections) --

// Main function
int main() {
    std::srand(std::time(nullptr));
    std::cout << "Welcome to Tic Tac Toe!\n";
    while (true) {
        std::cout << "Choose game type:\n1) Regular\n2) Battle\n3) Campaign\nEnter choice: ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) playRegular();
        else if (choice == 2) { std::cout << "Battle mode placeholder\n"; }
        else if (choice == 3) { std::cout << "Campaign mode placeholder\n"; }
        else continue;

        std::cout << "Play again? (y/n): ";
        char again;
        if (!(std::cin >> again)) break;
        if (again != 'y' && again != 'Y') break;
    }
    std::cout << "Thanks for playing!\n";
    return 0;
}




