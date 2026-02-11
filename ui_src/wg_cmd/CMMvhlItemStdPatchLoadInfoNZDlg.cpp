// CMMvhlItemStdPatchLoadInfoNZDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdPatchLoadInfoNZDlg.h"

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
// CCMMvhlItemStdPatchLoadInfoNZDlg dialog

#define COLCOUNT 3

CCMMvhlItemStdPatchLoadInfoNZDlg::CCMMvhlItemStdPatchLoadInfoNZDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdPatchLoadInfoNZDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdPatchLoadInfoNZDlg)	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
}

CCMMvhlItemStdPatchLoadInfoNZDlg::~CCMMvhlItemStdPatchLoadInfoNZDlg()
{
}

void CCMMvhlItemStdPatchLoadInfoNZDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdPatchLoadInfoNZDlg)  
	DDX_Control(pDX, IDC_CMD_FIRST_GRID, m_wndPatchGrid);

	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdPatchLoadInfoNZDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_wndPatchGrid.InitGrid(110);

	m_wndPatchGrid.SetDataSource(&m_Data, this);
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-------------------------------------------------------------------------
// Implementation

void CCMMvhlItemStdPatchLoadInfoNZDlg::SetData(T_MVHL_D& Data)
{
	m_Data = Data;
}

void CCMMvhlItemStdPatchLoadInfoNZDlg::Data2Dlg()
{
	m_wndPatchGrid.ShowData();

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdPatchLoadInfoNZDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_wndPatchGrid.SaveData();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdPatchLoadInfoNZDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdPatchLoadInfoNZDlg)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdPatchLoadInfoNZDlg message handlers

void CCMMvhlItemStdPatchLoadInfoNZDlg::OnOK()
{
	if (!Dlg2Data()) return;

	CDialogMove::OnOK();
}
