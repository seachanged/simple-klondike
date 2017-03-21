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

It's a depth-first search, optionally using the tree depth of winning
games to prune off subtrees.

BUILDING

To build on a linux box or in mingw, use this command:
g++ -o klondike -std=c++11 -g *.cpp -I.

-g is to provide debug information for gdb
-I is to include the current folder in the search path for .h files
On cygwin, the executable is klondike

To build on a windows box using Visual Studio 2010 Express, or Visual
Studio 2015 Community Edition, open the klondike.vcproj file.

SOME TESTS

These are some decks that were shuffled, transcribed, checked, played
by hand, and then run through the solver.

"3sqc10d5c9h4h6sqdqs4d2c10hjhkd7s2d7d5s4sqh6hah3h9s8h9cas10sad7h5h2h7c8d8c8s4cks6d3ckh6c9dkc2sjdac5d3djsjc10c"; // I lost: unwinnable!
"qs6s7s6hkd3h7d6cqh8had3d9c2c5d8dackh6djh5c9hjcas2dkc5s7c4cjs2sah10d10c2hks9s9d10h8c10s4h5hjd4d3c8s3sqd7h4sqc"; // I lost: likely unwinnable (deep tree)
"3h10h2s4s6s9h3cqc6h10d6cjdah2d7ckcjh5dksjcackd4d5h10s4h9d2c3s7h5sqhqd6d3d8sjs5c7sad7d8h4c9ckh2hasqs9s8c8d10c"; // I lost: winnable
"6c5d9s6sjs4s3cad8d9c9d8h3d7c2skcqc4cqs7d2d6d7skh3s8s5h4d6hacjcas2h4hkd10dqh2c10h5s9h8c5cjd3hqdah7h10sjhks10c"; // I lost: winnable
"9sjh6d4d4c5h10h7sad8has7hjc3c6c6skskdjd9hac5c10dkh6hah9c8d3hqsqd10ckc9djs2d3d4s8s2c5d3sqc2h8c2s10s7c5s7d4hqh"; // I lost: likely unwinnable (deep tree)
"10das8sqc5h6d6c5d10s10c9cac5s7djskdqs2c3c3s8h6h2d7hqdqhjdjh8c4s7c2h6s8d5cjc4hkhkcks9h2s9d3d10h9s4dad4c3h7sah"; // I lost: unwinnable, immediately!
"3h7s7c3sqh10d9c2cjs5s5dksacahkhad4d5c5hjhkdjd4h10s6d3d6hqc8h6sqd6c2s9d4s9h7h4c8sjcas3c2d10hqs10ckc8c7d2h8d9s"; // I lost: winnable
