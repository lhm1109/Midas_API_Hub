// CMSectItemPSC4CellGridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC4CellGridDlg.h"
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
// CCMSectItemPSC4CellGridDlg dialog


CCMSectItemPSC4CellGridDlg::CCMSectItemPSC4CellGridDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemPSC4CellGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC4CellGridDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectItemPSC4CellGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC4CellGridDlg)
	DDX_Control(pDX, IDC_CMD_4CELL_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemPSC4CellGridDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemPSC4CellGridDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC4CellGridDlg message handlers

BOOL CCMSectItemPSC4CellGridDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitControls();

	ShowData();
	DecideEnable();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMSectItemPSC4CellGridDlg::SetData(T_SECT_D* pSect,CWnd* pWnd)
{
	m_pSect=pSect;
	m_pWnd=pWnd;
}


void CCMSectItemPSC4CellGridDlg::InitControls()
{

	m_wndGrid.InitGrid();
	m_wndGrid.SetDataSource(m_pSect,this);

 
}
void CCMSectItemPSC4CellGridDlg::ShowData()
{
	m_wndGrid.ShowData();  
}
void CCMSectItemPSC4CellGridDlg::DecideEnable()
{
	m_wndGrid.DecideEnable();
}
