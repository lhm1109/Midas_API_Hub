// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "AntivibrationSupportGBItem_FPS.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBItem_FPS dialog


CAntivibrationSupportGBItem_FPS::CAntivibrationSupportGBItem_FPS(CWnd* pParent /*=NULL*/)
	: CChildDialog(CAntivibrationSupportGBItem_FPS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAntivibrationSupportGBItem_FPS)
	//}}AFX_DATA_INIT
}
CAntivibrationSupportGBItem_FPS::~CAntivibrationSupportGBItem_FPS()
{

}

void CAntivibrationSupportGBItem_FPS::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAntivibrationSupportGBItem_FPS)
	DDX_Control(pDX, IDC_CMD_FPS_R_COMBO, m_wndEffiRComb);
	DDX_Control(pDX, IDC_CMD_FPS_R_EDIT, m_wndEffiREdit);
	DDX_Control(pDX, IDC_CMD_MECH_KV_FPS_EDIT, m_wndMechKvcEdit_FPS);
	DDX_Control(pDX, IDC_CMD_MECH_KVT_FPS_EDIT, m_wndMechKvtEdit_FPS);
	DDX_Control(pDX, IDC_CMD_MECH_DY_FPS_EDIT, m_wndMechKhEdit_FPS);
	DDX_Control(pDX, IDC_CMD_MECH_FV_R_FPS_EDIT, m_wndMechrEdit_FPS);
	DDX_Control(pDX, IDC_CMD_MECH_US_FPS_COMBO, m_wndMechUs_FPS);
	DDX_Control(pDX, IDC_CMD_MECH_UF_FPS_COMBO, m_wndMechUf_FPS);
	DDX_Control(pDX, IDC_CMD_MECH_US_FPS_EDIT, m_wndMechUsEdit_FPS);
	DDX_Control(pDX, IDC_CMD_MECH_UF_FPS_EDIT, m_wndMechUfEdit_FPS);
	DDX_Control(pDX, IDC_CMD_ADJUST_COEF_TK_EDIT, m_wndTk);
	DDX_Control(pDX, IDC_CMD_ADJUST_COEF_TQ_EDIT, m_wndTq);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_FPS_HOR_DIST_EDIT, m_wndDgnHDistEdit_FPS);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_FPS_VER_BEAR_EDIT, m_wndDgnVBearEdit_FPS);

	DDX_Control(pDX, IDC_CMD_FPS_R_UNT, m_wndEffiRUnit);
	DDX_Control(pDX, IDC_CMD_MECH_KV_FPS_UNT, m_wndMechKvcUnit_FPS);
	DDX_Control(pDX, IDC_CMD_MECH_KVT_FPS_UNT, m_wndMechKvtUnit_FPS);
	DDX_Control(pDX, IDC_CMD_MECH_DY_FPS_UNT, m_wndMechDyUnit_FPS);
	DDX_Control(pDX, IDC_CMD_MECH_FV_R_FPS_UNT, m_wndMechrUnit_FPS);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_FPS_HOR_DIST_UNT, m_wndDgnHDistUnit_FPS);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_FPS_VER_BEAR_UNT, m_wndDgnVBearUnit_FPS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAntivibrationSupportGBItem_FPS, CChildDialog)
	//{{AFX_MSG_MAP(CAntivibrationSupportGBItem_FPS)
	ON_CBN_SELCHANGE(IDC_CMD_FPS_R_COMBO, OnSelChangeBasicEffiRCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MECH_US_FPS_COMBO, OnSelChangeMechUsCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MECH_UF_FPS_COMBO, OnSelChangeMechUfCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBItem_FPS message handlers
BOOL CAntivibrationSupportGBItem_FPS::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	CDBDoc* m_pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = m_pDoc->m_pUnitCtrl;
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);

	m_wndEffiRUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndMechKvcUnit_FPS.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndMechKvtUnit_FPS.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndMechDyUnit_FPS.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndMechrUnit_FPS.SetUnitType(D_UNITSYS_BASE_SEC_L);
	m_wndDgnHDistUnit_FPS.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDgnVBearUnit_FPS.SetUnitType(D_UNITSYS_BASE_FORCE);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CAntivibrationSupportGBItem_FPS::Data2Dlg()
{
	m_wndMechKvcEdit_FPS.SetEditUnit(m_Data.FPS.dKv);
	m_wndMechKvtEdit_FPS.SetEditUnit(0);
	m_wndMechKhEdit_FPS.SetEditUnit(m_Data.FPS.dK1);
	m_wndMechrEdit_FPS.SetEditUnit(m_Data.FPS.nr);
	if (m_Data.FPS.nInputMethod == 0)
	{
		m_wndEffiREdit.SetEditUnit(m_Data.FPS.nR);
		m_wndMechUsEdit_FPS.SetEditUnit(m_Data.FPS.dUs);
		m_wndMechUfEdit_FPS.SetEditUnit(m_Data.FPS.dUf);
	}
	else
	{
		InitBasicEffiRCmb();
		InitMechUsCmb();
		InitMechUfCmb();
		m_wndEffiRComb.SetCurSel(m_Data.FPS.nSelR);
		m_wndMechUs_FPS.SetCurSel(m_Data.FPS.nSelUs);
		m_wndMechUf_FPS.SetCurSel(m_Data.FPS.nSelUf);
	}

	m_wndTk.SetEditUnit(m_Data.FPS.dTk);
	m_wndTq.SetEditUnit(m_Data.FPS.dTq);
	m_wndDgnHDistEdit_FPS.SetEditUnit(m_Data.FPS.dHorDis);
	m_wndDgnVBearEdit_FPS.SetEditUnit(m_Data.FPS.dVBearing);

	EnableDisableItemCtrls(m_Data.FPS.nInputMethod);
}

BOOL CAntivibrationSupportGBItem_FPS::Dlg2Data()
{
	UpdateData();
	m_Data.FPS.dKv = m_wndMechKvcEdit_FPS.GetEditValue();
	m_Data.FPS.dKvt = 0;
	m_Data.FPS.dK1 = m_wndMechKhEdit_FPS.GetEditValue();
	m_Data.FPS.nr = m_wndMechrEdit_FPS.GetEditValue();
	if (m_Data.FPS.nInputMethod == 0)
	{
		m_Data.FPS.nR = m_wndEffiREdit.GetEditValue();
		m_Data.FPS.dUs = m_wndMechUsEdit_FPS.GetEditValue();
		m_Data.FPS.dUf = m_wndMechUfEdit_FPS.GetEditValue();
	}
	else
	{
		m_Data.FPS.nSelR = m_wndEffiRComb.GetCurSel();
		m_Data.FPS.nSelUs = m_wndMechUs_FPS.GetCurSel();
		m_Data.FPS.nSelUf = m_wndMechUf_FPS.GetCurSel();
		CString strR = _T("");
		m_wndEffiRComb.GetWindowText(strR);
		m_Data.FPS.nR = _ttoi(strR);
		double dUs[6] = { 0.01, 0.02, 0.03, 0.04, 0.05, 0.06 };
		double dUf[7] = { 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08 };
		m_Data.FPS.dUs = dUs[m_Data.FPS.nSelUs];
		m_Data.FPS.dUf = dUf[m_Data.FPS.nSelUf];
	}

	m_Data.FPS.dTk = m_wndTk.GetEditValue();
	m_Data.FPS.dTq = m_wndTq.GetEditValue();
	m_Data.FPS.dHorDis = m_wndDgnHDistEdit_FPS.GetEditValue();
	m_Data.FPS.dVBearing = m_wndDgnVBearEdit_FPS.GetEditValue();
	return TRUE;
}

void CAntivibrationSupportGBItem_FPS::OnSelChangeBasicEffiRCmb()
{
	UpdateData();
	CString strR = _T("");
	m_wndEffiRComb.GetWindowText(strR);
	m_Data.FPS.nSelR = m_wndEffiRComb.GetCurSel();
	m_Data.FPS.nR = _ttoi(strR);
}

void CAntivibrationSupportGBItem_FPS::InitBasicEffiRCmb()
{
	m_wndEffiRComb.ResetContent();
	CString strEffiR;
	if (m_Data.FPS.nCode == 1)
	{
		strEffiR.Format(_T("%.2f"), m_Data.FPS.nR);
		m_wndEffiRComb.AddString(strEffiR);
		m_wndEffiRComb.SetCurSel(0);
		return;
	}
	double dR11[3] = { 2.000, 2.500, 3.000};
	double dR12[3] = { 2.500, 3.000, 4.000};
	double dR2[3] = { 3.000, 4.000, 4.500};

	double dR = 0;
	int i = 0;
	if (m_Data.FPS.nClassify == 0)
	{
		if (m_Data.FPS.nInputType < 18)
		{
			for (i = 0; i < 3; i++)
			{
				dR = ConvertUnitROut(dR11[i]);
				strEffiR.Format(_T("%.2f"), dR);
				m_wndEffiRComb.AddString(strEffiR);
			}
		}
		else
		{
			for (i = 0; i < 3; i++)
			{
				dR = ConvertUnitROut(dR12[i]);
				strEffiR.Format(_T("%.2f"), dR);
				m_wndEffiRComb.AddString(strEffiR);
			}
		}
	}
	else
	{
		for (i = 0; i < 3; i++)
		{
			dR = ConvertUnitROut(dR2[i]);
			strEffiR.Format(_T("%.2f"), dR);
			m_wndEffiRComb.AddString(strEffiR);
		}
	}
	m_wndEffiRComb.SetCurSel(0);
}

void CAntivibrationSupportGBItem_FPS::OnSelChangeMechUsCmb()
{
	UpdateData();
	double dUs[6] = { 0.01, 0.02, 0.03, 0.04, 0.05 , 0.06};
	m_Data.FPS.nSelUs = m_wndMechUs_FPS.GetCurSel();
	m_Data.FPS.dUs = dUs[m_wndMechUs_FPS.GetCurSel()];
}

void CAntivibrationSupportGBItem_FPS::InitMechUsCmb()
{
	UpdateData(TRUE);
	CString strUs;
	m_wndMechUs_FPS.ResetContent();
	if (m_Data.FPS.nCode == 1)
	{
		strUs.Format(_T("%.2f"), m_Data.FPS.dUs);
		CDlgUtil::CobxAddItem(m_wndMechUs_FPS, strUs, 0);
		m_wndMechUs_FPS.SetCurSel(0);
		return;
	}

	double dUs[6] = { 0.01, 0.02, 0.03, 0.04, 0.05, 0.06 };
	for (int i = 0; i < 6; i++)
	{
		strUs.Format(_T("%.2f"), dUs[i]);
		CDlgUtil::CobxAddItem(m_wndMechUs_FPS, strUs, i);
	}
	m_wndMechUs_FPS.SetCurSel(0);
}

void CAntivibrationSupportGBItem_FPS::OnSelChangeMechUfCmb()
{
	UpdateData();
	double dUf[7] = { 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08 };
	m_Data.FPS.nSelUf = m_wndMechUf_FPS.GetCurSel();
	m_Data.FPS.dUf = dUf[m_wndMechUf_FPS.GetCurSel()];
}

void CAntivibrationSupportGBItem_FPS::InitMechUfCmb()
{
	UpdateData(TRUE);
	m_wndMechUf_FPS.ResetContent();
	CString strUf;
	if (m_Data.FPS.nCode == 1)
	{
		strUf.Format(_T("%.2f"), m_Data.FPS.dUf);
		CDlgUtil::CobxAddItem(m_wndMechUf_FPS, strUf, 0);
		m_wndMechUf_FPS.SetCurSel(0);
		return;
	}

	double dUf[7] = { 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08 };
	for (int i = 0; i < 7; i++)
	{
		strUf.Format(_T("%.2f"), dUf[i]);
		CDlgUtil::CobxAddItem(m_wndMechUf_FPS, strUf, i);
	}
	m_wndMechUf_FPS.SetCurSel(0);
}

void CAntivibrationSupportGBItem_FPS::EnableDisableItemCtrls(int nInputMethod)
{
	if (nInputMethod == 0)
	{
		m_wndMechKvcEdit_FPS.SetReadOnly(FALSE);
		m_wndMechKvtEdit_FPS.SetReadOnly(TRUE);
		m_wndMechKhEdit_FPS.SetReadOnly(FALSE);
		m_wndMechrEdit_FPS.SetReadOnly(FALSE);
		m_wndTk.SetReadOnly(FALSE);
		m_wndTq.SetReadOnly(FALSE);
		m_wndDgnHDistEdit_FPS.SetReadOnly(FALSE);
		m_wndDgnVBearEdit_FPS.SetReadOnly(FALSE);
		GetDlgItem(IDC_CMD_FPS_R_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_MECH_US_FPS_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_MECH_UF_FPS_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_FPS_R_EDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_MECH_US_FPS_EDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_MECH_UF_FPS_EDIT)->ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndMechKvcEdit_FPS.SetReadOnly(FALSE);
		m_wndMechKvtEdit_FPS.SetReadOnly(TRUE);
		m_wndMechKhEdit_FPS.SetReadOnly(FALSE);
		m_wndMechrEdit_FPS.SetReadOnly(FALSE);
		m_wndTk.SetReadOnly(TRUE);
		m_wndTq.SetReadOnly(TRUE);
		m_wndDgnHDistEdit_FPS.SetReadOnly(TRUE);
		m_wndDgnVBearEdit_FPS.SetReadOnly(TRUE);
		GetDlgItem(IDC_CMD_FPS_R_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_MECH_US_FPS_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_MECH_UF_FPS_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_FPS_R_EDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_MECH_US_FPS_EDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_MECH_UF_FPS_EDIT)->ShowWindow(SW_HIDE);
	}

}

void CAntivibrationSupportGBItem_FPS::SetData2Dlg(T_ASGB_D& data)
{
	m_Data.FPS.nInputMethod = data.FPS.nInputMethod; //0:User, 1:Code 
	m_Data.FPS.nCode = data.FPS.nCode;
	m_Data.FPS.nClassify = data.FPS.nClassify;
	m_Data.FPS.nInputType = data.FPS.nInputType;
	m_Data.FPS.nR = data.FPS.nR;
	m_Data.FPS.nSelR = data.FPS.nSelR;
	m_Data.FPS.dKv = data.FPS.dKv; //Kvc
	m_Data.FPS.dKvt = data.FPS.dKvt;
	m_Data.FPS.dK1 = data.FPS.dK1;
	m_Data.FPS.nr = data.FPS.nr;
	m_Data.FPS.dUs = data.FPS.dUs;
	m_Data.FPS.nSelUs = data.FPS.nSelUs;
	m_Data.FPS.dUf = data.FPS.dUf;
	m_Data.FPS.nSelUf = data.FPS.nSelUf;
	m_Data.FPS.dTk = data.FPS.dTk;
	m_Data.FPS.dTq = data.FPS.dTq;
	m_Data.FPS.dHorDis = data.FPS.dHorDis;
	m_Data.FPS.dVBearing = data.FPS.dVBearing;
	Data2Dlg();
}

BOOL CAntivibrationSupportGBItem_FPS::SetDlg2Data(T_ASGB_D& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CAntivibrationSupportGBItem_FPS::OnOK()
{
	// TODO: Add extra validation here
	if (!Dlg2Data())	return;

	if (m_wndMechKvcEdit_FPS.GetEditValue() <= 0 || m_wndTk.GetEditValue() <= 0 || m_wndTq.GetEditValue() <= 0
		|| m_wndDgnHDistEdit_FPS.GetEditValue() <= 0 || m_wndDgnVBearEdit_FPS.GetEditValue() <= 0
		|| m_wndMechKvtEdit_FPS.GetEditValue() <= 0 || m_wndMechrEdit_FPS.GetEditValue() <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR)); 	return;
	}

	CChildDialog::OnOK();
}

void CAntivibrationSupportGBItem_FPS::OnCancel()
{
	// TODO: Add extra cleanup here

	CChildDialog::OnCancel();
}

double CAntivibrationSupportGBItem_FPS::ConvertUnitROut(double dR)
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	int iLenID = CurIndex.nBase_Length;
	switch (iLenID)
	{
	case (D_UNITSYS_LENGTH_INDEX_MM): dR /= 0.001; break;
	case (D_UNITSYS_LENGTH_INDEX_CM): dR /= 0.01; break;
	case (D_UNITSYS_LENGTH_INDEX_M): dR /= 1.; break;
	case (D_UNITSYS_LENGTH_INDEX_IN): dR /= 0.0254; break;
	case (D_UNITSYS_LENGTH_INDEX_FT): dR /= 0.3048; break;
	default:
		break;
	}

	return dR;
}
