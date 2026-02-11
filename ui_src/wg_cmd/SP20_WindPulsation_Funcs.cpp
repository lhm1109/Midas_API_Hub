
#include "StdAfx.h"
#include "SP20_WindPulsation_Params.h"
#include <algorithm>

#undef min
#undef max

namespace ru_sp20 {

/* SP20 par 11.1.5 */
double GetEquivalentAltitude_z_e(const Params::BuildingType type, const double building_height, const double building_wind_cross_size, const double altitude_z) {
	const auto h = building_height, d = building_wind_cross_size, z = altitude_z;
	ASSERT(0.0 < h && 0.0 < d && 0.0 < z);
	switch (type) {
		case Params::BuildingType::Tower:
		{
			return z;
		}
		case Params::BuildingType::Regular:
		{
			if (h <= d) {
				return h;
			} else if (d < h && h <= 2.0 * d) {
				if (0.0 < z && z < h - d) {
					return d;
				} else if (h - d <= z) {
					return h;
				}
			} else if (2.0 * d < h) {
				if (0.0 < z && z <= d) {
					return d;
				} else if (d < z && z < h - d) {
					return z;
				} else if (h - d < z) {
					return h;
				}
			}
		}
	}
	ASSERT(false); return z;
}

/* SP20 tab 11.1 */
double GetNormWindPressure_w_0_kPa(const Params::WindZone zone) {
	switch (zone) {
		case Params::WindZone::Ia:  return 0.17;
		case Params::WindZone::I:   return 0.23;
		case Params::WindZone::II:  return 0.30;
		case Params::WindZone::III: return 0.38;
		case Params::WindZone::IV:  return 0.48;
		case Params::WindZone::V:   return 0.60;
		case Params::WindZone::VI:  return 0.73;
		case Params::WindZone::VII: return 0.85;
	}
	ASSERT(false); return 1.00;
}

double GetNormWindPressure_w_0_Pa(const Params::WindZone zone, const double default_wind_pressure_w_0) {
	if (zone != Params::WindZone::None) {
		return GetNormWindPressure_w_0_kPa(zone) * 1000.0; // kPa -> Pa
	} else {
		ASSERT(0.0 < default_wind_pressure_w_0 && default_wind_pressure_w_0 < 1'000'000'000.00);
		return default_wind_pressure_w_0;
	}
}

/* SP20 tab 11.2 */
double GetFactor_k(const Params::TerrainType type, const double altitude_z_e) {
	const auto z_e = altitude_z_e;
	switch (type) {
		case Params::TerrainType::A:
		{
			static const auto table = std::vector<std::pair<double, double>>{
				{ 5.0, 0.75 }, { 10.0, 1.00 }, { 20.0, 1.25 }, { 40.0, 1.50 }, { 60.0, 1.70 }, { 80.0, 1.85 }, { 100.0, 2.00 }, { 150.0, 2.25 }, { 200.0, 2.45 }, { 250.0, 2.65 }, { 300.0, 2.75 }
			};
			if (0.0 <= z_e && z_e <= table.front().first) {
				return table.front().second;
			} else if (table.front().first < z_e && z_e <= 300.0) {
				return dgn::math::InterpolationLinear(table, z_e);
			}
		}
		case Params::TerrainType::B:
		{
			static const auto table = std::vector<std::pair<double, double>>{
				{ 5.0, 0.50 }, { 10.0, 0.65 }, { 20.0, 0.85 }, { 40.0, 1.10 }, { 60.0, 1.30 }, { 80.0, 1.45 }, { 100.0, 1.60 }, { 150.0, 1.90 }, { 200.0, 2.10 }, { 250.0, 2.30 }, { 300.0, 2.50 }
			};
			if (0.0 <= z_e && z_e <= table.front().first) {
				return table.front().second;
			} else if (table.front().first < z_e && z_e <= 300.0) {
				return dgn::math::InterpolationLinear(table, z_e);
			}
		}
		case Params::TerrainType::C:
		{
			static const auto table = std::vector<std::pair<double, double>>{
				{ 5.0, 0.40 }, { 10.0, 0.40 }, { 20.0, 0.55 }, { 40.0, 0.80 }, { 60.0, 1.00 }, { 80.0, 1.15 }, { 100.0, 1.25 }, { 150.0, 1.55 }, { 200.0, 1.80 }, { 250.0, 2.00 }, { 300.0, 2.20 }
			};
			if (0.0 <= z_e && z_e <= table.front().first) {
				return table.front().second;
			} else if (table.front().first < z_e && z_e <= 300.0) {
				return dgn::math::InterpolationLinear(table, z_e);
			}
		}
	}
	ASSERT(false); return 1.00;
}

/* SP20 tab 11.4 */
double GetFactor_zeta(const Params::TerrainType type, const double altitude_z_e) {
	const auto z_e = altitude_z_e;
	switch (type) {
		case Params::TerrainType::A:
		{
			static const auto table = std::vector<std::pair<double, double>>{
				{ 5.0, 0.85 }, { 10.0, 0.76 }, { 20.0, 0.69 }, { 40.0, 0.62 }, { 60.0, 0.58 }, { 80.0, 0.56 }, { 100.0, 0.54 }, { 150.0, 0.51 }, { 200.0, 0.49 }, { 250.0, 0.47 }, { 300.0, 0.46 }
			};
			if (0.0 <= z_e && z_e <= table.front().first) {
				return table.front().second;
			} else if (table.front().first < z_e && z_e <= 300.0) {
				return dgn::math::InterpolationLinear(table, z_e);
			}
		}
		case Params::TerrainType::B:
		{
			static const auto table = std::vector<std::pair<double, double>>{
				{ 5.0, 1.22 }, { 10.0, 1.06 }, { 20.0, 0.92 }, { 40.0, 0.80 }, { 60.0, 0.74 }, { 80.0, 0.70 }, { 100.0, 0.67 }, { 150.0, 0.62 }, { 200.0, 0.58 }, { 250.0, 0.56 }, { 300.0, 0.54 }
			};
			if (0.0 <= z_e && z_e <= table.front().first) {
				return table.front().second;
			} else if (table.front().first < z_e && z_e <= 300.0) {
				return dgn::math::InterpolationLinear(table, z_e);
			}
		}
		case Params::TerrainType::C:
		{
			static const auto table = std::vector<std::pair<double, double>>{
				{ 5.0, 1.78 }, { 10.0, 1.78 }, { 20.0, 1.50 }, { 40.0, 1.26 }, { 60.0, 1.14 }, { 80.0, 1.06 }, { 100.0, 1.00 }, { 150.0, 0.90 }, { 200.0, 0.84 }, { 250.0, 0.80 }, { 300.0, 0.76 }
			};
			if (0.0 <= z_e && z_e <= table.front().first) {
				return table.front().second;
			} else if (table.front().first < z_e && z_e <= 300.0) {
				return dgn::math::InterpolationLinear(table, z_e);
			}
		}
	}
	ASSERT(false); return 1.00;
}

/* SP20 tab 11.5 */
double GetFactor_T_g_lim(const double log_decrement) {
	const auto delta = log_decrement;
	constexpr auto precision_epsilon = 0.01;
	if (std::abs(delta - 0.15) < precision_epsilon) {
		return 0.0077;
	} else if (std::abs(delta - 0.22) < precision_epsilon) {
		return 0.0140;
	} else if (std::abs(delta - 0.30) < precision_epsilon) {
		return 0.0230;
	}
	ASSERT(false); return 1.00;
}

/* SP20 tab 11.6 */
double GetFactor_v(const double rho, const double chi) {
	ASSERT(0.0 < rho && 0.0 < chi);
	static const auto tables = std::vector<std::pair<double, std::vector<std::pair<double, double>>>>{
		{ 0.1, {
			{ 5.0, 0.95 }, { 10.0, 0.92 }, { 20.0, 0.88 }, { 40.0, 0.83 }, { 80.0, 0.76 }, { 160.0, 0.67 }, { 350.0, 0.56 }
		} },
		{ 5.0, {
			{ 5.0, 0.89 }, { 10.0, 0.87 }, { 20.0, 0.84 }, { 40.0, 0.80 }, { 80.0, 0.73 }, { 160.0, 0.65 }, { 350.0, 0.54 }
		} },
		{ 10.0, {
			{ 5.0, 0.85 }, { 10.0, 0.84 }, { 20.0, 0.81 }, { 40.0, 0.77 }, { 80.0, 0.71 }, { 160.0, 0.64 }, { 350.0, 0.53 }
		} },
		{ 20.0, {
			{ 5.0, 0.80 }, { 10.0, 0.78 }, { 20.0, 0.76 }, { 40.0, 0.73 }, { 80.0, 0.68 }, { 160.0, 0.61 }, { 350.0, 0.51 }
		} },
		{ 40.0, {
			{ 5.0, 0.72 }, { 10.0, 0.72 }, { 20.0, 0.70 }, { 40.0, 0.67 }, { 80.0, 0.63 }, { 160.0, 0.57 }, { 350.0, 0.48 }
		} },
		{ 80.0, {
			{ 5.0, 0.63 }, { 10.0, 0.63 }, { 20.0, 0.61 }, { 40.0, 0.59 }, { 80.0, 0.56 }, { 160.0, 0.51 }, { 350.0, 0.44 }
		} },
		{ 160.0, {
			{ 5.0, 0.53 }, { 10.0, 0.53 }, { 20.0, 0.52 }, { 40.0, 0.50 }, { 80.0, 0.47 }, { 160.0, 0.44 }, { 350.0, 0.38 }
		} }
	};

	const auto it = std::find_if(tables.begin(), tables.end(), [rho] (auto& table) { return rho <= table.first; });
	const auto& tab_lesser = *(it != tables.begin() ? std::prev(it) : it);
	const auto& tab_greater = *(it != tables.end() ? it : std::prev(it));

	const auto val_lesser = (chi <= tab_lesser.second.front().first) ? tab_lesser.second.front().second
		: (chi >= tab_lesser.second.back().first) ? tab_lesser.second.back().second
		: dgn::math::InterpolationLinear(tab_lesser.second, chi);

	const auto val_greater = (chi <= tab_greater.second.front().first) ? tab_greater.second.front().second
		: (chi >= tab_greater.second.back().first) ? tab_greater.second.back().second
		: dgn::math::InterpolationLinear(tab_greater.second, chi);

	const auto rho_arg = std::min(std::max(tab_lesser.first, rho), tab_greater.first); // clamp
	return dgn::math::CalcLinearFunction(tab_lesser.first, val_lesser, tab_greater.first, val_greater, rho_arg);
}

/* SP20 tab 11.7 */
std::pair<double, double> GetFactors_rho_chi(const Params::MainCoordsPlane plane, const double building_wind_cross_size, const double building_height, const double building_wind_direction_size) {
	const auto b = building_wind_cross_size, h = building_height, a = building_wind_direction_size;
	switch (plane) {
		case Params::MainCoordsPlane::ZOY: return { b, h };
		case Params::MainCoordsPlane::ZOX: return { 0.4 * a, h };
		case Params::MainCoordsPlane::XOY: return { b, a };
	}
	ASSERT(false); return { 1.00, 1.00 };
}

/* SP20 fig 11.1 */
double GetFactor_xi(const double log_decrement, const double T_g) {
	const auto delta = log_decrement;
	constexpr auto precision_epsilon = 0.01;
	if (std::abs(delta - 0.15) < precision_epsilon) {
		static const auto table = std::vector<std::pair<double, double>>{
			{ 0.00, 1.00 }, { 0.01, 1.42 }, { 0.02, 1.58 }, { 0.03, 1.73 }, { 0.04, 1.85 },
			{ 0.05, 1.95 }, { 0.06, 2.04 }, { 0.07, 2.12 }, { 0.08, 2.20 }, { 0.09, 2.27 },
			{ 0.10, 2.32 }, { 0.15, 2.57 }, { 0.20, 2.75 }, { 0.25, 2.92 }, { 0.30, 3.04 }
		};
		if (0.00 < T_g && T_g < 0.30) {
			return dgn::math::InterpolationLinear(table, T_g);
		}
	} else if (std::abs(delta - 0.22) < precision_epsilon) {
		static const auto table = std::vector<std::pair<double, double>>{
			{ 0.00, 1.00 }, { 0.01, 1.29 }, { 0.02, 1.43 }, { 0.03, 1.54 }, { 0.04, 1.62 },
			{ 0.05, 1.69 }, { 0.06, 1.76 }, { 0.07, 1.82 }, { 0.08, 1.88 }, { 0.09, 1.94 },
			{ 0.10, 2.00 }, { 0.15, 2.18 }, { 0.20, 2.33 }, { 0.25, 2.45 }, { 0.30, 2.54 }
		};
		if (0.00 < T_g && T_g < 0.30) {
			return dgn::math::InterpolationLinear(table, T_g);
		}
	} else if (std::abs(delta - 0.30) < precision_epsilon) {
		static const auto table = std::vector<std::pair<double, double>>{
			{ 0.00, 1.00 }, { 0.01, 1.21 }, { 0.02, 1.31 }, { 0.03, 1.40 }, { 0.04, 1.47 },
			{ 0.05, 1.54 }, { 0.06, 1.59 }, { 0.07, 1.64 }, { 0.08, 1.69 }, { 0.09, 1.73 },
			{ 0.10, 1.76 }, { 0.15, 1.91 }, { 0.20, 2.04 }, { 0.25, 2.12 }, { 0.30, 2.19 }
		};
		if (0.00 < T_g && T_g < 0.30) {
			return dgn::math::InterpolationLinear(table, T_g);
		}
	}
	ASSERT(false); return 1.00;
}

/* SP20 par 11.1.8 */
double GetEquivalentAltitude_z_ek(const double building_height) {
	return 0.8 * building_height;
}

double GetFactor_T_g_1(const double norm_wind_pressure, const double k, const double safety_factor, const double frequency_1) {
	const auto w_0 = norm_wind_pressure, gamma_f = safety_factor, f_1 = frequency_1;
	return std::sqrt(w_0 * k * gamma_f) / (940.0 * f_1);
}

/* SP20 par 11.1.10 */
double GetFrequencyLimit_f_lim(const double norm_wind_pressure, const double k, const double safety_factor, const double T_g_lim) {
	const auto w_0 = norm_wind_pressure, gamma_f = safety_factor;
	return std::sqrt(w_0 * k * gamma_f) / (940.0 * T_g_lim);
}

}
