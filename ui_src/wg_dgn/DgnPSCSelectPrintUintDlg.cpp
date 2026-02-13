// CDgnPSCSelectPrintUnitDlg.cpp: implementation of the CDgnPSCSelectPrintUnitDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnPSCSelectPrintUintDlg.h"
#include "..\wg_cmd\wg_cmd_LoadCombCtrl.h"
#include "..\wg_db\DBLib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnPSCSelectPrintUnitDlg::CDgnPSCSelectPrintUnitDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnPSCSelectPrintUnitDlg::IDD, pParent)
{
	m_nDesignType = EN_MEMBER_PSC;
	//{{AFX_DATA_INIT(CDgnPSCSelectPrintUnitDlg)

	//}}AFX_DATA_INIT
}


void CDgnPSCSelectPrintUnitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnPSCSelectPrintUnitDlg)
	DDX_Control(pDX, IDC_SELECT_UNIT, m_CmbSelUnit);
	DDX_Control(pDX, IDC_SELECT_LANG, m_CmbSelLang);
	//}}AFX_DATA_MAP
}


#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnPSCSelectPrintUnitDlg, CInternationalDlg)
	ON_CBN_SELCHANGE(IDC_SELECT_LANG,     OnSelchangeUnit)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnPSCSelectPrintUnitDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CDgnPSCSelectPrintUnitDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	Initial_Data();

	OnSelchangeUnit();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnPSCSelectPrintUnitDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnPSCSelectPrintUnitDlg::OnOK() 
{
	UpdateData(TRUE);

	
	m_iSelLang = m_CmbSelLang.GetCurSel();

	if (m_iSelLang == 1)
	{
		m_iSelUnit = 2; // 0 : English US // 1 : English SI // 2: Turkey SI
	}
	else
	{
		m_iSelUnit = m_CmbSelUnit.GetCurSel();
	}


	CDialogMove::OnOK();  
}

void CDgnPSCSelectPrintUnitDlg::Initial_Data()
{
	BOOL bTurkish = CDBLib::AllowTurkish();

	if (bTurkish == FALSE)
	{
		GetDlgItem(IDC_WG_DGN_STATIC0)->SetWindowText(_LS(IDS_DGN_PSC_AASHTO_EXCEL_ENG));
		GetDlgItem(IDC_WG_DGN_LANG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SELECT_LANG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WG_DGN_STATIC1)->SetWindowPos(NULL, 140, 82, 18, 14, SWP_NOSIZE);
		GetDlgItem(IDC_SELECT_UNIT)->SetWindowPos(NULL, 180, 82, 36, 12, SWP_NOSIZE);
	}
	else if (bTurkish == TRUE && m_nDesignType==EN_MEMBER_CSG)
	{
		GetDlgItem(IDC_WG_DGN_STATIC0)->SetWindowText(_LS(IDS_DGN_PSC_AASHTO_EXCEL_ENG));
		GetDlgItem(IDC_WG_DGN_LANG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SELECT_LANG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WG_DGN_STATIC1)->SetWindowPos(NULL, 140, 82, 18, 14, SWP_NOSIZE);
		GetDlgItem(IDC_SELECT_UNIT)->SetWindowPos(NULL, 180, 82, 36, 12, SWP_NOSIZE);
	}
	else
	{
		GetDlgItem(IDC_WG_DGN_STATIC0)->SetWindowText(_LS(IDS_DGN_PSC_AASHTO_EXCEL_TUK));
		GetDlgItem(IDC_WG_DGN_LANG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SELECT_LANG)->ShowWindow(SW_SHOW);
	}


	if(m_nDesignType==EN_MEMBER_CSG)	this->SetWindowText(_T("Composite Design Report"));

	m_iSelUnit=0;
	m_iSelLang=0;
	int iIndex=0;
	int jIndex=0;
	
	CString aStrUnit[2] = {_T("US"),_T("SI")};
	int i=0;
	for(i=0; i<2; i++)
	{
		m_CmbSelUnit.AddString(aStrUnit[i]);
	}
	m_CmbSelUnit.SetCurSel(iIndex);

	CString aStrLang[2] = {_T("English"),_T("Turkish")};
	int j=0;
	for(j=0; j<2; j++)
	{
		m_CmbSelLang.AddString(aStrLang[j]);
	}
	m_CmbSelLang.SetCurSel(jIndex);


}

int CDgnPSCSelectPrintUnitDlg::GetSelectUnit()
{
	return m_iSelUnit;
}


void CDgnPSCSelectPrintUnitDlg::SetDesignType(int nDesignType)
{
	m_nDesignType=nDesignType;
}

void CDgnPSCSelectPrintUnitDlg::OnSelchangeUnit()
{
	CString strCode = _T("");

	m_CmbSelLang.GetWindowText(strCode);

	BOOL bEnable = FALSE;

	if (strCode==_T("Turkish"))
	{
		bEnable = FALSE;
		m_CmbSelUnit.SetCurSel(1);
		GetDlgItem(IDC_SELECT_UNIT)->EnableWindow(bEnable);
	}
	else
	{
		bEnable = TRUE;
		m_CmbSelUnit.SetCurSel(0);
		GetDlgItem(IDC_SELECT_UNIT)->EnableWindow(bEnable);
	}
}

