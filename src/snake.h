#include "easylogging++.h"
#include "json.hpp"

struct Snake {
  std::string name = "snake++";

  std::string get_next_move(nlohmann::json map) {
    std::string response = "DOWN";

    LOG(INFO) << "Snake is making move " << response << " at worldtick: " << map["worldTick"];
    return response;
  };

  void on_game_ended() {
    LOG(INFO) << "Game has ended";
  };

  void on_tournament_ended() {
    LOG(INFO) << "Tournament has ended";
  };

  void on_snake_dead(std::string death_reason) {
    LOG(INFO) << "Our snake has died, reason was: " << death_reason;
  };

  void on_game_starting() {
    LOG(INFO) << "Game is starting";
  };

  void on_player_registered() {
    LOG(INFO) << "Player was successfully registered";
  };

  void on_invalid_playername() {
    LOG(INFO) << "The player name is invalid, try another?";
  };
};
