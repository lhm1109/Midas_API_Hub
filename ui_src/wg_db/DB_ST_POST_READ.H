#ifndef __DB_ST_POST_READ_DB_H__
#define __DB_ST_POST_READ_DB_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

//////////////////////////
// displacement

struct _DISP_BN
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

struct _DISP2_7DOF_BN
{
    T_DISP_D max;
    T_DISP_D min;
};

struct _DISP2_BN
{
    _DISP_BN max;
    _DISP_BN min;
};

struct _DISP3_7DOF_BN
{
    T_DISP_D Displ;
};

struct _DISP3_BN
{
    _DISP_BN Displ;
};

struct _DISP3_FLOAT_7DOF_BN
{
    float dblDisp[DISP_SIZE];
    void ConvertToDouble(_DISP3_7DOF_BN& Data)
    {
        for(int i=0; i<DISP_SIZE; i++)Data.Displ.dblDisp[i]=dblDisp[i];
    }
};

struct _DISP3_FLOAT_BN
{
    float dblDisp[6];
    void ConvertToDouble(_DISP3_BN& Data)
    {
        for(int i=0; i<6; i++)Data.Displ.dblDisp[i]=dblDisp[i];
    }
};
/*
struct _DISP4_BN
{
    T_DISP_D dsp;
    T_DISP_D vel;
    T_DISP_D acc;
};
*/

struct _REAC_SSPS2_BN
{
    double dMaxForce[5];
    double dMaxDisplacement[5];
    double dMinForce[5];
    double dMinDisplacement[5];
    double dElement;
    double dDirection;
    double dElementType;
};

struct _REAC2_7DOF_BN
{
    T_REAC_D max;
    T_REAC_D min;
};

struct _REAC_BN
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
    void Mult(double dFactor)
    {
        if(dFactor == 1.)return;
        for(int i=0; i<6; i++)dblReac[i]*=dFactor;
    }
};

struct _REAC2_BN
{
    _REAC_BN max;
    _REAC_BN min;
};

struct _LRAC2_7DOF_BN
{
    T_REAC_D max;
    T_REAC_D min;
    double ino;
};

struct _LRAC2_BN
{
    _REAC_BN max;
    _REAC_BN min;
    double ino;
};

struct _LRAC4_7DOF_BN
{
    T_REAC_D max;
    T_REAC_D min;
    double ino;
};

struct _LRAC4_BN
{
    _REAC_BN max;
    _REAC_BN min;
    double ino;
};

struct _REAC3_7DOF_BN
{
    double ino;
    T_REAC_D Reacf;
};

struct _REAC3_BN
{
    double ino;
    _REAC_BN Reacf;
};

struct _REAC3_FLOAT_7DOF_BN
{
    float ino;
    float dblReac[REAC_SIZE];
    void ConvertToDouble(_REAC3_7DOF_BN& Data)
    {
        Data.ino=ino;
        for(int i=0; i<REAC_SIZE; i++)Data.Reacf.dblReac[i]=dblReac[i];
    }
};

struct _REAC3_FLOAT_BN
{
    float ino;
    float dblReac[6];
    void ConvertToDouble(_REAC3_BN& Data)
    {
        Data.ino=ino;
        for(int i=0; i<6; i++)Data.Reacf.dblReac[i]=dblReac[i];
    }
};

struct _LRAC3_7DOF_BN
{
    double ino;
    T_REAC_D Reacf;
};

struct _LRAC3_BN
{
    double ino;
    _REAC_BN Reacf;
};

struct _LRAC3_FLOAT_7DOF_BN
{
    float ino;
    float dblReac[REAC_SIZE];
    void ConvertToDouble(_LRAC3_7DOF_BN& Data)
    {
        Data.ino=ino;
        for(int i=0; i<REAC_SIZE; i++)Data.Reacf.dblReac[i]=dblReac[i];
    }
};

struct _LRAC3_FLOAT_BN
{
    float ino;
    float dblReac[6];
    void ConvertToDouble(_LRAC3_BN& Data)
    {
        Data.ino=ino;
        for(int i=0; i<6; i++)Data.Reacf.dblReac[i]=dblReac[i];
    }
};

struct _STRT2_BN
{
// dblForce     : local force, min, max , 
//                component : Fxi, Fxj
    double dblForceMax[2];
    double dblForceMin[2];
    double dblStressMax[2];
    double dblStressMin[2];
    int iel;
};

struct _STRT4_BN
{
// dblForce     : local force, min, max , 
//                component : Fxi, Fxj
    double dblForceMax[2];
    double dblStressMax[2];
	double dblForceMin[2];
    double dblStressMin[2];
};

struct _STRT3_BN
{
// dblForce     : local force, 
//                component : Fxi, Fxj
    double iel;
    double dblForce[2];
};

struct _STRT3_FLOAT_BN
{
// dblForce     : local force, 
//                component : Fxi, Fxj
    float iel;
    float dblForce[2];
    void ConvertToDouble(_STRT3_BN& Data)
    {
        Data.iel=iel;
        for(int i=0; i<2; i++)Data.dblForce[i]=dblForce[i];
    }
};

struct _STBW2_BN
{
	// dblForce     : local force,  min, max, 5 point
	//                component : Fx, Fy, Fz, Mx, My, Mz, BI, ST, Warping Tor
    double dblForceMax[5][9];
    double dblForceMin[5][9];
    double dblStressMax[5][27];
    double dblStressMin[5][27];
    double dblCombStrsMax[5][12];
    double dblCombStrsMin[5][12];
    int iel;
    void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
    {
        DataMax.Initialize();
        DataMin.Initialize();
        DataMax.nDOFType=2;
        DataMin.nDOFType=2;
		int k=0;
        for(int j=0; j<4; j++)
        {
            for(k=0; k<6; k++)DataMax.dblForce[j][  k]=dblForceMax[j  ][k];
            for(k=0; k<6; k++)DataMax.dblForce[j][6+k]=dblForceMax[j+1][k];
            for(k=0; k<6; k++)DataMin.dblForce[j][  k]=dblForceMin[j  ][k];
            for(k=0; k<6; k++)DataMin.dblForce[j][6+k]=dblForceMin[j+1][k];
            for(k=0; k<7; k++)DataMax.dblStress[j][  k]=dblStressMax[j  ][k];
            for(k=0; k<7; k++)DataMax.dblStress[j][7+k]=dblStressMax[j+1][k];
            for(k=0; k<7; k++)DataMin.dblStress[j][  k]=dblStressMin[j  ][k];
            for(k=0; k<7; k++)DataMin.dblStress[j][7+k]=dblStressMin[j+1][k];
			for(k=0; k<4; k++)
			{
				DataMax.dblStress[j][16+k*2  ]=dblCombStrsMax[j  ][k];
				DataMax.dblStress[j][16+k*2+1]=dblCombStrsMax[j+1][k];
				DataMin.dblStress[j][16+k*2  ]=dblCombStrsMin[j  ][k];
				DataMin.dblStress[j][16+k*2+1]=dblCombStrsMin[j+1][k];
			}

			for(k=0; k<3; k++)
			{
				DataMax.dblForce[j][12+k*2  ]=dblForceMax[j  ][6+k];
				DataMax.dblForce[j][12+k*2+1]=dblForceMax[j+1][6+k];
				DataMin.dblForce[j][12+k*2  ]=dblForceMin[j  ][6+k];
				DataMin.dblForce[j][12+k*2+1]=dblForceMin[j+1][6+k];
			}

			for(k=0; k<20; k++)
			{
				DataMax.dblStress[j][24+k   ]=dblStressMax[j  ][7+k];
				DataMax.dblStress[j][24+k+20]=dblStressMax[j+1][7+k];
				DataMin.dblStress[j][24+k   ]=dblStressMin[j  ][7+k];
				DataMin.dblStress[j][24+k+20]=dblStressMin[j+1][7+k];
			}
			for(k=0; k<8; k++)
			{
				DataMax.dblStress[j][64+k*2  ]=dblCombStrsMax[j  ][4+k];
				DataMax.dblStress[j][64+k*2+1]=dblCombStrsMax[j+1][4+k];
				DataMin.dblStress[j][64+k*2  ]=dblCombStrsMin[j  ][4+k];
				DataMin.dblStress[j][64+k*2+1]=dblCombStrsMin[j+1][4+k];
			}
        }
        DataMax.calcMaximumCombinedStress();
        DataMin.calcMaximumCombinedStress();
    }
};

struct _STRB2_BN
{
// dblForce     : local force,  min, max, 5 point
//                component : Fx, Fy, Fz, Mx, My, Mz
    double dblForceMax[5][6];
    double dblForceMin[5][6];
    double dblStressMax[5][7];
    double dblStressMin[5][7];
    double dblCombStrsMax[5][4];
    double dblCombStrsMin[5][4];
    int iel;
    void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
    {
		int k = 0;
        DataMax.Initialize();
        DataMin.Initialize();
        for(int j=0; j<4; j++)
        {
            for(k=0; k<6; k++)DataMax.dblForce[j][  k]=dblForceMax[j][k];
            for(k=0; k<6; k++)DataMax.dblForce[j][6+k]=dblForceMax[j+1][k];
            for(k=0; k<6; k++)DataMin.dblForce[j][  k]=dblForceMin[j][k];
            for(k=0; k<6; k++)DataMin.dblForce[j][6+k]=dblForceMin[j+1][k];
            for(k=0; k<7; k++)DataMax.dblStress[j][  k]=dblStressMax[j  ][k];
            for(k=0; k<7; k++)DataMax.dblStress[j][7+k]=dblStressMax[j+1][k];
            for(k=0; k<7; k++)DataMin.dblStress[j][  k]=dblStressMin[j  ][k];
            for(k=0; k<7; k++)DataMin.dblStress[j][7+k]=dblStressMin[j+1][k];
            DataMax.dblStress[j][16]=dblCombStrsMax[j  ][0];
            DataMax.dblStress[j][17]=dblCombStrsMax[j+1][0];
            DataMax.dblStress[j][18]=dblCombStrsMax[j  ][1];
            DataMax.dblStress[j][19]=dblCombStrsMax[j+1][1];
            DataMax.dblStress[j][20]=dblCombStrsMax[j  ][2];
            DataMax.dblStress[j][21]=dblCombStrsMax[j+1][2];
            DataMax.dblStress[j][22]=dblCombStrsMax[j  ][3];
            DataMax.dblStress[j][23]=dblCombStrsMax[j+1][3];
            DataMin.dblStress[j][16]=dblCombStrsMin[j  ][0];
            DataMin.dblStress[j][17]=dblCombStrsMin[j+1][0];
            DataMin.dblStress[j][18]=dblCombStrsMin[j  ][1];
            DataMin.dblStress[j][19]=dblCombStrsMin[j+1][1];
            DataMin.dblStress[j][20]=dblCombStrsMin[j  ][2];
            DataMin.dblStress[j][21]=dblCombStrsMin[j+1][2];
            DataMin.dblStress[j][22]=dblCombStrsMin[j  ][3];
            DataMin.dblStress[j][23]=dblCombStrsMin[j+1][3];
        }
        DataMax.calcMaximumCombinedStress();
        DataMin.calcMaximumCombinedStress();
    }
};

struct _SIMB2_BN
{
    // dblForce     : local force,  min, max, 5 point
    //                component : Fx, Fy, Fz, Mx, My, Mz
    double dblForceMax[2][6];
    double dblForceMin[2][6];
    void ConvertOut(T_SIMB_D& DataMax, T_SIMB_D& DataMin)
    {
        DataMax.Initialize();
        DataMin.Initialize();
        {
            int k=0;
            for(k=0; k<6; k++)DataMax.dblForce[  k]=dblForceMax[0][k];
            for(k=0; k<6; k++)DataMax.dblForce[6+k]=dblForceMax[1][k];
            for(k=0; k<6; k++)DataMin.dblForce[  k]=dblForceMin[0][k];
            for(k=0; k<6; k++)DataMin.dblForce[6+k]=dblForceMin[1][k];
        }
    }
};

struct _SRIM2_BN
{
	double dblStressMax[SRIM_SIZE]; // I:18개 단면지점,J:18개 단면지점
    double dblStressMin[SRIM_SIZE]; // I:18개 단면지점,J:18개 단면지점

	void ConvertOut(T_SRIM_D& DataMax, T_SRIM_D& DataMin)
	{
        DataMax.Initialize();
        DataMin.Initialize();
        for (int i = 0; i < SRIM_SIZE; i++)
        {
            DataMax.dblStress[i] = dblStressMax[i];
            DataMin.dblStress[i] = dblStressMin[i];
        }
	}
};

struct _FSIM2_BN
{
	// dblStress    :  force, 4 part, 
	//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj,
	double dblStressMax[4][12];
	double dCombStrsMax[4][2][4];
	double dblStressMin[4][12];
	double dCombStrsMin[4][2][4];

	void ConvertOut(T_FSIM_D& DataMax, T_FSIM_D& DataMin)
	{
        DataMax.Initialize();
        DataMin.Initialize();
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 12; k++)DataMax.dblForce[j][k] = dblStressMax[j][k];
			DataMax.dblStress[j][2] = dCombStrsMax[j][0][0]; // i
			DataMax.dblStress[j][4] = dCombStrsMax[j][0][1];
			DataMax.dblStress[j][6] = dCombStrsMax[j][0][2];
			DataMax.dblStress[j][8] = dCombStrsMax[j][0][3];

			DataMax.dblStress[j][3] = dCombStrsMax[j][1][0]; // j
			DataMax.dblStress[j][5] = dCombStrsMax[j][1][1];
			DataMax.dblStress[j][7] = dCombStrsMax[j][1][2];
			DataMax.dblStress[j][9] = dCombStrsMax[j][1][3];


			for (int k = 0; k < 12; k++)DataMin.dblForce[j][k] = dblStressMin[j][k];
			DataMin.dblStress[j][2] = dCombStrsMin[j][0][0]; // i
            DataMin.dblStress[j][4] = dCombStrsMin[j][0][1];
            DataMin.dblStress[j][6] = dCombStrsMin[j][0][2];
            DataMin.dblStress[j][8] = dCombStrsMin[j][0][3];

			DataMin.dblStress[j][3] = dCombStrsMin[j][1][0]; // j
            DataMin.dblStress[j][5] = dCombStrsMin[j][1][1];
            DataMin.dblStress[j][7] = dCombStrsMin[j][1][2];
            DataMin.dblStress[j][9] = dCombStrsMin[j][1][3];

		}
        DataMax.calcMaximumCombinedStress();
        DataMin.calcMaximumCombinedStress();
	}
};

// Design Strip Beam
#define _PTSB2_BN _SIMB2_BN

struct _STBW5_BN
{
	// dblForce     : local force,  min, max, 5 point
	//                component : Fx, Fy, Fz, Mx, My, Mz, BI, ST, WarpingT
    double dblForceMax[5][9];
	double dblCombStrsMax[5][12];
	double dblStressMax[5][27];
	double dblCurForceMax[5][9][10];    // force
	double dblCurStressMax[5][12][10];    // stress
	
    double dblForceMin[5][9];
	double dblCombStrsMin[5][12];
    double dblStressMin[5][27];
	double dblCurForceMin[5][9][10];
	double dblCurStressMin[5][12][10];
	
    void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
    {
		int k=0;
        DataMax.Initialize();
        DataMin.Initialize();
        DataMax.nDOFType=2;
        DataMin.nDOFType=2;
        for(int j=0; j<4; j++)
        {
            for(k=0; k<6; k++)DataMax.dblForce[j][  k]=dblForceMax[j][k];
            for(k=0; k<6; k++)DataMax.dblForce[j][6+k]=dblForceMax[j+1][k];
            for(k=0; k<6; k++)DataMin.dblForce[j][  k]=dblForceMin[j][k];
            for(k=0; k<6; k++)DataMin.dblForce[j][6+k]=dblForceMin[j+1][k];
            for(k=0; k<7; k++)DataMax.dblStress[j][  k]=dblStressMax[j  ][k];
            for(k=0; k<7; k++)DataMax.dblStress[j][7+k]=dblStressMax[j+1][k];
            for(k=0; k<7; k++)DataMin.dblStress[j][  k]=dblStressMin[j  ][k];
            for(k=0; k<7; k++)DataMin.dblStress[j][7+k]=dblStressMin[j+1][k];

			for(k=0; k<4; k++)
			{
				DataMax.dblStress[j][16+k*2  ]=dblCombStrsMax[j  ][k];
				DataMax.dblStress[j][16+k*2+1]=dblCombStrsMax[j+1][k];
			}

			for(k=0; k<3; k++)
			{
				DataMax.dblForce[j][12+k*2  ]=dblForceMax[j  ][6+k];
				DataMax.dblForce[j][12+k*2+1]=dblForceMax[j+1][6+k];
				DataMin.dblForce[j][12+k*2  ]=dblForceMin[j  ][6+k];
				DataMin.dblForce[j][12+k*2+1]=dblForceMin[j+1][6+k];
			}

			for(k=0; k<8; k++)
			{
				DataMax.dblStress[j][64+k*2  ]=dblCombStrsMax[j  ][4+k];
				DataMax.dblStress[j][64+k*2+1]=dblCombStrsMax[j+1][4+k];
				DataMin.dblStress[j][64+k*2  ]=dblCombStrsMin[j  ][4+k];
				DataMin.dblStress[j][64+k*2+1]=dblCombStrsMin[j+1][4+k];
			}
			for(k=0; k<20; k++)
			{
				DataMax.dblStress[j][24+k   ]=dblStressMax[j  ][7+k];
				DataMax.dblStress[j][24+k+20]=dblStressMax[j+1][7+k];
				DataMin.dblStress[j][24+k   ]=dblStressMin[j  ][7+k];
				DataMin.dblStress[j][24+k+20]=dblStressMin[j+1][7+k];
			}
        }
        DataMax.calcMaximumCombinedStress();
        DataMin.calcMaximumCombinedStress();
    }
	
    void ConvertOut2(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
    {
        DataMax.Initialize();
        DataMin.Initialize();
        for(int i=0; i<4; i++)
        {	
            for(int k=0; k<9; k++)
            {
                for(int l=0; l<9; l++)
				{
					DataMax.dblForce[i][0][k][l]=dblCurForceMax[i][k][l];
					DataMax.dblForce[i][1][k][l]=dblCurForceMax[i+1][k][l];
					DataMin.dblForce[i][0][k][l]=dblCurForceMin[i][k][l];
					DataMin.dblForce[i][1][k][l]=dblCurForceMin[i+1][k][l];
				}
            }
        }
    }
    void ConvertOut3(T_SBCS_D& DataMax, T_SBCS_D& DataMin)
    {
        DataMax.Initialize();
        DataMin.Initialize();
        for(int i=0; i<4; i++)
        {	
			for(int j=0; j<4; j++)
			{
				for(int k=0; k<7; k++)
				{
					DataMax.dblForce[i][j][0][k]=dblCurStressMax[i][j][k];
					DataMax.dblForce[i][j][1][k]=dblCurStressMax[i+1][j][k];
					DataMin.dblForce[i][j][0][k]=dblCurStressMin[i][j][k];
					DataMin.dblForce[i][j][1][k]=dblCurStressMin[i+1][j][k];
				}
			}
        }
    }
};

struct _STRB5_BN
{
// dblForce     : local force,  min, max, 5 point
//                component : Fx, Fy, Fz, Mx, My, Mz
    double dblForceMax[5][6];
	double dblCombStrsMax[5][4];
	double dblStressMax[5][7];
	double dblCurForceMax[5][6][7];    // force
	double dblCurStressMax[5][4][7];    // stress

    double dblForceMin[5][6];
	double dblCombStrsMin[5][4];
    double dblStressMin[5][7];
	double dblCurForceMin[5][6][7];
	double dblCurStressMin[5][4][7];

    void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
    {
        DataMax.Initialize();
        DataMin.Initialize();
        for(int j=0; j<4; j++)
        {
			int k = 0;
            for(k=0; k<6; k++)DataMax.dblForce[j][  k]=dblForceMax[j][k];
            for(k=0; k<6; k++)DataMax.dblForce[j][6+k]=dblForceMax[j+1][k];
            for(k=0; k<6; k++)DataMin.dblForce[j][  k]=dblForceMin[j][k];
            for(k=0; k<6; k++)DataMin.dblForce[j][6+k]=dblForceMin[j+1][k];
            for(k=0; k<7; k++)DataMax.dblStress[j][  k]=dblStressMax[j  ][k];
            for(k=0; k<7; k++)DataMax.dblStress[j][7+k]=dblStressMax[j+1][k];
            for(k=0; k<7; k++)DataMin.dblStress[j][  k]=dblStressMin[j  ][k];
            for(k=0; k<7; k++)DataMin.dblStress[j][7+k]=dblStressMin[j+1][k];
            DataMax.dblStress[j][16]=dblCombStrsMax[j  ][0];
            DataMax.dblStress[j][17]=dblCombStrsMax[j+1][0];
            DataMax.dblStress[j][18]=dblCombStrsMax[j  ][1];
            DataMax.dblStress[j][19]=dblCombStrsMax[j+1][1];
            DataMax.dblStress[j][20]=dblCombStrsMax[j  ][2];
            DataMax.dblStress[j][21]=dblCombStrsMax[j+1][2];
            DataMax.dblStress[j][22]=dblCombStrsMax[j  ][3];
            DataMax.dblStress[j][23]=dblCombStrsMax[j+1][3];
            DataMin.dblStress[j][16]=dblCombStrsMin[j  ][0];
            DataMin.dblStress[j][17]=dblCombStrsMin[j+1][0];
            DataMin.dblStress[j][18]=dblCombStrsMin[j  ][1];
            DataMin.dblStress[j][19]=dblCombStrsMin[j+1][1];
            DataMin.dblStress[j][20]=dblCombStrsMin[j  ][2];
            DataMin.dblStress[j][21]=dblCombStrsMin[j+1][2];
            DataMin.dblStress[j][22]=dblCombStrsMin[j  ][3];
            DataMin.dblStress[j][23]=dblCombStrsMin[j+1][3];
        }
        DataMax.calcMaximumCombinedStress();
        DataMin.calcMaximumCombinedStress();
    }

    void ConvertOut2(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
    {
        DataMax.Initialize();
        DataMin.Initialize();
        for(int i=0; i<4; i++)
        {	
            for(int k=0; k<6; k++)
            {
                for(int l=0; l<6; l++)
				{
					DataMax.dblForce[i][0][k][l]=dblCurForceMax[i][k][l];
					DataMax.dblForce[i][1][k][l]=dblCurForceMax[i+1][k][l];
					DataMin.dblForce[i][0][k][l]=dblCurForceMin[i][k][l];
					DataMin.dblForce[i][1][k][l]=dblCurForceMin[i+1][k][l];
				}
            }
        }
    }
    void ConvertOut3(T_SBCS_D& DataMax, T_SBCS_D& DataMin)
    {
        DataMax.Initialize();
        DataMin.Initialize();
        for(int i=0; i<4; i++)
        {	
			for(int j=0; j<4; j++)
			{
				for(int k=0; k<7; k++)
				{
					DataMax.dblForce[i][j][0][k]=dblCurStressMax[i][j][k];
					DataMax.dblForce[i][j][1][k]=dblCurStressMax[i+1][j][k];
					DataMin.dblForce[i][j][0][k]=dblCurStressMin[i][j][k];
					DataMin.dblForce[i][j][1][k]=dblCurStressMin[i+1][j][k];
				}
			}
        }
    }
};

struct _STRB3_BN
{
	// dblForce     : local force         , 4 part, 
	//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
	//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
	//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
	//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
	//													: Bii, STi, Wti, n1bi, n2bi, n1St,
	//													: n2St, n1Wt, n2Wt, n1bi, n2bi, n1St,
	//													: n2St, n1Wt, n2Wt, n1bi, n2bi, n1St,
	//													: n2St, n1Wt, n2Wt, Bij, STj, Wtj
    double iel;
    double dblForce[4][12];
	double dblForce7Dof[4][6];
    double dblCombStrs[5][12];
    void Initialize()
    {
        memset(this, 0, sizeof(_STRB3_BN));
    }
    void ConvertToStrb(T_STRB_D& DataStrb)
    {
        DataStrb.Initialize();
        // force
        memcpy(&DataStrb.dblForce[0][0],&dblForce[0][0],sizeof(double)*6);
		DataStrb.dblForce[0][12] = dblForce7Dof[0][0];
		DataStrb.dblForce[0][14] = dblForce7Dof[0][1];
		DataStrb.dblForce[0][16] = dblForce7Dof[0][2];
        for(int i=0; i<3; i++)
        {
            DataStrb.dblForce[i][ 6]=dblForce[i+0][ 6];
            DataStrb.dblForce[i][ 7]=dblForce[i+0][ 8];
            DataStrb.dblForce[i][ 8]=dblForce[i+0][10];
            DataStrb.dblForce[i][ 9]=dblForce[i+1][ 0];
            DataStrb.dblForce[i][10]=dblForce[i+1][ 2];
            DataStrb.dblForce[i][11]=dblForce[i+1][ 4];
			DataStrb.dblForce[i][13]=dblForce7Dof[i+0][4];
			DataStrb.dblForce[i][15]=dblForce7Dof[i+1][0];
			DataStrb.dblForce[i][17]=dblForce7Dof[i+1][2];

            DataStrb.dblForce[i+1][ 0]=dblForce[i+0][ 7];
            DataStrb.dblForce[i+1][ 1]=dblForce[i+0][ 9];
            DataStrb.dblForce[i+1][ 2]=dblForce[i+0][11];
            DataStrb.dblForce[i+1][ 3]=dblForce[i+1][ 1];
            DataStrb.dblForce[i+1][ 4]=dblForce[i+1][ 3];
            DataStrb.dblForce[i+1][ 5]=dblForce[i+1][ 5];
			DataStrb.dblForce[i+1][12]=dblForce7Dof[i+0][3];
			DataStrb.dblForce[i+1][14]=dblForce7Dof[i+0][5];
			DataStrb.dblForce[i+1][16]=dblForce7Dof[i+1][1];
        }
        memcpy(&DataStrb.dblForce[3][6],&dblForce[3][6],sizeof(double)*6);
		DataStrb.dblForce[3][13] = dblForce7Dof[3][3];
		DataStrb.dblForce[3][15] = dblForce7Dof[3][4];
		DataStrb.dblForce[3][17] = dblForce7Dof[3][5];
        // combined stress
        for(int j=0; j<4; j++)
        {
			for(int k=0; k<4; k++)
			{
				DataStrb.dblStress[j][16+k*2  ]=dblCombStrs[j  ][k];
				DataStrb.dblStress[j][16+k*2+1]=dblCombStrs[j+1][k];
			}
			for(int k=0; k<8; k++)
			{
				DataStrb.dblStress[j][64+k*2  ]=dblCombStrs[j  ][4+k];
				DataStrb.dblStress[j][64+k*2+1]=dblCombStrs[j+1][4+k];
			}
        }
        DataStrb.calcMaximumCombinedStress();
    }
};

struct _STRB3_BN_OLD
{
// dblForce     : local force         , 4 part, 
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
    double iel;
    double dblForce[4][12];
    void ConvertToNew(_STRB3_BN& Data)
    {
        memcpy(&Data.dblForce, dblForce,sizeof(dblForce));
        memset(&Data.dblCombStrs, 0, sizeof(Data.dblCombStrs));
    }
};

struct _STRB3_FLOAT_BN
{
// dblForce     : local force         , 4 part, 
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
//                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
    float iel;
    float dblForce[4][12];
    float dblCombStrs[5][4];
    void ConvertToDouble(_STRB3_BN& Data)
    {
        Data.iel=iel;
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<12; j++)Data.dblForce[i][j]=dblForce[i][j];
        }
        for(int i = 0; i<5; i++)
        {
            for(int j=0; j<4; j++)Data.dblCombStrs[i][j]=dblCombStrs[i][j];
        }
    }
};

struct _STBW3_BN_OLD
{
    // dblForce     : local force         , 4 part, 
    //                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
    //                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
    //                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
    //                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
    double iel;
    double dblForce[4][12];
    double dblForce7Dof[4][6];
    void ConvertToNew(_STRB3_BN& Data)
    {
        memcpy(&Data.dblForce, dblForce,sizeof(dblForce));
        memcpy(&Data.dblForce7Dof, dblForce7Dof,sizeof(dblForce7Dof));
        memset(&Data.dblCombStrs, 0, sizeof(Data.dblCombStrs));
    }
};

struct _STBW3_FLOAT_BN
{
    // dblForce     : local force         , 4 part, 
    //                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
    //                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
    //                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fxi, Fyj, Fyi, Fzj, Fzi
    //                component : Mxj, Mxi, Myj, Myi, Mzj, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
    //													: Bii, STi, Wti, n1bi, n2bi, n1St,
    //													: n2St, n1Wt, n2Wt, n1bi, n2bi, n1St,
    //													: n2St, n1Wt, n2Wt, n1bi, n2bi, n1St,
    //													: n2St, n1Wt, n2Wt, Bij, STj, Wtj
    float iel;
    float dblForce[4][12];
    float dblForce7Dof[4][6];
    float dblCombStrs[5][12];
    void ConvertToDouble(_STRB3_BN& Data)
    {
        int j=0;
        Data.iel=iel;
        for(int i=0; i<4; i++)
        {
            for(j=0; j<12; j++)Data.dblForce[i][j]=dblForce[i][j];
            for(j=0; j<6; j++)Data.dblForce7Dof[i][j]=dblForce7Dof[i][j];
        }
        for(int i = 0; i<5; i++)
        {
            for(j=0; j<12; j++)Data.dblCombStrs[i][j]=dblCombStrs[i][j];
        }
    }
};

struct _STRB4_BN
{
// dblStress    : local force, stress         , 4 part, 
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj,
//                            Sxi, Syi, Szi, Bypi, Bymi, Bzpi, Bzmi, Sxj, Syj, Szj, Bypj, Bymj, Bzpj, Bzmj
    double dblStress[1][26];
};

struct _STRW2_BN
{
// dblForce     : local force         , 2 part, top, bottom
//                component : Fxi, Fyi, Fzi, Mxi, Myi, Mzi,  Fxj, Fyj, Fzj, Mxj, Myj, Mzj
    double dblForceMax[2][6];
//	double dblCurForceMax[2][6][6];
    double dblForceMin[2][6];
//	double dblCurForceMin[2][6][6];
};

struct _SBCF_MV_7DOF_BN
{
	// dblForce     : local force,  min, max, 5 point
	//                component : Fx, Fy, Fz, Mx, My, Mz
    double dblForceMax[5][9][9];
    double dblForceMin[5][9][9];
    int iel;
    void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
    {
        DataMax.Initialize();
        DataMin.Initialize();
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<2; j++)
            {
                int m=i+j;
                for(int k=0; k<9; k++)
                {
                    for(int l=0; l<9; l++)DataMax.dblForce[i][j][k][l]=dblForceMax[m][k][l];
                    for(int l=0; l<9; l++)DataMin.dblForce[i][j][k][l]=dblForceMin[m][k][l];
                }
            }
        }
    }
};

struct _SBCF_MV_BN
{
// dblForce     : local force,  min, max, 5 point
//                component : Fx, Fy, Fz, Mx, My, Mz
    double dblForceMax[5][6][6];
    double dblForceMin[5][6][6];
    int iel;
    void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
    {
        DataMax.Initialize();
        DataMin.Initialize();
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<2; j++)
            {
                int m=i+j;
                for(int k=0; k<6; k++)
                {
                    for(int l=0; l<6; l++)DataMax.dblForce[i][j][k][l]=dblForceMax[m][k][l];
					for(int l=0; l<6; l++)DataMin.dblForce[i][j][k][l]=dblForceMin[m][k][l];
                }
            }
        }
    }
};

struct _MBSS_MV_BN
{
	// 5 point
    // component  6 : Fx, Fy, Fz, Mx, My, Mz
	// component 11 : Sx, Sy, Sz, Byp, Bym, Bzp, Bzm, Comb1, Comb2, Comb3, Comb4

    // Concurrent stress by Force
    double dblStressMax[5][6][11];
	double dblStressMin[5][6][11];
    // Concurrent stress by Stress
	double dblStress2Max[5][11][11];
	double dblStress2Min[5][11][11];

    // Original result
	void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
	{
        // DataMax.dblStress[4][2][9][40];
        memset(DataMax.dblStress, 0, sizeof(DataMax.dblStress));
        memset(DataMin.dblStress, 0, sizeof(DataMin.dblStress));

        for(int i=0; i<4; i++)
        {
            for(int j=0; j<2; j++)
            {
                int m=i+j;
                for(int k=0; k<6; k++) // 해석은 6개만 존재
                {
                    for (int l=0; l<11; l++)
                    {
                        if(l < 7)  DataMax.dblStress[i][j][k][l]   = dblStressMax[m][k][l];
                        else       DataMax.dblStress[i][j][k][l+1] = dblStressMax[m][k][l];
                    }
					for(int l=0; l<11; l++)
                    {
                        if(l < 7)  DataMin.dblStress[i][j][k][l]   = dblStressMin[m][k][l];
                        else       DataMin.dblStress[i][j][k][l+1] = dblStressMin[m][k][l];
                    }

					// Combi. 다시 계산.
                    // component 12 : Sx, Sy, Sz, Byp, Bym, Bzp, Bzm, Comb, Comb1, Comb2, Comb3, Comb4
					double dblStressMax01 = (fabs(DataMax.dblStress[i][j][k][8] ) >= fabs(DataMax.dblStress[i][j][k][9]) ) ? DataMax.dblStress[i][j][k][8]  : DataMax.dblStress[i][j][k][9];
					double dblStressMax02 = (fabs(DataMax.dblStress[i][j][k][10]) >= fabs(DataMax.dblStress[i][j][k][11])) ? DataMax.dblStress[i][j][k][10] : DataMax.dblStress[i][j][k][11];
                    DataMax.dblStress[i][j][k][7] = (fabs(dblStressMax01) >= fabs(dblStressMax02)) ? dblStressMax01 : dblStressMax02;

					double dblStressMin01 = (fabs(DataMin.dblStress[i][j][k][8] ) >= fabs(DataMin.dblStress[i][j][k][9]) ) ? DataMin.dblStress[i][j][k][8]  : DataMin.dblStress[i][j][k][9];
					double dblStressMin02 = (fabs(DataMin.dblStress[i][j][k][10]) >= fabs(DataMin.dblStress[i][j][k][11])) ? DataMin.dblStress[i][j][k][10] : DataMin.dblStress[i][j][k][11];
					DataMin.dblStress[i][j][k][7] = (fabs(dblStressMin01) >= fabs(dblStressMin02)) ? dblStressMin01 : dblStressMin02;
                }
            }
        }

        // DataMax.dblStress2[4][2][11][12]
        memset(DataMax.dblStress2, 0, sizeof(DataMax.dblStress2));
        memset(DataMin.dblStress2, 0, sizeof(DataMin.dblStress2));

//         for(int i=0; i<4; i++)
//         {
//             for(int j=0; j<2; j++)
//             {
//                 int m=i+j;
//                 for(int k=0; k<9; k++)
//                 {
//                     for (int l=0; l<11; l++)
//                     {
//                         if(l < 7)  DataMax.dblStress2[i][j][k][l]   = dblStress2Max[m][k][l];
//                         else       DataMax.dblStress2[i][j][k][l+1] = dblStress2Max[m][k][l];
//                     }
// 					for(int l=0; l<11; l++)
//                     {
//                         if(l < 7)  DataMin.dblStress2[i][j][k][l]   = dblStress2Min[m][k][l];
//                         else       DataMin.dblStress2[i][j][k][l+1] = dblStress2Min[m][k][l];
//                     }
// 
// 					// Combi. 다시 계산.
//                     // component 12 : Sx, Sy, Sz, Byp, Bym, Bzp, Bzm, Comb, Comb1, Comb2, Comb3, Comb4
// 					double dblStressMax01 = (fabs(DataMax.dblStress2[i][j][k][8] ) >= fabs(DataMax.dblStress2[i][j][k][9]) ) ? DataMax.dblStress2[i][j][k][8]  : DataMax.dblStress2[i][j][k][9];
// 					double dblStressMax02 = (fabs(DataMax.dblStress2[i][j][k][10]) >= fabs(DataMax.dblStress2[i][j][k][11])) ? DataMax.dblStress2[i][j][k][10] : DataMax.dblStress2[i][j][k][11];
//                     DataMax.dblStress2[i][j][k][7] = (fabs(dblStressMax01) >= fabs(dblStressMax02)) ? dblStressMax01 : dblStressMax02;
// 
// 					double dblStressMin01 = (fabs(DataMin.dblStress2[i][j][k][8] ) >= fabs(DataMin.dblStress2[i][j][k][9])) ? DataMin.dblStress2[i][j][k][8] : DataMin.dblStress2[i][j][k][9];
// 					double dblStressMin02 = (fabs(DataMin.dblStress2[i][j][k][10]) >= fabs(DataMin.dblStress2[i][j][k][11])) ? DataMin.dblStress2[i][j][k][10] : DataMin.dblStress2[i][j][k][11];
// 					DataMin.dblStress2[i][j][k][7] = (fabs(dblStressMin01) >= fabs(dblStressMin02)) ? dblStressMin01 : dblStressMin02;
//                 }
//             }
//         }
	} // ConvertOut

    // View by max stress table 출력
	void ConvertOut2(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
	{
        // DataMax.dblStress[4][2][9][40];
        memset(DataMax.dblStress, 0, sizeof(DataMax.dblStress));
        memset(DataMin.dblStress, 0, sizeof(DataMin.dblStress));

        // DataMax.dblStress2[4][2][11][12]
        memset(DataMax.dblStress2, 0, sizeof(DataMax.dblStress2));
        memset(DataMin.dblStress2, 0, sizeof(DataMin.dblStress2));

        for(int i=0; i<4; i++)
        {
            for(int j=0; j<2; j++)
            {
                int m=i+j;
                for(int k=0; k<9; k++)
                {
                    for (int l=0; l<11; l++)
                    {
                        if(l < 7)  DataMax.dblStress[i][j][k][l]   = dblStress2Max[m][k][l];  // dblStress2Max[5][11][11];
                        else       DataMax.dblStress[i][j][k][l+1] = dblStress2Max[m][k][l];
                    }
					for(int l=0; l<11; l++)
                    {
                        if(l < 7)  DataMin.dblStress[i][j][k][l]   = dblStress2Min[m][k][l];
                        else       DataMin.dblStress[i][j][k][l+1] = dblStress2Min[m][k][l];
                    }

					// Combi. 다시 계산.
                    // component 12 : Sx, Sy, Sz, Byp, Bym, Bzp, Bzm, Comb, Comb1, Comb2, Comb3, Comb4
					double dblStressMax01 = (fabs(DataMax.dblStress[i][j][k][8] ) >= fabs(DataMax.dblStress[i][j][k][9]) ) ? DataMax.dblStress[i][j][k][8]  : DataMax.dblStress[i][j][k][9];
					double dblStressMax02 = (fabs(DataMax.dblStress[i][j][k][10]) >= fabs(DataMax.dblStress[i][j][k][11])) ? DataMax.dblStress[i][j][k][10] : DataMax.dblStress[i][j][k][11];
                    DataMax.dblStress[i][j][k][7] = (fabs(dblStressMax01) >= fabs(dblStressMax02)) ? dblStressMax01 : dblStressMax02;

					double dblStressMin01 = (fabs(DataMin.dblStress[i][j][k][8] ) >= fabs(DataMin.dblStress[i][j][k][9])) ? DataMin.dblStress[i][j][k][8] : DataMin.dblStress[i][j][k][9];
					double dblStressMin02 = (fabs(DataMin.dblStress[i][j][k][10]) >= fabs(DataMin.dblStress[i][j][k][11])) ? DataMin.dblStress[i][j][k][10] : DataMin.dblStress[i][j][k][11];
					DataMin.dblStress[i][j][k][7] = (fabs(dblStressMin01) >= fabs(dblStressMin02)) ? dblStressMin01 : dblStressMin02;
                }
            }
        }
	} // ConvertOut2

};

struct _PCFM_MV_BN
{
	// dblForce     : local unit length force     , cent + 4 node       ,
	//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

	double dForceMax[5][8][8];
	double dForceMin[5][8][8];

	void ConvertToSpcf(T_SPCF_DL& DataMax, T_SPCF_DL& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();

		for(int i=0; i<5; i++)
		{
			for(int j=0; j<8; j++)
			{
				for(int k=0; k<8; k++)
				{
					DataMax.dForce[i][j][k] = dForceMax[i][j][k];
					DataMin.dForce[i][j][k] = dForceMin[i][j][k];
				}
			}
		}
	}
};

struct _SIMC_MV_BN
{
    // dblForce     : local force,  min, max, 2 point
    //                component : Fx, Fy, Fz, Mx, My, Mz
    double dblForceMax[2][6][6];
    double dblForceMin[2][6][6];
    void ConvertOut(T_SIMC_D& DataMax, T_SIMC_D& DataMin)
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
};

// BS Code Special Vehicle Force
struct _MBSB_MV_BN
{
	// dblForce     : local force,  min, max, 5 point
	//                component : Fx, Fy, Fz, Mx, My, Mz
	double dblForceMax[5][6];
	double dblForceMin[5][6];
	int iel;

	void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();

		for(int j=0; j<4; j++)
		{
			for(int k=0; k<6; k++) DataMax.dblForce[j][  k] = dblForceMax[j  ][k];
			for(int k=0; k<6; k++) DataMax.dblForce[j][6+k] = dblForceMax[j+1][k];

			for(int k=0; k<6; k++) DataMin.dblForce[j][  k] = dblForceMin[j  ][k];
			for(int k=0; k<6; k++) DataMin.dblForce[j][6+k] = dblForceMin[j+1][k];
		}
	}
};

// BS Code Special Vehicle Force - 7DOF
struct _MWSB_MV_BN
{
	// dblForce     : local force,  min, max, 5 point
	//                component : Fx, Fy, Fz, Mx, My, Mz, BI, ST, WarpingT
	double dblForceMax[5][9];
	double dblForceMin[5][9];
	int iel;

	void ConvertOut(T_STRB_D& DataMax, T_STRB_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();

		for(int j=0; j<4; j++)
		{
			for(int k=0; k<6; k++) DataMax.dblForce[j][  k] = dblForceMax[j  ][k];
			for(int k=0; k<6; k++) DataMax.dblForce[j][6+k] = dblForceMax[j+1][k];
			for(int k=0; k<3; k++) DataMax.dblForce[j][12+k*2  ] = dblForceMax[j  ][6+k];
			for(int k=0; k<3; k++) DataMax.dblForce[j][12+k*2+1] = dblForceMax[j+1][6+k];

			for(int k=0; k<6; k++) DataMin.dblForce[j][  k] = dblForceMin[j  ][k];
			for(int k=0; k<6; k++) DataMin.dblForce[j][6+k] = dblForceMin[j+1][k];
			for(int k=0; k<3; k++) DataMin.dblForce[j][12+k*2  ] = dblForceMin[j  ][6+k];
			for(int k=0; k<3; k++) DataMin.dblForce[j][12+k*2+1] = dblForceMin[j+1][6+k];
		}
	}
};

// BS Code Special Vehicle Concurrent Force
struct _MBSV_MV_BN
{
	// dblForce     : local force,  min, max, 5 point
	//                component : Fx, Fy, Fz, Mx, My, Mz
	double dblForceMax[5][6][6];
	double dblForceMin[5][6][6];
	int iel;

	void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();

		for(int i=0; i<4; i++)
		{
			for(int j=0; j<6; j++)
			{
				for(int k=0; k<6; k++)
				{
					DataMax.dblForce[i][0][j][k] = dblForceMax[i  ][j][k];
					DataMax.dblForce[i][1][j][k] = dblForceMax[i+1][j][k];

					DataMin.dblForce[i][0][j][k] = dblForceMin[i  ][j][k];
					DataMin.dblForce[i][1][j][k] = dblForceMin[i+1][j][k];
				}
			}
		}
	}
};

// BS Code Special Vehicle Concurrent Force - 7DOF
struct _MWSV_MV_BN
{
	// dblForce     : local force,  min, max, 5 point
	//                component : Fx, Fy, Fz, Mx, My, Mz, BI, ST, WarpingT
	double dblForceMax[5][9][9];
	double dblForceMin[5][9][9];
	int iel;

	void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();

		for(int i=0; i<4; i++)
		{
			for(int j=0; j<9; j++)
			{
				for(int k=0; k<9; k++)
				{
					DataMax.dblForce[i][0][j][k] = dblForceMax[i  ][j][k];
					DataMax.dblForce[i][1][j][k] = dblForceMax[i+1][j][k];

					DataMin.dblForce[i][0][j][k] = dblForceMin[i  ][j][k];
					DataMin.dblForce[i][1][j][k] = dblForceMin[i+1][j][k];
				}
			}
		}
	}
};

// section part beam concurrent force
struct _STCV_CS_BN
{
    double dblForceMax[4][2][3][3];  // Fx, My, Mz
    double dblForceMin[4][2][3][3];
    void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
    {
        DataMax.Initialize();
        DataMin.Initialize();
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<2; j++)
            {
                for(int k=0; k<3; k++)
                {
                    int l;
                    if(k == 0)l=0;
                    if(k == 1)l=4;
                    if(k == 2)l=5;
                    DataMax.dblForce[i][j][l][0]=dblForceMax[i][j][k][0];
                    DataMax.dblForce[i][j][l][4]=dblForceMax[i][j][k][1];
                    DataMax.dblForce[i][j][l][5]=dblForceMax[i][j][k][2];
                    DataMin.dblForce[i][j][l][0]=dblForceMin[i][j][k][0];
                    DataMin.dblForce[i][j][l][4]=dblForceMin[i][j][k][1];
                    DataMin.dblForce[i][j][l][5]=dblForceMin[i][j][k][2];
                }
            }
        }
    }
    void Add(_STCV_CS_BN& Data)
    {
        for(int i=0; i<4; i++)
            for(int j=0; j<2; j++)
                for(int k=0; k<3; k++)
                    for(int l=0; l<3; l++)
                    {
                        dblForceMax[i][j][k][l]+=Data.dblForceMax[i][j][k][l];
                        dblForceMin[i][j][k][l]+=Data.dblForceMin[i][j][k][l];
                    }
    }
};
struct _STCV_MV_BN
{
    double dblForceMax[5][3][3];  // Fx, My, Mz
    double dblForceMin[5][3][3];
    void ConvertOut(T_SBCF_D& DataMax, T_SBCF_D& DataMin)
    {
        DataMax.Initialize();
        DataMin.Initialize();
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<2; j++)
            {
                int m=i+j;
                for(int k=0; k<3; k++)
                {
                    int l=0;
                    if(k == 1)l=4;
                    if(k == 2)l=5;
                    DataMax.dblForce[i][j][l][0]=dblForceMax[m][k][0];
                    DataMax.dblForce[i][j][l][4]=dblForceMax[m][k][1];
                    DataMax.dblForce[i][j][l][5]=dblForceMax[m][k][2];
                    DataMin.dblForce[i][j][l][0]=dblForceMin[m][k][0];
                    DataMin.dblForce[i][j][l][4]=dblForceMin[m][k][1];
                    DataMin.dblForce[i][j][l][5]=dblForceMin[m][k][2];
                }
            }
        }
    }
    void Add(_STCV_MV_BN& Data)
    {
        for(int i=0; i<5; i++)
            for(int j=0; j<3; j++)
                for(int k=0; k<3; k++)
                {
                    dblForceMax[i][j][k]+=Data.dblForceMax[i][j][k];
                    dblForceMin[i][j][k]+=Data.dblForceMin[i][j][k];
                }
    }
};
#define _STCV_SM_BN _STCV_MV_BN

// Beam
struct _NLBM_TH_BN
{
    double dblStress[26];
    double dblCombStrs[2][4];
    void ConvertOut(T_STRB_D& Data)
    {
        Data.Initialize();
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
                Data.dblForce[j][k]   = dblStress[k] + (dblStress[k+6]-dblStress[k])*dIRatio;
                Data.dblForce[j][k+6] = dblStress[k] + (dblStress[k+6]-dblStress[k])*dJRatio;
            } 
            for(k=0; k<7; k++)
            {
                nIIdx = k+12; nJIdx = k+12+7;
                Data.dblStress[j][k]   = dblStress[nIIdx] + (dblStress[nJIdx]-dblStress[nIIdx])*dIRatio;
                Data.dblStress[j][k+7] = dblStress[nIIdx] + (dblStress[nJIdx]-dblStress[nIIdx])*dJRatio;
            }

            Data.dblStress[j][16]=dblCombStrs[0][0] +(dblCombStrs[1][0] - dblCombStrs[0][0])*dIRatio;
            Data.dblStress[j][18]=dblCombStrs[0][1] +(dblCombStrs[1][1] - dblCombStrs[0][1])*dIRatio;
            Data.dblStress[j][20]=dblCombStrs[0][2] +(dblCombStrs[1][2] - dblCombStrs[0][2])*dIRatio;
            Data.dblStress[j][22]=dblCombStrs[0][3] +(dblCombStrs[1][3] - dblCombStrs[0][3])*dIRatio;

            Data.dblStress[j][17]=dblCombStrs[0][0] +(dblCombStrs[1][0] - dblCombStrs[0][0])*dJRatio;
            Data.dblStress[j][19]=dblCombStrs[0][1] +(dblCombStrs[1][1] - dblCombStrs[0][1])*dJRatio;
            Data.dblStress[j][21]=dblCombStrs[0][2] +(dblCombStrs[1][2] - dblCombStrs[0][2])*dJRatio;
            Data.dblStress[j][23]=dblCombStrs[0][3] +(dblCombStrs[1][3] - dblCombStrs[0][3])*dJRatio;
        }
        Data.calcMaximumCombinedStress();
    }
};

// Time History Large Displacements 고려시 7자유도 Beam 결과
struct _GLBW_TH_BN
{
    // dblStress    : local force, stress
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
    double dblStress[72];
    double dblCombStrs[2][4];  // Cb1, Cb2, Cb3, Cb4
    double dblCombStrsEtc[2][8];
    void ConvertOut(T_STRB_D& DataStrb)
    {
        DataStrb.Initialize();

        DataStrb.nDOFType=2;

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
                DataStrb.dblForce[j][k]   = dblStress[k] + (dblStress[k+6]-dblStress[k])*dIRatio;
                DataStrb.dblForce[j][k+6] = dblStress[k] + (dblStress[k+6]-dblStress[k])*dJRatio;
            } 

            // 7자유도 결과 Force/Moment. (Mb: Bi-Moment, Mt: Twisting Moment, Mw: Warping Moment)
            int iMb = 12;     // Mb 결과 시작 index
            int iMt = iMb+2;  // =14;  
            int iMw = iMt+2;  // =16;  
            for (k=iMb; k<iMb+6; k+=2)
            {
                DataStrb.dblForce[j][k  ]   = dblStress[k] + (dblStress[k+1]-dblStress[k])*dIRatio;   // i-max
                DataStrb.dblForce[j][k+1]   = dblStress[k] + (dblStress[k+1]-dblStress[k])*dJRatio;   // j-max
            }

            // 6자유도 결과 Stress.
            int iStress = 12+6;  // 12: Force & Moment, 6: 7th DOF Moment
            for(k=0; k<7; k++)
            {
                nIIdx = k+iStress;  nJIdx = nIIdx+7;   // 7: Stress의 i단 결과 갯수(=j단 결과 갯수)
                DataStrb.dblStress[j][k]   = dblStress[nIIdx] + (dblStress[nJIdx]-dblStress[nIIdx])*dIRatio;
                DataStrb.dblStress[j][k+7] = dblStress[nIIdx] + (dblStress[nJIdx]-dblStress[nIIdx])*dJRatio;
            }

            // Combined Stress
            int iCombStrs  = 14;   // T_STRB_D에서 Combined Stress의 시작 index
            int iCombStrs1 = iCombStrs+2; // T_STRB_D에서 Comb1i의 시작 index
            for (k=iCombStrs1; k<iCombStrs1+8; k+=2)
            {
                for (int m=0; m<4; ++m)
                {
                    DataStrb.dblStress[j][k  ]=dblCombStrs[0][m] +(dblCombStrs[1][m] - dblCombStrs[0][m])*dIRatio;
                    DataStrb.dblStress[j][k+1]=dblCombStrs[0][m] +(dblCombStrs[1][m] - dblCombStrs[0][m])*dJRatio;
                }
            }

            // Warping Normal Stress
            int iWarpNormalStrs = 32;   // Warping Normal Stress I-end 결과의 시작 index
            int index = 0;
            for (k=0; k<20; ++k)
            {
                nIIdx = k+iWarpNormalStrs;  nJIdx = nIIdx+20;   // 20: Warping Stress의 i단 결과 갯수(=j단 결과 갯수)
                index = k+24;                                   // 24: T_STRB_D에서 Warping Stress의 시작 index
                DataStrb.dblStress[j][index   ] = dblStress[nIIdx] + (dblStress[nJIdx]-dblStress[nIIdx])*dIRatio;
                DataStrb.dblStress[j][index+20] = dblStress[nIIdx] + (dblStress[nJIdx]-dblStress[nIIdx])*dJRatio;
            }

            // CombTau1i, CombTau1j .... CombTau8i, CombTau8j (for 7th DOF : 16개)
            int iCombTau  = 64;   // T_STRB_D에서 CombTau의 시작 index
            for (int m=0; m<8; ++m)
            {
                index = 2*m + iCombTau;

                DataStrb.dblStress[j][index  ]=dblCombStrsEtc[0][m] +(dblCombStrsEtc[1][m] - dblCombStrsEtc[0][m])*dIRatio;
                DataStrb.dblStress[j][index+1]=dblCombStrsEtc[0][m] +(dblCombStrsEtc[1][m] - dblCombStrsEtc[0][m])*dJRatio;
            }
        }

        DataStrb.calcMaximumCombinedStress();
    }
};

// wall
// _NLWL_TH_BN
struct _STWL_TH_BN
{
    // dblForce     : local force         , top, bot
    //                component : Fx, Fy, Fz, Mx, My, Mz
    double dblForce[2][6];
    void ConvertOut(T_STRW_D& Data)
    {
        Data.Initialize();
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<6; j++) Data.dblForce[i][j] = dblForce[i][j];
        }
    }
};

struct _NLTF_TH_BN
{
    double dblValue[2]; // 0:부재력, 1:응력
    void ConvertOut(T_STRT_D& Data)
    {
        Data.Initialize();
        Data.dblForce[0] = Data.dblForce[1]  = dblValue[0];
        Data.dblStress[0] = Data.dblStress[1]  = dblValue[1];
    }
};

struct _NLHG_TH_BN
{
    float   dForce;
    float   dDispl;
    float   dDuctility_Pos;      // Crack Tension
    float   dDuctility_Neg;      // Crack Compression 
    float   dYeildDuctility_Pos; // Yield Tension
    float   dYeildDuctility_Neg; // Yield Compression
    float   dHingeStatusLevel_Pos;
    float   dHingeStatusLevel_Neg;
    float   dHingeStatus_Pos;    // Linear:1.0, Crack:2.0, Yield=3.0, Ultimate:4.0
    float   dHingeStatus_Neg;    // Linear:-1.0, Crack:-2.0, Yield=-3.0, Ultimate:-4.0
    float   dD1[2]; //0:Pos, 1:Neg
    float   dD2[2]; //0:Pos, 1:Neg
    float   dD3[2]; //0:Pos, 1:Neg
};

struct _PNHG_TH_BN
{
    float   dForce;
    float   dDispl;
    float   dDuctility_Pos;      // Crack Tension
    float   dDuctility_Neg;      // Crack Compression 
    float   dYeildDuctility_Pos; // Yield Tension
    float   dYeildDuctility_Neg; // Yield Compression
    float   dHingeStatus_Pos;    // Linear:1.0, Crack:2.0, Yield=3.0, Ultimate:4.0
    float   dHingeStatus_Neg;    // Linear:-1.0, Crack:-2.0, Yield=-3.0, Ultimate:-4.0
};

struct _HGSM_TH_BN
{
    float dMaxVal[16];    // Force,Deformation,Ductility Pos,Ductility Neg, Duct Crack Pos, Duct Crack Neg, StatusLevel_Pos, StatusLevel_Neg, Status_Pos, Status_Neg, D1[0], D1[1], D2[0], D2[1], D3[0], D3[1]
    float dMinVal[16];    // Force,Deformation,Ductility Pos,Ductility Neg, Duct Crack Pos, Duct Crack Neg, StatusLevel_Pos, StatusLevel_Neg, Status_Pos, Status_Neg, D1[0], D1[1], D2[0], D2[1], D3[0], D3[1]
    float dMaxTime[16];   // Force,Deformation,Ductility Pos,Ductility Neg, Duct Crack Pos, Duct Crack Neg, StatusLevel_Pos, StatusLevel_Neg, Status_Pos, Status_Neg, D1[0], D1[1], D2[0], D2[1], D3[0], D3[1]
    float dMinTime[16];   // Force,Deformation,Ductility Pos,Ductility Neg, Duct Crack Pos, Duct Crack Neg, StatusLevel_Pos, StatusLevel_Neg, Status_Pos, Status_Neg, D1[0], D1[1], D2[0], D2[1], D3[0], D3[1]
    float dYieldTime[3];  // Crack Time,Yield Time, Ultimate Time 
};


struct _STRP2_BN
{
// dblForce     : local unit length force     , cent + 4 node       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

    double dblForceMax[5][8];
    double dblStressMax[5][6];
    double dblForceMin[5][8];
    double dblStressMin[5][6];
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
				for(int k=0; k<8; k++)
				{
					DataMax.dForce[i][j][k] = dblForceMax[i][k];
					DataMin.dForce[i][j][k] = dblForceMin[i][k];
				}				
			}
		}
	}
};

struct _STRP3_BN
{
// dblNodalForce : Nodal Force (4*6)
// dblForce      : local unit length force     , cent + 4 node       ,
//                 component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY
// dblUnitForce  : Unit force (5*8) , 8 node

	double dblNodalForceMax[4][6];
	double dblUnitForceMax[5][8];
    double dblForceMax[5][14];     // 6(Stress)+8(Force)

	double dblNodalForceMin[4][6];
	double dblUnitForceMin[5][8];
    double dblForceMin[5][14];     // 6(Stress)+8(Force)

	void ConvertToSpcf(T_SPCF_DL& DataMax, T_SPCF_DL& DataMin)
	{
		DataMax.Initialize();
		DataMin.Initialize();

		for(int i=0; i<5; i++)
		{
			for(int j=0; j<8; j++)
			{
				for(int k=0; k<8; k++)
				{
					DataMax.dForce[i][j][0] = dblForceMax[i][6];
					DataMax.dForce[i][j][1] = dblForceMax[i][7];
					DataMax.dForce[i][j][2] = dblForceMax[i][8];
					DataMax.dForce[i][j][3] = dblForceMax[i][9];
					DataMax.dForce[i][j][4] = dblForceMax[i][10];
					DataMax.dForce[i][j][5] = dblForceMax[i][11];
					DataMax.dForce[i][j][6] = dblForceMax[i][12];
					DataMax.dForce[i][j][7] = dblForceMax[i][13];

					DataMin.dForce[i][j][0] = dblForceMin[i][6];
					DataMin.dForce[i][j][1] = dblForceMin[i][7];
					DataMin.dForce[i][j][2] = dblForceMin[i][8];
					DataMin.dForce[i][j][3] = dblForceMin[i][9];
					DataMin.dForce[i][j][4] = dblForceMin[i][10];
					DataMin.dForce[i][j][5] = dblForceMin[i][11];
					DataMin.dForce[i][j][6] = dblForceMin[i][12];
					DataMin.dForce[i][j][7] = dblForceMin[i][13];
				}				
			}
		}
	}
};

struct _STRP31_BN
{
// dblForce     : local unit length force     , cent + 4 node       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

    double iel;
    double ino[4];
    double dblForce[5][8];
    double dblStress[5][6];
};

struct _STRP31_FLOAT_BN
{
// dblForce     : local unit length force     , cent + 4 node       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

    float iel;
    float ino[4];
    float dblForce[5][8];
    float dblStress[5][6];
    void ConvertToDouble(_STRP31_BN& Data)
    {
        Data.iel=iel;
        for(int i=0; i<4; i++)Data.ino[i]=ino[i];
        for(int i=0; i<5; i++)
        {
            for(int j=0; j<8; j++)Data.dblForce[i][j]=dblForce[i][j];
            for(int j=0; j<6; j++)Data.dblStress[i][j]=dblStress[i][j];
        }
    }
};

struct _STRP32_BN
{
// dblForce     : local unit length force     , cent       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

    double iel;
    double ino[4];
    double dblForce[1][8];
    double dblStress[1][6];
};

struct _STRP32_FLOAT_BN
{
// dblForce     : local unit length force     , cent       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

    float iel;
    float ino[4];
    float dblForce[1][8];
    float dblStress[1][6];
    void ConvertToDouble(_STRP32_BN& Data)
    {
        Data.iel=iel;
        for(int i=0; i<4; i++)Data.ino[i]=ino[i];
        for(int i=0; i<1; i++)
        {
            for(int j=0; j<8; j++)Data.dblForce[i][j]=dblForce[i][j];
            for(int j=0; j<6; j++)Data.dblStress[i][j]=dblStress[i][j];
        }
    }
};

struct _STRP33_BN
{
// dblForce     : local unit length force     , cent + 4 node       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

    double iel;
    double ino[4];
    double dblForce[5][8];
};

struct _STRP33_FLOAT_BN
{
// dblForce     : local unit length force     , cent + 4 node       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

    float iel;
    float ino[4];
    float dblForce[5][8];
    void ConvertToDouble(_STRP33_BN& Data)
    {
        Data.iel=iel;
        for(int i=0; i<4; i++)Data.ino[i]=ino[i];
        for(int i=0; i<5; i++)
        {
            for(int j=0; j<8; j++)Data.dblForce[i][j]=dblForce[i][j];
        }
    }
};

struct _STRP34_BN
{
// dblForce     : local unit length force     , cent + 4 node       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

    double iel;
    double ino[4];
    double dblForce[1][8];
};

struct _STRP34_FLOAT_BN
{
// dblForce     : local unit length force     , cent + 4 node       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY

    float iel;
    float ino[4];
    float dblForce[1][8];
    void ConvertToDouble(_STRP34_BN& Data)
    {
        Data.iel=iel;
        for(int i=0; i<4; i++)Data.ino[i]=ino[i];
        for(int i=0; i<1; i++)
        {
            for(int j=0; j<8; j++)Data.dblForce[i][j]=dblForce[i][j];
        }
    }
};
struct _STRS2_BN
{
// dblForce     : local unit length force     , cent + 4 node       ,
//                component : FXX, FYY, FXY, MXX, MYY, MXY, VXX, VYY
// dblForce  : global force , 8 node       , component : FX, FY, FZ
// dblStress : global stress, cent + 8 node, component : SXX, SYY, SZZ, SXY, SYZ, SXZ
    double dblForceMax[8][3];
    double dblStressMax[1+8][6];
    double dblForceMin[8][3];
    double dblStressMin[1+8][6];
    int iel;
    int ino[8];
    void ConvertOut(T_STRS_D& DataMax, T_STRS_D& DataMin)
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
};

struct _STRS_IF_FLOAT_BN
{
    int iel;
    int ino[8];
    float dblForce[8][3];
    float dblStress[1+8][6];
    void ConvertToStrs(T_STRS_D& Data)
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

// Elastic Link
struct _STEK2_BN
{
// dblForce  : local force max, min, 2 node       , component : fx, fy, fz, mx, my, mz
    double dblForceMax[2][6];
    double dblForceMin[2][6];
    int iel;
    int ino[2];
};
struct _STEK3_BN
{
// dblForce  : local force, 2 node       , component : fx, fy, fz, mx, my, mz
	int iel;
	int ino[2];
	float dblForce[2][6];

	void ConvertToStek(T_STEK_D& rData)
	{
        rData.Initialize();

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 6; j++)
			{
                rData.dblForce[i][j] = dblForce[i][j];
			}
		}
	}
};
struct _STEK3_FLOAT_BN
{
// dblForce  : local force, 1 node       , component : fx, fy, fz, mx, my, mz
    float iel;
    float ino[2];
    float dblForce[6];
};

// Elastic Link - Concurrent
struct _STCK_BN_MV
{
    // dblForce  : local force max, min, 2 node       , component : fx, fy, fz, mx, my, mz
    double dblForceMax[2][6][6];
    double dblForceMin[2][6][6];

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
                    DataMax.dblForce[i][j][k]=dblForceMax[i][j][k];
                    DataMin.dblForce[i][j][k]=dblForceMin[i][j][k];
                }
            }
        }
    }
};


// story shear
struct _STSM_BN
{
// dStoryShear
//                component : Max-X, Min-X, MaxTime-X, MinTime-X, 
//                            Max-Y, Min-Y, MaxTime-Y, MinTime-Y, 
    double dStoryShear[8];
};

struct _HGST_BN
{
    int nElem;
    int nHingeStatus[5][3];
};

struct _NDSP_OLD_BN
{
    double dEquation[6];
    double dTranMatrix[21];
};

struct _NDSP_BN
{
    double dEquation[7];
    double dTranMatrix[21];
    void ConvertToNdsp(_NDSP_OLD_BN& Data)
    {
        for(int i=0; i<6; i++) dEquation[i]=Data.dEquation[i];
		dEquation[6]=0.;
		memcpy(&dTranMatrix,&Data.dTranMatrix,sizeof(dTranMatrix));
    }
};

struct T_SSRTDATA_D
{
    unsigned int nPosition;
    T_STOR_K KeyStor;
};

struct T_STRDDATA_D
{
    unsigned int nPosition;
    T_STOR_K KeyStor_cur;
    T_STOR_K KeyStor_up;
};

struct _SSRT_BN
{
    double nType; // 1=Frame 2=벽체 3=스프링 4=ElaticLink 5=Frame Sum 6=Wall Sum 7=Planar 8=Solid 9=Planar Sum 10=Solid Sun
    double nId;   // 요소번호, 벽체번호, 스프링 순번, 링크 순번
    double dShearX;
    double dShearY;
};

struct _STEC_INFO_BN
{
    double dData[2];
};

struct _ITTR_BN
{
    double dForce[2]; //I, J단부 축력
};

struct _ITBM_BN
{
    double dForce[2][6]; //I, J단부 부재력
};

struct _ITBW_BN
{
    double dForce[2][7]; //I, J단부 부재력
};

struct _GRUP_NODE_BN
{
    CArray<int, int> arNode;

    void Initialize()
    {
        arNode.RemoveAll();
    }

	_GRUP_NODE_BN() {}
	_GRUP_NODE_BN(_GRUP_NODE_BN& GrupNode)	{*this = GrupNode;}
	_GRUP_NODE_BN& operator = (_GRUP_NODE_BN& GrupNode)
	{
		Initialize();
        arNode.Copy(GrupNode.arNode);
		return *this;
	}
};

struct _GRUP_ELEM_BN_BASE
{
	int nElemK;
	int nType;  // 1:트러스, 2:보  3:보(7dof)
	int nPos;   // 1:I   2:J

	void Initialize()
	{
		nElemK = 0;
		nType = 0;
		nPos = 0;
	}
};

struct _GRUP_ELEM_BN
{
	CArray<_GRUP_ELEM_BN_BASE, _GRUP_ELEM_BN_BASE> arElem;

	void Initialize()
	{
		arElem.RemoveAll();
	}

	_GRUP_ELEM_BN() {}
	_GRUP_ELEM_BN(_GRUP_ELEM_BN& GrupElem) { *this = GrupElem; }
	_GRUP_ELEM_BN& operator = (_GRUP_ELEM_BN& GrupElem)
	{
		Initialize();
		arElem.Copy(GrupElem.arElem);
		return *this;
	}
};

struct _SOTF_BN
{
//                component : Max-X, Min-X, MaxTime-X, MinTime-X, 
//                            Max-Y, Min-Y, MaxTime-Y, MinTime-Y, 
    double dStoryOverMoment[8];
};

#pragma pack()

#include "DB_ST_POST_READ_STLD.h"
#include "DB_ST_POST_READ_EIGV.h"
#include "DB_ST_POST_READ_BUCK.h"
#include "DB_ST_POST_READ_SPEC.h"
#include "DB_ST_POST_READ_THIS.h"
#include "DB_ST_POST_READ_IFMV.h"
#include "DB_ST_POST_READ_IFGR.h"
#include "DB_ST_POST_READ_IGLD.h"
#include "DB_ST_POST_READ_POVR.h"
#include "DB_ST_POST_READ_NONL.h"
#include "DB_ST_POST_READ_RAEM.h"
#include "DB_ST_POST_READ_SMLC.h"
#include "DB_ST_POST_READ_STAG.h"
#include "DB_ST_POST_READ_HYLD.h"
#include "DB_ST_POST_READ_PCDN.h"
#include "DB_ST_POST_READ_SPBR.h"

#endif