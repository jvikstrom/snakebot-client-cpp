#include <string>
#include "json.hpp"

using nlohmann::json;

// Inbound
static const std::string GAME_ENDED = "se.cygni.snake.api.event.GameEndedEvent";
static const std::string TOURNAMENT_ENDED = "se.cygni.snake.api.event.TournamentEndedEvent";
static const std::string MAP_UPDATE = "se.cygni.snake.api.event.MapUpdateEvent";
static const std::string SNAKE_DEAD = "se.cygni.snake.api.event.SnakeDeadEvent";
static const std::string GAME_STARTING = "se.cygni.snake.api.event.GameStartingEvent";
static const std::string PLAYER_REGISTERED = "se.cygni.snake.api.response.PlayerRegistered";
static const std::string INVALID_PLAYER_NAME = "se.cygni.snake.api.exception.InvalidPlayerName";
static const std::string HEART_BEAT_RESPONSE = "se.cygni.snake.api.response.HeartBeatResponse";
static const std::string GAME_LINK_EVENT = "se.cygni.snake.api.event.GameLinkEvent";

// Outbound
static const std::string REGISTER_PLAYER_MESSAGE_TYPE = "se.cygni.snake.api.request.RegisterPlayer";
static const std::string START_GAME = "se.cygni.snake.api.request.StartGame";
static const std::string REGISTER_MOVE = "se.cygni.snake.api.request.RegisterMove";
static const std::string HEART_BEAT_REQUEST = "se.cygni.snake.api.request.HeartBeatRequest";
static const std::string CLIENT_INFO = "se.cygni.snake.api.request.ClientInfo";

json start_game() {
  json start_game_msg;
  start_game_msg["type"] = START_GAME;

  return start_game_msg;
}

json client_info()
{
  json client_info_msg;
  client_info_msg["type"] = CLIENT_INFO;
  client_info_msg["language"] = "C++";
  client_info_msg["languageVersion"] = "11";
  client_info_msg["operatingSystem"] = "";
  client_info_msg["operatingSystemVersion"] = "";
  client_info_msg["clientVersion"] = "0.1";

  return client_info_msg;
}

json default_game_settings()
{
  json game_settings;
  game_settings["maxNoofPlayers"] = 5;
  game_settings["startSnakeLenth"] = 1;
  game_settings["timeInMsPerTick"] = 250;
  game_settings["obstaclesEnabled"] = true;
  game_settings["foodEnabled"] = true;
  game_settings["headToTailConsumes"] = true;
  game_settings["tailConsumeGrows"] = false;
  game_settings["addFoodLikelihood"] = 15;
  game_settings["removeFoodLikelihood"] = 5;
  game_settings["spontaneousGrowthEveryNWorldTick"] = 3;
  game_settings["trainingGame"] = false;
  game_settings["pointsPerLength"] = 1;
  game_settings["pointsPerFood"] = 2;
  game_settings["pointsPerCausedDeath"] = 5;
  game_settings["pointsPerNibble"] = 10;
  game_settings["noofRoundsTailProtectedAfterNibble"] = 3;

  return game_settings;
}

json player_registration(std::string snake_name)
{
  json player_registration_msg;
  player_registration_msg["type"] = REGISTER_PLAYER_MESSAGE_TYPE;
  player_registration_msg["playerName"] = snake_name;
  player_registration_msg["gameSettings"] = default_game_settings();

  return player_registration_msg;
}

json register_move(std::string next_move, json incoming_json)
{
  json register_move_msg;
  register_move_msg["type"] = REGISTER_MOVE;
  register_move_msg["direction"] = next_move;
  register_move_msg["gameTick"] = incoming_json["gameTick"];
  register_move_msg["receivingPlayerId"] = incoming_json["receivingPlayerId"];
  register_move_msg["gameId"] = incoming_json["gameId"];

  return register_move_msg;
}
