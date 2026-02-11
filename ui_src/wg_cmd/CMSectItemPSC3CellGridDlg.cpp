// CMSectItemPSC3CellGridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC3CellGridDlg.h"
#include "CMSectItemPSC3Cell.h"

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
// CCMSectItemPSC3CellGridDlg dialog


CCMSectItemPSC3CellGridDlg::CCMSectItemPSC3CellGridDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemPSC3CellGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC3CellGridDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectItemPSC3CellGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC3CellGridDlg)
	DDX_Control(pDX, IDC_CMD_3CELL_TAB, m_tabOption);
	DDX_Control(pDX, IDC_CMD_3CELL_JOINT_GRID, m_wndJointGrid);
	DDX_Control(pDX, IDC_CMD_3CELL_SIZE_GRID, m_wndSizeGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemPSC3CellGridDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemPSC3CellGridDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_3CELL_TAB, OnSelchangeCmd3cellTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC3CellGridDlg message handlers

BOOL CCMSectItemPSC3CellGridDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitControls();

	ShowData();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMSectItemPSC3CellGridDlg::SetData(T_SECT_D* pSect,CWnd* pWnd)
{
	m_pSect=pSect;
	m_pWnd=pWnd;
}


void CCMSectItemPSC3CellGridDlg::InitControls()
{

	TC_ITEM item;
	memset(&item, 0, sizeof(TC_ITEM));
	item.mask = TCIF_TEXT;

	const TCHAR* aTitle[] =  {_LS(IDS_CMD_SECT_ITEM_PSC_DEF), 
										 _LS(IDS_CMD_SECT_ITEM_PSC_OPT)};
	
	int nNum = sizeof(aTitle)/sizeof(const TCHAR*);
	for (int i = 0; i < nNum; i++)
	{
		item.pszText = (LPTSTR)aTitle[i];
		m_tabOption.InsertItem(i, &item);
	}
	m_wndJointGrid.InitGrid();
	m_wndJointGrid.SetDataSource(m_pSect,this);

	m_wndSizeGrid.InitGrid();
	int index=((CCMSectItemPSC3Cell*)m_pWnd)->GetSelectedTab();
	m_wndSizeGrid.SetDataSource(m_pSect,this,index);
	m_tabOption.SetCurSel(index);
}
void CCMSectItemPSC3CellGridDlg::ShowData()
{
	m_wndJointGrid.ShowData();
	m_wndSizeGrid.ShowData();
	DecideEnable();
}
void CCMSectItemPSC3CellGridDlg::DecideEnable()
{
	m_wndJointGrid.DecideEnable();
	m_wndSizeGrid.DecideEnable();
}



void CCMSectItemPSC3CellGridDlg::OnSelchangeCmd3cellTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int index=m_tabOption.GetCurSel();
	ROWCOL i,j;
	m_wndSizeGrid.GetCurrentCell(i,j);
	if(j==4)
		m_wndSizeGrid.SetLeftValidCell(i,j);
	else
		m_wndSizeGrid.SetRightValidCell(i,j);
	m_wndSizeGrid.SetDataSource(m_pSect,this,index);
	m_wndSizeGrid.ShowData();
	
	((CCMSectItemPSC3Cell*)m_pWnd)->ChangeTab(index);	
	DecideEnable();
	*pResult = 0;
}
