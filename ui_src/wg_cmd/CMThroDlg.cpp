// CMThroDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThroDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_METHOD_GENERAL 0
#define D_METHOD_SPECIAL 1

/////////////////////////////////////////////////////////////////////////////
// CCMThroDlg dialog
CCMThroDlg::CCMThroDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMThroDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMThroDlg)
	m_bEnergyResult=FALSE;
	m_bSdvi        =FALSE;
	m_bSdve        =FALSE;
	m_bSdst        =FALSE;
	m_bSdhy        =FALSE;
	m_bSdis        =FALSE;

	m_nHingeOutput = D_THGC_OUTPUT_OPT_ALL;
	m_bFiberSameHinge = FALSE;
	m_nFiberOutput = D_THGC_OUTPUT_OPT_ALL;
	//}}AFX_DATA_INIT
}

void CCMThroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThroDlg)
	DDX_Check(pDX, IDC_THGC_ENRG_CHK, m_bEnergyResult);
	DDX_Check(pDX, IDC_THGC_SDVI_CHK, m_bSdvi        );
	DDX_Check(pDX, IDC_THGC_SDVE_CHK, m_bSdve        );
	DDX_Check(pDX, IDC_THGC_SDST_CHK, m_bSdst        );
	DDX_Check(pDX, IDC_THGC_SDHY_CHK, m_bSdhy        );
	DDX_Check(pDX, IDC_THGC_SDIS_CHK, m_bSdis        );
	DDX_Radio(pDX, IDC_THGC_HINGE_OPT1_RDO, m_nHingeOutput);
	DDX_Radio(pDX, IDC_THGC_FIBER_OPT1_RDO, m_nFiberOutput);
	DDX_Check(pDX, IDC_THGC_FIBER_SAME_CHK, m_bFiberSameHinge);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMThroDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMThroDlg)
	ON_BN_CLICKED(IDC_THGC_FIBER_SAME_CHK, UpdateFiberOutputCtrl)
	ON_BN_CLICKED(IDC_THGC_HINGE_OPT1_RDO, UpdateFiberOutputCtrl)
	ON_BN_CLICKED(IDC_THGC_HINGE_OPT2_RDO, UpdateFiberOutputCtrl)
	ON_BN_CLICKED(IDC_THGC_HINGE_OPT3_RDO, UpdateFiberOutputCtrl)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThroDlg message handlers

BOOL CCMThroDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetThro(m_ThroD))
		m_ThroD.Initialize();	
	
	Data2Dlg();
	UpdateFiberOutputCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThroDlg::Data2Dlg()
{
	m_bEnergyResult=m_ThroD.bEnergyResult;
	m_bSdvi        =m_ThroD.bSdvi        ;
	m_bSdve        =m_ThroD.bSdve        ;
	m_bSdst        =m_ThroD.bSdst        ;
	m_bSdhy        =m_ThroD.bSdhy        ;
	m_bSdis        =m_ThroD.bSdis        ;
	
	m_nHingeOutput    = m_ThroD.nHingeOutput;
	m_nFiberOutput    = m_ThroD.nFiberOutput;
	m_bFiberSameHinge = m_ThroD.bFiberSameHinge;

	UpdateData(FALSE);
}

void CCMThroDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_ThroD.bEnergyResult=m_bEnergyResult;
	m_ThroD.bSdvi        =m_bSdvi        ;
	m_ThroD.bSdve        =m_bSdve        ;
	m_ThroD.bSdst        =m_bSdst        ;
	m_ThroD.bSdhy        =m_bSdhy        ;
	m_ThroD.bSdis        =m_bSdis        ;

	m_ThroD.nHingeOutput    = m_nHingeOutput    ;
	m_ThroD.nFiberOutput    = m_nFiberOutput    ;
	m_ThroD.bFiberSameHinge = m_bFiberSameHinge ;
	
}

void CCMThroDlg::OnOK()
{
	Dlg2Data();
	if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddThro(m_ThroD)) return;

	CDialogMove::OnOK();
}


void CCMThroDlg::UpdateFiberOutputCtrl()
{
	UpdateData();

	if(m_bFiberSameHinge || m_nFiberOutput<m_nHingeOutput)
	{
		m_nFiberOutput = m_nHingeOutput;
		UpdateData(FALSE);
	}

	// Fiber´Â Âï´Âµ¥ ÈùÁö¸¦ ¾È Âï´Â°Ç ºÒ°¡
	GetDlgItem(IDC_THGC_FIBER_OPT1_RDO)->EnableWindow(!m_bFiberSameHinge && m_nHingeOutput<=D_THGC_OUTPUT_OPT_ALL);
	GetDlgItem(IDC_THGC_FIBER_OPT1_STC)->EnableWindow(!m_bFiberSameHinge && m_nHingeOutput<=D_THGC_OUTPUT_OPT_ALL);
	GetDlgItem(IDC_THGC_FIBER_OPT2_RDO)->EnableWindow(!m_bFiberSameHinge && m_nHingeOutput<=D_THGC_OUTPUT_OPT_SEL);
	GetDlgItem(IDC_THGC_FIBER_OPT2_STC)->EnableWindow(!m_bFiberSameHinge && m_nHingeOutput<=D_THGC_OUTPUT_OPT_SEL);
	GetDlgItem(IDC_THGC_FIBER_OPT3_RDO)->EnableWindow(!m_bFiberSameHinge);
	GetDlgItem(IDC_THGC_FIBER_OPT3_STC)->EnableWindow(!m_bFiberSameHinge);
}