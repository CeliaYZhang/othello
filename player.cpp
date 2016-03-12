#include "player.h"
#include <iostream>
#include <cstddef>
#include <utility>

using namespace std;

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds. 
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    mySide = side;
    opSide = WHITE;
    if(mySide == WHITE)
        opSide = BLACK;
    board = new Board();
}

/*
 * Destructor for the player. 
 */
Player::~Player() {
}

void Player::setBoard(Board *currState){
    board = currState;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    // Update board for opponents move.
    if (mySide == WHITE) 
        board->doMove(opponentsMove, BLACK);
    else
        board->doMove(opponentsMove, WHITE);

    cerr << "Made opponents move.\n";
    Move *skip = new Move(-1, -1);
    
    // Check for no more valid moves
    if (!board->hasMoves(mySide))
        return skip;

    cerr << "Checked for valid moves.\n";
    
    // Minmax function
    if (testingMinimax)
    {
        // Move *bestMove = skip;
        // int bestScore = -100000;
        // vector<Move*> moves = board->getMoves(mySide);
        // cerr << "Number of moves to consider is " << moves.size() << "\n";
        // for(int i = 0; i < (int)moves.size(); ++i) {
        //     Board *copyBoard = board->copy();
        //     int score = considerMove(copyBoard, moves[i], mySide, 2);
        //     cerr << score << endl;
        //     if(score > bestScore) {
        //         bestScore = score;
        //         bestMove = moves[i];
        //     }
        // }
        pair<int, Move*> bestMove = minimax(board, skip, mySide, 3);
        board->doMove(bestMove.second, mySide);
        cerr << bestMove.second->getX() << " "
             << bestMove.second->getY() << "\n";
        return bestMove.second;    
    }
    // Heuristic function (Notice that minimax for 1 level is the same
    // as just testing for the Heuristic function.)
    else
    {
        pair<int, Move*> bestMove = minimax(board, skip, mySide, 1);
        board->doMove(bestMove.second, mySide);
        return bestMove.second;
    }
    return NULL;
}

pair<int, Move*> Player::minimax(Board *currState, Move *move,
                                 Side currSide, int depth){
    vector<Move*> moves = currState->getMoves(currSide);
    int numMoves = (int)moves.size();
    if(depth == 0 || numMoves == 0){
        if(currSide == mySide)
            return make_pair(simpleHeuristic(currState, move, opSide), move);
            //return make_pair(diffHeuristic(currState, opSide), move);
        return make_pair(simpleHeuristic(currState, move, mySide), move);
        //return make_pair(diffHeuristic(currState, mySide), move);
        
        // return make_pair(simpleHeuristic(currState, move, mySide), move);
    }
    if(currSide == mySide){
        int bestScore = -100000;
        Move *bestMove = new Move(-1, -1);
        for(int i = 0; i < numMoves; ++i){
            Board *copyBoard = currState->copy();
            copyBoard->doMove(moves[i], currSide);
            pair<int, Move*> v = minimax(copyBoard, moves[i],
                                         opSide, depth - 1);
            if(v.first > bestScore){
                bestScore = v.first;
                bestMove = moves[i];
            }
            delete copyBoard;
        }
        return make_pair(bestScore, bestMove);
    }
    else{
        int bestScore = 100000;
        Move *bestMove = new Move(-1, -1);
        for(int i = 0; i < numMoves; ++i){
            Board *copyBoard = currState->copy();
            pair<int, Move*> v = minimax(copyBoard, moves[i],
                                         mySide, depth - 1);
            if(v.first < bestScore){
                bestScore = v.first;
                bestMove = moves[i];
            }
            delete copyBoard;
        }
        return make_pair(bestScore, bestMove);
    }
}

int Player::considerMove(Board *currState, Move *move,
                         Side currSide, int depth){
    currState->doMove(move, currSide);
    if(depth == 1){
        if(currSide != mySide)
            //return simpleHeuristic(currState, move, currSide);
            return -diffHeuristic(currState, currSide);
        return diffHeuristic(currState, currSide);
    }
    else{
        int bestScore = -100000;
        Side nxtSide = WHITE;
        if(currSide == WHITE)
            nxtSide = BLACK;
        
        vector<Move*> moves = currState->getMoves(nxtSide);
        if(moves.size() == 0)
            return simpleHeuristic(currState, move, currSide);

        for(int i = 0; i < (int)moves.size(); ++i) {
            //cerr << "Move: (" << moves[i]->getX() << ", "
            //<< moves[i]->getY()
            //      << ")\n";
            Board *copyBoard = currState->copy();
            int score = considerMove(copyBoard, moves[i],
                                     nxtSide, depth - 1);
            if(currSide != mySide)
                score = -score;
            cerr << score << endl;
            if(score > bestScore)
                bestScore = score;
            delete copyBoard;
        }
        return bestScore;
    }
}

int Player::diffHeuristic(Board *currState, Side side){
    int blacks = currState->countBlack();
    int whites = currState->countWhite();
    if(side == WHITE)
        return whites - blacks;
    return blacks - whites;
}

int Player::simpleHeuristic(Board *currState, Move* move, Side side){
    int blacks = board->countBlack();
    int whites = board->countWhite();
    Side nxtSide = WHITE;
    if(side == WHITE)
        nxtSide = BLACK;
    int numMoves = (currState->getMoves(nxtSide)).size();
    int cornerBonus = 0;

    if(move == NULL){
        if(side == WHITE)
            return whites - blacks - numMoves * 100;
        return blacks - whites - numMoves * 100;
    }

    // Check if the move was a corner place.
    if(move->getX() == 0 && move->getY() == 0)
        cornerBonus += 1000;
    if(move->getX() == 0 && move->getY() == 7)
        cornerBonus += 1000;
    if(move->getX() == 7 && move->getY() == 0)
        cornerBonus += 1000;
    if(move->getX() == 7 && move->getY() == 7)
        cornerBonus += 1000;

    // Check if the move was an edge place.
    if(move->getX() == 0 && move->getY() != 0 && move->getY() != 7)
        cornerBonus += 200;
    if(move->getX() == 7 && move->getY() != 0 && move->getY() != 7)
        cornerBonus += 200;
    if(move->getY() == 0 && move->getX() != 0 && move->getX() != 7)
        cornerBonus += 200;
    if(move->getY() == 7 && move->getX() != 0 && move->getX() != 7)
        cornerBonus += 200;

    // Check if the move was one to corner place.
    if(move->getX() == 0 && (move->getY() == 1 || move->getY() == 6))
        cornerBonus -= 400;
    if(move->getX() == 7 && (move->getY() == 1 || move->getY() == 6))
        cornerBonus -= 400;
    if(move->getY() == 0 && (move->getX() == 1 || move->getX() == 6))
        cornerBonus -= 400;
    if(move->getY() == 7 && (move->getX() == 1 ||  move->getX() == 6))
        cornerBonus -= 400;
    if(move->getX() == 1 && move->getY() == 1)
        cornerBonus -= 400;
    if(move->getX() == 1 && move->getY() == 6)
        cornerBonus -= 400;
    if(move->getX() == 6 && move->getY() == 1)
        cornerBonus -= 400;
    if(move->getX() == 6 && move->getY() == 6)
        cornerBonus -= 400;

    if(side == mySide){
        if(mySide == WHITE)
            return whites - blacks - numMoves * 100 + cornerBonus;
        return blacks - whites - numMoves * 100 + cornerBonus;
    }
    else{
        if(mySide == WHITE)
            return whites - blacks + numMoves * 100 - cornerBonus;
        return blacks - whites + numMoves * 100 - cornerBonus;
    }
    
    // if(side == WHITE)
    //     return whites - blacks - numMoves * 100 + cornerBonus;
    // return blacks - whites - numMoves * 100 + cornerBonus;
}
