#include "moves.h"
#include "options.h"

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


