#include "parser.h"

Map parse_map_json(json map_json){
	Map map;
	if(map_json.find("width") != map_json.end()){
		map.width = map_json["width"].get<int>();
	}
	
	if(map_json.find("height") != map_json.end()){
		map.height = map_json["height"].get<int>();
	}
	
	if(map_json.find("worldTick") != map_json.end()){
		map.worldTick = map_json["worldTick"].get<int>();
	}
	
	if(map_json.find("foodPositions") != map_json.end()){
		json arr = map_json["foodPositions"];
		if(arr.is_array()){
			for(int foodPosition : arr.get<std::vector<int>>()){
				map.foodPositions.push_back(foodPosition);
			}
		}
	}

  if(map_json.find("obstaclePositions") != map_json.end()){
		json arr = map_json["obstaclePositions"];
		if(arr.is_array()){
			for(int obstaclePosition : arr.get<std::vector<int>>()){
				map.obstaclePositions.push_back(obstaclePosition);
			}
		}
	}

	if(map_json.find("snakeInfos") != map_json.end()){
		for(json snake : map_json["snakeInfos"]){
			map.snakeInfos.push_back(parse_snake_json(snake));
		}
	}

	return map;
}

Snake parse_snake_json(json snake_json){
	Snake snake;
	if(snake_json.find("id") != snake_json.end()){
		snake.id = snake_json["id"].get<std::string>();
	}

	if(snake_json.find("name") != snake_json.end()){
		snake.name = snake_json["name"].get<std::string>();
	}

	if(snake_json.find("points") != snake_json.end()){
		snake.points = snake_json["points"].get<int>();
	}
	
	if(snake_json.find("positions") != snake_json.end()){
		json arr = snake_json["positions"];
		if(arr.is_array()){
			for(int position : arr.get<std::vector<int>>()){
				snake.positions.push_back(position);
			}
		}
	}

	if(snake_json.find("tailProtectedForGameTicks") != snake_json.end()){
		snake.tailProtectedForGameTicks = snake_json["tailProtectedForGameTicks"].get<int>();
	}

	return snake;
}

