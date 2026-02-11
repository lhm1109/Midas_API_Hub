#ifndef __DB_ST_DN_RESULT_SOD_H__
#define __DB_ST_DN_RESULT_SOD_H__

#define MIDAS_SDK

#include "DB_ST_DT_UNIT.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

///////////////////////////////////////////////////////////////
// Steel Orthotropic Deck
///////////////////////////////////////////////////////////////

struct T_SODC_ITEM
{
    BOOL bSodCheck[8]; // 0: STR-TF  1: STR-BF  2: STR-Web
                                         // 3: LTB     4: STB-TF  5: STB-BF  6: STB-Web
	                   // 7: FAT 
    void Initialize()
    {
        memset(bSodCheck, 0, sizeof(bSodCheck));
    }
    T_SODC_ITEM() {Initialize();}
};

struct T_SDLS_NUM_B
{
    int nFlgNum;
    int nLStfnNum;
    int nPanNum;
    void Initialize()
    {
        nFlgNum   = 0;
        nLStfnNum = 0;
        nPanNum   = 0;
    }
    T_SDLS_NUM_B() {Initialize();}
};

struct T_SDLS_NUM_INFO
{
    int nTopFlgNum;
    int nBotFlgNum;
    int nTopNum;
    int nBotNum;
    int nWebNum;
    int nPanTNum;
    int nPanBNum;
    int nPanWNum;
    T_SDLS_NUM_B LST[3]; // Top
    T_SDLS_NUM_B LSB[3]; // Bot
    T_SDLS_NUM_B LSW[2]; // Web

    void Initialize()
    {
        nTopFlgNum = 0;
        nBotFlgNum = 0;
        nTopNum  = 0;
        nBotNum  = 0;
        nWebNum  = 0;
        nPanTNum = 0;
        nPanBNum = 0;
        nPanWNum = 0;
        for (int i=0; i<3; ++i) 
        {
            LST[i].Initialize();
            LSB[i].Initialize();
        }
        for (int i=0; i<2; ++i) LSW[i].Initialize();
    }
    T_SDLS_NUM_INFO() {Initialize();}
};

struct T_SDLS_NUM_ELEM
{
    T_SDLS_NUM_INFO LsD[2];  // 0=I, 1=J
    void Initialize()
    {
        for (int i=0; i<2; ++i) LsD[i].Initialize();
    }
    T_SDLS_NUM_ELEM() {Initialize();}
};

struct T_SODD_FORCE
{
    double dForce[6];
    
    void Initialize()
    {
        memset(dForce,0,sizeof(dForce));
    }
    T_SODD_FORCE() {Initialize();}
};

struct T_SODD_FORCE_UNIT
{
    T_SODD_FORCE_UNIT()
    {
        dForce[0] = D_UNITSYS_BASE_FORCE;       
        dForce[1] = D_UNITSYS_BASE_FORCE;   
        dForce[2] = D_UNITSYS_BASE_FORCE;       
        dForce[3] = D_UNITSYS_BASE_MOMENT;   
        dForce[4] = D_UNITSYS_BASE_MOMENT;       
        dForce[5] = D_UNITSYS_BASE_MOMENT;   
    }
    int dForce[6];
};

struct T_SODD_ULMF
{
    T_SODD_FORCE For[3]; // I,M,J
    
    void Initialize()
    {
        for (int i=0; i<3; ++i)
        {
            For[i].Initialize();
        }
    }
    T_SODD_ULMF() {Initialize();}
};

struct T_SODD_DGN_FORCE
{
    T_SODD_FORCE Elem[2];
    T_SODD_FORCE ElemDL[2];
    T_SODD_ULMF  Ulmf;

    void Initialize()
    {
        for(int i=0; i<2; ++i) 
        {
            Elem[i].Initialize();
            ElemDL[i].Initialize();
        }
        Ulmf.Initialize();
    }

    T_SODD_DGN_FORCE() {Initialize();}
};

// Member Force for 5Postion
struct T_SODD_FORCE_EX
{
	T_SODD_FORCE ForD[5]; // I-End, 1/4, 2/4, 3/4, J-End

	void Initialize()
	{
		for(int i=0; i<5; ++i) 
		{
			ForD[i].Initialize();
		}
	}

	T_SODD_FORCE_EX() {Initialize();}
};

// Longitudinal Stiffener
struct T_SDLS_STRN_FLG
{
    BOOL   bChk;     // 검토여부
    int    nABPoint; // LS Bottom
    double dSigxc_r;
    double dSigxp_r;
    double dSigx_r;
    double dRyr;     // Ry*m Appendix 18 식(5),(6),(7)
    double dRatioRib;
    BOOL   bOKrib;

    int    nAB1point; // LS Deck
    double dSigx_d;
    double dSigy_d;
    double dTauxy_d;
    double dSigma_d;
    double dRyd;      // Ry*m Appendix 18 식(9)
    double dRatioDeck;
    BOOL   bOKdeck;
    double dRatioMax;
    BOOL   bOK;


    void Initialize()
    {
        bChk       = FALSE;
        nABPoint   = 0;
        dSigxc_r   = 0.0;
        dSigxp_r   = 0.0;
        dSigx_r    = 0.0;
        dRyr       = 0.0;
        dRatioRib  = 0.0;
        bOKrib     = FALSE;
        nAB1point  = 0;
        dSigx_d    = 0.0;
        dSigy_d    = 0.0;
        dTauxy_d   = 0.0;
        dSigma_d   = 0.0;
        dRyd       = 0.0;
        dRatioDeck = 0.0;
        bOKdeck    = FALSE;
        dRatioMax  = 0.0;
        bOK        = FALSE;

    }

    T_SDLS_STRN_FLG() {Initialize();}
};

struct T_SDLS_STRN_FLG_UNIT
{
    T_SDLS_STRN_FLG_UNIT()
    {
        dSigxc_r = D_UNITSYS_BASE_STRESS;
        dSigxp_r = D_UNITSYS_BASE_STRESS;
        dSigx_r  = D_UNITSYS_BASE_STRESS;
        dRyr     = D_UNITSYS_BASE_STRESS;    
        dSigx_d  = D_UNITSYS_BASE_STRESS;
        dSigy_d  = D_UNITSYS_BASE_STRESS;
        dTauxy_d = D_UNITSYS_BASE_STRESS;
        dSigma_d = D_UNITSYS_BASE_STRESS;
        dRyd     = D_UNITSYS_BASE_STRESS;
    }
    int dSigxc_r;
    int dSigxp_r;
    int dSigx_r;
    int dRyr;    
    int dSigx_d;
    int dSigy_d;
    int dTauxy_d;
    int dSigma_d;
    int dRyd;

};
typedef CArray<T_SDLS_STRN_FLG,T_SDLS_STRN_FLG&> LSSTRNFLG_ARR;

// Part Strength of Section
// Flange의 Left, Middle, Right의 각 파트 데이터
struct T_SDPT_STRN_FLG
{
    BOOL bChk;             // 검토여부
    LSSTRNFLG_ARR aLSChk;
    int  nRib_cr;          // part에서 가장 불리한 rib 번호
    int  nDeck_cr;         // part에서 가장 불리한 plate 번호
    int  nFB_cr;           // part에서 가장 불리한 Floorbeam 번호 

    // 추가
    BOOL bChk_Deck; 
    BOOL bChk_Rib;
    BOOL bChk_FB; 
    double dRat_Deck;
    double dRat_Rib;
    double dRat_FB;
    double dRat_Max;
    BOOL bOK;

    void Initialize()
    {
        bChk = FALSE;
        aLSChk.RemoveAll();
        nRib_cr  = 0;
        nDeck_cr = 0;
        nFB_cr   = 0;

        bChk_Deck= FALSE;
        bChk_Rib = FALSE;
        bChk_FB  = FALSE; 
        dRat_Deck=0.0;
        dRat_Rib =0.0;
        dRat_FB  =0.0;
        dRat_Max =0.0;
        bOK      =FALSE;

    }
    T_SDPT_STRN_FLG() {Initialize();}
    T_SDPT_STRN_FLG(const T_SDPT_STRN_FLG &rData)	{*this = rData;}
    T_SDPT_STRN_FLG& operator = (const T_SDPT_STRN_FLG &rData)
    {
        bChk    = rData.bChk;
        aLSChk.Copy(rData.aLSChk);
        nRib_cr  = rData.nRib_cr;
        nDeck_cr = rData.nDeck_cr;
        nFB_cr   = rData.nFB_cr;

        bChk_Deck = rData.bChk_Deck;
        bChk_Rib  = rData.bChk_Rib ;
        bChk_FB   = rData.bChk_FB  ;
        dRat_Deck = rData.dRat_Deck;
        dRat_Rib  = rData.dRat_Rib ;
        dRat_FB   = rData.dRat_FB  ;
        dRat_Max  = rData.dRat_Max ;
        bOK       = rData.bOK      ;
        return *this;
    }
};

struct T_SDPT_STRN_FLG_RW
{
    BOOL bChk;             // 검토여부
    int  nLSChkSize;
    int  nRib_cr;          // part에서 가장 불리한 rib 번호
    int  nDeck_cr;         // part에서 가장 불리한 plate 번호
    int  nFB_cr;           // part에서 가장 불리한 Floorbeam 번호 

    // 추가
    BOOL bChk_Deck; 
    BOOL bChk_Rib;
    BOOL bChk_FB; 
    double dRat_Deck;
    double dRat_Rib;
    double dRat_FB;
    double dRat_Max;
    BOOL bOK;

    void Initialize()
    {
        bChk = FALSE;
        nLSChkSize=0;
        nRib_cr  = 0;
        nDeck_cr = 0;
        nFB_cr   = 0;

        bChk_Deck= FALSE;
        bChk_Rib = FALSE;
        bChk_FB  = FALSE; 
        dRat_Deck=0.0;
        dRat_Rib =0.0;
        dRat_FB  =0.0;
        dRat_Max =0.0;
        bOK      =FALSE;

    }
    T_SDPT_STRN_FLG_RW() {Initialize();}
    void GetData(T_SDPT_STRN_FLG &rData)
    {
        rData.bChk     = bChk;
        rData.aLSChk.SetSize(nLSChkSize);
        rData.nRib_cr  = nRib_cr;
        rData.nDeck_cr = nDeck_cr;
        rData.nFB_cr   = nFB_cr;

        rData.bChk_Deck = bChk_Deck;
        rData.bChk_Rib  = bChk_Rib ;
        rData.bChk_FB   = bChk_FB  ;
        rData.dRat_Deck = dRat_Deck;
        rData.dRat_Rib  = dRat_Rib ;
        rData.dRat_FB   = dRat_FB  ;
        rData.dRat_Max  = dRat_Max ;
        rData.bOK       = bOK      ;

    }
    void SetData(T_SDPT_STRN_FLG &rData)
    {
        bChk       = rData.bChk;
        nLSChkSize = (int)rData.aLSChk.GetSize();
        nRib_cr    = rData.nRib_cr;
        nDeck_cr   = rData.nDeck_cr;
        nFB_cr     = rData.nFB_cr;

        bChk_Deck = rData.bChk_Deck;
        bChk_Rib  = rData.bChk_Rib ;
        bChk_FB   = rData.bChk_FB  ;
        dRat_Deck = rData.dRat_Deck;
        dRat_Rib  = rData.dRat_Rib ;
        dRat_FB   = rData.dRat_FB  ;
        dRat_Max  = rData.dRat_Max ;
        bOK       = rData.bOK      ;
    }
};
// Flange Strength Check
struct T_SDSN_FLG
{
    T_SDPT_STRN_FLG StrnChk[3]; // I-L,R // Box-L,M,R
    int nPart_Rib_cr;           // Rib Ratio 가 큰 Part
    int nPart_Deck_cr;          // Deck Ratio 가 큰 Part

    void Initialize()
    {
        for (int i=0; i<3; ++i)
        {
            StrnChk[i].Initialize();
        }
        nPart_Rib_cr  = 0;
        nPart_Deck_cr = 0;

    }
    T_SDSN_FLG() {Initialize();}
    T_SDSN_FLG(const T_SDSN_FLG &rData)	{*this = rData;}
    T_SDSN_FLG& operator = (const T_SDSN_FLG &rData)
    {
        for (int i=0; i<3; ++i)
        {
            StrnChk[i] = rData.StrnChk[i];
        }
        nPart_Rib_cr  = rData.nPart_Rib_cr;
        nPart_Deck_cr = rData.nPart_Deck_cr;

        return *this;
    }

};

struct T_SDSN_FLG_RW
{
    T_SDPT_STRN_FLG_RW StrnChk[3]; //
    int nPart_Rib_cr;           // Rib Ratio 가 큰 Part
    int nPart_Deck_cr;          // Deck Ratio 가 큰 Part

    void Initialize()
    {
        for (int i=0; i<3; ++i)
        {
            StrnChk[i].Initialize();
        }
        nPart_Rib_cr  = 0;
        nPart_Deck_cr = 0;

    }
    T_SDSN_FLG_RW() {Initialize();}
    void GetData(T_SDSN_FLG &rData)
    {
        for (int i=0; i<3; ++i)
        {
            StrnChk[i].GetData(rData.StrnChk[i]);
        }
        rData.nPart_Rib_cr  = nPart_Rib_cr;
        rData.nPart_Deck_cr = nPart_Deck_cr;
    }

    void SetData(T_SDSN_FLG &rData)
    {
        for (int i=0; i<3; ++i)
        {
            StrnChk[i].SetData(rData.StrnChk[i]);
        }
        nPart_Rib_cr  = rData.nPart_Rib_cr;
        nPart_Deck_cr = rData.nPart_Deck_cr;
    }
};
// Web Strength check

// Web Position Check (Longitudinal Stiffener )
struct T_SDPS_STRN_WEB
{
    BOOL   bChk;     // 검토여부
    double dSigx;
    double dSigy;
    double dTauxy;
    double dSigsum;  // 4.31절 식(161)

    double dRatioSig;
    double dRatioTau;
    double dRatioMax;
    BOOL   bOK_Sigsum;
    BOOL   bOK_Tauxy;  
    BOOL   bOK;

    void Initialize()
    {
        bChk    = FALSE;
        dSigx   = 0.0;
        dSigy   = 0.0;
        dTauxy  = 0.0;
        dSigsum = 0.0;

        dRatioSig  = 0.0;
        dRatioTau  = 0.0;
        dRatioMax  = 0.0;
        bOK_Sigsum = FALSE;
        bOK_Tauxy  = FALSE;
        bOK        = FALSE;    
    }

    T_SDPS_STRN_WEB() {Initialize();}
};

struct T_SDPS_STRN_WEB_UNIT
{
    T_SDPS_STRN_WEB_UNIT()
    {
        dSigx   = D_UNITSYS_BASE_STRESS;
        dSigy   = D_UNITSYS_BASE_STRESS;
        dTauxy  = D_UNITSYS_BASE_STRESS;
        dSigsum = D_UNITSYS_BASE_STRESS;
    }
    int dSigx;
    int dSigy;
    int dTauxy;
    int dSigsum;  // 4.31절 식(161)
};

typedef CArray<T_SDPS_STRN_WEB,T_SDPS_STRN_WEB&> WEBSTRNCHK_ARR;

// Web Part Strength
// Webn의 Left, Right 각각의 데이터
struct T_SDPT_STRN_WEB
{
    BOOL   bChk;             // 검토여부
    double dTau_min[2];      // 0=Top, 1=Bottom
    double dTau_max;         // Centroid
    double dTau_M0;          // 4.30절 식(159)
    double dRatioTau;
    BOOL   bOK_tauM0;
    double dRyr;             // Gamma*Ry*m, 4.31절 식(161)
    double dRsr;             // Rs*m 
    T_SDPS_STRN_WEB PosChk[3];  // 0=Top, 1=Centroid, 2=Bottom

    // For Steel Composite Max Shear Stress Check
    double dTau_str;
    double dmRs;

    WEBSTRNCHK_ARR  aWebChk; // LS 개수 만큼 
    int nRib_cr;             // 가장 불리한 rib 번호

    // 추가
    BOOL bChk_DeckNoM; // 4.30
    BOOL bChk_DeckVon; // 4.31
    BOOL bChk_DeckCom;
    BOOL bChk_DeckMean;
    BOOL bChk_Deck;  // max(4.30, 4.31)
    BOOL bChk_Rib;
    double dRat_DeckNoM;
    double dRat_DeckVon;
    double dRat_DeckCom;
    double dRat_Mean;
    double dRat_Deck;
    double dRat_Rib;
    double dRat_Max; // max(dRat_Deck, dRat_Rib)

    BOOL bOK;
    BOOL bOK_DeckNoM; // 4.31
    BOOL bOK_DeckVon; // 4.31
    BOOL bOK_DeckCom;
    BOOL bOK_DeckMean;
    BOOL bOK_Deck;  // max(4.30, 4.31)
    BOOL bOK_Rib;
    void Initialize()
    {
        bChk      = FALSE;
        dTau_min[0] = 0.0;
        dTau_min[1] = 0.0;
        dTau_max  = 0.0;
        dTau_M0   = 0.0;
        dRatioTau = 0.0;
        bOK_tauM0 = FALSE;
        dRyr      = 0.0;
        dRsr      = 0.0;
        for (int i=0; i<3; ++i) PosChk[i].Initialize();
        dTau_str = 0.0;
        dmRs =0.0;
        aWebChk.RemoveAll();
        nRib_cr = 0;

        bChk_DeckNoM = FALSE; 
        bChk_DeckVon = FALSE; 
        bChk_DeckCom = FALSE;
        bChk_DeckMean = FALSE;
        bChk_Deck = FALSE; 
        bChk_Rib  = FALSE;
        dRat_DeckNoM =0.0;
        dRat_DeckVon =0.0;
        dRat_DeckCom = 0.0;
        dRat_Mean = 0.0;
        dRat_Deck =0.0;
        dRat_Rib  =0.0;
        dRat_Max  =0.0;
        bOK = FALSE;
        bOK_DeckNoM = FALSE;
        bOK_DeckVon = FALSE;
        bOK_DeckCom = FALSE;
        bOK_DeckMean = FALSE;
        bOK_Deck = FALSE;
        bOK_Rib = FALSE;
    }
    T_SDPT_STRN_WEB() {Initialize();}
    T_SDPT_STRN_WEB(const T_SDPT_STRN_WEB &rData)	{*this = rData;}
    T_SDPT_STRN_WEB& operator = (const T_SDPT_STRN_WEB &rData)
    {
        bChk      = rData.bChk;
        memset(dTau_min,0,sizeof(dTau_min));
        dTau_max  = rData.dTau_max;
        dTau_M0   = rData.dTau_M0;
        dRatioTau = rData.dRatioTau;
        bOK_tauM0 = rData.bOK_tauM0;
        dRyr      = rData.dRyr;
        dRsr      = rData.dRsr;
        for (int i=0; i<3; ++i) PosChk[i] = rData.PosChk[i];
        dTau_str = rData.dTau_str;
        dmRs =rData.dmRs;
        aWebChk.Copy(rData.aWebChk);
        nRib_cr   = rData.nRib_cr;

        bChk_DeckNoM = rData.bChk_DeckNoM;
        bChk_DeckVon = rData.bChk_DeckVon;
        bChk_DeckCom = rData.bChk_DeckCom;
        bChk_DeckMean = rData.bChk_DeckMean;
        bChk_Deck = rData.bChk_Deck;
        bChk_Rib  = rData.bChk_Rib ;
        dRat_DeckNoM = rData.dRat_DeckNoM;
        dRat_DeckVon = rData.dRat_DeckVon;
        dRat_DeckCom = rData.dRat_DeckCom;
        dRat_Mean = rData.dRat_Mean;
        dRat_Deck = rData.dRat_Deck;
        dRat_Rib  = rData.dRat_Rib ;
        dRat_Max   = rData.dRat_Max;
        bOK        = rData.bOK;
        bOK_DeckNoM = rData.bOK_DeckNoM;
        bOK_DeckVon = rData.bOK_DeckVon;
        bOK_DeckCom = rData.bOK_DeckCom;
        bOK_DeckMean = rData.bOK_DeckMean;
        bOK_Deck = rData.bOK_Deck;
        bOK_Rib = rData.bOK_Rib;
        return *this;
    }
};

struct T_SDPT_STRN_WEB_RW
{
    BOOL   bChk;             // 검토여부
    double dTau_min[2];      // 0=Top, 1=Bottom
    double dTau_max;         // Centroid
    double dTau_M0;          // 4.30절 식(159)
    double dRatioTau;
    BOOL   bOK_tauM0;
    double dRyr;             // Gamma*Ry*m, 4.31절 식(161)
    double dRsr;             // Rs*m 
    T_SDPS_STRN_WEB PosChk[3];  // 0=Top, 1=Centroid, 2=Bottom

    int nWebChkSize;         // WEBSTRNCHK_ARR  aWebChk; // LS 개수 만큼 
    int nRib_cr;             // 가장 불리한 rib 번호

    // 추가
    BOOL bChk_DeckNoM; 
    BOOL bChk_DeckVon; 
    BOOL bChk_Deck; 
    BOOL bChk_Rib;
    double dRat_DeckNoM;
    double dRat_DeckVon;
    double dRat_Deck;
    double dRat_Rib;
    double dRat_Max;
    BOOL bOK;

    void Initialize()
    {
        bChk      = FALSE;
        dTau_max  = 0.0;
        dTau_M0   = 0.0;
        dRatioTau = 0.0;
        bOK_tauM0 = FALSE;
        dRyr      = 0.0;
        dRsr      = 0.0;
        for (int i=0; i<3; ++i) PosChk[i].Initialize();
        nWebChkSize = 0;
        nRib_cr = 0;

        bChk_DeckNoM = FALSE; 
        bChk_DeckVon = FALSE; 
        bChk_Deck = FALSE; 
        bChk_Rib  = FALSE;
        dRat_DeckNoM =0.0;
        dRat_DeckVon =0.0;
        dRat_Deck =0.0;
        dRat_Rib  =0.0;
        dRat_Max =0.0;
        bOK = FALSE;

    }
    T_SDPT_STRN_WEB_RW() {Initialize();}

    void GetData(T_SDPT_STRN_WEB &rData)
    {
        rData.bChk      = bChk;
        memcpy(rData.dTau_min,dTau_min,sizeof(rData.dTau_min));
        rData.dTau_max  = dTau_max;
        rData.dTau_M0   = dTau_M0;
        rData.dRatioTau = dRatioTau;
        rData.bOK_tauM0 = bOK_tauM0;
        rData.dRyr      = dRyr;
        rData.dRsr      = dRsr;
        for (int i=0; i<3; ++i) rData.PosChk[i] = PosChk[i];
        rData.aWebChk.SetSize(nWebChkSize);
        rData.nRib_cr   = nRib_cr;

        rData.bChk_DeckNoM    = bChk_DeckNoM ;
        rData.bChk_DeckVon    = bChk_DeckVon ;
        rData.bChk_Deck    = bChk_Deck ;
        rData.bChk_Rib     = bChk_Rib  ;
        rData.dRat_DeckNoM    = dRat_DeckNoM ;
        rData.dRat_DeckVon    = dRat_DeckVon ;
        rData.dRat_Deck    = dRat_Deck ;
        rData.dRat_Rib     = dRat_Rib  ;
        rData.dRat_Max   = dRat_Max;
        rData.bOK        = bOK;
    }

    void SetData(T_SDPT_STRN_WEB &rData)
    {
        bChk      = rData.bChk;
        memcpy(dTau_min,rData.dTau_min,sizeof(dTau_min));
        dTau_max  = rData.dTau_max;
        dTau_M0   = rData.dTau_M0;
        dRatioTau = rData.dRatioTau;
        bOK_tauM0 = rData.bOK_tauM0;
        dRyr      = rData.dRyr;
        dRsr      = rData.dRsr;
        for (int i=0; i<3; ++i) PosChk[i] = rData.PosChk[i];
        nRib_cr   = rData.nRib_cr;    
        nWebChkSize = (int)rData.aWebChk.GetSize();

        bChk_DeckNoM    = rData.bChk_DeckNoM ;
        bChk_DeckVon    = rData.bChk_DeckVon ;
        bChk_Deck    = rData.bChk_Deck ;
        bChk_Rib     = rData.bChk_Rib  ;
        dRat_DeckNoM    = rData.dRat_DeckNoM ;
        dRat_DeckVon    = rData.dRat_DeckVon ;
        dRat_Deck    = rData.dRat_Deck ;
        dRat_Rib     = rData.dRat_Rib  ;
        dRat_Max     = rData.dRat_Max;
        bOK          = rData.bOK;
    }  
};

struct T_SDPT_STRN_WEB_UNIT
{
    T_SDPT_STRN_WEB_UNIT()
    {
        dTau = D_UNITSYS_BASE_STRESS;
        dRyr = D_UNITSYS_BASE_STRESS;
        dRsr = D_UNITSYS_BASE_STRESS;
    }
    int dTau;        
    int dRyr;
    int dRsr;

};
// Web Strength Check
struct T_SDSN_WEB
{
    T_SDPT_STRN_WEB WebD[2]; // Left / Right

    void Initialize()
    {
        for (int i=0; i<2; ++i)
        {
            WebD[i].Initialize();
        }
    }
    T_SDSN_WEB() {Initialize();}
    T_SDSN_WEB(const T_SDSN_WEB &rData)	{*this = rData;}
    T_SDSN_WEB& operator = (const T_SDSN_WEB &rData)
    {
        for (int i=0; i<2; ++i)
        {
            WebD[i] = rData.WebD[i];
        }

        return *this;
    }

};

struct T_SDSN_WEB_RW
{
    T_SDPT_STRN_WEB_RW WebD[2]; //

    void Initialize()
    {
        for (int i=0; i<2; ++i)
        {
            WebD[i].Initialize();
        }
    }
    T_SDSN_WEB_RW() {Initialize();}

    void GetData(T_SDSN_WEB &rData)
    {
        for (int i=0; i<2; ++i)
        {
            WebD[i].GetData(rData.WebD[i]);
        }
    }

    void SetData(T_SDSN_WEB &rData)
    {
        for (int i=0; i<2; ++i)
        {
            WebD[i].SetData(rData.WebD[i]);
        }
    }

};





// Stability
struct T_SDSB_PAN
{
    double dBTRa;    // 0.951*alpha~
    double dRatio;   // (b/t) / (dBTRa)
    BOOL   bChkBtr;  // 4.45 판단

    void Initialize()
    {
        dBTRa   = 0.0;
        dRatio  = 0.0;
        bChkBtr = FALSE;
    }
    T_SDSB_PAN() {Initialize();}
};

struct T_SDSB_SECT_NOLS
{
    // 종방향 보강재 또는 보강재가 없는 주단면에 사용 
    // Flat : 0=Web
    // I형  : 0=TL, 1=TR, 2=BL, 3=BR, 4=Web
    // T형  : 0=TL, 1=TR, 2=Web
    // Box형: 0=TL, 1=TInt, 2=TR, 3=BL, 4=BR, 5=BInt, 6=LWeb , 7=RWeb
    // U-Rib : 0=BInt, 1=LWeb, 2=RWeb
    T_SDSB_PAN PanChk[8];
    int        nChkIx;      // 불리한 위치 Index
    void Initialize()
    {
        for (int i=0; i<8; ++i)    PanChk[i].Initialize();
        nChkIx = 0;
    }
    T_SDSB_SECT_NOLS() {Initialize();}

};

typedef CArray<T_SDSB_SECT_NOLS,T_SDSB_SECT_NOLS&> LSSTAB_ARR;



// Part Stability of Section
struct T_SDPT_STAB_FLG
{
    BOOL bChk;             // 검토여부
    LSSTAB_ARR aLSChk;
    CArray<T_SDSB_PAN,T_SDSB_PAN&> aPanChk;  // Rib사이 검토 
    int  nDeckG_cr;    // 파트의 dRat_DeckG중  불리한 것       
    int  nDeckL_cr;    // 파트의 dRat_DeckL중  불리한 것 
    int  nRib_cr;      // 파트의 리브중 불리한 것
    int  nRibPan_cr;   // nRib_cr에서 불리한 plate

    int  nFBG_cr;      //  C1, C2, C3 중 하나. for dRat_FBG
    int  nFBL_cr;      //  C1, C2, C3 중 하나. for dRat_FBL
    int  nFBLPan_cr;   //  nFBL_cr 에서 불리한 plate

    // 추가
    double dRat_DeckG;  // Deck-Global
    double dRat_DeckL;  // Deck-Local(BTR) 
    double dRat_Deck;   // max
    double dRat_Rib;
    double dRat_FBG;    // FloorBeam - Global 
    double dRat_FBL;    // FloorBeam - Local(BTR) 
    double dRat_FB;     // max
    double dRat_Max;

    BOOL bChk_DeckG;
    BOOL bChk_DeckL;
    BOOL bChk_Deck;
    BOOL bChk_Rib ;
    BOOL bChk_FBG;
    BOOL bChk_FBL;
    BOOL bChk_FB;
    BOOL bOK;  

    void Initialize()
    {
        bChk = FALSE;
        aLSChk.RemoveAll();
        aPanChk.RemoveAll();
        nDeckG_cr  = 0;
        nDeckL_cr  = 0;
        nRib_cr  = 0;
        nRibPan_cr = 0;
        nFBG_cr  = 0;
        nFBL_cr  = 0;
        nFBLPan_cr= 0;

        dRat_DeckG =0.0;
        dRat_DeckL =0.0;
        dRat_Deck  =0.0;
        dRat_Rib   =0.0;
        dRat_FBG   =0.0;
        dRat_FBL   =0.0;
        dRat_FB    =0.0;
        dRat_Max   =0.0;
        bChk_DeckG = FALSE;
        bChk_DeckL = FALSE;
        bChk_Deck  = FALSE;
        bChk_Rib   = FALSE;  
        bChk_FBG   = FALSE;
        bChk_FBL   = FALSE;
        bChk_FB    = FALSE;
        bOK        = FALSE;  
    }
    T_SDPT_STAB_FLG() {Initialize();}
    T_SDPT_STAB_FLG(const T_SDPT_STAB_FLG &rData)	{*this = rData;}
    T_SDPT_STAB_FLG& operator = (const T_SDPT_STAB_FLG &rData)
    {
        bChk    = rData.bChk;
        aLSChk.Copy(rData.aLSChk);
        aPanChk.Copy(rData.aPanChk);
        nDeckG_cr  = rData.nDeckG_cr;
        nDeckL_cr  = rData.nDeckL_cr;
        nRib_cr   = rData.nRib_cr;
        nRibPan_cr = rData.nRibPan_cr;
        nFBG_cr   = rData.nFBG_cr;
        nFBL_cr   = rData.nFBL_cr;
        nFBLPan_cr= rData.nFBLPan_cr;

        dRat_DeckG  = rData.dRat_DeckG ;
        dRat_DeckL  = rData.dRat_DeckL ;
        dRat_Deck   = rData.dRat_Deck  ;
        dRat_Rib    = rData.dRat_Rib   ;
        dRat_FBG    = rData.dRat_FBG   ;
        dRat_FBL    = rData.dRat_FBL   ;
        dRat_FB     = rData.dRat_FB    ;
        dRat_Max    = rData.dRat_Max   ;
        bChk_DeckG  = rData.bChk_DeckG ;
        bChk_DeckL  = rData.bChk_DeckL ;
        bChk_Deck   = rData.bChk_Deck  ;
        bChk_Rib    = rData.bChk_Rib   ;
        bChk_FBG    = rData.bChk_FBG   ;
        bChk_FBL    = rData.bChk_FBL   ;
        bChk_FB     = rData.bChk_FB    ;
        bOK         = rData.bOK        ;

        return *this;
    }
};

struct T_SDPT_STAB_FLG_RW
{
    BOOL bChk;             // 검토여부
    int  nLSChkSize;       // LSSTAB_ARR aLSChk;
    int  nPanChkSize;      // CArray<T_SDSB_PAN,T_SDSB_PAN&> aPanChk;  // Rib사이 검토 
    int  nDeckG_cr;          
    int  nDeckL_cr;          
    int  nRib_cr;
    int nRibPan_cr;
    int  nFBG_cr; //  C1, C2, C3 중 하나. for dRat_FBG
    int  nFBL_cr; //  C1, C2, C3 중 하나. for dRat_FBL
    int nFBLPan_cr;

    // 추가
    double dRat_DeckG;  // Deck-Global
    double dRat_DeckL;  // Deck-Local(BTR) 
    double dRat_Deck;   // max
    double dRat_Rib;
    double dRat_FBG;    // FloorBeam - Global 
    double dRat_FBL;    // FloorBeam - Local(BTR) 
    double dRat_FB;     // max
    double dRat_Max;

    BOOL bChk_DeckG;
    BOOL bChk_DeckL;
    BOOL bChk_Deck;
    BOOL bChk_Rib ;
    BOOL bChk_FBG;
    BOOL bChk_FBL;
    BOOL bChk_FB;
    BOOL bOK;  

    void Initialize()
    {
        bChk        = FALSE;
        nLSChkSize  = 0;
        nPanChkSize = 0;
        nDeckG_cr   = 0;
        nDeckL_cr   = 0;
        nRib_cr     = 0;
        nRibPan_cr  = 0;
        nFBG_cr     = 0;
        nFBL_cr     = 0;
        nFBLPan_cr  = 0;

        dRat_DeckG =0.0;
        dRat_DeckL =0.0;
        dRat_Deck  =0.0;
        dRat_Rib   =0.0;
        dRat_FBG   =0.0;
        dRat_FBL   =0.0;
        dRat_FB    =0.0;
        dRat_Max   =0.0;
        bChk_DeckG = FALSE;
        bChk_DeckL = FALSE;
        bChk_Deck  = FALSE;
        bChk_Rib   = FALSE;  
        bChk_FBG   = FALSE;
        bChk_FBL   = FALSE;
        bChk_FB    = FALSE;
        bOK        = FALSE;  

    }
    T_SDPT_STAB_FLG_RW() {Initialize();}

    void GetData(T_SDPT_STAB_FLG &rData)
    {
        rData.bChk    = bChk;
        rData.aLSChk.SetSize(nLSChkSize);
        rData.aPanChk.SetSize(nPanChkSize);
        rData.nDeckG_cr = nDeckG_cr;
        rData.nDeckL_cr = nDeckL_cr;
        rData.nRib_cr   = nRib_cr;
        rData.nRibPan_cr = nRibPan_cr;
        rData.nFBG_cr  = nFBG_cr;
        rData.nFBL_cr  = nFBL_cr;
        rData.nFBLPan_cr = nFBLPan_cr;

        rData.dRat_DeckG  = dRat_DeckG ;
        rData.dRat_DeckL  = dRat_DeckL ;
        rData.dRat_Deck   = dRat_Deck  ;
        rData.dRat_Rib    = dRat_Rib   ;
        rData.dRat_FBG    = dRat_FBG   ;
        rData.dRat_FBL    = dRat_FBL   ;
        rData.dRat_FB     = dRat_FB    ;
        rData.dRat_Max    = dRat_Max   ;
        rData.bChk_DeckG  = bChk_DeckG ;
        rData.bChk_DeckL  = bChk_DeckL ;
        rData.bChk_Deck   = bChk_Deck  ;
        rData.bChk_Rib    = bChk_Rib   ;
        rData.bChk_FBG    = bChk_FBG   ;
        rData.bChk_FBL    = bChk_FBL   ;
        rData.bChk_FB     = bChk_FB    ;
        rData.bOK         = bOK        ;
    }

    void SetData(T_SDPT_STAB_FLG &rData)
    {
        bChk        = rData.bChk;
        nLSChkSize  = (int)rData.aLSChk.GetSize();
        nPanChkSize = (int)rData.aPanChk.GetSize();
        nDeckG_cr   = rData.nDeckG_cr;
        nDeckL_cr   = rData.nDeckL_cr;
        nRib_cr     = rData.nRib_cr;
        nRibPan_cr  = rData.nRibPan_cr;
        nFBG_cr     = rData.nFBG_cr;
        nFBL_cr     = rData.nFBL_cr;
        nFBLPan_cr  = rData.nFBLPan_cr;

        dRat_DeckG  = rData.dRat_DeckG ;
        dRat_DeckL  = rData.dRat_DeckL ;
        dRat_Deck   = rData.dRat_Deck  ;
        dRat_Rib    = rData.dRat_Rib   ;
        dRat_FBG    = rData.dRat_FBG   ;
        dRat_FBL    = rData.dRat_FBL   ;
        dRat_FB     = rData.dRat_FB    ;
        dRat_Max    = rData.dRat_Max   ;
        bChk_DeckG  = rData.bChk_DeckG ;
        bChk_DeckL  = rData.bChk_DeckL ;
        bChk_Deck   = rData.bChk_Deck  ;
        bChk_Rib    = rData.bChk_Rib   ;
        bChk_FBG    = rData.bChk_FBG   ;
        bChk_FBL    = rData.bChk_FBL   ;
        bChk_FB     = rData.bChk_FB    ;
        bOK         = rData.bOK        ;
    }  
};

// Flange Stability Check
struct T_SDSB_FLG
{

    T_SDPT_STAB_FLG StabChk[3]; //
    int nPart_Rib_cr;           // Rib Ratio 가 큰 Part
    int nPart_Deck_cr;          // Deck Ratio 가 큰 Part

    void Initialize()
    {
        for (int i=0; i<3; ++i)
        {
            StabChk[i].Initialize();
        }
        nPart_Rib_cr  = 0;
        nPart_Deck_cr = 0;

    }
    T_SDSB_FLG() {Initialize();}
    T_SDSB_FLG(const T_SDSB_FLG &rData)	{*this = rData;}
    T_SDSB_FLG& operator = (const T_SDSB_FLG &rData)
    {
        for (int i=0; i<3; ++i)
        {
            StabChk[i] = rData.StabChk[i];
        }
        nPart_Rib_cr  = rData.nPart_Rib_cr;
        nPart_Deck_cr = rData.nPart_Deck_cr;

        return *this;
    }

};

struct T_SDSB_FLG_RW
{
    T_SDPT_STAB_FLG_RW StabChk[3]; //
    int nPart_Rib_cr;           // Rib Ratio 가 큰 Part
    int nPart_Deck_cr;          // Deck Ratio 가 큰 Part

    void Initialize()
    {
        for (int i=0; i<3; ++i)
        {
            StabChk[i].Initialize();
        }
        nPart_Rib_cr  = 0;
        nPart_Deck_cr = 0;

    }
    T_SDSB_FLG_RW() {Initialize();}

    void GetData(T_SDSB_FLG &rData)
    {
        for (int i=0; i<3; ++i)
        {
            StabChk[i].GetData(rData.StabChk[i]);
        }
        rData.nPart_Rib_cr  = nPart_Rib_cr;
        rData.nPart_Deck_cr = nPart_Deck_cr;
    }

    void SetData(T_SDSB_FLG &rData)
    {
        for (int i=0; i<3; ++i)
        {
            StabChk[i].SetData(rData.StabChk[i]);
        }
        nPart_Rib_cr  = rData.nPart_Rib_cr;
        nPart_Deck_cr = rData.nPart_Deck_cr;
    }  
};

// Web Stability check

// Web Position Check (Longitudinal Stiffener )
struct T_SDPS_STAB_WEB_PAN
{
    BOOL   bChk;     // 검토여부

    int    nAppItemType;  // 0 : Item 4, 1 : Item 5,  2: Item6,  3: Item 7 
    int    nChkPlateType; // in case of Item4 : 0-only one type plate
    // in case of Item5 : 0-first plate, 1-second plate
    // in case of Item6 : 0-first plate, 1-Subsequent compression plate, 2-Comp.&Tens. plate, 3-Intermediate Tension plate, 4-plate adjoining tension flange
    // in case of Item7 : 0-only one type plate    
    double dSigx;
    double dSigy;
    double dTauxy;
    double dSigx_cr;
    double dSigy_cr;
    double dTauxy_cr;
    double dComRat;   // Appendix 16 4. 식(10) 4.3식(14) 등...
    BOOL   bOK;

    void Initialize()
    {
        bChk          = FALSE;
        nAppItemType  = 0;
        nChkPlateType = 0;

        dSigx     = 0.0;
        dSigy     = 0.0;
        dTauxy    = 0.0;
        dSigx_cr  = 0.0;
        dSigy_cr  = 0.0;
        dTauxy_cr = 0.0;
        dComRat   = 0.0;
        bOK       = FALSE;    
    }

    T_SDPS_STAB_WEB_PAN() {Initialize();}
    T_SDPS_STAB_WEB_PAN(const T_SDPS_STAB_WEB_PAN &rData)	{*this = rData;}
    T_SDPS_STAB_WEB_PAN& operator = (const T_SDPS_STAB_WEB_PAN &rData)
    {
        bChk          = rData.bChk         ;    
        nAppItemType  = rData.nAppItemType ;    
        nChkPlateType = rData.nChkPlateType;    
        dSigx    = rData.dSigx    ;    
        dSigy    = rData.dSigy    ;    
        dTauxy   = rData.dTauxy   ;    
        dSigx_cr = rData.dSigx_cr ;    
        dSigy_cr = rData.dSigy_cr ; 
        dTauxy_cr= rData.dTauxy_cr; 
        dComRat  = rData.dComRat  ; 
        bOK      = rData.bOK      ; 

        return *this;
    }

};

struct T_SDPS_STAB_WEB_PAN_UNIT
{
    T_SDPS_STAB_WEB_PAN_UNIT()
    {
        dSigma = D_UNITSYS_BASE_STRESS;
        dTau   = D_UNITSYS_BASE_STRESS;
    }
    int dSigma;
    int dTau;
};

typedef CArray<T_SDPS_STAB_WEB_PAN,T_SDPS_STAB_WEB_PAN&> WEBSTABCHK_ARR;

// Web Part Stability
struct T_SDPT_STAB_WEB
{
    BOOL   bChk;             // 검토여부  
    WEBSTABCHK_ARR  aWebChk; // LS 나누어진 패널 수 만큼 
    int nPan_cr;             // 가장 불리한 리브 중 해당 plate.

    // 추가
    int nRib_cr;    // 가장 불리한 리브
    int nRibPan_cr; // nRib_cr 중 불리한 plate
    BOOL bChk_Deck; 
    BOOL bChk_Rib;
    double dRat_Deck;
    double dRat_Rib;
    double dRat_Max;
    BOOL bOK;

    void Initialize()
    {
        bChk      = FALSE;
        aWebChk.RemoveAll();
        nPan_cr = 0;

        nRib_cr = 0;
        nRibPan_cr =0;
        bChk_Deck = FALSE; 
        bChk_Rib  = FALSE;
        dRat_Deck =0.0;
        dRat_Rib  =0.0;
        dRat_Max  =0.0;
        bOK = FALSE;

    }
    T_SDPT_STAB_WEB() {Initialize();}
    T_SDPT_STAB_WEB(const T_SDPT_STAB_WEB &rData)	{*this = rData;}
    T_SDPT_STAB_WEB& operator = (const T_SDPT_STAB_WEB &rData)
    {
        bChk    = rData.bChk;    
        aWebChk.Copy(rData.aWebChk);
        nPan_cr = rData.nPan_cr;

        nRib_cr   = rData.nRib_cr;
        nRibPan_cr= rData.nRibPan_cr;
        bChk_Deck = rData.bChk_Deck;
        bChk_Rib  = rData.bChk_Rib ;
        dRat_Deck = rData.dRat_Deck;
        dRat_Rib  = rData.dRat_Rib ;
        dRat_Max   = rData.dRat_Max;
        bOK        = rData.bOK;

        return *this;
    }
};

struct T_SDPT_STAB_WEB_RW
{
    BOOL bChk;         // 검토여부  
    int  nWebChkSize;  //WEBSTABCHK_ARR  aWebChk; // LS 나누어진 패널 수 만큼 
    int  nPan_cr;      // 패널중 dComRat값이 큰 패널 

    // 추가
    int  nRib_cr;  // 가장 불리한 리브
    int nRibPan_cr;
    BOOL bChk_Deck; 
    BOOL bChk_Rib;
    double dRat_Deck;
    double dRat_Rib;
    double dRat_Max;
    BOOL bOK;

    void Initialize()
    {
        bChk        = FALSE;
        nWebChkSize = 0;    
        nPan_cr     = 0;

        nRib_cr  = 0;
        nRibPan_cr = 0;
        bChk_Deck = FALSE; 
        bChk_Rib  = FALSE;
        dRat_Deck =0.0;
        dRat_Rib  =0.0;
        dRat_Max  =0.0;
        bOK = FALSE;

    }
    T_SDPT_STAB_WEB_RW() {Initialize();}

    void GetData(T_SDPT_STAB_WEB &rData)
    {
        rData.bChk    = bChk;    
        rData.aWebChk.SetSize(nWebChkSize);
        rData.nPan_cr = nPan_cr;

        rData.nRib_cr   = nRib_cr;
        rData.nRibPan_cr = nRibPan_cr;
        rData.bChk_Deck = bChk_Deck;
        rData.bChk_Rib  = bChk_Rib ;
        rData.dRat_Deck = dRat_Deck;
        rData.dRat_Rib  = dRat_Rib ;
        rData.dRat_Max  = dRat_Max;
        rData.bOK       = bOK;
    }

    void SetData(T_SDPT_STAB_WEB &rData)
    {
        bChk        = rData.bChk;    
        nWebChkSize = (int)rData.aWebChk.GetSize();
        nPan_cr     = rData.nPan_cr;

        nRib_cr   = rData.nRib_cr;
        nRibPan_cr= rData.nRibPan_cr;
        bChk_Deck = rData.bChk_Deck;
        bChk_Rib  = rData.bChk_Rib ;
        dRat_Deck = rData.dRat_Deck;
        dRat_Rib  = rData.dRat_Rib ;
        dRat_Max   = rData.dRat_Max;
        bOK        = rData.bOK;
    }
};

// Web Stability Check
struct T_SDSB_WEB
{
    T_SDPT_STAB_WEB WebD[2]; // Left, Right

    void Initialize()
    {
        for (int i=0; i<2; ++i)
        {
            WebD[i].Initialize();
        }
    }
    T_SDSB_WEB() {Initialize();}
    T_SDSB_WEB(const T_SDSB_WEB &rData)	{*this = rData;}
    T_SDSB_WEB& operator = (const T_SDSB_WEB &rData)
    {
        for (int i=0; i<2; ++i)
        {
            WebD[i] = rData.WebD[i];
        }

        return *this;
    }

};

struct T_SDSB_WEB_RW
{
    T_SDPT_STAB_WEB_RW WebD[2]; //

    void Initialize()
    {
        for (int i=0; i<2; ++i)
        {
            WebD[i].Initialize();
        }
    }
    T_SDSB_WEB_RW() {Initialize();}

    void GetData(T_SDSB_WEB &rData)
    {
        for (int i=0; i<2; ++i)
        {
            WebD[i].GetData(rData.WebD[i]);
        }
    }

    void SetData(T_SDSB_WEB &rData)
    {
        for (int i=0; i<2; ++i)
        {
            WebD[i].SetData(rData.WebD[i]);
        }
    }  
};

struct T_SDSN_B
{
    BOOL bChk;
    T_SODD_FORCE GForce;
    T_SDSN_FLG   TopSn;
    T_SDSN_FLG   BotSn;
    T_SDSN_WEB   WebSn;
    T_SDSB_FLG   TopSb;
    T_SDSB_FLG   BotSb;
    T_SDSB_WEB   WebSb;

    void Initialize()
    {
        bChk = FALSE;
        GForce.Initialize();
        TopSn.Initialize();
        BotSn.Initialize();
        WebSn.Initialize();
        TopSb.Initialize();
        BotSb.Initialize();
        WebSb.Initialize();
    }

    T_SDSN_B() {Initialize();}
    T_SDSN_B(const T_SDSN_B &rData)	{*this = rData;}
    T_SDSN_B& operator = (const T_SDSN_B &rData)
    {
        bChk   = rData.bChk;
        GForce = rData.GForce;
        TopSn  = rData.TopSn;
        BotSn  = rData.BotSn;
        WebSn  = rData.WebSn;
        TopSb  = rData.TopSb;
        BotSb  = rData.BotSb;
        WebSb  = rData.WebSb;

        return *this;
    }

};

// Strength, Top/Bottom에 사용
struct T_SDSN_FLG_CASE
{
    T_SDSN_FLG  StrnFlg[2];  // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) StrnFlg[i].Initialize();
    }

    T_SDSN_FLG_CASE() {Initialize();}
    T_SDSN_FLG_CASE(const T_SDSN_FLG_CASE &rData)	{*this = rData;}
    T_SDSN_FLG_CASE& operator = (const T_SDSN_FLG_CASE &rData)
    {
        for (int i=0; i<2; ++i) StrnFlg[i] = rData.StrnFlg[i];

        return *this;
    }
};

struct T_SDSN_FLG_CASE_RW
{
    T_SDSN_FLG_RW  StrnFlg[2];  // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) StrnFlg[i].Initialize();
    }

    T_SDSN_FLG_CASE_RW() {Initialize();}

    void GetData(T_SDSN_FLG_CASE &rData)
    {
        for (int i=0; i<2; ++i)
        {
            StrnFlg[i].GetData(rData.StrnFlg[i]);
        }
    }

    void SetData(T_SDSN_FLG_CASE &rData)
    {
        for (int i=0; i<2; ++i)
        {
            StrnFlg[i].SetData(rData.StrnFlg[i]);
        }
    }  

};

struct T_SDSN_WEB_CASE
{
    T_SDSN_WEB  StrnWeb[2];  // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) StrnWeb[i].Initialize();
    }

    T_SDSN_WEB_CASE() {Initialize();}
    T_SDSN_WEB_CASE(const T_SDSN_WEB_CASE &rData)	{*this = rData;}
    T_SDSN_WEB_CASE& operator = (const T_SDSN_WEB_CASE &rData)
    {
        for (int i=0; i<2; ++i) StrnWeb[i] = rData.StrnWeb[i];

        return *this;
    }
};

struct T_SDSN_WEB_CASE_RW
{
    T_SDSN_WEB_RW  StrnWeb[2];  // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) StrnWeb[i].Initialize();
    }

    T_SDSN_WEB_CASE_RW() {Initialize();}
    void GetData(T_SDSN_WEB_CASE &rData)
    {
        for (int i=0; i<2; ++i)
        {
            StrnWeb[i].GetData(rData.StrnWeb[i]);
        }
    }

    void SetData(T_SDSN_WEB_CASE &rData)
    {
        for (int i=0; i<2; ++i)
        {
            StrnWeb[i].SetData(rData.StrnWeb[i]);
        }
    }  
};


struct  T_SDSB_GD 
{
    BOOL   bChk; 
    double dRat_GD;
    BOOL   bOK;  

    void Initialize()
    {
        bChk = FALSE; 
        dRat_GD = 0.0;
        bOK  = FALSE;  

    }
    T_SDSB_GD() {Initialize();}
    T_SDSB_GD(const T_SDSB_GD &rData)	{*this = rData;}
    T_SDSB_GD& operator = (const T_SDSB_GD &rData)
    {
        bChk    = rData.bChk;
        dRat_GD = rData.dRat_GD;
        bOK     = rData.bOK;
        return *this;
    }
};

struct T_SDSB_GD_RW
{
    BOOL   bChk; 
    double dRat_GD;
    BOOL   bOK;  

    void Initialize()
    {
        bChk = FALSE; 
        dRat_GD = 0.0;
        bOK  = FALSE;      
    }
    T_SDSB_GD_RW() {Initialize();}

    void GetData(T_SDSB_GD &rData)
    {
        rData.bChk    = bChk;
        rData.dRat_GD = dRat_GD;
        rData.bOK     = bOK;
    }

    void SetData(T_SDSB_GD &rData)
    {
        bChk    = rData.bChk;
        dRat_GD = rData.dRat_GD;
        bOK     = rData.bOK;
    }  
};

// Global Deck Stability
struct T_SDSB_GD_CASE
{
    T_SDSB_GD StabGD[2];  // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) StabGD[i].Initialize();
    }

    T_SDSB_GD_CASE() {Initialize();}
    T_SDSB_GD_CASE(const T_SDSB_GD_CASE &rData)	{*this = rData;}
    T_SDSB_GD_CASE& operator = (const T_SDSB_GD_CASE &rData)
    {
        for (int i=0; i<2; ++i) StabGD[i] = rData.StabGD[i];

        return *this;
    }
};

struct T_SDSB_GD_CASE_RW
{
    T_SDSB_GD_RW  StabGD[2];  // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) StabGD[i].Initialize();
    }

    T_SDSB_GD_CASE_RW() {Initialize();}
    void GetData(T_SDSB_GD_CASE &rData)
    {
        for (int i=0; i<2; ++i)
        {
            StabGD[i].GetData(rData.StabGD[i]);
        }
    }

    void SetData(T_SDSB_GD_CASE &rData)
    {
        for (int i=0; i<2; ++i)
        {
            StabGD[i].SetData(rData.StabGD[i]);
        }
    }  
};


// Stability, Top/Bottom에 사용
struct T_SDSB_FLG_CASE
{
    T_SDSB_FLG  StabFlg[2];  // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) StabFlg[i].Initialize();
    }

    T_SDSB_FLG_CASE() {Initialize();}
    T_SDSB_FLG_CASE(const T_SDSB_FLG_CASE &rData)	{*this = rData;}
    T_SDSB_FLG_CASE& operator = (const T_SDSB_FLG_CASE &rData)
    {
        for (int i=0; i<2; ++i) StabFlg[i] = rData.StabFlg[i];

        return *this;
    }
};

struct T_SDSB_FLG_CASE_RW
{
    T_SDSB_FLG_RW  StabFlg[2];  // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) StabFlg[i].Initialize();
    }

    T_SDSB_FLG_CASE_RW() {Initialize();}
    void GetData(T_SDSB_FLG_CASE &rData)
    {
        for (int i=0; i<2; ++i)
        {
            StabFlg[i].GetData(rData.StabFlg[i]);
        }
    }

    void SetData(T_SDSB_FLG_CASE &rData)
    {
        for (int i=0; i<2; ++i)
        {
            StabFlg[i].SetData(rData.StabFlg[i]);
        }
    }  
};

struct T_SDSB_WEB_CASE
{
    T_SDSB_WEB  StabWeb[2];  // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) StabWeb[i].Initialize();
    }

    T_SDSB_WEB_CASE() {Initialize();}
    T_SDSB_WEB_CASE(const T_SDSB_WEB_CASE &rData)	{*this = rData;}
    T_SDSB_WEB_CASE& operator = (const T_SDSB_WEB_CASE &rData)
    {
        for (int i=0; i<2; ++i) StabWeb[i] = rData.StabWeb[i];

        return *this;
    }
};

struct T_SDSB_WEB_CASE_RW
{
    T_SDSB_WEB_RW  StabWeb[2];  // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) StabWeb[i].Initialize();
    }

    T_SDSB_WEB_CASE_RW() {Initialize();}
    void GetData(T_SDSB_WEB_CASE &rData)
    {
        for (int i=0; i<2; ++i)
        {
            StabWeb[i].GetData(rData.StabWeb[i]);
        }
    }

    void SetData(T_SDSB_WEB_CASE &rData)
    {
        for (int i=0; i<2; ++i)
        {
            StabWeb[i].SetData(rData.StabWeb[i]);
        }
    }  
};

struct T_SDSN_CASE
{
    T_ELEM_K ElemK;
    T_SDSN_B StrnB[2];  //0=I, 1=J

    void Initialize()
    {
        ElemK = 0;
        for (int i=0; i<2; ++i) StrnB[i].Initialize();
    }

    T_SDSN_CASE() {Initialize();}
    T_SDSN_CASE(const T_SDSN_CASE &rData)	{*this = rData;}
    T_SDSN_CASE& operator = (const T_SDSN_CASE &rData)
    {
        ElemK = rData.ElemK;
        for (int i=0; i<2; ++i) StrnB[i] = rData.StrnB[i];

        return *this;
    }

};

// Fatigue
struct T_SDFG_BASE
{
    BOOL   bChk;
    T_SODD_FORCE ForDL;
    int    nChkPoint;
    double dSig_max_ef;
    double dGamRym;    // γw * Ry * m
    double dSig_min;
    double dSig_max;
    double dRatio;
    BOOL   bOK;

    void Initialize()
    {
        bChk = FALSE;
        ForDL.Initialize();
        nChkPoint = 0;
        dSig_max_ef = 0.0;
        dGamRym     = 0.0;
        dSig_min    = 0.0;
        dSig_max    = 0.0;
        dRatio      = 0.0;
        bOK  = FALSE;     
    }
    T_SDFG_BASE() {Initialize();}
};

struct T_SDFG_BASE_UNIT
{
    T_SDFG_BASE_UNIT()
    {
        dSig_max_ef = D_UNITSYS_BASE_STRESS;
        dGamRym     = D_UNITSYS_BASE_STRESS;
        dSig_min    = D_UNITSYS_BASE_STRESS;
        dSig_max    = D_UNITSYS_BASE_STRESS;
    }
    int dSig_max_ef;
    int dGamRym;
    int dSig_min;
    int dSig_max;

};

struct T_SDFG_CASE
{
    T_SDFG_BASE FatiB[2]; // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) FatiB[i].Initialize();
    }
    T_SDFG_CASE() {Initialize();}
};

struct T_SDCR_BASE
{
    //T_ELEM_K ElemK;
    //UINT     nPos;
    int      nLcomK;      // Lcom Key
    int      nMaxMin;     // 0=None, 2=Max/Min, 12=Concurrent
    int      nConcurrent; // Fx_max .... Mz_min(1~12) 
    int      nDgnLcomK;  
    int      nDgnLcomKFirst;  
    int      nLcomSize;

    void Initialize()
    {
        nLcomK      = 0;
        nMaxMin     = 0;
        nConcurrent = 0;
        nDgnLcomK   = 0;
        nDgnLcomKFirst =0;
        nLcomSize   = 0;
    }

    T_SDCR_BASE() {Initialize();}
};

struct T_SDSN_FLG_CR // Top/Bottom 사용 
{
    T_SDCR_BASE RibCr[4];   //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom
    T_SDCR_BASE DeckCr[4];  //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom
    T_SDCR_BASE FBeamCr[4]; //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom

    void Initialize()
    {
        for (int i=0; i<4; ++i)
        {
            RibCr[i].Initialize();
            DeckCr[i].Initialize();
            FBeamCr[i].Initialize();
        }
    }
    T_SDSN_FLG_CR() {Initialize();}
    T_SDSN_FLG_CR(const T_SDSN_FLG_CR &rData)	{*this = rData;}
    T_SDSN_FLG_CR& operator = (const T_SDSN_FLG_CR &rData)
    {
        for (int i=0; i<4; ++i)
        {
            RibCr[i]   = rData.RibCr[i];
            DeckCr[i]  = rData.DeckCr[i];
            FBeamCr[i] = rData.FBeamCr[i];
        }

        return *this;
    }  
};

struct T_SDSN_WEB_CR // Web 사용 
{
    // Left
    T_SDCR_BASE RibCrL[2]; //0=I, 1=J  
    T_SDCR_BASE DeckCrL[2]; //0=I, 1=J  
    T_SDCR_BASE DeckNoMCrL[2]; //0=I, 1=J  
    T_SDCR_BASE DeckVonCrL[2]; //0=I, 1=J  
    T_SDCR_BASE DeckMeanCrL[2]; //0=I, 1=J  
    T_SDCR_BASE DeckCombCrL[2]; //0=I, 1=J  

    //Right
    T_SDCR_BASE RibCrR[2]; //0=I, 1=J  
    T_SDCR_BASE DeckCrR[2]; //0=I, 1=J  
    T_SDCR_BASE DeckNoMCrR[2]; //0=I, 1=J  
    T_SDCR_BASE DeckVonCrR[2]; //0=I, 1=J  
    T_SDCR_BASE DeckMeanCrR[2]; //0=I, 1=J  
    T_SDCR_BASE DeckCombCrR[2]; //0=I, 1=J  

    void Initialize()
    {
        for (int i=0; i<2; ++i)
        {
            RibCrL[i].Initialize();
            DeckCrL[i].Initialize();
            DeckNoMCrL[i].Initialize();
            DeckVonCrL[i].Initialize();
            DeckMeanCrL[i].Initialize();
            DeckCombCrL[i].Initialize();

            RibCrR[i].Initialize();
            DeckCrR[i].Initialize();
            DeckNoMCrR[i].Initialize();
            DeckVonCrR[i].Initialize();
            DeckMeanCrR[i].Initialize();
            DeckCombCrR[i].Initialize();

        }
    }
    T_SDSN_WEB_CR() {Initialize();}
    T_SDSN_WEB_CR(const T_SDSN_WEB_CR &rData)	{*this = rData;}
    T_SDSN_WEB_CR& operator = (const T_SDSN_WEB_CR &rData)
    {
        for (int i=0; i<2; ++i)
        {
            RibCrL[i]  = rData.RibCrL[i];      
            DeckCrL[i] = rData.DeckCrL[i];      
            DeckNoMCrL[i] = rData.DeckNoMCrL[i];      
            DeckVonCrL[i] = rData.DeckVonCrL[i];      
            DeckMeanCrL[i] = rData.DeckMeanCrL[i];      
            DeckCombCrL[i] = rData.DeckCombCrL[i];     

            RibCrR[i]     = rData.RibCrR[i];      
            DeckCrR[i]    = rData.DeckCrR[i];      
            DeckNoMCrR[i] = rData.DeckNoMCrR[i];      
            DeckVonCrR[i] = rData.DeckVonCrR[i];      
            DeckMeanCrR[i] = rData.DeckMeanCrR[i];      
            DeckCombCrR[i] = rData.DeckCombCrR[i];    
        }

        return *this;
    }  
};

struct T_SDSB_FLG_CR // Top/Bottom 사용 
{
    T_SDCR_BASE DeckGCr[4];  //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom   // Global
    T_SDCR_BASE DeckLCr[4];  //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom   // Local
    T_SDCR_BASE DeckCr[4];   //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom   // Max

    T_SDCR_BASE RibCr[4];    //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom

    T_SDCR_BASE FBeamGCr[4]; //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom   // Global
    T_SDCR_BASE FBeamLCr[4]; //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom   // Local
    T_SDCR_BASE FBeamCr[4];  //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom   // Deck

    void Initialize()
    {
        for(int i=0; i<4; ++i)
        {
            DeckGCr[i].Initialize();
            DeckLCr[i].Initialize();
            DeckCr[i].Initialize();
            RibCr[i].Initialize();
            FBeamGCr[i].Initialize();
            FBeamLCr[i].Initialize();
            FBeamCr[i].Initialize();
        }
    }
    T_SDSB_FLG_CR() {Initialize();}
    T_SDSB_FLG_CR(const T_SDSB_FLG_CR &rData)	{*this = rData;}
    T_SDSB_FLG_CR& operator = (const T_SDSB_FLG_CR &rData)
    {
        for (int i=0; i<4; ++i)
        {
            DeckGCr[i]  = rData.DeckGCr[i];
            DeckLCr[i]  = rData.DeckLCr[i];
            DeckCr[i]   = rData.DeckCr[i];
            RibCr[i]    = rData.RibCr[i];
            FBeamGCr[i] = rData.FBeamGCr[i];
            FBeamLCr[i] = rData.FBeamLCr[i];
            FBeamCr[i]  = rData.FBeamCr[i];
        }
        return *this;
    }  
};

struct T_SDSB_WEB_CR // Web 사용 
{
    // Left
    T_SDCR_BASE RibCrL[2]; //0=I, 1=J  
    T_SDCR_BASE DeckCrL[2]; //0=I, 1=J  

    //Right
    T_SDCR_BASE RibCrR[2]; //0=I, 1=J  
    T_SDCR_BASE DeckCrR[2]; //0=I, 1=J  


    void Initialize()
    {
        for (int i=0; i<2; ++i)
        {
            RibCrL[i].Initialize();      
            DeckCrL[i].Initialize();
            RibCrR[i].Initialize();      
            DeckCrR[i].Initialize();
        }
    }
    T_SDSB_WEB_CR() {Initialize();}
    T_SDSB_WEB_CR(const T_SDSB_WEB_CR &rData)	{*this = rData;}
    T_SDSB_WEB_CR& operator = (const T_SDSB_WEB_CR &rData)
    {
        for (int i=0; i<2; ++i)
        {
            RibCrL[i]  = rData.RibCrL[i];      
            DeckCrL[i] = rData.DeckCrL[i];      
            RibCrR[i]  = rData.RibCrR[i];      
            DeckCrR[i] = rData.DeckCrR[i];      
        }

        return *this;
    }

};

struct T_SDSB_GD_CR // 
{
    T_SDCR_BASE GDCr[2]; //0=I, 1=J 

    void Initialize()
    {
        for (int i=0; i<2; ++i) GDCr[i].Initialize();
    }

    T_SDSB_GD_CR() {Initialize();}
};


struct T_SDFG_CR 
{
    T_SDCR_BASE FatiCr[2]; //0=I, 1=J 

    void Initialize()
    {
        for (int i=0; i<2; ++i) FatiCr[i].Initialize();
    }

    T_SDFG_CR() {Initialize();}
};

struct T_EDLK_D
{
    T_ELEM_K ElemK;
    T_SPAN_K SpanK;
    T_MEMB_K MembK;
     
    BOOL bExistSpanInfo;
    BOOL bExistMembInfo;
    BOOL bExistUbracedInfo;

    double dDgnLengY;
    double dDgnLengZ;
    double dEffBuckCoeffY;
    double dEffBuckCoeffZ;

    int nSupTypeY[2]; // I, J for local Y Direction // 0 : Fix, 1 : Hinge, 2 : Free
    int nSupTypeZ[2]; // I, J for local Z Direction // 0 : Fix, 1 : Hinge, 2 : Free
    int nLoInfoInMemb[2]; // I, J // 0 : two middle forth portions of the length , 1 : The Others

    void Initialize()
    {
        ElemK = 0;
        SpanK = 0;
        MembK = 0;

        bExistSpanInfo    = FALSE;
        bExistMembInfo    = FALSE;
        bExistUbracedInfo = FALSE;

        dDgnLengY      = 0.0;
        dDgnLengZ      = 0.0;
        dEffBuckCoeffY = 0.0;
        dEffBuckCoeffZ = 0.0;

        for (int i=0; i<2; ++i)
        {
            nSupTypeY[i]      = 0;
            nSupTypeZ[i]      = 0;
            nLoInfoInMemb[i]  = 0;      
        }
    }

    T_EDLK_D() {Initialize();}
    T_EDLK_D(const T_EDLK_D &rData)	{*this = rData;}
    T_EDLK_D& operator = (const T_EDLK_D &rData)
    {
        ElemK = rData.ElemK;
        SpanK = rData.SpanK;
        MembK = rData.MembK;
        
        bExistSpanInfo    = rData.bExistSpanInfo   ;
        bExistMembInfo    = rData.bExistMembInfo   ;
        bExistUbracedInfo = rData.bExistUbracedInfo;
        
        dDgnLengY      = rData.dDgnLengY     ;
        dDgnLengZ      = rData.dDgnLengZ     ;
        dEffBuckCoeffY = rData.dEffBuckCoeffY;
        dEffBuckCoeffZ = rData.dEffBuckCoeffZ;
        
        for (int i=0; i<2; ++i)
        {
            nSupTypeY[i]      = rData.nSupTypeY[i]    ;
            nSupTypeZ[i]      = rData.nSupTypeZ[i]    ;
            nLoInfoInMemb[i]  = rData.nLoInfoInMemb[i];      
        }
        
        return *this;
    }

};


//////////////////////////////////////////////////////////////////////////
//  RUS Steel Composite Design 
//////////////////////////////////////////////////////////////////////////

struct T_RSCD_ITEM
{
    BOOL bRSCCheckBefore[7]; // 0: STR-TF  1: STR-BF  2: STR-Web  3: STB-LTB     4: STB-TF  5: STB-BF  6: STB-Web
    BOOL bRSCCheckAfter[10]; // 0: STR-Concrete  1: STR-Rebar  2: STR-Top Flange    3: STR-Bottom Flange   4: STR-Web
                                                                                                                    // 5: STB-Bottom Flange 6: STB-Web  7: Fatigue    8: Crack  9 : Shear Connector

    void Initialize()
    {
        memset(bRSCCheckBefore, 0, sizeof(bRSCCheckBefore));
        memset(bRSCCheckAfter, 0, sizeof(bRSCCheckAfter));
    }
    T_RSCD_ITEM() {Initialize();}
};

struct T_ACST_CONC // After Composite Strength Concrete Slab - Part I & J
{
    BOOL bCheck; // 검토 유무
    BOOL bOK; // TRUE :  OK,   FALSE : NG
    int nPosNega; // 0 : Positive, 1 : Negative
    int nCaseType; // 0 : A, 1 : B, 2: C, 3: D, 4 : E
    double dSigma_b_Cri;
    double dSigma_r_Cri;
    double dSigma_b_lim_Cri;
    double dSigma_r_lim_Cri;
    double dEpsilon_b;
    double dEpsilon_b_lim;
    double dRatio;

    void Initialize()
    {
        bCheck = FALSE;
        bOK = FALSE;
        nPosNega= 0; 
        nCaseType = 0;
        dSigma_b_Cri = 0.0;
        dSigma_r_Cri = 0.0;
        dSigma_b_lim_Cri = 0.0;
        dSigma_r_lim_Cri = 0.0;
        dEpsilon_b = 0.0;
        dEpsilon_b_lim = 0.0;
        dRatio = 0.0;
    }
    T_ACST_CONC() {Initialize();}  
    T_ACST_CONC(const T_ACST_CONC &rData)	{*this = rData;}
    T_ACST_CONC& operator = (const T_ACST_CONC &rData)
    {
        bCheck = rData.bCheck;
        bOK = rData.bOK;
        nPosNega= rData.nPosNega;
        nCaseType = rData.nCaseType;
        dSigma_b_Cri = rData.dSigma_b_Cri;
        dSigma_r_Cri = rData.dSigma_r_Cri;
        dSigma_b_lim_Cri = rData.dSigma_b_lim_Cri;
        dSigma_r_lim_Cri = rData.dSigma_r_lim_Cri;
        dEpsilon_b = rData.dEpsilon_b;
        dEpsilon_b_lim =rData.dEpsilon_b_lim;
        dRatio = rData.dRatio;

        return *this;
    }

};

struct  T_ACST_CONC_UNIT
{
    T_ACST_CONC_UNIT()
    {
        nPosNega = D_UNITSYS_NONE;
        nCaseType = D_UNITSYS_NONE;
        dSigma_b_Cri = D_UNITSYS_BASE_STRESS;
        dSigma_r_Cri = D_UNITSYS_BASE_STRESS;
        dSigma_b_lim_Cri = D_UNITSYS_BASE_STRESS;
        dSigma_r_lim_Cri = D_UNITSYS_BASE_STRESS;
        dEpsilon_b = D_UNITSYS_NONE;
        dEpsilon_b_lim = D_UNITSYS_NONE;
        dRatio = D_UNITSYS_NONE;
    }
    int nPosNega; // 0 : Positive, 1 : Negative
    int nCaseType; // 0 : A, 1 : B, 2: C, 3: D, 4 : E
    int dSigma_b_Cri;
    int dSigma_r_Cri;
    int dSigma_b_lim_Cri;
    int dSigma_r_lim_Cri;
    int dEpsilon_b;
    int dEpsilon_b_lim;
    int dRatio;
};

struct T_ACST_CONC_CASE // After Composite Strength Concrete Slab
{
    T_ACST_CONC  StrnConc[2]; // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) StrnConc[i].Initialize();
    }
    T_ACST_CONC_CASE() {Initialize();}  
    T_ACST_CONC_CASE(const T_ACST_CONC_CASE &rData)	{*this = rData;}
    T_ACST_CONC_CASE& operator = (const T_ACST_CONC_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) StrnConc[i] = StrnConc[i];
        return *this;
    }

};

struct T_PLASTIC_LIM_BASE // After Composite Plastic Limit State
{
    //T_ELEM_K ElemK;
    //UINT     nPos;
    int      nLcomK;      // Lcom Key
    int      nMaxMin;     // 0=None, 2=Max/Min, 12=Concurrent
    int      nConcurrent; // Fx_max .... Mz_min(1~12) 
    int      nDgnLcomK;  
    int      nDgnLcomKFirst;  
    int      nLcomSize;

    void Initialize()
    {
        nLcomK = 0;     
        nMaxMin = 0; 
        nConcurrent = 0;
        nDgnLcomK = 0;  
        nDgnLcomKFirst = 0;  
        nLcomSize = 0;
    }
    T_PLASTIC_LIM_BASE() {Initialize();}  
    T_PLASTIC_LIM_BASE(const T_PLASTIC_LIM_BASE &rData)	{*this = rData;}
    T_PLASTIC_LIM_BASE& operator = (const T_PLASTIC_LIM_BASE &rData)
    {
        nLcomK = rData.nLcomK;  
        nMaxMin = rData.nMaxMin; 
        nConcurrent = rData.nConcurrent;
        nDgnLcomK = rData.nDgnLcomK;  
        nDgnLcomKFirst = rData.nDgnLcomKFirst;  
        nLcomSize = rData.nLcomSize;

        return *this;
    }
};

struct T_PLASTIC_LIM_CR // After Composite Plastic Limit State
{
    T_PLASTIC_LIM_BASE  PlasticLimStrainCr[2]; // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) PlasticLimStrainCr[i].Initialize();
    }

    T_PLASTIC_LIM_CR() {Initialize();}  
    T_PLASTIC_LIM_CR(const T_PLASTIC_LIM_CR &rData)	{*this = rData;}
    T_PLASTIC_LIM_CR& operator = (const T_PLASTIC_LIM_CR &rData)
    {
        for(int i = 0; i < 2; ++i) PlasticLimStrainCr[i]= PlasticLimStrainCr[i];
        return *this;
    }
};

struct T_ACST_CONC_BASE // After Composite Strength Concrete Slab Critical Base
{
    //T_ELEM_K ElemK;
    //UINT     nPos;
    int      nLcomK;      // Lcom Key
    int      nMaxMin;     // 0=None, 2=Max/Min, 12=Concurrent
    int      nConcurrent; // Fx_max .... Mz_min(1~12) 
    int      nDgnLcomK;  
    int      nDgnLcomKFirst;  
    int      nLcomSize;

    void Initialize()
    {
        nLcomK = 0;     
        nMaxMin = 0; 
        nConcurrent = 0;
        nDgnLcomK = 0;  
        nDgnLcomKFirst = 0;  
        nLcomSize = 0;
    }
    T_ACST_CONC_BASE() {Initialize();}  
    T_ACST_CONC_BASE(const T_ACST_CONC_BASE &rData)	{*this = rData;}
    T_ACST_CONC_BASE& operator = (const T_ACST_CONC_BASE &rData)
    {
        nLcomK = rData.nLcomK;  
        nMaxMin = rData.nMaxMin; 
        nConcurrent = rData.nConcurrent;
        nDgnLcomK = rData.nDgnLcomK;  
        nDgnLcomKFirst = rData.nDgnLcomKFirst;  
        nLcomSize = rData.nLcomSize;

        return *this;
    }
};

struct T_ACST_CONC_CR // After Composite Strength Concrete Slab Critical
{
    T_ACST_CONC_BASE  StrnConcCR[2]; // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) StrnConcCR[i].Initialize();
    }

    T_ACST_CONC_CR() {Initialize();}  
    T_ACST_CONC_CR(const T_ACST_CONC_CR &rData)	{*this = rData;}
    T_ACST_CONC_CR& operator = (const T_ACST_CONC_CR &rData)
    {
        for(int i = 0; i < 2; ++i) StrnConcCR[i]= StrnConcCR[i];
        return *this;
    }
};


struct T_ACST_REBAR // After Composite Strength Rebar - Part  I & J
{
    BOOL bCheck; // 검토 유무
    BOOL bOK; // TRUE :  OK,   FALSE : NG
    int nPosNega; // 0 : Positive, 1 : Negative
    int nCaseType; // 0 : A, 1 : B, 2: C, 3: D, 4 : E
    double dSigma_b_Cri;
    double dSigma_b_lim_Cri;
    double dSigma_r;
    double dSigma_r_lim;
    double dRatio;

    void Initialize()
    {
        bCheck = FALSE;
        bOK = FALSE;
        nPosNega = 0;
        nCaseType = 0;
        dSigma_b_Cri = 0.0;
        dSigma_b_lim_Cri = 0.0;
        dSigma_r = 0.0;
        dSigma_r_lim = 0.0;
        dRatio = 0.0;
    }

    T_ACST_REBAR() {Initialize();}  
    T_ACST_REBAR(const T_ACST_REBAR &rData)	{*this = rData;}
    T_ACST_REBAR& operator = (const T_ACST_REBAR &rData)
    {
        bCheck = rData.bCheck;
        bOK = rData.bOK;
        nPosNega = rData.nPosNega;
        nCaseType = rData.nCaseType;
        dSigma_b_Cri = rData.dSigma_b_Cri;
        dSigma_b_lim_Cri = rData.dSigma_b_lim_Cri;
        dSigma_r = rData.dSigma_r;
        dSigma_r_lim = rData.dSigma_r_lim;
        dRatio = rData.dRatio;
        return *this;
    }
};

struct T_ACST_REBAR_UNIT // After Composite Strength Rebar - Part  I & J
{
    int nPosNega; // 0 : Positive, 1 : Negative
    int nCaseType; // 0 : A, 1 : B, 2: C, 3: D, 4 : E
    int dSigma_b_Cri;
    int dSigma_b_lim_Cri;
    int dSigma_r;
    int dSigma_r_lim;
    int dRatio;

    void Initialize()
    {
        nPosNega = D_UNITSYS_NONE;
        nCaseType = D_UNITSYS_NONE;
        dSigma_b_Cri = D_UNITSYS_BASE_STRESS;
        dSigma_b_lim_Cri = D_UNITSYS_BASE_STRESS;
        dSigma_r = D_UNITSYS_BASE_STRESS;
        dSigma_r_lim = D_UNITSYS_BASE_STRESS;
        dRatio =D_UNITSYS_NONE;
    }

    T_ACST_REBAR_UNIT() {Initialize();}  
};

struct T_ACST_REBAR_CASE // After Composite Strength Rebar
{
    T_ACST_REBAR StrnRebar[2];

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) StrnRebar[i].Initialize();
    }

    T_ACST_REBAR_CASE() {Initialize();}  
    T_ACST_REBAR_CASE(const T_ACST_REBAR_CASE &rData)	{*this = rData;}
    T_ACST_REBAR_CASE& operator = (const T_ACST_REBAR_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) StrnRebar[i] = rData.StrnRebar[i];
        return *this;
    }
};

struct T_ACST_REBAR_BASE // After Composite Strength Rebar Critical - Part  I & J
{
    //T_ELEM_K ElemK;
    //UINT     nPos;
    int      nLcomK;      // Lcom Key
    int      nMaxMin;     // 0=None, 2=Max/Min, 12=Concurrent
    int      nConcurrent; // Fx_max .... Mz_min(1~12) 
    int      nDgnLcomK;  
    int      nDgnLcomKFirst;  
    int      nLcomSize;

    void Initialize()
    {
        nLcomK = 0;     
        nMaxMin = 0; 
        nConcurrent = 0;
        nDgnLcomK = 0;  
        nDgnLcomKFirst = 0;  
        nLcomSize = 0;
    }
    T_ACST_REBAR_BASE() {Initialize();}  
    T_ACST_REBAR_BASE(const T_ACST_REBAR_BASE &rData)	{*this = rData;}
    T_ACST_REBAR_BASE& operator = (const T_ACST_REBAR_BASE &rData)
    {
        nLcomK = rData.nLcomK;  
        nMaxMin = rData.nMaxMin; 
        nConcurrent = rData.nConcurrent;
        nDgnLcomK = rData.nDgnLcomK;  
        nDgnLcomKFirst = rData.nDgnLcomKFirst;  
        nLcomSize = rData.nLcomSize;

        return *this;
    }
};

struct T_ACST_REBAR_CR // After Composite Strength Rebar Critical
{
    T_ACST_REBAR_BASE StrnRebarCR[2];

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) StrnRebarCR[i].Initialize();
    }

    T_ACST_REBAR_CR() {Initialize();}  
    T_ACST_REBAR_CR(const T_ACST_REBAR_CR &rData)	{*this = rData;}
    T_ACST_REBAR_CR& operator = (const T_ACST_REBAR_CR &rData)
    {
        for(int i = 0; i < 2; ++i) StrnRebarCR[i] = rData.StrnRebarCR[i];
        return *this;
    }
};

struct T_ACST_SGFR // After Composite Strength Steel Girder Flange Result
{
    BOOL bCheck;// 검토 유무
    BOOL bOK;   // TRUE :  OK,   FALSE : NG
    BOOL bTopBot;   // TRUE :  Top,   FALSE : Bottom
    int nPosNega;   // 0 : Positive, 1 : Negative
    int nCaseType; // 0 : A, 1 : B, 2: C, 3: D, 4 : E
    double dSigma_b_Cri;
    double dSigma_b_lim_Cri;
    double dSigma_r_Cri;
    double dSigma_r_lim_Cri;
    double dRigidity_Cri;
    double dRigidity_lim_Cri;
    double dSigma; // Check Demand
    double dSigma_lim; // Check Capacity
    double dRatio;

    void Initialize()
    {
        bCheck = FALSE;
        bOK = FALSE; 
        bTopBot = TRUE; 
        nPosNega = 0;  
        nCaseType = 0; 
        dSigma_b_Cri = 0.0;
        dSigma_b_lim_Cri = 0.0;
        dSigma_r_Cri = 0.0;
        dSigma_r_lim_Cri = 0.0;
        dRigidity_Cri = 0.0;
        dRigidity_lim_Cri = 0.0;
        dSigma = 0.0; 
        dSigma_lim = 0.0;
        dRatio = 0.0;
    }

    T_ACST_SGFR() {Initialize();}  
    T_ACST_SGFR(const T_ACST_SGFR &rData)	{*this = rData;}
    T_ACST_SGFR& operator = (const T_ACST_SGFR &rData)
    {
        bCheck = rData.bCheck;
        bOK = rData.bOK; 
        bTopBot = rData.bTopBot; 
        nPosNega = rData.nPosNega;  
        nCaseType = rData.nCaseType; 
        dSigma_b_Cri = rData.dSigma_b_Cri;
        dSigma_b_lim_Cri = rData.dSigma_b_lim_Cri;
        dSigma_r_Cri = rData.dSigma_r_Cri;
        dSigma_r_lim_Cri = rData.dSigma_r_lim_Cri;
        dRigidity_Cri = rData.dRigidity_Cri;
        dRigidity_lim_Cri = rData.dRigidity_lim_Cri;
        dSigma = rData.dSigma; 
        dSigma_lim = rData.dSigma_lim;
        dRatio = rData.dRatio;
        return *this;
    }
};

struct T_ACST_SGFR_UNIT // After Composite Strength Steel Girder Flange Result - Unit System
{
    int nPosNega;   // 0 : Positive, 1 : Negative
    int nCaseType; // 0 : A, 1 : B, 2: C, 3: D, 4 : E
    int dSigma_b_Cri;
    int dSigma_b_lim_Cri;
    int dSigma_r_Cri;
    int dSigma_r_lim_Cri;
    int dRigidity_Cri;
    int dRigidity_lim_Cri;
    int dSigma; // Check Demand
    int dSigma_lim; // Check Capacity
    int dRatio;

    void Initialize()
    {
        nPosNega =  D_UNITSYS_NONE ;
        nCaseType =D_UNITSYS_NONE ; 
        dSigma_b_Cri = D_UNITSYS_BASE_STRESS;
        dSigma_b_lim_Cri = D_UNITSYS_BASE_STRESS;
        dSigma_r_Cri =D_UNITSYS_BASE_STRESS ;
        dSigma_r_lim_Cri = D_UNITSYS_BASE_STRESS;
        dRigidity_Cri = D_UNITSYS_BASE_DISTSTIFF;
        dRigidity_lim_Cri = D_UNITSYS_BASE_DISTSTIFF;
        dSigma =D_UNITSYS_BASE_STRESS ; 
        dSigma_lim = D_UNITSYS_BASE_STRESS;
        dRatio = D_UNITSYS_NONE;
    }
};


typedef CArray<T_ACST_SGFR, T_ACST_SGFR&> arStlCompStrFlg;

struct T_ACST_SGFR_ELEM // After Composite Strength Steel Girder Flange Result - Part (left, Middle, Right) Unit
{
    BOOL bCheck; // 검토 여부
    BOOL bChk_Deck; 
    BOOL bChk_Rib;
    BOOL bChk_FB; 
    BOOL bOK;   // TRUE :  OK,   FALSE : NG

    arStlCompStrFlg aFlangeStrRes; // 위치별 검토 결과 - Flange
    LSSTRNFLG_ARR aRibStrRes; //위치별 검토 결과 - Flange

    int nCrFlgIndex; // Critical Index for Flange;  
    int nCrRibIndex; // Critical Index for Rib;  

    double dCrFlgRatio; // Critical Ratio for Flange
    double dCrRibRatio; // Critical Ratio for Rib 
    double dCrFBRatio; // Critical Ratio for Floor Beam

    double dCrRatio; // Critical Ratio of all

    void Initialize()
    {
        bCheck = FALSE;
        bChk_Deck = FALSE;
        bChk_Rib = FALSE;
        bChk_FB = FALSE;
        bOK =FALSE;

        aFlangeStrRes.RemoveAll();
        aRibStrRes.RemoveAll();

        nCrFlgIndex = 0;
        nCrRibIndex = 0;

        dCrFlgRatio = 0.0;
        dCrRibRatio = 0.0;
        dCrFBRatio = 0.0;
        dCrRatio = 0.0;
    }

    T_ACST_SGFR_ELEM() {Initialize();}  
    T_ACST_SGFR_ELEM(const T_ACST_SGFR_ELEM &rData)	{*this = rData;}
    T_ACST_SGFR_ELEM& operator = (const T_ACST_SGFR_ELEM &rData)
    {
        bCheck = rData.bCheck;
        bChk_Deck = rData.bChk_Deck;
        bChk_Rib = rData.bChk_Rib;
        bChk_FB = rData.bChk_FB;
        bOK = rData.bOK;

        aFlangeStrRes.Copy(rData.aFlangeStrRes);
        aRibStrRes.Copy(rData.aRibStrRes);

        nCrFlgIndex = rData.nCrFlgIndex;
        nCrRibIndex = rData.nCrRibIndex;

        dCrFlgRatio = rData.dCrFlgRatio;
        dCrRibRatio = rData.dCrRibRatio;
        dCrFBRatio = rData.dCrFBRatio;
        dCrRatio = rData.dCrRatio;

        return *this;
    }
};

struct T_ACST_SGFR_PART// After Composite Strength Steel Girder Flange Result
{
    BOOL bCheck; // 검토 여부
    BOOL bOK;   // TRUE :  OK,   FALSE : NG
    T_ACST_SGFR_ELEM StrnFlangePart[3]; // Left , Middle, Right

    int nCrPart_Flg;  // Critical Index for Flange;  0 : Left, 1 :  Middle, 2 :  Right
    double dCrRatio_Flg;// Critical Ratio for Flange
    int nCrPart_Rib;  // Critical Index for Rib;  0 : Left, 1 :  Middle, 2 :  Right
    double dCrRatio_Rib;// Critical Ratio for Rib
    int nCrPart_FB; // Critical Index for Floor Beam;  0 : Left, 1 :  Middle, 2 :  Right
    double dCrRatio_FB; // Critical Ratio for Floor Beam

    int nCrPart; // Critical Index;  0 : Left, 1 :  Middle, 2 :  Rigth
    int nCrCaseType;
    double dCrRatio; // Critical Ratio;  

    void Initialize()
    {
        bCheck = FALSE;
        bOK =FALSE;
        for(int i = 0; i < 3; ++i) StrnFlangePart[i].Initialize();
        nCrPart_Flg = 0;
        dCrRatio_Flg = 0.0;
        nCrPart_Rib = 0;
        dCrRatio_Rib = 0.0;
        nCrPart_FB = 0;
        dCrRatio_FB = 0.0;
        nCrPart = 0;
        nCrCaseType = 0;
        dCrRatio = 0.0;
    }

    T_ACST_SGFR_PART() {Initialize();}  
    T_ACST_SGFR_PART(const T_ACST_SGFR_PART &rData)	{*this = rData;}
    T_ACST_SGFR_PART& operator = (const T_ACST_SGFR_PART &rData)
    {
        bCheck = rData.bCheck;
        bOK = rData.bOK;
        for(int i = 0; i < 3; ++i)StrnFlangePart[i] = rData.StrnFlangePart[i];
        nCrPart_Flg = rData.nCrPart_Flg;
        dCrRatio_Flg = rData.dCrRatio_Flg;
        nCrPart_Rib = rData.nCrPart_Rib;
        dCrRatio_Rib = rData.dCrRatio_Rib;
        nCrPart_FB = rData.nCrPart_FB;
        dCrRatio_FB = rData.dCrRatio_FB;
        nCrPart = rData.nCrPart;
        nCrCaseType = rData.nCrCaseType;
        dCrRatio = rData.dCrRatio;

        return *this;
    }
};

struct T_ACST_SGFR_CASE // After Composite Strength Steel Girder Flange Result
{
    T_ACST_SGFR_PART StrnFlgIJ[2]; // 0 : I  , 1 :  J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) StrnFlgIJ[i].Initialize();
    }

    T_ACST_SGFR_CASE() {Initialize();}  
};

struct T_ACST_SGFR_BASE // After Composite Strength Steel Girder Flange Result
{
    //T_ELEM_K ElemK;
    //UINT     nPos;
    int      nLcomK;      // Lcom Key
    int      nMaxMin;     // 0=None, 2=Max/Min, 12=Concurrent
    int      nConcurrent; // Fx_max .... Mz_min(1~12) 
    int      nDgnLcomK;  
    int      nDgnLcomKFirst;  
    int      nLcomSize;

    void Initialize()
    {
        nLcomK = 0;     
        nMaxMin = 0; 
        nConcurrent = 0;
        nDgnLcomK = 0;  
        nDgnLcomKFirst = 0;  
        nLcomSize = 0;
    }
    T_ACST_SGFR_BASE() {Initialize();}  
    T_ACST_SGFR_BASE(const T_ACST_SGFR_BASE &rData)	{*this = rData;}
    T_ACST_SGFR_BASE& operator = (const T_ACST_SGFR_BASE &rData)
    {
        nLcomK = rData.nLcomK;  
        nMaxMin = rData.nMaxMin; 
        nConcurrent = rData.nConcurrent;
        nDgnLcomK = rData.nDgnLcomK;  
        nDgnLcomKFirst = rData.nDgnLcomKFirst;  
        nLcomSize = rData.nLcomSize;

        return *this;
    }
};

struct T_ACST_SGFR_CR // After Composite Strength Steel Girder Flange Result
{
    T_ACST_SGFR_BASE StrnFlgCR[4]; // 0 : I - Top , 1 :  I -  Bottom, 0 : J - Top , 1 :  J -  Bottom
    T_SDCR_BASE RibCr[4];   //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom
    T_SDCR_BASE FBeamCr[4]; //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom

    void Initialize()
    {
        for(int i = 0; i < 4; ++i) 
        {
            StrnFlgCR[i].Initialize();
            RibCr[i].Initialize();
            FBeamCr[i].Initialize();
        }
    }

    T_ACST_SGFR_CR() {Initialize();}  
};

struct T_ACST_SGFR_RW // After Composite Strength Steel Girder Flange Result - RW,  I & J Unit
{
    BOOL bCheck;// 검토 유무
    BOOL bOK;   // TRUE :  OK,   FALSE : NG
    BOOL bTopBot;   // TRUE :  Top,   FALSE : Bottom
    int nPosNega;   // 0 : Positive, 1 : Negative
    int nCaseType; // 0 : A, 1 : B, 2: C, 3: D, 4 : E
    double dSigma_b_Cri;
    double dSigma_b_lim_Cri;
    double dSigma_r_Cri;
    double dSigma_r_lim_Cri;
    double dRigidity_Cri;
    double dRigidity_lim_Cri;
    double dSigma; // Check Demand
    double dSigma_lim; // Check Capacity
    double dRatio;

    void Initialize()
    {
        bCheck = FALSE;
        bOK = FALSE; 
        bTopBot = TRUE; 
        nPosNega = 0;  
        nCaseType = 0; 
        dSigma_b_Cri = 0.0;
        dSigma_b_lim_Cri = 0.0;
        dSigma_r_Cri = 0.0;
        dSigma_r_lim_Cri = 0.0;
        dRigidity_Cri = 0.0;
        dRigidity_lim_Cri = 0.0;
        dSigma = 0.0; 
        dSigma_lim = 0.0;
        dRatio = 0.0;
    }

    T_ACST_SGFR_RW() {Initialize();}  
    T_ACST_SGFR_RW(const T_ACST_SGFR_RW &rData)	{*this = rData;}
    T_ACST_SGFR_RW& operator = (const T_ACST_SGFR_RW &rData)
    {
        bCheck = rData.bCheck;
        bOK = rData.bOK; 
        bTopBot = rData.bTopBot; 
        nPosNega = rData.nPosNega;  
        nCaseType = rData.nCaseType; 
        dSigma_b_Cri = rData.dSigma_b_Cri;
        dSigma_b_lim_Cri = rData.dSigma_b_lim_Cri;
        dSigma_r_Cri = rData.dSigma_r_Cri;
        dSigma_r_lim_Cri = rData.dSigma_r_lim_Cri;
        dRigidity_Cri = rData.dRigidity_Cri;
        dRigidity_lim_Cri = rData.dRigidity_lim_Cri;
        dSigma = rData.dSigma; 
        dSigma_lim = rData.dSigma_lim;
        dRatio = rData.dRatio;

        return *this;
    }

    void GetData(T_ACST_SGFR &rData)
    {
        rData.bCheck = bCheck;
        rData.bOK = bOK;
        rData.bTopBot = bTopBot; 
        rData.nPosNega = nPosNega;  
        rData.nCaseType = nCaseType; 
        rData.dSigma_b_Cri = dSigma_b_Cri;
        rData.dSigma_b_lim_Cri = dSigma_b_lim_Cri;
        rData.dSigma_r_Cri = dSigma_r_Cri;
        rData.dSigma_r_lim_Cri = dSigma_r_lim_Cri;
        rData.dRigidity_Cri = dRigidity_Cri;
        rData.dRigidity_lim_Cri = dRigidity_lim_Cri;
        rData.dSigma = dSigma; 
        rData.dSigma_lim = dSigma_lim;
        rData.dRatio = dRatio;
    }

    void SetData(const T_ACST_SGFR &rData)
    {
        bCheck = rData.bCheck;
        bOK = rData.bOK; 
        bTopBot = rData.bTopBot; 
        nPosNega = rData.nPosNega;  
        nCaseType = rData.nCaseType; 
        dSigma_b_Cri = rData.dSigma_b_Cri;
        dSigma_b_lim_Cri = rData.dSigma_b_lim_Cri;
        dSigma_r_Cri = rData.dSigma_r_Cri;
        dSigma_r_lim_Cri = rData.dSigma_r_lim_Cri;
        dRigidity_Cri = rData.dRigidity_Cri;
        dRigidity_lim_Cri = rData.dRigidity_lim_Cri;
        dSigma = rData.dSigma; 
        dSigma_lim = rData.dSigma_lim;
        dRatio = rData.dRatio;
    }
};


struct T_ACST_SGFR_ELEM_RW // After Composite Strength Steel Girder Flange Result - RW, Elem
{
    BOOL bCheck;
    BOOL bChk_Deck; 
    BOOL bChk_Rib;
    BOOL bChk_FB; 
    BOOL bOK;
    int nStrnFlgResNum;
    int nStrnRibResNum;

    int nCrFlgIndex;
    int nCrRibIndex;

    double dCrFlgRatio; // Critical Ratio for Flange
    double dCrRibRatio; // Critical Ratio for Rib 
    double dCrFBRatio; // Critical Ratio for Floor Beam

    double dCrRatio; // Critical Ratio of all

    void Initialize()
    {
        bCheck = FALSE;
        bChk_Deck = FALSE;
        bChk_Rib = FALSE;
        bChk_FB = FALSE;
        bOK = FALSE;

        nStrnFlgResNum = 0;
        nStrnRibResNum = 0;

        nCrFlgIndex = 0;
        nCrRibIndex = 0;

        dCrFlgRatio = 0.0;
        dCrRibRatio = 0.0;
        dCrFBRatio = 0.0;
        dCrRatio = 0.0;

    }

    T_ACST_SGFR_ELEM_RW() {Initialize();}  

    void GetData(T_ACST_SGFR_ELEM &rData)
    {
        rData.bCheck =  bCheck;
        rData.bChk_Deck = bChk_Deck;
        rData.bChk_Rib = bChk_Rib;
        rData.bChk_FB = bChk_FB;
        rData.bOK =  bOK;

        rData.aFlangeStrRes.SetSize(nStrnFlgResNum);
        rData.aRibStrRes.SetSize(nStrnRibResNum);

        rData.nCrFlgIndex = nCrFlgIndex;
        rData.nCrRibIndex = nCrRibIndex;

        rData.dCrFlgRatio = dCrFlgRatio;
        rData.dCrRibRatio = dCrRibRatio;
        rData.dCrFBRatio = dCrFBRatio;

        rData.dCrRatio = dCrRatio;
    }

    void SetData(const T_ACST_SGFR_ELEM &rData)
    {
        bCheck =  rData.bCheck;
        bChk_Deck =  rData.bChk_Deck;
        bChk_Rib =  rData.bChk_Rib;
        bChk_FB =  rData.bChk_FB;

        bOK =  rData.bOK;

        nStrnFlgResNum = (int)rData.aFlangeStrRes.GetSize();
        nStrnRibResNum = (int)rData.aFlangeStrRes.GetSize();

        nCrFlgIndex = rData.nCrFlgIndex;
        nCrRibIndex = rData.nCrRibIndex;

        dCrFlgRatio = rData.dCrFlgRatio;
        dCrRibRatio = rData.dCrRibRatio;
        dCrFBRatio = rData.dCrFBRatio;

        dCrRatio = rData.dCrRatio;
    }

};

struct T_ACST_SGFR_PART_RW // After Composite Strength Steel Girder Flange Result - RW, PART
{
    T_ACST_SGFR_ELEM_RW StrnFlgPart[3];  // Part (left, Middle, Right) Unit

    void Initialize()
    {
        for(int i = 0; i<3; ++i) StrnFlgPart[i].Initialize();
    }

    T_ACST_SGFR_PART_RW() {Initialize();}  

    void GetData(T_ACST_SGFR_PART &rData)
    {
        for(int i = 0; i<3; ++i) StrnFlgPart[i].GetData(rData.StrnFlangePart[i]);
    }

    void SetData(const T_ACST_SGFR_PART &rData)
    {
        for(int i = 0; i<3; ++i) StrnFlgPart[i].SetData(rData.StrnFlangePart[i]);
    }
};

struct T_ACST_SGFR_CASE_RW // After Composite Strength Steel Girder Flange Result - RW
{
    T_ACST_SGFR_PART_RW StrnFlgIJ[2]; //  I & J Unit
    void Initialize()
    {
        for(int i = 0; i<2; ++i) StrnFlgIJ[i].Initialize();
    }

    T_ACST_SGFR_CASE_RW() {Initialize();}  

    void GetData(T_ACST_SGFR_CASE &rData)
    {
        for(int i = 0; i<2; ++i) 
        {
            StrnFlgIJ[i].GetData(rData.StrnFlgIJ[i]);

            double dMaxRat = 0.0;
            for(int j = 0; j < 3; ++j)
            {
                if(StrnFlgIJ[i].StrnFlgPart[j].bCheck) 
                {rData.StrnFlgIJ[i].bCheck = TRUE;}

                if(rData.StrnFlgIJ[i].dCrRatio < StrnFlgIJ[i].StrnFlgPart[j].dCrRatio)
                {rData.StrnFlgIJ[i].dCrRatio = StrnFlgIJ[i].StrnFlgPart[j].dCrRatio;}
            }      
        }
    }

    void SetData(const T_ACST_SGFR_CASE &rData)
    {
        for(int i = 0; i<2; ++i) StrnFlgIJ[i].SetData(rData.StrnFlgIJ[i]);
    }
};


struct T_SHEAR_CONN_ELEM_RESD
{
    UINT ElemK;
    double dLongiSForceElem;  // along the element
    double dLongiSForce[2]; //  0 : I-end ,  1:  J-end

    void Initialize()
    {
        ElemK = 0;
        dLongiSForceElem = 0.0;
        memset(dLongiSForce, 0, sizeof(dLongiSForce));
    }

    T_SHEAR_CONN_ELEM_RESD() {Initialize();}  
    T_SHEAR_CONN_ELEM_RESD(const T_SHEAR_CONN_ELEM_RESD &rData)	{*this = rData;}
    T_SHEAR_CONN_ELEM_RESD& operator = (const T_SHEAR_CONN_ELEM_RESD &rData)
    {
        ElemK = rData.ElemK;
        dLongiSForceElem = rData.dLongiSForceElem;
        dLongiSForce[0] = rData.dLongiSForce[0];
        dLongiSForce[1] = rData.dLongiSForce[1];
        return *this;
    }
};

struct T_STRD_SHEAR_CONN // After Composite Shear Connector
{
    BOOL bCheck; // 검토 여부
    BOOL bOK; // TRUE :  OK,   FALSE : NG
    T_SCGD_K GroupKey;
    CString strGroupName;
    T_ELEM_K StartElem; // Check Group의 Start Elem Key
    T_ELEM_K EndElem; // Check Group의 End Elem Key
    double dSlabAxialForce; // Check Elem-Part(I&J)의 Longi . Force
    double dSlabAxialForce_Str;// Check Group의 Start Position Longi . Force
    double dSlabAxialForce_End; // Check Group의 End Position Longi . Force
    double dSlabAxialForce_Res; // Check Group의 Check  Longi . Force : fabs(dSlabAxialForce_Str - dSlabAxialForce_End)
    int nDgnLengthType; // RUS SNiP APP 21에서 정의하는 Design Length Type
    double dGroupLength; // Group Length
    double dResistanceStud; // Each shear Connector Resistance
    int nNumofStud; // Shear Connector Group Stud Num
    double dReqSpace; // Require Space
    double dUserSpace; // User Input Space
    double dRatio;

    CArray<T_SHEAR_CONN_ELEM_RESD, T_SHEAR_CONN_ELEM_RESD> arElemResD;

    void Initialize()
    {
        bCheck =FALSE;
        bOK= FALSE;
        GroupKey = 0;
        strGroupName = _T("");
        StartElem = 0;
        EndElem = 0;
        dSlabAxialForce = 0.0;
        dSlabAxialForce_Str = 0.0;
        dSlabAxialForce_End = 0.0;
        dSlabAxialForce_Res = 0.0;
        nDgnLengthType = 0;
        dGroupLength = 0.0;
        dResistanceStud = 0.0;
        nNumofStud = 0;
        dReqSpace = 0.0;
        dUserSpace = 0.0;
        dRatio = 0.0;
        arElemResD.RemoveAll();
    }

    T_STRD_SHEAR_CONN() {Initialize();}  
    T_STRD_SHEAR_CONN(const T_STRD_SHEAR_CONN &rData)	{*this = rData;}
    T_STRD_SHEAR_CONN& operator = (const T_STRD_SHEAR_CONN &rData)
    {
        bCheck =rData.bCheck;
        bOK= rData.bOK;
        GroupKey= rData.GroupKey;
        strGroupName = rData.strGroupName;
        StartElem = rData.StartElem;
        EndElem = rData.EndElem;
        dSlabAxialForce =rData.dSlabAxialForce;
        dSlabAxialForce_Str = rData.dSlabAxialForce_Str;
        dSlabAxialForce_End = rData.dSlabAxialForce_End;
        dSlabAxialForce_Res = rData.dSlabAxialForce_Res;
        nDgnLengthType = rData.nDgnLengthType;
        dGroupLength = rData.dGroupLength;
        dResistanceStud = rData.dResistanceStud;
        nNumofStud = rData.nNumofStud;
        dReqSpace = rData.dReqSpace;
        dUserSpace = rData.dUserSpace;
        dRatio = rData.dRatio;
        arElemResD.Copy(rData.arElemResD);
        return *this;
    }
};

struct T_STRD_SHEAR_CONN_UNIT
{
    int dSlabAxialForce;
    int dSlabAxialForce_Str;
    int dSlabAxialForce_End;
    int dSlabAxialForce_Res;
    int nDgnLengthType;
    int dGroupLength;
    int dResistanceStud;
    int nNumofStud;
    int dReqSpace;
    int dUserSpace;
    int dRatio;

    void Initialize()
    {
        dSlabAxialForce = D_UNITSYS_BASE_FORCE;
        dSlabAxialForce_Str = D_UNITSYS_BASE_FORCE;
        dSlabAxialForce_End = D_UNITSYS_BASE_FORCE;
        dSlabAxialForce_Res = D_UNITSYS_BASE_FORCE;
        nDgnLengthType = D_UNITSYS_NONE;
        dGroupLength = D_UNITSYS_BASE_LENGTH;
        dResistanceStud = D_UNITSYS_BASE_FORCE;
        nNumofStud = D_UNITSYS_NONE;
        dReqSpace = D_UNITSYS_BASE_LENGTH;
        dUserSpace = D_UNITSYS_BASE_LENGTH;
        dRatio = D_UNITSYS_NONE;
    }

    T_STRD_SHEAR_CONN_UNIT(){Initialize();}
};

struct T_STRD_SHEAR_CONN_CASE // After Composite Shear Connector
{
    T_STRD_SHEAR_CONN ShearConnRes[2]; // I & J
    void Initialize()
    {
        for(int i = 0; i < 2; ++i) ShearConnRes[i].Initialize();
    }

    T_STRD_SHEAR_CONN_CASE() {Initialize();}  
    T_STRD_SHEAR_CONN_CASE(const T_STRD_SHEAR_CONN_CASE &rData)	{*this = rData;}
    T_STRD_SHEAR_CONN_CASE& operator = (const T_STRD_SHEAR_CONN_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) ShearConnRes[i] = rData.ShearConnRes[i];
        return *this;
    }

};

struct T_STRD_SHEAR_CONN_BASE // After Composite Shear Connector
{
    //T_ELEM_K ElemK;
    //UINT     nPos;
    int      nLcomK;      // Lcom Key
    int      nMaxMin;     // 0=None, 2=Max/Min, 12=Concurrent
    int      nConcurrent; // Fx_max .... Mz_min(1~12) 
    int      nDgnLcomK;  
    int      nDgnLcomKFirst;  
    int      nLcomSize;

    void Initialize()
    {
        nLcomK = 0;     
        nMaxMin = 0; 
        nConcurrent = 0;
        nDgnLcomK = 0;  
        nDgnLcomKFirst = 0;  
        nLcomSize = 0;
    }
    T_STRD_SHEAR_CONN_BASE() {Initialize();}  
    T_STRD_SHEAR_CONN_BASE(const T_STRD_SHEAR_CONN_BASE &rData)	{*this = rData;}
    T_STRD_SHEAR_CONN_BASE& operator = (const T_STRD_SHEAR_CONN_BASE &rData)
    {
        nLcomK = rData.nLcomK;  
        nMaxMin = rData.nMaxMin; 
        nConcurrent = rData.nConcurrent;
        nDgnLcomK = rData.nDgnLcomK;  
        nDgnLcomKFirst = rData.nDgnLcomKFirst;  
        nLcomSize = rData.nLcomSize;

        return *this;
    }
};

struct T_STRD_SHEAR_CONN_CR// After Composite Shear Connector
{
    T_STRD_SHEAR_CONN_BASE ShearConnResCR[2]; // I & J
    void Initialize()
    {
        for(int i = 0; i < 2; ++i) ShearConnResCR[i].Initialize();
    }

    T_STRD_SHEAR_CONN_CR() {Initialize();}  
};

struct T_STRD_SHEAR_CONN_RW // After Composite Shear Connector
{
    BOOL bCheck; // 검토 여부
    BOOL bOK; // TRUE :  OK,   FALSE : NG
    T_SCGD_K GroupKey;
    char  strGroupName[64];
    T_ELEM_K StartElem; // Check Group의 Start Elem Key
    T_ELEM_K EndElem; // Check Group의 End Elem Key
    double dSlabAxialForce; // Check Elem-Part(I&J)의 Longi . Force
    double dSlabAxialForce_Str;// Check Group의 Start Position Longi . Force
    double dSlabAxialForce_End; // Check Group의 End Position Longi . Force
    double dSlabAxialForce_Res; // Check Group의 Check  Longi . Force : fabs(dSlabAxialForce_Str - dSlabAxialForce_End)
    int nDgnLengthType; // RUS SNiP APP 21에서 정의하는 Design Length Type
    double dGroupLength; // Group Length
    double dResistanceStud; // Each shear Connector Resistance
    int nNumofStud; // Shear Connector Group Stud Num
    double dReqSpace; // Require Space
    double dUserSpace; // User Input Space
    double dRatio;
    INT_PTR nGElemNum;

    void Initialize()
    {
        bCheck = FALSE;
        bOK = FALSE;
        GroupKey = 0;
        memset(strGroupName, ' ', sizeof(strGroupName));
        StartElem = 0;
        EndElem = 0;
        dSlabAxialForce = 0.0;
        dSlabAxialForce_Str = 0.0;
        dSlabAxialForce_End = 0.0;
        dSlabAxialForce_Res = 0.0;
        nDgnLengthType = 0; 
        dGroupLength = 0.0; 
        dResistanceStud = 0.0; 
        nNumofStud = 0; 
        dReqSpace = 0.0;
        dUserSpace = 0.0;
        dRatio = 0.0;
        nGElemNum = 0;
    }

    T_STRD_SHEAR_CONN_RW() {Initialize();}  

    void GetData(T_STRD_SHEAR_CONN &rData)
    {
        rData.bCheck = bCheck;
        rData.bOK = bOK; 
        rData.GroupKey = GroupKey;
        ConvertCharStr(strGroupName, rData.strGroupName,sizeof(strGroupName));
        rData.StartElem = StartElem;
        rData.EndElem = EndElem; 
        rData.dSlabAxialForce  = dSlabAxialForce;
        rData.dSlabAxialForce_Str = dSlabAxialForce_Str;
        rData.dSlabAxialForce_End = dSlabAxialForce_End;
        rData.dSlabAxialForce_Res = dSlabAxialForce_Res;
        rData.nDgnLengthType = nDgnLengthType; 
        rData.dGroupLength = dGroupLength;
        rData.dResistanceStud = dResistanceStud; 
        rData.nNumofStud = nNumofStud; 
        rData.dReqSpace = dReqSpace;
        rData.dUserSpace = dUserSpace; 
        rData.dRatio = dRatio;
        rData.arElemResD.SetSize(nGElemNum);
    }
    void SetData(T_STRD_SHEAR_CONN &rData)
    {
        bCheck = rData.bCheck;
        bOK = rData.bOK; 
        GroupKey = rData.GroupKey;
        ConvertStrChar(rData.strGroupName, strGroupName, sizeof(strGroupName));
        StartElem = rData.StartElem;
        EndElem =  rData.EndElem;
        dSlabAxialForce = rData.dSlabAxialForce;
        dSlabAxialForce_Str = rData.dSlabAxialForce_Str;
        dSlabAxialForce_End = rData.dSlabAxialForce_End;
        dSlabAxialForce_Res = rData.dSlabAxialForce_Res;
        nDgnLengthType = rData.nDgnLengthType;
        dGroupLength = rData.dGroupLength;
        dResistanceStud = rData.dResistanceStud;
        nNumofStud = rData.nNumofStud;
        dReqSpace = rData.dReqSpace;
        dUserSpace = rData.dUserSpace;
        dRatio = rData.dRatio;
        nGElemNum = rData.arElemResD.GetSize();
    }
};

struct T_STRD_SHEAR_CONN_CASE_RW // After Composite Shear Connector
{
    T_STRD_SHEAR_CONN_RW ShearConnRes[2]; // Part I & J
    void Initialize()
    {
        for(int i = 0; i <2; ++i)ShearConnRes[i].Initialize();
    }

    T_STRD_SHEAR_CONN_CASE_RW() {Initialize();}  

    void GetData(T_STRD_SHEAR_CONN_CASE &rData)
    {
        for(int i = 0; i <2; ++i)ShearConnRes[i].GetData(rData.ShearConnRes[i]);
    }

    void SetData(T_STRD_SHEAR_CONN_CASE &rData)
    {
        for(int i = 0; i <2; ++i)ShearConnRes[i].SetData(rData.ShearConnRes[i]);
    }

};

struct T_FATI_CONC // After Composite Fatigue Concrete
{
    BOOL bCheck;
    BOOL bOK;
    double dSigma_bf; // Demand 
    double dmbRb; // Capacity
    double dRatio;  

    void Initialize()
    {
        bCheck =FALSE;
        bOK = FALSE;
        dSigma_bf = 0.0;
        dmbRb = 0.0; 
        dRatio = 0.0;  
    }

    T_FATI_CONC() {Initialize();}  
    T_FATI_CONC(const T_FATI_CONC &rData)	{*this = rData;}
    T_FATI_CONC& operator = (const T_FATI_CONC &rData)
    {
        bCheck =rData.bCheck;
        bOK = rData.bOK;
        dSigma_bf = rData.dSigma_bf;
        dmbRb = rData.dmbRb; 
        dRatio = rData.dRatio;  

        return *this;
    }
};

struct T_FATI_CONC_UNIT
{
    int dSigma_bf; // Demand 
    int dmbRb; // Capacity
    int dRatio;  

    void Initialize()
    {
        dSigma_bf = D_UNITSYS_BASE_STRESS;
        dmbRb = D_UNITSYS_BASE_STRESS;
        dRatio = D_UNITSYS_NONE;
    }

    T_FATI_CONC_UNIT() {Initialize();}

};


struct T_FATI_CONC_CASE // After Composite Fatigue Concrete
{
    T_FATI_CONC FatiConc[2]; // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) FatiConc[i].Initialize();
    }

    T_FATI_CONC_CASE() {Initialize();}  
};

struct T_FATI_CONC_BASE // After Composite Fatigue Concrete
{
    //T_ELEM_K ElemK;
    //UINT     nPos;
    int      nLcomK;      // Lcom Key
    int      nMaxMin;     // 0=None, 2=Max/Min, 12=Concurrent
    int      nConcurrent; // Fx_max .... Mz_min(1~12) 
    int      nDgnLcomK;  
    int      nDgnLcomKFirst;  
    int      nLcomSize;

    void Initialize()
    {
        nLcomK = 0;     
        nMaxMin = 0; 
        nConcurrent = 0;
        nDgnLcomK = 0;  
        nDgnLcomKFirst = 0;  
        nLcomSize = 0;
    }
    T_FATI_CONC_BASE() {Initialize();}  
    T_FATI_CONC_BASE(const T_FATI_CONC_BASE &rData)	{*this = rData;}
    T_FATI_CONC_BASE& operator = (const T_FATI_CONC_BASE &rData)
    {
        nLcomK = rData.nLcomK;  
        nMaxMin = rData.nMaxMin; 
        nConcurrent = rData.nConcurrent;
        nDgnLcomK = rData.nDgnLcomK;  
        nDgnLcomKFirst = rData.nDgnLcomKFirst;  
        nLcomSize = rData.nLcomSize;

        return *this;
    }
};


struct T_FATI_CONC_CR // After Composite Fatigue Concrete
{
    T_FATI_CONC_BASE FatiConc[2]; // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) FatiConc[i].Initialize();
    }

    T_FATI_CONC_CR() {Initialize();}  
    T_FATI_CONC_CR(const T_FATI_CONC_CR &rData)	{*this = rData;}
    T_FATI_CONC_CR& operator = (const T_FATI_CONC_CR &rData)
    {
        for(int i = 0; i < 2; ++i) FatiConc[i] = rData.FatiConc[i];
        return *this;
    }
};

struct T_FATI_CONC_RW // After Composite Fatigue Concrete
{
    BOOL bCheck;
    BOOL bOK;
    double dSigma_bf; // Demand 
    double dmbRb; // Capacity
    double dRatio;  

    void Initialize()
    {
        bCheck =FALSE;
        bOK = FALSE;
        dSigma_bf = 0.0;
        dmbRb = 0.0; 
        dRatio = 0.0;  
    }

    T_FATI_CONC_RW() {Initialize();}  

    void GetData(T_FATI_CONC &rData)
    {
        rData.bCheck =bCheck;
        rData.bOK = bOK;
        rData.dSigma_bf = dSigma_bf;
        rData.dmbRb = dmbRb; 
        rData.dRatio = dRatio; 
    }
    void SetData(const T_FATI_CONC &rData)
    {
        bCheck =rData.bCheck;
        bOK = rData.bOK;
        dSigma_bf = rData.dSigma_bf;
        dmbRb = rData.dmbRb; 
        dRatio = rData.dRatio;  
    }  
};


struct T_FATI_CONC_CASE_RW // After Composite Fatigue Concrete
{
    T_FATI_CONC_RW FatiConc[2]; // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) FatiConc[i].Initialize();
    }

    T_FATI_CONC_CASE_RW() {Initialize();}  

    void GetData(T_FATI_CONC_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) FatiConc[i].GetData(rData.FatiConc[i]);
    }
    void SetData(const T_FATI_CONC_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) FatiConc[i].SetData(rData.FatiConc[i]);
    }
};

struct T_FATI_REBAR // After Composite Fatigue Rebar
{
    BOOL bCheck;
    double dRatio; // Larger ratio
    BOOL bOK;

    BOOL bCheck_3_94; // Checked by Item 3.94
    double dSigmap;
    double dmsRs_lim;
    double dRatio_3_94;

    BOOL bCheck_4_57; // Checked by Item 4.57
    double dSigma_max_ef;
    double dGamwRym_lim;
    double dRatio_4_57;  

    void Initialize()
    {
        bCheck = FALSE;
        dRatio = 0.0; // Larger ratio
        bOK = FALSE;

        bCheck_3_94 = FALSE; // Checked by Item 3.94
        dSigmap = 0.0;
        dmsRs_lim = 0.0;
        dRatio_3_94 = 0.0;

        bCheck_4_57 = FALSE; // Checked by Item 4.57
        dSigma_max_ef = 0.0;
        dGamwRym_lim = 0.0;
        dRatio_4_57 = 0.0;  
    }

    T_FATI_REBAR() {Initialize();}  
    T_FATI_REBAR(const T_FATI_REBAR &rData)	{*this = rData;}
    T_FATI_REBAR& operator = (const T_FATI_REBAR &rData)
    {
        bCheck = rData.bCheck;
        dRatio = rData.dRatio; 
        bOK = rData.bOK;

        bCheck_3_94 = rData.bCheck_3_94; 
        dSigmap = rData.dSigmap;
        dmsRs_lim = rData.dmsRs_lim;
        dRatio_3_94 = rData.dRatio_3_94;

        bCheck_4_57 = rData.bCheck_4_57; 
        dSigma_max_ef = rData.dSigma_max_ef;
        dGamwRym_lim = rData.dGamwRym_lim;
        dRatio_4_57 = rData.dRatio_4_57;  
        return *this;
    }

};

struct T_FATI_REBAR_UNIT
{
    int dRatio; // Larger ratio

    int dSigmap;
    int dmsRs_lim;
    int dRatio_3_94;

    int dSigma_max_ef;
    int dGamwRym_lim;
    int dRatio_4_57;

    void Initialize()
    {
        dRatio = D_UNITSYS_NONE;

        dSigmap = D_UNITSYS_BASE_STRESS;
        dmsRs_lim = D_UNITSYS_BASE_STRESS;
        dRatio_3_94 = D_UNITSYS_NONE;

        dSigma_max_ef = D_UNITSYS_BASE_STRESS;
        dGamwRym_lim = D_UNITSYS_BASE_STRESS;
        dRatio_4_57 = D_UNITSYS_NONE;
    }
    T_FATI_REBAR_UNIT(){Initialize();}

};

struct T_FATI_REBAR_CASE // After Composite Fatigue Rebar
{
    T_FATI_REBAR FatiRebar[2];  // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) FatiRebar[i].Initialize();
    }

    T_FATI_REBAR_CASE() {Initialize();}  
    T_FATI_REBAR_CASE(const T_FATI_REBAR_CASE &rData)	{*this = rData;}
    T_FATI_REBAR_CASE& operator = (const T_FATI_REBAR_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) FatiRebar[i] = rData.FatiRebar[i];
        return *this;
    }
};

struct T_FATI_REBAR_BASE // After Composite Fatigue Rebar
{
    //T_ELEM_K ElemK;
    //UINT     nPos;
    int      nLcomK;      // Lcom Key
    int      nMaxMin;     // 0=None, 2=Max/Min, 12=Concurrent
    int      nConcurrent; // Fx_max .... Mz_min(1~12) 
    int      nDgnLcomK;  
    int      nDgnLcomKFirst;  
    int      nLcomSize;

    void Initialize()
    {
        nLcomK = 0;     
        nMaxMin = 0; 
        nConcurrent = 0;
        nDgnLcomK = 0;  
        nDgnLcomKFirst = 0;  
        nLcomSize = 0;
    }
    T_FATI_REBAR_BASE() {Initialize();}  
    T_FATI_REBAR_BASE(const T_FATI_REBAR_BASE &rData)	{*this = rData;}
    T_FATI_REBAR_BASE& operator = (const T_FATI_REBAR_BASE &rData)
    {
        nLcomK = rData.nLcomK;  
        nMaxMin = rData.nMaxMin; 
        nConcurrent = rData.nConcurrent;
        nDgnLcomK = rData.nDgnLcomK;  
        nDgnLcomKFirst = rData.nDgnLcomKFirst;  
        nLcomSize = rData.nLcomSize;

        return *this;
    }
};

struct T_FATI_REBAR_CR // After Composite Fatigue Rebar
{
    T_FATI_REBAR_BASE FatiRebarCR[2];  // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) FatiRebarCR[i].Initialize();
    }

    T_FATI_REBAR_CR() {Initialize();}  
    T_FATI_REBAR_CR(const T_FATI_REBAR_CR &rData)	{*this = rData;}
    T_FATI_REBAR_CR& operator = (const T_FATI_REBAR_CR &rData)
    {
        for(int i = 0; i < 2; ++i) FatiRebarCR[i] = rData.FatiRebarCR[i];

        return *this;
    }
};

struct T_FATI_REBAR_RW // After Composite Fatigue Rebar
{
    BOOL bCheck;
    double dRatio; // Larger ratio
    BOOL bOK;

    BOOL bCheck_3_94; // Checked by Item 3.94
    double dSigmap;
    double dmsRs_lim;
    double dRatio_3_94;

    BOOL bCheck_4_57; // Checked by Item 4.57
    double dSigma_max_ef;
    double dGamwRym_lim;
    double dRatio_4_57;  

    void Initialize()
    {
        bCheck = FALSE;
        dRatio = 0.0; // Larger ratio
        bOK = FALSE;

        bCheck_3_94 = FALSE; // Checked by Item 3.94
        dSigmap = 0.0;
        dmsRs_lim = 0.0;
        dRatio_3_94 = 0.0;

        bCheck_4_57 = FALSE; // Checked by Item 4.57
        dSigma_max_ef = 0.0;
        dGamwRym_lim = 0.0;
        dRatio_4_57 = 0.0;  
    }

    T_FATI_REBAR_RW() {Initialize();}  



    void GetData(T_FATI_REBAR &rData)
    {
        rData.bCheck = bCheck;
        rData.dRatio = dRatio; 
        rData.bOK = bOK;

        rData.bCheck_3_94 = bCheck_3_94; 
        rData.dSigmap = dSigmap;
        rData.dmsRs_lim = dmsRs_lim;
        rData.dRatio_3_94 = dRatio_3_94;

        rData.bCheck_4_57 = bCheck_4_57; 
        rData.dSigma_max_ef = dSigma_max_ef;
        rData.dGamwRym_lim = dGamwRym_lim;
        rData.dRatio_4_57 = dRatio_4_57;  
    }

    void SetData(const T_FATI_REBAR &rData)
    {
        bCheck = rData.bCheck;
        dRatio = rData.dRatio; 
        bOK = rData.bOK;

        bCheck_3_94 = rData.bCheck_3_94; 
        dSigmap = rData.dSigmap;
        dmsRs_lim = rData.dmsRs_lim;
        dRatio_3_94 = rData.dRatio_3_94;

        bCheck_4_57 = rData.bCheck_4_57; 
        dSigma_max_ef = rData.dSigma_max_ef;
        dGamwRym_lim = rData.dGamwRym_lim;
        dRatio_4_57 = rData.dRatio_4_57;  
    }
};


struct T_FATI_REBAR_CASE_RW // After Composite Fatigue Rebar
{
    T_FATI_REBAR_RW FatiRebar[2]; // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) FatiRebar[i].Initialize();
    }

    T_FATI_REBAR_CASE_RW() {Initialize();}  

    void GetData(T_FATI_REBAR_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) FatiRebar[i].GetData(rData.FatiRebar[i]);
    }
    void SetData(const T_FATI_REBAR_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) FatiRebar[i].SetData(rData.FatiRebar[i]);
    }
};

struct T_FATI_STL_GIRDER // After Composite Fatigue Steel Girder
{
    BOOL bCheck;
    BOOL bOK;
    BOOL bTopCritical;
    double dRatio;
    double dSigmas_Top;
    double dGammawRy_Top;
    double dRatio_Top;
    double dSigmas_Bot;
    double dGammawRy_Bot;
    double dRatio_Bot;

    void Initialize()
    {
        bCheck = FALSE;
        bOK = FALSE;
        bTopCritical = FALSE;
        dRatio = 0.0;
        dSigmas_Top = 0.0;
        dGammawRy_Top = 0.0;
        dRatio_Top = 0.0;
        dSigmas_Bot = 0.0;
        dGammawRy_Bot = 0.0;
        dRatio_Bot = 0.0;
    }

    T_FATI_STL_GIRDER() {Initialize();}  
    T_FATI_STL_GIRDER(const T_FATI_STL_GIRDER &rData)	{*this = rData;}
    T_FATI_STL_GIRDER& operator = (const T_FATI_STL_GIRDER &rData)
    {
        bCheck = rData.bCheck;
        bOK = rData.bOK;
        bTopCritical = rData.bTopCritical;
        dRatio = rData.dRatio;
        dSigmas_Top = rData.dSigmas_Top;
        dGammawRy_Top = rData.dGammawRy_Top;
        dRatio_Top = rData.dRatio_Top;
        dSigmas_Bot = rData.dSigmas_Bot;
        dGammawRy_Bot = rData.dGammawRy_Bot;
        dRatio_Bot = rData.dRatio_Bot;
        return *this;
    }
};

struct T_FATI_STL_GIRDER_UNIT
{
    int dRatio;
    int dSigmas_Top;
    int dGammawRy_Top;
    int dRatio_Top;
    int dSigmas_Bot;
    int dGammawRy_Bot;
    int dRatio_Bot;

    void Initialize()
    {
        dRatio = D_UNITSYS_NONE;
        dSigmas_Top = D_UNITSYS_BASE_STRESS;
        dGammawRy_Top = D_UNITSYS_BASE_STRESS;
        dRatio_Top = D_UNITSYS_NONE;
        dSigmas_Bot = D_UNITSYS_BASE_STRESS;
        dGammawRy_Bot = D_UNITSYS_BASE_STRESS;
        dRatio_Bot = D_UNITSYS_NONE;
    }

    T_FATI_STL_GIRDER_UNIT(){Initialize();}
};

struct T_FATI_STL_GIRDER_CASE // After Composite Fatigue Steel Girder
{
    T_FATI_STL_GIRDER FatiStlGirder[2]; // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) FatiStlGirder[i].Initialize();
    }

    T_FATI_STL_GIRDER_CASE() {Initialize();}  
    T_FATI_STL_GIRDER_CASE(const T_FATI_STL_GIRDER_CASE &rData)	{*this = rData;}
    T_FATI_STL_GIRDER_CASE& operator = (const T_FATI_STL_GIRDER_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) FatiStlGirder[i] = rData.FatiStlGirder[i];
        return *this;
    }
};

struct T_FATI_STL_GIRDER_BASE // After Composite Fatigue Steel Girder
{
    //T_ELEM_K ElemK;
    //UINT     nPos;
    int      nLcomK;      // Lcom Key
    int      nMaxMin;     // 0=None, 2=Max/Min, 12=Concurrent
    int      nConcurrent; // Fx_max .... Mz_min(1~12) 
    int      nDgnLcomK;  
    int      nDgnLcomKFirst;  
    int      nLcomSize;

    void Initialize()
    {
        nLcomK = 0;     
        nMaxMin = 0; 
        nConcurrent = 0;
        nDgnLcomK = 0;  
        nDgnLcomKFirst = 0;  
        nLcomSize = 0;
    }
    T_FATI_STL_GIRDER_BASE() {Initialize();}  
    T_FATI_STL_GIRDER_BASE(const T_FATI_STL_GIRDER_BASE &rData)	{*this = rData;}
    T_FATI_STL_GIRDER_BASE& operator = (const T_FATI_STL_GIRDER_BASE &rData)
    {
        nLcomK = rData.nLcomK;  
        nMaxMin = rData.nMaxMin; 
        nConcurrent = rData.nConcurrent;
        nDgnLcomK = rData.nDgnLcomK;  
        nDgnLcomKFirst = rData.nDgnLcomKFirst;  
        nLcomSize = rData.nLcomSize;

        return *this;
    }
};

struct T_FATI_STL_GIRDER_CR // After Composite Fatigue Steel Girder
{
    T_FATI_STL_GIRDER_BASE FatiStlGirderCR[2]; // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) FatiStlGirderCR[i].Initialize();
    }

    T_FATI_STL_GIRDER_CR() {Initialize();}  
    T_FATI_STL_GIRDER_CR(const T_FATI_STL_GIRDER_CR &rData)	{*this = rData;}
    T_FATI_STL_GIRDER_CR& operator = (const T_FATI_STL_GIRDER_CR &rData)
    {
        for(int i = 0; i < 2; ++i) FatiStlGirderCR[i] = rData.FatiStlGirderCR[i];
        return *this;
    }

};

struct T_FATI_STL_GIRDER_RW // After Composite Fatigue Steel Girder
{
    BOOL bCheck;
    BOOL bOK;
    BOOL bTopCritical;
    double dRatio;
    double dSigmas_Top;
    double dGammawRy_Top;
    double dRatio_Top;
    double dSigmas_Bot;
    double dGammawRy_Bot;
    double dRatio_Bot;

    void Initialize()
    {
        bCheck = FALSE;
        bOK = FALSE;
        bTopCritical = FALSE;
        dRatio = 0.0;
        dSigmas_Top = 0.0;
        dGammawRy_Top = 0.0;
        dRatio_Top = 0.0;
        dSigmas_Bot = 0.0;
        dGammawRy_Bot = 0.0;
        dRatio_Bot = 0.0;
    }

    T_FATI_STL_GIRDER_RW() {Initialize();}  

    void GetData(T_FATI_STL_GIRDER &rData)
    {
        rData.bCheck = bCheck;
        rData.bOK = bOK;
        rData.bTopCritical = bTopCritical;
        rData.dRatio = dRatio;
        rData.dSigmas_Top = dSigmas_Top;
        rData.dGammawRy_Top = dGammawRy_Top;
        rData.dRatio_Top = dRatio_Top;
        rData.dSigmas_Bot = dSigmas_Bot;
        rData.dGammawRy_Bot = dGammawRy_Bot;
        rData.dRatio_Bot = dRatio_Bot;
    }
    void SetData(const T_FATI_STL_GIRDER &rData)
    {
        bCheck = rData.bCheck;
        bOK = rData.bOK;
        bTopCritical = rData.bTopCritical;
        dRatio = rData.dRatio;
        dSigmas_Top = rData.dSigmas_Top;
        dGammawRy_Top = rData.dGammawRy_Top;
        dRatio_Top = rData.dRatio_Top;
        dSigmas_Bot = rData.dSigmas_Bot;
        dGammawRy_Bot = rData.dGammawRy_Bot;
        dRatio_Bot = rData.dRatio_Bot;
    }

};


struct T_FATI_STL_GIRDER_CASE_RW // After Composite Fatigue Steel Girder
{
    T_FATI_STL_GIRDER_RW FatiStlGirder[2]; // I & J

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) FatiStlGirder[i].Initialize();
    }

    T_FATI_STL_GIRDER_CASE_RW() {Initialize();}  

    void GetData(T_FATI_STL_GIRDER_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) FatiStlGirder[i].GetData(rData.FatiStlGirder[i]);
    }

    void SetData(const T_FATI_STL_GIRDER_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) FatiStlGirder[i].SetData(rData.FatiStlGirder[i]);
    }
};


struct T_CRACK_CONC // After Composite Crack Concrete
{
    BOOL bCheck;
    BOOL bOK;
    BOOL bPositiveChk;
    double dSigbf;
    double dmbRb_mc2;  
    double dRatio_Po;

    BOOL bNegativeChk;
    double dSigma_cr; // Crack Stress
    double da_cr; // Crack Width calculated by Stress, Demand
    double dDelta_crd; // Crack Opening Limit, Default 0.03cm, SNiP 5.27, Capacity
    double dRatio_Ne;

    double dRatio;

    void Initialize()
    {
        bCheck = FALSE;
        bOK = FALSE;
        bPositiveChk =  FALSE;
        dSigbf  = 0.0;
        dmbRb_mc2 = 0.0;
        dRatio_Po = 0.0;

        bNegativeChk = FALSE;
        dSigma_cr = 0.0; 
        da_cr = 0.0;
        dDelta_crd = 0.0;
        dRatio_Ne = 0.0;
        dRatio = 0.0;
    }

    T_CRACK_CONC() {Initialize();}  
    T_CRACK_CONC(const T_CRACK_CONC &rData)	{*this = rData;}
    T_CRACK_CONC& operator = (const T_CRACK_CONC &rData)
    {
        bCheck = rData.bCheck;
        bOK = rData.bOK;

        bPositiveChk = rData.bPositiveChk;
        dSigbf  = rData.dSigbf;
        dmbRb_mc2 = rData.dmbRb_mc2;
        dRatio_Po = rData.dRatio_Po;

        bNegativeChk = rData.bNegativeChk;
        dSigma_cr = rData.dSigma_cr; 
        da_cr = rData.da_cr;
        dDelta_crd = rData.dDelta_crd;
        dRatio_Ne = rData.dRatio_Ne;

        dRatio = rData.dRatio;

        return *this;
    }
};

struct T_CRACK_CONC_UNIT
{
    int dSigma_cr; // Crack Stress
    int da_cr; // Crack Width calculated by Stress, Demand
    int dDelta_crd; // Crack Opening Limit, Default 0.03cm, SNiP 5.27, Capacity
    int dRatio;

    void Initialize()
    {
        dSigma_cr = D_UNITSYS_BASE_STRESS; 
        da_cr = D_UNITSYS_BASE_LENGTH; 
        dDelta_crd = D_UNITSYS_BASE_LENGTH; 
        dRatio = D_UNITSYS_NONE; 
    }

    T_CRACK_CONC_UNIT() {Initialize();}  
};


struct T_CRACK_CONC_CASE // After Composite Crack Concrete
{
    T_CRACK_CONC CrackConc[2];

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) CrackConc[i].Initialize();
    }

    T_CRACK_CONC_CASE() {Initialize();}  
    T_CRACK_CONC_CASE(const T_CRACK_CONC_CASE &rData)	{*this = rData;}
    T_CRACK_CONC_CASE& operator = (const T_CRACK_CONC_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) CrackConc[i] = rData.CrackConc[i];

        return *this;
    }

};

struct T_CRACK_CONC_BASE // After Composite Crack Concrete
{
    //T_ELEM_K ElemK;
    //UINT     nPos;
    int      nLcomK;      // Lcom Key
    int      nMaxMin;     // 0=None, 2=Max/Min, 12=Concurrent
    int      nConcurrent; // Fx_max .... Mz_min(1~12) 
    int      nDgnLcomK;  
    int      nDgnLcomKFirst;  
    int      nLcomSize;

    void Initialize()
    {
        nLcomK = 0;     
        nMaxMin = 0; 
        nConcurrent = 0;
        nDgnLcomK = 0;  
        nDgnLcomKFirst = 0;  
        nLcomSize = 0;
    }
    T_CRACK_CONC_BASE() {Initialize();}  
    T_CRACK_CONC_BASE(const T_CRACK_CONC_BASE &rData)	{*this = rData;}
    T_CRACK_CONC_BASE& operator = (const T_CRACK_CONC_BASE &rData)
    {
        nLcomK = rData.nLcomK;  
        nMaxMin = rData.nMaxMin; 
        nConcurrent = rData.nConcurrent;
        nDgnLcomK = rData.nDgnLcomK;  
        nDgnLcomKFirst = rData.nDgnLcomKFirst;  
        nLcomSize = rData.nLcomSize;

        return *this;
    }
};


struct T_CRACK_CONC_CR // After Composite Crack Concrete
{
    T_CRACK_CONC_BASE CrackConcCR[2];

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) CrackConcCR[i].Initialize();
    }

    T_CRACK_CONC_CR() {Initialize();}  
    T_CRACK_CONC_CR(const T_CRACK_CONC_CR &rData)	{*this = rData;}
    T_CRACK_CONC_CR& operator = (const T_CRACK_CONC_CR &rData)
    {
        for(int i = 0; i < 2; ++i) CrackConcCR[i] = rData.CrackConcCR[i];
        return *this;
    }

};

struct T_CRACK_CONC_RW // After Composite Crack Concrete
{

    BOOL bCheck;
    BOOL bOK;
    double dSigma_cr; // Crack Stress
    double da_cr; // Crack Width calculated by Stress, Demand
    double dDelta_crd; // Crack Opening Limit, Default 0.03cm, SNiP 5.27, Capacity
    double dRatio;

    void Initialize()
    {
        bCheck = FALSE;
        bOK = FALSE;
        dSigma_cr = 0.0; 
        da_cr = 0.0;
        dDelta_crd = 0.0;
        dRatio = 0.0;
    }

    T_CRACK_CONC_RW() {Initialize();}  

    void GetData(T_CRACK_CONC &rData)
    {
        rData.bCheck = bCheck;
        rData.bOK = bOK;
        rData.dSigma_cr = dSigma_cr; 
        rData.da_cr = da_cr;
        rData.dDelta_crd = dDelta_crd;
        rData.dRatio = dRatio;
    }
    void SetData(const T_CRACK_CONC &rData)
    {
        bCheck = rData.bCheck;
        bOK = rData.bOK;
        dSigma_cr = rData.dSigma_cr; 
        da_cr = rData.da_cr;
        dDelta_crd = rData.dDelta_crd;
        dRatio = rData.dRatio;
    }  
};


struct T_CRACK_CONC_CASE_RW // After Composite Crack Concrete
{
    T_CRACK_CONC_RW CrackConc[2];

    void Initialize()
    {
        for(int i = 0; i < 2; ++i) CrackConc[i].Initialize();
    }

    T_CRACK_CONC_CASE_RW() {Initialize();}  

    void GetData(T_CRACK_CONC_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) CrackConc[i].GetData(rData.CrackConc[i]);
    }
    void SetData(const T_CRACK_CONC_CASE &rData)
    {
        for(int i = 0; i < 2; ++i) CrackConc[i].SetData(rData.CrackConc[i]);
    }
};

#pragma pack()

#endif