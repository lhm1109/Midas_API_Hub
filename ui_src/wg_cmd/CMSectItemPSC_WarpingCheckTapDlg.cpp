// CMSectItemPSC_WarpingCheckTapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC_WarpingCheckTapDlg.h"
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
// CCMSectItemPSC_WarpingCheckTapDlg dialog


CCMSectItemPSC_WarpingCheckTapDlg::CCMSectItemPSC_WarpingCheckTapDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemPSC_WarpingCheckTapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC_WarpingCheckTapDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectItemPSC_WarpingCheckTapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC_WarpingCheckTapDlg)
	DDX_Control(pDX, IDC_CMD_SECT_ITEM_PSC_WARPINGCHECK_GRID, m_wndPointGrid);
	DDX_Control(pDX, IDC_CMD_SECT_ITEM_PSC_WARPINGCHECK_J_GRID, m_wndPointJGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemPSC_WarpingCheckTapDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemPSC_WarpingCheckTapDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_WarpingCheckTapDlg message handlers

BOOL CCMSectItemPSC_WarpingCheckTapDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitControls();

	ShowData();

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCMSectItemPSC_WarpingCheckTapDlg::SetData(T_SECT_D* pSect,CWnd* pWnd)
{
	m_pSect=pSect;
	m_pWnd=pWnd;
}


void CCMSectItemPSC_WarpingCheckTapDlg::InitControls()
{
	m_wndPointGrid.InitGrid();
	m_wndPointGrid.SetDataSource(m_pSect,this, TRUE);

	m_wndPointJGrid.InitGrid();
	m_wndPointJGrid.SetDataSource(m_pSect,this);
}
void CCMSectItemPSC_WarpingCheckTapDlg::ShowData()
{
	m_wndPointGrid.ShowData();
	m_wndPointJGrid.ShowData();
}

