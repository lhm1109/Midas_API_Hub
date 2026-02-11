// MPhiParameterDlg_JP.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "MPhiParameterDlg_JP.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\EditData.h"
#include "..\wg_db\UndoCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\MatlDB.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DB_QUERY_MATL.h"

#include "MPhiGlobalDlg.h"
#include "MPhiHingeDlg.h"
#include "MPhiMaterialDlg_JP.h"
#include "MPhiSectionDlg_JP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CMPhiParameterDlg_JP, CDialogMove)
	//{{AFX_MSG_MAP(CMPhiParameterDlg_JP)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB_CTRL_JP, OnSelchangeCmdTabCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CMPhiParameterDlg_JP::CMPhiParameterDlg_JP(CWnd* pParent /*=NULL*/)
	: CDialogMove(CMPhiParameterDlg_JP::IDD, pParent)
{
	m_nKind = 0;
	m_nModifyKey = 0;
	m_pDoc = NULL;
	m_pDlgGlobal = NULL;
	m_pDlgHinge = NULL;
	m_pDlgMaterial = NULL;
}

CMPhiParameterDlg_JP::~CMPhiParameterDlg_JP()
{
	//여기서 new 한 sub dialog 는 CDlgTabCtrl::~CDlgTabCtrl() 함수에서 해제한다.
	//if (m_pDlgGlobal) { delete m_pDlgGlobal; }
}

void CMPhiParameterDlg_JP::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer(lHint, pHint);
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer(lHint, pHint);
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START:
	case D_UPDATE_EXE_END:
		break;
	default:
		break;
	}
}

void CMPhiParameterDlg_JP::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPhiParameterDlg_JP)
	DDX_Control(pDX, IDC_CMD_TAB_CTRL_JP, m_cTabCtrl);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CMPhiParameterDlg_JP message handlers
BOOL CMPhiParameterDlg_JP::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	SetMembDataByDB();

	// Section Tab에서 데이터 자동세팅에 Rebar정보를 가지고 오는 부분이 있는데.. 거기 있는 함수를 부르기전에 m_pMatlDB->Create_RebarData를 한번 해주어야 함..
	T_UNIT_INDEX currUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(currUnit);
	m_pDoc->m_pMatlDB->Create_RebarData(currUnit.nBase_Length, currUnit.nBase_Force, TRUE);

	// Add Child Dialogs.........
	// Global..
	m_pDlgGlobal = new CMPhiGlobalDlg(&m_DataAll, this);
	m_cTabCtrl.AddTab(m_pDlgGlobal, _LS(IDS_CMD_MPHI_GROBAL), CMPhiGlobalDlg::IDD, TRUE);
	// Inelastic Hinge....
	m_pDlgHinge = new CMPhiHingeDlg(&m_DataAll, this);
	m_cTabCtrl.AddTab(m_pDlgHinge, _LS(IDS_CMD_MPHI_INELASTIC_HINGE), CMPhiHingeDlg::IDD, TRUE);
	// Element ...
	m_pDlgMaterial = new CMPhiMaterialDlg_JP(&m_DataAll, this);
 	m_cTabCtrl.AddTab(m_pDlgMaterial, _LS(IDS_CMD_MPHI_INELASTIC_ELEMENT), CMPhiMaterialDlg_JP::IDD, TRUE);
	// Element ...
	m_pDlgSection = new CMPhiSectionDlg_JP(&m_DataAll, this);
	m_cTabCtrl.AddTab(m_pDlgSection, _LS(IDS_CMD_MPHI_SECTION), CMPhiSectionDlg_JP::IDD, TRUE);

	if (m_nKind == 1) m_pDlgHinge->SetModifyData(m_nModifyKey);
	m_cTabCtrl.ShowTab(m_nKind);

	return FALSE;	// return TRUE unless you set the focus to a control
					// EXCEPTION: OCX Property Pages should return FALSE
}

void CMPhiParameterDlg_JP::OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	Invalidate();
	UpdateWindow();
	*pResult = 0;
}

// CTreeWorkCtrl::EditProperty_Property() 에서 호출
void CMPhiParameterDlg_JP::SetParamData(int nKind, UINT Key)
{
	m_nKind = nKind;
	m_nModifyKey = Key;
}

void CMPhiParameterDlg_JP::SetMembDataByDB()
{
	m_DataAll.Init();
	m_pDoc->m_pAttrCtrl->GetMpgb(m_DataAll.MpgbD);

	CArray<T_MPHG_K, T_MPHG_K> aMphgK;
	m_pDoc->m_pAttrCtrl->GetMphgKeyList(aMphgK);
	for (int i = 0; i < aMphgK.GetSize(); i++)
	{
		T_MPHG_D MphgD;
		m_pDoc->m_pAttrCtrl->GetMphg(aMphgK[i], MphgD);
		m_DataAll.aMphgK.Add(aMphgK[i]);
		m_DataAll.aMphgD.Add(MphgD);
	}

	CArray<T_MATL_K, T_MATL_K> aMatdK;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatdK);

	for (int i = 0; i < aMatdK.GetSize(); i++)
	{
		T_MATD_D MatdD;
		if(m_pDoc->m_pAttrCtrl->GetMatlDesign(aMatdK[i], MatdD) == TRUE)
		{
			m_DataAll.aMatdK.Add(aMatdK[i]);
			m_DataAll.aMatdD.Add(MatdD);
		}
		
		T_MATD_REIN_D MatdReinD;
		if (m_pDoc->m_pAttrCtrl2->GetQMatdRein()->Get(aMatdK[i], MatdReinD) == TRUE)
		{
			m_DataAll.aMatdReinK.Add(aMatdK[i]);
			m_DataAll.aMatdReinD.Add(MatdReinD);
		}
	}

	CArray<T_MPST_K, T_MPST_K> aMpstK;
	m_pDoc->m_pAttrCtrl->GetMpstKeyList(aMpstK);
	for (int i = 0; i < aMpstK.GetSize(); i++)
	{
		T_MPST_D MpstD;
		m_pDoc->m_pAttrCtrl->GetMpst(aMpstK[i], MpstD);
		m_DataAll.aMpstK.Add(aMpstK[i]);
		m_DataAll.aMpstD.Add(MpstD);
	}
}

void CMPhiParameterDlg_JP::UpdateBuffer(LPARAM lHint, CObject* pHint)
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_MATD_ADD):
		case(UR_MATD_DEL):
		case(UR_MATD_MFD):
		case(UR_MATD_MFS):
			//m_pDlgHinge->MyUpdate(lHint, pHint);
			//m_pDlgElement->MyUpdate(lHint, pHint);
			break;
		case(UR_RPSC_ADD):
		case(UR_RPSC_DEL):
		case(UR_RCHK_ADD):
		case(UR_RCHK_DEL):
			//m_pDlgSection_JP->MyUpdate(lHint, pHint);
			break;
		default:
			break;
		}
	} // end of while
}

void CMPhiParameterDlg_JP::OnOK()
{
	if (m_pDlgGlobal && m_pDlgGlobal->GetSafeHwnd() && IsWindow(m_pDlgGlobal->GetSafeHwnd()))
		m_pDlgGlobal->Dlg2Data();
	if (m_pDlgSection && m_pDlgSection->GetSafeHwnd() && IsWindow(m_pDlgSection->GetSafeHwnd()))
		m_pDlgSection->Dlg2Data();

	m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_MPhiParameters)), CMDTYPE_FREE);

	if (!m_pDoc->m_pEditData->AddMpgb(m_DataAll.MpgbD))
		return m_pDoc->m_pUndoCtrl->CancelEditDB();

	for (int i = 0; i < m_DataAll.aMphgK.GetSize(); i++)
	{
		if (!m_pDoc->m_pEditData->AddMphg(m_DataAll.aMphgK[i], m_DataAll.aMphgD[i]))
			return m_pDoc->m_pUndoCtrl->CancelEditDB();
	}

	for (int i = 0; i < m_DataAll.aMatdK.GetSize(); i++)
	{
		if (!m_pDoc->m_pEditData->AddMatd(m_DataAll.aMatdK[i], m_DataAll.aMatdD[i]))
			return m_pDoc->m_pUndoCtrl->CancelEditDB();
	}

	for (int i = 0; i < m_DataAll.aMatdReinK.GetSize(); i++)
	{
		if (m_pDoc->m_pAttrCtrl2->GetQMatdRein()->Exist(m_DataAll.aMatdReinK[i]) == TRUE) {
			if (m_DataAll.aMatdReinD[i].kMatl == 0) {// 삭제 된 것으로 간주
				if (!m_pDoc->m_pEditData->DelMatdRein(m_DataAll.aMatdReinK[i]))
					return m_pDoc->m_pUndoCtrl->CancelEditDB();
			}
			else {
				if (!m_pDoc->m_pEditData->ModifyMatdRein(m_DataAll.aMatdReinK[i], m_DataAll.aMatdReinD[i]))
					return m_pDoc->m_pUndoCtrl->CancelEditDB();
			}
		}
		else {
			if (!m_pDoc->m_pEditData->AddMatdRein(m_DataAll.aMatdReinK[i], m_DataAll.aMatdReinD[i]))
				return m_pDoc->m_pUndoCtrl->CancelEditDB();
		}
	}

	for (int i = 0; i < m_DataAll.aMpstK.GetSize(); i++)
	{
		if (!m_pDoc->m_pEditData->IsValidMpst(m_DataAll.aMpstK[i], m_DataAll.aMpstD[i]))
			return m_pDoc->m_pUndoCtrl->CancelEditDB();

		if (!m_pDoc->m_pEditData->AddMpst(m_DataAll.aMpstK[i], m_DataAll.aMpstD[i]))
			return m_pDoc->m_pUndoCtrl->CancelEditDB();
	}

	m_pDoc->m_pUndoCtrl->CloseEditDB(TRUE);

	CDialogMove::OnOK();
}