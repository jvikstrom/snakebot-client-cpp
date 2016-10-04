#include "easywsclient/easywsclient.cpp"
#include "json.hpp"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <string>
#include <memory>

using easywsclient::WebSocket;
using nlohmann::json;

// Inbound
const std::string GAME_ENDED = "se.cygni.snake.api.event.GameEndedEvent";
const std::string TOURNAMENT_ENDED = "se.cygni.snake.api.event.TournamentEndedEvent";
const std::string MAP_UPDATE = "se.cygni.snake.api.event.MapUpdateEvent";
const std::string SNAKE_DEAD = "se.cygni.snake.api.event.SnakeDeadEvent";
const std::string GAME_STARTING = "se.cygni.snake.api.event.GameStartingEvent";
const std::string PLAYER_REGISTERED = "se.cygni.snake.api.response.PlayerRegistered";
const std::string INVALID_PLAYER_NAME = "se.cygni.snake.api.exception.InvalidPlayerName";
const std::string HEART_BEAT_RESPONSE = "se.cygni.snake.api.response.HeartBeatResponse";
const std::string GAME_LINK_EVENT = "se.cygni.snake.api.event.GameLinkEvent";

// Outbound
const std::string REGISTER_PLAYER_MESSAGE_TYPE = "se.cygni.snake.api.request.RegisterPlayer";
const std::string START_GAME = "se.cygni.snake.api.request.StartGame";
const std::string REGISTER_MOVE = "se.cygni.snake.api.request.RegisterMove";
const std::string HEART_BEAT_REQUEST = "se.cygni.snake.api.request.HeartBeatRequest";
const std::string CLIENT_INFO = "se.cygni.snake.api.request.ClientInfo";

// Websocket
const std::string host = "localhost";
const std::string port = "8080";
const std::string snake_name = "snake++";
const std::string venue = "training";

std::unique_ptr<WebSocket> connect_to_server()
{
  auto url = "ws://" + host + ":" + port + "/" + venue;
  std::unique_ptr<WebSocket> ws(WebSocket::from_url(url));

  assert(ws);
  return ws;
}

json get_client_info()
{
  json client_info;
  client_info["type"] = CLIENT_INFO;
  client_info["language"] = "C++";
  client_info["languageVersion"] = "11";
  client_info["operatingSystem"] = "";
  client_info["operatingSystemVersion"] = "";
  client_info["clientVersion"] = "0.1";

  return client_info;
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

json player_registration_msg()
{
  json player_registration_msg;
  player_registration_msg["type"] = REGISTER_PLAYER_MESSAGE_TYPE;
  player_registration_msg["playerName"] = snake_name;
  player_registration_msg["gameSettings"] = default_game_settings();

  return player_registration_msg;
}

void route_message(WebSocket::pointer wsp, const std::string & message)
{
  LOG(DEBUG) << "Received message (in unparsed state)" + message;

  auto incoming_json = json::parse(message.c_str());
  std::string type = incoming_json["type"];
  LOG(DEBUG) << "Received message of type " + type;
  LOG(DEBUG) << incoming_json.dump(2);

  if (type == GAME_ENDED) {
    std::string game_mode = incoming_json["gameMode"];
    // TODO: What if it's a tournament, should we still close the websocket?
    wsp->close();
  } else if (type == TOURNAMENT_ENDED) {
    wsp->close();
  } else if (type == MAP_UPDATE) {
    json register_move_msg;
    register_move_msg["type"] = REGISTER_MOVE;
    register_move_msg["direction"] = "DOWN";
    register_move_msg["gameTick"] = incoming_json["gameTick"];
    register_move_msg["receivingPlayerId"] = incoming_json["receivingPlayerId"];
    register_move_msg["gameId"] = incoming_json["gameId"];

    LOG(DEBUG) << "Responding to map update";
    LOG(DEBUG) << register_move_msg.dump(2);

    wsp->send(register_move_msg.dump());
  } else if (type == SNAKE_DEAD) {

  } else if (type == GAME_STARTING) {
    LOG(INFO) << "The game is starting";
  } else if (type == PLAYER_REGISTERED) {
    LOG(INFO) << "Player was successfully registered";
    std::string game_mode = incoming_json["gameMode"];
    if (game_mode == "TRAINING") {
      LOG(INFO) << "Requesting a game start";

      json start_game_msg;
      start_game_msg["type"] = START_GAME;
      wsp->send(start_game_msg.dump());
    }

  } else if (type == INVALID_PLAYER_NAME) {

  } else if (type == HEART_BEAT_RESPONSE) {

  } else if (type == GAME_LINK_EVENT) {
    std::string url = incoming_json["url"];
    LOG(INFO) << "Watch game at: " + url;
  } else {
    LOG(WARNING) << "Unable to route message, did not match any known type";
    LOG(WARNING) << incoming_json.dump(2);
  }
}

int main(int argc, char* argv[])
{
  START_EASYLOGGINGPP(argc, argv);

  json client_info = get_client_info();
  json player_registration = player_registration_msg();

  auto ws = connect_to_server();
  ws->send(client_info.dump());
  ws->send(player_registration.dump());
  ws->poll();

  while (true) {
    WebSocket::pointer wsp = &*ws; // <-- because a unique_ptr cannot be copied into a lambda
    ws->poll();
    ws->dispatch([wsp](const std::string & message) {
        route_message(wsp, message);
      });
  }

  LOG(INFO) << "Websocket closed, shutting down";
  return 0;
}
