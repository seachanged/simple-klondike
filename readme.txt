3/23/2017

Plays Klondike, one card draw from talon with as many recycles of the
waste to the talon as needed.  This is a command line/console program.

It will either play until the games possible from the deck are fully
explored, or until it wins a specified number of times, or until it
has played through a specified number of games (win or lose or aborted
due to length), or until it has played all the games that are shorter
than the specified length.

Winning is defined as either all of the cards in the tableau being
face up (win tableau) or until all of the cards are in the foundations
(win foundation). Losing is when there are no moves possible, or no
moves other than cycling through the talon over and over again.

A list of winning games is returned. If the shortest games are being
sought, a list of all the games that were at some point the shortest
winning games will be returned.

The initial deck is specified as either a test deck (deck 0) or a
randomized deck, or a literal deck specified like "ac 10s 3h ...".

The default behavior has winning defined as uncovering all the cards
on the tableau, returning a list ending in the shortest game, taking
as long as it takes.

OPERATION

Klondike implements a depth-first search, optionally using the tree
depth of winning games to prune off subtrees.  

After the tableau is dealt from the deck, a list of possible moves is
created. Each possible move is explored by making the move,
recursively exploring the subtree, and then un-making the move. If
there was a win in the subtree, that winning tree traversal is
returned and merged with other subtrees' winning games.

Options are specified in the first few lines of klondike.cpp, in
main().

BUILDING

To build on a linux box or in mingw, use this command:
g++ -o klondike -std=c++11 -g *.cpp -I.

To build on a windows box using Visual Studio 2010 Express or later,
open the klondike.vcproj file.

It's been built with:
  - Windows 7 with gcc 4.9.3, minGW runtime version 3021001L, and
    binutils version 2.25.1.
  - Windows 7 with Visual Studio Express 2010,
  - Windows 7 with Visual Studio 2015 Community Edition, and
  - Ubuntu 16.04 with gcc 5.4.0

SOME TESTS

Klondike as shipped explores all the games in particular shuffled deck
single hand, about 14,497,425 games between the length of 100 moves
and 82 moves, and prints out the 8 games as it finds them, and then
prints some simple statistics.

These are some other decks that were shuffled, transcribed, checked,
played by hand, and then run through the solver.

"3sqc10d5c9h4h6sqdqs4d2c10hjhkd7s2d7d5s4sqh6hah3h9s8h9cas10sad7h5h2h7c8d8c8s4cks6d3ckh6c9dkc2sjdac5d3djsjc10c"; // I lost: unwinnable!
"qs6s7s6hkd3h7d6cqh8had3d9c2c5d8dackh6djh5c9hjcas2dkc5s7c4cjs2sah10d10c2hks9s9d10h8c10s4h5hjd4d3c8s3sqd7h4sqc"; // I lost: likely unwinnable (deep tree)
"3h10h2s4s6s9h3cqc6h10d6cjdah2d7ckcjh5dksjcackd4d5h10s4h9d2c3s7h5sqhqd6d3d8sjs5c7sad7d8h4c9ckh2hasqs9s8c8d10c"; // I lost: winnable
"6c5d9s6sjs4s3cad8d9c9d8h3d7c2skcqc4cqs7d2d6d7skh3s8s5h4d6hacjcas2h4hkd10dqh2c10h5s9h8c5cjd3hqdah7h10sjhks10c"; // I lost: winnable
"9sjh6d4d4c5h10h7sad8has7hjc3c6c6skskdjd9hac5c10dkh6hah9c8d3hqsqd10ckc9djs2d3d4s8s2c5d3sqc2h8c2s10s7c5s7d4hqh"; // I lost: likely unwinnable (deep tree)
"10das8sqc5h6d6c5d10s10c9cac5s7djskdqs2c3c3s8h6h2d7hqdqhjdjh8c4s7c2h6s8d5cjc4hkhkcks9h2s9d3d10h9s4dad4c3h7sah"; // I lost: unwinnable, immediately!
"3h7s7c3sqh10d9c2cjs5s5dksacahkhad4d5c5hjhkdjd4h10s6d3d6hqc8h6sqd6c2s9d4s9h7h4c8sjcas3c2d10hqs10ckc8c7d2h8d9s"; // I lost: winnable

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

