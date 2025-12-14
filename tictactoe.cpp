#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <limits>
#include <cstdlib>
#include <ctime>

struct Character {
    std::string name;
    std::string archetype;
    int health;
    int attack;
    int defense;
};

struct Enemy {
    std::string name;
    int health;
    int attack;
    int defense;
    bool specialUsed;
};

char showCell(const std::vector<char>& board, int i) {
    if (board[i] != ' ') return board[i];
    return static_cast<char>('1' + i);
}

void displayTable(const std::vector<char>& board) {
    std::cout << " " << showCell(board, 0) << " | " << showCell(board, 1) << " | " << showCell(board, 2) << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 3) << " | " << showCell(board, 4) << " | " << showCell(board, 5) << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 6) << " | " << showCell(board, 7) << " | " << showCell(board, 8) << "\n\n";
}

bool boardFull(const std::vector<char>& board) {
    for (char c : board) if (c == ' ') return false;
    return true;
}

char checkWinner(const std::vector<char>& b) {
    const int lines[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
    for (int i = 0; i < 8; ++i) {
        int a = lines[i][0];
        int c = lines[i][1];
        int d = lines[i][2];
        if (b[a] != ' ' && b[a] == b[c] && b[c] == b[d]) return b[a];
    }
    return ' ';
}

int readMove(const std::vector<char>& board, char player) {
    while (true) {
        std::cout << "Player " << player << " enter move (1-9): ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input.\n";
            continue;
        }
        if (choice < 1 || choice > 9) {
            std::cout << "Out of bounds.\n";
            continue;
        }
        if (board[choice - 1] != ' ') {
            std::cout << "Cell occupied.\n";
            continue;
        }
        return choice - 1;
    }
}

bool validMarkChar(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    std::string allowed = "?!*~$%#";
    return allowed.find(c) != std::string::npos;
}

char promptForMove(int playerNumber, char otherPlayerMark) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose single-character mark: ";
        std::string s;
        if (!(std::cin >> s)) { std::cin.clear(); std::cin.ignore(1000,'\n'); continue; }
        if (s.size() != 1) { std::cout << "Enter exactly one character.\n"; continue; }
        char c = s[0];
        if (!validMarkChar(c)) { std::cout << "Invalid mark.\n"; continue; }
        if (otherPlayerMark != '\0' && c == otherPlayerMark) { std::cout << "Mark taken.\n"; continue; }
        return c;
    }
}

std::string toLower(const std::string& s) {
    std::string out = s;
    for (char &c : out) c = static_cast<char>(std::tolower(c));
    return out;
}

std::string promptArchetype(int playerNumber) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose archetype (Paladin, Alchemist): ";
        std::string s;
        if (!(std::cin >> s)) { std::cin.clear(); std::cin.ignore(1000,'\n'); continue; }
        std::string low = toLower(s);
        if (low == "paladin" || low == "alchemist") return low;
        std::cout << "Invalid archetype.\n";
    }
}

void playRegular() {
    std::vector<char> board(9,' ');
    char current = 'X';
    displayTable(board);
    while (true) {
        int idx = readMove(board, current);
        board[idx] = current;
        displayTable(board);
        char winner = checkWinner(board);
        if (winner != ' ') {
            std::cout << winner << " won!\n";
            break;
        }
        if (boardFull(board)) {
            std::cout << "Draw!\n";
            break;
        }
        current = (current == 'X') ? 'O' : 'X';
    }
}

void playBattle(Character &player, Enemy &enemy) {
    std::vector<char> board(9,' ');
    char pMark = promptForMove(1,'\0');
    char eMark = 'O'; // Enemy mark always O
    std::cout << player.name << " (" << player.archetype << ") vs " << enemy.name << "\n";
    displayTable(board);

    char current = pMark;
    while (player.health > 0 && enemy.health > 0) {
        int idx;
        if (current == pMark) {
            idx = readMove(board, current);
        } else { // enemy move random
            std::vector<int> open;
            for (int i=0;i<9;i++) if (board[i]==' ') open.push_back(i);
            idx = open[rand()%open.size()];
            std::cout << enemy.name << " plays at " << idx+1 << "\n";
        }
        board[idx] = current;
        displayTable(board);
        char winner = checkWinner(board);
        if (winner != ' ') {
            if (winner == pMark) {
                int dmg = player.attack - enemy.defense;
                if (dmg<0) dmg=0;
                enemy.health -= dmg;
                std::cout << player.name << " won the round! " << enemy.name << " loses " << dmg << " health.\n";
            } else {
                int dmg = enemy.attack - player.defense;
                if (dmg<0) dmg=0;
                player.health -= dmg;
                std::cout << enemy.name << " won the round! " << player.name << " loses " << dmg << " health.\n";
            }
            board = std::vector<char>(9,' ');
            std::cout << "Health " << player.name << ": " << player.health << " | " << enemy.name << ": " << enemy.health << "\n";
        }
        current = (current == pMark) ? eMark : pMark;
    }
}

bool eventBetween(Character &player) {
    std::cout << "You find a fork in the path. Choose:\n1) Rest (+5 health)\n2) Train (+2 attack)\nChoice: ";
    int c;
    if (!(std::cin >> c)) { std::cin.clear(); std::cin.ignore(1000,'\n'); c=1; }
    if (c==1) { player.health +=5; std::cout << "Health is now " << player.health << "\n"; }
    else { player.attack +=2; std::cout << "Attack is now " << player.attack << "\n"; }
    return true;
}

void playCampaign() {
    Character player;
    std::cout << "Welcome Hero! Enter your name: ";
    std::cin >> player.name;
    player.archetype = promptArchetype(1);
    player.health=20; player.attack=5; player.defense=2;

    std::vector<Enemy> enemies = {
        {"Ares",15,4,1,false},
        {"Athena",18,3,2,false},
        {"Hades",20,5,3,false},
        {"Poseidon",22,6,2,false},
        {"Zeus",30,7,4,false} // final boss
    };

    for(size_t i=0;i<enemies.size();i++){
        Enemy &e = enemies[i];
        std::cout << "\nBattle against " << e.name << " begins!\n";
        playBattle(player,e);
        if(player.health<=0) {
            std::cout << "You have fallen! Campaign restarts.\n";
            return;
        }
        if(i<enemies.size()-1) eventBetween(player);
    }
    std::cout << "\nCongratulations " << player.name << "! You defeated all the gods!\n";
}

int main() {
    srand((unsigned)time(0));
    while(true){
        std::cout << "Choose game type:\n1) Regular\n2) Battle\n3) Campaign\nEnter choice: ";
        int choice;
        if(!(std::cin>>choice)) { std::cin.clear(); std::cin.ignore(1000,'\n'); continue; }
        if(choice==1) playRegular();
        else if(choice==2){
            Character p; Enemy e;
            p.name="Player"; p.archetype="Paladin"; p.health=20; p.attack=5; p.defense=2;
            e.name="Enemy"; e.health=15; e.attack=4; e.defense=2; e.specialUsed=false;
            playBattle(p,e);
        }
        else if(choice==3) playCampaign();
        else { std::cout << "Invalid choice.\n"; continue; }

        std::cout << "Play again? (y/n): ";
        char again; std::cin >> again;
        if(again=='y'||again=='Y') continue;
        else break;
    }
    std::cout << "Thanks for playing!\n";
    return 0;
}


