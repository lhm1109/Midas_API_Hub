// SeisItemUBC1991.cpp : implementation file
//
// 2003. 12. 30   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemUBC1991.h"
#include "SeisItemDlgNew.h"

#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemUBC1991 dialog


CSeisItemUBC1991::CSeisItemUBC1991(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemUBC1991::IDD, pParent)
{
	m_pParent = (CSeisItemDlgNew*)pParent;

	m_aSoilName.RemoveAll();
	m_aSoilData.RemoveAll();
	m_aSoilName.Add(_T("S1 (1.0)"));
	m_aSoilData.Add(1.0);
	m_aSoilName.Add(_T("S2 (1.2)"));
	m_aSoilData.Add(1.2);
	m_aSoilName.Add(_T("S3 (1.5)"));
	m_aSoilData.Add(1.5);

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

	m_Data.Initialize();
}

void CSeisItemUBC1991::SetData2Dlg(T_SEIS_UBC1991& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemUBC1991::SetDlg2Data(T_SEIS_UBC1991& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemUBC1991::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_SF_COMBO, m_cobxSFactor);
	DDX_Control(pDX, IDC_CMD_ZF_COMBO, m_cobxZFactor);
	DDX_Control(pDX, IDC_CMD_IF_COMBO, m_cobxIFactor);
	DDX_Control(pDX, IDC_CMD_PERIOD_A_X_EDIT, m_editAX);
	DDX_Control(pDX, IDC_CMD_PERIOD_A_Y_EDIT, m_editAY);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_X_EDIT, m_editCX);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_Y_EDIT, m_editCY);
	DDX_Control(pDX, IDC_CMD_RMF_X_EDIT, m_editRwX);
	DDX_Control(pDX, IDC_CMD_RMF_Y_EDIT, m_editRwY);
	DDX_Control(pDX, IDC_CMD_FRAME1, m_grpFold);
}


BEGIN_MESSAGE_MAP(CSeisItemUBC1991, CChildDialog)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemUBC1991 message handlers

BOOL CSeisItemUBC1991::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_editAX.SetUnitType(D_UNITSYS_NONE);
	m_editAY.SetUnitType(D_UNITSYS_NONE);
	m_editCX.SetUnitType(D_UNITSYS_NONE);
	m_editCY.SetUnitType(D_UNITSYS_NONE);
	m_editRwX.SetUnitType(D_UNITSYS_NONE);
	m_editRwY.SetUnitType(D_UNITSYS_NONE);

	InitSFactorCombo();
	InitZFactorCombo();
	InitIFactorCombo();

	SetFoldGroupCtrls();
	Data2Dlg();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemUBC1991::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemUBC1991::IDD, IDC_CMD_FRAME1, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemUBC1991::DestroyWindow()
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

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CSeisItemUBC1991::InitSFactorCombo()
{
	m_cobxSFactor.ResetContent();
	for (int i = 0; i < m_aSoilName.GetSize(); i++)
		m_cobxSFactor.AddString(m_aSoilName[i]);
	m_cobxSFactor.SetCurSel(0);
}

void CSeisItemUBC1991::InitZFactorCombo()
{
	m_cobxZFactor.ResetContent();
	for (int i = 0; i < m_aZoneName.GetSize(); i++)
		m_cobxZFactor.AddString(m_aZoneName[i]);
	m_cobxZFactor.SetCurSel(0);
}

void CSeisItemUBC1991::InitIFactorCombo()
{
	m_cobxIFactor.ResetContent();
	for (int i = 0; i < m_aIFactorName.GetSize(); i++)
		m_cobxIFactor.AddString(m_aIFactorName[i]);
	m_cobxIFactor.SetCurSel(0);
}

void CSeisItemUBC1991::Data2Dlg()
{
	int i;
	T_PREFERENCE ref;
	CDBDoc::GetDocPoint()->m_pInitCtrl->GetPreference(ref);
	double zero = ref.Data.dblRegardZero;

	for (i = 0; i < m_aSoilData.GetSize(); i++)
		if (fabs(m_aSoilData[i] - m_Data.dSoilProfileFactor) <= zero) break;
	m_cobxSFactor.SetCurSel(i);

	for (i = 0; i < m_aZoneData.GetSize(); i++)
		if (fabs(m_aZoneData[i] - m_Data.dZoneFactor) <= zero) break;
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
	m_editRwX.SetEditUnit(m_Data.dSystemCoefficient_X);
	m_editRwY.SetEditUnit(m_Data.dSystemCoefficient_Y);
}

BOOL CSeisItemUBC1991::Dlg2Data()
{
	int nIndex;
	CString str;

	nIndex = m_cobxSFactor.GetCurSel();
	if (nIndex >= m_aSoilData.GetSize()) return FALSE;
	m_Data.dSoilProfileFactor = m_aSoilData[nIndex];

	nIndex = m_cobxZFactor.GetCurSel();
	if (nIndex >= m_aZoneData.GetSize()) return FALSE;
	m_Data.dZoneFactor = m_aZoneData[nIndex];

	CFormulaEdit::GetEditValue(&m_cobxIFactor, m_Data.dImportanceFactor);
	CFormulaEdit::GetEditValue(&m_editAX, m_Data.dPeriodAnalysis_X);
	CFormulaEdit::GetEditValue(&m_editAY, m_Data.dPeriodAnalysis_Y);
	CFormulaEdit::GetEditValue(&m_editCX, m_Data.dPeriodCode_X);
	CFormulaEdit::GetEditValue(&m_editCY, m_Data.dPeriodCode_Y);
	CFormulaEdit::GetEditValue(&m_editRwX, m_Data.dSystemCoefficient_X);
	CFormulaEdit::GetEditValue(&m_editRwY, m_Data.dSystemCoefficient_Y);
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
	m_Data.dSoilProfileFactor = m_aSoilData[nIndex];

	nIndex = m_cobxZFactor.GetCurSel();
	if (nIndex >= m_aZoneData.GetSize()) return FALSE;
	m_Data.dZoneFactor = m_aZoneData[nIndex];

	m_cobxIFactor.GetWindowText(csValue);
	m_Data.dImportanceFactor = _tstof(csValue);

	m_Data.dPeriodAnalysis_X = m_editAX.GetEditValue();
	m_Data.dPeriodAnalysis_Y = m_editAY.GetEditValue();
	m_Data.dPeriodCode_X = m_editCX.GetEditValue();
	m_Data.dPeriodCode_Y = m_editCY.GetEditValue();
	m_Data.dSystemCoefficient_X = m_editRwX.GetEditValue();
	m_Data.dSystemCoefficient_Y = m_editRwY.GetEditValue();
	*/

	return TRUE;
}

LRESULT CSeisItemUBC1991::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
