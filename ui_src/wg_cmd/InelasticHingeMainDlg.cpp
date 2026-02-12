// InelasticHingeMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "InelasticHingeMainDlg.h"

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

#include "InelasticHingeAssignDlg.h"
#include "InelasticHingeAssignDlg_MEC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//int ____bNotifyComboMv = TRUE;
/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeMainDlg dialog

#define CFormView CCMDlgBarBase

IMPLEMENT_DYNCREATE(CInelasticHingeMainDlg, CFormView)


CInelasticHingeMainDlg::CInelasticHingeMainDlg(CWnd* pParent /*=NULL*/)
	: CFormView(CInelasticHingeMainDlg::IDD)
{
	//{{AFX_DATA_INIT(CInelasticHingeMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	CArray<const TCHAR*, const TCHAR*> ComboStrs;
	ComboStrs.Add(_LS(IDS_WG_CMD__ADDD__Assign_Inelastic_Hinge));

	m_ComboMenuStr.RemoveAll();
	int nStrs = ComboStrs.GetSize();
	for(int i = 0 ; i < nStrs ; i++)
	{
		m_ComboMenuStr.Add((TCHAR*) ComboStrs[i]);
	}

	CreateChildDlgs();
}

CInelasticHingeMainDlg::~CInelasticHingeMainDlg()
{
	DeleteChildDlgs();
}


void CInelasticHingeMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInelasticHingeMainDlg)
	DDX_Control(pDX, IDC_CMD_TABLE_BUTTON, m_wndTableButton);
	DDX_Control(pDX, IDC_DBAR_FUNC_COMBO, m_ComboMenu);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInelasticHingeMainDlg, CFormView)
	//{{AFX_MSG_MAP(CInelasticHingeMainDlg)
	ON_BN_CLICKED(IDC_CMD_TABLE_BUTTON, OnCmdTableButton)
	ON_CBN_SELCHANGE(IDC_DBAR_FUNC_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeMainDlg message handlers

void CInelasticHingeMainDlg::CreateChildDlgs()
{ 
	m_nCurChildDlg = -1; 

#if defined (_MEC)
	m_ChildDlgs.Add(new CInelasticHingeAssignDlg_MEC);
#else
	m_ChildDlgs.Add(new CInelasticHingeAssignDlg);
#endif
	m_DlgIDs.Add(__TMMODE_CMD_IEHA__);
	m_TableIDs.Add(ID_QUERY_STRUCT_IEHG);  
}

void CInelasticHingeMainDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

// TreeMenu Mode�� �ش��ϴ� Child Dialog�� Ȱ��ȭ ��Ų��. 
BOOL CInelasticHingeMainDlg::ShowChildDlgByID(int nID)
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
			if (m_TableIDs[i] > 0) m_wndTableButton.EnableWindow(TRUE);
			else m_wndTableButton.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CInelasticHingeMainDlg::ShowCurChildDlg(int nDlgIndex)
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

void CInelasticHingeMainDlg::LayoutChildDialog()
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

int CInelasticHingeMainDlg::_GetHeightFromFont()
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

void CInelasticHingeMainDlg::InitComboMenu()
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

void CInelasticHingeMainDlg::AdjustComboListBox()
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

void CInelasticHingeMainDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  


/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeMainDlg message handlers
void CInelasticHingeMainDlg::OnInitialUpdate() 
{
	CFormViewInBar::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CInelasticHingeMainDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{	
	CFormViewInBar::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CInelasticHingeMainDlg::OnComboMenuSelect() 
{
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	//if(____bNotifyComboMv)
	//{
		int nCurI = m_ComboMenu.GetCurSel();
		CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//}
	//ShowCurChildDlg(nCurI);	
}

int CInelasticHingeMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormViewInBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CInelasticHingeMainDlg::OnCmdTableButton() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));				
}

CWnd* CInelasticHingeMainDlg::GetDlgByID(int nID)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (nID == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}

void CInelasticHingeMainDlg::OnTmClose()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CInelasticHingeMainDlg::OnTmExecute()
{
	auto nSize = m_ChildDlgs.GetSize();
	// ���� �ϳ��ۿ� �����Ƿ� �����ϰ� ó���Ѵ�. 
	if (nSize)
	{
		m_ChildDlgs[0]->Execute();
	}
}


/*
void CInelasticHingeMainDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CInelasticHingeMainDlg::UpdateBuffer()
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
		// �ڵ尡 �ٲ�� ������ �ݾƹ�����.
		CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU_MENU__);
	}
}
*/
