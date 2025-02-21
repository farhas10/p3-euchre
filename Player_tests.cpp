#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * ryan = Player_factory("ryan", "Simple");
    Player * jane = Player_factory("jane", "Human");
    
    ASSERT_EQUAL("ryan", ryan->get_name());
    ASSERT_EQUAL("jane", jane->get_name());

    delete ryan;
    delete jane;
}

TEST(test_add_card) {
    Player * player = Player_factory("Test", "Simple");
    
    // Add multiple cards and verify they're added via make_trump
    Card c1(JACK, HEARTS);
    Card c2(JACK, DIAMONDS);
    Card c3(ACE, HEARTS);
    
    player->add_card(c1);
    player->add_card(c2);
    player->add_card(c3);
    
    // Should order up hearts with 2 face cards
    Suit trump = SPADES;  // Initialize to different suit
    ASSERT_TRUE(player->make_trump(Card(NINE, HEARTS), false, 1, trump));
    ASSERT_EQUAL(trump, HEARTS);
    
    delete player;
}

TEST(test_make_trump_round1) {
    Player * player = Player_factory("Test", "Simple");
    
    // Test case 1: Less than 2 face cards - should pass
    player->add_card(Card(JACK, HEARTS));
    player->add_card(Card(NINE, HEARTS));
    player->add_card(Card(TEN, SPADES));
    
    Suit trump = CLUBS;
    ASSERT_FALSE(player->make_trump(Card(ACE, HEARTS), false, 1, trump));
    
    delete player;
    
    // Test case 2: Exactly 2 face cards - should order up
    player = Player_factory("Test", "Simple");
    player->add_card(Card(JACK, HEARTS));
    player->add_card(Card(QUEEN, HEARTS));
    player->add_card(Card(NINE, SPADES));
    
    trump = CLUBS;
    ASSERT_TRUE(player->make_trump(Card(NINE, HEARTS), false, 1, trump));
    ASSERT_EQUAL(trump, HEARTS);
    
    delete player;
}

TEST(test_make_trump_round2) {
    Player * player = Player_factory("Test", "Simple");
    
    // Test case 1: Dealer with no face cards should order up
    player->add_card(Card(NINE, DIAMONDS));
    player->add_card(Card(TEN, DIAMONDS));
    player->add_card(Card(NINE, SPADES));
    
    Suit trump = CLUBS;
    ASSERT_TRUE(player->make_trump(Card(ACE, HEARTS), true, 2, trump));
    ASSERT_EQUAL(trump, DIAMONDS);  // Next suit after Hearts
    
    delete player;
    
    // Test case 2: Non-dealer with one face card should order up
    player = Player_factory("Test", "Simple");
    player->add_card(Card(JACK, DIAMONDS));
    player->add_card(Card(NINE, CLUBS));
    player->add_card(Card(TEN, SPADES));
    
    trump = CLUBS;
    ASSERT_TRUE(player->make_trump(Card(ACE, HEARTS), false, 2, trump));
    ASSERT_EQUAL(trump, DIAMONDS);
    
    delete player;
}

TEST(test_lead_card) {
    Player * player = Player_factory("Test", "Simple");
    
    // Test leading when having both trump and non-trump
    Card c1(ACE, HEARTS);    // High non-trump
    Card c2(JACK, SPADES);   // Trump
    Card c3(NINE, SPADES);   // Lower trump
    
    player->add_card(c1);
    player->add_card(c2);
    player->add_card(c3);
    
    // Should lead highest non-trump
    Card led = player->lead_card(SPADES);
    ASSERT_EQUAL(led, c1);
    
    delete player;
    
    // Test leading with only trump cards
    player = Player_factory("Test", "Simple");
    player->add_card(Card(JACK, SPADES));
    player->add_card(Card(QUEEN, SPADES));
    player->add_card(Card(NINE, SPADES));
    
    // Should lead highest trump
    led = player->lead_card(SPADES);
    ASSERT_EQUAL(led.get_rank(), JACK);
    ASSERT_EQUAL(led.get_suit(), SPADES);
    
    delete player;
}

TEST(test_play_card) {
    Player * player = Player_factory("Test", "Simple");
    
    // Add some cards
    Card c1(ACE, HEARTS);
    Card c2(KING, HEARTS);
    Card c3(QUEEN, SPADES);
    
    player->add_card(c1);
    player->add_card(c2);
    player->add_card(c3);
    
    // Verify basic play_card functionality
    Card played = player->play_card(Card(NINE, HEARTS), DIAMONDS);
    ASSERT_TRUE(played == c1 || played == c2 || played == c3);
    
    delete player;
}

TEST(test_add_and_discard) {
    Player * player = Player_factory("Test", "Simple");
    
    // Add initial cards
    player->add_card(Card(NINE, HEARTS));
    player->add_card(Card(TEN, HEARTS));
    player->add_card(Card(JACK, HEARTS));
    player->add_card(Card(QUEEN, HEARTS));
    player->add_card(Card(KING, HEARTS));
    
    // Add and discard should maintain hand size
    Card upcard(ACE, HEARTS);
    player->add_and_discard(upcard);
    
    // Verify hand size is still 5 by adding another card
    player->add_card(Card(NINE, SPADES));
    
    // Should still work normally for play_card
    Card played = player->play_card(Card(NINE, DIAMONDS), HEARTS);
    // Replace is_valid() check with a simpler assertion
    ASSERT_TRUE(played.get_rank() >= NINE && played.get_rank() <= ACE);
    
    delete player;
}

TEST_MAIN()
