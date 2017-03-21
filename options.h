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

