//////////
// AssessmentRptPSC.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_dgn.h"
#include "AssessmentRptPSC.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssessmentRptPSCdialog
CAssessmentRptPSC::CAssessmentRptPSC(CWnd* pParent /*=NULL*/)
	: CDialogMove(CAssessmentRptPSC::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CAssessmentRptPSC::~CAssessmentRptPSC()
{

}

void CAssessmentRptPSC::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssessmentRptPSC)	
	DDX_Control(pDX, IDC_DGN_EMF_CHK, m_chkEmf);
	//DDX_Control(pDX, IDC_DGN_RATING_PARAM_STEEL_UNIT, m_TensSteelUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAssessmentRptPSC, CDialogMove)
	//{{AFX_MSG_MAP(CAssessmentRptPSC)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAssessmentRptPSC::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	SetControl();
	//Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAssessmentRptPSC::SetControl()
{
	m_aFileOpt.RemoveAll();
	m_aFileOpt.Add(IDC_DGN_NEW_FILE_RDO);
	m_aFileOpt.Add(IDC_DGN_EXIST_FILE_RDO);
	CDlgUtil::CtrlRadioSetCheck(this, m_aFileOpt, 0);
}

void CAssessmentRptPSC::OnOk() 
{
	if(!Dlg2Data())
	{
		return;
	}

	CDialogMove::OnOK();
}

BOOL CAssessmentRptPSC::Dlg2Data() 
{
	m_bEmf = m_chkEmf.GetCheck();	
	CDlgUtil::CtrlRadioGetCheck(this, m_aFileOpt, nFileOpt);

	return TRUE;
}

void CAssessmentRptPSC::Data2Dlg() 
{
	//CDlgUtil::CtrlRadioSetCheck(this, m_aFileOpt, nFileOpt);
	//m_chkEmf.SetCheck(m_bEmf);
}

void CAssessmentRptPSC::OnCancel() 
{
	CDialogMove::OnCancel();
}