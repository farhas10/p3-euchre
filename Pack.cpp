#include <cassert>
#include <iostream>
#include <array>
#include "Pack.hpp"

using namespace std;

  // EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on. 
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
  // NOTE: The pack is initially full, with no cards dealt.
Pack(){
    int i = 0;
    for (int suit = 0; suit < 3; suit++){
        for (int rank = 0; rank < 14; rank++){
            cards[i] = Card(static_cast<Rank>(rank), static_cast<Suit>(suit));
            i++;
        }
    }
    reset();
}

void reset(){
  next = 0;
}

/*
void shuffle(){
    array<Card, PACK_SIZE> temp;
    int i = 0;
    for (int j = 0; j < PACK_SIZE/2; j++){
        temp[i] = cards[j];
        i += 2;
    }
    i = 1;
    for (int j = PACK_SIZE/2; j < PACK_SIZE; j++){
        temp[i] = cards[j];
        i += 2;
    }
    for (int i = 0; i < PACK_SIZE; i++){
        cards[i] = temp[i];
    }
    reset();
}
    */

bool empty() const{
    return next == PACK_SIZE;
}
    