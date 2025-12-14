#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include "battle.hpp"

void playRegular();
void playBattle();
void playCampaign();

char showCell(const std::vector<char>& b, int i) {
    return b[i] == ' ' ? char('1' + i) : b[i];
}

void display(const std::vector<char>& b) {
    for (int i = 0; i < 9; i += 3) {
        std::cout << " " << showCell(b,i) << " | "
                  << showCell(b,i+1) << " | "
                  << showCell(b,i+2) << "\n";
        if (i < 6) std::cout << "---+---+---\n";
    }
    std::cout << "\n";
}

char winner(const std::vector<char>& b) {
    int w[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},
                   {1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for (auto &l : w)
        if (b[l[0]] != ' ' && b[l[0]] == b[l[1]] && b[l[1]] == b[l[2]])
            return b[l[0]];
    return ' ';
}

int readMove(const std::vector<char>& b, char p) {
    int x;
    while (true) {
        std::cout << "Player " << p << " — enter a move (1-9): ";
        std::cin >> x;
        if (x >= 1 && x <= 9 && b[x-1] == ' ') return x-1;
    }
}

void playRegular() {
    std::vector<char> b(9, ' ');
    char p = 'X';
    while (true) {
        display(b);
        b[readMove(b,p)] = p;
        if (winner(b) != ' ') {
            display(b);
            std::cout << p << " wins!\n";
            return;
        }
        if (countMoves(b) == 9) {
            display(b);
            std::cout << "Draw!\n";
            return;
        }
        p = (p == 'X') ? 'O' : 'X';
    }
}

void playBattle() {
    std::vector<char> board(9, ' ');

    char p1 = promptForMove(1, '\0');
    char p2 = promptForMove(2, p1);

    char current = p1;

    while (true) {
        display(board);

        if (current == p1) {
            board[readMove(board, p1)] = p1;
        } else {
            board[readMove(board, p2)] = p2;
        }

        char w = winner(board);
        if (w != ' ') {
            display(board);
            std::cout << "Player " << w << " wins!\n";
            return;
        }

        if (countMoves(board) == 9) {
            display(board);
            std::cout << "It's a draw!\n";
            return;
        }

        current = (current == p1) ? p2 : p1;
    }
}

struct Entity {
    std::string name;
    int hp, atk, def;
    char mark;
};

int aiMove(const std::vector<char>& b) {
    std::vector<int> e;
    for (int i=0;i<9;i++) if (b[i]==' ') e.push_back(i);
    static std::mt19937 g(time(0));
    return e[g()%e.size()];
}

void playCampaign() {
    Entity p;
    std::cout << "Enter name: ";
    std::cin >> p.name;
    p.hp = 50; p.atk = 8; p.def = 4;
    p.mark = promptForMove(0,'\0');

    Entity enemy{"Zeus",30,6,2,'Z'};

    while (p.hp > 0 && enemy.hp > 0) {
        std::vector<char> b(9,' ');
        char turn = p.mark;
        while (true) {
            if (turn == p.mark)
                b[readMove(b,p.mark)] = p.mark;
            else
                b[aiMove(b)] = enemy.mark;

            display(b);
            char w = winner(b);
            if (w != ' ' || countMoves(b)==9) {
                if (w == p.mark) enemy.hp -= std::max(0,p.atk-enemy.def);
                if (w == enemy.mark) p.hp -= std::max(0,enemy.atk-p.def);
                break;
            }
            turn = (turn==p.mark)?enemy.mark:p.mark;
        }
    }
    std::cout << (p.hp>0?"Campaign complete!\n":"You died.\n");
}

int main() {
    while (true) {
        std::cout << "1) regular\n2) battle\n3) campaign\nChoose: ";
        int c;
        std::cin >> c;
        if (c==1) playRegular();
        else if (c==2) playBattle();
        else if (c==3) playCampaign();
        else continue;

        std::cout << "Play again? (y/n): ";
        char a; std::cin >> a;
        if (a!='y' && a!='Y') break;
    }
}