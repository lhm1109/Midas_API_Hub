// CMMvhlItemStdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\SpecialChar.h"

#include "CMMvhlStdEuroBSNewTempForRefactor.h"
#include "CMMvhlSItemUsrLaneFactor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdDlg dialog
#define COLCOUNT 3

CCMMvhlItemStdDlg::CCMMvhlItemStdDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdDlg::IDD, pParent)
{
	m_pDoc          = 0;
	m_bModify       = FALSE;
	m_dD1           = 0.0;
	m_dD2           = 0.0;
	m_dW1           = 0.0;
	m_dW2           = 0.0;
	m_W             = 0.0;
	m_Ps            = 0.0;
	m_Pm            = 0.0;
	m_dP            = 0.0;
	m_dQm           = 0.0;
	m_dQq           = 0.0;

	m_dBSW1         = 0.0;
	m_dBSW2         = 0.0;
	m_dBSW3         = 0.0;
	m_dBSL1         = 0.0;
	m_dBSL2         = 0.0;
	m_dBSPa         = 0.0;
	m_dBSPb         = 0.0;
	m_dBSD1         = 0.0;
	m_dBSD2         = 0.0;
	m_dBSD          = 0.0;
	m_dBSD3         = 0.0;
	m_dBSD4         = 0.0;
	m_dBSD5         = 0.0;
	m_dBSD6         = 0.0;
 
	m_dQkJTGLane    = 0.0;
	m_dPk1JTGLane   = 0.0;
	m_dPk2JTGLane   = 0.0;
	m_dL1JTGLane    = 0.0;
	m_dL2JTGLane    = 0.0;
	m_dW1JTGCrowd   = 0.0;
	m_dW2JTGCrowd   = 0.0;
	m_dL1JTGCrowd   = 0.0;
	m_dL2JTGCrowd   = 0.0;
	//m_dWdJTGCrowd = 0.0;

	m_dBS37W1       = 0.0;
	m_dBS37W2       = 0.0;
	m_dBS37W3       = 0.0;
	m_dBS37L1       = 0.0;
	m_dBS37L2       = 0.0;
	m_dBS37L21      = 0.0;
	m_dBS37L3       = 0.0;
	m_strBS37W1_exp = _T("");
	m_strBS37W2_exp = _T("");

	m_nDynaType     = 0;
	m_nStndCode     = 0;

	InitRcAr();
}

CCMMvhlItemStdDlg::~CCMMvhlItemStdDlg()
{
}


void CCMMvhlItemStdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdDlg)  
	DDX_Control(pDX, IDC_CMD_MVHL_BS_DD_EDIT,               m_edtBSdd);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_DD_UNIT,               m_untBSdd);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_W1_EDIT,              m_edtPedW1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_W1_UNIT,              m_untPedW1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_W2_EDIT,              m_edtPedW2);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_W2_UNIT,              m_untPedW2);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L1_EDIT,              m_edtPedL1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L1_UNIT,              m_untPedL1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L2_EDIT,              m_edtPedL2);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L2_UNIT,              m_untPedL2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_UNIT_NUM_EDIT,         m_edtNumUnit);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNAMIC,                  m_untDynLoadAllowance);
	DDX_Control(pDX, IDC_CMD_MVHL_NAME,                     m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_Qq_UNIT,                  m_stQq);
	DDX_Control(pDX, IDC_CMD_MVHL_Qm_UNIT,                  m_stQm);
	DDX_Control(pDX, IDC_CMD_MVHL_P_UNIT,                   m_stP);
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,                 m_cbxCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_W_UNIT,                   m_stW);
	DDX_Control(pDX, IDC_CMD_MVHL_Ps_UNIT,                  m_stPs);
	DDX_Control(pDX, IDC_CMD_MVHL_Pm_UNIT,                  m_stPm);
	DDX_Control(pDX, IDC_CMD_MVHL_dW2_UNIT,                 m_stdW2);
	DDX_Control(pDX, IDC_CMD_MVHL_dW1_UNIT,                 m_stdW1);
	DDX_Control(pDX, IDC_CMD_MVHL_dD2_UNIT,                 m_stdD2);
	DDX_Control(pDX, IDC_CMD_MVHL_dD1_UNIT,                 m_stdD1);
	DDX_Control(pDX, IDC_CMD_MVHL_PTLIST,                   m_List);	
	DDX_Control(pDX, IDC_CMD_MVHL_TYPE_COMBO,               m_wndVehicleTypeList);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_QK_UNIT,               m_untJTGQkLane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_PK1_UNIT,              m_untJTGPk1Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_PK2_UNIT,              m_untJTGPk2Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L1_UNIT,               m_untJTGL1Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L2_UNIT,               m_untJTGL2Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_W1_UNIT,               m_untJTGW1Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_W2_UNIT,               m_untJTGW2Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L1_UNIT2,              m_untJTGL1Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L2_UNIT2,              m_untJTGL2Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_WD_UNIT,               m_untJTGWdCrowd);
	DDX_Text(pDX, IDC_CMD_MVHL_CH_QK_EDIT,                  m_dQkJTGLane);
	DDX_Text(pDX, IDC_CMD_MVHL_CH_PK1_EDIT,                 m_dPk1JTGLane);
	DDX_Text(pDX, IDC_CMD_MVHL_CH_PK2_EDIT,                 m_dPk2JTGLane);
	DDX_Text(pDX, IDC_CMD_MVHL_CH_L1_EDIT,                  m_dL1JTGLane);
	DDX_Text(pDX, IDC_CMD_MVHL_CH_L2_EDIT,                  m_dL2JTGLane);
	DDX_Text(pDX, IDC_CMD_MVHL_CH_W1_EDIT,                  m_dW1JTGCrowd);
	DDX_Text(pDX, IDC_CMD_MVHL_CH_W2_EDIT,                  m_dW2JTGCrowd);
	DDX_Text(pDX, IDC_CMD_MVHL_CH_L1_EDIT2,                 m_dL1JTGCrowd);
	DDX_Text(pDX, IDC_CMD_MVHL_CH_L2_EDIT2,                 m_dL2JTGCrowd);
	//DDX_Text(pDX, IDC_CMD_MVHL_CH_WD_EDIT,                m_dWdJTGCrowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_WD_EDIT,               m_edtJTGCrowd);
	DDX_Text(pDX, IDC_CMD_MVHL_dD1,                         m_dD1);
	DDX_Text(pDX, IDC_CMD_MVHL_dD2,                         m_dD2);
	DDX_Text(pDX, IDC_CMD_MVHL_dW1,                         m_dW1);
	DDX_Text(pDX, IDC_CMD_MVHL_dW2,                         m_dW2);
	DDX_Text(pDX, IDC_CMD_MVHL_W,                           m_W);
	DDX_Text(pDX, IDC_CMD_MVHL_PLH,                         m_Ps);
	DDX_Text(pDX, IDC_CMD_MVHL_PLV,                         m_Pm);
	DDX_Text(pDX, IDC_CMD_MVHL_P,                           m_dP);
	DDX_Text(pDX, IDC_CMD_MVHL_Qm,                          m_dQm);
	DDX_Text(pDX, IDC_CMD_MVHL_Qq,                          m_dQq);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_W1_EDIT,                  m_dBSW1);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_W2_EDIT,                  m_dBSW2);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_W3_EDIT,                  m_dBSW3);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_L1_EDIT,                  m_dBSL1);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_L2_EDIT,                  m_dBSL2);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_PA_EDIT,                  m_dBSPa);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_PB_EDIT,                  m_dBSPb);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_D1_EDIT,                  m_dBSD1);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_D2_EDIT,                  m_dBSD2);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_D_EDIT,                   m_dBSD);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_D3_EDIT,                  m_dBSD3);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_D4_EDIT,                  m_dBSD4);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_D5_EDIT,                  m_dBSD5);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_D6_EDIT,                  m_dBSD6);
	DDX_Text(pDX, IDC_CMD_MVHL_BS_W2_EXP_STATIC,            m_strBSW2_exp);

	DDX_Text(pDX, IDC_CMD_MVHL_BS37_W1_EDIT,                m_dBS37W1);
	DDX_Text(pDX, IDC_CMD_MVHL_BS37_W2_EDIT,                m_dBS37W2);
	DDX_Text(pDX, IDC_CMD_MVHL_BS37_W3_EDIT,                m_dBS37W3);
	DDX_Text(pDX, IDC_CMD_MVHL_BS37_L1_EDIT,                m_dBS37L1);
	DDX_Text(pDX, IDC_CMD_MVHL_BS37_L2_EDIT,                m_dBS37L2);
	DDX_Text(pDX, IDC_CMD_MVHL_BS37_L2_EDIT1,               m_dBS37L21);
	DDX_Text(pDX, IDC_CMD_MVHL_BS37_L3_EDIT,                m_dBS37L3);
	DDX_Text(pDX, IDC_CMD_MVHL_BS37_W2_EXP_STATIC,          m_strBS37W2_exp);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_W1_UNIT,             m_untBS37W1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_W2_UNIT,             m_untBS37W2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_W3_UNIT,             m_untBS37W3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_L1_UNIT,             m_untBS37L1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_L2_UNIT,             m_untBS37L2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_L3_UNIT,             m_untBS37L3);

	DDX_Control(pDX, IDC_CMD_MVHL_BS_W1_UNIT,               m_untBSW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_UNIT,               m_untBSW2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W3_UNIT,               m_untBSW3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L1_UNIT,               m_untBSL1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L2_UNIT,               m_untBSL2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PA_UNIT,               m_untBSPa);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PB_UNIT,               m_untBSPb);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D1_UNIT,               m_untBSD1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D2_UNIT,               m_untBSD2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D_UNIT,                m_untBSD);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D3_UNIT,               m_untBSD3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D4_UNIT,               m_untBSD4);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D5_UNIT,               m_untBSD5);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D6_UNIT,               m_untBSD6);
 	DDX_Control(pDX, IDC_CMD_APPLY,                         m_wndBtnApply);

	// BS BD37/01 Lane Factor
	DDX_Radio  (pDX, IDC_CMD_MVHL_LANE_FACTOR_RDO1,         m_nLaneFactor);

	// BS CS 454 Lane Factor
	DDX_Radio  (pDX, IDC_CMD_MVHL_LANE_FACTOR_CS_RDO1,      m_nLaneFactorCS);

	// Canada Dynamic Load Allowance
	DDX_Control(pDX, IDC_CMD_MVHL_INCRE_AXSPACE_CHK,        m_chkIncreAxleSpace);
	DDX_Control(pDX, IDC_CMD_MVHL_INCRE_AXSPACE_EDT,        m_edtIncreAxleSpace);
	DDX_Control(pDX, IDC_CMD_MVHL_INCRE_AXSPACE_UNT,        m_untIncreAxleSpace);
	DDX_Radio  (pDX, IDC_CMD_MVHL_DYNA_AUTO_RDO,            m_nDynaType);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_1AXLE_EDT,           m_edt1Axle);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_2AXLE_EDT,           m_edt2Axle);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_3AXLE_EDT,           m_edt3Axle);

	DDX_Control(pDX, IDC_CMD_MVHL_CH_CROWD_WP_EDT,          m_edtCJJ11wp);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_CROWD_WP_UNT,          m_undCJJ11wp);

	DDX_Control(pDX, IDC_CMD_MVHL_DYNAMIC_CHK,              m_wndDynamic);
	DDX_Control(pDX, IDC_CMD_MVHL_PHI_EDT,                  m_wndPhi);

	DDX_Control(pDX, IDC_OVERLOAD_FACTOR_CHK,               m_chkOverLoadFactor);

	DDX_Control(pDX, IDC_OVERLOAD_FACTOR_CRI_AXLE_EDT,      m_edtOverCriAxle);
	DDX_Control(pDX, IDC_OVERLOAD_FACTOR_OTHER_AXLE_EDT,    m_edtOverOtherAxle);

	DDX_Control(pDX, IDC_CMD_MVHL_DD,                       m_edtDD);
	DDX_Control(pDX, IDC_CMD_MVHL_DD_UNIT,                  m_untDD);

	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_AL_EDT2, m_edtaL);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_AL_UNT, m_untaL);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO, m_cbxCategory);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_LOAD_LEVEL_CBO, m_cbxLoadLevel);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_DATA_CHK, m_chkAddData);  

	DDX_Control(pDX, IDC_CMD_MVHL_LEGAL_W,          m_edtLegalW   );
	DDX_Control(pDX, IDC_CMD_MVHL_LEGAL_W_UNIT,     m_untLegalW   );
	DDX_Control(pDX, IDC_CMD_MVHL_LEGAL_R,          m_edtLegalR   );
	DDX_Control(pDX, IDC_CMD_MVHL_LEGAL_R_UNIT,     m_untLegalR   );
	DDX_Control(pDX, IDC_CMD_MVHL_LEGAL_DIST,       m_edtLegalDist);
	DDX_Control(pDX, IDC_CMD_MVHL_LEGAL_DIST_UNIT,  m_untLegalDist);
	DDX_Control(pDX, IDC_CMD_MVHL_Z_EDT,            m_edtz);

	DDX_Control(pDX, IDC_CMD_MVHL_PENNDOT_W,          m_edtPenndotW );
	DDX_Control(pDX, IDC_CMD_MVHL_PENNDOT_W_UNIT,     m_untPenndotW );
	DDX_Control(pDX, IDC_CMD_MVHL_PENNDOT_R1      ,   m_edtPenndotR1);
	DDX_Control(pDX, IDC_CMD_MVHL_PENNDOT_R1_UNIT ,   m_untPenndotR1);
	DDX_Control(pDX, IDC_CMD_MVHL_PENNDOT_R2      ,   m_edtPenndotR2);
	DDX_Control(pDX, IDC_CMD_MVHL_PENNDOT_R2_UNIT ,   m_untPenndotR2);
	DDX_Control(pDX, IDC_CMD_MVHL_PENNDOT_DIST    ,   m_edtPenndotDist);
	DDX_Control(pDX, IDC_CMD_MVHL_PENNDOT_DIST_UNIT,  m_untPenndotDist);

	DDX_Control(pDX, IDC_CMD_MVHL_TDM_W,          m_edtTdmW   );
	DDX_Control(pDX, IDC_CMD_MVHL_TDM_W_UNIT,     m_untTdmW   );
	DDX_Control(pDX, IDC_CMD_MVHL_TDM_D2,         m_edtTdmD2  );
	DDX_Control(pDX, IDC_CMD_MVHL_TDM_D2_UNIT,    m_untTdmD2  );
	DDX_Control(pDX, IDC_CMD_MVHL_TDM_D3,         m_edtTdmD3  );
	DDX_Control(pDX, IDC_CMD_MVHL_TDM_D3_UNIT,    m_untTdmD3  );

	DDX_Control(pDX, IDC_CMD_MVHL_ADD_CF_CHK, m_chkCentF);  

	DDX_Control(pDX, IDC_CMD_FIRST_GRID, m_wndThreeGrid[0]);
	DDX_Control(pDX, IDC_CMD_SECOND_GRID, m_wndThreeGrid[1]);

	DDX_Control(pDX, IDC_CMD_MVHL_PIC_WND, m_wndPicture);
	DDX_Control(pDX, IDC_CMD_MVHL_TDM_CHK, m_chkTDM);

	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_CHK, m_chkPatch);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_EDT, m_edtPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_UNT, m_untPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_EDT, m_edtPatchL);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_UNT, m_untPatchL);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_TYPE_COMBO, OnSelchangeCmdMvhlTypeCombo)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB, OnSelchangeCmdMvhlCodeCmb)

	ON_BN_CLICKED(IDC_CMD_MVHL_LANE_FACTOR_RDO1, OnChangeLaneFactor)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_AUTO_BD21_01_RDO, OnChangeLaneFactor)  
	ON_BN_CLICKED(IDC_CMD_MVHL_LANE_FACTOR_RDO2, OnChangeLaneFactor)

	ON_BN_CLICKED(IDC_CMD_MVHL_LANE_FACTOR_CS_RDO1, OnChangeLaneFactorCS)
	ON_BN_CLICKED(IDC_CMD_MVHL_LANE_FACTOR_CS_RDO2, OnChangeLaneFactorCS)

	ON_BN_CLICKED(IDC_CMD_MVHL_INCRE_AXSPACE_CHK, OnBnClickedIncreAxleSpaceChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNA_AUTO_RDO,    OnChangeDynaTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNA_USER_RDO,    OnChangeDynaTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNAMIC_CHK,         OnCmdMvhlEuroDynamicChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_AUTO_RDO,            OnCmdMvhlEuroAutoRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_USER_RDO,            OnCmdMvhlEuroAutoRdo)

	ON_BN_CLICKED(IDC_OVERLOAD_FACTOR_CHK,         OnCmdMvhlOverLoadChk)
	ON_BN_CLICKED(IDC_OVERLOAD_FACTOR_AUTO_RDO,    OnCmdMvhlOverLoadRdo)
	ON_BN_CLICKED(IDC_OVERLOAD_FACTOR_USER_RDO,    OnCmdMvhlOverLoadRdo)

	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_BTN, OnHALaneFactor)  
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_CS_LANE_FACT_USER_BTN, OnHALaneFactor)  

	ON_BN_CLICKED(IDC_CMD_MVHL_BS_ADD_DATA_CHK, OnChkAddData)

	ON_BN_CLICKED(IDC_CMD_MVHL_PATCH_CHK, OnChkPatch)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdDlg message handlers

BOOL CCMMvhlItemStdDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	T_MVCD_D MvcdD; MvcdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return TRUE;
	m_nMovingType = MvcdD.nCodeType;

	m_bPatchLoad = FALSE;
	  
	InitGrid();
	SaveCtrlPos();
	AlignControl();		// 코드타입에 따라 프레임 정렬  

	SetDefValAndCbxItem();

	int nLoadType;
	GetIndex(nLoadType);
	if (nLoadType < 0) return TRUE;
	// 중복된 이름이 있을 경우 Standard 를 따라가야 한다.
	if(m_Data.nStandardCode != nLoadType) nLoadType = m_Data.nStandardCode;

	SetLoadTypeList(nLoadType);
	InitThreeGrid(nLoadType);

	SetHeaderTitle();
	Data2Dlg();

	if(m_nMovingType == D_MOVE_CODE_BS)
	{
		if(m_nStndCode==D_MVHL_BS_CS454) SetEnableBS_CS();
		else							 SetEnableBS();
	}

	OnBnClickedIncreAxleSpaceChk();
	OnChangeDynaTypeRdo();
	EnableDisableIncreAxleSpace();

	if(m_bModify)	m_wndBtnApply.EnableWindow(FALSE);

	SetCtrlUnit();  
	AlignControlDynamic();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMMvhlItemStdDlg::InitRcAr()
{
	m_aCtrlCom.Add(IDC_CMD_MVHL_W_STATIC);
	m_aCtrlCom.Add(IDC_CMD_MVHL_Ps_STATIC);
	m_aCtrlCom.Add(IDC_CMD_MVHL_Pm_STATIC);
	m_aCtrlCom.Add(IDC_CMD_MVHL_W);
	m_aCtrlCom.Add(IDC_CMD_MVHL_PLH);
	m_aCtrlCom.Add(IDC_CMD_MVHL_PLV);
	m_aCtrlCom.Add(IDC_CMD_MVHL_W_UNIT);
	m_aCtrlCom.Add(IDC_CMD_MVHL_Ps_UNIT);
	m_aCtrlCom.Add(IDC_CMD_MVHL_Pm_UNIT);

	m_aCtrlCh.Add(IDC_CMD_MVHL_P_STATIC);
	m_aCtrlCh.Add(IDC_CMD_MVHL_Qm_STATIC);
	m_aCtrlCh.Add(IDC_CMD_MVHL_Qq_STATIC);
	m_aCtrlCh.Add(IDC_CMD_MVHL_P);
	m_aCtrlCh.Add(IDC_CMD_MVHL_Qm);
	m_aCtrlCh.Add(IDC_CMD_MVHL_Qq);
	m_aCtrlCh.Add(IDC_CMD_MVHL_P_UNIT);
	m_aCtrlCh.Add(IDC_CMD_MVHL_Qm_UNIT);
	m_aCtrlCh.Add(IDC_CMD_MVHL_Qq_UNIT);  

	m_aCtrlAashtoLrfd.Add(IDC_CMD_MVHL_DYNAMIC);
	m_aCtrlAashtoLrfd.Add(IDC_CMD_MVHL_DYN_TEXT);
	m_aCtrlAashtoLrfd.Add(IDC_CMD_MVHL_DYN_TEXT2);

	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dW1_STATIC);
	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dW1);
	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dW1_UNIT);
	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dD1_STATIC);
	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dD1);
	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dD1_UNIT);
	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dW2_STATIC);
	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dW2);
	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dW2_UNIT);
	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dD2_STATIC);
	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dD2);
	m_aCtrlIrc1.Add(IDC_CMD_MVHL_dD2_UNIT);

	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W1_STATIC);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W1_EDIT);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W1_UNIT);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W2_STATIC);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W2_EDIT);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W2_STATIC1);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W2_EXP_STATIC);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W2_UNIT);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W3_STATIC);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W3_EDIT);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W3_UNIT);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_W3_STATIC1);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_L1_STATIC);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_L1_EDIT);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_L1_UNIT);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_L2_STATIC);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_L2_EDIT);
	m_aCtrlBS5400.Add(IDC_CMD_MVHL_BS_L2_UNIT);

	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W1_STATIC);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W1_EDIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W1_STATIC1);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W1_EXP_STATIC);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W1_UNIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W2_STATIC);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W2_EDIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W2_STATIC1);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W2_EXP_STATIC);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W2_UNIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W3_STATIC);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W3_EDIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_W3_UNIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L1_STATIC);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L1_EDIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L1_UNIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L2_STATIC);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L2_EDIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L2_STATIC1);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L2_EDIT1);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L2_UNIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L3_STATIC);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L3_EDIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L3_UNIT);
	m_aCtrlBS3701.Add(IDC_CMD_MVHL_BS37_L3_STATIC1);
	
	m_aCtrlBSAddData.RemoveAll();
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_BS_ADD_DATA_CHK);
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_BS_ADD_DATA_STC);
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_BS_AD_FACTOR_ST);
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_AL_ST);
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_BS_ADD_AL_EDT2);
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_BS_ADD_AL_UNT);
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_BS_RD_FACTOR_ST);
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_BS_CATEGORY_ST2);
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_BS_CS_CATEGORY_TEMP_STC2);
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO);
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_BS_LOADLEVEL_ST2);
	m_aCtrlBSAddData.Add(IDC_CMD_MVHL_BS_ADD_LOAD_LEVEL_CBO);

	m_aCtrlBSCSAddData.RemoveAll();
	m_aCtrlBSCSAddData.Add(IDC_CMD_MVHL_BS_CS_CATEGORY_GRP);
	m_aCtrlBSCSAddData.Add(IDC_CMD_MVHL_BS_CS_CATEGORY_STC);
	m_aCtrlBSCSAddData.Add(IDC_CMD_MVHL_BS_CS_LOADLEVEL_STC);
	m_aCtrlBSCSAddData.Add(IDC_CMD_MVHL_BS_CS_CATEGORY_TEMP_STC);
	
	m_aCtrlBS_UnitNum.Add(IDC_CMD_MVHL_BS_UNIT_NUM_STATIC);
	m_aCtrlBS_UnitNum.Add(IDC_CMD_MVHL_BS_UNIT_NUM_EDIT);
	m_aCtrlBS_Pa.Add(IDC_CMD_MVHL_BS_PA_STATIC);
	m_aCtrlBS_Pa.Add(IDC_CMD_MVHL_BS_PA_EDIT);
	m_aCtrlBS_Pa.Add(IDC_CMD_MVHL_BS_PA_UNIT);
	m_aCtrlBS_Pb.Add(IDC_CMD_MVHL_BS_PB_STATIC);
	m_aCtrlBS_Pb.Add(IDC_CMD_MVHL_BS_PB_EDIT);
	m_aCtrlBS_Pb.Add(IDC_CMD_MVHL_BS_PB_UNIT);
	m_aCtrlBS_dd.Add(IDC_CMD_MVHL_BS_DD_STATIC);
	m_aCtrlBS_dd.Add(IDC_CMD_MVHL_BS_DD_EDIT);
	m_aCtrlBS_dd.Add(IDC_CMD_MVHL_BS_DD_UNIT);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_W1_STATIC);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_W1_EDIT);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_W1_UNIT);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_W2_STATIC);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_W2_EDIT);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_W2_UNIT);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_L1_STATIC);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_L1_EDIT);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_L1_UNIT);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_L2_STATIC);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_L2_EDIT);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_L2_UNIT);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_L3_STATIC);
	m_aCtrlBS_Ped.Add(IDC_CMD_MVHL_PED_K_STATIC);

	m_aCtrlBS.Copy(m_aCtrlBS_Pa);
	m_aCtrlBS.Append(m_aCtrlBS_Pb);
	m_aCtrlBS.Append(m_aCtrlBS_UnitNum);
	m_aCtrlBS.Append(m_aCtrlBS_dd);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D_STATIC);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D_EDIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D_UNIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D1_STATIC);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D1_EDIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D1_UNIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D2_STATIC);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D2_EDIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D2_UNIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D3_STATIC);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D3_EDIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D3_UNIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D4_STATIC);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D4_EDIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D4_UNIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D5_STATIC);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D5_EDIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D5_UNIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D6_STATIC);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D6_EDIT);
	m_aCtrlBS.Add(IDC_CMD_MVHL_BS_D6_UNIT);

	m_aCtrlBS_HA.Copy(m_aCtrlBS3701);
	m_aCtrlBS_HA.Append(m_aCtrlBS_Pa);
	m_aCtrlBS_HB.Copy(m_aCtrlBS);
	m_aCtrlBS_HB.RemoveAt(0);  // Remove PA Static
	m_aCtrlBS_HB.RemoveAt(0);  // Remove PA Edit
	m_aCtrlBS_HB.RemoveAt(0); // Remove PA Unit

	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_QK_STATIC);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_QK_EDIT);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_QK_UNIT);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_PK1_STATIC);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_PK1_EDIT);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_PK1_UNIT);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_PK2_STATIC);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_PK2_EDIT);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_PK2_UNIT);  
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_L1_STATIC);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_L1_EDIT);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_L1_UNIT);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_L2_STATIC);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_L2_EDIT);
	m_aCtrlJTGLane.Add(IDC_CMD_MVHL_CH_L2_UNIT);  

	m_aCtrlJTGTruck.Add(IDC_CMD_MVHL_PTLIST);  

	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_W1_STATIC);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_W1_EDIT);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_W1_UNIT);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_W2_STATIC);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_W2_EDIT);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_W2_UNIT);  
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_L1_STATIC2);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_L1_EDIT2);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_L1_UNIT2);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_L2_STATIC2);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_L2_EDIT2);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_L2_UNIT2);  
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_WD_STATIC);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_WD_EDIT);
	m_aCtrlJTGCrowd.Add(IDC_CMD_MVHL_CH_WD_UNIT);  

	m_aGroupBox1.Add(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1);

	m_aGroupBox2.Add(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES2);

	m_aBSBD3701LaneFactor.Add(IDC_CMD_MVHL_LANE_FACTOR);
	m_aBSBD3701LaneFactor.Add(IDC_CMD_MVHL_LANE_FACTOR_RDO1);
	m_aBSBD3701LaneFactor.Add(IDC_CMD_MVHL_BS_HA_LANE_FACT_AUTO_BD21_01_RDO);  
	m_aBSBD3701LaneFactor.Add(IDC_CMD_MVHL_LANE_FACTOR_RDO2);
	m_aBSBD3701LaneFactor.Add(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_STC);
	m_aBSBD3701LaneFactor.Add(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_BTN);

	m_aBSCS454LaneFactor.Add(IDC_CMD_MVHL_LANE_FACTOR_CS);
	m_aBSCS454LaneFactor.Add(IDC_CMD_MVHL_LANE_FACTOR_CS_RDO1);
	m_aBSCS454LaneFactor.Add(IDC_CMD_MVHL_LANE_FACTOR_CS_RDO2);
	m_aBSCS454LaneFactor.Add(IDC_CMD_MVHL_BS_CS_LANE_FACT_USER_STC);
	m_aBSCS454LaneFactor.Add(IDC_CMD_MVHL_BS_CS_LANE_FACT_USER_BTN);
	m_aBSCS454LaneFactor.Add(IDC_CMD_MVHL_PIC_CS_WND);

	m_aBSCS454LaneFactorCmb.Add(IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO);
	m_aBSCS454LaneFactorCmb.Add(IDC_CMD_MVHL_BS_ADD_LOAD_LEVEL_CBO);
	
	m_aOKCancelApply.Add(IDOK);
	m_aOKCancelApply.Add(IDCANCEL);
	m_aOKCancelApply.Add(IDC_CMD_APPLY);

	m_aDyna.Add(IDC_CMD_MVHL_INCRE_AXSPACE_CHK);
	m_aDyna.Add(IDC_CMD_MVHL_INCRE_AXSPACE_EDT);
	m_aDyna.Add(IDC_CMD_MVHL_INCRE_AXSPACE_UNT);
	m_aDyna.Add(IDC_CMD_MVHL_DYNA_GRP);
	m_aDyna.Add(IDC_CMD_MVHL_DYNA_AUTO_RDO);
	m_aDyna.Add(IDC_CMD_MVHL_DYNA_USER_RDO);
	m_aDyna.Add(IDC_CMD_MVHL_DYNA_1AXLE_EDT);
	m_aDyna.Add(IDC_CMD_MVHL_DYNA_1AXLE_STC);
	m_aDyna.Add(IDC_CMD_MVHL_DYNA_2AXLE_EDT);
	m_aDyna.Add(IDC_CMD_MVHL_DYNA_2AXLE_STC);
	m_aDyna.Add(IDC_CMD_MVHL_DYNA_3AXLE_EDT);
	m_aDyna.Add(IDC_CMD_MVHL_DYNA_3AXLE_STC);

	m_aCJJ11Crowd.Add(IDC_CMD_MVHL_CH_CROWD_WP_STC);
	m_aCJJ11Crowd.Add(IDC_CMD_MVHL_CH_CROWD_WP_EDT);
	m_aCJJ11Crowd.Add(IDC_CMD_MVHL_CH_CROWD_WP_UNT);
	
	m_aCtrlSoSov.RemoveAll();
	m_aCtrlSoSov.Add(IDC_CMD_MVHL_FIRST_TXT);
	m_aCtrlSoSov.Add(IDC_CMD_MVHL_SECOND_TXT);
	m_aCtrlSoSov.Add(IDC_CMD_FIRST_GRID);
	m_aCtrlSoSov.Add(IDC_CMD_SECOND_GRID);
	m_aCtrlSoSov.Add(IDC_CMD_MVHL_SVSOV_A_STC);
	m_aCtrlSoSov.Add(IDC_CMD_MVHL_PHI_GRUP);
	m_aCtrlSoSov.Add(IDC_CMD_MVHL_DYNAMIC_CHK);
	m_aCtrlSoSov.Add(IDC_CMD_MVHL_AUTO_RDO);
	m_aCtrlSoSov.Add(IDC_CMD_MVHL_USER_RDO);
	m_aCtrlSoSov.Add(IDC_CMD_MVHL_PHI_EQUATION_TXT);
	m_aCtrlSoSov.Add(IDC_CMD_MVHL_PHI_TXT);
	m_aCtrlSoSov.Add(IDC_CMD_MVHL_PHI_EDT);
	m_aCtrlSoSov.Add(IDC_OVERLOAD_FACTOR_GRP);
	m_aCtrlSoSov.Add(IDC_OVERLOAD_FACTOR_CHK);
	m_aCtrlSoSov.Add(IDC_OVERLOAD_FACTOR_AUTO_RDO);
	m_aCtrlSoSov.Add(IDC_OVERLOAD_FACTOR_USER_RDO);
	m_aCtrlSoSov.Add(IDC_OVERLOAD_FACTOR_CRI_AXLE_STC);
	m_aCtrlSoSov.Add(IDC_OVERLOAD_FACTOR_CRI_AXLE_EDT);
	m_aCtrlSoSov.Add(IDC_OVERLOAD_FACTOR_OTHER_AXLE_STC);
	m_aCtrlSoSov.Add(IDC_OVERLOAD_FACTOR_OTHER_AXLE_EDT);
	m_aCtrlSoSov.Add(IDC_VEHICLE_SPEED_GRP);
	m_aCtrlSoSov.Add(IDC_VEHICLE_SPEED_NORMAL_RDO);
	m_aCtrlSoSov.Add(IDC_VEHICLE_SPEED_USER_RDO);

	m_aCtrlPhi.RemoveAll();
	m_aCtrlPhi.Add(IDC_CMD_MVHL_PHI_TXT);
	m_aCtrlPhi.Add(IDC_CMD_MVHL_PHI_EDT);

	m_aCtrlDD.Add(IDC_CMD_MVHL_DD_STATIC);
	m_aCtrlDD.Add(IDC_CMD_MVHL_DD);
	m_aCtrlDD.Add(IDC_CMD_MVHL_DD_UNIT);

	m_aCtrlz.Add(IDC_CMD_MVHL_Z_STC);
	m_aCtrlz.Add(IDC_CMD_MVHL_Z_EDT);

	m_aCtrlPenndot.RemoveAll();
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_W_STATIC);
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_W);
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_W_UNIT);
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_R1_STATIC);
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_R1);
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_R1_UNIT);
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_R2_STATIC);
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_R2);
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_R2_UNIT);
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_DIST_STATIC);
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_DIST);
	m_aCtrlPenndot.Add(IDC_CMD_MVHL_PENNDOT_DIST_UNIT);

	m_aCtrlPatch.RemoveAll();
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_CHK);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_UNT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_UNT);

}

void CCMMvhlItemStdDlg::SaveCtrlPos()
{
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->GetWindowRect(m_rectPosVehiLoadProCommon);
	ScreenToClient(m_rectPosVehiLoadProCommon);

	m_rectPosVehiLoadProBS_BD8611 = m_rectPosVehiLoadProCommon;
	m_rectPosVehiLoadProBS_BD8611.bottom += 218;
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvhlItemStdDlg::Data2Dlg()
{
	m_nStndCode = m_Data.nStandardCode;
	if(m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(m_Data.VehicleTypeName);
	else
		m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);
	
	int nLoadType;
	//int nIndex = GetIndex(m_Data.VehicleTypeName, &sIndex);
	//CDlgUtil::CobxSetCurSelItemData(m_wndCodeName, nIndex);  
	//m_wndVehicleTypeList.SetCurSel(sIndex);
	//ChangeBitmap(nIndex, sIndex);

	CDlgUtil::CobxSetCurSelItemData(m_cbxCodeName, m_nStndCode);
	ResetVehicleTypeCmb();
	GetIndex(nLoadType);
	m_wndVehicleTypeList.SetCurSel(nLoadType);  
	m_untDynLoadAllowance.SetEditUnit(m_Data.dDynLoadAllowance);

	// BS BD37/01 : Lane Factor  
	m_nLaneFactor = m_Data.nLaneFactor;
	// BS CS 454 : Lane Factor  
	m_nLaneFactorCS = (m_Data.nLaneFactor>1)? 1 : m_Data.nLaneFactor;
	
	T_MVHL_PATCH_LOAD PatchLoadDB;
	CVehlDB db(m_pDoc);
	if (db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB))	MakeItemEx();
	//SetChinaAndAashtoLrfdCtrl(nIndex, sIndex);  
	SetChinaAndAashtoLrfdCtrl(m_nStndCode, nLoadType);

	double dDD = 0.0;
	if(!m_bModify) // new
	{
		dDD = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_TON, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 40.0);

		m_Data.PatchLoad = PatchLoadDB;
	}
	else //
	{
		dDD = m_Data.dDD;

		if (m_Data.PatchLoad.aPatchLoad.GetSize() == 0)
		{
			m_Data.PatchLoad = PatchLoadDB;
		}
	}

	m_edtDD.SetEditUnit(dDD);

	BOOL bEnableIncreAxleSpace = m_Data.bIncreAxleSpace;
	m_chkIncreAxleSpace.SetCheck(bEnableIncreAxleSpace);
	m_edtIncreAxleSpace.SetEditUnit(m_Data.dIncreAxleSpace);

	m_nDynaType = m_Data.nDynaFactor;
	m_edt1Axle.SetEditUnit(m_Data.dDynaFact1Axle);
	m_edt2Axle.SetEditUnit(m_Data.dDynaFact2Axle);
	m_edt3Axle.SetEditUnit(m_Data.dDynaFact3Axle);

	m_edtJTGCrowd.SetEditUnit(m_Data.dJTGCrowdWidth);
	m_edtCJJ11wp.SetEditUnit(m_Data.dCJJ11CrowdWidth);
	
	m_wndThreeGrid[0].SetDataSource(&m_Data,this,0,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(m_Data,0),nLoadType);
	m_wndThreeGrid[1].SetDataSource(&m_Data,this,1,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(m_Data,1),nLoadType);

	CArray<UINT,UINT> aRadio;
	aRadio.Add(IDC_CMD_MVHL_AUTO_RDO);
	aRadio.Add(IDC_CMD_MVHL_USER_RDO);
	m_wndDynamic.SetCheck(m_Data.bDynamicFactor);
	int nRdo = (m_Data.bUserInput)?1:0;
	CDlgUtil::CtrlRadioSetCheck(this,aRadio,nRdo);
	CString strTemp;
	strTemp.Format(_T("%g"),m_Data.dAmplification);
	m_wndPhi.SetWindowText(strTemp);

	aRadio.RemoveAll();
	aRadio.Add(IDC_OVERLOAD_FACTOR_AUTO_RDO);
	aRadio.Add(IDC_OVERLOAD_FACTOR_USER_RDO);
	m_chkOverLoadFactor.SetCheck(m_Data.bOverLoad);  
	CDlgUtil::CtrlRadioSetCheck(this, aRadio, m_Data.nOverLoad);
	m_edtOverCriAxle.SetEditUnit(m_Data.dCriticalAxle);
	m_edtOverOtherAxle.SetEditUnit(m_Data.dOtherAxle);

	aRadio.RemoveAll();
	aRadio.Add(IDC_VEHICLE_SPEED_NORMAL_RDO);
	aRadio.Add(IDC_VEHICLE_SPEED_USER_RDO);
	CDlgUtil::CtrlRadioSetCheck(this, aRadio, m_Data.nVehiSpeed);

	m_chkAddData.SetCheck(m_Data.bAddDataBD2101);

	m_edtaL.SetEditUnit(m_Data.dAddDataaL);
	m_cbxCategory.SetCurSel(m_Data.nAddDataCategory);
	m_cbxLoadLevel.SetCurSel(m_Data.nAddDataLoadLevel); 

	if (m_nStndCode == D_MVHL_TURKEY)
	{
		m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
	}
	else if (m_nStndCode == D_MVHL_AASHTO_LRFD)
	{
		m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
	}
	else if (m_nStndCode == D_MVHL_COLOMBIA) {
		if (IsPermit(m_nStndCode, nLoadType))
		{
			m_edtLegalW.SetEditUnit(m_Data.dCombinedUniformLoad);
		}
		else
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
	}
	else {
		m_edtLegalW.SetEditUnit(m_Data.dCombinedUniformLoad);
	}

	m_edtTdmW.SetEditUnit(m_Data.dUniformLoad);
	m_edtTdmD2.SetEditUnit(m_Data.dD2);
	m_edtTdmD3.SetEditUnit(m_Data.dD3);

	if (m_nStndCode == D_MVHL_AASHTO_LRFD)
	{
		m_edtLegalR.SetEditUnit(90);
	}
	else if (m_nStndCode == D_MVHL_TURKEY)
	{
		m_edtLegalR.SetEditUnit(90);
	}
	else if (m_nStndCode == D_MVHL_COLOMBIA)
	{
		if (IsPermit(m_nStndCode, nLoadType))
		{
			m_edtLegalR.SetEditUnit(75);
		}
		else
		{
			m_edtLegalR.SetEditUnit(100);
		}
	}
	else if (m_nStndCode == D_MVHL_AASHTO_ILDOT && nLoadType == 22)
	{
		m_edtLegalR.SetEditUnit(100);
	}
	else if (m_nStndCode == D_MVHL_MINNESOTA && nLoadType == 3)
	{
		m_edtLegalR.SetEditUnit(100);
	}
	else if (m_nStndCode == D_MVHL_NORTHDAKOTA && nLoadType == 7)
	{
		m_edtLegalR.SetEditUnit(100);
	}
	else if (m_nStndCode == D_MVHL_SOUTHDAKOTA && nLoadType == 5)
	{
		m_edtLegalR.SetEditUnit(100);
	}
	else
	{
		m_edtLegalR.SetEditUnit(75);
	}

	//m_edtLegalDist.SetEditUnit(m_Data.dCombinedTruckSpace);
	CString strLegalDist;
	if(m_nStndCode == D_MVHL_COLOMBIA && nLoadType == 1)
	{
		double d8 =  m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 8.0);
		double d12 = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 12.0);

		strLegalDist.Format(_T("%g~%g"), d8, d12);
	}
	else
	{
		strLegalDist.Format(_T("%g"), m_Data.dCombinedTruckSpace); 
	}

	m_edtLegalDist.SetWindowText(strLegalDist);

	// V955
	{
		if (m_nStndCode == D_MVHL_AASHTO_PENDOT)
		{
			m_edtLegalW.SetEditUnit(m_Data.dCombinedUniformLoad);
			m_edtLegalR.SetEditUnit(100);
		}

		if (m_nStndCode == D_MVHL_AASHTO_PENDOT && nLoadType == 1)
		{
			double d26 = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, 26.0);
			double d40 = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, 40.0);

			strLegalDist.Format(_T("%g~%g"), d26, d40);
			m_edtLegalDist.SetWindowText(strLegalDist);
		}

		if (m_nStndCode == D_MVHL_AASHTO_PENDOT && nLoadType == 0)
		{
			double d50 = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, 50.0);
			CString strPenndotDist;
			strPenndotDist.Format(_T("%g"), d50);

			m_edtPenndotW.SetEditUnit(m_Data.dCombinedUniformLoad);
			m_edtPenndotR1.SetEditUnit(90.0);
			m_edtPenndotR2.SetEditUnit(100.0);
			m_edtPenndotDist.SetWindowText(strPenndotDist);
		}

		if (m_Data.PatchLoad.aPatchLoad.GetSize() > 0)
		{
			m_chkPatch.SetCheck(m_Data.PatchLoad.bPatchLoad);
			m_edtPatchW.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dWidth);
			m_edtPatchL.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dLength);
		}
		else
		{
			m_edtPatchW.SetEditUnit(0.0);
			m_edtPatchW.SetEditUnit(0.0);
		}

		OnChkPatch();
	}

	m_edtz.SetEditUnit(m_Data.dImpactCoef);

	m_chkCentF.SetCheck(m_Data.bCentF);

	m_chkTDM.SetCheck(m_Data.bTDM);

	ChangeCtrlText();
	OnCmdMvhlEuroDynamicChk();
	OnCmdMvhlOverLoadChk();
	ChangeBitmap(m_nStndCode, nLoadType);

	UpdateData(FALSE);
}

void CCMMvhlItemStdDlg::SetChinaAndAashtoLrfdCtrl(int nIndex, int sIndex) 
{
	BOOL bChina  = FALSE; // China(CJJ77-98)  
	BOOL bLrfd   = FALSE;
	BOOL bLrfdCF = FALSE;
	BOOL bTDMType = (m_Data.VehicleTypeName == _T("HL-93TDM") || m_Data.VehicleTypeName == _T("KYHL-93TDM")) ? TRUE : FALSE;
	BOOL bCanada = FALSE; // Canada(CAN/CSA-S6-00)
	BOOL bBS     = FALSE; // BS(BS5400,BS3701)
	BOOL bBS5400 = FALSE; // BS(BS5400,BS3701)
	BOOL bBS3701 = FALSE; // BS(BS5400,BS3701)
	BOOL bCS454  = FALSE; // BS(CS 454 Assessment)
	BOOL bBS_NumUnit = FALSE;
	BOOL bBS_dd = FALSE;
	BOOL bBS_Ped = FALSE;
	BOOL bBS_HA = FALSE;
	BOOL bBS_HB = FALSE;
	BOOL bBS_GroupBox = FALSE; 
	BOOL bBS_LaneFactor = FALSE;
	BOOL bBS_CS_LaneFactor = FALSE;
	BOOL bBS_Only = FALSE;
	BOOL bBS_TT = FALSE;

	BOOL bChinaJTGLane  = FALSE;  // China(JTG B01-2003) Lane Load
	BOOL bChinaJTGTruck = FALSE;  // China(JTG B01-2003) Truck Load
	BOOL bChinaJTGCrowd = FALSE;  // China(JTG B01-2003) Crowd Load

	BOOL bChinaCJJ11Lane  = FALSE;  // China(CJJ11-2011) Lane Load
	BOOL bChinaCJJ11Truck = FALSE;  // China(CJJ11-2011) Truck Load
	BOOL bChinaCJJ11Crowd = FALSE;  // China(CJJ11-2011) Crowd Load

	BOOL bDyna = FALSE;

	BOOL bBS_BD8611 = FALSE; // BS BD86/11
	BOOL bSoSov = FALSE;

	BOOL bDD = FALSE;
	BOOL bTurkey = FALSE;
	
	BOOL bDescription1 = (m_Data.VehicleTypeName == _T("IL-PS2-21") || m_Data.VehicleTypeName == _T("IL-PS3-31") ||
										m_Data.VehicleTypeName == _T("IL-PS4-34.75") || m_Data.VehicleTypeName == _T("IL-PC4-41") ||
										m_Data.VehicleTypeName == _T("IL-PC5-41") || m_Data.VehicleTypeName == _T("HST-18") || m_Data.VehicleTypeName == _T("P2016-13"));

	BOOL bDescription2 = (m_Data.VehicleTypeName == _T("IL-PS2-21") || m_Data.VehicleTypeName == _T("IL-PS3-31") ||
											m_Data.VehicleTypeName == _T("IL-PS4-28") || m_Data.VehicleTypeName == _T("IL-PS4-34.75") ||
											m_Data.VehicleTypeName == _T("IL-PS5-36") || m_Data.VehicleTypeName == _T("IL-PS6-35.75") ||
											m_Data.VehicleTypeName == _T("IL-PS7-39.75") || m_Data.VehicleTypeName == _T("IL-PS3-31") ||
											m_Data.VehicleTypeName == _T("IL-PC4-41") || m_Data.VehicleTypeName == _T("IL-PC5-41") ||
											m_Data.VehicleTypeName == _T("IL-PD6-40") || m_Data.VehicleTypeName == _T("IL-PD6-200") ||
											m_Data.VehicleTypeName == _T("P2016-13"));

	BOOL bCenF = FALSE;

	switch(nIndex)
	{
	case D_MVHL_CH_JTJ001_97: case D_MVHL_CH_CJJ77_98: case D_MVHL_CH_TB100021_99: case D_MVHL_CH_TB10002_2017: case D_MVHL_CH_QCR9300_2018:
		bChina = TRUE;
		break;
	case D_MVHL_AASHTO_LRFD:  case D_MVHL_AASHTO_PENDOT: case D_MVHL_AASHTO_LRFD_LEGAL:
	case D_MVHL_AASHTO_IADOT: case D_MVHL_AASHTO_ILDOT:  case D_MVHL_AASHTO_LADOT:
	case D_MVHL_AASHTO_MODOT: case D_MVHL_AASHTO_OHDOT:  case D_MVHL_AASHTO_RIDOT:
	case D_MVHL_AASHTO_VADOT: case D_MVHL_AASHTO_WIDOT:  case D_MVHL_AASHTO_INDOT:
	case D_MVHL_COLOMBIA:     case D_MVHL_FAST_ACT_EV_LOAD: case D_MVHL_ALABAMA:
	case D_MVHL_ARIZONA: case D_MVHL_CALIFORNIA: case D_MVHL_COLORADO:
	case D_MVHL_CONNECTICUT: case D_MVHL_FLORIDA: case D_MVHL_GEORGIA:
	case D_MVHL_HAWAII: case D_MVHL_IDAHO: case D_MVHL_KANSAS: case D_MVHL_KENTUCKY:
	case D_MVHL_MAINE: case D_MVHL_MARYLAND: case D_MVHL_MASSACHUSETTS:
	case D_MVHL_MICHIGAN_NL: case D_MVHL_MICHIGAN_DL: case D_MVHL_MICHIGAN_SD:
	case D_MVHL_MINNESOTA: case D_MVHL_MISSISSIPI: case D_MVHL_NEBRASKA:
	case D_MVHL_NEWJERSEY: case D_MVHL_NEWMEXICO: case D_MVHL_NEWYORK:
	case D_MVHL_NORTHDAKOTA: case D_MVHL_OKLAHOMA: case D_MVHL_OREGON:
	case D_MVHL_SOUTHCAROLINA: case D_MVHL_SOUTHDAKOTA: case D_MVHL_TENNESSEE:
	case D_MVHL_TEXAS: case D_MVHL_UTAH: case D_MVHL_VERMONT: case D_MVHL_WASHINGTON:
	case D_MVHL_WESTVIRGINIA: case D_MVHL_DELAWARE:
	case D_MVHL_ARKANSAS:
		bLrfd = TRUE;
		break;
	case D_MVHL_TURKEY:
		if(m_nMovingType == D_MOVE_CODE_AASHTO_STAN)
		{
			if(sIndex==4) bTurkey = TRUE;
		}
		else
		{
			if(sIndex==0) bTurkey = TRUE;
		}		
		break;
	case D_MVHL_CANCSA_S600:
		bCanada = TRUE;
		bDyna = TRUE;
		break;
	case D_MVHL_BS_5400: 
		bBS5400 = TRUE;
		bBS = TRUE;
		bBS_NumUnit = TRUE;
		break;
	case D_MVHL_BS_BD3701:
		bBS3701 = TRUE;
		bBS = TRUE;
		bBS_Only = TRUE;
		if (sIndex == 0) bBS_HA = bBS_GroupBox = bBS_LaneFactor = TRUE;       // HA
		else if (sIndex == 1) bBS_HB = bBS_NumUnit = TRUE;  // HB
		else if (sIndex == 2) bBS_HA = bBS_HB = bBS_dd = bBS_GroupBox = bBS_LaneFactor = bBS_NumUnit = TRUE;  // HA & HB
		else if (sIndex == 3) bBS_HA = bBS_HB = bBS_GroupBox = bBS_LaneFactor = bBS_NumUnit = TRUE;  // HA & HB(Auto)  
		else if (sIndex == 4) bBS_Ped = TRUE;  // Pedestrian
		break;
	case D_MVHL_BS_CS454: 
		bCS454 = TRUE;
		bBS_CS_LaneFactor = TRUE;
		bBS = TRUE;
		break;

	case D_MVHL_CH_JTGB01_2003:
	case D_MVHL_CH_JTGB01_2014:
		{
			if     (sIndex==0) bChinaJTGLane = TRUE;
			else if(sIndex==1) bChinaJTGTruck = TRUE;
			else if(sIndex==2) bChinaJTGCrowd = TRUE;
			else if(sIndex==3) bChinaJTGLane = TRUE;
			else if(sIndex==4) bChinaJTGTruck = bDD = TRUE;
			else if(sIndex==5) bChinaJTGTruck = TRUE;
		}
		break;
	case D_MVHL_CH_CJJ11_2011: 
	case D_MVHL_CH_CJJ11_2019: 
		{
			if     (sIndex==0) bChinaCJJ11Truck = TRUE;
			else if(sIndex==1) bChinaCJJ11Truck = TRUE;
			else if(sIndex==2) bChinaCJJ11Lane = TRUE;
			else if(sIndex==3) bChinaCJJ11Crowd = TRUE;
			else if(sIndex==4) bChinaCJJ11Truck = TRUE;
			else if(sIndex==5) bChinaCJJ11Truck = TRUE;
			else if(sIndex==6) bChinaCJJ11Truck = TRUE;
			else if(sIndex==7) bChinaCJJ11Truck = TRUE;
		}
		break;
	case D_MVHL_BS_BD8611:
		{
			if(sIndex==5) bBS_TT = TRUE;

			bBS_BD8611 = TRUE;
		}
		break;
	}

	switch(nIndex)
	{
	case D_MVHL_AASHTO_LRFD:  case D_MVHL_AASHTO_LRFD_LEGAL: /*case D_MVHL_AASHTO_LEGAL:*/
	case D_MVHL_AASHTO_IADOT: case D_MVHL_AASHTO_ILDOT:  case D_MVHL_AASHTO_LADOT:
	case D_MVHL_AASHTO_MODOT: case D_MVHL_AASHTO_OHDOT:  case D_MVHL_AASHTO_RIDOT:
	case D_MVHL_AASHTO_VADOT: case D_MVHL_AASHTO_WIDOT:  case D_MVHL_AASHTO_INDOT:
	case D_MVHL_ALABAMA:	case D_MVHL_ARIZONA:	case D_MVHL_CALIFORNIA:	case D_MVHL_COLORADO:
	case D_MVHL_CONNECTICUT:	case D_MVHL_FLORIDA:	case D_MVHL_GEORGIA:	case D_MVHL_HAWAII:
	case D_MVHL_IDAHO:	case D_MVHL_KANSAS:	case D_MVHL_KENTUCKY:	case D_MVHL_DISTRICT_OF_COLUMBIA:
	case D_MVHL_MAINE:	case D_MVHL_MARYLAND:	case D_MVHL_MASSACHUSETTS:	case D_MVHL_MICHIGAN_NL:
	case D_MVHL_MICHIGAN_DL:	case D_MVHL_MICHIGAN_SD:	case D_MVHL_MINNESOTA:	case D_MVHL_MISSISSIPI:
	case D_MVHL_NEBRASKA:	case D_MVHL_NEWJERSEY:	case D_MVHL_NEWMEXICO:	case D_MVHL_NEWYORK:
	case D_MVHL_NORTHDAKOTA:	case D_MVHL_OKLAHOMA:	case D_MVHL_OREGON:	case D_MVHL_SOUTHCAROLINA:
	case D_MVHL_SOUTHDAKOTA:	case D_MVHL_TENNESSEE:	case D_MVHL_TEXAS:	case D_MVHL_UTAH:
	case D_MVHL_VERMONT:	case D_MVHL_WASHINGTON:	case D_MVHL_WESTVIRGINIA:	case D_MVHL_DELAWARE:
	case D_MVHL_NEVADA:	case D_MVHL_ARKANSAS:	case D_MVHL_ALASKA:
		bLrfdCF = TRUE;
		break;
	}
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlAashtoLrfd, bLrfd || bTurkey);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCom,  !bChina && !bCanada && !bBS && !bBS_BD8611 && !bChinaJTGLane && !bChinaJTGTruck && !bChinaJTGCrowd && !bChinaCJJ11Lane && !bChinaCJJ11Truck && !bChinaCJJ11Crowd); 
	CDlgUtil::CtrlShowHide(this, m_aCtrlCh,         bChina);
	CDlgUtil::CtrlShowHide(this, m_aCtrlJTGLane,  bChinaJTGLane || bChinaCJJ11Lane);
	CDlgUtil::CtrlShowHide(this, m_aCtrlJTGTruck, bChinaJTGTruck || bChinaCJJ11Truck);
	CDlgUtil::CtrlShowHide(this, m_aCtrlJTGCrowd, bChinaJTGCrowd);
	CDlgUtil::CtrlShowHide(this, m_aCJJ11Crowd,   bChinaCJJ11Crowd);
	CDlgUtil::CtrlShowHide(this, m_aCtrlIrc1,      !bCanada && !bBS && !bBS_BD8611 && !bChinaJTGLane && !bChinaJTGTruck && !bChinaJTGCrowd && !bChinaCJJ11Lane && !bChinaCJJ11Truck && !bChinaCJJ11Crowd && !bTurkey); 
	CDlgUtil::CtrlShowHide(this, m_aCtrlDD,         bDD);

	if(bBS_BD8611 || bBS || bChinaJTGLane || bChinaJTGCrowd || bChinaCJJ11Lane || bChinaCJJ11Crowd)
		GetDlgItem(IDC_CMD_MVHL_PTLIST)->ShowWindow(FALSE);
	else 
		GetDlgItem(IDC_CMD_MVHL_PTLIST)->ShowWindow(TRUE);
	
	// Dynamic Load Allowance 
	CDlgUtil::CtrlShowHide(this, m_aDyna, bDyna);
	
	// BS BD86/11
	CDlgUtil::CtrlShowHide(this, m_aCtrlSoSov,      bBS_BD8611);
	GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->ShowWindow(FALSE);

	bCenF = (bLrfdCF || bCanada);
	// Centrifugal Force
	GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->ShowWindow(bCenF);

	GetDlgItem(IDC_CMD_MVHL_TDM_CHK)->ShowWindow(bTDMType);
	
	GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION1)->ShowWindow(bDescription1);
	GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION2)->ShowWindow(bDescription2);

	CRect rectPosVehiLoadPro;
	if(bBS_BD8611)//IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1의 크기를 조절해야 할 경우 이 if문을 수정 하시오..
	{
		rectPosVehiLoadPro = m_rectPosVehiLoadProBS_BD8611;
	}
	else
	{ 
		rectPosVehiLoadPro = m_rectPosVehiLoadProCommon;
	}

	if(!bCanada)
	{
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->SetWindowPos(NULL, rectPosVehiLoadPro.left, rectPosVehiLoadPro.top, rectPosVehiLoadPro.Width(), rectPosVehiLoadPro.Height(), SWP_SHOWWINDOW);  
	}    

	if(bCanada)
	{
		GetDlgItem(IDC_CMD_MVHL_W_STATIC)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_MVHL_W)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_MVHL_W_UNIT)->ShowWindow(TRUE);
	}
	else if(bBS)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlJTGLane, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlJTGCrowd, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlAashtoLrfd, FALSE);
	}
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlBSAddData, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aCtrlBSCSAddData, FALSE);

	CDlgUtil::CtrlShowHide(this, m_aCtrlBS, bBS && !bBS_Ped);
	CDlgUtil::CtrlShowHide(this, m_aCtrlBS5400, bBS5400);
	CDlgUtil::CtrlShowHide(this, m_aCtrlBS3701, bBS3701 && !bBS_Ped);

	CDlgUtil::CtrlShowHide(this, m_aGroupBox2,           bBS_GroupBox);
	CDlgUtil::CtrlShowHide(this, m_aBSBD3701LaneFactor,  bBS_LaneFactor);
	CDlgUtil::CtrlShowHide(this, m_aBSCS454LaneFactor,   bBS_CS_LaneFactor);

	if(m_Data.VehicleTypeName == _T("CH-ZH-SL") || m_Data.VehicleTypeName == _T("CH-ZH-NL"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlz, SW_SHOW);
	}
	else 
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlz, SW_HIDE);
	}	

	GetDlgItem(IDC_CMD_MVHL_BS_CS_CATEGORY_TEMP_STC2)->ShowWindow(FALSE);

	CRect rRef, rMove, rToMove, r;
	int nDistY, nDistX;
	if (bCS454)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlBS_HA, bBS_HA);
		CDlgUtil::CtrlShowHide(this, m_aCtrlBS_HB, bBS_HB); 

		CDlgUtil::CtrlShowHide(this, m_aCtrlBS_Pa, FALSE); // HA or HA & HB(Auto)    
		
		CDlgUtil::CtrlShowHide(this, m_aGroupBox1, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aGroupBox2, TRUE);
		
		CDlgUtil::CtrlShowHide(this, m_aCtrlBSAddData, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlBSCSAddData, TRUE);

		GetDlgItem(IDC_CMD_MVHL_BS_CS_LANE_FACT_USER_STC)->EnableWindow(m_nLaneFactorCS == 1);
		GetDlgItem(IDC_CMD_MVHL_BS_CS_LANE_FACT_USER_BTN)->EnableWindow(m_nLaneFactorCS == 1);	

		// Add Data
		GetDlgItem(IDC_CMD_MVHL_PIC_CS_WND)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_BS_CS_CATEGORY_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(6);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlBSCSAddData, nDistY);
	
		GetDlgItem(IDC_CMD_MVHL_BS_CS_CATEGORY_TEMP_STC)->ShowWindow(FALSE);
		CDlgUtil::CtrlShowHide     (this, m_aBSCS454LaneFactorCmb,   TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aBSCS454LaneFactorCmb,   TRUE);
		GetDlgItem(IDC_CMD_MVHL_BS_CS_CATEGORY_TEMP_STC)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aBSCS454LaneFactorCmb, nDistX, nDistY);

		int nMarginY = 134;
#if defined (_CIVIL_RUS)
		nMarginY = 144;
#endif
		// Dlg Size 
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES2)->GetWindowRect(rMove);
		SetWindowPos(NULL, rMove.left, rMove.top, rMove.Width()+globalUtils.ScaleByDPI(16), rMove.Height()+nMarginY, SWP_NOMOVE);

		// Move OK, Cancel, Apply      
		GetDlgItem(IDOK)->GetWindowRect(r);
		nDistY = rMove.bottom-r.top + globalUtils.ScaleByDPI(12);
		CDlgUtil::CtrlMoveDistY(this, m_aOKCancelApply, nDistY);

	}
	else if (bBS3701)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlBS_HA, bBS_HA);
		CDlgUtil::CtrlShowHide(this, m_aCtrlBS_HB, bBS_HB); 
		if (sIndex == 1 || sIndex == 2) // HB or HA & HB
		{
			// Pa 숨기고, Pb를 왼쪽으로
			CDlgUtil::CtrlShowHide(this, m_aCtrlBS_Pa, FALSE);      
			GetDlgItem(m_aCtrlBS_Pa[0])->GetWindowRect(rRef);
			GetDlgItem(m_aCtrlBS_Pb[0])->GetWindowRect(rToMove);
			int nDistX = rRef.left - rToMove.left;
			CDlgUtil::CtrlMoveDistX(this, m_aCtrlBS_Pb, nDistX);
		}
		else
		{
			// Pa 나태내고 Pb 오른쪽으로
			CDlgUtil::CtrlShowHide(this, m_aCtrlBS_Pa, sIndex == 0 || sIndex == 3); // HA or HA & HB(Auto)      
			GetDlgItem(IDC_CMD_MVHL_BS_DD_UNIT)->GetWindowRect(rRef);
			GetDlgItem(m_aCtrlBS_Pb[2])->GetWindowRect(rToMove);
			int nDistX = rRef.left - rToMove.left;
			CDlgUtil::CtrlMoveDistX(this, m_aCtrlBS_Pb, nDistX);
		}
		if (sIndex == 1)  // HB를 위로
		{
			GetDlgItem(IDC_CMD_MVHL_BS37_W1_STATIC)->GetWindowRect(rRef);
			GetDlgItem(m_aCtrlBS_Pb[0])->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_HB, nDistY);
		}
		else  // HB를 아래로(위에 가 있을 수 있기 때문)
		{
			GetDlgItem(m_aCtrlBS_Pa[0])->GetWindowRect(rRef);
			GetDlgItem(m_aCtrlBS_Pb[0])->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_HB, nDistY);
		}

		CDlgUtil::CtrlEnableDisable(this, m_aBSCS454LaneFactorCmb,   FALSE);
		GetDlgItem(IDC_CMD_MVHL_BS_CS_CATEGORY_TEMP_STC2)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aBSCS454LaneFactorCmb, nDistX, nDistY);

		// BS BD37/01 Lane Factor 추가 하면서 삽입된 코드 2009.06.05
		if(sIndex == 0 || sIndex == 2 || sIndex == 3)
		{
			// Vehicular Load Properties Group Box Show/Hide
			CDlgUtil::CtrlShowHide(this, m_aGroupBox1, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aGroupBox2, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlBSAddData, TRUE);

			int nMarginY = 134;
#if defined (_CIVIL_RUS)
			nMarginY = 144;
#endif

			// Dlg Size 
			GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES2)->GetWindowRect(rMove);
			SetWindowPos(NULL, rMove.left, rMove.top, rMove.Width()+globalUtils.ScaleByDPI(16), rMove.Height()+nMarginY, SWP_NOMOVE);

			// Move OK, Cancel, Apply      
			GetDlgItem(IDOK)->GetWindowRect(r);
			nDistY = rMove.bottom-r.top + globalUtils.ScaleByDPI(12);
			CDlgUtil::CtrlMoveDistY(this, m_aOKCancelApply, nDistY);

			// Lane Factor Move
			GetDlgItem(IDC_CMD_MVHL_LANE_FACTOR)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_BS37_W1_EDIT)->GetWindowRect(rMove);
			nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS3701, nDistY);

			if(sIndex == 0) // HA
			{
				// Pa Move
				GetDlgItem(IDC_CMD_MVHL_BS37_W3_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_PA_EDIT)->GetWindowRect(rMove);
				nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(4);
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_Pa, nDistY);

				// Add Data
				GetDlgItem(IDC_CMD_MVHL_BS_PA_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_ADD_DATA_CHK)->GetWindowRect(rMove);
				nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(6);
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBSAddData, nDistY);
			}
			else if(sIndex == 2) // HA & HB
			{
				// D 관련 값 Move
				GetDlgItem(IDC_CMD_MVHL_BS37_W3_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_PA_EDIT)->GetWindowRect(rMove);
				nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(4);
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS, nDistY);

				// No. of Units Move
				GetDlgItem(IDC_CMD_MVHL_BS37_W3_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_UNIT_NUM_EDIT)->GetWindowRect(rMove);
				nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(4);
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_UnitNum, nDistY);

				// Add Data
				GetDlgItem(IDC_CMD_MVHL_BS_D4_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_ADD_DATA_CHK)->GetWindowRect(rMove);
				nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(6);
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBSAddData, nDistY);
			}
			else // HA & HB (Auto)
			{
				// D 관련 값 Move
				GetDlgItem(IDC_CMD_MVHL_BS37_W3_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_PA_EDIT)->GetWindowRect(rMove);
				nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(4);
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS, nDistY);

				// Pb Move
				GetDlgItem(IDC_CMD_MVHL_BS37_W3_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_PB_EDIT)->GetWindowRect(rMove);
				nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(4);
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_Pb, nDistY);
				
				// No. of Units Move
				GetDlgItem(IDC_CMD_MVHL_BS_PB_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_UNIT_NUM_EDIT)->GetWindowRect(rMove);
				nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(4);
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_UnitNum, nDistY);

				// Add Data
				GetDlgItem(IDC_CMD_MVHL_BS_D4_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_ADD_DATA_CHK)->GetWindowRect(rMove);
				nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(6);
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBSAddData, nDistY);
			}
		}
		else
		{      
			// Vehicular Load Properties Group Box Show/Hide
			CDlgUtil::CtrlShowHide(this, m_aGroupBox1, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aGroupBox2, FALSE);      

			int nMarginY = 134;
#if defined (_CIVIL_RUS)
			nMarginY = 144;
#endif

			// Dlg Size 
			GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->GetWindowRect(rMove);
			SetWindowPos(NULL, rMove.left, rMove.top, rMove.Width()+globalUtils.ScaleByDPI(16), rMove.Height()+nMarginY, SWP_NOMOVE);

			GetDlgItem(IDOK)->GetWindowRect(r);
			nDistY = rMove.bottom-r.top + globalUtils.ScaleByDPI(12);
			CDlgUtil::CtrlMoveDistY(this, m_aOKCancelApply, nDistY);

			if(sIndex == 1) // HB
			{
				GetDlgItem(IDC_CMD_MVHL_W)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_PB_EDIT)->GetWindowRect(rMove);
				nDistY = rRef.top-rMove.top;
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_Pb, nDistY);

				GetDlgItem(IDC_CMD_MVHL_BS_PB_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_D1_EDIT)->GetWindowRect(rMove);
				nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(8);
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS, nDistY);

				GetDlgItem(IDC_CMD_MVHL_BS_D1_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_UNIT_NUM_EDIT)->GetWindowRect(rMove);
				nDistY = rRef.top-rMove.top;
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_UnitNum, nDistY);

				GetDlgItem(IDC_CMD_MVHL_BS_D1_EDIT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_BS_PB_EDIT)->GetWindowRect(rMove);
				nDistY = rRef.top-rMove.bottom - globalUtils.ScaleByDPI(4);
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_Pb, nDistY);
			}
		}
	}  
	else // BS5400
	{
		CRect rRef, rToMove, rMove, r;
		CDlgUtil::CtrlShowHide(this, m_aGroupBox1, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aGroupBox2, FALSE);

		int nMarginY = 124;
#if defined (_CIVIL_RUS)
		nMarginY = 134;
#endif

		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->GetWindowRect(rMove);
		SetWindowPos(NULL, rMove.left, rMove.top, rMove.Width()+globalUtils.ScaleByDPI(16), rMove.Height()+nMarginY, SWP_NOMOVE);

		GetDlgItem(IDOK)->GetWindowRect(r);
		int nDistY = rMove.bottom-r.top + globalUtils.ScaleByDPI(6);
		CDlgUtil::CtrlMoveDistY(this, m_aOKCancelApply, nDistY);

		// Pb 오른쪽으로
		GetDlgItem(IDC_CMD_MVHL_BS_DD_UNIT)->GetWindowRect(rRef);
		GetDlgItem(m_aCtrlBS_Pb[2])->GetWindowRect(rToMove);
		int nDistX = rRef.left - rToMove.left;
		CDlgUtil::CtrlMoveDistX(this, m_aCtrlBS_Pb, nDistX);
		// HB 아래로
		GetDlgItem(m_aCtrlBS_Pa[0])->GetWindowRect(rRef);
		GetDlgItem(m_aCtrlBS_Pb[0])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_HB, nDistY);
	}
	
	if(bBS5400)
	{
		CRect rRef, rMove, r;

		GetDlgItem(IDC_CMD_MVHL_BS_W3_EDIT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_BS_PA_EDIT)->GetWindowRect(rMove);
		int nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(4);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS, nDistY);
	}

	if (bBS_NumUnit && !bBS_Only)
	{
		if (bBS3701 && sIndex == 2) // BS BD37/01 HA & HB (Pb 위치에)
		{
			CRect rRef, rToMove;
			GetDlgItem(m_aCtrlBS_Pb[0])->GetWindowRect(rRef);
			GetDlgItem(m_aCtrlBS_UnitNum[0])->GetWindowRect(rToMove);
			int nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_UnitNum, nDistY);
		}
		else if (!bBS5400)
		{
			CRect rRef, rToMove;
			GetDlgItem(m_aCtrlBS_dd[0])->GetWindowRect(rRef);
			GetDlgItem(m_aCtrlBS_UnitNum[0])->GetWindowRect(rToMove);
			int nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_UnitNum, nDistY);
		}
	}
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlBS_dd, bBS_dd);
	CDlgUtil::CtrlShowHide(this, m_aCtrlBS_UnitNum, bBS_NumUnit);
	CDlgUtil::CtrlShowHide(this, m_aCtrlBS_Ped, bBS_Ped);

	GetDlgItem(IDC_CMD_MVHL_SVSOV_A_STC)->ShowWindow(bBS_BD8611 && !bBS_TT);

	BOOL bPennotLegal = FALSE;
	// V955
	{
		bPennotLegal = (m_nMovingType == D_MOVE_CODE_PENDOT && IsLegal(nIndex, sIndex));
	}

	BOOL bPennotLegalTRK = (bPennotLegal && sIndex == 0) ? TRUE : FALSE;
	CDlgUtil::CtrlShowHide(this, m_aCtrlPenndot, bPennotLegalTRK);

	m_bPatchLoad = FALSE;
	if (m_pDoc->m_pAttrCtrl->IsPatchLoadVehicle(m_Data))
	{
		m_bPatchLoad = TRUE;
	}

	CDlgUtil::CtrlShowHide(this, m_aCtrlPatch, m_bPatchLoad);

	if(m_nMovingType == D_MOVE_CODE_KOREA || m_nMovingType == D_MOVE_CODE_AASHTO_LRFD || m_nMovingType == D_MOVE_CODE_AASHTO_STAN || bPennotLegal)
	{
		BOOL bLegal = IsLegal(nIndex, sIndex);
		BOOL bPermit = IsPermit(nIndex, sIndex);
		BOOL bTDMType = (m_Data.VehicleTypeName == _T("HL-93TDM") || m_Data.VehicleTypeName == _T("KYHL-93TDM")) ? TRUE : FALSE;

		GetDlgItem(IDC_CMD_MVHL_LEGAL_W_STATIC)->ShowWindow(!bPennotLegalTRK && (bLegal || bPermit));
		GetDlgItem(IDC_CMD_MVHL_LEGAL_W)->ShowWindow(!bPennotLegalTRK && (bLegal || bPermit));
		GetDlgItem(IDC_CMD_MVHL_LEGAL_W_UNIT)->ShowWindow(!bPennotLegalTRK && (bLegal || bPermit));
		GetDlgItem(IDC_CMD_MVHL_LEGAL_R_STATIC)->ShowWindow(!bPennotLegalTRK && bLegal);
		GetDlgItem(IDC_CMD_MVHL_LEGAL_R)->ShowWindow(!bPennotLegalTRK && bLegal);
		GetDlgItem(IDC_CMD_MVHL_LEGAL_R_UNIT)->ShowWindow(!bPennotLegalTRK && bLegal);
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DIST_STATIC)->ShowWindow(!bPennotLegalTRK && bLegal);
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DIST)->ShowWindow(!bPennotLegalTRK && bLegal);
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DIST_UNIT)->ShowWindow(!bPennotLegalTRK && bLegal);		

		GetDlgItem(IDC_CMD_MVHL_W_STATIC)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_W)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_W_UNIT)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_Ps_STATIC)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_PLH)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_Ps_UNIT)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_Pm_STATIC)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_PLV)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_Pm_UNIT)->ShowWindow(!bLegal && !bPermit);    
		GetDlgItem(IDC_CMD_MVHL_dW1_STATIC)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_dW1)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_dW1_UNIT)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_dD1_STATIC)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_dD1)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_dD1_UNIT)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_dW2_STATIC)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_dW2)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_dW2_UNIT)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_dD2_STATIC)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_dD2)->ShowWindow(!bLegal && !bPermit);
		GetDlgItem(IDC_CMD_MVHL_dD2_UNIT)->ShowWindow(!bLegal && !bPermit);

		GetDlgItem(IDC_CMD_MVHL_TDM_W_STATIC )->ShowWindow(bTDMType);
		GetDlgItem(IDC_CMD_MVHL_TDM_W        )->ShowWindow(bTDMType);
		GetDlgItem(IDC_CMD_MVHL_TDM_W_UNIT   )->ShowWindow(bTDMType);
		GetDlgItem(IDC_CMD_MVHL_TDM_D2_STATIC)->ShowWindow(bTDMType);
		GetDlgItem(IDC_CMD_MVHL_TDM_D2       )->ShowWindow(bTDMType);
		GetDlgItem(IDC_CMD_MVHL_TDM_D2_UNIT  )->ShowWindow(bTDMType);
		GetDlgItem(IDC_CMD_MVHL_TDM_D3_STATIC)->ShowWindow(bTDMType);
		GetDlgItem(IDC_CMD_MVHL_TDM_D3       )->ShowWindow(bTDMType);
		GetDlgItem(IDC_CMD_MVHL_TDM_D3_UNIT  )->ShowWindow(bTDMType);
		if(bTDMType)
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlCom, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlIrc1, FALSE);
		}
	}

	Invalidate(TRUE);
	UpdateWindow();
}

BOOL CCMMvhlItemStdDlg::IsPermit(int nIndex, int nLoadType)
{
	switch (nIndex)
	{
	case D_MVHL_AASHTO_IADOT:
		if(nLoadType == 0)        return TRUE;
		else if(nLoadType == 1)   return TRUE;
		else if(nLoadType == 2)   return TRUE;
		else if(nLoadType == 3)   return TRUE;
		break;
	case D_MVHL_AASHTO_ILDOT:
		if(nLoadType == 7)        return TRUE;
		else if(nLoadType == 8)   return TRUE;
		else if(nLoadType == 9)   return TRUE;
		else if(nLoadType == 10)  return TRUE;
		else if(nLoadType == 11)  return TRUE;
		break;
	case D_MVHL_AASHTO_LADOT:
		if(nLoadType == 0)        return TRUE;
		else if(nLoadType == 1)   return TRUE;
		else if(nLoadType == 2)   return TRUE;
		else if(nLoadType == 3)   return TRUE;
		else if(nLoadType == 4)   return TRUE;
		else if(nLoadType == 5)   return TRUE;
		else if(nLoadType == 6)   return TRUE;
		else if(nLoadType == 7)   return TRUE;
		break;
	case D_MVHL_AASHTO_RIDOT:
		if(nLoadType == 0)        return TRUE;
		else if(nLoadType == 1)   return TRUE;
		else if(nLoadType == 2)   return TRUE;
		else if(nLoadType == 3)   return TRUE;
		else if(nLoadType == 4)   return TRUE;
		else if(nLoadType == 5)   return TRUE;
		else if(nLoadType == 6)   return TRUE;
		else if(nLoadType == 7)   return TRUE;
		else if(nLoadType == 8)   return TRUE;
		break;
	case D_MVHL_AASHTO_VADOT:
		if(nLoadType == 2)        return TRUE;
		else if(nLoadType == 3)   return TRUE;
		break;
	case D_MVHL_AASHTO_WIDOT:
		if(nLoadType == 0)        return TRUE;
		break;
	case D_MVHL_COLOMBIA:
		if (nLoadType == 11)       return TRUE;
		break;
	case D_MVHL_ARIZONA:
		if (nLoadType == 2)        return TRUE;
		break;
	case D_MVHL_FLORIDA:
		if (nLoadType == 2)        return TRUE;
		else if (nLoadType == 3)   return TRUE;
		else if (nLoadType == 4)   return TRUE;
		else if (nLoadType == 5)   return TRUE;
		else if (nLoadType == 6)   return TRUE;
		else if (nLoadType == 7)   return TRUE;
		else if (nLoadType == 8)   return TRUE;
		else if (nLoadType == 9)   return TRUE;
		break;
	case D_MVHL_MINNESOTA:
		if (nLoadType == 4)        return TRUE;
		else if (nLoadType == 5)   return TRUE;
		else if (nLoadType == 6)   return TRUE;
		else if (nLoadType == 7)   return TRUE;
		else if (nLoadType == 8)   return TRUE;
		else if (nLoadType == 9)   return TRUE;
		else if (nLoadType == 10)  return TRUE;
		break;
	}
	return FALSE;
}

BOOL CCMMvhlItemStdDlg::IsLegal(int nIndex, int nLoadType)
{
	switch (nIndex)
	{
	case D_MVHL_AASHTO_LRFD:
		if(nLoadType == 0)        return TRUE;
		break;
	case D_MVHL_AASHTO_LEGAL:
		if(nLoadType == 2)        return TRUE;
		break;
	case D_MVHL_AASHTO_LRFD_LEGAL:
	case D_MVHL_ARKANSAS:
		if(nLoadType == 2)        return TRUE;
		break;
	case D_MVHL_AASHTO_IADOT:
		if (nLoadType == 4)        return TRUE;
		else if (nLoadType == 5)   return TRUE;
		else if (nLoadType == 6)   return TRUE;
		else if (nLoadType == 7)   return TRUE;
		else if (nLoadType == 8)   return TRUE;
		break;
	case D_MVHL_AASHTO_ILDOT:
		if(nLoadType == 0)        return TRUE;
		else if(nLoadType == 1)   return TRUE;
		else if(nLoadType == 2)   return TRUE;
		else if(nLoadType == 3)   return TRUE;
		else if(nLoadType == 4)   return TRUE;
		else if(nLoadType == 5)   return TRUE;
		else if(nLoadType == 6)   return TRUE;
		break;
	case D_MVHL_AASHTO_LADOT:
		if(nLoadType == 8)        return TRUE;
		else if(nLoadType == 9)   return TRUE;
		else if(nLoadType == 10)  return TRUE;
		else if(nLoadType == 11)  return TRUE;
		break;
	case D_MVHL_AASHTO_MODOT:
		if(nLoadType == 4)        return TRUE;
		else if(nLoadType == 5)   return TRUE;
		else if(nLoadType == 6)   return TRUE;
		else if(nLoadType == 14)  return TRUE;
		break;
	case D_MVHL_AASHTO_OHDOT:
		if(nLoadType == 0)        return TRUE;
		else if(nLoadType == 1)   return TRUE;
		else if(nLoadType == 2)   return TRUE;
		else if(nLoadType == 3)   return TRUE;
		break;
	case D_MVHL_AASHTO_VADOT:
		if(nLoadType == 0)        return TRUE;
		else if(nLoadType == 1)   return TRUE;
		break;
	case D_MVHL_AASHTO_WIDOT:
		if (nLoadType == 3)        return TRUE;
		else if (nLoadType == 8)   return TRUE;
		else if (nLoadType == 9)   return TRUE;
		break;
	case D_MVHL_TURKEY:
		if(m_nMovingType == D_MOVE_CODE_AASHTO_STAN)
		{
			if(nLoadType == 4)      return TRUE;
		}
		else
		{
			if(nLoadType == 0)      return TRUE;
		}
		break;
	case D_MVHL_COLOMBIA:
		if(nLoadType == 0)        return TRUE;
		if(nLoadType == 1)        return TRUE;
		if(nLoadType == 11)       return TRUE;
		break;
	case D_MVHL_AASHTO_PENDOT:
		if (nLoadType == 0)        return TRUE;
		if (nLoadType == 1)        return TRUE;
		break;
	case D_MVHL_ARIZONA:
		if (nLoadType == 2)        return TRUE;
		break;
	case D_MVHL_FLORIDA:
		if (nLoadType == 0)   return TRUE;
		else if (nLoadType == 3)   return TRUE;
		else if (nLoadType == 4)   return TRUE;
		else if (nLoadType == 5)   return TRUE;
		else if (nLoadType == 6)   return TRUE;
		else if (nLoadType == 7)   return TRUE;
		else if (nLoadType == 8)   return TRUE;
		else if (nLoadType == 9)   return TRUE;
		break;
	case D_MVHL_MAINE:
		if (nLoadType == 0)        return TRUE;
		else if (nLoadType == 1)   return TRUE;
		else if (nLoadType == 2)   return TRUE;
		else if (nLoadType == 3)   return TRUE;
		else if (nLoadType == 4)   return TRUE;
		else if (nLoadType == 5)   return TRUE;
		else if (nLoadType == 6)   return TRUE;
		else if (nLoadType == 7)   return TRUE;
		break;
	case D_MVHL_MINNESOTA:
		if (nLoadType == 3)        return TRUE;
		break;
	case D_MVHL_NEWJERSEY:
		if (nLoadType == 6)        return TRUE;
		else if (nLoadType == 7)   return TRUE;
		else if (nLoadType == 9)   return TRUE;
		break;
	case D_MVHL_NORTHDAKOTA:
		if (nLoadType == 0)        return TRUE;
		else if (nLoadType == 6)        return TRUE;
		else if (nLoadType == 7)        return TRUE;
		break;
	case D_MVHL_OREGON:
		if (nLoadType == 22)        return TRUE;
		break;
	case D_MVHL_SOUTHCAROLINA:
		if (nLoadType == 2)        return TRUE;
		break;
	case D_MVHL_SOUTHDAKOTA:
		if (nLoadType == 0)        return TRUE;
		else if (nLoadType == 5)        return TRUE;
		break;
	case D_MVHL_TENNESSEE:
		if (nLoadType == 0)        return TRUE;
		else if (nLoadType == 8)        return TRUE;
		break;
	case D_MVHL_TEXAS:
		if (nLoadType == 2)        return TRUE;
		break;
	case D_MVHL_UTAH:
		if (nLoadType == 0)        return TRUE;
		else if (nLoadType == 4)        return TRUE;
		break;
	case D_MVHL_WASHINGTON:
		if (nLoadType == 0)        return TRUE;
		else if (nLoadType == 1)   return TRUE;
		break;
	case D_MVHL_CALIFORNIA:
		if (nLoadType == 0)        return TRUE;
		break;
	case D_MVHL_COLORADO:
		if (nLoadType == 2)        return TRUE;
	case D_MVHL_DISTRICT_OF_COLUMBIA:
		if (nLoadType == 4)        return TRUE;
		else if (nLoadType == 9)        return TRUE;
		break;
	case D_MVHL_IDAHO:
		if (nLoadType == 0)        return TRUE;
		break;
	case D_MVHL_KANSAS:
		if (nLoadType == 0)        return TRUE;
		break;
	case D_MVHL_KENTUCKY:
		if (nLoadType == 0)        return TRUE;
		else if (nLoadType == 2)        return TRUE;
		break;
	case D_MVHL_MARYLAND:
		if (nLoadType == 0)        return TRUE;
		break;
	case D_MVHL_NEBRASKA:
		if (nLoadType == 2)        return TRUE;
		break;
	case D_MVHL_NEWYORK:
		if (nLoadType == 0)        return TRUE;
		break;
	case D_MVHL_OKLAHOMA:
		if (nLoadType == 0)        return TRUE;
		break;
	case D_MVHL_WESTVIRGINIA:
		if (nLoadType == 0)        return TRUE;
		break;
	}

	return FALSE;
}

BOOL CCMMvhlItemStdDlg::Dlg2Data()
{
	UpdateData(TRUE);
	CString strTemp;

	// m_Data.dLaneFactor의 값은 CCMMvhlSItemUsrLaneFactor 에서 채움
	double dLaneFactor[4];
	dLaneFactor[0] = m_Data.dLaneFactor[0];
	dLaneFactor[1] = m_Data.dLaneFactor[1];
	dLaneFactor[2] = m_Data.dLaneFactor[2];
	dLaneFactor[3] = m_Data.dLaneFactor[3];

	m_Data.Initialize();  
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);   
	m_Data.bStandard = TRUE;
	m_wndVehicleTypeList.GetWindowText(m_Data.VehicleTypeName);
	m_Data.dDynLoadAllowance = m_untDynLoadAllowance.GetEditValue();
	m_Data.nStandardCode = m_nStndCode;
	
	CVehlDB db(m_pDoc);
	if(!db.GetStandardVehicleLoadValue(m_Data))  return FALSE;  
	m_Data.dUnitNum = m_edtNumUnit.GetEditValue();

	// BS BD37/01 Lane Factor
	CString csLaneFactor;
	m_Data.nLaneFactor = (m_nStndCode==D_MVHL_BS_CS454)? m_nLaneFactorCS : m_nLaneFactor;  
	m_Data.dLaneFactor[0] = dLaneFactor[0];
	m_Data.dLaneFactor[1] = dLaneFactor[1];
	m_Data.dLaneFactor[2] = dLaneFactor[2];
	m_Data.dLaneFactor[3] = dLaneFactor[3];

	m_Data.bIncreAxleSpace = (m_chkIncreAxleSpace.GetCheck())? TRUE : FALSE;
	m_Data.dIncreAxleSpace = m_edtIncreAxleSpace.GetEditValue();
	m_Data.nDynaFactor = m_nDynaType;
	m_Data.dDynaFact1Axle = m_edt1Axle.GetEditValue();
	m_Data.dDynaFact2Axle = m_edt2Axle.GetEditValue();
	m_Data.dDynaFact3Axle = m_edt3Axle.GetEditValue();

	m_Data.dJTGCrowdWidth = m_edtJTGCrowd.GetEditValue();
	m_Data.dCJJ11CrowdWidth = m_edtCJJ11wp.GetEditValue();
	
	BOOL bAmplification2 = FALSE;
	
	CArray<UINT,UINT> aRadio;

	BOOL bDynamic;
	bDynamic = m_wndDynamic.GetCheck();
	m_Data.bDynamicFactor = bDynamic;
	int nRdo;
	aRadio.RemoveAll();
	aRadio.Add(IDC_CMD_MVHL_AUTO_RDO);
	aRadio.Add(IDC_CMD_MVHL_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this,aRadio,nRdo);
	m_Data.bUserInput = nRdo;
	m_wndPhi.GetWindowText(strTemp);
	m_Data.dAmplification = _tstof(strTemp);
	
	aRadio.RemoveAll();
	aRadio.Add(IDC_OVERLOAD_FACTOR_AUTO_RDO);
	aRadio.Add(IDC_OVERLOAD_FACTOR_USER_RDO);
	m_Data.bOverLoad = m_chkOverLoadFactor.GetCheck();  
	CDlgUtil::CtrlRadioGetCheck(this,aRadio,nRdo);
	m_Data.nOverLoad = nRdo;    
	m_Data.dCriticalAxle = m_edtOverCriAxle.GetEditValue();
	m_Data.dOtherAxle = m_edtOverOtherAxle.GetEditValue();

	aRadio.RemoveAll();
	aRadio.Add(IDC_VEHICLE_SPEED_NORMAL_RDO);
	aRadio.Add(IDC_VEHICLE_SPEED_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this, aRadio, m_Data.nVehiSpeed);

	m_Data.dDD = m_edtDD.GetEditValue();

	m_wndThreeGrid[0].SaveData();
	m_wndThreeGrid[1].SaveData();
	m_wndThreeGrid[2].SaveData();

	m_Data.bAddDataBD2101 =  m_chkAddData.IsWindowEnabled() ? m_chkAddData.GetCheck() : FALSE;
	m_Data.dAddDataaL = m_edtaL.GetEditValue();
	m_Data.nAddDataCategory = m_cbxCategory.GetCurSel();
	m_Data.nAddDataLoadLevel = m_cbxLoadLevel.GetCurSel();
	m_Data.dImpactCoef = m_edtz.GetEditValue();

	T_MVHL_PATCH_ITEM Patch;
	Patch.dWidth  = m_edtPatchW.GetEditValue();
	Patch.dLength = m_edtPatchL.GetEditValue();
	m_Data.PatchLoad.aPatchLoad.RemoveAll();
	m_Data.PatchLoad.aPatchLoad.Add(Patch);
	m_Data.PatchLoad.bPatchLoad = m_chkPatch.GetCheck();

	m_Data.bCentF =  m_chkCentF.GetCheck();
	m_Data.bTDM   = m_chkTDM.GetCheck();
	BOOL bTDMType = (m_Data.VehicleTypeName == _T("HL-93TDM") || m_Data.VehicleTypeName == _T("KYHL-93TDM")) ? TRUE : FALSE;
	if (!bTDMType)
		m_Data.bTDM = FALSE;

	return TRUE;
}

void CCMMvhlItemStdDlg::UpdateLegalDescription2STC()
{
	if (m_Data.VehicleTypeName == _T("IL-PD6-40"))
	{
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION2)->SetWindowText(_LS(IDS_CMD_MVHL_ILLINOIS_PD6_40));
	}
	else if (m_Data.VehicleTypeName == _T("IL-PD6-200"))
	{
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION2)->SetWindowText(_LS(IDS_CMD_MVHL_ILLINOIS_PD6_200));
	}
	else if (m_Data.VehicleTypeName == _T("P2016-13"))
	{
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION2)->SetWindowText(_LS(IDS_CMD_MVHL_PENNSYLVANIA_P2016_13));
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION2)->SetWindowText(_LS(IDS_CMD_MVHL_ILLINOIS_TWO_TRUCK));
	}
}

void CCMMvhlItemStdDlg::UpdateLegalDescription1STC()
{
	if (m_Data.VehicleTypeName == _T("IL-PS2-21"))
	{
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION1)->SetWindowText(_LS(IDS_CMD_MVHL_ILLINOIS_TEXT_IL_PS2_21));
	}
	else if (m_Data.VehicleTypeName == _T("IL-PS3-31"))
	{
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION1)->SetWindowText(_LS(IDS_CMD_MVHL_ILLINOIS_TEXT_IL_PS3_31));
	}
	else if (m_Data.VehicleTypeName == _T("IL-PS4-34.75"))
	{
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION1)->SetWindowText(_LS(IDS_CMD_MVHL_ILLINOIS_TEXT_IL_PS4_3475));
	}
	else if (m_Data.VehicleTypeName == _T("IL-PC4-41"))
	{
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION1)->SetWindowText(_LS(IDS_CMD_MVHL_ILLINOIS_TEXT_IL_PC4_41));
	}
	else if (m_Data.VehicleTypeName == _T("IL-PC5-41"))
	{
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION1)->SetWindowText(_LS(IDS_CMD_MVHL_ILLINOIS_TEXT_IL_PC5_41));
	}
	else if (m_Data.VehicleTypeName == _T("HST-18"))
	{
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION1)->SetWindowText(_LS(IDS_CMD_MVHL_LOUISIANA_TEXT_HST_18));
	}
	else if (m_Data.VehicleTypeName == _T("P2016-13"))
	{
		GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION1)->SetWindowText(_LS(IDS_CMD_MVHL_PENNSYLVANIA_TEXT_HST_18));
	}
	else
		ASSERT(0);
}

void CCMMvhlItemStdDlg::AlignControlDynamic(BOOL bResize)
{
	BOOL bChange = FALSE;   // 무조건 움직이면 유저가 봤을 때 정신없으니.. 필요할때만 움직임..
	CRect rRef;
	CRect rToMove, rToMoveClone;
	int nDistX,nDistY;	
	nDistX = nDistY = 0;
	CArray<UINT, UINT> aCtrl;

	BOOL b955 = TRUE; // V955
	int nLoadType;
	GetIndex(nLoadType);
	BOOL bPennot = (b955 && m_nMovingType == D_MOVE_CODE_PENDOT);
	if(m_nMovingType == D_MOVE_CODE_KOREA ||  m_nMovingType == D_MOVE_CODE_AASHTO_LRFD || m_nMovingType == D_MOVE_CODE_AASHTO_STAN || bPennot)
	{
		aCtrl.RemoveAll();
		aCtrl.Add(IDC_CMD_MVHL_PTLIST);
		aCtrl.Add(IDC_CMD_MVHL_W_STATIC);
		aCtrl.Add(IDC_CMD_MVHL_W);
		aCtrl.Add(IDC_CMD_MVHL_W_UNIT);
		aCtrl.Add(IDC_CMD_MVHL_Ps_STATIC);
		aCtrl.Add(IDC_CMD_MVHL_PLH);
		aCtrl.Add(IDC_CMD_MVHL_Ps_UNIT);
		aCtrl.Add(IDC_CMD_MVHL_Pm_STATIC);
		aCtrl.Add(IDC_CMD_MVHL_PLV);
		aCtrl.Add(IDC_CMD_MVHL_Pm_UNIT);
		aCtrl.Append(m_aCtrlIrc1);
		aCtrl.Add(IDOK);
		aCtrl.Add(IDCANCEL);
		aCtrl.Add(IDC_CMD_APPLY);

		GetDlgItem(IDC_CMD_MVHL_PIC_WND)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(6);
		if(nDistY != 0)
		{
			CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);
			bChange = TRUE;

			aCtrl.RemoveAll();
			aCtrl.Add(IDC_CMD_MVHL_LEGAL_W_STATIC);
			aCtrl.Add(IDC_CMD_MVHL_LEGAL_W);
			aCtrl.Add(IDC_CMD_MVHL_LEGAL_W_UNIT);
			aCtrl.Add(IDC_CMD_MVHL_LEGAL_R_STATIC);
			aCtrl.Add(IDC_CMD_MVHL_LEGAL_R);
			aCtrl.Add(IDC_CMD_MVHL_LEGAL_R_UNIT);
			aCtrl.Add(IDC_CMD_MVHL_LEGAL_DIST_STATIC);
			aCtrl.Add(IDC_CMD_MVHL_LEGAL_DIST);
			aCtrl.Add(IDC_CMD_MVHL_LEGAL_DIST_UNIT);

			GetDlgItem(IDC_CMD_MVHL_W)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_LEGAL_W)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);

			//
			GetDlgItem(IDC_CMD_MVHL_W)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_PENNDOT_W)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aCtrlPenndot, nDistX, nDistY);

			//
			aCtrl.RemoveAll();
			aCtrl.Add(IDC_CMD_MVHL_TDM_W_STATIC );
			aCtrl.Add(IDC_CMD_MVHL_TDM_W        );
			aCtrl.Add(IDC_CMD_MVHL_TDM_W_UNIT   );
			aCtrl.Add(IDC_CMD_MVHL_TDM_D2_STATIC);
			aCtrl.Add(IDC_CMD_MVHL_TDM_D2       );
			aCtrl.Add(IDC_CMD_MVHL_TDM_D2_UNIT  );
			aCtrl.Add(IDC_CMD_MVHL_TDM_D3_STATIC);
			aCtrl.Add(IDC_CMD_MVHL_TDM_D3       );
			aCtrl.Add(IDC_CMD_MVHL_TDM_D3_UNIT  );

			GetDlgItem(IDC_CMD_MVHL_W)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_TDM_W)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);
		}

		BOOL bLrfdCF=FALSE;
		switch(m_nStndCode)
		{
		case D_MVHL_AASHTO_LRFD:  case D_MVHL_AASHTO_LRFD_LEGAL: /*case D_MVHL_AASHTO_LEGAL:*/
		case D_MVHL_AASHTO_IADOT: case D_MVHL_AASHTO_ILDOT:  case D_MVHL_AASHTO_LADOT:
		case D_MVHL_AASHTO_MODOT: case D_MVHL_AASHTO_OHDOT:  case D_MVHL_AASHTO_RIDOT:
		case D_MVHL_AASHTO_VADOT: case D_MVHL_AASHTO_WIDOT:  case D_MVHL_AASHTO_INDOT:
		case D_MVHL_ALABAMA:	case D_MVHL_ARIZONA:	case D_MVHL_CALIFORNIA:	case D_MVHL_COLORADO:
		case D_MVHL_CONNECTICUT:	case D_MVHL_FLORIDA:	case D_MVHL_GEORGIA:	case D_MVHL_HAWAII:
		case D_MVHL_IDAHO:	case D_MVHL_KANSAS:	case D_MVHL_KENTUCKY:	case D_MVHL_DISTRICT_OF_COLUMBIA:
		case D_MVHL_MAINE:	case D_MVHL_MARYLAND:	case D_MVHL_MASSACHUSETTS:	case D_MVHL_MICHIGAN_NL:
		case D_MVHL_MICHIGAN_DL:	case D_MVHL_MICHIGAN_SD:	case D_MVHL_MINNESOTA:	case D_MVHL_MISSISSIPI:
		case D_MVHL_NEBRASKA:	case D_MVHL_NEWJERSEY:	case D_MVHL_NEWMEXICO:	case D_MVHL_NEWYORK:
		case D_MVHL_NORTHDAKOTA:	case D_MVHL_OKLAHOMA:	case D_MVHL_OREGON:	case D_MVHL_SOUTHCAROLINA:
		case D_MVHL_SOUTHDAKOTA:	case D_MVHL_TENNESSEE:	case D_MVHL_TEXAS:	case D_MVHL_UTAH:
		case D_MVHL_VERMONT:	case D_MVHL_WASHINGTON:	case D_MVHL_WESTVIRGINIA:	case D_MVHL_DELAWARE:
		case D_MVHL_NEVADA:	case D_MVHL_ARKANSAS:	case D_MVHL_ALASKA:
			bLrfdCF = TRUE;
			break;
		}

		BOOL bTDMType = (m_Data.VehicleTypeName == _T("HL-93TDM") || m_Data.VehicleTypeName == _T("KYHL-93TDM")) ? TRUE : FALSE;
		BOOL b93TRK   = (m_Data.VehicleTypeName == _T("HL-93TRK") || m_Data.VehicleTypeName == _T("KYHL-93TRK") || m_Data.VehicleTypeName == _T("TP-16TDM")) ? TRUE : FALSE;
		BOOL bFTG     = (m_Data.VehicleTypeName == _T("HS20-FTG")) ? TRUE : FALSE;
		BOOL bDescription1 = (m_Data.VehicleTypeName == _T("IL-PS2-21") || m_Data.VehicleTypeName == _T("IL-PS3-31") ||
											m_Data.VehicleTypeName == _T("IL-PS4-34.75") || m_Data.VehicleTypeName == _T("IL-PC4-41") ||
											m_Data.VehicleTypeName == _T("IL-PC5-41") || m_Data.VehicleTypeName == _T("HST-18") || m_Data.VehicleTypeName == _T("P2016-13"));
		BOOL bDescription2 = (m_Data.VehicleTypeName == _T("IL-PS2-21") || m_Data.VehicleTypeName == _T("IL-PS3-31") ||
												m_Data.VehicleTypeName == _T("IL-PS4-28") || m_Data.VehicleTypeName == _T("IL-PS4-34.75") ||
												m_Data.VehicleTypeName == _T("IL-PS5-36") || m_Data.VehicleTypeName == _T("IL-PS6-35.75") ||
												m_Data.VehicleTypeName == _T("IL-PS7-39.75") || m_Data.VehicleTypeName == _T("IL-PS3-31") ||
												m_Data.VehicleTypeName == _T("IL-PC4-41") || m_Data.VehicleTypeName == _T("IL-PC5-41") ||
												m_Data.VehicleTypeName == _T("IL-PD6-40") || m_Data.VehicleTypeName == _T("IL-PD6-200") ||
												m_Data.VehicleTypeName == _T("P2016-13"));


		if (m_Data.VehicleTypeName == _T("HL-93TRK") || m_Data.VehicleTypeName == _T("KGM-45") ||
			m_Data.VehicleTypeName == _T("KYHL-93TRK") || m_Data.VehicleTypeName == _T("TP-16TDM"))
		{
			GetDlgItem(IDC_CMD_MVHL_LEGAL_DIST_STATIC)->SetWindowText(_LS(IDS_CMD_MVHL_MVHL_LEGAL_MIN_DIST));
		}
		else
		{
			GetDlgItem(IDC_CMD_MVHL_LEGAL_DIST_STATIC)->SetWindowText(_LS(IDS_CMD_MVHL_MVHL_LEGAL_DIST));
		}

		if (bDescription2)
		{
			UpdateLegalDescription2STC();
		}
		//////////////////////////////////////////////////////////////////////////
		// Patch Load Ctrl
		if (b93TRK)
		{
			GetDlgItem(IDC_CMD_MVHL_LEGAL_DIST)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->GetWindowRect(rToMove);
			nDistX = rRef.right - rToMove.right;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(30);
			CDlgUtil::CtrlMoveDistXY(this, m_aCtrlPatch, nDistX, nDistY);
		}
		else if (bTDMType)
		{
			GetDlgItem(IDC_CMD_MVHL_TDM_D3)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->GetWindowRect(rToMove);
			nDistX = rRef.right - rToMove.right;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(30);
			CDlgUtil::CtrlMoveDistXY(this, m_aCtrlPatch, nDistX, nDistY);
		}
		else if (bFTG)
		{
			GetDlgItem(IDC_CMD_MVHL_dD2)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->GetWindowRect(rToMove);
			nDistX = rRef.right - rToMove.right;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(30);
			CDlgUtil::CtrlMoveDistXY(this, m_aCtrlPatch, nDistX, nDistY);
		}
		//////////////////////////////////////////////////////////////////////////

		if(bLrfdCF)
		{
			// Centrifugal Force
			aCtrl.RemoveAll();
			aCtrl.Add(IDC_CMD_MVHL_ADD_CF_CHK);
			aCtrl.Add(IDC_CMD_MVHL_TDM_CHK);
			GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);

			if (bDescription1) {
				UpdateLegalDescription1STC();

				if (bDescription2)
				{
					GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION2)->GetWindowRect(rToMove);

					nDistX = 0;
					nDistY = rToMove.bottom - rToMove.top;
					CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);
				}

				GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION1)->GetWindowRect(rToMove);

				nDistX = 0;
				nDistY = rToMove.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
				CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);

				aCtrl.RemoveAll();
				aCtrl.Add(IDC_CMD_MVHL_LEGAL_DESCRIPTION1);
				if (bDescription2)
				{
					aCtrl.Add(IDC_CMD_MVHL_LEGAL_DESCRIPTION2);
				}

				nDistX = rRef.left - rToMove.left;
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
				CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);

				GetDlgItem(aCtrl[aCtrl.GetSize()-1])->GetWindowRect(rRef);
			}
			else if (bDescription2)
			{
				GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION2)->GetWindowRect(rToMove);

				nDistX = 0;
				nDistY = rToMove.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
				CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);

				aCtrl.RemoveAll();
				aCtrl.Add(IDC_CMD_MVHL_LEGAL_DESCRIPTION1);
				aCtrl.Add(IDC_CMD_MVHL_LEGAL_DESCRIPTION2);

				nDistX = rRef.left - rToMove.left;
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
				CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);

				GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION2)->GetWindowRect(rRef);
			}
		}
		else
		{
			if (bDescription1) {

				aCtrl.RemoveAll();
				aCtrl.Add(IDC_CMD_MVHL_LEGAL_DESCRIPTION1);
				aCtrl.Add(IDC_CMD_MVHL_LEGAL_DESCRIPTION2);

				GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION1)->GetWindowRect(rToMove);
				nDistX = rRef.left - rToMove.left;
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
				CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);

				UpdateLegalDescription1STC();
			}
			else if (bDescription2)
			{
				aCtrl.RemoveAll();
				aCtrl.Add(IDC_CMD_MVHL_LEGAL_DESCRIPTION1);
				aCtrl.Add(IDC_CMD_MVHL_LEGAL_DESCRIPTION2);

				GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION2)->GetWindowRect(rToMove);
				nDistX = rRef.left - rToMove.left;
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
				CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);
			}
		}

		if(bTDMType)
			GetDlgItem(IDC_CMD_MVHL_TDM_CHK)->GetWindowRect(rRef);
		else if(bLrfdCF)
			GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->GetWindowRect(rRef);
		else if (bDescription2)
			GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION2)->GetWindowRect(rRef);
		else if(bDescription1)
			GetDlgItem(IDC_CMD_MVHL_LEGAL_DESCRIPTION1)->GetWindowRect(rRef);
		else
			GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->GetWindowRect(rToMove);      
		rToMoveClone = rToMove;
		rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
		if (m_bPatchLoad) rToMove.bottom += 25;

		if(rToMove != rToMoveClone)
		{
			ScreenToClient(rToMove);
			GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_SHOWWINDOW);
			bChange = TRUE;
		}
	}
	else
	{
		bChange = TRUE;
	}

	if(bChange)
	{
		// Move OK, Cancel, Apply    
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom-rToMove.top + globalUtils.ScaleByDPI(16);
		CDlgUtil::CtrlMoveDistY(this, m_aOKCancelApply, nDistY);

		// resize self size
		if (bResize)
		{
			CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
			CRect rectLast;
			pWndLast->GetWindowRect(rectLast);
			CRect r;
			GetWindowRect(r);
			r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
			MoveWindow(r);
		}
	}
}

void CCMMvhlItemStdDlg::AlignControl()
{
	CRect rRef, rRefcs;
	CRect rToMove;
	int nDistX,nDistY;	
	UINT aFirstCtrlID[] = {IDC_CMD_MVHL_P};
	CArray<UINT, UINT> *aCtrlSet[] = {&m_aCtrlCh};

	GetDlgItem(IDC_CMD_MVHL_W)->GetWindowRect(rRef);
	for (int i = 0; i < sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, *aCtrlSet[i], nDistY);
	}

	// BS Control
	GetDlgItem(IDC_CMD_MVHL_BS_W1_EDIT)->GetWindowRect(rToMove);
	//nDistY = rRef.top - rToMove.top - 10;
	nDistY = rRef.top - rToMove.top;

	CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS5400, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS, nDistY);

	GetDlgItem(IDC_CMD_MVHL_BS37_W1_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top - globalUtils.ScaleByDPI(10);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS3701, nDistY);

	// No. of Units를 PB와 동일한 높이로 이동 (PB 먼저 이동된 후 수행해야 함)
	CRect rdd;
	GetDlgItem(m_aCtrlBS_dd[0])->GetWindowRect(rdd);
	GetDlgItem(m_aCtrlBS_UnitNum[0])->GetWindowRect(rToMove);
	nDistY = rdd.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_UnitNum, nDistY);

	// China JTG Lane Load Control
	GetDlgItem(IDC_CMD_MVHL_CH_QK_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlJTGLane, nDistY);

	// China JTG Crowd Load Control
	GetDlgItem(IDC_CMD_MVHL_CH_W1_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlJTGCrowd, nDistY);

	// China CJJ11-2011 Crowd Load Control
	GetDlgItem(IDC_CMD_MVHL_CH_CROWD_WP_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCJJ11Crowd, nDistY);

	// BS3701-Pedestrian
	GetDlgItem(m_aCtrlBS_Ped[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlBS_Ped, nDistY);
	
	// BS BD37/01 : Lane Factor 
	GetDlgItem(IDC_CMD_MVHL_LANE_FACTOR)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	//CDlgUtil::CtrlMoveDistY(this, m_aBSBD3701LaneFactor, nDistY-10);
	CDlgUtil::CtrlMoveDistY(this, m_aBSBD3701LaneFactor, nDistY);

	// BS CS 454 : Lane Factor 
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRefcs);
	GetDlgItem(IDC_CMD_MVHL_LANE_FACTOR_CS)->GetWindowRect(rToMove);
	int nDistY_CS = rRefcs.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aBSCS454LaneFactor, nDistY_CS);
	
	// Vehicular Load Properties Group Box 2
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES2)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aGroupBox2, nDistY);

	// Dynamic Load Allownace
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	//GetDlgItem(IDC_CMD_MVHL_DYNA_GRP)->GetWindowRect(rToMove);
	GetDlgItem(IDC_CMD_MVHL_INCRE_AXSPACE_CHK)->GetWindowRect(rToMove); 
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aDyna, nDistY);
	
//   GetDlgItem(IDC_CMD_MVHL_DYN_TEXT)->GetWindowRect(rRef);
//   GetDlgItem(IDC_STATIC_IRC)->GetWindowRect(rToMove);
//   nDistY = rRef.top - rToMove.top;
//   CDlgUtil::CtrlMoveDistY(this, m_aCtrlIrc, nDistY);

	// BS BD86/11
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSoSov, nDistY);

	GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlPhi, nDistY);

	GetDlgItem(IDC_CMD_MVHL_W)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_DD)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlDD, nDistX, nDistY);

	// reSize
	if(m_nMovingType == D_MOVE_CODE_CANADA)
	{

		GetDlgItem(IDC_CMD_MVHL_DYNA_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);

		CArray<UINT, UINT> aCtrlCF; 
		aCtrlCF.Add(IDC_CMD_MVHL_ADD_CF_CHK);
		CDlgUtil::CtrlMoveDistY(this, aCtrlCF, nDistY);

		CRect rectTarget;
		CRect rectRef1;
		CRect rectRef2;
		CRect rectRef3;
		CRect rectRef4;
		
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->GetWindowRect(rectTarget);
		
		GetDlgItem(IDC_CMD_MVHL_NAME)->GetWindowRect(rectRef1);
		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rectRef2);
		nDistY = rectRef2.bottom - rectRef1.top;
		
		GetDlgItem(IDC_CMD_MVHL_DYNA_GRP)->GetWindowRect(rectRef3);
		GetDlgItem(IDC_CMD_MVHL_INCRE_AXSPACE_CHK)->GetWindowRect(rectRef4);
		nDistY += rectRef3.bottom - rectRef4.top;

		GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->GetWindowRect(rectRef4);
		nDistY += rectRef4.bottom - rectRef3.bottom;
		
		rectTarget.bottom = rectTarget.top + nDistY + globalUtils.ScaleByDPI(50);
		ScreenToClient(rectTarget);
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->MoveWindow(rectTarget);
	}

	AlignControlDynamic(FALSE);	

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+ globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

BOOL CCMMvhlItemStdDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;

	return TRUE;
}

void CCMMvhlItemStdDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemStdDlg::OnCmdApply() 
{	
	ApplyOrOK();
}

/////////////////////////////////////////////////////////////////////////////
// picture window <--- nowlee *^o^*
void CCMMvhlItemStdDlg::ChangeBitmap(int nIndex, int nLoadType)
{
	BOOL b955 = TRUE; // V955

	CString strSVG;  
	switch(nIndex)
	{
		case D_MVHL_KOR_STND_LOAD:
		case D_MVHL_KOR_STND_2005:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_01.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_01.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_01.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_02.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_02.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_02.svg");
			else ASSERT(0);
			break;
			break;
		case D_MVHL_KOR_RAIL_LOAD:
		case D_MVHL_KOR_RAIL_2004:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_13.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_13.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_13.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_13.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_14.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_14.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_14.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_14.svg");
			else if(nLoadType == 8)   strSVG = _T("cmd_mvhl_15.svg");
			else if(nLoadType == 9)   strSVG = _T("cmd_mvhl.svg");
			else if(nLoadType == 10)  strSVG = _T("cmd_mvhl.svg");
			else if(nLoadType == 11)  strSVG = _T("cmd_mvhl.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_STAND:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_08.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_09.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_10.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_11.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_08.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_09.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_10.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_11.svg");
			else if(nLoadType == 8)   strSVG = _T("cmd_mvhl_09.svg");
			else if(nLoadType == 9)   strSVG = _T("cmd_mvhl_11.svg");
			else if(nLoadType == 10)  strSVG = _T("cmd_mvhl_12.svg");
			else ASSERT(0);
			break;
		case D_MVHL_CALTRANS_STND:
		case D_MVHL_CALTRANS_STND_2017:
		case D_MVHL_NEVADA:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_03.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_04.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_05.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_06.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_07.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_80.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_75.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_76.svg");
			else if(nLoadType == 8)   strSVG = _T("cmd_mvhl_77.svg");
			else if(nLoadType == 9)   strSVG = _T("cmd_mvhl_78.svg");
			else if(nLoadType == 10)  strSVG = _T("cmd_mvhl_79.svg");
			else if(nLoadType == 11)  strSVG = _T("cmd_mvhl_74.svg");
			else ASSERT(0);
			break;
		case D_MVHL_CH_JTJ001_97:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_19.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_19.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_20.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_21.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_22.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_23.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_23.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_23.svg");
			else ASSERT(0);
			break;
		case D_MVHL_CH_CJJ77_98:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_24.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_25.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_26.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_26.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_26.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_26.svg");
			else ASSERT(0);
			break;
		case D_MVHL_CH_TB100021_99:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_27.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_28.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_15.svg");
			else ASSERT(0);
			break;
		case D_MVHL_OTHERS:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_16.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_17.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_85.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_86.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_LRFD:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_222.svg");
			else if(nLoadType == 1)   
			{
				BOOL bTDMType = (m_Data.VehicleTypeName == _T("HL-93TDM")) ? TRUE : FALSE;
				if(bTDMType) strSVG = _T("cmd_mvhl_30_TDM.svg");
				else         strSVG = _T("cmd_mvhl_30.svg");
			}
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_01.svg");
			else ASSERT(0);
			break;
		case D_MVHL_ROC_TAIWAN:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_09.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_09.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_08.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_08.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_08.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_10.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_10.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_10.svg");
			else if(nLoadType == 8)   strSVG = _T("cmd_mvhl_10.svg");
			else if(nLoadType == 9)   strSVG = _T("cmd_mvhl_10.svg");
			else if(nLoadType == 10)  strSVG = _T("cmd_mvhl_12.svg");
			else ASSERT(0);
			break;
		case D_MVHL_CANCSA_S600:
			if (nLoadType == 0)        strSVG = _T("cmd_mvhl_05.svg");
			else if (nLoadType == 1)   strSVG = _T("cmd_mvhl_34.svg");
			else if (nLoadType == 2)   strSVG = _T("cmd_mvhl_05.svg");
			else if (nLoadType == 3)   strSVG = _T("cmd_mvhl_34.svg");
			else if (nLoadType == 4)   strSVG = _T("cmd_mvhl_34.svg");
			else if (nLoadType == 5)   strSVG = _T("cmd_mvhl_34.svg");
			else if (nLoadType == 6)   strSVG = _T("cmd_mvhl_04.svg");
			else if (nLoadType == 7)   strSVG = _T("cmd_mvhl_35.svg");
			else if (nLoadType == 8)   strSVG = _T("cmd_mvhl_35.svg");
			else if (nLoadType == 9)   strSVG = _T("cmd_mvhl_35.svg");
			else if (nLoadType == 10)  strSVG = _T("cmd_mvhl_03.svg");
			else if (nLoadType == 11)  strSVG = _T("cmd_mvhl_36.svg");
			else if (nLoadType == 12)  strSVG = _T("cmd_mvhl_36.svg");
			else if (nLoadType == 13)  strSVG = _T("cmd_mvhl_36.svg");
			else if(nLoadType == 14)  strSVG = _T("cmd_mvhl_121.svg");
			else if(nLoadType == 15)  strSVG = _T("cmd_mvhl_122.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_PENDOT:
			if(nLoadType == 0)        strSVG = (b955) ? _T("cmd_mvhl_222.svg") : _T("cmd_mvhl_29.svg");
			else if(nLoadType == 1)   strSVG = (b955) ? _T("cmd_mvhl_236.svg") : _T("cmd_mvhl_30.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_01.svg");
			else if(nLoadType == 3)   strSVG = _T("bcmd_mvhl_37.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_38.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_42.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_231.svg");
			else ASSERT(0);
			break;
		case D_MVHL_BS_5400:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_39.svg");
			else ASSERT(0);
			break;
		case D_MVHL_CH_JTGB01_2003:
		case D_MVHL_CH_JTGB01_2014:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_43.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_44.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_45.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_198.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_196.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_23.svg");
			else ASSERT(0);
			break;    
		case D_MVHL_BS_BD3701:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_47.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_48.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_49.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_39.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_50.svg");
			else ASSERT(0);
			break;
		case D_MVHL_BS_CS454: 
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_47_CS.svg");  // 임시
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_LEGAL:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_81.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_82.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_83_new.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_84.svg");
			else ASSERT(0);
			break;
		case D_MVHL_CH_CJJ11_2011:
		case D_MVHL_CH_CJJ11_2019:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_24.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_44.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_43.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_123.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_124.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_125.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_126.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_127.svg");
			else ASSERT(0);
			break;
		case D_MVHL_KOR_RAIL_2012:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_138.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_138.svg");
			else ASSERT(0);
			break;		
		case D_MVHL_AASHTO_LRFD_LEGAL:
		case D_MVHL_ARKANSAS:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_81.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_82.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_83_new.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_139.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_140.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_141.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_142.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_143.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_IADOT:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_144.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_145.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_146.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_147.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_150_new.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_171_new.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_171_new.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_171_new.svg");
			else if(nLoadType == 8)   strSVG = _T("cmd_mvhl_268_IA4S3.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_ILDOT:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_148_new.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_149_new.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_150_new.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_151_new.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_152_new.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_153_new.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_154_new.svg");
			else if (nLoadType == 7)   strSVG = _T("cmd_mvhl_155.svg");
			else if (nLoadType == 8)   strSVG = _T("cmd_mvhl_156.svg");
			else if (nLoadType == 9)   strSVG = _T("cmd_mvhl_157.svg");
			else if (nLoadType == 10)  strSVG = _T("cmd_mvhl_158.svg");
			else if (nLoadType == 11)  strSVG = _T("cmd_mvhl_159.svg");
			else if (nLoadType == 12)  strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 13)  strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 14)  strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 15)  strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 16)  strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 17)  strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 18)  strSVG = _T("cmd_mvhl_228.svg");
			else if (nLoadType == 19)  strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 20)  strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 21)  strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 22)  strSVG = _T("cmd_mvhl_187_NA_Illinois_PD6_40.svg");
			else if (nLoadType == 23)  strSVG = _T("cmd_mvhl_188_NA_Illinois_PD6_200.svg");
			else if (nLoadType == 24)  strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 25)  strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 26)  strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 27)  strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 28)  strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 29)  strSVG = _T("cmd_mvhl_235.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_LADOT:
			if (nLoadType == 0)        strSVG = _T("cmd_mvhl_160.svg");
			else if (nLoadType == 1)   strSVG = _T("cmd_mvhl_161.svg");
			else if (nLoadType == 2)   strSVG = _T("cmd_mvhl_162.svg");
			else if (nLoadType == 3)   strSVG = _T("cmd_mvhl_163.svg");
			else if (nLoadType == 4)   strSVG = _T("cmd_mvhl_164.svg");
			else if (nLoadType == 5)   strSVG = _T("cmd_mvhl_165.svg");
			else if (nLoadType == 6)   strSVG = _T("cmd_mvhl_166.svg");
			else if (nLoadType == 7)   strSVG = _T("cmd_mvhl_167.svg");
			else if(nLoadType == 8)   strSVG = _T("cmd_mvhl_168_new.svg");
			else if(nLoadType == 9)   strSVG = _T("cmd_mvhl_169_new.svg");
			else if(nLoadType == 10)  strSVG = _T("cmd_mvhl_170_new.svg");
			else if(nLoadType == 11)  strSVG = _T("cmd_mvhl_171_new.svg");
			else if (nLoadType == 12)        strSVG = _T("cmd_mvhl_09.svg");
			else if (nLoadType == 13)   strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType == 14)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 15)   strSVG = _T("cmd_mvhl_234.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_MODOT:
			if(nLoadType == 0)         strSVG = _T("cmd_mvhl_172.svg");
			else if(nLoadType == 1)    strSVG = _T("cmd_mvhl_173.svg");
			else if(nLoadType == 2)    strSVG = _T("cmd_mvhl_174.svg");
			else if(nLoadType == 3)    strSVG = _T("cmd_mvhl_175.svg");
			else if(nLoadType == 4)    strSVG = _T("cmd_mvhl_176_new.svg");
			else if(nLoadType == 5)    strSVG = _T("cmd_mvhl_177_new.svg");
			else if(nLoadType == 6)    strSVG = _T("cmd_mvhl_178_new.svg");
			else if(nLoadType == 7)    strSVG = _T("cmd_mvhl_179.svg");
			else if(nLoadType == 8)    strSVG = _T("cmd_mvhl_235.svg");
			else if(nLoadType == 9)    strSVG = _T("cmd_mvhl_226.svg");
			else if(nLoadType == 10)   strSVG = _T("cmd_mvhl_226.svg");
			else if(nLoadType == 11)   strSVG = _T("cmd_mvhl_226.svg");
			else if(nLoadType == 12)   strSVG = _T("cmd_mvhl_81.svg");
			else if(nLoadType == 13)   strSVG = _T("cmd_mvhl_82.svg");
			else if(nLoadType == 14)   strSVG = _T("cmd_mvhl_83_new.svg");
			else if(nLoadType == 15)   strSVG = _T("cmd_mvhl_140.svg");
			else if(nLoadType == 16)   strSVG = _T("cmd_mvhl_141.svg");
			else if(nLoadType == 17)   strSVG = _T("cmd_mvhl_142.svg");
			else if(nLoadType == 18)   strSVG = _T("cmd_mvhl_143.svg");
			else if(nLoadType == 19)   strSVG = _T("cmd_mvhl_234.svg");
			else if(nLoadType == 20)   strSVG = _T("cmd_mvhl_235.svg");
			else if(nLoadType == 21)   strSVG = _T("cmd_mvhl_06.svg");
			else if(nLoadType == 22)   strSVG = _T("cmd_mvhl_228.svg");
			else if(nLoadType == 23)   strSVG = _T("cmd_mvhl_228.svg");
			else if(nLoadType == 24)   strSVG = _T("cmd_mvhl_229.svg");
			else if(nLoadType == 25)   strSVG = _T("cmd_mvhl_196_NA_Missouri_9.svg");
			else if(nLoadType == 26)   strSVG = _T("cmd_mvhl_226.svg");
			else if(nLoadType == 27)   strSVG = _T("cmd_mvhl_226.svg");
			else if(nLoadType == 28)   strSVG = _T("cmd_mvhl_06.svg");
			else if(nLoadType == 29)   strSVG = _T("cmd_mvhl_228.svg");
			else if(nLoadType == 30)   strSVG = _T("cmd_mvhl_229.svg");
			else if(nLoadType == 31)   strSVG = _T("cmd_mvhl_197_NA_Missouri_10.svg");
			else if(nLoadType == 32)   strSVG = _T("cmd_mvhl_230.svg");
			else if(nLoadType == 33)   strSVG = _T("cmd_mvhl_230.svg");
			else if(nLoadType == 34)   strSVG = _T("cmd_mvhl_198_NA_Missouri_12.svg");
			else if(nLoadType == 35)   strSVG = _T("cmd_mvhl_231.svg");
			else if(nLoadType == 36)   strSVG = _T("cmd_mvhl_231.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_OHDOT:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_148_new.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_181_new.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_182_new.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_183_new.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_RIDOT:
			if(nLoadType == 0)         strSVG = _T("cmd_mvhl_184.svg");
			else if(nLoadType == 1)    strSVG = _T("cmd_mvhl_185.svg");
			else if(nLoadType == 2)    strSVG = _T("cmd_mvhl_186.svg");
			else if(nLoadType == 3)    strSVG = _T("cmd_mvhl_187.svg");
			else if(nLoadType == 4)    strSVG = _T("cmd_mvhl_226.svg");
			else if(nLoadType == 5)    strSVG = _T("cmd_mvhl_229.svg");
			else if(nLoadType == 6)    strSVG = _T("cmd_mvhl_231.svg");
			else if(nLoadType == 7)    strSVG = _T("cmd_mvhl_06.svg");
			else if(nLoadType == 8)    strSVG = _T("cmd_mvhl_228.svg");
			else if(nLoadType == 9)    strSVG = _T("cmd_mvhl_234.svg");
			else if(nLoadType == 10)   strSVG = _T("cmd_mvhl_235.svg");
			else if(nLoadType == 11)   strSVG = _T("cmd_mvhl_227.svg");
			else if(nLoadType == 12)   strSVG = _T("cmd_mvhl_226.svg");
			else if(nLoadType == 13)   strSVG = _T("cmd_mvhl_226.svg");
			else if(nLoadType == 14)   strSVG = _T("cmd_mvhl_06.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_VADOT:
			if(nLoadType == 0)          strSVG = _T("cmd_mvhl_191_new.svg");
			else if(nLoadType == 1)     strSVG = _T("cmd_mvhl_192_new.svg");
			else if(nLoadType == 2)     strSVG = _T("cmd_mvhl_193.svg");
			else if(nLoadType == 3)     strSVG = _T("cmd_mvhl_194.svg");
			else if (nLoadType == 4)    strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 5)    strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 6)    strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 7)    strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 8)    strSVG = _T("cmd_mvhl_228.svg");
			else if (nLoadType == 9)    strSVG = _T("cmd_mvhl_228.svg");
			else if (nLoadType == 10)   strSVG = _T("cmd_mvhl_228.svg");
			else if (nLoadType == 11)   strSVG = _T("cmd_mvhl_228.svg");
			else if (nLoadType == 12)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 13)   strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 14)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 15)   strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 16)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 17)   strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 18)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 19)   strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 20)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 21)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 22)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 23)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 24)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 25)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 26)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 27)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 28)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 29)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 30)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 31)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 32)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 33)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 34)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 35)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 36)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 37)   strSVG = _T("cmd_mvhl_06.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_WIDOT:
			if(nLoadType == 0)          strSVG = _T("cmd_mvhl_195.svg");
			else if (nLoadType == 1)    strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 2)    strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 3)    strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 4)    strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 5)    strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 6)    strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 7)    strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 8)    strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 9)    strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 10)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 11)   strSVG = _T("cmd_mvhl_235.svg");
			else ASSERT(0);      
			break;
		case D_MVHL_BS_BD8611:
			if(nLoadType == 0 || nLoadType == 1)        strSVG = _T("cmd_mvhl_sv80_sv100.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_sv150.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_sv196.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_sv-train.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_SVTT.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_sov250.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_sov350.svg");
			else if(nLoadType == 8)   strSVG = _T("cmd_mvhl_sov450.svg");
			else if(nLoadType == 9)   strSVG = _T("cmd_mvhl_sov600.svg");
			else ASSERT(0);
			break;
		case D_MVHL_TURKEY:
			if(m_nMovingType == D_MOVE_CODE_AASHTO_STAN)
			{
				if(nLoadType == 0)        strSVG = _T("cmd_mvhl_09.svg");
				else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_10.svg");
				else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_09.svg");
				else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_10.svg");
				else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_222.svg");
				else ASSERT(0);
			}
			else
			{
				if(nLoadType == 0)        strSVG = _T("cmd_mvhl_222.svg");
				else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_09.svg");
				else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_10.svg");
				else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_09.svg");
				else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_10.svg");
				else ASSERT(0);
			}
			break;
		case D_MVHL_CH_TB10002_2017:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_106.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_106.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_106.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_224.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_15.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_15.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_15.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_223.svg");
			else ASSERT(0);
			break;			
		case D_MVHL_CH_QCR9300_2018:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_106.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_106.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_106.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_224.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_15.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_15.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_15.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_223.svg");
			else ASSERT(0);
			break;
		case D_MVHL_AASHTO_INDOT:
			if(nLoadType == 0)          strSVG = _T("cmd_mvhl_226.svg");
			else if(nLoadType == 1)     strSVG = _T("cmd_mvhl_227.svg");
			else if(nLoadType == 2)     strSVG = _T("cmd_mvhl_228.svg");
			else if(nLoadType == 3)     strSVG = _T("cmd_mvhl_229.svg");
			else if(nLoadType == 4)     strSVG = _T("cmd_mvhl_230.svg");
			else if(nLoadType == 5)     strSVG = _T("cmd_mvhl_230.svg");
			else if(nLoadType == 6)     strSVG = _T("cmd_mvhl_231.svg");
			else if(nLoadType == 7)     strSVG = _T("cmd_mvhl_232.svg");
			else if(nLoadType == 8)     strSVG = _T("cmd_mvhl_233.svg");
			else if(nLoadType == 9)     strSVG = _T("cmd_mvhl_233.svg");
			else if(nLoadType == 10)    strSVG = _T("cmd_mvhl_09.svg");
			else if(nLoadType == 11)    strSVG = _T("cmd_mvhl_11.svg");
			else if(nLoadType == 12)    strSVG = _T("cmd_mvhl_140.svg");
			else if(nLoadType == 13)    strSVG = _T("cmd_mvhl_141.svg");
			else if(nLoadType == 14)    strSVG = _T("cmd_mvhl_142.svg");
			else if(nLoadType == 15)    strSVG = _T("cmd_mvhl_143.svg");
 			else ASSERT(0);      
			break;
		case D_MVHL_COLOMBIA:
			if(nLoadType == 0)         strSVG = _T("cmd_mvhl_222.svg");
			else if(nLoadType == 1)    strSVG = _T("cmd_mvhl_236.svg");
			else ASSERT(0);
			break;
		case D_MVHL_FAST_ACT_EV_LOAD:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_234.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_235.svg");
			else ASSERT(0);      
			break;
		case D_MVHL_ALABAMA:
			if (nLoadType == 0)        strSVG = _T("cmd_mvhl_09.svg");
			else ASSERT(0);
			break;
		case D_MVHL_ARIZONA:
			if (nLoadType == 0)        strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 1)   strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 2)   strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 3)   strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 4)   strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 5)   strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 6)   strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 7)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 8)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 9)   strSVG = _T("cmd_mvhl_226.svg");
			else ASSERT(0);
			break;
		case D_MVHL_CALIFORNIA:
			if (nLoadType == 0)        strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 1)   strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType == 2)   strSVG = _T("cmd_mvhl_03.svg");
			else if (nLoadType == 3)   strSVG = _T("cmd_mvhl_04.svg");
			else if (nLoadType == 4)   strSVG = _T("cmd_mvhl_05.svg");
			else if (nLoadType == 5)   strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 6)   strSVG = _T("cmd_mvhl_07.svg");
			else if (nLoadType == 7)   strSVG = _T("cmd_mvhl_74.svg");
			else ASSERT(0);
			break;
		case D_MVHL_COLORADO:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_09.svg");
			else if (nLoadType ==  1)   strSVG = _T("cmd_mvhl_11.svg");
			else if (nLoadType ==  2)   strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType ==  3)   strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType ==  4)   strSVG = _T("cmd_mvhl_139.svg");
			else if (nLoadType ==  5)   strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType ==  6)   strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType ==  7)   strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType ==  8)   strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType ==  9)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 10)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 11)   strSVG = _T("cmd_mvhl_173.svg");
			else if (nLoadType == 12)   strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 13)   strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 14)   strSVG = _T("cmd_mvhl_173.svg");
			else if (nLoadType == 15)   strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 16)   strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 17)   strSVG = _T("cmd_mvhl_80.svg");
			else if (nLoadType == 18)   strSVG = _T("cmd_mvhl_106.svg");
			else ASSERT(0);
			break;
		case D_MVHL_CONNECTICUT:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_05.svg");
			else if (nLoadType ==  1)   strSVG = _T("cmd_mvhl_04.svg");
			else if (nLoadType ==  2)   strSVG = _T("cmd_mvhl_04.svg");
			else if (nLoadType ==  3)   strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType ==  4)   strSVG = _T("cmd_mvhl_07.svg");
			else if (nLoadType ==  5)   strSVG = _T("cmd_mvhl_07.svg");
			else if (nLoadType ==  6)   strSVG = _T("cmd_mvhl_80.svg");
			else if (nLoadType ==  7)   strSVG = _T("cmd_mvhl_80.svg");
			else if (nLoadType ==  8)   strSVG = _T("cmd_mvhl_182_NA_P180_9.svg");
			else if (nLoadType ==  9)   strSVG = _T("cmd_mvhl_183_NA_P200_10.svg");
			else if (nLoadType == 10)   strSVG = _T("cmd_mvhl_233.svg");
			else if (nLoadType == 11)   strSVG = _T("cmd_mvhl_184_NA_TLC.svg");
			else ASSERT(0);
			break;
		case D_MVHL_FLORIDA:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType ==  1)   strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType ==  2)   strSVG = _T("cmd_mvhl_184.svg");
			else if (nLoadType ==  3)   strSVG = _T("cmd_mvhl_148_new.svg");
			else if (nLoadType ==  4)   strSVG = _T("cmd_mvhl_149_new.svg");
			else if (nLoadType ==  5)   strSVG = _T("cmd_mvhl_150_new.svg");
			else if (nLoadType ==  6)   strSVG = _T("cmd_mvhl_149_new.svg");
			else if (nLoadType ==  7)   strSVG = _T("cmd_mvhl_150_new.svg");
			else if (nLoadType ==  8)   strSVG = _T("cmd_mvhl_151_new.svg");
			else if (nLoadType ==  9)   strSVG = _T("cmd_mvhl_151_new.svg");
			else if (nLoadType == 10)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 11)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 12)   strSVG = _T("cmd_mvhl_09.svg");
			else if (nLoadType == 13)   strSVG = _T("cmd_mvhl_11.svg");
			else ASSERT(0);
			break;
		case D_MVHL_GEORGIA:
			if (nLoadType == 0)        strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 1)   strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 2)   strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 3)   strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 4)   strSVG = _T("cmd_mvhl_04.svg");
			else if (nLoadType == 5)   strSVG = _T("cmd_mvhl_82.svg");
			else ASSERT(0);
			break;
		case D_MVHL_HAWAII:
			if (nLoadType == 0)        strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 1)   strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 2)   strSVG = _T("cmd_mvhl_185_NA_Hawaii.svg");
			else ASSERT(0);
			break;
		case D_MVHL_IDAHO:
			if (nLoadType == 0)        strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 1)   strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType == 2)   strSVG = _T("cmd_mvhl_186_NA_Idaho.svg");
			else if (nLoadType == 3)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 4)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 5)   strSVG = _T("cmd_mvhl_06.svg");
			else ASSERT(0);
			break;
		case D_MVHL_KANSAS:
			if (nLoadType == 0)        strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 1)   strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType == 2)   strSVG = _T("cmd_mvhl_09.svg");
			else if (nLoadType == 3)   strSVG = _T("cmd_mvhl_11.svg");
			else ASSERT(0);
			break;
		case D_MVHL_KENTUCKY:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType ==  1)   strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType ==  2)   strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType ==  3)   strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType ==  4)   strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType ==  5)   strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType ==  6)   strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType ==  7)   strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType ==  8)   strSVG = _T("cmd_mvhl_09.svg");
			else if (nLoadType ==  9)   strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType == 10)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 11)   strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 12)   strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 13)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 14)   strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 15)   strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 16)   strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 17)   strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 18)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 19)   strSVG = _T("cmd_mvhl_235.svg");
			else ASSERT(0);
			break;
		case D_MVHL_MAINE:
			if (nLoadType == 0)        strSVG = _T("cmd_mvhl_171_new.svg");
			else if (nLoadType == 1)   strSVG = _T("cmd_mvhl_171_new.svg");
			else if (nLoadType == 2)   strSVG = _T("cmd_mvhl_171_new.svg");
			else if (nLoadType == 3)   strSVG = _T("cmd_mvhl_177_new.svg");
			else if (nLoadType == 4)   strSVG = _T("cmd_mvhl_177_new.svg");
			else if (nLoadType == 5)   strSVG = _T("cmd_mvhl_148_new.svg");
			else if (nLoadType == 6)   strSVG = _T("cmd_mvhl_149_new.svg");
			else if (nLoadType == 7)   strSVG = _T("cmd_mvhl_150_new.svg");
			else ASSERT(0);
			break;
		case D_MVHL_MARYLAND:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType ==  1)   strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType ==  2)   strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType ==  3)   strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType ==  4)   strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType ==  5)   strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType ==  6)   strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType ==  7)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType ==  8)   strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType ==  9)   strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 10)   strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 11)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 12)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 13)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 14)   strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 15)   strSVG = _T("cmd_mvhl_226.svg");
			else ASSERT(0);
			break;
		case D_MVHL_MASSACHUSETTS:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType ==  1)   strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType ==  2)   strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType ==  3)   strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType ==  4)   strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType ==  5)   strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType ==  6)   strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType ==  7)   strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType ==  8)   strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType ==  9)   strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 10)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 11)   strSVG = _T("cmd_mvhl_235.svg");
			else ASSERT(0);
			break;
		case D_MVHL_MICHIGAN_NL:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 1)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 2)    strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 3)    strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 4)    strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 5)    strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 6)    strSVG = _T("cmd_mvhl_228.svg");
			else if (nLoadType == 7)    strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 8)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 9)    strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 10)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 11)   strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 12)   strSVG = _T("cmd_mvhl_189_NA_Michigan_13.svg");
			else if (nLoadType == 13)   strSVG = _T("cmd_mvhl_190_NA_Michigan_14.svg");
			else if (nLoadType == 14)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 15)   strSVG = _T("cmd_mvhl_191_NA_Michigan_16.svg");
			else if (nLoadType == 16)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 17)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 18)   strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 19)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 20)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 21)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 22)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 23)   strSVG = _T("cmd_mvhl_228.svg");
			else if (nLoadType == 24)   strSVG = _T("cmd_mvhl_230.svg");
			else ASSERT(0);
			break;
		case D_MVHL_MICHIGAN_DL:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 1)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 2)    strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 3)    strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 4)    strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 5)    strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 6)    strSVG = _T("cmd_mvhl_228.svg");
			else if (nLoadType == 7)    strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 8)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 9)    strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 10)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 11)   strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 12)   strSVG = _T("cmd_mvhl_189_NA_Michigan_13.svg");
			else if (nLoadType == 13)   strSVG = _T("cmd_mvhl_190_NA_Michigan_14.svg");
			else if (nLoadType == 14)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 15)   strSVG = _T("cmd_mvhl_191_NA_Michigan_16.svg");
			else if (nLoadType == 16)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 17)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 18)   strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 19)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 20)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 21)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 22)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 23)   strSVG = _T("cmd_mvhl_228.svg");
			else if (nLoadType == 24)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 25)   strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 26)   strSVG = _T("cmd_mvhl_06.svg");
			else ASSERT(0);
			break;
		case D_MVHL_MICHIGAN_SD:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 1)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 2)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 3)    strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 4)    strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 5)    strSVG = _T("cmd_mvhl_235.svg");
			else ASSERT(0);
			break;
		case D_MVHL_MINNESOTA:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 1)    strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 2)    strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 3)    strSVG = _T("cmd_mvhl_269_LaneType.svg");
			else if (nLoadType == 4)    strSVG = _T("cmd_mvhl_145.svg");
			else if (nLoadType == 5)    strSVG = _T("cmd_mvhl_145.svg");
			else if (nLoadType == 6)    strSVG = _T("cmd_mvhl_192_NA_Minnesota_C.svg");
			else if (nLoadType == 7)    strSVG = _T("cmd_mvhl_161.svg");
			else if (nLoadType == 8)    strSVG = _T("cmd_mvhl_161.svg");
			else if (nLoadType == 9)    strSVG = _T("cmd_mvhl_145.svg");
			else if (nLoadType == 10)    strSVG = _T("cmd_mvhl_145.svg");
			else if (nLoadType == 11)   strSVG = _T("cmd_mvhl_230.svg");
			else if (nLoadType == 12)   strSVG = _T("cmd_mvhl_231.svg");
			else if (nLoadType == 13)   strSVG = _T("cmd_mvhl_231.svg");
			else if (nLoadType == 14)   strSVG = _T("cmd_mvhl_231.svg");
			else if (nLoadType == 15)   strSVG = _T("cmd_mvhl_193_NA_C200J.svg");
			else if (nLoadType == 16)   strSVG = _T("cmd_mvhl_74.svg");
			else if (nLoadType == 17)   strSVG = _T("cmd_mvhl_194_NA_C174B.svg");
			else if (nLoadType == 18)   strSVG = _T("cmd_mvhl_195_NA_C198_23.svg");
			else if (nLoadType == 19)   strSVG = _T("cmd_mvhl_230.svg");
			else ASSERT(0);
			break;
		case D_MVHL_MISSISSIPI:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType == 1)    strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType == 2)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 3)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 4)    strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 5)    strSVG = _T("cmd_mvhl_226.svg");
			else ASSERT(0);
			break;
		case D_MVHL_NEBRASKA:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 1)    strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 2)    strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 3)    strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 4)    strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 5)    strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 6)    strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 7)    strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 8)    strSVG = _T("cmd_mvhl_235.svg");
			else ASSERT(0);
			break;
		case D_MVHL_NEWJERSEY:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType == 1)    strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType == 2)    strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType == 3)    strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType == 4)    strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 5)    strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 6)    strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 7)    strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 8)    strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType == 9)    strSVG = _T("cmd_mvhl_273_TP_16TDM.svg");
			else ASSERT(0);
			break;
		case D_MVHL_NEWMEXICO:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 1)    strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 2)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 3)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 4)    strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 5)    strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 6)    strSVG = _T("cmd_mvhl_226.svg");
			else ASSERT(0);
			break;
		case D_MVHL_NEWYORK:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 1)    strSVG = _T("cmd_mvhl_30_TDM.svg");
			else ASSERT(0);
			break;
		case D_MVHL_NORTHDAKOTA:
			if (nLoadType == 0)          strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 1)     strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType == 2)     strSVG = _T("cmd_mvhl_09.svg");
			else if (nLoadType == 3)     strSVG = _T("cmd_mvhl_11.svg");
			else if (nLoadType == 4)     strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 5)     strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 6)     strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 7)     strSVG = _T("cmd_mvhl_259_NDakotaLaneType.svg");
			else if (nLoadType == 8)     strSVG = _T("cmd_mvhl_139.svg");
			else if (nLoadType == 9)     strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 10)     strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 11)    strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 12)    strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 13)    strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 14)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 15)    strSVG = _T("cmd_mvhl_228.svg");
			else if (nLoadType == 16)    strSVG = _T("cmd_mvhl_228.svg");
			else ASSERT(0);
			break;
		case D_MVHL_OKLAHOMA:
			if (nLoadType == 0)         strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 1)    strSVG = _T("cmd_mvhl_30_TDM.svg");
			else ASSERT(0);
			break;
		case D_MVHL_OREGON:
			if (nLoadType == 0)          strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 1)     strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 2)     strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 3)     strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 4)     strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 5)     strSVG = _T("cmd_mvhl_200_NA_Oregon_STP_4B.svg");
			else if (nLoadType == 6)     strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 7)     strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 8)     strSVG = _T("cmd_mvhl_231.svg");
			else if (nLoadType == 9)     strSVG = _T("cmd_mvhl_201_NA_Oregon_STP_5BW.svg");
			else if (nLoadType == 10)    strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 11)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 12)    strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 13)    strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 14)    strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 15)    strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 16)    strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 17)    strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 18)    strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 19)    strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 20)    strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 21)    strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 22)    strSVG = _T("cmd_mvhl_83_new.svg");
			else ASSERT(0);
			break;
		case D_MVHL_SOUTHCAROLINA:
			if (nLoadType == 0)          strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 1)     strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 2)     strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 3)     strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 4)     strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 5)     strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 6)     strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 7)     strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 8)     strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 9)     strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 10)    strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 11)    strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 12)    strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 13)    strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 14)    strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 15)    strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 16)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 17)    strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 18)    strSVG = _T("cmd_mvhl_202_NA_Crane.svg");
			else if (nLoadType == 19)    strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 20)    strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 21)    strSVG = _T("cmd_mvhl_06.svg");
			else ASSERT(0);
			break;
		case D_MVHL_SOUTHDAKOTA:
			if (nLoadType == 0)          strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 1)     strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType == 2)     strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 3)     strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 4)     strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 5)     strSVG = _T("cmd_mvhl_263_SDakotaLaneType.svg");
			else if (nLoadType == 6)     strSVG = _T("cmd_mvhl_139.svg");
			else if (nLoadType == 7)     strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 8)     strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 9)     strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 10)    strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 11)    strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 12)    strSVG = _T("cmd_mvhl_235.svg");
			else ASSERT(0);
			break;
		case D_MVHL_TENNESSEE:
			if (nLoadType == 0)          strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 1)     strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType == 2)     strSVG = _T("cmd_mvhl_09.svg");
			else if (nLoadType == 3)     strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType == 4)     strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType == 5)     strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType == 6)     strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 7)     strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 8)     strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 9)     strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 10)    strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 11)    strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 12)    strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 13)    strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 14)    strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 15)    strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 16)    strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 17)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 18)    strSVG = _T("cmd_mvhl_231.svg");
			else ASSERT(0);
			break;
		case D_MVHL_TEXAS:
			if (nLoadType == 0)        strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 1)   strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 2)   strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 3)   strSVG = _T("cmd_mvhl_139.svg");
			else if (nLoadType == 4)   strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 5)   strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 6)   strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 7)   strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 8)   strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 9)   strSVG = _T("cmd_mvhl_235.svg");
			else ASSERT(0);
			break;
		case D_MVHL_UTAH:
			if (nLoadType == 0)          strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 1)     strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType == 2)     strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 3)     strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 4)     strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 5)     strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 6)     strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 7)     strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 8)     strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 9)     strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 10)    strSVG = _T("cmd_mvhl_228.svg");
			else if (nLoadType == 11)    strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 12)    strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 13)    strSVG = _T("cmd_mvhl_229.svg");
			else ASSERT(0);
			break;
		case D_MVHL_VERMONT:
			if (nLoadType == 0)          strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 1)     strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType == 2)     strSVG = _T("cmd_mvhl_08.svg");
			else if (nLoadType == 3)     strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 4)     strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 5)     strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 6)     strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 7)     strSVG = _T("cmd_mvhl_226.svg");
			else ASSERT(0);
			break;
		case D_MVHL_WASHINGTON:
			if (nLoadType == 0)          strSVG = _T("cmd_mvhl_177_new.svg");
			else if (nLoadType == 1)     strSVG = _T("cmd_mvhl_204_Washington.svg");
			else ASSERT(0);
			break;
		case D_MVHL_WESTVIRGINIA:
			if (nLoadType == 0)          strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 1)     strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType == 2)     strSVG = _T("cmd_mvhl_09.svg");
			else if (nLoadType == 3)     strSVG = _T("cmd_mvhl_10.svg");
			else if (nLoadType == 4)     strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 5)     strSVG = _T("cmd_mvhl_140.svg");
			else if (nLoadType == 6)     strSVG = _T("cmd_mvhl_141.svg");
			else if (nLoadType == 7)     strSVG = _T("cmd_mvhl_142.svg");
			else if (nLoadType == 8)     strSVG = _T("cmd_mvhl_143.svg");
			else if (nLoadType == 9)     strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 10)    strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 11)    strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 12)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 13)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 14)    strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 15)    strSVG = _T("cmd_mvhl_226.svg");
			else if (nLoadType == 16)    strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 17)    strSVG = _T("cmd_mvhl_06.svg");
			else if (nLoadType == 18)    strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 19)    strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 20)    strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 21)    strSVG = _T("cmd_mvhl_226.svg");
			else ASSERT(0);
			break;
		case D_MVHL_DELAWARE:
			if (nLoadType == 0)          strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 1)     strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 2)     strSVG = _T("cmd_mvhl_227.svg");
			else if (nLoadType == 3)     strSVG = _T("cmd_mvhl_226.svg");
			else ASSERT(0);
			break;
		case D_MVHL_ALASKA:
			if (nLoadType == 0)          strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 1)     strSVG = _T("cmd_mvhl_229.svg");
			else if (nLoadType == 2)     strSVG = _T("cmd_mvhl_229.svg");
			else ASSERT(0);
			break;
		case D_MVHL_DISTRICT_OF_COLUMBIA:
			if (nLoadType == 0)   strSVG = _T("cmd_mvhl_09.svg");
			else if (nLoadType == 1)   strSVG = _T("cmd_mvhl_11.svg");
			else if (nLoadType == 2)   strSVG = _T("cmd_mvhl_09.svg");
			else if (nLoadType == 3)   strSVG = _T("cmd_mvhl_11.svg");
			else if (nLoadType == 4)   strSVG = _T("cmd_mvhl_222.svg");
			else if (nLoadType == 5)   strSVG = _T("cmd_mvhl_30_TDM.svg");
			else if (nLoadType == 6)   strSVG = _T("cmd_mvhl_139.svg");
			else if (nLoadType == 7)   strSVG = _T("cmd_mvhl_81.svg");
			else if (nLoadType == 8)  strSVG = _T("cmd_mvhl_82.svg");
			else if (nLoadType == 9)  strSVG = _T("cmd_mvhl_83_new.svg");
			else if (nLoadType == 10)  strSVG = _T("cmd_mvhl_234.svg");
			else if (nLoadType == 11)  strSVG = _T("cmd_mvhl_235.svg");
			else if (nLoadType == 12)  strSVG = _T("cmd_mvhl_05.svg");
			else if (nLoadType == 13)  strSVG = _T("cmd_mvhl_80.svg");
			else ASSERT(0);
			break;
		default:
			ASSERT(0);
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

void CCMMvhlItemStdDlg::ResetVehicleTypeCmb() 
{
	int m_nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cbxCodeName, m_cbxCodeName.GetCurSel());
	SetLoadTypeList(m_nCodeType);
}

void CCMMvhlItemStdDlg::OnSelchangeCmdMvhlCodeCmb() 
{
	int nIndex = m_cbxCodeName.GetCurSel();
	m_nStndCode = m_cbxCodeName.GetItemData(nIndex);
	
	ResetVehicleTypeCmb();
	ChangeData();  
	AlignControlDynamic();
}

void CCMMvhlItemStdDlg::OnSelchangeCmdMvhlTypeCombo() 
{  
	int nIndex = m_cbxCodeName.GetCurSel();
	m_nStndCode = m_cbxCodeName.GetItemData(nIndex);

	ChangeData();  
	AlignControlDynamic();
}

BOOL CCMMvhlItemStdDlg::ChangeData()
{
	m_Data.Initialize();
	m_Data.bStandard = TRUE;
	m_wndVehicleTypeList.GetWindowText(m_Data.VehicleTypeName);
	m_Data.nStandardCode = m_nStndCode;

	int nLoadType;
	//int nIndex = GetIndex(m_Data.VehicleTypeName, &sIndex);
	//CDlgUtil::CobxSetCurSelItemData(m_wndCodeName, nIndex);  
	//m_wndVehicleTypeList.SetCurSel(sIndex);
	//ChangeBitmap(nIndex, sIndex);

	CDlgUtil::CobxSetCurSelItemData(m_cbxCodeName, m_nStndCode);
	GetIndex(nLoadType);
	ChangeBitmap(m_nStndCode, nLoadType);

	CVehlDB db(m_pDoc);
	if(db.GetStandardVehicleLoadValue(m_Data)) MakeItemEx();    
	//SetChinaAndAashtoLrfdCtrl(nIndex, sIndex);
	SetChinaAndAashtoLrfdCtrl(m_nStndCode, nLoadType);

	if(m_Data.SelVehicle.IsEmpty())
	{
		m_edtVehicleName.SetWindowText(m_Data.VehicleTypeName);
	}
	else
	{
		m_edtVehicleName.SetWindowText(m_Data.SelVehicle);
	}

	EnableDisableIncreAxleSpace();
	OnCmdMvhlEuroDynamicChk();
	OnCmdMvhlOverLoadChk();

	InitThreeGrid(nLoadType);

	ChangeCtrlText();

	BOOL bPennotLegal = FALSE;
	// V955
	{
		bPennotLegal = (m_nMovingType == D_MOVE_CODE_PENDOT && IsLegal(m_nStndCode, nLoadType));
	}

	if(m_nStndCode == D_MVHL_AASHTO_LRFD || m_nMovingType == D_MOVE_CODE_AASHTO_LRFD || m_nMovingType == D_MOVE_CODE_AASHTO_STAN || bPennotLegal)
	{
		if (m_nStndCode == D_MVHL_TURKEY)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_AASHTO_LRFD)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_COLOMBIA) {
			if (IsPermit(m_nStndCode, nLoadType))
			{
				m_edtLegalW.SetEditUnit(m_Data.dCombinedUniformLoad);
			}
			else
			{
				m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
			}
		}
		else if (m_nStndCode == D_MVHL_CALIFORNIA && (nLoadType == 0 || nLoadType == 1))
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_COLORADO && nLoadType == 2)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_DISTRICT_OF_COLUMBIA && nLoadType == 4)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_FLORIDA && nLoadType == 0)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_IDAHO && nLoadType == 0)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_KANSAS && nLoadType == 0)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_KENTUCKY && (nLoadType == 0 || nLoadType == 2))
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_MARYLAND && nLoadType == 0)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_NORTHDAKOTA && nLoadType == 0)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_NEWJERSEY && nLoadType == 7)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_NEWYORK && nLoadType == 0)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_SOUTHDAKOTA && nLoadType == 0)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_TENNESSEE && nLoadType == 0)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_UTAH && nLoadType == 0)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else if (m_nStndCode == D_MVHL_WESTVIRGINIA && nLoadType == 0)
		{
			m_edtLegalW.SetEditUnit(m_Data.dUniformLoad);
		}
		else {
			m_edtLegalW.SetEditUnit(m_Data.dCombinedUniformLoad);
		}
		
		m_edtTdmW.SetEditUnit(m_Data.dUniformLoad);
		m_edtTdmD2.SetEditUnit(m_Data.dD2);
		m_edtTdmD3.SetEditUnit(m_Data.dD3);

		if (m_nStndCode == D_MVHL_AASHTO_LRFD)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_CALIFORNIA && nLoadType == 0)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_COLORADO && nLoadType == 2)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_DISTRICT_OF_COLUMBIA && nLoadType == 4)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_FLORIDA && nLoadType == 0)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_IDAHO && nLoadType == 0)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_KANSAS && nLoadType == 0)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_KENTUCKY && (nLoadType == 0 || nLoadType == 2))
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_MARYLAND && nLoadType == 0)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_NORTHDAKOTA && nLoadType == 0)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_NEWJERSEY && nLoadType == 7)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_NEWYORK && nLoadType == 0)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_SOUTHDAKOTA && nLoadType == 0)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_TENNESSEE && nLoadType == 0)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_UTAH && nLoadType == 0)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_WESTVIRGINIA && nLoadType == 0)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_TURKEY)
		{
			m_edtLegalR.SetEditUnit(90);
		}
		else if (m_nStndCode == D_MVHL_COLOMBIA)
		{
			if (IsPermit(m_nStndCode, nLoadType))
			{
				m_edtLegalR.SetEditUnit(75);
			}
			else
			{
				m_edtLegalR.SetEditUnit(100);
			}
		}
		else if (m_nStndCode == D_MVHL_AASHTO_ILDOT && nLoadType == 22)
		{
				m_edtLegalR.SetEditUnit(100);
		}
		else if (m_nStndCode == D_MVHL_MINNESOTA && nLoadType == 3)
		{
			m_edtLegalR.SetEditUnit(100);
		}
		else if (m_nStndCode == D_MVHL_NORTHDAKOTA && nLoadType == 7)
		{
			m_edtLegalR.SetEditUnit(100);
		}
		else if (m_nStndCode == D_MVHL_SOUTHDAKOTA && nLoadType == 5)
		{
			m_edtLegalR.SetEditUnit(100);
		}
		else if (m_nStndCode == D_MVHL_NEWJERSEY && nLoadType == 9)
		{
			m_edtLegalR.SetEditUnit(100);
		}
		else
		{
			m_edtLegalR.SetEditUnit(75);
		}
		
		//m_edtLegalDist.SetEditUnit(m_Data.dCombinedTruckSpace);
		CString strLegalDist;
		if(m_nStndCode == D_MVHL_COLOMBIA && nLoadType == 1)
		{
			double d8 =  m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 8.0);
			double d12 = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 12.0);

			strLegalDist.Format(_T("%g~%g"), d8, d12);
		}
		else
		{
			strLegalDist.Format(_T("%g"), m_Data.dCombinedTruckSpace); 
		}

		m_edtLegalDist.SetWindowText(strLegalDist);

		// V955
		{
			if (m_nStndCode == D_MVHL_AASHTO_PENDOT)
			{
				m_edtLegalW.SetEditUnit(m_Data.dCombinedUniformLoad);
				m_edtLegalR.SetEditUnit(100);
			}

			if (m_nStndCode == D_MVHL_AASHTO_PENDOT && nLoadType == 1)
			{
				double d26 = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, 26.0);
				double d40 = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, 40.0);

				strLegalDist.Format(_T("%g~%g"), d26, d40);
				m_edtLegalDist.SetWindowText(strLegalDist);
			}

			if (m_nStndCode == D_MVHL_AASHTO_PENDOT && nLoadType == 0)
			{
				double d50 = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, 50.0);
				CString strPenndotDist;
				strPenndotDist.Format(_T("%g"), d50);

				m_edtPenndotW.SetEditUnit(m_Data.dCombinedUniformLoad);
				m_edtPenndotR1.SetEditUnit(90.0);
				m_edtPenndotR2.SetEditUnit(100.0);
				m_edtPenndotDist.SetWindowText(strPenndotDist);
			}
		}
	}

	return TRUE;
}

void CCMMvhlItemStdDlg::ChangeCtrlText()
{
	CString strText;

	strText.Format(_T("%s = 1.40 -L / 500  (1<= %s<= 1.40 )"), CSpecialChar::m_csPhi, CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->SetWindowText(strText);

	strText.Format(_T("%s :"), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->SetWindowText(strText);  

	if(m_nStndCode == D_MVHL_BS_BD8611)
	{
		int nSelVehicle = m_wndVehicleTypeList.GetCurSel();
		if(nSelVehicle == 0 || nSelVehicle == 1 || nSelVehicle == 2 || nSelVehicle == 3 || nSelVehicle == 4)
		{
			GetDlgItem(IDC_CMD_MVHL_SVSOV_A_STC)->SetWindowText(_LS(IDS_CMD_MVHL_EURO_BS_SOSOV_TEXT_1));
		}
		else
		{
			GetDlgItem(IDC_CMD_MVHL_SVSOV_A_STC)->SetWindowText(_LS(IDS_CMD_MVHL_EURO_BS_SOSOV_TEXT_2));
		}
	}
}

void CCMMvhlItemStdDlg::EnableDisableIncreAxleSpace()
{
	BOOL bUseIncre = FALSE;
	if(m_Data.VehicleTypeName == _T("BCL-625 Truck") || m_Data.VehicleTypeName == _T("BCL-625 Lane")) bUseIncre = TRUE;
	
	GetDlgItem(IDC_CMD_MVHL_INCRE_AXSPACE_CHK)->EnableWindow(bUseIncre);
	GetDlgItem(IDC_CMD_MVHL_INCRE_AXSPACE_EDT)->EnableWindow(bUseIncre);
	GetDlgItem(IDC_CMD_MVHL_INCRE_AXSPACE_UNT)->EnableWindow(bUseIncre);	
}


//-------------------------------------------------------------------------
// Implementation
void CCMMvhlItemStdDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Load), _LS(IDS_WG_CMD__ADDD__Spacing)};

	// title에 단위 추가
	CString sLdUnit,sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");

	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 67; nColWidth[2] = 85;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlItemStdDlg::MakeItemEx()
{
	m_List.DeleteAllItems();    
	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistCount();
	
	if (nLoadCount > 0)
	{
		int i = 0;
		for (i = 0; i < nDistCount; i++)
			InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);

		if (nLoadCount > nDistCount)
			InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
	}

	m_dD1 = m_Data.dD1;
	m_dD2 = m_Data.dD2;
	m_dW1 = m_Data.dW1;
	m_dW2 = m_Data.dW2;
	m_W		= m_Data.dUniformLoad;
	m_Ps	= m_Data.dPLV;
	m_Pm	= m_Data.dPLM;
	m_dP  = m_Data.dP;
	m_dQm = m_Data.dQm;
	m_dQq = m_Data.dQq;

	if(m_Data.nStandardCode == D_MVHL_CH_JTJ001_97 && m_Data.VehicleTypeName == _T("LD-50") ) // China(JTJ001-97) 궤도차량 nTruckType = 1, dDTrailer = 4.5,  dWTrailer = 56.; 
	{
		m_dD1 = m_Data.dDTrailer;
		m_dW1 = m_Data.dWTrailer;	
	}
	// BS5400
	if(m_Data.nStandardCode == D_MVHL_BS_5400 || m_Data.nStandardCode == D_MVHL_BS_BD3701)
	{
		if(m_Data.nStandardCode == D_MVHL_BS_5400)
		{
			m_dBSW1 = m_Data.dW1;
			m_dBSW2 = m_Data.dW2;
			m_dBSW3 = m_Data.dW3;
			m_dBSL1 = m_Data.dLoadedLength;
			m_dBSL2 = m_Data.dLoadedLength;

			double dExp = 0.475;
			BOOL bCalc = TRUE;
			if(m_dBSW1 <= 0. || m_dBSW2 <= 0.) bCalc = FALSE;
			if(m_dBSL1 <= 0. || m_dBSL1 == 1.) bCalc = FALSE; 
			if(bCalc) dExp = log10(m_dBSW2/m_dBSW1) / log10(m_dBSL1);
			else      dExp = 0.475;
			m_strBSW2_exp.Format(_T("%.3g"), dExp);
		}
		else if(m_Data.nStandardCode == D_MVHL_BS_BD3701)
		{
			m_dBS37W1		= m_Data.dW1;
			m_dBS37W2		= m_Data.dW2;
			m_dBS37W3		= m_Data.dW3;
			m_dBS37L1		= m_Data.dLoadedLength;
			m_dBS37L2		= m_Data.dLoadedLength;
			m_dBS37L21		= m_Data.dLoadedLength2;
			m_dBS37L3		= m_Data.dLoadedLength2;

			double dExp = 0.1;
			BOOL bCalc = TRUE;
			if(m_dBS37W1 <= 0. || m_dBS37W2 <= 0.) bCalc = FALSE;
			if(m_dBS37L1 <= 0. || m_dBS37L1 == 1.) bCalc = FALSE;
			if(bCalc)
			{
				double dW = m_dBS37W1*pow(1.0/m_dBS37L1, 0.671);
				dExp = log10(m_dBS37W2/dW) / log10(m_dBS37L1);
			}
			m_strBS37W2_exp.Format(_T("%.3g"), dExp);

			m_edtBSdd.SetEditUnit(m_Data.dDD); // HA & HB

			// pedestrian
			m_edtPedW1.SetEditUnit(m_Data.dW1);
			CString csVal;
			m_edtPedW1.GetWindowText(csVal);
			csVal = _T("k*")+csVal;
			m_edtPedW2.SetWindowText(csVal);
			m_edtPedL1.SetEditUnit(m_Data.dLoadedLength);
			m_edtPedL1.GetWindowText(csVal);
			m_edtPedL2.SetWindowText(csVal);
		}
		m_dBSPa = m_Data.dPa;
		m_dBSPb = m_Data.dPb;
		m_dBSD1 = m_Data.dD1;
		m_dBSD2 = m_Data.dD2;
		m_dBSD  = m_Data.dDelta;
		m_dBSD3 = m_dBSD2 + m_dBSD;
		m_dBSD4 = m_dBSD2 + m_dBSD * 2.;
		m_dBSD5 = m_dBSD2 + m_dBSD * 3.;
		m_dBSD6 = m_dBSD2 + m_dBSD * 4.;

		m_edtNumUnit.SetEditUnit(m_Data.dUnitNum);
	}
	if(m_Data.nStandardCode == D_MVHL_BS_CS454 )
	{
		//ASSERT(0);
	}
	if(m_Data.nStandardCode == D_MVHL_CH_JTGB01_2003 || m_Data.nStandardCode == D_MVHL_CH_JTGB01_2014)
	{
		if(m_Data.VehicleTypeName == _T("CH-CD") || m_Data.VehicleTypeName == _T("CH-PL1"))
		{
			m_dQkJTGLane  = m_Data.dQq;
			m_dPk1JTGLane = m_Data.dPa;
			m_dPk2JTGLane = m_Data.dPb;
			m_dL1JTGLane  = m_Data.dD1;
			m_dL2JTGLane  = m_Data.dD2;  
		}
		else if(m_Data.VehicleTypeName == _T("CH-RQ"))
		{
			m_dW1JTGCrowd = m_Data.dWPres1;
			m_dW2JTGCrowd = m_Data.dWPres2;
			m_dL1JTGCrowd = m_Data.dD1;
			m_dL2JTGCrowd = m_Data.dD2;
			//m_dWdJTGCrowd = m_Data.dDb;
		}
	}

	if(m_Data.nStandardCode == D_MVHL_CH_CJJ11_2011 ||
	   m_Data.nStandardCode == D_MVHL_CH_CJJ11_2019)
	{
		if(m_Data.VehicleTypeName == _T("C-CD(A/B)"))
		{
			m_dQkJTGLane  = m_Data.dQq;
			m_dPk1JTGLane = m_Data.dPa;
			m_dPk2JTGLane = m_Data.dPb;
			m_dL1JTGLane  = m_Data.dD1;
			m_dL2JTGLane  = m_Data.dD2;  
		}
	}

	UpdateData(FALSE);
}


BOOL CCMMvhlItemStdDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
{
	LVITEM lvitem;
	CString str;

	lvitem.iItem = nPos;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		if (i == 0) str.Format(_T("%d"), nNo);
		else if (i == 1) str.Format(_T("%g"), dLoad);
		else if (i == 2)
		{
			double dDistTmp = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dDist);
			
//      if (dDistTmp == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
//      else if (dDistTmp == -1.0) str = _LS(IDS_WG_CMD_INFINITE);
// 			else if (dDistTmp == -2.0) str = _LS(IDS_WG_CMD_VARIABLE);
//      else str.Format(_T("%g"), dDist);

			double dTol = 1.e-10;
			if      (fabs(dDistTmp)       < dTol) str = _LS(IDS_WG_CMD__ADDD__end);
			else if (fabs(dDistTmp + 1.0) < dTol) str = _LS(IDS_WG_CMD_INFINITE);
			else if (fabs(dDistTmp + 2.0) < dTol) str = _LS(IDS_WG_CMD_VARIABLE);
			else                                  str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if(i == 0)
			m_List.InsertItem(&lvitem);
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

int CCMMvhlItemStdDlg::GetTruckLoadCount()
{
	int i = 0;
	for(i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemStdDlg::GetTruckDistCount()
{
	int i = 0;
	for(i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}  
	return i;
}

// nIndex - 0: KSL, 1: KSTL, 2: AASHTO, 3: CSL, 4: JTJ001-97, 5: CJJ77-98, 6: TB10002.1-99,
//          7: OTHERS, 8: AASHTO_LRFD, 9: IRC6-2000
void CCMMvhlItemStdDlg::SetLoadTypeList(int nIndex)
{
	if (nIndex < 0) return;

	CArray<CString, CString> aVehicleType;
	int i = 0;
	while (i < D_MAX_COUNT && CVehlDB::GetTypeListAr()[nIndex][i] != _T(""))
	{
		aVehicleType.Add(CVehlDB::GetTypeListAr()[nIndex][i++]);
	}

	if(aVehicleType.GetSize() == 0) return;

	m_wndVehicleTypeList.ResetContent();

	if(m_nMovingType == D_MOVE_CODE_AASHTO_STAN && nIndex == D_MVHL_TURKEY)
	{
		for(i=1; i<aVehicleType.GetSize(); i++)
		{
			m_wndVehicleTypeList.AddString(aVehicleType[i]);
		}

		m_wndVehicleTypeList.AddString(aVehicleType[0]);
	}
	else
	{
		for(i=0; i<aVehicleType.GetSize(); i++)
		{
			m_wndVehicleTypeList.AddString(aVehicleType[i]);
		}
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_wndVehicleTypeList);
	m_wndVehicleTypeList.SetCurSel(0);

//   m_wndVehicleTypeList.ResetContent();
//   int i = 0;
//   while (i < D_MAX_COUNT && CVehlDB::GetTypeListAr()[nIndex][i] != _T(""))
//   {
//     m_wndVehicleTypeList.AddString(CVehlDB::GetTypeListAr()[nIndex][i++]);
//   }
//   m_wndVehicleTypeList.SetCurSel(0);
}

void CCMMvhlItemStdDlg::GetIndex(int& nLoadType)
{  
	// hslee : 이 함수 VehlDB로 빼서 다같이 써야 함...
	nLoadType = 0;
	if (m_nStndCode == 0)
		return;
	
	BOOL bBreakLoadType = TRUE;

	CArray<CString, CString> aVehicleType;
	if(m_nMovingType == D_MOVE_CODE_AASHTO_STAN && m_Data.nStandardCode == D_MVHL_TURKEY)
	{
		for (int i=1; i < D_MAX_COUNT; i++)
		{
			if(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i] == _T("")) continue;
			aVehicleType.Add(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i]);
		}
		aVehicleType.Add(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][0]);
	}
	else
	{
		for (int i=0; i < D_MAX_COUNT; i++)
		{
			if(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i] == _T("")) continue;
			aVehicleType.Add(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i]);
		}
	}
	
	for(int i=0; i<aVehicleType.GetSize(); i++)
	{
		if(aVehicleType[i] == m_Data.VehicleTypeName)
		{
			nLoadType = i;
			bBreakLoadType = TRUE;
			break;
		}
	}

//   for (int i = 0; i < D_MAX_COUNT; i++)
//   {
//     if(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i] == m_Data.VehicleTypeName)
//     {
//       nLoadType = i;
//       bBreakLoadType = TRUE;
//       break;
//     }
//   }
	
	if(bBreakLoadType == FALSE)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
	}
}

void CCMMvhlItemStdDlg::OnChangeLaneFactorCS()
{
	SetEnableBS_CS();
}

void CCMMvhlItemStdDlg::SetEnableBS_CS()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_BS_CS_LANE_FACT_USER_STC)->EnableWindow(m_nLaneFactorCS == 1);
	GetDlgItem(IDC_CMD_MVHL_BS_CS_LANE_FACT_USER_BTN)->EnableWindow(m_nLaneFactorCS == 1);	
}


void CCMMvhlItemStdDlg::OnChangeLaneFactor()
{
	SetEnableBS();
}

void CCMMvhlItemStdDlg::SetEnableBS()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_STC)->EnableWindow(m_nLaneFactor == 2);
	GetDlgItem(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_BTN)->EnableWindow(m_nLaneFactor == 2);

	if(m_nLaneFactor == 0)
	{    
		m_chkAddData.EnableWindow(FALSE);
	}
	else
	{
		m_chkAddData.EnableWindow(TRUE);
	}

	BOOL bAddData = m_chkAddData.GetCheck() && m_chkAddData.IsWindowEnabled();
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_AL_EDT2)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_AL_UNT)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_LOAD_LEVEL_CBO)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_AD_FACTOR_ST)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_AL_ST)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_RD_FACTOR_ST)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_CATEGORY_ST2)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_LOADLEVEL_ST2)->EnableWindow(bAddData);
}

void CCMMvhlItemStdDlg::OnCmdMvhlEuroAutoRdo()
{
	OnCmdMvhlEuroDynamicChk();
}

void CCMMvhlItemStdDlg::OnCmdMvhlEuroDynamicChk()
{
	BOOL bDynamic = m_wndDynamic.GetCheck();

	CArray<UINT,UINT> aRadio;
	int nAuto = 0;
	aRadio.Add(IDC_CMD_MVHL_AUTO_RDO);
	aRadio.Add(IDC_CMD_MVHL_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this, aRadio, nAuto);

	GetDlgItem(IDC_CMD_MVHL_AUTO_RDO)->EnableWindow(bDynamic);
	GetDlgItem(IDC_CMD_MVHL_USER_RDO)->EnableWindow(bDynamic);
	GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->EnableWindow(bDynamic);
	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->EnableWindow(bDynamic && (nAuto==1));
	GetDlgItem(IDC_CMD_MVHL_PHI_EDT)->EnableWindow(bDynamic && (nAuto==1));
}

void CCMMvhlItemStdDlg::OnCmdMvhlOverLoadRdo()
{
	OnCmdMvhlOverLoadChk();
}

void CCMMvhlItemStdDlg::OnCmdMvhlOverLoadChk()
{
	BOOL bOverLoadFactor = m_chkOverLoadFactor.GetCheck();

	CArray<UINT,UINT> aRadio;
	int nAuto = 0;
	aRadio.Add(IDC_OVERLOAD_FACTOR_AUTO_RDO);
	aRadio.Add(IDC_OVERLOAD_FACTOR_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this, aRadio, nAuto);

	GetDlgItem(IDC_OVERLOAD_FACTOR_AUTO_RDO)->EnableWindow(bOverLoadFactor);
	GetDlgItem(IDC_OVERLOAD_FACTOR_USER_RDO)->EnableWindow(bOverLoadFactor);
	GetDlgItem(IDC_OVERLOAD_FACTOR_CRI_AXLE_STC)->EnableWindow(bOverLoadFactor && (nAuto==1));
	GetDlgItem(IDC_OVERLOAD_FACTOR_CRI_AXLE_EDT)->EnableWindow(bOverLoadFactor && (nAuto==1));
	GetDlgItem(IDC_OVERLOAD_FACTOR_OTHER_AXLE_STC)->EnableWindow(bOverLoadFactor && (nAuto==1));
	GetDlgItem(IDC_OVERLOAD_FACTOR_OTHER_AXLE_EDT)->EnableWindow(bOverLoadFactor && (nAuto==1));
}

void CCMMvhlItemStdDlg::OnChangeDynaTypeRdo()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_DYNA_1AXLE_EDT)->EnableWindow(m_nDynaType == 1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_1AXLE_STC)->EnableWindow(m_nDynaType == 1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_2AXLE_EDT)->EnableWindow(m_nDynaType == 1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_2AXLE_STC)->EnableWindow(m_nDynaType == 1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_3AXLE_EDT)->EnableWindow(m_nDynaType == 1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_3AXLE_STC)->EnableWindow(m_nDynaType == 1);
}

void CCMMvhlItemStdDlg::OnBnClickedIncreAxleSpaceChk()
{
	BOOL bIncreAxleSpace = (m_chkIncreAxleSpace.GetCheck())? TRUE : FALSE;
	GetDlgItem(IDC_CMD_MVHL_INCRE_AXSPACE_EDT)->EnableWindow(bIncreAxleSpace);
	GetDlgItem(IDC_CMD_MVHL_INCRE_AXSPACE_UNT)->EnableWindow(bIncreAxleSpace);
}

void CCMMvhlItemStdDlg::SetCtrlUnit()
{
	m_stW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dUniformLoad);
	m_stPs.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPLV);
	m_stPm.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPLM);
	m_stdW2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW2);
	m_stdW1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW1);
	m_stdD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_stdD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);
	m_stP.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dP);
	m_stQm.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dQm);
	m_stQq.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dQq);

	m_untBSW1.SetTextFmt(_LSX(kN/_));
	m_untBSW1.SetUnitType(D_UNITSYS_NONE);
	m_untBSW2.SetTextFmt(_LSX(kN/m));
	m_untBSW2.SetUnitType(D_UNITSYS_NONE);
	m_untBSW3.SetTextFmt(_LSX(kN/m));
	m_untBSW3.SetUnitType(D_UNITSYS_NONE);
	m_untBSL1.SetTextFmt(_LSX(m));
	m_untBSL1.SetUnitType(D_UNITSYS_NONE);
	m_untBSL2.SetTextFmt(_LSX(m));
	m_untBSL2.SetUnitType(D_UNITSYS_NONE);

	m_untBS37W1.SetTextFmt(_LSX(kN/m));
	m_untBS37W1.SetUnitType(D_UNITSYS_NONE);
	m_untBS37W2.SetTextFmt(_LSX(kN/m));
	m_untBS37W2.SetUnitType(D_UNITSYS_NONE);
	m_untBS37W3.SetTextFmt(_LSX(kN/m));
	m_untBS37W3.SetUnitType(D_UNITSYS_NONE);
	m_untBS37L1.SetTextFmt(_LSX(m));
	m_untBS37L1.SetUnitType(D_UNITSYS_NONE);
	m_untBS37L2.SetTextFmt(_LSX(m));
	m_untBS37L2.SetUnitType(D_UNITSYS_NONE);
	m_untBS37L3.SetTextFmt(_LSX(m));
	m_untBS37L3.SetUnitType(D_UNITSYS_NONE);

	m_edtPedW1.SetUnitType(D_UNITSYS_NONE);
	m_untPedW1.SetTextFmt(_LSX(kN/m)+CUnitCtrl::m_str2);
	m_untPedW1.SetUnitType(D_UNITSYS_NONE);

	m_untPedW2.SetTextFmt(_LSX(kN/m)+CUnitCtrl::m_str2);
	m_untPedW2.SetUnitType(D_UNITSYS_NONE);

	m_edtPedL1.SetUnitType(D_UNITSYS_NONE);
	m_untPedL1.SetTextFmt(_LSX(m));
	m_untPedL1.SetUnitType(D_UNITSYS_NONE);

	m_untPedL2.SetTextFmt(_LSX(m));
	m_untPedL2.SetUnitType(D_UNITSYS_NONE);

	m_untBSPa.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPa);
	m_untBSPb.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPb);
	m_edtNumUnit.SetUnitType(D_UNITSYS_NONE);
	m_edtBSdd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSdd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD4.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD5.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD6.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);

	m_untJTGQkLane.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dQq);
	m_untJTGPk1Lane.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPa);
	m_untJTGPk2Lane.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPb);
	m_untJTGL1Lane.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);
	m_untJTGL2Lane.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);

	m_untJTGW1Crowd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dWPres1);
	m_untJTGW2Crowd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dWPres2);
	m_untJTGL1Crowd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);
	m_untJTGL2Crowd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_untJTGWdCrowd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDb);
	
	m_edtIncreAxleSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untIncreAxleSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edt1Axle.SetUnitType(D_UNITSYS_NONE);
	m_edt2Axle.SetUnitType(D_UNITSYS_NONE);
	m_edt3Axle.SetUnitType(D_UNITSYS_NONE);

	m_edtCJJ11wp.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_undCJJ11wp.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_untDD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDD);

	m_edtaL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untaL.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtLegalW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedUniformLoad);
	m_untLegalW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedUniformLoad);
	m_edtLegalR.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLegalR);
	m_untLegalR.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLegalR);
// 	m_edtLegalDist.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedTruckSpace);
// 	m_untLegalDist.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedTruckSpace);
	m_edtz.SetUnitType(D_UNITSYS_NONE);

	m_edtPenndotW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedUniformLoad);
	m_untPenndotW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedUniformLoad);
	m_edtPenndotR1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLegalR);
	m_untPenndotR1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLegalR);
	m_edtPenndotR2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLegalR);
	m_untPenndotR2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLegalR);
	m_edtPenndotDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPenndotDist.SetUnitType(D_UNITSYS_BASE_LENGTH);


	m_edtTdmW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedUniformLoad);
	m_untTdmW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedUniformLoad);
	m_edtTdmD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTdmD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtTdmD3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTdmD3.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdDlg::SetDefValAndCbxItem()
{
	CString csDefaultType;
	CString csDefaultVehi;
	int nDefaultStndCode = 1;

	int nCodeSeq[57];
	int nNumIter = 9;

	switch(m_nMovingType)
	{
	case D_MOVE_CODE_AASHTO_STAN:
		nCodeSeq[0] = D_MVHL_AASHTO_STAND;
		nCodeSeq[1] = D_MVHL_AASHTO_LEGAL;
		nCodeSeq[2] = D_MVHL_CALTRANS_STND_2017;
		nCodeSeq[3] = D_MVHL_CALTRANS_STND;
		nCodeSeq[4] = D_MVHL_TURKEY;
		nCodeSeq[5] = D_MVHL_OTHERS;
		csDefaultType = _T("HS20-44"); 
		csDefaultVehi = _T("");
		nNumIter = 6;
		nDefaultStndCode = D_MVHL_AASHTO_STAND;
		break;
	case D_MOVE_CODE_AASHTO_LRFD:
		nCodeSeq[ 0] = D_MVHL_AASHTO_LRFD;
		nCodeSeq[ 1] = D_MVHL_AASHTO_STAND;
		nCodeSeq[ 2] = D_MVHL_AASHTO_LRFD_LEGAL;
		nCodeSeq[ 3] = D_MVHL_ALASKA;
		nCodeSeq[ 4] = D_MVHL_ALABAMA;
		nCodeSeq[ 5] = D_MVHL_ARIZONA;
		nCodeSeq[ 6] = D_MVHL_ARKANSAS;
		nCodeSeq[ 7] = D_MVHL_CALIFORNIA;
		nCodeSeq[ 8] = D_MVHL_COLORADO;
		nCodeSeq[ 9] = D_MVHL_CONNECTICUT;
		nCodeSeq[10] = D_MVHL_DISTRICT_OF_COLUMBIA;
		nCodeSeq[11] = D_MVHL_DELAWARE;
		nCodeSeq[12] = D_MVHL_FLORIDA;
		nCodeSeq[13] = D_MVHL_GEORGIA;
		nCodeSeq[14] = D_MVHL_HAWAII;
		nCodeSeq[15] = D_MVHL_AASHTO_IADOT;
		nCodeSeq[16] = D_MVHL_IDAHO;
		nCodeSeq[17] = D_MVHL_AASHTO_ILDOT;
		nCodeSeq[18] = D_MVHL_AASHTO_INDOT;
		nCodeSeq[19] = D_MVHL_KANSAS;
		nCodeSeq[20] = D_MVHL_KENTUCKY;
		nCodeSeq[21] = D_MVHL_AASHTO_LADOT;
		nCodeSeq[22] = D_MVHL_MASSACHUSETTS;
		nCodeSeq[23] = D_MVHL_MARYLAND;
		nCodeSeq[24] = D_MVHL_MAINE;
		nCodeSeq[25] = D_MVHL_MICHIGAN_NL;
		nCodeSeq[26] = D_MVHL_MICHIGAN_DL;
		nCodeSeq[27] = D_MVHL_MICHIGAN_SD;
		nCodeSeq[28] = D_MVHL_MINNESOTA;
		nCodeSeq[29] = D_MVHL_AASHTO_MODOT;
		nCodeSeq[30] = D_MVHL_MISSISSIPI;
		nCodeSeq[31] = D_MVHL_NORTHDAKOTA;
		nCodeSeq[32] = D_MVHL_NEBRASKA;
		nCodeSeq[33] = D_MVHL_NEVADA;
		nCodeSeq[34] = D_MVHL_NEWJERSEY;
		nCodeSeq[35] = D_MVHL_NEWMEXICO;
		nCodeSeq[36] = D_MVHL_NEWYORK;
		nCodeSeq[37] = D_MVHL_AASHTO_OHDOT;
		nCodeSeq[38] = D_MVHL_OKLAHOMA;
		nCodeSeq[39] = D_MVHL_OREGON;
		nCodeSeq[40] = D_MVHL_AASHTO_RIDOT;
		nCodeSeq[41] = D_MVHL_SOUTHCAROLINA;
		nCodeSeq[42] = D_MVHL_SOUTHDAKOTA;
		nCodeSeq[43] = D_MVHL_TENNESSEE;
		nCodeSeq[44] = D_MVHL_TEXAS;
		nCodeSeq[45] = D_MVHL_UTAH;
		nCodeSeq[46] = D_MVHL_AASHTO_VADOT;
		nCodeSeq[47] = D_MVHL_VERMONT;
		nCodeSeq[48] = D_MVHL_WASHINGTON;
		nCodeSeq[49] = D_MVHL_AASHTO_WIDOT;
		nCodeSeq[50] = D_MVHL_WESTVIRGINIA;
		nCodeSeq[51] = D_MVHL_FAST_ACT_EV_LOAD;
		nCodeSeq[52] = D_MVHL_CALTRANS_STND_2017;
		nCodeSeq[53] = D_MVHL_CALTRANS_STND;
		nCodeSeq[54] = D_MVHL_TURKEY;
		nCodeSeq[55] = D_MVHL_COLOMBIA;
		nCodeSeq[56] = D_MVHL_OTHERS;
		csDefaultType = _T("HL-93TRK"); 
		csDefaultVehi = _T("");
		nNumIter = 57;
		nDefaultStndCode = D_MVHL_AASHTO_LRFD;
		break;
	case D_MOVE_CODE_CHINA:      
		nCodeSeq[0] = D_MVHL_CH_JTGB01_2014;
		nCodeSeq[1] = D_MVHL_CH_JTGB01_2003;
		nCodeSeq[2] = D_MVHL_CH_JTJ001_97;
		nCodeSeq[3] = D_MVHL_CH_CJJ77_98;
		nCodeSeq[4] = D_MVHL_CH_CJJ11_2011;
		nCodeSeq[5] = D_MVHL_CH_CJJ11_2019;
		nCodeSeq[6] = D_MVHL_CH_TB100021_99;
		nCodeSeq[7] = D_MVHL_CH_TB10002_2017;
		nCodeSeq[8] = D_MVHL_CH_QCR9300_2018;
			
		csDefaultType = _T("CH-CD");
		csDefaultVehi = _T("");
		nNumIter = 9;
		nDefaultStndCode = D_MVHL_CH_JTGB01_2014;		
		break;
	case D_MOVE_CODE_TAIWAN:
		nCodeSeq[0] = D_MVHL_ROC_TAIWAN;
		csDefaultType = _T("HS20-44(MS18)");
		csDefaultVehi = _T("");
		nNumIter = 1;
		nDefaultStndCode = D_MVHL_ROC_TAIWAN;
		break;
	case D_MOVE_CODE_JAPAN_RAIL:
	case D_MOVE_CODE_JAPAN:
		ASSERT(0);
		nCodeSeq[0] = D_MVHL_AASHTO_STAND;
		nCodeSeq[1] = D_MVHL_AASHTO_LRFD;
		nCodeSeq[2] = D_MVHL_CALTRANS_STND_2017;
		nCodeSeq[3] = D_MVHL_CALTRANS_STND;
		nCodeSeq[4] = D_MVHL_KOR_STND_LOAD;
		nCodeSeq[5] = D_MVHL_KOR_STND_2005;
		nCodeSeq[6] = D_MVHL_KOR_RAIL_LOAD;
		nCodeSeq[7] = D_MVHL_KOR_RAIL_2004;
		csDefaultType = _T("H15-44");
		csDefaultVehi = _T("");
		nNumIter = 8;
		nDefaultStndCode = D_MVHL_AASHTO_STAND;
		break;
	case D_MOVE_CODE_KOREA:
		nCodeSeq[0] = D_MVHL_KOR_STND_LOAD;
		nCodeSeq[1] = D_MVHL_KOR_STND_2005;
		nCodeSeq[2] = D_MVHL_KOR_RAIL_2012;
		nCodeSeq[3] = D_MVHL_KOR_RAIL_LOAD;
		nCodeSeq[4] = D_MVHL_KOR_RAIL_2004;
		nCodeSeq[5] = D_MVHL_AASHTO_STAND;
		nCodeSeq[6] = D_MVHL_AASHTO_LRFD;
		nCodeSeq[7] = D_MVHL_CALTRANS_STND_2017;
		nCodeSeq[8] = D_MVHL_CALTRANS_STND;
		nCodeSeq[9] = D_MVHL_OTHERS;
		csDefaultType = _T("DB-24");
		csDefaultVehi = _T("");
		nNumIter = 10;
		nDefaultStndCode = D_MVHL_KOR_STND_LOAD;
		break;
	case D_MOVE_CODE_CANADA:
		nCodeSeq[0] = D_MVHL_CANCSA_S600;
		csDefaultType = _T("CL-625 Truck"); 
		csDefaultVehi = _T("");
		nNumIter = 1;
		nDefaultStndCode = D_MVHL_CANCSA_S600;
		break;
	case D_MOVE_CODE_NEWZEALAND:
		nCodeSeq[0] = D_MVHL_TRAFFIC_LOAD_NZ;
		nCodeSeq[1] = D_MVHL_GENERAL_ACCESS_NZ;
		nCodeSeq[2] = D_MVHL_50MAX_NZ;
		nCodeSeq[3] = D_MVHL_HPMV_NZ;
		nCodeSeq[4] = D_MVHL_DECK_EVALUATION;
		nCodeSeq[5] = D_MVHL_FATIGUE_NZ;
		csDefaultType = _T("HN (normal) loading");
		csDefaultVehi = _T("");
		nNumIter = 6;
		nDefaultStndCode = D_MVHL_TRAFFIC_LOAD_NZ;
		break;
	case D_MOVE_CODE_PENDOT:
		nCodeSeq[0] = D_MVHL_AASHTO_PENDOT;
		nCodeSeq[1] = D_MVHL_OTHERS;
		csDefaultType = _T("PHL-93TRK"); 
		csDefaultVehi = _T("");
		nNumIter = 2;
		nDefaultStndCode = D_MVHL_AASHTO_PENDOT;
		break;
	case D_MOVE_CODE_BS:
		nCodeSeq[0] = D_MVHL_BS_5400;
		nCodeSeq[1] = D_MVHL_BS_BD3701;
		nCodeSeq[2] = D_MVHL_BS_CS454;
		nCodeSeq[3] = D_MVHL_BS_BD8611;
		csDefaultType = _T("HA");//_T("HA & HB (BS5400)");
		csDefaultVehi = _T("");
		nNumIter = 4;
		nDefaultStndCode = D_MVHL_BS_BD3701;
		break;
	default: ASSERT(0); break;
	}

	// TODO: Add extra initialization here  
	if(m_bModify)
	{
		m_nStndCode = m_Data.nStandardCode;
	}
	else
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;    
		m_Data.VehicleTypeName = csDefaultType;    
		m_nStndCode = m_Data.nStandardCode = nDefaultStndCode;
		m_Data.SelVehicle = csDefaultVehi; 
		m_Data.dAddDataaL = M_InitValueCurUnit(3.65, N, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dImpactCoef = 1.0;
	}

	for(int i = 0; i < nNumIter; i++) 
	{
		CDlgUtil::CobxAddItem(m_cbxCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq[i]], nCodeSeq[i]);
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_cbxCodeName);

	SetBS_BD3701AddData();
	//SetBS_CS454AddData();
}

void CCMMvhlItemStdDlg::SetBS_BD3701AddData()
{
	m_cbxCategory.ResetContent();
	m_cbxCategory.AddString(_T("Hp"));
	m_cbxCategory.AddString(_T("Mp"));
	m_cbxCategory.AddString(_T("Lp"));
	m_cbxCategory.AddString(_T("Hg"));
	m_cbxCategory.AddString(_T("Mg"));
	m_cbxCategory.AddString(_T("Lg"));
	m_cbxCategory.SetCurSel(0);

	m_cbxLoadLevel.ResetContent();
	m_cbxLoadLevel.AddString(_T("40t"));
	m_cbxLoadLevel.AddString(_T("26t"));
	m_cbxLoadLevel.AddString(_T("18t"));
	m_cbxLoadLevel.AddString(_T("7.5t"));
	m_cbxLoadLevel.AddString(_T("G1 FE"));
	m_cbxLoadLevel.AddString(_T("G2 FE"));
	m_cbxLoadLevel.AddString(_T("3t"));
	m_cbxLoadLevel.SetCurSel(0);
}

void CCMMvhlItemStdDlg::SetBS_CS454AddData()
{
	m_cbxCategoryCS.ResetContent();
	m_cbxCategoryCS.AddString(_T("Hp"));
	m_cbxCategoryCS.AddString(_T("Mp"));
	m_cbxCategoryCS.AddString(_T("Lp"));
	m_cbxCategoryCS.AddString(_T("Hg"));
	m_cbxCategoryCS.AddString(_T("Mg"));
	m_cbxCategoryCS.AddString(_T("Lg"));
	m_cbxCategoryCS.SetCurSel(0);

	m_cbxLoadLevelCS.ResetContent();
	m_cbxLoadLevelCS.AddString(_T("40t"));
	m_cbxLoadLevelCS.AddString(_T("26t"));
	m_cbxLoadLevelCS.AddString(_T("18t"));
	m_cbxLoadLevelCS.AddString(_T("7.5t"));
	m_cbxLoadLevelCS.AddString(_T("G1 FE"));
	m_cbxLoadLevelCS.AddString(_T("G2 FE"));
	m_cbxLoadLevelCS.AddString(_T("3t"));
	m_cbxLoadLevelCS.SetCurSel(0);
}


void CCMMvhlItemStdDlg::InitThreeGrid(int nLoadType)
{
	m_wndThreeGrid[0].SetDataSource(&m_Data,this,0,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(m_Data,0),nLoadType);
	m_wndThreeGrid[1].SetDataSource(&m_Data,this,1,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(m_Data,1),nLoadType);
	GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt(m_Data,0));
	GetDlgItem(IDC_CMD_MVHL_SECOND_TXT)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt(m_Data,1));  
} 

void CCMMvhlItemStdDlg::InitGrid()
{
	m_wndThreeGrid[0].InitGrid(153);
	m_wndThreeGrid[1].InitGrid(153);
}

void CCMMvhlItemStdDlg::OnHALaneFactor()
{
	CCMMvhlSItemUsrLaneFactor dlg;
	dlg.SetData(m_Data);
	if(dlg.DoModal() == IDOK)
	{
		m_Data.dLaneFactor[0] = dlg.GetData().dLaneFactor[0];
		m_Data.dLaneFactor[1] = dlg.GetData().dLaneFactor[1];
		m_Data.dLaneFactor[2] = dlg.GetData().dLaneFactor[2];
		m_Data.dLaneFactor[3] = dlg.GetData().dLaneFactor[3];
	}
}

void CCMMvhlItemStdDlg::OnChkAddData()
{
	SetEnableBS();
}

void CCMMvhlItemStdDlg::OnChkPatch()
{
	BOOL bPatchLoad = m_chkPatch.GetCheck();

	GetDlgItem(IDC_CMD_MVHL_PATCH_W_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_UNT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_UNT)->EnableWindow(bPatchLoad);		
}