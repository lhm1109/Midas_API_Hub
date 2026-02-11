// StldIfgsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldIfgsDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_cmd\wg_cmdAll.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldIfgsDlg dialog
CStldIfgsDlg::CStldIfgsDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldIfgsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldIfgsDlg)
	//}}AFX_DATA_INIT
	m_Group.Add(IDC_TM_STATIC1);
	m_Group.Add(IDC_TM_STATIC2);
	m_Group.Add(IDC_TM_STATIC3);
	m_Group.Add(IDC_TM_IFGS_DIREC);
	m_Group.Add(IDC_TM_IFGS_FORCE);
	m_Group.Add(IDC_TM_IFGS_FORCE_UNIT);

	m_aCtrlOption.Add(IDC_TM_REPLACE);
	m_aCtrlOption.Add(IDC_TM_DELETE);
}

void CStldIfgsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldIfgsDlg)
	DDX_Control(pDX, IDC_TM_IFGS_DIREC, m_direct);
	DDX_Control(pDX, IDC_TM_IFGS_FORCE_UNIT, m_forceunit);
	DDX_Control(pDX, IDC_TM_IFGS_FORCE, m_force);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStldIfgsDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldIfgsDlg)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_REPLACE, OnTmReplace)
	ON_BN_CLICKED(IDC_TM_DELETE, OnTmDelete)
	//ON_BN_CLICKED(IDC_TM_INIT_FORCE_BTN, OnTmInitForceBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldIfgsDlg message handlers
BOOL CStldIfgsDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_direct.AddString(_LS(IDS_TM_IFGS_Axial));
	m_direct.AddString(_LS(IDS_TM_IFGS_Global_X_Projection));
	m_direct.AddString(_LS(IDS_TM_IFGS_Global_Y_Projection));
	m_direct.AddString(_LS(IDS_TM_IFGS_Global_Z_Projection));
	CDlgUtil::CobxAdjustListBoxWidth(m_direct);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, 0);
	CDlgUtil::CtrlEnableDisable(this, m_Group, TRUE);

	m_direct.SetCurSel(0);

	m_force.SetUnitType(CUnitCtrl::m_IFGS_UNIT.InitForce);
	m_forceunit.SetUnitType(CUnitCtrl::m_IFGS_UNIT.InitForce);

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldIfgsDlg::OnTmExecute() 
{
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
		return;
	}
	T_IFGS_D data;
	data.Initialize();

	data.nDirection = m_direct.GetCurSel();
	data.InitForce = m_force.GetEditValue();

	int nOption;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);

	BOOL bSuccess=FALSE;
	if (nOption == 0) // Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddIfgs(aSelKey, data);	
	else if (nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelIfgs(aSelKey);	

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CStldIfgsDlg::OnTmClose() 
{
	CloseDlg();
}

void CStldIfgsDlg::OnTmReplace() 
{
	CDlgUtil::CtrlEnableDisable(this,	m_Group, TRUE);
}

void CStldIfgsDlg::OnTmDelete() 
{
	CDlgUtil::CtrlEnableDisable(this,	m_Group, FALSE);
}


