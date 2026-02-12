
#pragma once
#include <tuple>
#include <unordered_map>
#include <utility>
#include "SP20_WindPulsation_Params.h"
#include "../wg_db/DB_ST_DT_CASE.h"
#include "../wg_db/DB_ST_DT_MASS.h"
#include "../wg_db/DB_ST_DT_NODE.h"
#include "../wg_db/DB_ST_DT_STLD.h"
#include "../wg_db/DB_ST_RT.h"

class CDBDoc;
struct T_WNPS_D;

namespace ru_sp20 {

struct NodeData {
	double altitude_over_base_level;
	T_NMAS_D mass;
	T_CNLD_D static_wind_load;

	static std::unordered_map<T_NODE_K, NodeData> GetNodeData(CDBDoc& doc, const T_STLD_K wind_load_key);
};

struct PulsationData {
	double frequency;
	std::unordered_map<T_NODE_K, T_MODE_D> displacements;

	static std::unordered_map<unsigned, PulsationData> GetPulsationData(const CDBDoc& doc);
};

std::tuple<double, double, double> GetCorrelationFactors_v_xyz(const Params::BuildingType building,
															   const std::pair<double, double> wind_factor_xy,
															   const std::pair<double, double> building_wind_direction_size_xy,
															   const std::pair<double, double> building_wind_cross_size_xy,
															   const double building_height,
															   const bool is_main_mode);

double CalcPsiFactorForMode(const Params::BuildingType building,
							const Params::TerrainType terrain,
							const std::pair<double, double> building_wind_cross_size_xy,
							const double building_height,
							const std::tuple<double, double, double> correlation_factors_v_xyz,
							const std::unordered_map<T_NODE_K, NodeData>& nodes,
							const std::unordered_map<T_NODE_K, T_MODE_D>& displacements);

std::unordered_map<T_NODE_K, T_CNLD_D> CalcNodalLoadsForMode(const double xi,
															 const double psi,
															 const std::unordered_map<T_NODE_K, NodeData>& nodes,
															 const std::unordered_map<T_NODE_K, T_MODE_D>& displacements);

std::unordered_map<unsigned, std::unordered_map<T_NODE_K, T_CNLD_D>> CalcNodalLoads(const Params& params,
																					const double building_height,
																					const std::unordered_map<T_NODE_K, NodeData>& nodes,
																					const std::unordered_map<unsigned, PulsationData>& pulsation);

bool MakePulsation(CDBDoc& doc, T_STLD_K wind_load_key, Params params);

}
