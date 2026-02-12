// SeisUBCClacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisUBCClacDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"  


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisUBCClacDlg dialog


CSeisUBCClacDlg::CSeisUBCClacDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisUBCClacDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisUBCClacDlg)	
	m_hnX = 0.0;
	m_hnY = 0.0;
	m_xrdo = -1;
	m_yrdo = -1;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSeisUBCClacDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisUBCClacDlg)
	DDX_Control(pDX, IDC_ETC_SEIS_YHn, m_wndHy);
	DDX_Control(pDX, IDC_ETC_SEIS_XHn, m_wndHx);
	DDX_Radio(pDX, IDC_RDO_XMETHOD1, m_xrdo);
	DDX_Radio(pDX, IDC_RDO_YMETHOD1, m_yrdo);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisUBCClacDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisUBCClacDlg)
	ON_BN_CLICKED(IDC_RDO_XMETHOD1, OnSelChangeX)
	ON_BN_CLICKED(IDC_RDO_YMETHOD1, OnSelChangeY)
	ON_BN_CLICKED(IDC_RDO_XMETHOD2, OnSelChangeX)
	ON_BN_CLICKED(IDC_RDO_YMETHOD2, OnSelChangeY)
	ON_BN_CLICKED(IDC_RDO_XMETHOD3, OnSelChangeX)	
	ON_BN_CLICKED(IDC_RDO_YMETHOD3, OnSelChangeY)	
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisUBCClacDlg message handlers


BOOL CSeisUBCClacDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_hnX = m_hnX / 0.3048;
	m_hnY = m_hnY / 0.3048;
	
	m_wndHx.SetEditUnit(m_hnX);
	m_wndHy.SetEditUnit(m_hnY);

	OnSelChangeX();
	OnSelChangeY();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisUBCClacDlg::OnSelChangeX() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_wndHx.EnableWindow(TRUE);
}

void CSeisUBCClacDlg::OnSelChangeY() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_wndHy.EnableWindow(TRUE);
}

BOOL CSeisUBCClacDlg::CalcPeriod(double dH, int nMethod, double& dPeriod)
{
	if(dH < 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Invalid_value_of_H));
		return FALSE;
	}

	switch(nMethod)
	{
		case 0: dPeriod = 0.035 * pow(dH, (3./4.));	break;
		case 1:	dPeriod = 0.030 * pow(dH, (3./4.));	break;
		case 2:	dPeriod = 0.020 * pow(dH, (3./4.));	break;
	}
	return TRUE;
}

void CSeisUBCClacDlg::OnOK() 
{
	UpdateData(TRUE);
	
	m_hnX = m_wndHx.GetEditValue();
	m_hnY = m_wndHy.GetEditValue();

	if(!CalcPeriod(m_hnX, m_xrdo, m_dPeriod_X)) return;
	if(!CalcPeriod(m_hnY, m_yrdo, m_dPeriod_Y)) return;

	m_hnX = m_hnX * 0.3048;
	m_hnY = m_hnY * 0.3048;
	  
	CDialogMove::OnOK();
}

