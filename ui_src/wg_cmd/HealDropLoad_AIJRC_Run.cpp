// HealDropLoad_AIJRC_Run.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "HealDropLoad_AIJRC_Run.h"
#include "..\wg_db\wg_db_dbdoc.h"
#include "..\wg_db\wg_db_unitctrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_AIJRC_Run dialog


CHealDropLoad_AIJRC_Run::CHealDropLoad_AIJRC_Run(CWnd* pParent /*=NULL*/)
	: CHeelLoadChildDialog(CHealDropLoad_AIJRC_Run::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHealDropLoad_AIJRC_Run)
	m_dFs = 2.0;
	m_dTimeStep = 0.005;
	//}}AFX_DATA_INIT
}


void CHealDropLoad_AIJRC_Run::DoDataExchange(CDataExchange* pDX)
{
	CHeelLoadChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHealDropLoad_AIJRC_Run)
	DDX_Text(pDX, IDC_HEELLOAD_AIJRC_RUN_FS, m_dFs);
	DDV_MinMaxDouble(pDX, m_dFs, 0., 1000.);
	DDX_Text(pDX, IDC_HEELLOAD_AIJRC_RUN_TIMESTEP, m_dTimeStep);
	DDV_MinMaxDouble(pDX, m_dTimeStep, 0., 1000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHealDropLoad_AIJRC_Run, CHeelLoadChildDialog)
	//{{AFX_MSG_MAP(CHealDropLoad_AIJRC_Run)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_AIJRC_Run message handlers

void CHealDropLoad_AIJRC_Run::End()
{
	UpdateData(TRUE);
	CHeelLoadChildDialog::End();
}
void CHealDropLoad_AIJRC_Run::MakeSpectrumData()
{
	double st= 0.0;
	double ed= 1.0/m_dFs; // 1 Cycle
	int nStep = (ed- st)/m_dTimeStep;
	const double Pi = 3.141592654;
		
	double tn;
	int n=0;
	
	double Fp;

	double dMultFact,dDivFact;
	GetConvertFactor_ForceUnitToCurrent(D_UNITSYS_FORCE_INDEX_KG,dMultFact,dDivFact);
	for (int t=0;t<=nStep; t++)
	{
		tn = st + m_dTimeStep*t;
		
		if(tn<=0.04)
			Fp = 70.69*sin(78.54*tn);
		else
			Fp = 0.0;
		
	  m_parPeriod->Add(tn);
		m_parAccel->Add(Fp*dMultFact/dDivFact);
	}


	m_strFuncName=_LS(IDS_WG_CMD__ADD2__Run_AIJ_RC_);


}

BOOL CHealDropLoad_AIJRC_Run::ISVALID()
{

	return TRUE;
}
