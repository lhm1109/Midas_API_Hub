// DgnSrcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSrcDlg.h"

#include "DgnSrcSectDlg.h"
#include "DgnSrcBeamDlg.h"


#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"    // For GetDocPoint().

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcDlg

IMPLEMENT_DYNCREATE(CDgnSrcDlg, MChildFormView)


CDgnSrcDlg::CDgnSrcDlg()
	: MChildFormView(CDgnSrcDlg::IDD)
{
	//{{AFX_DATA_INIT(CDgnSrcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CreateChildDlgs();
}

CDgnSrcDlg::~CDgnSrcDlg()
{
	DeleteChildDlgs();
}

void CDgnSrcDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSrcDlg)
	DDX_Control(pDX, IDC_DGN_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_DGN_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_DGN_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnSrcDlg, MChildFormView)
	//{{AFX_MSG_MAP(CDgnSrcDlg)
	ON_CBN_SELCHANGE(IDC_DGN_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DGN_TABLE_BTN, OnDgnTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcDlg diagnostics

#ifdef _DEBUG
void CDgnSrcDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CDgnSrcDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcDlg message handlers

// 인영근 : 추가시 고칠곳 3 : class와 TreeMenuMode에 정의한 ID
void CDgnSrcDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 

	#if defined(_CIVIL)
	  m_ChildDlgs.Add(new CDgnSrcSectDlg);
	  m_DlgIDs.Add(__TMMODE_DGN_SRC_REBS__);
		m_TableIDs.Add(ID_QUERY_DGN_SRC_REBS);
	#else
	  m_ChildDlgs.Add(new CDgnSrcBeamDlg);
	  m_DlgIDs.Add(__TMMODE_DGN_SRC_RBSB__);
		m_TableIDs.Add(ID_QUERY_DGN_SRC_RBSB);
	  m_ChildDlgs.Add(new CDgnSrcSectDlg);
	  m_DlgIDs.Add(__TMMODE_DGN_SRC_REBS__);
		m_TableIDs.Add(ID_QUERY_DGN_SRC_REBS);
	#endif

}

void CDgnSrcDlg::DeleteChildDlgs()
{
	for(int i=0; i<m_ChildDlgs.GetSize(); i++)	delete m_ChildDlgs[i];
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CDgnSrcDlg::ShowChildDlgByID(int nID)
{
	if(nID==__TMMODE_DGN_SRC__)  // 인영근 : 이 파일 Copy시 여기 수정
	{
		ShowCurChildDlg(0);  
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for(int i=0; i<nIDCount; i++)
	{
		if(nID==m_DlgIDs[i])
		{
			ShowCurChildDlg(i);  
			m_ComboMenu.SetCurSel(i);
			if(m_TableIDs[i] > 0)	m_wndTableBtn.EnableWindow(TRUE);
			else									m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CDgnSrcDlg::ShowCurChildDlg(int nDlgIndex)
{
	if(m_nCurChildDlg==nDlgIndex)	return;

	if(nDlgIndex > m_ChildDlgs.GetSize()-1)
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
	// Scroll Size를 조정한다. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();

	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();

	ScrSize += TSize;

	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);
}

int CDgnSrcDlg::GetHeightFromFont()
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

void CDgnSrcDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	Update_ComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CDgnSrcDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

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
}

void CDgnSrcDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);
}

int CDgnSrcDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDgnSrcDlg::OnDgnTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}

void CDgnSrcDlg::Update_ComboMenu()
{
	int iSelID = m_ComboMenu.GetCurSel();
	// Initialize Data.
	m_ComboMenu.ResetContent();
	#if defined(_CIVIL)
		m_ComboMenuStr[0] = _LS(IDS_TREE_TEXT_SRC_MOD_COLM);
	#else
	  m_ComboMenuStr[0] = _LS(IDS_TREE_TEXT_SRC_MOD_BEAM);
	  m_ComboMenuStr[1] = _LS(IDS_TREE_TEXT_SRC_MOD_COLM);
	#endif

	int wHeight = (sizeof(m_ComboMenuStr)/sizeof(*m_ComboMenuStr)+1) * GetHeightFromFont();
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL,0,0,rectWnd.Width(),wHeight,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	// Add String at ComboBox.
	for(int i=0; i<sizeof(m_ComboMenuStr)/sizeof(*m_ComboMenuStr); i++)
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	// Recalculate Width.
	CClientDC dc(this);
	int nSize = sizeof(m_ComboMenuStr)/sizeof(CString);
	CSize sizeMax, sizeCur;
	sizeMax = dc.GetTextExtent(m_ComboMenuStr[0]);
	for(int k=1; k<nSize; k++)
	{
		sizeCur = dc.GetTextExtent(m_ComboMenuStr[k]);
		if(sizeCur.cx > sizeMax.cx)	sizeMax.cx = sizeCur.cx;
	}
	if(sizeMax.cx > m_ComboMenu.GetDroppedWidth())	m_ComboMenu.SetDroppedWidth(sizeMax.cx);

	if(iSelID < 0)	m_ComboMenu.SetCurSel(0);
	else						m_ComboMenu.SetCurSel(iSelID);
}

void CDgnSrcDlg::OnTmExecute()
{
	if ( m_nCurChildDlg < m_ChildDlgs.GetSize() )
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CDgnSrcDlg::OnTmClose()
{
	if ( CDBDoc::GetDocPoint() )
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}