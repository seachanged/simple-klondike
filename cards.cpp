#include "cards.h"

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
