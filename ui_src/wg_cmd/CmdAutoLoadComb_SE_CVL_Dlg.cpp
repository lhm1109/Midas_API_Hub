// CmdAutoLoadComb_SE_CVL_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadComb_SE_CVL_Dlg.h"
#include "LoadCombCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_dbLock\LockMgr.h"   // for CLockMgr
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_dbLock\LockUtil.h"

#include "CmdSetOrthoLC_C3_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadComb_SE_CVL_Dlg dialog
enum LIST_IDX { IDX_NAME=0, IDX_FACTOR };

CCmdAutoLoadComb_SE_CVL_Dlg::CCmdAutoLoadComb_SE_CVL_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdAutoLoadComb_SE_CVL_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadComb_SE_CVL_Dlg)
	m_nOption = -1;
	m_bAddEnvelope = 0;
	m_nOrthoCombType = 0;
	m_nDesignType = EN_SEIS_EVL_CVL;
	m_Factor = 0.0;
	m_nGrvLF_DC = 0;
	m_nGrvLF_DW = 0;
	m_nGrvLF_DC_Num = 0;
	m_nGrvLF_DW_Num = 0;
	m_dGrvLF_DC1 = 0.0;
	m_dGrvLF_DC2 = 0.0;
	m_dGrvLF_DW1 = 0.0;
	m_dGrvLF_DW2 = 0.0;
	m_dEffLoadFactor = 0.5;
	//}}AFX_DATA_INIT
}


void CCmdAutoLoadComb_SE_CVL_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadComb_SE_CVL_Dlg)
	DDX_Radio  (pDX, IDC_CMD_LOADCOMB_ADD    ,m_nOption       );
	DDX_Check  (pDX, IDC_CMD_LOADCOMB_ADDENV ,m_bAddEnvelope  );
	DDX_Control(pDX, IDC_CMD_DESIGN_OTHO_CHK ,m_wndOthoChk    );
	DDX_Control(pDX, IDC_CMD_LOADCOMB_CODE   ,m_Code          );
	DDX_Radio  (pDX, IDC_GRV_LF1_MAX_RDO     ,m_nGrvLF_DC     );
	DDX_Radio  (pDX, IDC_GRV_LF2_MAX_RDO     ,m_nGrvLF_DW     );

	DDX_Control(pDX, IDC_GRV_LOAD_SEL_LST    ,m_lstGrvSel     );		
	DDX_Control(pDX, IDC_GRV_LOAD_UNSEL_LST  ,m_lstGrvUnsel   );		
	DDX_Control(pDX, IDC_EFF_LOAD_LST        ,m_lstEffLoad    );		
	DDX_Control(pDX, IDC_EFF_LOAD_LC_CMB     ,m_cmbEffLoadCase);		
	DDX_Text   (pDX, IDC_EFF_LOAD_LF_EDT2    ,m_dEffLoadFactor);	

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadComb_SE_CVL_Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdAutoLoadComb_SE_CVL_Dlg)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_ADD, OnCmdLoadcombAdd)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_REPLACE, OnCmdLoadcombReplace)
	ON_CBN_SELCHANGE(IDC_CMD_LOADCOMB_CODE, OnCodeSelchange)
	ON_BN_CLICKED(IDC_CMD_DESIGN_OTHO_CHK, OnCmdOthoChk)
	ON_BN_CLICKED(IDC_CMD_DESIGN_OTHOLC_SET_BTN, OnCmdSetOtholcBtn)
	ON_BN_CLICKED(IDC_GRV_LF1_MAX_RDO, OnCmdGrvLFType)
	ON_BN_CLICKED(IDC_GRV_LF1_MIN_RDO, OnCmdGrvLFType)
	ON_BN_CLICKED(IDC_GRV_LF1_BOTH_RDO, OnCmdGrvLFType)
	ON_BN_CLICKED(IDC_GRV_LOAD_LEFT_BTN , OnGrvLeftBtn   )
	ON_BN_CLICKED(IDC_GRV_LOAD_RIGHT_BTN, OnGrvRightBtn  )
	ON_BN_CLICKED(IDC_EFF_LOAD_ADD_BTN  , OnEffLoadAddBtn)
	ON_BN_CLICKED(IDC_EFF_LOAD_MOD_BTN  , OnEffLoadModBtn)
	ON_BN_CLICKED(IDC_EFF_LOAD_DEL_BTN  , OnEffLoadDelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadComb_SE_CVL_Dlg message handlers

BOOL CCmdAutoLoadComb_SE_CVL_Dlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// 변수들을 초기화합니다.
	m_strCodeName = _T("");
	m_strOldCodeName = _T("KSCE-LSD15");//
	
	InitMVCombo();
	InitCtrl();
	Initial_Data();  
	Data2Dlg();

	OnCmdGravLFEnableDisable(); // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화
	GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
	m_wndOthoChk.SetCheck(0);

	GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);

	CtrlManager();

	return TRUE;
}

void CCmdAutoLoadComb_SE_CVL_Dlg::InitMVCombo()
{
	m_cmbEffLoadCase.ResetContent();
	//m_cmbEffLoadCase.AddString(_LS(IDS_WG_CMD__ADDD__None));

	CArray<T_MVLD_K, T_MVLD_K> KeyList; KeyList.RemoveAll();
	CArray<T_MVLDjp_K, T_MVLDjp_K> KeyListjp; KeyListjp.RemoveAll();
	CArray<T_MVLDch_K, T_MVLDch_K> KeyListch; KeyListch.RemoveAll();
	CArray<T_MVLDid_K, T_MVLDid_K> KeyListid; KeyListid.RemoveAll();
	CArray<T_MVLDbs_K, T_MVLDbs_K> KeyListbs; KeyListbs.RemoveAll();
	CArray<T_MVLDfr_K, T_MVLDfr_K> KeyListfr; KeyListfr.RemoveAll();
	CArray<T_MVLDtr_K, T_MVLDtr_K> KeyListtr; KeyListtr.RemoveAll();

	// 코드에 따른 방식으로 수정 
	int nCount = 0;
	T_MVCD_D DataMvcd; DataMvcd.Initialize();
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

	for(int i=0; i<nCount; i++)
	{      
		T_MVLD_D rData; rData.Initialize();
		T_MVLDjp_D rDatajp; m_pDoc->m_pAttrCtrl->InitializeMvldjp(rDatajp);
		T_MVLDch_D rDatach; rDatach.Initialize();
		T_MVLDid_D rDataid; rDataid.Initialize();
		T_MVLDbs_D rDatabs; rDatabs.Initialize();
		T_MVLDfr_D rDatafr; rDatafr.Initialize();
		T_MVLDtr_D rDatatr; rDatatr.Initialize();

		T_MVCD_D DataMvcd;
		if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
		CString strLoadCase = _T("");
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
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvld(KeyList.GetAt(i), rData)) ASSERT(0); 
				strLoadCase = rData.LoadCaseName;
			}
			break;
		case D_MOVE_CODE_CHINA:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldch(KeyListch.GetAt(i), rDatach)) ASSERT(0); 
				strLoadCase = rDatach.LoadCaseName;
			}
			break;
		case D_MOVE_CODE_JAPAN:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldjp(KeyListjp.GetAt(i), rDatajp)) ASSERT(0); 
				strLoadCase = rDatajp.LoadCaseName;
			}
			break;
		case D_MOVE_CODE_INDIA:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldid(KeyListid.GetAt(i), rDataid)) ASSERT(0); 
				strLoadCase = rDataid.LoadCaseName;
			}
			break;
		case D_MOVE_CODE_BS:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldbs(KeyListbs.GetAt(i), rDatabs)) ASSERT(0); 
				strLoadCase = rDatabs.LoadCaseName;
			}
			break;
		case D_MOVE_CODE_FRANCE:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldfr(KeyListfr.GetAt(i), rDatafr)) ASSERT(0); 
				strLoadCase = rDatafr.LoadCaseName;
			}
			break;
		case D_MOVE_CODE_TRANS:
			{
				if(!m_pDoc->m_pAttrCtrl->GetMvldtr(KeyListtr.GetAt(i), rDatatr)) ASSERT(0); 
				strLoadCase = rDatatr.LoadCaseName;
			}
			break;
		default:
			ASSERT(0);
			break;
		}
		m_cmbEffLoadCase.AddString(strLoadCase);
		m_arrEff_MVData.Add(strLoadCase); // MV Type 저장
	}
	m_cmbEffLoadCase.SetCurSel(0);
}

void CCmdAutoLoadComb_SE_CVL_Dlg::InitCtrl()
{
	CString aTitle_grv[] = { _LS(IDS_WG_CMD__ADDD__Load_Case_Name) };
	int nColWidth_grv[] = {115};
	SetListCtrlHeader(m_lstGrvSel, 1, aTitle_grv, nColWidth_grv);
	SetListCtrlHeader(m_lstGrvUnsel, 1, aTitle_grv, nColWidth_grv);

	CString aTitle_eff[] = { _LS(IDS_WG_CMD__ADDD__Load_Case), _LS(IDS_WG_CMD__ADDD__Factor) };
	int nColWidth_eff[] = {105, 70};
	SetListCtrlHeader(m_lstEffLoad, 2, aTitle_eff, nColWidth_eff);

	CArray<T_STLD_K, T_STLD_K> aStld_EffLoad;
	m_pDoc->m_pAttrCtrl->GetStldKeyListVariableParam(aStld_EffLoad, 2, _T("L"), _T("S"));
	for(int i=0; i<aStld_EffLoad.GetSize(); i++)
	{
		T_STLD_D StldD;
		if(!m_pDoc->m_pAttrCtrl->GetStld(aStld_EffLoad[i], StldD)) { ASSERT(0); continue; }
		//CDlgUtil::CobxAddItem(m_cmbEffLoadCase, StldD.LoadCaseName, aStld_EffLoad[i]);
		m_cmbEffLoadCase.AddString(StldD.LoadCaseName);
		m_arrEff_LLData.Add(StldD.LoadCaseName);
	}
	m_cmbEffLoadCase.SetCurSel(0);
}

BOOL CCmdAutoLoadComb_SE_CVL_Dlg::GetLoadCombTypeByLoadCaseType(int nLoadCaseType, int& nAnalType)
{
	switch(nLoadCaseType)
	{
	case D_LOADCASE_STATIC        : nAnalType = D_LCOM_STATIC;      break;
	case D_LOADCASE_SPECTRUM      : nAnalType = D_LCOM_SPECTRUM;    break;    
	case D_LOADCASE_HISTORY       : nAnalType = D_LCOM_HISTORY;     break;
	case D_LOADCASE_MOVING        : nAnalType = D_LCOM_MOVING;      break;
	case D_LOADCASE_SETTLE        : nAnalType = D_LCOM_SETTLEMENT;  break;
	case D_LOADCASE_ENVELOPE      : nAnalType = D_LCOM_ENVELOPE;    break;
	case D_LOADCASE_COMB_GENERAL  : nAnalType = D_LCOM_CB_GENERAL;  break;
	case D_LOADCASE_COMB_STEEL    : nAnalType = D_LCOM_CB_STEEL;    break;
	case D_LOADCASE_COMB_CONCRETE : nAnalType = D_LCOM_CB_CONCRETE; break;
	case D_LOADCASE_COMB_FDN      : nAnalType = D_LCOM_CB_FOOTING;  break;
	case D_LOADCASE_COMB_SRC      : nAnalType = D_LCOM_CB_SRC;      break;
	case D_LOADCASE_COMB_ALUMINUM : nAnalType = D_LCOM_CB_ALUMINUM; break;
	case D_LOADCASE_COMB_STLCOMP  : nAnalType = D_LCOM_CB_STLCOMP;  break;
	case D_LOADCASE_PUSHOVER      : nAnalType = D_LCOM_PUSHOVER;    break;
	case D_LOADCASE_STAGE         : nAnalType = D_LCOM_STAGE;       break;
	case D_LOADCASE_ECCENSPEC     : nAnalType = D_LCOM_ECCENSPEC;   break;
	case D_LOADCASE_GRIDMODEL     : nAnalType = D_LCOM_GRIDMODEL;   break;
	case D_LOADCASE_CRANE         : nAnalType = D_LCOM_CRANE;       break;      
	default:  ASSERT(0);    return FALSE;
	}
	return TRUE;
}

void CCmdAutoLoadComb_SE_CVL_Dlg::SetDefaultGravData()
{
	m_pDoc->m_pAttrCtrl->GetStldKeyListVariableParam(m_aUnselGravLoad, 3, _T("D"), _T("DC"), _T("DW")); // MQC.5794_15 Select -> Unselect
}

void CCmdAutoLoadComb_SE_CVL_Dlg::Initial_Data()
{
	if(m_nOption!=0 && m_nOption!=1) 
		m_nOption = 0;  //  0 : Add,   1 : Replace
	m_Code.ResetContent();
	m_bAddEnvelope = TRUE;
	Set_InitCode();
	m_Factor = 1.;
	SetDefaultGravData();

	UpdateData(FALSE);
}

// Coded by Seungjun ('20070719) MNet:No.2915.
void CCmdAutoLoadComb_SE_CVL_Dlg::Set_InitCode()
{
	m_aCodeEnable.RemoveAll();
	CString strCurrentCode=_T("KSCE-LSD15");
	m_Code.AddString(_T("KSCE-LSD15"));
	m_Code.AddString(_T("KSCE-USD10"));

	int Index = m_Code.FindString(-1, strCurrentCode);
	if(Index==-1)	Index = 0;
	m_Code.SetCurSel(Index);

	m_nOldCodeIndex = Index;

}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnCmdLoadcombAdd() 
{
	m_nOption = 0;
}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnCmdLoadcombReplace() 
{
	m_nOption = 1;
}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnOK() 
{
	UpdateData(TRUE);
	if(!Dlg2Data()) return;

	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);

	CString csErrMsg;
	int nType;
	if (m_nDesignType == EN_SEIS_EVL_CVL ) nType = CLoadDgnCodeChecker::eLCBSeisEvaluation;
	else { ASSERT(0); return; }
	BOOL bAllow = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(nType, strCodeName, csErrMsg);
	if (!bAllow)
	{
		AfxMessageBox(csErrMsg);
		return;
	}

	//   SaveLfcaListData();

	if(CDBLib::IsAutoLoadCombCodeName(strCodeName))
	{
		GetGravLoadFactor(strCodeName);
		CLoadCombCtrl LoadCombCtrl;
		LoadCombCtrl.m_nDesignType = m_nDesignType;
		LoadCombCtrl.Set_LcomType(m_nLcomType);
		LoadCombCtrl.m_nOrthoType	 = (m_wndOthoChk.GetCheck() ? 0	: -1);
		LoadCombCtrl.Set_AddEnvelope(m_bAddEnvelope);     // 19.01.28. AddEnvelope 요청
		LoadCombCtrl.Set_OrthoCombType(m_nOrthoCombType); // 19.01.28. CombType 요청
		//
		LoadCombCtrl.m_nKoreaLr11DcNum = m_nGrvLF_DC_Num;
		LoadCombCtrl.m_dKoreaLr11Dc[0] = m_dGrvLF_DC1;
		LoadCombCtrl.m_dKoreaLr11Dc[1] = m_dGrvLF_DC2;
		LoadCombCtrl.m_nKoreaLr11DwNum = m_nGrvLF_DW_Num;
		LoadCombCtrl.m_dKoreaLr11Dw[0] = m_dGrvLF_DW1;
		LoadCombCtrl.m_dKoreaLr11Dw[1] = m_dGrvLF_DW2;

		/// set parameter for seismic evaluation.
		LoadCombCtrl.m_aSelGravLoad.RemoveAll();
		for (int i=0; i<m_aSelGravLoad.GetSize(); ++i)
		{
			LoadCombCtrl.m_aSelGravLoad.Add(m_aSelGravLoad[i]);
		}
		LoadCombCtrl.m_aEffectiveLoad.RemoveAll();
		for (int i=0; i<m_aEffectiveLoad.GetSize(); ++i)
		{
			LoadCombCtrl.m_aEffectiveLoad.Add(m_aEffectiveLoad[i]);
		}
		LoadCombCtrl.m_aEffectiveMVLoad.RemoveAll();
		for (int i=0; i<m_aEffectiveMVLoad.GetSize(); ++i)
		{
			LoadCombCtrl.m_aEffectiveMVLoad.Add(m_aEffectiveMVLoad[i]);
		}
		for (int i=0; i<m_a100_30_Load.GetSize(); ++i)
		{
			LoadCombCtrl.m_a100_30_Load.Add(m_a100_30_Load[i]);
		}
		BOOL bOK = LoadCombCtrl.Set_DefaultLoadComb(m_nOption, strCodeName);

		if(bOK)	CDialogMove::OnOK();
	}
	else	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Generation_code_unavailable_),MB_OK);
}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CCmdAutoLoadComb_SE_CVL_Dlg::AlignControl()
{
	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	if(Index != -1)	m_Code.GetLBText(Index,strCodeName);
	//
	BOOL bShow = FALSE;
	if(strCodeName == _T("KSCE-LSD15")) bShow = TRUE;

	CArray<UINT,UINT> aCtrlID_LSD15;
	aCtrlID_LSD15.Add(IDC_GRV_LOAD_LF_STC);
	aCtrlID_LSD15.Add(IDC_GRV_LF1_MAX_RDO);
	aCtrlID_LSD15.Add(IDC_GRV_LF1_MIN_RDO);
	aCtrlID_LSD15.Add(IDC_GRV_LF1_BOTH_RDO);
	aCtrlID_LSD15.Add(IDC_GRV_LOAD_LF_STC2);
	aCtrlID_LSD15.Add(IDC_GRV_LF2_MAX_RDO);
	aCtrlID_LSD15.Add(IDC_GRV_LF2_MIN_RDO);
	aCtrlID_LSD15.Add(IDC_GRV_LF2_BOTH_RDO);
	//
	CArray<UINT,UINT> aCtrlID_Move;
	aCtrlID_Move.Add(IDC_GRV_LOAD_SEL_STC);
	aCtrlID_Move.Add(IDC_GRV_LOAD_UNSEL_STC);
	aCtrlID_Move.Add(IDC_GRV_LOAD_SEL_LST);
	aCtrlID_Move.Add(IDC_GRV_LOAD_LEFT_BTN);
	aCtrlID_Move.Add(IDC_GRV_LOAD_RIGHT_BTN);
	aCtrlID_Move.Add(IDC_GRV_LOAD_UNSEL_LST);
	aCtrlID_Move.Add(IDC_EFF_LOAD_GRP);
	aCtrlID_Move.Add(IDC_EFF_LOAD_LC_STC);
	aCtrlID_Move.Add(IDC_EFF_LOAD_LC_CMB);
	aCtrlID_Move.Add(IDC_EFF_LOAD_LF_STC);
	aCtrlID_Move.Add(IDC_EFF_LOAD_LF_EDT2);
	aCtrlID_Move.Add(IDC_EFF_LOAD_LST);
	aCtrlID_Move.Add(IDC_EFF_LOAD_ADD_BTN);
	aCtrlID_Move.Add(IDC_EFF_LOAD_MOD_BTN);
	aCtrlID_Move.Add(IDC_EFF_LOAD_DEL_BTN);
	aCtrlID_Move.Add(IDC_CMD_DESIGN_OTHO_CHK);
	aCtrlID_Move.Add(IDC_CMD_DESIGN_OTHOLC_SET_BTN);
	aCtrlID_Move.Add(IDC_CMD_OTHO_LCB_FRAME);
	aCtrlID_Move.Add(IDOK);
	aCtrlID_Move.Add(IDCANCEL);

	const int nMove = 48;

	if(bShow)
	{
		CDlgUtil::CtrlMoveDistY(this, aCtrlID_Move, nMove);
		CDlgUtil::ResizeDlgByBotItem(this, IDC_CMD_GRAVITY_FRAME, IDC_EFF_LOAD_GRP, globalUtils.ScaleByDPI(7));
		CDlgUtil::CtrlShowHide(this, aCtrlID_LSD15, bShow);
	}
	else
	{
		CDlgUtil::CtrlMoveDistY(this, aCtrlID_Move, -nMove);
		CDlgUtil::ResizeDlgByBotItem(this, IDC_CMD_GRAVITY_FRAME, IDC_EFF_LOAD_GRP, globalUtils.ScaleByDPI(7));
		CDlgUtil::CtrlShowHide(this, aCtrlID_LSD15, bShow);
	}

	// 윈도우 사이즈를 맞춘다.
	CWnd* pWndLast = GetDlgItem(IDOK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + 12;
	MoveWindow(r);
	// 화면 잔상 문제 제거
	Invalidate(TRUE);

}

void CCmdAutoLoadComb_SE_CVL_Dlg::CtrlManager()
{
	UpdateData(FALSE);
}

//add by cylee 05.04.22
void CCmdAutoLoadComb_SE_CVL_Dlg::OnCmdSetOtholcBtn()
{
	CCmdSetOrthoLC_C3_Dlg dlg;
	//dlg.SetSeismicEvaluation(TRUE);
	dlg.SetData(m_nOrthoCombType, m_a100_30_Load);
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetData(m_nOrthoCombType, m_a100_30_Load);
	}
}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnCmdOthoChk()
{
	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);

	BOOL bCheck = m_wndOthoChk.GetCheck();
	GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(bCheck);

}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnCmdGrvLFType()
{
	// if(m_nOthoType==0) m_=2;
}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnCodeSelchange()
{
	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);
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
	if (Index == m_nOldCodeIndex)
	{
		return;
	}
	m_nOldCodeIndex = Index;

	AlignControl();
	CtrlManager();
}

void CCmdAutoLoadComb_SE_CVL_Dlg::Data2Dlg()
{
	LV_ITEM lvitem;

	// Selected
	lvitem.iSubItem = IDX_NAME;
	for(int i=0; i<m_aSelGravLoad.GetSize(); i++)
	{
		T_STLD_D StldD;
		if(!m_pDoc->m_pAttrCtrl->GetStld(m_aSelGravLoad[i], StldD)) { ASSERT(0); continue; }
		lvitem.iItem  	= i;
		lvitem.mask	= LVIF_TEXT | LVFI_STRING;
		lvitem.pszText	= (LPTSTR)(LPCTSTR)StldD.LoadCaseName;
		m_lstGrvSel.InsertItem(&lvitem);
	}

	// Unselected
	for(int i=0; i<m_aUnselGravLoad.GetSize(); i++)
	{
		T_STLD_D StldD;
		if(!m_pDoc->m_pAttrCtrl->GetStld(m_aUnselGravLoad[i], StldD)) { ASSERT(0); continue; }
		lvitem.iItem  	= i;
		lvitem.mask	= LVIF_TEXT | LVFI_STRING;
		lvitem.pszText	= (LPTSTR)(LPCTSTR)StldD.LoadCaseName;
		m_lstGrvUnsel.InsertItem(&lvitem);
	}

	// Effective Load
	for(int i=0; i<m_aEffectiveLoad.GetSize(); i++)
	{
		T_STLD_D StldD;
		if(!m_pDoc->m_pAttrCtrl->GetStld(m_aEffectiveLoad[i].StldK, StldD)) { ASSERT(0); continue; }
		lvitem.iItem  	= i;
		lvitem.iSubItem = IDX_NAME;
		lvitem.mask=LVIF_TEXT | LVFI_STRING;
		lvitem.pszText	= (LPTSTR)(LPCTSTR)StldD.LoadCaseName;
		m_lstEffLoad.InsertItem(&lvitem);

		lvitem.iSubItem = IDX_FACTOR;
		lvitem.mask=LVIF_TEXT;
		CString strFactor;
		strFactor.Format(_T("%g"), m_aEffectiveLoad[i].dFactor);
		lvitem.pszText	= (LPTSTR)(LPCTSTR)strFactor;
		m_lstEffLoad.SetItem(&lvitem);
	}

	UpdateData(FALSE);
} 

BOOL CCmdAutoLoadComb_SE_CVL_Dlg::Dlg2Data()
{
	m_aSelGravLoad.RemoveAll();
	for(int i=0; i<m_lstGrvSel.GetItemCount(); i++)
	{
		CString strStldName = m_lstGrvSel.GetItemText(i,IDX_NAME);
		T_STLD_K StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strStldName);
		m_aSelGravLoad.Add(StldK);
	}

	m_aUnselGravLoad.RemoveAll();
	for(int i=0; i<m_lstGrvUnsel.GetItemCount(); i++)
	{
		CString strStldName = m_lstGrvUnsel.GetItemText(i,IDX_NAME);
		T_STLD_K StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strStldName);
		m_aUnselGravLoad.Add(StldK);
	}

	m_aEffectiveLoad.RemoveAll();
	m_aEffectiveMVLoad.RemoveAll();
	for(int i=0; i<m_lstEffLoad.GetItemCount(); i++)
	{
		 CString strStldName = m_lstEffLoad.GetItemText(i,IDX_NAME);
		 if (IsEff_MV_Type(strStldName))
		 {
			 _LCOM_GRAV_EFF_MV gEff_MV;
			 gEff_MV.MvldK = m_pDoc->m_pAttrCtrl->GetMvldKeyNew(strStldName);
			 gEff_MV.dFactor = _ttof(m_lstEffLoad.GetItemText(i,IDX_FACTOR));
			 m_aEffectiveMVLoad.Add(gEff_MV);
		 }
		 else if (IsEff_LL_Type(strStldName))
		 {
			 _LCOM_GRAV_EFF gEff_LL;
			 gEff_LL.StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strStldName);
			 gEff_LL.dFactor = _ttof(m_lstEffLoad.GetItemText(i,IDX_FACTOR));
			 m_aEffectiveLoad.Add(gEff_LL);
		 }
		 else ASSERT(0); //여기 들어오면 절대안됨
	}

	return TRUE;
}

void CCmdAutoLoadComb_SE_CVL_Dlg::SetListCtrlHeader(CListCtrl& list, const int nColNum, const CString* aTitle, const int* nColWidth)
{
	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(list.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(list.GetSafeHwnd(), dwStyle);

	// Set Title
	for(int i = 0; i<nColNum; i++)
	{
		CString title = aTitle[i];
		LV_COLUMN lvcolumn;
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = i==IDX_NAME ? LVCFMT_LEFT : LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		list.InsertColumn(i,&lvcolumn);
		list.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnEffLoadAddBtn()
{
	UpdateData();
	int Index = m_cmbEffLoadCase.GetCurSel();
	if (Index != -1)
	{
		int ListNo = 0;
		CString strName=_T("");
		m_cmbEffLoadCase.GetLBText(Index,strName);
		if (CheckMovingCaseName(strName, ListNo))
		{
			// 리스트에 있나?
			LVFINDINFO FindInfo;
			FindInfo.flags = LVFI_STRING;
			FindInfo.psz = strName;
			int nItem = m_lstEffLoad.FindItem(&FindInfo);
			if (nItem != -1)
				return;

			LV_ITEM lvitem;
			lvitem.mask = LVIF_TEXT | LVFI_STRING;
			lvitem.iItem  	= m_lstEffLoad.GetItemCount();
			lvitem.iSubItem = IDX_NAME;
			lvitem.pszText	= (LPTSTR)(LPCTSTR)strName;
			m_lstEffLoad.InsertItem(&lvitem);

			lvitem.iSubItem = IDX_FACTOR;
			lvitem.mask	= LVIF_TEXT;
			CString strFactor;
			strFactor.Format(_T("%g"), m_dEffLoadFactor);
			lvitem.pszText	= (LPTSTR)(LPCTSTR)strFactor;
			m_lstEffLoad.SetItem(&lvitem);
		}
		else
		{
			CString msg;      
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), strName);
			AfxMessageBox(msg);
			return;
		}
	}
	else AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));  	
}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnEffLoadModBtn()
{
	UpdateData();
	int Index = m_cmbEffLoadCase.GetCurSel();
	if (Index != -1) return;
	
	int ListNo = 0;
	CString strName=_T("");
	m_cmbEffLoadCase.GetLBText(Index, strName);    

	// 리스트에 있나?
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_STRING;
	FindInfo.psz = strName;
	int iItem = m_lstEffLoad.FindItem(&FindInfo);
	if (iItem == -1)
		return;

	LV_ITEM lvitem;
	lvitem.mask = LVIF_TEXT | LVFI_STRING;
	lvitem.iItem  	= iItem;
	lvitem.iSubItem = IDX_NAME;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strName;
	m_lstEffLoad.SetItem(&lvitem);

	lvitem.iSubItem = IDX_FACTOR;
	lvitem.mask	= LVIF_TEXT;
	CString strFactor;
	strFactor.Format(_T("%g"), m_dEffLoadFactor);
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strFactor;
	m_lstEffLoad.SetItem(&lvitem);
}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnEffLoadDelBtn()
{
	int iItem = m_lstEffLoad.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}
	m_lstEffLoad.DeleteItem(iItem);
}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnGrvLeftBtn()
{
	CArray<int, int> aSelItem;
	CDlgUtil::GetSelectedListItemIndex(&m_lstGrvUnsel, aSelItem);
	if(aSelItem.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Not_exist_selected_item));
		return;
	}

	CString strStldName;
	LV_ITEM lvitem;
	for (int i = 0; i < aSelItem.GetSize(); i++)
	{
		strStldName = m_lstGrvUnsel.GetItemText(aSelItem[i],0);  
		lvitem.iItem  	= m_lstGrvSel.GetItemCount();
		lvitem.iSubItem = IDX_NAME;
		lvitem.mask	= LVIF_TEXT | LVFI_STRING;
		lvitem.pszText	= (LPTSTR)(LPCTSTR)strStldName;
		m_lstGrvSel.InsertItem(&lvitem);    
	}

	for (int i = aSelItem.GetSize() - 1; i >= 0; i--)
	{
		m_lstGrvUnsel.DeleteItem(aSelItem[i]);
	}
}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnGrvRightBtn()
{
	CArray<int, int> aSelItem;
	CDlgUtil::GetSelectedListItemIndex(&m_lstGrvSel, aSelItem);
	if(aSelItem.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Not_exist_selected_item));
		return;
	}

	CString strStldName;  
	LV_ITEM lvitem;
	for (int i = 0; i < aSelItem.GetSize(); i++)
	{
		strStldName = m_lstGrvSel.GetItemText(aSelItem[i],0);
		lvitem.iItem  	= m_lstGrvUnsel.GetItemCount();
		lvitem.iSubItem = IDX_NAME;
		lvitem.mask	= LVIF_TEXT | LVFI_STRING;
		lvitem.pszText	= (LPTSTR)(LPCTSTR)strStldName;
		m_lstGrvUnsel.InsertItem(&lvitem);    
	}

	for (int i = aSelItem.GetSize() - 1; i >= 0; i--)
	{
		m_lstGrvSel.DeleteItem(aSelItem[i]);
	}
}

BOOL CCmdAutoLoadComb_SE_CVL_Dlg::ErrorCheck()
{
	BOOL bCheck=TRUE;

	if(m_Factor < 0. || m_Factor > 10.)  bCheck = FALSE;
	if(!bCheck)	
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_J_Factor____),MB_OK); return bCheck;
	}

	if(m_Factor < 1. && m_Factor > 0.)  bCheck = FALSE;
	if(!bCheck)	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Warning___Check_J_Factor__),MB_OK);

	return TRUE;
}

BOOL CCmdAutoLoadComb_SE_CVL_Dlg::CheckMovingCaseName(CString str,int& Index)
{
	BOOL bCheck=TRUE;
	int Count = m_lstEffLoad.GetItemCount();
	for(int i = 0; i < Count; i++)
	{
		CString strLcName = m_lstEffLoad.GetItemText(i,0);
		if(str == strLcName)  
		{
			Index = i;
			bCheck = FALSE;
		}
	}
	return bCheck;
}

BOOL CCmdAutoLoadComb_SE_CVL_Dlg::IsEff_MV_Type(CString str)
{
	BOOL bMV_Type = FALSE;
	for (int i = 0; i < m_arrEff_MVData.GetCount(); ++i)
	{
		CString strMVData = m_arrEff_MVData.GetAt(i);
		if (str == strMVData)
		{
			bMV_Type = TRUE;
		}
	}
	return bMV_Type;
}

BOOL CCmdAutoLoadComb_SE_CVL_Dlg::IsEff_LL_Type(CString str)
{
	BOOL bLL_Type = FALSE;
	for (int i = 0; i < m_arrEff_LLData.GetCount(); ++i)
	{
		CString strLLData = m_arrEff_LLData.GetAt(i);
		if (str == strLLData)
		{
			bLL_Type = TRUE;
		}
	}
	return bLL_Type;
}

void CCmdAutoLoadComb_SE_CVL_Dlg::OnCmdGravLFEnableDisable()
{
	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();

	BOOL bDcExist = FALSE;
	BOOL bDwExist = FALSE;

	if (iCount!=0)
	{
		for (int i = 0; i < iCount; ++i)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetStld(iKey, rData)) { ASSERT(0); continue; }

			if     (rData.LoadCaseType==_T("D"))	 { bDcExist = TRUE; }  // General Dead Load
			else if(rData.LoadCaseType==_T("DC"))	 { bDcExist = TRUE; }  // Component and Attachments
			else if(rData.LoadCaseType==_T("DW"))	 { bDwExist = TRUE; }  // Wearing Surfaces and Utilities

		}
	}
	UpdateData(TRUE);
	GetDlgItem(IDC_GRV_LF1_MAX_RDO )->EnableWindow(bDcExist);
	GetDlgItem(IDC_GRV_LF1_MIN_RDO )->EnableWindow(bDcExist);
	GetDlgItem(IDC_GRV_LF1_BOTH_RDO)->EnableWindow(bDcExist);

	GetDlgItem(IDC_GRV_LF2_MAX_RDO )->EnableWindow(bDwExist);
	GetDlgItem(IDC_GRV_LF2_MIN_RDO )->EnableWindow(bDwExist);
	GetDlgItem(IDC_GRV_LF2_BOTH_RDO)->EnableWindow(bDwExist);
}

void CCmdAutoLoadComb_SE_CVL_Dlg::GetGravLoadFactor(CString strCodeName)
{
	UpdateData(TRUE);
	// 0:Max 1: Min 2: Both
	switch(m_nGrvLF_DC)  // Component and Attachments
	{
	case 0: m_nGrvLF_DC_Num = 1; m_dGrvLF_DC1 = 1.25; m_dGrvLF_DC2 = 1.25; break; 
	case 1: m_nGrvLF_DC_Num = 1; m_dGrvLF_DC1 = 0.90; m_dGrvLF_DC2 = 0.90; break; 
	case 2: m_nGrvLF_DC_Num = 2; m_dGrvLF_DC1 = 1.25; m_dGrvLF_DC2 = 0.90; break;
	}
	switch(m_nGrvLF_DW)  // Wearing Surfaces and Utilities
	{
	case 0: m_nGrvLF_DW_Num = 1; m_dGrvLF_DW1 = 1.50; m_dGrvLF_DW2 = 1.50; break; 
	case 1: m_nGrvLF_DW_Num = 1; m_dGrvLF_DW1 = 0.65; m_dGrvLF_DW2 = 0.65; break; 
	case 2: m_nGrvLF_DW_Num = 2; m_dGrvLF_DW1 = 1.50; m_dGrvLF_DW2 = 0.65; break;
	}
	if (strCodeName==_T("KSCE-USD10"))
	{
		m_nGrvLF_DC = 0;
		m_nGrvLF_DW = 0;
		m_nGrvLF_DC_Num = 1;
		m_nGrvLF_DW_Num = 1;
		m_dGrvLF_DC1 = 1.0;
		m_dGrvLF_DC2 = 1.0;
		m_dGrvLF_DW1 = 1.0;
		m_dGrvLF_DW2 = 1.0;
	}
}