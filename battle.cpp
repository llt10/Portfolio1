#include "battle.hpp"
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <random>
#include <ctime>

bool apprMoveChar(char c) {
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
        if (s.size() != 1) continue;
        char c = s[0];
        if (!apprMoveChar(c)) continue;
        if (otherPlayerMark != '\0' && c == otherPlayerMark) continue;
        return c;
    }
}

std::string toLowerLocal(const std::string& s) {
    std::string out = s;
    for (char &c : out) c = static_cast<char>(std::tolower((unsigned char)c));
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
        std::string low = toLowerLocal(s);
        if (low == "paladin" || low == "alchemist" || low == "chronomage") return low;
    }
}

int countMoves(const std::vector<char>& board) {
    int cnt = 0;
    for (char c : board) if (c != ' ') ++cnt;
    return cnt;
}

bool isAdjacent(int from, int to) {
    int fr = from / 3;
    int fc = from % 3;
    int tr = to / 3;
    int tc = to % 3;
    int dr = abs(fr - tr);
    int dc = abs(fc - tc);
    return (dr <= 1 && dc <= 1) && !(dr == 0 && dc == 0);
}

int aiRandomMove(const std::vector<char>& board) {
    std::vector<int> empties;
    for (int i = 0; i < 9; ++i) if (board[i] == ' ') empties.push_back(i);
    if (empties.empty()) return -1;
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, (int)empties.size() - 1);
    return empties[dist(rng)];
}

bool alchemistSwap(std::vector<char>& board) {
    int a;
    std::cout << "Enter first position to swap (1-9): ";
    if (!(std::cin >> a)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (a < 1 || a > 9) return false;
    int i = a - 1;
    if (board[i] == ' ') return false;
    int b;
    std::cout << "Enter second position to swap (1-9): ";
    if (!(std::cin >> b)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (b < 1 || b > 9) return false;
    int j = b - 1;
    if (board[j] == ' ') return false;
    if (board[i] == board[j]) return false;
    std::swap(board[i], board[j]);
    return true;
}

bool alchemSwap(std::vector<char>& board) {
    return alchemistSwap(board);
}

bool paladinShift(std::vector<char>& board) {
    int a;
    std::cout << "Enter the position of the mark to shift (1-9): ";
    if (!(std::cin >> a)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (a < 1 || a > 9) return false;
    int from = a - 1;
    if (board[from] == ' ') return false;
    int b;
    std::cout << "Enter the destination position (1-9): ";
    if (!(std::cin >> b)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (b < 1 || b > 9) return false;
    int to = b - 1;
    if (board[to] != ' ') return false;
    if (!isAdjacent(from, to)) return false;
    board[to] = board[from];
    board[from] = ' ';
    return true;
}
