#include "campaign.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>



struct Character {
    std::string name;
    std::string role;
    int health;
    int attack;
    int defense;
};


int playSingleTicTacToeMatch() {
    int roll = rand() % 3;
    if (roll == 0) return 1;
    if (roll == 1) return -1;
    return 0;
}


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

//battle

void battle(Character& player, int enemyNumber, bool finalBoss = false) {
    Character enemy;
    enemy.name = finalBoss ? "The Dark Overlord" : "Enemy " + std::to_string(enemyNumber);
    enemy.health = finalBoss ? 40 : 25 + enemyNumber * 5;
    enemy.attack = finalBoss ? 8 : 5 + enemyNumber;
    enemy.defense = finalBoss ? 4 : 2 + enemyNumber / 2;

    std::cout << "\nBattle vs " << enemy.name << "\n";

    while (player.health > 0 && enemy.health > 0) {
        int result = playSingleTicTacToeMatch();

        if (result == 1) {
            int damage = std::max(0, player.attack - enemy.defense);
            enemy.health -= damage;
            std::cout << "You won the tic-tac-toe round! Enemy takes "
                      << damage << " damage.\n";
        }
        else if (result == -1) {
            int damage = std::max(0, enemy.attack - player.defense);

            if (finalBoss && rand() % 3 == 0) {
                std::cout << "The Dark Overlord uses Shadow Burst!\n";
                damage += 5;
            }

            player.health -= damage;
            std::cout << "Enemy won the round! You take "
                      << damage << " damage.\n";
        }
        else {
            std::cout << "The round was a draw. No damage dealt.\n";
        }

        std::cout << "Player HP: " << player.health
                  << " | Enemy HP: " << enemy.health << "\n\n";
    }

    if (player.health <= 0) {
        std::cout << "You have fallen. Restarting...\n\n";
        throw std::runtime_error("Player died");
    }

    std::cout << enemy.name << " defeated!\n";
}

//CAMPAIGN

void playCampaign() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Character player;
    std::cout << "\nYou awaken in a land shattered by endless war.\n";
    std::cout << "Five trials stand between you.\n\n";

    std::cout << "Enter your hero's name: ";
    std::cin >> player.name;

    std::cout << "Choose class (Paladin / Alchemist): ";
    std::cin >> player.role;

    player.health = 50;
    player.attack = (player.role == "Paladin") ? 8 : 6;
    player.defense = (player.role == "Paladin") ? 6 : 4;

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
        playCampaign(); // restart 
    }
}