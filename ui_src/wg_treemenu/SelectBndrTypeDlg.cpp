// SelectBndrTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SelectBndrTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectBndrTypeDlg dialog


CSelectBndrTypeDlg::CSelectBndrTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSelectBndrTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectBndrTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectBndrTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectBndrTypeDlg)
	DDX_Control(pDX, IDC_BNDR_TYPE_LIST, m_wndBndrTypeList);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CSelectBndrTypeDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CSelectBndrTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectBndrTypeDlg message handlers

BOOL CSelectBndrTypeDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CArray<int, int> aItemChecked;
	DataExchange(aItemChecked, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectBndrTypeDlg::OnOK() 
{
	// TODO: Add extra validation here
	DataExchange(m_TypeArray, 1);
	CDialogMove::OnOK();
}

void CSelectBndrTypeDlg::GetTypes(CArray<int, int>& Types)
{
	Types.RemoveAll();
	Types.Copy(m_TypeArray);
}

void CSelectBndrTypeDlg::DataExchange(CArray<int, int>& aItemChecked, int nOperation)
{
	struct T_LIST_BOX_ITEM
	{
		CString str;
		int nCmd;
		int nMask;
	};
	// 절점/요소에 적용되는 경계조건이 아니면(STDG) 뺀다.
	T_LIST_BOX_ITEM aListBoxItem[] = {
		{_LS(IDS_TM_DT_CONS), LT_CONS_CMD, D_PROD_GEN+D_PROD_CVL+D_PROD_TOW},
		{_LS(IDS_TM_DT_NSPR), LT_NSPR_CMD, D_PROD_GEN+D_PROD_CVL+D_PROD_TOW},
		{_LS(IDS_TM_DT_GSPR), LT_GSPR_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_ELNK), LT_ELNK_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_NLNK), LT_NLNK_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_FRLS), LT_FRLS_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_OFFS), LT_OFFS_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_PRLS), LT_PRLS_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_RIGD), LT_RIGD_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_MCON), LT_MCON_CMD, D_PROD_GEN+D_PROD_CVL},
		//{IDS_TM_DT_STDG, LT_STDG_CMD, D_PROD_GEN},
		{_LS(IDS_TM_DT_HECB), LT_HECB_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_HSPT), LT_HSPT_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_EWSF), LT_EWSF_CMD,            D_PROD_CVL},
	};

	int nMask = 0;
#if defined(_MGEN)
	nMask = D_PROD_GEN;
#elif defined(_CIVIL)
	nMask = D_PROD_CVL;
#elif defined(_TOWER)
	nMask = D_PROD_TOW;
#else
	#error _T("Unknown Product Definition!")
#endif

	aItemChecked.RemoveAll();

	int i, nCount, ix=0;
	nCount = sizeof(aListBoxItem)/sizeof(T_LIST_BOX_ITEM);
	for(i=0; i < nCount; i++)
	{
		if ((nMask & aListBoxItem[i].nMask) == 0) continue;
		if (nOperation == 0)
		{
			ix = m_wndBndrTypeList.AddString(aListBoxItem[i].str);
			m_wndBndrTypeList.SetCheck(ix, 1);
		}
		else
		{
			if (m_wndBndrTypeList.GetCheck(ix++) != 0)
				aItemChecked.Add(aListBoxItem[i].nCmd);
		}
	}
}
