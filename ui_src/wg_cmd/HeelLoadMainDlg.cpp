// HeelLoadMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "HeelLoadMainDlg.h"
#include "HealDropLoad_IABSE.h"
#include "HealDropLoad_AIJRC_walk.h" 
#include "HealDropLoad_AIJRC_Run.h" 
#include "HealDropLoad_ALLENRAINER.h"
#include "HealDropLoad_GB_T51228_WalkFreely.h"
#include "HealDropLoad_GB_T51228_WalkRhythm.h"
#include "HealDropLoad_JGJ_T441_2019_WalkIncentiveLoad.h"
#include "HealDropLoad_JGJ_T441_2019_WalkRhythmMovement.h"
#include "HealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CDialogMove

/////////////////////////////////////////////////////////////////////////////
// CHeelLoadMainDlg dialog


CHeelLoadMainDlg::CHeelLoadMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHeelLoadMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHeelLoadMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CHeelLoadMainDlg::~CHeelLoadMainDlg()
{
	// Close message를 대비해서...

 	CHeelLoadChildDialog* dlg;
	int key;

	if(m_mapChildDlg.Lookup(m_nCurrentDlg,dlg))
		if( dlg->IsCreated() )
			dlg->End();

	POSITION pos = m_mapChildDlg.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapChildDlg.GetNextAssoc(pos,key,dlg);
		delete dlg;
	}
	m_mapChildDlg.RemoveAll();
}


void CHeelLoadMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHeelLoadMainDlg)
	DDX_Control(pDX, IDC_ETC_GDS_SPECTRUM_CMB, m_cmbDesignCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeelLoadMainDlg, CDialog)
	//{{AFX_MSG_MAP(CHeelLoadMainDlg)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_SPECTRUM_CMB, OnSelchangeEtcGdsSpectrumCmb)
	ON_CBN_CLOSEUP(IDC_ETC_GDS_SPECTRUM_CMB, OnCloseupEtcGdsSpectrumCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeelLoadMainDlg message handlers

BOOL CHeelLoadMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ASSERT(m_parAccel);
	////////////////////////////////////////////////////
	// Init Combobox
	CString sDesignCode[]={ _LS(IDS_WG_CMD__ADDD__Walking_1_step_Baumann_),_LS(IDS_WG_CMD__ADDD__Walking_continuous_IABSE_), _LS(IDS_WG_CMD__ADDD__Walking_discont__AIJ_),
		_LS(IDS_WG_CMD__ADD2__Running_AIJ_),_LS(IDS_WG_CMD__ADD2__Running__Allen___Rainer_), _LS(IDS_WG_CMD__ADDD__Walking_Crowd_Freely), _LS(IDS_WG_CMD__ADDD__Walking_Crowd_Rhythm__),
		_LS(IDS_WG_CMD__ADD2__Walk_Incentive_Load_JGJT441_), _LS(IDS_WG_CMD__ADD2__Walk_Rhythm_Movement_JGJT441_), _LS(IDS_WG_CMD__ADD2__Corridors_Indoor_Bridegs_JGJT441_)};
	int     nDesignCode[]= { IABSE1STEP,IABSE,AIJRC_WALK,AIJRC_RUN,ALLEN_RAINER, GB_T51288_FREE, GB_T51288_RHYT, JGJ_T441_WALK, JGJ_T441_RHYT, JGJ_T441_BRIDGE };

	int index;
	for(int i=0;i <5 ; i++)
	{
		index = m_cmbDesignCode.AddString(sDesignCode[i]);
		m_cmbDesignCode.SetItemData(index,nDesignCode[i]);
	}

#ifdef _MGEN_CH
	for(int i=5;i <10 ; i++)
	{
		index = m_cmbDesignCode.AddString(sDesignCode[i]);
		m_cmbDesignCode.SetItemData(index,nDesignCode[i]);
	}
#endif

	m_cmbDesignCode.SetCurSel(0);
	m_nCurrentDlg = m_cmbDesignCode.GetItemData(0);

	/////////////////////////////////////////////////////
	
	m_mapChildDlg.SetAt(nDesignCode[0], new HealDropLoad_IABSE(IABSE1STEP));
	m_mapChildDlg.SetAt(nDesignCode[1], new HealDropLoad_IABSE(IABSE));
	m_mapChildDlg.SetAt(nDesignCode[2], new HealDropLoad_AIJRC_walk()); 
	m_mapChildDlg.SetAt(nDesignCode[3], new CHealDropLoad_AIJRC_Run()); 
	m_mapChildDlg.SetAt(nDesignCode[4], new CHealDropLoad_ALLENRAINER()); 
	m_mapChildDlg.SetAt(nDesignCode[5], new CHealDropLoad_GB_T51288_WalkFreely()); 
	m_mapChildDlg.SetAt(nDesignCode[6], new CHealDropLoad_GB_T51288_WalkRhythm()); 
	m_mapChildDlg.SetAt(nDesignCode[7], new CHealDropLoad_JGJ_T441_2019_WalkIncentiveLoad());
	m_mapChildDlg.SetAt(nDesignCode[8], new CHealDropLoad_JGJ_T441_2019_WalkRhythmMovement());
	m_mapChildDlg.SetAt(nDesignCode[9], new CHealDropLoad_JGJ_T441_2019_CorridorsAndIndoorBridegs());
	

	POSITION pos = m_mapChildDlg.GetStartPosition();
	CHeelLoadChildDialog* dlg;
	int key;

	while(pos!=NULL)
	{
		m_mapChildDlg.GetNextAssoc(pos,key,dlg);
		dlg->m_parAccel = m_parAccel;
		dlg->m_parPeriod = m_parPeriod;
	}

	UpdateCurDlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHeelLoadMainDlg::OnOK() 
{
	CHeelLoadChildDialog* dlg;
	int key;

	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);

	if( dlg->IsCreated() )
	{
		dlg->UpdateData(TRUE);
		if(!dlg->ISVALID()) return;
		dlg->End();
		dlg->MakeSpectrumData();
		m_sDescript = dlg->m_sDescript;
		m_strFuncName = dlg->m_strFuncName;
	}

	
	POSITION pos = m_mapChildDlg.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapChildDlg.GetNextAssoc(pos,key,dlg);
		delete dlg;
	}
	m_mapChildDlg.RemoveAll();

	
	CDialog::OnOK();
}

void CHeelLoadMainDlg::OnSelchangeEtcGdsSpectrumCmb() 
{
	CHeelLoadChildDialog* dlg;

	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
	dlg->UpdateData(TRUE);
	dlg->End();
	
	int index = m_cmbDesignCode.GetCurSel();
	m_nCurrentDlg = m_cmbDesignCode.GetItemData(index);
	UpdateCurDlg();

}


void CHeelLoadMainDlg::UpdateCurDlg()
{ 
	CHeelLoadChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(IDC_ETC_GDS_PLACEHOLDER);

	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
	
	dlg->CreateInit(pPlaceHolder);
	dlg->UpdateData(FALSE);
	dlg->ShowWindow(SW_SHOW);
}

void CHeelLoadMainDlg::OnCloseupEtcGdsSpectrumCmb() 
{
	
	
}

void CHeelLoadMainDlg::OnCancel() 
{
	CHeelLoadChildDialog* dlg;
	int key;

	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
	if( dlg->IsCreated() )
		dlg->End();

	POSITION pos = m_mapChildDlg.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapChildDlg.GetNextAssoc(pos,key,dlg);
		delete dlg;
	}
	m_mapChildDlg.RemoveAll();
	CDialog::OnCancel();
}
