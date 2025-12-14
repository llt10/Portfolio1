#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "battle.hpp"

struct Character {
    std::string name;
    std::string archetype;
    int health;
    int attack;
    int defense;
    char mark;
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

char checkWinner(const std::vector<char>& b) {
    const int lines[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
    for (int i = 0; i < 8; ++i) {
        int a = lines[i][0], c = lines[i][1], d = lines[i][2];
        if (b[a] != ' ' && b[a] == b[c] && b[c] == b[d]) return b[a];
    }
    return ' ';
}

int readMove(const std::vector<char>& board, char player) {
    while (true) {
        std::cout << "Player " << player << " enter move (1-9): ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter a number 1-9.\n"; continue;
        }
        if (choice < 1 || choice > 9 || board[choice-1] != ' ') {
            std::cout << "Invalid or occupied cell.\n"; continue;
        }
        return choice-1;
    }
}

bool boardFull(const std::vector<char>& b) {
    for (char c : b) if (c == ' ') return false;
    return true;
}

int whichPlayerForMove(char mark, char p1Move, char p2Move) {
    if (mark == p1Move) return 1;
    if (mark == p2Move) return 2;
    return 0;
}

bool validMarkChar(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    std::string allowed = "?!*~$%#";
    return allowed.find(c) != std::string::npos;
}

char promptForMove(int playerNumber, char otherPlayerMark) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose a single-character mark: ";
        std::string s; if (!(std::cin >> s)) { std::cin.clear(); std::cin.ignore(10000,'\n'); continue; }
        if (s.size() != 1 || !validMarkChar(s[0]) || (otherPlayerMark!='\0' && s[0]==otherPlayerMark)) {
            std::cout << "Invalid mark.\n"; continue;
        }
        return s[0];
    }
}

std::string toLower(const std::string& s) {
    std::string out = s; for(char &c : out) c = std::tolower(c); return out;
}

std::string promptArchetype(int playerNumber) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose archetype (Paladin, Alchemist, Chronomage): ";
        std::string s; if (!(std::cin >> s)) { std::cin.clear(); std::cin.ignore(10000,'\n'); continue; }
        std::string low = toLower(s);
        if (low=="paladin" || low=="alchemist" || low=="chronomage") return low;
        std::cout << "Invalid archetype.\n";
    }
}

int countMoves(const std::vector<char>& board) {
    int cnt=0; for(char c:board) if(c!=' ') ++cnt; return cnt;
}

bool isAdjacent(int from, int to) {
    int fr = from/3, fc=from%3, tr=to/3, tc=to%3;
    int dr = abs(fr-tr), dc=abs(fc-tc); return (dr<=1 && dc<=1) && !(dr==0 && dc==0);
}

bool paladinShift(std::vector<char>& board) {
    std::cout << "Enter mark to shift (1-9): "; int a; if(!(std::cin>>a)) return false; int from=a-1;
    if(board[from]==' ') { std::cout<<"No mark.\n"; return false; }
    std::cout<<"Enter destination (1-9): "; int b; if(!(std::cin>>b)) return false; int to=b-1;
    if(board[to]!=' ' || !isAdjacent(from,to)) { std::cout<<"Invalid.\n"; return false; }
    std::swap(board[from],board[to]); return true;
}

bool alchemistSwap(std::vector<char>& board) {
    std::cout<<"Enter first position to swap (1-9): "; int a; if(!(std::cin>>a)) return false; int i=a-1;
    if(board[i]==' ') return false;
    std::cout<<"Enter second position to swap (1-9): "; int b; if(!(std::cin>>b)) return false; int j=b-1;
    if(board[j]==' ' || board[i]==board[j]) return false;
    std::swap(board[i],board[j]); return true;
}

void playRegular() {
    std::vector<char> board(9,' '); char current='X';
    std::cout<<"Player 1 is X || Player 2 is O\n"; displayTable(board);
    while(true){
        int idx=readMove(board,current); board[idx]=current;
        displayTable(board);
        char w=checkWinner(board);
        if(w!=' ') { std::cout<<w<<" won!\n"; break; }
        if(boardFull(board)) { std::cout<<"Draw!\n"; break; }
        current=(current=='X')?'O':'X';
    }
}

void playBattle() {
    std::cout<<"Battle Mode!\n";
    char p1Move=promptForMove(1,'\0'); char p2Move=promptForMove(2,p1Move);
    std::string p1Arche=promptArchetype(1); std::string p2Arche=promptArchetype(2);
    std::vector<char> board(9,' '); char current=p1Move;
    displayTable(board);
    while(true){
        int playerNum = (current==p1Move)?1:2;
        std::string arche = (playerNum==1)?p1Arche:p2Arche;
        bool didAction=false;
        std::cout<<"Player "<<playerNum<<" ("<<current<<") turn.\n";
        std::cout<<"1) Regular 2) Special: "; int choice; if(!(std::cin>>choice)) choice=1;
        if(choice==1){ int idx=readMove(board,current); board[idx]=current; didAction=true; }
        else {
            if(arche=="alchemist"){ if(countMoves(board)<2){ int idx=readMove(board,current); board[idx]=current; didAction=true; }
            else { if(!alchemistSwap(board)){ int idx=readMove(board,current); board[idx]=current; didAction=true; } else didAction=true; } }
            else if(arche=="paladin"){ if(countMoves(board)<1){ int idx=readMove(board,current); board[idx]=current; didAction=true; }
            else { if(!paladinShift(board)){ int idx=readMove(board,current); board[idx]=current; didAction=true; } else didAction=true; } }
            else { int idx=readMove(board,current); board[idx]=current; didAction=true; }
        }
        displayTable(board);
        char w=checkWinner(board);
        if(w!=' '){ int who=whichPlayerForMove(w,p1Move,p2Move); std::cout<<"Player "<<who<<" won!\n"; return; }
        if(boardFull(board)) { std::cout<<"Draw!\n"; return; }
        current=(current==p1Move)?p2Move:p1Move;
    }
}

int damage(Character& attacker, Character& defender){
    int dmg = attacker.attack - defender.defense;
    return (dmg>0)?dmg:1;
}

void playCampaign() {
    std::srand(std::time(nullptr));
    Character player;
    std::cout<<"Welcome Hero! Enter your name: "; std::cin>>player.name;
    player.archetype=promptArchetype(1);
    player.mark=promptForMove(1,'\0');
    player.health=20; player.attack=5; player.defense=2;

    std::vector<std::string> enemies = {"Ares","Athena","Hermes","Poseidon","Zeus"};
    std::vector<int> enemyHealth={6,8,10,12,15};

    for(size_t i=0;i<enemies.size();i++){
        Character enemy;
        enemy.name=enemies[i]; enemy.archetype="paladin";
        enemy.health=enemyHealth[i]; enemy.attack=4+i; enemy.defense=1+i; enemy.mark='O';
        std::cout<<"\nBattle against "<<enemy.name<<" begins!\n";
        std::vector<char> board(9,' '); char current=player.mark;

        while(player.health>0 && enemy.health>0){
            displayTable(board);
            int idx;
            if(current==player.mark){ idx=readMove(board,current); board[idx]=current; }
            else { do{ idx = std::rand()%9; } while(board[idx]!=' '); board[idx]=current; }

            char w=checkWinner(board);
            if(w!=' '){
                if(w==player.mark) enemy.health-=damage(player,enemy);
                else player.health-=damage(enemy,player);
                std::cout<<"Health "<<player.name<<": "<<player.health<<" | "<<enemy.name<<": "<<enemy.health<<"\n";
                if(player.health>0 && enemy.health>0) board.assign(9,' ');
                displayTable(board);
            }

            if(boardFull(board)) board.assign(9,' ');
            current=(current==player.mark)?enemy.mark:player.mark;
        }

        if(player.health<=0){ std::cout<<"You have fallen. Game Over.\n"; return; }
        else {
            std::cout<<"You defeated "<<enemy.name<<"!\n";
            if(i<enemies.size()-1){
                std::cout<<"Fountain found! 1) Heal 2) Boost Attack: ";
                int choice; std::cin>>choice;
                if(choice==1) player.health+=5;
                else player.attack+=2;
            }
        }
    }
    std::cout<<"Congratulations! You conquered Olympus!\n";
}

int main() {
    while(true){
        std::cout<<"Choose game type:\n1) Regular\n2) Battle\n3) Campaign\nEnter choice: ";
        int choice; if(!(std::cin>>choice)){ std::cin.clear(); std::cin.ignore(10000,'\n'); continue; }
        if(choice==1) playRegular();
        else if(choice==2) playBattle();
        else if(choice==3) playCampaign();
        else continue;
        std::cout<<"Play again? (y/n): "; char again; std::cin>>again;
        if(again!='y' && again!='Y') break;
    }
    std::cout<<"Thanks for playing!\n";
    return 0;
}