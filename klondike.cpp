#include "classes.h"
#include "options.h"
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

int main(int argc,char *argv[])
{STACK decks[14];
    // set up the default options
    options::setOptionBool(OWinTableau,true);
    options::setOptionBool(OWinShortest,true);

    const char *d;

    d = "2dqd7c8d7hjh4d5d3c6d2s9hqc5cks4hkd7d6c3s10casjcahqs2c5skhqh5h10dad3d9sacjskc10s8h3h4s2h9d6s7sjd9c10h6h8c4c8s"; // I lost: winnable
    decks[IDECK].create(d);

    // the first move is a deal from the deck to the tableau
    MOVES moves;
    MOVE m;
    m.addMove(mDeal,-1,-1,0,decks);
    moves.push_back(m);

    // we're ready to play
    //
    gameplay game;
    GAMES winners = game.dfs(decks,moves);

    cout << "winning games:" << winners.size() << "\n";

    GAMES::iterator g = winners.begin();
    cout << "winner lengths:";
    for (int i = 0; i < (int) winners.size(); i++,g++) {
        cout << g->size(); cout << " ";
    }
    cout<< "\n";

    cout << "lost games:"; cout << game.losingGames; cout << "\n";
    cout << "aborted games:"; cout << game.abortedGames; cout << "\n";

    return 0;
}

