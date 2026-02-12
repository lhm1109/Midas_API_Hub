// StructMassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StructMassDlg.h"

// Dialogï¿½ß°ï¿½ï¿½ï¿½ 3ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ : '(ï¿½ï¿½ï¿½ï¿½)'ï¿½Ì¶ï¿½ï¿½ ï¿½Ú¸ï¿½Æ® Ä£ ï¿½ï¿½
// ï¿½ï¿½ï¿½ï¿½ : ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ß¾ï¿½ ï¿½ï¿½ ï¿½ï¿½
// #ifdef ???? ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ StructBndrDlgï¿½ï¿½ StructStldDlgÃ³ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½

// (ï¿½ï¿½ï¿½ï¿½)
#include "MassNmasDlg.h"
#include "MassDmasDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\MIT_frx\MBarBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStructMassDlg

IMPLEMENT_DYNCREATE(CStructMassDlg, MChildFormView)

CStructMassDlg::CStructMassDlg()
	: MChildFormView(CStructMassDlg::IDD)
{
	//{{AFX_DATA_INIT(CStructMassDlg)
	//}}AFX_DATA_INIT

	struct T_COMBO_ITEM
	{
		int nMask;
		CString strItem;
	};
// (ï¿½ï¿½ï¿½ï¿½) // redesigned by J.B.Seon 2002.05.22
	T_COMBO_ITEM aItem[] = {
		{ D_PROD_GEN+D_PROD_CVL+D_PROD_TOW , _LS(IDS_WG_TREEMENU_Nodal_Masses) },
		{ D_PROD_GEN                       , _LS(IDS_WG_TREEMENU_Floor_Diaphragm_Masses) },
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

CStructMassDlg::~CStructMassDlg()
{
	DeleteChildDlgs();
}

void CStructMassDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStructMassDlg)
	DDX_Control(pDX, IDC_TM_MASS_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_MASS_MENU_COMBO, m_ComboMenu);
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStructMassDlg, MChildFormView)
	//{{AFX_MSG_MAP(CStructMassDlg)
	ON_CBN_SELCHANGE(IDC_MASS_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TM_MASS_TABLE_BTN, OnTmTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStructMassDlg diagnostics

#ifdef _DEBUG
void CStructMassDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CStructMassDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStructMassDlg Implementation Functions
// (ï¿½ï¿½ï¿½ï¿½)
void CStructMassDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 
	
	m_ChildDlgs.Add(new CMassNmasDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_MASS_NMAS__);
	m_TableIDs.Add(ID_QUERY_STRUCT_NMAS);

#ifdef _MGEN
	m_ChildDlgs.Add(new CMassDmasDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_MASS_DMAS__);
	m_TableIDs.Add(0);
#endif
}

void CStructMassDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

// TreeMenu Modeï¿½ï¿½ ï¿½Ø´ï¿½ï¿½Ï´ï¿½ Child Dialogï¿½ï¿½ È°ï¿½ï¿½È­ ï¿½ï¿½Å²ï¿½ï¿½. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CStructMassDlg::ShowChildDlgByID(int nID)
{
	if(nID == __TMMODE_STRUCT_MASS__)
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

void CStructMassDlg::ShowCurChildDlg(int nDlgIndex)
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
	
	/////////////////////////////////////////////////////////////
	// Scroll Sizeï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ñ´ï¿½. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);

	// ¿¹¿ÜÃ³¸®. Apply¸¦ ¼û°Ü¾ßÇÏ´Â °æ¿ì
// Child Dlg °¡ ¿©·¯°³ÀÎ °æ¿ì
	mit::frx::MBarBaseDlg* pParent = (mit::frx::MBarBaseDlg*)GetParent();
	pParent->SetApplyBtnShowHide(ShowApplyBtn() ? SW_SHOW : SW_HIDE);
}

bool CStructMassDlg::ShowApplyBtn()
{
	if (m_nCurChildDlg < 0 || m_ChildDlgs.GetSize() - 1 < m_nCurChildDlg)
	{
		ASSERT(0);  return true;
	}

	switch (m_DlgIDs.GetAt(m_nCurChildDlg))
	{
	case __TMMODE_STRUCT_MASS_DMAS__:
		return false;
	default:
		return true;
	}
}
void CStructMassDlg::LayoutChildDialog()
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
	// Scroll Sizeï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ñ´ï¿½. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);
}

int CStructMassDlg::_GetHeightFromFont()
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

void CStructMassDlg::AdjustComboListBox()
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

void CStructMassDlg::InitComboMenu()
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

void CStructMassDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  


/////////////////////////////////////////////////////////////////////////////
// CStructMassDlg message handlers

void CStructMassDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CStructMassDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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

void CStructMassDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CStructMassDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CStructMassDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlgï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Î¸ï¿½ï¿½ï¿½ CBarTreeMenuï¿½ï¿½ m_nCurModeï¿½ï¿½ Update
	// ï¿½ï¿½ï¿½ï¿½ ï¿½Ê¾ï¿½ Menuï¿½ï¿½ï¿½ï¿½ Selectï¿½Øµï¿½ ï¿½Ø´ï¿½ Dialogï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ê´ï¿½ ï¿½ï¿½ì°¡ ï¿½ß»ï¿½
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);	
}

int CStructMassDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CStructMassDlg::OnTmTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));		
}
/////////////////////////////////////////////////////////////////////
// By L.C.G
void CStructMassDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	CMouseEdit::ImNotAssocWindowST();
	CMouseEdit::DisConnectAssocWindowST();
	if(CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);  
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1,-1);
	}
	//if(bActivate)
	{
		int nCurID;
		if(m_nCurChildDlg != -1) nCurID = m_nCurChildDlg;
		else nCurID = 0;

		ShowCurChildDlg(nCurID);
		CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[nCurID]);

		// ï¿½ï¿½ï¿½Ìºï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½×¸ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ Table ï¿½ï¿½Æ° Disable
		if (m_TableIDs[nCurID] > 0) m_wndTableBtn.EnableWindow(TRUE);
		else m_wndTableBtn.EnableWindow(FALSE);
	}
}
/////////////////////////////////////////////////////////////////////
// By L.C.G
// ï¿½ï¿½ï¿½ï¿½ Unselect ï¿½ï¿½ Notify ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ 
void CStructMassDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}

CWnd* CStructMassDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}