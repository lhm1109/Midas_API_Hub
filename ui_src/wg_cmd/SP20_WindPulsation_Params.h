
#pragma once
#include <unordered_map>
#include <utility>

struct T_WNPS_D;

namespace ru_sp20 {

struct Params {
	enum struct BuildingType { None, Tower, Regular } building_type = BuildingType::None;
	enum struct MainCoordsPlane { None, ZOY, ZOX, XOY } coords_plane = MainCoordsPlane::None;
	enum struct TerrainType { None, A, B, C } terrain_type = TerrainType::None;
	enum struct WindZone { None, Ia, I, II, III, IV, V, VI, VII } wind_zone = WindZone::None;
	double wind_pressure = 1.0;                                               /* [Pa] */
	double log_decrement = 1.0;                                               /*  [1] */
	double safety_factor = 1.4;                                               /*  [1] */
	std::pair<double, double> wind_factor_xy = { 1.0, 1.0 };                  /*  [1] */
	std::pair<double, double> building_wind_direction_size_xy = { 1.0, 1.0 }; /*  [L] */
	std::pair<double, double> building_wind_cross_size_xy = { 1.0, 1.0 };     /*  [L] */
	std::unordered_map<unsigned, std::pair<bool, bool>> mode_selection_xy = {};

	Params() = default;
	Params(const T_WNPS_D& wnps);
};

}
