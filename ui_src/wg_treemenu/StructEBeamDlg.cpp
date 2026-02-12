// StructEBeamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StructEBeamDlg.h"
#include "..\wg_base\TreeMenuMode.h"

// Dialog�߰��� 3�� ���� : '(����)'�̶�� �ڸ�Ʈ ģ ��
// ���� : ���� ���߾� �� ��
// #ifdef ???? �������� StructBndrDlg�� StructStldDlgó�� ������ ��

// (����)
#include "ElemEBeamCreateDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStructEBeamDlg

IMPLEMENT_DYNCREATE(CStructEBeamDlg, MChildFormView)

CStructEBeamDlg::CStructEBeamDlg()
	: MChildFormView(CStructEBeamDlg::IDD)
{
	//{{AFX_DATA_INIT(CStructEBeamDlg)
	//}}AFX_DATA_INIT

	struct T_COMBO_ITEM
	{
		int nMask;
		CString strItem;
	};
// (����) // redesigned by J.B.Seon 2002.05.22
	T_COMBO_ITEM aItem[] = {
		{ D_PROD_GEN                       , _LS(IDS_WG_TREEMENU_Create_EBeam) },
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

CStructEBeamDlg::~CStructEBeamDlg()
{
	DeleteChildDlgs();
}

void CStructEBeamDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStructEBeamDlg)
	DDX_Control(pDX, IDC_TM_ELEM_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_START_NODE_NUM, m_EditNode);
	DDX_Control(pDX, IDC_START_ELEM_NUM, m_EditElem);
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_ELEM_MENU_COMBO, m_ComboMenu);
	DDX_Control(pDX, IDC_TM_START_STC, m_gbStartNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStructEBeamDlg, MChildFormView)
	//{{AFX_MSG_MAP(CStructEBeamDlg)
	//ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_CBN_SELCHANGE(IDC_ELEM_MENU_COMBO, OnComboMenuSelect)
	//ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_VIEW_START_NODE_NUM, OnViewStartNodeNum)
	ON_BN_CLICKED(IDC_VIEW_START_ELEM_NUM, OnViewStartElemNum)
	ON_BN_CLICKED(IDC_TM_ELEM_TABLE_BTN, OnTmTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStructEBeamDlg diagnostics

#ifdef _DEBUG
void CStructEBeamDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CStructEBeamDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStructEBeamDlg message handlers

void CStructEBeamDlg::OnViewStartNodeNum() 
{
	((CDBDoc*)CTreeMenuBarBase::GetDocST())->NodeNumberDlg();
}

void CStructEBeamDlg::OnViewStartElemNum() 
{
	((CDBDoc*)CTreeMenuBarBase::GetDocST())->ElemNumberDlg();
}

void CStructEBeamDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	InitContent();
	InitComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
	CMouseEdit::ImAssocWindowST();
	m_gbStartNum.InitControl(this, CStructEBeamDlg::IDD, IDC_TM_START_STC,TRUE,FALSE,3,4);
}

// (����)
void CStructEBeamDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 
	
	m_ChildDlgs.Add(new CElemEBeamCreateDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_EBEAM_CREATE__);
	m_TableIDs.Add(0); 
}

void CStructEBeamDlg::AdjustComboListBox()
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

void CStructEBeamDlg::InitComboMenu()
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

// Combo Menu���ÿ� ���� ��Ȳ�ݿ��� ���Ѵ�. 
void CStructEBeamDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();
	
	//I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
}

void CStructEBeamDlg::ShowExecuteButton(UINT nShowHide)
{
	CWnd * pWnd;
	pWnd = GetDlgItem(IDC_TM_EXECUTE);
	if(pWnd)
		pWnd->ShowWindow(nShowHide);
}

void CStructEBeamDlg::OnTmExecute() 
{
	if(m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CStructEBeamDlg::OnTmClose() 
{
	if(CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1,-1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);  	
}

void CStructEBeamDlg::OnTmExecAndClose(BOOL bExecute)
{
	if (bExecute)
		OnTmExecute();
	else
		OnTmClose();
}

int  CStructEBeamDlg::_GetHeightFromFont()
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

BOOL CStructEBeamDlg::ShowChildDlgByID(int nID)
{
	if(nID == __TMMODE_STRUCT_EBEAM_CREATE__)
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

void CStructEBeamDlg::ShowCurChildDlg(int nDlgIndex)
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
	//m_ChildDlgs[nDlgIndex]->CreateInit(this,0,0);
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
	
	//CRect CBRect,EBRect;
	//int SCX,SEX,BY,CW,EW;
	
	//m_wndExecute.ShowWindow(SW_SHOW);
	//m_wndClose.ShowWindow(SW_SHOW);  
	//
	//m_wndClose.GetWindowRect(CBRect);
	//m_wndExecute.GetWindowRect(EBRect);

//	BY = Sy+Wy+5;
//	CW = CBRect.Width();
//	EW = EBRect.Width();
//
//#ifdef _RUS    
//	SEX = PHRect.right - CW - EW - 2;
//	SCX = SEX + CW + 15;
//#else
//	SEX = PHRect.right - CW - EW - 10;
//	SCX = SEX + CW + 5;
//#endif

	//m_wndExecute.SetWindowPos(NULL,SEX,BY,0,0, SWP_NOSIZE |SWP_NOZORDER | SWP_NOACTIVATE);
	//m_wndClose  .SetWindowPos(NULL,SCX,BY,0,0, SWP_NOSIZE |SWP_NOZORDER | SWP_NOACTIVATE);


	/////////////////////////////////////////////////////////////
	// Scroll Size�� �����Ѵ�. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);

	//__TMMODE_STRUCT_ELEM_EXTRUDE__

	
}

void CStructEBeamDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}


void CStructEBeamDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}

void CStructEBeamDlg::OnTmTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}
/////////////////////////////////////////////////////////////////////
// By L.C.G
void CStructEBeamDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
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

		// ���̺� ���� �׸��� ��� Table ��ư Disable
		if (m_TableIDs[nCurID] > 0) m_wndTableBtn.EnableWindow(TRUE);
		else m_wndTableBtn.EnableWindow(FALSE);
	}

	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount()) return;
	
}
/////////////////////////////////////////////////////////////////////
// By L.C.G
void CStructEBeamDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount()) return;
}

CWnd* CStructEBeamDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}

void CStructEBeamDlg::SetHolderSize(int nHeight)
{
	CWnd *pWndHolder/*, *pWndExecute, *pWndClose*/; 
	CRect rectHolder, rectExecute, rectClose, rectParent;
	pWndHolder = GetDlgItem(IDC_TM_PLACEHOLDER);
	pWndHolder->GetWindowRect(rectHolder);
	rectHolder.bottom = rectHolder.top + nHeight;
	pWndHolder->GetParent()->ScreenToClient(rectHolder);
	pWndHolder->MoveWindow(rectHolder);
}

