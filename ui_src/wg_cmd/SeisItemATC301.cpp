// SeisItemATC301.cpp : implementation file
//
// 2003. 12. 29    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemATC301.h"
#include "SeisItemDlgNew.h"

#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemATC301 dialog


CSeisItemATC301::CSeisItemATC301(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemATC301::IDD, pParent)
{
	m_Data.Initialize();
	m_pParent = (CSeisItemDlgNew*)pParent;
}

void CSeisItemATC301::SetData2Dlg(T_SEIS_ATC306& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemATC301::SetDlg2Data(T_SEIS_ATC306& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemATC301::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisItemATC301)
	DDX_Control(pDX, IDC_CMD_RMF_X_EDIT, m_editRMFX);
	DDX_Control(pDX, IDC_CMD_RMF_Y_EDIT, m_editRMFY);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_Y_EDIT, m_editCodeY);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_X_EDIT, m_editCodeX);
	DDX_Control(pDX, IDC_CMD_PERIOD_A_Y_EDIT, m_editAnalY);
	DDX_Control(pDX, IDC_CMD_PERIOD_A_X_EDIT, m_editAnalX);
	DDX_Control(pDX, IDC_CMD_ZF_EDIT, m_editZF);
	DDX_Control(pDX, IDC_CMD_SF_EDIT, m_editSF);
	DDX_Control(pDX, IDC_CMD_FRAME1, m_grpFold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisItemATC301, CChildDialog)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemATC301 message handlers
BOOL CSeisItemATC301::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editRMFX.SetUnitType(D_UNITSYS_NONE);
	m_editRMFY.SetUnitType(D_UNITSYS_NONE);
	m_editCodeY.SetUnitType(D_UNITSYS_NONE);
	m_editCodeX.SetUnitType(D_UNITSYS_NONE);
	m_editAnalY.SetUnitType(D_UNITSYS_NONE);
	m_editAnalX.SetUnitType(D_UNITSYS_NONE);
	m_editZF.SetUnitType(D_UNITSYS_NONE);
	m_editSF.SetUnitType(D_UNITSYS_NONE);
	SetFoldGroupCtrls();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CSeisItemATC301::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemATC301::IDD, IDC_CMD_FRAME1, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemATC301::DestroyWindow() 
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

void CSeisItemATC301::Data2Dlg()
{
	m_editZF.SetEditUnit(m_Data.dEffectivePeakVelocity);
	m_editSF.SetEditUnit(m_Data.dSoilProfileCoefficient);

	m_editAnalX.SetEditUnit(m_Data.dPeriodAnalysis_X);
	m_editAnalY.SetEditUnit(m_Data.dPeriodAnalysis_Y);
	m_editCodeX.SetEditUnit(m_Data.dPeriodCode_X);
	m_editCodeY.SetEditUnit(m_Data.dPeriodCode_Y);
	m_editRMFX.SetEditUnit(m_Data.dResponseModificationFactor_X);
	m_editRMFY.SetEditUnit(m_Data.dResponseModificationFactor_Y);
}

BOOL CSeisItemATC301::Dlg2Data()
{ 
	CFormulaEdit::GetEditValue(&m_editZF, m_Data.dEffectivePeakVelocity);
	CFormulaEdit::GetEditValue(&m_editSF, m_Data.dSoilProfileCoefficient);

	CFormulaEdit::GetEditValue(&m_editAnalX, m_Data.dPeriodAnalysis_X);
	CFormulaEdit::GetEditValue(&m_editAnalY, m_Data.dPeriodAnalysis_Y);
	CFormulaEdit::GetEditValue(&m_editCodeX, m_Data.dPeriodCode_X);
	CFormulaEdit::GetEditValue(&m_editCodeY, m_Data.dPeriodCode_Y);
	CFormulaEdit::GetEditValue(&m_editRMFX,  m_Data.dResponseModificationFactor_X);
	CFormulaEdit::GetEditValue(&m_editRMFY,  m_Data.dResponseModificationFactor_Y);
	
	if (m_Data.dPeriodAnalysis_X < 0 || m_Data.dPeriodAnalysis_Y < 0 || 
			m_Data.dPeriodCode_X < 0     || m_Data.dPeriodCode_Y < 0)
	{
		CString strError = _LS(IDS_CMD_HSFC_error);  // Error
		strError += _T("! ");         // Error!
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}

	/*
	m_Data.dSoilProfileCoefficient = m_editZF.GetEditValue();
	m_Data.dEffectivePeakVelocity = m_editSF.GetEditValue();

	m_Data.dPeriodAnalysis_X = m_editAnalY.GetEditValue();
	m_Data.dPeriodAnalysis_Y = m_editAnalX.GetEditValue();
	m_Data.dPeriodCode_X = m_editCodeY.GetEditValue();
	m_Data.dPeriodCode_Y = m_editCodeX.GetEditValue();
	m_Data.dResponseModificationFactor_X = m_editRMFX.GetEditValue();
	m_Data.dResponseModificationFactor_Y = m_editRMFY.GetEditValue();
	*/
	
	return TRUE;
}

LRESULT CSeisItemATC301::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
