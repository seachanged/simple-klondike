#include "classes.h"
#include "options.h"
#include <assert.h>
#include <iterator>
#include <list>
#include <cstdlib>
#include <string.h>
#include <iostream>

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

CARD::CARD(){
    setFace(CA);
}

void CARD::setFace(FACE face) {
    f = face;
}

FACE CARD::face() {
    return(f);
}

void CARD::setCard(char s,int r) {
    switch (s) {
        case 'c':f = (FACE) 0; break;
        case 's':f = (FACE) 13; break;
        case 'd':f = (FACE) 26; break;
        case 'h':f = (FACE) 39; break;
        default: throw 3;
    }
    if (r < 1 || r > 13) throw 5;
    f = (FACE) ((int) f + r - 1);
}

SUIT CARD::suit(){
    switch (f / 13)  {
        case 0:return C;
        case 1:return S;
        case 2:return D;
        default:return H;
    };
}

bool CARD::color(){
    return(f < 26);
}

int CARD::rank(){
    return(1 + f % 13);
}

STACK::STACK(){
    markEmpty();
}

void STACK::markEmpty(){
    topDown = pile.end();
    top = pile.begin();
}


// allocate and initialize the 52 cards

void STACK::create(const char *d) {
    pile.clear();
    int state = 0;
    int i = 0;

    // the rank and suit
    int r = 0;
    char s = ' ';

    while (i < (int) strlen(d)) {
        char c = d[i++];
        switch (state) {
            case 0:
                // waiting for the start of a new card
                state = 2;
                if ('2' <= c && c <= '9')
                    r = c - '2' + 2;
                else if (c == 'a')
                    r = 1;
                else if (c == 'j')
                    r = 11;
                else if (c == 'q')
                    r = 12;
                else if (c == 'k')
                    r = 13;
                else if (c == '1')
                    state = 1;
                else if (c == ' ')
                    state = 0;
                else throw 3;
                break;
            case 1:
                // waiting for the 0
                if (c == '0') {
                    state = 2;
                    r = 10;
                }
                else throw 5;
                break;
            case 2:
                // waiting for the suit
                if (c == 'd' || c == 's' || c == 'h' || c == 'c') {
                    state = 0;
                    CARD card;
                    card.setCard(c,r);
                    pile.push_back(card);
                }
                else throw 7;
                break;
        }
    }
    // check for incomplete deck
    if (pile.size() != 52) throw 11;

    // check for unfinished card
    if (state != 0) throw 13;

    // check for duplicate card
    bool used[52];
    for (int i = 0; i < 52; i++) used[i] = false;
    list<CARD>::iterator it = pile.begin();
    while (it == pile.end()) {
        if (used[it->face()]) throw 17;
        used[it->face()] = true;
    }

    // no missing card possible
}

void STACK::create(void) {
    create(0);
}
void STACK::create(int whichone){
    switch (whichone) {
    case 0:
        for (int i = CA; i <= HK; i++) {
            CARD c;
            c.setFace((FACE) i);
            pile.push_front(c);
        }
        break;
    default:
        for (int i = HK; i >= CA; i--) {
            CARD c;
            c.setFace((FACE) i);
            pile.push_front(c);
        }
        break;
    }

    topDown = pile.begin();
    top = topDown;
}

// shuffle the cards
void STACK::shuffle(int w){
    if (w == 0) return;
    // get some unitary shuffles out of the way. this shuffle is repeatable
    while (w-- > 0) {
        for (int i = 0; i < 1000; i++) rand();
    }
    // do the real shuffle.
    for (int i = 0; i < 1000; i++){
        // select two random locations in the deck ..
        int f = rand();
        if (f < 0) f = 0 - f;
        f = f % 52;

        int t = rand();
        if (t < 0) t = 0 - t;
        t = t % 52;

        list<CARD>::iterator fi = pile.begin();
        list<CARD>::iterator ti = pile.begin();
        while (f > 0) {f--; fi++;}
        while (t > 0) {t--; ti++;}

        // .. and exchange the cards there
        CARD temp = *fi;
        *fi = *ti;
        *ti = temp;
    }
}

int STACK::length() {
    return pile.size();
}

// deal the cards, up or down
// cards are never dealt face down on top of cards that are face up.
// the source deck can be either face up or face down.
//
void STACK::deal(STACK &dest,int count,bool upordown){
    bool destempty = dest.pile.empty();
    bool sourceDown = (topDown == top);
    if ((int) pile.size() >= count) {
        while (count-- > 0) {
            dest.pile.splice(dest.pile.begin(),pile,pile.begin());
            // fix up the source deck. If we are moving cards that are
            // facedown, then we need to update the topDown pointer
            top = pile.begin();
            if (sourceDown) topDown = top;

            // fix up the destination deck
            dest.top = dest.pile.begin();
            if (destempty) {
                // if it is the first card of the destination, then
                // init the destination as face up or face down
                if (upordown)
                    dest.topDown = dest.pile.end();
                else dest.topDown = dest.top;
                destempty = false;
            }
            else if (!upordown) {
                // if not the first card in destination, and we
                // are dealing cards that are face down, we know they
                // are on top
                dest.topDown = dest.top;
            }
        }

        // done moving cards from the source to the destination.
        // if the source is empty, we have to fix it up
        if (pile.empty()) markEmpty();
    }
}

// uncover top card of deck. must be cards. must be face down
CARD STACK::uncover(){
    if (top == topDown) {
        CARD c = *topDown;
        topDown++;
        return c;
    }
    assert(false); // "uncover on an empty pile or pile already face up";
}

void STACK::cover(){
    if (std::next(top) == topDown) {
        topDown--;
        return;
    }
    assert(false); // "cover on an empty pile or face down over face up";
}

// move a card, face up to face up
void STACK::move(CARD thiscard, STACK topile){
}

// recycle cards from waste to talon
void STACK::recycleWaste(STACK waste) {
}

// retrieve top visible card on top of the pile
CARD STACK::topCard(){
    if (!pile.empty())
        if (top != topDown)
            return *top;
    assert(false); // "no topCard on empty pile, or pile with no visible top card";
}

// retrieve bottommost visible card
CARD STACK::bottomCard(){
    if (!pile.empty())
        if (top != topDown)
            return *std::prev(topDown);
    assert(false); // "no bottomCard on pile with no cards showing";
}
// is the deck empty?
bool STACK::empty(){
    return pile.empty();
}

// is there a single card showing over some number of face-down cards?
bool STACK::oneUp(){
    return top != topDown && std::next(top) == topDown && topDown != pile.end();
}

bool STACK::allFaceDown(){
    return top == topDown;
}

// is the deck showing any upturned cards?
int STACK::upturned(){
    if (allFaceDown()) return 0;
    int c = 0;
    for (list<CARD>::iterator t = top; t != topDown; t++, c++) ;
    return c;
}
// is the deck all face up
bool STACK::allFaceUp(){
    return topDown == pile.end();
}

MOVE::MOVE(void){
}

MOVE::~MOVE(void){
}

void MOVE::addMove(WHATMOVE whatm, int fr, int to, int c,STACK decks[14]){
    what = whatm;
    frompile = fr;
    topile = to;
    count = c;
}

void MOVE::writeMove(){
    switch (what) {
    case mNone:
        cout << "none";
        break;
    case mDeal:
        break;
    case mTableau:
    case mTableauTableau:
    case mTableauTableauUncover:
        if (count != 1) cout << "all of ";
        if (frompile == IWASTE)
            cout << "waste";
        else cout << frompile - ITABLEAU + 1;
        cout << " to "; cout << topile - ITABLEAU + 1;
        break;
    case mFoundation:
    case mFoundationUncover:
        if (frompile == IWASTE)
            cout << "waste";
        else cout << frompile - ITABLEAU + 1;
        cout << " to foundation "; cout << topile - IFOUND + 1;
        break;
    case mWaste:
        cout << "waste";
        break;
    case mTalon:
        cout << "recycle waste";
        break;
    }
}

// make or unmake the last move on the list of moves
//
void MOVE::makeMove(STACK decks[14]){
    makeMove(decks,true);
}

void MOVE::unmakeMove(STACK decks[14]){
    makeMove(decks,false);
};

void MOVE::makeMove(STACK decks[14],bool makeornot){
    switch (what) {
    case mDeal:
        if (makeornot) {
            if (options::getOptionBool(ODeck))
                decks[IDECK].shuffle(options::getOptionInt(ODeck));
            else decks[IDECK].shuffle(0);

            for (int i = 0; i < 7; i++)
                for (int j = i; j < 7; j++)
                    if (i == j)
                        decks[IDECK].deal(decks[ITABLEAU + j],1,true);
                    else decks[IDECK].deal(decks[ITABLEAU + j],1,false);

            // move the deck to the talon, so they end up in the same
            // order as if you had just moved the deck to the talon
            //
            decks[IDECK].deal(decks[IWASTE],decks[IDECK].length(),false);
            decks[IWASTE].deal(decks[ITALON],decks[IWASTE].length(),false);
        }
        else {
            // unmake the tableau, and put it back into the deck
            // assert(false);
            // don't unmake the tableau. the deck is reproduceable
        }
        break;
    case mTableau:
        if (makeornot) {
            decks[frompile].deal(decks[topile],1,true);
        }
        else {
            decks[topile].deal(decks[frompile],1,true);
        }
        break;
    case mTableauTableau:
    case mTableauTableauUncover:
        if (makeornot) {
            // dealing twice maintains card order in the cards we move
            // We know the deck is empty at this time. the "reversing trick".
            decks[frompile].deal(decks[IDECK],count,true);
            decks[IDECK].deal(decks[topile],count,true);
            if (!decks[frompile].empty()) {
                // if the top card is face down, we need to remember
                // we uncovered a card, so we can undo the move faithfully.
                // this knowledge is easier to get here than in nextMove
                if (decks[frompile].allFaceDown())
                    what = mTableauTableauUncover;

                if (mTableauTableauUncover == what)
                    decks[frompile].uncover();
            }
        }
        else {
            if (mTableauTableauUncover == what)
                decks[frompile].cover();
            // dealing twice maintains card order in the cards we move:
            // the "reversing trick"
            decks[topile].deal(decks[IDECK],count,true);
            decks[IDECK].deal(decks[frompile],count,true);
        }
        break;
    case mFoundation:
    case mFoundationUncover:
        // one face-up card to foundation, or vice-versa
        if (makeornot) {
            decks[frompile].deal(decks[topile],1,true);
            if (mFoundationUncover == what)
                decks[frompile].uncover();
        }
        else {
            if (mFoundationUncover == what)
                decks[frompile].cover();
            decks[topile].deal(decks[frompile],1,true);
        }
        break;

    case mWaste:
        if (makeornot) {
            decks[frompile].deal(decks[topile],1,true);
        }
        else {
            decks[topile].deal(decks[frompile],1,false);
        }
        break;
    case mTalon:
        if (makeornot) {
            // see "reversing trick" above
            decks[frompile].deal(decks[IDECK],count,false);
            decks[IDECK].deal(decks[topile],count,false);
        }
        else {
            // see "reversing trick" above
            decks[topile].deal(decks[IDECK],count,true);
            decks[IDECK].deal(decks[frompile],count,true);
        }
        break;
    default:
        break;
    }
};

gameplay::gameplay(void)
{
    winningDepth = -1;
    currentDepth = 0;
    winningGames = 0; // debug. easy to keep track this way in the debugger
    losingGames = 0; // debug
    abortedGames = 0; // debug
}

gameplay::~gameplay(void)
{
}

// for the current node in the game tree, compute a list of all possible
// moves in this turn, and return it as a list of moves.
MOVES gameplay::nextMove(STACK decks[14]){
    MOVES thisply;

    // waste-to-tableau
    //     rank/color matching the top card of the waste with top tableau card
    //     king move special case
    if (! decks[IWASTE].empty())
        if (0 != decks[IWASTE].upturned())
            for (int i = ITABLEAU; i < ITABLEAU+7; i++)
                if (! decks[i].empty()) {
                    if (decks[IWASTE].topCard().color() == decks[i].topCard().color())
                        continue;
                    if (decks[IWASTE].topCard().rank()+1 != decks[i].topCard().rank())
                        continue;
                    // card from the waste to the tableau at i
                    MOVE m; m.addMove(mTableau,IWASTE,i,1,decks);
                    thisply.push_back(m);
                }
                else if (13 == decks[IWASTE].topCard().rank()) {
                    // king from the waste to the to the tableau at i
                    MOVE m; m.addMove(mTableau,IWASTE,i,1,decks);
                    thisply.push_back(m);
                }

    // intra-tableau
    //     rank/color matching the bottom card with the the top card
    //     king move special case
    for (int i = ITABLEAU; i < ITABLEAU+7; i++)
        for (int j = ITABLEAU; j < ITABLEAU+7; j++)
            if (i != j)
                if (! decks[i].empty())
                    if (! decks[j].empty()) {
                        if (decks[i].bottomCard().color() == decks[j].topCard().color())
                            continue;
                        if (decks[i].bottomCard().rank()+1 != decks[j].topCard().rank())
                            continue;

                        // entire stack at i onto j
                        // this move can be transformed to an mTableauTableauUncover in MOVES::makeMove(), too
                        WHATMOVE t = mTableauTableau;
                        if (decks[i].oneUp()) t = mTableauTableauUncover;
                        MOVE m; m.addMove(t,i,j,decks[i].upturned(),decks);
                        thisply.push_back(m);
                    }
                    else if (13 == decks[i].bottomCard().rank()) {
                        if (decks[i].allFaceUp()) continue; // king already at the bottom
                        // king and its stack from i to j

                        // this move can be transformed to an
                        // mTableauTableauUncover in
                        // MOVES::makeMove(), too

                        WHATMOVE t = mTableauTableau;
                        if (decks[i].oneUp()) t = mTableauTableauUncover;
                        MOVE m; m.addMove(t,i,j,decks[i].upturned(),decks);
                        thisply.push_back(m);
                    }

    // tableau-to-foundation
    //     suit/rank matching the top tableau card with the foundation
    //     ace move special case
    for (int i = ITABLEAU; i < ITABLEAU+7; i++)
        if (! decks[i].empty()) {
            int fix = IFOUND + decks[i].topCard().suit();
            if (! decks[fix].empty()) {
                if (decks[i].topCard().color() != decks[fix].topCard().color())
                    continue;
                if (decks[i].topCard().rank()-1 != decks[fix].topCard().rank())
                    continue;

                // card from the tableau at i to the foundation.
                WHATMOVE t = mFoundation;
                if (decks[i].oneUp()) t = mFoundationUncover;
                MOVE m; m.addMove(t,i,fix,1,decks);
                thisply.push_back(m);
            }
            else if (1 == decks[i].topCard().rank()) {
                // ace from the tableau at i to the foundation
                WHATMOVE t = mFoundation;
                if (decks[i].oneUp()) t = mFoundationUncover;
                MOVE m; m.addMove(t,i,fix,1,decks);
                thisply.push_back(m);
            }
        }

    // waste-to-foundation
    //     suit/rank matching top card of waste to top card foundation
    //     ace move special case
    do {
        if (! decks[IWASTE].empty()) {
            int fix = IFOUND + decks[IWASTE].topCard().suit();
            if (! decks[fix].empty()) {
                if (decks[IWASTE].topCard().color() != decks[fix].topCard().color())
                    continue;
                if (decks[IWASTE].topCard().rank()-1 != decks[fix].topCard().rank())
                    continue;
                // card from the waste at i to the foundation
                MOVE m; m.addMove(mFoundation,IWASTE,fix,1,decks);
                thisply.push_back(m);
            }
            else if (1 == decks[IWASTE].topCard().rank()) {
                // ace from the waste at i to the foundation
                MOVE m; m.addMove(mFoundation,IWASTE,fix,1,decks);
                thisply.push_back(m);
            }
        }
    } while (false);

    // if we don't have any moves, add to the waste
    if (thisply.size() == 0) {
        // talon-to-waste
        //     turning one card
        // waste-to-talon
        //     recycling deck
        if (! decks[ITALON].empty()) {
            // we can move one card from the talon to the waste
            MOVE m; m.addMove(mWaste,ITALON,IWASTE,1,decks);
            thisply.push_back(m);
        }
        else if (! decks[IWASTE].empty()) {
            // we can recycle the entire waste deck back to the talon
            MOVE m; m.addMove(mTalon,IWASTE,ITALON,decks[IWASTE].length(),decks);
            thisply.push_back(m);
        }
    }

    return thisply;
}

// go through the moves in the movetree that we have been passed,
// one at a time. For each possible play, add the play to the moves
// in the parameter list,
//
GAMES gameplay::dfs(STACK decks[],MOVES &moves){
    currentDepth++;

    GAMES ret = GAMES();

    // if we are tasked with playing no more than some specified number of games
    // or no more than a specified winning number of games, obey.
    // These clauses will return all the way to the top, eventually
    if (options::getOptionBool(OMaxGames))
        if (winningGames + losingGames + abortedGames > options::getOptionInt(OMaxGames))
            return ret;

    if (options::getOptionBool(OMaxWinners) && options::getOptionInt(OMaxWinners) <= winningGames)
        return ret;

    // search no deeper than the shallowest winning game
    if (options::getOptionBool(OWinShortest) && winningDepth != -1 && currentDepth >= winningDepth) {
        abortedGames++;
        return ret;
    }

    // search no deepest than the directed depth
    if (options::getOptionBool(OMaxDepth) && options::getOptionInt(OMaxDepth) < currentDepth) {
        abortedGames++;
        return ret;
    }

    // if we've played the entire talon to the waste without changing the tableau, there is
    // no way to win this game (because of the talon-to-waste logic)
    if (moves.back().what == mTalon)
        if (checkExhaustion(moves))
            return ret;

    // make the move that we have been directed to play.
    moves.back().makeMove(decks);

    // see if we have uncovered all of the cards in the tableau.
    // if we have, we might as well quit playing. This will shorten games by dozens of moves
    if (options::getOptionBool(OWinTableau)) for (int i = 0; i < 7; i++)
        if (!decks[ITABLEAU+i].allFaceUp())
            break;
        else if (i == 6) {
            // we won. all the cards in the tableau are uncovered
            markWinner(ret,moves);
            goto UNMAKE;
        }

    { // shut the compiler up about that goto across thisply and playlength

    // compute the list of possible next moves
    MOVES thisply = nextMove(decks);
    int playlength = thisply.size();

    // see if we have won by getting everything onto the foundation
    if (options::getOptionBool(OWinFoundation) && playlength == 0)
        if (decks[IFOUND].length() == 13 &&
            decks[IFOUND+1].length() == 13 &&
            decks[IFOUND+2].length() == 13 &&
            decks[IFOUND+3].length() == 13) {
                // we won. all the cards are in the foundations
                markWinner(ret,moves);
                goto UNMAKE;
        }

    // evaluate all the moves possible from this position
    while (playlength--) {
        // move the candidate move onto the move list ..
        moves.splice(moves.end(),thisply,thisply.begin());

        // .. and play its subtree
        GAMES oneret = dfs(decks,moves);
        currentDepth--;

        // add any winning games from that subtree
        ret.splice(ret.end(),oneret);

        // delete the move from the move list, preparatory for the next
        moves.pop_back();
    }

    } // end compiler silencing

UNMAKE:
    // return the decks to the state that we received them in
    moves.back().unmakeMove(decks);

    return ret;
}

void gameplay::writeMoves(MOVES &moves){
    MOVES::iterator b = moves.begin();
    while (b != moves.end()) {
        b->writeMove();
        b++;
        cout << "\n";
    }
    cout << "\n";
}

void gameplay::markWinner(GAMES &ret,MOVES &moves){
    winningGames++;

    // remember this win.
    winningDepth = currentDepth;

    // debug output
    cout << "winning depth:";
    cout << winningDepth;
    writeMoves(moves);
    fflush(stdout);

    // remember these winning moves
    ret.push_front(moves);
    return;
}

bool gameplay::checkExhaustion(MOVES &moves){
    // if we have just recycled the waste to the talon, look back at
    // the moves and see if we have done anything but deal to the
    // waste deck since the last time this happened.  If so, we are
    // done: we've exhausted all the moves that we can play with this
    // tableau
    MOVES::iterator check = moves.end();
    check--; // pointing to most recent waste-to-talon
    check--; // point to the move immediately before the talon-to-waste
    do {
        MOVE m = *check;
        if (m.what != mWaste)
            if (m.what != mTalon)
                return false; // game not over: we played a card from the waste
            else {
                losingGames++; // game over. nothing played since last recycle
                return true;
            }
        else check--; // don't know yet.
    } while (check != moves.begin());
    return false;
}
