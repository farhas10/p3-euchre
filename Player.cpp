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
    std::cout << "\nBefore add_and_discard, hand: ";
    for (const Card &c : hand) {
        std::cout << c << ", ";
    }
    
    hand.push_back(upcard);
    std::cout << "\nAfter adding " << upcard << ", hand: ";
    for (const Card &c : hand) {
        std::cout << c << ", ";
    }
    
    size_t discard_index = 0;
    Suit trump = upcard.get_suit();
    
    for (size_t i = 1; i < hand.size(); ++i) {
        std::cout << "\nComparing " << hand[i] << " with " << hand[discard_index];
        if (Card_less(hand[i], hand[discard_index], trump)) {
            discard_index = i;
            std::cout << " - new lowest";
        }
    }
    
    std::cout << "\nDiscarding: " << hand[discard_index];
    hand.erase(hand.begin() + discard_index);
    
    std::cout << "\nFinal hand: ";
    for (const Card &c : hand) {
        std::cout << c << ", ";
    }
    std::cout << "\n";
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
    size_t play_index = 0;
    bool can_follow_suit = false;
    
    // First check if we can follow suit
    for (size_t i = 0; i < hand.size(); ++i) {
        if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
            if (!can_follow_suit) {
                play_index = i;
                can_follow_suit = true;
            }
            else if (Card_less(hand[play_index], hand[i], led_card, trump)) {
                play_index = i;
            }
        }
    }
    
    // If we can't follow suit, play our highest trump if we have one
    if (!can_follow_suit) {
        bool found_trump = false;
        for (size_t i = 0; i < hand.size(); ++i) {
            if (hand[i].is_trump(trump)) {
                if (!found_trump || Card_less(hand[play_index], hand[i], trump)) {
                    play_index = i;
                    found_trump = true;
                }
            }
        }
        
        // If no trump, play lowest card
        if (!found_trump) {
            for (size_t i = 1; i < hand.size(); ++i) {
                if (Card_less(hand[i], hand[play_index], led_card, trump)) {
                    play_index = i;
                }
            }
        }
    }
    
    Card card_to_play = hand[play_index];
    hand.erase(hand.begin() + play_index);
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