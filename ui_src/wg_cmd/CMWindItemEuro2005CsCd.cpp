// CMWindItemEuro2005CsCd.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemEuro2005CsCd.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro2005CsCd dialog


CCMWindItemEuro2005CsCd::CCMWindItemEuro2005CsCd(T_WIND_EURO2005* pData, CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMWindItemEuro2005CsCd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMWindItemEuro2005CsCd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pData = 0;
	ASSERT(pData);
	m_pData = pData;
}


void CCMWindItemEuro2005CsCd::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemEuro2005CsCd)
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_TC_EDIT, m_editTC);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_VB_EDIT, m_editVb);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_KL_EDIT, m_editKl);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_H_EDIT, m_editH);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_ORO_EDIT, m_editOro);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_NEI_EDIT, m_editNei);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_DIS_EDIT, m_editDis);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_B_EDIT, m_editB);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_D_EDIT, m_editD);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_N1_EDIT, m_editN1);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_DT_EDIT, m_editDt);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_ZS_EDIT, m_editZs);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_VM_EDIT, m_editVm);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_L_EDIT, m_editL);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_LV_EDIT, m_editLv);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_FL_EDIT, m_editFl);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_SL_EDIT, m_editSl);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_RH_EDIT, m_editRh);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_RB_EDIT, m_editRb);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_EH_EDIT, m_editEh);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_EB_EDIT, m_editEb);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_BB_EDIT, m_editBB);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_RR_EDIT, m_editRR);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_V_EDIT, m_editV);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_KP_EDIT, m_editKp);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_CD_EDIT, m_editCd);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_H_UNIT, m_unitH);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_B_UNIT, m_unitB);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_D_UNIT, m_unitD);
	DDX_Control(pDX, IDC_CMD_WIND_EURO05_CS_ZS_UNIT, m_unitZs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemEuro2005CsCd, CDialogMove)
	//{{AFX_MSG_MAP(CCMWindItemEuro2005CsCd)
	ON_BN_CLICKED(IDC_CMD_WIND_EURO05_CALC_BTN, OnCmdCalcBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro2005CsCd message handlers

BOOL CCMWindItemEuro2005CsCd::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitControls();

	Data2Dlg();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMWindItemEuro2005CsCd::OnOK() 
{
	// TODO: Add extra validation here

	if (!Dlg2Data()) return;
	
	CDialogMove::OnOK();
}

void CCMWindItemEuro2005CsCd::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}


void CCMWindItemEuro2005CsCd::InitControls()
{  
	m_editH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editB.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editD.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editZs.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_unitH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitB.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitD.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitZs.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMWindItemEuro2005CsCd::Data2Dlg()
{
	CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
	ASSERT(pUnitCtrl);
	
	SetTerrainCategory();
	SetEffectsData();
	
	double dVb = m_pData->dVb * m_pData->dCdir * m_pData->dCseason;
	m_editVb.SetEditUnit(dVb);
	m_editKl.SetEditUnit(m_pData->dKl);
	m_editH.SetEditUnit(m_pData->dH);
	m_editB.SetEditUnit(m_pData->dCsB);
	m_editD.SetEditUnit(m_pData->dCsD);
	m_editN1.SetEditUnit(m_pData->dCsN1);
	m_editDt.SetEditUnit(m_pData->dCsDt);
	m_editCd.SetEditUnit(m_pData->dCsCd);  
	m_editZs.SetEditUnit(m_pData->dZs);
}

BOOL CCMWindItemEuro2005CsCd::Dlg2Data()
{
	m_pData->dCsB  = m_editB.GetEditValue();   
	m_pData->dCsD  = m_editD.GetEditValue(); 
	m_pData->dCsN1 = m_editN1.GetEditValue(); 
	m_pData->dCsDt = m_editDt.GetEditValue();   
	m_pData->dCsCd = m_editCd.GetEditValue(); 
	m_pData->dZs   = m_editZs.GetEditValue();
	return TRUE;
} 

void CCMWindItemEuro2005CsCd::SetTerrainCategory()
{
	if(m_pData->nTerrain>=10)
	{
		CString aItem[] = {_T("1"), _T("2")};
		int nIndex = 0;
		if(m_pData->nTerrain==11) nIndex=1;
		m_editTC.SetWindowText(aItem[nIndex]);
		GetDlgItem(IDC_CMD_WIND_EURO05_TERRAIN_STC)->SetWindowText(_LS(IDS_CMD_Structure_Type));
	}
	else
	{
		CString aItem[] = { _T("0"), _T("I"), _T("II"), _T("III"), _T("IV")};  
		m_editTC.SetWindowText(aItem[m_pData->nTerrain]);
		GetDlgItem(IDC_CMD_WIND_EURO05_TERRAIN_STC)->SetWindowText(_LS(IDS_CMD_Terrain_Category));
	} 
}

void CCMWindItemEuro2005CsCd::SetEffectsData()
{
	CString aItem[] = {_LS(IDS_WG_CMD__ADDD__Do_not_consider), _LS(IDS_WG_CMD__ADDD__Consider)};  
	int ix = m_pData->bOrography ? 1 : 0;
	m_editOro.SetWindowText(aItem[ix]);  
	ix = m_pData->bNeighbor ? 1 : 0;
	m_editNei.SetWindowText(aItem[ix]);  
	ix = m_pData->bDisplace ? 1 : 0;
	m_editDis.SetWindowText(aItem[ix]);  
}

void CCMWindItemEuro2005CsCd::OnCmdCalcBtn()
{
	if(!Dlg2Data()) return;

	if(m_pData->dCsB  <= 0.) {AfxMessageBox(_T("Error! b")); return;}
	if(m_pData->dCsD  <= 0.) {AfxMessageBox(_T("Error! d")); return;}
	if(m_pData->dCsN1 <= 0.) {AfxMessageBox(_T("Error! n1")); return;}
	if(m_pData->dCsDt <= 0.) {AfxMessageBox(_T("Error! Delta")); return;}

	CLateralLoad LateralLoad;
	stEuro05CsCdFactor stVal;
	T_WIND_EURO2005* pData; 
	pData = m_pData;
//  stVal.Initialize();

	if(!LateralLoad.CalcEuro2005StructuralFactor(pData, stVal)) {AfxMessageBox(_T("Error in Calculation !")); return;}

	//m_editZs.SetEditUnit(stVal.dZs);
	m_editVm.SetEditUnit(stVal.dVm);
	m_editL.SetEditUnit(stVal.dL);
	m_editLv.SetEditUnit(stVal.dLv);
	m_editFl.SetEditUnit(stVal.dFL);
	m_editSl.SetEditUnit(stVal.dSL);
	m_editRh.SetEditUnit(stVal.dRh);
	m_editRb.SetEditUnit(stVal.dRb);
	m_editEh.SetEditUnit(stVal.dEtah);
	m_editEb.SetEditUnit(stVal.dEtab);
	m_editBB.SetEditUnit(stVal.dB2);
	m_editRR.SetEditUnit(stVal.dR2);
	m_editV.SetEditUnit(stVal.dv);
	m_editKp.SetEditUnit(stVal.dKp);
	m_editCd.SetEditUnit(stVal.dCsCd);
}
