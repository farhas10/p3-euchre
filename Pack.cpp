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
    string rank;
    string suit;
    string extra;
    for (int i = 0; i < PACK_SIZE; ++i) {
        pack_input >> rank >> extra >> suit;
        cards[i] = Card(string_to_rank(rank), string_to_suit(suit));
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
        
        // For a perfect in-shuffle:
        // - Second half cards (12-23) go to even positions (0,2,4...)
        // - First half cards (0-11) go to odd positions (1,3,5...)
        int mid = PACK_SIZE / 2;
        for (int i = 0; i < mid; i++) {
            // Cards from second half (index mid+i) go to even positions (2*i)
            cards[2 * i] = temp[mid + i];
            // Cards from first half (index i) go to odd positions (2*i + 1)
            cards[2 * i + 1] = temp[i];
        }
    }
    reset();
}

bool Pack::empty() const {
    return next >= PACK_SIZE;
}
    