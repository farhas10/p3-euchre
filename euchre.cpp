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
    Game(const string &pack_filename, bool shuffle_deck, int points, vector<Player*>& players);
    void play();

  private:
    Pack pack;
    vector<Player*> players;
    Suit trump;
    int points_to_win; 
    int dealer;
    vector <int> scores;
    bool shuffle_deck;

    void set_players(const vector<Player*>& new_players);
    void shuffle();
    void deal();
    void make_trump();
    void play_hand();
    void update_scores(const vector<int>& tricks_won);
    void print_scores();
    void print_winner();    
};

//Reads in data from terminal, parsing data into variables.
int main(int argc, char **argv) {
  bool shuffle = false;
  
  if (argc != 12){
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4" << endl;
    return 1;
  }

  
  string pack_filename = argv[1];
  ifstream file(pack_filename);

  if (file.is_open()){
    Pack pack(file);
  }

  else{
    cout << "Error opening file" << endl;
    return 1;
  }
  
  if (argv[2] == string("shuffle")){
    shuffle = true;
  }
  else if (argv[2] == string("noshuffle")){
    shuffle = false;
  }
  else{
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4" << endl;
    return 1;
  }

  int points_to_win = stoi(argv[3]);
  if(!(points_to_win > 0 &&  points_to_win <= 100)){
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4" << endl;
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
      cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
              "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4" << endl;
      return 1;
    }
  }
  Game game(pack_filename, shuffle, points_to_win, players);
  game.play();
}

//Creates an instance of game.
Game::Game(const string &pack_filename, bool shuffle_deck, int points, vector<Player*>& players)
    : pack(pack_filename), shuffle_deck(shuffle_deck), points_to_win(points), dealer(0), scores(2, 0) {
    if (shuffle_deck) {
        pack.shuffle();
    }
    set_players(players);
}

void Game::play(){
  int dealer = 0;
  //team 1 is players 0 & 2
  //team 2 is players 1 & 3
  //vector<int> scores(2,0);

  //loop until a team wins
  while(this->scores[0] < this->points_to_win && this->scores[1] < this->points_to_win){
    cout << "Hand " << dealer << endl;
    cout << players[dealer]->get_name() << " deals" << endl;
    if(shuffle_deck){
      this->shuffle();
    }
    deal();
    make_trump();
    play_hand();
    print_scores();
    dealer = (dealer + 1) % 4;
  }
  print_winner();
}

//Accesses the private player vector to set the new variables.
void Game::set_players(const vector<Player*>& new_players){
    players = new_players;
  }

void Game::shuffle(){
  pack.shuffle();
}

void Game::deal(){
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < 4; j++){
      players[j]->add_card(pack.deal_one());
    }
  }
}

void Game::make_trump(){
  //propose a trump suit
  Card potential_trump = pack.deal_one();
  cout << potential_trump << " turned up" << endl;
  Suit trump;
  bool trump_chosen = false;

  //players decide if they want that trump
  for(int i = 0; i <= 3; ++i){
    int current_player = (dealer + i) % 4;
    bool is_dealer = (current_player == dealer);

    if(players[current_player]->make_trump(potential_trump, is_dealer, 1, trump)){
      cout << players[current_player]->get_name() << " orders up " << potential_trump << endl;
      players[dealer]->add_and_discard(potential_trump);
      trump_chosen = true;
    } else{
      cout << players[current_player]->get_name() << " passes" << endl;
    }
  }
  //players decide if they want to pick trump suit
  if(trump_chosen == false){
    for(int i = 0; i <= 3; ++i){
      int current_player = (dealer + i) % 4;
      bool is_dealer = (current_player == dealer);

      if(players[current_player]->make_trump(potential_trump, is_dealer, 2, trump)){
        cout << players[current_player]->get_name() << " orders up " << trump << endl;
        trump_chosen = true;
      } else{
        cout << players[current_player]->get_name() << " passes" << endl;
      }
      //screw the dealer (simple players pick next suit)
      if (is_dealer && !trump_chosen) {
            trump = Suit_next(potential_trump.get_suit());  // Dealer must pick the next suit
            cout << players[dealer]->get_name() << " must order up " << trump << endl;
            return;
        }
    }
  }
}

void Game::play_hand(){
  vector<int> tricks_won(2, 0);
  int leader = (dealer + 1) % 4;

  for (int trick = 0; trick < 5; trick++) {
        vector<Card> trick_cards;
        vector<int> trick_players;

        //first player leads
        Card led_card = players[leader]->lead_card(trump);
        trick_cards.push_back(led_card);
        trick_players.push_back(leader);
        cout << led_card << " led by " << players[leader]->get_name() << endl;

        //other players follow
        for (int i = 1; i < 4; i++) {
            int current_player = (leader + i) % 4;
            Card played = players[current_player]->play_card(led_card, trump);
            trick_cards.push_back(played);
            trick_players.push_back(current_player);
            cout << played << " played by " << players[current_player]->get_name() << endl;
        }

        //find trick winner
        int winner_i = 0;
        for (int i = 1; i < 4; i++) {
            if (Card_less(trick_cards[winner_i], trick_cards[i], led_card, trump)) {
                winner_i = i;
            }
        }

        int winner = trick_players[winner_i];
        cout << players[winner]->get_name() << " takes the trick" << endl;

        //update tricks won & winner leads next
        tricks_won[winner % 2]++;
        leader = winner;
    }
    update_scores(tricks_won);
}

void Game::update_scores(const vector<int>& tricks_won){
  int trump_team = (dealer + 1) % 2;
    if (tricks_won[trump_team] >= 3) {
        if (tricks_won[trump_team] == 5) {
            cout << "march!" << endl;
            scores[trump_team] += 2;
        } else {
            scores[trump_team] += 1;
        }
    } else {
        cout << "euchred!" << endl;
        scores[1 - trump_team] += 2;
    }
}

void Game::print_scores(){
  cout << players[0]->get_name() << " and " << players[2]->get_name() 
         << " have " << this->scores[0] << " points" << endl;
    cout << players[1]->get_name() << " and " << players[3]->get_name() 
         << " have " << this->scores[1] << " points" << endl;
}

void Game::print_winner(){
  if (scores[0] >= this->points_to_win) {
        cout << players[0]->get_name() << " and " << players[2]->get_name() << " win!" << endl;
    } else {
        cout << players[1]->get_name() << " and " << players[3]->get_name() << " win!" << endl;
    }
}