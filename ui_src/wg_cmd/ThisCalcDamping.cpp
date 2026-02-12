// ThisCalcDamping.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ThisCalcDamping.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThisCalcDamping dialog


CThisCalcDamping::CThisCalcDamping(CWnd* pParent /*=NULL*/)
	: CDialogMove(CThisCalcDamping::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThisCalcDamping)  
	//}}AFX_DATA_INIT
}


void CThisCalcDamping::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThisCalcDamping)
	DDX_Control(pDX, IDC_CMD_MASS_COEFFICIENT_CHK, m_wndMassChk);
	DDX_Control(pDX, IDC_CMD_STIFF_COEFFICIENT_CHK, m_wndStiffChk);
	DDX_Control(pDX, IDC_CMD_MASS_COEFFICIENT_EDIT, m_wndMassCoef);
	DDX_Control(pDX, IDC_CMD_STIFF_COEFFICIENT_EDIT, m_wndStiffCoef);
	DDX_Control(pDX, IDC_CMD_FREQUENCY_EDIT, m_wndFrequency);
	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT, m_wndPeriod);
	DDX_Control(pDX, IDC_CMD_DAMPING_RATIO_EDIT, m_wndDampingRatio);
	DDX_Radio(pDX, IDC_CMD_FREQ_PERIOD_RDO, m_nFreqPeriodRdo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThisCalcDamping, CDialogMove)
	//{{AFX_MSG_MAP(CThisCalcDamping)
	ON_BN_CLICKED(IDC_CMD_FREQ_PERIOD_RDO, OnChangeFreqPeriod)
	ON_BN_CLICKED(IDC_CMD_FREQ_PERIOD_RDO2, OnChangeFreqPeriod)    
	ON_EN_CHANGE(IDC_CMD_FREQUENCY_EDIT, OnChangeCmdFrequencyEdit)
	ON_EN_CHANGE(IDC_CMD_PERIOD_EDIT, OnChangeCmdPeriodEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThisCalcDamping message handlers

BOOL CThisCalcDamping::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_wndMassChk.SetCheck(m_bMass);
	m_wndStiffChk.SetCheck(m_bStiff);
	m_wndMassCoef.SetEditUnit(m_dRm);
	m_wndStiffCoef.SetEditUnit(m_dRk);  
	m_wndFrequency.SetEditUnit(m_dFrequency);  // 1.0
	m_wndPeriod.SetEditUnit(m_dPeriod);
	CtrlManager();
	AutoCalcDampingRatio();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CThisCalcDamping::OnChangeFreqPeriod()
{
	CtrlManager();
	AutoCalcDampingRatio();
}

void CThisCalcDamping::OnChangeCmdFrequencyEdit() 
{
	AutoCalcDampingRatio();	
}

void CThisCalcDamping::OnChangeCmdPeriodEdit() 
{
	AutoCalcDampingRatio();	
}

void CThisCalcDamping::CtrlManager()
{
	UpdateData(TRUE);
	CWnd *pWndFrequency = GetDlgItem(IDC_CMD_FREQUENCY_EDIT);
	CWnd *pWndPeriod    = GetDlgItem(IDC_CMD_PERIOD_EDIT);
	pWndFrequency->EnableWindow(m_nFreqPeriodRdo == 0);
	pWndPeriod->EnableWindow(m_nFreqPeriodRdo == 1);
	UpdateData(FALSE);
}

void CThisCalcDamping::AutoCalcDampingRatio()
{
	UpdateData(TRUE);
	double dDampingRatio = 0.;
	double dFrequency = m_wndFrequency.GetEditValue();
	double dPeriod    = m_wndPeriod.GetEditValue();
	double dOmega = 0; 

	BOOL bErrorCondition = FALSE;
	if(m_nFreqPeriodRdo == 0 && dFrequency <= 0.) bErrorCondition = TRUE; 
	if(m_nFreqPeriodRdo == 1 && dPeriod <= 0.)    bErrorCondition = TRUE; 

//  if(m_nFreqPeriodRdo == 0 && dFrequency <= 0.) {AfxMessageBox(_LS(IDS_WG_CMD_FREQUENCY_ERROR)); return;} // _T("Error : Input value is invalid!")
//  if(m_nFreqPeriodRdo == 1 && dPeriod <= 0.)    {AfxMessageBox(_LS(IDS_WG_CMD_PERIOD_ERROR)); return;}

	if(bErrorCondition)  m_wndDampingRatio.SetWindowText(_T("--"));
	else
	{
		if(m_nFreqPeriodRdo == 0)      dOmega = 2 * PI * dFrequency;
		else if(m_nFreqPeriodRdo == 1) dOmega = 2 * PI / dPeriod;
		else ASSERT(FALSE);

		if(m_bMass == TRUE && m_bStiff == FALSE)       dDampingRatio = m_dRm / 2. / dOmega;
		else if(m_bMass == FALSE && m_bStiff == TRUE)  dDampingRatio = m_dRk * dOmega / 2.;
		else if(m_bMass == TRUE && m_bStiff == TRUE)   dDampingRatio = (m_dRm / dOmega + m_dRk * dOmega) / 2.;
		else if(m_bMass == FALSE && m_bStiff == FALSE) ASSERT(FALSE);
		
		m_wndDampingRatio.SetEditUnit(dDampingRatio);
	}
}

void CThisCalcDamping::OnOK() 
{
	UpdateData();
	m_dFrequency = m_wndFrequency.GetEditValue();
	m_dPeriod    = m_wndPeriod.GetEditValue();  

	CDialogMove::OnOK();
}


