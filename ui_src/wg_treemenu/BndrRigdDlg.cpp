// BndrRigdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrRigdDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrRigdDlg dialog

CBndrRigdDlg::CBndrRigdDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrRigdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrRigdDlg)
	m_nOption = 0;
	m_csMasterNode = _T("");
	//}}AFX_DATA_INIT
	m_aCtrlOption.Add(IDC_TM_OPT_ADD);
	m_aCtrlOption.Add(IDC_TM_OPT_DELETE);

	m_aCtrlDisable.Add(IDC_TM_DOF_DX);
	m_aCtrlDisable.Add(IDC_TM_DOF_DY);
	m_aCtrlDisable.Add(IDC_TM_DOF_DZ);
	m_aCtrlDisable.Add(IDC_TM_DOF_RX);
	m_aCtrlDisable.Add(IDC_TM_DOF_RY);
	m_aCtrlDisable.Add(IDC_TM_DOF_RZ);

	m_aCtrlDisable.Add(IDC_TM_BTN_RB);
	m_aCtrlDisable.Add(IDC_TM_BTN_XY);
	m_aCtrlDisable.Add(IDC_TM_BTN_YZ);  
	m_aCtrlDisable.Add(IDC_TM_BTN_XZ);

	m_aCtrlDisable.Add(IDC_TM_COPY_LOAD);
	m_aCtrlDisable.Add(IDC_TM_AXIS_TITLE);
	m_aCtrlDisable.Add(IDC_TM_AXIS_X);
	m_aCtrlDisable.Add(IDC_TM_AXIS_Y);
	m_aCtrlDisable.Add(IDC_TM_AXIS_Z);
	m_aCtrlDisable.Add(IDC_TM_DIST_TITLE);
	m_aCtrlDisable.Add(IDC_TM_DIST_EDIT);
	m_aCtrlDisable.Add(IDC_TM_DIST_UNIT);
	m_aCtrlDisable.Add(IDC_TM_DIST_EXAM);

	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_TITLE);
	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_X);
	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_Y);
	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_Z);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_TITLE);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_EDIT);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_UNIT);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_EXAM);

	m_aCtrlAxis.Add(IDC_TM_AXIS_X);
	m_aCtrlAxis.Add(IDC_TM_AXIS_Y);
	m_aCtrlAxis.Add(IDC_TM_AXIS_Z);
}


void CBndrRigdDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrRigdDlg)
	DDX_Control(pDX, IDC_TM_RELEASE_SLAVE_NODE, m_wndReleaseSlaveNode);
	DDX_Control(pDX, IDC_TM_DIST_UNIT, m_wndDistUnit);
	DDX_Control(pDX, IDC_TM_DIST_EDIT, m_wndDistEdit);
	DDX_Control(pDX, IDC_TM_COPY_LOAD, m_wndCopyLoad);
	DDX_Control(pDX, IDC_TM_MASTER_NODE, m_wndMasterNode);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Text(pDX, IDC_TM_MASTER_NODE, m_csMasterNode);
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC4, m_wndPicture);
	//}}AFX_DATA_MAP
	UINT nDOFid[] = {
		IDC_TM_DOF_DX, IDC_TM_DOF_DY, IDC_TM_DOF_DZ,
		IDC_TM_DOF_RX, IDC_TM_DOF_RY, IDC_TM_DOF_RZ,
	};
	for (int i = 0; i < 6; i++) DDX_Check(pDX, nDOFid[i], m_bDOF[i]);
}



BEGIN_MESSAGE_MAP(CBndrRigdDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrRigdDlg)
	ON_BN_CLICKED(IDC_TM_RELEASE_SLAVE_NODE, OnTmReleaseSlaveNode)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_BTN_RB, OnBtnRigidBody)
	ON_BN_CLICKED(IDC_TM_BTN_XY, OnBtnPlaneXY)
	ON_BN_CLICKED(IDC_TM_BTN_XZ, OnBtnPlaneXZ)
	ON_BN_CLICKED(IDC_TM_BTN_YZ, OnBtnPlaneYZ)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_COPY_LOAD, OnTmCopyLoad)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_BN_CLICKED(IDC_TM_START_LINK_NUM_BTN, OnViewStartLinkNum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrRigdDlg message handlers

BOOL CBndrRigdDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here

	// Added by BugBoy 2000.7.25
	// Copy Rigid Link 내에 Default 값이 설정되어 있지 않다.

#ifdef _CIVIL
	((CButton *)GetDlgItem(IDC_TM_AXIS_X))->SetCheck(TRUE);
#else
	((CButton *)GetDlgItem(IDC_TM_AXIS_Z))->SetCheck(TRUE);
#endif

	m_pDoc = CDBDoc::GetDocPoint();
	
	m_nOption = 0;
	m_csMasterNode = _T("");

	m_wndMasterNode.SetAttNodeList();
	m_wndMasterNode.SetLButtonDownNotifyWindow(this);
	m_wndMasterNode.SetEnterNotifyWindow(this);
	m_wndMasterNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndMasterNode.SetMaxNodeKeyNum(1);

	m_bDOF[0] = TRUE; m_bDOF[1] = TRUE; m_bDOF[2] = FALSE;
	m_bDOF[3] = FALSE; m_bDOF[4] = FALSE; m_bDOF[5] = TRUE;

	CtrlEnableDisable(m_aCtrlDisable, m_nOption == 0);
	if (m_nOption == 0)  // add
	{
		BOOL bCopyLoad = m_wndCopyLoad.GetCheck();
		CtrlEnableDisable(m_aCtrlCopyLoad, bCopyLoad);
		m_wndReleaseSlaveNode.EnableWindow(FALSE);
	}
	ShowHideStartLinkNumCtrl();
 
	m_wndDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	UpdateData(FALSE);

	m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_rigd 1.svg"));

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrRigdDlg::Execute()
{
	UpdateData(TRUE);
	T_NODE_K nMasterKey;
	CArray<T_NODE_K, T_NODE_K> aSelKey;
	BOOL bReleaseSlaveNode = m_wndReleaseSlaveNode.GetCheck();

	if ((m_nOption == 0 && m_csMasterNode.IsEmpty()) ||
		(m_nOption == 1 && !bReleaseSlaveNode && m_csMasterNode.IsEmpty()))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Master_node_is_not_specifie));
		return;
	}

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedNodeKeyList(aSelKey);
	if (aSelKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_slave_node_));
		return;
	}

	T_RIGD_D data;
	data.Initialize();  
	nMasterKey = _ttol(m_csMasterNode);
	// Delete인 경우 Master Node를 지정 않는 옵션을 선택하면 Master Node를 0으로 넘겨줌.
	// DataCtrl->DelRigd(nMasterKey, aSelKey, BngrK)에서 nMasterKey가 0인 경우
	// MasterKey와 상관없이 aSelKey를 Rigid Link에서 해제시켜줌
	if (m_nOption == 1 && bReleaseSlaveNode) nMasterKey = 0;

	data.nSlaveType = 0;
	for (int i = 0; i < 6; i++)
	{
		data.nSlaveType *= 10;
		if (m_bDOF[i]) data.nSlaveType += 1;
	}
	data.arSlave.Copy(aSelKey);

	BOOL bCopyLoad;
	bCopyLoad = m_wndCopyLoad.GetCheck();
	double ux = 0.0, uy = 0.0, uz = 0.0;
	double wx = 0.0, wy = 0.0, wz = 0.0;
	CArray<double, double> aDistance;
	if (bCopyLoad)
	{
		int nAxis;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAxis, nAxis);
		switch (nAxis)
		{
		case 0: ux = 1.0; break;
		case 1: uy = 1.0; break;
		case 2: uz = 1.0; break;
		}
		pIGM->GetWcsVector(ux, uy, uz, wx, wy, wz);

		CString csDistance;
		m_wndDistEdit.GetWindowText(csDistance);
		if (!GetUEDistance(csDistance, aDistance) ||
			aDistance.GetSize() <= 0)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_distance_value_));
			return;
		}
	}

	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	BOOL bSuccess = FALSE;
	if (m_nOption == 0) // Add/Replace
	{
		data.GroupKey = BngrK;
		bSuccess = m_pDoc->m_pDataCtrl->AddRigd(nMasterKey, data, bCopyLoad, wx, wy, wz, aDistance);
	}
	else if (m_nOption == 1)  // delete
	{
		bSuccess = m_pDoc->m_pDataCtrl->DelRigd(nMasterKey, aSelKey, BngrK);
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}
void CBndrRigdDlg::OnTmExecute()
{	
	Execute();
}

void CBndrRigdDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CBndrRigdDlg::OnBtnRigidBody() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	for (int i = 0; i < 6; i++) m_bDOF[i] = TRUE;
	UpdateData(FALSE);
}

void CBndrRigdDlg::OnBtnPlaneXY() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_bDOF[0] = TRUE; m_bDOF[1] = TRUE; m_bDOF[2] = FALSE;
	m_bDOF[3] = FALSE; m_bDOF[4] = FALSE; m_bDOF[5] = TRUE;
	UpdateData(FALSE);	
}

void CBndrRigdDlg::OnBtnPlaneXZ() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_bDOF[0] = TRUE; m_bDOF[1] = FALSE; m_bDOF[2] = TRUE;
	m_bDOF[3] = FALSE; m_bDOF[4] = TRUE; m_bDOF[5] = FALSE;
	UpdateData(FALSE);
}

void CBndrRigdDlg::OnBtnPlaneYZ() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_bDOF[0] = FALSE; m_bDOF[1] = TRUE; m_bDOF[2] = TRUE;
	m_bDOF[3] = TRUE; m_bDOF[4] = FALSE; m_bDOF[5] = FALSE;
	UpdateData(FALSE);	
}


void CBndrRigdDlg::OnChangeOption() 
{
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nCheck);
	CtrlEnableDisable(m_aCtrlDisable, nCheck == 0);
	BOOL bReleaseSlaveNode = m_wndReleaseSlaveNode.GetCheck();
	if (nCheck == 0)  // add
	{
		BOOL bCopyLoad = m_wndCopyLoad.GetCheck();
		CtrlEnableDisable(m_aCtrlCopyLoad, bCopyLoad);
		this->m_wndGroupCombo.ShowHideAll(FALSE);
		m_wndReleaseSlaveNode.EnableWindow(FALSE);
		if(bReleaseSlaveNode) 
		{
			m_wndReleaseSlaveNode.SetCheck(FALSE);
			m_wndMasterNode.EnableWindow(TRUE);
		}
	}
	else // Delete
	{
		this->m_wndGroupCombo.ShowHideAll(TRUE);
		m_wndReleaseSlaveNode.EnableWindow(TRUE);
	}
}

void CBndrRigdDlg::OnTmCopyLoad() 
{
	BOOL bCopyLoad = m_wndCopyLoad.GetCheck();
	CtrlEnableDisable(m_aCtrlCopyLoad, bCopyLoad);	
}

void CBndrRigdDlg::OnTmReleaseSlaveNode()
{
	BOOL bReleaseSlaveNode = m_wndReleaseSlaveNode.GetCheck();
	m_wndMasterNode.EnableWindow(bReleaseSlaveNode == FALSE);
	m_csMasterNode = _T("");
}

BOOL CBndrRigdDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_RIGD_K rigdkey;
	T_RIGD_D data;

	rigdkey.keymap = key;
	m_pDoc->m_pAttrCtrl->GetRigd(rigdkey, data);

	UINT temp;
	temp = data.nSlaveType;
	m_bDOF[5] = temp % 10;
	for (int i=1; i < 6; i++)
	{
		temp /= 10;
		m_bDOF[5-i] = temp % 10;
	}
	// Group도 Setting
	T_BNGR_D  bngrD;
	m_pDoc->m_pAttrCtrl->GetBngr(data.GroupKey, bngrD);
	
	for(int i=0; i<m_wndGroupCombo.GetCount(); i++)
	{
		CString str;
		m_wndGroupCombo.GetLBText(i, str);
		if(str.CompareNoCase(bngrD.GroupName)==0) { m_wndGroupCombo.SetCurSel(i); break; }
	}

	m_nOption = 0;
	UpdateData(FALSE);
	return TRUE;
}

BOOL CBndrRigdDlg::ExternalInit_by_SelectLabel(UINT key)
{
	return TRUE;
}

void CBndrRigdDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);	
}

void CBndrRigdDlg::OnViewStartLinkNum() 
{

}

// 레지스트리 쓰는 동안만 필요한 함수임
void CBndrRigdDlg::ShowHideStartLinkNumCtrl()
{
	if(((CDBDoc*)CTreeMenuBarBase::GetDocST())->ExistRegPMS5629())
		return;

	GetDlgItem(IDC_TM_START_LINK_NUM_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_START_LINK_NUM_EDT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_START_LINK_NUM_BTN)->ShowWindow(SW_HIDE);

	CArray<UINT, UINT> aCtrl_Under_LinkNum;
	aCtrl_Under_LinkNum.Add(IDC_TM_MASTER_NODE       );
	aCtrl_Under_LinkNum.Add(IDC_TM_RELEASE_SLAVE_NODE);
	aCtrl_Under_LinkNum.Add(IDC_TM_DOF_DX            );
	aCtrl_Under_LinkNum.Add(IDC_TM_DOF_DY            );
	aCtrl_Under_LinkNum.Add(IDC_TM_DOF_DZ            );
	aCtrl_Under_LinkNum.Add(IDC_TM_DOF_RX            );
	aCtrl_Under_LinkNum.Add(IDC_TM_DOF_RY            );
	aCtrl_Under_LinkNum.Add(IDC_TM_DOF_RZ            );
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC0  );
	aCtrl_Under_LinkNum.Add(IDC_TM_COPY_LOAD         );
	aCtrl_Under_LinkNum.Add(IDC_TM_AXIS_X            );
	aCtrl_Under_LinkNum.Add(IDC_TM_AXIS_Y            );
	aCtrl_Under_LinkNum.Add(IDC_TM_AXIS_Z            );
	aCtrl_Under_LinkNum.Add(IDC_TM_DIST_EDIT         );
	aCtrl_Under_LinkNum.Add(IDC_TM_BTN_RB            );
	aCtrl_Under_LinkNum.Add(IDC_TM_BTN_XY            );
	aCtrl_Under_LinkNum.Add(IDC_TM_BTN_YZ            );
	aCtrl_Under_LinkNum.Add(IDC_TM_BTN_XZ            );
	aCtrl_Under_LinkNum.Add(IDC_TM_EXECUTE           );
	aCtrl_Under_LinkNum.Add(IDC_TM_CLOSE             );
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC2  );
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC3  );
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC4  );
	aCtrl_Under_LinkNum.Add(IDC_TM_AXIS_TITLE        );
	aCtrl_Under_LinkNum.Add(IDC_TM_DIST_TITLE        );
	aCtrl_Under_LinkNum.Add(IDC_TM_DIST_UNIT         );
	aCtrl_Under_LinkNum.Add(IDC_TM_DIST_EXAM         );
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC5  );
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC6  );

	CRect rRef, rToMove;
	CWnd* pWnd = GetDlgItem(IDC_TM_START_LINK_NUM_STC);
	pWnd->GetWindowRect(rRef);
	pWnd = GetDlgItem(IDC_WG_TREEMENU_STATIC6);
	pWnd->GetWindowRect(rToMove);
	int nDistY = rRef.top- rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aCtrl_Under_LinkNum, nDistY);
}

/*
void CBndrRigdDlg::OnTmExecute()
{	
	UpdateData(TRUE);
	T_NODE_K nMasterKey;
	CArray<T_NODE_K, T_NODE_K> aSelKey;

	if (m_csMasterNode.IsEmpty()) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Master_node_is_not_specifie));
		return;
	}

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedNodeKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_slave_node_));
		return;
	}

	T_RIGD_D data;
	data.Initialize();
	nMasterKey = _ttol(m_csMasterNode);

	data.nSlaveType = 0;
	for (int i = 0; i < 6; i++)
	{
		data.nSlaveType *= 10;
		if (m_bDOF[i]) data.nSlaveType += 1;
	}
	data.arSlave.Copy(aSelKey);

	BOOL bCopyLoad;
	bCopyLoad = m_wndCopyLoad.GetCheck();
	double ux=0.0, uy=0.0, uz=0.0;
	double wx, wy, wz;
	CArray<double, double> aDistance;
	if (bCopyLoad)
	{
		int nAxis;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAxis, nAxis);
		switch(nAxis)
		{
		case 0: ux = 1.0; break;
		case 1: uy = 1.0; break;
		case 2: uz = 1.0; break;
		}
		pIGM->GetWcsVector(ux, uy, uz, wx, wy, wz);

		CString csDistance;
		m_wndDistEdit.GetWindowText(csDistance);
		if(!GetUEDistance(csDistance, aDistance) ||
			 aDistance.GetSize() <= 0)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_distance_value_));
			return;
		}
	}

	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	BOOL bSuccess;
	if (m_nOption == 0) // Add or
	{ 
		data.GroupKey = BngrK;
		bSuccess = m_pDoc->m_pDataCtrl->AddRigd(nMasterKey, data, bCopyLoad, wx, wy, wz, aDistance);
	}
	else if (m_nOption == 1)  // delete
	{
		bSuccess = m_pDoc->m_pDataCtrl->DelRigd(nMasterKey, aSelKey, BngrK);
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}
*/