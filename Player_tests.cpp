#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

// Basic factory and name tests
TEST(test_player_creation) {
    Player* p1 = Player_factory("p", "Simple");
    Player* p2 = Player_factory("p", "Human");
    
    ASSERT_EQUAL("p", p1->get_name());
    ASSERT_EQUAL("p", p2->get_name());
    
    delete p1;
    delete p2;
}

// Test string output operator
TEST(test_player_output) {
    Player* p = Player_factory("p", "Simple");
    ostringstream oss;
    oss << *p;
    ASSERT_EQUAL("p", oss.str());
    delete p;
}

// Test lead card with only non-trump
TEST(test_lead_all_non_trump) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(KING, DIAMONDS));
    p->add_card(Card(QUEEN, CLUBS));
    p->add_card(Card(ACE, DIAMONDS));
    
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(ACE, DIAMONDS));
    delete p;
}

// Test lead card with only trump
TEST(test_lead_all_trump) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(JACK, HEARTS));
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(KING, HEARTS));
    
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(JACK, HEARTS));
    delete p;
}

// Test lead card with right bower
TEST(test_lead_right_bower) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(JACK, HEARTS));
    p->add_card(Card(NINE, DIAMONDS));
    
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(NINE, DIAMONDS));
    delete p;
}

// Test lead card with left bower
TEST(test_lead_card_left_bower_spades) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(JACK, CLUBS));
    p->add_card(Card(QUEEN, SPADES));
    p->add_card(Card(ACE, SPADES));
    
    p->add_and_discard(Card(NINE, SPADES));
    
    ASSERT_EQUAL(p->lead_card(SPADES), Card(JACK, CLUBS));
    delete p;
}

// Test lead card with all trump except one
TEST(test_lead_card_all_but_one_trump) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, DIAMONDS));
    p->add_card(Card(JACK, CLUBS));
    p->add_card(Card(KING, SPADES));
    p->add_card(Card(QUEEN, SPADES));
    p->add_card(Card(ACE, SPADES));
    
    p->add_and_discard(Card(NINE, DIAMONDS));
    
    ASSERT_EQUAL(p->lead_card(SPADES), Card(NINE, DIAMONDS));
    delete p;
}

// Test make trump round 1 with one face card
TEST(test_make_trump_round1_one_face) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(KING, SPADES));
    p->add_card(Card(TEN, SPADES));
    
    Suit trump = HEARTS;
    ASSERT_FALSE(p->make_trump(Card(ACE, SPADES), false, 1, trump));
    delete p;
}

// Test make trump round 1 with right bower only
TEST(test_make_trump_round1_right_bower) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(JACK, SPADES));
    p->add_card(Card(NINE, SPADES));
    
    Suit trump = HEARTS;
    ASSERT_FALSE(p->make_trump(Card(ACE, SPADES), false, 1, trump));
    delete p;
}

// Test make trump round 2 as dealer
TEST(test_make_trump_round2_dealer) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, DIAMONDS));
    p->add_card(Card(TEN, CLUBS));
    
    Suit trump = CLUBS;
    ASSERT_TRUE(p->make_trump(Card(ACE, HEARTS), true, 2, trump));
    delete p;
}

// Test make trump round 2 with face in next suit
TEST(test_make_trump_round2_next_face) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(KING, DIAMONDS));
    p->add_card(Card(NINE, CLUBS));
    
    Suit trump = CLUBS;
    ASSERT_TRUE(p->make_trump(Card(ACE, HEARTS), false, 2, trump));
    delete p;
}

// Test make trump round 2 with left bower
TEST(test_make_trump_round2_left_bower) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(JACK, DIAMONDS));
    p->add_card(Card(NINE, CLUBS));
    
    Suit trump = CLUBS;
    ASSERT_TRUE(p->make_trump(Card(ACE, HEARTS), false, 2, trump));
    delete p;
}

// Test play card following suit
TEST(test_play_card_follow_suit) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(KING, HEARTS));
    
    Card played = p->play_card(Card(ACE, HEARTS), SPADES);
    ASSERT_EQUAL(played, Card(KING, HEARTS));
    delete p;
}

// Test play card with no matching suit
TEST(test_play_card_no_match) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, DIAMONDS));
    p->add_card(Card(TEN, DIAMONDS));
    
    Card played = p->play_card(Card(ACE, HEARTS), SPADES);
    ASSERT_EQUAL(played, Card(NINE, DIAMONDS));
    delete p;
}

// Test play card with both bowers
TEST(test_play_card_both_bowers) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(JACK, HEARTS));
    p->add_card(Card(JACK, DIAMONDS));
    
    Card played = p->play_card(Card(ACE, HEARTS), HEARTS);
    ASSERT_EQUAL(played, Card(JACK, HEARTS));
    delete p;
}

// Test add and discard with higher card
TEST(test_add_discard_higher) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(JACK, SPADES));
    p->add_card(Card(QUEEN, SPADES));
    p->add_card(Card(KING, SPADES));
    
    p->add_and_discard(Card(ACE, SPADES));
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(ACE, SPADES));
    delete p;
}

// Test add and discard with lower card
TEST(test_add_discard_lower) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(JACK, SPADES));
    p->add_card(Card(QUEEN, SPADES));
    p->add_card(Card(KING, SPADES));
    p->add_card(Card(ACE, SPADES));
    
    p->add_and_discard(Card(NINE, SPADES));
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(ACE, SPADES));
    delete p;
}

// Test single card scenarios
TEST(test_single_card_plays) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(ACE, SPADES));
    
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(ACE, SPADES));
    delete p;
}

// Test empty hand make trump
TEST(test_empty_hand_make_trump) {
    Player* p = Player_factory("Test", "Simple");
    Suit trump = HEARTS;
    ASSERT_FALSE(p->make_trump(Card(ACE, SPADES), false, 1, trump));
    delete p;
}


// Test play card with only trump cards
TEST(test_play_card_only_trump) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(KING, HEARTS));
    
    Card played = p->play_card(Card(NINE, DIAMONDS), HEARTS);
    ASSERT_EQUAL(played, Card(QUEEN, HEARTS));
    delete p;
}

// Test make trump with mixed suits
TEST(test_make_trump_mixed_suits) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(QUEEN, DIAMONDS));
    p->add_card(Card(JACK, CLUBS));
    
    Suit trump = SPADES;
    ASSERT_FALSE(p->make_trump(Card(ACE, HEARTS), false, 1, trump));
    delete p;
}

// Test add and discard with equal cards
TEST(test_add_discard_equal_cards) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(NINE, DIAMONDS));
    
    p->add_and_discard(Card(NINE, SPADES));
    Card played = p->play_card(Card(TEN, CLUBS), CLUBS);
    ASSERT_EQUAL(played.get_rank(), NINE);
    delete p;
}

// Test make trump round 2 with exactly one trump in next suit
TEST(test_make_trump_round2_exactly_one_trump) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(KING, CLUBS));     // One face card in next suit
    p->add_card(Card(NINE, DIAMONDS));
    p->add_card(Card(TEN, HEARTS));
    
    Suit trump = CLUBS;
    ASSERT_TRUE(p->make_trump(Card(ACE, SPADES), false, 2, trump));
    delete p;
}

// Test make trump round 2 with no trump in next suit
TEST(test_make_trump_round2_no_trump) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(KING, DIAMONDS));
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(TEN, SPADES));
    
    Suit trump = CLUBS;  // Next suit after SPADES
    ASSERT_FALSE(p->make_trump(Card(ACE, SPADES), false, 2, trump));
    delete p;
}

// Test lead card with both bowers in hand
TEST(test_lead_card_both_bowers) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(JACK, CLUBS));
    p->add_card(Card(QUEEN, SPADES));
    p->add_card(Card(ACE, SPADES));
    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(JACK, SPADES));
   
    p->add_and_discard(Card(NINE, SPADES));
    
    ASSERT_EQUAL(p->lead_card(SPADES), Card(JACK, SPADES));
    delete p;
}

// Test lead card with all trump when no suit
TEST(test_simple_player_lead_trump_when_no_suit) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(JACK, SPADES));
    p->add_card(Card(KING, SPADES));
    p->add_card(Card(QUEEN, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(ACE, SPADES));
        
    ASSERT_EQUAL(p->lead_card(SPADES), Card(JACK, SPADES));
    
    delete p;
}

// Test make trump with varying hand sizes
TEST(test_simple_player_make_trump_first_round_varying_hands) {
    // Two cards
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(ACE, SPADES));
    p->add_card(Card(KING, SPADES));
    
    Card upcard(NINE, SPADES);
    Suit trump;
    bool orderup = p->make_trump(upcard, true, 1, trump);
    
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(trump, SPADES);
    delete p;

    // One card
    p = Player_factory("Test", "Simple");
    p->add_card(Card(ACE, SPADES));
    
    orderup = p->make_trump(upcard, true, 1, trump);
    
    ASSERT_FALSE(orderup);
    delete p;
}

// Test make trump second round with exactly one trump
TEST(test_simple_player_make_trump_second_round_two_cards) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, CLUBS));
    p->add_card(Card(JACK, CLUBS));
    
    Card upcard(NINE, SPADES);
    Suit trump;
    bool orderup = p->make_trump(upcard, false, 2, trump);
    
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(trump, CLUBS);
    delete p;
}

// Test make trump round 1 with two face cards in upcard suit
TEST(test_make_trump_round1_faces) {
    Player* p = Player_factory("p", "Simple");
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(JACK, DIAMONDS));
    p->add_card(Card(NINE, HEARTS));
    
    Suit trump = DIAMONDS;
    ASSERT_TRUE(p->make_trump(Card(KING, HEARTS), false, 1, trump));
    ASSERT_EQUAL(HEARTS, trump);
    delete p;
}

// Test dealer passing in round 1 with weak hand
TEST(test_make_trump_round1_correct) {
    Player* p = Player_factory("p", "Simple");
    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(JACK, DIAMONDS));
    
    Suit trump = HEARTS;
    ASSERT_FALSE(p->make_trump(Card(KING, HEARTS), true, 1, trump));
    ASSERT_EQUAL(HEARTS, trump);
    delete p;
}

// Test dealer making trump in round 2 with left bower
TEST(test_make_trump_round2_left) {
    Player* p = Player_factory("p", "Simple");
    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(JACK, DIAMONDS));
    
    Suit trump = HEARTS;
    ASSERT_TRUE(p->make_trump(Card(KING, HEARTS), true, 2, trump));
    ASSERT_EQUAL(DIAMONDS, trump);
    delete p;
}

// Test simple player play card follows suit play all
TEST(test_simple_player_play_card_follows_suit_play_all) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, CLUBS));
    p->add_card(Card(TEN, CLUBS));
    p->add_card(Card(QUEEN, DIAMONDS));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(ACE, CLUBS));
    
    Card led_card = p->lead_card(CLUBS);
    p->play_card(led_card, CLUBS);
    p->play_card(led_card, CLUBS);
    p->play_card(led_card, CLUBS);
    
    ASSERT_EQUAL(p->play_card(led_card, HEARTS), Card(ACE, CLUBS));
    delete p;
}

// Test play with different suits
TEST(test_simple_player_play_card_follows_suit_play_all_different) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, DIAMONDS));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(QUEEN, DIAMONDS));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(ACE, HEARTS));
    
    Card led_card = p->lead_card(HEARTS);
    p->play_card(led_card, CLUBS);
    p->play_card(led_card, CLUBS);
    p->play_card(led_card, CLUBS);
    
    ASSERT_EQUAL(p->play_card(led_card, HEARTS), Card(ACE, HEARTS));
    delete p;
}

// Test card selection with specific hand
TEST(test_simple_player_play_card_first_again) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(JACK, HEARTS));

    p->add_and_discard(Card(NINE, HEARTS));
     
    ASSERT_NOT_EQUAL(p->play_card(Card(NINE, CLUBS), CLUBS), Card(NINE, HEARTS));
    delete p;
}

// Test playing out all cards in sequence
TEST(test_simple_player_play_card_follows_suit_play_all_different_play_out) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, DIAMONDS));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(QUEEN, DIAMONDS));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(ACE, HEARTS));

    p->add_and_discard(Card(JACK, HEARTS));
    
    Card led_card = p->lead_card(HEARTS);
 
    ASSERT_EQUAL(p->play_card(led_card, HEARTS), Card(TEN, SPADES));
    ASSERT_EQUAL(p->play_card(led_card, HEARTS), Card(QUEEN, DIAMONDS));
    ASSERT_EQUAL(p->play_card(led_card, HEARTS), Card(JACK, HEARTS));
    ASSERT_EQUAL(p->play_card(led_card, HEARTS), Card(ACE, HEARTS));

    delete p;
}

// Test playing cards in order with trump
TEST(test_simple_player_play_card_first) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(ACE, HEARTS));

    p->add_and_discard(Card(JACK, HEARTS));
     
    ASSERT_EQUAL(p->play_card(Card(NINE, CLUBS), HEARTS), Card(TEN, HEARTS));
    ASSERT_EQUAL(p->play_card(Card(NINE, CLUBS), HEARTS), Card(JACK, HEARTS));
    ASSERT_EQUAL(p->play_card(Card(NINE, CLUBS), HEARTS), Card(QUEEN, HEARTS));
    ASSERT_EQUAL(p->play_card(Card(NINE, CLUBS), HEARTS), Card(KING, HEARTS));
    ASSERT_EQUAL(p->play_card(Card(NINE, CLUBS), HEARTS), Card(ACE, HEARTS));

    delete p;
}

// Test playing cards in order with trump, adding ace last
TEST(test_simple_player_play_card_last) {
    Player* p = Player_factory("Test", "Simple");
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(JACK, HEARTS));

    p->add_and_discard(Card(ACE, HEARTS));
     
    ASSERT_EQUAL(p->play_card(Card(NINE, CLUBS), HEARTS), Card(TEN, HEARTS));
    ASSERT_EQUAL(p->play_card(Card(NINE, CLUBS), HEARTS), Card(JACK, HEARTS));
    ASSERT_EQUAL(p->play_card(Card(NINE, CLUBS), HEARTS), Card(QUEEN, HEARTS));
    ASSERT_EQUAL(p->play_card(Card(NINE, CLUBS), HEARTS), Card(KING, HEARTS));
    ASSERT_EQUAL(p->play_card(Card(NINE, CLUBS), HEARTS), Card(ACE, HEARTS));

    delete p;
}

TEST_MAIN()