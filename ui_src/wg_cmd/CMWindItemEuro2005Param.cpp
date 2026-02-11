// CMWindItemEuro2005Param.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemEuro2005Param.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro2005Param dialog


CCMWindItemEuro2005Param::CCMWindItemEuro2005Param(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMWindItemEuro2005Param::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMWindItemEuro2005Param)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMWindItemEuro2005Param::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemEuro2005Param)
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_ORO_CHK, m_chkOrography);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_NEI_CHK, m_chkNeighbor);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_DIS_CHK, m_chkDisplace);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_ORO_TYPE_CMB, m_cobxOroType);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_BLDG_LOC_CMB, m_cobxBldgLoc);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_ORO_H_EDIT, m_editOroH);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_ORO_LU_EDIT, m_editOroLu);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_ORO_LD_EDIT, m_editOroLd);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_ORO_X_EDIT, m_editOroX);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_ORO_H_UNIT, m_unitOroH);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_ORO_LU_UNIT, m_unitOroLu);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_ORO_LD_UNIT, m_unitOroLd);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_ORO_X_UNIT, m_unitOroX);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_NEI_H_EDIT, m_editNeiH);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_NEI_HA_EDIT, m_editNeiHa);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_NEI_X_EDIT, m_editNeiX);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_NEI_DL_EDIT, m_editNeiDl);  
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_NEI_HH_EDIT, m_editNeiHh);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_NEI_H_UNIT, m_unitNeiH);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_NEI_HA_UNIT, m_unitNeiHa);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_NEI_X_UNIT, m_unitNeiX);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_NEI_DL_UNIT, m_unitNeiDl);  
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_NEI_HH_UNIT, m_unitNeiHh);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_DIS_TC_EDIT, m_editDisTC);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_DIS_HA_EDIT, m_editDisHa);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_DIS_X_EDIT, m_editDisX);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_DIS_HA_UNIT, m_unitDisHa);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_DIS_X_UNIT, m_unitDisX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemEuro2005Param, CDialogMove)
	//{{AFX_MSG_MAP(CCMWindItemEuro2005Param)
	ON_CBN_SELCHANGE(IDC_CMD_WIND_EURO05_BLDG_LOC_CMB, OnSelchangeCmdBldgLocCmb)
	ON_CBN_SELCHANGE(IDC_CMD_WIND_EURO05_ORO_TYPE_CMB, OnSelchangeCmdOroTypeCmb)
	ON_BN_CLICKED(IDC_CMD_WIND_EURO05_ORO_CHK, OnCmdOrographyCheck)
	ON_BN_CLICKED(IDC_CMD_WIND_EURO05_NEI_CHK, OnCmdNeighborCheck)
	ON_BN_CLICKED(IDC_CMD_WIND_EURO05_DIS_CHK, OnCmdDisplaceCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro2005Param message handlers

BOOL CCMWindItemEuro2005Param::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitControls();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindItemEuro2005Param::OnOK() 
{
	// TODO: Add extra validation here

	if (!Dlg2Data()) return;
	
	CDialogMove::OnOK();
}

void CCMWindItemEuro2005Param::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}


//---------------------------------------------------------------------------
void CCMWindItemEuro2005Param::InitControls()
{
	InitOroTypeCobx();
	InitBldgLocCobx();  

	m_chkDisplace.EnableWindow(m_nTerrain == 4 || m_nTerrain>=10);
	m_editDisHa.EnableWindow(m_nTerrain == 4 || m_nTerrain>=10);
	m_editDisX.EnableWindow(m_nTerrain == 4 || m_nTerrain>=10);

	m_unitOroH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitOroLu.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitOroLd.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitOroX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editOroH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editOroLu.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editOroLd.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editOroX.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_unitNeiH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitNeiHa.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitNeiX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitNeiDl.SetUnitType(D_UNITSYS_BASE_LENGTH);  
	m_unitNeiHh.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editNeiH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editNeiHa.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editNeiX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editNeiDl.SetUnitType(D_UNITSYS_BASE_LENGTH);  
	m_editNeiHh.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_unitDisHa.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitDisX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editDisHa.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editDisX.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMWindItemEuro2005Param::Data2Dlg()
{
	SetTerrainCategory();

	m_chkOrography.SetCheck(m_bOrography);
	m_chkNeighbor.SetCheck(m_bNeighbor);
	m_chkDisplace.SetCheck(m_bDisplace);
	m_cobxOroType.SetCurSel(m_nOroType);
	m_cobxBldgLoc.SetCurSel(m_nBldgLoc);
	m_editOroH.SetEditUnit (m_dOroH );
	m_editOroLu.SetEditUnit(m_dOroLu);
	m_editOroLd.SetEditUnit(m_dOroLd);
	m_editOroX.SetEditUnit (m_dOroX );
	m_editNeiH.SetEditUnit (m_dNeiH );
	m_editNeiHa.SetEditUnit(m_dNeiHa);
	m_editNeiX.SetEditUnit (m_dNeiX );
	m_editNeiDl.SetEditUnit(m_dNeiDl);  
	m_editNeiHh.SetEditUnit(m_dNeiHh);
	m_editDisHa.SetEditUnit(m_dDisHa);
	m_editDisX.SetEditUnit (m_dDisX );
	OnCmdOrographyCheck();
	OnCmdNeighborCheck();
	OnCmdDisplaceCheck();
}

BOOL CCMWindItemEuro2005Param::Dlg2Data()
{
	m_bOrography    = m_chkOrography.GetCheck();
	m_bNeighbor     = m_chkNeighbor.GetCheck();
	m_bDisplace     = m_chkDisplace.GetCheck();
	m_nOroType      = m_cobxOroType.GetCurSel();
	m_nBldgLoc      = m_cobxBldgLoc.GetCurSel();

	m_dOroH  = m_editOroH.GetEditValue ();   
	m_dOroLu = m_editOroLu.GetEditValue(); 
	m_dOroLd = m_editOroLd.GetEditValue(); 
	m_dOroX  = m_editOroX.GetEditValue ();   
	m_dNeiHa = m_editNeiHa.GetEditValue(); 
	m_dNeiX  = m_editNeiX.GetEditValue (); 
	m_dNeiDl = m_editNeiDl.GetEditValue();   
	m_dNeiHh = m_editNeiHh.GetEditValue(); 
	m_dDisHa = m_editDisHa.GetEditValue();
	m_dDisX  = m_editDisX.GetEditValue ();
	
	if(m_bOrography)
	{
		if(m_dOroH < 0.) {AfxMessageBox(_T("Error! Height of Topographic Feature")); return FALSE;}
		if(m_dOroLu <= 0.) {AfxMessageBox(_T("Error! Length of Upwind Slope")); return FALSE;} 
		if(m_nBldgLoc == 1 && m_nOroType == 1)  // upwind 
		{        
			if(m_dOroLd <= 0.) {AfxMessageBox(_T("Error! Length of Downwind Slope")); return FALSE;}
		}          
	}
	if(m_bNeighbor)
	{
		if(m_dNeiHa < 0) {AfxMessageBox(_T("Error! Average Height of Nearby Structures")); return FALSE;}
		if(m_dNeiX < 0) {AfxMessageBox(_T("Error! Distance to the High-rise Structure")); return FALSE;}
		if(m_dNeiDl <= 0.) {AfxMessageBox(_T("Error! Larger Horizontal Dimension of Neighbouring High-rise Structure")); return FALSE;}
		if(m_dNeiHh <= 0.) {AfxMessageBox(_T("Error! Height of Neighbouring High-rise Structure")); return FALSE;}   
	}
	if(m_nTerrain == 4 || m_nTerrain>=10)
	{
		if(m_bDisplace)
		{
			if(m_dDisHa < 0.) {AfxMessageBox(_T("Error! Obstruction Height")); return FALSE;}
			if(m_dDisX  < 0.) {AfxMessageBox(_T("Error! Upwind Spacing")); return FALSE;}
		}
	}

	return TRUE;
} 


void CCMWindItemEuro2005Param::SetTerrainCategory()
{
	if(m_nTerrain>=10)
	{
		CString aItem[] = {_T("1"), _T("2")};
		int nIndex = 0;
		if(m_nTerrain==11) nIndex=1;
		m_editDisTC.SetWindowText(aItem[nIndex]);
		GetDlgItem(IDC_CMD_WIND_EURO05_TERRAIN_STC)->SetWindowText(_LS(IDS_CMD_Structure_Type));
	}
	else
	{
		CString aItem[] = {_T("0"), _T("I"), _T("II"), _T("III"), _T("IV")};  
		m_editDisTC.SetWindowText(aItem[m_nTerrain]);
		GetDlgItem(IDC_CMD_WIND_EURO05_TERRAIN_STC)->SetWindowText(_LS(IDS_CMD_Terrain_Category));
	}
}

void CCMWindItemEuro2005Param::InitOroTypeCobx()
{
	m_cobxOroType.ResetContent();

	CString aItem[] = { _LS(IDS_CMD_WIND_EURO2005_Cliffs_or_Escarpment), _LS(IDS_CMD_WIND_EURO2005_Hills_or_Ridges)};
	int nNum = sizeof(aItem)/sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		m_cobxOroType.AddString(aItem[i]);    
	}
	m_cobxOroType.SetCurSel(0);  
}

void CCMWindItemEuro2005Param::InitBldgLocCobx()
{
	m_cobxBldgLoc.ResetContent();
	
	CString aItem[] = {_LS(IDS_CMD_WIND_NBC_BLDG_LOCATION_UP), _LS(IDS_CMD_WIND_NBC_BLDG_LOCATION_DN)};
	int nNum = sizeof(aItem)/sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		m_cobxBldgLoc.AddString(aItem[i]);
	}
	m_cobxBldgLoc.SetCurSel(0);  
}


void CCMWindItemEuro2005Param::OnCmdOrographyCheck() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = m_chkOrography.GetCheck();
	m_cobxOroType.EnableWindow(bCheck);
	m_cobxBldgLoc.EnableWindow(bCheck);
	m_editOroH.EnableWindow(bCheck);
	m_editOroLu.EnableWindow(bCheck);
	m_editOroLd.EnableWindow(bCheck);
	m_editOroX.EnableWindow(bCheck);
	CtrlManager();
}

void CCMWindItemEuro2005Param::OnCmdNeighborCheck() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = m_chkNeighbor.GetCheck();
	m_editNeiHa.EnableWindow(bCheck);
	m_editNeiX.EnableWindow(bCheck);
	m_editNeiDl.EnableWindow(bCheck);  
	m_editNeiHh.EnableWindow(bCheck);  
}

void CCMWindItemEuro2005Param::OnCmdDisplaceCheck() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = m_chkDisplace.GetCheck();
	m_editDisHa.EnableWindow(bCheck);
	m_editDisX.EnableWindow(bCheck);
}

void CCMWindItemEuro2005Param::OnSelchangeCmdBldgLocCmb() 
{
	CtrlManager();
}

void CCMWindItemEuro2005Param::OnSelchangeCmdOroTypeCmb() 
{
	CtrlManager();
}

void CCMWindItemEuro2005Param::CtrlManager() 
{	
	BOOL bCheck = m_chkOrography.GetCheck();
	if(bCheck)
	{
		int nOroType = m_cobxOroType.GetCurSel();
		int nBldgLoc = m_cobxBldgLoc.GetCurSel();
		m_editOroLd.EnableWindow(nOroType == 1 && nBldgLoc == 1);
	}
}
