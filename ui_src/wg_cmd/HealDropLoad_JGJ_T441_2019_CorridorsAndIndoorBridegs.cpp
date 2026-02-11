// HealDropLoad_GB_T51288_WalkRhythm.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "HealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs.h"
#include "..\wg_db\wg_db_dbdoc.h"
#include "..\wg_db\wg_db_unitctrl.h"
#include "..\MIT_lib\MConvertString.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs dialog


CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs(CWnd* pParent /*=NULL*/)
	: CHeelLoadChildDialog(CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs)
	m_dPb = 0.28;
	m_dF1 = 1.0;
	m_dPhi = 0.25;
	m_dArea = 10;
	m_dDampRto = 0.01;
	m_bAutoCal = TRUE;
	//}}AFX_DATA_INIT
	m_stPbUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_stAreaUnit.SetUnitType(D_UNITSYS_BASE_AREA);
	m_nNType = 0;
	m_nOrderNumType = 0;
	m_nLoadDir = 0;
	m_iRepeat = 20;
}


void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::DoDataExchange(CDataExchange* pDX)
{
	CHeelLoadChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs)
	DDX_Radio(pDX, IDC_HEELLOAD_WALK_CORRIDOR_VER_RADIO, m_nLoadDir);
	DDX_Control(pDX, IDC_HEELLOAD_WALK_CORRIDOR_PB_UNIT, m_stPbUnit);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_CORRIDOR_PB_EDIT, m_dPb);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_CORRIDOR_F1_EDIT, m_dF1);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_CORRIDOR_PHI_EDIT, m_dPhi);
	DDX_Control(pDX, IDC_HEELLOAD_WALK_CORRIDOR_AREA_UNIT, m_stAreaUnit);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_CORRIDOR_AREA_EDIT, m_dArea);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_CORRIDOR_DAMPRTO_EDIT, m_dDampRto);
	DDX_Check(pDX, IDC_HEELLOAD_WALK_CORRIDOR_AUTOCAL_CHECK, m_bAutoCal);
	DDX_Control(pDX, IDC_HEELLOAD_WALK_CORRIDOR_N_COMBO, m_cmbNType);
	DDX_Control(pDX, IDC_HEELLOAD_WALK_CORRIDOR_CALC_ORDER_COMBO, m_cmbOrderNumType);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_CORRIDOR_REPEAT_EDIT, m_iRepeat);
	DDV_MinMaxInt(pDX, m_iRepeat, 1, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs, CHeelLoadChildDialog)
	//{{AFX_MSG_MAP(CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs)
	// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_HEELLOAD_WALK_CORRIDOR_VER_RADIO, OnChkLoadDirRadio)
	ON_BN_CLICKED(IDC_HEELLOAD_WALK_CORRIDOR_HOR_RADIO, OnChkLoadDirRadio)
	ON_BN_CLICKED(IDC_HEELLOAD_WALK_CORRIDOR_AUTOCAL_CHECK, OnChkAutoCalcPhi)
	ON_CBN_SELCHANGE(IDC_HEELLOAD_WALK_CORRIDOR_N_COMBO, OnSelchangeNCmb)
	ON_CBN_SELCHANGE(IDC_HEELLOAD_WALK_CORRIDOR_CALC_ORDER_COMBO, OnSelchangeOrderNumCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs message handlers

void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::End()
{
	//UpdateData(TRUE);
	CHeelLoadChildDialog::End();
}

void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::MakeSpectrumData()
{
	
	double tn;
	double st= 0.005;
	double ed= 1.0 / m_dF1; // 1 second
	int nStep = (ed- st)/0.005;
	if (m_iRepeat < 1) m_iRepeat = 1;

	double dfs1 = 0.;
	double dRto = m_dF1 / (m_nNType + 1);
	if (dRto < 1.25)
		dfs1 = 1.25;
	else if (dRto > 2.50)
		dfs1 = 2.50;
	else
		dfs1 = dRto;
	double dfs2 = 2 * dfs1;
	double dPhi, dPhiL;
	GetPhi(dfs1, dfs2, dPhi);

	double dfsL = 0.;
	if (dRto < 0.5)
		dfsL = 0.5;
	else if (dRto > 1.2)
		dfsL = 1.20;
	else
		dfsL = dRto;

	GetPhiL(dfsL , dPhiL);
	double dGamma1;
	GetGamma1(dGamma1);

	if (!m_bAutoCal)
	{
		dPhiL = dPhi = m_dPhi;
	}

	double Fp;

	for (int t=0; t<nStep * m_iRepeat; t++)
	{
		tn = st + t * 0.005;
		Fp = 0.0;
		if (m_nLoadDir == 0)
		{
			if (m_nOrderNumType == 0)
				Fp = m_dPb * dGamma1 * dPhi * cos(2 * M_PI * dfs1 * tn);
			else
				Fp = m_dPb * dGamma1 * dPhi * cos(2 * M_PI * dfs2 * tn);
		} 
		else
			Fp = m_dPb * dGamma1 * dPhiL * cos(2 * M_PI * dfsL * tn);

		m_parPeriod->Add(tn);
		m_parAccel->Add(Fp);
	}


	m_strFuncName=_LS(IDS_WG_CMD__ADD2__Corridors_Indoor_Bridegs_JGJT441_);


}

void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::GetGamma1(double& dGamma1)
{
	double dN = 0.5 * m_dArea;
	dGamma1 = 10.8 * sqrt(m_dDampRto * dN) / m_dArea;
}

void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::GetPhi(double dfs1, double dfs2, double &dPhi)
{
	if (m_nOrderNumType == 0)
	{
		if (dfs1 <= 1.25)
			dPhi = 0.;
		else if (dfs1 > 1.25 && dfs1 <= 1.7)
			dPhi = (dfs1-1.25) / (1.7-1.25);
		else if (dfs1 > 1.25 && dfs1 <= 2.1)
			dPhi = 1.0;
		else if (dfs1 > 2.1 && dfs1 <= 2.25)
			dPhi = 1.0 - (dfs1 - 2.1) / (2.3 - 2.1);
		else
			dPhi = 0.25;
	}
	else
	{
		if (dfs2 > 2.5 && dfs2 <= 4.2)
			dPhi = 0.25;
		else if (dfs2 > 4.2 && dfs2 <= 4.6)
			dPhi = 0.25 * (1.0 - (dfs2 - 4.2) / (4.6 - 4.2));
		else
			dPhi = 0.;
	}
}

void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::GetPhiL(double dfsL, double& dPhiL)
{
	if (dfsL <= 0.5)
		dPhiL = 0.;
	else if (dfsL > 0.5 && dfsL <= 0.7)
		dPhiL = (dfsL - 0.5) / (0.7 - 0.5);
	else if (dfsL > 0.7 && dfsL <= 1.0)
		dPhiL = 1.0;
	else if (dfsL > 1.0 && dfsL <= 1.2)
		dPhiL = 1.0 - (dfsL - 1.0) / (1.2 - 1.0);
	else
		dPhiL = 0.;
}

BOOL CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::ISVALID()
{
	return TRUE;
}

void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::OnChkLoadDirRadio()
{
	UpdateData(TRUE);
	double dMultFact, dDivFact;
	GetConvertFactor_ForceUnitToCurrent(D_UNITSYS_FORCE_INDEX_KN, dMultFact, dDivFact);
	m_dPb = m_nLoadDir == 0 ? 0.28 : 0.035;
	m_dPb = m_dPb * dMultFact / dDivFact;
	CString strPb;
	strPb.Format(_T("%.3f"), m_dPb);
	GetDlgItem(IDC_HEELLOAD_WALK_CORRIDOR_PB_EDIT) ->SetWindowText(strPb);
	ChangeItemStatic();
	OnChkAutoCalcPhi();
	UpdateData(FALSE);
}

void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::OnChkAutoCalcPhi()
{
	UpdateData(TRUE);
	
	if (m_bAutoCal)
	{
		GetDlgItem(IDC_HEELLOAD_WALK_CORRIDOR_PHI_EDIT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_HEELLOAD_WALK_CORRIDOR_PHI_EDIT)->EnableWindow(TRUE);
	}
	ChangeAutoCalc();
	UpdateData(FALSE);
}

void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::OnSelchangeNCmb()
{
	m_nNType = m_cmbNType.GetCurSel();
	m_cmbNType.SetCurSel(m_nNType);
}

void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::OnSelchangeOrderNumCmb()
{
	m_nOrderNumType = m_cmbOrderNumType.GetCurSel();
	m_cmbOrderNumType.SetCurSel(m_nOrderNumType);
	ChangeAutoCalc();
}

BOOL CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::OnInitDialog()
{
	CHeelLoadChildDialog::OnInitDialog();
	ChangeItemStatic();
	double dMultFact,dDivFact;
	GetConvertFactor_ForceUnitToCurrent(D_UNITSYS_FORCE_INDEX_KN,dMultFact,dDivFact);
	m_dPb = m_nLoadDir == 0 ? 0.28 : 0.035;
	m_dPb = m_dPb *dMultFact/dDivFact;

	GetConvertFactor_AreaUnitToCurrent(D_UNITSYS_LENGTH_FACTOR_M, dMultFact, dDivFact);
	m_dArea = 10 * 10E3;
	m_dArea = m_dArea * dMultFact / dDivFact;

	m_cmbNType.ResetContent();
	CString strNType[]={ _T("1"),_T("2"),_T("3")};
	for(int i=0;i <3 ; i++)
	{
		m_cmbNType.AddString(strNType[i]);
	}
	m_cmbNType.SetCurSel(0);

	m_cmbOrderNumType.ResetContent();
	CString strOrderNumType[]={ _T("1"),_T("2")};
	for(int i=0;i <2 ; i++)
	{
		m_cmbOrderNumType.AddString(strOrderNumType[i]);
	}
	m_cmbOrderNumType.SetCurSel(0);

	if (m_bAutoCal)
	{
		GetDlgItem(IDC_HEELLOAD_WALK_CORRIDOR_PHI_EDIT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_HEELLOAD_WALK_CORRIDOR_PHI_EDIT)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::ChangeItemStatic()
{
	if (m_nLoadDir == 0)
	{
		GetDlgItem(IDC_WG_CMD_STATIC0)->SetWindowText(_LS(IDS_IDD_HEELDROPLOAD_WALK_CORRIDOR_IDC_WG_CMD_STATIC0));
		GetDlgItem(IDC_WG_CMD_STATIC1)->SetWindowText(_LS(IDS_IDD_HEELDROPLOAD_WALK_CORRIDOR_IDC_WG_CMD_STATIC1));
		GetDlgItem(IDC_WG_CMD_STATIC2)->SetWindowText(_LS(IDS_IDD_HEELDROPLOAD_WALK_CORRIDOR_IDC_WG_CMD_STATIC2));
		GetDlgItem(IDC_WG_CMD_STATIC12)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_HEELLOAD_WALK_CORRIDOR_CALC_ORDER_COMBO)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_WG_CMD_STATIC0)->SetWindowText(_LS(IDS_IDD_HEELDROPLOAD_WALK_CORRIDOR_IDC_WG_CMD_STATIC0_H));
		GetDlgItem(IDC_WG_CMD_STATIC1)->SetWindowText(_LS(IDS_IDD_HEELDROPLOAD_WALK_CORRIDOR_IDC_WG_CMD_STATIC1_H));
		GetDlgItem(IDC_WG_CMD_STATIC2)->SetWindowText(_LS(IDS_IDD_HEELDROPLOAD_WALK_CORRIDOR_IDC_WG_CMD_STATIC2_H));
		GetDlgItem(IDC_WG_CMD_STATIC12)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_HEELLOAD_WALK_CORRIDOR_CALC_ORDER_COMBO)->ShowWindow(SW_HIDE);
	}
}
void CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs::ChangeAutoCalc()
{
	double dfs1 = 0.;
	double dRto = m_dF1 / (m_nNType + 1);
	if (dRto < 1.25)
		dfs1 = 1.25;
	else if (dRto > 2.50)
		dfs1 = 2.50;
	else
		dfs1 = dRto;
	double dfs2 = 2 * dfs1;
	double dPhi, dPhiL;
	GetPhi(dfs1, dfs2, dPhi);

	double dfsL = 0.;
	if (dRto < 0.5)
		dfsL = 0.5;
	else if (dRto > 1.2)
		dfsL = 1.20;
	else
		dfsL = dRto;

	GetPhiL(dfsL, dPhiL);
	
	if (m_bAutoCal)
		m_dPhi = m_nLoadDir == 0 ? dPhi : dPhiL;
	else
	{
		m_dPhi = 0.25;
	}
	
}