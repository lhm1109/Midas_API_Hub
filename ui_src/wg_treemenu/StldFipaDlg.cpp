// StldFipaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldFipaDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_base\DlgUtil.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CMenuBarChildDlg
/////////////////////////////////////////////////////////////////////////////
// CStldFipaDlg dialog


CStldFipaDlg::CStldFipaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStldFipaDlg::IDD, pParent)
{
	m_nOption = 0;
	m_dfireResTime = 0.0;
	m_dRadiance = 0.0;
	m_dSHeat = 0.0;
	m_wndThicknessEdit = 0.0;
	m_wndResisEdit = 0.0;
	m_wndConDucEdit = 0.0;
	m_wndDensityEdit = 0.0;
	{
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_RESISTANCE_EDIT);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_RESISTANCE_STATIC);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_RESISTANCE_UNIT);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_THICKNESS_EDIT);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_THICKNESS_STATIC);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_THICKNESS_UNIT);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_CONDUCTIVITY_EDIT);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_CONDUCTIVITY_STATIC);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_CONDUCTIVITY_UNIT);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_DENSITY_EDIT);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_DENSITY_STATIC);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_DENSITY_UNIT);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_SHEAT_EDIT);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_SHEAT_STATIC);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_SHEAT_UNIT);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_RADIANCE_EDIT);
		m_aCtrlAll.Add(IDC_WG_TM_FIPA_RADIANCE_STATIC);
	}
	{
		//Resistance
		m_aCtrlEdit[T_FIPA_D::LIGHT_EXPANSION].Add(IDC_WG_TM_FIPA_RESISTANCE_EDIT);
		m_aCtrlText[T_FIPA_D::LIGHT_EXPANSION].Add(IDC_WG_TM_FIPA_RESISTANCE_STATIC);
		m_aCtrlText[T_FIPA_D::LIGHT_EXPANSION].Add(IDC_WG_TM_FIPA_RESISTANCE_UNIT);
	}
	{
		//ThickNess
		m_aCtrlEdit[T_FIPA_D::LIGHT_NOEXPANSION].Add(IDC_WG_TM_FIPA_THICKNESS_EDIT);
		m_aCtrlText[T_FIPA_D::LIGHT_NOEXPANSION].Add(IDC_WG_TM_FIPA_THICKNESS_STATIC);
		m_aCtrlText[T_FIPA_D::LIGHT_NOEXPANSION].Add(IDC_WG_TM_FIPA_THICKNESS_UNIT);
		//Conductivity
		m_aCtrlEdit[T_FIPA_D::LIGHT_NOEXPANSION].Add(IDC_WG_TM_FIPA_CONDUCTIVITY_EDIT);
		m_aCtrlText[T_FIPA_D::LIGHT_NOEXPANSION].Add(IDC_WG_TM_FIPA_CONDUCTIVITY_STATIC);
		m_aCtrlText[T_FIPA_D::LIGHT_NOEXPANSION].Add(IDC_WG_TM_FIPA_CONDUCTIVITY_UNIT);
	}
	{
		//ThickNess
		m_aCtrlEdit[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_THICKNESS_EDIT);
		m_aCtrlText[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_THICKNESS_STATIC);
		m_aCtrlText[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_THICKNESS_UNIT);
		//Conductivity
		m_aCtrlEdit[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_CONDUCTIVITY_EDIT);
		m_aCtrlText[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_CONDUCTIVITY_STATIC);
		m_aCtrlText[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_CONDUCTIVITY_UNIT);

		//dDensity
		m_aCtrlEdit[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_DENSITY_EDIT);
		m_aCtrlText[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_DENSITY_STATIC);
		m_aCtrlText[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_DENSITY_UNIT);
		//specific heat 
		m_aCtrlEdit[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_SHEAT_EDIT);
		m_aCtrlText[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_SHEAT_STATIC);
		m_aCtrlText[T_FIPA_D::NO_LIGHT_FIRECONTAIN].Add(IDC_WG_TM_FIPA_SHEAT_UNIT);
	}
	{
		//Radiance
		m_aCtrlEdit[T_FIPA_D::NO_FIRECONTAIN].Add(IDC_WG_TM_FIPA_RADIANCE_EDIT);
		m_aCtrlText[T_FIPA_D::NO_FIRECONTAIN].Add(IDC_WG_TM_FIPA_RADIANCE_STATIC);
	}
}


void CStldFipaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WG_TM_FIPA_TIME_EDIT, m_dfireResTime);
	DDX_Text(pDX, IDC_WG_TM_FIPA_RADIANCE_EDIT, m_dRadiance);
	DDX_Text(pDX, IDC_WG_TM_FIPA_SHEAT_EDIT, m_dSHeat);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Control(pDX, IDC_WG_TM_FIPA_TYPE_COMBO, m_wndTypeCobx);


	DDX_Text(pDX, IDC_WG_TM_FIPA_THICKNESS_EDIT, m_wndThicknessEdit);
	DDX_Text(pDX, IDC_WG_TM_FIPA_RESISTANCE_EDIT, m_wndResisEdit);
	DDX_Text(pDX, IDC_WG_TM_FIPA_CONDUCTIVITY_EDIT, m_wndConDucEdit);
	DDX_Text(pDX, IDC_WG_TM_FIPA_DENSITY_EDIT, m_wndDensityEdit);
}


BEGIN_MESSAGE_MAP(CStldFipaDlg, CDialog)
	//{{AFX_MSG_MAP(CStldFipaDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_WG_TM_FIPA_TYPE_COMBO, &CStldFipaDlg::OnSelFipaType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldFipaDlg message handlers
BOOL CStldFipaDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	CString strType[] = {_LS(IDS_TM_TREEMENU_FIPA_TYPE1),_LS(IDS_TM_TREEMENU_FIPA_TYPE2),_LS(IDS_TM_TREEMENU_FIPA_TYPE3),_LS(IDS_TM_TREEMENU_FIPA_TYPE4)};
	for(int i = 0;i < 4;i++)
		m_wndTypeCobx.SetItemData(m_wndTypeCobx.AddString(strType[i]),i);
	m_wndTypeCobx.SetCurSel(0);
	m_dfireResTime = 2.0;
	UpdateData(FALSE);
	OnSelFipaType();
	return TRUE;
}

BOOL CStldFipaDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_data.Initialize();
	m_data.firetype			= (T_FIPA_D::FireType)m_wndTypeCobx.GetItemData(m_wndTypeCobx.GetCurSel());
	m_data.dfireResTime		= m_dfireResTime;
	if(m_data.firetype == T_FIPA_D::LIGHT_NOEXPANSION || m_data.firetype == T_FIPA_D::NO_LIGHT_FIRECONTAIN)
	{
		m_data.dConductivity	= m_wndConDucEdit;
		m_data.dThickness		= m_wndThicknessEdit;
		m_data.dDensity			= m_wndDensityEdit;
		m_data.dSHeat			= m_dSHeat;
	}
	if(m_data.firetype == T_FIPA_D::NO_FIRECONTAIN)
		m_data.dRadiance		= m_dRadiance;
	if(m_data.firetype == T_FIPA_D::LIGHT_EXPANSION)
		m_data.dResistance		= m_wndResisEdit;
	return TRUE;
}

void CStldFipaDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
		return;
	}
	Dlg2Data();
	BOOL bSuccess;
	if (m_nOption == 0)
	{
		bSuccess = m_pDoc->m_pDataCtrl->AddFipa(aSelKey, m_data);
	}
	else
	{
		bSuccess = m_pDoc->m_pDataCtrl->DelFipa(aSelKey);
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	
}

void CStldFipaDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldFipaDlg::OnSelFipaType()
{
	T_FIPA_D::FireType nType = (T_FIPA_D::FireType)m_wndTypeCobx.GetItemData(m_wndTypeCobx.GetCurSel());
	for(int i = 0;i < m_aCtrlAll.GetSize();i++)
		CDlgUtil::CtrlEnableDisable(this,m_aCtrlAll[i],FALSE);
	for(int i = 0;i < m_aCtrlEdit[nType].GetSize();i++)
		CDlgUtil::CtrlEnableDisable(this,m_aCtrlEdit[nType][i],TRUE);
	for(int i = 0;i < m_aCtrlText[nType].GetSize();i++)
		CDlgUtil::CtrlEnableDisable(this,m_aCtrlText[nType][i],TRUE);

	for(int i = 0;i < 4;i++)
	{
		for(int j = 0;j < m_aCtrlEdit[i].GetSize();j++)
			GetDlgItem(m_aCtrlEdit[i][j])->SetWindowText(_T("0"));
	}
	if(nType == T_FIPA_D::LIGHT_EXPANSION)
	{
		GetDlgItem(m_aCtrlEdit[nType][0])->SetWindowText(_T("0.17"));//Resistance
	}
	else if(nType == T_FIPA_D::LIGHT_NOEXPANSION)
	{
		GetDlgItem(m_aCtrlEdit[nType][0])->SetWindowText(_T("10"));//ThickNess
		GetDlgItem(m_aCtrlEdit[nType][1])->SetWindowText(_T("0.1"));//Conductivity
	}
	else if(nType == T_FIPA_D::NO_LIGHT_FIRECONTAIN)
	{
		GetDlgItem(m_aCtrlEdit[nType][0])->SetWindowText(_T("25"));//ThickNess
		GetDlgItem(m_aCtrlEdit[nType][1])->SetWindowText(_T("1.5"));//Conductivity
		GetDlgItem(m_aCtrlEdit[nType][2])->SetWindowText(_T("2200"));//dDensity
		GetDlgItem(m_aCtrlEdit[nType][3])->SetWindowText(_T("0.95"));//specific heat 
	}
	else if(nType == T_FIPA_D::NO_FIRECONTAIN)
	{
		GetDlgItem(m_aCtrlEdit[nType][0])->SetWindowText(_T("0.7"));//Radiance
	}
	////ThickNess
	//BOOL bEnable = (nType == T_FIPA_D::LIGHT_NOEXPANSION) || (nType == T_FIPA_D::NO_LIGHT_FIRECONTAIN);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_THICKNESS_STATIC,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_THICKNESS_EDIT,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_THICKNESS_UNIT,bEnable);
	////Conductivity
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_CONDUCTIVITY_STATIC,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_CONDUCTIVITY_EDIT,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_CONDUCTIVITY_UNIT,bEnable);
	////dDensity&  specific heat 
	//bEnable = nType == T_FIPA_D::NO_LIGHT_FIRECONTAIN;
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_DENSITY_STATIC,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_DENSITY_EDIT,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_DENSITY_UNIT,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_SHEAT_STATIC,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_SHEAT_EDIT,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_SHEAT_UNIT,bEnable);
	////Radiance
	//bEnable = nType == T_FIPA_D::NO_FIRECONTAIN;
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_RADIANCE_STATIC,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_RADIANCE_EDIT,bEnable);
	////Resistance
	//bEnable = nType == T_FIPA_D::LIGHT_EXPANSION;
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_RESISTANCE_STATIC,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_RESISTANCE_EDIT,bEnable);
	//CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FIPA_RESISTANCE_UNIT,bEnable);
}
