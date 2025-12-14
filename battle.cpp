#include "battle.hpp"
#include <iostream>
#include <limits>
#include <random>
#include <ctime>
#include <cctype>
#include <algorithm>

bool apprMoveChar(char c) {
    if (std::isalpha(static_cast<unsigned char>(c))) return true;
    std::string allowed = "?!*~$%#";
    return allowed.find(c) != std::string::npos;
}

char promptForMove(int playerNumber, char otherPlayerMark) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose a single-character mark: ";
        std::string s;
        std::cin >> s;

        if (s.size() != 1) {
            std::cout << "Enter exactly ONE character.\n";
            continue;
        }

        char c = s[0];
        if (!apprMoveChar(c)) {
            std::cout << "Invalid character.\n";
            continue;
        }

        if (otherPlayerMark != '\0' && c == otherPlayerMark) {
            std::cout << "That mark is already taken.\n";
            continue;
        }

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
                  << " choose archetype (Paladin, Alchemist, Chronomage): ";
        std::string s;
        std::cin >> s;

        std::string low = toLower(s);
        if (low == "paladin" || low == "alchemist" || low == "chronomage")
            return low;

        std::cout << "Invalid archetype.\n";
    }
}

int countMoves(const std::vector<char>& board) {
    int c = 0;
    for (char x : board) if (x != ' ') c++;
    return c;
}

bool isAdjacent(int from, int to) {
    int fr = from / 3, fc = from % 3;
    int tr = to / 3, tc = to % 3;
    return std::abs(fr - tr) <= 1 && std::abs(fc - tc) <= 1 && from != to;
}

bool alchemSwap(std::vector<char>& board) {
    int a, b;
    std::cout << "Swap first position (1-9): ";
    std::cin >> a;
    std::cout << "Swap second position (1-9): ";
    std::cin >> b;

    a--; b--;
    if (a < 0 || a > 8 || b < 0 || b > 8) return false;
    if (board[a] == ' ' || board[b] == ' ') return false;
    if (board[a] == board[b]) return false;

    std::swap(board[a], board[b]);
    return true;
}

bool paladinShift(std::vector<char>& board) {
    int from, to;
    std::cout << "Shift from (1-9): ";
    std::cin >> from;
    std::cout << "Shift to (1-9): ";
    std::cin >> to;

    from--; to--;
    if (from < 0 || from > 8 || to < 0 || to > 8) return false;
    if (board[from] == ' ' || board[to] != ' ') return false;
    if (!isAdjacent(from, to)) return false;

    board[to] = board[from];
    board[from] = ' ';
    return true;
}