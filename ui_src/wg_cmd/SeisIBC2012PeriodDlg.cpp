// SeisIBC2012PeriodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisIBC2012PeriodDlg.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\Seismic_NSCP2024.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisIBC2012PeriodDlg dialog


CSeisIBC2012PeriodDlg::CSeisIBC2012PeriodDlg(CWnd* pParent /*=NULL*/, int nCode /*= 0*/)
	: CDialogMove(CSeisIBC2012PeriodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisIBC2012PeriodDlg)
	m_nMethodX = 0;
	m_nMethodY = 0;
	//}}AFX_DATA_INIT
	m_dCwX = 0.0;
	m_dCwY = 0.0;
	m_dHnX = 0.0;
	m_dHnY = 0.0;	
	m_nNX = 0;
	m_nNY = 0;
	
	SetCode(nCode);
}

void CSeisIBC2012PeriodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisIBC2012PeriodDlg)
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


BEGIN_MESSAGE_MAP(CSeisIBC2012PeriodDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisIBC2012PeriodDlg)
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
// CSeisIBC2012PeriodDlg message handlers
void CSeisIBC2012PeriodDlg::OnRdoXmethodChange() 
{
	CtrlManager();
}

void CSeisIBC2012PeriodDlg::OnRdoYmethodChange() 
{
	CtrlManager();
}

void CSeisIBC2012PeriodDlg::CtrlManager()
{
	UpdateData(TRUE);
	m_wndHnX.EnableWindow(m_nMethodX != 4);
	m_wndNX.EnableWindow(m_nMethodX == 4);
	m_wndCwX.EnableWindow(m_nMethodX == 5);	

	m_wndHnY.EnableWindow(m_nMethodY != 4);
	m_wndNY.EnableWindow(m_nMethodY == 4);
	m_wndCwY.EnableWindow(m_nMethodY == 5);	     
}

void CSeisIBC2012PeriodDlg::SetControlByCode(int nCode)
{
	CString strTitle = _T("-");
	CString strTaMethod1 = _T("-");
	CString strTaMethod2 = _T("-");
	CString strTaMethod3 = _T("-");
	CString strTaMethod4 = _T("-");
	CString strTaMethod5 = _T("-");
	CString strTaMethod6 = _T("-");
	CString strUnitH = _T("-");
		
	switch (nCode)
	{
	case IBC_E_2009: 
	case IBC_E_2012:
		{
			strTitle = _LS(IDS_WG_CMD_IBC2012_CALC_PERIOD);
			strTaMethod1 = _T("1. T = 0.028*hn^(0.8)");
			strTaMethod2 = _T("2. T = 0.016*hn^(0.9)");
			strTaMethod3 = _T("3. T = 0.03*hn^(0.75)");
			strTaMethod4 = _T("4. T = 0.02*hn^(0.75)");
			strTaMethod5 = _T("5. T = 0.1N");
			strTaMethod6 = _T("6. T = 0.0019*hn/sqrt(Cw)");
			strUnitH = _T("(ft)");
		}
		break;
	case NSCP_E_2024:
		{			
			strTitle.Format(_T("%s %s"), FULL_NSCP_E_2024, _LS(IDS_WG_CMD_CALC_PERIOD));
			strTaMethod1 = _T("1. T = 0.0724*hn^(0.8)");
			strTaMethod2 = _T("2. T = 0.0466*hn^(0.9)");
			strTaMethod3 = _T("3. T = 0.0731*hn^(0.75)");
			strTaMethod4 = _T("4. T = 0.0488*hn^(0.75)");
			strTaMethod5 = _T("5. T = 0.1N");
			strTaMethod6 = _T("6. T = 0.00058*hn/sqrt(Cw)");
			strUnitH = _T("(m)");
		}
		break;
	default: 
		ASSERT(0);
		break;
	}
	
	SetWindowText(strTitle);
	
	GetDlgItem(IDC_CMD_XMETHOD1)->SetWindowText(strTaMethod1);
	GetDlgItem(IDC_CMD_XMETHOD2)->SetWindowText(strTaMethod2);
	GetDlgItem(IDC_CMD_XMETHOD3)->SetWindowText(strTaMethod3);
	GetDlgItem(IDC_CMD_XMETHOD4)->SetWindowText(strTaMethod4);
	GetDlgItem(IDC_CMD_XMETHOD5)->SetWindowText(strTaMethod5);
	GetDlgItem(IDC_CMD_XMETHOD6)->SetWindowText(strTaMethod6);
	GetDlgItem(IDC_WG_CMD_STATIC4)->SetWindowText(strUnitH);

	GetDlgItem(IDC_CMD_YMETHOD1)->SetWindowText(strTaMethod1);
	GetDlgItem(IDC_CMD_YMETHOD2)->SetWindowText(strTaMethod2);
	GetDlgItem(IDC_CMD_YMETHOD3)->SetWindowText(strTaMethod3);
	GetDlgItem(IDC_CMD_YMETHOD4)->SetWindowText(strTaMethod4);
	GetDlgItem(IDC_CMD_YMETHOD5)->SetWindowText(strTaMethod5);
	GetDlgItem(IDC_CMD_YMETHOD6)->SetWindowText(strTaMethod6);
	GetDlgItem(IDC_WG_CMD_STATIC5)->SetWindowText(strUnitH);
}

void CSeisIBC2012PeriodDlg::OnOK() 
{
	UpdateData(TRUE);

	double dCodeHnX = m_wndHnX.GetEditValue();
	double dCodeHnY = m_wndHnY.GetEditValue();
	m_dCwX = m_wndCwX.GetEditValue();
	m_dCwY = m_wndCwY.GetEditValue();
	m_nNX  = m_wndNX.GetEditValue();
	m_nNY  = m_wndNY.GetEditValue();

	if(!CalcPeriod(dCodeHnX, m_nNX, m_dCwX, m_dPeriodX, m_nMethodX)) return;
	if(!CalcPeriod(dCodeHnY, m_nNY, m_dCwY, m_dPeriodY, m_nMethodY)) return;
		
	CDialogMove::OnOK();
}

BOOL CSeisIBC2012PeriodDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();	
		
	double dCodeHnX = m_dHnX * m_dLengFactCode;
	double dCodeHnY = m_dHnY * m_dLengFactCode;

	m_wndHnX.SetEditUnit(dCodeHnX);
	m_wndHnY.SetEditUnit(dCodeHnY);
	m_wndCwX.SetEditUnit(m_dCwX);
	m_wndCwY.SetEditUnit(m_dCwY);
	m_wndNX.SetEditUnit(m_nNX);
	m_wndNY.SetEditUnit(m_nNY);
	
	CtrlManager();

	SetControlByCode(m_nCode);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSeisIBC2012PeriodDlg::CalcPeriod(double dHn, int nN, double dCw, double &dPeriod,int nMethod)
{  
	if (nMethod == 5 && dCw == 0.0) { return FALSE; }
		
	switch (m_nCode)
	{
	case IBC_E_2009:
	case IBC_E_2012:
		{			
			switch (nMethod)
			{
			case 0: dPeriod = 0.028 * pow(dHn, 0.8); break;
			case 1: dPeriod = 0.016 * pow(dHn, 0.9); break;
			case 2: dPeriod = 0.03 * pow(dHn, 0.75); break;
			case 3: dPeriod = 0.02 * pow(dHn, 0.75); break;
			case 4: dPeriod = 0.1 * nN; break;
			case 5: dPeriod = 0.0019 * dHn / sqrt(dCw); break;
			default: ASSERT(0); return FALSE; 
			}
		}
		break;
	case NSCP_E_2024:
		{
			CSeismic_NSCP2024 CodeCalc;
			dPeriod = CodeCalc.CalcApproximatePeriod(nMethod, dHn, nN, dCw);
		}
		break;
	default:
		{
			ASSERT(0);
			return FALSE;
		}
		break;
	}
		
	return TRUE;
}

void CSeisIBC2012PeriodDlg::SetCode(int nCode)
{
	m_nCode = (nCode == 0) ? IBC_E_2012 : nCode;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) { ASSERT(0); return; }

	const int nCodeLengIndex = [](int nCurCode)
	{
		switch (nCurCode)
		{
		case IBC_E_2009:
		case IBC_E_2012:
			return D_UNITSYS_LENGTH_INDEX_FT;			
		case NSCP_E_2024:
			return D_UNITSYS_LENGTH_INDEX_M;
		default:
			ASSERT(0);
			return D_UNITSYS_LENGTH_INDEX_FT;
		}
	}(m_nCode);

	m_dLengFactCode = pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, nCodeLengIndex, 
		D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 1.0);
}