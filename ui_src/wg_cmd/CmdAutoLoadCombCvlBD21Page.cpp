// CmdAutoLoadCombCvlSP35133302011Page.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlBD21Page.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "CmdAutoLoadCombCvlHelpDlg.h"
#include "LoadCombCtrl.h"


#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "CmdMovingFactorBSDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


enum { Highway = 0, PedestrianCyclist = 1};

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlBD21Page dialog
CCmdAutoLoadCombCvlBD21Page::CCmdAutoLoadCombCvlBD21Page(CWnd* pParent /*=NULL*/)
    : CChildDialog(CCmdAutoLoadCombCvlBD21Page::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCmdAutoLoadCombCvlBD21Page)
    //}}AFX_DATA_INIT
    m_pDoc = CDBDoc::GetDocPoint();

    m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

    m_nConstLoad = 0;

    m_nBD21D  = 0;
    m_nBD21DW = 0;
    m_nBD21DC = 0;
    m_nBD21EV = 0;
    m_nBD21EH = 0;



}

void CCmdAutoLoadCombCvlBD21Page::DoDataExchange(CDataExchange* pDX)
{
    CChildDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCmdAutoLoadCombCvlBD21Page)  
    DDX_Control(pDX, IDC_CMD_BD_21_BRIDGE_TYPE_CMB,	    m_cmbBridgeType);
    DDX_Radio  (pDX, IDC_CMD_BD_21_D_MAX_RDO,		    m_nBD21D);
    DDX_Control(pDX, IDC_CMD_BD_21_D_MAX_EDT,		    m_edtBD21D_Max);
    DDX_Control(pDX, IDC_CMD_BD_21_D_MIN_EDT,		    m_edtBD21D_Min);
    DDX_Radio  (pDX, IDC_CMD_BD_21_DW_MAX_RDO,		    m_nBD21DW);
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
    DDX_Control(pDX, IDC_CMD_BD_21_EH_MIN_EDT,		    m_edtBD21EH_Min);

    DDX_Control(pDX, IDC_CMD_BD_21_MV_TYPE_CMB,	        m_MoveType);

    DDX_Control(pDX, IDC_CMD_BD_21_MVL_CMB,             m_MoveLcase);
    DDX_Control(pDX, IDC_CMD_BD_21_MVL_LST,             m_MovingList);

    DDX_Control(pDX, IDC_CMD_BD_21_ULS_EDT,				m_edtBD21Gamma_ULS);
    DDX_Control(pDX, IDC_CMD_BD_21_SLS_EDT,				m_edtBD21Gamma_SLS);

    DDX_Control(pDX, IDC_CMD_BD_21_LOAD_FACTOR_GRP,	    m_chkLoadFactor);

}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlBD21Page, CChildDialog)
    //{{AFX_MSG_MAP(CCmdAutoLoadCombCvlBD21Page)
    //}}AFX_MSG_MAP
    ON_CBN_SELCHANGE(IDC_CMD_BD_21_BRIDGE_TYPE_CMB,     OnCmdBD21Brdge)
    ON_CBN_SELCHANGE(IDC_CMD_BD_21_MVL_CMB,             OnCmdBD21MV)
    ON_BN_CLICKED(IDC_CMD_BD_21_MVL_ADD_BTN,            OnCmdBD21MVAddBtn)
    ON_BN_CLICKED(IDC_CMD_BD_21_MVL_DEL_BTN,            OnCmdBD21MVDelBtn)
    ON_BN_CLICKED(IDC_CMD_BD_21_MVL_TABLE_BTN,          OnCmdBD21MVFactorBtn)

    ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()


void CCmdAutoLoadCombCvlBD21Page::InitCombo()
{
    m_cmbBridgeType.ResetContent();
    m_cmbBridgeType.AddString(_LS(IDS_CMD_LOAD_COMB_BRIDGE_TYPE_ROAD));
    m_cmbBridgeType.AddString(_LS(IDS_CMD_LOAD_COMB_BRIDGE_TYPE_PEDESTRIAN_CYCLIST));
    m_cmbBridgeType.SetCurSel(0);
}


void CCmdAutoLoadCombCvlBD21Page::InitMVCombo()
{
    //Eurocode 하중조합에서 하는듯이 BS 차량만 하중조합에 추가하도록 남승규 대리님과 협의 BS 를 제외한 나머지 부분들을 nCount = 0 으로 처리함 

    m_MoveLcase.ResetContent();

    m_MoveType.ResetContent();
    m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_HA));
    m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_HB));
    m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_HAHB));
    m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_PEDESTRIAN_CYCLIST));

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
}

void CCmdAutoLoadCombCvlBD21Page::Show_MoveComboBox(BOOL bCheck)
{
    m_MoveLcase.EnableWindow(bCheck);
    m_MoveType.EnableWindow(bCheck);
    m_MovingList.EnableWindow(bCheck);
    //GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_ADD_BTN)->EnableWindow(bCheck);
    //GetDlgItem(IDC_CMD_AS_5100_2017_FATI_MVL_DEL_BTN)->EnableWindow(bCheck);  
}


void CCmdAutoLoadCombCvlBD21Page::Initial_MVListBox()
{
    DWORD dwStyle;
    dwStyle = ListView_GetExtendedListViewStyle(m_MovingList.GetSafeHwnd());
    dwStyle |= LVS_EX_FULLROWSELECT; 
    ListView_SetExtendedListViewStyle(m_MovingList.GetSafeHwnd(),dwStyle);

    LV_COLUMN lvcolumn;
    CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Load_Case), _LS(IDS_CMD_MVHL_vehicle_name)};

    CString title;
    int width[] = {80, 160};
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

void CCmdAutoLoadCombCvlBD21Page::SetDefault()
{
    // Dead Load
    double dDMax = 1.20, dDMin = 1.15;
    m_edtBD21D_Max.SetEditUnit(dDMax, 2);
    m_edtBD21D_Min.SetEditUnit(dDMin, 2);

    // Superimposed Dead Load (Deck Surfacing)
    double dDWMax = 1.75, dDWMin = 1.2;
    m_edtBD21DW_Max.SetEditUnit(dDWMax, 2);
    m_edtBD21DW_Min.SetEditUnit(dDWMin, 2);

    // Superimposed Dead Load (Other Loads)
    double dDCMax = 1.2, dDCMin = 1.2;
    m_edtBD21DC_Max.SetEditUnit(dDCMax, 2);
    m_edtBD21DC_Min.SetEditUnit(dDCMin, 2);

    // Vertical EarthPressure
    double dEVMax = 1.2, dEVMin = 1.0;
    m_edtBD21EV_Max.SetEditUnit(dEVMax, 2);
    m_edtBD21EV_Min.SetEditUnit(dEVMin, 2);

    // Non-Vertical EarthPressure
    double dEHMax = 1.5, dEHMin = 1.0;
    m_edtBD21EH_Max.SetEditUnit(dEHMax, 2);
    m_edtBD21EH_Min.SetEditUnit(dEHMin, 2);

    // Gamma f3 for ULS
    double dGammaf3ULS = 1.1;
    m_edtBD21Gamma_ULS.SetEditUnit(dGammaf3ULS, 2);

    // Gamma f3 for SLS
    double dGammaf3SLS = 1.0;
    m_edtBD21Gamma_SLS.SetEditUnit(dGammaf3SLS, 2);

}

void CCmdAutoLoadCombCvlBD21Page::GetBD21LoadFactor()
{
    UpdateData(TRUE);

    m_nBD21BridgeType = m_cmbBridgeType.GetCurSel();

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

}

void CCmdAutoLoadCombCvlBD21Page::OnCmdBD21EnableDisable()
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


    GetDlgItem(IDC_CMD_BD_21_D_MAX_RDO )->EnableWindow(bDExist);
    GetDlgItem(IDC_CMD_BD_21_D_MAX_EDT )->EnableWindow(bDExist);
    GetDlgItem(IDC_CMD_BD_21_D_MIN_RDO )->EnableWindow(bDExist);
    GetDlgItem(IDC_CMD_BD_21_D_MIN_EDT )->EnableWindow(bDExist);
    GetDlgItem(IDC_CMD_BD_21_D_BOTH_RDO)->EnableWindow(bDExist);

    GetDlgItem(IDC_CMD_BD_21_DW_MAX_RDO )->EnableWindow(bDWExist);
    GetDlgItem(IDC_CMD_BD_21_DW_MAX_EDT )->EnableWindow(bDWExist);
    GetDlgItem(IDC_CMD_BD_21_DW_MIN_RDO )->EnableWindow(bDWExist);
    GetDlgItem(IDC_CMD_BD_21_DW_MIN_EDT )->EnableWindow(bDWExist);
    GetDlgItem(IDC_CMD_BD_21_DW_BOTH_RDO)->EnableWindow(bDWExist);

    GetDlgItem(IDC_CMD_BD_21_DC_MAX_RDO )->EnableWindow(bDCExist);
    GetDlgItem(IDC_CMD_BD_21_DC_MAX_EDT )->EnableWindow(bDCExist);
    GetDlgItem(IDC_CMD_BD_21_DC_MIN_RDO )->EnableWindow(bDCExist);
    GetDlgItem(IDC_CMD_BD_21_DC_MIN_EDT )->EnableWindow(bDCExist);
    GetDlgItem(IDC_CMD_BD_21_DC_BOTH_RDO)->EnableWindow(bDCExist);

    GetDlgItem(IDC_CMD_BD_21_EV_MAX_RDO )->EnableWindow(bEVExist);
    GetDlgItem(IDC_CMD_BD_21_EV_MAX_EDT )->EnableWindow(bEVExist);
    GetDlgItem(IDC_CMD_BD_21_EV_MIN_RDO )->EnableWindow(bEVExist);
    GetDlgItem(IDC_CMD_BD_21_EV_MIN_EDT )->EnableWindow(bEVExist);
    GetDlgItem(IDC_CMD_BD_21_EV_BOTH_RDO)->EnableWindow(bEVExist);

    GetDlgItem(IDC_CMD_BD_21_EH_MAX_RDO )->EnableWindow(bEHExist);
    GetDlgItem(IDC_CMD_BD_21_EH_MAX_EDT )->EnableWindow(bEHExist);
    GetDlgItem(IDC_CMD_BD_21_EH_MIN_RDO )->EnableWindow(bEHExist);
    GetDlgItem(IDC_CMD_BD_21_EH_MIN_EDT )->EnableWindow(bEHExist);
    GetDlgItem(IDC_CMD_BD_21_EH_BOTH_RDO)->EnableWindow(bEHExist);

    GetDlgItem(IDC_CMD_BD_21_MVL_CMB	)->EnableWindow(bMovExist);
    GetDlgItem(IDC_CMD_BD_21_MVL_ADD_BTN)->EnableWindow(bMovExist);
    GetDlgItem(IDC_CMD_BD_21_MVL_DEL_BTN)->EnableWindow(bMovExist);
    GetDlgItem(IDC_CMD_BD_21_MVL_LST	)->EnableWindow(bMovExist);

    GetDlgItem(IDC_CMD_BD_21_LOAD_FACTOR_GRP)->EnableWindow(bMovExist);

}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlBD21Page message handlers
BOOL CCmdAutoLoadCombCvlBD21Page::OnInitDialog() 
{
    CChildDialog::OnInitDialog();

    m_chkLoadFactor.InitControl(this, CCmdAutoLoadCombCvlBD21Page::IDD, IDC_CMD_BD_21_LOAD_FACTOR_GRP,TRUE,TRUE);

    InitCombo();
    SetDefault();

    InitMVCombo();
    Initial_MVListBox();

    UpdateData(FALSE);
    GetBD21LoadFactor();

    OnCmdBD21Brdge();

    OnCmdBD21MV();

    OnCmdBD21EnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화

    return TRUE;
}

BOOL CCmdAutoLoadCombCvlBD21Page::CheckMovingCaseName(CString str,int& Index)
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


void CCmdAutoLoadCombCvlBD21Page::OnCmdBD21MVAddBtn()
{
    UpdateData(TRUE);

    int nTrailer = 1;  // 평판과차 여대차
    int nBSVehicle = 2;  // BS 전용 차량

    //  Moving Load Case Data의 순번을 얻어옵니다.
    int nKey = 0;
    int nKeyVehicle = 0;
    int ListNo;
    int Index = m_MoveLcase.GetCurSel();

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

            nBSVehicle = IndexMVType + 2; //2,3,4,5

            if(nKey != 0) m_pParent->m_MovingData.SetAt(nKey, nTrailer); 
            if(nKey != 0) m_pParent->m_MovingData.SetAt(nKey, nBSVehicle); 

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

void CCmdAutoLoadCombCvlBD21Page::OnCmdBD21MVDelBtn()
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

    if (nKey == 0) return;
    m_pParent->m_MovingData.RemoveKey(nKey);
    m_MovingList.DeleteItem(nItem);
    int nCount = m_MovingList.GetItemCount();
    if (nCount <= nItem) nItem -= 1;
    if (nItem >= 0) 
        m_MovingList.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCmdAutoLoadCombCvlBD21Page::OnCmdBD21Brdge()
{
    UpdateData(TRUE);
    //Pedestrian 이나 Cyclist에 대한 부분은 Vehicle 고려하지 않음
    m_nBD21BridgeType = m_cmbBridgeType.GetCurSel();
    GetDlgItem(IDC_CMD_BD_21_MVL_ADD_BTN)->EnableWindow(m_nBD21BridgeType==Highway);
    GetDlgItem(IDC_CMD_BD_21_MVL_DEL_BTN)->EnableWindow(m_nBD21BridgeType==Highway);
    GetDlgItem(IDC_CMD_BD_21_MVL_CMB)->EnableWindow(m_nBD21BridgeType==Highway);
    GetDlgItem(IDC_CMD_BD_21_MV_TYPE_CMB)->EnableWindow(m_nBD21BridgeType==Highway);
    GetDlgItem(IDC_CMD_BD_21_MVL_LST)->EnableWindow(m_nBD21BridgeType==Highway);

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

void CCmdAutoLoadCombCvlBD21Page::OnCmdBD21MV()
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
                m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_HA));
                m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_HB));
                m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_HAHB));
                m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_PEDESTRIAN_CYCLIST));
                m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_AUTO));
                m_MoveType.SetCurSel(4);
                GetDlgItem(IDC_CMD_BD_21_MV_TYPE_CMB)->EnableWindow(FALSE);
            }
            else
            {
                m_MoveType.ResetContent();
                m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_HA));
                m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_HB));
                m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_HAHB));
                m_MoveType.AddString(_LS(IDS_CMD_BD21_MOVE_TYPE_PEDESTRIAN_CYCLIST));
                m_MoveType.SetCurSel(0);
                GetDlgItem(IDC_CMD_BD_21_MV_TYPE_CMB)->EnableWindow(TRUE);
            }
        }
    }
}

void CCmdAutoLoadCombCvlBD21Page::OnCmdBD21MVFactorBtn() 
{
    // TODO: Add your control notification handler code here
    CMovingFactorBSDlg dlg;

    dlg.DoModal();
}


LRESULT CCmdAutoLoadCombCvlBD21Page::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
    m_pParent->AlignControl();

    return 0L;
}
