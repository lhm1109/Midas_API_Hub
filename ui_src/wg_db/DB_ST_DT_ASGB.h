#ifndef __DB_ST_DT_ASGB_H__
#define __DB_ST_DT_ASGB_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#define T_ASGB_K unsigned int

struct T_ASGB_LNR_930
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dKh;
	double dTk;
	double dTq;
	double dS2;
	void Initialize()
	{
		nInputMethod = 1;
		nCode = 1;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		dD = 1500;
		dTr = 300;
		dKv = 8300;
		dKvt = 8300;
		dKh = 2.3;
		dTk = 1.0;
		dTq = 1.0;
		dS2 = 5;
	}
};

struct T_ASGB_LRB_930
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	double dKd;
	double dOd;//Qd
	double dTk;
	double dTq;
	double dHorEquiStiff;
	double dEquiRot;
	double dS2;
	void Initialize()
	{
		nInputMethod = 1;
		nCode = 1;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		dD = 1500;
		dTr = 300;
		dKv = 8600;
		dKvt = 8600;
		dK1 = 29.1;
		dKd = 2.24;
		dOd = 420;
		dTk = 1.0;
		dTq = 1.0;
		dHorEquiStiff = 3.64;
		dEquiRot = 23;
		dS2 = 5;
	}
};

struct T_ASGB_HDR_930
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	double dKd;
	double dOd;
	double dTk;
	double dTq;
	double dHorEquiStiff;
	double dEquiRot;
	double dS2;
	void Initialize()
	{
		nInputMethod = 1;
		nCode = 0;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		dD = 1500;
		dTr = 300;
		dKv = 8600;
		dKvt = 8600;
		dK1 = 29.1;
		dKd = 2.24;
		dOd = 420;
		dTk = 1.0;
		dTq = 1.0;
		dHorEquiStiff = 3.64;
		dEquiRot = 23;
		dS2 = 5;
	}
};

//////////////////////////////////////////////////////////////
struct T_ASGB_LNR
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dKh;
	double dTk;
	double dTq;
	double dS2;
	double dS1;
	void Initialize()
	{
		nInputMethod = 1;
		nCode = 1;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		dD = 1500;
		dTr = 300;
		dKv = 8300;
		dKvt = 8300;
		dKh = 2.3;
		dTk = 1.0;
		dTq = 1.0;
		dS2 = 5;
		dS1 = 30;
	}
};

struct T_ASGB_LRB
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	double dKd;
	double dOd;//Qd
	double dTk;
	double dTq;
	double dHorEquiStiff;
	double dEquiRot;
	double dS2;
	double dS1;
	void Initialize()
	{
		nInputMethod = 1;
		nCode = 1;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		dD = 1500;
		dTr = 300;
		dKv = 8600;
		dKvt = 8600;
		dK1 = 29.1;
		dKd = 2.24;
		dOd = 420;
		dTk = 1.0;
		dTq = 1.0;
		dHorEquiStiff = 3.64;
		dEquiRot = 23;
		dS2 = 5;
		dS1 = 30;
	}
};

struct T_ASGB_HDR
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	double dKd;
	double dOd;
	double dTk;
	double dTq;
	double dHorEquiStiff;
	double dEquiRot;
	double dS2;
	double dS1;
	void Initialize()
	{
		nInputMethod = 1;
		nCode = 0;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		dD = 1500;
		dTr = 300;
		dKv = 8600;
		dKvt = 8600;
		dK1 = 29.1;
		dKd = 2.24;
		dOd = 420;
		dTk = 1.0;
		dTq = 1.0;
		dHorEquiStiff = 3.64;
		dEquiRot = 23;
		dS2 = 5;
		dS1 = 30;
	}
};

struct T_ASGB_ESB
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	CString strClassify;
	CString strInputType;
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	int nSel;
	double dU;
	CString strU;
	double dTk;
	double dTq;

	void Initialize()
	{
		nInputMethod = 1;
		nCode = 0;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		dD = 1500;
		dTr = 300;
		dKv = 30000;
		dKvt = 0;
		dK1 = 35;
		nSel = 0;
		dU  = 0.03;
		strU = _T("");
		dTk = 1.0;
		dTq = 1.0;
	}
};

struct T_ASGB_FPS
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	double nR;
	int nSelR;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	double nr;
	double dUs;
	int nSelUs;
	double dUf;
	int nSelUf;
	double dTk;
	double dTq;
	double dHorDis;
	double dVBearing;
	CString strClassify;
	CString strInputType;
	CString strR;
	CString strKv;
	CString strUs;
	CString strUf;
	CString strHorDis;

	void Initialize()
	{
		nInputMethod = 1;
		nCode = 0;
		nClassify = 0;
		nInputType = 0;
		strClassify = _T("");
		strInputType = _T("");
		nR = 2500;
		nSelR = 0;
		dKv = 550;
		dKvt = 0;
		dK1 = 30;
		nr = 0.02;
		dUs = 0.03;
		nSelUs = 0;
		dUf = 0.03;
		nSelUf = 0;
		dTk = 1.0;
		dTq = 1.0;
		dHorDis = 100;
		dVBearing = 1000;
		strKv = _T("");
		strUs = _T("");
		strUf = _T("");
		strHorDis = _T("");
	}

};

#define D_ASGB_DEV_LNR 0 // LNR
#define D_ASGB_DEV_LRB 1 // LRB
#define D_ASGB_DEV_HDR 2 // HDR
#define D_ASGB_DEV_ESB 3 // ESB
#define D_ASGB_DEV_FPS 4 // FBS

struct T_ASGB_D_930
{
	CString		PropName;
	CString		Description;
	int         nPropType;    // 0:LNR, 1:LRB, 2:HDR, 3:ESB, 4:FPS
	unsigned int nSeq;    // 鉴锅

	T_ASGB_LNR_930 LNR;
	T_ASGB_LRB_930 LRB;
	T_ASGB_HDR_930 HDR;
	T_ASGB_ESB ESB;
	T_ASGB_FPS FPS;
	UINT        ElemKey4D2E;
	void Initialize()
	{
		PropName.Empty();
		Description.Empty();
		nPropType = 0;
		nSeq = 0;
		ElemKey4D2E = 0;
		LNR.Initialize();
		LRB.Initialize();
		HDR.Initialize();
		ESB.Initialize();
		FPS.Initialize();
	}
	T_ASGB_D_930() { Initialize(); }
	T_ASGB_D_930(const T_ASGB_D_930& src) { T_ASGB_D_930(); *this = src; }
	T_ASGB_D_930& T_ASGB_D_930::operator=(const T_ASGB_D_930 &src)
	{
		PropName = src.PropName;
		Description = src.Description;
		nPropType = src.nPropType;
		nSeq = src.nSeq;
		LNR = src.LNR;
		LRB = src.LRB;
		HDR = src.HDR;
		ESB = src.ESB;
		FPS = src.FPS;
		ElemKey4D2E = src.ElemKey4D2E;

		return *this;
	}
};

struct T_ASGB_D
{
	CString		PropName;
	CString		Description;
	int         nPropType;    // 0:LNR, 1:LRB, 2:HDR, 3:ESB, 4:FPS
	unsigned int nSeq;    // 鉴锅
	
	T_ASGB_LNR LNR;
	T_ASGB_LRB LRB;
	T_ASGB_HDR HDR;
	T_ASGB_ESB ESB;
	T_ASGB_FPS FPS;
	UINT        ElemKey4D2E;
	void Initialize()
	{
		PropName.Empty();
		Description.Empty();
		nPropType = 0;
		nSeq = 0;
		ElemKey4D2E = 0;
		LNR.Initialize();
		LRB.Initialize();
		HDR.Initialize();
		ESB.Initialize();
		FPS.Initialize();
	}
	T_ASGB_D() { Initialize(); }
	T_ASGB_D(const T_ASGB_D& src) { T_ASGB_D(); *this = src; }
	T_ASGB_D& T_ASGB_D::operator=(const T_ASGB_D &src)
	{
		PropName = src.PropName;
		Description = src.Description;
		nPropType = src.nPropType;
		nSeq = src.nSeq;
		LNR = src.LNR;
		LRB = src.LRB;
		HDR = src.HDR;
		ESB = src.ESB;
		FPS = src.FPS;
		ElemKey4D2E = src.ElemKey4D2E;

		return *this;
	}
};

struct T_ASGB_LNR_CH_930
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dKh;
	double dTk;
	double dTq;
	double dS2;
};
struct T_ASGB_LRB_CH_930
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	double dKd;
	double dOd;//Qd
	double dTk;
	double dTq;
	double dHorEquiStiff;
	double dEquiRot;
	double dS2;
};
struct T_ASGB_HDR_CH_930
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	double dKd;
	double dOd;
	double dTk;
	double dTq;
	double dHorEquiStiff;
	double dEquiRot;
	double dS2;
};
struct T_ASGB_LNR_CH
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dKh;
	double dTk;
	double dTq;
	double dS2;
	double dS1;
};
struct T_ASGB_LRB_CH
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	double dKd;
	double dOd;//Qd
	double dTk;
	double dTq;
	double dHorEquiStiff;
	double dEquiRot;
	double dS2;
	double dS1;
};
struct T_ASGB_HDR_CH
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	double dKd;
	double dOd;
	double dTk;
	double dTq;
	double dHorEquiStiff;
	double dEquiRot;
	double dS2;
	double dS1;
};
struct T_ASGB_ESB_CH
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	char strClassify[40];
	char strInputType[40];
	double dD;
	double dTr;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	int nSel;
	double dU;
	char strU[40];
	double dTk;
	double dTq;
};
struct T_ASGB_FPS_CH
{
	int	nInputMethod; //0:User, 1:Code 
	int	nCode;
	int	nClassify;
	int	nInputType;
	double nR;
	int nSelR;
	double dKv; //Kvc
	double dKvt;
	double dK1;
	double nr;
	double dUs;
	int nSelUs;
	double dUf;
	int nSelUf;
	double dTk;
	double dTq;
	double dHorDis;
	double dVBearing;
	char strClassify[40];
	char strInputType[40];
	char strR[40];
	char strKv[40];
	char strUs[40];
	char strUf[40];
	char strHorDis[40];
};
struct T_ASGB_D_CH_930
{
	char		PropName[40];
	char		Description[40];
	int         nPropType;    // 0:LNR, 1:LRB, 2:HDR, 3:ESB, 4:Lead FPS
	unsigned int nSeq;    // 鉴锅

	T_ASGB_LNR_CH_930 LNR;
	T_ASGB_LRB_CH_930 LRB;
	T_ASGB_HDR_CH_930 HDR;
	T_ASGB_ESB_CH ESB;
	T_ASGB_FPS_CH FPS;
	UINT        ElemKey4D2E;
};
struct T_ASGB_UDRD_D_930
{
	T_ASGB_K key;
	T_ASGB_D_930 data;
};
struct T_ASGB_UDRD_D_CH_930
{
	T_ASGB_K key;
	T_ASGB_D_CH_930 data;
};


struct T_ASGB_D_CH
{
	char		PropName[40];
	char		Description[40];
	int         nPropType;    // 0:LNR, 1:LRB, 2:HDR, 3:ESB, 4:Lead FPS
	unsigned int nSeq;    // 鉴锅

	T_ASGB_LNR_CH LNR;
	T_ASGB_LRB_CH LRB;
	T_ASGB_HDR_CH HDR;
	T_ASGB_ESB_CH ESB;
	T_ASGB_FPS_CH FPS;
	UINT        ElemKey4D2E;

	void ConvertToString(T_ASGB_D & rData)
	{
		ConvertCharStr(PropName, rData.PropName, sizeof(PropName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nPropType = nPropType;
		rData.nSeq = nSeq;
		rData.ElemKey4D2E = ElemKey4D2E;

		ConvertCharStr(LNR.strClassify, rData.LNR.strClassify, sizeof(LNR.strClassify));
		ConvertCharStr(LNR.strInputType, rData.LNR.strInputType, sizeof(LNR.strInputType));
		rData.LNR.nInputMethod = LNR.nInputMethod;
		rData.LNR.nCode = LNR.nCode;
		rData.LNR.nClassify = LNR.nClassify;
		rData.LNR.nInputType = LNR.nInputType;
		rData.LNR.dD = LNR.dD;
		rData.LNR.dTr = LNR.dTr;
		rData.LNR.dKv = LNR.dKv; //Kvc
		rData.LNR.dKvt = LNR.dKvt;
		rData.LNR.dKh = LNR.dKh;
		rData.LNR.dTk = LNR.dTk;
		rData.LNR.dTq = LNR.dTq;
		rData.LNR.dS2 = LNR.dS2;
		rData.LNR.dS1 = LNR.dS1;

		ConvertCharStr(LRB.strClassify, rData.LRB.strClassify, sizeof(LRB.strClassify));
		ConvertCharStr(LRB.strInputType, rData.LRB.strInputType, sizeof(LRB.strInputType));
		rData.LRB.nInputMethod = LRB.nInputMethod;
		rData.LRB.nCode = LRB.nCode;
		rData.LRB.nClassify = LRB.nClassify;
		rData.LRB.nInputType = LRB.nInputType;
		rData.LRB.dD = LRB.dD;
		rData.LRB.dTr = LRB.dTr;
		rData.LRB.dKv = LRB.dKv;
		rData.LRB.dKvt = LRB.dKvt;
		rData.LRB.dK1 = LRB.dK1;
		rData.LRB.dKd = LRB.dKd;
		rData.LRB.dOd = LRB.dOd;
		rData.LRB.dTk = LRB.dTk;
		rData.LRB.dTq = LRB.dTq;
		rData.LRB.dHorEquiStiff = LRB.dHorEquiStiff;
		rData.LRB.dEquiRot = LRB.dEquiRot;
		rData.LRB.dS2 = LRB.dS2;
		rData.LRB.dS1 = LRB.dS1;

		ConvertCharStr(HDR.strClassify, rData.HDR.strClassify, sizeof(HDR.strClassify));
		ConvertCharStr(HDR.strInputType, rData.HDR.strInputType, sizeof(HDR.strInputType));
		rData.HDR.nInputMethod = HDR.nInputMethod; //0:User, 1:Code 
		rData.HDR.nCode = HDR.nCode;
		rData.HDR.nClassify = HDR.nClassify;
		rData.HDR.nInputType = HDR.nInputType;
		rData.HDR.dD = HDR.dD;
		rData.HDR.dTr = HDR.dTr;
		rData.HDR.dKv = HDR.dKv; //Kvc
		rData.HDR.dKvt = HDR.dKvt;
		rData.HDR.dK1 = HDR.dK1;
		rData.HDR.dKd = HDR.dKd;
		rData.HDR.dOd = HDR.dOd;
		rData.HDR.dTk = HDR.dTk;
		rData.HDR.dTq = HDR.dTq;
		rData.HDR.dHorEquiStiff = HDR.dHorEquiStiff;
		rData.HDR.dEquiRot = HDR.dEquiRot;
		rData.HDR.dS2 = HDR.dS2;
		rData.HDR.dS1 = HDR.dS1;

		ConvertCharStr(ESB.strClassify, rData.ESB.strClassify, sizeof(ESB.strClassify));
		ConvertCharStr(ESB.strInputType, rData.ESB.strInputType, sizeof(ESB.strInputType));
		ConvertCharStr(ESB.strU, rData.ESB.strU, sizeof(ESB.strU));
		rData.ESB.nInputMethod = ESB.nInputMethod; //0:User, 1:Code 
		rData.ESB.nCode = ESB.nCode;
		rData.ESB.nClassify = ESB.nClassify;
		rData.ESB.nInputType = ESB.nInputType;
		rData.ESB.dD = ESB.dD;
		rData.ESB.dTr = ESB.dTr;
		rData.ESB.dKv = ESB.dKv; //Kvc
		rData.ESB.dKvt = ESB.dKvt;
		rData.ESB.dK1 = ESB.dK1;
		rData.ESB.nSel = ESB.nSel;
		rData.ESB.dU = ESB.dU;
		ConvertCharStr(ESB.strU, rData.ESB.strU, sizeof(ESB.strU));
		rData.ESB.dTk = ESB.dTk;
		rData.ESB.dTq = ESB.dTq;

		ConvertCharStr(FPS.strClassify, rData.FPS.strClassify, sizeof(FPS.strClassify));
		ConvertCharStr(FPS.strInputType, rData.FPS.strInputType, sizeof(FPS.strInputType));
		ConvertCharStr(FPS.strR, rData.FPS.strR, sizeof(FPS.strR));
		ConvertCharStr(FPS.strKv, rData.FPS.strKv, sizeof(FPS.strKv));
		ConvertCharStr(FPS.strUs, rData.FPS.strUs, sizeof(FPS.strUs));
		ConvertCharStr(FPS.strUf, rData.FPS.strUf, sizeof(FPS.strUf));
		ConvertCharStr(FPS.strHorDis, rData.FPS.strHorDis, sizeof(FPS.strHorDis));
		rData.FPS.nInputMethod = FPS.nInputMethod; //0:User, 1:Code 
		rData.FPS.nCode = FPS.nCode;
		rData.FPS.nClassify = FPS.nClassify;
		rData.FPS.nInputType = FPS.nInputType;
		rData.FPS.nR = FPS.nR;
		rData.FPS.nSelR = FPS.nSelR;
		rData.FPS.dKv = FPS.dKv;
		rData.FPS.dKvt = FPS.dKvt;
		rData.FPS.dK1 = FPS.dK1;
		rData.FPS.nr = FPS.nr;
		rData.FPS.dUs = FPS.dUs;
		rData.FPS.nSelUs = FPS.nSelUs;
		rData.FPS.dUf = FPS.dUf;
		rData.FPS.nSelUf = FPS.nSelUf;
		rData.FPS.dTk = FPS.dTk;
		rData.FPS.dTq = FPS.dTq;
		rData.FPS.dHorDis = FPS.dHorDis;
		rData.FPS.dVBearing = FPS.dVBearing;
	}
	void ConvertToChar(T_ASGB_D& rData)
	{
		ConvertStrChar(rData.PropName, PropName, sizeof(PropName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nPropType = rData.nPropType;
		nSeq = rData.nSeq;
		ElemKey4D2E = rData.ElemKey4D2E;

		ConvertStrChar(rData.LNR.strClassify, LNR.strClassify, sizeof(LNR.strClassify));
		ConvertStrChar(rData.LNR.strInputType, LNR.strInputType, sizeof(LNR.strInputType));
		LNR.nInputMethod = rData.LNR.nInputMethod;
		LNR.nCode = rData.LNR.nCode;
		LNR.nClassify = rData.LNR.nClassify;
		LNR.nInputType = rData.LNR.nInputType;
		LNR.dD = rData.LNR.dD;
		LNR.dTr = rData.LNR.dTr;
		LNR.dKv = rData.LNR.dKv; //Kvc
		LNR.dKvt = rData.LNR.dKvt;
		LNR.dKh = rData.LNR.dKh;
		LNR.dTk = rData.LNR.dTk;
		LNR.dTq = rData.LNR.dTq;
		LNR.dS2 = rData.LNR.dS2;
		LNR.dS1 = rData.LNR.dS1;

		ConvertStrChar(rData.LRB.strClassify, LRB.strClassify, sizeof(LRB.strClassify));
		ConvertStrChar(rData.LRB.strInputType, LRB.strInputType, sizeof(LRB.strInputType));
		LRB.nInputMethod = rData.LRB.nInputMethod;
		LRB.nCode = rData.LRB.nCode;
		LRB.nClassify = rData.LRB.nClassify;
		LRB.nInputType = rData.LRB.nInputType;
		LRB.dD = rData.LRB.dD;
		LRB.dTr = rData.LRB.dTr;
		LRB.dKv = rData.LRB.dKv;
		LRB.dKvt = rData.LRB.dKvt;
		LRB.dK1 = rData.LRB.dK1;
		LRB.dKd = rData.LRB.dKd;
		LRB.dOd = rData.LRB.dOd;
		LRB.dTk = rData.LRB.dTk;
		LRB.dTq = rData.LRB.dTq;
		LRB.dHorEquiStiff = rData.LRB.dHorEquiStiff;
		LRB.dEquiRot = rData.LRB.dEquiRot;
		LRB.dS2 = rData.LRB.dS2;
		LRB.dS1 = rData.LRB.dS1;

		ConvertStrChar(rData.HDR.strInputType, HDR.strInputType, sizeof(HDR.strInputType));
		ConvertStrChar(rData.HDR.strClassify, HDR.strClassify, sizeof(HDR.strClassify));
		HDR.nInputMethod = rData.HDR.nInputMethod; //0:User, 1:Code 
		HDR.nCode = rData.HDR.nCode;
		HDR.nClassify = rData.HDR.nClassify;
		HDR.nInputType = rData.HDR.nInputType;
		HDR.dD = rData.HDR.dD;
		HDR.dTr = rData.HDR.dTr;
		HDR.dKv = rData.HDR.dKv; //Kvc
		HDR.dKvt = rData.HDR.dKvt;
		HDR.dK1 = rData.HDR.dK1;
		HDR.dKd = rData.HDR.dKd;
		HDR.dOd = rData.HDR.dOd;
		HDR.dTk = rData.HDR.dTk;
		HDR.dTq = rData.HDR.dTq;
		HDR.dHorEquiStiff = rData.HDR.dHorEquiStiff;
		HDR.dEquiRot = rData.HDR.dEquiRot;
		HDR.dS2 = rData.HDR.dS2;
		LRB.dS1 = rData.LRB.dS1;

		ConvertStrChar(rData.ESB.strClassify, ESB.strClassify, sizeof(ESB.strClassify));
		ConvertStrChar(rData.ESB.strInputType, ESB.strInputType, sizeof(ESB.strInputType));
		ConvertStrChar(rData.ESB.strU, ESB.strU, sizeof(ESB.strU));
		ESB.nInputMethod = rData.ESB.nInputMethod; //0:User, 1:Code 
		ESB.nCode = rData.ESB.nCode;
		ESB.nClassify = rData.ESB.nClassify;
		ESB.nInputType = rData.ESB.nInputType;
		ESB.dD = rData.ESB.dD;
		ESB.dTr = rData.ESB.dTr;
		ESB.dKv = rData.ESB.dKv; //Kvc
		ESB.dKvt = rData.ESB.dKvt;
		ESB.dK1 = rData.ESB.dK1;
		ESB.nSel = rData.ESB.nSel;
		ESB.dU = rData.ESB.dU;
		ESB.dTk = rData.ESB.dTk;
		ESB.dTq = rData.ESB.dTq;

		ConvertStrChar(rData.FPS.strClassify, FPS.strClassify, sizeof(FPS.strClassify));
		ConvertStrChar(rData.FPS.strInputType, FPS.strInputType, sizeof(FPS.strInputType));
		ConvertStrChar(rData.FPS.strR, FPS.strR, sizeof(FPS.strR));
		ConvertStrChar(rData.FPS.strKv, FPS.strKv, sizeof(FPS.strKv));
		ConvertStrChar(rData.FPS.strUf, FPS.strUf, sizeof(FPS.strUf));
		ConvertStrChar(rData.FPS.strUs, FPS.strUs, sizeof(FPS.strUs));
		ConvertStrChar(rData.FPS.strHorDis, FPS.strHorDis, sizeof(FPS.strHorDis));
		FPS.nInputMethod = rData.FPS.nInputMethod; //0:User, 1:Code 
		FPS.nCode = rData.FPS.nCode;
		FPS.nClassify = rData.FPS.nClassify;
		FPS.nInputType = rData.FPS.nInputType;
		FPS.nR = rData.FPS.nR;
		FPS.nSelR = rData.FPS.nSelR;
		FPS.dKv = rData.FPS.dKv;
		FPS.dKvt = rData.FPS.dKvt;
		FPS.dK1 = rData.FPS.dK1;
		FPS.nr = rData.FPS.nr;
		FPS.dUs = rData.FPS.dUs;
		FPS.nSelUs = rData.FPS.nSelUs;
		FPS.dUf = rData.FPS.dUf;
		FPS.nSelUf = rData.FPS.nSelUf;
		FPS.dTk = rData.FPS.dTk;
		FPS.dTq = rData.FPS.dTq;
		FPS.dHorDis = rData.FPS.dHorDis;
		FPS.dVBearing = rData.FPS.dVBearing;
	}

	void Convert930(T_ASGB_D_CH_930 & rData)

	{
		memcpy(PropName, rData.PropName, sizeof(PropName));
		memcpy(Description, rData.Description, sizeof(Description));
		nPropType = rData.nPropType;
		nSeq = rData.nSeq;
		ElemKey4D2E = rData.ElemKey4D2E;

		memcpy(LNR.strClassify, rData.LNR.strClassify, sizeof(LNR.strClassify));
		memcpy(LNR.strInputType, rData.LNR.strInputType, sizeof(LNR.strInputType));
		LNR.nInputMethod = rData.LNR.nInputMethod;
		LNR.nCode = rData.LNR.nCode;
		LNR.nClassify = rData.LNR.nClassify;
		LNR.nInputType = rData.LNR.nInputType;
		LNR.dD = rData.LNR.dD;
		LNR.dTr = rData.LNR.dTr;
		LNR.dKv = rData.LNR.dKv; //Kvc
		LNR.dKvt = rData.LNR.dKvt;
		LNR.dKh = rData.LNR.dKh;
		LNR.dTk = rData.LNR.dTk;
		LNR.dTq = rData.LNR.dTq;
		LNR.dS2 = rData.LNR.dS2;
		LNR.dS1 = 30;

		memcpy(LRB.strClassify, rData.LRB.strClassify, sizeof(LRB.strClassify));
		memcpy(LRB.strInputType, rData.LRB.strInputType, sizeof(LRB.strInputType));
		LRB.nInputMethod = rData.LRB.nInputMethod ;
		LRB.nCode = rData.LRB.nCode;
		LRB.nClassify = rData.LRB.nClassify;
		LRB.nInputType = rData.LRB.nInputType;
		LRB.dD = rData.LRB.dD;
		LRB.dTr = rData.LRB.dTr;
		LRB.dKv = rData.LRB.dKv;
		LRB.dKvt = rData.LRB.dKvt;
		LRB.dK1 = rData.LRB.dK1;
		LRB.dKd = rData.LRB.dKd;
		LRB.dOd = rData.LRB.dOd;
		LRB.dTk = rData.LRB.dTk;
		LRB.dTq = rData.LRB.dTq;
		LRB.dHorEquiStiff = rData.LRB.dHorEquiStiff;
		LRB.dEquiRot = rData.LRB.dEquiRot;
		LRB.dS2 = rData.LRB.dS2;
		LRB.dS1 = 30;

		memcpy(HDR.strClassify, rData.HDR.strClassify, sizeof(HDR.strClassify));
		memcpy(HDR.strInputType, rData.HDR.strInputType, sizeof(HDR.strInputType));
		HDR.nInputMethod = rData.HDR.nInputMethod; //0:User, 1:Code 
		HDR.nCode = rData.HDR.nCode;
		HDR.nClassify = rData.HDR.nClassify;
		HDR.nInputType = rData.HDR.nInputType;
		HDR.dD = rData.HDR.dD;
		HDR.dTr = rData.HDR.dTr;
		HDR.dKv = rData.HDR.dKv; //Kvc
		HDR.dKvt = rData.HDR.dKvt;
		HDR.dK1 = rData.HDR.dK1;
		HDR.dKd = rData.HDR.dKd;
		HDR.dOd = rData.HDR.dOd;
		HDR.dTk = rData.HDR.dTk;
		HDR.dTq = rData.HDR.dTq;
		HDR.dHorEquiStiff = rData.HDR.dHorEquiStiff;
		HDR.dEquiRot = rData.HDR.dEquiRot;
		HDR.dS2 = rData.HDR.dS2;
		HDR.dS1 = 30;

		memcpy(ESB.strClassify, rData.ESB.strClassify, sizeof(ESB.strClassify));
		memcpy(ESB.strInputType, rData.ESB.strInputType, sizeof(ESB.strInputType));
		memcpy(ESB.strU, rData.ESB.strU, sizeof(ESB.strU));
		ESB.nInputMethod = rData.ESB.nInputMethod; //0:User, 1:Code 
		ESB.nCode = rData.ESB.nCode;
		ESB.nClassify = rData.ESB.nClassify;
		ESB.nInputType = rData.ESB.nInputType;
		ESB.dD = rData.ESB.dD;
		ESB.dTr = rData.ESB.dTr ;
		ESB.dKv = rData.ESB.dKv; //Kvc
		ESB.dKvt = rData.ESB.dKvt;
		ESB.dK1 = rData.ESB.dK1;
		ESB.nSel = rData.ESB.nSel;
		ESB.dU = rData.ESB.dU;
		memcpy(ESB.strU, rData.ESB.strU, sizeof(ESB.strU));
		ESB.dTk = rData.ESB.dTk;
		ESB.dTq = rData.ESB.dTq;

		memcpy(FPS.strClassify, rData.FPS.strClassify, sizeof(FPS.strClassify));
		memcpy(FPS.strInputType, rData.FPS.strInputType, sizeof(FPS.strInputType));
		memcpy(FPS.strR, rData.FPS.strR, sizeof(FPS.strR));
		memcpy(FPS.strKv, rData.FPS.strKv, sizeof(FPS.strKv));
		memcpy(FPS.strUs, rData.FPS.strUs, sizeof(FPS.strUs));
		memcpy(FPS.strUf, rData.FPS.strUf, sizeof(FPS.strUf));
		memcpy(FPS.strHorDis, rData.FPS.strHorDis, sizeof(FPS.strHorDis));
		FPS.nInputMethod = rData.FPS.nInputMethod; //0:User, 1:Code 
		FPS.nCode = rData.FPS.nCode;
		FPS.nClassify = rData.FPS.nClassify;
		FPS.nInputType = rData.FPS.nInputType;
		FPS.nR = rData.FPS.nR;
		FPS.nSelR = rData.FPS.nSelR;
		FPS.dKv = rData.FPS.dKv;
		FPS.dKvt = rData.FPS.dKvt;
		FPS.dK1 = rData.FPS.dK1;
		FPS.nr = rData.FPS.nr;
		FPS.dUs = rData.FPS.dUs;
		FPS.nSelUs = rData.FPS.nSelUs;
		FPS.dUf = rData.FPS.dUf;
		FPS.nSelUf = rData.FPS.nSelUf;
		FPS.dTk = rData.FPS.dTk;
		FPS.dTq = rData.FPS.dTq;
		FPS.dHorDis = rData.FPS.dHorDis;
		FPS.dVBearing = rData.FPS.dVBearing;
	}

};
struct T_ASGB_UDRD_D
{
	T_ASGB_K key;
	T_ASGB_D data;
};
struct T_ASGB_UDRD_D_CH
{
	T_ASGB_K key;
	T_ASGB_D_CH data;
};

struct T_ASGB_UNIT
{
	T_ASGB_UNIT()
	{
		dD = D_UNITSYS_BASE_LENGTH;
		dTr = D_UNITSYS_BASE_LENGTH;
		dKv = D_UNITSYS_BASE_SPRING;
		dKvt = D_UNITSYS_BASE_SPRING;
		dKh = D_UNITSYS_BASE_SPRING;
		dK1 = D_UNITSYS_BASE_SPRING;
		dKd = D_UNITSYS_BASE_SPRING;
		dOd = D_UNITSYS_BASE_FORCE;
		dHorEquiStiff = D_UNITSYS_BASE_SPRING;
		nR = D_UNITSYS_BASE_LENGTH;
		nr = D_UNITSYS_BASE_SEC_L;
		dHorDis = D_UNITSYS_BASE_LENGTH;
		dVBearing = D_UNITSYS_BASE_FORCE;
	}

	int dD;
	int dTr;
	int dKv;
	int dKvt;
	int dKh;
	int dK1;
	int dKd;
	int dOd;
	int dHorEquiStiff;
	int nR;
	int nr;
	int dHorDis;
	int dVBearing;
};
#define HASHSIZEASGB 11

/************************************************************************/
/*      Isolation Specify Special Type :T_ISST_D;by xuezc				*/
/************************************************************************/
#define T_ISST_K unsigned int
#define HASHSIZEISST 1001
struct T_ISST_D
{
	int nIsolationType;//0 ordinary Memb;1 Critical Memb; 2 import horizontal Memb;3 base Memb;

	void Initialize()
	{
		nIsolationType = 0;
	}
	T_ISST_D() { Initialize(); }
	T_ISST_D(const T_ISST_D& src) { T_ISST_D(); *this = src; }
	T_ISST_D& T_ISST_D::operator=(const T_ISST_D& src)
	{
		nIsolationType = src.nIsolationType;
		return *this;
	}
};
struct T_ISST_UDRD_D
{
	T_ISST_K key;
	T_ISST_D data;
};

///////////////////////////////////////////////////////////
struct T_ISGB_UDRD_D_RW_940
{
	int nCode;			//"GB/T 51408-2021"
	int nFortificationCategory;	//0:特殊设防类 1:重点设防类 2:标准设防类
	int nStrutCategory; //0:钢筋混凝土框架结构 1:钢筋混凝土框架-抗震墙结构 2:框架-核心筒结构 3:板柱-抗震墙结构 4:钢筋混凝土抗震墙结构 5:钢结构
	int nSeisLayer;		//隔震层
	int nBeginLayer;	//上部结构起始层
	int nLcomG;			//重力荷载代表值组合
	int nLmtType;
	double dLNR[3];
	double dESB[3];
	double dFPS[3];
	int nResistThisCase;		//设防地震工况(时程)
	int nResistSplcCase;		//设防地震工况(反应谱）
	int nRareEThisCase;		//罕遇地震工况(时程)
	int nRareESplcCase;		//罕遇地震工况(反应谱）
	int nExtreRareEThisCase;	//极罕遇地震工况(时程)
	int nExtreRareESplcCase;	//极罕遇地震工况(反应谱）
};

#define T_ISGB_K unsigned int
struct T_ISGB_D
{
	int nCode;			//0 : "GB/T 51408-2021" 1 : "DB11-2075-2022"
	int nFortificationCategory;	//0:特殊设防类 1:重点设防类 2:标准设防类
	int nBldCategory;	//0:?类 1:?类 2:一般
	int nStrutCategory; //0:钢筋混凝土框架结构 1:钢筋混凝土框架-抗震墙结构 2:框架-核心筒结构 3:板柱-抗震墙结构 4:钢筋混凝土抗震墙结构 5:钢结构
	int nSeisLayer;		//隔震层
	int nBeginLayer;	//上部结构起始层
	int nLcomG;			//重力荷载代表值组合
	int nLmtType;		//支座应力限值计算方法 0：自动计算 1：用户自定义
	double dLNR[3];		//橡胶支座：0:压应力(重力荷载代表值) 1:压应力(罕遇地震) 2:拉应力(罕遇地震)
	double dESB[3];		//滑板支座
	double dFPS[3];		//摩擦摆支座
	CArray<T_STLD_K, T_STLD_K> aResistThisCase;		//设防地震工况(时程)   //(未使用)
	CArray<T_STLD_K, T_STLD_K> aResistSplcCase;		//设防地震工况(反应谱）//(未使用)
	CArray<T_STLD_K, T_STLD_K> aRareEThisCase;		//罕遇地震工况(时程)   //设防地震工况(时程)
	CArray<T_STLD_K, T_STLD_K> aRareESplcCase;		//罕遇地震工况(反应谱）//设防地震工况(反应谱）
	CArray<T_STLD_K, T_STLD_K> aExtreRareEThisCase;	//极罕遇地震工况(时程) //罕遇地震工况(时程)  
	CArray<T_STLD_K, T_STLD_K> aExtreRareESplcCase;	//极罕遇地震工况(反应谱）//罕遇地震工况(反应谱）

	void Initialize()
	{
		nCode = 0;
		nFortificationCategory = 1;
		nBldCategory = 0;
		nStrutCategory = 5;
		nSeisLayer = 0;
		nBeginLayer = 0;
		nLcomG = 0;
		nLmtType = 0;
		dLNR[0] = 12 * 1e6;
		dESB[0] = 15 * 1e6;
		dFPS[0] = 25 * 1e6;
		dLNR[1] = 25 * 1e6;
		dESB[1] = 30 * 1e6;
		dFPS[1] = 50 * 1e6;
		dLNR[2] = dESB[2] = dFPS[2] = 1 * 1e6;
		aResistThisCase.RemoveAll();
		aResistSplcCase.RemoveAll();
		aRareEThisCase.RemoveAll();
		aRareESplcCase.RemoveAll();
		aExtreRareEThisCase.RemoveAll();
		aExtreRareESplcCase.RemoveAll();
	}

	T_ISGB_D() { Initialize(); }
	T_ISGB_D(const T_ISGB_D& src) { T_ISGB_D(); *this = src; }
	T_ISGB_D& T_ISGB_D::operator=(const T_ISGB_D &src)
	{
		nCode = src.nCode;
		nFortificationCategory = src.nFortificationCategory;
		nBldCategory = src.nBldCategory;
		nStrutCategory = src.nStrutCategory;
		nSeisLayer = src.nSeisLayer;
		nBeginLayer = src.nBeginLayer;
		nLcomG = src.nLcomG;
		nLmtType = src.nLmtType;
		for (int i=0; i<3; i++)
		{
			dLNR[i] = src.dLNR[i];
			dESB[i] = src.dESB[i];
			dFPS[i] = src.dFPS[i];
		}
		aResistThisCase.Copy(src.aResistThisCase);
		aResistSplcCase.Copy(src.aResistSplcCase);
		aRareEThisCase.Copy(src.aRareEThisCase);
		aRareESplcCase.Copy(src.aRareESplcCase);
		aExtreRareEThisCase.Copy(src.aExtreRareEThisCase);
		aExtreRareESplcCase.Copy(src.aExtreRareESplcCase);

		return *this;
	}
};
struct T_ISGB_UDRD_D
{
	T_ISGB_D data;
};
struct T_ISGB_UNIT
{
	T_ISGB_UNIT()
	{
		nCode = D_UNITSYS_NONE;
		nFortificationCategory = D_UNITSYS_NONE;
		nBldCategory = D_UNITSYS_NONE;
		nStrutCategory = D_UNITSYS_NONE;
		nSeisLayer = D_UNITSYS_NONE;
		nBeginLayer = D_UNITSYS_NONE;
		nLcomG = D_UNITSYS_NONE;
		nLmtType = D_UNITSYS_NONE;
		for (int i=0; i<3; i++)
		{
			dLNR[i] = dESB[i] = dFPS[i] = D_UNITSYS_BASE_STRESS;
		}
	}
	int nCode;
	int nFortificationCategory;
	int nBldCategory;
	int nStrutCategory;
	int nSeisLayer;
	int nBeginLayer;
	int nLcomG;
	int nLmtType;
	int dLNR[3];
	int dESB[3];
	int dFPS[3];

};
struct T_ISGB_UDRD_D_RW
{
	int nCode;			//"GB/T 51408-2021"
	int nFortificationCategory;	//0:特殊设防类 1:重点设防类 2:标准设防类
	int nBldCategory;	//0:?类 1:?类 2:一般
	int nStrutCategory; //0:钢筋混凝土框架结构 1:钢筋混凝土框架-抗震墙结构 2:框架-核心筒结构 3:板柱-抗震墙结构 4:钢筋混凝土抗震墙结构 5:钢结构
	int nSeisLayer;		//隔震层
	int nBeginLayer;	//上部结构起始层
	int nLcomG;			//重力荷载代表值组合
	int nLmtType;
	double dLNR[3];
	double dESB[3];
	double dFPS[3];
	int nResistThisCase;		//设防地震工况(时程)
	int nResistSplcCase;		//设防地震工况(反应谱）
	int nRareEThisCase;		//罕遇地震工况(时程)
	int nRareESplcCase;		//罕遇地震工况(反应谱）
	int nExtreRareEThisCase;	//极罕遇地震工况(时程)
	int nExtreRareESplcCase;	//极罕遇地震工况(反应谱）

	void GetIsgb(T_ISGB_D &rData)
	{
		rData.nCode = nCode;
		rData.nFortificationCategory = nFortificationCategory;
		rData.nBldCategory = nBldCategory;
		rData.nStrutCategory = nStrutCategory;
		rData.nSeisLayer = nSeisLayer;
		rData.nBeginLayer = nBeginLayer;
		rData.nLcomG = nLcomG;
		rData.nLmtType = nLmtType;
		for (int i=0; i<3; i++)
		{
			rData.dLNR[i] = dLNR[i];
			rData.dESB[i] = dESB[i];
			rData.dFPS[i] = dFPS[i];
		}
	}
	void SetIsgb(const T_ISGB_D &rData)
	{
		nCode = rData.nCode;
		nFortificationCategory = rData.nFortificationCategory;
		nBldCategory = rData.nBldCategory;
		nStrutCategory = rData.nStrutCategory;
		nSeisLayer = rData.nSeisLayer;
		nBeginLayer = rData.nBeginLayer;
		nLcomG = rData.nLcomG;
		nLmtType = rData.nLmtType;
		for (int i = 0; i < 3; i++)
		{
			dLNR[i] = rData.dLNR[i];
			dESB[i] = rData.dESB[i];
			dFPS[i] = rData.dFPS[i];
		}
		nResistThisCase=(int)rData.aResistThisCase.GetSize();
		nResistSplcCase = (int)rData.aResistSplcCase.GetSize();
		nRareEThisCase = (int)rData.aRareEThisCase.GetSize();
		nRareESplcCase = (int)rData.aRareESplcCase.GetSize();
		nExtreRareEThisCase = (int)rData.aExtreRareEThisCase.GetSize();
		nExtreRareESplcCase = (int)rData.aExtreRareESplcCase.GetSize();
	}
	void Convert940(const T_ISGB_UDRD_D_RW_940& rData)
	{
		nCode = rData.nCode;
		nFortificationCategory = rData.nFortificationCategory;
		nStrutCategory = rData.nStrutCategory;
		nSeisLayer = rData.nSeisLayer;
		nBeginLayer = rData.nBeginLayer;
		nLcomG = rData.nLcomG;
		nLmtType = rData.nLmtType;
		for(int i = 0; i < 3; i++)
		{
			dLNR[i] = rData.dLNR[i];
			dESB[i] = rData.dESB[i];
			dFPS[i] = rData.dFPS[i];
		}
		nResistThisCase = rData.nResistThisCase;
		nResistSplcCase = rData.nResistSplcCase;
		nRareEThisCase = rData.nRareEThisCase;
		nRareESplcCase = rData.nRareESplcCase;
		nExtreRareEThisCase = rData.nExtreRareEThisCase;
		nExtreRareESplcCase = rData.nExtreRareESplcCase;
		//Init New Data
		nBldCategory = 0;
	}
};


#define HASHSIZEISGB 1


///////////////////////////////////////////////////////////
#define T_ISLY_K unsigned int
struct T_ISLY_D
{
	int nNodeLayer;	//0:隔震层 1:隔震层上层
	CArray<T_NODE_K, T_NODE_K> aIsoLayerNode;		//隔震层节点
	CArray<T_NODE_K, T_NODE_K> aIsoUpLayerNode;		//隔震层上层节点
	CArray<T_NLNK_K, T_NLNK_K> aIsoNlnk;		//隔震支座列表

	void Initialize()
	{
		nNodeLayer = 0;
		aIsoLayerNode.RemoveAll();
		aIsoUpLayerNode.RemoveAll();
		aIsoNlnk.RemoveAll();
	}

	T_ISLY_D() { Initialize(); }
	T_ISLY_D(const T_ISLY_D& src) { T_ISLY_D(); *this = src; }
	T_ISLY_D& T_ISLY_D::operator=(const T_ISLY_D &src)
	{
		nNodeLayer = src.nNodeLayer;
		aIsoLayerNode.Copy(src.aIsoLayerNode);
		aIsoUpLayerNode.Copy(src.aIsoUpLayerNode);
		aIsoNlnk.Copy(src.aIsoNlnk);
		return *this;
	}
};
struct T_ISLY_UDRD_D
{
	T_ISLY_D data;
};
struct T_ISLY_UNIT
{
	T_ISLY_UNIT()
	{
		nNodeLayer = D_UNITSYS_NONE;
	}
	int nNodeLayer;


};
struct T_ISLY_UDRD_D_RW
{
	int nNodeLayer;	//0:隔震层 1:隔震层上层
	int nIsoLayerNode;		
	int nIsoUpLayerNode;	
	int nIsoNlnk;

	void GetIsly(T_ISLY_D &rData)
	{
		rData.nNodeLayer = nNodeLayer;
	}
	void SetIsly(const T_ISLY_D &rData)
	{
		nNodeLayer = rData.nNodeLayer;

		nIsoLayerNode = (int)rData.aIsoLayerNode.GetSize();
		nIsoUpLayerNode = (int)rData.aIsoUpLayerNode.GetSize();
		nIsoNlnk = (int)rData.aIsoNlnk.GetSize();
	}
};
#define HASHSIZEISLY 1
#pragma pack(pop)
#endif 


