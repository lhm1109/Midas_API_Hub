// HealDropLoad_JGJ_T441_2019_WalkRhythmMovement.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "HealDropLoad_JGJ_T441_2019_WalkRhythmMovement.h"
#include "..\wg_db\wg_db_dbdoc.h"
#include "..\wg_db\wg_db_unitctrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement dialog


CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement(CWnd* pParent /*=NULL*/)
	: CHeelLoadChildDialog(CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement)
	m_dQp = 1.5 * 1.0e-4;
	m_dF1 = 1.0;
	m_nNType = 0;
	m_nSportType = 0;
	m_nOrderNum = 0;
	m_iRepeat = 20;
	//}}AFX_DATA_INIT
	m_stQpUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
}


void CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::DoDataExchange(CDataExchange* pDX)
{
	CHeelLoadChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement)
	DDX_Control(pDX, IDC_HEELLOAD_WALK_MOVEMENT_QP_UNIT, m_stQpUnit);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_MOVEMENT_QP_EDIT, m_dQp);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_MOVEMENT_F1_EDIT, m_dF1);
	DDX_Control(pDX, IDC_HEELLOAD_WALK_MOVEMENT_N_COMBO, m_cmbNType);
	DDX_Control(pDX, IDC_HEELLOAD_WALK_MOVEMENT_SPORT_TYPE_COMBO, m_cmbSportType);
	DDX_Control(pDX, IDC_HEELLOAD_WALK_MOVEMENT_ORDER_NUM_COMBO, m_cmbOrderNum);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_MOVEMENT_REPEAT_EDIT, m_iRepeat);
	DDV_MinMaxInt(pDX, m_iRepeat, 1, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement, CHeelLoadChildDialog)
	//{{AFX_MSG_MAP(CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement)
	// NOTE: the ClassWizard will add message map macros here
	ON_CBN_SELCHANGE(IDC_HEELLOAD_WALK_MOVEMENT_N_COMBO, OnSelchangeNTypeCmb)
	ON_CBN_SELCHANGE(IDC_HEELLOAD_WALK_MOVEMENT_SPORT_TYPE_COMBO, OnSelchangeSportTypeCmb)
	ON_CBN_SELCHANGE(IDC_HEELLOAD_WALK_MOVEMENT_ORDER_NUM_COMBO, OnSelchangeCoordTypeCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement message handlers

void CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::End()
{
	//UpdateData(TRUE);
	CHeelLoadChildDialog::End();
}

void CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::MakeSpectrumData()
{
	double dGamma =0.0;
	GetGamma(dGamma);

	double tn;
	double st= 0.005;
	double ed= 1.0 / m_dF1;
	int nStep = (ed- st)/0.005;
	double dRto = m_dF1 / (m_nNType + 1);
	double dF1_ = 0.0;
	if (m_iRepeat < 1) m_iRepeat = 1;

	if (m_nSportType == 3)
	{
		if (dRto < 2.0)
			dF1_ = 2.0;
		else if (dRto >= 2.0 && dRto <= 2.75)
			dF1_ = dRto;
		else
			dF1_ = 2.75;
	}
	else
	{
		if (dRto < 1.5)
			dF1_ = 1.5;
		else if (dRto >= 1.5 && dRto <= 3.0)
			dF1_ = dRto;
		else
			dF1_ = 3.0;
	}
	
	double dPt = 0.;
	for (int t=0; t<nStep * m_iRepeat; t++)
	{
		tn = st + t*0.005;
		dPt = dGamma * m_dQp * cos(2 * M_PI * dF1_ * (m_nOrderNum+1) * tn);
		m_parPeriod->Add(tn);		
		m_parAccel->Add(dPt);
	}

	m_strFuncName=_LS(IDS_WG_CMD__ADD2__Walk_Rhythm_Movement_JGJT441_);

}

void CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::GetGamma(double& dGamma)
{
	if (m_nSportType == 0)
	{
		dGamma =0.5;
	}
	else if (m_nSportType == 1)
	{
		switch (m_nOrderNum)
		{
		case 0: dGamma = 0.25; break;
		case 1: dGamma = 0.05; break;
		default:
			break;
		}
	}
	else if (m_nSportType == 2)
	{
		switch (m_nOrderNum)
		{
		case 0: dGamma = 0.4; break;
		case 1: dGamma = 0.15; break;
		default:
			break;
		}
	}
	else
	{
		switch (m_nOrderNum)
		{
		case 0: dGamma = 1.5; break;
		case 1: dGamma = 0.6; break;
		case 2: dGamma = 0.1; break;
		default:
			break;
		}
	}
}

BOOL CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::ISVALID()
{
	return TRUE;
}

void CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::OnSelchangeNTypeCmb()
{
	m_nNType = m_cmbNType.GetCurSel();
	m_cmbNType.SetCurSel(m_nNType);
}
void CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::OnSelchangeSportTypeCmb()
{
	m_nSportType = m_cmbSportType.GetCurSel();
	InitOrderNumComb();
	m_cmbSportType.SetCurSel(m_nSportType);
}

void CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::OnSelchangeCoordTypeCmb()
{
	m_nOrderNum = m_cmbOrderNum.GetCurSel();
	m_cmbOrderNum.SetCurSel(m_nOrderNum);
}

BOOL CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::OnInitDialog()
{
	CHeelLoadChildDialog::OnInitDialog();

	double dMultFact,dDivFact;
	GetConvertFactor_StressUnitToCurrent(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_FACTOR_M,dMultFact,dDivFact);
	m_dQp = 1.5 * 1.0e-4;
	m_dQp = m_dQp*dMultFact/dDivFact;

	m_cmbNType.ResetContent();
	CString strNType[] = { _T("1"),_T("2"), _T("3") };
	for (int i = 0;i < 3; i++)
	{
		m_cmbNType.AddString(strNType[i]);
	}
	m_cmbNType.SetCurSel(0);

	m_cmbSportType.ResetContent();
	CString strSportType[]={ _LS(IDS_WG_CMD__ADDD__Walking_Rhythm_Movement_Dance),_LS(IDS_WG_CMD__ADDD__Walking_Rhythm_Movement_Fixed_Seat), 
		_LS(IDS_WG_CMD__ADDD__Walking_Rhythm_Movement_Unfixed_Seat), _LS(IDS_WG_CMD__ADDD__Walking_Rhythm_Movement_Sports)};
	for(int i=0;i <4 ; i++)
	{
		m_cmbSportType.AddString(strSportType[i]);
	}
	m_cmbSportType.SetCurSel(0);

	InitOrderNumComb();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement::InitOrderNumComb()
{
	m_cmbOrderNum.ResetContent();
	CString strOrderNum[] = { _T("1"),_T("2"), _T("3") };

	if (m_nSportType == 0)
	{
		m_cmbOrderNum.AddString(strOrderNum[0]);
	} 
	else if (m_nSportType == 3)
	{
		for (int i = 0;i < 3; i++)
		{
			m_cmbOrderNum.AddString(strOrderNum[i]);
		}
	}
	else
	{
		for (int i = 0;i < 2; i++)
		{
			m_cmbOrderNum.AddString(strOrderNum[i]);
		}
	}

	m_cmbOrderNum.SetCurSel(0);
}
