// DgnPSCAssessmentOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnPSCAssessmentOptionDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnPSCAssessmentOptionDlg dialog


CDgnPSCAssessmentOptionDlg::CDgnPSCAssessmentOptionDlg(CWnd* pParent)
	: CMenuBarChildDlg(CDgnPSCAssessmentOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnPSCAssessmentOptionDlg)
	m_nOption  = 0;	
	m_nPart    = 0;
	m_nClass   = 0;
	m_bTypeC   = FALSE;
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnPSCAssessmentOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnPSCAssessmentOptionDlg)
	DDX_Radio(pDX, IDC_DGN_AR,                 m_nOption);
	DDX_Radio(pDX, IDC_DGN_I_RADIO,            m_nPart);
	DDX_Radio(pDX, IDC_DGN_CLASS1_RDO,	     m_nClass);
	DDX_Check(pDX, IDC_DGN_CLASS3_TYPE_C_CHK, m_bTypeC);
	//}}AFX_DATA_MAP
}

void CDgnPSCAssessmentOptionDlg::EnableDisableControls()
{
	GetDlgItem(IDC_DGN_I_RADIO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_J_RADIO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_IJ_RADIO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_CLASS1_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_CLASS2_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_CLASS3_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_CLASS3_TYPE_C_CHK)->EnableWindow(m_nOption == 0 && m_nClass == 2);
	
	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CLOSE)->EnableWindow(TRUE);
}

BEGIN_MESSAGE_MAP(CDgnPSCAssessmentOptionDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnPSCAssessmentOptionDlg)
	ON_BN_CLICKED(IDC_DGN_AR,                 OnDgnArDel)
	ON_BN_CLICKED(IDC_DGN_DEL,                OnDgnArDel)		
	ON_BN_CLICKED(IDC_DGN_CLASS1_RDO,         OnDgnClassRdo)
	ON_BN_CLICKED(IDC_DGN_CLASS2_RDO,         OnDgnClassRdo)
	ON_BN_CLICKED(IDC_DGN_CLASS3_RDO,         OnDgnClassRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnPSCAssessmentOptionDlg message handlers

BOOL CDgnPSCAssessmentOptionDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();	
	
	EnableDisableControls();

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnPSCAssessmentOptionDlg::Execute()
{
	UpdateData(TRUE);
	
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);
	
	if(aSelKey.GetSize() > 0)
	{
		if(m_nOption==0)	// Add/Replace
		{
			CString strGroup = _T("");
			T_ASOP_D rData; rData.Initialize();
			
			rData.nPos = m_nPart;
			rData.nClass = m_nClass;
			rData.bTypeC = m_bTypeC;

			if( CDBDoc::GetDocPoint()->m_pDataCtrl->AddAsop(aSelKey,rData))
			{
                CDBDoc::GetDocPoint()->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
			}
		}
		else	// Delete
		{
			if( CDBDoc::GetDocPoint()->m_pDataCtrl->DelAsop(aSelKey))
			{
                CDBDoc::GetDocPoint()->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
			}
		}    
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

// void CDgnPSCAssessmentOptionDlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnPSCAssessmentOptionDlg::OnDgnArDel() 
{
	UpdateData(TRUE);
	EnableDisableControls();
}

void CDgnPSCAssessmentOptionDlg::OnDgnClassRdo()
{
	UpdateData(TRUE);
	EnableDisableControls();
}