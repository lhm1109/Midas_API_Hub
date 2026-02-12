// SeisKBC2005PeriodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisKBC2005PeriodDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisKBC2005PeriodDlg dialog


CSeisKBC2005PeriodDlg::CSeisKBC2005PeriodDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisKBC2005PeriodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisKBC2005PeriodDlg)
	m_nMethodX = 0;
	m_nMethodY = 0;
	//}}AFX_DATA_INIT
	m_dAcX = 0.0;
	m_dAcY = 0.0;
	m_dHnX = 0.0;
	m_dHnY = 0.0;	
	m_nNX = 0;
	m_nNY = 0;
}


void CSeisKBC2005PeriodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisKBC2005PeriodDlg)
	DDX_Control(pDX, IDC_ETC_SEIS_NX, m_wndNX);
	DDX_Control(pDX, IDC_ETC_SEIS_NX2, m_wndNY);
	DDX_Control(pDX, IDC_CMD_SEIS_YAc, m_wndAcY);
	DDX_Control(pDX, IDC_CMD_SEIS_XAc, m_wndAcX);
	DDX_Control(pDX, IDC_ETC_SEIS_YHn, m_wndHnY);
	DDX_Control(pDX, IDC_ETC_SEIS_XHn, m_wndHnX);
	DDX_Radio(pDX, IDC_CMD_XMETHOD1, m_nMethodX);
	DDX_Radio(pDX, IDC_CMD_YMETHOD1, m_nMethodY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisKBC2005PeriodDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisKBC2005PeriodDlg)
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
// CSeisKBC2005PeriodDlg message handlers
void CSeisKBC2005PeriodDlg::OnRdoXmethodChange() 
{
	CtrlManager();
}

void CSeisKBC2005PeriodDlg::OnRdoYmethodChange() 
{
	CtrlManager();
}

void CSeisKBC2005PeriodDlg::CtrlManager()
{
	UpdateData(TRUE);
	m_wndHnX.EnableWindow(m_nMethodX != 3);
	m_wndNX.EnableWindow(m_nMethodX == 3);
	m_wndAcX.EnableWindow(m_nMethodX == 4);	

	m_wndHnY.EnableWindow(m_nMethodY != 3);
	m_wndNY.EnableWindow(m_nMethodY == 3);
	m_wndAcY.EnableWindow(m_nMethodY == 4);	     
}

void CSeisKBC2005PeriodDlg::OnOK() 
{
	UpdateData(TRUE);

	m_dHnX = m_wndHnX.GetEditValue();
	m_dHnY = m_wndHnY.GetEditValue();
	m_dAcX = m_wndAcX.GetEditValue();
	m_dAcY = m_wndAcY.GetEditValue();
	m_nNX  = m_wndNX.GetEditValue();
	m_nNY  = m_wndNY.GetEditValue();

	if(!CalcPeriod(m_dHnX, m_nNX, m_dAcX, m_dPeriodX, m_nMethodX)) return;
	if(!CalcPeriod(m_dHnY, m_nNY, m_dAcY, m_dPeriodY, m_nMethodY)) return;
	
	CDialogMove::OnOK();
}

BOOL CSeisKBC2005PeriodDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();	

	m_wndHnX.SetEditUnit(m_dHnX);
	m_wndHnY.SetEditUnit(m_dHnY);
	m_wndAcX.SetEditUnit(m_dAcX);
	m_wndAcY.SetEditUnit(m_dAcY);
	m_wndNX.SetEditUnit(m_nNX);
	m_wndNY.SetEditUnit(m_nNY);
	
	CtrlManager();

	if(!m_strTitle.IsEmpty())
		SetWindowText(m_strTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSeisKBC2005PeriodDlg::CalcPeriod(double dHn, int nN, double dAc, double &dPeriod,int nMethod)
{  
	switch(nMethod)
	{
		case 0:
			dPeriod = 0.085 * pow(dHn,(3.0/4.0));
			break;
		case 1:
			dPeriod = 0.073 * pow(dHn,(3.0/4.0));
			break;
		case 2:
			dPeriod = 0.049 * pow(dHn,(3.0/4.0));
			break;
		case 3:
			dPeriod = 0.1 * nN;
			break;
		case 4:
			if(dAc == 0.) return FALSE;
			dPeriod = 0.0743 * pow(dHn,(3.0/4.0)) / sqrt(dAc);
			break;
	}		
	return TRUE;
}
