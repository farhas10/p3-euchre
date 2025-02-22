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

// Test multiple shuffles produce different results
TEST(test_pack_multiple_shuffles) {
    Pack pack1;
    Pack pack2;
    
    // Shuffle each pack multiple times
    pack1.shuffle();
    pack1.shuffle();
    pack1.shuffle();
    
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    
    bool all_same = true;
    for (int i = 0; i < 24; ++i) {
        Card card1 = pack1.deal_one();
        Card card2 = pack2.deal_one();
        if (card1.get_rank() != card2.get_rank() || 
            card1.get_suit() != card2.get_suit()) {
            all_same = false;
            break;
        }
    }
    
    // Two different sequences of shuffles should (very likely) produce different orders
    ASSERT_FALSE(all_same);
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
