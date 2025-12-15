#include "campaign.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <stdexcept>

// tictactoe
char checkWinner(const std::vector<char>&);
bool boardFull(const std::vector<char>&);
void displayTable(const std::vector<char>&);
int readMove(const std::vector<char>&, char);


int playCampaignTicTacToe() {
    std::vector<char> board(9, ' ');
    char playerMark = 'X';
    char enemyMark = 'O';
    char current = playerMark;

    displayTable(board);

    while (true) {
        if (current == playerMark) {
            int move = readMove(board, playerMark);
            board[move] = playerMark;
        } else {
            // Enemy random move
            std::vector<int> empty;
            for (int i = 0; i < 9; ++i)
                if (board[i] == ' ')
                    empty.push_back(i);

            int choice = empty[rand() % empty.size()];
            board[choice] = enemyMark;
            std::cout << "Enemy chooses position " << (choice + 1) << "\n";
        }

        displayTable(board);

        char winner = checkWinner(board);
        if (winner == playerMark) return 1;
        if (winner == enemyMark) return -1;
        if (boardFull(board)) return 0;

        current = (current == playerMark) ? enemyMark : playerMark;
    }
}

// ---------------- EVENTS ----------------

void healingEvent(Character& player) {
    std::cout << "You find a healing shrine. +10 HP\n";
    player.health += 10;
}

void curseEvent(Character& player) {
    std::cout << "A cursed mist weakens you. -2 DEF\n";
    player.defense = std::max(0, player.defense - 2);
}

void choiceEvent(Character& player) {
    std::cout << "A stranger offers you power.\n";
    std::cout << "1) Gain +3 ATK\n";
    std::cout << "2) Gain +5 HP\n";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        player.attack += 3;
        std::cout << "Your attack increases!\n";
    } else {
        player.health += 5;
        std::cout << "You feel healthier!\n";
    }
}



void battle(Character& player, int enemyNumber, bool finalBoss = false) {
    Character enemy;
    enemy.name = finalBoss ? "The Dark Overlord"
                           : "Enemy " + std::to_string(enemyNumber);
    enemy.health = finalBoss ? 40 : 25 + enemyNumber * 5;
    enemy.attack = finalBoss ? 8 : 5 + enemyNumber;
    enemy.defense = finalBoss ? 4 : 2 + enemyNumber / 2;

    std::cout << "\nBattle vs " << enemy.name << "\n";

    while (player.health > 0 && enemy.health > 0) {
        int result = playCampaignTicTacToe();

        if (result == 1) {
            int dmg = std::max(0, player.attack - enemy.defense);
            enemy.health -= dmg;
            std::cout << "You won the match! Enemy takes "
                      << dmg << " damage.\n";
        }
        else if (result == -1) {
            int dmg = std::max(0, enemy.attack - player.defense);

            if (finalBoss && rand() % 3 == 0) {
                std::cout << "The Dark Overlord unleashes Shadow Burst!\n";
                dmg += 5;
            }

            player.health -= dmg;
            std::cout << "Enemy won the match! You take "
                      << dmg << " damage.\n";
        }
        else {
            std::cout << "The match was a draw. No damage dealt.\n";
        }

        std::cout << "Player HP: " << player.health
                  << " | Enemy HP: " << enemy.health << "\n\n";
    }

    if (player.health <= 0) {
        std::cout << "You have fallen. Restarting campaign...\n\n";
        throw std::runtime_error("dead");
    }

    std::cout << enemy.name << " defeated!\n";
}

// ---------------- CAMPAIGN ----------------

void playCampaign() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Character player;
    std::string classType;

    std::cout << "\nYou awaken in a land shattered by endless war.\n";
    std::cout << "Five trials stand between you.\n\n";

    std::cout << "Enter your hero's name: ";
    std::cin >> player.name;

    std::cout << "Choose class (Paladin / Alchemist): ";
    std::cin >> classType;

    player.health = 50;

    if (classType == "Paladin" || classType == "paladin") {
        player.attack = 8;
        player.defense = 6;
    } else {
        player.attack = 6;
        player.defense = 4;
    }

    try {
        battle(player, 1);
        healingEvent(player);

        battle(player, 2);
        curseEvent(player);

        battle(player, 3);
        choiceEvent(player);

        battle(player, 4);
        healingEvent(player);

        battle(player, 5, true); 

        std::cout << "\nYou have conquered the campaign!\n";
        std::cout << "Peace returns to the land. Victory is yours.\n\n";
    }
    catch (...) {
        playCampaign(); 
    }
}
