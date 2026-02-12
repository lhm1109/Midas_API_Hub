#ifndef __DB_ST_POST_READ_STLD_H__
#define __DB_ST_POST_READ_STLD_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

// static
#define _REAC7_STLD_BN T_REAC_D
#define _DISP7_STLD_BN T_DISP_D

struct _REAC_STLD_BN
{
	double dblReac[6];

	void ConvertOut(T_REAC_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<6; i++)Data.dblReac[i]=dblReac[i];
	}
	void ConvertIn(T_REAC_D& Data)
	{
		for(int i=0; i<6; i++)dblReac[i]=Data.dblReac[i];
	}
};

struct _LRAC_STLD_7DOF_BN
{
	double dblReac[REAC_SIZE];
	double ino;
	void ConvertOut(T_REAC_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<REAC_SIZE; i++)Data.dblReac[i]=dblReac[i];
	}
};

struct _LRAC_STLD_BN
{
	double dblReac[6];
	double ino;
	void ConvertOut(T_REAC_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<6; i++)Data.dblReac[i]=dblReac[i];
	}
};

struct _DISP_STLD_BN
{
	double dblDisp[6];
	void ConvertOut(T_DISP_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<6; i++)Data.dblDisp[i]=dblDisp[i];
	}
	void ConvertIn(T_DISP_D& Data)
	{
		for(int i=0; i<6; i++)dblDisp[i]=Data.dblDisp[i];
	}
};

struct _ELNK_STLD_BN
{
// dblForce  : local force , 2 node       , component : fx, fy, fz, mx, my, mz
	double dblForce[2][6];
	int iel;
	int ino[2];
	void Add(_ELNK_STLD_BN& Data)
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)dblForce[i][j]+=Data.dblForce[i][j];
		}
	}
	void ConvertOut(T_STEK_D& Data)
	{
		Data.Initialize();
		ASSERT(sizeof(dblForce) == sizeof(Data.dblForce));
		memcpy(&Data.dblForce, dblForce, sizeof(dblForce));
	}
	void ConvertIn(T_STEK_D& Data)
	{
		ASSERT(sizeof(dblForce) == sizeof(Data.dblForce));
		memcpy(&dblForce, Data.dblForce, sizeof(dblForce));
	}
//   void ConvertToStck(T_STCK_D& DataStck)
//   {
//     DataStck.Initialize();
//     for(int i=0; i<2; i++) // i, j
//     {
//       for(int k=0; k<6; k++) DataStck.dblForce[i][0][k]=dblForce[i][k];
// 
//       for(int j=1; j<6; j++) memcpy(DataStck.dblForce[i][j], DataStck.dblForce[i][0], sizeof(DataStck.dblForce[i][0]));
//     }
//   }

};

struct _NLDE_STLD_BN
{
	//General Link Defomation:Static,RS
	double iel;
	double ino[2];
	double dblDisp[6];
	/*void Add(_NLDE_STLD_BN& Data)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 6; j++)dblForce[i][j] += Data.dblForce[i][j];
		}
	}*/
	//void ConvertOut(T_DISP_D& Data)
	//{
	//	Data.Initialize();
	//	ASSERT(sizeof(dblForce) == sizeof(Data.dblForce));
	//	memcpy(&Data.dblForce, dblForce, sizeof(dblForce));
	//}
	//void ConvertIn(T_DISP_D& Data)
	//{
	//	ASSERT(sizeof(dblForce) == sizeof(Data.dblForce));
	//	memcpy(&dblForce, Data.dblForce, sizeof(dblForce));
	//}
};

#define _NLNK_STLD_BN _ELNK_STLD_BN

struct _STRT_STLD_BN
{
// dblStress    : local force, stress         , 
//                component : Fxi, Fxj, Sxi, Sxj
	double dblStress[4];
	int iel;
	void Add(_STRT_STLD_BN& Data)
	{
		for(int i=0; i<4; i++)dblStress[i]+=Data.dblStress[i];
	}
	void ConvertOut(T_STRT_D& Data)
	{
		Data.Initialize();
		Data.dblForce[0]=dblStress[0];
		Data.dblForce[1]=dblStress[1];
		Data.dblStress[0]=dblStress[2];
		Data.dblStress[1]=dblStress[3];
	}
	void ConvertIn(T_STRT_D& Data)
	{
		dblStress[0]=Data.dblForce[0];
		dblStress[1]=Data.dblForce[1];
		dblStress[2]=Data.dblStress[0];
		dblStress[3]=Data.dblStress[1];
	}
};

struct _VMST_STLD_BN
{
	double dblStress[4][56][7]; // [4 part][I:28개 단면지점,J:28개 단면지점][7개 성분]
	void ConvertOut(T_VMST_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<4; i++)
				for(int j=0; j<56; j++)
					for(int k=0; k<7; k++)
						Data.dblStress[i][j][k] = dblStress[i][j][k];
	}
};

struct _STRB_STLD_BN
{
	// dblStress    : local force, stress         , 4 part, 
	//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj,
	//                            Sxi, Syi, Szi, Bypi, Bymi, Bzpi, Bzmi, Sxj, Syj, Szj, Bypj, Bymj, Bzpj, Bzmj
	double dblStress[4][26];
	double dCombStrs[4][2][4];
	int iel;
	void ConvertOut(T_STRB_D& DataStrb)
	{
		DataStrb.Initialize();
		DataStrb.nDOFType=1;
		for(int j=0; j<4; j++)
		{
			for(int k=0; k<12; k++)DataStrb.dblForce[j][k]=dblStress[j][k];
			for(int l=0; l<14; l++)DataStrb.dblStress[j][l]=dblStress[j][l+12];
			DataStrb.dblStress[j][16]=dCombStrs[j][0][0];
			DataStrb.dblStress[j][18]=dCombStrs[j][0][1];
			DataStrb.dblStress[j][20]=dCombStrs[j][0][2];
			DataStrb.dblStress[j][22]=dCombStrs[j][0][3];
			DataStrb.dblStress[j][17]=dCombStrs[j][1][0];
			DataStrb.dblStress[j][19]=dCombStrs[j][1][1];
			DataStrb.dblStress[j][21]=dCombStrs[j][1][2];
			DataStrb.dblStress[j][23]=dCombStrs[j][1][3];
		}
		DataStrb.calcMaximumCombinedStress();
	}
	void ConvertIn(T_STRB_D& DataStrb)
	{
		for(int j=0; j<4; j++)
		{
			for(int k=0; k<12; k++)dblStress[j][k]    = DataStrb.dblForce[j][k];
			for(int l=0; l<14; l++)dblStress[j][l+12] = DataStrb.dblStress[j][l];
			dCombStrs[j][0][0] = DataStrb.dblStress[j][16];
			dCombStrs[j][0][1] = DataStrb.dblStress[j][18];
			dCombStrs[j][0][2] = DataStrb.dblStress[j][20];
			dCombStrs[j][0][3] = DataStrb.dblStress[j][22];
			dCombStrs[j][1][0] = DataStrb.dblStress[j][17];
			dCombStrs[j][1][1] = DataStrb.dblStress[j][19];
			dCombStrs[j][1][2] = DataStrb.dblStress[j][21];
			dCombStrs[j][1][3] = DataStrb.dblStress[j][23];
		}
		//DataStrb.calcMaximumCombinedStress();
	}
	static void ConvertToSbcf(T_STRB_D& DataStrb, T_SBCF_D& DataSbcf)
	{
		DataSbcf.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				for(int k=0; k<6; k++)DataSbcf.dblForce[i][j][0][k]=DataStrb.dblForce[i][j*6+k];
				for(int k=1; k<6; k++)memcpy(DataSbcf.dblForce[i][j][k], DataSbcf.dblForce[i][j][0], sizeof(DataSbcf.dblForce[i][j][0]));
			}
		}
	}
	void ConvertToSbcf(T_SBCF_D& DataSbcf)
	{
		DataSbcf.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				for(int k=0; k<6; k++)DataSbcf.dblForce[i][j][0][k]=dblStress[i][j*6+k];
				for(int k=1; k<6; k++)memcpy(DataSbcf.dblForce[i][j][k], DataSbcf.dblForce[i][j][0], sizeof(DataSbcf.dblForce[i][j][0]));
			}
		}
	}
	void ConvertToSbcs(T_SBCS_D& DataSbcs)
	{
		DataSbcs.Initialize();
		for(int m=0; m<4; m++)       // CB1~CB4
		{
			for(int i=0; i<4; i++)     // 4Part
			{
				for(int j=0; j<2; j++)   // i-end,j-end
				{
					for(int k=0; k<6; k++)DataSbcs.dblForce[m][i][j][k]=dblStress[i][j*6+k];
					DataSbcs.dblForce[m][i][j][6]=dCombStrs[i][j][m];
				}
			}
		}
	}	
	void Add(_STRB_STLD_BN& Data)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<26; j++)dblStress[i][j]+=Data.dblStress[i][j];
			for(int j=0; j<2; j++)
			{
				for(int k=0; k<4; k++)dCombStrs[i][j][k]+=Data.dCombStrs[i][j][k];
			}
		}
	}
};

struct _STBW_STLD_BN
{
// dblStress    : local force, stress         , 4 part, 
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj, (12개)
//														BiMi, BiMj, ST.VTi, ST.VTj, WTi, WTj, (_STRB_STLD_BN force에 6개 추가)
//                            Sxi, Syi, Szi, Bypi, Bymi, Bzpi, Bzmi, Sxj, Syj, Szj, Bypj, Bymj, Bzpj, Bzmj (14개)
//														WnS1i, WnS2i, WnS3i, WnSri,
//														ST.VTSxy1i, ST.VTSxz1i, ST.VTSxy2i, ST.VTSxz2i, ST.VTSxy3i, ST.VTSxz3i, ST.VTSxy4i, ST.VTSxz4i,
//														WTSxy1i, WTSxz1i, WTSxy2i, WTSxz2i, WTSxy3i, WTSxz3i, WTSxy4i, WTSxz4i,
//														WnS1j, WnS2j, WnS3j, WnSrj,
//														ST.VTSxy1j, ST.VTSxz1j, ST.VTSxy2j, ST.VTSxz2j, ST.VTSxy3j, ST.VTSxz3j, ST.VTSxy4j, ST.VTSxz4j,
//														WTSxy1j, WTSxz1j, WTSxy2j, WTSxz2j, WTSxy3j, WTSxz3j, WTSxy4j, WTSxz4j (_STRB_STLD_BN stress에 40개 추가)
// dCombStrs		:
	double dblStress[4][72];
	double dCombStrs[4][24];
	int iel;
	void ConvertOut(T_STRB_D& DataStrb)
	{
		DataStrb.Initialize();
		DataStrb.nDOFType=2;
		for(int j=0; j<4; j++)
		{
			for(int k=0; k<18; k++)DataStrb.dblForce[j][k]=dblStress[j][k];
			for(int l=0; l<14; l++)DataStrb.dblStress[j][l]=dblStress[j][l+18];
			DataStrb.dblStress[j][16]=dCombStrs[j][0];
			DataStrb.dblStress[j][18]=dCombStrs[j][1];
			DataStrb.dblStress[j][20]=dCombStrs[j][2];
			DataStrb.dblStress[j][22]=dCombStrs[j][3];
			DataStrb.dblStress[j][17]=dCombStrs[j][4];
			DataStrb.dblStress[j][19]=dCombStrs[j][5];
			DataStrb.dblStress[j][21]=dCombStrs[j][6];
			DataStrb.dblStress[j][23]=dCombStrs[j][7];
			for(int l=14; l<54; l++)DataStrb.dblStress[j][l+10]=dblStress[j][l+18];
			DataStrb.dblStress[j][64]=dCombStrs[j][8];
			DataStrb.dblStress[j][66]=dCombStrs[j][9];
			DataStrb.dblStress[j][68]=dCombStrs[j][10];
			DataStrb.dblStress[j][70]=dCombStrs[j][11];
			DataStrb.dblStress[j][72]=dCombStrs[j][12];
			DataStrb.dblStress[j][74]=dCombStrs[j][13];
			DataStrb.dblStress[j][76]=dCombStrs[j][14];
			DataStrb.dblStress[j][78]=dCombStrs[j][15];
			DataStrb.dblStress[j][65]=dCombStrs[j][16];
			DataStrb.dblStress[j][67]=dCombStrs[j][17];
			DataStrb.dblStress[j][69]=dCombStrs[j][18];
			DataStrb.dblStress[j][71]=dCombStrs[j][19];
			DataStrb.dblStress[j][73]=dCombStrs[j][20];
			DataStrb.dblStress[j][75]=dCombStrs[j][21];
			DataStrb.dblStress[j][77]=dCombStrs[j][22];
			DataStrb.dblStress[j][79]=dCombStrs[j][23];
		}
		DataStrb.calcMaximumCombinedStress();
	}
	void ConvertIn(T_STRB_D& DataStrb)
	{
		for(int j=0; j<4; j++)
		{
			for(int k=0; k<18; k++)dblStress[j][k]    = DataStrb.dblForce[j][k];
			for(int l=0; l<14; l++)dblStress[j][l+18] = DataStrb.dblStress[j][l];
			dCombStrs[j][0]=DataStrb.dblStress[j][16];
			dCombStrs[j][1]=DataStrb.dblStress[j][18];
			dCombStrs[j][2]=DataStrb.dblStress[j][20];
			dCombStrs[j][3]=DataStrb.dblStress[j][22];
			dCombStrs[j][4]=DataStrb.dblStress[j][17];
			dCombStrs[j][5]=DataStrb.dblStress[j][19];
			dCombStrs[j][6]=DataStrb.dblStress[j][21];
			dCombStrs[j][7]=DataStrb.dblStress[j][23];
			for(int l=14; l<54; l++)dblStress[j][l+18] = DataStrb.dblStress[j][l+10];
			dCombStrs[j][8]=DataStrb.dblStress[j][64];
			dCombStrs[j][9]=DataStrb.dblStress[j][66];
			dCombStrs[j][10]=DataStrb.dblStress[j][68];
			dCombStrs[j][11]=DataStrb.dblStress[j][70];
			dCombStrs[j][12]=DataStrb.dblStress[j][72];
			dCombStrs[j][13]=DataStrb.dblStress[j][74];
			dCombStrs[j][14]=DataStrb.dblStress[j][76];
			dCombStrs[j][15]=DataStrb.dblStress[j][78];
			dCombStrs[j][16]=DataStrb.dblStress[j][65];
			dCombStrs[j][17]=DataStrb.dblStress[j][67];
			dCombStrs[j][18]=DataStrb.dblStress[j][69];
			dCombStrs[j][19]=DataStrb.dblStress[j][71];
			dCombStrs[j][20]=DataStrb.dblStress[j][73];
			dCombStrs[j][21]=DataStrb.dblStress[j][75];
			dCombStrs[j][22]=DataStrb.dblStress[j][77];
			dCombStrs[j][23]=DataStrb.dblStress[j][79];
		}
		//DataStrb.calcMaximumCombinedStress();
	}
	static void ConvertToSbcf(T_STRB_D& DataStrb, T_SBCF_D& DataSbcf)
	{
		DataSbcf.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				for(int k=0; k<6; k++) DataSbcf.dblForce[i][j][0][k]=DataStrb.dblForce[i][j*6+k];
				for(int k=6; k<9; k++) DataSbcf.dblForce[i][j][0][k]=DataStrb.dblForce[i][k*2+j];

				for(int k=1; k<9; k++)memcpy(DataSbcf.dblForce[i][j][k], DataSbcf.dblForce[i][j][0], sizeof(DataSbcf.dblForce[i][j][0]));
			}
		}
	}
	void ConvertToSbcf(T_SBCF_D& DataSbcf)
	{
		DataSbcf.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				for(int k=0; k<6; k++) DataSbcf.dblForce[i][j][0][k]=dblStress[i][j*6+k];
				for(int k=6; k<9; k++) DataSbcf.dblForce[i][j][0][k]=dblStress[i][k*2+j];
				
				for(int k=1; k<9; k++)memcpy(DataSbcf.dblForce[i][j][k], DataSbcf.dblForce[i][j][0], sizeof(DataSbcf.dblForce[i][j][0]));
			}
		}
	}
	void ConvertToSbcs(T_SBCS_D& DataSbcs)
	{
		DataSbcs.Initialize();
		for(int m=0; m<4; m++)       // CB1~CB4
		{
			for(int i=0; i<4; i++)     // 4Part
			{
				for(int j=0; j<2; j++)   // i-end,j-end
				{
					for(int k=0; k<6; k++) DataSbcs.dblForce[m][i][j][k]  =dblStress[i][j*6+k];
					for(int k=6; k<9; k++) DataSbcs.dblForce[m][i][j][k+1]=dblStress[i][k*2+j];

					DataSbcs.dblForce[m][i][j][6]=dCombStrs[i][j*4+m];
				}
			}
		}
	}	
	void Add(_STBW_STLD_BN& Data)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<72; j++)dblStress[i][j]+=Data.dblStress[i][j];
			for(int j=0; j<24; j++)dCombStrs[i][j]+=Data.dCombStrs[i][j];
		}
	}
};

// 가상보 결과
struct _SIMB_STLD_BN
{
	// dblStress    : local force, 1 part, 
	//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj,
	double dblStress[12];
	void ConvertOut(T_SIMB_D& DataSimb)
	{
		DataSimb.Initialize();
		for(int k=0; k<12; k++)DataSimb.dblForce[k]=dblStress[k];
	}
	void ConvertIn(T_SIMB_D& DataSimb)
	{
		for(int k=0; k<12; k++)dblStress[k]    = DataSimb.dblForce[k];
	}
	static void ConvertToSimc(T_SIMB_D& DataSimb, T_SIMC_D& DataSimc)
	{
		DataSimc.Initialize();
		for(int j=0; j<2; j++)
		{
			for(int k=0; k<6; k++)DataSimc.dblForce[j][0][k]=DataSimb.dblForce[j*6+k];
			for(int k=1; k<6; k++)memcpy(DataSimc.dblForce[j][k], DataSimc.dblForce[j][0], sizeof(DataSimc.dblForce[j][0]));
		}
	}
	void ConvertToSimc(T_SIMC_D& DataSimc)
	{
		DataSimc.Initialize();
		for(int j=0; j<2; j++)
		{
			for(int k=0; k<6; k++)DataSimc.dblForce[j][0][k]=dblStress[j*6+k];
			for(int k=1; k<6; k++)memcpy(DataSimc.dblForce[j][k], DataSimc.dblForce[j][0], sizeof(DataSimc.dblForce[j][0]));
		}
	}
	void Add(_SIMB_STLD_BN& Data)
	{
		for(int j=0; j<12; j++)dblStress[j]+=Data.dblStress[j];
	}
};

// 가상보 결과 - STRB형식으로
struct _FSIM_STLD_BN
{
	// dblStress    :  force, 4 part, 
	//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj,
	
	double dblStress[4][12];
	double dCombStrs[4][2][4];
	void ConvertOut(T_FSIM_D& DataFsim)
	{
		DataFsim.Initialize();
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 12; k++)DataFsim.dblForce[j][k] = dblStress[j][k];
			DataFsim.dblStress[j][2] = dCombStrs[j][0][0]; // i
			DataFsim.dblStress[j][4] = dCombStrs[j][0][1];
			DataFsim.dblStress[j][6] = dCombStrs[j][0][2];
			DataFsim.dblStress[j][8] = dCombStrs[j][0][3];

			DataFsim.dblStress[j][3] = dCombStrs[j][1][0]; // j
			DataFsim.dblStress[j][5] = dCombStrs[j][1][1];
			DataFsim.dblStress[j][7] = dCombStrs[j][1][2];
			DataFsim.dblStress[j][9] = dCombStrs[j][1][3];
		}
		DataFsim.calcMaximumCombinedStress();
	}

	void Add(_FSIM_STLD_BN& Data)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 12; j++)dblStress[i][j] += Data.dblStress[i][j];
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 4; k++)dCombStrs[i][j][k] += Data.dCombStrs[i][j][k];
			}
		}
	}
};

struct _SRIM_STLD_BN
{
	double dblStress[SRIM_SIZE]; // I:18개 단면지점,J:18개 단면지점
	void ConvertOut(T_SRIM_D& Data)
	{
		Data.Initialize();
		for(int i=0; i< SRIM_SIZE; i++)
			Data.dblStress[i] = dblStress[i];
	}
};

#define _PTSB_STLD_BN _SIMB_STLD_BN // Design Strip Beam Force
#define _PTPF_STLD_BN _SIMB_STLD_BN // Design Strip Tendon Primary Force

//합성단면 파트별 결과(정적해석)
struct _STCM_STLD_BN
{
	double dblForce[4][2][3];   // Fx, My, Mz
	double dblStress[4][2][5];  // Sx, Byp, Bym, Bzp, Bzm
	double dCombStrs[4][2][4];
	static void ConvertToSbcf(T_STRB_D& DataStrb, T_SBCF_D& DataSbcf)
	{
		DataSbcf.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				for(int k=0; k<6; k++)DataSbcf.dblForce[i][j][0][k]=DataStrb.dblForce[i][j*6+k];
				for(int k=1; k<6; k++)memcpy(DataSbcf.dblForce[i][j][k], DataSbcf.dblForce[i][j][0], sizeof(DataSbcf.dblForce[i][j][0]));
			}
		}
	}
	void ConvertToSbcf(T_SBCF_D& DataSbcf)
	{
		DataSbcf.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				DataSbcf.dblForce[i][j][0][0]=dblForce[i][j][0];
				DataSbcf.dblForce[i][j][0][4]=dblForce[i][j][1];
				DataSbcf.dblForce[i][j][0][5]=dblForce[i][j][2];
				for(int k=1; k<6; k++)memcpy(DataSbcf.dblForce[i][j][k], DataSbcf.dblForce[i][j][0], sizeof(DataSbcf.dblForce[i][j][0]));
			}
		}
	}
	void ConvertOut(T_STRB_D& Data)
	{
		Data.Initialize();
		for(int j=0; j<4; j++)
		{
			Data.dblForce[j][0  ]=dblForce[j][0][0];
			Data.dblForce[j][4  ]=dblForce[j][0][1];
			Data.dblForce[j][5  ]=dblForce[j][0][2];
			Data.dblForce[j][0+6]=dblForce[j][1][0];
			Data.dblForce[j][4+6]=dblForce[j][1][1];
			Data.dblForce[j][5+6]=dblForce[j][1][2];
			Data.dblStress[j][0  ]=dblStress[j][0][0];
			Data.dblStress[j][3  ]=dblStress[j][0][1];
			Data.dblStress[j][4  ]=dblStress[j][0][2];
			Data.dblStress[j][5  ]=dblStress[j][0][3];
			Data.dblStress[j][6  ]=dblStress[j][0][4];
			Data.dblStress[j][0+7]=dblStress[j][1][0];
			Data.dblStress[j][3+7]=dblStress[j][1][1];
			Data.dblStress[j][4+7]=dblStress[j][1][2];
			Data.dblStress[j][5+7]=dblStress[j][1][3];
			Data.dblStress[j][6+7]=dblStress[j][1][4];
			Data.dblStress[j][16]=dCombStrs[j][0][0];
			Data.dblStress[j][18]=dCombStrs[j][0][1];
			Data.dblStress[j][20]=dCombStrs[j][0][2];
			Data.dblStress[j][22]=dCombStrs[j][0][3];
			Data.dblStress[j][17]=dCombStrs[j][1][0];
			Data.dblStress[j][19]=dCombStrs[j][1][1];
			Data.dblStress[j][21]=dCombStrs[j][1][2];
			Data.dblStress[j][23]=dCombStrs[j][1][3];
		}
		Data.calcMaximumCombinedStress();
	}
	void Add(_STCM_STLD_BN& Data)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				for(int k=0; k<3; k++)dblForce[i][j][k]+=Data.dblForce[i][j][k];
				for(int k=0; k<5; k++)dblStress[i][j][k]+=Data.dblStress[i][j][k];
				for(int k=0; k<4; k++)dCombStrs[i][j][k]+=Data.dCombStrs[i][j][k];
			}
		}
	}
};

struct _STCM_STLD_BN_WP
{
	double dblForce[4][2][3];   // Fx, My, Mz
	double dblStress[4][2][5];  // Sx, Byp, Bym, Bzp, Bzm
	double dCombStrs[4][2][4];
	double dWStress[4][2][4];   // warping stress1~4
	static void ConvertToSbcf(T_STRB_D& DataStrb, T_SBCF_D& DataSbcf)
	{
		DataSbcf.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				for(int k=0; k<6; k++)DataSbcf.dblForce[i][j][0][k]=DataStrb.dblForce[i][j*6+k];
				for(int k=1; k<6; k++)memcpy(DataSbcf.dblForce[i][j][k], DataSbcf.dblForce[i][j][0], sizeof(DataSbcf.dblForce[i][j][0]));
			}
		}
	}
	void ConvertToSbcf(T_SBCF_D& DataSbcf)
	{
		DataSbcf.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				DataSbcf.dblForce[i][j][0][0]=dblForce[i][j][0];
				DataSbcf.dblForce[i][j][0][4]=dblForce[i][j][1];
				DataSbcf.dblForce[i][j][0][5]=dblForce[i][j][2];
				for(int k=1; k<6; k++)memcpy(DataSbcf.dblForce[i][j][k], DataSbcf.dblForce[i][j][0], sizeof(DataSbcf.dblForce[i][j][0]));
			}
		}
	}
	void ConvertOut(T_STRB_D& Data)
	{
		Data.Initialize();
		for(int j=0; j<4; j++)
		{
			Data.dblForce[j][0  ]=dblForce[j][0][0];
			Data.dblForce[j][4  ]=dblForce[j][0][1];
			Data.dblForce[j][5  ]=dblForce[j][0][2];
			Data.dblForce[j][0+6]=dblForce[j][1][0];
			Data.dblForce[j][4+6]=dblForce[j][1][1];
			Data.dblForce[j][5+6]=dblForce[j][1][2];
			Data.dblStress[j][0  ]=dblStress[j][0][0];
			Data.dblStress[j][3  ]=dblStress[j][0][1];
			Data.dblStress[j][4  ]=dblStress[j][0][2];
			Data.dblStress[j][5  ]=dblStress[j][0][3];
			Data.dblStress[j][6  ]=dblStress[j][0][4];
			Data.dblStress[j][0+7]=dblStress[j][1][0];
			Data.dblStress[j][3+7]=dblStress[j][1][1];
			Data.dblStress[j][4+7]=dblStress[j][1][2];
			Data.dblStress[j][5+7]=dblStress[j][1][3];
			Data.dblStress[j][6+7]=dblStress[j][1][4];
			Data.dblStress[j][16]=dCombStrs[j][0][0];
			Data.dblStress[j][18]=dCombStrs[j][0][1];
			Data.dblStress[j][20]=dCombStrs[j][0][2];
			Data.dblStress[j][22]=dCombStrs[j][0][3];
			Data.dblStress[j][17]=dCombStrs[j][1][0];
			Data.dblStress[j][19]=dCombStrs[j][1][1];
			Data.dblStress[j][21]=dCombStrs[j][1][2];
			Data.dblStress[j][23]=dCombStrs[j][1][3];

			Data.dblStress[j][24]=dWStress[j][0][0];
			Data.dblStress[j][25]=dWStress[j][0][1];
			Data.dblStress[j][26]=dWStress[j][0][2];
			Data.dblStress[j][27]=dWStress[j][0][3];
			Data.dblStress[j][44]=dWStress[j][1][0];
			Data.dblStress[j][45]=dWStress[j][1][1];
			Data.dblStress[j][46]=dWStress[j][1][2];
			Data.dblStress[j][47]=dWStress[j][1][3];
		}
		Data.calcMaximumCombinedStress();
	}
	void Add(_STCM_STLD_BN_WP& Data)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				for(int k=0; k<3; k++)dblForce[i][j][k]+=Data.dblForce[i][j][k];
				for(int k=0; k<5; k++)dblStress[i][j][k]+=Data.dblStress[i][j][k];
				for(int k=0; k<4; k++)dCombStrs[i][j][k]+=Data.dCombStrs[i][j][k];
				for(int k=0; k<4; k++)dWStress[i][j][k]+=Data.dWStress[i][j][k];
			}
		}
	}
};

struct _SCFP_STLD_BN
{
	// dbForce	: local force         , 5 part,
	//          component : Axial, Moment-y, Moment-z (3개)
	// dbStress: local stress        , 5 part, 
	//          component : Axial, Moment-y top, Moment-y Bot, Moment-z top, Moment-z Bot, Center, Stress Point 1, Stress Point 2, Stress Point 3, Stress Point 4 (10개) 
	// dbLStress: local stress        , 5 part, 
	//          component : Axial, Moment-y 응력 9개, Moment-z 응력 9개 (19개)

	double dbForce[5][3];
	double dbStress[5][10];
	double dbLStress[5][19];

	void ConvertToScfp(T_SCFP_D& DataScfp)
	{
		DataScfp.Initialize();

		for(int i=0; i<5; i++)
		{
			for(int j=0; j<3; j++) DataScfp.dbForce[i][j] = dbForce[i][j];

			for(int k=0; k<10; k++)
			{
				if(k<5) DataScfp.dbStress[i][k]   = dbStress[i][k];
				else    DataScfp.dbStress[i][k+1] = dbStress[i][k];			
			}

			for(int l=0; l<19; l++) DataScfp.dbLStress[i][l] = dbLStress[i][l];
		}
	}
};

// plane stress
struct _STRM_STLD_BN
{
// dblForce     : local force                 , 4 node       ,
//                component : FX, FY, FZ
// dblStress    : local stress                , cent + 4 node, 
//                component : SXX, SYY, SXY, P1, P2, ANG, EFF
	double dblForce[4][3];
	double dblStress[5][7];
	int iel;
	int ino[4];
	void Add(_STRM_STLD_BN& Data)
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<3; j++)dblForce[i][j]+=Data.dblForce[i][j];
		for(int i = 0; i<5; i++)
			for(int j=0; j<3; j++)dblStress[i][j]+=Data.dblStress[i][j];
	}
	void ConvertOut(T_STRM_DL& Data)
	{
		Data.Initialize();
		for(int j=0; j<4; j++)
		{
			for(int k=0; k<3; k++)Data.dblForce[j][k]=dblForce[j][k];
		}
		for(int j=0; j<5; j++)
		{
			for(int l=0; l<3; l++)Data.dblStress[j][l]=dblStress[j][l];
		}
	}
	void ConvertIn(T_STRM_DL& Data)
	{
		for(int j=0; j<4; j++)for(int k=0; k<3; k++)dblForce[j][k]=Data.dblForce[j][k];
		for(int j=0; j<5; j++)for(int l=0; l<3; l++)    dblStress[j][l]=Data.dblStress[j][l];
	}
};

// plate
struct _STRP_STLD_BN
{
// dblForce     : local force                 , 4 node       ,
//                component : FX, FY, FZ, MX, MY, MZ
// dblStressIOS : local stress(membrane, bending, shear), cent + 4 node, 
//                component : SMX, SMY, SMZ, SBX, SBY, SBZ, VX, VY
// dblStress    : local stress + unit force   , cent + 4 node, 
//                component : SXXt, SYYt, SXYt, SXXb, SYYb, SXYb, FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

	double dblForce[4][6];
	double dblStressIOS[5][8];
	double dblStress[5][14];
	int iel;
	int ino[4];
	void Add(_STRP_STLD_BN& Data)
	{
		for(int i=0; i<4; i++)
			for(int j=0; j<6; j++)dblForce[i][j]+=Data.dblForce[i][j];
		for(int i = 0; i<5; i++)
			for(int j=0; j<8; j++)dblStressIOS[i][j]+=Data.dblStressIOS[i][j];
		for(int i = 0; i<5; i++)
			for(int j=0; j<14; j++)dblStress[i][j]+=Data.dblStress[i][j];
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
		for(int j=0; j<4; j++)for(int k=0; k<6; k++)dblForce[j][k]=Data.dblForce[j][k];
		for(int j=0; j<5; j++)
		{
			dblStress[j][ 0]=Data.dblStress[j][ 0];
			dblStress[j][ 1]=Data.dblStress[j][ 1];
			dblStress[j][ 2]=Data.dblStress[j][ 2];
			dblStress[j][ 3]=Data.dblStress[j][ 7];
			dblStress[j][ 4]=Data.dblStress[j][ 8];
			dblStress[j][ 5]=Data.dblStress[j][ 9];
			dblStress[j][ 6]=Data.dblStress[j][14];
			dblStress[j][ 7]=Data.dblStress[j][15];
			dblStress[j][ 8]=Data.dblStress[j][16];
			dblStress[j][ 9]=Data.dblStress[j][20];
			dblStress[j][10]=Data.dblStress[j][21];
			dblStress[j][11]=Data.dblStress[j][22];
			dblStress[j][12]=Data.dblStress[j][26];
			dblStress[j][13]=Data.dblStress[j][27];
		}
	}
	void ConvertToSpcf(T_SPCF_DL& Data)
	{
		Data.Initialize();

		for(int i=0; i<5; i++)
		{
			for(int j=0; j<8; j++)
			{
				Data.dForce[i][j][0] = dblStress[i][6];
				Data.dForce[i][j][1] = dblStress[i][7];
				Data.dForce[i][j][2] = dblStress[i][8];
				Data.dForce[i][j][3] = dblStress[i][9];
				Data.dForce[i][j][4] = dblStress[i][10];
				Data.dForce[i][j][5] = dblStress[i][11];
				Data.dForce[i][j][6] = dblStress[i][12];
				Data.dForce[i][j][7] = dblStress[i][13];
			}
		}
	}
};

// solid
struct _STRS_STLD_BN
{
// dblForce  : global force , 8 node       , component : FX, FY, FZ
// dblStress : global stress, cent + 8 node, component : SXX, SYY, SZZ, SXY, SYZ, SXZ
	double dblForce[8][3];
	double dblStress[1+8][6];
	int iel;
	int ino[8];
	void Initialize()
	{
		memset(this, 0, sizeof(_STRS_STLD_BN));
	}
	void Add(_STRS_STLD_BN& Data)
	{
		for(int i=0; i<8; i++)
			for(int j=0; j<3; j++)dblForce[i][j]+=Data.dblForce[i][j];
		for(int i = 0; i<9; i++)
			for(int j=0; j<6; j++)dblStress[i][j]+=Data.dblStress[i][j];
	}
	void ConvertOut(T_STRS_D& Data)
	{
		Data.Initialize();
		for(int j=0; j<8; j++)
		{
			for(int k=0; k<3; k++)Data.dblForce[j][k]=dblForce[j][k];
		}
		for(int j=0; j<9; j++)
		{
			Data.dblStress[j][ 0]=dblStress[j][ 0];
			Data.dblStress[j][ 1]=dblStress[j][ 1];
			Data.dblStress[j][ 2]=dblStress[j][ 2];
			Data.dblStress[j][ 3]=dblStress[j][ 3];
			Data.dblStress[j][ 4]=dblStress[j][ 4];
			Data.dblStress[j][ 5]=dblStress[j][ 5];
		}
	}
	void ConvertIn(T_STRS_D& Data)
	{
		for(int j=0; j<8; j++)for(int k=0; k<3; k++)dblForce[j][k]=Data.dblForce[j][k];
		for(int j=0; j<9; j++)
		{
			dblStress[j][ 0]=Data.dblStress[j][ 0];
			dblStress[j][ 1]=Data.dblStress[j][ 1];
			dblStress[j][ 2]=Data.dblStress[j][ 2];
			dblStress[j][ 3]=Data.dblStress[j][ 3];
			dblStress[j][ 4]=Data.dblStress[j][ 4];
			dblStress[j][ 5]=Data.dblStress[j][ 5];
		}
	}
};

// wall
struct _STRW_STLD_BN
{
// dblForce     : local force         , 2 part, top, bottom
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
	double dblForce[2][6];
	int iel;
	void Add(_STRW_STLD_BN& Data)
	{
		for(int i=0; i<2; i++)
			for(int j=0; j<6; j++)dblForce[i][j]+=Data.dblForce[i][j];
	}
	void ConvertOut(T_STRW_D& Data)
	{
		Data.Initialize();
		for(int j=0; j<2; j++)
		{
			for(int k=0; k<6; k++)Data.dblForce[j][k]=dblForce[j][k];
		}
	}
};

// Plane Strain
struct _STPN_STLD_BN
{
// dblForce  : global force , 4 node       , component : FX, FY, FZ.
// dblStress : global stress, cent + 4 node, component : SXX, SZZ, SYY=0., SXZ
	double dblForce[4][3];
	double dblStress[1+4][4];
	int iel;
	int ino[4];
	void ConvertOut(T_STPN_D& DataStpn)
	{
		DataStpn.Initialize();
		for(int j=0; j<4; j++)
		{
			DataStpn.dblForce[j][0]=dblForce[j][0];
			DataStpn.dblForce[j][1]=dblForce[j][1];
			DataStpn.dblForce[j][2]=dblForce[j][2];
		}
		for(int j=0; j<5; j++)
		{
			DataStpn.dblStress[j][ 0]=dblStress[j][ 0];
			DataStpn.dblStress[j][ 1]=dblStress[j][ 2];
			DataStpn.dblStress[j][ 2]=dblStress[j][ 1];
			DataStpn.dblStress[j][ 3]=0.;
			DataStpn.dblStress[j][ 4]=0.;
			DataStpn.dblStress[j][ 5]=dblStress[j][ 3];
		}
	}
	void ConvertIn(T_STPN_D& DataStpn)
	{
		for(int j=0; j<4; j++)
		{
			dblForce[j][0]=DataStpn.dblForce[j][0];
			dblForce[j][1]=DataStpn.dblForce[j][1];
			dblForce[j][2]=DataStpn.dblForce[j][2];
		}
		for(int j=0; j<5; j++)
		{
			dblStress[j][ 0]=DataStpn.dblStress[j][ 0];
			dblStress[j][ 2]=DataStpn.dblStress[j][ 1];
			dblStress[j][ 1]=DataStpn.dblStress[j][ 2];
			//DataStpn.dblStress[j][ 3]=0.;
			//DataStpn.dblStress[j][ 4]=0.;
			dblStress[j][ 3]=DataStpn.dblStress[j][ 5];
		}
	}
};

// Axisymmetric
struct _STAX_STLD_BN
{
// dblForce  : global force , 4 node       , component : FX, FY, FZ.
// dblStress : global stress, cent + 4 node, component : SXX, SZZ, SYY=0., SXZ
	double dblForce[4][3];
	double dblStress[1+4][4];
	int iel;
	int ino[4];
	void Initialize()
	{
		memset(this, 0, sizeof(_STAX_STLD_BN));
	}
	void ConvertOut(T_STAX_D& DataStax)
	{
		DataStax.Initialize();
		for(int j=0; j<4; j++)
		{
			DataStax.dblForce[j][0]=dblForce[j][0];
			DataStax.dblForce[j][1]=dblForce[j][1];
			DataStax.dblForce[j][2]=dblForce[j][2];
		}
		for(int j=0; j<5; j++)
		{
			DataStax.dblStress[j][ 0]=dblStress[j][ 0];
			DataStax.dblStress[j][ 1]=dblStress[j][ 2];
			DataStax.dblStress[j][ 2]=dblStress[j][ 1];
			DataStax.dblStress[j][ 3]=0.;
			DataStax.dblStress[j][ 4]=0.;
			DataStax.dblStress[j][ 5]=dblStress[j][ 3];
		}
	}
};

// beam load
struct _CFML_STLD_BN
{
	T_ELEM_K ElemBeamSerial;
	T_STLD_K LoadCaseKeySerial;
	int LoadType;         // 1=집중하중, 2=집중모멘트, 3=분포하중, 4=분포모멘트
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ
	double x1, x2;  // 0.에서 1. 사이의 값
	double p1, p2;  // Load
};

// offset
#define _CFMO_STLD_BN T_CFMO_D

struct _SSRT_STLD_BN
{
	double nType; // 1=Frame 2=벽체 3=스프링 4=ElaticLink 5=Frame Sum(Beam+Truss) 6=Wall Sum 7=Planar 8=Solid 9=Planar Sum 10=Solid Sun 11=Truss 13=Brace Type Sum
	double nId;   // 요소번호, 벽체번호, 스프링 순번, 링크 순번
	double dShearX;
	double dShearY;
};

struct _STRD_STLD_BN
{
	double nStor_cur; // 현재층 serial 번호
	double nStor_up;  // 바로 위층 serial 번호
	double dShearX;   // Shear Force (X)
	double dShearY;   // Shear Force (Y)
	double dShearX_beam;   // Shear Force (X) - sum of beam
	double dShearY_beam;   // Shear Force (Y) - sum of beam
	double dShearX_truss;   // Shear Force (X) - sum of truss
	double dShearY_truss;   // Shear Force (Y) - sum of truss
	double dShearX_wall;   // Shear Force (X) - sum of wall
	double dShearY_wall;   // Shear Force (Y) - sum of wall
	double dShearX_global;   // Shear Force (X) - about global axis
	double dShearY_global;   // Shear Force (Y) - about global axis
};

struct _RCBL_STLD_BN
{
	double dForce[2][4]; //[i, j] [Tension, FX, FY, FZ]
	double dTotalLength;
	double dElongation;
	double dUnstrainedLength;
	double dSag;
	double dHDistance, dVDistance;
	double dGradient;
	double dSkewAngle[2]; //i, j

	void ConvertOut(T_RCBL_D& Data)
	{
		Data.Initialize();

		for(int i=0; i<2; i++)
		{
			Data.dTension[i] = dForce[i][0];
			for(int j=0; j<3; j++) Data.dForce[i][j] = dForce[i][j+1];
			Data.dSkewAngle[i]=dSkewAngle[i];
		}
		Data.dTotalLength = dTotalLength;
		Data.dElongation = dElongation;
		Data.dUnstrainedLength = dUnstrainedLength;
		Data.dSag = dSag;
		Data.dHDistance = dHDistance; 
		Data.dVDistance = dVDistance;
		Data.dGradient = dGradient;
	}
	void ConvertIn(T_RCBL_D& Data)
	{
		for(int i=0; i<2; i++)
		{
			dForce[i][0] = Data.dTension[i];
			for(int j=0; j<3; j++) dForce[i][j+1] = Data.dForce[i][j];
			dSkewAngle[i] = Data.dSkewAngle[i];
		}
		dTotalLength = Data.dTotalLength;
		dElongation = Data.dElongation;
		dUnstrainedLength = Data.dUnstrainedLength;
		dSag = Data.dSag;
		dHDistance = Data.dHDistance; 
		dVDistance = Data.dVDistance;
		dGradient = Data.dGradient;
	}
};

struct _RCAE_STLD_BN
{
	double dElem;
	double dChordLength;
	double dEA;
	double dWeight;
	double dTension;
	double dEAMod;
	double dEfficiency;

	void ConvertOut(T_RCAE_D& Data)
	{
		Data.Initialize();

		Data.ElemK = (UINT)dElem;
		Data.dChordLength = dChordLength;
		Data.dEA = dEA;
		Data.dWeight = dWeight;
		Data.dTension = dTension; 
		Data.dEAMod = dEAMod;
		Data.dEfficiency = dEfficiency;
	}
	void ConvertIn(T_RCAE_D& Data)
	{
		dElem = Data.ElemK;
		dChordLength = Data.dChordLength;
		dEA = Data.dEA;
		dWeight = Data.dWeight;
		dTension = Data.dTension; 
		dEAMod = Data.dEAMod;
		dEfficiency = Data.dEfficiency;
	}
};

struct _BTSS_STLD_BN_OLD
{
	double dElem;
	double dLcom;
	double dblStress[5][10]; // [5지점][4개 합성응력 + 6개 전단응력 계산위치의 응력]
	double dblLineStress[5][18]; // [5지점][Local-y 9개 Line + Local-z 9개 Line]

	void ConvertOut(T_BTSS_D& Data)
	{
		Data.Initialize();
		ASSERT(sizeof(dblStress) == sizeof(Data.dblStress));
		ASSERT(sizeof(dblLineStress) == sizeof(Data.dblLineStress));
		memcpy(Data.dblStressMz, dblStress, sizeof(Data.dblStress));
		memcpy(Data.dblStress,   dblStress, sizeof(Data.dblStress));
		memcpy(Data.dblLineStress, dblLineStress, sizeof(Data.dblLineStress));
	}
};

struct _BTSS_STLD_BN_691
{
	double dElem;
	double dLcom;
	double dblStress[5][10]; // [5지점][4개 합성응력 + 6개 전단응력 계산위치의 응력]
	double dblLineStress[5][18]; // [5지점][Local-y 9개 Line + Local-z 9개 Line]

	double dN;  //Beam Section Temperature에의한 축력
	double dMy; //Beam Section Temperature에의한 모멘트
	double dMz; //Beam Section Temperature에의한 모멘트
	void ConvertOut(T_BTSS_D& Data)
	{
		Data.Initialize();
		ASSERT(sizeof(dblStress) == sizeof(Data.dblStress));
		ASSERT(sizeof(dblLineStress) == sizeof(Data.dblLineStress));
		memcpy(Data.dblStressMz, dblStress, sizeof(Data.dblStress));
		memcpy(Data.dblStress,   dblStress, sizeof(Data.dblStress));
		memcpy(Data.dblLineStress, dblLineStress, sizeof(Data.dblLineStress));
		Data.dN=dN;
		Data.dMy=dMy;
		Data.dMz=dMz;
	}
};

struct _BTSS_STLD_BN_845
{
	double dElem;
	double dLcom;
	double dblStressMz[5][10]; // [5지점][4개 합성응력 + 6개 전단응력 계산위치의 응력]   V692에서 추가 
	double dblStressMy[5][10]; // [5지점][4개 합성응력 + 6개 전단응력 계산위치의 응력] 
	double dblLineStress[5][18]; // [5지점][Local-y 9개 Line + Local-z 9개 Line]

	double dN;  //Beam Section Temperature에의한 축력
	double dMy; //Beam Section Temperature에의한 모멘트
	double dMz; //Beam Section Temperature에의한 모멘트
	void ConvertOut(T_BTSS_D& Data)
	{
		Data.Initialize();
		
		memset(Data.dblStressMz, 0, sizeof(Data.dblStressMz));
		memset(Data.dblStress, 0, sizeof(Data.dblStress));
		memset(Data.dblLineStress, 0, sizeof(Data.dblLineStress));

		for(int i=0; i<5; i++)
		{  for(int j=0; j<10; j++)
			 {
				 Data.dblStressMz[i][j] = dblStressMz[i][j];
				 Data.dblStress[i][j] = dblStressMy[i][j];
			 }
		}
		memcpy(Data.dblLineStress, dblLineStress, sizeof(dblLineStress));

		Data.dN=dN;
		Data.dMy=dMy;
		Data.dMz=dMz;
	}
};

struct _BTSS_STLD_BN
{
	double dElem;
	double dLcom;
	double dblStressMz[5][16]; // [5지점][4개 합성응력 + 6개 전단응력 계산위치의 응력 + 6개 포인트(7DOF)]   V692에서 추가 
	double dblStressMy[5][16]; // [5지점][4개 합성응력 + 6개 전단응력 계산위치의 응력 + 6개 포인트(7DOF)] 
	double dblLineStress[5][18]; // [5지점][Local-y 9개 Line + Local-z 9개 Line]

	double dN;  //Beam Section Temperature에의한 축력
	double dMy; //Beam Section Temperature에의한 모멘트
	double dMz; //Beam Section Temperature에의한 모멘트
	void Initialize()
	{
		memset(this, 0, sizeof(_BTSS_STLD_BN));
	}
	void ConvertOut(T_BTSS_D& Data)
	{
		Data.Initialize();
		ASSERT(sizeof(dblStressMz) == sizeof(Data.dblStress));
		ASSERT(sizeof(dblStressMy) == sizeof(Data.dblStress));
		ASSERT(sizeof(dblLineStress) == sizeof(Data.dblLineStress));
		memcpy(Data.dblStressMz, dblStressMz, sizeof(Data.dblStress));
		memcpy(Data.dblStress,   dblStressMy, sizeof(Data.dblStress));
		memcpy(Data.dblLineStress, dblLineStress, sizeof(Data.dblLineStress));
		Data.dN=dN;
		Data.dMy=dMy;
		Data.dMz=dMz;
	}
};

struct _BTPF_STLD_7DOF_BN
{
	double dElem;
	double dLcom;
	double dForce[4][18];
	
	void ConvertOut(T_BTPF_D& Data)
	{
		Data.Initialize();
		ASSERT(sizeof(dForce) == sizeof(Data.dForce));
		memcpy(Data.dForce, dForce, sizeof(Data.dForce));
	}
};

struct _BTPF_STLD_BN
{
	double dElem;
	double dLcom;
	double dForce[4][12];

	void ConvertOut(T_BTPF_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<12; j++)
			{
				Data.dForce[i][j] = dForce[i][j];
			}
		}
	}
};

// [2012-06-01] Kim, Geun Young (Tel: 2042, gykim@midasit.com)
// 사용안함.
struct _SSPS_STLD_BN
{
	double dForce[5];
	double dDisplacement[5];
	double dElement;
	double dDirection;
	double dElementType;     
	void Add(_SSPS_STLD_BN& Data)
	{
		for(int i=0; i<5; i++)
		{
			dForce[i]+=Data.dForce[i];
			dDisplacement[i]+=Data.dDisplacement[i];
		}
	}
	void ConvertOut(T_REAC_SSPS_D& Data)
	{
		Data.Initialize();
		for (int i=0; i<5; i++)
		{
			Data.dForce[i] = dForce[i];
			Data.dDisplacement[i] = dDisplacement[i];
		}
		Data.dElement     = dElement    ;
		Data.dDirection   = dDirection  ;
		Data.dElementType = dElementType;
	}
	void ConvertIn(T_REAC_SSPS_D& Data)
	{
		for (int i=0; i<5; i++)
		{
			dForce[i] = Data.dForce[i];
			dDisplacement[i] = Data.dDisplacement[i];
		}
		dElement     = Data.dElement    ;
		dDirection   = Data.dDirection  ;
		dElementType = Data.dElementType;
	}  
};

struct _GSLH_BN
{
	//Plate Total Strain 
	//Strain : Center + Node 4
	//         stn-xxt, stn-yyt, stn-xyt, stn-xxb, stn-yyb, stn-xyb 
	double dStrain[5][6];

//   void Add(_GSLH_BN& Data)
//   {
//     for(int i=0; i<5; i++)
//       for(int j=0; j<6; j++)dStrain[i][j]+=Data.dStrain[i][j];
//   }
};

struct _PSLH_BN
{
	//Plate Plastic Strain 
	//Strain : Center + Node 4
	//         stn-xxt, stn-yyt, stn-xyt, stn-xxb, stn-yyb, stn-xyb 
	double dStrain[5][6];
};

struct _PDID_BN
{
	//Plate Damage Ratio
	//Strain : Center + Node 4
	//         comp_t, tens_t, total_t, comp_b, tens_b, total_b
	double dUseDamageRatio;
	double dStrain[5][6];
};

struct _GSLS_BN
{
	//Solid Total Strain 
	//Strain : Center + Node 8
	//         stn-xx, stn-yy, stn-zz, stn-xy, stn-yz, stn-xz
	double dStrain[9][6];

//   void Add(_GSLS_BN& Data)
//   {
//     for(int i=0; i<9; i++)
//       for(int j=0; j<6; j++)dStrain[i][j]+=Data.dStrain[i][j];
//   }
};

struct _PSLS_BN
{
	//Solid Plastic Strain 
	//Strain : Center + Node 8
	//         stn-xx, stn-yy, stn-zz, stn-xy, stn-yz, stn-xz
	double dStrain[9][6];
};

struct _SDID_BN
{
	//Solid Damage Ratio
	//Strain : Center + Node 8
	//         comp, tens, total
	double dUseDamageRatio;
	double dStrain[9][3];
};
#pragma pack()

#endif