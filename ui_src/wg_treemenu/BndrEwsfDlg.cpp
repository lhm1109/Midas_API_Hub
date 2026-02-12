// BndrEwsfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrEwsfDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrEwsfDlg dialog


CBndrEwsfDlg::CBndrEwsfDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrEwsfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrEwsfDlg)
	m_nOption = 0;
	m_bJ = FALSE;
	//}}AFX_DATA_INIT
	m_aCtrlScaleFactor_i.Add(IDC_TM_SCALE_FACTOR_TITLE);
	m_aCtrlScaleFactor_i.Add(IDC_TM_SCALE_FACTOR_EDIT);
	m_aCtrlScaleFactor_i.Add(IDC_TM_SCALE_FACTOR_TITLE2);
	m_aCtrlScaleFactor_i.Add(IDC_TM_ZTOP_FACTOR_EDIT);
	m_aCtrlScaleFactor_i.Add(IDC_TM_SCALE_FACTOR_TITLE3);
	m_aCtrlScaleFactor_i.Add(IDC_TM_ZBOT_FACTOR_EDIT);
	m_aCtrlScaleFactor_i.Add(IDC_TM_IEND_FRM);
	
	m_aCtrlScaleFactor_j.Add(IDC_TM_SCALE_FACTOR_TITLE4);
	m_aCtrlScaleFactor_j.Add(IDC_TM_SCALE_FACTOR_EDIT2);
	m_aCtrlScaleFactor_j.Add(IDC_TM_SCALE_FACTOR_TITLE5);
	m_aCtrlScaleFactor_j.Add(IDC_TM_ZTOP_FACTOR_EDIT2);
	m_aCtrlScaleFactor_j.Add(IDC_TM_SCALE_FACTOR_TITLE6);
	m_aCtrlScaleFactor_j.Add(IDC_TM_ZBOT_FACTOR_EDIT2);
}


void CBndrEwsfDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrEwsfDlg)
	DDX_Radio  (pDX, IDC_TM_OPT_REPLACE,        m_nOption);
	DDX_Control(pDX, IDC_TM_GROUP_COMBO,        m_wndGroupCombo);
	DDX_Control(pDX, IDC_TM_SCALE_FACTOR_EDIT,  m_wndScaleFactor);
	DDX_Control(pDX, IDC_TM_ZTOP_FACTOR_EDIT,   m_ZTopEdt);
	DDX_Control(pDX, IDC_TM_ZBOT_FACTOR_EDIT,   m_ZBotEdt);
	DDX_Control(pDX, IDC_TM_SCALE_FACTOR_EDIT2, m_wndScaleFactor_j);
	DDX_Control(pDX, IDC_TM_ZTOP_FACTOR_EDIT2,  m_ZTopEdt_j);
	DDX_Control(pDX, IDC_TM_ZBOT_FACTOR_EDIT2,  m_ZBotEdt_j);
	DDX_Check  (pDX, IDC_TM_JEND_CHK,           m_bJ);
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC3, m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBndrEwsfDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrEwsfDlg)
	/*ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)*/
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnChangeOption)
	/*ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)*/
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_BN_CLICKED(IDC_TM_JEND_CHK, OnCheckJ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CBndrEwsfDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_EWSF_K Key;
	T_EWSF_D data;
	Key.keymap = key;
	m_pDoc->m_pAttrCtrl->GetEwsf(Key, data);

	m_wndGroupCombo.ChangeSelect(data.GroupKey);
	m_wndScaleFactor.SetEditUnit(data.EwsfSubY.dScaleFactor);
	m_ZTopEdt.SetEditUnit(data.EwsfSubY.dZtopFactor);
	m_ZBotEdt.SetEditUnit(data.EwsfSubY.dZbotFactor);
	m_nOption = 0;

	m_bJ = data.EwsfSubY.bJ;
	if(m_bJ)
	{
		m_wndScaleFactor_j.SetEditUnit(data.EwsfSubY.dScaleFactor_j);
		m_ZTopEdt_j.SetEditUnit(data.EwsfSubY.dZtopFactor_j);
		m_ZBotEdt_j.SetEditUnit(data.EwsfSubY.dZbotFactor_j);
	}
	else
	{
		m_wndScaleFactor_j.SetEditUnit(data.EwsfSubY.dScaleFactor);
		m_ZTopEdt_j.SetEditUnit(data.EwsfSubY.dZtopFactor);
		m_ZBotEdt_j.SetEditUnit(data.EwsfSubY.dZbotFactor);
	}
	UpdateData(FALSE);

	OnChangeOption();

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBndrEwsfDlg message handlers
BOOL CBndrEwsfDlg::OnInitDialog() 
{
	frx::MInitCombo initCombo;
	SetRedraw(FALSE);
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add your specialized code here and/or call the base class
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_nOption = 0;
	m_wndScaleFactor.SetUnitType(0);
	m_wndScaleFactor.SetEditUnit(1.0);
	m_ZTopEdt.SetUnitType(0);
	m_ZTopEdt.SetEditUnit(1.0);
	m_ZBotEdt.SetUnitType(0);
	m_ZBotEdt.SetEditUnit(1.0);

	m_bJ = FALSE;
	m_wndScaleFactor_j.SetUnitType(0);
	m_wndScaleFactor_j.SetEditUnit(1.0);
	m_ZTopEdt_j.SetUnitType(0);
	m_ZTopEdt_j.SetEditUnit(1.0);
	m_ZBotEdt_j.SetUnitType(0);
	m_ZBotEdt_j.SetEditUnit(1.0);
	
	UpdateData(FALSE);
	m_wndPicture.SetImage("SVG\\illustration\\Dialog\\tm_ewsf 1.svg");
	OnChangeOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrEwsfDlg::Execute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	T_ELEM_D ElemData;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
		if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp)) aSelKey.RemoveAt(i);
	}

	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_beam_element_));
		return;
	}
	T_EWSF_D data;
	data.Initialize();

	m_wndGroupCombo.GetSelectedBngr(data.GroupKey);
	data.EwsfSubY.dScaleFactor = m_wndScaleFactor.GetEditValue();
	data.EwsfSubY.dZtopFactor = m_ZTopEdt.GetEditValue();
	data.EwsfSubY.dZbotFactor = m_ZBotEdt.GetEditValue();

	data.EwsfSubY.bJ = m_bJ;
	if(m_bJ)
	{
		data.EwsfSubY.dScaleFactor_j = m_wndScaleFactor_j.GetEditValue();
		data.EwsfSubY.dZtopFactor_j = m_ZTopEdt_j.GetEditValue();
		data.EwsfSubY.dZbotFactor_j = m_ZBotEdt_j.GetEditValue();
	}
	else
	{
		data.EwsfSubY.dScaleFactor_j = data.EwsfSubY.dScaleFactor;
		data.EwsfSubY.dZtopFactor_j = data.EwsfSubY.dZtopFactor;
		data.EwsfSubY.dZbotFactor_j = data.EwsfSubY.dZbotFactor;
	}
	
	BOOL bSuccess=FALSE;
	if (m_nOption == 0) // Add or Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddEwsf(aSelKey, data);	
	else if (m_nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelEwsf(aSelKey, data.GroupKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CBndrEwsfDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CBndrEwsfDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wndGroupCombo.ShowHideAll(m_nOption==1);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlScaleFactor_i, m_nOption==0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlScaleFactor_j, m_nOption==0 && m_bJ);
	GetDlgItem(IDC_TM_JEND_CHK)->EnableWindow(m_nOption==0);
}

void CBndrEwsfDlg::OnTmDefineGroupButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_DEF_BNDR_GROUP,0));	
}

void CBndrEwsfDlg::OnCheckJ()
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlScaleFactor_j, m_nOption==0 && m_bJ);
}