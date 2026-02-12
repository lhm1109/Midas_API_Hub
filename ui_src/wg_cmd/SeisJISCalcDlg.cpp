// SeisJISCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisJISCalcDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisJISCalcDlg dialog


CSeisJISCalcDlg::CSeisJISCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisJISCalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisJISCalcDlg)
	m_ax = 0.0;
	m_ay = 0.0;
	m_hx = 0.0;
	m_hy = 0.0;
	//}}AFX_DATA_INIT
}


void CSeisJISCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisJISCalcDlg)
	DDX_Control(pDX, IDC_ETC_SEIS_AY, m_wndAy);
	DDX_Control(pDX, IDC_ETC_SEIS_AX, m_wndAx);
	DDX_Control(pDX, IDC_ETC_SEIS_HY, m_wndHy);
	DDX_Control(pDX, IDC_ETC_SEIS_HX, m_wndHx);
	DDX_Text(pDX, IDC_ETC_SEIS_AX, m_ax);
	DDX_Text(pDX, IDC_ETC_SEIS_AY, m_ay);
	DDX_Text(pDX, IDC_ETC_SEIS_HX, m_hx);
	DDX_Text(pDX, IDC_ETC_SEIS_HY, m_hy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisJISCalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisJISCalcDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisJISCalcDlg message handlers
BOOL CSeisJISCalcDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
		
	// TODO: Add extra initialization here
	m_wndHx.SetEditUnit(m_hx);
	m_wndHy.SetEditUnit(m_hy);
	m_wndAx.SetEditUnit(m_ax);
	m_wndAy.SetEditUnit(m_ay);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisJISCalcDlg::SetPeriodValue(double hx, double hy, double ax, double ay)
{
}

void CSeisJISCalcDlg::GetPeriodValue(double& hx, double& hy, double& ax, double& ay)
{
}

void CSeisJISCalcDlg::OnOK() 
{
	UpdateData(TRUE);

	m_hx = m_wndHx.GetEditValue();
	m_hy = m_wndHy.GetEditValue();
	m_ax = m_wndAx.GetEditValue();
	m_ay = m_wndAy.GetEditValue();

	CDialogMove::OnOK();
}
