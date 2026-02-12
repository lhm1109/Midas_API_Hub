// BndrMconDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrMconDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "BndrMconGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrMconDlg dialog

CBndrMconDlg::CBndrMconDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrMconDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrMconDlg)
	m_nOption   = 0; 
	//}}AFX_DATA_INIT
	m_pGrid = NULL;

	m_aCtrlOption      .RemoveAll();
	m_aCtrlOptionExp   .RemoveAll();
	m_aCtrlDisable     .RemoveAll();
	m_aCtrlExp         .RemoveAll();
	m_aCtrlWD          .RemoveAll();
	m_aCtrlDOF_RDO     .RemoveAll();
	m_aCtrlShow_EXP    .RemoveAll();
	m_aCtrlShow_WD     .RemoveAll();
	m_aCtrlGridApplyGrp.RemoveAll();
	m_aCtrlDOF_Exp_RDO .RemoveAll();
	m_aDT              .RemoveAll();
								
	//Add or Del
	m_aCtrlOption.Add(IDC_TM_OPT_ADD_RDO);
	m_aCtrlOption.Add(IDC_TM_OPT_DELETE_RDO);
	
	//Explicit or Weighted Displacement
	m_aCtrlOptionExp.Add(IDC_TM_OPT_EXP_RDO);
	m_aCtrlOptionExp.Add(IDC_TM_OPT_WD_RDO);

	//disable option
	m_aCtrlDisable.Add(IDC_TM_DOF_DX_CHK);
	m_aCtrlDisable.Add(IDC_TM_DOF_DY_CHK);
	m_aCtrlDisable.Add(IDC_TM_DOF_DZ_CHK);
	m_aCtrlDisable.Add(IDC_TM_DOF_RX_CHK);
	m_aCtrlDisable.Add(IDC_TM_DOF_RY_CHK);
	m_aCtrlDisable.Add(IDC_TM_DOF_RZ_CHK);
	m_aCtrlDisable.Add(IDC_TM_DOF_INDE_DX_RDO);
	m_aCtrlDisable.Add(IDC_TM_DOF_INDE_DY_RDO);
	m_aCtrlDisable.Add(IDC_TM_DOF_INDE_DZ_RDO);
	m_aCtrlDisable.Add(IDC_TM_DOF_INDE_RX_RDO);
	m_aCtrlDisable.Add(IDC_TM_DOF_INDE_RY_RDO);
	m_aCtrlDisable.Add(IDC_TM_DOF_INDE_RZ_RDO);
	m_aCtrlDisable.Add(IDC_TM_DOF_EXP_DX_RDO);
	m_aCtrlDisable.Add(IDC_TM_DOF_EXP_DY_RDO);
	m_aCtrlDisable.Add(IDC_TM_DOF_EXP_DZ_RDO);
	m_aCtrlDisable.Add(IDC_TM_DOF_EXP_RX_RDO);
	m_aCtrlDisable.Add(IDC_TM_DOF_EXP_RY_RDO);
	m_aCtrlDisable.Add(IDC_TM_DOF_EXP_RZ_RDO);

	m_aCtrlDisable.Add(IDC_TM_OPT_EXP_RDO);
	m_aCtrlDisable.Add(IDC_TM_OPT_WD_RDO);
	m_aCtrlDisable.Add(IDC_TM_WD_NODE_EDT);
	m_aCtrlDisable.Add(IDC_TM_WD_WEIGHT_EDT);
	m_aCtrlDisable.Add(IDC_TM_MCON_WD_ADD_BTN);
	m_aCtrlDisable.Add(IDC_TM_MCON_WD_DEL_BTN);
	m_aCtrlDisable.Add(IDC_TM_EXP_NODE_EDT);
	m_aCtrlDisable.Add(IDC_TM_MASTER_COEF_EDT);
	m_aCtrlDisable.Add(IDC_TM_MCON_EXP_ADD_BTN);
	m_aCtrlDisable.Add(IDC_TM_MCON_EXP_DEL_BTN);
	
	m_aCtrlDisable.Add(IDC_TM_COEF_STC);
	m_aCtrlDisable.Add(IDC_TM_MCON_WD_CHK_GRP);
	m_aCtrlDisable.Add(IDC_TM_MCON_EXP_CHK_GRP);
	m_aCtrlDisable.Add(IDC_TM_MCON_EXP_STC);
	m_aCtrlDisable.Add(IDC_TM_MCON_EXP_NODE_STC);
	m_aCtrlDisable.Add(IDC_TM_MCON_EXP_DOF_STC);

	m_aCtrlDisable.Add(IDC_TM_COEF_STC);
	m_aCtrlDisable.Add(IDC_TM_MCON_WD_STC);
	m_aCtrlDisable.Add(IDC_TM_MCON_WD_NODE_STC);
	m_aCtrlDisable.Add(IDC_TM_MCON_WD_WEIGHT_STC);

	//Weighted Displacement Enable Group
	m_aCtrlWD.Add(IDC_TM_DOF_DX_CHK);
	m_aCtrlWD.Add(IDC_TM_DOF_DY_CHK);
	m_aCtrlWD.Add(IDC_TM_DOF_DZ_CHK);
	m_aCtrlWD.Add(IDC_TM_DOF_RX_CHK);
	m_aCtrlWD.Add(IDC_TM_DOF_RY_CHK);
	m_aCtrlWD.Add(IDC_TM_DOF_RZ_CHK);
	m_aCtrlWD.Add(IDC_TM_MCON_WD_CHK_GRP);
	m_aCtrlWD.Add(IDC_TM_MCON_WD_STC);
	m_aCtrlWD.Add(IDC_TM_WD_NODE_EDT);
	m_aCtrlWD.Add(IDC_TM_WD_WEIGHT_EDT);
	m_aCtrlWD.Add(IDC_TM_MCON_WD_ADD_BTN);
	m_aCtrlWD.Add(IDC_TM_MCON_WD_DEL_BTN);
	
	//Explicit Enable group
	m_aCtrlExp.Add(IDC_TM_DOF_EXP_DX_RDO);
	m_aCtrlExp.Add(IDC_TM_DOF_EXP_DY_RDO);
	m_aCtrlExp.Add(IDC_TM_DOF_EXP_DZ_RDO);
	m_aCtrlExp.Add(IDC_TM_DOF_EXP_RX_RDO);
	m_aCtrlExp.Add(IDC_TM_DOF_EXP_RY_RDO);
	m_aCtrlExp.Add(IDC_TM_DOF_EXP_RZ_RDO);
	m_aCtrlExp.Add(IDC_TM_DOF_INDE_DX_RDO);
	m_aCtrlExp.Add(IDC_TM_DOF_INDE_DY_RDO);
	m_aCtrlExp.Add(IDC_TM_DOF_INDE_DZ_RDO);
	m_aCtrlExp.Add(IDC_TM_DOF_INDE_RX_RDO);
	m_aCtrlExp.Add(IDC_TM_DOF_INDE_RY_RDO);
	m_aCtrlExp.Add(IDC_TM_DOF_INDE_RZ_RDO);
	m_aCtrlExp.Add(IDC_TM_MCON_EXP_CHK_GRP);
	m_aCtrlExp.Add(IDC_TM_MCON_EXP_STC);
	m_aCtrlExp.Add(IDC_TM_MCON_EXP_DOF_STC);
	m_aCtrlExp.Add(IDC_TM_EXP_NODE_EDT);
	m_aCtrlExp.Add(IDC_TM_MASTER_COEF_EDT);
	m_aCtrlExp.Add(IDC_TM_MCON_EXP_ADD_BTN);
	m_aCtrlExp.Add(IDC_TM_MCON_EXP_DEL_BTN);
	m_aCtrlExp.Add(IDC_TM_COEF_STC);

	//DOF Radio button group
	m_aCtrlDOF_RDO.Add(IDC_TM_DOF_INDE_DX_RDO);
	m_aCtrlDOF_RDO.Add(IDC_TM_DOF_INDE_DY_RDO);
	m_aCtrlDOF_RDO.Add(IDC_TM_DOF_INDE_DZ_RDO);
	m_aCtrlDOF_RDO.Add(IDC_TM_DOF_INDE_RX_RDO);
	m_aCtrlDOF_RDO.Add(IDC_TM_DOF_INDE_RY_RDO);
	m_aCtrlDOF_RDO.Add(IDC_TM_DOF_INDE_RZ_RDO);
	m_aCtrlDOF_RDO.Add(IDC_TM_MCON_EXP_CHK_GRP);
	
	//Dof Radio Button Explicit Group
	m_aCtrlDOF_Exp_RDO.Add(IDC_TM_DOF_EXP_DX_RDO);
	m_aCtrlDOF_Exp_RDO.Add(IDC_TM_DOF_EXP_DY_RDO);
	m_aCtrlDOF_Exp_RDO.Add(IDC_TM_DOF_EXP_DZ_RDO);
	m_aCtrlDOF_Exp_RDO.Add(IDC_TM_DOF_EXP_RX_RDO);
	m_aCtrlDOF_Exp_RDO.Add(IDC_TM_DOF_EXP_RY_RDO);
	m_aCtrlDOF_Exp_RDO.Add(IDC_TM_DOF_EXP_RZ_RDO);

	//DOF CheckBox Group 
	m_aCtrlDOF_CHK.Add(IDC_TM_DOF_DX_CHK);
	m_aCtrlDOF_CHK.Add(IDC_TM_DOF_DY_CHK);
	m_aCtrlDOF_CHK.Add(IDC_TM_DOF_DZ_CHK);
	m_aCtrlDOF_CHK.Add(IDC_TM_DOF_RX_CHK);
	m_aCtrlDOF_CHK.Add(IDC_TM_DOF_RY_CHK);
	m_aCtrlDOF_CHK.Add(IDC_TM_DOF_RZ_CHK);
	m_aCtrlDOF_CHK.Add(IDC_TM_MCON_WD_CHK_GRP);

	//Explicit 시 show해줄 부분
	m_aCtrlShow_EXP.Add(IDC_TM_DOF_EXP_DX_RDO);
	m_aCtrlShow_EXP.Add(IDC_TM_DOF_EXP_DY_RDO);
	m_aCtrlShow_EXP.Add(IDC_TM_DOF_EXP_DZ_RDO);
	m_aCtrlShow_EXP.Add(IDC_TM_DOF_EXP_RX_RDO);
	m_aCtrlShow_EXP.Add(IDC_TM_DOF_EXP_RY_RDO);
	m_aCtrlShow_EXP.Add(IDC_TM_DOF_EXP_RZ_RDO);
	m_aCtrlShow_EXP.Add(IDC_TM_MCON_EXP_CHK_GRP);
	m_aCtrlShow_EXP.Add(IDC_TM_MCON_EXP_STC);
	m_aCtrlShow_EXP.Add(IDC_TM_MCON_EXP_DOF_STC);
	m_aCtrlShow_EXP.Add(IDC_TM_EXP_NODE_EDT);
	m_aCtrlShow_EXP.Add(IDC_TM_MASTER_COEF_EDT);
	m_aCtrlShow_EXP.Add(IDC_TM_MCON_EXP_ADD_BTN);
	m_aCtrlShow_EXP.Add(IDC_TM_MCON_EXP_DEL_BTN);
	m_aCtrlShow_EXP.Add(IDC_TM_MCON_EXP_NODE_STC);
	m_aCtrlShow_EXP.Add(IDC_TM_COEF_STC);

	//Weighted Displacement 시에 show 해줄 부분 
	m_aCtrlShow_WD.Add(IDC_TM_MCON_WD_NODE_STC);
	m_aCtrlShow_WD.Add(IDC_TM_MCON_WD_WEIGHT_STC);
	m_aCtrlShow_WD.Add(IDC_TM_WD_NODE_EDT);
	m_aCtrlShow_WD.Add(IDC_TM_WD_WEIGHT_EDT);
	m_aCtrlShow_WD.Add(IDC_TM_MCON_WD_ADD_BTN);
	m_aCtrlShow_WD.Add(IDC_TM_MCON_WD_DEL_BTN);
	m_aCtrlShow_WD.Add(IDC_TM_MCON_WD_STC);

	//Grid, Cancle, Apply Group
	m_aCtrlGridApplyGrp.Add(IDC_TM_MCON_GRD);
	m_aCtrlGridApplyGrp.Add(IDC_TM_MCON_EXECUTE_BTN);
	m_aCtrlGridApplyGrp.Add(IDC_TM_MCON_CLOSE_BTN);
}


void CBndrMconDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CBndrMconDlg)
	DDX_Control(pDX, IDC_TM_RELEASE_SLAVE_NODE_CHK, m_chkReleaseSlaveNode);
	DDX_Control(pDX, IDC_TM_Constraint_NODE_EDT   , m_edtMasterNode      );
	DDX_Radio  (pDX, IDC_TM_OPT_ADD_RDO           , m_nOption            );
	DDX_Control(pDX, IDC_TM_GROUP_CMB             , m_cbxGroupCombo      );
	DDX_Control(pDX, IDC_TM_EXP_NODE_EDT          , m_edtExpNode         );
	DDX_Control(pDX, IDC_TM_WD_NODE_EDT           , m_edtWDNode          );
	DDX_Control(pDX, IDC_TM_MASTER_COEF_EDT       , m_edtCoeff           );
	DDX_Control(pDX, IDC_TM_WD_WEIGHT_EDT         , m_edtWeight          );
	
	//}}AFX_DATA_MAP                              
	
	UINT nDOFid[] = {
		IDC_TM_DOF_DX_CHK, IDC_TM_DOF_DY_CHK, IDC_TM_DOF_DZ_CHK,
		IDC_TM_DOF_RX_CHK, IDC_TM_DOF_RY_CHK, IDC_TM_DOF_RZ_CHK,
	};
	for (int i = 0; i < 6; i++) DDX_Check(pDX, nDOFid[i], m_bDOF[i]);

	UINT nIndependent_DOFid[] = {
		IDC_TM_DOF_INDE_DX_RDO, IDC_TM_DOF_INDE_DY_RDO, IDC_TM_DOF_INDE_DZ_RDO,
		IDC_TM_DOF_INDE_RX_RDO, IDC_TM_DOF_INDE_RY_RDO, IDC_TM_DOF_INDE_RZ_RDO,
	};
	for(int i=0; i < 6; i++) DDX_Check(pDX, nIndependent_DOFid[i], m_bIndependentDOF[i]);

	UINT nEXP_DOFid[] = {
		IDC_TM_DOF_EXP_DX_RDO, IDC_TM_DOF_EXP_DY_RDO, IDC_TM_DOF_EXP_DZ_RDO,
		IDC_TM_DOF_EXP_RX_RDO, IDC_TM_DOF_EXP_RY_RDO, IDC_TM_DOF_EXP_RZ_RDO,
	};
	for(int i=0; i < 6; i++) DDX_Check(pDX, nEXP_DOFid[i], m_bEXP_DOF[i]);

	DDX_Control(pDX, IDC_TM_MCON_GRD, *m_pGrid);
}

BEGIN_MESSAGE_MAP(CBndrMconDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrMconDlg)
	ON_BN_CLICKED(IDC_TM_RELEASE_SLAVE_NODE_CHK, OnTmReleaseSlaveNode)
	ON_BN_CLICKED(IDC_TM_MCON_EXP_ADD_BTN, OnTmExpAdd)
	ON_BN_CLICKED(IDC_TM_MCON_EXP_DEL_BTN, OnTmExpDel)
	ON_BN_CLICKED(IDC_TM_MCON_WD_ADD_BTN, OnTmWDAdd)
	ON_BN_CLICKED(IDC_TM_MCON_WD_DEL_BTN, OnTmWDDel)
	ON_BN_CLICKED(IDC_TM_OPT_ADD_RDO, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE_RDO, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_EXP_RDO, OnChangeOptionExp)
	ON_BN_CLICKED(IDC_TM_OPT_WD_RDO, OnChangeOptionExp)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BTN, OnTmDefineGroupButton)
	ON_EN_CHANGE(IDC_TM_Constraint_NODE_EDT, OnChangeMasterNodeEdt)
	ON_CBN_SELCHANGE(IDC_TM_GROUP_CMB, OnSelChangeGroupCombo)
	ON_BN_CLICKED(IDC_TM_DOF_INDE_DX_RDO, OnClickExpDof)
	ON_BN_CLICKED(IDC_TM_DOF_INDE_DY_RDO, OnClickExpDof)
	ON_BN_CLICKED(IDC_TM_DOF_INDE_DZ_RDO, OnClickExpDof)
	ON_BN_CLICKED(IDC_TM_DOF_INDE_RX_RDO, OnClickExpDof)
	ON_BN_CLICKED(IDC_TM_DOF_INDE_RY_RDO, OnClickExpDof)
	ON_BN_CLICKED(IDC_TM_DOF_INDE_RZ_RDO, OnClickExpDof)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrMconDlg message handlers

BOOL CBndrMconDlg::OnInitDialog() 
{
	if (m_pGrid == nullptr)
		m_pGrid = new CBndrMconGrid(this);

	CMenuBarChildDlg::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	
	InitGrid();

	m_edtExpNode.Connect(SC_ID_NODE, &m_edtExpNode);
	m_edtWDNode.Connect(SC_ID_NODE, &m_edtWDNode);

	m_edtMasterNode.SetAttNodeList();
	m_edtMasterNode.SetLButtonDownNotifyWindow(this);
	m_edtMasterNode.SetEnterNotifyWindow(this);
	m_edtMasterNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_edtMasterNode.SetMaxNodeKeyNum(1);

	m_nOption = 0;
	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOptionExp, 0);

	m_bDOF[0] = TRUE;  m_bDOF[1] = TRUE;  m_bDOF[2] = FALSE;
	m_bDOF[3] = FALSE; m_bDOF[4] = FALSE; m_bDOF[5] = TRUE;

	m_bIndependentDOF[0] = TRUE;  m_bIndependentDOF[1] = FALSE; m_bIndependentDOF[2] = FALSE;
	m_bIndependentDOF[3] = FALSE; m_bIndependentDOF[4] = FALSE; m_bIndependentDOF[5] = FALSE;

	m_bEXP_DOF[0] = TRUE;  m_bEXP_DOF[1] = FALSE; m_bEXP_DOF[2] = FALSE;
	m_bEXP_DOF[3] = FALSE; m_bEXP_DOF[4] = FALSE; m_bEXP_DOF[5] = FALSE;

	UpdateData(FALSE);

	m_edtCoeff.SetUnitType(0);
	m_edtWeight.SetUnitType(0);

	AlignControls();

	OnChangeOptionExp();
	
	CtrlEnableDisable(m_aCtrlDisable, m_nOption==0);

	if (m_nOption == 0)  // add
	{
		m_chkReleaseSlaveNode.EnableWindow(FALSE);
	}
	GetDlgItem(IDC_TM_MCON_EXECUTE_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_MCON_CLOSE_BTN)->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrMconDlg::Execute()
{	
	UpdateData(TRUE);
	T_NODE_K nMasterKey;
	CArray<T_NODE_K, T_NODE_K> aSelKey;
	BOOL bReleaseSlaveNode = m_chkReleaseSlaveNode.GetCheck();

	CString strMasterNode = _T("");
	m_edtMasterNode.GetWindowText(strMasterNode);
	if((m_nOption == 0 && strMasterNode.IsEmpty()) || 
		 (m_nOption == 1 && !bReleaseSlaveNode && strMasterNode.IsEmpty())) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Master_node_is_not_specifie));
		return;
	}

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedNodeKeyList(aSelKey);
	/*
	if(aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_slave_node_));
		return;
	}
	*/
	
	nMasterKey = _ttol(strMasterNode);
	// Delete인 경우 Master Node를 지정 않는 옵션을 선택하면 Master Node를 0으로 넘겨줌.
	// DataCtrl->DelRigd(nMasterKey, aSelKey, BngrK)에서 nMasterKey가 0인 경우
	// MasterKey와 상관없이 aSelKey를 Rigid Link에서 해제시켜줌
	if(m_nOption == 1 && bReleaseSlaveNode) nMasterKey = 0;

	T_MCON_D data;
	
	data.nSlaveType = 0;

	int nCheck = 0;
	int nSlaveType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOptionExp, nCheck);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDOF_RDO, nSlaveType);
	
	if (nCheck == 0) // explicit 
	{
			data.nSlaveType = nSlaveType;
	}
	else            //  Weighted Displacement
	{
		for (int i = 0; i < 6; i++)
		{
			data.nSlaveType *= 10;
			if (m_bDOF[i]) data.nSlaveType += 1;
		}
	}

	data.arSlave.Copy(m_aDT);
	data.nType = nCheck; 

	T_BNGR_K BngrK;
	m_cbxGroupCombo.GetSelectedBngr(BngrK);
	BOOL bSuccess=FALSE;

	T_MCON_K MconK;
	m_pDoc->m_pAttrCtrl->MakeKeyMcon(nMasterKey, nSlaveType, data.nType, BngrK, MconK);

	if (m_nOption == 0)       // Add/Replace
	{ 
		data.GroupKey = BngrK;
		bSuccess = m_pDoc->m_pDataCtrl->AddMcon(MconK, data);
	}
	else if (m_nOption == 1)  // delete
	{
		bSuccess = m_pDoc->m_pDataCtrl->DelMcon(nMasterKey, aSelKey, BngrK);
		if(bSuccess) // 삭제 메시지
		{
			if(bReleaseSlaveNode)
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Success_Delete2));
			else
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Success_Delete1));
		}
	}

	if (bSuccess) 
	{
		m_pGrid->Clear();
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
}

void CBndrMconDlg::OnChangeOption() 
{
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nCheck);
	CtrlEnableDisable(m_aCtrlDisable, nCheck == 0);
	BOOL bReleaseSlaveNode = m_chkReleaseSlaveNode.GetCheck();
	if (nCheck == 0)  // add
	{
		this->m_cbxGroupCombo.ShowHideAll(FALSE);
		m_chkReleaseSlaveNode.EnableWindow(FALSE);
		if(bReleaseSlaveNode) 
		{
			m_chkReleaseSlaveNode.SetCheck(FALSE);
			m_edtMasterNode.EnableWindow(TRUE);
		}
	}
	else // Delete
	{
		this->m_cbxGroupCombo.ShowHideAll(TRUE);
		m_chkReleaseSlaveNode.EnableWindow(TRUE);
	}
}

void CBndrMconDlg::OnChangeOptionExp() 
{
	int nCheck=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOptionExp, nCheck);
	
	BOOL bExplicit = (nCheck==0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDOF_RDO,   bExplicit);
	CDlgUtil::CtrlShowHide(this, m_aCtrlShow_EXP,  bExplicit);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDOF_CHK,  !bExplicit);
	CDlgUtil::CtrlShowHide(this, m_aCtrlShow_WD,  !bExplicit);

	m_pGrid->SetType(nCheck);
	m_pGrid->Clear();   // Grid clear   [2007-09-10] Lee, Jong Hyup (Tel: 2145, jhlee@midasit.com)
	
	int nDist=0;
	CRect rRef, rToMove;

	CWnd *pWnd = bExplicit ? GetDlgItem(IDC_TM_MCON_EXP_STC) : GetDlgItem(IDC_TM_MCON_WD_STC);
	ASSERT(pWnd);
	
	pWnd->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_MCON_GRD)->GetWindowRect(rToMove);
	nDist = rRef.bottom - rToMove.top+ globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlGridApplyGrp, nDist);
	
	Invalidate();
}

void CBndrMconDlg::OnTmReleaseSlaveNode()
{
	BOOL bReleaseSlaveNode = m_chkReleaseSlaveNode.GetCheck();
	m_edtMasterNode.EnableWindow(bReleaseSlaveNode == FALSE);
	m_edtMasterNode.SetWindowText(_T(""));
}

BOOL CBndrMconDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;
	
	T_MCON_K mconkey;
	T_MCON_D data;
	
	mconkey.keymap = key;
	m_pDoc->m_pAttrCtrl->GetMcon(mconkey, data);
	
	// type
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOptionExp, data.nType);
	OnChangeOptionExp();
	
	// master node
	CString str;
	str.Format(_T("%d"), mconkey.key.entity);
	m_edtMasterNode.SetWindowText(str);
	
	// dof
	if(data.nType==0)
	{
		for(int i=0 ; i<6 ; ++i) m_bIndependentDOF[i]=FALSE;
		m_bIndependentDOF[data.nSlaveType] = TRUE;
	}
	else
	{
		UINT temp = data.nSlaveType;
		for(int i=0 ; i<6 ; ++i)
		{
			m_bDOF[5-i] = (temp%10==1 ? TRUE : FALSE);
			temp/=10;
		}
	}
	
	
	// Group도 Setting
	T_BNGR_D  bngrD;
	m_pDoc->m_pAttrCtrl->GetBngr(data.GroupKey, bngrD);
	
	for(int i=0; i<m_cbxGroupCombo.GetCount(); i++)
	{
		CString str;
		m_cbxGroupCombo.GetLBText(i, str);
		if(str.CompareNoCase(bngrD.GroupName)==0) { m_cbxGroupCombo.SetCurSel(i); break; }
	}
	
	m_pGrid->AddData(data.arSlave);
	
	m_nOption = 0;
	
	UpdateData(FALSE);
	return TRUE;
}

// Master Node/ Group 은 세팅하지 않는 것. 
// (OnChangeMasterNodeEdt, OnSelChangeGroupCombo 자꾸 불리는 것 막도록)
BOOL CBndrMconDlg::ExternalInit2(UINT key)
{
	if(key == 0) return FALSE;
	
	T_MCON_K mconkey;
	T_MCON_D data;
	
	mconkey.keymap = key;
	m_pDoc->m_pAttrCtrl->GetMcon(mconkey, data);
	
	// type
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOptionExp, data.nType);
	OnChangeOptionExp();
	
	// master node
	//CString str;
	//str.Format("%d", mconkey.key.entity);
	//m_edtMasterNode.SetWindowText(str);
	
	// dof
	if(data.nType==0)
	{
		for(int i=0 ; i<6 ; ++i) m_bIndependentDOF[i]=FALSE;
		m_bIndependentDOF[data.nSlaveType] = TRUE;
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDOF_RDO, data.nSlaveType);
	}
	else
	{
		UINT temp = data.nSlaveType;
		for(int i=0 ; i<6 ; ++i)
		{
			m_bDOF[5-i] = (temp%10==1 ? TRUE : FALSE);
			temp/=10;
			if((5-i)==0) ((CButton*)(GetDlgItem(IDC_TM_DOF_DX_CHK)))->SetCheck(m_bDOF[5-i]);
			if((5-i)==1) ((CButton*)(GetDlgItem(IDC_TM_DOF_DY_CHK)))->SetCheck(m_bDOF[5-i]);
			if((5-i)==2) ((CButton*)(GetDlgItem(IDC_TM_DOF_DZ_CHK)))->SetCheck(m_bDOF[5-i]);
			if((5-i)==3) ((CButton*)(GetDlgItem(IDC_TM_DOF_RX_CHK)))->SetCheck(m_bDOF[5-i]);
			if((5-i)==4) ((CButton*)(GetDlgItem(IDC_TM_DOF_RY_CHK)))->SetCheck(m_bDOF[5-i]);
			if((5-i)==5) ((CButton*)(GetDlgItem(IDC_TM_DOF_RZ_CHK)))->SetCheck(m_bDOF[5-i]);
		}
	}
	
	// Group도 Setting
	/*
	T_BNGR_D  bngrD;
	m_pDoc->m_pAttrCtrl->GetBngr(data.GroupKey, bngrD);
	
	for(int i=0; i<m_cbxGroupCombo.GetCount(); i++)
	{
		CString str;
		m_cbxGroupCombo.GetLBText(i, str);
		if(str.CompareNoCase(bngrD.GroupName)==0) { m_cbxGroupCombo.SetCurSel(i); break; }
	}*/
	
	m_pGrid->AddData(data.arSlave);
	
	//m_nOption = 0;
	
	//UpdateData(FALSE);
	return TRUE;
}

void CBndrMconDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);	
}

void CBndrMconDlg::AlignControls()
{
	int nDist;
	CRect rRef, rToMove;

	GetDlgItem(IDC_TM_MCON_WD_CHK_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_MCON_EXP_CHK_GRP)->GetWindowRect(rToMove);
	nDist = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlDOF_RDO, nDist);

	GetDlgItem(IDC_TM_MCON_EXP_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_MCON_WD_STC)->GetWindowRect(rToMove);
	nDist = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlShow_WD, nDist);
}

void CBndrMconDlg::InitGrid()
{
	ASSERT(m_pGrid);
	
	m_pGrid->Initialize(&m_aDT);
}

void CBndrMconDlg::OnTmExpAdd()
{
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<T_NODE_K, T_NODE_K> aSelNodeKey;
	int nDOF = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDOF_Exp_RDO, nDOF);

	T_MCON_BASE BaseData; BaseData.Initialize();
	BaseData.nDof = nDOF;
	
	CString strNode = _T("");
	m_edtExpNode.GetWindowText(strNode);
	CStrParser::GetNodeList(strNode, aSelNodeKey);
	
	int nSize = aSelNodeKey.GetSize();
	T_MCON_DT aDT; aDT.SetSize(nSize);
	for (int i=0; i<nSize; ++i)
	{
		BaseData.NodeK = aSelNodeKey[i];
		BaseData.dCoeff = m_edtCoeff.GetEditValue();
		aDT[i] = BaseData;
	}

	m_pGrid->AddData(aDT);
}

void CBndrMconDlg::OnTmExpDel()
{
	m_pGrid->Clear();
}

void CBndrMconDlg::OnTmWDAdd()
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<T_NODE_K, T_NODE_K> aSelNodeKey;
	
	T_MCON_BASE BaseData; BaseData.Initialize();

	CString strNode = _T("");
	m_edtExpNode.GetWindowText(strNode);
	CStrParser::GetNodeList(strNode , aSelNodeKey);

	int nSize = aSelNodeKey.GetSize();
	T_MCON_DT aDT; aDT.SetSize(nSize);
	BaseData.nDof = 0;

	for (int i=0; i<nSize; ++i)
	{
		BaseData.NodeK = aSelNodeKey[i];
		BaseData.dWeight = m_edtWeight.GetEditValue();
		aDT[i] = BaseData;
	}

	m_pGrid->AddData(aDT);
}

void CBndrMconDlg::OnTmWDDel()
{
	m_pGrid->Clear();
}

void CBndrMconDlg::PostNcDestroy() 
{
	if(m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
	
	CMenuBarChildDlg::PostNcDestroy();
}

// Master Node Edit 가 변경시 데이터가 있으면 뿌려주고 없으면 Grid 초기화
void CBndrMconDlg::OnChangeMasterNodeEdt()
{
	//int nOption = 0;
	//CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);
	//if(nOption==1) return;

	T_BNGR_K BngrK;
	m_cbxGroupCombo.GetSelectedBngr(BngrK);

	CString strMasterNode = _T("");
	m_edtMasterNode.GetWindowText(strMasterNode);
	if(strMasterNode.IsEmpty()) return;
	int nMasterKey = _ttol(strMasterNode);
	if(nMasterKey==0) return;

	T_MCON_K MconK;
	T_MCON_D MconD;
	BOOL bFound = FALSE;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMcon();
	while(pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextMcon(pos, MconK, MconD);
		if(MconK.key.entity==nMasterKey && MconD.GroupKey==BngrK)
		{
			bFound = TRUE;
			break;
		}
	}

	if(bFound) ExternalInit2(MconK.keymap);
	else m_pGrid->Clear();
}

// Group 콤보박스가 변경시도 위와 동일한 동작을 한다.
void CBndrMconDlg::OnSelChangeGroupCombo()
{
	OnChangeMasterNodeEdt();
}

// Explicit 일때 Master Dof 변경시
// 해당 마스터 DOF 를 가진 Explicit 데이터 있으면 뿌려주고 없으면 그리드 초기화
void CBndrMconDlg::OnClickExpDof()
{
	//int nOption = 0;
	//CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);
	//if(nOption==1) return;

	int nDOF = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDOF_RDO, nDOF);

	T_BNGR_K BngrK;
	m_cbxGroupCombo.GetSelectedBngr(BngrK);

	CString strMasterNode = _T("");
	m_edtMasterNode.GetWindowText(strMasterNode);
	if(strMasterNode.IsEmpty()) return;
	int nMasterKey = _ttol(strMasterNode);
	if(nMasterKey==0) return;

	T_MCON_K MconK;
	T_MCON_D MconD;
	BOOL bFound = FALSE;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMcon();
	while(pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextMcon(pos, MconK, MconD);
		if(MconK.key.entity==nMasterKey && MconD.GroupKey==BngrK && MconD.nType==0 && MconD.nSlaveType==nDOF)
		{
			bFound = TRUE;
			break;
		}
	}

	if(bFound) ExternalInit2(MconK.keymap);
	else m_pGrid->Clear();
}

// 그리드에서 대화창의 마스터 DOF 를 호출하기 위해 호출
// WD 타입일때는 -1 리턴
int CBndrMconDlg::GetMasterDof()
{
	int nCheck=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOptionExp, nCheck);
	if(nCheck==1) return -1;

	int nDOF = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDOF_RDO, nDOF);
	return nDOF;
}



////////////////////////////////////////////////////////////////
