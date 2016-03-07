#include "player.h"
#include <iostream>

using namespace std;

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds. 
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = true;
    mySide = side;
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
        Move *bestMove = skip;
        int bestScore = -64;
        vector<Move*> moves = board->getMoves(mySide);
        cerr << "Number of moves to consider is " << moves.size() << "\n";
        for(int i = 0; i < (int)moves.size(); ++i) {
            Board *copyBoard = board->copy();
            int score = considerMove(copyBoard, moves[i], mySide, 2);
            cerr << score << endl;
            if(score > bestScore) {
                bestScore = score;
                bestMove = moves[i];
            }
        }
        board->doMove(bestMove, mySide);
        cerr << bestMove->getX() << " " << bestMove->getY() << "\n";
        return bestMove;    
    }
    // Heuristic function
    else
    {
        
    }
    return NULL;
}

int Player::considerMove(Board *currState, Move *move,
                         Side currSide, int depth){
    if(depth == 0){
        currState->doMove(move, currSide);
        return simpleHeuristic(currState, move, currSide);
    }
    else{
        currState->doMove(move, currSide);
        int bestScore = 0;
        Side nxtSide = WHITE;
        if(currSide == WHITE)
            nxtSide = BLACK;
        
        vector<Move*> moves = currState->getMoves(nxtSide);

        for(int i = 0; i < (int)moves.size(); ++i) {
            //cerr << "Move: (" << moves[i]->getX() << ", "
            //<< moves[i]->getY()
            //      << ")\n";
            Board *copyBoard = currState->copy();
            int score = considerMove(copyBoard, moves[i],
                                     nxtSide, depth - 1);
            if(score > bestScore)
                bestScore = score;
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
    int numMoves = (currState->getMoves(side)).size();
    int cornerBonus = 0;

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
        cornerBonus -= 300;
    if(move->getX() == 7 && (move->getY() == 1 || move->getY() == 6))
        cornerBonus -= 300;
    if(move->getY() == 0 && (move->getX() == 1 || move->getX() == 6))
        cornerBonus -= 300;
    if(move->getY() == 7 && (move->getX() == 1 ||  move->getX() == 6))
        cornerBonus -= 300;
    
    if(side == WHITE)
        return whites - blacks + numMoves * 100 + cornerBonus;
    return blacks - whites + numMoves * 100 + cornerBonus;
}
