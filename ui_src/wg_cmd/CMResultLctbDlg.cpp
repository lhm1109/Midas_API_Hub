// CMResultLctbDlg.cpp : implementation file
//
// 2004. 02. 16    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMResultLctbDlg.h"

#include "CMResultLctbGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_dbLock\LockOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMResultLctbDlg dialog


CCMResultLctbDlg::CCMResultLctbDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMResultLctbDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMResultLctbDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_wndGrid = new CCMResultLctbGrid();
}

CCMResultLctbDlg::~CCMResultLctbDlg()
{
	if (m_wndGrid)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
}

void CCMResultLctbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMResultLctbDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMResultLctbDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMResultLctbDlg)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnCmdDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMResultLctbDlg message handlers

BOOL CCMResultLctbDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	m_wndGrid->Initialize(pDoc);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMResultLctbDlg::OnCmdDelete() 
{
	// TODO: Add your control notification handler code here
	CArray<CString, CString&> aSelName;
	int nCount = m_wndGrid->GetSelectedNameList(aSelName);
	if (nCount < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

#if defined(_CIVIL_JP)
	if (!pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_STEEL_MEMBER_DESIGN))
	{
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), _LS(IDS_IDD_MAIN_PROTECTION_EX_OPT_CIVILNX_JP_DLG_IDC_LBC_LOCKOPT_CHK7));

		AfxMessageBox(strErrMsg);
		return;
	}
#endif

	pDoc->m_pDataCtrl->DelLctb(aSelName);	
}

void CCMResultLctbDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		m_wndGrid->UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		m_wndGrid->UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}