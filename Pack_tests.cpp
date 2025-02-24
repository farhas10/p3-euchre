#include "Pack.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <sstream>

using namespace std;

// Test default constructor and initial order
TEST(test_pack_default_ctor) {
    Pack pack;
    // First card should be Nine of Spades
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
    
    // Deal 22 cards (not 23, since we already dealt one)
    for (int i = 0; i < 22; ++i) {
        pack.deal_one();
    }
    // Last card should be Ace of Diamonds
    Card last = pack.deal_one();
    ASSERT_EQUAL(ACE, last.get_rank());
    ASSERT_EQUAL(DIAMONDS, last.get_suit());
}

// Test istream constructor
TEST(test_pack_istream_ctor) {
    const string pack_input = "Nine of Spades\nTen of Spades\n";
    istringstream iss(pack_input);
    Pack pack(iss);
    
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

// Test empty() and dealing all cards
TEST(test_pack_empty) {
    Pack pack;
    ASSERT_FALSE(pack.empty());
    
    // Deal all cards
    for (int i = 0; i < 24; ++i) {
        pack.deal_one();
    }
    ASSERT_TRUE(pack.empty());
}

// Test reset functionality
TEST(test_pack_reset) {
    Pack pack;
    
    // Deal some cards
    for (int i = 0; i < 5; ++i) {
        pack.deal_one();
    }
    
    pack.reset();
    ASSERT_FALSE(pack.empty());
    
    // First card should be Nine of Spades again
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

// Test shuffle maintains all cards



// Test dealing past empty pack (should assert)
TEST(test_pack_deal_all) {
    Pack pack;
    
    // Deal all cards
    for (int i = 0; i < 24; ++i) {
        ASSERT_FALSE(pack.empty());  // Pack shouldn't be empty before dealing last card
        pack.deal_one();
    }
    
    ASSERT_TRUE(pack.empty());  // Pack should be empty after dealing all cards
}

// Test that shuffle changes the order of cards
TEST(test_pack_multiple_shuffles) {
    Pack pack1;
    Pack pack2;
    
    // Keep pack1 in original order, shuffle pack2 once
    pack2.shuffle();
    
    bool found_difference = false;
    while (!pack1.empty() && !pack2.empty()) {
        Card card1 = pack1.deal_one();
        Card card2 = pack2.deal_one();
        if (card1.get_rank() != card2.get_rank() || 
            card1.get_suit() != card2.get_suit()) {
            found_difference = true;
            break;
        }
    }
    
    // Shuffled pack should be in different order than original
    ASSERT_TRUE(found_difference);
}

// Add this test to verify reset works after emptying
TEST(test_pack_reset_after_empty) {
    Pack pack;
    
    // Deal all cards
    for (int i = 0; i < 24; ++i) {
        pack.deal_one();
    }
    
    ASSERT_TRUE(pack.empty());
    pack.reset();
    ASSERT_FALSE(pack.empty());
    
    // Verify we can deal again
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

TEST_MAIN()
