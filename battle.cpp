#include "battle.hpp"
#include <iostream>
#include <limits>
#include <cctype>
#include <random>
#include <ctime>

bool apprMoveChar(char c) {
    if (std::isalpha(c)) return true;
    std::string allowed = "?!*~$%#";
    return allowed.find(c) != std::string::npos;
}

char promptForMove(int playerNumber, char otherPlayerMark) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose a single-character mark: ";
        std::string s;
        std::cin >> s;
        if (s.size() != 1) continue;
        char c = s[0];
        if (!apprMoveChar(c)) continue;
        if (otherPlayerMark != '\0' && c == otherPlayerMark) continue;
        return c;
    }
}

static std::string toLower(const std::string& s) {
    std::string out = s;
    for (char &c : out) c = std::tolower(c);
    return out;
}

std::string promptArchetype(int playerNumber) {
    while (true) {
        std::cout << "Player " << playerNumber
                  << " choose archetype (Paladin/Alchemist): ";
        std::string s;
        std::cin >> s;
        s = toLower(s);
        if (s == "paladin" || s == "alchemist") return s;
    }
}

int countMoves(const std::vector<char>& board) {
    int c = 0;
    for (char x : board) if (x != ' ') c++;
    return c;
}

bool isAdjacent(int from, int to) {
    int r1 = from / 3, c1 = from % 3;
    int r2 = to / 3, c2 = to % 3;
    return std::abs(r1 - r2) <= 1 && std::abs(c1 - c2) <= 1 && from != to;
}

bool alchemSwap(std::vector<char>& board) {
    int a, b;
    std::cout << "Swap first position (1-9): ";
    std::cin >> a;
    std::cout << "Swap second position (1-9): ";
    std::cin >> b;
    a--; b--;
    if (a < 0 || b < 0 || a > 8 || b > 8) return false;
    if (board[a] == ' ' || board[b] == ' ') return false;
    std::swap(board[a], board[b]);
    return true;
}

bool paladinShift(std::vector<char>& board) {
    int a, b;
    std::cout << "Move from (1-9): ";
    std::cin >> a;
    std::cout << "Move to (1-9): ";
    std::cin >> b;
    a--; b--;
    if (a < 0 || b < 0 || a > 8 || b > 8) return false;
    if (board[a] == ' ' || board[b] != ' ') return false;
    if (!isAdjacent(a, b)) return false;
    board[b] = board[a];
    board[a] = ' ';
    return true;
}
