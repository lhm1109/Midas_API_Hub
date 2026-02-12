// SeisItemKorea1992.cpp : implementation file
//
// 2003. 12. 30   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemKorea1992.h"
#include "ETC_KSPeriodCalc.h"
#include "SeisItemDlgNew.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemKorea1992 dialog


CSeisItemKorea1992::CSeisItemKorea1992(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemKorea1992::IDD, pParent)
{
	m_aSoilName.RemoveAll();
	m_aSoilData.RemoveAll();
	m_aSoilName.Add(_T("S1(1.0)"));
	m_aSoilData.Add(1.0);
	m_aSoilName.Add(_T("S2(1.2)"));
	m_aSoilData.Add(1.2);
	m_aSoilName.Add(_T("S3(1.5)"));
	m_aSoilData.Add(1.5);

	m_aAreaName.RemoveAll();
	m_aAreaData.RemoveAll();
	m_aAreaName.Add(_LS(IDS_WG_CMD__ADD2__Area1_0_08_));
	m_aAreaData.Add(0.08);
	m_aAreaName.Add(_LS(IDS_WG_CMD__ADD2__Area2_0_12_));
	m_aAreaData.Add(0.12);

	m_aIFactorName.RemoveAll();
	m_aIFactorData.RemoveAll();
	m_aIFactorName.Add(_T("0.8"));
	m_aIFactorData.Add(0.8);
	m_aIFactorName.Add(_T("1.0"));
	m_aIFactorData.Add(1.0);
	m_aIFactorName.Add(_T("1.2"));
	m_aIFactorData.Add(1.2);
	m_aIFactorName.Add(_T("1.5"));
	m_aIFactorData.Add(1.5);

	m_bInit = FALSE;
	m_Data.Initialize();
	m_pParent = (CSeisItemDlgNew*)pParent;
}

void CSeisItemKorea1992::SetData2Dlg(T_SEIS_KS1992& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemKorea1992::SetDlg2Data(T_SEIS_KS1992& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemKorea1992::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisItemKorea1992)
	DDX_Control(pDX, IDC_CMD_SF_COMBO, m_cobxSFactor);
	DDX_Control(pDX, IDC_CMD_ZF_COMBO, m_cobxZFactor);
	DDX_Control(pDX, IDC_CMD_IF_COMBO, m_cobxIFactor);
	DDX_Control(pDX, IDC_CMD_PERIOD_A_X_EDIT, m_editAX);
	DDX_Control(pDX, IDC_CMD_PERIOD_A_Y_EDIT, m_editAY);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_X_EDIT, m_editCX);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_Y_EDIT, m_editCY);
	DDX_Control(pDX, IDC_CMD_RMF_X_EDIT, m_editRX);
	DDX_Control(pDX, IDC_CMD_RMF_Y_EDIT, m_editRY);
	DDX_Control(pDX, IDC_CMD_FRAME1, m_grpFold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisItemKorea1992, CChildDialog)
	//{{AFX_MSG_MAP(CSeisItemKorea1992)
	ON_BN_CLICKED(IDC_CMD_BTN_CALC_PERIOD, OnCmdBtnCalcPeriod)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemKorea1992 message handlers

BOOL CSeisItemKorea1992::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_editAX.SetUnitType(D_UNITSYS_NONE);
	m_editAY.SetUnitType(D_UNITSYS_NONE);
	m_editCX.SetUnitType(D_UNITSYS_NONE);
	m_editCY.SetUnitType(D_UNITSYS_NONE);
	m_editRX.SetUnitType(D_UNITSYS_NONE);
	m_editRY.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitSFactorCombo();
	InitZFactorCombo();
	InitIFactorCombo();
	SetFoldGroupCtrls();

	Data2Dlg();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemKorea1992::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemKorea1992::IDD, IDC_CMD_FRAME1, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemKorea1992::DestroyWindow()
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

void CSeisItemKorea1992::OnCmdBtnCalcPeriod()
{
	// TODO: Add your control notification handler code here
	CETC_KSPeriodCalc dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_dHn_x = m_dHXKOR;
	dlg.m_dHn_y = m_dHYKOR;
	dlg.m_dBx = m_dBXKOR1992;
	dlg.m_dBy = m_dBYKOR1992;
	dlg.m_nXMethod = m_nMtdXKOR1992;
	dlg.m_nYMethod = m_nMtdYKOR1992;
	dlg.m_nCodeType = 1; // 1=KS.Arch.1992

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriod_x);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriod_y);
		m_editCX.SetEditUnit(_tstof(cx));
		m_editCY.SetEditUnit(_tstof(cy));

		m_dHXKOR = dlg.m_dHn_x;
		m_dHYKOR = dlg.m_dHn_y;
		m_dBXKOR1992 = dlg.m_dBx;
		m_dBYKOR1992 = dlg.m_dBy;
		m_nMtdXKOR1992 = dlg.m_nXMethod;
		m_nMtdYKOR1992 = dlg.m_nYMethod;
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CSeisItemKorea1992::InitDefaultData()
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	double dBx = 1.0;
	double dBy = 1.0;
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

	if (nStorNum > 0)
	{
		for (int i = 0; i < nStorNum; i++)
		{
			pAttrCtrl->GetStor(aStorK[i], BaseStorD);
			if (BaseStorD.dStoryLevel > dBaseLevel) break;
		}
		if (BaseStorD.dStoryLevel > dBaseLevel)
		{
			dBx = BaseStorD.WindWidthX;
			dBy = BaseStorD.WindWidthY;
		}
		else { dBx = dBy = 1.0; }
	}
	else { dBx = dBy = 1.0; }

	m_dHXKOR = m_dHYKOR = dHn;
	m_dBXKOR1992 = dBx;
	m_dBYKOR1992 = dBy;
	m_nMtdXKOR1992 = 0;
	m_nMtdYKOR1992 = 0;

	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemKorea1992::InitSFactorCombo()
{
	m_cobxSFactor.ResetContent();
	for (int i = 0; i < m_aSoilName.GetSize(); i++)
		m_cobxSFactor.AddString(m_aSoilName[i]);
	m_cobxSFactor.SetCurSel(0);
}

void CSeisItemKorea1992::InitZFactorCombo()
{
	m_cobxZFactor.ResetContent();
	for (int i = 0; i < m_aAreaName.GetSize(); i++)
		m_cobxZFactor.AddString(m_aAreaName[i]);
	m_cobxZFactor.SetCurSel(0);
}

void CSeisItemKorea1992::InitIFactorCombo()
{
	m_cobxIFactor.ResetContent();
	for (int i = 0; i < m_aIFactorName.GetSize(); i++)
		m_cobxIFactor.AddString(m_aIFactorName[i]);
	m_cobxIFactor.SetCurSel(0);
}

void CSeisItemKorea1992::Data2Dlg()
{
	int i;
	T_PREFERENCE ref;
	CDBDoc::GetDocPoint()->m_pInitCtrl->GetPreference(ref);
	double zero = ref.Data.dblRegardZero;

	for (i = 0; i < m_aSoilData.GetSize(); i++)
		if (fabs(m_aSoilData[i] - m_Data.dSoilFactor) <= zero) break;
	m_cobxSFactor.SetCurSel(i);

	for (i = 0; i < m_aAreaData.GetSize(); i++)
		if (fabs(m_aAreaData[i] - m_Data.dZoneFactor) <= zero) break;
	m_cobxZFactor.SetCurSel(i);

	//   for (i=0; i<m_aIFactorData.GetSize(); i++)
	//     if (fabs(m_aIFactorData[i] - m_Data.dImportanceFactor) <= zero) break;
	//   m_cobxIFactor.SetCurSel(i);
	CString csVal;
	csVal.Format(_T("%g"), m_Data.dImportanceFactor);
	if (CB_ERR == m_cobxIFactor.FindStringExact(-1, csVal))
	{
		m_cobxIFactor.SetWindowText(csVal);
	}
	else
	{
		m_cobxIFactor.SelectString(-1, csVal);
	}

	m_editAX.SetEditUnit(m_Data.dPeriodAnalysis_X);
	m_editAY.SetEditUnit(m_Data.dPeriodAnalysis_Y);
	m_editCX.SetEditUnit(m_Data.dPeriodCode_X);
	m_editCY.SetEditUnit(m_Data.dPeriodCode_Y);
	m_editRX.SetEditUnit(m_Data.dResponseModificationFactor_X);
	m_editRY.SetEditUnit(m_Data.dResponseModificationFactor_Y);
}

BOOL CSeisItemKorea1992::Dlg2Data()
{
	int nIndex;
	CString str;

	nIndex = m_cobxSFactor.GetCurSel();
	if (nIndex >= m_aSoilData.GetSize()) return FALSE;
	m_Data.dSoilFactor = m_aSoilData[nIndex];

	nIndex = m_cobxZFactor.GetCurSel();
	if (nIndex >= m_aAreaData.GetSize()) return FALSE;
	m_Data.dZoneFactor = m_aAreaData[nIndex];

	CFormulaEdit::GetEditValue(&m_cobxIFactor, m_Data.dImportanceFactor);
	CFormulaEdit::GetEditValue(&m_editAX, m_Data.dPeriodAnalysis_X);
	CFormulaEdit::GetEditValue(&m_editAY, m_Data.dPeriodAnalysis_Y);
	CFormulaEdit::GetEditValue(&m_editCX, m_Data.dPeriodCode_X);
	CFormulaEdit::GetEditValue(&m_editCY, m_Data.dPeriodCode_Y);
	CFormulaEdit::GetEditValue(&m_editRX, m_Data.dResponseModificationFactor_X);
	CFormulaEdit::GetEditValue(&m_editRY, m_Data.dResponseModificationFactor_Y);

	if (m_Data.dPeriodAnalysis_X < 0 || m_Data.dPeriodAnalysis_Y < 0 ||
		m_Data.dPeriodCode_X < 0 || m_Data.dPeriodCode_Y < 0)
	{
		CString strError;
		strError.Format(_T("%s! "), _LS(IDS_CMD_HSFC_error));
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}

	/*
	int nIndex;
	CString csValue;

	nIndex = m_cobxSFactor.GetCurSel();
	if (nIndex >= m_aSoilData.GetSize()) return FALSE;
	m_Data.dSoilFactor = m_aSoilData[nIndex];

	nIndex = m_cobxZFactor.GetCurSel();
	if (nIndex >= m_aAreaData.GetSize()) return FALSE;
	m_Data.dZoneFactor = m_aAreaData[nIndex];

	m_cobxIFactor.GetWindowText(csValue);
	m_Data.dImportanceFactor = _tstof(csValue);
	
	m_Data.dPeriodAnalysis_X = m_editAX.GetEditValue();
	m_Data.dPeriodAnalysis_Y = m_editAY.GetEditValue();
	m_Data.dPeriodCode_X = m_editCX.GetEditValue();
	m_Data.dPeriodCode_Y = m_editCY.GetEditValue();
	m_Data.dResponseModificationFactor_X = m_editRX.GetEditValue();
	m_Data.dResponseModificationFactor_Y = m_editRY.GetEditValue();
	*/

	return TRUE;
}

LRESULT CSeisItemKorea1992::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
