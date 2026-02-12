// InelasticHingeMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "PushOverHingeMainDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\I_GENStateDefine.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_db\wg_dbRes2.h"
#include "..\wg_main\wg_mainRes2.h"

#include "PushOverHingeAssignDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//int ____bNotifyComboMv = TRUE;
/////////////////////////////////////////////////////////////////////////////
// CPushoverHingeMainDlg dialog


IMPLEMENT_DYNCREATE(CPushoverHingeMainDlg, MChildFormView)


CPushoverHingeMainDlg::CPushoverHingeMainDlg(CWnd* pParent /*=NULL*/)
	: MChildFormView(CPushoverHingeMainDlg::IDD)
{
	//{{AFX_DATA_INIT(CPushoverHingeMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	CArray<const TCHAR*, const TCHAR*> ComboStrs;
	ComboStrs.Add(_LS(IDS_WG_CMD__ADDD__Assign_PushOver_Hinge));

	m_ComboMenuStr.RemoveAll();
	int nStrs = ComboStrs.GetSize();
	for(int i = 0 ; i < nStrs ; i++)
	{
		m_ComboMenuStr.Add((TCHAR*) ComboStrs[i]);
	}

	CreateChildDlgs();
}

CPushoverHingeMainDlg::~CPushoverHingeMainDlg()
{
	DeleteChildDlgs();
}

void CPushoverHingeMainDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPushoverHingeMainDlg)
	DDX_Control(pDX, IDC_CMD_TABLE_BUTTON, m_wndTableButton);
	DDX_Control(pDX, IDC_DBAR_FUNC_COMBO, m_ComboMenu);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPushoverHingeMainDlg, MChildFormView)
	//{{AFX_MSG_MAP(CPushoverHingeMainDlg)
	ON_BN_CLICKED(IDC_CMD_TABLE_BUTTON, OnCmdTableButton)
	ON_CBN_SELCHANGE(IDC_DBAR_FUNC_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPushoverHingeMainDlg message handlers

void CPushoverHingeMainDlg::CreateChildDlgs()
{ 
	m_nCurChildDlg = -1; 
	
	m_ChildDlgs.Add(new CPushOverHingeAssignDlg);
	m_DlgIDs.Add(__TMMODE_CMD_POHA__);
	m_TableIDs.Add(ID_QUERY_DGN_POVR_HNGE);  
}

void CPushoverHingeMainDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

// TreeMenu Mode�� �ش��ϴ� Child Dialog�� Ȱ��ȭ ��Ų��. 
BOOL CPushoverHingeMainDlg::ShowChildDlgByID(int nID)
{
	if (nID == __TMMODE_CMD_POHA__)
	{
		m_ComboMenu.SetCurSel(0);
		ShowCurChildDlg(0);  
		
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for (int i = 0 ; i < nIDCount ; i++)
	{
		if (nID == m_DlgIDs[i])
		{
			m_ComboMenu.SetCurSel(i);
			ShowCurChildDlg(i);  
			if (m_TableIDs[i] > 0) m_wndTableButton.EnableWindow(TRUE);
			else m_wndTableButton.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CPushoverHingeMainDlg::ShowCurChildDlg(int nDlgIndex)
{
	if (m_nCurChildDlg == nDlgIndex)
		return;
		
	if (nDlgIndex > m_ChildDlgs.GetSize() - 1 )
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}
	
	if (m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize()-1 >= m_nCurChildDlg)
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

void CPushoverHingeMainDlg::LayoutChildDialog()
{
	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect,ParentRect,PHRect;
	
	GetClientRect(&ParentRect);
	m_ChildDlgs[m_nCurChildDlg]->GetWindowRect(&DlgRect);
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

int CPushoverHingeMainDlg::_GetHeightFromFont()
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

void CPushoverHingeMainDlg::InitComboMenu()
{  
	int nComboNum = m_ComboMenuStr.GetSize();

	int wHeight;
	wHeight = (nComboNum+1) * _GetHeightFromFont();
	
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0,rectWnd.Width(),wHeight,
				         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	for( int i=0;  i<nComboNum; ++i )
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	m_ComboMenu.SetCurSel(0);

	AdjustComboListBox();
}

void CPushoverHingeMainDlg::AdjustComboListBox()
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

	if (m_ComboMenu.GetDroppedWidth() < dx)
	{
		m_ComboMenu.SetDroppedWidth(dx);
		ASSERT(m_ComboMenu.GetDroppedWidth() == dx);
	}
}

void CPushoverHingeMainDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  


/////////////////////////////////////////////////////////////////////////////
// CPushoverHingeMainDlg message handlers
void CPushoverHingeMainDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CPushoverHingeMainDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{	
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CPushoverHingeMainDlg::OnComboMenuSelect() 
{
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	int nCurI = m_ComboMenu.GetCurSel();
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
}

int CPushoverHingeMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CPushoverHingeMainDlg::OnCmdTableButton() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));				
}

CWnd* CPushoverHingeMainDlg::GetDlgByID(int nID)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (nID == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}

void CPushoverHingeMainDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CPushoverHingeMainDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}