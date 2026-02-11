// HealDropLoad_AIJRC_walk.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "HealDropLoad_AIJRC_walk.h"
#include "..\wg_db\wg_db_dbdoc.h"
#include "..\wg_db\wg_db_unitctrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HealDropLoad_AIJRC_walk dialog


HealDropLoad_AIJRC_walk::HealDropLoad_AIJRC_walk(CWnd* pParent /*=NULL*/)
	: CHeelLoadChildDialog(HealDropLoad_AIJRC_walk::IDD, pParent)
{
	//{{AFX_DATA_INIT(HealDropLoad_AIJRC_walk)
	m_dFs = 2.0;
	m_dTimeStep = 0.005;
	//}}AFX_DATA_INIT
}


void HealDropLoad_AIJRC_walk::DoDataExchange(CDataExchange* pDX)
{
	CHeelLoadChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HealDropLoad_AIJRC_walk)
	DDX_Text(pDX, IDC_HEELLOAD_AIJRC_WALK_FS, m_dFs);
	DDV_MinMaxDouble(pDX, m_dFs, 0., 100.);
	DDX_Text(pDX, IDC_HEELLOAD_AIJRC_WALK_TIMESTEP, m_dTimeStep);
	DDV_MinMaxDouble(pDX, m_dTimeStep, 0., 1000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HealDropLoad_AIJRC_walk, CHeelLoadChildDialog)
	//{{AFX_MSG_MAP(HealDropLoad_AIJRC_walk)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HealDropLoad_AIJRC_walk message handlers

void HealDropLoad_AIJRC_walk::End()
{
	UpdateData(TRUE);
	CHeelLoadChildDialog::End();
}
void HealDropLoad_AIJRC_walk::MakeSpectrumData()
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
			Fp = 11.78*sin(78.54*tn);
		else
			Fp = 0.0;
		
	  m_parPeriod->Add(tn);
		m_parAccel->Add(Fp*dMultFact/dDivFact);
	}


	m_strFuncName=_LS(IDS_WG_CMD__ADD2__Walk_discont_AIJ_RC_);

}
BOOL HealDropLoad_AIJRC_walk::ISVALID()
{

	return TRUE;
}
