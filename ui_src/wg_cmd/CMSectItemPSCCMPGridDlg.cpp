// CMSectItemPSCCMPGridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCCMPGridDlg.h"
#include "CMSectItemPSC_CMP.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_NewSect.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCCMPGridDlg dialog


CCMSectItemPSCCMPGridDlg::CCMSectItemPSCCMPGridDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemPSCCMPGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCCMPGridDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectItemPSCCMPGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCCMPGridDlg)
	DDX_Control(pDX, IDC_CMD_TAB, m_tab);
	DDX_Control(pDX, IDC_CMD_CMP_JOINT_GRID, m_wndJointGrid);
	DDX_Control(pDX, IDC_CMD_CMP_SIZE_GRID, m_wndSizeGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemPSCCMPGridDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemPSCCMPGridDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB, OnSelchangeCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCCMPGridDlg message handlers

BOOL CCMSectItemPSCCMPGridDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitControls();

	ShowData();
	DecideEnable();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMSectItemPSCCMPGridDlg::SetData(T_SECT_D* pSect,CWnd* pWnd)
{
	m_pSect=pSect;
	m_pWnd=pWnd;
}


void CCMSectItemPSCCMPGridDlg::InitControls()
{
	TC_ITEM item;
	memset(&item, 0, sizeof(TC_ITEM));
	item.mask = TCIF_TEXT;

	const TCHAR* aTitle[] =  {_LS(IDS_CMD_PSC_CMP_LeftSide),
										 _LS(IDS_CMD_PSC_CMP_RightSide)};
	
	int nNum = sizeof(aTitle)/sizeof(const TCHAR*);
	for (int i = 0; i < nNum; i++)
	{
		item.pszText = (LPTSTR)aTitle[i];
		m_tab.InsertItem(i, &item);
	}
	m_wndJointGrid.InitGrid();
	m_wndJointGrid.SetDataSource(m_pSect,this);

	m_wndSizeGrid.InitGrid();
	int index=((CCMSectItemPSC_CMP*)m_pWnd)->GetSelectedTab();
	m_wndSizeGrid.SetDataSource(m_pSect,this,index);
	m_tab.SetCurSel(index);
}
void CCMSectItemPSCCMPGridDlg::ShowData()
{
	
	m_wndJointGrid.ShowData();
	m_wndSizeGrid.ShowData();
	
}
void CCMSectItemPSCCMPGridDlg::DecideEnable()
{
	
	m_wndSizeGrid.DecideEnable();
	m_wndSizeGrid.ShowData();
}

void CCMSectItemPSCCMPGridDlg::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int index=m_tab.GetCurSel();
	m_wndSizeGrid.SetDataSource(m_pSect,this,index);
	m_wndSizeGrid.ShowData();
	
	
	((CCMSectItemPSC_CMP*)m_pWnd)->ChangeTab(index);	
	DecideEnable();
	*pResult = 0;
}


