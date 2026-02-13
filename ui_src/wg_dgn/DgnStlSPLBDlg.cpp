//splb
// DgnStlSPLBDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlSPLBDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

// CDgnStlSPLBDlg 대화 상자입니다.

CDgnStlSPLBDlg::CDgnStlSPLBDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlSPLBDlg::IDD, pParent)
{
	m_nAddDel = 0;
	m_nCompression_Girder = 0;
	m_nFixing_Lateral_Bucking = 0;
	m_nLoadType_Lateral_Bucking = 0;
	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnStlSPLBDlg::~CDgnStlSPLBDlg()
{
}

void CDgnStlSPLBDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnStlSPLBDlg::UpdateBuffer()
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
		case(UR_SPLB_ADD):
		case(UR_SPLB_DEL):
			break;
		default:
			break;
		}
	} // end of while
}

void CDgnStlSPLBDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_AR_RDO, m_nAddDel);
	DDX_Radio(pDX, IDC_DGN_COMP_GIRDER_RDO1, m_nCompression_Girder);
	DDX_Radio(pDX, IDC_DGN_LATERAL_BUCKLING_RDO1, m_nFixing_Lateral_Bucking);
	DDX_Radio(pDX, IDC_DGN_TYPE_LATERAL_BUCKLING_RDO1, m_nLoadType_Lateral_Bucking);
}

BEGIN_MESSAGE_MAP(CDgnStlSPLBDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
END_MESSAGE_MAP()

void CDgnStlSPLBDlg::OnDgnClose()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStlSPLBDlg::OnDgnExecute()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelectElemK;
	CArray<T_ELEM_K, T_ELEM_K> aSPLBK;

	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aSelectElemK);

	for ( int i = 0; i < aSelectElemK.GetCount(); i++ )
	{
		T_ELEM_K ElemK = aSelectElemK[i];

		if ( !m_pDoc->m_pAttrCtrl->IsSplb(ElemK) ) continue;

		aSPLBK.Add(ElemK);
	}

	T_SPLB_D SPLBD;
	SPLBD.Initialize();
	SPLBD.Compression_Girder = m_nCompression_Girder;
	SPLBD.Fixing_Lateral_Bucking = m_nFixing_Lateral_Bucking;
	SPLBD.LoadType_Lateral_Bucking = m_nLoadType_Lateral_Bucking;

	if ( m_nAddDel == 0 )	m_pDoc->m_pDataCtrl->AddSplb(aSPLBK, SPLBD);
	else					m_pDoc->m_pDataCtrl->DelSplb(aSPLBK);

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	UpdateData(FALSE);
}

BOOL CDgnStlSPLBDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	if ( m_pDoc == NULL )	return TRUE;

	return TRUE;
}