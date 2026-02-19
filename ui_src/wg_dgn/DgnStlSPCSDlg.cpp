//spcs
// DgnStlSPCSDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlSPCSDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

// CDgnStlSPCSDlg 대화 상자입니다.

CDgnStlSPCSDlg::CDgnStlSPCSDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlSPCSDlg::IDD, pParent)
{
	m_nAddDel = 0;
	m_EditValue.SetEditUnit(0.);
	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnStlSPCSDlg::~CDgnStlSPCSDlg()
{
}

void CDgnStlSPCSDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ( (hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd) ) return;

	switch ( lHint )
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		//UpdateBuffer();
		break;
	case D_UPDATE_NOTIFY:
		if ( pHint != NULL )
		{
			CString strCommand = ((T_UPDATE_HINT*)pHint)->csNotifyCmd;
			if ( strCommand == _LSX(STLCODE_UPDATE) ) OnDgnClose();
		}
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CDgnStlSPCSDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if ( nCount == 0 ) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while ( pos != NULL )
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;

		switch ( nCmd )
		{
		case(UR_SPCS_ADD):
		case(UR_SPCS_DEL):
			break;
		default:
			break;
		}
	} // end of while
}

void CDgnStlSPCSDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_AR_RDO, m_nAddDel);
	DDX_Control(pDX, IDC_DGN_SPCS_EDT1, m_EditValue);
	DDX_Control(pDX, IDC_DGN_SPCS_EDT1_UNIT, m_EditValueUnit);
}

BEGIN_MESSAGE_MAP(CDgnStlSPCSDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
END_MESSAGE_MAP()

void CDgnStlSPCSDlg::OnDgnClose()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStlSPCSDlg::OnDgnExecute()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelectElemK;
	CArray<T_ELEM_K, T_ELEM_K> aSPCSK;

	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aSelectElemK);

	for ( int i = 0; i < aSelectElemK.GetCount(); i++ )
	{
		T_ELEM_K ElemK = aSelectElemK[i];

		if ( !m_pDoc->m_pAttrCtrl->IsSpcs(ElemK) ) continue;

		aSPCSK.Add(ElemK);
	}

	T_SPCS_D SPCSD;
	SPCSD.Initialize();
	SPCSD.CR_Spacing = m_EditValue.GetEditValue();

	if ( m_nAddDel == 0 )	m_pDoc->m_pDataCtrl->AddSpcs(aSPCSK, SPCSD);
	else					m_pDoc->m_pDataCtrl->DelSpcs(aSPCSK);

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	UpdateData(FALSE);
}

BOOL CDgnStlSPCSDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	if ( m_pDoc == NULL )	return TRUE;

	Initial_Unit();

	return TRUE;
}

void CDgnStlSPCSDlg::Initial_Unit()
{
	m_EditValue.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_EditValueUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}