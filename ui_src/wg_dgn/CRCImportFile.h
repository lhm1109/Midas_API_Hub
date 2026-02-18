// CRCImportFile.h: interface for the CCRCImportFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRCIMPORTFILE_H__63BBEE1A_7C0A_4F50_A6B8_E0EA392224E9__INCLUDED_)
#define AFX_CRCIMPORTFILE_H__63BBEE1A_7C0A_4F50_A6B8_E0EA392224E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRCFileBase.h"
#include "CRCDataCtrl.h"
#include "CRCForceCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

class CPCDesign;

class CCRCImportFile : public CCRCFileBase   
{
public:
	CCRCImportFile();
	virtual ~CCRCImportFile();

	int m_iTotLineNum;
	ArrElemPairKey m_FysgKList;
	CArray<UINT,UINT> m_FypcKList;
	ArrElemPairKey m_FycmKList;
	ArrElemPairKey m_BdcwKList;
	ArrElemPairKey m_TbarKList;
	ArrElemPairKey m_BdcrKList;
	ArrElemPairKey m_FssgKList;
	ArrElemPairKey m_FsmsKList;
	ArrElemPairKey m_CwmsKList;
	ArrElemPairKey m_CumsKList;
	ArrElemPairKey m_CrmsKList;
	ArrElemPairKey m_FsmtKList;
	ArrElemPairKey m_CwmtKList;
	ArrElemPairKey m_CumtKList;
	ArrElemPairKey m_CrmtKList;
	CArray<UINT,UINT> m_FassCmpwKList;
	CArray<UINT,UINT> m_FmssCmpwKList;
	CArray<UINT,UINT> m_FprtCmpwKList;
	CArray<UINT,UINT> m_FwhlCmpwKList;

	CArray<UINT,UINT> m_FypcKsceKList;
	ArrElemPairKey m_BdcyKsceKList;
	ArrElemPairKey m_BdczKsceKList;
	ArrElemPairKey m_CumsKsceKList;
	ArrElemPairKey m_CrmtKsceKList;
	CArray<UINT,UINT> m_TndnKsceKList;

	CArray<UINT,UINT> m_FypcJtgKList;
	ArrElemPairKey m_FycmJtgKList;
	ArrElemPairKey m_FpmsJtgKList;
	ArrElemPairKey m_FpmtJtgKList;
	ArrElemPairKey m_TbarJtgKList;
	CArray<UINT,UINT> m_TndnJtgKList;
	ArrElemPairKey m_BdcyJtgKList;
	ArrElemPairKey m_CumsJtgKList;
	ArrElemPairKey m_CrmtJtgKList;
	ArrElemPairKey m_CscsJtgKList;

	CMap<UINT,UINT,UINT,UINT> m_DgnTdgrMap; //DgnTdgrK, TdgrK
	CMap<UINT,UINT,UINT,UINT> m_DgnTdnaMap; //DgnTdgrK, TdnaK
	CArray<_CRC_FYPC_K,_CRC_FYPC_K&> m_FypcBaseKList; 

	CMap<UINT, UINT, UINT, UINT> m_DgnLcomKMap;
	CMap<_CRC_COMF_K, _CRC_COMF_K, _CRC_COMF_D, _CRC_COMF_D&> m_ComfDMap;
    CMapEx<int, int, _CRC_COMF_K, _CRC_COMF_K&> m_mComfKey;
	
	BOOL Export_File();
	BOOL Export_File(CCRCDataCtrl& DataCtrl);

protected:
	CCRCDataCtrl m_DataCtrl;
	CCRCForceCtrl m_ForceCtrl;
	ArrElemPairKey m_ElemKList;
	int m_iDgnCode;
	int m_iBrgDir;

	CDBDoc* m_pDoc;
	CPCDesign* m_pPSCDesign;
	CStdioFile m_rptfile; //add for JTG rpt. by maxiao 2009-12.01

	BOOL MakeData();
	BOOL MakeData_JSCE02();
	BOOL Write_ImportData();

	BOOL Write_Fysg(); // 가설중 휨 응력도
	BOOL Write_Fypc(); // PC강재 최대응력도
	BOOL Write_Fycm(); // 합성응력도
	BOOL Write_Bdcw(); // 휨 균열폭
	BOOL Write_Tbar(); // 인장철근
	BOOL Write_Bdcr(); // 휨파괴안전도
	BOOL Write_Fssg(); // 가설중 경사인장 응력도
	BOOL Write_Fsms(); // 설계하중시 경사인장응력도(전단력 최대시)
	BOOL Write_Cwms(); // 설계하중시 전단균열(전단력 최대시)
	BOOL Write_Cums(); // 종국하중시 웹 또는 플랜지 압괴내력에 대한 검토(전단력 최대시)
	BOOL Write_Crms(); // 종국하중시 경사 파괴 내력에 대한 검토(전단력 최대시)
	BOOL Write_Fsmt(); // 설계하중시 경사 인장 응력도(비틀림 최대시)
	BOOL Write_Cwmt(); // 설계하중시 전단균열(비틀림 최대시)
	BOOL Write_Cumt(); // 종국하중시 웹또는 플랜지 압괴내력에 대한 검토 (비틀림 최대시)
	BOOL Write_Crmt(); // 종국하중시 경사파괴내력에 대한 검토(비틀림 최대시)
	BOOL Write_Fylc(); // 합성응력도(하중조합)
	BOOL Write_FassCmpw(); // 설계하중시 평균전단응력도 (전단력 최대시)
	BOOL Write_FmssCmpw(); // 종국하중시 최대전단응력도 (전단력 최대시)
	BOOL Write_FprtCmpw(); // 파형강판 국부좌굴 검토
	BOOL Write_FwhlCmpw(); // 파형강판 전체좌굴 검토

	BOOL Write_Fyst(); // 스테이지별 가설중 휨 응력도
	BOOL Write_Fsst(); // 스테이지별 가설중 경사 인장 응력도
	BOOL Write_Fssm(); // 스테이지별 가설중 경사 인장 응력도 중간값
	BOOL Write_Ftms(); // 하중조합별 설계하중시 경사 인장 응력도 계산을 위한 중간값 (전단력최대시)
	BOOL Write_Ftmt(); // 하중조합별 설계하중시 경사 인장 응력도 계산을 위한 중간값 (비틀림최대시)
	BOOL Write_Lcba(); // 설계하중시 최대/최소에 대한 하중조합번호
	BOOL Write_Lcbu(); // 종국시 최대/최소에 대한 하중조합번호

	BOOL Write_FypcKsce();	// PC강재 최대응력도
	BOOL Write_FssgKsce();
	BOOL Write_BdcyKsce();	// 계수하중시 휨강도에 대한 검토(강축)
	BOOL Write_BdczKsce();	// 계수하중시 휨강도에 대한 검토(약축)
	BOOL Write_CumsKsce();	// 계수하중시 전단강도에 대한 검토
	BOOL Write_CrmtKsce();	// 계수하중시 비틀림강도에 대한 검토
	BOOL Write_TndnKsce();  // 필요인장텐던

	BOOL Write_FypcJtg();	// 2. 정상사용극한상태 PC강재 최대응력도 (텐던별)
	BOOL Write_FypcJtg_Btype();	
	BOOL Write_FycmJtg();
	BOOL Write_FpmsJtg(); // 4. 정상사용극한상태 주응력도 (전단력 최대시)
	BOOL Write_FpmtJtg(); // 5. 정상사용극한상태 주응력도 (비틀림 최대시)
	BOOL Write_BdcwJtg();
	BOOL Write_TbarJtg(); // 7. 정상사용극한상태 인장철근
	BOOL Write_TndnJtg(); // 8. 정상사용극한상태 인장텐던
	BOOL Write_BdcyJtg();	// 9. 재하능력극한상태 휨강도 (강축)
	BOOL Write_CumsJtg();	// 10. 재하능력극한상태 전단강도
	BOOL Write_CrmtJtg();	// 11. 재하능력극한상태 비틀림강도 
	BOOL Write_CscsJtg(); // 12.compressive stress check at service stage.예箕駱炬 add by maxiao 2008.01.11
	
	// For AASHTO-LRFD08
	BOOL Write_Fysg_AASHTO(); // Construction stage(시공단계응력)
	BOOL Write_Fypc_AASHTO(); // Tendon Stress(텐던응력)
	BOOL Write_Fycm_AASHTO(); // After.. cross section stress(합성응력)
	BOOL Write_Fssg_AASHTO(); // Principal stress by construction stage
	BOOL Write_Fpms_AASHTO(); // Principal stress, shear.
	BOOL Write_Fpmt_AASHTO(); // Principal stress, Torsion.
	BOOL Write_Bdcw_AASHTO(); // Crack width(휨균열폭)
	BOOL Write_Tbar_AASHTO(); // Tension Bar(인장철근)
	BOOL Write_Bdcy_AASHTO(); // Flexural strength(휨강도) Y-axis
	BOOL Write_Bdcz_AASHTO(); // Flexural strength(휨강도) Z-axis
	BOOL Write_Cums_AASHTO(); // Shear strength
	BOOL Write_Crmt_AASHTO(); // Torsion design.
	BOOL Write_Bdcw_CSA(); // Crack width(휨균열폭)
    BOOL Write_PSCDesign();
	 //
	// For Eurocode2-2:05,
	BOOL Write_Fysg_PSC(); // Construction stage(시공단계응력)
	BOOL Write_Fypc_PSC(); // Tendon Stress(텐던응력)
	BOOL Write_Fycm_PSC(); // After.. cross section stress(합성응력)
	BOOL Write_Fpsg_PSC(); // Principal stress by construction stage
	BOOL Write_Fpms_PSC(); // Principal stress, shear.  
	BOOL Write_Bdcw_PSC(); // Crack width(휨균열폭)
	BOOL Write_Bdcr_PSC(); // Moment resistance(휨강도) Y-axis
	BOOL Write_Cums_PSC(); // Shear resistance
	BOOL Write_Crmt_PSC(); // Torsion resistance.

	// For  KSCE-LSD15
	BOOL Write_CHECK_PSC();
	//BOOL Write_CSSR_PSC_LSD15();
	//BOOL Write_ULSR_PSC_LSD15();
	//BOOL Write_SLSR_PSC_LSD15();
	BOOL Write_Fysg_PSC_LSD15(); // Construction stage(시공단계응력)
	BOOL Write_Fypc_PSC_LSD15(); // Tendon Stress(텐던응력)
	BOOL Write_Fycm_PSC_LSD15(); // After.. cross section stress(합성응력)
	BOOL Write_Fpsg_PSC_LSD15(); // Principal stress by construction stage
	BOOL Write_Fpms_PSC_LSD15(); // Principal stress, shear.  
	BOOL Write_Bdcw_PSC_LSD15(); // Crack width(휨균열폭)
	BOOL Write_Bdcr_PSC_LSD15(); // Moment resistance(휨강도) Y-axis
	BOOL Write_Cums_PSC_LSD15(); // Shear resistance
	BOOL Write_Crmt_PSC_LSD15(); // Torsion resistance.

	// For SNiP 2.05.03-84*, SP 35.13330.2011
	BOOL Write_FirstGroupLS();
	BOOL Write_SecondGroupLS();
	BOOL Write_Bdcw_SNiP(); // Crack width(휨균열폭)
	BOOL Write_Bdcr_SNiP(); // Moment resistance(휨강도) Y-axis
	BOOL Write_Cums_SNiP(); // Shear resistance
	//BOOL Write_Crmt_SNiP(); // Torsion resistance.
	BOOL Write_Fati_SNiP(); // Fatigue.

	BOOL Write_CHECK_PSC_AS();
    BOOL Write_CHECK_PSC_BS();

    BOOL Write_Fypc_BS(); // Tendon Stress(텐던응력)

	BOOL Write_Fypc_IRS(); // Tendon Stress(텐던응력)

	BOOL ConvertBDCW_PSCtoAASHTO(T_BDCW_PSC& rData, T_BDCW_AASHTO& rData_aashto);

	BOOL m_bSkipTendon;

	////////////////////////////////////////////////////////////////////////////////////////////////
	HINSTANCE m_hDllCalc;

private:
    BOOL Write_ImportData_JSCE();
    BOOL Write_ImportData_JTG();
    BOOL Write_ImportData_KSCE();
    BOOL Write_ImportData_AASHTO();
    BOOL Write_ImportData_CSA();
    BOOL Write_ImportData_EURO();
    BOOL Write_ImportData_IRC();
    BOOL Write_ImportData_KSCELSD();
    BOOL Write_ImportData_RUS();
    BOOL Write_ImportData_AS();
    BOOL Write_ImportData_BS();
    BOOL Write_ImportData_IRS();
	BOOL Write_ImportData_BRA();

};

#endif // !defined(AFX_CRCIMPORTFILE_H__63BBEE1A_7C0A_4F50_A6B8_E0EA392224E9__INCLUDED_)