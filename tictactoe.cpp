#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
#include <string>

using namespace std;

char showCell(const vector<char>& board, int i) {
    return (board[i] != ' ') ? board[i] : static_cast<char>('1' + i);
}

void displayTable(const vector<char>& board) {
    cout << " " << showCell(board, 0) << " | " << showCell(board, 1) << " | " << showCell(board, 2) << endl;
    cout << "---+---+---\n";
    cout << " " << showCell(board, 3) << " | " << showCell(board, 4) << " | " << showCell(board, 5) << endl;
    cout << "---+---+---\n";
    cout << " " << showCell(board, 6) << " | " << showCell(board, 7) << " | " << showCell(board, 8) << endl;
    cout << "\n";
}

char checkWinner(const vector<char>& b) {
    const int lines[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for (int i = 0; i < 8; ++i) {
        int a = lines[i][0], c = lines[i][1], d = lines[i][2];
        if (b[a] != ' ' && b[a] == b[c] && b[c] == b[d])
            return b[a];
    }
    return ' ';
}

int readMove(const vector<char>& board, char player) {
    while (true) {
        cout << "Player " << player << " — enter a move (1-9): ";
        int choice;
        if (!(cin >> choice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        if (choice < 1 || choice > 9) { cout << "Out of bounds. Choose 1-9.\n"; continue; }
        int index = choice - 1;
        if (board[index] != ' ') { cout << "Cell taken. Choose another.\n"; continue; }
        return index;
    }
}

struct Character {
    string name;
    string role;
    int health;
    int attack;
    int defense;
    int gold = 0;
};

void playRegular() {
    vector<char> board(9, ' ');
    char current = 'X';
    cout << "Player 1 is X || Player 2 is O\n" << endl;
    displayTable(board);
    while (true) {
        int idx = readMove(board, current);
        board[idx] = current;
        displayTable(board);
        char winner = checkWinner(board);
        if (winner != ' ') { cout << winner << " won the game!\n\n"; break; }
        if (all_of(board.begin(), board.end(), [](char c){ return c != ' '; })) { cout << "It's a draw!\n\n"; break; }
        current = (current == 'X') ? 'O' : 'X';
    }
}

void playBattle() {
    char p1Move = 'X';
    char p2Move = 'O';
    vector<char> board(9, ' ');
    cout << "Battle mode! Player 1: " << p1Move << " vs Player 2: " << p2Move << endl;
    displayTable(board);
    char current = p1Move;
    while (true) {
        int idx = readMove(board, current);
        board[idx] = current;
        displayTable(board);
        char winner = checkWinner(board);
        if (winner != ' ') { cout << winner << " won the battle!\n\n"; break; }
        if (all_of(board.begin(), board.end(), [](char c){ return c != ' '; })) { cout << "Battle draw!\n\n"; break; }
        current = (current == p1Move) ? p2Move : p1Move;
    }
}

int playTicTacToeRound() {
    vector<char> board(9, ' ');
    displayTable(board);
    while (true) {
        int move = readMove(board, 'X');
        board[move] = 'X';
        displayTable(board);
        if (checkWinner(board) == 'X') return 1;
        if (all_of(board.begin(), board.end(), [](char c){ return c != ' '; })) return 0;
        vector<int> options;
        for (int i = 0; i < 9; i++) if (board[i] == ' ') options.push_back(i);
        int enemyMove = options[rand() % options.size()];
        board[enemyMove] = 'O';
        displayTable(board);
        if (checkWinner(board) == 'O') return -1;
        if (all_of(board.begin(), board.end(), [](char c){ return c != ' '; })) return 0;
    }
}

void enemySpecial(Character& player, Character& enemy) {
    int choice = rand() % 3;
    switch(choice) {
        case 0: player.health = max(0, player.health - 3); break;
        case 1: enemy.defense += 2; break;
        case 2: enemy.health += 2; break;
    }
}

void finalBossSpecial(Character& player, Character& enemy) {
    int choice = rand() % 3;
    switch(choice) {
        case 0: player.health = max(0, player.health - 5); break;
        case 1: enemy.defense += 4; break;
        case 2: player.health = max(0, player.health - 3); enemy.health += 3; break;
    }
}

bool battle(Character& player, int enemyNum, bool boss=false) {
    Character enemy;
    enemy.name = boss ? "Dark Overlord" : "Enemy " + to_string(enemyNum);
    enemy.health = boss ? 60 : 20 + enemyNum*5;
    enemy.attack = boss ? 10 : 5 + enemyNum;
    enemy.defense = boss ? 6 : 2 + enemyNum;
    while (player.health > 0 && enemy.health > 0) {
        int result = playTicTacToeRound();
        if (result == 1) {
            int dmg = max(0, player.attack - enemy.defense);
            enemy.health -= dmg;
        } else if (result == -1) {
            int dmg = max(0, enemy.attack - player.defense);
            player.health -= dmg;
        }
        if (boss) finalBossSpecial(player, enemy);
        else if (rand()%4==0) enemySpecial(player, enemy);
    }
    if (player.health <= 0) { cout << "You have fallen.\n"; return false; }
    else { cout << "X won the battle!\n"; return true; }
}

void healingEvent(Character& player) { player.health += 10; }
void curseEvent(Character& player) { player.defense = max(0, player.defense - 2); }
void choiceEvent(Character& player) {
    int choice; cin >> choice;
    if(choice==1) player.attack+=3;
    else player.health+=5;
}
void shopEvent(Character& player){
    int choice=-1;
    while(choice!=0){
        cin >> choice;
        switch(choice){
            case 1: if(player.gold>=5){player.health+=10;player.gold-=5;} break;
            case 2: if(player.gold>=7){player.attack+=2;player.gold-=7;} break;
            case 3: if(player.gold>=7){player.defense+=2;player.gold-=7;} break;
            case 0: break;
        }
    }
}

void playCampaign() {
    Character player;
    cout << "Enter your hero's name: "; cin >> player.name;
    cout << "Choose class (Paladin / Alchemist): "; cin >> player.role;
    if(player.role=="Paladin"){player.health=50;player.attack=8;player.defense=6;}
    else{player.health=45;player.attack=6;player.defense=4;}

    if(!battle(player,1)) goto restart; healingEvent(player);
    if(!battle(player,2)) goto restart; curseEvent(player);
    if(!battle(player,3)) goto restart; choiceEvent(player);
    shopEvent(player);
    if(!battle(player,4)) goto restart; healingEvent(player);
    if(!battle(player,5,true)) goto restart;

    cout << "You have completed the campaign! Congratulations!\n";
    char again; cout << "Play again? (y/n): "; cin>>again;
    if(again=='y'||again=='Y') playCampaign();
    return;

restart:
    char again; cout << "Restart campaign? (y/n): "; cin>>again;
    if(again=='y'||again=='Y') playCampaign();
}

int main() {
    srand(time(nullptr));
    cout<<"Welcome to the Tic Tac Toe Game!!\n\n";
    while(true){
        cout<<"Choose game type:\n1) regular\n2) battle\n3) campaign\nEnter 1, 2, or 3: ";
        int choice; if(!(cin>>choice)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n');continue;}
        if(choice==1) playRegular();
        else if(choice==2) playBattle();
        else if(choice==3) playCampaign();
        else {cout<<"Invalid choice.\n"; continue;}
        char again; cout<<"Play again? (y/n): "; cin>>again; if(!(again=='y'||again=='Y')) break;
    }
    return 0;
}
