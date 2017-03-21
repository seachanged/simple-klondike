#include "cards.h"

CARD::card(){
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
