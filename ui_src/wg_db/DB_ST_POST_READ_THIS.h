#ifndef __DB_ST_POST_READ_THIS_H__
#define __DB_ST_POST_READ_THIS_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

struct _GLNP_THIS_BN  // _STRP_STLD_BN에서 2번째 변수 삭제 + double을 float으로 수정
{
	// dblForce     : local force                 , 4 node       ,
	//                component : FX, FY, FZ, MX, MY, MZ  
	// dblStress    : local stress + unit force   , cent + 4 node, 
	//                component : SXXt, SYYt, SXYt, SXXb, SYYb, SXYb, FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

	float dblForce[4][6];
	float dblStress[5][14];
	void Add(_STRP_STLD_BN& Data)
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<6; j++)dblForce[i][j]+=(float)Data.dblForce[i][j];
		for(int i = 0; i<5; i++)
			for(int j=0; j<14; j++)dblStress[i][j]+=(float)Data.dblStress[i][j];
	}
	void ConvertOut(T_STRP_DL& Data)
	{
		Data.Initialize();
		for(int j=0; j<4; j++)
		{
			for(int k=0; k<6; k++)Data.dblForce[j][k]=dblForce[j][k];
		}
		for(int j=0; j<5; j++)
		{
			Data.dblStress[j][ 0]=dblStress[j][ 0];
			Data.dblStress[j][ 1]=dblStress[j][ 1];
			Data.dblStress[j][ 2]=dblStress[j][ 2];
			Data.dblStress[j][ 7]=dblStress[j][ 3];
			Data.dblStress[j][ 8]=dblStress[j][ 4];
			Data.dblStress[j][ 9]=dblStress[j][ 5];
			Data.dblStress[j][14]=dblStress[j][ 6];
			Data.dblStress[j][15]=dblStress[j][ 7];
			Data.dblStress[j][16]=dblStress[j][ 8];
			Data.dblStress[j][20]=dblStress[j][ 9];
			Data.dblStress[j][21]=dblStress[j][10];
			Data.dblStress[j][22]=dblStress[j][11];
			Data.dblStress[j][26]=dblStress[j][12];
			Data.dblStress[j][27]=dblStress[j][13];
		}
	}
	void ConvertIn(T_STRP_DL& Data)
	{
		for(int j=0; j<4; j++)for(int k=0; k<6; k++)dblForce[j][k]=(float)Data.dblForce[j][k];
		for(int j=0; j<5; j++)
		{
			dblStress[j][ 0]=(float)Data.dblStress[j][ 0];
			dblStress[j][ 1]=(float)Data.dblStress[j][ 1];
			dblStress[j][ 2]=(float)Data.dblStress[j][ 2];
			dblStress[j][ 3]=(float)Data.dblStress[j][ 7];
			dblStress[j][ 4]=(float)Data.dblStress[j][ 8];
			dblStress[j][ 5]=(float)Data.dblStress[j][ 9];
			dblStress[j][ 6]=(float)Data.dblStress[j][14];
			dblStress[j][ 7]=(float)Data.dblStress[j][15];
			dblStress[j][ 8]=(float)Data.dblStress[j][16];
			dblStress[j][ 9]=(float)Data.dblStress[j][20];
			dblStress[j][10]=(float)Data.dblStress[j][21];
			dblStress[j][11]=(float)Data.dblStress[j][22];
			dblStress[j][12]=(float)Data.dblStress[j][26];
			dblStress[j][13]=(float)Data.dblStress[j][27];
		}
	}
};
#define _GLNP_THIS_860_BN _STRP_STLD_BN
#define _EPRP_THIS_BN _GLNP_THIS_BN

struct _REAC_SSPS_THIS_BN
{
	double dMaxForce[5];
	double dMaxDisplacement[5];
	double dMinForce[5];
	double dMinDisplacement[5];
	double dForceMaxTime[5];
	double dDispMaxTime[5];  
	double dForceMinTime[5];
	double dDispMinTime[5];  
	double dElement;
	double dDirection;
	double dElementType;
};

struct _REAC_THIS_7DOF_BN
{
	T_REAC_D max;
	T_REAC_D min;
	double dMaxTime[REAC_SIZE];
	double dMinTime[REAC_SIZE];
};

struct _REAC_THIS_BN
{
	_REAC_STLD_BN max;
	_REAC_STLD_BN min;
	double dMaxTime[6];
	double dMinTime[6];
};

struct _LRAC_THIS_7DOF_BN
{
	T_REAC_D max;
	T_REAC_D min;
	double dMaxTime[REAC_SIZE];
	double dMinTime[REAC_SIZE];
	double ino;
};

struct _LRAC_THIS_BN
{
	_REAC_STLD_BN max;
	_REAC_STLD_BN min;
	double dMaxTime[6];
	double dMinTime[6];
	double ino;
};

struct _DISP_THIS_BN
{
	_DISP_STLD_BN maxDis;      // 변위
	_DISP_STLD_BN minDis;
	double dMaxDisTime[6];
	double dMinDisTime[6];
	_DISP_STLD_BN maxVel;      // 속도
	_DISP_STLD_BN minVel;
	double dMaxVelTime[6];
	double dMinVelTime[6];
	_DISP_STLD_BN maxAcc;      // 상대 가속도 
	_DISP_STLD_BN minAcc;
	double dMaxTimeAcc[6];
	double dMinTimeAcc[6];
	_DISP_STLD_BN maxAAc;      // 절대 가속도
	_DISP_STLD_BN minAAc;
	double dMaxAAcTime[6];
	double dMinAAcTime[6];

	void ConvertOut(T_DISP_TH_D& Data);   // .cpp 파일로 구현부 이동함.
};

struct _STRT_THIS_BN
{
	double dblStressMax[4];
	double dblStressMin[4];
	double dMaxTime[4];
	double dMinTime[4];
	int iel;
};

struct _STBW_THIS_BN
{
	double dblStressMax[72];
	double dblCombStrsMax[2][4];      // Cb1, Cb2, Cb3, Cb4
	double dblCombStrsMaxEtc[2][8];   // CombTau1i,.... CombTau8i, CombTau1j,.... CombTau8j (for 7th DOF : 16개)
	double dblStressMin[72];
	double dblCombStrsMin[2][4];
	double dblCombStrsMinEtc[2][8];
	double dMaxTime[72];
	double dCombStrsMaxTime[2][4];
	double dCombStrsMaxTimeEtc[2][8];
	double dMinTime[72];
	double dCombStrsMinTime[2][4];
	double dCombStrsMinTimeEtc[2][8];
	int iel;
	void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin);  // .cpp 파일로 구현부 이동함.

	// Max,Min Time을 Covert함 
	void ConvertOutTime(T_STRB_D& DataMax, T_STRB_D& DataMin);  // .cpp 파일로 구현부 이동함.
};

struct _STRB_THIS_BN
{
	/*
	double dblStressMax[4][26];
	double dblCombStrsMax[4][2][4];
	double dblStressMin[4][26];
	double dblCombStrsMin[4][2][4];
	double dMaxTime[4][26];
	double dCombStrsMaxTime[4][2][4];
	double dMinTime[4][26];
	double dCombStrsMinTime[4][2][4];
	*/
	// V650이후 I,J단의 값만 출력
	double dblStressMax[26];
	double dblCombStrsMax[2][4];
	double dblStressMin[26];
	double dblCombStrsMin[2][4];
	double dMaxTime[26];
	double dCombStrsMaxTime[2][4];
	double dMinTime[26];
	double dCombStrsMinTime[2][4];
	int iel;
	void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin);  // .cpp 파일로 구현부 이동함.

	// Max,Min Time을 Covert함 
	void ConvertOutTime(T_STRB_D& DataMax, T_STRB_D& DataMin);  // .cpp 파일로 구현부 이동함.
};

struct _STRM_THIS_BN
{
// dblForce     : local force                 , 4 node       ,
//                component : FX, FY, FZ
// dblStress    : local stress                , cent + 4 node, 
//                component : SXX, SYY, SXY, P1, P2, ANG, EFF
	double dblForceMax[4][3];
	double dblStressMax[5][7];
	double dblForceMin[4][3];
	double dblStressMin[5][7];
	double dForceMaxTime[4][3];
	double dStressMaxTime[5][7];
	double dForceMinTime[4][3];
	double dStressMinTime[5][7];
	int iel;
	int ino[4];
};

struct _STRP_THIS_BN
{
// dblForce     : local force                 , 4 node       ,
//                component : FX, FY, FZ, MX, MY, MZ
// dblStressIOS : local stress(membrane, bending, shear), cent + 4 node, 
//                component : SMX, SMY, SMZ, SBX, SBY, SBZ, VX, VY
// dblStress    : local stress + unit force   , cent + 4 node, 
//                component : SXXt, SYYt, SXYt, SXXb, SYYb, SXYb, FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY
	double dblForceMax[4][6];
	double dblStressIOSMax[5][8];
	double dblStressMax[5][14];
	double dblForceMin[4][6];
	double dblStressIOSMin[5][8];
	double dblStressMin[5][14];
	double dForceMaxTime[4][6];
	double dStressIOSMaxTime[5][8];
	double dStressMaxTime[5][14];
	double dForceMinTime[4][6];
	double dStressIOSMinTime[5][8];
	double dStressMinTime[5][14];
	int iel;
	int ino[4];

	void ConvertToSpcf(T_SPCF_DL& DataMax, T_SPCF_DL& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<8; j++)
			{
				DataMax.dForce[i][j][0] = dblStressMax[i][6];
				DataMax.dForce[i][j][1] = dblStressMax[i][7];
				DataMax.dForce[i][j][2] = dblStressMax[i][8];
				DataMax.dForce[i][j][3] = dblStressMax[i][9];
				DataMax.dForce[i][j][4] = dblStressMax[i][10];
				DataMax.dForce[i][j][5] = dblStressMax[i][11];
				DataMax.dForce[i][j][6] = dblStressMax[i][12];
				DataMax.dForce[i][j][7] = dblStressMax[i][13];

				DataMin.dForce[i][j][0] = dblStressMin[i][6];
				DataMin.dForce[i][j][1] = dblStressMin[i][7];
				DataMin.dForce[i][j][2] = dblStressMin[i][8];
				DataMin.dForce[i][j][3] = dblStressMin[i][9];
				DataMin.dForce[i][j][4] = dblStressMin[i][10];
				DataMin.dForce[i][j][5] = dblStressMin[i][11];
				DataMin.dForce[i][j][6] = dblStressMin[i][12];
				DataMin.dForce[i][j][7] = dblStressMin[i][13];
			}
		}
	}
};

struct _STRS_THIS_BN
{
// dblForce  : global force , 8 node       , component : FX, FY, FZ
// dblStress : global stress, cent + 8 node, component : SXX, SYY, SZZ, SXY, SYZ, SXZ
	double dblForceMax[8][3];
	double dblStressMax[1+8][6];
	double dblForceMin[8][3];
	double dblStressMin[1+8][6];
	double dForceMaxTime[8][3];
	double dStressMaxTime[1+8][6];
	double dForceMinTime[8][3];
	double dStressMinTime[1+8][6];
	int iel;
	int ino[8];
	void ConvertOut(T_STRS_D& DataMax, T_STRS_D& DataMin);  // .cpp 파일로 구현부 이동함.
};

struct _STRW_THIS_BN
{
// dblForce     : local force         , 2 part, top, bottom
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
	double dblForceMax[2][6];
	double dblForceMin[2][6];
	double dForceMaxTime[2][6];
	double dForceMinTime[2][6];
	int iel;
};

struct _STPN_THIS_BN
{
// dblForce  : global force , 4 node       , component : FX, FY, FZ.
// dblStress : global stress, cent + 4 node, component : SXX, SZZ, SYY=0., SXZ
	double dblForceMax[4][3];
	double dblStressMax[1+4][4];
	double dblForceMin[4][3];
	double dblStressMin[1+4][4];
	double dblForceMaxTime[4][3];
	double dblStressMaxTime[1+4][4];
	double dblForceMinTime[4][3];
	double dblStressMinTime[1+4][4];
	int iel;
	int ino[4];
	void ConvertOut(T_STPN_D& DataMax, T_STPN_D& DataMin);  // .cpp 파일로 구현부 이동함.
};

struct _ELNK_THIS_BN
{
// dblForce  : local force , 2 node       , component : fx, fy, fz, mx, my, mz
	double dblForceMax[2][6];
	double dblForceMin[2][6];
	double dForceMaxTime[2][6];
	double dForceMinTime[2][6];
	int iel;
	int ino[2];

//   void ConvertToStck(T_STCK_D& DataMax, T_STCK_D& DataMin)
//   {
//     //Max
//     DataMax.Initialize();
//     for(int i=0; i<2; i++) // i, j
//     {
//       for(int k=0; k<6; k++) DataMax.dblForce[i][0][k]=dblForceMax[i][k];
// 
//       for(int j=1; j<6; j++) memcpy(DataMax.dblForce[i][j], DataMax.dblForce[i][0], sizeof(DataMax.dblForce[i][0]));
//     }
// 
//     //Min
//     DataMin.Initialize();
//     for(int i=0; i<2; i++) // i, j
//     {
//       for(int k=0; k<6; k++) DataMin.dblForce[i][0][k]=dblForceMin[i][k];
// 
//       for(int j=1; j<6; j++) memcpy(DataMin.dblForce[i][j], DataMin.dblForce[i][0], sizeof(DataMin.dblForce[i][0]));
//     }
//   }
};

struct _BMCR_THIS_BN
{
	// dblForce  : local force , 2 node   , component : fx, fy, fz, mx, my, mz, Time
	double dblForceMax[2][6][7];
	double dblForceMin[2][6][7];

	void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();

		double dRatio = 0.0;

		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				if(j==0) dRatio = ((double)i) / 4.0; 
				else     dRatio = ((double)(i+1)) / 4.0;

				for(int k=0; k<6; k++)
				{
					for(int l=0; l<6; l++)
					{
						DataMax.dblForce[i][j][k][l] = dblForceMax[0][k][l] + (dblForceMax[1][k][l] - dblForceMax[0][k][l]) * dRatio;
						DataMin.dblForce[i][j][k][l] = dblForceMin[0][k][l] + (dblForceMin[1][k][l] - dblForceMin[0][k][l]) * dRatio;
					}
				}
			}
		}
	}
};

struct _WMCR_THIS_BN
{
	// dblForce  : local force , 2 node   , component : fx, fy, fz, mx, my, mz, Bi, St, Wt, Time
	double dblForceMax[2][9][10];
	double dblForceMin[2][9][10];

	void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();

		double dRatio = 0.0;

		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				if(j==0) dRatio = ((double)i) / 4.0; 
				else     dRatio = ((double)(i+1)) / 4.0;

				for(int k=0; k<9; k++)
				{
					for(int l=0; l<9; l++)
					{
						DataMax.dblForce[i][j][k][l] = dblForceMax[0][k][l] + (dblForceMax[1][k][l] - dblForceMax[0][k][l]) * dRatio;
						DataMin.dblForce[i][j][k][l] = dblForceMin[0][k][l] + (dblForceMin[1][k][l] - dblForceMin[0][k][l]) * dRatio;
					}
				}
			}
		}
	}
};

struct _ELCF_THIS_BN
{
	// dblForce  : local force , 2 node   , component : fx, fy, fz, mx, my, mz, Time
	double dblForceMax[2][6][7];
	double dblForceMin[2][6][7];

	void ConvertOut(T_STCK_D& DataMax, T_STCK_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();

		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
			{
				for(int k=0; k<6; k++)
				{
					DataMax.dblForce[i][j][k] = dblForceMax[i][j][k];
					DataMin.dblForce[i][j][k] = dblForceMin[i][j][k];
				}
			}
		}
	}
};

struct _GLCF_THIS_BN
{
	// dblForce  : local force , 2 node   , component : fx, fy, fz, mx, my, mz, Time
	double dblForceMax[2][6][7];
	double dblForceMin[2][6][7];

	void ConvertOut(T_STCK_D& DataMax, T_STCK_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();

		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)
			{
				for(int k=0; k<6; k++)
				{
					DataMax.dblForce[i][j][k] = dblForceMax[i][j][k];
					DataMin.dblForce[i][j][k] = dblForceMin[i][j][k];
				}
			}
		}
	}
};

#define _NLNK_THIS_BN _ELNK_THIS_BN

struct _NDFM_THIS_BN
{
// dblForce  : local force , 2 node       , component : fx, fy, fz, mx, my, mz
	double dblDispMax[6];
	double dblDispMin[6];
	double dDispMaxTime[6];
	double dDispMinTime[6];
};

// beam concurrent force
struct _SBCF_THIS_BN
{
	double dblForceMax[4][2][6][6];  // 4 part, i,j, 
	double dblForceMin[4][2][6][6];  // 4 part, i,j, 
	int iel;
	void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin);  // .cpp 파일로 구현부 이동함.
	void Add(_SBCF_THIS_BN& Data);  // .cpp 파일로 구현부 이동함.
};

// 7DOF beam concurrent force
struct _SBCF_THIS_7DOF_BN
{
	double dblForceMax[4][2][9][9];  // 4 part, i,j, 
	double dblForceMin[4][2][9][9];  // 4 part, i,j, 
	int iel;
	void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin);  // .cpp 파일로 구현부 이동함.
	void Add(_SBCF_THIS_7DOF_BN& Data);  // .cpp 파일로 구현부 이동함.
};

// virtual beam concurrent force
struct _SIMC_THIS_BN
{
	double dblForceMax[2][6][6];  // i,j, 
	double dblForceMin[2][6][6];  // i,j, 
	int iel;
	void ConvertOut(T_SIMC_D& DataMax, T_SIMC_D& DataMin);  // .cpp 파일로 구현부 이동함.
	void Add(_SIMC_THIS_BN& Data);  // .cpp 파일로 구현부 이동함.
};

struct _MDFL_BN
{
	double dDispl;
	double dVelocity;
	double dAcceleration;
	void Initialize();  // .cpp 파일로 구현부 이동함.
};

struct _INDF_BN
{
	double dAcceleration;
	double dVelocity;
	double dDispl;
	double dDynamicNode; //사용안함 
	void Initialize();  // .cpp 파일로 구현부 이동함.
};


#pragma pack()

#endif