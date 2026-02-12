// CMSectItemPSCNormalGridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCNormalGridDlg.h"

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
// CCMSectItemPSCNormalGridDlg dialog


CCMSectItemPSCNormalGridDlg::CCMSectItemPSCNormalGridDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemPSCNormalGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCNormalGridDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectItemPSCNormalGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCNormalGridDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_SP_ID_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemPSCNormalGridDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemPSCNormalGridDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCNormalGridDlg message handlers

BOOL CCMSectItemPSCNormalGridDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitControls();

	ShowData();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMSectItemPSCNormalGridDlg::SetData(T_SECT_D* pSect,CWnd* pWnd)
{
	m_pSect=pSect;
	m_pWnd=pWnd;
}


void CCMSectItemPSCNormalGridDlg::InitControls()
{


	m_wndGrid.InitGrid();
	m_wndGrid.SetDataSource(m_pSect,this);

	
	
}
void CCMSectItemPSCNormalGridDlg::ShowData()
{
	m_wndGrid.ShowData();
	
}

