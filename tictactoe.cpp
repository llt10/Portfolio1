#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>
#include <random>
#include <ctime>
#include "battle.hpp"

char showCell(const std::vector<char>& board, int i) {
    if (board[i] != ' ') {
        return board[i];
    } else {
        return static_cast<char>('1' + i);
    }
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
        if (b[a] != ' ' && b[a] == b[c] && b[c] == b[d]) {
            return b[a];
        }
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
    for (int i = 0; i < 9; ++i) {
        if (b[i] == ' ') {
            return false;
        }
    }
    return true;
}

int whichPlayerForMove(char mark, char p1Move, char p2Move) {
    if (mark == p1Move) return 1;
    if (mark == p2Move) return 2;
    return 0;
}

// wrapper to satisfy header name if implementation differs
bool apprMoveChar(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    std::string allowed = "?!*~$%#";
    return allowed.find(c) != std::string::npos;
}

// if header declares alchemSwap but implementation name is alchemistSwap in other code, provide wrapper
bool alchemSwap(std::vector<char>& board) {
    return alchemistSwap(board);
}

// --- simple AI for random move ---
int aiRandomMove(const std::vector<char>& board) {
    std::vector<int> empties;
    for (int i = 0; i < 9; ++i) if (board[i] == ' ') empties.push_back(i);
    if (empties.empty()) return -1;
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, (int)empties.size() - 1);
    return empties[dist(rng)];
}

// --- alchemistSwap implementation (kept name used above) ---
bool alchemistSwap(std::vector<char>& board) {
    std::cout << "Enter first position to swap (1-9): ";
    int a;
    if (!(std::cin >> a)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    if (a < 1 || a > 9) return false;
    int i = a - 1;
    if (board[i] == ' ') {
        std::cout << "No mark at first position.\n";
        return false;
    }
    std::cout << "Enter second position to swap (1-9): ";
    int b;
    if (!(std::cin >> b)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    if (b < 1 || b > 9) return false;
    int j = b - 1;
    if (board[j] == ' ') {
        std::cout << "No mark at second position.\n";
        return false;
    }
    if (board[i] == board[j]) {
        std::cout << "Cannot swap two identical marks.\n";
        return false;
    }
    std::swap(board[i], board[j]);
    return true;
}

// --- keep paladinShift implementation as declared in header ---
bool paladinShift(std::vector<char>& board) {
    std::cout << "Enter the position of the mark to shift (1-9): ";
    int a;
    if (!(std::cin >> a)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    if (a < 1 || a > 9) return false;
    int from = a - 1;
    if (board[from] == ' ') {
        std::cout << "No mark at that position.\n";
        return false;
    }
    std::cout << "Enter the destination position (1-9): ";
    int b;
    if (!(std::cin >> b)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    if (b < 1 || b > 9) return false;
    int to = b - 1;
    if (board[to] != ' ') {
        std::cout << "Destination is not empty.\n";
        return false;
    }
    if (!isAdjacent(from, to)) {
        std::cout << "Destination is not adjacent.\n";
        return false;
    }
    board[to] = board[from];
    board[from] = ' ';
    return true;
}

// --- header-declared helper implementations used above ---
std::string toLower(const std::string& s) {
    std::string out = s;
    for (char &c : out) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

std::string promptArchetype(int playerNumber) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose archetype (Paladin, Alchemist, Chronomage): ";
        std::string s;
        if (!(std::cin >> s)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::string low = toLower(s);
        if (low == "paladin" || low == "alchemist" || low == "chronomage") return low;
        std::cout << "Invalid archetype.\n";
    }
}

char promptForMove(int playerNumber, char otherPlayerMark) {
    while (true) {
        std::cout << "Player " << playerNumber << " choose a single-character mark: ";
        std::string s;
        if (!(std::cin >> s)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (s.size() != 1) {
            std::cout << "Enter exactly one character.\n";
            continue;
        }
        char c = s[0];
        if (!apprMoveChar(c)) {
            std::cout << "Invalid mark. Choose A-Z, a-z, or one of ? ! * ~ $ % #\n";
            continue;
        }
        if (otherPlayerMark != '\0' && c == otherPlayerMark) {
            std::cout << "That mark is already taken by the other player.\n";
            continue;
        }
        return c;
    }
}

int countMoves(const std::vector<char>& board) {
    int cnt = 0;
    for (char c : board) if (c != ' ') ++cnt;
    return cnt;
}

bool isAdjacent(int from, int to) {
    int fr = from / 3;
    int fc = from % 3;
    int tr = to / 3;
    int tc = to % 3;
    int dr = abs(fr - tr);
    int dc = abs(fc - tc);
    return (dr <= 1 && dc <= 1) && !(dr == 0 && dc == 0);
}

// --- Regular play (unchanged) ---
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
        if (winner != ' ') {
            std::cout << winner << " won\n"<< std::endl;
            break;
        }
        if (boardFull(board)) {
            std::cout << "It's a draw!!\n" << std::endl;
            break;
        }
        if (current == 'X') current = 'O';
        else current = 'X';
    }
}

// --- Battle mode (kept) ---
void playBattle() {
    std::cout << "Starting Battle Mode" << std::endl;
    char p1Move = promptForMove(1,'\0');
    char p2Move = promptForMove(2, p1Move);
    std::string p1Arche = promptArchetype(1);
    std::string p2Arche = promptArchetype(2);
    std::vector<char> board (9, ' ');
    char current = p1Move;
    std::cout << "Player 1: " << p1Move << " (" << p1Arche << ")\n" << std::endl;
    std::cout << "Player 2: " << p2Move << " (" << p2Arche << ")\n" << std::endl;
    displayTable(board);
    while (true) {
        int playerNum = (current == p1Move) ? 1: 2;
        std::string arche = (playerNum == 1) ? p1Arche : p2Arche;
        std::cout << " Player " << playerNum << " (" << current << ")'s turn. Archetype: " << arche << "\n" << std::endl;
        bool didAction = false;
        if (arche == "alchemist" || arche == "paladin" || arche == "chronomage" ) {
            std::cout << "Choose action:\n1) Regular move\n2) Special move\n" << std::endl;
            std::cout << "Enter 1 or 2: " << std::endl;
            int choice;
            if (!(std::cin>> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Wrong input (Default = Move)\n" <<std::endl;
                choice = 1;
            }
            if (choice ==1) {
                int idx = readMove(board, current);
                board[idx] = current;
                didAction = true;
            } else {
                if (arche == "alchemist") {
                    if (countMoves(board) < 2) {
                        std::cout << "Not enough moves to swap.\n" << std::endl;
                        int idx = readMove(board, current);
                        board[idx] = current;
                        didAction = true;
                    } else {
                        bool swapped = alchemSwap(board);
                        if (!swapped) {
                            std::cout << "Swap failed; performing regular move instead.\n" << std::endl;
                            int idx = readMove(board, current);
                            board[idx] = current;
                            didAction = true;
                        } else {
                            didAction = true;
                        }
                    }
                } else if (arche == "paladin") {
                    if (countMoves(board) < 1) {
                        std::cout << "No marks on board to shift. Choose a regular move instead.\n";
                        int idx = readMove(board, current);
                        board[idx] = current;
                        didAction = true;
                    } else {
                        bool shifted = paladinShift(board);
                        if (!shifted) {
                            std::cout << "Shift was not successful; performing regular move instead.\n";
                            int idx = readMove(board, current);
                            board[idx] = current;
                            didAction = true;
                        } else {
                            didAction = true;
                        }
                    }
                } else {
                    std::cout << "Chronomage ability not yet implemented. Performing regular move.\n";
                    int idx = readMove(board, current);
                    board[idx] = current;
                    didAction = true;
                }
            }
        } else {
            int idx = readMove(board, current);
            board[idx] = current;
            didAction = true;
        }
        displayTable(board);
        char winner = checkWinner(board);
        if (winner != ' ') {
            int who = whichPlayerForMove(winner, p1Move, p2Move);
            if (who == 1) {
                std::cout << "Player 1 (" << p1Move << ") won\n" << std::endl;
            } else if (who == 2) {
                std::cout << "Player 2 (" << p2Move << ") won\n" << std::endl;
            } else {
                std::cout << winner << " won\n" << std::endl;
            }
            break;
        }
        if (boardFull(board)) {
            std::cout << "It's a draw!!\n" << std::endl;
            break;
        }
        if (current == p1Move) current = p2Move;
        else current = p1Move;
    }
}

// --- Campaign system ---

struct Entity {
    std::string name;
    std::string cls; // "paladin" or "alchemist" or others
    int hp;
    int atk;
    int def;
    char mark;
    bool isBoss;
};

int damageFormula(int atk, int def) {
    int dmg = atk - def;
    if (dmg < 0) return 0;
    return dmg;
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
    int heal = 10;
    player.hp += heal;
    std::cout << "You find a healing spring. You recover " << heal << " health.\n";
    if (player.hp < 0) player.hp = 0;
    std::cout << "Current HP: " << player.hp << "\n\n";
}

void eventStatBoost(Entity &player) {
    int inc = 1;
    player.atk += inc;
    player.def += inc;
    std::cout << "You train with a wandering swordsman. ATK and DEF increased by " << inc << ".\n";
    if (player.atk < 0) player.atk = 0;
    if (player.def < 0) player.def = 0;
    std::cout << "ATK: " << player.atk << " DEF: " << player.def << "\n\n";
}

void eventChoiceChest(Entity &player) {
    std::cout << "You find a mysterious chest. Do you open it? (y/n): ";
    char c;
    if (!(std::cin >> c)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return; }
    if (c == 'y' || c == 'Y') {
        static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
        std::uniform_int_distribution<int> dist(0,2);
        int r = dist(rng);
        if (r == 0) { player.hp += 15; std::cout << "It contained a potion. HP +15.\n"; }
        else if (r == 1) { player.atk += 2; std::cout << "A sharp gem! ATK +2.\n"; }
        else { player.def += 2; std::cout << "A warding charm! DEF +2.\n"; }
    } else {
        std::cout << "You leave the chest alone.\n";
    }
    if (player.hp < 0) player.hp = 0;
    if (player.atk < 0) player.atk = 0;
    if (player.def < 0) player.def = 0;
    std::cout << "Status: HP " << player.hp << " ATK " << player.atk << " DEF " << player.def << "\n\n";
}

void finalBossSpecial(Entity &boss, Entity &player, int &appliedDamageToPlayer, int &appliedDamageToBoss) {
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0,2);
    int r = dist(rng);
    if (r == 0) {
        std::cout << boss.name << " uses a devastating strike! Damage doubled this round.\n";
        appliedDamageToPlayer *= 2;
    } else if (r == 1) {
        std::cout << boss.name << " raises a magical shield! Its defense increases temporarily.\n";
        int extra = 3;
        int reduced = damageFormula(player.atk, boss.def + extra);
        appliedDamageToBoss = reduced;
    } else {
        std::cout << boss.name << " consumes dark energy and heals!\n";
        int heal = 10;
        boss.hp += heal;
        appliedDamageToBoss = 0;
    }
}

bool battleUntilDeath(Entity &player, Entity opponent) {
    std::cout << "Battle start: " << player.name << " vs " << opponent.name << "\n";
    std::cout << player.name << " HP:" << player.hp << " ATK:" << player.atk << " DEF:" << player.def << "\n";
    std::cout << opponent.name << " HP:" << opponent.hp << " ATK:" << opponent.atk << " DEF:" << opponent.def << "\n\n";

    while (player.hp > 0 && opponent.hp > 0) {
        char matchWinner = playSingleMatchAgainstAI(player.mark, opponent.mark);
        if (matchWinner == 'D') {
            std::cout << "Match was a draw. No damage dealt.\n\n";
        } else if (matchWinner == player.mark) {
            int dmg = damageFormula(player.atk, opponent.def);
            if (opponent.isBoss) {
                int appliedToPlayer = 0;
                int appliedToBoss = dmg;
                finalBossSpecial(opponent, player, appliedToPlayer, appliedToBoss);
                opponent.hp -= appliedToBoss;
                if (appliedToPlayer > 0) player.hp -= appliedToPlayer;
            } else {
                opponent.hp -= dmg;
                std::cout << "You won the match. " << opponent.name << " takes " << dmg << " damage.\n\n";
            }
        } else {
            int dmg = damageFormula(opponent.atk, player.def);
            if (opponent.isBoss) {
                int appliedToPlayer = dmg;
                int appliedToBoss = 0;
                finalBossSpecial(opponent, player, appliedToPlayer, appliedToBoss);
                player.hp -= appliedToPlayer;
                if (appliedToBoss > 0) opponent.hp -= appliedToBoss;
            } else {
                player.hp -= dmg;
                std::cout << opponent.name << " won the match. You take " << dmg << " damage.\n\n";
            }
        }
        if (player.hp < 0) player.hp = 0;
        if (opponent.hp < 0) opponent.hp = 0;
        std::cout << player.name << " HP: " << player.hp << " | " << opponent.name << " HP: " << opponent.hp << "\n\n";
    }

    if (player.hp <= 0) {
        std::cout << "You have been defeated by " << opponent.name << ".\n\n";
        return false;
    } else {
        std::cout << "You defeated " << opponent.name << "!\n\n";
        return true;
    }
}

void playCampaign() {
    std::cout << "Starting Campaign Mode\n\n";
    Entity player;
    std::cout << "Enter your character name: ";
    std::getline(std::cin >> std::ws, player.name);
    while (true) {
        std::cout << "Choose class (Paladin/Alchemist): ";
        std::string cls;
        if (!(std::cin >> cls)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); continue; }
        std::string low = toLower(cls);
        if (low == "paladin" || low == "alchemist") { player.cls = low; break; }
        std::cout << "Invalid class.\n";
    }
    player.hp = 50;
    player.atk = (player.cls == "paladin") ? 8 : 6;
    player.def = (player.cls == "paladin") ? 4 : 3;
    player.isBoss = false;
    std::cout << "Choose your mark for campaign (single char): ";
    player.mark = promptForMove(0, '\0');

    std::vector<Entity> opponents;
    opponents.push_back({"Goblin Scout", "monster", 20, 4, 1, 'G', false});
    opponents.push_back({"Bandit Lieutenant", "villain", 25, 6, 2, 'B', false});
    opponents.push_back({"Witch of the Hollow", "monster", 30, 7, 3, 'W', false});
    opponents.push_back({"Iron Golem", "obstacle", 35, 9, 4, 'I', false});
    opponents.push_back({"Dragon Lord", "final", 60, 10, 5, 'D', true});

    std::cout << "\nStory: You are " << player.name << ", a " << player.cls << " on a quest to defeat the Dragon Lord.\n\n";

    int battlesNeeded = 5;
    int eventsNeeded = 3;
    int battlesDone = 0;
    int eventsDone = 0;

    for (int i = 0; i < (int)opponents.size(); ++i) {
        Entity opp = opponents[i];
        if (opp.isBoss) opp.isBoss = true;
        if (opp.mark == player.mark) opp.mark = (opp.mark == 'G') ? 'g' : opp.mark + 1;
        bool won = battleUntilDeath(player, opp);
        if (!won) {
            std::cout << "Campaign failed. Restarting campaign.\n\n";
            return;
        }
        battlesDone++;
        if (i < (int)opponents.size() - 1) {
            if (eventsDone < eventsNeeded) {
                // event
                if (eventsDone == 0) eventHeal(player);
                else if (eventsDone == 1) eventStatBoost(player);
                else eventChoiceChest(player);
                eventsDone++;
            }
        }
    }

    std::cout << "Congratulations " << player.name << "! You have completed the campaign and defeated the Dragon Lord.\n\n";
}

// --- main menu ---
int main() {
    std::cout << "Welcome to the Tic Tac Toe Game!!\n" << std::endl;
    while (true) {
        std::cout << "Choose game type:\n1) regular\n2) battle\n3) campaign\nEnter 1, 2, or 3: ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again.\n" << std::endl;
            continue;
        }
        if (choice == 1) {
            playRegular();
        } else if (choice == 2) {
            playBattle();
        } else if (choice == 3) {
            playCampaign();
        } else {
            std::cout << "Please enter 1, 2 or 3.\n" << std::endl;
            continue;
        }
        std::cout << "Play again? (y/n): ";
        char again;
        if (!(std::cin >> again)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
        if (again == 'y' || again == 'Y') {
            std::cout << "\nRestarting...\n" << std::endl;
            continue;
        } else {
            break;
        }
    }
    std::cout << "Thank you for playing Tic Tac Toe/Battlefield!!\n"<< std::endl;
    return 0;
}
