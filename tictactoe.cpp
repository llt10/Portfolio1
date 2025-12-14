#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include <ctime>
#include "battle.hpp"

struct Entity {
    std::string name;
    int hp;
    int atk;
    int def;
    char mark;
    bool boss;
};

char checkWinner(const std::vector<char>& b) {
    const int w[8][3] = {{0,1,2},{3,4,5},{6,7,8},
                         {0,3,6},{1,4,7},{2,5,8},
                         {0,4,8},{2,4,6}};
    for (auto &l : w)
        if (b[l[0]]!=' ' && b[l[0]]==b[l[1]] && b[l[1]]==b[l[2]])
            return b[l[0]];
    return ' ';
}

bool boardFull(const std::vector<char>& b) {
    for (char c : b) if (c == ' ') return false;
    return true;
}

int readMove(const std::vector<char>& board, char mark) {
    int m;
    while (true) {
        std::cout << "Player " << mark << " — enter a move (1-9): ";
        if (!(std::cin >> m)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        m--;
        if (m >= 0 && m < 9 && board[m] == ' ') return m;
    }
}

int aiMove(const std::vector<char>& board) {
    std::vector<int> e;
    for (int i = 0; i < 9; i++) if (board[i] == ' ') e.push_back(i);
    static std::mt19937 rng(time(nullptr));
    return e[rng() % e.size()];
}

char playMatch(char p, char a) {
    std::vector<char> board(9,' ');
    char turn = p;
    while (true) {
        int m = (turn == p) ? readMove(board,p) : aiMove(board);
        board[m] = turn;
        char w = checkWinner(board);
        if (w != ' ') return w;
        if (boardFull(board)) return 'D';
        turn = (turn == p ? a : p);
    }
}

int damage(int atk, int def) {
    int d = atk - def;
    return d < 0 ? 0 : d;
}

bool battle(Entity& player, Entity enemy) {
    while (player.hp > 0 && enemy.hp > 0) {
        char w = playMatch(player.mark, enemy.mark);
        if (w == player.mark) enemy.hp -= damage(player.atk, enemy.def);
        else if (w == enemy.mark) player.hp -= damage(enemy.atk, player.def);
        std::cout << player.name << " HP: " << player.hp
                  << " | " << enemy.name << " HP: " << enemy.hp << "\n\n";
    }
    return player.hp > 0;
}

void playCampaign() {
    Entity player;
    std::cout << "Enter name: ";
    std::cin >> player.name;
    player.hp = 50; player.atk = 8; player.def = 4;
    player.mark = promptForMove(0,'\0');

    std::vector<Entity> foes = {
        {"God",20,4,1,'G',false},
        {"Burglar",25,6,2,'B',false},
        {"Witch",30,7,3,'W',false},
        {"Icy",35,9,4,'I',false},
        {"Dragon",60,10,5,'D',true}
    };

    for (auto &e : foes) {
        if (!battle(player, e)) {
            std::cout << "Campaign failed.\n";
            return;
        }
    }
    std::cout << "You won the campaign!\n";
}

int main() {
    while (true) {
        std::cout << "1) campaign\nChoose: ";
        int c; std::cin >> c;
        if (c == 1) playCampaign();
        std::cout << "Play again? (y/n): ";
        char a; std::cin >> a;
        if (a != 'y' && a != 'Y') break;
    }
}





