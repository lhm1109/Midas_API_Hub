// StructMeshDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StructMeshDlg.h"

// Dialog�߰��� 3�� ���� : '(����)'�̶�� �ڸ�Ʈ ģ ��
// ���� : ���� ���߾� �� ��

// (����)
#include "MeshAutoDlg.h"
#include "MeshMapDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStructMeshDlg

IMPLEMENT_DYNCREATE(CStructMeshDlg, MChildFormView)

CStructMeshDlg::CStructMeshDlg()
	: MChildFormView(CStructMeshDlg::IDD)
{
	//{{AFX_DATA_INIT(CStructMeshDlg)
	//}}AFX_DATA_INIT

	struct T_COMBO_ITEM
	{
		int nMask;
		int nMode;
	};
	// (����) // redesigned by J.B.Seon 2002.05.22
	T_COMBO_ITEM aItem[] = {    
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_MESH_AUTO__ },
		//{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_MESH_MAP_KE__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_MESH_MAP__ },    
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
		AddChildDlgs(aItem[i].nMode);
	}
 
	m_nCurChildDlg = -1; 
}

CStructMeshDlg::~CStructMeshDlg()
{
	DeleteChildDlgs();
}

void CStructMeshDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStructMeshDlg)
	DDX_Control(pDX, IDC_TM_MESH_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_MESH_MENU_COMBO, m_ComboMenu);
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStructMeshDlg, MChildFormView)
	//{{AFX_MSG_MAP(CStructMeshDlg)
	ON_CBN_SELCHANGE(IDC_MESH_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TM_MESH_TABLE_BTN, OnTmTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStructMeshDlg diagnostics

#ifdef _DEBUG
void CStructMeshDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CStructMeshDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStructMeshDlg Implementation Functions
// (����)
void CStructMeshDlg::AddChildDlgs(int nMode)
{
	m_DlgIDs.Add(nMode);
	switch(nMode)
	{
	case __TMMODE_STRUCT_MESH_AUTO__:
		m_ComboMenuStr.Add(_LS(IDS_TM_Automesh));
		m_ChildDlgs.Add(new CMeshAutoDlg);
		//m_TableIDs.Add(ID_QUERY_STRUCT_CONSTRAINT);
		break;
	/*
	case __TMMODE_STRUCT_MESH_MAP_KE__:
		m_ComboMenuStr.Add(_LS(IDS_TM_Mapmesh_KE));
		m_ChildDlgs.Add(new CMeshMapKEDlg);
		//m_TableIDs.Add(ID_QUERY_STRUCT_NSPR);
		break;
	*/
	case __TMMODE_STRUCT_MESH_MAP__:
		m_ComboMenuStr.Add(_LS(IDS_TM_Mapmesh));
		m_ChildDlgs.Add(new CMeshMapDlg);
		//m_TableIDs.Add(ID_QUERY_STRUCT_GENSPRING);
		break;  
	default: ASSERT(0); break;
	}
}

void CStructMeshDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

// TreeMenu Mode�� �ش��ϴ� Child Dialog�� Ȱ��ȭ ��Ų��. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CStructMeshDlg::ShowChildDlgByID(int nID)
{
	/*
	if(nID == __TMMODE_STRUCT_MESH__)
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
			m_ComboMenu.SetCurSel(i);
			if(m_TableIDs.GetSize() > 0)
			{
				if (m_TableIDs[i] > 0) m_wndTableBtn.EnableWindow(TRUE);
				else m_wndTableBtn.EnableWindow(FALSE);
			}      
			return TRUE;
		}
	}
	return FALSE;
}

void CStructMeshDlg::ShowCurChildDlg(int nDlgIndex)
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

int CStructMeshDlg::_GetHeightFromFont()
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

void CStructMeshDlg::AdjustComboListBox()
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

	//dx += ::GetSystemMetrics(SM_CXVSCROLL); + 2*::GetSystemMetrics(SM_CXEDGE);

	if (m_ComboMenu.GetDroppedWidth() < dx)
	{
		m_ComboMenu.SetDroppedWidth(dx);
		ASSERT(m_ComboMenu.GetDroppedWidth() == dx);
	}
}

void CStructMeshDlg::InitComboMenu()
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

void CStructMeshDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  


/////////////////////////////////////////////////////////////////////////////
// CStructMeshDlg message handlers

void CStructMeshDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_wndTableBtn.EnableWindow(FALSE);

	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CStructMeshDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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

void CStructMeshDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	/*CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));*/
	ShowCurChildDlg(nCurI);	
}

int CStructMeshDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CStructMeshDlg::OnTmTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;

	if(m_TableIDs.GetSize() > 0)
	{
		if (m_TableIDs[nCurSel] <= 0) return;
		AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));
	}	
	else
		ASSERT(0);
}
void CStructMeshDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
	{
		m_ChildDlgs[m_nCurChildDlg]->UpdateData(TRUE);
		m_ChildDlgs[m_nCurChildDlg]->Execute();
	}
}

void CStructMeshDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}
//////////////////////////////////////////////////////////////////
// By L.C.G
void CStructMeshDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	CMouseEdit::ImNotAssocWindowST();
	CMouseEdit::DisConnectAssocWindowST();

	if(CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE); 
	}
	//if(bActivate)
	{
		int nCurID;
		if(m_nCurChildDlg != -1) nCurID = m_nCurChildDlg;
		else nCurID = 0;

		ShowCurChildDlg(nCurID);
		CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[nCurID]);

		// ���̺� ���� �׸��� ��� Table ��ư Disable
		if(m_TableIDs.GetSize() > 0)
		{
			if (m_TableIDs[nCurID] > 0) m_wndTableBtn.EnableWindow(TRUE);
			else m_wndTableBtn.EnableWindow(FALSE);
		}
	}  
}
// By L.C.G
// ���� Unselect �� Notify ���� ���� 
void CStructMeshDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}

CWnd* CStructMeshDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}
