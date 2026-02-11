// DB_ST_POST_READ_THIS.cpp: implementation of the CCraneInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_ST_DT_UNIT.h"
#include "DB_ST_DT.h"
#include "DB_ST_DT_CASE.h"
#include "DB_ST_RT.h"
#include "DB_ST_POST_READ_STLD.h"

#include "DB_ST_POST_READ_THIS.h"

////////////////////////////////////////////////////////////
// _DISP_THIS_BN
void _DISP_THIS_BN::ConvertOut(T_DISP_TH_D& Data)
{
	maxDis.ConvertOut(Data.maxDis);
	minDis.ConvertOut(Data.minDis);
	maxVel.ConvertOut(Data.maxVel);
	minVel.ConvertOut(Data.minVel);
	maxAcc.ConvertOut(Data.maxAcc);
	minAcc.ConvertOut(Data.minAcc);
	maxAAc.ConvertOut(Data.maxAAc);
	minAAc.ConvertOut(Data.minAAc);
	for(int i=0; i<6; i++)
	{
		Data.dMaxDisTime[i] = dMaxDisTime[i];
		Data.dMinDisTime[i] = dMinDisTime[i];
		Data.dMaxVelTime[i] = dMaxVelTime[i];
		Data.dMinVelTime[i] = dMinVelTime[i];
		Data.dMaxTimeAcc[i] = dMaxTimeAcc[i];
		Data.dMinTimeAcc[i] = dMinTimeAcc[i];
		Data.dMaxAAcTime[i] = dMaxAAcTime[i];
		Data.dMinAAcTime[i] = dMinAAcTime[i];
	}
}

////////////////////////////////////////////////////////////
// _STBW_THIS_BN
void _STBW_THIS_BN::ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
{
	DataMax.Initialize();
	DataMin.Initialize();
		
	DataMax.nDOFType=2;
	DataMin.nDOFType=2;

	// 선형보간 
	int     j,k;
	int     nIIdx,nJIdx;
	double  dIRatio,dJRatio;
	for(j=0; j<4; j++)
	{
		dIRatio = ((double)j)/4.0;
		dJRatio = ((double)(j+1))/4.0;
			
		// 6자유도 결과 Force/Moment.
		for(k=0; k<6; k++)
		{
			DataMax.dblForce[j][k]   = dblStressMax[k] + (dblStressMax[k+6]-dblStressMax[k])*dIRatio;
			DataMax.dblForce[j][k+6] = dblStressMax[k] + (dblStressMax[k+6]-dblStressMax[k])*dJRatio;

			DataMin.dblForce[j][k]   = dblStressMin[k] + (dblStressMin[k+6]-dblStressMin[k])*dIRatio;
			DataMin.dblForce[j][k+6] = dblStressMin[k] + (dblStressMin[k+6]-dblStressMin[k])*dJRatio;
		} 

		// 7자유도 결과 Force/Moment. (Mb: Bi-Moment, Mt: Twisting Moment, Mw: Warping Moment)
		int iMb = 12;     // Mb 결과 시작 index
		int iMt = iMb+2;  // =14;  
		int iMw = iMt+2;  // =16;  
		for (k=iMb; k<iMb+6; k+=2)
		{
			DataMax.dblForce[j][k  ]   = dblStressMax[k] + (dblStressMax[k+1]-dblStressMax[k])*dIRatio;   // i-max
			DataMax.dblForce[j][k+1]   = dblStressMax[k] + (dblStressMax[k+1]-dblStressMax[k])*dJRatio;   // j-max

			DataMin.dblForce[j][k  ]   = dblStressMin[k] + (dblStressMin[k+1]-dblStressMin[k])*dIRatio;   // i-min
			DataMin.dblForce[j][k+1]   = dblStressMin[k] + (dblStressMin[k+1]-dblStressMin[k])*dJRatio;   // j-min
		}

		// 6자유도 결과 Stress.
		int iStress = 12+6;  // 12: Force & Moment, 6: 7th DOF Moment
		for(k=0; k<7; k++)
		{
			nIIdx = k+iStress;  nJIdx = nIIdx+7;   // 7: Stress의 i단 결과 갯수(=j단 결과 갯수)
			DataMax.dblStress[j][k]   = dblStressMax[nIIdx] + (dblStressMax[nJIdx]-dblStressMax[nIIdx])*dIRatio;
			DataMax.dblStress[j][k+7] = dblStressMax[nIIdx] + (dblStressMax[nJIdx]-dblStressMax[nIIdx])*dJRatio;

			DataMin.dblStress[j][k]   = dblStressMin[nIIdx] + (dblStressMin[nJIdx]-dblStressMin[nIIdx])*dIRatio;
			DataMin.dblStress[j][k+7] = dblStressMin[nIIdx] + (dblStressMin[nJIdx]-dblStressMin[nIIdx])*dJRatio;
		}

		// Combined Stress
		int iCombStrs  = 14;   // T_STRB_D에서 Combined Stress의 시작 index
		int iCombStrs1 = iCombStrs+2; // T_STRB_D에서 Comb1i의 시작 index
		for (k=iCombStrs1; k<iCombStrs1+8; k+=2)
		{
			int m = (int)(k-iCombStrs1)/2; 
			//for (int m=0; m<4; ++m)
			{
				DataMax.dblStress[j][k  ]=dblCombStrsMax[0][m] +(dblCombStrsMax[1][m] - dblCombStrsMax[0][m])*dIRatio;
				DataMax.dblStress[j][k+1]=dblCombStrsMax[0][m] +(dblCombStrsMax[1][m] - dblCombStrsMax[0][m])*dJRatio;

				DataMin.dblStress[j][k  ]=dblCombStrsMin[0][m] +(dblCombStrsMin[1][m] - dblCombStrsMin[0][m])*dIRatio;
				DataMin.dblStress[j][k+1]=dblCombStrsMin[0][m] +(dblCombStrsMin[1][m] - dblCombStrsMin[0][m])*dJRatio;
			}
		}

		// Warping Normal Stress
		int iWarpNormalStrs = 32;   // Warping Normal Stress I-end 결과의 시작 index
		int index = 0;
		for (k=0; k<20; ++k)
		{
			nIIdx = k+iWarpNormalStrs;  nJIdx = nIIdx+20;   // 20: Warping Stress의 i단 결과 갯수(=j단 결과 갯수)
			index = k+24;                                   // 24: T_STRB_D에서 Warping Stress의 시작 index
			DataMax.dblStress[j][index   ] = dblStressMax[nIIdx] + (dblStressMax[nJIdx]-dblStressMax[nIIdx])*dIRatio;
			DataMax.dblStress[j][index+20] = dblStressMax[nIIdx] + (dblStressMax[nJIdx]-dblStressMax[nIIdx])*dJRatio;

			DataMin.dblStress[j][index   ] = dblStressMin[nIIdx] + (dblStressMin[nJIdx]-dblStressMin[nIIdx])*dIRatio;
			DataMin.dblStress[j][index+20] = dblStressMin[nIIdx] + (dblStressMin[nJIdx]-dblStressMin[nIIdx])*dJRatio;
		}

		// CombTau1i,.... CombTau8i, CombTau1j,.... CombTau8j (for 7th DOF : 16개)
		int iCombTau  = 64;   // T_STRB_D에서 CombTau의 시작 index
		for (int m=0; m<8; ++m)
		{
			index = 2*m + iCombTau;

			DataMax.dblStress[j][index  ]=dblCombStrsMaxEtc[0][m] +(dblCombStrsMaxEtc[1][m] - dblCombStrsMaxEtc[0][m])*dIRatio;
			DataMax.dblStress[j][index+1]=dblCombStrsMaxEtc[0][m] +(dblCombStrsMaxEtc[1][m] - dblCombStrsMaxEtc[0][m])*dJRatio;

			DataMin.dblStress[j][index  ]=dblCombStrsMinEtc[0][m] +(dblCombStrsMinEtc[1][m] - dblCombStrsMinEtc[0][m])*dIRatio;
			DataMin.dblStress[j][index+1]=dblCombStrsMinEtc[0][m] +(dblCombStrsMinEtc[1][m] - dblCombStrsMinEtc[0][m])*dJRatio;
		}
	}
		
	DataMax.calcMaximumCombinedStress();
	DataMin.calcMaximumCombinedStress();
}

// Max,Min Time을 Covert함 
void _STBW_THIS_BN::ConvertOutTime(T_STRB_D& DataMax, T_STRB_D& DataMin)
{
	DataMax.Initialize();
	DataMin.Initialize();

	DataMax.nDOFType=2;
	DataMin.nDOFType=2;
		
	// 선형보간 
	int     j,k;
	int     nIIdx,nJIdx;
	double  dIRatio,dJRatio;
	for(j=0; j<1; j++)
	{
		dIRatio = 0.0;
		dJRatio = 1.0;
			
		// 6자유도 결과 Force/Moment.
		for(k=0; k<6; k++)
		{
			DataMax.dblForce[0][k]   = dMaxTime[k];
			DataMax.dblForce[3][k+6] = dMaxTime[k+6];

			DataMin.dblForce[0][k]   = dMinTime[k];
			DataMin.dblForce[3][k+6] = dMinTime[k+6];
		} 

		// 7자유도 결과 Force/Moment. (Mb: Bi-Moment, Mt: Twisting Moment, Mw: Warping Moment)
		int iMb = 12;     // Mb 결과 시작 index
		int iMt = iMb+2;  // =14;  
		int iMw = iMt+2;  // =16;  
		for (k=iMb; k<iMb+6; k+=2)
		{
			DataMax.dblForce[0][k  ]   = dMaxTime[k];   // i-max
			DataMax.dblForce[3][k+1]   = dMaxTime[k+1];   // j-max

			DataMin.dblForce[0][k  ]   = dMinTime[k];   // i-min
			DataMin.dblForce[3][k+1]   = dMinTime[k+1];   // j-min
		}

		// 6자유도 결과 Stress.
		int iStress = 12+6;  // 12: Force & Moment, 6: 7th DOF Moment
		for(k=0; k<7; k++)
		{
			nIIdx = k+iStress;  nJIdx = nIIdx+7;   // 7: Stress의 i단 결과 갯수(=j단 결과 갯수)
			DataMax.dblStress[0][k]   = dMaxTime[nIIdx];
			DataMax.dblStress[3][k+7] = dMaxTime[nJIdx];

			DataMin.dblStress[0][k]   = dMinTime[nIIdx];
			DataMin.dblStress[3][k+7] = dMinTime[nJIdx];
		}

		// Combined Stress
		int iCombStrs  = 14;   // T_STRB_D에서 Combined Stress의 시작 index
		int iCombStrs1 = iCombStrs+2; // T_STRB_D에서 Comb1i의 시작 index
		for (k=iCombStrs1; k<iCombStrs1+8; k+=2)
		{
			int m = (int)(k-iCombStrs1)/2; 
			//for (int m=0; m<4; ++m)
			{
				DataMax.dblStress[0][k  ]=dCombStrsMaxTime[0][m];
				DataMax.dblStress[3][k+1]=dCombStrsMaxTime[1][m];

				DataMin.dblStress[0][k  ]=dCombStrsMinTime[0][m];
				DataMin.dblStress[3][k+1]=dCombStrsMinTime[1][m];
			}
		}

		// Warping Normal Stress
		int iWarpNormalStrs = 32;   // Warping Normal Stress I-end 결과의 시작 index
		int index = 0;
		for (k=0; k<20; ++k)
		{
			nIIdx = k+iWarpNormalStrs;  nJIdx = nIIdx+20;   // 20: Warping Stress의 i단 결과 갯수(=j단 결과 갯수)
			index = k+24;                                   // 24: T_STRB_D에서 Warping Stress의 시작 index
			DataMax.dblStress[0][index   ] = dMaxTime[nIIdx];
			DataMax.dblStress[3][index+20] = dMaxTime[nJIdx];

			DataMin.dblStress[0][index   ] = dMinTime[nIIdx];
			DataMin.dblStress[3][index+20] = dMinTime[nJIdx];
		}

		// CombTau1i,.... CombTau8i, CombTau1j,.... CombTau8j (for 7th DOF : 16개)
		int iCombTau  = 64;   // T_STRB_D에서 CombTau의 시작 index
		for (int m=0; m<8; ++m)
		{
			index = 2*m + iCombTau;

			DataMax.dblStress[0][index  ]=dCombStrsMaxTimeEtc[0][m];
			DataMax.dblStress[3][index+1]=dCombStrsMaxTimeEtc[1][m];

			DataMin.dblStress[0][index  ]=dCombStrsMinTimeEtc[0][m];
			DataMin.dblStress[3][index+1]=dCombStrsMinTimeEtc[1][m];
		}

	}
}

////////////////////////////////////////////////////////////
// _STRB_THIS_BN
void _STRB_THIS_BN::ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
{
	DataMax.Initialize();
	DataMin.Initialize();
	/*
	for(int j=0; j<4; j++)
	{
		for(int k=0; k<12; k++)DataMax.dblForce[j][k]=dblStressMax[j][k];
		for(int l=0; l<14; l++)DataMax.dblStress[j][l]=dblStressMax[j][l+12];
		DataMax.dblStress[j][16]=dblCombStrsMax[j][0][0];
		DataMax.dblStress[j][18]=dblCombStrsMax[j][0][1];
		DataMax.dblStress[j][20]=dblCombStrsMax[j][0][2];
		DataMax.dblStress[j][22]=dblCombStrsMax[j][0][3];
		DataMax.dblStress[j][17]=dblCombStrsMax[j][1][0];
		DataMax.dblStress[j][19]=dblCombStrsMax[j][1][1];
		DataMax.dblStress[j][21]=dblCombStrsMax[j][1][2];
		DataMax.dblStress[j][23]=dblCombStrsMax[j][1][3];

		for(    k=0; k<12; k++)DataMin.dblForce[j][k]=dblStressMin[j][k];
		for(    l=0; l<14; l++)DataMin.dblStress[j][l]=dblStressMin[j][l+12];
		DataMin.dblStress[j][16]=dblCombStrsMin[j][0][0];
		DataMin.dblStress[j][18]=dblCombStrsMin[j][0][1];
		DataMin.dblStress[j][20]=dblCombStrsMin[j][0][2];
		DataMin.dblStress[j][22]=dblCombStrsMin[j][0][3];
		DataMin.dblStress[j][17]=dblCombStrsMin[j][1][0];
		DataMin.dblStress[j][19]=dblCombStrsMin[j][1][1];
		DataMin.dblStress[j][21]=dblCombStrsMin[j][1][2];
		DataMin.dblStress[j][23]=dblCombStrsMin[j][1][3];
	}
	*/

	// 선형보간 
	int     j,k;
	int     nIIdx,nJIdx;
	double  dIRatio,dJRatio;
	for(j=0; j<4; j++)
	{
		dIRatio = ((double)j)/4.0;
		dJRatio = ((double)(j+1))/4.0;
			
		for(k=0; k<6; k++)
		{
			DataMax.dblForce[j][k]   = dblStressMax[k] + (dblStressMax[k+6]-dblStressMax[k])*dIRatio;
			DataMax.dblForce[j][k+6] = dblStressMax[k] + (dblStressMax[k+6]-dblStressMax[k])*dJRatio;

			DataMin.dblForce[j][k]   = dblStressMin[k] + (dblStressMin[k+6]-dblStressMin[k])*dIRatio;
			DataMin.dblForce[j][k+6] = dblStressMin[k] + (dblStressMin[k+6]-dblStressMin[k])*dJRatio;
		} 
		for(k=0; k<7; k++)
		{
			nIIdx = k+12; nJIdx = k+12+7;
			DataMax.dblStress[j][k]   = dblStressMax[nIIdx] + (dblStressMax[nJIdx]-dblStressMax[nIIdx])*dIRatio;
			DataMax.dblStress[j][k+7] = dblStressMax[nIIdx] + (dblStressMax[nJIdx]-dblStressMax[nIIdx])*dJRatio;

			DataMin.dblStress[j][k]   = dblStressMin[nIIdx] + (dblStressMin[nJIdx]-dblStressMin[nIIdx])*dIRatio;
			DataMin.dblStress[j][k+7] = dblStressMin[nIIdx] + (dblStressMin[nJIdx]-dblStressMin[nIIdx])*dJRatio;
		}

		DataMax.dblStress[j][16]=dblCombStrsMax[0][0] +(dblCombStrsMax[1][0] - dblCombStrsMax[0][0])*dIRatio;
		DataMax.dblStress[j][18]=dblCombStrsMax[0][1] +(dblCombStrsMax[1][1] - dblCombStrsMax[0][1])*dIRatio;
		DataMax.dblStress[j][20]=dblCombStrsMax[0][2] +(dblCombStrsMax[1][2] - dblCombStrsMax[0][2])*dIRatio;
		DataMax.dblStress[j][22]=dblCombStrsMax[0][3] +(dblCombStrsMax[1][3] - dblCombStrsMax[0][3])*dIRatio;

		DataMax.dblStress[j][17]=dblCombStrsMax[0][0] +(dblCombStrsMax[1][0] - dblCombStrsMax[0][0])*dJRatio;
		DataMax.dblStress[j][19]=dblCombStrsMax[0][1] +(dblCombStrsMax[1][1] - dblCombStrsMax[0][1])*dJRatio;
		DataMax.dblStress[j][21]=dblCombStrsMax[0][2] +(dblCombStrsMax[1][2] - dblCombStrsMax[0][2])*dJRatio;
		DataMax.dblStress[j][23]=dblCombStrsMax[0][3] +(dblCombStrsMax[1][3] - dblCombStrsMax[0][3])*dJRatio;

		DataMin.dblStress[j][16]=dblCombStrsMin[0][0] +(dblCombStrsMin[1][0] - dblCombStrsMin[0][0])*dIRatio;
		DataMin.dblStress[j][18]=dblCombStrsMin[0][1] +(dblCombStrsMin[1][1] - dblCombStrsMin[0][1])*dIRatio;
		DataMin.dblStress[j][20]=dblCombStrsMin[0][2] +(dblCombStrsMin[1][2] - dblCombStrsMin[0][2])*dIRatio;
		DataMin.dblStress[j][22]=dblCombStrsMin[0][3] +(dblCombStrsMin[1][3] - dblCombStrsMin[0][3])*dIRatio;

		DataMin.dblStress[j][17]=dblCombStrsMin[0][0] +(dblCombStrsMin[1][0] - dblCombStrsMin[0][0])*dJRatio;
		DataMin.dblStress[j][19]=dblCombStrsMin[0][1] +(dblCombStrsMin[1][1] - dblCombStrsMin[0][1])*dJRatio;
		DataMin.dblStress[j][21]=dblCombStrsMin[0][2] +(dblCombStrsMin[1][2] - dblCombStrsMin[0][2])*dJRatio;
		DataMin.dblStress[j][23]=dblCombStrsMin[0][3] +(dblCombStrsMin[1][3] - dblCombStrsMin[0][3])*dJRatio;
	}
		
	DataMax.calcMaximumCombinedStress();
	DataMin.calcMaximumCombinedStress();
}

// Max,Min Time을 Covert함 
void _STRB_THIS_BN::ConvertOutTime(T_STRB_D& DataMax, T_STRB_D& DataMin)
{
	DataMax.Initialize();
	DataMin.Initialize();

	/*
	for(int j=0; j<4; j++)
	{
		for(int k=0; k<12; k++)DataMax.dblForce[j][k]=dMaxTime[j][k];
		for(int l=0; l<14; l++)DataMax.dblStress[j][l]=dMaxTime[j][l+12];
		DataMax.dblStress[j][16]=dCombStrsMaxTime[j][0][0];
		DataMax.dblStress[j][18]=dCombStrsMaxTime[j][0][1];
		DataMax.dblStress[j][20]=dCombStrsMaxTime[j][0][2];
		DataMax.dblStress[j][22]=dCombStrsMaxTime[j][0][3];
		DataMax.dblStress[j][17]=dCombStrsMaxTime[j][1][0];
		DataMax.dblStress[j][19]=dCombStrsMaxTime[j][1][1];
		DataMax.dblStress[j][21]=dCombStrsMaxTime[j][1][2];
		DataMax.dblStress[j][23]=dCombStrsMaxTime[j][1][3];
		for(    k=0; k<12; k++)DataMin.dblForce[j][k]=dMinTime[j][k];
		for(    l=0; l<14; l++)DataMin.dblStress[j][l]=dMinTime[j][l+12];
		DataMin.dblStress[j][16]=dCombStrsMinTime[j][0][0];
		DataMin.dblStress[j][18]=dCombStrsMinTime[j][0][1];
		DataMin.dblStress[j][20]=dCombStrsMinTime[j][0][2];
		DataMin.dblStress[j][22]=dCombStrsMinTime[j][0][3];
		DataMin.dblStress[j][17]=dCombStrsMinTime[j][1][0];
		DataMin.dblStress[j][19]=dCombStrsMinTime[j][1][1];
		DataMin.dblStress[j][21]=dCombStrsMinTime[j][1][2];
		DataMin.dblStress[j][23]=dCombStrsMinTime[j][1][3];
	}
	*/

	//for(j=0; j<4; j++)
	// 선형보간 
	int     j,k;
	int     nIIdx,nJIdx;
	double  dIRatio,dJRatio;
	for(j=0; j<1; j++)
	{
		dIRatio = 0.0;
		dJRatio = 1.0;
			
		for(k=0; k<6; k++)
		{
			DataMax.dblForce[0][k]   = dMaxTime[k];
			DataMax.dblForce[3][k+6] = dMaxTime[k+6];

			DataMin.dblForce[0][k]   = dMinTime[k];
			DataMin.dblForce[3][k+6] = dMinTime[k+6];
		} 
		for(k=0; k<7; k++)
		{
			nIIdx = k+12; nJIdx = k+12+7;
			DataMax.dblStress[0][k]   = dMaxTime[nIIdx];
			DataMax.dblStress[3][k+7] = dMaxTime[nJIdx];

			DataMin.dblStress[0][k]   = dMinTime[nIIdx];
			DataMin.dblStress[3][k+7] = dMinTime[nJIdx];
		}

		DataMax.dblStress[0][16]=dCombStrsMaxTime[0][0];
		DataMax.dblStress[0][18]=dCombStrsMaxTime[0][1];
		DataMax.dblStress[0][20]=dCombStrsMaxTime[0][2];
		DataMax.dblStress[0][22]=dCombStrsMaxTime[0][3];

		DataMax.dblStress[3][17]=dCombStrsMaxTime[1][0];
		DataMax.dblStress[3][19]=dCombStrsMaxTime[1][1];
		DataMax.dblStress[3][21]=dCombStrsMaxTime[1][2];
		DataMax.dblStress[3][23]=dCombStrsMaxTime[1][3];

		DataMin.dblStress[0][16]=dCombStrsMinTime[0][0] ;
		DataMin.dblStress[0][18]=dCombStrsMinTime[0][1] ;
		DataMin.dblStress[0][20]=dCombStrsMinTime[0][2] ;
		DataMin.dblStress[0][22]=dCombStrsMinTime[0][3] ;

		DataMin.dblStress[3][17]=dCombStrsMinTime[1][0] ;
		DataMin.dblStress[3][19]=dCombStrsMinTime[1][1] ;
		DataMin.dblStress[3][21]=dCombStrsMinTime[1][2] ;
		DataMin.dblStress[3][23]=dCombStrsMinTime[1][3] ;
	}
}

////////////////////////////////////////////////////////////
// _STRS_THIS_BN
void _STRS_THIS_BN::ConvertOut(T_STRS_D& DataMax, T_STRS_D& DataMin)
{
	DataMax.Initialize();
	DataMin.Initialize();
	for(int j=0; j<8; j++)
	{
		for(int k=0; k<3; k++)DataMax.dblForce[j][k]=dblForceMax[j][k];
		for(int k=0; k<3; k++)DataMin.dblForce[j][k]=dblForceMin[j][k];
	}
	for(int j=0; j<9; j++)
	{
		DataMax.dblStress[j][ 0]=dblStressMax[j][ 0];
		DataMax.dblStress[j][ 1]=dblStressMax[j][ 1];
		DataMax.dblStress[j][ 2]=dblStressMax[j][ 2];
		DataMax.dblStress[j][ 3]=dblStressMax[j][ 3];
		DataMax.dblStress[j][ 4]=dblStressMax[j][ 4];
		DataMax.dblStress[j][ 5]=dblStressMax[j][ 5];
		DataMin.dblStress[j][ 0]=dblStressMin[j][ 0];
		DataMin.dblStress[j][ 1]=dblStressMin[j][ 1];
		DataMin.dblStress[j][ 2]=dblStressMin[j][ 2];
		DataMin.dblStress[j][ 3]=dblStressMin[j][ 3];
		DataMin.dblStress[j][ 4]=dblStressMin[j][ 4];
		DataMin.dblStress[j][ 5]=dblStressMin[j][ 5];
	}
}

////////////////////////////////////////////////////////////
// _STPN_THIS_BN
void _STPN_THIS_BN::ConvertOut(T_STPN_D& DataMax, T_STPN_D& DataMin)
{
	DataMax.Initialize();
	DataMin.Initialize();
	for(int j=0; j<4; j++)
	{
		for(int k=0; k<3; k++)DataMax.dblForce[j][k]=dblForceMax[j][k];
		for(int k=0; k<3; k++)DataMin.dblForce[j][k]=dblForceMin[j][k];
	}
	for(int j=0; j<5; j++)
	{
		DataMax.dblStress[j][ 0]=dblStressMax[j][ 0];
		DataMax.dblStress[j][ 1]=dblStressMax[j][ 2];
		DataMax.dblStress[j][ 2]=dblStressMax[j][ 1];
		DataMax.dblStress[j][ 3]=0.;
		DataMax.dblStress[j][ 4]=0.;
		DataMax.dblStress[j][ 5]=dblStressMax[j][ 3];
		DataMin.dblStress[j][ 0]=dblStressMin[j][ 0];
		DataMin.dblStress[j][ 1]=dblStressMin[j][ 2];
		DataMin.dblStress[j][ 2]=dblStressMin[j][ 1];
		DataMin.dblStress[j][ 3]=0.;
		DataMin.dblStress[j][ 4]=0.;
		DataMin.dblStress[j][ 5]=dblStressMin[j][ 3];
	}
}

////////////////////////////////////////////////////////////
// _SBCF_THIS_BN
// beam concurrent force
void _SBCF_THIS_BN::ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
{
	DataMax.Initialize();
	DataMin.Initialize();
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<2; j++)
		{
			for(int k=0; k<6; k++)
			{
				for(int l=0; l<6; l++)DataMax.dblForce[i][j][k][l]=dblForceMax[i][j][k][l];
				for(int l=0; l<6; l++)DataMin.dblForce[i][j][k][l]=dblForceMin[i][j][k][l];
			}
		}
	}
}

void _SBCF_THIS_BN::Add(_SBCF_THIS_BN& Data)
{
	for(int i=0; i<4; i++)
		for(int j=0; j<2; j++)
			for(int k=0; k<6; k++)
				for(int l=0; l<6; l++)
				{
					dblForceMax[i][j][k][l]+=Data.dblForceMax[i][j][k][l];
					dblForceMin[i][j][k][l]+=Data.dblForceMin[i][j][k][l];
				}
}

////////////////////////////////////////////////////////////
// _SBCF_THIS_7DOF_BN
// 7DOF beam concurrent force
void _SBCF_THIS_7DOF_BN::ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
{
	DataMax.Initialize();
	DataMin.Initialize();
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<2; j++)
		{
			for(int k=0; k<9; k++)
			{
				for(int l=0; l<9; l++)
				{
					DataMax.dblForce[i][j][k][l]=dblForceMax[i][j][k][l];
					DataMin.dblForce[i][j][k][l]=dblForceMin[i][j][k][l];
				}
			}
		}
	}
}

void _SBCF_THIS_7DOF_BN::Add(_SBCF_THIS_7DOF_BN& Data)
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<2; j++)
		{
			for(int k=0; k<9; k++)
			{
				for(int l=0; l<9; l++)
				{
					dblForceMax[i][j][k][l]+=Data.dblForceMax[i][j][k][l];
					dblForceMin[i][j][k][l]+=Data.dblForceMin[i][j][k][l];
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////
// _SIMC_THIS_BN
// virtual beam concurrent force
void _SIMC_THIS_BN::ConvertOut(T_SIMC_D& DataMax, T_SIMC_D& DataMin)
{
	DataMax.Initialize();
	DataMin.Initialize();
	for(int j=0; j<2; j++)
	{
		for(int k=0; k<6; k++)
		{
			for(int l=0; l<6; l++)DataMax.dblForce[j][k][l]=dblForceMax[j][k][l];
			for(int l=0; l<6; l++)DataMin.dblForce[j][k][l]=dblForceMin[j][k][l];
		}
	}
}

void _SIMC_THIS_BN::Add(_SIMC_THIS_BN& Data)
{
	for(int j=0; j<2; j++)
		for(int k=0; k<6; k++)
			for(int l=0; l<6; l++)
			{
				dblForceMax[j][k][l]+=Data.dblForceMax[j][k][l];
				dblForceMin[j][k][l]+=Data.dblForceMin[j][k][l];
			}
}

////////////////////////////////////////////////////////////
// _MDFL_BN
void _MDFL_BN::Initialize()
{
	dDispl=0.;
	dVelocity=0.;
	dAcceleration=0.;
}

////////////////////////////////////////////////////////////
// _INDF_BN
void _INDF_BN::Initialize()
{
	dDispl=0.;
	dVelocity=0.;
	dAcceleration=0.;
	dDynamicNode=0.;
}
