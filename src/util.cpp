#include <string>
#include <math.h>
#include "optional.hpp"
#include "util.h"
#include "structs.h"

using std::experimental::optional;

std::string direction_as_string(Direction direction) {
  switch(direction) {
  case Down:
    return "DOWN";
  case Up:
    return "UP";
  case Left:
    return "LEFT";
  case Right:
    return "RIGHT";
  }
}

std::tuple<int, int> direction_as_movement_delta(Direction direction) {
  switch(direction) {
  case Down:
    return std::make_tuple(0, 1);
  case Up:
    return std::make_tuple(0, -1);
  case Left:
    return std::make_tuple(-1, 0);
  case Right:
    return std::make_tuple(1, 0);
  }
}

bool inside_map(Map map, std::tuple<int, int> coordinate) {
  int x, y;
  std::tie(x, y) = coordinate;
  bool inside_x = x >= 0 && x < map.width;
  bool inside_y = y >= 0 && y < map.height;
  return inside_x && inside_y;
}

optional<Snake> get_snake_by_id(Map map, std::string id) {
  auto it =
    std::find_if(map.snakeInfos.begin(),
                 map.snakeInfos.end(),
                 [id](Snake snake) {
                   return snake.id == id;
                 });

  if (it != map.snakeInfos.end()) {
    return *it;
  }

  return std::experimental::nullopt;
}

bool list_contains_position(std::vector<int> positions, int position) {
  return std::find(positions.begin(),
                   positions.end(),
                   position) != positions.end();
}

Tile get_tile_at(Map map, std::tuple<int, int> coordinate) {
  int position = translate_coordinate(coordinate, map.width);
  std::vector<Snake> snake_at_tile(map.snakeInfos.size());
  auto it = std::copy_if(map.snakeInfos.begin(),
               map.snakeInfos.end(),
               snake_at_tile.begin(),
               [position](Snake snake) {
                 return list_contains_position(snake.positions, position);
               });

  snake_at_tile.resize(std::distance(snake_at_tile.begin(), it));

  if (list_contains_position(map.obstaclePositions, position)) {
    return Obstacle;
  } else if (list_contains_position(map.foodPositions, position)) {
    return Food;
  } else if (snake_at_tile.size() > 0) {
    Snake snake = snake_at_tile[0];

    if (snake.positions[0] == position) {
      return SnakeHead;
    } else if (snake.positions[snake.positions.size() - 1] == position) {
      return SnakeTail;
    } else {
      return SnakeBody;
    }
  } else if (!inside_map(map, coordinate)) {
    return Wall;
  } else {
    return Empty;
  }
}

bool is_tile_available_for_movement(Map map, std::tuple<int, int> coordinate) {
  Tile tile = get_tile_at(map, coordinate);
  switch(tile) {
  case Empty:
    return true;
  case Food:
    return true;
  default:
    return false;
  }
}

bool can_snake_move_in_direction(Map map, Snake snake, Direction direction) {
  int x, y, xd, yd;

  std::tie(xd, yd) = direction_as_movement_delta(direction);
  std::tie(x, y) = translate_position(snake.positions[0], map.width);

  std::tuple<int, int> new_coord = std::make_tuple(x+xd, y+yd);

  return is_tile_available_for_movement(map, new_coord);
}

bool is_coordinate_out_of_bounds(Map map, std::tuple<int, int> coordinate) {
  int x, y;
  std::tie(x,y) = coordinate;
  return x < 0 || x >= map.width || y < 0 || y >= map.height;
}

std::tuple<int, int> translate_position(int position, int map_width) {
  float pos = position;
  float width = map_width;

  int y = floor(pos / width);
  int x = fabs(pos - y * width);

  return std::make_tuple(x, y);
}

std::vector<std::tuple<int, int>> translate_positions(std::vector<int> positions, int map_width) {
  std::vector<std::tuple<int, int>> result;
  result.resize(positions.size());

  std::transform(positions.begin(), positions.end(), result.begin(),
                 [map_width](int pos) {
                   return translate_position(pos, map_width);
                 });

  return result;
}

int translate_coordinate(std::tuple<int, int> coordinates, int map_width) {
  int x, y;
  std::tie(x, y) = coordinates;
  return x + y * map_width;
}

int get_manhattan_distance(std::tuple<int, int> start, std::tuple<int, int> goal) {
  int x1, y1, x2, y2;

  std::tie(x1, y1) = start;
  std::tie(x2, y2) = goal;

  int x = abs( x1 - x2 );
  int y = abs( y1 - y2 );

  return x+y;
}

float get_euclidian_distance(std::tuple<int, int> start, std::tuple<int, int> goal) {
  int x1, y1, x2, y2;

  std::tie(x1,y1) = start;
  std::tie(x2,y2) = goal;

  int x = pow(x1 - x2, 2);
  int y = pow(y1 - y2, 2);
  float d = (x + y);

  return floor(sqrt(d));
}

bool is_within_square(std::tuple<int, int> coord, std::tuple<int, int> ne_coord, std::tuple<int, int> sw_coord) {
  int x, y, ne_x, ne_y, sw_x, sw_y;

  std::tie(x,y) = coord;
  std::tie(ne_x, ne_y) = ne_coord;
  std::tie(sw_x, sw_y) = sw_coord;

  return x < ne_x || x > sw_x || y < sw_y || y > ne_y;
}
