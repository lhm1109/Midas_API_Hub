// CMBgenTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMBgenTableDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMBgenTableDlg dialog


CCMBgenTableDlg::CCMBgenTableDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMBgenTableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMBgenTableDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arMakeBuilding.RemoveAll();
	m_bGridInit = FALSE;
}


void CCMBgenTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMBgenTableDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_GRID_PLACEHOLDER, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMBgenTableDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMBgenTableDlg)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMBgenTableDlg message handlers

BOOL CCMBgenTableDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	// TODO: Add extra initialization here

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	InitGrid();

	m_wndGrid.MakeItemEx(m_arMakeBuilding);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMBgenTableDlg::InitGrid()
{
//  m_wndGrid.Initialize(&m_arBGenData);
	m_wndGrid.Initialize(CDBDoc::GetDocPoint());
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	m_bGridInit = TRUE;
}

BOOL CCMBgenTableDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
	  if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{
		  if(pMsg->wParam == VK_RETURN)
			if(pMsg->wParam == VK_ESCAPE)
			
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;        
			// DO NOT process further
		}
	}
	return CDialogMove::PreTranslateMessage(pMsg);
}


void CCMBgenTableDlg::OnCmdCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

void CCMBgenTableDlg::OnCmdOk() 
{
	// TODO: Add your control notification handler code here
	T_MAKEBUILDING tempBuildingbase,tempBuildingcomp;
	int tempCopy=1;
	int rowcount = m_wndGrid.GetCount();
	m_arMakeBuilding.RemoveAll();

	//기준값을 설정한다.
	tempBuildingbase.dDistance = _tstof(m_wndGrid.GetValueRowCol(1,1));
	tempBuildingbase.nIncMatl = _tstof(m_wndGrid.GetValueRowCol(1,2));
	tempBuildingbase.nIncColumn = _tstof(m_wndGrid.GetValueRowCol(1,3));
	tempBuildingbase.nIncBeam = _tstof(m_wndGrid.GetValueRowCol(1,4));
	tempBuildingbase.nIncBrace = _tstof(m_wndGrid.GetValueRowCol(1,5));
	tempBuildingbase.nIncWall = _tstof(m_wndGrid.GetValueRowCol(1,6));

	for(int i=1; i<rowcount; i++)
	{
		if(i != rowcount-1)
		{
			tempBuildingcomp.dDistance = _tstof(m_wndGrid.GetValueRowCol(i+1,1));
			tempBuildingcomp.nIncMatl = _tstof(m_wndGrid.GetValueRowCol(i+1,2));
			tempBuildingcomp.nIncColumn = _tstof(m_wndGrid.GetValueRowCol(i+1,3));
			tempBuildingcomp.nIncBeam = _tstof(m_wndGrid.GetValueRowCol(i+1,4));
			tempBuildingcomp.nIncBrace = _tstof(m_wndGrid.GetValueRowCol(i+1,5));
			tempBuildingcomp.nIncWall = _tstof(m_wndGrid.GetValueRowCol(i+1,6));
			
			if((tempBuildingbase.dDistance == tempBuildingcomp.dDistance) &&
				 (tempBuildingcomp.nIncMatl == 0) &&
				 (tempBuildingcomp.nIncColumn == 0) &&
				 (tempBuildingcomp.nIncBeam == 0) &&
				 (tempBuildingcomp.nIncBrace == 0) &&
				 (tempBuildingcomp.nIncWall == 0)) 
				tempCopy++;
			else
			{
				tempBuildingbase.nCopy = tempCopy;
				m_arMakeBuilding.Add(tempBuildingbase);
				tempCopy=1;
				tempBuildingbase.dDistance = tempBuildingcomp.dDistance;
				tempBuildingbase.nIncMatl = tempBuildingcomp.nIncMatl;
				tempBuildingbase.nIncColumn = tempBuildingcomp.nIncColumn;
				tempBuildingbase.nIncBeam = tempBuildingcomp.nIncBeam;
				tempBuildingbase.nIncBrace = tempBuildingcomp.nIncBrace;
				tempBuildingbase.nIncWall = tempBuildingcomp.nIncWall; 
			}
		}
		else
		{
			tempBuildingbase.nCopy = tempCopy;
			m_arMakeBuilding.Add(tempBuildingbase);
			break;
		}
	}
	
	CDialogMove::OnOK();	
}

/*
void CCMBgenTableDlg::OnCmdOk() 
{
	// TODO: Add your control notification handler code here
	T_MAKEBUILDING tempBuildingbase,tempBuildingcomp;
	int tempCopy=1;
	int rowcount = m_wndGrid.GetCount();
	m_arMakeBuilding.RemoveAll();

	//기준값을 설정한다.
	tempBuildingbase.dDistance = _tstof(m_wndGrid.GetValueRowCol(1,1));
	tempBuildingbase.nIncMatl = _tstof(m_wndGrid.GetValueRowCol(1,2));
	tempBuildingbase.nIncColumn = _tstof(m_wndGrid.GetValueRowCol(1,3));
	tempBuildingbase.nIncBeam = _tstof(m_wndGrid.GetValueRowCol(1,4));
	tempBuildingbase.nIncBrace = _tstof(m_wndGrid.GetValueRowCol(1,5));
	tempBuildingbase.nIncWall = _tstof(m_wndGrid.GetValueRowCol(1,6));

	for(int i=1; i<rowcount; i++)
	{
		if(i != rowcount-1)
		{
			tempBuildingcomp.dDistance = _tstof(m_wndGrid.GetValueRowCol(i+1,1));
			tempBuildingcomp.nIncMatl = _tstof(m_wndGrid.GetValueRowCol(i+1,2));
			tempBuildingcomp.nIncColumn = _tstof(m_wndGrid.GetValueRowCol(i+1,3));
			tempBuildingcomp.nIncBeam = _tstof(m_wndGrid.GetValueRowCol(i+1,4));
			tempBuildingcomp.nIncBrace = _tstof(m_wndGrid.GetValueRowCol(i+1,5));
			tempBuildingcomp.nIncWall = _tstof(m_wndGrid.GetValueRowCol(i+1,6));
			
			if((tempBuildingbase.dDistance == tempBuildingcomp.dDistance) &&
				 (tempBuildingbase.nIncMatl == tempBuildingcomp.nIncMatl) &&
				 (tempBuildingbase.nIncColumn == tempBuildingcomp.nIncColumn) &&
				 (tempBuildingbase.nIncBeam == tempBuildingcomp.nIncBeam) &&
				 (tempBuildingbase.nIncBrace == tempBuildingcomp.nIncBrace) &&
				 (tempBuildingbase.nIncWall == tempBuildingcomp.nIncWall)) 
				tempCopy++;
			else
			{
				tempBuildingbase.nCopy = tempCopy;
				m_arMakeBuilding.Add(tempBuildingbase);
				tempCopy=1;
				tempBuildingbase.dDistance = tempBuildingcomp.dDistance;
				tempBuildingbase.nIncMatl = tempBuildingcomp.nIncMatl;
				tempBuildingbase.nIncColumn = tempBuildingcomp.nIncColumn;
				tempBuildingbase.nIncBeam = tempBuildingcomp.nIncBeam;
				tempBuildingbase.nIncBrace = tempBuildingcomp.nIncBrace;
				tempBuildingbase.nIncWall = tempBuildingcomp.nIncWall; 
			}
		}
		else
		{
			tempBuildingbase.nCopy = tempCopy;
			m_arMakeBuilding.Add(tempBuildingbase);
			break;
		}
	}
	
	CDialogMove::OnOK();	
}  */
