#include "Player.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>

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
    // Sort hand after adding new card
    std::sort(hand.begin(), hand.end());
}

bool Simple::make_trump(const Card &upcard, bool is_dealer,
                       int round, Suit &order_up_suit) const {
    if (hand.empty()) {
        return false;
    }
    
    int trump_face_cards = 0;
    
    if (round == 1) {
        // In round 1, count face cards in upcard's suit
        for (const Card &card : hand) {
            // Check if it's effectively a trump card using get_suit
            if (card.get_suit(upcard.get_suit()) == upcard.get_suit() && 
                card.is_face_or_ace()) {
                trump_face_cards++;
            }
        }
        if (trump_face_cards >= 2) {
            order_up_suit = upcard.get_suit();
            return true;
        }
    } else {
        Suit next = Suit_next(upcard.get_suit());
        for (const Card &card : hand) {
            if (card.get_suit(next) == next && 
                card.is_face_or_ace()) {
                trump_face_cards++;
            }
        }
        if (trump_face_cards >= 1 || is_dealer) {
            order_up_suit = next;
            return true;
        }
    }
    return false;
}

void Simple::add_and_discard(const Card &upcard) {
    hand.push_back(upcard);
    std::sort(hand.begin(), hand.end());
    // Remove lowest card (first card after sorting)
    hand.erase(hand.begin());
}

Card Simple::lead_card(Suit trump) {
    std::sort(hand.begin(), hand.end());
    
    // First try to find highest non-trump card
    for (int i = hand.size() - 1; i >= 0; i--) {
        if (!hand[i].is_trump(trump)) {
            Card card_to_play = hand[i];
            hand.erase(hand.begin() + i);
            return card_to_play;
        }
    }
    
    // If only trump cards remain, check for right and left bowers
    for (int i = hand.size() - 1; i >= 0; i--) {
        if (hand[i].is_right_bower(trump)) {
            Card card_to_play = hand[i];
            hand.erase(hand.begin() + i);
            return card_to_play;
        }
    }
    
    for (int i = hand.size() - 1; i >= 0; i--) {
        if (hand[i].is_left_bower(trump)) {
            Card card_to_play = hand[i];
            hand.erase(hand.begin() + i);
            return card_to_play;
        }
    }
    
    // Play highest remaining trump
    Card card_to_play = hand.back();
    hand.pop_back();
    return card_to_play;
}

Card Simple::play_card(const Card &led_card, Suit trump) {
    std::sort(hand.begin(), hand.end());
    
    // Try to follow suit with highest card
    for (int i = hand.size() - 1; i >= 0; i--) {
        if (hand[i].get_suit() == led_card.get_suit()) {
            Card card_to_play = hand[i];
            hand.erase(hand.begin() + i);
            return card_to_play;
        }
    }
    
    // If can't follow suit, play lowest non-trump
    for (size_t i = 0; i < hand.size(); i++) {
        if (!hand[i].is_trump(trump)) {
            Card card_to_play = hand[i];
            hand.erase(hand.begin() + i);
            return card_to_play;
        }
    }
    
    // If only trump remains, play lowest trump
    Card card_to_play = hand.front();
    hand.erase(hand.begin());
    return card_to_play;
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