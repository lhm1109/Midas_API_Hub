#ifndef __DB_ST_RT_DB_H__
#define __DB_ST_RT_DB_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <math.h>

#include "../wg_base/Profiler.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#include "HeaderPre.h"
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#pragma pack(1)

#define DISP_SIZE 7
#define REAC_SIZE 7
#define BTPF_SIZE 18

enum { DOF_FX=0, DOF_FY, DOF_FZ, DOF_MX, DOF_MY, DOF_MZ };

// for post-processor

// load
#define T_CFML_KEY unsigned int
union T_CFML_K
{
	T_CFML_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_CFML_D
{
	T_STLD_K LoadCaseKey;
	int LoadType;         // 1=집중하중, 2=집중모멘트, 3=분포하중, 4=분포모멘트
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ
	double DistanceRatio[2];  // 0.에서 1. 사이의 값
	double Force_Type[4][2];  // LoadType 별 Load
	void Initialize()
	{
		memset(this, 0, sizeof(T_CFML_D));
		//LoadCaseKey=0;
		//LoadType=0;
		//LoadDirection=0;
		//DistanceRatio[0]=0.;
		//DistanceRatio[1]=0.;
		//for(int i=0; i<4; i++)
		//{
		//  Force_Type[i][0]=0.;
		//  Force_Type[i][1]=0.;
		//}
	}
};
struct T_CFML_UNIT
{
	T_CFML_UNIT()
	{
		LoadCaseKey=D_UNITSYS_NONE;
		LoadType=D_UNITSYS_NONE;
		LoadDirection=D_UNITSYS_NONE;
		DistanceRatio=D_UNITSYS_NONE;
		Force_Type[0]=D_UNITSYS_BASE_FORCE;
		Force_Type[1]=D_UNITSYS_BASE_MOMENT;
		Force_Type[2]=D_UNITSYS_BASE_UNITFORCE;
		Force_Type[3]=D_UNITSYS_BASE_UNITMOMENT;
	}
	int LoadCaseKey;
	int LoadType;
	int LoadDirection;
	int DistanceRatio;
	int Force_Type[4];
};

// offset
#define T_CFMO_K T_ELEM_K

struct T_CFMO_D_690
{
	double dLength;
	int nOffsetType;
	double dOffset[6];
	void Initialize()
	{
		memset(this, 0, sizeof(T_CFMO_D_690));
		//dLength=0.;
		//nOffsetType=0;
		//for(int i=0; i<6; i++)dOffset[i]=0;
	}
};

struct T_CFMO_D
{
	double dLength;
	int nOffsetType;
	double dOffset[6];
	double dRigidFactor;
	void Initialize()
	{
		memset(this, 0, sizeof(T_CFMO_D));
		
		//dLength=0.;
		//nOffsetType=0;
		//for(int i=0; i<6; i++)dOffset[i]=0;
	}
	void ConvertBy690(T_CFMO_D_690 &rData)
	{
		dLength     = rData.dLength;
		nOffsetType = rData.nOffsetType;
		memcpy(dOffset,rData.dOffset,sizeof(dOffset));
		dRigidFactor = 0.0;
	}
};



struct T_CFMO_UNIT
{
	T_CFMO_UNIT()
	{
		dLength=D_UNITSYS_BASE_LENGTH;
		nOffsetType=D_UNITSYS_NONE;
		dOffset=D_UNITSYS_BASE_LENGTH;
	}
	int dLength;
	int nOffsetType;
	int dOffset;
};

// displacement
struct T_DISP_D
{
	double dblDisp[DISP_SIZE];
	void Initialize()
	{
		memset(this, 0, sizeof(T_DISP_D));
	}
	BOOL ExistNaN()
	{
		for (int i = 0; i < DISP_SIZE; i++)
			if (std::isnan(dblDisp[i])) return TRUE;

		return FALSE;
	}
	void Max(T_DISP_D& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)
			if(Data.dblDisp[i] > dblDisp[i])dblDisp[i]=Data.dblDisp[i];
	}
	void Min(T_DISP_D& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)
			if(Data.dblDisp[i] < dblDisp[i])dblDisp[i]=Data.dblDisp[i];
	}
	void AbsMax(T_DISP_D& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)dblDisp[i]=__max(fabs(dblDisp[i]), fabs(Data.dblDisp[i]));
	}
	void AbsMax(T_DISP_D& Data1, T_DISP_D& Data2)
	{
		for(int i=0; i<DISP_SIZE; i++)dblDisp[i]=__max(fabs(Data1.dblDisp[i]), fabs(Data2.dblDisp[i]));
	}
	void AbsMaxwithSign(T_DISP_D& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)dblDisp[i]=(fabs(dblDisp[i]) >= fabs(Data.dblDisp[i]) ? dblDisp[i] : Data.dblDisp[i]);
	}
	void AbsMaxwithSign(T_DISP_D& Data1, T_DISP_D& Data2)
	{
		for(int i=0; i<DISP_SIZE; i++)dblDisp[i]=(fabs(Data1.dblDisp[i]) >= fabs(Data2.dblDisp[i]) ? Data1.dblDisp[i] : Data2.dblDisp[i]);
	}
	void Add(T_DISP_D& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)dblDisp[i]+=Data.dblDisp[i];
	}
	void Minus(T_DISP_D& Data)
	{
		for (int i = 0; i < DISP_SIZE; i++)dblDisp[i] -= Data.dblDisp[i];
	}
	void Abs()
	{
		for(int i=0; i<DISP_SIZE; i++)dblDisp[i]=fabs(dblDisp[i]);
	}
	void Sqr()
	{
		for(int i=0; i<DISP_SIZE; i++)dblDisp[i]=dblDisp[i]*dblDisp[i];
	}
	void Root()
	{
		for(int i=0; i<DISP_SIZE; i++)dblDisp[i]=sqrt(dblDisp[i]);
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<DISP_SIZE; i++)dblDisp[i]*=dFactor;
	}
};
struct T_DISP_UNIT
{
	T_DISP_UNIT()
	{
		nCount=DISP_SIZE;
		nUnit[0]=D_UNITSYS_BASE_LENGTH;
		nUnit[1]=D_UNITSYS_BASE_LENGTH;
		nUnit[2]=D_UNITSYS_BASE_LENGTH;
		nUnit[3]=D_UNITSYS_BASE_RADIAN;
		nUnit[4]=D_UNITSYS_BASE_RADIAN;
		nUnit[5]=D_UNITSYS_BASE_RADIAN;
		if(DISP_SIZE==7) nUnit[6]=D_UNITSYS_BASE_RADIAN_L;
		for(int i=0; i<nCount; i++)dblDisp[i]=nUnit[i];
	}
	int dblDisp[DISP_SIZE];
	int nCount;
	int nUnit[DISP_SIZE];
};

struct T_RNCD_D
{
	double dblRncd[DISP_SIZE];

	void Initialize()
	{
		for(int i=0; i<DISP_SIZE; i++)dblRncd[i]=0.;
	}
	T_RNCD_D() { Initialize(); }
	
	T_RNCD_D& operator=(const T_DISP_D& src)
	{
		for (int i=0; i<DISP_SIZE; ++i)  dblRncd[i] = src.dblDisp[i];
		return *this;
	}

	void Max(T_RNCD_D& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)
			if(Data.dblRncd[i] > dblRncd[i])dblRncd[i]=Data.dblRncd[i];
	}
	void Min(T_RNCD_D& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)
			if(Data.dblRncd[i] < dblRncd[i])dblRncd[i]=Data.dblRncd[i];
	}
	void AbsMax(T_RNCD_D& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)dblRncd[i]=__max(fabs(dblRncd[i]), fabs(Data.dblRncd[i]));
	}
	void AbsMax(T_RNCD_D& Data1, T_RNCD_D& Data2)
	{
		for(int i=0; i<DISP_SIZE; i++)dblRncd[i]=__max(fabs(Data1.dblRncd[i]), fabs(Data2.dblRncd[i]));
	}
	void AbsMaxwithSign(T_RNCD_D& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)dblRncd[i]=(fabs(dblRncd[i]) >= fabs(Data.dblRncd[i]) ? dblRncd[i] : Data.dblRncd[i]);
	}
	void AbsMaxwithSign(T_RNCD_D& Data1, T_RNCD_D& Data2)
	{
		for(int i=0; i<DISP_SIZE; i++)dblRncd[i]=(fabs(Data1.dblRncd[i]) >= fabs(Data2.dblRncd[i]) ? Data1.dblRncd[i] : Data2.dblRncd[i]);
	}
	void Add(T_RNCD_D& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)dblRncd[i]+=Data.dblRncd[i];
	}
	void Abs()
	{
		for(int i=0; i<DISP_SIZE; i++)dblRncd[i]=fabs(dblRncd[i]);
	}
	void Sqr()
	{
		for(int i=0; i<DISP_SIZE; i++)dblRncd[i]=dblRncd[i]*dblRncd[i];
	}
	void Root()
	{
		for(int i=0; i<DISP_SIZE; i++)dblRncd[i]=sqrt(dblRncd[i]);
	}
	void Mult(double dFactor)
	{
		for(int i=0; i<DISP_SIZE; i++)dblRncd[i]*=dFactor;
	}
	void SignRecovery(BOOL bRecovery=FALSE)
	{
		if(bRecovery) return; // 해석결과 자체가 부호재생이 된 결과이다.
		for(int i=0; i<DISP_SIZE; i++) dblRncd[i] = fabs(dblRncd[i]);
	};
};
struct T_RNCD_UNIT
{
	T_RNCD_UNIT()
	{
		nCount=DISP_SIZE;
		nUnit[0]=D_UNITSYS_BASE_LENGTH;
		nUnit[1]=D_UNITSYS_BASE_LENGTH;
		nUnit[2]=D_UNITSYS_BASE_LENGTH;
		nUnit[3]=D_UNITSYS_BASE_RADIAN;
		nUnit[4]=D_UNITSYS_BASE_RADIAN;
		nUnit[5]=D_UNITSYS_BASE_RADIAN;
		if(DISP_SIZE==7) nUnit[6]=D_UNITSYS_BASE_RADIAN_L;
	}
	int nCount;
	int nUnit[DISP_SIZE];
};

// Time History Displacement
struct T_DISP_TH_D
{
	T_DISP_D maxDis;      // 변위
	T_DISP_D minDis;
	double dMaxDisTime[DISP_SIZE];
	double dMinDisTime[DISP_SIZE];
	T_DISP_D maxVel;      // 속도
	T_DISP_D minVel;
	double dMaxVelTime[DISP_SIZE];
	double dMinVelTime[DISP_SIZE];
	T_DISP_D maxAcc;      // 상대 가속도 
	T_DISP_D minAcc;
	double dMaxTimeAcc[DISP_SIZE];
	double dMinTimeAcc[DISP_SIZE];
	T_DISP_D maxAAc;      // 절대 가속도
	T_DISP_D minAAc;
	double dMaxAAcTime[DISP_SIZE];
	double dMinAAcTime[DISP_SIZE];
	void Initialize()
	{
		memset(this, 0, sizeof(T_DISP_TH_D));
	}
};

// Time History Ldsplacement(Local)
struct T_LDSP_TH_D
{
	double dNodeID; // Node의 Serial Number임.. 솔버에서 찍어주긴 하는데.. 쓰지는 않음
	double maxDis[6]; // Dx, Dy, Dz, Rx, Ry, Rz
	double minDis[6]; // Dx, Dy, Dz, Rx, Ry, Rz

	void Initialize()
	{
		memset(this, 0, sizeof(T_LDSP_TH_D));
	}
};

// Response Spectrum displacement(Local)
struct T_LDSP_RS_D
{
	double dNodeID; // Node의 Serial Number임.. 솔버에서 찍어주긴 하는데.. 쓰지는 않음
	double dblDisp[6]; // Dx, Dy, Dz, Rx, Ry, Rz

	void Initialize()
	{
		memset(this, 0, sizeof(T_LDSP_RS_D));
	}
	void ConvertOut(T_DISP_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<6; i++)Data.dblDisp[i]=dblDisp[i];
	}

};

struct T_LDSP_RS_D_7DOF
{
	double dNodeID; // Node의 Serial Number임.. 솔버에서 찍어주긴 하는데.. 쓰지는 않음
	double dblDisp[DISP_SIZE]; // Dx, Dy, Dz, Rx, Ry, Rz

	void Initialize()
	{
		memset(this, 0, sizeof(T_LDSP_RS_D_7DOF));
	}
	void ConvertOut(T_DISP_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<DISP_SIZE; i++)Data.dblDisp[i]=dblDisp[i];
	}
};


struct T_LDSP_MV_D
{
	double dNodeID; // Node의 Serial Number임.. 솔버에서 찍어주긴 하는데.. 쓰지는 않음
	double maxDis[6]; // Dx, Dy, Dz, Rx, Ry, Rz
	double minDis[6]; // Dx, Dy, Dz, Rx, Ry, Rz

	void Initialize()
	{
		memset(this, 0, sizeof(T_LDSP_MV_D));
	}
};
struct T_LDSP_MV_D_7DOF
{
	double dNodeID; // Node의 Serial Number임.. 솔버에서 찍어주긴 하는데.. 쓰지는 않음
	double maxDis[DISP_SIZE];
	double minDis[DISP_SIZE];

	void Initialize()
	{
		memset(this, 0, sizeof(T_LDSP_MV_D_7DOF));
	}
};


// mode shape(eigenvalue, buckling)
struct T_FREQ_D
{
	double dblFreq[4];
	void Initialize()
	{
		memset(this, 0, sizeof(T_FREQ_D));
		//for(int i=0; i<4; i++)dblFreq[i]=0.;
	}
};
struct T_FREQ_UNIT
{
	T_FREQ_UNIT()
	{
		nCount=4;
		nUnit[0]=D_UNITSYS_NONE;   //[rad]/[sec]
		nUnit[1]=D_UNITSYS_NONE;   //[cycle]/[sec]
		nUnit[2]=D_UNITSYS_NONE;   //[sec]/[cycle]
		nUnit[3]=D_UNITSYS_NONE;   //error tolerance
		for(int i=0; i<nCount; i++)dblFreq[i]=nUnit[i];
	}
	int dblFreq[4];
	int nCount;
	int nUnit[4];
};

struct _MPAM_BN
{
	double dblMpam[6];  // %
	double dblMpamSum[6];  // %
	void Initialize()
	{
		memset(this, 0, sizeof(_MPAM_BN));
	}
};

struct T_MPAM_D
{
	double dblMpam[7];  // %
	double dblMpamSum[7];  // %
	void Initialize()
	{
		memset(this, 0, sizeof(T_MPAM_D));
	}
	void ConvertToMpam(_MPAM_BN& Data)
	{
		Initialize();
		for(int i=0; i<6; i++)
		{
			dblMpam[i] = Data.dblMpam[i];
			dblMpamSum[i] = Data.dblMpamSum[i];
		}
	}
};

struct T_MPAM_UNIT
{
	T_MPAM_UNIT()
	{
		nCount=12;
		nUnit[0]=D_UNITSYS_NONE;  // %
		nUnit[1]=D_UNITSYS_NONE;  // %
		nUnit[2]=D_UNITSYS_NONE;  // %
		nUnit[3]=D_UNITSYS_NONE;  // %
		nUnit[4]=D_UNITSYS_NONE;  // %
		nUnit[5]=D_UNITSYS_NONE;  // %
		nUnit[6]=D_UNITSYS_NONE;  // %
		nUnit[7]=D_UNITSYS_NONE;  // %
		nUnit[8]=D_UNITSYS_NONE;  // %
		nUnit[9]=D_UNITSYS_NONE;  // %
		nUnit[10]=D_UNITSYS_NONE;  // %
		nUnit[11]=D_UNITSYS_NONE;  // %
		for(int i=0; i<6; i++)dblMpam[i]=nUnit[i];
		for(int j=0; j<6; j++)dblMpamSum[j]=nUnit[j];
	}
	int dblMpam[6];
	int dblMpamSum[6];
	int nCount;
	int nUnit[12];
};

struct _MPAF_BN
{
	double dblMpaf[7];
	void Initialize()
	{
		memset(this, 0, sizeof(_MPAF_BN));
	}
};

struct T_MPAF_D
{
	double dblMpaf[8];
	void Initialize()
	{
		memset(this, 0, sizeof(T_MPAF_D));
	}
	void ConvertToMpaf(_MPAF_BN& Data)
	{
		Initialize();
		for(int i=0; i<7; i++)
		{
			dblMpaf[i] = Data.dblMpaf[i];
		}
	}
};

struct _EMMR_BN
{
	double dblEmmrMass[6];
	double dblEmmrMassSum[6];
	void Initialize()
	{
		memset(this, 0, sizeof(_EMMR_BN));
	}
};


struct T_EMMR_D
{
	double dblEmmrMass[7];
	double dblEmmrMassSum[7];
	void Initialize()
	{
		memset(this, 0, sizeof(T_EMMR_D));
	}
	void ConvertToEmmr(_EMMR_BN& Data)
	{
		Initialize();
		for(int i=0; i<6; i++)
		{
			dblEmmrMass[i] = Data.dblEmmrMass[i];
			dblEmmrMassSum[i] = Data.dblEmmrMassSum[i];
		}
	}
};

struct T_EMMR_UNIT
{
	T_EMMR_UNIT()
	{
		nCount=14;
		for(int i=0; i<3; i++)dblEmmrMass[i]=D_UNITSYS_BASE_MASS;
		for(int i=3; i<7; i++)dblEmmrMass[i]=D_UNITSYS_BASE_ROMASS;
		for(int j=0; j<3; j++)dblEmmrMassSum[j]=D_UNITSYS_BASE_MASS;
		for(int j=3; j<7; j++)dblEmmrMassSum[j]=D_UNITSYS_BASE_ROMASS;
	}
	int dblEmmrMass[7];
	int dblEmmrMassSum[7];
	int nCount;
	int nUnit[14];
};

struct _MDRF_BN
{
	double dblMdrf[6];
	void Initialize()
	{
		memset(this, 0, sizeof(_MDRF_BN));
	}
};

struct T_MDRF_D
{
	double dblMdrf[7];
	void Initialize()
	{
		memset(this, 0, sizeof(T_MDRF_D));
	}
	void ConvertToMdrf(_MDRF_BN& Data)
	{
		Initialize();
		for(int i=0; i<6; i++)
		{
			dblMdrf[i] = Data.dblMdrf[i];
		}
	}
};

struct _MODE_BN
{
	double dblMode[6];
	void Initialize()
	{
		memset(this, 0, sizeof(_MODE_BN));
	}
};

struct T_MODE_D
{
	double dblMode[7];
	void Initialize()
	{
		memset(this, 0, sizeof(T_MODE_D));
	}
	void Add(T_MODE_D &Data)
	{
		for(int i=0; i<7; i++)dblMode[i]+=Data.dblMode[i];
	}
	void ConvertToMode(_MODE_BN& Data)
	{
		Initialize();
		for(int i=0; i<6; i++)
		{
			dblMode[i] = Data.dblMode[i];
		}
	}
};

struct T_MODE_UNIT
{
	T_MODE_UNIT()
	{
		nCount=7;
		nUnit[0]=D_UNITSYS_NONE;
		nUnit[1]=D_UNITSYS_NONE;
		nUnit[2]=D_UNITSYS_NONE;
		nUnit[3]=D_UNITSYS_NONE;
		nUnit[4]=D_UNITSYS_NONE;
		nUnit[5]=D_UNITSYS_NONE;
		nUnit[6]=D_UNITSYS_NONE;
		for(int i=0; i<nCount; i++)dblMode[i]=nUnit[i];
	}
	int dblMode[7];
	int nCount;
	int nUnit[7];
};

struct T_REAC6_D
{
	double dblReac[6];
	void Initialize()
	{
		memset(this, 0, sizeof(T_REAC6_D));
	}
};

// reaction
struct T_REAC_D
{
	double dblReac[REAC_SIZE];
	void Initialize()
	{
		memset(this, 0, sizeof(T_REAC_D));
	}
	void Max(T_REAC_D& Data)
	{
		for(int i=0; i<REAC_SIZE; i++)
			if(Data.dblReac[i] > dblReac[i])dblReac[i]=Data.dblReac[i];
	}
	void Min(T_REAC_D& Data)
	{
		for(int i=0; i<REAC_SIZE; i++)
			if(Data.dblReac[i] < dblReac[i])dblReac[i]=Data.dblReac[i];
	}
	void AbsMax(T_REAC_D& Data)
	{
		for(int i=0; i<REAC_SIZE; i++)dblReac[i]=__max(fabs(dblReac[i]), fabs(Data.dblReac[i]));
	}
	void AbsMax(T_REAC_D& Data1, T_REAC_D& Data2)
	{
		for(int i=0; i<REAC_SIZE; i++)dblReac[i]=__max(fabs(Data1.dblReac[i]), fabs(Data2.dblReac[i]));
	}
	void AbsMaxwithSign(T_REAC_D& Data)
	{
		for(int i=0; i<REAC_SIZE; i++)dblReac[i]=(fabs(dblReac[i]) >= fabs(Data.dblReac[i]) ? dblReac[i] : Data.dblReac[i]);
	}
	void AbsMaxwithSign(T_REAC_D& Data1, T_REAC_D& Data2)
	{
		for(int i=0; i<REAC_SIZE; i++)dblReac[i]=(fabs(Data1.dblReac[i]) >= fabs(Data2.dblReac[i]) ? Data1.dblReac[i] : Data2.dblReac[i]);
	}
	void Add(T_REAC_D& Data)
	{
		for(int i=0; i<REAC_SIZE; i++)dblReac[i]+=Data.dblReac[i];
	}
	void Minus(T_REAC_D& Data)
	{
		for (int i = 0; i < REAC_SIZE; i++)dblReac[i] -= Data.dblReac[i];
	}
	void Abs()
	{
		for(int i=0; i<REAC_SIZE; i++)dblReac[i]=fabs(dblReac[i]);
	}
	void Sqr()
	{
		for(int i=0; i<REAC_SIZE; i++)dblReac[i]=dblReac[i]*dblReac[i];
	}
	void Root()
	{
		for(int i=0; i<REAC_SIZE; i++)dblReac[i]=sqrt(dblReac[i]);
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<REAC_SIZE; i++)dblReac[i]*=dFactor;
	}
};
struct T_REAC_UNIT
{
	T_REAC_UNIT()
	{
		nCount=REAC_SIZE;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;
		nUnit[3]=D_UNITSYS_BASE_MOMENT;
		nUnit[4]=D_UNITSYS_BASE_MOMENT;
		nUnit[5]=D_UNITSYS_BASE_MOMENT;
		if(REAC_SIZE==7) nUnit[6]=D_UNITSYS_BASE_DISTSTIFF;
		for(int i=0; i<nCount; i++)dblReac[i]=nUnit[i];
	}
	int dblReac[REAC_SIZE];
	int nCount;
	int nUnit[REAC_SIZE];
};

// reaction
struct T_REAC_SSPS_D
{
	double dForce[5];
	double dDisplacement[5];
	double dElement;
	double dDirection;
	double dElementType;

	void Initialize()
	{
		memset(this, 0, sizeof(T_REAC_SSPS_D));
	}
	void Max(T_REAC_SSPS_D& Data)
	{
		for(int i=0; i<5; i++) {
			if(Data.dForce[i] > dForce[i])dForce[i]=Data.dForce[i];
			if(Data.dDisplacement[i] > dDisplacement[i])dDisplacement[i]=Data.dDisplacement[i];
		}
	}
	void Min(T_REAC_SSPS_D& Data)
	{
		for(int i=0; i<5; i++) {
			if(Data.dForce[i] < dForce[i])dForce[i]=Data.dForce[i];
			if(Data.dDisplacement[i] < dDisplacement[i])dDisplacement[i]=Data.dDisplacement[i];
		}
	}
	void AbsMax(T_REAC_SSPS_D& Data)
	{
		for(int i=0; i<5; i++) {
			dForce[i]=__max(fabs(dForce[i]), fabs(Data.dForce[i]));
			dDisplacement[i]=__max(fabs(dDisplacement[i]), fabs(Data.dDisplacement[i]));
		}
	}
	void AbsMax(T_REAC_SSPS_D& Data1, T_REAC_SSPS_D& Data2)
	{
		for(int i=0; i<5; i++) {
			dForce[i]=__max(fabs(Data1.dForce[i]), fabs(Data2.dForce[i]));
			dDisplacement[i]=__max(fabs(Data1.dDisplacement[i]), fabs(Data2.dDisplacement[i]));
		}
	}
	void AbsMaxwithSign(T_REAC_SSPS_D& Data)
	{
		for(int i=0; i<5; i++) {
			dForce[i]=(fabs(dForce[i]) >= fabs(Data.dForce[i]) ? dForce[i] : Data.dForce[i]);
			dDisplacement[i]=(fabs(dDisplacement[i]) >= fabs(Data.dDisplacement[i]) ? dDisplacement[i] : Data.dDisplacement[i]);
		}
	}
	void AbsMaxwithSign(T_REAC_SSPS_D& Data1, T_REAC_SSPS_D& Data2)
	{
		for(int i=0; i<5; i++) {
			dForce[i]=(fabs(Data1.dForce[i]) >= fabs(Data2.dForce[i]) ? Data1.dForce[i] : Data2.dForce[i]);
			dDisplacement[i]=(fabs(Data1.dDisplacement[i]) >= fabs(Data2.dDisplacement[i]) ? Data1.dDisplacement[i] : Data2.dDisplacement[i]);
		}
	}
	void Add(T_REAC_SSPS_D& Data)
	{
		for(int i=0; i<5; i++) {
			dForce[i]+=Data.dForce[i];
			dDisplacement[i]+=Data.dDisplacement[i];
		}
	}
	void Abs()
	{
		for(int i=0; i<5; i++) {
			dForce[i]=fabs(dForce[i]);
			dDisplacement[i]=fabs(dDisplacement[i]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++) {
			dForce[i]=dForce[i]*dForce[i];
			dDisplacement[i]=dDisplacement[i]*dDisplacement[i];
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++) {
			dForce[i]=sqrt(dForce[i]);
			dDisplacement[i]=sqrt(dDisplacement[i]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++) {
			dForce[i]*=dFactor;
			dDisplacement[i]*=dFactor;
		}
	}
};

// truss
struct __MY_EXT_CLASS__ T_STRT_D
{
// dblForce     : local force,  component : Fxi, Fxj
// dblStress    : local stress, component : Sxi, Sxj
	double dblForce[2];
	double dblStress[2];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRT_D));
		//memset(dblForce,0,sizeof(dblForce));
		//memset(dblStress,0,sizeof(dblStress));
	}
	void Max(T_STRT_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			if(Data.dblForce[i] > dblForce[i])dblForce[i]=Data.dblForce[i];
			if(Data.dblStress[i] > dblStress[i])dblStress[i]=Data.dblStress[i];
		}
	}
	void Min(T_STRT_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			if(Data.dblForce[i] < dblForce[i])dblForce[i]=Data.dblForce[i];
			if(Data.dblStress[i] < dblStress[i])dblStress[i]=Data.dblStress[i];
		}
	}
	void AbsMax(T_STRT_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			dblForce[i]=__max(fabs(dblForce[i]), fabs(Data.dblForce[i]));
			dblStress[i]=__max(fabs(dblStress[i]), fabs(Data.dblStress[i]));
		}
	}
	void AbsMax(T_STRT_D& Data1, T_STRT_D& Data2)
	{
		for(int i=0; i<2; i++)
		{
			dblForce[i]=__max(fabs(Data1.dblForce[i]), fabs(Data2.dblForce[i]));
			dblStress[i]=__max(fabs(Data1.dblStress[i]), fabs(Data2.dblStress[i]));
		}
	}
	void AbsMaxwithSign(T_STRT_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			dblForce[i]=(fabs(dblForce[i]) >= fabs(Data.dblForce[i]) ? dblForce[i] : Data.dblForce[i]);
			dblStress[i]=(fabs(dblStress[i]) >= fabs(Data.dblStress[i]) ? dblStress[i] : Data.dblStress[i]);
		}
	}
	void AbsMaxwithSign(T_STRT_D& Data1, T_STRT_D& Data2)
	{
		for(int i=0; i<2; i++)
		{
			dblForce[i]=(fabs(Data1.dblForce[i]) >= fabs(Data2.dblForce[i]) ? Data1.dblForce[i] : Data2.dblForce[i]);
			dblStress[i]=(fabs(Data1.dblStress[i]) >= fabs(Data2.dblStress[i]) ? Data1.dblStress[i] : Data2.dblStress[i]);
		}
	}
	void Add(T_STRT_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			dblForce[i]+=Data.dblForce[i];
			dblStress[i]+=Data.dblStress[i];
		}
	}
	void Abs()
	{
		for(int i=0; i<2; i++)
		{
			dblForce[i]=fabs(dblForce[i]);
			dblStress[i]=fabs(dblStress[i]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<2; i++)
		{
			dblForce[i]=dblForce[i]*dblForce[i];
			dblStress[i]=dblStress[i]*dblStress[i];
		}
	}
	void Root()
	{
		for(int i=0; i<2; i++)
		{
			dblForce[i]=sqrt(dblForce[i]);
			dblStress[i]=sqrt(dblStress[i]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<2; i++)
		{
			dblForce[i]*=dFactor;
			dblStress[i]*=dFactor;
		}
	}
};
struct T_STRT_UNIT
{
	T_STRT_UNIT()
	{
		nCount=4;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_STRESS;
		nUnit[3]=D_UNITSYS_BASE_STRESS;
		for(int i=0; i<2; i++)dblForce[i]=nUnit[i];
		for(int i = 0; i<2; i++)dblStress[i]=nUnit[i+2];
	}
	int dblForce[2];
	int dblStress[2];
	int nCount;
	int nUnit[4];
};


// Concurrent Joint Force
struct T_CJFF_D
{
	BOOL bConcur;  // 동시발생 부재력
	int nSize;  // 결과 개수
	int nElem;  // 절점 or 요소 개수
	int nDof;   // 자유도 수
	CArray<double, double> adForce;
	void Initialize()
	{
		bConcur = FALSE;
		nSize = 0;
		nElem = 0;
		nDof = 0;
		adForce.RemoveAll();
	}
	void InitializeZero(int nrSize)
	{
		nSize = nrSize;
		for (int i = 0; i < nSize; i++)adForce.Add(0.0);
	}

	T_CJFF_D() { Initialize(); }
	T_CJFF_D(T_CJFF_D& src) { *this = src; }
	T_CJFF_D& operator=(const T_CJFF_D& src)
	{
		bConcur = src.bConcur;
		nSize = src.nSize;
		nElem = src.nElem;
		nDof = src.nDof;
		adForce.Copy(src.adForce);
		return *this;
	}

	void Max(T_CJFF_D& Data);
	void Min(T_CJFF_D& Data);
	void AbsMax(T_CJFF_D& Data1, T_CJFF_D& Data2);
	void AbsMaxwithSign(T_CJFF_D& Data1, T_CJFF_D& Data2);
	void Add(T_CJFF_D& Data);
	void Abs();
	void Sqr();
	void Root();
	void Mult(double dFactor);

// 	void Max(T_CJFF_D& Data)
// 	{
// 		for (int i = 0; i < nSize; i++)
// 			if (Data.adForce[i] > adForce[i])adForce[i] = Data.adForce[i];
// 	}
// 	void Min(T_CJFF_D& Data)
// 	{
// 		for (int i = 0; i < nSize; i++)
// 			if (Data.adForce[i] < adForce[i])adForce[i] = Data.adForce[i];
// 	}
// 	void AbsMax(T_CJFF_D& Data)
// 	{
// 		for (int i = 0; i < nSize; i++)adForce[i] = __max(fabs(adForce[i]), fabs(Data.adForce[i]));
// 	}
// 	void AbsMax(T_CJFF_D& Data1, T_CJFF_D& Data2)
// 	{
// 		for (int i = 0; i < nSize; i++)adForce[i] = __max(fabs(Data1.adForce[i]), fabs(Data2.adForce[i]));
// 	}
// 	void AbsMaxwithSign(T_CJFF_D& Data)
// 	{
// 		for (int i = 0; i < nSize; i++)adForce[i] = (fabs(adForce[i]) >= fabs(Data.adForce[i]) ? adForce[i] : Data.adForce[i]);
// 	}
// 	void AbsMaxwithSign(T_CJFF_D& Data1, T_CJFF_D& Data2)
// 	{
// 		for (int i = 0; i < nSize; i++)adForce[i] = (fabs(Data1.adForce[i]) >= fabs(Data2.adForce[i]) ? Data1.adForce[i] : Data2.adForce[i]);
// 	}
// 	void Add(T_CJFF_D& Data)
// 	{
// 		for (int i = 0; i < nSize; i++)adForce[i] += Data.adForce[i];
// 	}
// 	void Abs()
// 	{
// 		for (int i = 0; i < nSize; i++)adForce[i] = fabs(adForce[i]);
// 	}
// 	void Sqr()
// 	{
// 		for (int i = 0; i < nSize; i++)adForce[i] = adForce[i] * adForce[i];
// 	}
// 	void Root()
// 	{
// 		for (int i = 0; i < nSize; i++)adForce[i] = sqrt(adForce[i]);
// 	}
// 	void Mult(double dFactor)
// 	{
// 		if (dFactor == 1.)return;
// 		for (int i = 0; i < nSize; i++)adForce[i] *= dFactor;
// 	}
};

struct T_CJFF_UNIT
{
	T_CJFF_UNIT()
	{
		nUnit[0] = D_UNITSYS_BASE_FORCE;
		nUnit[1] = D_UNITSYS_BASE_FORCE;
		nUnit[2] = D_UNITSYS_BASE_FORCE;
		nUnit[3] = D_UNITSYS_BASE_MOMENT;
		nUnit[4] = D_UNITSYS_BASE_MOMENT;
		nUnit[5] = D_UNITSYS_BASE_MOMENT;
		for (int i = 0; i < nSize; i++)adForce[i] = nUnit[i];
	}
	int nSize;
	int adForce[REAC_SIZE];
	int nUnit[REAC_SIZE];
};


// Time HIstory Concurrent  Displ/Vel./Acc 
struct T_CDVA_D
{
	CArray<double, double> adValue;
	void Initialize()
	{
		adValue.RemoveAll();
	}

	T_CDVA_D() { Initialize(); }
	T_CDVA_D(T_CDVA_D& src) { *this = src; }
	T_CDVA_D& operator=(const T_CDVA_D& src)
	{
		adValue.Copy(src.adValue);
		return *this;
	}
};

// beam
struct __MY_EXT_CLASS__ T_STRB_D
{
// dblForce	: local force         , 4 part,
//          component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj, (12개)
//											biMi, biMj, STVenantTorsion i, STVenantTorsion j, WarpingTorsion i, WarpingTorsion j (for 7th DOF : 6개)
// dblStress: local stress        , 4 part, 
//          component : Sxi, Syi, Szi, Bypi, Bymi, Bzpi, Bzmi, Sxj, Syj, Szj, Bypj, Bymj, Bzpj, Bzmj, (14개)
//											Combi, Combj, Comb1i, Comb1j .... Comb4i, Comb4j (10개)
//											warping normal stress(1,2,3,4) i, ST. Venant torsional stress (xy1, xz1, xy2, xz2..xy4,xz4) i, warping torsional stress(xy1, xz1, xy2, xz2..xy4,xz4) i, (for 7th DOF : 20개)
//											warping normal stress(1,2,3,4) j, ST. Venant torsional stress (xy1, xz1, xy2, xz2..xy4,xz4) j, warping torsional stress(xy1, xz1, xy2, xz2..xy4,xz4) j, (for 7th DOF : 20개)
//											CombTau1i, CombTau1j .... CombTau8i, CombTau8j (for 7th DOF : 16개)

	double dblForce[4][18];
	double dblStress[4][80];
	int nDOFType; // 1=6th DOF, 2=7th DOF
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRB_D));
		nDOFType = 1;
	}
	void Max(T_STRB_D& Data);
	void Min(T_STRB_D& Data);
	void AbsMax(T_STRB_D& Data);
	void AbsMax(T_STRB_D& Data1, T_STRB_D& Data2);
	void AbsMaxwithSign(T_STRB_D& Data);
	void AbsMaxwithSign(T_STRB_D& Data1, T_STRB_D& Data2);
	void Add(T_STRB_D& Data);
	void Abs();
	void Sqr();
	void Root();
	void Mult(double dFactor);
	void calcMaximumCombinedStress();
	double GetMaximumStress_Warping(int part, int node);
	double GetMaximumStress_Torsional(int part, int node, int nType);
	double GetMaximumCombinedStress_Torsional(int part, int node, int nType);
};
struct T_STRB_UNIT
{
	T_STRB_UNIT()
	{
		nCount=36;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;
		nUnit[3]=D_UNITSYS_BASE_MOMENT;
		nUnit[4]=D_UNITSYS_BASE_MOMENT;
		nUnit[5]=D_UNITSYS_BASE_MOMENT;
		nUnit[6]=D_UNITSYS_BASE_FORCE;
		nUnit[7]=D_UNITSYS_BASE_FORCE;
		nUnit[8]=D_UNITSYS_BASE_FORCE;
		nUnit[9]=D_UNITSYS_BASE_MOMENT;
		nUnit[10]=D_UNITSYS_BASE_MOMENT;
		nUnit[11]=D_UNITSYS_BASE_MOMENT;
		for(int i=12; i<nCount; i++)nUnit[i]=D_UNITSYS_BASE_STRESS;

		for(int i = 0; i<12; i++)dblForce[i]=nUnit[i];
		dblForce[12]=D_UNITSYS_BASE_DISTSTIFF;
		dblForce[13]=D_UNITSYS_BASE_DISTSTIFF;
		dblForce[14]=D_UNITSYS_BASE_MOMENT;
		dblForce[15]=D_UNITSYS_BASE_MOMENT;
		dblForce[16]=D_UNITSYS_BASE_MOMENT;
		dblForce[17]=D_UNITSYS_BASE_MOMENT;

		dblStress = D_UNITSYS_BASE_STRESS;
	}
	int dblForce[18];
	int dblStress;
	int nCount;
	int nUnit[36];
};

// Self Constraint Beam Force &Stress
struct T_SCFP_D
{
	// dbForce	: local force         , 5 part,
	//          component : Axial, Moment-y, Moment-z (3개)
	// dbStress: local stress        , 5 part, 
	//          component : Axial, Moment-y top, Moment-y Bot, Moment-z top, Moment-z Bot, Cb(Min/Max), Center, Stress Point 1, Stress Point 2, Stress Point 3, Stress Point 4 (10개) 
	// dbLStress: local stress        , 5 part, 
	//          component : Axial, Moment-y 응력 9개, Moment-z 응력 9개 (19개)
	
	double dbForce[5][3];
	double dbStress[5][11];
	double dbLStress[5][19];
	
	void Initialize()
	{
		memset(this, 0, sizeof(T_SCFP_D));
	}
	void Max(T_SCFP_D& Data);
	void Min(T_SCFP_D& Data);
	void AbsMax(T_SCFP_D& Data);
	void AbsMax(T_SCFP_D& Data1, T_SCFP_D& Data2);
	void AbsMaxwithSign(T_SCFP_D& Data);
	void AbsMaxwithSign(T_SCFP_D& Data1, T_SCFP_D& Data2);
	void Add(T_SCFP_D& Data);
	void Abs();
	void Sqr();
	void Root();
	void Mult(double dFactor);
	void calcMaximumCombinedStress();
};
struct T_SCFP_UNIT
{
	T_SCFP_UNIT()
	{
		dbForce[0] = D_UNITSYS_BASE_FORCE;
		dbForce[1] = D_UNITSYS_BASE_MOMENT;
		dbForce[2] = D_UNITSYS_BASE_MOMENT;
		dbStress = D_UNITSYS_BASE_STRESS;
	}
	int dbForce[3];
	int dbStress;
};

struct __MY_EXT_CLASS__ T_SBCF_D
{
// dblForce     : local force         , 4 part,
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi, Bii, Sti, Wti,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj, Bij, Stj, Wtj
// dblStress    : local stress        , 4 part, 
//                component : Sxi, Syi, Szi, Bypi, Bymi, Bzpi, Bzmi, Sxj, Syj, Szj, Bypj, Bymj, Bzpj, Bzmj,
//                            Combi, Combj, Comb1i, Comb1j .... Comb4i, Comb4j
	double dblForce[4][2][9][9];      // 4 part / i,j /[9]  Fx, Fy, Fz, Mx, My, Mz, Bi, St, Wt
	double dblStress[4][2][9][40];    // 4 part / i,j /[9]  Fx, Fy, Fz, Mx, My, Mz, Bi, St, Wt  => (bStressTable=TRUE) [7] : Sx, Sy, Sz, Byp, Bym, Bzp, Bzms 
									  //              /[40] Sx, Sy, Sz, Byp, Bym, Bzp, Bzm, Comb, Comb1, Comb2, Comb3, Comb4
									  //				    warping normal stress(1,2,3,4), ST. Venant torsional stress (xy1, xz1, xy2, xz2..xy4,xz4),warping torsional stress(xy1, xz1, xy2, xz2..xy4,xz4)
									  //				    CombTau1, CombTau2, ... CombTau
	double dblStress2[4][2][11][12];  // 4 part / i,j /[11] Sx, Sy, Sz, Byp, Bym, Bzp, Bzm,       Comb1, Comb2, Comb3, Comb4
									  //			  /[12] Sx, Sy, Sz, Byp, Bym, Bzp, Bzm, Comb, Comb1, Comb2, Comb3, Comb4
	//BOOL b7thDOF; //SBCF의 max,min 계산위함.
	BOOL bStressTable;  // //SBCF의 View by Max value stresses 테이블 출력용.

	void Initialize()
	{
		memset(this, 0, sizeof(T_SBCF_D));
		//b7thDOF=FALSE;
		bStressTable = FALSE;
	}
	void Max(T_SBCF_D& Data);
	void Min(T_SBCF_D& Data);
	void MaxMod(T_SBCF_D& Data);
	void MinMod(T_SBCF_D& Data);
	void ModSbcfStress(T_SBCF_D& Data);
	//void AbsMax(T_SBCF_D& Data);
	void AbsMax(T_SBCF_D& Data1, T_SBCF_D& Data2);
	//void AbsMaxwithSign(T_SBCF_D& Data);
	void AbsMaxwithSign(T_SBCF_D& Data1, T_SBCF_D& Data2);
	void Add(T_SBCF_D& Data);
	void Abs();
	void Sqr();
	void Root();
	void Mult(double dFactor);
	void SetCombinedStress();
	void calcMaximumCombinedStress();
	void ConvertToSbcf(T_STRB_D& DataStrb);
	void ConvertToStrb(T_STRB_D& StrbD,BOOL bMax);  // Force 만 사용
	static void ConvertToStrbMxMn(T_SBCF_D& SbcfMxD,T_SBCF_D& SbcfMnD,T_STRB_D& StrbMxD,T_STRB_D &StrbMnD);
};
struct T_SBCF_UNIT
{
	T_SBCF_UNIT()
	{
		dblForce[0]=D_UNITSYS_BASE_FORCE;
		dblForce[1]=D_UNITSYS_BASE_FORCE;
		dblForce[2]=D_UNITSYS_BASE_FORCE;
		dblForce[3]=D_UNITSYS_BASE_MOMENT;
		dblForce[4]=D_UNITSYS_BASE_MOMENT;
		dblForce[5]=D_UNITSYS_BASE_MOMENT;
		dblForce[6]=D_UNITSYS_BASE_DISTSTIFF;
		dblForce[7]=D_UNITSYS_BASE_MOMENT;
		dblForce[8]=D_UNITSYS_BASE_MOMENT;

		for(int i=0; i<40; i++)
			dblStress[i]=D_UNITSYS_BASE_STRESS;
	}
	int dblForce[9];
	int dblStress[40];
};

struct __MY_EXT_CLASS__ T_SPCF_DL
{
	// dForce  : local unit length force,             cent + 4 node
	//	         component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

	double dForce[5][8][8];

	void Initialize()
	{
		memset(this, 0, sizeof(T_SPCF_DL));
	}
	void Max(T_SPCF_DL& Data);
	void Min(T_SPCF_DL& Data);
	void AbsMax(T_SPCF_DL& Data);
	void AbsMax(T_SPCF_DL& Data1, T_SPCF_DL& Data2);
	void AbsMaxwithSign(T_SPCF_DL& Data);
	void AbsMaxwithSign(T_SPCF_DL& Data1, T_SPCF_DL& Data2);
	void Add(T_SPCF_DL& Data);
	void Abs();
	void Sqr();
	void Root();
	void Mult(double dFactor);
};
struct T_SPCF_UNIT
{
	T_SPCF_UNIT()
	{
		dForce[0]=D_UNITSYS_BASE_UNITFORCE;
		dForce[1]=D_UNITSYS_BASE_UNITFORCE;
		dForce[2]=D_UNITSYS_BASE_UNITFORCE;
		dForce[3]=D_UNITSYS_BASE_UNITMOMENT;
		dForce[4]=D_UNITSYS_BASE_UNITMOMENT;
		dForce[5]=D_UNITSYS_BASE_UNITMOMENT;
		dForce[6]=D_UNITSYS_BASE_UNITFORCE;
		dForce[7]=D_UNITSYS_BASE_UNITFORCE;
	}
	int dForce[8];
};

// virtual beam
struct T_SIMB_D
{
	// dblForce	: local force         , 1 part,
	//          component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj, (12개)

	double dblForce[12];
	void Initialize()
	{
		memset(this, 0, sizeof(T_SIMB_D));
	}
	void Max(T_SIMB_D& Data)
	{
		for(int j=0; j<12; j++)
		{
			if(Data.dblForce[j] > dblForce[j])dblForce[j]=Data.dblForce[j];
		}
	}
	void Min(T_SIMB_D& Data)
	{
		for(int j=0; j<12; j++)
		{
			if(Data.dblForce[j] < dblForce[j])dblForce[j]=Data.dblForce[j];
		}
	}
	void AbsMax(T_SIMB_D& Data)
	{
		for(int j=0; j<12; j++)
		{
			dblForce[j]=__max(fabs(dblForce[j]),fabs(Data.dblForce[j]));
		}
	}
	void AbsMax(T_SIMB_D& Data1, T_SIMB_D& Data2)
	{
		Initialize();
		for(int j=0; j<12; j++)
		{
			dblForce[j]=__max(fabs(Data1.dblForce[j]),fabs(Data2.dblForce[j]));
		}
	}
	void AbsMaxwithSign(T_SIMB_D& Data)
	{
		for(int j=0; j<12; j++)
		{
			dblForce[j]=(fabs(dblForce[j]) >= fabs(Data.dblForce[j]) ? dblForce[j] : Data.dblForce[j]);
		}
	}
	void AbsMaxwithSign(T_SIMB_D& Data1, T_SIMB_D& Data2)
	{
		Initialize();
		for(int j=0; j<12; j++)
		{
			dblForce[j]=(fabs(Data1.dblForce[j]) >= fabs(Data2.dblForce[j]) ? Data1.dblForce[j] : Data2.dblForce[j]);
		}
	}
	void Add(T_SIMB_D& Data)
	{
		for(int j=0; j<12; j++)
		{
			dblForce[j]+=Data.dblForce[j];
		}
	}
	void Abs()
	{
		for(int j=0; j<12; j++)
		{
			dblForce[j]=fabs(dblForce[j]);
		}
	}
	void Sqr()
	{
		for(int j=0; j<12; j++)
		{
			dblForce[j]=dblForce[j]*dblForce[j];
		}
	}
	void Root()
	{
		for(int j=0; j<12; j++)
		{
			dblForce[j]=sqrt(dblForce[j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int j=0; j<12; j++)
		{
			dblForce[j]*=dFactor;
		}
	}

	void T_SIMB_D::ConvertToStrb(T_STRB_D& StrbD)
	{
		StrbD.Initialize();
		StrbD.nDOFType = 1;

		for (int part = 0; part < 4; part++)
		{
			double ratio_i = part * 0.25;
			double ratio_j = (part + 1) * 0.25;

			for (int j = 0; j < 6; j++)
			{
				StrbD.dblForce[part][j]     = dblForce[j] * (1.0 - ratio_i) + dblForce[j + 6] * ratio_i;
				StrbD.dblForce[part][j + 6] = dblForce[j] * (1.0 - ratio_j) + dblForce[j + 6] * ratio_j;
			}
		}
	}
};


struct T_SIMB_UNIT
{
	T_SIMB_UNIT()
	{
		nCount = 12;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;
		nUnit[3]=D_UNITSYS_BASE_MOMENT;
		nUnit[4]=D_UNITSYS_BASE_MOMENT;
		nUnit[5]=D_UNITSYS_BASE_MOMENT;

		nUnit[6]=D_UNITSYS_BASE_FORCE;
		nUnit[7]=D_UNITSYS_BASE_FORCE;
		nUnit[8]=D_UNITSYS_BASE_FORCE;
		nUnit[9]=D_UNITSYS_BASE_MOMENT;
		nUnit[10]=D_UNITSYS_BASE_MOMENT;
		nUnit[11]=D_UNITSYS_BASE_MOMENT;

		for(int i=0; i<12; i++)dblForce[i]=nUnit[i];
	}
	int dblForce[12];
	int nCount;
	int nUnit[12];
};

// Design Strip 부재력
#define T_PTSB_K unsigned int 
#define T_PTSB_D T_SIMB_D 

// Design Strip Tendon Primary Force
#define T_PTPF_K unsigned int 
#define T_PTPF_D T_SIMB_D 

// Design Strip Tendon Direction Force
#define T_DTFS_K unsigned int 

//
struct __MY_EXT_CLASS__ T_SIMC_D
{
	double dblForce[2][6][6];      // i,j / Fx , Fy, Fz, Mx, My, Mz
	void Initialize()
	{
		memset(this, 0, sizeof(T_SIMC_D));
	}

	void Add(T_SIMC_D& Data);
	void Mult(double dFactor);
	void Max(T_SIMC_D& Data);
	void Min(T_SIMC_D& Data);
	void AbsMax(T_SIMC_D& Data);
	void AbsMax(T_SIMC_D& Data1, T_SIMC_D& Data2);
	void AbsMaxwithSign(T_SIMC_D& Data);
	void AbsMaxwithSign(T_SIMC_D& Data1, T_SIMC_D& Data2);
	void Abs();
	void Sqr();
	void Root();
	void ConvertToSimc(T_SIMB_D& DataSimb);
	void ConvertToSimb(T_SIMB_D& SimbD, BOOL bMax);  // Force 만 사용
	void ConvertToSimbMxMn(T_SIMC_D& SimcMxD, T_SIMC_D& SimcMnD, T_SIMB_D& SimbMxD, T_SIMB_D& SimbMnD);
	void ConvertToSbcf(T_SBCF_D& SbcfD);
};

struct T_SIMC_UNIT
{
	T_SIMC_UNIT()
	{
		dblForce[0]=D_UNITSYS_BASE_FORCE;
		dblForce[1]=D_UNITSYS_BASE_FORCE;
		dblForce[2]=D_UNITSYS_BASE_FORCE;
		dblForce[3]=D_UNITSYS_BASE_MOMENT;
		dblForce[4]=D_UNITSYS_BASE_MOMENT;
		dblForce[5]=D_UNITSYS_BASE_MOMENT;
	}
	int dblForce[6];
};

// virtual beam - STRB type
struct T_FSIM_D
{
	// dblForce	: local force         , 4 part,
	//          component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj, (12개)
	// dblStress: local stress        , 4 part, 
	//          component :	Combi, Combj, Comb1i, Comb1j .... Comb4i, Comb4j (10개)
	double dblForce[4][12];
	double dblStress[4][10];
	void Initialize()
	{
		memset(this, 0, sizeof(T_FSIM_D));
	}
	void Max(T_FSIM_D& Data);
	void Min(T_FSIM_D& Data);
	void AbsMax(T_FSIM_D& Data);
	void AbsMax(T_FSIM_D& Data1, T_FSIM_D& Data2);
	void AbsMaxwithSign(T_FSIM_D& Data);
	void AbsMaxwithSign(T_FSIM_D& Data1, T_FSIM_D& Data2);
	void Add(T_FSIM_D& Data);
	void Abs();
	void Sqr();
	void Root();
	void Mult(double dFactor);
	void calcMaximumCombinedStress();
	void ConvertToStrb(T_STRB_D& StrbD);


};
struct T_FSIM_UNIT
{
	T_FSIM_UNIT()
	{
		dblForce[0] = D_UNITSYS_BASE_FORCE;
		dblForce[1] = D_UNITSYS_BASE_FORCE;
		dblForce[2] = D_UNITSYS_BASE_FORCE;
		dblForce[3] = D_UNITSYS_BASE_MOMENT;
		dblForce[4] = D_UNITSYS_BASE_MOMENT;
		dblForce[5] = D_UNITSYS_BASE_MOMENT;
		dblForce[6] = D_UNITSYS_BASE_FORCE;
		dblForce[7] = D_UNITSYS_BASE_FORCE;
		dblForce[8] = D_UNITSYS_BASE_FORCE;
		dblForce[9] = D_UNITSYS_BASE_MOMENT;
		dblForce[10] = D_UNITSYS_BASE_MOMENT;
		dblForce[11] = D_UNITSYS_BASE_MOMENT;

		dblStress = D_UNITSYS_BASE_STRESS;
	}
	int dblForce[12];
	int dblStress;
};

#define SRIM_IJ     18 
#define SRIM_SIZE   SRIM_IJ*2
struct T_SRIM_D
{
	double dblStress[SRIM_SIZE]; // I:18개 단면지점,J:18개 단면지점
	void Initialize()
	{
		memset(this, 0, sizeof(T_SRIM_D));
	}
	void Max(T_SRIM_D& Data);
	void Min(T_SRIM_D& Data);
	void AbsMax(T_SRIM_D& Data);
	void AbsMax(T_SRIM_D& Data1, T_SRIM_D& Data2);
	void AbsMaxwithSign(T_SRIM_D& Data);
	void AbsMaxwithSign(T_SRIM_D& Data1, T_SRIM_D& Data2);
	void Add(T_SRIM_D& Data);
	void Abs();
	void Sqr();
	void Root();
	void Mult(double dFactor);
};
struct T_SRIM_UNIT
{
	T_SRIM_UNIT()
	{
		dblStress = D_UNITSYS_BASE_STRESS;
	}
	int dblStress;
};

struct T_SBCS_D
{
// dblForce     : local force     4 part, CB1~CB4, 2end(I,J), 
//                component :     Fx , Fy, Fz, Mx, My, Mz, Stress-max(Stress-min), Bi, St, Wt
	double dblForce[4][4][2][10];
	void Initialize()
	{
		memset(this, 0, sizeof(T_SBCS_D));
	}
	void Max(T_SBCS_D& Data);
	void Min(T_SBCS_D& Data);
	void AbsMax(T_SBCS_D& Data);
	void AbsMax(T_SBCS_D& Data1, T_SBCS_D& Data2);
	void AbsMaxwithSign(T_SBCS_D& Data);
	void AbsMaxwithSign(T_SBCS_D& Data1, T_SBCS_D& Data2);
	void Add(T_SBCS_D& Data);
	void Abs();
	void Sqr();
	void Root();
	void Mult(double dFactor);
	void ConvertToSbcs(T_STRB_D& DataStrb);
};
struct T_SBCS_UNIT
{
	T_SBCS_UNIT()
	{
		dblForce[0]=D_UNITSYS_BASE_FORCE;
		dblForce[1]=D_UNITSYS_BASE_FORCE;
		dblForce[2]=D_UNITSYS_BASE_FORCE;
		dblForce[3]=D_UNITSYS_BASE_MOMENT;
		dblForce[4]=D_UNITSYS_BASE_MOMENT;
		dblForce[5]=D_UNITSYS_BASE_MOMENT;
		dblForce[6]=D_UNITSYS_BASE_STRESS;
	}
	int dblForce[7];
};

struct T_BCSF_D
{
// dblForce     : local force     4 part, 2end(I,J), 
//                component :     Fx , Fy, Fz, Mx, My, Mz, Stress-max(Stress-min), Bi, St, Wt
	double dblForce[4][2][10];
	void Initialize()
	{
		memset(this, 0, sizeof(T_BCSF_D));
	}
	void ConvertToBcsfMax(T_SBCS_D& Data);
	void ConvertToBcsfMin(T_SBCS_D& Data);
	void ConvertToBcsfAbsMax(T_SBCS_D& Data1, T_SBCS_D& Data2);
	void ConvertToBcsfAbsMaxwithSign(T_SBCS_D& Data1, T_SBCS_D& Data2);
};

// plane stress
struct __MY_EXT_CLASS__ T_STRM_DL
{
// dblForce     : local force                 , 4 node       ,
//                component : Fx, Fy, Fz
// dblStress    : local stress                , cent + 4 node, 
//                component : Sxx, Syy, Sxy, P1, P2, ANG, EFF, Max-Shear
	double dblForce[4][3];
	double dblStress[5][8];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRM_DL));
		//memset(dblForce,0,sizeof(dblForce));
		//memset(dblStress,0,sizeof(dblStress));
	}
	void Max(T_STRM_DL& Data);
	void Min(T_STRM_DL& Data);
	void AbsMax(T_STRM_DL& Data);
	void AbsMax(T_STRM_DL& Data1, T_STRM_DL& Data2);
	void AbsMaxwithSign(T_STRM_DL& Data, double dZero=1.e-10); // fixed by mylee
	void AbsMaxwithSign(T_STRM_DL& Data1, T_STRM_DL& Data2, double dZero=1.e-10); // fixed by mylee
	void Add(T_STRM_DL& Data);
	void Abs();
	void Sqr();
	void Root();
	void Mult(double dFactor);
	void Multiple(T_STRM_DL Data);
	void calcPrincipalStress();
};
struct T_STRML_UNIT
{
	T_STRML_UNIT()
	{
		nCount=11;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;
		nUnit[3]=D_UNITSYS_BASE_STRESS;
		nUnit[4]=D_UNITSYS_BASE_STRESS;
		nUnit[5]=D_UNITSYS_BASE_STRESS;
		nUnit[6]=D_UNITSYS_BASE_STRESS;
		nUnit[7]=D_UNITSYS_BASE_STRESS;
		nUnit[8]=D_UNITSYS_BASE_DEGREE;
		nUnit[9]=D_UNITSYS_BASE_STRESS;
		nUnit[10]=D_UNITSYS_BASE_STRESS;
		for(int i=0; i<3; i++)dblForce[i]=nUnit[i];
		for(int i = 0; i<8; i++)dblStress[i]=nUnit[i+3];
	}
	int dblForce[3];
	int dblStress[8];
	int nCount;
	int nUnit[11];
};

// plane stress
struct __MY_EXT_CLASS__ T_STRM_DG
{
// dblForce     : global force                 , 4 node       ,
//                component : FX, FY, FZ
// dblStress    : global stress                , cent + 4 node, 
//                component : SXX, SYY, SZZ, SXY, SYZ, SXZ, P1, P2, ANG, EFF, Max-Shear
	double dblForce[4][3];
	double dblStress[5][11];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRM_DG));
		//memset(dblForce,0,sizeof(dblForce));
		//memset(dblStress,0,sizeof(dblStress));
	}
	void Max(T_STRM_DG& Data);
	void Min(T_STRM_DG& Data);
	void AbsMax(T_STRM_DG& Data);
	void AbsMax(T_STRM_DG& Data1, T_STRM_DG& Data2);
	void AbsMaxwithSign(T_STRM_DG& Data, double dZero=1.e-10); // fixed by mylee
	void AbsMaxwithSign(T_STRM_DG& Data1, T_STRM_DG& Data2, double dZero=1.e-10); // fixed by mylee
	void Add(T_STRM_DG& Data);
	void Abs();
	void Sqr();
	void Root();
	void Mult(double dFactor);
	void calcPrincipalStress();
};
struct T_STRMG_UNIT
{
	T_STRMG_UNIT()
	{
		nCount=14;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;
		nUnit[3]=D_UNITSYS_BASE_STRESS;
		nUnit[4]=D_UNITSYS_BASE_STRESS;
		nUnit[5]=D_UNITSYS_BASE_STRESS;
		nUnit[6]=D_UNITSYS_BASE_STRESS;
		nUnit[7]=D_UNITSYS_BASE_STRESS;
		nUnit[8]=D_UNITSYS_BASE_STRESS;
		nUnit[9]=D_UNITSYS_BASE_STRESS;
		nUnit[10]=D_UNITSYS_BASE_STRESS;
		nUnit[11]=D_UNITSYS_BASE_DEGREE;
		nUnit[12]=D_UNITSYS_BASE_STRESS;
		nUnit[13]=D_UNITSYS_BASE_STRESS;
		for(int i=0; i<3; i++)dblForce[i]=nUnit[i];
		for(int i = 0; i<11; i++)dblStress[i]=nUnit[i+3];
	}
	int dblForce[3];
	int dblStress[11];
	int nCount;
	int nUnit[14];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MNET:XXXX-SHJUNG2-20120615 - 관련 테이블의 메모리 개선을 위해 구조체 축소
struct __MY_EXT_CLASS__ T_STRP_FORC
{
// dblForce     : local force                 , 4 node,
//                component : Fx, Fy, Fz, Mx, My, Mz
	double dblForce[4][6];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRP_FORC));
	}
	void Max(T_STRP_FORC& Data)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)
				if(Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
	}
	void Min(T_STRP_FORC& Data)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)
				if(Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
	}
	void AbsMax(T_STRP_FORC& Data)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=__max(fabs(dblForce[i][j]),fabs(Data.dblForce[i][j]));
		}
	}
	void AbsMax(T_STRP_FORC& Data1, T_STRP_FORC& Data2)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=__max(fabs(Data1.dblForce[i][j]),fabs(Data2.dblForce[i][j]));
		}
	}
	void AbsMaxwithSign(T_STRP_FORC& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)
			{
				a = dblForce[i][j]; b = Data.dblForce[i][j];
				dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dblForce[i][j] : Data.dblForce[i][j]);
			}
		}
	}
	void AbsMaxwithSign(T_STRP_FORC& Data1, T_STRP_FORC& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)
			{
				a = Data1.dblForce[i][j]; b = Data2.dblForce[i][j];
				dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
			}
		}
	}
	void Add(T_STRP_FORC& Data)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]+=Data.dblForce[i][j];
		}
	}
	void Abs()
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=fabs(dblForce[i][j]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=dblForce[i][j]*dblForce[i][j];
		}
	}
	void Root()
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=sqrt(dblForce[i][j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]*=dFactor;
		}
	}
	void Multiple(T_STRP_FORC& Data)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]*=Data.dblForce[i][j];
		}
	}
};

struct __MY_EXT_CLASS__ T_STRP_STRS
{
// dblStress    : local stress + unit force   , cent + 4 node, 
//                component : Fxx, Fyy, Fxy, MAXF, MINF, ANGF, Mxx, Myy, Mxy, MAXM, MINM, ANGM, Vxx, Vyy
// T_STRP_DL의 dblStress[5][14]~dblStress[5][27]
	
	double dblStress[5][14];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRP_STRS));
	}
	void Max(T_STRP_STRS& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)
			{
				if(k != 5 && k != 11 && Data.dblStress[i][k] > dblStress[i][k])
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k == 3)dblStress[i][5]=Data.dblStress[i][5];
					if(k == 9)dblStress[i][11]=Data.dblStress[i][11];
				}
			}
		}
	}
	void Min(T_STRP_STRS& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)
			{
				if(k != 5 && k != 25 && Data.dblStress[i][k] < dblStress[i][k])
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k == 3)dblStress[i][5]=Data.dblStress[i][5];
					if(k == 9)dblStress[i][11]=Data.dblStress[i][11];
				}
			}
		}
	}
	void AbsMax(T_STRP_STRS& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)
			{
				if(k != 5 && k != 11)
				{
					if(fabs(Data.dblStress[i][k]) > fabs(dblStress[i][k]))
					{
						dblStress[i][k]=fabs(Data.dblStress[i][k]);
						if(k == 3)dblStress[i][5]=Data.dblStress[i][5];
						if(k == 9)dblStress[i][11]=Data.dblStress[i][11];
					}
					else
					{
						dblStress[i][k]=fabs(dblStress[i][k]);
						if(k == 3)dblStress[i][5]=dblStress[i][5];
						if(k == 9)dblStress[i][11]=dblStress[i][11];
					}
				}
			}
		}
	}
	void AbsMax(T_STRP_STRS& Data1, T_STRP_STRS& Data2)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)
			{
				if(k != 5 && k != 11)
				{
					if(fabs(Data2.dblStress[i][k]) > fabs(Data1.dblStress[i][k]))
					{
						dblStress[i][k]=fabs(Data2.dblStress[i][k]);
						if(k == 3)dblStress[i][5]=Data2.dblStress[i][5];
						if(k == 9)dblStress[i][11]=Data2.dblStress[i][11];
					}
					else
					{
						dblStress[i][k]=fabs(Data1.dblStress[i][k]);
						if(k == 3)dblStress[i][5]=Data1.dblStress[i][5];
						if(k == 9)dblStress[i][11]=Data1.dblStress[i][11];
					}
				}
			}
		}
	}
	void AbsMaxwithSign(T_STRP_STRS& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)
			{
				if(k != 5 && k != 11)
				{
					a = dblStress[i][k]; b = Data.dblStress[i][k];
					if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a) )
					{
						dblStress[i][k]=Data.dblStress[i][k];
						if(k == 3)dblStress[i][5]=Data.dblStress[i][5];
						if(k == 9)dblStress[i][11]=Data.dblStress[i][11];
					}
				}
			}
		}
	}
	void AbsMaxwithSign(T_STRP_STRS& Data1, T_STRP_STRS& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)
			{
				switch(k)
				{
				case 5:
				case 11:
					break;
				default:
					{
						a = Data1.dblStress[i][k]; b = Data2.dblStress[i][k];
						if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a)) 
						{
							dblStress[i][k]=Data2.dblStress[i][k];
							switch(k)
							{
							case 3:  dblStress[i][5]=Data2.dblStress[i][5]; break;
							case 9:  dblStress[i][11]=Data2.dblStress[i][11]; break;
							default: break;
							}
						}
						else
						{
							dblStress[i][k]=Data1.dblStress[i][k];
							switch(k)
							{
							case 3:  dblStress[i][5]=Data1.dblStress[i][5]; break;
							case 9:  dblStress[i][11]=Data1.dblStress[i][11]; break;
							default: break;
							}
						}
					}
					break;
				}
			}
		}
	}
	void Add(T_STRP_STRS& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)
			{
				switch(k)
				{
				case 5:
				case 11:
					break;
				default:
					dblStress[i][k]+=Data.dblStress[i][k];
					break;
				}
			}
			dblStress[i][5]=0.;
			dblStress[i][11]=0.;
		}
	}
	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)dblStress[i][k]=fabs(dblStress[i][k]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)
			{
				if(k != 5 && k != 11)dblStress[i][k]=dblStress[i][k]*dblStress[i][k];
			}
			dblStress[i][5]=0.;
			dblStress[i][11]=0.;
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)
			{
				if(k != 5 && k != 11)dblStress[i][k]=sqrt(dblStress[i][k]);
			}
			dblStress[i][5]=0.;
			dblStress[i][11]=0.;
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)
			{
				switch(k)
				{
				case 5:
				case 11:
					break;
				default:
					dblStress[i][k]*=dFactor;
					break;
				}
			}
		}
	}
	void Multiple(T_STRP_STRS& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<14; k++)
			{
				if(k != 5 && k != 11)dblStress[i][k]*=Data.dblStress[i][k];
			}
		}
	}
	void calcPrincipalStress()
	{
		double cc, bb, cr;
		double sigma[4];
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<2; j++)
			{
				int n=j*7;
				if(j == 1)n=6;
				for(int k=0; k<3; k++)sigma[k]=0.;
				if(dblStress[i][n+0] == 0. && dblStress[i][n+1] == 0. && dblStress[i][n+2] == 0.)goto END;
				cc = (dblStress[i][n+0]+dblStress[i][n+1])/2.;
				bb = (dblStress[i][n+0]-dblStress[i][n+1])/2.;
				cr = sqrt(pow(bb,2)+pow(dblStress[i][n+2],2));
				sigma[0] = cc+cr;
				sigma[1] = cc-cr;
				if(fabs(bb) <= 1.e-10 && fabs(dblStress[i][n+2]) <= 1.e-10)
					sigma[2] = 0.;
				else if(fabs(sigma[2]) > 1.e-10 && fabs(bb) <= 1.e-10)
				{
					if(dblStress[i][n+2] >= 0.)sigma[2]=45.;
					if(dblStress[i][n+2] < 0.)sigma[2]=-45.;
				}
				else
					sigma[2]=90./(4.*atan(1.))*atan2(dblStress[i][n+2],bb);
END:
				dblStress[i][n+3]=sigma[0];
				dblStress[i][n+4]=sigma[1];
				dblStress[i][n+5]=sigma[2];
			}
		}
	}
};

struct __MY_EXT_CLASS__ T_STRP_STRS2
{

// dblStress    : local stress + unit force   , cent + 4 node, 
//                component : Sxxt, Syyt, Sxyt, P1t, P2t, ANGt, EFFt, Sxxb, Syyb, Sxyb, P1b, P2b, ANGb, EFFb, 
//                            Max-Shear_t, Max-Shear_b
// T_STRP_DL의 dblStress[5][0]~dblStress[5][13], dblStress[5][28], dblStress[5][29]
	
	double dblStress[5][16];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRP_STRS2));
	}
	void Max(T_STRP_STRS2& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)
			{
				if(k != 5 && k != 12 && Data.dblStress[i][k] > dblStress[i][k])
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
					if(k == 10)dblStress[i][12]=Data.dblStress[i][12];
				}
			}
		}
	}
	void Min(T_STRP_STRS2& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)
			{
				if(k != 5 && k != 12 && Data.dblStress[i][k] < dblStress[i][k])
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
					if(k == 10)dblStress[i][12]=Data.dblStress[i][12];
				}
			}
		}
	}
	void AbsMax(T_STRP_STRS2& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)
			{
				if(k != 5 && k != 12)
				{
					if(fabs(Data.dblStress[i][k]) > fabs(dblStress[i][k]))
					{
						dblStress[i][k]=fabs(Data.dblStress[i][k]);
						if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=Data.dblStress[i][12];
					}
					else
					{
						dblStress[i][k]=fabs(dblStress[i][k]);
						if(k ==  3)dblStress[i][ 5]=dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=dblStress[i][12];
					}
				}
			}
		}
	}
	void AbsMax(T_STRP_STRS2& Data1, T_STRP_STRS2& Data2)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)
			{
				if(k != 5 && k != 12)
				{
					if(fabs(Data2.dblStress[i][k]) > fabs(Data1.dblStress[i][k]))
					{
						dblStress[i][k]=fabs(Data2.dblStress[i][k]);
						if(k ==  3)dblStress[i][ 5]=Data2.dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=Data2.dblStress[i][12];
					}
					else
					{
						dblStress[i][k]=fabs(Data1.dblStress[i][k]);
						if(k ==  3)dblStress[i][ 5]=Data1.dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=Data1.dblStress[i][12];
					}
				}
			}
		}
	}
	void AbsMaxwithSign(T_STRP_STRS2& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)
			{
				if(k != 5 && k != 12)
				{
					a = dblStress[i][k]; b = Data.dblStress[i][k];
					if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a) )
					{
						dblStress[i][k]=Data.dblStress[i][k];
						if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=Data.dblStress[i][12];
					}
				}
			}
		}
	}
	void AbsMaxwithSign(T_STRP_STRS2& Data1, T_STRP_STRS2& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)
			{
				switch(k)
				{
				case 5:
				case 12:
					break;
				default:
					{
						a = Data1.dblStress[i][k]; b = Data2.dblStress[i][k];
						if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a)) 
						{
							dblStress[i][k]=Data2.dblStress[i][k];
							switch(k)
							{
							case 3:   dblStress[i][ 5]=Data2.dblStress[i][ 5]; break;
							case 10:  dblStress[i][12]=Data2.dblStress[i][12]; break;
							default: break;
							}
						}
						else
						{
							dblStress[i][k]=Data1.dblStress[i][k];
							switch(k)
							{
							case 3:   dblStress[i][ 5]=Data1.dblStress[i][ 5]; break;
							case 10:  dblStress[i][12]=Data1.dblStress[i][12]; break;
							default: break;
							}
						}
					}
					break;
				}
			}
		}
	}
	void Add(T_STRP_STRS2& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)
			{
				switch(k)
				{
				case 5:
				case 12:
					break;
				default:
					dblStress[i][k]+=Data.dblStress[i][k];
					break;
				}
			}
			dblStress[i][ 5]=0.;
			dblStress[i][12]=0.;
		}
	}
	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)dblStress[i][k]=fabs(dblStress[i][k]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)
			{
				if(k != 5 && k != 12)dblStress[i][k]=dblStress[i][k]*dblStress[i][k];
			}
			dblStress[i][ 5]=0.;
			dblStress[i][12]=0.;
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)
			{
				if(k != 5 && k != 12)dblStress[i][k]=sqrt(dblStress[i][k]);
			}
			dblStress[i][ 5]=0.;
			dblStress[i][12]=0.;
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)
			{
				switch(k)
				{
				case 5:
				case 12:
					break;
				default:
					dblStress[i][k]*=dFactor;
					break;
				}
			}
		}
	}
	void Multiple(T_STRP_STRS2& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<16; k++)
			{
				if(k != 5 && k != 12)dblStress[i][k]*=Data.dblStress[i][k];
			}
		}
	}	
	void calcPrincipalStress()
	{
		BOOL bEffective;
		double cc, bb, cr;
		double sigma[4];
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<2; j++)
			{
				int n=j*7;
				bEffective=FALSE;
				if(j==0 || j==1)bEffective=TRUE;
				for(int k=0; k<3; k++)sigma[k]=0.;
				if(bEffective)sigma[3]=0.;
				if(dblStress[i][n+0] == 0. && dblStress[i][n+1] == 0. && dblStress[i][n+2] == 0.)goto END;
				cc = (dblStress[i][n+0]+dblStress[i][n+1])/2.;
				bb = (dblStress[i][n+0]-dblStress[i][n+1])/2.;
				cr = sqrt(pow(bb,2)+pow(dblStress[i][n+2],2));
				sigma[0] = cc+cr;
				sigma[1] = cc-cr;
				if(fabs(bb) <= 1.e-10 && fabs(dblStress[i][n+2]) <= 1.e-10)
					sigma[2] = 0.;
				else if(fabs(sigma[2]) > 1.e-10 && fabs(bb) <= 1.e-10)
				{
					if(dblStress[i][n+2] >= 0.)sigma[2]=45.;
					if(dblStress[i][n+2] < 0.)sigma[2]=-45.;
				}
				else
					sigma[2]=90./(4.*atan(1.))*atan2(dblStress[i][n+2],bb);
				if(bEffective)sigma[3] = sqrt(pow(sigma[0],2)+pow(sigma[1],2)-sigma[0]*sigma[1]);
END:
				dblStress[i][n+3]=sigma[0];
				dblStress[i][n+4]=sigma[1];
				dblStress[i][n+5]=sigma[2];
				if(bEffective)dblStress[i][n+6]=sigma[3];
			}

			//Max-Shear
			dblStress[i][14] = __max(__max( fabs(dblStress[i][3]-dblStress[i][4])/2., fabs(dblStress[i][3])/2. ), fabs(dblStress[i][4])/2. ) ;
			dblStress[i][15] = __max(__max( fabs(dblStress[i][10]-dblStress[i][11])/2.,fabs(dblStress[i][10])/2. ),fabs(dblStress[i][11])/2.);
		}
	}
};

struct __MY_EXT_CLASS__ T_STRP_STRS3
{
// dblStress    : global stress + unit force   , cent + 4 node, 
//                component : SXXt, SYYt, SZZt, SXYt, SYZt, SXZt, P1t, P2t, ANGt, EFFt, 
//                            SXXb, SYYb, SZZb, SXYb, SYZb, SXZb, P1b, P2b, ANGb, EFFb
//                            Max-Shear_t, Max-Shear_b
// T_STRP_DG의 dblStress[5][0]~dblStress[5][19], dblStress[5][40], dblStress[5][41]

	double dblStress[5][22];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRP_STRS3));
	}
	void Max(T_STRP_STRS3& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<22; k++)
			{
				if(k != 8 && k != 18 && Data.dblStress[i][k] > dblStress[i][k])
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k ==  6)dblStress[i][ 8]=Data.dblStress[i][ 8];
					if(k == 16)dblStress[i][18]=Data.dblStress[i][18];
				}
			}
		}
	}
	void Min(T_STRP_STRS3& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<22; k++)
			{
				if(k != 8 && k != 18 && Data.dblStress[i][k] < dblStress[i][k])
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k ==  6)dblStress[i][ 8]=Data.dblStress[i][ 8];
					if(k == 16)dblStress[i][18]=Data.dblStress[i][18];
				}
			}
		}
	}
	void AbsMax(T_STRP_STRS3& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<22; k++)
			{
				if(k != 8 && k != 18)
				{
					if(fabs(Data.dblStress[i][k]) > fabs(dblStress[i][k]))
					{
						dblStress[i][k]=fabs(Data.dblStress[i][k]);
						if(k ==  6)dblStress[i][ 8]=Data.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data.dblStress[i][18];
					}
					else
					{
						dblStress[i][k]=fabs(dblStress[i][k]);
						if(k ==  6)dblStress[i][ 8]=dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=dblStress[i][18];
					}
				}
			}
		}
	}
	void AbsMax(T_STRP_STRS3& Data1, T_STRP_STRS3& Data2)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<22; k++)
			{
				if(k != 8 && k != 18)
				{
					if(fabs(Data2.dblStress[i][k]) > fabs(Data1.dblStress[i][k]))
					{
						dblStress[i][k]=fabs(Data2.dblStress[i][k]);
						if(k ==  6)dblStress[i][ 8]=Data2.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data2.dblStress[i][18];
					}
					else
					{
						dblStress[i][k]=fabs(Data1.dblStress[i][k]);
						if(k ==  6)dblStress[i][ 8]=Data1.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data1.dblStress[i][18];
					}
				}
			}
		}
	}
	void AbsMaxwithSign(T_STRP_STRS3& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<22; k++)
			{
				if(k != 8 && k != 18)
				{
					a = dblStress[i][k]; b = Data.dblStress[i][k];
					if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a) )
					{
						dblStress[i][k]=Data.dblStress[i][k];
						if(k ==  6)dblStress[i][ 8]=Data.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data.dblStress[i][18];
					}
				}
			}
		}
	}
	void AbsMaxwithSign(T_STRP_STRS3& Data1, T_STRP_STRS3& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<22; k++)
			{
				if(k != 8 && k != 18)
				{
					a = Data1.dblStress[i][k]; b = Data2.dblStress[i][k];
					if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a)) 
					{
						dblStress[i][k]=Data2.dblStress[i][k];
						if(k ==  6)dblStress[i][ 8]=Data2.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data2.dblStress[i][18];
					}
					else 
					{
						dblStress[i][k]=Data1.dblStress[i][k];
						if(k ==  6)dblStress[i][ 8]=Data1.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data1.dblStress[i][18];
					}
				}
			}
		}
	}
	void Add(T_STRP_STRS3& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<22; k++)
			{
				if(k != 8 && k != 18)dblStress[i][k]+=Data.dblStress[i][k];
			}
			dblStress[i][ 6]=0.;
			dblStress[i][16]=0.;
		}
	}
	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<22; k++)dblStress[i][k]=fabs(dblStress[i][k]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<22; k++)
			{
				if(k != 8 && k != 18)dblStress[i][k]=dblStress[i][k]*dblStress[i][k];
			}
			dblStress[i][ 6]=0.;
			dblStress[i][16]=0.;
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<22; k++)
			{
				if(k != 8 && k != 18)dblStress[i][k]=sqrt(dblStress[i][k]);
			}
			dblStress[i][ 6]=0.;
			dblStress[i][16]=0.;
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			for(int k=0; k<22; k++)
			{
				if(k != 8 && k != 18)dblStress[i][k]*=dFactor;
			}
		}
	}
	void calcPrincipalStress()
	{
		BOOL bEffective;
		double cc, bb, cr;
		double sigma[4];
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<2; j++)
			{
				int n=j*10;
				bEffective=FALSE;
				if(j==0 || j==1)bEffective=TRUE;
				for(int k=0; k<3; k++)sigma[k]=0.;
				if(bEffective)sigma[3]=0.;
				if(dblStress[i][n+0] == 0. && dblStress[i][n+1] == 0. && dblStress[i][n+3] == 0.)goto END;
				cc = (dblStress[i][n+0]+dblStress[i][n+1])/2.;
				bb = (dblStress[i][n+0]-dblStress[i][n+1])/2.;
				cr = sqrt(pow(bb,2)+pow(dblStress[i][n+3],2));
				sigma[0] = cc+cr;
				sigma[1] = cc-cr;
				if(fabs(bb) <= 1.e-10 && fabs(dblStress[i][n+3]) <= 1.e-10)
					sigma[2] = 0.;
				else if(fabs(sigma[2]) > 1.e-10 && fabs(bb) <= 1.e-10)
				{
					if(dblStress[i][n+3] >= 0.)sigma[2]=45.;
					if(dblStress[i][n+3] < 0.)sigma[2]=-45.;
				}
				else
					sigma[2]=90./(4.*atan(1.))*atan2(dblStress[i][n+3],bb);
				if(bEffective)sigma[3] = sqrt(pow(sigma[0],2)+pow(sigma[1],2)-sigma[0]*sigma[1]);
END:
				dblStress[i][n+6]=sigma[0];
				dblStress[i][n+7]=sigma[1];
				dblStress[i][n+8]=sigma[2];
				if(bEffective)dblStress[i][n+9]=sigma[3];
			}

			//Max-Shear
			// MNET:5760-HSSHIM-20130328 : 2차원 요소에 대해서는 2차원 식을 반영하도록 수정.
			// 3차원 계산식 
			//dblStress[i][20] = __max(__max( fabs(dblStress[i][6]-dblStress[i][7])/2., fabs(dblStress[i][6])/2. ), fabs(dblStress[i][7])/2. ) ;
			//dblStress[i][21] = __max(__max( fabs(dblStress[i][16]-dblStress[i][17])/2.,fabs(dblStress[i][16])/2. ),fabs(dblStress[i][17])/2.);
			// 2차원 계산식 
			dblStress[i][20] = fabs(dblStress[i][6]-dblStress[i][7])/2.;
			dblStress[i][21] = fabs(dblStress[i][16]-dblStress[i][17])/2.;
		}
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// plate
struct __MY_EXT_CLASS__ T_STRP_DL
{
// dblForce     : local force                 , 4 node       ,
//                component : Fx, Fy, Fz, Mx, My, Mz
// dblStressIOS : local stress(membrane, bending, shear), cent + 4 node, 
//                component : SMx, SMy, SMz, SBx, SBy, SBz, Vx, Vx
// dblStress    : local stress + unit force   , cent + 4 node, 
//                component : Sxxt, Syyt, Sxyt, P1t, P2t, ANGt, EFFt, Sxxb, Syyb, Sxyb, P1b, P2b, ANGb, EFFb, 
//                            Fxx, Fyy, Fxy, MAXF, MINF, ANGF, Mxx, Myy, Mxy, MAXM, MINM, ANGM, Vxx, Vyy, Max-Shear_t, Max-Shear_b

	double dblForce[4][6];
//  double dblStressIOS[5][8];
	double dblStress[5][30];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRP_DL));
		//memset(dblForce,0,sizeof(dblForce));
		////memset(dblStressIOS,0,sizeof(dblStressIOS));
		//memset(dblStress,0,sizeof(dblStress));
	}
	void Max(T_STRP_DL& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)
					if(Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
			}
			for(int k=0; k<30; k++)
			{
				if(k != 5 && k != 12 && k != 19 && k != 25 && Data.dblStress[i][k] > dblStress[i][k])
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
					if(k == 10)dblStress[i][12]=Data.dblStress[i][12];
					if(k == 17)dblStress[i][19]=Data.dblStress[i][19];
					if(k == 23)dblStress[i][25]=Data.dblStress[i][25];
				}
			}
		}
	}
	void Min(T_STRP_DL& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)
					if(Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
			}
			for(int k=0; k<30; k++)
			{
				if(k != 5 && k != 12 && k != 19 && k != 25 && Data.dblStress[i][k] < dblStress[i][k])
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
					if(k == 10)dblStress[i][12]=Data.dblStress[i][12];
					if(k == 17)dblStress[i][19]=Data.dblStress[i][19];
					if(k == 23)dblStress[i][25]=Data.dblStress[i][25];
				}
			}
		}
	}
	void AbsMax(T_STRP_DL& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]=__max(fabs(dblForce[i][j]),fabs(Data.dblForce[i][j]));
			}
			for(int k=0; k<30; k++)
			{
				if(k != 5 && k != 12 && k != 19 && k != 25)
				{
					if(fabs(Data.dblStress[i][k]) > fabs(dblStress[i][k]))
					{
						dblStress[i][k]=fabs(Data.dblStress[i][k]);
						if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=Data.dblStress[i][12];
						if(k == 17)dblStress[i][19]=Data.dblStress[i][19];
						if(k == 23)dblStress[i][25]=Data.dblStress[i][25];
					}
					else
					{
						dblStress[i][k]=fabs(dblStress[i][k]);            
					}
				}
			}
		}
	}
	void AbsMax(T_STRP_DL& Data1, T_STRP_DL& Data2)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]=__max(fabs(Data1.dblForce[i][j]),fabs(Data2.dblForce[i][j]));
			}
			for(int k=0; k<30; k++)
			{
				if(k != 5 && k != 12 && k != 19 && k != 25)
				{
					if(fabs(Data2.dblStress[i][k]) > fabs(Data1.dblStress[i][k]))
					{
						dblStress[i][k]=fabs(Data2.dblStress[i][k]);
						if(k ==  3)dblStress[i][ 5]=Data2.dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=Data2.dblStress[i][12];
						if(k == 17)dblStress[i][19]=Data2.dblStress[i][19];
						if(k == 23)dblStress[i][25]=Data2.dblStress[i][25];
					}
					else
					{
						dblStress[i][k]=fabs(Data1.dblStress[i][k]);
						if(k ==  3)dblStress[i][ 5]=Data1.dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=Data1.dblStress[i][12];
						if(k == 17)dblStress[i][19]=Data1.dblStress[i][19];
						if(k == 23)dblStress[i][25]=Data1.dblStress[i][25];
					}
				}
			}
		}
	}
	void AbsMaxwithSign(T_STRP_DL& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)
				{
					a = dblForce[i][j]; b = Data.dblForce[i][j];
					dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dblForce[i][j] : Data.dblForce[i][j]);
					// dblForce[i][j]=(fabs(dblForce[i][j]) >= fabs(Data.dblForce[i][j]) ? dblForce[i][j] : Data.dblForce[i][j]);
				}
			}
			for(int k=0; k<30; k++)
			{
				if(k != 5 && k != 12 && k != 19 && k != 25)
				{
					// if(fabs(Data.dblStress[i][k]) > fabs(dblStress[i][k]))
					a = dblStress[i][k]; b = Data.dblStress[i][k];
					if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a) )
					{
						dblStress[i][k]=Data.dblStress[i][k];
						if(k ==  3)dblStress[i][ 5]=Data.dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=Data.dblStress[i][12];
						if(k == 17)dblStress[i][19]=Data.dblStress[i][19];
						if(k == 23)dblStress[i][25]=Data.dblStress[i][25];
					}
					/*
					else
					{
						dblStress[i][k]=dblStress[i][k];
						if(k ==  3)dblStress[i][ 5]=dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=dblStress[i][12];
						if(k == 17)dblStress[i][19]=dblStress[i][19];
						if(k == 23)dblStress[i][25]=dblStress[i][25];
					}
					*/
				}
			}
		}
	}
	void AbsMaxwithSign(T_STRP_DL& Data1, T_STRP_DL& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)
				{
					a = Data1.dblForce[i][j]; b = Data2.dblForce[i][j];
					dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
					// dblForce[i][j]=(fabs(Data1.dblForce[i][j]) >= fabs(Data2.dblForce[i][j]) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
				}
			}
			for(int k=0; k<30; k++)
			{
				if(k != 5 && k != 12 && k != 19 && k != 25)
				{
					// if(fabs(Data2.dblStress[i][k]) > fabs(Data1.dblStress[i][k]))
					a = Data1.dblStress[i][k]; b = Data2.dblStress[i][k];
					if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a)) 
					{
						dblStress[i][k]=Data2.dblStress[i][k];
						if(k ==  3)dblStress[i][ 5]=Data2.dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=Data2.dblStress[i][12];
						if(k == 17)dblStress[i][19]=Data2.dblStress[i][19];
						if(k == 23)dblStress[i][25]=Data2.dblStress[i][25];
					}
					else
					{
						dblStress[i][k]=Data1.dblStress[i][k];
						if(k ==  3)dblStress[i][ 5]=Data1.dblStress[i][ 5];
						if(k == 10)dblStress[i][12]=Data1.dblStress[i][12];
						if(k == 17)dblStress[i][19]=Data1.dblStress[i][19];
						if(k == 23)dblStress[i][25]=Data1.dblStress[i][25];
					}
				}
			}
		}
	}
	void Add(T_STRP_DL& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]+=Data.dblForce[i][j];
			}
			for(int k=0; k<30; k++)
			{
				if(k != 5 && k != 12 && k != 19 && k != 25)dblStress[i][k]+=Data.dblStress[i][k];
			}
			dblStress[i][ 5]=0.;
			dblStress[i][12]=0.;
			dblStress[i][19]=0.;
			dblStress[i][25]=0.;
		}
	}
	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]=fabs(dblForce[i][j]);
			}
			for(int k=0; k<30; k++)dblStress[i][k]=fabs(dblStress[i][k]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]=dblForce[i][j]*dblForce[i][j];
			}
			for(int k=0; k<30; k++)
			{
				if(k != 5 && k != 12 && k != 19 && k != 25)dblStress[i][k]=dblStress[i][k]*dblStress[i][k];
			}
			dblStress[i][ 5]=0.;
			dblStress[i][12]=0.;
			dblStress[i][19]=0.;
			dblStress[i][25]=0.;
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]=sqrt(dblForce[i][j]);
			}
			for(int k=0; k<30; k++)
			{
				if(k != 5 && k != 12 && k != 19 && k != 25)dblStress[i][k]=sqrt(dblStress[i][k]);
			}
			dblStress[i][ 5]=0.;
			dblStress[i][12]=0.;
			dblStress[i][19]=0.;
			dblStress[i][25]=0.;
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]*=dFactor;
			}
			for(int k=0; k<30; k++)
			{
				if(k != 5 && k != 12 && k != 19 && k != 25)dblStress[i][k]*=dFactor;
			}
		}
	}
	void Multiple(T_STRP_DL Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]*=Data.dblForce[i][j];
			}
			for(int k=0; k<30; k++)
			{
				if(k != 5 && k != 12 && k != 19 && k != 25)dblStress[i][k]*=Data.dblStress[i][k];
			}
		}
	}	
	void calcPrincipalStress()
	{
		M_PROFILE('calcPrincipalStress()');
		BOOL bEffective;
		double cc, bb, cr;
		double sigma[4];
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				int n=j*7;
				if(j == 3)n=20;
				bEffective=FALSE;
				if(j==0 || j==1)bEffective=TRUE;
				for(int k=0; k<3; k++)sigma[k]=0.;
				if(bEffective)sigma[3]=0.;
				if(dblStress[i][n+0] == 0. && dblStress[i][n+1] == 0. && dblStress[i][n+2] == 0.)goto END;
				cc = (dblStress[i][n+0]+dblStress[i][n+1])/2.;
				bb = (dblStress[i][n+0]-dblStress[i][n+1])/2.;
				cr = sqrt(pow(bb,2)+pow(dblStress[i][n+2],2));
				sigma[0] = cc+cr;
				sigma[1] = cc-cr;
				if(fabs(bb) <= 1.e-10 && fabs(dblStress[i][n+2]) <= 1.e-10)
					sigma[2] = 0.;
				else if(fabs(sigma[2]) > 1.e-10 && fabs(bb) <= 1.e-10)
				{
					if(dblStress[i][n+2] >= 0.)sigma[2]=45.;
					if(dblStress[i][n+2] < 0.)sigma[2]=-45.;
				}
				else
					sigma[2]=90./(4.*atan(1.))*atan2(dblStress[i][n+2],bb);
				if(bEffective)sigma[3] = sqrt(pow(sigma[0],2)+pow(sigma[1],2)-sigma[0]*sigma[1]);
END:
				dblStress[i][n+3]=sigma[0];
				dblStress[i][n+4]=sigma[1];
				dblStress[i][n+5]=sigma[2];
				if(bEffective)dblStress[i][n+6]=sigma[3];
			}

			//Max-Shear
			dblStress[i][28] = __max(__max( fabs(dblStress[i][3]-dblStress[i][4])/2., fabs(dblStress[i][3])/2. ), fabs(dblStress[i][4])/2. ) ;
			dblStress[i][29] = __max(__max( fabs(dblStress[i][10]-dblStress[i][11])/2.,fabs(dblStress[i][10])/2. ),fabs(dblStress[i][11])/2.);
		}
	}
	void ConvertToSpcf(T_SPCF_DL& Data)
	{
		Data.Initialize();

		for(int i=0; i<5; i++)
		{
			for(int j=0; j<8; j++)
			{
				Data.dForce[i][j][0] = dblStress[i][14];
				Data.dForce[i][j][1] = dblStress[i][15];
				Data.dForce[i][j][2] = dblStress[i][16];
				Data.dForce[i][j][3] = dblStress[i][20];
				Data.dForce[i][j][4] = dblStress[i][21];
				Data.dForce[i][j][5] = dblStress[i][22];
				Data.dForce[i][j][6] = dblStress[i][26];
				Data.dForce[i][j][7] = dblStress[i][27];
			}
		}
	}
};
struct T_STRPL_UNIT
{
	T_STRPL_UNIT()
	{
		nCount=36;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;
		nUnit[3]=D_UNITSYS_BASE_MOMENT;
		nUnit[4]=D_UNITSYS_BASE_MOMENT;
		nUnit[5]=D_UNITSYS_BASE_MOMENT;

		nUnit[6]=D_UNITSYS_BASE_STRESS;
		nUnit[7]=D_UNITSYS_BASE_STRESS;
		nUnit[8]=D_UNITSYS_BASE_STRESS;
		nUnit[9]=D_UNITSYS_BASE_STRESS;
		nUnit[10]=D_UNITSYS_BASE_STRESS;
		nUnit[11]=D_UNITSYS_BASE_DEGREE;
		nUnit[12]=D_UNITSYS_BASE_STRESS;
		nUnit[13]=D_UNITSYS_BASE_STRESS;
		nUnit[14]=D_UNITSYS_BASE_STRESS;
		nUnit[15]=D_UNITSYS_BASE_STRESS;
		nUnit[16]=D_UNITSYS_BASE_STRESS;
		nUnit[17]=D_UNITSYS_BASE_STRESS;
		nUnit[18]=D_UNITSYS_BASE_DEGREE;
		nUnit[19]=D_UNITSYS_BASE_STRESS;

		nUnit[20]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[21]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[22]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[23]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[24]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[25]=D_UNITSYS_BASE_DEGREE;
		nUnit[26]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[27]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[28]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[29]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[30]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[31]=D_UNITSYS_BASE_DEGREE;
		nUnit[32]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[33]=D_UNITSYS_BASE_UNITFORCE;

		nUnit[34]=D_UNITSYS_BASE_STRESS;
		nUnit[35]=D_UNITSYS_BASE_STRESS;

		for(int i=0; i<6; i++)dblForce[i]=nUnit[i];
		for(int i = 0; i<30; i++)dblStress[i]=nUnit[i+6];
	}
	int dblForce[6];
	int dblStress[30];
	int nCount;
	int nUnit[36];
};

// plate
struct __MY_EXT_CLASS__ T_STRP_DG
{
// dblForce     : global force                 , 4 node       ,
//                component : FX, FY, FZ, MX, MY, MZ
// dblStressIOS : global stress(membrane, bending, shear), cent + 4 node, 
//                component : SMX, SMY, SMZ, SBX, SBY, SBZ, VX, VY
// dblStress    : global stress + unit force   , cent + 4 node, 
//                component : SXXt, SYYt, SZZt, SXYt, SYZt, SXZt, P1t, P2t, ANGt, EFFt, 
//                            SXXb, SYYb, SZZb, SXYb, SYZb, SXZb, P1b, P2b, ANGb, EFFb
//                            FXX, FYY, FZZ, FXY, FYZ, FXZ, MAXF, MINF, ANGF,
//                            MXX, MYY, MZZ, MXY, MYZ, MXZ, MAXM, MINM, ANGM,
//                            Vxx, Vyy, Max-Shear_t, Max-Shear_b

	double dblForce[4][6];
//  double dblStressIOS[5][8];
	double dblStress[5][42];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRP_DG));
		//memset(dblForce,0,sizeof(dblForce));
		////memset(dblStressIOS,0,sizeof(dblStressIOS));
		//memset(dblStress,0,sizeof(dblStress));
	}
	void Max(T_STRP_DG& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)
					if(Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
			}
			for(int k=0; k<42; k++)
			{
				if(k != 8 && k != 18 && k != 28 && k != 37 && Data.dblStress[i][k] > dblStress[i][k])
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k ==  6)dblStress[i][ 8]=Data.dblStress[i][ 8];
					if(k == 16)dblStress[i][18]=Data.dblStress[i][18];
					if(k == 26)dblStress[i][28]=Data.dblStress[i][28];
					if(k == 35)dblStress[i][37]=Data.dblStress[i][37];
				}
			}
		}
	}
	void Min(T_STRP_DG& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)
					if(Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
			}
			for(int k=0; k<42; k++)
			{
				if(k != 8 && k != 18 && k != 28 && k != 37 && Data.dblStress[i][k] < dblStress[i][k])
				{
					dblStress[i][k]=Data.dblStress[i][k];
					if(k ==  6)dblStress[i][ 8]=Data.dblStress[i][ 8];
					if(k == 16)dblStress[i][18]=Data.dblStress[i][18];
					if(k == 26)dblStress[i][28]=Data.dblStress[i][28];
					if(k == 35)dblStress[i][37]=Data.dblStress[i][37];
				}
			}
		}
	}
	void AbsMax(T_STRP_DG& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]=__max(fabs(dblForce[i][j]),fabs(Data.dblForce[i][j]));
			}
			for(int k=0; k<42; k++)
			{
				if(k != 8 && k != 18 && k != 28 && k != 37)
				{
					if(fabs(Data.dblStress[i][k]) > fabs(dblStress[i][k]))
					{
						dblStress[i][k]=fabs(Data.dblStress[i][k]);
						if(k ==  6)dblStress[i][ 8]=Data.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data.dblStress[i][18];
						if(k == 26)dblStress[i][28]=Data.dblStress[i][28];
						if(k == 35)dblStress[i][37]=Data.dblStress[i][37];
					}
					else
					{
						dblStress[i][k]=fabs(dblStress[i][k]);
						if(k ==  6)dblStress[i][ 8]=dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=dblStress[i][18];
						if(k == 26)dblStress[i][28]=dblStress[i][28];
						if(k == 35)dblStress[i][37]=dblStress[i][37];
					}
				}
			}
		}
	}
	void AbsMax(T_STRP_DG& Data1, T_STRP_DG& Data2)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]=__max(fabs(Data1.dblForce[i][j]),fabs(Data2.dblForce[i][j]));
			}
			for(int k=0; k<42; k++)
			{
				if(k != 8 && k != 18 && k != 28 && k != 37)
				{
					if(fabs(Data2.dblStress[i][k]) > fabs(Data1.dblStress[i][k]))
					{
						dblStress[i][k]=fabs(Data2.dblStress[i][k]);
						if(k ==  6)dblStress[i][ 8]=Data2.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data2.dblStress[i][18];
						if(k == 26)dblStress[i][28]=Data2.dblStress[i][28];
						if(k == 35)dblStress[i][37]=Data2.dblStress[i][37];
					}
					else
					{
						dblStress[i][k]=fabs(Data1.dblStress[i][k]);
						if(k ==  6)dblStress[i][ 8]=Data1.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data1.dblStress[i][18];
						if(k == 26)dblStress[i][28]=Data1.dblStress[i][28];
						if(k == 35)dblStress[i][37]=Data1.dblStress[i][37];
					}
				}
			}
		}
	}
	void AbsMaxwithSign(T_STRP_DG& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)
				{
					a = dblForce[i][j]; b = Data.dblForce[i][j];
					dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dblForce[i][j] : Data.dblForce[i][j]);
					// dblForce[i][j]=(fabs(dblForce[i][j]) >= fabs(Data.dblForce[i][j]) ? dblForce[i][j] : Data.dblForce[i][j]);
				}
			}
			for(int k=0; k<42; k++)
			{
				if(k != 8 && k != 18 && k != 28 && k != 37)
				{
					// if(fabs(Data.dblStress[i][k]) > fabs(dblStress[i][k]))
					a = dblStress[i][k]; b = Data.dblStress[i][k];
					if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a) )
					{
						dblStress[i][k]=Data.dblStress[i][k];
						if(k ==  6)dblStress[i][ 8]=Data.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data.dblStress[i][18];
						if(k == 26)dblStress[i][28]=Data.dblStress[i][28];
						if(k == 35)dblStress[i][37]=Data.dblStress[i][37];
					}
					/*
					else
					{
						dblStress[i][k]=dblStress[i][k];
						if(k ==  6)dblStress[i][ 8]=dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=dblStress[i][18];
						if(k == 26)dblStress[i][28]=dblStress[i][28];
						if(k == 35)dblStress[i][37]=dblStress[i][37];
					}
					*/
				}
			}
		}
	}
	void AbsMaxwithSign(T_STRP_DG& Data1, T_STRP_DG& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)
				{
					a = Data1.dblForce[i][j]; b = Data2.dblForce[i][j];
					dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
					// dblForce[i][j]=(fabs(Data1.dblForce[i][j]) >= fabs(Data2.dblForce[i][j]) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
				}
			}
			for(int k=0; k<42; k++)
			{
				if(k != 8 && k != 18 && k != 28 && k != 37)
				{
					a = Data1.dblStress[i][k]; b = Data2.dblStress[i][k];
					if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a)) 
					{
						dblStress[i][k]=Data2.dblStress[i][k];
						if(k ==  6)dblStress[i][ 8]=Data2.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data2.dblStress[i][18];
						if(k == 26)dblStress[i][28]=Data2.dblStress[i][28];
						if(k == 35)dblStress[i][37]=Data2.dblStress[i][37];
					}
					else 
					{
						dblStress[i][k]=Data1.dblStress[i][k];
						if(k ==  6)dblStress[i][ 8]=Data1.dblStress[i][ 8];
						if(k == 16)dblStress[i][18]=Data1.dblStress[i][18];
						if(k == 26)dblStress[i][28]=Data1.dblStress[i][28];
						if(k == 35)dblStress[i][37]=Data1.dblStress[i][37];
					}
				}
			}
		}
	}
	void Add(T_STRP_DG& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]+=Data.dblForce[i][j];
			}
			for(int k=0; k<42; k++)
			{
				if(k != 8 && k != 18 && k != 28 && k != 37)dblStress[i][k]+=Data.dblStress[i][k];
			}
			dblStress[i][ 6]=0.;
			dblStress[i][16]=0.;
			dblStress[i][26]=0.;
			dblStress[i][35]=0.;
		}
	}
	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]=fabs(dblForce[i][j]);
			}
			for(int k=0; k<42; k++)dblStress[i][k]=fabs(dblStress[i][k]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]=dblForce[i][j]*dblForce[i][j];
			}
			for(int k=0; k<42; k++)
			{
				if(k != 8 && k != 18 && k != 28 && k != 37)dblStress[i][k]=dblStress[i][k]*dblStress[i][k];
			}
			dblStress[i][ 6]=0.;
			dblStress[i][16]=0.;
			dblStress[i][26]=0.;
			dblStress[i][35]=0.;
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]=sqrt(dblForce[i][j]);
			}
			for(int k=0; k<42; k++)
			{
				if(k != 8 && k != 18 && k != 28 && k != 37)dblStress[i][k]=sqrt(dblStress[i][k]);
			}
			dblStress[i][ 6]=0.;
			dblStress[i][16]=0.;
			dblStress[i][26]=0.;
			dblStress[i][35]=0.;
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<6; j++)dblForce[i][j]*=dFactor;
			}
			for(int k=0; k<42; k++)
			{
				if(k != 8 && k != 18 && k != 28 && k != 37)dblStress[i][k]*=dFactor;
			}
		}
	}
	void calcPrincipalStress()
	{
		BOOL bEffective;
		double cc, bb, cr;
		double sigma[4];
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				int n=j*10;
				if(j == 3)n=29;
				bEffective=FALSE;
				if(j==0 || j==1)bEffective=TRUE;
				for(int k=0; k<3; k++)sigma[k]=0.;
				if(bEffective)sigma[3]=0.;
				if(dblStress[i][n+0] == 0. && dblStress[i][n+1] == 0. && dblStress[i][n+3] == 0.)goto END;
				cc = (dblStress[i][n+0]+dblStress[i][n+1])/2.;
				bb = (dblStress[i][n+0]-dblStress[i][n+1])/2.;
				cr = sqrt(pow(bb,2)+pow(dblStress[i][n+3],2));
				sigma[0] = cc+cr;
				sigma[1] = cc-cr;
				if(fabs(bb) <= 1.e-10 && fabs(dblStress[i][n+3]) <= 1.e-10)
					sigma[2] = 0.;
				else if(fabs(sigma[2]) > 1.e-10 && fabs(bb) <= 1.e-10)
				{
					if(dblStress[i][n+3] >= 0.)sigma[2]=45.;
					if(dblStress[i][n+3] < 0.)sigma[2]=-45.;
				}
				else
					sigma[2]=90./(4.*atan(1.))*atan2(dblStress[i][n+3],bb);
				if(bEffective)sigma[3] = sqrt(pow(sigma[0],2)+pow(sigma[1],2)-sigma[0]*sigma[1]);
END:
				dblStress[i][n+6]=sigma[0];
				dblStress[i][n+7]=sigma[1];
				dblStress[i][n+8]=sigma[2];
				if(bEffective)dblStress[i][n+9]=sigma[3];
			}
			
			//Max-Shear
			// MNET:5760-HSSHIM-20130328 : 2차원 요소에 대해서는 2차원 식을 반영하도록 수정.
			// 3차원 계산식 
			//dblStress[i][40] = __max(__max( fabs(dblStress[i][6]-dblStress[i][7])/2., fabs(dblStress[i][6])/2. ), fabs(dblStress[i][7])/2. ) ;
			//dblStress[i][41] = __max(__max( fabs(dblStress[i][16]-dblStress[i][17])/2.,fabs(dblStress[i][16])/2. ),fabs(dblStress[i][17])/2.);
			// 2차원 계산식 
			dblStress[i][40] = fabs(dblStress[i][6]-dblStress[i][7])/2.;
			dblStress[i][41] = fabs(dblStress[i][16]-dblStress[i][17])/2.;
		}
	}
};
struct T_STRPG_UNIT
{
	T_STRPG_UNIT()
	{
		nCount=26;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;
		nUnit[3]=D_UNITSYS_BASE_MOMENT;
		nUnit[4]=D_UNITSYS_BASE_MOMENT;
		nUnit[5]=D_UNITSYS_BASE_MOMENT;

		nUnit[6]=D_UNITSYS_BASE_STRESS;
		nUnit[7]=D_UNITSYS_BASE_STRESS;
		nUnit[8]=D_UNITSYS_BASE_STRESS;
		nUnit[9]=D_UNITSYS_BASE_STRESS;
		nUnit[10]=D_UNITSYS_BASE_STRESS;
		nUnit[11]=D_UNITSYS_BASE_STRESS;
		nUnit[12]=D_UNITSYS_BASE_STRESS;
		nUnit[13]=D_UNITSYS_BASE_STRESS;
		nUnit[14]=D_UNITSYS_BASE_DEGREE;
		nUnit[15]=D_UNITSYS_BASE_STRESS;

		nUnit[16]=D_UNITSYS_BASE_STRESS;
		nUnit[17]=D_UNITSYS_BASE_STRESS;
		nUnit[18]=D_UNITSYS_BASE_STRESS;
		nUnit[19]=D_UNITSYS_BASE_STRESS;
		nUnit[20]=D_UNITSYS_BASE_STRESS;
		nUnit[21]=D_UNITSYS_BASE_STRESS;
		nUnit[22]=D_UNITSYS_BASE_STRESS;
		nUnit[23]=D_UNITSYS_BASE_STRESS;
		nUnit[24]=D_UNITSYS_BASE_DEGREE;
		nUnit[25]=D_UNITSYS_BASE_STRESS;

		nUnit[26]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[27]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[28]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[29]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[30]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[31]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[32]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[33]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[34]=D_UNITSYS_BASE_DEGREE;

		nUnit[35]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[36]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[37]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[38]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[39]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[40]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[41]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[42]=D_UNITSYS_BASE_UNITMOMENT;
		nUnit[43]=D_UNITSYS_BASE_DEGREE;

		nUnit[44]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[45]=D_UNITSYS_BASE_UNITFORCE;

		nUnit[46]=D_UNITSYS_BASE_STRESS;
		nUnit[47]=D_UNITSYS_BASE_STRESS;

		for(int i=0; i<6; i++)dblForce[i]=nUnit[i];
		for(int i = 0; i<42; i++)dblStress[i]=nUnit[i+6];
	}
	int dblForce[6];
	int dblStress[42];
	int nCount;
	int nUnit[48];
};

// solid
struct __MY_EXT_CLASS__ T_STRS_D
{
// dblForce  : global force , 8 node       , component : FX, FY, FZ
// dblStress : global stress, cent + 8 node, component : SXX, SYY, SZZ, SXY, SYZ, SXZ, P1, P2, P3, MAX-SHEAR, EFF-STRS, OCT-SHEAR,
//                             P1(ux,uy,uz), P2(ux,uy,uz), P3(ux,uy,uz)
	double dblForce[8][3];
	double dblStress[1+8][21];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRS_D));
		//memset(dblForce,0,sizeof(dblForce));
		//memset(dblStress,0,sizeof(dblStress));
	}
	void Max(T_STRS_D& Data)
	{
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++){if(Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];}
			}
			for(int j=0; j<12; j++)
			{
				if(Data.dblStress[i][j] > dblStress[i][j])
				{
					dblStress[i][j]=Data.dblStress[i][j];
					if(j == 6){for(int k=12; k<15; k++)dblStress[i][k]=Data.dblStress[i][k];}
					if(j == 7){for(int k=15; k<18; k++)dblStress[i][k]=Data.dblStress[i][k];}
					if(j == 8){for(int k=18; k<21; k++)dblStress[i][k]=Data.dblStress[i][k];}
				}
			}
		}
	}
	void Min(T_STRS_D& Data)
	{
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++){if(Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];}
			}
			for(int j=0; j<12; j++)
			{
				if(Data.dblStress[i][j] < dblStress[i][j])
				{
					dblStress[i][j]=Data.dblStress[i][j];
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data.dblStress[i][l];}
				}
			}
		}
	}
	void AbsMax(T_STRS_D& Data)
	{
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=__max(fabs(dblForce[i][j]),fabs(Data.dblForce[i][j]));
			}
			for(int j=0; j<12; j++)
			{
				if(fabs(Data.dblStress[i][j]) > fabs(dblStress[i][j]))
				{
					dblStress[i][j]=fabs(Data.dblStress[i][j]);
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data.dblStress[i][l];}
				}
				else
				{
					dblStress[i][j]=fabs(dblStress[i][j]);
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=dblStress[i][l];}
				}
			}
		}
	}
	void AbsMax(T_STRS_D& Data1, T_STRS_D& Data2)
	{
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=__max(fabs(Data1.dblForce[i][j]),fabs(Data2.dblForce[i][j]));
			}
			for(int j=0; j<12; j++)
			{
				if(fabs(Data2.dblStress[i][j]) > fabs(Data1.dblStress[i][j]))
				{
					dblStress[i][j]=fabs(Data2.dblStress[i][j]);
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data2.dblStress[i][l];}
				}
				else
				{
					dblStress[i][j]=fabs(Data1.dblStress[i][j]);
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data1.dblStress[i][l];}
				}
			}
		}
	}
	void AbsMaxwithSign(T_STRS_D& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++)
				{
					a = dblForce[i][j]; b = Data.dblForce[i][j];
					dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dblForce[i][j] : Data.dblForce[i][j]);
					// dblForce[i][j]=(fabs(dblForce[i][j]) >= fabs(Data.dblForce[i][j]) ? dblForce[i][j] : Data.dblForce[i][j]);
				}
			}
			for(int j=0; j<12; j++)
			{
				// if(fabs(Data.dblStress[i][j]) > fabs(dblStress[i][j]))
				a = dblStress[i][j]; b = Data.dblStress[i][j];
				if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a) )
				{
					dblStress[i][j]=Data.dblStress[i][j];
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data.dblStress[i][l];}
				}
				/*
				else
				{
					dblStress[i][j]=dblStress[i][j];
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=dblStress[i][l];}
				}
				*/
			}
		}
	}
	void AbsMaxwithSign(T_STRS_D& Data1, T_STRS_D& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++)
				{
					a = Data1.dblForce[i][j]; b = Data2.dblForce[i][j];
					dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
					// dblForce[i][j]=(fabs(Data1.dblForce[i][j]) >= fabs(Data2.dblForce[i][j]) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
				}
			}
			for(int j=0; j<12; j++)
			{
				// if(fabs(Data2.dblStress[i][j]) > fabs(Data1.dblStress[i][j]))
				a = Data1.dblStress[i][j]; b = Data2.dblStress[i][j];
				if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a)) 
				{
					dblStress[i][j]=Data2.dblStress[i][j];
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data2.dblStress[i][l];}
				}
				else
				{
					dblStress[i][j]=Data1.dblStress[i][j];
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data1.dblStress[i][l];}
				}
			}
		}
	}
	void Add(T_STRS_D& Data)
	{
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++)dblForce[i][j]+=Data.dblForce[i][j];
			}
			for(int j=0; j<12; j++)
			{
				dblStress[i][j]+=Data.dblStress[i][j];
				for(int l=12; l<15; l++)dblStress[i][l]=0.;
				for(int l =15; l<18; l++)dblStress[i][l]=0.;
				for(int l =18; l<21; l++)dblStress[i][l]=0.;
			}
		}
	}
	void Abs()
	{
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=fabs(dblForce[i][j]);
			}
			for(int j=0; j<12; j++)dblStress[i][j]=fabs(dblStress[i][j]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=dblForce[i][j]*dblForce[i][j];
			}
			for(int j=0; j<12; j++)
			{
				dblStress[i][j]=dblStress[i][j]*dblStress[i][j];
				for(int l=12; l<15; l++)dblStress[i][l]=0.;
				for(int l =15; l<18; l++)dblStress[i][l]=0.;
				for(int l =18; l<21; l++)dblStress[i][l]=0.;
			}
		}
	}
	void Root()
	{
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=sqrt(dblForce[i][j]);
			}
			for(int j=0; j<12; j++)
			{
				dblStress[i][j]=sqrt(dblStress[i][j]);
				for(int l=12; l<15; l++)dblStress[i][l]=0.;
				for(int l =15; l<18; l++)dblStress[i][l]=0.;
				for(int l =18; l<21; l++)dblStress[i][l]=0.;
			}
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++)dblForce[i][j]*=dFactor;
			}
			for(int j=0; j<12; j++)dblStress[i][j]*=dFactor;
		}
	}
	void Multiple(T_STRS_D Data)
	{
		for(int i=0; i<9; i++)
		{
			if(i < 8)
			{
				for(int j=0; j<3; j++)dblForce[i][j]*=Data.dblForce[i][j];
			}
			for(int j=0; j<12; j++)dblStress[i][j]*=Data.dblStress[i][j];
		}
	}
};
struct T_STRS_UNIT
{
	T_STRS_UNIT()
	{
		nCount=24;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;

		nUnit[3]=D_UNITSYS_BASE_STRESS;
		nUnit[4]=D_UNITSYS_BASE_STRESS;
		nUnit[5]=D_UNITSYS_BASE_STRESS;
		nUnit[6]=D_UNITSYS_BASE_STRESS;
		nUnit[7]=D_UNITSYS_BASE_STRESS;
		nUnit[8]=D_UNITSYS_BASE_STRESS;
		nUnit[9]=D_UNITSYS_BASE_STRESS;
		nUnit[10]=D_UNITSYS_BASE_STRESS;
		nUnit[11]=D_UNITSYS_BASE_STRESS;
		nUnit[12]=D_UNITSYS_BASE_STRESS;
		nUnit[13]=D_UNITSYS_BASE_STRESS;
		nUnit[14]=D_UNITSYS_BASE_STRESS;

		nUnit[15]=D_UNITSYS_NONE;
		nUnit[16]=D_UNITSYS_NONE;
		nUnit[17]=D_UNITSYS_NONE;
		nUnit[18]=D_UNITSYS_NONE;
		nUnit[19]=D_UNITSYS_NONE;
		nUnit[20]=D_UNITSYS_NONE;
		nUnit[21]=D_UNITSYS_NONE;
		nUnit[22]=D_UNITSYS_NONE;
		nUnit[23]=D_UNITSYS_NONE;

		for(int i=0; i<3; i++)dblForce[i]=nUnit[i];
		for(int i = 0; i<21; i++)dblStress[i]=nUnit[i+3];
	}
	int dblForce[3];
	int dblStress[21];
	int nCount;
	int nUnit[24];
};

// axisymmetric
struct T_STAX_D
{
// dblForce  : global force , 4 node       , component : FX, FY, FZ
// dblStress : global stress, cent + 4 node, component : SXX, SYY, SZZ, SXY, SYZ, SXZ, P1, P2, P3, MAX-SHEAR, EFF-STRS, OCT-SHEAR,
//                             P1(ux,uy,uz), P2(ux,uy,uz), P3(ux,uy,uz)
	double dblForce[4][3];
	double dblStress[1+4][21];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STAX_D));
		//memset(dblForce,0,sizeof(dblForce));
		//memset(dblStress,0,sizeof(dblStress));
	}
	void Max(T_STAX_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++){if(Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];}
			}
			for(int j=0; j<12; j++)
			{
				if(Data.dblStress[i][j] > dblStress[i][j])
				{
					dblStress[i][j]=Data.dblStress[i][j];
					if(j == 6){for(int k=12; k<15; k++)dblStress[i][k]=Data.dblStress[i][k];}
					if(j == 7){for(int k=15; k<18; k++)dblStress[i][k]=Data.dblStress[i][k];}
					if(j == 8){for(int k=18; k<21; k++)dblStress[i][k]=Data.dblStress[i][k];}
				}
			}
		}
	}
	void Min(T_STAX_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++){if(Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];}
			}
			for(int j=0; j<12; j++)
			{
				if(Data.dblStress[i][j] < dblStress[i][j])
				{
					dblStress[i][j]=Data.dblStress[i][j];
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data.dblStress[i][l];}
				}
			}
		}
	}
	void AbsMax(T_STAX_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=__max(fabs(dblForce[i][j]),fabs(Data.dblForce[i][j]));
			}
			for(int j=0; j<12; j++)
			{
				if(fabs(Data.dblStress[i][j]) > fabs(dblStress[i][j]))
				{
					dblStress[i][j]=fabs(Data.dblStress[i][j]);
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data.dblStress[i][l];}
				}
				else
				{
					dblStress[i][j]=fabs(dblStress[i][j]);
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=dblStress[i][l];}
				}
			}
		}
	}
	void AbsMax(T_STAX_D& Data1, T_STAX_D& Data2)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=__max(fabs(Data1.dblForce[i][j]),fabs(Data2.dblForce[i][j]));
			}
			for(int j=0; j<12; j++)
			{
				if(fabs(Data2.dblStress[i][j]) > fabs(Data1.dblStress[i][j]))
				{
					dblStress[i][j]=fabs(Data2.dblStress[i][j]);
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data2.dblStress[i][l];}
				}
				else
				{
					dblStress[i][j]=fabs(Data1.dblStress[i][j]);
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data1.dblStress[i][l];}
				}
			}
		}
	}
	void AbsMaxwithSign(T_STAX_D& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)
				{
					a = dblForce[i][j]; b = Data.dblForce[i][j];
					dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dblForce[i][j] : Data.dblForce[i][j]);
					// dblForce[i][j]=(fabs(dblForce[i][j]) > fabs(Data.dblForce[i][j]) ? dblForce[i][j] : Data.dblForce[i][j]);
				}
			}
			for(int j=0; j<12; j++)
			{
				// if(fabs(Data.dblStress[i][j]) > fabs(dblStress[i][j]))
				a = dblStress[i][j]; b = Data.dblStress[i][j];
				if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a) )
				{
					dblStress[i][j]=Data.dblStress[i][j];
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data.dblStress[i][l];}
				}
				/*
				else
				{
					dblStress[i][j]=dblStress[i][j];
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=dblStress[i][l];}
				}
				*/
			}
		}
	}
	void AbsMaxwithSign(T_STAX_D& Data1, T_STAX_D& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)
				{
					a = Data1.dblForce[i][j]; b = Data2.dblForce[i][j];
					dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
					// dblForce[i][j]=(fabs(Data1.dblForce[i][j]) >= fabs(Data2.dblForce[i][j]) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
				}
			}
			for(int j=0; j<12; j++)
			{
				// if(fabs(Data2.dblStress[i][j]) > fabs(Data1.dblStress[i][j]))
				a = Data1.dblStress[i][j]; b = Data2.dblStress[i][j];
				if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a)) 
				{
					dblStress[i][j]=Data2.dblStress[i][j];
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data2.dblStress[i][l];}
				}
				else
				{
					dblStress[i][j]=Data1.dblStress[i][j];
					if(j == 6){for(int l=12; l<15; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data1.dblStress[i][l];}
				}
			}
		}
	}
	void Add(T_STAX_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]+=Data.dblForce[i][j];
			}
			for(int j=0; j<12; j++)
			{
				dblStress[i][j]+=Data.dblStress[i][j];
				for(int l=12; l<15; l++)dblStress[i][l]=0.;
				for(int l =15; l<18; l++)dblStress[i][l]=0.;
				for(int l =18; l<21; l++)dblStress[i][l]=0.;
			}
		}
	}
	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=fabs(dblForce[i][j]);
			}
			for(int j=0; j<12; j++)dblStress[i][j]=fabs(dblStress[i][j]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=dblForce[i][j]*dblForce[i][j];
			}
			for(int j=0; j<12; j++)
			{
				dblStress[i][j]=dblStress[i][j]*dblStress[i][j];
				for(int l=12; l<15; l++)dblStress[i][l]=0.;
				for(int l =15; l<18; l++)dblStress[i][l]=0.;
				for(int l =18; l<21; l++)dblStress[i][l]=0.;
			}
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=sqrt(dblForce[i][j]);
			}
			for(int j=0; j<12; j++)
			{
				dblStress[i][j]=sqrt(dblStress[i][j]);
				for(int l=12; l<15; l++)dblStress[i][l]=0.;
				for(int l =15; l<18; l++)dblStress[i][l]=0.;
				for(int l =18; l<21; l++)dblStress[i][l]=0.;
			}
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]*=dFactor;
			}
			for(int j=0; j<12; j++)dblStress[i][j]*=dFactor;
		}
	}
};
struct T_STAX_UNIT
{
	T_STAX_UNIT()
	{
		nCount=24;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;

		nUnit[3]=D_UNITSYS_BASE_STRESS;
		nUnit[4]=D_UNITSYS_BASE_STRESS;
		nUnit[5]=D_UNITSYS_BASE_STRESS;
		nUnit[6]=D_UNITSYS_BASE_STRESS;
		nUnit[7]=D_UNITSYS_BASE_STRESS;
		nUnit[8]=D_UNITSYS_BASE_STRESS;
		nUnit[9]=D_UNITSYS_BASE_STRESS;
		nUnit[10]=D_UNITSYS_BASE_STRESS;
		nUnit[11]=D_UNITSYS_BASE_STRESS;
		nUnit[12]=D_UNITSYS_BASE_STRESS;
		nUnit[13]=D_UNITSYS_BASE_STRESS;
		nUnit[14]=D_UNITSYS_BASE_STRESS;

		nUnit[15]=D_UNITSYS_NONE;
		nUnit[16]=D_UNITSYS_NONE;
		nUnit[17]=D_UNITSYS_NONE;
		nUnit[18]=D_UNITSYS_NONE;
		nUnit[19]=D_UNITSYS_NONE;
		nUnit[20]=D_UNITSYS_NONE;
		nUnit[21]=D_UNITSYS_NONE;
		nUnit[22]=D_UNITSYS_NONE;
		nUnit[23]=D_UNITSYS_NONE;

		for(int i=0; i<3; i++)dblForce[i]=nUnit[i];
		for(int i = 0; i<21; i++)dblStress[i]=nUnit[i+3];
	}
	int dblForce[3];
	int dblStress[21];
	int nCount;
	int nUnit[24];
};

// planestrain
struct T_STPN_D
{
// dblForce  : global force , 4 node       , component : FX, FY, FZ
// dblStress : global stress, cent + 4 node, component : SXX, SYY, SZZ, SXY, SYZ, SXZ, P1, P2, P3, MAX-SHEAR, EFF-STRS, OCT-SHEAR,
//                             P1(ux,uy,uz), P2(ux,uy,uz), P3(ux,uy,uz)
	double dblForce[4][3];
	double dblStress[1+4][21];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STPN_D));
		//memset(dblForce,0,sizeof(dblForce));
		//memset(dblStress,0,sizeof(dblStress));
	}
	void Max(T_STPN_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++){if(Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];}
			}
			for(int j=0; j<11; j++)
			{
				if(Data.dblStress[i][j] > dblStress[i][j])
				{
					dblStress[i][j]=Data.dblStress[i][j];
					if(j == 6){for(int k=13; k<15; k++)dblStress[i][k]=Data.dblStress[i][k];}
					if(j == 7){for(int k=15; k<18; k++)dblStress[i][k]=Data.dblStress[i][k];}
					if(j == 8){for(int k=18; k<21; k++)dblStress[i][k]=Data.dblStress[i][k];}
				}
			}
		}
	}
	void Min(T_STPN_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++){if(Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];}
			}
			for(int j=0; j<11; j++)
			{
				if(Data.dblStress[i][j] < dblStress[i][j])
				{
					dblStress[i][j]=Data.dblStress[i][j];
					if(j == 6){for(int l=13; l<15; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data.dblStress[i][l];}
				}
			}
		}
	}
	void AbsMax(T_STPN_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=__max(fabs(dblForce[i][j]),fabs(Data.dblForce[i][j]));
			}
			for(int j=0; j<11; j++)
			{
				if(fabs(Data.dblStress[i][j]) > fabs(dblStress[i][j]))
				{
					dblStress[i][j]=fabs(Data.dblStress[i][j]);
					if(j == 6){for(int l=13; l<15; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data.dblStress[i][l];}
				}
				else
				{
					dblStress[i][j]=fabs(dblStress[i][j]);
					if(j == 6){for(int l=13; l<15; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=dblStress[i][l];}
				}
			}
		}
	}
	void AbsMax(T_STPN_D& Data1, T_STPN_D& Data2)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=__max(fabs(Data1.dblForce[i][j]),fabs(Data2.dblForce[i][j]));
			}
			for(int j=0; j<11; j++)
			{
				if(fabs(Data2.dblStress[i][j]) > fabs(Data1.dblStress[i][j]))
				{
					dblStress[i][j]=fabs(Data2.dblStress[i][j]);
					if(j == 6){for(int l=13; l<15; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data2.dblStress[i][l];}
				}
				else
				{
					dblStress[i][j]=fabs(Data1.dblStress[i][j]);
					if(j == 6){for(int l=13; l<15; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data1.dblStress[i][l];}
				}
			}
		}
	}
	void AbsMaxwithSign(T_STPN_D& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)
				{
					a = dblForce[i][j]; b = Data.dblForce[i][j];
					dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dblForce[i][j] : Data.dblForce[i][j]);
					// dblForce[i][j]=(fabs(dblForce[i][j]) >= fabs(Data.dblForce[i][j]) ? dblForce[i][j] : Data.dblForce[i][j]);
				}
			}
			for(int j=0; j<11; j++)
			{
				// if(fabs(Data.dblStress[i][j]) > fabs(dblStress[i][j]))
				a = dblStress[i][j]; b = Data.dblStress[i][j];
				if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a) )
				{
					dblStress[i][j]=Data.dblStress[i][j];
					if(j == 6){for(int l=13; l<15; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data.dblStress[i][l];}
				}
				/*
				else
				{
					dblStress[i][j]=dblStress[i][j];
					if(j == 6){for(int l=13; l<15; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=dblStress[i][l];}
				}
				*/
			}
		}
	}
	void AbsMaxwithSign(T_STPN_D& Data1, T_STPN_D& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)
				{
					a = Data1.dblForce[i][j]; b = Data2.dblForce[i][j];
					dblForce[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
					// dblForce[i][j]=(fabs(Data1.dblForce[i][j]) >= fabs(Data2.dblForce[i][j]) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
				}
			}
			for(int j=0; j<11; j++)
			{
				// if(fabs(Data2.dblStress[i][j]) > fabs(Data1.dblStress[i][j]))
				a = Data1.dblStress[i][j]; b = Data2.dblStress[i][j];
				if ( fabs(b)-fabs(a) > dZero || (fabs(fabs(b)-fabs(a)) <= dZero && b > a)) 
				{
					dblStress[i][j]=Data2.dblStress[i][j];
					if(j == 6){for(int l=13; l<15; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data2.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data2.dblStress[i][l];}
				}
				else
				{
					dblStress[i][j]=Data1.dblStress[i][j];
					if(j == 6){for(int l=13; l<15; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 7){for(int l=15; l<18; l++)dblStress[i][l]=Data1.dblStress[i][l];}
					if(j == 8){for(int l=18; l<21; l++)dblStress[i][l]=Data1.dblStress[i][l];}
				}
			}
		}
	}
	void Add(T_STPN_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]+=Data.dblForce[i][j];
			}
			for(int j=0; j<11; j++)
			{
				dblStress[i][j]+=Data.dblStress[i][j];
				for(int l=13; l<15; l++)dblStress[i][l]=0.;
				for(int l=15; l<18; l++)dblStress[i][l]=0.;
				for(int l=18; l<21; l++)dblStress[i][l]=0.;
			}
		}
	}
	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=fabs(dblForce[i][j]);
			}
			for(int j=0; j<11; j++)dblStress[i][j]=fabs(dblStress[i][j]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=dblForce[i][j]*dblForce[i][j];
			}
			for(int j=0; j<11; j++)
			{
				dblStress[i][j]=dblStress[i][j]*dblStress[i][j];
				for(int l=13; l<15; l++)dblStress[i][l]=0.;
				for(int l =15; l<18; l++)dblStress[i][l]=0.;
				for(int l =18; l<21; l++)dblStress[i][l]=0.;
			}
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]=sqrt(dblForce[i][j]);
			}
			for(int j=0; j<11; j++)
			{
				dblStress[i][j]=sqrt(dblStress[i][j]);
				for(int l=13; l<15; l++)dblStress[i][l]=0.;
				for(int l =15; l<18; l++)dblStress[i][l]=0.;
				for(int l =18; l<21; l++)dblStress[i][l]=0.;
			}
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			if(i < 4)
			{
				for(int j=0; j<3; j++)dblForce[i][j]*=dFactor;
			}
			for(int j=0; j<11; j++)dblStress[i][j]*=dFactor;
		}
	}
};
struct T_STPN_UNIT
{
	T_STPN_UNIT()
	{
		nCount=24;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;

		nUnit[3]=D_UNITSYS_BASE_STRESS;
		nUnit[4]=D_UNITSYS_BASE_STRESS;
		nUnit[5]=D_UNITSYS_BASE_STRESS;
		nUnit[6]=D_UNITSYS_BASE_STRESS;
		nUnit[7]=D_UNITSYS_BASE_STRESS;
		nUnit[8]=D_UNITSYS_BASE_STRESS;
		nUnit[9]=D_UNITSYS_BASE_STRESS;
		nUnit[10]=D_UNITSYS_BASE_STRESS;
		nUnit[11]=D_UNITSYS_BASE_STRESS;
		nUnit[12]=D_UNITSYS_BASE_STRESS;
		nUnit[13]=D_UNITSYS_BASE_STRESS;
		nUnit[14]=D_UNITSYS_BASE_STRESS;

		nUnit[15]=D_UNITSYS_NONE;
		nUnit[16]=D_UNITSYS_NONE;
		nUnit[17]=D_UNITSYS_NONE;
		nUnit[18]=D_UNITSYS_NONE;
		nUnit[19]=D_UNITSYS_NONE;
		nUnit[20]=D_UNITSYS_NONE;
		nUnit[21]=D_UNITSYS_NONE;
		nUnit[22]=D_UNITSYS_NONE;
		nUnit[23]=D_UNITSYS_NONE;

		for(int i=0; i<3; i++)dblForce[i]=nUnit[i];
		for(int i = 0; i<21; i++)dblStress[i]=nUnit[i+3];
	}
	int dblForce[3];
	int dblStress[21];
	int nCount;
	int nUnit[24];
};

// wall
struct __MY_EXT_CLASS__ T_STRW_D
{
// dblForce     : local force         , top, bot
//                component : Fx, Fy, Fz, Mx, My, Mz
	double dblForce[2][6];
	double dForceTime[2][6];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STRW_D));
		//memset(dblForce,0,sizeof(dblForce));
	}
	void Max(T_STRW_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
				if(Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
	}
	void Min(T_STRW_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
				if(Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
	}
	void AbsMax(T_STRW_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=__max(fabs(dblForce[i][j]),fabs(Data.dblForce[i][j]));
		}
	}
	void AbsMax(T_STRW_D& Data1, T_STRW_D& Data2)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=__max(fabs(Data1.dblForce[i][j]),fabs(Data2.dblForce[i][j]));
		}
	}
	void AbsMaxwithSign(T_STRW_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=(fabs(dblForce[i][j]) >= fabs(Data.dblForce[i][j]) ? dblForce[i][j] : Data.dblForce[i][j]);
		}
	}
	void AbsMaxwithSign(T_STRW_D& Data1, T_STRW_D& Data2)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=(fabs(Data1.dblForce[i][j]) >= fabs(Data2.dblForce[i][j]) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
		}
	}
	void Add(T_STRW_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]+=Data.dblForce[i][j];
		}
	}
	void Abs()
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=fabs(dblForce[i][j]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=dblForce[i][j]*dblForce[i][j];
		}
	}
	void Root()
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=sqrt(dblForce[i][j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]*=dFactor;
		}
	}
};
struct T_STRW_UNIT
{
	T_STRW_UNIT()
	{
		dblForce[0]=D_UNITSYS_BASE_FORCE;
		dblForce[1]=D_UNITSYS_BASE_FORCE;
		dblForce[2]=D_UNITSYS_BASE_FORCE;
		dblForce[3]=D_UNITSYS_BASE_MOMENT;
		dblForce[4]=D_UNITSYS_BASE_MOMENT;
		dblForce[5]=D_UNITSYS_BASE_MOMENT;
	}
	int dblForce[6];
};

// Response Spectrum story shear
struct T_STSR_D
{
	double dInertiaForce[2];
	double dStoryShear[2];
	double dSpringForce[2];
	double dStoryShearWithSpring[2];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STSR_D));
		//dInertiaForce[0]=0.;
		//dInertiaForce[1]=0.;
		//dStoryShear[0]=0.;
		//dStoryShear[1]=0.;
		//dSpringForce[0]=0.;
		//dSpringForce[1]=0.;
		//dStoryShearWithSpring[0]=0.;
		//dStoryShearWithSpring[1]=0.;
	}
};
struct T_STSR_UNIT
{
	T_STSR_UNIT()
	{
		dInertiaForce=D_UNITSYS_BASE_FORCE;
		dStoryShear=D_UNITSYS_BASE_FORCE;
		dSpringForce=D_UNITSYS_BASE_FORCE;
		dStoryShearWithSpring=D_UNITSYS_BASE_FORCE;
	}
	int dInertiaForce;
	int dStoryShear;
	int dSpringForce;
	int dStoryShearWithSpring;
};

// Response Spectrum story shear  // Dummy Story 정보 (다탑의 경계층 추가 결과)
struct T_STSD_D
{
	double dStoryShear[2];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STSD_D));
		//dStoryShear[0]=0.;
		//dStoryShear[1]=0.;
	}
};
struct T_STSD_UNIT
{
	T_STSD_UNIT()
	{
		dStoryShear=D_UNITSYS_BASE_FORCE;
	}
	int dStoryShear;
};

// Time History story shear
struct T_THSS_D
{
	double dMax_X;
	double dMin_X;
	double MaxTime_X;
	double MinTime_X;
	double dMax_Y;
	double dMin_Y;
	double MaxTime_Y;
	double MinTime_Y;
	void Initialize()
	{
		memset(this, 0, sizeof(T_THSS_D));
		//dMax_X=0.;
		//dMin_X=0.;
		//MaxTime_X=0.;
		//MinTime_X=0.;
		//dMax_Y=0.;
		//dMin_Y=0.;
		//MaxTime_Y=0.;
		//MinTime_Y=0.;
	}
};
struct T_THSS_UNIT
{
	T_THSS_UNIT()
	{
		dMax_X=D_UNITSYS_BASE_FORCE;
		dMin_X=D_UNITSYS_BASE_FORCE;
		MaxTime_X=D_UNITSYS_BASE_TIME;
		MinTime_X=D_UNITSYS_BASE_TIME;
		dMax_Y=D_UNITSYS_BASE_FORCE;
		dMin_Y=D_UNITSYS_BASE_FORCE;
		MaxTime_Y=D_UNITSYS_BASE_TIME;
		MinTime_Y=D_UNITSYS_BASE_TIME;
	}
	int dMax_X;
	int dMin_X;
	int MaxTime_X;
	int MinTime_X;
	int dMax_Y;
	int dMin_Y;
	int MaxTime_Y;
	int MinTime_Y;
};

// story shear ratio
#define T_STRD_KEY unsigned int
union T_STSRD_K
{
	T_STRD_KEY keymap;
	struct
	{
		unsigned int entity1 : 12;   // 0-4096
		unsigned int entity2 : 12;   // 0-4096
		unsigned int type : 8;   // 0-255
	}key;
};
#define T_SSRT_KEY unsigned int
union T_SSRT_K
{
	T_SSRT_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int type   : 12;   // 0-4096
	}key;
};
struct T_SSRT_D
{
	double dShearX;
	double dShearY;
	double dSpringForceX;
	double dSpringForceY;
	T_STOR_K StorKey;
	void Initialize()
	{
		memset(this, 0, sizeof(T_SSRT_D));
		//dShearX=0;
		//dShearY=0;
		//dSpringForceX=0;
		//dSpringForceY=0;
		//StorKey=0;
	}
};
struct T_SSRT_UNIT
{
	T_SSRT_UNIT()
	{
		dShearX=D_UNITSYS_BASE_FORCE;
		dShearY=D_UNITSYS_BASE_FORCE;
		dSpringForceX=D_UNITSYS_BASE_FORCE;
		dSpringForceY=D_UNITSYS_BASE_FORCE;
	}
	int dShearX;
	int dShearY;
	int dSpringForceX;
	int dSpringForceY;
};

// Elastic Link
struct T_STEK_D
{
// dblForce  : local force , 2 node       , component : fx, fy, fz, mx, my, mz
	double dblForce[2][6];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STEK_D));
		//memset(dblForce,0,sizeof(dblForce));
	}
	void Max(T_STEK_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
				if(Data.dblForce[i][j] > dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
	}
	void Min(T_STEK_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
				if(Data.dblForce[i][j] < dblForce[i][j])dblForce[i][j]=Data.dblForce[i][j];
		}
	}
	void AbsMax(T_STEK_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=__max(fabs(dblForce[i][j]),fabs(Data.dblForce[i][j]));
		}
	}
	void AbsMax(T_STEK_D& Data1, T_STEK_D& Data2)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=__max(fabs(Data1.dblForce[i][j]),fabs(Data2.dblForce[i][j]));
		}
	}
	void AbsMaxwithSign(T_STEK_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=(fabs(dblForce[i][j]) >= fabs(Data.dblForce[i][j]) ? dblForce[i][j] : Data.dblForce[i][j]);
		}
	}
	void AbsMaxwithSign(T_STEK_D& Data1, T_STEK_D& Data2)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=(fabs(Data1.dblForce[i][j]) >= fabs(Data2.dblForce[i][j]) ? Data1.dblForce[i][j] : Data2.dblForce[i][j]);
		}
	}
	void Add(T_STEK_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]+=Data.dblForce[i][j];
		}
	}
	void Abs()
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=fabs(dblForce[i][j]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=dblForce[i][j]*dblForce[i][j];
		}
	}
	void Root()
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]=sqrt(dblForce[i][j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]*=dFactor;
		}
	}
};
struct T_STEK_UNIT
{
	T_STEK_UNIT()
	{
		dblForce[0]=D_UNITSYS_BASE_FORCE;
		dblForce[1]=D_UNITSYS_BASE_FORCE;
		dblForce[2]=D_UNITSYS_BASE_FORCE;
		dblForce[3]=D_UNITSYS_BASE_MOMENT;
		dblForce[4]=D_UNITSYS_BASE_MOMENT;
		dblForce[5]=D_UNITSYS_BASE_MOMENT;
	}
	int dblForce[6];
};


// Elastic Link concurrent
struct T_STCK_D
{
	// dblForce  : local force , 2 node       , component : fx, fy, fz, mx, my, mz
	double dblForce[2][6][6];
	void Initialize()
	{
		memset(this, 0, sizeof(T_STCK_D));
		//memset(dblForce,0,sizeof(dblForce));
	}
	void Max(T_STCK_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
			{
				if(Data.dblForce[i][j][j] >= dblForce[i][j][j])
				{
					 for(int k=0; k<6; k++) dblForce[i][j][k]=Data.dblForce[i][j][k];
				}
			}
		}
	}
	void Min(T_STCK_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
			{
				if(Data.dblForce[i][j][j] < dblForce[i][j][j])
				{
					for(int k=0; k<6; k++) dblForce[i][j][k]=Data.dblForce[i][j][k];
				}
			}
		}
	}
	void AbsMax(T_STCK_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
			{
				if(fabs(Data.dblForce[i][j][j]) > fabs(dblForce[i][j][j]))
				{
					for(int k=0; k<6; k++) dblForce[i][j][k]=fabs(Data.dblForce[i][j][k]);
				}
			}
		}
	}
	void AbsMax(T_STCK_D& Data1, T_STCK_D& Data2)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
			{
				if(fabs(Data1.dblForce[i][j][j]) >= fabs(Data2.dblForce[i][j][j]))
					for(int k=0; k<6; k++) dblForce[i][j][k]=fabs(Data1.dblForce[i][j][k]);
				else
					for(int k=0; k<6; k++) dblForce[i][j][k]=fabs(Data2.dblForce[i][j][k]);
			}
		}
	}
	void AbsMaxwithSign(T_STCK_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
			{
				if(fabs(Data.dblForce[i][j][j]) >= fabs(dblForce[i][j][j]))
				{
					for(int k=0; k<6; k++) dblForce[i][j][k]= Data.dblForce[i][j][k];
				}
			}
		}
	}
	void AbsMaxwithSign(T_STCK_D& Data1, T_STCK_D& Data2)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
			{
				if(fabs(Data1.dblForce[i][j][j]) >= fabs(Data2.dblForce[i][j][j]))
					for(int k=0; k<6; k++) dblForce[i][j][k]= Data1.dblForce[i][j][k];
				else 
					for(int k=0; k<6; k++) dblForce[i][j][k]= Data2.dblForce[i][j][k];
			}
		}
	}
	void Add(T_STCK_D& Data)
	{
		for(int i=0; i<2; i++) for(int j=0; j<6; j++) for(int k=0; k<6; k++) 
		{
			dblForce[i][j][k]+=Data.dblForce[i][j][k];
		}
	}
	void Abs()
	{
		for(int i=0; i<2; i++) for(int j=0; j<6; j++) for(int k=0; k<6; k++)
		{
			dblForce[i][j][k]=fabs(dblForce[i][j][k]);
		}
	}
	void Sqr()
	{
		for(int i=0; i<2; i++) for(int j=0; j<6; j++) for(int k=0; k<6; k++)
		{
			dblForce[i][j][k]=dblForce[i][j][k]*dblForce[i][j][k];
		}
	}
	void Root()
	{
		for(int i=0; i<2; i++) for(int j=0; j<6; j++) for(int k=0; k<6; k++)
		{
			dblForce[i][j][k]=sqrt(dblForce[i][j][k]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<2; i++) for(int j=0; j<6; j++) for(int k=0; k<6; k++)
		{
			dblForce[i][j][k]*=dFactor;
		}
	}
	void ConvertToStck(T_STEK_D& DataStek)
	{
		Initialize();
		for(int i=0; i<2; i++) // i, j
		{
			for(int k=0; k<6; k++) dblForce[i][0][k]=DataStek.dblForce[i][k];

			for(int j=1; j<6; j++) memcpy(dblForce[i][j], dblForce[i][0], sizeof(dblForce[i][0]));
		}
	}
};

struct T_STCK_UNIT
{
	T_STCK_UNIT()
	{
		dblForce[0]=D_UNITSYS_BASE_FORCE;
		dblForce[1]=D_UNITSYS_BASE_FORCE;
		dblForce[2]=D_UNITSYS_BASE_FORCE;
		dblForce[3]=D_UNITSYS_BASE_MOMENT;
		dblForce[4]=D_UNITSYS_BASE_MOMENT;
		dblForce[5]=D_UNITSYS_BASE_MOMENT;
	}
	int dblForce[6];
};



// Partial Model Mirror
#define T_MIRE_KEY unsigned int
union T_MIRE_K
{
	T_MIRE_KEY keymap;
	struct
	{
		unsigned int elem_k   : 20;   // 0-1048576
		unsigned int mirror_k : 11;   // 0-2048
		unsigned int mirror_f : 1 ;   // 1;
	}key;
};

#define T_MIRN_KEY unsigned int
union T_MIRN_K
{
	T_MIRN_KEY keymap;
	struct
	{
		unsigned int node_k   : 20;   // 0-1048576
		unsigned int mirror_k : 11;   // 0-2048
		unsigned int mirror_f : 1 ;   // 1;
	}key;
};

struct T_TDFI_D
{
	int nStageId;
	CArray<double, double> aTdfi;

	void Initialize()
	{
		nStageId = 0;
		aTdfi.RemoveAll();
	}

	T_TDFI_D() {}
	T_TDFI_D(T_TDFI_D& src) { *this = src; }
	T_TDFI_D& operator=(const T_TDFI_D& src)
	{
		nStageId = src.nStageId;
		aTdfi.Copy(src.aTdfi);
		return *this;
	}
};

// Tendon Elongation
struct T_TDEG_D
{
	int nStepId;            // 긴장된 스테이지,스텝 순번
	double dElongation[6];  // Tendon 수축량 I,J, 요소수축량 I,J, 두개합한값 I,J
};

struct T_TDGE_UNIT
{
	T_TDGE_UNIT()
	{
		dElongation=D_UNITSYS_BASE_LENGTH;
	}
	int dElongation;
};


struct T_MDSP_D
{
	double dDispl;
	double dVelocity;
	double dAcceleration;
	void Initialize()
	{
		memset(this, 0, sizeof(T_MDSP_D));
		//dDispl=0.;
		//dVelocity=0.;
		//dAcceleration=0.;
	}
};


struct _MDSP_BN
{
	double dDispl;
	double dVelocity;
	double dAcceleration;
	void Initialize()
	{
		memset(this, 0, sizeof(_MDSP_BN));
		//dDispl=0.0;
		//dVelocity=0.0;
		//dAcceleration=0.0;
	}
};

struct T_STORY_INFO
{
	double dCenter_X;
	double dCenter_Y;
	double dStiffnessCenter_X;
	double dStiffnessCenter_Y;
	double dEccentricDistance_X;
	double dEccentricDistance_Y;
	double dTorsionStiffness;
	double dElasticRadius_X;
	double dElasticRadius_Y;
	double dEccentricRatio_X;
	double dEccentricRatio_Y;
	void Initialize()
	{
		memset(this, 0, sizeof(T_STORY_INFO));
		//dCenter_X=0.;
		//dCenter_Y=0.;
		//dStiffnessCenter_X=0.;
		//dStiffnessCenter_Y=0.;
		//dEccentricDistance_X=0.;
		//dEccentricDistance_Y=0.;
		//dTorsionStiffness=0.;
		//dElasticRadius_X=0.;
		//dElasticRadius_Y=0.;
		//dEccentricRatio_X=0.;
		//dEccentricRatio_Y=0.;
	}
};

struct T_HINGEID
{
	int nHingeID[5];
};

struct T_HGST_D
{
	int nHingeStatus[5][3];
	void Initialize()
	{
		memset(this, 0, sizeof(T_HGST_D));
		//memset(nHingeStatus, 0, sizeof(nHingeStatus));
	}
};

// General Link Hinge Status
struct T_GLHP_D
{
	int nHingeStatus[6];
	int nTotalHingeNum;
	void Initialize()
	{
		memset(this, 0, sizeof(T_GLHP_D));
		nTotalHingeNum = 0; 
	}
};

// Pushover Hinge Step
struct T_FHYS_D
{
	int nHingeStep[2];
	void Initialize()
	{
		memset(this, 0, sizeof(T_FHYS_D));
	}
};

struct T_TENDON_INFO_BASE
{
	double dPositionRatio_x;  // 요소길이에 대한 비
	double dTendonLength;     // 텐던 시작부에서 현위치까지 누적된 텐던길이
	double dTendonAngle;      // 텐던 시작부에서 현위치까지 누적된 각변화
	double dTendonPosition_y;
	double dTendonPosition_z;
	double dPlaneNormal[3];
	double dNoTensionLengthRatio;   // 무효력장길이에 대한 현재 위치의 비, 구간 밖은 1.0
	double dInAngle_y;        // 해당 위치의 텐던 입사각, 출사각. 2005.11.23 V692에 추가..
	double dInAngle_z;
	double dOutAngle_y;
	double dOutAngle_z;
	BOOL bDebonded;     // dNoTensionLengthRatio 가 0인 구간

	// PT Slab
	int nEdge;           // Edge 번호 (1~4, 0이면 내부 시작)
	double dEdge;        // Edge의 시작점으로부터의 거리 (Edge 방향 거리)
	T_NODE_D EdgePos;    // Edge 위치
	T_NODE_D TendonPos;  // Tendon 위치

	void Initialize()
	{
		memset(this, 0, sizeof(T_TENDON_INFO_BASE));
		//dPositionRatio_x=0.;
		//dTendonLength=0.;
		//dTendonAngle=0.;
		//dTendonPosition_y=0.;
		//dTendonPosition_z=0.;
		//dPlaneNormal[0]=dPlaneNormal[1]=dPlaneNormal[2]=0.;
		dNoTensionLengthRatio=1.;
		bDebonded = FALSE;
		EdgePos.Initialize();
		TendonPos.Initialize();
	}
	T_TENDON_INFO_BASE() {Initialize();}
	T_TENDON_INFO_BASE(T_TENDON_INFO_BASE& src) { *this = src;}
	T_TENDON_INFO_BASE& operator=(const T_TENDON_INFO_BASE& src)
	{
		dPositionRatio_x=src.dPositionRatio_x;
		dTendonLength=src.dTendonLength;
		dTendonAngle=src.dTendonAngle;
		dTendonPosition_y=src.dTendonPosition_y;
		dTendonPosition_z=src.dTendonPosition_z;
		dPlaneNormal[0]=src.dPlaneNormal[0];
		dPlaneNormal[1]=src.dPlaneNormal[1];
		dPlaneNormal[2]=src.dPlaneNormal[2];
		dNoTensionLengthRatio=src.dNoTensionLengthRatio;
		bDebonded=src.bDebonded;
		dInAngle_y=src.dInAngle_y; 
		dInAngle_z=src.dInAngle_z;
		dOutAngle_y=src.dOutAngle_y;
		dOutAngle_z=src.dOutAngle_z;

		nEdge = src.nEdge;
		dEdge = src.dEdge;
		EdgePos = src.EdgePos;
		TendonPos = src.TendonPos;

		return *this;
	}
};

struct T_TENDON_INFO_ELEM
{
	T_ELEM_K KeyElem;
	int nSpanId;
	double dElemLength;
	BOOL bReverseIJ;
	CArray<T_TENDON_INFO_BASE, T_TENDON_INFO_BASE&>aTendonInfoBase;
	void Initialize()
	{
		KeyElem=0;
		nSpanId = 0;
		dElemLength=0.;
		bReverseIJ=FALSE;
		aTendonInfoBase.RemoveAll();
	}
	T_TENDON_INFO_ELEM() {Initialize();}
	T_TENDON_INFO_ELEM(T_TENDON_INFO_ELEM& src) {*this = src;}
	T_TENDON_INFO_ELEM& operator=(const T_TENDON_INFO_ELEM& src)
	{
		KeyElem=src.KeyElem;
		nSpanId = src.nSpanId;
		dElemLength=src.dElemLength;
		bReverseIJ=src.bReverseIJ;
		aTendonInfoBase.Copy(src.aTendonInfoBase);
		return *this;
	}
};

struct T_TENDON_INFO
{
	T_TDNA_K KeyTdna;      // Tendon Profile Key or T_TDSL_K (PT Slab)
	CArray<T_TENDON_INFO_ELEM, T_TENDON_INFO_ELEM&>aTendonInfoElem;
	double dNoTensionBeginLen;
	double dNoTensionEndLen;
	double dTendonNum;  // 대표텐던, V700 추가.
	int nSerialId;

	UINT TdstK;  // PT Slab - Stress Key

	void Initialize()
	{
		KeyTdna=0;
		aTendonInfoElem.RemoveAll();
		dNoTensionBeginLen=dNoTensionEndLen=0.;
		dTendonNum=1.0;
		nSerialId=0;

		TdstK = 0;
	}
	T_TENDON_INFO() { Initialize(); }
	T_TENDON_INFO(T_TENDON_INFO& src) {	*this = src;}
	T_TENDON_INFO& operator=(const T_TENDON_INFO& src)
	{
		KeyTdna=src.KeyTdna;
		aTendonInfoElem.Copy(src.aTendonInfoElem);
		dNoTensionBeginLen=src.dNoTensionBeginLen;
		dNoTensionEndLen=src.dNoTensionEndLen;
		dTendonNum=src.dTendonNum;
		nSerialId=src.nSerialId;

		TdstK = src.TdstK;
		return *this;
	}
};

struct T_TENDON_ELEMINFO_TENDON_BASE
{
	double dPositionRatio_x;
	double dDistance_y;
	double dDistance_z;
	double dNoTensionLengthRatio;   // 무효력장길이에 대한 현재 위치의 비, 구간 밖은 1.0
	double dInAngle_y;        // 해당 위치의 텐던 입사각, 출사각. 2005.11.23 V692에 추가..
	double dInAngle_z;
	double dOutAngle_y;
	double dOutAngle_z;
	BOOL bDebonded;

	// PT Slab
	int nEdge;           // Edge 번호 (1~4, 0이면 내부 시작)
	double dEdge;        // Edge의 시작점으로부터의 거리 (Edge 방향 거리)
	T_NODE_D EdgePos;    // Edge 위치
	T_NODE_D TendonPos;  // Tendon 위치

	void Initialize()
	{
		memset(this, 0, sizeof(T_TENDON_ELEMINFO_TENDON_BASE));
		dNoTensionLengthRatio=1.;   // 무효력장길이에 대한 현재 위치의 비, 구간 밖은 1.0
		bDebonded = FALSE;          // dNoTensionLengthRatio=0 인 영역
		EdgePos.Initialize();
		TendonPos.Initialize();
	}
	T_TENDON_ELEMINFO_TENDON_BASE() { Initialize();}
	T_TENDON_ELEMINFO_TENDON_BASE(T_TENDON_ELEMINFO_TENDON_BASE& src) {	*this = src;}
	T_TENDON_ELEMINFO_TENDON_BASE& operator=(const T_TENDON_ELEMINFO_TENDON_BASE& src)
	{
		dPositionRatio_x=src.dPositionRatio_x;
		dDistance_y=src.dDistance_y;
		dDistance_z=src.dDistance_z;
		dNoTensionLengthRatio=src.dNoTensionLengthRatio;
		bDebonded=src.bDebonded;
		dInAngle_y=src.dInAngle_y; 
		dInAngle_z=src.dInAngle_z;
		dOutAngle_y=src.dOutAngle_y;
		dOutAngle_z=src.dOutAngle_z;

		nEdge = src.nEdge;
		dEdge = src.dEdge;
		EdgePos = src.EdgePos;
		TendonPos = src.TendonPos;
		return *this;
	}
};

struct T_TENDON_ELEMINFO_TENDON
{
	T_TDNA_K KeyTdna;   // Tendon Profile Key or T_TDSL_K (PT Slab)
	BOOL bPrestressed;  // 마지막 스테이지에서 긴장된 상태의 텐던인가?
	double dTendonNum;     // 대표텐던, V700 추가.
	CArray<T_TENDON_ELEMINFO_TENDON_BASE, T_TENDON_ELEMINFO_TENDON_BASE&>aTendonElemInfoTendonBase;
	void Initialize()
	{
		KeyTdna=0;
		bPrestressed=FALSE;
		dTendonNum=1;
		aTendonElemInfoTendonBase.RemoveAll();
	}
	T_TENDON_ELEMINFO_TENDON()	{Initialize();}
	T_TENDON_ELEMINFO_TENDON(T_TENDON_ELEMINFO_TENDON& src) {*this = src;}
	T_TENDON_ELEMINFO_TENDON& operator=(const T_TENDON_ELEMINFO_TENDON& src)
	{
		aTendonElemInfoTendonBase.Copy(src.aTendonElemInfoTendonBase);
		KeyTdna=src.KeyTdna;
		bPrestressed=src.bPrestressed;
		dTendonNum=src.dTendonNum;
		return *this;
	}
};

#define TD_NA 0
#define TD_BM 1
#define TD_SL 2
struct T_TENDON_ELEMINFO
{
	double dLength;
	BOOL bLastStage;  // 마지막 스테이지에 있는 요소인가?
	int nSpanId;
	CArray<T_TENDON_ELEMINFO_TENDON, T_TENDON_ELEMINFO_TENDON&>aTendonElemInfoTendon;
	void Initialize()
	{
		dLength=0.;
		bLastStage=FALSE;
		aTendonElemInfoTendon.RemoveAll();
	}
	T_TENDON_ELEMINFO() { Initialize(); }
	T_TENDON_ELEMINFO(T_TENDON_ELEMINFO& src) {	*this = src;}
	T_TENDON_ELEMINFO& operator=(const T_TENDON_ELEMINFO& src)
	{
		dLength=src.dLength;
		aTendonElemInfoTendon.Copy(src.aTendonElemInfoTendon);
		bLastStage=src.bLastStage;
		return *this;
	}
};


struct T_TDFS_D
{
	double dForce_I[4]; // Total, Fx, Fy, Fz
	double dSlope_I[3]; //        Slope_x, Slope_y, Slope_z
	double dForce_J[4];
	double dSlope_J[3];

	int nTendonID;      // PT Slab에서 사용됨

	void Initialize()
	{
		memset(dForce_I, 0, sizeof(dForce_I));
		memset(dSlope_I, 0, sizeof(dSlope_I));
		memset(dForce_J, 0, sizeof(dForce_J));
		memset(dSlope_J, 0, sizeof(dSlope_J));

		nTendonID = 0;
	}
	T_TDFS_D() {Initialize();}
	T_TDFS_D(T_TDFS_D& src){*this = src;}
	T_TDFS_D& operator=(const T_TDFS_D& src)
	{
		memcpy(dForce_I, src.dForce_I, sizeof(dForce_I));
		memcpy(dSlope_I, src.dSlope_I, sizeof(dSlope_I));
		memcpy(dForce_J, src.dForce_J, sizeof(dForce_J));
		memcpy(dSlope_J, src.dSlope_J, sizeof(dSlope_J));

		nTendonID = src.nTendonID;
		return *this;
	}

	void Mult(double dFactor)
	{
		if (dFactor == 1.)return;
		for (int j = 0; j < 4; j++)
		{
			dForce_I[j] *= dFactor;
			dForce_J[j] *= dFactor;
		}
	}

};

#define T_DTFS_D T_TDFS_D

struct T_INELASTICDYN_HINGE_INFO_BASE
{
	int nHingeDirection;  // 0 ~ 5
	int nHingeType;       // 0=Spring, 1=Sectional
	int nHingeLocation;   // 0:I, 1:J, 2:I&J, 3:Center (if nHingeType == 0)
	int nNumHinge;        // Number of Section
	int nStartHingeId;
	int nStartHingeId_SelectOuputOnly; // Siho, Sihg 고려, Sihp 고려
	BOOL bLumpType;       // 0:Distribute, 1:Lumped (HingeType:0,1 & Pushover)
	int  nSkcvType;       // 0,2:BiLinear, 1,3:TriLinear, 4:FEMA, 5:EC8 6:Comp. Only
	int  nHysModel;
	void Initialize()
	{
		nHingeDirection=0;
		nHingeType=0;
		nHingeLocation=0;
		nNumHinge=0;       
		nStartHingeId=0;
		nStartHingeId_SelectOuputOnly = 0;
		bLumpType=TRUE;
		nSkcvType=0;
		nHysModel=0;
	}
};

#define INELASTIC_ELEM_TYPE_BEAM  1
#define INELASTIC_ELEM_TYPE_NLNK  2
#define INELASTIC_ELEM_TYPE_TRUSS 3
#define INELASTIC_ELEM_TYPE_WALL  4
#define INELASTIC_ELEM_TYPE_NSPR  5
struct T_INELASTICDYN_HINGE_INFO
{
	int nElemType; //  1:Beam, 2:Nonlinear Link, 3:Truss
	BOOL bSelectOutput; // Select Inelastic Hinge Result Output
	CArray<T_INELASTICDYN_HINGE_INFO_BASE, T_INELASTICDYN_HINGE_INFO_BASE&>aInelasticDynHingeInfo;
	void Initialize()
	{
		aInelasticDynHingeInfo.RemoveAll();
	}
	T_INELASTICDYN_HINGE_INFO()
	{
	}
	T_INELASTICDYN_HINGE_INFO(T_INELASTICDYN_HINGE_INFO& src)
	{
		*this = src;
	}
	T_INELASTICDYN_HINGE_INFO& operator=(const T_INELASTICDYN_HINGE_INFO& src)
	{
		nElemType = src.nElemType;
		bSelectOutput = src.bSelectOutput;
		aInelasticDynHingeInfo.Copy(src.aInelasticDynHingeInfo);
		return *this;
	}
};

struct T_NLHG_D
{
	double dForceMoment;
	double dDisplacement;
	double dDuctility_Pos;
	double dDuctility_Neg;
	double dYeildDuctility_Pos;
	double dYeildDuctility_Neg;
	int    nHingeStatusLevel_Pos;
	int    nHingeStatusLevel_Neg;
	double dHingeStatus_Pos; // Linear:1.0, Crack:2.0, Yield=3.0, Ultimate:4.0
	double dHingeStatus_Neg; // Linear:-1.0, Crack:-2.0, Yield=-3.0, Ultimate:-4.0
	double dD1[2]; //0:Pos, 1:Neg
	double dD2[2]; //0:Pos, 1:Neg
	double dD3[2]; //0:Pos, 1:Neg
	double dP1[2]; //0:Pos, 1:Neg
	double dP2[2]; //0:Pos, 1:Neg
	double dP3[2]; //0:Pos, 1:Neg
	BOOL b945New;
	double dCrackTime;
	double dYieldTime;
	double dUltimateTime;

	void Initialize()
	{
		dForceMoment=0.;
		dDisplacement=0.;
		dDuctility_Pos=0.;
		dDuctility_Neg=0.;
		dYeildDuctility_Pos=0.;
		dYeildDuctility_Neg=0.;
		nHingeStatusLevel_Pos=0;
		nHingeStatusLevel_Neg=0;
		dHingeStatus_Pos=0.;
		dHingeStatus_Neg=0.;
		dD1[0] = dD1[1] = 0.0;
		dD2[0] = dD2[1] = 0.0;
		dD3[0] = dD3[1] = 0.0;
		dP1[0] = dP1[1] = 0.0;
		dP2[0] = dP2[1] = 0.0;
		dP3[0] = dP3[1] = 0.0;
		b945New = FALSE;
		dCrackTime = 0.;
		dYieldTime = 0.;
		dUltimateTime = 0.;
	}
};

struct T_NLHG_TIME
{
	double dCrackTime;
	double dYieldTime;
	double dUltimateTime;
	void Initialize()
	{
		dCrackTime=0.;
		dYieldTime=0.;
		dUltimateTime=0.;
	}
};

struct T_AEFM_D
{
	double dEccenDist;
	double dStoryForce;
	double dStoryMoment;
	void Initialize()
	{
		dEccenDist=0.;
		dStoryForce=0.;
		dStoryMoment=0.;
	}
};

// 시공단계 최종단계 상태의 각 요소별 단면 특성값
struct T_BMPD_D
{
	double dStiffness[2][10];   // [i,j] [Area, Ixx, Iyy, Izz, Cyp, Cym, Czp, Czm, WArea, Iw] : 단면 특성 
	double dCentTrans[2][2];   // [dy, dz] [i,j] : 중립축 이동거리
	void Initialize()
	{
		memset(this, 0, sizeof(T_BMPD_D));
		//memset(dStiffness,0,sizeof(dStiffness));
		//memset(dCentTrans,0,sizeof(dCentTrans));
	}
};
struct T_BMPD_UNIT
{
	T_BMPD_UNIT()
	{
		dStiffness[0]=D_UNITSYS_BASE_AREA;
		dStiffness[1]=D_UNITSYS_BASE_STIF;
		dStiffness[2]=D_UNITSYS_BASE_STIF;
		dStiffness[3]=D_UNITSYS_BASE_STIF;
		dStiffness[4]=D_UNITSYS_BASE_LENGTH;
		dStiffness[5]=D_UNITSYS_BASE_LENGTH;
		dStiffness[6]=D_UNITSYS_BASE_LENGTH;
		dStiffness[7]=D_UNITSYS_BASE_LENGTH;
		dStiffness[8]=D_UNITSYS_BASE_AREA;
		dStiffness[9]=D_UNITSYS_BASE_L6;
		dCentTrans=D_UNITSYS_BASE_LENGTH;
	}
	int dStiffness[10];
	int dCentTrans;
};

// 부재의 시공단계 각 STAGE 별 특성값 저장 
struct T_ELFM_D
{
	double dStartMatAge; //각 시공단계에서의 시작 재령
	double dEndMatAge;   //각 시공단계에서의 마지막 재령
	double dStartElast;  //각 시공단계에서의 시작 재령의 탄성계수  
	double dEndElast;    //각 시공단계에서의 마지막 재령의 탄성계수  
	double dAddedCrSkStrain; //현단계까지 누적된 건조수축 변형량 
	double dCreepStrain; //부재의 생성시점을 기준에 재하되는 응력에 대한 크리프 변형률 
	void Initialize()
	{
		memset(this, 0, sizeof(T_ELFM_D));
		//dStartMatAge=0.0;
		//dEndMatAge=0.0;
		//dStartElast=0.0;
		//dEndElast=0.0;
		//dAddedCrSkStrain=0.0;
		//dCreepStrain=0.0;
	}
};
struct T_ELFM_UNIT
{
	T_ELFM_UNIT()
	{
		dStartElast = dEndElast = D_UNITSYS_BASE_ELAST;
	}
	int dStartElast;
	int dEndElast;
};

struct T_TDLC_D  // 텐던의 장력 손실량
{
	double dTendonLoss[3]; //[0] 탄성변형, [1]크리프/건조수축 [2]Relaxation 에 의한 손실
	void Initialize()
	{
		memset(this, 0, sizeof(T_TDLC_D));
		//dTendonLoss[0]=0.;
		//dTendonLoss[1]=0.;
		//dTendonLoss[2]=0.;
	}
};
struct T_TDLC_UNIT
{
	T_TDLC_UNIT()
	{
		dTendonLoss = D_UNITSYS_BASE_FORCE;
	}
	int dTendonLoss;
};

struct T_VMST_D
{
	double dblStress[4][56][7]; // [4 part][I:28개 단면지점,J:28개 단면지점][7개 성분]
															// 1. Normal
															// 2. Tau_xy
															// 3. Tau_xz
															// 4. Von-Mises
															// 5. Max-Shear
															// 6. Princ.(max)
															// 7. Princ.(min)
					
	int nPoint4EffectiveSect;

	void Initialize()
	{
		memset(this, 0, sizeof(T_VMST_D));
		nPoint4EffectiveSect = 0;
	}
	void AbsMaxwithSign(T_VMST_D& Data)
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<56; j++)
				for(int k=0; k<7; k++)
					dblStress[i][j][k]=(fabs(dblStress[i][j][k]) >= fabs(Data.dblStress[i][j][k]) ? dblStress[i][j][k] : Data.dblStress[i][j][k]);
	}
	void AbsMaxwithSign(T_VMST_D& Data1,T_VMST_D& Data2)
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<56; j++)
				for(int k=0; k<7; k++)
					dblStress[i][j][k]=(fabs(Data1.dblStress[i][j][k]) >= fabs(Data2.dblStress[i][j][k]) ? Data1.dblStress[i][j][k] : Data2.dblStress[i][j][k]);
	}
	void AbsMax(T_VMST_D& Data1, T_VMST_D& Data2)
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<56; j++)
				for(int k=0; k<7; k++)
				{
					dblStress[i][j][k]=__max(fabs(Data1.dblStress[i][j][k]),fabs(Data2.dblStress[i][j][k]));
				}
	}
	void Max(T_VMST_D& Data)
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<56; j++)
				for(int k=0; k<7; k++)
					if(Data.dblStress[i][j][k] > dblStress[i][j][k])dblStress[i][j][k]=Data.dblStress[i][j][k];
	}
	void Min(T_VMST_D& Data)
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<56; j++)
				for(int k=0; k<7; k++)
					if(Data.dblStress[i][j][k] < dblStress[i][j][k])dblStress[i][j][k]=Data.dblStress[i][j][k];
	}
	void Add(T_VMST_D& Data)
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<56; j++)
				for(int k=0; k<7; k++)
					dblStress[i][j][k]+=Data.dblStress[i][j][k];
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<4; i++)
			for(int j=0; j<56; j++)
				for(int k=0; k<7; k++)
					dblStress[i][j][k]*=dFactor;
	}
	void Abs()
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<56; j++)
				for(int k=0; k<7; k++)
					dblStress[i][j][k]=fabs(dblStress[i][j][k]);
	}
	void Sqr()
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<56; j++)
				for(int k=0; k<7; k++)
					dblStress[i][j][k]=dblStress[i][j][k]*dblStress[i][j][k];
	}
	void Root()
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<56; j++)
				for(int k=0; k<7; k++)
					dblStress[i][j][k]=sqrt(dblStress[i][j][k]);
	}
};

struct T_VMST_UNIT
{
	T_VMST_UNIT()
	{
		dblStress = D_UNITSYS_BASE_STRESS;
	}
	int dblStress;
};

//사인장, 전단응력 
struct T_STSS_D
{
	// Change by ZINU.('04.11.11). 6개 단면지점(yzV) -> 10개 단면지점(yzM+yzV), 7개 성분 -> 9개 성분(주응력2).
/*
	double dblStress[5][6][7]; // [5지점][6개 단면지점][7개 성분]
														 // 1.Axial + Moment-y + Moment-z + 강봉의 Axial
														 // 2.강봉에 의한 Z축응력
														 // 3.Shear z에 의한 전단응력
														 // 4.Torsion에 의한 전단응력 
														 // 5.강봉에 의한 전단응력
														 // 6.비틀림을 제외한 사인장 응력 
														 // 7.비틀림을 포함한 사인장 응력
*/
	/* KYE-HONG-20050604
	double dblStress[5][10][9]; // [5지점][10개 단면지점][9개 성분]
															// 1.Axial + Moment-y + Moment-z + 강봉의 Axial
															// 2.강봉에 의한 Z축응력
															// 3.Shear z에 의한 전단응력
															// 4.Torsion에 의한 전단응력 
															// 5.강봉에 의한 전단응력
															// 6.비틀림을 제외한 사인장 응력 
															// 7.비틀림을 포함한 사인장 응력
															// 8.주응력1 (Max)
															// 9.주응력2 (Min)
															*/
	/*
	double dblStress[5][10][12];// [5지점][10개 단면지점][12개 성분]
															// 1.Axial X Normal Stress
															// 2.Moment-y X Normal Stress 
															// 3.Moment-z X Normal Stress       
															// 4.강봉 X Normal Stres
															// 5.Sum
															// 6.강봉 z Normal Stress
															// 7.Shear z Shear Stress
															// 8.Torsion Shear Stress  
															// 9.강봉 Shear Stress
															// 10.비틀림 제외 사인장 응력 
															// 11.비틀림 포함 사인장 응력 / 주응력 최대 
															// 12.주응력 최소 
															*/
	double dblStress[5][16][19];// [5지점][16개 단면지점][19개 성분] // 10 + 6개
															// 1.Axial X Normal Stress
															// 2.Moment-y X Normal Stress 
															// 3.Moment-z X Normal Stress       
															// 4.강봉 X Normal Stres
															// 5.Sum
															// 6.강봉 z Normal Stress
															// 7.Shear z Shear Stress
															// 8.Torsion Shear Stress  
															// 9.강봉 Shear Stress
															// 10.비틀림 제외 사인장 응력 
															// 11.비틀림 포함 사인장 응력 / 주응력 최대 
															// 12.주응력 최소 
															// 13.warping normal stress
															// 14.pure torsion xy
															// 15.warping torsion xy
															// 16.pure torsion xz
															// 17.warping torsion xz
															// 18.summation shear xy
															// 19.summation shear xz
															
	void Initialize()
	{
		memset(this, 0, sizeof(T_STSS_D));
		//memset(dblStress,0,sizeof(dblStress));
	}

	void AbsMaxwithSign(T_STSS_D& Data)
	{
		for(int i=0; i<5; i++)
		for(int j=0; j<16; j++)
		for(int k=0; k<19; k++)
			dblStress[i][j][k]=(fabs(dblStress[i][j][k]) >= fabs(Data.dblStress[i][j][k]) ? dblStress[i][j][k] : Data.dblStress[i][j][k]);
	}
	void AbsMaxwithSign(T_STSS_D& Data1,T_STSS_D& Data2)
	{
		for(int i=0; i<5; i++)
		for(int j=0; j<16; j++)
		for(int k=0; k<19; k++)
			dblStress[i][j][k]=(fabs(Data1.dblStress[i][j][k]) >= fabs(Data2.dblStress[i][j][k]) ? Data1.dblStress[i][j][k] : Data2.dblStress[i][j][k]);
	}
	void AbsMax(T_STSS_D& Data1, T_STSS_D& Data2)
	{
		for(int i=0; i<5; i++)
		for(int j=0; j<16; j++)
		for(int k=0; k<19; k++)
		{
			dblStress[i][j][k]=__max(fabs(Data1.dblStress[i][j][k]),fabs(Data2.dblStress[i][j][k]));
		}
	}

	void Max(T_STSS_D& Data)
	{
		for(int i=0; i<5; i++)
		for(int j=0; j<16; j++)
		for(int k=0; k<19; k++)
			if(Data.dblStress[i][j][k] > dblStress[i][j][k])dblStress[i][j][k]=Data.dblStress[i][j][k];
	}
	void Min(T_STSS_D& Data)
	{
		for(int i=0; i<5; i++)
		for(int j=0; j<16; j++)
		for(int k=0; k<19; k++)
			if(Data.dblStress[i][j][k] < dblStress[i][j][k])dblStress[i][j][k]=Data.dblStress[i][j][k];
	}
	void Add(T_STSS_D& Data)
	{
		for(int i=0; i<5; i++)
		for(int j=0; j<16; j++)
		for(int k=0; k<19; k++)
			dblStress[i][j][k]+=Data.dblStress[i][j][k];
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		for(int j=0; j<16; j++)
		for(int k=0; k<19; k++)
			dblStress[i][j][k]*=dFactor;
	}
	void Abs()
	{
		for(int i=0; i<5; i++)
		for(int j=0; j<10; j++)
		for(int k=0; k<19; k++)
			dblStress[i][j][k]=fabs(dblStress[i][j][k]);
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		for(int j=0; j<16; j++)
		for(int k=0; k<19; k++)
			dblStress[i][j][k]=dblStress[i][j][k]*dblStress[i][j][k];
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		for(int j=0; j<16; j++)
		for(int k=0; k<19; k++)
			dblStress[i][j][k]=sqrt(dblStress[i][j][k]);
	}

};

struct T_STSS_UNIT
{
	T_STSS_UNIT()
	{
		dblStress = D_UNITSYS_BASE_STRESS;
	}
	int dblStress;
};

//Beam Section Temperature하중에 의한 자기구속응력
struct T_BTSS_D
{
	double dblStress[5][16];    // Local-z에대한 Stress [5지점][4개 합성응력 + 6개 전단응력 계산위치의 응력 + 6개 포인트(7DOF)]
	double dblStressMz[5][16];  // Local-y에대한 Stress [5지점][4개 합성응력 + 6개 전단응력 계산위치의 응력 + 6개 포인트(7DOF)] : V692에서 추가 
	double dblLineStress[5][18]; // [5지점][Local-y 9개 Line + Local-z 9개 Line]
	double dN;  //Beam Section Temperature에의한 축력
	double dMy; //Beam Section Temperature에의한 모멘트
	double dMz; //Beam Section Temperature에의한 모멘트

	void Initialize()
	{
		memset(this, 0, sizeof(T_BTSS_D));
		//memset(dblStress,0,sizeof(dblStress));
		dN=0.0;
		dMy=0.0;
		dMz=0.0;
	}

	void AbsMaxwithSign(T_BTSS_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<16; j++)
				dblStress[i][j]=(fabs(dblStress[i][j]) >= fabs(Data.dblStress[i][j]) ? dblStress[i][j] : Data.dblStress[i][j]);
			for (int j = 0; j<16; j++)
				dblStressMz[i][j]=(fabs(dblStressMz[i][j]) >= fabs(Data.dblStressMz[i][j]) ? dblStressMz[i][j] : Data.dblStressMz[i][j]);
			for (int j = 0; j<18; j++)
				dblLineStress[i][j]=(fabs(dblLineStress[i][j]) >= fabs(Data.dblLineStress[i][j]) ? dblLineStress[i][j] : Data.dblLineStress[i][j]);
		}
		dN=(fabs(dN) >= fabs(Data.dN) ? dN : Data.dN);
		dMy=(fabs(dMy) >= fabs(Data.dMy) ? dMy : Data.dMy);
		dMz=(fabs(dMz) >= fabs(Data.dMz) ? dMz : Data.dMz);
	}
	void AbsMaxwithSign(T_BTSS_D& Data1,T_BTSS_D& Data2)
	{
		for(int i=0; i<5; i++)
		{
		for(int j=0; j<16; j++)
			dblStress[i][j]=(fabs(Data1.dblStress[i][j]) >= fabs(Data2.dblStress[i][j]) ? Data1.dblStress[i][j] : Data2.dblStress[i][j]);
		for (int j = 0; j<16; j++)
			dblStressMz[i][j]=(fabs(Data1.dblStressMz[i][j]) >= fabs(Data2.dblStressMz[i][j]) ? Data1.dblStressMz[i][j] : Data2.dblStressMz[i][j]);
		for (int j = 0; j<18; j++)
			dblLineStress[i][j]=(fabs(Data1.dblLineStress[i][j]) >= fabs(Data2.dblLineStress[i][j]) ? Data1.dblLineStress[i][j] : Data2.dblLineStress[i][j]);
		}
		dN=(fabs(Data1.dN) >= fabs(Data2.dN) ? Data1.dN : Data2.dN);
		dMy=(fabs(Data1.dMy) >= fabs(Data2.dMy) ? Data1.dMy : Data2.dMy);
		dMz=(fabs(Data1.dMz) >= fabs(Data2.dMz) ? Data1.dMz : Data2.dMz);
	}
	void AbsMax(T_BTSS_D& Data1, T_BTSS_D& Data2)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<16; j++)
				dblStress[i][j]=__max(fabs(Data1.dblStress[i][j]),fabs(Data2.dblStress[i][j]));
			for (int j = 0; j<16; j++)
				dblStressMz[i][j]=__max(fabs(Data1.dblStressMz[i][j]),fabs(Data2.dblStressMz[i][j]));
			for (int j = 0; j<18; j++)
				dblLineStress[i][j]=__max(fabs(Data1.dblLineStress[i][j]),fabs(Data2.dblLineStress[i][j]));
		}
		dN=__max(fabs(Data1.dN),fabs(Data2.dN));
		dMy=__max(fabs(Data1.dMy),fabs(Data2.dMy));
		dMz=__max(fabs(Data1.dMz),fabs(Data2.dMz));
	}

	void Max(T_BTSS_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<16; j++)
				if(Data.dblStress[i][j] > dblStress[i][j])dblStress[i][j]=Data.dblStress[i][j];
			for(int j = 0; j<16; j++)
				if(Data.dblStressMz[i][j] > dblStressMz[i][j])dblStressMz[i][j]=Data.dblStressMz[i][j];
			for(int j = 0; j<18; j++)
				if(Data.dblLineStress[i][j] > dblLineStress[i][j])dblLineStress[i][j]=Data.dblLineStress[i][j];
		}

		if(Data.dN > dN) dN=Data.dN;
		if(Data.dMy > dMy) dMy=Data.dMy;
		if(Data.dMz > dMz) dMz=Data.dMz;
	}
	void Min(T_BTSS_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<16; j++)
				if(Data.dblStress[i][j] < dblStress[i][j])dblStress[i][j]=Data.dblStress[i][j];
			for(int j = 0; j<16; j++)
				if(Data.dblStressMz[i][j] < dblStressMz[i][j])dblStressMz[i][j]=Data.dblStressMz[i][j];
			for(int j = 0; j<18; j++)
				if(Data.dblLineStress[i][j] < dblLineStress[i][j])dblLineStress[i][j]=Data.dblLineStress[i][j];
		}

		if(Data.dN < dN) dN=Data.dN;
		if(Data.dMy < dMy) dMy=Data.dMy;
		if(Data.dMz < dMz) dMz=Data.dMz;
	}
	void Add(T_BTSS_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<16; j++)
				dblStress[i][j]+=Data.dblStress[i][j];
			for(int j = 0; j<16; j++)
				dblStressMz[i][j]+=Data.dblStressMz[i][j];
			for(int j = 0; j<18; j++)
				dblLineStress[i][j]+=Data.dblLineStress[i][j];
		}

		dN+=Data.dN;
		dMy+=Data.dMy;
		dMz+=Data.dMz;
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<16; j++)
				dblStress[i][j]*=dFactor;
			for(int j = 0; j<16; j++)
				dblStressMz[i][j]*=dFactor;  
			for(int j = 0; j<18; j++)
				dblLineStress[i][j]*=dFactor;
		}
		dN*=dFactor;
		dMy*=dFactor;
		dMz*=dFactor;
	}
	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<16; j++)
				dblStress[i][j]=fabs(dblStress[i][j]);
			for(int j=0; j<16; j++)
				dblStressMz[i][j]=fabs(dblStressMz[i][j]);  
			for(int j=0; j<18; j++)
				dblLineStress[i][j]=fabs(dblLineStress[i][j]);
		}

		dN=fabs(dN);
		dMy=fabs(dMy);
		dMz=fabs(dMz);
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<16; j++)
				dblStress[i][j]=dblStress[i][j]*dblStress[i][j];
			for(int j=0; j<16; j++)
				dblStressMz[i][j]=dblStressMz[i][j]*dblStressMz[i][j];
			for(int j=0; j<18; j++)
				dblLineStress[i][j]=dblLineStress[i][j]*dblLineStress[i][j];
		}
		dN=dN*dN;
		dMy=dMy*dMy;
		dMz=dMz*dMz;
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<16; j++)
				dblStress[i][j]=sqrt(dblStress[i][j]);
			for(int j=0; j<16; j++)
				dblStressMz[i][j]=sqrt(dblStressMz[i][j]);
			for(int j=0; j<18; j++)
				dblLineStress[i][j]=sqrt(dblLineStress[i][j]);
		}
		dN=sqrt(dN);
		dMy=sqrt(dMy);
		dMz=sqrt(dMz);
	}

};

struct T_BTSS_UNIT
{
	T_BTSS_UNIT()
	{
		dblStress = D_UNITSYS_BASE_STRESS;
		dblLineStress = D_UNITSYS_BASE_STRESS;
		dN = D_UNITSYS_BASE_FORCE;
		dMy = D_UNITSYS_BASE_MOMENT;
		dMz = D_UNITSYS_BASE_MOMENT;
	}
	int dblStress;
	int dblLineStress;
	int dN;
	int dMy;
	int dMz;
};

//Beam Section Temperature하중에 의한 자기구속응력(시공단계)
struct T_BTMS_D
{
	double dblLineStress[5][18]; // [5지점][Local-y 9개 Line + Local-z 9개 Line]
	double dN;  //Beam Section Temperature에의한 축력
	double dMy; //Beam Section Temperature에의한 모멘트
	double dMz; //Beam Section Temperature에의한 모멘트

	void Initialize()
	{
		memset(this, 0, sizeof(T_BTMS_D));
		//memset(dblStress,0,sizeof(dblStress));
		dN=0.0;
		dMy=0.0;
		dMz=0.0;
	}

	void AbsMaxwithSign(T_BTMS_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<18; j++)
				dblLineStress[i][j]=(fabs(dblLineStress[i][j]) >= fabs(Data.dblLineStress[i][j]) ? dblLineStress[i][j] : Data.dblLineStress[i][j]);
		}
		dN=(fabs(dN) >= fabs(Data.dN) ? dN : Data.dN);
		dMy=(fabs(dMy) >= fabs(Data.dMy) ? dMy : Data.dMy);
		dMz=(fabs(dMz) >= fabs(Data.dMz) ? dMz : Data.dMz);
	}
	void AbsMaxwithSign(T_BTMS_D& Data1,T_BTMS_D& Data2)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<18; j++)
				dblLineStress[i][j]=(fabs(Data1.dblLineStress[i][j]) >= fabs(Data2.dblLineStress[i][j]) ? Data1.dblLineStress[i][j] : Data2.dblLineStress[i][j]);
		}
		dN=(fabs(Data1.dN) >= fabs(Data2.dN) ? Data1.dN : Data2.dN);
		dMy=(fabs(Data1.dMy) >= fabs(Data2.dMy) ? Data1.dMy : Data2.dMy);
		dMz=(fabs(Data1.dMz) >= fabs(Data2.dMz) ? Data1.dMz : Data2.dMz);
	}
	void AbsMax(T_BTMS_D& Data1, T_BTMS_D& Data2)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<18; j++)
				dblLineStress[i][j]=__max(fabs(Data1.dblLineStress[i][j]),fabs(Data2.dblLineStress[i][j]));
		}
		dN=__max(fabs(Data1.dN),fabs(Data2.dN));
		dMy=__max(fabs(Data1.dMy),fabs(Data2.dMy));
		dMz=__max(fabs(Data1.dMz),fabs(Data2.dMz));
	}

	void Max(T_BTMS_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<18; j++)
				if(Data.dblLineStress[i][j] > dblLineStress[i][j])dblLineStress[i][j]=Data.dblLineStress[i][j];
		}
		if(Data.dN > dN) dN=Data.dN;
		if(Data.dMy > dMy) dMy=Data.dMy;
		if(Data.dMz > dMz) dMz=Data.dMz;
	}
	void Min(T_BTMS_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<18; j++)
				if(Data.dblLineStress[i][j] < dblLineStress[i][j])dblLineStress[i][j]=Data.dblLineStress[i][j];
		}
		if(Data.dN < dN) dN=Data.dN;
		if(Data.dMy < dMy) dMy=Data.dMy;
		if(Data.dMz < dMz) dMz=Data.dMz;
	}
	void Add(T_BTMS_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<18; j++)
				dblLineStress[i][j]+=Data.dblLineStress[i][j];
		}
		dN+=Data.dN;
		dMy+=Data.dMy;
		dMz+=Data.dMz;
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<18; j++)
				dblLineStress[i][j]*=dFactor;
		}
		dN*=dFactor;
		dMy*=dFactor;
		dMz*=dFactor;
	}
	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<18; j++)
				dblLineStress[i][j]=fabs(dblLineStress[i][j]);
		}
		dN=fabs(dN);
		dMy=fabs(dMy);
		dMz=fabs(dMz);
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<18; j++)
				dblLineStress[i][j]=dblLineStress[i][j]*dblLineStress[i][j];
		}
		dN=dN*dN;
		dMy=dMy*dMy;
		dMz=dMz*dMz;
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<18; j++)
				dblLineStress[i][j]=sqrt(dblLineStress[i][j]);
		}
		dN=sqrt(dN);
		dMy=sqrt(dMy);
		dMz=sqrt(dMz);
	}

};

struct T_BTMS_UNIT
{
	T_BTMS_UNIT()
	{
		dblLineStress = D_UNITSYS_BASE_STRESS;
		dN = D_UNITSYS_BASE_FORCE;
		dMy = D_UNITSYS_BASE_MOMENT;
		dMz = D_UNITSYS_BASE_MOMENT;
	}
	int dblLineStress;
	int dN;
	int dMy;
	int dMz;

};

struct T_THAM_STEP_BASE_D
{
	double    dStep;
	CString   strName;
	void Initialize()
	{
		dStep = 0.0;
		strName.Empty(); 
	}
};

#define T_THEX_K UINT
#define T_THAM_K UINT
#define T_THPO_K UINT

//Time History Extension Load Case, Animation Load Case
struct T_THEX_D
{
	CString strName; 
	CArray<T_THAM_STEP_BASE_D, T_THAM_STEP_BASE_D&>arStep;

	void Initialize()
	{
		strName.Empty();
		arStep.RemoveAll(); 
	}
	T_THEX_D(){;}
	T_THEX_D(T_THEX_D& src)
	{
		*this = src;
	}
	T_THEX_D& operator=(const T_THEX_D& src)
	{
		strName = src.strName;
		arStep.Copy(src.arStep);
		return *this;
	}
};

struct T_THAM_D
{
	CString strName; 
	CArray<T_THAM_STEP_BASE_D, T_THAM_STEP_BASE_D&>arStep;

	void Initialize()
	{
		strName.Empty();
		arStep.RemoveAll(); 
	}
	T_THAM_D(){;}
	T_THAM_D(T_THAM_D& src)
	{
		*this = src;
	}
	T_THAM_D& operator=(const T_THAM_D& src)
	{
		strName = src.strName;
		arStep.Copy(src.arStep);
		return *this;
	}
};

struct T_THPO_D
{
	CString strName; 
	CArray<T_THAM_STEP_BASE_D, T_THAM_STEP_BASE_D&>arStep;

	void Initialize()
	{
		strName.Empty();
		arStep.RemoveAll(); 
	}
	T_THPO_D(){;}
	T_THPO_D(T_THPO_D& src)
	{
		*this = src;
	}
	T_THPO_D& operator=(const T_THPO_D& src)
	{
		strName = src.strName;
		arStep.Copy(src.arStep);
		return *this;
	}
};

// Event Bar(Beam)
struct T_EVTB_D
{
	int nEvent[2][6]; // [I,J][DOF1,DOF2,DOF3,DOF4,DOF5,DOF6] 
										// 0:선형, 1:균열, 2:항복, 3:종국 
	void Initialize()
	{
		memset(nEvent,0,sizeof(nEvent));
	}

	void Add(T_EVTB_D& Data)
	{
		for(int i=0; i<2; i++)
		for(int j=0; j<6; j++)
			nEvent[i][j]+=Data.nEvent[i][j];
	}
};

// Event Rod(Truss)
struct T_EVTR_D
{
	int nEvent[2]; // [I,J]
								 // 0:선형, 1:균열, 2:항복, 3:종국 
	void Initialize()
	{
		memset(nEvent,0,sizeof(nEvent));
	}
	void Add(T_EVTR_D& Data)
	{
		for(int i=0; i<2; i++)
			nEvent[i]+=Data.nEvent[i];
	}
};


// Event Spring(General Link)
struct T_EVTS_D
{
	int nEvent[2][6]; // [I,J][DOF1,DOF2,DOF3,DOF4,DOF5,DOF6] 
										// 0:선형, 1:균열, 2:항복, 3:종국 
	void Initialize()
	{
		memset(nEvent,0,sizeof(nEvent));
	}
	void Add(T_EVTS_D& Data)
	{
		for(int i=0; i<2; i++)
		for(int j=0; j<6; j++)
			nEvent[i][j]+=Data.nEvent[i][j];
	}
};

struct T_FBCH_D_28byte
{
	float dStress;
	float dStrain;
	float dStifReductionRatio; // K
	float dMinStrain;
	int nStatus;
	int nState;
	int nOutCellIdx; //대조조사기능에 의해 추가( 0:Inner Cell, 1: Outer Cell)
	void Initialize()
	{
		dStress=dStrain=0.0;
		nStatus=nState=0;
		dStifReductionRatio = dMinStrain = 0;
		nOutCellIdx = 0;
	}
};

// Fiber Model
struct T_FBCH_D
{
	float dStress;
	float dStrain;
	float dStifReductionRatio; // K
	float nStatus;
	float nState;
	float nOutCellIdx; //대조조사기능에 의해 추가( 0:Inner Cell, 1: Outer Cell)
	void Initialize()
	{
		dStress=dStrain=0.0;
		nStatus=nState=0;
		dStifReductionRatio = 0;
		nOutCellIdx = 0;
	}
	void ConvertFrom28byte(T_FBCH_D_28byte& data28)
	{
		dStress				= data28.dStress			;
		dStrain				= data28.dStrain			;
		dStifReductionRatio	= data28.dStifReductionRatio;
		nStatus				= (float)data28.nStatus			;
		nState				= (float)data28.nState				;
		nOutCellIdx			= (float)data28.nOutCellIdx		;
	}
};

// Fiber Model.. (개선전은 FBCH 에서 Step Maxm 를 전후처리에서 계산.. 개선후는 Solver 에서 계산)
struct T_FBCM_D
{
	float dTensile;
	float dComp;
	float dTensileTimePerStep;
	float dCompTimePerStep;
	float dTYPlus;
	float dCYMinus;
	float dCuMinus;
	int nState1; // if(Concrete) 0:Elastic, 1:Crack Steel elif(Steel) 0:Elastic, 1:Yielding
	int nState2; // if(Concrete) 0:Elastic, 1:Crack Steel, 2:Crushing elif(Steel) 0:Elastic, 1:Yielding
	void Initialize()
	{
		dTensile = 0.0;
		dComp = 0.0;
		dTensileTimePerStep = 0.0;
		dCompTimePerStep = 0.0;
		dTYPlus = 0.0;
		dCYMinus = 0.0;
		dCuMinus = 0.0;
		nState1 = 0;
		nState2 = 0;
	}
};

// Fiber Model Summary.. (개선전은 FBCH 에서 Step Maxm 를 전후처리에서 계산.. 개선후는 Solver 에서 계산)
struct T_FBSM_D
{
	float dConcTensile;
	float dConcTensileTimePerStep;
	float dConcTensileC;
	float dConcTensileCu;
	float dConcComp;	
	float dConcCompTimePerStep;
	float dConcCompC;
	float dConcCompCu;
	float dStlTensile;
	float dStlTensileTimePerStep;
	float dStlTensileYPlus;
	float dStlTensileYMinus;
	float dStlComp;	
	float dStlCompTimePerStep;
	float dStlCompYPlus;
	float dStlCompYMinus;
	int nCellNoConcMax;
	int nCellNoConcMin;
	int nCellNoStlMax;
	int nCellNoStlMin;
	int nCellStateConcMax;
	int nCellStateConcMin;
	int nCellStateStlMax;
	int nCellStateStlMin;

	void Initialize()
	{
		dConcTensile = 0.0;
		dConcTensileTimePerStep = 0.0;
		dConcTensileC = 0.0;
		dConcTensileCu = 0.0;
		dConcComp = 0.0;	
		dConcCompTimePerStep = 0.0;
		dConcCompC = 0.0;
		dConcCompCu = 0.0;
		dStlTensile = 0.0;
		dStlTensileTimePerStep = 0.0;
		dStlTensileYPlus = 0.0;
		dStlTensileYMinus = 0.0;
		dStlComp = 0.0;	
		dStlCompTimePerStep = 0.0;
		dStlCompYPlus = 0.0;
		dStlCompYMinus = 0.0;
		nCellNoConcMax = 0;
		nCellNoConcMin = 0;
		nCellNoStlMax = 0;
		nCellNoStlMin = 0;
		nCellStateConcMax = 0;
		nCellStateConcMin = 0;
		nCellStateStlMax = 0;
		nCellStateStlMin = 0;
	}
};

// Fiber Section Yield Status
struct T_FBYS_D
{
	double nDirYorZ; // Y or Z 를 판단하는 값(Solver 에서 넘겨줌)
	double nCrackStepDirY;
	double nYieldStepDirY;
	double nCrackStepDirZ;
	double nYieldStepDirZ;

	void Initialize()
	{
		nDirYorZ = 0;
		nCrackStepDirY = 0;
		nYieldStepDirY = 0;
		nCrackStepDirZ = 0;
		nYieldStepDirZ = 0;
	}
};

// Fiber Section material strain (min/max)
struct T_BFBM_BASE_D
{
	float nFlag;
	float nMatlId;
	float nMatlType;
	float nCellIdx;
	float dStrain; // CONCRETE MIN. STRAIN (Compressive) or STEEL MAX STRAIN ( tensile )
	void Initialize()
	{
		nFlag = 0;
		nMatlId = 0;
		nMatlType = 0;
		nCellIdx = 0;
		dStrain = 0;
	}
};
struct T_BFBM_D
{	
	T_BFBM_BASE_D aData[6];
	float nElemId;

	void Initialize()
	{
		for (int i = 0; i < 6; i++)
			aData[i].Initialize();
		nElemId = 0;
	}
};

// Rotation Angle
struct T_AGDS_D
{
	double d_i_EndRotationY; // i 단의 Y 방향 회전각
	double d_i_EndRotationZ; // i 단의 Z 방향 회전각
	double d_j_EndRotationY; // j 단의 Y 방향 회전각
	double d_j_EndRotationZ; // j 단의 Z 방향 회전각

	void Initialize()
	{
		d_i_EndRotationY = 0.;
		d_i_EndRotationZ = 0.;
		d_j_EndRotationY = 0.;
		d_j_EndRotationZ = 0.;
	}
};

// Pushover 를 비선형동적해석으로
struct T_POSF_D
{
	double dFactor;
	void Initialize()
	{
		dFactor=0.;
	}
};

enum { DEFORM_POS, DEFORM_NEG };

struct T_FHNE_D
{
	double dForceMoment;
	double dDisplacement;
	double dPlasticDeform;
	double dDuctility_Pos;
	double dDuctility_Neg;
	double dYieldDuctility_Pos;
	double dYieldDuctility_Neg;
	double dHingeStatus_Pos; // Linear:1.0, Crack:2.0, Yield=3.0, Ultimate:4.0
	double dHingeStatus_Neg; // Linear:-1.0, Crack:-2.0, Yield=-3.0, Ultimate:-4.0
//   double dP1;
//   double dP2;
//   double dD1;
//   double dD2;
	double dInitStiff;
	double dNewPlasticDeformation;   // Plastic Deform (D-D Elas) 
	double dPerformance_Pos;
	double dPerformance_Neg;
	double adP1[2]; //0:Pos, 1:Neg
	double adP2[2]; //0:Pos, 1:Neg
	double adD1[2]; //0:Pos, 1:Neg
	double adD2[2]; //0:Pos, 1:Neg

	void Initialize()
	{
		memset(this, 0, sizeof(T_FHNE_D));
	}	

	double GetP1() const { return dDisplacement < 0.0 ? adP1[DEFORM_NEG] : adP1[DEFORM_POS]; }
	double GetP2() const { return dDisplacement < 0.0 ? adP2[DEFORM_NEG] : adP2[DEFORM_POS]; }
	double GetD1() const { return dDisplacement < 0.0 ? adD1[DEFORM_NEG] : adD1[DEFORM_POS]; }
	double GetD2() const { return dDisplacement < 0.0 ? adD2[DEFORM_NEG] : adD2[DEFORM_POS]; }
};

struct T_EVAL2ND_PHNE
{
	int    nHingeType;
	int    nDof;
	double dForceMoment;
	double dDisplacement;
	double dDuctility;			// fabs(dDuctility_Pos) >= fabs(dDuctility_Neg) ? dDuctility_Pos : dDuctility_Neg
	double dDuctility_Pos;
	double dDuctility_Neg;
	double dP1_Pos;
	double dP1_Neg;
	double dD1_Pos;
	double dD1_Neg;
	double dP2_Pos;
	double dP2_Neg;
	double dD2_Pos;
	double dD2_Neg;
	double dInitStiff;
	double dIO;
	double dLS;
	double dCP;
    bool bEvalForcTerm;
    double dChiGamma; /// KISTEC2019 : Chi*Gamma | AIK-G-001-2021 : 1.2 (constant value).
    double dQUF;  /// KISTEC2019 : QUF | AIK-G-001-2021 : Fs.
    double dQG;   /// KISTEC2019 : QG  | AIK-G-001-2021 : Fns.
    double dQCN;  /// KISTEC2019 : QCN | AIK-G-001-2021 : phi*Fn.
    double dQUFapp; /// KISTEC2019 : QUFapp = Chi*Gamma*(QUF-QG) + QG | AIK-G-001-2021 : Fsapp = 1.2*(Fs-Fns) + Fns.
    double dRatio;  /// KISTEC2019 : QUFapp/QCN | AIK-G-001-2021 : Fspp/phi*Fn.
	UINT enLevel_Deform;
	UINT enLevel_Force;
	UINT enLevel;
	BOOL bEvaluateForce;
	BOOL bDeformGood;
	double dTime; // time history인 경우.
	bool bCount0;

	void Initialize()
	{
		nHingeType		= 0;
		nDof			= 0;
		dForceMoment	= 0.0;
		dDisplacement	= 0.0;
		dDuctility		= 0.0;
		dDuctility_Pos	= 0.0;
		dDuctility_Neg	= 0.0;
		dP1_Pos			= 0.0;
		dP1_Neg			= 0.0;
		dD1_Pos			= 0.0;
		dD1_Neg			= 0.0;
		dP2_Pos			= 0.0;
		dP2_Neg			= 0.0;
		dD2_Pos			= 0.0;
		dD2_Neg			= 0.0;
		dInitStiff		= 0.0;
		dIO				= 0.0;
		dLS				= 0.0;
		dCP				= 0.0;
        bEvalForcTerm	= false;
        dChiGamma		= 0.0;
		dQUF			= 0.0;
		dQG				= 0.0;
		dQCN			= 0.0;
		dQUFapp			= 0.0;
		dRatio			= 0.0;
		enLevel_Deform	= 0;
		enLevel_Force	= 0;
		enLevel			= 0;
		bEvaluateForce	= FALSE;
		bDeformGood		= TRUE;
		dTime = 0.0;
		bCount0 = false;
	}

	T_EVAL2ND_PHNE() { Initialize(); }
	T_EVAL2ND_PHNE(const T_EVAL2ND_PHNE& src) { *this = src; }
	T_EVAL2ND_PHNE& operator= (const T_EVAL2ND_PHNE& src)
	{
		nHingeType      = src.nHingeType;
		nDof            = src.nDof;
		dForceMoment	= src.dForceMoment;		
		dDisplacement	= src.dDisplacement;
		dDuctility    	= src.dDuctility;
		dDuctility_Pos	= src.dDuctility_Pos;
		dDuctility_Neg	= src.dDuctility_Neg;
		dP1_Pos			= src.dP1_Pos;
		dP1_Neg			= src.dP1_Neg;
		dD1_Pos			= src.dD1_Pos;
		dD1_Neg			= src.dD1_Neg;
		dP2_Pos			= src.dP2_Pos;
		dP2_Neg			= src.dP2_Neg;
		dD2_Pos			= src.dD2_Pos;
		dD2_Neg			= src.dD2_Neg;
		dInitStiff		= src.dInitStiff;
		dIO				= src.dIO;
		dLS				= src.dLS;
		dCP				= src.dCP;
        bEvalForcTerm   = src.bEvalForcTerm;
        dChiGamma       = src.dChiGamma;
        dQUF    = src.dQUF;
        dQG     = src.dQG;
        dQCN    = src.dQCN;
        dQUFapp = src.dQUFapp;
        dRatio  = src.dRatio;
		enLevel_Deform	= src.enLevel_Deform;
		enLevel_Force	= src.enLevel_Force;
		enLevel			= src.enLevel;
		bEvaluateForce	= src.bEvaluateForce;
		bDeformGood		= src.bDeformGood;
		dTime = src.dTime;
		bCount0 = src.bCount0;
		return *this;
	}
	double GetP1() const { return dDisplacement < 0.0 ? dP1_Neg : dP1_Pos; }
	double GetD1() const { return dDisplacement < 0.0 ? dD1_Neg : dD1_Pos; }
	double GetP2() const { return dDisplacement < 0.0 ? dP2_Neg : dP2_Pos; }
	double GetD2() const { return dDisplacement < 0.0 ? dD2_Neg : dD2_Pos; }
};

struct T_FHNE_TIME
{
	double dCrackTime;
	double dYieldTime;
	void Initialize()
	{
		dCrackTime=0.;
		dYieldTime=0.;
	}
};

struct T_FHNE_TIME_SET
{
	// DX,DY,DZ,RX,RY,RZ
	double dCrackTime[6];
	double dYieldTime[6];
	void Initialize()
	{
		memset(this, 0, sizeof(T_FHNE_TIME_SET));
	}
};

// Suspension Bridge
struct T_SBCO_D    // 업데이트된 좌표
{
	double dx;
	double dy;
	double dz;
	void Initialize()
	{
		dx=dy=dz=0.;
	}
};
struct T_SBCO_UNIT
{
	T_SBCO_UNIT()
	{
		dx=D_UNITSYS_BASE_LENGTH;
		dy=D_UNITSYS_BASE_LENGTH;
		dz=D_UNITSYS_BASE_LENGTH;
	}
	int dx, dy, dz;
};

struct T_SBCR_D    // 케이블의 변형전 길이비
{
	double dCableForce;   // Cable Pretension Force
	double dCableLength;  // 케이블 변형전 길이
	double dHorzForce;    // Cable Horizontal Force // MNET:XXXX 20060918 JHKIM : 정진상 부장님 요구
	double dLengthRatio;  // 변형전 길이 비
	void Initialize()
	{
		dCableForce=0.0;
		dCableLength=0.;
		dHorzForce=0.;
		dLengthRatio=0.;
	}
};
struct T_SBCR_UNIT
{
	T_SBCR_UNIT()
	{
		dCableForce=D_UNITSYS_BASE_FORCE;
		dCableLength=D_UNITSYS_BASE_LENGTH;
		dHorzForce=D_UNITSYS_BASE_FORCE;
	}
	int dCableForce;
	int dCableLength;
	int dHorzForce;
};

struct T_SBIF_D    // 부재의 평형상태 절점력 및 부재력
{
	T_ELEM_K ElemK;
	int nType;  // 1=Truss 2=Beam 3=Elastic Link 4=General Link
	double dNodeForce[2][7];
	double dElemForce[2][7];
	void Initialize()
	{
		memset(this, 0, sizeof(T_SBIF_D));
	}
};
struct T_SBIF_UNIT
{
	T_SBIF_UNIT()
	{
		nCount=7;
		nUnit[0]=D_UNITSYS_BASE_FORCE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;
		nUnit[3]=D_UNITSYS_BASE_MOMENT;
		nUnit[4]=D_UNITSYS_BASE_MOMENT;
		nUnit[5]=D_UNITSYS_BASE_MOMENT;
		nUnit[6]=D_UNITSYS_BASE_DISTSTIFF;
		for(int i=0; i<7; i++)dblForce[i]=nUnit[i];
	}
	int dblForce[7];
	int nCount;
	int nUnit[7];
};

struct T_IMPR_D    // Imperfection Load Result
{
	CString strLComName;
	double dHorizontalForcX;
	double dHorizontalForcY;
	double dAxialForc;

	void Initialize()
	{
		strLComName = "";
		dHorizontalForcX = 0.0;
		dHorizontalForcY = 0.0;;
		dAxialForc = 0.0;
	}
};
struct T_IMPR_UNIT
{
	T_IMPR_UNIT()
	{
		dHorizontalForcX = D_UNITSYS_BASE_FORCE;
		dHorizontalForcY = D_UNITSYS_BASE_FORCE;
		dAxialForc = D_UNITSYS_BASE_FORCE;
	}
	double dHorizontalForcX;
	double dHorizontalForcY;
	double dAxialForc;
};

// cable
struct T_RCBL_D
{
	double dTension[2];  //i, j
	double dForce[2][3]; //[i, j] [FX, FY, FZ]
	double dTotalLength;
	double dElongation;
	double dUnstrainedLength;
	double dSag;
	double dHDistance, dVDistance;
	double dGradient;
	double dSkewAngle[2]; //i, j
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			dTension[i] = 0.0;
			for(int j=0; j<3; j++) dForce[i][j] = 0.0;
			dSkewAngle[i]=0.0;
		}
		dTotalLength = 0.0;
		dElongation = 0.0;
		dUnstrainedLength = 0.0;
		dSag = 0.0;
		dHDistance = 0.0; dVDistance=0.0;
		dGradient=0.0;
	}
	T_RCBL_D(){Initialize();}
	T_RCBL_D(T_RCBL_D& src){ *this = src;}
	T_RCBL_D& operator=(const T_RCBL_D& src)
	{
			for(int i=0; i<2; i++)
			{
					dTension[i] = src.dTension[i];
					for(int j=0; j<3; j++) dForce[i][j] = src.dForce[i][j];
					dSkewAngle[i] = src.dSkewAngle[i];
			}
			dTotalLength      = src.dTotalLength     ;
			dElongation       = src.dElongation      ;
			dUnstrainedLength = src.dUnstrainedLength;
			dSag              = src.dSag             ;
			dHDistance        = src.dHDistance       ;
			dVDistance        = src.dVDistance       ;
			dGradient         = src.dGradient        ;
			return *this;
	}

};

struct T_RCBL_UNIT
{
	T_RCBL_UNIT()
	{
		for(int i=0; i<2; i++)
		{
			dTension[i] = D_UNITSYS_BASE_FORCE;
			for(int j=0; j<3; j++) dForce[i][j] = D_UNITSYS_BASE_FORCE;
			dSkewAngle[i] = D_UNITSYS_BASE_DEGREE;
		}
		dTotalLength = D_UNITSYS_BASE_LENGTH;
		dElongation = D_UNITSYS_BASE_LENGTH;
		dUnstrainedLength = D_UNITSYS_BASE_LENGTH;
		dSag = D_UNITSYS_BASE_LENGTH;
		dHDistance = D_UNITSYS_BASE_LENGTH;
		dVDistance = D_UNITSYS_BASE_LENGTH;
		dGradient = D_UNITSYS_NONE;
	}
	int dTension[2];  //i, j
	int dForce[2][3]; //[i, j] [FX, FY, FZ]
	int dTotalLength;
	int dElongation;
	int dUnstrainedLength;
	int dSag;
	int dHDistance, dVDistance;
	int dGradient;
	int dSkewAngle[2]; //i, j

};

struct T_RCAE_D
{
	T_ELEM_K ElemK;
	double dChordLength;
	double dEA;
	double dWeight;
	double dTension;
	double dEAMod;
	double dEfficiency;

	void Initialize()
	{
		ElemK = 0;
		dChordLength = 0.;
		dEA = 0.;
		dWeight = 0.;
		dTension = 0.;
		dEAMod = 0.;
		dEfficiency = 0.;
	}
	T_RCAE_D(){Initialize();}
	T_RCAE_D(T_RCAE_D& src){ *this = src;}
	T_RCAE_D& operator=(const T_RCAE_D& src)
	{
			ElemK        = src.ElemK;
			dChordLength = src.dChordLength;
			dEA          = src.dEA;
			dWeight      = src.dWeight;
			dTension     = src.dTension;
			dEAMod       = src.dEAMod;
			dEfficiency  = src.dEfficiency;
			return *this;
	}

};

struct T_RCAE_UNIT
{
	T_RCAE_UNIT()
	{
		dChordLength = D_UNITSYS_BASE_LENGTH;
		dEA = D_UNITSYS_BASE_FORCE;
		dWeight = D_UNITSYS_BASE_FORCE;
		dTension = D_UNITSYS_BASE_FORCE;
		dEAMod = D_UNITSYS_BASE_FORCE;
		dEfficiency = D_UNITSYS_NONE;
	}
	int dChordLength;
	int dEA;
	int dWeight;
	int dTension;
	int dEAMod;
	int dEfficiency;
};

struct T_ITTR_D
{
	double dForce[2]; //I, J단부 축력
	void Initialize()
	{
		for(int i=0; i<2; i++) dForce[i] = 0.0;
	}
};

struct T_ITTR_UNIT
{
	T_ITTR_UNIT()
	{
		dForce = D_UNITSYS_BASE_FORCE;
	}
	int dForce;
};

struct T_ITBM_D
{
	double dForce[2][7]; //I, J단부 부재력
	void Initialize()
	{
		for(int i=0; i<2; i++) 
		{
			for(int j=0; j<7; j++) dForce[i][j] = 0.0;
		}
	}
};

struct T_ITBM_UNIT
{
	T_ITBM_UNIT()
	{
		dForce[0] = D_UNITSYS_BASE_FORCE;
		dForce[1] = D_UNITSYS_BASE_FORCE;
		dForce[2] = D_UNITSYS_BASE_FORCE;
		dForce[3] = D_UNITSYS_BASE_MOMENT;
		dForce[4] = D_UNITSYS_BASE_MOMENT;
		dForce[5] = D_UNITSYS_BASE_MOMENT;
		dForce[6] = D_UNITSYS_BASE_DISTSTIFF;
	}
	int dForce[7];
};

// Tendon Weight
struct T_TENDON_WEIGHT
{
	double  dArea;
	double  dLength;
	double  dWeight;
	void Initialize()
	{
		dArea = 0.0;
		dLength = 0.0;
		dWeight = 0.0;
	}
};

struct T_LOFF_D
{
	T_ELEM_K KeyElem;
	T_STAG_K KeyStag;
	int iStep;
	T_NODE_K KeyNodeI;
	T_NODE_K KeyNodeJ;
	double dLocalVector[3];
	double dAngle;
	double dElastic;
	double dArea;
	double dDisp_I[3];
	double dDisp_J[3];
	double dDeform;
	double dPretension;
	double dLofForce;
	double dSum;

	void Initialize()
	{
		KeyElem = 0;
		KeyStag = 0;
		iStep = 0;
		KeyNodeI = 0;
		KeyNodeJ = 0;
		for(int i=0; i<3; i++) dLocalVector[i] = 0.0;
		dAngle = 0.0;
		dElastic = 0.0;
		dArea = 0.0;
		for(int i = 0; i<3; i++) dDisp_I[i]=0.0;
		for(int i = 0; i<3; i++) dDisp_J[i]=0.0;
		dDeform=0.0;
		dPretension=0.0;
		dLofForce=0.0;
		dSum=0.0;
	}
};

struct T_LOFF_UNIT
{
	T_LOFF_UNIT()
	{
		dElastic = D_UNITSYS_BASE_STRESS;
		dArea = D_UNITSYS_BASE_AREA;
		dDisp_I=D_UNITSYS_BASE_LENGTH;
		dDisp_J=D_UNITSYS_BASE_LENGTH;
		dDeform=D_UNITSYS_BASE_LENGTH;
		dPretension=D_UNITSYS_BASE_FORCE;
		dLofForce=D_UNITSYS_BASE_FORCE;
		dSum=D_UNITSYS_BASE_FORCE;
	}
	int dElastic;
	int dArea;
	int dDisp_I;
	int dDisp_J;
	int dDeform;
	int dPretension;
	int dLofForce;
	int dSum;
};

struct T_LOFB_D
{
	T_ELEM_K KeyElem;
	T_STAG_K KeyStag;
	int iStep;
	T_NODE_K KeyNodeI;
	T_NODE_K KeyNodeJ;
	double dForce_I[6];
	double dForce_J[6];
	double dBiMoment_I;
	double dBiMoment_J;
	double dDisp_I[6];
	double dDisp_J[6];
	double dRw_I;
	double dRw_J;

	void Initialize()
	{
		KeyElem = 0;
		KeyStag = 0;
		iStep = 0;
		KeyNodeI = 0;
		KeyNodeJ = 0;
		for(int i=0; i<6; i++)
		{
			dForce_I[i] = 0.0;
			dForce_J[i] = 0.0;
			dDisp_I[i] = 0.0;
			dDisp_J[i] = 0.0;
		}

		dBiMoment_I = 0.0;
		dBiMoment_J = 0.0;
		dRw_I = 0.0;
		dRw_J = 0.0;
	}
};

struct T_LOFB_UNIT
{
	T_LOFB_UNIT()
	{
		dForce[0] = D_UNITSYS_BASE_FORCE;
		dForce[1] = D_UNITSYS_BASE_FORCE;
		dForce[2] = D_UNITSYS_BASE_FORCE;
		dForce[3] = D_UNITSYS_BASE_MOMENT;
		dForce[4] = D_UNITSYS_BASE_MOMENT;
		dForce[5] = D_UNITSYS_BASE_MOMENT;
		dBiMoment = D_UNITSYS_BASE_DISTSTIFF;
		dDisp[0] = D_UNITSYS_BASE_LENGTH;
		dDisp[1] = D_UNITSYS_BASE_LENGTH;
		dDisp[2] = D_UNITSYS_BASE_LENGTH;
		dDisp[3] = D_UNITSYS_BASE_RADIAN;
		dDisp[4] = D_UNITSYS_BASE_RADIAN;
		dDisp[5] = D_UNITSYS_BASE_RADIAN;
		dRw = D_UNITSYS_BASE_RADIAN_L;
	}
	int dForce[6];
	int dBiMoment;
	int dDisp[6];
	int dRw;
};

struct T_LOFP_D
{
	T_ELEM_K KeyElem;
	T_NODE_K KeyNode;
	double dForce[6];
	double dDisp[6];

	void Initialize()
	{
		KeyElem = 0;
		KeyNode = 0;

		for(int i=0; i<6; i++)
		{
			dForce[i] = 0.0;
			dDisp[i] = 0.0;
		}
	}
};

struct T_LOFP_UNIT
{
	T_LOFP_UNIT()
	{
		dForce[0] = D_UNITSYS_BASE_FORCE;
		dForce[1] = D_UNITSYS_BASE_FORCE;
		dForce[2] = D_UNITSYS_BASE_FORCE;
		dForce[3] = D_UNITSYS_BASE_MOMENT;
		dForce[4] = D_UNITSYS_BASE_MOMENT;
		dForce[5] = D_UNITSYS_BASE_MOMENT;
		dDisp[0] = D_UNITSYS_BASE_LENGTH;
		dDisp[1] = D_UNITSYS_BASE_LENGTH;
		dDisp[2] = D_UNITSYS_BASE_LENGTH;
		dDisp[3] = D_UNITSYS_BASE_RADIAN;
		dDisp[4] = D_UNITSYS_BASE_RADIAN;
		dDisp[5] = D_UNITSYS_BASE_RADIAN;
	}
	int dForce[6];
	int dDisp[6];
};

struct T_BTPF_D
{
	double dForce[4][BTPF_SIZE];

	void Initialize()
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<BTPF_SIZE; j++) dForce[i][j] = 0.0;
		}
	}
};

struct T_BTPF_UNIT
{
	T_BTPF_UNIT()
	{
		dForce[0] = D_UNITSYS_BASE_FORCE;
		dForce[1] = D_UNITSYS_BASE_FORCE;
		dForce[2] = D_UNITSYS_BASE_FORCE;
		dForce[3] = D_UNITSYS_BASE_MOMENT;
		dForce[4] = D_UNITSYS_BASE_MOMENT;
		dForce[5] = D_UNITSYS_BASE_MOMENT;
		dForce[6] = D_UNITSYS_BASE_FORCE;
		dForce[7] = D_UNITSYS_BASE_FORCE;
		dForce[8] = D_UNITSYS_BASE_FORCE;
		dForce[9] = D_UNITSYS_BASE_MOMENT;
		dForce[10] = D_UNITSYS_BASE_MOMENT;
		dForce[11] = D_UNITSYS_BASE_MOMENT;
		dForce[12] = D_UNITSYS_BASE_DISTSTIFF;
		dForce[13] = D_UNITSYS_BASE_DISTSTIFF;
		dForce[14] = D_UNITSYS_BASE_MOMENT;
		dForce[15] = D_UNITSYS_BASE_MOMENT;
		dForce[16] = D_UNITSYS_BASE_MOMENT;
		dForce[17] = D_UNITSYS_BASE_MOMENT;
	}
	int dForce[18];
};

// Time History Overturning Moment
struct T_SOTF_D
{
	double dMax_X;
	double dMin_X;
	double MaxTime_X;
	double MinTime_X;
	double dMax_Y;
	double dMin_Y;
	double MaxTime_Y;
	double MinTime_Y;
	void Initialize()
	{
		memset(this, 0, sizeof(T_SOTF_D));
		//dMax_X=0.;
		//dMin_X=0.;
		//MaxTime_X=0.;
		//MinTime_X=0.;
		//dMax_Y=0.;
		//dMin_Y=0.;
		//MaxTime_Y=0.;
		//MinTime_Y=0.;
	}
};

//층별 변위, 속도, 가속도의 최대/최소값
struct T_SVMX_D
{
	double dblMax[6];
	double dblMin[6];
	double dblTime[12];
	void Initialize()
	{
		memset(this, 0, sizeof(T_SVMX_D));
	}
};

struct T_SVMX_UNIT
{
	T_SVMX_UNIT()
	{
		dblMax[0]=D_UNITSYS_BASE_LENGTH;    //X-변위
		dblMax[1]=D_UNITSYS_BASE_VELOCITY;  //X-속도
		dblMax[2]=D_UNITSYS_BASE_GRAVITY;   //X-가속도
		dblMax[3]=D_UNITSYS_BASE_LENGTH;    //Y-변위
		dblMax[4]=D_UNITSYS_BASE_VELOCITY;  //Y-속도
		dblMax[5]=D_UNITSYS_BASE_GRAVITY;   //Y-가속도
		dblMin[0]=D_UNITSYS_BASE_LENGTH;    //X-변위
		dblMin[1]=D_UNITSYS_BASE_VELOCITY;  //X-속도
		dblMin[2]=D_UNITSYS_BASE_GRAVITY;   //X-가속도
		dblMin[3]=D_UNITSYS_BASE_LENGTH;    //Y-변위
		dblMin[4]=D_UNITSYS_BASE_VELOCITY;  //Y-속도
		dblMin[5]=D_UNITSYS_BASE_GRAVITY;   //Y-가속도
		for(int i=0; i<12; i++)dblTime[i]=D_UNITSYS_NONE; //1~12성분 발생시간
	}
	int dblMax[6];
	int dblMin[6];
	int dblTime[12];
};

//층간 변위, 속도, 가속도, 층간변형각의 최대/최소값
struct T_DVMX_BN
{
	double dblMax[8];
	double dblMin[8];
	double dblTime[16];
	double nNodeDriftMaxX; // 층간변위 절점 (Max X)
	double nNodeDriftMaxY; // 층간변위 절점 (Max Y)
	double nNodeDriftMinX; // 층간변위 절점 (Min X)
	double nNodeDriftMinY; // 층간변위 절점 (Min Y)

	void Initialize()
	{
		memset(this, 0, sizeof(T_DVMX_BN));
	}
};

//층간 변위, 속도, 가속도, 층간변형각의 최대/최소값
struct T_DVMX_D
{
	double dblMax[8];
	double dblMin[8];
	double dblTime[16];
	int nNodeDriftMaxX; // 층간변위 절점 (Max X)
	int nNodeDriftMaxY; // 층간변위 절점 (Max Y)
	int nNodeDriftMinX; // 층간변위 절점 (Min X)
	int nNodeDriftMinY; // 층간변위 절점 (Min Y)

	void Initialize()
	{
		memset(this, 0, sizeof(T_DVMX_D));
	}
};

struct T_DVMX_UNIT
{
	T_DVMX_UNIT()
	{
		dblMax[0]=D_UNITSYS_BASE_LENGTH;    //X-변위
		dblMax[1]=D_UNITSYS_BASE_VELOCITY;  //X-속도
		dblMax[2]=D_UNITSYS_BASE_GRAVITY;   //X-가속도
		dblMax[3]=D_UNITSYS_BASE_LENGTH;    //Y-변위
		dblMax[4]=D_UNITSYS_BASE_VELOCITY;  //Y-속도
		dblMax[5]=D_UNITSYS_BASE_GRAVITY;   //Y-가속도
		dblMax[6]=D_UNITSYS_NONE;           //X-층간변형각
		dblMax[7]=D_UNITSYS_NONE;           //Y-층간변형각

		dblMin[0]=D_UNITSYS_BASE_LENGTH;    //X-변위
		dblMin[1]=D_UNITSYS_BASE_VELOCITY;  //X-속도
		dblMin[2]=D_UNITSYS_BASE_GRAVITY;   //X-가속도
		dblMin[3]=D_UNITSYS_BASE_LENGTH;    //Y-변위
		dblMin[4]=D_UNITSYS_BASE_VELOCITY;  //Y-속도
		dblMin[5]=D_UNITSYS_BASE_GRAVITY;   //Y-가속도
		dblMin[6]=D_UNITSYS_NONE;           //X-층간변형각
		dblMin[7]=D_UNITSYS_NONE;           //Y-층간변형각

		for(int i=0; i<16; i++)dblTime[i]=D_UNITSYS_NONE;  //1~16성분 발생시간
	}
	int dblMax[8];
	int dblMin[8];
	int dblTime[16];
};

//층간 변위, 속도, 가속도, 층간변형각의 최대/최소값.  for dummy story.
struct T_DVMD_BN
{
	double dblMax[8];
	double dblMin[8];
	double dblTime[16];
	double nNodeDriftMaxX; // 층간변위 절점 (Max X)
	double nNodeDriftMaxY; // 층간변위 절점 (Max Y)
	double nNodeDriftMinX; // 층간변위 절점 (Min X)
	double nNodeDriftMinY; // 층간변위 절점 (Min Y)
	double nStor_cur; // 현재층 serial 번호
	double nStor_up;  // 바로 위층 serial 번호

	void Initialize()
	{
		memset(this, 0, sizeof(T_DVMD_BN));
	}
};

//층간 변위, 속도, 가속도, 층간변형각의 최대/최소값
struct T_SDVA_D
{
	double dblDVA[6]; // X 방향 변위/속도/가속도 + Y방향 변위/속도/가속도 
	void Initialize()
	{
		memset(this, 0, sizeof(T_SDVA_D));
	}
};

struct T_SDVA_UNIT
{
	T_SDVA_UNIT()
	{
		dblDVA[0]=D_UNITSYS_BASE_LENGTH;    //X-변위
		dblDVA[1]=D_UNITSYS_BASE_VELOCITY;  //X-속도
		dblDVA[2]=D_UNITSYS_BASE_GRAVITY;   //X-가속도
		dblDVA[3]=D_UNITSYS_BASE_LENGTH;    //Y-변위
		dblDVA[4]=D_UNITSYS_BASE_VELOCITY;  //Y-속도
		dblDVA[5]=D_UNITSYS_BASE_GRAVITY;   //Y-가속도
	}
	int dblDVA[6];
};

// KYE-HONG-20060321
// Nodal Acceleration for RS
struct _NDAC_BN
{
	double dblAccl[6]; 
	void Initialize()
	{
		memset(this, 0, sizeof(_NDAC_BN));
	}
};

struct T_NDAC_D
{
	double dblAccl[7]; 
	void Initialize()
	{
		memset(this, 0, sizeof(T_NDAC_D));
	}
	void ConvertToNdac(_NDAC_BN& rData)
	{
		Initialize();
		for(int i=0; i<6; i++)
		{
			dblAccl[i] = rData.dblAccl[i];
		}
	}
};

struct T_NDAC_UNIT
{
	T_NDAC_UNIT()
	{
		dblAccl[0]=D_UNITSYS_BASE_ACCELERATION;   
		dblAccl[1]=D_UNITSYS_BASE_ACCELERATION;  
		dblAccl[2]=D_UNITSYS_BASE_ACCELERATION;  
		dblAccl[3]=D_UNITSYS_BASE_RADIAN_SEC2;    
		dblAccl[4]=D_UNITSYS_BASE_RADIAN_SEC2;  
		dblAccl[5]=D_UNITSYS_BASE_RADIAN_SEC2;  
		dblAccl[6]=D_UNITSYS_BASE_RADIAN_L_SEC2;
	}
	int dblAccl[7];
};

// KYE-HONG-20060321
// Nodal Inertia Force for RS
struct _NDIF_BN
{
	double dblForce[6]; 
	void Initialize()
	{
		memset(this, 0, sizeof(_NDIF_BN));
	}
};

struct T_NDIF_D
{
	double dblForce[7]; 
	void Initialize()
	{
		memset(this, 0, sizeof(T_NDIF_D));
	}
	void ConvertToNdif(_NDIF_BN& rData)
	{
		Initialize();
		for(int i=0; i<6; i++)
		{
			dblForce[i] = rData.dblForce[i];
		}
	}
};

struct T_NDIF_UNIT
{
	T_NDIF_UNIT()
	{
		dblForce[0]=D_UNITSYS_BASE_FORCE;   
		dblForce[1]=D_UNITSYS_BASE_FORCE;  
		dblForce[2]=D_UNITSYS_BASE_FORCE;  
		dblForce[3]=D_UNITSYS_BASE_MOMENT;    
		dblForce[4]=D_UNITSYS_BASE_MOMENT;  
		dblForce[5]=D_UNITSYS_BASE_MOMENT;  
		dblForce[6]=D_UNITSYS_BASE_DISTSTIFF;
	}
	int dblForce[7];
};

// KYE-HONG-20060510
// Tendon Approximate Loss
struct T_APTL_D
{
	double dblForce[5][8]; // 5지점 : [0] Immediate Loss, [1] Creep Loss, [2] Shrink. Loss, [3] Relax. Loss
												 //         [4] All Loss  [5] Stress(immediate loss) [6] Stress(all loss)
												 //         [7] Stress(All loss)/Stress(Immediate Loss)
	void Initialize()
	{
		memset(dblForce,0,sizeof(dblForce)); 
	}
};

// KYUNG-HA 20070112'
struct T_MACC_D
{
	double dblFactor[5]; // 2Pi/주기, Fdx, Fdy, Fdz Factor, RS모드별 계수
	void Initialize()
	{
		memset(this, 0, sizeof(T_MACC_D));
	}
};

struct T_AVGN_D
{
	// P1, P2, P3, Tresca, Vonmis
	double dAvgNodalVal[5];

	void Initialize()
	{
		memset(this, 0, sizeof(T_AVGN_D));
	}
};
struct T_AVGN_UNIT
{
	T_AVGN_UNIT()
	{
		dAvgNodalVal[0]=D_UNITSYS_BASE_STRESS;
		dAvgNodalVal[1]=D_UNITSYS_BASE_STRESS;
		dAvgNodalVal[2]=D_UNITSYS_BASE_STRESS;
		dAvgNodalVal[3]=D_UNITSYS_BASE_STRESS;
		dAvgNodalVal[4]=D_UNITSYS_BASE_STRESS;
	}
	int dAvgNodalVal[5];
};

// 충격계수 자동산출 결과 (Truss)
struct T_AIFT_D
{
	double dblEffLength[2];      // max, min
	double dblMinEffLength;
	double dblMaxImpactFactor;
};

// 충격계수 자동산출 결과 (Beam)
struct T_AIFB_D
{
	double dblEffLength[5][6];   // 5: i, 1/4, 1/2, 3/4, j
															 // 6: My_max, My_min, Mz_max
															 //    Mz_min, Fx_max, Fx_min
	double dblMinEffLength;
	double dblMaxImpactFactor;
};

// 충격계수 자동산출 결과 (Beam) - 7자유도
struct T_AIFW_D
{
	double dblEffLength[5][6];   // 5: i, 1/4, 1/2, 3/4, j
	// 6: My_max, My_min, Mz_max
	//    Mz_min, Fx_max, Fx_min
	double dblMinEffLength;
	double dblMaxImpactFactor;
};

// 충격계수 자동산출 결과 (Plate)
struct T_AIFP_D
{
	double dblEffLength[5][8];   // 5: cent, i, j, k, l
															 // 8: Mxx_max, Mxx_min, Myy_max, Myy_min,
															 //    Fxx_max, Fxx_min, Fyy_max, Fyy_min
	double dblMinEffLength;
	double dblMaxImpactFactor;
};

// [2010-06-04] Kim, Geun Young (Tel: 2042, gykim@midasit.com) - PMS:4092
struct T_CRLT_D
{
	double dRatioXY[2]; // 0:X방향, 1:Y방향
	int    nCrack[5];   // Node:1,2,3,4,Center순서, Crack=1, No Crack=0
	void Initialize()
	{
		memset(this, 0, sizeof(T_CRLT_D));   
	}
};
struct T_CRLT_UNIT
{
	T_CRLT_UNIT()
	{
		nCount=7;
		nUnit[0]=D_UNITSYS_NONE;  
		nUnit[1]=D_UNITSYS_NONE;  
		nUnit[2]=D_UNITSYS_NONE;  
		nUnit[3]=D_UNITSYS_NONE;  
		nUnit[4]=D_UNITSYS_NONE;  
		nUnit[5]=D_UNITSYS_NONE;  
		nUnit[6]=D_UNITSYS_NONE;      
		for(int i=0; i<2; i++)dRatioXY[i]=nUnit[i];
		for(int j=0; j<5; j++)nCrack[j]=nUnit[j];
	}
	int dRatioXY[2];
	int nCrack[5];
	int nCount;
	int nUnit[7];
};

// Composite Design 결과 테이블 Force/Moment 
struct T_CPFM_D
{
	double dDeadB[3];     // My, Mz, Fz
	double dDeadA[3];     // My, Mz, Fz
	double dShortTerm[3]; // My, Mz, Fz

	void Initialize()
	{
		for(int i=0; i<3; ++i)
		{
			dDeadB[i] = 0.0;
			dDeadA[i] = 0.0;
			dShortTerm[i] = 0.0;
		}
	}
};

struct T_CPFM_UNIT
{
	T_CPFM_UNIT()
	{			
		for(int i=0; i<3; ++i)
		{
			if(i==0)
			{
				dDeadB[i] = D_UNITSYS_BASE_FORCE;
				dDeadA[i] = D_UNITSYS_BASE_FORCE;
				dShortTerm[i] = D_UNITSYS_BASE_FORCE;
			}
			else
			{
				dDeadB[i] = D_UNITSYS_BASE_MOMENT;
				dDeadA[i] = D_UNITSYS_BASE_MOMENT;
				dShortTerm[i] = D_UNITSYS_BASE_MOMENT;
			}
		}
	}

	int dDeadB[3];
	int dDeadA[3];
	int dShortTerm[3];
};

struct T_WAMT_D
{
	// Center, Node1, Node2, Node3, Node4
	// [4] : Top Dir1, Top Dir2, Bot Dir1, Bot Dir2
	// [4] : Mxx. Myy. Mxy, Wood Armer Moment
	double dWAMoment[5][4][4];

	void Initialize()
	{
		memset(this, 0, sizeof(T_WAMT_D));
	}

	void Max(T_WAMT_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				if(Data.dWAMoment[i][j][3] > dWAMoment[i][j][3])
				for(int k=0; k<4; k++) 
				{
						dWAMoment[i][j][k] = Data.dWAMoment[i][j][k];
				}
			}
		}
	}
	void Min(T_WAMT_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				if(Data.dWAMoment[i][j][3] < dWAMoment[i][j][3])
					for(int k=0; k<4; k++) 
					{
						dWAMoment[i][j][k] = Data.dWAMoment[i][j][k];
					}
			}
		}
	}
	void AbsMax(T_WAMT_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				for(int k=0; k<4; k++)
				{
					if(fabs(dWAMoment[i][j][k]) >= fabs(Data.dWAMoment[i][j][k]))
					{
						dWAMoment[i][j][k] = fabs(dWAMoment[i][j][k]);
					}
					else
					{
						dWAMoment[i][j][k] = fabs(Data.dWAMoment[i][j][k]);
					}		
				}						
			}
		}
	}
	void AbsMaxByWamt(T_WAMT_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				for(int k=0; k<4; k++)
				{
					if(fabs(dWAMoment[i][j][3]) >= fabs(Data.dWAMoment[i][j][3]))
					{
						dWAMoment[i][j][k] = fabs(dWAMoment[i][j][k]);
					}
					else
					{
						dWAMoment[i][j][k] = fabs(Data.dWAMoment[i][j][k]);
					}		
				}						
			}
		}
	}
	void AbsMax(T_WAMT_D& Data1, T_WAMT_D& Data2)
	{
		Initialize();
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				for(int k=0; k<4; k++)
				{          
					if(fabs(Data1.dWAMoment[i][j][k]) >= fabs(Data2.dWAMoment[i][j][k]))
					{
						dWAMoment[i][j][k] = fabs(Data1.dWAMoment[i][j][k]);
					}
					else
					{
						dWAMoment[i][j][k] = fabs(Data2.dWAMoment[i][j][k]);
					}				
				}
			}
		}
	}
	void AbsMaxByWamt(T_WAMT_D& Data1, T_WAMT_D& Data2)
	{
		Initialize();
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				for(int k=0; k<4; k++)
				{          
					if(fabs(Data1.dWAMoment[i][j][3]) >= fabs(Data2.dWAMoment[i][j][3]))
					{
						dWAMoment[i][j][k] = fabs(Data1.dWAMoment[i][j][k]);
					}
					else
					{
						dWAMoment[i][j][k] = fabs(Data2.dWAMoment[i][j][k]);
					}				
				}
			}
		}
	}
	void AbsMaxwithSign(T_WAMT_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				for(int k=0; k<4; k++)
				{
					if(fabs(dWAMoment[i][j][k]) >= fabs(Data.dWAMoment[i][j][k]))
					{
						dWAMoment[i][j][k] = dWAMoment[i][j][k];
					}
					else
					{
						dWAMoment[i][j][k] = Data.dWAMoment[i][j][k];
					}				
				}				
			}
		}
	}
	void AbsMaxwithSignByWamt(T_WAMT_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				for(int k=0; k<4; k++)
				{
					if(fabs(dWAMoment[i][j][3]) >= fabs(Data.dWAMoment[i][j][3]))
					{
						dWAMoment[i][j][k] = dWAMoment[i][j][k];
					}
					else
					{
						dWAMoment[i][j][k] = Data.dWAMoment[i][j][k];
					}				
				}				
			}
		}
	}
	void AbsMaxwithSign(T_WAMT_D& Data1, T_WAMT_D& Data2)
	{
		Initialize();
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				for(int k=0; k<4; k++)
				{
					if(fabs(Data1.dWAMoment[i][j][k]) >= fabs(Data2.dWAMoment[i][j][k]))
					{
						dWAMoment[i][j][k] = Data1.dWAMoment[i][j][k];
					}
					else
					{
						dWAMoment[i][j][k] = Data2.dWAMoment[i][j][k];
					}				
				}				
			}
		}
	}
	void AbsMaxwithSignByWamt(T_WAMT_D& Data1, T_WAMT_D& Data2)
	{
		Initialize();
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				for(int k=0; k<4; k++)
				{
					if(fabs(Data1.dWAMoment[i][j][3]) >= fabs(Data2.dWAMoment[i][j][3]))
					{
						dWAMoment[i][j][k] = Data1.dWAMoment[i][j][k];
					}
					else
					{
						dWAMoment[i][j][k] = Data2.dWAMoment[i][j][k];
					}				
				}				
			}
		}
	}
	void Add(T_WAMT_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				// Wood Armer Moment 값은 최종 Add 된 mxx,myy,mxy 로 재계산 해주어야 한다.
				for(int k=0; k<4; k++) dWAMoment[i][j][k] += Data.dWAMoment[i][j][k];
			}
		}
	}
	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				// Wood Armer Moment 값은 최종 abs 된 mxx,myy,mxy 로 재계산 해주어야 한다.
				for(int k=0; k<4; k++) dWAMoment[i][j][k] = fabs(dWAMoment[i][j][k]);
			}
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				// Wood Armer Moment 값은 최종 sqr 된 mxx,myy,mxy 로 재계산 해주어야 한다.
				for(int k=0; k<4; k++) dWAMoment[i][j][k] = dWAMoment[i][j][k] * dWAMoment[i][j][k];
			}
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				// Wood Armer Moment 값은 최종 sqr 된 mxx,myy,mxy 로 재계산 해주어야 한다.
				for(int k=0; k<4; k++) dWAMoment[i][j][k] = sqrt(dWAMoment[i][j][k]);
			}
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.0) return;

		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				// Wood Armer Moment 값은 mxx,myy,mxy 에 Factor 를 곱한값으로 재계산 해주어야 한다.
				for(int k=0; k<4; k++) dWAMoment[i][j][k] *= dFactor;
			}
		}
	}
	void Multiple(T_WAMT_D Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)
			{
				// Wood Armer Moment 값은 최종 mxx,myy,mxy 로 재계산 해주어야 한다.
				for(int k=0; k<4; k++) dWAMoment[i][j][k] *= dWAMoment[i][j][k];
			}
		}
	}	
};

struct T_WAMT_UNIT
{
	T_WAMT_UNIT()
	{
		nUnit = D_UNITSYS_BASE_UNITMOMENT;
	}
	int nUnit;
};

struct T_CMFP_D
{
	// [5] : Center, Node1, Node2, Node3, Node4
	// [3] : Mxx, Myy, Mxy
	// [3] : Mxx, Myy, Mxy

	double dStress[5][3][3]; 

	void Initialize()
	{
		memset(this, 0, sizeof(T_CMFP_D));
	}
};

// Seismic Control Devices Result - Viscous/Oil Damper
struct T_SDVI_RT_D
{
	float dForceSprDsh   ;
	float dDeformSprDsh  ;
	float dForceSpr      ;
	float dForceDsh      ;
	float dDeformSpr     ;
	float dVelocityDsh   ;
	float dEnergySprDsh  ;
	float dEnergySpr     ;
	float dEnergyDsh     ;

	void Initialize()
	{
		dForceSprDsh  = 0.;
		dDeformSprDsh = 0.;
		dForceSpr     = 0.;
		dForceDsh     = 0.;
		dDeformSpr    = 0.;
		dVelocityDsh  = 0.;
		dEnergySprDsh = 0.;
		dEnergySpr    = 0.;
		dEnergyDsh    = 0.;
	}
};
struct T_SDVI_RT_UNIT
{
	T_SDVI_RT_UNIT()
	{
		dForceSprDsh  = D_UNITSYS_BASE_FORCE;
		dDeformSprDsh = D_UNITSYS_BASE_LENGTH;
		dForceSpr     = D_UNITSYS_BASE_FORCE;
		dForceDsh     = D_UNITSYS_BASE_FORCE;
		dDeformSpr    = D_UNITSYS_BASE_LENGTH;
		dVelocityDsh  = D_UNITSYS_BASE_VELOCITY;
		dEnergySprDsh = D_UNITSYS_BASE_MOMENT;
		dEnergySpr    = D_UNITSYS_BASE_MOMENT;
		dEnergyDsh    = D_UNITSYS_BASE_MOMENT;
	}
	int dForceSprDsh ;
	int dDeformSprDsh;
	int dForceSpr    ;
	int dForceDsh    ;
	int dDeformSpr   ;
	int dVelocityDsh ;
	int dEnergySprDsh;
	int dEnergySpr   ;
	int dEnergyDsh   ;
};

// Seismic Control Devices Result - Viscoelastic Damper
struct T_SDVE_RT_D_856
{
	float dForceTotal      ;
	float dDeformTotal     ;
	float dForceVoigtSpr   ;
	float dForceMaxwellSpr ;
	float dForceVoigtDsh   ;
	float dForceMaxwellDsh ;
	float dVelocityTotal   ;
	float dEnergyTotal     ;
	float dEnergyVoigtSpr  ;
	float dEnergyMaxwellSpr;
	float dEnergyVoigtDsh  ;
	float dEnergyMaxwellDsh;
};
struct T_SDVE_RT_D_865
{
	float dForceTotal      ;
	float dDeformTotal     ;
	float dForceVoigtSpr   ;
	float dForceMaxwellSpr ;
	float dForceVoigtDsh   ;
	float dForceMaxwellDsh ;
	float dVelocityTotal   ;
	float dEnergyTotal     ;
	float dEnergyVoigtSpr  ;
	float dEnergyMaxwellSpr;
	float dEnergyVoigtDsh  ;
	float dEnergyMaxwellDsh;
	float dForceMount;
	float dDeformMount;
	float dEnergyMount;
};
struct T_SDVE_RT_D
{
	float dForceTotal      ;
	float dDeformTotal     ;
	float dForceVoigtSpr   ;
	float dForceMaxwellSpr ;
	float dForceVoigtDsh   ;
	float dForceMaxwellDsh ;
	float dVelocityTotal   ;
	float dEnergyTotal     ;
	float dEnergyVoigtSpr  ;
	float dEnergyMaxwellSpr;
	float dEnergyVoigtDsh  ;
	float dEnergyMaxwellDsh;
	float dForceMount;
	float dDeformMount;
	float dEnergyMount;
	float dVeloMaxwellDsh  ;
	float dDeformMaxwellSpr;

	void Initialize()
	{
		dForceTotal       = 0.0;
		dDeformTotal      = 0.0;
		dForceVoigtSpr    = 0.0;
		dForceMaxwellSpr  = 0.0;
		dForceVoigtDsh    = 0.0;
		dForceMaxwellDsh  = 0.0;
		dVelocityTotal    = 0.0;
		dEnergyTotal      = 0.0;
		dEnergyVoigtSpr   = 0.0;
		dEnergyMaxwellSpr = 0.0;
		dEnergyVoigtDsh   = 0.0;
		dEnergyMaxwellDsh = 0.0;
		dForceMount       = 0.0;
		dDeformMount      = 0.0;
		dEnergyMount      = 0.0;
		dVeloMaxwellDsh   = 0.0;
		dDeformMaxwellSpr = 0.0;
	}
	void Convert865(T_SDVE_RT_D_865& Data)
	{
		dForceTotal       = Data.dForceTotal       ;
		dDeformTotal      = Data.dDeformTotal      ;
		dForceVoigtSpr    = Data.dForceVoigtSpr    ;
		dForceMaxwellSpr  = Data.dForceMaxwellSpr  ;
		dForceVoigtDsh    = Data.dForceVoigtDsh    ;
		dForceMaxwellDsh  = Data.dForceMaxwellDsh  ;
		dVelocityTotal    = Data.dVelocityTotal    ;
		dEnergyTotal      = Data.dEnergyTotal      ;
		dEnergyVoigtSpr   = Data.dEnergyVoigtSpr   ;
		dEnergyMaxwellSpr = Data.dEnergyMaxwellSpr ;
		dEnergyVoigtDsh   = Data.dEnergyVoigtDsh   ;
		dEnergyMaxwellDsh = Data.dEnergyMaxwellDsh ;
		dForceMount       = Data.dForceMount       ;
		dDeformMount      = Data.dDeformMount      ;
		dEnergyMount      = Data.dEnergyMount      ;
		dVeloMaxwellDsh   = 0.0;
		dDeformMaxwellSpr = 0.0;
	}
	void Convert860(T_SDVE_RT_D_856& Data)
	{
		dForceTotal       = Data.dForceTotal       ;
		dDeformTotal      = Data.dDeformTotal      ;
		dForceVoigtSpr    = Data.dForceVoigtSpr    ;
		dForceMaxwellSpr  = Data.dForceMaxwellSpr  ;
		dForceVoigtDsh    = Data.dForceVoigtDsh    ;
		dForceMaxwellDsh  = Data.dForceMaxwellDsh  ;
		dVelocityTotal    = Data.dVelocityTotal    ;
		dEnergyTotal      = Data.dEnergyTotal      ;
		dEnergyVoigtSpr   = Data.dEnergyVoigtSpr   ;
		dEnergyMaxwellSpr = Data.dEnergyMaxwellSpr ;
		dEnergyVoigtDsh   = Data.dEnergyVoigtDsh   ;
		dEnergyMaxwellDsh = Data.dEnergyMaxwellDsh ;
		dForceMount       = 0.0;
		dDeformMount      = 0.0;
		dEnergyMount      = 0.0;
		dVeloMaxwellDsh   = 0.0;
		dDeformMaxwellSpr = 0.0;
	}
};
struct T_SDVE_RT_UNIT
{
	T_SDVE_RT_UNIT()
	{
		dForceTotal       = D_UNITSYS_BASE_FORCE;
		dDeformTotal      = D_UNITSYS_BASE_LENGTH;
		dForceVoigtSpr    = D_UNITSYS_BASE_FORCE;
		dForceMaxwellSpr  = D_UNITSYS_BASE_FORCE;
		dForceVoigtDsh    = D_UNITSYS_BASE_FORCE;
		dForceMaxwellDsh  = D_UNITSYS_BASE_FORCE;
		dVelocityTotal    = D_UNITSYS_BASE_VELOCITY;
		dEnergyTotal      = D_UNITSYS_BASE_MOMENT;
		dEnergyVoigtSpr   = D_UNITSYS_BASE_MOMENT;
		dEnergyMaxwellSpr = D_UNITSYS_BASE_MOMENT;
		dEnergyVoigtDsh   = D_UNITSYS_BASE_MOMENT;
		dEnergyMaxwellDsh = D_UNITSYS_BASE_MOMENT;
		dForceMount       = D_UNITSYS_BASE_FORCE;
		dDeformMount      = D_UNITSYS_BASE_LENGTH;
		dEnergyMount      = D_UNITSYS_BASE_MOMENT;
		dVeloMaxwellDsh   = D_UNITSYS_BASE_VELOCITY;
		dDeformMaxwellSpr = D_UNITSYS_BASE_LENGTH;
	}
	int dForceTotal      ;
	int dDeformTotal     ;
	int dForceVoigtSpr   ;
	int dForceMaxwellSpr ;
	int dForceVoigtDsh   ;
	int dForceMaxwellDsh ;
	int dVelocityTotal   ;
	int dEnergyTotal     ;
	int dEnergyVoigtSpr  ;
	int dEnergyMaxwellSpr;
	int dEnergyVoigtDsh  ;
	int dEnergyMaxwellDsh;
	int dForceMount      ;
	int dDeformMount     ;
	int dEnergyMount     ;
	int dVeloMaxwellDsh  ;
	int dDeformMaxwellSpr;
};

// Seismic Control Devices Result - Steel Damper
struct T_SDST_RT_D
{
	float dForce       ;
	float dDeform      ;
	float dDuctFact    ;
	float dCumulativePD;
	float dEnergy      ;

	void Initialize()
	{
		dForce    = 0.;
		dDeform   = 0.;
		dDuctFact = 0.;
		dCumulativePD=0.;
		dEnergy   = 0.;
	}
};
struct T_SDST_RT_UNIT
{
	T_SDST_RT_UNIT()
	{
		dForce        = D_UNITSYS_BASE_FORCE;
		dDeform       = D_UNITSYS_BASE_LENGTH;
		dDuctFact     = D_UNITSYS_NONE;
		dCumulativePD = D_UNITSYS_NONE;
		dEnergy       = D_UNITSYS_BASE_MOMENT;
	}
	int dForce       ;
	int dDeform      ;
	int dDuctFact    ;
	int dCumulativePD;
	int dEnergy      ;
};

// Seismic Control Devices Result - Viscous/Oil Damper
struct T_SDHY_RT_D
{
	float dForce;
	float dDeform;
	float dDuctFact;
	float dEnergy;

	void Initialize()
	{
		dForce    = 0.;
		dDeform   = 0.;
		dDuctFact = 0.;
		dEnergy   = 0.;
	}
};
struct T_SDHY_RT_UNIT
{
	T_SDHY_RT_UNIT()
	{
		dForce        = D_UNITSYS_BASE_FORCE;
		dDeform       = D_UNITSYS_BASE_LENGTH;
		dDuctFact     = D_UNITSYS_NONE;
		dEnergy       = D_UNITSYS_BASE_MOMENT;
	}
	int dForce       ;
	int dDeform      ;
	int dDuctFact    ;
	int dEnergy      ;
};

// Seismic Control Devices Result - Viscous/Oil Damper
struct T_SDIS_RT_D
{
	float dForce   ;
	float dDeform  ;
	float dDuctFact;
	float dEnergy  ;

	void Initialize()
	{
		dForce    = 0.;
		dDeform   = 0.;
		dDuctFact = 0.;
		dEnergy   = 0.;
	}
};
struct T_SDIS_RT_UNIT
{
	T_SDIS_RT_UNIT()
	{
		dForce        = D_UNITSYS_BASE_FORCE;
		dDeform       = D_UNITSYS_BASE_LENGTH;
		dDuctFact     = D_UNITSYS_NONE;
		dEnergy       = D_UNITSYS_BASE_MOMENT;
	}
	int dForce       ;
	int dDeform      ;
	int dDuctFact    ;
	int dEnergy      ;
};

// Seismic Control Devices Result - MSS Status Result
struct T_MSSP_RT_D
{
	float dForceSprDsh;
	float dDeformSprDsh;

	void Initialize()
	{
		dForceSprDsh  = 0.;
		dDeformSprDsh = 0.;
	}
};
struct T_MSSP_RT_UNIT
{
	T_MSSP_RT_UNIT()
	{
		dForceSprDsh        = D_UNITSYS_BASE_FORCE;
		dDeformSprDsh       = D_UNITSYS_BASE_LENGTH;
	}
	int dForceSprDsh       ;
	int dDeformSprDsh      ;
};

// Seismic Control Devices Result - Ioslation:GB
struct T_SDGB_RT_D
{
	float dForce;
	float dDeform;
	float dEnergy;

	void Initialize()
	{
		dForce = 0.;
		dDeform = 0.;
		dEnergy = 0.;
	}
};
struct T_SDGB_RT_UNIT
{
	T_SDGB_RT_UNIT()
	{
		dForce = D_UNITSYS_BASE_FORCE;
		dDeform = D_UNITSYS_BASE_LENGTH;
		dEnergy = D_UNITSYS_BASE_MOMENT;
	}
	int dForce;
	int dDeform;
	int dEnergy;
};

// Time History Energy Result
struct T_ENRG_RT_D
{
	float dEi;   /* Input Energy                     */
	float dEk;   /* Kinetic Energy                   */
	float dEs;   /* Strain Energy                    */ 
	float dEd;   /* Damping Energy                   */
	float dEm;   /* Maxwell Damper Energy            */
	float dEv;   /* Velocity Dependent Device Energy */
	float dEt;   /* Strain Dependent Device Energy   */
	float dEo;   /* Isolator Device Energy           */
	float dEh;   /* Dissipated Inelastic Energy      */
	float dEp;   /* Plastic Energy                   */

	void Initialize()
	{
		dEi = 0.;
		dEk = 0.;
		dEs = 0.;
		dEd = 0.;
		dEm = 0.;
		dEv = 0.;
		dEt = 0.;
		dEo = 0.;
		dEh = 0.;
		dEp = 0.;
	}
};
struct T_ENRG_RT_UNIT
{
	T_ENRG_RT_UNIT()
	{
		dEi = D_UNITSYS_BASE_MOMENT;
		dEk = D_UNITSYS_BASE_MOMENT;
		dEs = D_UNITSYS_BASE_MOMENT;
		dEd = D_UNITSYS_BASE_MOMENT;
		dEm = D_UNITSYS_BASE_MOMENT;
		dEv = D_UNITSYS_BASE_MOMENT;
		dEt = D_UNITSYS_BASE_MOMENT;
		dEo = D_UNITSYS_BASE_MOMENT;
		dEh = D_UNITSYS_BASE_MOMENT;
		dEp = D_UNITSYS_BASE_MOMENT;
	}
	int dEi;
	int dEk;
	int dEs;
	int dEd;
	int dEm;
	int dEv;
	int dEt;
	int dEo;
	int dEh;
	int dEp;
};

struct T_GSLH_DL
{
	double dStrain[5][14];
	//center + 4Node
	//    xxt, yyt, xyt, p1t, p2t, aglt, 
	//    xxb, yyb, xyb, p1b, p2b, aglb, 
	//    maxt, maxb
	void Initialize()
	{
		memset(this, 0, sizeof(T_GSLH_DL));
	}

	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)dStrain[i][j]=fabs(dStrain[i][j]);
		}
	}
	void AbsMax(T_GSLH_DL& Data1)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)this->dStrain[i][j]=__max(fabs(this->dStrain[i][j]),fabs(Data1.dStrain[i][j]));
		}
	}
	void AbsMax(T_GSLH_DL& Data1, T_GSLH_DL& Data2)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)dStrain[i][j]=__max(fabs(Data1.dStrain[i][j]),fabs(Data2.dStrain[i][j]));
		}
	}
	void AbsMaxwithSign(T_GSLH_DL& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)
			{
				a = dStrain[i][j]; b = Data.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dStrain[i][j] : Data.dStrain[i][j]);
				// dStrain[i][j]=(fabs(dStrain[i][j]) >= fabs(Data.dStrain[i][j]) ? dStrain[i][j] : Data.dStrain[i][j]);
			}
		}
	}
	void AbsMaxwithSign(T_GSLH_DL& Data1, T_GSLH_DL& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)
			{
				a = Data1.dStrain[i][j]; b = Data2.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
				// dStrain[i][j]=(fabs(Data1.dStrain[i][j]) >= fabs(Data2.dStrain[i][j]) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
			}
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)dStrain[i][j]=sqrt(dStrain[i][j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)dStrain[i][j]*=dFactor;
		}
	}
	void Add(T_GSLH_DL& Data)
	{
		for(int i=0; i<5; i++)
			for(int j=0; j<14; j++)
				dStrain[i][j]+=Data.dStrain[i][j];
	}
	void Max(T_GSLH_DL& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)
				if(Data.dStrain[i][j] > dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Min(T_GSLH_DL& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)
				if(Data.dStrain[i][j] < dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)
				dStrain[i][j] *= dStrain[i][j];
		}
	}
	void calcPrincipalStrain()
	{
		double cc, bb, cr;
		double sigma[4];
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<2; j++)
			{
				int n=j*6;
				for(int k=0; k<3; k++)sigma[k]=0.;
				if(dStrain[i][n+0] == 0. && dStrain[i][n+1] == 0. && dStrain[i][n+2] == 0.)goto END;
				cc = (dStrain[i][n+0]+dStrain[i][n+1])/2.;
				bb = (dStrain[i][n+0]-dStrain[i][n+1])/2.;
				cr = sqrt(pow(bb,2)+pow(dStrain[i][n+2],2));
				sigma[0] = cc+cr;
				sigma[1] = cc-cr;
				if(fabs(bb) <= 1.e-10 && fabs(dStrain[i][n+2]) <= 1.e-10)
					sigma[2] = 0.;
				else if(fabs(sigma[2]) > 1.e-10 && fabs(bb) <= 1.e-10)
				{
					if(dStrain[i][n+2] >= 0.)sigma[2]=45.;
					if(dStrain[i][n+2] < 0.)sigma[2]=-45.;
				}
				else
					sigma[2]=90./(4.*atan(1.))*atan2(dStrain[i][n+2],bb);

				END:
				dStrain[i][n+3]=sigma[0];
				dStrain[i][n+4]=sigma[1];
				dStrain[i][n+5]=sigma[2];
			}

			//Max-Shear
			dStrain[i][12] = fabs(dStrain[i][3]-dStrain[i][4])/2.;
			dStrain[i][13] = fabs(dStrain[i][9]-dStrain[i][10])/2.;
		}
	}
};

struct T_GSLH_DG
{
	double dStrain[5][20];
	//center + 4Node
	//    xxt, yyt, zzt, xyt, yzt, xzt, p1t, p2t, aglt, 
	//    xxb, yyb, zzb, xyb, yzb, xzb, p1b, p2b, aglb,
	//    maxt, maxb
	void Initialize()
	{
		memset(this, 0, sizeof(T_GSLH_DG));
	}

	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)dStrain[i][j]=fabs(dStrain[i][j]);
		}
	}
	void AbsMax(T_GSLH_DG& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)dStrain[i][j]=__max(fabs(dStrain[i][j]),fabs(Data.dStrain[i][j]));
		}
	}
	void AbsMax(T_GSLH_DG& Data1, T_GSLH_DG& Data2)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)dStrain[i][j]=__max(fabs(Data1.dStrain[i][j]),fabs(Data2.dStrain[i][j]));
		}
	}
	void AbsMaxwithSign(T_GSLH_DG& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)
			{
				a = dStrain[i][j]; b = Data.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dStrain[i][j] : Data.dStrain[i][j]);
				// dStrain[i][j]=(fabs(dStrain[i][j]) >= fabs(Data.dStrain[i][j]) ? dStrain[i][j] : Data.dStrain[i][j]);
			}
		}
	}
	void AbsMaxwithSign(T_GSLH_DG& Data1, T_GSLH_DG& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)
			{
				a = Data1.dStrain[i][j]; b = Data2.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
				// dStrain[i][j]=(fabs(Data1.dStrain[i][j]) >= fabs(Data2.dStrain[i][j]) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
			}
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)dStrain[i][j]=sqrt(dStrain[i][j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)dStrain[i][j]*=dFactor;
		}
	}
	void Add(T_GSLH_DG& Data)
	{
		for(int i=0; i<5; i++)
			for(int j=0; j<20; j++)
				dStrain[i][j]+=Data.dStrain[i][j];
	}
	void Max(T_GSLH_DG& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)
				if(Data.dStrain[i][j] > dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Min(T_GSLH_DG& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)
				if(Data.dStrain[i][j] < dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)
				dStrain[i][j] *= dStrain[i][j];
		}
	}
	void calcPrincipalStrain()
	{
		double cc, bb, cr;
		double sigma[3];
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<2; j++)
			{
				int n=j*9;
				if(j == 3)n=29;
				for(int k=0; k<3; k++)sigma[k]=0.;
				if(dStrain[i][n+0] == 0. && dStrain[i][n+1] == 0. && dStrain[i][n+3] == 0.)goto END;
				cc = (dStrain[i][n+0]+dStrain[i][n+1])/2.;
				bb = (dStrain[i][n+0]-dStrain[i][n+1])/2.;
				cr = sqrt(pow(bb,2)+pow(dStrain[i][n+3],2));
				sigma[0] = cc+cr;
				sigma[1] = cc-cr;
				if(fabs(bb) <= 1.e-10 && fabs(dStrain[i][n+3]) <= 1.e-10)
					sigma[2] = 0.;
				else if(fabs(sigma[2]) > 1.e-10 && fabs(bb) <= 1.e-10)
				{
					if(dStrain[i][n+3] >= 0.)sigma[2]=45.;
					if(dStrain[i][n+3] < 0.)sigma[2]=-45.;
				}
				else
					sigma[2]=90./(4.*atan(1.))*atan2(dStrain[i][n+3],bb);
END:
				dStrain[i][n+6]=sigma[0];
				dStrain[i][n+7]=sigma[1];
				dStrain[i][n+8]=sigma[2];
			}

			dStrain[i][18] = fabs(dStrain[i][6]-dStrain[i][7])/2.;
			dStrain[i][19] = fabs(dStrain[i][15]-dStrain[i][16])/2.;
		}
	}
};

struct T_PSLH_DL
{
	double dStrain[5][14];
	//center + 4Node
	//    xxt, yyt, xyt, p1t, p2t, aglt, xxb, yyb, xyb, p1b, p2b, aglb, maxt, maxb
	void Initialize()
	{
		memset(this, 0, sizeof(T_PSLH_DL));
	}

	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)dStrain[i][j]=fabs(dStrain[i][j]);
		}
	}
	void AbsMax(T_PSLH_DL& Data1)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)dStrain[i][j]=__max(fabs(dStrain[i][j]),fabs(Data1.dStrain[i][j]));
		}
	}
	void AbsMax(T_PSLH_DL& Data1, T_PSLH_DL& Data2)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)dStrain[i][j]=__max(fabs(Data1.dStrain[i][j]),fabs(Data2.dStrain[i][j]));
		}
	}
	void AbsMaxwithSign(T_PSLH_DL& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)
			{
				a = dStrain[i][j]; b = Data.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dStrain[i][j] : Data.dStrain[i][j]);
				// dStrain[i][j]=(fabs(dStrain[i][j]) >= fabs(Data.dStrain[i][j]) ? dStrain[i][j] : Data.dStrain[i][j]);
			}
		}
	}
	void AbsMaxwithSign(T_PSLH_DL& Data1, T_PSLH_DL& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)
			{
				a = Data1.dStrain[i][j]; b = Data2.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
				// dStrain[i][j]=(fabs(Data1.dStrain[i][j]) >= fabs(Data2.dStrain[i][j]) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
			}
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)dStrain[i][j]=sqrt(dStrain[i][j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)dStrain[i][j]*=dFactor;
		}
	}
	void Add(T_PSLH_DL& Data)
	{
		for(int i=0; i<5; i++)
			for(int j=0; j<14; j++)
				dStrain[i][j]+=Data.dStrain[i][j];
	}
	void Max(T_PSLH_DL& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)
				if(Data.dStrain[i][j] > dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Min(T_PSLH_DL& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)
				if(Data.dStrain[i][j] < dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<14; j++)
				dStrain[i][j] *= dStrain[i][j];
		}
	}
	void calcPrincipalStrain()
	{
		double cc, bb, cr;
		double sigma[4];
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<2; j++)
			{
				int n=j*6;
				for(int k=0; k<3; k++)sigma[k]=0.;
				if(dStrain[i][n+0] == 0. && dStrain[i][n+1] == 0. && dStrain[i][n+2] == 0.)goto END;
				cc = (dStrain[i][n+0]+dStrain[i][n+1])/2.;
				bb = (dStrain[i][n+0]-dStrain[i][n+1])/2.;
				cr = sqrt(pow(bb,2)+pow(dStrain[i][n+2],2));
				sigma[0] = cc+cr;
				sigma[1] = cc-cr;
				if(fabs(bb) <= 1.e-10 && fabs(dStrain[i][n+2]) <= 1.e-10)
					sigma[2] = 0.;
				else if(fabs(sigma[2]) > 1.e-10 && fabs(bb) <= 1.e-10)
				{
					if(dStrain[i][n+2] >= 0.)sigma[2]=45.;
					if(dStrain[i][n+2] < 0.)sigma[2]=-45.;
				}
				else
					sigma[2]=90./(4.*atan(1.))*atan2(dStrain[i][n+2],bb);

END:
				dStrain[i][n+3]=sigma[0];
				dStrain[i][n+4]=sigma[1];
				dStrain[i][n+5]=sigma[2];
			}

			//Max-Shear
			dStrain[i][12] = fabs(dStrain[i][3]-dStrain[i][4])/2.;
			dStrain[i][13] = fabs(dStrain[i][9]-dStrain[i][10])/2.;
		}
	}
};

struct T_PSLH_DG
{
	double dStrain[5][20];
	//center + 4Node
	//    xxt, yyt, zzt, xyt, yzt, xzt, p1t, p2t, aglt, 
	//    xxb, yyb, zzb, xyb, yzb, xzb, p1b, p2b, aglb, 
	//    maxt, maxb
	void Initialize()
	{
		memset(this, 0, sizeof(T_PSLH_DG));
	}

	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)dStrain[i][j]=fabs(dStrain[i][j]);
		}
	}
	void AbsMax(T_PSLH_DG& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)dStrain[i][j]=__max(fabs(dStrain[i][j]),fabs(Data.dStrain[i][j]));
		}
	}
	void AbsMax(T_PSLH_DG& Data1, T_PSLH_DG& Data2)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)dStrain[i][j]=__max(fabs(Data1.dStrain[i][j]),fabs(Data2.dStrain[i][j]));
		}
	}
	void AbsMaxwithSign(T_PSLH_DG& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)
			{
				a = dStrain[i][j]; b = Data.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dStrain[i][j] : Data.dStrain[i][j]);
				// dStrain[i][j]=(fabs(dStrain[i][j]) >= fabs(Data.dStrain[i][j]) ? dStrain[i][j] : Data.dStrain[i][j]);
			}
		}
	}
	void AbsMaxwithSign(T_PSLH_DG& Data1, T_PSLH_DG& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)
			{
				a = Data1.dStrain[i][j]; b = Data2.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
				// dStrain[i][j]=(fabs(Data1.dStrain[i][j]) >= fabs(Data2.dStrain[i][j]) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
			}
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)dStrain[i][j]=sqrt(dStrain[i][j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)dStrain[i][j]*=dFactor;
		}
	}
	void Add(T_PSLH_DG& Data)
	{
		for(int i=0; i<5; i++)
			for(int j=0; j<20; j++)
				dStrain[i][j]+=Data.dStrain[i][j];
	}
	void Max(T_PSLH_DG& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)
				if(Data.dStrain[i][j] > dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Min(T_PSLH_DG& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)
				if(Data.dStrain[i][j] < dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<20; j++)
				dStrain[i][j] *= dStrain[i][j];
		}
	}
};

struct T_PDID_D
{
	double dDamage[5][6];
	//center + 4Node
	//    compt, tenst, totalt, compb, tensb, totalb
	BOOL bUseDamageRatio;

	void Initialize()
	{
		memset(this, 0, sizeof(T_PDID_D));
	}

	void Abs()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<6; j++)dDamage[i][j]=fabs(dDamage[i][j]);
		}
	}
	void AbsMax(T_PDID_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<6; j++)dDamage[i][j]=__max(fabs(dDamage[i][j]),fabs(Data.dDamage[i][j]));
		}
	}
	void AbsMax(T_PDID_D& Data1, T_PDID_D& Data2)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<6; j++)dDamage[i][j]=__max(fabs(Data1.dDamage[i][j]),fabs(Data2.dDamage[i][j]));
		}
	}
	void AbsMaxwithSign(T_PDID_D& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<6; j++)
			{
				a = dDamage[i][j]; b = Data.dDamage[i][j];
				dDamage[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dDamage[i][j] : Data.dDamage[i][j]);
				// dStrain[i][j]=(fabs(dStrain[i][j]) >= fabs(Data.dStrain[i][j]) ? dStrain[i][j] : Data.dStrain[i][j]);
			}
		}
	}
	void AbsMaxwithSign(T_PDID_D& Data1, T_PDID_D& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<6; j++)
			{
				a = Data1.dDamage[i][j]; b = Data2.dDamage[i][j];
				dDamage[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dDamage[i][j] : Data2.dDamage[i][j]);
				// dStrain[i][j]=(fabs(Data1.dStrain[i][j]) >= fabs(Data2.dStrain[i][j]) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
			}
		}
	}
	void Root()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<6; j++)dDamage[i][j]=sqrt(dDamage[i][j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<6; j++)dDamage[i][j]*=dFactor;
		}
	}
	void Add(T_PDID_D& Data)
	{
		for(int i=0; i<5; i++)
			for(int j=0; j<6; j++)
				dDamage[i][j]+=Data.dDamage[i][j];
	}
	void Max(T_PDID_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<6; j++)
				if(Data.dDamage[i][j] > dDamage[i][j])dDamage[i][j]=Data.dDamage[i][j];
		}
	}
	void Min(T_PDID_D& Data)
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<6; j++)
				if(Data.dDamage[i][j] < dDamage[i][j])dDamage[i][j]=Data.dDamage[i][j];
		}
	}
	void Sqr()
	{
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<6; j++)
				dDamage[i][j] *= dDamage[i][j];
		}
	}
};
// Time History Energy Result(솔버에서 Percent는 double로 출력)
struct T_ENRG_RT_PERCENT_D
{
	double dEi;   /* Input Energy                     */
	double dEk;   /* Kinetic Energy                   */
	double dEs;   /* Strain Energy                    */ 
	double dEd;   /* Damping Energy                   */
	double dEm;   /* Maxwell Damper Energy            */
	double dEv;   /* Velocity Dependent Device Energy */
	double dEt;   /* Strain Dependent Device Energy   */
	double dEo;   /* Isolator Device Energy           */
	double dEh;   /* Dissipated Inelastic Energy      */
	double dEp;   /* Plastic Energy                   */

	void Initialize()
	{
		dEi = 0.;
		dEk = 0.;
		dEs = 0.;
		dEd = 0.;
		dEm = 0.;
		dEv = 0.;
		dEt = 0.;
		dEo = 0.;
		dEh = 0.;
		dEp = 0.;
	}
	void ConvertToEnrgRt(T_ENRG_RT_D& tgt)
	{
		tgt.dEi = static_cast<float>(dEi);
		tgt.dEk = static_cast<float>(dEk);
		tgt.dEs = static_cast<float>(dEs);
		tgt.dEd = static_cast<float>(dEd);
		tgt.dEm = static_cast<float>(dEm);
		tgt.dEv = static_cast<float>(dEv);
		tgt.dEt = static_cast<float>(dEt);
		tgt.dEo = static_cast<float>(dEo);
		tgt.dEh = static_cast<float>(dEh);
		tgt.dEp = static_cast<float>(dEp);
	}
};

// Additional Damping Ratio (에너지그래프 부가감쇠비)
struct T_ADDM_RT_D
{
	double dEm   ; /* Maxwell Damper Energy            */
	double dEv   ; /* Velocity Dependent Device Energy */
	double dEt   ; /* Strain Dependent Device Energy   */
	double dEo   ; /* Isolator Device Energy           */
	double dEh   ; /* Dissipated Inelastic Energy      */
	double dEp   ; /* Plastic Energy                   */
	double dTotal; /* Total Additional Damping Ratio   */

	void Initialize()
	{
		dEm = 0.;
		dEv = 0.;
		dEt = 0.;
		dEo = 0.;
		dEh = 0.;
		dEp = 0.;
		dTotal = 0.;
	}
};

struct T_GSLS_D
{
	double dStrain[9][21];
	//center + 8Node
	//    xx, yy, zz, xy, yz, xz, p1, p2, p3, max, eff, oct
	//    p1(ux,uy,uz), p2(ux,uy,uz), p3(ux,uy,uz)
	//    사실 필요없는 것도 있는데 있던 함수 쓰려고..
	void Initialize()
	{
		memset(this, 0, sizeof(T_GSLS_D));
	}

	void Abs()
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)dStrain[i][j]=fabs(dStrain[i][j]);
		}
	}
	void AbsMax(T_GSLS_D& Data1)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)this->dStrain[i][j]=__max(fabs(this->dStrain[i][j]),fabs(Data1.dStrain[i][j]));
		}
	}
	void AbsMax(T_GSLS_D& Data1, T_GSLS_D& Data2)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)dStrain[i][j]=__max(fabs(Data1.dStrain[i][j]),fabs(Data2.dStrain[i][j]));
		}
	}
	void AbsMaxwithSign(T_GSLS_D& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)
			{
				a = dStrain[i][j]; b = Data.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dStrain[i][j] : Data.dStrain[i][j]);
				// dStrain[i][j]=(fabs(dStrain[i][j]) >= fabs(Data.dStrain[i][j]) ? dStrain[i][j] : Data.dStrain[i][j]);
			}
		}
	}
	void AbsMaxwithSign(T_GSLS_D& Data1, T_GSLS_D& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)
			{
				a = Data1.dStrain[i][j]; b = Data2.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
				// dStrain[i][j]=(fabs(Data1.dStrain[i][j]) >= fabs(Data2.dStrain[i][j]) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
			}
		}
	}
	void Root()
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)dStrain[i][j]=sqrt(dStrain[i][j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)dStrain[i][j]*=dFactor;
		}
	}
	void Add(T_GSLS_D& Data)
	{
		for(int i=0; i<9; i++)
			for(int j=0; j<21; j++)
				dStrain[i][j]+=Data.dStrain[i][j];
	}
	void Max(T_GSLS_D& Data)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)
				if(Data.dStrain[i][j] > dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Min(T_GSLS_D& Data)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)
				if(Data.dStrain[i][j] < dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Sqr()
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)
				dStrain[i][j] *= dStrain[i][j];
		}
	}
};

struct T_PSLS_D
{
	double dStrain[9][21];
	//center + 8Node
	//    xx, yy, zz, xy, yz, xz, p1, p2, p3, max, eff, oct
	//    p1(ux,uy,uz), p2(ux,uy,uz), p3(ux,uy,uz)
	//    사실 필요없는 것도 있는데 있던 함수 쓰려고..
	void Initialize()
	{
		memset(this, 0, sizeof(T_PSLS_D));
	}

	void Abs()
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)dStrain[i][j]=fabs(dStrain[i][j]);
		}
	}
	void AbsMax(T_PSLS_D& Data1)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)this->dStrain[i][j]=__max(fabs(this->dStrain[i][j]),fabs(Data1.dStrain[i][j]));
		}
	}
	void AbsMax(T_PSLS_D& Data1, T_PSLS_D& Data2)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)dStrain[i][j]=__max(fabs(Data1.dStrain[i][j]),fabs(Data2.dStrain[i][j]));
		}
	}
	void AbsMaxwithSign(T_PSLS_D& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)
			{
				a = dStrain[i][j]; b = Data.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dStrain[i][j] : Data.dStrain[i][j]);
				// dStrain[i][j]=(fabs(dStrain[i][j]) >= fabs(Data.dStrain[i][j]) ? dStrain[i][j] : Data.dStrain[i][j]);
			}
		}
	}
	void AbsMaxwithSign(T_PSLS_D& Data1, T_PSLS_D& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)
			{
				a = Data1.dStrain[i][j]; b = Data2.dStrain[i][j];
				dStrain[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
				// dStrain[i][j]=(fabs(Data1.dStrain[i][j]) >= fabs(Data2.dStrain[i][j]) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
			}
		}
	}
	void Root()
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)dStrain[i][j]=sqrt(dStrain[i][j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)dStrain[i][j]*=dFactor;
		}
	}
	void Add(T_PSLS_D& Data)
	{
		for(int i=0; i<9; i++)
			for(int j=0; j<21; j++)
				dStrain[i][j]+=Data.dStrain[i][j];
	}
	void Max(T_PSLS_D& Data)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)
				if(Data.dStrain[i][j] > dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Min(T_PSLS_D& Data)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)
				if(Data.dStrain[i][j] < dStrain[i][j])dStrain[i][j]=Data.dStrain[i][j];
		}
	}
	void Sqr()
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<21; j++)
				dStrain[i][j] *= dStrain[i][j];
		}
	}
};

struct T_SDID_D
{
	double dDamage[9][3];
	//center + 8Node
	//    compt, tenst, totalt, compb, tensb, totalb
	BOOL bUseDamageRatio;

	void Initialize()
	{
		memset(this, 0, sizeof(T_SDID_D));
	}

	void Abs()
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<3; j++)dDamage[i][j]=fabs(dDamage[i][j]);
		}
	}
	void AbsMax(T_SDID_D& Data1, T_SDID_D& Data2)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<3; j++)dDamage[i][j]=__max(fabs(Data1.dDamage[i][j]),fabs(Data2.dDamage[i][j]));
		}
	}
	void AbsMax(T_SDID_D& Data)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<3; j++)dDamage[i][j]=__max(fabs(dDamage[i][j]),fabs(Data.dDamage[i][j]));
		}
	}
	void AbsMaxwithSign(T_SDID_D& Data, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<3; j++)
			{
				a = dDamage[i][j]; b = Data.dDamage[i][j];
				dDamage[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? dDamage[i][j] : Data.dDamage[i][j]);
				// dStrain[i][j]=(fabs(dStrain[i][j]) >= fabs(Data.dStrain[i][j]) ? dStrain[i][j] : Data.dStrain[i][j]);
			}
		}
	}
	void AbsMaxwithSign(T_SDID_D& Data1, T_SDID_D& Data2, double dZero=1.e-10) // fixed by mylee
	{
		double a, b;
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<3; j++)
			{
				a = Data1.dDamage[i][j]; b = Data2.dDamage[i][j];
				dDamage[i][j]=(( fabs(a)-fabs(b) > dZero || (fabs(fabs(a)-fabs(b)) <= dZero && a > b) ) ? Data1.dDamage[i][j] : Data2.dDamage[i][j]);
				// dStrain[i][j]=(fabs(Data1.dStrain[i][j]) >= fabs(Data2.dStrain[i][j]) ? Data1.dStrain[i][j] : Data2.dStrain[i][j]);
			}
		}
	}
	void Root()
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<3; j++)dDamage[i][j]=sqrt(dDamage[i][j]);
		}
	}
	void Mult(double dFactor)
	{
		if(dFactor == 1.)return;
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<3; j++)dDamage[i][j]*=dFactor;
		}
	}
	void Add(T_SDID_D& Data)
	{
		for(int i=0; i<9; i++)
			for(int j=0; j<3; j++)
				dDamage[i][j]+=Data.dDamage[i][j];
	}
	void Max(T_SDID_D& Data)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<3; j++)
				if(Data.dDamage[i][j] > dDamage[i][j])dDamage[i][j]=Data.dDamage[i][j];
		}
	}
	void Min(T_SDID_D& Data)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<3; j++)
				if(Data.dDamage[i][j] < dDamage[i][j])dDamage[i][j]=Data.dDamage[i][j];
		}
	}
	void Sqr()
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<3; j++)
				dDamage[i][j] *= dDamage[i][j];
		}
	}
};

// Element Rotation Result - Time History 
struct T_POST_AGBT_D
{
	double dMaxVal_Ry_I ;
	double dMaxVal_Ry_J ;
	double dMaxVal_Rz_I ;
	double dMaxVal_Rz_J ;
	double dMinVal_Ry_I ;
	double dMinVal_Ry_J ;
	double dMinVal_Rz_I ;
	double dMinVal_Rz_J ;
	double dMaxTime_Ry_I;
	double dMaxTime_Ry_J;
	double dMaxTime_Rz_I;
	double dMaxTime_Rz_J;
	double dMinTime_Ry_I;
	double dMinTime_Ry_J;
	double dMinTime_Rz_I;
	double dMinTime_Rz_J;

	T_POST_AGBT_D() { Initialize(); }
	void Initialize()
	{
		dMaxVal_Ry_I  = 0.0;
		dMaxVal_Ry_J  = 0.0;
		dMaxVal_Rz_I  = 0.0;
		dMaxVal_Rz_J  = 0.0;
		dMinVal_Ry_I  = 0.0;
		dMinVal_Ry_J  = 0.0;
		dMinVal_Rz_I  = 0.0;
		dMinVal_Rz_J  = 0.0;
		dMaxTime_Ry_I = 0.0;
		dMaxTime_Ry_J = 0.0;
		dMaxTime_Rz_I = 0.0;
		dMaxTime_Rz_J = 0.0;
		dMinTime_Ry_I = 0.0;
		dMinTime_Ry_J = 0.0;
		dMinTime_Rz_I = 0.0;
		dMinTime_Rz_J = 0.0;
	}
};

// Element Rotation Result - Pushover
struct T_POST_AGBP_D
{
	double dVal_Ry_I;
	double dVal_Ry_J;
	double dVal_Rz_I;
	double dVal_Rz_J;

	T_POST_AGBP_D() { Initialize(); }
	void Initialize()
	{
		dVal_Ry_I = 0.0;
		dVal_Ry_J = 0.0;
		dVal_Rz_I = 0.0;
		dVal_Rz_J = 0.0;
	}
};
// ==========================

#pragma pack()

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#include "HeaderPost.h"
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#endif
