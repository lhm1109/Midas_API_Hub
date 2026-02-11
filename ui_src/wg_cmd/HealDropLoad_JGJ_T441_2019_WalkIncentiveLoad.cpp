// HealDropLoad_JGJ_T441_2019_WalkIncentiveLoad.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "HealDropLoad_JGJ_T441_2019_WalkIncentiveLoad.h"
#include "..\wg_db\wg_db_dbdoc.h"
#include "..\wg_db\wg_db_unitctrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad dialog


CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad::CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad(CWnd* pParent /*=NULL*/)
	: CHeelLoadChildDialog(CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad)
	m_dPp = 0.70;
	m_dF1 = 1.0;
	m_iRepeat = 20;
	//}}AFX_DATA_INIT
	m_stWeightUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_nNType = 0;
}


void CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad::DoDataExchange(CDataExchange* pDX)
{
	CHeelLoadChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad)
	DDX_Control(pDX, IDC_HEELLOAD_WALK_INCENTIVE_PP_UNIT, m_stWeightUnit);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_INCENTIVE_PP_EDIT, m_dPp);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_INCENTIVE_F1_EDIT, m_dF1);
	DDX_Control(pDX, IDC_HEELLOAD_WALK_INCENTIVE_N_COMBO, m_cmbNType);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_INCENTIVE_REPEAT_EDIT, m_iRepeat);
	DDV_MinMaxInt(pDX, m_iRepeat, 1, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad, CHeelLoadChildDialog)
	//{{AFX_MSG_MAP(CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad)
	// NOTE: the ClassWizard will add message map macros here
	ON_CBN_SELCHANGE(IDC_HEELLOAD_WALK_INCENTIVE_N_COMBO, OnSelchangeParaNCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad message handlers

void CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad::End()
{
	//UpdateData(TRUE);
	CHeelLoadChildDialog::End();
}

void CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad::MakeSpectrumData()
{
	const double Pi = 3.141592654;
	if (m_iRepeat < 1) m_iRepeat = 1;

	double dGamma =0.0;
	double dPhi =0.0;

	double tn;
	double st= 0.005;
	double ed= 1.0/m_dF1; 
	double dTimeStep = 0.005;
	int nStep = (ed- st)/ dTimeStep;
	double dRto = m_dF1 / (m_nNType + 1);
	double dF1_ = 0.;
	if (dRto < 1.6)
		dF1_ = 1.6;
	else if (dRto >= 1.6 && dRto <= 2.2)
		dF1_ = dRto;
	else
		dF1_ = 2.2;
	
	double Ft;
	for (int t=0; t<nStep * m_iRepeat; t++)
	{
		tn = st + dTimeStep*t;
		Ft = 0.;
		for (int i = 0; i < 3; i++)
		{
			GetGamma(i, dGamma);
			GetPhi(i, dPhi);
			Ft += dGamma * m_dPp * cos(2.0* Pi * (i+1) * dF1_ * tn + dPhi);
		}
		
		m_parPeriod->Add(tn);
		m_parAccel->Add(Ft);
	}
	m_strFuncName=_LS(IDS_WG_CMD__ADD2__Walk_Incentive_Load_JGJT441_);
}

void CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad::GetGamma(int nIdx, double &dGamma)
{
	if (nIdx == 0)
	{
		dGamma =0.5;	
	}
	else if (nIdx == 1)
	{
		dGamma =0.2;
	}
	else if (nIdx == 2)
	{
		dGamma =0.1;
	}
}

void CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad::GetPhi(int nIdx, double &dPhi)
{
	if (nIdx == 0)
	{
		dPhi = 0.0;
	}
	else
	{
		dPhi = M_PI/2.0;
	}
}

void CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad::OnSelchangeParaNCmb()
{
	m_nNType = m_cmbNType.GetCurSel();
	m_cmbNType.SetCurSel(m_nNType);
}

BOOL CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad::OnInitDialog()
{
	CHeelLoadChildDialog::OnInitDialog();

	double dMultFact,dDivFact;
	GetConvertFactor_ForceUnitToCurrent(D_UNITSYS_FORCE_INDEX_KN,dMultFact,dDivFact);
	m_dPp = 0.7;
	m_dPp = m_dPp *dMultFact/dDivFact;

	m_cmbNType.ResetContent();
	CString strNType[]={ _T("1"),_T("2"), _T("3") };
	for(int i=0;i <3 ; i++)
	{
		m_cmbNType.AddString(strNType[i]);
	}
	m_cmbNType.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad::ISVALID()
{

	return TRUE;
}
