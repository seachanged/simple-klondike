#include "options.h"

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

