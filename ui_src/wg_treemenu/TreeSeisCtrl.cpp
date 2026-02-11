#include "stdafx.h"
#include "wg_treemenu.h"
#include "TreeSeisCtrl.h"
#include "TreeMenuCtrl.h"

#include "..\wg_main\wg_mainres2.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_StoryData.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\SeisEvalDataTool.h"
#include "..\wg_dbLock\LockMgr.h"

#include "..\wg_db\DispOptMgr.h"
#include "..\wg_db\DispOptColorObj.h"

#include "..\wg_jud\JUDBasePlateTypeDlg.h"

#include "..\wg_jfd\JFD_FoundationInfoDlg.h"

#include "..\wg_rps\wg_cmdPushoverAll.h"
#include "..\wg_gr\wg_grDisplaySetting.h"
#include "..\wg_cmd2\ExportFuncCMD2.h"
#include "..\wg_cmd\IehpItemDlg.h"
#include "..\wg_cmd\IehpItemDlg_MEC.h"

IMPLEMENT_DYNAMIC(CTreeSeisCtrl, MBaseTreeCtrl);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeSeisCtrl

CTreeSeisCtrl::CTreeSeisCtrl() : m_nMethod(0)
{
	m_BkColor = RGB(192, 220, 192);
	m_strBkColorName.Empty();
	m_strBkTitleColorName.Empty();
}

CTreeSeisCtrl::~CTreeSeisCtrl()
{
	m_strBkColorName.Empty();
	m_strBkTitleColorName.Empty();
}

#define CTreeCtrl MBaseTreeCtrl
BEGIN_MESSAGE_MAP(CTreeSeisCtrl, CTreeCtrl)
#undef CTreeCtrl
	//{{AFX_MSG_MAP(CTreeSeisCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeSeisCtrl message handlers

void CTreeSeisCtrl::CreateTree()
{
	return;
}

void CTreeSeisCtrl::LoadMenu()
{
	SetItemTree(TRUE);
}

void CTreeSeisCtrl::DestroyTree()
{
	// 생성되지 않거나 Destroy 함수가 두번 호출되는 경우 비정상종료 방지 
	if (!m_ImageList.GetSafeHandle())
		return;

	for (int i = 0; i < m_ImageList.GetImageCount(); i++)
		m_ImageList.Remove(0);
	m_ImageList.DeleteImageList();
}

int CTreeSeisCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (MBaseTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateTree();
	LoadMenu();
	EndInsertTreeItem();

	//ToggleStyle(TVS_LINESATROOT);  
	return 0;
}

void CTreeSeisCtrl::OnDestroy()
{
	MBaseTreeCtrl::OnDestroy();
	DestroyTree();
}

void CTreeSeisCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	HTREEITEM ht;
	ht = GetSelectedItem();

	int pos = 0;
	HTREEITEM prvHt, curHt = ht;
	for (prvHt = curHt; prvHt != NULL; curHt = prvHt, pos++) prvHt = GetPrevSiblingItem(curHt);

	if (FALSE == ItemHasChildren(ht))
	{
		BOOL bSendMessage = TRUE;
		UINT Data = GetItemData(ht);
		if (Data != 0)
		{
			//비활성화 된 아이템들 실행 못하고 알림 띄우도록 함. WGenDoc에서 처리해주면 지워도 됨...  pyj0827
			if (Data == ID_MAIN_RIBBON_SEISEVAL_1ST_RC_PERFORM ||
				Data == ID_MAIN_RIBBON_SEISEVAL_1ST_STL_PFMC ||
				Data == ID_MAIN_RIBBON_MENU_MEMBER_DCR_RCBEAM ||
				Data == ID_MAIN_RIBBON_MENU_MEMBER_DCR_RCCOLUMN ||
				Data == ID_MAIN_RIBBON_MENU_MEMBER_DCR_RCWALL ||
				Data == ID_MAIN_RIBBON_SEISEVAL_1ST_STL_BEAM_PFMC ||
				Data == ID_MAIN_RIBBON_SEISEVAL_1ST_STL_COLM_PFMC ||
				Data == ID_MAIN_RIBBON_SEISEVAL_1ST_STL_PNZN_PFMC ||
				Data == ID_MAIN_RIBBON_SEISEVAL_1ST_STL_BRAC_PFMC ||
				Data == ID_MAIN_RIBBON_SEISEVAL_STORY_DCR ||
				Data == ID_MAIN_RIBBON_SEISEVAL_STIFF_CHK_SECTION ||
				Data == ID_MAIN_RIBBON_SEISEVAL_STORY_DEFORM_DCR ||
				Data == ID_MAIN_RIBBON_SEISEVAL_STORY_PERFORM)
			{
				BOOL bEnable = IsAnalysisEnable();

				if (!bEnable)
				{
					AfxMessageBox(_T("해석 필요"));
					return;
				}
			}
			else if (Data == ID_MAIN_UPDATE_PO_PROP ||
				Data == ID_MAIN_SEIS_PUSHOVER_CURVE ||
				Data == ID_MAIN_RIBBON_SEISEVAL_2ND_RC_PERFORM ||
				Data == ID_MAIN_RIBBON_SEISEVAL_2ND_TB_RC_BEAM_PFM ||
				Data == ID_MAIN_RIBBON_SEISEVAL_2ND_TB_RC_COLM_PFM ||
				Data == ID_MAIN_RIBBON_SEISEVAL_2ND_TB_RC_WALL_PFM ||
				Data == ID_MAIN_RIBBON_SEISEVAL_2ND_STL_BEAM_PFMC ||
				Data == ID_MAIN_RIBBON_SEISEVAL_2ND_STL_COLM_PFMC ||
				Data == ID_MAIN_RIBBON_SEISEVAL_2ND_STL_PNZN_PFMC ||
				Data == ID_MAIN_RIBBON_SEISEVAL_2ND_STL_BRAC_PFMC ||
				Data == ID_MAIN_RIBBON_SEISEVAL_2ND_TB_STORMEMB_PFM ||
				Data == ID_MAIN_RIBBON_SEISEVAL_2ND_TB_STORDRFT_PFM ||
				Data == ID_PO_RESULT_TEMP ||
				Data == ID_PO_RESULT_TB_TEMP)
			{
				BOOL bEnable = IsPOAnalysisEnable();

				if (!bEnable)
				{
					AfxMessageBox(_LS(IDS_DB_SEIS_PO_AUTO_PO_ANAL_NEEDED));
					return;
				}
			}
			else if (Data == ID_PHGE_LST)
			{
				EditProperty_Pushover_PHGE(pDoc, ht, 0, pos);
				bSendMessage = FALSE;
			}
			else if (Data == ID_PHGT_LST)
			{
				EditProperty_Pushover_PHGT(pDoc, ht, 0, pos);
				bSendMessage = FALSE;
			}
			else if (Data == ID_POLC_LST)
			{
				EditProperty_Pushover_POLC(pDoc, ht, 0, pos);
				bSendMessage = FALSE;
			}
			else if (Data == ID_IEHP_LST)
			{
				CArray<T_IEHP_K, T_IEHP_K> aIehpKeyList;
				pDoc->m_pAttrCtrl->GetIehpKeyList(aIehpKeyList);
				T_IEHP_K IehpKey = aIehpKeyList[pos - 1];
				T_IEHP_D IehpD;
				if (pDoc->m_pAttrCtrl->GetIehp(IehpKey, IehpD) == TRUE)
				{
					T_IEHG_K IehgKey;

#if defined (_MEC)
					CIehpItemDlg_MEC dlg;
#else
					CIehpItemDlg dlg;
#endif
					dlg.SetAssignHingeMode(FALSE, IehgKey);
					dlg.SetParamData(IehpKey, IehpD);
					dlg.DoModal();
				}
				bSendMessage = FALSE;
			}
			else if (Data == ID_IEHG_LST)
			{
				CArray<T_IEHG_K, T_IEHG_K> aIehgKeyList;
				pDoc->m_pAttrCtrl->GetIehgKeyList(aIehgKeyList);
				T_IEHG_K IehgKey = aIehgKeyList[pos - 1];
				T_IEHG_D IehgD;
				if (!pDoc->m_pAttrCtrl->GetIehg(IehgKey, IehgD)) return;

				T_IEHP_D IehpD;
				if (!pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, IehpD)) return;

#if defined (_MEC)
				CIehpItemDlg_MEC dlg;
#else
				CIehpItemDlg dlg;
#endif
				dlg.SetAssignHingeMode(TRUE, IehgKey);
				dlg.SetParamData(IehgD.IehpKey, IehpD);
				dlg.DoModal();

				bSendMessage = FALSE;
			}
			else if (Data == ID_FIMP_LST || Data == ID_FIMP_PO_LST)
			{
				CArray<T_FIMP_K, T_FIMP_K> aFimpKeyList;
				pDoc->m_pAttrCtrl->GetFimpKeyList(aFimpKeyList);
				T_FIMP_K FimpKey = aFimpKeyList[pos - 1];
				CMD2_DoModal(D_CMD2_FIBER_MATERIAL_PROP_ITEM, &FimpKey);

				bSendMessage = FALSE;
			}
			else if (Data == ID_FIBR_LST)
			{
				CArray<T_FIBR_K, T_FIBR_K> aFibrKeyList; // Fiber Division of Section (MCPC Wall)
				pDoc->m_pAttrCtrl->GetFibrKeyList(aFibrKeyList, D_FIBR_TYPE_IEHP);
				T_FIBR_K FibrK = aFibrKeyList[pos - 1];
				ExportDisplaySetting::ShowSectDivDlg(this, FibrK, D_FIBR_DLG_TYPE_IEHP_BEAM);

				bSendMessage = FALSE;
			}
			else if (Data == ID_FIBW_LST)
			{
				CArray<T_FIBW_K, T_FIBW_K> aFibwKeyList; // Fiber Division of Section (MCPC Wall)
				pDoc->m_pAttrCtrl->GetFibwKeyList(aFibwKeyList, D_FIBW_TYPE_IEHP);
				T_FIBW_K FibwK = aFibwKeyList[pos - 1];
				ExportDisplaySetting::ShowSectDivDlg(this, FibwK, D_FIBR_DLG_TYPE_IEHP_WALL);

				bSendMessage = FALSE;
			}
			else if (Data == ID_FIBR_PO_LST)
			{
				CArray<T_FIBR_K, T_FIBR_K> aFibrKeyList; // Fiber Division of Section (MCPC Wall)
				pDoc->m_pAttrCtrl->GetFibrKeyList(aFibrKeyList, D_FIBR_TYPE_PHGT);
				T_FIBR_K FibrK = aFibrKeyList[pos - 1];
				ExportDisplaySetting::ShowSectDivDlg(this, FibrK, D_FIBR_DLG_TYPE_PHGT_BEAM);

				bSendMessage = FALSE;
			}
			else if (Data == ID_FIBW_LST)
			{
				CArray<T_FIBW_K, T_FIBW_K> aFibwKeyList; // Fiber Division of Section (MCPC Wall)
				pDoc->m_pAttrCtrl->GetFibwKeyList(aFibwKeyList, D_FIBW_TYPE_PHGT);
				T_FIBW_K FibwK = aFibwKeyList[pos - 1];
				ExportDisplaySetting::ShowSectDivDlg(this, FibwK, D_FIBR_DLG_TYPE_PHGT_WALL);

				bSendMessage = FALSE;
			}
			if (bSendMessage)
			{
				WPARAM wParam = MAKEWPARAM(Data, 0);
				AfxGetMainWnd()->SendMessage(WM_COMMAND, wParam, 0L);
			}
		}
	}
}

void CTreeSeisCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTreeSeisCtrl::ToggleStyle(DWORD lStyleMask)
{
	long lStyleOld;

	lStyleOld = GetStyle();
	BOOL bSetBits = !(lStyleMask & lStyleOld);
	if (bSetBits)
		lStyleOld |= lStyleMask;
	else
		lStyleOld &= ~(lStyleMask);

	SetWindowLongPtr(m_hWnd, GWL_STYLE, lStyleOld);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

	DWORD dwStyle, dwExStyle;
	GetTreeCtrlStyles(dwStyle, dwExStyle);
	if (dwStyle & lStyleMask)
		ModifyTreeCtrlStyles(lStyleMask, 0, 0, 0);
	else
		ModifyTreeCtrlStyles(0, lStyleMask, 0, 0);
}

void CTreeSeisCtrl::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		SetItemTree();
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_SEL_PRE:
	case D_UPDATE_SEL_POST:
	case D_UPDATE_REMOVE_PUSHOVER:
	case D_UPDATE_EXE_END:
		Invalidate(FALSE);
		break;
	default:
		break;
	}
}

void CTreeSeisCtrl::SetBkGroundColor(COLORREF BkColor)
{
	m_BkColor = BkColor;
	if (GetSafeHwnd())
	{
		// change the background color
		SetBkColor(m_BkColor);
		Invalidate();
	}
}


void CTreeSeisCtrl::SetItemTree(BOOL bInit/*=TRUE*/)
{
	if (GetCount()) DeleteAllItems();
	// 기존 Work tree의 메모리 지움

	//	m_hRootTi = InsertTreeItem(_LS(IDS_TMWK__MSG2),2,2);
	m_hRootTi = GetRootItem();

	m_hRootTi = InsertTreeItem(m_hRootTi, 0,_LS(IDS_TREE_SEIS_SEIS_EVAL), _T(""));

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc)
	{
		m_nMethod = pDoc->m_pAttrCtrl2->GetSemeMethod();
	}

	SetMainCtrlItemTree();
	SetEffeStiffItemTree();
	SetPropertyItemTree();
	SetGeneralDgnParamItemTree();
	SetRcDgnParamItemTree();
	SetCommonParameItemTree();
	if (m_nMethod != D_SEME_METHOD_AIK_G_001_2021)
	{
		SetInfillStrutItemTree();
		SetPreItemTree();
	}

	switch (m_nMethod)
	{
	case D_SEME_METHOD_KISTEC2013:
	case D_SEME_METHOD_MOE2018:
	case D_SEME_METHOD_MOE2019:
	case D_SEME_METHOD_KISTEC2019:
		{
			Set1stItemTree();
			Set2ndItemTree(pDoc, bInit);
		}
		break;
	case D_SEME_METHOD_AIK_G_001_2021:
		{
			SetPBDPOItemTree(pDoc);
			SetPBDTHItemTree(pDoc);
		}
		break;
	default: ASSERT(0);
		break;
	}

	SelectItem(m_hRootTi);

	Expand(m_hRootTi, TVE_EXPAND); // 일단 한단계는 열고
	if (bInit) // 처음 여는거면 한층 더 !
	{
		HTREEITEM TiChild = GetChildItem(m_hRootTi);;
		while (TiChild != NULL)
		{
			Expand(TiChild, TVE_EXPAND);
			TiChild = GetNextItem(TiChild, TVGN_NEXT);
		}
	}
}

BOOL CTreeSeisCtrl::GetCustomColor(HTREEITEM hItem)
{
	MBaseTreeCtrl::GetCustomColor(hItem);

	UINT Data = GetItemData(hItem);

	int nItemData = GetItemData(hItem);

	CSeisEvalDataTool Tool;
	const bool bMasonryMethod = Tool.IsMasonryEvalMethod();
	const bool bEffStiffAxialMethod = Tool.IsEffectStiffFromAxialRatioMethod();
	const bool bPerformanceDesign = Tool.IsPerformanceDesignMethod();

	BOOL bSet = TRUE;
	switch (nItemData)
	{
	case ID_1ST_RESULT_TEMP: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_1ST_RC_PERFORM: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_1ST_STL_PFMC: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_MASONRY_INFO: bSet = (CDBDoc::GetDocPoint() != NULL && CDBDoc::GetDocPoint()->
			m_pAttrCtrl2->IsConsiderEffectOfMasonry());
		break;
	case ID_MAIN_RIBBON_SEISEVAL_MASONRY_CAPA: bSet = (CDBDoc::GetDocPoint() != NULL && CDBDoc::GetDocPoint()->
			m_pAttrCtrl2->IsConsiderEffectOfMasonry());
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STEEL_MATERIAL: bSet = !bPerformanceDesign;
		break;
	case ID_MAIN_RIBBON_SEISEVAL_MASONRY_MATERIAL: bSet = bMasonryMethod;
		break;
	case ID_MAIN_SEIS_REINF_DETAILS: bSet = (IsKISTEC2019() || IsMOE());
		break;
	case ID_MAIN_RC_COLUMN_M_TYPE: bSet = (IsKISTEC2019() || IsMOE2019());
		break;
	case ID_MAIN_SEIS_PM_CURVE_METHOD: bSet = (IsKISTEC2019() || IsMOE2019());
		break;
	case ID_MAIN_RIBBON_SEISEVAL_MASONRY_INFILL_STRUT: bSet = bMasonryMethod;
		break;
	case ID_MAIN_RIBBON_SEISEVAL_ASSIGN_INFILL_STRUT: bSet = bMasonryMethod;
		break;
	case ID_MAIN_RIBBON_SEISEVAL_REDUCE_STIFF: bSet = IsMOE();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STIFF_SET_LC: bSet = bEffStiffAxialMethod;
		break;
	case ID_1ST_RESULT_TB_TEMP: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_MENU_MEMBER_DCR_RCBEAM: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_MENU_MEMBER_DCR_RCCOLUMN: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_MENU_MEMBER_DCR_RCWALL: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_1ST_STL_BEAM_PFMC: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_1ST_STL_COLM_PFMC: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_1ST_STL_PNZN_PFMC: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_1ST_STL_BRAC_PFMC: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_1ST_MASONRY_PFMC: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_MENU_MEMBER_DCR_RCCLIF: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_1ST_MASONRY_STRUT_PFMC: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STORY_DCR: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STORY_DEFORM_DCR: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STIFF_CHK_SECTION: bSet = bEffStiffAxialMethod && IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STORY_PERFORM: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_RC_BEAM_GRAV_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_RC_COLM_GRAV_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_RC_WALL_GRAV_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_RC_CLIF_GRAV_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_RC_BEAM_SEIS_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_RC_COLM_SEIS_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_RC_WALL_SEIS_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_RC_CLIF_SEIS_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STL_BEAM_GRAV_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STL_COLM_GRAV_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STL_BRAC_GRAV_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STL_BEAM_SEIS_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STL_COLM_SEIS_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STL_BRAC_SEIS_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_MSTR_SEIS_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_RATIO_SEIS_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_RATIO_GRAV_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_STORY_DRIFT: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_LN_MSTR_OUT_OF_PLANE_TB: bSet = IsAnalysisEnable();
		break;
	case ID_MAIN_UPDATE_PO_PROP: bSet = IsPOAnalysisEnable();
		break;
	case ID_2ND_RESULT_TEMP: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_SEIS_PUSHOVER_CURVE: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_RC_PERFORM: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_STL_PFMC: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_MASONRY_PFMC: bSet = IsPOAnalysisEnable();
		break;
	case ID_2ND_RESULT_TB_TEMP: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_TB_RC_BEAM_PFM: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_TB_RC_COLM_PFM: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_TB_RC_WALL_PFM: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_STL_BEAM_PFMC: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_STL_COLM_PFMC: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_STL_PNZN_PFMC: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_STL_BRAC_PFMC: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_TB_RC_CLIF_PFM: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_MASONRY_STRUT_PFMC: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_TB_STORMEMB_PFM: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_RIBBON_SEISEVAL_2ND_TB_STORDRFT_PFM: bSet = IsPOAnalysisEnable();
		break;
	case ID_PO_RESULT_TEMP: bSet = IsPOAnalysisEnable();
		break;
	case ID_PO_RESULT_TB_TEMP: bSet = IsPOAnalysisEnable();
		break;
	case ID_MAIN_SEIS_XY_FACTOR: bSet = IsKISTEC2019();
		break;
	case ID_MAIN_SEIS_XY_FACTOR_TB: bSet = IsKISTEC2019();
		break;
	case ID_MAIN_SEIS_DETAIL_USED_BEAM: bSet = IsAIK2021();
		break;
	case ID_MAIN_SEIS_DETAIL_USED_COLM: bSet = IsAIK2021();
		break;
	case ID_MAIN_SEIS_SPECIAL_BNDR_WALL: bSet = IsAIK2021();
		break;
	case ID_MAIN_SEIS_PLASTIC_LENG_WALL: bSet = IsAIK2021();
		break;
	case ID_MAIN_SEIS_SHEAR_SPAN_BMCL: bSet = IsAIK2021();
		break;
	}

	if (bSet == TRUE) m_ColorData.m_clrText = RGB(0, 0, 0); //SetTextColor(RGB(0,0,0));
	else if (bSet == FALSE) m_ColorData.m_clrText = RGB(192, 192, 192); //SetTextColor(RGB(192,192,192));
	else
		ASSERT(0);

	return TRUE;
}

COLORREF CTreeSeisCtrl::GetBkColor()
{
	//CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//COLORREF Color;
	//if (pDoc != 0 && pDoc->GetDispOpt() != 0)
	//{
	//	if (m_strBkColorName.IsEmpty())
	//	{
	//		COptColorObjNames CC;  
	//		m_strBkColorName = CC.m_TreeMenu_Background;
	//	}
	//
	//	CDispOptColorObj* pColor = pDoc->GetDispOpt()->GetColor(m_strBkColorName);
	//	Color = pColor->GetCurVal(0);
	//}
	//else
	//{
	//	CWinApp* pApp = AfxGetApp(); ASSERT(pApp);
	//	CString csSection, csEntry, csValue;
	//	csSection = _T("DispOpt-Color");
	//	csEntry = _T("Treemenu	Background:Value");
	//	csValue = pApp->GetProfileString(csSection, csEntry, NULL);
	//	Color = RGB(128,128,128);
	//	if (!csValue.IsEmpty()) Color = (COLORREF)_ttoi(csValue);
	//}
	//return Color;
	return RGB(233, 235, 239);
}

COLORREF CTreeSeisCtrl::GetBkColorTitle()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	COLORREF clrColor2;
	if (pDoc != 0 && pDoc->GetDispOpt() != 0)
	{
		if (m_strBkTitleColorName.IsEmpty())
		{
			COptColorObjNames CC;
			m_strBkTitleColorName = CC.m_MV_WorktreeBkTitle;
		}

		CDispOptColorObj* pColor = pDoc->GetDispOpt()->GetColor(m_strBkTitleColorName);
		clrColor2 = pColor->GetCurVal(0);
	}
	else
	{
		CWinApp* pApp = AfxGetApp();
		ASSERT(pApp);
		CString csSection, csEntry, csValue;
		csSection = _T("DispOpt-Color");
		csEntry = _T("Tree Menu	Background Title:Value");
		csValue = pApp->GetProfileString(csSection, csEntry, NULL);
		clrColor2 = RGB(128, 128, 128);
		if (!csValue.IsEmpty()) clrColor2 = (COLORREF)_ttoi(csValue);
	}
	return clrColor2;
}

BOOL CTreeSeisCtrl::IsAnalysisEnable()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	return pDoc->IsPostMode();
}

BOOL CTreeSeisCtrl::IsPOAnalysisEnable()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	return pDoc->m_pPostCtrl->IsPushOverResultEnable();
}

BOOL CTreeSeisCtrl::IsMOE2018()
{
	return (m_nMethod == D_SEME_METHOD_MOE2018);
}

BOOL CTreeSeisCtrl::IsMOE2019()
{
	return (m_nMethod == D_SEME_METHOD_MOE2019);
}

BOOL CTreeSeisCtrl::IsMOE()
{
	return CDBDoc::GetDocPoint()->m_pAttrCtrl2->IsSemeMethod_MOE_Series();
}

BOOL CTreeSeisCtrl::IsKISTEC2019()
{
	return (CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetSemeMethod() == D_SEME_METHOD_KISTEC2019);
}

BOOL CTreeSeisCtrl::IsAIK2021()
{
	return (CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetSemeMethod() == D_SEME_METHOD_AIK_G_001_2021);
}

BOOL CTreeSeisCtrl::InsertTreeItemPhgt(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_PHGT_K, T_PHGT_K>& aPhgtK)
{
	CString str;
	HTREEITEM ThTi = NULL;

	int nPhgt = aPhgtK.GetSize();
	str.Format(_LS(IDS_TMWK_PUSHOVER_DEFINE_PO_HINGE_PROP_d), nPhgt);

	for (int i = 0; i < nPhgt; i++)
	{
		if (!GetPushoverhingetypeString(pDoc, aPhgtK[i], str))
		{
			ASSERT(0);
			continue;
		}
		InsertTreeItem(hTI, ID_PHGT_LST, str, _T(""));
	}

	return TRUE;
}

BOOL CTreeSeisCtrl::InsertTreeItemPhge(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_PHGE_K, T_PHGE_K>& aPhgeK)
{
	CString str;
	HTREEITEM ThTi = NULL;

	int nPhge = aPhgeK.GetSize();
	str.Format(_LS(IDS_TMWK_PUSHOVER_ASSIGN_PO_HINGE_PROP_d), nPhge);

	T_PHGE_D PhgeD;
	CString strName;

	// Wall 타입은 층이름을 함께 표시하기 위해 미리 정보를 생성해 둔다.
	CMapEx<T_ELEM_K, T_ELEM_K, int, int> mElemK_wall;
	mElemK_wall.RemoveAll();
	mElemK_wall.InitHashTable(HASHSIZEELEM);
	for (int i = 0; i < aPhgeK.GetSize(); ++i)
	{
		if (aPhgeK[i].key.type != D_PHGE_WALL) continue;
		if (!pDoc->m_pAttrCtrl->ExistPhge(aPhgeK[i]))
		{
			ASSERT(0);
			continue;
		}

		mElemK_wall.SetAt(aPhgeK[i].key.objK, 0);
	}

	CMap<T_ELEM_K, T_ELEM_K, T_STOR_K, T_STOR_K> mWallElemK2StorK;
	mWallElemK2StorK.RemoveAll();
	CMap<T_STOR_K, T_STOR_K, CString, LPCTSTR> mStorK2Name;
	mStorK2Name.RemoveAll();
	T_STOR_K StorK = 0;
	CString strStorName;
	strStorName.Empty();
	if (mElemK_wall.GetCount() > 0)
	{
		CArray<T_ELEM_K, T_ELEM_K> aElemK_wall;
		mElemK_wall.GetKeyList(aElemK_wall);

		//CStoryData storyData(pDoc);
		CStoryData* pStoryData = pDoc->GetStoryData();
		if (!pStoryData->GetWallElemToStorMap(aElemK_wall, mWallElemK2StorK))
			ASSERT(0);

		T_STOR_D StorD;
		mStorK2Name.RemoveAll();
		mStorK2Name.InitHashTable(HASHSIZESTOR);
		POSITION pos = pDoc->m_pAttrCtrl->GetStartStor();
		while (pos)
		{
			pDoc->m_pAttrCtrl->GetNextStor(pos, StorK, StorD, FALSE);
			mStorK2Name.SetAt(StorK, StorD.StoryName);
		}
	}

	// tree item 추가 
	for (int i = 0; i < nPhge; i++)
	{
		if (!pDoc->m_pAttrCtrl->GetPhge(aPhgeK[i], PhgeD))
		{
			ASSERT(0);
			continue;
		}
		if (!pDoc->m_pAttrCtrl->GetGhngName(aPhgeK[i], strName))
		{
			ASSERT(0);
			continue;
		}

		if (aPhgeK[i].key.type == D_PHGE_WALL)
		{
			T_ELEM_D ElemD;
			T_ELEM_K ElemK = aPhgeK[i].key.objK;

			if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
			if (!pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp)) continue;

			if (!mWallElemK2StorK.Lookup(ElemK, StorK)) StorK = 0;
			if (!mStorK2Name.Lookup(StorK, strStorName)) strStorName = _T("");

			const T_PHGT_D* pPhgt = pDoc->m_pAttrCtrl->GetPhgtConstPtr(PhgeD.HingeTypeK);
			if (pPhgt == nullptr) continue;

			CString strTemp;
			strTemp.Format(_T("W%d_%s-ID%d_%s"), aPhgeK[i].key.objK, strStorName, ElemD.nWallId, pPhgt->strName);
			strName = strTemp;
		}

		str.Format(_T("%s "), strName);
		//if(!PhgeD.bUpdateProperties) str += _LS(IDS_TMWK_PUSHOVER_Disconnected);
		//if(!PhgeD.bUpdateProperties) str += _T("(*)");
		InsertTreeItem(hTI, ID_PHGE_LST, str, _T(""));
	}

	return TRUE;
}

BOOL CTreeSeisCtrl::InsertTreeItemIehp(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_IEHP_K, T_IEHP_K>& aIehpK)
{
	CString str;
	HTREEITEM ThTi = NULL;

	int nIehp = aIehpK.GetSize();
	str.Format(_LS(IDS_TW_INELATIC_HINGE_PROPERTIES_d), nIehp);

	for (int i = 0; i < nIehp; i++)
	{
		if (!GetInelastichingetypeString(pDoc, aIehpK[i], str))
		{
			ASSERT(0);
			continue;
		}
		InsertTreeItem(hTI, ID_IEHP_LST, str, _T(""));
	}

	return TRUE;
}

BOOL CTreeSeisCtrl::InsertTreeItemIehg(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_IEHG_K, T_IEHG_K>& aIehgK)
{
	CString str;
	HTREEITEM ThTi = NULL;

	int nIehg = aIehgK.GetSize();
	str.Format(_LS(IDS_TW_INELATIC_HINGES_d), nIehg);

	// Wall 타입은 층이름을 함께 표시하기 위해 미리 정보를 생성해 둔다.
	T_IEHG_D IehgD;
	CMapEx<T_ELEM_K, T_ELEM_K, int, int> mElemK_wall;
	mElemK_wall.RemoveAll();
	mElemK_wall.InitHashTable(HASHSIZEELEM);
	for (int i = 0; i < nIehg; ++i)
	{
		pDoc->m_pAttrCtrl->GetIehg(aIehgK[i], IehgD);

		const T_IEHP_D* pIehp = pDoc->m_pAttrCtrl->GetIehpConstPtr(IehgD.IehpKey);
		if (pIehp == nullptr) continue;
		if (pIehp->nHingeType != 4) continue; // Wall 인가?

		mElemK_wall.SetAt(aIehgK[i].key.objK, 0);
	}

	CMap<T_ELEM_K, T_ELEM_K, T_STOR_K, T_STOR_K> mWallElemK2StorK;
	mWallElemK2StorK.RemoveAll();
	CMap<T_STOR_K, T_STOR_K, CString, LPCTSTR> mStorK2Name;
	mStorK2Name.RemoveAll();
	T_STOR_K StorK = 0;
	CString strStorName;
	strStorName.Empty();
	if (mElemK_wall.GetCount() > 0)
	{
		CArray<T_ELEM_K, T_ELEM_K> aElemK_wall;
		mElemK_wall.GetKeyList(aElemK_wall);

		//CStoryData storyData(pDoc);
		CStoryData* pStoryData = pDoc->GetStoryData();
		if (!pStoryData->GetWallElemToStorMap(aElemK_wall, mWallElemK2StorK))
			ASSERT(0);

		T_STOR_D StorD;
		mStorK2Name.RemoveAll();
		mStorK2Name.InitHashTable(HASHSIZESTOR);
		POSITION pos = pDoc->m_pAttrCtrl->GetStartStor();
		while (pos)
		{
			pDoc->m_pAttrCtrl->GetNextStor(pos, StorK, StorD, FALSE);
			mStorK2Name.SetAt(StorK, StorD.StoryName);
		}
	}

	for (int i = 0; i < nIehg; i++)
	{
		if (!GetInelastichingeAssignString(pDoc, aIehgK[i], mStorK2Name, mWallElemK2StorK, str))
		{
			ASSERT(0);
			continue;
		}
		InsertTreeItem(hTI, ID_IEHG_LST, str, _T(""));
	}

	return TRUE;
}

BOOL CTreeSeisCtrl::InsertTreeItemFimp(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_FIMP_K, T_FIMP_K>& aFimpK,
                                       BOOL bPushover /*= FALSE*/)
{
	CString str;
	HTREEITEM ThTi = NULL;

	int nList = ID_FIMP_LST;
	if (bPushover) nList = ID_FIMP_PO_LST;
	int nFimp = aFimpK.GetSize();
	str.Format(_LS(IDS_TM_WORKTREE_Fiber_Material_Property_d), nFimp);

	for (int i = 0; i < nFimp; i++)
	{
		if (!GetInelasticFiberTypeString(pDoc, aFimpK[i], str))
		{
			ASSERT(0);
			continue;
		}
		InsertTreeItem(hTI, nList, str, _T(""));
	}

	return TRUE;
}

BOOL CTreeSeisCtrl::InsertTreeItemFibr(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_FIBR_K, T_FIBR_K>& aFibrK,
                                       BOOL bPushover /*= FALSE*/)
{
	CString str;
	HTREEITEM ThTi = NULL;

	int nFibr = aFibrK.GetSize();
	str.Format(_LS(IDS_TM_WORKTREE_Fiber_Division_Section_d), nFibr);

	int nList = ID_FIBR_LST;
	if (bPushover) nList = ID_FIBR_PO_LST;

	for (int i = 0; i < nFibr; i++)
	{
		T_FIBR_D FibrD;
		T_SECT_D SectD;
		T_FIMP_D FimpD[3];

		if (!pDoc->m_pAttrCtrl->GetFibr(aFibrK[i], FibrD))
			ASSERT(0);
		if (!pDoc->m_pAttrCtrl->GetSect(FibrD.SectKey, SectD))
			ASSERT(0);
		if (!pDoc->m_pAttrCtrl->GetFimp(FibrD.FimpKey[0], FimpD[0]))
			ASSERT(0);
		if (!pDoc->m_pAttrCtrl->GetFimp(FibrD.FimpKey[1], FimpD[1]))
			ASSERT(0);
		if (!pDoc->m_pAttrCtrl->GetFimp(FibrD.FimpKey[2], FimpD[2]))
			ASSERT(0);

		str.Format(_LS(IDS_TM_WORKTREE_Fiber_Division_Info),
		           FibrD.strName, SectD.SName, FimpD[0].strName, FimpD[1].strName, FimpD[2].strName);

		InsertTreeItem(hTI, nList, str, _T(""));
	}

	return TRUE;
}

BOOL CTreeSeisCtrl::InsertTreeItemFibw(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_FIBW_K, T_FIBW_K>& aFibwK,
                                       BOOL bPushover /*= FALSE*/)
{
	CString str;
	HTREEITEM ThTi = NULL;

	int nFibw = aFibwK.GetSize();
	str.Format(_LS(IDS_TM_WORKTREE_Fiber_Division_Section_Wall_d), nFibw);

	int nList = ID_FIBW_LST;
	if (bPushover) nList = ID_FIBW_PO_LST;

	T_FIBW_D FibwD;
	T_STOR_D StorD;
	T_FIMP_D FimpD[3];
	for (int i = 0; i < nFibw; i++)
	{
		if (!pDoc->m_pAttrCtrl->GetFibw(aFibwK[i], FibwD))
			ASSERT(0);
		if (!pDoc->m_pAttrCtrl->GetStor(FibwD.StorK, StorD))
			ASSERT(0);
		if (!pDoc->m_pAttrCtrl->GetFimp(FibwD.FimpKey[0], FimpD[0]))
			ASSERT(0);
		if (!pDoc->m_pAttrCtrl->GetFimp(FibwD.FimpKey[1], FimpD[1]))
			ASSERT(0);
		if (!pDoc->m_pAttrCtrl->GetFimp(FibwD.FimpKey[2], FimpD[2]))
			ASSERT(0);

		str.Format(_LS(IDS_TM_WORKTREE_Fiber_Division_Info_Wall),
		           FibwD.strName, FibwD.nWallID, StorD.StoryName, FimpD[0].strName, FimpD[1].strName, FimpD[2].strName);

		InsertTreeItem(hTI, nList, str, _T(""));
	}
	return TRUE;
}

BOOL CTreeSeisCtrl::InsertTreeItemPolc(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_POLC_K, T_POLC_K>& aPolcK)
{
	CString str;

	int nPolc = aPolcK.GetSize();
	str.Format(_LS(IDS_TMWK_PUSHOVER_LOADCASE_d), nPolc);

	CString aIncMethod[] = {_LS(IDS_TMWK_PUSHOVER_FORCE), _LS(IDS_TMWK_PUSHOVER_DISPLACEMENT)};
	T_POLC_D PolcD;
	for (int i = 0; i < nPolc; i++)
	{
		if (!pDoc->m_pAttrCtrl->GetPolc(aPolcK[i], PolcD))
		{
			ASSERT(0);
			continue;
		}
		ASSERT(PolcD.nIncrementMethod == 0 || PolcD.nIncrementMethod == 1);
		str.Format(_T("%s [ %s ]"), PolcD.strName, aIncMethod[PolcD.nIncrementMethod]);
		InsertTreeItem(hTI, ID_POLC_LST, str, _T(""));
	}

	return TRUE;
}


BOOL CTreeSeisCtrl::GetPushoverhingetypeString(CDBDoc* pDoc, T_PHGT_K PhgtK, CString& strName)
{
	CString aElemType[] = {
		_LS(IDS_TW_INELATIC_Beam_Column), _LS(IDS_TMWK_NC_MSG14), _LS(IDS_TM_Truss), _LS(IDS_TMTB_Nonlinear_Link),
		_LS(IDS_TM_GROUP_Point_Spring_Support)
	};
	CString aWallType[] = {_LS(IDS_TW_INELATIC_Membrane),_LS(IDS_TMWK_NC_MSG9)};
	CString aMaterial[] = {_LS(IDS_TMWK_GRID_RC), _LS(IDS_TMWK_GRID_Steel), _LS(IDS_TM_WORKTREE_Masonry)};
	CString aInteraction[] = {_LS(IDS_TW_INELATIC_NONE), _LS(IDS_TW_PUSHOVER_PMM), _LS(IDS_TW_PUSHOVER_PM)};

	strName.Empty();

	const T_PHGT_D* pPhgt = pDoc->m_pAttrCtrl->GetPhgtConstPtr(PhgtK);
	if (pPhgt == nullptr)
	{
		ASSERT(0);
		return FALSE;
	}
	if (pPhgt->nElemType == 0) // Beam/Column
	{
		strName.Format(_T("%s [ %s ; %s ; %s"), pPhgt->strName, aElemType[pPhgt->nElemType],
		               aMaterial[pPhgt->nMaterial], aInteraction[pPhgt->nInteractionType]);
	}
	else if (pPhgt->nElemType == 1) // Wall
		strName.Format(_T("%s [ %s ; %s ; %s ; %s"), pPhgt->strName, aElemType[pPhgt->nElemType],
		               aWallType[pPhgt->nWallType], aMaterial[pPhgt->nMaterial], aInteraction[pPhgt->nInteractionType]);
	else if (pPhgt->nElemType == 2) // Truss
		strName.Format(_T("%s [ %s ; %s"), pPhgt->strName, aElemType[pPhgt->nElemType], aMaterial[pPhgt->nMaterial]);
	else if (pPhgt->nElemType == 3 || pPhgt->nElemType == 4) // General Link, Point Spring Support
		strName.Format(_T("%s [ %s "), pPhgt->strName, aElemType[pPhgt->nElemType]);

	if (pPhgt->nElemType == 2) // Truss
	{
		strName += _T(" ]");
	}
	else if (pPhgt->nElemType == 3 || pPhgt->nElemType == 4) // General Link
	{
		strName += _T(" ; ") + _LS(IDS_TW_INELATIC_Dir) + _T("=");
		for (int i = 0; i < 6; ++i) strName += (pPhgt->bDOF[i] ? '1' : '0');
		strName += _T(" ]");
	}
	else
	{
		const CString strSqBracketEnd = _T(" ]");
		if (pPhgt->nMaterial != 2 && pPhgt->nDefinition == 2)
		{
			CString strTmp;
			strName += _T(" ; ") + _LS(IDS_TW_INELATIC_Num) + _T("=");
			for (int i = 0; i < 6; ++i)
			{
				strTmp.Format(_T("%d"), (pPhgt->bDOF[i] ? pPhgt->nLoc[i] : 0));
				strName += strTmp;
			}
			strName += strSqBracketEnd;
		}
		else
		{
			strName += _T(" ; ") + _LS(IDS_TW_INELATIC_Dir) + _T("=");
			for (int i = 0; i < 4; ++i) strName += (pPhgt->bDOF[i] ? '1' : '0');
			for (int j = 4; j < 6; ++j)
			{
				if (!pPhgt->bDOF[j]) strName += '0';
				else if (pPhgt->nLoc[j] == 1) strName += _T("I");
				else if (pPhgt->nLoc[j] == 2) strName += _T("J");
				else if (pPhgt->nLoc[j] == 3) strName += _T("A");
			}
			strName += strSqBracketEnd;
		}

		// Wall-Membrane-Fiber일 때는 Mz성분은 UI에서는 미사용으로 표시. 실제 솔버에는 사용함으로 출력함.
		BOOL bWallConsOut = FALSE;
		double dShearR = 0.0;
		pDoc->m_pAttrCtrl2->GetPhgtFibrAutoVal(*pPhgt, bWallConsOut, dShearR);
		if (pPhgt->nElemType == D_PHGT_ELEM_WALL && (pPhgt->nWallType == 0 || !bWallConsOut) && pPhgt->nHingeType == 1)
		{
			int nOffset = strSqBracketEnd.GetLength() + 1;
			strName.SetAt(strName.GetLength() - nOffset, '0');
		}
	}

	return TRUE;
}

BOOL CTreeSeisCtrl::GetInelastichingetypeString(CDBDoc* pDoc, T_IEHP_K IehpK, CString& strName)
{
	strName.Empty();
	const T_IEHP_D* pIehp = pDoc->m_pAttrCtrl->GetIehpConstPtr(IehpK);
	if (pIehp == nullptr) return FALSE;

	ASSERT(pIehp->nInteractionType >= 0 && pIehp->nInteractionType <= 3);

	CString strAdd;
	strAdd.Empty();
	CString aInteraction[] = {
		_LS(IDS_TW_INELATIC_NONE), _LS(IDS_TW_INELATIC_PM), _LS(IDS_TW_INELATIC_PMM), _LS(IDS_TW_INELATIC_PM_Multi)
	};

	for (int k = 0; k < 6; k++) strAdd += pIehp->cDir[k];

	// Wall-Membrane-Fiber일 때는 Mz성분은 UI에서는 미사용으로 표시. 실제 솔버에는 사용함으로 출력함.
	int nLocation = 0;
	BOOL bWallConsOut = FALSE;
	double dShearR = 0.0;
	pDoc->m_pAttrCtrl2->GetIehpFibrAutoVal(*pIehp, nLocation, bWallConsOut, dShearR);
	if (pIehp->nHingeType == D_IEHP_HTYP_WALL && (pIehp->nWallType == 0 || !bWallConsOut) && pIehp->nDefinition == 1)
	{
		strAdd.SetAt(strAdd.GetLength() - 1, '0');
	}

	if (pIehp->nHingeType == 0) // lumped
	{
		strName.Format(_LS(IDS_TW_INELATIC_HINGE_PROPERTIES_INFO1),
		               pIehp->strName, aInteraction[pIehp->nInteractionType], strAdd);
	}
	else if (pIehp->nHingeType == 1) // distributed
	{
		if (pIehp->nDefinition == 0)
			strName.Format(_LS(IDS_TW_INELATIC_HINGE_PROPERTIES_INFO2),
			               pIehp->strName, aInteraction[pIehp->nInteractionType], strAdd);
		else if (pIehp->nDefinition == 1)
			strName.Format(_LS(IDS_TW_INELATIC_HINGE_PROPERTIES_INFO3), pIehp->strName, strAdd);
		else
			ASSERT(0);
	}
	else if (pIehp->nHingeType == 2) // spring
	{
		strName.Format(_LS(IDS_TW_INELATIC_HINGE_PROPERTIES_INFO4), pIehp->strName, strAdd);
	}
	else if (pIehp->nHingeType == 3) // truss
	{
		strName.Format(_LS(IDS_TW_INELATIC_HINGE_PROPERTIES_INFO5), pIehp->strName, strAdd);
	}
	else if (pIehp->nHingeType == 4) // Wall
	{
		strName.Format(_LS(IDS_TW_INELATIC_HINGE_PROPERTIES_INFO6), pIehp->strName, strAdd);
	}
	else if (pIehp->nHingeType == D_IEHP_HTYP_POINTSPRING)
	{
		strName.Format(_LS(IDS_TW_INELATIC_HINGE_PROPERTIES_INFO7), pIehp->strName, strAdd);
	}
	else
		ASSERT(0);

	return TRUE;
}

BOOL CTreeSeisCtrl::GetInelastichingeAssignString(CDBDoc* pDoc, T_IEHG_K IehgK,
                                                  CMap<T_STOR_K, T_STOR_K, CString, LPCTSTR>& mStorK2Name,
                                                  CMap<T_ELEM_K, T_ELEM_K, T_STOR_K, T_STOR_K>& mWallElemK2StorK,
                                                  CString& strName)
{
	T_IEHG_D IehgD;
	pDoc->m_pAttrCtrl->GetIehg(IehgK, IehgD);
	const T_IEHP_D* pIehp = pDoc->m_pAttrCtrl->GetIehpConstPtr(IehgD.IehpKey);
	if (pIehp == nullptr) return FALSE;

	CString strHingeType;
	if (pIehp->nHingeType == 0 || pIehp->nHingeType == 1) strHingeType = _LS(IDS_TW_INELASTIC_BEAM);
	else if (pIehp->nHingeType == 2) strHingeType = _LS(IDS_TW_INELASTIC_GENERAL_LINK);
	else if (pIehp->nHingeType == 3) strHingeType = _LS(IDS_TW_INELASTIC_TRUSS);
	else if (pIehp->nHingeType == 4) strHingeType = _LS(IDS_TW_INELASTIC_WALL);

	T_STOR_K StorK = 0;
	CString strStorName;
	strStorName.Empty();
	if (pIehp->nHingeType == 4)
	{
		T_ELEM_D ElemD;
		T_ELEM_K ElemK = IehgK.key.objK;

		if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) return FALSE;
		if (!pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp)) return FALSE;;

		if (!mWallElemK2StorK.Lookup(ElemK, StorK)) StorK = 0;
		if (!mStorK2Name.Lookup(StorK, strStorName)) strStorName = _T("");

		strName.Format(_T("%s%d_%s-%s%d_%s"), strHingeType, IehgK, strStorName, _T("ID"), ElemD.nWallId,
		               pIehp->strName);
	}
	else
	{
		strName.Format(_T("%s%d_%s"), strHingeType, IehgK, pIehp->strName);
	}

	return TRUE;
}

BOOL CTreeSeisCtrl::GetInelasticFiberTypeString(CDBDoc* pDoc, T_FIMP_K FimpK, CString& strName)
{
	T_FIMP_D FimpD;
	pDoc->m_pAttrCtrl->GetFimp(FimpK, FimpD);
	if (FimpD.nMaterialType == 0) // concrete
	{
		if (FimpD.nHysModelType == 0)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_CONC_KENTPK), FimpD.strName, FimpD.PROP.CON_KENTPK.dFc);
		else if (FimpD.nHysModelType == 1)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_CONC_STANJP), FimpD.strName, FimpD.PROP.CON_STANJP.dFc);
		else if (FimpD.nHysModelType == 2)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_CONC_ROADJP), FimpD.strName, FimpD.PROP.CON_ROADJP.dSigmack);
		else if (FimpD.nHysModelType == 3)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_CONC_NAGOYA), FimpD.strName, FimpD.PROP.CON_NAGOYA.dSigmack);
		else if (FimpD.nHysModelType == 4)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_CONC_TRILIN), FimpD.strName, FimpD.PROP.CON_TRILIN.dSigmay[0]);
		else if (FimpD.nHysModelType == 5)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_CONC_GBCHINA), FimpD.strName, FimpD.PROP.CON_GB1002.dFck);
		else if (FimpD.nHysModelType == 6)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_CONC_MANDER), FimpD.strName);
		else if (FimpD.nHysModelType == 7)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_CONC_ROADJP_H24), FimpD.strName,
			               FimpD.PROP.CON_ROADJP_H24.dSigmack);
		else if (FimpD.nHysModelType == 8)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_CONC_GBCHINA2010), FimpD.strName, FimpD.PROP.CON_GB1010.dFcr);
	}
	else if (FimpD.nMaterialType == 1) // steel
	{
		if (FimpD.nHysModelType == 0)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_STL_MENEGO), FimpD.strName, FimpD.PROP.STL_MENEGO.dFy);
		else if (FimpD.nHysModelType == 1)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_STL_BILINE), FimpD.strName, FimpD.PROP.STL_BILINE.dFy);
		else if (FimpD.nHysModelType == 2)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_STL_GENBIL), FimpD.strName, FimpD.PROP.STL_GENBIL.dSigmay);
		else if (FimpD.nHysModelType == 3)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_STL_TRILIN), FimpD.strName, FimpD.PROP.STL_TRILIN.dSigmay[0]);
		else if (FimpD.nHysModelType == 4)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_STL_PARK), FimpD.strName, FimpD.PROP.STL_PARK.dFy);
		else if (FimpD.nHysModelType == 5)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_STL_JBRIDGE), FimpD.strName, FimpD.PROP.STL_ROADJP.dSigsy);
		else if (FimpD.nHysModelType == 6)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_STL_GBCHINA2010), FimpD.strName, FimpD.PROP.STL_GB2010.dFyr);
		else if (FimpD.nHysModelType == D_FIMP_CURV_STEL_ENRGDS)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_STL_ENRGDS), FimpD.strName);
	}
	else if (FimpD.nMaterialType == 2) // rebar
	{
		if (FimpD.nHysModelType == 6)
			strName.Format(_LS(IDS_TM_WORKTREE_FIMP_STL_GBCHINA2010), FimpD.strName, FimpD.PROP.STL_GB2010.dFyr);
	}
	else
		ASSERT(0);

	return TRUE;
}

HTREEITEM CTreeSeisCtrl::SetMainCtrlItemTree()
{
	HTREEITEM hTI;
	HTREEITEM hT2;
	hTI = InsertTreeItem(m_hRootTi, 0, _LS(IDS_TREE_SEIS_MAIN_CTRL),
	                     _T("ico24_seismicperformance_maincontrol_evaluationmethod"));
	{
		InsertTreeItem(hTI, ID_MAIN_RIBBON_SEISEVAL_METHOD, _LS(IDS_TREE_SEIS_EVAL_METHOD),
		               _T("ico24_seismicperformance_maincontrol_evaluationmethod"));
		hT2 = InsertTreeItem(hTI, 0, _LS(IDS_MAIN_RIBBON_MENU_SEIS_DEFINE_MATL),
		                     _T("ico24_seismicperformance_maincontrol_definematerial"));
		{
			InsertTreeItem(hT2, ID_MAIN_RIBBON_SEISEVAL_RC_MATERIAL, _LS(IDS_TREE_SEIS_CONC_MATL),
			               _T("ico24_seismicperformance_maincontrol_concretematerial"));
			InsertTreeItem(hT2, ID_MAIN_RIBBON_SEISEVAL_STEEL_MATERIAL, _LS(IDS_MAIN_RIBBON_MENU_SEIS_STEEL_MATL),
			               _T("ico24_seismicperformance_maincontrol_steelmaterial"));
			InsertTreeItem(hT2, ID_MAIN_RIBBON_SEISEVAL_MASONRY_MATERIAL, _LS(IDS_MAIN_RIBBON_MENU_SEIS_MASONRY_MATL),
			               _T("ico24_seismicperformance_maincontrol_masonrymaterial"));
		}
	}

	return hTI;
}

HTREEITEM CTreeSeisCtrl::SetEffeStiffItemTree()
{
	HTREEITEM hTI;

	hTI = InsertTreeItem(m_hRootTi, 0, _LS(IDS_TREE_SEIS_EFFECT_STIFF),
	                     _T("ico24_seismicperformance_effectivestiffness_generatesectioneffectivestiffness"));
	{
		InsertTreeItem(hTI, ID_MAIN_RIBBON_SEISEVAL_STIFF_GENERATE, _LS(IDS_TREE_SEIS_GENER_SECT_STIFF),
		               _T("ico24_seismicperformance_effectivestiffness_generatesectioneffectivestiffness"));
		InsertTreeItem(hTI, ID_MAIN_RIBBON_SEISEVAL_STIFF_SET_LC, _LS(IDS_TREE_SEIS_SET_LC_CHECK),
		               _T("ico24_seismicperformance_effectivestiffness_setloadcaseforcheck"));
		InsertTreeItem(hTI, ID_MAIN_RIBBON_SEISEVAL_STIFF_CHK_SECTION, _LS(IDS_TREE_SEIS_CHK_SECT_STIFF),
		               _T("ico24_seismicperformance_effectivestiffness_checksectioneffectivestiffness"));
	}

	return hTI;
}

HTREEITEM CTreeSeisCtrl::SetPropertyItemTree()
{
	HTREEITEM hTI;

	hTI = InsertTreeItem(m_hRootTi, 0, _LS(IDS_TREE_SEIS_PROPERTIES),
	                     _T("ico24_properties_section_sectionstiffnessscalefactor"));
	{
		InsertTreeItem(hTI, ID_STRUCT_PROP_SECTION_SFACTOR, _LS(IDS_TREE_SEIS_SECT_SCALE_FACTOR),
		               _T("ico24_properties_section_sectionstiffnessscalefactor"));
		InsertTreeItem(hTI, ID_STRUCT_PROP_ELEM_SFACTOR, _LS(IDS_TM_WORKTREE_Element_Stiffness_Scale_Factor),
		               _T("ico24_properties_section_elementstiffnessscalefactor"));
		InsertTreeItem(hTI, ID_STRUCT_PROP_WSSF, _LS(IDS_TREE_SEIS_WALL_SCALE_FACTOR),
		               _T("ico24_properties_section_wallstiffnessscalefactor"));
	}

	return hTI;
}

HTREEITEM CTreeSeisCtrl::SetGeneralDgnParamItemTree()
{
	HTREEITEM hTI;

	hTI = InsertTreeItem(m_hRootTi, 0, _LS(IDS_TREE_SEIS_GENERAL_DGN_PARAM), _T("ico24_mods_design_commonpara"));
	{
		InsertTreeItem(hTI, ID_DGN_GEN_MEMB, _LS(IDS_TREE_SEIS_MEMBER_ASSIGN),
		               _T("ico24_rating_bridgeratingdesign_memberassignment"));
		InsertTreeItem(hTI, ID_DGN_GEN_SEIS, _LS(IDS_TREE_SEIS_SEIS_LOAD_COMBINATION),
		               _T("ico24_design_general_seismicloadcombinationtype"));
	}

	return hTI;
}

HTREEITEM CTreeSeisCtrl::SetRcDgnParamItemTree()
{
	HTREEITEM hTI;
	HTREEITEM SeTi;

	hTI = InsertTreeItem(m_hRootTi, 0, _LS(IDS_TREE_SEIS_RC_DGN_PARAM), _T("ico24_mods_design_commonpara"));
	{
		InsertTreeItem(hTI, ID_DGN_CON_REBB, _LS(IDS_TREE_SEIS_MODIFY_REBB),
		               _T("ico24_design_design_dropmenu2_modifybeamrebardata"));
		InsertTreeItem(hTI, ID_DGN_CON_REBC, _LS(IDS_TREE_SEIS_MODIFY_REBC),
		               _T("ico24_design_design_dropmenu2_modifycolumnrebardata"));
		InsertTreeItem(hTI, ID_DGN_CON_REBW, _LS(IDS_TREE_SEIS_MODIFY_REBW),
		               _T("ico24_design_design_dropmenu2_modifywallrebardata"));
		InsertTreeItem(hTI, ID_DGN_CON_WMAK, _LS(IDS_TREE_SEIS_MODIFY_WMAK),
		               _T("ico24_design_design_dropmenu2_modifywallmarkdata"));
		SeTi = InsertTreeItem(hTI, 0, _LS(IDS_TREE_SEIS_CONC_DGN_TB), _T("ico24_mods_design_commonpara"));
		{
			InsertTreeItem(SeTi, ID_QUERY_DGN_CON_REBB, _LS(IDS_TREE_SEIS_QUERY_REBB),
			               _T("ico24_design_design_dropmenu2_modifybeamrebardata"));
			InsertTreeItem(SeTi, ID_QUERY_DGN_CON_REBC, _LS(IDS_TREE_SEIS_QUERY_REBC),
			               _T("ico24_design_design_dropmenu2_modifycolumnrebardata"));
			InsertTreeItem(SeTi, ID_QUERY_DGN_CON_REBW, _LS(IDS_TREE_SEIS_QUERY_REBW),
			               _T("ico24_design_design_dropmenu2_modifywallrebardata"));
		}
	}

	return hTI;
}

HTREEITEM CTreeSeisCtrl::SetCommonParameItemTree()
{
	HTREEITEM hTI;

	hTI = InsertTreeItem(m_hRootTi, 0, _LS(IDS_MAIN_RIBBON_MENU_SEIS_COMMON_PARAM),
	                     _T("ico24_seismicperformance_common_commonparameter"));
	{
		switch (m_nMethod)
		{
		case D_SEME_METHOD_KISTEC2013:
			{
				InsertTreeItem(hTI, ID_MAIN_CLEAR_HEIGHT_COLUMN, _LS(IDS_MAIN_RIBBON_MENU_SEIS_CLEAR_HGT_COL),
				               _T("ico24_seismicperformance_common_clearheightofcolumn"));
				InsertTreeItem(hTI, ID_MAIN_USE_SEISMIC_HOOK, _LS(IDS_MAIN_RIBBON_MENU_SEIS_SEISMIC_HOOK),
				               _T("ico24_seismicperformance_common_useseismichook"));
				InsertTreeItem(hTI, ID_MAIN_SEIS_COMPONENT_TYPE, _LS(IDS_MAIN_RIBBON_MENU_SEIS_COMPONENT_TYPE),
				               _T("ico24_seismicperformance_common_seismiccomponenttype"));
			}
			break;
		case D_SEME_METHOD_MOE2018:
			{
				InsertTreeItem(hTI, ID_MAIN_CLEAR_HEIGHT_COLUMN, _LS(IDS_MAIN_RIBBON_MENU_SEIS_CLEAR_HGT_COL),
				               _T("ico24_seismicperformance_common_clearheightofcolumn"));
				InsertTreeItem(hTI, ID_MAIN_USE_SEISMIC_HOOK, _LS(IDS_MAIN_RIBBON_MENU_SEIS_SEISMIC_HOOK),
				               _T("ico24_seismicperformance_common_useseismichook"));
				InsertTreeItem(hTI, ID_MAIN_SEIS_COMPONENT_TYPE, _LS(IDS_MAIN_RIBBON_MENU_SEIS_COMPONENT_TYPE),
				               _T("ico24_seismicperformance_common_seismiccomponenttype"));
				InsertTreeItem(hTI, ID_MAIN_SEIS_REINF_DETAILS, _LS(IDS_MAIN_RIBBON_MENU_SERD),
				               _T("ico24_seismicperformance_common_transversereinforcementdetails"));
			}
			break;
		case D_SEME_METHOD_MOE2019:
		case D_SEME_METHOD_KISTEC2019:
			{
				InsertTreeItem(hTI, ID_MAIN_CLEAR_HEIGHT_COLUMN, _LS(IDS_MAIN_RIBBON_MENU_SEIS_CLEAR_HGT_COL),
				               _T("ico24_seismicperformance_common_clearheightofcolumn"));
				InsertTreeItem(hTI, ID_MAIN_USE_SEISMIC_HOOK, _LS(IDS_MAIN_RIBBON_MENU_SEIS_SEISMIC_HOOK),
				               _T("ico24_seismicperformance_common_useseismichook"));
				InsertTreeItem(hTI, ID_MAIN_SEIS_COMPONENT_TYPE, _LS(IDS_MAIN_RIBBON_MENU_SEIS_COMPONENT_TYPE),
				               _T("ico24_seismicperformance_common_seismiccomponenttype"));
				InsertTreeItem(hTI, ID_MAIN_SEIS_REINF_DETAILS, _LS(IDS_MAIN_RIBBON_MENU_SERD),
				               _T("ico24_seismicperformance_common_transversereinforcementdetails"));
				InsertTreeItem(hTI, ID_MAIN_RC_COLUMN_M_TYPE, _LS(IDS_MAIN_RIBBON_MENU_SEIS_RC_COLUMN_M_TYPE),
				               _T("ico24_seismicperformance_common_modifyrccolumn(m)type"));
				InsertTreeItem(hTI, ID_MAIN_SEIS_PM_CURVE_METHOD, _LS(IDS_MAIN_RIBBON_MENU_PMCM),
				               _T("ico24_seismicperformance_common_pmcurvecalculationmethod"));
			}
			break;
		case D_SEME_METHOD_AIK_G_001_2021:
			{
				InsertTreeItem(hTI, ID_MAIN_CLEAR_HEIGHT_COLUMN, _LS(IDS_MAIN_RIBBON_MENU_SEIS_CLEAR_HGT_COL),
				               _T("ico24_seismicperformance_common_clearheightofcolumn"));
				InsertTreeItem(hTI, ID_MAIN_SEIS_DETAIL_USED_BEAM, _LS(IDS_MAIN_RIBBON_MENU_SEIS_REBAR_DETAIL_BEAM),
				               _T("ico24_seismicperformance_common_seismicrebardetailsforbeam2"));
				InsertTreeItem(hTI, ID_MAIN_SEIS_DETAIL_USED_COLM, _LS(IDS_MAIN_RIBBON_MENU_SEIS_REBAR_DETAIL_COLM),
				               _T("ico24_seismicperformance_common_seismicrebardetailsforbeam2"));
				InsertTreeItem(hTI, ID_MAIN_SEIS_SPECIAL_BNDR_WALL, _LS(IDS_MAIN_RIBBON_MENU_SEIS_SPECIAL_BNDR_WALL),
				               _T("ico24_seismicperformance_common_specialboundaryelementforwall2"));
				InsertTreeItem(hTI, ID_MAIN_SEIS_PLASTIC_LENG_WALL, _LS(IDS_MAIN_RIBBON_MENU_SEIS_PLASTIC_LENG_WALL),
				               _T("ico24_seismicperformance_common_plastichingelengthforfiberwall2"));
				InsertTreeItem(hTI, ID_MAIN_SEIS_SHEAR_SPAN_BMCL, _LS(IDS_MAIN_RIBBON_MENU_SEIS_SHEAR_SPAN_BMCL),
				               _T("ico24_seismicperformance_common_shearspanofbeamcolumn2"));
			}
			break;
		default: ASSERT(0);
			break;
		}
		// Table은 출력하지 않음 - 이해주 과장 요청
	}

	return hTI;
}

HTREEITEM CTreeSeisCtrl::SetInfillStrutItemTree()
{
	HTREEITEM hTI;

	hTI = InsertTreeItem(m_hRootTi, 0, _LS(IDS_MAIN_RIBBON_MENU_SEIS_MASONRY_INFILL_STRUT),
	                     _T("ico24_seismicperformance_masonryinfillstrut_masonryinfillstrut"));
	{
		InsertTreeItem(hTI, ID_MAIN_RIBBON_SEISEVAL_MASONRY_INFILL_STRUT,
		               _LS(IDS_MAIN_RIBBON_MENU_SEIS_DEF_MASONRY_INFILL_STRUT),
		               _T("ico24_seismicperformance_masonryinfillstrut_definemasonryinfillstrut"));
		InsertTreeItem(hTI, ID_MAIN_RIBBON_SEISEVAL_ASSIGN_INFILL_STRUT,
		               _LS(IDS_MAIN_RIBBON_MENU_SEIS_ASSIGN_MASONRY_INFILL_STRUT),
		               _T("ico24_seismicperformance_masonryinfillstrut_assignmasonryinfillstrut"));
		InsertTreeItem(hTI, ID_STRUCT_BNDR_IGNORE, _LS(IDS_MAIN_RIB_MENU_BNDR_IGNORE_ELEMENT),
		               _T("ico24_boundary_etc_ignoreelementsforloadcases"));
		//InsertTreeItem(hTI, ID_MAIN_RIBBON_SEISEVAL_REDUCE_STIFF, _LS(IDS_MAIN_RIBBON_MENU_SEIS_REDUCE_INFILL_STRUT_STIFFNESS), _T(""));
	}

	return hTI;
}

HTREEITEM CTreeSeisCtrl::SetPreItemTree()
{
	HTREEITEM hTI;

	switch (m_nMethod)
	{
	case D_SEME_METHOD_KISTEC2013:
	case D_SEME_METHOD_KISTEC2019:
		SetPreItemTree_KISTEC(hTI);
		break;
	case D_SEME_METHOD_MOE2018:
	case D_SEME_METHOD_MOE2019:
		SetPreItemTree_MOE2018(hTI);
		break;
	case D_SEME_METHOD_AIK_G_001_2021:
		SetPreItemTree_MOE2018(hTI);
		break;
	default:
		ASSERT(0);
		return nullptr;
	}

	return hTI;
}

HTREEITEM CTreeSeisCtrl::Set1stItemTree()
{
	HTREEITEM hTI;
	HTREEITEM SeTi;
	HTREEITEM ThTi;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CSeisEvalDataTool Tool;
	const bool bPreEvalMethod = Tool.IsPreEvalMethod();
	const bool bMasonryMethod = Tool.IsMasonryEvalMethod();

	const CString& strTitle = bPreEvalMethod ? _LS(IDS_TREE_SEIS_1ST) : _LS(IDS_TREE_SEIS_M_FACTOR);
	hTI = InsertTreeItem(m_hRootTi, 0, strTitle, _T("ico24_seismicperformance_evaluationtype_1steval"));
	{
		SeTi = InsertTreeItem(hTI, 0, _LS(IDS_TREE_SEIS_SEIS_LOAD),
		                      _T("ico24_seismicperformance_load_pseudoseismicforce"));
		{
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_PSEUDO_FORCE, _LS(IDS_TREE_SEIS_PSEUDO_SEIS_FORCE),
			               _T("ico24_seismicperformance_load_pseudoseismicforce"));

			ThTi = InsertTreeItem(SeTi, 0, _LS(IDS_TREE_SEIS_RESP_SPECT),
			                      _T("ico24_seismicperform_demand_responsespectrumdata"));
			{
				InsertTreeItem(ThTi, ID_STRUCT_SPEC_DEFINE, _LS(IDS_TREE_SEIS_RS_FUNC),
				               _T("ico24_load_responsespectrumdata_rsfunctions"));
				InsertTreeItem(ThTi, ID_STRUCT_SPEC_ASSIGN, _LS(IDS_TREE_SEIS_RS_LOAD_CASE),
				               _T("ico24_load_responsespectrumdata_rsloadcases"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_RS_MODIFY_FACT, _LS(IDS_TREE_SEIS_MODIFY_FACTOR_CTRL),
				               _T("ico24_seismicperformance_responsespectrum_modificationfactorcontrol"));
			}
		}

		SeTi = InsertTreeItem(hTI, 0, _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_PARAM),
		                      _T("ico24_seismicperformance_misc_parameter"));
		{
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_1ST_RC_M_FACTOR, _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_RC_M_FACT),
			               _T("ico24_seismicperformance_misc_parameter"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_1ST_STL_M_FACTOR,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_STL_M_FACT),
			               _T("ico24_seismicperformance_misc_parameter"));
			if (bMasonryMethod)
			{
				InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_1ST_MASONRY_M_FACTOR,
				               _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_MASONRY_M_FACT),
				               _T("ico24_seismicperformance_misc_parameter"));
			}

			ThTi = InsertTreeItem(SeTi, 0, _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_PARAM_TB),
			                      _T("ico24_seismicperformance_misc_parametertables"));
			{
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_1ST_RC_M_FACTOR_TB,
				               _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_RC_M_FACT),
				               _T("ico24_seismicperformance_misc_parametertables"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_1ST_STL_M_FACTOR_TB,
				               _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_STL_M_FACT),
				               _T("ico24_seismicperformance_misc_parametertables"));
				if (bMasonryMethod)
				{
					InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_1ST_MASONRY_M_FACTOR_TB,
					               _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_MASONRY_M_FACT),
					               _T("ico24_seismicperformance_misc_parametertables"));
				}
			}
		}

		InsertTreeItem(hTI, ID_MAIN_RIBBON_SEISEVAL_LOAD_COMB, _LS(IDS_TREE_SEIS_LOAD_COMBINATION),
		               _T("ico24_load_createloadcases_usingloadcombinations"));

		SeTi = InsertTreeItem(hTI, ID_1ST_RESULT_TEMP, _LS(IDS_TREE_SEIS_1ST_RESULT),
		                      _T("ico24_seismicperformance_result_performance"));
		{
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_1ST_RC_PERFORM, _LS(IDS_TREE_SEIS_1ST_RC_PERFORMANCE),
			               _T("ico24_seismicperformance_result_performance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_1ST_STL_PFMC, _LS(IDS_TREE_SEIS_STL_PERFORMANCE),
			               _T("ico24_seismicperformance_result_performance"));
			if (bMasonryMethod)
			{
				InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_1ST_MASONRY_PFMC, _LS(IDS_WG_GPS_MASONRY_PERFORMANCE),
				               _T("ico24_seismicperformance_result_performance"));
			}
		}

		SeTi = InsertTreeItem(hTI, ID_1ST_RESULT_TB_TEMP, _LS(IDS_TREE_SEIS_1ST_RESULT_TABLE),
		                      _T("ico24_results_tables_resultstables"));
		{
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_MENU_MEMBER_DCR_RCBEAM, _LS(IDS_TREE_SEIS_1ST_RC_BEAM_PERFORM),
			               _T("ico24_seismicperformance_resulttables_rcbeamperformance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_MENU_MEMBER_DCR_RCCOLUMN, _LS(IDS_TREE_SEIS_1ST_RC_COLM_PERFORM),
			               _T("ico24_seismicperformance_resulttables_rccolumnperformance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_MENU_MEMBER_DCR_RCWALL, _LS(IDS_TREE_SEIS_1ST_RC_WALL_PERFORM),
			               _T("ico24_seismicperformance_resulttables_rcwallperformance"));
			if (bMasonryMethod)
			{
				InsertTreeItem(SeTi, ID_MAIN_RIBBON_MENU_MEMBER_DCR_RCCLIF,
				               _LS(IDS_MAIN_RIBBON_MENU_MEMBER_DCR_RC_CLIF),
				               _T("ico24_seismicperformance_resulttables_rccolumnwithmasonryinfillperformance"));
			}
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_1ST_STL_BEAM_PFMC,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_STL_BEAM_Performance),
			               _T("ico24_seismicperformance_resulttables_steelperformance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_1ST_STL_COLM_PFMC,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_STL_COLM_Performance),
			               _T("ico24_seismicperformance_resulttables_steelperformance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_1ST_STL_BRAC_PFMC,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_STL_BRAC_Performance),
			               _T("ico24_seismicperformance_resulttables_steelperformance"));
			if (bMasonryMethod)
			{
				InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_1ST_MASONRY_STRUT_PFMC,
				               _LS(IDS_MAIN_RIBBON_MENU_SEIS_MASONRY_STRUT_Performance),
				               _T("ico24_seismicperformance_resulttables_masonryperformance"));
			}
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_STORY_DCR, _LS(IDS_TREE_SEIS_1ST_STORY_MEMBER_PERFORM),
			               _T("ico24_seismicperformance_resulttables_storymemberperformance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_STORY_DEFORM_DCR,
			               _LS(IDS_TREE_SEIS_1ST_STORY_DRIFT_RATIO_PERFORM),
			               _T("ico24_seismicperformance_resulttables_storydriftratioperformance"));
		}
	}
	return hTI;
}

void CTreeSeisCtrl::SetPreItemTree_KISTEC(HTREEITEM& hTI)
{
	HTREEITEM SeTi;

	hTI = InsertTreeItem(m_hRootTi, 0, _LS(IDS_TREE_SEIS_PRELIMINARY),
	                     _T("ico24_seismicperformance_evaluationtype_preeval"));
	{
		SeTi = InsertTreeItem(hTI, 0, _LS(IDS_TREE_SEIS_CACL_PARAM),
		                      _T("ico24_seismicperformance_calculationparameters_setcalc.parameters"));
		{
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_CALC_PARAM, _LS(IDS_TREE_SEIS_SET_CALC_PARAM),
			               _T("ico24_seismicperformance_calculationparameters_setcalc.parameters"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_RC_COLM_INFO, _LS(IDS_TREE_SEIS_RC_COLM_INFO),
			               _T("seismicperformance_calculationparameters_rccolumninformation"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_RC_WALL_INFO, _LS(IDS_TREE_SEIS_RC_WALL_INFO),
			               _T("ico24_seismicperformance_calculationparameters_rcwallinformation"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_MASONRY_INFO, _LS(IDS_TREE_SEIS_MASONRY_INFO),
			               _T("ico24_seismicperformance_table_masonryinfillwallinformation"));
		}
		SeTi = InsertTreeItem(hTI, 0, _LS(IDS_TREE_SEIS_RESULT_TB), _T("ico24_results_tables_resultstables"));
		{
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_RC_COLM_CAPA_S, _LS(IDS_TREE_SEIS_RC_COLM_CAP_S),
			               _T("seismicperformance_resulttables_rccapacity(s)"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_RC_WALL_CAPA_S, _LS(IDS_TREE_SEIS_RC_WALL_CAP_S),
			               _T("seismicperformance_resulttables_rcwallcapacity(s)"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_RC_COLM_CAPA_G, _LS(IDS_TREE_SEIS_RC_COLM_CAP_G),
			               _T("seismicperformance_calculationparameters_rccolumninformation"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_RC_WALL_CAPA_G, _LS(IDS_TREE_SEIS_RC_WALL_CAP_G),
			               _T("ico24_seismicperformance_calculationparameters_rcwallinformation"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_MASONRY_CAPA, _LS(IDS_TREE_SEIS_MASONRY_CAP),
			               _T("ico24_seismicperformance_resulttables_masonryinfillwallcapacity"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_STORY_PERFORM, _LS(IDS_TREE_SEIS_STORY_PERFOMANCE),
			               _T("ico24_seismicperformance_resulttables_storyperformance"));

			if (IsKISTEC2019())
			{
				InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_LN_MSTR_OUT_OF_PLANE_TB,
				               _LS(IDS_MAIN_RIBBON_MENU_SEIS_LINEAR_MSTR_OUT_OF_PLANE),
				               _T("ico24_seismicperformance_resulttables_overturningcheckofmasonryinfillwalls"));
			}
		}
	}
}

void CTreeSeisCtrl::SetPreItemTree_MOE2018(HTREEITEM& hTI)
{
	HTREEITEM SeTi;
	HTREEITEM ThTi;

	hTI = InsertTreeItem(m_hRootTi, 0, _LS(IDS_TREE_SEIS_LINEAR_EVALUATION),
	                     _T("ico24_seismicperformance_evaluationtype_preeval"));
	{
		SeTi = InsertTreeItem(hTI, 0, _LS(IDS_TREE_SEIS_SEIS_LOAD),
		                      _T("ico24_seismicperformance_load_pseudoseismicforce"));
		{
			InsertTreeItem(SeTi, ID_STRUCT_STATIC_SEISMIC, _LS(IDS_TREE_SEIS_SEISMIC_LOADS),
			               _T("ico24_seismicperformance_load_pseudoseismicforce"));
			ThTi = InsertTreeItem(SeTi, 0, _LS(IDS_TREE_SEIS_RESP_SPECT),
			                      _T("ico24_seismicperform_demand_responsespectrumdata"));
			{
				InsertTreeItem(ThTi, ID_STRUCT_SPEC_DEFINE, _LS(IDS_TREE_SEIS_RS_FUNC),
				               _T("ico24_load_responsespectrumdata_rsfunctions"));
				InsertTreeItem(ThTi, ID_STRUCT_SPEC_ASSIGN, _LS(IDS_TREE_SEIS_RS_LOAD_CASE),
				               _T("ico24_load_responsespectrumdata_rsloadcases"));
			}
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_GROUP, _LS(IDS_TREE_SEIS_EVALUATION_GROUP),
			               _T("ico24_seismicperformance_load_evaluationgroup"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_STORY_CDR_METHOD, _LS(IDS_DB_DT_SCDR),
			               _T("ico24_seismicperformance_load_storycdrmethod"));
		}

		InsertTreeItem(hTI, ID_MAIN_RIBBON_SEISEVAL_LINEAR_LCOM, _LS(IDS_TREE_SEIS_LOAD_COMBINATION),
		               _T("ico24_load_createloadcases_usingloadcombinations"));

		SeTi = InsertTreeItem(hTI, ID_1ST_RESULT_TB_TEMP, _LS(IDS_TREE_SEIS_1ST_RESULT_TABLE),
		                      _T("ico24_results_tables_resultstables"));
		{
			ThTi = InsertTreeItem(SeTi, ID_1ST_RESULT_TB_TEMP, _LS(IDS_TREE_SEIS_RC_CDR),
			                      _T("seismicperformance_resulttables_rcperformance"));
			{
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_RC_BEAM_GRAV_TB, _LS(IDS_TREE_SEIS_RC_BEAM_CDR_G),
				               _T("ico24_seismicperformance_resulttables_rcbeamcdr(g)"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_RC_COLM_GRAV_TB, _LS(IDS_TREE_SEIS_RC_COLUMN_CDR_G),
				               _T("ico24_seismicperformance_resulttables_rccolumncdr(g)"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_RC_WALL_GRAV_TB, _LS(IDS_TREE_SEIS_RC_WALL_CDR_G),
				               _T("ico24_seismicperformance_resulttables_rcwallcdr(g)"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_RC_CLIF_GRAV_TB,
				               _LS(IDS_TREE_SEIS_RC_COLUM_WITH_MASONRY_INFILL_CDR_G),
				               _T("ico24_seismicperformance_resulttables_masonryperformance"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_RC_BEAM_SEIS_TB, _LS(IDS_TREE_SEIS_RC_BEAM_CDR_E),
				               _T("ico24_seismicperformance_resulttables_rcbeamcdr(e)"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_RC_COLM_SEIS_TB, _LS(IDS_TREE_SEIS_RC_COLUMN_CDR_E),
				               _T("ico24_seismicperformance_resulttables_rccolumncdr(g)"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_RC_WALL_SEIS_TB, _LS(IDS_TREE_SEIS_RC_WALL_CDR_E),
				               _T("ico24_seismicperformance_resulttables_rcwallcdr(e)"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_RC_CLIF_SEIS_TB,
				               _LS(IDS_TREE_SEIS_RC_COLUM_WITH_MASONRY_INFILL_CDR_E),
				               _T("ico24_seismicperformance_resulttables_masonryperformance"));
			}
			ThTi = InsertTreeItem(SeTi, ID_1ST_RESULT_TB_TEMP, _LS(IDS_TREE_SEIS_STEEL_CDR),
			                      _T("ico24_seismicperformance_resulttables_steelcdr"));
			{
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_STL_BEAM_GRAV_TB, _LS(IDS_TREE_SEIS_STEEL_BEAM_CDR_G),
				               _T("ico24_seismicperformance_resulttables_steelcdr"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_STL_COLM_GRAV_TB, _LS(IDS_TREE_SEIS_STEEL_COLUMN_CDR_G),
				               _T("ico24_seismicperformance_resulttables_steelcdr"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_STL_BRAC_GRAV_TB, _LS(IDS_TREE_SEIS_STEEL_BRACE_CDR_G),
				               _T("ico24_seismicperformance_resulttables_steelcdr"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_STL_BEAM_SEIS_TB, _LS(IDS_TREE_SEIS_STEEL_BEAM_CDR_E),
				               _T("ico24_seismicperformance_resulttables_steelcdr"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_STL_COLM_SEIS_TB, _LS(IDS_TREE_SEIS_STEEL_COLUMN_CDR_E),
				               _T("ico24_seismicperformance_resulttables_steelcdr"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_STL_BRAC_SEIS_TB, _LS(IDS_TREE_SEIS_STEEL_BRACE_CDR_E),
				               _T("ico24_seismicperformance_resulttables_steelcdr"));
			}
			ThTi = InsertTreeItem(SeTi, ID_1ST_RESULT_TB_TEMP, _LS(IDS_TREE_SEIS_MASONRY_CDR),
			                      _T("ico24_seismicperformance_resulttables_masonryperformance"));
			{
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_MSTR_SEIS_TB,
				               _LS(IDS_TREE_SEIS_MASONRY_INFILL_STRUT_CDR_E),
				               _T("ico24_seismicperformance_resulttables_masonryperformance"));
			}
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_SHEAR_TB, _LS(IDS_TREE_SEIS_LATERAL_STIFFNESS),
			               _T("ico24_seismicperformance_resulttables_lateralstiffness"));

			ThTi = InsertTreeItem(SeTi, ID_1ST_RESULT_TB_TEMP, _LS(IDS_TREE_SEIS_TOTAL_EVALUATION),
			                      _T("ico24_seismicperformance_resulttables_totalevaluation"));
			{
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_RATIO_GRAV_TB, _LS(IDS_TREE_SEIS_TOTAL_EVALUATION_G),
				               _T("ico24_seismicperformance_resulttables_steelcdr"));
				InsertTreeItem(ThTi, ID_MAIN_RIBBON_SEISEVAL_RATIO_SEIS_TB, _LS(IDS_TREE_SEIS_TOTAL_EVALUATION_E),
				               _T("ico24_seismicperformance_resulttables_steelcdr"));
			}
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_STORY_DRIFT, _LS(IDS_TMTB_Story_Drift),
			               _T("ico24_seismicperformance_resulttables_storydriftratioperformance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_LN_MSTR_OUT_OF_PLANE_TB,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_LINEAR_MSTR_OUT_OF_PLANE),
			               _T("ico24_seismicperformance_resulttables_overturningcheckofmasonryinfillwalls"));
		}
	}
}

void CTreeSeisCtrl::SetPreItemTree_AIKG2021(HTREEITEM& hTI)
{
	hTI = InsertTreeItem(m_hRootTi, 0, _T("AIK-G-001-2021 seismic tree"), _T(""));
}

HTREEITEM CTreeSeisCtrl::Set2ndItemTree(CDBDoc* pDoc, BOOL bInit/*=TRUE*/)
{
	HTREEITEM hTI;
	HTREEITEM SeTi;
	HTREEITEM ThTi;
	HTREEITEM TiDefinedHinge;
	HTREEITEM TiAssignedHinge;
	HTREEITEM TiPOGC, TiPOLC;

	CSeisEvalDataTool Tool;
	const bool bPreEvalMethod = Tool.IsPreEvalMethod();
	const bool bMasonryMethod = Tool.IsMasonryEvalMethod();

	const CString& strTitle = bPreEvalMethod ? _LS(IDS_TREE_SEIS_2ND) : _LS(IDS_TREE_SEIS_PUSHOVER);
	hTI = InsertTreeItem(m_hRootTi, 0, strTitle, _T("ico24_seismicperformance_evaluationtype_2ndeval(pushover)"));
	{
		TiPOGC = InsertTreeItem(hTI, ID_STRUCT_PUSHOVER_CONTROL, _LS(IDS_TREE_SEIS_GLOBAL_CTRL),
		                        _T("ico24_pushover_control_globalcontrol"));
		InsertTreeItem(TiPOGC, ID_STRUCT_PUSHOVER_CONTROL, _LS(IDS_MAIN_RIBBON_MENU_Global_Control),
		               _T("ico24_pushover_control_globalcontrol"));
		InsertTreeItem(TiPOGC, ID_STRUCT_BNDR_IEPI, _LS(IDS_DB_DT_IEPI),
		               _T("ico24_pushover_control_ignoreelementsfornlanalysisinitialload"));

		TiPOLC = InsertTreeItem(hTI, ID_STRUCT_PUSHOVER_LOADCASE, _LS(IDS_TREE_SEIS_LOAD_CASE),
		                        _T("ico24_pushover_loadcase_loadcase"));
		InsertTreeItem(hTI, ID_MAIN_INITIAL_PO_PROP, _LS(IDS_TREE_SEIS_AUTO_GENERATE_INIT),
		               _T("ico24_seismicperformance_hingeproperties_updatepushoverhingeproperties"));
		InsertTreeItem(hTI, ID_MAIN_UPDATE_PO_PROP, _LS(IDS_TREE_SEIS_UPDATE),
		               _T("ico24_seismicperformance_hingeproperties_updatepushoverhingeproperties-1"));
		// 		InsertTreeItem(hTI, ID_STRUCT_PUSHOVER_EXPORT_HINGE, _LS(IDS_MAIN_RIBBON_SEISMIC_EXPORT), _T(""));

		TiDefinedHinge = InsertTreeItem(hTI, ID_STRUCT_PUSHOVER_DEFINE_HINGE, _LS(IDS_TREE_SEIS_PO_DEFINE_HINGE),
		                                _T("ico24_seismicperform_misc_bridgeinformation"));
		TiAssignedHinge = InsertTreeItem(hTI, ID_STRUCT_PUSHOVER_ASSIGN, _LS(IDS_TREE_SEIS_PO_ASSIGN_HINGE),
		                                 _T("ico24_seismicperform_misc_bridgeinformation"));
		SeTi = InsertTreeItem(hTI, 0, _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_PARAM),
		                      _T("ico24_seismicperformance_misc_parametertables"));
		{
			InsertTreeItem(SeTi, ID_MAIN_SEIS_XY_FACTOR, _LS(IDS_MAIN_RIBBON_MENU_SEIS_XY_FACT),
			               _T("ico24_seismicperformance_misc_modifygammachifactorforforcecontrolledactionmembers"));

			ThTi = InsertTreeItem(SeTi, 0, _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_PARAM_TB),
			                      _T("ico24_seismicperformance_misc_parametertables"));
			{
				InsertTreeItem(ThTi, ID_MAIN_SEIS_XY_FACTOR_TB, _LS(IDS_MAIN_RIBBON_MENU_SEIS_XY_FACT),
				               _T("ico24_seismicperformance_misc_parametertables"));
			}
		}

		SeTi = InsertTreeItem(hTI, ID_2ND_RESULT_TEMP, _LS(IDS_TREE_SEIS_2ND_RESULT),
		                      _T("ico24_seismicperformance_result_performance"));
		{
			InsertTreeItem(SeTi, ID_MAIN_SEIS_PUSHOVER_CURVE, _LS(IDS_TREE_SEIS_PO_CURVE),
			               _T("ico24_pushover_pushoverresults_pushovercurve"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_RC_PERFORM, _LS(IDS_TREE_SEIS_2ND_RC_PERFORMANCE),
			               _T("ico24_seismicperformance_results_performance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_STL_PFMC, _LS(IDS_TREE_SEIS_STL_PERFORMANCE),
			               _T("ico24_seismicperformance_results_performance"));
			if (bMasonryMethod)
			{
				InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_MASONRY_PFMC,
				               _LS(IDS_MAIN_RIBBON_MENU_SEIS_MASONRY_Performance),
				               _T("ico24_seismicperformance_results_performance"));
			}
		}

		SeTi = InsertTreeItem(hTI, ID_2ND_RESULT_TB_TEMP, _LS(IDS_TREE_SEIS_2ND_RESULT_TABLE),
		                      _T("ico24_results_tables_resultstables"));
		{
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_TB_RC_BEAM_PFM, _LS(IDS_TREE_SEIS_2ND_RC_BEAM_PERFORM),
			               _T("ico24_seismicperformance_resulttables_rcbeamperformance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_TB_RC_COLM_PFM, _LS(IDS_TREE_SEIS_2ND_RC_COLM_PERFORM),
			               _T("ico24_seismicperformance_resulttables_rccolumnperformance"));
			if (!IsAIK2021())
			{
				InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_TB_RC_WALL_PFM, _LS(IDS_TREE_SEIS_2ND_RC_WALL_PERFORM),
				               _T("ico24_seismicperformance_resulttables_rcwallperformance"));
			}
			else
			{
				InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_STRAIN_WALL_PO, _LS(IDS_TREE_PBSD_RC_WALL_PRFM_AXIAL_STRAIN),
				               _T("ico24_seismicperformance_resulttable_rcwallperformance(axialstrain)"));
			}
			if (bMasonryMethod)
			{
				InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_TB_RC_CLIF_PFM,
				               _LS(IDS_MAIN_RIBBON_MENU_MEMBER_DCR_RC_CLIF),
				               _T("ico24_seismicperformance_resulttables_rccolumnwithmasonryinfillperformance"));
			}
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_STL_BEAM_PFMC,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_STL_BEAM_Performance),
			               _T("ico24_seismicperformance_resulttables_steelperformance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_STL_COLM_PFMC,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_STL_COLM_Performance),
			               _T("ico24_seismicperformance_resulttables_steelperformance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_STL_BRAC_PFMC,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_STL_BRAC_Performance),
			               _T("ico24_seismicperformance_resulttables_steelperformance"));
			if (bMasonryMethod)
			{
				InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_MASONRY_STRUT_PFMC,
				               _LS(IDS_MAIN_RIBBON_MENU_SEIS_MASONRY_STRUT_Performance),
				               _T("ico24_seismicperformance_resulttables_masonryperformance"));
			}
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_TB_STORMEMB_PFM,
			               _LS(IDS_TREE_SEIS_2ND_STORY_MEMBER_PERFORM),
			               _T("ico24_seismicperformance_resulttables_storymemberperformance"));
			InsertTreeItem(SeTi, ID_MAIN_RIBBON_SEISEVAL_2ND_TB_STORDRFT_PFM,
			               _LS(IDS_TREE_SEIS_1ST_STORY_DRIFT_RATIO_PERFORM),
			               _T("ico24_seismicperformance_resulttables_storydriftratioperformance"));
		}
	}

	if (!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return hTI;
	}

	// pushover global control
	int nPogd = pDoc->m_pAttrCtrl->ExistPogd() ? 1 : 0;

	// Member Assigned
	T_POGD_D PogdD;
	if (!pDoc->m_pAttrCtrl->GetPogd(PogdD))
		PogdD.Initialize();
	int nMembAssign = PogdD.bAssignByMember == TRUE ? 1 : 0;

	// pushover load case
	CArray<T_POLC_K, T_POLC_K> aPolcK;
	pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
	int nPolc = aPolcK.GetSize();

	// pushover hinge type
	CArray<T_PHGT_K, T_PHGT_K> aPhgtK;
	pDoc->m_pAttrCtrl->GetPhgtKeyList(aPhgtK);
	int nPhgt = aPhgtK.GetSize();

	// pushover hinge
	CArray<T_PHGE_K, T_PHGE_K> aPhgeK;
	pDoc->m_pAttrCtrl->GetPhgeKeyListSolOrd(aPhgeK);
	int nPhge = aPhgeK.GetSize();

	// pushover load case
	if (nPolc > 0)
	{
		if (!InsertTreeItemPolc(pDoc, TiPOLC, aPolcK))
			ASSERT(0);
	}

	// pushover hinge type
	if (nPhgt > 0)
	{
		if (!InsertTreeItemPhgt(pDoc, TiDefinedHinge, aPhgtK))
			ASSERT(0);
	}

	// pushover hinge ; 요소 하나씩 출력되도록
	if (nPhge > 0)
	{
		if (!InsertTreeItemPhge(pDoc, TiAssignedHinge, aPhgeK))
			ASSERT(0);
	}

	return hTI;
}

HTREEITEM CTreeSeisCtrl::SetPBDPOItemTree(CDBDoc* pDoc)
{
	HTREEITEM hTI;
	HTREEITEM SeTi;
	HTREEITEM TiDefinedHinge;
	HTREEITEM TiAssignedHinge;
	HTREEITEM TiPOGC, TiPOLC;
	HTREEITEM TiDEFFiber, TiPropFiber, TiPropFiberWall;

	const CString& strTitle = _LS(IDS_TREE_SEIS_PUSHOVER);
	hTI = InsertTreeItem(m_hRootTi, 0, strTitle, _T("ico24_pushover_control_globalcontrol"));
	{
		TiPOGC = InsertTreeItem(hTI, ID_STRUCT_PUSHOVER_CONTROL, _LS(IDS_TREE_SEIS_GLOBAL_CTRL),
		                        _T("ico24_pushover_control_globalcontrol"));
		InsertTreeItem(TiPOGC, ID_STRUCT_PUSHOVER_CONTROL, _LS(IDS_MAIN_RIBBON_MENU_Global_Control),
		               _T("ico24_pushover_control_globalcontrol"));
		InsertTreeItem(TiPOGC, ID_STRUCT_BNDR_IEPI, _LS(IDS_DB_DT_IEPI),
		               _T("ico24_pushover_control_ignoreelementsfornlanalysisinitialload"));

		TiPOLC = InsertTreeItem(hTI, ID_STRUCT_PUSHOVER_LOADCASE, _LS(IDS_TREE_SEIS_LOAD_CASE),
		                        _T("ico24_pushover_loadcase_loadcase"));
		InsertTreeItem(hTI, ID_MAIN_INITIAL_PO_PROP, _LS(IDS_TREE_SEIS_AUTO_GENERATE_INIT),
		               _T("ico24_seismicperformance_hingeproperties_updatepushoverhingeproperties"));
		InsertTreeItem(hTI, ID_MAIN_UPDATE_PO_PROP, _LS(IDS_TREE_SEIS_UPDATE),
		               _T("ico24_seismicperformance_hingeproperties_updatepushoverhingeproperties-1"));
		InsertTreeItem(hTI, ID_MAIN_SEIS_HINGE_BY_ELASTIC_PO, _LS(IDS_MAIN_RIBBON_MENU_SEIS_HINGE_BY_ELASTIC),
		               _T("ico24_seismicperformance_hingeproperties_updatepushoverhingeproperties-2"));

		TiDefinedHinge = InsertTreeItem(hTI, ID_STRUCT_PUSHOVER_DEFINE_HINGE, _LS(IDS_TREE_SEIS_PO_DEFINE_HINGE),
		                                _T("ico24_seismicperform_misc_bridgeinformation"));
		TiAssignedHinge = InsertTreeItem(hTI, ID_STRUCT_PUSHOVER_ASSIGN, _LS(IDS_TREE_SEIS_PO_ASSIGN_HINGE),
		                                 _T("ico24_seismicperform_misc_bridgeinformation"));

		TiDEFFiber = InsertTreeItem(hTI, ID_MAIN_PUSHOVER_DEFINE_FIMP, _LS(IDS_MAIN_RIBBON_PUSHOVER_DEFINE_FIMP),
		                            _T("ico24_seismicperform_misc_bridgeinformation"));
		TiPropFiber = InsertTreeItem(hTI, ID_STRUCT_PROP_FIBER_DIVISION_PO,
		                             _LS(IDS_MAIN_RIBBON_STRUCT_PROP_FIBER_DIVISION_PO),
		                             _T("ico24_seismicperform_misc_bridgeinformation"));
		TiPropFiberWall = InsertTreeItem(hTI, ID_STRUCT_PROP_FIBER_DIVISION_WALL_PO,
		                                 _LS(IDS_MAIN_RIBBON_STRUCT_PROP_FIBER_DIVISION_PO_WALL),
		                                 _T("ico24_seismicperform_misc_bridgeinformation"));

		SeTi = InsertTreeItem(hTI, ID_2ND_RESULT_TEMP, _LS(IDS_TREE_SEIS_2ND_RESULT),
		                      _T("ico24_seismicperformance_result_performance"));
		{
			InsertTreeItem(SeTi, ID_MAIN_SEIS_PUSHOVER_CURVE, _LS(IDS_TREE_SEIS_PO_CURVE),
			               _T("ico24_pushover_pushoverresults_pushovercurve"));
			InsertTreeItem(SeTi, ID_MAIN_SEIS_LUMPED_HINGE_PFRM_PO,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_PBD_LUMPED_HINGE_PERFORMANCE),
			               _T("ico24_seismicperformance_results_performance"));
			InsertTreeItem(SeTi, ID_MAIN_SEIS_AXIAL_STRAIN_PFRM_PO,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_PBD_AXIAL_STRAIN_PERFORMANCE),
			               _T("ico24_seismicperformance_results_performance"));
			InsertTreeItem(SeTi, ID_MAIN_SEIS_FIBER_HINGE_PFRM_PO,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_PBD_FIBER_MEMBER_PERFORMANCE),
			               _T("ico24_seismicperformance_results_performance"));
			InsertTreeItem(SeTi, ID_MAIN_SEIS_SHEAR_FORCE_PFRM_PO,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_PBD_SHEAR_FORCE_PERFORMANCE),
			               _T("ico24_seismicperformance_results_performance"));
		}

		SeTi = InsertTreeItem(hTI, ID_2ND_RESULT_TB_TEMP, _LS(IDS_TREE_SEIS_2ND_RESULT_TABLE),
		                      _T("ico24_results_tables_resultstables"));
		{
			InsertTreeItem(SeTi, ID_MAIN_LUMPED_HINGE_MEMB_BEAM_PO, _LS(IDS_MAIN_RIBBON_MENU_MEMBER_DCR_RC_BEAM),
			               _T("ico24_seismicperformance_resulttable_rcbeamperformance"));
			InsertTreeItem(SeTi, ID_MAIN_LUMPED_HINGE_MEMB_COLM_PO, _LS(IDS_MAIN_RIBBON_MENU_MEMBER_DCR_RC_COLM),
			               _T("ico24_seismicperformance_resulttable_rccolumnperformance"));

			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_STRAIN_COLM_PO, _LS(IDS_TM_FIBER_HINGE_STRAIN_COLM),
			               _T("ico24_seismicperformance_resulttable_rccolumnperformance-1"));
			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_PANGLE_COLM_PO, _LS(IDS_TM_FIBER_HINGE_PANGLE_COLM),
			               _T("ico24_seismicperformance_resulttable_rccolumnperformance(plasticrotationangle)"));
			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_SHEARF_COLM_PO, _LS(IDS_TM_FIBER_HINGE_SHEARF_COLM),
			               _T("ico24_seismicperformance_resulttable_rccolumnperformance(shearforce)"));
			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_STRAIN_WALL_PO, _LS(IDS_TM_FIBER_HINGE_STRAIN_WALL),
			               _T("ico24_seismicperformance_resulttable_rcwallperformance(axialstrain)"));
			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_PANGLE_WALL_PO, _LS(IDS_TM_FIBER_HINGE_PANGLE_WALL),
			               _T("ico24_seismicperformance_resulttable_rcwallperformance(plasticrotationangle)"));
			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_SHEARF_WALL_PO, _LS(IDS_TM_FIBER_HINGE_SHEARF_WALL),
			               _T("ico24_seismicperformance_resulttable_rcwallperformance(shearforce)"));
			InsertTreeItem(SeTi, ID_MAIN_SEISMIC_PBD_STORY_DRIFT_PO,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_Story_Drift_Ratio_Performance),
			               _T("ico24_seismicperformance_resulttables_storydriftratioperformance"));
			InsertTreeItem(SeTi, ID_MAIN_SEISMIC_PBD_STORY_SHEAR_PO,
			               _LS(IDS_WGENDOC_TB_RESULT_SEIS_PBD_STORY_SHEAR_FORCE),
			               _T("ico24_seismicperformance_resulttables_storydriftratioperformance"));
		}
	}

	if (!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return hTI;
	}

	// pushover global control
	int nPogd = pDoc->m_pAttrCtrl->ExistPogd() ? 1 : 0;

	// Member Assigned
	T_POGD_D PogdD;
	if (!pDoc->m_pAttrCtrl->GetPogd(PogdD))
		PogdD.Initialize();
	int nMembAssign = PogdD.bAssignByMember == TRUE ? 1 : 0;

	// pushover load case
	CArray<T_POLC_K, T_POLC_K> aPolcK;
	pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
	int nPolc = aPolcK.GetSize();

	// pushover hinge type
	CArray<T_PHGT_K, T_PHGT_K> aPhgtK;
	pDoc->m_pAttrCtrl->GetPhgtKeyList(aPhgtK);
	int nPhgt = aPhgtK.GetSize();

	// pushover hinge
	CArray<T_PHGE_K, T_PHGE_K> aPhgeK;
	pDoc->m_pAttrCtrl->GetPhgeKeyListSolOrd(aPhgeK);
	int nPhge = aPhgeK.GetSize();

	// pushover load case
	if (nPolc > 0)
	{
		if (!InsertTreeItemPolc(pDoc, TiPOLC, aPolcK))
			ASSERT(0);
	}

	// pushover hinge type
	if (nPhgt > 0)
	{
		if (!InsertTreeItemPhgt(pDoc, TiDefinedHinge, aPhgtK))
			ASSERT(0);
	}

	// pushover hinge ; 요소 하나씩 출력되도록
	if (nPhge > 0)
	{
		if (!InsertTreeItemPhge(pDoc, TiAssignedHinge, aPhgeK))
			ASSERT(0);
	}

	CArray<T_FIMP_K, T_FIMP_K> aFimpK;
	CArray<T_FIBR_K, T_FIBR_K> aFibrK;
	CArray<T_FIBW_K, T_FIBW_K> aFibwK;
	pDoc->m_pAttrCtrl->GetFimpKeyList(aFimpK);
	pDoc->m_pAttrCtrl->GetFibrKeyList(aFibrK, D_FIBR_TYPE_PHGT);
	pDoc->m_pAttrCtrl->GetFibwKeyList(aFibwK, D_FIBW_TYPE_PHGT);
	BOOL bPOExist = (nPogd + nPolc + nPhgt) > 0;
	int nFimp = bPOExist ? aFimpK.GetSize() : 0;
	int nFibr = aFibrK.GetSize();
	int nFibw = aFibwK.GetSize();
	if (nFimp)
	{
		if (!InsertTreeItemFimp(pDoc, TiDEFFiber, aFimpK, TRUE))
			ASSERT(0);
	}

	if (nFibr > 0)
	{
		if (!InsertTreeItemFibr(pDoc, TiPropFiber, aFibrK, TRUE))
			ASSERT(0);
	}

	if (nFibw > 0)
	{
		if (!InsertTreeItemFibw(pDoc, TiPropFiberWall, aFibwK, TRUE))
			ASSERT(0);
	}

	return hTI;
}

HTREEITEM CTreeSeisCtrl::SetPBDTHItemTree(CDBDoc* pDoc)
{
	HTREEITEM hTI;
	HTREEITEM SeTi;
	HTREEITEM TiPOGC, TiPOLC;
	HTREEITEM TiFuction;
	HTREEITEM TiGround;
	HTREEITEM TiDefinedHinge;
	HTREEITEM TiAssignedHinge;
	HTREEITEM TiDEFFiber, TiPropFiber, TiPropFiberWall;


	// TODO 일단은 Result Table 만 추가
	hTI = InsertTreeItem(m_hRootTi, 0, _LS(IDS_MAIN_RIBBON_MENU_Time_History_Analysis_Data),
	                     _T("ico24_seismicperformance_result_performance"));
	{
		TiPOGC = InsertTreeItem(hTI, ID_TH_GLOBAL_CONTROL, _LS(IDS_MAIN_RIBBON_MENU_Global_Control),
		                        _T("ico24_pushover_control_globalcontrol"));
		InsertTreeItem(TiPOGC, ID_TH_GLOBAL_CONTROL, _LS(IDS_MAIN_RIBBON_MENU_Global_Control),
		               _T("ico24_pushover_control_globalcontrol"));
		InsertTreeItem(TiPOGC, ID_STRUCT_BNDR_IEPI, _LS(IDS_DB_DT_IEPI),
		               _T("ico24_pushover_control_ignoreelementsfornlanalysisinitialload"));

		TiPOLC = InsertTreeItem(hTI, ID_STRUCT_HISTORY_LOADCASE, _LS(IDS_MAIN_RIBBON_MENU_Load_Cases),
		                        _T("ico24_load_timehistoryanalysisdata_loadcases"));
		TiFuction = InsertTreeItem(hTI, ID_STRUCT_TIME_TFUNCTION, _LS(IDS_MAIN_RIBBON_MENU_Time_History_Functions),
		                           _T("ico24_load_timehistoryanalysisdata_timehistoryfunction"));
		TiGround = InsertTreeItem(hTI, ID_STRUCT_TIME_GROUND, _LS(IDS_MAIN_RIBBON_MENU_Ground_n_Acceleration),
		                          _T("ico24_load_timehistoryanalysisdata_groundacceleration"));

		TiDefinedHinge = InsertTreeItem(hTI, ID_STRUCT_ITHA_DEFINE_HINGE, _LS(IDS_MAIN_RIBBON_STRUCT_ITHA_DEFINE_HINGE),
		                                _T("ico24_seismicperform_misc_bridgeinformation"));
		TiAssignedHinge = InsertTreeItem(hTI, ID_STRUCT_ITHA_ASSIGN_HINGE,
		                                 _LS(IDS_MAIN_RIBBON_STRUCT_ITHA_ASSIGN_HINGE),
		                                 _T("ico24_seismicperform_misc_bridgeinformation"));

		TiDEFFiber = InsertTreeItem(hTI, ID_STRUCT_PROP_FIBER_PROP, _LS(IDS_MAIN_RIBBON_STRUCT_PROP_FIBER_PROP),
		                            _T("ico24_seismicperform_misc_bridgeinformation"));
		TiPropFiber = InsertTreeItem(hTI, ID_STRUCT_PROP_FIBER_DIVISION,
		                             _LS(IDS_MAIN_RIBBON_STRUCT_PROP_FIBER_DIVISION_BEAM),
		                             _T("ico24_seismicperform_misc_bridgeinformation"));
		TiPropFiberWall = InsertTreeItem(hTI, ID_STRUCT_PROP_FIBER_DIVISION_WALL,
		                                 _LS(IDS_MAIN_RIBBON_STRUCT_PROP_FIBER_DIVISION_WALL),
		                                 _T("ico24_seismicperform_misc_bridgeinformation"));


		SeTi = InsertTreeItem(hTI, ID_2ND_RESULT_TEMP, _LS(IDS_TREE_SEIS_2ND_RESULT),
		                      _T("ico24_seismicperformance_result_performance"));
		{
			InsertTreeItem(SeTi, ID_MAIN_SEIS_LUMPED_HINGE_PFRM_TH,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_PBD_LUMPED_HINGE_PERFORMANCE),
			               _T("ico24_seismicperformance_result_performance"));
			InsertTreeItem(SeTi, ID_MAIN_SEIS_AXIAL_STRAIN_PFRM_TH,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_PBD_AXIAL_STRAIN_PERFORMANCE),
			               _T("ico24_seismicperformance_result_performance"));
			InsertTreeItem(SeTi, ID_MAIN_SEIS_FIBER_HINGE_PFRM_TH,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_PBD_FIBER_MEMBER_PERFORMANCE),
			               _T("ico24_seismicperformance_result_performance"));
			InsertTreeItem(SeTi, ID_MAIN_SEIS_SHEAR_FORCE_PFRM_TH,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_PBD_SHEAR_FORCE_PERFORMANCE),
			               _T("ico24_seismicperformance_result_performance"));
			InsertTreeItem(SeTi, ID_MAIN_SEIS_STORY_GRAPH_PFRM_TH,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_PBD_STORY_GRAPH_PERFORMANCE),
			               _T("ico24_seismicperformance_result_performance"));
		}

		SeTi = InsertTreeItem(hTI, ID_2ND_RESULT_TB_TEMP, _LS(IDS_MAIN_RIBBON_MENU_SEIS_CVL_Result_Table),
		                      _T("ico24_seismicperformance_result_performance"));
		{
			InsertTreeItem(SeTi, ID_MAIN_LUMPED_HINGE_MEMB_BEAM_TH, _LS(IDS_MAIN_RIBBON_MENU_MEMBER_DCR_RC_BEAM),
			               _T("ico24_seismicperformance_resulttable_rcbeamperformance"));
			InsertTreeItem(SeTi, ID_MAIN_LUMPED_HINGE_MEMB_COLM_TH, _LS(IDS_MAIN_RIBBON_MENU_MEMBER_DCR_RC_COLM),
			               _T("ico24_seismicperformance_resulttable_rccolumnperformance"));

			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_STRAIN_COLM_TH, _LS(IDS_TM_FIBER_HINGE_STRAIN_COLM),
			               _T("ico24_seismicperformance_resulttable_rccolumnperformance"));
			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_PANGLE_COLM_TH, _LS(IDS_TM_FIBER_HINGE_PANGLE_COLM),
			               _T("ico24_seismicperformance_resulttable_rccolumnperformance(plasticrotationangle)"));
			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_SHEARF_COLM_TH, _LS(IDS_TM_FIBER_HINGE_SHEARF_COLM),
			               _T("ico24_seismicperformance_resulttable_rccolumnperformance(shearforce)"));
			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_STRAIN_WALL_TH, _LS(IDS_TM_FIBER_HINGE_STRAIN_WALL),
			               _T("ico24_seismicperformance_resulttable_rcwallperformance(axialstrain)"));
			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_PANGLE_WALL_TH, _LS(IDS_TM_FIBER_HINGE_PANGLE_WALL),
			               _T("ico24_seismicperformance_resulttable_rcwallperformance(plasticrotationangle)"));
			InsertTreeItem(SeTi, ID_MAIN_FIBER_HINGE_SHEARF_WALL_TH, _LS(IDS_TM_FIBER_HINGE_SHEARF_WALL),
			               _T("ico24_seismicperformance_resulttable_rcwallperformance(shearforce)"));

			InsertTreeItem(SeTi, ID_MAIN_SEISMIC_PBD_STORY_DRIFT_TH,
			               _LS(IDS_MAIN_RIBBON_MENU_SEIS_1ST_Story_Drift_Ratio_Performance),
			               _T("ico24_seismicperform_misc_bridgeinformation"));
			InsertTreeItem(SeTi, ID_MAIN_SEISMIC_PBD_STORY_SHEAR_TH,
			               _LS(IDS_WGENDOC_TB_RESULT_SEIS_PBD_STORY_SHEAR_FORCE),
			               _T("ico24_seismicperform_misc_bridgeinformation"));
		}
	}
	if (!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return hTI;
	}

	CArray<T_IEHP_K, T_IEHP_K> aIehpKeyList; // Inelastic Hinge Property
	pDoc->m_pAttrCtrl->GetIehpKeyList(aIehpKeyList);
	int nIehpCount = aIehpKeyList.GetSize();
	// pushover hinge type
	if (nIehpCount > 0)
	{
		if (!InsertTreeItemIehp(pDoc, TiDefinedHinge, aIehpKeyList))
			ASSERT(0);
	}

	CArray<T_IEHG_K, T_IEHG_K> aIehgKeyList; // Inelastic Hinge Assign
	pDoc->m_pAttrCtrl->GetIehgKeyList(aIehgKeyList);
	int nIehgCount = aIehgKeyList.GetSize();
	if (nIehgCount > 0)
	{
		if (!InsertTreeItemIehg(pDoc, TiAssignedHinge, aIehgKeyList))
			ASSERT(0);
	}

	CArray<T_FIMP_K, T_FIMP_K> aFimpKeyList; // Fiber Material Property
	pDoc->m_pAttrCtrl->GetFimpKeyList(aFimpKeyList);
	int nFimpCount = aFimpKeyList.GetSize();
	if (nIehpCount > 0 && nFimpCount > 0)
	{
		if (!InsertTreeItemFimp(pDoc, TiDEFFiber, aFimpKeyList))
			ASSERT(0);
	}

	CArray<T_FIBR_K, T_FIBR_K> aFibrKeyList; // Fiber Division of Section
	pDoc->m_pAttrCtrl->GetFibrKeyList(aFibrKeyList, D_FIBW_TYPE_IEHP);
	int nFibrCount = aFibrKeyList.GetSize();
	if (nFibrCount > 0)
	{
		if (!InsertTreeItemFibr(pDoc, TiPropFiber, aFibrKeyList))
			ASSERT(0);
	}

	CArray<T_FIBW_K, T_FIBW_K> aFibwKeyList; // Fiber Division of Section (MCPC Wall)
	pDoc->m_pAttrCtrl->GetFibwKeyList(aFibwKeyList, D_FIBW_TYPE_IEHP);
	int nFibwCount = aFibwKeyList.GetSize();
	if (nFibwCount > 0)
	{
		if (!InsertTreeItemFibw(pDoc, TiPropFiberWall, aFibwKeyList))
			ASSERT(0);
	}
	return hTI;
}

void CTreeSeisCtrl::UpdateBuffer()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	LockWindowUpdate();
	SetInUpdateBuffer(TRUE);

	BOOL bPushoverItem = FALSE;
	BOOL bSemeItem = FALSE;

	int nModified = 0;

	// 바뀐 Directory 찾기 
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_SEME_ADD):
		case(UR_SEME_DEL):
			bSemeItem = TRUE;
			break;
		case(UR_FIMP_ADD):
		case(UR_FIMP_DEL):
		case(UR_FIMP_MFS):
		case(UR_FIMP_MFD):
		case(UR_FIBR_ADD):
		case(UR_FIBR_DEL):
		case(UR_FIBR_MFS):
		case(UR_FIBR_MFD):
		case(UR_FIBW_ADD):
		case(UR_FIBW_DEL):
		case(UR_FIBW_MFS):
		case(UR_FIBW_MFD):
			bPushoverItem = TRUE;
			break;
		case(UR_POGD_ADD):
		case(UR_POGD_DEL):
		case(UR_POLC_ADD):
		case(UR_POLC_DEL):
		case(UR_POLC_MFS):
		case(UR_POLC_MFD):
		case(UR_POFC_ADD):
		case(UR_POFC_DEL):
		case(UR_POFC_MFS):
		case(UR_POFC_MFD):
		case(UR_PORD_ADD):
		case(UR_PORD_DEL):
		case(UR_PORD_MFS):
		case(UR_PORD_MFD):
		case(UR_PORG_ADD):
		case(UR_PORG_DEL):
		case(UR_PORE_ADD):
		case(UR_PORE_DEL):
		case(UR_PORI_ADD):
		case(UR_PORI_DEL):
		case(UR_POEF_ADD):
		case(UR_POEF_DEL):
			bPushoverItem = TRUE;
			break;
		case(UR_PHGT_ADD):
		case(UR_PHGT_DEL):
		case(UR_PHGT_MFS):
		case(UR_PHGT_MFD):
		case(UR_PHGE_ADD):
		case(UR_PHGE_DEL):
		case(UR_PHGE_MFS):
		case(UR_PHGE_MFD):
			bPushoverItem = TRUE;
			break;
		case(UR_JDCR_ADD):
		case(UR_JDMS_ADD):
		case(UR_GRUP_ADD):
		case(UR_NPLN_ADD):
			Invalidate(FALSE);
			break;
		default:
			break;
		}
	} // end of while

	// Tree의 현재 위치 기억 !!
	HTREEITEM hTIKeep = GetFirstVisibleItem();
	int indx = ItemIndex(hTIKeep);

	if (bSemeItem)
	{
		m_nMethod = pDoc->m_pAttrCtrl2->GetSemeMethod();
	}

	if (bPushoverItem || bSemeItem)
	{
		SetItemTree();
	}

	UnlockWindowUpdate();
	SetInUpdateBuffer(FALSE);
	// Tree의 이전 위치로 복귀 
	hTIKeep = GetItemAt(indx);
	if (hTIKeep != NULL) SelectSetFirstVisible(hTIKeep);
}

void CTreeSeisCtrl::EditProperty_Pushover_PHGE(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos)
{
	CArray<T_PHGE_K, T_PHGE_K> aPhgeK;
	pDoc->m_pAttrCtrl->GetPhgeKeyListSolOrd(aPhgeK);
	T_PHGE_K PhgeK = aPhgeK[pos - 1];
	T_PHGE_D PhgeD;
	T_PHGT_D PhgtD;
	pDoc->m_pAttrCtrl->GetPhge(PhgeK, PhgeD);
	pDoc->m_pAttrCtrl->GetPhgt(PhgeD.HingeTypeK, PhgtD);

	CPO_HingeTypeDlg dlg;
	dlg.m_bModify = TRUE;
	dlg.SetParamData4PhgtData(PhgeK, PhgtD, PhgtD); // 복사한다
	dlg.DoModal();
}

void CTreeSeisCtrl::EditProperty_Pushover_PHGT(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos)
{
	CArray<T_PHGT_K, T_PHGT_K> aPhgtK;
	pDoc->m_pAttrCtrl->GetPhgtKeyList(aPhgtK);
	T_PHGT_D PhgtD;
	pDoc->m_pAttrCtrl->GetPhgt(aPhgtK[pos - 1], PhgtD);

	CPO_HingeTypeDlg dlg;
	dlg.m_bModify = TRUE;
	dlg.SetParamData4Phgt(PhgtD);
	dlg.DoModal();
}

void CTreeSeisCtrl::EditProperty_Pushover_POLC(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos)
{
	CArray<T_POLC_K, T_POLC_K> aPolcK;
	pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);

#if defined (_MEC)
	CreateOrActivateRPSDlg(pDoc, CPO_LoadCaseDlg_MEC::IDD, NULL, &aPolcK[pos - 1]);
#else
	CreateOrActivateRPSDlg(pDoc, CPO_LoadCaseDlg::IDD, NULL, &aPolcK[pos - 1]);
#endif
}
