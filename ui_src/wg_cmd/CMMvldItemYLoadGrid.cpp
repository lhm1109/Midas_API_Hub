// CMMvldItemYLoadGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemYLoadGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemYLoadGrid dialog


CCMMvldItemYLoadGrid::CCMMvldItemYLoadGrid(CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvldItemYLoadGrid::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemYLoadGrid)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMMvldItemYLoadGrid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemYLoadGrid)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvldItemYLoadGrid, CDialog)
	//{{AFX_MSG_MAP(CCMMvldItemYLoadGrid)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemYLoadGrid message handlers
