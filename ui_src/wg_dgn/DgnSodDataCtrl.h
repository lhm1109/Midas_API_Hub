// DgnSodDataCtrl.h: interface for the CDgnSodDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNSODDataCtrl_H__INCLUDED_)
#define AFX_DGNSODDataCtrl_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnPscCommon.h"
#include "Dgn_OrthotropicDeckManager.h"
#include "..\wg_db\DB_ST_DN_RESULT_SOD.H"

//#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"


#include "HeaderPre.h"

class CStlOrthDeckDesign;

struct T_SODSTFN_INX_MAP
{
	CMap<CString,LPCTSTR,int,int&> mStfnIdx;
	void Initialize()
	{
		mStfnIdx.RemoveAll();
	}
	T_SODSTFN_INX_MAP() {Initialize();}
	T_SODSTFN_INX_MAP(const T_SODSTFN_INX_MAP& rData) { *this = rData; }
 	T_SODSTFN_INX_MAP& operator = (const T_SODSTFN_INX_MAP &rData)
	{
		POSITION Pos = rData.mStfnIdx.GetStartPosition();
		while(Pos)
		{
			CString sStfnName=_T("");
			int nStfnIdx=0;      
			rData.mStfnIdx.GetNextAssoc(Pos, sStfnName, nStfnIdx);
			
			mStfnIdx.SetAt(sStfnName, nStfnIdx);
		}
		return *this;
	}  
};
struct T_SODSTFN_INX_D
{
	T_SODSTFN_INX_MAP StfnTL;
	T_SODSTFN_INX_MAP StfnTM;
	T_SODSTFN_INX_MAP StfnTR;
	T_SODSTFN_INX_MAP StfnWL;
	T_SODSTFN_INX_MAP StfnWR;
	T_SODSTFN_INX_MAP StfnBL;
	T_SODSTFN_INX_MAP StfnBM;
	T_SODSTFN_INX_MAP StfnBR;

	void Initialize()
	{
		StfnTL.Initialize();
		StfnTM.Initialize();
		StfnTR.Initialize();
		StfnWL.Initialize();
		StfnWR.Initialize();
		StfnBL.Initialize();
		StfnBM.Initialize();
		StfnBR.Initialize();
	}

	T_SODSTFN_INX_D() {Initialize();}
	T_SODSTFN_INX_D(const T_SODSTFN_INX_D& rData) { *this = rData; }
 	T_SODSTFN_INX_D& operator = (const T_SODSTFN_INX_D &rData)
	{
		StfnTL = rData.StfnTL;
		StfnTM = rData.StfnTM;
		StfnTR = rData.StfnTR;
		StfnWL = rData.StfnWL;
		StfnWR = rData.StfnWR;
		StfnBL = rData.StfnBL;
		StfnBM = rData.StfnBM;
		StfnBR = rData.StfnBR;
		return *this;
	}  
};

struct T_SODSECT_STFN_INX
{
	T_SODSTFN_INX_D Stiff[2]; // 0=I, 1=J

	void Initialize()
	{
		for(int i=0; i<2; ++i) Stiff[i].Initialize();
	}
	T_SODSECT_STFN_INX() {Initialize();}
	T_SODSECT_STFN_INX(const T_SODSECT_STFN_INX& rData) { *this = rData; }
 	T_SODSECT_STFN_INX& operator = (const T_SODSECT_STFN_INX &rData)
	{
		for(int i=0; i<2; ++i) Stiff[i] = rData.Stiff[i];
		return *this;
	}  

};

class __MY_EXT_CLASS__ CDgnSodDataCtrl : public CDgnPscCommon
{
	// Member Functions.
public:
	CDgnSodDataCtrl();
	virtual ~CDgnSodDataCtrl();
	
	virtual void SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl);
	//void SetDesignCode(int nDgnCode);  
	
public:
	BOOL InitialData(int nDgnCode);
	void SetSodDgnCode(int nDgnCode);
	void SetChkSodDgnElem(CArray<T_ELEM_K,T_ELEM_K> &raElemK);
	BOOL Make_SodDgnData();

	BOOL Check_StlOrthotropicDeck();

	BOOL Get_SodPosData(T_ELEM_K ElemK, T_SECT_K SectK, int nStype, int nPos, STL_MEMB_POSD_OD &rData);
	BOOL Get_LocalForceData(T_ELEM_K ElemK, T_LCOM_K LcomK, int nPos, STL_MEMB_POSD_SNiP_OD &rData);
	BOOL Get_UnbracedLengthData(T_ELEM_K ElemK, BOOL bKfac, T_KFAC_D &KFacD, STL_MEMB_ULEN &rData);
	BOOL Get_UnbracedLengthData(T_ELEM_K ElemK, STL_MEMB_ULEN &rData, BOOL &bUnLeng, BOOL &bEffLengF);

	BOOL GetLcom4Kapa(int nOrgLcomNo, ADGNFORCE& aForceLcom, ADGNFORCE& aForceLTerm, 
		                STLCHKFOR_ARRAY& aChkForceI, STLCHKFOR_ARRAY& aChkForceJ, STLCHKFOR_ARRAY& aChkDLForceI, STLCHKFOR_ARRAY& aChkDLForceJ, int& nLcomSize, int& nDgnLcomKFirst);
	BOOL GetDgnLengD();
	BOOL GetDgnLengDFromMemb(T_ELEM_K ElemK, const T_MEMB_D &MembD, T_EDLK_D &DgnLengD);
	BOOL GetDgnLengDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, T_EDLK_D &DgnLengD);
	BOOL GetDgnSuptLengDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, double &dSpanLength);
	BOOL GetDgnSuptTypeDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, int nSpanEndSupType1YZ[2], int nSpanEndSupType2YZ[2]);
	BOOL GetDgnElemLocDInSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, const double dSpanLength, const int nStartElemIndex, const int nEndElemIndex, int nElemLocaD[2]); // I & J
	BOOL GetSodFlexuralBucklingData(T_ELEM_K ElemK, int nIJ, int nDgnLcomK, STL_MEMB_POSD_SNiP_OD &MembLcomD); 
	void ConvertForceEx(const _DGN_FORC_CRC_EX& crInD, T_SODD_FORCE_EX& rData);
	BOOL ConvertSodDgnLengthData(const T_EDLK_D &DgnLengD, STL_RES_BASE_SNiP_OD_SNiP &rData);
	BOOL Make_ElemSpanData();

protected:
	//BOOL Make_ElemSpanMapKey();
	//BOOL Make_ElemSpanData();
	BOOL Make_SodMatlDgnData();
	BOOL Make_SodSectDgnData();
	BOOL Make_SodKappaFactorDgnData();
	BOOL Make_SodKappaFactorChkPos();
	BOOL Make_SodFatiCheckPos();
	BOOL Make_SodLocalForce4Deck();
	BOOL Make_SodFloorBeamDgnData();
	BOOL Make_SodWebPlateDgnData();
	BOOL Make_SodWebVerticalForce();
	BOOL Make_SodSectCheckPart();

	BOOL Check_SOD_Fatigue(T_ELEM_K ElemK,                                        
												 BOOL bChkIJ[],
												 STL_MEMB_POSD_SNiP_OD &MembLcomD,
												 STL_RES_BASE_SNiP_OD_SNiP *pMembResD,
												 ADGNFORCE &aForceLcom,
												 ADGNFORCE &aForceLTerm, 
												 BOOL &bSodCheck);
	

	void GetCrFlgStrnRibRatio  (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNRibRat, double dBFSNRibRat, double &dTFlgSNRibMaxR, double &dBFlgSNRibMaxR, T_SDCR_BASE &rTRib, T_SDCR_BASE &rBRib);
	void GetCrFlgStrnDeckRatio (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNDeckRat, double dBFSNDeckRat, double &dTFlgSNDeckMaxR, double &dBFlgSNDeckMaxR, T_SDCR_BASE &rTDeck, T_SDCR_BASE &rBDeck);
	void GetCrFlgStrnFBeamRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNFBeamRat, double dBFSNFBeamRat, double &dTFlgSNFBeamMaxR, double &dBFlgSNFBeamMaxR, T_SDCR_BASE &rTFbeam, T_SDCR_BASE &rBFbeam);
	void GetCrWebStrnRibRatio  (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSNRibRat, double &dWebSNRibMaxR, T_SDCR_BASE &rWRib);
	void GetCrWebStrnDeckRatio (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSNDeckRat, double &dWebSNDeckMaxR, T_SDCR_BASE &rWDeck);
	 // 
	void GetCrFlgStabRibRatio  (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBRibRat, double dBFSBRibRat, double &dTFlgSBRibMaxR, double &dBFlgSBRibMaxR, T_SDCR_BASE &rTRib,  T_SDCR_BASE &rBRib);
	void GetCrFlgStabDeckRatio (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBDeckRat, double dBFSBDeckRat, double &dTFlgSBDeckMaxR, double &dBFlgSBDeckMaxR, T_SDCR_BASE &rTDeck, T_SDCR_BASE &rBDeck);
	void GetCrFlgStabFBeamRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBFBeamRat, double dBFSBFBeamRat, double &dTFlgSBFBeamMaxR, double &dBFlgSBFBeamMaxR, T_SDCR_BASE &rTFbeam, T_SDCR_BASE &rBFbeam);
	void GetCrWebStabRibRatio  (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSBRibRat, double &dWebSBRibMaxR, T_SDCR_BASE &rWRib);
	void GetCrWebStabDeckRatio (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSBDeckRat, double &dWebSBDeckMaxR, T_SDCR_BASE &rWDeck);
	void GetCrGDStabRatio      (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dStbGDRat,     double &dSBGDMaxR,      T_SDCR_BASE &rGDCr);
	
	 //
	void GetCrFatigueRatio(int nFatiCt, const T_SDCR_BASE &LcomKeyD, double dFatiRat, double &dFatiMaxR, T_SDCR_BASE &rData);



	void SetSodCrResultKey(int nLcomK, int nMaxMin, int nConcurrent, int nDgnLcomK, T_SDCR_BASE &rData);


// Convert Data
public:
	void ConvertSODGlobalData(STL_GDGND_OD_D &rData);  

protected:
	void ConvertSodMatlData();          // Material
	void ConvertSodMatl4SectPart();     // Material Section
	void ConvertSodMatlProp(T_MATD_D &MatD, STL_MATL_SNiP_PROP &rData);
	void ConvertSodMatlStlPart(T_MATL_ANALYSIS &AnalMatl, T_MATL_DESIGN &DgnMatl, STL_MATL_SNiP &rData);
	void ConvertSodSectSize(int nStype, T_SECT_SECTBASE_D *pSectAfter, T_SECT_SECTBASE_D *pSectD, ST_SECT_SECTBASE_D &rData);
	void ConvertSodSectData(int nPos, T_SECT_D *pSectD, T_STPO_D &StpoD, T_SODSTFN_INX_D &rStfnId, STL_SECT_INFO_OD &rData);          // Section
	void ConvertSodSectBaseData(int nStype, T_SECT_SECTBASE_D *pSectAfter, T_SECT_SECTBASE_D *pSectD, T_STPO_D &StpoD, T_SODSTFN_INX_D &rStfnId, ST_SECT_SECTBASE_D &rData);
	BOOL ConvertSodSectPolygonData(int nPos, T_SECT_D *pSectD, ST_SECT_SECTBASE_D &rData);
		
	void ConvertSodKappaFactor(T_SDPS_SUB_D &InD, STL_KAPPA_INPUT_D &rData);       // Limited Plastic Strain (kappa factor)
	void ConvertSodKappaFactorChkPos(T_SDPS_SUB_D &InD, STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD &rData); // Check of Assumption of Plastic Strains
	void ConvertSodKappaFactorChkPosOne(T_POINT_SSM &InD, STL_MEMB_POSD_SNiP_CHK_KAPPA &rData);
	void ConvertSodKappaFactorChkPosDefault(CArray<T_POINT_SSM, T_POINT_SSM&>& aStressPoint, STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD &rData); // Check of Assumption of Plastic Strains
	void ConvertSodFatiCheckPos(int nPos, T_SDFC_D &InD, STL_POSD_DGND_SNiP_FAT_CHK_INFO_OD &rData); 
	void ConvertSodFatiCheckPosOne(T_SDFC_SUB_D &InD, STL_MEMB_POSD_SNiP_FAT &rData);      // Fatigue Check Position 
	void ConvertSodLocalForce4Deck(T_SECT_K SectK, int nPos, T_SDLF_D &InD, STL_LFORCE_RIBDECK_CHKUNIT_LCOM &rData);   // Local Forces in the Top Orthotropic Deck
	void ConvertSodLocalForce4DeckBase(T_SDLF_SUB_D &InD, STL_LFORCE_RIBDECK_D &rData);
	void ConvertSodLocalForceArraySize(int nPos, T_SODSECT_STFN_INX &InD, STL_LFORCE_RIBDECK_CHKUNIT_D &rData);
	bool ConvertSodFloorBeam(int nPos, T_SDTS_D &InD, STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D &rData);
	bool ConvertSodFloorBeamPart(int nPos, CString &strFBPartName, T_SDTS_D &InD, STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_D &rData);         // Floor beam
	void ConvertSodWebPlateInfo();      // Additional Properties for the Girder Web - Web plate
	void ConvertSodWebVerticalForce(int nStype, int nPos, T_SDAP_D &InD, STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM &rForWeb, STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM &rForWebP);  // Additional Properties for the Girder Web - vertical force on a web
	void CovertSodWebPlateForceBase(T_SDAP_PROP_D &InD, STL_LFORCE_WEB_PLATE_D &rData);
	void ConvertSodWebVerticalLcomForce(int nStype, T_SDAP_SUB_D &VerFor, STL_LFORCE_WEB_SIGY_CHKUNIT_D &rData);
	void ConvertSodWebPlateForce(int nStype, double dBeta, int nConnectType, T_SDAP_SUB_D &VerFor, STL_LFORCE_WEB_PLATE_CHKUNIT_D &rData);
	void ConvertSodWebVerticalForceBase(int nWeb, T_SDAP_VERT_D &InD, STL_LFORCE_WEB_SIGY_D &rData);
	void ConvertSodSectCheckPart(int nStype, int nPos, T_SDDP_D &InD, STL_CHK_POSD_SNiP_OD &rData);     // Define Flanges and Webs for Steel Section (Design position과 연관)

	void ConvertSodCheckForce(int nPos, _DGN_FORC_CRC &For, STL_FORCE_D &rData);
	void ConvertSodMemberForce(_DGN_FORC_MEMB &MFor,  STL_ULMF_D &rData);
	void ConvertSODDFORCE(int nPos, _DGN_FORC_CRC &For, T_SODD_FORCE &rData);
	void ConvertSodCalcChkForce(STL_FORCE_D &ForOD, STLCHKFOR_ARRAY &aChkDLForce, STL_MEMB_POSD_SNiP_OD &rData);
	void ConvertSodCalcChkForce4Kapa(STLCHKFOR_ARRAY& aChkForce, STLCHKFOR_ARRAY& aChkDLForce, STL_MEMB_POSD_SNiP_OD &MembLcomD);
	void ConvertSod5PosD(const _DGN_FORC_CRC_EX& ForD, STL_POSD_DGND_SNiP_MEMB& rData);
	void MakeSectStfnIndexInfo(int nStype, T_SECT_SECTBASE_D *pSectD, T_SODSTFN_INX_D &rData);
	void MakeSectStfnPropileIndex(int nStype, T_SECT_SECTBASE_D *pSectD, T_SECT_SOD_STIFF_SUB_D &PartStfn, T_SODSTFN_INX_D &rData);	
	CString GetDeckPartName(int nId);
	
	// Result
	void ConvertSodSdsnFlg(int nStype, BOOL bIsTop, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSN_FLG &rData, BOOL &bSodCheck);
	void ConvertSodSdsnWeb(int nStype, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSN_WEB &rData, BOOL &bSodCheck);
	void ConvertSodSdsbFlg(int nStype, BOOL bIsTop, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSB_FLG &rData, BOOL &bSodCheck);
	void ConvertSodSdsbWeb(int nStype, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSB_WEB &rData, BOOL &bSodCheck);
	void ConvertSodSdFati(STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_SNiP_FAT &LcomRes, T_SDFG_BASE &rData);
	void ConvertSodSdsbFB(int nStype, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSB_GD &rData, BOOL &bSodCheck);
	void ConvertSodSdsbFTB(int nStype, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSB_GD &rData, BOOL &bSodCheck);
	void ConvertSodSdsbGD(int nStype, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSB_GD &rData, BOOL &bSodCheck);

	void ConvertStrnFlgPartEngToCVL(STL_MEMB_RES_SNiP_STR_DP_CHK_UNIT &StrnPartR, T_SDPT_STRN_FLG &rData);
	void ConvertStrnLRibEngToCVL(STL_MEMB_RES_SNiP_STR_RIB &RibR, STL_MEMB_RES_SNiP_STR_DP &DeckR, T_SDLS_STRN_FLG &rData);
	void ConvertStrnWebPartEngToCVL(STL_MEMB_RES_SNiP_STR_WEB_CHK_UNIT &StrnWebR, T_SDPT_STRN_WEB &rData);
	void ConvertStrnWebPlateEngToCVL(STL_MEMB_RES_SNiP_STR_WEB_RIB &WebP, T_SDPS_STRN_WEB &rData);
	void ConvertStabFlgPartEngToCVL(STL_MEMB_RES_SNiP_STA_DP_CHK_UNIT &StabPartR, T_SDPT_STAB_FLG &rData);
	void ConvertStabLRibEngToCVL(STL_MEMB_RES_SNiP_STA_RIB_CHK_UNIT &RibR, T_SDSB_SECT_NOLS &rData, int &nRibPart_cr, double &dRat_Rib_Temp, BOOL &bChk_Rib);
	void ConvertStabDeckEngToCVL(STL_MEMB_RES_SNiP_STA_LOCAL_BTR &DeckR, T_SDSB_PAN &rData);
	void ConvertStabWebPartEngToCVL(STL_MEMB_RES_SNiP_STA_WEB &StabWebR, T_SDPT_STAB_WEB &rData);
	void ConvertStabWebPanEngToCVL(STL_MEMB_RES_SNiP_STA_WEB_PLATE &WebP, T_SDPS_STAB_WEB_PAN &rData);

public:
	int m_nDgnCode;
	
	CDgn_OrthotropicDeckManager *m_pSodManager;


//protected:  

	CMap<T_SECT_K,T_SECT_K,T_SODSECT_STFN_INX,T_SODSECT_STFN_INX&> m_SectStfnIdx;

	//CMap<T_MATL_K,T_MATL_K,SLT_MATL_SNiP_POSD,SLT_MATL_SNiP_POSD&>  m_mSodMatl;
	CMap<T_SECT_K,T_SECT_K,SLT_MATL_SNiP_POSD,SLT_MATL_SNiP_POSD&>  m_mSodMatlSect;
	CMap<T_SECT_K,T_SECT_K,STL_SECT_ELEMD_SNiP_OD,STL_SECT_ELEMD_SNiP_OD&>      m_mSodSect;
	CMap<T_ELEM_K,T_ELEM_K,STL_SECT_ELEMD_SNiP_OD,STL_SECT_ELEMD_SNiP_OD&>      m_mSodSectElem; // Tapered Section Group 경우 요소별 사용하기 위해 
	CMap<T_ELEM_K,T_ELEM_K,STL_KAPPA_INPUT_D,STL_KAPPA_INPUT_D&>    m_mSodKappaFactor;
	CMap<T_ELEM_K,T_ELEM_K,STL_KAPPA_INPUT_D,STL_KAPPA_INPUT_D&>    m_mSodKappaFactorJ;
	CMap<T_ELEM_K,T_ELEM_K,STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD,STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD&>     m_mSodKappaChkPos;
	CMap<T_ELEM_K,T_ELEM_K,STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD,STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD&>     m_mSodKappaChkPosJ;
	CMap<T_ELEM_K,T_ELEM_K,STL_POSD_DGND_SNiP_FAT_CHK_INFO_OD,STL_POSD_DGND_SNiP_FAT_CHK_INFO_OD&>                 m_mSodFatiChkPos;
	CMap<T_ELEM_K,T_ELEM_K,STL_POSD_DGND_SNiP_FAT_CHK_INFO_OD,STL_POSD_DGND_SNiP_FAT_CHK_INFO_OD&>                 m_mSodFatiChkPosJ;
	CMap<T_ELEM_K,T_ELEM_K,STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D,STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D&> m_mSodFloorBeam;
	CMap<T_ELEM_K,T_ELEM_K,STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D,STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D&> m_mSodFloorBeamJ;
	
	CMap<T_ELEM_K,T_ELEM_K,STL_LFORCE_RIBDECK_CHKUNIT_LCOM,STL_LFORCE_RIBDECK_CHKUNIT_LCOM&>     m_mSodLForRibDeck;
	CMap<T_ELEM_K,T_ELEM_K,STL_LFORCE_RIBDECK_CHKUNIT_LCOM,STL_LFORCE_RIBDECK_CHKUNIT_LCOM&>     m_mSodLForRibDeckJ;
	CMap<T_ELEM_K,T_ELEM_K,STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM,STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM&>   m_mSodLForWeb;
	CMap<T_ELEM_K,T_ELEM_K,STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM,STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM&>   m_mSodLForWebJ;
	CMap<T_ELEM_K,T_ELEM_K,STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM,STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM&> m_mSodLForWebPlate;
	CMap<T_ELEM_K,T_ELEM_K,STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM,STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM&> m_mSodLForWebPlateJ;

	// Design Length Info.
	CMap<T_ELEM_K, T_ELEM_K, T_EDLK_D, T_EDLK_D&> m_mDgnLengD;

	// Span Info.
	CMap<T_ELEM_K,T_ELEM_K, T_SPAN_K,T_SPAN_K&> m_mElemSpanK; 
	CMap<T_ELEM_K,T_ELEM_K, T_SPAN_BASE,T_SPAN_BASE&> m_mElemSpanD; 
};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNSODDataCtrl_H__INCLUDED_)
