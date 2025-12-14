#include "battle.hpp"
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <cctype>

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
        if (s.size() != 1) continue;
        char c = s[0];
        if (!apprMoveChar(c)) continue;
        if (c == otherPlayerMark) continue;
        return c;
    }
}

std::string promptArchetype(int playerNumber) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose archetype (Paladin or Alchemist): ";
        std::string s;
        std::cin >> s;
        for (char &c : s) c = std::tolower(c);
        if (s == "paladin" || s == "alchemist") return s;
    }
}

int countMoves(const std::vector<char>& board) {
    int c = 0;
    for (char x : board) if (x != ' ') c++;
    return c;
}

bool isAdjacent(int a, int b) {
    int ar = a / 3, ac = a % 3;
    int br = b / 3, bc = b % 3;
    return std::abs(ar - br) <= 1 && std::abs(ac - bc) <= 1 && a != b;
}

bool alchemSwap(std::vector<char>& board) {
    int a, b;
    std::cin >> a >> b;
    a--; b--;
    if (a < 0 || a > 8 || b < 0 || b > 8) return false;
    if (board[a] == ' ' || board[b] == ' ') return false;
    std::swap(board[a], board[b]);
    return true;
}

bool paladinShift(std::vector<char>& board) {
    int a, b;
    std::cin >> a >> b;
    a--; b--;
    if (a < 0 || a > 8 || b < 0 || b > 8) return false;
    if (board[a] == ' ' || board[b] != ' ') return false;
    if (!isAdjacent(a, b)) return false;
    board[b] = board[a];
    board[a] = ' ';
    return true;
}
