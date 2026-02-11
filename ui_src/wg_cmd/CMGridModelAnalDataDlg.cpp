// CMGridModelAnalDataDlg.cpp: implementation of the CCMGridModelAnalDataDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridModelAnalDataDlg.h"

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

#include "CMGridSpanLengthDlg.h"
#include "CMGridImpactFactorDlg.h"
#include "CMGridP1LoadFactorDlg.h"
#include "CMGridCrossReferenceDlg.h"
#include "CMGridMassConverterDlg.h"
#include "CMGridImpactNLoadDlg.h"
#include "CMGridElemStructTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int ____bNotifyComboGrid = TRUE;
/////////////////////////////////////////////////////////////////////////////
// CCMGridModelAnalDataDlg dialog

//#define CFormView CCMDlgBarBase
IMPLEMENT_DYNCREATE(CCMGridModelAnalDataDlg, MChildFormView)

CCMGridModelAnalDataDlg::CCMGridModelAnalDataDlg(CWnd* pParent /*=NULL*/)
	: MChildFormView(CCMGridModelAnalDataDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMGridModelAnalDataDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ComboStrs.Add(_LS(IDS_CMD_GRID_SPAN_LENGTH));  
	ComboStrs.Add(_LS(IDS_CMD_GRID_TITLE_ELEM_STRUCT_TYPE));
	ComboStrs.Add(_LS(IDS_CMD_GRID_TITLE_IMPACT_FACTOR));
	ComboStrs.Add(_LS(IDS_CMD_GRID_TITLE_P1LOAD_FACTPR));
	ComboStrs.Add(_LS(IDS_CMD_GRID_TITLE_IMPACTNP1));    
	ComboStrs.Add(_LS(IDS_CMD_GRID_TITLE_CROSS_REFERENCE));  
	ComboStrs.Add(_LS(IDS_CMD_GRID_TITLE_MASS_CONVERTER));
	
	CreateChildDlgs();
}

CCMGridModelAnalDataDlg::~CCMGridModelAnalDataDlg()
{
	DeleteChildDlgs();
}

void CCMGridModelAnalDataDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridModelAnalDataDlg)
	DDX_Control(pDX, IDC_CMD_TABLE_BUTTON, m_wndTableButton);
	DDX_Control(pDX, IDC_DBAR_FUNC_COMBO, m_ComboMenu);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridModelAnalDataDlg, MChildFormView)
	//{{AFX_MSG_MAP(CCMGridModelAnalDataDlg)
	ON_BN_CLICKED(IDC_CMD_TABLE_BUTTON, OnCmdTableButton)
	ON_CBN_SELCHANGE(IDC_DBAR_FUNC_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMGridModelAnalDataDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 
	
	//Span Length
	m_ChildDlgs.Add(new CCMGridSpanLengthDlg);
	m_DlgIDs.Add(__TMMODE_CMD_BSPN__);
	m_TableIDs.Add(0);  

	//Element Struct Type
	m_ChildDlgs.Add(new CCMGridElemStructTypeDlg);
	m_DlgIDs.Add(__TMMODE_CMD_GSTY__);
	m_TableIDs.Add(ID_QUERY_STRUCT_GSTY);

	//Impact Factor
	m_ChildDlgs.Add(new CCMGridImpactFactorDlg);
	m_DlgIDs.Add(__TMMODE_CMD_GIMP__);
	m_TableIDs.Add(ID_QUERY_STRUCT_GIMP);

	//P1 Load Factor
	m_ChildDlgs.Add(new CCMGridP1LoadFactorDlg);
	m_DlgIDs.Add(__TMMODE_CMD_P1LA__);
	m_TableIDs.Add(ID_QUERY_STRUCT_P1LA);

	//Impact anc P1 Load Factor For Link
	m_ChildDlgs.Add(new CCMGridImpactNLoadDlg);
	m_DlgIDs.Add(__TMMODE_CMD_LNKF__);
	m_TableIDs.Add(0);
	
	//Cross Reference
	m_ChildDlgs.Add(new CCMGridCrossReferenceDlg);
	m_DlgIDs.Add(__TMMODE_CMD_CREF__);
	m_TableIDs.Add(0);  

	//Mass Converter
	m_ChildDlgs.Add(new CCMGridMassConverterDlg);
	m_DlgIDs.Add(__TMMODE_CMD_GLTM__);
	m_TableIDs.Add(0);  
}

void CCMGridModelAnalDataDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

BOOL CCMGridModelAnalDataDlg::ShowChildDlgByID(int nID)
{
	if(nID == __TMMODE_CMD_GMAD__)
	{
		____bNotifyComboGrid = FALSE;    
		m_ComboMenu.SetCurSel(0);
		____bNotifyComboGrid = TRUE;    
		ShowCurChildDlg(0);  
		
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for(int i = 0 ; i < nIDCount ; i++)
	{
		if(nID == m_DlgIDs[i])
		{
			____bNotifyComboGrid = FALSE;    
			m_ComboMenu.SetCurSel(i);
			____bNotifyComboGrid = TRUE;    
			ShowCurChildDlg(i);  
			if (m_TableIDs[i] > 0) m_wndTableButton.EnableWindow(TRUE);
			else m_wndTableButton.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CCMGridModelAnalDataDlg::ShowCurChildDlg(int nDlgIndex)
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
	// Scroll Size�� �����Ѵ�. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);
}

void CCMGridModelAnalDataDlg::LayoutChildDialog()
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

int CCMGridModelAnalDataDlg::_GetHeightFromFont()
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

void CCMGridModelAnalDataDlg::InitComboMenu()
{
	int wHeight;
	//wHeight = (sizeof(m_ComboMenuStr)/sizeof(**m_ComboMenuStr)+1) * _GetHeightFromFont();
	wHeight = (ComboStrs.GetSize()+1) * _GetHeightFromFont();
	
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0,rectWnd.Width(),wHeight,
				         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	//for( int i=0;  i<sizeof(m_ComboMenuStr)/sizeof(**m_ComboMenuStr); ++i )
	for (int i=0; i<ComboStrs.GetSize(); i++)
		m_ComboMenu.AddString(ComboStrs[i]);
	
	m_ComboMenu.SetCurSel(0);
}

void CCMGridModelAnalDataDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}
	
/////////////////////////////////////////////////////////////////////////////
// CCMGridModelAnalDataDlg message handlers
int CCMGridModelAnalDataDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CCMGridModelAnalDataDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CCMGridModelAnalDataDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CCMGridModelAnalDataDlg::OnComboMenuSelect()
{
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	int nCurI = m_ComboMenu.GetCurSel();
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
}

void CCMGridModelAnalDataDlg::OnCmdTableButton() 
{
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));				
}

void CCMGridModelAnalDataDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	
}

void CCMGridModelAnalDataDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}

CWnd* CCMGridModelAnalDataDlg::GetDlgByID(int nID)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (nID == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}

void CCMGridModelAnalDataDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CCMGridModelAnalDataDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}