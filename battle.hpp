#ifndef BATTLE_HPP
#define BATTLE_HPP

#include <string>
#include <vector>

bool apprMoveChar(char c);
char promptForMove(int playerNumber, char otherPlayerMove);
std::string promptArchetype(int playerNumber);
int countMoves(const std::vector<char>& board);
bool alchemSwap(std::vector<char>& board);
bool alchemistSwap(std::vector<char>& board);
bool paladinShift(std::vector<char>& board);
bool isAdjacent(int from, int to);
int aiRandomMove(const std::vector<char>& board);

#endif
