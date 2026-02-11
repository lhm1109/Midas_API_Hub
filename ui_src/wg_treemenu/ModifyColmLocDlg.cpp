// ModifyColmLocDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ModifyColmLocDlg.h"
#include "..\wg_base\TreeMenuMode.h"

// Dialog추가시 3곳 수정 : '(수정)'이라고 코멘트 친 곳
// 주의 : 순서 맞추어 줄 것
// #ifdef ???? 많아지면 StructBndrDlg나 StructStldDlg처럼 변경할 것

// (수정)
#include "ModifyColmLocPage.h"
#include "SlabShearReinforce.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyColmLocDlg

IMPLEMENT_DYNCREATE(CModifyColmLocDlg, MChildFormView)

CModifyColmLocDlg::CModifyColmLocDlg()
	: MChildFormView(CModifyColmLocDlg::IDD)
{
	//{{AFX_DATA_INIT(CModifyColmLocDlg)
	//}}AFX_DATA_INIT

	struct T_COMBO_ITEM
	{
		int nMask;
		CString strItem;
	};
// (수정)
	T_COMBO_ITEM aItem[] = {
		{ D_PROD_GEN , _LS(IDS_TM_COLUMN_LOCATION) },
		{ D_PROD_GEN , _LS(IDS_TM_SLAB_SHEAR_REINFORCEMENT) },
	};

	int nMask = 0;
#if defined(_MGEN)
	nMask = D_PROD_GEN;
#elif defined(_CIVIL)
	nMask = D_PROD_CVL;
#elif defined(_TOWER)
	nMask = D_PROD_TOW;
#else
	#error _T("Unknown Product Definition!")
#endif

	int i, nCount;
	nCount = sizeof(aItem)/sizeof(T_COMBO_ITEM);
	for(i=0; i < nCount; i++)
	{
		if ((nMask & aItem[i].nMask) == 0) continue;
		m_ComboMenuStr.Add(aItem[i].strItem);
	}

	CreateChildDlgs();  
}

CModifyColmLocDlg::~CModifyColmLocDlg()
{
	DeleteChildDlgs();
}

void CModifyColmLocDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyColmLocDlg)
	DDX_Control(pDX, IDC_TM_COLM_LOC_TABLE_BTN, m_wndTableBtn);
// 	DDX_Control(pDX, IDC_TM_CLOSE, m_wndClose);
// 	DDX_Control(pDX, IDC_TM_EXECUTE, m_wndExecute);
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_TM_COLM_LOC_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifyColmLocDlg, MChildFormView)
	//{{AFX_MSG_MAP(CModifyColmLocDlg)
// 	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_CBN_SELCHANGE(IDC_TM_COLM_LOC_MENU_COMBO, OnComboMenuSelect)
// 	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_COLM_LOC_TABLE_BTN, OnTmTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyColmLocDlg diagnostics

#ifdef _DEBUG
void CModifyColmLocDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CModifyColmLocDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CModifyColmLocDlg message handlers

void CModifyColmLocDlg::OnViewStartNodeNum() 
{
	((CDBDoc*)CTreeMenuBarBase::GetDocST())->NodeNumberDlg();
}

void CModifyColmLocDlg::OnViewStartElemNum() 
{
	((CDBDoc*)CTreeMenuBarBase::GetDocST())->ElemNumberDlg();
}

void CModifyColmLocDlg::OnInitialUpdate() 
{
	CFormViewInBar::OnInitialUpdate();
	InitContent();
	InitComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
	CMouseEdit::ImAssocWindowST();
	//m_gbStartNum.InitControl(this, CModifyColmLocDlg::IDD, IDC_TM_START_STC,TRUE,FALSE,3,4);
}

// (수정)
void CModifyColmLocDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 
	
	m_ChildDlgs.Add(new CModifyColmLocPage());
	m_DlgIDs.Add(__TMMODE_MESHDGN_COLUMN_LOCATION__);
	m_TableIDs.Add(ID_QUERY_COLUMN_LOCATION);

	m_ChildDlgs.Add(new CSlabShearReinforcePage());
	m_DlgIDs.Add(__TMMODE_MESHDGN_SLABSHEAR_REINFORCE__);
	m_TableIDs.Add(ID_QUERY_SLAB_SHEAR_REINFORCE);

}

void CModifyColmLocDlg::AdjustComboListBox()
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
}

void CModifyColmLocDlg::InitComboMenu()
{
	int wHeight;
	wHeight = (m_ComboMenuStr.GetSize()+1) * GetHeightFromFont();
	
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

// Combo Menu선택에 따른 상황반영을 행한다. 
void CModifyColmLocDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();
	
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));

// 	m_wndExecute.ShowWindow(SW_SHOW);
// 	m_wndClose.ShowWindow(SW_SHOW);
}

// void CModifyColmLocDlg::ShowExecuteButton(UINT nShowHide)
// {
// 	CWnd * pWnd;
// 	pWnd = GetDlgItem(IDC_TM_EXECUTE);
// 	if(pWnd)
// 		pWnd->ShowWindow(nShowHide);
// }

void CModifyColmLocDlg::OnTmExecute() 
{
	if(m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CModifyColmLocDlg::OnTmClose() 
{
	if(CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1,-1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);  	
}

void CModifyColmLocDlg::OnTmExecAndClose(BOOL bExecute)
{
	if (bExecute)
		OnTmExecute();
	else
		OnTmClose();
}

int  CModifyColmLocDlg::_GetHeightFromFont()
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

BOOL CModifyColmLocDlg::ShowChildDlgByID(int nID)
{
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

void CModifyColmLocDlg::ShowCurChildDlg(int nDlgIndex)
{
//   if(m_nCurChildDlg == nDlgIndex)
//     return;
		
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
	
// 	CRect CBRect,EBRect;
// 	int SCX,SEX,BY,CW,EW;
// 	
// 	m_wndExecute.ShowWindow(SW_SHOW);
// 	m_wndClose.ShowWindow(SW_SHOW);  
// 
// 	m_wndClose.GetWindowRect(CBRect);
// 	m_wndExecute.GetWindowRect(EBRect);
// 		
// 	BY = Sy+Wy+5;
// 	CW = CBRect.Width();
// 	EW = EBRect.Width();
// 		
// #ifdef _RUS    
// 	SEX = PHRect.right - CW - EW - 2;
// 	SCX = SEX + CW + 15;
// #else
// 	SEX = PHRect.right - CW - EW - 10;
// 	SCX = SEX + CW + 5;
// #endif
// 
// 	m_wndExecute.SetWindowPos(NULL,SEX,BY,0,0, SWP_NOSIZE |SWP_NOZORDER | SWP_NOACTIVATE);
// 	m_wndClose  .SetWindowPos(NULL,SCX,BY,0,0, SWP_NOSIZE |SWP_NOZORDER | SWP_NOACTIVATE);

	/////////////////////////////////////////////////////////////
	// Scroll Size를 조정한다. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize); 
}

void CModifyColmLocDlg::LayoutChildDialog()
{
	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect, ParentRect, PHRect;

	GetClientRect(&ParentRect);
	m_ChildDlgs[m_nCurChildDlg]->GetWindowRect(&DlgRect);
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
}
void CModifyColmLocDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}


void CModifyColmLocDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}

void CModifyColmLocDlg::OnTmTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}
/////////////////////////////////////////////////////////////////////
// By L.C.G
void CModifyColmLocDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	//m_ChildDlgs[m_nCurChildDlg]->OnTabSelect(NULL,NULL);
	if(CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode    (FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode (FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1,-1);
	}
	//if(bActivate)
	{
		int nCurID;
		if(m_nCurChildDlg != -1) nCurID = m_nCurChildDlg;
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
}
/////////////////////////////////////////////////////////////////////
// By L.C.G
void CModifyColmLocDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount()) return;
}

CWnd* CModifyColmLocDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}

// void CModifyColmLocDlg::SetHolderSize(int nHeight)
// {
// 	CWnd *pWndHolder/*, *pWndExecute, *pWndClose*/; 
// 	CRect rectHolder, rectExecute, rectClose, rectParent;
// 	pWndHolder = GetDlgItem(IDC_TM_PLACEHOLDER);
// 	pWndHolder->GetWindowRect(rectHolder);
// 	rectHolder.bottom = rectHolder.top + nHeight;
// 	pWndHolder->GetParent()->ScreenToClient(rectHolder);
// 	pWndHolder->MoveWindow(rectHolder);
// }
// 
