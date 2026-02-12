// CMSectItemPSCMidGridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCMidGridDlg.h"
#include "CMSectItemPSCMid.h"

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
// CCMSectItemPSCMidGridDlg dialog


CCMSectItemPSCMidGridDlg::CCMSectItemPSCMidGridDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemPSCMidGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCMidGridDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectItemPSCMidGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCMidGridDlg)
	DDX_Control(pDX, IDC_CMD_PSC_JOINT_GRID, m_wndJointGrid);
	DDX_Control(pDX, IDC_CMD_PSC_SIZE_GRID, m_wndSizeGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemPSCMidGridDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemPSCMidGridDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCMidGridDlg message handlers

BOOL CCMSectItemPSCMidGridDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitControls();

	ShowData();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMSectItemPSCMidGridDlg::SetData(T_SECT_D* pSect,CWnd* pWnd)
{
	m_pSect=pSect;
	m_pWnd=pWnd;
}


void CCMSectItemPSCMidGridDlg::InitControls()
{	
	m_wndJointGrid.InitGrid();
	m_wndJointGrid.SetDataSource(m_pSect,this);

	m_wndSizeGrid.InitGrid();
	m_wndSizeGrid.SetDataSource(m_pSect,this);
}
void CCMSectItemPSCMidGridDlg::ShowData()
{
	m_wndJointGrid.ShowData();
	m_wndSizeGrid.ShowData();
	
}
void CCMSectItemPSCMidGridDlg::DecideEnable()
{
	m_wndJointGrid.DecideEnable();
	m_wndSizeGrid.DecideEnable();
	ShowData();
}


