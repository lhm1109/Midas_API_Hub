#include "stdafx.h"
#include "wg_dgn.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_dbLock\LockMgr.h"   // for CLockMgr
#include "..\wg_dbLock\WebLockUtil.h"
/*!@#$*/#include "..\MIT_frx\MBaseTabWnd.h"

// 탭 추가시 수정할 곳 : Bar Tree Dlg
#include "DgnGenDlg.h"
#include "DgnStlDlg.h"
#include "DgnConDlg.h"
#include "DgnSrcDlg.h"
#include "DgnCfsDlg.h"
#include "DgnPscDlg.h"
#include "DgnSodDlg.h"
#include "DgnCpgDlg.h"
#include "DgnRatingDlg.h"
#include "DgnSteelRatingDlg.h"
#include "DgnRCRatingDlg.h"
#include "DgnExportFunc.h"
#include "DgnAluDlg.h"
#include "..\wg_db\AluDgnTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// code guru : Dialog in DLL 참조


///////////////////////////////////////////////////////////////////////////////////
// Tree Menu related export functions

// 탭 추가시 수정할 곳

//!@#$extern __declspec(dllexport) void DGN_CreateBarTreeDlg(CWnd* pParent, SEC3DTabWnd* pTabWnd, int nWhich)
extern __declspec(dllexport) void DGN_CreateBarTreeDlg(CWnd* pParent, mit::frx::MBaseTabWnd* pTabWnd, int nWhich)
{
	if(!::IsWindow(pTabWnd->GetSafeHwnd()))
	{
#if defined(_TOWER)
		pTabWnd->Create(pParent,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1300);
		pTabWnd->AddTab(RUNTIME_CLASS(CDgnGenDlg),_T(""));
#elif defined(_CIVIL_JP)
		pTabWnd->Create(pParent,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1300);
 		pTabWnd->AddTab(RUNTIME_CLASS(CDgnGenDlg),_LS(IDS_DGN_TAB_GENERAL));
		pTabWnd->AddTab(RUNTIME_CLASS(CDgnStlDlg), _LS(IDS_DGN_TAB_STEEL));
#else
		pTabWnd->Create(pParent,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1300);
		pTabWnd->AddTab(RUNTIME_CLASS(CDgnGenDlg),_LS(IDS_DGN_TAB_GENERAL));
		pTabWnd->AddTab(RUNTIME_CLASS(CDgnStlDlg),_LS(IDS_DGN_TAB_STEEL));
	// Change by Jaeoh .('05.01.26) 
	#if defined(_CIVIL) 
		// Change by Jaeoh .('05.07.15) #if defined(_ORG) || defined(_CH) 삭제
		// Change by sshan .('06.03.02) #if defined(_ORG) 일 경우 Add 하지 않는다
		// Modify by sshan. MNET:2607.('20070103) (_ORG)일경우 추가하지 않으며 (_CH)일경우 _T("RC")로 추가한다.
		// Change by sshan .('08.01.02) #if defined(_ORG) 일 경우 Add함(Common->Concrete)
		#if defined(_CH)
			pTabWnd->AddTab(RUNTIME_CLASS(CDgnConDlg),_LS(IDS_DGN_TAB_RC));
		#else
			pTabWnd->AddTab(RUNTIME_CLASS(CDgnConDlg),_LS(IDS_DGN_TAB_CONCRETE));
		#endif
	#else
		pTabWnd->AddTab(RUNTIME_CLASS(CDgnConDlg),_LS(IDS_DGN_TAB_CONCRETE));
	#endif

	// Change by sshan .('06.03.02) #if defined(_ORG) 일 경우 Add 하지 않는다
	// Change by sshan .('08.01.02) #if defined(_ORG) 일 경우 Add함(Common->SRC)
	#if defined(_CIVIL) 
		pTabWnd->AddTab(RUNTIME_CLASS(CDgnSrcDlg),_LS(IDS_DGN_TAB_SRC));		
	#else
		pTabWnd->AddTab(RUNTIME_CLASS(CDgnSrcDlg),_LS(IDS_DGN_TAB_SRC));
	#endif

	#if defined(_MGEN)
		pTabWnd->AddTab(RUNTIME_CLASS(CDgnCfsDlg),_LS(IDS_DGN_TAB_CFSTEEL));
		#endif

		//pTabWnd->AddTab(RUNTIME_CLASS(CDgnSrcDlg),_LS(IDS_DGN_TAB_SRC)));
	#if defined(_CIVIL)
 		CStringArray aPscCodeName;
		CDBLib::GetPscCodeNameList(aPscCodeName);
		if(aPscCodeName.GetSize()>0)      
	    pTabWnd->AddTab(RUNTIME_CLASS(CDgnPscDlg),_LS(IDS_DGN_TAB_PSC));
		else {}    
	#endif

	//Cpg Tab 추가 sshan 061031
	#if defined(_CIVIL) 
		#if defined(_US) || defined(_RUS) || defined(_ORG) || defined(_JP)
		  pTabWnd->AddTab(RUNTIME_CLASS(CDgnCpgDlg),_LS(IDS_DGN_TAB_CPG));		
		#elif defined(_CH)	
			CLockMgr* pLockMgr = CLockMgr::GetInstance();
			int wCode = pLockMgr->GetLockCode();
			if(IsOnBitCode(wCode, AllowNationCode::_ECH_US)) // US	
			{
				pTabWnd->AddTab(RUNTIME_CLASS(CDgnCpgDlg),_LS(IDS_DGN_TAB_CPG));		
			}
		#endif
	#endif
		
	//Rating Tab 추가 sshan 080214
	// Modify, Jaeoh. [4/29/2009], US에서 AASHTO 일 때는 Tab이 필요없다 
	#if defined(_CIVIL) 
		#if defined(_RUS)
			// MQC 12150, RU Config Build, Design Menu Err Modified. 
			// 차후에 RU에 Rating 기능 지원 할 경우, CDgnRatingDlg 생성자에서 생성하는 Child Dlg Config도 수정 필요
			//pTabWnd->AddTab(RUNTIME_CLASS(CDgnRatingDlg),_LS(IDS_DGN_TAB_RATING)));
		#elif defined(_US) || defined(_JP)
			pTabWnd->AddTab(RUNTIME_CLASS(CDgnRatingDlg),_LS(IDS_DGN_TAB_PSC_RATING));		
			pTabWnd->AddTab(RUNTIME_CLASS(CDgnSteelRatingDlg),_LS(IDS_DGN_TAB_STEEL_RATING));	
		#elif defined(_ORG) 
			pTabWnd->AddTab(RUNTIME_CLASS(CDgnRatingDlg),_LS(IDS_DGN_TAB_PSC_RATING));		
			pTabWnd->AddTab(RUNTIME_CLASS(CDgnSteelRatingDlg),_LS(IDS_DGN_TAB_STEEL_RATING));		
			pTabWnd->AddTab(RUNTIME_CLASS(CDgnRCRatingDlg),_LS(IDS_DGN_TAB_RC_RATING));		
		#endif
	#endif			

	#if defined(_CIVIL)
			pTabWnd->AddTab(RUNTIME_CLASS(CDgnSodDlg),_LS(IDS_DGN_TAB_SOD));		
	#endif

	if(IAluDgnTool::IsUseAluDgn())
	{
			pTabWnd->AddTab(RUNTIME_CLASS(CDgnAluDlg),_LS(IDS_MAIN_RIBBON_MENU_ALU));
	}
#endif

	}
}

// Child 추가시 수정할 곳

//!@#$extern __declspec(dllexport) void DGN_ShowChildDlgByID(SEC3DTabWnd* pTabWnd, int nCurMode)
extern __declspec(dllexport) void DGN_ShowChildDlgByID(mit::frx::MBaseTabWnd* pTabWnd, int nCurMode, CString& curString)
{
#if defined(_TOWER)
	CWnd* pWnd;
	switch(nCurMode)
	{
		// General Tab
		case __TMMODE_DGN_TWR_MBTP__:
		{
			pTabWnd->ActivateTab(0);  // Only General 0
			pTabWnd->ScrollToTab(0);
			pTabWnd->GetActiveTab(pWnd);
			CDgnGenDlg* pTab = (CDgnGenDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);
		}break;
		break;
	}
#else
	CWnd* pWnd;
	switch(nCurMode)
	{
		//	General Tab
		case __TMMODE_DGN_GEN_MEMB__:
		case __TMMODE_DGN_GEN_MEMB_CHG__:
		case __TMMODE_DGN_GEN_LENG__:
		case __TMMODE_DGN_GEN_SPLN__:
		case __TMMODE_DGN_GEN_KFAC__:
		case __TMMODE_DGN_GEN_LTSR__:
		case __TMMODE_DGN_GEN_CMFT__:
		case __TMMODE_DGN_GEN_FMAG__:
		case __TMMODE_DGN_GEN_BCIF__:
		case __TMMODE_DGN_GEN_PMCR__:
		case __TMMODE_DGN_GEN_REDU__:
		case __TMMODE_DGN_GEN_SUEQ__:
		case __TMMODE_DGN_GEN_GREQ__:
		case __TMMODE_DGN_GEN_MBTP__:
		case __TMMODE_DGN_GEN_SEIS__:
		case __TMMODE_DGN_GEN_SEIS_DGN__:
		case __TMMODE_DGN_GEN_HCBM__:
		case __TMMODE_DGN_GEN_EWSD__:
		case __TMMODE_DGN_GEN_ULCT__:
		case __TMMODE_DGN_GEN_DSST__:
		case __TMMODE_DGN_GEN_ARST__:
		{
			pTabWnd->ActivateTab(0);  // General 0, Steel 1, RC : 2, SRC : 3, PSC : 4
																// General 0, Steel 1, PSC: 2   ===>>  (_ORG)
																// General 0, Steel 1, Concret 2, SRC 3, PSC 4 ((_ORG) Modify sshan 080122)
			pTabWnd->ScrollToTab(0);
			pTabWnd->GetActiveTab(pWnd);
			CDgnGenDlg* pTab = (CDgnGenDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);

			int nSel = pTab->m_ComboMenu.GetCurSel();
			CString strSelected;
			pTab->m_ComboMenu.GetLBText(nSel, strSelected);
			curString = strSelected;
		}break;
		//	Steel Tab
		case __TMMODE_DGN_STL_SERV__:// Coded by Seungjun MNet:No.2393 ('20060828)
		case __TMMODE_DGN_STL_CBFT__:
		case __TMMODE_DGN_STL_CVFT__:
		case __TMMODE_DGN_STL_DALW__:
		case __TMMODE_DGN_STL_UCFA__:
		case __TMMODE_DGN_STL_STFN__:
		case __TMMODE_DGN_STL_SLRS__:
		case __TMMODE_DGN_STL_PHIB__://add by maxiao 2015-10-12
		case __TMMODE_DGN_STL_SETY__:	//add by tss 2020/02/12
		case __TMMODE_DGN_STL_BMRO__:	//add by tss 2021/01/13
		case __TMMODE_DGN_STL_SPDF__:	//add by tss 2024/02/29
		case __TMMODE_DGN_STL_DSAS__:
		case __TMMODE_DGN_STL_DSAU__:
		case __TMMODE_DGN_STL_IMEL__:
		case __TMMODE_DGN_STL_INDF__:
        case __TMMODE_DGN_STL_SDSW__://add by maxiao 2015-10-20
		case __TMMODE_DGN_STL_PFMC__:
		case	__TMMODE_DGN_STL_PFCM__:
		case	__TMMODE_DGN_STL_PFDL__:
		case	__TMMODE_DGN_STL_PFMP__:
		case	__TMMODE_DGN_STL_PFNP__:
		case __TMMODE_DGN_STL_SMSP__:
		case __TMMODE_DGN_STL_JP_POSITION__:
		case __TMMODE_DGN_STL_STUS__:
		case __TMMODE_DGN_STL_STRE__:
		case __TMMODE_DGN_STL_STBD__:
		case __TMMODE_DGN_STL_SPSC__:
		case __TMMODE_DGN_STL_SPLB__:
		case __TMMODE_DGN_STL_SPCS__:
		case __TMMODE_DGN_STL_SPLS__:
		case __TMMODE_DGN_STL_SPPB__:
		case __TMMODE_DGN_STL_SP14SeisMembParams__:
		case __TMMODE_DGN_STL_SP16SafetyMembParams__:
		case __TMMODE_DGN_STL_SP16CombinedSectParams__:
		{
			pTabWnd->ActivateTab(1);
			pTabWnd->ScrollToTab(1);
			pTabWnd->GetActiveTab(pWnd);
			CDgnStlDlg* pTab = (CDgnStlDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);

			int nSel = pTab->m_ComboMenu.GetCurSel();
			CString strSelected;
			pTab->m_ComboMenu.GetLBText(nSel, strSelected);
			curString = strSelected;
		}break;
		//	Concrete Tab
		case __TMMODE_DGN_CON_REBB__:
		case __TMMODE_DGN_CON_REBC__:
		case __TMMODE_DGN_CON_REBR__:
		case __TMMODE_DGN_CON_REBW__:
		case __TMMODE_DGN_CON_WMAK__:
		case __TMMODE_DGN_CON_BEMW__:
		case __TMMODE_DGN_CON_EBMW__:
		case __TMMODE_DGN_CON_CVL_SUFC__:
		case __TMMODE_DGN_CON_CVL_RMFR__:
		case __TMMODE_DGN_CON_CVL_ULSP__:
		case __TMMODE_DGN_CON_CVL_RDGN_BEAM__:
		case __TMMODE_DGN_CON_CVL_RDGN_COLU__:
		case __TMMODE_DGN_CON_JSFT__://Add By RSH 2002.12.26
		case __TMMODE_DGN_CON_PSAS__://Add By RSH 2002.12.02
		case __TMMODE_DGN_CON_MRFT__://Add By RSH 2003.07.04
		case __TMMODE_DGN_CON_MCMB__://Add By RSH 2003.07.04
		case __TMMODE_DGN_CON_REIN_SHEAR__:
		case __TMMODE_DGN_CON_SERV__:// Coded by Seungjun MNet:No.2393 ('20060828)
		case __TMMODE_DGN_CON_SPVAREMA__:// Coded by Seungjun MNet:No.2393 ('20060828)
		case __TMMODE_DGN_CON_SERV_SLAB__:
		case __TMMODE_DGN_GEN_MEMB_PLATE__:
		case __TMMODE_DGN_CON_ELEM_BAR__:// Coded by sshan MNet:No.2484 ('20061109)
		case __TMMODE_DGN_CON_ELEM_BAR__SUB_BEAM:
		case __TMMODE_DGN_CON_ELEM_BAR__SUB_COL :
		case __TMMODE_DGN_CON_ELEM_BAR__SUB_BRAC:
		case __TMMODE_DGN_CON_ELEM_BAR__SUB_WALL:
		case __TMMODE_DGN_CON_BEAM_BAR__:
		case __TMMODE_DGN_CON_DORC__:// Coded by sshan MNet:No.2607 ('20061220)
		case __TMMODE_DGN_CON_PORC__:// Coded by sshan MNet:No.2607 ('20061220)
		case __TMMODE_DGN_CON_CWRC__:// Coded by sshan MNet:No.2607 ('20061220)
		case __TMMODE_DGN_CON_LEKF__:// Coded by sshan MNet:No.2607 ('20061226)
		case __TMMODE_DGN_CON_TRFT__:// Coded by sshan MNet:No.3780 ('20090320)		
		case __TMMODE_DGN_CON_UCCF__:// Add by GAY. PMS:4267. ('11.12.27). 중국신기준-불확정성 분항계수.
		case __TMMODE_DGN_CON_RSDM__:// Coded by bjshin MNet:No.3992 ('20090517)
		case __TMMODE_DGN_CON_REXC__:
		case __TMMODE_DGN_CON_DFBA__:
		case __TMMODE_DGN_CON_PMDM__:
		case __TMMODE_DGN_CON_SCOL__:
		case __TMMODE_DGN_CON_OCDF__:
		case __TMMODE_DGN_CON_PBDC__:
		case __TMMODE_DGN_CON_PCDC__:
		case __TMMODE_DGN_CON_PBDD__:
		case __TMMODE_DGN_CON_PCDD__:
		case __TMMODE_DGN_CON_STAN__:
		case __TMMODE_DGN_CON_MNDC__:
		case __TMMODE_DGN_CON_SAFM__:
		case __TMMODE_DGN_CON_RRTR__:
		case __TMMODE_DGN_CON_EXPOSURE_CLASS__:
		{
#if defined(_CIVIL) 
			pTabWnd->ActivateTab(2);
			pTabWnd->ScrollToTab(2);
			pTabWnd->GetActiveTab(pWnd);
			CDgnConDlg* pTab = (CDgnConDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);

			int nSel = pTab->m_ComboMenu.GetCurSel();
			CString strSelected;
			pTab->m_ComboMenu.GetLBText(nSel, strSelected);
			curString = strSelected;
#else
			pTabWnd->ActivateTab(2);
			pTabWnd->ScrollToTab(2);
			pTabWnd->GetActiveTab(pWnd);
			CDgnConDlg* pTab = (CDgnConDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);
#endif
		}break;
		// SRC Tab
		case __TMMODE_DGN_SRC_RBSB__:
		case __TMMODE_DGN_SRC_REBS__:
		{
#if defined(_CIVIL)
			pTabWnd->ActivateTab(3);
			pTabWnd->ScrollToTab(3);
			pTabWnd->GetActiveTab(pWnd);
			CDgnSrcDlg* pTab = (CDgnSrcDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);

			int nSel = pTab->m_ComboMenu.GetCurSel();
			CString strSelected;
			pTab->m_ComboMenu.GetLBText(nSel, strSelected);
			curString = strSelected;
#else
			pTabWnd->ActivateTab(3);
			pTabWnd->ScrollToTab(3);
			pTabWnd->GetActiveTab(pWnd);
			CDgnSrcDlg* pTab = (CDgnSrcDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);
#endif
		}break;
		//	Cold Formed Steel Tab
		case __TMMODE_DGN_CFS_SERV__:
		case __TMMODE_DGN_CFS_CBFT__:
			{
				pTabWnd->ActivateTab(4);
				pTabWnd->ScrollToTab(4);
				pTabWnd->GetActiveTab(pWnd);
				CDgnCfsDlg* pTab = (CDgnCfsDlg*)pWnd; // class name
				pTab->ShowChildDlgByID(nCurMode);

				int nSel = pTab->m_ComboMenu.GetCurSel();
				CString strSelected;
				pTab->m_ComboMenu.GetLBText(nSel, strSelected);
				curString = strSelected;
			}break;
		case __TMMODE_DGN_CFS_CSMS__:	//Add by tss(2017.8.16)
		case __TMMODE_DGN_CFS_CSOS__:	//Add by tss(2017.8.16)
		case __TMMODE_DGN_CFS_SPHI__:	//Add by tss 2022/10/12
		case __TMMODE_DGN_CFS_PLIN__:
		case __TMMODE_DGN_CFS_PFMC__:
		case __TMMODE_DGN_CFS_PFCM__:
		case __TMMODE_DGN_CFS_PFDL__:
		case __TMMODE_DGN_CFS_PFMP__:
		case __TMMODE_DGN_CFS_PFNP__:
		{
			pTabWnd->ActivateTab(4);
			pTabWnd->ScrollToTab(4);
			pTabWnd->GetActiveTab(pWnd);
			CDgnCfsDlg* pTab = (CDgnCfsDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);

			int nSel = pTab->m_ComboMenu.GetCurSel();
			CString strSelected;
			pTab->m_ComboMenu.GetLBText(nSel, strSelected);
			curString = strSelected;
		}break;
	
		// PSC Tab
		case __TMMODE_DGN_CRC_DGN_POSC__:
		case __TMMODE_DGN_CRC_DGN_POSX__:
		case __TMMODE_DGN_PSC_CRACK_PSCW__:
		case __TMMODE_DGN_PSC_SEGMENT__:
		case __TMMODE_DGN_PSC_EXPOSURE_CLASS__:
		case __TMMODE_DGN_PSC_OCDF__:
		case __TMMODE_DGN_PSC_SHEAR_CONNECTOR__:
		case __TMMODE_DGN_PSC_INTERFACE_SHEAR__:
		case __TMMODE_DGN_PSC_FATIGUE_CHECK__:
		{
#if defined(_CIVIL)
	#if defined(_ORG)
			pTabWnd->ActivateTab(4);
			pTabWnd->ScrollToTab(4);
	#else
			pTabWnd->ActivateTab(4);
			pTabWnd->ScrollToTab(4);
	#endif
#else
			pTabWnd->ActivateTab(4);
			pTabWnd->ScrollToTab(4);
#endif
			pTabWnd->GetActiveTab(pWnd);
			CDgnPscDlg* pTab = (CDgnPscDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);

			int nSel = pTab->m_ComboMenu.GetCurSel();
			CString strSelected;
			pTab->m_ComboMenu.GetLBText(nSel, strSelected);
			curString = strSelected;
		}break;
		// CPG Tab
		case __TMMODE_DGN_CPG_STIF__:
		case __TMMODE_DGN_CPG_SHEAR__:
		case __TMMODE_DGN_CPG_POSITION__:		
		case __TMMODE_DGN_CPG_POSI_DGN_OUT__:		
		case __TMMODE_DGN_CPG_LOAD_APPLICATION__:		
		case __TMMODE_DGN_CPG_TORSION__:		
		case __TMMODE_DGN_CPG_DAMAGE__:		
		case __TMMODE_DGN_CPG_SHEAR_CONN__:
		case __TMMODE_DGN_CPG_SHEAR_CONN_CSA__:
		case __TMMODE_DGN_CPG_FATIGUE_PARAM__:
			//case __TMMODE_DGN_CPG_UNBRA_LENG__:		
		case __TMMODE_DGN_CPG_CURVE_BRID__:		
		case __TMMODE_DGN_CPG_DECK_OVERHANG__:
		case __TMMODE_DGN_CPG_SHEAR_CONN_GROUP__:
		case __TMMODE_DGN_CPG_SERV__:
		case __TMMODE_DGN_CPG_LENG__:
		case __TMMODE_DGN_CPG_WEB_PANEL__:
		case __TMMODE_DGN_CPG_RESIDUAL_STRESS__:
		{
#if defined(_CIVIL)
//   #if defined(_ORG)
// 			pTabWnd->ActivateTab(3);
// 			pTabWnd->ScrollToTab(3);
	#if defined(_US) || defined(_RUS) || defined(_ORG) || defined(_CH) || defined(_JP)
			pTabWnd->ActivateTab(5);
			pTabWnd->ScrollToTab(5);
	#endif
#else
			pTabWnd->ActivateTab(4);
			pTabWnd->ScrollToTab(4);
#endif
			pTabWnd->GetActiveTab(pWnd);
			CDgnCpgDlg* pTab = (CDgnCpgDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);

			int nSel = pTab->m_ComboMenu.GetCurSel();
			CString strSelected;
			pTab->m_ComboMenu.GetLBText(nSel, strSelected);
			curString = strSelected;
		}break;
		//case __TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__: // AASHTO Steel Composite Rating 
		case __TMMODE_DGN_STEEL_RATING_CURVE_BRID__:
		case __TMMODE_DGN_STEEL_RATING_DECK_OVERHANG__:
			{
#if defined(_CIVIL)
				//   #if defined(_ORG)
				// 			pTabWnd->ActivateTab(3);
				// 			pTabWnd->ScrollToTab(3);
#if defined(_US) || defined(_RUS) || defined(_ORG) || defined(_CH) || defined(_JP)
				pTabWnd->ActivateTab(5);
				pTabWnd->ScrollToTab(5);
#endif
#else
				pTabWnd->ActivateTab(4);
				pTabWnd->ScrollToTab(4);
#endif
				pTabWnd->GetActiveTab(pWnd);
				CDgnCpgDlg* pTab = (CDgnCpgDlg*)pWnd; // class name
				pTab->ShowChildDlgByID(nCurMode, 1);

				int nSel = pTab->m_ComboMenu.GetCurSel();
				CString strSelected;
				pTab->m_ComboMenu.GetLBText(nSel, strSelected);
				curString = strSelected;
			}break;
		case __TMMODE_DGN_SOD_FLEXBUCK_INPUT__:
			{
				int nCount = pTabWnd->GetTabCount();
				pTabWnd->ActivateTab(nCount-1);
				pTabWnd->ScrollToTab(nCount-1);

				pTabWnd->GetActiveTab(pWnd);
				CDgnSodDlg* pTab = (CDgnSodDlg*)pWnd; // class name
				pTab->ShowChildDlgByID(nCurMode);

				int nSel = pTab->m_ComboMenu.GetCurSel();
				CString strSelected;
				pTab->m_ComboMenu.GetLBText(nSel, strSelected);
				curString = strSelected;
			}
			break;
		case __TMMODE_DGN_SOD_POSI_DGN_OUT__:
		{
			int nCount = pTabWnd->GetTabCount();
			pTabWnd->ActivateTab(nCount-1);
			pTabWnd->ScrollToTab(nCount-1);

			pTabWnd->GetActiveTab(pWnd);
			CDgnSodDlg* pTab = (CDgnSodDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);

			int nSel = pTab->m_ComboMenu.GetCurSel();
			CString strSelected;
			pTab->m_ComboMenu.GetLBText(nSel, strSelected);
			curString = strSelected;
		}
		break;
		//Rating Tab
		case __TMMODE_DGN_RATING_OPTION__:
		case __TMMODE_DGN_RATING_PRINT__:
		case __TMMODE_DGN_RATING_PSC_ENVIRONMENT_CLASS__:
		case __TMMODE_DGN_PSC_ASSESSMENT_OPTION__:
		{
#if defined(_CIVIL)
	#if defined(_US) || defined(_RUS) || defined(_ORG) || defined(_JP)
			pTabWnd->ActivateTab(6);
			pTabWnd->ScrollToTab(6);
/*
	#else
			pTabWnd->ActivateTab(5); 
			pTabWnd->ScrollToTab(5);
*/
	#endif
#else
			pTabWnd->ActivateTab(4);
			pTabWnd->ScrollToTab(4);
#endif
			pTabWnd->GetActiveTab(pWnd);
			CDgnRatingDlg* pTab = (CDgnRatingDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);

			int nSel = pTab->m_ComboMenu.GetCurSel();
			CString strSelected;
			pTab->m_ComboMenu.GetLBText(nSel, strSelected);
			curString = strSelected;
		}break;
#if defined(_CIVIL)
		// Steel Rating Tab 
		case __TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__: // AASHTO Steel Composite Rating 
		case __TMMODE_DGN_STEEL_RATING_OPTION__:
		case __TMMODE_DGN_STEEL_RATING_PRINT__:
		case __TMMODE_DGN_STEEL_RATING_WEB_PANEL__:
		case __TMMODE_DGN_STEEL_RATING_TORSION__:
		case __TMMODE_DGN_STEEL_RATING_SHEAR_CONN__:
		case __TMMODE_DGN_STEEL_RATING_SERVICE_PARAM__:
		case __TMMODE_DGN_STEEL_RATING_ASSESS_CHECK__:
		case __TMMODE_DGN_STEEL_RATING_ASSESS_REPORT__:
		case __TMMODE_DGN_STEEL_RATING_EFFECT_LENGTH_U_FRAME__:
		case __TMMODE_DGN_STEEL_RATING_MOMENT_DIST_FACT_SLENDER__:
		case __TMMODE_DGN_STEEL_RATING_FASTENER_RIVET__:
		case __TMMODE_DGN_STEEL_RATING_ASSIGN_MEMB_FACTOR__:
		{
			pTabWnd->ActivateTab(7);
			pTabWnd->ScrollToTab(7);
			pTabWnd->GetActiveTab(pWnd);
			CDgnSteelRatingDlg* pTab = (CDgnSteelRatingDlg*)pWnd; // class name
			pTab->ShowChildDlgByID(nCurMode);

			int nSel = pTab->m_ComboMenu.GetCurSel();
			CString strSelected;
			pTab->m_ComboMenu.GetLBText(nSel, strSelected);
			curString = strSelected;
		}break;
		case __TMMODE_DGN_RC_RATING_OPTION_BEAM__:
		case __TMMODE_DGN_RC_RATING_OPTION_PLATE__:
		case __TMMODE_DGN_RC_RATING_DGN_OPTION_PLATE__:
		case __TMMODE_DGN_RC_RATING_PRINT_BEAM__:
		case __TMMODE_DGN_RC_RATING_PRINT_PLATE__:
			{
				pTabWnd->ActivateTab(8);
				pTabWnd->ScrollToTab(8);
				pTabWnd->GetActiveTab(pWnd);
				CDgnRCRatingDlg* pTab = (CDgnRCRatingDlg*)pWnd; // class name
				pTab->ShowChildDlgByID(nCurMode);

				int nSel = pTab->m_ComboMenu.GetCurSel();
				CString strSelected;
				pTab->m_ComboMenu.GetLBText(nSel, strSelected);
				curString = strSelected;
			}break;
#endif
		case __TMMODE_DGN_ALU_ARIB__:
		case __TMMODE_DGN_ALU_AWED__:
		case __TMMODE_DGN_ALU_AHTR__:
		case __TMMODE_DGN_ALU_APHI__:
			{
				if(IAluDgnTool::IsUseAluDgn())
				{
					pTabWnd->ActivateTab(5);
					pTabWnd->ScrollToTab(5);
					pTabWnd->GetActiveTab(pWnd);
					CDgnAluDlg* pTab = (CDgnAluDlg*)pWnd; // class name
					pTab->ShowChildDlgByID(nCurMode);

					int nSel = pTab->m_ComboMenu.GetCurSel();
					CString strSelected;
					pTab->m_ComboMenu.GetLBText(nSel, strSelected);
					curString = strSelected;
				}
			}break;
	}
#endif
}





















