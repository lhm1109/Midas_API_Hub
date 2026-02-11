#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCommonSespDlg.h"
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

CSeisCommonSespDlg::CSeisCommonSespDlg(CWnd* pParent) : CMenuBarChildDlg(CSeisCommonSespDlg::IDD, pParent)
{
}

CSeisCommonSespDlg::~CSeisCommonSespDlg()
{
}

void CSeisCommonSespDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_RDO_EXEC_ADD, m_rdoExecAdd);
	DDX_Control(pDX, IDC_TM_RDO_EXEC_DEL, m_rdoExecDel);

	DDX_Control(pDX, IDC_TM_RDO_MEMB_BEAM, m_rdoMembBeam);
	DDX_Control(pDX, IDC_TM_RDO_MEMB_COLM, m_rdoMembColm);

	DDX_Control(pDX, IDC_TM_LBL_SPAN_LSYI, m_lblSpanLsyi);
	DDX_Control(pDX, IDC_TM_LBL_SPAN_LSYJ, m_lblSpanLsyj);
	DDX_Control(pDX, IDC_TM_LBL_SPAN_LSZI, m_lblSpanLszi);
	DDX_Control(pDX, IDC_TM_LBL_SPAN_LSZJ, m_lblSpanLszj);

	DDX_Control(pDX, IDC_TM_EDT_SPAN_LSYI, m_edtSpanLsyi);
	DDX_Control(pDX, IDC_TM_EDT_SPAN_LSYJ, m_edtSpanLsyj);
	DDX_Control(pDX, IDC_TM_EDT_SPAN_LSZI, m_edtSpanLszi);
	DDX_Control(pDX, IDC_TM_EDT_SPAN_LSZJ, m_edtSpanLszj);

	DDX_Control(pDX, IDC_TM_UNT_SPAN_LSYI, m_untSpanLsyi);
	DDX_Control(pDX, IDC_TM_UNT_SPAN_LSYJ, m_untSpanLsyj);
	DDX_Control(pDX, IDC_TM_UNT_SPAN_LSZI, m_untSpanLszi);
	DDX_Control(pDX, IDC_TM_UNT_SPAN_LSZJ, m_untSpanLszj);

	DDX_Control(pDX, IDC_TM_CHK_SPAN_RELA, m_chkRelative);
}

BEGIN_MESSAGE_MAP(CSeisCommonSespDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_TM_RDO_MEMB_BEAM, OnChangeMembType)
	ON_BN_CLICKED(IDC_TM_RDO_MEMB_COLM, OnChangeMembType)
	ON_BN_CLICKED(IDC_TM_CHK_SPAN_RELA, OnChangeLengType)
END_MESSAGE_MAP()

BOOL CSeisCommonSespDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();
	T_SESP_D InitD;
	InitUnitByLengType(InitD.bReal);
	Data2Dlg(InitD);
	return FALSE;
}

void CSeisCommonSespDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CSeisCommonSespDlg::UpdateBuffer()
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
		case UR_SESP_ADD:
		case UR_SESP_DEL:
			break;
		default:
			break;
		}
	}
}

void CSeisCommonSespDlg::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}

void CSeisCommonSespDlg::Execute()
{
	T_ELEM_K_LIST aElemK;
	if ( !GetElemKeyList(aElemK) ) return;

	T_SESP_D SespD;
	if ( !Dlg2Data(SespD) ) { ASSERT(0); return; }

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if ( pDoc == nullptr ) { ASSERT(0); return; }

	switch ( GetRunMode() )
	{
	case MenuBarRunMode::enAdd:
		{
			pDoc->m_pDataCtrl->AddSesp(aElemK, SespD);
			IUsageCounter::Use(_T("SPD_SESP"));
		}
		break;
	case MenuBarRunMode::enDel:
		{
			pDoc->m_pDataCtrl->DelSesp(aElemK);
		}
		break;
	default: ASSERT(0); break;
	}

	pDoc->m_pViewCtrl->UnselectAll(NULL);
}

bool CSeisCommonSespDlg::GetElemKeyList(T_ELEM_K_LIST& raElemK)
{
	raElemK.RemoveAll();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if ( pDoc == nullptr ) { ASSERT(0); return false; }

	T_ELEM_K_LIST aSelElemK;
	pDoc->m_pViewCtrl->GetAllSelectedElem(aSelElemK);
	INT_PTR nSelElemK = aSelElemK.GetCount();
	if ( nSelElemK < 1 ) return false;

	UINT nMembType = GetMembType();

	for ( INT_PTR i = 0; i < nSelElemK; ++i )
	{
		T_ELEM_K SelElemK = aSelElemK[i];
		T_ELEM_D SelElemD;
		if ( !pDoc->m_pAttrCtrl->GetElem(SelElemK, SelElemD) ) { ASSERT(0); continue; }

		T_MATL_D MatlD;
		if ( !pDoc->m_pAttrCtrl->GetMatl(SelElemD.elmat, MatlD) ) { ASSERT(0); continue; }

		if ( MatlD.Type != _T("C") ) continue;

		if ( pDoc->m_pAttrCtrl->GetMemberType(SelElemK) != nMembType) continue;

		raElemK.Add(SelElemK);
	}

	return !raElemK.IsEmpty();
}

bool CSeisCommonSespDlg::Data2Dlg(const T_SESP_D& SespD)
{
	SetRunMode(MenuBarRunMode::enAdd);

	SetMembType(SespD.nMembType);

	m_edtSpanLsyi.SetEditUnitAuto(SespD.dLsyi);
	m_edtSpanLsyj.SetEditUnitAuto(SespD.dLsyj);
	m_edtSpanLszi.SetEditUnitAuto(SespD.dLszi);
	m_edtSpanLszj.SetEditUnitAuto(SespD.dLszj);
	m_chkRelative.SetCheck(!SespD.bReal);

	InitUnitByLengType(SespD.bReal);
	EnableByMembType(SespD.nMembType);
	return true;
}

bool CSeisCommonSespDlg::Dlg2Data(T_SESP_D& rSespD)
{
	rSespD.Initialize();
	rSespD.nMembType = GetMembType();

	rSespD.dLszi = m_edtSpanLszi.GetEditValue();
	rSespD.dLszj = m_edtSpanLszj.GetEditValue();

	if (rSespD.nMembType == D_MBTP_COLUMN)
	{
		rSespD.dLsyi = m_edtSpanLsyi.GetEditValue();
		rSespD.dLsyj = m_edtSpanLsyj.GetEditValue();
	}

	rSespD.bReal = ( m_chkRelative.GetCheck() == BST_UNCHECKED ) ? TRUE : FALSE;
	return true;
}

void CSeisCommonSespDlg::SetRunMode(MenuBarRunMode nMode)
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

MenuBarRunMode CSeisCommonSespDlg::GetRunMode()
{
	if ( m_rdoExecAdd.GetCheck() == BST_CHECKED ) return MenuBarRunMode::enAdd;
	if ( m_rdoExecDel.GetCheck() == BST_CHECKED ) return MenuBarRunMode::enDel;
	ASSERT(0); return MenuBarRunMode::enAdd;
}

void CSeisCommonSespDlg::SetMembType(UINT nMembType)
{
	m_rdoMembBeam.SetCheck(BST_UNCHECKED);
	m_rdoMembColm.SetCheck(BST_UNCHECKED);

	switch ( nMembType )
	{
	case D_MBTP_BEAM: m_rdoMembBeam.SetCheck(BST_CHECKED); break;
	case D_MBTP_COLM: m_rdoMembColm.SetCheck(BST_CHECKED); break;
	default: ASSERT(0); m_rdoMembBeam.SetCheck(BST_CHECKED); break;
	}
}

UINT CSeisCommonSespDlg::GetMembType()
{
	if ( m_rdoMembBeam.GetCheck() == BST_CHECKED ) return D_MBTP_BEAM;
	if ( m_rdoMembColm.GetCheck() == BST_CHECKED ) return D_MBTP_COLM;
	ASSERT(0); return D_MBTP_BEAM;
}

void CSeisCommonSespDlg::OnChangeMembType()
{
	UINT nMembType = GetMembType();
	EnableByMembType(nMembType);
}

void CSeisCommonSespDlg::OnChangeLengType()
{
	BOOL bReal = ( m_chkRelative.GetCheck() == BST_UNCHECKED ) ? TRUE : FALSE;
	InitUnitByLengType(bReal);
	ShowHideByLengType(bReal);
}

void CSeisCommonSespDlg::InitUnitByLengType(BOOL bReal)
{
	UINT nUnitType = bReal ? D_UNITSYS_BASE_LENGTH : D_UNITSYS_NONE;
	m_edtSpanLsyi.SetUnitType(nUnitType);
	m_edtSpanLsyj.SetUnitType(nUnitType);
	m_edtSpanLszi.SetUnitType(nUnitType);
	m_edtSpanLszj.SetUnitType(nUnitType);

	m_untSpanLsyi.SetUnitType(nUnitType);
	m_untSpanLsyj.SetUnitType(nUnitType);
	m_untSpanLszi.SetUnitType(nUnitType);
	m_untSpanLszj.SetUnitType(nUnitType);
}

void CSeisCommonSespDlg::ShowHideByLengType(BOOL bReal)
{
	int nShow = bReal ? SW_SHOW : SW_HIDE;
	m_untSpanLsyi.ShowWindow(nShow);
	m_untSpanLsyj.ShowWindow(nShow);
	m_untSpanLszi.ShowWindow(nShow);
	m_untSpanLszj.ShowWindow(nShow);
}

void CSeisCommonSespDlg::EnableByMembType(UINT nMembType)
{
	bool bEnable = false;
	switch ( nMembType )
	{
	case D_MBTP_BEAM: bEnable = false; break;
	case D_MBTP_COLM: bEnable = true; break;
	default: ASSERT(0); break;
	}

	m_lblSpanLsyi.EnableWindow(bEnable);
	m_lblSpanLsyj.EnableWindow(bEnable);

	m_edtSpanLsyi.EnableWindow(bEnable);
	m_edtSpanLsyj.EnableWindow(bEnable);

	m_untSpanLsyi.EnableWindow(bEnable);
	m_untSpanLsyj.EnableWindow(bEnable);
}