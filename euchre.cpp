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
};

int main(int argc, char* argv[]) {
  vector<Player*> players;
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

Game::Game(const string &pack_filename, bool shuffle_deck, int points, vector<Player*>& players){
  
}

void Game::play(){
};