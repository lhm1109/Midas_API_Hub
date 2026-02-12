#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCommonSecoDlg.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSeisCommonSecoDlg::CSeisCommonSecoDlg(CWnd* pParent) : CMenuBarChildDlg(CSeisCommonSecoDlg::IDD, pParent)
{
}

CSeisCommonSecoDlg::~CSeisCommonSecoDlg()
{
}

void CSeisCommonSecoDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_RDO_EXEC_ADD, m_rdoExecAdd);
	DDX_Control(pDX, IDC_TM_RDO_EXEC_DEL, m_rdoExecDel);

	DDX_Control(pDX, IDC_TM_RDO_DETL_SEIS, m_rdoDetlSeis);
	DDX_Control(pDX, IDC_TM_RDO_DETL_NONE, m_rdoDetlNone);
}

BEGIN_MESSAGE_MAP(CSeisCommonSecoDlg, CMenuBarChildDlg)
END_MESSAGE_MAP()

BOOL CSeisCommonSecoDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();
	T_SECO_D InitD;
	Data2Dlg(InitD);
	return FALSE;
}

void CSeisCommonSecoDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if ( hWnd == nullptr ) return;
	if ( !IsWindow(hWnd) ) return;

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

void CSeisCommonSecoDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	if ( pViewBuff == nullptr ) { ASSERT(0); return; }

	int nCount = pViewBuff->GetCount();
	if ( nCount == 0 ) return;
	ASSERT(nCount == 1);

	POSITION pos = pViewBuff->GetStartBuffer();
	while ( pos != nullptr )
	{
		T_UDRD_BUFFER buffer_ur = pViewBuff->GetNextBuffer(pos);
		switch ( buffer_ur.nCmd )
		{
		case UR_SECO_ADD:
		case UR_SECO_DEL:
			break;
		default:
			break;
		}
	}
}

void CSeisCommonSecoDlg::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}

void CSeisCommonSecoDlg::Execute()
{
	T_ELEM_K_LIST aElemK;
	if ( !GetElemKeyList(aElemK) ) return;

	T_SECO_D SecoD;
	if ( !Dlg2Data(SecoD) ) { ASSERT(0); return; }

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if ( pDoc == nullptr ) { ASSERT(0); return; }

	switch ( GetRunMode() )
	{
	case MenuBarRunMode::enAdd:
		{
			pDoc->m_pDataCtrl->AddSeco(aElemK, SecoD);
			IUsageCounter::Use(_T("SPD_SECO"));
		}
		break;
	case MenuBarRunMode::enDel:
		{
			pDoc->m_pDataCtrl->DelSeco(aElemK);
		}
		break;
	default: ASSERT(0); break;
	}

	pDoc->m_pViewCtrl->UnselectAll(NULL);
}

bool CSeisCommonSecoDlg::GetElemKeyList(T_ELEM_K_LIST& raElemK)
{
	raElemK.RemoveAll();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if ( pDoc == nullptr ) { ASSERT(0); return false; }

	T_ELEM_K_LIST aSelElemK;
	pDoc->m_pViewCtrl->GetAllSelectedElem(aSelElemK);
	INT_PTR nSelElemK = aSelElemK.GetCount();
	if ( nSelElemK < 1 ) return false;

	for ( INT_PTR i = 0; i < nSelElemK; ++i )
	{
		T_ELEM_K SelElemK = aSelElemK[i];
		T_ELEM_D SelElemD;
		if ( !pDoc->m_pAttrCtrl->GetElem(SelElemK, SelElemD) ) { ASSERT(0); continue; }

		T_MATL_D MatlD;
		if ( !pDoc->m_pAttrCtrl->GetMatl(SelElemD.elmat, MatlD) ) { ASSERT(0); continue; }

		if ( MatlD.Type != _T("C") ) continue;
		if ( pDoc->m_pAttrCtrl->GetMemberType(SelElemK) != D_MBTP_COLUMN ) continue;
		raElemK.Add(SelElemK);
	}

	return !raElemK.IsEmpty();
}

bool CSeisCommonSecoDlg::Data2Dlg(const T_SECO_D& SecoD)
{
	SetRunMode(MenuBarRunMode::enAdd);
	SetRebarDetail(SecoD.nDetail);
	return true;
}

bool CSeisCommonSecoDlg::Dlg2Data(T_SECO_D& rSecoD)
{
	rSecoD.Initialize();
	rSecoD.nDetail = GetRebarDetail();
	return true;
}

void CSeisCommonSecoDlg::SetRunMode(MenuBarRunMode nMode)
{
	m_rdoExecAdd.SetCheck(BST_UNCHECKED);
	m_rdoExecDel.SetCheck(BST_UNCHECKED);

	switch ( nMode )
	{
	case MenuBarRunMode::enAdd: m_rdoExecAdd.SetCheck(BST_CHECKED); break;
	case MenuBarRunMode::enDel: m_rdoExecDel.SetCheck(BST_CHECKED); break;
	default: ASSERT(0); m_rdoExecAdd.SetCheck(BST_CHECKED); break;
	}
}

MenuBarRunMode CSeisCommonSecoDlg::GetRunMode()
{
	if ( m_rdoExecAdd.GetCheck() == BST_CHECKED ) return MenuBarRunMode::enAdd;
	if ( m_rdoExecDel.GetCheck() == BST_CHECKED ) return MenuBarRunMode::enDel;
	ASSERT(0); return MenuBarRunMode::enAdd;
}

void CSeisCommonSecoDlg::SetRebarDetail(UINT nDetail)
{
	m_rdoDetlSeis.SetCheck(BST_UNCHECKED);
	m_rdoDetlNone.SetCheck(BST_UNCHECKED);

	switch ( nDetail )
	{
	case EN_SECO_DETL_SEIS: m_rdoDetlSeis.SetCheck(BST_CHECKED); break;
	case EN_SECO_DETL_NONE: m_rdoDetlNone.SetCheck(BST_CHECKED); break;
	default: ASSERT(0); m_rdoDetlNone.SetCheck(BST_CHECKED); break;
	}
}

UINT CSeisCommonSecoDlg::GetRebarDetail()
{
	if ( m_rdoDetlSeis.GetCheck() == BST_CHECKED ) return EN_SECO_DETL_SEIS;
	if ( m_rdoDetlNone.GetCheck() == BST_CHECKED ) return EN_SECO_DETL_NONE;
	ASSERT(0); return EN_SECO_DETL_NONE;
}