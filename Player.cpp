#include "Player.hpp"
#include <cassert>
#include <iostream>

// Define concrete classes here in the .cpp file
class Simple : public Player {
private:
    std::string name;
    std::vector<Card> hand;
public:
    Simple(const std::string &name_in);
    virtual const std::string & get_name() const override;

    virtual void add_card(const Card &c) override;

    virtual bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override;

    virtual void add_and_discard(const Card &upcard) override;

    virtual Card lead_card(Suit trump) override;
    
    virtual Card play_card(const Card &led_card, Suit trump) override;
};

class Human : public Player {
private:
    std::string name;
    std::vector<Card> hand;
public:
    Human(const std::string &name_in);
    virtual const std::string & get_name() const override;
    virtual void add_card(const Card &c) override;
    virtual bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override;
    virtual void add_and_discard(const Card &upcard) override;
    virtual Card lead_card(Suit trump) override;
    virtual Card play_card(const Card &led_card, Suit trump) override;
    void print_hand() const;
    Card card_from_input() const;
};

// Factory function implementation
Player * Player_factory(const std::string &name, const std::string &strategy) {
    if (strategy == "Simple") {
        return new Simple(name);
    }
    if (strategy == "Human") {
        return new Human(name);
    }
    
    // If strategy is not recognized, assert false
    assert(false);
    return nullptr;
}

// Output operator implementation
std::ostream & operator<<(std::ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}

// Simple class implementations
Simple::Simple(const std::string &name_in) : name(name_in) {}

const std::string & Simple::get_name() const {
    return name;
}

void Simple::add_card(const Card &c) {
    hand.push_back(c);
}

bool Simple::make_trump(const Card &upcard, bool is_dealer,
                       int round, Suit &order_up_suit) const {
    // If hand is empty, never order up
    if (hand.empty()) {
        return false;
    }
    
    // Count number of face cards or aces in the same suit as upcard
    int trump_face_cards = 0;
    
    if (round == 1) {
        // In round 1, count cards of same suit as upcard
        for (const Card &card : hand) {
            if (card.get_suit() == upcard.get_suit() && 
                card.is_face_or_ace()) {
                trump_face_cards++;
            }
        }
        // Order up if we have 2 or more high cards
        if (trump_face_cards >= 2) {
            order_up_suit = upcard.get_suit();
            return true;
        }
    }
    else { // round 2
        Suit next = Suit_next(upcard.get_suit());
        // In round 2, count cards of next suit
        for (const Card &card : hand) {
            if (card.get_suit() == next && 
                card.is_face_or_ace()) {
                trump_face_cards++;
            }
        }
        // Order up if we have 1 or more high cards, or if we're the dealer
        if (trump_face_cards >= 1 || is_dealer) {
            order_up_suit = next;
            return true;
        }
    }
    return false;
}

void Simple::add_and_discard(const Card &upcard) {
    // Add the upcard first
    hand.push_back(upcard);
    
    // Find the lowest card to discard
    size_t discard_index = 0;
    Suit trump = upcard.get_suit();  // The upcard's suit is trump
    
    // Compare each card with the current lowest
    for (size_t i = 1; i < hand.size(); ++i) {
        // If current card is lower than our lowest so far, update discard_index
        if (Card_less(hand[i], hand[discard_index], trump)) {
            discard_index = i;
        }
    }
    
    // Remove the lowest card
    hand.erase(hand.begin() + discard_index);
}

Card Simple::lead_card(Suit trump) {
    // Find highest non-trump card
    size_t highest_index = 0;
    bool found_non_trump = false;
    
    // First try to find the highest non-trump card
    for (size_t i = 0; i < hand.size(); ++i) {
        if (!hand[i].is_trump(trump)) {
            if (!found_non_trump || hand[i] > hand[highest_index]) {
                highest_index = i;
                found_non_trump = true;
            }
        }
    }
    
    // If we only have trump cards, find the highest trump
    if (!found_non_trump) {
        for (size_t i = 1; i < hand.size(); ++i) {
            if (!Card_less(hand[i], hand[highest_index], trump)) {
                highest_index = i;
            }
        }
    }
    
    // Remove and return the selected card
    Card card_to_play = hand[highest_index];
    hand.erase(hand.begin() + highest_index);
    return card_to_play;
}

Card Simple::play_card(const Card &led_card, Suit trump) {
    // Basic implementation - can be expanded later
    Card card = hand.front();
    hand.erase(hand.begin());
    return card;
}

// Human class implementations
Human::Human(const std::string &name_in) : name(name_in) {}

const std::string & Human::get_name() const {
    return name;
}

void Human::add_card(const Card &c) {
    hand.push_back(c);
}

void Human::print_hand() const {
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << "Card " << i << ": " << hand[i] << std::endl;
    }
}

Card Human::card_from_input() const {
    // Basic implementation - can be expanded later
    return hand.front();
}

bool Human::make_trump(const Card &upcard, bool is_dealer,
                      int round, Suit &order_up_suit) const {
    // Basic implementation - can be expanded later
    return false;
}

void Human::add_and_discard(const Card &upcard) {
    hand.push_back(upcard);
    print_hand();
    // Basic implementation - can be expanded later
}

Card Human::lead_card(Suit trump) {
    print_hand();
    // Basic implementation - can be expanded later
    Card card = hand.front();
    hand.erase(hand.begin());
    return card;
}

Card Human::play_card(const Card &led_card, Suit trump) {
    print_hand();
    // Basic implementation - can be expanded later
    Card card = hand.front();
    hand.erase(hand.begin());
    return card;
} 