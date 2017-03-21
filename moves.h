#pragma once
#include "stacks.h"
#include <iostream>
#include <list>
using namespace std;


//a MOVE is necessary to construct a game tree, so we can search it, and
//move up and down it. It means that we have to be able to reverse
//previous moves.  It serves a dispatcher for the possible
//operations on piles.

//It's a friend of the solitaire game state, perhaps. Maybe closer.

// deal: from inital deck to tableau and talon
// tableau: move 1 from the waste pile to the tableau
// tableauTableau: move N cards from a tableau pile to a tableau pile
// foundation: move 1 from waste or tableau to foundation
// waste: move 1 or 3 from the talon to the waste
// talon: move all the cards from the waste back to the talon
typedef enum {mNone,mDeal,mTableau,mTableauTableau,mTableauTableauUncover,mFoundation,mFoundationUncover,mWaste,mTalon} WHATMOVE;

class MOVE {
friend class gameplay;
    WHATMOVE what;
    int frompile,topile;
    int count;
//each of the moves so recorded has action routines, that can do or undo the move
// mDeal is always the first entry in a list of game moves.
public:

MOVE(void);
~MOVE(void);
void addMove(WHATMOVE whatm, int fr, int to, int c,STACK decks[14]);
void writeMove(void);
void makeMove(STACK decks[14]);
void unmakeMove(STACK decks[14]);
void makeMove(STACK decks[14],bool makeornot);
};

typedef list<MOVE> MOVES;
typedef list<MOVES> GAMES;
