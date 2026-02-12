// CMSihoMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSihoMainDlg.h"

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

#include "CMSihoElemDlg.h"
#include "CMSihgGLinkDlg.h"
#include "CMSihpPSSPOTDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//int ____bNotifyComboMv = TRUE;
/////////////////////////////////////////////////////////////////////////////
// CCMSihoMainDlg dialog

#define CFormView CCMDlgBarBase

IMPLEMENT_DYNCREATE(CCMSihoMainDlg, CFormView)


CCMSihoMainDlg::CCMSihoMainDlg(CWnd* pParent /*=NULL*/)
	: CFormView(CCMSihoMainDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSihoMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	CArray<const TCHAR*, const TCHAR*> ComboStrs;
	ComboStrs.Add(_LS(IDS_CMD_SIHO));
	ComboStrs.Add(_LS(IDS_CMD_SIHG));

#if defined (_MEC)
	ComboStrs.Add(_LS(IDS_CMD_SIHP));
#endif
	
	m_ComboMenuStr.RemoveAll();
	int nStrs = ComboStrs.GetSize();
	for(int i = 0 ; i < nStrs ; i++)
	{
		m_ComboMenuStr.Add(ComboStrs[i]);
	}

	CreateChildDlgs();
}

CCMSihoMainDlg::~CCMSihoMainDlg()
{
	DeleteChildDlgs();
}


void CCMSihoMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSihoMainDlg)
	DDX_Control(pDX, IDC_CMD_TABLE_BTN, m_wndTableButton);
	DDX_Control(pDX, IDC_CMD_MENU_COMBO, m_ComboMenu);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSihoMainDlg, CFormView)
	//{{AFX_MSG_MAP(CCMSihoMainDlg)
	ON_BN_CLICKED(IDC_CMD_TABLE_BTN, OnCmdTableButton)
	ON_CBN_SELCHANGE(IDC_CMD_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSihoMainDlg message handlers

void CCMSihoMainDlg::CreateChildDlgs()
{ 
	m_nCurChildDlg = -1; 
	
	m_ChildDlgs.Add(new CCMSihoElemDlg);
	m_ChildDlgs.Add(new CCMSihgGLinkDlg);

	m_DlgIDs.Add(__TMMODE_CMD_SIHO_ELEM__);
	m_DlgIDs.Add(__TMMODE_CMD_SIHO_GLINK__);

#if defined (_MEC)
	m_ChildDlgs.Add(new CCMSihpPSSPOTDlg);
	m_DlgIDs.Add(__TMMODE_CMD_SIHO_PSSPOT__);
#endif

	//m_TableIDs.Add();
}

void CCMSihoMainDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
BOOL CCMSihoMainDlg::ShowChildDlgByID(int nID)
{
	if (nID == __TMMODE_CMD_IEHA__)
	{
		//____bNotifyComboMv = FALSE;    
		m_ComboMenu.SetCurSel(0);
		//____bNotifyComboMv = TRUE;    
		ShowCurChildDlg(0);  
		
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for (int i = 0 ; i < nIDCount ; i++)
	{
		if (nID == m_DlgIDs[i])
		{
			//____bNotifyComboMv = FALSE;    
			m_ComboMenu.SetCurSel(i);
			//____bNotifyComboMv = TRUE;    
			ShowCurChildDlg(i);  

// 			if (m_TableIDs[i] > 0) m_wndTableButton.EnableWindow(TRUE);
// 			else m_wndTableButton.EnableWindow(FALSE);
			m_wndTableButton.EnableWindow(FALSE);


			return TRUE;
		}
	}
	return FALSE;
}

void CCMSihoMainDlg::ShowCurChildDlg(int nDlgIndex)
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
	// Scroll Size를 조정한다. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);
}

void CCMSihoMainDlg::LayoutChildDialog()
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

int CCMSihoMainDlg::_GetHeightFromFont()
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

void CCMSihoMainDlg::InitComboMenu()
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

void CCMSihoMainDlg::AdjustComboListBox()
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

void CCMSihoMainDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  


/////////////////////////////////////////////////////////////////////////////
// CCMSihoMainDlg message handlers
void CCMSihoMainDlg::OnInitialUpdate() 
{
	CFormViewInBar::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);	
}

void CCMSihoMainDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{	
	CFormViewInBar::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CCMSihoMainDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
	{
		m_ChildDlgs[m_nCurChildDlg]->Execute();
	}
}

void CCMSihoMainDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU_MENU__);
}

void CCMSihoMainDlg::OnComboMenuSelect() 
{
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	//if(____bNotifyComboMv)
	//{
		int nCurI = m_ComboMenu.GetCurSel();
		CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//}
	//ShowCurChildDlg(nCurI);	
}

int CCMSihoMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormViewInBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CCMSihoMainDlg::OnCmdTableButton() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));				
}

CWnd* CCMSihoMainDlg::GetDlgByID(int nID)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (nID == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}


/*
void CCMSihoMainDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU_MENU__);
		break;
	default:
		break;
	}
}

void CCMSihoMainDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bMvcdChanged = FALSE;
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
		case(UR_MVCD_ADD):
		case(UR_MVCD_DEL):
			bMvcdChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if (bMvcdChanged) 
	{
		// 코드가 바뀌면 무조건 닫아버린다.
		CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU_MENU__);
	}
}
*/
