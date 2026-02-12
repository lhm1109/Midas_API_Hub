// StldTimeLoadsForCS.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldTimeLoadsForCS.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldTimeLoadsForCS dialog
#define CDialog CMenuBarChildDlg

CStldTimeLoadsForCS::CStldTimeLoadsForCS(CWnd* pParent /*=NULL*/)
	: CDialog(CStldTimeLoadsForCS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldTimeLoadsForCS)
	m_nOption = 0;
	//}}AFX_DATA_INIT
}


void CStldTimeLoadsForCS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldTimeLoadsForCS)
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_CobxLdgr);
	DDX_Control(pDX, IDC_TM_DAY_EDIT, m_wndDay);
	DDX_Radio(pDX, IDC_TM_SD_OPT_ADD, m_nOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldTimeLoadsForCS, CDialog)
	//{{AFX_MSG_MAP(CStldTimeLoadsForCS)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_DAY_SPIN, OnDeltaposTmDaySpin)
	ON_BN_CLICKED(IDC_TM_SD_OPT_ADD, OnTmSdOptRadio)
	ON_BN_CLICKED(IDC_TM_SD_OPT_DELETE, OnTmSdOptRadio)
	ON_BN_CLICKED(IDC_TM_SD_OPT_REPLACE, OnTmSdOptRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldTimeLoadsForCS message handlers
BOOL CStldTimeLoadsForCS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_nOption = 0;
	UpdateData(FALSE);

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	m_wndDay.SetRange(0, SHRT_MAX);
	m_wndDay.SetValue(0);
	m_wndDay.SetInteger(TRUE);

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldTimeLoadsForCS::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMLoadGroupDlg::IDD);		
}

void CStldTimeLoadsForCS::OnTmExecute() 
{
	UpdateData(TRUE);
	//T_LDGR_K LdgrK;
	T_TMLD_D TmldD;
	
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	//T_ELEM_D ElemData;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);

	if(aSelKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_T("No selected elements exist!"));
		return ;
	}
	
	
	/*
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
		if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsBeam(ElemData.eltyp)) aSelKey.RemoveAt(i);
	}
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_beam_element_));
		return;
	}
	*/
	 
	T_LDGR_K LdgrK;
	m_CobxLdgr.GetSelectedLdgr(LdgrK);
	TmldD.GroupKey = LdgrK;
	TmldD.dDay = m_wndDay.GetEditValue();
	
	BOOL bSuccess;
	if(m_nOption == 0)
		bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->AddTmld(aSelKey,TmldD,FALSE);
	else if(m_nOption ==1)
		bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->AddTmld(aSelKey,TmldD,TRUE);
	else
		bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->DelTmld(aSelKey,TmldD.GroupKey);

	if (bSuccess) CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
}

void CStldTimeLoadsForCS::OnTmClose() 
{
	CloseDlg();
}

void CStldTimeLoadsForCS::OnDeltaposTmDaySpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetFloatSpinValue(this,IDC_TM_DAY_EDIT,_T("0"),_T("ULM"),1.0,pNMHDR,pResult);
}

void CStldTimeLoadsForCS::OnTmSdOptRadio() 
{
	UpdateData();
	
	CArray<UINT,UINT> Ctrls;
	//Ctrls.Add(IDC_TM_DAY_SPIN);
	Ctrls.Add(IDC_TM_DAY_EDIT);

	if(m_nOption == 0 || m_nOption == 1)
		CDlgUtil::CtrlEnableDisable(this,Ctrls,TRUE);
	else
		CDlgUtil::CtrlEnableDisable(this,Ctrls,FALSE);
}
