// SeisItemUBC1997.cpp : implementation file
//
// 2003. 12. 30   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemUBC1997.h"

#include "SeisUBCClacDlg.h"
#include "SeisItemDlgNew.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemUBC1997 dialog


CSeisItemUBC1997::CSeisItemUBC1997(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemUBC1997::IDD, pParent)
{
	m_pParent = (CSeisItemDlgNew*)pParent;

	m_aZoneName.RemoveAll();
	m_aZoneData.RemoveAll();
	m_aZoneName.Add(_T("1 (0.075)"));
	m_aZoneData.Add(0.075);
	m_aZoneName.Add(_T("2A(0.15)"));
	m_aZoneData.Add(0.15);
	m_aZoneName.Add(_T("2B(0.20)"));
	m_aZoneData.Add(0.20);
	m_aZoneName.Add(_T("3 (0.30)"));
	m_aZoneData.Add(0.30);
	m_aZoneName.Add(_T("4 (0.40)"));
	m_aZoneData.Add(0.40);

	m_aIFactorName.RemoveAll();
	m_aIFactorData.RemoveAll();
	m_aIFactorName.Add(_T("1.0"));
	m_aIFactorData.Add(1.0);
	m_aIFactorName.Add(_T("1.25"));
	m_aIFactorData.Add(1.25);

	m_bInit = FALSE;
	m_Data.Initialize();
}

void CSeisItemUBC1997::SetData2Dlg(T_SEIS_UBC1997& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemUBC1997::SetDlg2Data(T_SEIS_UBC1997& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemUBC1997::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisItemUBC1997)
	DDX_Control(pDX, IDC_CMD_SPT_COMBO2, m_cobxSoilType);
	DDX_Control(pDX, IDC_CMD_ZF_COMBO2, m_cobxZFactor);
	DDX_Control(pDX, IDC_CMD_SST_COMBO2, m_cobxSType);
	DDX_Control(pDX, IDC_CMD_CD_UNIT2, m_unitDistance);
	DDX_Control(pDX, IDC_CMD_CD_EDIT2, m_editDistance);
	DDX_Control(pDX, IDC_CMD_IF_COMBO2, m_cobxIFactor);

	DDX_Control(pDX, IDC_CMD_PERIOD_X_EDIT1, m_editPXAnal);
	DDX_Control(pDX, IDC_CMD_PERIOD_Y_EDIT1, m_editPYAnal);

	DDX_Control(pDX, IDC_CMD_PERIOD_X_EDIT2, m_editPX);
	DDX_Control(pDX, IDC_CMD_PERIOD_Y_EDIT2, m_editPY);
	DDX_Control(pDX, IDC_CMD_RMF_X_EDIT2, m_editRX);
	DDX_Control(pDX, IDC_CMD_RMF_Y_EDIT2, m_editRY);
	DDX_Control(pDX, IDC_CMD_FRAME3, m_grpFold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisItemUBC1997, CChildDialog)
	//{{AFX_MSG_MAP(CSeisItemUBC1997)
	ON_CBN_SELCHANGE(IDC_CMD_ZF_COMBO2, OnSelchangeCmdZfCombo)
	ON_BN_CLICKED(IDC_CMD_IBC_BTN_CALC_PERIOD, OnCmdUbcBtnCalcPeriod)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemUBC1997 message handlers

BOOL CSeisItemUBC1997::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here
	//m_unitDistance.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editDistance.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editPX.SetUnitType(D_UNITSYS_NONE);
	m_editPY.SetUnitType(D_UNITSYS_NONE);

	//6.5.0 추가
	m_editPXAnal.SetUnitType(D_UNITSYS_NONE);
	m_editPYAnal.SetUnitType(D_UNITSYS_NONE);

	m_editRX.SetUnitType(D_UNITSYS_NONE);
	m_editRY.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitSoilTypeCombo();
	InitZFactorCombo();
	InitSrcTypeCombo();
	InitIFactorCombo();
	SetFoldGroupCtrls();

	if (m_bModify) Data2Dlg();
	else
	{
		m_editDistance.SetWindowText(_T("10"));
		m_editRX.SetWindowText(_T("0"));
		m_editRY.SetWindowText(_T("0"));
	}

	OnSelchangeCmdZfCombo();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemUBC1997::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemUBC1997::IDD, IDC_CMD_FRAME3, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemUBC1997::DestroyWindow()
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

void CSeisItemUBC1997::OnCmdUbcBtnCalcPeriod()
{
	CSeisUBCClacDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_hnX = m_dHXUBC1997;
	dlg.m_hnY = m_dHYUBC1997;
	dlg.m_xrdo = m_nMtdXUBC1997;
	dlg.m_yrdo = m_nMtdYUBC1997;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriod_X);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriod_Y);
		m_editPX.SetEditUnit(_tstof(cx));
		m_editPY.SetEditUnit(_tstof(cy));

		m_dHXUBC1997 = dlg.m_hnX;
		m_dHYUBC1997 = dlg.m_hnY;
		m_nMtdXUBC1997 = dlg.m_xrdo;
		m_nMtdYUBC1997 = dlg.m_yrdo;
	}
}


void CSeisItemUBC1997::InitDefaultData()
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	double dHn = 0.0;
	T_STOR_D StorD, BaseStorD;

	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // meter 기준
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);

	CArray<T_STOR_K,T_STOR_K> aStorK;
	pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0) pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);
	else StorD.Initialize();

	double dBaseLevel = pAttrCtrl->GetBaseLevel(FALSE);
	dHn = StorD.dStoryLevel - dBaseLevel;
	if (!pAttrCtrl->GetCountStor()) dHn = 0;

	m_dHXUBC1997 = m_dHYUBC1997 = dHn;
	m_nMtdXUBC1997 = 0;
	m_nMtdYUBC1997 = 0;

	// user 기준 unit으로 전환...
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}


void CSeisItemUBC1997::OnSelchangeCmdZfCombo()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_cobxZFactor.GetCurSel();
	m_cobxSType.EnableWindow(nIndex == 4);
	m_editDistance.EnableWindow(nIndex == 4);
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CSeisItemUBC1997::InitSoilTypeCombo()
{
	CString aItem[6] = {_T("Sa"), _T("Sb"), _T("Sc"), _T("Sd"), _T("Se"), _T("Sf")};

	m_cobxSoilType.ResetContent();
	for (int i = 0; i < 6; i++)
		m_cobxSoilType.AddString(aItem[i]);
	m_cobxSoilType.SetCurSel(0);
}

void CSeisItemUBC1997::InitZFactorCombo()
{
	m_cobxZFactor.ResetContent();
	for (int i = 0; i < m_aZoneName.GetSize(); i++)
		m_cobxZFactor.AddString(m_aZoneName[i]);
	m_cobxZFactor.SetCurSel(0);
}

void CSeisItemUBC1997::InitSrcTypeCombo()
{
	CString aItem[3] = {_T("A"), _T("B"), _T("C")};

	m_cobxSType.ResetContent();
	for (int i = 0; i < 3; i++)
		m_cobxSType.AddString(aItem[i]);
	m_cobxSType.SetCurSel(0);
}

void CSeisItemUBC1997::InitIFactorCombo()
{
	m_cobxIFactor.ResetContent();
	for (int i = 0; i < m_aIFactorName.GetSize(); i++)
		m_cobxIFactor.AddString(m_aIFactorName[i]);
	m_cobxIFactor.SetCurSel(0);
}

void CSeisItemUBC1997::Data2Dlg()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_PREFERENCE ref;
	pDoc->m_pInitCtrl->GetPreference(ref);
	double zero = ref.Data.dblRegardZero;

	m_cobxSoilType.SetCurSel(m_Data.nSoilProfileType - 1);
	m_cobxSType.SetCurSel(m_Data.nSeismicSourceType - 1);
	m_editDistance.SetEditUnit(m_Data.dCloseastDistance);

	//   for (int i=0; i<m_aIFactorData.GetSize(); i++)
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
	int i = 0;
	for (i = 0; i < m_aZoneData.GetSize(); i++)
		if (fabs(m_aZoneData[i] - m_Data.dSeismicZoneFactor) <= zero) break;
	m_cobxZFactor.SetCurSel(i);

	m_editPX.SetEditUnit(m_Data.dPeriod_X);
	m_editPY.SetEditUnit(m_Data.dPeriod_Y);

	// 6.5.0 추가
	m_editPXAnal.SetEditUnit(m_Data.dPeriodAnalysis_X);
	m_editPYAnal.SetEditUnit(m_Data.dPeriodAnalysis_Y);

	m_editRX.SetEditUnit(m_Data.dResponseModificationFactor_X);
	m_editRY.SetEditUnit(m_Data.dResponseModificationFactor_Y);
}

BOOL CSeisItemUBC1997::Dlg2Data()
{
	int nIndex = m_cobxZFactor.GetCurSel();
	m_Data.dSeismicZoneFactor = m_aZoneData[nIndex];
	if (m_cobxZFactor.GetCurSel() == 4)
	{
		m_Data.nSeismicSourceType = m_cobxSType.GetCurSel() + 1;
		CFormulaEdit::GetEditValue(&m_editDistance, m_Data.dCloseastDistance);
	}
	else
	{
		m_Data.nSeismicSourceType = 1;
		m_Data.dCloseastDistance = 10.0;
	}

	m_Data.nSoilProfileType = m_cobxSoilType.GetCurSel() + 1;
	CFormulaEdit::GetEditValue(&m_cobxIFactor, m_Data.dImportanceFactor);

	CFormulaEdit::GetEditValue(&m_editPX, m_Data.dPeriod_X);
	CFormulaEdit::GetEditValue(&m_editPY, m_Data.dPeriod_Y);

	//6.5.0추가
	CFormulaEdit::GetEditValue(&m_editPXAnal, m_Data.dPeriodAnalysis_X);
	CFormulaEdit::GetEditValue(&m_editPYAnal, m_Data.dPeriodAnalysis_Y);

	CFormulaEdit::GetEditValue(&m_editRX, m_Data.dResponseModificationFactor_X);
	CFormulaEdit::GetEditValue(&m_editRY, m_Data.dResponseModificationFactor_Y);
	if (m_Data.dPeriod_X < 0 || m_Data.dPeriod_Y < 0)
	{
		CString strError;
		strError.Format(_T("%s! "), _LS(IDS_CMD_HSFC_error));
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}

	/*
	CString csValue;
	m_Data.nSeismicSourceType = m_cobxSType.GetCurSel();
	m_Data.dCloseastDistance = m_editDistance.GetEditValue();
	
	m_cobxIFactor.GetWindowText(csValue);
	m_Data.dImportanceFactor = _tstof(csValue);
 
	m_Data.dPeriod_X = m_editPX.GetEditValue();
	m_Data.dPeriod_Y = m_editPY.GetEditValue();
	m_Data.dResponseModificationFactor_X = m_editRX.GetEditValue();
	m_Data.dResponseModificationFactor_Y = m_editRY.GetEditValue();
	*/

	return TRUE;
}

LRESULT CSeisItemUBC1997::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
