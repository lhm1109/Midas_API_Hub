#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_dbLock\LockMgr.h"   // for CLockMgr

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "CmdAutoLoadCombSelCvlDlg.h"
#include "CmdAutoLoadCombCvlHelpDlg.h"
#include "CmdAutoLoadCombCvlKOREALRFD11Dlg.h"
#include "CmdAutoLoadCombCvlAASHTOLRFD02Dlg.h"
#include "CmdAutoLoadCombCvlAASHTOLRFD08Dlg.h"
#include "CmdAutoLoadCombCvlAASHTOLRFD12Dlg.h"
#include "CmdAutoLoadCombCvlCSAPage.h"
#include "CmdAutoLoadCombCvlSP35133302011Page.h"
#include "CmdAutoLoadCombCvlEuroRoadDlg.h"
#include "CmdAutoLoadCombCvlAS51002017Page.h"
#include "CmdAutoLoadCombCvlKSCEUSD10Dlg.h"
#include "CmdAutoLoadCombCvlBD21Page.h"
#include "CmdAutoLoadCombCvlPNS10030Page.h"
#include "CmdAutoLoadCombCvlEURO0Page.h"
#include "CmdAutoLoadCombCvlIRC6LSDPage.h"
#include "CmdAutoLoadCombCvlIRSPage.h"
#include "CmdAutoLoadCombCvlIRC_SPVHLDlg.h"
#include "CmdAutoLoadCombCvlTMH7Page.h"


#include "..\wg_main\wg_mainres2.h"

#include "..\wg_dbLock\LockMgr.h"	
#include "..\wg_dbLock\LockOption.h"	
#include "..\wg_dbLock\WebLockUtil.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CDialogMove

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlDlg dialog

namespace AutoLoadComb
{
	constexpr const wchar_t* KSCE_LSD15 = L"KSCE-LSD15";
	constexpr const wchar_t* AASHTO_LRFD12 = L"AASHTO-LRFD12";
	constexpr const wchar_t* AASHTO_LRFD16 = L"AASHTO-LRFD16";
	constexpr const wchar_t* AASHTO_LRFD17 = L"AASHTO-LRFD17";
	constexpr const wchar_t* AASHTO_LRFD20 = L"AASHTO-LRFD20";
	constexpr const wchar_t* AASHTO_LRFD24 = L"AASHTO-LRFD24";
	constexpr const wchar_t* CSA_S6S1_10 = L"CSA-S6S1-10";
	constexpr const wchar_t* SNIP   = STLCODE_SNIP_2_05_03_84;  //L"SNiP 2.05.03-84*";
	constexpr const wchar_t* SP3511 = STLCODE_SP_35_13330_2011; //L"SP 35.13330.2011";
	constexpr const wchar_t* CSA_S6_19 = L"CSA-S6-19";
	constexpr const wchar_t* CSA_S6_14 = L"CSA-S6-14";
	constexpr const wchar_t* AS5100 = L"AS 5100.2:17";
	constexpr const wchar_t* KSCEUSD10 = L"KSCE-USD10";
	constexpr const wchar_t* BD21 = L"BS 5400";
	constexpr const wchar_t* PNS10030 = L"PN-85/S-10030";
	constexpr const wchar_t* EURO0 = L"Eurocode 0";
	constexpr const wchar_t* IRC6LSD = L"IRC:6 LSD";
	constexpr const wchar_t* AASHTOLRFD02 = L"AASHTO-LRFD02";
	constexpr const wchar_t* AASHTOLRFD08 = L"AASHTO-LRFD07";
	constexpr const wchar_t* IRS = L"IRS";
	constexpr const wchar_t* TMH7 = L"TMH07-1981";
}

CCmdAutoLoadCombCvlDlg::CCmdAutoLoadCombCvlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCmdAutoLoadCombCvlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombCvlDlg)
	m_nOption = 0;	
	m_nDesignType = 0;
	m_bIrcServCond = TRUE;
	m_bIrcConsCond = FALSE;
	m_bJSCEChk1 = TRUE;
	m_bJSCEChk2 = TRUE;
	m_bJSCEChk3 = TRUE;
	m_bJSCEChk4 = FALSE;
	m_bJSCEChk5 = TRUE;
	m_bJSCEChk6 = TRUE;
	m_bJSCEChk7 = TRUE;
	m_bJTJD60Capc = TRUE;
	m_bJTJD60Gen  = TRUE;
	m_bJTJD60Acdn = TRUE;
	m_bJTJD60Serv = TRUE;
	m_bJTJD60Elst = TRUE;
	m_bTB05Main = TRUE;//add by maxiao 2007-04-24
	m_bTB05MainAddition = TRUE;
	m_bTB05MainSpecial = TRUE;
	m_bTB10002_2017Main = TRUE;
	m_bTB10002_2017MainAddition = TRUE;
	m_bTB10002_2017MainSpecial = TRUE;
	m_bCJJ166_2011Capc = TRUE;
	m_bCJJ166_2011Gen  = TRUE;
	m_bCJJ166_2011Acdn = TRUE;
	m_bCJJ166_2011Serv = TRUE;
	m_bCJJ166_2011Elst = TRUE;

	m_bJTG15Capc = TRUE;//add by dongqiufeng 2015-10-16
	m_bJTG15Gen  = TRUE;
	m_bJTG15Acdn = TRUE;
	m_bJTG15Serv = TRUE;
	m_bJTG15Elst = TRUE;
	m_bJTG15_Freq = TRUE;
	m_bJTG15_QuasiPerm = FALSE;
	m_bJTG15_FreqComb = TRUE;  
	m_bJTG15_QuasiPermComb = TRUE;
	m_bJTG15_StandComb = TRUE;
	m_bJTG15_Factor = FALSE;
	m_bJTG15FoundationChk = FALSE;
	m_bJTG15FdnCommonChk = FALSE;
	m_bJTG15FdnCommonBasicChk = FALSE;
	m_bJTG15FdnCommonAccChk = FALSE;
	m_bJTG15FdnCommonFreqChk = FALSE;
	m_bJTG15FdnCommonPermChk = FALSE;

	m_tLcomESCGB19.Init();

    m_nConstLoad = 0;

    m_bIS456_Cvl = TRUE; 

	m_bInit = TRUE;

	m_bUlti1 = TRUE;
	m_bUlti2 = TRUE;
	m_bUlti3 = TRUE;
	m_bUlti4 = TRUE;
	m_bUlti5 = TRUE;
	m_bExtr1 = FALSE;
	m_bExtr2 = FALSE;
	m_bServ1 = FALSE;
	m_bServ2 = TRUE;
	m_bServ3 = FALSE;
	m_bServ4 = FALSE;
	m_bServ5 = FALSE;
	m_bFati1 = TRUE;
	//}}AFX_DATA_INIT

}


void CCmdAutoLoadCombCvlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadCombCvlDlg)  
	DDX_Check(pDX, IDC_CMD_JSCE02_CHK1, m_bJSCEChk1);
	DDX_Check(pDX, IDC_CMD_JSCE02_CHK2, m_bJSCEChk2);
	DDX_Check(pDX, IDC_CMD_JSCE02_CHK3, m_bJSCEChk3);
	DDX_Check(pDX, IDC_CMD_JSCE02_CHK4, m_bJSCEChk4);
	DDX_Check(pDX, IDC_CMD_JSCE02_CHK5, m_bJSCEChk5);
	DDX_Check(pDX, IDC_CMD_JSCE02_CHK6, m_bJSCEChk6);
	DDX_Check(pDX, IDC_CMD_JSCE02_CHK7, m_bJSCEChk7);
	DDX_Control(pDX, IDC_CMD_LOSS_FACTOR_TRANSFER, m_wndLossFactorTransfer);
	DDX_Control(pDX, IDC_CMD_LOSS_FACTOR_SERVICE, m_wndLossFactorService);
	DDX_Control(pDX, IDC_CMD_LOSS_FACTOR_CHK, m_wndLossFactorChk);
	DDX_Control(pDX, IDC_CMD_USE_CS_ANALYSIS_CHK, m_wndCSAnalysisChk);
	DDX_Control(pDX, IDC_CMD_AASHTO_SER_CULVERT_V_EDIT, m_wndCulvertV);
	DDX_Control(pDX, IDC_CMD_AASHTO_SER_CULVERT_H_EDIT, m_wndCulvertH);
	DDX_Control(pDX, IDC_CMD_AASHTO_STR_DL_CMB, m_StrDlCmb);
	DDX_Control(pDX, IDC_CMD_AASHTO_STR_EP_CMB, m_StrEpCmb);
	DDX_Control(pDX, IDC_CMD_AASHTO_SER_EP_CMB, m_SerEpCmb);
	DDX_Control(pDX, IDC_CMD_CURVE_CHK, m_CurvedBridgeChk);
	DDX_Control(pDX, IDC_CMD_MOVE_LIST, m_MovingList);
	DDX_Control(pDX, IDC_CMD_MOVE_CASE, m_MoveLcase);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_CODE, m_Code);
	DDX_Radio(pDX, IDC_CMD_LOADCOMB_ADD, m_nOption);
	DDX_Radio(pDX, IDC_CMD_DESIGN_TYPE_STL_RDO, m_nDesignType);  
    DDX_Check(pDX, IDC_CMD_ADDENVELOPE_CHK, m_bAddEnvelope);
	DDX_Check(pDX, IDC_IRC_SERVCOND_CHK, m_bIrcServCond);
	DDX_Check(pDX, IDC_IRC_CONSCOND_CHK, m_bIrcConsCond);
	DDX_Radio(pDX, IDC_CMD_DESIGN_CSLC_ST_RDO, m_nConstLoad);
	DDX_Check(pDX, IDC_JTJD60_CAPC_CHK, m_bJTJD60Capc);
	DDX_Check(pDX, IDC_JTJD60_GEN_CHK,  m_bJTJD60Gen);
	DDX_Check(pDX, IDC_JTJD60_ACDN_CHK, m_bJTJD60Acdn);
	DDX_Check(pDX, IDC_JTJD60_SERV_CHK, m_bJTJD60Serv);
	DDX_Check(pDX, IDC_JTJD60_ELST_CHK, m_bJTJD60Elst);
	DDX_Check(pDX, IDC_TB05_MAIN_CHK, m_bTB05Main);//add by maxiao 2007-04-24
	DDX_Check(pDX, IDC_TB05_MAIN_ADDITION_CHK, m_bTB05MainAddition);
	DDX_Check(pDX, IDC_TB05_MAIN_SPECIAL_CHK, m_bTB05MainSpecial);
	DDX_Check(pDX, IDC_TB100022017_MAIN_CHK, m_bTB10002_2017Main);
	DDX_Check(pDX, IDC_TB100022017_MAIN_ADDITION_CHK, m_bTB10002_2017MainAddition);
	DDX_Check(pDX, IDC_TB100022017_MAIN_SPECIAL_CHK, m_bTB10002_2017MainSpecial);
	DDX_Check(pDX, IDC_CJJ166_2011_CAPC_CHK, m_bCJJ166_2011Capc);//add by maxiao 2007-04-24
	DDX_Check(pDX, IDC_CJJ166_2011_GEN_CHK,  m_bCJJ166_2011Gen);
	DDX_Check(pDX, IDC_CJJ166_2011_ACDN_CHK, m_bCJJ166_2011Acdn);
	DDX_Check(pDX, IDC_CJJ166_2011_SERV_CHK, m_bCJJ166_2011Serv);
	DDX_Check(pDX, IDC_CJJ166_2011_ELST_CHK, m_bCJJ166_2011Elst);

	DDX_Check(pDX, IDC_JTG15_CAPC_CHK, m_bJTG15Capc);//add by dongqifueng 2015-10-16
	DDX_Check(pDX, IDC_JTG15_GEN_CHK,  m_bJTG15Gen);
	DDX_Check(pDX, IDC_JTG15_ACDN_CHK, m_bJTG15Acdn);
	DDX_Check(pDX, IDC_JTG15_SERV_CHK, m_bJTG15Serv);
	DDX_Check(pDX, IDC_JTG15_ELST_CHK, m_bJTG15Elst);
	DDX_Check(pDX, IDC_JTG15_FREQ_CHK,  m_bJTG15_Freq);
	DDX_Check(pDX, IDC_JTG15_QUASIPERM_CHK, m_bJTG15_QuasiPerm);
	DDX_Check(pDX, IDC_JTG15_FREQCOMB_CHK, m_bJTG15_FreqComb);
	DDX_Check(pDX, IDC_JTG15_QUASIPERMCOMB_CHK, m_bJTG15_QuasiPermComb);
	DDX_Check(pDX, IDC_JTG15_STANDCOMB_CHK, m_bJTG15_StandComb);
	DDX_Check(pDX, IDC_CMD_LOADCOM_AUTO_SERVICE_LIFE, m_bJTG15_Factor);
	DDX_Check(pDX, IDC_CHK_JTG15_FOUNDATION, m_bJTG15FoundationChk);
	DDX_Check(pDX, IDC_CHK_JTG15_FOUNDATION_COMMON, m_bJTG15FdnCommonChk);
	DDX_Check(pDX, IDC_CHK_JTG15_FOUNDATION_COMMON_BASIC, m_bJTG15FdnCommonBasicChk);
	DDX_Check(pDX, IDC_CHK_JTG15_FOUNDATION_COMMON_ACC, m_bJTG15FdnCommonAccChk);
	DDX_Check(pDX, IDC_CHK_JTG15_FOUNDATION_COMMON_FREQ, m_bJTG15FdnCommonFreqChk);
	DDX_Check(pDX, IDC_CHK_JTG15_FOUNDATION_COMMON_PERM, m_bJTG15FdnCommonPermChk);

	DDX_Check(pDX, IDC_CVL_GB19_CAPC_CHK,                 m_tLcomESCGB19.bGB19Capc      );
	DDX_Check(pDX, IDC_CVL_GB19_GEN_CHK,                  m_tLcomESCGB19.bGB19Gen       );
	DDX_Check(pDX, IDC_CVL_GB19_ACDN_CHK,                 m_tLcomESCGB19.bGB19Acdn      );
	DDX_Check(pDX, IDC_CVL_GB19_SERV_CHK,                 m_tLcomESCGB19.bGB19Serv      );
	DDX_Check(pDX, IDC_CVL_GB19_ELST_CHK,                 m_tLcomESCGB19.bGB19Elst      );
	DDX_Check(pDX, IDC_CVL_GB19_FREQ_CHK,                 m_tLcomESCGB19.bGB19_Freq     );
	DDX_Check(pDX, IDC_CVL_GB19_QUASIPERM_CHK,            m_tLcomESCGB19.bGB19_QuasiPerm);
	DDX_Check(pDX, IDC_CVL_GB19_FREQCOMB_CHK,             m_tLcomESCGB19.bGB19_FreqComb );
	DDX_Check(pDX, IDC_CVL_GB19_QUASIPERMCOMB_CHK,        m_tLcomESCGB19.bGB19_QuasiPermComb);
	DDX_Check(pDX, IDC_CVL_GB19_STANDCOMB_CHK,            m_tLcomESCGB19.bGB19_StandComb    );
	DDX_Check(pDX, IDC_CMD_LOADCOM_CVL_GB19_SERVICE_LIFE, m_tLcomESCGB19.bGB19_Factor       );

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlDlg, CDialog)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlDlg)
	ON_BN_CLICKED(IDC_CMD_LOSS_FACTOR_BTN, OnCmdLossFactorBtn)
	ON_BN_CLICKED(IDC_CMD_LOSS_FACTOR_CHK, OnCmdLossFactorChk)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_ADD, OnCmdLoadcombAdd)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_REPLACE, OnCmdLoadcombReplace)
	ON_CBN_SELCHANGE(IDC_CMD_LOADCOMB_CODE, OnSelchangeLoadcombCode)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_SEL_BTN, OnCmdLcomSelBtn)
	ON_BN_CLICKED(IDC_CMD_MOVE_ADD, OnCmdMoveAdd)
	ON_BN_CLICKED(IDC_CMD_MOVE_DEL, OnCmdMoveDel)
	ON_CBN_SELCHANGE(IDC_CMD_AASHTO_SER_EP_CMB, OnSelchangeCmdAashtoSerEpCmb)
	ON_BN_CLICKED(IDC_CMD_DESIGN_TYPE_STL_RDO, OnCmdDesignTypeRdo)
	ON_BN_CLICKED(IDC_CMD_DESIGN_TYPE_CON_RDO, OnCmdDesignTypeRdo)
	ON_BN_CLICKED(IDC_CMD_DESIGN_TYPE_SRC_RDO, OnCmdDesignTypeRdo)
	ON_BN_CLICKED(IDC_CMD_DESIGN_TYPE_STLCOMP_RDO, OnCmdDesignTypeRdo)
	ON_BN_CLICKED(IDC_CMD_DESIGN_CSLC_ST_RDO, OnCmdConstStageRdo)
	ON_BN_CLICKED(IDC_CMD_DESIGN_CSLC_CS_RDO, OnCmdConstStageRdo)
	ON_BN_CLICKED(IDC_CMD_DESIGN_CSLC_ALL_RDO, OnCmdConstStageRdo)
	ON_BN_CLICKED(IDC_JTJD60_CAPC_CHK, OnCmdJTJD60CapcChk)
	ON_BN_CLICKED(IDC_CJJ166_2011_CAPC_CHK, OnCmdCJJ166_2011CapcChk)

	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)

	ON_BN_CLICKED(IDC_JTG15_CAPC_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15CapcChk)
	ON_BN_CLICKED(IDC_JTG15_ACDN_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15AcdnChk)
	ON_BN_CLICKED(IDC_JTG15_SERV_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15ServChk)
	ON_BN_CLICKED(IDC_JTG15_ELST_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15ElstChk)
	ON_BN_CLICKED(IDC_JTG15_FREQ_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15FreqChk)
	ON_BN_CLICKED(IDC_JTG15_QUASIPERM_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15QuasipermChk)
	ON_BN_CLICKED(IDC_CMD_LOADCOM_AUTO_SERVICE_LIFE, &CCmdAutoLoadCombCvlDlg::OnBnClickedCmdLoadcomAutoServiceLife)
	ON_BN_CLICKED(IDC_CHK_JTG15_FOUNDATION_COMMON, &CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15FoundationCommonChk)
	ON_BN_CLICKED(IDC_CHK_JTG15_FOUNDATION, &CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15FoundationChk)

	ON_BN_CLICKED(IDC_CVL_GB19_CAPC_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19CapcChk)
	ON_BN_CLICKED(IDC_CVL_GB19_ACDN_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19AcdnChk)
	ON_BN_CLICKED(IDC_CVL_GB19_SERV_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19ServChk)
	ON_BN_CLICKED(IDC_CVL_GB19_ELST_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19ElstChk)
	ON_BN_CLICKED(IDC_CVL_GB19_FREQ_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19FreqChk)
	ON_BN_CLICKED(IDC_CVL_GB19_QUASIPERM_CHK, &CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19QuasipermChk)
	ON_BN_CLICKED(IDC_CMD_LOADCOM_CVL_GB19_SERVICE_LIFE, &CCmdAutoLoadCombCvlDlg::OnBnClickedCmdLoadcomESCGB19ServiceLife)


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlDlg message handlers

BOOL CCmdAutoLoadCombCvlDlg::OnInitDialog() 
{
	SetRedraw(FALSE);
	CDialog::OnInitDialog();

	MInitCombo init;
	m_pDoc = CDBDoc::GetDocPoint();
	m_bInit = FALSE;

	// child window objects
	m_pKoreaLRFD11 = new CCmdAutoLoadCombCvlKOREALRFD11Dlg(this);
	m_pAASHTOLRFD12 = new CCmdAutoLoadCombCvlAASHTOLRFD12Dlg(this);
	m_pCSA = new CCmdAutoLoadCombCvlCSAPage(this);
	m_pSNiP = new CCmdAutoLoadCombCvlSP35133302011Page(this);
	m_pSP3511 = new CCmdAutoLoadCombCvlSP35133302011Page(this);
	m_pCSA14 = new CCmdAutoLoadCombCvlCSAPage(this);
	m_pAS5100 = new CCmdAutoLoadCombCvlAS51002017Page(this);
	m_pKSCEUSD10 = new CCmdAutoLoadCombCvlKSCEUSD10Dlg(this);
    m_pBD21 = new CCmdAutoLoadCombCvlBD21Page(this);
    m_pPNS10030 = new CCmdAutoLoadCombCvlPNS10030Page(this);
    m_pEURO0 = new CCmdAutoLoadCombCvlEURO0Page(this);
    m_pIRC6LSD = new CCmdAutoLoadCombCvlIRC6LSDPage(this);
    m_pAASHTOLRFD02 = new CCmdAutoLoadCombCvlAASHTOLRFD02Dlg(this);
    m_pAASHTOLRFD08 = new CCmdAutoLoadCombCvlAASHTOLRFD08Dlg(this);
    m_pIRS = new CCmdAutoLoadCombCvlIRSPage(this);
	m_pTMH7 = new CCmdAutoLoadCombCvlTMH7Page(this);

	// init variables
	m_wndCulvertH.SetEditUnit(1.0);
	m_wndCulvertV.SetEditUnit(1.0);

	T_DCON_D DconData;
	DconData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcon(DconData);
	m_wndLossFactorTransfer.SetEditUnit(DconData.dTranLossFactor);
	m_wndLossFactorService.SetEditUnit(DconData.dServLossFactor);

	BOOL bCS = m_pDoc->m_pAttrCtrl->ExistConstStag();
	if(bCS) m_nConstLoad = 2;
	else    m_nConstLoad = 0;
	UpdateData(FALSE);

	Initial_MovingLoadCaseComboBox();  
	Initial_CtrlData();   
	Initial_Data();    // must be excuted after Initial_CtrlData()
	Initial_MoveListBox();  
	SetLoadCombSelData();

	// below functions access childWindow
	OnSelchangeLoadcombCode();
	CtrlManager();
	OnSelchangeCmdAashtoSerEpCmb();   
    
	OnCmdJTJD60CapcChk();
	OnCmdCJJ166_2011CapcChk();

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;
}

void CCmdAutoLoadCombCvlDlg::InitChildDialog(const CString& strCodeName) const
{
	const auto CreateChildDialog = [&](unsigned int holderNum, CChildDialog* dlg)
	{
		const auto pPlaceHolder = GetDlgItem(holderNum);
		if (!dlg)
			return;
		dlg->CreateInit(pPlaceHolder);
		dlg->ShowWindow(SW_SHOW);
	};

	if (strCodeName == AutoLoadComb::KSCE_LSD15)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER, m_pKoreaLRFD11);
		m_pKoreaLRFD11->m_nConstLoad = m_nConstLoad;
		m_pKoreaLRFD11->OnCmdKoreaLRFD11EnableDisable();
	}
	else if (strCodeName == AutoLoadComb::AASHTO_LRFD12 || strCodeName == AutoLoadComb::AASHTO_LRFD16 ||
			 strCodeName == AutoLoadComb::AASHTO_LRFD17 || strCodeName == AutoLoadComb::AASHTO_LRFD20 ||
			 strCodeName == AutoLoadComb::AASHTO_LRFD24)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER2, m_pAASHTOLRFD12);
		BOOL bAASHTO16 = (strCodeName == AutoLoadComb::AASHTO_LRFD16) ? TRUE : FALSE;
		BOOL bAASHTO18 = (strCodeName == AutoLoadComb::AASHTO_LRFD17) ? TRUE : FALSE;
		BOOL bAASHTO20 = (strCodeName == AutoLoadComb::AASHTO_LRFD20) ? TRUE : FALSE;
		BOOL bAASHTO24 = (strCodeName == AutoLoadComb::AASHTO_LRFD24) ? TRUE : FALSE;
		m_pAASHTOLRFD12->SetAASHTOLrfd16(bAASHTO16);
		m_pAASHTOLRFD12->SetAASHTOLrfd18(bAASHTO18);
		m_pAASHTOLRFD12->SetAASHTOLrfd20(bAASHTO20);
		m_pAASHTOLRFD12->SetAASHTOLrfd24(bAASHTO24);
		m_pAASHTOLRFD12->m_nConstLoad = m_nConstLoad;
		m_pAASHTOLRFD12->OnCmdAASHTOLRFDEnableDisable();
	}
	else if (strCodeName == AutoLoadComb::CSA_S6S1_10)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER3, m_pCSA);
		m_pCSA->m_nConstLoad = m_nConstLoad;
		m_pCSA->OnCmdCSAEnableDisable();

		// CSA-S6S1-10 changes Text
		m_pCSA->ChangeText(m_nDesignType);
	}
	else if (strCodeName ==AutoLoadComb::SNIP)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER4, m_pSNiP);
		m_pSNiP->m_nConstLoad = m_nConstLoad;
		m_pSNiP->OnCmdSP3511EnableDisable();
	}
	else if (strCodeName == AutoLoadComb::SP3511)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER5, m_pSP3511);
		m_pSP3511->m_nConstLoad = m_nConstLoad;
		m_pSP3511->OnCmdSP3511EnableDisable();
	}
	else if (strCodeName == AutoLoadComb::CSA_S6_19 || strCodeName == AutoLoadComb::CSA_S6_14)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER6, m_pCSA14);
		m_pCSA14->m_nConstLoad = m_nConstLoad;
		m_pCSA14->OnCmdCSAEnableDisable();

		// CSA-S6S1-10 changes Text
		m_pCSA14->ChangeText(m_nDesignType);
	}
	else if (strCodeName == AutoLoadComb::AS5100)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER7, m_pAS5100);
		m_pAS5100->m_nConstLoad = m_nConstLoad;
		m_pAS5100->OnCmdAS5100EnableDisable();
	}
	else if (strCodeName == AutoLoadComb::KSCEUSD10)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER8, m_pKSCEUSD10);
		m_pKSCEUSD10->OnCmdKSCEUSD10EnableDisable();
	}
	else if (strCodeName == AutoLoadComb::BD21)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER9, m_pBD21);
		m_pBD21->m_nConstLoad = m_nConstLoad;
		m_pBD21->OnCmdBD21EnableDisable();
	}
	else if (strCodeName == AutoLoadComb::PNS10030)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER10, m_pPNS10030);
		m_pPNS10030->m_nConstLoad = m_nConstLoad;
		m_pPNS10030->OnCmdPNS10030EnableDisable();
	}
	else if (strCodeName == AutoLoadComb::EURO0)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER11, m_pEURO0);
		m_pEURO0->m_nConstLoad = m_nConstLoad;
		m_pEURO0->OnCmdEURO0EnableDisable();
	}
	else if (strCodeName == AutoLoadComb::IRC6LSD)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER12, m_pIRC6LSD);
		m_pIRC6LSD->m_nConstLoad = m_nConstLoad;
		m_pIRC6LSD->OnCmdIRC6LSDEnableDisable();
	}
	else if (strCodeName == AutoLoadComb::AASHTOLRFD02)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER13, m_pAASHTOLRFD02);
		m_pAASHTOLRFD02->m_nConstLoad = m_nConstLoad;
		m_pAASHTOLRFD02->OnCmdAASHTOLRFD02EnableDisable();
	}
	else if (strCodeName == AutoLoadComb::AASHTOLRFD08)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER14, m_pAASHTOLRFD08);
		m_pAASHTOLRFD08->m_nConstLoad = m_nConstLoad;
		m_pAASHTOLRFD08->OnCmdAASHTOLRFD08EnableDisable();
	}
	else if (strCodeName == AutoLoadComb::IRS)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER15, m_pIRS);
		m_pIRS->m_nConstLoad = m_nConstLoad;
		m_pIRS->OnCmdIRSEnableDisable();
	}
	else if (strCodeName == AutoLoadComb::TMH7)
	{
		CreateChildDialog(IDC_CMD_LCOM_PLACE_HOLDER16, m_pTMH7);
		m_pTMH7->m_nConstLoad = m_nConstLoad;
		m_pTMH7->OnCmdBD21EnableDisable();
	}
}

////////////////////////////////////////////
//  #define D_LCOMTYPE_GENERAL     1
//  #define D_LCOMTYPE_STEEL       2
//  #define D_LCOMTYPE_CONCRETE    3
//  #define D_LCOMTYPE_BASEPLATE   4
///////////////////////////////////////////
void CCmdAutoLoadCombCvlDlg::Initial_Data()
{
	//m_nOption = 0;  //  0 : Add,   1 : Replace
	m_Code.ResetContent();

	if(m_nLcomType == D_LCOMTYPE_GENERAL) m_bAddEnvelope = TRUE;
	else                                  m_bAddEnvelope = FALSE; 

	CStringArray GenCodeName;	  
	if(m_nDesignType == 0) // Steel
	{
		CDBLib::GetStlLoadCombList(GenCodeName);
		for (int i = 0; i < GenCodeName.GetSize(); i++)
		{
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(GenCodeName[i]);
#endif
			m_Code.AddString(GenCodeName.GetAt(i));
		}
	}
	else if(m_nDesignType == 1) // Concrete
	{
		CDBLib::GetConLoadCombList(GenCodeName);
		for (int i = 0; i < GenCodeName.GetSize(); i++)
		{
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(GenCodeName[i]);
#endif
			m_Code.AddString(GenCodeName.GetAt(i));
		}
	}
	else if(m_nDesignType == 2) // SRC
	{
		CDBLib::GetSrcLoadCombList(GenCodeName);
		for (int i = 0; i < GenCodeName.GetSize(); i++)
		{
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(GenCodeName[i]);
#endif
			m_Code.AddString(GenCodeName.GetAt(i));
		}
	}
	else if(m_nDesignType == 3) // Steel Composite
	{
		CDBLib::GetStlCompLoadCombList(GenCodeName);
		for (int i = 0; i < GenCodeName.GetSize(); i++)
		{
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(GenCodeName[i]);
#endif
			m_Code.AddString(GenCodeName.GetAt(i));
		}
	}
	if(m_Code.GetCount() > 0)	m_Code.SetCurSel(0);

	if(m_MoveLcase.GetCount() != 0)  m_MoveLcase.SetCurSel(0);

	UpdateData(FALSE);
}

void CCmdAutoLoadCombCvlDlg::Initial_CtrlData()
{
	m_aCtrlMoveCase.Add(IDC_CMD_MOVE_FRAME);
	m_aCtrlMoveCase.Add(IDC_CMD_LOADCASE);
	m_aCtrlMoveCase.Add(IDC_CMD_MOVE_CASE);
	m_aCtrlMoveCase.Add(IDC_CMD_MOVE_ADD);
	m_aCtrlMoveCase.Add(IDC_CMD_MOVE_DEL);
	m_aCtrlMoveCase.Add(IDC_CMD_MOVE_LIST);
	m_aCtrlMoveCase.Add(IDC_CMD_CURVE_CHK);


	// AASHTO Standard 2000
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_SER_FRAME);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_SER_EP_TEXT);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_SER_EP_CMB);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_SER_CULVERT_FRAME);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_SER_CULVERT_H_TEXT);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_SER_CULVERT_V_TEXT);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_SER_CULVERT_H_EDIT);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_SER_CULVERT_V_EDIT);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_STR_FRAME);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_STR_EP_TEXT);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_STR_EP_CMB);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_STR_DL_TEXT);
	m_aCtrlAashto.Add(IDC_CMD_AASHTO_STR_DL_CMB);

	m_aCtrlKOREALRFD11.Add(IDC_CMD_LCOM_PLACE_HOLDER);
	m_aCtrlAASHTOLRFD12.Add(IDC_CMD_LCOM_PLACE_HOLDER2);
	m_aCtrlCSA.Add(IDC_CMD_LCOM_PLACE_HOLDER3);
	m_aCtrlCSA14.Add(IDC_CMD_LCOM_PLACE_HOLDER6);
	m_aCtrlSNiP.Add(IDC_CMD_LCOM_PLACE_HOLDER4);
	m_aCtrlSP3511.Add(IDC_CMD_LCOM_PLACE_HOLDER5);
	m_aCtrlAS5100.Add(IDC_CMD_LCOM_PLACE_HOLDER7);
	m_aCtrlKSCEUSD10.Add(IDC_CMD_LCOM_PLACE_HOLDER8);
    m_aCtrlBD21.Add(IDC_CMD_LCOM_PLACE_HOLDER9);
    m_aCtrlPNS10030.Add(IDC_CMD_LCOM_PLACE_HOLDER10);
    m_aCtrlEC4.Add(IDC_CMD_LCOM_PLACE_HOLDER11);
    m_aCtrlIRC6_LSD.Add(IDC_CMD_LCOM_PLACE_HOLDER12);
    m_aCtrlAASHTOLRFD02.Add(IDC_CMD_LCOM_PLACE_HOLDER13);
    m_aCtrlAASHTOLRFD08.Add(IDC_CMD_LCOM_PLACE_HOLDER14);
    m_aCtrlIRS.Add(IDC_CMD_LCOM_PLACE_HOLDER15);
	m_aCtrlTMH7.Add(IDC_CMD_LCOM_PLACE_HOLDER16);

	m_aCtrlIRC6_2000.Add(IDC_IRC_STATIC);
	m_aCtrlIRC6_2000.Add(IDC_IRC_SERVCOND_CHK);
	m_aCtrlIRC6_2000.Add(IDC_IRC_CONSCOND_CHK);

	m_aCtrlJSCE02.Add(IDC_CMD_JSCE02_FRM);
	m_aCtrlJSCE02.Add(IDC_CMD_JSCE02_CHK1);
	m_aCtrlJSCE02.Add(IDC_CMD_JSCE02_CHK2);
	m_aCtrlJSCE02.Add(IDC_CMD_JSCE02_CHK3);
	m_aCtrlJSCE02.Add(IDC_CMD_JSCE02_CHK4);
	m_aCtrlJSCE02.Add(IDC_CMD_JSCE02_CHK5);
	m_aCtrlJSCE02.Add(IDC_CMD_JSCE02_CHK6);
	m_aCtrlJSCE02.Add(IDC_CMD_JSCE02_CHK7);

	// JTG D60-04
	m_aCtrlJTJD60.Add(IDC_JTJD60_STC);
	m_aCtrlJTJD60.Add(IDC_JTJD60_CAPC_CHK);
	m_aCtrlJTJD60.Add(IDC_JTJD60_GEN_CHK);
	m_aCtrlJTJD60.Add(IDC_JTJD60_ACDN_CHK);
	m_aCtrlJTJD60.Add(IDC_JTJD60_SERV_CHK);
	m_aCtrlJTJD60.Add(IDC_JTJD60_ELST_CHK);

	RECT recRef;
	GetDlgItem(IDC_CMD_CSLC_DSC1_STA)->GetWindowRect(&recRef);

	//CJJ 166_2011  add by maxiao 2012-03-01
	RECT recCJJ1662011_sup;
	GetDlgItem(IDC_CJJ166_2011_STC)->GetWindowRect(&recCJJ1662011_sup);
	m_aCtrlCJJ166_2011.Add(IDC_CJJ166_2011_STC);
	m_aCtrlCJJ166_2011.Add(IDC_CJJ166_2011_CAPC_CHK);
	m_aCtrlCJJ166_2011.Add(IDC_CJJ166_2011_GEN_CHK);
	m_aCtrlCJJ166_2011.Add(IDC_CJJ166_2011_ACDN_CHK);
	m_aCtrlCJJ166_2011.Add(IDC_CJJ166_2011_SERV_CHK);
	m_aCtrlCJJ166_2011.Add(IDC_CJJ166_2011_ELST_CHK);
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlCJJ166_2011, recRef.left - recCJJ1662011_sup.left);

	// TB10002.1-2005  add by maxiao 2007-04-24
	m_aCtrlTB05.Add(IDC_TB05_STC);
	m_aCtrlTB05.Add(IDC_TB05_MAIN_CHK);
	m_aCtrlTB05.Add(IDC_TB05_MAIN_ADDITION_CHK);
	m_aCtrlTB05.Add(IDC_TB05_MAIN_SPECIAL_CHK);

	//TB10002-2017
	RECT recTB10022017_sup;
	GetDlgItem(IDC_TB100022017_STC)->GetWindowRect(&recTB10022017_sup);
	m_aCtrlTB10002_2017.Add(IDC_TB100022017_STC);
	m_aCtrlTB10002_2017.Add(IDC_TB100022017_MAIN_CHK);
	m_aCtrlTB10002_2017.Add(IDC_TB100022017_MAIN_ADDITION_CHK);
	m_aCtrlTB10002_2017.Add(IDC_TB100022017_MAIN_SPECIAL_CHK);
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlTB10002_2017, recRef.left - recTB10022017_sup.left);

	//////////////////////////////////////////////////////////////////////////
	//add by dongqiufeng 2015-10-16
	RECT recRJtg15_sup;
	GetDlgItem(IDC_JTG15_CAPC_CHK)->GetWindowRect(&recRJtg15_sup);

	m_aCtrlJTG15.Add(IDC_JTG15_STC);
	m_aCtrlJTG15.Add(IDC_CHK_JTG15_FOUNDATION);
	m_aCtrlJTG15.Add(IDC_CMD_LOADCOM_AUTO_SERVICE_LIFE);
	m_aCtrlJTG15.Add(IDC_CMD_LOADCOM_AUTO_FACTOR_TXT);
	m_aCtrlJTG15.Add(IDC_CMD_LOADCOM_AUTO_FACTOR_EDT);
	m_aCtrlJTG15.Add(IDC_COMBTYPE_TXT);

	m_aCtrlJTG15_Sup.Add(IDC_JTG15_CAPC_CHK);
	m_aCtrlJTG15_Sup.Add(IDC_JTG15_GEN_CHK);
	m_aCtrlJTG15_Sup.Add(IDC_JTG15_ACDN_CHK);
	m_aCtrlJTG15_Sup.Add(IDC_JTG15_FREQ_CHK);
	m_aCtrlJTG15_Sup.Add(IDC_JTG15_QUASIPERM_CHK);
	m_aCtrlJTG15_Sup.Add(IDC_JTG15_SERV_CHK);
	m_aCtrlJTG15_Sup.Add(IDC_JTG15_FREQCOMB_CHK);
	m_aCtrlJTG15_Sup.Add(IDC_JTG15_QUASIPERMCOMB_CHK);
	m_aCtrlJTG15_Sup.Add(IDC_JTG15_ELST_CHK);
	m_aCtrlJTG15_Sup.Add(IDC_JTG15_STANDCOMB_CHK);

	RECT recRJtg15_sub;
	GetDlgItem(IDC_CHK_JTG15_FOUNDATION_COMMON)->GetWindowRect(&recRJtg15_sub);
	m_aCtrlJTG15_Sub.Add(IDC_CHK_JTG15_FOUNDATION_COMMON);
	m_aCtrlJTG15_Sub.Add(IDC_CHK_JTG15_FOUNDATION_COMMON_BASIC);
	m_aCtrlJTG15_Sub.Add(IDC_CHK_JTG15_FOUNDATION_COMMON_ACC);
	m_aCtrlJTG15_Sub.Add(IDC_CHK_JTG15_FOUNDATION_COMMON_FREQ);
	m_aCtrlJTG15_Sub.Add(IDC_CHK_JTG15_FOUNDATION_COMMON_PERM);

	m_aCtrlJTG15.Append(m_aCtrlJTG15_Sup);
	m_aCtrlJTG15.Append(m_aCtrlJTG15_Sub);

	CDlgUtil::CtrlMoveDistX(this, m_aCtrlJTG15    , recRef.left - recRJtg15_sup.left);
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlJTG15_Sub, recRJtg15_sup.left - recRJtg15_sub.left);
	//////////////////////////////////////////////////////////////////////////
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_STC);
	m_aCtrlESCGB19.Add(IDC_CMD_LOADCOM_CVL_GB19_SERVICE_LIFE);
	m_aCtrlESCGB19.Add(IDC_CMD_LOADCOM_CVL_GB19_FACTOR_TXT);
	m_aCtrlESCGB19.Add(IDC_CMD_LOADCOM_CVL_GB19_FACTOR_EDT);
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_COMBTYPE_TXT);
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_CAPC_CHK);
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_GEN_CHK);
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_ACDN_CHK);
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_FREQ_CHK);
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_QUASIPERM_CHK);
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_SERV_CHK);
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_FREQCOMB_CHK);
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_QUASIPERMCOMB_CHK);
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_ELST_CHK);
	m_aCtrlESCGB19.Add(IDC_CVL_GB19_STANDCOMB_CHK);
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlESCGB19, globalUtils.ScaleByDPI(-350));
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlESCGB19, globalUtils.ScaleByDPI(-166));
	//////////////////////////////////////////////////////////////////////////
	//add by qiangeng 2019-1-18
	m_aCtrlSTL_JTG15.Add(IDC_JTG15_STC);
	m_aCtrlSTL_JTG15.Add(IDC_CHK_JTG15_FOUNDATION);
	m_aCtrlSTL_JTG15.Add(IDC_CMD_LOADCOM_AUTO_SERVICE_LIFE);
	m_aCtrlSTL_JTG15.Add(IDC_CMD_LOADCOM_AUTO_FACTOR_TXT);
	m_aCtrlSTL_JTG15.Add(IDC_CMD_LOADCOM_AUTO_FACTOR_EDT);
	m_aCtrlSTL_JTG15.Add(IDC_COMBTYPE_TXT);
	m_aCtrlSTL_JTG15.Add(IDC_JTG15_GEN_CHK);
	m_aCtrlSTL_JTG15.Add(IDC_JTG15_ACDN_CHK);
	m_aCtrlSTL_JTG15.Add(IDC_JTG15_FREQ_CHK);
	m_aCtrlSTL_JTG15.Add(IDC_JTG15_QUASIPERM_CHK);
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlSTL_JTG15, 0);
	//////////////////////////////////////////////////////////////////////////

	// PS狼 Loss Factor 绊妨
	m_aCtrlPS.Add(IDC_CMD_USE_CS_ANALYSIS_CHK);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_FRAME);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_CHK);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_TRANSFER);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_SERVICE);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_TRANSFER_TEXT);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_SERVICE_TEXT);
	m_aCtrlPS.Add(IDC_CMD_LOSS_FACTOR_BTN);
	// CS load combination
	m_aCtrlCS.Add(IDC_CMD_CSLC_FRAME);
	m_aCtrlCS.Add(IDC_CMD_CSLC_DSC1_STA);
	m_aCtrlCS.Add(IDC_CMD_CSLC_DSC2_STA);
	m_aCtrlCS.Add(IDC_CMD_DESIGN_CSLC_ST_RDO);
	m_aCtrlCS.Add(IDC_CMD_DESIGN_CSLC_CS_RDO);
	m_aCtrlCS.Add(IDC_CMD_DESIGN_CSLC_ALL_RDO);

	m_aCtrlOkCancel.Add(IDOK);
	m_aCtrlOkCancel.Add(IDCANCEL);  

	// Design Type狼 Radio Button 劝己拳 累诀
	GetDlgItem(IDC_CMD_DESIGN_TYPE_STL_RDO    )->EnableWindow(m_nLcomType == D_LCOMTYPE_GENERAL || m_nLcomType == D_LCOMTYPE_STEEL);
	GetDlgItem(IDC_CMD_DESIGN_TYPE_CON_RDO    )->EnableWindow(m_nLcomType == D_LCOMTYPE_GENERAL || m_nLcomType == D_LCOMTYPE_CONCRETE);
	GetDlgItem(IDC_CMD_DESIGN_TYPE_SRC_RDO    )->EnableWindow(m_nLcomType == D_LCOMTYPE_GENERAL || m_nLcomType == D_LCOMTYPE_SRC);
	GetDlgItem(IDC_CMD_DESIGN_TYPE_STLCOMP_RDO)->EnableWindow(m_nLcomType == D_LCOMTYPE_GENERAL || m_nLcomType == D_LCOMTYPE_STLCOMP);

	// Envelope Check Box22++狼 Show or hide : General牢 版快父 焊咯霖促.
	GetDlgItem(IDC_CMD_ADDENVELOPE_CHK)->ShowWindow(m_nLcomType == D_LCOMTYPE_GENERAL);

	if     (m_nLcomType == D_LCOMTYPE_GENERAL)  m_nDesignType = 0;
	else if(m_nLcomType == D_LCOMTYPE_STEEL)    m_nDesignType = 0;
	else if(m_nLcomType == D_LCOMTYPE_CONCRETE) m_nDesignType = 1;
	else if(m_nLcomType == D_LCOMTYPE_SRC)      m_nDesignType = 2;
	else if(m_nLcomType == D_LCOMTYPE_STLCOMP)  m_nDesignType = 3;

	CString aSerEpData[]  = {_LS(IDS_CMD_AASHTO_SER_EP1),         _LS(IDS_CMD_AASHTO_SER_EP2),
		_LS(IDS_CMD_AASHTO_SER_EP3)};
	CString aStrEpData[]  = {_LS(IDS_CMD_AASHTO_STR_EP_NON_CUL1), _LS(IDS_CMD_AASHTO_STR_EP_NON_CUL2),
		_LS(IDS_CMD_AASHTO_STR_EP_NON_CUL3), _LS(IDS_CMD_AASHTO_STR_EP_CUL1),     
		_LS(IDS_CMD_AASHTO_STR_EP_CUL2)};
	CString aStrDlData[]  = {_LS(IDS_CMD_AASHTO_STR_DL1),         _LS(IDS_CMD_AASHTO_STR_DL2), 
		_LS(IDS_CMD_AASHTO_STR_DL3)};

	int i, nIndex;
	for(i = 0; i < sizeof(aSerEpData)/sizeof(CString); i++)  
	{
		nIndex = m_SerEpCmb.AddString(aSerEpData[i]);
		m_SerEpCmb.SetItemData(nIndex, i);    
	}
	for(i = 0; i < sizeof(aStrEpData)/sizeof(CString); i++)
	{
		nIndex = m_StrEpCmb.AddString(aStrEpData[i]);
		m_StrEpCmb.SetItemData(nIndex, i);
	}
	for(i = 0; i < sizeof(aStrDlData)/sizeof(CString); i++)
	{
		nIndex = m_StrDlCmb.AddString(aStrDlData[i]);
		m_StrDlCmb.SetItemData(nIndex, i);
	}

	m_SerEpCmb.SetCurSel(0);
	m_StrEpCmb.SetCurSel(0);
	m_StrDlCmb.SetCurSel(0);

	CDC* pDC = GetDC();  
	CSize max, cur;
	max = pDC->GetTextExtent(aSerEpData[0]);
	for(i = 1; i < sizeof(aSerEpData)/sizeof(CString); i++)
	{
		cur = pDC->GetTextExtent(aSerEpData[i]); 
		if (cur.cx > max.cx) max.cx = cur.cx;
	}
	if (max.cx > m_SerEpCmb.GetDroppedWidth()) m_SerEpCmb.SetDroppedWidth(max.cx);

	max = pDC->GetTextExtent(aStrEpData[0]);
	for(i = 1; i < sizeof(aStrEpData)/sizeof(CString); i++)
	{
		cur = pDC->GetTextExtent(aStrEpData[i]); 
		if (cur.cx > max.cx) max.cx = cur.cx;
	}
	if (max.cx > m_StrEpCmb.GetDroppedWidth()) m_StrEpCmb.SetDroppedWidth(max.cx);

	max = pDC->GetTextExtent(aStrDlData[0]);
	for(i = 1; i < sizeof(aStrDlData)/sizeof(CString); i++)
	{
		cur = pDC->GetTextExtent(aStrDlData[i]); 
		if (cur.cx > max.cx) max.cx = cur.cx;
	}
	if (max.cx > m_StrDlCmb.GetDroppedWidth()) m_StrDlCmb.SetDroppedWidth(max.cx);

	ReleaseDC(pDC);	  
}

void CCmdAutoLoadCombCvlDlg::CtrlManager()
{
	UpdateData(TRUE);

	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();
	BOOL bPsExist = m_nConstLoad==0 ? FALSE:TRUE;

	if(iCount!=0 && !bPsExist)
	{
		for(int i = 0; i < iCount; i++)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
			ASSERT(bCheck);            
			if(rData.LoadCaseType==_T("PS")) bPsExist = TRUE;  // Prestress      
		}    
	}
	// KJH20040206 颊角伏篮 汲拌俊辑 荤侩登绰巴捞促. 瘤陛 civil俊辑绰 PS汲拌 决蝶. 弊贰辑 阜疽.. 狙......
	BOOL bPostMode = FALSE;
	if(m_pDoc->IsPostMode()) bPostMode = TRUE;
	// CSAnalysis客 LossFactorChk绰 Prestress Load Case啊 乐绰 版快父 荤侩(劝己拳)
	m_wndCSAnalysisChk.EnableWindow(bPsExist && !bPostMode);   // CSAnalysis绰 PostMode俊辑绰 厚劝己拳
	m_wndLossFactorChk.EnableWindow(bPsExist);

	BOOL bLossFactorChk = m_wndLossFactorChk.GetCheck();
	m_wndLossFactorTransfer.EnableWindow(bPsExist && bLossFactorChk);
	m_wndLossFactorService.EnableWindow(bPsExist && bLossFactorChk);    
	GetDlgItem(IDC_CMD_LOSS_FACTOR_TRANSFER_TEXT)->EnableWindow(bPsExist && bLossFactorChk);
	GetDlgItem(IDC_CMD_LOSS_FACTOR_SERVICE_TEXT)->EnableWindow(bPsExist && bLossFactorChk);
	GetDlgItem(IDC_CMD_LOSS_FACTOR_TRANSFER_TEXT)->EnableWindow(bPsExist && bLossFactorChk);
	GetDlgItem(IDC_CMD_LOSS_FACTOR_SERVICE_TEXT)->EnableWindow(bPsExist && bLossFactorChk);
	GetDlgItem(IDC_CMD_LOSS_FACTOR_BTN)->EnableWindow(bPsExist && bLossFactorChk);

	UpdateData(FALSE);
}

void CCmdAutoLoadCombCvlDlg::OnCmdDesignTypeRdo()
{
	UpdateData(TRUE);

	SetLoadCombSelData();
	Initial_Data();
	AlignControl();

	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	if(Index != -1)	m_Code.GetLBText(Index,strCodeName);

#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif

	if(m_nDesignType == 0 || m_nDesignType == 1 || m_nDesignType == 3)
	{
		if(strCodeName == _T("KSCE-LSD15")) GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(TRUE);
		else                            GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(FALSE);
	}
}

void CCmdAutoLoadCombCvlDlg::OnCmdLossFactorChk()
{
	CtrlManager();
}

void CCmdAutoLoadCombCvlDlg::Initial_MovingLoadCaseComboBox()
{
	m_MoveLcase.ResetContent();
	CArray<T_MVLD_K, T_MVLD_K> KeyList;
	CArray<T_MVLDjp_K, T_MVLDjp_K> KeyListjp;
	CArray<T_MVLDch_K, T_MVLDch_K> KeyListch;
	CArray<T_MVLDid_K, T_MVLDid_K> KeyListid;
	CArray<T_MVLDbs_K, T_MVLDbs_K> KeyListbs;
	CArray<T_MVLDfr_K, T_MVLDfr_K> KeyListfr;
	CArray<T_MVLDtr_K, T_MVLDtr_K> KeyListtr;

	int nCount = 0;
	// 内靛俊 蝶弗 规侥栏肺 荐沥 
	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	switch (DataMvcd.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_JAPAN_RAIL:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		m_pDoc->m_pAttrCtrl->GetMvldKeyList(KeyList);      nCount = KeyList.GetSize(); 
		break;
	case D_MOVE_CODE_CHINA:
		m_pDoc->m_pAttrCtrl->GetMvldchKeyList(KeyListch);  nCount = KeyListch.GetSize();
		break;
	case D_MOVE_CODE_JAPAN:
		m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(KeyListjp);  nCount = KeyListjp.GetSize();
		break;
	case D_MOVE_CODE_INDIA:
		m_pDoc->m_pAttrCtrl->GetMvldidKeyList(KeyListid);  nCount = KeyListid.GetSize(); 
		break;
	case D_MOVE_CODE_BS:
		m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(KeyListbs);  nCount = KeyListbs.GetSize(); 
		break;
	case D_MOVE_CODE_FRANCE:
		m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(KeyListfr);  nCount = KeyListfr.GetSize();
		break;
	case D_MOVE_CODE_TRANS:
		m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(KeyListtr);  nCount = KeyListtr.GetSize(); 
		break;
	default:
		ASSERT(0);
		break;
	}

	if(nCount > 0)
	{
		//  Moving Load Case Data啊 乐澜.
		for(int i=0; i<nCount; i++)
		{      
			T_MVLD_D rData;
			T_MVLDjp_D rDatajp;        
			T_MVLDch_D rDatach;
			T_MVLDid_D rDataid;
			T_MVLDbs_D rDatabs;
			T_MVLDfr_D rDatafr;
			T_MVLDtr_D rDatatr;

			rData.Initialize();
			m_pDoc->m_pAttrCtrl->InitializeMvldjp(rDatajp);
			rDatach.Initialize();

			int key;
			BOOL bCheck;
			// 内靛俊 蝶弗 规侥栏肺 荐沥 
			switch (DataMvcd.nCodeType)
			{
			case D_MOVE_CODE_NONE:
				break;
			case D_MOVE_CODE_AASHTO_STAN:
			case D_MOVE_CODE_AASHTO_LRFD:
			case D_MOVE_CODE_TAIWAN:
			case D_MOVE_CODE_KOREA:
			case D_MOVE_CODE_CANADA:
			case D_MOVE_CODE_PENDOT:
			case D_MOVE_CODE_EURO_BS:
			case D_MOVE_CODE_RUSSIA:
			case D_MOVE_CODE_KOREA_LRFD_2011:
			case D_MOVE_CODE_AUSTRALIA:
			case D_MOVE_CODE_POLAND:
			case D_MOVE_CODE_SOUTH_AFRICA:
			case D_MOVE_CODE_JAPAN_RAIL:
			case D_MOVE_CODE_NEWZEALAND:
			case D_MOVE_CODE_BRAZIL:
				key = KeyList.GetAt(i);   bCheck = m_pDoc->m_pAttrCtrl->GetMvld(key,rData);
				break;
			case D_MOVE_CODE_CHINA:
				key = KeyListch.GetAt(i); bCheck = m_pDoc->m_pAttrCtrl->GetMvldch(key,rDatach);
				break;
			case D_MOVE_CODE_JAPAN:
				key = KeyListjp.GetAt(i); bCheck = m_pDoc->m_pAttrCtrl->GetMvldjp(key,rDatajp);
				break;
			case D_MOVE_CODE_INDIA:
				key = KeyListid.GetAt(i);   bCheck = m_pDoc->m_pAttrCtrl->GetMvldid(key,rDataid);
				break;
			case D_MOVE_CODE_BS:
				key = KeyListbs.GetAt(i);   bCheck = m_pDoc->m_pAttrCtrl->GetMvldbs(key,rDatabs);
				break;
			case D_MOVE_CODE_FRANCE:
				key = KeyListfr.GetAt(i);   bCheck = m_pDoc->m_pAttrCtrl->GetMvldfr(key, rDatafr);
				break;
			case D_MOVE_CODE_TRANS:
				key = KeyListtr.GetAt(i);   bCheck = m_pDoc->m_pAttrCtrl->GetMvldtr(key, rDatatr);
				break;
			default:
				ASSERT(0);
				break;
			}
			ASSERT(bCheck);

			CString str;
			// 内靛俊 蝶弗 规侥栏肺 荐沥 
			switch (DataMvcd.nCodeType)
			{
			case D_MOVE_CODE_NONE:
				break;
			case D_MOVE_CODE_AASHTO_STAN:
			case D_MOVE_CODE_AASHTO_LRFD:
			case D_MOVE_CODE_TAIWAN:
			case D_MOVE_CODE_KOREA:
			case D_MOVE_CODE_CANADA:
			case D_MOVE_CODE_PENDOT:
			case D_MOVE_CODE_EURO_BS:
			case D_MOVE_CODE_RUSSIA:
			case D_MOVE_CODE_KOREA_LRFD_2011:
			case D_MOVE_CODE_AUSTRALIA:	
			case D_MOVE_CODE_POLAND:
			case D_MOVE_CODE_SOUTH_AFRICA:
			case D_MOVE_CODE_JAPAN_RAIL:
			case D_MOVE_CODE_NEWZEALAND:
			case D_MOVE_CODE_BRAZIL:
				str = rData.LoadCaseName;
				break;
			case D_MOVE_CODE_CHINA:
				str = rDatach.LoadCaseName;
				break;
			case D_MOVE_CODE_JAPAN:
				str = rDatajp.LoadCaseName;
				break;
			case D_MOVE_CODE_INDIA:
				str = rDataid.LoadCaseName;
				break;
			case D_MOVE_CODE_BS:
				str = rDatabs.LoadCaseName;
				break;
			case D_MOVE_CODE_FRANCE:
				str = rDatafr.LoadCaseName;
				break;
			case D_MOVE_CODE_TRANS:
				str = rDatatr.LoadCaseName;
				break;
			default:
				ASSERT(0);
				break;
			}
			m_MoveLcase.AddString(str);
		}
		Show_MoveEditComboBox(TRUE);
	}
	else
		Show_MoveEditComboBox(FALSE);
}

void CCmdAutoLoadCombCvlDlg::Show_MoveEditComboBox(BOOL bCheck)
{
	m_MoveLcase.EnableWindow(bCheck);
	m_MovingList.EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_MOVE_ADD)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_MOVE_DEL)->EnableWindow(bCheck);  
}

void CCmdAutoLoadCombCvlDlg::Initial_MoveListBox()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_MovingList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_MovingList.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__Load_Case) };

	CString title;
	int width[1] = {140};
	for(int i = 0; i < sizeof(aTitle) / sizeof(CString); i++)
	{
		title = aTitle[i];

		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_LEFT;
		lvcolumn.pszText  = title.GetBuffer(0);
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_MovingList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCmdAutoLoadCombCvlDlg::OnSelchangeLoadcombCode() 
{
	SetRedraw(FALSE);

	const int Index = m_Code.GetCurSel();
	CString strCodeName = _T("");
	if (Index != -1)	m_Code.GetLBText(Index, strCodeName);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif
	InitChildDialog(strCodeName);

	BOOL bJTJFlag = FALSE;
	BOOL bCJJFlag = FALSE;
	const BOOL bMoveCount = m_MoveLcase.GetCount()>0 ? TRUE : FALSE;

    if(strCodeName == _T("JTJ021-89"))     bJTJFlag = TRUE;
	else if(strCodeName == _T("CJJ77-98")) bCJJFlag = TRUE;

	GetDlgItem(IDC_CMD_MOVE_LIST)->EnableWindow(bJTJFlag && bMoveCount);
	GetDlgItem(IDC_CMD_MOVE_CASE)->EnableWindow(bJTJFlag && bMoveCount);
	GetDlgItem(IDC_CMD_MOVE_ADD)->EnableWindow(bJTJFlag && bMoveCount);
	GetDlgItem(IDC_CMD_MOVE_DEL)->EnableWindow(bJTJFlag && bMoveCount);
	GetDlgItem(IDC_CMD_CURVE_CHK)->EnableWindow(bJTJFlag || bCJJFlag);

	BOOL bCS = m_pDoc->m_pAttrCtrl->ExistConstStag();
	if(bCS) m_nConstLoad = 2;
	else    m_nConstLoad = 0;

	AlignControl();
    
	if(m_nDesignType == 0 || m_nDesignType == 1 || m_nDesignType == 3)
	{
		if(strCodeName == _T("KSCE-LSD15")) GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(TRUE);
		else                            GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(FALSE);
	}

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

void CCmdAutoLoadCombCvlDlg::OnCmdLcomSelBtn() 
{
	CCmdAutoLoadCombSelCvlDlg dlg;
	dlg.m_nDesignType = m_nDesignType;

	dlg.m_bUlti1 = m_bUlti1;
	dlg.m_bUlti2 = m_bUlti2;
	dlg.m_bUlti3 = m_bUlti3;
	dlg.m_bUlti4 = m_bUlti4;
	dlg.m_bUlti5 = m_bUlti5;
	dlg.m_bExtr1 = m_bExtr1;
	dlg.m_bExtr2 = m_bExtr2;
	dlg.m_bServ1 = m_bServ1;
	dlg.m_bServ2 = m_bServ2;
	dlg.m_bServ3 = m_bServ3;
	dlg.m_bServ4 = m_bServ4;
	dlg.m_bServ5 = m_bServ5;
	dlg.m_bFati1 = m_bFati1;

	if(dlg.DoModal() == IDOK)
	{
		m_bUlti1 = dlg.m_bUlti1;
		m_bUlti2 = dlg.m_bUlti2;
		m_bUlti3 = dlg.m_bUlti3;
		m_bUlti4 = dlg.m_bUlti4;
		m_bUlti5 = dlg.m_bUlti5;
		m_bExtr1 = dlg.m_bExtr1;
		m_bExtr2 = dlg.m_bExtr2;
		m_bServ1 = dlg.m_bServ1;
		m_bServ2 = dlg.m_bServ2;
		m_bServ3 = dlg.m_bServ3;
		m_bServ4 = dlg.m_bServ4;
		m_bServ5 = dlg.m_bServ5;
		m_bFati1 = dlg.m_bFati1;
	}
}

void CCmdAutoLoadCombCvlDlg::OnSelchangeCmdAashtoSerEpCmb() 
{
	int Index = m_SerEpCmb.GetCurSel();
	GetDlgItem(IDC_CMD_AASHTO_SER_CULVERT_H_EDIT)->EnableWindow(Index == 1);
	GetDlgItem(IDC_CMD_AASHTO_SER_CULVERT_V_EDIT)->EnableWindow(Index == 1);
}

void CCmdAutoLoadCombCvlDlg::MoveJTG15Widgets()
{
	CArray<UINT, UINT> aCtrl;
	aCtrl.Add(IDC_JTG15_GEN_CHK);
	aCtrl.Add(IDC_JTG15_ACDN_CHK);
	aCtrl.Add(IDC_JTG15_FREQ_CHK);
	aCtrl.Add(IDC_JTG15_QUASIPERM_CHK);

	CRect rRef;
	CRect rToMove;
	double nDistY;
	GetDlgItem(IDC_JTG15_CAPC_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_JTG15_GEN_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

	CRect rRefFdn, rMoveFdn1, rMoveFdn2;
	GetDlgItem(IDC_JTG15_CAPC_CHK)->GetWindowRect(rRefFdn);
	GetDlgItem(IDC_CHK_JTG15_FOUNDATION_COMMON)->GetWindowRect(rMoveFdn1);//通用组合
	double dMoveFdnY = rMoveFdn1.top - rRefFdn.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlJTG15_Sub, dMoveFdnY);

	GetDlgItem(IDC_JTG15_QUASIPERM_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_JTG15_STC)->GetWindowRect(rToMove);
	CRect rRefFdnLast;
	GetDlgItem(IDC_CHK_JTG15_FOUNDATION_COMMON_PERM)->GetWindowRect(rRefFdnLast);
	double dHeight1  = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
	double dHeight2 = rRefFdnLast.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
	double dHeight = max(dHeight1, dHeight2);
	double dWidth  = rToMove.Width();
	GetDlgItem(IDC_JTG15_STC)->SetWindowPos(NULL, 0/*rToMove.left*/, 0/*rToMove.top*/, dWidth/*rToMove.Width()*/, dHeight, SWP_NOZORDER | SWP_NOMOVE);


	m_bJTG15_Freq = TRUE;
	m_bJTG15_QuasiPerm = TRUE;
	((CButton*)GetDlgItem(IDC_JTG15_FREQ_CHK))->SetCheck(m_bJTG15_Freq);
	((CButton*)GetDlgItem(IDC_JTG15_QUASIPERM_CHK))->SetCheck(m_bJTG15_QuasiPerm);
}


void CCmdAutoLoadCombCvlDlg::RecoverJTG15Widgets()
{
	//承载能力
	CArray<UINT, UINT> aCtrl;
	aCtrl.Add(IDC_JTG15_GEN_CHK);
	aCtrl.Add(IDC_JTG15_ACDN_CHK);
	aCtrl.Add(IDC_JTG15_FREQ_CHK);
	aCtrl.Add(IDC_JTG15_QUASIPERM_CHK);
	//正常使用
	CArray<UINT, UINT> aCtrl2;
	aCtrl2.Add(IDC_JTG15_SERV_CHK);
	aCtrl2.Add(IDC_JTG15_FREQCOMB_CHK);
	aCtrl2.Add(IDC_JTG15_QUASIPERMCOMB_CHK);
	aCtrl2.Add(IDC_JTG15_ELST_CHK);
	aCtrl2.Add(IDC_JTG15_STANDCOMB_CHK);

	CRect rRef;
	CRect rToMove;
	double nDistY0, nDistY;
	GetDlgItem(IDC_COMBTYPE_TXT)->GetWindowRect(rRef);//组合类型
	GetDlgItem(IDC_JTG15_CAPC_CHK)->GetWindowRect(rToMove);//承载能力
	nDistY0 = rToMove.top - rRef.top;

	GetDlgItem(IDC_JTG15_CAPC_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_JTG15_GEN_CHK)->GetWindowRect(rToMove);//基本组合
	nDistY = rRef.top - rToMove.top + nDistY0;
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

	CRect rRefFdn,rMoveFdn1,rMoveFdn2;
	GetDlgItem(IDC_JTG15_CAPC_CHK)->GetWindowRect(rRefFdn);
	GetDlgItem(IDC_CHK_JTG15_FOUNDATION_COMMON)->GetWindowRect(rMoveFdn1);//通用组合
	double dMoveFdnY = rMoveFdn1.top - rRefFdn.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlJTG15_Sub, dMoveFdnY);

	GetDlgItem(IDC_JTG15_QUASIPERM_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_JTG15_SERV_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top + nDistY0;
	CDlgUtil::CtrlMoveDistY(this, aCtrl2, nDistY);

	GetDlgItem(IDC_JTG15_STANDCOMB_CHK)->GetWindowRect(rRef);//标准组合
	GetDlgItem(IDC_JTG15_STC)->GetWindowRect(rToMove);
	double dHeight = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
	double dWidth  = rToMove.Width();
	GetDlgItem(IDC_JTG15_STC)->SetWindowPos(NULL, 0/*rToMove.left*/, 0/*rToMove.top*/, dWidth/*rToMove.Width()*/, dHeight, SWP_NOZORDER | SWP_NOMOVE);

	m_bJTG15_Freq = TRUE;
	m_bJTG15_QuasiPerm = FALSE;
	((CButton*)GetDlgItem(IDC_JTG15_FREQ_CHK))->SetCheck(m_bJTG15_Freq);
	((CButton*)GetDlgItem(IDC_JTG15_QUASIPERM_CHK))->SetCheck(m_bJTG15_QuasiPerm);
}

void CCmdAutoLoadCombCvlDlg::AlignControl()
{
	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	if(Index != -1)	m_Code.GetLBText(Index,strCodeName);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif
	// 阿 内靛喊 静捞绰 Ctrl阑 沥困摹 矫挪促.
	CRect rRef;
	CRect rToMove;
	int nDistY;	
	UINT aFirstCtrlID[] = {
        IDC_CMD_MOVE_FRAME, 
        IDC_CMD_AASHTO_SER_FRAME, 
		IDC_IRC_STATIC, 
        IDC_CMD_JSCE02_FRM, 
        IDC_JTJD60_STC, 
        IDC_TB05_STC,
        IDC_CMD_LCOM_PLACE_HOLDER, 
        IDC_CJJ166_2011_STC,
        IDC_JTG15_STC,
		IDC_CVL_GB19_STC,
        IDC_JTG15_STC,
		IDC_CMD_LCOM_PLACE_HOLDER2, 
        IDC_CMD_LCOM_PLACE_HOLDER3, 
        IDC_CMD_LCOM_PLACE_HOLDER4, 
		IDC_CMD_LCOM_PLACE_HOLDER5, 
        IDC_CMD_LCOM_PLACE_HOLDER6, 
		IDC_CMD_LCOM_PLACE_HOLDER7, 
        IDC_TB100022017_STC, 
        IDC_CMD_LCOM_PLACE_HOLDER8,
        IDC_CMD_LCOM_PLACE_HOLDER9, 
        IDC_CMD_LCOM_PLACE_HOLDER10,
        IDC_CMD_LCOM_PLACE_HOLDER11,
        IDC_CMD_LCOM_PLACE_HOLDER12,
        IDC_CMD_LCOM_PLACE_HOLDER13,
        IDC_CMD_LCOM_PLACE_HOLDER14,
        IDC_CMD_LCOM_PLACE_HOLDER15,
        IDC_CMD_LCOM_PLACE_HOLDER16

    };//add by dongqiufeng 2015-10-15
	CArray<UINT, UINT> *aCtrlSet[] = {
        &m_aCtrlMoveCase, 
        &m_aCtrlAashto, 
		&m_aCtrlIRC6_2000, 
        &m_aCtrlJSCE02, 
        &m_aCtrlJTJD60, 
        &m_aCtrlTB05, 
        &m_aCtrlKOREALRFD11, 
        &m_aCtrlCJJ166_2011,
        &m_aCtrlJTG15, 
		&m_aCtrlESCGB19, 
        &m_aCtrlSTL_JTG15, 
        &m_aCtrlAASHTOLRFD12, 
        &m_aCtrlCSA, 
        &m_aCtrlSNiP, 
        &m_aCtrlSP3511,
        &m_aCtrlCSA14, 
        &m_aCtrlAS5100, 
        &m_aCtrlTB10002_2017, 
		&m_aCtrlKSCEUSD10,
        &m_aCtrlBD21, 
        &m_aCtrlPNS10030, 
        &m_aCtrlEC4,
        &m_aCtrlIRC6_LSD,
        &m_aCtrlAASHTOLRFD02,
        &m_aCtrlAASHTOLRFD08,
        &m_aCtrlIRS,
		&m_aCtrlTMH7
    };//PinakinIRC6LSD//add by dongqiufeng 2015-10-15
	CWnd *pWnd;
#if defined(_CIVIL)
	pWnd = GetDlgItem(IDC_CMD_CSLC_FRAME);
#else
	pWnd = m_nDesignType==1 ? GetDlgItem(IDC_CMD_LOSS_FACTOR_FRAME) : GetDlgItem(IDC_CMD_CSLC_FRAME);
#endif
	pWnd->GetWindowRect(rRef);

	for(int i=0; i<sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		CDlgUtil::CtrlShowHide(this, *aCtrlSet[i], FALSE); // 20071023 mylee - 颗辨锭 措拳芒 柄廉辑 炼摹
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, *aCtrlSet[i], nDistY);
	}

	if (strCodeName == _LSX(JTG D60-15) || strCodeName == _LSX(CJJ11-2019) || strCodeName == SRCCODE_JTGD60_2015)
	{
	  if (m_nDesignType == 0) MoveJTG15Widgets();
	  else	RecoverJTG15Widgets();
	}

	if     (strCodeName == _LSX(JTJ021-89))        GetDlgItem(IDC_CMD_CURVE_CHK)->GetWindowRect(rRef);
	else if(strCodeName == _LSX(CJJ77-98))         GetDlgItem(IDC_CMD_CURVE_CHK)->GetWindowRect(rRef);
	else if(strCodeName == _LSX(AASHTO-Std2K) || strCodeName == _LSX(Taiwan))
		GetDlgItem(IDC_CMD_AASHTO_STR_FRAME)->GetWindowRect(rRef);  
	else if(strCodeName == _LSX(IRC:6-2000))       GetDlgItem(IDC_IRC_STATIC)->GetWindowRect(rRef);
	else if(strCodeName == _LSX(JRoad-H14/H24))    GetDlgItem(IDC_CMD_JSCE02_FRM)->GetWindowRect(rRef);
	else if(strCodeName == _LSX(JTG D60-04))       GetDlgItem(IDC_JTJD60_STC)->GetWindowRect(rRef);
	else if(strCodeName == _LSX(TB 10002.1-05))    GetDlgItem(IDC_TB05_STC)->GetWindowRect(rRef); //add by maxiao
	else if(strCodeName == _LSX(TB 10002-2017))    GetDlgItem(IDC_TB100022017_STC)->GetWindowRect(rRef);
	//else if(strCodeName == AutoLoadComb::KSCE_LSD15)  GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER)->GetWindowRect(rRef);
	else if(strCodeName == AutoLoadComb::KSCE_LSD15)       m_pKoreaLRFD11->GetWindowRect(rRef);		
	else if(strCodeName == _LSX(CJJ11-2011))       GetDlgItem(IDC_CJJ166_2011_STC)->GetWindowRect(rRef);//add by maxiao
	else if(strCodeName == AutoLoadComb::AASHTO_LRFD12 || strCodeName == AutoLoadComb::AASHTO_LRFD16 || strCodeName == AutoLoadComb::AASHTO_LRFD17 ||
		strCodeName == AutoLoadComb::AASHTO_LRFD20 || strCodeName == AutoLoadComb::AASHTO_LRFD24)
		m_pAASHTOLRFD12->GetWindowRect(rRef);		
	else if(strCodeName == AutoLoadComb::CSA_S6S1_10)      m_pCSA->GetWindowRect(rRef);	
	else if(strCodeName == AutoLoadComb::CSA_S6_14)        m_pCSA14->GetWindowRect(rRef);
	else if(strCodeName == AutoLoadComb::CSA_S6_19)        m_pCSA14->GetWindowRect(rRef);
	else if(strCodeName ==AutoLoadComb::SNIP) m_pSNiP->GetWindowRect(rRef);	
	else if(strCodeName == AutoLoadComb::SP3511) m_pSP3511->GetWindowRect(rRef);	
	else if(strCodeName == _LSX(JTG D60-15)|| strCodeName == SRCCODE_JTGD60_2015)       GetDlgItem(IDC_JTG15_STC)->GetWindowRect(rRef);//add by dongqiufeng
	else if(strCodeName == _LSX(CJJ11-2019))       GetDlgItem(IDC_JTG15_STC)->GetWindowRect(rRef);
	else if(strCodeName == _LS(IDS_DB_LCOM_CODE_ENGNEERINGSTRUCTURECOMMONCODE_GB2019))     
	{
		GetDlgItem(IDC_CVL_GB19_STC)->GetWindowRect(rRef);
	}
	else if(strCodeName == _LSX(PN-85/S-10030))    m_pPNS10030->GetWindowRect(rRef);
	else if(strCodeName == _LSX(AS 5100.2:17))     m_pAS5100->GetWindowRect(rRef);
	else if(strCodeName == _LSX(KSCE-USD10))       m_pKSCEUSD10->GetWindowRect(rRef);
    else if(strCodeName == _LSX(BS 5400))          m_pBD21->GetWindowRect(rRef);
    else if(strCodeName == _LSX(Eurocode 0))       m_pEURO0->GetWindowRect(rRef);
    else if(strCodeName == _LSX(IRC:6 LSD))        m_pIRC6LSD->GetWindowRect(rRef);//PinakinIRC6LSD
    else if(strCodeName == _LSX(AASHTO-LRFD02))    m_pAASHTOLRFD02->GetWindowRect(rRef);
    else if(strCodeName == _LSX(AASHTO-LRFD07))    m_pAASHTOLRFD08->GetWindowRect(rRef);
    else if(strCodeName == _LSX(IRS))              m_pIRS->GetWindowRect(rRef);
	else if(strCodeName == _LSX(TMH07-1981))	   m_pTMH7->GetWindowRect(rRef);
	else pWnd->GetWindowRect(rRef);

	double dBottom = rRef.bottom;

	ScreenToClient(rRef);
	if     (strCodeName == AutoLoadComb::KSCE_LSD15)       GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER)->MoveWindow(rRef);
	else if(strCodeName == AutoLoadComb::AASHTO_LRFD12 || strCodeName==AutoLoadComb::AASHTO_LRFD16 || strCodeName==AutoLoadComb::AASHTO_LRFD17 ||
		strCodeName == AutoLoadComb::AASHTO_LRFD20 || strCodeName == AutoLoadComb::AASHTO_LRFD24)    GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER2)->MoveWindow(rRef);
	else if(strCodeName == AutoLoadComb::CSA_S6S1_10)      GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER3)->MoveWindow(rRef);
	else if(strCodeName == AutoLoadComb::CSA_S6_14)        GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER6)->MoveWindow(rRef);
	else if(strCodeName == AutoLoadComb::CSA_S6_19)        GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER6)->MoveWindow(rRef);
	else if(strCodeName ==AutoLoadComb::SNIP) GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER4)->MoveWindow(rRef);
	else if(strCodeName == AutoLoadComb::SP3511) GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER5)->MoveWindow(rRef);
	else if(strCodeName == AutoLoadComb::AS5100)	   GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER7)->MoveWindow(rRef);
	else if(strCodeName == AutoLoadComb::KSCEUSD10)	   GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER8)->MoveWindow(rRef);
    else if(strCodeName == AutoLoadComb::BD21)		   GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER9)->MoveWindow(rRef);
    else if(strCodeName == AutoLoadComb::PNS10030)    GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER10)->MoveWindow(rRef);
    else if(strCodeName == AutoLoadComb::EURO0)       GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER11)->MoveWindow(rRef);
    else if(strCodeName == AutoLoadComb::IRC6LSD)        GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER12)->MoveWindow(rRef);
    else if(strCodeName == AutoLoadComb::AASHTOLRFD02)    GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER13)->MoveWindow(rRef);
    else if(strCodeName == AutoLoadComb::AASHTOLRFD08)    GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER14)->MoveWindow(rRef);
    else if(strCodeName == AutoLoadComb::IRS)              GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER15)->MoveWindow(rRef);
    else if(strCodeName == AutoLoadComb::TMH7)             GetDlgItem(IDC_CMD_LCOM_PLACE_HOLDER16)->MoveWindow(rRef);

	// OK, CANCEL阑 捞悼矫挪促.
	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	nDistY = dBottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlOkCancel, nDistY);

	// 扩档快 荤捞令甫 嘎冕促.
	CWnd* pWndLast = GetDlgItem(IDOK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	// MQC-18235, Civil俊辑 利侩窍瘤 臼绰 PS, Loss factor, Hide
	BOOL bShowPSLossFactor = m_nDesignType==1;
#if defined(_CIVIL)
	bShowPSLossFactor = FALSE;
#endif // DEBUG 


	// 内靛喊肺 鞘夸茄 Ctrl父 焊捞霸 茄促.
	CDlgUtil::CtrlShowHide(this, m_aCtrlMoveCase,     strCodeName == _LSX(JTJ021-89) || strCodeName == _LSX(CJJ77-98));  
	CDlgUtil::CtrlShowHide(this, m_aCtrlAashto,       strCodeName == _LSX(AASHTO-Std2K) || strCodeName == _LSX(Taiwan));
	CDlgUtil::CtrlShowHide(this, m_aCtrlIRC6_2000,    strCodeName == _LSX(IRC:6-2000));//PinakinRC6LSD
	CDlgUtil::CtrlShowHide(this, m_aCtrlJSCE02,       strCodeName == _LSX(JRoad-H14/H24));
	CDlgUtil::CtrlShowHide(this, m_aCtrlJTJD60,       strCodeName == _LSX(JTG D60-04));
	CDlgUtil::CtrlShowHide(this, m_aCtrlTB05,         strCodeName == _LSX(TB 10002.1-05));//add by maxiao 2007-04-24
	CDlgUtil::CtrlShowHide(this, m_aCtrlTB10002_2017, strCodeName == _LSX(TB 10002-2017) );
	CDlgUtil::CtrlShowHide(this, m_aCtrlPS,           bShowPSLossFactor);  
	CDlgUtil::CtrlShowHide(this, m_aCtrlKOREALRFD11,  strCodeName == AutoLoadComb::KSCE_LSD15);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCJJ166_2011,  strCodeName == _LSX(CJJ11-2011));//add by maxiao 2012-3-1
	CDlgUtil::CtrlShowHide(this, m_aCtrlAASHTOLRFD12, strCodeName == AutoLoadComb::AASHTO_LRFD12 || strCodeName==AutoLoadComb::AASHTO_LRFD16 || strCodeName==AutoLoadComb::AASHTO_LRFD17 || strCodeName==AutoLoadComb::AASHTO_LRFD20 || strCodeName == AutoLoadComb::AASHTO_LRFD24);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCSA,          strCodeName == AutoLoadComb::CSA_S6S1_10);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCSA14,        strCodeName == AutoLoadComb::CSA_S6_14 || strCodeName == AutoLoadComb::CSA_S6_19);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSNiP,         strCodeName ==AutoLoadComb::SNIP);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSP3511,       strCodeName == AutoLoadComb::SP3511);
	//CDlgUtil::CtrlShowHide(this, m_aCtrlJTG15,        strCodeName == _LSX(JTG D60-15));//add by dongqiufeng 2015-10-15
	//CDlgUtil::CtrlShowHide(this, m_aCtrlSTL_JTG15,    strCodeName == _LSX(JTG D60-15) && m_nDesignType == 0);//add by qiangeng  2019-1-18
	CDlgUtil::CtrlShowHide(this, m_aCtrlAS5100,		  strCodeName == AutoLoadComb::AS5100);
	CDlgUtil::CtrlShowHide(this, m_aCtrlKSCEUSD10,    strCodeName == AutoLoadComb::KSCEUSD10);
    CDlgUtil::CtrlShowHide(this, m_aCtrlBD21,		  strCodeName == AutoLoadComb::BD21);
    CDlgUtil::CtrlShowHide(this, m_aCtrlPNS10030,     strCodeName == AutoLoadComb::PNS10030);//add by Polish 2016-2-11
    CDlgUtil::CtrlShowHide(this, m_aCtrlEC4,          strCodeName == AutoLoadComb::EURO0);
    CDlgUtil::CtrlShowHide(this, m_aCtrlIRC6_LSD,     strCodeName == AutoLoadComb::IRC6LSD);
    CDlgUtil::CtrlShowHide(this, m_aCtrlAASHTOLRFD02, strCodeName == AutoLoadComb::AASHTOLRFD02);
    CDlgUtil::CtrlShowHide(this, m_aCtrlAASHTOLRFD08, strCodeName == AutoLoadComb::AASHTOLRFD08);
    CDlgUtil::CtrlShowHide(this, m_aCtrlIRS,          strCodeName == AutoLoadComb::IRS);
	CDlgUtil::CtrlShowHide(this, m_aCtrlTMH7,         strCodeName == AutoLoadComb::TMH7);

	//钢混组合预应力工况损失、弹性
	if (strCodeName == _LSX(JTG D60-15) || strCodeName == _LSX(CJJ11-2019) || strCodeName == SRCCODE_JTGD60_2015)
	{
	  if (m_nDesignType == 0)
	  {  
		  CDlgUtil::CtrlShowHide(this, m_aCtrlSTL_JTG15, TRUE);
		  CDlgUtil::CtrlShowHide(this, m_aCtrlJTG15_Sub, FALSE);
	  }
	  else
	  {
		  CDlgUtil::CtrlShowHide(this, m_aCtrlJTG15, TRUE);
		  if (m_bJTG15FoundationChk && m_nDesignType == 1)
		  {
			  CDlgUtil::CtrlShowHide(this, m_aCtrlJTG15_Sup, FALSE);
			  CDlgUtil::CtrlShowHide(this, m_aCtrlJTG15_Sub, TRUE);
		  }
		  else
		  {
			  CDlgUtil::CtrlShowHide(this, m_aCtrlJTG15_Sup, TRUE);
			  CDlgUtil::CtrlShowHide(this, m_aCtrlJTG15_Sub, FALSE);
		  }
	  }

		if (m_nDesignType == 2 || m_nDesignType == 3)
		{
			CDlgUtil::CtrlEnableDisable(this,IDC_JTG15_ELST_CHK, FALSE);
			CDlgUtil::CtrlEnableDisable(this,IDC_JTG15_STANDCOMB_CHK, FALSE);

			((CButton*)GetDlgItem(IDC_JTG15_ELST_CHK))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_JTG15_STANDCOMB_CHK))->SetCheck(FALSE);
		}
		else
		{
			CDlgUtil::CtrlEnableDisable(this,IDC_JTG15_ELST_CHK, TRUE);
			CDlgUtil::CtrlEnableDisable(this,IDC_JTG15_STANDCOMB_CHK, TRUE);

			((CButton*)GetDlgItem(IDC_JTG15_ELST_CHK))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_JTG15_STANDCOMB_CHK))->SetCheck(TRUE);
		}
	}
	else if (strCodeName == _LS(IDS_DB_LCOM_CODE_ENGNEERINGSTRUCTURECOMMONCODE_GB2019))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlESCGB19, TRUE);

		CDlgUtil::CtrlEnableDisable(this,IDC_CVL_GB19_ELST_CHK, TRUE);
		CDlgUtil::CtrlEnableDisable(this,IDC_CVL_GB19_STANDCOMB_CHK, TRUE);

		((CButton*)GetDlgItem(IDC_CVL_GB19_ELST_CHK))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CVL_GB19_STANDCOMB_CHK))->SetCheck(TRUE);
	}
	GetDlgItem(IDC_CHK_JTG15_FOUNDATION)->EnableWindow(m_nDesignType == 1);

	BOOL bCS = m_pDoc->m_pAttrCtrl->ExistConstStag();
	if ((strCodeName == _LSX(JTG D60-15) || strCodeName == SRCCODE_JTGD60_2015) && m_nDesignType == 0)   bCS = TRUE;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCS, bCS); 

	//BOOL bFold = m_chkLoadFactor.GetFoldState();
	//BOOL bAASHTO08 = FALSE;
	//if(strCodeName == _T("AASHTO-LRFD07")) bAASHTO08 = TRUE;

	//CDlgUtil::CtrlShowHide(this, m_aCtrlAashtoLrfd08FoldGroup, !bFold && bAASHTO08);

	// 烙矫
	BOOL bSTonly = (strCodeName==_LSX(KCI-USD99) || strCodeName==_LSX(ACI318-02) || strCodeName==_LSX(KCI-USD12));// || strCodeName==_LSX(KSCE-USD96);
	GetDlgItem(IDC_CMD_DESIGN_CSLC_CS_RDO)->EnableWindow(!bSTonly && bCS);
	GetDlgItem(IDC_CMD_DESIGN_CSLC_ALL_RDO)->EnableWindow(!bSTonly && bCS);
	if (strCodeName==AutoLoadComb::IRC6LSD)//PinakinIRC6LSD
	{
		// 	  GetDlgItem(IDC_IRC_SERVCOND_CHK)->SetWindowTextA(_LS(IDS_CMD_CONSIDER_ULTIMATE_LIMIT_STATE));
		// 	  GetDlgItem(IDC_IRC_CONSCOND_CHK)->SetWindowTextA(_LS(IDS_CMD_CONSIDER_SERVICEABILITY_LIMIT_STATE));
	}
	else
	{
		GetDlgItem(IDC_IRC_SERVCOND_CHK)->SetWindowText(_LS(IDS_CMD_CONSIDER_SERVICE_CONDITION));
		GetDlgItem(IDC_IRC_CONSCOND_CHK)->SetWindowText(_LS(IDS_CMD_CONSIDER_CONSTRUCTION_CONDITION));
	}
	if(bSTonly)
	{
		m_nConstLoad = 0;
		UpdateData(FALSE);
	}
	GetDlgItem(IDC_CMD_LOADCOM_AUTO_FACTOR_EDT)->SetWindowText(_T("1.0"));//add by donqiufeng 2015-10-16
	GetDlgItem(IDC_CMD_LOADCOM_CVL_GB19_FACTOR_EDT)->SetWindowText(_T("1.0"));
}

void CCmdAutoLoadCombCvlDlg::OnCmdMoveAdd() 
{
	UpdateData(TRUE);

	int nTrailer = 1;  // 乞魄苞瞒 咯措瞒
	//  Moving Load Case Data狼 鉴锅阑 掘绢可聪促.
	int ListNo;
	int Index = m_MoveLcase.GetCurSel();
	if(Index != -1)
	{
		CString strName=_T("");
		m_MoveLcase.GetLBText(Index,strName);

		if(CheckMovingCaseName(strName, ListNo))  //  TRUE=Add
		{      
            UINT nKey = GetMovingKey(strName);
			if(nKey != 0) m_MovingData.SetAt(nKey, nTrailer);      
			int No = m_MovingList.GetItemCount();
			Write_MoveListBox(No, strName);
		}
		else                        //  FALSE=Modify or Cancel
		{
			CString msg;      
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), strName);
			AfxMessageBox(msg);
			return;
		}
	}
	else AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));  	
}

BOOL CCmdAutoLoadCombCvlDlg::CheckMovingCaseName(CString str,int& Index)
{
	BOOL bCheck=TRUE;
	int Count = m_MovingList.GetItemCount();
	for(int i = 0; i < Count; i++)
	{
		CString strLcName = m_MovingList.GetItemText(i,0);
		if(str == strLcName)  
		{
			Index = i;
			bCheck = FALSE;
		}
	}
	return bCheck;
}

void CCmdAutoLoadCombCvlDlg::Write_MoveListBox(int Index, CString strLoadCase)
{
	LV_ITEM lvitem;
	//  ListCtrl Box俊 蔼阑 涝仿钦聪促.	
	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 0;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strLoadCase;
	m_MovingList.InsertItem(&lvitem);	
}

void CCmdAutoLoadCombCvlDlg::OnCmdMoveDel() 
{
	int nItem = m_MovingList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}
	CString strName=_T("");
	strName = m_MovingList.GetItemText(nItem, 0);

	UINT nKey = GetMovingKey(strName);
	if (nKey == 0) return;

	m_MovingData.RemoveKey(nKey);
	m_MovingList.DeleteItem(nItem);
	int nCount = m_MovingList.GetItemCount();
	if (nCount <= nItem) nItem -= 1;
	if (nItem >= 0) 
		m_MovingList.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCmdAutoLoadCombCvlDlg::OnCmdLoadcombAdd() 
{
	m_nOption = 0;
	//  UpdateData(FALSE);
}

void CCmdAutoLoadCombCvlDlg::OnCmdLoadcombReplace() 
{
	m_nOption = 1;
	//  UpdateData(FALSE);
}

void CCmdAutoLoadCombCvlDlg::OnOK()
{
	UpdateData(TRUE);

	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	if(Index != -1)	m_Code.GetLBText(Index,strCodeName);

#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif

	CString csErrMsg;
	int nType;
	if      (m_nDesignType == 0) nType = CLoadDgnCodeChecker::eLCBSteel;
	else if (m_nDesignType == 1) nType = CLoadDgnCodeChecker::eLCBConcrete;
	else if (m_nDesignType == 2) nType = CLoadDgnCodeChecker::eLCBSRC;
	else if (m_nDesignType == 3) nType = CLoadDgnCodeChecker::eLCBStlComp;
	else { ASSERT(0); return; }
	BOOL bAllow = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(nType, strCodeName, csErrMsg);
	if (!bAllow)
	{
		AfxMessageBox(csErrMsg);
		return;
	}

#if defined(_CIVIL_RUS) || defined (_CIVIL_US)
	if (strCodeName==AutoLoadComb::SNIP || strCodeName==AutoLoadComb::SP3511 || strCodeName==AutoLoadComb::PNS10030)
	{
		if(!m_pDoc->AllowCommand(D_OPTN_ID_RUS_RCPSC_DGN)) 
		{
			AfxMessageBox(_LS(IDS_MAIN_NO_LICENSE_RUS_RCPSC_DGN), MB_OK);
			return;    
		}
	}

	if(m_nDesignType == 3) // Steel Composite
	{
		if (strCodeName == AutoLoadComb::AASHTO_LRFD24 || strCodeName == AutoLoadComb::AASHTO_LRFD20 || strCodeName==AutoLoadComb::AASHTO_LRFD17 || strCodeName==AutoLoadComb::AASHTO_LRFD16 || strCodeName==AutoLoadComb::AASHTO_LRFD12 || strCodeName==AutoLoadComb::AASHTOLRFD08)
		{
			if(!m_pDoc->AllowCommand(D_OPTN_ID_AASHTO_STL_COMP)) 
			{
				AfxMessageBox(_LS(IDS_MAIN_NO_LICENSE_AASHTO_STL_COMP), MB_OK);
				return;    
			}
		}
	}

	if (strCodeName==AutoLoadComb::AS5100)
	{
		if(!m_pDoc->AllowCommand(D_OPTN_ID_AASHTO_STL_COMP)) 
		{
			AfxMessageBox(_LS(IDS_MAIN_NO_LICENSE_AASHTO_STL_COMP), MB_OK);
			return;    
		}
	}
#elif defined (_CIVIL_ORG)
	if(strCodeName == AutoLoadComb::KSCE_LSD15 || strCodeName == AutoLoadComb::AASHTO_LRFD24 || strCodeName == AutoLoadComb::AASHTO_LRFD20 || strCodeName==AutoLoadComb::AASHTO_LRFD17 || strCodeName==AutoLoadComb::AASHTO_LRFD16 || strCodeName == AutoLoadComb::AASHTO_LRFD12 ||
		strCodeName == AutoLoadComb::CSA_S6S1_10 || strCodeName == AutoLoadComb::CSA_S6_14 || strCodeName == AutoLoadComb::CSA_S6_19)
	{
		if(!m_pDoc->AllowCommand(D_OPTN_ID_LSD)) 
		{
			AfxMessageBox(_T("LSD 包访 扁瓷阑 荤侩且 鼻茄捞 绝绰 滚傈涝聪促."), MB_OK);
			return;    
		}
	}
#endif

	if(!ErrorCheckPS()) return;

	if(CDBLib::IsAutoLoadCombCodeName(strCodeName))
	{
		CLoadCombCtrl LoadCombCtrl;
		LoadCombCtrl.m_nDesignType   = m_nDesignType;
		LoadCombCtrl.Set_LcomType(m_nLcomType);
		LoadCombCtrl.Set_MovingLoadData(m_MovingData); 
		LoadCombCtrl.Set_lsLoadData(m_lsdata);
		LoadCombCtrl.m_bCurvedBridge = m_CurvedBridgeChk.GetCheck();   // JTJ, CJJ俊辑 邦急背
		LoadCombCtrl.m_nSerEpType    = m_SerEpCmb.GetItemData(m_SerEpCmb.GetCurSel());
		LoadCombCtrl.m_nStrEpType    = m_StrEpCmb.GetItemData(m_StrEpCmb.GetCurSel());
		LoadCombCtrl.m_nStrDlType    = m_StrDlCmb.GetItemData(m_StrDlCmb.GetCurSel());
		LoadCombCtrl.m_dCulvertH     = m_wndCulvertH.GetEditValue();
		LoadCombCtrl.m_dCulvertV     = m_wndCulvertV.GetEditValue();

		LoadCombCtrl.m_bIrcServCond       = m_bIrcServCond;
		LoadCombCtrl.m_bIrcConsCond       = m_bIrcConsCond;

		LoadCombCtrl.m_bCSAnalysis = m_wndCSAnalysisChk.GetCheck();
		LoadCombCtrl.m_bLossFactor = m_wndLossFactorChk.GetCheck();    
		LoadCombCtrl.m_dLossFactorTransfer = m_wndLossFactorTransfer.GetEditValue();
		LoadCombCtrl.m_dLossFactorService  = m_wndLossFactorService.GetEditValue();

		LoadCombCtrl.Set_AddEnvelope(m_bAddEnvelope);
		LoadCombCtrl.Set_ConstLoad(m_nConstLoad);

		LoadCombCtrl.m_bJSCE02[0]   = m_bJSCEChk1;
		LoadCombCtrl.m_bJSCE02[1]   = m_bJSCEChk2;
		LoadCombCtrl.m_bJSCE02[2]   = m_bJSCEChk3;
		LoadCombCtrl.m_bJSCE02[3]   = m_bJSCEChk4;
		LoadCombCtrl.m_bJSCE02[4]   = m_bJSCEChk5;
		LoadCombCtrl.m_bJSCE02[5]   = m_bJSCEChk6;
		LoadCombCtrl.m_bJSCE02[6]   = m_bJSCEChk7;

		LoadCombCtrl.m_bJTJD60Capc = m_bJTJD60Capc;
		LoadCombCtrl.m_bJTJD60Gen = m_bJTJD60Gen;
		LoadCombCtrl.m_bJTJD60Acdn = m_bJTJD60Acdn;
		LoadCombCtrl.m_bJTJD60Serv = m_bJTJD60Serv;
		LoadCombCtrl.m_bJTJD60Elst = m_bJTJD60Elst;

		//add by maxiao 2007-04-24
		LoadCombCtrl.m_bTB05Main         =	m_bTB05Main;
		LoadCombCtrl.m_bTB05MainAddition = m_bTB05MainAddition;
		LoadCombCtrl.m_bTB05MainSpecial  =  m_bTB05MainSpecial;
		LoadCombCtrl.m_bTB10002_2017Main         =	m_bTB10002_2017Main;
		LoadCombCtrl.m_bTB10002_2017MainAddition = m_bTB10002_2017MainAddition;
		LoadCombCtrl.m_bTB10002_2017MainSpecial  =  m_bTB10002_2017MainSpecial;
		LoadCombCtrl.m_bCJJ166_2011Capc  = m_bCJJ166_2011Capc;//add by maxiao 2012-03-01
		LoadCombCtrl.m_bCJJ166_2011Gen   = m_bCJJ166_2011Gen ;
		LoadCombCtrl.m_bCJJ166_2011Acdn  = m_bCJJ166_2011Acdn;
		LoadCombCtrl.m_bCJJ166_2011Serv  = m_bCJJ166_2011Serv;
		LoadCombCtrl.m_bCJJ166_2011Elst  = m_bCJJ166_2011Elst;

		// KSCE-LSD15
		if(strCodeName == AutoLoadComb::KSCE_LSD15)
		{
			m_pKoreaLRFD11->GetKoreaLRFD11LoadFactor();
			LoadCombCtrl.m_dKoreaLr11Modifier   = m_pKoreaLRFD11->m_edtLrfd11Modifier.GetEditValue();
			LoadCombCtrl.m_dKoreaLr11Stl        = m_pKoreaLRFD11->m_edtLrfd11Settlement.GetEditValue();
			LoadCombCtrl.m_bKoreaLr11CulvertChk = m_pKoreaLRFD11->m_bLrfd11CulvertChk;    
			LoadCombCtrl.m_nKoreaLr11DcNum      = m_pKoreaLRFD11->m_nKoreaLr11DcNum;
			LoadCombCtrl.m_nKoreaLr11DdNum      = m_pKoreaLRFD11->m_nKoreaLr11DdNum;
			LoadCombCtrl.m_nKoreaLr11DwNum      = m_pKoreaLRFD11->m_nKoreaLr11DwNum;
			LoadCombCtrl.m_nKoreaLr11EhNum      = m_pKoreaLRFD11->m_nKoreaLr11EhNum;
			LoadCombCtrl.m_nKoreaLr11ElNum      = m_pKoreaLRFD11->m_nKoreaLr11ElNum;
			LoadCombCtrl.m_nKoreaLr11EvNum      = m_pKoreaLRFD11->m_nKoreaLr11EvNum;
			LoadCombCtrl.m_nKoreaLr11EsNum      = m_pKoreaLRFD11->m_nKoreaLr11EsNum;
			LoadCombCtrl.m_nKoreaLr11PsNum      = m_pKoreaLRFD11->m_nKoreaLr11PsNum;
			LoadCombCtrl.m_nKoreaLr11CrNum      = m_pKoreaLRFD11->m_nKoreaLr11CrNum;
			LoadCombCtrl.m_nKoreaLr11ShNum      = m_pKoreaLRFD11->m_nKoreaLr11ShNum;
			LoadCombCtrl.m_nKoreaLr11PSCRSHType = m_pKoreaLRFD11->m_nKoreaLr11PSCRSHType;

			LoadCombCtrl.m_dKoreaLr11Dc[0]      = m_pKoreaLRFD11->m_dKoreaLr11Dc1;
			LoadCombCtrl.m_dKoreaLr11Dc[1]      = m_pKoreaLRFD11->m_dKoreaLr11Dc2;
			LoadCombCtrl.m_dKoreaLr11Dd[0]      = m_pKoreaLRFD11->m_dKoreaLr11Dd1;
			LoadCombCtrl.m_dKoreaLr11Dd[1]      = m_pKoreaLRFD11->m_dKoreaLr11Dd2;
			LoadCombCtrl.m_dKoreaLr11Dw[0]      = m_pKoreaLRFD11->m_dKoreaLr11Dw1;
			LoadCombCtrl.m_dKoreaLr11Dw[1]      = m_pKoreaLRFD11->m_dKoreaLr11Dw2;
			LoadCombCtrl.m_dKoreaLr11Eh[0]      = m_pKoreaLRFD11->m_dKoreaLr11Eh1;
			LoadCombCtrl.m_dKoreaLr11Eh[1]      = m_pKoreaLRFD11->m_dKoreaLr11Eh2;
			LoadCombCtrl.m_dKoreaLr11Ev[0]      = m_pKoreaLRFD11->m_dKoreaLr11Ev1;
			LoadCombCtrl.m_dKoreaLr11Ev[1]      = m_pKoreaLRFD11->m_dKoreaLr11Ev2;
			LoadCombCtrl.m_dKoreaLr11Es[0]      = m_pKoreaLRFD11->m_dKoreaLr11Es1;
			LoadCombCtrl.m_dKoreaLr11Es[1]      = m_pKoreaLRFD11->m_dKoreaLr11Es2;
			LoadCombCtrl.m_dKoreaLr11El[0]      = m_pKoreaLRFD11->m_dKoreaLr11El1;
			LoadCombCtrl.m_dKoreaLr11El[1]      = m_pKoreaLRFD11->m_dKoreaLr11El2;
			LoadCombCtrl.m_dKoreaLr11Ps[0]      = m_pKoreaLRFD11->m_dKoreaLr11Ps1;
			LoadCombCtrl.m_dKoreaLr11Ps[1]      = m_pKoreaLRFD11->m_dKoreaLr11Ps2;
			LoadCombCtrl.m_dKoreaLr11Cr[0]      = m_pKoreaLRFD11->m_dKoreaLr11Cr1;
			LoadCombCtrl.m_dKoreaLr11Cr[1]      = m_pKoreaLRFD11->m_dKoreaLr11Cr2;
			LoadCombCtrl.m_dKoreaLr11Sh[0]      = m_pKoreaLRFD11->m_dKoreaLr11Sh1;
			LoadCombCtrl.m_dKoreaLr11Sh[1]      = m_pKoreaLRFD11->m_dKoreaLr11Sh2;
			LoadCombCtrl.m_nKoreaLr11Deform     = m_pKoreaLRFD11->m_nLrfd11Deformation;

			LoadCombCtrl.m_bLrfd11GravityLoadChk= m_pKoreaLRFD11->m_bLrfd11GravityLoadChk;    
			LoadCombCtrl.m_dLrfd11EffLoadFactor = m_pKoreaLRFD11->m_edtLrfd11EffLoadFactor.GetEditValue();    

			// Fatigue ////////////////////////////////////////////////////////////////////////////////////
			CString strName = _T("");
			int Index = m_pKoreaLRFD11->m_cmbLLCase.GetCurSel();

			// Initialize
			LoadCombCtrl.m_nKoreaLr11LL = 0;

			if(Index != -1)
			{
				m_pKoreaLRFD11->m_cmbLLCase.GetLBText(Index, strName);
				LoadCombCtrl.m_nKoreaLr11LL = GetMovingKey(strName);
			}

			LoadCombCtrl.Set_DeadLoadData(m_pKoreaLRFD11->m_aDLData);
			//////////////////////////////////////////////////////////////////////////////////////////////
		}

		LoadCombCtrl.m_bUlti1 = m_bUlti1;
		LoadCombCtrl.m_bUlti2 = m_bUlti2;
		LoadCombCtrl.m_bUlti3 = m_bUlti3;
		LoadCombCtrl.m_bUlti4 = m_bUlti4;
		LoadCombCtrl.m_bUlti5 = m_bUlti5;
		LoadCombCtrl.m_bExtr1 = m_bExtr1;
		LoadCombCtrl.m_bExtr2 = m_bExtr2;
		LoadCombCtrl.m_bServ1 = m_bServ1;
		LoadCombCtrl.m_bServ2 = m_bServ2;
		LoadCombCtrl.m_bServ3 = m_bServ3;
		LoadCombCtrl.m_bServ4 = m_bServ4;
		LoadCombCtrl.m_bServ5 = m_bServ5;
		LoadCombCtrl.m_bFati1 = m_bFati1;

        // AASHTO LRFD02
        if(strCodeName == AutoLoadComb::AASHTOLRFD02)
        {
            m_pAASHTOLRFD02->GetAASHTOLRFD02LoadFactor();    // AASHTO-LRFD98狼 Load Factor甫 备茄促.
            LoadCombCtrl.m_dAshLr98Modifier   = m_pAASHTOLRFD02->m_wndModifier.GetEditValue();
            LoadCombCtrl.m_dAshLr98Stl        = m_pAASHTOLRFD02->m_wndSettlement.GetEditValue();
            LoadCombCtrl.m_bAshLr98CulvertChk = m_pAASHTOLRFD02->m_wndCulvertChk.GetCheck();    
            LoadCombCtrl.m_nAshLr98DcNum      = m_pAASHTOLRFD02->m_nAshLr98DcNum;
            LoadCombCtrl.m_nAshLr98DdNum      = m_pAASHTOLRFD02->m_nAshLr98DdNum;
            LoadCombCtrl.m_nAshLr98DwNum      = m_pAASHTOLRFD02->m_nAshLr98DwNum;
            LoadCombCtrl.m_nAshLr98EhNum      = m_pAASHTOLRFD02->m_nAshLr98EhNum;
            LoadCombCtrl.m_nAshLr98ElNum      = m_pAASHTOLRFD02->m_nAshLr98ElNum;
            LoadCombCtrl.m_nAshLr98EvNum      = m_pAASHTOLRFD02->m_nAshLr98EvNum;
            LoadCombCtrl.m_nAshLr98EsNum      = m_pAASHTOLRFD02->m_nAshLr98EsNum;
            LoadCombCtrl.m_dAshLr98Dc[0]      = m_pAASHTOLRFD02->m_dAshLr98Dc1;
            LoadCombCtrl.m_dAshLr98Dc[1]      = m_pAASHTOLRFD02->m_dAshLr98Dc2;
            LoadCombCtrl.m_dAshLr98Dd[0]      = m_pAASHTOLRFD02->m_dAshLr98Dd1;
            LoadCombCtrl.m_dAshLr98Dd[1]      = m_pAASHTOLRFD02->m_dAshLr98Dd2;
            LoadCombCtrl.m_dAshLr98Dw[0]      = m_pAASHTOLRFD02->m_dAshLr98Dw1;
            LoadCombCtrl.m_dAshLr98Dw[1]      = m_pAASHTOLRFD02->m_dAshLr98Dw2;
            LoadCombCtrl.m_dAshLr98Eh[0]      = m_pAASHTOLRFD02->m_dAshLr98Eh1;
            LoadCombCtrl.m_dAshLr98Eh[1]      = m_pAASHTOLRFD02->m_dAshLr98Eh2;
            LoadCombCtrl.m_dAshLr98Ev[0]      = m_pAASHTOLRFD02->m_dAshLr98Ev1;
            LoadCombCtrl.m_dAshLr98Ev[1]      = m_pAASHTOLRFD02->m_dAshLr98Ev2;
            LoadCombCtrl.m_dAshLr98Es[0]      = m_pAASHTOLRFD02->m_dAshLr98Es1;
            LoadCombCtrl.m_dAshLr98Es[1]      = m_pAASHTOLRFD02->m_dAshLr98Es2;
            LoadCombCtrl.m_dAshLr98El[0]      = m_pAASHTOLRFD02->m_dAshLr98El1;
            LoadCombCtrl.m_dAshLr98El[1]      = m_pAASHTOLRFD02->m_dAshLr98El2;
            LoadCombCtrl.m_nAshLr98Deform     = m_pAASHTOLRFD02->m_nDeformationRdo;
        }

        // AASHTO LRFD08
        if(strCodeName == AutoLoadComb::AASHTOLRFD08)
        {
            m_pAASHTOLRFD08->GetAASHTOLRFD08LoadFactor();    // AASHTO-LRFD08狼 Load Factor甫 备茄促.
            LoadCombCtrl.m_dAshLr08Modifier   = m_pAASHTOLRFD08->m_dLrfd08ModifierEdt.GetEditValue();
            LoadCombCtrl.m_dAshLr08Stl        = m_pAASHTOLRFD08->m_dLrfd08SettlementEdt.GetEditValue();
            LoadCombCtrl.m_bAshLr08CulvertChk = m_pAASHTOLRFD08->m_bLrfd08CulvertChk.GetCheck(); 
            LoadCombCtrl.m_nAshLr08DcNum      = m_pAASHTOLRFD08->m_nAshLr08DcNum;
            LoadCombCtrl.m_nAshLr08DdNum      = m_pAASHTOLRFD08->m_nAshLr08DdNum;
            LoadCombCtrl.m_nAshLr08DwNum      = m_pAASHTOLRFD08->m_nAshLr08DwNum;
            LoadCombCtrl.m_nAshLr08EhNum      = m_pAASHTOLRFD08->m_nAshLr08EhNum;
            LoadCombCtrl.m_nAshLr08ElNum      = m_pAASHTOLRFD08->m_nAshLr08ElNum;
            LoadCombCtrl.m_nAshLr08EvNum      = m_pAASHTOLRFD08->m_nAshLr08EvNum;
            LoadCombCtrl.m_nAshLr08EsNum      = m_pAASHTOLRFD08->m_nAshLr08EsNum;
            LoadCombCtrl.m_nAshLr08PsNum      = m_pAASHTOLRFD08->m_nAshLr08PsNum;
            LoadCombCtrl.m_nAshLr08CrNum      = m_pAASHTOLRFD08->m_nAshLr08CrNum;
            LoadCombCtrl.m_nAshLr08ShNum      = m_pAASHTOLRFD08->m_nAshLr08ShNum;
            LoadCombCtrl.m_dAshLr08Dc[0]      = m_pAASHTOLRFD08->m_dAshLr08Dc1;
            LoadCombCtrl.m_dAshLr08Dc[1]      = m_pAASHTOLRFD08->m_dAshLr08Dc2;
            LoadCombCtrl.m_dAshLr08Dd[0]      = m_pAASHTOLRFD08->m_dAshLr08Dd1;
            LoadCombCtrl.m_dAshLr08Dd[1]      = m_pAASHTOLRFD08->m_dAshLr08Dd2;
            LoadCombCtrl.m_dAshLr08Dw[0]      = m_pAASHTOLRFD08->m_dAshLr08Dw1;
            LoadCombCtrl.m_dAshLr08Dw[1]      = m_pAASHTOLRFD08->m_dAshLr08Dw2;
            LoadCombCtrl.m_dAshLr08Eh[0]      = m_pAASHTOLRFD08->m_dAshLr08Eh1;
            LoadCombCtrl.m_dAshLr08Eh[1]      = m_pAASHTOLRFD08->m_dAshLr08Eh2;
            LoadCombCtrl.m_dAshLr08Ev[0]      = m_pAASHTOLRFD08->m_dAshLr08Ev1;
            LoadCombCtrl.m_dAshLr08Ev[1]      = m_pAASHTOLRFD08->m_dAshLr08Ev2;
            LoadCombCtrl.m_dAshLr08Es[0]      = m_pAASHTOLRFD08->m_dAshLr08Es1;
            LoadCombCtrl.m_dAshLr08Es[1]      = m_pAASHTOLRFD08->m_dAshLr08Es2;
            LoadCombCtrl.m_dAshLr08El[0]      = m_pAASHTOLRFD08->m_dAshLr08El1;
            LoadCombCtrl.m_dAshLr08El[1]      = m_pAASHTOLRFD08->m_dAshLr08El2;
            LoadCombCtrl.m_dAshLr08Ps[0]      = m_pAASHTOLRFD08->m_dAshLr08Ps1;
            LoadCombCtrl.m_dAshLr08Ps[1]      = m_pAASHTOLRFD08->m_dAshLr08Ps2;
            LoadCombCtrl.m_dAshLr08Cr[0]      = m_pAASHTOLRFD08->m_dAshLr08Cr1;
            LoadCombCtrl.m_dAshLr08Cr[1]      = m_pAASHTOLRFD08->m_dAshLr08Cr2;
            LoadCombCtrl.m_dAshLr08Sh[0]      = m_pAASHTOLRFD08->m_dAshLr08Sh1;
            LoadCombCtrl.m_dAshLr08Sh[1]      = m_pAASHTOLRFD08->m_dAshLr08Sh2;
            LoadCombCtrl.m_nAshLr08Deform     = m_pAASHTOLRFD08->m_nLrfd08DeformationRdo;
        }

		// AASHTO LRFD12
		if(strCodeName == AutoLoadComb::AASHTO_LRFD12 || strCodeName==AutoLoadComb::AASHTO_LRFD16 || strCodeName==AutoLoadComb::AASHTO_LRFD17 ||
			strCodeName == AutoLoadComb::AASHTO_LRFD20 || strCodeName == AutoLoadComb::AASHTO_LRFD24)
		{
			m_pAASHTOLRFD12->GetAASHTOLRFD12LoadFactor();
			LoadCombCtrl.m_dAshLr12Modifier   = m_pAASHTOLRFD12->m_edtLrfd12Modifier.GetEditValue();
			LoadCombCtrl.m_dAshLr12Stl        = m_pAASHTOLRFD12->m_edtLrfd12Settlement.GetEditValue();
			LoadCombCtrl.m_bAshLr12CulvertChk = m_pAASHTOLRFD12->m_bLrfd12Culvert;
			LoadCombCtrl.m_nAshLr12DcType     = m_pAASHTOLRFD12->m_nAshLr12DcType;
			LoadCombCtrl.m_nAshLr12DdType     = m_pAASHTOLRFD12->m_nAshLr12DdType;
			LoadCombCtrl.m_nAshLr12DwType     = m_pAASHTOLRFD12->m_nAshLr12DwType;
			LoadCombCtrl.m_nAshLr12EhType     = m_pAASHTOLRFD12->m_nAshLr12EhType;
			LoadCombCtrl.m_nAshLr12ElType     = m_pAASHTOLRFD12->m_nAshLr12ElType;
			LoadCombCtrl.m_nAshLr12EvType     = m_pAASHTOLRFD12->m_nAshLr12EvType;
			LoadCombCtrl.m_nAshLr12EsType     = m_pAASHTOLRFD12->m_nAshLr12EsType;
			LoadCombCtrl.m_nAshLr12PsType     = m_pAASHTOLRFD12->m_nAshLr12PsType;
			LoadCombCtrl.m_nAshLr12CrType     = m_pAASHTOLRFD12->m_nAshLr12CrType;
			LoadCombCtrl.m_nAshLr12ShType     = m_pAASHTOLRFD12->m_nAshLr12ShType;
			LoadCombCtrl.m_dAshLr12Dc[0]      = m_pAASHTOLRFD12->m_dAshLr12Dc1;
			LoadCombCtrl.m_dAshLr12Dc[1]      = m_pAASHTOLRFD12->m_dAshLr12Dc2;
			LoadCombCtrl.m_dAshLr12Dd[0]      = m_pAASHTOLRFD12->m_dAshLr12Dd1;
			LoadCombCtrl.m_dAshLr12Dd[1]      = m_pAASHTOLRFD12->m_dAshLr12Dd2;
			LoadCombCtrl.m_dAshLr12Dw[0]      = m_pAASHTOLRFD12->m_dAshLr12Dw1;
			LoadCombCtrl.m_dAshLr12Dw[1]      = m_pAASHTOLRFD12->m_dAshLr12Dw2;
			LoadCombCtrl.m_dAshLr12Eh[0]      = m_pAASHTOLRFD12->m_dAshLr12Eh1;
			LoadCombCtrl.m_dAshLr12Eh[1]      = m_pAASHTOLRFD12->m_dAshLr12Eh2;
			LoadCombCtrl.m_dAshLr12Ev[0]      = m_pAASHTOLRFD12->m_dAshLr12Ev1;
			LoadCombCtrl.m_dAshLr12Ev[1]      = m_pAASHTOLRFD12->m_dAshLr12Ev2;
			LoadCombCtrl.m_dAshLr12Es[0]      = m_pAASHTOLRFD12->m_dAshLr12Es1;
			LoadCombCtrl.m_dAshLr12Es[1]      = m_pAASHTOLRFD12->m_dAshLr12Es2;
			LoadCombCtrl.m_dAshLr12El[0]      = m_pAASHTOLRFD12->m_dAshLr12El1;
			LoadCombCtrl.m_dAshLr12El[1]      = m_pAASHTOLRFD12->m_dAshLr12El2;
			LoadCombCtrl.m_dAshLr12Ps[0]      = m_pAASHTOLRFD12->m_dAshLr12Ps1;
			LoadCombCtrl.m_dAshLr12Ps[1]      = m_pAASHTOLRFD12->m_dAshLr12Ps2;
			LoadCombCtrl.m_dAshLr12Cr[0]      = m_pAASHTOLRFD12->m_dAshLr12Cr1;
			LoadCombCtrl.m_dAshLr12Cr[1]      = m_pAASHTOLRFD12->m_dAshLr12Cr2;
			LoadCombCtrl.m_dAshLr12Sh[0]      = m_pAASHTOLRFD12->m_dAshLr12Sh1;
			LoadCombCtrl.m_dAshLr12Sh[1]      = m_pAASHTOLRFD12->m_dAshLr12Sh2;
			LoadCombCtrl.m_nAshLr12Deform     = m_pAASHTOLRFD12->m_nLrfd12Deformation;
			LoadCombCtrl.m_dGamma_LL          = m_pAASHTOLRFD12->m_edtLrfd16_GammaLL.GetEditValue();

            if (m_pAASHTOLRFD12->m_wndSeisChk.GetCheck())
            {
                LoadCombCtrl.m_bSeismic = TRUE;
                m_pAASHTOLRFD12->Get_LiveLoadData(LoadCombCtrl.m_aEffectiveMVLoad);
				m_pAASHTOLRFD12->Get_LiveLoadData(LoadCombCtrl.m_aEffectiveLoad);

                LoadCombCtrl.m_nOrthoType = (m_pAASHTOLRFD12->m_wndOthoChk.GetCheck() ? 0 : 1);
                LoadCombCtrl.Set_OrthoCombType(m_pAASHTOLRFD12->m_nOrthoCombType);
                for (int i=0; i<m_pAASHTOLRFD12->m_a100_30_Load.GetSize(); ++i)
                {
                    LoadCombCtrl.m_a100_30_Load.Add(m_pAASHTOLRFD12->m_a100_30_Load[i]);
                }
            }
            else
            {
                LoadCombCtrl.m_bSeismic = FALSE;
                LoadCombCtrl.m_nOrthoType = 0;
            }
		}

		// CSA-S6S1-10
		if(strCodeName == AutoLoadComb::CSA_S6S1_10)
		{
			m_pCSA->GetCSALoadFactor();
			LoadCombCtrl.m_nCSADNum = m_pCSA->m_nCSADNum;
			LoadCombCtrl.m_nCSADWNum = m_pCSA->m_nCSADWNum;
			LoadCombCtrl.m_nCSAEP1Num = m_pCSA->m_nCSAEP1Num;
			LoadCombCtrl.m_nCSAEP2Num = m_pCSA->m_nCSAEP2Num;
			LoadCombCtrl.m_nCSAEP3Num = m_pCSA->m_nCSAEP3Num;
			LoadCombCtrl.m_nCSAEP4Num = m_pCSA->m_nCSAEP4Num;
			LoadCombCtrl.m_nCSAWPNum = m_pCSA->m_nCSAWPNum;
			LoadCombCtrl.m_nCSAPSNum = m_pCSA->m_nCSAPSNum;
			LoadCombCtrl.m_dCSAD[0] = m_pCSA->m_dCSADMax;
			LoadCombCtrl.m_dCSAD[1] = m_pCSA->m_dCSADMin;
			LoadCombCtrl.m_dCSADW[0] = m_pCSA->m_dCSADWMax;
			LoadCombCtrl.m_dCSADW[1] = m_pCSA->m_dCSADWMin;
			LoadCombCtrl.m_dCSAEP1[0] = m_pCSA->m_dCSAEP1Max;
			LoadCombCtrl.m_dCSAEP1[1] = m_pCSA->m_dCSAEP1Min;
			LoadCombCtrl.m_dCSAEP2[0] = m_pCSA->m_dCSAEP2Max;
			LoadCombCtrl.m_dCSAEP2[1] = m_pCSA->m_dCSAEP2Min;
			LoadCombCtrl.m_dCSAEP3[0] = m_pCSA->m_dCSAEP3Max;
			LoadCombCtrl.m_dCSAEP3[1] = m_pCSA->m_dCSAEP3Min;
			LoadCombCtrl.m_dCSAEP4[0] = m_pCSA->m_dCSAEP4Max;
			LoadCombCtrl.m_dCSAEP4[1] = m_pCSA->m_dCSAEP4Min;
			LoadCombCtrl.m_dCSAWP[0] = m_pCSA->m_dCSAWPMax;
			LoadCombCtrl.m_dCSAWP[1] = m_pCSA->m_dCSAWPMin;
			LoadCombCtrl.m_dCSAPS[0] = m_pCSA->m_dCSAPSMax;
			LoadCombCtrl.m_dCSAPS[1] = m_pCSA->m_dCSAPSMin;

			LoadCombCtrl.m_aStldEp1.Copy(m_pCSA->m_aStldEp1);
			LoadCombCtrl.m_aStldEp2.Copy(m_pCSA->m_aStldEp2);
			LoadCombCtrl.m_aStldEp3.Copy(m_pCSA->m_aStldEp3);
			LoadCombCtrl.m_aStldEp4.Copy(m_pCSA->m_aStldEp4);
		}

		if(strCodeName == AutoLoadComb::CSA_S6_14 || strCodeName == AutoLoadComb::CSA_S6_19)
		{
			m_pCSA14->GetCSALoadFactor();
			LoadCombCtrl.m_nCSADNum = m_pCSA14->m_nCSADNum;
			LoadCombCtrl.m_nCSADWNum = m_pCSA14->m_nCSADWNum;
			LoadCombCtrl.m_nCSAEP1Num = m_pCSA14->m_nCSAEP1Num;
			LoadCombCtrl.m_nCSAEP2Num = m_pCSA14->m_nCSAEP2Num;
			LoadCombCtrl.m_nCSAEP3Num = m_pCSA14->m_nCSAEP3Num;
			LoadCombCtrl.m_nCSAEP4Num = m_pCSA14->m_nCSAEP4Num;
			LoadCombCtrl.m_nCSAWPNum = m_pCSA14->m_nCSAWPNum;
			LoadCombCtrl.m_nCSAPSNum = m_pCSA14->m_nCSAPSNum;
			LoadCombCtrl.m_dCSAD[0] = m_pCSA14->m_dCSADMax;
			LoadCombCtrl.m_dCSAD[1] = m_pCSA14->m_dCSADMin;
			LoadCombCtrl.m_dCSADW[0] = m_pCSA14->m_dCSADWMax;
			LoadCombCtrl.m_dCSADW[1] = m_pCSA14->m_dCSADWMin;
			LoadCombCtrl.m_dCSAEP1[0] = m_pCSA14->m_dCSAEP1Max;
			LoadCombCtrl.m_dCSAEP1[1] = m_pCSA14->m_dCSAEP1Min;
			LoadCombCtrl.m_dCSAEP2[0] = m_pCSA14->m_dCSAEP2Max;
			LoadCombCtrl.m_dCSAEP2[1] = m_pCSA14->m_dCSAEP2Min;
			LoadCombCtrl.m_dCSAEP3[0] = m_pCSA14->m_dCSAEP3Max;
			LoadCombCtrl.m_dCSAEP3[1] = m_pCSA14->m_dCSAEP3Min;
			LoadCombCtrl.m_dCSAEP4[0] = m_pCSA14->m_dCSAEP4Max;
			LoadCombCtrl.m_dCSAEP4[1] = m_pCSA14->m_dCSAEP4Min;
			LoadCombCtrl.m_dCSAWP[0] = m_pCSA14->m_dCSAWPMax;
			LoadCombCtrl.m_dCSAWP[1] = m_pCSA14->m_dCSAWPMin;
			LoadCombCtrl.m_dCSAPS[0] = m_pCSA14->m_dCSAPSMax;
			LoadCombCtrl.m_dCSAPS[1] = m_pCSA14->m_dCSAPSMin;

			LoadCombCtrl.m_aStldEp1.Copy(m_pCSA14->m_aStldEp1);
			LoadCombCtrl.m_aStldEp2.Copy(m_pCSA14->m_aStldEp2);
			LoadCombCtrl.m_aStldEp3.Copy(m_pCSA14->m_aStldEp3);
			LoadCombCtrl.m_aStldEp4.Copy(m_pCSA14->m_aStldEp4);
		}

		// SNiP 2.05.03-84*
		if(strCodeName ==AutoLoadComb::SNIP)
		{
			m_pSNiP->GetSP3511LoadFactor();
			LoadCombCtrl.m_nBridgeType     = m_pSNiP->m_nBridgeType;
			LoadCombCtrl.m_nSP3511DNum     = m_pSNiP->m_nSP3511DNum;
			LoadCombCtrl.m_nSP3511LIPNum   = m_pSNiP->m_nSP3511LIPNum;
			LoadCombCtrl.m_nSP3511PLNum    = m_pSNiP->m_nSP3511PLNum;
			LoadCombCtrl.m_nSP3511BLNum    = m_pSNiP->m_nSP3511BLNum;
			LoadCombCtrl.m_nSP3511PSNum    = m_pSNiP->m_nSP3511PSNum;
			LoadCombCtrl.m_nSP3511EPNum    = m_pSNiP->m_nSP3511EPNum;
			LoadCombCtrl.m_nSP3511WPNum    = m_pSNiP->m_nSP3511WPNum;
			LoadCombCtrl.m_nSP3511CRSHNum  = m_pSNiP->m_nSP3511CRSHNum;
			LoadCombCtrl.m_nSP3511STLNum   = m_pSNiP->m_nSP3511STLNum;
			LoadCombCtrl.m_dSP3511D[0]     = m_pSNiP->m_dSP3511DMax;
			LoadCombCtrl.m_dSP3511D[1]     = m_pSNiP->m_dSP3511DMin;
			LoadCombCtrl.m_dSP3511LIP[0]   = m_pSNiP->m_dSP3511LIPMax;
			LoadCombCtrl.m_dSP3511LIP[1]   = m_pSNiP->m_dSP3511LIPMin;
			LoadCombCtrl.m_dSP3511PL[0]    = m_pSNiP->m_dSP3511PLMax;
			LoadCombCtrl.m_dSP3511PL[1]    = m_pSNiP->m_dSP3511PLMin;
			LoadCombCtrl.m_dSP3511BL[0]    = m_pSNiP->m_dSP3511BLMax;
			LoadCombCtrl.m_dSP3511BL[1]    = m_pSNiP->m_dSP3511BLMin;
			LoadCombCtrl.m_dSP3511PS[0]    = m_pSNiP->m_dSP3511PSMax;
			LoadCombCtrl.m_dSP3511PS[1]    = m_pSNiP->m_dSP3511PSMin;
			LoadCombCtrl.m_dSP3511EP[0]    = m_pSNiP->m_dSP3511EPMax;
			LoadCombCtrl.m_dSP3511EP[1]    = m_pSNiP->m_dSP3511EPMin;
			LoadCombCtrl.m_dSP3511WP[0]    = m_pSNiP->m_dSP3511WPMax;
			LoadCombCtrl.m_dSP3511WP[1]    = m_pSNiP->m_dSP3511WPMin;
			LoadCombCtrl.m_dSP3511CRSH[0]  = m_pSNiP->m_dSP3511CRSHMax;
			LoadCombCtrl.m_dSP3511CRSH[1]  = m_pSNiP->m_dSP3511CRSHMin;
			LoadCombCtrl.m_dSP3511STL[0]   = m_pSNiP->m_dSP3511STLMax;
			LoadCombCtrl.m_dSP3511STL[1]   = m_pSNiP->m_dSP3511STLMin;
		}

		// SP 35.13330.2011
		if(strCodeName == AutoLoadComb::SP3511)
		{
			m_pSP3511->GetSP3511LoadFactor();
			LoadCombCtrl.m_nBridgeType     = m_pSP3511->m_nBridgeType;
			LoadCombCtrl.m_nSP3511DNum     = m_pSP3511->m_nSP3511DNum;
			LoadCombCtrl.m_nSP3511LIPNum   = m_pSP3511->m_nSP3511LIPNum;
			LoadCombCtrl.m_nSP3511PLNum    = m_pSP3511->m_nSP3511PLNum;
			LoadCombCtrl.m_nSP3511BLNum    = m_pSP3511->m_nSP3511BLNum;
			LoadCombCtrl.m_nSP3511PSNum    = m_pSP3511->m_nSP3511PSNum;
			LoadCombCtrl.m_nSP3511EPNum    = m_pSP3511->m_nSP3511EPNum;
			LoadCombCtrl.m_nSP3511WPNum    = m_pSP3511->m_nSP3511WPNum;
			LoadCombCtrl.m_nSP3511CRSHNum  = m_pSP3511->m_nSP3511CRSHNum;
			LoadCombCtrl.m_nSP3511STLNum   = m_pSP3511->m_nSP3511STLNum;
			LoadCombCtrl.m_dSP3511D[0]     = m_pSP3511->m_dSP3511DMax;
			LoadCombCtrl.m_dSP3511D[1]     = m_pSP3511->m_dSP3511DMin;
			LoadCombCtrl.m_dSP3511LIP[0]   = m_pSP3511->m_dSP3511LIPMax;
			LoadCombCtrl.m_dSP3511LIP[1]   = m_pSP3511->m_dSP3511LIPMin;
			LoadCombCtrl.m_dSP3511PL[0]    = m_pSP3511->m_dSP3511PLMax;
			LoadCombCtrl.m_dSP3511PL[1]    = m_pSP3511->m_dSP3511PLMin;
			LoadCombCtrl.m_dSP3511BL[0]    = m_pSP3511->m_dSP3511BLMax;
			LoadCombCtrl.m_dSP3511BL[1]    = m_pSP3511->m_dSP3511BLMin;
			LoadCombCtrl.m_dSP3511PS[0]    = m_pSP3511->m_dSP3511PSMax;
			LoadCombCtrl.m_dSP3511PS[1]    = m_pSP3511->m_dSP3511PSMin;
			LoadCombCtrl.m_dSP3511EP[0]    = m_pSP3511->m_dSP3511EPMax;
			LoadCombCtrl.m_dSP3511EP[1]    = m_pSP3511->m_dSP3511EPMin;
			LoadCombCtrl.m_dSP3511WP[0]    = m_pSP3511->m_dSP3511WPMax;
			LoadCombCtrl.m_dSP3511WP[1]    = m_pSP3511->m_dSP3511WPMin;
			LoadCombCtrl.m_dSP3511CRSH[0]  = m_pSP3511->m_dSP3511CRSHMax;
			LoadCombCtrl.m_dSP3511CRSH[1]  = m_pSP3511->m_dSP3511CRSHMin;
			LoadCombCtrl.m_dSP3511STL[0]   = m_pSP3511->m_dSP3511STLMax;
			LoadCombCtrl.m_dSP3511STL[1]   = m_pSP3511->m_dSP3511STLMin;
		}

		if(strCodeName == AutoLoadComb::AS5100)
		{
			m_pAS5100->GetAS5100LoadFactor();
			LoadCombCtrl.Set_MovingLoadData(m_MovingData); 
			LoadCombCtrl.m_nASBridgeType    = m_pAS5100->m_nASBridgeType;
			LoadCombCtrl.m_nAS5100DNum     = m_pAS5100->m_nAS5100DNum;
			LoadCombCtrl.m_nAS5100DWNum		 = m_pAS5100->m_nAS5100DWNum;
			LoadCombCtrl.m_nAS5100SLNum		 = m_pAS5100->m_nAS5100SLNum;
			LoadCombCtrl.m_nAS5100GLNum		 = m_pAS5100->m_nAS5100GLNum;
			LoadCombCtrl.m_dAS5100D[0]     = m_pAS5100->m_dAS5100DMax;
			LoadCombCtrl.m_dAS5100D[1]     = m_pAS5100->m_dAS5100DMin;
			LoadCombCtrl.m_dAS5100DW[0]    = m_pAS5100->m_dAS5100DWMax;
			LoadCombCtrl.m_dAS5100DW[1]    = m_pAS5100->m_dAS5100DWMin;
			LoadCombCtrl.m_dAS5100SL[0]		 = m_pAS5100->m_dAS5100SLMax;
			LoadCombCtrl.m_dAS5100SL[1]    = m_pAS5100->m_dAS5100SLMin;
			LoadCombCtrl.m_dAS5100GL[0]    = m_pAS5100->m_dAS5100GLMax;
			LoadCombCtrl.m_dAS5100GL[1]    = m_pAS5100->m_dAS5100GLMin;

			/*LoadCombCtrl.Set_RTLoadData(m_pAS5100->m_aRTData);*/
		}

		if(strCodeName == _LSX(JTG D62-15) || strCodeName == _LSX(JTG D60-15) || strCodeName == _LSX(CJJ11-2019) || strCodeName == SRCCODE_JTGD60_2015)
		{
			//add by gongxing 2015-10-22 
			LoadCombCtrl.m_bJTG15Capc           = m_bJTG15Capc       ;
			LoadCombCtrl.m_bJTG15Gen            = m_bJTG15Gen        ;
			LoadCombCtrl.m_bJTG15Acdn           = m_bJTG15Acdn       ;
			LoadCombCtrl.m_bJTG15Serv           = m_bJTG15Serv       ;
			LoadCombCtrl.m_bJTG15Elst           = m_bJTG15Elst       ;
			LoadCombCtrl.m_bJTG15_Freq          = m_bJTG15_Freq      ;
			LoadCombCtrl.m_bJTG15_QuasiPerm     = m_bJTG15_QuasiPerm ;
			LoadCombCtrl.m_bJTG15_FreqComb      = m_bJTG15_FreqComb  ;
			LoadCombCtrl.m_bJTG15_QuasiPermComb = m_bJTG15_QuasiPermComb ;
			LoadCombCtrl.m_bJTG15_StandComb     = m_bJTG15_StandComb ;
			LoadCombCtrl.m_bJTG15_Factor        = m_bJTG15_Factor;
			LoadCombCtrl.m_bJTG15FoundationChk  = FALSE;
			if (m_bJTG15FoundationChk && m_nDesignType == 1)
			{
				LoadCombCtrl.m_bJTG15FoundationChk = m_bJTG15FoundationChk;
				LoadCombCtrl.m_bJTG15Capc       = m_bJTG15FdnCommonChk;
				LoadCombCtrl.m_bJTG15Gen        = m_bJTG15FdnCommonBasicChk;
				LoadCombCtrl.m_bJTG15Acdn       = m_bJTG15FdnCommonAccChk;
				LoadCombCtrl.m_bJTG15_Freq      = m_bJTG15FdnCommonFreqChk;
				LoadCombCtrl.m_bJTG15_QuasiPerm = m_bJTG15FdnCommonPermChk;
			}
			if (m_bJTG15_Factor)
			{
				CString csFactorAdjust;
				((CEdit*)GetDlgItem(IDC_CMD_LOADCOM_AUTO_FACTOR_EDT))->GetWindowText(csFactorAdjust) ;
				LoadCombCtrl.m_dJTG15_Factor = _ttof(csFactorAdjust);
			} 
			else
			{
				LoadCombCtrl.m_dJTG15_Factor = 1.0 ;
			}

			LoadCombCtrl.m_bJTG15_PrestressLoss = FALSE;
			LoadCombCtrl.m_nEC4LVarTraffType    = 0;
		}

		if(strCodeName == _LS(IDS_DB_LCOM_CODE_ENGNEERINGSTRUCTURECOMMONCODE_GB2019))
		{
			//add by gongxing 2020-03-17 
			LoadCombCtrl.m_tLcomESCGB19.bGB19Capc           = m_tLcomESCGB19.bGB19Capc           ;
			LoadCombCtrl.m_tLcomESCGB19.bGB19Gen            = m_tLcomESCGB19.bGB19Gen            ;
			LoadCombCtrl.m_tLcomESCGB19.bGB19Acdn           = m_tLcomESCGB19.bGB19Acdn           ;
			LoadCombCtrl.m_tLcomESCGB19.bGB19Serv           = m_tLcomESCGB19.bGB19Serv           ;
			LoadCombCtrl.m_tLcomESCGB19.bGB19Elst           = m_tLcomESCGB19.bGB19Elst           ;
			LoadCombCtrl.m_tLcomESCGB19.bGB19_Freq          = m_tLcomESCGB19.bGB19_Freq          ;
			LoadCombCtrl.m_tLcomESCGB19.bGB19_QuasiPerm     = m_tLcomESCGB19.bGB19_QuasiPerm     ;
			LoadCombCtrl.m_tLcomESCGB19.bGB19_FreqComb      = m_tLcomESCGB19.bGB19_FreqComb      ;
			LoadCombCtrl.m_tLcomESCGB19.bGB19_QuasiPermComb = m_tLcomESCGB19.bGB19_QuasiPermComb ;
			LoadCombCtrl.m_tLcomESCGB19.bGB19_StandComb     = m_tLcomESCGB19.bGB19_StandComb     ;
			LoadCombCtrl.m_tLcomESCGB19.bGB19_Factor        = m_tLcomESCGB19.bGB19_Factor        ;
			if (m_tLcomESCGB19.bGB19_Factor)
			{
				CString csFactorAdjust;
				((CEdit*)GetDlgItem(IDC_CMD_LOADCOM_CVL_GB19_FACTOR_EDT))->GetWindowText(csFactorAdjust) ;
				LoadCombCtrl.m_tLcomESCGB19.dGB19_Factor = _ttof(csFactorAdjust);
			} 
			else
			{
				LoadCombCtrl.m_tLcomESCGB19.dGB19_Factor = 1.0 ;
			}

			LoadCombCtrl.m_tLcomESCGB19.bGB19_PrestressLoss = FALSE;
			LoadCombCtrl.m_nEC4LVarTraffType    = 0;
		}

		if (strCodeName == AutoLoadComb::KSCEUSD10)
		{
			LoadCombCtrl.m_nKoreaLr11DcNum = 1;
			LoadCombCtrl.m_nKoreaLr11DwNum = 1;
			LoadCombCtrl.m_dKoreaLr11Dc[0] = 1.0;
			LoadCombCtrl.m_dKoreaLr11Dc[1] = 1.0;
			LoadCombCtrl.m_dKoreaLr11Dw[0] = 1.0;
			LoadCombCtrl.m_dKoreaLr11Dw[1] = 1.0;
			LoadCombCtrl.m_bLrfd11GravityLoadChk= m_pKSCEUSD10->m_bKSCEGravityLoadChk;    
			LoadCombCtrl.m_dLrfd11EffLoadFactor = m_pKSCEUSD10->m_edtKSCEEffLoadFactor.GetEditValue(); 
		}
		if (strCodeName == AutoLoadComb::IRS)
		{
			LoadCombCtrl.m_dIRSUlt  = m_pIRS->m_dIRSULT.GetEditValue();
			LoadCombCtrl.m_dIRSServ = m_pIRS->m_dIRSSRV.GetEditValue();
			LoadCombCtrl.m_bIRSCons_crp_shr_DT_ULS = m_pIRS->m_bChkCrp;

            if ( m_pIRS->m_aIRSUlt.GetSize()==0 && m_pIRS->m_aIRSServ.GetSize()==0 )
            {
                CArray<T_STLD_K, T_STLD_K> arKeyList;
                m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
                LoadCombCtrl.m_aIRSUlt.Copy(arKeyList);
            }
            else
            {
                LoadCombCtrl.m_aIRSUlt.Copy(m_pIRS->m_aIRSUlt);
                LoadCombCtrl.m_aIRSServ.Copy(m_pIRS->m_aIRSServ);
            }
		}

        if(strCodeName == AutoLoadComb::BD21)
        {
            m_pBD21->GetBD21LoadFactor();
            LoadCombCtrl.Set_MovingLoadData(m_MovingData); 
            LoadCombCtrl.m_nBD21BridgeType   = m_pBD21->m_nBD21BridgeType;
            LoadCombCtrl.m_nBD21DNum         = m_pBD21->m_nBD21DNum;
            LoadCombCtrl.m_nBD21DWNum	     = m_pBD21->m_nBD21DWNum;
            LoadCombCtrl.m_nBD21DCNum	     = m_pBD21->m_nBD21DCNum;
            LoadCombCtrl.m_nBD21EVNum	     = m_pBD21->m_nBD21EVNum;
            LoadCombCtrl.m_nBD21EHNum        = m_pBD21->m_nBD21EHNum;
            LoadCombCtrl.m_dBD21D[0]         = m_pBD21->m_dBD21DMax;
            LoadCombCtrl.m_dBD21D[1]         = m_pBD21->m_dBD21DMin;
            LoadCombCtrl.m_dBD21DW[0]        = m_pBD21->m_dBD21DWMax;
            LoadCombCtrl.m_dBD21DW[1]	     = m_pBD21->m_dBD21DWMin;
            LoadCombCtrl.m_dBD21DC[0]        = m_pBD21->m_dBD21DCMax;
            LoadCombCtrl.m_dBD21DC[1]        = m_pBD21->m_dBD21DCMin;
            LoadCombCtrl.m_dBD21EV[0]        = m_pBD21->m_dBD21EVMax;
            LoadCombCtrl.m_dBD21EV[1]        = m_pBD21->m_dBD21EVMin;
            LoadCombCtrl.m_dBD21EH[0]        = m_pBD21->m_dBD21EHMax;
            LoadCombCtrl.m_dBD21EH[1]	     = m_pBD21->m_dBD21EHMin;
            LoadCombCtrl.m_dBD21GF3ULS	     = m_pBD21->m_dBD21GF3ULS;
            LoadCombCtrl.m_dBD21GF3SLS	     = m_pBD21->m_dBD21GF3SLS;
        }

        if (strCodeName == AutoLoadComb::PNS10030)
        {
            m_pPNS10030->GetPNS10030LoadFactor();
            LoadCombCtrl.m_nPNS10030DNum        = m_pPNS10030->m_nPNS10030DNum;
            LoadCombCtrl.m_nPNS10030DCNum       = m_pPNS10030->m_nPNS10030DcNum;
            LoadCombCtrl.m_nPNS10030DWNum       = m_pPNS10030->m_nPNS10030DwNum;
            LoadCombCtrl.m_nPNS10030PSNum       = m_pPNS10030->m_nPNS10030PsNum;
            LoadCombCtrl.m_nPNS10030WPNum       = m_pPNS10030->m_nPNS10030WpNum;
            LoadCombCtrl.m_bPNS10030BaseType    = m_pPNS10030->m_bPNS10030BaseChk;
            LoadCombCtrl.m_bPNS10030AddType     = m_pPNS10030->m_bPNS10030DAddChk;
            LoadCombCtrl.m_bPNS10030UniqType    = m_pPNS10030->m_bPNS10030DUniqChk;
            LoadCombCtrl.m_dPNS10030D[0]        = m_pPNS10030->m_dPNS10030DMax;
            LoadCombCtrl.m_dPNS10030D[1]        = m_pPNS10030->m_dPNS10030DMin;
            LoadCombCtrl.m_dPNS10030DW[0]       = m_pPNS10030->m_dPNS10030DwMax;
            LoadCombCtrl.m_dPNS10030DW[1]       = m_pPNS10030->m_dPNS10030DwMin;
            LoadCombCtrl.m_dPNS10030DC[0]       = m_pPNS10030->m_dPNS10030DcMax;
            LoadCombCtrl.m_dPNS10030DC[1]       = m_pPNS10030->m_dPNS10030DcMin;
            LoadCombCtrl.m_dPNS10030PS[0]       = m_pPNS10030->m_dPNS10030PsMax;
            LoadCombCtrl.m_dPNS10030PS[1]       = m_pPNS10030->m_dPNS10030PsMin;
            LoadCombCtrl.m_dPNS10030WP[0]       = m_pPNS10030->m_dPNS10030WpMax;
            LoadCombCtrl.m_dPNS10030WP[1]       = m_pPNS10030->m_dPNS10030WpMin;
        }

        if (strCodeName == AutoLoadComb::EURO0)
        {
            m_pEURO0->GetEuroLoadFactor();
            LoadCombCtrl.m_nEC4Permanent        = m_pEURO0->m_nEC4Permanent;
            LoadCombCtrl.m_nEC4Prestress        = m_pEURO0->m_nEC4Prestress;
            LoadCombCtrl.m_nEC4Settlement       = m_pEURO0->m_nEC4Settlement;
            LoadCombCtrl.m_bEC4LVarTraff        = m_pEURO0->m_bEC4LVarTraff;
            LoadCombCtrl.m_bEC4LVarWind         = m_pEURO0->m_bEC4LVarWind;
            LoadCombCtrl.m_bEC4LVarTherm        = m_pEURO0->m_bEC4LVarTherm;
            LoadCombCtrl.m_bEC4LVarSnow         = m_pEURO0->m_bEC4LVarSnow;
            LoadCombCtrl.m_bEC4LVarConst        = m_pEURO0->m_bEC4LVarConst;
            LoadCombCtrl.m_nEC4LVarTraffType    = m_pEURO0->m_nEC4LVarTraffType;

            if ( m_pEURO0->m_aMvldChar.GetSize()==0 && m_pEURO0->m_aMvldFreq.GetSize()==0 )
            {
                CArray<T_MVLD_K, T_MVLD_K> arKeyList;
                m_pDoc->m_pAttrCtrl->GetMvldKeyList(arKeyList);
                LoadCombCtrl.m_aMvldChar.Copy(arKeyList);
            }
            else
            {
                LoadCombCtrl.m_aMvldChar.Copy(m_pEURO0->m_aMvldChar);
                LoadCombCtrl.m_aMvldFreq.Copy(m_pEURO0->m_aMvldFreq);
            }
        }

        if (strCodeName == AutoLoadComb::IRC6LSD)
        {
            LoadCombCtrl.m_bIRC6_LSDLVarTraff     = m_pIRC6LSD->m_bIRC6_LSDLVarTraff;
            LoadCombCtrl.m_bIRC6_LSDLVarWind      = m_pIRC6LSD->m_bIRC6_LSDLVarWind;
            LoadCombCtrl.m_bIRC6_LSDLVarTherm     = m_pIRC6LSD->m_bIRC6_LSDLVarTherm;
            LoadCombCtrl.m_bIRC6_LSDLVarSnow      = m_pIRC6LSD->m_bIRC6_LSDLVarSnow;
            LoadCombCtrl.m_bIRC6_LSDLVarConst     = m_pIRC6LSD->m_bIRC6_LSDLVarConst;
            LoadCombCtrl.m_bIRC6_IRC1122020       = m_pIRC6LSD->m_bIRC6_IRC1122020;
            LoadCombCtrl.m_nIRC6_LSDLVarTraffType = m_pIRC6LSD->m_nIRC6LVarTraffType;

            if ( m_pIRC6LSD->m_aMvldChar.GetSize()==0 && m_pIRC6LSD->m_aMvldFreq.GetSize()==0 )
            {
                CArray<T_MVLD_K, T_MVLD_K> arKeyList;
                m_pDoc->m_pAttrCtrl->GetMvldKeyList(arKeyList);
                LoadCombCtrl.m_aMvldChar.Copy(arKeyList);
            }
            else
            {
                LoadCombCtrl.m_aMvldChar.Copy(m_pIRC6LSD->m_aMvldChar);
                LoadCombCtrl.m_aMvldFreq.Copy(m_pIRC6LSD->m_aMvldFreq);
            }
            //for IRC6 Special Vehicle Amendments
            if ( m_pIRC6LSD->m_aMvldSPV.GetSize()==0 && m_pIRC6LSD->m_aMvldSPVSel.GetSize()==0 )
            {
                CArray<T_MVLDid_K, T_MVLDid_K> arKeyList;
                m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arKeyList);
                LoadCombCtrl.m_aMvldSPV.Copy(arKeyList);
            }
            else
            {
                LoadCombCtrl.m_aMvldSPV.Copy(m_pIRC6LSD->m_aMvldSPV);
                LoadCombCtrl.m_aMvldSPVSel.Copy(m_pIRC6LSD->m_aMvldSPVSel);
            }
        }

		if (strCodeName == _LSX(TMH07-1981))
		{
			m_pTMH7->GetBD21LoadFactor();
			LoadCombCtrl.Set_MovingLoadData(m_MovingData);
			LoadCombCtrl.Set_MVLDData(m_pTMH7->m_aMvldType);
			LoadCombCtrl.Set_BrakingTractionData(m_pTMH7->m_aLongiBrakTra);
			//LoadCombCtrl.Set_BrakingTractionData(m_pTMH7->m_lstEffLoad);
			LoadCombCtrl.m_nBD21BridgeType = m_pTMH7->m_nBD21BridgeType;
			LoadCombCtrl.m_nBD21DNum = m_pTMH7->m_nBD21DNum;
			LoadCombCtrl.m_nBD21DWNum = m_pTMH7->m_nBD21DWNum;
			LoadCombCtrl.m_nBD21DCNum = m_pTMH7->m_nBD21DCNum;
			LoadCombCtrl.m_nBD21EVNum = m_pTMH7->m_nBD21EVNum;
			LoadCombCtrl.m_nBD21EHNum = m_pTMH7->m_nBD21EHNum;
			/*LoadCombCtrl.m_nBD21ESNum = m_pTMH7->m_nBD21ESMax;*/
			LoadCombCtrl.m_dBD21D[0] = 1.2;// m_pTMH7->m_dBD21DMax;
			LoadCombCtrl.m_dBD21D[1] = 1.05;// m_pTMH7->m_dBD21DMin;
			LoadCombCtrl.m_dBD21DW[0] = 1.2;// m_pTMH7->m_dBD21DWMax;
			LoadCombCtrl.m_dBD21DW[1] = 1.05;// m_pTMH7->m_dBD21DWMin;
			LoadCombCtrl.m_dBD21DC[0] = 1.2;// m_pTMH7->m_dBD21DCMax;
			LoadCombCtrl.m_dBD21DC[1] = 1.05;// m_pTMH7->m_dBD21DCMin;
			LoadCombCtrl.m_dBD21EV[0] = 1.5;// m_pTMH7->m_dBD21EVMax;
			LoadCombCtrl.m_dBD21EV[1] = 0.;// m_pTMH7->m_dBD21EVMin;
			LoadCombCtrl.m_dBD21EH[0] = 1.5;// m_pTMH7->m_dBD21EHMax;
			LoadCombCtrl.m_dBD21EH[1] = 0.;// m_pTMH7->m_dBD21EHMin;
			LoadCombCtrl.m_dBD21ES[0] = 1.4;
			LoadCombCtrl.m_dBD21ES[1] = 1.2;
			LoadCombCtrl.m_dBD21GF3ULS = m_pTMH7->m_dBD21GF3ULS;
			LoadCombCtrl.m_dBD21GF3SLS = m_pTMH7->m_dBD21GF3SLS;


			LoadCombCtrl.m_nCodeBasedOrUserDef = m_pTMH7->m_nCodeBasedOrUserDef;
			LoadCombCtrl.m_nCodeBasedVerticalEarthPressure = m_pTMH7->m_nCodeBasedVerticalEarthPressure;
			LoadCombCtrl.m_nCodeBasedNonVerticalEarthPressure = m_pTMH7->m_nCodeBasedNonVerticalEarthPressure;
			LoadCombCtrl.m_dUserDefDL = m_pTMH7->m_dUserDefDL;
			LoadCombCtrl.m_dUserDefDL_Max = m_pTMH7->m_dUserDefDL_Max;
			LoadCombCtrl.m_dUserDefDL_Min = m_pTMH7->m_dUserDefDL_Min;
			LoadCombCtrl.m_dUserDefSIDL_Max = m_pTMH7->m_dUserDefSIDL_Max;
			LoadCombCtrl.m_dUserDefSIDL_Min = m_pTMH7->m_dUserDefSIDL_Min; 
			LoadCombCtrl.m_dEPRelieving = m_pTMH7->m_dEPRelieving;
			LoadCombCtrl.m_dUserRdo_Dmax = m_pTMH7->m_dUserRdo_Dmax;
			LoadCombCtrl.m_dUserRdo_Dmin = m_pTMH7->m_dUserRdo_Dmin;
			LoadCombCtrl.m_dUserRdo_Dboth = m_pTMH7->m_dUserRdo_Dboth;
			LoadCombCtrl.m_dUserRdo_SIDLmax = m_pTMH7->m_dUserRdo_SIDLmax;
			LoadCombCtrl.m_dUserRdo_SIDLmin = m_pTMH7->m_dUserRdo_SIDLmin;
			LoadCombCtrl.m_dUserRdo_SIDLboth = m_pTMH7->m_dUserRdo_SIDLboth;

			if (m_pTMH7->m_wndOthoChk.GetCheck())
			{
				LoadCombCtrl.m_bSeismic = TRUE;

				LoadCombCtrl.m_nOrthoType = (m_pTMH7->m_wndOthoChk.GetCheck() ? 0 : 1);
				LoadCombCtrl.Set_OrthoCombType(m_pTMH7->m_nOrthoCombType);
				for (int i = 0; i < m_pTMH7->m_a100_30_Load.GetSize(); ++i)
				{
					LoadCombCtrl.m_a100_30_Load.Add(m_pTMH7->m_a100_30_Load[i]);
				}
			}
			else
			{
				LoadCombCtrl.m_bSeismic = FALSE;
				LoadCombCtrl.m_nOrthoType = 0;
			}

			//LoadCombCtrl.m_a.Copy(m_pIRC6LSD->m_aMvldChar);
			//LoadCombCtrl.m_aTMH7BrackingTractiveData = m_pTMH7->m_aLongiBrakTra;
		}
		BOOL bOK = LoadCombCtrl.Set_DefaultLoadComb(m_nOption, strCodeName);

		m_MovingData.RemoveAll();

		if (bOK)
		{
			if (strCodeName == _LSX(IRC:6 LSD))
			{
				IUsageCounter::Use(_T("LCOMIRC"));
			}

			CDialog::OnOK();
		}
	}
	else	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Generation_code_unavailable_),MB_OK);
}

void CCmdAutoLoadCombCvlDlg::OnCancel() 
{
	if(m_pKoreaLRFD11)  { delete m_pKoreaLRFD11;  m_pKoreaLRFD11  = NULL; }
	if(m_pAASHTOLRFD12) { delete m_pAASHTOLRFD12; m_pAASHTOLRFD12 = NULL; }
	if(m_pCSA)          { delete m_pCSA;    m_pCSA    = NULL; }
	if(m_pCSA14)        { delete m_pCSA14;  m_pCSA14  = NULL; }
	if(m_pSNiP)         { delete m_pSNiP;   m_pSNiP   = NULL; }
	if(m_pSP3511)       { delete m_pSP3511; m_pSP3511 = NULL; }
	if(m_pAS5100)       { delete m_pAS5100; m_pAS5100 = NULL; }
    if(m_pBD21)         { delete m_pBD21;   m_pBD21   = NULL; }
    if(m_pPNS10030)     { delete m_pPNS10030;   m_pPNS10030 = NULL; }
    if(m_pEURO0)        { delete m_pEURO0; m_pEURO0 = NULL; }
    if(m_pIRC6LSD)      { delete m_pIRC6LSD; m_pIRC6LSD = NULL; }
    if(m_pAASHTOLRFD02) { delete m_pAASHTOLRFD02; m_pAASHTOLRFD02 = NULL; }
    if(m_pAASHTOLRFD08) { delete m_pAASHTOLRFD08; m_pAASHTOLRFD08 = NULL; }
    if(m_pIRS)          { delete m_pIRS; m_pIRS = NULL; }
	CDialog::OnCancel();
}

BOOL CCmdAutoLoadCombCvlDlg::ErrorCheckPS() 
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

void CCmdAutoLoadCombCvlDlg::OnCmdLossFactorBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_DGN_CON_LOSSFACTOR,0));
	T_DCON_D DconData;
	DconData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcon(DconData);
	m_wndLossFactorTransfer.SetEditUnit(DconData.dTranLossFactor);
	m_wndLossFactorService.SetEditUnit(DconData.dServLossFactor);  
}

void CCmdAutoLoadCombCvlDlg::OnCmdConstStageRdo()
{
	CtrlManager();

    CString strCodeName=_T("");
    int Index = m_Code.GetCurSel();
    if (Index != -1)	m_Code.GetLBText(Index, strCodeName);

#if defined(_RUS)
		CInitCtrl::ConvertRusEngDesignCode(strCodeName);
#endif

    if (strCodeName == AutoLoadComb::KSCE_LSD15)
    {
        m_pKoreaLRFD11->m_nConstLoad = m_nConstLoad;
        m_pKoreaLRFD11->OnCmdKoreaLRFD11EnableDisable();
    }
    else if (strCodeName == AutoLoadComb::AASHTO_LRFD12 || strCodeName == AutoLoadComb::AASHTO_LRFD16 || strCodeName == AutoLoadComb::AASHTO_LRFD17 ||
		strCodeName == AutoLoadComb::AASHTO_LRFD20 || strCodeName == AutoLoadComb::AASHTO_LRFD24)
    {
        m_pAASHTOLRFD12->m_nConstLoad = m_nConstLoad;
        m_pAASHTOLRFD12->OnCmdAASHTOLRFDEnableDisable(FALSE);
    }
    else if (strCodeName ==AutoLoadComb::SNIP)
    {
        m_pSNiP->m_nConstLoad = m_nConstLoad;
        m_pSNiP->OnCmdSP3511EnableDisable();
    }
    else if (strCodeName == AutoLoadComb::SP3511)
    {
        m_pSP3511->m_nConstLoad = m_nConstLoad;
        m_pSP3511->OnCmdSP3511EnableDisable();
    }
    else if (strCodeName == AutoLoadComb::PNS10030)
    {
        m_pPNS10030->m_nConstLoad = m_nConstLoad;
        m_pPNS10030->OnCmdPNS10030EnableDisable();
    }
    else if (strCodeName == AutoLoadComb::AS5100)
    {
        m_pAS5100->m_nConstLoad = m_nConstLoad;
        m_pAS5100->OnCmdAS5100EnableDisable();
    }
    else if (strCodeName == AutoLoadComb::KSCEUSD10)
    {
        m_pKSCEUSD10->OnCmdKSCEUSD10EnableDisable();
    }
    else if (strCodeName == AutoLoadComb::BD21)
    {
        m_pBD21->m_nConstLoad = m_nConstLoad;
        m_pBD21->OnCmdBD21EnableDisable();
    }
    else if (strCodeName == AutoLoadComb::EURO0)
    {
        m_pEURO0->m_nConstLoad = m_nConstLoad;
        m_pEURO0->OnCmdEURO0EnableDisable();
    }
    else if (strCodeName == AutoLoadComb::IRC6LSD)
    {
        m_pIRC6LSD->m_nConstLoad = m_nConstLoad;
        m_pIRC6LSD->OnCmdIRC6LSDEnableDisable();
    }
    else if (strCodeName == AutoLoadComb::AASHTOLRFD02)
    {
        m_pAASHTOLRFD02->m_nConstLoad = m_nConstLoad;
        m_pAASHTOLRFD02->OnCmdAASHTOLRFD02EnableDisable();
    }
    else if (strCodeName == AutoLoadComb::AASHTOLRFD08)
    {
        m_pAASHTOLRFD08->m_nConstLoad = m_nConstLoad;
        m_pAASHTOLRFD08->OnCmdAASHTOLRFD08EnableDisable();
    }
    else if (strCodeName == AutoLoadComb::IRS)
    {
        m_pIRS->m_nConstLoad = m_nConstLoad;
        m_pIRS->OnCmdIRSEnableDisable();
    }
    else if ( strCodeName == AutoLoadComb::CSA_S6S1_10 )
    {
        m_pCSA->m_nConstLoad = m_nConstLoad;
        m_pCSA->OnCmdCSAEnableDisable();
    }
    else if ( strCodeName == AutoLoadComb::CSA_S6_14 || strCodeName == AutoLoadComb::CSA_S6_19 )
    {
        m_pCSA14->m_nConstLoad = m_nConstLoad;
        m_pCSA14->OnCmdCSAEnableDisable();
    }
	else if (strCodeName == AutoLoadComb::TMH7)
	{
		m_pTMH7->m_nConstLoad = m_nConstLoad;
		m_pTMH7->OnCmdBD21EnableDisable();
	}
}

void CCmdAutoLoadCombCvlDlg::OnCmdJTJD60CapcChk()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_JTJD60_GEN_CHK) ->EnableWindow(m_bJTJD60Capc);
	GetDlgItem(IDC_JTJD60_ACDN_CHK)->EnableWindow(m_bJTJD60Capc);
}

void CCmdAutoLoadCombCvlDlg::OnCmdCJJ166_2011CapcChk()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CJJ166_2011_GEN_CHK) ->EnableWindow(m_bCJJ166_2011Capc);
	GetDlgItem(IDC_CJJ166_2011_ACDN_CHK)->EnableWindow(m_bCJJ166_2011Capc);
}

LRESULT CCmdAutoLoadCombCvlDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	AlignControl();
	return 0L;
}

void CCmdAutoLoadCombCvlDlg::SetLoadCombSelData()
{
	if(m_nDesignType == 0) // Steel
	{
		m_bUlti1 = TRUE;
		m_bUlti2 = TRUE;
		m_bUlti3 = TRUE;
		m_bUlti4 = TRUE;
		m_bUlti5 = TRUE;
		m_bExtr1 = FALSE;
		m_bExtr2 = FALSE;
		m_bServ1 = FALSE;
		m_bServ2 = TRUE;
		m_bServ3 = FALSE;
		m_bServ4 = FALSE;
		m_bServ5 = FALSE;
		m_bFati1 = TRUE;
	}
	else if(m_nDesignType == 1) // Concrete
	{
		m_bUlti1 = TRUE;
		m_bUlti2 = TRUE;
		m_bUlti3 = TRUE;
		m_bUlti4 = TRUE;
		m_bUlti5 = TRUE;
		m_bExtr1 = FALSE;
		m_bExtr2 = FALSE;
		m_bServ1 = TRUE;
		m_bServ2 = FALSE;
		m_bServ3 = TRUE;
		m_bServ4 = TRUE;
		m_bServ5 = TRUE;
		m_bFati1 = FALSE;
	}
	else if(m_nDesignType == 3) // Steel Composite
	{
		m_bUlti1 = TRUE;
		m_bUlti2 = TRUE;
		m_bUlti3 = TRUE;
		m_bUlti4 = TRUE;
		m_bUlti5 = TRUE;
		m_bExtr1 = FALSE;
		m_bExtr2 = FALSE;
		m_bServ1 = FALSE;
		m_bServ2 = TRUE;
		m_bServ3 = FALSE;
		m_bServ4 = FALSE;
		m_bServ5 = FALSE;
		m_bFati1 = TRUE;
	}
	else
	{
		// Nothing
	}
}	

//////////////////////////////////////////////////////////////////////////
//add by dongqiufeng 2015-10-16

void CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15FoundationChk(void)
{
	UpdateData(TRUE);
	if (m_nDesignType == 1)//concrete
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlJTG15_Sup, !m_bJTG15FoundationChk);
		CDlgUtil::CtrlShowHide(this, m_aCtrlJTG15_Sub, m_bJTG15FoundationChk);
	}

}
void CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15FoundationCommonChk(void)
{
	UpdateData(TRUE);
	BOOL bEnable = m_bJTG15FoundationChk && m_bJTG15FdnCommonChk;
	((CButton*)GetDlgItem(IDC_CHK_JTG15_FOUNDATION_COMMON_BASIC))->SetCheck(bEnable);
	((CButton*)GetDlgItem(IDC_CHK_JTG15_FOUNDATION_COMMON_ACC))->SetCheck(bEnable);
	((CButton*)GetDlgItem(IDC_CHK_JTG15_FOUNDATION_COMMON_FREQ))->SetCheck(bEnable);
	((CButton*)GetDlgItem(IDC_CHK_JTG15_FOUNDATION_COMMON_PERM))->SetCheck(bEnable);
}

void CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15CapcChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_JTG15_GEN_CHK) ->EnableWindow(m_bJTG15Capc);
	GetDlgItem(IDC_JTG15_ACDN_CHK)->EnableWindow(m_bJTG15Capc);
	GetDlgItem(IDC_JTG15_FREQ_CHK) ->EnableWindow(m_bJTG15Capc);
	GetDlgItem(IDC_JTG15_QUASIPERM_CHK)->EnableWindow(m_bJTG15Capc);
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15AcdnChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_JTG15_FREQ_CHK) ->EnableWindow(m_bJTG15Acdn);
	GetDlgItem(IDC_JTG15_QUASIPERM_CHK)->EnableWindow(m_bJTG15Acdn);
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15ServChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_JTG15_FREQCOMB_CHK) ->EnableWindow(m_bJTG15Serv);
	GetDlgItem(IDC_JTG15_QUASIPERMCOMB_CHK)->EnableWindow(m_bJTG15Serv);
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15ElstChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_JTG15_STANDCOMB_CHK)->EnableWindow(m_bJTG15Elst);
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15FreqChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (!m_nDesignType == 0)
	{
	((CButton*)GetDlgItem(IDC_JTG15_QUASIPERM_CHK))->SetCheck(!m_bJTG15_Freq);
}
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedJtg15QuasipermChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (!m_nDesignType == 0)
	{
	((CButton*)GetDlgItem(IDC_JTG15_FREQ_CHK))->SetCheck(!m_bJTG15_QuasiPerm);
}
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedCmdLoadcomAutoServiceLife()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_LOADCOM_AUTO_FACTOR_EDT)->EnableWindow(m_bJTG15_Factor);
	if (!m_bJTG15_Factor)
	{
		GetDlgItem(IDC_CMD_LOADCOM_AUTO_FACTOR_EDT)->SetWindowText(_T("1.0"));//add by donqiufeng 2015-10-16
	}
}

void CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19CapcChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_CVL_GB19_GEN_CHK) ->EnableWindow(m_tLcomESCGB19.bGB19Capc);
	GetDlgItem(IDC_CVL_GB19_ACDN_CHK)->EnableWindow(m_tLcomESCGB19.bGB19Capc);
	GetDlgItem(IDC_CVL_GB19_FREQ_CHK) ->EnableWindow(m_tLcomESCGB19.bGB19Capc);
	GetDlgItem(IDC_CVL_GB19_QUASIPERM_CHK)->EnableWindow(m_tLcomESCGB19.bGB19Capc);
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19AcdnChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_CVL_GB19_FREQ_CHK) ->EnableWindow(m_tLcomESCGB19.bGB19Acdn);
	GetDlgItem(IDC_CVL_GB19_QUASIPERM_CHK)->EnableWindow(m_tLcomESCGB19.bGB19Acdn);
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19ServChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_CVL_GB19_FREQCOMB_CHK) ->EnableWindow(m_tLcomESCGB19.bGB19Serv);
	GetDlgItem(IDC_CVL_GB19_QUASIPERMCOMB_CHK)->EnableWindow(m_tLcomESCGB19.bGB19Serv);
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19ElstChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_CVL_GB19_STANDCOMB_CHK)->EnableWindow(m_tLcomESCGB19.bGB19Elst);
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19FreqChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (!m_nDesignType == 0)
	{
		((CButton*)GetDlgItem(IDC_CVL_GB19_QUASIPERM_CHK))->SetCheck(!m_tLcomESCGB19.bGB19_Freq);
	}
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedESCGB19QuasipermChk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (!m_nDesignType == 0)
	{
		((CButton*)GetDlgItem(IDC_CVL_GB19_FREQ_CHK))->SetCheck(!m_tLcomESCGB19.bGB19_QuasiPerm);
	}
}


void CCmdAutoLoadCombCvlDlg::OnBnClickedCmdLoadcomESCGB19ServiceLife()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_LOADCOM_CVL_GB19_FACTOR_EDT)->EnableWindow(m_tLcomESCGB19.bGB19_Factor);
	if (!m_tLcomESCGB19.bGB19_Factor)
	{
		GetDlgItem(IDC_CMD_LOADCOM_CVL_GB19_FACTOR_EDT)->SetWindowText(_T("1.0"));
	}
}

UINT CCmdAutoLoadCombCvlDlg::GetMovingKey(CString& strName)
{
    UINT Key = 0;
    // 内靛俊 蝶弗 规侥栏肺 荐沥 
    T_MVCD_D DataMvcd;
    if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
    switch (DataMvcd.nCodeType)
    {
        case D_MOVE_CODE_NONE:
            break;
        case D_MOVE_CODE_AASHTO_STAN:
        case D_MOVE_CODE_AASHTO_LRFD:
        case D_MOVE_CODE_TAIWAN:
        case D_MOVE_CODE_KOREA:
        case D_MOVE_CODE_CANADA:
        case D_MOVE_CODE_PENDOT:
        case D_MOVE_CODE_EURO_BS:
        case D_MOVE_CODE_RUSSIA:
        case D_MOVE_CODE_KOREA_LRFD_2011:
        case D_MOVE_CODE_AUSTRALIA:
        case D_MOVE_CODE_POLAND:
        case D_MOVE_CODE_SOUTH_AFRICA:
		case D_MOVE_CODE_JAPAN_RAIL:
        case D_MOVE_CODE_NEWZEALAND:
        case D_MOVE_CODE_BRAZIL:
            Key = m_pDoc->m_pAttrCtrl->GetMvldKey(strName);
            break;
        case D_MOVE_CODE_CHINA:
            Key = m_pDoc->m_pAttrCtrl->GetMvldchKey(strName);
            break;
        case D_MOVE_CODE_JAPAN:
            Key = m_pDoc->m_pAttrCtrl->GetMvldjpKey(strName);
            break;
        case D_MOVE_CODE_INDIA:
            Key = m_pDoc->m_pAttrCtrl->GetMvldidKey(strName);
            break;
        case D_MOVE_CODE_BS:
            Key = m_pDoc->m_pAttrCtrl->GetMvldbsKey(strName);
            break;
		case D_MOVE_CODE_FRANCE:
			Key = m_pDoc->m_pAttrCtrl->GetMvldfrKey(strName);
			break;
        case D_MOVE_CODE_TRANS:
            Key = m_pDoc->m_pAttrCtrl->GetMvldtrKey(strName);
            break;
        default:
            ASSERT(0);
            break;
    }

    return Key;
}