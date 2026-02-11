// CMMvhlItemUsrSouthAfricaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrSouthAfricaDlg.h"
#include "CMMvhlItemUsrPermitDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrSouthAfricaDlg dialog

CCMMvhlItemUsrSouthAfricaDlg::CCMMvhlItemUsrSouthAfricaDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemUsrSouthAfricaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrSouthAfricaDlg)	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
	m_bModify = FALSE;

	m_nSubType = 0;
	m_bNAIncreLength = FALSE;
	m_bNCOpposite = FALSE;

	m_bPermitLoadCall = FALSE;

	m_aCtrl_NA.RemoveAll();
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_W1_STC);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_W1_EDT);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_W1_UNT);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_L1_STC);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_L1_EDT);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_L1_UNT);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_W2_STC1);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_W2_EDT1);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_W2_STC2);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_W2_EDT2);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_W2_STC3);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_W2_UNT);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_L2_STC);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_L2_EDT);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_L2_UNT);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_PA_STC1);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_PA_EDT);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_PA_STC2);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_PA_UNT);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_PA_STC3);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_INCRE_LENGTH_CHK);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_INCRE_LENGTH_EDT);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_INCRE_LENGTH_UNT);

	m_aCtrl_NB.RemoveAll();
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_PB_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_PB_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_PB_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_NUM_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_NUM_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D1_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D1_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D1_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D2_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D2_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D2_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D3_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D3_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D3_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D4_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D4_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D4_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D5_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D5_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D5_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D6_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D6_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D6_UNT);

	m_aCtrl_NC.RemoveAll();
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_W_STC);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_W_EDT);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_W_UNT);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_GRP);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_A_STC);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_A_EDT);
	//m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_A_SPN);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_A_GRID);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_B_STC);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_B_EDT);
	//m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_B_SPN);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_B_GRID);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_C_STC);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_C_EDT);
	//m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_C_SPN);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_C_GRID);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_OPPOSITE_CHK);		

	m_aCtrl_NBNC.RemoveAll();
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_P_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_P_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_P_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D1_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D1_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D1_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D2_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D2_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D2_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D3_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D3_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D3_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D4_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D4_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D4_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D5_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D5_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D5_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D6_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D6_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_D6_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_W1_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_W1_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_W1_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_W2_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_W2_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_W2_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD1_W_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD1_W_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD1_W_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD2_W_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD2_W_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD2_W_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD1_L_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD1_L_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD1_L_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD2_L_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD2_L_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD2_L_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD3_L_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD3_L_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_DD3_L_UNT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_WIDTH_STC);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_WIDTH_EDT);
	m_aCtrl_NBNC.Add(IDC_CMD_MVHL_NBNC_WIDTH_UNT);

	m_aCtrl_OKCancel.Add(IDOK);
	m_aCtrl_OKCancel.Add(IDCANCEL);
	m_aCtrl_OKCancel.Add(IDC_CMD_APPLY);
}

CCMMvhlItemUsrSouthAfricaDlg::~CCMMvhlItemUsrSouthAfricaDlg()
{
}

void CCMMvhlItemUsrSouthAfricaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrSouthAfricaDlg)  
	DDX_Radio  (pDX, IDC_CMD_MVHL_NA_RDO,               m_nSubType);
	DDX_Control(pDX, IDC_CMD_MVHL_NAME_EDT,             m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_W1_EDT,            m_edtNAW1);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_W1_UNT,            m_untNAW1);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_L1_EDT,            m_edtNAL1);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_L1_UNT,            m_untNAL1);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_W2_EDT1,           m_edtNAW21);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_W2_EDT2,           m_edtNAW22);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_W2_UNT,            m_untNAW2);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_L2_EDT,            m_edtNAL2);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_L2_UNT,            m_untNAL2);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_PA_EDT,            m_edtNAPa);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_PA_UNT,            m_untNAPa);
	DDX_Check  (pDX, IDC_CMD_MVHL_NA_INCRE_LENGTH_CHK,  m_bNAIncreLength);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_INCRE_LENGTH_EDT,  m_edtNAIncreLength);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_INCRE_LENGTH_UNT,  m_untNAIncreLength);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_PB_EDT,            m_edtNBPb);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_PB_UNT,            m_untNBPb);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_NUM_EDT,           m_edtNBNumUnit);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D_EDT,             m_edtNBd);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D_UNT,             m_untNBd);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D1_EDT,            m_edtNBD1);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D1_UNT,            m_untNBD1);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D2_EDT,            m_edtNBD2);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D2_UNT,            m_untNBD2);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D3_EDT,            m_edtNBD3);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D3_UNT,            m_untNBD3);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D4_EDT,            m_edtNBD4);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D4_UNT,            m_untNBD4);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D5_EDT,            m_edtNBD5);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D5_UNT,            m_untNBD5);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D6_EDT,            m_edtNBD6);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D6_UNT,            m_untNBD6);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_W_EDT,             m_edtNCW);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_W_UNT,             m_untNCW);
// 	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_A_SPN,           m_spnNCNum[0]);
// 	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_B_SPN,           m_spnNCNum[1]);
// 	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_C_SPN,           m_spnNCNum[2]);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_A_EDT,           m_edtNCNum[0]);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_B_EDT,           m_edtNCNum[1]);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_C_EDT,           m_edtNCNum[2]);
	DDX_Check  (pDX, IDC_CMD_MVHL_NC_OPPOSITE_CHK,      m_bNCOpposite);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_P_EDT,           m_edtNBNCP);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_P_UNT,           m_untNBNCP);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D1_EDT,          m_edtNBNCD1);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D1_UNT,          m_untNBNCD1);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D2_EDT,          m_edtNBNCD2);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D2_UNT,          m_untNBNCD2);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D3_EDT,          m_edtNBNCD3);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D3_UNT,          m_untNBNCD3);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D4_EDT,          m_edtNBNCD4);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D4_UNT,          m_untNBNCD4);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D5_EDT,          m_edtNBNCD5);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D5_UNT,          m_untNBNCD5);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D6_EDT,          m_edtNBNCD6);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_D6_UNT,          m_untNBNCD6);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_W1_EDT,          m_edtNBNCW1);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_W1_UNT,          m_untNBNCW1);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_W2_EDT,          m_edtNBNCW2);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_W2_UNT,          m_untNBNCW2);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_DD1_W_EDT,       m_edtNBNCDD1W);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_DD1_W_UNT,       m_untNBNCDD1W);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_DD2_W_EDT,       m_edtNBNCDD2W);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_DD2_W_UNT,       m_untNBNCDD2W);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_DD1_L_EDT,       m_edtNBNCDD1L);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_DD1_L_UNT,       m_untNBNCDD1L);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_DD2_L_EDT,       m_edtNBNCDD2L);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_DD2_L_UNT,       m_untNBNCDD2L);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_DD3_L_EDT,       m_edtNBNCDD3L);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_DD3_L_UNT,       m_untNBNCDD3L);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_WIDTH_EDT,       m_edtNBNCWidth);
	DDX_Control(pDX, IDC_CMD_MVHL_NBNC_WIDTH_UNT,       m_untNBNCWidth);

	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_A_GRID, m_NCGrid[0]);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_B_GRID, m_NCGrid[1]);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_C_GRID, m_NCGrid[2]);

	DDX_Control(pDX, IDC_CMD_MVHL_PIC_WND, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemUsrSouthAfricaDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrSouthAfricaDlg)
	ON_BN_CLICKED(IDC_CMD_MVHL_NA_RDO,				       OnCmdMvhlSubTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_NB_RDO,				       OnCmdMvhlSubTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_NC_RDO,				       OnCmdMvhlSubTypeRdo)
	//ON_BN_CLICKED(IDC_CMD_MVHL_NBNC_RDO,	  	       OnCmdMvhlSubTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_PERMIT_RDO,		       OnCmdMvhlSubTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_NA_INCRE_LENGTH_CHK,	 OnCmdMvhlIncreLengthChk)
	ON_EN_CHANGE (IDC_CMD_MVHL_NA_L1_EDT,            OnCmdMvhlChangeL1Edt)
	ON_EN_CHANGE (IDC_CMD_MVHL_NB_D2_EDT,            OnCmdMvhlChangeDistEdt)
	ON_EN_CHANGE (IDC_CMD_MVHL_NB_D_EDT,             OnCmdMvhlChangeDistEdt)
	ON_EN_CHANGE (IDC_CMD_MVHL_NC_L_A_EDT,           OnCmdMvhlChangeNCNumaEdt)
	ON_EN_CHANGE (IDC_CMD_MVHL_NC_L_B_EDT,           OnCmdMvhlChangeNCNumbEdt)
	ON_EN_CHANGE (IDC_CMD_MVHL_NC_L_C_EDT,           OnCmdMvhlChangeNCNumcEdt)
	ON_BN_CLICKED(IDC_CMD_APPLY,				             OnCmdMvhlApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemUsrSouthAfricaDlg::DefaultSetting()
{
	BOOL bDefalutNA = FALSE;
	BOOL bDefalutNB = FALSE;
	BOOL bDefalutNC = FALSE;
	BOOL bDefaultNBNC = FALSE;

	if(m_bModify)
	{
		if     (m_Data.nSubType == 0) { bDefalutNA = FALSE; bDefalutNB = TRUE;  bDefalutNC = TRUE;  bDefaultNBNC = TRUE;  }
		else if(m_Data.nSubType == 1) { bDefalutNA = TRUE;  bDefalutNB = FALSE; bDefalutNC = TRUE;  bDefaultNBNC = TRUE;  }
		else if(m_Data.nSubType == 2) { bDefalutNA = TRUE;  bDefalutNB = TRUE;  bDefalutNC = FALSE; bDefaultNBNC = TRUE;  }
		//else if(m_Data.nSubType == 3) { bDefalutNA = TRUE;  bDefalutNB = TRUE;  bDefalutNC = TRUE;  bDefaultNBNC = FALSE; }
		else if(m_Data.nSubType == 3) { bDefalutNA = TRUE;  bDefalutNB = TRUE;  bDefalutNC = TRUE;  bDefaultNBNC = TRUE; }
		else                           { ASSERT(0); }
	}
	else
	{
		m_Data.Initialize();
		m_Data.bStandard = FALSE;    

		bDefalutNA = TRUE; 
		bDefalutNB = TRUE;
		bDefalutNC = TRUE;
		bDefaultNBNC = TRUE;
	}

	if(bDefalutNA)
	{
		m_Data.dW1 = 36.0; 
		m_Data.dLoadedLength = 36.0;
		m_Data.dW2 = 180.0;
		m_Data.dW3 = 6.0;
		m_Data.dPa = 144.0;
		m_Data.dIncreLength  = M_InitValueCurUnit( 1.0, KN, M, D_UNITSYS_BASE_LENGTH);
	}

	if(bDefalutNB)
	{
		m_Data.dPb           = M_InitValueCurUnit(10.0, KN, M, D_UNITSYS_BASE_FORCE);
		m_Data.dUnitNum      = 36.0;
		m_Data.dDelta        = M_InitValueCurUnit( 5.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dD1           = M_InitValueCurUnit( 2.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dD2           = M_InitValueCurUnit( 6.0, KN, M, D_UNITSYS_BASE_LENGTH);
	}

	if(bDefalutNC)
	{
		m_Data.dPressureLoad = M_InitValueCurUnit(30.0, KN, M, D_UNITSYS_BASE_PRESSURE);
		m_Data.nNumLoad3[0] = 4;
		m_Data.nNumLoad3[1] = 3;
		m_Data.nNumLoad3[2] = 6;

		m_Data.dPointDistance3[0][0] = M_InitValueCurUnit( 5.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dPointDistance3[0][1] = M_InitValueCurUnit(10.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dPointDistance3[0][2] = M_InitValueCurUnit(15.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dPointDistance3[0][3] = M_InitValueCurUnit(20.0, KN, M, D_UNITSYS_BASE_LENGTH);

		m_Data.dPointDistance3[1][0] = M_InitValueCurUnit( 3.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dPointDistance3[1][1] = M_InitValueCurUnit( 4.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dPointDistance3[1][2] = M_InitValueCurUnit( 5.0, KN, M, D_UNITSYS_BASE_LENGTH);

		m_Data.dPointDistance3[2][0] = M_InitValueCurUnit( 0.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dPointDistance3[2][1] = M_InitValueCurUnit( 5.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dPointDistance3[2][2] = M_InitValueCurUnit(10.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dPointDistance3[2][3] = M_InitValueCurUnit(15.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dPointDistance3[2][4] = M_InitValueCurUnit(20.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dPointDistance3[2][5] = M_InitValueCurUnit(25.0, KN, M, D_UNITSYS_BASE_LENGTH);
	}

	if(bDefaultNBNC)
	{
		// 필요시 추가
	}
}

void CCMMvhlItemUsrSouthAfricaDlg::InitUnit()
{
 	m_edtVehicleName.SetUnitType(D_UNITSYS_NONE);

	// NA 차량은 고정 단위계
	m_untNAW1.SetTextFmt(_LSX(kN/m));
	m_untNAL1.SetTextFmt(_LSX(m));
	m_untNAW2.SetTextFmt(_LSX(kN/m));
	m_untNAL2.SetTextFmt(_LSX(m));
	m_untNAPa.SetTextFmt(_LSX(kN));
	
	m_edtNAW1.SetUnitType(D_UNITSYS_NONE);
	m_untNAW1.SetUnitType(D_UNITSYS_NONE);
	m_edtNAL1.SetUnitType(D_UNITSYS_NONE);
	m_untNAL1.SetUnitType(D_UNITSYS_NONE);
	m_edtNAW21.SetUnitType(D_UNITSYS_NONE);
	m_edtNAW22.SetUnitType(D_UNITSYS_NONE);
	m_untNAW2.SetUnitType(D_UNITSYS_NONE);
	m_edtNAL2.SetUnitType(D_UNITSYS_NONE);
	m_untNAL2.SetUnitType(D_UNITSYS_NONE);
	m_edtNAPa.SetUnitType(D_UNITSYS_NONE);
	m_untNAPa.SetUnitType(D_UNITSYS_NONE);
	m_edtNAIncreLength.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNAIncreLength.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtNBPb.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untNBPb.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtNBd .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBd .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBD3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBD3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBD4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBD4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBD5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBD5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBD6.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBD6.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtNCW.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_untNCW.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	
	m_edtNBNCP.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untNBNCP.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtNBNCD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBNCD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBNCD3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCD3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBNCD4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCD4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBNCD5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCD5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBNCD6.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCD6.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBNCW1.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_untNBNCW1.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_edtNBNCW2.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_untNBNCW2.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_edtNBNCDD1W.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCDD1W.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBNCDD2W.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCDD2W.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBNCDD1L.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCDD1L.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBNCDD2L.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCDD2L.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBNCDD3L.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCDD3L.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNBNCWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNBNCWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemUsrSouthAfricaDlg::InitControls()
{
	m_NCGrid[0].InitGrid();
	m_NCGrid[1].InitGrid();
	m_NCGrid[2].InitGrid();

	m_edtNCNum[0].SetRange(0,100);
	m_edtNCNum[1].SetRange(0,100);
	m_edtNCNum[2].SetRange(0,100);

	m_edtNCNum[0].SetInteger(TRUE);
	m_edtNCNum[1].SetInteger(TRUE);
	m_edtNCNum[2].SetInteger(TRUE);
}

void CCMMvhlItemUsrSouthAfricaDlg::AlignControl()
{
	UpdateData(TRUE);

	CRect rRef;
	CRect rToMove;
	CRect rectGrp;
	int nDistY = 0;

	GetDlgItem(IDC_CMD_MVHL_NA_W1_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_NB_PB_STC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_NB, nDistY);

	GetDlgItem(IDC_CMD_MVHL_NA_W1_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_NC_W_STC )->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_NC, nDistY);

	GetDlgItem(IDC_CMD_MVHL_NA_W1_STC )->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_NBNC_P_STC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_NBNC, nDistY);
		
	// Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rectGrp);

	if(m_nSubType == 0 || m_nSubType == 1)
	{
		GetDlgItem(IDC_CMD_MVHL_NB_D4_EDT)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_NC_OPPOSITE_CHK)->GetWindowRect(rRef);
	}

	rectGrp.bottom = rRef.bottom + globalUtils.ScaleByDPI(16);
	ScreenToClient(rectGrp);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->MoveWindow(rectGrp);

	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_OKCancel, nDistY);

	CRect r;
	GetWindowRect(r);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	r.bottom = rToMove.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	RedrawWindow();
}

void CCMMvhlItemUsrSouthAfricaDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlShowHide(this, m_aCtrl_NA,   m_nSubType==0);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_NB,   m_nSubType==1);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_NC,   m_nSubType==2);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_NBNC, SW_HIDE);
}

void CCMMvhlItemUsrSouthAfricaDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_NA_INCRE_LENGTH_EDT)->EnableWindow(m_bNAIncreLength);
	GetDlgItem(IDC_CMD_MVHL_NA_INCRE_LENGTH_UNT)->EnableWindow(m_bNAIncreLength);
}

void CCMMvhlItemUsrSouthAfricaDlg::ChangeBitmap(int nBitmap)
{
	CString aBitmapID[] = { _T("cmd_mvhl_199.svg"), _T("cmd_mvhl_200.svg"), _T("cmd_mvhl_201.svg"), _T("cmd_mvhl_207.svg") };

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + aBitmapID[nBitmap]);
}

void CCMMvhlItemUsrSouthAfricaDlg::Data2Dlg()
{
	m_nSubType = m_Data.nSubType;
	m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);
	ChangeBitmap(m_nSubType);

	m_edtNAW1 .SetEditUnit(m_Data.dW1);
	m_edtNAL1 .SetEditUnit(m_Data.dLoadedLength);
	m_edtNAW21.SetEditUnit(m_Data.dW2);
	m_edtNAW22.SetEditUnit(m_Data.dW3);
	m_edtNAL2 .SetEditUnit(m_Data.dLoadedLength);
	m_edtNAPa .SetEditUnit(m_Data.dPa);
	m_bNAIncreLength = m_Data.bIncreLength;
	m_edtNAIncreLength.SetEditUnit(m_Data.dIncreLength);
	
	m_edtNBPb.SetEditUnit(m_Data.dPb);
	m_edtNBNumUnit.SetEditUnit(m_Data.dUnitNum);
	m_edtNBd .SetEditUnit(m_Data.dDelta);
	m_edtNBD1.SetEditUnit(m_Data.dD1);
	m_edtNBD2.SetEditUnit(m_Data.dD2);	
	m_edtNBD3.SetEditUnit(m_Data.dD2 + m_Data.dDelta);
	m_edtNBD4.SetEditUnit(m_Data.dD2 + m_Data.dDelta * 2.);
	m_edtNBD5.SetEditUnit(m_Data.dD2 + m_Data.dDelta * 3.);
	m_edtNBD6.SetEditUnit(m_Data.dD2 + m_Data.dDelta * 4.);

	m_edtNCW.SetEditUnit(m_Data.dPressureLoad);	

	m_edtNCNum[0].SetValue(m_Data.nNumLoad3[0]);
	m_edtNCNum[1].SetValue(m_Data.nNumLoad3[1]);
	m_edtNCNum[2].SetValue(m_Data.nNumLoad3[2]);
	
	m_NCGrid[0].SetDataSource(&m_Data, 0, 1);
	m_NCGrid[1].SetDataSource(&m_Data, 1, 1);
	m_NCGrid[2].SetDataSource(&m_Data, 2, 1);

	m_bNCOpposite = m_Data.bOpposite;

	m_edtNBNCP.SetEditUnit(m_Data.dP);
	m_edtNBNCD1.SetEditUnit(m_Data.dPointDistance[0]);
	m_edtNBNCD2.SetEditUnit(m_Data.dPointDistance[1]);
	m_edtNBNCD3.SetEditUnit(m_Data.dPointDistance[2]);
	m_edtNBNCD4.SetEditUnit(m_Data.dPointDistance2[0]);
	m_edtNBNCD5.SetEditUnit(m_Data.dPointDistance2[1]);
	m_edtNBNCD6.SetEditUnit(m_Data.dPointDistance2[2]);
	m_edtNBNCW1.SetEditUnit(m_Data.dWPres1);
	m_edtNBNCW2.SetEditUnit(m_Data.dWPres2);
	m_edtNBNCDD1W.SetEditUnit(m_Data.dTrainDD1);
	m_edtNBNCDD2W.SetEditUnit(m_Data.dTrainDD2);
	m_edtNBNCDD1L.SetEditUnit(m_Data.dTrainD1);
	m_edtNBNCDD2L.SetEditUnit(m_Data.dTrainD2);
	m_edtNBNCDD3L.SetEditUnit(m_Data.dTrainD3);
	m_edtNBNCDD2W.SetEditUnit(m_Data.dWheelSpacing);
	
	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrSouthAfricaDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.Initialize();  
	m_Data.nSubType = m_nSubType;
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);   
	m_Data.bStandard = FALSE;
	m_Data.nStandardCode = D_MVHL_SOUTH_AFRICA_TMH7_1981;

	m_Data.dW1 = m_edtNAW1.GetEditValue();
	m_Data.dLoadedLength = m_edtNAL1.GetEditValue();
	m_Data.dW2 = m_edtNAW21.GetEditValue();
	m_Data.dW3 = m_edtNAW22.GetEditValue();
	m_Data.dPa = m_edtNAPa .GetEditValue();
	m_Data.bIncreLength = m_bNAIncreLength;
	m_Data.dIncreLength = m_edtNAIncreLength.GetEditValue();

	m_Data.dPb = m_edtNBPb.GetEditValue();
	m_Data.dUnitNum = m_edtNBNumUnit.GetEditValue();
	m_Data.dDelta = m_edtNBd .GetEditValue();
	m_Data.dD1    = m_edtNBD1.GetEditValue();
	m_Data.dD2    = m_edtNBD2.GetEditValue();

	m_Data.dPressureLoad = m_edtNCW.GetEditValue();

	m_Data.nNumLoad3[0] = m_edtNCNum[0].GetEditValueInt();
	m_Data.nNumLoad3[1] = m_edtNCNum[1].GetEditValueInt();
	m_Data.nNumLoad3[2] = m_edtNCNum[2].GetEditValueInt();

	m_NCGrid[0].SaveData();
	m_NCGrid[1].SaveData();
	m_NCGrid[2].SaveData();

	m_Data.bOpposite = m_bNCOpposite;

	m_Data.dP = m_edtNBNCP.GetEditValue();
	m_Data.dPointDistance[0] = m_edtNBNCD1.GetEditValue();
	m_Data.dPointDistance[1] = m_edtNBNCD2.GetEditValue();
	m_Data.dPointDistance[2] = m_edtNBNCD3.GetEditValue();
	m_Data.dPointDistance2[0] = m_edtNBNCD4.GetEditValue();
	m_Data.dPointDistance2[1] = m_edtNBNCD5.GetEditValue();
	m_Data.dPointDistance2[2] = m_edtNBNCD6.GetEditValue();
	m_Data.dWPres1 = m_edtNBNCW1.GetEditValue();
	m_Data.dWPres2 = m_edtNBNCW2.GetEditValue();
	m_Data.dTrainDD1 = m_edtNBNCDD1W.GetEditValue();
	m_Data.dTrainDD2 = m_edtNBNCDD2W.GetEditValue();
	m_Data.dTrainD1 = m_edtNBNCDD1L.GetEditValue();
	m_Data.dTrainD2 = m_edtNBNCDD2L.GetEditValue();
	m_Data.dTrainD3 = m_edtNBNCDD3L.GetEditValue();
	m_Data.dWheelSpacing = m_edtNBNCDD2W.GetEditValue();
	
	return TRUE;
}

BOOL CCMMvhlItemUsrSouthAfricaDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrSouthAfricaDlg message handlers

BOOL CCMMvhlItemUsrSouthAfricaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
		
	DefaultSetting();
	InitUnit();
	InitControls();
		
	Data2Dlg();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();

	OnCmdMvhlSubTypeRdo();
	
	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	GetDlgItem(IDC_CMD_MVHL_NBNC_RDO)->ShowWindow(SW_HIDE); // 지원하지 않음

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrSouthAfricaDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemUsrSouthAfricaDlg::OnCmdMvhlSubTypeRdo()
{
	UpdateData(TRUE);

	if(m_nSubType == 3)
	{
		if (m_bPermitLoadCall)
		{
			m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);   
			m_bPermitLoadCall = FALSE;
			CCMMvhlItemUsrPermitDlg dlg;
			dlg.SetInitPos(D_INIT_POS_LT);
			CString csRadioText[3];

			T_MVCD_D MvcdD; MvcdD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return;
			dlg.SetModifyData(m_bModify, m_csOldName, &m_Data, m_pDoc, csRadioText, MvcdD.nCodeType);
			dlg.DoModal();
			//Type Change이면
			int nLastEvent = dlg.GetNextAction();
			if (nLastEvent == CCMMvhlItemUsrPermitDlg::CHANGE_TYPE)
			{
				CRect r = dlg.GetLastWindowPos();
				CRect rOrg;
				GetWindowRect(rOrg);
				r.right = r.left + rOrg.Width();
				r.bottom = r.top + rOrg.Height();
				MoveWindow(r);
				ShowWindow(SW_SHOW);
				m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);

				m_nSubType = dlg.GetSouthAfricaType();

				UpdateData(FALSE);

				AlignControl();
				ChangeBitmap(m_nSubType);
				ControlsShowHide();
				EnableDisableControls();
			}
			else
			{
				CDialogMove::OnOK();
			}
		}
		else
		{
			m_bPermitLoadCall = TRUE;
			PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_MVHL_PERMIT_RDO, BN_CLICKED), 0);
		}
	}
	else
	{
		AlignControl();
		ChangeBitmap(m_nSubType);
		ControlsShowHide();
		EnableDisableControls();
	}
}

void CCMMvhlItemUsrSouthAfricaDlg::OnCmdMvhlChangeL1Edt()
{
	UpdateData(TRUE);

	double dL1 = m_edtNAL1.GetEditValue();
	m_edtNAL2.SetEditUnit(dL1);

	UpdateData(FALSE);
}

void CCMMvhlItemUsrSouthAfricaDlg::OnCmdMvhlIncreLengthChk() 
{
	EnableDisableControls();
}

void CCMMvhlItemUsrSouthAfricaDlg::OnCmdMvhlChangeDistEdt()
{
	UpdateData(TRUE);

	double dD2 = m_edtNBD2.GetEditValue();
	double dd  = m_edtNBd .GetEditValue();

	m_edtNBD3.SetEditUnit(dD2 + dd);
	m_edtNBD4.SetEditUnit(dD2 + dd * 2.);
	m_edtNBD5.SetEditUnit(dD2 + dd * 3.);
	m_edtNBD6.SetEditUnit(dD2 + dd * 4.);

	UpdateData(FALSE);
}

void CCMMvhlItemUsrSouthAfricaDlg::OnCmdMvhlChangeNCNumaEdt()
{
	if(!IsWindow(m_edtNCNum[0]))return;

	m_Data.nNumLoad3[0] = m_edtNCNum[0].GetEditValueInt();
	m_NCGrid[0].SetDataSource(&m_Data, 0, 1);
}

void CCMMvhlItemUsrSouthAfricaDlg::OnCmdMvhlChangeNCNumbEdt()
{
	if(!IsWindow(m_edtNCNum[1]))return;

	m_Data.nNumLoad3[1] = m_edtNCNum[1].GetEditValueInt();
	m_NCGrid[1].SetDataSource(&m_Data, 1, 1);
}

void CCMMvhlItemUsrSouthAfricaDlg::OnCmdMvhlChangeNCNumcEdt()
{
	if(!IsWindow(m_edtNCNum[2]))return;

	m_Data.nNumLoad3[2] = m_edtNCNum[2].GetEditValueInt();
	m_NCGrid[2].SetDataSource(&m_Data, 2, 1);
}

void CCMMvhlItemUsrSouthAfricaDlg::OnCmdMvhlApply() 
{	
	ApplyOrOK();
}