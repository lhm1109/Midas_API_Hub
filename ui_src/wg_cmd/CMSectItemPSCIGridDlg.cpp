// CMSectItemPSCIGridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCIGridDlg.h"
#include "CMSectItemPSC_I.h"

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
// CCMSectItemPSCIGridDlg dialog


CCMSectItemPSCIGridDlg::CCMSectItemPSCIGridDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemPSCIGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCIGridDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectItemPSCIGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCIGridDlg)
	DDX_Control(pDX, IDC_CMD_PSC_JOINT_GRID, m_wndJointGrid);
	DDX_Control(pDX, IDC_CMD_PSC_SIZE_GRID, m_wndSizeGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemPSCIGridDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemPSCIGridDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCIGridDlg message handlers

BOOL CCMSectItemPSCIGridDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitControls();

	ShowData();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMSectItemPSCIGridDlg::SetData(T_SECT_D* pSect,CWnd* pWnd)
{
	m_pSect=pSect;
	m_pWnd=pWnd;
}


void CCMSectItemPSCIGridDlg::InitControls()
{	
	m_wndJointGrid.InitGrid();
	m_wndJointGrid.SetDataSource(m_pSect,this);

	m_wndSizeGrid.InitGrid();
	m_wndSizeGrid.SetDataSource(m_pSect,this);
}
void CCMSectItemPSCIGridDlg::ShowData()
{
	m_wndJointGrid.ShowData();
	m_wndSizeGrid.ShowData();
	
}
void CCMSectItemPSCIGridDlg::DecideEnable()
{
	m_wndJointGrid.DecideEnable();
	m_wndSizeGrid.DecideEnable();
	ShowData();
}


