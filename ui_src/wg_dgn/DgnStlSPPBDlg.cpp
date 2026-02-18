//sppb
//sppb
// DgnStlSPPBDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlSPPBDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

// CDgnStlSPPBDlg 대화 상자입니다.

CDgnStlSPPBDlg::CDgnStlSPPBDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlSPPBDlg::IDD, pParent)
{
	m_nAddDel = 0;
	m_bCheck = TRUE;
	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnStlSPPBDlg::~CDgnStlSPPBDlg()
{
}

void CDgnStlSPPBDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnStlSPPBDlg::UpdateBuffer()
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
		case(UR_SPPB_ADD):
		case(UR_SPPB_DEL):
			break;
		default:
			break;
		}
	} // end of while
}

void CDgnStlSPPBDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_AR_RDO, m_nAddDel);
	DDX_Check(pDX, IDC_DGN_SPPB_CHK, m_bCheck);
}

BEGIN_MESSAGE_MAP(CDgnStlSPPBDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
END_MESSAGE_MAP()

void CDgnStlSPPBDlg::OnDgnClose()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStlSPPBDlg::OnDgnExecute()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelectElemK;
	CArray<T_ELEM_K, T_ELEM_K> aSPPBK;

	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aSelectElemK);

	for ( int i = 0; i < aSelectElemK.GetCount(); i++ )
	{
		T_ELEM_K ElemK = aSelectElemK[i];

		if ( !m_pDoc->m_pAttrCtrl->IsSppb(ElemK) ) continue;

		aSPPBK.Add(ElemK);
	}

	T_SPPB_D SPPBD;
	SPPBD.Initialize();
	SPPBD.bCheck = m_bCheck;

	if ( m_nAddDel == 0 )	m_pDoc->m_pDataCtrl->AddSppb(aSPPBK, SPPBD);
	else					m_pDoc->m_pDataCtrl->DelSppb(aSPPBK);

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	UpdateData(FALSE);
}

BOOL CDgnStlSPPBDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	if ( m_pDoc == NULL )	return TRUE;

	return TRUE;
}