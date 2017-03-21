#pragma once
#include "stacks.h"
#include "moves.h"

class gameplay
{
public:
    int winningDepth;
    int currentDepth;
    int winningGames;
    int losingGames;
    int abortedGames;

    gameplay(void);
    ~gameplay(void);
    MOVES nextMove(STACK decks[]);
    GAMES dfs(STACK decks[],MOVES &moves);
    void writeMoves(MOVES &moves);
    void markWinner(GAMES &ret,MOVES &moves);
    bool checkExhaustion(MOVES &moves);
};

