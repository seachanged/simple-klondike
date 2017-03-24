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

options::options(void)
{
    for (int i = OFirst; i < OLast; i++) {
        bSettings[i] = false;
        iSettings[i] = 0;
    }
}

options::~options(void)
{
}

void options::setOptionBool(int opt,bool value){
    if (OFirst < opt && opt < OLast)
        bSettings[opt] = value;
}
void options::setOptionInt(int opt,int value){
    if (OFirst < opt && opt < OLast)
        iSettings[opt] = value;
}
bool options::getOptionBool(int opt){
    if (OFirst < opt && opt < OLast)
        return bSettings[opt];
    return false;
}
int options::getOptionInt(int opt){
    if (OFirst < opt && opt < OLast)
        return iSettings[opt];
    return 0;
}

// a single copy for static members.
//
bool realBSettings[OLast];
int realISettings[OLast];

bool * options::bSettings = realBSettings;
int * options::iSettings = realISettings;

