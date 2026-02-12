// SeisLoadClassifyMethodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisLoadClassifyMethodDlg.h"

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisLoadClassifyMethodDlg dialog
CSeisLoadClassifyMethodDlg::CSeisLoadClassifyMethodDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisLoadClassifyMethodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisLoadClassifyMethodDlg)
	m_nMethod = -1;
	//}}AFX_DATA_INIT

}


void CSeisLoadClassifyMethodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisLoadClassifyMethodDlg)
	DDX_Control(pDX, IDC_ETC_SEIS_EDIT0, m_edtGroundLevel);
	DDX_Control(pDX, IDC_ETC_SEIS_EDIT1, m_edtBedrockLevel);
	DDX_Control(pDX, IDC_ETC_SEIS_EDIT2, m_edtFootingLevel);
	DDX_Control(pDX, IDC_ETC_SEIS_UNIT0, m_wndGroundUnit);
	DDX_Control(pDX, IDC_ETC_SEIS_UNIT1, m_wndBedrockUnit);
	DDX_Control(pDX, IDC_ETC_SEIS_UNIT2, m_wndFootingUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisLoadClassifyMethodDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisLoadClassifyMethodDlg)
	ON_BN_CLICKED(IDC_ETC_SEIS_CLASSIFY_BTN, OnCmdClassify)
	ON_BN_CLICKED(IDC_ETC_SEIS_CLOSE, OnCmdCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisLoadClassifyMethodDlg message handlers

BOOL CSeisLoadClassifyMethodDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
		
	((CButton *)GetDlgItem(IDC_ETC_SEIS_EDIT5))->SetWindowText(_T(""));

	InitialData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisLoadClassifyMethodDlg::InitialData()
{
	m_edtGroundLevel.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtBedrockLevel.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtFootingLevel.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndGroundUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndBedrockUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndFootingUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	T_BLDC_D BldcD;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl->GetBldc(BldcD)) BldcD.Initialize();

	double dGroundLevel = BldcD.dBaseLevel;
	double dBedrockLevel = BldcD.dBedrockLevel;
	double dFootingLevel = pDoc->m_pAttrCtrl->GetBottomofFootingLevel();

	m_edtGroundLevel.SetEditUnit(dGroundLevel);
	m_edtBedrockLevel.SetEditUnit(dBedrockLevel);
	m_edtFootingLevel.SetEditUnit(dFootingLevel);

}


void CSeisLoadClassifyMethodDlg::OnCmdClassify() 
{
	double dGroundLevel = m_edtGroundLevel.GetEditValue();
	double dBedrockLevel = m_edtBedrockLevel.GetEditValue();
	double dFootingLevel = m_edtFootingLevel.GetEditValue();

	// 단위를 m로 바꾼다.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;

	double dHr   = pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dGroundLevel - dBedrockLevel);
	double dZb   = pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dGroundLevel - dFootingLevel);
	
	CString strMethod=_T("");
	if(dHr <= 15.0 && dZb <= dHr*(2.0/3.0))
	{
		strMethod = _LS(IDS_CMD_SEIS_LOAD_METHOD_EQU);
		m_nMethod = EN_EPSE_METHOD_EQUIVALENT;
	}
	else
	{
		strMethod = _LS(IDS_CMD_SEIS_LOAD_METHOD_RDM);
		m_nMethod = EN_EPSE_METHOD_RESPONSE;
	}
	
	((CButton *)GetDlgItem(IDC_ETC_SEIS_EDIT5))->SetWindowText(strMethod);
}

void CSeisLoadClassifyMethodDlg::OnCmdCancel() 
{
	CDialogMove::OnCancel();		
}