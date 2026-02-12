// CmdAutoLoadCombDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombDlg.h"
#include "LoadCombCtrl.h"
#include "LoadCaseWindDirectDlg.h"
#include "CmdAutoLoadUnderGroundDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_MsgDll.h"


#include "..\wg_main\wg_mainres2.h"	

#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_dbLock\LockMgr.h"   // for CLockMgr
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\DBCodeDef.h"

#include "CmdSetOrthoLCDlg.h"

#include "..\wg_dgn\Dgn_ServiceDef.h"
#include "..\wg_base\ServiceProvider.h"
//#include "..\wg_dgn\DgnGenSeisFactor.h"
//#include "..\wg_dgn\wg_dgnAll.h"

#include "CMDLoadCalcAlphaH.h"
#include "CmdLcomPsiFactorDlg.h"
#include "CmdLcomLiveLoadTypeDlg.h"
#include "CmdAutoLoadCombImperfectionLoadDlg.h"
#include "CmdAutoLoadLiveLoadRFDlg.h"
#include "..\wg_common\TBUtilFunc.h"

#include "CmdLcomSP20PsiFactorDlg.h"
#include "CmdLcomSP20ReliabilityFactorDlg.h"
#include "CmdLcomSP20LinksDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombDlg dialog


CCmdAutoLoadCombDlg::CCmdAutoLoadCombDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdAutoLoadCombDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombDlg)
	m_nOption = -1;
	m_Factor = 0.0;
	m_nDesignType = 0;
	// Coded by Seungjun MNet:No.2393 ('20060919)
	m_dLLPsi0=0.0;
	m_dLLPsi1=0.0;
	m_dLLPsi2=0.0;
	m_dWLPsi0=0.0;
	m_dWLPsi1=0.0;
	m_dWLPsi2=0.0;
	m_dSLPsi0=0.0;
	m_dSLPsi1=0.0;
	m_dSLPsi2=0.0;  
	m_dTLPsi0=0.0;
	m_dTLPsi1=0.0;
	m_dTLPsi2=0.0;
	m_dGammaG=0.0;
	m_dGammaQ=0.0;
	m_dAlpha_H=0.0;
	//}}AFX_DATA_INIT
	m_mLcomPsiFactor.RemoveAll();
	m_enNationalAnnex = dgn::def::enNationalAnnex::Recommended;
	m_aImperfectionLoad.RemoveAll();
	m_bUseUnderLoadSeisFactor = FALSE;
	m_bUseLoadCaseZCombo = FALSE;
	m_bOver1000m = false;
}


void CCmdAutoLoadCombDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadCombDlg)
	DDX_Control(pDX, IDC_CMD_LOSS_FACTOR_TRANSFER, m_wndLossFactorTransfer);
	DDX_Control(pDX, IDC_CMD_LOSS_FACTOR_SERVICE, m_wndLossFactorService);
	DDX_Control(pDX, IDC_CMD_LOSS_FACTOR_CHK, m_wndLossFactorChk);
	DDX_Control(pDX, IDC_CMD_USE_CS_ANALYSIS_CHK, m_wndCSAnalysisChk);
	DDX_Check(pDX, IDC_ADDENVELOPE_CHK, m_bAddEnvelope);
	DDX_Control(pDX, IDC_CMD_SCALEUP_LCASE, m_SpecLcase);
	DDX_Control(pDX, IDC_CMD_SCALEUP_LIST, m_CtrlList);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_CODE, m_Code);
	DDX_Control(pDX, IDC_CMD_NATIONALANNEX_CMB, m_cmbNationalAnnex);
	DDX_Control(pDX, IDC_CMD_LOADCODE_CMB, m_cmbLoadCode);
	DDX_Control(pDX, IDC_CMD_SCALE_UP_FRAME, m_wndScaleUpFrame);
	DDX_Control(pDX, IDC_CMD_CSLC_FRAME, m_wndManipulationFrame);
	DDX_Control(pDX, IDC_CMD_DESIGN_HORI_EARTHPRESSURE_STRENGTH_CMB, m_CobxStrength);
	DDX_Control(pDX, IDC_CMD_DESIGN_HORI_EARTHPRESSURE_SERV_CMB, m_CobxServiceability);
	DDX_Control(pDX, IDC_CMD_DESIGN_HORI_EARTHPRESSURE_CHK, m_wndHorEarthChk);
	DDX_Radio(pDX, IDC_CMD_DESIGN_TYPE_STL_RDO, m_nDesignType);  
	DDX_Radio(pDX, IDC_CMD_LOADCOMB_ADD, m_nOption);
	DDX_Radio(pDX, IDC_CMD_DESIGN_CSLC_ST_RDO, m_nConstLoad);
	DDX_Text(pDX, IDC_CMD_SCALEUP_FACTOR, m_Factor);
	//add by cylee 05.04.21
	DDX_Control(pDX, IDC_CMD_DESIGN_OTHO_CHK, m_wndOthoChk);
	DDX_Radio(pDX, IDC_CMD_DESIGN_OTHO_LCB_10030_RDO, m_nOthoType);
	//add by cylee 05.06.15
	DDX_Control(pDX, IDC_CMD_DESIGN_SPECIAL_SEIS_CHK, m_wndSpecialChk);
	DDX_Control(pDX, IDC_CMD_DESIGN_UNDER_LOAD_CHK, m_wndUnderLoadChk);
	DDX_Control(pDX, IDC_CMD_DESIGN_VERT_SEIS_CHK, m_wndVertForceChk);
	// Coded by Seungjun MNet:No.2393 ('20060919)
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_LLPHI0,   m_dLLPsi0);
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_LLPHI1,   m_dLLPsi1);
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_LLPHI2,   m_dLLPsi2);
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_WLPHI0,   m_dWLPsi0);
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_WLPHI1,   m_dWLPsi1);
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_WLPHI2,   m_dWLPsi2);
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_SLPHI0,   m_dSLPsi0);
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_SLPHI1,   m_dSLPsi1);
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_SLPHI2,   m_dSLPsi2);
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_TLPHI0,   m_dTLPsi0);
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_TLPHI1,   m_dTLPsi1);
	DDX_Text(pDX,    IDC_CMD_SERV_FACTOR_TLPHI2,   m_dTLPsi2);
	DDX_Text(pDX,    IDC_CMD_LF_GAMMA_G        ,   m_dGammaG);
	DDX_Text(pDX,    IDC_CMD_LF_GAMMA_Q        ,   m_dGammaQ);
	// Coded by Seungjun ('20071204) MNet:No.3095.
	DDX_Text(pDX,    IDC_CMD_PRESS_FACTOR_ALPHA_H, m_dAlpha_H);
	DDX_Control(pDX, IDC_CMD_LLRF_GAMMA_L_EDT  ,   m_edtGammaL);
	DDX_Control(pDX, IDC_CMD_REDUN_LOAD_FACTOR_EDT, m_edtReduFactor);
	DDX_Control(pDX, IDC_CMD_LL_REDUN_LOAD_FACTOR_CHK, m_wndLLReduFactor);
	DDX_Control(pDX, IDC_CMD_REDUN_LOAD_FACTOR_CHK, m_wndReduFactor);

	// Add by GAY. PMS:4267. ('11.12.12). 중국신기준 추가 : 수직지진하중을 주하중으로 고려.
	DDX_Control(pDX, IDC_CMD_CONSIDER_VERT_SEIS_MAIN_CHK, m_wndVertSeisMainChk);
	// Add by GAY. PMS:4450. ('12.03.20). Add ACI318-11.
	// Define load level of wind load and seismic load (Strength-level, Service-level).
	// Define lateral soil pressure load factor.
	DDX_Radio(pDX, IDC_CMD_WIND_STRN_RDO, m_nWindLevel);
	DDX_Radio(pDX, IDC_CMD_SEIS_STRN_RDO, m_nSeisLevel);
	DDX_Control(pDX, IDC_CMD_LATERAL_SOIL_CMB, m_cmbLSPfact);
	DDX_Control(pDX, IDC_CMD_LATERAL_SOIL_FACTOR_CHK, m_wndLSPfactChk);
	DDX_Control(pDX, IDC_CMD_IMP_LOAD_CHK, m_chkImpLoad);
	
	//Add by tss(2019.09)
	DDX_Control(pDX, IDC_CMD_COMB_TEMP_CONDITION_CMB, m_CobxTempCond);
	DDX_Text(pDX,    IDC_CMD_LOAD_FREQ_COEF_PHI_F_EDT,   m_dPhif);
	DDX_Text(pDX,    IDC_CMD_LOAD_QPER_COEF_PHI_Q_EDT,   m_dPhiq);
	DDX_Radio(pDX, IDC_CMD_STRUCT_TYPE_A_RDO, m_nStructType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombDlg)
	ON_BN_CLICKED(IDC_CMD_LOSS_FACTOR_BTN, OnCmdLossFactorBtn)
	ON_BN_CLICKED(IDC_CMD_LOSS_FACTOR_CHK, OnCmdLossFactorChk)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_ADD, OnCmdLoadcombAdd)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_REPLACE, OnCmdLoadcombReplace)
	ON_BN_CLICKED(IDC_CMD_SCALEUP_ADD, OnCmdScaleupAdd)
	ON_BN_CLICKED(IDC_CMD_SCALEUP_DEL, OnCmdScaleupDel)
	ON_BN_CLICKED(IDC_CMD_SCALEUP_MODIFY, OnCmdScaleupModify)
	ON_BN_CLICKED(IDC_CMD_DESIGN_TYPE_STL_RDO, OnCmdDesignTypeRdo)
	ON_BN_CLICKED(IDC_CMD_DESIGN_TYPE_CON_RDO, OnCmdDesignTypeRdo)
	ON_BN_CLICKED(IDC_CMD_DESIGN_TYPE_SRC_RDO, OnCmdDesignTypeRdo)
	ON_BN_CLICKED(IDC_CMD_DESIGN_TYPE_FOT_RDO, OnCmdDesignTypeRdo)	
	ON_BN_CLICKED(IDC_CMD_DESIGN_TYPE_ALU_RDO, OnCmdDesignTypeRdo)	
	ON_BN_CLICKED(IDC_CMD_DESIGN_TYPE_CFS_RDO, OnCmdDesignTypeRdo)
	ON_NOTIFY(NM_CLICK, IDC_CMD_SCALEUP_LIST, OnClickCmdScaleupList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SCALEUP_LIST, OnItemchangedCmdScaleupList)
	//add by cylee 05.04.22
	ON_BN_CLICKED(IDC_CMD_DESIGN_OTHOLC_SET_BTN, OnCmdSetOtholcBtn)
	ON_BN_CLICKED(IDC_CMD_DESIGN_OTHO_CHK, OnCmdOthoChk)
	ON_BN_CLICKED(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO, OnCmdOthoType)
	ON_BN_CLICKED(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO, OnCmdOthoType)
	//add by cylee 05.05.13
	ON_CBN_SELCHANGE(IDC_CMD_LOADCOMB_CODE, OnCodeSelchange)
	ON_CBN_SELCHANGE(IDC_CMD_NATIONALANNEX_CMB, OnNASelchange)
	//add by cylee 05.06.15
	ON_BN_CLICKED(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK, OnCmdSpecialVertChk)
	ON_BN_CLICKED(IDC_CMD_DESIGN_UNDER_LOAD_CHK, OnCmdUnderLoadChk)
	ON_BN_CLICKED(IDC_CMD_DESIGN_VERT_SEIS_CHK, OnCmdSpecialVertChk)
    ON_BN_CLICKED(IDC_CMD_DESIGN_SEIS_FACTOR_BTN, OnCmdSeisFactorBtn)
	ON_BN_CLICKED(IDC_CMD_DESIGN_UNDER_LOAD_BTN, OnCmdUnderLoadBtn)
	ON_BN_CLICKED(IDC_CMD_PRESS_FACTOR_AUTO_ALPHA_H, OnCmdAutoAlphaHBtn)
	ON_BN_CLICKED(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_CHK, OnCmdHorEarthChk)

	ON_BN_CLICKED(IDC_CMD_LL_REDUN_LOAD_FACTOR_BTN, OnCmdLLRedunBtn)
	ON_BN_CLICKED(IDC_CMD_LL_REDUN_LOAD_FACTOR_CHK, OnCmdLLRedunChk)
	ON_BN_CLICKED(IDC_CMD_REDUN_LOAD_FACTOR_CHK, OnCmdRedunChk)

	// Add by GAY. PMS:4450. ('12.03.20). Add ACI318-11.
	// Define load level of wind load and seismic load (Strength-level, Service-level).
	// Define lateral soil pressure load factor.
	ON_BN_CLICKED(IDC_CMD_WIND_STRN_RDO, OnCmdWindLevel)
	ON_BN_CLICKED(IDC_CMD_SEIS_STRN_RDO, OnCmdSeisLevel)
	ON_BN_CLICKED(IDC_CMD_LATERAL_SOIL_FACTOR_CHK, OnCmdSoilFactorChk)

	ON_BN_CLICKED(IDC_CMD_SERV_FACTOR_BTN, OnCmdServFactorBtn)
	ON_BN_CLICKED(IDC_CMD_DESIGN_WIND_DIRECT_BTN, OnCmdWindDirectionBtn)

	ON_BN_CLICKED(IDC_CMD_IMP_LOAD_CHK, OnCmdImpLoadChk)
	ON_BN_CLICKED(IDC_CMD_IMP_LOAD_BTN, OnCmdImpLoadBtn)  
	ON_BN_CLICKED(IDC_CMD_LL_TYPE_BTN, OnCmdLiveLoadTypeBtn)
	//add by tss
	ON_CBN_SELCHANGE(IDC_CMD_COMB_TEMP_CONDITION_CMB, OnCmdTemperatureCaseSelChange)

	ON_BN_CLICKED(IDC_CMD_LINK_LOAD_CHK, OnCmdSP20LinkChk)
	ON_BN_CLICKED(IDC_CMD_LINK_LOAD_BTN, OnCmdSP20LinkBtn)
	ON_BN_CLICKED(IDC_CMD_RELIABILITY_TYPE_BTN, OnCmdSP20ReliabilityBtn)

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombDlg message handlers

BOOL CCmdAutoLoadCombDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	m_wndScaleUpFrame.InitControl(this, CCmdAutoLoadCombDlg::IDD, IDC_CMD_SCALE_UP_FRAME,TRUE,FALSE);
	m_wndScaleUpFrame.SetFoldState(FALSE); // 무조건 펼친다
	
	

	// 변수들을 초기화합니다.
	m_nDistY = 0;
	m_nOthoType = 0;
	m_nWindLevel = 0;
	m_nSeisLevel = 0;
	m_nStructType = 0;
	m_strCodeName = _T("");
	m_strOldCodeName = _T("Eurocode3:05");//
	//add by cylee 05.08.31
	if     (m_nLcomType == D_LCOMTYPE_GENERAL)  m_nDesignType = EN_STEEL;
	else if(m_nLcomType == D_LCOMTYPE_STEEL)    m_nDesignType = EN_STEEL;
	else if(m_nLcomType == D_LCOMTYPE_CONCRETE) m_nDesignType = EN_CONCRETE;
	else if(m_nLcomType == D_LCOMTYPE_SRC)      m_nDesignType = EN_SRC;
	else if(m_nLcomType == D_LCOMTYPE_FDN)      m_nDesignType = EN_FOOTING;
	else if(m_nLcomType == D_LCOMTYPE_ALUMINUM) m_nDesignType = EN_ALUMINUM;
	else if(m_nLcomType == D_LCOMTYPE_CFSTEEL)  m_nDesignType = EB_CFSTL;

	Initial_Data();  
	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif

#if defined(_CH)
	GetDlgItem(IDC_CMD_DESIGN_TYPE_FOT_RDO)->ShowWindow(SW_HIDE);
#endif

#if defined(_MGEN)
	#if /*defined(_CH) || */defined(_JP) || /*defined(_US) ||*/ defined(_RUS)
		GetDlgItem(IDC_CMD_DESIGN_TYPE_ALU_RDO)->ShowWindow(SW_HIDE);
	#endif
#elif defined(_CIVIL)
		GetDlgItem(IDC_CMD_DESIGN_TYPE_ALU_RDO)->ShowWindow(SW_HIDE);
#endif
	
	T_DCON_D DconData;
	DconData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcon(DconData);
	m_wndLossFactorTransfer.SetEditUnit(DconData.dTranLossFactor);
	m_wndLossFactorService.SetEditUnit(DconData.dServLossFactor);

	m_aCtrlScaleUp.RemoveAll();
	m_aCtrlScaleUp.Add(IDC_CMD_SCALE_UP_FRAME);
	m_aCtrlScaleUp.Add(IDC_CMD_SCALEUP_FACTOR_STR);
	m_aCtrlScaleUp.Add(IDC_CMD_SCALEUP_FACTOR);
	m_aCtrlScaleUp.Add(IDC_CMD_SCALEUP_LCASE);
	m_aCtrlScaleUp.Add(IDC_CMD_SCALEUP_LIST);
	m_aCtrlScaleUp.Add(IDC_CMD_SCALEUP_ADD);
	m_aCtrlScaleUp.Add(IDC_CMD_SCALEUP_MODIFY);
	m_aCtrlScaleUp.Add(IDC_CMD_SCALEUP_DEL);

	m_aCtrlWindComb.Add(IDC_CMD_WIND_LB_FRAME);         // Wind Load Combinations
	m_aCtrlWindComb.Add(IDC_CMD_DESIGN_WIND_DIRECT_BTN);

	m_aCtrlPS.Add(IDC_CMD_USE_CS_ANALYSIS_CHK);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_FRAME);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_CHK);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_TRANSFER);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_SERVICE);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_TRANSFER_TEXT);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_SERVICE_TEXT);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_BTN);

	m_aCtrlOkCancel.Add(IDOK);
	m_aCtrlOkCancel.Add(IDCANCEL);
	
	//Add HJS 2003.8.21
	m_aCtrlConstLoad.Add(IDC_CMD_CSLC_FRAME);
	m_aCtrlConstLoad.Add(IDC_CMD_DESIGN_CSLC_ST_RDO);
	m_aCtrlConstLoad.Add(IDC_CMD_DESIGN_CSLC_CS_RDO);
	m_aCtrlConstLoad.Add(IDC_CMD_DESIGN_CSLC_ALL_RDO);
	m_aCtrlConstLoad.Add(IDC_CMD_CSLC_DSC1_STA);
	m_aCtrlConstLoad.Add(IDC_CMD_CSLC_DSC2_STA);

	// Coded by Seungjun MNet:No.2393 ('20060919)
	m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FRAME);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_TEXT1);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_TEXT2);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_TEXT3);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_TEXT4);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_TEXT5);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_LLPHI0);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_LLPHI1);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_LLPHI2);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_WLPHI0);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_WLPHI1);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_WLPHI2);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_TEXT6);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_SLPHI0);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_SLPHI1);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_SLPHI2);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_TEXT7);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_TLPHI0);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_TLPHI1);
//   m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_TLPHI2);
	m_aCtrlEC2Phi.Add(IDC_CMD_SERV_FACTOR_BTN);

	// Coded by Seungjun ('20070726).
	m_aCtrlGamma.Add(IDC_CMD_LF_FRAME       );
	m_aCtrlGamma.Add(IDC_CMD_LF_FACTOR_TEXT1);
	m_aCtrlGamma.Add(IDC_CMD_LF_FACTOR_TEXT2);
	m_aCtrlGamma.Add(IDC_CMD_LF_GAMMA_G     );
	m_aCtrlGamma.Add(IDC_CMD_LF_GAMMA_Q     );

	// Coded by Seungjun MNet:No.2393 ('20060919)
	m_aCtrlSEISLCB.Add(IDC_CMD_SEIS_LCB_FRAME);
	m_aCtrlSEISLCB.Add(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK);
	m_aCtrlSEISLCB.Add(IDC_CMD_DESIGN_VERT_SEIS_CHK);
	m_aCtrlSEISLCB.Add(IDC_CMD_DESIGN_SEIS_FACTOR_BTN);
	m_aCtrlSEISLCB.Add(IDC_CMD_DESIGN_UNDER_LOAD_CHK);
	m_aCtrlSEISLCB.Add(IDC_CMD_DESIGN_UNDER_LOAD_BTN);
	m_aCtrlUnderLoad.Add(IDC_CMD_DESIGN_UNDER_LOAD_CHK);
	m_aCtrlUnderLoad.Add(IDC_CMD_DESIGN_UNDER_LOAD_BTN);

	// Coded by Seungjun ('20071204) MNet:No.3095.
	m_aCtrlPress.Add(IDC_CMD_PRESS_FACTOR_FRAME);
	m_aCtrlPress.Add(IDC_CMD_PRESS_FACTOR_TRANSFER_TEXT);
	m_aCtrlPress.Add(IDC_CMD_PRESS_FACTOR_ALPHA_H);
	m_aCtrlPress.Add(IDC_CMD_PRESS_FACTOR_AUTO_ALPHA_H);
	
	m_aCtrlOrthoLCB.Add(IDC_CMD_OTHO_LCB_FRAME);
	m_aCtrlOrthoLCB.Add(IDC_CMD_DESIGN_OTHO_CHK);
	m_aCtrlOrthoLCB.Add(IDC_CMD_DESIGN_OTHOLC_SET_BTN);
	m_aCtrlOrthoLCB.Add(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO);
	m_aCtrlOrthoLCB.Add(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO);

	m_aCtrlGamma_L.Add(IDC_CMD_LLRF_GRP_STC);
	m_aCtrlGamma_L.Add(IDC_CMD_LLRF_GAMMA_L_STC);
	m_aCtrlGamma_L.Add(IDC_CMD_LLRF_GAMMA_L_EDT);

	//add by tss(2019.09)
	m_aCtrlTemperature.Add(IDC_CMD_TEMPERAUTER_CONDITION_FRAME);
	m_aCtrlTemperature.Add(IDC_CMD_COMB_TEMP_CONDITION_CMB);
	m_aCtrlTemperature.Add(IDC_CMD_COMB_TEMP_CONDITION_STC);
	m_aCtrlTemperature.Add(IDC_CMD_LOAD_FREQ_COEF_PHI_F_EDT);
	m_aCtrlTemperature.Add(IDC_CMD_LOAD_FREQ_COEF_PHI_F_STC);
	m_aCtrlTemperature.Add(IDC_CMD_LOAD_QPER_COEF_PHI_Q_EDT);
	m_aCtrlTemperature.Add(IDC_CMD_LOAD_QPER_COEF_PHI_Q_STC);	
	

	m_aCtrlWindFact.RemoveAll();
	GetWindFactCtrls(m_aCtrlWindFact);
	
	m_aCtrlSeisFact.RemoveAll();
	GetSeisFactCtrls(m_aCtrlSeisFact);
	
	m_aCtrlSoilPresFact.RemoveAll();
	GetSoilPresFactCtrls(m_aCtrlSoilPresFact);

	m_aCtrlHorEarth.RemoveAll();
	m_aCtrlHorEarth.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_CHK);
	m_aCtrlHorEarth.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_GRB);
	m_aCtrlHorEarth.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_STRENGTH_STC);
	m_aCtrlHorEarth.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_STRENGTH_CMB);
	m_aCtrlHorEarth.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_SERV_STC);
	m_aCtrlHorEarth.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_SERV_CMB);

	m_aCtrlACI318_11.Add(IDC_CMD_WIND_FACTOR_FRAME);  
	m_aCtrlACI318_11.Add(IDC_CMD_WIND_STRN_RDO);
	m_aCtrlACI318_11.Add(IDC_CMD_WIND_SERV_RDO);
	m_aCtrlACI318_11.Add(IDC_CMD_SEIS_FACTOR_FRAME);
	m_aCtrlACI318_11.Add(IDC_CMD_SEIS_STRN_RDO);
	m_aCtrlACI318_11.Add(IDC_CMD_SEIS_SERV_RDO);
	m_aCtrlACI318_11.Add(IDC_CMD_LATERAL_SOIL_FACTOR_FRAME);
	m_aCtrlACI318_11.Add(IDC_CMD_LATERAL_SOIL_FACTOR_CHK);
	m_aCtrlACI318_11.Add(IDC_CMD_LATERAL_SOIL_FACTOR_STC);
	m_aCtrlACI318_11.Add(IDC_CMD_LATERAL_SOIL_CMB);

	m_aCtrlImperfectLoad.Add(IDC_CMD_IMP_LOAD_CHK);
	m_aCtrlImperfectLoad.Add(IDC_CMD_IMP_LOAD_FRAME);
	m_aCtrlImperfectLoad.Add(IDC_CMD_IMP_LOAD_BTN);

	m_aCtrlNonDissipative.Add(IDC_CMD_ND_FRAME);
	m_aCtrlNonDissipative.Add(IDC_CMD_ND_CHK);

	m_aCtrlNSCP2015.Add(IDC_CMD_REDUN_LOAD_FACTOR_CHK);
	m_aCtrlNSCP2015.Add(IDC_CMD_REDUN_LOAD_FACTOR_FRM);
	m_aCtrlNSCP2015.Add(IDC_CMD_REDUN_LOAD_FACTOR_STC);
	m_aCtrlNSCP2015.Add(IDC_CMD_REDUN_LOAD_FACTOR_EDT);
	m_aCtrlNSCP2015.Add(IDC_CMD_LL_REDUN_LOAD_FACTOR_CHK);
	m_aCtrlNSCP2015.Add(IDC_CMD_LL_REDUN_LOAD_FACTOR_FRM);
	m_aCtrlNSCP2015.Add(IDC_CMD_LL_REDUN_LOAD_FACTOR_BTN);

	m_aCtrlNTCCDE2017.Add(IDC_CMD_LL_TYPE_BTN);
	m_aCtrlNTCCDE2017.Add(IDC_CMD_LL_TYPE_FRM);
	m_aCtrlNTCCDE2017.Add(IDC_CMD_STRUCT_TYPE_FRM);
	m_aCtrlNTCCDE2017.Add(IDC_CMD_STRUCT_TYPE_A_RDO);
	m_aCtrlNTCCDE2017.Add(IDC_CMD_STRUCT_TYPE_B_RDO);

	m_aCtrlSP_20_13330_2016.Add(IDC_CMD_RELIABILITY_TYPE_FRM);
	m_aCtrlSP_20_13330_2016.Add(IDC_CMD_RELIABILITY_TYPE_BTN);
	m_aCtrlSP_20_13330_2016.Add(IDC_CMD_LINK_LOAD_CHK);
	m_aCtrlSP_20_13330_2016.Add(IDC_CMD_LINK_LOAD_FRAME);
	m_aCtrlSP_20_13330_2016.Add(IDC_CMD_LINK_LOAD_BTN);
	m_aCtrlSP_20_13330_2016.Add(IDC_CMD_REQUIRED_TYPE_FRAME);
	m_aCtrlSP_20_13330_2016.Add(IDC_CMD_LOAD_REQUIRED_MAIN_CHK);
	m_aCtrlSP_20_13330_2016.Add(IDC_CMD_LOAD_REQUIRED_SPECIAL_CHK);
	m_aCtrlSP_20_13330_2016.Add(IDC_CMD_LOAD_REQUIRED_SEISMIC_CHK);

	// Design Type의 Radio Button 활성화 작업
	GetDlgItem(IDC_CMD_DESIGN_TYPE_STL_RDO)->EnableWindow(m_nLcomType == D_LCOMTYPE_GENERAL || m_nLcomType == D_LCOMTYPE_STEEL);
	GetDlgItem(IDC_CMD_DESIGN_TYPE_CON_RDO)->EnableWindow(m_nLcomType == D_LCOMTYPE_GENERAL || m_nLcomType == D_LCOMTYPE_CONCRETE);
	GetDlgItem(IDC_CMD_DESIGN_TYPE_SRC_RDO)->EnableWindow(m_nLcomType == D_LCOMTYPE_GENERAL || m_nLcomType == D_LCOMTYPE_SRC);
	GetDlgItem(IDC_CMD_DESIGN_TYPE_FOT_RDO)->EnableWindow(m_nLcomType == D_LCOMTYPE_GENERAL || m_nLcomType == D_LCOMTYPE_FDN);
	GetDlgItem(IDC_CMD_DESIGN_TYPE_ALU_RDO)->EnableWindow(m_nLcomType == D_LCOMTYPE_GENERAL || m_nLcomType == D_LCOMTYPE_ALUMINUM);
	GetDlgItem(IDC_CMD_DESIGN_TYPE_CFS_RDO)->EnableWindow(m_nLcomType == D_LCOMTYPE_GENERAL || m_nLcomType == D_LCOMTYPE_CFSTEEL);
		
	m_aMoveCtrlID.RemoveAll();
	m_aMoveCtrlID.Add(IDC_CMD_SCALE_UP_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_SCALEUP_FACTOR_STR);
	m_aMoveCtrlID.Add(IDC_CMD_SCALEUP_FACTOR);
	m_aMoveCtrlID.Add(IDC_CMD_SCALEUP_LCASE);
	m_aMoveCtrlID.Add(IDC_CMD_SCALEUP_LIST);
	m_aMoveCtrlID.Add(IDC_CMD_SCALEUP_ADD);
	m_aMoveCtrlID.Add(IDC_CMD_SCALEUP_MODIFY);
	m_aMoveCtrlID.Add(IDC_CMD_SCALEUP_DEL);

	m_aMoveCtrlID.Add(IDC_CMD_WIND_LB_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_WIND_DIRECT_BTN);

	m_aMoveCtrlID.Add(IDC_CMD_WIND_CB_FRAME);
	m_aMoveCtrlID.Add(IDC_RADIO_SOILPRESS_ASDL);
	m_aMoveCtrlID.Add(IDC_RADIO_WHATERPRESS_ASLL);

	m_aMoveCtrlID.Add(IDC_CMD_CSLC_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_CSLC_DSC1_STA);
	m_aMoveCtrlID.Add(IDC_CMD_CSLC_DSC2_STA);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_CSLC_ST_RDO);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_CSLC_CS_RDO);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_CSLC_ALL_RDO);

	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_OTHO_CHK);
	m_aMoveCtrlID.Add(IDC_CMD_OTHO_LCB_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_OTHOLC_SET_BTN);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO);

	m_aMoveCtrlID.Add(IDC_CMD_SEIS_LCB_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_VERT_SEIS_CHK);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_SEIS_FACTOR_BTN);

	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_UNDER_LOAD_CHK);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_UNDER_LOAD_BTN);

	m_aMoveCtrlID.Add(IDC_CMD_PRESS_FACTOR_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_PRESS_FACTOR_TRANSFER_TEXT);
	m_aMoveCtrlID.Add(IDC_CMD_PRESS_FACTOR_ALPHA_H);
	m_aMoveCtrlID.Add(IDC_CMD_PRESS_FACTOR_AUTO_ALPHA_H);

	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_CHK);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_GRB);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_STRENGTH_STC);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_STRENGTH_CMB);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_SERV_STC);
	m_aMoveCtrlID.Add(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_SERV_CMB);

	m_aMoveCtrlID.Add(IDC_CMD_USE_CS_ANALYSIS_CHK);
	m_aMoveCtrlID.Add(IDC_CMD_LOSS_FACTOR_CHK);
	m_aMoveCtrlID.Add(IDC_CMD_LOSS_FACTOR_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_LOSS_FACTOR_TRANSFER_TEXT);
	m_aMoveCtrlID.Add(IDC_CMD_LOSS_FACTOR_SERVICE_TEXT);

	m_aMoveCtrlID.Add(IDC_CMD_LOSS_FACTOR_TRANSFER);
	m_aMoveCtrlID.Add(IDC_CMD_LOSS_FACTOR_SERVICE);
	m_aMoveCtrlID.Add(IDC_CMD_LOSS_FACTOR_BTN);

	m_aMoveCtrlID.Add(IDC_CMD_SERV_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_SERV_FACTOR_BTN);

	m_aMoveCtrlID.Add(IDC_CMD_LF_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_LF_FACTOR_TEXT1);
	m_aMoveCtrlID.Add(IDC_CMD_LF_GAMMA_G);
	m_aMoveCtrlID.Add(IDC_CMD_LF_FACTOR_TEXT2);
	m_aMoveCtrlID.Add(IDC_CMD_LF_GAMMA_Q);

	m_aMoveCtrlID.Add(IDC_CMD_LLRF_GRP_STC);
	m_aMoveCtrlID.Add(IDC_CMD_LLRF_GAMMA_L_STC);
	m_aMoveCtrlID.Add(IDC_CMD_LLRF_GAMMA_L_EDT);

	m_aMoveCtrlID.Add(IDC_CMD_CONSIDER_VERT_SEIS_MAIN_CHK);

	m_aMoveCtrlID.Add(IDC_CMD_WIND_FACTOR_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_WIND_STRN_RDO);
	m_aMoveCtrlID.Add(IDC_CMD_WIND_SERV_RDO);

	m_aMoveCtrlID.Add(IDC_CMD_SEIS_FACTOR_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_SEIS_STRN_RDO);
	m_aMoveCtrlID.Add(IDC_CMD_SEIS_SERV_RDO);

	m_aMoveCtrlID.Add(IDC_CMD_LATERAL_SOIL_FACTOR_CHK);

	m_aMoveCtrlID.Add(IDC_CMD_LATERAL_SOIL_FACTOR_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_LATERAL_SOIL_FACTOR_STC);
	m_aMoveCtrlID.Add(IDC_CMD_LATERAL_SOIL_CMB);

	m_aMoveCtrlID.Add(IDC_CMD_LL_TYPE_BTN);
	m_aMoveCtrlID.Add(IDC_CMD_LL_TYPE_FRM);
	m_aMoveCtrlID.Add(IDC_CMD_STRUCT_TYPE_FRM);
	m_aMoveCtrlID.Add(IDC_CMD_STRUCT_TYPE_A_RDO);
	m_aMoveCtrlID.Add(IDC_CMD_STRUCT_TYPE_B_RDO);

	m_aMoveCtrlID.Add(IDOK);
	m_aMoveCtrlID.Add(IDCANCEL);

	Set_LoadCode();

	SetInitAlignLoadCode();
	EnableCtrlNationalAnnex();


	if(m_nLcomType == D_LCOMTYPE_GENERAL)
		GetDlgItem(IDC_ADDENVELOPE_CHK)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_ADDENVELOPE_CHK)->ShowWindow(SW_HIDE);

	CArray<T_SGLD_K,T_SGLD_K> arSgldKey; 
	m_pDoc->m_pAttrCtrl->GetSgldKeyList(arSgldKey);
	m_bExistCS = arSgldKey.GetSize() > 0 ? TRUE : FALSE;
	if(m_bExistCS) 
	{
		for(int i=0; i<m_aCtrlConstLoad.GetSize(); i++)
			GetDlgItem(m_aCtrlConstLoad[i])->EnableWindow(TRUE);
		m_nConstLoad   = 2;//All
	}
	else
	{
		for(int i=0; i<m_aCtrlConstLoad.GetSize(); i++)
			GetDlgItem(m_aCtrlConstLoad[i])->EnableWindow(FALSE);
		m_nConstLoad   = 0;//static
	}

	CArray<T_STLD_K,T_STLD_K> arStldKey;
	//GetUsedStldKeyList(arStldKey);
	m_pDoc->m_pAttrCtrl2->GetUsedStldKeyList(arStldKey);  // PARKHJ-20110823 : 함수 이동!
	int iCount = arStldKey.GetSize();
	m_bExistEV = FALSE;
	for(int i=0; i<iCount; i++)
	{
		T_STLD_K Stldk = arStldKey[i];
		T_STLD_D StldD;
		StldD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetStld(Stldk, StldD))   ASSERT(0);
		CString strType =StldD.LoadCaseType;
		if(strType==_T("EV"))
		{
			m_bExistEV = TRUE;
			break;
		}
	}
	
	InitUnderLoadCtrl();

	//change by cylee 05.08.31
	// Change by ZINU.('05.12.20). GB50010-02 -> GBJ17-88, GB50017-03, GB50010-02, JGJ138-01.
	// add by Seungjun (`06.06.15) No.2079. KBC-ASD05
	// Coded by Seungjun MNet:No.2465 ('20060925) KBC-SRC05
	// Add by GAY. PMS:4035. ('09.07.10). KBC-LSD09.
	int nSeisOrthoType = GetSeisOrthoType(strCodeName);
	switch (nSeisOrthoType)
	{
	case EN_SEIS_ORTHO_BOTH:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
		}
		break;
	case EN_SEIS_ORTHO_SRSS:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			m_nOthoType = 1;
		}
		break;
	case EN_SEIS_ORTHO_100_30:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			m_nOthoType = 0;
		}
		break;
	default:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(FALSE);
			m_wndOthoChk.SetCheck(0);
		}
		break;
	}

	//add by cylee 05.05.21
	GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
	GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
	if     (m_nLcomType == D_LCOMTYPE_FDN)      GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
	else if(m_nLcomType == D_LCOMTYPE_ALUMINUM) GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
	else GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
 
	//change by cylee 05.08.31
	// add by Seungjun (`06.06.15) No.2079. KBC-ASD05
	// Coded by Seungjun MNet:No.2465 ('20060925) KBC-SRC05
	// Add by GAY. PMS:4035. ('09.07.10). KBC-LSD09.
	int nSeisLcomAddType = GetSeisAddLcomType(strCodeName);
	if (nSeisLcomAddType == EN_SEIS_LCOMADD_BOTH && m_nDesignType != EN_FOOTING)
	//if(m_nLcomType == D_LCOMTYPE_CONCRETE)
	{
		GetDlgItem(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_DESIGN_VERT_SEIS_CHK)->EnableWindow(TRUE);
		m_wndSpecialChk.SetCheck(0);
		m_wndVertForceChk.SetCheck(0);
		GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->EnableWindow(TRUE);
	}
	else if (nSeisLcomAddType == EN_SEIS_LCOMADD_SPEC && m_nDesignType != EN_FOOTING)  // MNET:3803-Seungjun-20081208
	{
		GetDlgItem(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_DESIGN_VERT_SEIS_CHK)->EnableWindow(FALSE);
		m_wndSpecialChk.SetCheck(0);
		m_wndVertForceChk.SetCheck(0);
		GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_DESIGN_VERT_SEIS_CHK)->EnableWindow(FALSE);
		m_wndSpecialChk.SetCheck(0);
		m_wndVertForceChk.SetCheck(0);
		GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->EnableWindow(FALSE);
	}		
	GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->EnableWindow(TRUE);
	
	// Coded by Seungjun ('20071204) MNet:No.3095.
	if(strCodeName == _T("KBC-USD09") || strCodeName==_T("KCI-USD12"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlPress,  TRUE );
		if(m_bExistCS || !m_bExistEV)
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlPress,  FALSE);
		else
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlPress,  TRUE );
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlPress,  FALSE);
	}

	CDlgUtil::CtrlShowHide(this, m_aCtrlHorEarth, SW_HIDE);
	m_wndHorEarthChk.SetCheck(0);
	OnCmdHorEarthChk();

	// Coded by Seungjun MNet:No.2393 ('20060919)
	CRect rRef, rMove;
	if(m_nDesignType == EN_STEEL || m_nDesignType == EN_CONCRETE || m_nDesignType == EN_FOOTING ||
		m_nDesignType == EB_CFSTL || strCodeName == _T("GB50429-2007"))  // general or steel or concrete or foundation or CFsteel
	{
		GetDlgItem(IDC_CMD_SERV_FRAME)->GetWindowRect(rMove);
		GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->GetWindowRect(rRef);    

		int nDistY = rRef.top - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlEC2Phi, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlGamma, nDistY);

		if(strCodeName == _T("Eurocode3:05"))
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi,  TRUE );
			CDlgUtil::CtrlShowHide(this, m_aCtrlGamma,   TRUE );
			CDlgUtil::CtrlShowHide(this, m_aCtrlSEISLCB, FALSE);
		}
		else if(strCodeName == _T("Eurocode2") || strCodeName == _T("Eurocode3") || strCodeName == _T("Eurocode2:04") || strCodeName == _T("Eurocode3-1-3:06"))
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi,  TRUE );
			CDlgUtil::CtrlShowHide(this, m_aCtrlGamma,   TRUE );
			CDlgUtil::CtrlShowHide(this, m_aCtrlSEISLCB, FALSE);
		}
		else if(strCodeName == _T("GB50429-2007"))
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi,  FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlGamma,   FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlSEISLCB, FALSE);
		}
		else
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi,  FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlGamma,   FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlSEISLCB, TRUE );
		}
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlGamma,  FALSE);
	}
	//////////////////////////////////////////////////////////////////////////
	
	GetDlgItem(IDC_CMD_SCALE_UP_FRAME)->GetWindowRect(rRef);   
	GetDlgItem(IDC_CMD_LLRF_GRP_STC)->GetWindowRect(rMove);

	int nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlGamma_L, nDistY);
	
	CArray<UINT,UINT> aCtrlVertSeis;   aCtrlVertSeis.Add(IDC_CMD_CONSIDER_VERT_SEIS_MAIN_CHK);
	CDlgUtil::CtrlMoveDistY(this, aCtrlVertSeis, nDistY);
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlGamma_L, strCodeName == _T("GB/T50010-10") || strCodeName == _T("GB50429-2007") ||  strCodeName == _T("GB50068-2018"));
	CDlgUtil::CtrlShowHide(this, aCtrlVertSeis, strCodeName == _T("GB/T50010-10")  || strCodeName == _T("GB50429-2007") ||  strCodeName == _T("GB50068-2018") );
	CDlgUtil::CtrlShowHide(this, m_aCtrlGamma_L, strCodeName == _T("GB51249-2017"));
	GetDlgItem(IDC_CMD_WIND_FACTOR_FRAME)->GetWindowRect(rMove);  
	GetDlgItem(IDC_CMD_WIND_LB_FRAME)->GetWindowRect(rRef); 

	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlWindFact, nDistY);
	CDlgUtil::CtrlShowHide(this, m_aCtrlWindFact, IsCodeWindFact(strCodeName));

	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSeisFact, nDistY);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSeisFact, IsCodeSeisFact(strCodeName));

	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSoilPresFact, nDistY);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSoilPresFact, IsCodeSoilPresFact(strCodeName));

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlSeisFact, IsCodeSeisFact(strCodeName));
	EnableUnderCtrl();

//   CDlgUtil::CtrlMoveDistY(this, m_aCtrlACI318_11, nDistY);
//   CDlgUtil::CtrlShowHide(this, m_aCtrlACI318_11, strCodeName == _T("ACI318-11"));
	//////////////////////////////////////////////////////////////////////////

	GetDlgItem(IDC_CMD_PRESS_FACTOR_TRANSFER_TEXT)->SetWindowText(_LS(IDS_CMD_AUTOLC_Alpha_H));

	BOOL bUseColdFormed = TRUE;
	if(CTestEnvMgr::IsTestEnvST(_T("UseColdFormed"), _T("yes"))) bUseColdFormed = TRUE;

	if(bUseColdFormed==FALSE)
	{
		CArray<UINT, UINT> aCtrlTop;
		CArray<UINT, UINT> aCtrlBot;
		aCtrlTop.Add(IDC_CMD_DESIGN_TYPE_STL_RDO);
		aCtrlTop.Add(IDC_CMD_DESIGN_TYPE_CON_RDO);
		aCtrlTop.Add(IDC_CMD_DESIGN_TYPE_SRC_RDO);
		aCtrlBot.Add(IDC_CMD_DESIGN_TYPE_FOT_RDO);
		aCtrlBot.Add(IDC_CMD_DESIGN_TYPE_ALU_RDO);
		GetDlgItem(IDC_CMD_DESIGN_TYPE_CFS_RDO)->ShowWindow(SW_HIDE);
		int nDistY = globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, aCtrlTop, nDistY);
		CDlgUtil::CtrlMoveDistY(this, aCtrlBot, -nDistY);
	}

	m_wndLLReduFactor.SetCheck(0);
	m_wndReduFactor.SetCheck(0);
	GetDlgItem(IDC_CMD_REDUN_LOAD_FACTOR_STC)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_REDUN_LOAD_FACTOR_EDT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_LL_REDUN_LOAD_FACTOR_BTN)->EnableWindow(FALSE);

	Initial_LCaseComboBox();  
	Initial_Data();  
	Initial_ListCtrlBox();
	Initial_SoilFactor_ComboBox();
// 	AlignControl(strCodeName);
// 	CtrlManager();

	SetDefaultImpfData();

	OnCodeSelchange();

	UseLoadCaseZCombo(strCodeName);

	m_wndManipulationFrame.InitControl(this, CCmdAutoLoadCombDlg::IDD, IDC_CMD_CSLC_FRAME, TRUE, FALSE);
	m_wndManipulationFrame.SetFoldState(TRUE); // 무조건 접느다.(처음에는)

	return TRUE;
}

BOOL CCmdAutoLoadCombDlg::GetLoadCombTypeByLoadCaseType(int nLoadCaseType, int& nAnalType)
{
	switch(nLoadCaseType)
	{
	case D_LOADCASE_STATIC			: nAnalType = D_LCOM_STATIC;		break;
	case D_LOADCASE_SPECTRUM		: nAnalType = D_LCOM_SPECTRUM;		break;
	case D_LOADCASE_SPECTRUM_ND		: nAnalType = D_LCOM_SPECTRUM_ND;	break;
	case D_LOADCASE_HISTORY			: nAnalType = D_LCOM_HISTORY;		break;
	case D_LOADCASE_MOVING			: nAnalType = D_LCOM_MOVING;		break;
	case D_LOADCASE_SETTLE			: nAnalType = D_LCOM_SETTLEMENT;	break;
	case D_LOADCASE_ENVELOPE		: nAnalType = D_LCOM_ENVELOPE;		break;
	case D_LOADCASE_COMB_GENERAL	: nAnalType = D_LCOM_CB_GENERAL;	break;
	case D_LOADCASE_COMB_STEEL		: nAnalType = D_LCOM_CB_STEEL;		break;
	case D_LOADCASE_COMB_CONCRETE	: nAnalType = D_LCOM_CB_CONCRETE;	break;
	case D_LOADCASE_COMB_FDN		: nAnalType = D_LCOM_CB_FOOTING;	break;
	case D_LOADCASE_COMB_SRC		: nAnalType = D_LCOM_CB_SRC;		break;
	case D_LOADCASE_COMB_ALUMINUM	: nAnalType = D_LCOM_CB_ALUMINUM;	break;
	case D_LOADCASE_COMB_STLCOMP	: nAnalType = D_LCOM_CB_STLCOMP;	break;
	case D_LOADCASE_PUSHOVER		: nAnalType = D_LCOM_PUSHOVER;		break;
	case D_LOADCASE_STAGE			: nAnalType = D_LCOM_STAGE;			break;
	case D_LOADCASE_ECCENSPEC		: nAnalType = D_LCOM_ECCENSPEC;		break;
	case D_LOADCASE_ECCENSPEC_ND	: nAnalType = D_LCOM_ECCENSPEC_ND;	break;
	case D_LOADCASE_GRIDMODEL		: nAnalType = D_LCOM_GRIDMODEL;		break;
	case D_LOADCASE_CRANE			: nAnalType = D_LCOM_CRANE;			break;      
	default:  ASSERT(0);    return FALSE;
	}
	return TRUE;
}

void CCmdAutoLoadCombDlg::SetDefaultImpfData()
{
	m_aImperfectionLoad.RemoveAll();

	T_IPCR_D IpcrD;
	T_STLD_K StldK;
	T_STLD_D StldD, DummyStldD;
	_LCOM_IMPF LComImpf;

	CString strLoadCaseNameOri;
	CString strLoadCaseName;

	CArray<T_IPCR_K, T_IPCR_K> aIpcrK;
	m_pDoc->m_pAttrCtrl2->GetIpcrKeyList(aIpcrK);
	for (int i = 0; i < aIpcrK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetIpcr(aIpcrK[i], IpcrD)) {ASSERT(0); IpcrD.Initialize();}


		if(!m_pDoc->m_pAttrCtrl->GetStld(IpcrD.LoadCaseKey, StldD)) {ASSERT(0); StldD.Initialize();}
		
		strLoadCaseName = IpcrD.strImpLCaseName;
		CArray<T_STLD_K, T_STLD_K> aStldK;
		StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strLoadCaseName);
		if(StldK)
		{
			int nAnalType;
			GetLoadCombTypeByLoadCaseType(IpcrD.AnalType, nAnalType);
			LComImpf.bElem = FALSE;
			if(IpcrD.m_nCode == 1 && IpcrD.m_CHData.m_bElem)
					LComImpf.bElem = TRUE;
			LComImpf.LCase1K = StldK;
			LComImpf.nAnalType = nAnalType;
			LComImpf.LCase2K = IpcrD.LoadCaseKey;
			//LComImpf.strLCase2GridExpr.Format(_T("%d"), D_STLD_MAXNUM + IpcrD.LoadCaseKey);
			LComImpf.nDir = IpcrD.nDir;
			m_aImperfectionLoad.Add(LComImpf);
		}
	}
}

////////////////////////////////////////////
//  #define D_LCOMTYPE_GENERAL     1
//  #define D_LCOMTYPE_STEEL       2
//  #define D_LCOMTYPE_CONCRETE    3
//  #define D_LCOMTYPE_BASEPLATE   4
////////////////////////////////////////////
void CCmdAutoLoadCombDlg::Initial_LCaseComboBox()
{
	m_SpecLcase.ResetContent();

	CArray<T_SPLC_K, T_SPLC_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(KeyList);
	int Count = KeyList.GetSize();  
	if(Count > 0)
	{
		//  Spectrum Load Case Data가 있음.
		for(int i=0; i<Count; i++)
		{
			T_SPLC_D rData;
			rData.Initialize();
			int key = KeyList.GetAt(i);

			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSplc(key,rData);
			ASSERT(bCheck);

			CString str = rData.LoadCaseName;
	    m_SpecLcase.AddString(str);
		}
//    Show_EditComboBox(TRUE);
	}
//  else Show_EditComboBox(FALSE);
}

void CCmdAutoLoadCombDlg::Show_EditComboBox(BOOL bCheck)
{
	m_SpecLcase.EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SCALEUP_LIST)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SCALEUP_FACTOR)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SCALEUP_ADD)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SCALEUP_DEL)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SCALEUP_MODIFY)->EnableWindow(bCheck);  
}

void CCmdAutoLoadCombDlg::Initial_ListCtrlBox()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_CtrlList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_CtrlList.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	CString aTitle[] = { _LS(IDS_CMD_AUTOLC_factor), _LS(IDS_WG_CMD__ADDD__Load_Case) };
//18japan	TCHAR *list[2] = {_T("Factor"),_T("Load Case")};

	CString title;
	int width[2] = {60, 80};
	for(int i = 0; i < sizeof(aTitle) / sizeof(CString); i++)
	{
		title = aTitle[i];

		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_LEFT;
		lvcolumn.pszText  = title.GetBuffer(0);
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_CtrlList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
	// Add by GAY. MQC:4788. ('12.09.14). Data2Dlg. Scale Up factors for Response Spectrum Load Cases.
	T_SURS_D SursD;
	if(m_pDoc->m_pAttrCtrl2->GetSurs(SursD))
	{
		int nSursSize = SursD.aSursBase.GetSize();
		for(int i=0; i<nSursSize; ++i)
		{
			T_SURS_BASE SursBase = SursD.aSursBase[i];
			m_ScaleData.SetAt(SursBase.SplcK, SursBase.dFactor);
			
			T_SPLC_D SplcD;
			if(!m_pDoc->m_pAttrCtrl->GetSplc(SursBase.SplcK, SplcD)) { ASSERT(0); continue; }
			Write_ListCtrlBox(i, SursBase.dFactor, SplcD.LoadCaseName);
		}
	}
}

void CCmdAutoLoadCombDlg::OnCmdTemperatureCaseSelChange()
{
	int Index = m_CobxTempCond.GetCurSel();
	CString strCaseName = _T("");
	m_CobxTempCond.GetLBText(Index, strCaseName);
	T_STLD_D rData;
	m_pDoc->m_pAttrCtrl->GetStld(strCaseName, rData);
}

void CCmdAutoLoadCombDlg::OnCmdUnderLoadChk()
{
	BOOL bUnderLoad = m_wndUnderLoadChk.GetCheck();

	GetDlgItem(IDC_CMD_DESIGN_UNDER_LOAD_BTN)->EnableWindow(bUnderLoad);
}

void CCmdAutoLoadCombDlg::OnCmdSP20ReliabilityBtn()
{
	CCmdLcomSP20ReliabilityFactorDlg  Dlg;
	Dlg.DoModal();
	//{
	//	Dlg.GetData(m_mLcomSP20RfiFactor);
	//	CLoadCombSP20Ctrl::Instance()->UpdateRfiParam(m_mLcomSP20RfiFactor);
	//	//update [m_aLcaseSP20LinkData][m_aLcaseSP20PsiFactor] loadtype
	//	if (!m_aLcaseSP20LinkData.IsEmpty())
	//		CLoadCombSP20Ctrl::Instance()->UpdateLoadCaseLink(m_aLcaseSP20LinkData);
	//	if (!m_aLcaseSP20PsiFactor.IsEmpty())
	//		CLoadCombSP20Ctrl::Instance()->UpdatePsiParam(m_aLcaseSP20PsiFactor);
	//}
}
void CCmdAutoLoadCombDlg::OnCmdSP20LinkChk()
{
	BOOL bEnable = ((CButton*)GetDlgItem(IDC_CMD_LINK_LOAD_CHK))->GetCheck() ? TRUE : FALSE;
	GetDlgItem(IDC_CMD_LINK_LOAD_BTN)->EnableWindow(bEnable);
}
void CCmdAutoLoadCombDlg::OnCmdSP20LinkBtn()
{
	CCmdLcomSP20LinksDlg Dlg;
	Dlg.DoModal();
}

void CCmdAutoLoadCombDlg::Initial_Data()
{
	if(m_nOption!=0 && m_nOption!=1) 
		m_nOption = 0;  //  0 : Add,   1 : Replace
	m_Code.ResetContent();

	if(m_nLcomType == D_LCOMTYPE_GENERAL) m_bAddEnvelope = TRUE;
	else                                  m_bAddEnvelope = FALSE; 

	Set_InitCode();
	
	m_Factor = 1.;
	if(m_SpecLcase.GetCount() != 0)  m_SpecLcase.SetCurSel(0);

	// Coded by Seungjun MNet:No.2393 ('20060919)
	Set_InitEC();  

	UpdateData(FALSE);

	InitLcomPsiFactor();

	//Add by tss(2019/09)
	Set_InitTemperatureCaseList();
	m_CobxTempCond.SetCurSel(0);
	//
	CDlgUtilEx::CtrlCheck(this, IDC_RADIO_SOILPRESS_ASDL, TRUE);
	CDlgUtilEx::CtrlCheck(this, IDC_RADIO_WHATERPRESS_ASLL, TRUE);
}

void CCmdAutoLoadCombDlg::Set_LoadCode()
{
	m_cmbLoadCode.ResetContent();

	CDlgUtil::CobxAddItem(m_cmbLoadCode, _T("KDS 41 10 15:2022"), EN_LOADCODE_KDS2021);
	CDlgUtil::CobxAddItem(m_cmbLoadCode, _T("KDS 41 10 15:2019"), EN_LOADCODE_KDS2019);
	CDlgUtil::CobxSetCurSelItemData(m_cmbLoadCode, 1);
}
void CCmdAutoLoadCombDlg::Set_InitNationalAnnex()
{
	m_cmbNationalAnnex.ResetContent();
	if(CDBLib::ActivateEC_NA23())
	{
		if(m_nDesignType==EN_STEEL || m_nDesignType==EN_CONCRETE || m_nDesignType==EN_FOOTING || m_nDesignType==EB_CFSTL)
		{
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_RECOMMENDED	), dgn::def::enNationalAnnex::Recommended);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Austria		), dgn::def::enNationalAnnex::Austria);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Belgium		), dgn::def::enNationalAnnex::Belgium);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Cyprus		), dgn::def::enNationalAnnex::Cyprus);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Czech_Republic), dgn::def::enNationalAnnex::CzechRepublic);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Denmark		), dgn::def::enNationalAnnex::Denmark);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Finland		), dgn::def::enNationalAnnex::Finland);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_France		), dgn::def::enNationalAnnex::France);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Germany		), dgn::def::enNationalAnnex::Germany);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Greece		), dgn::def::enNationalAnnex::Greece);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Ireland		), dgn::def::enNationalAnnex::Ireland);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_ITALY			), dgn::def::enNationalAnnex::Italy);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Luxembourg	), dgn::def::enNationalAnnex::Luxembourg);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Malaysia		), dgn::def::enNationalAnnex::Malaysia);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Netherlands	), dgn::def::enNationalAnnex::Netherlands);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Norway		), dgn::def::enNationalAnnex::Norway);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Poland		), dgn::def::enNationalAnnex::Poland);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Romania		), dgn::def::enNationalAnnex::Romania);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Singapore		), dgn::def::enNationalAnnex::Singapore);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Slovakia		), dgn::def::enNationalAnnex::Slovakia);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_Slovenia		), dgn::def::enNationalAnnex::Slovenia);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_SWEDEN		), dgn::def::enNationalAnnex::Sweden);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_SWEDEN2019	), dgn::def::enNationalAnnex::Sweden19);
			CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_UK			), dgn::def::enNationalAnnex::UnitedKingdom);
		}
		else return;
	}
	else
	{
		switch(m_nDesignType)
		{
			case EN_STEEL:
			{
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_RECOMMENDED), dgn::def::enNationalAnnex::Recommended);
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_SWEDEN 	 ), dgn::def::enNationalAnnex::Sweden);
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_SWEDEN2019 ), dgn::def::enNationalAnnex::Sweden19);
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_SINGAPORE  ), dgn::def::enNationalAnnex::Singapore);
			}
			break;
			case EN_CONCRETE:
			case EN_FOOTING:
			{
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_ITALY 	 ), dgn::def::enNationalAnnex::Italy);
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_RECOMMENDED), dgn::def::enNationalAnnex::Recommended);
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_SWEDEN 	 ), dgn::def::enNationalAnnex::Sweden);
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_SWEDEN2019 ), dgn::def::enNationalAnnex::Sweden19);
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_SINGAPORE  ), dgn::def::enNationalAnnex::Singapore);
			}
			break;
			case EB_CFSTL:
			{
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_RECOMMENDED), dgn::def::enNationalAnnex::Recommended);
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_SINGAPORE  ), dgn::def::enNationalAnnex::Singapore);
				CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_DGN_ITALY 	 ), dgn::def::enNationalAnnex::Italy);
			}
			break;
			default:
			break;
		}
	}
	
	CDlgUtil::CobxSetCurSelItemData(m_cmbNationalAnnex, 0);
}

// Coded by Seungjun ('20070719) MNet:No.2915.
void CCmdAutoLoadCombDlg::Set_InitCode()
{
	int nNationalAnnex = -1;
	CString strCurrentCode=_T("");
	CStringArray GenCodeName;	  

	m_aCodeEnable.RemoveAll();
	if(m_nDesignType == EN_STEEL) // Steel
	{
		CDBLib::GetStlLoadCombList(GenCodeName, &m_aCodeEnable);
	  for(int i=0; i<GenCodeName.GetSize(); i++)	
		{
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(GenCodeName[i]);
#endif
			m_Code.AddString(GenCodeName.GetAt(i));  
		}

		if(m_pDoc->m_pAttrCtrl->ExistDstl())
		{
			T_DSTL_D rData;
			rData.Initialize();
			m_pDoc->m_pAttrCtrl->GetDstl(rData);
			strCurrentCode = rData.DesignCode;
			nNationalAnnex = rData.nNationalAnnex;
		}
		else
		{
			T_PREFERENCE rPref;
			rPref.Initialize();
			m_pDoc->m_pInitCtrl->GetPreference(rPref);
			strCurrentCode = rPref.DgnCode.SteelCode;
			nNationalAnnex = rPref.DgnCode.nStlNationalAnnex;
		}
		// 하중조합용 기준이 따로 존재하거나 존재하지 않는 경우.
		if(strCurrentCode==_T("KSSC-ASD03"))        strCurrentCode=_T("KBC-ASD05");
		else if(strCurrentCode==_T("KSCE-ASD96"))   strCurrentCode=_T("AIK-ASD83");
		else if(strCurrentCode==_T("KSSC-LSD09"))   strCurrentCode=_T("KBC-LSD09"); // Add by GAY. PMS:4035. ('09.07.10).
		else if(strCurrentCode==_T("KSSC-LSD16"))   strCurrentCode=_T("KBC-LSD16");
	}
	else if(m_nDesignType == EN_CONCRETE) // Concrete
	{
		CDBLib::GetConLoadCombList(GenCodeName, &m_aCodeEnable);
	  for(int i=0; i<GenCodeName.GetSize(); i++)	
		{
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(GenCodeName[i]);
#endif
			m_Code.AddString(GenCodeName.GetAt(i));  
		}
		if(m_pDoc->m_pAttrCtrl->ExistDcon())
		{
			T_DCON_D rData;
			rData.Initialize();
			m_pDoc->m_pAttrCtrl->GetDcon(rData);
			strCurrentCode = rData.DesignCode;
			nNationalAnnex = rData.nNationalAnnex;
		}
		else
		{
			T_PREFERENCE rPref;
			rPref.Initialize();
			m_pDoc->m_pInitCtrl->GetPreference(rPref);
			strCurrentCode = rPref.DgnCode.ConcCode;
			nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
		}
		// 하중조합용 기준이 따로 존재하거나 존재하지 않는 경우.
		if(strCurrentCode==_T("KCI-USD03"))        strCurrentCode=_T("KBC-USD05");
		else if(strCurrentCode==_T("KSCE-USD96"))  strCurrentCode=_T("AIK-USD94");    
		else if(strCurrentCode==_T("KCI-USD12"))   strCurrentCode=_T("KBC-USD16");    
	}
	else if(m_nDesignType == EN_SRC) // SRC
	{
		CDBLib::GetSrcLoadCombList(GenCodeName, &m_aCodeEnable);
		for (int i = 0; i < GenCodeName.GetSize(); i++)
		{
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(GenCodeName[i]);
#endif
			m_Code.AddString(GenCodeName.GetAt(i));
		}
		if(m_pDoc->m_pAttrCtrl->ExistDsrc())
		{
			T_DSRC_D rData;
			rData.Initialize();
			m_pDoc->m_pAttrCtrl->GetDsrc(rData);
			strCurrentCode = rData.DesignCode;
		}
		else
		{
			T_PREFERENCE rPref;
			rPref.Initialize();
			m_pDoc->m_pInitCtrl->GetPreference(rPref);
			strCurrentCode = rPref.DgnCode.SRCCode;
		}
	}
	else if(m_nDesignType == EN_FOOTING) // Footing
	{
		CDBLib::GetConLoadCombList(GenCodeName, &m_aCodeEnable);	
		for(int i=0; i<GenCodeName.GetSize(); i++)
		{
		// Change by ZINU.('05.12.20). GB50010-02 -> GBJ17-88, GB50017-03, GB50010-02, JGJ138-01.
		//add by maxiao(2015-9-25)GB50017-15
			CString strCodeName = GenCodeName.GetAt(i);
			if (!(strCodeName == _T("GBJ17-88") || strCodeName == _T("GB50017-03") || strCodeName == _T("GB50017-15") || strCodeName == _T("GB50010-02") || strCodeName == _T("GB50017-17") || //add by xuezc(2018/2/8)GB50017-17
				strCodeName == _T("GB/T50010-10") || strCodeName == _T("GB50429-2007") || strCodeName == _T("JGJ138-01") || strCodeName == _T("GB50018-02") || strCodeName == _T("GB50068-2018") ||	//add by tss(2019/6/8)GB50017-17
				strCodeName == _T("GB55006-2021") || strCodeName == _T("GB/T50018-25")))
			{
#if defined(_RUS)
				CInitCtrl::ConvertEngRusDesignCode(strCodeName);
#endif
				m_Code.AddString(strCodeName);
			}
		}
		if(m_pDoc->m_pAttrCtrl->ExistDcon())
		{
			T_DCON_D rData;
			rData.Initialize();
			m_pDoc->m_pAttrCtrl->GetDcon(rData);
			strCurrentCode = rData.DesignCode;
			nNationalAnnex = rData.nNationalAnnex;
		}
		else
		{
			T_PREFERENCE rPref;
			rPref.Initialize();
			m_pDoc->m_pInitCtrl->GetPreference(rPref);
			strCurrentCode = rPref.DgnCode.ConcCode;
			nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
		}
	//add by maxiao(2015-9-25)GB50017-15
		if(strCurrentCode==_T("GBJ17-88") || strCurrentCode==_T("GB50017-03") || strCurrentCode==_T("GB50017-15") || strCurrentCode==_T("GB50010-02") || strCurrentCode==_T("GB50017-17") ||
			 strCurrentCode==_T("GB/T50010-10") || strCurrentCode == _T("GB50429-2007")|| strCurrentCode == _T("GB50068-2018")|| strCurrentCode == _T("GB55006-2021") ||
			 strCurrentCode==_T("JGJ138-01") ||  strCurrentCode==_T("KCI-USD03"))        strCurrentCode=_T("KBC-USD16");
		else if(strCurrentCode==_T("KSCE-USD96"))  strCurrentCode=_T("AIK-USD94");    
		else if(strCurrentCode==_T("KCI-USD12"))   strCurrentCode=_T("KBC-USD16");    
	}
	else if(m_nDesignType == EN_ALUMINUM) // Aluminum
	{
		CDBLib::GetAluLoadCombList(GenCodeName);
		for (int i = 0; i < GenCodeName.GetSize(); i++)
		{
			CString strCodeName = GenCodeName.GetAt(i);
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(strCodeName);
#endif
			m_Code.AddString(strCodeName);
		}
		if (GenCodeName.GetSize() > 0) strCurrentCode = GenCodeName[0];
	}
	else if(m_nDesignType == EB_CFSTL) // CFSteel
	{
		CDBLib::GetCfsLoadCombList(GenCodeName, &m_aCodeEnable);
		for(int i=0; i<GenCodeName.GetSize(); i++)	
		{
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(GenCodeName[i]);
#endif
			m_Code.AddString(GenCodeName.GetAt(i));  
		}

		if(m_pDoc->m_pAttrCtrl->ExistDcfs())
		{
			T_DCFS_D rData;
			rData.Initialize();
			m_pDoc->m_pAttrCtrl->GetDcfs(rData);
			strCurrentCode = rData.DesignCode;
			nNationalAnnex = rData.nNationalAnnex;
		}
		else
		{
			T_PREFERENCE rPref;
			rPref.Initialize();
			m_pDoc->m_pInitCtrl->GetPreference(rPref);
			strCurrentCode = rPref.DgnCode.CFSteelCode;
			nNationalAnnex = rPref.DgnCode.nCfsNationalAnnex;
		}
	}
	
#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(strCurrentCode);
#endif

	int Index = m_Code.FindString(-1, strCurrentCode);
	if(Index==-1)	Index = 0;
	m_Code.SetCurSel(Index);

	m_nOldCodeIndex = Index;

	// Coded by Seungjun ('20071204) MNet:No.3095.
	m_dAlpha_H = 1.0;
	m_edtGammaL.SetEditUnit(1.0);

	m_edtReduFactor.SetEditUnit(1.0);
	//Add by tss)2019/09)
	m_dPhif = 0.5;
	m_dPhiq = 0.4;

	Set_InitNationalAnnex();
	if(nNationalAnnex>=0)
	{
		CDlgUtil::CobxSetCurSelItemData(m_cmbNationalAnnex, nNationalAnnex);
		//m_cmbNationalAnnex.SetCurSel(nNationalAnnex);
	}

	Set_InitHorEarth();

	m_enNationalAnnex = GetCurrentNationalAnnex();
}

void CCmdAutoLoadCombDlg::Set_InitHorEarth()
{
	m_CobxStrength.ResetContent();
	m_CobxStrength.AddString(_T("1.6"));
	m_CobxStrength.AddString(_T("0.9"));
	m_CobxStrength.AddString(_T("0"));
	m_CobxStrength.SetCurSel(0);

	m_CobxServiceability.ResetContent();
	m_CobxServiceability.AddString(_T("1.0"));
	m_CobxServiceability.AddString(_T("0.6"));
	m_CobxServiceability.AddString(_T("0"));
	m_CobxServiceability.SetCurSel(0);
}

void CCmdAutoLoadCombDlg::Set_InitTemperatureCaseList()
{
	m_CobxTempCond.ResetContent();

	CArray<T_STLD_K, T_STLD_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(KeyList);

	T_STLD_D rData;
	for (int i=0; i<KeyList.GetCount(); i++)
	{
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetStld(KeyList[i], rData);
		if (rData.LoadCaseName==FIRE_LOAD_CASE_NAME)
		{
			m_CobxTempCond.AddString(FIRE_LOAD_CASE_NAME);
			break;
		}
		else
			continue;
	}
	m_CobxTempCond.SetCurSel(0);
}

void CCmdAutoLoadCombDlg::OnCmdDesignTypeRdo()
{
	UpdateData(TRUE);
	Initial_Data();
	
	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif

	InitUnderLoadCtrl();
	AlignControl(strCodeName);
	//add by cylee 05.05.19
	// Change by ZINU.('05.12.20). GB50010-02 -> GBJ17-88, GB50017-03, GB50010-02, JGJ138-01.
	 // add by Seungjun (`06.06.15) No.2079. KBC-ASD05
	// Coded by Seungjun MNet:No.2465 ('20060925) KBC-SRC05
	// Add by GAY. PMS:4035. ('09.07.10). KBC-LSD09.
	int nSeisOrthoType = GetSeisOrthoType(strCodeName);
	switch (nSeisOrthoType)
	{
	case EN_SEIS_ORTHO_BOTH:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	case EN_SEIS_ORTHO_100_30:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			m_nOthoType = 0;
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	case EN_SEIS_ORTHO_SRSS:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			m_nOthoType = 1;
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	default:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(FALSE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	}
	
	//add by cylee 05.06.15
	int nSeisLcomAddType = GetSeisAddLcomType(strCodeName);
	if (nSeisLcomAddType == EN_SEIS_LCOMADD_BOTH && m_nDesignType != EN_FOOTING)
	{
		GetDlgItem(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_DESIGN_VERT_SEIS_CHK)->EnableWindow(TRUE);
		m_wndSpecialChk.SetCheck(0);
		m_wndVertForceChk.SetCheck(0);
		GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->EnableWindow(TRUE);
	}
	else if (nSeisLcomAddType == EN_SEIS_LCOMADD_SPEC && m_nDesignType != EN_FOOTING)  // MNET:3803-Seungjun-20081208
	{
		GetDlgItem(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_DESIGN_VERT_SEIS_CHK)->EnableWindow(FALSE);
		m_wndSpecialChk.SetCheck(0);
		m_wndVertForceChk.SetCheck(0);
		GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_DESIGN_VERT_SEIS_CHK)->EnableWindow(FALSE);
		m_wndSpecialChk.SetCheck(0);
		m_wndVertForceChk.SetCheck(0);
		GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->EnableWindow(TRUE); // FoldGroupBox이므로, 늘 활성화되도록 함

	// PMS:XXXX-THBAE-20130924
	if (m_nDesignType == EN_ALUMINUM)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlGamma  , FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlSEISLCB, FALSE);
	}
	else
	{
		// Coded by Seungjun MNet:No.2393 ('20060925)
		if(strCodeName ==_T("Eurocode2") || strCodeName ==_T("Eurocode3") || strCodeName ==_T("Eurocode2:04") || strCodeName ==_T("Eurocode3:05") || strCodeName ==_T("Eurocode3-1-3:06"))
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlGamma,  TRUE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlSEISLCB, FALSE);
		}
		else
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlGamma,  FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlSEISLCB, TRUE);
		}
	}
	// Coded by Seungjun ('20071204) MNet:No.3095.
	if(strCodeName == _T("KBC-USD09") || strCodeName==_T("KCI-USD12"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlPress,  TRUE );
		if(m_bExistCS || !m_bExistEV)
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlPress,  FALSE);
		else
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlPress,  TRUE );
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlPress,  FALSE);
	}

	CDlgUtil::CtrlShowHide(this, m_aCtrlHorEarth, SW_HIDE);
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlGamma_L, strCodeName == _T("GB/T50010-10") || strCodeName == _T("GB50429-2007") ||  strCodeName == _T("GB50068-2018"));

	CArray<UINT,UINT> aCtrlVertSeis;   aCtrlVertSeis.Add(IDC_CMD_CONSIDER_VERT_SEIS_MAIN_CHK);
	CDlgUtil::CtrlShowHide(this, aCtrlVertSeis, strCodeName == _T("GB/T50010-10") || strCodeName == _T("GB50429-2007") ||  strCodeName == _T("GB50068-2018"));

	CDlgUtil::CtrlShowHide(this, m_aCtrlWindFact, IsCodeWindFact(strCodeName));
	CDlgUtil::CtrlShowHide(this, m_aCtrlSeisFact, IsCodeSeisFact(strCodeName));
	CDlgUtil::CtrlShowHide(this, m_aCtrlSoilPresFact, IsCodeSoilPresFact(strCodeName));

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlSeisFact, IsCodeSeisFact(strCodeName));
	//CDlgUtil::CtrlShowHide(this, m_aCtrlACI318_11, strCodeName==_T("ACI318-11"));
	EnableUnderCtrl();
	EnableCtrlNationalAnnex();

	UpdateData(FALSE);
}

void CCmdAutoLoadCombDlg::OnCmdLoadcombAdd() 
{
	m_nOption = 0;
	//UpdateData(FALSE);
}

void CCmdAutoLoadCombDlg::OnCmdLoadcombReplace() 
{
	m_nOption = 1;
	//UpdateData(FALSE);
}

void CCmdAutoLoadCombDlg::OnOK() 
{
	UpdateData(TRUE);

	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif
	if (strCodeName == LCOMCODE_SP_20_13330_2016)
	{
		BOOL bAdditional[3];
		bAdditional[0] = CDlgUtilEx::CtrlIsCheck(this, IDC_CMD_LOAD_REQUIRED_MAIN_CHK);
		bAdditional[1] = CDlgUtilEx::CtrlIsCheck(this, IDC_CMD_LOAD_REQUIRED_SPECIAL_CHK);
		bAdditional[2] = CDlgUtilEx::CtrlIsCheck(this, IDC_CMD_LOAD_REQUIRED_SEISMIC_CHK);
		return CLoadCombSP20Ctrl::Instance()->GenerateLcomAndAddDB(m_nOption, m_nLcomType, bAdditional);
	}

	CString csErrMsg;
	int nType;
	if      (m_nDesignType == EN_STEEL    ) nType = CLoadDgnCodeChecker::eLCBSteel;
	else if (m_nDesignType == EN_CONCRETE ) nType = CLoadDgnCodeChecker::eLCBConcrete;
	else if (m_nDesignType == EN_SRC      ) nType = CLoadDgnCodeChecker::eLCBSRC;
	else if (m_nDesignType == EN_FOOTING  ) nType = CLoadDgnCodeChecker::eLCBFooting;
	else if (m_nDesignType == EN_ALUMINUM ) nType = CLoadDgnCodeChecker::eLCBAluminum;
	else if (m_nDesignType == EB_CFSTL    ) nType = CLoadDgnCodeChecker::eLCBCFSteel;
	else { ASSERT(0); return; }
	BOOL bAllow = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(nType, strCodeName, csErrMsg);
	if (!bAllow)
	{
		AfxMessageBox(csErrMsg);
		return;
	}

	if (IsCodeStlTempFact(strCodeName) && !IsConcludeTemperatureCase())
	{
		csErrMsg = _LS(IDS_CMD_ERROR_MODS_GB51249_2017_NO_TEMPERATURE_CASE);
		AfxMessageBox(csErrMsg);
		return;
	}

	if(!ErrorCheckPS()) return;
	
	// 옵션 체크(국문에서 KBC 2016을 사용하려면 MODS 옵션이 있어야 함)
	if(CProduct::IsMovingType(D_PRODUCT_MOVING_ORG))
	{
        const BOOL bCheckMODS = [] (int nDgnType, const CString& strCode)
        {
            switch ( nDgnType )
            {
            case EN_STEEL:
                return CDBLib::IsSteelCodeMODS(strCode) ? TRUE : FALSE;
            case EN_CONCRETE:
            case EN_FOOTING:
                return (strCode==_T("KBC-USD16") || strCode==CONCODE_KDS_41_30_2018 || strCode==CONCODE_KDS_41_20_2022 || 
                    strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318_25) ? TRUE : FALSE;  // ACI318-14도  MODS Option이 있어야 함(이해주 대리 요청)
            case EN_SRC:
                return (strCode==_T("KBC-SRC16") || strCode==SRCCODE_KDS_41_SRC_2019 || strCode==SRCCODE_KDS_41_SRC_2022) ? TRUE : FALSE;            
            default:
                return FALSE;
            }
        }(m_nDesignType, strCodeName);
				
		if (bCheckMODS && !m_pDoc->AllowCommand(D_OPTN_ID_MODS_SERVICE)) 
		{
			csErrMsg = _LS(IDS_CMD_ERROR_MODS_CHEC_KBC2106);
			AfxMessageBox(csErrMsg);
			return;
		}
	}

	if(CDBLib::IsAutoLoadCombCodeName(strCodeName))
	{
		CLoadCombCtrl LoadCombCtrl;
		LoadCombCtrl.m_nDesignType = m_nDesignType;
		LoadCombCtrl.Set_LcomType(m_nLcomType);
		LoadCombCtrl.Set_ScaleUpData(m_ScaleData);

		LoadCombCtrl.m_nOrthoType	 = (m_wndOthoChk.GetCheck() ? m_nOthoType	: -1);
		LoadCombCtrl.SetSlcmSpecFlag(m_wndSpecialChk.GetCheck() ? TRUE : FALSE);
		LoadCombCtrl.SetSlcmVertFlag(m_wndVertForceChk.GetCheck() ? TRUE : FALSE);
        LoadCombCtrl.SetLcomUGFlag(m_wndUnderLoadChk.GetCheck() ? TRUE : FALSE);
        LoadCombCtrl.SetLcomUGSpecFlag(m_bUseUnderLoadSeisFactor);
		LoadCombCtrl.m_bCSAnalysis = m_wndCSAnalysisChk.GetCheck();
		LoadCombCtrl.m_bLossFactor = m_wndLossFactorChk.GetCheck();    
		LoadCombCtrl.m_dLossFactorTransfer = m_wndLossFactorTransfer.GetEditValue();
		LoadCombCtrl.m_dLossFactorService  = m_wndLossFactorService.GetEditValue();
		LoadCombCtrl.Set_AddEnvelope(m_bAddEnvelope);
		LoadCombCtrl.Set_ConstLoad(m_nConstLoad);

		// Coded by Seungjun MNet:No.2393 ('20060919)
		LoadCombCtrl.m_dEC2LLPsi0 = m_dLLPsi0;
		LoadCombCtrl.m_dEC2LLPsi1 = m_dLLPsi1;
		LoadCombCtrl.m_dEC2LLPsi2 = m_dLLPsi2;
		LoadCombCtrl.m_dEC2WLPsi0 = m_dWLPsi0;
		LoadCombCtrl.m_dEC2WLPsi1 = m_dWLPsi1;
		LoadCombCtrl.m_dEC2WLPsi2 = m_dWLPsi2;
		// Coded by Seungjun ('20070719) MNet:No.2914.
		LoadCombCtrl.m_dEC2SLPsi0 = m_dSLPsi0;
		LoadCombCtrl.m_dEC2SLPsi1 = m_dSLPsi1;
		LoadCombCtrl.m_dEC2SLPsi2 = m_dSLPsi2;
		// MNET:4820-SHJUNG-20140213
		LoadCombCtrl.m_dEC2TLPsi0 = m_dTLPsi0;
		LoadCombCtrl.m_dEC2TLPsi1 = m_dTLPsi1;
		LoadCombCtrl.m_dEC2TLPsi2 = m_dTLPsi2;

		LoadCombCtrl.SetData(m_mLcomPsiFactor);
		// Coded by Seungjun ('20070726) MNet:No.XXXX.
		LoadCombCtrl.m_dGamma_G   = m_dGammaG;
		LoadCombCtrl.m_dGamma_Q   = m_dGammaQ;
		// Coded by Seungjun ('20071210) MNet:No.3132.
		LoadCombCtrl.m_dAlpha_H   = m_dAlpha_H;
     	// Coded by MAK
    	LoadCombCtrl.m_bIS456_Cvl=FALSE; // Refers to Gen LoadCombination

		LoadCombCtrl.m_nLoadCode = CDlgUtil::CobxGetCurSelItemData(m_cmbLoadCode, m_cmbLoadCode.GetCurSel());

		//Add by tss(2019/09)
		int nSelTempCase = m_CobxTempCond.GetCurSel();
		if(nSelTempCase != -1)
			m_CobxTempCond.GetLBText(nSelTempCase, LoadCombCtrl.m_strFireCase);
		else
			LoadCombCtrl.m_strFireCase = _T("");
		LoadCombCtrl.m_dPhi_f = m_dPhif;
		LoadCombCtrl.m_dPhi_q = m_dPhiq;

		double  dFactor_Strength = 0.0;
		double  dFactor_Serviceability = 0.0;
		BOOL bHorEarthPressure = m_wndHorEarthChk.GetCheck();
		if(strCodeName!=_T("KBC-USD16")) bHorEarthPressure = FALSE;
		if(bHorEarthPressure)
		{
			CString strVal = _T("");
			m_CobxStrength.GetWindowText(strVal);
			dFactor_Strength = _ttof(strVal);
			m_CobxServiceability.GetWindowText(strVal);
			dFactor_Serviceability = _ttof(strVal);
		}
			
		LoadCombCtrl.m_dGamma_L   = m_edtGammaL.GetEditValue();
		LoadCombCtrl.m_bVertSeisAsMain = m_wndVertSeisMainChk.GetCheck();

		bool bConsider = m_wndReduFactor.GetCheck() ? true : false;
		double dRF = m_edtReduFactor.GetEditValue();
		if (bConsider && (dRF > 1.5 || dRF < 1.0))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__WARNING_LCOM_REDUNDANCY_FACTOR_));
			return;
		}
 		LoadCombCtrl.Set_RedundancyFactor(bConsider, dRF);
		LoadCombCtrl.m_bLLRedunFactor = m_wndLLReduFactor.GetCheck();
		// Add by GAY. PMS:4450. ('12.03.21).
		CString strLSPF = _T("");
		m_cmbLSPfact.GetWindowText(strLSPF);
		LoadCombCtrl.m_dLateralSoilFactor = _ttof(strLSPF);
		LoadCombCtrl.m_bLateralSoil = IsCodeSoilPresFact(strCodeName) ? m_wndLSPfactChk.GetCheck() : TRUE;
		LoadCombCtrl.m_nWindLoadLevel = m_nWindLevel;
		LoadCombCtrl.m_nSeisLoadLevel = m_nSeisLevel;
		LoadCombCtrl.m_nStructType = m_nStructType;
		//

		LoadCombCtrl.m_bSoilAsDL = CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_SOILPRESS_ASDL);
		LoadCombCtrl.m_bWatherAsLL = CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_WHATERPRESS_ASLL);

		LoadCombCtrl.m_aImperfectionLoad.RemoveAll();
		if(m_chkImpLoad.GetCheck() && m_chkImpLoad.IsWindowVisible() && m_chkImpLoad.IsWindowEnabled())
		{
			for (int i = 0; i < m_aImperfectionLoad.GetSize(); i++)
			{
				LoadCombCtrl.m_aImperfectionLoad.Add(m_aImperfectionLoad[i]);
			}
		}

		// Add by GAY. MQC:4788. ('12.09.14). Save Scale Up Factors for Response Spectrum Load Cases.
		int nSursBase = m_ScaleData.GetCount();
		if(nSursBase > 0 || m_pDoc->m_pAttrCtrl2->ExistSurs())
		{
			T_SURS_D SursD; SursD.Initialize();      
			
			T_KEY_LIST arSplcK; arSplcK.RemoveAll();
			m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcK);
			
			int nSplcSize = arSplcK.GetSize();      
			double dFactor = 0.0;
			for(int i=0; i<nSplcSize; ++i)
			{
				T_KEY SplcK = arSplcK[i];
				if(m_ScaleData.Lookup(SplcK, dFactor))
				{
					T_SURS_BASE SursBase;
					SursBase.SplcK   = SplcK;
					SursBase.dFactor = dFactor;
					SursD.aSursBase.Add(SursBase);
				}
			}
			m_pDoc->m_pDataCtrl->AddSurs(SursD);      
		}

        /// Set Non-Dissipative Flag.
        CButton* pbtnND = (CButton*)GetDlgItem(IDC_CMD_ND_CHK);        
        LoadCombCtrl.SetNonDissipative(pbtnND->GetCheck());

		if(strCodeName == LCOMCODE_NTC_CDE_2017)
		{
			CArray<T_LLLT_K, T_LLLT_K> aLlltK;
			if(m_pDoc->m_pAttrCtrl->GetLlltKeyList(aLlltK) == 0)
			{
				// _T("Three is no information in 'Live Load Type'\nClick 'OK' if you want to continue.")
				if (IDCANCEL == AfxMessageBox(_LS(IDS_WG_CMD__ADDD__NO_Lllt_Information), MB_OKCANCEL | MB_ICONWARNING))
				{
					return;
				}
			}
		}

		BOOL bOK = LoadCombCtrl.Set_DefaultLoadComb(m_nOption, strCodeName);

		m_ScaleData.RemoveAll();
		if(bOK)	CDialogMove::OnOK();
	}
	else	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Generation_code_unavailable_),MB_OK);
}

void CCmdAutoLoadCombDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

BOOL CCmdAutoLoadCombDlg::ErrorCheckPS() 
{
	BOOL bCheck = TRUE; 
	if(m_wndLossFactorChk.GetCheck())
	{
		double dLossFactorTransfer = m_wndLossFactorTransfer.GetEditValue();
		double dLossFactorService  = m_wndLossFactorService.GetEditValue();
		if(dLossFactorTransfer <= 0 || dLossFactorTransfer > 1) bCheck = FALSE;
		if(bCheck == FALSE)
		{
//      AfxMessageBox(_T("Error : Check Loss Factor for Prestress Load Cases at Transfer Stage !\n(0 < Factor <= 1)"), MB_OK); 
			AfxMessageBox(_LS(IDS_WG_CMD_ERROR_CHK_LOSS_FACTOR_TRANSFER),MB_OK); return bCheck;
			return bCheck;
		}

		if(dLossFactorService <= 0 || dLossFactorService > 1) bCheck = FALSE;
		if(bCheck == FALSE)
		{
//      AfxMessageBox(_T("Error : Check Loss Factor for Prestress Load Cases at Service Load Stage !\n(0 < Factor <= 1)"), MB_OK); 
			AfxMessageBox(_LS(IDS_WG_CMD_ERROR_CHK_LOSS_FACTOR_SERVICE),MB_OK); return bCheck;
			return bCheck;
		}

	}
	return TRUE;
}

void CCmdAutoLoadCombDlg::OnCmdScaleupAdd() 
{
	int nKey;
	UpdateData(TRUE);
	if(ErrorCheck())
	{
		//  Spectrum Load Case Data의 순번을 얻어옵니다.
		int ListNo;
		int Index = m_SpecLcase.GetCurSel();
		if(Index != -1)
		{
			CString strName=_T("");
			m_SpecLcase.GetLBText(Index,strName);
			if(CheckSpectrumName(strName,ListNo))  //  TRUE(같은 이름이 존재하지 않을 때)=Add
			{
				nKey = m_pDoc->m_pAttrCtrl->GetSplcKey(strName);
				if(nKey != 0)  m_ScaleData.SetAt(nKey,m_Factor);
				//  List Control Box에 작성합니다.
				int No = m_CtrlList.GetItemCount();
				Write_ListCtrlBox(No,m_Factor,strName);
			}
			else                        //  FALSE(같은 이름이 존재할 때)=Modify or Cancel
			{
				if(AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Existed_Spectrum_Load_Case_____nM),MB_YESNO)==IDYES)
				{
					CString strName=_T("");
					m_SpecLcase.GetLBText(Index,strName);
					nKey = m_pDoc->m_pAttrCtrl->GetSplcKey(strName);
					if(nKey != 0)  m_ScaleData.SetAt(nKey,m_Factor);
					//  List Control Box에 재작성합니다.
					OverWrite_ListCtrlBox(ListNo,m_Factor,strName);

					m_CtrlList.SetItemState(ListNo,LVIS_FOCUSED | LVIS_SELECTED,LVIS_SELECTED | LVIS_FOCUSED);
				}
			}
		}
		else
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Spectrum_Loa),MB_OK);
	}
}

BOOL CCmdAutoLoadCombDlg::CheckSpectrumName(CString str,int& Index)
{
	BOOL bCheck=TRUE;
	int Count = m_CtrlList.GetItemCount();
	for(int i=0; i<Count; i++)
	{
		CString strLcName = m_CtrlList.GetItemText(i,1);
		if(str == strLcName)  
		{
			Index = i;
			bCheck = FALSE;
		}
	}
	return bCheck;
}

void CCmdAutoLoadCombDlg::OnCmdScaleupDel() 
{
	int nItem = m_CtrlList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}
	CString strName=_T("");
	strName = m_CtrlList.GetItemText(nItem,1);
	int nKey = m_pDoc->m_pAttrCtrl->GetSplcKey(strName);
	m_ScaleData.RemoveKey(nKey);
	m_CtrlList.DeleteItem(nItem);

	int nCount = m_CtrlList.GetItemCount();
	if (nCount <= nItem) nItem -= 1;
	if (nItem >= 0) 
		m_CtrlList.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCmdAutoLoadCombDlg::OnCmdScaleupModify() 
{
	int nItem = m_CtrlList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}

	int nKey;
	UpdateData(TRUE);
	if(ErrorCheck())
	{
		//  Spectrum Load Case Data의 순번을 얻어옵니다.
		int Index = m_SpecLcase.GetCurSel();
		if(Index != -1)
		{
			CString strName=_T("");
			m_SpecLcase.GetLBText(Index,strName);
			nKey = m_pDoc->m_pAttrCtrl->GetSplcKey(strName);
			if(nKey != 0)  m_ScaleData.SetAt(nKey,m_Factor);
			//  List Control Box에 재작성합니다.
			OverWrite_ListCtrlBox(nItem,m_Factor,strName);

			m_CtrlList.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		}
		else
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_Load_Case____),MB_OK);
	}
}

BOOL CCmdAutoLoadCombDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;

	if(m_Factor < 0. || m_Factor > 10.)  bCheck = FALSE;
	if(!bCheck)	
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_Scale_Up_Factor____),MB_OK); return bCheck;
	}

	if(m_Factor < 1. && m_Factor >= 0.)  bCheck = FALSE;
	if(!bCheck)	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Warning___Check_Scale_Up_Factor__),MB_OK);

	return TRUE;
}

void CCmdAutoLoadCombDlg::Write_ListCtrlBox(int Index,double Factor,CString strLoadCase)
{
	LV_ITEM lvitem;

	CString strFactor=_T("");
	strFactor.Format(_T("%10.3f"),Factor);
	//  ListCtrl Box에 값을 입력합니다.
	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 0;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strFactor;
	m_CtrlList.InsertItem(&lvitem);

	lvitem.mask 		= LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 1;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strLoadCase;
	m_CtrlList.SetItem(&lvitem);
}

void CCmdAutoLoadCombDlg::OverWrite_ListCtrlBox(int Index,double Factor,CString strLoadCase)
{
	CString strFactor=_T("");
	strFactor.Format(_T("%10.3f"),Factor);
	//  ListCtrl Box에 값을 입력합니다.
	m_CtrlList.SetItemText(Index,0,strFactor);
	m_CtrlList.SetItemText(Index,1,strLoadCase);
}

void CCmdAutoLoadCombDlg::OnClickCmdScaleupList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CCmdAutoLoadCombDlg::OnItemchangedCmdScaleupList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	CString strFactor = m_CtrlList.GetItemText(nItem,0);
	CString strLcName = m_CtrlList.GetItemText(nItem,1);
	if(strFactor != _T("") && strLcName != _T(""))
	{
		m_Factor = _ttof(strFactor);
		int Index = m_SpecLcase.FindString(-1,strLcName);
		if(Index != -1) m_SpecLcase.SetCurSel(Index);

		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CCmdAutoLoadCombDlg::ShowCBCtrl(CString strCurCode)
{
		BOOL bShow = strCurCode == _T("GB50069-2002");
		GetDlgItem(IDC_CMD_WIND_CB_FRAME)->ShowWindow(bShow);
		GetDlgItem(IDC_RADIO_SOILPRESS_ASDL)->ShowWindow(bShow);
		GetDlgItem(IDC_RADIO_WHATERPRESS_ASLL)->ShowWindow(bShow);
}

void CCmdAutoLoadCombDlg::InitUnderLoadCtrl()
{
	EnableUnderCtrl();
	m_wndUnderLoadChk.SetCheck(0);
	OnCmdUnderLoadChk();
}

void CCmdAutoLoadCombDlg::EnableUnderCtrl()
{
	BOOL bIsUnderLoad = IsCodeUnderLoad();
	CRect rRef, rUnderChk, rMove;
	int nDistY, UnderDistY = 0;
	GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->GetWindowRect(rMove);
	GetDlgItem(IDC_CMD_DESIGN_UNDER_LOAD_CHK)->GetWindowRect(rUnderChk);
	CArray<UINT, UINT> aCtrl;
	aCtrl.Append(m_aCtrlUnderLoad);

	if (bIsUnderLoad)
	{
		GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->GetWindowRect(rRef);
		UnderDistY = rRef.bottom - rUnderChk.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, aCtrl, UnderDistY);

		GetDlgItem(IDC_CMD_DESIGN_UNDER_LOAD_BTN)->GetWindowRect(rRef);
		nDistY = rRef.bottom - rMove.bottom + globalUtils.ScaleByDPI(8);
	}
	else
	{
		UnderDistY = rMove.top - rUnderChk.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, aCtrl, UnderDistY);

		GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->GetWindowRect(rRef);
		nDistY = rRef.bottom - rMove.bottom + globalUtils.ScaleByDPI(8);

	}
	rMove.bottom += nDistY;
	if (nDistY != 0)
		GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->SetWindowPos(NULL, rMove.left, rMove.top, rMove.Width(), rMove.Height(), SWP_NOMOVE | SWP_NOZORDER);

	aCtrl.RemoveAll();
	aCtrl.Append(m_aCtrlPress);
	rRef.CopyRect(rMove);
	GetDlgItem(IDC_CMD_PRESS_FACTOR_FRAME)->GetWindowRect(rMove);
	nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

	CDlgUtil::CtrlShowHide(this, m_aCtrlUnderLoad, bIsUnderLoad);
}

void CCmdAutoLoadCombDlg::AlignControl(CString strCurCode)
{
	BOOL bLossFactorPS = FALSE;
	if(m_nDesignType == EN_CONCRETE) bLossFactorPS = TRUE;
	else bLossFactorPS = FALSE;
		
	CRect rRef;
	CRect rMove;
	CArray<UINT, UINT> *aCtrlOkCancel = &m_aCtrlOkCancel;  

	GetWindowRect(rRef);
	GetDlgItem(IDOK)->GetWindowRect(rMove);
	int nMargin = rRef.bottom - rMove.bottom;

	CDlgUtil::CtrlShowHide(this, m_aCtrlPS, FALSE);
	//////////////////////////////////////////////////////////////////////////
	CArray<UINT,UINT> aCtrlBelowGammaL;
	aCtrlBelowGammaL.Append(m_aCtrlConstLoad);
	aCtrlBelowGammaL.Append(m_aCtrlEC2Phi);
	aCtrlBelowGammaL.Append(m_aCtrlGamma);
	aCtrlBelowGammaL.Append(m_aCtrlSEISLCB);
	aCtrlBelowGammaL.Append(m_aCtrlPress);
	aCtrlBelowGammaL.Append(m_aCtrlOrthoLCB);
	aCtrlBelowGammaL.Append(m_aCtrlPS); 
	
    BOOL bShowWindComb = [strCurCode] ()
    {
        if ( strCurCode == _T("KBC-USD16") ) return TRUE;
        if ( strCurCode == _T("KBC-LSD16") ) return TRUE;
        if ( strCurCode == _T("KBC-SRC16") ) return TRUE;
        if ( strCurCode == _T("KDS 41 31 : 2019") ) return TRUE;
        if ( strCurCode == _T("KDS 41 30 : 2022") ) return TRUE;
        if ( strCurCode == _T("KDS 41 30 : 2018") ) return TRUE;
        if ( strCurCode == _T("KDS 41 20 : 2022") ) return TRUE;
        if ( strCurCode == SRCCODE_KDS_41_SRC_2019 ) return TRUE;
        if ( strCurCode == SRCCODE_KDS_41_SRC_2022 ) return TRUE;
		if ( strCurCode == LCOMCODE_THAILAND_2021 ) return TRUE;
        return FALSE;
    }();
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlWindComb, bShowWindComb);   

	GetDlgItem(IDC_CMD_LATERAL_SOIL_FACTOR_FRAME)->GetWindowRect(rMove);
	if ( strCurCode == _T("ACI318-14") )
	{
		GetDlgItem(IDC_CMD_WIND_FACTOR_FRAME)->GetWindowRect(rRef);
	}
	else if ( strCurCode == _T("ACI318-19") )
	{
		GetDlgItem(IDC_CMD_WIND_FACTOR_FRAME)->GetWindowRect(rRef);
	}
	else if ( strCurCode == _T("ACI318-25") )
	{
		GetDlgItem(IDC_CMD_WIND_FACTOR_FRAME)->GetWindowRect(rRef);
	}
	else if ( strCurCode == CONCODE_NSCP_2015)
	{
		GetDlgItem(IDC_CMD_SCALE_UP_FRAME)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_CMD_SEIS_FACTOR_FRAME)->GetWindowRect(rRef);
	}

	int nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSoilPresFact, nDistY);
		//
		if(strCurCode == _T("GB50069-2002")) 
		{
				CRect rRectCB;
				GetDlgItem(IDC_CMD_WIND_CB_FRAME)->GetWindowRect(rRectCB);
				GetDlgItem(IDC_CMD_WIND_LB_FRAME)->GetWindowRect(rRef);
				nDistY = rRef.top - rRectCB.top + globalUtils.ScaleByDPI(4);
				//
				CArray<UINT,UINT> aCtrlCB;
				aCtrlCB.Add(IDC_CMD_WIND_CB_FRAME);
				aCtrlCB.Add(IDC_RADIO_SOILPRESS_ASDL);
				aCtrlCB.Add(IDC_RADIO_WHATERPRESS_ASLL);
				CDlgUtil::CtrlMoveDistY(this, aCtrlCB, nDistY);   
		}

	GetDlgItem(IDC_CMD_CSLC_FRAME)->GetWindowRect(rMove);   
    if ( strCurCode == _T("GB/T50010-10") || strCurCode == _T("GB50429-2007") ||  strCurCode == _T("GB50068-2018") )
    {
        GetDlgItem(IDC_CMD_CONSIDER_VERT_SEIS_MAIN_CHK)->GetWindowRect(rRef);
    }
    else if ( strCurCode == _T("ACI318-11") || strCurCode == _T("ACI318-14") || strCurCode == _T("ACI318-19") || strCurCode == _T("ACI318-25") || strCurCode == CONCODE_NSCP_2015 )
    {
        GetDlgItem(IDC_CMD_LATERAL_SOIL_FACTOR_FRAME)->GetWindowRect(rRef);
    }	
    else if ( strCurCode == _T("KBC-LSD16") || strCurCode == STLCODE_KDS_41_31_2019 || strCurCode == STLCODE_KDS_41_30_10_2022 ||
        strCurCode == _T("KBC-USD16") || strCurCode == CONCODE_KDS_41_30_2018 || strCurCode == CONCODE_KDS_41_20_2022 ||
        strCurCode == _T("KBC-SRC16") || strCurCode == SRCCODE_KDS_41_SRC_2019 || strCurCode == SRCCODE_KDS_41_SRC_2022 ||
		strCurCode == LCOMCODE_THAILAND_2021)
    {
        GetDlgItem(IDC_CMD_WIND_LB_FRAME)->GetWindowRect(rRef);
    }
    else if ( strCurCode == _T("GB50069-2002") )
    {
        GetDlgItem(IDC_CMD_WIND_LB_FRAME)->GetWindowRect(rRef);
    }
    else
    {
        GetDlgItem(IDC_CMD_SCALE_UP_FRAME)->GetWindowRect(rRef);
    }
	ShowCBCtrl(strCurCode);
	nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aCtrlBelowGammaL, nDistY);   
	//////////////////////////////////////////////////////////////////////////
	if (strCurCode == LCOMCODE_SP_20_13330_2016)
	{
		GetDlgItem(IDC_CMD_SERV_FRAME)->GetWindowRect(rMove);
		GetDlgItem(IDC_CMD_SCALE_UP_FRAME)->GetWindowRect(rRef);
		int nDistY = rRef.top - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlEC2Phi, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlGamma, nDistY);
	}
	else
	{
		GetDlgItem(IDC_CMD_SERV_FRAME)->GetWindowRect(rMove);
		GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->GetWindowRect(rRef);
		int nDistY = rRef.top - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlEC2Phi, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlGamma, nDistY);
	}
	//////////////////////////////////////////////////////////////////////////
	
	// Coded by Seungjun ('20070719) MNet:No.2914.  Add snow load psi.
	if(strCurCode == _T("Eurocode3") || strCurCode == _T("Eurocode3:05") || strCurCode == _T("Eurocode3-1-3:06"))
	{
		GetDlgItem(IDC_CMD_USE_CS_ANALYSIS_CHK)->GetWindowRect(rMove);
		GetDlgItem(IDC_CMD_SERV_FRAME)->GetWindowRect(rRef);    

		int nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlPS, nDistY);      
	}
	else if(strCurCode == _T("Eurocode2") || strCurCode == _T("Eurocode2:04"))
	{
		GetDlgItem(IDC_CMD_USE_CS_ANALYSIS_CHK)->GetWindowRect(rMove);
		GetDlgItem(IDC_CMD_LF_FRAME)->GetWindowRect(rRef);    

		int nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlPS, nDistY);      
	}
	else
	{
		// Coded by Seungjun ('20071205) MNet:No.3095.
		if(strCurCode == _T("KBC-USD16") || strCurCode == _T("KDS 41 30 : 2018") || strCurCode == _T("KDS 41 20 : 2022"))
		{
			GetDlgItem(IDC_CMD_USE_CS_ANALYSIS_CHK)->GetWindowRect(rMove);
			GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->GetWindowRect(rRef);
		}
		else if(strCurCode==_T("KBC-USD09") || strCurCode==_T("KCI-USD12"))
		{
			GetDlgItem(IDC_CMD_USE_CS_ANALYSIS_CHK)->GetWindowRect(rMove);
			GetDlgItem(IDC_CMD_PRESS_FACTOR_FRAME)->GetWindowRect(rRef);   
		}
		else if (strCurCode == LCOMCODE_NTC_CDE_2017)
		{
			if (strCurCode == LCOMCODE_NTC_CDE_2017)
			{
				GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_LL_TYPE_FRM)->GetWindowRect(rMove);
				nDistY = rRef.top - rMove.top;
				//if (nDistY != 0) m_nDistY = nDistY;
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlNTCCDE2017, nDistY);
			}

			GetDlgItem(IDC_CMD_USE_CS_ANALYSIS_CHK)->GetWindowRect(rMove);
			GetDlgItem(IDC_CMD_STRUCT_TYPE_FRM)->GetWindowRect(rRef);
		}
		else
		{
			GetDlgItem(IDC_CMD_USE_CS_ANALYSIS_CHK)->GetWindowRect(rMove);
			GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->GetWindowRect(rRef);    
		}
		int nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlPS, nDistY);      
	}
			
	if(bLossFactorPS)
	{
		// Coded by Seungjun MNet:No.2393 ('20060919)
		GetDlgItem(IDC_CMD_LOSS_FACTOR_FRAME)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rMove);
		int nDistY = -rMove.top + rRef.bottom + globalUtils.ScaleByDPI(16);
		if(nDistY!=0) m_nDistY = nDistY;
		CDlgUtil::CtrlMoveDistY(this, *aCtrlOkCancel, nDistY);
		//CDlgUtil::CtrlMoveDistY(this, *aCtrlOkCancel, m_nDistY);  	
		//m_nDistY = 0;
	}
	else // Concrete 하중조합이 아닐 경우 PS Ctrl은 정위치 시키지 않고 OK, CANCEL만 이동시킨다.
	{
		int nDistY=0;
		GetDlgItem(IDOK)->GetWindowRect(rMove);
		// Coded by Seungjun ('20070719) MNet:No.2864.  Add snow load psi.
		if(strCurCode == _T("Eurocode2") || strCurCode == _T("Eurocode3") || strCurCode == _T("Eurocode2:04") || strCurCode == _T("Eurocode3:05") || strCurCode == _T("Eurocode3-1-3:06"))
		{
			GetDlgItem(IDC_CMD_LF_FRAME)->GetWindowRect(rRef);    
			nDistY = rMove.top - rRef.bottom - globalUtils.ScaleByDPI(16);
		}
		else if(strCurCode == _T("GB50429-2007"))
		{
				GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->GetWindowRect(rRef);    
				nDistY = rMove.top - rRef.top;
		}
		else
		{
			GetDlgItem(IDC_CMD_USE_CS_ANALYSIS_CHK)->GetWindowRect(rRef);    
			nDistY = rMove.top - rRef.top - globalUtils.ScaleByDPI(16);
		}
		if(nDistY!=0) m_nDistY = nDistY;
		CDlgUtil::CtrlMoveDistY(this, *aCtrlOkCancel, -1.*nDistY);
	}
	
	// Imperfection Load
	BOOL bShowImpf;
	UINT uiTargetImpf;
	GetImperfectionCtrlState(strCurCode, uiTargetImpf, bShowImpf);

	CDlgUtil::CtrlShowHide(this, m_aCtrlImperfectLoad, bShowImpf);
	if(bShowImpf)
	{
		// Imperfection Load 관련 컨트럴 위치 수정
		GetDlgItem(IDC_CMD_IMP_LOAD_FRAME)->GetWindowRect(rMove);
		GetDlgItem(uiTargetImpf)->GetWindowRect(rRef);    
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlImperfectLoad, nDistY);

		// IDOK 버튼 등의 위치 수정
		GetDlgItem(IDC_CMD_IMP_LOAD_FRAME)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rMove);
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(16);
		if(nDistY!=0) m_nDistY = nDistY;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlOkCancel, nDistY);

		OnCmdImpLoadChk();
	}

	//Add by tss
	// Temperature Load
	 if(strCurCode == _T("GB51249-2017"))
	 {
		 BOOL bShowTemperature;
		 UINT uiTargetTemper;
		 GetImperfectionCtrlState(strCurCode, uiTargetTemper, bShowTemperature);
		 Show_EditComboBox(FALSE);
		 CDlgUtil::CtrlShowHide(this, m_aCtrlImperfectLoad, FALSE);
		 CDlgUtil::CtrlShowHide(this, m_aCtrlTemperature, bShowTemperature);
		 CDlgUtil::CtrlShowHide(this, m_aCtrlGamma_L, FALSE);
		 if(bShowTemperature)
		 {
			 //
			 GetDlgItem(IDC_CMD_TEMPERAUTER_CONDITION_FRAME)->GetWindowRect(rMove);
			 GetDlgItem(uiTargetTemper)->GetWindowRect(rRef);    
			 nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
			 CDlgUtil::CtrlMoveDistY(this, m_aCtrlTemperature, nDistY);

			 // IDOK 버튼 등의 위치 수정
			 GetDlgItem(IDC_CMD_TEMPERAUTER_CONDITION_FRAME)->GetWindowRect(rRef);
			 GetDlgItem(IDOK)->GetWindowRect(rMove);
			 nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(16);
			 if(nDistY!=0) m_nDistY = nDistY;
			 CDlgUtil::CtrlMoveDistY(this, m_aCtrlOkCancel, nDistY);
		 }
	 }
	 else
		 CDlgUtil::CtrlShowHide(this, m_aCtrlTemperature, FALSE);
	
	 if (strCurCode == CONCODE_NSCP_2015 || strCurCode == STLCODE_NSCP_2015_LRFD )
	 {
		 CDlgUtil::CtrlShowHide(this, m_aCtrlNSCP2015, TRUE);

		 if (bLossFactorPS) GetDlgItem(IDC_CMD_LOSS_FACTOR_FRAME)->GetWindowRect(rRef);
		 else GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->GetWindowRect(rRef);
		 GetDlgItem(IDC_CMD_REDUN_LOAD_FACTOR_CHK)->GetWindowRect(rMove);

		 int nDistY = -rMove.top + rRef.bottom + globalUtils.ScaleByDPI(8);
		 if (nDistY != 0) m_nDistY = nDistY;
		 CDlgUtil::CtrlMoveDistY(this, m_aCtrlNSCP2015, nDistY);

		 GetDlgItem(IDC_CMD_LL_REDUN_LOAD_FACTOR_FRM)->GetWindowRect(rRef);
		 GetDlgItem(IDOK)->GetWindowRect(rMove);
		 nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(16);
		 if (nDistY != 0) m_nDistY = nDistY;
		 CDlgUtil::CtrlMoveDistY(this, m_aCtrlOkCancel, nDistY);
	 }
	 else
	 {
		 CDlgUtil::CtrlShowHide(this, m_aCtrlNSCP2015, FALSE);
	 }

	BOOL bShowND;
	UINT uiTargetND;
	GetNonDissipativeCtrlState(strCurCode, uiTargetND, bShowND);
	CDlgUtil::CtrlShowHide(this, m_aCtrlNonDissipative, bShowND);
	if(bShowND)
	{
		GetDlgItem(IDC_CMD_ND_FRAME)->GetWindowRect(rMove);
		GetDlgItem(uiTargetND)->GetWindowRect(rRef);    
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlNonDissipative, nDistY);

		// IDOK 버튼 등의 위치 수정
		GetDlgItem(IDC_CMD_ND_FRAME)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rMove);
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(16);
		if(nDistY!=0) m_nDistY = nDistY;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlOkCancel, nDistY);
	}

	if (strCurCode == LCOMCODE_SP_20_13330_2016)
	{
		GetDlgItem(IDC_CMD_SERV_FRAME)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_RELIABILITY_TYPE_FRM)->GetWindowRect(rMove);
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
		if (nDistY != 0) m_nDistY = nDistY;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlSP_20_13330_2016, nDistY);

		GetDlgItem(IDC_CMD_REQUIRED_TYPE_FRAME)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rMove);
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(16);
		if (nDistY != 0) m_nDistY = nDistY;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlOkCancel, nDistY);
	}
	else
	{
	}
	// 윈도우 사이즈를 맞춘다.
	CWnd* pWndLast = GetDlgItem(IDOK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	// Concrete일 경우만 PS를 보인다.
	CDlgUtil::CtrlShowHide(this, m_aCtrlPS, bLossFactorPS);

	Invalidate();
}

void CCmdAutoLoadCombDlg::GetImperfectionCtrlState( CString strCurCode, UINT& uiTargetImpf, BOOL &bShowImpf)
{
	CRect rMove, rRef;
	uiTargetImpf = 0;
	
	bShowImpf = FALSE;
	if(m_nDesignType == EN_STEEL)
	{
		if(strCurCode == _T("Eurocode3:05"))
		{
			bShowImpf = TRUE;
			uiTargetImpf = IDC_CMD_LF_FRAME;
		}
		else if (
			strCurCode == _T("AISC(14th)-LRFD10") || strCurCode == _T("AISC(14th)-ASD10") ||
			strCurCode == _T("AISC(15th)-LRFD16") || strCurCode == _T("AISC(15th)-ASD16") ||
			strCurCode == STLCODE_AISC_LRFD22 || strCurCode == STLCODE_AISC_ASD22)
		{
			bShowImpf = TRUE;
			uiTargetImpf = IDC_CMD_SEIS_LCB_FRAME;      
		}
		else if(strCurCode == _T("GB50017-17"))
		{
				bShowImpf = TRUE;
				uiTargetImpf = IDC_CMD_SEIS_LCB_FRAME;    
		}
		else if (strCurCode == _T("GB51249-2017"))
		{
			bShowImpf = TRUE;
			uiTargetImpf = IDC_CMD_SEIS_LCB_FRAME;  
		}
		else if (strCurCode == _T("GB55006-2021"))
		{
			bShowImpf = TRUE;
			uiTargetImpf = IDC_CMD_SEIS_LCB_FRAME;
		}
	}
	else if(m_nDesignType == EN_CONCRETE)
	{
		if(strCurCode == _T("Eurocode2:04"))
		{
			bShowImpf = TRUE;
			uiTargetImpf = IDC_CMD_LOSS_FACTOR_FRAME;
		}    
	}
	else if(m_nDesignType == EN_FOOTING)
	{
		if(strCurCode == _T("Eurocode2:04"))
		{
			bShowImpf = TRUE;
			uiTargetImpf = IDC_CMD_LF_FRAME;
		}    
	}
	else if(m_nDesignType == EB_CFSTL)
	{
		if(strCurCode == _T("Eurocode3-1-3:06"))
		{
			bShowImpf = TRUE;
			uiTargetImpf = IDC_CMD_LF_FRAME;
		}
	}

#ifndef _MGEN // MGEN이 아니면 안보여줌..
	bShowImpf = FALSE;
#endif // !_MGEN
}

void CCmdAutoLoadCombDlg::GetNonDissipativeCtrlState( CString strCurCode, UINT& uiTargetND, BOOL &bShowND)
{
	CRect rMove, rRef;
	uiTargetND = IDC_CMD_IMP_LOAD_FRAME;
	bShowND = FALSE;

	if(m_nDesignType == EN_CONCRETE)
	{
		if(strCurCode == _T("Eurocode2:04") && m_enNationalAnnex == dgn::def::enNationalAnnex::Italy)
		{
			bShowND = TRUE;
			uiTargetND = IDC_CMD_IMP_LOAD_FRAME;
		}    
	}

#ifndef _MGEN // MGEN이 아니면 안보여줌..
	bShowND = FALSE;
#endif // !_MGEN
}

void CCmdAutoLoadCombDlg::CtrlManager()
{
	CArray<T_SPLC_K, T_SPLC_K> KeyList;
	int nSplcCount = m_pDoc->m_pAttrCtrl->GetCountSplc();
	BOOL bSplcExist = FALSE;  
	if(nSplcCount > 0) bSplcExist = TRUE;
	
	m_SpecLcase.EnableWindow(bSplcExist);
	GetDlgItem(IDC_CMD_SCALEUP_LIST)->EnableWindow(bSplcExist);
	GetDlgItem(IDC_CMD_SCALEUP_FACTOR)->EnableWindow(bSplcExist);
	GetDlgItem(IDC_CMD_SCALEUP_ADD)->EnableWindow(bSplcExist);
	GetDlgItem(IDC_CMD_SCALEUP_DEL)->EnableWindow(bSplcExist);
	GetDlgItem(IDC_CMD_SCALEUP_MODIFY)->EnableWindow(bSplcExist);  

	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();
	BOOL bPsExist = FALSE;
	// Add by GAY. PMS:4450. ('12.03.21).  
	BOOL bSPExist = FALSE;
	
	if(iCount!=0)
	{
		for(int i = 0; i < iCount; i++)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
			ASSERT(bCheck);            
			if(rData.LoadCaseType==_T("PS")) bPsExist = TRUE;  // Prestress 
			else if(rData.LoadCaseType==_T("EP") || rData.LoadCaseType==_T("EH") || rData.LoadCaseType==_T("WP")) bSPExist = TRUE;      
		}    
	}
	
	BOOL bPostMode = FALSE;
	if(m_pDoc->IsPostMode()) bPostMode = TRUE;
	// CSAnalysis와 LossFactorChk는 Prestress Load Case가 있는 경우만 사용(활성화)
	m_wndCSAnalysisChk.EnableWindow(bPsExist && !bPostMode);   // CSAnalysis는 PostMode에서는 비활성화
	m_wndLossFactorChk.EnableWindow(bPsExist);           

	BOOL bLossFactorChk = m_wndLossFactorChk.GetCheck();
	m_wndLossFactorTransfer.EnableWindow(bPsExist && bLossFactorChk);
	m_wndLossFactorService.EnableWindow(bPsExist && bLossFactorChk);    
	GetDlgItem(IDC_CMD_LOSS_FACTOR_TRANSFER_TEXT)->EnableWindow(bPsExist && bLossFactorChk);
	GetDlgItem(IDC_CMD_LOSS_FACTOR_SERVICE_TEXT)->EnableWindow(bPsExist && bLossFactorChk);
	GetDlgItem(IDC_CMD_LOSS_FACTOR_TRANSFER_TEXT)->EnableWindow(bPsExist && bLossFactorChk);
	GetDlgItem(IDC_CMD_LOSS_FACTOR_SERVICE_TEXT)->EnableWindow(bPsExist && bLossFactorChk);
	GetDlgItem(IDC_CMD_LOSS_FACTOR_BTN)->EnableWindow(bPsExist && bLossFactorChk);

	// Add by GAY. PMS:4550. ('12.03.21).  

	BOOL bSoilFactor = m_wndLSPfactChk.GetCheck();
	GetDlgItem(IDC_CMD_LATERAL_SOIL_FACTOR_CHK)->EnableWindow(bSPExist);
	GetDlgItem(IDC_CMD_LATERAL_SOIL_FACTOR_STC)->EnableWindow(bSPExist && bSoilFactor);
	GetDlgItem(IDC_CMD_LATERAL_SOIL_CMB)->EnableWindow(bSPExist && bSoilFactor);  
	
	UpdateData(FALSE);
}

void CCmdAutoLoadCombDlg::OnCmdLossFactorChk()
{
	CtrlManager();
}

void CCmdAutoLoadCombDlg::OnCmdLossFactorBtn()
{    
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_DGN_CON_LOSSFACTOR,0));
	T_DCON_D DconData;
	DconData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcon(DconData);
	m_wndLossFactorTransfer.SetEditUnit(DconData.dTranLossFactor);
	m_wndLossFactorService.SetEditUnit(DconData.dServLossFactor);  
}

//add by cylee 05.04.22
void CCmdAutoLoadCombDlg::OnCmdSetOtholcBtn()
{
	UpdateData(TRUE);
	BOOL bUseLoadCaseZCombo = m_bUseLoadCaseZCombo;
	if(m_nOthoType==1) bUseLoadCaseZCombo = FALSE;

	BOOL bShowLoadCaseZ = TRUE;
	if (m_strCodeName.Find(_T("Eurocode"), 0) < 0 && m_strCodeName != CONCODE_IS456_2000 && m_strCodeName != STLCODE_IS800_2007)
	{
		bShowLoadCaseZ = FALSE;
	}

	CCmdSetOrthoLCDlg dlg;
	dlg.SetUseLoadCaseZCombo(bUseLoadCaseZCombo);
	dlg.SetShowLoadCaseZ(bShowLoadCaseZ);
	dlg.DoModal();
}

void CCmdAutoLoadCombDlg::OnCmdOthoChk()
{
	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif

	// Change by ZINU.('05.12.20). GB50010-02 -> GBJ17-88, GB50017-03, GB50010-02, JGJ138-01.
	// Add by GAY. PMS:4064. ('09.09.17). Add <TWN-ASD96>, <TWN-LSD96>.
	if(strCodeName == _T("TWN-ASD90")   || strCodeName == _T("TWN-LSD90")  || strCodeName == _T("TWN-USD92")   || 
		 strCodeName == _T("TWN-SRC92")   || strCodeName == _T("IS456:2000") || strCodeName == _T("IS:800-1984") || 
		 strCodeName == _T("IS:800-2007") || strCodeName == _T("TWN-ASD96")  || strCodeName == _T("TWN-LSD96")   || 
		 strCodeName == _T("TWN-USD112")  || strCodeName == _T("TWN-USD100") || strCodeName == _T("TWN-SRC100")  ||
		 strCodeName == _T("ACI318-08")   || strCodeName == _T("ACI318-11")  || strCodeName == _T("ACI318-14")   || strCodeName == _T("ACI318M-14") ||
         strCodeName == _T("ACI318-19")   || strCodeName == _T("ACI318M-19") || strCodeName == _T("ACI318-25")   || strCodeName == _T("ACI318M-25") || 
		 strCodeName == LCOMCODE_NTC_CDE_2017|| strCodeName == LCOMCODE_THAILAND_2021)
	{
		GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
		GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
		GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
	}
	else if(strCodeName==_T("NSR-10"))
	{
		BOOL bEnable = m_wndOthoChk.GetCheck()? TRUE : FALSE;
		GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(bEnable);
		GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(bEnable);
		GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(bEnable);
	}
	//add by maxiao(2015-9-25)GB50017-15
	else if(strCodeName==_T("GBJ17-88")  || strCodeName==_T("GB50017-03") || strCodeName==_T("GB50017-15") || strCodeName==_T("GB50010-02") || strCodeName==_T("GB50017-17")
			 || strCodeName==_T("GB/T50010-10")|| strCodeName == _T("GB50429-2007") || strCodeName==_T("JGJ138-01") || strCodeName == _T("GB50068-2018") || strCodeName == _T("GB55006-2021"))
	{
		GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
		GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
        GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
	}
	else if(strCodeName == _T("KBC-USD05") && m_nDesignType == EN_FOOTING)
	{
		GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
		GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
        GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
	}
	else if(strCodeName==_T("KBC-ASD05") && m_nDesignType == EN_FOOTING)  // add by Seungjun (`06.06.15) No.2079.
	{
		GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
		GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
        GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
	}
	else if((strCodeName==_T("KBC-SRC05") || strCodeName==_T("KBC-SRC09") || strCodeName==_T("KBC-SRC16") || strCodeName==SRCCODE_KDS_41_SRC_2019 || strCodeName==SRCCODE_KDS_41_SRC_2022) && m_nDesignType == EN_FOOTING)  // Coded by Seungjun MNet:No.2465 ('20060925)
	{
		GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
		GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
        GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
	}
	else if((strCodeName == _T("KBC-USD09") || strCodeName==_T("KCI-USD12") || strCodeName==_T("KBC-USD16") || strCodeName==CONCODE_KDS_41_30_2018 || strCodeName==CONCODE_KDS_41_20_2022) && m_nDesignType == EN_FOOTING)
	{
		GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
		GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
        GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
	}
	else if((strCodeName == _T("KBC-LSD09") || strCodeName == _T("KBC-LSD16") || strCodeName==STLCODE_KDS_41_31_2019 || strCodeName==STLCODE_KDS_41_30_10_2022) && m_nDesignType == EN_FOOTING) // Add by GAY. PMS:4035. ('09.07.10)
	{
		GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
		GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
	  GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
		GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
		if     (m_nLcomType == D_LCOMTYPE_FDN)      GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		else if(m_nLcomType == D_LCOMTYPE_ALUMINUM) GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		else GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
	}
}

void CCmdAutoLoadCombDlg::OnCmdOthoType()
{
// if(m_nOthoType==0) m_=2;
}

dgn::def::enNationalAnnex CCmdAutoLoadCombDlg::GetCurrentNationalAnnex()
{
	if(m_nDesignType!=EN_STEEL&&m_nDesignType!=EN_CONCRETE&&m_nDesignType!=EN_FOOTING&&m_nDesignType!=EB_CFSTL) 
		return dgn::def::enNationalAnnex::Recommended;

	const int nSel = CDlgUtil::CobxGetCurSelItemData(m_cmbNationalAnnex, m_cmbNationalAnnex.GetCurSel());
	return static_cast<dgn::def::enNationalAnnex>(nSel);
}

void CCmdAutoLoadCombDlg::OnNASelchange()
{
	const auto enCurNA = GetCurrentNationalAnnex();

	const auto bInitPsi = m_enNationalAnnex != enCurNA;
	if(bInitPsi)
	{
		m_enNationalAnnex = enCurNA;
		switch(m_enNationalAnnex)
		{
			case dgn::def::enNationalAnnex::Denmark:
			{
				m_dGammaG=1.20;
				m_dGammaQ=1.50;	
			}
			break;
			case dgn::def::enNationalAnnex::Italy:
			{
				m_dGammaG=1.30;
				m_dGammaQ=1.50;
			}
			break;
			default:
			{
				m_dGammaG=1.35;
				m_dGammaQ=1.50;
			}
			break;
		}
		UpdateData(FALSE);
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	CRect rMove, rRef;
	GetWindowRect(rRef);
	GetDlgItem(IDOK)->GetWindowRect(rMove);
	int nMargin = rRef.bottom - rMove.bottom;

	BOOL bShowND=FALSE;
	UINT uiTargetND=IDC_CMD_IMP_LOAD_FRAME;
	GetNonDissipativeCtrlState(m_strCodeName, uiTargetND, bShowND);
	CDlgUtil::CtrlShowHide(this, m_aCtrlNonDissipative, bShowND);
	if(bShowND)
	{
		GetDlgItem(IDC_CMD_ND_FRAME)->GetWindowRect(rMove);
		GetDlgItem(uiTargetND)->GetWindowRect(rRef);    
		int nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlNonDissipative, nDistY);

		// IDOK 버튼 등의 위치 수정
		GetDlgItem(IDC_CMD_ND_FRAME)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rMove);
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
		if(nDistY!=0) m_nDistY = nDistY;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlOkCancel, nDistY);
	}
	else
	{
		// IDOK 버튼 등의 위치 수정
		GetDlgItem(IDC_CMD_IMP_LOAD_FRAME)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rMove);
		int nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
		if(nDistY!=0) m_nDistY = nDistY;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlOkCancel, nDistY);
	}

	// 윈도우 사이즈를 맞춘다.
	CWnd* pWndLast = GetDlgItem(IDOK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + nMargin;
	MoveWindow(r);
	////////////////////////////////////////////////////////////////////////////////////////////

	if(bInitPsi)
	{
		InitLcomPsiFactor();
	}
}

//add by cylee 05.05.21
void CCmdAutoLoadCombDlg::OnCodeSelchange()
{
	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif
	m_strCodeName = strCodeName;

	if(!m_aCodeEnable.IsEmpty() && m_aCodeEnable[Index]==FALSE)
	{
		CString strCodeName = _T("");
		m_Code.GetLBText(Index, strCodeName);
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), strCodeName);
		GSaveHistoryFormatNF(strErrMsg);
		m_Code.SetCurSel(m_nOldCodeIndex);

		return;
	}

	m_nOldCodeIndex = Index;

	if (!m_wndScaleUpFrame.GetFoldState())
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlScaleUp, TRUE);
	}
	
 	InitUnderLoadCtrl();
	// Change by ZINU.('05.12.20). GB50010-02 -> GBJ17-88, GB50017-03, GB50010-02, JGJ138-01.
	// add by Seungjun (`06.06.15) No.2079. KBC-ASD05
	// Coded by Seungjun MNet:No.2465 ('20060925) KBC-SRC05
	// Add by GAY. PMS:4035. ('09.07.10). KBC-LSD09.
	int nSeisOrthoType = GetSeisOrthoType(strCodeName);
	switch (nSeisOrthoType)
	{
	case EN_SEIS_ORTHO_BOTH:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	case EN_SEIS_ORTHO_SRSS:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			m_nOthoType = 1;
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	case EN_SEIS_ORTHO_100_30:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			m_nOthoType = 0;
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	default:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(FALSE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	}
	
	int nSeisLcomAddType = GetSeisAddLcomType(strCodeName);
	//add by cylee 05.06.15	
	if (nSeisLcomAddType == EN_SEIS_LCOMADD_BOTH && m_nDesignType != EN_FOOTING)  // add by Seungjun (`06.06.15) No.2079.
	{
		GetDlgItem(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_DESIGN_VERT_SEIS_CHK)->EnableWindow(TRUE);
		m_wndSpecialChk.SetCheck(0);
		m_wndVertForceChk.SetCheck(0);
		GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->EnableWindow(TRUE);
	}
	else if (nSeisLcomAddType == EN_SEIS_LCOMADD_SPEC && m_nDesignType != EN_FOOTING)  // MNET:3803-Seungjun-20081208
	{
		GetDlgItem(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_DESIGN_VERT_SEIS_CHK)->EnableWindow(FALSE);
		m_wndSpecialChk.SetCheck(0);
		m_wndVertForceChk.SetCheck(0);         
        GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_DESIGN_SPECIAL_SEIS_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_DESIGN_VERT_SEIS_CHK)->EnableWindow(FALSE);
		m_wndSpecialChk.SetCheck(0);
		m_wndVertForceChk.SetCheck(0);
		GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->EnableWindow(TRUE);

	// Coded by Seungjun ('20070719) MNet:No.2914.  Add snow load psi.
	if(m_nDesignType == EN_STEEL || m_nDesignType == EN_CONCRETE || m_nDesignType == EN_FOOTING || m_nDesignType == EB_CFSTL)
	{
		if(strCodeName == _T("Eurocode2") || strCodeName == _T("Eurocode3") || strCodeName == _T("Eurocode2:04") || strCodeName == _T("Eurocode3:05") || strCodeName == _T("Eurocode3-1-3:06"))
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlSEISLCB, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlGamma, TRUE);
		}
		else if (strCodeName == LCOMCODE_NTC_CDE_2017)
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlSEISLCB, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlGamma, FALSE);
		}
		else
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlSEISLCB, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlGamma, FALSE);
		}
		Set_InitEC();
	}

	// Coded by Seungjun ('20071204) MNet:No.3095.
	if(strCodeName == _T("KBC-USD09") || strCodeName==_T("KCI-USD12"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlPress,  TRUE );
		if(m_bExistCS || !m_bExistEV)
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlPress,  FALSE);
		else
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlPress,  TRUE );
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlPress,  FALSE);
	}

	CDlgUtil::CtrlShowHide(this, m_aCtrlHorEarth, SW_HIDE);

	CDlgUtil::CtrlShowHide(this, m_aCtrlGamma_L, strCodeName == _T("GB/T50010-10") || strCodeName == _T("GB50429-2007") ||  strCodeName == _T("GB50068-2018"));

	CArray<UINT,UINT> aCtrlVertSeis;   aCtrlVertSeis.Add(IDC_CMD_CONSIDER_VERT_SEIS_MAIN_CHK);
	CDlgUtil::CtrlShowHide(this, aCtrlVertSeis, strCodeName == _T("GB/T50010-10")|| strCodeName == _T("GB50429-2007") ||  strCodeName == _T("GB50068-2018"));

	// Add by GAY. PMS:4450. ('12.03.20).
	CDlgUtil::CtrlShowHide(this, m_aCtrlWindFact, IsCodeWindFact(strCodeName));
	CDlgUtil::CtrlShowHide(this, m_aCtrlSeisFact, IsCodeSeisFact(strCodeName));
	CDlgUtil::CtrlShowHide(this, m_aCtrlSoilPresFact, IsCodeSoilPresFact(strCodeName));

	CDlgUtil::CtrlShowHide(this, m_aCtrlNTCCDE2017, strCodeName==LCOMCODE_NTC_CDE_2017);

	if (strCodeName == LCOMCODE_SP_20_13330_2016)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlEC2Phi, SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aCtrlSP_20_13330_2016, SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aCtrlScaleUp, SW_HIDE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlSEISLCB, SW_HIDE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlConstLoad, SW_HIDE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlOrthoLCB, SW_HIDE);

	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlSP_20_13330_2016, SW_HIDE);
		//CDlgUtil::CtrlShowHide(this, m_aCtrlConstLoad, SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aCtrlOrthoLCB, SW_SHOW);
		if (!m_wndManipulationFrame.GetFoldState())
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlConstLoad, TRUE);
		}

	}

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlSeisFact, IsCodeSeisFact(strCodeName));
	//CDlgUtil::CtrlShowHide(this, m_aCtrlACI318_11, strCodeName==_T("ACI318-11"));
	EnableUnderCtrl();

	EnableCtrlNationalAnnex();
	UpdateData(FALSE);
	AlignControl(strCodeName);
	CtrlManager();

	UseLoadCaseZCombo(strCodeName);
}

//add by cylee 05.04.22
void CCmdAutoLoadCombDlg::OnCmdSpecialVertChk()
{
	//	m_wndSpecialChk = 0 or 1
	//  m_wndVertForceChk = 0 or 1
    if (m_strCodeName == _T("IS:800-2007") && m_wndSpecialChk.GetCheck()==TRUE)
    {
        GetDlgItem(IDC_CMD_DESIGN_SEIS_FACTOR_BTN)->EnableWindow(TRUE);
    }
}

void CCmdAutoLoadCombDlg::OnCmdSeisFactorBtn()
{
	//CDgnGenSeisFactor dlg;
	//dlg.DoModal();
	_DGN_GEN_SEIS_FACTOR_DATA SeisFactor;
	SeisFactor.bHideSpecialGroup = FALSE;
	SeisFactor.strConCode = m_strCodeName;
	CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
	pHeadOffice->ReqService(_ULS(dgn), EN_DGNSA_GEN_SEIS_FACTOR, (void*)&SeisFactor);
}

void CCmdAutoLoadCombDlg::OnCmdUnderLoadBtn()
{
	CCmdAutoLoadUnderGroundDlg Dlg;
	Dlg.SetUseSeisFactor(m_bUseUnderLoadSeisFactor);
	if(Dlg.DoModal() == IDOK)
		m_bUseUnderLoadSeisFactor = Dlg.GetUseSeisFactor();
}

void CCmdAutoLoadCombDlg::Set_InitEC()
{
	m_dLLPsi0=0.7;
	m_dLLPsi1=0.5;
	m_dLLPsi2=0.3;
	m_dWLPsi0=0.6;
	m_dWLPsi1=0.2;
	m_dWLPsi2=0.0;
	m_dSLPsi0=0.7;
	m_dSLPsi1=0.5;
	m_dSLPsi2=0.2;
	m_dTLPsi0=0.6;
	m_dTLPsi1=0.5;
	m_dTLPsi2=0.0;
	if(GetCurrentNationalAnnex() == dgn::def::enNationalAnnex::Italy)
	{
		m_dGammaG=1.30;
		m_dGammaQ=1.50;
	}
	else if(GetCurrentNationalAnnex() == dgn::def::enNationalAnnex::Denmark)
	{
		m_dGammaG=1.20;
		m_dGammaQ=1.50;
	}
	else
	{
		m_dGammaG=1.35;
		m_dGammaQ=1.50;
	}
}

void CCmdAutoLoadCombDlg::SetInitAlignLoadCode()
{
	CRect rMove, rRef;
	GetDlgItem(IDC_CMD_NATIONALANNEX_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LOADCODE_STC)->GetWindowRect(rMove);

	int nDistY = rRef.top - rMove.top;
	int nDistX = rRef.left - rMove.left;

	CArray<UINT, UINT> aLoadCodeCtrl;
	aLoadCodeCtrl.Add(IDC_CMD_LOADCODE_STC);
	aLoadCodeCtrl.Add(IDC_CMD_LOADCODE_CMB);
	CDlgUtil::CtrlMoveDistXY(this, aLoadCodeCtrl, nDistX, nDistY);
}
void CCmdAutoLoadCombDlg::EnableCtrlNationalAnnex()
{
	CString strCurrentCode=_T("");
	m_Code.GetWindowText(strCurrentCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCurrentCode);
#endif
	BOOL bEnable = CDBLib::Is_DesignCode_NationalAnnex(strCurrentCode);
	BOOL bEnable_Old = CDBLib::Is_DesignCode_NationalAnnex(m_strOldCodeName);
	BOOL bEnableKDS = FALSE;// CDBLib::Is_DesignCode_KDS(strCurrentCode);
	BOOL bEnableKDS_Old = FALSE;// CDBLib::Is_DesignCode_KDS(m_strOldCodeName);
	m_strCodeName = strCurrentCode;
	GetDlgItem(IDC_CMD_NATIONALANNEX_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_NATIONALANNEX_CMB)->EnableWindow(bEnable);
	
	GetDlgItem(IDC_CMD_NATIONALANNEX_STC)->ShowWindow(bEnable);
	GetDlgItem(IDC_CMD_NATIONALANNEX_CMB)->ShowWindow(bEnable);

	GetDlgItem(IDC_CMD_LOADCODE_STC)->ShowWindow(bEnableKDS);
	GetDlgItem(IDC_CMD_LOADCODE_CMB)->ShowWindow(bEnableKDS);

	//control re-position
	BOOL bMoveCtrl = FALSE;

	if( m_strCodeName == m_strOldCodeName ) return;
	
	if( m_strCodeName == _T("Eurocode3:05") && (m_strOldCodeName == _T("Eurocode2:04") || m_strOldCodeName == _T("Eurocode3-1-3:06")))
	{ 
		m_strOldCodeName = m_strCodeName;
		return;
	} 
	
	if( m_strCodeName == _T("Eurocode2:04") && (m_strOldCodeName == _T("Eurocode3:05") || m_strOldCodeName == _T("Eurocode3-1-3:06")))
	{ 
		m_strOldCodeName = m_strCodeName;
		return;
	}

	if( m_strCodeName == _T("Eurocode3-1-3:06") && (m_strOldCodeName == _T("Eurocode2:04") || m_strOldCodeName == _T("Eurocode3:05")))
	{ 
		m_strOldCodeName = m_strCodeName;
		return;
	} 

	if( (m_strCodeName == _T("Eurocode2:04") || m_strCodeName == _T("Eurocode3:05") || m_strCodeName == _T("Eurocode3-1-3:06")) && (m_strOldCodeName == _T("")) ) 
	{ 
		m_strOldCodeName = m_strCodeName;
		return;
	} 

	if (m_strOldCodeName == _T("") && bEnableKDS)
	{
		m_strOldCodeName = m_strCodeName;
		return;
	}
	if (bEnableKDS && bEnableKDS_Old)
	{
		m_strOldCodeName = m_strCodeName;
		return;
	}
	if (bEnable && bEnableKDS_Old)
	{
		m_strOldCodeName = m_strCodeName;
		return;
	}
	if (bEnableKDS && bEnable_Old)
	{
		m_strOldCodeName = m_strCodeName;
		return;
	}
	if( m_strCodeName == _T("Eurocode3:05")     || m_strOldCodeName == _T("Eurocode3:05")     ||
			m_strCodeName == _T("Eurocode2:04")     || m_strOldCodeName == _T("Eurocode2:04")     ||
			m_strCodeName == _T("Eurocode3-1-3:06") || m_strOldCodeName == _T("Eurocode3-1-3:06"))
	{
		bMoveCtrl = TRUE;
	}

	if (bEnableKDS || bEnableKDS_Old) bMoveCtrl = TRUE;
	if(bMoveCtrl)
	{
		CRect rNa,rFrame;
		GetDlgItem(IDC_CMD_NATIONALANNEX_CMB)->GetWindowRect(rNa);
		GetDlgItem(IDC_CMD_DESIGN_TYPE_GRB)->GetWindowRect(rFrame);
		int nDistY = rNa.Height();

		if( (m_strOldCodeName == _T("Eurocode2:04")) || (m_strOldCodeName == _T("Eurocode3:05"))  || (m_strOldCodeName == _T("Eurocode3-1-3:06")) || bEnableKDS_Old)//up
		{
			nDistY*=(-1);
		}

		rFrame.bottom += nDistY;
		
		GetDlgItem(IDC_CMD_DESIGN_TYPE_GRB)->ScreenToClient(rFrame);
		GetDlgItem(IDC_CMD_DESIGN_TYPE_GRB)->SetWindowPos(NULL, rFrame.left, rFrame.top, rFrame.Width(), rFrame.Height(), SWP_NOMOVE | SWP_NOZORDER);
		
		CRect RectDlg;
		GetWindowRect(&RectDlg);
		
		RectDlg.bottom += nDistY;
		
		ScreenToClient(RectDlg);
		SetWindowPos(NULL, RectDlg.left, RectDlg.top, RectDlg.Width(), RectDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
		
		CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrlID, nDistY);

		BOOL bShowImpf;
		UINT uiTargetImpf;
		GetImperfectionCtrlState(m_strCodeName, uiTargetImpf, bShowImpf);
		if(bShowImpf)
		{
			// Imperfecton Load 관련 컨트럴 위치 수정
			CRect rMove, rRef;
			GetDlgItem(IDC_CMD_IMP_LOAD_FRAME)->GetWindowRect(rMove);
			GetDlgItem(uiTargetImpf)->GetWindowRect(rRef);    
			nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlImperfectLoad, nDistY);
		}

		BOOL bShowTemper;
		UINT uiTargetTemper;
		GetImperfectionCtrlState(m_strCodeName, uiTargetTemper, bShowTemper);
		if(bShowTemper)
		{
			// Imperfecton Load 관련 컨트럴 위치 수정
			CRect rMove, rRef;
			GetDlgItem(IDC_CMD_TEMPERAUTER_CONDITION_FRAME)->GetWindowRect(rMove);
			GetDlgItem(uiTargetTemper)->GetWindowRect(rRef);    
			nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlTemperature, nDistY);
		}

		if (m_strCodeName == CONCODE_NSCP_2015 || m_strCodeName == STLCODE_NSCP_2015_LRFD || m_strCodeName == STLCODE_NSCP_2015_ASD)
		{
			BOOL bLossFactorPS = FALSE;
			if (m_nDesignType == EN_CONCRETE) bLossFactorPS = TRUE;

			CRect rMove, rRef;
			if (bLossFactorPS) GetDlgItem(IDC_CMD_LOSS_FACTOR_FRAME)->GetWindowRect(rRef);
			else GetDlgItem(IDC_CMD_SEIS_LCB_FRAME)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_REDUN_LOAD_FACTOR_CHK)->GetWindowRect(rMove);
			nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlNSCP2015, nDistY);
		}

		BOOL bShowND;
		UINT uiTargetND;
		GetNonDissipativeCtrlState(m_strCodeName, uiTargetND, bShowND);
		if(bShowND)
		{
			CRect rMove, rRef;
			GetDlgItem(IDC_CMD_ND_FRAME)->GetWindowRect(rMove);
			GetDlgItem(uiTargetND)->GetWindowRect(rRef);    
			nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlNonDissipative, nDistY);
		}
	}
	m_strOldCodeName = m_strCodeName;
}

int CCmdAutoLoadCombDlg::GetSeisOrthoType(const CString& strCodeName)
{
	if (strCodeName == _T("KBC-ASD05")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-LSD09")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-LSD16")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KDS 41 31 : 2019")) return EN_SEIS_ORTHO_BOTH;
    if (strCodeName == _T("KDS 41 30 : 2022")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-USD05")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-USD09")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KDS 41 30 : 2018")) return EN_SEIS_ORTHO_BOTH;
    if (strCodeName == _T("KDS 41 20 : 2022")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-USD16")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KCI-USD12")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-SRC05")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-SRC09")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-SRC16")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-SRC16")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-SRC16")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-SRC16")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("KBC-SRC16")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == SRCCODE_KDS_41_SRC_2019 ) return EN_SEIS_ORTHO_BOTH;
    if (strCodeName == SRCCODE_KDS_41_SRC_2022 ) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("Eurocode2")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("Eurocode3")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("Eurocode2:04")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("Eurocode3:05")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("Eurocode3-1-3:06")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("NSR-10")) return EN_SEIS_ORTHO_BOTH;
	/////////////////////////////////////////////////////////////////////
	if (strCodeName == _T("GBJ17-88")) return EN_SEIS_ORTHO_SRSS;
	if (strCodeName == _T("GB50017-03")) return EN_SEIS_ORTHO_SRSS;
	if (strCodeName == _T("GB50017-15")) return EN_SEIS_ORTHO_SRSS;
	if (strCodeName == _T("GB50017-17")) return EN_SEIS_ORTHO_SRSS;
	if (strCodeName == _T("GB55006-2021")) return EN_SEIS_ORTHO_SRSS;
	if (strCodeName == _T("GB50068-2018")) return EN_SEIS_ORTHO_SRSS;
	if (strCodeName == _T("GB50010-02")) return EN_SEIS_ORTHO_SRSS;
	if (strCodeName == _T("GB/T50010-10")) return EN_SEIS_ORTHO_SRSS;
	if (strCodeName == _T("GB50429-2007")) return EN_SEIS_ORTHO_SRSS;
	if (strCodeName == _T("JGJ138-01")) return EN_SEIS_ORTHO_SRSS;
	/////////////////////////////////////////////////////////////////////
	if (strCodeName == _T("TWN-ASD90")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("TWN-ASD96")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("TWN-LSD90")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("TWN-LSD96")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("TWN-USD92")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("TWN-USD100")) return EN_SEIS_ORTHO_100_30;
    if (strCodeName == _T("TWN-USD112")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("TWN-SRC92")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("TWN-SRC100")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("IS456:2000")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("IS:800-1984")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("IS:800-2007")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("ACI318-08")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("ACI318-11")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("ACI318-14")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("ACI318M-14")) return EN_SEIS_ORTHO_100_30;	
    if (strCodeName == _T("ACI318-19")) return EN_SEIS_ORTHO_100_30;
    if (strCodeName == _T("ACI318M-19")) return EN_SEIS_ORTHO_100_30;
	if (strCodeName == _T("ACI318-25")) return EN_SEIS_ORTHO_100_30;
    if (strCodeName == _T("ACI318M-25")) return EN_SEIS_ORTHO_100_30;
    if ( strCodeName == CONCODE_NSCP_2015 ) return EN_SEIS_ORTHO_100_30;
    if ( strCodeName == STLCODE_NSCP_2015_LRFD ) return EN_SEIS_ORTHO_100_30;
    if ( strCodeName == STLCODE_NSCP_2015_ASD  ) return EN_SEIS_ORTHO_100_30;
    if ( strCodeName == STLCODE_AISC_ASD16 ) return EN_SEIS_ORTHO_BOTH;
    if ( strCodeName == STLCODE_AISC_LRFD16 ) return EN_SEIS_ORTHO_BOTH;
    if ( strCodeName == STLCODE_AISC_LRFD22 ) return EN_SEIS_ORTHO_BOTH;
	if ( strCodeName == STLCODE_KDS_24_14_31_2018 ) return EN_SEIS_ORTHO_BOTH;
	if ( strCodeName == LCOMCODE_NTC_CDE_2017 ) return EN_SEIS_ORTHO_100_30;
	if ( strCodeName == LCOMCODE_THAILAND_2021 ) return EN_SEIS_ORTHO_100_30;

	return EN_SEIS_ORTHO_NONE;
}

int CCmdAutoLoadCombDlg::GetSeisAddLcomType(const CString& strCodeName)
{
	if (strCodeName == _T("KBC-USD05")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("KBC-USD09")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("KDS 41 30 : 2018")) return EN_SEIS_LCOMADD_BOTH;
    if (strCodeName == _T("KDS 41 20 : 2022")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("KBC-USD16")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("KCI-USD12")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("ACI318-05")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("ACI318-08")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("ACI318-11")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("ACI318-14")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("ACI318M-14")) return EN_SEIS_LCOMADD_BOTH;
    if (strCodeName == _T("ACI318-19"))   return EN_SEIS_LCOMADD_BOTH;
    if (strCodeName == _T("ACI318M-19"))  return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("ACI318-25"))   return EN_SEIS_LCOMADD_BOTH;
    if (strCodeName == _T("ACI318M-25"))  return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("KBC-ASD05")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("KBC-LSD09")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("KBC-LSD16")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("KDS 41 31 : 2019")) return EN_SEIS_LCOMADD_BOTH;
    if (strCodeName == _T("KDS 41 30 : 2022")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("KBC-SRC05")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("KBC-SRC09")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("KBC-SRC16")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == SRCCODE_KDS_41_SRC_2019) return EN_SEIS_LCOMADD_BOTH;
    if (strCodeName == SRCCODE_KDS_41_SRC_2022) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == STLCODE_AISC_LRFD22) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == STLCODE_AISC_ASD22)  return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("AISC(15th)-LRFD16") ) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("AISC(15th)-ASD16") )  return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("AISC(14th)-LRFD10")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("AISC(14th)-ASD10"))  return EN_SEIS_LCOMADD_BOTH;	
    if (strCodeName == STLCODE_NSCP_2015_LRFD ) return EN_SEIS_LCOMADD_BOTH;
    if (strCodeName == STLCODE_NSCP_2015_ASD  ) return EN_SEIS_LCOMADD_BOTH;
    if (strCodeName == CONCODE_NSCP_2015 ) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == CONCODE_TWN_USD112) return EN_SEIS_LCOMADD_SPEC;	
	if (strCodeName == _T("IS:800-2007")) return EN_SEIS_LCOMADD_SPEC;
	if (strCodeName == LCOMCODE_THAILAND_2021) return EN_SEIS_LCOMADD_BOTH;
	
	return EN_SEIS_LCOMADD_NONE;
}

int CCmdAutoLoadCombDlg::GetWindFactCtrls(CArray<UINT, UINT>& raCtrl)	
{
	raCtrl.RemoveAll();
	raCtrl.Add(IDC_CMD_WIND_FACTOR_FRAME);  
	raCtrl.Add(IDC_CMD_WIND_STRN_RDO);
	raCtrl.Add(IDC_CMD_WIND_SERV_RDO);
	return (int)raCtrl.GetSize();
}

int CCmdAutoLoadCombDlg::GetSeisFactCtrls(CArray<UINT, UINT>& raCtrl)
{
	raCtrl.RemoveAll();
	raCtrl.Add(IDC_CMD_SEIS_FACTOR_FRAME);
	raCtrl.Add(IDC_CMD_SEIS_STRN_RDO);
	raCtrl.Add(IDC_CMD_SEIS_SERV_RDO);	
	return (int)raCtrl.GetSize();
}

int CCmdAutoLoadCombDlg::GetSoilPresFactCtrls(CArray<UINT, UINT>& raCtrl)
{
	raCtrl.RemoveAll();
	raCtrl.Add(IDC_CMD_LATERAL_SOIL_FACTOR_FRAME);
	raCtrl.Add(IDC_CMD_LATERAL_SOIL_FACTOR_CHK);
	raCtrl.Add(IDC_CMD_LATERAL_SOIL_FACTOR_STC);
	raCtrl.Add(IDC_CMD_LATERAL_SOIL_CMB);
	return (int)raCtrl.GetSize();
}


BOOL CCmdAutoLoadCombDlg::IsCodeWindFact(const CString& strCodeName)
{
	if (strCodeName == _T("ACI318-11")) return TRUE;
	if (strCodeName == _T("ACI318-14")) return TRUE;
	if (strCodeName == _T("ACI318M-14")) return TRUE;
    if (strCodeName == _T("ACI318-19")) return TRUE;
    if (strCodeName == _T("ACI318M-19")) return TRUE;
	if (strCodeName == _T("ACI318-25")) return TRUE;
    if (strCodeName == _T("ACI318M-25")) return TRUE;
	return FALSE;
}

BOOL CCmdAutoLoadCombDlg::IsCodeSeisFact(const CString& strCodeName)
{
	if (strCodeName == _T("ACI318-11")) return TRUE;	
	return FALSE;
}

BOOL CCmdAutoLoadCombDlg::IsCodeSoilPresFact(const CString& strCodeName)
{
	if (strCodeName == _T("ACI318-11")) return TRUE;
	if (strCodeName == _T("ACI318-14")) return TRUE;
	if (strCodeName == _T("ACI318M-14")) return TRUE;
    if (strCodeName == _T("ACI318-19")) return TRUE;
    if (strCodeName == _T("ACI318M-19")) return TRUE;
	if (strCodeName == _T("ACI318-25")) return TRUE;
    if (strCodeName == _T("ACI318M-25")) return TRUE;
	if (strCodeName == CONCODE_NSCP_2015) return TRUE;

	return FALSE;
}

BOOL CCmdAutoLoadCombDlg::IsCodeStlTempFact(const CString& strCodeName)
{
	if (strCodeName == _T("GB51249-2017")) return TRUE;
	return FALSE;
}

BOOL CCmdAutoLoadCombDlg::IsConcludeTemperatureCase()
{
	CString str = _T("");
	m_CobxTempCond.GetWindowText(str);
	if (str == FIRE_LOAD_CASE_NAME)
		return TRUE;
	return FALSE;
}


BOOL CCmdAutoLoadCombDlg::IsCodeUnderLoad(CString strCodeName /*= _T("")*/)
{
	BOOL bUnderLoad = FALSE;

#if !defined(_CIVIL)
	if (strCodeName == _T(""))
	{
		int Index = m_Code.GetCurSel();
		m_Code.GetLBText(Index, strCodeName);
#if defined(_RUS)
		CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif
	}

    bUnderLoad = [] (int nDgnType, const CString& strCode)
    {
        switch ( nDgnType )
        {
        case EN_STEEL:
            return (strCode == STLCODE_KDS_41_31_2019 || strCode == STLCODE_KDS_41_30_10_2022) ? TRUE : FALSE;
        case EN_CONCRETE:
        case EN_FOOTING:
            return (strCode == CONCODE_KDS_41_30_2018 || strCode == CONCODE_KDS_41_20_2022) ? TRUE : FALSE;
        case EN_SRC:
            return (strCode == SRCCODE_KDS_41_SRC_2019 || strCode == SRCCODE_KDS_41_SRC_2022) ? TRUE : FALSE;
        default:
            return FALSE;
        }
    }(m_nDesignType, strCodeName);  
#endif

	return bUnderLoad;
}

void CCmdAutoLoadCombDlg::OnCmdAutoAlphaHBtn()
{
	UpdateData(TRUE);

	CCMDLoadCalcAlphaH Dlg;

	if(Dlg.DoModal()==IDOK)
	{
		m_dAlpha_H = Dlg.m_dAlpha_H;
	}

	UpdateData(FALSE);
}

void CCmdAutoLoadCombDlg::InitLcomPsiFactor()
{
	CArray<T_STLD_K, T_STLD_K> aStldKeyList; aStldKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetStldKeyList4LcomPsiFactor(aStldKeyList);
	int nSize = aStldKeyList.GetSize();
	if(nSize<1) return;
	
	m_mLcomPsiFactor.RemoveAll();
	m_mLcomPsiFactor.InitHashTable(nSize);
	
	T_LPSI_D LpsiD;
	T_STLD_D StldD;
	for(int i=0; i<nSize; i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetStld(aStldKeyList[i], StldD)) {ASSERT(0); continue;}
		
		LpsiD.nIndex = i+1;
		LpsiD.StldKey = aStldKeyList[i];
		LpsiD.LoadCaseName = StldD.LoadCaseName;
		LpsiD.LoadCaseType = StldD.LoadCaseType;
		if(StldD.LoadCaseType==_T("L") || StldD.LoadCaseType==_T("LR"))
		{
			LpsiD.dPsi0 = 0.7;
			LpsiD.dPsi1 = 0.5;
			LpsiD.dPsi2 = 0.3;
		}
		else if(StldD.LoadCaseType==_T("W"))
		{
			GetPsi4WindLoad(LpsiD.dPsi0, LpsiD.dPsi1, LpsiD.dPsi2);
		}
		else if(StldD.LoadCaseType==_T("S"))
		{
			GetPsi4SnowLoad(LpsiD.dPsi0, LpsiD.dPsi1, LpsiD.dPsi2);
		}
		else if(StldD.LoadCaseType==_T("T"))
		{
			LpsiD.dPsi0 = 0.6;
			LpsiD.dPsi1 = 0.5;
			LpsiD.dPsi2 = 0.0;
		}
		else {ASSERT(0); continue;}
		
		m_mLcomPsiFactor.SetAt(LpsiD.StldKey, LpsiD);
	}
}

void CCmdAutoLoadCombDlg::GetPsi4WindLoad(double& dPsi0, double& dPsi1, double& dPsi2)
{
	const auto aPsi = [this](){
		switch(m_enNationalAnnex)
		{
			case dgn::def::Denmark:		
			case dgn::def::Sweden:
			case dgn::def::Sweden19:		return std::make_tuple(0.3, 0.2, 0.0);
			case dgn::def::Malaysia:	
			case dgn::def::Singapore:	
			case dgn::def::UnitedKingdom:	return std::make_tuple(0.5, 0.2, 0.0);
			case dgn::def::Netherlands:		return std::make_tuple(0.0, 0.2, 0.0);
			case dgn::def::Romania:			return std::make_tuple(0.7, 0.2, 0.0);
			default:						return std::make_tuple(0.6, 0.2, 0.0); // Recommended
		}
	}();

	dPsi0 = std::get<0>(aPsi);
	dPsi1 = std::get<1>(aPsi);
	dPsi2 = std::get<2>(aPsi);
}

void CCmdAutoLoadCombDlg::GetPsi4SnowLoad(double& dPsi0, double& dPsi1, double& dPsi2)
{
	const auto aPsi = [this](){
		switch(m_enNationalAnnex)
		{
			case dgn::def::Belgium:			return !m_bOver1000m ? std::make_tuple(0.5, 0.0, 0.0) : std::make_tuple(0.7, 0.5, 0.2);
			case dgn::def::CzechRepublic:	return !m_bOver1000m ? std::make_tuple(0.5, 0.2, 0.0) : std::make_tuple(0.7, 0.5, 0.2);
			case dgn::def::Denmark:			return std::make_tuple(0.3, 0.2, 0.0);
			case dgn::def::Finland:			return !m_bOver1000m ? std::make_tuple(0.7, 0.4, 0.2) : std::make_tuple(0.7, 0.5, 0.3);
			case dgn::def::France:			return !m_bOver1000m ? std::make_tuple(0.5, 0.2, 0.0) : std::make_tuple(0.7, 0.5, 0.2);
			case dgn::def::Germany:			return !m_bOver1000m ? std::make_tuple(0.5, 0.2, 0.0) : std::make_tuple(0.7, 0.5, 0.2);
			case dgn::def::Ireland:			return std::make_tuple(0.5, 0.2, 0.0);
			case dgn::def::Italy:			return !m_bOver1000m ? std::make_tuple(0.5, 0.2, 0.0) : std::make_tuple(0.7, 0.5, 0.2);
			case dgn::def::Malaysia:		return !m_bOver1000m ? std::make_tuple(0.5, 0.2, 0.0) : std::make_tuple(0.7, 0.5, 0.2);
			case dgn::def::Netherlands:		return std::make_tuple(0.0, 0.2, 0.0);
			case dgn::def::Romania:			return std::make_tuple(0.7, 0.5, 0.4);
			case dgn::def::Singapore:		return std::make_tuple(0.0, 0.0, 0.0);
			case dgn::def::Sweden:
			case dgn::def::Sweden19:		return std::make_tuple(0.8, 0.6, 0.2); // sk > 3.0kN/m2
			case dgn::def::UnitedKingdom:	return !m_bOver1000m ? std::make_tuple(0.5, 0.2, 0.0) : std::make_tuple(0.7, 0.5, 0.2);
			default:						return std::make_tuple(0.7, 0.5, 0.2); // Recommended
		}
	}();

	dPsi0 = std::get<0>(aPsi);
	dPsi1 = std::get<1>(aPsi);
	dPsi2 = std::get<2>(aPsi);
}

void CCmdAutoLoadCombDlg::OnCmdServFactorBtn()
{
	if (m_strCodeName == LCOMCODE_SP_20_13330_2016)
	{
		CCmdLcomSP20PsiFactorDlg Dlg;
		Dlg.DoModal();
	}
	else
	{
		CCmdLcomPsiFactorDlg Dlg;
		Dlg.SetData(m_mLcomPsiFactor);
		if (Dlg.DoModal() == IDOK)
		{
			Dlg.GetData(m_mLcomPsiFactor);
		}
	}
}

void CCmdAutoLoadCombDlg::OnCmdWindDirectionBtn()
{
	CLoadCaseWindDirectDlg Dlg;
//   Dlg.SetData(m_mLcomPsiFactor);
	if(Dlg.DoModal()==IDOK)
	{
//     Dlg.GetData(m_mLcomPsiFactor);
	}
}

void CCmdAutoLoadCombDlg::OnCmdHorEarthChk()
{
	BOOL bHorEarthChk = m_wndHorEarthChk.GetCheck()?TRUE:FALSE;
	GetDlgItem(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_STRENGTH_STC)->EnableWindow(bHorEarthChk);
	GetDlgItem(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_SERV_STC)->EnableWindow(bHorEarthChk);
	GetDlgItem(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_STRENGTH_CMB)->EnableWindow(bHorEarthChk);
	GetDlgItem(IDC_CMD_DESIGN_HORI_EARTHPRESSURE_SERV_CMB)->EnableWindow(bHorEarthChk);
}

void CCmdAutoLoadCombDlg::OnCmdLLRedunBtn()
{
	CCmdAutoLoadLiveLoadRFDlg dlg;
	dlg.DoModal();
}
void CCmdAutoLoadCombDlg::OnCmdLLRedunChk()
{
	BOOL bLLRedunChk = m_wndLLReduFactor.GetCheck() ? TRUE : FALSE;
	GetDlgItem(IDC_CMD_LL_REDUN_LOAD_FACTOR_BTN)->EnableWindow(bLLRedunChk);
}
void CCmdAutoLoadCombDlg::OnCmdRedunChk()
{
	BOOL bRedunChk = m_wndReduFactor.GetCheck() ? TRUE : FALSE;
	GetDlgItem(IDC_CMD_REDUN_LOAD_FACTOR_STC)->EnableWindow(bRedunChk);
	GetDlgItem(IDC_CMD_REDUN_LOAD_FACTOR_EDT)->EnableWindow(bRedunChk);
}
void CCmdAutoLoadCombDlg::OnCmdWindLevel()
{

}

void CCmdAutoLoadCombDlg::OnCmdSeisLevel()
{

}

void CCmdAutoLoadCombDlg::OnCmdSoilFactorChk()
{
	UpdateData(TRUE);
	CtrlManager();
}

// PARKHJ-20110823 : 함수 이동 -> CAttrCtrl2
BOOL CCmdAutoLoadCombDlg::GetUsedStldKeyList(CArray<T_STLD_K, T_STLD_K>& aKeyList)
{
	// Load Case 추가시 BOOL CCmdSetOrthoLCDlg::GetUsedEQKeyList() 에도 반영 할 것.
	POSITION pos;
	T_STLD_K StldK;

	CMap<T_STLD_K, T_STLD_K, BOOL, BOOL> mStldK;
	// Self Weight
	pos = m_pDoc->m_pAttrCtrl->GetStartBodf();
	while(pos)
	{
		T_BODF_D BodfD;
		m_pDoc->m_pAttrCtrl->GetNextBodf(pos, StldK, BodfD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(BodfD.LoadCaseKey)) mStldK.SetAt(BodfD.LoadCaseKey, TRUE);
	}
	// Nodal Load
	pos = m_pDoc->m_pAttrCtrl->GetStartCnld();
	while(pos)
	{
		T_CNLD_K CnldK; T_CNLD_D CnldD;
		m_pDoc->m_pAttrCtrl->GetNextCnld(pos, CnldK, CnldD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(CnldD.LoadCaseKey)) mStldK.SetAt(CnldD.LoadCaseKey, TRUE);
	}
	// Add by GAY.('06.04.11). Nodal Body Force
	pos = m_pDoc->m_pAttrCtrl->GetStartNbof();
	while(pos)
	{
		T_NBOF_K NbofK; T_NBOF_D NbofD;
		m_pDoc->m_pAttrCtrl->GetNextNbof(pos, NbofK, NbofD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(NbofD.LoadCaseKey)) mStldK.SetAt(NbofD.LoadCaseKey, TRUE);
	}
	// Specified Displacement
	pos = m_pDoc->m_pAttrCtrl->GetStartSdsp();
	while(pos)
	{
		T_SDSP_K SdspK; T_SDSP_D SdspD;
		m_pDoc->m_pAttrCtrl->GetNextSdsp(pos, SdspK, SdspD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(SdspD.LoadCaseKey)) mStldK.SetAt(SdspD.LoadCaseKey, TRUE);
	}
	// Beam Load
	pos = m_pDoc->m_pAttrCtrl->GetStartBmld();
	while(pos)
	{
		T_BMLD_K BmldK; T_BMLD_D BmldD;
		m_pDoc->m_pAttrCtrl->GetNextBmld(pos, BmldK, BmldD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(BmldD.LoadCaseKey)) mStldK.SetAt(BmldD.LoadCaseKey, TRUE);
	}
	// Typical Beam Load 
	// Floor Load
	pos = m_pDoc->m_pAttrCtrl->GetStartFbla();
	while(pos)
	{
		T_FBLA_K TblaK; T_FBLA_D FblaD; T_FBLD_D FbldD;
		m_pDoc->m_pAttrCtrl->GetNextFbla(pos, TblaK, FblaD);
		m_pDoc->m_pAttrCtrl->GetFbld(FblaD.LoadTypeKey, FbldD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[0])) mStldK.SetAt(FbldD.LoadCaseKey[0], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[1])) mStldK.SetAt(FbldD.LoadCaseKey[1], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[2])) mStldK.SetAt(FbldD.LoadCaseKey[2], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[3])) mStldK.SetAt(FbldD.LoadCaseKey[3], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[4])) mStldK.SetAt(FbldD.LoadCaseKey[4], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[5])) mStldK.SetAt(FbldD.LoadCaseKey[5], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[6])) mStldK.SetAt(FbldD.LoadCaseKey[6], TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[7])) mStldK.SetAt(FbldD.LoadCaseKey[7], TRUE);
	}
	// Plane Load
	pos = m_pDoc->m_pAttrCtrl->GetStartPnla();
	while(pos)
	{
		T_PNLA_K PlnaK; T_PNLA_D PnlaD;
		m_pDoc->m_pAttrCtrl->GetNextPnla(pos, PlnaK, PnlaD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(PnlaD.LoadCaseKey)) mStldK.SetAt(PnlaD.LoadCaseKey, TRUE);
	}
	// Finishing Material Load
	pos = m_pDoc->m_pAttrCtrl->GetStartFmld();
	while(pos)
	{
		T_FMLD_K FmldK; T_FMLD_D FmldD;
		m_pDoc->m_pAttrCtrl->GetNextFmld(pos, FmldK, FmldD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(FmldD.LoadCaseKey)) mStldK.SetAt(FmldD.LoadCaseKey, TRUE);
	}
	// Prestress Beam Load
	pos = m_pDoc->m_pAttrCtrl->GetStartPrst();
	while(pos)
	{
		 T_PRST_K PrstK; T_PRST_D PrstD;
			m_pDoc->m_pAttrCtrl->GetNextPrst(pos, PrstK, PrstD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(PrstD.LoadCaseKey)) mStldK.SetAt(PrstD.LoadCaseKey, TRUE);
	}
	// Pretension Load
	pos = m_pDoc->m_pAttrCtrl->GetStartPtns();
	while(pos)
	{
		T_PTNS_K PtnsK; T_PTNS_D PtnsD;
		m_pDoc->m_pAttrCtrl->GetNextPtns(pos, PtnsK, PtnsD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(PtnsD.LoadCaseKey)) mStldK.SetAt(PtnsD.LoadCaseKey, TRUE);
	}
	// Tendon Prestress Load
	pos = m_pDoc->m_pAttrCtrl->GetStartTdpl();
	while(pos)
	{
		T_TDPL_K TdplK; T_TDPL_D TdplD;
		m_pDoc->m_pAttrCtrl->GetNextTdpl(pos, TdplK, TdplD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(TdplD.LoadCaseKey)) mStldK.SetAt(TdplD.LoadCaseKey, TRUE);
	}
	// Pressure Load
	pos = m_pDoc->m_pAttrCtrl->GetStartPres();
	while(pos)
	{
		T_PRES_K PresK; T_PRES_D PresD;
		m_pDoc->m_pAttrCtrl->GetNextPres(pos, PresK, PresD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(PresD.LoadCaseKey)) mStldK.SetAt(PresD.LoadCaseKey, TRUE);
	}
	// Hydrostatic Pressure Load
	// Initial Force
	if(m_pDoc->m_pAttrCtrl->ExistIfct())
	{
		T_IFCT_D IfctD;
		m_pDoc->m_pAttrCtrl->GetIfct(IfctD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(IfctD.LoadCaseKey)) mStldK.SetAt(IfctD.LoadCaseKey, TRUE);
	}
	// System Temperature
	T_STMP_K StmpK; T_STMP_D StmpD;
	pos = m_pDoc->m_pAttrCtrl->GetStartStmp();
	while(pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextStmp(pos, StmpK, StmpD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(StmpD.LoadCaseKey)) mStldK.SetAt(StmpD.LoadCaseKey, TRUE);
	}
	// Nodal Temperature
	pos = m_pDoc->m_pAttrCtrl->GetStartNtmp();
	while(pos)
	{
		T_NTMP_K NtmpK; T_NTMP_D NtmpD;
		m_pDoc->m_pAttrCtrl->GetNextNtmp(pos, NtmpK, NtmpD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(NtmpD.LoadCaseKey)) mStldK.SetAt(NtmpD.LoadCaseKey, TRUE);
	}
	// Element Temperature
	pos = m_pDoc->m_pAttrCtrl->GetStartEtmp();
	while(pos)
	{
		T_ETMP_K EtmpK; T_ETMP_D EtmpD;
		m_pDoc->m_pAttrCtrl->GetNextEtmp(pos, EtmpK, EtmpD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(EtmpD.LoadCaseKey)) mStldK.SetAt(EtmpD.LoadCaseKey, TRUE);
	}
	// Temperature Gradient
	T_GTMP_K GtmpK; T_GTMP_D GtmpD;
	pos = m_pDoc->m_pAttrCtrl->GetStartGtmp();
	while(pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextGtmp(pos, GtmpK, GtmpD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(GtmpD.LoadCaseKey)) mStldK.SetAt(GtmpD.LoadCaseKey, TRUE);
	}
	// Beam Section Temperature
	pos = m_pDoc->m_pAttrCtrl->GetStartBtmp();
	while(pos)
	{
		T_BTMP_K BtmpK; T_BTMP_D BtmpD;
		m_pDoc->m_pAttrCtrl->GetNextBtmp(pos, BtmpK, BtmpD);
		if(m_pDoc->m_pAttrCtrl->ExistStld(BtmpD.LoadCaseKey)) mStldK.SetAt(BtmpD.LoadCaseKey, TRUE);
	}
	// Wind Load
	pos = m_pDoc->m_pAttrCtrl->GetStartWind();
	while(pos)
	{
		T_WIND_K WindK; T_WIND_D WindD;
		m_pDoc->m_pAttrCtrl->GetNextWind(pos, WindK, WindD);
		if(m_pDoc->m_pAttrCtrl->ExistStld((T_STLD_K)WindK)) mStldK.SetAt((T_STLD_K)WindK, TRUE);
	}
	// Static Seismic Load
	pos = m_pDoc->m_pAttrCtrl->GetStartSeis();
	while(pos)
	{
		T_SEIS_K SeisK; T_SEIS_D SeisD;
		m_pDoc->m_pAttrCtrl->GetNextSeis(pos, SeisK, SeisD);
		if(m_pDoc->m_pAttrCtrl->ExistStld((T_STLD_K)SeisK)) mStldK.SetAt((T_STLD_K)SeisK, TRUE);
	}
	// Superstructure Seismic Load
	pos = m_pDoc->m_pAttrCtrl->GetStartSsei();
	while(pos)
	{
		T_SSEI_K SseiK; T_SSEI_D SseiD;
		m_pDoc->m_pAttrCtrl->GetNextSsei(pos, SseiK, SseiD);
		//if(m_pDoc->m_pAttrCtrl->ExistStld(SseiD.StldKey)) mStldK.SetAt(SseiD.StldKey, TRUE);
		if(m_pDoc->m_pAttrCtrl->ExistStld((T_STLD_K)SseiK)) mStldK.SetAt((T_STLD_K)SseiK, TRUE);
	}
 
	BOOL tmp;
	pos = mStldK.GetStartPosition();
	while(pos)
	{
		mStldK.GetNextAssoc(pos, StldK, tmp);
		aKeyList.Add(StldK);
	}
	aKeyList.FreeExtra();
	qsort(aKeyList.GetData(), aKeyList.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);

	return TRUE;
}

void CCmdAutoLoadCombDlg::Initial_SoilFactor_ComboBox()
{
	m_cmbLSPfact.ResetContent();
	
	m_cmbLSPfact.AddString(_T("1.6"));
	m_cmbLSPfact.AddString(_T("0.9"));
	m_cmbLSPfact.SetCurSel(0);
}

LRESULT CCmdAutoLoadCombDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rWnd, rTgt;
	GetWindowRect(rWnd);
	GetDlgItem(IDOK)->GetWindowRect(rTgt);

	rWnd.bottom = rTgt.bottom+8;
	MoveWindow(rWnd);

	return 0l;
}

void CCmdAutoLoadCombDlg::OnCmdImpLoadBtn()
{
		CString strCodeName=_T("");
		int Index = m_Code.GetCurSel();
		m_Code.GetLBText(Index,strCodeName);
#if defined(_RUS)
		CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif
		CCmdAutoLoadCombImperfectionLoadDlg dlg(strCodeName);
		dlg.SetData(m_aImperfectionLoad);
		if(dlg.DoModal())
		{
				dlg.GetData(m_aImperfectionLoad);    
		}  
}

void CCmdAutoLoadCombDlg::OnCmdImpLoadChk()
{
		CString strCodeName=_T("");
		int Index = m_Code.GetCurSel();
		m_Code.GetLBText(Index, strCodeName);
#if defined(_RUS)
		CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif
		if(strCodeName == _T("GB50017-17") || strCodeName == _T("GB55006-2021"))
				GetDlgItem(IDC_CMD_IMP_LOAD_BTN)->EnableWindow(FALSE);
		else
		{
				BOOL bChkImpLoad = m_chkImpLoad.GetCheck();
				GetDlgItem(IDC_CMD_IMP_LOAD_BTN)->EnableWindow(bChkImpLoad);
		}  
}

void CCmdAutoLoadCombDlg::OnCmdLiveLoadTypeBtn()
{
	CCmdLcomLiveLoadTypeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{

	}
// 	CString strCodeName = _T("");
// 	int Index = m_Code.GetCurSel();
// 	m_Code.GetLBText(Index, strCodeName);
// 	CCmdAutoLoadCombImperfectionLoadDlg dlg(strCodeName);
// 	dlg.SetData(m_aImperfectionLoad);
// 	if (dlg.DoModal())
// 	{
// 		dlg.GetData(m_aImperfectionLoad);
// 	}
}

void CCmdAutoLoadCombDlg::UseLoadCaseZCombo(CString& strCodeName)
{
	m_bUseLoadCaseZCombo = FALSE;
	CString strRule = _LS(IDS_CMD_LCOM_100_30_RULE);
	if (strCodeName == CONCODE_EC2 || strCodeName == STLCODE_EC3 || strCodeName == CONCODE_EC2_04 || strCodeName == STLCODE_EC3_2_05 || strCodeName == STLCODE_EC3_05 || strCodeName == CFSCODE_EC3_06 ||
		strCodeName == CONCODE_IS456_2000 || strCodeName == STLCODE_IS800_2007)
	{
		m_bUseLoadCaseZCombo = TRUE;
		strRule = _LS(IDS_CMD_LCOM_100_30_30_RULE);
	}

	GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->SetWindowText(strRule);
}