#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;
//spades & clubs are black
//diamonds & hearts are red

//constructors
TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}
TEST(test_default){
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}
//get_suit
TEST(get_suit1){
    Card c(JACK, HEARTS); //left bower
    ASSERT_EQUAL(DIAMONDS, c.get_suit(DIAMONDS));
}
TEST(get_suit2){
    Card c(JACK, HEARTS);
    ASSERT_EQUAL(HEARTS, c.get_suit(HEARTS));
}
TEST(get_suit3){
    Card c(JACK, HEARTS);
    ASSERT_EQUAL(HEARTS, c.get_suit(SPADES));
}
TEST(get_suit4){
    Card c(JACK, HEARTS); 
    ASSERT_EQUAL(HEARTS, c.get_suit(CLUBS));
}
//wrong inputs
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
//trump
TEST(trump){
    Card c(JACK, SPADES);
    ASSERT_TRUE(c.is_trump(CLUBS)); //if trump is clubs
    ASSERT_TRUE(c.is_trump(SPADES)); //if trump is spades
    ASSERT_FALSE(c.is_trump(DIAMONDS)); //if trump is diamonds
    ASSERT_FALSE(c.is_trump(HEARTS)); //if trump is hearts
}
//bowers
TEST(right_bower){
    Card c(JACK, DIAMONDS);
    ASSERT_TRUE(c.is_right_bower(DIAMONDS));
    ASSERT_FALSE(c.is_right_bower(HEARTS));
    ASSERT_FALSE(c.is_right_bower(SPADES));
    ASSERT_FALSE(c.is_right_bower(CLUBS));
}
TEST(left_bower){
    Card c(JACK, DIAMONDS);
    ASSERT_TRUE(c.is_left_bower(HEARTS));
    ASSERT_FALSE(c.is_left_bower(DIAMONDS));
    ASSERT_FALSE(c.is_left_bower(CLUBS));
    ASSERT_FALSE(c.is_left_bower(SPADES));
}
//operators
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
//ace or face
TEST(test_is_face_or_ace){
    Card c1(ACE, SPADES); //ace card
    Card c2(KING, SPADES); //face card
    Card c3(TEN, SPADES); //neither

    ASSERT_TRUE(c1.is_face_or_ace());
    ASSERT_TRUE(c2.is_face_or_ace());
    ASSERT_FALSE(c3.is_face_or_ace());
}
//input
TEST(test_card_input_output){
    istringstream input("Two of Clubs");
    Card c;
    input >> c;

    ostringstream output;
    output << c;
    
    ASSERT_EQUAL("Two of Clubs", output.str());
}
//card_less
TEST(card_less_normal) {
    Card c1(JACK, HEARTS);  
    Card c2(NINE, HEARTS);  

    ASSERT_TRUE(Card_less(c2, c1, HEARTS));  
}
TEST(card_less_right_left) {
    Card right(JACK, SPADES);  
    Card left(JACK, CLUBS);  

    ASSERT_TRUE(Card_less(left, right, SPADES));  
}
TEST(card_less_left){
    Card left(JACK, CLUBS);
    Card c(TWO, SPADES);
    ASSERT_TRUE(Card_less(c, left, SPADES));
}
TEST(card_less_trump){
    Card trump(FIVE, CLUBS);
    Card c(ACE, HEARTS);
    ASSERT_TRUE(Card_less(c, trump, CLUBS));
}
TEST(card_less_led_card){
    Card led(NINE, HEARTS);
    Card c1(TEN, HEARTS);
    Card c2(TWO, CLUBS);
    ASSERT_TRUE(Card_less(c1, c2, led, CLUBS));
}
TEST(card_less_no_trump){
    Card led(EIGHT, DIAMONDS);
    Card c1(KING, SPADES);   
    Card c2(SEVEN, DIAMONDS); 

    ASSERT_TRUE(Card_less(c1, c2, led, CLUBS));  
}
//suit_next
TEST(test_suit_next){
    ASSERT_EQUAL(SPADES, Suit_next(CLUBS));
    ASSERT_EQUAL(CLUBS, Suit_next(SPADES));
    ASSERT_EQUAL(HEARTS, Suit_next(DIAMONDS));
    ASSERT_EQUAL(DIAMONDS, Suit_next(HEARTS));
}

TEST_MAIN()