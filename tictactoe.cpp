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
    return board[i] != ' ' ? board[i] : static_cast<char>('1' + i);
}

void displayTable(const std::vector<char>& board) {
    std::cout << " " << showCell(board, 0) << " | " << showCell(board, 1) << " | " << showCell(board, 2) << std::endl;
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 3) << " | " << showCell(board, 4) << " | " << showCell(board, 5) << std::endl;
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 6) << " | " << showCell(board, 7) << " | " << showCell(board, 8) << std::endl;
    std::cout << "\n";
}

char checkWinner(const std::vector<char>& b) {
    const int lines[8][3] = { {0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6} };
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
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Error: Please enter a number 1-9.\n";
            continue;
        }
        if (choice < 1 || choice > 9) { std::cout << "Out of bounds.\n"; continue; }
        int idx = choice - 1;
        if (board[idx] != ' ') { std::cout << "Cell occupied.\n"; continue; }
        return idx;
    }
}

bool boardFull(const std::vector<char>& b) {
    for (char c : b) if (c == ' ') return false;
    return true;
}

int countMoves(const std::vector<char>& board) {
    int cnt = 0;
    for (char c : board) if (c != ' ') ++cnt;
    return cnt;
}

bool validMarkChar(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    std::string allowed = "?!*~$%#";
    return allowed.find(c) != std::string::npos;
}

char promptForMove(int playerNumber, char otherPlayerMark) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose single-character mark: ";
        std::string s; if (!(std::cin >> s)) { std::cin.clear(); std::cin.ignore(10000,'\n'); continue; }
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
        std::string s; if (!(std::cin >> s)) { std::cin.clear(); std::cin.ignore(10000,'\n'); continue; }
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
        int idx = readMove(board,current);
        board[idx] = current;
        displayTable(board);
        char winner = checkWinner(board);
        if (winner != ' ') { std::cout << winner << " won!\n"; break; }
        if (boardFull(board)) { std::cout << "It's a draw!\n"; break; }
        current = (current=='X') ? 'O' : 'X';
    }
}

void playBattle() {
    std::vector<char> board(9,' ');
    char p1Move = promptForMove(1,'\0');
    char p2Move = promptForMove(2,p1Move);
    std::string p1Arche = promptArchetype(1);
    std::string p2Arche = promptArchetype(2);
    Character player1 = {"Hero", p1Arche, 20, 5, 2, p1Move};
    Character player2 = {"Enemy", p2Arche, 15, 4, 1, p2Move};
    char current = player1.mark;
    displayTable(board);
    while (player1.health>0 && player2.health>0) {
        int playerNum = (current==player1.mark)?1:2;
        Character &active = (playerNum==1)?player1:player2;
        Character &opponent = (playerNum==1)?player2:player1;

        int choice=1; // Always normal move for simplicity
        int idx = readMove(board,active.mark);
        board[idx]=active.mark;
        displayTable(board);

        char winner = checkWinner(board);
        if(winner != ' '){
            if(winner==player1.mark) player2.health -= std::max(0,player1.attack-player2.defense);
            else player1.health -= std::max(0,player2.attack-player1.defense);

            std::cout << "Health " << player1.name << ": " << player1.health << " | "
                      << player2.name << ": " << player2.health << "\n\n";
            if(player1.health <= 0 || player2.health <=0) break;
            board.assign(9,' ');
            displayTable(board);
            continue;
        }
        current = (current==player1.mark)?player2.mark:player1.mark;
    }

    if(player1.health>0) std::cout << "Player 1 wins the battle!\n";
    else std::cout << "Player 2 wins the battle!\n";
}

void playCampaign() {
    std::srand(std::time(nullptr));
    Character player;
    std::cout << "Welcome Hero! Enter your name: ";
    std::cin >> player.name;
    player.archetype = promptArchetype(1);
    player.mark = promptForMove(1,'\0');
    player.health = 20; player.attack=5; player.defense=2;

    std::vector<std::string> gods = {"Ares","Athena","Poseidon","Hades","Zeus"};
    for(size_t i=0;i<gods.size();++i){
        Character god = {gods[i],"God", 15+(int)(i*2), 4+(int)(i*2), 1+(int)i, (char)('A'+i)};
        std::vector<char> board(9,' ');
        char current = player.mark;
        std::cout << "\nBattle against " << god.name << " begins!\n";
        displayTable(board);
        while(player.health>0 && god.health>0){
            int idx;
            if(current==player.mark) idx=readMove(board,player.mark);
            else { // opponent random move
                do{ idx=std::rand()%9; }while(board[idx]!=' ');
            }
            board[idx]=current;
            displayTable(board);
            char winner=checkWinner(board);
            if(winner!=' '){
                if(winner==player.mark) god.health -= std::max(0,player.attack-god.defense);
                else player.health -= std::max(0,god.attack-player.defense);
                std::cout << "Health " << player.name << ": " << player.health
                          << " | " << god.name << ": " << god.health << "\n";
                board.assign(9,' ');
            }
            current=(current==player.mark)?god.mark:player.mark;
        }
        if(player.health<=0){ std::cout << "You were defeated by " << god.name << "!\n"; return; }
        else std::cout << "You defeated " << god.name << "!\n";
    }
    std::cout << "Congratulations! You defeated all the gods!\n";
}

int main() {
    std::cout << "Welcome to Tic Tac Toe!\n";
    while(true){
        std::cout << "Choose game type:\n1) Regular\n2) Battle\n3) Campaign\nEnter choice: ";
        int choice;
        if(!(std::cin >> choice)){ std::cin.clear(); std::cin.ignore(10000,'\n'); continue; }
        if(choice==1) playRegular();
        else if(choice==2) playBattle();
        else if(choice==3) playCampaign();
        else { std::cout << "Invalid choice.\n"; continue; }

        std::cout << "Play again? (y/n): ";
        char again; if(!(std::cin>>again)) break;
        if(again!='y' && again!='Y') break;
    }
    std::cout << "Thanks for playing!\n";
    return 0;
}