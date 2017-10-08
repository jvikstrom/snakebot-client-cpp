#include "easylogging++.h"
#include "json.hpp"
#include "snake.h"

using nlohmann::json;

std::string MySnake::get_next_move(Map map) {
  std::string response = "DOWN";

  LOG(INFO) << "Snake is making move " << response << " at worldtick: " << map.worldTick;
  return response;
};

void MySnake::on_game_ended() {
  LOG(INFO) << "Game has ended";
};

void MySnake::on_tournament_ended() {
  LOG(INFO) << "Tournament has ended";
};

void MySnake::on_snake_dead(std::string death_reason) {
  LOG(INFO) << "Our snake has died, reason was: " << death_reason;
};

void MySnake::on_game_starting() {
  LOG(INFO) << "Game is starting";
};

void MySnake::on_player_registered() {
  LOG(INFO) << "Player was successfully registered";
};

void MySnake::on_invalid_playername() {
  LOG(INFO) << "The player name is invalid, try another?";
};

void MySnake::on_game_result(nlohmann::json playerRanks) {
  LOG(INFO) << "Game result:";
  nlohmann::json playerRank;
  el::Logger* defaultLogger = el::Loggers::getLogger("default");
  for (json::iterator it = playerRanks.begin(); it != playerRanks.end(); ++it) {
    playerRank = (nlohmann::json) *it;
    defaultLogger->info("%v.\t%v pts\t%v (%v)", playerRank["rank"], playerRank["points"],
            playerRank["playerName"], playerRank["alive"] ? "alive" : "dead");
  }
};
;
