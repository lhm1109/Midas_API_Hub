// Dgn_PSCRptManager_AS5100.h: interface for the CDgn_PSCRptManager_AS5100 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Dgn_PSCRptManager_AS5100_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_)
#define AFX_Dgn_PSCRptManager_AS5100_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DBDoc.h"

#include "Dgn_PSCRptManager.h"
#include "DgnPscCommon.h"
//#include "..\dgnengine\src\DgnBase\DgnBaseShape_Struct.h"
//#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_MPhi_Struct.h"
//#include "..\dgnengine\src\DgnReportBase\DgnReportBase_MPhi_Struct.h"
//#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_PSC_XLStruct.h"
#include "Dgn_PSCReportModule.h"
#include "Dgn_PSCManager.h"

typedef BOOL (PSC_Print_Result)(UINT /*iElemKey*/, UINT /*iIJ*/, CStringW& /*LcomName*/, void* /*pFysgAS5100Base*/, size_t /*sizeFysgAS5100base*/, BOOL & /*bVersionOK*/ );
typedef BOOL (PSC_Print_Result2)(int /*iDgnCode*/, UINT /*iElemKey*/, UINT /*iIJ*/, bool /*bFinal*/, CStringW& /*LcomName*/, void* /*pFysgAS5100Base*/, size_t /*sizeFysgAS5100base*/, BOOL & /*bVersionOK*/ );
typedef BOOL (PSC_Print_Result_Comp)(int /*iDgnCode*/, UINT /*iElemKey*/, UINT /*iIJ*/, bool /*bFinal*/, CStringW& /*LcomName*/, void* /*pFysgAS5100Base*/, size_t /*sizeFysgAS5100base*/, BOOL & /*bVersionOK*/, BOOL /* bGirder*/ );
typedef BOOL (PSC_Print_Result_Tendon)(int /*iDgnCode*/, UINT /*iTndn*/, bool /*bFinal*/, CStringW& /*TendonName*/, void* /*pFpmtAS5100*/, size_t /*sizeFypcAS5100*/, BOOL & /*bVersionOK*/);
typedef BOOL (PSC_Print_FlexureDesign)(UINT /*iElemKey*/, UINT /*iIJ*/, BOOL /*bCompatibility*/, CStringW& , void* /*pBdcyAS5100Base*/, size_t /*sizeBdcyAS5100Base*/, BOOL & /*bVersionOK*/);
typedef BOOL (PSC_Print_AS5100_Detail)(int /*iDgnCode*/, UINT, UINT, void* , size_t, void* , size_t, BOOL&);
typedef BOOL (PSC_Print_AS5100_Detail_Dgn)(int /*iDgnCode*/, UINT, UINT, BOOL, CStringW&, void* , size_t, void* , size_t, BOOL&);
typedef BOOL (PSC_Print_AS5100_Detail_Alw)(int /*iDgnCode*/, UINT, UINT, BOOL, CStringW,void* , size_t, void* , size_t, BOOL&, BOOL);

struct _TRAN_PSC_BASE;
struct _FYSG_PSC_LRFD_BASE;
struct _FYPC_AASHTO;
struct _FYCM_PSC_LRFD_BASE;
struct _FPMT_PSC_LRFD_BASE;
struct _BDCW_PSC_LRFD_BASE;
struct _TBAR_PSC_LRFD_BASE;
struct _BDCY_PSC_LRFD_BASE;
struct _CUMS_AASHTO_BASE;
struct _CRMS_AASHTO_BASE;

class CDgn_PSCRptManager_AS5100 : public CDgn_PSCRptManager       
{
public:
	CDgn_PSCRptManager_AS5100();
	virtual ~CDgn_PSCRptManager_AS5100();

protected:
	PSC_Print_Result2*            m_pAlwStressCS;
	PSC_Print_Result_Comp*        m_pAlwStressCS_Comp;

	PSC_Print_Result2*            m_pAlwStressServComb;
	PSC_Print_Result_Comp*        m_pAlwStressServComb_Comp;

	PSC_Print_Result2*            m_pAlwPrincipalStressCS;
	PSC_Print_Result2*            m_pAlwPrincipalStressShear;
	PSC_Print_Result2*            m_pAlwPrincipalStressTorsion;
	PSC_Print_Result_Tendon*      m_pTendon;  
	PSC_Print_Result*             m_pCrack;
	PSC_Print_FlexureDesign*      m_pFlexure;
	PSC_Print_Result*             m_pShear;
	PSC_Print_Result*             m_pTorsion;

	PSC_Print_AS5100_Detail*      m_pDgnCondition; 
    PSC_Print_AS5100_Detail_Dgn*  m_pDgnTransferStress;
	PSC_Print_AS5100_Detail_Dgn*  m_pDgnFlexure; 
	PSC_Print_AS5100_Detail_Dgn*  m_pDgnCrack; 
	PSC_Print_AS5100_Detail_Dgn*  m_pDgnShear; 
	PSC_Print_AS5100_Detail_Dgn*  m_pDgnTorsion; 
	PSC_Print_AS5100_Detail_Alw*  m_pDgnFysg;
	PSC_Print_AS5100_Detail_Alw*  m_pDgnFycm;

public:
	BOOL InitialData(int iNationType);
	BOOL Initialize(int iDgnCode, int nAmd, int iRptUnit, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName);
	BOOL Terminate(int iDgnCode, BOOL bIsAborted, int nType);
	
	// RESULT-TABLE
	BOOL Print_AllowableStresses_Conc_ConstructionStage(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYSG_PSC_AS_BASE & fysgBase);
	BOOL Print_AllowableStresses_Conc_ConstructionStage_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYSG_PSC_AS_BASE & fysgBase, BOOL bGirder);
	BOOL Print_AllowableStresses_Conc_ServiceLoadCombination(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYCM_PSC_AS_BASE & fycmBase);
	BOOL Print_AllowableStresses_Conc_ServiceLoadCombination_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYCM_PSC_AS_BASE & fycmBase, BOOL bGirder);
	BOOL Print_AllowablePrincipalStress_Conc_ConstructionStage(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FPMT_JTG_BASE & fpmtBase);
	BOOL Print_AllowablePrincipalStress_Conc_ServiceLoadCombination(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FPMS_PSC_BASE & fpmtBase);
	//BOOL Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxTorsion(int iDgnCode, UINT iElemKey, UINT iIJ, const T_FPMS_JTG_BASE & fpmtBase);
	BOOL Print_AllowableStress_Tendon(int iDgnCode, UINT iTndn, bool bFinal, const T_FYPC_PSC_AS & fypc);
	//BOOL Print_TensionRebarRequired(UINT iElemKey, UINT iIJ, const T_TBAR_AASHTO_BASE & tbarBase);

	// DETAIL-REPORT
	BOOL Print_Detail_Condition    (int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, _BDCY_PSC_LRFD_BASE& EngBdcyD);
    BOOL Print_Detail_TransferStress(int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _TRAN_PSC_BASE & TranB);
	BOOL Print_Detail_FlexureDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _BDCY_PSC_LRFD_BASE& EngBdcyD);
	BOOL Print_Detail_CrackCheck   (int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _BDCW_PSC_LRFD_BASE & bdcwBase);
	BOOL Print_Detail_ShearDesign  (int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _CUMS_AASHTO_BASE & bdcwBase);
	BOOL Print_Detail_TorsionDesign(int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _STCM_AASHTO_BASE & STcmBase);
	BOOL Print_Detail_Allowable_Comp_Fysg(int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _FYSG_PSC_LRFD_BASE & dgnFysgBase, BOOL bGirder);
	BOOL Print_Detail_Allowable_Comp_Fycm(int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _FYCM_PSC_LRFD_BASE & dgnFycmBase, BOOL bGirder);

	// SUMMART-REPROT
	BOOL Print_CrackCheck(ElemPairK iElemKey, UINT iIJ, const T_BDCW_PSC_AS_BASE & bdcwBase);
	BOOL Print_FlexureDesign(ElemPairK iElemKey, UINT iIJ, BOOL bCompatibility, const T_BDCR_PSC_AS_BASE & bdcwBase);
	BOOL Print_ShearDesign(ElemPairK iElemKey, UINT iIJ, const T_SHRR_PSC_AS_BASE & bdcwBase);
	//BOOL Print_TorsionDesign(UINT iElemKey, UINT iIJ, const T_TORR_PSC_AS_BASE & crmtBase);

private:
	CAASHTOPSCRptDllModule m_AASHTOReportModule;
	CDgn_PSCManager m_pscManager;
	
	BOOL m_bInitialized;

// 	CString Get_StageName(T_STAG_K StagK);
// 	CString Get_LcomName(T_LCOM_K LcomK);
};
#endif // !defined(AFX_Dgn_PSCRptManager_AS5100_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_)