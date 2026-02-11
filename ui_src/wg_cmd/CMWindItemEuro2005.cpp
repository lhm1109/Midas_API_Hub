// CMWindItemEuro2005.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemEuro2005.h"
#include "CMWindItemDlg.h"
#include "CMWindItemEuro2005Param.h"
#include "CMWindItemEuro2005CsCd.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_LateralLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro2005 dialog


CCMWindItemEuro2005::CCMWindItemEuro2005(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemEuro2005::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemEuro2005)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Data.Initialize();
	m_DataBack.Initialize();
	m_bModify = FALSE;
	m_bInit = FALSE;
	m_bSingapore = FALSE;

	m_aCtrlCpeMethod.Add(IDC_CMD_WIND_EURO05_CE_RDO1);
	m_aCtrlCpeMethod.Add(IDC_CMD_WIND_EURO05_CE_RDO2);

	m_aCtrlLackMethod.Add(IDC_CMD_WIND_EURO05_LACK_RDO1);
	m_aCtrlLackMethod.Add(IDC_CMD_WIND_EURO05_LACK_RDO2);

	m_aCtrlCpeInput.Add(IDC_CMD_WIND_EURO05_CE10_EDIT);
	m_aCtrlCpeInput.Add(IDC_CMD_WIND_EURO05_CE01_EDIT);
	m_aCtrlCpeInput.Add(IDC_CMD_WIND_EURO05_CEL_EDIT);

	m_aCtrlCpeLack.Add(IDC_CMD_WIND_EURO05_CE_RDO1);
	m_aCtrlCpeLack.Add(IDC_CMD_WIND_EURO05_CE_RDO2);
	m_aCtrlCpeLack.Add(IDC_CMD_WIND_EURO05_CE10_EDIT);
	m_aCtrlCpeLack.Add(IDC_CMD_WIND_EURO05_CE01_EDIT);
	m_aCtrlCpeLack.Add(IDC_CMD_WIND_EURO05_CEL_EDIT);
	m_aCtrlCpeLack.Add(IDC_CMD_WIND_EURO05_LACK_RDO1);
	m_aCtrlCpeLack.Add(IDC_CMD_WIND_EURO05_LACK_RDO2);
	m_aCtrlCpeLack.Add(IDC_CMD_WIND_EURO05_LACK_EDIT);
}


void CCMWindItemEuro2005::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemEuro2005)
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_TERRAIN_CMB, m_cobxTerrain);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_CFR_EDIT, m_editCfr);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_VB_EDIT, m_editVb);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_CDIR_EDIT, m_editCdir);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_CSEASON_EDIT, m_editCseason);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_KL_EDIT, m_editKl);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_H_EDIT, m_editH);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_H_UNIT, m_unitH);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_CE10_EDIT, m_editCpe10);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_CE01_EDIT, m_editCpe01);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_CEL_EDIT, m_editCpeLee);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_LACK_EDIT, m_editLackFactor);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_CSCD_EDIT, m_editCsCd);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_FORCE_EDIT, m_editForceCoef);
		DDX_Control(pDX, IDC_CMD_WIND_EURO05_FORCE_CHK, m_chkForcCoef);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemEuro2005, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemEuro2005)
	ON_BN_CLICKED(IDC_CMD_WIND_EURO05_CSCD_BTN, OnCmdCscdBtn)
	ON_BN_CLICKED(IDC_CMD_WIND_EURO05_PARA_BTN, OnCmdParaBtn)  
	ON_BN_CLICKED(IDC_CMD_WIND_EURO05_CE_RDO1, OnCmdCpeMethodRadio)
	ON_BN_CLICKED(IDC_CMD_WIND_EURO05_LACK_RDO1, OnCmdLackMethodRadio)
	ON_BN_CLICKED(IDC_CMD_WIND_EURO05_FORCE_CHK, OnCmdForcCoefCheck)	
	ON_BN_CLICKED(IDC_CMD_WIND_EURO05_CE_RDO2, OnCmdCpeMethodRadio)	
	ON_BN_CLICKED(IDC_CMD_WIND_EURO05_LACK_RDO2, OnCmdLackMethodRadio)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro2005 message handlers

BOOL CCMWindItemEuro2005::DestroyWindow() 
{
	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpWindParam.SetFoldState(FALSE, FALSE);
		}
	}
	
	return CChildDialog::DestroyWindow();
}

BOOL CCMWindItemEuro2005::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitControls();

	m_grpWindParam.InitControl(this, CCMWindItemEuro2005::IDD, IDC_CMD_FRAME, TRUE, TRUE);
	m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	Data2Dlg();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindItemEuro2005::SetNationalAnnex(BOOL bSingapore)
{
	if(m_bSingapore==bSingapore) return;
	m_bSingapore = bSingapore;

	if(m_bSingapore)
	{
		m_Data.dVb = 20.0;
	}
	else
	{
		m_Data.dVb = 26.0;
	}

	InitCategoryCombo();
	m_editVb.SetEditUnit(m_Data.dVb);
}

void CCMWindItemEuro2005::SetData2Dlg(T_WIND_EURO2005& data, BOOL bModify, BOOL bSingapore)
{
	m_Data = data;
	m_DataBack = data;
	m_bModify = bModify;
	m_bSingapore = bSingapore;
}


BOOL CCMWindItemEuro2005::SetDlg2Data(T_WIND_EURO2005& data)
{
	BOOL bOK = Dlg2Data(&m_Data);
	data = m_Data;
	return bOK;
}


void CCMWindItemEuro2005::InitDefaultData()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	// 건물의 높이 자동계산
	CArray<T_STOR_K, T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nNumStor = aStorK.GetSize();
	T_STOR_D StorD;
	StorD.Initialize();
	if (nNumStor > 0) pDoc->m_pAttrCtrl->GetStor(aStorK[nNumStor-1], StorD);

	int nNumOfStory = aStorK.GetSize();
	CLateralLoad LateralLoad;
	double dBldgHeight = LateralLoad.GetBldgHeightAboveBaseLevel(); // 베이스에서 최상층까지의 높이
	m_Data.dH = dBldgHeight;	
	m_DataBack.dH = m_Data.dH;
	
	// Bx, By 자동계산
	double dBx = 0., dBy = 0.;
	LateralLoad.GetAreaAverageBldgWidthAboveBaseLevel(dBx, dBy);
	if (!m_bModify) 
	{
		m_Data.dCsB = dBy;
		m_Data.dCsD = dBx;
		m_DataBack.dCsB = m_Data.dCsB;
		m_DataBack.dCsD = m_Data.dCsD;

		m_Data.dZs = LateralLoad.CalcEuro2005Zs(&m_Data);
		m_DataBack.dZs = m_Data.dZs;
	}
}

void CCMWindItemEuro2005::InitControls()
{
	InitCategoryCombo();
	
	m_editH.SetReadOnly(TRUE);
	m_editH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitH.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMWindItemEuro2005::InitCategoryCombo()
{
	m_cobxTerrain.ResetContent();
	
	if(m_bSingapore)
	{
		CString aItem[] = {_T("1"), _T("2")};
		int nNum = sizeof(aItem) / sizeof(CString);
		for (int i=0; i<nNum; i++)
			m_cobxTerrain.AddString(aItem[i]);
		m_cobxTerrain.SetCurSel(1);

		GetDlgItem(IDC_CMD_WIND_EURO05_TERRAIN_STC)->SetWindowText(_LS(IDS_CMD_Structure_Type));
	}
	else
	{
		CString aItem[] = {_T("0"), _T("I"), _T("II"), _T("III"), _T("IV")};
		int nNum = sizeof(aItem) / sizeof(CString);
		for (int i=0; i<nNum; i++)
			m_cobxTerrain.AddString(aItem[i]);
		m_cobxTerrain.SetCurSel(2);

		GetDlgItem(IDC_CMD_WIND_EURO05_TERRAIN_STC)->SetWindowText(_LS(IDS_CMD_Terrain_Category));
	}
}

void CCMWindItemEuro2005::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCpeMethod, m_Data.nCpeMethod);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLackMethod, m_Data.nLackMethod);  
	if(m_bSingapore)
	{
		int nSel=0;
		if(m_Data.nTerrain==11) nSel=1;
		m_cobxTerrain.SetCurSel(nSel);
	}
	else
	{
		m_cobxTerrain.SetCurSel(m_Data.nTerrain);
	}
	m_editCfr.SetEditUnit(m_Data.dCfr);
	m_editVb.SetEditUnit(m_Data.dVb);
	m_editCdir.SetEditUnit(m_Data.dCdir);
	m_editCseason.SetEditUnit(m_Data.dCseason);
	m_editKl.SetEditUnit(m_Data.dKl);
	m_editH.SetEditUnit(m_Data.dH);
	m_editCpe10.SetEditUnit(m_Data.dCpe10);
	m_editCpe01.SetEditUnit(m_Data.dCpe01);
	m_editCpeLee.SetEditUnit(m_Data.dCpeLee);
	m_editLackFactor.SetEditUnit(m_Data.dLackFactor);
	m_editCsCd.SetEditUnit(m_Data.dCsCd);    
	m_editForceCoef.SetEditUnit(m_Data.dForceCoef);
	m_chkForcCoef.SetCheck(m_Data.bUseForceCoef);
	CtrlManager();
}

BOOL CCMWindItemEuro2005::Dlg2Data(T_WIND_EURO2005* pData)
{
	if (pData == 0) { ASSERT(0); return FALSE; }
	pData->Initialize();

	if(m_bSingapore)
	{
		int nSel=m_cobxTerrain.GetCurSel();
		if(nSel==0) pData->nTerrain=10;
		else pData->nTerrain=11;
	}
	else
	{
		pData->nTerrain      = m_cobxTerrain.GetCurSel();
	}
	
	pData->dCfr          = m_editCfr.GetEditValue();
	pData->dVb           = m_editVb.GetEditValue();
	pData->dCdir         = m_editCdir.GetEditValue();
	pData->dCseason      = m_editCseason.GetEditValue();
	pData->dKl           = m_editKl.GetEditValue();
	pData->dH            = m_editH.GetEditValue();  
	pData->dCsCd         = m_editCsCd.GetEditValue(); 
	pData->bUseForceCoef = m_chkForcCoef.GetCheck();
	if(pData->bUseForceCoef)
	{
		pData->dForceCoef = m_editForceCoef.GetEditValue();
	}
	else
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCpeMethod, pData->nCpeMethod);
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLackMethod, pData->nLackMethod);
		if(pData->nCpeMethod == 1)
		{
			pData->dCpe10 = m_editCpe10.GetEditValue();
			pData->dCpe01 = m_editCpe01.GetEditValue();
			pData->dCpeLee = m_editCpeLee.GetEditValue();  
		}
		if(pData->nLackMethod == 1)
		{
			pData->dLackFactor = m_editLackFactor.GetEditValue();
		}
	}
	if(m_DataBack.bOrography)
	{
		pData->bOrography = m_DataBack.bOrography;    
		pData->nOroType   = m_DataBack.nOroType  ;
		pData->nBldgLoc   = m_DataBack.nBldgLoc  ;    
		pData->dOroH      = m_DataBack.dOroH     ;     
		pData->dOroLu     = m_DataBack.dOroLu    ;    
		pData->dOroLd     = m_DataBack.dOroLd    ;    
		pData->dOroX      = m_DataBack.dOroX     ;     
	}
	if(m_DataBack.bNeighbor)
	{
		pData->bNeighbor  = m_DataBack.bNeighbor ;     
		pData->dNeiHa     = m_DataBack.dNeiHa    ;    
		pData->dNeiX      = m_DataBack.dNeiX     ;     
		pData->dNeiDl     = m_DataBack.dNeiDl    ;        
		pData->dNeiHh     = m_DataBack.dNeiHh    ; 
	}
	if(m_DataBack.bNeighbor)
	{  
		pData->bDisplace  = m_DataBack.bDisplace ; 
		pData->dDisHa     = m_DataBack.dDisHa    ;    
		pData->dDisX      = m_DataBack.dDisX     ; 
	}
	pData->dCsB  = m_DataBack.dCsB;
	pData->dCsD  = m_DataBack.dCsD;
	pData->dCsN1 = m_DataBack.dCsN1;
	pData->dCsDt = m_DataBack.dCsDt;
	pData->dZs   = m_DataBack.dZs;
	return TRUE;
}


void CCMWindItemEuro2005::OnCmdParaBtn()
{
	CCMWindItemEuro2005Param dlg;

	if(m_bSingapore)
	{
		int nSel=m_cobxTerrain.GetCurSel();
		if(nSel==0) dlg.m_nTerrain=10;
		else dlg.m_nTerrain=11;
	}
	else
	{
		dlg.m_nTerrain      = m_cobxTerrain.GetCurSel();

		if(m_cobxTerrain.GetCurSel() != 4)   // Terrain Category IV인 경우만 Displacement height 고려 가능
		{
			m_DataBack.bDisplace = FALSE; 
		}
	}

	m_DataBack.dH = m_editH.GetEditValue();

	dlg.m_bOrography = m_DataBack.bOrography;
	dlg.m_bNeighbor  = m_DataBack.bNeighbor ; 
	dlg.m_bDisplace  = m_DataBack.bDisplace ; 
	dlg.m_nOroType   = m_DataBack.nOroType  ;
	dlg.m_nBldgLoc   = m_DataBack.nBldgLoc  ;    
	dlg.m_dOroH      = m_DataBack.dOroH     ;     
	dlg.m_dOroLu     = m_DataBack.dOroLu    ;    
	dlg.m_dOroLd     = m_DataBack.dOroLd    ;    
	dlg.m_dOroX      = m_DataBack.dOroX     ;     
	dlg.m_dNeiH      = m_DataBack.dH        ;     
	dlg.m_dNeiHa     = m_DataBack.dNeiHa    ;    
	dlg.m_dNeiX      = m_DataBack.dNeiX     ;     
	dlg.m_dNeiDl     = m_DataBack.dNeiDl    ;      
	dlg.m_dNeiHh     = m_DataBack.dNeiHh    ;    
	dlg.m_dDisHa     = m_DataBack.dDisHa    ;    
	dlg.m_dDisX      = m_DataBack.dDisX     ;     

	if (dlg.DoModal() == IDOK)
	{
		m_DataBack.bOrography = dlg.m_bOrography;
		m_DataBack.bNeighbor  = dlg.m_bNeighbor ; 
		m_DataBack.bDisplace  = dlg.m_bDisplace ; 
		m_DataBack.nOroType   = dlg.m_nOroType  ;
		m_DataBack.nBldgLoc   = dlg.m_nBldgLoc  ;  
		m_DataBack.dOroH      = dlg.m_dOroH     ;     
		m_DataBack.dOroLu     = dlg.m_dOroLu    ;    
		m_DataBack.dOroLd     = dlg.m_dOroLd    ;    
		m_DataBack.dOroX      = dlg.m_dOroX     ;         
		m_DataBack.dNeiHa     = dlg.m_dNeiHa    ;    
		m_DataBack.dNeiX      = dlg.m_dNeiX     ;     
		m_DataBack.dNeiDl     = dlg.m_dNeiDl    ;        
		m_DataBack.dNeiHh     = dlg.m_dNeiHh    ;    
		m_DataBack.dDisHa     = dlg.m_dDisHa    ;    
		m_DataBack.dDisX      = dlg.m_dDisX     ;    
	}
}

void CCMWindItemEuro2005::OnCmdCscdBtn() 
{
	// TODO: Add your control notification handler code here
	T_WIND_EURO2005 data;
	data.Initialize();
	if (!Dlg2Data(&data)) ASSERT(0);
	if(m_cobxTerrain.GetCurSel() != 4 && m_bSingapore==FALSE)   // Terrain Category IV인 경우만 Displacement height 고려 가능
	{
		data.bDisplace = FALSE; 
	}
	else
	{
		data.bDisplace  = m_DataBack.bDisplace;
	}
	data.bOrography = m_DataBack.bOrography;
	data.bNeighbor  = m_DataBack.bNeighbor;  
	data.dCsB       = m_DataBack.dCsB;
	data.dCsD       = m_DataBack.dCsD;
	data.dCsN1      = m_DataBack.dCsN1;
	data.dCsDt      = m_DataBack.dCsDt;
	data.dCsCd      = m_DataBack.dCsCd;
	data.dZs		= m_DataBack.dZs;

	CCMWindItemEuro2005CsCd dlg(&data);
	if (dlg.DoModal() == IDOK)
	{
		m_DataBack.dCsB  = data.dCsB;
		m_DataBack.dCsD  = data.dCsD;
		m_DataBack.dCsN1 = data.dCsN1;
		m_DataBack.dCsDt = data.dCsDt;   
		m_DataBack.dCsCd = data.dCsCd;
		m_DataBack.dZs   = data.dZs;
		m_editCsCd.SetEditUnit(data.dCsCd);    
	}
}

void CCMWindItemEuro2005::OnCmdCpeMethodRadio()
{
	CtrlManager();
}

void CCMWindItemEuro2005::OnCmdLackMethodRadio()
{
	CtrlManager();
}

void CCMWindItemEuro2005::OnCmdForcCoefCheck()
{
	CtrlManager();
}

void CCMWindItemEuro2005::CtrlManager()
{
	int nCpeMethod, nLackMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCpeMethod, nCpeMethod);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLackMethod, nLackMethod);  
	BOOL bForceCheck = m_chkForcCoef.GetCheck();    

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCpeMethod, bForceCheck == FALSE);   
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlLackMethod, bForceCheck == FALSE);   
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCpeInput, nCpeMethod == 1 && bForceCheck == FALSE); 
	m_editLackFactor.EnableWindow(nLackMethod == 1 && bForceCheck == FALSE);  
	m_editForceCoef.EnableWindow(bForceCheck == TRUE);  
}

LRESULT CCMWindItemEuro2005::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}
