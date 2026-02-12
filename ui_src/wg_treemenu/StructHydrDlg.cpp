// StructHydrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StructHydrDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

// Dialog�߰��� 3�� ���� : '(����)'�̶�� �ڸ�Ʈ ģ ��
// ���� : ���� ���߾� �� ��
// #ifdef ???? �������� StructBndrDlg�� StructStldDlgó�� ������ ��

// (����)
#include "HydrTemperDlg.h"
#include "HydrHeatDlg.h"
#include "HydrConvectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStructHydrDlg dialog
IMPLEMENT_DYNCREATE(CStructHydrDlg, MChildFormView)

CStructHydrDlg::CStructHydrDlg()
	: MChildFormView(CStructHydrDlg::IDD)
{
	//{{AFX_DATA_INIT(CStructHydrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
// (����)
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Element_Convection_Boundary));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Prescribed_Temperature));
	m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Assign_Heat_Source));
	
	CreateChildDlgs();
}

CStructHydrDlg::~CStructHydrDlg()
{
	DeleteChildDlgs();
}

void CStructHydrDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStructHydrDlg)
	DDX_Control(pDX, IDC_TM_HYDR_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_HYDR_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStructHydrDlg, MChildFormView)
	//{{AFX_MSG_MAP(CStructHydrDlg)
	ON_CBN_SELCHANGE(IDC_HYDR_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TM_HYDR_TABLE_BTN, OnTmTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStructHydrDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CStructStldDlg diagnostics

#ifdef _DEBUG
void CStructHydrDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CStructHydrDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CStructHydrDlg Implementation Functions
// (����)
void CStructHydrDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 
	
	m_ChildDlgs.Add(new CHydrConvectDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_HYDR_CONVECT__);
	m_TableIDs.Add(ID_QUERY_STRUCT_HECB);  // ���̺� ������ 0����

	m_ChildDlgs.Add(new CHydrTemperDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_HYDR_TEMPER__);
	m_TableIDs.Add(ID_QUERY_STRUCT_HSPT);  // ���̺� ������ 0����    

	m_ChildDlgs.Add(new CHydrHeatDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_HYDR_HEAT__);
	m_TableIDs.Add(ID_QUERY_STRUCT_HAHS);  // ���̺� ������ 0����    

	
	//m_ChildDlgs.Add(new CHydrMaterialDlg);
	//m_DlgIDs.Add(__TMMODE_STRUCT_HYDR_MATERIAL__);
	//m_TableIDs.Add(0);  // ���̺� ������ 0����

	

}

void CStructHydrDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

BOOL CStructHydrDlg::ShowChildDlgByID(int nID)
{
	if(nID == __TMMODE_STRUCT_HYDR__)
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

void CStructHydrDlg::ShowCurChildDlg(int nDlgIndex)
{
	if(m_nCurChildDlg == nDlgIndex)
		return;
		
	if(nDlgIndex > m_ChildDlgs.GetSize() - 1 )
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	// check hydration convection //////////////////////////////////////
	//if(nDlgIndex == 0) CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(TRUE);
	//else CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
	////////////////////////////////////////////////////////////////////
	
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

void CStructHydrDlg::LayoutChildDialog()
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

int CStructHydrDlg::_GetHeightFromFont()
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

void CStructHydrDlg::AdjustComboListBox()
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

void CStructHydrDlg::InitComboMenu()
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

void CStructHydrDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  

/////////////////////////////////////////////////////////////////////////////
// CStructStldDlg message handlers

void CStructHydrDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CStructHydrDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CStructHydrDlg::OnComboMenuSelect() 
{
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	//ShowCurChildDlg(nCurI);
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
}

int CStructHydrDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}


void CStructHydrDlg::OnTmTableBtn() 
{
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));			
}

void CStructHydrDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CStructHydrDlg::OnTmClose()
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
// By L.C.G
void CStructHydrDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	CMouseEdit::ImNotAssocWindowST();
	CMouseEdit::DisConnectAssocWindowST();
	
	if(m_nCurChildDlg != 0)
	{
		if(CDBDoc::GetDocPoint())
		{
			CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE); 
			CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE); 
			CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1,-1);
		}
	}

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
}

/////////////////////////////////////////////////////////////////////
// By L.C.G
// ���� Unselect �� Notify ���� ���� 
void CStructHydrDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}

CWnd* CStructHydrDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}

