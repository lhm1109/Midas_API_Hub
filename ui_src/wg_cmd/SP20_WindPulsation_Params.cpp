
#include "StdAfx.h"
#include "SP20_WindPulsation_Params.h"
#include "../wg_db/DB_ST_DT_WIND.h"

#undef min
#undef max

namespace ru_sp20 {

Params::Params(const T_WNPS_D& wnps) {
	switch (wnps.enConstructionType) {
		case EN_WNPS_TYPE_TOWERS: this->building_type = BuildingType::Tower; break;
		case EN_WNPS_TYPE_OTHERS: this->building_type = BuildingType::Regular; break;
		default: ASSERT(false); this->building_type = BuildingType::None; break;
	}

	this->coords_plane = MainCoordsPlane::None;

	switch (wnps.enTerrainType) {
		case EN_WNPS_TERRAIN_A: this->terrain_type = TerrainType::A; break;
		case EN_WNPS_TERRAIN_B: this->terrain_type = TerrainType::B; break;
		case EN_WNPS_TERRAIN_C: this->terrain_type = TerrainType::C; break;
		default: ASSERT(false); this->terrain_type = TerrainType::None; break;
	}

	this->wind_zone = WindZone::None;

	ASSERT(0.0 < wnps.dWindPressure && wnps.dWindPressure < 1'000'000.0);
	this->wind_pressure = wnps.dWindPressure * 1000.0; // kPa -> Pa

	ASSERT(0.0 < wnps.dLogDecrement && wnps.dLogDecrement < 1.0);
	this->log_decrement = wnps.dLogDecrement;

	this->safety_factor = 1.4;

	ASSERT(-1'000.0 < wnps.dFactorX && wnps.dFactorX < +1'000.0 && -1'000.0 < wnps.dFactorY && wnps.dFactorY < +1'000.0);
	this->wind_factor_xy = { wnps.dFactorX, wnps.dFactorY };

	ASSERT(0.0 < wnps.dDimXPlan && wnps.dDimXPlan < +1'000'000.0 && 0.0 < wnps.dDimYPlan && wnps.dDimYPlan < +1'000'000.0);
	this->building_wind_direction_size_xy = { wnps.dDimXPlan, wnps.dDimYPlan };

	ASSERT(0.0 < wnps.dDimXWind && wnps.dDimXWind < +1'000'000.0 && 0.0 < wnps.dDimYWind && wnps.dDimYWind < +1'000'000.0);
	this->building_wind_cross_size_xy = { wnps.dDimXWind, wnps.dDimYWind };

	const auto mode_count = std::min(wnps.arModeShapesX.GetCount(), wnps.arModeShapesY.GetCount());
	ASSERT(0 < mode_count && mode_count == wnps.arModeShapesX.GetCount() && mode_count == wnps.arModeShapesY.GetCount());
	this->mode_selection_xy.reserve(mode_count);
	for (auto mode = 1u; mode <= mode_count; ++mode) {
		this->mode_selection_xy[mode] = { wnps.arModeShapesX[mode - 1], wnps.arModeShapesY[mode - 1] };
	}
}

}
