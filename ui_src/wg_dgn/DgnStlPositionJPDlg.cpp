// DgnPSCAssessmentOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlPositionJPDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_MsgDll.h"

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
// CDgnStlPositionJPDlg dialog


CDgnStlPositionJPDlg::CDgnStlPositionJPDlg(CWnd* pParent)
	: CMenuBarChildDlg(CDgnStlPositionJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlPositionJPDlg)
	m_nOption = 0;
	m_iChkMethod = 0;
	m_iChkAxial = 0;
	m_nPart = 0;

	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlPositionJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlPositionJPDlg)
	DDX_Radio(pDX, IDC_DGN_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CHECK_METHOD1_RADIO, m_iChkMethod);
	DDX_Radio(pDX, IDC_DGN_Y_RADIO            , m_iChkAxial);
	DDX_Radio(pDX, IDC_DGN_I_RADIO            , m_nPart);

	//}}AFX_DATA_MAP
}

void CDgnStlPositionJPDlg::EnableDisableControls()
{
	GetDlgItem(IDC_DGN_CHECK_METHOD1_RADIO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_CHECK_METHOD2_RADIO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_CHECK_METHOD3_RADIO)->EnableWindow(m_nOption == 0);

	GetDlgItem(IDC_DGN_Y_RADIO )->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_Z_RADIO )->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_YZ_RADIO)->EnableWindow(m_nOption == 0);

	GetDlgItem(IDC_DGN_I_RADIO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_J_RADIO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_IJ_RADIO)->EnableWindow(m_nOption == 0);

	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CLOSE)->EnableWindow(TRUE);
}

BEGIN_MESSAGE_MAP(CDgnStlPositionJPDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlPositionJPDlg)
// 	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
// 	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_AR, OnDgnArDel)
	ON_BN_CLICKED(IDC_DGN_DEL, OnDgnArDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlPositionJPDlg message handlers

BOOL CDgnStlPositionJPDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlPositionJPDlg::Execute()
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
			T_STDP_D rData; rData.Initialize();

			if (m_nPart == 0 || m_nPart == 2) rData.bPosi[0] = TRUE;
			if (m_nPart == 1 || m_nPart == 2) rData.bPosi[1] = TRUE;

			rData.iChkMethod = m_iChkMethod;
			rData.iChkAxial  = m_iChkAxial;

			if (m_pDoc->m_pDataCtrl->AddStdp(aSelKey, rData))
			{
				m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
			}
		}
		else	// Delete
		{
			if (m_pDoc->m_pDataCtrl->DelStdp(aSelKey))
			{
				m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
			}
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
}

// void CDgnStlPositionJPDlg::OnDgnClose()
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnStlPositionJPDlg::OnDgnArDel()
{
	UpdateData(TRUE);
	EnableDisableControls();
}