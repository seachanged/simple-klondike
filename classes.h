#pragma once
#include <list>
using namespace std;

/*
Copyright 2017 Daniel L. Liddell

This file is part of klondike.

Klondike is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// card FACE - an integer in the range of 1 to 52, arranged by suit
// club, spade, diamond, heart
// black first, then red
//
typedef enum {
    CA,C2,C3,C4,C5,C6,C7,C8,C9,C10,CJ,CQ,CK,
    SA,S2,S3,S4,S5,S6,S7,S8,S9,S10,SJ,SQ,SK,
    DA,D2,D3,D4,D5,D6,D7,D8,D9,D10,DJ,DQ,DK,
    HA,H2,H3,H4,H5,H6,H7,H8,H9,H10,HJ,HQ,HK} FACE;

typedef enum {C,S,D,H} SUIT;

class CARD {
private:
    FACE f;

public:
    CARD();
    void setFace(FACE face);
    FACE face();
    void setCard(char s,int r);
    SUIT suit();
    bool color();
    int rank();
};

// a stack of cards is a stack, of cards, with the addition of UP or DOWN
// Cards that are DOWN are always under cards that are UP
// A deck is a stack, and a foundation is a stack, and a tableu is a stack,
// and the talon is a stack, and the waste is a stack
//
enum {IDECK=0,ITALON=1,IWASTE=2,ITABLEAU=3,IFOUND=10};

class STACK {
private:
    // topDown is the topmost card that is face down
    // if topDown is equal to end(), then the whole deck is face up
    // if topDown is equal to top, then the whole deck is face down
    // top is the top of the pile
    list<CARD> pile;
    list<CARD>::iterator topDown, top;
    void markEmpty();
public:
    STACK();
    void create();
    void create(int whichone);
    void create(const char *d);
    void shuffle(int w);
    int length();
    void deal(STACK &pile,int count,bool upordown);
    CARD uncover();
    void cover();
    void move(CARD thiscard, STACK topile);
    void recycleWaste(STACK waste);

    CARD topCard();
    CARD bottomCard();
    bool empty();
    bool oneUp();
    bool allFaceDown();
    int upturned();
    bool allFaceUp();
};

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
