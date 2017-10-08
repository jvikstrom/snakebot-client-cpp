#define CATCH_CONFIG_MAIN

#include <json.hpp>
#include "catch.hpp"
#include "../src/structs.h"
#include "../src/util.h"
#include "../src/parser.h"

const int MAP_WIDTH = 3;

// The map used for testing, 1 and 2 represents the snakes
//yx012
//0  F
//1 11#
//2  2

Snake get_snake_one() {
  Snake snake;
  snake.name = "1";
  snake.points = 0;
  snake.tailProtectedForGameTicks = 0;
  snake.id = "1";

  auto p1 = std::make_tuple(1, 1);
  auto p2 = std::make_tuple(0, 1);
  snake.positions.push_back(translate_coordinate(p1, MAP_WIDTH));
  snake.positions.push_back(translate_coordinate(p2, MAP_WIDTH));

  return snake;
}

Snake get_snake_two() {
  Snake snake;
  snake.name = "2";
  snake.points = 0;
  snake.tailProtectedForGameTicks = 0;
  snake.id = "2";

  auto p1 = std::make_tuple(1, 2);
  snake.positions.push_back(translate_coordinate(p1, MAP_WIDTH));

  return snake;
}

Map construct_map() {
  Map map;
  map.width = MAP_WIDTH;
  map.height = MAP_WIDTH;
  map.worldTick = 0;
  map.snakeInfos.push_back(get_snake_one());
  map.snakeInfos.push_back(get_snake_two());

  auto foodPosition = std::make_tuple(1, 0);
  auto obstaclePosition = std::make_tuple(2, 1);

  map.obstaclePositions.push_back(translate_coordinate(obstaclePosition, MAP_WIDTH));
  map.foodPositions.push_back(translate_coordinate(foodPosition, MAP_WIDTH));

  return map;
}

TEST_CASE("snake_can_be_found_by_id", "[util]") {
  auto map = construct_map();
  auto snake_one = get_snake_one();
  auto snake = get_snake_by_id(map, snake_one.id);
  REQUIRE(snake);
};

TEST_CASE("translate_positions works", "[util]") {
  auto p1 = std::make_tuple(1, 0);
  auto p2 = std::make_tuple(1, 2);
  auto p3 = std::make_tuple(0, 1);

  REQUIRE(translate_coordinate(p1, 10) == 1);
  REQUIRE(translate_coordinate(p2, 1) == 3);
  REQUIRE(translate_coordinate(p2, 10) == 21);
  REQUIRE(translate_coordinate(p3, 3) == 3);
}

TEST_CASE("tile_is_correctly_found", "[util]") {
  std::vector<Tile> row1, row2, row3;
  std::vector<std::vector<Tile>> tiles;

  row1.push_back(Tile::Empty);
  row1.push_back(Tile::Food);
  row1.push_back(Tile::Empty);

  row2.push_back(Tile::SnakeTail);
  row2.push_back(Tile::SnakeHead);
  row2.push_back(Tile::Obstacle);

  row3.push_back(Tile::Empty);
  row3.push_back(Tile::SnakeHead);
  row3.push_back(Tile::Empty);

  tiles.push_back(row1);
  tiles.push_back(row2);
  tiles.push_back(row3);

  Map map = construct_map();

  for (int y = 0; y < map.height; y++) {
    for (int x = 0; x < map.width; x++) {
      REQUIRE(tiles[y][x] == get_tile_at(map, std::make_tuple(x, y)));
    }
  }
};

TEST_CASE("tile_is_correctly_marked_as_movable", "[util]") {
  std::vector<bool> row1, row2, row3;
  std::vector<std::vector<bool>> tiles;

  row1.push_back(true);
  row1.push_back(true);
  row1.push_back(true);

  row2.push_back(false);
  row2.push_back(false);
  row2.push_back(false);

  row3.push_back(true);
  row3.push_back(false);
  row3.push_back(true);

  tiles.push_back(row1);
  tiles.push_back(row2);
  tiles.push_back(row3);

  Map map = construct_map();
  for (int y = 0; y < map.height; ++y) {
    for (int x = 0; x < map.width; ++x) {
      REQUIRE(tiles[y][x] == is_tile_available_for_movement(map, std::make_tuple(x,y)));
    }
  }
};

TEST_CASE("can_snake_move_identifies_correctly", "[util]") {
  Map map = construct_map();
  std::string id = get_snake_one().id;
  Snake snake = get_snake_by_id(map, id).value();

  REQUIRE(can_snake_move_in_direction(map, snake, Direction::Up));
  REQUIRE(!can_snake_move_in_direction(map, snake, Direction::Down));
  REQUIRE(!can_snake_move_in_direction(map, snake, Direction::Left));
  REQUIRE(!can_snake_move_in_direction(map, snake, Direction::Right));
};

TEST_CASE("can_not_move_to_walls", "[util]") {
  Map map = construct_map();
  std::string id = get_snake_two().id;
  Snake snake = get_snake_by_id(map, id).value();

  auto can_move_down = can_snake_move_in_direction(map, snake, Direction::Down);
  REQUIRE(!can_move_down);
};

TEST_CASE("snake_parser_parses_correctly", "[parser]"){
  auto j = R"(
    {
      "id": "testid",
      "name": "testname",
      "points": 200,
      "positions": [90,91,92,93],
      "tailProtectedForGameTicks": 3
    }
  )"_json;

  Snake snake = parse_snake_json(j);
  std::vector<int> positions{90,91,92,93};
  REQUIRE(snake.id == "testid");
  REQUIRE(snake.name == "testname");
  REQUIRE(snake.points == 200);
  REQUIRE(snake.positions == positions);
  REQUIRE(snake.tailProtectedForGameTicks == 3);
}

TEST_CASE("map_parser_does_not_fail_on_empty", "[parser]"){
  auto j = R"(
    {
      "width": 10,
      "height": 10,
      "worldTick": 20,
      "foodPositions": [],
      "obstaclePositions": [],
      "snakeInfos": []
    }
  )"_json;
	
  Map map = parse_map_json(j);

  REQUIRE(map.width == 10);
  REQUIRE(map.height == 10);
  REQUIRE(map.worldTick == 20);
  REQUIRE(map.foodPositions == std::vector<int>());
  REQUIRE(map.obstaclePositions == std::vector<int>());
  REQUIRE(!map.snakeInfos.size());
}

TEST_CASE("map_parser_parses_correctly", "[parser]"){
  auto j = R"(
    {
      "height": 10,
      "width": 10,
      "worldTick": 20,
      "foodPositions": [0, 2, 4, 8, 10, 13],
      "obstaclePositions": [1, 3],
      "snakeInfos": [
        {
          "id": "testid",
          "name": "testname",
          "points": 200,
          "positions": [90, 91, 92, 93],
          "tailProtectedForGameTicks": 3
        },
        {
          "id": "testid2",
          "name": "name 2",
          "points": 0,
          "positions": [],
          "tailProtectedForGameTicks": 0
        }
      ]
    }
  )"_json;

  Map map = parse_map_json(j);
  REQUIRE(map.height == 10);
  REQUIRE(map.width == 10);
  REQUIRE(map.worldTick == 20);
  std::vector<int> foodPositions{0,2,4,8,10,13};
  REQUIRE(map.foodPositions == foodPositions);
  std::vector<int> obstaclePositions{1,3};
  REQUIRE(map.obstaclePositions == obstaclePositions);
  REQUIRE(map.snakeInfos[0].id == "testid");
  REQUIRE(map.snakeInfos[0].name == "testname");
  std::vector<int> snake1positions{90,91,92,93};
  REQUIRE(map.snakeInfos[0].positions == snake1positions);
  REQUIRE(map.snakeInfos[0].tailProtectedForGameTicks == 3);
  REQUIRE(map.snakeInfos[1].id == "testid2");
  REQUIRE(map.snakeInfos[1].name == "name 2");
  REQUIRE(map.snakeInfos[1].positions == std::vector<int>());
  REQUIRE(map.snakeInfos[1].tailProtectedForGameTicks == 0);
}

