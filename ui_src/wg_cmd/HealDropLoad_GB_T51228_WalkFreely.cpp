// HealDropLoad_GB_T51288_WalkFreely.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "HealDropLoad_GB_T51228_WalkFreely.h"
#include "..\wg_db\wg_db_dbdoc.h"
#include "..\wg_db\wg_db_unitctrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_GB_T51288_WalkFreely dialog


CHealDropLoad_GB_T51288_WalkFreely::CHealDropLoad_GB_T51288_WalkFreely(CWnd* pParent /*=NULL*/)
	: CHeelLoadChildDialog(CHealDropLoad_GB_T51288_WalkFreely::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHealDropLoad_GB_T51288_WalkFreely)
	m_dG = 60;
	m_dFs = 2.0;
	m_iStepNum = 3;
	m_iTotalCrowd = 10;
	m_dTimeStep = 0.01;
	m_iRepeat = 2;
	//}}AFX_DATA_INIT
	m_stWeightUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
}


void CHealDropLoad_GB_T51288_WalkFreely::DoDataExchange(CDataExchange* pDX)
{
	CHeelLoadChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHealDropLoad_GB_T51288_WalkFreely)
	DDX_Control(pDX, IDC_HEELLOAD_WALK_FREELY_G_UNIT, m_stWeightUnit);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_FREELY_G_EDIT, m_dG);
	DDV_MinMaxDouble(pDX, m_dG, 0., 10000.);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_FREELY_FS_EDIT, m_dFs);
	DDV_MinMaxDouble(pDX, m_dFs, 1.25, 2.30);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_FREELY_TIMESTEP_EDIT, m_dTimeStep);

	DDV_MinMaxDouble(pDX, m_dTimeStep, 0., 1000.);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_FREELY_REPEAT_EDIT, m_iRepeat);
	DDV_MinMaxInt(pDX, m_iRepeat, 1, 1000);
	DDX_Text(pDX, IDC_HEELLOAD_WALK_FREELY_STEP_NUM_EDIT, m_iStepNum);
	DDV_MinMaxInt(pDX, m_iStepNum, 1, 5);

	DDX_Text(pDX, IDC_HEELLOAD_WALK_FREELY_CROWN_NUM_EDIT, m_iTotalCrowd);
	DDV_MinMaxInt(pDX, m_iTotalCrowd, 1, 100000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHealDropLoad_GB_T51288_WalkFreely, CHeelLoadChildDialog)
	//{{AFX_MSG_MAP(CHealDropLoad_GB_T51288_WalkFreely)
	// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_GB_T51288_WalkFreely message handlers

void CHealDropLoad_GB_T51288_WalkFreely::End()
{
	//UpdateData(TRUE);
	CHeelLoadChildDialog::End();
}
void CHealDropLoad_GB_T51288_WalkFreely::MakeSpectrumData()
{
	const double Pi = 3.141592654;
	double dAlpha[5] = {0.37*(m_dFs-1.00), 0.10, 0.06, 0.06, 0.06};
	double dPhi[5] = {0, Pi/2, Pi/2, Pi/2, Pi/2};

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
		for (int i=0; i<m_iStepNum; i++)
		{
			Fp += sqrt(m_iTotalCrowd)* dAlpha[i] * m_dG * sin(2*Pi*(i+1)*m_dFs*tn - dPhi[i]);
		}

		m_parPeriod->Add(tn);
		m_parAccel->Add(Fp);
	}


	m_strFuncName=_LS(IDS_WG_CMD__ADD2__Walk_Free_GBT51228_);


}

BOOL CHealDropLoad_GB_T51288_WalkFreely::ISVALID()
{
	return TRUE;
}

BOOL CHealDropLoad_GB_T51288_WalkFreely::OnInitDialog()
{
	CHeelLoadChildDialog::OnInitDialog();
	double dMultFact,dDivFact;
	GetConvertFactor_ForceUnitToCurrent(D_UNITSYS_FORCE_INDEX_KG,dMultFact,dDivFact);
	m_dG = 60;
	m_dG = m_dG*dMultFact/dDivFact;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
