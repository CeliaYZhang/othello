#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    void setBoard(Board *currstate);

    int diffHeuristic(Board *currState, Side side);
    int simpleHeuristic(Board *currState, Move *move, Side side);
    
    Move *doMove(Move *opponentsMove, int msLeft);
    int considerMove(Board *currState, Move *move, Side currSide, int depth);
    pair<int, Move*> minimax(Board *currState, Move *move, Side currSide, int depth);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    
    // Save which side we are on.
    Side mySide;
    Side opSide;
    
    // Game board state.
    Board *board;
};

#endif
