// HealDropLoad_GB_T51288_WalkRhythm.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "HealDropLoad_GB_T51228_WalkRhythm.h"
#include "..\wg_db\wg_db_dbdoc.h"
#include "..\wg_db\wg_db_unitctrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_GB_T51288_WalkRhythm dialog


CHealDropLoad_GB_T51288_WalkRhythm::CHealDropLoad_GB_T51288_WalkRhythm(CWnd* pParent /*=NULL*/)
	: CHeelLoadChildDialog(CHealDropLoad_GB_T51288_WalkRhythm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHealDropLoad_GB_T51288_WalkRhythm)
	m_dG = 60;
	m_dFs = 2.0;
	m_iTimeStep = 3;
	m_iTotalCrowd = 10;
	m_dTimeStep = 0.01;
	m_iRepeat = 2;
	//}}AFX_DATA_INIT
	m_stWeightUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_nSportType = 0;
	m_nCoordType = 0;
}


void CHealDropLoad_GB_T51288_WalkRhythm::DoDataExchange(CDataExchange* pDX)
{
	CHeelLoadChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHealDropLoad_GB_T51288_WalkRhythm)
	DDX_Control(pDX, IDC_HEELLOAD_WALK_RHYTHM_G_UNIT, m_stWeightUnit);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_RHYTHM_G_EDIT, m_dG);
	DDV_MinMaxDouble(pDX, m_dG, 0., 10000.);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_RHYTHM_FS_EDIT, m_dFs);
	DDV_MinMaxDouble(pDX, m_dFs, 1.5, 3.5);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_RHYTHM_TIMESTEP_EDIT, m_dTimeStep);
	DDV_MinMaxDouble(pDX, m_dTimeStep, 0., 1000.);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_RHYTHM_REPEAT_EDIT, m_iRepeat);
	DDV_MinMaxInt(pDX, m_iRepeat, 1, 1000);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_RHYTHM_STEP_NUM_EDIT, m_iTimeStep);
	DDV_MinMaxInt(pDX, m_iTimeStep, 1, 3);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_RHYTHM_CROWN_NUM_EDIT, m_iTotalCrowd);
	DDV_MinMaxInt(pDX, m_iTotalCrowd, 1, 100000);
	DDX_Control(pDX, IDC_HEELLOAD_WALK_RHYTHM_SPORT_TYPE_COMBO, m_cmbSportType);
	DDX_Control(pDX, IDC_HEELLOAD_WALK_RHYTHM_SPORT_COORDINATION_COMBO, m_cmbCoordType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHealDropLoad_GB_T51288_WalkRhythm, CHeelLoadChildDialog)
	//{{AFX_MSG_MAP(CHealDropLoad_GB_T51288_WalkRhythm)
	// NOTE: the ClassWizard will add message map macros here
	ON_CBN_SELCHANGE(IDC_HEELLOAD_WALK_RHYTHM_SPORT_TYPE_COMBO, OnSelchangeSportTypeCmb)
	ON_CBN_SELCHANGE(IDC_HEELLOAD_WALK_RHYTHM_SPORT_COORDINATION_COMBO, OnSelchangeCoordTypeCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_GB_T51288_WalkRhythm message handlers

void CHealDropLoad_GB_T51288_WalkRhythm::End()
{
	//UpdateData(TRUE);
	CHeelLoadChildDialog::End();
}

void CHealDropLoad_GB_T51288_WalkRhythm::MakeSpectrumData()
{
	const double Pi = 3.141592654;

	double dAlpha1 =0.0;
	double dAlpha2 = 0.0;
	double dAlpha3 = 0.0;
	GetAlhpa(dAlpha1, dAlpha2, dAlpha3);
	double dAlpha[3] = {dAlpha1, dAlpha2, dAlpha3};

	double dCn1 =0.0;
	double dCn2 = 0.0;
	double dCn3 = 0.0;
	GetCn(dCn1,dCn2,dCn3);
	double Cn[3] = {dCn1, dCn2, dCn3};

	double dPhi1 =0.0;
	double dPhi2 = 0.0;
	double dPhi3 = 0.0;
	GetPhi(Pi, dPhi1,dPhi2,dPhi3);
	double dPhi[3] = {dPhi1, dPhi2, dPhi3};

	double tn;
	double st= 0.0;
	double ed= 1.0/m_dFs; // 1 Cycle
	int nStep = (ed- st)/m_dTimeStep;
	if(m_iRepeat<1) m_iRepeat = 1;

	double Fp;

	for (int t=0; t<nStep*m_iRepeat; t++)
	{
		tn = st + m_dTimeStep*t;
		Fp = 0.0;
		for (int i=0; i<m_iTimeStep; i++)
		{
			Fp += dAlpha[i] * m_dG * sin(2*Pi*(i+1)*m_dFs*tn - dPhi[i]) * m_iTotalCrowd * Cn[i];
		}

		m_parPeriod->Add(tn);
		m_parAccel->Add(Fp);
	}


	m_strFuncName=_LS(IDS_WG_CMD__ADD2__Walk_Rhythm_GBT51228_);


}

void CHealDropLoad_GB_T51288_WalkRhythm::GetAlhpa(double &dAlpha1, double &dAlpha2, double &dAlpha3)
{
	if (m_nSportType == 0)
	{
		dAlpha1=0.5;	
		dAlpha2=0.25;	
		dAlpha3=0.15;	
	}
	else if (m_nSportType == 1)
	{
		dAlpha1=2.1-0.15*m_dFs;	
		dAlpha2=1.9-0.17*2*m_dFs;	
	}
	else if (m_nSportType == 2)
	{
		dAlpha1=2.1-0.15*m_dFs;	
		dAlpha2=1.9-0.17*2*m_dFs;	
		dAlpha3=1.25-0.11*3*m_dFs;
	}
}

void CHealDropLoad_GB_T51288_WalkRhythm::GetCn(double &dCn1, double &dCn2, double &dCn3)
{
	if (m_nSportType != 2)
	{
		dCn1=1.0;	
		dCn2=1.0;	
		dCn3=1.0;	
	}
	else
	{
		if (m_iTotalCrowd <= 5)
		{
			dCn1=1.0;	
			dCn2=1.0;	
			dCn3=1.0;	
		}
		else if (m_iTotalCrowd >= 50)
		{
			if (m_nCoordType == 0)
			{
				dCn1=0.8;	
				dCn2=0.67;	
				dCn3=0.5;	
			}
			else if (m_nCoordType == 1)
			{
				dCn1=0.67;	
				dCn2=0.5;	
				dCn3=0.4;	
			}
			else if (m_nCoordType == 2)
			{
				dCn1=0.5;
				dCn2=0.4;
				dCn3=0.3;
			}
		}
		else
		{
			if (m_nCoordType == 0)
			{
				dCn1= 1.0-(1.0-0.8)/(50-5)*(m_iTotalCrowd-5);	
				dCn2= 1.0-(1.0-0.67)/(50-5)*(m_iTotalCrowd-5);	
				dCn3= 1.0-(1.0-0.5)/(50-5)*(m_iTotalCrowd-5);	
			}
			else if (m_nCoordType == 1)
			{
				dCn1= 1.0-(1.0-0.67)/(50-5)*(m_iTotalCrowd-5);	
				dCn2= 1.0-(1.0-0.5)/(50-5)*(m_iTotalCrowd-5);	
				dCn3= 1.0-(1.0-0.4)/(50-5)*(m_iTotalCrowd-5);	
			}
			else if (m_nCoordType == 2)
			{
				dCn1= 1.0-(1.0-0.5)/(50-5)*(m_iTotalCrowd-5);
				dCn2= 1.0-(1.0-0.4)/(50-5)*(m_iTotalCrowd-5);
				dCn3= 1.0-(1.0-0.3)/(50-5)*(m_iTotalCrowd-5);
			}
		}
	}
	
}

void CHealDropLoad_GB_T51288_WalkRhythm::GetPhi(double Pi, double &dPhi1,  double &dPhi2, double &dPhi3)
{
	if (m_nSportType == 0)
	{
		dPhi1=0.0;	
		dPhi2=Pi/2;
		dPhi3=Pi/2;
	}
	else
	{
		dPhi1=0.0;	
		dPhi2=0.0;
		dPhi3=0.0;
	}
}

BOOL CHealDropLoad_GB_T51288_WalkRhythm::ISVALID()
{
	if (!CheckFsData())
		return FALSE;
	return TRUE;
}

BOOL CHealDropLoad_GB_T51288_WalkRhythm::CheckFsData()
{
	int nIndex = m_cmbSportType.GetCurSel();
	CString strFs = _T("");
	GetDlgItem(IDC_HEELLOAD_WALK_RHYTHM_FS_EDIT)->GetWindowText(strFs);
	m_dFs = _tstof(strFs);
	CString strError = _T("");
	if (nIndex == 0)
	{
		if (m_dFs<1.5 || m_dFs > 3.0)
		{

			strError.Format(_LS(IDS_WG_CMD__ADDD__Walking_Crowd_Freely_FsRange),1.5, 3.0);
			AfxMessageBox(strError, MB_OK);
			return FALSE;
		}	
	}	
	else
	{
		if (m_dFs<1.5 || m_dFs > 3.5)
		{

			strError.Format(_LS(IDS_WG_CMD__ADDD__Walking_Crowd_Freely_FsRange),1.5, 3.5);
			AfxMessageBox(strError, MB_OK);
			return FALSE;
		}	
	}
	return TRUE;
}

void CHealDropLoad_GB_T51288_WalkRhythm::OnSelchangeSportTypeCmb()
{
	m_nSportType = m_cmbSportType.GetCurSel();
	CString strStep = _T("");
	if (m_nSportType == 1)
		strStep.Format(_T("%d"), 2);
	else
		strStep.Format(_T("%d"), 3);
	GetDlgItem(IDC_HEELLOAD_WALK_RHYTHM_STEP_NUM_EDIT)->SetWindowText(strStep);

	m_cmbSportType.SetCurSel(m_nSportType);
}

void CHealDropLoad_GB_T51288_WalkRhythm::OnSelchangeCoordTypeCmb()
{
	m_nCoordType = m_cmbCoordType.GetCurSel();
	m_cmbCoordType.SetCurSel(m_nCoordType);
}

BOOL CHealDropLoad_GB_T51288_WalkRhythm::OnInitDialog()
{
	CHeelLoadChildDialog::OnInitDialog();

	double dMultFact,dDivFact;
	GetConvertFactor_ForceUnitToCurrent(D_UNITSYS_FORCE_INDEX_KG,dMultFact,dDivFact);
	m_dG = 60;
	m_dG = m_dG*dMultFact/dDivFact;

	m_cmbSportType.ResetContent();
	CString strSportType[]={ _LS(IDS_WG_CMD__ADDD__Walking_Crowd_Rhythm_Sport_Competition),_LS(IDS_WG_CMD__ADDD__Walking_Crowd_Rhythm_Fixed_Seat), _LS(IDS_WG_CMD__ADDD__Walking_Crowd_Rhythm_UnFixed_Seat)};
	for(int i=0;i <3 ; i++)
	{
		m_cmbSportType.AddString(strSportType[i]);
	}
	m_cmbSportType.SetCurSel(0);

	m_cmbCoordType.ResetContent();
	CString strCoordType[]={ _LS(IDS_WG_CMD__ADDD__Walking_Crowd_Rhythm_High),_LS(IDS_WG_CMD__ADDD__Walking_Crowd_Rhythm_Middle), _LS(IDS_WG_CMD__ADDD__Walking_Crowd_Rhythm_Low)};
	for(int i=0;i <3 ; i++)
	{
		m_cmbCoordType.AddString(strCoordType[i]);
	}
	m_cmbCoordType.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
