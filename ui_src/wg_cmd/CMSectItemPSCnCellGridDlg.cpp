// CMSectItemPSCnCellGridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCnCellGridDlg.h"
#include "CMSectItemPSCnCell.h"


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
// CCMSectItemPSCnCellGridDlg dialog


CCMSectItemPSCnCellGridDlg::CCMSectItemPSCnCellGridDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemPSCnCellGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCnCellGridDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectItemPSCnCellGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCnCellGridDlg)
	DDX_Control(pDX, IDC_CMD_TAB_CTRL, m_wndTab);
	DDX_Control(pDX, IDC_CMD_SP_ID_DOWN_GRID, m_wndDownGrid);
	DDX_Control(pDX, IDC_CMD_SP_ID_UP_GRID, m_wndUpGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemPSCnCellGridDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemPSCnCellGridDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB_CTRL, OnSelchangeCmdTabCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMSectItemPSCnCellGridDlg::SetData(T_SECT_D* pSect,CWnd* pWnd)
{
	m_pSect=pSect;
	m_pWnd=pWnd;
}


void CCMSectItemPSCnCellGridDlg::InitControls()
{

	TC_ITEM item;
	memset(&item, 0, sizeof(TC_ITEM));
	item.mask = TCIF_TEXT;

	const TCHAR* aTitle[] =  {(LPCTSTR)_LS(IDS_CMD_PSC_NCELL_LEFT),
										 (LPCTSTR)_LS(IDS_CMD_PSC_NCELL_RIGHT)};
	
	int nNum = sizeof(aTitle)/sizeof(const TCHAR*);
	for (int i = 0; i < nNum; i++)
	{
		item.pszText = (LPTSTR)aTitle[i];
		m_wndTab.InsertItem(i, &item);
	}
	m_wndDownGrid.InitGrid();
	m_wndDownGrid.SetDataSource(m_pSect,this);

	m_wndUpGrid.InitGrid();
	m_wndUpGrid.SetDataSource(m_pSect,this);

	
}
void CCMSectItemPSCnCellGridDlg::ShowData()
{
	m_wndDownGrid.ShowData();
	m_wndUpGrid.ShowData();
 
}


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCellGridDlg message handlers

void CCMSectItemPSCnCellGridDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialogMove::OnOK();
}

void CCMSectItemPSCnCellGridDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

BOOL CCMSectItemPSCnCellGridDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitControls();

	ShowData();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CCMSectItemPSCnCellGridDlg::OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_wndDownGrid.SetLeftRight(m_wndTab.GetCurSel());
	m_wndDownGrid.ShowData();
	int index=m_wndTab.GetCurSel();
	((CCMSectItemPSCnCell*)m_pWnd)->ChangeTab(index);
}
