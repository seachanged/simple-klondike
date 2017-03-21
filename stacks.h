#pragma once
#include "cards.h"
#include <assert.h>
#include <string.h>
#include <list>
#include <iterator>
using namespace std;

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
