#include "StdAfx.h"
#include "wg_treemenu.h"
#include "TreeWorkCtrlNew.h"

#include "..\wg_main\wg_mainres2.h"
#include "..\wg_db\DispOptMgr.h"
#include "..\wg_db\DispOptColorObj.h"
#include "..\wg_base\XMultiLang.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\UndoRedo.h"
#include "..\wg_dbLock\LockOption.h"

#include "..\wg_cmd\wg_cmd_Dialog.h"
#include "..\wg_cmd\CMMvhlItemStdEuroBSDlg.h"
#include "..\wg_cmd\CMMvldItemTrDlg.h"
#include "..\wg_cmd\CMMvldItemEuroBSDlg.h"
#include "..\wg_cmd\CMMvldItemEuroBSNewDlg.h"

//#include "..\wg_cmd\CMSlanTrDlg.h"
//#include "..\wg_cmd2\wg_cmd2_Dialog.h"
#include "..\wg_sw\wg_swAll.h"
#include "..\wg_cmd2\CMMvldAddImpDlg.h"

#include "..\wg_base\I_GENModelBase.h"

#include "..\wg_rps\wg_cmdPushoverAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace mit::frx;

IMPLEMENT_DYNAMIC(CTreeWorkCtrlNew, MBaseTreeCtrl);

#if defined(_CIVIL)
BOOL g_bCivil = TRUE;
#else
BOOL g_bCivil = FALSE;
#endif

BEGIN_MESSAGE_MAP(CTreeWorkCtrlNew, MBaseTreeCtrl)
	//{{AFX_MSG_MAP(CTreeWorkCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_WORKTREE_ACTIVE, OnWorktreeActive)
	ON_COMMAND(ID_WORKTREE_DELETE, OnWorktreeDelete)
	ON_COMMAND(ID_WORKTREE_INACTIVE, OnWorktreeInactive)
	ON_COMMAND(ID_WORKTREE_SELECT, OnWorktreeSelect)
	ON_COMMAND(ID_WORKTREE_UNSELECT, OnWorktreeUnselect)
	ON_COMMAND(ID_WORKTREE_UNSELECTALL, OnWorktreeUnselectall)
	ON_COMMAND(ID_WORKTREE_ACTIVEALL, OnWorktreeActiveall)
	ON_COMMAND(ID_WORKTREE_ASSIGN, OnWorktreeAssign)
	ON_COMMAND(ID_WORKTREE_PROPERTIES, OnWorktreeProperties)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_WORKTREE_ADDMATL, OnWorktreeAddmatl)
	ON_COMMAND(ID_WORKTREE_ADDSECT, OnWorktreeAddsect)
	ON_COMMAND(ID_WORKTREE_ADDTHIK, OnWorktreeAddthik)
	ON_COMMAND(ID_WORKTREE_ADDCSCS, OnWorktreeAddsect)
	ON_COMMAND(ID_WORKTREE_SELECTPLUS, OnWorktreeSelectplus)
	ON_COMMAND(ID_WORKTREE_DISPLAY, OnWorktreeDisplay)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_WORKTREE_DISAPPEAR, OnWorktreeDisappear)
	ON_COMMAND(ID_WORKTREE_DELETE_UNUS_MATL, OnWorktreeDeleteUnusMatl)
	ON_COMMAND(ID_WORKTREE_DELETE_UNUS_SECT, OnWorktreeDeleteUnusSect)
	ON_COMMAND(ID_WORKTREE_DELETE_UNUS_THIK, OnWorktreeDeleteUnusThik)
	ON_COMMAND(ID_WORKTREE_ACTIVE_PLUS, OnWorktreeActivePlus)
	ON_COMMAND(ID_WORKTREE_TABLES, OnWorktreeTables)
	ON_COMMAND(ID_WORKTREE_ENABLE, OnWorktreeEnable)
	ON_COMMAND(ID_WORKTREE_DISABLE, OnWorktreeDisable)
	ON_COMMAND(ID_WORKTREE_PROPERTY_REBAR, OnWorktreePSCRebar)
	ON_COMMAND(ID_WORKTREE_EXPANDALL, OnWorktreeExpandAll)
	ON_COMMAND(ID_WORKTREE_EXPAND_1LEVEL, OnWorktreeExpand1LEVEL)
	ON_COMMAND(ID_WORKTREE_EXPAND_2LEVEL, OnWorktreeExpand2LEVEL)
	ON_COMMAND(ID_QUERY_DGN_POVR_HNGEPROP, OnWorktreePhgePropTable)
	ON_COMMAND(ID_QUERY_DGN_POVR_HNGE, OnWorktreePhgeAssignTable)
	ON_COMMAND(ID_STRUCT_PUSHOVER_DEFINE_HINGE, OnWorktreePhgtDefine)

	ON_COMMAND(ID_WORKTREE_ADDMADO, OnWorktreeAddmado)
	ON_COMMAND(ID_WORKTREE_ADDSBDO, OnWorktreeAddsbdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTreeWorkCtrlNew::CTreeWorkCtrlNew()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_bDrag = FALSE;
	m_bDragPoss = FALSE;

	m_mFld2TreeItem.clear();
	m_mLst2KeyTreeItem.clear();
	m_mLst2TreeItemKey.clear();

	m_aMvcdStr.RemoveAll();
	m_aMvcdData.RemoveAll();
#define M_NAME_ID_ADD(NAME, ID)  m_aMvcdStr.Add(NAME); m_aMvcdData.Add(ID)
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_None), D_MOVE_CODE_NONE);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_Korea), D_MOVE_CODE_KOREA);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_AASHTO_Strd), D_MOVE_CODE_AASHTO_STAN);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_AASHTO_LRFD), D_MOVE_CODE_AASHTO_LRFD);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_PENDOT), D_MOVE_CODE_PENDOT);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_China), D_MOVE_CODE_CHINA);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_Japan), D_MOVE_CODE_JAPAN);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_India), D_MOVE_CODE_INDIA);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_Taiwan), D_MOVE_CODE_TAIWAN);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_Canada), D_MOVE_CODE_CANADA);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_BS), D_MOVE_CODE_BS);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_EURO_BS), D_MOVE_CODE_EURO_BS);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_RUSSIA), D_MOVE_CODE_RUSSIA);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_KOREA_LRFD_2011), D_MOVE_CODE_KOREA_LRFD_2011);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_AUSTRALIA), D_MOVE_CODE_AUSTRALIA);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_POLAND), D_MOVE_CODE_POLAND);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_SOUTH_AFRICA), D_MOVE_CODE_SOUTH_AFRICA);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_France), D_MOVE_CODE_FRANCE);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_TRANS), D_MOVE_CODE_TRANS);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_NEWZEALAND), D_MOVE_CODE_NEWZEALAND);
	M_NAME_ID_ADD(_LS(IDS_TM_WORKTREE_MVCD_BRAZIL), D_MOVE_CODE_BRAZIL);
#undef M_NAME_ID_ADD
}

CTreeWorkCtrlNew::~CTreeWorkCtrlNew()
{
	m_mFld2TreeItem.clear();
	m_mLst2KeyTreeItem.clear();
	m_mLst2TreeItemKey.clear();

	DeleteAllItems();
}

void CTreeWorkCtrlNew::SetBkGroundColor(COLORREF BkColor)
{
	m_BkColor = BkColor;
	if (GetSafeHwnd())
	{
		SetBkColor(m_BkColor);
		Invalidate();
	}
}

void CTreeWorkCtrlNew::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	CDBDoc* m_pDoc;

	if (lHint == D_UPDATE_DEFAULT && ((T_UPDATE_HINT*)pHint)->nCommand == 0xffffffff)
	{
		// CDBDoc::OnAfterOpenDocument() 에서 호출되는 경우임.
		return;
	}

	InitializeTree init_tree(this);

	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		CBCGPGridCtrl::m_pHighlightedItem = nullptr;
		UnselectAllItems();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		// do something...
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_DISPLAYOPTION:
	{
		COLORREF color = GetBkColor();
		SetBkColor(color);
		Invalidate();
	}
	break;
	case D_UPDATE_UNIT:
		m_pDoc = CDBDoc::GetDocPoint();
		if (!m_pDoc) { TRACE(_LS(IDS_TMWK__MSG1));	return; }
		break;
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
		// Stage가 변할 때 Work Tree를 다시 그려 줌 
		break;
	case D_UPDATE_POST_STEP_CHANGED:
		break;
	case D_UPDATE_PREF_CHANGED:
		/*-----------------------------------
			Preference가 변경될 때 Property가 변경되는 경우
			현재 Reinforcement for PSC Section경우만 업데이트 시킴
			2005.11.08 jkpark
		----------------------------------*/
		m_pDoc = CDBDoc::GetDocPoint();
		if (!m_pDoc) { TRACE(_LS(IDS_TMWK__MSG1));	return; }
		break;
	case D_UPDATE_NOTIFY:
		m_pDoc = CDBDoc::GetDocPoint();
		break;
	default:
		//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
		ASSERT(TRUE);
		break;
	}

	Invalidate();
}

void CTreeWorkCtrlNew::UpdateBuffer()
{
	CDBDoc* m_pDoc = CDBDoc::GetDocPoint();
	if (!m_pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	T_UDRD_INDEX index_ur = m_pDoc->m_vbuf->m_uridx.GetTail();
	int current_idx = index_ur.Index;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = m_pDoc->m_vbuf->m_urbuf.GetTailPosition();
	while (pos)
	{
		buffer_ur = m_pDoc->m_vbuf->m_urbuf.GetPrev(pos);
		// Tail 기준으로 Index가 같은 애들만 체크
		if (index_ur.Index != current_idx)
			break;
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_NODE_ADD): case(UR_NODE_DEL): case(UR_NODE_MFS): case(UR_NODE_MFD):
			SetNode(nCmd, nKey); break;
		case(UR_STOR_ADD): case(UR_STOR_DEL): case(UR_STOR_MFS): case(UR_STOR_MFD): case(UR_STOR_LT_ADD): case(UR_STOR_LT_DEL):
			break;
		case(UR_MODL_ADD): case(UR_MODL_DEL):
		case(UR_SPAN_ADD): case(UR_SPAN_DEL): case(UR_SPAN_MFS): case(UR_SPAN_MFD):
		case(UR_EFWD_ADD): case(UR_EFWD_DEL):
		case(UR_GRUP_ADD): case(UR_GRUP_DEL): case(UR_GRUP_MFS): case(UR_GRUP_MFD):
		case(UR_NPLN_ADD): case(UR_NPLN_DEL):
		case(UR_DIMM_ADD): case(UR_DIMM_DEL): case(UR_DIMM_MFS): case(UR_DIMM_MFD):
		case(UR_ELEM_ADD): case(UR_ELEM_DEL): case(UR_ELEM_MFS): case(UR_ELEM_MFD):
			SetElem(nCmd, nKey); break;
		case(UR_SECT_ADD): case(UR_SECT_DEL): case(UR_SECT_MFS): case(UR_SECT_MFD):
			SetSect(nCmd, nKey); break;
		case(UR_MATL_ADD): case(UR_MATL_DEL): case(UR_MATL_MFS): case(UR_MATL_MFD):
		case(UR_TDMT_ADD): case(UR_TDMT_DEL): case(UR_TDMT_MFS): case(UR_TDMT_MFD):
		case(UR_TDME_ADD): case(UR_TDME_DEL): case(UR_TDME_MFS): case(UR_TDME_MFD):
		case(UR_TDMF_ADD): case(UR_TDMF_DEL): case(UR_TDMF_MFS): case(UR_TDMF_MFD):
		case(UR_TMAT_ADD): case(UR_TMAT_DEL):
		case(UR_MHTP_ADD): case(UR_MHTP_DEL): case(UR_MHTP_MFS): case(UR_MHTP_MFD):
		case(UR_MHAT_ADD): case(UR_MHAT_DEL):
		case(UR_CSCS_ADD): case(UR_CSCS_DEL): case(UR_CSCS_MFS): case(UR_CSCS_MFD):
		case(UR_THIK_ADD): case(UR_THIK_DEL): case(UR_THIK_MFS): case(UR_THIK_MFD):
		case(UR_MPGB_ADD): case(UR_MPGB_DEL):
		case(UR_MPHG_ADD): case(UR_MPHG_DEL):
		case(UR_MPST_ADD): case(UR_MPST_DEL):
		case(UR_SECF_ADD): case(UR_SECF_DEL):
		case(UR_RPSC_ADD): case(UR_RPSC_DEL):
		case(UR_RPSC_SSM_ADD): case(UR_RPSC_SSM_DEL):
		case(UR_EDMP_ADD): case(UR_EDMP_DEL):
		case(UR_TSGR_ADD): case(UR_TSGR_DEL): case(UR_TSGR_MFS): case(UR_TSGR_MFD):
		case(UR_EPMT_ADD): case(UR_EPMT_DEL): case(UR_EPMT_MFS): case(UR_EPMT_MFD):
		case(UR_ESSF_ADD): case(UR_ESSF_DEL):
		case(UR_WSSF_ADD): case(UR_WSSF_DEL):
		case(UR_PSSF_ADD): case(UR_PSSF_DEL):
		case(UR_GRDP_ADD): case(UR_GRDP_DEL):
		case(UR_TDNT_ADD): case(UR_TDNT_DEL): case(UR_TDNT_MFS): case(UR_TDNT_MFD):
		case(UR_TDNA_ADD): case(UR_TDNA_DEL):
		case(UR_TDCS_ADD): case(UR_TDCS_DEL): case(UR_TDCS_MFS): case(UR_TDCS_MFD):
		case(UR_TDGR_ADD): case(UR_TDGR_DEL): case(UR_TDGR_MFS): case(UR_TDGR_MFD):
		case(UR_IEHP_ADD): case(UR_IEHP_DEL): case(UR_IEHP_MFS): case(UR_IEHP_MFD):
		case(UR_IEHG_ADD): case(UR_IEHG_DEL):
		case(UR_IEHC_ADD): case(UR_IEHC_DEL):
		case(UR_SIHO_ADD): case(UR_SIHO_DEL):
		case(UR_SIHG_ADD): case(UR_SIHG_DEL):
		case(UR_FIMP_ADD): case(UR_FIMP_DEL): case(UR_FIMP_MFS): case(UR_FIMP_MFD):
		case(UR_FIBR_ADD): case(UR_FIBR_DEL): case(UR_FIBR_MFS): case(UR_FIBR_MFD):
		case(UR_FIBW_ADD): case(UR_FIBW_DEL): case(UR_FIBW_MFS): case(UR_FIBW_MFD):
		case(UR_CONS_ADD): case(UR_CONS_DEL):
		case(UR_NSPR_ADD): case(UR_NSPR_DEL):
		case(UR_MLFC_ADD): case(UR_MLFC_DEL): case(UR_MLFC_MFS): case(UR_MLFC_MFD):
		case(UR_RLFC_ADD): case(UR_RLFC_DEL): case(UR_RLFC_MFS): case(UR_RLFC_MFD):
		case(UR_GSTP_ADD): case(UR_GSTP_DEL): case(UR_GSTP_MFS): case(UR_GSTP_MFD):
		case(UR_GSPR_ADD): case(UR_GSPR_DEL):
		case(UR_SSPS_ADD): case(UR_SSPS_DEL):
		case(UR_ELNK_ADD): case(UR_ELNK_DEL):
		case(UR_FRLS_ADD): case(UR_FRLS_DEL):
		case(UR_OFFS_ADD): case(UR_OFFS_DEL):
		case(UR_PRLS_ADD): case(UR_PRLS_DEL):
		case(UR_RIGD_ADD): case(UR_RIGD_DEL):
		case(UR_MCON_ADD): case(UR_MCON_DEL): // MNET:2790-JHLEE-20070911
		case(UR_DRLS_ADD): case(UR_DRLS_DEL):
		case(UR_PZEF_ADD): case(UR_PZEF_DEL):
		case(UR_SKEW_ADD): case(UR_SKEW_DEL):
		case(UR_NLLP_ADD): case(UR_NLLP_DEL): case(UR_NLLP_MFS): case(UR_NLLP_MFD):
		case(UR_EWSF_ADD): case(UR_EWSF_DEL):
		case(UR_CGLP_ADD): case(UR_CGLP_DEL):
		case(UR_CLDR_ADD): case(UR_CLDR_DEL):
		case(UR_SDVI_ADD): case(UR_SDVI_DEL): case(UR_SDVI_MFS): case(UR_SDVI_MFD):
		case(UR_SDVE_ADD): case(UR_SDVE_DEL): case(UR_SDVE_MFS): case(UR_SDVE_MFD):
		case(UR_SDST_ADD): case(UR_SDST_DEL): case(UR_SDST_MFS): case(UR_SDST_MFD):
		case(UR_SDHY_ADD): case(UR_SDHY_DEL): case(UR_SDHY_MFS): case(UR_SDHY_MFD):
		case(UR_SDIS_ADD): case(UR_SDIS_DEL): case(UR_SDIS_MFS): case(UR_SDIS_MFD):
		case(UR_ASGB_ADD): case(UR_ASGB_DEL): case(UR_ASGB_MFS): case(UR_ASGB_MFD):
		case(UR_DMGB_ADD): case(UR_DMGB_DEL): case(UR_DMGB_MFS): case(UR_DMGB_MFD):
		case(UR_NLNK_ADD): case(UR_NLNK_DEL):
		case(UR_NMAS_ADD): case(UR_NMAS_DEL):
		case(UR_DMAS_ADD): case(UR_DMAS_DEL):
		case(UR_LTOM_ADD): case(UR_LTOM_DEL):
		case(UR_STLD_ADD): case(UR_STLD_DEL): case(UR_STLD_MFS): case(UR_STLD_MFD):
		case(UR_CNLD_ADD): case(UR_CNLD_DEL):
		case(UR_NBOF_ADD): case(UR_NBOF_DEL):
		case(UR_BODF_ADD): case(UR_BODF_DEL):
		case(UR_SDSP_ADD): case(UR_SDSP_DEL):
		case(UR_BMLD_ADD): case(UR_BMLD_DEL):
		case(UR_PRES_ADD): case(UR_PRES_DEL):
		case(UR_ARPR_ADD): case(UR_ARPR_DEL):
		case(UR_FBLD_ADD): case(UR_FBLD_DEL): case(UR_FBLD_MFS): case(UR_FBLD_MFD):
		case(UR_FBLA_ADD): case(UR_FBLA_DEL):
		case(UR_PNLA_ADD): case(UR_PNLA_DEL):
		case(UR_PRST_ADD): case(UR_PRST_DEL):
		case(UR_PTNS_ADD): case(UR_PTNS_DEL):
		case(UR_STMP_ADD): case(UR_STMP_DEL):
		case(UR_NTMP_ADD): case(UR_NTMP_DEL):
		case(UR_ETMP_ADD): case(UR_ETMP_DEL):
		case(UR_GTMP_ADD): case(UR_GTMP_DEL):
		case(UR_WIND_ADD): case(UR_WIND_DEL): case(UR_WIND_MFS): case(UR_WIND_MFD):
		case(UR_SEIS_ADD): case(UR_SEIS_DEL): case(UR_SEIS_MFS): case(UR_SEIS_MFD):
		case(UR_TMLD_ADD): case(UR_TMLD_DEL):
		case(UR_CMCS_ADD): case(UR_CMCS_DEL):
		case(UR_TDPL_ADD): case(UR_TDPL_DEL):
		case(UR_STBK_ADD): case(UR_STBK_DEL):
		case(UR_ULCB_ADD): case(UR_ULCB_DEL):
		case(UR_CRPC_ADD): case(UR_CRPC_DEL):
		case(UR_FMLD_ADD): case(UR_FMLD_DEL):
		case(UR_BTMP_ADD): case(UR_BTMP_DEL):
		case(UR_PTMP_ADD): case(UR_PTMP_DEL):
		case(UR_SSEI_ADD): case(UR_SSEI_DEL):
		case(UR_EXLD_ADD): case(UR_EXLD_DEL): case(UR_EXLD_MFS): case(UR_EXLD_MFD):
		case(UR_WDPR_ADD): case(UR_WDPR_DEL): case(UR_WDPR_MFS): case(UR_WDPR_MFD):
		case(UR_WVEP_ADD): case(UR_WVEP_DEL): case(UR_WVEP_MFS): case(UR_WVEP_MFD):
		case(UR_WDFC_ADD): case(UR_WDFC_DEL): case(UR_WDFC_MFS): case(UR_WDFC_MFD):
		case(UR_EPSE_ADD): case(UR_EPSE_DEL): case(UR_EPSE_MFS): case(UR_EPSE_MFD):
		case(UR_EPST_ADD): case(UR_EPST_DEL): case(UR_EPST_MFS): case(UR_EPST_MFD):
		case(UR_POSL_ADD): case(UR_POSL_DEL): case(UR_POSL_MFS): case(UR_POSL_MFD):
		case(UR_POSP_ADD): case(UR_POSP_DEL): case(UR_POSP_MFS): case(UR_POSP_MFD):
		case(UR_SPFC_ADD): case(UR_SPFC_DEL): case(UR_SPFC_MFS): case(UR_SPFC_MFD):
		case(UR_SPLC_ADD): case(UR_SPLC_DEL): case(UR_SPLC_MFS): case(UR_SPLC_MFD):
		case(UR_THIS_ADD): case(UR_THIS_DEL): case(UR_THIS_MFS): case(UR_THIS_MFD):
		case(UR_THFC_ADD): case(UR_THFC_DEL): case(UR_THFC_MFS): case(UR_THFC_MFD):
		case(UR_THNL_ADD): case(UR_THNL_DEL):
		case(UR_THGA_ADD): case(UR_THGA_DEL):
		case(UR_THSL_ADD): case(UR_THSL_DEL):
		case(UR_THCR_ADD): case(UR_THCR_DEL):
		case(UR_THMS_ADD): case(UR_THMS_DEL):
		case(UR_THRD_ADD): case(UR_THRD_DEL): case(UR_THRD_MFS): case(UR_THRD_MFD):
		case(UR_THAT_ADD): case(UR_THAT_DEL): case(UR_THAT_MFS): case(UR_THAT_MFD):
		case(UR_THRG_ADD): case(UR_THRG_DEL):
		case(UR_THRI_ADD): case(UR_THRI_DEL):
		case(UR_THRS_ADD): case(UR_THRS_DEL):
		case(UR_THEF_ADD): case(UR_THEF_DEL):
		case(UR_THSF_ADD): case(UR_THSF_DEL):
		case(UR_CRAN_ADD): case(UR_CRAN_DEL): case(UR_CRAN_MFS): case(UR_CRAN_MFD):
		case(UR_NCRN_ADD): case(UR_NCRN_DEL):
		case(UR_MVCD_ADD): case(UR_MVCD_DEL):
		case(UR_MVLD_ADD): case(UR_MVLD_DEL): case(UR_MVLD_MFS): case(UR_MVLD_MFD):
		case(UR_LLAN_ADD): case(UR_LLAN_DEL): case(UR_LLAN_MFS): case(UR_LLAN_MFD):
		case(UR_SLAN_ADD): case(UR_SLAN_DEL): case(UR_SLAN_MFS): case(UR_SLAN_MFD):
		case(UR_LLANop_ADD): case(UR_LLANop_DEL): case(UR_LLANop_MFS): case(UR_LLANop_MFD):
		case(UR_SLANop_ADD): case(UR_SLANop_DEL): case(UR_SLANop_MFS): case(UR_SLANop_MFD):
		case(UR_SINF_ADD): case(UR_SINF_DEL): case(UR_IMPF_ADD): case(UR_IMPF_DEL):
		case(UR_MLSP_ADD): case(UR_MLSP_DEL): case(UR_DYFG_ADD): case(UR_DYFG_DEL):
		case(UR_DYNF_ADD): case(UR_DYNF_DEL): case(UR_MLSR_ADD): case(UR_MLSR_DEL):
		case(UR_DYLA_ADD): case(UR_DYLA_DEL):
		case(UR_DYNZ_ADD): case(UR_DYNZ_DEL):
		case(UR_DYBS_ADD): case(UR_DYBS_DEL):
		case(UR_DSFC_ADD): case(UR_DSFC_DEL):
		case(UR_IFBZ_ADD): case(UR_IFBZ_DEL):
		case(UR_MVHL_ADD): case(UR_MVHL_DEL): case(UR_MVHL_MFS): case(UR_MVHL_MFD):
		case(UR_MVHC_ADD): case(UR_MVHC_DEL): case(UR_MVHC_MFS): case(UR_MVHC_MFD):
		case(UR_MVLDch_ADD): case(UR_MVLDch_DEL): case(UR_MVLDch_MFS): case(UR_MVLDch_MFD):
		case(UR_LLANch_ADD): case(UR_LLANch_DEL): case(UR_LLANch_MFS): case(UR_LLANch_MFD):
		case(UR_SLANch_ADD): case(UR_SLANch_DEL): case(UR_SLANch_MFS): case(UR_SLANch_MFD):
		case(UR_MVLDjp_ADD): case(UR_MVLDjp_DEL): case(UR_MVLDjp_MFS): case(UR_MVLDjp_MFD):
		case(UR_LLANjp_ADD): case(UR_LLANjp_DEL):
		case(UR_MVLDid_ADD): case(UR_MVLDid_DEL): case(UR_MVLDid_MFS): case(UR_MVLDid_MFD):
		case(UR_LLANid_ADD): case(UR_LLANid_DEL): case(UR_LLANid_MFS): case(UR_LLANid_MFD):
		case(UR_MVLDfr_ADD): case(UR_MVLDfr_DEL): case(UR_MVLDfr_MFS): case(UR_MVLDfr_MFD):
		case(UR_LLANfr_ADD): case(UR_LLANfr_DEL): case(UR_LLANfr_MFS): case(UR_LLANfr_MFD):
		case(UR_SLANfr_ADD): case(UR_SLANfr_DEL): case(UR_SLANfr_MFS): case(UR_SLANfr_MFD):
		case(UR_MVLDbs_ADD): case(UR_MVLDbs_DEL): case(UR_MVLDbs_MFS): case(UR_MVLDbs_MFD):
		case(UR_LLANtr_ADD): case(UR_LLANtr_DEL): case(UR_LLANtr_MFS): case(UR_LLANtr_MFD):
		case(UR_MVHLtr_ADD): case(UR_MVHLtr_DEL): case(UR_MVHLtr_MFS): case(UR_MVHLtr_MFD):
		case(UR_MVLDtr_ADD): case(UR_MVLDtr_DEL): case(UR_MVLDtr_MFS): case(UR_MVLDtr_MFD):
		case(UR_SMLC_ADD): case(UR_SMLC_DEL): case(UR_SMLC_MFS): case(UR_SMLC_MFD):
		case(UR_SMPT_ADD): case(UR_SMPT_DEL): case(UR_SMPT_MFS): case(UR_SMPT_MFD):
		case(UR_CRGR_ADD): case(UR_CRGR_DEL): case(UR_CRGR_MFS): case(UR_CRGR_MFD):
		case(UR_CJFG_ADD): case(UR_CJFG_DEL): case(UR_CJFG_MFS): case(UR_CJFG_MFD):
		case(UR_PLCB_ADD): case(UR_PLCB_DEL):
		case(UR_HMTP_ADD): case(UR_HMTP_DEL): case(UR_HMTP_MFS): case(UR_HMTP_MFD):
		case(UR_HMAT_ADD): case(UR_HMAT_DEL):
		case(UR_ETFC_ADD): case(UR_ETFC_DEL): case(UR_ETFC_MFS): case(UR_ETFC_MFD):
		case(UR_CCFC_ADD): case(UR_CCFC_DEL): case(UR_CCFC_MFS): case(UR_CCFC_MFD):
		case(UR_HECB_ADD): case(UR_HECB_DEL):
		case(UR_HSPT_ADD): case(UR_HSPT_DEL):
		case(UR_HSFC_ADD): case(UR_HSFC_DEL): case(UR_HSFC_MFS): case(UR_HSFC_MFD):
		case(UR_HAHS_ADD): case(UR_HAHS_DEL):
		case(UR_HPCE_ADD): case(UR_HPCE_DEL): case(UR_HPCE_MFS): case(UR_HPCE_MFD):
		case(UR_HCRP_ADD): case(UR_HCRP_DEL):
		case(UR_STAG_ADD): case(UR_STAG_DEL): case(UR_STAG_MFS): case(UR_STAG_MFD):
		case(UR_STAG_ITEM_ADD): case(UR_STAG_ITEM_DEL):
		case(UR_BNGR_ADD): case(UR_BNGR_DEL): case(UR_BNGR_MFS): case(UR_BNGR_MFD):
		case(UR_LDGR_ADD): case(UR_LDGR_DEL): case(UR_LDGR_MFS): case(UR_LDGR_MFD):
		case(UR_PDEL_ADD): case(UR_PDEL_DEL): case(UR_PDEL_MFS): case(UR_PDEL_MFD):
		case(UR_BUCK_ADD): case(UR_BUCK_DEL): case(UR_BUCK_MFS): case(UR_BUCK_MFD):
		case(UR_EIGV_ADD): case(UR_EIGV_DEL):
		case(UR_MVCT_ADD): case(UR_MVCT_DEL):
		case(UR_MVCTch_ADD): case(UR_MVCTch_DEL):
		case(UR_MVCTjp_ADD): case(UR_MVCTjp_DEL):
		case(UR_MVCTid_ADD): case(UR_MVCTid_DEL):
		case(UR_MVCTbs_ADD): case(UR_MVCTbs_DEL):
		case(UR_MVCTtr_ADD): case(UR_MVCTtr_DEL):
		case(UR_MVCTfr_ADD): case(UR_MVCTfr_DEL):
		case(UR_HHCT_ADD): case(UR_HHCT_DEL):
		case(UR_SMCT_ADD): case(UR_SMCT_DEL):
		case(UR_NLCT_ADD): case(UR_NLCT_DEL):
		case(UR_STCT_ADD): case(UR_STCT_DEL):
		case(UR_SBCT_ADD): case(UR_SBCT_DEL): case(UR_SBCT_MFS): case(UR_SBCT_MFD):
		case(UR_BCCT_ADD): case(UR_BCCT_DEL):
		case(UR_IFGS_ADD): case(UR_IFGS_DEL): case(UR_IFGS_MFS): case(UR_IFGS_MFD):
		case(UR_EQMF_ADD): case(UR_EQMF_DEL): case(UR_EQMF_MFS): case(UR_EQMF_MFD):
		case(UR_EFCT_ADD): case(UR_EFCT_DEL): case(UR_EFCT_MFS): case(UR_EFCT_MFD):
		case(UR_INMF_ADD): case(UR_INMF_DEL): case(UR_INMF_MFS): case(UR_INMF_MFD):
		case(UR_AETL_ADD): case(UR_AETL_DEL):
		case(UR_GACD_ADD): case(UR_GACD_DEL):
		case(UR_MGDR_ADD): case(UR_MGDR_DEL): case(UR_MGDR_MFS): case(UR_MGDR_MFD):
		case(UR_CBEM_ADD): case(UR_CBEM_DEL): case(UR_CBEM_MFS): case(UR_CBEM_MFD):
		case(UR_LDLN_ADD): case(UR_LDLN_DEL): case(UR_LDLN_MFS): case(UR_LDLN_MFD):
		case(UR_GILC_ADD): case(UR_GILC_DEL): case(UR_GILC_MFS): case(UR_GILC_MFD):
		case(UR_GILD_ADD): case(UR_GILD_DEL):
		case(UR_BRGT_ADD): case(UR_BRGT_DEL):
		case(UR_BSPN_ADD): case(UR_BSPN_DEL): case(UR_BSPN_MFS): case(UR_BSPN_MFD):
		case(UR_GIMP_ADD): case(UR_GIMP_DEL): case(UR_GIMP_MFS): case(UR_GIMP_MFD):
		case(UR_CREF_ADD): case(UR_CREF_DEL): case(UR_CREF_MFS): case(UR_CREF_MFD):
		case(UR_P1LT_ADD): case(UR_P1LT_DEL): case(UR_P1LT_MFS): case(UR_P1LT_MFD):
		case(UR_P1LA_ADD): case(UR_P1LA_DEL): case(UR_P1LA_MFS): case(UR_P1LA_MFD):
		case(UR_GSTY_ADD): case(UR_GSTY_DEL): case(UR_GSTY_MFS): case(UR_GSTY_MFD):
		case(UR_LNKF_ADD): case(UR_LNKF_DEL): case(UR_LNKF_MFS): case(UR_LNKF_MFD):
		case(UR_GMAS_ADD): case(UR_GMAS_DEL):
		case(UR_POGD_ADD): case(UR_POGD_DEL):
		case(UR_POLC_ADD): case(UR_POLC_DEL): case(UR_POLC_MFS): case(UR_POLC_MFD):
		case(UR_POFC_ADD): case(UR_POFC_DEL): case(UR_POFC_MFS): case(UR_POFC_MFD):
		case(UR_PORD_ADD): case(UR_PORD_DEL): case(UR_PORD_MFS): case(UR_PORD_MFD):
		case(UR_PORG_ADD): case(UR_PORG_DEL):
		case(UR_PORE_ADD): case(UR_PORE_DEL):
		case(UR_PORI_ADD): case(UR_PORI_DEL):
		case(UR_POEF_ADD): case(UR_POEF_DEL):
		case(UR_PHGT_ADD): case(UR_PHGT_DEL): case(UR_PHGT_MFS): case(UR_PHGT_MFD):
		case(UR_PHGE_ADD): case(UR_PHGE_DEL): case(UR_PHGE_MFS): case(UR_PHGE_MFD):
		case(UR_ACOP_ADD): case(UR_ACOP_DEL):
		case(UR_REBB_ADD): case(UR_REBB_DEL): case(UR_REBB_MFS): case(UR_REBB_MFD):
		case(UR_REBC_ADD): case(UR_REBC_DEL): case(UR_REBC_MFS): case(UR_REBC_MFD):
		case(UR_REBR_ADD): case(UR_REBR_DEL): case(UR_REBR_MFS): case(UR_REBR_MFD):
		case(UR_REBW_ADD): case(UR_REBW_DEL): case(UR_REBW_MFS): case(UR_REBW_MFD):
		case(UR_RBMW_ADD): case(UR_RBMW_DEL): case(UR_RBMW_MFS): case(UR_RBMW_MFD):
		case(UR_RBMS_ADD): case(UR_RBMS_DEL): case(UR_RBMS_MFS): case(UR_RBMS_MFD):
		case(UR_MREB_ADD): case(UR_MREB_DEL):
		case(UR_RBST_ADD): case(UR_RBST_DEL): case(UR_RBST_MFS): case(UR_RBST_MFD):
		case(UR_RBSL_ADD): case(UR_RBSL_DEL): case(UR_RBSL_MFS): case(UR_RBSL_MFD):
		case(UR_RIPB_ADD): case(UR_RIPB_DEL): case(UR_RIPB_MFS): case(UR_RIPB_MFD):
		case(UR_RIPC_ADD): case(UR_RIPC_DEL): case(UR_RIPC_MFS): case(UR_RIPC_MFD):
		case(UR_DCON_ADD): case(UR_DCON_DEL):
		case(UR_MADO_ADD): case(UR_MADO_DEL):
		case(UR_SBDO_ADD): case(UR_SBDO_DEL):
		case(UR_DOEL_ADD): case(UR_DOEL_DEL):
		case(UR_PREB_ADD): case(UR_PREB_DEL): case(UR_PREB_MFS): case(UR_PREB_MFD):
		case(UR_PEBA_ADD): case(UR_PEBA_DEL): case(UR_PEBA_MFS): case(UR_PEBA_MFD):
		case(UR_PRCS_ADD): case(UR_PRCS_DEL): case(UR_PRCS_MFS): case(UR_PRCS_MFD):
		case(UR_MSTR_ADD): case(UR_MSTR_DEL): case(UR_MSTR_MFS): case(UR_MSTR_MFD):
		case(UR_DPAN_ADD): case(UR_DPAN_DEL): case(UR_DPAN_MFS): case(UR_DPAN_MFD):
		case(UR_DPAA_ADD): case(UR_DPAA_DEL): case(UR_DPAA_MFS): case(UR_DPAA_MFD):
		case(UR_CCPT_ADD): case(UR_CCPT_DEL): case(UR_CCPT_MFS): case(UR_CCPT_MFD):
		case(UR_CCPA_ADD): case(UR_CCPA_DEL): case(UR_CCPA_MFS): case(UR_CCPA_MFD):
		case(UR_MSIS_ADD): case(UR_MSIS_DEL): case(UR_MSIS_MFS): case(UR_MSIS_MFD):
		case(UR_MISA_ADD): case(UR_MISA_DEL): case(UR_MISA_MFS): case(UR_MISA_MFD):
		case(UR_FIPA_ADD): case(UR_FIPA_DEL): case(UR_FIPA_MFS): case(UR_FIPA_MFD):
		case(UR_STTF_ADD): case(UR_STTF_DEL): case(UR_STTF_MFS): case(UR_STTF_MFD):
		case(UR_FLUE_ADD): case(UR_FLUE_DEL): case(UR_FLUE_MFS): case(UR_FLUE_MFD):
		case(UR_FSSF_ADD): case(UR_FSSF_DEL): case(UR_FSSF_MFS): case(UR_FSSF_MFD):
		case(UR_PSLL_ADD): case(UR_PSLL_DEL):
		default:
			break;
		}
	} // end of while

	AdjustLayout();
}

BOOL CTreeWorkCtrlNew::GetCustomColor(HTREEITEM hItem)
{
	return MBaseTreeCtrl::GetCustomColor(hItem);
}

int CTreeWorkCtrlNew::GetSelectedItemList(CArray<HTREEITEM, HTREEITEM>& aTreeItem)
{
	aTreeItem.RemoveAll();

	CList<CBCGPGridItem*, CBCGPGridItem*> aItem;
	GetSelectedItems(aItem);

	int nCnt = aItem.GetSize();
	if (nCnt == 0)
		return 0;

	aTreeItem.SetSize(nCnt);

	POSITION pos = aItem.GetHeadPosition();
	for(int i = 0; pos != NULL; i++)
	{
		CBCGPGridItem* pItem = aItem.GetNext(pos);
		aTreeItem[i++] = TreeItem(pItem->GetParentRow());
	}
	return nCnt;
}

BOOL CTreeWorkCtrlNew::GetSelectedItemKeyList(UINT data, CArray<T_NODE_K, T_NODE_K>& rNode, CArray<T_ELEM_K, T_ELEM_K>& rElem)
{
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	rNode.RemoveAll();
	rElem.RemoveAll();

	//=== 무엇을 선택했을 까??? ===
	if (data == WORK_NODE_FLD) // Node로 selection
	{
		m_pDoc->m_pAttrCtrl->GetNodeKeyList(rNode);
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

COLORREF CTreeWorkCtrlNew::GetBkColor()
{
	return RGB(233, 235, 239);
}

COLORREF CTreeWorkCtrlNew::GetBkColorTitle()
{
	CDBDoc* m_pDoc = CDBDoc::GetDocPoint();
	COLORREF clrColor2;
	if (m_pDoc != 0 && m_pDoc->GetDispOpt() != 0)
	{
		if (m_strBkTitleColorName.IsEmpty())
		{
			COptColorObjNames CC;
			m_strBkTitleColorName = CC.m_MV_WorktreeBkTitle;
		}
		CDispOptColorObj* pColor = m_pDoc->GetDispOpt()->GetColor(m_strBkTitleColorName);
		clrColor2 = pColor->GetCurVal(0);
	}
	else
	{
		CWinApp* pApp = AfxGetApp(); ASSERT(pApp);
		CString csSection, csEntry, csValue;
		csSection = _T("DispOpt-Color");
		csEntry = _T("Tree Menu	Background Title:Value");
		csValue = pApp->GetProfileString(csSection, csEntry, NULL);
		clrColor2 = RGB(128, 128, 128);
		if (!csValue.IsEmpty()) clrColor2 = (COLORREF)_ttoi(csValue);
	}
	return clrColor2;
}

HTREEITEM CTreeWorkCtrlNew::GetPreviousItemToInsertAfter(UINT insItem)
{
	if (ItemHasChildren(m_hRootTi) == FALSE) return NULL; // Work Tree에 기존의 트리가 존재하지 않으면 

	HTREEITEM   hNextItem, hChildItem, hKeepItem;
	int  max = 0;

	hChildItem = GetChildItem(m_hRootTi);
	while (hChildItem != NULL)
	{
		hNextItem = GetNextSiblingItemWithoutFilter(hChildItem);
		UINT Data = GetItemData(hChildItem);
		if (Data < insItem)
		{
			if (Data > max) { max = Data; hKeepItem = hChildItem; }
		}
		hChildItem = hNextItem;
	}

	if (max > 0) return hKeepItem;
	else return NULL;
}

HTREEITEM CTreeWorkCtrlNew::SetSubTreeItem(UINT insItem)
{
	HTREEITEM hTI = NULL;
	CString   tStr = _T("");
	CString	  strSVGPath = _T("");
	HTREEITEM   hNextItem, hChildItem;

	switch (insItem)
	{
	case WORK_ANAL_FLD: tStr.Format(_LS(IDS_TM_WORKTREE_Analysis_Control)); strSVGPath = _T("ico24_treemenu_works_analysiscontroldata");  break;
	case WORK_STRU_FLD: tStr.Format(_LS(IDS_TMWK__MSG44)); strSVGPath = _T("ico24_treemenu_works_structures"); break;
	case WORK_GRUP_FLD: tStr.Format(_LS(IDS_TMWK_NC_MSG45)); strSVGPath = _T("ico24_Temp"); break;
	case WORK_NPLN_FLD: tStr.Format(_LS(IDS_TMWK_NC_MSG46)); strSVGPath = _T("ico24_structure_ucsplan_definenamedplane"); break;
	case WORK_PROP_FLD: tStr.Format(_LS(IDS_TMWK__MSG47)); strSVGPath = _T("ico24_properties_material_materialproperties"); break;
	case WORK_INEL_PROP_FLD: tStr.Format(_LS(IDS_TMWK__INELASTIC_PROP)); strSVGPath = _T("ico24_properties_inelproperties_inelasticmaterial"); break;
	case WORK_BNDR_FLD: tStr.Format(_LS(IDS_TMWK__MSG48)); strSVGPath = _T("ico24_boundary_supports_definesupports"); break;
	case WORK_MASS_FLD: tStr.Format(_LS(IDS_TMWK__MSG49)); strSVGPath = _T("ico24_load_structureloadsmasses_nodalmasses"); break;
	case WORK_STLD_FLD: tStr.Format(_LS(IDS_TMWK__MSG50)); strSVGPath = _T("ico24_load_structureloadsmasses_nodalloads"); break;
	case WORK_ERPR_FLD: tStr.Format(_LS(IDS_TMWK__MSG202)); strSVGPath = _T("ico24_load_pressureload_pressureloads"); break;
	case WORK_SPEC_FLD: tStr.Format(_LS(IDS_TMWK__MSG51)); strSVGPath = _T("ico24_load_timehistoryanalysisdata_groundacceleration"); break;
	case WORK_THISANL_FLD: tStr.Format(_LS(IDS_TMWK__MSG52)); strSVGPath = _T("ico24_load_timehistoryanalysisdata_groundacceleration"); break;
	case WORK_MVLDANL_FLD: tStr.Format(_LS(IDS_TMWK__MSG53)); strSVGPath = _T("ico24_analysis_analysiscontrol_movingload"); break;
	case WORK_MVLDANLjp_FLD: tStr.Format(_LS(IDS_TMWK__MSG53)); strSVGPath = _T("ico24_analysis_analysiscontrol_movingload"); break;
	case WORK_MVLDANLch_FLD: tStr.Format(_LS(IDS_TMWK__MSG53)); strSVGPath = _T("ico24_analysis_analysiscontrol_movingload"); break;
	case WORK_MVLDANLid_FLD: tStr.Format(_LS(IDS_TMWK__MSG53)); strSVGPath = _T("ico24_analysis_analysiscontrol_movingload"); break;
	case WORK_MVLDANLtr_FLD: tStr.Format(_LS(IDS_TMWK__MSG53)); strSVGPath = _T("ico24_analysis_analysiscontrol_movingload"); break;
	case WORK_STTL_FLD: tStr.Format(_LS(IDS_TMWK__MSG54)); strSVGPath = _T("ico24_load_settlementanalysisdata_settlementgroup"); break;
	case WORK_ESEQ_FLD: tStr.Format(_LS(IDS_TMWK_NC_MSG55)); strSVGPath = _T("ico24_Temp"); break;
	case WORK_PLCB_FLD: tStr.Format(_LS(IDS_TMWK__MSG56)); strSVGPath = _T("ico24_load_misc_precompositesection"); break;
	case WORK_HYDR_FLD: tStr.Format(_LS(IDS_TMWK__MSG57)); strSVGPath = _T("ico24_load_heatofhydrationanalysisdata_ambienttemperaturefunctions"); break;
	case WORK_STAG_FLD: tStr.Format(_LS(IDS_TMWK_NC_MSG172)); strSVGPath = _T("ico24_load_constructionstagedata_definecs"); break;
	case WORK_PSTD_FLD: tStr.Format(_LS(IDS_TM_WORKTREE_Prestressing_Tendon)); strSVGPath = _T("ico24_load_prestressloads_tendonprestress"); break;
	case WORK_GRID_FLD: tStr.Format(_LS(IDS_TMWK_Grid_Analysis_Data)); strSVGPath = _T("ico24_loads_Grid_jpn5"); break;
	case WORK_PUSH_FLD: tStr.Format(_LS(IDS_TMWK_Pushover_Analysis)); strSVGPath = _T("ico24_pushover_properties_assignpushoverhingeproperties"); break;
	case WORK_CRAN_FLD: tStr.Format(_LS(IDS_TREE_CRAN)); strSVGPath = _T("ico24_analysis_analysiscontrol_movingload"); break;
	case WORK_DIMM_FLD: tStr.Format(_LS(IDS_TMWK_NC_MSG191)); strSVGPath = _T("ico24_nodeelement_nodes_scale"); break;
	case WORK_REBD_FLD: tStr.Format(_LS(IDS_TM_REBAR_DATA)); strSVGPath = _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"); break;
	case WORK_DOMN_FLD: tStr.Format(_LS(IDS_TMWK_NC_MSG192)); strSVGPath = _T("ico24_nodeelement_mesh_definedomain"); break;
	case WORK_PREB_FLD: tStr.Format(_LS(IDS_TMWK_NC_PREB)); strSVGPath = _T("ico24_mods_design_dropmenu2_limitingmaximumrebarratio"); break;
	case WORK_PRCS_FLD: tStr.Format(_LS(IDS_TMWK_NC_PRCS)); strSVGPath = _T("ico24_rating_bridgeratingdesign_dropmenu1_webpanelassignment"); break;
	case WORK_DPAN_FLD: tStr.Format(_LS(IDS_TMWK_NC_DPAN)); strSVGPath = _T("ico24_structure_wizard_spaninformation"); break;
	case WORK_CCPT_FLD: tStr.Format(_LS(IDS_TMWK_NC_CCPT)); strSVGPath = _T("ico24_seismicperform_evaluationtype_piereval"); break;
	case WORK_MSIS_FLD: tStr.Format(_LS(IDS_TMWK_NC_MSIS)); strSVGPath = _T("ico24_mods_mesh_automesh"); break;
	case WORK_FIRE_FLD: tStr.Format(_LS(IDS_TREE_FIRE)); strSVGPath = _T("ico24_mods_design_dropmenu1_loadcombinationtype"); break;
	case WORK_STLD_TYPE_FLD: tStr.Format(_LS(IDS_WG_TREEMENU_Loads_Type)); strSVGPath = _T("ico24_load_structureloadsmasses_nodalloads"); break;
	case WORK_CRGR_FLD: tStr.Format(_LS(IDS_TMWK__MSG2114)); strSVGPath = _T("ico24_load_settlementanalysisdata_concurrentreactiongroup"); break;
	case WORK_CJFG_FLD: tStr.Format(_LS(IDS_TMWK__MSG2116)); strSVGPath = _T("ico24_load_settlementanalysisdata_concurrentjointforcegroup"); break;

	}

	std::map<int, HTREEITEM>::iterator itr;
	if ((itr = m_mFld2TreeItem.find(insItem)) != m_mFld2TreeItem.end()) // 처음 생성하는게 아니면
	{
		hTI = itr->second;
		BOOL  bExist = FALSE;

		if (hTI != NULL) // ITEM이 존재하면 
		{
			// 그 그룹의 Text를 Refresh : 갯수가 바뀔 수 있으므로    
			SetItemText(hTI, tStr.LockBuffer());
			//SetItemDisplayCount(hChildItem, nCount);

			hChildItem = GetChildItem(hTI);
			while (hChildItem != NULL)
			{
				hNextItem = GetNextSiblingItemWithoutFilter(hChildItem);
				DeleteItem(hChildItem);
				hChildItem = hNextItem;
			}
		}
		else // 처음은 아닌데 ITEM이 존재하지 않으면 
		{
			BOOL  bExpand = FALSE;
			if (ItemHasChildren(m_hRootTi) == FALSE) bExpand = TRUE; // 처음 삽입되는 아이템이면 펼쳐라

			int imageID = AddSVGImage(strSVGPath);

			TV_INSERTSTRUCT  TreeCtrlItem;
			HTREEITEM insAfter = GetPreviousItemToInsertAfter(insItem);
			if (insAfter == NULL) TreeCtrlItem.hInsertAfter = TVI_FIRST;
			else 	             TreeCtrlItem.hInsertAfter = insAfter;
			TreeCtrlItem.hParent = m_hRootTi;
			TreeCtrlItem.item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
			TreeCtrlItem.item.lParam = insItem;
			TreeCtrlItem.item.iImage = imageID;
			TreeCtrlItem.item.iSelectedImage = imageID;
			TreeCtrlItem.item.pszText = tStr.LockBuffer();

			hTI = InsertItem(&TreeCtrlItem);
			if (bExpand) ExpandCompletely(m_hRootTi, TRUE);
		}
	}
	else // 처음생성하는 것이라면 
	{
		hTI = InsertTreeItem(m_hRootTi, insItem, tStr, strSVGPath);
	}
	return hTI;
}

BOOL CTreeWorkCtrlNew::SelectEntity(UINT data, BOOL bReplace)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	BOOL bResult = GetSelectedItemKeyList(data, aNodeList, aElemList);

	m_pDoc->m_pViewCtrl->Select(NULL, aNodeList, aElemList, FALSE, bReplace);
	return bResult;
}

void CTreeWorkCtrlNew::UnselectEntity(UINT data)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	GetSelectedItemKeyList(data, aNodeList, aElemList);

	m_pDoc->m_pViewCtrl->Unselect(NULL, aNodeList, aElemList, FALSE);
}

void CTreeWorkCtrlNew::ActiveEntity(UINT data)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	GetSelectedItemKeyList(data, aNodeList, aElemList);

	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
	if (pIGM) pIGM->ActiveThisObjects(aNodeList, aElemList);
}

void CTreeWorkCtrlNew::ActiveEntityPlus(UINT data)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	GetSelectedItemKeyList(data, aNodeList, aElemList);

	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
	//  if (pIGM) pIGM->ManagementObject_ActOrInAct(aNodeList, aElemList, TRUE, 1);
	if (pIGM) pIGM->ManagementObject_ActOrInAct(aNodeList, aElemList, FALSE, 1);
}

void CTreeWorkCtrlNew::InactiveEntity(UINT data)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	GetSelectedItemKeyList(data, aNodeList, aElemList);

	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
	if (pIGM) pIGM->InActiveThisObjects(aNodeList, aElemList);
}

void CTreeWorkCtrlNew::DeleteItemEntity(CArray<HTREEITEM, HTREEITEM>& aItem)
{
	HTREEITEM parentItem = GetParentItem(aItem[0]);
	UINT parentData = GetItemData(parentItem);
	
	HTREEITEM ht, prvHt;
	int pos = 0;
	ht = GetSelectedItem();
	for (prvHt = ht; prvHt != NULL; ht = prvHt, pos++) prvHt = GetPrevSiblingItem(ht);

	UINT data = GetItemData(aItem[0]);
	BOOL bSuccess = FALSE;

	if (data == WORK_NODE_FLD) // Node로 지울 때
	{
		CArray<T_NODE_K, T_NODE_K> aNodeKeyList;
		m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeKeyList);
		bSuccess = m_pDoc->m_pDataCtrl->DelNode(aNodeKeyList);
		aNodeKeyList.RemoveAll();
	}
}

void CTreeWorkCtrlNew::InitTree()
{
	DeleteAllItems();

	m_mFld2TreeItem.clear();
	m_mLst2KeyTreeItem.clear();
	m_mLst2TreeItemKey.clear();

	m_hRootTi = GetRootItem();
	m_hRootTi = InsertTreeItem(m_hRootTi, WORK_ROOT, _LS(IDS_TMWK__MSG2), _T("ico24_treemenu_works_works"));

	// 갯수가 변하지 않는 고정 아이템 위치 잡기 위해 먼저 생성. 이후 숨기기로 위치 그대로 유지하는 방식. 데이터 중복 금지
	CString str;
	HTREEITEM hTI, hTI2;

	// 일반적으로 하나만 생성되는 애들은 이거 사용
	auto InsertTreeItem_ = [this](HTREEITEM hParent, int nData, CString svg) -> HTREEITEM
	{
		HTREEITEM hTI = InsertTreeItem(hParent, nData, svg, svg);
#if defined(_DEBUG)
		if (nData != NULL && m_mFld2TreeItem.find(nData) != m_mFld2TreeItem.end())
			ASSERT(0);
#endif
		m_mFld2TreeItem[nData] = hTI;
		return hTI;
	};

	// 이름 포함하기. Root에서 바로 생성되는 아이템들이 주로 사용.
	auto InsertTreeItem_str = [this](HTREEITEM hParent, int nData, CString str, CString svg) -> HTREEITEM
	{
		HTREEITEM hTI = InsertTreeItem(hParent, nData, str, svg);
#if defined(_DEBUG)
		if (m_mFld2TreeItem.find(nData) != m_mFld2TreeItem.end())
			ASSERT(0);
#endif
		m_mFld2TreeItem[nData] = hTI;
		return hTI;
	};

	// Moving에서 유니크하게 사용. 국가 코드 때문에..
	auto InsertTreeItem_MV = [this](std::vector<int> aCode, HTREEITEM hParent, int nData, CString svg) -> HTREEITEM
	{
		HTREEITEM hTI = InsertTreeItem(hParent, nData, svg, svg);
		for (int nMVCode : aCode)
		{
#if defined(_DEBUG)
			if (m_mMvcd2FldTreeItem[nMVCode].find(nData) != m_mMvcd2FldTreeItem[nMVCode].end())
				ASSERT(0);
#endif
			m_mMvcd2FldTreeItem[nMVCode][nData] = hTI;
		}
		return hTI;
	};

	// Moving에서 유니크하게 사용. 국가 코드 때문에..
	auto InsertTreeItem_strMV = [this](std::vector<int> aCode, HTREEITEM hParent, int nData, CString str, CString svg) -> HTREEITEM
	{
		HTREEITEM hTI = InsertTreeItem(hParent, nData, str, svg);
		for (int nMVCode : aCode)
		{
#if defined(_DEBUG)
			if (m_mMvcd2FldTreeItem[nMVCode].find(nData) != m_mMvcd2FldTreeItem[nMVCode].end())
				ASSERT(0);
#endif
			m_mMvcd2FldTreeItem[nMVCode][nData] = hTI;
		}
		return hTI;
	};

	//Analysis
	hTI = InsertTreeItem_str(m_hRootTi, WORK_ANAL_FLD, _LS(IDS_TM_WORKTREE_Analysis_Control), _T("ico24_treemenu_works_analysiscontroldata"));
	InsertTreeItem_(hTI, WORK_PDEL_FLD, _T("ico24_analysis_analysiscontrol_pdelta"));
	InsertTreeItem_(hTI, WORK_BUCK_FLD, _T("ico24_analysis_analysiscontrol_buckling"));
	InsertTreeItem_(hTI, WORK_EIGV_FLD, _T("ico24_analysis_analysiscontrol_eigenvalue"));
	InsertTreeItem_(hTI, WORK_MVCT_FLD, _T("ico24_analysis_analysiscontrol_movingload"));
	InsertTreeItem_(hTI, WORK_HHCT_FLD, _T("ico24_analysis_analysiscontrol_heatofhydration"));
	InsertTreeItem_str(hTI, WORK_SMCT_FLD, _LS(IDS_TM_WORKTREE_Settlement_Analysis_Data), _T("ico24_analysis_analysiscontrol_settlement"));
	InsertTreeItem_(hTI, WORK_NLCT_FLD, _T("ico24_analysis_analysiscontrol_nonlinear"));
	InsertTreeItem_(hTI, WORK_NLCT_LST, _T("ico24_analysis_analysiscontrol_nonlinear"));
	InsertTreeItem_(hTI, WORK_STCT_FLD, _T("ico24_analysis_analysiscontrol_constructionstage"));
	InsertTreeItem_(hTI, WORK_SBCT_FLD, _T("ico24_analysis_analysiscontrol_suspensionbridge"));
	InsertTreeItem_str(hTI, WORK_BCCT_FLD, _LS(IDS_TM_WORKTREE_BCCT_Analysis_Data), _T("ico24_analysis_analysiscontrol_boundarychagneassignment"));
	InsertTreeItem_str(hTI, WORK_AETL_FLD, _LS(IDS_TM_WORKTREE_AETL_Analysis_Data), _T("ico24_analysis_analysiscontrol_approximatetendonlosses"));
	hTI2 = InsertTreeItem_str(hTI, WORK_ILDS_FLD, _LS(IDS_TM_WORKTREE_AETL_Analysis_Data), _T("ico24_load_initialforcesetc_initialforcesforgeometricstiffness"));
	InsertTreeItem_(hTI2, WORK_IFGS_FLD, _T("ico24_load_initialforcesetc_initialforcesforgeometricstiffness"));
	InsertTreeItem_(hTI2, WORK_EQMF_FLD, _T("ico24_load_initialforcesetc_euiilibriumelementnodalforces"));
	hTI2 = InsertTreeItem_(hTI, WORK_ISDS_FLD, _T("ico24_load_initialforcesetc_initialforcescontroldata"));
	InsertTreeItem_(hTI2, WORK_IFCT_FLD, _T("ico24_load_initialforcesetc_initialforcescontroldata"));
	InsertTreeItem_(hTI2, WORK_INMF_FLD, _T("ico24_load_initialforcesetc_initialelementforces"));

	//Geometry
	hTI = InsertTreeItem_str(m_hRootTi, WORK_STRU_FLD, _LS(IDS_TMWK__MSG44), _T("ico24_properties_material_materialproperties"));
	InsertTreeItem_str(hTI, WORK_STOR_FLD, _LS(IDS_TMWK_NC_Stories_d), _T("ico24_Temp"));
	InsertTreeItem_str(hTI, WORK_NODE_FLD, _LS(IDS_TMWK_NC_MSG3), _T("ico24_nodeelement_nodes_nodestable"));
	hTI2 = InsertTreeItem_str(hTI, WORK_ELEM_FLD, _LS(IDS_TMWK_NC_MSG4), _T("ico24_nodeelement_element_elementtable"));
	InsertTreeItem_str(hTI2, WORK_TRUSS_LST, _LS(IDS_TMWK_NC_MSG5), _T("ico24_structure_wizard_truss"));
	InsertTreeItem_str(hTI2, WORK_TRTENS_LST, _LS(IDS_TMWK_NC_MSG6), _T("ico24_structure_wizard_truss"));
	InsertTreeItem_str(hTI2, WORK_TRCOMP_LST, _LS(IDS_TMWK_NC_MSG7), _T("ico24_structure_wizard_truss"));
	InsertTreeItem_str(hTI2, WORK_BEAM_LST, _LS(IDS_TMWK_NC_MSG8), _T("ico24_structure_wizard_beam"));
	InsertTreeItem_str(hTI2, WORK_PLATE_LST, _LS(IDS_TMWK_NC_MSG9), _T("ico24_structure_wizard_plate"));
	InsertTreeItem_str(hTI2, WORK_PLSTRS_LST, _LS(IDS_TMWK_NC_MSG10), _T("ico24_structure_wizard_frame"));
	InsertTreeItem_str(hTI2, WORK_PLSTRN_LST, _LS(IDS_TMWK_NC_MSG11), _T("ico24_structure_wizard_frame"));
	InsertTreeItem_str(hTI2, WORK_AXISYM_LST, _LS(IDS_TMWK_NC_MSG12), _T("ico24_structure_wizard_shell"));
	InsertTreeItem_str(hTI2, WORK_SOLID_LST, _LS(IDS_TMWK_NC_MSG13), _T("ico24_view_dynamicview_viewpoint"));
	InsertTreeItem_str(hTI2, WORK_WALL_LST, _LS(IDS_TMWK_NC_MSG14), _T("ico24_Temp"));
	InsertTreeItem_str(hTI2, WORK_WALLOPEN_LST, _LS(IDS_TMWK_NC_MSG_WallOpen), _T("ico24_Temp"));
	InsertTreeItem_str(hTI, WORK_SPAN_FLD, _LS(IDS_TMWK_NC_MSG188), _T("ico24_structure_wizard_spaninformation"));
	InsertTreeItem_str(hTI, WORK_SPAN_COMPOSTIE_FLD, _LS(IDS_TMWK_NC_MSG188_COMPOSITE), _T("ico24_structure_wizard_spaninformation"));
	InsertTreeItem_str(hTI, WORK_SPAN_COMPOSTIE_VB_FLD, _LS(IDS_TMWK_NC_MSG188_COMPOSITE_VB), _T("ico24_structure_wizard_spaninformation"));
	InsertTreeItem_str(hTI, WORK_SPAN_STEELGIRDER_FLD, _LS(IDS_TMWK_NC_MSG188_STEELGIRDER), _T("ico24_structure_wizard_spaninformation"));
	InsertTreeItem_str(hTI, WORK_EFWD_FLD, _LS(IDS_TM_WORKTREE_NC_Efwd), _T("ico24_structure_wizard_effectivewidth"));

	//Name Plane
	InsertTreeItem_str(m_hRootTi, WORK_NPLN_FLD, _LS(IDS_TMWK_NC_MSG46), _T("ico24_structure_ucsplan_definenamedplane"));

	//Dimension
	InsertTreeItem_str(m_hRootTi, WORK_DIMM_FLD, _LS(IDS_TMWK_NC_MSG191), _T("ico24_nodeelement_nodes_scale"));

	//Property
	hTI = InsertTreeItem_str(m_hRootTi, WORK_PROP_FLD, _LS(IDS_TMWK__MSG47), _T("ico24_properties_material_materialproperties"));
	InsertTreeItem_str(hTI, WORK_MATL_FLD, _LS(IDS_TMWK_NC_MSG60), _T("ico24_properties_material_materialproperties"));
	InsertTreeItem_str(hTI, WORK_TDMF_FLD, _LS(IDS_TMWK__MSG181), _T("ico24_properties_tdm_userdefine"));
	InsertTreeItem_str(hTI, WORK_TDMT_FLD, _LS(IDS_TMWK__MSG180), _T("ico24_properties_tdm_creepshrinkage"));
	InsertTreeItem_str(hTI, WORK_TMAT_FLD, _LS(IDS_TMWK__MSG182), _T("ico24_properties_tdm_materiallink"));
	InsertTreeItem_str(hTI, WORK_MHTP_FLD, _LS(IDS_TM_WORKTREE_High_Temperature_Material_Properties__Define), _T("ico24_properties_tdm_userdefine"));
	InsertTreeItem_str(hTI, WORK_MHAT_FLD, _LS(IDS_TM_WORKTREE_High_Temperature_Material_Properties__Link), _T("ico24_properties_tdm_materiallink"));
	InsertTreeItem_(hTI, WORK_EDMP_FLD, _T("ico24_properties_tdm_changeproperty"));
	InsertTreeItem_str(hTI, WORK_SECT_FLD, _LS(IDS_TMWK_NC_MSG63), _T("ico24_properties_section_sectionproperties"));
	InsertTreeItem_str(hTI, WORK_SECF_FLD, _LS(IDS_TM_WORKTREE_Section_Stiffness_Scale_Factor), _T("ico24_properties_section_stiffness"));
	InsertTreeItem_str(hTI, WORK_ESSF_FLD, _LS(IDS_TM_WORKTREE_Element_Stiffness_Scale_Factor), _T("ico24_properties_section_stiffness"));
	InsertTreeItem_str(hTI, WORK_RPSC_FLD, _LS(IDS_TM_WORKTREE_NC_Reinforcement), _T("ico24_mods_design_dropmenu2_columnsectiondatafordesign"));
	InsertTreeItem_str(hTI, WORK_TSGR_FLD, _LS(IDS_TM_WORKTREE_Tapered_Section_Group), _T("ico24_properties_section_taperedgroup"));
	InsertTreeItem_str(hTI, WORK_CSCS_FLD, _LS(IDS_TBWK_NC_CSCS), _T("ico24_load_constructionstagedata_compositesectionforcs"));
	InsertTreeItem_str(hTI, WORK_THIK_FLD, _LS(IDS_TMWK_NC_MSG64), _T("ico24_properties_section_thickness"));
	InsertTreeItem_str(hTI, WORK_VBEM_FLD, _LS(IDS_WG_TREEMENU_Section_for_Resultant_Forces), _T("ico24_properties_section_thickness"));
	InsertTreeItem_str(hTI, WORK_SECV_FLD, _LS(IDS_MAIN_RIBBON_MODS_STL_RATING_VIRTUAL_BEAM), _T("ico24_properties_section_thickness"));
	InsertTreeItem_str(hTI, WORK_MPGB_FLD, _LS(IDS_TMWK_MPHI_GLOBAL_DATA), _T("ico24_properties_inelproperties_inelasticmaterial"));
	InsertTreeItem_str(hTI, WORK_MPHG_FLD, _LS(IDS_TMWK_MPHI_HINGE), _T("ico24_properties_inelproperties_inelasticmaterialproperties"));
	InsertTreeItem_str(hTI, WORK_MPST_FLD, _LS(IDS_TMWK_MPHI_SECTION), _T("ico24_properties_inelproperties_fiberdivisionofsection"));
	InsertTreeItem_str(hTI, WORK_WSSF_FLD, _LS(IDS_TM_Wall_Shear_Stiffness_Reduction_Factor), _T("ico24_Temp"));
	InsertTreeItem_str(hTI, WORK_PSSF_FLD, _LS(IDS_TM_Plate_Shear_Stiffness_Reduction_Factor), _T("ico24_properties_section_platestiffnessscalefactor"));
	InsertTreeItem_str(hTI, WORK_GRDP_FLD, _LS(IDS_TM_GROUP_DAMPING_TITLE), _T("ico24_properties_damping_groupdamping"));

	//Inelastic Property
	hTI = InsertTreeItem_str(m_hRootTi, WORK_INEL_PROP_FLD, _LS(IDS_TMWK__INELASTIC_PROP), _T("ico24_properties_inelproperties_inelasticmaterial"));
	InsertTreeItem_str(hTI, WORK_IEHC_FLD, _LS(IDS_TM_IEHC_TITLE), _T("ico24_properties_inelproperties_inelcontraldata"));
	InsertTreeItem_str(hTI, WORK_SIHO_FLD, _LS(IDS_MAIN_SIHO_ELEM), _T("ico24_properties_inelproperties_selectindlastichingeresultoutput(elements)"));
	InsertTreeItem_str(hTI, WORK_SIHG_FLD, _LS(IDS_MAIN_SIHO_GLINK), _T("ico24_properties_inelproperties_selectindlastichingeresultoutput(generallink)"));
	InsertTreeItem_str(hTI, WORK_IEHP_FLD, _LS(IDS_TW_NC_INELATIC_HINGE_PROPERTIES_d), _T("ico24_properties_inelproperties_inelastichingeproperties"));
	InsertTreeItem_str(hTI, WORK_IEHG_FLD, _LS(IDS_TW_NC_INELATIC_HINGES_d), _T("ico24_properties_inelproperties_assigninelastichingeproperties"));
	InsertTreeItem_str(hTI, WORK_FIMP_FLD, _LS(IDS_TM_NC_WORKTREE_Fiber_Material_Property_d), _T("ico24_properties_inelproperties_inelasticmaterialproperties"));
	InsertTreeItem_str(hTI, WORK_FIBR_FLD, _LS(IDS_TM_NC_WORKTREE_Fiber_Division_Section_d), _T("ico24_properties_inelproperties_fiberdivisionofsection"));
	InsertTreeItem_str(hTI, WORK_FIBW_FLD, _LS(IDS_TM_NC_WORKTREE_Fiber_Division_Section_Wall_d), _T("ico24_Temp"));

	//Boundary
	hTI = InsertTreeItem_str(m_hRootTi, WORK_BNDR_FLD, _LS(IDS_TMWK__MSG48), _T("ico24_boundary_supports_definesupports"));
	InsertTreeItem_str(hTI, WORK_CONS_FLD, _LS(IDS_TMWK_NC_MSG143), _T("ico24_boundary_supports_definesupports"));
	hTI2 = InsertTreeItem_str(hTI, WORK_NSPR_FLD, _LS(IDS_TMWK_NC_MSG145), _T("ico24_boundary_springsupports_pointspring"));
	InsertTreeItem_str(hTI2, WORK_NSPR_LIN_FLD, _LS(IDS_TMWK_NC_LINEAR), _T("ico24_boundary_springsupports_pointspring"));
	InsertTreeItem_str(hTI2, WORK_NSPR_NON_FLD, _LS(IDS_TMWK_NC_NONLINEAR), _T("ico24_boundary_springsupports_pointspring"));
	InsertTreeItem_str(hTI2, WORK_NSPR_MLT_FLD, _LS(IDS_TM_NC_MULTI_LINEAR_d__), _T("ico24_boundary_springsupports_pointspring"));
	InsertTreeItem_str(hTI, WORK_GSTP_FLD, _LS(IDS_TMWK_NC_MSG147), _T("ico24_boundary_springsupports_generalspring"));
	hTI2 = InsertTreeItem_str(hTI, WORK_SSPS_FLD, _LS(IDS_TM_NC_SSPS), _T("ico24_boundary_springsupports_surfacespring"));
	InsertTreeItem_str(hTI2, WORK_SSPS_LIN_FLD, _LS(IDS_TM_NC_SSPS_Linear), _T("ico24_boundary_springsupports_surfacespring"));
	InsertTreeItem_str(hTI2, WORK_SSPS_NON_FLD, _LS(IDS_TM_NC_SSPS_CompTens), _T("ico24_boundary_springsupports_surfacespring"));
	InsertTreeItem_str(hTI, WORK_ELNK_FLD, _LS(IDS_TMWK_NC_MSG148), _T("ico24_boundary_link_elasticlink"));
	InsertTreeItem_str(hTI, WORK_NLLP_FLD, _LS(IDS_TM_NC_WORKTREE_Nonlinear_Link_Properties____d), _T("ico24_boundary_link_generallinkproperties"));
	InsertTreeItem_str(hTI, WORK_NLNK_FLD, _LS(IDS_TM_NC_WORKTREE_Nonlinear_Link____d), _T("ico24_boundary_link_generallink2"));
	InsertTreeItem_str(hTI, WORK_CGLP_FLD, _LS(IDS_TMTB_Change_General_Link_Property), _T("ico24_boundary_link_changegenerallinkproperties"));
	hTI2 = InsertTreeItem_(hTI, WORK_SSDV_FLD, _T("ico24_boundary_link_viscousdamperoildamper"));
	InsertTreeItem_str(hTI2, WORK_SDVI_FLD, _LS(IDS_TM_MENUCTRL_SDVI), _T("ico24_boundary_link_viscousdamperoildamper"));
	InsertTreeItem_str(hTI2, WORK_SDVE_FLD, _LS(IDS_TM_MENUCTRL_SDVE), _T("ico24_boundary_link_viscoelasticdamper"));
	InsertTreeItem_str(hTI2, WORK_SDST_FLD, _LS(IDS_TM_MENUCTRL_SDST), _T("ico24_boundary_link_steeldamper"));
	InsertTreeItem_str(hTI2, WORK_SDHY_FLD, _LS(IDS_TM_MENUCTRL_SDHY), _T("ico24_boundary_link_hystereticisolator(mss)"));
	InsertTreeItem_str(hTI2, WORK_SDIS_FLD, _LS(IDS_TM_MENUCTRL_SDIS), _T("ico24_boundary_link_isolator(mss)"));
	InsertTreeItem_str(hTI2, WORK_ASGB_FLD, _LS(IDS_TM_MENUCTRL_ASGB), _T("ico24_boundary_link_seismiccontroldevicedbmanager"));
	InsertTreeItem_str(hTI2, WORK_DMGB_FLD, _LS(IDS_TM_MENUCTRL_DMGB), _T("ico24_Temp"));
	InsertTreeItem_str(hTI, WORK_FRLS_FLD, _LS(IDS_TMWK_NC_MSG149), _T("ico24_boundary_releaseoffset_beamendrelease"));
	InsertTreeItem_str(hTI, WORK_OFFS_FLD, _LS(IDS_TMWK_NC_MSG150), _T("ico24_boundary_releaseoffset_beamendoffsets"));
	InsertTreeItem_str(hTI, WORK_PRLS_FLD, _LS(IDS_TMWK_NC_MSG154), _T("ico24_boundary_releaseoffset_plateendrelease"));
	InsertTreeItem_str(hTI, WORK_RIGD_FLD, _LS(IDS_TMWK_NC_MSG155), _T("ico24_boundary_link_rigidlink"));
	InsertTreeItem_str(hTI, WORK_MCON_FLD, _LS(IDS_WG_TREEMENU_Linear_Constraints), _T("ico24_boundary_etc_linearconstraints"));
	InsertTreeItem_str(hTI, WORK_DRLS_FLD, _LS(IDS_TMWK_NC_MSG158), _T("ico24_boundary_link_rigidlink"));
	InsertTreeItem_(hTI, WORK_PZEF_FLD, _T("ico24_boundary_etc_panelzoneeffects"));
	InsertTreeItem_str(hTI, WORK_SKEW_FLD, _LS(IDS_TMWK_NC_MSG162), _T("ico24_boundary_etc_nodelocalaxis"));
	InsertTreeItem_str(hTI, WORK_EWSF_FLD, _LS(IDS_TM_NC_WORK_EWSF_d), _T("ico24_boundary_etc_effectivewidth"));
	InsertTreeItem_str(hTI, WORK_CLDR_FLD, _LS(IDS_TMWK__CLDR), _T("ico24_boundary_etc_definelabeldir"));
	InsertTreeItem_str(hTI, WORK_MLFC_FLD, _LS(IDS_TMWK_NC_MLFC), _T("ico24_boundary_link_forces-deformationfunction"));
	InsertTreeItem_str(hTI, WORK_RLFC_FLD, _LS(IDS_TMWK_NC_RLFC), _T("ico24_boundary_link_rail-trackinteractionfunction"));

	//Mass
	hTI = InsertTreeItem_str(m_hRootTi, WORK_MASS_FLD, _LS(IDS_TMWK__MSG49), _T("ico24_load_structureloadsmasses_nodalmasses"));
	InsertTreeItem_str(hTI, WORK_NMAS_FLD, _LS(IDS_TMWK_NC_MSG166), _T("ico24_load_structureloadsmasses_nodalmasses"));
	InsertTreeItem_str(hTI, WORK_DMAS_FLD, _LS(IDS_TMWK_NC_MSG167), _T("ico24_load_structureloadsmasses_nodalmasses"));
	InsertTreeItem_str(hTI, WORK_LTOM_FLD, _LS(IDS_TMWK_NC_MSG168), _T("ico24_load_structureloadsmasses_loadstomasses"));

	//Static Loads
	hTI = InsertTreeItem_str(m_hRootTi, WORK_STLD_FLD, _LS(IDS_TMWK__MSG50), _T("ico24_load_structureloadsmasses_nodalloads"));

	//Earth Pressure
	hTI = InsertTreeItem_str(m_hRootTi, WORK_ERPR_FLD, _LS(IDS_TMWK__MSG202), _T("ico24_load_pressureload_pressureloads"));
	InsertTreeItem_(hTI, WORK_POSL_FLD, _T("ico24_load_timehistoryanalysisdata_timehistoryfunction"));
	InsertTreeItem_(hTI, WORK_POSP_FLD, _T("ico24_boundary_springsupports_integralbridge"));

	//Tendon
	hTI = InsertTreeItem_str(m_hRootTi, WORK_PSTD_FLD, _LS(IDS_TM_WORKTREE_Prestressing_Tendon), _T("ico24_load_prestressloads_tendonprestress"));
	InsertTreeItem_str(hTI, WORK_TDNT_FLD, _LS(IDS_TM_WORKTREE_Tendon_Property), _T("ico24_load_prestressloads_tendonproperty"));
	InsertTreeItem_str(hTI, WORK_TDNA_FLD, _LS(IDS_TM_WORKTREE_Tendon_Profile), _T("ico24_load_prestressloads_tendonprofile"));
	InsertTreeItem_str(hTI, WORK_TDCS_FLD, _LS(IDS_TM_WORKTREE_Tendon_Location_Comp_Sect), _T("ico24_load_prestressloads_tendonprofile"));

	//Response Spectrum
	hTI = InsertTreeItem_str(m_hRootTi, WORK_SPEC_FLD, _LS(IDS_TMWK__MSG51), _T("ico24_load_timehistoryanalysisdata_groundacceleration"));
	InsertTreeItem_str(hTI, WORK_SPFC_FLD, _LS(IDS_TMWK_NC_MSG66), _T("ico24_load_responsespectrumdata_rsfunctions"));
	InsertTreeItem_str(hTI, WORK_SPLC_FLD, _LS(IDS_TMWK_NC_MSG72), _T("ico24_load_responsespectrumdata_rsloadcases"));

	//Time History
	hTI = InsertTreeItem_str(m_hRootTi, WORK_THISANL_FLD, _LS(IDS_TMWK__MSG52), _T("ico24_load_timehistoryanalysisdata_groundacceleration"));
	InsertTreeItem_str(hTI, WORK_THGC_FLD, _LS(IDS_TM_THGC), _T("ico24_Temp"));
	InsertTreeItem_str(hTI, WORK_THIS_FLD, _LS(IDS_TMWK_NC_MSG90), _T("ico24_load_timehistoryanalysisdata_loadcases"));
	InsertTreeItem_str(hTI, WORK_THFC_FLD, _LS(IDS_TMWK_NC_MSG75), _T("ico24_load_timehistoryanalysisdata_timehistoryfunction"));
	InsertTreeItem_str(hTI, WORK_THGA_FLD, _LS(IDS_TMWK_NC_MSG85), _T("ico24_load_timehistoryanalysisdata_groundacceleration"));
	InsertTreeItem_str(hTI, WORK_THNL_FLD, _LS(IDS_TMWK_NC_MSG81), _T("ico24_load_timehistoryanalysisdata_dynamicnodalloads"));
	InsertTreeItem_str(hTI, WORK_THSL_FLD, _LS(IDS_TM_WORKTREE_Dynamic_Spatial_Load), _T("ico24_load_timehistoryanalysisdata_timevaryingstaticloads"));
	InsertTreeItem_str(hTI, WORK_THMS_FLD, _LS(IDS_TM_Multi_Support_Excitation), _T("ico24_load_timehistoryanalysisdata_multiplesupportexcitation"));
	InsertTreeItem_str(hTI, WORK_THRD_FLD, _LS(IDS_TM_WORKTREE_Time_History_Result_Function), _T("ico24_load_timehistoryanalysisdata_defineresultfunction"));
	InsertTreeItem_str(hTI, WORK_THCR_FLD, _LS(IDS_TM_WORKTREE_Time_History_CunCur_Result_Group), _T("ico24_load_timehistoryanalysisdata_defineresultfunction"));
	InsertTreeItem_str(hTI, WORK_THAT_FLD, _LS(IDS_TM_DEFINE_ANIMATION_TIME), _T("ico24_tools_generator_datagenerator"));
	hTI2 = InsertTreeItem_(hTI, WORK_THSG_FLD, _T("ico24_results_timehistory_timehistorysmartgraph"));
	InsertTreeItem_str(hTI2, WORK_THEF_FLD, _LS(IDS_TM_WORKTREE_THEF_TITLE_NC), _T("ico24_results_timehistory_elementforcegraph"));
	InsertTreeItem_str(hTI2, WORK_THRG_FLD, _LS(IDS_TM_WORKTREE_THRG_TITLE_NC), _T("ico24_results_timehistory_generallinkgraph"));
	InsertTreeItem_str(hTI2, WORK_THRI_FLD, _LS(IDS_TM_WORKTREE_THRI_TITLE_NC), _T("ico24_results_timehistory_inelastichingegraph"));
	InsertTreeItem_str(hTI2, WORK_THRS_FLD, _LS(IDS_TM_WORKTREE_THRS_TITLE_NC), _T("ico24_results_timehistory_seismicdevicesgraph"));
	InsertTreeItem_str(hTI2, WORK_THSF_FLD, _LS(IDS_TM_WORKTREE_THSF_TITLE_NC), _T("ico24_tools_generator_artificialearthquake"));

	//Moving - kr, etc.
	std::vector<int> aCode_kr = { D_MOVE_CODE_AASHTO_STAN, D_MOVE_CODE_AASHTO_LRFD, D_MOVE_CODE_PENDOT, D_MOVE_CODE_TAIWAN, D_MOVE_CODE_KOREA, D_MOVE_CODE_CANADA, D_MOVE_CODE_KOREA_LRFD_2011, D_MOVE_CODE_AUSTRALIA, D_MOVE_CODE_RUSSIA, D_MOVE_CODE_POLAND, D_MOVE_CODE_SOUTH_AFRICA, D_MOVE_CODE_NEWZEALAND, D_MOVE_CODE_BRAZIL };
	hTI = InsertTreeItem_strMV(aCode_kr, m_hRootTi, WORK_MVLDANL_FLD, _LS(IDS_TMWK__MSG53), _T("ico24_analysis_analysiscontrol_movingload"));
	InsertTreeItem_MV(aCode_kr, hTI, WORK_MVCD_FLD, _T("ico24_load_movingloadanalysisdata_movingloadcases"));
	InsertTreeItem_strMV(aCode_kr, hTI, WORK_LLAN_FLD, _LS(IDS_TMWK_NC_MSG94), _T("ico24_load_movingloadanalysisdata_trafficlinelanes"));
	InsertTreeItem_strMV(aCode_kr, hTI, WORK_LLANop_FLD, _LS(IDS_TMWK_NC_MSG197), _T("ico24_load_movingloadanalysisdata_movingloadoptimization"));
	InsertTreeItem_strMV(aCode_kr, hTI, WORK_SLAN_FLD, _LS(IDS_TMWK_NC_MSG96), _T("ico24_load_movingloadanalysisdata_trafficsurfacelanes2"));
	InsertTreeItem_strMV(aCode_kr, hTI, WORK_SLANop_FLD, _LS(IDS_TMWK_NC_MSG199), _T("ico24_load_movingloadanalysisdata_movingloadoptimization2"));
	InsertTreeItem_strMV(aCode_kr, hTI, WORK_SINF_FLD, _LS(IDS_TMWK_NC_MSG98), _T("ico24_load_movingloadanalysisdata_plateelementforinfluencesurface"));
	InsertTreeItem_strMV(aCode_kr, hTI, WORK_MLSP_FLD, _LS(IDS_TMWK_NC_MSG99), _T("ico24_load_movingloadanalysisdata_lanesupportnegamoment"));
	InsertTreeItem_strMV(aCode_kr, hTI, WORK_MLSR_FLD, _LS(IDS_TMWK_NC_MSG99_1), _T("ico24_load_movingloadanalysisdata_lanesupportreaction"));
	InsertTreeItem_strMV(aCode_kr, hTI, WORK_MVHL_FLD, _LS(IDS_TMWK_NC_MSG100), _T("ico24_load_movingloadanalysisdata_vehicles"));
	InsertTreeItem_strMV(aCode_kr, hTI, WORK_MVHC_FLD, _LS(IDS_TMWK_NC_MSG104), _T("ico24_load_movingloadanalysisdata_vehicleclasses"));
	InsertTreeItem_strMV(aCode_kr, hTI, WORK_MVLD_FLD, _LS(IDS_TMWK_NC_MSG106), _T("ico24_load_movingloadanalysisdata_movingloadcases"));
	InsertTreeItem_MV(aCode_kr, hTI, WORK_IMPF_FLD, _T("ico24_load_movingloadanalysisdata_addimpactfactor"));
	InsertTreeItem_MV(aCode_kr, hTI, WORK_DYLA_FLD, _T("ico24_load_movingloadanalysisdata_dynamicloadallowance"));

	//Moving - jp.
	hTI = InsertTreeItem_strMV({ D_MOVE_CODE_JAPAN }, m_hRootTi, WORK_MVLDANLjp_FLD, _LS(IDS_TMWK__MSG53), _T("ico24_analysis_analysiscontrol_movingload"));
	InsertTreeItem_MV({ D_MOVE_CODE_JAPAN }, hTI, WORK_MVCD_FLD, _T("ico24_load_movingloadanalysisdata_movingloadcases"));
	InsertTreeItem_MV({ D_MOVE_CODE_JAPAN }, hTI, WORK_LLANjp_FLD, _T("ico24_load_movingloadanalysisdata_trafficlinelanes"));
	InsertTreeItem_strMV({ D_MOVE_CODE_JAPAN }, hTI, WORK_MVLDjp_FLD, _LS(IDS_TMWK_NC_MSG106), _T("ico24_load_movingloadanalysisdata_movingloadcases"));

	//Moving - ch.
	hTI = InsertTreeItem_strMV({ D_MOVE_CODE_CHINA }, m_hRootTi, WORK_MVLDANLch_FLD, _LS(IDS_TMWK__MSG53), _T("ico24_analysis_analysiscontrol_movingload"));
	InsertTreeItem_MV({ D_MOVE_CODE_CHINA }, hTI, WORK_MVCD_FLD, _T("ico24_load_movingloadanalysisdata_movingloadcases"));
	InsertTreeItem_strMV({ D_MOVE_CODE_CHINA }, hTI, WORK_LLANch_FLD, _LS(IDS_TMWK_NC_MSG94), _T("ico24_load_movingloadanalysisdata_trafficlinelanes"));
	InsertTreeItem_strMV({ D_MOVE_CODE_CHINA }, hTI, WORK_SLANch_FLD, _LS(IDS_TMWK_NC_MSG96), _T("ico24_load_movingloadanalysisdata_trafficsurfacelanes2"));
	InsertTreeItem_strMV({ D_MOVE_CODE_CHINA }, hTI, WORK_LLANop_FLD, _LS(IDS_TMWK_NC_MSG197), _T("ico24_load_movingloadanalysisdata_movingloadoptimization"));
	InsertTreeItem_strMV({ D_MOVE_CODE_CHINA }, hTI, WORK_SLANop_FLD, _LS(IDS_TMWK_NC_MSG199), _T("ico24_load_movingloadanalysisdata_movingloadoptimization2"));
	InsertTreeItem_strMV({ D_MOVE_CODE_CHINA }, hTI, WORK_SINFch_FLD, _LS(IDS_TMWK_NC_MSG98), _T("ico24_load_movingloadanalysisdata_plateelementforinfluencesurface"));
	InsertTreeItem_strMV({ D_MOVE_CODE_CHINA }, hTI, WORK_MVHL_FLD, _LS(IDS_TMWK_NC_MSG100), _T("ico24_load_movingloadanalysisdata_vehicles"));
	InsertTreeItem_strMV({ D_MOVE_CODE_CHINA }, hTI, WORK_MVHC_FLD, _LS(IDS_TMWK_NC_MSG104), _T("ico24_load_movingloadanalysisdata_vehicleclasses"));
	InsertTreeItem_strMV({ D_MOVE_CODE_CHINA }, hTI, WORK_MVLDch_FLD, _LS(IDS_TMWK_NC_MSG106), _T("ico24_load_movingloadanalysisdata_movingloadcases"));

	//Moving - id.
	hTI = InsertTreeItem_strMV({ D_MOVE_CODE_INDIA }, m_hRootTi, WORK_MVLDANLid_FLD, _LS(IDS_TMWK__MSG53), _T("ico24_analysis_analysiscontrol_movingload"));
	InsertTreeItem_MV({ D_MOVE_CODE_INDIA }, hTI, WORK_MVCD_FLD, _T("ico24_load_movingloadanalysisdata_movingloadcases"));
	InsertTreeItem_strMV({ D_MOVE_CODE_INDIA }, hTI, WORK_LLANid_FLD, _LS(IDS_TMWK_NC_MSG94), _T("ico24_load_movingloadanalysisdata_trafficlinelanes"));
	InsertTreeItem_strMV({ D_MOVE_CODE_INDIA }, hTI, WORK_SLAN_FLD, _LS(IDS_TMWK_NC_MSG96), _T("ico24_load_movingloadanalysisdata_trafficsurfacelanes2"));
	InsertTreeItem_strMV({ D_MOVE_CODE_INDIA }, hTI, WORK_LLANop_FLD, _LS(IDS_TMWK_NC_MSG197), _T("ico24_load_movingloadanalysisdata_movingloadoptimization"));
	InsertTreeItem_strMV({ D_MOVE_CODE_INDIA }, hTI, WORK_SLANop_FLD, _LS(IDS_TMWK_NC_MSG199), _T("ico24_load_movingloadanalysisdata_movingloadoptimization2"));
	InsertTreeItem_strMV({ D_MOVE_CODE_INDIA }, hTI, WORK_MVLDid_FLD, _LS(IDS_TMWK_NC_MSG106), _T("ico24_load_movingloadanalysisdata_movingloadcases"));

	//Moving - bs.
	hTI = InsertTreeItem_strMV({ D_MOVE_CODE_BS }, m_hRootTi, WORK_MVLDANL_FLD, _LS(IDS_TMWK__MSG53), _T("ico24_analysis_analysiscontrol_movingload"));
	InsertTreeItem_MV({ D_MOVE_CODE_BS }, hTI, WORK_MVCD_FLD, _T("ico24_load_movingloadanalysisdata_movingloadcases"));
	InsertTreeItem_strMV({ D_MOVE_CODE_BS }, hTI, WORK_LLAN_FLD, _LS(IDS_TMWK_NC_MSG94), _T("ico24_load_movingloadanalysisdata_trafficlinelanes"));
	InsertTreeItem_strMV({ D_MOVE_CODE_BS }, hTI, WORK_SLAN_FLD, _LS(IDS_TMWK_NC_MSG96), _T("ico24_load_movingloadanalysisdata_trafficsurfacelanes2"));
	InsertTreeItem_strMV({ D_MOVE_CODE_BS }, hTI, WORK_LLANop_FLD, _LS(IDS_TMWK_NC_MSG197), _T("ico24_load_movingloadanalysisdata_movingloadoptimization"));
	InsertTreeItem_strMV({ D_MOVE_CODE_BS }, hTI, WORK_SLANop_FLD, _LS(IDS_TMWK_NC_MSG199), _T("ico24_load_movingloadanalysisdata_movingloadoptimization2"));
	InsertTreeItem_strMV({ D_MOVE_CODE_BS }, hTI, WORK_MVHL_FLD, _LS(IDS_TMWK_NC_MSG100), _T("ico24_load_movingloadanalysisdata_vehicles"));
	InsertTreeItem_strMV({ D_MOVE_CODE_BS }, hTI, WORK_MVLDbs_FLD, _LS(IDS_TMWK_NC_MSG106), _T("ico24_load_movingloadanalysisdata_movingloadcases"));

	//Moving - fr.
	hTI = InsertTreeItem_strMV({ D_MOVE_CODE_FRANCE }, m_hRootTi, WORK_MVLDANL_FLD, _LS(IDS_TMWK__MSG53), _T("ico24_analysis_analysiscontrol_movingload"));
	InsertTreeItem_MV({ D_MOVE_CODE_FRANCE }, hTI, WORK_MVCD_FLD, _T("ico24_load_movingloadanalysisdata_movingloadcases"));
	InsertTreeItem_strMV({ D_MOVE_CODE_FRANCE }, hTI, WORK_LLANfr_FLD, _LS(IDS_TMWK_NC_MSG94), _T("ico24_load_movingloadanalysisdata_trafficlinelanes"));
	InsertTreeItem_strMV({ D_MOVE_CODE_FRANCE }, hTI, WORK_MVHL_FLD, _LS(IDS_TMWK_NC_MSG100), _T("ico24_load_movingloadanalysisdata_vehicles"));
	InsertTreeItem_strMV({ D_MOVE_CODE_FRANCE }, hTI, WORK_MVLDfr_FLD, _LS(IDS_TMWK_NC_MSG106), _T("ico24_load_movingloadanalysisdata_movingloadcases"));

	//Moving - Euro.
	hTI = InsertTreeItem_strMV({ D_MOVE_CODE_EURO_BS }, m_hRootTi, WORK_MVLDANL_FLD, _LS(IDS_TMWK__MSG53), _T("ico24_analysis_analysiscontrol_movingload"));
	InsertTreeItem_MV({ D_MOVE_CODE_EURO_BS }, hTI, WORK_MVCD_FLD, _T("ico24_load_movingloadanalysisdata_movingloadcases"));
	InsertTreeItem_strMV({ D_MOVE_CODE_EURO_BS }, hTI, WORK_LLAN_FLD, _LS(IDS_TMWK_NC_MSG94), _T("ico24_load_movingloadanalysisdata_trafficlinelanes"));
	InsertTreeItem_strMV({ D_MOVE_CODE_EURO_BS }, hTI, WORK_SLAN_FLD, _LS(IDS_TMWK_NC_MSG96), _T("ico24_load_movingloadanalysisdata_trafficsurfacelanes2"));
	InsertTreeItem_strMV({ D_MOVE_CODE_EURO_BS }, hTI, WORK_LLANop_FLD, _LS(IDS_TMWK_NC_MSG197), _T("ico24_load_movingloadanalysisdata_movingloadoptimization"));
	InsertTreeItem_strMV({ D_MOVE_CODE_EURO_BS }, hTI, WORK_SLANop_FLD, _LS(IDS_TMWK_NC_MSG199), _T("ico24_load_movingloadanalysisdata_movingloadoptimization2"));
	InsertTreeItem_strMV({ D_MOVE_CODE_EURO_BS }, hTI, WORK_MVHL_FLD, _LS(IDS_TMWK_NC_MSG100), _T("ico24_load_movingloadanalysisdata_vehicles"));
	InsertTreeItem_strMV({ D_MOVE_CODE_EURO_BS }, hTI, WORK_MVLD_FLD, _LS(IDS_TMWK_NC_MSG106), _T("ico24_load_movingloadanalysisdata_movingloadcases"));
	InsertTreeItem_MV({ D_MOVE_CODE_EURO_BS }, hTI, WORK_DYFG_FLD, _T("ico24_load_movingloadanalysisdata_railwaydynamicfactor"));
	InsertTreeItem_MV({ D_MOVE_CODE_EURO_BS }, hTI, WORK_DYNF_FLD, _T("ico24_load_movingloadanalysisdata_railwaydynamicfactor"));

	//Moving - Tran.
	hTI = InsertTreeItem_strMV({ D_MOVE_CODE_TRANS }, m_hRootTi, WORK_MVLDANLtr_FLD, _LS(IDS_TMWK__MSG53), _T("ico24_analysis_analysiscontrol_movingload"));
	InsertTreeItem_MV({ D_MOVE_CODE_TRANS }, hTI, WORK_MVCD_FLD, _T("ico24_load_movingloadanalysisdata_movingloadcases"));
	InsertTreeItem_strMV({ D_MOVE_CODE_TRANS }, hTI, WORK_LLANtr_FLD, _LS(IDS_TMWK_NC_MSG94), _T("ico24_load_movingloadanalysisdata_trafficlinelanes"));
	InsertTreeItem_strMV({ D_MOVE_CODE_TRANS }, hTI, WORK_MVHLtr_FLD, _LS(IDS_TMWK_NC_MSG100), _T("ico24_load_movingloadanalysisdata_vehicles"));
	InsertTreeItem_strMV({ D_MOVE_CODE_TRANS }, hTI, WORK_MVLDtr_FLD, _LS(IDS_TMWK_NC_MSG106), _T("ico24_load_movingloadanalysisdata_movingloadcases"));

	//Moving - NewZealand
	InsertTreeItem_strMV({ D_MOVE_CODE_NEWZEALAND }, hTI, WORK_DYNZ_FLD, _LS(IDS_TMWK__DYNZ), _T("ico24_load_movingloadanalysisdata_dynamicloadallowance"));

	//Moving - BS
	InsertTreeItem_strMV({ D_MOVE_CODE_BS }, hTI, WORK_DYBS_FLD, _LS(IDS_TMWK__DYBS), _T("ico24_load_movingloadanalysisdata_railwaydynamicfactor"));
	InsertTreeItem_strMV({ D_MOVE_CODE_BS }, hTI, WORK_DSFC_FLD, _LS(IDS_TMWK__DSFC), _T("ico24_load_movingloadanalysisdata_dynamicloadallowance"));

	//Moving - Brazil
	InsertTreeItem_strMV({ D_MOVE_CODE_BRAZIL }, hTI, WORK_IFBZ_FLD, _LS(IDS_TMWK__IFBZ), _T("ico24_load_movingloadanalysisdata_dynamicloadallowance"));

	//Crane Load
	hTI = InsertTreeItem_str(m_hRootTi, WORK_CRANELOAD_FLD, _LS(IDS_TREE_CRAN), _T("ico24_analysis_analysiscontrol_movingload"));
	InsertTreeItem_(hTI, WORK_CRAN_FLD, _T("ico24_load_movingloadanalysisdata_vehicles"));
	hTI2 = InsertTreeItem_str(hTI, WORK_NCRN_FLD, _LS(IDS_TREE_NCRN_WORK1), _T("ico24_load_movingloadanalysisdata_movingloadcases"));
	InsertTreeItem_(hTI2, WORK_NCRN_LST, _T("ico24_load_movingloadanalysisdata_movingloadcases"));

	//Settlement
	hTI = InsertTreeItem_str(m_hRootTi, WORK_STTL_FLD, _LS(IDS_TMWK__MSG54), _T("ico24_load_settlementanalysisdata_settlementgroup"));
	InsertTreeItem_str(hTI, WORK_SMPT_FLD, _LS(IDS_TMWK_NC_MSG108), _T("ico24_load_settlementanalysisdata_settlementgroup"));
	InsertTreeItem_str(hTI, WORK_SMLC_FLD, _LS(IDS_TMWK_NC_MSG110), _T("ico24_load_settlementanalysisdata_settlementloadcase"));

	//Concurrent Reaction
	hTI = InsertTreeItem_str(m_hRootTi, WORK_CRGR_FLD, _LS(IDS_TMWK__MSG2114), _T("ico24_load_settlementanalysisdata_concurrentreactiongroup"));

	//Concurrent Joint Force
	hTI = InsertTreeItem_str(m_hRootTi, WORK_CJFG_FLD, _LS(IDS_TMWK__MSG2116), _T("ico24_load_settlementanalysisdata_concurrentjointforcegroup"));

	//Composite Bridge
	hTI = InsertTreeItem_str(m_hRootTi, WORK_PLCB_FLD, _LS(IDS_TMWK__MSG56), _T("ico24_load_misc_precompositesection"));

	//Hydration
	hTI = InsertTreeItem_str(m_hRootTi, WORK_HYDR_FLD, _LS(IDS_TMWK__MSG57), _T("ico24_load_heatofhydrationanalysisdata_ambienttemperaturefunctions"));
	InsertTreeItem_(hTI, WORK_HMTP_FLD, _T("ico24_load_heatofhydrationanalysisdata_convectioncoefficientfunctions"));
	InsertTreeItem_str(hTI, WORK_HMAT_FLD, _LS(IDS_TMWK_NC_MSG121), _T("ico24_load_heatofhydrationanalysisdata_convectioncoefficientfunctions"));
	InsertTreeItem_str(hTI, WORK_ETFC_FLD, _LS(IDS_TMWK_NC_MSG123), _T("ico24_load_heatofhydrationanalysisdata_ambienttemperaturefunctions"));
	InsertTreeItem_str(hTI, WORK_CCFC_FLD, _LS(IDS_TMWK_NC_MSG126), _T("ico24_load_heatofhydrationanalysisdata_convectioncoefficientfunctions"));
	InsertTreeItem_str(hTI, WORK_HECB_FLD, _LS(IDS_TMWK_NC_MSG128), _T("ico24_load_heatofhydrationanalysisdata_elementconvectionboundary"));
	InsertTreeItem_str(hTI, WORK_HSPT_FLD, _LS(IDS_TMWK_NC_MSG130), _T("ico24_load_heatofhydrationanalysisdata_prescribedtemp"));
	InsertTreeItem_str(hTI, WORK_HSFC_FLD, _LS(IDS_TMWK_NC_MSG132), _T("ico24_load_heatofhydrationanalysisdata_heatsource"));
	InsertTreeItem_str(hTI, WORK_HPCE_FLD, _LS(IDS_TMWK_NC_MSG134), _T("ico24_load_heatofhydrationanalysisdata_pipecooling"));

	//Stage
	hTI = InsertTreeItem_str(m_hRootTi, WORK_STAG_FLD, _LS(IDS_TMWK_NC_MSG172), _T("ico24_load_constructionstagedata_definecs"));

	//Grid Model
	hTI = InsertTreeItem_str(m_hRootTi, WORK_GRID_FLD, _LS(IDS_TMWK_Grid_Analysis_Data), _T("ico24_loads_Grid_jpn5"));
	InsertTreeItem_str(hTI, WORK_GACD_FLD, _LS(IDS_TMWK_Grid_Analysis_Control_Data_d), _T("ico24_loads_Grid_jpn1"));
	InsertTreeItem_str(hTI, WORK_MGDR_FLD, _LS(IDS_TMWK_NC_GRID_MAIN_GIRDER_d), _T("ico24_loads_Grid_jpn2"));
	InsertTreeItem_str(hTI, WORK_CBEM_FLD, _LS(IDS_TMWK_NC_GRID_CROSS_BEAM_d), _T("ico24_loads_Grid_jpn4"));
	InsertTreeItem_str(hTI, WORK_LDLN_FLD, _LS(IDS_TMWK_NC_GRID_LOAD_LINE_d), _T("ico24_loads_Grid_jpn3"));
	InsertTreeItem_str(hTI, WORK_GILC_FLD, _LS(IDS_TMWK_NC_GRID_ANALYSIS_LOAD_d), _T("ico24_loads_Grid_jpn6"));
	InsertTreeItem_(hTI, WORK_BRGT_FLD, _T("ico24_loads_Grid_jpn7"));
	InsertTreeItem_(hTI, WORK_BSPN_LST, _T("ico24_loads_Grid_jpn8"));
	InsertTreeItem_str(hTI, WORK_GSTY_FLD, _LS(IDS_TM_NC_WORKTREE_Element_Structure_Type_d), _T("ico24_loads_Grid_jpn9"));
	InsertTreeItem_str(hTI, WORK_GIMP_FLD, _LS(IDS_TMWK_NC_GRID_IMPACT_FACTOR_d), _T("ico24_loads_Grid_jpn10"));
	InsertTreeItem_str(hTI, WORK_CREF_FLD, _LS(IDS_TMWK_NC_GRID_CROSS_REF_d), _T("ico24_loads_Grid_jpn16"));
	InsertTreeItem_str(hTI, WORK_P1LT_FLD, _LS(IDS_TM_NC_WORKTREE_P1_Load_Factor_Type_d), _T("ico24_loads_Grid_jpn11"));
	InsertTreeItem_str(hTI, WORK_P1LA_FLD, _LS(IDS_TM_NC_WORKTREE_P1_Load_Factor_d), _T("ico24_loads_Grid_jpn11"));
	InsertTreeItem_str(hTI, WORK_LNKF_FLD, _LS(IDS_TM_NC_WORKTREE_Impact_Factor_d), _T("ico24_loads_Grid_jpn12"));
	InsertTreeItem_str(hTI, WORK_GMAS_FLD, _LS(IDS_TMWK_NC_GRID_NODAL_MASS), _T("ico24_loads_Grid_jpn14"));

	//Pushover
	hTI = InsertTreeItem_str(m_hRootTi, WORK_PUSH_FLD, _LS(IDS_TMWK_Pushover_Analysis), _T("ico24_pushover_properties_assignpushoverhingeproperties"));
	InsertTreeItem_(hTI, WORK_POGD_FLD, _T("ico24_pushover_control_globalcontrol"));
	InsertTreeItem_str(hTI, WORK_PO_MEMBASSIG_FLD, _LS(IDS_TREE_TEXT_GEN_MEMBER), _T("ico24_mods_design_memberassignment"));
	InsertTreeItem_str(hTI, WORK_POLD_FLD, _LS(IDS_TMWK_NC_PUSHOVER_LOADCASE_d), _T("ico24_pushover_loadcase_loadcase"));
	InsertTreeItem_str(hTI, WORK_POFC_FLD, _LS(IDS_TMWK_NC_PUSHOVER_FUNCTION_d), _T("ico24_pushover_pushoverresults_pushovercurve"));
	InsertTreeItem_str(hTI, WORK_HNGT_FLD, _LS(IDS_TMWK_NC_PUSHOVER_DEFINE_PO_HINGE_PROP_d), _T("ico24_pushover_properties_definepushoverhingeproperties"));
	InsertTreeItem_str(hTI, WORK_HNGE_FLD, _LS(IDS_TMWK_NC_PUSHOVER_ASSIGN_PO_HINGE_PROP_d), _T("ico24_pushover_properties_assignpushoverhingeproperties"));
	InsertTreeItem_str(hTI, WORK_FIMP_PO_FLD, _LS(IDS_TM_NC_WORKTREE_Fiber_Material_Property_d_PO), _T("ico24_properties_inelproperties_inelasticmaterialproperties"));
	InsertTreeItem_str(hTI, WORK_FIBR_PO_FLD, _LS(IDS_TM_NC_WORKTREE_Fiber_Division_Section_d_PO), _T("ico24_properties_inelproperties_fiberdivisionofsection"));
	InsertTreeItem_str(hTI, WORK_FIBW_PO_FLD, _LS(IDS_TM_NC_WORKTREE_Fiber_Division_Section_Wall_d_PO), _T("ico24_properties_inelproperties_fiberdivisionofsection"));
	InsertTreeItem_str(hTI, WORK_PORD_FLD, _LS(IDS_TM_NC_WORKTREE_Pushover_Result_Function_d), _T("ico24_pushover_pushoverresults_pushovercurve"));
	hTI2 = InsertTreeItem_(hTI, NULL, _T("ico24_pushover_pushoverresults_pushovergraph"));
	InsertTreeItem_str(hTI2, WORK_POEF_FLD, _LS(IDS_TM_WORKTREE_THEF_TITLE), _T("ico24_results_timehistory_elementforcegraph"));
	InsertTreeItem_str(hTI2, WORK_PORG_FLD, _LS(IDS_TM_WORKTREE_THRG_TITLE), _T("ico24_results_timehistory_generallinkgraph"));
	InsertTreeItem_str(hTI2, WORK_PORE_FLD, _LS(IDS_TM_WORKTREE_THRE_TITLE), _T("ico24_results_timehistory_generallinkgraph"));
	InsertTreeItem_str(hTI2, WORK_PORI_FLD, _LS(IDS_TM_WORKTREE_PORI_TITLE), _T("ico24_results_timehistory_inelastichingegraph"));

	//Rebar
	hTI = InsertTreeItem_str(m_hRootTi, WORK_REBD_FLD, _LS(IDS_TM_REBAR_DATA), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI, WORK_REBB_FLD, _LS(IDS_WG_TREEMENU_Beam), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI, WORK_REBC_FLD, _LS(IDS_TM_Column), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI, WORK_REBR_FLD, _LS(IDS_TM_BRACE), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI, WORK_REBW_FLD, _LS(IDS_WG_TREEMENU_Wall), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	hTI2 = InsertTreeItem_str(hTI, WORK_RBMS_FLD, _LS(IDS_TM_MESHED_SLAB), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI2, WORK_RBMS_T1_FLD, _LS(IDS_TM_MESHED_SLAB_TOP_DIR1), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI2, WORK_RBMS_B1_FLD, _LS(IDS_TM_MESHED_SLAB_BOT_DIR1), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI2, WORK_RBMS_T2_FLD, _LS(IDS_TM_MESHED_SLAB_TOP_DIR2), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI2, WORK_RBMS_B2_FLD, _LS(IDS_TM_MESHED_SLAB_BOT_DIR2), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI, WORK_RBMW_FLD, _LS(IDS_TM_MESHED_WALL), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	hTI2 = InsertTreeItem_str(hTI, WORK_RBST_FLD, _LS(IDS_TM_STRIP), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI2, WORK_RBST_T_FLD, _LS(IDS_TM_STRIP_TOP), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI2, WORK_RBST_B_FLD, _LS(IDS_TM_STRIP_BOT), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	hTI2 = InsertTreeItem_str(hTI, WORK_RBSL_FLD, _LS(IDS_TM_Shell), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI2, WORK_RBSL_T1_FLD, _LS(IDS_TM_MESHED_SLAB_TOP_DIR1), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI2, WORK_RBSL_B1_FLD, _LS(IDS_TM_MESHED_SLAB_BOT_DIR1), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI2, WORK_RBSL_T2_FLD, _LS(IDS_TM_MESHED_SLAB_TOP_DIR2), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI2, WORK_RBSL_B2_FLD, _LS(IDS_TM_MESHED_SLAB_BOT_DIR2), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI, WORK_RIPB_FLD, _LS(IDS_WG_TREEMENU_Beam), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));
	InsertTreeItem_str(hTI, WORK_RIPC_FLD, _LS(IDS_WG_TREEMENU_Column), _T("ico24_mods_design_dropmenu2_columnsectiondataforchecking"));

	//Domain
	hTI = InsertTreeItem_str(m_hRootTi, WORK_DOMN_FLD, _LS(IDS_TMWK_NC_MSG192), _T("ico24_nodeelement_mesh_definedomain"));

	//EBeam
	hTI = InsertTreeItem_str(m_hRootTi, WORK_PREB_FLD, _LS(IDS_TMWK_NC_PREB), _T("ico24_mods_design_dropmenu2_limitingmaximumrebarratio"));

	//Strip
	hTI = InsertTreeItem_str(m_hRootTi, WORK_PRCS_FLD, _LS(IDS_TMWK_NC_PRCS), _T("ico24_rating_bridgeratingdesign_dropmenu1_webpanelassignment"));

	//Drop Panel
	hTI = InsertTreeItem_str(m_hRootTi, WORK_DPAN_FLD, _LS(IDS_TMWK_NC_DPAN), _T("ico24_structure_wizard_spaninformation"));

	//Column Capital
	hTI = InsertTreeItem_str(m_hRootTi, WORK_CCPT_FLD, _LS(IDS_TMWK_NC_CCPT), _T("ico24_seismicperform_evaluationtype_piereval"));

	//Infill Struct
	hTI = InsertTreeItem_str(m_hRootTi, WORK_MSIS_FLD, _LS(IDS_TMWK_NC_MSIS), _T("ico24_mods_mesh_automesh"));

	//Fire Steel
	hTI = InsertTreeItem_str(m_hRootTi, WORK_FIRE_FLD, _LS(IDS_TREE_FIRE), _T("ico24_mods_design_dropmenu1_loadcombinationtype"));
	InsertTreeItem_str(hTI, WORK_FLUE_FLD, _LS(IDS_TM_WORKTREE_Fire_Flue____d_NC), _T("ico24_mods_design_dropmenu1_loadcombinationtype"));
	InsertTreeItem_str(hTI, WORK_STTF_FLD, _LS(IDS_TM_WORKTREE_Fire_Sttf____d_NC), _T("ico24_mods_design_dropmenu1_loadcombinationtype"));
	InsertTreeItem_str(hTI, WORK_FIPA_FLD, _LS(IDS_WG_TREEMENU_Fire_Parameter), _T("ico24_mods_design_dropmenu1_loadcombinationtype"));
	InsertTreeItem_str(hTI, WORK_FSSF_FLD, _LS(IDS_WG_TREEMENU_Fire_Sect_Shape_Coe), _T("ico24_mods_design_dropmenu1_loadcombinationtype"));

	for (std::pair<int, HTREEITEM> pair : m_mFld2TreeItem)
		SetItemVisible(pair.second, FALSE);

	for (std::pair<int, std::map<int, HTREEITEM>> pair : m_mMvcd2FldTreeItem)
		for (std::pair<int, HTREEITEM> pair2 : pair.second)
			SetItemVisible(pair2.second, FALSE);
}


void CTreeWorkCtrlNew::InsertTreeItemByLSTKey(HTREEITEM parent, int nLST, UINT Key, CString str, CString svg)
{
	HTREEITEM hInsertAfter = NULL;
	if (m_mLst2KeyTreeItem.find(nLST) == m_mLst2KeyTreeItem.end())
	{
		m_mLst2KeyTreeItem[nLST] = std::map<UINT, HTREEITEM>();
		m_mLst2TreeItemKey[nLST] = std::map<HTREEITEM, UINT>();
	}

	std::map<UINT, HTREEITEM>& mKey2TreeItem = m_mLst2KeyTreeItem[nLST];
	std::map<UINT, HTREEITEM>::iterator itr;
	if ((itr = mKey2TreeItem.find(Key)) != mKey2TreeItem.end()) // 이미 해당 LST - Key 에 들어간 아이템이 있는 경우
	{
		HTREEITEM hTI = itr->second;
		SetItemText(hTI, str); // 텍스트만 modify
		//TODO : 설마 아이콘이 바뀌는 경우는.. 없겠지..????
	}
	else
	{
		// 일단 키를 집어넣어서 위치를 잡고
		mKey2TreeItem[Key] = HTREEITEM();

		// find 로 iterator를 가져온 다음
		itr = mKey2TreeItem.find(Key);

		// iterator가 시작이면 TVI_FIRST, 한칸 뒤가 end면 TVI_LAST
		// 둘 다 아니면 들어간 애에 바로 앞 iterator에서 hInsertAfter를 가져옴
		if (itr == mKey2TreeItem.begin())
			hInsertAfter = TVI_FIRST;
		else
		{
			if (std::next(itr) == mKey2TreeItem.end())
				hInsertAfter = TVI_LAST;
			else
				hInsertAfter = std::prev(itr)->second;
		}

		HTREEITEM hTI = InsertTreeItem(parent, nLST, str, svg, hInsertAfter);

		mKey2TreeItem[Key] = hTI;
		m_mLst2TreeItemKey[nLST][hTI] = Key;
	}
}

void CTreeWorkCtrlNew::DeleteTreeItemByLSTKey(int nLST, UINT Key)
{
	if (m_mLst2KeyTreeItem.find(nLST) == m_mLst2KeyTreeItem.end())
		return;

	std::map<UINT, HTREEITEM>& mKey2TreeItem = m_mLst2KeyTreeItem[nLST];
	std::map<HTREEITEM, UINT>& mTreeItem2Key = m_mLst2TreeItemKey[nLST];

	if (mKey2TreeItem.find(Key) == mKey2TreeItem.end())
		return;

	HTREEITEM hTI = mKey2TreeItem[Key];
	mKey2TreeItem.erase(Key);
	mTreeItem2Key.erase(hTI);

	DeleteItem(hTI);
}

HTREEITEM CTreeWorkCtrlNew::GetTreeItemByLSTKey(int nLST, UINT Key)
{
	if (m_mLst2KeyTreeItem.find(nLST) == m_mLst2KeyTreeItem.end())
		return NULL;

	std::map<UINT, HTREEITEM>& mKey2TreeItem = m_mLst2KeyTreeItem[nLST];
	if (mKey2TreeItem.find(Key) == mKey2TreeItem.end())
		return NULL;

	return mKey2TreeItem[Key];
}

void CTreeWorkCtrlNew::SetNode(int nCmd, UINT Key)
{
	CArray<T_NODE_K, T_NODE_K> aNode;
	m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNode);

	std::map<int, HTREEITEM>::iterator itr;
	if ((itr = m_mFld2TreeItem.find(WORK_NODE_FLD)) == m_mFld2TreeItem.end())
	{
		ASSERT(0);
		return;
	}

	HTREEITEM hTI = itr->second;
	int nNodeCount = aNode.GetSize();

	if (nNodeCount == 0)
	{
		SetItemVisible(hTI, FALSE);
	}
	else
	{
		SetItemVisible(hTI, TRUE);
		SetItemDisplayCount(hTI, nNodeCount);
	}
}

void CTreeWorkCtrlNew::SetElem(int nCmd, UINT Key)
{
	CArray<T_ELEM_K, T_ELEM_K> aElem;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(aElem);

	std::map<int, HTREEITEM>::iterator itr;
	if ((itr = m_mFld2TreeItem.find(WORK_ELEM_FLD)) == m_mFld2TreeItem.end())
	{
		ASSERT(0);
		return;
	}

	std::map<int, int> mELTyp2LST;
	mELTyp2LST[TRUSS_EL] = WORK_TRUSS_LST;
	mELTyp2LST[TRTENS_EL] = WORK_TRTENS_LST;
	mELTyp2LST[TRCOMP_EL] = WORK_TRCOMP_LST;
	mELTyp2LST[BEAM_EL] = WORK_BEAM_LST;
	mELTyp2LST[PLATE_EL] = WORK_PLATE_LST;
	mELTyp2LST[PLSTRS_EL] = WORK_PLSTRS_LST;
	mELTyp2LST[PLSTRN_EL] = WORK_PLSTRN_LST;
	mELTyp2LST[AXISYM_EL] = WORK_AXISYM_LST;
	mELTyp2LST[SOLID_EL] = WORK_SOLID_LST;
	mELTyp2LST[WALL_EL] = WORK_WALL_LST;
	mELTyp2LST[WALLOPEN_EL] = WORK_WALLOPEN_LST;

	std::map<int, int> mELTyp2Cnt;
	mELTyp2Cnt[TRUSS_EL] = 0;
	mELTyp2Cnt[TRTENS_EL] = 0;
	mELTyp2Cnt[TRCOMP_EL] = 0;
	mELTyp2Cnt[BEAM_EL] = 0;
	mELTyp2Cnt[PLATE_EL] = 0;
	mELTyp2Cnt[PLSTRS_EL] = 0;
	mELTyp2Cnt[PLSTRN_EL] = 0;
	mELTyp2Cnt[AXISYM_EL] = 0;
	mELTyp2Cnt[SOLID_EL] = 0;
	mELTyp2Cnt[WALL_EL] = 0;
	mELTyp2Cnt[WALLOPEN_EL] = 0;

	for (int i = 0; i < aElem.GetSize(); i++)
	{
		T_ELEM_K ElemK = aElem[i];
		T_ELEM_D ElemD;
		m_pDoc->m_pAttrCtrl->GetElem(aElem[i], ElemD);

		if (mELTyp2Cnt.find(ElemD.eltyp) == mELTyp2Cnt.end())
			continue;
		mELTyp2Cnt[ElemD.eltyp]++;
	}

	for (std::pair<int, int> pairELTyp2LST : mELTyp2LST)
	{
		std::map<int, HTREEITEM>::iterator itr2;
		if ((itr2 = m_mFld2TreeItem.find(pairELTyp2LST.second)) == m_mFld2TreeItem.end())
		{
			ASSERT(0);
			continue;
		}

		HTREEITEM hTI = itr2->second;

		int nElemCount = mELTyp2Cnt[pairELTyp2LST.first];
		if (nElemCount == 0)
		{
			SetItemVisible(hTI, FALSE);
		}
		else
		{
			SetItemVisible(hTI, TRUE);
			SetItemDisplayCount(hTI, nElemCount);
		}
	}
}

void CTreeWorkCtrlNew::SetSect(int nCmd, UINT Key)
{
	T_SECT_UDRD_D data_ur;
	m_pDoc->m_vbuf->m_sect.Get(Key, data_ur);

	T_SECT_K SectK = data_ur.key;

	std::map<int, HTREEITEM>::iterator itr;
	if ((itr = m_mFld2TreeItem.find(WORK_ELEM_FLD)) == m_mFld2TreeItem.end())
	{
		ASSERT(0);
		return;
	}

	int nLST = WORK_SECT_LST;
	HTREEITEM hTIParent = itr->second;
	switch (nCmd)
	{
	case UR_SECT_ADD:
	case UR_SECT_MFS:
	case UR_SECT_MFD:
	{
		T_SECT_D SectD;
		if (!m_pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
			return;

		CString tStr;
		tStr.Format(_LS(IDS_TMWK__MSG61), SectK, SectD.SName);
		InsertTreeItemByLSTKey(hTIParent, nLST, SectK, tStr, _T("ico24_properties_section_sectionproperties"));
		break;
	}
	case UR_SECT_DEL:
	{
		DeleteTreeItemByLSTKey(nLST, SectK);
		break;
	}
	default:
		ASSERT(0);
		break;
	}
}

int CTreeWorkCtrlNew::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (MBaseTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	AddAllSVGImages(CString(_T("")));
	EnableMultiSelect();
	EndInsertTreeItem();
	InitTree();
	
	return 0;
}

void CTreeWorkCtrlNew::OnDestroy()
{
}

void CTreeWorkCtrlNew::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
}

void CTreeWorkCtrlNew::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu     menu;
	CMenu* pPopup;
	HMENU			hMenu;

	INIResourceManager::LoadMenuToINITranslate(hMenu, IDR_WORKTREE_CONTEXT, _T("wg_treemenu"), &menu);

	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if (selItemNum == 0) return;
	UINT Data = GetItemData(aItem[0]);

	auto TrackPopupMenu = [this, &pPopup, point]()
	{
		if (g_pContextMenuManager != nullptr)
		{
			int nMenuID = g_pContextMenuManager->TrackPopupMenu(pPopup->GetSafeHmenu(), point.x, point.y, this);
			PostMessage(WM_COMMAND, MAKEWPARAM(nMenuID, 0), 0);
		}
		else
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	};

	switch (Data)
	{
	case WORK_NODE_FLD:
		pPopup = menu.GetSubMenu(0);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_TRUSS_LST:
	case WORK_TRTENS_LST:
	case WORK_TRCOMP_LST:
	case WORK_BEAM_LST:
	case WORK_PLATE_LST:
	case WORK_PLSTRS_LST:
	case WORK_PLSTRN_LST:
	case WORK_AXISYM_LST:
	case WORK_SOLID_LST:
	case WORK_WALL_LST:
	case WORK_WALLOPEN_LST:
		pPopup = menu.GetSubMenu(0);
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(11, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_TABLES, MF_BYCOMMAND);  // Table을 지움 
		TrackPopupMenu();
		break;
	case WORK_SPAN_LST:
	case WORK_SPAN_COMPOSTIE_LST:
	case WORK_SPAN_COMPOSTIE_VB_LST:
	case WORK_SPAN_STEELGIRDER_LST:
	case WORK_NPLN_LST:
	case WORK_HNGT_LST:
	case WORK_CRGR_LST:
	case WORK_CJFG_LST:
		pPopup = menu.GetSubMenu(1);
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(1, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_ASSIGN, MF_BYCOMMAND); // Assign을 지움
		TrackPopupMenu();
		break;
	case WORK_PROP_FLD:
		pPopup = menu.GetSubMenu(2);
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(12, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTY_REBAR, MF_BYCOMMAND);
		pPopup->DeleteMenu(10, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_DELETE_UNUS_MATL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_DELETE_UNUS_SECT, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_DELETE_UNUS_THIK, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_TABLES, MF_BYCOMMAND);  // Table을 지움 
		TrackPopupMenu();
		break;
	case WORK_MATL_FLD:
		pPopup = menu.GetSubMenu(2);
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(12, MF_BYPOSITION);
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_EXPANDALL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_1LEVEL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_2LEVEL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTY_REBAR, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_ADDSECT, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_ADDTHIK, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_DELETE_UNUS_SECT, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_DELETE_UNUS_THIK, MF_BYCOMMAND);
		TrackPopupMenu();
		break;
	case WORK_SECT_FLD:
		pPopup = menu.GetSubMenu(2);
		VERIFY(pPopup != NULL);
#if defined(_MGEN) // mylee 20090731 - Gen 일 경우 Rebar 메뉴 지워준다 770 부터 적용 (김영민)
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTY_REBAR, MF_BYCOMMAND);
		pPopup->DeleteMenu(10, MF_BYPOSITION);
#endif
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_EXPANDALL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_1LEVEL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_2LEVEL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_ADDMATL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_ADDTHIK, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_DELETE_UNUS_MATL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_DELETE_UNUS_THIK, MF_BYCOMMAND);
		TrackPopupMenu();
		break;
	case WORK_THIK_FLD:
		pPopup = menu.GetSubMenu(2);
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(12, MF_BYPOSITION);
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_EXPANDALL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_1LEVEL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_2LEVEL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTY_REBAR, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_ADDMATL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_ADDSECT, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_DELETE_UNUS_MATL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_DELETE_UNUS_SECT, MF_BYCOMMAND);
		TrackPopupMenu();
		break;
	case WORK_CSCS_FLD:
		pPopup = menu.GetSubMenu(12);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_GRUP_LST:
		pPopup = menu.GetSubMenu(1);
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(13, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND); // properties를 지움
		TrackPopupMenu();
		break;
	case WORK_STOR_FLD:
		pPopup = menu.GetSubMenu(6);  // Property만 보이게
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		//pPopup->DeleteMenu(ID_WORKTREE_DISPLAY,MF_BYCOMMAND);    // Diplay를 지움
		//pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR,MF_BYCOMMAND);  // Undisplay를 지움
		pPopup->DeleteMenu(ID_WORKTREE_TABLES, MF_BYCOMMAND);     // tables을 지움
		TrackPopupMenu();
		break;
	case WORK_STOR_LST:
		pPopup = menu.GetSubMenu(1);
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(1, MF_BYPOSITION);      // 구분선 지움
		pPopup->DeleteMenu(ID_WORKTREE_ASSIGN, MF_BYCOMMAND); // Assign을 지움
		TrackPopupMenu();
		break;
	case WORK_MATL_LST:
	case WORK_SECT_LST:
	case WORK_THIK_LST:
	case WORK_ESSF_LST:
	case WORK_WSSF_LST:
	case WORK_PSSF_LST:
	case WORK_CSCS_LST:
	case WORK_REBB_LST:
	case WORK_REBC_LST:
	case WORK_REBR_LST:
	case WORK_REBW_LST:
	case WORK_RBMS_LST:
	case WORK_RBSL_LST:
	case WORK_RBMW_LST:
	case WORK_RBST_LST:
	case WORK_RIPB_LST:
	case WORK_RIPC_LST:
		pPopup = menu.GetSubMenu(1);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;

	case WORK_SBDO_LST:
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		BOOL bApproval = FALSE;
		pPopup = menu.GetSubMenu(1);
		VERIFY(pPopup != NULL);
#if defined(_JP)
		bApproval = FALSE;
#elif defined(_ORG)
		if (pDoc->AllowCommand(D_OPTN_ID_SLAB_STRIP)) bApproval = TRUE;
#elif defined(_CH)
		if (pDoc->AllowCommand(D_OPTN_ID_SLAB_STRIP)) bApproval = TRUE;
#else
		if (pDoc->AllowCommand(D_OPTN_ID_SL_WA_DGN))  bApproval = TRUE;
#endif
		if (!bApproval)
		{
			//pPopup->DeleteMenu(ID_WORKTREE_PROPERTIES,MF_BYCOMMAND); 
			pPopup->DeleteMenu(13, MF_BYPOSITION);
		}
		TrackPopupMenu();
		break;
	}

	case WORK_DOMN_FLD:
		pPopup = menu.GetSubMenu(19);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;

	case WORK_MADO_LST:
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		BOOL bApproval = FALSE;
#if defined(_JP)
		bApproval = FALSE;
#elif defined(_ORG)
		if (pDoc->AllowCommand(D_OPTN_ID_AUTOMESH)) bApproval = TRUE;
#else
		if (pDoc->AllowCommand(D_OPTN_ID_AUTOMESH))  bApproval = TRUE;
#endif
		if (bApproval) pPopup = menu.GetSubMenu(20);
		else          pPopup = menu.GetSubMenu(1);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	}
	case WORK_GSTY_LST:
	case WORK_GIMP_LST:
	case WORK_P1LA_LST:
		pPopup = menu.GetSubMenu(1);
		VERIFY(pPopup != NULL);
		pPopup->InsertMenu(14, MF_BYPOSITION, ID_WORKTREE_DISAPPEAR, _LS(IDS_TM_WORKTREE_Undisplay));
		pPopup->InsertMenu(14, MF_BYPOSITION, ID_WORKTREE_DISPLAY, _LS(IDS_TM_WORKTREE_Display));
		TrackPopupMenu();
		break;
	case WORK_CONS_LST:
	case WORK_NSPR_LIN_LST:
	case WORK_NSPR_NON_LST:
	case WORK_NSPR_MLT_LST:
	case WORK_GSTP_LST:
	case WORK_DRLS_FLD:
	case WORK_SKEW_LST:
	case WORK_NMAS_LST:
	case WORK_THNL_LST:
	case WORK_SMPT_LST:
		pPopup = menu.GetSubMenu(3); // select assinged node
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_CLDR_LST:
		pPopup = menu.GetSubMenu(3);  // select assigned element
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(1, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_ASSIGN, MF_BYCOMMAND); // Assign을 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISPLAY, MF_BYCOMMAND); // Display를 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR, MF_BYCOMMAND); // Disappear를 지움
		TrackPopupMenu();
		break;
	case WORK_SSPS_LIN_LST:
	case WORK_SSPS_NON_LST:
		pPopup = menu.GetSubMenu(4); // Select Assinged Element
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(1, MF_BYPOSITION);
		pPopup->DeleteMenu(5, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_ASSIGN, MF_BYCOMMAND); // Assign을 지움 
		pPopup->DeleteMenu(ID_WORKTREE_SELECT, MF_BYCOMMAND); // Select Assigned Element을 지움
		pPopup->DeleteMenu(ID_WORKTREE_SELECTPLUS, MF_BYCOMMAND); // Select Assigned Element Plus을 지움
		pPopup->DeleteMenu(ID_WORKTREE_UNSELECT, MF_BYCOMMAND); // Unselect Assigned Element을 지움
		pPopup->DeleteMenu(ID_WORKTREE_UNSELECTALL, MF_BYCOMMAND); // Unselect All을 지움
		TrackPopupMenu();
		break;
	case WORK_HPCE_LST:
	case WORK_THMS_LST:
	case WORK_NLNK_LST:
	case WORK_MLSR_FLD:
		pPopup = menu.GetSubMenu(3); // select assinged node
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(1, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_ASSIGN, MF_BYCOMMAND); // Assign을 지움
		TrackPopupMenu();
		break;
	case WORK_GMAS_LST:
		pPopup = menu.GetSubMenu(3); // select assinged node
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND); // Property를 지움
		TrackPopupMenu();
		break;
	case WORK_FRLS_LST:
	case WORK_OFFS_LST:
	case WORK_PRLS_LST:
	case WORK_HECB_LST:
	case WORK_HSPT_LST:
	case WORK_HSFC_LST:
		pPopup = menu.GetSubMenu(4);  // select assigned element
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_MLSP_FLD:
		//	  case WORK_MLSR_FLD:
	case WORK_IEHG_LST:
	case WORK_TDNA_LST:
	case WORK_HNGE_LST:
	{
		UINT nMenu = 4;
		if (WORK_HNGE_LST == Data)
		{
			// TODO ! TODO ! TODO ! TODO ! TODO ! TODO !
			//GLink인경우는 Node에 대한 것으로 띄워 주어야할것임.
			CArray<T_PHGE_K, T_PHGE_K> aKey;
			//GetSelectedPhgeKeyListWithMultiTreeItemSelect(aKey);
			//<- 두가지 선택된 경우는 무조건 첨 선택된 것의 종류를 따른다!!!
			if (aKey.GetSize() > 0)
			{
				if (aKey[0].key.type != D_PHGE_GLINK && aKey[0].key.type != D_PHGE_PSPR) nMenu = 4;
				else nMenu = 3;
			}
			else break;
		}

		pPopup = menu.GetSubMenu(nMenu);  // select assigned element
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(1, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_ASSIGN, MF_BYCOMMAND); // Assign을 지움
		TrackPopupMenu();
	}
	break;
	case WORK_TSGR_LST:
		pPopup = menu.GetSubMenu(4);  // select assigned element
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(1, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_ASSIGN, MF_BYCOMMAND); // Assign을 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISPLAY, MF_BYCOMMAND); // Display를 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR, MF_BYCOMMAND); // Disappear를 지움
		TrackPopupMenu();
		break;
	case WORK_VBEM_FLD:
	case WORK_SECV_LST:
		pPopup = menu.GetSubMenu(8);  // Delete & Display & Property만 보이게
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(ID_WORKTREE_DELETE, MF_BYCOMMAND);   // Delete를 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISPLAY, MF_BYCOMMAND);   // Display를 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR, MF_BYCOMMAND); // Disappear를 지움
		TrackPopupMenu();
		break;
	case WORK_EWSF_FLD:
		pPopup = menu.GetSubMenu(4);  // select assigned element
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(1, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_ASSIGN, MF_BYCOMMAND); // Assign을 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISPLAY, MF_BYCOMMAND); // Display를 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR, MF_BYCOMMAND); // Disappear를 지움
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND); // Property를 지움
		pPopup->AppendMenu(MF_STRING, ID_WORKTREE_TABLES, _LS(IDS_WG_TREEMENU_Tables)); // Table 추가
		TrackPopupMenu();
		break;
	case WORK_CGLP_LST:
		pPopup = menu.GetSubMenu(3);  // select assigned element
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(1, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_ASSIGN, MF_BYCOMMAND); // Assign을 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISPLAY, MF_BYCOMMAND); // Display를 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR, MF_BYCOMMAND); // Disappear를 지움
		//pPopup->DeleteMenu(ID_WORKTREE_PROPERTIES,MF_BYCOMMAND); // Property를 지움
		pPopup->AppendMenu(MF_STRING, ID_WORKTREE_TABLES, _LS(IDS_WG_TREEMENU_Tables)); // Table 추가
		TrackPopupMenu();
		break;
	case WORK_SINF_FLD:
	case WORK_SINFch_FLD:
		pPopup = menu.GetSubMenu(4);  // select assigned element
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(ID_WORKTREE_DISPLAY, MF_BYCOMMAND); // Display를 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR, MF_BYCOMMAND); // Disappear를 지움
		TrackPopupMenu();
		break;
	case WORK_RIGD_LST:
		pPopup = menu.GetSubMenu(5);  // Select slave node
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_MCON_LST:
		pPopup = menu.GetSubMenu(5);  // Select slave node  MNET:2790-JHLEE-20070911
		pPopup->DeleteMenu(1, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_ASSIGN, MF_BYCOMMAND); // Assign을 지움
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_LLAN_LST:
	case WORK_LLANch_LST:
	case WORK_LLANjp_LST:
	case WORK_LLANid_LST:
	case WORK_LLANfr_LST:
	case WORK_LLANtr_LST:
	case WORK_LLANop_LST:
	case WORK_SLAN_LST:
	case WORK_SLANch_LST:
	case WORK_SLANfr_LST:
	case WORK_SLANop_LST:
		//    case WORK_NLNK_LST:
	case WORK_LDLN_LST:
	case WORK_GILC_LST:
	case WORK_MGDR_LST:
	case WORK_CBEM_LST:
	case WORK_DIMM_LST:
		pPopup = menu.GetSubMenu(8);  // Delete & Display & Property만 보이게
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_RPSC_LST:
	case WORK_DYFG_FLD:
	case WORK_DYLA_FLD:
	case WORK_EFWD_LST:
	case WORK_DYNZ_FLD:
	case WORK_DYBS_FLD:
	case WORK_DSFC_FLD:
	case WORK_IFBZ_FLD:
		pPopup = menu.GetSubMenu(8);  // Delete & Display & Property만 보이게
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(ID_WORKTREE_DISPLAY, MF_BYCOMMAND); // Display를 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR, MF_BYCOMMAND); // Disappear를 지움
		TrackPopupMenu();
		break;
	case WORK_IMPF_FLD:                                // MNET:2812-JWLEE-20070626
	case WORK_DYNF_FLD:
		pPopup = menu.GetSubMenu(15);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;

	case WORK_BODF_FLD:
	case WORK_NBOF_FLD:
	case WORK_STMP_FLD:
	case WORK_SPLC_LST:
	case WORK_SPFC_LST:
	case WORK_THFC_LST:
	case WORK_THIS_LST:
	case WORK_THAT_LST:
	case WORK_THGA_LST:
	case WORK_MVHL_LST:
	case WORK_MVHLtr_LST:
	case WORK_MVHC_LST:
	case WORK_MVLD_LST:
	case WORK_MVLDjp_LST:
	case WORK_MVLDch_LST:
	case WORK_MVLDid_LST:
	case WORK_MVLDbs_LST:
	case WORK_MVLDfr_LST:
	case WORK_MVLDtr_LST:
	case WORK_SMLC_LST:
	case WORK_HMTP_LST:
	case WORK_HMAT_LST:
	case WORK_ETFC_LST:
	case WORK_CCFC_LST:
	case WORK_TDMT_LST:
	case WORK_TDMF_LST:
	case WORK_TDME_LST:
	case WORK_SECF_LST:
		//    case WORK_RPSC_LST:  
	case WORK_TMAT_LST:
	case WORK_MHAT_LST:
	case WORK_MHTP_LST:
	case WORK_TDNT_LST:
	case WORK_TDCS_LST:
	case WORK_NLLP_LST:
	case WORK_THSL_LST:
	case WORK_EPMT_LST:
		//    case WORK_TDNA_LST:
	case WORK_IEHP_LST:
	case WORK_PDEL_FLD:     // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 복구 시킴)
	case WORK_BUCK_FLD:
	case WORK_EIGV_FLD:
		//     case WORK_ACOP_FLD:

				//case WORK_SPCT_FLD:
					// MNET:1950-BJLEE(BBONG)-20060318
	case WORK_MVCT_FLD:     // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 복구 시킴)
	case WORK_HHCT_FLD:
	case WORK_SMCT_FLD:
	case WORK_NLCT_FLD:
	case WORK_NLCT_LST:
	case WORK_STCT_FLD:     // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 복구 시킴)
	case WORK_GRDP_LST:
	case WORK_BRGT_FLD:
	case WORK_BSPN_LST:
	case WORK_CREF_LST:
	case WORK_P1LT_LST:
	case WORK_LNKF_LST:
	case WORK_FIMP_LST:
	case WORK_THRD_LST:
	case WORK_THCR_LST:
	case WORK_FIBR_LST:
	case WORK_FIBW_LST:
	case WORK_SBCT_FLD:     // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 복구 시킴)
	case WORK_EXLD_FLD:
	case WORK_POGD_FLD:
	case WORK_POLD_LST:
	case WORK_PORD_LST:
	case WORK_BCCT_FLD:     // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 복구 시킴)
	case WORK_AETL_FLD:
	case WORK_MPGB_FLD:
	case WORK_MPHG_LST:
	case WORK_MPST_LST:
	case WORK_IFCT_FLD:
	case WORK_POFC_LST:
	case WORK_SDVI_LST:
	case WORK_SDVE_LST:
	case WORK_SDST_LST:
	case WORK_SDHY_LST:
	case WORK_SDIS_LST:
	case WORK_ASGB_LST:
	case WORK_DMGB_LST:
	case WORK_THRG_LST:
	case WORK_THRI_LST:
	case WORK_THRS_LST:
	case WORK_THEF_LST:
	case WORK_THSF_LST:
	case WORK_IEHC_FLD:
	case WORK_SIHO_FLD:
	case WORK_SIHG_FLD:
	case WORK_FIMP_PO_LST:
	case WORK_FIBR_PO_LST:
	case WORK_FIBW_PO_LST:
	case WORK_PORG_LST:
	case WORK_PORE_LST:
	case WORK_PORI_LST:
	case WORK_POEF_LST:
	case WORK_MLFC_LST:
	case WORK_RLFC_LST:
	case WORK_GACD_FLD:
	case WORK_THGC_FLD:
	case WORK_FLUE_LST:
	case WORK_STTF_LST:
		//case WORK_GLTM_FLD:
		pPopup = menu.GetSubMenu(7);
		VERIFY(pPopup != NULL); // Delete & Property만 보이게
		pPopup->DeleteMenu(5, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_TABLES, MF_BYCOMMAND);  // Table을 지움 
		pPopup->DeleteMenu(ID_WORKTREE_DISPLAY, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR, MF_BYCOMMAND);
		TrackPopupMenu();
		break;
	case WORK_CNLD_SUB_LST:
		pPopup = menu.GetSubMenu(8);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_SPFC_ND_LST:
		pPopup = menu.GetSubMenu(21); // Property만 보이게
		TrackPopupMenu();
		break;
		// 내용은 Properties, 메뉴 이름 변경
	case WORK_PREB_FLD:
	case WORK_PRCS_FLD:
	case WORK_DPAN_FLD:
	case WORK_CCPT_FLD:
	case WORK_MSIS_FLD:
		pPopup = menu.GetSubMenu(21);
		if (Data == WORK_PREB_FLD) pPopup->ModifyMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND, ID_WORKTREE_PROPERTIES, _LS(IDS_TM_ADD_PREB));
		else if (Data == WORK_PRCS_FLD) pPopup->ModifyMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND, ID_WORKTREE_PROPERTIES, _LS(IDS_TM_ADD_PRCS));
		else if (Data == WORK_DPAN_FLD) pPopup->ModifyMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND, ID_WORKTREE_PROPERTIES, _LS(IDS_TM_ADD_DPAN));
		else if (Data == WORK_CCPT_FLD) pPopup->ModifyMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND, ID_WORKTREE_PROPERTIES, _LS(IDS_TM_ADD_CCPT));
		else if (Data == WORK_MSIS_FLD) pPopup->ModifyMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND, ID_WORKTREE_PROPERTIES, _LS(IDS_TM_ADD_MSIS));
		TrackPopupMenu();
		break;
	case WORK_PREB_LST:
		pPopup = menu.GetSubMenu(1);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_PRCS_LST:
	case WORK_DPAN_LST:
	case WORK_CCPT_LST:
	case WORK_MSIS_LST:
		pPopup = menu.GetSubMenu(1);
		VERIFY(pPopup != NULL);
		pPopup->InsertMenu(14, MF_BYPOSITION, ID_WORKTREE_DISAPPEAR, _LS(IDS_TM_WORKTREE_Undisplay));
		pPopup->InsertMenu(14, MF_BYPOSITION, ID_WORKTREE_DISPLAY, _LS(IDS_TM_WORKTREE_Display));
		TrackPopupMenu();
		break;
		//     case WORK_PDEL_FLD:    // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 주석처리)
		//     case WORK_BUCK_FLD:
		//     case WORK_EIGV_FLD:
		//     case WORK_HHCT_FLD:
		//     case WORK_NLCT_FLD:
		//     case WORK_STCT_FLD:
		//     case WORK_SBCT_FLD:
		//     case WORK_BCCT_FLD:
		//     case WORK_MVCT_FLD:
		//     case WORK_CRAN_FLD:
		//     case WORK_PLCB_FLD:
		//       pPopup = menu.GetSubMenu(17);
		//       VERIFY(pPopup != NULL);   // PMS:XXXX-JWKWON-20090623 Delete, Enable, Disable, Properties만..
		//       pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		//       break;

		//     case WORK_SPEC_FLD:    // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 주석처리)
		//     case WORK_STTL_FLD:
		//     case WORK_THISANL_FLD:
		//     case WORK_MVLDANLjp_FLD:
		//     case WORK_MVLDANLch_FLD:
		//     case WORK_MVLDANLid_FLD:
		//     case WORK_MVLDANLtr_FLD:
		//       pPopup = menu.GetSubMenu(18);
		//       VERIFY(pPopup != NULL);
		//       pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		//       break;

	case WORK_CNLD_FLD:
	case WORK_SDSP_FLD:
	case WORK_BMLD_ELEM_FLD:
	case WORK_BMLD_TYPC_FLD:
	case WORK_PRES_FLD:
	case WORK_NTMP_FLD:
	case WORK_ETMP_FLD:
		pPopup = menu.GetSubMenu(22);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_BMLD_LINE_FLD:
	case WORK_BMLD_FLOR_FLD:
	case WORK_PNLA_FLD:
	case WORK_FMLD_FLD:
	case WORK_PRST_FLD:
	case WORK_PTNS_FLD:
	case WORK_GTMP_FLD:
	case WORK_BTMP_FLD:
	case WORK_PTMP_FLD:
	case WORK_ARPR_FLD:
	case WORK_FIPA_LST:
	case WORK_FSSF_FLD:
		pPopup = menu.GetSubMenu(7);  // Display load & Delete 만
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND); // properties를 지움
		TrackPopupMenu();
		break;
	case WORK_TDPL_FLD:
		pPopup = menu.GetSubMenu(7);  // Display load & Delete & Property
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
		/*
	  case WORK_PRES_FLD:
		pPopup = menu.GetSubMenu(7);  // Display load & Delete 만
		VERIFY(pPopup != NULL);
			pPopup->DeleteMenu(5,MF_BYPOSITION);
			pPopup->DeleteMenu(ID_WORKTREE_TABLES,MF_BYCOMMAND);  // Table을 지움
			pPopup->DeleteMenu(ID_WORKTREE_PROPERTIES,MF_BYCOMMAND); // properties를 지움
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
			break;
			*/
	case WORK_CRAN_LST:
		//case WORK_NCRN_LST:
	case WORK_WIND_FLD:
	case WORK_SEIS_FLD:
	case ID_MAIN_RIBBON_SEISEVAL_PSEUDO_FORCE:
	case WORK_SSEI_FLD:
		pPopup = menu.GetSubMenu(7);  // Display load
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(5, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_TABLES, MF_BYCOMMAND);  // Table을 지움 
		TrackPopupMenu();
		break;
	case WORK_WIND_WA_WT_FLD:
		pPopup = menu.GetSubMenu(7);  // Display load
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(1, MF_BYPOSITION);
		pPopup->DeleteMenu(4, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_DELETE, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_TABLES, MF_BYCOMMAND);  // Table을 지움 
		TrackPopupMenu();
		break;
	case WORK_WDPR_AREA_FLD:
	case WORK_WDPR_BEAM_FLD:
	case WORK_WDPR_NODAL_FLD:
	case WORK_WDPR_FUNC_FLD:
	case WORK_EPST_FLD:
	case WORK_EPSE_FLD:
		pPopup = menu.GetSubMenu(7);  // Display load & Delete 만
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND); // properties를 지움
		TrackPopupMenu();
		break;
	case WORK_WVEP_LST:
	case WORK_WDFC_LST:
	case WORK_POSL_LST:
	case WORK_POSP_LST:
		//case WORK_GLTM_FLD:
		pPopup = menu.GetSubMenu(7);
		VERIFY(pPopup != NULL); // Delete & Property만 보이게
		pPopup->DeleteMenu(5, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_TABLES, MF_BYCOMMAND);  // Table을 지움 
		pPopup->DeleteMenu(ID_WORKTREE_DISPLAY, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR, MF_BYCOMMAND);
		TrackPopupMenu();
		break;
	case WORK_PLCB_FLD:               // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 복구시킴)
		pPopup = menu.GetSubMenu(11);  // Property만 보이게
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_EXPANDALL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_1LEVEL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_2LEVEL, MF_BYCOMMAND);
		TrackPopupMenu();
		break;
	case WORK_ESEQ_FLD:
		//case WORK_PZEF_FLD:  
	case WORK_HCRP_FLD:
	case WORK_STAG_LST:
		pPopup = menu.GetSubMenu(11);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_PZEF_FLD:
	case WORK_PO_MEMBASSIG_FLD:
		pPopup = menu.GetSubMenu(6);  // Property만 보이게
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_TABLES, MF_BYCOMMAND);  // Table을 지움 
		pPopup->DeleteMenu(ID_WORKTREE_DISPLAY, MF_BYCOMMAND); // Display를 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR, MF_BYCOMMAND); // Disappear를 지움
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_STLD_LST:
	case WORK_LTOM_LST:
	case WORK_DMAS_FLD:
		pPopup = menu.GetSubMenu(6);  // Display & Property만 보이게
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_DISPLAY, MF_BYCOMMAND);    // Diplay를 지움
		pPopup->DeleteMenu(ID_WORKTREE_DISAPPEAR, MF_BYCOMMAND);  // Undisplay를 지움
		pPopup->DeleteMenu(ID_WORKTREE_TABLES, MF_BYCOMMAND);     // tables을 지움
		TrackPopupMenu();
		break;
	case WORK_ELNK_FLD:
	case WORK_FBLA_FLD:
		pPopup = menu.GetSubMenu(7);  // Display만 보이게
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND); // properties를 지움
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_TDNA_FLD:
	case WORK_SLAN_FLD:
	case WORK_SLANch_FLD:
	case WORK_SLANop_FLD:
	case WORK_LLAN_FLD:
	case WORK_LLANch_FLD:
	case WORK_LLANjp_FLD:
	case WORK_LLANid_FLD:
	case WORK_LLANfr_FLD:
	case WORK_LLANtr_FLD:
	case WORK_LLANop_FLD:
		pPopup = menu.GetSubMenu(6);  // Display만 보이게
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_TABLES, MF_BYCOMMAND);     // tables을 지움
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND); // properties를 지움
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_TMLD_FLD:
	case WORK_CMCS_FLD:
	case WORK_CRPC_FLD:
	case WORK_STBK_FLD:
	case WORK_ULCB_FLD:
		pPopup = menu.GetSubMenu(9);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_ELEM_FLD: // Table만 표시 
	case WORK_CONS_FLD:
	case WORK_NSPR_FLD:
	case WORK_NSPR_LIN_FLD:
	case WORK_NSPR_NON_FLD:
	case WORK_NSPR_MLT_FLD:
	case WORK_GSTP_FLD:
	case WORK_SSPS_FLD:
	case WORK_SSPS_LIN_FLD:
	case WORK_SSPS_NON_FLD:
	case WORK_NLNK_FLD:
	case WORK_FRLS_FLD:
	case WORK_OFFS_FLD:
	case WORK_PRLS_FLD:
	case WORK_RIGD_FLD:
		//case WORK_MCON_FLD:   // MNET:2790-JHLEE-20070911
	case WORK_SKEW_FLD:
	case WORK_NMAS_FLD:
	case WORK_GMAS_FLD:
	case WORK_HECB_FLD:
	case WORK_HSPT_FLD:
	case WORK_HAHS_FLD:
	case WORK_IEHG_FLD:
	case WORK_EDMP_FLD:
	case WORK_THMS_FLD:
	case WORK_GIMP_FLD:
	case WORK_GSTY_FLD:
	case WORK_P1LA_FLD:
	case WORK_MGDR_FLD:
	case WORK_CBEM_FLD:
	case WORK_IFGS_FLD:
	case WORK_EQMF_FLD:
	case WORK_INMF_FLD:
	case WORK_SDVI_FLD:
	case WORK_SDVE_FLD:
	case WORK_SDST_FLD:
	case WORK_SDHY_FLD:
	case WORK_SDIS_FLD:
	case WORK_ASGB_FLD:
	case WORK_DMGB_FLD:
		pPopup = menu.GetSubMenu(10);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_HNGE_FLD: // MNET:XXXX 20070716 mylee
		pPopup = menu.GetSubMenu(10);
		pPopup->DeleteMenu(0, MF_BYPOSITION);
		pPopup->AppendMenu(MF_STRING, ID_QUERY_DGN_POVR_HNGEPROP, _LS(IDS_TREE_MENU_PHGE_PROP));
		pPopup->AppendMenu(MF_STRING, ID_QUERY_DGN_POVR_HNGE, _LS(IDS_TREE_MENU_PHGE_ASGN));
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_HNGT_FLD: // MNET:XXXX 20071217 mylee
		pPopup = menu.GetSubMenu(10);
		pPopup->DeleteMenu(0, MF_BYPOSITION);
		pPopup->AppendMenu(MF_STRING, ID_STRUCT_PUSHOVER_DEFINE_HINGE, _LS(IDS_TREE_MENU_PHGT_DEFINE));
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_MVCD_FLD:
		pPopup = menu.GetSubMenu(11);
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_EXPANDALL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_1LEVEL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_2LEVEL, MF_BYCOMMAND);
		TrackPopupMenu();
		break;
	case WORK_BCCT_LST: // MNET1199, KJH20050217
		pPopup = menu.GetSubMenu(13);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_ROOT:
	case WORK_ANAL_FLD:
	case WORK_STRU_FLD:
	case WORK_NPLN_FLD:
	case WORK_BNDR_FLD:
	case WORK_MASS_FLD:
	case WORK_STLD_FLD:
	case WORK_SPEC_FLD:     // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 복구시킴)
	case WORK_PSTD_FLD:
	case WORK_THISANL_FLD:  // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 복구시킴)
		//    case WORK_MVLDANL_FLD:
	case WORK_MVLDANLjp_FLD:  // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 복구시킴)
	case WORK_MVLDANLch_FLD:
	case WORK_MVLDANLid_FLD:
	case WORK_MVLDANLtr_FLD:
	case WORK_STTL_FLD:       // PMS:3976-JWKWON-20090710 : 8월말에 작업(우선 복구시킴)
	case WORK_HYDR_FLD:
	case WORK_STAG_FLD:
	case WORK_GRID_FLD:
	case WORK_PUSH_FLD:
	case WORK_FIRE_FLD:
		pPopup = menu.GetSubMenu(14);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_REBD_FLD:
		pPopup = menu.GetSubMenu(11);
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_PROPERTIES, MF_BYCOMMAND);
		TrackPopupMenu();
		break;
	case WORK_REBB_FLD:   case WORK_REBC_FLD:   case WORK_REBR_FLD:
	case WORK_REBW_FLD:   case WORK_RBMS_FLD:   case WORK_RBMW_FLD:
	case WORK_RBST_FLD:   case WORK_RBSL_FLD:
		pPopup = menu.GetSubMenu(11);
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_EXPANDALL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_1LEVEL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_2LEVEL, MF_BYCOMMAND);
		TrackPopupMenu();
		break;
	case WORK_RIPB_FLD:   case WORK_RIPC_FLD:
		pPopup = menu.GetSubMenu(11);
		VERIFY(pPopup != NULL);
		pPopup->DeleteMenu(ID_WORKTREE_ASSIGN, MF_BYCOMMAND); // Assign을 지움
		pPopup->DeleteMenu(3, MF_BYPOSITION);
		pPopup->DeleteMenu(ID_WORKTREE_EXPANDALL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_1LEVEL, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_WORKTREE_EXPAND_2LEVEL, MF_BYCOMMAND);
		TrackPopupMenu();
		break;
	}
}

void CTreeWorkCtrlNew::OnWorktreeActive()
{
}

void CTreeWorkCtrlNew::OnWorktreeDelete()
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);

	if (selItemNum) DeleteItemEntity(aItem);
}

void CTreeWorkCtrlNew::OnWorktreeInactive()
{
}

void CTreeWorkCtrlNew::OnWorktreeSelect()
{
}

void CTreeWorkCtrlNew::OnWorktreeUnselect()
{
}

void CTreeWorkCtrlNew::OnWorktreeUnselectall()
{
}

void CTreeWorkCtrlNew::OnWorktreeActiveall()
{
}

void CTreeWorkCtrlNew::OnWorktreeAssign()
{
}

void CTreeWorkCtrlNew::OnWorktreeProperties()
{
}

void CTreeWorkCtrlNew::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if (aItem.GetSize() == 0)  return;
	UINT Data = GetItemData(aItem[0]);
	if (selItemNum > 1) { Data = 0; } // 두 개 이상의 아이템이 선택되어 있으면 Assign 불가!!

	switch (Data)
	{
	case WORK_GRUP_LST:
	case WORK_MATL_LST:
	case WORK_SECT_LST:
	case WORK_CSCS_LST:
	case WORK_THIK_LST:
	case WORK_CONS_LST:
	case WORK_NSPR_LIN_LST:
	case WORK_NSPR_NON_LST:
	case WORK_NSPR_MLT_LST:
	case WORK_GSTP_LST:
	case WORK_SSPS_LIN_LST:
	case WORK_SSPS_NON_LST:
	case WORK_FRLS_LST:
	case WORK_OFFS_LST:
	case WORK_PRLS_LST:
	case WORK_RIGD_LST:
	case WORK_MCON_LST:  // MNET:2790-JHLEE-20070911  
	case WORK_DRLS_FLD:
	case WORK_SKEW_LST:
	case WORK_NMAS_LST:
	case WORK_GMAS_LST:
		//  	case WORK_MLSP_FLD:  
	case WORK_SINF_FLD:
	case WORK_SINFch_FLD:
	case WORK_THNL_LST:
	case WORK_SMPT_LST:
	case WORK_HECB_LST:
	case WORK_HSPT_LST:
	case WORK_HSFC_LST:
	case WORK_IEHP_LST:
	case WORK_ESSF_LST:
	case WORK_WSSF_LST:
	case WORK_PSSF_LST:
	case WORK_GIMP_LST:
	case WORK_P1LA_LST:
	case WORK_GSTY_LST:
	case WORK_HNGT_LST:
	case WORK_REBB_LST:
	case WORK_REBC_LST:
	case WORK_REBR_LST:
		//    case WORK_REBW_LST:
	case WORK_RBMS_LST:
	case WORK_RBSL_LST:
	case WORK_RBMW_LST:
	case WORK_RBST_LST:
	case WORK_SBDO_LST:
	case WORK_MADO_LST:
	case WORK_PREB_LST:
	case WORK_PRCS_LST:
	case WORK_DPAN_LST:
	case WORK_CCPT_LST:
	case WORK_MSIS_LST:
		// 마우스 커서를 드래깅 모드로
		::SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (LONG_PTR)AfxGetApp()->LoadCursor(IDC_CURSOR_ASSIGN));
		//::SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (long)AfxGetApp()->LoadCursor(IDC_CURSOR_ASSIGN));
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ASSIGN));
		m_bDragPoss = TRUE;
		break;
	default:
		// 마우스 커서를 드래깅 불가능 모드로
		::SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (LONG_PTR)AfxGetApp()->LoadCursor(IDC_CURSOR_NOASSIGN));
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_NOASSIGN));
		m_bDragPoss = FALSE;
		break;
	}
	m_bDrag = TRUE;
	m_hDragItem = GetSelectedItem();

	CWnd* pWnd = GetCapture();
	if (pWnd == NULL)
	{
		BOOL bCheck = ReleaseCapture();
		if (bCheck == FALSE)
		{
			AfxMessageBox(_T("Error"));
		}
		else
		{
			SetCapture();
		}
	}
	else
		SetCapture();

	*pResult = 0;
}

void CTreeWorkCtrlNew::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDrag) // 드래깅 모드이면 
	{
		SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_ARROW));
		::SetCursor(LoadCursor(NULL, IDC_ARROW));
		m_bDrag = FALSE;
		ReleaseCapture();
		if (m_bDragPoss == TRUE) /*EndDrag(point)*/
			m_bDragPoss = FALSE;
	}
	MBaseTreeCtrl::OnLButtonUp(nFlags, point);
}

void CTreeWorkCtrlNew::OnCaptureChanged(CWnd* pWnd)
{
	if (m_bDrag && pWnd == 0)
		::SetCapture(this->GetSafeHwnd());
}

LRESULT CTreeWorkCtrlNew::OnNcHitTest(CPoint point)
{
	return MBaseTreeCtrl::OnNcHitTest(point);
}

void CTreeWorkCtrlNew::OnWorktreeAddmatl()
{
	CCMMatPageItemDlg MatDlg(m_pDoc);
	MatDlg.DoModal();
}

void CTreeWorkCtrlNew::OnWorktreeAddsect()
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if (selItemNum == 0) return;

	UINT data = GetItemData(aItem[0]);

	if (data == WORK_SECT_FLD || data == WORK_PROP_FLD)
	{
		CCMSectItemDlg SecDlg;
		SecDlg.DoModal();
	}
	else if (data == WORK_CSCS_FLD)
	{
		CCMStageCompSectItemDlg dlg;
		dlg.DoModal();
	}
	else ASSERT(0);
}

void CTreeWorkCtrlNew::OnWorktreeAddthik()
{
	CCMThkPageItemDlg ThkDlg(m_pDoc);
	ThkDlg.DoModal();
}

void CTreeWorkCtrlNew::OnWorktreeSelectplus()
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if (selItemNum == 0) return;

	// [2009-09-28] Kim, Geun Young (Tel: 2042, gykim@midasit.com)
	// Main Domain의 경우 자식 Item이 있어도 동작하게끔 하기 위함.
	UINT Data = GetItemData(aItem[0]);
	if (FALSE == ItemHasChildren(aItem[0]) || Data == WORK_MADO_LST)
	{
		SelectEntity(Data, FALSE);
	}
}

void CTreeWorkCtrlNew::OnWorktreeDisplay()
{
}

void CTreeWorkCtrlNew::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	BOOL  bMulti = FALSE;

	short nKeyState = GetKeyState(VK_LCONTROL);
	if (nKeyState & 0x8000) bMulti = TRUE;
	if (bMulti == FALSE) { nKeyState = GetKeyState(VK_LSHIFT); if (nKeyState & 0x8000) bMulti = TRUE; }

	if (selItemNum && bMulti)
	{
		HTREEITEM preParItem = GetParentItem(aItem[0]);
		UINT preParData = GetItemData(preParItem);
		HTREEITEM newParItem = GetParentItem(pNMTreeView->itemNew.hItem);
		UINT newParData = GetItemData(newParItem);
		if (preParData != newParData) { *pResult = TRUE; return; }
	}
	// need to debug

	*pResult = 0;
}

void CTreeWorkCtrlNew::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DELETE)
		OnWorktreeDelete();

	MBaseTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTreeWorkCtrlNew::OnWorktreeDisappear()
{
}

void CTreeWorkCtrlNew::OnWorktreeDeleteUnusMatl()
{
}

void CTreeWorkCtrlNew::OnWorktreeDeleteUnusSect()
{
}

void CTreeWorkCtrlNew::OnWorktreeDeleteUnusThik()
{
}

void CTreeWorkCtrlNew::OnWorktreeActivePlus()
{
}

void CTreeWorkCtrlNew::OnWorktreeTables()
{
}

void CTreeWorkCtrlNew::OnWorktreeEnable()
{
}

void CTreeWorkCtrlNew::OnWorktreeDisable()
{
}

void CTreeWorkCtrlNew::OnWorktreePSCRebar()
{
}

void CTreeWorkCtrlNew::OnWorktreeExpandAll()
{
}

void CTreeWorkCtrlNew::OnWorktreeExpand1LEVEL()
{
}

void CTreeWorkCtrlNew::OnWorktreeExpand2LEVEL()
{
}

void CTreeWorkCtrlNew::OnWorktreePhgePropTable()
{
}

void CTreeWorkCtrlNew::OnWorktreePhgeAssignTable()
{
}

void CTreeWorkCtrlNew::OnWorktreePhgtDefine()
{
}

void CTreeWorkCtrlNew::OnWorktreeAddmado()
{
}

void CTreeWorkCtrlNew::OnWorktreeAddsbdo()
{
}
