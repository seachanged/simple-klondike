#pragma once
#include "stacks.h"
#include "moves.h"

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

class gameplay
{
public:
    int winningDepth;
    int currentDepth;
    int winningGames;
    int losingGames;
    int abortedGames;

    gameplay(void);
    ~gameplay(void);
    MOVES nextMove(STACK decks[]);
    GAMES dfs(STACK decks[],MOVES &moves);
    void writeMoves(MOVES &moves);
    void markWinner(GAMES &ret,MOVES &moves);
    bool checkExhaustion(MOVES &moves);
};

