
#pragma once
#define MIDAS_SDK

#pragma pack (push, 1)
// #pragma warning ( disable : 4819 )
// #pragma warning ( disable : 4244 )

#define T_SP16SafetyMembParams_K T_ELEM_K
#define HASHSIZESP16SafetyMembParams HASHSIZEELEM

struct T_SP16SafetyMembParams_D {
	double gamma_u = 1.3;
	double gamma_resp = 1.0;
	double gamma_c_str = 1.0;
	double gamma_c_stab = 1.0;
	void Initialize() { *this = {}; }
};

struct T_SP16SafetyMembParams_UDRD_D {
	T_SP16SafetyMembParams_K key = {};
	T_SP16SafetyMembParams_D data = {};
	static constexpr auto record_type_id = "S16F";
};

#pragma pack (pop)
