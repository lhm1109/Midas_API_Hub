#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCommonDlg.h"
#include "SeisCommonSechDlg.h"
#include "SeisCommonSehkDlg.h"
#include "SeisCommonScmtDlg.h"
#include "SeisCommonSerdDlg.h"
#include "SeisCommonRccmDlg.h"
#include "SeisCommonPmcmDlg.h"
// Performance Design
#include "SeisCommonSebeDlg.h"
#include "SeisCommonSecoDlg.h"
#include "SeisCommonSewaDlg.h"
#include "SeisCommonHlfwDlg.h"
#include "SeisCommonSespDlg.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\I_GENStateDefine.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning ( disable : 4267 )

IMPLEMENT_DYNCREATE(CSeisCommonDlg, MChildFormView)

CSeisCommonDlg::CSeisCommonDlg() : MChildFormView(CSeisCommonDlg::IDD)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if ( !pDoc ) { ASSERT(0); return; }

	int nMethod = pDoc->m_pAttrCtrl2->GetSemeMethod();
	GetComboMenuItem(m_acsComboMenu, nMethod);
	CreateChildDlgs();
}

CSeisCommonDlg::~CSeisCommonDlg()
{
	DeleteChildDlgs();
}

void CSeisCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_TABLE_BTN, m_wndTableBtn);
	//DDX_Control(pDX, IDC_TM_CLOSE, m_wndClose);
	//DDX_Control(pDX, IDC_TM_EXECUTE, m_wndExecute);
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_TM_MENU_COMBO, m_ComboMenu);
}

BEGIN_MESSAGE_MAP(CSeisCommonDlg, MChildFormView)
	//ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_CBN_SELCHANGE(IDC_TM_MENU_COMBO, OnComboMenuSelect)
	//ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_TABLE_BTN, OnTmTableBtn)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CSeisCommonDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CSeisCommonDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif

void CSeisCommonDlg::OnViewStartNodeNum()
{
	( ( CDBDoc* ) CTreeMenuBarBase::GetDocST() )->NodeNumberDlg();
}

void CSeisCommonDlg::OnViewStartElemNum()
{
	( ( CDBDoc* ) CTreeMenuBarBase::GetDocST() )->ElemNumberDlg();
}

void CSeisCommonDlg::OnInitialUpdate()
{
	MChildFormView::OnInitialUpdate();
	InitContent();
	InitComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
	CMouseEdit::ImAssocWindowST();
}

void CSeisCommonDlg::GetComboMenuItem(CStringArray& raMenuItem, int nMethod)
{
	raMenuItem.RemoveAll();
	if ( nMethod < 0 ) { ASSERT(0); return; }

	switch ( nMethod )
	{
	case D_SEME_METHOD_KISTEC2013:
		{
			raMenuItem.Add(_LS(IDS_TM_SECH));
			raMenuItem.Add(_LS(IDS_TM_SEHK));
			raMenuItem.Add(_LS(IDS_TM_SCMT));
		}
		break;
	case D_SEME_METHOD_MOE2018:
		{
			raMenuItem.Add(_LS(IDS_TM_SECH));
			raMenuItem.Add(_LS(IDS_TM_SEHK));
			raMenuItem.Add(_LS(IDS_TM_SCMT));
			raMenuItem.Add(_LS(IDS_TM_SERD));
		}
		break;
	case D_SEME_METHOD_MOE2019:
	case D_SEME_METHOD_KISTEC2019:
		{
			raMenuItem.Add(_LS(IDS_TM_SECH));
			raMenuItem.Add(_LS(IDS_TM_SEHK));
			raMenuItem.Add(_LS(IDS_TM_SCMT));
			raMenuItem.Add(_LS(IDS_TM_SERD));
			raMenuItem.Add(_LS(IDS_TM_RCCM));
			raMenuItem.Add(_LS(IDS_TM_PMCM));
		}
		break;
	case D_SEME_METHOD_AIK_G_001_2021:
		{
			raMenuItem.Add(_LS(IDS_TM_SECH)); // Clear Height of Column
			raMenuItem.Add(_LS(IDS_TM_SEBE)); // Seismic Rebar Detail ( Beam )
			raMenuItem.Add(_LS(IDS_TM_SECO)); // Seismic Rebar Detail ( Column )
			raMenuItem.Add(_LS(IDS_TM_SEWA)); // Special Boundary Element ( Wall )
			raMenuItem.Add(_LS(IDS_TM_HLFW)); // Plastic Hinge Length for Fiber Wall
			raMenuItem.Add(_LS(IDS_TM_SESP)); // Shear Span of Beam / Column
		}
		break;
	default: ASSERT(0); break;
	}
}

void CSeisCommonDlg::CreateChildDlgs()
{
	for ( T_SEIS_COMM_DLG_INFO& DlgInfo : m_vChildDlg )
	{
		if ( DlgInfo.pDlg != nullptr )
		{
			delete DlgInfo.pDlg;
		}
	}
	m_vChildDlg.clear();
	m_nCurChildDlg = -1;

	auto L_IsExist = [ ] (const CStringArray& aComboItem, const CString& csItem) -> bool
	{
		INT_PTR nItem = aComboItem.GetCount();
		for ( INT_PTR i = 0; i < nItem; ++i )
		{
			if ( csItem == aComboItem[i] )
			{
				return true;
			}
		}
		return false;
	};

	if ( L_IsExist(m_acsComboMenu, _LS(IDS_TM_SECH)) )
	{
		m_vChildDlg.push_back(T_SEIS_COMM_DLG_INFO(new CSeisCommonSechDlg(),
			__TMMODE_STRUCT_SEIS_CLEAR_HGT_COL__, ID_MAIN_CLEAR_HEIGHT_COLUMN_TB));
	}

	if ( L_IsExist(m_acsComboMenu, _LS(IDS_TM_SEHK)) )
	{
		m_vChildDlg.push_back(T_SEIS_COMM_DLG_INFO(new CSeisCommonSehkDlg(),
			__TMMODE_STRUCT_SEIS_SEISMIC_HOOK__, ID_MAIN_USE_SEISMIC_HOOK_TB));
	}

	if ( L_IsExist(m_acsComboMenu, _LS(IDS_TM_SCMT)) )
	{
		m_vChildDlg.push_back(T_SEIS_COMM_DLG_INFO(new CSeisCommonScmtDlg(),
			__TMMODE_STRUCT_SEIS_COMP_TYPE__, ID_MAIN_SEIS_COMPONENT_TYPE_TB));
	}

	if ( L_IsExist(m_acsComboMenu, _LS(IDS_TM_SERD)) )
	{
		m_vChildDlg.push_back(T_SEIS_COMM_DLG_INFO(new CSeisCommonSerdDlg(),
			__TMMODE_STRUCT_SEIS_REINF_DETAILS__, ID_MAIN_SEIS_REINF_DETAILS_TB));
	}

	if ( L_IsExist(m_acsComboMenu, _LS(IDS_TM_RCCM)) )
	{
		m_vChildDlg.push_back(T_SEIS_COMM_DLG_INFO(new CSeisCommonRccmDlg(),
			__TMMODE_STRUCT_SEIS_RC_COLUMN_M__, ID_MAIN_RC_COLUMN_M_TYPE_TB));
	}

	if ( L_IsExist(m_acsComboMenu, _LS(IDS_TM_PMCM)) )
	{
		m_vChildDlg.push_back(T_SEIS_COMM_DLG_INFO(new CSeisCommonPmcmDlg(),
			__TMMODE_STRUCT_SEIS_PM_CURVE_METHOD__, ID_MAIN_SEIS_PM_CURVE_METHOD_TB));
	}

	if ( L_IsExist(m_acsComboMenu, _LS(IDS_TM_SEBE)) )
	{
		m_vChildDlg.push_back(T_SEIS_COMM_DLG_INFO(new CSeisCommonSebeDlg(),
			__TMMODE_STRUCT_SEIS_PBD_SEBE__, ID_MAIN_SEIS_DETAIL_USED_BEAM_TB));
	}

	if ( L_IsExist(m_acsComboMenu, _LS(IDS_TM_SECO)) )
	{
		m_vChildDlg.push_back(T_SEIS_COMM_DLG_INFO(new CSeisCommonSecoDlg(),
			__TMMODE_STRUCT_SEIS_PBD_SECO__, ID_MAIN_SEIS_DETAIL_USED_COLM_TB));
	}

	if ( L_IsExist(m_acsComboMenu, _LS(IDS_TM_SEWA)) )
	{
		m_vChildDlg.push_back(T_SEIS_COMM_DLG_INFO(new CSeisCommonSewaDlg(),
			__TMMODE_STRUCT_SEIS_PBD_SEWA__, ID_MAIN_SEIS_SPECIAL_BNDR_WALL_TB));
	}

	if ( L_IsExist(m_acsComboMenu, _LS(IDS_TM_HLFW)) )
	{
		m_vChildDlg.push_back(T_SEIS_COMM_DLG_INFO(new CSeisCommonHlfwDlg(),
			__TMMODE_STRUCT_SEIS_PBD_HLFW__, ID_MAIN_SEIS_PLASTIC_LENG_WALL_TB));
	}

	if ( L_IsExist(m_acsComboMenu, _LS(IDS_TM_SESP)) )
	{
		m_vChildDlg.push_back(T_SEIS_COMM_DLG_INFO(new CSeisCommonSespDlg(),
			__TMMODE_STRUCT_SEIS_PBD_SESP__, ID_MAIN_SEIS_SHEAR_SPAN_BMCL_TB));
	}
}

void CSeisCommonDlg::InitComboMenu(int nInitSel)
{
	m_ComboMenu.ResetContent();
	for ( int i = 0; i < m_acsComboMenu.GetSize(); ++i )
	{
		m_ComboMenu.AddString(m_acsComboMenu[i]);
	}
	m_ComboMenu.SetCurSel(nInitSel);
}

void CSeisCommonDlg::OnComboMenuSelect()
{
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update ���� �ʾ�
	// �޴����� �����ص� �ش� ��ȭ���ڰ� ���� �ʴ� ��찡 �߻�
	int nSelDlg = m_ComboMenu.GetCurSel();
	CTreeMenuBarBase::SetModeST(m_vChildDlg[nSelDlg].nDlgID);

	//m_wndExecute.ShowWindow(SW_SHOW);
	//m_wndClose.ShowWindow(SW_SHOW);
}

void CSeisCommonDlg::ShowExecuteButton(UINT nShowHide)
{
	//CWnd* pWnd = GetDlgItem(IDC_TM_EXECUTE);
	//if ( pWnd )
	//{
	//	pWnd->ShowWindow(nShowHide);
	//}
}

void CSeisCommonDlg::OnTmExecute()
{
	if ( m_nCurChildDlg < m_vChildDlg.size() )
	{
		m_vChildDlg[m_nCurChildDlg].pDlg->Execute();
	}
}

void CSeisCommonDlg::OnTmClose()
{
	if ( CDBDoc::GetDocPoint() )
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CSeisCommonDlg::OnTmExecAndClose(BOOL bExecute)
{
	if ( bExecute )
	{
		OnTmExecute();
	}
	else
	{
		OnTmClose();
	}
}

BOOL CSeisCommonDlg::ShowChildDlgByID(int nID)
{
	int nChildDlg = m_vChildDlg.size();
	for ( int i = 0; i < nChildDlg; ++i )
	{
		const T_SEIS_COMM_DLG_INFO& ChildDlg = m_vChildDlg[i];
		if ( nID != ChildDlg.nDlgID ) continue;

		ShowCurChildDlg(i);
		m_ComboMenu.SetCurSel(i);
		if ( ChildDlg.nTableID > 0 )
		{
			m_wndTableBtn.EnableWindow(TRUE);
		}
		else
		{
			m_wndTableBtn.EnableWindow(FALSE);
		}
		return TRUE;
	}
	return FALSE;
}

void CSeisCommonDlg::ShowCurChildDlg(int nDlgIndex)
{
	if ( m_nCurChildDlg == nDlgIndex ) return;
	if ( nDlgIndex > m_vChildDlg.size() - 1 )
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	if ( m_nCurChildDlg >= 0 && m_vChildDlg.size() - 1 >= m_nCurChildDlg )
	{
		m_vChildDlg[m_nCurChildDlg].pDlg->End();
	}

	CMenuBarChildDlg* pChildDlg = m_vChildDlg[nDlgIndex].pDlg;
	if ( pChildDlg == nullptr ) { ASSERT(0); return; }

	m_nCurChildDlg = nDlgIndex;
	pChildDlg->CreateInit(&m_wndPlaceHolder, 0, 0);

	// Layout Child Dialog...
	CRect DlgRect, ParentRect;
	GetClientRect(&ParentRect);
	pChildDlg->GetWindowRect(&DlgRect);
	ScreenToClient(&DlgRect);

	int Sx = ( ParentRect.Width() - DlgRect.Width() ) / 2;
	int Sy = DlgRect.top;
	int Wx = DlgRect.Width();
	int Wy = DlgRect.Height();
	m_wndPlaceHolder.SetWindowPos(NULL, Sx, Sy, Wx, Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	CRect PHRect;
	m_wndPlaceHolder.GetWindowRect(&PHRect);
	ScreenToClient(&PHRect);

//	CRect CBRect, EBRect;
//	m_wndClose.GetWindowRect(CBRect);
//	m_wndExecute.GetWindowRect(EBRect);
//
//	int BY  = Sy + Wy + 5;
//	int CW  = CBRect.Width();
//	int EW  = EBRect.Width();
//	int SEX = PHRect.right - CW - EW - 10;
//	int SCX = SEX + CW + 5;
//#ifdef _RUS
//	SEX = PHRect.right - CW - EW - 2;
//	SCX = SEX + CW + 15;
//#endif
//	m_wndExecute.SetWindowPos(NULL, SEX, BY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
//	m_wndClose  .SetWindowPos(NULL, SCX, BY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	// Scroll Size ����.
	CSize TSize;
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();

	CSize ScrSize = GetTotalSize();
	ScrSize += TSize;
	SetScrollSizes(MM_TEXT, ScrSize);

	m_RectPlaceHolder = PHRect;
}

void CSeisCommonDlg::DeleteChildDlgs()
{
	for ( const T_SEIS_COMM_DLG_INFO& ChildDlg : m_vChildDlg )
	{
		if ( ChildDlg.pDlg != nullptr )
		{
			delete ChildDlg.pDlg;
		}
	}
}

void CSeisCommonDlg::InitContent()
{
	CDBDoc* pDoc = ( CDBDoc* ) CTreeMenuBarBase::GetDocST();
}

void CSeisCommonDlg::OnTmTableBtn()
{
	int nCurSel = m_ComboMenu.GetCurSel();
	if ( nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount() ) return;

	UINT nTableID = m_vChildDlg[nCurSel].nTableID;
	if ( nTableID > 0 )
	{
		AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(nTableID, 0));
	}
}

void CSeisCommonDlg::OnTabSelect(WPARAM wParam, LPARAM lParam)
{
	if ( CDBDoc::GetDocPoint() )
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
	}

	if ( true )
	{
		int nCurID = 0;
		if ( m_nCurChildDlg != -1 )
		{
			nCurID = m_nCurChildDlg;
		}

		const T_SEIS_COMM_DLG_INFO& ChildDlg = m_vChildDlg[nCurID];

		ShowCurChildDlg(nCurID);
		CTreeMenuBarBase::SetModeOnlyST(ChildDlg.nDlgID);
		if ( ChildDlg.nTableID > 0 )
		{
			m_wndTableBtn.EnableWindow(TRUE);
		}
		else
		{
			m_wndTableBtn.EnableWindow(FALSE);
		}
	}

	int nCurSel = m_ComboMenu.GetCurSel();
	if ( nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount() ) return;
	if ( 0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount() ) return;
}

void CSeisCommonDlg::OnTabUnSelect(WPARAM wParam, LPARAM lParam)
{
	int nCurSel = m_ComboMenu.GetCurSel();
	if ( nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount() ) return;
	if ( 0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount() ) return;
}

CWnd* CSeisCommonDlg::GetDlgByID(int Mode)
{
	for ( const T_SEIS_COMM_DLG_INFO& ChildDlg : m_vChildDlg )
	{
		if ( Mode == ChildDlg.nDlgID )
		{
			return ChildDlg.pDlg;
		}
	}
	return 0;
}

void CSeisCommonDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if ( ( hWnd == nullptr ) || !IsWindow(hWnd) ) return;

	switch ( lHint )
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_AFTER:
		{
			UpdateBuffer();
		}
		break;
	default:
		break;
	}
}

void CSeisCommonDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if ( nCount == 0 ) return;
	ASSERT(nCount == 1);

	int nMethod = -1;

	POSITION pos = pViewBuff->GetStartBuffer();
	while ( pos != nullptr )
	{
		T_UDRD_BUFFER buffer_ur = pViewBuff->GetNextBuffer(pos);
		switch ( buffer_ur.nCmd )
		{
		case UR_SEME_ADD:
			{
				T_SEME_D SemeD;
				pViewBuff->GetSeme(buffer_ur.nKey, SemeD);
				nMethod = SemeD.nMethod;
			}
			break;
		default:
			break;
		}
	}

	if ( nMethod > -1 )
	{
		GetComboMenuItem(m_acsComboMenu, nMethod);
		CreateChildDlgs();
		InitComboMenu(0);
		ShowCurChildDlg(0);
	}
}