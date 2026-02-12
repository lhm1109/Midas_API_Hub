// SeisIS875_15CalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisIS875_15CalcDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisIS875_15CalcDlg dialog

CSeisIS875_15CalcDlg::CSeisIS875_15CalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisIS875_15CalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisIS875_15CalcDlg)
	m_nXMethod = 0;
	m_nYMethod = 0;
	//}}AFX_DATA_INIT
	m_nNX = 0;
	m_nNY = 0;
	m_dBX = 0.0;
	m_dBY = 0.0;
	m_dHX = 0.0;
	m_dHY = 0.0;	
}


void CSeisIS875_15CalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisIS875_15CalcDlg)
	DDX_Control(pDX, IDC_ETC_SEIS_NY, m_wndNY);
	DDX_Control(pDX, IDC_ETC_SEIS_NX, m_wndNX);
	DDX_Control(pDX, IDC_CMD_SEIS_BY, m_wndBY);
	DDX_Control(pDX, IDC_CMD_SEIS_BX, m_wndBX);
	DDX_Control(pDX, IDC_ETC_SEIS_HX, m_wndHX);
	DDX_Control(pDX, IDC_ETC_SEIS_HY, m_wndHY);
	DDX_Radio(pDX, IDC_RDO_XMETHOD1, m_nXMethod);
	DDX_Radio(pDX, IDC_RDO_YMETHOD1, m_nYMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisIS875_15CalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisIS875_15CalcDlg)
	ON_BN_CLICKED(IDC_RDO_XMETHOD1, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD1, OnRdoYmethod)
	ON_BN_CLICKED(IDC_RDO_XMETHOD2, OnRdoXmethod)	
	ON_BN_CLICKED(IDC_RDO_YMETHOD2, OnRdoYmethod)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisIS875_15CalcDlg message handlers

BOOL CSeisIS875_15CalcDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	/*GetDlgItem(IDC_RDO_XMETHOD1)->SetWindowText(_T("1. T = 0.22+0.035H/B^(1/3)"));
	GetDlgItem(IDC_RDO_XMETHOD2)->SetWindowText(_T("2. T = 0.25+0.00053H^2/B^(1/3)"));
	GetDlgItem(IDC_RDO_XMETHOD3)->SetWindowText(_T("3. T = 0.03+0.03H/B^(1/3)"));
	GetDlgItem(IDC_RDO_XMETHOD4)->SetWindowText(_T("4. T = 0.1n"));
	GetDlgItem(IDC_RDO_YMETHOD1)->SetWindowText(_T("1. T = 0.22+0.035H/B^(1/3)"));
	GetDlgItem(IDC_RDO_YMETHOD2)->SetWindowText(_T("2. T = 0.25+0.00053H^2/B^(1/3)"));
	GetDlgItem(IDC_RDO_YMETHOD3)->SetWindowText(_T("3. T = 0.03+0.03H/B^(1/3)"));
	GetDlgItem(IDC_RDO_YMETHOD4)->SetWindowText(_T("4. T = 0.1n"));*/

	// TODO: Add extra initialization here
	m_wndHX.EnableWindow( m_nXMethod==1 );	
	m_wndBX.EnableWindow( m_nXMethod==1 );	
	m_wndNX.EnableWindow(m_nXMethod==0);	
	m_wndHY.EnableWindow(m_nYMethod==1 );	
	m_wndBY.EnableWindow(m_nYMethod==1 );	
	m_wndNY.EnableWindow(m_nYMethod==0);	

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX UnitIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);

	if(m_nUnitChange == 0)  // 단위변환 : Wind Load 인 경우
	{
		switch(UnitIndex.nBase_Length)
		{
			case D_UNITSYS_LENGTH_INDEX_MM:
				m_dHX = m_dHX * D_UNITSYS_LENGTH_FACTOR_MM;
				m_dHY = m_dHY * D_UNITSYS_LENGTH_FACTOR_MM;
				m_dBX = m_dBX * D_UNITSYS_LENGTH_FACTOR_MM;
				m_dBY = m_dBY * D_UNITSYS_LENGTH_FACTOR_MM;
				break;
			case D_UNITSYS_LENGTH_INDEX_CM:
				m_dHX = m_dHX * D_UNITSYS_LENGTH_FACTOR_CM;
				m_dHY = m_dHY * D_UNITSYS_LENGTH_FACTOR_CM;
				m_dBX = m_dBX * D_UNITSYS_LENGTH_FACTOR_CM;
				m_dBY = m_dBY * D_UNITSYS_LENGTH_FACTOR_CM;
				break;
			case D_UNITSYS_LENGTH_INDEX_M:
				m_dHX = m_dHX * D_UNITSYS_LENGTH_FACTOR_M;
				m_dHY = m_dHY * D_UNITSYS_LENGTH_FACTOR_M;
				m_dBX = m_dBX * D_UNITSYS_LENGTH_FACTOR_M;
				m_dBY = m_dBY * D_UNITSYS_LENGTH_FACTOR_M;
				break;
			case D_UNITSYS_LENGTH_INDEX_IN:
				m_dHX = m_dHX * D_UNITSYS_LENGTH_FACTOR_IN;
				m_dHY = m_dHY * D_UNITSYS_LENGTH_FACTOR_IN;
				m_dBX = m_dBX * D_UNITSYS_LENGTH_FACTOR_IN;
				m_dBY = m_dBY * D_UNITSYS_LENGTH_FACTOR_IN;
				break;
			case D_UNITSYS_LENGTH_INDEX_FT:
				m_dHX = m_dHX * D_UNITSYS_LENGTH_FACTOR_FT;
				m_dHY = m_dHY * D_UNITSYS_LENGTH_FACTOR_FT;
				m_dBX = m_dBX * D_UNITSYS_LENGTH_FACTOR_FT;
				m_dBY = m_dBY * D_UNITSYS_LENGTH_FACTOR_FT;
				break;
		}
	}

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

void CSeisIS875_15CalcDlg::OnRdoXmethod() 
{
	UpdateData(TRUE);
	m_wndHX.EnableWindow( m_nXMethod==1 );	
	m_wndBX.EnableWindow( m_nXMethod==1 );	
	m_wndNX.EnableWindow(m_nXMethod==0);	
}

void CSeisIS875_15CalcDlg::OnRdoYmethod() 
{
	UpdateData(TRUE);
	m_wndHY.EnableWindow( m_nYMethod==1 );	
	m_wndBY.EnableWindow( m_nYMethod==1 );	
	m_wndNY.EnableWindow(m_nYMethod==0);	
}

BOOL CSeisIS875_15CalcDlg::CalcPeriod(double dH, double dB, int nN, int nMethod, double& dPeriod)
{
	if(nMethod == 0 || nMethod == 1 || nMethod == 2) 
	{
		if(dB <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_B_is_invalid_));
			return FALSE;
		}
	}

	switch(nMethod)
	{
		case 0: dPeriod = 0.1 * nN;	break;			
		case 1: dPeriod = 0.09 * dH / pow(dB, (1.0/2.0));	break;
			
	}
	return TRUE;
}

void CSeisIS875_15CalcDlg::OnOK() 
{
	UpdateData(TRUE);

	m_dHX = m_wndHX.GetEditValue();
	m_dHY = m_wndHY.GetEditValue();
	m_dBX = m_wndBX.GetEditValue();
	m_dBY = m_wndBY.GetEditValue();
	m_nNX = m_wndNX.GetEditValue();
	m_nNY = m_wndNY.GetEditValue();	

	if(!CalcPeriod(m_dHX, m_dBX, m_nNX, m_nXMethod, m_dPeriod_X)) return;
	if(!CalcPeriod(m_dHY, m_dBY, m_nNY, m_nYMethod, m_dPeriod_Y)) return;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX UnitIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	if(m_nUnitChange == 0)  // Wind Load 인 경우에는 단위변환
	{
		switch(UnitIndex.nBase_Length)
		{
			case D_UNITSYS_LENGTH_INDEX_MM:
				m_dHX = m_dHX / D_UNITSYS_LENGTH_FACTOR_MM;
				m_dHY = m_dHY / D_UNITSYS_LENGTH_FACTOR_MM;
				m_dBX = m_dBX / D_UNITSYS_LENGTH_FACTOR_MM;
				m_dBY = m_dBY / D_UNITSYS_LENGTH_FACTOR_MM;
				break;
			case D_UNITSYS_LENGTH_INDEX_CM:
				m_dHX = m_dHX / D_UNITSYS_LENGTH_FACTOR_CM;
				m_dHY = m_dHY / D_UNITSYS_LENGTH_FACTOR_CM;
				m_dBX = m_dBX / D_UNITSYS_LENGTH_FACTOR_CM;
				m_dBY = m_dBY / D_UNITSYS_LENGTH_FACTOR_CM;
				break;
			case D_UNITSYS_LENGTH_INDEX_M:
				m_dHX = m_dHX / D_UNITSYS_LENGTH_FACTOR_M;
				m_dHY = m_dHY / D_UNITSYS_LENGTH_FACTOR_M;
				m_dBX = m_dBX / D_UNITSYS_LENGTH_FACTOR_M;
				m_dBY = m_dBY / D_UNITSYS_LENGTH_FACTOR_M;
				break;
			case D_UNITSYS_LENGTH_INDEX_IN:
				m_dHX = m_dHX / D_UNITSYS_LENGTH_FACTOR_IN;
				m_dHY = m_dHY / D_UNITSYS_LENGTH_FACTOR_IN;
				m_dBX = m_dBX / D_UNITSYS_LENGTH_FACTOR_IN;
				m_dBY = m_dBY / D_UNITSYS_LENGTH_FACTOR_IN;
				break;
			case D_UNITSYS_LENGTH_INDEX_FT:
				m_dHX = m_dHX / D_UNITSYS_LENGTH_FACTOR_FT;
				m_dHY = m_dHY / D_UNITSYS_LENGTH_FACTOR_FT;
				m_dBX = m_dBX / D_UNITSYS_LENGTH_FACTOR_FT;
				m_dBY = m_dBY / D_UNITSYS_LENGTH_FACTOR_FT;
				break;
		}
	}

	CDialogMove::OnOK();
}

