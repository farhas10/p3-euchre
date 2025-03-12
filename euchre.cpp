#include <iostream>
#include "Player.hpp"
#include "Pack.hpp"
#include "Card.hpp"
#include <vector>
#include <cassert>
#include <fstream>
using namespace std;

class Game{
  public:
    Game(const string &pack_filename, bool shuffle_deck, int points, 
      vector<Player*>& players);
    void play();
    const vector<Player*>& get_players() const;
    

  private:
    Pack pack;
    vector<Player*> players;
    Suit trump;
    int points_to_win; 
    int dealer;
    int hand;
    vector<int> scores;
    bool shuffle_deck;
    int trump_team;

    void set_players(const vector<Player*>& new_players);
    void shuffle();
    void deal();
    void make_trump();
    void play_hand();
    void update_scores(const vector<int>& tricks_won);
    void print_scores();
    void print_winner();    
};

string err_msg = "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] ";
string err_msg2 = "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4";

//Reads in data from terminal, parsing data into variables.
int main(int argc, char **argv) {
  // Print the executable and all arguments, ending with a space.
  cout << argv[0];
  for (int i = 1; i < argc; ++i) {
    cout << " " << argv[i];
  }
  cout << " " << endl;  // Space before newline
  
  bool shuffle = false;
  
  if (argc != 12){
    cout << err_msg << err_msg2 << endl;
    return 1;
  }

  string pack_filename = argv[1];
  ifstream file(pack_filename);

  if (!file) {
    cout << "Error opening file" << endl;
    return 1;
  }
  
  if (argv[2] == string("shuffle")){
    shuffle = true;
  }
 
  else if (argv[2] != string("shuffle") && argv[2] != string("noshuffle")){
    cout << err_msg << err_msg2 << endl;
    return 1;
  }

  int points_to_win = stoi(argv[3]);
  if(!(points_to_win > 0 && points_to_win <= 100)){
    cout << err_msg << err_msg2 << endl;
    return 1;
  }

  vector<Player*> players;
  for (int i = 4; i < argc; i += 2){
    string name = argv[i];
    string type = argv[i + 1];
    if (type == "Simple"){
      Player* player = Player_factory(name, "Simple");
      players.push_back(player);
    }
    else if (type == "Human"){
      Player* player = Player_factory(name, "Human");
      players.push_back(player);
    }
    else{
      cout << err_msg << err_msg2 << endl;
      return 1;
    }
  }
  Game game(pack_filename, shuffle, points_to_win, players);
  game.play();
}

//Creates an instance of game.
Game::Game(const string &pack_filename, bool shuffle_setting, int points, 
  vector<Player*>& players)
    : pack(), players(players), points_to_win(points), dealer(0), 
    scores(2, 0), shuffle_deck(shuffle_setting) {  
    ifstream file(pack_filename);
    if (!file) {
        cerr << "Error opening file: " << pack_filename << endl;
        exit(1);
    }

    pack = Pack(file);  
}

void Game::play(){
  //loop until a team wins
  while(this->scores[0] < this->points_to_win && this->scores[1] < this->points_to_win){
    cout << "Hand " << hand << endl;
    cout << players[dealer]->get_name() << " deals" << endl;
    
    if(shuffle_deck) {
      shuffle();
    } else{
      pack.reset(); 
    }

    deal();
    make_trump();
    play_hand();
    print_scores();
    dealer = (dealer + 1) % 4;
    hand++;
  }
  print_winner();
  for (size_t i = 0; i < players.size(); ++i) {
    delete players[i];
  }
}

//Accesses the private player vector to set the new variables.
void Game::set_players(const vector<Player*>& new_players){
    players = new_players;
}

void Game::shuffle(){
  pack.shuffle();
}

//3-2-3-2 order
void Game::deal() {
    // Deal first round: 3, 2, 3, 2
    // Starting with player to the left of the dealer
    
    // First deal 3 cards to first player
    int current_player = (dealer + 1) % 4;
    for(int i = 0; i < 3; i++) {
        Card dealt = pack.deal_one();
        players[current_player]->add_card(dealt);
    }
    
    // Deal 2 cards to second player
    current_player = (dealer + 2) % 4;
    for(int i = 0; i < 2; i++) {
        Card dealt = pack.deal_one();
        players[current_player]->add_card(dealt);
    }
    
    // Deal 3 cards to third player
    current_player = (dealer + 3) % 4;
    for(int i = 0; i < 3; i++) {
        Card dealt = pack.deal_one();
        players[current_player]->add_card(dealt);
    }
    
    // Deal 2 cards to dealer
    current_player = dealer;
    for(int i = 0; i < 2; i++) {
        Card dealt = pack.deal_one();
        players[current_player]->add_card(dealt);
    }
    
    // Second round: 2, 3, 2, 3
    // Deal 2 cards to first player
    current_player = (dealer + 1) % 4;
    for(int i = 0; i < 2; i++) {
        Card dealt = pack.deal_one();
        players[current_player]->add_card(dealt);
    }
    
    // Deal 3 cards to second player
    current_player = (dealer + 2) % 4;
    for(int i = 0; i < 3; i++) {
        Card dealt = pack.deal_one();
        players[current_player]->add_card(dealt);
    }
    
    // Deal 2 cards to third player
    current_player = (dealer + 3) % 4;
    for(int i = 0; i < 2; i++) {
        Card dealt = pack.deal_one();
        players[current_player]->add_card(dealt);
    }
    
    // Deal 3 cards to dealer
    current_player = dealer;
    for(int i = 0; i < 3; i++) {
        Card dealt = pack.deal_one();
        players[current_player]->add_card(dealt);
    }
}

void Game::make_trump(){
  Card upcard = pack.deal_one();
  cout << upcard << " turned up" << endl;
  
  bool trump_chosen = false;
  
  // Round 1
  for(int i = 1; i <= 4; ++i) {
    int current_player = (dealer + i) % 4;
    bool is_dealer = (current_player == dealer);
    
    if(players[current_player]->make_trump(upcard, is_dealer, 1, trump)) {
      cout << players[current_player]->get_name() << " orders up " << trump << endl;
      trump_team = current_player % 2;
      players[dealer]->add_and_discard(upcard);
      trump_chosen = true;
      cout << endl;  // Extra newline after making trump
      break;
    } else {
      cout << players[current_player]->get_name() << " passes" << endl;
    }
  }
  
  // Round 2 if needed
  if(trump_chosen) {
    return; // Early return to avoid nested block
  }
  
  // Continue with round 2
  for(int i = 1; i <= 4; ++i) {
    int current_player = (dealer + i) % 4;
    bool is_dealer = (current_player == dealer);
    
    if(players[current_player]->make_trump(upcard, is_dealer, 2, trump)) {
      cout << players[current_player]->get_name() << " orders up " << trump << endl;
      trump_team = current_player % 2;
      cout << endl;
      return; // Exit after trump is chosen
    }
    
    cout << players[current_player]->get_name() << " passes" << endl;
    
    // Handle dealer separately
    if(is_dealer) {
      trump = Suit_next(upcard.get_suit());
      cout << players[dealer]->get_name() << " must order up " << trump << endl;
      trump_team = dealer % 2;
      cout << endl;
    }
  }
}

void Game::play_hand(){
  vector<int> tricks_won(2, 0);
  int leader = (dealer + 1) % 4;

  for (int trick = 0; trick < 5; trick++) {
    // Lead
    Card led_card = players[leader]->lead_card(trump);
    cout << led_card << " led by " << players[leader]->get_name() << endl;
    
    // Play remaining cards
    Card highest_card = led_card;
    int winner = leader;
    
    for(int i = 1; i < 4; i++) {
      int current_player = (leader + i) % 4;
      Card played = players[current_player]->play_card(led_card, trump);
      cout << played << " played by " << players[current_player]->get_name() << endl;
      
      if(Card_less(highest_card, played, led_card, trump)) {
        highest_card = played;
        winner = current_player;
      }
    }
    
    cout << players[winner]->get_name() << " takes the trick" << endl;
    cout << endl;  // Extra newline after each trick
    
    tricks_won[winner % 2]++;
    leader = winner;
  }
  update_scores(tricks_won);
}

void Game::update_scores(const vector<int>& tricks_won){
  //cout << trump_team << endl;
  if (tricks_won[trump_team] >= 3) {
    cout << players[trump_team]->get_name() << " and " 
         << players[trump_team + 2]->get_name() << " win the hand" << endl;
    if(tricks_won[trump_team] == 5) {
      scores[trump_team] += 2;
      cout << "march!" << endl;
    } else {
      scores[trump_team] += 1;
    }
  } else {
    cout << players[1 - trump_team]->get_name() << " and " 
         << players[(1 - trump_team) + 2]->get_name() << " win the hand" << endl;
    scores[1 - trump_team] += 2;
    cout << "euchred!" << endl;
  }
}

void Game::print_scores(){
  cout << players[0]->get_name() << " and " << players[2]->get_name() 
  << " have " << scores[0] << " points" << endl;
  cout << players[1]->get_name() << " and " << players[3]->get_name() 
  << " have " << scores[1] << " points" << endl;
  cout << endl;  // Extra newline after scores
}

void Game::print_winner(){
  //cout << endl;  // Extra newline before winner
  if (scores[0] >= this->points_to_win) {
    cout << players[0]->get_name() << " and " << players[2]->get_name() << " win!" << 
    endl;
  } else {
    cout << players[1]->get_name() << " and " << players[3]->get_name() << " win!" << 
    endl;
  }
}

// Getter method implementation
const vector<Player*>& Game::get_players() const {
  return players;
}