// SeisNBC95CalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisNBC95CalcDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisNBC95CalcDlg dialog

CSeisNBC95CalcDlg::CSeisNBC95CalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisNBC95CalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisNBC95CalcDlg)
	m_nNX = 0;
	m_nNY = 0;
	m_dHX = 0.0;
	m_dHY = 0.0;
	m_dBX = 0.0;
	m_dBY = 0.0;
	m_nXMethod = 0;
	m_nYMethod = 0;
	//}}AFX_DATA_INIT
}


void CSeisNBC95CalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisNBC95CalcDlg)
	DDX_Control(pDX, IDC_CMD_SEIS_DSY, m_wndBY);
	DDX_Control(pDX, IDC_CMD_SEIS_DSX, m_wndBX);
	DDX_Control(pDX, IDC_CMD_SEIS_HNY, m_wndHY);
	DDX_Control(pDX, IDC_CMD_SEIS_HNX, m_wndHX);
	DDX_Control(pDX, IDC_CMD_SEIS_NY, m_wndNY);
	DDX_Control(pDX, IDC_CMD_SEIS_NX, m_wndNX);
	DDX_Text(pDX, IDC_CMD_SEIS_NX, m_nNX);
	DDX_Text(pDX, IDC_CMD_SEIS_NY, m_nNY);
	DDX_Text(pDX, IDC_CMD_SEIS_HNX, m_dHX);
	DDX_Text(pDX, IDC_CMD_SEIS_HNY, m_dHY);
	DDX_Text(pDX, IDC_CMD_SEIS_DSX, m_dBX);
	DDX_Text(pDX, IDC_CMD_SEIS_DSY, m_dBY);
	DDX_Radio(pDX, IDC_RDO_XMETHOD1, m_nXMethod);
	DDX_Radio(pDX, IDC_RDO_YMETHOD1, m_nYMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisNBC95CalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisNBC95CalcDlg)
	ON_BN_CLICKED(IDC_RDO_XMETHOD1, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD1, OnRdoYmethod)
	ON_BN_CLICKED(IDC_RDO_XMETHOD2, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_XMETHOD3, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_XMETHOD4, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD2, OnRdoYmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD3, OnRdoYmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD4, OnRdoYmethod)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisNBC95CalcDlg message handlers
BOOL CSeisNBC95CalcDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	m_wndNX.EnableWindow(m_nXMethod==0);	
	m_wndHX.EnableWindow(m_nXMethod==1 || m_nXMethod==2 || m_nXMethod==3);	
	m_wndBX.EnableWindow(m_nXMethod==3);		
	m_wndNY.EnableWindow(m_nYMethod==0);	
	m_wndHY.EnableWindow(m_nYMethod==1 || m_nYMethod==2 || m_nYMethod==3);	
	m_wndBY.EnableWindow(m_nYMethod==3);		

	m_wndHX.SetEditUnit(m_dHX);
	m_wndHY.SetEditUnit(m_dHY);
	m_wndBX.SetEditUnit(m_dBX);
	m_wndBY.SetEditUnit(m_dBY);
	m_wndNX.SetEditUnit(m_nNX);
	m_wndNY.SetEditUnit(m_nNY);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisNBC95CalcDlg::OnRdoXmethod() 
{
	UpdateData(TRUE);
	m_wndNX.EnableWindow(m_nXMethod==0);	
	m_wndHX.EnableWindow(m_nXMethod==1 || m_nXMethod==2 || m_nXMethod==3);	
	m_wndBX.EnableWindow(m_nXMethod==3);		
}

void CSeisNBC95CalcDlg::OnRdoYmethod() 
{
	UpdateData(TRUE);
	m_wndNY.EnableWindow(m_nYMethod==0);	
	m_wndHY.EnableWindow(m_nYMethod==1 || m_nYMethod==2 || m_nYMethod==3);	
	m_wndBY.EnableWindow(m_nYMethod==3);		
}

BOOL CSeisNBC95CalcDlg::CalcPeriod(double dH, double dB, int nN, int nMethod, double& dPeriod)
{
	if(nMethod == 3) 
	{
		if(dB <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_Ds_is_invalid_));
			return FALSE;
		}
	}

	switch(nMethod)
	{
	  case 0: dPeriod = 0.1 * nN;	break;			
		case 1: dPeriod = 0.085 * pow(dH, 0.75);	break;
		case 2: dPeriod = 0.075 * pow(dH, 0.75);	break;
		case 3: dPeriod = 0.09 * dH / pow(dB, 0.5);	break;		
	}
	return TRUE;
}

void CSeisNBC95CalcDlg::OnOK() 
{
	UpdateData(TRUE);

	if(!CalcPeriod(m_dHX, m_dBX, m_nNX, m_nXMethod, m_dPeriod_X)) return;
	if(!CalcPeriod(m_dHY, m_dBY, m_nNY, m_nYMethod, m_dPeriod_Y)) return;

	m_dHX = m_wndHX.GetEditValue();
	m_dHY = m_wndHY.GetEditValue();
	m_dBX = m_wndBX.GetEditValue();
	m_dBY = m_wndBY.GetEditValue();
	m_nNX = m_wndNX.GetEditValue();
	m_nNY = m_wndNY.GetEditValue();	

	CDialog::OnOK();
}
