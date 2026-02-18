// CDgnCPGDesignParamSNiPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCPGDesignParamSNiPDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\DBLib.h"

#include "..\wg_main\wg_main.h"

#include "DgnCPGDesignParamBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamSNiPDlg dialog


CDgnCPGDesignParamSNiPDlg::CDgnCPGDesignParamSNiPDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnCPGDesignParamSNiPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCPGDesignParamSNiPDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(pParent);
	m_pParent = (CDgnCPGDesignParamBaseDlg*)pParent;  
	}

CDgnCPGDesignParamSNiPDlg::~CDgnCPGDesignParamSNiPDlg()
{

}

void CDgnCPGDesignParamSNiPDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCPGDesignParamSNiPDlg)
	
	DDX_Control(pDX, IDC_DGN_CPG_RUS_PARAM_BRDTYPE_CMB,  m_cmbBridgeType);

	DDX_Control(pDX, IDC_CPG_RUS_INPUT_MB7_MB8_CHK,     m_btnmb7mb8);
	DDX_Control(pDX, IDC_CPG_RUS_INPUT_MB7_EDT,         m_edtmb7);
	DDX_Control(pDX, IDC_CPG_RUS_INPUT_MB8_EDT,         m_edtmb8);

	DDX_Control(pDX, IDC_CPG_RUS_INPUT_MB9_CHK,         m_btnmb9);
	DDX_Control(pDX, IDC_CPG_RUS_INPUT_MB9_EDT,         m_edtmb9);	

	DDX_Control(pDX, IDC_CPG_RUS_INPUT_CONC_CONTRI_STR_EDT,       m_edtConcContri_Str);
	DDX_Control(pDX, IDC_CPG_RUS_INPUT_CONC_CONTRI_CRACK_EDT,  m_edtConcContri_Crack);	

	DDX_Control(pDX, IDC_DGN_CPG_RUS_FACTOR_M_ERECTION_EDT,	      m_edtFactorErection_m);
	DDX_Control(pDX, IDC_DGN_CPG_RUS_FACTOR_M_OPERATION_EDT,	  m_edtFactorOperation_m);
	DDX_Control(pDX, IDC_DGN_CPG_RUS_FACTOR_GAMMA_N_EDT,	          m_edtFactor_gamma_n);
	
	DDX_Control(pDX, IDC_DGN_CPG_RUS_PARAM_STRENGTH_CHK,		m_btnDgnParam_Sterngth);
	DDX_Control(pDX, IDC_DGN_CPG_RUS_PARAM_STABILITY_CHK,			m_btnDgnParam_Stability);
	DDX_Control(pDX, IDC_DGN_CPG_RUS_PARAM_FATIGU_CHK,					m_btnDgnParam_Fatigue);
	DDX_Control(pDX, IDC_DGN_CPG_RUS_PARAM_SHEAR_CONNECTOR_CHK,					m_btnDgnParam_Shear_Connector);  
	DDX_Control(pDX, IDC_DGN_CPG_RUS_PARAM_CRACK_CHK,					  m_btnDgnParam_Crack);

	//}}AFX_DATA_MAP
}

BOOL CDgnCPGDesignParamSNiPDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	CCheckDialogOpen::SetSoftwareRendering();

	// Initialize Control Data
	InitCtrl();
	InitUnit();

	Data2Dlg();	
	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDgnCPGDesignParamSNiPDlg::Data2Dlg()
{
	m_pDoc->m_pAttrCtrl->GetDgnCpgd(*m_pData);

	m_cmbBridgeType.SetCurSel(m_pData->nBridgeType);

	m_btnmb7mb8.SetCheck(m_pData->bConsidermb7bm8);
	CDlgUtil::CtrlRadioSetCheck(this, m_armb7mb8Type, m_pData->nConsidermb7mb8); 
	m_edtmb7.SetEditUnit(m_pData->dmb7);
	m_edtmb8.SetEditUnit(m_pData->dmb8);

	m_btnmb9.SetCheck(m_pData->bConsidermb9);

	CDlgUtil::CtrlRadioSetCheck(this, m_armb9Type, m_pData->nConsidermb9); 
	m_edtmb9.SetEditUnit(m_pData->dmb9);

	m_edtConcContri_Str.SetEditUnit(m_pData->dPsi_1stGroup);
	m_edtConcContri_Crack.SetEditUnit(m_pData->dPsi_2ndGroup);

	m_edtFactorErection_m.SetEditUnit(m_pData->dBehaviorConF4Erection);
	m_edtFactorOperation_m.SetEditUnit(m_pData->dBehaviorConF4Operation);
	m_edtFactor_gamma_n.SetEditUnit(m_pData->dGamman);

	CDlgUtil::CtrlRadioSetCheck(this, m_arStressComponentType, m_pData->nComponentSigxType); 

	m_btnDgnParam_Sterngth.SetCheck(m_pData->bDesignParameter[0]);
	m_btnDgnParam_Stability.SetCheck(m_pData->bDesignParameter[1]);
	m_btnDgnParam_Fatigue.SetCheck(m_pData->bDesignParameter[2]);
	m_btnDgnParam_Shear_Connector.SetCheck(m_pData->bDesignParameter[3]);
	m_btnDgnParam_Crack.SetCheck(m_pData->bDesignParameter[4]);

	EnableDisableControls();
	UpdateData(FALSE);
}

void CDgnCPGDesignParamSNiPDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->nBridgeType = m_cmbBridgeType.GetItemData(m_cmbBridgeType.GetCurSel());
	m_bmb7mb8 = m_btnmb7mb8.GetCheck();
	m_pData->bConsidermb7bm8 = m_bmb7mb8;
	CDlgUtil::CtrlRadioGetCheck(this, m_armb7mb8Type, m_pData->nConsidermb7mb8);
	m_pData->dmb7 = m_edtmb7.GetEditValue();
	m_pData->dmb8 = m_edtmb8.GetEditValue();


	m_bmb9 = m_btnmb9.GetCheck();
	m_pData->bConsidermb9 = m_bmb9;
	CDlgUtil::CtrlRadioGetCheck(this, m_armb9Type, m_pData->nConsidermb9);
	m_pData->dmb9 = m_edtmb9.GetEditValue();

	m_pData->dPsi_1stGroup = m_edtConcContri_Str.GetEditValue();
	m_pData->dPsi_2ndGroup = m_edtConcContri_Crack.GetEditValue();

	m_pData->dBehaviorConF4Erection = m_edtFactorErection_m.GetEditValue();
	m_pData->dBehaviorConF4Operation = m_edtFactorOperation_m.GetEditValue();
	m_pData->dGamman = m_edtFactor_gamma_n.GetEditValue();

	CDlgUtil::CtrlRadioGetCheck(this, m_arStressComponentType, m_pData->nComponentSigxType);

	m_pData->bDesignParameter[0] = m_btnDgnParam_Sterngth.GetCheck();
	m_pData->bDesignParameter[1] = m_btnDgnParam_Stability.GetCheck();
	m_pData->bDesignParameter[2] = m_btnDgnParam_Fatigue.GetCheck();
	m_pData->bDesignParameter[3] = m_btnDgnParam_Shear_Connector.GetCheck();
	m_pData->bDesignParameter[4] = m_btnDgnParam_Crack.GetCheck();
}


BEGIN_MESSAGE_MAP(CDgnCPGDesignParamSNiPDlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnCPGDesignParamSNiPDlg)

	ON_BN_CLICKED(IDC_CPG_RUS_INPUT_MB7_MB8_CHK,  OnConditionFactormb7mb8Chk)
	ON_BN_CLICKED(IDC_CPG_RUS_INPUT_MB7_MB8_RDO1, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_CPG_RUS_INPUT_MB7_MB8_RDO2, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_CPG_RUS_INPUT_MB7_MB8_RDO3, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_CPG_RUS_INPUT_MB7_MB8_RDO4, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_CPG_RUS_INPUT_MB9_CHK,      OnConditionFactormb9Chk)
	ON_BN_CLICKED(IDC_CPG_RUS_INPUT_MB9_RDO1,     OnConditionFactormb9TypeRdo)
	ON_BN_CLICKED(IDC_CPG_RUS_INPUT_MB9_RDO2,     OnConditionFactormb9TypeRdo)

	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_DGN_CPG_RUS_PARAM_BRDTYPE_CMB, OnSelchangeBrdtypeCmb)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamSNiPDlg message handlers

void CDgnCPGDesignParamSNiPDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();		
	CChildDialog::OnOK();		
}

void CDgnCPGDesignParamSNiPDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CChildDialog::OnCancel();
}

void CDgnCPGDesignParamSNiPDlg::OnConditionFactormb7mb8Chk()
{
	EnableDisableControls();
}

void CDgnCPGDesignParamSNiPDlg::OnConditionFactormb7mb8TypeRdo()
{
	EnableDisableControls();
}

void CDgnCPGDesignParamSNiPDlg::OnConditionFactormb9Chk()
{
	EnableDisableControls();
}

void CDgnCPGDesignParamSNiPDlg::OnConditionFactormb9TypeRdo()
{
	EnableDisableControls();
}

void CDgnCPGDesignParamSNiPDlg::InitCtrl()
{
	m_cmbBridgeType.ResetContent();
	m_cmbBridgeType.SetItemData(0, m_cmbBridgeType.AddString(_LS(IDS_DGN_BRIDGE_TYPE_ROADWAY)));
	m_cmbBridgeType.SetItemData(1, m_cmbBridgeType.AddString(_LS(IDS_DGN_BRIDGE_TYPE_RAILROAD)));  
	m_cmbBridgeType.SetCurSel(0);

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbBridgeType);

	m_armb7mb8Type.RemoveAll();
	m_armb7mb8Type.Add(IDC_CPG_RUS_INPUT_MB7_MB8_RDO1);
	m_armb7mb8Type.Add(IDC_CPG_RUS_INPUT_MB7_MB8_RDO2);
	m_armb7mb8Type.Add(IDC_CPG_RUS_INPUT_MB7_MB8_RDO3);
	m_armb7mb8Type.Add(IDC_CPG_RUS_INPUT_MB7_MB8_RDO4);

	m_armb9Type.RemoveAll();
	m_armb9Type.Add(IDC_CPG_RUS_INPUT_MB9_RDO1);
	m_armb9Type.Add(IDC_CPG_RUS_INPUT_MB9_RDO2);

	m_arStressComponentType.RemoveAll();
	m_arStressComponentType.Add(IDC_DGN_CPG_RUS_PARAM_COMPONENT1_RDO);
	m_arStressComponentType.Add(IDC_DGN_CPG_RUS_PARAM_COMPONENT2_RDO);
	m_arStressComponentType.Add(IDC_DGN_CPG_RUS_PARAM_COMPONENT3_RDO);
	m_arStressComponentType.Add(IDC_DGN_CPG_RUS_PARAM_COMPONENT4_RDO);

	CDlgUtil::CtrlRadioSetCheck(this, m_arStressComponentType, m_pData->nComponentSigxType); 

	double dPsi_Default = 0.5;
	m_edtConcContri_Str.SetEditUnit(dPsi_Default);
	m_edtConcContri_Crack.SetEditUnit(dPsi_Default);

}

void CDgnCPGDesignParamSNiPDlg::InitUnit()
{
	m_edtmb7.SetEditUnit(D_UNITSYS_NONE);
	m_edtmb8.SetEditUnit(D_UNITSYS_NONE);
	m_edtmb9.SetEditUnit(D_UNITSYS_NONE);

	m_edtConcContri_Str.SetEditUnit(D_UNITSYS_NONE);
	m_edtConcContri_Crack.SetEditUnit(D_UNITSYS_NONE);

	m_edtFactorErection_m.SetEditUnit(D_UNITSYS_NONE);
	m_edtFactorOperation_m.SetEditUnit(D_UNITSYS_NONE);
	m_edtFactor_gamma_n.SetEditUnit(D_UNITSYS_NONE);
}


void CDgnCPGDesignParamSNiPDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	BOOL bEnablemb7mb8Ctrl = m_btnmb7mb8.GetCheck();
	int nmb7mb8Type = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_armb7mb8Type, nmb7mb8Type); 

	GetDlgItem(m_armb7mb8Type[0])->EnableWindow(bEnablemb7mb8Ctrl);
	GetDlgItem(m_armb7mb8Type[1])->EnableWindow(bEnablemb7mb8Ctrl);
	GetDlgItem(m_armb7mb8Type[2])->EnableWindow(bEnablemb7mb8Ctrl);
	GetDlgItem(m_armb7mb8Type[3])->EnableWindow(bEnablemb7mb8Ctrl);
	GetDlgItem(IDC_CPG_RUS_INPUT_MB7_STC)->EnableWindow(bEnablemb7mb8Ctrl && nmb7mb8Type==3);
	GetDlgItem(IDC_CPG_RUS_INPUT_MB7_EDT)->EnableWindow(bEnablemb7mb8Ctrl && nmb7mb8Type==3);
	GetDlgItem(IDC_CPG_RUS_INPUT_MB8_STC)->EnableWindow(bEnablemb7mb8Ctrl && nmb7mb8Type==3);
	GetDlgItem(IDC_CPG_RUS_INPUT_MB8_EDT)->EnableWindow(bEnablemb7mb8Ctrl && nmb7mb8Type==3);

	BOOL bEnablemb9Ctrl =  m_btnmb9.GetCheck();
	int nmb9Type =  0;
	CDlgUtil::CtrlRadioGetCheck(this, m_armb9Type, nmb9Type); 

	GetDlgItem(m_armb9Type[0])->EnableWindow(bEnablemb9Ctrl);
	GetDlgItem(m_armb9Type[1])->EnableWindow(bEnablemb9Ctrl);
	GetDlgItem(IDC_CPG_RUS_INPUT_MB9_STC )->EnableWindow(bEnablemb9Ctrl && nmb9Type==1);
	GetDlgItem(IDC_CPG_RUS_INPUT_MB9_EDT )->EnableWindow(bEnablemb9Ctrl && nmb9Type==1);
}

void CDgnCPGDesignParamSNiPDlg::OnSelchangeBrdtypeCmb()
{
	int nBridgeType = m_cmbBridgeType.GetItemData(m_cmbBridgeType.GetCurSel());
	switch (nBridgeType)
	{
	case 0:
		{
			m_edtConcContri_Str.SetEditUnit(0.5);
			m_edtConcContri_Crack.SetEditUnit(0.5);
			break;
		}
	case 1:
		{
			m_edtConcContri_Str.SetEditUnit(1.0);
			m_edtConcContri_Crack.SetEditUnit(0.75);
			break;
		}
	default: ASSERT(0);    break;
	}
}
