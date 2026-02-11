// StructTimeDepMatLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StructTimeDepMatLinkDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "HydrTimeDepMatDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog MChildFormView
/////////////////////////////////////////////////////////////////////////////
// CStructTimeDepMatLinkDlg dialog
IMPLEMENT_DYNCREATE(CStructTimeDepMatLinkDlg, MChildFormView)


CStructTimeDepMatLinkDlg::CStructTimeDepMatLinkDlg()
	: MChildFormView(CStructTimeDepMatLinkDlg::IDD)
{
	//{{AFX_DATA_INIT(CStructTimeDepMatLinkDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CreateChildDlgs();
}

CStructTimeDepMatLinkDlg::~CStructTimeDepMatLinkDlg()
{
	DeleteChildDlgs();
}


void CStructTimeDepMatLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStructTimeDepMatLinkDlg)
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStructTimeDepMatLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CStructTimeDepMatLinkDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



#ifdef _DEBUG
void CStructTimeDepMatLinkDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CStructTimeDepMatLinkDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

void CStructTimeDepMatLinkDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CStructTimeDepMatLinkDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

/////////////////////////////////////////////////////////////////////////////
// CStructTimeDepMatLinkDlg message handlers

int CStructTimeDepMatLinkDlg::_GetHeightFromFont()
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

BOOL CStructTimeDepMatLinkDlg::ShowChildDlgByID(int nID)
{
	
	if(nID == __TMMODE_STRUCT_HYDR_TIME_DEP_MAT__)
	{
		ShowCurChildDlg(0);
	}
	
	/*
	if(nID == __TMMODE_STRUCT_HYDR__)
	{
		ShowCurChildDlg(0);  
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}
	*/

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

void CStructTimeDepMatLinkDlg::ShowCurChildDlg(int nDlgIndex)
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

void CStructTimeDepMatLinkDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 

	m_ChildDlgs.Add(new CHydrTimeDepMatDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_HYDR_TIME_DEP_MAT__);
	m_TableIDs.Add(0);  // ���̺� ������ 0����
	
	/*
	m_ChildDlgs.Add(new CHydrTemperDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_HYDR_TEMPER__);
	m_TableIDs.Add(ID_QUERY_STRUCT_HSPT);  // ���̺� ������ 0����    

	m_ChildDlgs.Add(new CHydrHeatDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_HYDR_HEAT__);
	m_TableIDs.Add(ID_QUERY_STRUCT_HAHS);  // ���̺� ������ 0����    

	m_ChildDlgs.Add(new CHydrConvectDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_HYDR_CONVECT__);
	m_TableIDs.Add(ID_QUERY_STRUCT_HECB);  // ���̺� ������ 0����

	m_ChildDlgs.Add(new CHydrMaterialDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_HYDR_MATERIAL__);
	m_TableIDs.Add(0);  // ���̺� ������ 0����

	m_ChildDlgs.Add(new CHydrTimeDepMatDlg);
	m_DlgIDs.Add(__TMMODE_STRUCT_HYDR_TIME_DEP_MAT__);
	m_TableIDs.Add(0);  // ���̺� ������ 0����
	*/

}

void CStructTimeDepMatLinkDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

void CStructTimeDepMatLinkDlg::InitContent()
{
	//CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  

void CStructTimeDepMatLinkDlg::InitComboMenu()
{
	/*
	int wHeight;
	wHeight = (sizeof(m_ComboMenuStr)/sizeof(*m_ComboMenuStr)+1) * _GetHeightFromFont();
	
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0,rectWnd.Width(),wHeight,
				         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	for( int i=0;  i<sizeof(m_ComboMenuStr)/sizeof(*m_ComboMenuStr); ++i )
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	m_ComboMenu.SetCurSel(0);
	*/
}

void CStructTimeDepMatLinkDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CStructTimeDepMatLinkDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CStructTimeDepMatLinkDlg::LayoutChildDialog()
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

void CStructTimeDepMatLinkDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
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

		// ���̺� ���� �׸��� ��� Table ��ư Disable
		//if (m_TableIDs[nCurID] > 0) m_wndTableBtn.EnableWindow(TRUE);
		//else m_wndTableBtn.EnableWindow(FALSE);
	}
}

void CStructTimeDepMatLinkDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}

CWnd* CStructTimeDepMatLinkDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}
