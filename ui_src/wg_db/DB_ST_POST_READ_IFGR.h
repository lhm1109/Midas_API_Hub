#ifndef __DB_ST_POST_READ_IFGR_H__
#define __DB_ST_POST_READ_IFGR_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)
struct _REAC_IFGR_FLOAT_BN
{
	float ino;
	float dblReac[6];
	void ConvertOut(T_REAC_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<6; i++)Data.dblReac[i]=dblReac[i];
	}
};
#define _LRAC_IFGR_FLOAT_BN _REAC_IFGR_FLOAT_BN

struct _DISP_IFGR_FLOAT_BN
{
	float dblDisp[6];
	void ConvertOut(T_DISP_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<6; i++)Data.dblDisp[i]=dblDisp[i];
	}
};
struct _DISP7_IFGR_FLOAT_BN
{
	float dblDisp[DISP_SIZE];
	void ConvertOut(T_DISP_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<DISP_SIZE; i++)Data.dblDisp[i]=dblDisp[i];
	}
};


struct _STRT_IFGR_FLOAT_BN
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

struct _STRB_IFGR_FLOAT_BN
{
// dblForce     : local force         , 4 part, 
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
	float iel;
	float dblForce[2][6];
	float dblCombStrs[2][4];
	void ConvertOut(T_STRB_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				int nAdd=j*6;
				for(int k=0; k<6; k++)
				{
					// Force/Moment
					if     (i == 0 && j == 0)Data.dblForce[i][nAdd+k]=dblForce[0][k];
					else if(i == 3 && j == 1)Data.dblForce[i][nAdd+k]=dblForce[1][k];
					else if(j == 1)
					{
						Data.dblForce[i][nAdd+k]=dblForce[0][k]+(dblForce[1][k]-dblForce[0][k])*(i+1)/4;
						Data.dblForce[i+1][k]=Data.dblForce[i][nAdd+k];
					}
				}
				for(int k=0; k<4; k++)
				{
					// Combined Stress
					if     (i == 0 && j == 0)Data.dblStress[i][16+k*2+j]=dblCombStrs[0][k];
					else if(i == 3 && j == 1)Data.dblStress[i][16+k*2+j]=dblCombStrs[1][k];
					else if(j == 1)
					{
						Data.dblStress[i  ][16+k*2+1]=dblCombStrs[0][k]+(dblCombStrs[1][k]-dblCombStrs[0][k])*(i+1)/4;
						Data.dblStress[i+1][16+k*2+0]=Data.dblStress[i][16+k*2+1];
					}
				}
			}
		}
		Data.calcMaximumCombinedStress();
	}
};
struct _STBW_IFGR_FLOAT_BN
{
	// dblForce     : local force         , 4 part, 
	//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
	//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
	//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
	//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
	float iel;
	float dblForce[2][9];
	float dblCombStrs[2][4];
	void ConvertOut(T_STRB_D& Data)
	{
		Data.Initialize();
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				int nAdd=j*6;
				for(int k=0; k<6; k++)
				{
					// Force/Moment
					if     (i == 0 && j == 0)Data.dblForce[i][nAdd+k]=dblForce[0][k];
					else if(i == 3 && j == 1)Data.dblForce[i][nAdd+k]=dblForce[1][k];
					else if(j == 1)
					{
						Data.dblForce[i][nAdd+k]=dblForce[0][k]+(dblForce[1][k]-dblForce[0][k])*(i+1)/4;
						Data.dblForce[i+1][k]=Data.dblForce[i][nAdd+k];
					}
				}

//         for(int k=0; k<3; k++)
//         {
// 
//         }

				for(int k=0; k<4; k++)
				{
					// Combined Stress
					if     (i == 0 && j == 0)Data.dblStress[i][16+k*2+j]=dblCombStrs[0][k];
					else if(i == 3 && j == 1)Data.dblStress[i][16+k*2+j]=dblCombStrs[1][k];
					else if(j == 1)
					{
						Data.dblStress[i  ][16+k*2+1]=dblCombStrs[0][k]+(dblCombStrs[1][k]-dblCombStrs[0][k])*(i+1)/4;
						Data.dblStress[i+1][16+k*2+0]=Data.dblStress[i][16+k*2+1];
					}
				}
			}
		}
		Data.calcMaximumCombinedStress();
	}
};

struct _ELNK_IFGR_FLOAT_BN
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
#define _NLNK_IFGR_FLOAT_BN  _ELNK_IFGR_FLOAT_BN

#pragma pack()

#endif