// StructNodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StructNodeDlg.h"

// Dialog�߰��� 3�� ���� : '(����)'�̶�� �ڸ�Ʈ ģ ��
// ���� : ���� ���߾� �� ��
// #ifdef ???? �������� StructBndrDlg�� StructStldDlgó�� ������ ��

// (����)
#include "NodeCreateDlg.h"
#include "NodeRotateDlg.h"
#include "NodeProjectDlg.h"
#include "NodeTransDlg.h"
#include "NodeMirrorDlg.h"
#include "NodeDivideDlg.h"
#include "NodeDeleteDlg.h"
#include "NodeCompactDlg.h"
#include "NodeRenumberDlg.h"
#include "NodeMergeDlg.h"
#include "NodeScaleDlg.h"

#include "BndrSkewDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStructNodeDlg

IMPLEMENT_DYNCREATE(CStructNodeDlg, MChildFormView)

CStructNodeDlg::CStructNodeDlg()
	: MChildFormView(CStructNodeDlg::IDD)
{
	//{{AFX_DATA_INIT(CStructNodeDlg)
	//}}AFX_DATA_INIT

// (����)
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Create_Nodes));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Translate_Nodes));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Divide_Nodes));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Merge_Nodes));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Delete_Nodes));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Rotate_Nodes));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Mirror_Nodes));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Renumbering) + _T(" ") + _LS(IDS_WG_TREEMENU_Node));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Compact_Node_Numbers));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Project_Nodes));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Scale_Nodes)); 
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Node_Local_Axis));

	CreateChildDlgs();
}

CStructNodeDlg::~CStructNodeDlg()
{
	DeleteChildDlgs();
}

void CStructNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStructNodeDlg)
	DDX_Control(pDX, IDC_TM_NODE_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_START_NODE_NUM,  m_EditNode);
	DDX_Control(pDX, IDC_NODE_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStructNodeDlg, MChildFormView)
	//{{AFX_MSG_MAP(CStructNodeDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_VIEW_START_NUM, OnViewStartNum)
	ON_CBN_SELCHANGE(IDC_NODE_MENU_COMBO, OnComboMenuSelect)
	ON_BN_CLICKED(IDC_TM_NODE_TABLE_BTN, OnTmTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStructNodeDlg diagnostics

#ifdef _DEBUG
void CStructNodeDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CStructNodeDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStructNodeDlg message handlers

void CStructNodeDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);  	
}

// (����)
void CStructNodeDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 
	
	m_ChildDlgs.Add(new CNodeCreateDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_CREATE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);

	m_ChildDlgs.Add(new CNodeTransDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_TRANSLATE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);

	m_ChildDlgs.Add(new CNodeDivideDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_DIVIDE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);

	m_ChildDlgs.Add(new CNodeMergeDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_MERGE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);   // Merge Node  

	m_ChildDlgs.Add(new CNodeDeleteDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_DELETE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);   // Delete Node    
																					// DialogBar Menu���� �ٲ�. 2000.7.25
	m_ChildDlgs.Add(new CNodeRotateDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_ROTATE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);

	m_ChildDlgs.Add(new CNodeMirrorDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_MIRROR__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);

	m_ChildDlgs.Add(new CNodeRenumberDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_RENUMBER__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);  //2000.6.7  �ּ���

	m_ChildDlgs.Add(new CNodeCompactDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_COMPACT__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);

	m_ChildDlgs.Add(new CNodeProjectDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_PROJECT__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);

/*  m_ChildDlgs.Add(new CNodeDeleteDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_DELETE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);  */      // DeleteNode

																	// DialogBar Menu���� �ٲ�. 2000.7.25
	m_ChildDlgs.Add(new CNodeScaleDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_SCALE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);   // Node Scale       
																					// DialogBar Menu���� �ٲ�. 2000.7.25

	m_ChildDlgs.Add(new CBndrSkewDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_BNDR_SKEW__);
	m_TableIDs.Add(ID_QUERY_STRUCT_SKEWCS);

/*  m_ChildDlgs.Add(new CNodeMergeDlg());
	
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);           */

/*  m_ChildDlgs.Add(new CNodeScaleDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_NODE_SCALE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NODE);           */
}

void CStructNodeDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

// TreeMenu Mode�� �ش��ϴ� Child Dialog�� Ȱ��ȭ ��Ų��. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CStructNodeDlg::ShowChildDlgByID(int nID)
{
	if(nID == __TMMODE_STRUCT_NODE__)
	{
		ShowCurChildDlg(0);  
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for(int i = 0 ; i < nIDCount ; i++)
	{
		if(nID == m_DlgIDs[i])
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

void CStructNodeDlg::ShowCurChildDlg(int nDlgIndex)
{
	if(m_nCurChildDlg == nDlgIndex)
		return;
		
	if(nDlgIndex > m_ChildDlgs.GetSize() - 1 )
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}
	
	if(m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize()-1 >= m_nCurChildDlg)
		m_ChildDlgs[m_nCurChildDlg]->End();
		
	m_ChildDlgs[nDlgIndex]->CreateInit(&m_wndPlaceHolder,0,0);
	m_nCurChildDlg = nDlgIndex;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect,ParentRect,PHRect;
	
	GetClientRect(&ParentRect);
	m_ChildDlgs[nDlgIndex]->GetWindowRect(&DlgRect);
	ScreenToClient(&DlgRect);
	
	int Sx,Sy,Wx,Wy;
	Sx = (ParentRect.Width() - DlgRect.Width()) / 2 ;
	Sy = DlgRect.top;
	
	

	Wx = DlgRect.Width();
	Wy = DlgRect.Height();
	
	m_wndPlaceHolder.SetWindowPos(NULL, Sx,Sy,Wx,Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndPlaceHolder.GetWindowRect(&PHRect);

	ScreenToClient(&PHRect);
	
	if(m_DlgIDs[nDlgIndex] == __TMMODE_STRUCT_NODE_RENUMBER__)
	{
// 		m_wndExecute.ShowWindow(SW_HIDE);
// 		m_wndClose.ShowWindow(SW_HIDE);
	}
	else
	{
// 		m_wndExecute.ShowWindow(SW_SHOW);
// 		m_wndClose.ShowWindow(SW_SHOW);
	}

	/*
	CRect CBRect,EBRect;
	int SCX,SEX,BY,CW,EW;

	m_wndClose.GetWindowRect(CBRect);
	m_wndExecute.GetWindowRect(EBRect);
	
	BY = Sy+Wy+5;
	CW = CBRect.Width();
	EW = EBRect.Width();
	
#ifdef _RUS    
	SEX = PHRect.right - CW - EW - 2;
	SCX = SEX + CW + 15;
#else
	SEX = PHRect.right - CW - EW - 10;
	SCX = SEX + CW + 5;
#endif	
	
	m_wndExecute.SetWindowPos(NULL,SEX,BY,0,0, SWP_NOSIZE |SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndClose  .SetWindowPos(NULL,SCX,BY,0,0, SWP_NOSIZE |SWP_NOZORDER | SWP_NOACTIVATE);
	*/
	/////////////////////////////////////////////////////////////
	// Scroll Size�� �����Ѵ�. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);
}

int CStructNodeDlg::_GetHeightFromFont()
{
	 CFont * pNewFont  = GetFont();
	 CFont *pSysFont,*pOldFont,TempFont;
	 CDC   *pCdc;
	 TEXTMETRIC  tmNew,tmSys;
	 int     nTemp,nCorrectVal;
 
	 //get the DC for the edit control
	 pCdc = GetDC();
 
	 //get the metrics for the system font
	 pSysFont = TempFont.FromHandle(((HFONT)GetStockObject(SYSTEM_FONT)));
	 pOldFont  = pCdc->SelectObject( pSysFont);
	 pCdc->GetTextMetrics(&tmSys);
 
	 //get the metrics for the new font
	 pCdc->SelectObject(pNewFont);
	 pCdc->GetTextMetrics(&tmNew);
 
	 //select the original font back into the DC and release the DC
	 pCdc->SelectObject(pOldFont);
	 ReleaseDC(pCdc);
	 
	 nCorrectVal = (min(tmNew.tmHeight, tmSys.tmHeight)/2);
	 //calculate the new height for the edit control
	 nTemp = tmNew.tmHeight + nCorrectVal;
	 //nTemp = tmNew.tmHeight + tmNew.tmInternalLeading;
	 return nTemp;
}

void CStructNodeDlg::AdjustComboListBox()
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_ComboMenu.GetDC();
	for (int i=0;i < m_ComboMenu.GetCount();i++)
	{
		m_ComboMenu.GetLBText( i, str );
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

	//m_ComboMenu.SetItemHeight(-1, 28);
}

void CStructNodeDlg::InitComboMenu()
{
	int wHeight;
	wHeight = (m_ComboMenuStr.GetSize()+1) * _GetHeightFromFont();
	
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0,rectWnd.Width(),wHeight,
				         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	for( int i=0;  i<m_ComboMenuStr.GetSize(); ++i )
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	m_ComboMenu.SetCurSel(0);

	AdjustComboListBox();
}


int CStructNodeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

void CStructNodeDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  

void CStructNodeDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
	//CMouseEdit::ImAssocWindowST();
}

void CStructNodeDlg::OnViewStartNum() 
{
	// TODO: Add your control notification handler code here
	((CDBDoc*)CTreeMenuBarBase::GetDocST())->NodeNumberDlg();
}

void CStructNodeDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	/*
	if(bActivate)
	{
		if(m_nCurChildDlg != -1)
		{
			ShowCurChildDlg(m_nCurChildDlg);
			CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[m_nCurChildDlg]);
		}
		else
		{
			ShowCurChildDlg(0);
			CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[0]);
		}
	}
	*/
}

// Combo Menu���ÿ� ���� ��Ȳ�ݿ��� ���Ѵ�. 
void CStructNodeDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);	

	if( m_DlgIDs[nCurI] == __TMMODE_STRUCT_NODE_RENUMBER__)
	{
// 		m_wndExecute.ShowWindow(SW_HIDE);
// 		m_wndClose.ShowWindow(SW_HIDE);
	}
	else
	{
// 		m_wndExecute.ShowWindow(SW_SHOW);
// 		m_wndClose.ShowWindow(SW_SHOW);
	}
}

void CStructNodeDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	if(m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CStructNodeDlg::OnTmTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));		
}
/////////////////////////////////////////////////////////////////////
// By L.C.G
void CStructNodeDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	//GSaveHistoryNF("OnTabSelect");
	if(CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1,-1);
	}
	
//  if(bActivate)
	{
		int nCurID;
		if(m_nCurChildDlg != -1) nCurID = m_nCurChildDlg;
		else nCurID = 0;

		ShowCurChildDlg(nCurID);
		CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[nCurID]);

		// ���̺� ���� �׸��� ��� Table ��ư Disable
		if (m_TableIDs[nCurID] > 0) m_wndTableBtn.EnableWindow(TRUE);
		else m_wndTableBtn.EnableWindow(FALSE);
	}

	int nMode = I_GENModelBase::GetModelingModeST();
	
	if(m_nCurChildDlg == 0)
	{
		m_ChildDlgs[m_nCurChildDlg]->OnTabSelect(0L,0L);
	}

	//if(nMode != IG_STATE_NODE_ADD)
	//{
		//CMouseEdit::ImNotAssocWindowST();
		//CMouseEdit::DisConnectAssocWindowST();
	//}

}

/////////////////////////////////////////////////////////////////////
// By L.C.G
// ���� Unselect �� Notify ���� ���� 
void CStructNodeDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}

void CStructNodeDlg::SetHolderSize(int nHeight)
{
	CWnd* pWndHolder; 
	CRect rectHolder, rectExecute, rectClose, rectParent;
	pWndHolder = GetDlgItem(IDC_TM_PLACEHOLDER);
	pWndHolder->GetWindowRect(rectHolder);
	rectHolder.bottom = rectHolder.top + nHeight;
	pWndHolder->GetParent()->ScreenToClient(rectHolder);
	pWndHolder->MoveWindow(rectHolder);
}

void CStructNodeDlg::OnTmExecAndClose(BOOL bExecute)
{
	if (bExecute)
		OnTmExecute();
	else
		OnTmClose();
}

CWnd* CStructNodeDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}