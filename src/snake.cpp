#include "easylogging++.h"
#include "json.hpp"
#include "snake.h"
#include "util.h"

using nlohmann::json;

std::vector<Direction> dirs{Direction::Down, Direction::Up, Direction::Right, Direction::Left};

Direction MySnake::get_next_move(Map map) {
	Direction move_dir = Direction::Down;
	for(auto dir : dirs){
		if(can_snake_move_in_direction(map, get_snake_by_name(map, this->name).value(), dir)){
			move_dir = dir;
		}
	}

  LOG(INFO) << "Snake is making move " << direction_as_string(move_dir) << " at worldtick: " << map.worldTick;
  return move_dir;
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
