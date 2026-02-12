// SeisKDS4117PeriodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisKDS4117PeriodDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisKDS4117PeriodDlg dialog


CSeisKDS4117PeriodDlg::CSeisKDS4117PeriodDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisKDS4117PeriodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisKDS4117PeriodDlg)
	m_nMethodX = 0;
	m_nMethodY = 0;
	//}}AFX_DATA_INIT
	m_dHnX = 0.0;
	m_dHnY = 0.0;	
	m_nNX = 0;
	m_nNY = 0;
}


void CSeisKDS4117PeriodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisKDS4117PeriodDlg)
	DDX_Control(pDX, IDC_ETC_SEIS_NX , m_wndNX);
	DDX_Control(pDX, IDC_ETC_SEIS_NX2, m_wndNY);
	DDX_Control(pDX, IDC_ETC_SEIS_YHn, m_wndHnY);
	DDX_Control(pDX, IDC_ETC_SEIS_XHn, m_wndHnX);
	DDX_Radio(pDX, IDC_CMD_XMETHOD1, m_nMethodX);
	DDX_Radio(pDX, IDC_CMD_YMETHOD1, m_nMethodY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisKDS4117PeriodDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisKDS4117PeriodDlg)
	ON_BN_CLICKED(IDC_CMD_XMETHOD1, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD2, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD3, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD4, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_XMETHOD5, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_CMD_YMETHOD1, OnRdoYmethodChange)	
	ON_BN_CLICKED(IDC_CMD_YMETHOD2, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_CMD_YMETHOD3, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_CMD_YMETHOD4, OnRdoYmethodChange)	
	ON_BN_CLICKED(IDC_CMD_YMETHOD5, OnRdoYmethodChange)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisKDS4117PeriodDlg message handlers
void CSeisKDS4117PeriodDlg::OnRdoXmethodChange() 
{
	CtrlManager();
}

void CSeisKDS4117PeriodDlg::OnRdoYmethodChange() 
{
	CtrlManager();
}

void CSeisKDS4117PeriodDlg::CtrlManager()
{
	UpdateData(TRUE);

	BOOL bUseHn_X = m_nMethodX != 4;
	m_wndHnX.EnableWindow(bUseHn_X);
	m_wndNX.EnableWindow(!bUseHn_X);

	BOOL bUseHn_Y = m_nMethodY != 4;
	m_wndHnY.EnableWindow(bUseHn_Y);
	m_wndNY.EnableWindow(!bUseHn_Y);
}

void CSeisKDS4117PeriodDlg::OnOK() 
{
	UpdateData(TRUE);

	m_dHnX = m_wndHnX.GetEditValue();
	m_dHnY = m_wndHnY.GetEditValue();
	m_nNX  = m_wndNX.GetEditValue();
	m_nNY  = m_wndNY.GetEditValue();

	if(!CalcPeriod(m_dHnX, m_nNX, m_dPeriodX, m_nMethodX)) return;
	if(!CalcPeriod(m_dHnY, m_nNY, m_dPeriodY, m_nMethodY)) return;
	
	CDialogMove::OnOK();
}

BOOL CSeisKDS4117PeriodDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();	

	m_wndHnX.SetEditUnit(m_dHnX);
	m_wndHnY.SetEditUnit(m_dHnY);
	m_wndNX.SetEditUnit(m_nNX);
	m_wndNY.SetEditUnit(m_nNY);
	
	CtrlManager();

	if(!m_strTitle.IsEmpty())
		SetWindowText(m_strTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSeisKDS4117PeriodDlg::CalcPeriod(double hn, int N, double &dPeriod,int nMethod)
{  
	switch(nMethod)
	{
		case 0:
			dPeriod = 0.0466 * pow(hn,0.9);
			break;
		case 1:
			dPeriod = 0.0724 * pow(hn,0.8);
			break;
		case 2:
			dPeriod = 0.0731 * pow(hn,0.75);
			break;
		case 3:
			dPeriod = 0.0488 * pow(hn,0.75);
			break;
		case 4:
			dPeriod = 0.1 * N;
			break;
	}		
	return TRUE;
}
