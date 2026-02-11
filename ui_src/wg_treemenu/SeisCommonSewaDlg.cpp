#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCommonSewaDlg.h"
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

CSeisCommonSewaDlg::CSeisCommonSewaDlg(CWnd* pParent) : CMenuBarChildDlg(CSeisCommonSewaDlg::IDD, pParent)
{
}

CSeisCommonSewaDlg::~CSeisCommonSewaDlg()
{
}

void CSeisCommonSewaDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_RDO_EXEC_ADD, m_rdoExecAdd);
	DDX_Control(pDX, IDC_TM_RDO_EXEC_DEL, m_rdoExecDel);

	DDX_Control(pDX, IDC_TM_RDO_DETL_SEIS, m_rdoDetlSeis);
	DDX_Control(pDX, IDC_TM_RDO_DETL_MITI, m_rdoDetlMiti);
	DDX_Control(pDX, IDC_TM_RDO_DETL_NONE, m_rdoDetlNone);
}

BEGIN_MESSAGE_MAP(CSeisCommonSewaDlg, CMenuBarChildDlg)
END_MESSAGE_MAP()

BOOL CSeisCommonSewaDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();
	T_SEWA_D InitD;
	Data2Dlg(InitD);
	return FALSE;
}

void CSeisCommonSewaDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CSeisCommonSewaDlg::UpdateBuffer()
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
		case UR_SEWA_ADD:
		case UR_SEWA_DEL:
			break;
		default:
			break;
		}
	}
}

void CSeisCommonSewaDlg::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}

void CSeisCommonSewaDlg::Execute()
{
	T_ELEM_K_LIST aElemK;
	if ( !GetElemKeyList(aElemK) ) return;

	T_SEWA_D SewaD;
	if ( !Dlg2Data(SewaD) ) { ASSERT(0); return; }

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if ( pDoc == nullptr ) { ASSERT(0); return; }

	switch ( GetRunMode() )
	{
	case MenuBarRunMode::enAdd:
		{
			pDoc->m_pDataCtrl->AddSewa(aElemK, SewaD);
			IUsageCounter::Use(_T("SPD_SEWA"));
		}
		break;
	case MenuBarRunMode::enDel:
		{
			pDoc->m_pDataCtrl->DelSewa(aElemK);
		}
		break;
	default: ASSERT(0); break;
	}

	pDoc->m_pViewCtrl->UnselectAll(NULL);
}

bool CSeisCommonSewaDlg::GetElemKeyList(T_ELEM_K_LIST& raElemK)
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
		if ( pDoc->m_pAttrCtrl->GetMemberType(SelElemK) != D_MBTP_WALL ) continue;
		raElemK.Add(SelElemK);
	}

	return !raElemK.IsEmpty();
}

bool CSeisCommonSewaDlg::Data2Dlg(const T_SEWA_D& SewaD)
{
	SetRunMode(MenuBarRunMode::enAdd);
	SetDetailWall(SewaD.nDetail);
	return true;
}

bool CSeisCommonSewaDlg::Dlg2Data(T_SEWA_D& rSewaD)
{
	rSewaD.Initialize();
	rSewaD.nDetail = GetDetailWall();
	return true;
}

void CSeisCommonSewaDlg::SetRunMode(MenuBarRunMode nMode)
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

MenuBarRunMode CSeisCommonSewaDlg::GetRunMode()
{
	if ( m_rdoExecAdd.GetCheck() == BST_CHECKED ) return MenuBarRunMode::enAdd;
	if ( m_rdoExecDel.GetCheck() == BST_CHECKED ) return MenuBarRunMode::enDel;
	ASSERT(0); return MenuBarRunMode::enAdd;
}

void CSeisCommonSewaDlg::SetDetailWall(UINT nDetail)
{
	m_rdoDetlSeis.SetCheck(BST_UNCHECKED);
	m_rdoDetlMiti.SetCheck(BST_UNCHECKED);
	m_rdoDetlNone.SetCheck(BST_UNCHECKED);

	switch ( nDetail )
	{
	case EN_SEWA_BNDR_SEIS: m_rdoDetlSeis.SetCheck(BST_CHECKED); break;
	case EN_SEWA_BNDR_MITI: m_rdoDetlMiti.SetCheck(BST_CHECKED); break;
	case EN_SEWA_BNDR_NONE: m_rdoDetlNone.SetCheck(BST_CHECKED); break;
	default: ASSERT(0); m_rdoDetlNone.SetCheck(BST_CHECKED); break;
	}
}

UINT CSeisCommonSewaDlg::GetDetailWall()
{
	if ( m_rdoDetlSeis.GetCheck() == BST_CHECKED ) return EN_SEWA_BNDR_SEIS;
	if ( m_rdoDetlMiti.GetCheck() == BST_CHECKED ) return EN_SEWA_BNDR_MITI;
	if ( m_rdoDetlNone.GetCheck() == BST_CHECKED ) return EN_SEWA_BNDR_NONE;
	ASSERT(0); return EN_SEWA_BNDR_NONE;
}