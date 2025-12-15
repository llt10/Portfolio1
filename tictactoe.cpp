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

/* =========================================================
   REQUIRED FUNCTIONS (FIXES LINKER ERROR)
   ========================================================= */

std::string promptArchetype(int playerNum) {
    while (true) {
        std::cout << "Player " << playerNum
                  << " choose archetype (Paladin / Alchemist): ";
        std::string choice;
        std::cin >> choice;

        if (choice == "Paladin" || choice == "paladin")
            return "Paladin";
        if (choice == "Alchemist" || choice == "alchemist")
            return "Alchemist";

        std::cout << "Invalid archetype.\n";
    }
}

char promptForMove(int playerNum, char) {
    while (true) {
        std::cout << "Player " << playerNum
                  << " choose single-character mark: ";
        char m;
        std::cin >> m;

        if (std::isprint(m) && !std::isspace(m))
            return m;

        std::cout << "Invalid mark.\n";
    }
}

/* =========================================================
   TIC TAC TOE CORE
   ========================================================= */

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
    const int wins[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };

    for (int i = 0; i < 8; i++) {
        if (b[wins[i][0]] != ' ' &&
            b[wins[i][0]] == b[wins[i][1]] &&
            b[wins[i][1]] == b[wins[i][2]])
            return b[wins[i][0]];
    }
    return ' ';
}

bool boardFull(const std::vector<char>& b) {
    for (char c : b)
        if (c == ' ')
            return false;
    return true;
}

int readMove(const std::vector<char>& board, char player) {
    while (true) {
        std::cout << "Player " << player << " enter move (1-9): ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (choice < 1 || choice > 9) continue;
        if (board[choice - 1] != ' ') continue;
        return choice - 1;
    }
}

/* =========================================================
   SINGLE MATCH (USED BY BATTLE & CAMPAIGN)
   ========================================================= */

void playSingleMatch(Character& player, Character& enemy) {
    std::vector<char> board(9, ' ');
    char current = player.mark;

    displayTable(board);

    while (true) {
        int idx;
        if (current == player.mark)
            idx = readMove(board, player.mark);
        else
            do { idx = std::rand() % 9; } while (board[idx] != ' ');

        board[idx] = current;
        displayTable(board);

        char winner = checkWinner(board);
        if (winner != ' ') {
            if (winner == player.mark) {
                int dmg = std::max(0, player.attack - enemy.defense);
                enemy.health -= dmg;
                std::cout << "You win the match! Damage dealt: " << dmg << "\n";
            } else {
                int dmg = std::max(0, enemy.attack - player.defense);
                player.health -= dmg;
                std::cout << enemy.name << " wins the match! Damage taken: " << dmg << "\n";
            }
            return;
        }

        if (boardFull(board)) {
            std::cout << "Draw! No damage dealt.\n";
            return;
        }

        current = (current == player.mark) ? enemy.mark : player.mark;
    }
}

/* =========================================================
   EVENTS
   ========================================================= */

void eventBetweenBattles(Character& player) {
    std::cout << "\nAn event occurs!\n";
    std::cout << "1) Rest (+5 health)\n";
    std::cout << "2) Train (+1 attack)\n";
    std::cout << "Choose: ";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        player.health += 5;
        std::cout << "You feel refreshed.\n";
    } else {
        player.attack += 1;
        std::cout << "You feel stronger.\n";
    }
}

/* =========================================================
   CAMPAIGN MODE
   ========================================================= */

void playCampaign() {
    std::srand(std::time(nullptr));

    Character player;
    std::cout << "Welcome Hero! Enter your name: ";
    std::cin >> player.name;

    player.archetype = promptArchetype(1);
    player.mark = promptForMove(1, ' ');

    if (player.archetype == "Paladin") {
        player.health = 25;
        player.attack = 5;
        player.defense = 3;
    } else {
        player.health = 20;
        player.attack = 6;
        player.defense = 2;
    }

    std::vector<Character> gods = {
        {"Ares","God",12,4,1,'O'},
        {"Athena","God",14,5,2,'O'},
        {"Poseidon","God",16,6,3,'O'},
        {"Hades","God",18,7,3,'O'},
        {"Zeus","God",25,8,4,'O'}
    };

    std::cout << "\nYour journey to Mount Olympus begins...\n";

    for (size_t i = 0; i < gods.size(); i++) {
        Character& enemy = gods[i];
        std::cout << "\nYou face " << enemy.name << "!\n";

        while (player.health > 0 && enemy.health > 0) {
            playSingleMatch(player, enemy);

            if (enemy.name == "Zeus" && std::rand() % 2 == 0) {
                std::cout << "⚡ Zeus uses Lightning Strike!\n";
                player.health -= 2;
            }

            std::cout << "Your HP: " << player.health
                      << " | " << enemy.name << " HP: " << enemy.health << "\n";
        }

        if (player.health <= 0) {
            std::cout << "\nYou were defeated. Campaign restarting...\n";
            return;
        }

        std::cout << "You defeated " << enemy.name << "!\n";
        if (i < gods.size() - 1)
            eventBetweenBattles(player);
    }

    std::cout << "\n🏆 You defeated Zeus and completed the campaign! 🏆\n";
}

/* =========================================================
   OTHER MODES
   ========================================================= */

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
            return;
        }
        if (boardFull(board)) {
            std::cout << "Draw!\n";
            return;
        }
        current = (current == 'X') ? 'O' : 'X';
    }
}

void playBattleMode() {
    Character player = {"Player","Paladin",15,5,2,'X'};
    Character enemy = {"Enemy","God",15,4,1,'O'};

    while (player.health > 0 && enemy.health > 0)
        playSingleMatch(player, enemy);
}

/* =========================================================
   MAIN
   ========================================================= */

int main() {
    while (true) {
        std::cout << "\nChoose game type:\n"
                  << "1) Regular\n"
                  << "2) Battle\n"
                  << "3) Campaign\n"
                  << "Enter choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) playRegular();
        else if (choice == 2) playBattleMode();
        else if (choice == 3) playCampaign();
        else continue;

        std::cout << "Play again? (y/n): ";
        char again;
        std::cin >> again;
        if (again != 'y' && again != 'Y') break;
    }

    std::cout << "Thanks for playing!\n";
    return 0;
}
