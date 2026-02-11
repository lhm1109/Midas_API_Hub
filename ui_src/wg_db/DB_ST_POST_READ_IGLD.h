#ifndef __DB_ST_POST_READ_GMLD_H__
#define __DB_ST_POST_READ_GMLD_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

struct _REAC_GMLD_7DOF_BN
{
	int ino;
	double dblReacMax[REAC_SIZE];
	double dblReacMin[REAC_SIZE];
	void ConvertOut(T_REAC_D& DataMax, T_REAC_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<REAC_SIZE; i++)DataMax.dblReac[i]=dblReacMax[i];
		for(int i=0; i<REAC_SIZE; i++)DataMin.dblReac[i]=dblReacMin[i];
	}
};

struct _REAC_GMLD_BN
{
	int ino;
	double dblReacMax[6];
	double dblReacMin[6];
	void ConvertOut(T_REAC_D& DataMax, T_REAC_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<6; i++)DataMax.dblReac[i]=dblReacMax[i];
		for(int i=0; i<6; i++)DataMin.dblReac[i]=dblReacMin[i];
	}
};

#define _LRAC_GMLD_BN _REAC_GMLD_BN

struct _REAC_GMLD_BN1
{
	double dblReac[6];
	void ConvertOut(T_REAC_D& DataMax, T_REAC_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<6; i++)DataMax.dblReac[i]=dblReac[i];
		for(int i=0; i<6; i++)DataMin.dblReac[i]=dblReac[i];
	}
};
struct _REAC_GMLD_7DOF_BN1
{
	double dblReac[REAC_SIZE];
	void ConvertOut(T_REAC_D& DataMax, T_REAC_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<REAC_SIZE; i++)DataMax.dblReac[i]=dblReac[i];
		for(int i=0; i<REAC_SIZE; i++)DataMin.dblReac[i]=dblReac[i];
	}
};
struct _REAC_GMLD_BN2
{
	double dblReacMax[6];
	double dblReacMin[6];
	void ConvertOut(T_REAC_D& DataMax, T_REAC_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<6; i++)DataMax.dblReac[i]=dblReacMax[i];
		for(int i=0; i<6; i++)DataMin.dblReac[i]=dblReacMin[i];
	}
};
struct _REAC_GMLD_7DOF_BN2
{
	double dblReacMax[REAC_SIZE];
	double dblReacMin[REAC_SIZE];
	void ConvertOut(T_REAC_D& DataMax, T_REAC_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<REAC_SIZE; i++)DataMax.dblReac[i]=dblReacMax[i];
		for(int i=0; i<REAC_SIZE; i++)DataMin.dblReac[i]=dblReacMin[i];
	}
};


struct _DISP7_GMLD_BN
{
	int ino;
	double dblDispMax[DISP_SIZE];
	double dblDispMin[DISP_SIZE];
	void ConvertOut(T_DISP_D& DataMax, T_DISP_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<DISP_SIZE; i++)DataMax.dblDisp[i]=dblDispMax[i];
		for(int i=0; i<DISP_SIZE; i++)DataMin.dblDisp[i]=dblDispMin[i];
	}
};
struct _DISP_GMLD_BN
{
	int ino;
	double dblDispMax[6];
	double dblDispMin[6];
	void ConvertOut(_DISP7_GMLD_BN& Data)
	{
		for(int i=0; i<6; i++)Data.dblDispMax[i]=dblDispMax[i];
		for(int i=0; i<6; i++)Data.dblDispMin[i]=dblDispMin[i];
		Data.dblDispMax[6]=0.;
		Data.dblDispMin[6]=0.;
	}
};
struct _DISP_GMLD_BN1
{
	double dblDisp[6];
	void ConvertOut(_DISP7_GMLD_BN& Data)
	{
		for(int i=0; i<6; i++)Data.dblDispMax[i]=dblDisp[i];
		for(int i=0; i<6; i++)Data.dblDispMin[i]=dblDisp[i];
		Data.dblDispMax[6]=0.;
		Data.dblDispMin[6]=0.;
	}
};
struct _DISP7_GMLD_BN1
{
	double dblDisp[DISP_SIZE];
	void ConvertOut(_DISP7_GMLD_BN& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)Data.dblDispMax[i]=dblDisp[i];
		for(int i=0; i<DISP_SIZE; i++)Data.dblDispMin[i]=dblDisp[i];
	}
};
struct _DISP_GMLD_BN2
{
	double dblDispMax[6];
	double dblDispMin[6];
	void ConvertOut(_DISP7_GMLD_BN& Data)
	{
		for(int i=0; i<6; i++)Data.dblDispMax[i]=dblDispMax[i];
		for(int i=0; i<6; i++)Data.dblDispMin[i]=dblDispMin[i];
		Data.dblDispMax[6]=0.;
		Data.dblDispMin[6]=0.;
	}
};
struct _DISP7_GMLD_BN2
{
	double dblDispMax[DISP_SIZE];
	double dblDispMin[DISP_SIZE];
	void ConvertOut(_DISP7_GMLD_BN& Data)
	{
		for(int i=0; i<DISP_SIZE; i++)Data.dblDispMax[i]=dblDispMax[i];
		for(int i=0; i<DISP_SIZE; i++)Data.dblDispMin[i]=dblDispMin[i];
	}
};

struct _STRT_GMLD_BN
{
// dblForce     : local force, 
//                component : Fxi, Fxj
	int iel;
	double dblForceMax[2];
	double dblForceMin[2];
	void ConvertOut(T_STRT_D& DataMax, T_STRT_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<2; i++)DataMax.dblForce[i]=dblForceMax[i];
		for(int i=0; i<2; i++)DataMin.dblForce[i]=dblForceMin[i];
	}
};
struct _STRT_GMLD_BN1
{
	// dblForce     : local force, 
	//                component : Fxi, Fxj
	double dblForce[2];
	void ConvertOut(T_STRT_D& DataMax, T_STRT_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<2; i++)DataMax.dblForce[i]=dblForce[i];
		for(int i=0; i<2; i++)DataMin.dblForce[i]=dblForce[i];
	}
};
struct _STRT_GMLD_BN2
{
	// dblForce     : local force, 
	//                component : Fxi, Fxj
	double dblForceMax[2];
	double dblForceMin[2];
	void ConvertOut(T_STRT_D& DataMax, T_STRT_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		for(int i=0; i<2; i++)DataMax.dblForce[i]=dblForceMax[i];
		for(int i=0; i<2; i++)DataMin.dblForce[i]=dblForceMin[i];
	}
};

struct _STRB_GMLD_BN
{
// dblForce     : local force         , 4 part, 
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
	int iel;
	double dblForce[2][2][6];
	double dblCombStrs[2][2][4];
	void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
	{
		T_STRB_D* pData=NULL;
		for(int ii=0; ii<2; ii++)
		{
			if(ii == 0)pData=&DataMax;
			if(ii == 1)pData=&DataMin;
			pData->Initialize();
			for(int i=0; i<4; i++)
			{
				for(int j=0; j<2; j++)
				{
					int nAdd=j*6;
					for(int k=0; k<6; k++)
					{
						// Force/Moment
						if     (i == 0 && j == 0)pData->dblForce[i][nAdd+k]=dblForce[ii][0][k];
						else if(i == 3 && j == 1)pData->dblForce[i][nAdd+k]=dblForce[ii][1][k];
						else if(j == 1)
						{
							pData->dblForce[i][nAdd+k]=dblForce[ii][0][k]+(dblForce[ii][1][k]-dblForce[ii][0][k])*(i+1)/4;
							pData->dblForce[i+1][k]=pData->dblForce[i][nAdd+k];
						}
					}
					for(int k=0; k<4; k++)
					{
						// Combined Stress
						if     (i == 0 && j == 0)pData->dblStress[i][16+k*2+j]=dblCombStrs[ii][0][k];
						else if(i == 3 && j == 1)pData->dblStress[i][16+k*2+j]=dblCombStrs[ii][1][k];
						else if(j == 1)
						{
							pData->dblStress[i  ][16+k*2+1]=dblCombStrs[ii][0][k]+(dblCombStrs[ii][1][k]-dblCombStrs[ii][0][k])*(i+1)/4;
							pData->dblStress[i+1][16+k*2+0]=pData->dblStress[i][16+k*2+1];
						}
					}
				}
			}
			pData->calcMaximumCombinedStress();
		}
	}
};

struct _STRB_GMLD_BN1
{
	// dblForce     : local force         , 2 part, 
	//                component : Fx, Fy, Fz, Mx, My, Mz
	double dblForce[2][6];
	double dblCombStrs[2][4];
	void ConvertOut(T_STRB_D& DataMax)
	{
		T_STRB_D* pData=NULL;
		for(int ii=0; ii<1; ii++)
		{
			if(ii == 0)pData=&DataMax;
			//if(ii == 1)pData=&DataMin;
			pData->Initialize();
			for(int i=0; i<4; i++)
			{
				for(int j=0; j<2; j++)
				{
					int nAdd=j*6;
					for(int k=0; k<6; k++)
					{
						// Force/Moment
						if     (i == 0 && j == 0)pData->dblForce[i][nAdd+k]=dblForce[0][k];
						else if(i == 3 && j == 1)pData->dblForce[i][nAdd+k]=dblForce[1][k];
						else if(j == 1)
						{
							pData->dblForce[i][nAdd+k]=dblForce[0][k]+(dblForce[1][k]-dblForce[0][k])*(i+1)/4;
							pData->dblForce[i+1][k]=pData->dblForce[i][nAdd+k];
						}
					}
					for(int k=0; k<4; k++)
					{
						// Combined Stress
						if     (i == 0 && j == 0)pData->dblStress[i][16+k*2+j]=dblCombStrs[0][k];
						else if(i == 3 && j == 1)pData->dblStress[i][16+k*2+j]=dblCombStrs[1][k];
						else if(j == 1)
						{
							pData->dblStress[i  ][16+k*2+1]=dblCombStrs[0][k]+(dblCombStrs[1][k]-dblCombStrs[0][k])*(i+1)/4;
							pData->dblStress[i+1][16+k*2+0]=pData->dblStress[i][16+k*2+1];
						}
					}
				}
			}
			pData->calcMaximumCombinedStress();
		}
	}
};
struct _STRB_GMLD_BN2
{
	// dblForce     : local force         , 4 part, 
	//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
	//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
	//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
	//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
	double dblForceMAX[2][6];    // I/J, Fx~Mz
	double dblCombStrsMAX[2][4];
	double dblForceMIN[2][6];    // I/J, Fx~Mz
	double dblCombStrsMIN[2][4];

	void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
	{
		T_STRB_D* pData=NULL;
		for(int ii=0; ii<2; ii++)
		{
			if(ii == 0)pData=&DataMax;
			if(ii == 1)pData=&DataMin;
			pData->Initialize();

			for(int i=0; i<4; i++)
			{
				for(int j=0; j<2; j++)
				{
					int nAdd=j*6;
					for(int k=0; k<6; k++)
					{
						// Force/Moment
						if     (i == 0 && j == 0)pData->dblForce[i][nAdd+k]=(ii==0)? dblForceMAX[0][k] : dblForceMIN[0][k];
						else if(i == 3 && j == 1)pData->dblForce[i][nAdd+k]=(ii==0)? dblForceMAX[1][k] : dblForceMIN[1][k];
						else if(j == 1)
						{
							if(ii==0) pData->dblForce[i][nAdd+k]=dblForceMAX[0][k]+(dblForceMAX[1][k]-dblForceMAX[0][k])*(i+1)/4;
							else      pData->dblForce[i][nAdd+k]=dblForceMIN[0][k]+(dblForceMIN[1][k]-dblForceMIN[0][k])*(i+1)/4;

							pData->dblForce[i+1][k]=pData->dblForce[i][nAdd+k];
						}
					}
					for(int k=0; k<4; k++)
					{
						// Combined Stress
						if     (i == 0 && j == 0)pData->dblStress[i][16+k*2+j]=(ii==0)? dblCombStrsMAX[0][k] : dblCombStrsMIN[0][k];
						else if(i == 3 && j == 1)pData->dblStress[i][16+k*2+j]=(ii==0)? dblCombStrsMAX[1][k] : dblCombStrsMIN[1][k];
						else if(j == 1)
						{
							if(ii==0) pData->dblStress[i  ][16+k*2+1]=dblCombStrsMAX[0][k]+(dblCombStrsMAX[1][k]-dblCombStrsMAX[0][k])*(i+1)/4;
							else      pData->dblStress[i  ][16+k*2+1]=dblCombStrsMIN[0][k]+(dblCombStrsMIN[1][k]-dblCombStrsMIN[0][k])*(i+1)/4;

							pData->dblStress[i+1][16+k*2+0]=pData->dblStress[i][16+k*2+1];
						}
					}
				}
			}
			pData->calcMaximumCombinedStress();
		}
	}
};

struct _STBW_GMLD_BN1
{
	// dblForce     : local force         , 2 part, 
	//                component : Fx, Fy, Fz, Mx, My, Mz, BiM, ST-VT, WT
	double dblForce[2][9];
	double dblCombStrs[2][4];

	void ConvertOut(T_STRB_D& DataMax)
	{
		T_STRB_D* pData=NULL;
		for(int ii=0; ii<1; ii++)
		{
			if(ii == 0)pData=&DataMax;
			//if(ii == 1)pData=&DataMin;
			pData->Initialize();
			for(int i=0; i<4; i++)
			{
				for(int j=0; j<2; j++)
				{
					int nAdd=j*6;
					for(int k=0; k<6; k++)
					{
						// Force/Moment
						if     (i == 0 && j == 0)pData->dblForce[i][nAdd+k]=dblForce[0][k];  // I
						else if(i == 3 && j == 1)pData->dblForce[i][nAdd+k]=dblForce[1][k];  // J
						else if(j == 1)                                                          
						{
							pData->dblForce[i][nAdd+k]=dblForce[0][k]+(dblForce[1][k]-dblForce[0][k])*(i+1)/4;
							pData->dblForce[i+1][k]   =pData->dblForce[i][nAdd+k];
						}
					}
					for(int k=0; k<3; k++)
					{
						if     (i == 0 && j == 0) pData->dblForce[i][12+k*2  ]=dblForce[0][6+k];
						else if(i == 3 && j == 1) pData->dblForce[i][12+k*2+1]=dblForce[1][6+k];
						else if(j == 1)  
						{
							pData->dblForce[i  ][12+k] =dblForce[0][6+k]+(dblForce[1][6+k]-dblForce[0][6+k])*(i+1)/4;
							pData->dblForce[i+1][12+k] =pData->dblForce[i][12+k];
						}
					}
					for(int k=0; k<4; k++)
					{
						// Combined Stress
						if     (i == 0 && j == 0)pData->dblStress[i][16+k*2+j]=dblCombStrs[0][k];
						else if(i == 3 && j == 1)pData->dblStress[i][16+k*2+j]=dblCombStrs[1][k];
						else if(j == 1)
						{
							pData->dblStress[i  ][16+k*2+1]=dblCombStrs[0][k]+(dblCombStrs[1][k]-dblCombStrs[0][k])*(i+1)/4;
							pData->dblStress[i+1][16+k*2+0]=pData->dblStress[i][16+k*2+1];
						}
					}
				}
			}
			pData->calcMaximumCombinedStress();
		}
	}
};
struct _STBW_GMLD_BN2
{
	// dblForce     : local force         , 2 part, 
	//                component : Fx, Fy, Fz, Mx, My, Mz, BiM, ST-VT, WT
	double dblForceMAX[2][9];   // I/J, Fx~Mz
	double dblCombStrsMAX[2][4];
	double dblForceMIN[2][9];   
	double dblCombStrsMIN[2][4];

	void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
	{
		T_STRB_D* pData=NULL;
		for(int ii=0; ii<2; ii++)
		{
			if(ii == 0)pData=&DataMax;
			if(ii == 1)pData=&DataMin;
			pData->Initialize();
			for(int i=0; i<4; i++)
			{
				for(int j=0; j<2; j++)
				{
					int nAdd=j*6;
					for(int k=0; k<6; k++)
					{
						// Force/Moment
						if     (i == 0 && j == 0)pData->dblForce[i][nAdd+k]= (ii==0)? dblForceMAX[0][k] : dblForceMIN[0][k];  // I
						else if(i == 3 && j == 1)pData->dblForce[i][nAdd+k]= (ii==0)? dblForceMAX[1][k] : dblForceMIN[1][k];  // J
						else if(j == 1)                                                          
						{
							if(ii==0) pData->dblForce[i][nAdd+k]=dblForceMAX[0][k]+(dblForceMAX[1][k]-dblForceMAX[0][k])*(i+1)/4;
							else      pData->dblForce[i][nAdd+k]=dblForceMIN[0][k]+(dblForceMIN[1][k]-dblForceMIN[0][k])*(i+1)/4;

							pData->dblForce[i+1][k]   =pData->dblForce[i][nAdd+k];
						}
					}
					for(int k=0; k<3; k++)
					{
						// 내용 확인 필요
						//             if     (i == 0 && j == 0) pData->dblForce[i][12+k*2  ]=dblForce[ii][0][6+k];
						//             else if(i == 3 && j == 1) pData->dblForce[i][12+k*2+1]=dblForce[ii][1][6+k];
						//             else if(j == 1)  
						//             {
						// 
						//             }
					}
					for(int k=0; k<4; k++)
					{
						// Combined Stress
						if     (i == 0 && j == 0)pData->dblStress[i][16+k*2+j]= (ii==0)? dblCombStrsMAX[0][k] : dblCombStrsMIN[0][k];
						else if(i == 3 && j == 1)pData->dblStress[i][16+k*2+j]= (ii==0)? dblCombStrsMAX[1][k] : dblCombStrsMIN[1][k];
						else if(j == 1)
						{
							if(ii==0) pData->dblStress[i  ][16+k*2+1]=dblCombStrsMAX[0][k]+(dblCombStrsMAX[1][k]-dblCombStrsMAX[0][k])*(i+1)/4;
							else      pData->dblStress[i  ][16+k*2+1]=dblCombStrsMIN[0][k]+(dblCombStrsMIN[1][k]-dblCombStrsMIN[0][k])*(i+1)/4;

							pData->dblStress[i+1][16+k*2+0]=pData->dblStress[i][16+k*2+1];
						}
					}
				}
			}
			pData->calcMaximumCombinedStress();
		}
	}
};

struct _SBCF_GMLD_BN
{
// dblForce     : local force         , 4 part, 
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
	int iel;
	double dblForce[2][2][6][6];
	void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
	{
		T_SBCF_D* pData=NULL;
		for(int ii=0; ii<2; ii++)
		{
			if(ii == 0)pData=&DataMax;
			if(ii == 1)pData=&DataMin;
			pData->Initialize();
			for(int i=0; i<4; i++)
			{
				for(int j=0; j<2; j++)
				{
					for(int k=0; k<6; k++)
					{
						// Force/Moment
						if(i == 0 && j == 0)
						{
							for(int l=0; l<6; l++)pData->dblForce[i][j][k][l]=dblForce[ii][0][k][l];
						}
						else if(i == 3 && j == 1)
						{
							for(int l=0; l<6; l++)pData->dblForce[i][j][k][l]=dblForce[ii][1][k][l];
						}
						else if(j == 1)
						{
							for(int l=0; l<6; l++)
							{
								pData->dblForce[i][j][k][l]=dblForce[ii][0][k][l]+(dblForce[ii][1][k][l]-dblForce[ii][0][k][l])*(i+1)/4;
								pData->dblForce[i+1][j-1][k][l]=pData->dblForce[i][j][k][l];
							}
						}
					}
				}
			}
		}
	}
};

struct _ELNK_GMLD_BN
{
// dblForce  : local force, 1 node       , component : fx, fy, fz, mx, my, mz
	int iel;
	double ino[2];
	double dblForceMax[2][6];
	double dblForceMin[2][6];
	void ConvertOut(T_STEK_D& DataMax, T_STEK_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		ASSERT(sizeof(dblForceMax) == sizeof(DataMax.dblForce));
		memcpy(&DataMax.dblForce, dblForceMax, sizeof(dblForceMax));
		memcpy(&DataMin.dblForce, dblForceMin, sizeof(dblForceMin));
	}
};
#define _NLNK_GMLD_BN  _ELNK_GMLD_BN


struct _ELNK_GMLD_BN1
{
	// dblForce  : local force, 1 node       , component : fx, fy, fz, mx, my, mz
	double dblForce[2][6];
	void ConvertOut(T_STEK_D& DataMax, T_STEK_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		ASSERT(sizeof(dblForce) == sizeof(DataMax.dblForce));
		memcpy(&DataMax.dblForce, dblForce, sizeof(dblForce));
		memcpy(&DataMin.dblForce, dblForce, sizeof(dblForce));
	}
};
struct _ELNK_GMLD_BN2
{
	// dblForce  : local force, 1 node       , component : fx, fy, fz, mx, my, mz
	double dblForceMax[2][6];
	double dblForceMin[2][6];
	void ConvertOut(T_STEK_D& DataMax, T_STEK_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();
		ASSERT(sizeof(dblForceMax) == sizeof(DataMax.dblForce));
		memcpy(&DataMax.dblForce, dblForceMax, sizeof(dblForceMax));
		memcpy(&DataMin.dblForce, dblForceMin, sizeof(dblForceMin));
	}
};
#define _NLNK_GMLD_BN1  _ELNK_GMLD_BN1
#define _NLNK_GMLD_BN2  _ELNK_GMLD_BN2

#pragma pack()

#endif