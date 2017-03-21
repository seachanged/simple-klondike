// klondike.cpp : Defines the entry point for the console application.
//
#include "stacks.h"
#include "moves.h"
#include "gameplay.h"
#include "options.h"

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

