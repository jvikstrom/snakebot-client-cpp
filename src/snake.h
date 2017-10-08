#pragma once

#include "json.hpp"
#include "structs.h"

class MySnake
{
public:
  std::string name = "snake_c++";
  Direction get_next_move(Map map);
  void on_game_ended();
  void on_tournament_ended();
  void on_snake_dead(std::string death_reason);
  void on_game_starting();
  void on_player_registered();
  void on_invalid_playername();
  void on_game_result(nlohmann::json playerRanks);
};
