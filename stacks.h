#pragma once
#include "cards.h"
#include <assert.h>
#include <string.h>
#include <list>
#include <iterator>
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
