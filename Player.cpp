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

    // Helper method to find cards that follow the led suit
    std::vector<int> find_following_suit_cards(const Card &led_card, Suit trump) const;

    // Helper method to find the best card among those that follow suit
    int find_best_following_suit_card(const std::vector<int> &indices, Suit trump) const;

    // Helper method to find the lowest non-trump card
    int find_lowest_non_trump_card(Suit trump) const;

    // Helper method to find the lowest trump card that's not a bower
    int find_lowest_non_bower_trump(Suit trump) const;

    // Helper method to find the highest non-trump card
    int find_highest_non_trump_card(Suit trump) const;

    // Helper method to find special bower cards
    int find_bower(Suit trump, bool right_bower) const;

    // Helper method to find the highest trump card
    int find_highest_trump_card(Suit trump) const;
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
    Card select_card_from_hand(const std::string &prompt);
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
    int non_trump_index = find_highest_non_trump_card(trump);
    if (non_trump_index != -1) {
        Card card_to_play = hand[non_trump_index];
        hand.erase(hand.begin() + non_trump_index);
        return card_to_play;
    }

    // Right bower
    int right_bower_index = find_bower(trump, true);
    if (right_bower_index != -1) {
        Card card_to_play = hand[right_bower_index];
        hand.erase(hand.begin() + right_bower_index);
        return card_to_play;
    }

    // Left bower
    int left_bower_index = find_bower(trump, false);
    if (left_bower_index != -1) {
        Card card_to_play = hand[left_bower_index];
        hand.erase(hand.begin() + left_bower_index);
        return card_to_play;
    }

    // Highest trump card
    int highest_trump_index = find_highest_trump_card(trump);
    if (highest_trump_index != -1) {
        Card card_to_play = hand[highest_trump_index];
        hand.erase(hand.begin() + highest_trump_index);
        return card_to_play;
    }

    // If all else fails, play the last card
    Card card_to_play = hand.back();
    hand.pop_back();
    return card_to_play;
}

// Helper method to find cards that follow the led suit
std::vector<int> Simple::find_following_suit_cards(const Card &led_card, Suit trump) const {
    std::vector<int> following_suit_indices;
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
            following_suit_indices.push_back(i);
        }
    }
    return following_suit_indices;
}

// Helper method to find the best card among those that follow suit
int Simple::find_best_following_suit_card(const std::vector<int> &indices, Suit trump) const {
    int best_index = indices[0];
    
    for (int i : indices) {
        if (hand[i].is_right_bower(trump)) {
            return i;  // Right bower is always best
        }
        if (hand[i].is_left_bower(trump) && !hand[best_index].is_right_bower(trump)) {
            best_index = i;
            continue;
        }
        bool current_is_bower = hand[best_index].is_right_bower(trump) || 
                              hand[best_index].is_left_bower(trump);
        
        if (!current_is_bower && hand[i] > hand[best_index]) {
            best_index = i;
        }
    }
    return best_index;
}

// Helper method to find the lowest non-trump card
int Simple::find_lowest_non_trump_card(Suit trump) const {
    int lowest_i = -1;
    for (int i = 0; i < hand.size(); ++i) {
        if (!hand[i].is_trump(trump) && 
            !hand[i].is_left_bower(trump) && !hand[i].is_right_bower(trump)) {
            if (lowest_i == -1 || hand[i] < hand[lowest_i])
                lowest_i = i;
        }
    }
    return lowest_i;
}

// Helper method to find the lowest trump card that's not a bower
int Simple::find_lowest_non_bower_trump(Suit trump) const {
    int lowest_i_trump = -1;
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].is_trump(trump) && !hand[i].is_left_bower(trump) 
            && !hand[i].is_right_bower(trump)) {
            if (lowest_i_trump == -1 || hand[i] < hand[lowest_i_trump])
                lowest_i_trump = i;
        }
    }
    return lowest_i_trump;
}

Card Simple::play_card(const Card &led_card, Suit trump) {
    std::sort(hand.begin(), hand.end());

    // Find cards that follow suit
    std::vector<int> following_suit_indices = find_following_suit_cards(led_card, trump);
    
    // If we found cards that follow suit, determine the best one
    if (!following_suit_indices.empty()) {
        int best_index = find_best_following_suit_card(following_suit_indices, trump);
        Card card_to_play = hand[best_index];
        hand.erase(hand.begin() + best_index);
        return card_to_play;
    }

    // If cannot follow suit, try to play the lowest non-trump card
    int lowest_i = find_lowest_non_trump_card(trump);
    if (lowest_i != -1) {
        Card card_to_play = hand[lowest_i];
        hand.erase(hand.begin() + lowest_i);
        return card_to_play;
    }

    // If only trump cards remain, try to find a non-bower trump card
    int lowest_i_trump = find_lowest_non_bower_trump(trump);
    if (lowest_i_trump != -1) {
        Card card_to_play = hand[lowest_i_trump];
        hand.erase(hand.begin() + lowest_i_trump);
        return card_to_play;
    }

    // If no non-bower trump exists, then if the left bower is in hand, play it
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].is_left_bower(trump)) {
            Card card_to_play = hand[i];
            hand.erase(hand.begin() + i);
            return card_to_play;
        }
    }

    // If needed, play any trump card (could be right bower)
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].is_trump(trump)) {
            Card card_to_play = hand[i];
            hand.erase(hand.begin() + i);
            return card_to_play;
        }
    }

    // If all else fails, play the first card
    Card card_to_play = hand.front();
    hand.erase(hand.begin());
    return card_to_play;
}

// Helper method to find the highest non-trump card
int Simple::find_highest_non_trump_card(Suit trump) const {
    for (int i = hand.size() - 1; i >= 0; i--) {
        if (!hand[i].is_trump(trump)) {
            return i;
        }
    }
    return -1;  // No non-trump cards found
}

// Helper method to find special bower cards
int Simple::find_bower(Suit trump, bool right_bower) const {
    for (int i = hand.size() - 1; i >= 0; i--) {
        if (right_bower && hand[i].is_right_bower(trump)) {
            return i;
        }
        else if (!right_bower && hand[i].is_left_bower(trump)) {
            return i;
        }
    }
    return -1;  // Bower not found
}

// Helper method to find the highest trump card
int Simple::find_highest_trump_card(Suit trump) const {
    int highest_trump_index = -1;
    for (int i = hand.size() - 1; i >= 0; i--) {
        if (hand[i].is_trump(trump)) {
            if (highest_trump_index == -1 || hand[i] > hand[highest_trump_index]) {
                highest_trump_index = i;
            }
        }
    }
    return highest_trump_index;
}

// Human class implementations
Human::Human(const std::string &name_in) : name(name_in) {}

const string & Human::get_name() const {
    return name;
}

void Human::add_card(const Card &c) {
    hand.push_back(c);
    sort(hand.begin(), hand.end());
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
    // Print player's hand
    for (size_t i = 0; i < hand.size(); ++i) {
        cout << "Human player " << name << "'s hand: "
             << "[" << i << "] " << hand[i] << "\n";
    }
    
    // Prompt for decision
    cout << "Human player " << name 
         << ", please enter a suit, or \"pass\":" << endl;
    
    string input;
    cin >> input;
    
    if (input == "pass") {
        return false;
    }
    
    if (input == "Spades" || input == "Hearts" || 
        input == "Clubs" || input == "Diamonds") {
        order_up_suit = string_to_suit(input);
        return true;
    }
    
    return false;
}

void Human::add_and_discard(const Card &upcard) {
    // Print current hand
    for (size_t i = 0; i < hand.size(); ++i) {
        cout << "Human player " << name << "'s hand: "
             << "[" << i << "] " << hand[i] << "\n";
    }
    
    // Add upcard
    hand.push_back(upcard);
    
    // Show discard option
    cout << "Discard upcard: [-1]" << endl;
    
    // Prompt for discard
    cout << "Human player " << name 
         << ", please select a card to discard:";
    
    int index;
    cin >> index;
    
    // Handle discard
    if (index >= 0 && index < static_cast<int>(hand.size())) {
        hand.erase(hand.begin() + index);
    } else {
        hand.pop_back(); // Discard the upcard
    }
    
    cout << endl;
}

Card Human::select_card_from_hand(const std::string &prompt) {
    // Print current hand
    for (size_t i = 0; i < hand.size(); ++i) {
        cout << "Human player " << name << "'s hand: "
             << "[" << i << "] " << hand[i] << "\n";
    }
    
    // Prompt for card selection
    cout << "Human player " << name 
         << ", " << prompt << endl;
    
    int index;
    cin >> index;
    
    // Handle invalid input
    if (index < 0 || index >= static_cast<int>(hand.size())) {
        index = 0;
    }
    
    Card card_to_play = hand[index];
    hand.erase(hand.begin() + index);
    return card_to_play;
}

Card Human::lead_card(Suit trump) {
    return select_card_from_hand("please select a card:");
}

Card Human::play_card(const Card &led_card, Suit trump) {
    std::sort(hand.begin(), hand.end());
    return select_card_from_hand("please select a card:");
}