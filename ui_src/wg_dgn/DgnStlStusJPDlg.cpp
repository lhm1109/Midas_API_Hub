// DgnPSCAssessmentOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlStusJPDlg.h"

#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_dbLock\LockOption.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlStusJPDlg dialog


CDgnStlStusJPDlg::CDgnStlStusJPDlg(CWnd* pParent)
	: CMenuBarChildDlg(CDgnStlStusJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlStusJPDlg)
	m_nOption = 0;
	m_bUpperLimitLB = FALSE;
	m_bUpperLimitBC = FALSE;

	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlStusJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlStusJPDlg)
	DDX_Radio(pDX, IDC_DGN_AR, m_nOption);
	DDX_Check(pDX, IDC_DGN_STL_JP_STUS_UPPER_LBUCKLING_CHK, m_bUpperLimitLB);
	DDX_Check(pDX, IDC_DGN_STL_JP_STUS_UPPER_BENDSTRESS_CHK, m_bUpperLimitBC);

	//}}AFX_DATA_MAP
}

void CDgnStlStusJPDlg::EnableDisableControls()
{
	GetDlgItem(IDC_DGN_STL_JP_STUS_UPPER_LBUCKLING_CHK)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_STL_JP_STUS_UPPER_LBUCKLING_STC)->EnableWindow(m_nOption == 0);

	GetDlgItem(IDC_DGN_STL_JP_STUS_UPPER_BENDSTRESS_CHK)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_STL_JP_STUS_UPPER_BENDSTRESS_STC)->EnableWindow(m_nOption == 0);

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE  )->ShowWindow(SW_HIDE);
}

BEGIN_MESSAGE_MAP(CDgnStlStusJPDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlStusJPDlg)
// 	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
// 	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_AR, OnDgnArDel)
	ON_BN_CLICKED(IDC_DGN_DEL, OnDgnArDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlStusJPDlg message handlers

BOOL CDgnStlStusJPDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlStusJPDlg::Execute()
{
#if defined(_CIVIL_JP)
	if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_STEEL_MEMBER_DESIGN))
	{
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), _LS(IDS_IDD_MAIN_PROTECTION_EX_OPT_CIVILNX_JP_DLG_IDC_LBC_LOCKOPT_CHK7));
		AfxMessageBox(strErrMsg);
		return;
	}
#endif

	UpdateData(TRUE);

	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if (aSelKey.GetSize() > 0)
	{
		if (m_nOption == 0)	// Add/Replace
		{
			CString strGroup = _T("");
			T_STUS_D rData; rData.Initialize();

			rData.bUpperLimitLB = m_bUpperLimitLB;
			rData.bUpperLimitBC = m_bUpperLimitBC;

			if (m_pDoc->m_pDataCtrl->AddStus(aSelKey, rData))
			{
				m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
			}
		}
		else	// Delete
		{
			if (m_pDoc->m_pDataCtrl->DelStus(aSelKey))
			{
				m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
			}
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
}

// void CDgnStlStusJPDlg::OnDgnClose()
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnStlStusJPDlg::OnDgnArDel()
{
	UpdateData(TRUE);
	EnableDisableControls();
}