#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <fstream>

struct Character {
    std::string name;
    std::string archetype;
    int health;
    int attack;
    int defense;
    int gold;
    int progress; // index of next enemy in campaign
};

struct Enemy {
    std::string name;
    int health;
    int attack;
    int defense;
    bool specialUsed;
};

// ------------------ Tic-Tac-Toe Functions ------------------

char showCell(const std::vector<char>& board, int i) {
    return board[i]!=' '? board[i]: static_cast<char>('1'+i);
}

void displayTable(const std::vector<char>& board){
    std::cout<<" "<<showCell(board,0)<<" | "<<showCell(board,1)<<" | "<<showCell(board,2)<<"\n";
    std::cout<<"---+---+---\n";
    std::cout<<" "<<showCell(board,3)<<" | "<<showCell(board,4)<<" | "<<showCell(board,5)<<"\n";
    std::cout<<"---+---+---\n";
    std::cout<<" "<<showCell(board,6)<<" | "<<showCell(board,7)<<" | "<<showCell(board,8)<<"\n\n";
}

bool boardFull(const std::vector<char>& b){for(char c:b) if(c==' ') return false; return true;}
char checkWinner(const std::vector<char>& b){
    const int lines[8][3]={{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(int i=0;i<8;i++){int a=lines[i][0],c=lines[i][1],d=lines[i][2];if(b[a]!=' ' && b[a]==b[c] && b[c]==b[d]) return b[a];}
    return ' ';
}

int readMove(const std::vector<char>& board,char player){
    while(true){
        std::cout<<"Player "<<player<<" enter move (1-9): ";
        int choice; if(!(std::cin>>choice)){std::cin.clear();std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');continue;}
        if(choice<1||choice>9){std::cout<<"Out of bounds\n";continue;}
        int idx=choice-1;if(board[idx]!=' '){std::cout<<"Cell occupied\n";continue;}
        return idx;
    }
}

bool validMarkChar(char c){if((c>='A'&&c<='Z')||(c>='a'&&c<='z')) return true; return std::string("?!*~$%#").find(c)!=std::string::npos;}
char promptForMove(int playerNumber,char otherPlayerMark){
    while(true){
        std::cout<<"Player "<<playerNumber<<" choose single-character mark: ";
        std::string s; if(!(std::cin>>s)){std::cin.clear();std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');continue;}
        if(s.size()!=1){std::cout<<"Enter exactly one character\n";continue;}
        char c=s[0]; if(!validMarkChar(c)){std::cout<<"Invalid mark\n";continue;}
        if(otherPlayerMark!='\0' && c==otherPlayerMark){std::cout<<"Mark taken\n";continue;}
        return c;
    }
}

std::string toLower(const std::string &s){std::string out=s;for(char &c:out)c=static_cast<char>(tolower(static_cast<unsigned char>(c)));return out;}
std::string promptArchetype(int playerNumber){
    while(true){
        std::cout<<"Player "<<playerNumber<<" choose archetype (Paladin, Alchemist): ";
        std::string s; if(!(std::cin>>s)){std::cin.clear();std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');continue;}
        std::string low=toLower(s); if(low=="paladin"||low=="alchemist") return low;
        std::cout<<"Invalid archetype\n";
    }
}

// ------------------ Regular Mode ------------------

void playRegular(){
    std::vector<char> board(9,' '); char current='X'; displayTable(board);
    while(true){
        int idx=readMove(board,current); board[idx]=current; displayTable(board);
        char winner=checkWinner(board); if(winner!=' '){std::cout<<winner<<" won!\n"; break;}
        if(boardFull(board)){std::cout<<"Draw!\n"; break;} current=(current=='X')?'O':'X';
    }
}

// ------------------ Battle Functions ------------------

int resolveRound(Character &player, Enemy &enemy){
    std::vector<char> board(9,' ');
    char pMark='X', eMark='O'; char current=pMark; displayTable(board);

    while(true){
        int idx;
        if(current==pMark) idx=readMove(board,current);
        else{
            std::vector<int> open; for(int i=0;i<9;i++) if(board[i]==' ') open.push_back(i);
            idx=open[rand()%open.size()];
            std::cout<<enemy.name<<" plays at "<<idx+1<<"\n";
        }
        board[idx]=current; displayTable(board);
        char winner=checkWinner(board);
        if(winner!=' ' || boardFull(board)){
            if(winner==pMark){ int dmg=player.attack-enemy.defense; if(dmg<0)dmg=0; enemy.health-=dmg; std::cout<<player.name<<" wins round! "<<enemy.name<<" loses "<<dmg<<" HP\n"; }
            else if(winner==eMark){ int dmg=enemy.attack-player.defense; if(dmg<0)dmg=0; player.health-=dmg; std::cout<<enemy.name<<" wins round! "<<player.name<<" loses "<<dmg<<" HP\n";}
            else std::cout<<"Round draw! No damage\n";
            std::cout<<"Health "<<player.name<<": "<<player.health<<" | "<<enemy.name<<": "<<enemy.health<<"\n\n";
            break;
        }
        current=(current==pMark)?eMark:pMark;
    }
    return (player.health<=0)?-1: (enemy.health<=0)?1:0;
}

void playBattle(Character &player, Enemy &enemy){
    while(player.health>0 && enemy.health>0){
        int result = resolveRound(player,enemy);
        if(result!=0) break;
        // enemy special: 20% chance to boost attack
        if(rand()%100<20 && !enemy.specialUsed){
            enemy.attack+=1; enemy.specialUsed=true;
            std::cout<<enemy.name<<" uses special power! Attack +1\n";
        }
    }
    if(player.health<=0) std::cout<<"You were defeated!\n";
    if(enemy.health<=0) std::cout<<enemy.name<<" defeated!\n";
}

// ------------------ Campaign Mode ------------------

void shop(Character &player){
    std::cout<<"\nWelcome to the shop! You have "<<player.gold<<" gold.\n";
    std::cout<<"1) Heal 5 HP (5 gold)\n2) Increase Attack +1 (5 gold)\n3) Increase Defense +1 (5 gold)\n4) Leave\nEnter choice: ";
    int choice; std::cin>>choice;
    if(choice==1 && player.gold>=5){player.health+=5; player.gold-=5; std::cout<<"Healed 5 HP!\n";}
    else if(choice==2 && player.gold>=5){player.attack+=1; player.gold-=5; std::cout<<"Attack +1!\n";}
    else if(choice==3 && player.gold>=5){player.defense+=1; player.gold-=5; std::cout<<"Defense +1!\n";}
    else std::cout<<"Leaving shop...\n";
}

void saveProgress(const Character &player){
    std::ofstream fout("savegame.txt");
    if(!fout) return;
    fout<<player.name<<"\n"<<player.archetype<<"\n"<<player.health<<"\n"<<player.attack<<"\n"<<player.defense<<"\n"<<player.gold<<"\n"<<player.progress<<"\n";
    fout.close();
}

bool loadProgress(Character &player){
    std::ifstream fin("savegame.txt");
    if(!fin) return false;
    fin>>player.name>>player.archetype>>player.health>>player.attack>>player.defense>>player.gold>>player.progress;
    fin.close();
    return true;
}

void playCampaign(){
    Character player;
    bool loaded=false;
    std::cout<<"Load previous campaign? (y/n): "; char load; std::cin>>load;
    if(load=='y'||load=='Y'){ loaded=loadProgress(player); }
    if(!loaded){
        std::cout<<"Welcome Hero! Enter your name: "; std::cin>>player.name;
        player.archetype=promptArchetype(1);
        player.health=20; player.attack=5; player.defense=2; player.gold=10; player.progress=0;
    }

    std::vector<Enemy> enemies={
        {"Ares",10,4,1,false},{"Athena",12,3,2,false},{"Hades",15,5,3,false},{"Zeus",20,6,4,false},{"Poseidon",25,7,5,false}
    };

    for(size_t i=player.progress;i<enemies.size();i++){
        Enemy &e=enemies[i]; std::cout<<"\nBattle against "<<e.name<<" begins!\n";
        playBattle(player,e);
        if(player.health<=0){ std::cout<<"You lost the campaign...\n"; return; }
        player.gold+=5;
        std::cout<<"You earned 5 gold! Total: "<<player.gold<<"\n";

        if(i<enemies.size()-1){
            std::cout<<"\nEvent! Choose:\n1) Heal 5 HP\n2) Increase Attack 1\n3) Visit shop\nEnter choice: ";
            int choice; std::cin>>choice;
            if(choice==1){player.health+=5; std::cout<<"Healed 5 HP!\n";}
            else if(choice==2){player.attack+=1; std::cout<<"Attack +1!\n";}
            else if(choice==3) shop(player);
            std::cout<<"Stats - HP: "<<player.health<<" ATK: "<<player.attack<<" DEF: "<<player.defense<<" Gold: "<<player.gold<<"\n";
        }

        player.progress=i+1;
        saveProgress(player);
    }

    Enemy &finalBoss=enemies.back();
    while(player.health>0 && finalBoss.health>0){
        int action=rand()%2;
        if(action==0){ finalBoss.attack+=2; std::cout<<"Zeus uses Lightning Strike! Attack +2 this round.\n"; }
        else{ finalBoss.health+=3; std::cout<<"Zeus uses Divine Heal! Restores 3 HP.\n"; }
        playBattle(player,finalBoss);
    }

    if(player.health>0){ std::cout<<"\nCongratulations "<<player.name<<"! You defeated all the Greek gods!\n"; remove("savegame.txt"); }
    else std::cout<<"\nYou were defeated in the final battle...\n";
}

// ------------------ Main ------------------

int main(){
    srand(static_cast<unsigned int>(time(0)));
    while(true){
        std::cout<<"Choose game type:\n1) Regular\n2) Battle\n3) Campaign\nEnter choice: ";
        int choice; if(!(std::cin>>choice)){ std::cin.clear();std::cin.igno
        }}}



