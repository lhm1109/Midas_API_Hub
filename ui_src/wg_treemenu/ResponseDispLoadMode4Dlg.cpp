// BndrSospIntegral.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ResponseDispLoadMode4Dlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode4Dlg dialog


ResponseDispLoadMode4Dlg::ResponseDispLoadMode4Dlg(CWnd* pParent /*=NULL*/)
	: CStldNbofDlg(pParent)
{

}


void ResponseDispLoadMode4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CStldNbofDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ResponseDispLoadMode4Dlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ResponseDispLoadMode4Dlg, CStldNbofDlg)
	//{{AFX_MSG_MAP(ResponseDispLoadMode4Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode4Dlg message handlers
BOOL ResponseDispLoadMode4Dlg::IsAdd2ListCase(const T_STLD_D &Stld)
{    
		return Stld.LoadCaseType == _T("E");
}
BOOL ResponseDispLoadMode4Dlg::OnInitDialog() 
{
		CStldNbofDlg::OnInitDialog();
		GetDlgItem(IDC_TM_NBOF_NMAS_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_NBOF_LTMS_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_NBOF_STMS_CHK)->EnableWindow(FALSE);
		m_LoadCase.AddStaticIncludeType(_T("E"), FALSE);
		m_LoadCase.AddStaticIncludeType(_T("USER"), FALSE);
		m_LoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);    
		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}