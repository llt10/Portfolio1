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
    const int lines[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
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
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Please enter a number from 1 to 9.\n";
            continue;
        }
        if (choice < 1 || choice > 9) {
            cout << "Out of bounds. Choose 1-9.\n";
            continue;
        }
        int index = choice - 1;
        if (board[index] != ' ') {
            cout << "Cell taken. Choose another.\n";
            continue;
        }
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
        if (winner != ' ') {
            cout << winner << " won the game!\n" << endl;
            break;
        }
        if (all_of(board.begin(), board.end(), [](char c){ return c != ' '; })) {
            cout << "It's a draw!\n" << endl;
            break;
        }
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
        if (winner != ' ') {
            cout << winner << " won the battle!\n" << endl;
            break;
        }
        if (all_of(board.begin(), board.end(), [](char c){ return c != ' '; })) {
            cout << "Battle draw!\n" << endl;
            break;
        }
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
        cout << "Enemy chooses position " << enemyMove + 1 << endl;
        displayTable(board);
        if (checkWinner(board) == 'O') return -1;
        if (all_of(board.begin(), board.end(), [](char c){ return c != ' '; })) return 0;
    }
}

void enemySpecial(Character& player, Character& enemy) {
    int choice = rand() % 3;
    switch(choice) {
        case 0: cout << enemy.name << " uses Power Strike! -3 HP\n"; player.health = max(0, player.health - 3); break;
        case 1: cout << enemy.name << " shields! +2 DEF this round\n"; enemy.defense += 2; break;
        case 2: cout << enemy.name << " heals slightly +2 HP\n"; enemy.health += 2; break;
    }
}

void finalBossSpecial(Character& player, Character& enemy) {
    int choice = rand() % 3;
    switch(choice) {
        case 0: cout << enemy.name << " uses Shadow Burst! -5 HP\n"; player.health = max(0, player.health - 5); break;
        case 1: cout << enemy.name << " uses Dark Shield! +4 DEF\n"; enemy.defense += 4; break;
        case 2: cout << enemy.name << " uses Life Drain! -3 HP to player, +3 HP to self\n"; player.health = max(0, player.health - 3); enemy.health += 3; break;
    }
}

void battle(Character& player, int enemyNum, bool boss=false) {
    Character enemy;
    enemy.name = boss ? "Dark Overlord" : "Enemy " + to_string(enemyNum);
    enemy.health = boss ? 60 : 20 + enemyNum*5;
    enemy.attack = boss ? 10 : 5 + enemyNum;
    enemy.defense = boss ? 6 : 2 + enemyNum;
    cout << "\nBattle vs " << enemy.name << "\n";
    while (player.health > 0 && enemy.health > 0) {
        int result = playTicTacToeRound();
        if (result == 1) { int dmg = max(0, player.attack - enemy.defense); enemy.health -= dmg; cout << "You won the match! Enemy takes " << dmg << " damage.\n"; }
        else if (result == -1) { int dmg = max(0, enemy.attack - player.defense); player.health -= dmg; cout << "Enemy won the round! You take " << dmg << " damage.\n"; }
        else { cout << "Draw. No damage dealt.\n"; }
        if (boss) finalBossSpecial(player, enemy);
        else if (rand()%4 == 0) enemySpecial(player, enemy);
        cout << "Player HP: " << player.health << " | Enemy HP: " << enemy.health << "\n";
    }
    if (player.health <= 0) throw runtime_error("Player defeated");
    cout << enemy.name << " defeated!\n";
    int goldEarned = rand()%5 + 1;
    player.gold += goldEarned;
    cout << "You earn " << goldEarned << " gold.\n";
}

void healingEvent(Character& player) { cout << "You find a healing shrine. +10 HP\n"; player.health += 10; }
void curseEvent(Character& player) { cout << "A cursed mist weakens you. -2 DEF\n"; player.defense = max(0, player.defense - 2); }
void choiceEvent(Character& player) { cout << "A stranger offers you power.\n1) Gain +3 ATK\n2) Gain +5 HP\n"; int choice; cin >> choice; if(choice==1){player.attack+=3;cout<<"Your attack increases!\n";} else{player.health+=5;cout<<"You feel healthier!\n";} }
void shopEvent(Character& player){ cout<<"\nYou enter a merchant's shop. You have "<<player.gold<<" gold.\n"; cout<<"1) Health Potion +10 HP (5 gold)\n2) Steel Sword +2 ATK (7 gold)\n3) Armor +2 DEF (7 gold)\n0) Leave shop\n"; int choice; cin>>choice; switch(choice){case 1: if(player.gold>=5){player.health+=10;player.gold-=5;cout<<"Health increased!\n";} break; case 2: if(player.gold>=7){player.attack+=2;player.gold-=7;cout<<"Attack increased!\n";} break; case 3: if(player.gold>=7){player.defense+=2;player.gold-=7;cout<<"Defense increased!\n";} break; default: cout<<"Leaving shop.\n"; break;} }

void playCampaign() {
    try {
        Character player;
        cout<<"\nYou awaken in a land shattered by endless war.\nFive trials stand between you.\n\n";
        cout<<"Enter your hero's name: "; cin>>player.name;
        cout<<"Choose class (Paladin / Alchemist): "; cin>>player.role;
        if(player.role=="Paladin"){player.health=50;player.attack=8;player.defense=6;} else{player.health=45;player.attack=6;player.defense=4;}
        cout<<"\nA lone scout blocks your path.\n"; battle(player,1); healingEvent(player);
        cout<<"\nA corrupted soldier rises from the ruins.\n"; battle(player,2); curseEvent(player);
        cout<<"\nA brutal warlord challenges you.\n"; battle(player,3); choiceEvent(player);
        cout<<"\nYou find a traveling merchant.\n"; shopEvent(player);
        cout<<"\nThe Dark Overlord’s lieutenant confronts you.\n"; battle(player,4); healingEvent(player);
        cout<<"\nThe Dark Overlord emerges.\n"; battle(player,5,true);
        cout<<"\nThe land begins to heal. You are victorious!\n";
        char again; cout<<"Play campaign again? (y/n): "; cin>>again; if(again=='y'||again=='Y') playCampaign();
    } catch(...) { cout<<"\nYou have fallen.\n"; char again; cout<<"Restart campaign? (y/n): "; cin>>again; if(again=='y'||again=='Y') playCampaign();}
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