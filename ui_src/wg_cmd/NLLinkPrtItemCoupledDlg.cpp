// NLLinkPrtItemCoupledDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "NLLinkPrtItemCoupledDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLSIZE   21
/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtItemCoupledDlg dialog


CNLLinkPrtItemCoupledDlg::CNLLinkPrtItemCoupledDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CNLLinkPrtItemCoupledDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLLinkPrtItemCoupledDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_arLinkType.Add(IDC_CMD_NLLINK_S1_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_S2_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_S3_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_S4_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_S5_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_S6_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T01_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T02_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T03_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T04_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T05_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T06_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T07_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T08_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T09_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T10_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T11_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T12_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T13_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T14_EDIT);
	m_arLinkType.Add(IDC_CMD_NLLINK_T15_EDIT);

	m_arDLinkType.Add(IDC_CMD_NLLINK_B01_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B02_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B03_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B04_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B05_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B06_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B07_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B08_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B09_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B10_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B11_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B12_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B13_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B14_EDIT);
	m_arDLinkType.Add(IDC_CMD_NLLINK_B15_EDIT);

	m_bInit = TRUE;
}


void CNLLinkPrtItemCoupledDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLLinkPrtItemCoupledDlg)
	DDX_Control(pDX, IDC_CMD_NLLINK_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLLinkPrtItemCoupledDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNLLinkPrtItemCoupledDlg)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T01_EDIT, OnChangeCmd1Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T02_EDIT, OnChangeCmd2Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T03_EDIT, OnChangeCmd3Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T04_EDIT, OnChangeCmd4Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T05_EDIT, OnChangeCmd5Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T06_EDIT, OnChangeCmd6Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T07_EDIT, OnChangeCmd7Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T08_EDIT, OnChangeCmd8Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T09_EDIT, OnChangeCmd9Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T10_EDIT, OnChangeCmd10Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T11_EDIT, OnChangeCmd11Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T12_EDIT, OnChangeCmd12Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T13_EDIT, OnChangeCmd13Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T14_EDIT, OnChangeCmd14Edit)
	ON_EN_CHANGE(IDC_CMD_NLLINK_T15_EDIT, OnChangeCmd15Edit)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_NLLINK_TAB, OnSelchangeCmdTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_CMD_NLLINK_TAB, OnSelchangingCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtItemCoupledDlg message handlers

BOOL CNLLinkPrtItemCoupledDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_Tab.InsertItem(0, _LS(IDS_CMD_STIFFNESS));
	m_Tab.InsertItem(1, _LS(IDS_CMD_DAMPING));
	
	for(int i=0; i<m_arLinkType.GetSize(); i++)
		((CEdit *)GetDlgItem(m_arLinkType.GetAt(i)))->SetWindowText(_T("0"));
	
	m_Tab.SetCurSel(0);
	
	SetData(0);

	m_bInit = FALSE;
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNLLinkPrtItemCoupledDlg::OnChangeCmd1Edit()  {	ChangeLinkNumber(7);  }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd2Edit()  {	ChangeLinkNumber(8);  }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd3Edit()  {	ChangeLinkNumber(9);  }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd4Edit()  {	ChangeLinkNumber(10); }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd5Edit()  {	ChangeLinkNumber(11); }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd6Edit()  {	ChangeLinkNumber(12); }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd7Edit()  {	ChangeLinkNumber(13); }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd8Edit()  {	ChangeLinkNumber(14); }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd9Edit()  {	ChangeLinkNumber(15); }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd10Edit() {	ChangeLinkNumber(16); }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd11Edit() {	ChangeLinkNumber(17); }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd12Edit() {	ChangeLinkNumber(18); }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd13Edit() {	ChangeLinkNumber(19); }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd14Edit() {	ChangeLinkNumber(20); }
void CNLLinkPrtItemCoupledDlg::OnChangeCmd15Edit() {	ChangeLinkNumber(21); }

void CNLLinkPrtItemCoupledDlg::ChangeLinkNumber(int nEditNum)
{
	CString strTemp;
	((CEdit *)GetDlgItem(m_arLinkType.GetAt(nEditNum-1)))->GetWindowText(strTemp);
	((CEdit *)GetDlgItem(m_arDLinkType.GetAt(nEditNum-7)))->SetWindowText(strTemp);
}

void CNLLinkPrtItemCoupledDlg::OnSelchangingCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nSel = m_Tab.GetCurSel();
	GetData(nSel);
	
	*pResult = 0;
}

void CNLLinkPrtItemCoupledDlg::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	UINT ctrl[2][6] = {{IDC_CMD_NLLINK_SDX_STATIC, IDC_CMD_NLLINK_SDY_STATIC, IDC_CMD_NLLINK_SDZ_STATIC, IDC_CMD_NLLINK_SRX_STATIC, IDC_CMD_NLLINK_SRY_STATIC, IDC_CMD_NLLINK_SRZ_STATIC},
										 {IDC_CMD_NLLINK_SDX_STATIC2, IDC_CMD_NLLINK_SDY_STATIC2, IDC_CMD_NLLINK_SDZ_STATIC2, IDC_CMD_NLLINK_SRX_STATIC2, IDC_CMD_NLLINK_SRY_STATIC2, IDC_CMD_NLLINK_SRZ_STATIC2}};
	CString strStif[] = {_LSX(SDx), _LSX(SDy), _LSX(SDz), _LSX(SRx), _LSX(SRy), _LSX(SRz)};
	CString strDamp[] = {_LSX(cx),  _LSX(cy),  _LSX(cz),  _LSX(rcx), _LSX(rcy), _LSX(rcz)};

	int nSel = m_Tab.GetCurSel();
	CString* pStr;
	if (nSel == 0)
	{
		pStr = strStif;
	}
	else if (nSel == 1)
	{
		pStr = strDamp;
	}
	else
	{
		ASSERT(0);
		pStr = strStif;
	}

	for (int i=0; i<6; i++)
	{
		GetDlgItem(ctrl[0][i])->SetWindowText(pStr[i]);
		GetDlgItem(ctrl[1][i])->SetWindowText(pStr[i]);
	}
	SetData(nSel);
	*pResult = 0;
}

void CNLLinkPrtItemCoupledDlg::GetData(int nTabIndex)
{
	double* pData;
	if( nTabIndex == 0 )
		pData = m_dStif;
	else if( nTabIndex == 1 )
		pData = m_dDamp;
	else
	{
		ASSERT(0);
		// Nothing
	}

	CString strTemp;
	for(int i=0; i<m_arLinkType.GetSize(); i++)
	{
		((CEdit *)GetDlgItem(m_arLinkType.GetAt(i)))->GetWindowText(strTemp);
		pData[i] = strTemp.IsEmpty() ? 0 : _tstof(strTemp); 
	}
}

void CNLLinkPrtItemCoupledDlg::SetData(int nTabIndex)
{
	double* pData;
	if( nTabIndex == 0 )
		pData = m_dStif;
	else if( nTabIndex == 1 )
		pData = m_dDamp;
	else
	{
		ASSERT(0);
		// Nothing
	}

	// Disable
	BOOL bUse = TRUE;
	if (nTabIndex == 0)
	{
		if (m_nAppType == 0 && m_nType == 1) bUse = FALSE; 
	}
	else if(nTabIndex == 1)
	{
		if (m_nAppType == 0 && m_nType == 0) bUse = FALSE;
	}
	   
	CString strTemp;
	for (int i=0; i<m_arLinkType.GetSize(); i++)
	{
		strTemp.Format(_T("%g"), pData[i]);	
		((CEdit *)GetDlgItem(m_arLinkType.GetAt(i)))->SetWindowText(strTemp);

		CDlgUtil::CtrlEnableDisable(this, m_arLinkType[i], bUse);
	}
}

void CNLLinkPrtItemCoupledDlg::OnOK() 
{
	GetData(m_Tab.GetCurSel());	
	CDialogMove::OnOK();
}