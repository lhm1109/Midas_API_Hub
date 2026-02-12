// NLLinkPrtItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "NLLinkPrtItemDlg.h"
#include "NLLinkPrtViscDlg.h"
#include "NLLinkPrtGapDlg.h"
#include "NLLinkPrtHookDlg.h"
#include "NLLinkPrtHystDlg.h"
#include "NLLinkPrtLeadDlg.h"
#include "NLLinkPrtFricDlg.h"
#include "NLLinkPrtTfricDlg.h"
#include "NLLinkPrtDfricDlg.h"
#include "NLLinkPrtItemCoupledDlg.h"
#include "IehpListDlg.h"
#include <math.h>
#include "SdvpListDlg.h"
#include "AntivibrationSupportGBListDlg.h"
#include "DamperGBListDlg.h"

#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_dbLock\LockOption.h"
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtItemDlg dialog

		
CNLLinkPrtItemDlg::CNLLinkPrtItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CNLLinkPrtItemDlg::IDD, pParent)
{
	m_nType = 0;
	m_bModify = FALSE;
	for(int i = 0; i < 6; i++)
	{
		m_bLChk[i] = FALSE;
		m_bNChk[i] = FALSE;
		m_DofGroup[i].RemoveAll();    // 각 DOF에 대한 해당 Item(3개)
	}
	//{{AFX_DATA_INIT(CNLLinkPrtItemDlg)
//	m_bShearSprLoc = FALSE;
	//}}AFX_DATA_INIT

	m_DofGroup[0].Add(IDC_CMD_NLLK_DX_STIFF_EDIT);
	m_DofGroup[0].Add(IDC_CMD_NLLK_DX_STIFF_EDIT2);
	m_DofGroup[0].Add(IDC_CMD_NLLK_N_DX_CHK);
	m_DofGroup[0].Add(IDC_CMD_NLLK_DX_BTN);
	
	m_DofGroup[1].Add(IDC_CMD_NLLK_DY_STIFF_EDIT);
	m_DofGroup[1].Add(IDC_CMD_NLLK_DY_STIFF_EDIT2);
	m_DofGroup[1].Add(IDC_CMD_NLLK_N_DY_CHK);
	m_DofGroup[1].Add(IDC_CMD_NLLK_DY_BTN);
	m_DofGroup[1].Add(IDC_CMD_NLLK_SPR_LOCATION_DY_EDIT);

	m_DofGroup[2].Add(IDC_CMD_NLLK_DZ_STIFF_EDIT);
	m_DofGroup[2].Add(IDC_CMD_NLLK_DZ_STIFF_EDIT2);
	m_DofGroup[2].Add(IDC_CMD_NLLK_N_DZ_CHK);
	m_DofGroup[2].Add(IDC_CMD_NLLK_DZ_BTN);
	m_DofGroup[2].Add(IDC_CMD_NLLK_SPR_LOCATION_DZ_EDIT);

	m_DofGroup[3].Add(IDC_CMD_NLLK_RX_STIFF_EDIT);
	m_DofGroup[3].Add(IDC_CMD_NLLK_RX_STIFF_EDIT2);
	m_DofGroup[3].Add(IDC_CMD_NLLK_N_RX_CHK);
	m_DofGroup[3].Add(IDC_CMD_NLLK_RX_BTN);
	
	m_DofGroup[4].Add(IDC_CMD_NLLK_RY_STIFF_EDIT);
	m_DofGroup[4].Add(IDC_CMD_NLLK_RY_STIFF_EDIT2);
	m_DofGroup[4].Add(IDC_CMD_NLLK_N_RY_CHK);
	m_DofGroup[4].Add(IDC_CMD_NLLK_RY_BTN);
	
	m_DofGroup[5].Add(IDC_CMD_NLLK_RZ_STIFF_EDIT);
	m_DofGroup[5].Add(IDC_CMD_NLLK_RZ_STIFF_EDIT2);
	m_DofGroup[5].Add(IDC_CMD_NLLK_N_RZ_CHK);
	m_DofGroup[5].Add(IDC_CMD_NLLK_RZ_BTN);
	
	m_pDoc = CDBDoc::GetDocPoint();
	m_nAppType = 0;

	m_aCtrl_EnableOnSeis.Add(IDC_CMD_IEHP_SCD_TYPE_CMB);
	m_aCtrl_EnableOnSeis.Add(IDC_CMD_IEHP_SCD_PROP_CMB);
	m_aCtrl_EnableOnSeis.Add(IDC_CMD_IEHP_SCD_PROP_BTN);
	
	m_aCtrl_DisableOnSeis.Add(IDC_CMD_PROPERTY_TYPE);
	m_aCtrl_DisableOnSeis.Add(IDC_CMD_COUPLED_BTN);
	
	m_aCtrl_ShowOnSeis.Add(IDC_CMD_IEHP_REMARK_GRP);
	m_aCtrl_ShowOnSeis.Add(IDC_CMD_IEHP_REMARK_DX_EDT);
	m_aCtrl_ShowOnSeis.Add(IDC_CMD_IEHP_REMARK_DY_EDT);
	m_aCtrl_ShowOnSeis.Add(IDC_CMD_IEHP_REMARK_DZ_EDT);
	m_aCtrl_ShowOnSeis.Add(IDC_CMD_IEHP_REMARK_RX_EDT);
	m_aCtrl_ShowOnSeis.Add(IDC_CMD_IEHP_REMARK_RY_EDT);
	m_aCtrl_ShowOnSeis.Add(IDC_CMD_IEHP_REMARK_RZ_EDT);
	
	m_aCtrl_HideOnSeis.Add(IDC_WG_CMD_STATIC10);
	m_aCtrl_HideOnSeis.Add(IDC_WG_CMD_STATIC11);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_N_DX_CHK);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_DX_BTN);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_N_DY_CHK);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_DY_BTN);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_N_DZ_CHK);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_DZ_BTN);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_N_RX_CHK);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_RX_BTN);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_N_RY_CHK);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_RY_BTN);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_N_RZ_CHK);
	m_aCtrl_HideOnSeis.Add(IDC_CMD_NLLK_RZ_BTN);

	m_bOnInit=TRUE;
}


void CNLLinkPrtItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLLinkPrtItemDlg)
	DDX_Control(pDX, IDC_CMD_NLLK_MASS_CHK, m_wndMassChk);
	DDX_Control(pDX, IDC_CMD_NLLK_MASS_UNIT, m_wndMassUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_MASS_EDIT, m_wndMassEdit);
	DDX_Control(pDX, IDC_CMD_NLLK_MASS_RATIO_I_EDIT, m_wndMassRatioEdit_i);
	DDX_Control(pDX, IDC_CMD_NLLK_MASS_RATIO_J_EDIT, m_wndMassRatioEdit_j);
	DDX_Control(pDX, IDC_CMD_NLLK_SHEAR_CHK, m_chkShearSpr);
	DDX_Control(pDX, IDC_CMD_APPLY, m_wndBtnApply);
	DDX_Control(pDX, IDC_CMD_NLLK_SPR_LOCATION_DZ_EDIT, m_wndDzSprLoc);
	DDX_Control(pDX, IDC_CMD_NLLK_SPR_LOCATION_DY_EDIT, m_wndDySprLoc);
	DDX_Control(pDX, IDC_CMD_NLLK_WEIGHT_UNIT, m_wndWeightUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_WEIGHT_EDIT, m_wndWeightEdit);
	DDX_Control(pDX, IDC_CMD_NLLK_WEIGHT_RATIO_I_EDIT, m_wndWeightRatioEdit_i);
	DDX_Control(pDX, IDC_CMD_NLLK_WEIGHT_RATIO_J_EDIT, m_wndWeightRatioEdit_j);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_wndDescription);
	DDX_Control(pDX, IDC_CMD_PROPERTY_TYPE, m_wndPropType);
	DDX_Control(pDX, IDC_CMD_PROPERTY_NAME, m_wndName);	
	DDX_Radio(pDX, IDC_CMD_NLLK_TYPE_RDO1, m_nAppType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CMD_NLLK_DX_BTN, m_wndNBtn[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_DY_BTN, m_wndNBtn[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_DZ_BTN, m_wndNBtn[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_RX_BTN, m_wndNBtn[3]);
	DDX_Control(pDX, IDC_CMD_NLLK_RY_BTN, m_wndNBtn[4]);
	DDX_Control(pDX, IDC_CMD_NLLK_RZ_BTN, m_wndNBtn[5]);
	DDX_Control(pDX, IDC_CMD_NLLK_N_DX_CHK, m_wndNChk[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_N_DY_CHK, m_wndNChk[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_N_DZ_CHK, m_wndNChk[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_N_RX_CHK, m_wndNChk[3]);
	DDX_Control(pDX, IDC_CMD_NLLK_N_RY_CHK, m_wndNChk[4]);
	DDX_Control(pDX, IDC_CMD_NLLK_N_RZ_CHK, m_wndNChk[5]);
	DDX_Control(pDX, IDC_CMD_NLLK_L_DX_CHK, m_wndLChk[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_L_DY_CHK, m_wndLChk[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_L_DZ_CHK, m_wndLChk[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_L_RX_CHK, m_wndLChk[3]);
	DDX_Control(pDX, IDC_CMD_NLLK_L_RY_CHK, m_wndLChk[4]);
	DDX_Control(pDX, IDC_CMD_NLLK_L_RZ_CHK, m_wndLChk[5]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_RZ_UNIT, m_wndStifUnit[5]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_RY_UNIT, m_wndStifUnit[4]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_RX_UNIT, m_wndStifUnit[3]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_DZ_UNIT, m_wndStifUnit[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_DY_UNIT, m_wndStifUnit[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_DX_UNIT, m_wndStifUnit[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_RZ_STIFF_EDIT, m_wndStifEdit[5]);
	DDX_Control(pDX, IDC_CMD_NLLK_RY_STIFF_EDIT, m_wndStifEdit[4]);
	DDX_Control(pDX, IDC_CMD_NLLK_RX_STIFF_EDIT, m_wndStifEdit[3]);  
	DDX_Control(pDX, IDC_CMD_NLLK_DZ_STIFF_EDIT, m_wndStifEdit[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_DY_STIFF_EDIT, m_wndStifEdit[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_DX_STIFF_EDIT, m_wndStifEdit[0]);  
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_RZ_UNIT2, m_wndStifUnit2[5]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_RY_UNIT2, m_wndStifUnit2[4]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_RX_UNIT2, m_wndStifUnit2[3]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_DZ_UNIT2, m_wndStifUnit2[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_DY_UNIT2, m_wndStifUnit2[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFF_DX_UNIT2, m_wndStifUnit2[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_RZ_STIFF_EDIT2, m_wndStifEdit2[5]);
	DDX_Control(pDX, IDC_CMD_NLLK_RY_STIFF_EDIT2, m_wndStifEdit2[4]);
	DDX_Control(pDX, IDC_CMD_NLLK_RX_STIFF_EDIT2, m_wndStifEdit2[3]);  
	DDX_Control(pDX, IDC_CMD_NLLK_DZ_STIFF_EDIT2, m_wndStifEdit2[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_DY_STIFF_EDIT2, m_wndStifEdit2[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_DX_STIFF_EDIT2, m_wndStifEdit2[0]);  
	DDX_Check(pDX, IDC_CMD_NLLK_L_DX_CHK, m_bLChk[0]);
	DDX_Check(pDX, IDC_CMD_NLLK_L_DY_CHK, m_bLChk[1]);
	DDX_Check(pDX, IDC_CMD_NLLK_L_DZ_CHK, m_bLChk[2]);
	DDX_Check(pDX, IDC_CMD_NLLK_L_RX_CHK, m_bLChk[3]);
	DDX_Check(pDX, IDC_CMD_NLLK_L_RY_CHK, m_bLChk[4]);
	DDX_Check(pDX, IDC_CMD_NLLK_L_RZ_CHK, m_bLChk[5]);
	DDX_Check(pDX, IDC_CMD_NLLK_N_DX_CHK, m_bNChk[0]);
	DDX_Check(pDX, IDC_CMD_NLLK_N_DY_CHK, m_bNChk[1]);
	DDX_Check(pDX, IDC_CMD_NLLK_N_DZ_CHK, m_bNChk[2]);
	DDX_Check(pDX, IDC_CMD_NLLK_N_RX_CHK, m_bNChk[3]);
	DDX_Check(pDX, IDC_CMD_NLLK_N_RY_CHK, m_bNChk[4]);
	DDX_Check(pDX, IDC_CMD_NLLK_N_RZ_CHK, m_bNChk[5]);
	DDX_Control(pDX, IDC_CMD_COUPLED_BTN, m_wndCoupledBtn);
	DDX_Control(pDX, IDC_CMD_IEHP_SCD_TYPE_CMB, m_wndSeisDvType);
	DDX_Control(pDX, IDC_CMD_IEHP_SCD_PROP_CMB, m_wndSeisDvProp);
	DDX_Control(pDX, IDC_CMD_IEHP_REMARK_DX_EDT, m_wndRemarkEdt[0]);
	DDX_Control(pDX, IDC_CMD_IEHP_REMARK_DY_EDT, m_wndRemarkEdt[1]);
	DDX_Control(pDX, IDC_CMD_IEHP_REMARK_DZ_EDT, m_wndRemarkEdt[2]);
	DDX_Control(pDX, IDC_CMD_IEHP_REMARK_RX_EDT, m_wndRemarkEdt[3]);
	DDX_Control(pDX, IDC_CMD_IEHP_REMARK_RY_EDT, m_wndRemarkEdt[4]);
	DDX_Control(pDX, IDC_CMD_IEHP_REMARK_RZ_EDT, m_wndRemarkEdt[5]);
}

BEGIN_MESSAGE_MAP(CNLLinkPrtItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNLLinkPrtItemDlg)
	ON_BN_CLICKED(IDC_CMD_NLLK_DX_BTN, OnCmdNonLinDxBtn)
	ON_BN_CLICKED(IDC_CMD_NLLK_DY_BTN, OnCmdNonLinDyBtn)
	ON_BN_CLICKED(IDC_CMD_NLLK_DZ_BTN, OnCmdNonLinDzBtn)
	ON_BN_CLICKED(IDC_CMD_NLLK_RX_BTN, OnCmdNonLinRxBtn)
	ON_BN_CLICKED(IDC_CMD_NLLK_RY_BTN, OnCmdNonLinRyBtn)
	ON_BN_CLICKED(IDC_CMD_NLLK_RZ_BTN, OnCmdNonLinRzBtn)
	ON_BN_CLICKED(IDC_CMD_NLLK_L_DX_CHK, OnCmdNllkLChk)
 	ON_BN_CLICKED(IDC_CMD_NLLK_N_DX_CHK, OnCmdNllkNDxChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_N_DY_CHK, OnCmdNllkNDyChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_N_DZ_CHK, OnCmdNllkNDzChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_N_RX_CHK, OnCmdNllkNRxChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_N_RY_CHK, OnCmdNllkNRyChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_N_RZ_CHK, OnCmdNllkNRzChk) 
	ON_CBN_SELCHANGE(IDC_CMD_PROPERTY_TYPE, OnChangePropType)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_NLLK_SHEAR_CHK, OnCmdNllkShearChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_L_DY_CHK, OnCmdNllkLChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_L_DZ_CHK, OnCmdNllkLChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_L_RX_CHK, OnCmdNllkLChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_L_RY_CHK, OnCmdNllkLChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_L_RZ_CHK, OnCmdNllkLChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_MASS_CHK, OnCmdNllkMassChk)
	ON_BN_CLICKED(IDC_CMD_NLLK_TYPE_RDO1, OnCmdNookTypeRdo)
	ON_BN_CLICKED(IDC_CMD_NLLK_TYPE_RDO2, OnCmdNookTypeRdo)
	ON_BN_CLICKED(IDC_CMD_NLLK_TYPE_RDO3, OnCmdNookTypeRdo)
	ON_BN_CLICKED(IDC_CMD_IEHP_BTN, OnCmdIehpBtn)
	ON_EN_CHANGE(IDC_CMD_NLLK_MASS_RATIO_I_EDIT, OnChangeMassRatioIEdit)
	ON_EN_CHANGE(IDC_CMD_NLLK_WEIGHT_RATIO_I_EDIT, OnChangeWeightRatioIEdit)
	ON_BN_CLICKED(IDC_CMD_COUPLED_BTN, OnCmdCoupledBtn)
	ON_EN_CHANGE(IDC_CMD_NLLK_DY_STIFF_EDIT, OnCmdNllkDyEdt)
	ON_EN_CHANGE(IDC_CMD_NLLK_DY_STIFF_EDIT2, OnCmdNllkDyEdt2)
	ON_CBN_SELCHANGE(IDC_CMD_IEHP_SCD_TYPE_CMB, OnSelChangeSeisTypeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_IEHP_SCD_PROP_CMB, OnSelChangeSeisPropCmb)
	ON_BN_CLICKED(IDC_CMD_IEHP_SCD_PROP_BTN, OnClickSeisPropBtn)
	ON_BN_CLICKED(IDC_CMD_NLLK_DESC_BTN, OnCmdDescBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtItemDlg message handlers

BOOL CNLLinkPrtItemDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	if (!m_bModify)
	{
		m_Data.Initialize();
		for(int i = 0; i < 6; i++)   //	수정모드가 아닌 경우 Child Dialog에 있는 변수 초기화
		{      
			m_VD[i].Initialize();
			m_GAP[i].Initialize();
			m_HOOK[i].Initialize();
			m_HS[i].Initialize();
			m_LRBI[i].Initialize();
			m_FPSI[i].Initialize();
			m_TFPSI[i].Initialize();
			m_DFPSI[i].Initialize();
			      
			m_pDlgUtil->CtrlEnableDisable(this, m_DofGroup[i], FALSE);  // 각 Dof 그룹의 Item 초기화 
		}
		
		memset(m_dStif, 0.0, sizeof(m_dStif));
		memset(m_dDamp, 0.0, sizeof(m_dStif));
	}
	else  // 수정모드인 경우 기존 값을 현재 Dialog의 Local 변수에 넣는다.	
	{
		m_wndBtnApply.EnableWindow(FALSE);   // 수정모드인 경우 Apply 버튼을 비활성화한다.
		m_nType = m_Data.nType;              // Child Dlg에 넘겨줄 변수 설정
		for(int i = 0; i < 6; i++)
		{
			if(m_nType == 0)       m_VD[i]	  = m_Data.NDP_Prop[i].VD; 
			else m_VD[i].Initialize();
			if(m_nType == 1)       m_GAP[i]   = m_Data.NDP_Prop[i].GAP;   
			else m_GAP[i].Initialize();
			if(m_nType == 2)       m_HOOK[i]  = m_Data.NDP_Prop[i].HOOK;  
			else m_HOOK[i].Initialize();
			if(m_nType == 3)       m_HS[i]	  = m_Data.NDP_Prop[i].HS;    
			else m_HS[i].Initialize();
			if(m_nType == 4)       m_LRBI[i]  = m_Data.NDP_Prop[i].LRBI;  
			else m_LRBI[i].Initialize();
			if(m_nType == 5)       m_FPSI[i]  = m_Data.NDP_Prop[i].FPSI; 
			else m_FPSI[i].Initialize();
			if(m_nType == 6)       m_TFPSI[i] = m_Data.NDP_Prop[i].TFPSI; 
			else m_TFPSI[i].Initialize();
			if(m_nType == 7)       m_DFPSI[i] = m_Data.NDP_Prop[i].DFPSI; 
			else m_DFPSI[i].Initialize();

			m_dStif[i] = m_Data.LDP_dEffStiff[i];
			m_dDamp[i] = m_Data.LDP_dEffDamping[i];
		}
		if(m_nType == 4 || m_nType == 5 || m_nType == 6 || m_nType == 7)
		{
			m_GAP[0] = m_Data.NDP_Prop[0].GAP;
			m_GAP[3] = m_Data.NDP_Prop[3].GAP;   
			m_GAP[4] = m_Data.NDP_Prop[4].GAP;   
			m_GAP[5] = m_Data.NDP_Prop[5].GAP;   
		}

		if( m_Data.nICoupld == 1 )
		{
			for(int i=0; i<15; i++ )
			{
				m_dStif[i+6] = m_Data.LDP_dEffStif_Coupled[i];
				m_dDamp[i+6] = m_Data.LDP_dEffDamp_Coupled[i];
			}
		}
		else
		{			
			memset(m_dStif, 0.0, sizeof(m_dStif));
			memset(m_dDamp, 0.0, sizeof(m_dStif));
		}
	}                                // Lead Rubber와 Friction Pendulum의 Dx인 경우 GAP Type을 사용 
		 
	CDBDoc* pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;	

	CString strMsg;
	if(!m_pDoc->IsSeisCtrlDeviceEnable(strMsg))
	{
		if(m_Data.nApplicationType == 2) m_Data.nApplicationType = 0;
		GetDlgItem(IDC_CMD_NLLK_TYPE_RDO3)->EnableWindow(FALSE);
	}
	
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // M 기준
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);   
	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);

	// 단위변환
	m_wndMassUnit.SetUnitType(D_UNITSYS_BASE_MASS);	 
	m_wndWeightUnit.SetUnitType(D_UNITSYS_BASE_WEIGHT);
	m_wndStifUnit[0].SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndStifUnit[1].SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndStifUnit[2].SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndStifUnit[3].SetUnitType(D_UNITSYS_BASE_ROSPRING);
	m_wndStifUnit[4].SetUnitType(D_UNITSYS_BASE_ROSPRING);
	m_wndStifUnit[5].SetUnitType(D_UNITSYS_BASE_ROSPRING);
	m_wndStifUnit2[0].SetUnitType(D_UNITSYS_BASE_FSEC_L);
	m_wndStifUnit2[1].SetUnitType(D_UNITSYS_BASE_FSEC_L);
	m_wndStifUnit2[2].SetUnitType(D_UNITSYS_BASE_FSEC_L);
	m_wndStifUnit2[3].SetUnitType(D_UNITSYS_BASE_FLSEC_R);
	m_wndStifUnit2[4].SetUnitType(D_UNITSYS_BASE_FLSEC_R);
	m_wndStifUnit2[5].SetUnitType(D_UNITSYS_BASE_FLSEC_R);

	m_wndMassRatioEdit_i.SetUnitType(D_UNITSYS_NONE);
	m_wndMassRatioEdit_j.SetUnitType(D_UNITSYS_NONE);
	m_wndWeightRatioEdit_i.SetUnitType(D_UNITSYS_NONE);
	m_wndWeightRatioEdit_j.SetUnitType(D_UNITSYS_NONE);
	
	//MakeCombo(m_Data.nApplicationType);
	
	Data2Dlg();

	if (m_Data.nApplicationType == 0)
	{
		for(int i = 0; i < 3; i++)
			m_wndStifEdit2[i].SetEditUnit(m_Data.LDP_dEffDamping[i]);     // effective stiffness의 초기값 출력
		m_wndPropType.SetCurSel(m_Data.nElemType);
	}
	else
	{
		for(int i = 0; i < 6; i++)
			m_wndStifEdit[i].SetEditUnit(m_Data.LDP_dEffStiff[i]);     // effective stiffness의 초기값 출력
		m_wndPropType.SetCurSel(m_Data.nType);
	}

	if(m_Data.nApplicationType == 0) 
	{
		GetDlgItem(IDC_CMD_NLNK_STIFF_TEXT)->SetWindowText(_LS(IDS_CMD_NLNK_STIFF));
		GetDlgItem(IDC_CMD_NLNK_DAMP_TEXT)->SetWindowText(_LS(IDS_CMD_NLNK_DAMPING));
	}
	else if(m_Data.nApplicationType == 1) 
	{
		GetDlgItem(IDC_CMD_NLNK_STIFF_TEXT)->SetWindowText(_LS(IDS_CMD_NLNK_EFF_STIFF));
		GetDlgItem(IDC_CMD_NLNK_DAMP_TEXT)->SetWindowText(_LS(IDS_CMD_NLNK_EFF_DAMPING));
	}  

	AlignCtrls();
	EnableDisableSeisDvCtrls();
		
	UpdateData(FALSE);  // 변수에 입력된 값을 나타냄
	OnCmdNllkLChk();
	m_bOnInit=FALSE;

    InitCtrlTexts();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNLLinkPrtItemDlg::Data2Dlg()
{
	if (m_Data.nType < 0 || m_Data.nType > 7) return;

	m_wndName.SetWindowText(m_Data.PropName);
	m_nAppType = m_Data.nApplicationType;

	MakeCombo(m_Data.nApplicationType);

	if (m_nAppType == 0)
		CDlgUtil::CobxSetCurSelItemData(m_wndPropType, m_Data.nElemType);
	else if(m_nAppType==1)
	  CDlgUtil::CobxSetCurSelItemData(m_wndPropType, m_Data.nType);
	
	m_wndDescription.SetWindowText(m_Data.Description);
	m_wndMassChk.SetCheck((int)m_Data.bUseMass);
	m_wndMassEdit.EnableWindow(m_Data.bUseMass);
	m_wndMassRatioEdit_i.EnableWindow(m_Data.bUseMass);
	m_wndMassRatioEdit_j.EnableWindow(m_Data.bUseMass);
	if (m_Data.bUseMass) m_wndMassEdit.SetEditUnit(m_Data.dTotalMass);
	m_wndMassRatioEdit_i.SetEditUnit(m_Data.dMassRatio_i);
	m_wndMassRatioEdit_j.SetEditUnit(1.0-m_Data.dMassRatio_i);
	m_wndWeightEdit.SetEditUnit(m_Data.dTotalWeight);
	m_wndWeightRatioEdit_i.SetEditUnit(m_Data.dWeightRatio_i);
	m_wndWeightRatioEdit_j.SetEditUnit(1.0-m_Data.dWeightRatio_i);
	m_wndDySprLoc.SetEditUnit(m_Data.dDistRatioU2);
	m_wndDzSprLoc.SetEditUnit(m_Data.dDistRatioU3);
	m_chkShearSpr.SetCheck(m_Data.bShearSprLoc);  

	for(int i = 0; i < 6; i++)
	{
		if(m_Data.LDP_cDir[i] == '0') m_bLChk[i] = FALSE;
		else                          m_bLChk[i] = TRUE;
		
		if(m_Data.NDP_cDir[i] == '0') m_bNChk[i] = FALSE;
		else                          m_bNChk[i] = TRUE;
		
		m_wndStifEdit[i].SetEditUnit(m_Data.LDP_dEffStiff[i]);
		m_wndStifEdit2[i].SetEditUnit(m_Data.LDP_dEffDamping[i]);
		m_pDlgUtil->CtrlEnableDisable(this, m_DofGroup[i], m_bLChk[i]);  // 각 Dof 그룹의 Item 초기화
		
		if(m_bNChk[i] == FALSE) m_wndNBtn[i].EnableWindow(FALSE);        // 예외처리
	}
	m_chkShearSpr.EnableWindow(m_bLChk[1] || m_bLChk[2]); 
	m_wndDySprLoc.EnableWindow(m_bLChk[1] && m_Data.bShearSprLoc);
	m_wndDzSprLoc.EnableWindow(m_bLChk[2] && m_Data.bShearSprLoc);
	
	UpdateData(FALSE);
	OnIehpButtonCtrl();

	// Set seismic type cmb
	std::map<int,CString> mSeisPropType;
	mSeisPropType.insert(std::make_pair(0, _LS(IDS_CMD_SEIS_SDVI)));
	mSeisPropType.insert(std::make_pair(1, _LS(IDS_CMD_SEIS_SDVE)));
	mSeisPropType.insert(std::make_pair(2, _LS(IDS_CMD_SEIS_SDST)));
	mSeisPropType.insert(std::make_pair(3, _LS(IDS_CMD_SEIS_SDHY)));
	// 	mSeisPropType.insert(std::make_pair(4, _LS(IDS_CMD_SEIS_SDIS)));
#ifdef _CIVIL
	mSeisPropType.insert(std::make_pair(4, _LS(IDS_CMD_SEIS_SDIS)));
#endif
#ifdef _MGEN

		BOOL bAddIsolator_GB = FALSE;
#ifdef _CH
		bAddIsolator_GB = TRUE;
#endif
		if (CTestEnvMgr::IsTestEnvST(_T("Isolator(GB)"), _T("yes"))) bAddIsolator_GB = TRUE;
		mSeisPropType.insert(std::make_pair(4, _LS(IDS_CMD_SEIS_SDIS)));
		if (bAddIsolator_GB)
		{
			mSeisPropType.insert(std::make_pair(5, _LS(IDS_CMD_SEIS_ASGB)));
			mSeisPropType.insert(std::make_pair(6, _LS(IDS_CMD_SEIS_DMGB)));
		}
#endif
	

	for(auto itr : mSeisPropType)
		CDlgUtil::CobxAddItem(m_wndSeisDvType, itr.second, itr.first);

	CDlgUtil::CobxAdjustListBoxWidth(m_wndSeisDvType);
	m_wndSeisDvType.SetCurSel(m_Data.nSeisType);

	OnSelChangeSeisTypeCmb();  
}

BOOL CNLLinkPrtItemDlg::Dlg2Data()
{
	UpdateData();

	m_Data.Initialize();
	m_wndName.GetWindowText(m_Data.PropName);
	m_Data.nApplicationType = m_nAppType;
	if (m_nAppType==0)
		m_Data.nElemType = CDlgUtil::CobxGetCurSelItemData(m_wndPropType, m_wndPropType.GetCurSel());
	else if(m_nAppType==1)
	{
#ifdef _CIVIL_JP
		if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_SEIS_ISO_DEVICE))
		{
			CString strErrMsg = _T("");
			strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), _LS(IDS_IDD_MAIN_PROTECTION_EX_OPT_CIVILNX_JP_DLG_IDC_LBC_LOCKOPT_CHK2));
			AfxMessageBox(strErrMsg);
			return FALSE;
		}
#endif
		m_Data.nType = CDlgUtil::CobxGetCurSelItemData(m_wndPropType, m_wndPropType.GetCurSel());
	}
	else if(m_nAppType==2)
	{
#ifdef _CIVIL_JP
		if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_SEIS_ISO_DEVICE))
		{
			CString strErrMsg = _T("");
			strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), _LS(IDS_IDD_MAIN_PROTECTION_EX_OPT_CIVILNX_JP_DLG_IDC_LBC_LOCKOPT_CHK2));
			AfxMessageBox(strErrMsg);
			return FALSE;
		}
#endif
		m_Data.nSeisType = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvType, m_wndSeisDvType.GetCurSel());
		if(m_wndSeisDvProp.GetCurSel()<0) 
		{
			{	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_data_)); return FALSE;	}
		}
		m_Data.nSeisKey  = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvProp, m_wndSeisDvProp.GetCurSel());
	}
	else ASSERT(0);

	m_wndDescription.GetWindowText(m_Data.Description);
	m_Data.bUseMass = (m_wndMassChk.GetCheck()==1) ? TRUE : FALSE;
	if (m_Data.bUseMass) m_Data.dTotalMass   = m_wndMassEdit.GetEditValue();
	m_Data.dMassRatio_i   = m_wndMassRatioEdit_i.GetEditValue();
	//m_Data.dMassRatio_j   = m_wndMassRatioEdit_j.GetEditValue();
	m_Data.dTotalWeight = m_wndWeightEdit.GetEditValue();
	m_Data.dWeightRatio_i   = m_wndWeightRatioEdit_i.GetEditValue();
	m_Data.dDistRatioU2 = m_wndDySprLoc.GetEditValue();
	m_Data.dDistRatioU3 = m_wndDzSprLoc.GetEditValue();
//3  m_Data.bShearSprLoc = m_bShearSprLoc;
	m_Data.bShearSprLoc = m_chkShearSpr.GetCheck();
	
	for(int i = 0; i < 6; i++)
	{
		if(m_bLChk[i] == FALSE) m_Data.LDP_cDir[i] = '0';
		else                    m_Data.LDP_cDir[i] = '1';
		
		if(m_bNChk[i] == FALSE) m_Data.NDP_cDir[i] = '0';
		else                    m_Data.NDP_cDir[i] = '1';    
		m_Data.LDP_dEffStiff[i] = m_wndStifEdit[i].GetEditValue();
		m_Data.LDP_dEffDamping[i] = m_wndStifEdit2[i].GetEditValue();
	}
	
	if (m_nAppType == 0)    // element type 1
	{
		if (m_Data.nElemType == 0)        // spring support only stiffness
		{
			for (int i = 0; i < 21; i++)
			{
				m_dDamp[i] = 0.;
			}
		}
		else if (m_Data.nElemType == 1)        // linear dashpot support only damping
		{
			for (int i = 0; i < 21; i++)
			{
				m_dStif[i] = 0.;
			}
		}
	}
	else if (m_nAppType == 2)    // element type 2 : seismic devices does not support coupled term
	{
		for (int i = 0; i < 15; i++)
		{
			m_dStif[i + 6] = 0.;
			m_dDamp[i + 6] = 0.;
		}
	}

	double dTolerance = 1.0e-8;
	BOOL bAllZero = TRUE;
	for(int  i=0; i<15; i++ )
	{
		m_Data.LDP_dEffStif_Coupled[i] = m_dStif[i+6];
		m_Data.LDP_dEffDamp_Coupled[i] = m_dDamp[i+6];
		
		if( fabs(m_Data.LDP_dEffStif_Coupled[i]) > dTolerance || 
				fabs(m_Data.LDP_dEffDamp_Coupled[i]) > dTolerance )
		{
			bAllZero = FALSE;
		}
	}
	
	if( bAllZero ) m_Data.nICoupld = 0;
	else           m_Data.nICoupld = 1;

	if (m_nAppType == 1)
	{
		switch(m_Data.nType)
		{
		case 0:   // Visco-elastic damper
			{
				for(int i = 0; i < 6; i++)
				{
					m_Data.NDP_Prop[i].VD.dDamperStiffness  = m_VD[i].dDamperStiffness;
					m_Data.NDP_Prop[i].VD.dDamping          = m_VD[i].dDamping;
					m_Data.NDP_Prop[i].VD.dDampExp          = m_VD[i].dDampExp;
					m_Data.NDP_Prop[i].VD.dBracingStiffness = m_VD[i].dBracingStiffness;
					m_Data.NDP_Prop[i].VD.nVEDType     = m_VD[i].nVEDType;
					m_Data.NDP_Prop[i].VD.dRefVelocity = m_VD[i].dRefVelocity;
				}
			} break;
		case 1:   // Gap
			{
				for(int i = 0; i < 6; i++)
				{
					m_Data.NDP_Prop[i].GAP.dStiffness        = m_GAP[i].dStiffness;
					m_Data.NDP_Prop[i].GAP.dOpen             = m_GAP[i].dOpen     ;
				}
			} break;
		case 2:   // Hook
			{
				for(int i = 0; i < 6; i++)
				{
					m_Data.NDP_Prop[i].HOOK.dStiffness        = m_HOOK[i].dStiffness;
					m_Data.NDP_Prop[i].HOOK.dOpen             = m_HOOK[i].dOpen     ;
				}
			} break;  
		case 3:   // Hysteretic System
			{
				for(int i = 0; i < 6; i++)
				{
					m_Data.NDP_Prop[i].HS.dStiffness           = m_HS[i].dStiffness          ;
					m_Data.NDP_Prop[i].HS.dYieldStrength       = m_HS[i].dYieldStrength      ;
					m_Data.NDP_Prop[i].HS.dPostYieldStiffRatio = m_HS[i].dPostYieldStiffRatio;
					m_Data.NDP_Prop[i].HS.dYieldingExp         = m_HS[i].dYieldingExp        ;
					m_Data.NDP_Prop[i].HS.dLoopParamB          = m_HS[i].dLoopParamB         ;
					m_Data.NDP_Prop[i].HS.dLoopParamA          = m_HS[i].dLoopParamA         ;
				}
			} break;
		case 4:   // Lead Rubber Bearing Isolator
			{
				for(int i = 0; i < 6; i++)
				{
					if(i == 1 || i == 2)
					{
						m_Data.NDP_Prop[i].LRBI.dStiffness           = m_LRBI[i].dStiffness          ;
						m_Data.NDP_Prop[i].LRBI.dYieldStrength       = m_LRBI[i].dYieldStrength      ;
						m_Data.NDP_Prop[i].LRBI.dPostYieldStiffRatio = m_LRBI[i].dPostYieldStiffRatio;
						m_Data.NDP_Prop[i].LRBI.dLoopParamB          = m_LRBI[i].dLoopParamB         ;
						m_Data.NDP_Prop[i].LRBI.dLoopParamA          = m_LRBI[i].dLoopParamA         ;
					}
					else
					{
						m_Data.NDP_Prop[i].GAP.dStiffness        = m_GAP[i].dStiffness;
						m_Data.NDP_Prop[i].GAP.dOpen             = 0.    ;
					}
				}        
			} break;
		case 5:   // Friction Pendulum System Isolator
			{
				for(int i = 0; i < 6; i++)
				{          
					if(i == 1 || i == 2)
					{
						m_Data.NDP_Prop[i].FPSI.dStiffness        = m_FPSI[i].dStiffness    ;
						m_Data.NDP_Prop[i].FPSI.dFricCoeffSlow    = m_FPSI[i].dFricCoeffSlow;
						m_Data.NDP_Prop[i].FPSI.dFricCoeffFast    = m_FPSI[i].dFricCoeffFast;
						m_Data.NDP_Prop[i].FPSI.dRateParam        = m_FPSI[i].dRateParam    ;
						m_Data.NDP_Prop[i].FPSI.dRadius           = m_FPSI[i].dRadius       ;
						m_Data.NDP_Prop[i].FPSI.dLoopParamB       = m_FPSI[i].dLoopParamB   ;
						m_Data.NDP_Prop[i].FPSI.dLoopParamA       = m_FPSI[i].dLoopParamA   ;
					}
					else
					{
						m_Data.NDP_Prop[i].GAP.dStiffness        = m_GAP[i].dStiffness;
						m_Data.NDP_Prop[i].GAP.dOpen             = 0.     ;
					}
				}
			} break;
		case 6:   // Triple Friction Pendulum System Isolator
			{
				for(int i = 0; i < 6; i++)
				{          
					if(i == 1 || i == 2)
					{
						m_Data.NDP_Prop[i].TFPSI.bSymOut = m_TFPSI[i].bSymOut;
						m_Data.NDP_Prop[i].TFPSI.dHeightIn  = m_TFPSI[i].dHeightIn;
						m_Data.NDP_Prop[i].TFPSI.dHeightOut = m_TFPSI[i].dHeightOut;

						for(int j = 0; j < 4; j++)
						{
							m_Data.NDP_Prop[i].TFPSI.dStiffness[j]      = m_TFPSI[i].dStiffness[j]    ;
							m_Data.NDP_Prop[i].TFPSI.dFricCoeffSlow[j]  = m_TFPSI[i].dFricCoeffSlow[j];
							m_Data.NDP_Prop[i].TFPSI.dFricCoeffFast[j]  = m_TFPSI[i].dFricCoeffFast[j];
							m_Data.NDP_Prop[i].TFPSI.dRateParam[j]      = m_TFPSI[i].dRateParam[j]    ;
							m_Data.NDP_Prop[i].TFPSI.dRadius[j]         = m_TFPSI[i].dRadius[j]       ;
							m_Data.NDP_Prop[i].TFPSI.dStopDist[j]       = m_TFPSI[i].dStopDist[j]   ;
						}
					}
					else
					{
						m_Data.NDP_Prop[i].GAP.dStiffness        = m_GAP[i].dStiffness;
						m_Data.NDP_Prop[i].GAP.dOpen             = 0.     ;
					}
				}
			} break;
		case 7:   // Double Friction Pendulum System Isolator
			{
				for(int i = 0; i < 6; i++)
				{          
					if(i == 1 || i == 2)
					{
						m_Data.NDP_Prop[i].DFPSI.dAreaPressure = m_DFPSI[i].dAreaPressure;
						m_Data.NDP_Prop[i].DFPSI.dRefAxialPressure = m_DFPSI[i].dRefAxialPressure;
						for(int j = 0; j < 2; j++)
						{
							m_Data.NDP_Prop[i].DFPSI.dStiffness[j]      = m_DFPSI[i].dStiffness[j]    ;
							m_Data.NDP_Prop[i].DFPSI.dCoeff_uref[j]     = m_DFPSI[i].dCoeff_uref[j]   ;
							m_Data.NDP_Prop[i].DFPSI.dRateParam[j]      = m_DFPSI[i].dRateParam[j]    ;
							m_Data.NDP_Prop[i].DFPSI.dRadius[j]         = m_DFPSI[i].dRadius[j]       ;
							m_Data.NDP_Prop[i].DFPSI.dStopDist[j]       = m_DFPSI[i].dStopDist[j]     ;
							m_Data.NDP_Prop[i].DFPSI.dRatioCoeff_uv[j]  = m_DFPSI[i].dRatioCoeff_uv[j];
							m_Data.NDP_Prop[i].DFPSI.dRatioCoeff_up[j]  = m_DFPSI[i].dRatioCoeff_up[j];
							m_Data.NDP_Prop[i].DFPSI.dParamCoeff_ap[j]  = m_DFPSI[i].dParamCoeff_ap[j];
							m_Data.NDP_Prop[i].DFPSI.dParamCoeff_hp[j]  = m_DFPSI[i].dParamCoeff_hp[j];
						}
					}
					else
					{
						m_Data.NDP_Prop[i].GAP.dStiffness        = m_GAP[i].dStiffness;
						m_Data.NDP_Prop[i].GAP.dOpen             = 0.     ;
					}
				}
			} break;
	  }
	}
	
	////////// Error Check /////////////
	if(m_Data.PropName == _T(""))
	{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_BlankPropertyName)); 	return FALSE;	}	
	if(m_wndMassChk.GetCheck() == 1 && m_wndMassEdit.GetEditValue() < 0)
	{	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Mass)); 	return FALSE;	}
	if(m_wndMassRatioEdit_i.GetEditValue() < 0.0)   m_wndMassRatioEdit_i.SetEditUnit(0.0);
	if(m_wndMassRatioEdit_i.GetEditValue() > 1.0)   m_wndMassRatioEdit_i.SetEditUnit(1.0);
	if(m_wndWeightEdit.GetEditValue() < 0)
	{	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Weight)); return FALSE;	}
	if(m_wndWeightRatioEdit_i.GetEditValue() < 0.0)   m_wndWeightRatioEdit_i.SetEditUnit(0.0);
	if(m_wndWeightRatioEdit_i.GetEditValue() > 1.0)   m_wndWeightRatioEdit_i.SetEditUnit(1.0);
	
	if(m_bLChk[0] == FALSE && m_bLChk[1] == FALSE && m_bLChk[2] == FALSE && 
		 m_bLChk[3] == FALSE && m_bLChk[4] == FALSE && m_bLChk[5] == FALSE) 
	{	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_LAllUnChk)); return FALSE; }

	for(int i = 0; i < 6; i++)
	{
		if(m_bLChk[i])   // 해당 dof가 ture 인 경우에만 에러 체크를 함
		{
			if(m_wndStifEdit[i].GetEditValue() < 0)
		  {
			  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Effective_Stiffness)); return FALSE;
		  }
			switch(i)   // DY, DZ 인 경우 Spring Location의 Error Check 한다.
			{
			  case 0: case 3: case 4: case 5: case 6: break;
				case 1:
					if(m_chkShearSpr.GetCheck() == TRUE && //3 bShearSprLoc == TRUE && 
						(m_wndDySprLoc.GetEditValue() < 0 || m_wndDySprLoc.GetEditValue() > 1.0))
					{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Spring_Location));  return FALSE; }
					break;
				case 2:
					if(m_chkShearSpr.GetCheck() == TRUE && //3 m_bShearSprLoc == TRUE &&
						(m_wndDzSprLoc.GetEditValue() < 0 || m_wndDzSprLoc.GetEditValue() > 1.0))
					{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Spring_Location)); 	return FALSE; }
					break;
			}  // end of _T("switch statement")
		} // end of _T("if statement")
	} // end of _T("for statement")

	if(m_bNChk[0] == FALSE && m_bNChk[1] == FALSE && m_bNChk[2] == FALSE && 
		 m_bNChk[3] == FALSE && m_bNChk[4] == FALSE && m_bNChk[5] == FALSE &&
		 m_Data.nApplicationType == 1) 
	{	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_NAllUnChk)); return FALSE; }

	if(m_Data.nType == 5)
	{
		if(m_bNChk[0] == FALSE)
		{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_FPSDX)); 	return FALSE; }
		if(m_bNChk[1] == FALSE && m_bNChk[2] == FALSE)
		{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_FPSDY)); 	return FALSE; }          
	}
	
	return TRUE;
}

///////// Check for Linear Spring Properties  //////////////    
void CNLLinkPrtItemDlg::OnCmdNllkLChk() 
{
	UpdateData(TRUE);
	int nSel = m_wndPropType.GetCurSel();
	int nType = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvType, m_wndSeisDvType.GetCurSel());

	for(int i = 0; i < 6; i++)
	{
		if(m_bLChk[i] == FALSE)  // linear spring이 uncheck이면 nonlinear spring을 강제로 unckeck 시킨다.
			m_bNChk[i] = FALSE;   
				    
		m_wndStifEdit[i].EnableWindow(m_bLChk[i] && !(m_nAppType==0 && nSel==1));
		m_wndStifEdit2[i].EnableWindow(m_bLChk[i] && ((!(m_nAppType==0 && nSel==0) && m_nAppType!=2) || (m_nAppType==2 && nType==5)));
		m_wndDySprLoc.EnableWindow(m_bLChk[1] && m_chkShearSpr.GetCheck() /*m_bShearSprLoc*/);    // only related to DY
		m_wndDzSprLoc.EnableWindow(m_bLChk[2] && m_chkShearSpr.GetCheck() /*m_bShearSprLoc*/);    // only related to DZ
		m_wndNChk[i].EnableWindow(m_bLChk[i] && m_nAppType==1);
		m_wndNBtn[i].EnableWindow(m_bNChk[i] && m_nAppType==1);
	}
	m_chkShearSpr.EnableWindow(m_bLChk[1] || m_bLChk[2]);
	if(!(m_bLChk[1] || m_bLChk[2])) m_chkShearSpr.SetCheck(FALSE);  // Dy, Dz가 모두 FALSE이면 ShearSprLocation을 강제로 FALSE시킨다.

	if(m_nType==6 || m_nType==7) 
	{
		m_wndLChk[2].EnableWindow(FALSE);
		m_wndStifEdit[2].EnableWindow(FALSE);
		m_wndStifEdit2[2].EnableWindow(FALSE);
		m_wndNChk[2].EnableWindow(FALSE);
		m_wndNBtn[2].EnableWindow(FALSE);
		m_bLChk[2] = m_bLChk[1];
		UpdateData(FALSE);
	}	

	if(m_nAppType==2) UpdateRemarkText();
}

void CNLLinkPrtItemDlg::CtrlLNProperty()
{
	UpdateData();
	int nSel = m_wndPropType.GetCurSel();
	
	if (m_nAppType == 0)
	{
		//Nonlinear 항목 Disable
		for (int i=0; i<6; i++)
		{
			m_wndNChk[i].SetCheck(FALSE);
			m_wndNChk[i].EnableWindow(FALSE);
			m_wndNBtn[i].EnableWindow(FALSE);
		}

		if (nSel == 0)
		{
			for (int i=0; i<6; i++)
				m_wndStifEdit2[i].EnableWindow(FALSE);
		}
		else if (nSel == 1)
		{
			for (int i=0; i<6; i++)
				m_wndStifEdit[i].EnableWindow(FALSE);
		}
		else if (nSel == 2)
		{
			for (int i=0; i<6; i++)
			{
				m_wndStifEdit2[i].EnableWindow(TRUE);
				m_wndStifEdit[i].EnableWindow(TRUE);
			}
		}
	}
}

////// Check for Nonlinear Spring Properties  //////////////    
void CNLLinkPrtItemDlg::OnCmdNllkNDxChk() 
{
	UpdateData(TRUE);
	m_wndNBtn[0].EnableWindow(m_bNChk[0]);
	if(m_bNChk[0]) OnCmdNonLinDxBtn();  
}
void CNLLinkPrtItemDlg::OnCmdNllkNDyChk() 
{
	UpdateData(TRUE);
	m_wndNBtn[1].EnableWindow(m_bNChk[1]);
	if(m_nType==6 || m_nType==7) 
	{
		m_bNChk[2] = m_bNChk[1];
		UpdateData(FALSE);
	}
	if(m_bNChk[1]) OnCmdNonLinDyBtn();  	
}
void CNLLinkPrtItemDlg::OnCmdNllkNDzChk() 
{
	UpdateData(TRUE);
	m_wndNBtn[2].EnableWindow(m_bNChk[2]);
	if(m_bNChk[2]) OnCmdNonLinDzBtn();  
}
void CNLLinkPrtItemDlg::OnCmdNllkNRxChk() 
{
	UpdateData(TRUE);
	m_wndNBtn[3].EnableWindow(m_bNChk[3]);
	if(m_bNChk[3]) OnCmdNonLinRxBtn();  
}
void CNLLinkPrtItemDlg::OnCmdNllkNRyChk() 
{
	UpdateData(TRUE);
	m_wndNBtn[4].EnableWindow(m_bNChk[4]);
	if(m_bNChk[4]) OnCmdNonLinRyBtn();  
}
void CNLLinkPrtItemDlg::OnCmdNllkNRzChk() 
{
	UpdateData(TRUE);
	m_wndNBtn[5].EnableWindow(m_bNChk[5]);
	if(m_bNChk[5]) OnCmdNonLinRzBtn();  
}

////////////////////////////////////////////////////////////////
////// Buttons for Add/Modify Nonlinear Spring Properties  /////    
void CNLLinkPrtItemDlg::OnCmdNonLinDxBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_NLLK_DX_BTN);  // Nonlinear property를 수정할 때 Button Text 변경
	CString csText;
	pWnd->GetWindowText(csText);
	pWnd->SetWindowText(csText+_T(">>"));

	switch(m_nType)
	{
		case 0: OnCmdViscDlg(0); break;
		case 1: OnCmdGapDlg (0); break;
		case 2: OnCmdHookDlg(0); break;
		case 3: OnCmdHystDlg(0); break;
		case 4: OnCmdGapDlg (0); break;    // Lead Rubber Isolator는 DX Dof에 대해 Gap을 부름   
		case 5: OnCmdGapDlg (0); break;    // Friction Pendulum 은 DX Dof에 대해 Gap을 부름   
		case 6: OnCmdGapDlg (0); break;    // Triple Friction Pendulum 은 DX Dof에 대해 Gap을 부름   
		case 7: OnCmdGapDlg (0); break;    // Double Friction Pendulum 은 DX Dof에 대해 Gap을 부름   
	}
	pWnd->SetWindowText(csText);
}

void CNLLinkPrtItemDlg::OnCmdNonLinDyBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_NLLK_DY_BTN);
	CString csText;
	pWnd->GetWindowText(csText);
	pWnd->SetWindowText(csText+_T(">>"));

	switch(m_nType)
	{
		case 0: OnCmdViscDlg(1);  break;
		case 1: OnCmdGapDlg (1);  break;
		case 2: OnCmdHookDlg(1);  break;
		case 3: OnCmdHystDlg(1);  break;
		case 4: OnCmdLeadDlg(1);  break;
		case 5: OnCmdFricDlg(1);  break; 
		case 6: OnCmdTfricDlg(1); break;
		case 7: OnCmdDfricDlg(1); break;
	}
	pWnd->SetWindowText(csText);
}

void CNLLinkPrtItemDlg::OnCmdNonLinDzBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_NLLK_DZ_BTN);
	CString csText;
	pWnd->GetWindowText(csText);
	pWnd->SetWindowText(csText+_T(">>"));

	switch(m_nType)
	{
		case 0: OnCmdViscDlg(2);  break;
		case 1: OnCmdGapDlg (2);  break;
		case 2: OnCmdHookDlg(2);  break;
		case 3: OnCmdHystDlg(2);  break;
		case 4: OnCmdLeadDlg(2);  break;
		case 5: OnCmdFricDlg(2);  break; 
		//case 6: OnCmdTfricDlg(2); break; // 버튼이 비활성화 되어서 여기 들어오면 안됨!!!
	}
	pWnd->SetWindowText(csText);
}

void CNLLinkPrtItemDlg::OnCmdNonLinRxBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_NLLK_RX_BTN);
	CString csText;
	pWnd->GetWindowText(csText);
	pWnd->SetWindowText(csText+_T(">>"));

	switch(m_nType)
	{
		case 0: OnCmdViscDlg(3); break;
		case 1: OnCmdGapDlg (3); break;
		case 2: OnCmdHookDlg(3); break;
		case 3: OnCmdHystDlg(3); break;
		case 4: OnCmdGapDlg (3); break;
		case 5: OnCmdGapDlg (3); break;    
		case 6: OnCmdGapDlg (3); break;    
		case 7: OnCmdGapDlg (3); break;   
	}
	pWnd->SetWindowText(csText);
}

void CNLLinkPrtItemDlg::OnCmdNonLinRyBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_NLLK_RY_BTN);
	CString csText;
	pWnd->GetWindowText(csText);
	pWnd->SetWindowText(csText+_T(">>"));

	switch(m_nType)
	{
		case 0: OnCmdViscDlg(4); break;
		case 1: OnCmdGapDlg (4); break;
		case 2: OnCmdHookDlg(4); break;
		case 3: OnCmdHystDlg(4); break;
		case 4: OnCmdGapDlg (4); break;
		case 5: OnCmdGapDlg (4); break;    
		case 6: OnCmdGapDlg (4); break;    
		case 7: OnCmdGapDlg (4); break;   
	}
	pWnd->SetWindowText(csText);
}

void CNLLinkPrtItemDlg::OnCmdNonLinRzBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_NLLK_RZ_BTN);
	CString csText;
	pWnd->GetWindowText(csText);
	pWnd->SetWindowText(csText+_T(">>"));

	switch(m_nType)
	{
		case 0: OnCmdViscDlg(5); break;
		case 1: OnCmdGapDlg (5); break;
		case 2: OnCmdHookDlg(5); break;
		case 3: OnCmdHystDlg(5); break;
		case 4: OnCmdGapDlg (5); break;
		case 5: OnCmdGapDlg (5); break;    
		case 6: OnCmdGapDlg (5); break;    
		case 7: OnCmdGapDlg (5); break;    
	}
	pWnd->SetWindowText(csText);
}

void CNLLinkPrtItemDlg::OnCmdCoupledBtn()
{
	for( int i=0; i<6; i++ )
	{
		m_dStif[i] = m_wndStifEdit[i].GetEditValue();
		m_dDamp[i] = m_wndStifEdit2[i].GetEditValue();
	}

	CNLLinkPrtItemCoupledDlg dlg;
	//dlg.SetParamData(m_nAppType, m_nType); // The type is not updated when the Dialog is running.
	dlg.SetParamData(m_nAppType, m_wndPropType.GetCurSel());
	dlg.SetInitPos(D_INIT_POS_RT);
	
	memcpy(dlg.m_dStif, m_dStif, sizeof(m_dStif));
	memcpy(dlg.m_dDamp, m_dDamp, sizeof(m_dDamp));

	if(dlg.DoModal() == IDOK)
	{
		memcpy(m_dStif, dlg.m_dStif, sizeof(m_dStif));
		memcpy(m_dDamp, dlg.m_dDamp, sizeof(m_dDamp));
		
		for( int i=0; i<6; i++ )
		{
			m_wndStifEdit[i].SetEditUnit(m_dStif[i]);
			m_wndStifEdit2[i].SetEditUnit(m_dDamp[i]);
		}
	}
}

void CNLLinkPrtItemDlg::OnCmdNllkDyEdt()
{
	if(m_nType==6 || m_nType==7)
	{
		double dStiff = m_wndStifEdit[1].GetEditValue();
		m_wndStifEdit[2].SetEditUnit(dStiff);		
	}	
}

void CNLLinkPrtItemDlg::OnCmdNllkDyEdt2()
{
	if(m_nType==6 || m_nType==7)
	{
		double dStiff2 = m_wndStifEdit2[1].GetEditValue();
		m_wndStifEdit2[2].SetEditUnit(dStiff2);		
	}	
}

/////// Show corresponding child dialog and Change Data ////////////////
void CNLLinkPrtItemDlg::OnCmdViscDlg(int nDofType)
{
	CNLLinkPrtViscDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	dlg.m_nDofType  = nDofType;   // 현재 Child Dlg의 종류를 나타내는 Flag

	dlg.m_Data.NDP_Prop[nDofType].VD.dDamperStiffness  = m_VD[nDofType].dDamperStiffness ;
	dlg.m_Data.NDP_Prop[nDofType].VD.dDamping          = m_VD[nDofType].dDamping         ;
	dlg.m_Data.NDP_Prop[nDofType].VD.dDampExp          = m_VD[nDofType].dDampExp         ;
	dlg.m_Data.NDP_Prop[nDofType].VD.dBracingStiffness = m_VD[nDofType].dBracingStiffness;
	dlg.m_Data.NDP_Prop[nDofType].VD.nVEDType     = m_VD[nDofType].nVEDType    ;
	dlg.m_Data.NDP_Prop[nDofType].VD.dRefVelocity = m_VD[nDofType].dRefVelocity;
	if(dlg.DoModal() == IDOK)
	{
		m_VD[nDofType].dDamperStiffness  = dlg.m_Data.NDP_Prop[nDofType].VD.dDamperStiffness ;
		m_VD[nDofType].dDamping          = dlg.m_Data.NDP_Prop[nDofType].VD.dDamping         ; 
		m_VD[nDofType].dDampExp          = dlg.m_Data.NDP_Prop[nDofType].VD.dDampExp         ;
		m_VD[nDofType].dBracingStiffness = dlg.m_Data.NDP_Prop[nDofType].VD.dBracingStiffness;
		m_VD[nDofType].nVEDType     = dlg.m_Data.NDP_Prop[nDofType].VD.nVEDType    ;
		m_VD[nDofType].dRefVelocity = dlg.m_Data.NDP_Prop[nDofType].VD.dRefVelocity;
	} 
}

void CNLLinkPrtItemDlg::OnCmdGapDlg(int nDofType)
{
	CNLLinkPrtGapDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	dlg.m_nDofType  = nDofType;   // 현재 Child Dlg의 종류를 나타내는 Flag
	dlg.m_nCodeType = m_nType;    // Friction Pendulum에서 Gap을 사용할 때 필요함

	dlg.m_Data.NDP_Prop[nDofType].GAP.dStiffness        = m_GAP[nDofType].dStiffness;
	dlg.m_Data.NDP_Prop[nDofType].GAP.dOpen             = m_GAP[nDofType].dOpen     ;

	if(dlg.DoModal() == IDOK)
	{
		m_GAP[nDofType].dStiffness        = dlg.m_Data.NDP_Prop[nDofType].GAP.dStiffness;
		m_GAP[nDofType].dOpen             = dlg.m_Data.NDP_Prop[nDofType].GAP.dOpen     ;     
	} 
}

void CNLLinkPrtItemDlg::OnCmdHookDlg(int nDofType)
{
	CNLLinkPrtHookDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	dlg.m_nDofType  = nDofType;   // 현재 Child Dlg의 종류를 나타내는 Flag

	dlg.m_Data.NDP_Prop[nDofType].HOOK.dStiffness        = m_HOOK[nDofType].dStiffness;
	dlg.m_Data.NDP_Prop[nDofType].HOOK.dOpen             = m_HOOK[nDofType].dOpen     ;

	if(dlg.DoModal() == IDOK)
	{
		m_HOOK[nDofType].dStiffness        = dlg.m_Data.NDP_Prop[nDofType].HOOK.dStiffness;
		m_HOOK[nDofType].dOpen             = dlg.m_Data.NDP_Prop[nDofType].HOOK.dOpen     ; 
	} 
}

void CNLLinkPrtItemDlg::OnCmdHystDlg(int nDofType)
{
	CNLLinkPrtHystDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	dlg.m_nDofType  = nDofType;   // 현재 Child Dlg의 종류를 나타내는 Flag

	dlg.m_Data.NDP_Prop[nDofType].HS.dStiffness           = m_HS[nDofType].dStiffness          ;
	dlg.m_Data.NDP_Prop[nDofType].HS.dYieldStrength       = m_HS[nDofType].dYieldStrength      ;
	dlg.m_Data.NDP_Prop[nDofType].HS.dPostYieldStiffRatio = m_HS[nDofType].dPostYieldStiffRatio;
	dlg.m_Data.NDP_Prop[nDofType].HS.dYieldingExp         = m_HS[nDofType].dYieldingExp        ;
	dlg.m_Data.NDP_Prop[nDofType].HS.dLoopParamB          = m_HS[nDofType].dLoopParamB         ;
	dlg.m_Data.NDP_Prop[nDofType].HS.dLoopParamA          = m_HS[nDofType].dLoopParamA         ;

	if(dlg.DoModal() == IDOK)
	{
		m_HS[nDofType].dStiffness           = dlg.m_Data.NDP_Prop[nDofType].HS.dStiffness          ;
		m_HS[nDofType].dYieldStrength       = dlg.m_Data.NDP_Prop[nDofType].HS.dYieldStrength      ; 
		m_HS[nDofType].dPostYieldStiffRatio = dlg.m_Data.NDP_Prop[nDofType].HS.dPostYieldStiffRatio;
		m_HS[nDofType].dYieldingExp         = dlg.m_Data.NDP_Prop[nDofType].HS.dYieldingExp        ;
		m_HS[nDofType].dLoopParamB          = dlg.m_Data.NDP_Prop[nDofType].HS.dLoopParamB         ;
		m_HS[nDofType].dLoopParamA          = dlg.m_Data.NDP_Prop[nDofType].HS.dLoopParamA         ;
	} 
}

void CNLLinkPrtItemDlg::OnCmdLeadDlg(int nDofType)
{
	CNLLinkPrtLeadDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	dlg.m_nDofType  = nDofType;   // 현재 Child Dlg의 종류를 나타내는 Flag

	dlg.m_Data.NDP_Prop[nDofType].LRBI.dStiffness           = m_LRBI[nDofType].dStiffness          ;
	dlg.m_Data.NDP_Prop[nDofType].LRBI.dYieldStrength       = m_LRBI[nDofType].dYieldStrength      ;
	dlg.m_Data.NDP_Prop[nDofType].LRBI.dPostYieldStiffRatio = m_LRBI[nDofType].dPostYieldStiffRatio;
	dlg.m_Data.NDP_Prop[nDofType].LRBI.dLoopParamB          = m_LRBI[nDofType].dLoopParamB         ;
	dlg.m_Data.NDP_Prop[nDofType].LRBI.dLoopParamA          = m_LRBI[nDofType].dLoopParamA         ;

	if(dlg.DoModal() == IDOK)
	{
		m_LRBI[nDofType].dStiffness           = dlg.m_Data.NDP_Prop[nDofType].LRBI.dStiffness          ;
		m_LRBI[nDofType].dYieldStrength       = dlg.m_Data.NDP_Prop[nDofType].LRBI.dYieldStrength      ; 
		m_LRBI[nDofType].dPostYieldStiffRatio = dlg.m_Data.NDP_Prop[nDofType].LRBI.dPostYieldStiffRatio;
		m_LRBI[nDofType].dLoopParamB          = dlg.m_Data.NDP_Prop[nDofType].LRBI.dLoopParamB         ;
		m_LRBI[nDofType].dLoopParamA          = dlg.m_Data.NDP_Prop[nDofType].LRBI.dLoopParamA         ;
	} 
}

void CNLLinkPrtItemDlg::OnCmdFricDlg(int nDofType)
{
	CNLLinkPrtFricDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	dlg.m_nDofType  = nDofType;   // 현재 Child Dlg의 종류를 나타내는 Flag

	dlg.m_Data.NDP_Prop[nDofType].FPSI.dStiffness        = m_FPSI[nDofType].dStiffness    ;
	dlg.m_Data.NDP_Prop[nDofType].FPSI.dFricCoeffSlow    = m_FPSI[nDofType].dFricCoeffSlow;
	dlg.m_Data.NDP_Prop[nDofType].FPSI.dFricCoeffFast    = m_FPSI[nDofType].dFricCoeffFast;
	dlg.m_Data.NDP_Prop[nDofType].FPSI.dRateParam        = m_FPSI[nDofType].dRateParam    ;
	dlg.m_Data.NDP_Prop[nDofType].FPSI.dRadius           = m_FPSI[nDofType].dRadius       ;
	dlg.m_Data.NDP_Prop[nDofType].FPSI.dLoopParamB       = m_FPSI[nDofType].dLoopParamB   ;
	dlg.m_Data.NDP_Prop[nDofType].FPSI.dLoopParamA       = m_FPSI[nDofType].dLoopParamA   ;
	
	if(dlg.DoModal() == IDOK)
	{
		m_FPSI[nDofType].dStiffness        = dlg.m_Data.NDP_Prop[nDofType].FPSI.dStiffness    ;
		m_FPSI[nDofType].dFricCoeffSlow    = dlg.m_Data.NDP_Prop[nDofType].FPSI.dFricCoeffSlow; 
		m_FPSI[nDofType].dFricCoeffFast    = dlg.m_Data.NDP_Prop[nDofType].FPSI.dFricCoeffFast;
		m_FPSI[nDofType].dRateParam        = dlg.m_Data.NDP_Prop[nDofType].FPSI.dRateParam    ;
		m_FPSI[nDofType].dRadius           = dlg.m_Data.NDP_Prop[nDofType].FPSI.dRadius       ;
		m_FPSI[nDofType].dLoopParamB       = dlg.m_Data.NDP_Prop[nDofType].FPSI.dLoopParamB   ;
		m_FPSI[nDofType].dLoopParamA       = dlg.m_Data.NDP_Prop[nDofType].FPSI.dLoopParamA   ;
	}
}

void CNLLinkPrtItemDlg::OnCmdTfricDlg(int nDofType)
{
	CNLLinkPrtTfricDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_nDofType  = nDofType;   // 현재 Child Dlg의 종류를 나타내는 Flag

	dlg.m_Data.NDP_Prop[nDofType].TFPSI.bSymOut = m_TFPSI[nDofType].bSymOut;
	dlg.m_Data.NDP_Prop[nDofType].TFPSI.dHeightIn = m_TFPSI[nDofType].dHeightIn;
	dlg.m_Data.NDP_Prop[nDofType].TFPSI.dHeightOut = m_TFPSI[nDofType].dHeightOut;

	for(int i=0; i<4; i++)
	{
		dlg.m_Data.NDP_Prop[nDofType].TFPSI.dStiffness[i]      = m_TFPSI[nDofType].dStiffness[i]    ;
		dlg.m_Data.NDP_Prop[nDofType].TFPSI.dFricCoeffSlow[i]  = m_TFPSI[nDofType].dFricCoeffSlow[i];
		dlg.m_Data.NDP_Prop[nDofType].TFPSI.dFricCoeffFast[i]  = m_TFPSI[nDofType].dFricCoeffFast[i];
		dlg.m_Data.NDP_Prop[nDofType].TFPSI.dRateParam[i]      = m_TFPSI[nDofType].dRateParam[i]    ;
		dlg.m_Data.NDP_Prop[nDofType].TFPSI.dRadius[i]         = m_TFPSI[nDofType].dRadius[i]       ;
		dlg.m_Data.NDP_Prop[nDofType].TFPSI.dStopDist[i]       = m_TFPSI[nDofType].dStopDist[i]     ;
	}


	if(dlg.DoModal() == IDOK)
	{
		m_TFPSI[nDofType].bSymOut = dlg.m_Data.NDP_Prop[nDofType].TFPSI.bSymOut;
		m_TFPSI[nDofType].dHeightIn  = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dHeightIn;
		m_TFPSI[nDofType].dHeightOut = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dHeightOut;

		for(int i=0; i<4; i++)
		{
			m_TFPSI[nDofType].dStiffness[i]      = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dStiffness[i]    ;
			m_TFPSI[nDofType].dFricCoeffSlow[i]  = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dFricCoeffSlow[i]; 
			m_TFPSI[nDofType].dFricCoeffFast[i]  = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dFricCoeffFast[i];
			m_TFPSI[nDofType].dRateParam[i]      = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dRateParam[i]    ;
			m_TFPSI[nDofType].dRadius[i]         = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dRadius[i]       ;
			m_TFPSI[nDofType].dStopDist[i]       = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dStopDist[i]     ;
		}

		// DY 를 DZ 와 동일하게 저장한다.
		m_TFPSI[2].bSymOut = dlg.m_Data.NDP_Prop[nDofType].TFPSI.bSymOut;
		m_TFPSI[2].dHeightIn  = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dHeightIn;
		m_TFPSI[2].dHeightOut = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dHeightOut;

		for(int i=0; i<4; i++)
		{
			m_TFPSI[2].dStiffness[i]      = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dStiffness[i]    ;
			m_TFPSI[2].dFricCoeffSlow[i]  = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dFricCoeffSlow[i]; 
			m_TFPSI[2].dFricCoeffFast[i]  = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dFricCoeffFast[i];
			m_TFPSI[2].dRateParam[i]      = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dRateParam[i]    ;
			m_TFPSI[2].dRadius[i]         = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dRadius[i]       ;
			m_TFPSI[2].dStopDist[i]       = dlg.m_Data.NDP_Prop[nDofType].TFPSI.dStopDist[i]     ;
		}
	}
}

void CNLLinkPrtItemDlg::OnCmdDfricDlg(int nDofType)
{
	CNLLinkPrtDfricDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_nDofType  = nDofType;   // 현재 Child Dlg의 종류를 나타내는 Flag

	dlg.m_Data.NDP_Prop[nDofType].DFPSI.dAreaPressure     = m_DFPSI[nDofType].dAreaPressure;
	dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRefAxialPressure = m_DFPSI[nDofType].dRefAxialPressure;

	for(int i=0; i<2; i++)
	{
		dlg.m_Data.NDP_Prop[nDofType].DFPSI.dStiffness[i]      = m_DFPSI[nDofType].dStiffness[i]    ;
		dlg.m_Data.NDP_Prop[nDofType].DFPSI.dCoeff_uref[i]     = m_DFPSI[nDofType].dCoeff_uref[i]   ;
		dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRateParam[i]      = m_DFPSI[nDofType].dRateParam[i]    ;
		dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRadius[i]         = m_DFPSI[nDofType].dRadius[i]       ;
		dlg.m_Data.NDP_Prop[nDofType].DFPSI.dStopDist[i]       = m_DFPSI[nDofType].dStopDist[i]     ;
		dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRatioCoeff_uv[i]  = m_DFPSI[nDofType].dRatioCoeff_uv[i];
		dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRatioCoeff_up[i]  = m_DFPSI[nDofType].dRatioCoeff_up[i];
		dlg.m_Data.NDP_Prop[nDofType].DFPSI.dParamCoeff_ap[i]  = m_DFPSI[nDofType].dParamCoeff_ap[i];
		dlg.m_Data.NDP_Prop[nDofType].DFPSI.dParamCoeff_hp[i]  = m_DFPSI[nDofType].dParamCoeff_hp[i];
	}

	if(dlg.DoModal() == IDOK)
	{
		m_DFPSI[nDofType].dAreaPressure     = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dAreaPressure;
		m_DFPSI[nDofType].dRefAxialPressure = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRefAxialPressure;
		for(int i=0; i<2; i++)
		{
			m_DFPSI[nDofType].dStiffness[i]      = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dStiffness[i]    ;
			m_DFPSI[nDofType].dCoeff_uref[i]     = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dCoeff_uref[i]   ; 
			m_DFPSI[nDofType].dRateParam[i]      = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRateParam[i]    ;
			m_DFPSI[nDofType].dRadius[i]         = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRadius[i]       ;
			m_DFPSI[nDofType].dStopDist[i]       = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dStopDist[i]     ;
			m_DFPSI[nDofType].dRatioCoeff_uv[i]  = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRatioCoeff_uv[i];
			m_DFPSI[nDofType].dRatioCoeff_up[i]  = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRatioCoeff_up[i];
			m_DFPSI[nDofType].dParamCoeff_ap[i]  = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dParamCoeff_ap[i];
			m_DFPSI[nDofType].dParamCoeff_hp[i]  = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dParamCoeff_hp[i];
		}

		// DY 를 DZ 와 동일하게 저장한다.
		m_DFPSI[2].dAreaPressure     = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dAreaPressure;
		m_DFPSI[2].dRefAxialPressure = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRefAxialPressure;
		for(int i=0; i<2; i++)
		{
			m_DFPSI[2].dStiffness[i]      = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dStiffness[i]    ;
			m_DFPSI[2].dCoeff_uref[i]     = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dCoeff_uref[i]   ; 
			m_DFPSI[2].dRateParam[i]      = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRateParam[i]    ;
			m_DFPSI[2].dRadius[i]         = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRadius[i]       ;
			m_DFPSI[2].dStopDist[i]       = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dStopDist[i]     ;
			m_DFPSI[2].dRatioCoeff_uv[i]  = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRatioCoeff_uv[i];
			m_DFPSI[2].dRatioCoeff_up[i]  = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dRatioCoeff_up[i];
			m_DFPSI[2].dParamCoeff_ap[i]  = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dParamCoeff_ap[i];
			m_DFPSI[2].dParamCoeff_hp[i]  = dlg.m_Data.NDP_Prop[nDofType].DFPSI.dParamCoeff_hp[i];
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
void CNLLinkPrtItemDlg::OnChangePropType() 
{
	UpdateData(TRUE);
	m_nType = CDlgUtil::CobxGetCurSelItemData(m_wndPropType, m_wndPropType.GetCurSel());
		
	for(int i = 0; i < 6; i++)
	{
		m_bLChk[i] = FALSE;
		m_bNChk[i] = FALSE;
		m_wndStifEdit[i].SetWindowText(_T("0"));
		m_wndStifEdit2[i].SetWindowText(_T("0"));
		m_wndLChk[i].EnableWindow(TRUE);
		m_pDlgUtil->CtrlEnableDisable(this, m_DofGroup[i], FALSE);  // 각 Dof 그룹의 Item 초기화 

 //	Child Dialog에 있는 변수 초기화 
		m_VD[i].Initialize();
		m_GAP[i].Initialize();
		m_HOOK[i].Initialize();
		m_HS[i].Initialize();
		m_LRBI[i].Initialize();
		m_FPSI[i].Initialize();
		m_TFPSI[i].Initialize();
		m_DFPSI[i].Initialize();
	}

	m_wndDySprLoc.SetWindowText(_T("0.5"));
	m_wndDzSprLoc.SetWindowText(_T("0.5"));

	if(m_nAppType == 0)
	{
		for (int i = 0; i < 21; i++)
		{
			if (m_nType == 0) m_dDamp[i] = 0.0;    // When changing to element type-spring, initialize coupled damping
			if (m_nType == 1) m_dStif[i] = 0.0;    // When changing to element type-linear dashpot, initialize coupled stiffness
		}
	}
	else if (m_nAppType == 2)
	{
		// When changing to element type2-seismic devices, initialize coupled stiffness/damping
		for (int i = 0; i < 21; i++)
		{
			m_dStif[i] = 0.0;
			m_dDamp[i] = 0.0;
		}
	}

	UpdateData(FALSE);
	OnCmdNllkLChk();
	OnIehpButtonCtrl();
}

void CNLLinkPrtItemDlg::OnIehpButtonCtrl()
{
#if defined(_CIVIL_JP)
	if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_DYNAMIC_ANAL))
	{
		GetDlgItem(IDC_CMD_IEHP_BTN)->EnableWindow(FALSE);
		return;
	}
#endif

	UpdateData(TRUE);
	if(m_nAppType==2)
	{
		GetDlgItem(IDC_CMD_IEHP_BTN)->EnableWindow(FALSE);
		return;
	}

	int nSel = m_wndPropType.GetCurSel();
	if (nSel == CB_ERR) 
	{
		GetDlgItem(IDC_CMD_IEHP_BTN)->EnableWindow(FALSE);
		return;
	}

	CString str;
	m_wndPropType.GetLBText(nSel, str);

	if (str == _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop1))
		GetDlgItem(IDC_CMD_IEHP_BTN)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_CMD_IEHP_BTN)->EnableWindow(FALSE);
}

void CNLLinkPrtItemDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	CDialogMove::OnOK();
}

BOOL CNLLinkPrtItemDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	T_NLLP_D Data;
	m_pDoc->m_pAttrCtrl->GetNllp(m_OldKey, Data);   // T_NLLP_D의 key를 주고 T_NLLP_D를 받는다.
	if (m_bModify)
	{
		m_Data.nSeq = Data.nSeq;   // old data의 seq를 현재 data의 seq로 넣어준다.
		bSuccess = m_pDoc->m_pDataCtrl->ModifyNllp(Data.PropName, m_Data);
	}
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddNllp(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

void CNLLinkPrtItemDlg::OnCmdNllkShearChk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wndDySprLoc.EnableWindow(m_bLChk[1] && m_chkShearSpr.GetCheck() /*m_bShearSprLoc*/);
	m_wndDzSprLoc.EnableWindow(m_bLChk[2] && m_chkShearSpr.GetCheck() /*m_bShearSprLoc*/);
}

void CNLLinkPrtItemDlg::OnCmdApply() 
{
	if (!ApplyOrOK()) return;
	m_OldKey = m_Key;	
}

void CNLLinkPrtItemDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}


void CNLLinkPrtItemDlg::OnCmdNllkMassChk() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_wndMassChk.GetCheck();
	m_wndMassEdit.EnableWindow(nCheck == 1);
	m_wndMassRatioEdit_i.EnableWindow(nCheck == 1);
	m_wndMassRatioEdit_j.EnableWindow(nCheck == 1);
}

void CNLLinkPrtItemDlg::OnCmdNookTypeRdo()
{
	UpdateData(TRUE);

	EnableDisableSeisDvCtrls();

	if(m_nAppType == 0) 
	{
		GetDlgItem(IDC_CMD_NLNK_STIFF_TEXT)->SetWindowText(_LS(IDS_CMD_NLNK_STIFF));
		GetDlgItem(IDC_CMD_NLNK_DAMP_TEXT)->SetWindowText(_LS(IDS_CMD_NLNK_DAMPING));
		ChangeCombo(m_nAppType);
	}
	else if(m_nAppType == 1) 
	{
		GetDlgItem(IDC_CMD_NLNK_STIFF_TEXT)->SetWindowText(_LS(IDS_CMD_NLNK_EFF_STIFF));
		GetDlgItem(IDC_CMD_NLNK_DAMP_TEXT)->SetWindowText(_LS(IDS_CMD_NLNK_EFF_DAMPING));
		ChangeCombo(m_nAppType);
	}  
	else if(m_nAppType==2)
	{
		GetDlgItem(IDC_CMD_NLNK_STIFF_TEXT)->SetWindowText(_LS(IDS_CMD_NLNK_EFF_STIFF));
		GetDlgItem(IDC_CMD_NLNK_DAMP_TEXT)->SetWindowText(_LS(IDS_CMD_NLNK_DAMPING));
		OnChangePropType();
		OnSelChangeSeisTypeCmb();
	}
	else ASSERT(0);
}

void CNLLinkPrtItemDlg::MakeCombo(int nSel)
{
	m_wndPropType.ResetContent();
	if (nSel == 0)
	{
		CString strPropType[] = 
					{_LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop1),   // Spring
					 _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop2),   // Linear Dashpot
					 _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop3),   // Spring and Linear Dashpot
					 _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop4),   // Spring and Nonlinear Dashpot 1
					 _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop5)};  // Spring and Nonlinear Dashpot 2
	
		for(int i = 0; i < 3; i++)
			CDlgUtil::CobxAddItem(m_wndPropType, strPropType[i], i);
	}
	else
	{
		CString strPropType[] = 
					{_LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop1),   // Viscoelastic Damper
					 _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop2),   // Gap
					 _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop3),   // Hook
					 _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop4),   // Hysteretic System
					 _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop5),   // Lead Rubber Bearing Isolator
					 _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop6),   // Shear Spring in Friction Pendulum System Isolator
					 _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop12)}; // Triple Friction Pendulum System Isolator
		
		for(int i = 0; i < 7; i++)
			CDlgUtil::CobxAddItem(m_wndPropType, strPropType[i], i);
		if (CProduct::GetTestEnvValue(_T("DoubleFriction")) == _T("yes"))
			CDlgUtil::CobxAddItem(m_wndPropType, _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop13), 7);
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_wndPropType);
	m_wndPropType.SetCurSel(0);
}

void CNLLinkPrtItemDlg::ChangeCombo(int nSel)
{
	MakeCombo(nSel);
	OnChangePropType();
}

void CNLLinkPrtItemDlg::OnCmdIehpBtn()
{
	CString str;
	m_wndName.GetWindowText(str);

	CIehpListDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetInitSelect(str);
	dlg.DoModal();
}

void CNLLinkPrtItemDlg::OnChangeMassRatioIEdit() 
{
	double dVal_i = m_wndMassRatioEdit_i.GetEditValue();
	if(dVal_i < 0.0)   dVal_i = 0.0;
	if(dVal_i > 1.0)   dVal_i = 1.0;

	double dVal_j = 1.0 - dVal_i;
	m_wndMassRatioEdit_j.SetEditUnit(dVal_j);
}

void CNLLinkPrtItemDlg::OnChangeWeightRatioIEdit() 
{
	double dVal_i = m_wndWeightRatioEdit_i.GetEditValue();
	if(dVal_i < 0.0)   dVal_i = 0.0;
	if(dVal_i > 1.0)   dVal_i = 1.0;
	
	double dVal_j = 1.0 - dVal_i;
	m_wndWeightRatioEdit_j.SetEditUnit(dVal_j);
}

void CNLLinkPrtItemDlg::AlignCtrls()
{
	CRect rRef, rToMove;
	int nDistX, nDistY;

	// Remark (component type)
	CArray<UINT,UINT> aCtrls_Remark;
	aCtrls_Remark.Add(IDC_CMD_IEHP_REMARK_GRP);
	aCtrls_Remark.Add(IDC_CMD_IEHP_REMARK_DX_EDT);
	aCtrls_Remark.Add(IDC_CMD_IEHP_REMARK_DY_EDT);
	aCtrls_Remark.Add(IDC_CMD_IEHP_REMARK_DZ_EDT);
	aCtrls_Remark.Add(IDC_CMD_IEHP_REMARK_RX_EDT);
	aCtrls_Remark.Add(IDC_CMD_IEHP_REMARK_RY_EDT);
	aCtrls_Remark.Add(IDC_CMD_IEHP_REMARK_RZ_EDT);

	GetDlgItem(IDC_WG_CMD_STATIC10)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_IEHP_REMARK_GRP)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	m_pDlgUtil->CtrlMoveDistX(this, aCtrls_Remark, nDistX);

	//BOOL bUseSeisCtrlDevice = m_pDoc->IsSeisCtrlDeviceEnable();
	BOOL bUseSeisCtrlDevice = TRUE; // PMS 5357 : 면제진용 컨트롤 전 버전에서 사용 가능
	if(!bUseSeisCtrlDevice)
	{
		// 컨트롤 숨기고
		CArray<UINT,UINT> aCtrls_SeisDv;
		aCtrls_SeisDv.Add(IDC_CMD_NLLK_TYPE_RDO3);
		aCtrls_SeisDv.Add(IDC_WG_CMD_STATIC12);
		aCtrls_SeisDv.Add(IDC_WG_CMD_STATIC19);
		aCtrls_SeisDv.Add(IDC_CMD_IEHP_SCD_TYPE_CMB);
		aCtrls_SeisDv.Add(IDC_CMD_IEHP_SCD_PROP_CMB);
		aCtrls_SeisDv.Add(IDC_CMD_IEHP_SCD_PROP_BTN);
		aCtrls_SeisDv.Add(IDC_CMD_IEHP_SCD_TYPE_CMB);
		aCtrls_SeisDv.Add(IDC_CMD_IEHP_REMARK_GRP);
		aCtrls_SeisDv.Add(IDC_CMD_IEHP_REMARK_DX_EDT);
		aCtrls_SeisDv.Add(IDC_CMD_IEHP_REMARK_DY_EDT);
		aCtrls_SeisDv.Add(IDC_CMD_IEHP_REMARK_DZ_EDT);
		aCtrls_SeisDv.Add(IDC_CMD_IEHP_REMARK_RX_EDT);
		aCtrls_SeisDv.Add(IDC_CMD_IEHP_REMARK_RY_EDT);
		aCtrls_SeisDv.Add(IDC_CMD_IEHP_REMARK_RZ_EDT);
		aCtrls_SeisDv.Add(IDC_CMD_NLLK_DESC_BTN);
		m_pDlgUtil->CtrlShowHide(this, aCtrls_SeisDv, SW_HIDE);

		// Elem type 2만큼 땡겨올리자!
		GetDlgItem(IDC_CMD_NLLK_TYPE_RDO3)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_FRAME5)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(10);

		CArray<UINT,UINT> aCtrls_AllEtc;
		aCtrls_AllEtc.Add(IDC_CMD_FRAME5);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC6);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_WEIGHT_EDIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_WEIGHT_UNIT);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC13);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC15);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_WEIGHT_RATIO_I_EDIT);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC17);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_WEIGHT_RATIO_J_EDIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_FRAME);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_MASS_CHK);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC4);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_MASS_EDIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_MASS_UNIT);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC14);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC16);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_MASS_RATIO_I_EDIT);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC18);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_MASS_RATIO_J_EDIT);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC3);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC5);
		aCtrls_AllEtc.Add(IDC_CMD_NLNK_STIFF_TEXT);
		aCtrls_AllEtc.Add(IDC_CMD_NLNK_DAMP_TEXT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_L_DX_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_DX_STIFF_EDIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_DX_UNIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_DX_STIFF_EDIT2);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_DX_UNIT2);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_L_DY_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_DY_STIFF_EDIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_DY_UNIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_DY_STIFF_EDIT2);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_DY_UNIT2);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_L_DZ_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_DZ_STIFF_EDIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_DZ_UNIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_DZ_STIFF_EDIT2);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_DZ_UNIT2);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_L_RX_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_RX_STIFF_EDIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_RX_UNIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_RX_STIFF_EDIT2);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_RX_UNIT2);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_L_RY_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_RY_STIFF_EDIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_RY_UNIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_RY_STIFF_EDIT2);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_RY_UNIT2);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_L_RZ_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_RZ_STIFF_EDIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_RZ_UNIT);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_RZ_STIFF_EDIT2);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_STIFF_RZ_UNIT2);
		aCtrls_AllEtc.Add(IDC_CMD_COUPLED_BTN);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_DESC_BTN);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC10);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC11);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_N_DX_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_DX_BTN);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_N_DY_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_DY_BTN);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_N_DZ_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_DZ_BTN);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_N_RX_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_RX_BTN);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_N_RY_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_RY_BTN);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_N_RZ_CHK);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_RZ_BTN);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_SHEAR_CHK);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC7);
		aCtrls_AllEtc.Add(IDC_CMD_FRAME6);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC8);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_SPR_LOCATION_DY_EDIT);
		aCtrls_AllEtc.Add(IDC_WG_CMD_STATIC9);
		aCtrls_AllEtc.Add(IDC_CMD_NLLK_SPR_LOCATION_DZ_EDIT);
		aCtrls_AllEtc.Add(IDOK);
		aCtrls_AllEtc.Add(IDCANCEL);
		aCtrls_AllEtc.Add(IDC_CMD_APPLY);
		m_pDlgUtil->CtrlMoveDistY(this, aCtrls_AllEtc, nDistY);

		// 그룹박스랑 대화상자도 위로 땡겨올림!
		CRect rectDlg, rectAppGrp;
		this->GetWindowRect(rectDlg);
		rectDlg.bottom += nDistY; // 음수값임
		ScreenToClient(rectDlg);
		this->SetWindowPos(NULL, rectDlg.left, rectDlg.top, rectDlg.Width(), rectDlg.Height(), SWP_NOMOVE|SWP_NOZORDER);
		GetDlgItem(IDC_CMD_FRAME7)->GetWindowRect(rectAppGrp);
		rectAppGrp.bottom += nDistY;
		ScreenToClient(rectAppGrp);
		GetDlgItem(IDC_CMD_FRAME7)->SetWindowPos(NULL, rectAppGrp.left, rectAppGrp.top, rectAppGrp.Width(), rectAppGrp.Height(), SWP_NOMOVE|SWP_NOZORDER);

		// 텍스트도 원래대로
		GetDlgItem(IDC_CMD_NLLK_TYPE_RDO1)->SetWindowText(_LS(IDS_CMD_PJST_Element));
		GetDlgItem(IDC_CMD_NLLK_TYPE_RDO2)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Force));
	}
}

void CNLLinkPrtItemDlg::OnSelChangeSeisTypeCmb()
{
	m_wndSeisDvProp.ResetContent();

	int nType = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvType, m_wndSeisDvType.GetCurSel());  
	if(nType==0)
	{
		CArray<T_SDVI_K,T_SDVI_K> aSdviK;
		m_pDoc->m_pAttrCtrl2->GetSdviKeyList(aSdviK);

		T_SDVI_D SdviD;
		for(int i = 0; i < aSdviK.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl2->GetSdvi(aSdviK[i], SdviD);
			CDlgUtil::CobxAddItem(m_wndSeisDvProp, SdviD.strName, aSdviK[i]);
		}
	}
	else if(nType==1)
	{
		CArray<T_SDVE_K,T_SDVE_K> aSdveK;
		m_pDoc->m_pAttrCtrl2->GetSdveKeyList(aSdveK);

		T_SDVE_D SdveD;
		for(int i = 0; i < aSdveK.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl2->GetSdve(aSdveK[i], SdveD);
			CDlgUtil::CobxAddItem(m_wndSeisDvProp, SdveD.strName, aSdveK[i]);
		}
	}
	else if(nType==2)
	{
		CArray<T_SDST_K,T_SDST_K> aSdstK;
		m_pDoc->m_pAttrCtrl2->GetSdstKeyList(aSdstK);

		T_SDST_D SdstD;
		for(int i = 0; i < aSdstK.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl2->GetSdst(aSdstK[i], SdstD);
			CDlgUtil::CobxAddItem(m_wndSeisDvProp, SdstD.strName, aSdstK[i]);
		}
	}
	else if(nType==3)
	{
		CArray<T_SDHY_K,T_SDHY_K> aSdhyK;
		m_pDoc->m_pAttrCtrl2->GetSdhyKeyList(aSdhyK);

		T_SDHY_D SdhyD;
		for(int i = 0; i < aSdhyK.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl2->GetSdhy(aSdhyK[i], SdhyD);
			CDlgUtil::CobxAddItem(m_wndSeisDvProp, SdhyD.strName, aSdhyK[i]);
		}
	}
	else if(nType==4)
	{
		CArray<T_SDIS_K,T_SDIS_K> aSdisK;
		m_pDoc->m_pAttrCtrl2->GetSdisKeyList(aSdisK);

		T_SDIS_D SdisD;
		for(int i = 0; i < aSdisK.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl2->GetSdis(aSdisK[i], SdisD);
			CDlgUtil::CobxAddItem(m_wndSeisDvProp, SdisD.strName, aSdisK[i]);
		}
	}
	else if (nType==5)
	{
		CArray<T_ASGB_K, T_ASGB_K> aAsgbK;
		m_pDoc->m_pAttrCtrl->GetAsgbKeyList(aAsgbK);

		T_ASGB_D AsgbD;
		for (int i = 0; i < aAsgbK.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetAsgb(aAsgbK[i], AsgbD);
			CDlgUtil::CobxAddItem(m_wndSeisDvProp, AsgbD.PropName, aAsgbK[i]);
		}
	}
	else if (nType == 6)
	{
		CArray<T_DMGB_K, T_DMGB_K> aDmgbK;
		m_pDoc->m_pAttrCtrl->GetDmgbKeyList(aDmgbK);

		T_DMGB_D DmgbD;
		for (int i = 0; i < aDmgbK.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetDmgb(aDmgbK[i], DmgbD);
			CDlgUtil::CobxAddItem(m_wndSeisDvProp, DmgbD.PropName, aDmgbK[i]);
		}
	}
	else ASSERT(0);
	
	CDlgUtil::CobxAdjustListBoxWidth(m_wndSeisDvProp);
	if(m_bOnInit) 
		CDlgUtil::CobxSetCurSelItemData(m_wndSeisDvProp, m_Data.nSeisKey);
	else
		m_wndSeisDvProp.SetCurSel(0);
	OnSelChangeSeisPropCmb();
}

void CNLLinkPrtItemDlg::OnSelChangeSeisPropCmb()
{
	UpdateData(TRUE);
	if(m_nAppType!=2) return;

	int nType = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvType, m_wndSeisDvType.GetCurSel());
	T_KEY SdKey = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvProp, m_wndSeisDvProp.GetCurSel());
	if(nType==0)
	{
		T_SDVI_D SdviD;
		if(!m_pDoc->m_pAttrCtrl2->GetSdvi(SdKey, SdviD)) SdviD.Initialize();

		// Device에서 선택된 성분은 DOF_L 체크온,비활성화, Effective Stiffness 초기화(=0)
		for(int i=0; i<6; i++)
		{
			if(!m_bOnInit) m_wndStifEdit[i].SetEditUnit(0);
			if(SdviD.CompProp[i].bDof==FALSE && m_bOnInit) continue;

			m_bLChk[i] = SdviD.CompProp[i].bDof;
			m_wndLChk[i].EnableWindow(!SdviD.CompProp[i].bDof);
		}
	}
	else if(nType==1)
	{
		T_SDVE_D SdveD;
		m_pDoc->m_pAttrCtrl2->GetSdve(SdKey, SdveD);

		for(int i=0; i<6; i++)
		{
			if(!m_bOnInit) m_wndStifEdit[i].SetEditUnit(0);

			BOOL bChk = i == SdveD.nDir-1;
			if(bChk==FALSE && m_bOnInit) continue;
			m_bLChk[i] = bChk;   
			m_wndLChk[i].EnableWindow(!bChk);
		}
	}
	else if(nType==2)
	{
		T_SDST_D SdstD;
		m_pDoc->m_pAttrCtrl2->GetSdst(SdKey, SdstD);

		for(int i=0; i<6; i++)
		{
			if(!m_bOnInit) m_wndStifEdit[i].SetEditUnit(0);

			BOOL bChk = i == SdstD.nDir-1;
			if(bChk==FALSE && m_bOnInit) continue;
			m_bLChk[i] = bChk;   
			m_wndLChk[i].EnableWindow(!bChk);
		}
	}
	else if(nType==3)
	{
		for(int i=0; i<6; i++)
		{
			if(!m_bOnInit) m_wndStifEdit[i].SetEditUnit(0);

			BOOL bDyz = i==1 || i==2;
			if(bDyz==FALSE && m_bOnInit) continue;
			m_bLChk[i] = bDyz;   
			m_wndLChk[i].EnableWindow(!bDyz);
		}
	}
	else if(nType==4)
	{
		for(int i=0; i<6; i++)
		{
			if(!m_bOnInit) m_wndStifEdit[i].SetEditUnit(0);
			
			BOOL bDxyz = i<3;
			if(bDxyz==FALSE && m_bOnInit) continue;
			m_bLChk[i] = bDxyz;   
			m_wndLChk[i].EnableWindow(!bDxyz);
		}
	}
	else if (nType==5)
	{
		T_ASGB_D AsgbD;
		m_pDoc->m_pAttrCtrl->GetAsgb(SdKey, AsgbD);

		for (int i = 0; i < 6; i++)
		{
			if (!m_bOnInit) m_wndStifEdit[i].SetEditUnit(0);

			BOOL bDxyz = i < 3;
			if (bDxyz == FALSE && m_bOnInit) continue;
			m_bLChk[i] = bDxyz;
			m_wndLChk[i].EnableWindow(!bDxyz);
		}
		if (AsgbD.PropName != _T(""))
		{
			//m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(AsgbD);
			if (AsgbD.nPropType == 0)
			{
				m_wndStifEdit[0].SetEditUnit(AsgbD.LNR.dKv);
				m_wndStifEdit[1].SetEditUnit(AsgbD.LNR.dKh);
				m_wndStifEdit[2].SetEditUnit(AsgbD.LNR.dKh);
			}
			else if (AsgbD.nPropType == 1)
			{
				m_wndStifEdit[0].SetEditUnit(AsgbD.LRB.dKv);
				m_wndStifEdit[1].SetEditUnit(AsgbD.LRB.dHorEquiStiff);
				m_wndStifEdit[2].SetEditUnit(AsgbD.LRB.dHorEquiStiff);
			}
			else if (AsgbD.nPropType == 2)
			{
				m_wndStifEdit[0].SetEditUnit(AsgbD.HDR.dKv);
				m_wndStifEdit[1].SetEditUnit(AsgbD.HDR.dHorEquiStiff);
				m_wndStifEdit[2].SetEditUnit(AsgbD.HDR.dHorEquiStiff);
			}
			else if (AsgbD.nPropType == 3)
			{
				m_wndStifEdit[0].SetEditUnit(AsgbD.ESB.dKv);
			}
			else if (AsgbD.nPropType == 4)
			{
				m_wndStifEdit[0].SetEditUnit(AsgbD.FPS.dKv);
			}
		}
		
	}
	else if (nType == 6)
	{
		T_DMGB_D DmgbD;
		m_pDoc->m_pAttrCtrl->GetDmgb(SdKey, DmgbD);
		
		int nDof = 0;
		if (DmgbD.nPropType == 1)
		{
			nDof = DmgbD.MYD.nDof;
		}
		else if (DmgbD.nPropType == 2)
		{
			nDof = DmgbD.FD.nDof;
		}
		else if (DmgbD.nPropType == 3)
		{
			nDof = DmgbD.VFD.nDof;
		}
		else if (DmgbD.nPropType == 5)
		{
			nDof = DmgbD.VED.nDof;
		}
		
		for (int i = 0; i < 6; i++)
		{
			BOOL bShow = i == nDof;
			if (!m_bOnInit) m_wndStifEdit[i].SetEditUnit(0);
			if (bShow == FALSE && m_bOnInit) continue;
			m_bLChk[i] = bShow;
			m_wndLChk[i].EnableWindow(!bShow);
			if (DmgbD.PropName != _T(""))
			{
				if (DmgbD.nPropType == 5)
				{
					m_wndStifEdit[i].SetEditUnit(DmgbD.VED.dEffectStiff);
				}
			}
			
		}
	}
	else ASSERT(0);
	
	UpdateData(FALSE);
	OnCmdNllkLChk();

	UpdateRemarkText();
}

void CNLLinkPrtItemDlg::UpdateRemarkText()
{
	for(int nDOF=0; nDOF<6; nDOF++)
	{
		m_wndRemarkEdt[nDOF].SetWindowText(_T(""));
	}

	int nType = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvType, m_wndSeisDvType.GetCurSel());
	if(nType==0)
	{
		T_SDVI_K SdviK = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvProp, m_wndSeisDvProp.GetCurSel());
		T_SDVI_D SdviD;
		if(!m_pDoc->m_pAttrCtrl2->GetSdvi(SdviK, SdviD)) SdviD.Initialize();
		int nIdx=0;

		for(int nDOF=0; nDOF<6; nDOF++)
		{
			if(SdviD.CompProp[nDOF].bDof==TRUE) nIdx=0;
			else if(m_bLChk[nDOF]==TRUE) nIdx=1;
			else nIdx=2;

			CString strRemark[] = { _LS(IDS_CMD_NLLP_REMARK_SEISDV), _LS(IDS_CMD_NLLP_REMARK_LINEAR), _T("") };
			m_wndRemarkEdt[nDOF].SetWindowText(strRemark[nIdx]);
		}
	}
	else if(nType==1)
	{
		T_SDVE_K SdveK = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvProp, m_wndSeisDvProp.GetCurSel());
		T_SDVE_D SdveD;
		m_pDoc->m_pAttrCtrl2->GetSdve(SdveK, SdveD);
		m_wndRemarkEdt[SdveD.nDir-1].SetWindowText(_LS(IDS_CMD_NLLP_REMARK_SEISDV));
	}
	else if(nType==2)
	{
		T_SDST_K SdstK = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvProp, m_wndSeisDvProp.GetCurSel());
		T_SDST_D SdstD;
		m_pDoc->m_pAttrCtrl2->GetSdst(SdstK, SdstD);
		m_wndRemarkEdt[SdstD.nDir-1].SetWindowText(_LS(IDS_CMD_NLLP_REMARK_SEISDV));
	}
	else if(nType==3)
	{
		const int dy=1, dz=2;
		m_wndRemarkEdt[dy].SetWindowText(_LS(IDS_CMD_NLLP_REMARK_SEISDV_HOR));
		m_wndRemarkEdt[dz].SetWindowText(_LS(IDS_CMD_NLLP_REMARK_SEISDV_HOR));
	}
	else if(nType==4)
	{
		const int dx=0, dy=1, dz=2;
		m_wndRemarkEdt[dx].SetWindowText(_LS(IDS_CMD_NLLP_REMARK_SEISDV_VER));
		m_wndRemarkEdt[dy].SetWindowText(_LS(IDS_CMD_NLLP_REMARK_SEISDV_HOR));
		m_wndRemarkEdt[dz].SetWindowText(_LS(IDS_CMD_NLLP_REMARK_SEISDV_HOR));
	}
	else if (nType == 5)
	{
		const int dx = 0, dy = 1, dz = 2;
		m_wndRemarkEdt[dx].SetWindowText(_LS(IDS_CMD_NLLP_REMARK_SEISDV_VER));
		m_wndRemarkEdt[dy].SetWindowText(_LS(IDS_CMD_NLLP_REMARK_SEISDV_HOR));
		m_wndRemarkEdt[dz].SetWindowText(_LS(IDS_CMD_NLLP_REMARK_SEISDV_HOR));
	}
	else if (nType == 6)
	{
		T_DMGB_K DmgbK = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvProp, m_wndSeisDvProp.GetCurSel());
		T_DMGB_D DmgbD;
		m_pDoc->m_pAttrCtrl->GetDmgb(DmgbK, DmgbD);
		int nDof = 0;
		if (DmgbD.nPropType == 1)
		{
			nDof = DmgbD.MYD.nDof;
		}
		else if (DmgbD.nPropType == 2)
		{
			nDof = DmgbD.FD.nDof;
		}
		else if (DmgbD.nPropType == 3)
		{
			nDof = DmgbD.VFD.nDof;
		}
		else if (DmgbD.nPropType == 5)
		{
			nDof = DmgbD.VED.nDof;
		}
		m_wndRemarkEdt[nDof].SetWindowText(_LS(IDS_CMD_NLLP_REMARK_DAMPDV));
	}
}

void CNLLinkPrtItemDlg::InitCtrlTexts()
{
    GetDlgItem(IDC_CMD_NLLK_TYPE_RDO3)->SetWindowText(_LS(IDS_WG_CMD_ELEMENT_TYPE2_SEIS_CTRL_DEVICE));
    GetDlgItem(IDC_WG_CMD_STATIC19)->SetWindowText(_LS(IDS_WG_CMD_SEIS_CTRL_DEVICE_PROPERTIES));
}

void CNLLinkPrtItemDlg::OnClickSeisPropBtn()
{
	int nType = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvType, m_wndSeisDvType.GetCurSel());

	if(nType==0)
	{
		CSdviListDlg dlg;
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.DoModal();
	}
	else if(nType==1)
	{
		CSdveListDlg dlg;
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.DoModal();
	}
	else if(nType==2)
	{
		CSdstListDlg dlg;
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.DoModal();
	}
	else if(nType==3)
	{
		CSdhyListDlg dlg;
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.DoModal();
	}
	else if(nType==4)
	{
		CSdisListDlg dlg;
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.DoModal();
	}
	else if (nType==5)
	{
		CAntivibrationSupportGBListDlg dlg;
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.DoModal();
	}
	else if (nType == 6)
	{
		CDamperGBListDlg dlg;
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.DoModal();
	}
}

void CNLLinkPrtItemDlg::EnableDisableSeisDvCtrls()
{
	BOOL bSeis = m_nAppType==2;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrl_EnableOnSeis, bSeis);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrl_DisableOnSeis, !bSeis);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_ShowOnSeis, bSeis);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_HideOnSeis, !bSeis);

	GetDlgItem(IDC_CMD_NLLK_DESC_BTN)->EnableWindow(m_nAppType!=0);
}

void CNLLinkPrtItemDlg::OnCmdDescBtn()
{
	CNLLinkPrtItemDescDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

BOOL CNLLinkPrtItemDescDlg::OnInitDialog()
{
	if(!CDialogMove::OnInitDialog()) return FALSE;

	// 일문 버전인 경우, 텍스트가 짧아서 마지막 줄 감춤
	int nLang=0;
	nLang = AfxGetApp()->GetProfileInt( _T("LANGUAGE"), _T("LANGUAGE"), 0 );

	if(nLang==2)
	{
		GetDlgItem(IDC_CMD_NLLINK_PRT_DESC_ELEM2_STC6)->ShowWindow(SW_HIDE);
	}

	return TRUE;
}

void CNLLinkPrtItemDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		break;
	}
}

void CNLLinkPrtItemDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	T_KEY prevKey = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvProp, m_wndSeisDvProp.GetCurSel());

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SDVI_ADD):
		case(UR_SDVE_ADD):
		case(UR_SDST_ADD):
		case(UR_SDHY_ADD):
		case(UR_SDIS_ADD):
		case(UR_ASGB_ADD):
		case(UR_DMGB_ADD):
		case(UR_SDVI_MFD):
		case(UR_SDVE_MFD):
		case(UR_SDST_MFD):
		case(UR_SDHY_MFD):
		case(UR_SDIS_MFD):
		case(UR_ASGB_MFD):
		case(UR_DMGB_MFD):
			OnSelChangeSeisTypeCmb();
			CDlgUtil::CobxSetCurSelItemData(m_wndSeisDvProp, prevKey);
			break;
		case(UR_SDVI_DEL):
		case(UR_SDVE_DEL):
		case(UR_SDST_DEL):
		case(UR_SDHY_DEL):
		case(UR_SDIS_DEL):
		case(UR_ASGB_DEL):
		case(UR_DMGB_DEL):
			{
				OnSelChangeSeisTypeCmb();
				if(prevKey!=nKey) CDlgUtil::CobxSetCurSelItemData(m_wndSeisDvProp, prevKey);
				else m_wndSeisDvProp.SetCurSel(0);
			}
			break;
		default:
			break;
		}
	} // end of while

	if(m_wndSeisDvProp.GetCurSel()<0) m_wndSeisDvProp.SetCurSel(0);
}