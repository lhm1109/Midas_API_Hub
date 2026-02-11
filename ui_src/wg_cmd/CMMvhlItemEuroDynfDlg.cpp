// CMMvhlItemEuroDynfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemEuroDynfDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\SpecialChar.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvhlItemEuroDynfDlg dialog

//IMPLEMENT_DYNCREATE(CMMvhlItemEuroDynfDlg, CCMChildBarBase)

CCMMvhlItemEuroDynfDlg::CCMMvhlItemEuroDynfDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMMvhlItemEuroDynfDlg::IDD)
{
	//{{AFX_DATA_INIT(CMMvhlItemEuroDynfDlg)
	//}}AFX_DATA_INIT
	m_aAutoControls.RemoveAll();
	m_aAutoControls.Add(IDC_DBAR_DYNF_AUTO_LENGTH_STC);
	m_aAutoControls.Add(IDC_DBAR_DYNF_AUTO_LENGTH_EDT);
	m_aAutoControls.Add(IDC_DBAR_DYNF_AUTO_LENGTH_UNT);
	m_aAutoControls.Add(IDC_DBAR_DYNF_AUTO_MAINTAIN_GRP);
	m_aAutoControls.Add(IDC_DBAR_DYNF_AUTO_MAINTAIN_RDO1);
	m_aAutoControls.Add(IDC_DBAR_DYNF_AUTO_MAINTAIN_RDO2);
	m_aAutoControls.Add(IDC_DBAR_DYNF_AUTO_EFF_CHK);
	m_aAutoControls.Add(IDC_DBAR_DYNF_AUTO_HEIGHT_STC);
	m_aAutoControls.Add(IDC_DBAR_DYNF_AUTO_HEIGHT_EDT);
	m_aAutoControls.Add(IDC_DBAR_DYNF_AUTO_HEIGHT_UNT);

	m_aUserControls.RemoveAll();
	m_aUserControls.Add(IDC_DBAR_DYNF_USER_FACTOR_STC);
	m_aUserControls.Add(IDC_DBAR_DYNF_USER_FACTOR_EDT);

	m_aAllControls.RemoveAll();
	m_aAllControls.Copy(m_aAutoControls);
	m_aAllControls.Copy(m_aUserControls);
	m_aAllControls.Add(IDC_DBAR_DYNF_AUTO_RDO);
	m_aAllControls.Add(IDC_DBAR_DYNF_USER_RDO);

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();

	m_nOption = 0;
	m_nAutoUser = 0;
	m_nMaintainType = 1;
	m_bEffect = FALSE;
}

CCMMvhlItemEuroDynfDlg::~CCMMvhlItemEuroDynfDlg()
{
}

void CCMMvhlItemEuroDynfDlg::DoDataExchange(CDataExchange* pDX)
{                        
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemEuroDynfDlg)
	DDX_Radio  (pDX, IDC_DBAR_DYNF_OPT_ADD_RDO,           m_nOption);
	DDX_Radio  (pDX, IDC_DBAR_DYNF_AUTO_RDO,              m_nAutoUser);
	DDX_Control(pDX, IDC_DBAR_DYNF_AUTO_LENGTH_EDT,       m_edtLength);
	DDX_Control(pDX, IDC_DBAR_DYNF_AUTO_LENGTH_UNT,       m_untLength);
	DDX_Radio  (pDX, IDC_DBAR_DYNF_AUTO_MAINTAIN_RDO1,    m_nMaintainType);
	DDX_Check  (pDX, IDC_DBAR_DYNF_AUTO_EFF_CHK,          m_bEffect);
	DDX_Control(pDX, IDC_DBAR_DYNF_AUTO_HEIGHT_EDT,       m_edtHeight);
	DDX_Control(pDX, IDC_DBAR_DYNF_AUTO_HEIGHT_UNT,       m_untHeight);
	DDX_Control(pDX, IDC_DBAR_DYNF_USER_FACTOR_EDT,       m_edtDynFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvhlItemEuroDynfDlg, CCMChildBarBase)
//{{AFX_MSG_MAP(CCMMvhlItemEuroDynfDlg)
	ON_BN_CLICKED   (IDC_DBAR_DYNF_OPT_ADD_RDO,                 OnCMMvhlItemEuroDynfOptionRdo)
	ON_BN_CLICKED   (IDC_DBAR_DYNF_OPT_DEL_RDO,                 OnCMMvhlItemEuroDynfOptionRdo)
	ON_BN_CLICKED   (IDC_DBAR_DYNF_AUTO_RDO,                    OnCMMvhlItemEuroDynfAutoUserRdo)
	ON_BN_CLICKED   (IDC_DBAR_DYNF_USER_RDO,                    OnCMMvhlItemEuroDynfAutoUserRdo)
	ON_BN_CLICKED   (IDC_DBAR_DYNF_AUTO_EFF_CHK,                OnCMMvhlItemEuroDynfReduceDynfChk)
	ON_BN_CLICKED   (IDC_DBAR_DYNF_APPLY_BTN,                   OnCMMvhlItemEuroDynfApplyBtn)
	ON_BN_CLICKED   (IDC_DBAR_DYNF_CLOSE_BTN,                   OnCMMvhlItemEuroDynfCloseBtn)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemEuroDynfDlg::InitUnit()
{
	m_untLength.SetUnitType(CUnitCtrl::m_DYNF_UNIT.dLength);
	m_untHeight.SetUnitType(CUnitCtrl::m_DYNF_UNIT.dHeightCover);
}

void CCMMvhlItemEuroDynfDlg::InitControlsText()
{
	CString strText;
	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_DYNAMIC_FACTOR_DETERM_LENGTH), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_DBAR_DYNF_AUTO_LENGTH_STC)->SetWindowText(strText);

	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_DYNAMIC_FACTOR_DYN_FACTOR), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_DBAR_DYNF_USER_FACTOR_STC)->SetWindowText(strText);
}

void CCMMvhlItemEuroDynfDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CCMMvhlItemEuroDynfDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aAllControls, !m_nOption);

	CDlgUtil::CtrlEnableDisable(this, m_aAutoControls, !m_nOption && !m_nAutoUser);
	CDlgUtil::CtrlEnableDisable(this, m_aUserControls, !m_nOption &&  m_nAutoUser);

	GetDlgItem(IDC_DBAR_DYNF_AUTO_HEIGHT_STC)->EnableWindow(m_bEffect && !m_nOption && !m_nAutoUser);
	GetDlgItem(IDC_DBAR_DYNF_AUTO_HEIGHT_EDT)->EnableWindow(m_bEffect && !m_nOption && !m_nAutoUser);
	GetDlgItem(IDC_DBAR_DYNF_AUTO_HEIGHT_UNT)->EnableWindow(m_bEffect && !m_nOption && !m_nAutoUser);
}

BOOL CCMMvhlItemEuroDynfDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.nInputType    = m_nAutoUser;
	m_Data.dLength       = m_edtLength.GetEditValue();
	m_Data.nMaintainType = m_nMaintainType;
	m_Data.bReduceEff    = m_bEffect;
	m_Data.dHeightCover  = m_edtHeight.GetEditValue();
	m_Data.dDynFactor    = m_edtDynFactor.GetEditValue();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemEuroDynfDlg message handlers

BOOL CCMMvhlItemEuroDynfDlg::OnInitDialog()
{               
	CCMChildBarBase::OnInitDialog();

	InitUnit();
	InitControlsText();
	EnableDisableControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemEuroDynfDlg::OnCMMvhlItemEuroDynfOptionRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemEuroDynfDlg::OnCMMvhlItemEuroDynfAutoUserRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemEuroDynfDlg::OnCMMvhlItemEuroDynfReduceDynfChk()
{
	EnableDisableControls();
}

void CCMMvhlItemEuroDynfDlg::OnCMMvhlItemEuroDynfApplyBtn()
{
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		if(m_nOption==0)	// add/replace
	  {
		  Dlg2Data();
			if(m_pDoc->m_pDataCtrl->AddDynf(aSelKey, m_Data))	Initial_SelectItem();
		}
	  else	// Delete
		{
	  	if(m_pDoc->m_pDataCtrl->DelDynf(aSelKey))	Initial_SelectItem();
		}
	}
	else
	{
		AfxMessageBox(_LS(IDS_CMD_MVHL_EURO_BS_DYNAMIC_FACTOR_NOT_SEL_ELEM)); 
	}
}

void CCMMvhlItemEuroDynfDlg::OnCMMvhlItemEuroDynfCloseBtn()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

