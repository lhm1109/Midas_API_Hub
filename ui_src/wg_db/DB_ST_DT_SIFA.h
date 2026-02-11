#ifndef __DB_ST_DT_SIFA_H__
#define __DB_ST_DT_SIFA_H__

#define MIDAS_SDK

#pragma pack(push, 1)

/**
* RC 교각의 수평변위 제한치
* */
struct T_SIFA_RC_Disp
{
	enum eRcDisp
	{
		kXi1 = 0,
		kXi2,
		kPhiS,
		kSize,
	};

	double Delta_yEd[T_SIFA_RC_Disp::kSize];	//T_SIFA_RC_Disp::eRcDisp
	double Delta_ls2d[T_SIFA_RC_Disp::kSize];	//T_SIFA_RC_Disp::eRcDisp
	double Delta_ls3d[T_SIFA_RC_Disp::kSize];	//T_SIFA_RC_Disp::eRcDisp
	double dFactorK;
	double dFactorK2;

	void Initialize()
	{
		Delta_yEd[T_SIFA_RC_Disp::kXi1] = 1.0;
		Delta_yEd[T_SIFA_RC_Disp::kXi2] = 0.0;
		Delta_yEd[T_SIFA_RC_Disp::kPhiS] = 1.0;
		Delta_ls2d[T_SIFA_RC_Disp::kXi1] = 1.0;
		Delta_ls2d[T_SIFA_RC_Disp::kXi2] = 0.0;
		Delta_ls2d[T_SIFA_RC_Disp::kPhiS] = 0.65;
		Delta_ls3d[T_SIFA_RC_Disp::kXi1] = 1.0;
		Delta_ls3d[T_SIFA_RC_Disp::kXi2] = 1.0;
		Delta_ls3d[T_SIFA_RC_Disp::kPhiS] = 0.65;
		dFactorK = 1.3;
		dFactorK2 = 1.0;
	}

	T_SIFA_RC_Disp() { Initialize(); }
	T_SIFA_RC_Disp(const T_SIFA_RC_Disp& src) { *this = src; }
	T_SIFA_RC_Disp& operator=(const T_SIFA_RC_Disp& src)
	{
		if (this != &src) {
			for (int i = 0; i < T_SIFA_RC_Disp::kSize; i++) Delta_yEd[i] = src.Delta_yEd[i];
			for (int i = 0; i < T_SIFA_RC_Disp::kSize; i++) Delta_ls2d[i] = src.Delta_ls2d[i];
			for (int i = 0; i < T_SIFA_RC_Disp::kSize; i++) Delta_ls3d[i] = src.Delta_ls3d[i];
			dFactorK = src.dFactorK;
			dFactorK2 = src.dFactorK2;
		}
		return *this;
	}
};

/**
* 강재교각의 수평변위 제한치
* */
struct T_SIFA_ST_Disp
{
	enum eStDisp
	{
		kXi1 = 0,
		kXi2,
		kPhiS,
		kSize,
	};

	double Delta_ls2d[T_SIFA_ST_Disp::kSize];	//T_SIFA_ST_Disp::eStPierFactor
	double dFactorK;

	void Initialize()
	{
		Delta_ls2d[T_SIFA_ST_Disp::kXi1] = 1.0;
		Delta_ls2d[T_SIFA_ST_Disp::kXi2] = 0.0;
		Delta_ls2d[T_SIFA_ST_Disp::kPhiS] = 0.75;
		dFactorK = 1.0;
	}

	T_SIFA_ST_Disp() { Initialize(); }
	T_SIFA_ST_Disp(const T_SIFA_ST_Disp& src) { *this = src; }
	T_SIFA_ST_Disp& operator=(const T_SIFA_ST_Disp& src)
	{
		if (this != &src) {
			for (int i = 0; i < T_SIFA_ST_Disp::kSize; i++) Delta_ls2d[i] = src.Delta_ls2d[i];
			dFactorK = src.dFactorK;
		}
		return *this;
	}
};

/**
* 콘크리트 부재의 곡률 제한치
* */
struct T_SIFA_RC_Curv
{
	enum eRcCurv
	{
		kXi1 = 0,
		kXi2,
		kPhiS,
		kK,
		kSize,
	};

	double Phi_ls2d[T_SIFA_RC_Curv::kSize];	//T_SIFA_RC_Curv::eRcCurvature
	double Phi_ls3d[T_SIFA_RC_Curv::kSize];	//T_SIFA_RC_Curv::eRcCurvature

	void Initialize()
	{
		Phi_ls2d[T_SIFA_RC_Curv::kXi1] = 1.0;
		Phi_ls2d[T_SIFA_RC_Curv::kXi2] = 0.0;
		Phi_ls2d[T_SIFA_RC_Curv::kPhiS] = 0.65;
		Phi_ls2d[T_SIFA_RC_Curv::kK] = 1.3;
		Phi_ls3d[T_SIFA_RC_Curv::kXi1] = 1.0;
		Phi_ls3d[T_SIFA_RC_Curv::kXi2] = 1.0;
		Phi_ls3d[T_SIFA_RC_Curv::kPhiS] = 0.65;
		Phi_ls3d[T_SIFA_RC_Curv::kK] = 1.3;
	}

	T_SIFA_RC_Curv() { Initialize(); }
	T_SIFA_RC_Curv(const T_SIFA_RC_Curv& src) { *this = src; }
	T_SIFA_RC_Curv& operator=(const T_SIFA_RC_Curv& src)
	{
		if (this != &src) {
			for (int i = 0; i < T_SIFA_RC_Curv::kSize; i++) Phi_ls2d[i] = src.Phi_ls2d[i];
			for (int i = 0; i < T_SIFA_RC_Curv::kSize; i++) Phi_ls3d[i] = src.Phi_ls3d[i];
		}
		return *this;
	}
};

/**
* 콘크리트 부재의 모멘트 제한치 (Myd＝ξ1・Φy・Myc)
* */
struct T_SIFA_RC_Myd
{
	enum eRcMyd
	{
		kXi1 = 0,
		kPhiY,
		kSize,
	};

	double No10[T_SIFA_RC_Myd::kSize];		//T_SIFA_RC_Myd::eRcMomentMyd
	double No11[T_SIFA_RC_Myd::kSize];		//T_SIFA_RC_Myd::eRcMomentMyd

	void Initialize()
	{
		No10[T_SIFA_RC_Myd::kXi1] = 0.9;
		No10[T_SIFA_RC_Myd::kPhiY] = 1.0;
		No11[T_SIFA_RC_Myd::kXi1] = 1.0;
		No11[T_SIFA_RC_Myd::kPhiY] = 1.0;
	}

	T_SIFA_RC_Myd() { Initialize(); }
	T_SIFA_RC_Myd(const T_SIFA_RC_Myd& src) { *this = src; }
	T_SIFA_RC_Myd& operator=(const T_SIFA_RC_Myd& src)
	{
		if (this != &src) {
			for (int i = 0; i < T_SIFA_RC_Myd::kSize; i++) No10[i] = src.No10[i];
			for (int i = 0; i < T_SIFA_RC_Myd::kSize; i++) No11[i] = src.No11[i];
		}
		return *this;
	}
};

/**
* 콘크리트 부재의 모멘트 제한치 (Mud＝ξ1・ξ2・Φu・Muc)
* */
struct T_SIFA_RC_Mud
{
	enum eRcMud
	{
		kXi1 = 0,
		kXi2,
		kPhiU,
		kSize,
	};

	double No10[T_SIFA_RC_Mud::kSize];
	double No11[T_SIFA_RC_Mud::kSize];

	void Initialize()
	{
		No10[T_SIFA_RC_Mud::kXi1] = 0.9;
		No10[T_SIFA_RC_Mud::kXi2] = 0.9;
		No10[T_SIFA_RC_Mud::kPhiU] = 1.0;
		No11[T_SIFA_RC_Mud::kXi1] = 1.0;
		No11[T_SIFA_RC_Mud::kXi2] = 0.9;
		No11[T_SIFA_RC_Mud::kPhiU] = 1.0;
	}

	T_SIFA_RC_Mud() { Initialize(); }
	T_SIFA_RC_Mud(const T_SIFA_RC_Mud& src) { *this = src; }
	T_SIFA_RC_Mud& operator=(const T_SIFA_RC_Mud& src)
	{
		if (this != &src) {
			for (int i = 0; i < T_SIFA_RC_Mud::kSize; i++) No10[i] = src.No10[i];
			for (int i = 0; i < T_SIFA_RC_Mud::kSize; i++) No11[i] = src.No11[i];
		}
		return *this;
	}
};

/**
* 콘크리트 부재의 전단력 제한치 (Susd＝ξ1・ξ2・(Φuc Sc+Φus Ss)+ξ1・ξ2・Φup・Sp)
* */
struct T_SIFA_RC_Susd
{
	enum eRcSusd
	{
		kXi1 = 0,
		kXi2,
		kPhi_uc_us,
		kXi1_p,
		kXi2_Phi_up,
		kK,
		kSize,
	};

	double No10[T_SIFA_RC_Susd::kSize];
	double No11[T_SIFA_RC_Susd::kSize];

	void Initialize()
	{
		No10[T_SIFA_RC_Susd::kXi1] = 0.9;
		No10[T_SIFA_RC_Susd::kXi2] = 0.85;
		No10[T_SIFA_RC_Susd::kPhi_uc_us] = 0.95;
		No10[T_SIFA_RC_Susd::kXi1_p] = 0.9;
		No10[T_SIFA_RC_Susd::kXi2_Phi_up] = 0.95;
		No10[T_SIFA_RC_Susd::kK] = 1.3;
		No11[T_SIFA_RC_Susd::kXi1] = 1.0;
		No11[T_SIFA_RC_Susd::kXi2] = 0.85;
		No11[T_SIFA_RC_Susd::kPhi_uc_us] = 0.95;
		No11[T_SIFA_RC_Susd::kXi1_p] = 1.0;
		No11[T_SIFA_RC_Susd::kXi2_Phi_up] = 0.95;
		No11[T_SIFA_RC_Susd::kK] = 1.3;
	}

	T_SIFA_RC_Susd() { Initialize(); }
	T_SIFA_RC_Susd(const T_SIFA_RC_Susd& src) { *this = src; }
	T_SIFA_RC_Susd& operator=(const T_SIFA_RC_Susd& src)
	{
		if (this != &src) {
			for (int i = 0; i < T_SIFA_RC_Susd::kSize; i++) No10[i] = src.No10[i];
			for (int i = 0; i < T_SIFA_RC_Susd::kSize; i++) No11[i] = src.No11[i];
		}
		return *this;
	}
};

/**
* 강재교각의 수평변위 제한치 (Sucd＝ξ1・ξ2・Φucw・Sucw)+ξ1・ξ2・Φup・Sp)
* */
struct T_SIFA_RC_Sucd
{
	enum eRcSucd
	{
		kXi1 = 0,
		kXi2_Phi_ucw,
		kSize,
	};

	double No10[T_SIFA_RC_Sucd::kSize];
	double No11[T_SIFA_RC_Sucd::kSize];

	void Initialize()
	{
		No10[T_SIFA_RC_Sucd::kXi1] = 0.9;
		No10[T_SIFA_RC_Sucd::kXi2_Phi_ucw] = 1.0;
		No11[T_SIFA_RC_Sucd::kXi1] = 1.0;
		No11[T_SIFA_RC_Sucd::kXi2_Phi_ucw] = 1.0;
	}

	T_SIFA_RC_Sucd() { Initialize(); }
	T_SIFA_RC_Sucd(const T_SIFA_RC_Sucd& src) { *this = src; }
	T_SIFA_RC_Sucd& operator=(const T_SIFA_RC_Sucd& src)
	{
		if (this != &src) {
			for (int i = 0; i < T_SIFA_RC_Sucd::kSize; i++) No10[i] = src.No10[i];
			for (int i = 0; i < T_SIFA_RC_Sucd::kSize; i++) No11[i] = src.No11[i];
		}
		return *this;
	}
};

/**
* Seismic (Partial) Factor (저항측 부분계수) - H29년용
*/
#define T_SIFA_H29_K unsigned int
struct T_SIFA_H29_D
{
	//교각의 수평변위 제한치
	T_SIFA_RC_Disp RcPierVal;
	T_SIFA_ST_Disp StPierVal;
	//콘크리트 부재의 곡률 제한치
	T_SIFA_RC_Curv RcCurvature;
	//콘크리트 부재의 모멘트 제한치
	T_SIFA_RC_Myd RcMomentMyd;	// Myd＝ξ1・Φy・Myc
	T_SIFA_RC_Mud RcMomentMud;	// Mud＝ξ1・ξ2・Φu・Muc
	//콘크리트 부재의 전단력 제한치
	T_SIFA_RC_Susd RcShearSusd;	// Susd＝ξ1・ξ2・(Φuc Sc+Φus Ss)+ξ1・ξ2・Φup・Sp
	T_SIFA_RC_Sucd RcShearSucd;	// Sucd＝ξ1・ξ2・Φucw・Sucw)+ξ1・ξ2・Φup・Sp

	void Initialize()
	{
		RcPierVal.Initialize();
		StPierVal.Initialize();
		RcCurvature.Initialize();
		RcMomentMyd.Initialize();
		RcMomentMud.Initialize();
		RcShearSusd.Initialize();
		RcShearSucd.Initialize();
	}
	T_SIFA_H29_D() { Initialize(); }
	T_SIFA_H29_D(const T_SIFA_H29_D& src) { *this = src; }
	T_SIFA_H29_D& operator=(const T_SIFA_H29_D& src)
	{
		if (this != &src) {
			RcPierVal = src.RcPierVal;
			StPierVal = src.StPierVal;
			RcCurvature = src.RcCurvature;
			RcMomentMyd = src.RcMomentMyd;
			RcMomentMud = src.RcMomentMud;
			RcShearSusd = src.RcShearSusd;
			RcShearSucd = src.RcShearSucd;
		}
		return *this;
	}

	// nEarthquakeType : enum T_SIPA_D::eEarthquakeType
	double* Myd(const int& nEarthquakeType)
	{
		if (nEarthquakeType == T_SIPA_D::kFluctuating_No10)
			return RcMomentMyd.No10;
		else //if (nEarthquakeType == T_SIPA_D::kAccidental_No11)
			return RcMomentMyd.No11;
	}

	// nEarthquakeType : enum T_SIPA_D::eEarthquakeType
	double* Mud(const int& nEarthquakeType)
	{
		if (nEarthquakeType == T_SIPA_D::kFluctuating_No10)
			return RcMomentMud.No10;
		else //if (nEarthquakeType == T_SIPA_D::kAccidental_No11)
			return RcMomentMud.No11;
	}

	// nEarthquakeType : enum T_SIPA_D::eEarthquakeType
	double* Susd(const int& nEarthquakeType)
	{
		if (nEarthquakeType == T_SIPA_D::kFluctuating_No10)
			return RcShearSusd.No10;
		else //if (nEarthquakeType == T_SIPA_D::kAccidental_No11)
			return RcShearSusd.No11;
	}

	// nEarthquakeType : enum T_SIPA_D::eEarthquakeType
	double* Sucd(const int& nEarthquakeType)
	{
		if (nEarthquakeType == T_SIPA_D::kFluctuating_No10)
			return RcShearSucd.No10;
		else //if (nEarthquakeType == T_SIPA_D::kAccidental_No11)
			return RcShearSucd.No11;
	}
};

struct T_SIFA_H29_UNIT
{
	T_SIFA_H29_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

struct T_SIFA_H29_UDRD_D
{
	T_SIFA_H29_K key;
	T_SIFA_H29_D data;
};

#define HASHSIZESIFA_H29 1001
#define HASHSIZELISTSIFA_H29 11

#pragma pack(pop)

#endif  // __DB_ST_DT_SIFA_H__