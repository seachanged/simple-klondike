#include "gameplay.h"
#include "options.h"
#include <list>
#include <iterator>

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
