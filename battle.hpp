#ifndef BATTLE_HPP
#define BATTLE_HPP

#include <iostream>
#include <string> 
#include <vector>
#include <algorithm>

bool apprMoveChar(char c);

char promptForMove(int playerNumber, char otherPlayerMove);

std::string promptArchetype(int playerNumber);

int countMoves(const std::vector<char>& board);

bool alchemSwap (std::vector<char>& board);

bool paladinShift(Std::vector<char>& board);

bool isAdjacent(int from, int to); 


#endif