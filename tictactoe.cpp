#include <iostream>
#include "battle.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
#include <string>

using namespace std;

struct Character {
    string name;
    string role;
    int health;
    int attack;
    int defense;
    int gold = 0;
};

char checkWinner(const vector<char>& b) {
    const int lines[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(int i=0;i<8;i++){
        int a=lines[i][0], b1=lines[i][1], c=lines[i][2];
        if(b[a]!=' ' && b[a]==b[b1] && b[b1]==b[c]) return b[a];
    }
    return ' ';
}

int readMove(const vector<char>& board, char player){
    while(true){
        int choice;
        if(player=='X') cout<<"Player "<<player<<" — enter a move (1-9): ";
        vector<int> options;
        for(int i=0;i<9;i++) if(board[i]==' ') options.push_back(i);
        if(player=='O'){ choice=options[rand()%options.size()]+1; cout<<"Enemy chooses position "<<choice<<"\n"; }
        else if(!(cin>>choice)){ cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); continue; }
        if(choice<1 || choice>9) continue;
        int index=choice-1;
        if(board[index]!=' ') continue;
        return index;
    }
}

int playTicTacToeRound() {
    vector<char> board(9,' ');
    while(true){
        int idx = readMove(board,'X'); board[idx]='X';
        if(checkWinner(board)=='X') return 1;
        if(all_of(board.begin(),board.end(),[](char c){return c!=' ';})) return 0;
        int enemyMove = readMove(board,'O'); board[enemyMove-1]='O';
        if(checkWinner(board)=='O') return -1;
        if(all_of(board.begin(),board.end(),[](char c){return c!=' ';})) return 0;
    }
}

void enemySpecial(Character& player, Character& enemy){
    int r=rand()%3;
    switch(r){
        case 0: player.health=max(0,player.health-3); break;
        case 1: enemy.defense+=2; break;
        case 2: enemy.health+=2; break;
    }
}

void finalBossSpecial(Character& player, Character& enemy){
    int r=rand()%3;
    switch(r){
        case 0: player.health=max(0,player.health-5); break;
        case 1: enemy.defense+=4; break;
        case 2: player.health=max(0,player.health-3); enemy.health+=3; break;
    }
}

bool battle(Character& player,int enemyNum,bool boss=false){
    Character enemy;
    enemy.name = boss?"Dark Overlord":"Enemy "+to_string(enemyNum);
    enemy.health = boss?60:20+enemyNum*5;
    enemy.attack = boss?10:5+enemyNum;
    enemy.defense = boss?6:2+enemyNum;

    cout<<"\nBattle vs "<<enemy.name<<"\n";

    while(player.health>0 && enemy.health>0){
        int result = playTicTacToeRound();
        if(result==1){ int dmg=max(0,player.attack-enemy.defense); enemy.health-=dmg; }
        else if(result==-1){ int dmg=max(0,enemy.attack-player.defense); player.health-=dmg; }

        if(boss) finalBossSpecial(player,enemy);
        else if(rand()%4==0) enemySpecial(player,enemy);
    }

    if(player.health<=0){ cout<<"You have fallen.\n"; return false; }
    else { cout<<"X won the battle!\n"; return true; }
}

void healingEvent(Character& player){ cout<<"You find a healing shrine. +10 HP\n"; player.health+=10; }
void curseEvent(Character& player){ cout<<"A cursed mist weakens you. -2 DEF\n"; player.defense=max(0,player.defense-2);}
void choiceEvent(Character& player){
    cout<<"A stranger offers you power.\n1) Gain +3 ATK\n2) Gain +5 HP\nChoice: ";
    int choice; cin>>choice;
    if(choice==1){ player.attack+=3; cout<<"Your attack increases!\n"; }
    else { player.health+=5; cout<<"Your health increases!\n"; }
}
void shopEvent(Character& player){
    cout<<"You find a shop. You have "<<player.gold<<" gold.\n1) +10 HP (5 gold)\n2) +2 ATK (7 gold)\n3) +2 DEF (7 gold)\n0) Leave\n";
    int choice=-1;
    while(choice!=0){
        cin>>choice;
        switch(choice){
            case 1: if(player.gold>=5){player.health+=10;player.gold-=5; cout<<"Bought +10 HP\n";} break;
            case 2: if(player.gold>=7){player.attack+=2;player.gold-=7; cout<<"Bought +2 ATK\n";} break;
            case 3: if(player.gold>=7){player.defense+=2;player.gold-=7; cout<<"Bought +2 DEF\n";} break;
        }
    }
}

void playCampaign(){
    Character player; char again;
    cout<<"\nYou awaken in a land shattered by endless war.\nFive trials stand between you.\n\n";
    cout<<"Enter your hero's name: "; cin>>player.name;
    cout<<"Choose class (Paladin / Alchemist): "; cin>>player.role;
    if(player.role=="Paladin"){player.health=50;player.attack=8;player.defense=6;}
    else{player.health=45;player.attack=6;player.defense=4;}

    if(!battle(player,1)) goto restart; healingEvent(player);
    if(!battle(player,2)) goto restart; curseEvent(player);
    if(!battle(player,3)) goto restart; choiceEvent(player);
    shopEvent(player);
    if(!battle(player,4)) goto restart; healingEvent(player);
    if(!battle(player,5,true)) goto restart;

    cout<<"You have completed the campaign! Congratulations!\n";
    cout<<"Play again? (y/n): "; cin>>again;
    if(again=='y'||again=='Y') playCampaign();
    return;

restart:
    cout<<"Restart campaign? (y/n): "; cin>>again;
    if(again=='y'||again=='Y') playCampaign();
}

void playRegular(){
    vector<char> board(9,' ');
    char current='X';
    cout<<"Regular Tic-Tac-Toe!\n";
    while(true){
        int idx=readMove(board,current);
        board[idx]=current;
        char winner=checkWinner(board);
        if(winner!=' '){ cout<<winner<<" won!\n"; break; }
        if(all_of(board.begin(),board.end(),[](char c){return c!=' ';})){ cout<<"Draw!\n"; break; }
        current=(current=='X')?'O':'X';
    }
}

void playBattle(){
    vector<char> board(9,' ');
    char current='X';
    cout<<"Battle mode!\n";
    while(true){
        int idx=readMove(board,current);
        board[idx]=current;
        char winner=checkWinner(board);
        if(winner!=' '){ cout<<winner<<" won the battle!\n"; break; }
        if(all_of(board.begin(),board.end(),[](char c){return c!=' ';})){ cout<<"Battle draw!\n"; break; }
        current=(current=='X')?'O':'X';
    }
}

int main(){
    srand(time(nullptr));
    cout<<"Welcome to the Tic Tac Toe Game!!\n\n";
    while(true){
        cout<<"Choose game type:\n1) regular\n2) battle\n3) campaign\nEnter 1, 2, or 3: ";
        int choice; if(!(cin>>choice)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n'); continue;}
        if(choice==1) playRegular();
        else if(choice==2) playBattle();
        else if(choice==3) playCampaign();
        else {cout<<"Invalid choice.\n"; continue;}
        char again; cout<<"Play again? (y/n): "; cin>>again; if(!(again=='y'||again=='Y')) break;
    }
    return 0;
}

