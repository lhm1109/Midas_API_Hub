#ifndef __DB_ST_POST_READ_POVR_H__
#define __DB_ST_POST_READ_POVR_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

#define _REAC_POVR_BN _REAC_STLD_BN
#define _LRAC_POVR_BN _LRAC_STLD_BN
#define _DISP_POVR_BN _DISP_STLD_BN
#define _STRT_POVR_BN _STRT_STLD_BN
#define _STRB_POVR_BN _STRB_STLD_BN
#define _STRM_POVR_BN _STRM_STLD_BN
#define _STRP_POVR_BN _STRP_STLD_BN
#define _STRS_POVR_BN _STRS_STLD_BN
#define _STRW_POVR_BN _STRW_STLD_BN
#define _ELNK_POVR_BN _ELNK_STLD_BN
#define _NLNK_POVR_BN _NLNK_STLD_BN
struct _HGST_POVR_BN
{
    int nElem;
    int nHingeStatus[5][3];
    void ConvertOut(T_HGST_D& Data)
    {
        Data.Initialize();
        ASSERT(sizeof(Data.nHingeStatus) == sizeof(nHingeStatus));
        memcpy(Data.nHingeStatus, nHingeStatus, sizeof(Data.nHingeStatus));
    }
};

struct _GLHP_POVR_BN
{
    int nGnlkSerial;
    int nHingeStatus[6];
    int nTotalHingeNum;
    void ConvertOut(T_GLHP_D& Data)
    {
        Data.Initialize();
        ASSERT(sizeof(Data.nHingeStatus) == sizeof(nHingeStatus));
        memcpy(Data.nHingeStatus, nHingeStatus, sizeof(Data.nHingeStatus));
        Data.nTotalHingeNum = nTotalHingeNum;
    }
};

struct _PNHG_POVR_BN
{
	// double 19개
    // Force, Deform, Plastic Deform, D/D1(+), D/D1(-), D/D2(+), D/D2(-), <- 7개
    // Hinge Status(+), Hinge Status(-), 1st Str(+), 1st Str(-), 2nd Str(+), 2nd Str(-), <- 6개
    // 1st Def(+), 1st Def(-), 2nd Def(+), 2nd Def(-) <- 4개 
    // Stiff, Plastic Deformation (D - D Elas) <- 2개 : 총 19개
    double   dForce;
    double   dDispl;
	double   dPlastDispl;
    double   dDuctility_Pos;      // Crack Tension
    double   dDuctility_Neg;      // Crack Compression 
    double   dYieldDuctility_Pos; // Yield Tension
    double   dYieldDuctility_Neg; // Yield Compression
    double   dHingeStatus_Pos;    // Linear:1.0, Crack:2.0, Yield=3.0, Ultimate:4.0
    double   dHingeStatus_Neg;    // Linear:-1.0, Crack:-2.0, Yield=-3.0, Ultimate:-4.0
	double   dYieldStrength_Pos_1st;
	double   dYieldStrength_Neg_1st;
	double   dYieldStrength_Pos_2nd;
	double   dYieldStrength_Neg_2nd;
	double   dYieldDeform_Pos_1st;
	double   dYieldDeform_Neg_1st;
	double   dYieldDeform_Pos_2nd;
	double   dYieldDeform_Neg_2nd;
	double   dInitStiff;
    double   dNewPlasticDeformation; //820ver 에서 추가된 data. 
    double   dHingePerformance_Pos;  // Hinge Performance in (+) direction (v865에 추가)
    double   dHingePerformance_Neg;  // Hinge Performance in (+) direction (v865에 추가)
};

// MNET:4242-GYKIM-20101230 : Pushover 성능개선
// float 형의 _PNHG_POVR_BN와 동일한 Data. 솔버에서 읽어오기 위한 format.
struct _PNHG_POVR_BN_F
{
    // float형 19개
    float   dForce;
    float   dDispl;
    float   dPlastDispl;
    float   dDuctility_Pos;      
    float   dDuctility_Neg;      
    float   dYieldDuctility_Pos; 
    float   dYieldDuctility_Neg; 
    float   dHingeStatus_Pos;    
    float   dHingeStatus_Neg;    
    float   dYieldStrength_Pos_1st;
    float   dYieldStrength_Neg_1st;
    float   dYieldStrength_Pos_2nd;
    float   dYieldStrength_Neg_2nd;
    float   dYieldDeform_Pos_1st;
    float   dYieldDeform_Neg_1st;
    float   dYieldDeform_Pos_2nd;
    float   dYieldDeform_Neg_2nd;
    float   dInitStiff;
    float   dNewPlasticDeformation; //820ver 에서 추가된 data. 
    float   dHingePerformance_Pos;  // Hinge Performance in (+) direction (v865에 추가)
    float   dHingePerformance_Neg;  // Hinge Performance in (+) direction (v865에 추가)
};

struct _FHNE_POVR_BN
{
    // 1st Yield Step No, 2nd Yield Step No
    double   dNumStepYild[2];
};

struct _STRY_POVR_BN
{
    // Story Drift X, Story Drift Y
    double   dStoryDrift[2];
};

struct _FHYS_POVR_BN
{
    // [0] : 힌지의 최종상태 Index, [1] : Yield Step / Uplifting Step
    int nStepYild[2];
};

struct _PHIF_POVR_BN
{
	double dForce;
};

#define _SSRT_POVR_BN _SSRT_STLD_BN

#pragma pack()

#endif


