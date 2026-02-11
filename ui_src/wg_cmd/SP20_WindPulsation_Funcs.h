
#pragma once

#include "SP20_WindPulsation_Params.h"

namespace ru_sp20 {

/* SP20 par 11.1.5 */
double GetEquivalentAltitude_z_e(const Params::BuildingType type, const double building_height, const double building_wind_cross_size, const double altitude_z);

/* SP20 tab 11.1 */
double GetNormWindPressure_w_0_kPa(const Params::WindZone zone);
double GetNormWindPressure_w_0_Pa(const Params::WindZone zone, const double default_wind_pressure_w_0);

/* SP20 tab 11.2 */
double GetFactor_k(const Params::TerrainType type, const double altitude_z_e);

/* SP20 tab 11.4 */
double GetFactor_zeta(const Params::TerrainType type, const double altitude_z_e);

/* SP20 tab 11.5 */
double GetFactor_T_g_lim(const double log_decrement);

/* SP20 tab 11.6 */
double GetFactor_v(const double rho, const double chi);

/* SP20 tab 11.7 */
std::pair<double, double> GetFactors_rho_chi(const Params::MainCoordsPlane plane, const double building_wind_cross_size, const double building_height, const double building_wind_direction_size);

/* SP20 fig 11.1 */
double GetFactor_xi(const double log_decrement, const double T_g);

/* SP20 par 11.1.8 */
double GetEquivalentAltitude_z_ek(const double building_height);
double GetFactor_T_g_1(const double norm_wind_pressure, const double k, const double safety_factor, const double frequency_1);

/* SP20 par 11.1.10 */
double GetFrequencyLimit_f_lim(const double norm_wind_pressure, const double k, const double safety_factor, const double T_g_lim);

}
