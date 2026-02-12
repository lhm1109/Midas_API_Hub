#pragma once
#define MIDAS_SDK

#pragma pack (push, 1)
// #pragma warning ( disable : 4819 )
// #pragma warning ( disable : 4244 )

#define T_SP16CombinedSectParams_K T_ELEM_K
#define HASHSIZESP16CombinedSectParams HASHSIZEELEM

enum SP16CSP_RDO_BRANCH_CONN {
	SP16CSP_RDO_BRANCH_CONN_PLANK, SP16CSP_RDO_BRANCH_CONN_LATTICE
};

enum SP16CSP_RDO_BRANCH_LENGTH {
	SP16CSP_RDO_BRANCH_LENGTH_SAME, 
	SP16CSP_RDO_BRANCH_LENGTH_LATTICE_SPACING_COEFF
};

struct T_SP16CombinedSectParams_D {
	int branch_connection_type = SP16CSP_RDO_BRANCH_CONN_PLANK;
	double plank_height = 100; // default in mm
	double plank_thickness = 6; // default in mm
	double lattice_crossbar_area = 6000; // default in mm
	double lattice_spacing = 2000; // default in mm

	int ly_branch_type = SP16CSP_RDO_BRANCH_LENGTH_LATTICE_SPACING_COEFF;
	double ly_lattice_spacing_coeff = 1;
	int lz_branch_type = SP16CSP_RDO_BRANCH_LENGTH_SAME;
	double lz_lattice_spacing_coeff = 1;
	void Initialize() { *this = {}; }
};

struct T_SP16CombinedSectParams_UDRD_D {
	T_SP16CombinedSectParams_K key = {};
	T_SP16CombinedSectParams_D data = {};
	static constexpr auto record_type_id = "S16C";
};


struct T_SP16CombinedSectParams_UNIT {
	int branch_connection_type = D_UNITSYS_NONE;
	int plank_height = D_UNITSYS_BASE_LENGTH;
	int plank_thickness = D_UNITSYS_BASE_LENGTH;
	int lattice_crossbar_area = D_UNITSYS_BASE_AREA;
	int lattice_spacing = D_UNITSYS_BASE_LENGTH;
	int ly_branch_type = D_UNITSYS_NONE;
	int ly_lattice_spacing_coeff = D_UNITSYS_NONE;
	int lz_branch_type = D_UNITSYS_NONE;
	int lz_lattice_spacing_coeff = D_UNITSYS_NONE;
};

#pragma pack (pop)
