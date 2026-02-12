// CMLoadLineListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadLineListDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h" 
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h" 


#include "CMLoadLineItemDlg.h"
#include "ExportFunc.h"
#include "CMGridCopyLoadLineDlg.h"
#include "CMGridImportDxfDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_NUM_COL     3
/////////////////////////////////////////////////////////////////////////////
// CCMLoadLineListDlg dialog


CCMLoadLineListDlg::CCMLoadLineListDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLoadLineListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadLineListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_pWndChild = NULL;
}


void CCMLoadLineListDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadLineListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMLoadLineListDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMLoadLineListDlg)
	ON_BN_CLICKED(IDC_CMD_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_MODIFY, OnCmdModify)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnCmdDelete)
	ON_BN_CLICKED(IDC_CMD_COPY, OnCmdCopy)
	ON_BN_CLICKED(IDC_CMD_IMPORT, OnCmdImport)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadLineListDlg message handlers

void CCMLoadLineListDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		UpdateUnit();
		break;
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
	case D_UPDATE_EXE_START: 
	case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		DestroyWindow();
		break;
	default:
		break;
	}
}

void CCMLoadLineListDlg::UpdateUnit()
{
	m_wndGrid.UpdateUnit();
}

void CCMLoadLineListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	BOOL bLdlnChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_LDLN_ADD):
		case(UR_LDLN_DEL):
		case(UR_LDLN_MFD):
		case(UR_LDLN_MFS):
			bLdlnChanged = TRUE;
			break;
		default:
			break;
		}
		//if (bMgdrChanged)
		//  break;
	} // end of while

	if (bLdlnChanged) m_wndGrid.MakeItemEx(); 
}

void CCMLoadLineListDlg::OnCmdAdd() 
{
	CCMLoadLineItemDlg* pWndChild = NULL;
	pWndChild = (CCMLoadLineItemDlg*)CreateOrActivateDlg(m_pDoc, CCMLoadLineItemDlg::IDD, this);
	ASSERT(pWndChild);
	pWndChild->SetModify(FALSE);

	/*
	if (m_pWndChild == NULL)
		m_pWndChild = (CCMLoadLineItemDlg*)CreateOrActivateDlg(m_pDoc, CCMLoadLineItemDlg::IDD, this);
	m_pWndChild->SetModify(FALSE);
	*/
}

void CCMLoadLineListDlg::OnCmdModify()
{
	CArray<UINT, UINT> aSelKey;
	int nCount = m_wndGrid.GetSelectedKeyList(aSelKey);
	if (nCount <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_LOAD_LINE_NO_SELECTED));
		return;
	}
	if (nCount > 1)
	{
		AfxMessageBox(_LS(IDS_CMD_LOAD_LINE_MORE_SELECTED));
		return;
	}
	
	CCMLoadLineItemDlg* pWndChild = NULL;
	pWndChild = (CCMLoadLineItemDlg*)CreateOrActivateDlg(m_pDoc, CCMLoadLineItemDlg::IDD, this);
	ASSERT(pWndChild);
	pWndChild->SetInitModify(aSelKey[0]);
	pWndChild->SetModify(TRUE);

	/*
	if (m_pWndChild == NULL)
		m_pWndChild = (CCMLoadLineItemDlg*)CreateOrActivateDlg(m_pDoc, CCMLoadLineItemDlg::IDD, this);
	
	m_pWndChild->SetInitModify(aSelKey[0]);
	m_pWndChild->SetModify(TRUE);
	*/
}

void CCMLoadLineListDlg::OnCmdDelete()
{
	CArray<UINT, UINT> aSelKey;
	int nCount = m_wndGrid.GetSelectedKeyList(aSelKey);
	if (nCount <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD_LOAD_LINE_NO_SELECTED));
		return;
	}

	T_LDLN_D data;
	CArray<CString, CString&> aSelName;
	for (int i=0; i<aSelKey.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetLdln(aSelKey[i], data);
		aSelName.Add(data.Name);
	}
	m_pDoc->m_pDataCtrl->DelLdln(aSelName);
}

void CCMLoadLineListDlg::OnCmdCopy()
{
	CCMGridCopyLoadLineDlg dlg;
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
}

void CCMLoadLineListDlg::OnCmdImport()
{
	/**
	CCMGridImportDxfDlg dlg;
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
	**/
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CreateOrActivateDlg(pDoc, CCMGridImportDxfDlg::IDD, this);
}

void CCMLoadLineListDlg::OnClose()
{
	DestroyWindow();
}

BOOL CCMLoadLineListDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitGrid();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadLineListDlg::InitGrid()
{
	m_wndGrid.Initialize(m_pDoc);
}

void CCMLoadLineListDlg::OnLButtonDblClkOnGrid(UINT key)
{
	CCMLoadLineItemDlg* pWndChild = NULL;
	pWndChild = (CCMLoadLineItemDlg*)CreateOrActivateDlg(m_pDoc, CCMLoadLineItemDlg::IDD, this);
	ASSERT(pWndChild);
	pWndChild->SetInitModify(key);
	pWndChild->SetModify(TRUE);

	/*
	if (m_pWndChild == NULL)
		m_pWndChild = (CCMLoadLineItemDlg*)CreateOrActivateDlg(m_pDoc, CCMLoadLineItemDlg::IDD, this);
	
	m_pWndChild->SetInitModify(key);
	m_pWndChild->SetModify(TRUE);
	*/
}
