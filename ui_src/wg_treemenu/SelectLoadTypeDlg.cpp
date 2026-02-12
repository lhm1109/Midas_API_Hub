// SelectLoadTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SelectLoadTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectLoadTypeDlg dialog


CSelectLoadTypeDlg::CSelectLoadTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSelectLoadTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectLoadTypeDlg)
	//}}AFX_DATA_INIT
}


void CSelectLoadTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectLoadTypeDlg)
	DDX_Control(pDX, IDC_LOAD_TYPE_LIST, m_wndLoadTypeList);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CSelectLoadTypeDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CSelectLoadTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectLoadTypeDlg message handlers

BOOL CSelectLoadTypeDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CArray<int, int> aItemChecked;
	DataExchange(aItemChecked, 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectLoadTypeDlg::OnOK() 
{
	DataExchange(m_TypeArray, 1);
	
	CDialog::OnOK();
}

void CSelectLoadTypeDlg::GetTypes(CArray<int, int>& Types)
{
	Types.RemoveAll();
	Types.Copy(m_TypeArray);
}

void CSelectLoadTypeDlg::DataExchange(CArray<int, int>& aItemChecked, int nOperation)
{
	struct T_LIST_BOX_ITEM
	{
		CString str;
		int nCmd;
		int nMask;
	};
	// 절점/요소에 적용되는 하중이 아니면(BODF, STMP, TDPL) 뺀다.
	T_LIST_BOX_ITEM aListBoxItem[] = {
		//{_LS(IDS_TM_DT_BODF), LT_BODF_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_CNLD), LT_CNLD_CMD, D_PROD_GEN+D_PROD_CVL+D_PROD_TOW},
		{_LS(IDS_TM_DT_SDSP), LT_SDSP_CMD, D_PROD_GEN+D_PROD_CVL+D_PROD_TOW},
		{_LS(IDS_TM_DT_BMLD), LT_BMLD_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_FBLA), LT_FBLA_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_FMLD), LT_FMLD_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_PRES), LT_PRES_CMD, D_PROD_GEN+D_PROD_CVL},
		//{_LS(IDS_TM_DT_PNLA), LT_PNLA_CMD, D_PROD_GEN+D_PROD_CVL},
		//{_LS(IDS_TM_DT_STMP), LT_STMP_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_NTMP), LT_NTMP_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_ETMP), LT_ETMP_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_GTMP), LT_GTMP_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_BTMP), LT_BTMP_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_PTMP), LT_PTMP_CMD, D_PROD_CVL},
		{_LS(IDS_TM_DT_PRST), LT_PRST_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_PTNS), LT_PTNS_CMD, D_PROD_GEN+D_PROD_CVL},
		//{_LS(IDS_TM_DT_TDPL), LT_TDPL_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_TMLD), LT_TMLD_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_CRPC), LT_CRPC_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_STBK), LT_STBK_CMD, D_PROD_CVL},
		{_LS(IDS_TM_DT_ULCB), LT_ULCB_CMD, D_PROD_CVL},
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

		BOOL bFlag = TRUE;
		if(aListBoxItem[i].str == _LS(IDS_TM_DT_FMLD))
		{
			// RUS에서는 마감하중 제외
			#if defined(_RUS)
				bFlag = FALSE;
			#endif
		}

		if(bFlag)
		{
			if (nOperation == 0)
			{
				ix = m_wndLoadTypeList.AddString(aListBoxItem[i].str);
				m_wndLoadTypeList.SetCheck(ix, 1);
			}
			else
			{
				if (m_wndLoadTypeList.GetCheck(ix++) != 0)
					aItemChecked.Add(aListBoxItem[i].nCmd);
			}
		}
	}
}
