// CMMvhlItemStdPatchLoadInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdPatchLoadInfoDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\SpecialChar.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "CMMvhlStdEuroBSNewTempForRefactor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdPatchLoadInfoDlg dialog

#define COLCOUNT 3

CCMMvhlItemStdPatchLoadInfoDlg::CCMMvhlItemStdPatchLoadInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdPatchLoadInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdPatchLoadInfoDlg)	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
}

CCMMvhlItemStdPatchLoadInfoDlg::~CCMMvhlItemStdPatchLoadInfoDlg()
{
}

void CCMMvhlItemStdPatchLoadInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdPatchLoadInfoDlg)  
	DDX_Control(pDX, IDC_CMD_FIRST_GRID, m_wndPatchGrid);

	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdPatchLoadInfoDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_wndPatchGrid.InitGrid(110);

	m_wndPatchGrid.SetDataSource(&m_Data, this, CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(m_Data, 0));
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-------------------------------------------------------------------------
// Implementation

void CCMMvhlItemStdPatchLoadInfoDlg::SetData(T_MVHL_D& Data)
{
	m_Data = Data;
}

void CCMMvhlItemStdPatchLoadInfoDlg::Data2Dlg()
{
	m_wndPatchGrid.ShowData();

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdPatchLoadInfoDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_wndPatchGrid.SaveData();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdPatchLoadInfoDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdPatchLoadInfoDlg)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdPatchLoadInfoDlg message handlers

void CCMMvhlItemStdPatchLoadInfoDlg::OnOK() 
{
	if (!Dlg2Data()) return;

	CDialogMove::OnOK();
}
