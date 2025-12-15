#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cctype>
#include "battle.hpp"

struct Character {
    std::string name;
    std::string archetype;
    int health;
    int attack;
    int defense;
    char mark;
};

// wouldnt work without this
std::string promptArchetype(int playerNum) {
    while (true) {
        std::cout << "Player " << playerNum
                  << " choose archetype (Paladin / Alchemist): ";
        std::string s;
        std::cin >> s;

        if (s == "Paladin" || s == "paladin") return "Paladin";
        if (s == "Alchemist" || s == "alchemist") return "Alchemist";

        std::cout << "Invalid archetype.\n";
    }
}

char promptForMove(int playerNum, char) {
    while (true) {
        std::cout << "Player " << playerNum
                  << " choose single-character mark: ";
        char c;
        std::cin >> c;
        if (std::isprint(c) && !std::isspace(c)) return c;
        std::cout << "Invalid mark.\n";
    }
}

// original
char showCell(const std::vector<char>& board, int i) {
    return board[i] != ' ' ? board[i] : static_cast<char>('1' + i);
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

int readMove(const std::vector<char>& board, char player) {
    while (true) {
        std::cout << "Player " << player << " enter move (1-9): ";
        int c;
        if (!(std::cin >> c)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (c < 1 || c > 9 || board[c - 1] != ' ') {
            std::cout << "Invalid move.\n";
            continue;
        }
        return c - 1;
    }
}

// story
void eventBetweenBattles(Character &player) {
    std::cout << "\nChoose an event:\n";
    std::cout << "1) Heal +5\n2) Attack +1\n3) Defense +1\nChoice: ";
    int c;
    std::cin >> c;

    if (c == 1) player.health += 5;
    else if (c == 2) player.attack += 1;
    else if (c == 3) player.defense += 1;

    std::cout << "Stats — HP: " << player.health
              << " ATK: " << player.attack
              << " DEF: " << player.defense << "\n";
}

// battle mode
bool playBattle(Character &player, Character &enemy, bool finalBoss = false) {
    std::vector<char> board(9, ' ');
    char current = player.mark;

    while (player.health > 0 && enemy.health > 0) {
        int idx;
        if (current == player.mark)
            idx = readMove(board, player.mark);
        else {
            do { idx = std::rand() % 9; } while (board[idx] != ' ');
        }

        board[idx] = current;
        displayTable(board);

        char winner = checkWinner(board);
        if (winner != ' ') {
            if (winner == player.mark) {
                int dmg = std::max(0, player.attack - enemy.defense);
                enemy.health -= dmg;
                std::cout << "You dealt " << dmg << " damage!\n";
            } else {
                int dmg = std::max(0, enemy.attack - player.defense);
                player.health -= dmg;
                std::cout << enemy.name << " dealt " << dmg << " damage!\n";
            }

            if (finalBoss && std::rand() % 2 == 0) {
                std::cout << "⚡ Zeus uses Thunderbolt! +2 damage ⚡\n";
                player.health -= 2;
            }

            board.assign(9, ' ');
        }

        current = (current == player.mark) ? enemy.mark : player.mark;
    }

    return player.health > 0;
}

// campaign
void playCampaign() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    Character player;
    std::cout << "Enter hero name: ";
    std::cin >> player.name;
    player.archetype = promptArchetype(1);
    player.mark = promptForMove(1, '\0');
    player.health = 20;
    player.attack = 5;
    player.defense = 2;

    std::vector<Character> gods = {
        {"Ares","God",10,4,1,'A'},
        {"Athena","God",12,5,2,'B'},
        {"Poseidon","God",14,6,3,'C'},
        {"Hades","God",16,7,3,'D'},
        {"Zeus","God",20,8,4,'Z'}
    };

    for (size_t i = 0; i < gods.size(); i++) {
        std::cout << "\n⚔ Battle against " << gods[i].name << " ⚔\n";
        bool won = playBattle(player, gods[i], i == gods.size() - 1);

        if (!won) {
            std::cout << "\n You were defeated. Campaign over.\n";
            return;
        }

        std::cout << gods[i].name << " defeated!\n";
        if (i < gods.size() - 1) eventBetweenBattles(player);
    }

    std::cout << "\nYOU DEFEATED ZEUS AND WON THE CAMPAIGN \n";
}
void playBattleMode() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    Character player;
    player.name = "Player";
    player.archetype = "Paladin";
    player.mark = promptForMove(1, '\0');
    player.health = 15;
    player.attack = 5;
    player.defense = 2;

    Character enemy;
    enemy.name = "Trainy";
    enemy.archetype = "God";
    enemy.mark = 'O';
    enemy.health = 15;
    enemy.attack = 4;
    enemy.defense = 1;

    std::cout << "\n Battle Mode \n";
    playBattle(player, enemy, false);
}


// regular
void playRegular() {
    std::vector<char> board(9, ' ');
    char cur = 'X';
    while (true) {
        int idx = readMove(board, cur);
        board[idx] = cur;
        displayTable(board);
        char w = checkWinner(board);
        if (w != ' ') { std::cout << w << " wins!\n"; break; }
        cur = (cur == 'X') ? 'O' : 'X';
    }
}

// main
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    while (true) {
        std::cout << "\nWelcome to Tic Tac Toe!\n";
        std::cout << "Choose game type:\n";
        std::cout << "1) Regular\n";
        std::cout << "2) Battle\n";
        std::cout << "3) Campaign\n";
        std::cout << "Enter choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) {
            playRegular();
        }
        else if (choice == 2) {
            playBattleMode();   
        }
        else if (choice == 3) {
            playCampaign();
        }
        else {
            std::cout << "Invalid choice.\n";
            continue;
        }

        std::cout << "\nPlay again? (y/n): ";
        char again;
        std::cin >> again;
        if (again != 'y' && again != 'Y') break;
    }

    std::cout << "Thanks for playing!\n";
    return 0;
}

