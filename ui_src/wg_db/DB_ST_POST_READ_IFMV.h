#ifndef __DB_ST_POST_READ_IFMV_H__
#define __DB_ST_POST_READ_IFMV_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)
struct _REAC_IFMV_FLOAT_BN
{
	float ino;
	float dblReac[6];
	void ConvertOut(T_REAC_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<6; i++)Data.dblReac[i]=dblReac[i];
	}
};

struct _LRAC_IFMV_FLOAT_BN
{
	float ino;
	float dblReac[6];
	void ConvertOut(T_REAC_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<6; i++)Data.dblReac[i]=dblReac[i];
	}
};
struct _DISP_IFMV_FLOAT_BN
{
	float dblDisp[6];
	void ConvertOut(T_DISP_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<6; i++)Data.dblDisp[i]=dblDisp[i];
	}
};

struct _STRT_IFMV_FLOAT_BN
{
// dblForce     : local force, 
//                component : Fxi, Fxj
	float iel;
	float dblForce[2];
	void ConvertOut(T_STRT_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<2; i++)Data.dblForce[i]=dblForce[i];
	}
};

struct _STRB_IFMV_FLOAT_BN
{
// dblForce     : local force         , 4 part, 
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
	float iel;
	float dblForce[4][12];
	float dblCombStrs[5][4];

	void ConvertOut(T_STRB_D& Data)
	{
		Data.Initialize();
		// force
		memcpy(&Data.dblForce[0][0],&dblForce[0][0],sizeof(double)*6);
		for(int i=0; i<3; i++)
		{
			Data.dblForce[i][ 6]=dblForce[i+0][ 6];
			Data.dblForce[i][ 7]=dblForce[i+0][ 8];
			Data.dblForce[i][ 8]=dblForce[i+0][10];
			Data.dblForce[i][ 9]=dblForce[i+1][ 0];
			Data.dblForce[i][10]=dblForce[i+1][ 2];
			Data.dblForce[i][11]=dblForce[i+1][ 4];
			Data.dblForce[i+1][ 0]=dblForce[i+0][ 7];
			Data.dblForce[i+1][ 1]=dblForce[i+0][ 9];
			Data.dblForce[i+1][ 2]=dblForce[i+0][11];
			Data.dblForce[i+1][ 3]=dblForce[i+1][ 1];
			Data.dblForce[i+1][ 4]=dblForce[i+1][ 3];
			Data.dblForce[i+1][ 5]=dblForce[i+1][ 5];
		}
		memcpy(&Data.dblForce[3][6],&dblForce[3][6],sizeof(double)*6);
		// combined stress
		for(int j=0; j<4; j++)
		{
			Data.dblStress[j][16]=dblCombStrs[j  ][0];
			Data.dblStress[j][17]=dblCombStrs[j+1][0];
			Data.dblStress[j][18]=dblCombStrs[j  ][1];
			Data.dblStress[j][19]=dblCombStrs[j+1][1];
			Data.dblStress[j][20]=dblCombStrs[j  ][2];
			Data.dblStress[j][21]=dblCombStrs[j+1][2];
			Data.dblStress[j][22]=dblCombStrs[j  ][3];
			Data.dblStress[j][23]=dblCombStrs[j+1][3];
		}
		Data.calcMaximumCombinedStress();
	}
};

struct _ELNK_IFMV_FLOAT_BN
{
// dblForce  : local force, 1 node       , component : fx, fy, fz, mx, my, mz
	float iel;
	float ino[2];
	float dblForce[2][6];
	void ConvertOut(T_STEK_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<6; j++)Data.dblForce[i][j]=dblForce[i][j];
		}
	}
};
#define _NLNK_IFMV_FLOAT_BN  _ELNK_IFMV_FLOAT_BN

struct _STRP1_IFMV_FLOAT_BN
{
// dblForce     : local unit length force     , cent + 4 node       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

	float iel;
	float ino[4];
	float dblForce[5][8];
	float dblStress[5][6];
	void ConvertOut(T_STRP_DL& Data)
	{
	}
};

struct _STRP2_IFMV_FLOAT_BN
{
// dblForce     : local unit length force     , cent       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

	float iel;
	float ino[4];
	float dblForce[1][8];
	float dblStress[1][6];
	void ConvertOut(T_STRP_DL& Data)
	{
	}
};

struct _STRP3_IFMV_FLOAT_BN
{
// dblForce     : local unit length force     , cent + 4 node       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

	float iel;
	float ino[4];
	float dblForce[5][8];
	void ConvertOut(T_STRP_DL& Data)
	{
	}
};

struct _STRP4_IFMV_FLOAT_BN
{
// dblForce     : local unit length force     , cent + 4 node       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

	float iel;
	float ino[4];
	float dblForce[1][8];
	void ConvertOut(T_STRP_DL& Data)
	{
	}
};

struct _STRS_IFMV_FLOAT_BN
{
	int iel;
	int ino[8];
	float dblForce[8][3];
	float dblStress[1+8][6];
	void ConvertOut(T_STRS_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<8; i++)
		{
			for(int j=0; j<3; j++)Data.dblForce[i][j]=dblForce[i][j];
		}
		for(int i = 0; i<9; i++)
		{
			for(int j=0; j<6; j++)Data.dblStress[i][j]=dblStress[i][j];
		}
	}
};

//합성단면 파트별 결과(이동하중결과)
struct _STCM_MVLD_BN
{
	double dblForceMax[5][3];   // Fx, My, Mz
	double dblForceMin[5][3];   // Fx, My, Mz
	double dblStressMax[5][5];  // Sx, Byp, Bym, Bzp, Bzm
	double dblStressMin[5][5];  // Sx, Byp, Bym, Bzp, Bzm
	double dblCombStrsMax[5][4];
	double dblCombStrsMin[5][4];
	void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				int m=i+j;
				int k=j*6;
				int kk=j*7;
				DataMax.dblForce[i][k+0]=dblForceMax[m][0];
				DataMax.dblForce[i][k+4]=dblForceMax[m][1];
				DataMax.dblForce[i][k+5]=dblForceMax[m][2];
				DataMin.dblForce[i][k+0]=dblForceMin[m][0];
				DataMin.dblForce[i][k+4]=dblForceMin[m][1];
				DataMin.dblForce[i][k+5]=dblForceMin[m][2];
				DataMax.dblStress[i][kk+0]=dblStressMax[m][0];
				DataMax.dblStress[i][kk+3]=dblStressMax[m][1];
				DataMax.dblStress[i][kk+4]=dblStressMax[m][2];
				DataMax.dblStress[i][kk+5]=dblStressMax[m][3];
				DataMax.dblStress[i][kk+6]=dblStressMax[m][4];
				DataMin.dblStress[i][kk+0]=dblStressMin[m][0];
				DataMin.dblStress[i][kk+3]=dblStressMin[m][1];
				DataMin.dblStress[i][kk+4]=dblStressMin[m][2];
				DataMin.dblStress[i][kk+5]=dblStressMin[m][3];
				DataMin.dblStress[i][kk+6]=dblStressMin[m][4];
				if(j == 0)
				{
					DataMax.dblStress[i][16]=dblCombStrsMax[m][0];
					DataMax.dblStress[i][18]=dblCombStrsMax[m][1];
					DataMax.dblStress[i][20]=dblCombStrsMax[m][2];
					DataMax.dblStress[i][22]=dblCombStrsMax[m][3];
					DataMin.dblStress[i][16]=dblCombStrsMin[m][0];
					DataMin.dblStress[i][18]=dblCombStrsMin[m][1];
					DataMin.dblStress[i][20]=dblCombStrsMin[m][2];
					DataMin.dblStress[i][22]=dblCombStrsMin[m][3];
				}
				else
				{
					DataMax.dblStress[i][17]=dblCombStrsMax[m][0];
					DataMax.dblStress[i][19]=dblCombStrsMax[m][1];
					DataMax.dblStress[i][21]=dblCombStrsMax[m][2];
					DataMax.dblStress[i][23]=dblCombStrsMax[m][3];
					DataMin.dblStress[i][17]=dblCombStrsMin[m][0];
					DataMin.dblStress[i][19]=dblCombStrsMin[m][1];
					DataMin.dblStress[i][21]=dblCombStrsMin[m][2];
					DataMin.dblStress[i][23]=dblCombStrsMin[m][3];
				}
			}
		}
		DataMax.calcMaximumCombinedStress();
		DataMin.calcMaximumCombinedStress();
	}
};
struct _STCM_MVLD_BN_WP
{
	double dblForceMax[5][3];   // Fx, My, Mz
	double dblForceMin[5][3];   // Fx, My, Mz
	double dblStressMax[5][5];  // Sx, Byp, Bym, Bzp, Bzm
	double dblStressMin[5][5];  // Sx, Byp, Bym, Bzp, Bzm
	double dblCombStrsMax[5][4];
	double dblCombStrsMin[5][4];
	double dWStrsMax[5][4];
	double dWStrsMin[5][4];
	void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				int m=i+j;
				int k=j*6;
				int kk=j*7;
				DataMax.dblForce[i][k+0]=dblForceMax[m][0];
				DataMax.dblForce[i][k+4]=dblForceMax[m][1];
				DataMax.dblForce[i][k+5]=dblForceMax[m][2];
				DataMin.dblForce[i][k+0]=dblForceMin[m][0];
				DataMin.dblForce[i][k+4]=dblForceMin[m][1];
				DataMin.dblForce[i][k+5]=dblForceMin[m][2];
				DataMax.dblStress[i][kk+0]=dblStressMax[m][0];
				DataMax.dblStress[i][kk+3]=dblStressMax[m][1];
				DataMax.dblStress[i][kk+4]=dblStressMax[m][2];
				DataMax.dblStress[i][kk+5]=dblStressMax[m][3];
				DataMax.dblStress[i][kk+6]=dblStressMax[m][4];
				DataMin.dblStress[i][kk+0]=dblStressMin[m][0];
				DataMin.dblStress[i][kk+3]=dblStressMin[m][1];
				DataMin.dblStress[i][kk+4]=dblStressMin[m][2];
				DataMin.dblStress[i][kk+5]=dblStressMin[m][3];
				DataMin.dblStress[i][kk+6]=dblStressMin[m][4];
				if(j == 0)
				{
					DataMax.dblStress[i][16]=dblCombStrsMax[m][0];
					DataMax.dblStress[i][18]=dblCombStrsMax[m][1];
					DataMax.dblStress[i][20]=dblCombStrsMax[m][2];
					DataMax.dblStress[i][22]=dblCombStrsMax[m][3];
					DataMin.dblStress[i][16]=dblCombStrsMin[m][0];
					DataMin.dblStress[i][18]=dblCombStrsMin[m][1];
					DataMin.dblStress[i][20]=dblCombStrsMin[m][2];
					DataMin.dblStress[i][22]=dblCombStrsMin[m][3];

					DataMax.dblStress[i][24]=dWStrsMax[m][0];
					DataMax.dblStress[i][25]=dWStrsMax[m][1];
					DataMax.dblStress[i][26]=dWStrsMax[m][2];
					DataMax.dblStress[i][27]=dWStrsMax[m][3];
					DataMin.dblStress[i][24]=dWStrsMin[m][0];
					DataMin.dblStress[i][25]=dWStrsMin[m][1];
					DataMin.dblStress[i][26]=dWStrsMin[m][2];
					DataMin.dblStress[i][27]=dWStrsMin[m][3];
				}
				else
				{
					DataMax.dblStress[i][17]=dblCombStrsMax[m][0];
					DataMax.dblStress[i][19]=dblCombStrsMax[m][1];
					DataMax.dblStress[i][21]=dblCombStrsMax[m][2];
					DataMax.dblStress[i][23]=dblCombStrsMax[m][3];
					DataMin.dblStress[i][17]=dblCombStrsMin[m][0];
					DataMin.dblStress[i][19]=dblCombStrsMin[m][1];
					DataMin.dblStress[i][21]=dblCombStrsMin[m][2];
					DataMin.dblStress[i][23]=dblCombStrsMin[m][3];

					DataMax.dblStress[i][44]=dWStrsMax[m][0];
					DataMax.dblStress[i][45]=dWStrsMax[m][1];
					DataMax.dblStress[i][46]=dWStrsMax[m][2];
					DataMax.dblStress[i][47]=dWStrsMax[m][3];
					DataMin.dblStress[i][44]=dWStrsMin[m][0];
					DataMin.dblStress[i][45]=dWStrsMin[m][1];
					DataMin.dblStress[i][46]=dWStrsMin[m][2];
					DataMin.dblStress[i][47]=dWStrsMin[m][3];
				}
			}
		}
		DataMax.calcMaximumCombinedStress();
		DataMin.calcMaximumCombinedStress();
	}
};
#pragma pack()

#endif