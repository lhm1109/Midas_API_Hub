// SeisCvlMiscDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlMiscDlg.h"

#include "SeisCvlEvalParamDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlMiscDlg

IMPLEMENT_DYNCREATE(CSeisCvlMiscDlg, MChildFormView)

CSeisCvlMiscDlg::CSeisCvlMiscDlg()
	: MChildFormView(CSeisCvlMiscDlg::IDD)
{
	//{{AFX_DATA_INIT(CSeisCvlMiscDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	// Menu �߰� �ؾ��� - �䷱ ��������
	AddChildDlgs();
	m_nCurChildDlg = -1; 
}

CSeisCvlMiscDlg::~CSeisCvlMiscDlg()
{
	DelChildDlgs();
}

void CSeisCvlMiscDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlMiscDlg)
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_MENU_CMB,    m_cmbMenu);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_TABLE_BTN,   m_btnTable);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlMiscDlg, MChildFormView)
	//{{AFX_MSG_MAP(CSeisCvlMiscDlg)
	ON_CBN_SELCHANGE(IDC_TM_SEIS_CVL_MISC_MENU_CMB, OnTmMenuCmb)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_TABLE_BTN, OnTmTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlMiscDlg diagnostics

#ifdef _DEBUG
void CSeisCvlMiscDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CSeisCvlMiscDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlMiscDlg Implementation Functions
void CSeisCvlMiscDlg::AddChildDlgs()
{
	m_aDlgID.Add(__TMMODE_SEIS_CVL_RESULT_EVAL_PARAM__);
	m_aMenyStr.Add(_LS(IDS_WG_TREEMENU_Sect_Eval_Param));
	m_aChildDlg.Add(new CSeisCvlEvalParamDlg);
	m_aTableID.Add(ID_MAIN_RIBBON_SEIS_CVL_EVAL_PARAM_TB);
}

void CSeisCvlMiscDlg::DelChildDlgs()
{
	int nCount = m_aChildDlg.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_aChildDlg[i];
	}
}

// TreeMenu Mode�� �ش��ϴ� Child Dialog�� Ȱ��ȭ ��Ų��. 
BOOL CSeisCvlMiscDlg::ShowChildDlgByID(int nID)
{
	if(nID == __TMMODE_SEIS_CVL_RESULT_EVAL_PARAM__)
	{
		ShowCurChildDlg(0);  
		m_cmbMenu.SetCurSel(0);
		return TRUE;
	}

	int nIDCount = m_aDlgID.GetSize();
	for(int i = 0 ; i < nIDCount ; i++)
	{
		if(nID == m_aDlgID[i])
		{
			ShowCurChildDlg(i);  
			m_cmbMenu.SetCurSel(i);
			if (m_aTableID[i] > 0) m_btnTable.EnableWindow(TRUE);
			else m_btnTable.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CSeisCvlMiscDlg::ShowCurChildDlg(int nDlgIndex)
{
	if(m_nCurChildDlg == nDlgIndex)
		return;
		
	if(nDlgIndex > m_aChildDlg.GetSize() - 1 )
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}
	
	if(m_nCurChildDlg >= 0 && m_aChildDlg.GetSize()-1 >= m_nCurChildDlg)
		m_aChildDlg[m_nCurChildDlg]->End();
		
	m_aChildDlg[nDlgIndex]->CreateInit(&m_wndPlaceHolder,0,0);
	m_nCurChildDlg = nDlgIndex;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect,ParentRect,PHRect;
	
	GetClientRect(&ParentRect);
	m_aChildDlg[nDlgIndex]->GetWindowRect(&DlgRect);
	ScreenToClient(&DlgRect);
	
	int Sx,Sy,Wx,Wy;
	Sx = (ParentRect.Width() - DlgRect.Width()) / 2 ;
	Sy = DlgRect.top;

	Wx = DlgRect.Width();
	Wy = DlgRect.Height();
	
	m_wndPlaceHolder.SetWindowPos(NULL, Sx,Sy,Wx,Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndPlaceHolder.GetWindowRect(&PHRect);

	ScreenToClient(&PHRect);
	
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

void CSeisCvlMiscDlg::LayoutChildDialog()
{
	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect,ParentRect,PHRect;
	
	GetClientRect(&ParentRect);
	m_aChildDlg[m_nCurChildDlg]->GetWindowRect(&DlgRect);
	ScreenToClient(&DlgRect);
	
	int Sx,Sy,Wx,Wy;
	Sx = (ParentRect.Width() - DlgRect.Width()) / 2 ;
	Sy = DlgRect.top;

	Wx = DlgRect.Width();
	Wy = DlgRect.Height();
	
	m_wndPlaceHolder.SetWindowPos(NULL, Sx,Sy,Wx,Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndPlaceHolder.GetWindowRect(&PHRect);

	ScreenToClient(&PHRect);
	
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

int CSeisCvlMiscDlg::_GetHeightFromFont()
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

void CSeisCvlMiscDlg::AdjustComboListBox()
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_cmbMenu.GetDC();
	for (int i=0;i < m_cmbMenu.GetCount();i++)
	{
		m_cmbMenu.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_cmbMenu.ReleaseDC(pDC);

//  dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);

	if (m_cmbMenu.GetDroppedWidth() < dx)
	{
		m_cmbMenu.SetDroppedWidth(dx);
		ASSERT(m_cmbMenu.GetDroppedWidth() == dx);
	}
}

void CSeisCvlMiscDlg::InitComboMenu()
{
	int wHeight;
	wHeight = (m_aMenyStr.GetSize()+1) * _GetHeightFromFont();
	
	CRect rectWnd;
	m_cmbMenu.GetWindowRect(rectWnd);
	m_cmbMenu.SetWindowPos(NULL, 0, 0,rectWnd.Width(),wHeight,
				         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	for( int i=0;  i< m_aMenyStr.GetSize(); ++i )
	{
		m_cmbMenu.AddString(m_aMenyStr[i]);
	}
	m_cmbMenu.SetCurSel(0);

	AdjustComboListBox();
}

void CSeisCvlMiscDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  


/////////////////////////////////////////////////////////////////////////////
// CSeisCvlMiscDlg message handlers

void CSeisCvlMiscDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CSeisCvlMiscDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class	
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CSeisCvlMiscDlg::OnTmMenuCmb() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_cmbMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	CTreeMenuBarBase::SetModeST(m_aDlgID.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);	
}

int CSeisCvlMiscDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CSeisCvlMiscDlg::OnTmTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_cmbMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_cmbMenu.GetCount()) return;
	if (m_aTableID[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_aTableID[nCurSel], 0));			
}

void CSeisCvlMiscDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_aChildDlg.GetSize())
		m_aChildDlg[m_nCurChildDlg]->Execute();
}

void CSeisCvlMiscDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

/////////////////////////////////////////////////////////////////////
void CSeisCvlMiscDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	CMouseEdit::ImNotAssocWindowST();
	CMouseEdit::DisConnectAssocWindowST();
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
		CTreeMenuBarBase::SetModeOnlyST(m_aDlgID[nCurID]);

		// ���̺� ���� �׸��� ��� Table ��ư Disable
		if (m_aTableID[nCurID] > 0) m_btnTable.EnableWindow(TRUE);
		else m_btnTable.EnableWindow(FALSE);
	}

	int nCurSel = m_cmbMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_cmbMenu.GetCount()) return;
	if (0 > m_nCurChildDlg || m_nCurChildDlg >= m_cmbMenu.GetCount()) return;
}

/////////////////////////////////////////////////////////////////////
// ���� Unselect �� Notify ���� ���� 
void CSeisCvlMiscDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}

CWnd* CSeisCvlMiscDlg::GetDlgByID(int Mode)
{
	int nSize = m_aDlgID.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_aDlgID[i]) return m_aChildDlg[i];
	return 0;
}