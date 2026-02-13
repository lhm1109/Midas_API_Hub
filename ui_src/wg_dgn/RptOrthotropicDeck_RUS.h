#if !defined(AFX_RptOrthotropicDeck_RUS_H__)
#define AFX_RptOrthotropicDeck_RUS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RptOrthotropicDeck.h"
#include "..\wg_base\Thread.h"
#include "..\wg_db\DBDoc.h"
//#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

// #include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
// #include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"
#include "..\dgnengine\src\DGNe_RUS\DgnCalc_RUS_STL_OD_Struct.h"

#include "DgnSodDataCtrl.h"
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptOrthotropicDeck_RUS : public CThread
{
public:
	CRptOrthotropicDeck_RUS();
	virtual ~CRptOrthotropicDeck_RUS();
	
	
protected:
	CDBDoc* m_pDoc;
	BOOL m_bStopExecute;
	CCRCDataCtrl *m_pDataCtrl;
	
public: 
	BOOL Execute_OrthotropicDeckRpt_RUS(int iDgnCode, CString sFileName, int iPrintOpt, BOOL bSaveImage);

	BOOL Set_SodForceData4Kapa(T_ELEM_K ElemK, int nIJ, CDgn_OrthotropicDeckManager* pSodManager, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_POSD_SNiP_OD &MembLcomD, aKAPAF_PonitInfo &KAPAF_PonitInfo, BOOL &bAllKappaF);
	BOOL Set_SodForceDataSTN_TFL_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomDKapa);
	BOOL Set_SodForceDataSTN_BFL_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomDKapa);
	BOOL Set_SodForceDataSTN_WEB_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomDKapa);
	BOOL Set_SodForceDataSTB_TFL_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomDKapa);
	BOOL Set_SodForceDataSTB_BFL_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomDKapa);
	BOOL Set_SodForceDataSTB_WEB_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomDKapa);

	//BOOL Set_SodForceDataSTN_Result(T_ELEM_K ElemK, int nIJ, CDgn_OrthotropicDeckManager* pSodManager, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_POSD_SNiP_OD &MembLcomDStn, STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_RES_SNiP_OD &RptData);  

	BOOL Set_SodForceDataSTN_Result_FLG(T_ELEM_K ElemK, int nIJ, CDgn_OrthotropicDeckManager* pSodManager, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_POSD_SNiP_OD &MembLcomDStn, STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_RES_SNiP_OD &RptData, BOOL bAllKappaF);  
	BOOL Set_SodForceDataSTN_Result_WLG(T_ELEM_K ElemK, int nIJ, CDgn_OrthotropicDeckManager* pSodManager, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_POSD_SNiP_OD &MembLcomDStn, STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_RES_SNiP_OD &RptData, BOOL bAllKappaF);  
	BOOL Set_SodForceDataSTN_Flange(T_ELEM_K ElemK, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDPT_STRN_FLG &SDSN_Result);  
	BOOL Set_SodForceDataSTN_Web(T_ELEM_K ElemK, int nIJ, BOOL bLeft, int nWebChkType, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDPT_STRN_WEB &SDSN_Result);  

	BOOL Set_SodForceDataSTB_Result_FLG(T_ELEM_K ElemK, int nIJ, CDgn_OrthotropicDeckManager* pSodManager, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_POSD_SNiP_OD &MembLcomDStb, STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_RES_SNiP_OD &RptData, BOOL bAllKappaF);  
	BOOL Set_SodForceDataSTB_Result_WLG(T_ELEM_K ElemK, int nIJ, CDgn_OrthotropicDeckManager* pSodManager, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_POSD_SNiP_OD &MembLcomDStb, STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_RES_SNiP_OD &RptData, BOOL bAllKappaF);  

	BOOL Set_SodForceDataSTB_FB(T_ELEM_K ElemK, int nIJ, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDSB_GD &SDSB_GDR); 
	BOOL Set_SodForceDataSTB_FTB(T_ELEM_K ElemK, int nIJ, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDSB_GD &SDSB_GDR); 

	BOOL Set_SodForceDataSTB_GD(T_ELEM_K ElemK, int nIJ, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDSB_GD &SDSB_GDR); 
	BOOL Set_SodForceDataSTB_Flange(T_ELEM_K ElemK, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDPT_STAB_FLG &SDSB_Result); 
	BOOL Set_SodForceDataSTB_Web(T_ELEM_K ElemK, int nIJ, BOOL bLeft, int nWebChkType, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDPT_STAB_WEB &SDSB_Result);  

	//BOOL Set_SodForceDataSTN(T_ELEM_K ElemK, int nIJ,  STL_MEMB_POSD_SNiP_OD  &MembLcomD);  
	//BOOL Set_SodForceDataSTB(T_ELEM_K ElemK, int nIJ,  STL_MEMB_POSD_SNiP_OD  &MembLcomD);  
	BOOL Set_SodForceDataFatigue(T_ELEM_K ElemK, int nIJ,  STL_MEMB_POSD_SNiP_OD  &MembLcomD);  
	void ConvertSodCheckForce(T_SODD_FORCE &Indata, STL_FORCE_D &Outdata);

	void ConvertSodForce2Engine(T_ELEM_K ElemK, int nIJ,  int nChkPos, T_SDCR_BASE SDCR_BASE, STL_MEMB_POSD_SNiP_OD &MembLcomD);
	void ConvertSodMemberForce2Engine(T_SODD_ULMF &MFor,  STL_ULMF_D &rData);
	void ConvertSodGlobalForce2Engine(T_ELEM_K ElemK, T_SODD_FORCE &Elem, T_SDCR_BASE SDCR_BASE,  STL_LCOM_FORCE_OD &ChkForOD);
	void ConvertSodGlobalForce2Engine_Kapa(T_ELEM_K ElemKey, int nIJ, T_SDCR_BASE SDCR_BASE, STL_LCOM_FORCE_OD &ChkForOD);
	void ConvertSodCalcChkForceRpt(STLCHKFOR_ARRAY& aChkForce, STLCHKFOR_ARRAY& aChkDLForce, STL_LCOM_FORCE_OD& ChkForOD);
	void Get_FlexBuckMembForceD(T_ELEM_K ElemKey, int nIJ, int nDgnLcomK, STL_MEMB_POSD_SNiP_OD &MembLcomD);
	void Get_FlexBuckMembForceD_Sub(T_ELEM_K ElemKey, int nDgnLcomK, STL_POSD_DGND_SNiP_MEMB &MembForExD);
	void ConvertForceEx2DgnExForD(const T_SODD_FORCE_EX& crInD,  STL_POSD_DGND_SNiP_MEMB& rData);
	BOOL Get_LocalForceData(T_ELEM_K ElemK, T_LCOM_K nOrgLcomNo, int nPos, STL_LCOM_FORCE_OD &rData);
	CString Get_LcomType(int nConcurrent);

	void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage) {};
	void Start() {};
	
protected:
//   void SetRptData(int ElemK, int nPosi,CSG_MEMB_POSD& InData, CSG_MEMB_RES_AASHTO_LRFD_D& RptData);
//   void SetRptData_SectStiffness(int nPosi,  DGN_SECT_STIFFNESS & InData, CSG_SectionProperty& RptData);
//   BOOL ShouldCheckPosiIJ(int nFlag, BOOL& bCheckCalcI, BOOL& bCheckCalcJ);
//   void ConvertCsgCalcUlmForce(T_CSGD_ULMF &For, CSG_ULMF_D &rData);
//   void ConvertCsgCalcChkForce(T_CSGD_COMF &For, CSG_FORCE_COMP &rData);
//   void ConvertCsgCalcForce( T_CSGD_FORCE &For, CSG_FORCE_D &rData);
//   CString GetLoadCombType(int iMaxType);
// 	
//   BOOL Get_CsgUlmSection(T_ELEM_K ElemI, T_ELEM_K ElemM, T_ELEM_K ElemJ, int nPosI, int nPosM, int nPosJ, CSG_ULMS_D &rData);
//   CString Get_StageName(T_STAG_K StagK);
// 	int GetMaxRatioCase(T_CSFC_BASE &CsfcB);
};

#include "HeaderPost.h"

#endif // !defined(AFX_RptOrthotropicDeck_RUS_H__)
