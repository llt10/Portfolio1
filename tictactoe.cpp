#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>
#include <random>
#include <ctime>
#include "battle.hpp"

char showCell(const std::vector<char>& board, int i) {
    if (board[i] != ' ') return board[i];
    return static_cast<char>('1' + i);
}

void displayTable(const std::vector<char>& board) {
    std::cout << " " << showCell(board, 0) << " | " << showCell(board, 1) << " | " << showCell(board, 2) << std::endl;
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 3) << " | " << showCell(board, 4) << " | " << showCell(board, 5) << std::endl;
    std::cout << "---+---+---\n";
    std::cout << " " << showCell(board, 6) << " | " << showCell(board, 7) << " | " << showCell(board, 8) << std::endl;
    std::cout << "\n" << std::endl;
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
        std::cout << "Player " << player << " — enter a move (1-9): "<< std::endl;
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Error: Please enter a number from 1 to 9.\n" << std::endl;
            continue;
        }
        if (choice < 1 || choice > 9) {
            std::cout << "Sorry out of bounds, choose 1-9.\n" << std::endl;
            continue;
        }
        int index = choice - 1;
        if (board[index] != ' ') {
            std::cout << "That cell is already taken. Choose another.\n" << std::endl;
            continue;
        }
        return index;
    }
}

bool boardFull(const std::vector<char>& b) {
    for (int i = 0; i < 9; ++i) if (b[i] == ' ') return false;
    return true;
}

int whichPlayerForMove(char mark, char p1Move, char p2Move) {
    if (mark == p1Move) return 1;
    if (mark == p2Move) return 2;
    return 0;
}

bool apprMoveChar(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    std::string allowed = "?!*~$%#";
    return allowed.find(c) != std::string::npos;
}

bool alchemSwap(std::vector<char>& board) {
    return alchemistSwap(board);
}

int aiRandomMove(const std::vector<char>& board) {
    std::vector<int> empties;
    for (int i = 0; i < 9; ++i) if (board[i] == ' ') empties.push_back(i);
    if (empties.empty()) return -1;
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, (int)empties.size() - 1);
    return empties[dist(rng)];
}

bool alchemistSwap(std::vector<char>& board) {
    int a, b;
    std::cout << "Enter first position to swap (1-9): ";
    if (!(std::cin >> a)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (a < 1 || a > 9) return false;
    int i = a - 1;
    if (board[i] == ' ') return false;
    std::cout << "Enter second position to swap (1-9): ";
    if (!(std::cin >> b)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (b < 1 || b > 9) return false;
    int j = b - 1;
    if (board[j] == ' ') return false;
    if (board[i] == board[j]) return false;
    std::swap(board[i], board[j]);
    return true;
}

bool paladinShift(std::vector<char>& board) {
    int a, b;
    std::cout << "Enter the position of the mark to shift (1-9): ";
    if (!(std::cin >> a)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (a < 1 || a > 9) return false;
    int from = a - 1;
    if (board[from] == ' ') return false;
    std::cout << "Enter the destination position (1-9): ";
    if (!(std::cin >> b)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
    if (b < 1 || b > 9) return false;
    int to = b - 1;
    if (board[to] != ' ') return false;
    int fr = from / 3, fc = from % 3, tr = to / 3, tc = to % 3;
    int dr = abs(fr - tr), dc = abs(fc - tc);
    if (!((dr <= 1 && dc <= 1) && !(dr == 0 && dc == 0))) return false;
    board[to] = board[from];
    board[from] = ' ';
    return true;
}

std::string toLower(const std::string& s) {
    std::string out = s;
    for (char &c : out) c = static_cast<char>(std::tolower((unsigned char)c));
    return out;
}

std::string promptArchetype(int playerNumber) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose archetype (Paladin, Alchemist, Chronomage): ";
        std::string s;
        if (!(std::cin >> s)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); continue; }
        std::string low = toLower(s);
        if (low == "paladin" || low == "alchemist" || low == "chronomage") return low;
    }
}

char promptForMove(int playerNumber, char otherPlayerMark) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose a single-character mark: ";
        std::string s;
        if (!(std::cin >> s)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); continue; }
        if (s.size() != 1) continue;
        char c = s[0];
        if (!apprMoveChar(c)) continue;
        if (otherPlayerMark != '\0' && c == otherPlayerMark) continue;
        return c;
    }
}

int countMoves(const std::vector<char>& board) {
    int cnt = 0;
    for (char c : board) if (c != ' ') ++cnt;
    return cnt;
}

void playRegular() {
    std::vector<char> board(9, ' ');
    char current = 'X';
    std::cout << "Player 1 is X || Player 2 is O\n" << std::endl;
    displayTable(board);
    while (true) {
        int idx = readMove(board, current);
        board[idx] = current;
        displayTable(board);
        char winner = checkWinner(board);
        if (winner != ' ') { std::cout << winner << " won\n"<< std::endl; break; }
        if (boardFull(board)) { std::cout << "It's a draw!!\n" << std::endl; break; }
        current = (current == 'X') ? 'O' : 'X';
    }
}

struct Entity {
    std::string name;
    std::string cls;
    int hp;
    int atk;
    int def;
    char mark;
    bool isBoss;
};

int damageFormula(int atk, int def) {
    int dmg = atk - def;
    return (dmg < 0 ? 0 : dmg);
}

char playSingleMatchAgainstAI(char playerMark, char aiMark) {
    std::vector<char> board(9, ' ');
    char current = playerMark;
    while (true) {
        if (current == playerMark) {
            int idx = readMove(board, current);
            board[idx] = current;
        } else {
            int idx = aiRandomMove(board);
            if (idx == -1) break;
            board[idx] = current;
            std::cout << "AI (" << current << ") plays at " << (idx + 1) << std::endl;
        }
        displayTable(board);
        char winner = checkWinner(board);
        if (winner != ' ') return winner;
        if (boardFull(board)) return 'D';
        current = (current == playerMark) ? aiMark : playerMark;
    }
    return 'D';
}

void eventHeal(Entity &player) {
    player.hp += 10;
    std::cout << "You discover a Fountain of Ambrosia. HP +10.\n";
    std::cout << "Current HP: " << player.hp << "\n\n";
}

void eventStatBoost(Entity &player) {
    player.atk++;
    player.def++;
    std::cout << "A statue of Athena grants you strength. ATK +1, DEF +1.\n";
    std::cout << "ATK: " << player.atk << " DEF: " << player.def << "\n\n";
}

void eventChoiceChest(Entity &player) {
    std::cout << "You find Pandora's Box. Open it? (y/n): ";
    char c;
    if (!(std::cin >> c)) return;
    if (c == 'y' || c == 'Y') {
        static std::mt19937 rng(time(nullptr));
        std::uniform_int_distribution<int> dist(0,2);
        int r = dist(rng);
        if (r == 0) { player.hp += 15; std::cout << "You gain HP +15.\n"; }
        else if (r == 1) { player.atk += 2; std::cout << "ATK +2.\n"; }
        else { player.def += 2; std::cout << "DEF +2.\n"; }
    }
    std::cout << "Status: HP " << player.hp << " ATK " << player.atk << " DEF " << player.def << "\n\n";
}

void finalBossSpecial(Entity &boss, Entity &player, int &dmgToPlayer, int &dmgToBoss) {
    static std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<int> dist(0,2);
    int r = dist(rng);
    if (r == 0) dmgToPlayer *= 2;
    else if (r == 1) dmgToBoss = damageFormula(player.atk, boss.def + 3);
    else { boss.hp += 10; dmgToBoss = 0; }
}

bool battleUntilDeath(Entity &player, Entity &opponent) {
    std::cout << "Battle start: " << player.name << " vs " << opponent.name << "\n";
    std::cout << player.name << " HP:" << player.hp << " ATK:" << player.atk << " DEF:" << player.def << "\n";
    std::cout << opponent.name << " HP:" << opponent.hp << " ATK:" << opponent.atk << " DEF:" << opponent.def << "\n\n";

    while (player.hp > 0 && opponent.hp > 0) {
        char matchWinner = playSingleMatchAgainstAI(player.mark, opponent.mark);
        if (matchWinner == 'D') {
            std::cout << "Match was a draw.\n\n";
        } else if (matchWinner == player.mark) {
            int dmg = damageFormula(player.atk, opponent.def);
            if (opponent.isBoss) {
                int damageToPlayer = 0, damageToBoss = dmg;
                finalBossSpecial(opponent, player, damageToPlayer, damageToBoss);
                opponent.hp -= damageToBoss;
                player.hp -= damageToPlayer;
            } else {
                opponent.hp -= dmg;
                std::cout << opponent.name << " takes " << dmg << " damage.\n\n";
            }
        } else {
            int dmg = damageFormula(opponent.atk, player.def);
            if (opponent.isBoss) {
                int damageToPlayer = dmg, damageToBoss = 0;
                finalBossSpecial(opponent, player, damageToPlayer, damageToBoss);
                player.hp -= damageToPlayer;
                opponent.hp -= damageToBoss;
            } else {
                player.hp -= dmg;
                std::cout << "You take " << dmg << " damage.\n\n";
            }
        }

        if (player.hp < 0) player.hp = 0;
        if (opponent.hp < 0) opponent.hp = 0;

        std::cout << player.name << " HP: " << player.hp << " | " << opponent.name << " HP: " << opponent.hp << "\n\n";
    }

    if (player.hp <= 0) {
        std::cout << "You were defeated by " << opponent.name << ".\n\n";
        return false;
    }
    std::cout << "You defeated " << opponent.name << "!\n\n";
    return true;
}

void playCampaign() {
    std::cout << "Starting Campaign Mode\n\n";

    Entity player;
    std::cout << "Enter your character name: ";
    std::getline(std::cin >> std::ws, player.name);

    while (true) {
        std::cout << "Choose class (Paladin/Alchemist): ";
        std::string cls;
        if (!(std::cin >> cls)) continue;
        std::string low = toLower(cls);
        if (low == "paladin" || low == "alchemist") { player.cls = low; break; }
    }

    player.hp = 50;
    player.atk = (player.cls == "paladin") ? 8 : 6;
    player.def = (player.cls == "paladin") ? 4 : 3;
    player.isBoss = false;

    std::cout << "Choose your mark for campaign: ";
    player.mark = promptForMove(0, '\0');

    std::vector<Entity> opponents = {
        {"Hermes", "god", 20, 5, 1, 'H', false},
        {"Ares", "god", 28, 7, 2, 'A', false},
        {"Athena", "god", 32, 6, 4, 'T', false},
        {"Cerberus", "beast", 40, 8, 5, 'C', false},
        {"Zeus", "king", 70, 10, 6, 'Z', true}
    };

    std::cout << "\nStory: You are " << player.name 
              << ", a " << player.cls 
              << " on a quest to defeat Zeus, King of Olympus.\n\n";

    int eventsDone = 0;

    for (int i = 0; i < (int)opponents.size(); ++i) {
        Entity &opp = opponents[i];

        if (opp.mark == player.mark) opp.mark += 1;

        bool won = battleUntilDeath(player, opp);
        if (!won) {
            std::cout << "Campaign failed. Restarting...\n\n";
            return;
        }

        if (i < (int)opponents.size() - 1 && eventsDone < 3) {
            if (eventsDone == 0) eventHeal(player);
            else if (eventsDone == 1) eventStatBoost(player);
            else eventChoiceChest(player);
            eventsDone++;
        }
    }

    std::cout << "Congratulations " << player.name 
              << "! You have defeated Zeus and completed the campaign.\n\n";
}

int main() {
    std::cout << "Welcome to the Tic Tac Toe Game!!\n" << std::endl;
    while (true) {
        std::cout << "Choose game type:\n1) regular\n2) battle\n3) campaign\nEnter 1, 2, or 3: ";
        int choice;
        if (!(std::cin >> choice)) { std::cin.clear(); std::cin.ignore(99999, '\n'); continue; }
        if (choice == 1) playRegular();
        else if (choice == 2) playBattle();
        else if (choice == 3) playCampaign();
        else continue;

        std::cout << "Play again? (y/n): ";
        char again;
        if (!(std::cin >> again)) break;
        if (again != 'y' && again != 'Y') break;
    }
    std::cout << "Thank you for playing Tic Tac Toe!!\n"<< std::endl;
    return 0;
}
