#include "battle.hpp"
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>

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

static std::string toLowerLocal(const std::string& s) {
    std::string out = s;
    for (char &c : out) c = static_cast<char>(tolower(c));
    return out;
}

std::string promptArchetype(int playerNumber) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose archetype (Paladin or Alchemist): ";
        std::string s;
        std::cin >> s;
        std::string low = toLowerLocal(s);
        if (low == "paladin" || low == "alchemist") return low;
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
    return abs(fr - tr) <= 1 && abs(fc - tc) <= 1 && from != to;
}

bool alchemSwap(std::vector<char>& board) {
    int a, b;
    std::cout << "Swap first position: ";
    if (!(std::cin >> a)) return false;
    std::cout << "Swap second position: ";
    if (!(std::cin >> b)) return false;
    a--; b--;
    if (a < 0 || b < 0 || a > 8 || b > 8) return false;
    if (board[a] == ' ' || board[b] == ' ') return false;
    std::swap(board[a], board[b]);
    return true;
}

bool paladinShift(std::vector<char>& board) {
    int a, b;
    std::cout << "Shift from: ";
    if (!(std::cin >> a)) return false;
    std::cout << "Shift to: ";
    if (!(std::cin >> b)) return false;
    a--; b--;
    if (a < 0 || b < 0 || a > 8 || b > 8) return false;
    if (board[a] == ' ' || board[b] != ' ') return false;
    if (!isAdjacent(a, b)) return false;
    board[b] = board[a];
    board[a] = ' ';
    return true;
}
