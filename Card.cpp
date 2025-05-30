#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"

using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
//EFFECTS returns the Rank corresponding to str, for example "Two" -> TWO
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  return static_cast<Rank>(-1);  // Return invalid rank
}

//EFFECTS Prints Rank to stream, for example "Two"
ostream & operator<<(ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
istream & operator>>(istream &is, Rank &rank) {
  string str;
  if(is >> str) {
    rank = string_to_rank(str);
  }
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
//EFFECTS returns the Suit corresponding to str, for example "Clubs" -> CLUBS
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  return static_cast<Suit>(-1);  // Return invalid suit
}


//EFFECTS Prints Suit to stream, for example "Spades"
ostream & operator<<(ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
istream & operator>>(istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}


/////////////// Write your implementation for Card below ///////////////
//Card()
Card::Card() : rank(TWO), suit(SPADES) {}

//Card(Rank rank_in, Suit suit_in)
Card::Card(Rank rank_in, Suit suit_in) : rank(rank_in), suit(suit_in) {}

//get_rank() 
Rank Card::get_rank() const{
  return rank;
}

//get_suit() 
Suit Card::get_suit() const{
  return suit;
}

//get_suit(Suit trump) 
Suit Card::get_suit(Suit trump) const{
  if(suit == trump || ((rank == JACK)&&(suit == Suit_next(trump)))){
    return trump;
  }
  else{
    return suit;
  }
}

//is_face_or_ace() 
 bool Card::is_face_or_ace() const {
  if(rank == JACK || rank == QUEEN || rank == KING || rank == ACE){
    return true;
  } else{
    return false;
  }
 }

//is_right_bower(Suit trump) 
bool Card::is_right_bower(Suit trump) const{
  if(suit == trump && rank == JACK){
    return true;
  } else{
    return false;
  }
}

//is_left_bower(Suit trump) 
bool Card::is_left_bower(Suit trump) const{
  if(suit == Suit_next(trump) && rank == JACK){
    return true;
  } else{
    return false;
  }
}

//is_trump(Suit trump) 
bool Card::is_trump(Suit trump) const{
  if(suit == trump || is_left_bower(trump)){
    return true;
  } else{
    return false;
  }
}


ostream & operator<<(ostream &os, const Card &card){
  os << card.get_rank() << " of " << card.get_suit();
  return os;
}


istream & operator>>(istream &is, Card &card){
  string rank_str, of_str, suit_str;
  is >> rank_str >> of_str >> suit_str;

  if (of_str != "of") {
    is.setstate(ios::failbit); 
    return is;
  }
  
  Rank rank = string_to_rank(rank_str);
  Suit suit = string_to_suit(suit_str);
  
  // Check if either rank or suit is invalid
  if (rank < TWO || rank > ACE || suit < SPADES || suit > DIAMONDS) {
    is.setstate(ios::failbit);
    return is;
  }
  
  card.rank = rank;
  card.suit = suit;
  return is;
}

//bool operator<(const Card &lhs, const Card &rhs)
bool operator<(const Card &lhs, const Card &rhs){
  if (lhs.get_rank() < rhs.get_rank()){
    return true;
  }
  if (lhs.get_rank() == rhs.get_rank()){
    return lhs.get_suit() < rhs.get_suit();
  }
  return false;
}

//bool operator<=(const Card &lhs, const Card &rhs)
bool operator<=(const Card &lhs, const Card &rhs){
  return !(lhs > rhs);
}

//bool operator>(const Card &lhs, const Card &rhs)
bool operator>(const Card &lhs, const Card &rhs){
  if (lhs.get_rank() > rhs.get_rank()){
    return true;
  }
  if (lhs.get_rank() == rhs.get_rank()){
    return lhs.get_suit() > rhs.get_suit();
  }
  return false;
}

//bool operator>=(const Card &lhs, const Card &rhs)
bool operator>=(const Card &lhs, const Card &rhs){
  return !(lhs < rhs);
}

//bool operator==(const Card &lhs, const Card &rhs)
bool operator==(const Card &lhs, const Card &rhs){
  return (lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit());
}

//bool operator!=(const Card &lhs, const Card &rhs)
bool operator!=(const Card &lhs, const Card &rhs){
  return (lhs.get_rank() != rhs.get_rank());
}

// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec before implementing
// the following operator overload functions:
//   operator<<
//   operator>>
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=

// Helper function for both Card_less versions
bool Card_less_helper(const Card &a, const Card &b, Suit trump) {
    // Handle bower cases first
    if (a.is_right_bower(trump)) return false;
    if (b.is_right_bower(trump)) return true;
    if (a.is_left_bower(trump)) return false;
    if (b.is_left_bower(trump)) return true;

    // Then handle trump vs non-trump
    if (a.get_suit(trump) == trump && b.get_suit(trump) != trump) return false;
    if (a.get_suit(trump) != trump && b.get_suit(trump) == trump) return true;

    // If both are trump or both are not trump, compare ranks
    if (a.get_suit(trump) == b.get_suit(trump)) {
        return a.get_rank() < b.get_rank();
    }
    
    return false; // Default case
}

bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump) {
    // Use helper function for common logic
    bool result = Card_less_helper(a, b, trump);
    
    // Only add led card specific logic if not already determined
    if (a.get_suit(trump) != b.get_suit(trump) && 
        !a.is_trump(trump) && !b.is_trump(trump)) {
        
        // If one of the cards follows the led suit but neither is trump
        if (a.get_suit(trump) == led_card.get_suit(trump)) return false;
        if (b.get_suit(trump) == led_card.get_suit(trump)) return true;
    }
    
    return result;
}

bool Card_less(const Card &a, const Card &b, Suit trump) {
    return Card_less_helper(a, b, trump);
}

Suit Suit_next(Suit suit){
  if(suit == SPADES){
    return CLUBS;
  } else if(suit == CLUBS){
    return SPADES;
  } else if(suit == HEARTS){
    return DIAMONDS;
  } else{
    return HEARTS;
  }
}