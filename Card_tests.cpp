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


TEST_MAIN()
