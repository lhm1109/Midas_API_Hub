// CmdAutoLoadCombCvlSP35133302011Page.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlTMH7Page.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "CmdAutoLoadCombCvlHelpDlg.h"
#include "LoadCombCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "CmdSetOrthoLC_C3_Dlg.h"

#include "CmdMovingFactorTMH7Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


enum { Highway = 0, PedestrianCyclist = 1};

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlTMH7Page dialog
CCmdAutoLoadCombCvlTMH7Page::CCmdAutoLoadCombCvlTMH7Page(CWnd* pParent /*=NULL*/)
    : CChildDialog(CCmdAutoLoadCombCvlTMH7Page::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCmdAutoLoadCombCvlTMH7Page)
    //}}AFX_DATA_INIT
    m_pDoc = CDBDoc::GetDocPoint();

    m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

    m_nConstLoad = 0;

    m_nBD21D  = 0;
    m_nBD21DW = 0;
    m_nBD21DC = 0;
    m_nBD21EV = 0;
    m_nBD21EH = 0;
    m_nBD21ES = 0;

	m_dUserDefDLFactor = 1.2;
	m_dUserDefSIDLFactor=1.2;
	m_dEffLoadFactor = 1.25;
	

	m_nOrthoCombType = 0;
	m_a100_30_Load.RemoveAll();
}

void CCmdAutoLoadCombCvlTMH7Page::DoDataExchange(CDataExchange* pDX)
{
	double m_dMaxVal;
	double m_dMinVal;
	double m_dMaxVal2;
	double m_dMinVal2;
    
    CChildDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCmdAutoLoadCombCvlTMH7Page)  
    DDX_Control(pDX, IDC_CMD_TMH7_VEP_CMB, m_cmbVEP);
    DDX_Control(pDX, IDC_CMD_TMH7_NVEP_CMB, m_cmbNVEP);
    DDX_Radio  (pDX, IDC_CMD_TMH7_CODE_BASED,		    m_nTMH7_Code_based);
	DDX_Control(pDX, IDC_CMD_BD_21_EPS_EDT, m_edtEPS_Max);
	DDX_Control(pDX, IDC_GRV_LF1_MAX_EDIT,		    m_edtBD21D_Max);
	DDX_Control(pDX, IDC_GRV_LF1_MIN_EDIT,		    m_edtBD21D_Min);
    DDX_Radio(pDX, IDC_GRV_LF1_MAX_RDO, m_nBD21D);
    DDX_Radio(pDX, IDC_GRV_LF2_MAX_RDO, m_nBD21DW);
    DDX_Radio(pDX, IDC_GRV_LF2_MAX_RDO, m_nBD21DC);
  /*  DDX_Radio  (pDX, IDC_CMD_TMH7_USER_DEFINED,		    m_nTMH7_User_Defined);
    DDX_Control(pDX, IDC_CMD_BD_21_DW_MAX_EDT,		    m_edtBD21DW_Max);
    DDX_Control(pDX, IDC_CMD_BD_21_DW_MIN_EDT,		    m_edtBD21DW_Min);
    DDX_Radio  (pDX, IDC_CMD_BD_21_DC_MAX_RDO,		    m_nBD21DC);
    DDX_Control(pDX, IDC_CMD_BD_21_DC_MAX_EDT,		    m_edtBD21DC_Max);
    DDX_Control(pDX, IDC_CMD_BD_21_DC_MIN_EDT,		    m_edtBD21DC_Min);
    DDX_Radio  (pDX, IDC_CMD_BD_21_EV_MAX_RDO,		    m_nBD21EV);
    DDX_Control(pDX, IDC_CMD_BD_21_EV_MAX_EDT,		    m_edtBD21EV_Max);
    DDX_Control(pDX, IDC_CMD_BD_21_EV_MIN_EDT,		    m_edtBD21EV_Min);
    DDX_Radio  (pDX, IDC_CMD_BD_21_EH_MAX_RDO,		    m_nBD21EH);
	DDX_Control(pDX, IDC_CMD_BD_21_EH_MAX_EDT,		    m_edtBD21EH_Max);
	DDX_Control(pDX, IDC_CMD_BD_21_EH_MIN_EDT,		    m_edtBD21EH_Min);*/
	DDX_Control(pDX, IDC_GRV_LF2_MAX_EDIT, m_edtBD21DW_Max);
	DDX_Control(pDX, IDC_GRV_LF2_MIN_EDIT, m_edtBD21DW_Min);
    DDX_Control(pDX, IDC_CMD_BD_21_MV_TYPE_CMB,	        m_MoveType);//Vehicle TYpe

    DDX_Control(pDX, IDC_CMD_BD_21_MVL_CMB,             m_MoveLcase);//Load Case
     DDX_Control(pDX, IDC_CMD_TMH7_MVL_LST,             m_MovingList);

    DDX_Control(pDX, IDC_CMD_BD_21_ULS_EDT,				m_edtBD21Gamma_ULS);
    DDX_Control(pDX, IDC_CMD_BD_21_SLS_EDT,				m_edtBD21Gamma_SLS);

    DDX_Control(pDX, IDC_CMD_BD_21_LOAD_FACTOR_GRP,	    m_chkLoadFactor);
    DDX_Control(pDX, IDC_CMD_BD_21_PFMV_GRP, m_chkLongBrkTrc);
	DDX_Control(pDX, IDC_IRC6_LVARB_TRAFF_ROAD_RDO2,	m_LongBrkTrcChk);
	DDX_Control(pDX, IDC_CMD_TMH7_SEIS_LCOM_LL_LC_CMB, m_cmbEffLoadCase);
	DDX_Control(pDX, IDC_CMD_TMH7_SEIS_LCOM_LL_LST, m_lstEffLoad);

	DDX_Text(pDX, IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LF_EDT, m_dEffLoadFactor);
    DDX_Control(pDX, IDC_CMD_TMH7_OTHO_CHK, m_wndOthoChk);
	//DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LC_CMB,             m_BrkTrctv);
	//DDX_Text(pDX, IDC_GRV_LF1_MAX_EDIT, m_strMaxVal); // or m_dMaxVal
	//DDX_Text(pDX, IDC_GRV_LF1_MIN_EDIT, m_strMinVal); // or m_dMinVal
	//DDX_Text(pDX, IDC_GRV_LF2_MAX_EDIT, m_strMaxVal2); // or m_dMaxVal
	//DDX_Text(pDX, IDC_GRV_LF2_MIN_EDIT, m_strMinVal2); // or m_dMinVal

}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlTMH7Page, CChildDialog)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlTMH7Page)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_CMD_BD_21_BRIDGE_TYPE_CMB, OnCmdBD21Brdge)
	ON_BN_CLICKED(IDC_IRC6_LVARB_TRAFF_ROAD_RDO2, OnCmdLongBrkTrc)

	ON_CBN_SELCHANGE(IDC_CMD_BD_21_MVL_CMB, OnCmdBD21MV)
	ON_BN_CLICKED(IDC_CMD_BD_21_MVL_ADD_BTN, OnCmdBD21MVAddBtn)
	ON_BN_CLICKED(IDC_CMD_BD_21_MVL_DEL_BTN, OnCmdBD21MVDelBtn)
	ON_BN_CLICKED(IDC_CMD_BD_21_MVL_TABLE_BTN, OnCmdBD21MVFactorBtn)

	ON_BN_CLICKED(IDC_CMD_TMH7_CODE_BASED, OnCmdCodeOrUserRdo)
	ON_BN_CLICKED(IDC_CMD_TMH7_USER_DEFINED, OnCmdCodeOrUserRdo)

	ON_BN_CLICKED(IDC_GRV_LF1_MAX_RDO, OnCmdUserDefDL)
	ON_BN_CLICKED(IDC_GRV_LF1_MIN_RDO, OnCmdUserDefDL)
	ON_BN_CLICKED(IDC_GRV_LF1_BOTH_RDO, OnCmdUserDefDL)

	ON_BN_CLICKED(IDC_GRV_LF2_MAX_RDO, OnCmdUserDefSIDL)
	ON_BN_CLICKED(IDC_GRV_LF2_MIN_RDO, OnCmdUserDefSIDL)
	ON_BN_CLICKED(IDC_GRV_LF2_BOTH_RDO, OnCmdUserDefSIDL)

	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_ADD_BTN, OnEffLoadAddBtn)
	//ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_MOD_BTN, OnEffLoadModBtn)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_DEL_BTN, OnEffLoadDelBtn)

	ON_BN_CLICKED(IDC_CMD_TMH7_OTHO_CHK, OnCmdOthoChk)
	ON_BN_CLICKED(IDC_CMD_TMH7_OTHOLC_SET_BTN, OnCmdSetOtholcBtn)
 /*   ON_BN_CLICKED(IDC_CMD_LB_TRACTION_BTN, OnBnClickedLbTractionBtn) */// Add this line
    ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
    ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()


void CCmdAutoLoadCombCvlTMH7Page::InitCombo()
{
	m_cmbVEP.ResetContent();
	m_cmbVEP.AddString(_T("Method-1"));
	m_cmbVEP.AddString(_T("Method-2 & 3"));
	m_cmbVEP.SetCurSel(0);

	m_cmbNVEP.ResetContent();
	m_cmbNVEP.AddString(_T("Aproximate"));
	m_cmbNVEP.AddString(_T("Accurate"));
	m_cmbNVEP.SetCurSel(0);

	m_aCtrlCodeBased.Add(IDC_CMD_BD_21_PFMV_STC3);
	m_aCtrlCodeBased.Add(IDC_CMD_BD_21_PFMV_STC4);
	m_aCtrlCodeBased.Add(IDC_CMD_TMH7_VEP_CMB);
	m_aCtrlCodeBased.Add(IDC_CMD_TMH7_NVEP_CMB);

	m_aCtrlUserDefined.Add(IDC_CMD_SP35_13330_2011_MAX_STC);
	m_aCtrlUserDefined.Add(IDC_CMD_SP35_13330_2011_MIN_STC);
	m_aCtrlUserDefined.Add(IDC_CMD_SP35_13330_2011_BOTH_STC);
	m_aCtrlUserDefined.Add(IDC_GRV_LF1_MAX_RDO);
	m_aCtrlUserDefined.Add(IDC_GRV_LF1_MIN_RDO);
	m_aCtrlUserDefined.Add(IDC_GRV_LF1_BOTH_RDO);
	m_aCtrlUserDefined.Add(IDC_GRV_LF2_MAX_RDO);
	m_aCtrlUserDefined.Add(IDC_GRV_LF2_MIN_RDO);
	m_aCtrlUserDefined.Add(IDC_GRV_LF2_BOTH_RDO);
	m_aCtrlUserDefined.Add(IDC_GRV_LOAD_LF_STC);
	m_aCtrlUserDefined.Add(IDC_GRV_LOAD_LF_STC2);


	m_aCtrlLongBrkTra.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LC_STC);
	/*m_aCtrlLongBrkTra.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LC_CMB);*/
	m_aCtrlLongBrkTra.Add(IDC_CMD_TMH7_SEIS_LCOM_LL_LC_CMB);
	m_aCtrlLongBrkTra.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LF_STC);
	m_aCtrlLongBrkTra.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LF_EDT);
	/*m_aCtrlLongBrkTra.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LST);*/
	m_aCtrlLongBrkTra.Add(IDC_CMD_TMH7_SEIS_LCOM_LL_LST);
	m_aCtrlLongBrkTra.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_ADD_BTN);
	//m_aCtrlLongBrkTra.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_MOD_BTN);
	m_aCtrlLongBrkTra.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_DEL_BTN);

	CString aTitle_eff[] = { _LS(IDS_WG_CMD__ADDD__Load_Case), _LS(IDS_WG_CMD__ADDD__Factor) };
	int nColWidth_eff[] = { 105, 70 };
	SetListCtrlHeader(m_lstEffLoad, 2, aTitle_eff, nColWidth_eff);
}


void CCmdAutoLoadCombCvlTMH7Page::InitMVCombo()
{
    //Eurocode 하중조합에서 하는듯이 BS 차량만 하중조합에 추가하도록 남승규 대리님과 협의 BS 를 제외한 나머지 부분들을 nCount = 0 으로 처리함 

    m_MoveLcase.ResetContent();
	m_cmbEffLoadCase.ResetContent();

    m_MoveType.ResetContent();
	m_MoveType.AddString(_T("NA"));
	m_MoveType.AddString(_T("NB"));
	m_MoveType.AddString(_T("NC + 2/3 NA"));

    //m_MoveLcase.AddString(_LS(IDS_WG_CMD__ADDD__None));

    CArray<T_MVLD_K, T_MVLD_K> KeyList; /*KeyList.RemoveAll();*/
    CArray<T_MVLDjp_K, T_MVLDjp_K> KeyListjp; /*KeyListjp.RemoveAll();*/
    CArray<T_MVLDch_K, T_MVLDch_K> KeyListch; /*KeyListch.RemoveAll();*/
    CArray<T_MVLDid_K, T_MVLDid_K> KeyListid; /*KeyListid.RemoveAll();*/
    CArray<T_MVLDbs_K, T_MVLDbs_K> KeyListbs; /*KeyListbs.RemoveAll();*/
	CArray<T_MVLDbs_K, T_MVLDbs_K> KeyListfr; /*KeyListfr.RemoveAll();*/
    CArray<T_MVLDtr_K, T_MVLDtr_K> KeyListtr; /*KeyListtr.RemoveAll();*/

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

    if(nCount > 0)
    {
        for(int i=0; i<nCount; i++)
        {      
            T_MVLD_D   rData;
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
                key = KeyListfr.GetAt(i);   bCheck = m_pDoc->m_pAttrCtrl->GetMvldfr(key,rDatafr);
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
            // 코드에 따른 방식으로 수정 
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

            m_nBD21MoveCase = DataMvcd.nCodeType;

            if (rDatabs.bAutoLiveLoadComb == TRUE)
            {
                m_MoveType.SetCurSel(4);
                GetDlgItem(IDC_CMD_BD_21_MV_TYPE_CMB)->EnableWindow(FALSE);
            }
            else
            {
                m_MoveType.SetCurSel(0);
                GetDlgItem(IDC_CMD_BD_21_MV_TYPE_CMB)->EnableWindow(TRUE);
            }

            //if (rDatabs.nLoadModel != 1)
            //{
            int nIndex = m_MoveLcase.AddString(str);
			m_MoveLcase.SetItemData(nIndex, key);
            //}
        }
        Show_MoveComboBox(TRUE);
    }
    else
    {
        Show_MoveComboBox(FALSE);
    }
    m_MoveLcase.SetCurSel(0);
    m_MoveType.SetCurSel(0);

	//For Adding Earth Surcharge Case
	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();
	BOOL bDcExist = FALSE;
	BOOL bDdExist = FALSE;
	BOOL bDwExist = FALSE;
	BOOL bEhExist = FALSE;
	BOOL bEvExist = FALSE;
	BOOL bEsExist = FALSE;
	BOOL bLExist = FALSE;
	BOOL bTExist = FALSE;
	BOOL bLSExist = FALSE;
	BOOL bBrkExist = FALSE;
	BOOL bBkExist = FALSE;
	BOOL bStlExist = FALSE;
	if (iCount != 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
			ASSERT(bCheck);

			if (rData.LoadCaseType == _T("D"))	 bDcExist = TRUE;  // General Dead Load
			else if (rData.LoadCaseType == _T("DC"))	 bDcExist = TRUE;  // Component and Attachments
			else if (rData.LoadCaseType == _T("DD"))	 bDdExist = TRUE;  // Downdrag
			else if (rData.LoadCaseType == _T("DW"))	 bDwExist = TRUE;  // Wearing Surfaces and Utilities
			else if (rData.LoadCaseType == _T("EP"))	 bEhExist = TRUE;  // General earth pressure load
			else if (rData.LoadCaseType == _T("EH"))	 bEhExist = TRUE;  // horizontal earth pressure load
			else if (rData.LoadCaseType == _T("EV"))	 bEvExist = TRUE;  // vertical earth pressure load
			else if (rData.LoadCaseType == _T("ES"))	 bEsExist = TRUE;  // Earth Surcharge      
			else if (rData.LoadCaseType == _T("L"))	 bLExist = TRUE;  // Live Load
			else if (rData.LoadCaseType == _T("T"))	 bTExist = TRUE;  // 온도변화, 크리프, 건조수축 하중
			else if (rData.LoadCaseType == _T("CR"))	 bTExist = TRUE;  // 온도변화, 크리프, 건조수축 하중
			else if (rData.LoadCaseType == _T("SH"))	 bTExist = TRUE;  // 온도변화, 크리프, 건조수축 하중
			else if (rData.LoadCaseType == _T("STL"))   bStlExist = TRUE;  // 부등침하 하중      
			else if (rData.LoadCaseType == _T("LS"))   bLSExist = TRUE;//Live Load SUrcharge
			else if (rData.LoadCaseType == _T("BRK"))   bBrkExist = TRUE;//Braking
			else if (rData.LoadCaseType == _T("BK"))   bBkExist = TRUE;//Braking

			if (bLSExist || bLExist)
			{
				int nIndex = m_MoveLcase.AddString(rData.LoadCaseName);
				m_MoveLcase.SetItemData(nIndex, iKey);
				bLSExist = FALSE;
                bLExist = FALSE;
			}

			if (bBrkExist || bBkExist)
			{
				int nIndex = m_cmbEffLoadCase.AddString(rData.LoadCaseName);
				m_cmbEffLoadCase.SetItemData(nIndex, iKey);
				bBrkExist = FALSE; bBkExist = FALSE;
				m_cmbEffLoadCase.SetCurSel(0);
			}
		}

	}

}

void CCmdAutoLoadCombCvlTMH7Page::Show_MoveComboBox(BOOL bCheck)
{
    m_MoveLcase.EnableWindow(bCheck);
    m_MoveType.EnableWindow(bCheck);
    m_MovingList.EnableWindow(bCheck);
    /*GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_ADD_BTN)->EnableWindow(bCheck);*/
    //GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_DEL_BTN)->EnableWindow(bCheck);  
}


void CCmdAutoLoadCombCvlTMH7Page::Initial_MVListBox()
{
    DWORD dwStyle;
    dwStyle = ListView_GetExtendedListViewStyle(m_MovingList.GetSafeHwnd());
    dwStyle |= LVS_EX_FULLROWSELECT; 
    ListView_SetExtendedListViewStyle(m_MovingList.GetSafeHwnd(),dwStyle);

    LV_COLUMN lvcolumn;
    CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Load_Case), _T("Vehicle Type")};

    CString title;
    int width[] = {80, 160};
    for(int i = 0; i < sizeof(aTitle) / sizeof(CString); i++)
    {
        title = aTitle[i];

        lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvcolumn.fmt      = LVCFMT_LEFT;
        lvcolumn.pszText  = title.GetBuffer(0);
        lvcolumn.iSubItem = i;
        lvcolumn.cx       = width[i];
        m_MovingList.InsertColumn(i,&lvcolumn);
        title.ReleaseBuffer();
    }
}

void CCmdAutoLoadCombCvlTMH7Page::SetDefault()
{
    //Earth Pressure Releaving
	m_edtEPS_Max.SetEditUnit(0.5);
	// Dead Load
    double dDMax = 1.20, dDMin = 1.05;
    m_edtBD21D_Max.SetEditUnit(dDMax, 2);
    m_edtBD21D_Min.SetEditUnit(dDMin, 2);

    // Superimposed Dead Load (Deck Surfacing)
    double dDWMax = 1.20, dDWMin = 1.05;
    m_edtBD21DW_Max.SetEditUnit(dDWMax, 2);
    m_edtBD21DW_Min.SetEditUnit(dDWMin, 2);

    // Superimposed Dead Load (Other Loads)
    double dDCMax = 1.20, dDCMin = 1.05;
    m_edtBD21DC_Max.SetEditUnit(dDCMax, 2);
    m_edtBD21DC_Min.SetEditUnit(dDCMin, 2);

    // Vertical EarthPressure
    double dEVMax = 1.20, dEVMin = 1.05;
    m_edtBD21EV_Max.SetEditUnit(dEVMax, 2);
    m_edtBD21EV_Min.SetEditUnit(dEVMin, 2);

    // Non-Vertical EarthPressure
    double dEHMax = 1.20, dEHMin = 1.05;
    m_edtBD21EH_Max.SetEditUnit(dEHMax, 2);
    m_edtBD21EH_Min.SetEditUnit(dEHMin, 2);

    // Gamma f3 for ULS
    double dGammaf3ULS = 1.1;
    m_edtBD21Gamma_ULS.SetEditUnit(dGammaf3ULS, 2);

    // Gamma f3 for SLS
    double dGammaf3SLS = 1.0;
    m_edtBD21Gamma_SLS.SetEditUnit(dGammaf3SLS, 2);

	CButton* poButton;
	poButton = (CButton*)(GetDlgItem(IDC_CMD_TMH7_CODE_BASED));
	poButton->SetCheck(TRUE);

	CButton* poButton_user;
	poButton_user = (CButton*)(GetDlgItem(IDC_CMD_TMH7_USER_DEFINED));
	poButton_user->SetCheck(FALSE);

    CButton* poButtonMax;
    poButtonMax = (CButton*)(GetDlgItem(IDC_GRV_LF1_MAX_RDO));
    poButtonMax->SetCheck(TRUE);

	CButton* poButtonMaxSIDL;
	poButtonMaxSIDL = (CButton*)(GetDlgItem(IDC_GRV_LF2_MAX_RDO));
	poButtonMaxSIDL->SetCheck(TRUE);
}

void CCmdAutoLoadCombCvlTMH7Page::GetBD21LoadFactor()
{
    UpdateData(TRUE);

   // m_nBD21BridgeType = m_cmbBridgeType.GetCurSel();

    // Dead Load
    double dDMax = m_edtBD21D_Max.GetEditValue();
    double dDMin = m_edtBD21D_Min.GetEditValue();

    switch(m_nBD21D)
    {
    case 0: m_nBD21DNum = 1; m_dBD21DMax = dDMax; m_dBD21DMin = dDMax; break; 
    case 1: m_nBD21DNum = 1; m_dBD21DMax = dDMin; m_dBD21DMin = dDMin; break; 
    case 2: m_nBD21DNum = 2; m_dBD21DMax = dDMax; m_dBD21DMin = dDMin; break;
    }

    // Superimposed Dead Load (Deck Surfacing)
    double dDWMax = m_edtBD21DW_Max.GetEditValue();
    double dDWMin = m_edtBD21DW_Min.GetEditValue();

    switch(m_nBD21DW)
    {
    case 0: m_nBD21DWNum = 1; m_dBD21DWMax = dDWMax; m_dBD21DWMin = dDWMax; break; 
    case 1: m_nBD21DWNum = 1; m_dBD21DWMax = dDWMin; m_dBD21DWMin = dDWMin; break; 
    case 2: m_nBD21DWNum = 2; m_dBD21DWMax = dDWMax; m_dBD21DWMin = dDWMin; break;
    }

    // Superimposed Dead Load (Other Loads)
    double dDCMax = m_edtBD21DC_Max.GetEditValue();
    double dDCMin = m_edtBD21DC_Min.GetEditValue();

    switch(m_nBD21DC)
    {
    case 0: m_nBD21DCNum = 1; m_dBD21DCMax = dDCMax; m_dBD21DCMin = dDCMax; break; 
    case 1: m_nBD21DCNum = 1; m_dBD21DCMax = dDCMin; m_dBD21DCMin = dDCMin; break; 
    case 2: m_nBD21DCNum = 2; m_dBD21DCMax = dDCMax; m_dBD21DCMin = dDCMin; break;
    }

    // Vertical Earthpressure
    double dEVMax = m_edtBD21EV_Max.GetEditValue();
    double dEVMin = m_edtBD21EV_Min.GetEditValue();

    switch(m_nBD21EV)
    {
    case 0: m_nBD21EVNum = 1; m_dBD21EVMax = dEVMax; m_dBD21EVMin = dEVMax; break; 
    case 1: m_nBD21EVNum = 1; m_dBD21EVMax = dEVMin; m_dBD21EVMin = dEVMin; break; 
    case 2: m_nBD21EVNum = 2; m_dBD21EVMax = dEVMax; m_dBD21EVMin = dEVMin; break;
    }

    // Non-Vertical Earthpressure
    double dEHMax = m_edtBD21EH_Max.GetEditValue();
    double dEHMin = m_edtBD21EH_Min.GetEditValue();

    switch(m_nBD21EH)
    {
    case 0: m_nBD21EHNum = 1; m_dBD21EHMax = dEHMax; m_dBD21EHMin = dEHMax; break; 
    case 1: m_nBD21EHNum = 1; m_dBD21EHMax = dEHMin; m_dBD21EHMin = dEHMin; break; 
    case 2: m_nBD21EHNum = 2; m_dBD21EHMax = dEHMax; m_dBD21EHMin = dEHMin; break;
    }

    // Gammaf3 for ULS
    double dGammaf3ULS = m_edtBD21Gamma_ULS.GetEditValue();
    m_dBD21GF3ULS = dGammaf3ULS;
    // Gammaf3 for SLS
    double dGammaf3SLS = m_edtBD21Gamma_SLS.GetEditValue();
    m_dBD21GF3SLS = dGammaf3SLS;

	//((CButton*)GetDlgItem(IDC_CMD_TMH7_CODE_BASED))->SetCheck(TRUE);
	//THM7

	/*CButton* poButton;
	poButton = (CButton*)(GetDlgItem(IDC_CMD_TMH7_CODE_BASED));
	CButton* poButton1;
	poButton1 = (CButton*)(GetDlgItem(IDC_CMD_TMH7_USER_DEFINED));
	if (poButton->GetCheck() == 1) m_nCodeBasedOrUserDef = 0;
	else m_nCodeBasedOrUserDef = 1;*/

	//CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_CMD_TMH7_NVEP_CMB);
	//int nIndex = pComboBox->GetCurSel();
	m_nCodeBasedOrUserDef = m_nTMH7_Code_based;

	if (m_nCodeBasedOrUserDef == 0)
	{
		m_nCodeBasedVerticalEarthPressure = m_cmbVEP.GetCurSel();

		m_nCodeBasedNonVerticalEarthPressure = m_cmbNVEP.GetCurSel();
	}
	else
	{
		CButton* poButtonMax;
		poButtonMax = (CButton*)(GetDlgItem(IDC_GRV_LF1_MAX_RDO));
		CButton* poButtonMin;
		poButtonMin = (CButton*)(GetDlgItem(IDC_GRV_LF1_MIN_RDO));
		CButton* poButtonBoth;
		poButtonBoth = (CButton*)(GetDlgItem(IDC_GRV_LF1_BOTH_RDO));

        if (poButtonMax->GetCheck() == 1) {
            m_dUserDefDLFactor_Max = m_edtBD21D_Max.GetEditValue();
            m_dUserRdo_Dmax = 1;
        }
        else if (poButtonMin->GetCheck() == 1) {
            m_dUserDefDLFactor_Min = m_edtBD21D_Min.GetEditValue();
            m_dUserRdo_Dmin = 1;
        }
        else if (poButtonBoth->GetCheck() == 1) {
            m_dUserDefDLFactor_Max = m_edtBD21D_Max.GetEditValue();
            m_dUserDefDLFactor_Min = m_edtBD21D_Min.GetEditValue();
            m_dUserRdo_Dboth = 1;
        }
		m_dUserDefDL_Max = m_dUserDefDLFactor_Max;
        m_dUserDefDL_Min = m_dUserDefDLFactor_Min;
		CButton* poButtonMax_SIDL;
		poButtonMax_SIDL = (CButton*)(GetDlgItem(IDC_GRV_LF2_MAX_RDO));
		CButton* poButtonMin_SIDL;
		poButtonMin_SIDL = (CButton*)(GetDlgItem(IDC_GRV_LF2_MIN_RDO));
		CButton* poButtonBoth_SIDL;
		poButtonBoth_SIDL = (CButton*)(GetDlgItem(IDC_GRV_LF2_BOTH_RDO));
        if (poButtonMax_SIDL->GetCheck() == 1) {
            m_dUserDefSIDLFactor_Max = m_edtBD21DW_Max.GetEditValue();
            m_dUserRdo_SIDLmax = 1;
        }
        else if (poButtonMin_SIDL->GetCheck() == 1) {
            m_dUserDefSIDLFactor_Min = m_edtBD21DW_Min.GetEditValue();
            m_dUserRdo_SIDLmin = 1;
        }
        else if (poButtonBoth_SIDL->GetCheck() == 1) {
            m_dUserDefSIDLFactor = m_edtBD21DW_Max.GetEditValue() + m_edtBD21DW_Min.GetEditValue();
            m_dUserDefSIDLFactor_Max = m_edtBD21DW_Max.GetEditValue();
            m_dUserDefSIDLFactor_Min = m_edtBD21DW_Min.GetEditValue();
            m_dUserRdo_SIDLboth = 1;
        }
        m_dUserDefSIDL_Max = m_dUserDefSIDLFactor_Max;
        m_dUserDefSIDL_Min = m_dUserDefSIDLFactor_Min;

	}

	m_dEPRelieving = m_edtEPS_Max.GetEditValue();
}

void CCmdAutoLoadCombCvlTMH7Page::OnCmdBD21EnableDisable()
{  
    CArray <unsigned int, unsigned int> arMLoad;   // Moving Load

    CArray<T_STLD_K, T_STLD_K> arKeyList;
    m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
    int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();

    BOOL bDExist  = FALSE;
    BOOL bDWExist = FALSE;
    BOOL bDCExist = FALSE;
    BOOL bEVExist = FALSE;
    BOOL bEHExist = FALSE;

    BOOL bMovExist= FALSE;
    BOOL bLExist  = FALSE;

    // 항상 활성화 하도록 수정 
    bDExist  = TRUE;
    bDWExist = TRUE;
    bDCExist = TRUE;
    bEVExist = TRUE;
    bEHExist = TRUE;

    bMovExist= TRUE;

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
        m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMLoad);
        break;
    case D_MOVE_CODE_CHINA:
        m_pDoc->m_pAttrCtrl->GetMvldchKeyList(arMLoad);
        break;
    case D_MOVE_CODE_JAPAN:
        m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(arMLoad);
        break;
    case D_MOVE_CODE_INDIA:
        m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arMLoad);
        break;
    case D_MOVE_CODE_BS:
        m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(arMLoad);
        break;
	case D_MOVE_CODE_FRANCE:
		m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(arMLoad);
		break;
    case D_MOVE_CODE_TRANS:
        m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(arMLoad);
        break;
    default:
        ASSERT(0);
        break;
    }
    m_nBD21MoveCase = DataMvcd.nCodeType;

    if(arMLoad.GetSize() > 0)  { bLExist = TRUE; bMovExist = TRUE; }


    //GetDlgItem(IDC_CMD_BD_21_D_MAX_RDO )->EnableWindow(bDExist);
    //GetDlgItem(IDC_CMD_BD_21_D_MAX_EDT )->EnableWindow(bDExist);
    //GetDlgItem(IDC_CMD_BD_21_D_MIN_RDO )->EnableWindow(bDExist);
    //GetDlgItem(IDC_CMD_BD_21_D_MIN_EDT )->EnableWindow(bDExist);
    //GetDlgItem(IDC_CMD_BD_21_D_BOTH_RDO)->EnableWindow(bDExist);

    //GetDlgItem(IDC_CMD_BD_21_DW_MAX_RDO )->EnableWindow(bDWExist);
    //GetDlgItem(IDC_CMD_BD_21_DW_MAX_EDT )->EnableWindow(bDWExist);
    //GetDlgItem(IDC_CMD_BD_21_DW_MIN_RDO )->EnableWindow(bDWExist);
    //GetDlgItem(IDC_CMD_BD_21_DW_MIN_EDT )->EnableWindow(bDWExist);
    //GetDlgItem(IDC_CMD_BD_21_DW_BOTH_RDO)->EnableWindow(bDWExist);

    //GetDlgItem(IDC_CMD_BD_21_DC_MAX_RDO )->EnableWindow(bDCExist);
    //GetDlgItem(IDC_CMD_BD_21_DC_MAX_EDT )->EnableWindow(bDCExist);
    //GetDlgItem(IDC_CMD_BD_21_DC_MIN_RDO )->EnableWindow(bDCExist);
    //GetDlgItem(IDC_CMD_BD_21_DC_MIN_EDT )->EnableWindow(bDCExist);
    //GetDlgItem(IDC_CMD_BD_21_DC_BOTH_RDO)->EnableWindow(bDCExist);

    //GetDlgItem(IDC_CMD_BD_21_EV_MAX_RDO )->EnableWindow(bEVExist);
    //GetDlgItem(IDC_CMD_BD_21_EV_MAX_EDT )->EnableWindow(bEVExist);
    //GetDlgItem(IDC_CMD_BD_21_EV_MIN_RDO )->EnableWindow(bEVExist);
    //GetDlgItem(IDC_CMD_BD_21_EV_MIN_EDT )->EnableWindow(bEVExist);
    //GetDlgItem(IDC_CMD_BD_21_EV_BOTH_RDO)->EnableWindow(bEVExist);

    //GetDlgItem(IDC_CMD_BD_21_EH_MAX_RDO )->EnableWindow(bEHExist);
    //GetDlgItem(IDC_CMD_BD_21_EH_MAX_EDT )->EnableWindow(bEHExist);
    //GetDlgItem(IDC_CMD_BD_21_EH_MIN_RDO )->EnableWindow(bEHExist);
    //GetDlgItem(IDC_CMD_BD_21_EH_MIN_EDT )->EnableWindow(bEHExist);
    //GetDlgItem(IDC_CMD_BD_21_EH_BOTH_RDO)->EnableWindow(bEHExist);

    //GetDlgItem(IDC_CMD_BD_21_MVL_CMB	)->EnableWindow(bMovExist);
    //GetDlgItem(IDC_CMD_BD_21_MVL_ADD_BTN)->EnableWindow(bMovExist);
    //GetDlgItem(IDC_CMD_BD_21_MVL_DEL_BTN)->EnableWindow(bMovExist);
    //GetDlgItem(IDC_CMD_BD_21_MVL_LST	)->EnableWindow(bMovExist);

    GetDlgItem(IDC_CMD_BD_21_LOAD_FACTOR_GRP)->EnableWindow(bMovExist);

	//((CButton*)GetDlgItem(IDC_CMD_TMH7_CODE_BASED))->SetCheck(TRUE);
 //   ((CButton*)GetDlgItem(IDC_CMD_TMH7_USER_DEFINED))->SetCheck(FALSE);
    // Disable the four EDITTEXT controls
   /* GetDlgItem(IDC_GRV_LF1_MAX_EDIT)->EnableWindow(FALSE);
    GetDlgItem(IDC_GRV_LF2_MAX_EDIT)->EnableWindow(FALSE);
    GetDlgItem(IDC_GRV_LF2_MIN_EDIT)->EnableWindow(FALSE);
    GetDlgItem(IDC_GRV_LF1_MIN_EDIT)->EnableWindow(FALSE);*/
}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlTMH7Page message handlers
BOOL CCmdAutoLoadCombCvlTMH7Page::OnInitDialog() 
{
    CChildDialog::OnInitDialog();

    /*m_chkLoadFactor.InitControl(this, CCmdAutoLoadCombCvlTMH7Page::IDD, IDC_CMD_BD_21_LOAD_FACTOR_GRP,TRUE,TRUE);*/
    m_chkLongBrkTrc.InitControl(this, CCmdAutoLoadCombCvlTMH7Page::IDD, IDC_CMD_BD_21_PFMV_GRP, TRUE, TRUE);

    InitCombo();
    SetDefault();

    InitMVCombo();
    Initial_MVListBox();
	OnCmdLongBrkTrc();
    UpdateData(FALSE);
    m_wndOthoChk.SetCheck(0);
    //GetBD21LoadFactor();

    //OnCmdBD21Brdge();

    //OnCmdBD21MV();

    //OnCmdBD21EnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화

    return TRUE;
}

BOOL CCmdAutoLoadCombCvlTMH7Page::CheckMovingCaseName(CString str,int& Index)
{
    BOOL bCheck = TRUE;
    int nCount = m_MovingList.GetItemCount();
    for(int i=0; i<nCount; i++)
    {
        CString strLcName = m_MovingList.GetItemText(i, 0);
        if(str == strLcName)  
        {
            Index = i;
            bCheck = FALSE;
        }
    }
    return bCheck;
}


void CCmdAutoLoadCombCvlTMH7Page::OnCmdBD21MVAddBtn()
{
    UpdateData(TRUE);

    int nTrailer = 1;  // 평판과차 여대차
    int nBSVehicle = 2;  // BS 전용 차량

    //  Moving Load Case Data의 순번을 얻어옵니다.
    int nKey = 0;
    int nKeyVehicle = 0;
    int ListNo;
    int Index = m_MoveLcase.GetCurSel();
	/*CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();*/

    if(Index != -1)
    {
        int IndexMVType = m_MoveType.GetCurSel();

        CString strName=_T("");
        CString strMVName = _T("");
        m_MoveLcase.GetLBText(Index,strName);
        m_MoveType.GetLBText(IndexMVType,strMVName);
        if(CheckMovingCaseName(strName, ListNo))  //  TRUE=Add
        {      
            // 코드에 따른 방식으로 수정 
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
                nKey = m_pDoc->m_pAttrCtrl->GetMvldKey(strName);
                break;
            case D_MOVE_CODE_CHINA:
                nKey = m_pDoc->m_pAttrCtrl->GetMvldchKey(strName);
                break;
            case D_MOVE_CODE_JAPAN:
                nKey = m_pDoc->m_pAttrCtrl->GetMvldjpKey(strName);
                break;
            case D_MOVE_CODE_INDIA:
                nKey = m_pDoc->m_pAttrCtrl->GetMvldidKey(strName);
                break;
            case D_MOVE_CODE_BS:
                nKey = m_pDoc->m_pAttrCtrl->GetMvldbsKey(strName);
                break;
			case D_MOVE_CODE_FRANCE:
				nKey = m_pDoc->m_pAttrCtrl->GetMvldfrKey(strName);
				break;
            case D_MOVE_CODE_TRANS:
                nKey = m_pDoc->m_pAttrCtrl->GetMvldtrKey(strName);
                break;
            default:
                ASSERT(0);
                break;
            }
            bool bIsStldKey = false;
            if (nKey == 0) {
                nKey = m_pDoc->m_pAttrCtrl->GetStldKey(strName);
                bIsStldKey = true;
            }
            

            nBSVehicle = IndexMVType + 2; //2,3,4,5

            if (nKey != 0) {
                if (bIsStldKey) {
                    m_pParent->m_lsdata.SetAt(nKey, nBSVehicle);
                    m_pParent->m_MovingData.SetAt(nKey, nTrailer);
                    m_pParent->m_MovingData.SetAt(nKey, nBSVehicle);// Store in m_lsdata
                }
                else {
                    m_pParent->m_MovingData.SetAt(nKey, nTrailer);
                    m_pParent->m_MovingData.SetAt(nKey, nBSVehicle);
                }
            }
           
           
			//for (int i = 0; i < arKeyList.GetSize(); ++i)
			//{
			//	UINT stldKey = arKeyList[i]; // Since it's just a UINT
			//	m_pParent->m_MovingData.SetAt(stldKey, 1); // Or any value you want
			//}

            int No = m_MovingList.GetItemCount();
            //Write_RTMoveListBox(No, strName);

            LV_ITEM lvitem;
            LPTSTR szText[2];
            szText[0] = (LPTSTR)(LPCTSTR)strName;
            szText[1] = (LPTSTR)(LPCTSTR)strMVName;
            // Input ListBox.
            for(int i=0; i<2; i++)
            {
                lvitem.mask		  = LVIF_TEXT;
                lvitem.iItem  	= No;
                lvitem.iSubItem = i;
                lvitem.pszText	= szText[i];
                if(i==0)	m_MovingList.InsertItem(&lvitem);
                else		m_MovingList.SetItem(&lvitem);
            }
			T_MVLD_K MVLDKey = m_pDoc->m_pAttrCtrl->GetMvldKeyNew(strName);
			m_aMvldType.SetAt(MVLDKey, IndexMVType);
            if (MVLDKey == 0) {
				T_MVLD_K STLDKey = m_pDoc->m_pAttrCtrl->GetStldKey(strName);
				m_aMvldType.SetAt(STLDKey, IndexMVType);
            }
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

void CCmdAutoLoadCombCvlTMH7Page::OnCmdBD21MVDelBtn()
{
    int nItem = m_MovingList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
    if(nItem == -1)
    {
        AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
        return;
    }
    CString strName=_T("");
    strName = m_MovingList.GetItemText(nItem, 0);

    int nKey = 0;
    bool bIsStldKey = false;
    // 코드에 따른 방식으로 수정 
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
        nKey = m_pDoc->m_pAttrCtrl->GetMvldKey(strName);
        break;
    case D_MOVE_CODE_CHINA:
        nKey = m_pDoc->m_pAttrCtrl->GetMvldchKey(strName);
        break;
    case D_MOVE_CODE_JAPAN:
        nKey = m_pDoc->m_pAttrCtrl->GetMvldjpKey(strName);
        break;
    case D_MOVE_CODE_INDIA:
        nKey = m_pDoc->m_pAttrCtrl->GetMvldidKey(strName);
        break;
    case D_MOVE_CODE_BS:
        nKey = m_pDoc->m_pAttrCtrl->GetMvldbsKey(strName);
        break;
	case D_MOVE_CODE_FRANCE:
		nKey = m_pDoc->m_pAttrCtrl->GetMvldfrKey(strName);
		break;
    case D_MOVE_CODE_TRANS:
        nKey = m_pDoc->m_pAttrCtrl->GetMvldtrKey(strName);
        break;
    default:
        ASSERT(0);
        break;
    }
    if (nKey == 0) {
        nKey = m_pDoc->m_pAttrCtrl->GetStldKey(strName);
        bIsStldKey = true;
    }

    if (nKey == 0) return;
    if (bIsStldKey) {
        m_pParent->m_lsdata.RemoveKey(nKey);
    }
    else {
        m_pParent->m_MovingData.RemoveKey(nKey);
    }
    m_MovingList.DeleteItem(nItem);
	m_aMvldType.RemoveKey(nKey);
    int nCount = m_MovingList.GetItemCount();
    if (nCount <= nItem) nItem -= 1;
    if (nItem >= 0) 
        m_MovingList.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCmdAutoLoadCombCvlTMH7Page::OnCmdBD21Brdge()
{
    UpdateData(TRUE);
    //Pedestrian 이나 Cyclist에 대한 부분은 Vehicle 고려하지 않음
    m_nBD21BridgeType = m_cmbBridgeType.GetCurSel();
    GetDlgItem(IDC_CMD_BD_21_MVL_ADD_BTN)->EnableWindow(m_nBD21BridgeType==Highway);
    GetDlgItem(IDC_CMD_BD_21_MVL_DEL_BTN)->EnableWindow(m_nBD21BridgeType==Highway);
    GetDlgItem(IDC_CMD_BD_21_MVL_CMB)->EnableWindow(m_nBD21BridgeType==Highway);
    GetDlgItem(IDC_CMD_BD_21_MV_TYPE_CMB)->EnableWindow(m_nBD21BridgeType==Highway);
    GetDlgItem(IDC_CMD_TMH7_MVL_LST)->EnableWindow(m_nBD21BridgeType==Highway);

    GetDlgItem(IDC_CMD_BD_21_LOAD_FACTOR_GRP)->EnableWindow(m_nBD21BridgeType==Highway);
    int nCount = m_MovingList.GetItemCount();
    if (m_nBD21BridgeType == PedestrianCyclist)
    {
        COLORREF crTextColor = ::GetSysColor(COLOR_GRAYTEXT);
        m_MovingList.SetTextColor(crTextColor);
        m_MovingList.SetItemState(-1, 0, LVIS_SELECTED);
    }
    else
    {
        COLORREF crTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
        m_MovingList.SetTextColor(crTextColor);
    }
}

void CCmdAutoLoadCombCvlTMH7Page::OnCmdBD21MV()
{
    UpdateData(TRUE);

    if (m_nBD21MoveCase == D_MOVE_CODE_BS)
    {
        int Index = m_MoveLcase.GetCurSel() + 1;
        if(Index != -1)
        {
			int key = m_MoveLcase.GetItemData(m_MoveLcase.GetCurSel());
            T_MVLDbs_D rDatabs;
            m_pDoc->m_pAttrCtrl->GetMvldbs(key,rDatabs);
            if (rDatabs.bAutoLiveLoadComb == TRUE)
            {
                m_MoveType.ResetContent();
                m_MoveType.AddString(_T("NA"));
                m_MoveType.AddString(_T("NB"));
                m_MoveType.AddString(_T("NC + 2/3 NA"));
                
                m_MoveType.SetCurSel(0);
                GetDlgItem(IDC_CMD_BD_21_MV_TYPE_CMB)->EnableWindow(TRUE);
            }
            else
            {
				m_MoveType.ResetContent();
				m_MoveType.AddString(_T("NA"));
				m_MoveType.AddString(_T("NB"));
				m_MoveType.AddString(_T("NC + 2/3 NA"));

				m_MoveType.SetCurSel(0);
                GetDlgItem(IDC_CMD_BD_21_MV_TYPE_CMB)->EnableWindow(TRUE);
            }
        }
    }
}

void CCmdAutoLoadCombCvlTMH7Page::OnCmdBD21MVFactorBtn() 
{
    // TODO: Add your control notification handler code here
    CMovingFactorTMH7Dlg dlg;

    dlg.DoModal();
}


LRESULT CCmdAutoLoadCombCvlTMH7Page::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
    m_pParent->AlignControl();

    return 0L;
}


void CCmdAutoLoadCombCvlTMH7Page::OnCmdCodeOrUserRdo()
{
	UpdateData(TRUE);

	CButton* poButton;
	poButton = (CButton*)(GetDlgItem(IDC_CMD_TMH7_CODE_BASED));
    BOOL bCodeBasedSelected = (poButton->GetCheck() == 1);
	CButton* poButton1;
	poButton1 = (CButton*)(GetDlgItem(IDC_CMD_TMH7_USER_DEFINED));
	/*if (poButton->GetCheck() == 1)*/
    CDlgUtil::CtrlShowHide(this, m_aCtrlUserDefined, TRUE);
	/*(else CDlgUtil::CtrlShowHide(this, m_aCtrlCodeBased, FALSE); */
	//if (poButton1->GetCheck() == 1)	CDlgUtil::CtrlShowHide(this, m_aCtrlUserDefined, TRUE);
	//else CDlgUtil::CtrlShowHide(this, m_aCtrlUserDefined, FALSE);

	//SetLoadCombSelData();
	//Initial_Data();
	//AlignControl();

	//CString strCodeName = _T("");
	//int Index = m_Code.GetCurSel();
	//if (Index != -1)	m_Code.GetLBText(Index, strCodeName);

	//if (m_nDesignType == 0 || m_nDesignType == 1 || m_nDesignType == 3)
	//{
	//	if (strCodeName == _T("KSCE-LSD15")) GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(TRUE);
	//	else                            GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(FALSE);
	//}
	//else
	//{
	//	GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(FALSE);
	//}
	 // Get pointers to the EDITTEXT controls
	CWnd* pEditMax1 = GetDlgItem(IDC_GRV_LF1_MAX_EDIT);
	CWnd* pEditMax2 = GetDlgItem(IDC_GRV_LF2_MAX_EDIT);
	CWnd* pEditMin1 = GetDlgItem(IDC_GRV_LF1_MIN_EDIT);
	CWnd* pEditMin2 = GetDlgItem(IDC_GRV_LF2_MIN_EDIT);

	if (bCodeBasedSelected)
	{
		// Disable or hide the EDITTEXT controls
		pEditMax1->EnableWindow(FALSE);
		pEditMax2->EnableWindow(FALSE);
		pEditMin1->EnableWindow(FALSE);
		pEditMin2->EnableWindow(FALSE);

		GetDlgItem(IDC_GRV_LF1_MAX_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_GRV_LF1_MIN_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_GRV_LF1_BOTH_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_GRV_LF2_MAX_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_GRV_LF2_MIN_RDO)->EnableWindow(FALSE);
        GetDlgItem(IDC_GRV_LF2_BOTH_RDO)->EnableWindow(FALSE);
	}
	else
	{
		// Enable or show the EDITTEXT controls
		pEditMax1->EnableWindow(TRUE);
		pEditMax2->EnableWindow(TRUE);
		pEditMin1->EnableWindow(TRUE);
		pEditMin2->EnableWindow(TRUE);

		GetDlgItem(IDC_GRV_LF1_MAX_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_GRV_LF1_MIN_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_GRV_LF1_BOTH_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_GRV_LF2_MAX_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_GRV_LF2_MIN_RDO)->EnableWindow(TRUE);
        GetDlgItem(IDC_GRV_LF2_BOTH_RDO)->EnableWindow(TRUE);
	}
}


void CCmdAutoLoadCombCvlTMH7Page::OnCmdLongBrkTrc()
{
	UpdateData(TRUE);

	CButton* poButton;
	poButton = (CButton*)(GetDlgItem(IDC_IRC6_LVARB_TRAFF_ROAD_RDO2));
	
    if (m_LongBrkTrcChk.GetCheck() == 1)
    {
        CDlgUtil::CtrlEnableDisable(this, m_aCtrlLongBrkTra, TRUE);
    }
    else
    {
        CDlgUtil::CtrlEnableDisable(this, m_aCtrlLongBrkTra, FALSE);
        m_aLongiBrakTra.RemoveAll(); // Clear the map when unchecked
    }


	/*if (poButton->GetCheck() == 1)	CDlgUtil::CtrlEnableDisable(this, m_aCtrlLongBrkTra, TRUE);
	else CDlgUtil::CtrlShowHide(this, m_aCtrlLongBrkTra, FALSE);*/
	

	//SetLoadCombSelData();
	//Initial_Data();
	//AlignControl();

	//CString strCodeName = _T("");
	//int Index = m_Code.GetCurSel();
	//if (Index != -1)	m_Code.GetLBText(Index, strCodeName);

	//if (m_nDesignType == 0 || m_nDesignType == 1 || m_nDesignType == 3)
	//{
	//	if (strCodeName == _T("KSCE-LSD15")) GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(TRUE);
	//	else                            GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(FALSE);
	//}
	//else
	//{
	//	GetDlgItem(IDC_CMD_LOADCOMB_SEL_BTN)->EnableWindow(FALSE);
	//}
}


void CCmdAutoLoadCombCvlTMH7Page::OnEffLoadAddBtn()
{
	UpdateData();
	int Index = m_cmbEffLoadCase.GetCurSel();
	if (Index != -1)
	{
		int ListNo = 0;
		CString strName = _T("");
		m_cmbEffLoadCase.GetLBText(Index, strName);
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
			lvitem.iItem = m_lstEffLoad.GetItemCount();
			lvitem.iSubItem = IDX_NAME1;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strName;
			m_lstEffLoad.InsertItem(&lvitem);

			lvitem.iSubItem = IDX_FACTOR1;
			lvitem.mask = LVIF_TEXT;
			CString strFactor;
			strFactor.Format(_T("%g"), m_dEffLoadFactor);
			lvitem.pszText = (LPTSTR)(LPCTSTR)strFactor;
			m_lstEffLoad.SetItem(&lvitem);

			T_STLD_K StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strName);
			m_aLongiBrakTra.SetAt(StldK, m_dEffLoadFactor);
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

void CCmdAutoLoadCombCvlTMH7Page::OnEffLoadModBtn()
{
	UpdateData();
	int Index = m_cmbEffLoadCase.GetCurSel();
	if (Index != -1) return;

	int ListNo = 0;
	CString strName = _T("");
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
	lvitem.iItem = iItem;
	lvitem.iSubItem = IDX_NAME1;
	lvitem.pszText = (LPTSTR)(LPCTSTR)strName;
	m_lstEffLoad.SetItem(&lvitem);

	lvitem.iSubItem = IDX_FACTOR1;
	lvitem.mask = LVIF_TEXT;
	CString strFactor;
	strFactor.Format(_T("%g"), m_dEffLoadFactor);
	lvitem.pszText = (LPTSTR)(LPCTSTR)strFactor;
	m_lstEffLoad.SetItem(&lvitem);
}

void CCmdAutoLoadCombCvlTMH7Page::OnEffLoadDelBtn()
{
	int iItem = m_lstEffLoad.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}
	m_lstEffLoad.DeleteItem(iItem);

	int Index = m_cmbEffLoadCase.GetCurSel();
	if (Index == -1) return;
	CString strName = _T("");
	m_cmbEffLoadCase.GetLBText(Index, strName);
	T_STLD_K StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strName);

	m_aLongiBrakTra.RemoveKey(StldK);
}
void CCmdAutoLoadCombCvlTMH7Page::OnCmdUserDefDL()
{

	CButton* poButtonMax;
	poButtonMax = (CButton*)(GetDlgItem(IDC_GRV_LF1_MAX_RDO));
	CButton* poButtonMin;
	poButtonMin = (CButton*)(GetDlgItem(IDC_GRV_LF1_MIN_RDO));
	CButton* poButtonBoth;
	poButtonBoth = (CButton*)(GetDlgItem(IDC_GRV_LF1_BOTH_RDO));

	if (poButtonMax->GetCheck() == 1) m_dUserDefDLFactor = 1.20;
	else if (poButtonMin->GetCheck() == 1) m_dUserDefDLFactor = 1.05;
	else if (poButtonBoth->GetCheck() == 1) m_dUserDefDLFactor = 1.20 + 1.05;
	
}

void CCmdAutoLoadCombCvlTMH7Page::OnCmdUserDefSIDL()
{
	CButton* poButtonMax;
	poButtonMax = (CButton*)(GetDlgItem(IDC_GRV_LF2_MAX_RDO));
	CButton* poButtonMin;
	poButtonMin = (CButton*)(GetDlgItem(IDC_GRV_LF2_MIN_RDO));
	CButton* poButtonBoth;
	poButtonBoth = (CButton*)(GetDlgItem(IDC_GRV_LF2_BOTH_RDO));
	if (poButtonMax->GetCheck() == 1) m_dUserDefSIDLFactor = 1.20;
	else if (poButtonMin->GetCheck() == 1) m_dUserDefSIDLFactor = 1.05;
	else if (poButtonBoth->GetCheck() == 1) m_dUserDefSIDLFactor = 1.20 + 1.05;

}
void CCmdAutoLoadCombCvlTMH7Page::SetListCtrlHeader(CListCtrl& list, const int nColNum, const CString* aTitle, const int* nColWidth)
{
	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(list.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(list.GetSafeHwnd(), dwStyle);

	// Set Title
	for (int i = 0; i < nColNum; i++)
	{
		CString title = aTitle[i];
		LV_COLUMN lvcolumn;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = i == IDX_NAME1 ? LVCFMT_LEFT : LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = nColWidth[i];
		lvcolumn.pszText = title.GetBuffer(0);
		list.InsertColumn(i, &lvcolumn);
		list.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCmdAutoLoadCombCvlTMH7Page::OnCmdOthoChk()
{
	BOOL bCheck = m_wndOthoChk.GetCheck();
	GetDlgItem(IDC_CMD_TMH7_OTHOLC_SET_BTN)->EnableWindow(bCheck);

}

void CCmdAutoLoadCombCvlTMH7Page::OnCmdSetOtholcBtn()
{
	CCmdSetOrthoLC_C3_Dlg dlg;
	dlg.SetData(m_nOrthoCombType, m_a100_30_Load);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_nOrthoCombType, m_a100_30_Load);
	}
}
void CCmdAutoLoadCombCvlTMH7Page::OnBnClickedLbTractionBtn()
{
    // Create and show the dialog box
   /* CDialog lbTractionDlg(IDD_LB_TRACTION_DIALOG, this);
    lbTractionDlg.DoModal();*/
}