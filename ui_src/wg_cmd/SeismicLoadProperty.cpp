// SeismicLoadProperty.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeismicLoadProperty.h"


#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "ETC_EP_KDS2018.h"
#include "ETC_EP_KDS2019.h"

#include "ETC_DESIGN_SPECT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeismicLoadProperty dialog


CSeismicLoadProperty::CSeismicLoadProperty(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeismicLoadProperty::IDD, pParent)
{	
#if defined(_CIVIL)
	m_bCivil = TRUE;
#else
	m_bCivil = FALSE;
#endif

	m_Key = m_OldKey = 0;
	m_bModify = FALSE;

	m_Data.Initialize();

	m_mapChildDlg.RemoveAll();
	m_mapChildDlg.InitHashTable(10);

	auto L_AddChildDlg = [this](UINT nSeq, const CString& csCodeName, CMyChildDialog* pDialog)
	{
		if (pDialog == nullptr) { ASSERT(0); return; }
		if (csCodeName.IsEmpty()) { ASSERT(0); return; }

		CMyChildDialog* pDlgTest = nullptr;
		ASSERT(!m_mapChildDlg.Lookup(nSeq, pDlgTest));
		m_aCodeSeq.Add(nSeq);
		m_aCodeName.Add(csCodeName);
		m_mapChildDlg.SetAt(nSeq, pDialog);
	};

	L_AddChildDlg(EN_CHILD_ORDER_KSCE2019, _T("KDS(17-10-00:2018)"), new CETC_EP_KDS2018(&m_Data));
	L_AddChildDlg(EN_CHILD_ORDER_KDS2019, _T("KDS(41-17-00:2019)"), new CETC_EP_KDS2019(&m_Data));

	m_aLockSeq.Add(D_SPFC_CODE_KSCE2019);
	m_aLockSeq.Add(D_SPFC_CODE_KDS2019);

	m_nCurrentDlg = (m_bCivil) ? EN_CHILD_ORDER_KSCE2019 : EN_CHILD_ORDER_KDS2019;

}

CSeismicLoadProperty::~CSeismicLoadProperty()
{
	CMyChildDialog* dlg;
	int key;

	if (m_mapChildDlg.Lookup(m_nCurrentDlg, dlg))
	{
		if (dlg->IsCreated())
		{
			dlg->End();
		}
	}

	POSITION pos = m_mapChildDlg.GetStartPosition();
	while (pos != NULL)
	{
		m_mapChildDlg.GetNextAssoc(pos, key, dlg);
		delete dlg;
	}
	m_mapChildDlg.RemoveAll();
}

void CSeismicLoadProperty::SetParamData(T_POSL_K Key, T_POSL_D &ParamData)
{
	m_Key = m_OldKey = Key;
	m_Data = ParamData;
	m_bModify = TRUE;
}

void CSeismicLoadProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeismicLoadProperty)
	DDX_Control(pDX, IDC_CMD_EP_SEISMIC_NAME_EDT, m_editName);
	DDX_Control(pDX, IDC_CMD_EP_SEISMIC_CODE_CMB, m_cobxCodeName);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeismicLoadProperty, CDialogMove)
	//{{AFX_MSG_MAP(CSeismicLoadProperty)

	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeismicLoadProperty message handlers

BOOL CSeismicLoadProperty::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitCobxCodeName();

	if(!m_bModify) m_Data.Initialize();
	Data2Dlg();

	POSITION pos = m_mapChildDlg.GetStartPosition();
	CMyChildDialog* dlg;
	int key;
	while (pos != NULL)
	{
		m_mapChildDlg.GetNextAssoc(pos, key, dlg);
	}

	UpdateCurDlg();      // Data2Dlg
/*	CDlgUtil::CobxAdjustListBoxWidth(m_cmbDesignCode);*/

	return TRUE;
}


void CSeismicLoadProperty::InitCobxCodeName()
{
	m_cobxCodeName.ResetContent();

	if (m_bCivil)
	{
		m_cobxCodeName.SetItemData(m_cobxCodeName.AddString(_T("KDS(17-10-00:2018)")), D_SPFC_CODE_KSCE2019);

	}
	else
	{
		m_cobxCodeName.SetItemData(m_cobxCodeName.AddString(_T("KDS(41-17-00:2019)")), D_SPFC_CODE_KDS2019);
	}
	m_cobxCodeName.SetCurSel(0);
}


void CSeismicLoadProperty::Data2Dlg()
{
	m_editName.SetWindowText(m_Data.strName);
	m_Data.nCodeType = (m_bCivil) ? KDS_E_2018 : KDS_E_2019;

	CMyChildDialog* dlg=NULL;
	if (m_mapChildDlg.Lookup(m_nCurrentDlg, dlg))
	{
		if (dlg->IsCreated())
		{
			dlg->Data2Dlg();
		}
	}
}

BOOL CSeismicLoadProperty::Dlg2Data()
{
	CString str; 
	m_editName.GetWindowText(m_Data.strName);
	m_Data.strName.TrimLeft();
	m_Data.strName.TrimRight();

	CString strMsg, strCondition;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc->m_pAttrCtrl->GetPoslKeyByName(m_Data.strName) && !m_bModify)
	{
		AfxMessageBox(_LS(IDS_CMD_SAME_NAME_EXIST));
		return FALSE;
	}
	if (m_Data.strName.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_CMD_SOIL_PROP_NAME_EMPTY_ERROR));
		return FALSE;
	}

	CMyChildDialog* dlg = NULL;
	if (m_mapChildDlg.Lookup(m_nCurrentDlg, dlg))
	{
		if (dlg->IsCreated())
		{
			dlg->Dlg2Data();
		}
	}
	return TRUE;
}


void CSeismicLoadProperty::OnOK()
{
	if (!Dlg2Data()) return;
	BOOL bSuccess;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (m_bModify)
		bSuccess = pDoc->m_pDataCtrl->ModifyPosl(m_OldKey, m_Key, m_Data);
	else
		bSuccess = pDoc->m_pDataCtrl->AddPosl(m_Data);
	if (!bSuccess) return;

	CDialogMove::OnOK();
}

void CSeismicLoadProperty::OnCancel()
{
	CDialogMove::OnCancel();
}

void CSeismicLoadProperty::OnCmdApply()
{
	if (!Dlg2Data()) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	BOOL bSuccess;
	if (m_bModify)
		bSuccess = pDoc->m_pDataCtrl->ModifyPosl(m_OldKey, m_Key, m_Data);
	else
		bSuccess = pDoc->m_pDataCtrl->AddPosl(m_Data);
	if (!bSuccess) return;
}



void CSeismicLoadProperty::UpdateCurDlg()
{	
	CWnd* pPlaceHolder = GetDlgItem(IDC_ETC_EP_PLACEHOLDER);

	CMyChildDialog* pDlg = nullptr;
	m_mapChildDlg.Lookup(m_nCurrentDlg, pDlg);
	pDlg->CreateInit(pPlaceHolder);
	pDlg->UpdateData(FALSE);
	pDlg->ShowWindow(SW_SHOW);

/*	AlignDialog();*/

}


void CSeismicLoadProperty::AlignDialog()
{
	CRect rRef, rMove, rChild, rDlg;

	CMyChildDialog* dlg;
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
	dlg->GetWindowRect(rChild);

	CWnd* pWnd = GetDlgItem(IDC_ETC_EP_PLACEHOLDER);
	pWnd->GetWindowRect(rMove);
	rMove.bottom = rMove.bottom - (rMove.Height() - rChild.Height());
	rMove.right = rMove.right - (rMove.Width() - rChild.Width()); // MNET:3615-JHYUN-20080819
	ScreenToClient(rMove);
	pWnd->MoveWindow(rMove);

	SetWindowPos(NULL, rMove.left, rMove.top, rMove.Width() + globalUtils.ScaleByDPI(30), rMove.Height(), SWP_NOMOVE); // MNET:3615-JHYUN-20080819

	GetDlgItem(IDOK)->GetWindowRect(rMove);
	GetDlgItem(IDC_ETC_EP_PLACEHOLDER)->GetWindowRect(rRef);
	int nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(12);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlMethod, nDistY);

	GetDlgItem(IDOK)->GetWindowRect(rRef);
	GetWindowRect(rMove);

	rMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
	MoveWindow(rMove);

	/*UINT nItem = m_cmbDesignCode.GetItemData(m_cmbDesignCode.GetCurSel());*/

	CRect rToMove;
	BOOL bShow = FALSE;

	
	GetDlgItem(IDC_ETC_EP_PLACEHOLDER)->GetWindowRect(rRef);
	

	GetWindowRect(rToMove);
	rToMove.right = rRef.right + globalUtils.ScaleByDPI(16);
	MoveWindow(rToMove);

	GetDlgItem(IDC_ETC_SPEC_FRM)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_NRAC_RDO)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_ACCL_RDO)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_VELC_RDO)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_DISP_RDO)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_SCAL_FRM)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_SCLF_RDO)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_SCLF_EDT)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_MAXV_RDO)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_MAXV_EDT)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_MAXV_UNT)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_GRAV_FRM)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_GRAV_EDT)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_GRAV_UNT)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_DAMP_FRM)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_DAMP_EDT)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_GRAP_FRM)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_XLOG_CHK)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_YLOG_CHK)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_FUNC_GRP)->ShowWindow(bShow);
	GetDlgItem(IDC_ETC_IMPORT_BTN)->ShowWindow(bShow);
}