// HealDropLoad_ALLENRAINER.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "HealDropLoad_ALLENRAINER.h"
#include "..\wg_db\wg_db_dbdoc.h"
#include "..\wg_db\wg_db_unitctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_ALLENRAINER dialog


CHealDropLoad_ALLENRAINER::CHealDropLoad_ALLENRAINER(CWnd* pParent /*=NULL*/)
	: CHeelLoadChildDialog(CHealDropLoad_ALLENRAINER::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHealDropLoad_ALLENRAINER)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHealDropLoad_ALLENRAINER::DoDataExchange(CDataExchange* pDX)
{
	CHeelLoadChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHealDropLoad_ALLENRAINER)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHealDropLoad_ALLENRAINER, CHeelLoadChildDialog)
	//{{AFX_MSG_MAP(CHealDropLoad_ALLENRAINER)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_ALLENRAINER message handlers

void CHealDropLoad_ALLENRAINER::End()
{
	UpdateData(TRUE);
	CHeelLoadChildDialog::End();
}
void CHealDropLoad_ALLENRAINER::MakeSpectrumData()
{

	const double dForce[] = {0,	45,	68,	110,	150,
												 175,	200,	220,	242,	270,	
												 280,	288,	290,	288,	282,	
												 273,	262,	250,	235,	210,	
												 190,	175,	163,	150,	140,	
												 132,	126,	118,	110,	104,	
													100,	95,	90,	87,	84,	
													80,	77,	71,	66,	64,	
													58,	56,	54,	48,	46,	
													40,	35,	25,	20,	10,	0}; //51

	double tn;
	double dMultFact,dDivFact;
	GetConvertFactor_ForceUnitToCurrent(D_UNITSYS_FORCE_INDEX_KG,dMultFact,dDivFact);
	
	for (int t=0;t<=50; t++)
	{
		tn = 0.001*t;
		
		m_parPeriod->Add(tn);
		m_parAccel->Add(dForce[t]*dMultFact/dDivFact);

	}


	m_strFuncName=_LS(IDS_WG_CMD__ADD2__Run_Allen_Rainer_);

}
BOOL CHealDropLoad_ALLENRAINER::ISVALID()
{

	return TRUE;
}
