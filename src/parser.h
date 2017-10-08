#pragma once
#include <json.hpp>
#include "structs.h"

using nlohmann::json;

Map parse_map_json(json map_json);
Snake parse_snake_json(json snake_info);
