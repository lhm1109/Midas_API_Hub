#include "stdafx.h"
#include "wg_treemenu.h"
#include "PostTensionDrawTendonLineDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "ElemCreateDlg.h"
#include "ElemTranslateDlg.h"
#include "ElemDeleteDlg.h"
#include "ElemRenumberDlg.h"      
#include "StructElementDlg.h"

#include "PostTensionDrawTendonLineBeamDlg.h"
#include "PostTensionDrawTendonLineSlabDlg.h"
#include "PostTensionTendonElemTranslateDlg.h"
#include "PostTensionTendonElemDeleteDlg.h"
#include "PostTensionTendonElemRenumberDlg.h"
#include "PostTensionTendonProfileMatchDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace mit::frx;

IMPLEMENT_DYNCREATE(CPostTensionDrawTendonLineDlg, MChildFormView)

CPostTensionDrawTendonLineDlg::CPostTensionDrawTendonLineDlg()
	: MChildFormView(CPostTensionDrawTendonLineDlg::IDD)
{
	m_ChildDlgs.RemoveAll();
	m_DlgIDs.RemoveAll();
	m_TableIDs.RemoveAll();

	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Translate_Create_Tendon_Beam));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Translate_Create_Tendon_Slab));
	//m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Translate_Tendon_Slab));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Delete_Tendon_Line));
	//m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Renumbering_Tendon));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Match_Tendon_Profile));

	m_nOrgSlabPlaceHolderHeight = 0;

	CreateChildDlgs();
}

CPostTensionDrawTendonLineDlg::~CPostTensionDrawTendonLineDlg()
{
	DeleteChildDlgs();
}

void CPostTensionDrawTendonLineDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_TENDON_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_TENDON_START_NODE_NUM, m_EditNode);
	DDX_Control(pDX, IDC_TENDON_START_ELEM_NUM, m_EditTdbm);
	//DDX_Control(pDX, IDC_TM_TENDON_CLOSE, m_wndClose);
	//DDX_Control(pDX, IDC_TM_TENDON_EXECUTE, m_wndExecute);
	DDX_Control(pDX, IDC_TM_TENDON_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_TENDON_MENU_COMBO, m_ComboMenu);
	DDX_Control(pDX, IDC_TM_TENDON_START_STC, m_gbStartNum);
}

BEGIN_MESSAGE_MAP(CPostTensionDrawTendonLineDlg, MChildFormView)
	//ON_BN_CLICKED(IDC_TM_TENDON_CLOSE, OnTmClose)
	ON_CBN_SELCHANGE(IDC_TENDON_MENU_COMBO, OnComboMenuSelect)
	//ON_BN_CLICKED(IDC_TM_TENDON_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_VIEW_TENDON_START_NODE_NUM, OnViewStartNodeNum)
	ON_BN_CLICKED(IDC_VIEW_TENDON_START_ELEM_NUM, OnViewStartElemNum)
	ON_BN_CLICKED(IDC_TM_TENDON_TABLE_BTN, OnTmTableBtn)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CPostTensionDrawTendonLineDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CPostTensionDrawTendonLineDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif

void CPostTensionDrawTendonLineDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CPostTensionDrawTendonLineDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1;

	// Draw Tendon Line (Beam)
	m_ChildDlgs.Add(new CPostTensionDrawTendonLineBeamDlg());
	m_DlgIDs.Add(__TMMODE_PT_DRAW_TENDON_LINE_BEAM__);
	m_TableIDs.Add(ID_STRUCT_PTDN_TENDON_TABLE_LINE);

	// Draw Tendon Line (Slab)
	m_ChildDlgs.Add(new CPostTensionDrawTendonLineSlabDlg(this));
	m_DlgIDs.Add(__TMMODE_PT_DRAW_TENDON_LINE_SLAB__);
	m_TableIDs.Add(ID_STRUCT_PTDN_TENDON_TABLE_LINE);

	// Translate Tendon (Slab)
	//m_ChildDlgs.Add(new CPostTensionTendonElemTranslateDlg());
	//m_DlgIDs.Add(__TMMODE_PT_DRAW_TRANS_TENDON__);
	//m_TableIDs.Add(ID_STRUCT_PTDN_TENDON_TABLE_LINE);

	// Delete Tendon (Slab)
	m_ChildDlgs.Add(new CPostTensionTendonElemDeleteDlg());
	m_DlgIDs.Add(__TMMODE_PT_DRAW_DEL_TENDON__);
	m_TableIDs.Add(ID_STRUCT_PTDN_TENDON_TABLE_LINE);

	// Renumbering of Tendon
	//m_ChildDlgs.Add(new CPostTensionTendonElemRenumberDlg());
	//m_DlgIDs.Add(__TMMODE_PT_DRAW_RENUM_TENDON__);
	//m_TableIDs.Add(ID_STRUCT_PTDN_TENDON_TABLE_LINE);

	// Match Tendon Profile
	m_ChildDlgs.Add(new CPostTensionTendonProfileMatchDlg());
	m_DlgIDs.Add(__TMMODE_PT_TENDON_MATCH_PROFILE__);
	m_TableIDs.Add(ID_STRUCT_PTDN_TENDON_PROFILE);
}

void CPostTensionDrawTendonLineDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		delete m_ChildDlgs[i];
	}
}

BOOL CPostTensionDrawTendonLineDlg::ShowChildDlgByID(int nID)
{
	if (nID == __TMMODE_PT_DRAW_TENDON_LINE_BEAM__)
	{
		ShowCurChildDlg(0);
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for (int i = 0; i < nIDCount; i++)
	{
		if (nID == m_DlgIDs[i])
		{
			ShowCurChildDlg(i);
			m_ComboMenu.SetCurSel(i);
			if (m_TableIDs[i] > 0) m_wndTableBtn.EnableWindow(TRUE);
			else m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CPostTensionDrawTendonLineDlg::ShowCurChildDlg(int nDlgIndex)
{
	if (m_nCurChildDlg == nDlgIndex)
		return;

	if (nDlgIndex > m_ChildDlgs.GetSize() - 1)
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	if (m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize() - 1 >= m_nCurChildDlg)
		m_ChildDlgs[m_nCurChildDlg]->End();

	m_ChildDlgs[nDlgIndex]->CreateInit(&m_wndPlaceHolder, 0, 0);
	//m_ChildDlgs[nDlgIndex]->CreateInit(this,0,0);
	m_nCurChildDlg = nDlgIndex;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect, ParentRect, PHRect;

	GetClientRect(&ParentRect);
	m_ChildDlgs[nDlgIndex]->GetWindowRect(&DlgRect);
	//m_ChildDlgs[nDlgIndex]->SetExecButton(&m_wndExecute);
	ScreenToClient(&DlgRect);

	int Sx, Sy, Wx, Wy;
	Sx = (ParentRect.Width() - DlgRect.Width()) / 2;
	Sy = DlgRect.top;

	Wx = DlgRect.Width();
	Wy = DlgRect.Height();

	m_wndPlaceHolder.SetWindowPos(NULL, Sx, Sy, Wx, Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndPlaceHolder.GetWindowRect(&PHRect);

	ScreenToClient(&PHRect);

	/////////////////////////////////////////////////////////////
	// Scroll Size를 조정한다. 
	CSize ScrSize, TSize;
	ScrSize = GetTotalSize();

	TSize.cx = PHRect.Width() - m_RectPlaceHolder.Width();
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();

	ScrSize += TSize;

	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT, ScrSize);

	if (m_nOrgSlabPlaceHolderHeight == 0)
	{
		if (m_DlgIDs[m_nCurChildDlg] == __TMMODE_PT_DRAW_TENDON_LINE_SLAB__)
		{
			ScreenToClient(&m_RectPlaceHolder);
			m_nOrgSlabPlaceHolderHeight = m_RectPlaceHolder.Height();
		}
	}

	//__TMMODE_STRUCT_ELEM_EXTRUDE__
}

int CPostTensionDrawTendonLineDlg::_GetHeightFromFont()
{
	CFont* pNewFont = GetFont();
	CFont* pSysFont, * pOldFont, TempFont;
	CDC* pCdc;
	TEXTMETRIC  tmNew, tmSys;
	int     nTemp, nCorrectVal;

	//get the DC for the edit control
	pCdc = GetDC();

	//get the metrics for the system font
	pSysFont = TempFont.FromHandle(((HFONT)GetStockObject(SYSTEM_FONT)));
	pOldFont = pCdc->SelectObject(pSysFont);
	pCdc->GetTextMetrics(&tmSys);

	//get the metrics for the new font
	pCdc->SelectObject(pNewFont);
	pCdc->GetTextMetrics(&tmNew);

	//select the original font back into the DC and release the DC
	pCdc->SelectObject(pOldFont);
	ReleaseDC(pCdc);

	nCorrectVal = (min(tmNew.tmHeight, tmSys.tmHeight) / 2);
	//calculate the new height for the edit control
	nTemp = tmNew.tmHeight + nCorrectVal;
	//nTemp = tmNew.tmHeight + tmNew.tmInternalLeading;
	return nTemp;
}

void CPostTensionDrawTendonLineDlg::AdjustComboListBox()
{
	CString str;
	CSize   sz;
	int     dx = 0;
	CDC* pDC = m_ComboMenu.GetDC();
	for (int i = 0; i < m_ComboMenu.GetCount(); i++)
	{
		m_ComboMenu.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_ComboMenu.ReleaseDC(pDC);

	//  dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);

	if (m_ComboMenu.GetDroppedWidth() < dx)
	{
		m_ComboMenu.SetDroppedWidth(dx);
		ASSERT(m_ComboMenu.GetDroppedWidth() == dx);
	}
}

void CPostTensionDrawTendonLineDlg::InitComboMenu()
{
	int wHeight;
	wHeight = (m_ComboMenuStr.GetSize() + 1) * _GetHeightFromFont();

	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0, rectWnd.Width(), wHeight,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	for (int i = 0; i < m_ComboMenuStr.GetSize(); ++i)
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	m_ComboMenu.SetCurSel(0);

	AdjustComboListBox();
}

void CPostTensionDrawTendonLineDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}

void CPostTensionDrawTendonLineDlg::OnInitialUpdate()
{
	MChildFormView::OnInitialUpdate();
	InitContent();
	InitComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
	CMouseEdit::ImAssocWindowST();
	m_gbStartNum.InitControl(this, CPostTensionDrawTendonLineDlg::IDD, IDC_TM_TENDON_START_STC, TRUE, FALSE, 3, 4);
}

void CPostTensionDrawTendonLineDlg::OnComboMenuSelect()
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();
	if (nCurI < 0 || nCurI >= m_DlgIDs.GetSize())
	{
		ASSERT(0); return;
	}

	//I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	//CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	ShowCurChildDlg(nCurI);	
	//14
	//CWnd * pWnd;
	// if (m_DlgIDs[nCurI] == __TMMODE_PT_DRAW_RENUM_TENDON__)
	//{
	//	m_wndExecute.ShowWindow(SW_HIDE);
	//	m_wndClose.ShowWindow(SW_HIDE);
	//}
	//else
	//{
	//	//pWnd = GetDlgItem(IDC_TM_EXECUTE);
	//	//pWnd->ShowWindow(SW_SHOW);
	//	m_wndExecute.ShowWindow(SW_SHOW);
	//	m_wndClose.ShowWindow(SW_SHOW);
	//}


	/*
	CString str;
	m_ComboMenu.GetLBText(nCurI,str);
	GSaveHistoryNF(str);
	*/
}

void CPostTensionDrawTendonLineDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CPostTensionDrawTendonLineDlg::OnViewStartNodeNum()
{
	((CDBDoc*)CTreeMenuBarBase::GetDocST())->TendonNodeNumberDlg();
}

void CPostTensionDrawTendonLineDlg::OnViewStartElemNum()
{
	((CDBDoc*)CTreeMenuBarBase::GetDocST())->TendonElemNumberDlg();
}

void CPostTensionDrawTendonLineDlg::OnTmTableBtn()
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));
}

void CPostTensionDrawTendonLineDlg::OnTabSelect(WPARAM wParam, LPARAM lParam)
{
	//m_ChildDlgs[m_nCurChildDlg]->OnTabSelect(NULL,NULL);
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
	}
	//if(bActivate)
	{
		int nCurID;
		if (m_nCurChildDlg != -1) nCurID = m_nCurChildDlg;
		else nCurID = 0;

		ShowCurChildDlg(nCurID);
		CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[nCurID]);

		// 테이블 없는 항목인 경우 Table 버튼 Disable
		if (m_TableIDs[nCurID] > 0) m_wndTableBtn.EnableWindow(TRUE);
		else m_wndTableBtn.EnableWindow(FALSE);
	}

	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount()) return;

	//if(m_nCurChildDlg == 0)
	if (m_DlgIDs[m_nCurChildDlg] == __TMMODE_PT_DRAW_TENDON_LINE_BEAM__)
	{
		m_ChildDlgs[m_nCurChildDlg]->OnTabSelect(0L, 0L);
	}
	//if(I_GENModelBase::GetModelingModeST() != IG_STATE_ELEM_ADD)
	//{
		//CMouseEdit::ImNotAssocWindowST();
		//CMouseEdit::DisConnectAssocWindowST();
	//}
}

void CPostTensionDrawTendonLineDlg::OnTabUnSelect(WPARAM wParam, LPARAM lParam)
{
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount()) return;
	//m_ChildDlgs[m_nCurChildDlg]->OnTabUnSelect(NULL,NULL);

	//if(m_nCurChildDlg == 1)
	//  ((CElemCurveDlg *)m_ChildDlgs[m_nCurChildDlg])->EndPlugInParam(); 
}

void CPostTensionDrawTendonLineDlg::SetHolderSize(int nHeight)
{
	CWnd* pWndHolder/*, *pWndExecute, *pWndClose*/;
	CRect rectHolder;
	pWndHolder = GetDlgItem(IDC_TM_TENDON_PLACEHOLDER);
	pWndHolder->GetWindowRect(rectHolder);
	rectHolder.bottom = rectHolder.top + nHeight;
	pWndHolder->GetParent()->ScreenToClient(rectHolder);
	pWndHolder->MoveWindow(rectHolder);

	CRect PHRect;
	m_wndPlaceHolder.GetWindowRect(&PHRect);
	ScreenToClient(&PHRect);
	m_RectPlaceHolder = PHRect;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...

	//CArray<UINT, UINT> aNextCtrls;   aNextCtrls.RemoveAll();
	//aNextCtrls.Add(IDC_TM_TENDON_CLOSE);
	//aNextCtrls.Add(IDC_TM_TENDON_EXECUTE);
	//aNextCtrls.FreeExtra();

	//CRect rRef, rToMove;
	//pWndHolder->GetWindowRect(rRef);
	//GetDlgItem(aNextCtrls[0])->GetWindowRect(rToMove);
	//int nDistY = (rRef.bottom + 5) - rToMove.top;
	//CDlgUtil::CtrlMoveDistY(this, aNextCtrls, nDistY);

	///////////////////////////////////////////////////////////////
	//// Scroll Size를 조정한다. 
	//CRect rectClose;
	//GetDlgItem(IDC_TM_TENDON_CLOSE)->GetWindowRect(rectClose);
	//ScreenToClient(&rectClose);

	//CSize ScrSize, TSize;
	//ScrSize = GetTotalSize();
	//ScrSize.cy = rectClose.bottom + 5;

	//SetScrollSizes(MM_TEXT, ScrSize);
}

void CPostTensionDrawTendonLineDlg::ShowExecuteButton(UINT nShowHide)
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_TM_EXECUTE);
	if (pWnd)
		pWnd->ShowWindow(nShowHide);
}

void CPostTensionDrawTendonLineDlg::OnTmExecAndClose(BOOL bExecute)
{
	if (bExecute)
		OnTmExecute();
	else
		OnTmClose();
}

int CPostTensionDrawTendonLineDlg::GetOrgSlabPlaceHolderSize()
{
	return m_nOrgSlabPlaceHolderHeight;
}

CWnd* CPostTensionDrawTendonLineDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}