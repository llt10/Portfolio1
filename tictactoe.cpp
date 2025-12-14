#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "battle.hpp"

struct Entity {
    std::string name;
    int hp, atk, def;
    char mark;
    bool boss;
};

void draw(const std::vector<char>& b) {
    for (int i = 0; i < 9; i++) {
        std::cout << (b[i] == ' ' ? char('1' + i) : b[i]);
        if (i % 3 != 2) std::cout << " | ";
        if (i % 3 == 2 && i != 8) std::cout << "\n--+---+--\n";
    }
    std::cout << "\n\n";
}

char winner(const std::vector<char>& b) {
    int w[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for (auto &x : w)
        if (b[x[0]] != ' ' && b[x[0]] == b[x[1]] && b[x[1]] == b[x[2]])
            return b[x[0]];
    return ' ';
}

int aiMove(const std::vector<char>& b) {
    std::vector<int> e;
    for (int i = 0; i < 9; i++) if (b[i] == ' ') e.push_back(i);
    static std::mt19937 r(time(0));
    return e[r() % e.size()];
}

char playMatch(char p, char ai) {
    std::vector<char> b(9, ' ');
    char cur = p;
    while (true) {
        if (cur == p) {
            int m;
            std::cin >> m;
            m--;
            if (m < 0 || m > 8 || b[m] != ' ') continue;
            b[m] = p;
        } else b[aiMove(b)] = ai;
        draw(b);
        char w = winner(b);
        if (w != ' ') return w;
        if (countMoves(b) == 9) return 'D';
        cur = (cur == p ? ai : p);
    }
}

void playRegular() {
    char w = playMatch('X', 'O');
    if (w == 'D') std::cout << "Draw\n";
    else std::cout << w << " wins\n";
}

void playBattle() {
    char p = promptForMove(1, '\0');
    char e = promptForMove(2, p);
    playMatch(p, e);
}

void playCampaign() {
    Entity player;
    std::cout << "Name: ";
    std::cin >> player.name;
    player.mark = promptForMove(0, '\0');
    player.hp = 50; player.atk = 8; player.def = 4;

    std::vector<Entity> gods = {
        {"Hermes",20,5,1,'H',false},
        {"Ares",30,7,2,'A',false},
        {"Athena",35,6,4,'T',false},
        {"Hades",45,8,5,'D',false},
        {"Zeus",60,10,6,'Z',true}
    };

    for (auto &g : gods) {
        while (player.hp > 0 && g.hp > 0) {
            char w = playMatch(player.mark, g.mark);
            if (w == player.mark) g.hp -= std::max(0, player.atk - g.def);
            else if (w == g.mark) player.hp -= std::max(0, g.atk - player.def);
        }
        if (player.hp <= 0) {
            std::cout << "You lost\n";
            return;
        }
    }
    std::cout << "You defeated Zeus and won the campaign\n";
}

int main() {
    while (true) {
        std::cout << "1) regular\n2) battle\n3) campaign\nChoose: ";
        int c;
        std::cin >> c;
        if (c == 1) playRegular();
        else if (c == 2) playBattle();
        else if (c == 3) playCampaign();
        std::cout << "Play again? (y/n): ";
        char a;
        std::cin >> a;
        if (a != 'y') break;
    }
}