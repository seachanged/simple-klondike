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

#pragma once
enum OPTIONS {OFirst, // always before actual first one
    OWinTableau,OWinFoundation,
    OWinShortest,
    OMaxWinners,
    OMaxGames,
    OMaxDepth,
    ODeck,
    OLast}; // always after actual last one
class options
{
private:
    static bool *bSettings;
    static int *iSettings;
public:
    options(void);
    ~options(void);
    static void setOptionBool(int opt,bool value);
    static void setOptionInt(int opt,int value);
    static bool getOptionBool(int opt);
    static int getOptionInt(int opt);
};

