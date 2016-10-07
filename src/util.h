#ifndef MAP_UTILS_H
#define MAP_UTILS_H

#include <string>
#include "structs.h"
#include "optional.hpp"

enum Tile {
    Wall,
    Food,
    Obstacle,
    Empty,
    SnakeHead,
    SnakeBody,
    SnakeTail
};

enum Direction {
    Down,
    Up,
    Left,
    Right
};

std::string direction_as_string(Direction direction);
std::tuple<int, int> direction_as_movement_delta(Direction direction);
bool inside_map(Map map, std::tuple<int, int> coordinate);
std::experimental::optional<Snake>get_snake_by_id(Map map, std::string id);
Tile get_tile_at(Map map, std::tuple<int, int> coordinate);
bool is_tile_available_for_movement(Map map, std::tuple<int, int>);
bool can_snake_move_in_direction(Map map, Snake snake, Direction direction);
bool is_coordinate_out_of_bounds(Map map, std::tuple<int, int>);

std::tuple<int, int> translate_position(int position, int map_width);
std::vector<std::tuple<int, int>> translate_positions(std::vector<int> positions, int map_width);
int translate_coordinate(std::tuple<int, int> coordinates, int map_width);
int get_manhattan_distance(std::tuple<int, int> start, std::tuple<int, int> goal);
float get_euclidian_distance(std::tuple<int, int> start, std::tuple<int, int> goal);
bool is_within_square(std::tuple<int, int> coord, std::tuple<int, int> ne_coord, std::tuple<int, int> sw_coord);

#endif
