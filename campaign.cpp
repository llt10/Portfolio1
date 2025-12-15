#include "campaign.hpp"
#include "battle.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

static int damage(int atk, int def) {
    int dmg = atk - def;
    return dmg < 1 ? 1 : dmg;
}

static void storyIntro() {
    std::cout << "\nYou awaken in a land shattered by endless war.\n";
    std::cout << "Five trials stand between you.\n\n";
}

static Character createPlayer() {
    Character p;
    std::cout << "Enter your hero's name: ";
    std::cin >> p.name;

    while (true) {
        std::cout << "Choose class (Paladin / Alchemist): ";
        std::cin >> p.archetype;
        if (p.archetype == "Paladin" || p.archetype == "Alchemist") break;
        std::cout << "Invalid choice.\n";
    }

    if (p.archetype == "Paladin") {
        p.health = 120;
        p.attack = 15;
        p.defense = 8;
    } else {
        p.health = 100;
        p.attack = 18;
        p.defense = 5;
    }

    p.gold = 20;
    return p;
}

static Enemy createEnemy(int stage) {
    Enemy e;
    e.isBoss = false;

    if (stage == 5) {
        e.name = "The Void King";
        e.health = 150;
        e.attack = 20;
        e.defense = 6;
        e.isBoss = true;
    } else {
        e.name = "Enemy " + std::to_string(stage);
        e.health = 60 + stage * 10;
        e.attack = 10 + stage * 2;
        e.defense = 4 + stage;
    }
    return e;
}

static void shop(Character& p) {
    std::cout << "\nMerchant Encounter!\n";
    std::cout << "Gold: " << p.gold << "\n";
    std::cout << "1) Heal (+20 HP) - 10g\n";
    std::cout << "2) Sword (+3 ATK) - 15g\n";
    std::cout << "3) Armor (+2 DEF) - 15g\n";
    std::cout << "4) Leave\n";

    int choice;
    std::cin >> choice;

    if (choice == 1 && p.gold >= 10) {
        p.health += 20;
        p.gold -= 10;
    } else if (choice == 2 && p.gold >= 15) {
        p.attack += 3;
        p.gold -= 15;
    } else if (choice == 3 && p.gold >= 15) {
        p.defense += 2;
        p.gold -= 15;
    }
}

static void randomEvent(Character& p) {
    int r = rand() % 3;
    if (r == 0) {
        std::cout << "\nYou rest by a fire. +15 HP\n";
        p.health += 15;
    } else if (r == 1) {
        std::cout << "\nA cursed mist weakens you. -2 DEF\n";
        if (p.defense > 0) p.defense -= 2;
    } else {
        shop(p);
    }
}

static bool fight(Character& p, Enemy& e) {
    std::cout << "\nBattle vs " << e.name << "\n";

    while (p.health > 0 && e.health > 0) {
        std::cout << "\nYou won the tic-tac-toe round!\n";
        e.health -= damage(p.attack, e.defense);

        if (e.health <= 0) break;

        std::cout << "Enemy strikes back!\n";
        int enemyAtk = e.attack;

        if (e.isBoss && rand() % 2 == 0) {
            std::cout << "Boss uses Shadows!\n";
            enemyAtk += 5;
        }

        p.health -= damage(enemyAtk, p.defense);
    }

    if (p.health > 0) {
        std::cout << e.name << " defeated!\n";
        p.gold += 10;
        return true;
    }

    return false;
}

void playCampaign() {
    srand(static_cast<unsigned>(time(nullptr)));

    storyIntro();
    Character player = createPlayer();

    for (int i = 1; i <= 5; i++) {
        Enemy enemy = createEnemy(i);

        if (!fight(player, enemy)) {
            std::cout << "\nYou have fallen. Restarting...\n";
            playCampaign();
            return;
        }

        if (i < 5) randomEvent(player);
    }

    std::cout << "\nThe land is saved. You are amazing, " << player.name << "!\n";
}
