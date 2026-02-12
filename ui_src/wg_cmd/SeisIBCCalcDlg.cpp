// SeisIBCCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisIBCCalcDlg.h"

//#include "..\wg_base\wg_base_StrParser.h"
//#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisIBCCalcDlg dialog


CSeisIBCCalcDlg::CSeisIBCCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisIBCCalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisIBCCalcDlg)
	m_hnX = 0.0;
	m_hnY = 0.0;
	m_xrdo = -1;
	m_yrdo = -1;
	//}}AFX_DATA_INIT
}


void CSeisIBCCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisIBCCalcDlg)
	DDX_Control(pDX, IDC_ETC_SEIS_NY, m_wndNy);
	DDX_Control(pDX, IDC_ETC_SEIS_NX, m_wndNx);
	DDX_Control(pDX, IDC_ETC_SEIS_YHn, m_wndHy);
	DDX_Control(pDX, IDC_ETC_SEIS_XHn, m_wndHx);
	DDX_Radio(pDX, IDC_RDO_XMETHOD1, m_xrdo);
	DDX_Radio(pDX, IDC_RDO_YMETHOD1, m_yrdo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisIBCCalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisIBCCalcDlg)
	ON_BN_CLICKED(IDC_RDO_XMETHOD1, OnSelChangeX)
	ON_BN_CLICKED(IDC_RDO_YMETHOD1, OnSelChangeY)
	ON_BN_CLICKED(IDC_RDO_XMETHOD2, OnSelChangeX)
	ON_BN_CLICKED(IDC_RDO_XMETHOD3, OnSelChangeX)
	ON_BN_CLICKED(IDC_RDO_XMETHOD4, OnSelChangeX)
	ON_BN_CLICKED(IDC_RDO_YMETHOD2, OnSelChangeY)
	ON_BN_CLICKED(IDC_RDO_YMETHOD3, OnSelChangeY)
	ON_BN_CLICKED(IDC_RDO_YMETHOD4, OnSelChangeY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisIBCCalcDlg message handlers

BOOL CSeisIBCCalcDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_hnX = m_hnX / 0.3048;
	m_hnY = m_hnY / 0.3048;
	
	m_wndHx.SetEditUnit(m_hnX);
	m_wndHy.SetEditUnit(m_hnY);
	m_wndNx.SetEditUnit(m_nNX);
	m_wndNy.SetEditUnit(m_nNY);
	OnSelChangeX();
	OnSelChangeY();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisIBCCalcDlg::OnSelChangeX() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
//	CEdit* ed = (CEdit*)GetDlgItem(IDC_ETC_SEIS_XHn);  
//	ed->EnableWindow(m_xrdo != 3);
	m_wndHx.EnableWindow(m_xrdo != 3);
	m_wndNx.EnableWindow(m_xrdo == 3);

}

void CSeisIBCCalcDlg::OnSelChangeY() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
// Edit* ed = (CEdit*)GetDlgItem(IDC_ETC_SEIS_YHn);
//	ed->EnableWindow(m_yrdo != 3);
	m_wndHy.EnableWindow(m_yrdo != 3);
	m_wndNy.EnableWindow(m_yrdo == 3);
}

BOOL CSeisIBCCalcDlg::CalcPeriod(double dH, int nN, int nMethod, double& dPeriod)
{
	if(nMethod != 3 && dH < 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Invalid_value_of_H));
		return FALSE;
	}
	if(nMethod == 3 && nN < 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Invalid_value_of_N));
		return FALSE;
	}
	switch(nMethod)
	{
		case 0: dPeriod = 0.035 * pow(dH, (3./4.));	break;
		case 1:	dPeriod = 0.030 * pow(dH, (3./4.));	break;
		case 2:	dPeriod = 0.020 * pow(dH, (3./4.));	break;
		case 3:	dPeriod = 0.1 * nN;	break;      // added by KYM
	}
	return TRUE;
}

void CSeisIBCCalcDlg::OnOK() 
{
	UpdateData(TRUE);
	
	m_hnX = m_wndHx.GetEditValue();
	m_hnY = m_wndHy.GetEditValue();
	m_nNX = m_wndNx.GetEditValue();
	m_nNY = m_wndNy.GetEditValue();
	
	if(!CalcPeriod(m_hnX, m_nNX, m_xrdo, m_dPeriod_X)) return;
	if(!CalcPeriod(m_hnY, m_nNY, m_yrdo, m_dPeriod_Y)) return;

	m_hnX = m_hnX * 0.3048;
	m_hnY = m_hnY * 0.3048;
	  
	CDialogMove::OnOK();
}
