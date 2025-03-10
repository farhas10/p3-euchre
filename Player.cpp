#include "Player.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>

using namespace std;

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
    //try to discard a non-trump card
    for (size_t i = 0; i < hand.size(); ++i) {
        if (!hand[i].is_trump(upcard.get_suit())) {
            hand.erase(hand.begin() + i);
            return;
        }
    }
    // If all cards are trump, discard the lowest trump.
    hand.erase(hand.begin());
}

//When a Simple Player leads a trick, they play the highest non-trump card in their hand
//If they have only trump cards, they play the highest trump card in their hand.
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

    // Right bower
    for (int i = hand.size() - 1; i >= 0; i--) {
        if (hand[i].is_right_bower(trump)) {
            Card card_to_play = hand[i];
            hand.erase(hand.begin() + i);
            return card_to_play;
        }
    }

    // Left bower
    for (int i = hand.size() - 1; i >= 0; i--) {
        if (hand[i].is_left_bower(trump)) {
            Card card_to_play = hand[i];
            hand.erase(hand.begin() + i);
            return card_to_play;
        }
    }

    // Highest trump card
    int highest_trump_index = -1;
    for (int i = hand.size() - 1; i >= 0; i--) {
        if (hand[i].is_trump(trump)) {
            if (highest_trump_index == -1 || hand[i] > hand[highest_trump_index]) {
                highest_trump_index = i;  // Track highest trump card
            }
        }
    }
    if (highest_trump_index != -1) {
        Card card_to_play = hand[highest_trump_index];
        hand.erase(hand.begin() + highest_trump_index);
        return card_to_play;
    }

    Card card_to_play = hand.back();
    hand.pop_back();
    return card_to_play;
}

Card Simple::play_card(const Card &led_card, Suit trump) {
    std::sort(hand.begin(), hand.end());

    //try to follow suit with the highest card
    int highest_i = -1;
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].get_suit(trump) == led_card.get_suit(trump) && !hand[i].is_left_bower(trump)) {
            if (highest_i == -1 || hand[i] > hand[highest_i])
                highest_i = i;
        }
    }
    if (highest_i != -1) {
        Card card_to_play = hand[highest_i];
        hand.erase(hand.begin() + highest_i);
        return card_to_play;
    }

    //if cannot follow suit, try to play the lowest non-trump card (avoiding bowers)
    int lowest_i = -1;
    for (int i = 0; i < hand.size(); ++i) {
        if (!hand[i].is_trump(trump) && !hand[i].is_left_bower(trump) && !hand[i].is_right_bower(trump)) {
            if (lowest_i == -1 || hand[i] < hand[lowest_i])
                lowest_i = i;
        }
    }
    if (lowest_i != -1) {
        Card card_to_play = hand[lowest_i];
        hand.erase(hand.begin() + lowest_i);
        return card_to_play;
    }

    //if only trump cards remain, try to find a non-bower trump card
    int lowest_i_trump = -1;
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].is_trump(trump) && !hand[i].is_left_bower(trump) && !hand[i].is_right_bower(trump)) {
            if (lowest_i_trump == -1 || hand[i] < hand[lowest_i_trump])
                lowest_i_trump = i;
        }
    }
    if (lowest_i_trump != -1) {
        Card card_to_play = hand[lowest_i_trump];
        hand.erase(hand.begin() + lowest_i_trump);
        return card_to_play;
    }

    //if no non-bower trump exists, then if the left bower is in hand, play it
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].is_left_bower(trump)) {
            Card card_to_play = hand[i];
            hand.erase(hand.begin() + i);
            return card_to_play;
        }
    }

    //if needed, play any trump card (could be right bower)
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].is_trump(trump)) {
            Card card_to_play = hand[i];
            hand.erase(hand.begin() + i);
            return card_to_play;
        }
    }

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
    print_hand();
    cout << "Human player " << name << ", please enter a suit, or \"pass\": ";
    string input;
    cin >> input;
    if (input == "pass") {
    cout << name << " passes" << endl;
    return false;
    }
    if (input != "Hearts" && input != "Diamonds" &&
    input != "Clubs" && input != "Spades") {
    cout << "Invalid suit entered. " << name << " passes" << endl;
    return false;
    }
    order_up_suit = string_to_suit(input);
    cout << name << " orders up " << input << endl;
    return true;
}

void Human::add_and_discard(const Card &upcard) {
    hand.push_back(upcard);
    sort(hand.begin(), hand.end());
    print_hand();
    cout << "Discard upcard: [-1]" << endl;
    cout << "Human player " << name << ", please select a card to discard: ";
    int index;
    cin >> index;
    if (index < 0 || index >= static_cast<int>(hand.size())) {
        cout << "Invalid index. Discarding the last card." << endl;
        hand.pop_back();
    } else {
        hand.erase(hand.begin() + index);
    }
    cout << endl;
}

Card Human::lead_card(Suit trump) {
    print_hand();
    cout << "Human player " << name << ", please select a card to lead: ";
    int index;
    cin >> index;
    if (index < 0 || index >= static_cast<int>(hand.size())) {
        cout << "Invalid selection. Playing the first card." << endl;
        index = 0;
    }
    Card card = hand[index];
    hand.erase(hand.begin() + index);
    cout << card << " led by " << name << endl;
    return card;
}

Card Human::play_card(const Card &led_card, Suit trump) {
    print_hand();
    cout << "Human player " << name << ", please select a card: ";
    int index;
    cin >> index;
    if (index < 0 || index >= static_cast<int>(hand.size())) {
        cout << "Invalid selection. Playing the first card." << std::endl;
        index = 0;
    }
    Card card = hand[index];
    hand.erase(hand.begin() + index);
    return card;
}