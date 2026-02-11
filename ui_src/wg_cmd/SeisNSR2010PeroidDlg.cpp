// SeisIBC2012PeriodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisNSR2010PeroidDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisNSR2010PeriodDlg dialog


CSeisNSR2010PeriodDlg::CSeisNSR2010PeriodDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisNSR2010PeriodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisNSR2010PeriodDlg)
	m_nMethodX = 0;
	m_nMethodY = 0;
	//}}AFX_DATA_INIT
	m_dCwX = 0.0;
	m_dCwY = 0.0;
	m_dHnX = 0.0;
	m_dHnY = 0.0;	
	m_nNX = 0;
	m_nNY = 0;
}

void CSeisNSR2010PeriodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisNSR2010PeriodDlg)
	DDX_Control(pDX, IDC_ETC_SEIS_NX, m_wndNX);
	DDX_Control(pDX, IDC_ETC_SEIS_NX2, m_wndNY);
	DDX_Control(pDX, IDC_CMD_SEIS_YAc, m_wndCwY);
	DDX_Control(pDX, IDC_CMD_SEIS_XAc, m_wndCwX);
	DDX_Control(pDX, IDC_ETC_SEIS_YHn, m_wndHnY);
	DDX_Control(pDX, IDC_ETC_SEIS_XHn, m_wndHnX);
	DDX_Radio(pDX, IDC_CMD_XMETHOD1, m_nMethodX);
	DDX_Radio(pDX, IDC_CMD_YMETHOD1, m_nMethodY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisNSR2010PeriodDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisNSR2010PeriodDlg)
	ON_BN_CLICKED(IDC_CMD_XMETHOD1, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD2, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD3, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD4, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD5, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD6, OnRdoXmethodChange)  
	ON_BN_CLICKED(IDC_CMD_YMETHOD1, OnRdoYmethodChange)	
	ON_BN_CLICKED(IDC_CMD_YMETHOD2, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_CMD_YMETHOD3, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_CMD_YMETHOD4, OnRdoYmethodChange)	
	ON_BN_CLICKED(IDC_CMD_YMETHOD5, OnRdoYmethodChange)	
	ON_BN_CLICKED(IDC_CMD_YMETHOD6, OnRdoYmethodChange)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisNSR2010PeriodDlg message handlers
void CSeisNSR2010PeriodDlg::OnRdoXmethodChange() 
{
	CtrlManager();
}

void CSeisNSR2010PeriodDlg::OnRdoYmethodChange() 
{
	CtrlManager();
}

void CSeisNSR2010PeriodDlg::CtrlManager()
{
	UpdateData(TRUE);
	m_wndHnX.EnableWindow(m_nMethodX != 4);
	m_wndNX.EnableWindow(m_nMethodX == 4);
	m_wndCwX.EnableWindow(m_nMethodX == 5);	

	m_wndHnY.EnableWindow(m_nMethodY != 4);
	m_wndNY.EnableWindow(m_nMethodY == 4);
	m_wndCwY.EnableWindow(m_nMethodY == 5);	     
}

void CSeisNSR2010PeriodDlg::OnOK() 
{
	UpdateData(TRUE);

	m_dHnX = m_wndHnX.GetEditValue();
	m_dHnY = m_wndHnY.GetEditValue();
	m_dCwX = m_wndCwX.GetEditValue();
	m_dCwY = m_wndCwY.GetEditValue();
	m_nNX  = m_wndNX.GetEditValue();
	m_nNY  = m_wndNY.GetEditValue();

	if(!CalcPeriod(m_dHnX, m_nNX, m_dCwX, m_dPeriodX, m_nMethodX)) return;
	if(!CalcPeriod(m_dHnY, m_nNY, m_dCwY, m_dPeriodY, m_nMethodY)) return;

	CDialogMove::OnOK();
}

BOOL CSeisNSR2010PeriodDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();	

	m_wndHnX.SetEditUnit(m_dHnX);
	m_wndHnY.SetEditUnit(m_dHnY);
	m_wndCwX.SetEditUnit(m_dCwX);
	m_wndCwY.SetEditUnit(m_dCwY);
	m_wndNX.SetEditUnit(m_nNX);
	m_wndNY.SetEditUnit(m_nNY);

	CtrlManager();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSeisNSR2010PeriodDlg::CalcPeriod(double dHn, int nN, double dCw, double &dPeriod,int nMethod)
{  
	switch(nMethod)
	{
	case 0:
		dPeriod = 0.047 * pow(dHn,0.9);
		break;
	case 1:
		dPeriod = 0.072 * pow(dHn,0.8);
		break;
	case 2:
		dPeriod = 0.073 * pow(dHn,0.75);
		break;
	case 3:
		dPeriod = 0.049 * pow(dHn,0.75);			
		break;
	case 4:
		dPeriod = 0.1 * nN;      
		break;
	case 5:
		if(dCw == 0.) return FALSE;
		dPeriod = 0.0062 * dHn / sqrt(dCw);
		break;
	}		
	return TRUE;
}