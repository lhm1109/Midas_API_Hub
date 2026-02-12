
#include "StdAfx.h"
#include "SP20_WindPulsation.h"
#include "SP20_WindPulsation_Funcs.h"

#include "../wg_db/AttrCtrl.h"
#include "../wg_db/AttrCtrl2.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/EditData.h"
#include "../wg_db/PostCtrl.h"
#include "../wg_db/UnitCtrl.h"
#include "../wg_db/StoryLoad.h"

#undef min
#undef max

namespace ru_sp20 {

std::unordered_map<T_NODE_K, NodeData> NodeData::GetNodeData(CDBDoc& doc, const T_STLD_K wind_load_key) {
	ASSERT(doc.m_pAttrCtrl && doc.m_pPostCtrl && doc.m_pUnitCtrl && wind_load_key);

	const auto base_k = doc.m_pAttrCtrl->GetBaseStor4GL();
	ASSERT(base_k);
	const auto base_level = doc.m_pAttrCtrl->GetStorLevel(base_k);

	CMap<T_NODE_K, T_NODE_K, T_STOR_K, T_STOR_K> node_to_story;
	doc.m_pAttrCtrl->GetAllNodeforAllStor(node_to_story);

	CMap<T_STOR_K, T_STOR_K, T_StoryMass, T_StoryMass&> story_to_mass;
	CStoryLoad{}.Calc_Story_Mass(story_to_mass);

	CMap<T_STOR_K, T_STOR_K, T_CNLD_D, T_CNLD_D&> story_to_wind_load;
	{
		CMap<T_NODE_K, T_NODE_K, T_STOR_K, T_STOR_K> master_to_story;
		for (auto pos = story_to_mass.GetStartPosition(); pos != NULL;) {
			auto story_k = T_STOR_K{};
			auto story_mass = T_StoryMass{};
			story_to_mass.GetNextAssoc(pos, story_k, story_mass);
			if (story_k != 0 && story_k != base_k) { /* ignore total summation and base level */
				auto master_k = T_NODE_K{};
				const auto ok = doc.m_pPostCtrl->GetMasterNode(story_k, master_k);
				ASSERT(ok);
				master_to_story[master_k] = story_k;
			}
		}

		CArray<T_STLD_K, T_STLD_K> wind_load_keys;
		wind_load_keys.Add(wind_load_key);

		CArray<T_CNLD_UDRD_D, T_CNLD_UDRD_D&> wind_load;
		doc.m_pPostCtrl->CalcWindLoad(wind_load_keys, wind_load);
		for (auto i = INT_PTR{ 0 }, i_count = wind_load.GetCount(); i < i_count; ++i) {
			doc.m_pUnitCtrl->ConvertUnitCnldOut(wind_load[i].data); /* convert to unit */
			const auto master_k = T_NODE_K{ wind_load[i].key.key.entity };
			const auto story_k = master_to_story[master_k];
			ASSERT(master_k && story_k);
			story_to_wind_load[story_k] = wind_load[i].data;
		}
	}

	CArray<T_NMAS_UDRD_D, T_NMAS_UDRD_D&> node_masses;
	doc.calcAllMass(node_masses);

	auto node_data = std::unordered_map<T_NODE_K, NodeData>{};

	for (auto i = INT_PTR{ 0 }, i_count = node_masses.GetCount(); i < i_count; ++i) {
		const auto node_k = node_masses[i].key;
		const auto story_k = node_to_story[node_k];
		ASSERT(node_k && story_k);
		auto& data = node_data[node_k];
		data.altitude_over_base_level = doc.m_pAttrCtrl->GetStorLevel(story_k) - base_level;
		data.mass = node_masses[i].data;
		data.static_wind_load.Initialize();
		if (story_k != base_k) { /* ignore base level */
			data.static_wind_load = story_to_wind_load[story_k];
			const auto& story_mass = story_to_mass[story_k];
			for (auto j = size_t{ 0 }, j_count = std::size(data.static_wind_load.Force); j < j_count; ++j) {
				data.static_wind_load.Force[j] *= (dgn::EQ0(story_mass.Sum.Mass[j]) ? 0.0 : (data.mass.Mass[j] / story_mass.Sum.Mass[j]));
			}
		}
	}

	return node_data;
}

std::unordered_map<unsigned, PulsationData> PulsationData::GetPulsationData(const CDBDoc& doc) {
	ASSERT(doc.m_pAttrCtrl && doc.m_pPostCtrl);

	CArray<T_NODE_K, T_NODE_K> nodes;
	doc.m_pAttrCtrl->GetNodeKeyList(nodes);

	auto pulsation_data = std::unordered_map<unsigned, PulsationData>{};

	for (auto mode = 1, mode_count = doc.m_pPostCtrl->GetEigenModeCount(); mode <= mode_count; ++mode) {
		auto& data = pulsation_data[mode];
		auto freq = T_FREQ_D{};
		const auto ok = doc.m_pPostCtrl->GetEiFreq(mode, freq);
		ASSERT(ok);
		data.frequency = freq.dblFreq[1];

		doc.m_pPostCtrl->SelectMode(mode);
		for (auto i = INT_PTR{ 0 }, i_count = nodes.GetCount(); i < i_count; ++i) {
			const auto node_key = nodes[i];
			auto& node_displacement = data.displacements[node_key];
			const auto ok = doc.m_pPostCtrl->GetEiMode(node_key, node_displacement, true, false);
			ASSERT(ok);
		}
	}

	return pulsation_data;
}

std::tuple<double, double, double> GetCorrelationFactors_v_xyz(const Params::BuildingType building,
															   const std::pair<double, double> wind_factor_xy,
															   const std::pair<double, double> building_wind_direction_size_xy,
															   const std::pair<double, double> building_wind_cross_size_xy,
															   const double building_height,
															   const bool is_main_mode) {
	auto v_x = 1.0, v_y = 1.0, v_z = 1.0;
	auto rho = 1.0, chi = 1.0;

	if (building == Params::BuildingType::Tower) {
		if (is_main_mode) {
			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::ZOY, std::get<0>(building_wind_cross_size_xy), building_height, std::get<0>(building_wind_direction_size_xy));
			v_x = GetFactor_v(rho, chi);

			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::ZOX, std::get<1>(building_wind_cross_size_xy), building_height, std::get<1>(building_wind_direction_size_xy));
			v_y = GetFactor_v(rho, chi);

			v_z = 1.0;
		} else {
			v_x = 1.0, v_y = 1.0, v_z = 1.0;
		}
	} else {
		if (dgn::EQ0(std::get<1>(wind_factor_xy))) { /* wind in x-dir */
			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::ZOY, std::get<0>(building_wind_cross_size_xy), building_height, std::get<0>(building_wind_direction_size_xy));
			v_x = GetFactor_v(rho, chi);

			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::ZOX, std::get<0>(building_wind_cross_size_xy), building_height, std::get<0>(building_wind_direction_size_xy));
			v_y = GetFactor_v(rho, chi);

			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::XOY, std::get<0>(building_wind_cross_size_xy), building_height, std::get<0>(building_wind_direction_size_xy));
			v_z = GetFactor_v(rho, chi);
		} else if (dgn::EQ0(std::get<0>(wind_factor_xy))) { /* wind in y-dir */
			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::ZOX, std::get<1>(building_wind_cross_size_xy), building_height, std::get<1>(building_wind_direction_size_xy));
			v_x = GetFactor_v(rho, chi);

			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::ZOY, std::get<1>(building_wind_cross_size_xy), building_height, std::get<1>(building_wind_direction_size_xy));
			v_y = GetFactor_v(rho, chi);

			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::XOY, std::get<1>(building_wind_cross_size_xy), building_height, std::get<1>(building_wind_direction_size_xy));
			v_z = GetFactor_v(rho, chi);
		} else { /* wind in any direction */
			const auto wf_x = std::abs(std::get<0>(wind_factor_xy)) / (std::abs(std::get<0>(wind_factor_xy)) + std::abs(std::get<1>(wind_factor_xy)));
			const auto wf_y = std::abs(std::get<1>(wind_factor_xy)) / (std::abs(std::get<0>(wind_factor_xy)) + std::abs(std::get<1>(wind_factor_xy)));

			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::ZOY, std::get<0>(building_wind_cross_size_xy), building_height, std::get<0>(building_wind_direction_size_xy));
			const auto v_xx = GetFactor_v(rho, chi);
			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::ZOX, std::get<1>(building_wind_cross_size_xy), building_height, std::get<1>(building_wind_direction_size_xy));
			const auto v_xy = GetFactor_v(rho, chi);
			v_x = wf_x * v_xx + wf_y * v_xy;

			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::ZOY, std::get<1>(building_wind_cross_size_xy), building_height, std::get<1>(building_wind_direction_size_xy));
			const auto v_yy = GetFactor_v(rho, chi);
			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::ZOX, std::get<0>(building_wind_cross_size_xy), building_height, std::get<0>(building_wind_direction_size_xy));
			const auto v_yx = GetFactor_v(rho, chi);
			v_y = wf_y * v_yy + wf_x * v_yx;

			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::XOY, std::get<0>(building_wind_cross_size_xy), building_height, std::get<0>(building_wind_direction_size_xy));
			const auto v_zx = GetFactor_v(rho, chi);
			std::tie(rho, chi) = GetFactors_rho_chi(Params::MainCoordsPlane::XOY, std::get<1>(building_wind_cross_size_xy), building_height, std::get<1>(building_wind_direction_size_xy));
			const auto v_zy = GetFactor_v(rho, chi);
			v_z = wf_x * v_zx + wf_y * v_zy;
		}
	}

	return { v_x, v_y, v_z };
}

double CalcPsiFactorForMode(const Params::BuildingType building,
							const Params::TerrainType terrain,
							const std::pair<double, double> building_wind_cross_size_xy,
							const double building_height,
							const std::tuple<double, double, double> correlation_factors_v_xyz,
							const std::unordered_map<T_NODE_K, NodeData>& nodes,
							const std::unordered_map<T_NODE_K, T_MODE_D>& displacements) {
	ASSERT(nodes.size() == displacements.size());
	auto v_x = 1.0, v_y = 1.0, v_z = 1.0;
	std::tie(v_x, v_y, v_z) = correlation_factors_v_xyz;

	auto psi_numerator = 0.0, psi_denominator = 0.0;

	for (const auto& node : nodes) {
		const auto key = node.first;
		const auto& data = node.second;
		const auto& mass = data.mass.Mass;
		const auto& load = data.static_wind_load.Force;
		const auto& disp = displacements.at(key).dblMode;

		if (dgn::EQ0(data.altitude_over_base_level)) { /* ignore base level */
			continue;
		}

		const auto z_e_x = GetEquivalentAltitude_z_e(building, building_height, std::get<0>(building_wind_cross_size_xy), data.altitude_over_base_level),
			z_e_y = GetEquivalentAltitude_z_e(building, building_height, std::get<1>(building_wind_cross_size_xy), data.altitude_over_base_level);

		const auto zeta_x = GetFactor_zeta(terrain, z_e_x), zeta_y = GetFactor_zeta(terrain, z_e_y);
		const auto zeta_z = std::max(zeta_x, zeta_y);

		const auto m_x = mass[0], m_y = mass[1], m_z = mass[2];
		const auto u_x = disp[0], u_y = disp[1], u_z = disp[2];

		const auto w_pk_x = load[0] * zeta_x * v_x, w_pk_y = load[1] * zeta_y * v_y, w_pk_z = load[2] * zeta_z * v_z;

		psi_numerator += w_pk_x * u_x + w_pk_y * u_y + w_pk_z * u_z;
		psi_denominator += m_x * u_x * u_x + m_y * u_y * u_y + m_z * u_z * u_z;
	}

	if (dgn::EQ0(psi_numerator)) {
		return 0.0;
	} else if (dgn::EQ0(psi_denominator)) {
		ASSERT(false);
		return 0.0;
	} else {
		return psi_numerator / psi_denominator;
	}
}

std::unordered_map<T_NODE_K, T_CNLD_D> CalcNodalLoadsForMode(const double xi,
															 const double psi,
															 const std::unordered_map<T_NODE_K, NodeData>& nodes,
															 const std::unordered_map<T_NODE_K, T_MODE_D>& displacements) {
	ASSERT(nodes.size() == displacements.size());

	auto loads = std::unordered_map<T_NODE_K, T_CNLD_D>{};

	for (const auto& node : nodes) {
		const auto key = node.first;
		const auto& data = node.second;
		const auto& mass = data.mass.Mass;
		const auto& disp = displacements.at(key).dblMode;

		const auto m_x = mass[0], m_y = mass[1], m_z = mass[2];
		const auto u_x = disp[0], u_y = disp[1], u_z = disp[2];

		const auto w_g_x = m_x * xi * psi * u_x, w_g_y = m_y * xi * psi * u_y, w_g_z = m_z * xi * psi * u_z;

		auto& load = loads[key];
		load.Initialize();
		load.Force[0] = w_g_x, load.Force[1] = w_g_y, load.Force[2] = w_g_z;
	}

	return loads;
}

std::unordered_map<unsigned, std::unordered_map<T_NODE_K, T_CNLD_D>> CalcNodalLoads(const Params& params,
																					const double building_height,
																					const std::unordered_map<T_NODE_K, NodeData>& nodes,
																					const std::unordered_map<unsigned, PulsationData>& pulsation) {
	const auto w_0 = GetNormWindPressure_w_0_Pa(params.wind_zone, params.wind_pressure);
	const auto z_ek = GetEquivalentAltitude_z_ek(building_height);
	const auto k = GetFactor_k(params.terrain_type, z_ek);
	const auto frequency_1 = pulsation.at(1).frequency;
	const auto T_g_1 = GetFactor_T_g_1(w_0, k, params.safety_factor, frequency_1);

	auto loads = std::unordered_map<unsigned, std::unordered_map<T_NODE_K, T_CNLD_D>>{};

	for (auto mode = size_t{ 1 }, mode_count = pulsation.size(); mode <= mode_count; ++mode) {
		const auto is_main_mode = (mode == 1);
		auto selected_x = false, selected_y = false;
		std::tie(selected_x, selected_y) = params.mode_selection_xy.at(mode);

		auto wind_factor_xy = params.wind_factor_xy;
		if (!selected_x) {
			std::get<0>(wind_factor_xy) = 0.0;
		}
		if (!selected_y) {
			std::get<1>(wind_factor_xy) = 0.0;
		}

		auto& mode_loads = loads[mode];

		if (selected_x || selected_y) {
			const auto& data = pulsation.at(mode);
			const auto T_g = (T_g_1 * frequency_1) / data.frequency;
			const auto xi = GetFactor_xi(params.log_decrement, T_g);
			const auto correlation_factors_v_xyz = GetCorrelationFactors_v_xyz(params.building_type, wind_factor_xy, params.building_wind_direction_size_xy,
																			   params.building_wind_cross_size_xy, building_height, is_main_mode);
			const auto psi = CalcPsiFactorForMode(params.building_type, params.terrain_type, params.building_wind_cross_size_xy,
												  building_height, correlation_factors_v_xyz, nodes, data.displacements);
			mode_loads = CalcNodalLoadsForMode(xi, psi, nodes, data.displacements);
		}
	}

	return loads;
}

bool MakePulsation(CDBDoc& doc, T_STLD_K wind_load_key, Params params) {
	if (!doc.m_pPostCtrl->IsPostEnable()) {
		ASSERT(false); return false;
	}

	auto wind_load = T_STLD_D{};
	if (!doc.m_pAttrCtrl->GetStld(wind_load_key, wind_load)) {
		ASSERT(false); return false;
	}

	struct RAII_Edit {
		CDBDoc& doc; bool do_close;
		RAII_Edit(CDBDoc& new_doc) : doc{ new_doc }, do_close{ false } { if (!doc.IsEditing()) { doc.StartEditDB(CString{ __func__ }, CMDTYPE_FREE); do_close = true; } }
		~RAII_Edit() { if (do_close) { doc.CloseEditDB(); } }
	} raii{ doc };

	const auto nodes = NodeData::GetNodeData(doc, wind_load_key);
	const auto pulsation = PulsationData::GetPulsationData(doc);

	const auto building_height = std::max_element(nodes.begin(), nodes.end(), [] (auto& lhs, auto& rhs) {
		return lhs.second.altitude_over_base_level < rhs.second.altitude_over_base_level; }
	)->second.altitude_over_base_level;

	const auto w_0 = GetNormWindPressure_w_0_Pa(params.wind_zone, params.wind_pressure);
	const auto T_g_lim = GetFactor_T_g_lim(params.log_decrement);
	const auto z_ek = GetEquivalentAltitude_z_ek(building_height);
	const auto k = GetFactor_k(params.terrain_type, z_ek);
	const auto f_lim = GetFrequencyLimit_f_lim(w_0, k, params.safety_factor, T_g_lim);

	for (auto mode = size_t{ 1 }, mode_count = pulsation.size(); mode <= mode_count; ++mode) {
		if (pulsation.at(mode).frequency > f_lim) { /* disable modes */
			params.mode_selection_xy.at(mode) = { false, false };
		}
	}

	const auto loads = CalcNodalLoads(params, building_height, nodes, pulsation);

	/* erase prev pulsation */
	{
		const auto wind_pulsation_name = wind_load.LoadCaseName + _T(" (pulsation)");
		const auto wind_mode_prefix_name = wind_load.LoadCaseName + _T(" (mode");
		CArray<CString, CString&> stld_list;
		doc.m_pAttrCtrl->GetStldList(stld_list);
		for (auto i = INT_PTR{ 0 }, i_count = stld_list.GetCount(); i < i_count; ++i) {
			auto& stld_name = stld_list[i];
			if ((stld_name.GetLength() == wind_pulsation_name.GetLength() && stld_name == wind_pulsation_name)
		        || (stld_name.GetLength() > wind_mode_prefix_name.GetLength() && stld_name.Left(wind_mode_prefix_name.GetLength()) == wind_mode_prefix_name)) {
			    doc.m_pEditData->DelStld(stld_name);
			}
		}
	}

	{
		auto wind_pulsation_load = T_STLD_D{};
		wind_pulsation_load.Initialize();
		auto load_name = wind_load.LoadCaseName;
		load_name.AppendFormat(_T(" (pulsation)"));
		wind_pulsation_load.LoadCaseName = load_name;
		wind_pulsation_load.LoadCaseType = wind_load.LoadCaseType;
		doc.m_pEditData->DelStld(wind_pulsation_load.LoadCaseName);
		if (!doc.m_pEditData->AddStld(wind_pulsation_load)) {
			ASSERT(false); return false;
		}
	}

	// TODO: check for main mode
	for (auto mode = size_t{ 1 }, mode_count = pulsation.size(); mode <= mode_count; ++mode) {
		const auto& mode_loads = loads.at(mode);
		if (!mode_loads.empty()) {
			auto wind_pulsation_load = T_STLD_D{};
			wind_pulsation_load.Initialize();
			auto load_name = wind_load.LoadCaseName;
			load_name.AppendFormat(_T(" (mode: %d)"), mode);
			wind_pulsation_load.LoadCaseName = load_name;
			wind_pulsation_load.LoadCaseType = wind_load.LoadCaseType;
			doc.m_pEditData->DelStld(wind_pulsation_load.LoadCaseName);
			if (!doc.m_pEditData->AddStld(wind_pulsation_load)) {
				ASSERT(false); return false;
			}

			const auto wind_pulsation_load_key = doc.m_pAttrCtrl->GetStldKey(CString{ wind_pulsation_load.LoadCaseName });
			ASSERT(wind_pulsation_load_key);

			for (const auto& nodal_load : loads.at(mode)) {
				auto node_k = T_NODE_K{};
				auto load = T_CNLD_D{};
				std::tie(node_k, load) = nodal_load;

				if (dgn::EQ0(load.Force[0]) && dgn::EQ0(load.Force[1]) && dgn::EQ0(load.Force[2])) {
					continue;
				}

				load.LoadCaseKey = wind_pulsation_load_key, load.GroupKey = T_LDGR_K{ 0 };
				auto load_key = T_CNLD_K{};
				auto ok = doc.m_pAttrCtrl->MakeKeyCnld(node_k, load.LoadCaseKey, load.GroupKey, load_key);
				ASSERT(ok);
				ok = doc.m_pEditData->AddCnld(load_key, load, false);
				ASSERT(ok);
			}
		}
	}

	return true;
}

}
