#include "stacks.h"
#include <stdlib.h>


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
    for (list<card>::iterator t = top; t != topDown; t++, c++) ;
    return c;
}
// is the deck all face up
bool STACK::allFaceUp(){
    return topDown == pile.end();
}
