#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}
// Add more test cases here
TEST(test_default){
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}
TEST(invalid_input){
    istringstream input("Five Clubs");
    Card c;
    input >> c;
    ASSERT_TRUE(input.fail());
}
TEST(invalid_rank_input){
    istringstream input("Twelve of Hearts");
    Card c;
    input >> c;
    ASSERT_FALSE(input.fail());
}
TEST(invalid_suit_input) {
    istringstream input("Four of Stars"); 
    Card c;
    input >> c;
    ASSERT_FALSE(input.fail()); 
}
TEST(trump){
    Card c(JACK, SPADES);
    ASSERT_FALSE(c.is_trump(CLUBS)); //if trump is clubs
    ASSERT_TRUE(c.is_trump(SPADES)); //if trump is spades
}
//spades & clubs are black
//diamonds & hearts are red
TEST(right_bower){
    Card c(JACK, DIAMONDS);
    ASSERT_TRUE(c.is_right_bower(DIAMONDS));
    ASSERT_FALSE(c.is_right_bower(CLUBS));
}
TEST(left_bower){
    Card c(JACK, DIAMONDS);
    ASSERT_TRUE(c.is_left_bower(HEARTS));
    ASSERT_FALSE(c.is_left_bower(DIAMONDS));
}
TEST(greater_than_operator){
    Card c1(ACE, HEARTS);
    Card c2(KING, HEARTS);
    ASSERT_TRUE(c1 > c2);
    ASSERT_TRUE(c1 >= c2);
}
TEST(less_than_operator){
    Card c1(ACE, HEARTS);
    Card c2(KING, HEARTS);
    ASSERT_TRUE(c2 < c1);
    ASSERT_TRUE(c2 <= c1);
}
TEST(equal_operator){
    Card c1(ACE, HEARTS);
    Card c2(KING, HEARTS);
    ASSERT_FALSE(c1 == c2);
    ASSERT_TRUE(c1 != c2);
}
TEST(test_is_face_or_ace){
    Card c1(ACE, SPADES); //ace card
    Card c2(KING, SPADES); //fae card
    Card c3(TEN, SPADES); //neither

    ASSERT_TRUE(c1.is_face_or_ace());
    ASSERT_TRUE(c2.is_face_or_ace());
    ASSERT_FALSE(c3.is_face_or_ace());
}
TEST(test_card_input_output){
    istringstream input("Two of Clubs");
    Card c;
    input >> c;

    ostringstream output;
    output << c;
    
    ASSERT_EQUAL("Two of Clubs", output.str());
}
TEST(card_less_normal) {
    Card c1(JACK, HEARTS);  
    Card c2(NINE, HEARTS);  

    ASSERT_TRUE(Card_less(c2, c1, HEARTS));  
}
TEST(card_less_right_left) {
    Card right(JACK, SPADES);  
    Card left(JACK, CLUBS);  

    ASSERT_TRUE(Card_less(left, right, SPADES));  
    ASSERT_FALSE(Card_less(right, left, SPADES));
}
TEST(card_less_case){
    Card left(JACK, CLUBS);
    Card c(TWO, SPADES);
    ASSERT_TRUE(Card_less(c, left, SPADES));
}
TEST(test_suit_next){
    ASSERT_EQUAL(SPADES, Suit_next(CLUBS));
}

TEST_MAIN()



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
  //assert(false); // Input string didn't match any rank
  return {};
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
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
  //assert(false); // Input string didn't match any suit
  return {};
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
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

//et_suit(Suit trump) 
Suit Card::get_suit(Suit trump) const{
  if(suit == trump || ((rank == JACK)&&(Suit_next(trump))) || suit == Suit_next(Suit_next(trump))){
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
  if(suit == trump){
    return true;
  } else{
    return false;
  }
}

//std::ostream & operator<<(std::ostream &os, const Card &card)
std::ostream & operator<<(std::ostream &os, const Card &card){
  os << card.get_rank() << " of " << card.get_suit();
  return os;
}

//std::istream & operator>>(std::istream &is, Card &card)
std::istream & operator>>(std::istream &is, Card &card){
  std::string rank_str, of_str, suit_str;
  is >> rank_str >> of_str >> suit_str;

  if (of_str != "of") {
    is.setstate(std::ios::failbit); 
    return is;
  }
  card.rank = string_to_rank(rank_str); 
  card.suit = string_to_suit(suit_str); 

  return is;
}

//bool operator<(const Card &lhs, const Card &rhs)
bool operator<(const Card &lhs, const Card &rhs){
  return (lhs.get_rank() < rhs.get_rank());
}

//bool operator<=(const Card &lhs, const Card &rhs)
bool operator<=(const Card &lhs, const Card &rhs){
  return (lhs.get_rank() <= rhs.get_rank());
}

//bool operator>(const Card &lhs, const Card &rhs)
bool operator>(const Card &lhs, const Card &rhs){
  return (lhs.get_rank() > rhs.get_rank());
}

//bool operator>=(const Card &lhs, const Card &rhs)
bool operator>=(const Card &lhs, const Card &rhs){
  return (lhs.get_rank() >= rhs.get_rank());
}

//bool operator==(const Card &lhs, const Card &rhs)
bool operator==(const Card &lhs, const Card &rhs){
  return (lhs.get_rank() == rhs.get_rank());
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

bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump){
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

    // If one of the cards follows the led suit but neither is trump
    if (a.get_suit(trump) == led_card.get_suit(trump) && b.get_suit(trump) != led_card.get_suit(trump)) {
        return false;
    }
    if (b.get_suit(trump) == led_card.get_suit(trump) && a.get_suit(trump) != led_card.get_suit(trump)) {
        return true;
    }

    // In any other case, a is not less than b
    return false;
}

bool Card_less(const Card &a, const Card &b, Suit trump){
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

    // If different non-trump suits, a is not less than b
    return false;
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