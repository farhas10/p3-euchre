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
Pack::Pack() {
    int i = 0;
    // Initialize cards from NINE to ACE for each suit
    for (int suit = SPADES; suit <= DIAMONDS; suit++) {
        for (int rank = NINE; rank <= ACE; rank++) {
            cards[i] = Card(static_cast<Rank>(rank), static_cast<Suit>(suit));
            i++;
        }
    }
    reset();
}

Pack::Pack(istream& pack_input) {
    int i = 0;
    while (pack_input >> cards[i]) {
        i++;
    }
    reset();
}

Card Pack::deal_one() {
    assert(!empty());  // REQUIRES: cards remain in the Pack
    return cards[next++];
}

void Pack::reset() {
    next = 0;
}

// EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
void Pack::shuffle() {
    for (int shuffle_count = 0; shuffle_count < 7; shuffle_count++) {
        std::array<Card, PACK_SIZE> temp = cards;
        
        // Perform in-shuffle: split deck in half and interleave
        int mid = PACK_SIZE / 2;
        for (int i = 0; i < mid; i++) {
            // Second half cards go at even indices
            cards[2 * i] = temp[mid + i];
            // First half cards go at odd indices
            cards[2 * i + 1] = temp[i];
        }
    }
    reset();
}

bool Pack::empty() const {
    return next >= PACK_SIZE;
}
    