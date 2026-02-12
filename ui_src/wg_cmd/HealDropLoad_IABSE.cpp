// HealDropLoad_IABSE.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "HealDropLoad_IABSE.h"
#include "..\wg_db\wg_db_dbdoc.h"
#include "..\wg_db\wg_db_unitctrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HealDropLoad_IABSE dialog


HealDropLoad_IABSE::HealDropLoad_IABSE(int nLoadType,CWnd* pParent /*=NULL*/)
	: CHeelLoadChildDialog(HealDropLoad_IABSE::IDD, pParent)
{
	m_nLoadType = nLoadType;
	//{{AFX_DATA_INIT(HealDropLoad_IABSE)
	m_dFs = 2.0;
	m_dG = 60.0;
	m_dTimeStep = 0.01;
	m_nRepeat = 1;
	//}}AFX_DATA_INIT

	m_stWeightUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
}


void HealDropLoad_IABSE::DoDataExchange(CDataExchange* pDX)
{
	CHeelLoadChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HealDropLoad_IABSE)
	DDX_Control(pDX, IDC_HEELLOAD_IABSE_G_UNIT, m_stWeightUnit);
	DDX_Text(pDX, IDC_HEELLOAD_IABSE_FS, m_dFs);
	DDX_Text(pDX, IDC_HEELLOAD_IABSE_G, m_dG);
	DDV_MinMaxDouble(pDX, m_dG, 0., 100000.);
	DDX_Text(pDX, IDC_HEELLOAD_IABSE_TIMESTEP, m_dTimeStep);
	DDV_MinMaxDouble(pDX, m_dTimeStep, 0., 10000.);
	DDX_Text(pDX, IDC_HEELLOAD_IABSE_REPEAT, m_nRepeat);
	DDV_MinMaxInt(pDX, m_nRepeat, 1, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HealDropLoad_IABSE, CHeelLoadChildDialog)
	//{{AFX_MSG_MAP(HealDropLoad_IABSE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HealDropLoad_IABSE message handlers


void HealDropLoad_IABSE::End()
{
	UpdateData(TRUE);
	CHeelLoadChildDialog::End();
}
void HealDropLoad_IABSE::MakeSpectrumData()
{
	switch(m_nLoadType)
	{
	case IABSE1STEP:
		MakeIABSE1StepData();
		break;
	case IABSE:
		MakeIABSEContData();
		break;
	}
}

void HealDropLoad_IABSE::MakeIABSE1StepData()
{
	
	double dTime[61] = {0   ,0.01,0.02,0.03,0.04,0.05,0.06,0.07,0.08,0.09,
											0.1 ,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.19,
											0.2 ,0.21,0.22,0.23,0.24,0.25,0.26,0.27,0.28,0.29,
											0.3 ,0.31,0.32,0.33,0.34,0.35,0.36,0.37,0.38,0.39,
											0.4 ,0.41,0.42,0.43,0.44,0.45,0.46,0.47,0.48,0.49,
											0.5 ,0.51,0.52,0.53,0.54,0.55,0.56,0.57,0.58,0.59,
											0.6};
	double dRatio[61]= {0   ,0.2 ,0.42,0.65,0.8 ,0.93,1.03,1.12,1.2 ,1.27,
											1.32,1.34,1.34,1.33,1.3 ,1.26,1.22,1.16,1.1 ,1.01,
											0.94,0.88,0.84,0.78,0.75,0.71,0.67,0.64,0.61,0.59,
											0.56,0.54,0.54,0.53,0.53,0.54,0.55,0.57,0.6 ,0.64,
											0.66,0.70,0.74,0.78,0.83,0.88,0.93,0.97,1   ,1.01,
											0.99,0.90,0.77,0.67,0.58,0.46,0.35,0.26,0.17,0.08,
											0};

	
	double st= 0.0;
	double ed= 0.6*2.0/m_dFs; // 1 Cycle
	int nStep = (ed- st)/m_dTimeStep;
	const double Pi = 3.141592654;

	for(int i=0; i<61; i++) // Time Scaling
		dTime[i] *= 2.0/m_dFs;
		
	double tn;
	int n=0;
	double Fp;
	double ratio;

		
	n=0;
	int nTime = 0;
	for (int t=0;t<=nStep; t++)
	{
		tn = st + m_dTimeStep*t;
		if(tn > dTime[60]) break;
		while(tn>=dTime[nTime])
			nTime++;
		if(nTime >60) nTime = 60;
		
		ratio = dRatio[nTime-1] + (dRatio[nTime]-dRatio[nTime-1])*(tn-dTime[nTime-1])/(dTime[nTime]-dTime[nTime-1]);
		
		Fp = m_dG*ratio;
		
	  m_parPeriod->Add(tn);
		m_parAccel->Add(Fp);
	}


	m_strFuncName=_LS(IDS_WG_CMD__ADD2__Walk_1_step_Baumann_);
}


void HealDropLoad_IABSE::MakeIABSEContData()
{
	double st= 0.0;
	double ed= 1.0/m_dFs; // 1 Cycle
	int nStep = (ed- st)/m_dTimeStep;
	const double Pi = 3.141592654;
		
	double tn=0;
	int n=0;
	double dG1,dG2,dG3;
	double Fp;

	dG1 = m_dG*(0.4 + 0.1*(m_dFs-2.0)/(2.4-2.0));
	dG2 = m_dG*0.1;
	dG3 = m_dG*0.1;
	
	if(m_nRepeat<1) m_nRepeat = 1;
	n=0;
	
	for (int t=0;t<=nStep*m_nRepeat; t++)
	{
		tn = st + m_dTimeStep*t;
		
		Fp = m_dG + dG1*sin(2*Pi*m_dFs*tn) + dG2*sin(4*Pi*m_dFs*tn-Pi/2.0) + dG3*sin(6*Pi*m_dFs*tn-Pi/2.0);
		
	  m_parPeriod->Add(tn);
		m_parAccel->Add(Fp);
	}


	m_strFuncName=_LS(IDS_WG_CMD__ADD2__Walk_cont_IABSE_);
}

BOOL HealDropLoad_IABSE::ISVALID()
{

	return TRUE;
}

BOOL HealDropLoad_IABSE::OnInitDialog() 
{
	CHeelLoadChildDialog::OnInitDialog();
	
	double dMultFact,dDivFact;
	GetConvertFactor_ForceUnitToCurrent(D_UNITSYS_FORCE_INDEX_KG,dMultFact,dDivFact);
	m_dG = 60;
	m_dG = m_dG*dMultFact/dDivFact;

	if(m_nLoadType == IABSE1STEP)
	{
		GetDlgItem(IDC_HEELLOAD_IABSE_REPEAT)->EnableWindow(FALSE);
		m_dTimeStep = 0.005;
	}
	else
	{
		m_nRepeat = 2;
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
