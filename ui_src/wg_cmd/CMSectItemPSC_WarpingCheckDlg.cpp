// CMSectItemPSC_WarpingCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC_WarpingCheckDlg.h"
//#include "CMSectItemPSC_I.h"

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
// CCMSectItemPSC_WarpingCheckDlg dialog


CCMSectItemPSC_WarpingCheckDlg::CCMSectItemPSC_WarpingCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemPSC_WarpingCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC_WarpingCheckDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectItemPSC_WarpingCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC_WarpingCheckDlg)
	DDX_Control(pDX, IDC_CMD_SECT_ITEM_PSC_WARPINGCHECK_GRID, m_wndPointGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemPSC_WarpingCheckDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemPSC_WarpingCheckDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_WarpingCheckDlg message handlers

BOOL CCMSectItemPSC_WarpingCheckDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitControls();

	ShowData();

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCMSectItemPSC_WarpingCheckDlg::SetData(T_SECT_D* pSect,CWnd* pWnd)
{
	m_pSect=pSect;
	m_pWnd=pWnd;
}


void CCMSectItemPSC_WarpingCheckDlg::InitControls()
{
	m_wndPointGrid.InitGrid();
	m_wndPointGrid.SetDataSource(m_pSect,this);
}
void CCMSectItemPSC_WarpingCheckDlg::ShowData()
{
	m_wndPointGrid.ShowData();
}
// void CCMSectItemPSC_WarpingCheckDlg::DecideEnable()
// {
//   m_wndPointGrid.DecideEnable();
//   ShowData();
// }


