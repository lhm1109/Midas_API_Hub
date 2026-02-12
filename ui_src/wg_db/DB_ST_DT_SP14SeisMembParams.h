
#pragma once
#define MIDAS_SDK

#pragma pack (push, 1)
// #pragma warning ( disable : 4819 )
// #pragma warning ( disable : 4244 )

#define T_SP14SeisMembParams_K T_ELEM_K
#define HASHSIZESP14SeisMembParams HASHSIZEELEM

struct T_SP14SeisMembParams_D {
	double m_tr_1_strength = 1.3;
	double m_tr_2_lesser_flexibility = 1.2;
	double m_tr_3_greater_flexibility = 1.0;
	double m_tr_4_stability = 1.0;
	void Initialize() { *this = {}; }
};

struct T_SP14SeisMembParams_UDRD_D {
	T_SP14SeisMembParams_K key = {};
	T_SP14SeisMembParams_D data = {};
	static constexpr auto record_type_id = "S14S";
};

#pragma pack (pop)
