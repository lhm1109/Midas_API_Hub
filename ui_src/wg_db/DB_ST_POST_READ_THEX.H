#ifndef __DB_ST_POST_READ_THEX_H__
#define __DB_ST_POST_READ_THEX_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

#define _REAC_THEX_BN _REAC_STLD_BN
#define _LRAC_THEX_BN _LRAC_STLD_BN
#define _DISP_THEX_BN _DISP_STLD_BN
#define _STRT_THEX_BN _STRT_STLD_BN

struct _STRB_THEX_BN 
{
    double dblStress[26];
    double dCombStrs[2][4];
    int iel;

    void ConvertOut(T_STRB_D& DataStrb)
    {
        DataStrb.Initialize();
        
        int     nIIdx,nJIdx;
        double  dIRatio,dJRatio;
        for(int j=0; j<4; j++)
        {
            dIRatio = ((double)j)/4.0;
            dJRatio = ((double)(j+1))/4.0;

            for(int k=0; k<6; k++)
            {
                DataStrb.dblForce[j][k]   = dblStress[k] + (dblStress[k+6]-dblStress[k])*dIRatio;
                DataStrb.dblForce[j][k+6] = dblStress[k] + (dblStress[k+6]-dblStress[k])*dJRatio;
            }
            for(int k=0; k<7; k++)
            {
                nIIdx = k+12; nJIdx = k+12+7;
                DataStrb.dblStress[j][k]   = dblStress[nIIdx] + (dblStress[nJIdx]-dblStress[nIIdx])*dIRatio;
                DataStrb.dblStress[j][k+7] = dblStress[nIIdx] + (dblStress[nJIdx]-dblStress[nIIdx])*dJRatio;
            }

            DataStrb.dblStress[j][16]=dCombStrs[0][0] +(dCombStrs[1][0] - dCombStrs[0][0])*dIRatio;
            DataStrb.dblStress[j][18]=dCombStrs[0][1] +(dCombStrs[1][1] - dCombStrs[0][1])*dIRatio;
            DataStrb.dblStress[j][20]=dCombStrs[0][2] +(dCombStrs[1][2] - dCombStrs[0][2])*dIRatio;
            DataStrb.dblStress[j][22]=dCombStrs[0][3] +(dCombStrs[1][3] - dCombStrs[0][3])*dIRatio;

            DataStrb.dblStress[j][17]=dCombStrs[0][0] +(dCombStrs[1][0] - dCombStrs[0][0])*dJRatio;
            DataStrb.dblStress[j][19]=dCombStrs[0][1] +(dCombStrs[1][1] - dCombStrs[0][1])*dJRatio;
            DataStrb.dblStress[j][21]=dCombStrs[0][2] +(dCombStrs[1][2] - dCombStrs[0][2])*dJRatio;
            DataStrb.dblStress[j][23]=dCombStrs[0][3] +(dCombStrs[1][3] - dCombStrs[0][3])*dJRatio;
        }
        DataStrb.calcMaximumCombinedStress();
    }
    void ConvertIn(T_STRB_D& DataStrb)
    {
        memset(dblStress,0,sizeof(dblStress));
        memset(dCombStrs,0,sizeof(dCombStrs));
        
        for(int k=0; k<6; k++)dblStress[k]      = DataStrb.dblForce[0][k];
        for(int k=0; k<6; k++)dblStress[6+k]    = DataStrb.dblForce[3][6+k];

        for(int l=0; l<7; l++)dblStress[l+12]   = DataStrb.dblStress[0][l];
        for(int l=0; l<7; l++)dblStress[l+12+7] = DataStrb.dblStress[3][6+l];
                
        dCombStrs[0][0] = DataStrb.dblStress[0][16];
        dCombStrs[0][1] = DataStrb.dblStress[0][18];
        dCombStrs[0][2] = DataStrb.dblStress[0][20];
        dCombStrs[0][3] = DataStrb.dblStress[0][22];

        dCombStrs[1][0] = DataStrb.dblStress[3][17];
        dCombStrs[1][1] = DataStrb.dblStress[3][19];
        dCombStrs[1][2] = DataStrb.dblStress[3][21];
        dCombStrs[1][3] = DataStrb.dblStress[3][23];
    }
};

#define _STRM_THEX_BN _STRM_STLD_BN
#define _STRP_THEX_BN _STRP_STLD_BN
#define _STRS_THEX_BN _STRS_STLD_BN
#define _STPN_THEX_BN _STPN_STLD_BN
#define _STRW_THEX_BN _STRW_STLD_BN
#define _STPN_THEX_BN _STPN_STLD_BN
#define _STAX_THEX_BN _STAX_STLD_BN
#define _ELNK_THEX_BN _ELNK_STLD_BN
#define _NLNK_THEX_BN _NLNK_STLD_BN

struct _THEX_THEX_BN
{
    int     nStep;
    char    strName[32];
};

struct _EXST_THEX_BN
{
    double  dStepVal;
    char    strName[32];
};

struct _THAM_THEX_BN
{
    int     nStep;
    char    strName[32];
};

struct _AMST_THEX_BN
{
    double  dStepVal;
    char    strName[32];
};

struct _THPO_THEX_BN
{
    int     nStep;
    char    strName[32];
};

struct _POST_THEX_BN
{
    double  dStepVal;
    char    strName[32];
};

// Nonlinear Event Bar 
struct _EVTB_THEX_BN
{
    int nEvent[2][6]; 
};

// Nonliear Event Rod 
struct _EVTR_THEX_BN
{
    int nEvent[2];
};

// Nonliear Event Spring
struct _EVTS_THEX_BN
{
    int nEvent[2][6];
};

#pragma pack()
#endif