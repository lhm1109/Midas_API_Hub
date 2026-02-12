// StructHighTemperatureMatLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StructHighTemperatureMatLinkDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "MhtpMaterialDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CFormViewInBar
/////////////////////////////////////////////////////////////////////////////
// CStructHighTemperatureMatLinkDlg dialog
IMPLEMENT_DYNCREATE(CStructHighTemperatureMatLinkDlg, CFormViewInBar)


CStructHighTemperatureMatLinkDlg::CStructHighTemperatureMatLinkDlg()
	: CFormViewInBar(CStructHighTemperatureMatLinkDlg::IDD)
{
	//{{AFX_DATA_INIT(CStructHighTemperatureMatLinkDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CreateChildDlgs();
}

CStructHighTemperatureMatLinkDlg::~CStructHighTemperatureMatLinkDlg()
{
	DeleteChildDlgs();
}


void CStructHighTemperatureMatLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStructHighTemperatureMatLinkDlg)
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStructHighTemperatureMatLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CStructHighTemperatureMatLinkDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



#ifdef _DEBUG
void CStructHighTemperatureMatLinkDlg::AssertValid() const
{
	CFormViewInBar::AssertValid();
}

void CStructHighTemperatureMatLinkDlg::Dump(CDumpContext& dc) const
{
	CFormViewInBar::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CStructHighTemperatureMatLinkDlg message handlers

int CStructHighTemperatureMatLinkDlg::_GetHeightFromFont()
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

BOOL CStructHighTemperatureMatLinkDlg::ShowChildDlgByID(int nID)
{
	
	if(nID == __TMMODE_STRUCT_MHTP_MAT_LINK_CH__)
	{
		ShowCurChildDlg(0);
	}
	
	
	int nIDCount = m_DlgIDs.GetSize();
	for(int i = 0 ; i < nIDCount ; i++)
	{
		if(nID == m_DlgIDs[i])
		{
			ShowCurChildDlg(i);  
			//m_ComboMenu.SetCurSel(i);
			//if (m_TableIDs[i] > 0) m_wndTableBtn.EnableWindow(TRUE);
			//else m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return TRUE;
	
}

void CStructHighTemperatureMatLinkDlg::ShowCurChildDlg(int nDlgIndex)
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
	// Scroll Size를 조정한다. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);
}

void CStructHighTemperatureMatLinkDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 

	m_ChildDlgs.Add(new CMhtpMaterialDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_MHTP_MAT_LINK_CH__);
	m_TableIDs.Add(0);  // 테이블 없으면 0으로

}

void CStructHighTemperatureMatLinkDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

void CStructHighTemperatureMatLinkDlg::InitContent()
{
	
}  

void CStructHighTemperatureMatLinkDlg::InitComboMenu()
{

}

void CStructHighTemperatureMatLinkDlg::OnInitialUpdate() 
{
	CFormViewInBar::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);	
}

void CStructHighTemperatureMatLinkDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CFormViewInBar::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CStructHighTemperatureMatLinkDlg::LayoutChildDialog()
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
	// Scroll Size를 조정한다. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);
}

void CStructHighTemperatureMatLinkDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	CMouseEdit::ImNotAssocWindowST();
	CMouseEdit::DisConnectAssocWindowST();
	/*
	CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE); 
	CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE); 
	*/

	{
		int nCurID;
		if(m_nCurChildDlg != -1) nCurID = m_nCurChildDlg;
		else nCurID = 0;

		ShowCurChildDlg(nCurID);
		CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[nCurID]);

		// 테이블 없는 항목인 경우 Table 버튼 Disable
		//if (m_TableIDs[nCurID] > 0) m_wndTableBtn.EnableWindow(TRUE);
		//else m_wndTableBtn.EnableWindow(FALSE);
	}
}

void CStructHighTemperatureMatLinkDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}

CWnd* CStructHighTemperatureMatLinkDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}
