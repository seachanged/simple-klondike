#pragma once
// card FACE - an integer in the range of 1 to 52, arranged by suit
// club, spade, diamond, heart
// black first, then red
//
typedef enum {
    CA,C2,C3,C4,C5,C6,C7,C8,C9,C10,CJ,CQ,CK,
    SA,S2,S3,S4,S5,S6,S7,S8,S9,S10,SJ,SQ,SK,
    DA,D2,D3,D4,D5,D6,D7,D8,D9,D10,DJ,DQ,DK,
    HA,H2,H3,H4,H5,H6,H7,H8,H9,H10,HJ,HQ,HK} FACE;

typedef enum {C,S,D,H} SUIT;

typedef class card {
private:
    FACE f;

public:
    card();
    void setFace(FACE face);
    FACE face();
    void setCard(char s,int r);
    SUIT suit();
    bool color();
    int rank();
} CARD;
