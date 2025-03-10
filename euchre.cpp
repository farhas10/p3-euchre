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
    void set_players(const vector<Player*>& new_players);
    void shuffle();
    void deal();
    void make_trump();
    void play_hand();
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
Game::Game(const string &pack_filename, bool shuffle_deck, int points, vector<Player*>& players){
  ifstream file(pack_filename);
  Pack pack(file);
  if (shuffle_deck){
    pack.shuffle();
  }
  set_players(players);
}

void Game::play(){
  int dealer = 0;
  //team 1 is players 0 & 2
  //team 2 is players 1 & 3
  vector<int> scores(2,0);

  //loop until a team wins
  while(scores[0] < points_to_win && scores[1] < points_to_win){
    cout << "Hand " << dealer << endl;
    cout << players[dealer]->get_name() << " deals" << endl;
    if(shuffle){
      shuffle();
    }
    deal();
    make_trump();
    play_hand();
    print_scores(scores);
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
  for(int i = 0; i < 3; ++i){
    int current_player = (dealer + i) % 4;
    bool is_dealer = (current_player == dealer);

    if(players[current_player]->make_trump(potential_trump, is_dealer, 1, trump)){
      
    }
  }

  //players decide if they want to pick trump suit
}

void Game::play_hand(){

}

void Game::print_scores(){

}

void Game::print_winner(){

}






