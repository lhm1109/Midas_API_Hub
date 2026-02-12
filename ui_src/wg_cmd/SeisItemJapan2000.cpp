// SeisItemJapan2000.cpp : implementation file
//
// 2003. 12. 29    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemJapan2000.h"

#include "SeisJISCalcDlg.h"
#include "SeisItemJapan2000Sub.h"
#include "SeisItemDlgNew.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemJapan2000 dialog


CSeisItemJapan2000::CSeisItemJapan2000(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemJapan2000::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisItemJapan2000)
	//}}AFX_DATA_INIT
	m_aCtrlRadio.Add(IDC_CMD_AI_AUTO_RADIO);
	m_aCtrlRadio.Add(IDC_CMD_AI_USER_RADIO);

	m_bInit = FALSE;
	m_bInitAi = TRUE;
	m_Data.Initialize();
	m_pParent = (CSeisItemDlgNew*)pParent;
}

void CSeisItemJapan2000::SetData2Dlg(T_SEIS_JIS& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemJapan2000::SetDlg2Data(T_SEIS_JIS& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemJapan2000::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisItemJapan2000)
	DDX_Control(pDX, IDC_CMD_SLP_JPN_ZONE_EDIT, m_editZoneF);
	DDX_Control(pDX, IDC_CMD_SLP_JPN_SHEAR_FACTOR, m_editCo);
	DDX_Control(pDX, IDC_CMD_SLP_JPN_PERIOD_COBX, m_cobxTc);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_X_EDIT2, m_editPeriodX);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_Y_EDIT2, m_editPeriodY);
	DDX_Control(pDX, IDC_CMD_SLP_JPN_TITLE4, m_grpFold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisItemJapan2000, CChildDialog)
	//{{AFX_MSG_MAP(CSeisItemJapan2000)
	ON_BN_CLICKED(IDC_CMD_BTN_CALC_PERIOD2, OnCmdBtnCalcPeriod2)
	ON_BN_CLICKED(IDC_CMD_AI_USER_BTN, OnCmdAiUserBtn)
	ON_BN_CLICKED(IDC_CMD_AI_AUTO_RADIO, OnCmdAiRadio)
	ON_BN_CLICKED(IDC_CMD_AI_USER_RADIO, OnCmdAiRadio)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemJapan2000 message handlers

BOOL CSeisItemJapan2000::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_editZoneF.SetUnitType(D_UNITSYS_NONE);
	m_editCo.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodX.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodY.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}
	if (m_bModify && m_Data.nAiMethod == 1)
	{
		m_bInitAi = FALSE;
	}
	InitSoilPeriodCombo();
	SetFoldGroupCtrls();

	Data2Dlg();

	GetDlgItem(IDC_CMD_PERIOD_A_X_EDIT2)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CMD_PERIOD_A_Y_EDIT2)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CMD_RMF_X_EDIT3)->SetWindowText(_T("0"));
	GetDlgItem(IDC_CMD_RMF_Y_EDIT3)->SetWindowText(_T("0"));

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemJapan2000::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemJapan2000::IDD, IDC_CMD_SLP_JPN_TITLE4, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemJapan2000::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	//Dlg2Data();
	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpFold.SetFoldState(FALSE, FALSE);
		}
	}

	return CChildDialog::DestroyWindow();
}

void CSeisItemJapan2000::OnCmdBtnCalcPeriod2()
{
	// TODO: Add your control notification handler code here
	CString cx, cy;

	CSeisJISCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_hx = m_dHXJPN2000;
	dlg.m_hy = m_dHYJPN2000;
	dlg.m_ax = m_dAXJPN2000;
	dlg.m_ay = m_dAYJPN2000;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_hx * (0.02 + 0.01 * dlg.m_ax));
		cy.Format(_T("%-10.4g"), dlg.m_hy * (0.02 + 0.01 * dlg.m_ay));
		m_editPeriodX.SetWindowText(cx);
		m_editPeriodY.SetWindowText(cy);
		/*
		cx = dlg.m_hx * (0.02 + 0.01 * dlg.m_ax);
		cy = dlg.m_hy * (0.02 + 0.01 * dlg.m_ay);
		m_editPeriodX.SetEditUnit(cx);
		m_editPeriodY.SetEditUnit(cy);
		*/
		m_dHXJPN2000 = dlg.m_hx;
		m_dHYJPN2000 = dlg.m_hy;
		m_dAXJPN2000 = dlg.m_ax;
		m_dAYJPN2000 = dlg.m_ay;
	}
}

void CSeisItemJapan2000::OnCmdAiUserBtn()
{
	// TODO: Add your control notification handler code here
	double dTX, dTY;
	CFormulaEdit::GetEditValue(&m_editPeriodX, dTX);
	CFormulaEdit::GetEditValue(&m_editPeriodY, dTY);

	CSeisItemJapan2000Sub dlg;
	dlg.SetData(dTX, dTY, &m_Data.aSeisJisAi, m_bInitAi);
	if (dlg.DoModal() == IDOK)
	{
		m_bInitAi = FALSE;
	}
}

void CSeisItemJapan2000::OnCmdAiRadio()
{
	// TODO: Add your control notification handler code here
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, nMethod);
	GetDlgItem(IDC_CMD_AI_USER_BTN)->EnableWindow(nMethod == 1);
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CSeisItemJapan2000::InitDefaultData()
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	double dHn = 0.0;
	T_STOR_D StorD, BaseStorD;

	// 주의 !
	// Seismic Load 에서는 m 단위로 변환하여 받아온다.
	// Wind Load 에서는 현재 단위로 받아온다.
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // M 기준
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);

	CArray<T_STOR_K,T_STOR_K> aStorK;
	pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0) pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);
	else StorD.Initialize();

	double dBaseLevel = pAttrCtrl->GetBaseLevel(FALSE);
	dHn = StorD.dStoryLevel - dBaseLevel;
	if (!pAttrCtrl->GetCountStor()) dHn = 0;

	m_dHXJPN2000 = m_dHYJPN2000 = dHn;
	m_dAXJPN2000 = m_dAYJPN2000 = 0;

	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemJapan2000::InitSoilPeriodCombo()
{
	m_cobxTc.ResetContent();
	m_cobxTc.AddString(_T("0.4sec (I)"));
	m_cobxTc.AddString(_T("0.6sec (II)"));
	m_cobxTc.AddString(_T("0.8sec (III)"));
	m_cobxTc.SetCurSel(0);
}

void CSeisItemJapan2000::Data2Dlg()
{
	int nTemp = 0;
	if (fabs(m_Data.dSoilFactor - 0.4) <= 1.0e-6) nTemp = 0;
	else if (fabs(m_Data.dSoilFactor - 0.6) <= 1.0e-6) nTemp = 1;
	else if (fabs(m_Data.dSoilFactor - 0.8) <= 1.0e-6) nTemp = 2;
	m_editZoneF.SetEditUnit(m_Data.dZoneFactor);
	m_editCo.SetEditUnit(m_Data.dStdShearFactor);
	m_cobxTc.SetCurSel(nTemp);
	m_editPeriodX.SetEditUnit(m_Data.dPeriodCode_X);
	m_editPeriodY.SetEditUnit(m_Data.dPeriodCode_Y);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadio, m_Data.nAiMethod);
	OnCmdAiRadio();
}

BOOL CSeisItemJapan2000::Dlg2Data()
{
	int nTemp = m_cobxTc.GetCurSel();
	if (nTemp == 0) m_Data.dSoilFactor = 0.4;
	else if (nTemp == 1) m_Data.dSoilFactor = 0.6;
	else if (nTemp == 2) m_Data.dSoilFactor = 0.8;
	CFormulaEdit::GetEditValue(&m_editZoneF, m_Data.dZoneFactor);
	CFormulaEdit::GetEditValue(&m_editCo, m_Data.dStdShearFactor);
	CFormulaEdit::GetEditValue(&m_editPeriodX, m_Data.dPeriodCode_X);
	CFormulaEdit::GetEditValue(&m_editPeriodY, m_Data.dPeriodCode_Y);

	if (m_Data.dPeriodCode_X < 0 || m_Data.dPeriodCode_Y < 0)
	{
		CString strError;
		strError.Format(_T("%s! "), _LS(IDS_CMD_HSFC_error));
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, m_Data.nAiMethod);

	return TRUE;
}

LRESULT CSeisItemJapan2000::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
