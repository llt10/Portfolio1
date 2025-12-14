#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include <ctime>


char showCell(const std::vector<char>& board, int i) {
    if (board[i] != ' ') return board[i];
    return static_cast<char>('1' + i);
}

void displayTable(const std::vector<char>& board) {
    std::cout << " " << showCell(board,0) << " | " << showCell(board,1) << " | " << showCell(board,2) << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board,3) << " | " << showCell(board,4) << " | " << showCell(board,5) << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board,6) << " | " << showCell(board,7) << " | " << showCell(board,8) << "\n\n";
}

int countMoves(const std::vector<char>& board) {
    int cnt = 0;
    for (char c : board) if (c != ' ') ++cnt;
    return cnt;
}

bool isAdjacent(int from, int to) {
    int fr = from / 3, fc = from % 3;
    int tr = to / 3, tc = to % 3;
    int dr = abs(fr - tr), dc = abs(fc - tc);
    return (dr <= 1 && dc <= 1) && !(dr == 0 && dc == 0);
}

bool paladinShift(std::vector<char>& board) {
    std::cout << "Enter the position of the mark to shift (1-9): ";
    int a; if (!(std::cin >> a)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false;}
    if (a < 1 || a > 9) return false;
    int from = a-1;
    if (board[from]==' ') { std::cout<<"No mark at that position.\n"; return false; }
    std::cout<<"Enter destination (1-9): ";
    int b; if (!(std::cin >> b)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false;}
    if (b<1 || b>9) return false;
    int to = b-1;
    if (board[to] != ' ') { std::cout<<"Destination not empty.\n"; return false; }
    if (!isAdjacent(from,to)) { std::cout<<"Destination not adjacent.\n"; return false; }
    std::swap(board[from], board[to]);
    return true;
}

bool alchemistSwap(std::vector<char>& board) {
    std::cout << "Enter first position to swap (1-9): ";
    int a; if (!(std::cin >> a)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false;}
    int i = a-1; if (board[i]==' ') { std::cout<<"No mark at first position.\n"; return false; }
    std::cout << "Enter second position to swap (1-9): ";
    int b; if (!(std::cin >> b)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false;}
    int j = b-1; if (board[j]==' ') { std::cout<<"No mark at second position.\n"; return false; }
    if (board[i] == board[j]) { std::cout<<"Cannot swap identical marks.\n"; return false; }
    std::swap(board[i], board[j]);
    return true;
}

bool validMarkChar(char c) {
    if ((c>='A' && c<='Z') || (c>='a' && c<='z')) return true;
    std::string allowed = "?!*~$%#";
    return allowed.find(c) != std::string::npos;
}

char promptForMove(int playerNumber, char otherPlayerMark) {
    while (true) {
        std::cout<<"Player "<<playerNumber<<" choose single-character mark: ";
        std::string s; if (!(std::cin>>s)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); continue; }
        if (s.size()!=1) { std::cout<<"Enter exactly one character.\n"; continue; }
        char c = s[0];
        if (!validMarkChar(c)) { std::cout<<"Invalid mark. Use A-Z,a-z, or ?!*~$%#\n"; continue; }
        if (otherPlayerMark!='\0' && c==otherPlayerMark) { std::cout<<"Mark already taken.\n"; continue; }
        return c;
    }
}

std::string toLower(const std::string &s) {
    std::string out = s; for (char &c: out) c = std::tolower(static_cast<unsigned char>(c)); return out;
}

std::string promptArchetype(int playerNumber) {
    while(true){
        std::cout<<"Player "<<playerNumber<<" choose archetype (Paladin, Alchemist): ";
        std::string s; if (!(std::cin >> s)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); continue; }
        std::string low = toLower(s);
        if(low=="paladin" || low=="alchemist") return low;
        std::cout<<"Invalid archetype.\n";
    }
}

int readMove(const std::vector<char>& board, char mark) {
    while(true){
        std::cout<<"Player "<<mark<<" enter move (1-9): ";
        int n; if (!(std::cin >> n)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); continue; }
        if (n<1 || n>9) { std::cout<<"Out of bounds\n"; continue; }
        if (board[n-1] != ' ') { std::cout<<"Cell occupied\n"; continue; }
        return n-1;
    }
}

// regular
bool boardFull(const std::vector<char>& b) {
    for(char c: b) if(c==' ') return false;
    return true;
}

char checkWinner(const std::vector<char>& b) {
    const int lines[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(int i=0;i<8;i++){
        if(b[lines[i][0]]!=' ' && b[lines[i][0]]==b[lines[i][1]] && b[lines[i][1]]==b[lines[i][2]])
            return b[lines[i][0]];
    }
    return ' ';
}

void playRegular(){
    std::vector<char> board(9,' ');
    char current = 'X';
    displayTable(board);
    while(true){
        int idx = readMove(board,current);
        board[idx] = current;
        displayTable(board);
        char w = checkWinner(board);
        if(w!=' '){ std::cout<<w<<" won!\n"; break; }
        if(boardFull(board)){ std::cout<<"Draw!\n"; break; }
        current = (current=='X')?'O':'X';
    }
}

//battle

struct Character{
    std::string name;
    std::string archetype;
    int health;
    int attack;
    int defense;
    char mark;
};

int damage(Character& attacker, Character& defender){
    int dmg = attacker.attack - defender.defense;
    return (dmg>0)?dmg:1;
}

char whichPlayerForMove(char mark, char m1, char m2){
    if(mark==m1) return '1';
    if(mark==m2) return '2';
    return '0';
}

void playBattle(){
    Character p1,p2;
    std::cout<<"Enter Player 1 name: "; std::cin>>p1.name;
    p1.archetype = promptArchetype(1);
    p1.mark = promptForMove(1,'\0');
    p1.health = 20; p1.attack=5; p1.defense=2;

    std::cout<<"Enter Player 2 name: "; std::cin>>p2.name;
    p2.archetype = promptArchetype(2);
    p2.mark = promptForMove(2,p1.mark);
    p2.health = 20; p2.attack=5; p2.defense=2;

    std::vector<char> board(9,' ');
    char current = p1.mark;

    while(p1.health>0 && p2.health>0){
        displayTable(board);
        std::cout<<"Player "<<((current==p1.mark)?1:2)<<"'s turn.\n";
        int idx = readMove(board,current);
        board[idx]=current;
        char w = checkWinner(board);
        if(w!=' '){
            if(w==p1.mark) p2.health -= damage(p1,p2);
            else p1.health -= damage(p2,p1);
            std::cout<<"Health P1: "<<p1.health<<" | P2: "<<p2.health<<"\n";
            board.assign(9,' ');
        }
        if(boardFull(board)){ board.assign(9,' '); }
        current = (current==p1.mark)?p2.mark:p1.mark;
    }

    if(p1.health>0) std::cout<<"Player 1 Wins the Battle!\n";
    else std::cout<<"Player 2 Wins the Battle!\n";
}

//campaign

void playCampaign(){
    std::srand((unsigned)std::time(nullptr));
    Character player;
    std::cout<<"Welcome Hero! Enter your name: "; std::cin>>player.name;
    player.archetype = promptArchetype(1);
    player.mark = promptForMove(1,'\0');
    player.health=20; player.attack=5; player.defense=2;

    std::vector<std::string> enemies = {"Ares","Hades","Poseidon","Hermes","Zeus"};
    std::vector<int> enemyHealth = {10,12,15,8,25};

    for(size_t i=0;i<enemies.size();i++){
        Character enemy;
        enemy.name = enemies[i]; enemy.archetype="paladin"; enemy.health=enemyHealth[i]; enemy.attack=4+i; enemy.defense=1+i; enemy.mark='O';
        std::cout<<"\nBattle against "<<enemy.name<<" begins!\n";
        std::vector<char> board(9,' ');
        char current = player.mark;
        while(player.health>0 && enemy.health>0){
            displayTable(board);
            int idx;
            if(current==player.mark){ idx=readMove(board,current); board[idx]=current; }
            else{
                do{ idx = std::rand()%9; }while(board[idx]!=' '); board[idx]=current;
            }
            char w = checkWinner(board);
            if(w!=' '){
                if(w==player.mark) enemy.health -= damage(player,enemy);
                else player.health -= damage(enemy,player);
                std::cout<<"Health "<<player.name<<": "<<player.health<<" | "<<enemy.name<<": "<<enemy.health<<"\n";
                board.assign(9,' ');
            }
            if(boardFull(board)) board.assign(9,' ');
            current = (current==player.mark)?enemy.mark:player.mark;
        }
        if(player.health<=0){ std::cout<<"You have fallen. Game Over.\n"; return;}
        std::cout<<"You defeated "<<enemy.name<<"!\n";
        if(i<enemies.size()-1){
            std::cout<<"You find a fountain. Choose: 1) Heal 2) Boost Attack: ";
            int choice; std::cin>>choice;
            if(choice==1) player.health +=5;
            else player.attack +=2;
        }
    }
    std::cout<<"Congratulations! You conquered Olympus!\n";
}


//main
int main(){
    while(true){
        std::cout<<"\nChoose game type:\n1) Regular\n2) Battle\n3) Campaign\nEnter choice: ";
        int choice; if(!(std::cin>>choice)){ std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); continue; }
        if(choice==1) playRegular();
        else if(choice==2) playBattle();
        else if(choice==3) playCampaign();
        else { std::cout<<"Invalid choice\n"; continue; }

        std::cout<<"Play again? (y/n): ";
        char again; std::cin>>again;
        if(again!='y' && again!='Y') break;
    }
    std::cout<<"Thanks for playing!\n";
    return 0;
}