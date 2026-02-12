#ifndef __DB_ST_DT_DMGB_H__
#define __DB_ST_DT_DMGB_H__

#define MIDAS_SDK

#pragma pack(push, 1)
#pragma warning ( disable : 4819 )
#pragma warning ( disable : 4244 )

#define T_DMGB_K unsigned int

struct T_DMGB_BRB
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	int nDof;
	double dFy;
	double dInitStiff;
	double dDisp;
	double dStiffRot;
	int nCoreMtl;
	double dLength;
	void Initialize()
	{
		nInputMethod = 1;
		nCode = 0;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		nDof = 0;
		dFy = 500*1.0e3;
		dDisp = 4.8*1.0e-3;
		dInitStiff = 500/4.8*1.0e6;
		dStiffRot = 0.035;
		nCoreMtl = 0;
		dLength = 3500 * 1.0e-3;
	}
};

struct T_DMGB_MYD
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	int nDof;
	double dFy;
	double dDisp;
	double dInitStiff;
	double dStiffRot;
	int nCoreMtl;
	void Initialize()
	{
		nInputMethod = 1;
		nCode = 0;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		nDof = 0;
		dFy = 500 * 1.0e3;
		dDisp = 4.8*1.0e-3;
		dInitStiff = 500 / 4.8*1.0e6;
		dStiffRot = 0.035;
		nCoreMtl = 0;
	}
};

struct T_DMGB_FD
{
	int nInputMethod;
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	int nDof;
	double dForce;
	double dDisp;
	double dInitStiff;
	void Initialize()
	{
		nInputMethod = 0;
		nCode = 0;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		nDof = 0;
		dForce = 100 * 1.0e3;
		dDisp = 0.5*1.0e-3;
		dInitStiff = 100/0.5*1.0e6;
	}
};

struct T_DMGB_VFD
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	int nDof;
	int nDampType;
	double dDamperF;
	double dIndex;
	double dSpringStiff;
	double dDamperStiff;
	double dDesignF;
	double dDisp;

	void Initialize()
	{
		nInputMethod = 1;
		nCode = 0;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		nDof = 0;
		nDampType = 0;
		dDamperF = 100 * 1.0e3;
		dIndex = 0.2;
		dSpringStiff = 0;
		dDamperStiff = 0;
		dDesignF = 200 * 1.0e3;
		dDisp = 180 * 1.0e-3;
	}
};

struct T_DMGB_HDRD
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	int nDof;
	double dInitStiff;
	double dFy;
	double dYieldStiff;
	double dYieldStiffRot;
	double dEquiStiff;
	double dEquiDampRto;

	void Initialize()
	{
		nInputMethod = 1;
		nCode = 0;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		nDof = 0;
		dInitStiff = 80 * 1.0e6;
		dFy = 70 * 1.0e3;
		dYieldStiff = 8 * 1.0e6;
		dYieldStiffRot = 0.1;
		dEquiStiff = 12.5*1.0e6;
		dEquiDampRto = 18;
	}
};

struct T_DMGB_VED
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	int nDof;
	int nDampType;
	double dDamperF;
	double dIndex;
	double dSpringStiff; 
	double dDamperStiff;
	double dDesignF;
	double dEffectStiff;

	void Initialize()
	{
		nInputMethod = 1;
		nCode = 0;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		nDof = 0;
		nDampType = 1;
		dDamperF = 200 * 1.0e3;
		dIndex = 0.2;
		dSpringStiff = 10 * 1.0e6;
		dDamperStiff = 110 * 1.0e6;
		dDesignF = 200 * 1.0e3;
		dEffectStiff = 10 * 1.0e6;
	}
};

#define D_DMGB_DEV_BRB 0 // BRB
#define D_DMGB_DEV_MYD 1 // MYD
#define D_DMGB_DEV_FD  2 // FD
#define D_DMGB_DEV_VFD  3 // VFD
#define D_DMGB_DEV_HDRD 4 // FPS
#define D_DMGB_DEV_VED 5 // VED

struct T_DMGB_D
{
	CString		PropName;
	CString		Description;
	int         nPropType;    // 0:BRB, 1:MYD, 2:FD, 3:VFD, 4:HDRD 5:VED
	unsigned int nSeq;    // 鉴锅
	
	T_DMGB_BRB BRB;
	T_DMGB_MYD MYD;
	T_DMGB_FD FD;
	T_DMGB_VFD VFD;
	T_DMGB_HDRD HDRD;
	T_DMGB_VED VED;
	UINT        ElemKey4D2E;
	void Initialize()
	{
		PropName.Empty();
		Description.Empty();
		nPropType = 0;
		nSeq = 0;
		ElemKey4D2E = 0;
		BRB.Initialize();
		MYD.Initialize();
		FD.Initialize();
		VFD.Initialize();
		HDRD.Initialize();
		VED.Initialize();
	}
	T_DMGB_D() { Initialize(); }
	T_DMGB_D(const T_DMGB_D& src) { T_DMGB_D(); *this = src; }
	T_DMGB_D& T_DMGB_D::operator=(const T_DMGB_D &src)
	{
		PropName = src.PropName;
		Description = src.Description;
		nPropType = src.nPropType;
		nSeq = src.nSeq;
		BRB = src.BRB;
		MYD = src.MYD;
		FD = src.FD;
		VFD = src.VFD;
		HDRD = src.HDRD;
		VED = src.VED;
		ElemKey4D2E = src.ElemKey4D2E;

		return *this;
	}
};

struct T_DMGB_BRB_CH
{
	int nInputMethod;
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	int nDof;
	double dFy;
	double dDisp;
	double dInitStiff;
	double dStiffRot; 
	int nCoreMtl;
	double dLength;
};
struct T_DMGB_MYD_CH
{
	int nInputMethod;
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	int nDof;
	double dFy;
	double dDisp;
	double dInitStiff;
	double dStiffRot;
	int nCoreMtl;
};
struct T_DMGB_FD_CH
{
	int nInputMethod;
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	int nDof;
	double dForce;
	double dDisp;
	double dInitStiff;
};
struct T_DMGB_VFD_CH
{
	int nInputMethod;
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	int nDof;
	int nDampType;
	double dDamperF;
	double dIndex;
	double dSpringStiff;
	double dDamperStiff;
	double dDesignF;
	double dDisp;
};
struct T_DMGB_HDRD_CH
{
	int nInputMethod;
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	int nDof;
	double dInitStiff;
	double dFy;
	double dYieldStiff;
	double dYieldStiffRot;
	double dEquiStiff;
	double dEquiDampRto;
};
struct T_DMGB_VED_CH
{
	int nInputMethod;
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	int nDof;
	int nDampType;
	double dDamperF;
	double dIndex;
	double dSpringStiff;
	double dDamperStiff;
	double dDesignF;
	double dEffectStiff;
};
struct T_DMGB_D_CH
{
	char		PropName[40];
	char		Description[40];
	int         nPropType;    // 0:BRB, 1:MYD, 2:FD, 3:VFD, 4:Lead HDRD
	unsigned int nSeq;    // 鉴锅

	T_DMGB_BRB_CH BRB;
	T_DMGB_MYD_CH MYD;
	T_DMGB_FD_CH FD;
	T_DMGB_VFD_CH VFD;
	T_DMGB_HDRD_CH HDRD;
	T_DMGB_VED_CH VED;
	UINT        ElemKey4D2E;
	void ConvertToString(T_DMGB_D & rData)
	{
		ConvertCharStr(PropName, rData.PropName, sizeof(PropName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nPropType = nPropType;
		rData.nSeq = nSeq;
		rData.ElemKey4D2E = ElemKey4D2E;

		ConvertCharStr(BRB.strClassify, rData.BRB.strClassify, sizeof(BRB.strClassify));
		ConvertCharStr(BRB.strInputType, rData.BRB.strInputType, sizeof(BRB.strInputType));
		rData.BRB.nInputMethod = BRB.nInputMethod;
		rData.BRB.nCode = BRB.nCode;
		rData.BRB.nClassify = BRB.nClassify;
		rData.BRB.nInputType = BRB.nInputType;
		rData.BRB.nDof = BRB.nDof;
		rData.BRB.dFy = BRB.dFy;
		rData.BRB.dDisp = BRB.dDisp; 
		rData.BRB.dInitStiff = BRB.dInitStiff;
		rData.BRB.dStiffRot = BRB.dStiffRot;
		rData.BRB.nCoreMtl = BRB.nCoreMtl;
		rData.BRB.dLength = BRB.dLength;

		ConvertCharStr(MYD.strClassify, rData.MYD.strClassify, sizeof(MYD.strClassify));
		ConvertCharStr(MYD.strInputType, rData.MYD.strInputType, sizeof(MYD.strInputType));
		rData.MYD.nInputMethod = MYD.nInputMethod;
		rData.MYD.nCode= MYD.nCode;
		rData.MYD.nClassify= MYD.nClassify;
		rData.MYD.nInputType= MYD.nInputType;
		rData.MYD.nDof = MYD.nDof;
		rData.MYD.dFy = MYD.dFy;
		rData.MYD.dDisp = MYD.dDisp;
		rData.MYD.dInitStiff = MYD.dInitStiff;
		rData.MYD.dStiffRot = MYD.dStiffRot;
		rData.MYD.nCoreMtl = MYD.nCoreMtl;

		ConvertCharStr(FD.strClassify, rData.FD.strClassify, sizeof(FD.strClassify));
		ConvertCharStr(FD.strInputType, rData.FD.strInputType, sizeof(FD.strInputType));
		rData.FD.nInputMethod = FD.nInputMethod; //0:User, 1:Code 
		rData.FD.nCode= FD.nCode;
		rData.FD.nClassify= FD.nClassify;
		rData.FD.nInputType= FD.nInputType;
		rData.FD.nDof = FD.nDof;
		rData.FD.dForce = FD.dForce;
		rData.FD.dDisp = FD.dDisp; //Kvc
		rData.FD.dInitStiff = FD.dInitStiff;

		ConvertCharStr(VFD.strClassify, rData.VFD.strClassify, sizeof(VFD.strClassify));
		ConvertCharStr(VFD.strInputType, rData.VFD.strInputType, sizeof(VFD.strInputType));
		rData.VFD.nInputMethod= VFD.nInputMethod; //0:User, 1:Code 
		rData.VFD.nCode= VFD.nCode;
		rData.VFD.nClassify= VFD.nClassify;
		rData.VFD.nInputType= VFD.nInputType;
		rData.VFD.nDof = VFD.nDof;
		rData.VFD.nDampType = VFD.nDampType;
		rData.VFD.dDamperF = VFD.dDamperF; 
		rData.VFD.dIndex = VFD.dIndex;
		rData.VFD.dSpringStiff = VFD.dSpringStiff;
		rData.VFD.dDamperStiff = VFD.dDamperStiff;
		rData.VFD.dDesignF = VFD.dDesignF;
		rData.VFD.dDisp = VFD.dDisp;

		ConvertCharStr(HDRD.strClassify, rData.HDRD.strClassify, sizeof(HDRD.strClassify));
		ConvertCharStr(HDRD.strInputType, rData.HDRD.strInputType, sizeof(HDRD.strInputType));
		rData.HDRD.nInputMethod= HDRD.nInputMethod; //0:User, 1:Code 
		rData.HDRD.nCode = HDRD.nCode;
		rData.HDRD.nClassify = HDRD.nClassify;
		rData.HDRD.nInputType = HDRD.nInputType;
		rData.HDRD.nDof = HDRD.nDof;
		rData.HDRD.dInitStiff = HDRD.dInitStiff;
		rData.HDRD.dFy = HDRD.dFy;
		rData.HDRD.dYieldStiff = HDRD.dYieldStiff;
		rData.HDRD.dYieldStiffRot = HDRD.dYieldStiffRot;
		rData.HDRD.dEquiStiff = HDRD.dEquiStiff;
		rData.HDRD.dEquiDampRto = HDRD.dEquiDampRto;
	
		ConvertCharStr(VED.strClassify, rData.VED.strClassify, sizeof(VED.strClassify));
		ConvertCharStr(VED.strInputType, rData.VED.strInputType, sizeof(VED.strInputType));
		rData.VED.nInputMethod = VED.nInputMethod; //0:User, 1:Code 
		rData.VED.nCode = VED.nCode;
		rData.VED.nClassify = VED.nClassify;
		rData.VED.nInputType = VED.nInputType;
		rData.VED.nDof = VED.nDof;
		rData.VED.nDampType = VED.nDampType;
		rData.VED.dDamperF = VED.dDamperF;
		rData.VED.dIndex = VED.dIndex;
		rData.VED.dSpringStiff = VED.dSpringStiff;
		rData.VED.dDamperStiff = VED.dDamperStiff;
		rData.VED.dDesignF = VED.dDesignF;
		rData.VED.dEffectStiff = VED.dEffectStiff;

	}
	void ConvertToChar(T_DMGB_D& rData)
	{
		ConvertStrChar(rData.PropName, PropName, sizeof(PropName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nPropType = rData.nPropType;
		nSeq = rData.nSeq;
		ElemKey4D2E = rData.ElemKey4D2E;

		ConvertStrChar(rData.BRB.strClassify, BRB.strClassify, sizeof(BRB.strClassify));
		ConvertStrChar(rData.BRB.strInputType, BRB.strInputType, sizeof(BRB.strInputType));
		BRB.nInputMethod = rData.BRB.nInputMethod;
		BRB.nCode = rData.BRB.nCode;
		BRB.nClassify = rData.BRB.nClassify;
		BRB.nInputType = rData.BRB.nInputType;
		BRB.nDof = rData.BRB.nDof;
		BRB.dFy = rData.BRB.dFy;
		BRB.dInitStiff = rData.BRB.dInitStiff;
		BRB.dDisp = rData.BRB.dDisp;
		BRB.dStiffRot = rData.BRB.dStiffRot;
		BRB.nCoreMtl = rData.BRB.nCoreMtl;
		BRB.dLength = rData.BRB.dLength;

		ConvertStrChar(rData.MYD.strClassify, MYD.strClassify, sizeof(MYD.strClassify));
		ConvertStrChar(rData.MYD.strInputType, MYD.strInputType, sizeof(MYD.strInputType));
		MYD.nInputMethod = rData.MYD.nInputMethod;
		MYD.nCode = rData.MYD.nCode;
		MYD.nClassify = rData.MYD.nClassify;
		MYD.nInputType = rData.MYD.nInputType;
		MYD.nDof = rData.MYD.nDof;
		MYD.dFy = rData.MYD.dFy;
		MYD.dDisp = rData.MYD.dDisp;
		MYD.dInitStiff = rData.MYD.dInitStiff;
		MYD.dStiffRot = rData.MYD.dStiffRot;
		MYD.nCoreMtl = rData.MYD.nCoreMtl;

		ConvertStrChar(rData.FD.strInputType, FD.strInputType, sizeof(FD.strInputType));
		ConvertStrChar(rData.FD.strClassify, FD.strClassify, sizeof(FD.strClassify));
		FD.nInputMethod = rData.FD.nInputMethod; //0:User, 1:Code 
		FD.nCode = rData.FD.nCode;
		FD.nClassify = rData.FD.nClassify;
		FD.nInputType = rData.FD.nInputType;
		FD.nDof = rData.FD.nDof;
		FD.dForce = rData.FD.dForce;
		FD.dDisp = rData.FD.dDisp;
		FD.dInitStiff = rData.FD.dInitStiff;


		ConvertStrChar(rData.VFD.strClassify, VFD.strClassify, sizeof(VFD.strClassify));
		ConvertStrChar(rData.VFD.strInputType, VFD.strInputType, sizeof(VFD.strInputType));
		VFD.nInputMethod = rData.VFD.nInputMethod; //0:User, 1:Code 
		VFD.nCode = rData.VFD.nCode;
		VFD.nClassify = rData.VFD.nClassify;
		VFD.nInputType = rData.VFD.nInputType;
		VFD.nDof = rData.VFD.nDof;
		VFD.nDampType = rData.VFD.nDampType;
		VFD.dDamperF = rData.VFD.dDamperF; //Kvc
		VFD.dIndex = rData.VFD.dIndex;
		VFD.dSpringStiff = rData.VFD.dSpringStiff;
		VFD.dDamperStiff = rData.VFD.dDamperStiff;
		VFD.dDesignF = rData.VFD.dDesignF;
		VFD.dDisp = rData.VFD.dDisp;


		ConvertStrChar(rData.HDRD.strClassify, HDRD.strClassify, sizeof(HDRD.strClassify));
		ConvertStrChar(rData.HDRD.strInputType, HDRD.strInputType, sizeof(HDRD.strInputType));
		HDRD.nInputMethod = rData.HDRD.nInputMethod; //0:User, 1:Code 
		HDRD.nCode = rData.HDRD.nCode;
		HDRD.nClassify = rData.HDRD.nClassify;
		HDRD.nInputType = rData.HDRD.nInputType;
		HDRD.nDof = rData.HDRD.nDof;
		HDRD.dInitStiff = rData.HDRD.dInitStiff;
		HDRD.dFy = rData.HDRD.dFy;
		HDRD.dYieldStiff = rData.HDRD.dYieldStiff;
		HDRD.dYieldStiffRot = rData.HDRD.dYieldStiffRot;
		HDRD.dEquiStiff = rData.HDRD.dEquiStiff;
		HDRD.dEquiDampRto = rData.HDRD.dEquiDampRto;


		ConvertStrChar(rData.VED.strClassify, VED.strClassify, sizeof(VED.strClassify));
		ConvertStrChar(rData.VED.strInputType, VED.strInputType, sizeof(VED.strInputType));
		VED.nInputMethod = rData.VED.nInputMethod; //0:User, 1:Code 
		VED.nCode = rData.VED.nCode;
		VED.nClassify = rData.VED.nClassify;
		VED.nInputType = rData.VED.nInputType;
		VED.nDof = rData.VED.nDof;
		VED.nDampType = rData.VED.nDampType;
		VED.dDamperF = rData.VED.dDamperF; 
		VED.dIndex = rData.VED.dIndex;
		VED.dSpringStiff = rData.VED.dSpringStiff;
		VED.dDamperStiff = rData.VED.dDamperStiff;
		VED.dDesignF = rData.VED.dDesignF;
		VED.dEffectStiff = rData.VED.dEffectStiff;
	}
};

struct T_DMGB_UDRD_D
{
	T_DMGB_K key;
	T_DMGB_D data;
};
struct T_DMGB_UDRD_D_CH
{
	T_DMGB_K key;
	T_DMGB_D_CH data;
};

struct T_DMGB_UNIT
{
	T_DMGB_UNIT()
	{
		dFy = D_UNITSYS_BASE_FORCE;
		dDisp = D_UNITSYS_BASE_LENGTH;
		dInitStiff = D_UNITSYS_BASE_SPRING;
		dLength = D_UNITSYS_BASE_LENGTH;
		dForce = D_UNITSYS_BASE_FORCE;
		dDamperF = D_UNITSYS_BASE_FORCE;
		dSpringStiff = D_UNITSYS_BASE_SPRING;
		dDamperStiff = D_UNITSYS_BASE_SPRING;
		dDesignF = D_UNITSYS_BASE_FORCE;
		dYieldStiff = D_UNITSYS_BASE_SPRING;
		dEquiStiff = D_UNITSYS_BASE_SPRING;
		dEffectStiff = D_UNITSYS_BASE_SPRING;

	}

	int dFy;
	int dDisp;
	int dInitStiff;
	int dLength;
	int dForce;
	int dDamperF;
	int dSpringStiff;
	int dDamperStiff;
	int dDesignF;
	int dYieldStiff;
	int dEquiStiff;
	int dEffectStiff;
};
#define HASHSIZEDMGB 11


/************************************************************************/
/*      Damp performance check info:T_DPCI_D                            */
/************************************************************************/
#define T_DPCI_K unsigned int
struct T_DPCI_D
{
	T_STLD_K ThisCaseZ;
	T_STLD_K SplcCaseZ;
	T_STLD_K GravityCase;

	CArray<T_STLD_K, T_STLD_K> aThisCaseX;
	CArray<T_STLD_K, T_STLD_K> aSplcCaseX;

	double dRxCoe;
	double dRzCoe;

	void Initialize()
	{
		ThisCaseZ = 0;
		SplcCaseZ = 0;
		GravityCase = 0;
		aThisCaseX.RemoveAll();
		aSplcCaseX.RemoveAll();
		dRxCoe = 1.0;
		dRzCoe = 1.0;
	}
	T_DPCI_D() { Initialize(); }
	T_DPCI_D(const T_DPCI_D& src) { T_DPCI_D(); *this = src; }
	T_DPCI_D& T_DPCI_D::operator=(const T_DPCI_D &src)
	{
		ThisCaseZ = src.ThisCaseZ;
		SplcCaseZ = src.SplcCaseZ;
		GravityCase = src.GravityCase;
		aThisCaseX.Copy(src.aThisCaseX);
		aSplcCaseX.Copy(src.aSplcCaseX);
		dRxCoe = src.dRxCoe;
		dRzCoe = src.dRzCoe;

		return *this;
	}
};
struct T_DPCI_D_RW
{
	T_STLD_K ThisCaseZ;
	T_STLD_K SplcCaseZ;
	T_STLD_K GravityCase;
	int ThisCaseXNum;
	int SplcCaseXNum;
	double dRxCoe;
	double dRzCoe;

	void GetDpci(T_DPCI_D &rData)const
	{
		rData.ThisCaseZ = ThisCaseZ;
		rData.SplcCaseZ = SplcCaseZ;
		rData.GravityCase = GravityCase;

		rData.aThisCaseX.RemoveAll();
		rData.aSplcCaseX.RemoveAll();
		rData.dRxCoe = dRxCoe;
		rData.dRzCoe = dRzCoe;
	}
	void SetDpci(const T_DPCI_D &rData)
	{
		ThisCaseZ = rData.ThisCaseZ;
		SplcCaseZ = rData.SplcCaseZ;
		GravityCase = rData.GravityCase;

		ThisCaseXNum = rData.aThisCaseX.GetCount();
		SplcCaseXNum = rData.aSplcCaseX.GetCount();

		dRxCoe = rData.dRxCoe;
		dRzCoe = rData.dRzCoe;
	}
};
struct T_DPCI_UDRD_D
{
	T_DPCI_D data;
};

/************************************************************************/
/*      Damp Specify Special Type :T_DSST_D                            */
/************************************************************************/
#define T_DSST_K unsigned int
#define HASHSIZEDSST 1001
struct T_DSST_D
{
	int nDampType;//0 ordinary Memb;1 Critical Memb; 2 import horizontal Memb; 3 base Memb;

	void Initialize()
	{
		nDampType = 0;
	}
	T_DSST_D() { Initialize(); }
	T_DSST_D(const T_DSST_D& src) { T_DSST_D(); *this = src; }
	T_DSST_D& T_DSST_D::operator=(const T_DSST_D &src)
	{
		nDampType = src.nDampType;
		return *this;
	}
};
struct T_DSST_UDRD_D
{
	T_DSST_K key;
	T_DSST_D data;
};



/************************************************************************/
/*      Damping GB:T_DAGB_D                            */
/************************************************************************/
#define T_DAGB_K unsigned int
struct T_DAGB_D
{
	int nCode;			//0:"GB/T 51408-2021" 1:"DB11-2075-2022"
	int nBldCategory;	//-1:标记未初始化..  0:?类 1:?类 2:一般
	//0:钢筋混凝土框架结构 1:钢筋混凝土框架-抗震墙结构 2:框架-核心筒结构 3:钢筋混凝土抗震墙 4:板柱-抗震墙结构 5:筒中筒 
	//6:钢筋混凝土框支层结构 7:多高层钢结构
	int nStrutCategory;
	CArray<T_STLD_K, T_STLD_K> aNonUseThisCase;		//没有使用仅为了标记(时程)
	CArray<T_STLD_K, T_STLD_K> aNonUseSplcCase;		//没有使用仅为了标记(反应谱）
	CArray<T_STLD_K, T_STLD_K> aResistThisCase;		//设防地震工况(时程)
	CArray<T_STLD_K, T_STLD_K> aResistSplcCase;		//设防地震工况(反应谱）
	CArray<T_STLD_K, T_STLD_K> aRareEThisCase;		//罕遇地震工况(时程)
	CArray<T_STLD_K, T_STLD_K> aRareESplcCase;		//罕遇地震工况(反应谱）

	void Initialize()
	{
		nCode = 0;
		nBldCategory = -1;
		nStrutCategory = 5;
		aNonUseThisCase.RemoveAll();
		aNonUseSplcCase.RemoveAll();
		aResistThisCase.RemoveAll();
		aResistSplcCase.RemoveAll();
		aRareEThisCase.RemoveAll();
		aRareESplcCase.RemoveAll();
	}

	T_DAGB_D() { Initialize(); }
	T_DAGB_D(const T_DAGB_D& src) { T_DAGB_D(); *this = src; }
	T_DAGB_D& T_DAGB_D::operator=(const T_DAGB_D &src)
	{
		nCode = src.nCode;
		nBldCategory = src.nBldCategory;
		nStrutCategory = src.nStrutCategory;
		aNonUseThisCase.Copy(src.aNonUseThisCase);
		aNonUseSplcCase.Copy(src.aNonUseSplcCase);
		aResistThisCase.Copy(src.aResistThisCase);
		aResistSplcCase.Copy(src.aResistSplcCase);
		aRareEThisCase.Copy(src.aRareEThisCase);
		aRareESplcCase.Copy(src.aRareESplcCase);

		return *this;
	}
};
struct T_DAGB_UDRD_D
{
	T_DAGB_D data;
};
struct T_DAGB_UNIT
{
	T_DAGB_UNIT()
	{
		nCode = D_UNITSYS_NONE;
		nBldCategory = D_UNITSYS_NONE;
		nStrutCategory = D_UNITSYS_NONE;
	}
	int nCode;
	int nBldCategory;
	int nStrutCategory;
};
struct T_DAGB_UDRD_D_RW
{
	int nCode;			//"GB/T 51408-2021"
	int nBldCategory;	//0:特殊设防类 1:重点设防类 2:标准设防类”
	int nStrutCategory; //0:钢筋混凝土框架结构 1:钢筋混凝土框架-抗震墙结构 2:框架-核心筒结构 3:板柱-抗震墙结构 4:钢筋混凝土抗震墙结构 5:钢结构

	int nNonUseThisCase;
	int nNonUseSplcCase;
	int nResistThisCase;	//设防地震工况(时程)
	int nResistSplcCase;	//设防地震工况(反应谱）
	int nRareEThisCase;		//罕遇地震工况(时程)
	int nRareESplcCase;		//罕遇地震工况(反应谱）

	void GetDAGB(T_DAGB_D &rData)
	{
		rData.nCode = nCode;
		rData.nBldCategory = nBldCategory;
		rData.nStrutCategory = nStrutCategory;
	}
	void SetDAGB(const T_DAGB_D &rData)
	{
		nCode = rData.nCode;
		nBldCategory = rData.nBldCategory;
		nStrutCategory = rData.nStrutCategory;
		nNonUseThisCase = (int)rData.aNonUseThisCase.GetSize();
		nNonUseSplcCase = (int)rData.aNonUseSplcCase.GetSize();
		nResistThisCase = (int)rData.aResistThisCase.GetSize();
		nResistSplcCase = (int)rData.aResistSplcCase.GetSize();
		nRareEThisCase = (int)rData.aRareEThisCase.GetSize();
		nRareESplcCase = (int)rData.aRareESplcCase.GetSize();
	}
};
#define HASHSIZEDAGB 1

#pragma pack(pop)
#endif 


