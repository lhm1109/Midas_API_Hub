// CMGildOLiveDlg.cpp : implementation file
//
// 2004. 02. 17    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGildOLiveDlg.h"

#include "CMGildBaseGrid.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGildOLiveDlg dialog


CCMGildOLiveDlg::CCMGildOLiveDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMGildOLiveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGildOLiveDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_Key = 0;
	m_Data.Initialize();  
	m_bModify = FALSE;

	m_wndGrid = NULL;
	m_wndGrid = new CCMGildBaseGrid();
}

CCMGildOLiveDlg::~CCMGildOLiveDlg()
{
	if (m_wndGrid)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
}

void CCMGildOLiveDlg::SetLoadCase(T_GILD_K LoadCaseKey)
{
	m_Key = LoadCaseKey;
	if (m_pDoc->m_pAttrCtrl->GetGild(m_Key, m_Data))
		m_bModify = TRUE;
}

void CCMGildOLiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGildOLiveDlg)
	DDX_Control(pDX, IDC_CMD_NAME_EDIT, m_editName);
	DDX_Control(pDX, IDC_CMD_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGildOLiveDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMGildOLiveDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGildOLiveDlg message handlers

BOOL CCMGildOLiveDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	T_GILC_D GilcD;
	m_pDoc->m_pAttrCtrl->GetGilc(m_Key, GilcD);
	m_editName.SetWindowText(GilcD.LoadCaseName);
	m_editName.SetReadOnly();

	//m_wndGrid->AddColInfo(D_GILD_LOAD_TYPE_COL);
	m_wndGrid->AddColInfo(D_GILD_CENT_LINE_COL);
	m_wndGrid->AddColInfo(D_GILD_GIRD_LINE_COL);
	m_wndGrid->AddColInfo(D_GILD_CROS_LINE_COL);
	m_wndGrid->AddColInfo(D_GILD_LOAD_VALU_COL);
	//m_wndGrid->AddColInfo(D_GILD_LOAD_UNIT_COL);
	m_wndGrid->Initialize(m_pDoc, &m_Data);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGildOLiveDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS))
		return ;//m_wndGrid->CancelEdit();

	m_Data.nType = 4;

	BOOL bSucess;
	if (m_bModify)
		bSucess = m_pDoc->m_pDataCtrl->ModifyGild(m_Key, m_Key, m_Data);
	else 
		bSucess = m_pDoc->m_pDataCtrl->AddGild(m_Key, m_Data);
	if (!bSucess) return;

	CDialogMove::OnOK();
}

void CCMGildOLiveDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

