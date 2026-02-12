// CMGildCLiveDlg.cpp : implementation file
//
// 2004. 02. 17    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGildCLiveDlg.h"

#include "CMGildBaseGrid.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGildCLiveDlg dialog


CCMGildCLiveDlg::CCMGildCLiveDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMGildCLiveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGildCLiveDlg)
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

CCMGildCLiveDlg::~CCMGildCLiveDlg()
{
	if (m_wndGrid)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
}

void CCMGildCLiveDlg::SetLoadCase(T_GILD_K LoadCaseKey)
{
	m_Key = LoadCaseKey;
	if (m_pDoc->m_pAttrCtrl->GetGild(m_Key, m_Data)) 
		m_bModify = TRUE;
}

void CCMGildCLiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGildCLiveDlg)
	DDX_Control(pDX, IDC_CMD_NAME_EDIT, m_editName);
	DDX_Control(pDX, IDC_CMD_TYPE_COBX, m_cobxType);
	DDX_Control(pDX, IDC_CMD_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGildCLiveDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMGildCLiveDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGildCLiveDlg message handlers

BOOL CCMGildCLiveDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	T_GILC_D GilcD;
	m_pDoc->m_pAttrCtrl->GetGilc(m_Key, GilcD);
	m_editName.SetWindowText(GilcD.LoadCaseName);
	m_editName.SetReadOnly();

	InitTypeCombo();

	m_wndGrid->AddColInfo(D_GILD_CENT_LINE_COL);
	m_wndGrid->AddColInfo(D_GILD_GIRD_LINE_COL);
	m_wndGrid->AddColInfo(D_GILD_CROS_LINE_COL);
	m_wndGrid->Initialize(m_pDoc, &m_Data);
	
	if (m_bModify)
	{
		int nCount = m_cobxType.GetCount();
		int i = 0;
		for (i=0; i<nCount; i++)
			if (m_Data.nCrowdedLoadType == m_cobxType.GetItemData(i)) break;
		if (i == nCount) ASSERT(0);
		m_cobxType.SetCurSel(i);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGildCLiveDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS))
		return ;//m_wndGrid->CancelEdit();

	m_Data.nType = 3;

	int ix = m_cobxType.GetCurSel();
	if (ix == CB_ERR)
	{
		AfxMessageBox(_LS(IDS_CMD_GILD_ERROR_VEHICLE_LOAD_TYPE));
		return ;
	}
	m_Data.nCrowdedLoadType = m_cobxType.GetItemData(ix);

	BOOL bSucess;
	if (m_bModify)
		bSucess = m_pDoc->m_pDataCtrl->ModifyGild(m_Key, m_Key, m_Data);
	else 
		bSucess = m_pDoc->m_pDataCtrl->AddGild(m_Key, m_Data);
	if (!bSucess) return;

	CDialogMove::OnOK();
}

void CCMGildCLiveDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMGildCLiveDlg::InitTypeCombo()
{
	CString aType[] = { _LS(IDS_CMD_GILD_JAPAN_HIGHWAY)/*, _LS(IDS_CMD_GILD_HOMSHU_SHIKOKU) */};
	// pig UINT aData[] = { 0, 1, 2, 3};
	//UINT aData[] = { 0, 1};
	UINT aData[] = { 0};

	int ix;
	m_cobxType.ResetContent();
	int nNum = sizeof(aData) / sizeof(UINT);
	for (int i=0; i<nNum; i++)
	{
		ix = m_cobxType.AddString(aType[i]);
		m_cobxType.SetItemData(ix, aData[i]);
	}
	m_cobxType.SetCurSel(0);
}
