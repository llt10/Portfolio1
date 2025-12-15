#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;

struct Character {
    string name;
    string role;
    int health;
    int attack;
    int defense;
};

/* ================= TIC TAC TOE ================= */

void printBoard(const vector<char>& board) {
    cout << "\n";
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') cout << i + 1;
        else cout << board[i];
        if (i % 3 != 2) cout << " | ";
        if (i % 3 == 2 && i != 8)
            cout << "\n---+---+---\n";
    }
    cout << "\n\n";
}

bool checkWin(const vector<char>& b, char p) {
    int wins[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
    for (auto& w : wins)
        if (b[w[0]] == p && b[w[1]] == p && b[w[2]] == p)
            return true;
    return false;
}

bool boardFull(const vector<char>& b) {
    for (char c : b)
        if (c == ' ') return false;
    return true;
}

int playTicTacToeRound() {
    vector<char> board(9, ' ');
    printBoard(board);

    while (true) {
        int move;
        cout << "Player X — enter a move (1-9): ";
        cin >> move;
        move--;

        if (move < 0 || move > 8 || board[move] != ' ') {
            cout << "Invalid move.\n";
            continue;
        }

        board[move] = 'X';
        printBoard(board);

        if (checkWin(board, 'X')) return 1;
        if (boardFull(board)) return 0;

        vector<int> options;
        for (int i = 0; i < 9; i++)
            if (board[i] == ' ') options.push_back(i);

        int enemyMove = options[rand() % options.size()];
        board[enemyMove] = 'O';
        cout << "Enemy chooses position " << enemyMove + 1 << "\n";
        printBoard(board);

        if (checkWin(board, 'O')) return -1;
        if (boardFull(board)) return 0;
    }
}

/* ================= EVENTS ================= */

void healingEvent(Character& player) {
    cout << "You find a healing shrine. +10 HP\n";
    player.health += 10;
}

void curseEvent(Character& player) {
    cout << "A cursed mist weakens you. -2 DEF\n";
    player.defense = max(0, player.defense - 2);
}

void choiceEvent(Character& player) {
    cout << "A stranger offers you power.\n";
    cout << "1) Gain +3 ATK\n2) Gain +5 HP\n";
    int choice;
    cin >> choice;
    if (choice == 1) {
        player.attack += 3;
        cout << "Your attack increases!\n";
    } else {
        player.health += 5;
        cout << "You feel healthier!\n";
    }
}

/* ================= BATTLE ================= */

void battle(Character& player, int enemyNum, bool boss = false) {
    Character enemy;
    enemy.name = "Enemy " + to_string(enemyNum);
    enemy.health = 20 + enemyNum * 5;
    enemy.attack = 5 + enemyNum;
    enemy.defense = 2 + enemyNum;

    if (boss) {
        enemy.name = "Dark Overlord";
        enemy.health = 60;
        enemy.attack = 10;
        enemy.defense = 6;
    }

    cout << "\nBattle vs " << enemy.name << "\n";

    while (player.health > 0 && enemy.health > 0) {
        int result = playTicTacToeRound();

        if (result == 1) {
            int dmg = max(0, player.attack - enemy.defense);
            enemy.health -= dmg;
            cout << "You won the match! Enemy takes " << dmg << " damage.\n";
        } else if (result == -1) {
            int dmg = max(0, enemy.attack - player.defense);
            player.health -= dmg;
            cout << "Enemy won the round! You take " << dmg << " damage.\n";
        } else {
            cout << "The round was a draw. No damage dealt.\n";
        }

        if (boss && rand() % 3 == 0) {
            cout << "The Dark Overlord unleashes Shadow Burst!\n";
            player.health -= 5;
        }

        cout << "Player HP: " << player.health
             << " | Enemy HP: " << enemy.health << "\n";
    }

    if (player.health <= 0)
        throw runtime_error("Player defeated");

    cout << enemy.name << " defeated!\n";
}

/* ================= CAMPAIGN ================= */

void playCampaign() {
    try {
        Character player;

        cout << "\nYou awaken in a land shattered by endless war.\n";
        cout << "Five trials stand between you.\n\n";

        cout << "Enter your hero's name: ";
        cin >> player.name;

        cout << "Choose class (Paladin / Alchemist): ";
        cin >> player.role;

        if (player.role == "Paladin") {
            player.health = 50;
            player.attack = 8;
            player.defense = 6;
        } else {
            player.health = 45;
            player.attack = 6;
            player.defense = 4;
        }

        cout << "\nA lone scout blocks your path.\n";
        battle(player, 1);
        healingEvent(player);

        cout << "\nA corrupted soldier rises from the ruins.\n";
        battle(player, 2);
        curseEvent(player);

        cout << "\nA brutal warlord challenges you.\n";
        battle(player, 3);
        choiceEvent(player);

        cout << "\nThe Dark Overlord’s lieutenant confronts you.\n";
        battle(player, 4);
        healingEvent(player);

        cout << "\nThe Dark Overlord emerges.\n";
        battle(player, 5, true);

        cout << "\nThe land begins to heal. You are victorious.\n";

        char again;
        cout << "Play campaign again? (y/n): ";
        cin >> again;
        if (again == 'y' || again == 'Y')
            playCampaign();

    } catch (...) {
        cout << "\nYou have fallen.\n";
        char again;
        cout << "Restart campaign? (y/n): ";
        cin >> again;
        if (again == 'y' || again == 'Y')
            playCampaign();
    }
}

/* ================= MAIN ================= */

int main() {
    srand(time(nullptr));

    cout << "Welcome to the Tic Tac Toe Game!!\n\n";
    cout << "Choose game type:\n";
    cout << "1) regular\n2) battle\n3) campaign\n";
    cout << "Enter 1, 2, or 3: ";

    int choice;
    cin >> choice;

    if (choice == 3)
        playCampaign();
    else
        cout << "Only campaign mode.\n";

    return 0;
}