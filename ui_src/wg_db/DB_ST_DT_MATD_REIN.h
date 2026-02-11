#ifndef __DB_ST_DT_MATD_REIN_H__
#define __DB_ST_DT_MATD_REIN_H__

#define MIDAS_SDK

#pragma pack(push, 1)

struct T_MATD_REIN_RC
{
	CString strC_CodeName;			// Concrete 규격이름
	CString strC_CodeMatlName;		// Concrete 재질이름
	double dC_Fc;				// Concrete 압축강도 T_MATD_D.Data1.Design.C_fc
	double dC_Elast;				// Concrete 탄성 계수( elastic modulus(Ec) ) T_MATD_D.Data1.Analysis.Elast

	CString strR_CodeName;			// Rebar 규격이름
	CString strMR_RebarName;		// 주철근 재질이름
	CString strSR_RebarName;		// 띠철근 재질이름
	double dMR_Fy;				// 주철근 항복강도 T_MATD_D.MainRebarData.B_fy
	double dSR_Fy;				// 띠철근 항복강도 T_MATD_D.SubRebarData.B_fy

	T_MATD_REIN_RC() { }
	T_MATD_REIN_RC(const T_MATD_REIN_RC& src) { *this = src; }
	T_MATD_REIN_RC& operator=(const T_MATD_REIN_RC& src)
	{
		if (this != &src) {
			strC_CodeName = src.strC_CodeName;
			strC_CodeMatlName = src.strC_CodeMatlName;
			dC_Fc = src.dC_Fc;
			dC_Elast = src.dC_Elast;
			strR_CodeName = src.strR_CodeName;
			strMR_RebarName = src.strMR_RebarName;
			strSR_RebarName = src.strSR_RebarName;
			dMR_Fy = src.dMR_Fy;
			dSR_Fy = src.dSR_Fy;
		}
		return *this;
	}

	void Initialize()
	{
		strC_CodeName = _T("");
		strC_CodeMatlName = _T("");
		dC_Fc = 0;
		dC_Elast = 0;
		strR_CodeName = _T("");
		strMR_RebarName = _T("");
		strSR_RebarName = _T("");
		dMR_Fy = 0;
		dSR_Fy = 0;
	}
};

struct T_MATD_REIN_RC_CH
{
	char strC_CodeName[16];
	char strC_CodeMatlName[16];
	double dC_Fc;
	double dC_Elast;

	char strR_CodeName[16];
	char strMR_RebarName[16];
	char strSR_RebarName[16];
	double dMR_Fy;
	double dSR_Fy;

	void Get(T_MATD_REIN_RC& rData)
	{
		ConvertCharStr(strC_CodeName, rData.strC_CodeName, sizeof(strC_CodeName));
		ConvertCharStr(strC_CodeMatlName, rData.strC_CodeMatlName, sizeof(strC_CodeMatlName));
		rData.dC_Fc = dC_Fc;
		rData.dC_Elast = dC_Elast;
		ConvertCharStr(strR_CodeName, rData.strR_CodeName, sizeof(strR_CodeName));
		ConvertCharStr(strMR_RebarName, rData.strMR_RebarName, sizeof(strMR_RebarName));
		ConvertCharStr(strSR_RebarName, rData.strSR_RebarName, sizeof(strSR_RebarName));
		rData.dMR_Fy = dMR_Fy;
		rData.dSR_Fy = dSR_Fy;
	}

	void Set(/*const*/ T_MATD_REIN_RC& data)
	{
		ConvertStrChar(data.strC_CodeName, strC_CodeName, sizeof(strC_CodeName));
		ConvertStrChar(data.strC_CodeMatlName, strC_CodeMatlName, sizeof(strC_CodeMatlName));
		dC_Fc = data.dC_Fc;
		dC_Elast = data.dC_Elast;
		ConvertStrChar(data.strR_CodeName, strR_CodeName, sizeof(strR_CodeName));
		ConvertStrChar(data.strMR_RebarName, strMR_RebarName, sizeof(strMR_RebarName));
		ConvertStrChar(data.strSR_RebarName, strSR_RebarName, sizeof(strSR_RebarName));
		dMR_Fy = data.dMR_Fy;
		dSR_Fy = data.dSR_Fy;
	}
};

struct T_MATD_REIN_SP
{
	CString strCodeName;		// 규격이름
	CString strCodeMatlName;	// 재질이름
	double dFy;				// 항복강도 T_MATD_D.Data1.Design.S_Fy

	CString strR_CodeName;			// Rebar 규격이름
	CString strAnchor_RebarName;		// 앵커 철근 재질이름
	double dAnchor_Fy;				// 앵커 철근 항복강도 T_MATD_D.MainRebarData.B_fy

	T_MATD_REIN_SP() {}
	T_MATD_REIN_SP(const T_MATD_REIN_SP& src) { *this = src; }
	T_MATD_REIN_SP& operator=(const T_MATD_REIN_SP& src)
	{
		if (this != &src) {
			strCodeName = src.strCodeName;
			strCodeMatlName = src.strCodeMatlName;
			dFy = src.dFy;
			strR_CodeName = src.strR_CodeName;
			strAnchor_RebarName = src.strAnchor_RebarName;
			dAnchor_Fy = src.dAnchor_Fy;
		}
		return *this;
	}

	void Initialize()
	{
		strCodeName = _T("");
		strCodeMatlName = _T("");
		dFy = 0;
	}
};

struct T_MATD_REIN_SP_CH
{
	char strCodeName[16];
	char strCodeMatlName[16];
	double dFy;
	char strR_CodeName[16];
	char strAnchor_RebarName[16];
	double dAnchor_Fy;

	void Get(T_MATD_REIN_SP& rData)
	{
		ConvertCharStr(strCodeName, rData.strCodeName, sizeof(strCodeName));
		ConvertCharStr(strCodeMatlName, rData.strCodeMatlName, sizeof(strCodeMatlName));
		rData.dFy = dFy;
		ConvertCharStr(strR_CodeName, rData.strR_CodeName, sizeof(strR_CodeName));
		ConvertCharStr(strAnchor_RebarName, rData.strAnchor_RebarName, sizeof(strAnchor_RebarName));
		rData.dAnchor_Fy = dAnchor_Fy;
	}

	void Set(/*const*/ T_MATD_REIN_SP& data)
	{
		ConvertStrChar(data.strCodeName, strCodeName, sizeof(strCodeName));
		ConvertStrChar(data.strCodeMatlName, strCodeMatlName, sizeof(strCodeMatlName));
		dFy = data.dFy;
		ConvertStrChar(data.strR_CodeName, strR_CodeName, sizeof(strR_CodeName));
		ConvertStrChar(data.strAnchor_RebarName, strAnchor_RebarName, sizeof(strAnchor_RebarName));
		dAnchor_Fy = data.dAnchor_Fy;
	}
};

struct T_FILL_MATL_SP
{
	double dEc;
	double dSigmaCk;
	double dThickness;
	double dWeight;

	T_FILL_MATL_SP() {}
	T_FILL_MATL_SP(const T_FILL_MATL_SP& src) { *this = src; }
	T_FILL_MATL_SP& operator=(const T_FILL_MATL_SP& src)
	{
		if (this != &src) {
			dEc = src.dEc;
			dSigmaCk = src.dSigmaCk;
			dThickness = src.dThickness;
			dWeight = src.dWeight;
		}
		return *this;
	}

	void Initialize()
	{
		dEc = 0;
		dSigmaCk = 0;
		dThickness = 0;
		dWeight = 0;
	}
};

struct T_MATD_REIN_CF
{
	int nMainType;		// 시트타입
	int nSubType;		// 시트종류
	int nVolume;			// 목부량

	double dThickness;	// 두께
	double dElast;		// 탄성 계수
	double dFu;			// 인장강도

	T_MATD_REIN_CF() { }
	T_MATD_REIN_CF(const T_MATD_REIN_CF& src) { *this = src; }
	T_MATD_REIN_CF& operator=(const T_MATD_REIN_CF& src)
	{
		if (this != &src) {
			nMainType = src.nMainType;
			nSubType = src.nSubType;
			nVolume = src.nVolume;
			dThickness = src.dThickness;
			dElast = src.dElast;
			dFu = src.dFu;
		}
		return *this;
	}

	void Initialize()
	{
		nMainType = eMainType::kUnKnownMain;
		nSubType = eSubType::kUnKnownSub;
		nVolume = eVolume::kUnKnown;
		dThickness = 0;
		dElast = 0;
		dFu = 0;
	}

	enum eMainType
	{
		kUnKnownMain,
		kUserInput,		//직접입력
		kCarbonFiber,	//탄소섬유시트
		kAFRP,			//AFRP시트
	};

	enum eSubType
	{
		kUnKnownSub,
		kHighStrengthType1,		//고강도형1
		kHighStrengthType2,		//고강도형2
		kMediumElasticityType,	//중탄성형
		kHighElasticityType,	//고탄성형
		kAramid1,
		kAramid2_E78400,
		kAramid2_E78000,
	};

	enum eVolume
	{
		kUnKnown,
		k200,
		k235,
		k280,
		k300,
		k300_1,
		k300_2,
		k350,
		k400,
		k415,
		k450,
		k525,
		k600,
		k623,
		k700,
		k830,
	};
};

// Material Design - Reinforcement
#define T_MATD_REIN_K unsigned int
struct T_MATD_REIN_D
{
	T_MATL_K kMatl;
	T_MATD_REIN_RC CoverRC; //철근콘크리트(Rebar Concrete) 피복
	T_MATD_REIN_SP CoverSP; //강판(Steel Plate) 피복
	T_MATD_REIN_CF CoverCF; //탄소섬유(Carbon fiber) 피복
	T_FILL_MATL_SP CoverFM; //충진재료 - 강판피복 일때

	T_MATD_REIN_D() { }
	T_MATD_REIN_D(const T_MATD_REIN_D& src) { *this = src; }
	T_MATD_REIN_D& operator=(const T_MATD_REIN_D& src)
	{
		if (this != &src) {
			kMatl = src.kMatl;
			CoverRC = src.CoverRC;
			CoverSP = src.CoverSP;
			CoverCF = src.CoverCF;
			CoverFM = src.CoverFM;
		}
		return *this;
	}

	void Initialize()
	{
		kMatl = 0;
		CoverRC.Initialize();
		CoverSP.Initialize();
		CoverCF.Initialize();
		CoverFM.Initialize();
	}
};

struct T_MATD_REIN_UNIT
{
	T_MATD_REIN_UNIT()
	{
		Fc = D_UNITSYS_BASE_STRESS;
		Fy = D_UNITSYS_BASE_STRESS;
		Fu = D_UNITSYS_BASE_STRESS;
		Elast = D_UNITSYS_BASE_ELAST;
		Thickness = D_UNITSYS_BASE_LENGTH;
		Ec = D_UNITSYS_BASE_ELAST;
		Ck = D_UNITSYS_BASE_STRESS;
		Weight = D_UNITSYS_BASE_WEIGHT;
	}
	int Fc;
	int Fy;
	int Fu;
	int Elast;
	int Thickness;
	int Ec;
	int Ck;
	int Weight;
};

struct T_MATD_REIN_UDRD_D
{
	T_MATD_REIN_K key;
	T_MATD_REIN_D data;
};

struct T_MATD_REIN_D_CH
{
	T_MATL_K kMatl;
	T_MATD_REIN_RC_CH CoverRC;
	T_MATD_REIN_SP_CH CoverSP;
	T_MATD_REIN_CF	  CoverCF;
	T_FILL_MATL_SP	  CoverFM;

	void Get(T_MATD_REIN_D& rData)
	{
		rData.kMatl = kMatl;
		CoverRC.Get(rData.CoverRC);
		CoverSP.Get(rData.CoverSP);
		rData.CoverCF = CoverCF;
		rData.CoverFM = CoverFM;
	}

	void Set(/*const*/ T_MATD_REIN_D& data)
	{
		kMatl = data.kMatl;
		CoverRC.Set(data.CoverRC);
		CoverSP.Set(data.CoverSP);
		CoverCF = data.CoverCF;
		CoverFM = data.CoverFM;
	}
};

#define HASHSIZEMATDREIN HASHSIZEMATL
#pragma pack(pop)

#endif  // __DB_ST_DT_MATD_REIN_H__