#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_DESIGN_SPECT.h"

#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_dbLock\LockOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ETC_GDS_AASHTO_LRFD12.h"
#include "ETC_GDS_AS_5100_2_2017.h"
#include "ETC_GDS_CH_BRG89.h"	// added by KYM
#include "ETC_GDS_CH_CJJ166_2011.h" // added by maxiao
#include "ETC_GDS_CH_GB_T_51408_2021.h"
#include "ETC_GDS_CH_GBJ111_87.h" // added by KYM
#include "ETC_GDS_CH_JTG_T_2231_01_2020.h" //added by khj
#include "ETC_GDS_CH_JTJXXX_2008.h" // added by jhyun
#include "ETC_GDS_CH2002.h"		// added by KYM
#include "ETC_GDS_CH2010.h"
#include "ETC_GDS_CH2019.h"
#include "ETC_GDS_CHSH2003.h"
#include "ETC_GDS_DPT2018.h"
#include "ETC_GDS_DPWH_LRFD_2013.h"
#include "ETC_GDS_EURO1996.h"   // added by KYM
#include "ETC_GDS_EURO1996_ELA.h"
#include "ETC_GDS_EURO2003.h"
#include "ETC_GDS_GB50111_2006.h" // added by mylee
#include "ETC_GDS_IBC2000.h"	// added by KYM
#include "ETC_GDS_IBC2012.h"
#include "ETC_GDS_IRCSP114_2018.h"
#include "ETC_GDS_IS1893_2016.h"
#include "ETC_GDS_IS2002.h"
#include "ETC_GDS_JP_BRG2002.h"
#include "ETC_GDS_JP_BRG2012.h"
#include "ETC_GDS_JPN2000.h"	// added by KYM
#include "ETC_GDS_KBC2005.h"
#include "ETC_GDS_KBC2008.h"
#include "ETC_GDS_KBC2015.h"
#include "ETC_GDS_KDS2019.h"
#include "ETC_GDS_KISTEC2013.h"
#include "ETC_GDS_KS.h"
#include "ETC_GDS_KS_BRG.h"
#include "ETC_GDS_KS2000.h"
#include "ETC_GDS_KSCE2019.h"
#include "ETC_GDS_NBC95.h"
#include "ETC_GDS_NSR2010.h"
#include "ETC_GDS_NTC2018.h"
#include "ETC_GDS_P100_2013.h"
#include "ETC_GDS_TAIWAN22.h"
#include "ETC_GDS_TAIWAN02.h"
#include "ETC_GDS_TAIWAN86.h"
#include "ETC_GDS_TAIWAN86_BRG.h"
#include "ETC_GDS_TAIWAN86V.h"
#include "ETC_GDS_TAIWAN86V_BRG.h"
#include "ETC_GDS_TAIWAN98.h"
#include "ETC_GDS_UBC88.h"
#include "ETC_GDS_UBC97.h"
#include "ETC_GDS_UserPage.h"
#include "ETC_GDS_RUS_SP2016.h"
#include "ETC_GDS_RUS_SP14_2018.h"
#include "ETC_GDS_SANS.h"
#include "ETC_GDS_IS1893_2025.h"

#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

CETC_DESIGN_SPECT::CETC_DESIGN_SPECT(T_SPFC_D* pParamData, BOOL bModify, int nCase, BOOL bSeismicEvaluation, CWnd* pParent)
    : CDialogMove(CETC_DESIGN_SPECT::IDD, pParent)
{
    m_bSeismicEvaluation = bSeismicEvaluation;
    m_pDoc = CDBDoc::GetDocPoint();
    ASSERT(m_pDoc);
    m_bModify = bModify;
    m_aCtrlMethod.Add(IDOK);
    m_aCtrlMethod.Add(IDCANCEL);
    m_aCtrlMethod.Add(IDC_ETC_IMPORT_BTN);

    m_aCodeSeq.RemoveAll();
    m_aLockSeq.RemoveAll();
    m_aCodeName.RemoveAll();
    m_aCodeEnable.RemoveAll();
    m_mapChildDlg.RemoveAll();
    m_mapChildDlg.InitHashTable(40);

    m_nCase = nCase;
    if ( !m_bModify )
    {
        T_PREFERENCE rPref;
        rPref.Initialize();
        m_pDoc->m_pInitCtrl->GetPreference(rPref);
        if ( rPref.LoadCode.nRSNationalAnnex==2 )
        {
            pParamData->CodeParam.EURO2004.nGroundType = EN_SPFC_GROUND_B;
            pParamData->CodeParam.EURO2004.dI = 1.0;
            pParamData->CodeParam.EURO2004.nNationalAnnex = EN_SPFC_MALESIA;
        }
        else if ( rPref.LoadCode.nRSNationalAnnex==1 )
        {
            pParamData->CodeParam.EURO2004.nGroundType = EN_SPFC_GROUND_C;
            pParamData->CodeParam.EURO2004.dI = 1.4;
            pParamData->CodeParam.EURO2004.nNationalAnnex = EN_SPFC_SINGAPORE;
        }
    }

    m_pParamData = pParamData; // 20071203 mylee - SPFC 코드 여기서 저장하기 위해
    m_TempParamData = *m_pParamData;

    m_aCtrlSpectral.RemoveAll();
    m_aCtrlScaling.RemoveAll();

    m_nSpectralType = 0;

    memset(m_dMnMxXVal, 0, sizeof(m_dMnMxXVal));
    memset(m_dMnMxYVal, 0, sizeof(m_dMnMxYVal));

    // MNET:5310-HSSHIM-20121024
    m_parPeriod = NULL;
    m_parAccel = NULL;

    m_aMoveCtrlID.RemoveAll();
    m_aMoveCtrlID.Add(IDC_ETC_GDS_PLACEHOLDER);
    m_aMoveCtrlID.Add(IDC_ETC_IMPORT_BTN);
    m_aMoveCtrlID.Add(IDOK);
    m_aMoveCtrlID.Add(IDCANCEL);

    auto L_AddChildDlg = [ this ] (UINT nSeq, const CString& csCodeName, CMyChildDialog* pDialog)
    {
        if ( pDialog == nullptr ) { ASSERT(0); return; }
        if ( csCodeName.IsEmpty() ) { ASSERT(0); return; }

        CMyChildDialog* pDlgTest = nullptr;
        ASSERT(!m_mapChildDlg.Lookup(nSeq, pDlgTest));
        m_aCodeSeq.Add(nSeq);
        m_aCodeName.Add(csCodeName);
        m_mapChildDlg.SetAt(nSeq, pDialog);
    };

    //*^^* 여기추가 : Define Response Spectrum Function Code
    L_AddChildDlg(EN_CHILD_ORDER_KS,                 _T("Korea(Arch.1992)"),             new CETC_GDS_KS(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_KS2000,             _T("Korea(Arch.2000)"),             new CETC_GDS_KS2000(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_KS_BRG,             _T("Korea(Bridge)"),                new CETC_GDS_KS_BRG(pParamData, FALSE));
    L_AddChildDlg(EN_CHILD_ORDER_UBC88,              _T("UBC 88-94"),                    new CETC_GDS_UBC88(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_UBC97,              _T("UBC(1997)"),                    new CETC_GDS_UBC97(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_EURO1996,           _T("Eurocode-8(1996) Design"),      new CETC_GDS_EURO1996(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_CH2002,             _T("China(GB50011-2001)"),          new CETC_GDS_CH2002(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_IBC2000,            _T("IBC2000(ASCE7-98)"),            new CETC_GDS_IBC2000(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_JPN2000,            _T("Japan(Arch.2000)"),             new CETC_GDS_JPN2000(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_CH_BRG89,           _T("China(JTJ004-89)"),             new CETC_GDS_CH_BRG89(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_CH_GBJ111_87,       _T("China(GBJ11-87)"),              new CETC_GDS_CH_GBJ111_87(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_NBC95,              _T("NBC(1995)"),                    new CETC_GDS_NBC95(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_EURO1996_ELA,       _T("Eurocode-8(1996) Elastic"),     new CETC_GDS_EURO1996_ELA(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_IS2002,             _T("IS1893(2002)"),                 new CETC_GDS_IS2002(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_TAIWAN99H,          _T("Taiwan(1999) Horizontal"),      new CETC_GDS_TAIWAN86(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_TAIWAN99V,          _T("Taiwan(1999) Vertical"),        new CETC_GDS_TAIWAN86V(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_TAIWAN89H_BRG,      _T("TaiwanBrg(89) Horizontal"),     new CETC_GDS_TAIWAN86_BRG(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_TAIWAN89V_BRG,      _T("TaiwanBrg(89) Vertical"),       new CETC_GDS_TAIWAN86V_BRG(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_JP_BRG2002,         _T("Japan(Bridge2002)"),            new CETC_GDS_JP_BRG2002(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_KBC2005,            _T("KBC(2005)"),                    new CETC_GDS_KBC2005(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_CHSH2003,           _T("China Shanghai(DGJ08-9-2003)"), new CETC_GDS_CHSH2003(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_EURO2004,           _T("Eurocode-8(2004)"),             new CETC_GDS_EURO2003(pParamData, NULL, m_nCase)); // MNET:XXXX-GSJI-20060623 2003->2004 (김민정요청)
    L_AddChildDlg(EN_CHILD_ORDER_TAIWAN06,           _T("Taiwan(2006)"),                 new CETC_GDS_TAIWAN02(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_GB50111_2006,       _T("China(GB50111-2006)"),          new CETC_GDS_GB50111_2006(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_JTJXXX_2008,        _T("China(JTG/T B02-01-2008)"),     new CETC_GDS_CH_JTJXXX_2008(pParamData)); // MNET:3615-JHYUN-20080819
    L_AddChildDlg(EN_CHILD_ORDER_KBC2009,            _T("KBC(2009)"),                    new CETC_GDS_KBC2009(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_USER,               _LS(IDS_ETC_USER_TYPE),             new CETC_GDS_UserPage(pParamData, this));
    L_AddChildDlg(EN_CHILD_ORDER_TAIWANBRG98,        _T("TaiwanBrg(98)"),                new CETC_GDS_TAIWAN98(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_CH2010,             _T("China(GB/T50011-2010)"),          new CETC_GDS_CH2010(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_CJJ166_2011,        _T("China(CJJ 166-2011)"),          new CETC_GDS_CH_CJJ166_2011(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_IBC2009,            _T("IBC2009(ASCE7-05)"),            new CETC_GDS_IBC2012(pParamData, nullptr, D_SPFC_CODE_IBC2009));
    L_AddChildDlg(EN_CHILD_ORDER_IBC2012,            _T("IBC2012(ASCE7-10)"),            new CETC_GDS_IBC2012(pParamData, nullptr, D_SPFC_CODE_IBC2012));
    L_AddChildDlg(EN_CHILD_ORDER_AASHTO_LRFD12,      _T("AASHTO-LRFD6th(2012)"),         new CETC_GDS_AASHTO_LRFD12(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_NSR2010,            _T("NSR-10"),                       new CETC_GDS_NSR2010(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_KBC2015,            _T("KBC(2016)"),                    new CETC_GDS_KBC2015(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_P100_2013,          _T("P100-1(2013)"),                 new CETC_GDS_P100_2013(pParamData, NULL, m_nCase));
    L_AddChildDlg(EN_CHILD_ORDER_IS1893_2016,        _T("IS1893(2016)"),                 new CETC_GDS_IS1893_2016(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_KISTEC2013,         _T("KISTEC2013"),                   new CETC_GDS_KISTEC2013(pParamData)); // 현재 사용안되고 있음. CheckCodeAndErrorMsgST에서 걸러짐.
    L_AddChildDlg(EN_CHILD_ORDER_KSCE2019,           _T("KDS(17-10-00:2018)"),           new CETC_GDS_KSCE2019(pParamData, FALSE));
    L_AddChildDlg(EN_CHILD_ORDER_NTC2018,            _T("NTC2018"),                      new CETC_GDS_NTC2018(pParamData, NULL, m_nCase));
    L_AddChildDlg(EN_CHILD_ORDER_DPWH_LRFD_2013,     _T("DPWH-LRFD BSDS(2013)"),         new CETC_GDS_DPWH_LRFD_2013(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_KDS2019,            _T("KDS(41-17-00:2019)"),           new CETC_GDS_KDS2019(pParamData, FALSE));
    L_AddChildDlg(EN_CHILD_ORDER_AS_5100_2_2017,     _T("AS 5100.2(2017)"),              new CETC_GDS_AS_5100_2_2017(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_CH2019,             _T("China(GB50011-2019)"),          new CETC_GDS_CH2019(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_IRCSP114_2018,      _T("IRC:SP:114-2018"),              new CETC_GDS_IRCSP114_2018(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_JTG_T_2231_01_2020, _T("China(JTG/T 2231-01-2020)"),    new CETC_GDS_CH_JTG_T_2231_01_2020(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_JP_BRG2012,         _T("Japan(Bridge2012)"),            new CETC_GDS_JP_BRG2012(pParamData, 0));
    L_AddChildDlg(EN_CHILD_ORDER_JP_BRG2017,         _T("Japan(Bridge2017)"),            new CETC_GDS_JP_BRG2012(pParamData, 1));
    L_AddChildDlg(EN_CHILD_ORDER_DPT2018,            _T("DPT.1301/1302-61:2018"),        new CETC_GDS_DPT2018(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_CH_GB_T_51408_2021, _T("China(GB/T 51408-2021)"),		 new CETC_GDS_CH_GB_T_51408_2021(pParamData));
    L_AddChildDlg(EN_CHILD_ORDER_TAIWAN22,           _T("Taiwan(2022)"),                 new CETC_GDS_TAIWAN22(pParamData));
	L_AddChildDlg(EN_CHILD_ORDER_RUS_SP2016,         SPFC_RUS_SP_268_1325800,          new CETC_GDS_RUS_SP2016(pParamData, m_bModify));
    L_AddChildDlg(EN_CHILD_ORDER_NSCP_2024,          _T("NSCP 2024"),                    new CETC_GDS_IBC2012(pParamData, nullptr, D_SPFC_CODE_NSCP_2024));
    L_AddChildDlg(EN_CHILD_ORDER_SANS10160_4_2010,   _T("SANS10160-4:2010"),			 new CETC_GDS_SANS(pParamData, NULL, m_nCase));
	L_AddChildDlg(EN_CHILD_ORDER_RUS_SP14_2018,      SPFC_RUS_SP_14_13330,             new CETC_GDS_RUS_SP14_2018(pParamData, m_bModify));
	L_AddChildDlg(EN_CHILD_ORDER_IS1893_2025,        _T("IS1893(2025)"),                 new CETC_GDS_IS1893_2025(pParamData));
    CDBLib::GetRSCodeSeqList(m_aLockSeq);

    BOOL bGenCH = FALSE;
#if defined(_CH) && defined(_MGEN)
    bGenCH = TRUE;
#endif

    m_aCodeEnable.RemoveAll();
    CString csErrMsg("");
    int nCode = m_aLockSeq.GetSize();
    for ( int i = 0; i <nCode; i++ )
    {
        m_aCodeEnable.Add(TRUE);
    }
    if ( bGenCH )
    {
        for ( int i = nCode-1; i >= 0; i-- )
        {
            // 아래에서 각 국가별로 코드리스트를 재정리한다.
            if ( !CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLoadResponse, m_aLockSeq[i], csErrMsg) )
            {
                m_aCodeEnable.SetAt(i, FALSE);
            }
        }
    }
    else
    {
        for ( int i = nCode-1; i >= 0; i-- )
        {
            // 아래에서 각 국가별로 코드리스트를 재정리한다.
            if ( !CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLoadResponse, m_aLockSeq[i], csErrMsg) )
            {
                m_aLockSeq.RemoveAt(i);
                m_aCodeEnable.RemoveAt(i);
            }
        }
    }

    nCode = m_aLockSeq.GetSize();
    if ( m_nCase==1 ) // Pushover Curve 의 ATC-40, FEMA440 인경우 Eurocode의 Elastic Spectrum 삭제
    {
        for ( int i = nCode-1; i >= 0; i-- )
        {
            // 아래에서 각 국가별로 코드리스트를 재정리한다.
            if ( m_aLockSeq[i]==12 )
            {
                m_aLockSeq.RemoveAt(i);
                m_aCodeEnable.RemoveAt(i);
            }
        }
    }
    else if ( m_nCase==2 ) // Pushover Curve 의 EC8, NTC2008 인경우 Eurocode의 Design Spectrum 삭제
    {
        for ( int i = nCode-1; i >= 0; i-- )
        {
            // 아래에서 각 국가별로 코드리스트를 재정리한다.
            if ( m_aLockSeq[i]==5 )
            {
                m_aLockSeq.RemoveAt(i);
                m_aCodeEnable.RemoveAt(i);
            }
        }
    }

    // MNET:2138-GSJI-20060601
    m_bInitialCH = FALSE;
    m_nCurrentDlgSeq = 0;
    if ( !bModify || pParamData->nSpecCodeType == D_SPFC_CODE_USER || pParamData->nSpecCodeType == D_SPFC_CODE_UNDEFINED ) // Add이다 or 0 or 1
    {
        m_nCurrentDlg = m_aLockSeq[0];
        m_bInitialCH = TRUE;
        if ( m_nCase!=0 )
        {
            int nCurrentCodeType = GetCodeFromItem(m_nCurrentDlg);
            pParamData->nSpecCodeType = nCurrentCodeType;
            pParamData->CodeParam.POInitialize(nCurrentCodeType);
        }
    }
    else // CH2001, CHSH2003 는 DB에 저장하는 데이터를 보여주기 위함
    {
        SetCurrentDlg(pParamData->nSpecCodeType);
    }
    m_nShowMode = EN_MODE_RSFUNC;
    m_pDampingWnd = 0;
}

CETC_DESIGN_SPECT::~CETC_DESIGN_SPECT()
{
    CMyChildDialog* dlg;
    int key;

    if ( m_mapChildDlg.Lookup(m_nCurrentDlg, dlg) )
    {
        if ( dlg->IsCreated() )
        {
            dlg->End();
        }
    }

    POSITION pos = m_mapChildDlg.GetStartPosition();
    while ( pos!=NULL )
    {
        m_mapChildDlg.GetNextAssoc(pos, key, dlg);
        delete dlg;
    }
    m_mapChildDlg.RemoveAll();
}

void CETC_DESIGN_SPECT::DoDataExchange(CDataExchange* pDX)
{
    CDialogMove::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ETC_GDS_SPECTRUM_CMB, m_cmbDesignCode);
    DDX_Control(pDX, IDC_ETC_GDS_NA_CMB, m_cmbNationalAnnex);
    DDX_Control(pDX, IDC_ETC_SCLF_EDT, m_edtScaleFactor);
    DDX_Control(pDX, IDC_ETC_MAXV_EDT, m_edtMaxValue);
    DDX_Control(pDX, IDC_ETC_MAXV_UNT, m_untMaxValue);
    DDX_Control(pDX, IDC_ETC_GRAV_EDT, m_edtGravity);
    DDX_Control(pDX, IDC_ETC_GRAV_UNT, m_untGravity);
    DDX_Control(pDX, IDC_ETC_DAMP_EDT, m_edtDampRatio);
    DDX_Control(pDX, IDC_ETC_XLOG_CHK, m_chkXLog);
    DDX_Control(pDX, IDC_ETC_YLOG_CHK, m_chkYLog);
}

BEGIN_MESSAGE_MAP(CETC_DESIGN_SPECT, CDialogMove)
    ON_CBN_SELCHANGE(IDC_ETC_GDS_SPECTRUM_CMB, OnSelchangeEtcGdsSpectrumCmb)
    ON_CBN_SELCHANGE(IDC_ETC_GDS_NA_CMB, OnSelchangeCmdNationalAnnex)
    ON_CBN_CLOSEUP(IDC_ETC_GDS_SPECTRUM_CMB, OnCloseupEtcGdsSpectrumCmb)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_ETC_XLOG_CHK, OnETCXlogChk)
    ON_BN_CLICKED(IDC_ETC_YLOG_CHK, OnETCYlogChk)
    ON_BN_CLICKED(IDC_ETC_IMPORT_BTN, OnETCImportBtn)
    ON_BN_CLICKED(IDC_ETC_NRAC_RDO, OnChangeDataType)
    ON_BN_CLICKED(IDC_ETC_ACCL_RDO, OnChangeDataType)
    ON_BN_CLICKED(IDC_ETC_VELC_RDO, OnChangeDataType)
    ON_BN_CLICKED(IDC_ETC_DISP_RDO, OnChangeDataType)
    ON_BN_CLICKED(IDC_ETC_SCLF_RDO, OnETCScalingType)
    ON_BN_CLICKED(IDC_ETC_MAXV_RDO, OnETCScalingType)
END_MESSAGE_MAP()

BOOL CETC_DESIGN_SPECT::OnInitDialog()
{
    CDialogMove::OnInitDialog();

    ASSERT(m_parAccel);
    m_strCodeName = _T("");
    m_strOldCodeName = _T("Eurocode-8(2004)");
    // Init Combo box
    for ( int i = 0; i < m_aLockSeq.GetSize(); i++ )
    {
        CDlgUtil::CobxAddItem(m_cmbDesignCode, m_aCodeName[m_aLockSeq[i]], m_aLockSeq[i]);
    }

    if ( !m_bModify )
    {
        int nIndex = -1;
        //combo box init 프리퍼런스에 맞춰서.
        T_PREFERENCE rPref;
        rPref.Initialize();
        m_pDoc->m_pInitCtrl->GetPreference(rPref);

        if ( m_pParamData->SpecFuncName.IsEmpty() == FALSE )
        {
            nIndex = m_cmbDesignCode.FindString(-1, m_pParamData->SpecFuncName);
        }
        else
        {
            nIndex = m_cmbDesignCode.FindString(-1, rPref.LoadCode.RSCode);
        }

        //프리퍼런스에 맞는 데이터 Init
        m_pParamData->nSpecCodeType = m_aLockSeq.GetAt(nIndex) + 1;//plus 1
        m_nCurrentDlg = m_aLockSeq.GetAt(nIndex);
        m_nCurrentDlgSeq = nIndex;
    }
    m_cmbDesignCode.SetCurSel(m_nCurrentDlgSeq);

    m_cmbNationalAnnex.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_PREF_RECOMMENDED), EN_SPFC_RECOMMENDED);
	CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_PREF_SINGAPORE), EN_SPFC_SINGAPORE);
    CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_PREF_MALAYSIA), EN_SPFC_MALESIA);
    m_cmbNationalAnnex.SetCurSel(0);

    BOOL bEnableNA = EnableNationalAnnex();
    if ( bEnableNA && m_nCurrentDlg == 21)
    {
        CDlgUtil::CobxSetCurSelItemData(m_cmbNationalAnnex, m_pParamData->CodeParam.EURO2004.nNationalAnnex);
    }

    m_aCtrlSpectral.SetSize(4);
    m_aCtrlSpectral[0] = IDC_ETC_NRAC_RDO;
    m_aCtrlSpectral[1] = IDC_ETC_ACCL_RDO;
    m_aCtrlSpectral[2] = IDC_ETC_VELC_RDO;
    m_aCtrlSpectral[3] = IDC_ETC_DISP_RDO;

    m_aCtrlScaling.SetSize(2);
    m_aCtrlScaling[0] = IDC_ETC_SCLF_RDO;
    m_aCtrlScaling[1] = IDC_ETC_MAXV_RDO;

    m_edtScaleFactor.SetUnitType(0);
    m_edtMaxValue.SetUnitType(0);
    m_untMaxValue.SetUnitType(CUnitCtrl::m_SPFC_UNIT.dblValue[0]);
    m_edtGravity.SetUnitType(CUnitCtrl::m_SPFC_UNIT.Gravity);
    m_untGravity.SetUnitType(CUnitCtrl::m_SPFC_UNIT.Gravity);
    m_edtDampRatio.SetUnitType(0);

    POSITION pos = m_mapChildDlg.GetStartPosition();
    CMyChildDialog* dlg;
    int key;
    while ( pos!=NULL )
    {
        m_mapChildDlg.GetNextAssoc(pos, key, dlg);
        dlg->m_parAccel = m_parAccel;
        dlg->m_parPeriod = m_parPeriod;
    }

    if ( m_nCurrentDlg == EN_CHILD_ORDER_USER )
    {
        InitChart();
    }

    if ( m_pParamData->arSpecFuncData.GetSize() < 1 )
    {
        m_pParamData->ScaleFactor = 1.0;
        m_pParamData->Gravity = M_InitValueCurUnit(9.806, TON, M, CUnitCtrl::m_SPFC_UNIT.Gravity);
        m_pParamData->dDampingRatio = 0.05;
    }

    UpdateCurDlg();      // Data2Dlg

    CDlgUtil::CobxAdjustListBoxWidth(m_cmbDesignCode);

    return TRUE;
}

void CETC_DESIGN_SPECT::OnOK()
{
    int nSel = m_cmbDesignCode.GetCurSel();
    if ( nSel < 0 ) { ASSERT(0); return; }
    if ( nSel >= m_aLockSeq.GetCount() ) { ASSERT(0); return; }

    int nCodeSeq = m_aLockSeq[nSel];

    CString csErrMsg;
    BOOL bAllow = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLoadResponse, nCodeSeq, csErrMsg);
    if ( !bAllow )
    {
        AfxMessageBox(csErrMsg);
        return;
    }

    // 옵션 체크(국문에서 KBC 2016 또는 KDS(41-17-00:2019) 을 사용하려면 MODS 옵션이 있어야 함)
    if ( ( nCodeSeq==34 || nCodeSeq==41 ) && CProduct::IsMovingType(D_PRODUCT_MOVING_ORG) )
    {
        if ( !m_pDoc->AllowCommand(D_OPTN_ID_MODS_SERVICE) )
        {
            csErrMsg = _LS(IDS_CMD_ERROR_MODS_CHEC_KBC2106);
            AfxMessageBox(csErrMsg);
            return;
        }
    }

    CMyChildDialog* dlg;
    m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);

    int nCurCodeItem = m_cmbDesignCode.GetItemData(nSel);
    m_pParamData->nSpecCodeType = GetCodeFromItem(nCurCodeItem);

    if ( dlg->IsCreated() )
    {
        dlg->UpdateData(TRUE);
        if ( !dlg->ISVALID() ) return;
		BOOL bEnableNA = EnableNationalAnnex();
		if (bEnableNA && m_nCurrentDlg == 21)
		{
            m_pParamData->CodeParam.EURO2004.nNationalAnnex = (EN_SPFC_NA)CDlgUtil::CobxGetCurSelItemData(m_cmbNationalAnnex, m_cmbNationalAnnex.GetCurSel());
		}
        dlg->End();
        if ( m_nShowMode == EN_MODE_RSFUNC )
        {
            dlg->MakeSpectrumData();
            m_sDescript = dlg->m_sDescript;
            m_strFuncName = dlg->m_strFuncName;
        }
        //else CalcCurrentPOSpectra();

        if ( m_pDampingWnd )
        {
            dlg->UpdateDampingWnd(m_pDampingWnd);
        }
    }

    if ( nCurCodeItem != EN_CHILD_ORDER_USER )
    {
        dlg->Dlg2Data();
    }
    else if ( nCurCodeItem == EN_CHILD_ORDER_USER )
    {
        CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSpectral, m_pParamData->nSpecFuncType);
        m_pParamData->nSpecFuncType += 1;

        CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlScaling, m_pParamData->nScaleMethod);
        m_pParamData->ScaleFactor   = m_edtScaleFactor.GetEditValue();
        m_pParamData->dMaxValue     = m_edtMaxValue.GetEditValue();
        m_pParamData->Gravity       = m_edtGravity.GetEditValue();
        m_pParamData->dDampingRatio = m_edtDampRatio.GetEditValue();
    }

    // 20071203 mylee - SPFC 코드 여기서 저장
    CString strCodeName; m_cmbDesignCode.GetWindowText(strCodeName);
    m_pParamData->SpecFuncName = strCodeName;
    m_pParamData->nSpecCodeType = GetCodeFromItem(m_cmbDesignCode.GetItemData(nSel));
    if(m_pParamData->nSpecFuncType == D_SPFC_CODE_DPT2018)
    {
        m_pParamData->dDampingRatio = m_pParamData->CodeParam.DPT2018.dDampingRatio;
    }

    // 이렇게 ok버튼을 누르면 itemdlg의 상태가 modify가 되도록 한다.
    //m_bSetParam은 변하지 않음.

    m_TempParamData = *m_pParamData;

    CDialogMove::OnOK();
}

void CETC_DESIGN_SPECT::OnChangeDataType()
{
    const MSG* pMsg = GetCurrentMessage();
    UINT nMessageID = LOWORD(pMsg->wParam);

    UINT aID[] = { IDC_ETC_NRAC_RDO, IDC_ETC_ACCL_RDO, IDC_ETC_VELC_RDO, IDC_ETC_DISP_RDO, };
    CMapEx<UINT, UINT, int, int> mRadioID;
    for ( int i = 0; i < 4; i++ )
    {
        mRadioID.SetAt(aID[i], 0);
    }

    int nYesNo = IDYES;
    if ( m_pParamData->arSpecFuncData.GetSize() > 1 && mRadioID.Exist(nMessageID) && nMessageID != aID[m_nSpectralType] )
    {
        nYesNo = AfxMessageBox(_LS(IDS_CMD_THFC_WARNING_CHANGE_DATA_TYPE), MB_YESNO);
    }

    if ( nYesNo != IDYES )
    {
        // Turn off old radio
        CButton* pRadio = ( CButton* ) GetDlgItem(LOWORD(pMsg->wParam));
        pRadio->SetCheck(0);

        // Turn on new radio
        pRadio = ( CButton* ) GetDlgItem(aID[m_nSpectralType]);
        pRadio->SetCheck(1);
        return;
    }

    switch ( nMessageID )
    {
    case IDC_ETC_NRAC_RDO: m_nSpectralType = 0; break;
    case IDC_ETC_ACCL_RDO: m_nSpectralType = 1; break;
    case IDC_ETC_VELC_RDO: m_nSpectralType = 2; break;
    case IDC_ETC_DISP_RDO: m_nSpectralType = 3; break;
    }

    ChangeDataType(m_nSpectralType);
    EnableGravity(m_nSpectralType == 0);
    SpectralDataChanged();
}

void CETC_DESIGN_SPECT::EnableGravity(BOOL bEnable)
{
    CWnd* pWnd = GetDlgItem(IDC_ETC_GRAV_EDT);
    if ( pWnd )
    {
        pWnd->EnableWindow(bEnable);
    }
}

BOOL CETC_DESIGN_SPECT::EnableNationalAnnex()
{
    int nIndex = m_cmbDesignCode.GetCurSel();
    CString strCurrentCode=_T("");
    m_cmbDesignCode.GetLBText(nIndex, strCurrentCode);
    BOOL bEnable = CDBLib::Is_LoadCode_NationalAnnex(strCurrentCode);
    m_strCodeName = strCurrentCode;
    GetDlgItem(IDC_ETC_GDS_NA_STC)->EnableWindow(bEnable);
    GetDlgItem(IDC_ETC_GDS_NA_CMB)->EnableWindow(bEnable);

    GetDlgItem(IDC_ETC_GDS_NA_STC)->ShowWindow(bEnable);
    GetDlgItem(IDC_ETC_GDS_NA_CMB)->ShowWindow(bEnable);

    BOOL bMoveCtrl = FALSE;
    if ( m_strCodeName == m_strOldCodeName ) return bEnable;

    if ( ( m_strCodeName == _T("Eurocode-8(2004)") ) && ( m_strOldCodeName == _T("") ) )
    {
        m_strOldCodeName = m_strCodeName;
        return bEnable;
    }

    if ( ( m_strCodeName == _T("Eurocode-8(2004)") || m_strOldCodeName == _T("Eurocode-8(2004)") ) )
    {
        CRect rNA;
        GetDlgItem(IDC_ETC_GDS_NA_CMB)->GetWindowRect(rNA);
        int nDistY = rNA.Height();

        if ( m_strOldCodeName == _T("Eurocode-8(2004)") )
        {
            nDistY *= ( -1 );
        }

        CRect RectDlg;
        GetWindowRect(RectDlg);
        RectDlg.bottom += nDistY;
        ScreenToClient(RectDlg);
        SetWindowPos(NULL, RectDlg.left, RectDlg.top, RectDlg.Width(), RectDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);

        CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrlID, nDistY);

        RedrawWindow();
    }

    m_strOldCodeName = m_strCodeName;
    return bEnable;
}

// 20071203 mylee - SPFC 코드 저장위해 변환하는 함수
int CETC_DESIGN_SPECT::GetCodeFromItem(int nItem)
{
    switch ( nItem )
    {
    case EN_CHILD_ORDER_AASHTO_LRFD12:      return D_SPFC_CODE_AASHTO_LRFD12;
    case EN_CHILD_ORDER_AS_5100_2_2017:     return D_SPFC_CODE_AS_5100_2_2017;
    case EN_CHILD_ORDER_CH_BRG89:           return D_SPFC_CODE_CH_BRG89;
    case EN_CHILD_ORDER_CH_GBJ111_87:       return D_SPFC_CODE_CH_GBJ111_87;
    case EN_CHILD_ORDER_CH2002:             return D_SPFC_CODE_CH2002;
    case EN_CHILD_ORDER_CH2010:             return D_SPFC_CODE_CH2010;
    case EN_CHILD_ORDER_CH2019:             return D_SPFC_CODE_CH2019;
    case EN_CHILD_ORDER_CHSH2003:           return D_SPFC_CODE_CHSH2003;
    case EN_CHILD_ORDER_CJJ166_2011:        return D_SPFC_CODE_CJJ166_2011;
    case EN_CHILD_ORDER_DPT2018:            return D_SPFC_CODE_DPT2018;
    case EN_CHILD_ORDER_DPWH_LRFD_2013:     return D_SPFC_CODE_DPWH_LRFD_2013;
    case EN_CHILD_ORDER_EURO1996:           return D_SPFC_CODE_EURO1996;
    case EN_CHILD_ORDER_EURO1996_ELA:       return D_SPFC_CODE_EURO1996_ELA;
    case EN_CHILD_ORDER_EURO2004:           return D_SPFC_CODE_EURO2004;
    case EN_CHILD_ORDER_GB50111_2006:       return D_SPFC_CODE_GB50111_2006;
    case EN_CHILD_ORDER_IBC2000:            return D_SPFC_CODE_IBC2000;
    case EN_CHILD_ORDER_IBC2009:            return D_SPFC_CODE_IBC2009;
    case EN_CHILD_ORDER_IBC2012:            return D_SPFC_CODE_IBC2012;
    case EN_CHILD_ORDER_IRCSP114_2018:      return D_SPFC_CODE_IRCSP114_2018;
    case EN_CHILD_ORDER_IS1893_2016:        return D_SPFC_CODE_IS1893_2016;
    case EN_CHILD_ORDER_IS2002:             return D_SPFC_CODE_IS2002;
    case EN_CHILD_ORDER_JP_BRG2002:         return D_SPFC_CODE_JP_BRG2002;
    case EN_CHILD_ORDER_JP_BRG2012:         return D_SPFC_CODE_JP_BRG2012;
    case EN_CHILD_ORDER_JP_BRG2017:         return D_SPFC_CODE_JP_BRG2017;
    case EN_CHILD_ORDER_JPN2000:            return D_SPFC_CODE_JPN2000;
    case EN_CHILD_ORDER_JTG_T_2231_01_2020: return D_SPFC_CODE_JTG_T_2231_01_2020;
    case EN_CHILD_ORDER_CH_GB_T_51408_2021: return D_SPFC_CODE_CH_GB_T_51408_2021;
    case EN_CHILD_ORDER_JTJXXX_2008:        return D_SPFC_CODE_JTJXXX_2008;
    case EN_CHILD_ORDER_KBC2005:            return D_SPFC_CODE_KBC2005;
    case EN_CHILD_ORDER_KBC2009:            return D_SPFC_CODE_KBC2009;
    case EN_CHILD_ORDER_KBC2015:            return D_SPFC_CODE_KBC2015;
    case EN_CHILD_ORDER_KDS2019:            return D_SPFC_CODE_KDS2019;
    case EN_CHILD_ORDER_KS:                 return D_SPFC_CODE_KS;
    case EN_CHILD_ORDER_KS_BRG:             return D_SPFC_CODE_KS_BRG;
    case EN_CHILD_ORDER_KS2000:             return D_SPFC_CODE_KS2000;
    case EN_CHILD_ORDER_KSCE2019:           return D_SPFC_CODE_KSCE2019;
    case EN_CHILD_ORDER_NBC95:              return D_SPFC_CODE_NBC95;
    case EN_CHILD_ORDER_NSR2010:            return D_SPFC_CODE_NSR2010;
    case EN_CHILD_ORDER_NTC2018:            return D_SPFC_CODE_NTC2018;
    case EN_CHILD_ORDER_P100_2013:          return D_SPFC_CODE_P100_2013;
    case EN_CHILD_ORDER_TAIWAN22:           return D_SPFC_CODE_TAIWAN22;
    case EN_CHILD_ORDER_TAIWAN06:           return D_SPFC_CODE_TAIWAN06;
    case EN_CHILD_ORDER_TAIWAN89H_BRG:      return D_SPFC_CODE_TAIWAN89H_BRG;
    case EN_CHILD_ORDER_TAIWAN89V_BRG:      return D_SPFC_CODE_TAIWAN89V_BRG;
    case EN_CHILD_ORDER_TAIWAN99H:          return D_SPFC_CODE_TAIWAN99H;
    case EN_CHILD_ORDER_TAIWAN99V:          return D_SPFC_CODE_TAIWAN99V;
    case EN_CHILD_ORDER_TAIWANBRG98:        return D_SPFC_CODE_TAIWANBRG98;
    case EN_CHILD_ORDER_UBC88:              return D_SPFC_CODE_UBC88;
    case EN_CHILD_ORDER_UBC97:              return D_SPFC_CODE_UBC97;
	case EN_CHILD_ORDER_RUS_SP2016:         return D_SPFC_CODE_RUS_SP2016;
    case EN_CHILD_ORDER_RUS_SP14_2018:      return D_SPFC_CODE_RUS_SP14_2018;
    case EN_CHILD_ORDER_USER:               return D_SPFC_CODE_USER;
    case EN_CHILD_ORDER_NSCP_2024:          return D_SPFC_CODE_NSCP_2024;
	case EN_CHILD_ORDER_SANS10160_4_2010:   return D_SPFC_CODE_SANS;
	case EN_CHILD_ORDER_IS1893_2025:        return D_SPFC_CODE_IS1893_2025;
    }
    ASSERT(0); return D_SPFC_CODE_UNDEFINED;
}

void CETC_DESIGN_SPECT::SetCurrentDlg(int nSpecCodeType)
{
    for ( int i = 0; i < m_aLockSeq.GetSize(); i++ )
    {
        if ( m_aLockSeq[i] == nSpecCodeType - 1 )
        {
            m_nCurrentDlg = m_aLockSeq[i];
            m_nCurrentDlgSeq = i;
            break;
        }
    }
}
void CETC_DESIGN_SPECT::CalcCurrentPOSpectra()
{
    CMyChildDialog* dlg;

    int nCurrentCodeType = GetCodeFromItem(m_nCurrentDlg);
    if ( m_pParamData->nSpecCodeType != nCurrentCodeType )
    {
        SetCurrentDlg(m_pParamData->nSpecCodeType);
    }

	auto L_IsTaiwanCodeForFEMA440 = [ ] (int nCode) -> bool
	{ //  FEMA440 계산방식, GEN-2400 TWIWAN일 경우는 ATC-40에 사용된 스펙트럼을  FEMA440에 적용함
		switch ( nCode )
		{
		case D_SPFC_CODE_TAIWAN06:
		case D_SPFC_CODE_TAIWAN22:
			return false;
		}
		return true;
	};
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
    dlg->SetPushOverData(FALSE);
    if ( m_nShowMode==EN_MODE_PO_FEMA440 && L_IsTaiwanCodeForFEMA440(m_pParamData->nSpecCodeType) )
    {
        m_aPOPeriod[0].RemoveAll();
        m_aPOAccel[0].RemoveAll();

        dlg->m_parPeriod = &m_aPOPeriod[0];
        dlg->m_parAccel  = &m_aPOAccel[0];
        dlg->MakeSpectrumData(TRUE);
        for ( int i=1; i<5; i++ )
        {
            m_aPOPeriod[i].RemoveAll();
            m_aPOAccel[i].RemoveAll();
            m_aPOPeriod[i].Copy(m_aPOPeriod[0]);
            m_aPOAccel[i].Copy(m_aPOAccel[0]);

            double dB_betaeff = 4.0/( 5.6 - log(m_dPODamping[i]) );
            for ( int j=0; j<m_aPOAccel[i].GetSize(); j++ )
            {
                m_aPOAccel[i][j] = m_aPOAccel[i][j]/dB_betaeff;
            }
        }
    }
    else
    {
        for ( int i=0; i < 5; i++ )  // // fixed for PO dampings 수정하지 말것
        {
            m_aPOPeriod[i].RemoveAll();
            m_aPOAccel[i].RemoveAll();

            if ( m_dPODamping[i] <= 0 )
            {
                continue;
            }
            dlg->MakePOSpectrumData(m_dPODamping[i], -1, -1, m_aPOPeriod[i], m_aPOAccel[i]);
        }
    }
}

void CETC_DESIGN_SPECT::CalcPOSpectrumSRaSRv(double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aPOAccel)
{
    CMyChildDialog* dlg;
    aPeriod.RemoveAll();
    aPOAccel.RemoveAll();
    m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
    dlg->MakePOSpectrumData(-1, SRa, SRv, aPeriod, aPOAccel);
}

void CETC_DESIGN_SPECT::CalcPOSpectrumByDamping(double dDamping, CArray <double, double>& aPeriod, CArray <double, double>& aPOAccel)
{
    CMyChildDialog* dlg;
    aPeriod.RemoveAll();
    aPOAccel.RemoveAll();
    m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
    dlg->MakePOSpectrumData(dDamping, -1, -1, aPeriod, aPOAccel);
}

void CETC_DESIGN_SPECT::OnSelchangeEtcGdsSpectrumCmb()
{
    CMyChildDialog* dlg;
    CString strCurrentDlgSeq;
    int nOldCodeIndex = m_nCurrentDlg;

    int nSel = m_cmbDesignCode.GetCurSel();
    if ( m_aCodeEnable[nSel]==FALSE )
    {
        CDlgUtil::CobxSetCurSelItemData(m_cmbDesignCode, nOldCodeIndex);
        CString strCodeName = _T("");
        m_cmbDesignCode.GetLBText(nSel, strCodeName);
        CString strErrMsg = _T("");
        strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), strCodeName);
        GSaveHistoryFormatNF(strErrMsg);

        return;
    }

    m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
    dlg->UpdateData(TRUE);
    dlg->End();

    m_nCurrentDlgSeq = m_cmbDesignCode.GetCurSel();
    m_nCurrentDlg = m_cmbDesignCode.GetItemData(m_nCurrentDlgSeq);

    m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);

    if ( m_nCurrentDlg == EN_CHILD_ORDER_USER )
    {
        SRGraphDisplay *pD = ( SRGraphDisplay * ) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
        if ( pD == nullptr )
        {
            InitChart();
        }
    }
    else
    {
        m_Graph.KillComponentList();
    }

    EnableNationalAnnex();

    if ( nOldCodeIndex!=m_nCurrentDlg /*&& m_nShowMode != EN_MODE_RSFUNC */)
    {
        m_pParamData->nSpecCodeType = GetCodeFromItem(m_nCurrentDlg);

        switch (m_nShowMode)
        {
        case EN_MODE_RSFUNC:
            m_pParamData->CodeParam.Initialize(m_pParamData->nSpecCodeType);
            break;
        case EN_MODE_PO_FEMA440:
        case EN_MODE_PO_ATC:
            m_pParamData->CodeParam.POInitialize(m_pParamData->nSpecCodeType);
            break;
        default:
            ASSERT(0);
            m_pParamData->CodeParam.Initialize(m_pParamData->nSpecCodeType);
            break;
        }        
    }

    UpdateCurDlg();
}

void CETC_DESIGN_SPECT::OnSelchangeCmdNationalAnnex()
{
    int nCodeIndex = CDlgUtil::CobxGetCurSelItemData(m_cmbDesignCode, m_cmbDesignCode.GetCurSel());
    int nNAIndex = CDlgUtil::CobxGetCurSelItemData(m_cmbNationalAnnex, m_cmbNationalAnnex.GetCurSel()); 

    if ( nCodeIndex != EN_CHILD_ORDER_EURO2004 ) return;

    CMyChildDialog* dlg;
    m_mapChildDlg.Lookup(nCodeIndex, dlg);
    CETC_GDS_EURO2003 *pEURO2003 = ( CETC_GDS_EURO2003* ) dlg;

    pEURO2003->SetNationalAnnex(nNAIndex);
}

void CETC_DESIGN_SPECT::UpdateCurDlg()
{
    if ( m_bInitialCH && ( m_nCurrentDlg == EN_CHILD_ORDER_DPT2018 ) )
    {
        m_pParamData->CodeParam.DPT2018.Init();
    }
    CWnd* pPlaceHolder = GetDlgItem(IDC_ETC_GDS_PLACEHOLDER);

    m_pParamData->nSpecFuncType = GetCodeFromItem(m_nCurrentDlg);

    CMyChildDialog* pDlg = nullptr;
    m_mapChildDlg.Lookup(m_nCurrentDlg, pDlg);
    pDlg->CreateInit(pPlaceHolder);   

    switch ( m_nCurrentDlg )
    {
    case EN_CHILD_ORDER_CH2002:
    case EN_CHILD_ORDER_CHSH2003:
    case EN_CHILD_ORDER_EURO2004:
    case EN_CHILD_ORDER_GB50111_2006:
    case EN_CHILD_ORDER_JTJXXX_2008:
    case EN_CHILD_ORDER_CH2010:
    case EN_CHILD_ORDER_CH2019:
    case EN_CHILD_ORDER_CJJ166_2011:
    case EN_CHILD_ORDER_JTG_T_2231_01_2020: // CH2001, CHSH2003에서만 우선 적용
    case EN_CHILD_ORDER_CH_GB_T_51408_2021:
    case EN_CHILD_ORDER_IS1893_2025:
        {
            // 중국코드가 아닌 것으로 만든 데이터를 보여줄때
            // 최초에 중국 코드가 뜨면 예전에 CodeParam에 저장했던 값이 출력하게되므로 초기화주어야 함.
            if ( m_bInitialCH )
            {
                pDlg->Initialize();
            }
            pDlg->ShowDlgData(); // 뒤에서(m_nShowMode!=0(pushover) 일때) 안한다.
        }
        break;
    case EN_CHILD_ORDER_NTC2018:
        {
            if (m_bInitialCH)
            {
                m_pParamData->CodeParam.Initialize(D_SPFC_CODE_NTC2018);
            }
            pDlg->ShowDlgData();
        }
        break;
    case EN_CHILD_ORDER_IBC2009:
    case EN_CHILD_ORDER_IBC2012:
    case EN_CHILD_ORDER_NSCP_2024:
        {
            pDlg->SetCodeType(m_nCurrentDlg);
        }
        break;
    case EN_CHILD_ORDER_USER:
        {
            CMyChildDialog* dlg;
            m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
            CETC_GDS_UserPage *pUser = ( CETC_GDS_UserPage* ) dlg;
            pUser->Data2Dlg();

            m_nSpectralType = ( m_pParamData->nSpecFuncType == 0 ) ? 0 : m_pParamData->nSpecFuncType-1;
            CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlSpectral, m_nSpectralType);
            OnChangeDataType();
            CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlScaling, m_pParamData->nScaleMethod);
            OnETCScalingType();
            m_edtScaleFactor.SetEditUnit(m_pParamData->ScaleFactor);
            m_edtMaxValue.SetEditUnit(m_pParamData->dMaxValue);
            m_edtGravity.SetEditUnit(m_pParamData->Gravity);
            m_edtDampRatio.SetEditUnit(m_pParamData->dDampingRatio);
            m_chkXLog.SetCheck(FALSE);
            m_chkYLog.SetCheck(FALSE);
        }
        break;
    default:
        {
            pDlg->UpdateData(FALSE);
        }
        break;
    }

    pDlg->ShowWindow(SW_SHOW);
    if ( m_nShowMode != EN_MODE_RSFUNC )  // Design >> Pushover Curve 에서만 적용
    {
        // MNET:5310-HSSHIM-20121024. 음... Child Dialog의 Control들이 뭐가 있을지 알고 여기다 처리했을까... 이런 초보스러운 코딩을 봤나... 쯥...
        CWnd* pWnd = NULL;
        ASSERT(pDlg);

        if ( m_nCurrentDlg != EN_CHILD_ORDER_USER )
        {
            if ( m_nCurrentDlg == EN_CHILD_ORDER_JTJXXX_2008 )
            {
                pWnd = pDlg->GetDlgItem(IDC_ETC_GDS_CH_JTJXXX_2008_STRUCTURE_MAX_STC);  if ( pWnd ) pWnd->ShowWindow(SW_HIDE);
                pWnd = pDlg->GetDlgItem(IDC_ETC_GDS_CH_JTJXXX_2008_STRUCTURE_MAX_EDIT); if ( pWnd ) pWnd->ShowWindow(SW_HIDE);
                pWnd = pDlg->GetDlgItem(IDC_ETC_GDS_CH_JTJXXX_2008_STRUCTURE_MAX_UNIT); if ( pWnd ) pWnd->ShowWindow(SW_HIDE);
            }
            else
            {
                pWnd = pDlg->GetDlgItem(IDC_CMD_PERIOD_EDIT); if ( pWnd ) pWnd->ShowWindow(SW_HIDE);
                pWnd = pDlg->GetDlgItem(IDC_CMD_PERIOD_TEXT); if ( pWnd ) pWnd->ShowWindow(SW_HIDE);
                pWnd = pDlg->GetDlgItem(IDC_CMD_PERIOD_UNIT); if ( pWnd ) pWnd->ShowWindow(SW_HIDE);
            }
        }
        pDlg->Data2Dlg();
        switch ( m_nCurrentDlg )
        {
        case EN_CHILD_ORDER_USER:
            break;
        case EN_CHILD_ORDER_AS_5100_2_2017:
        case EN_CHILD_ORDER_CH_BRG89:
        case EN_CHILD_ORDER_CH_GBJ111_87:
        case EN_CHILD_ORDER_CH2002:
        case EN_CHILD_ORDER_CH2010:
        case EN_CHILD_ORDER_CH2019:
        case EN_CHILD_ORDER_CHSH2003:
        case EN_CHILD_ORDER_CJJ166_2011:
        case EN_CHILD_ORDER_DPT2018:
        case EN_CHILD_ORDER_DPWH_LRFD_2013:
        case EN_CHILD_ORDER_EURO1996:
        case EN_CHILD_ORDER_EURO1996_ELA:
        case EN_CHILD_ORDER_EURO2004:
        case EN_CHILD_ORDER_GB50111_2006:
        case EN_CHILD_ORDER_IBC2000:
        case EN_CHILD_ORDER_IBC2009:
        case EN_CHILD_ORDER_IBC2012:
        case EN_CHILD_ORDER_NSCP_2024:
        case EN_CHILD_ORDER_IRCSP114_2018:
        case EN_CHILD_ORDER_IS1893_2016:
        case EN_CHILD_ORDER_IS2002:
        case EN_CHILD_ORDER_JP_BRG2002:
        case EN_CHILD_ORDER_JP_BRG2012:
        case EN_CHILD_ORDER_JP_BRG2017:
        case EN_CHILD_ORDER_JPN2000:
        case EN_CHILD_ORDER_JTG_T_2231_01_2020:
        case EN_CHILD_ORDER_CH_GB_T_51408_2021:
        case EN_CHILD_ORDER_JTJXXX_2008:
        case EN_CHILD_ORDER_KBC2005:
        case EN_CHILD_ORDER_KBC2009:
        case EN_CHILD_ORDER_KBC2015:
        case EN_CHILD_ORDER_KDS2019:
        case EN_CHILD_ORDER_KISTEC2013:
        case EN_CHILD_ORDER_KS:
        case EN_CHILD_ORDER_KS_BRG:
        case EN_CHILD_ORDER_KS2000:
        case EN_CHILD_ORDER_KSCE2019:
        case EN_CHILD_ORDER_NBC95:
        case EN_CHILD_ORDER_NSR2010:
        case EN_CHILD_ORDER_NTC2018:
        case EN_CHILD_ORDER_P100_2013:
        case EN_CHILD_ORDER_TAIWAN22:
        case EN_CHILD_ORDER_TAIWAN06:
        case EN_CHILD_ORDER_TAIWAN89H_BRG:
        case EN_CHILD_ORDER_TAIWAN89V_BRG:
        case EN_CHILD_ORDER_TAIWAN99H:
        case EN_CHILD_ORDER_TAIWAN99V:
        case EN_CHILD_ORDER_TAIWANBRG98:
        case EN_CHILD_ORDER_UBC88:
        case EN_CHILD_ORDER_UBC97:
		case EN_CHILD_ORDER_RUS_SP2016:
        case EN_CHILD_ORDER_RUS_SP14_2018:
		case EN_CHILD_ORDER_SANS10160_4_2010:
		case EN_CHILD_ORDER_IS1893_2025:
            {
                pDlg->SetPushOverData();
            }
            break;
        default:
            ASSERT(FALSE);
            break;
        }
    }

    AlignDialog();
}

void CETC_DESIGN_SPECT::OnCloseupEtcGdsSpectrumCmb()
{
}

void CETC_DESIGN_SPECT::OnCancel()
{
    CMyChildDialog* dlg;

    m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
    if ( dlg->IsCreated() )
    {
        dlg->UpdateData(TRUE);
        dlg->End();
    }

    int nSel = m_cmbDesignCode.GetCurSel();
    int nCurCodeItem = m_cmbDesignCode.GetItemData(nSel);

    if ( nCurCodeItem == EN_CHILD_ORDER_USER )
    {
        *m_pParamData = m_TempParamData;
        m_Graph.KillData();
    }

    CDialogMove::OnCancel();
}

void CETC_DESIGN_SPECT::SetShowMode(int nShowMode)
{
    m_nShowMode = nShowMode;
}

void CETC_DESIGN_SPECT::PostNcDestroy()
{
    CDialogMove::PostNcDestroy();
}

void CETC_DESIGN_SPECT::OnClose()
{
    CMyChildDialog* dlg;

    m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
    if ( dlg->IsCreated() )
    {
        dlg->UpdateData(TRUE);
        dlg->End();
    }
    CDialogMove::OnClose();
}

void CETC_DESIGN_SPECT::AlignDialog()
{
    CRect rRef, rMove, rChild, rDlg;

    CMyChildDialog* dlg;
    m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
    dlg->GetWindowRect(rChild);

    CWnd* pWnd = GetDlgItem(IDC_ETC_GDS_PLACEHOLDER);
    pWnd->GetWindowRect(rMove);
    rMove.bottom = rMove.bottom - ( rMove.Height() - rChild.Height() );
    rMove.right = rMove.right - ( rMove.Width() - rChild.Width() ); // MNET:3615-JHYUN-20080819
    ScreenToClient(rMove);
    pWnd->MoveWindow(rMove);

    SetWindowPos(NULL, rMove.left, rMove.top, rMove.Width() + globalUtils.ScaleByDPI(22), rMove.Height(), SWP_NOMOVE); // MNET:3615-JHYUN-20080819

    GetDlgItem(IDOK)->GetWindowRect(rMove);
    GetDlgItem(IDC_ETC_GDS_PLACEHOLDER)->GetWindowRect(rRef);
    int nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(16);
    CDlgUtil::CtrlMoveDistY(this, m_aCtrlMethod, nDistY);

    GetDlgItem(IDOK)->GetWindowRect(rRef);
    GetWindowRect(rMove);

    rMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
    MoveWindow(rMove);

    UINT nItem = m_cmbDesignCode.GetItemData(m_cmbDesignCode.GetCurSel());

    CRect rToMove;
    BOOL bShow = FALSE;
    if ( nItem==EN_CHILD_ORDER_USER )
    {
        GetDlgItem(IDC_ETC_FUNC_GRP)->GetWindowRect(rRef);
        bShow = TRUE;
    }
    else
    {
        GetDlgItem(IDC_ETC_GDS_PLACEHOLDER)->GetWindowRect(rRef);
    }

    GetWindowRect(rToMove);
    rToMove.right = rRef.right+ globalUtils.ScaleByDPI(10);
    MoveWindow(rToMove);

    GetDlgItem(IDC_ETC_SPEC_FRM)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_NRAC_RDO)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_ACCL_RDO)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_VELC_RDO)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_DISP_RDO)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_SCAL_FRM)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_SCLF_RDO)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_SCLF_EDT)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_MAXV_RDO)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_MAXV_EDT)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_MAXV_UNT)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_GRAV_FRM)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_GRAV_EDT)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_GRAV_UNT)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_DAMP_FRM)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_DAMP_EDT)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_GRAP_FRM)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_XLOG_CHK)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_YLOG_CHK)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_FUNC_GRP)->ShowWindow(bShow);
    GetDlgItem(IDC_ETC_IMPORT_BTN)->ShowWindow(bShow);

	Invalidate();
}

void CETC_DESIGN_SPECT::InitChart()
{
    m_Graph.KillComponentList();

    m_GraphView.SetDragable(FALSE); // set data-drag unable

    SRGraphBackground* pB = new SRGraphBackground;
    pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
    pB->GetStyle()->SetColor(CXCLR_WHITE);
    m_Graph.AddComponent(( SRGraphComponent * ) pB);

    // Add Display Component
    SRGraphDisplay* pDisplay = new SRGraphDisplay;
    pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX);
    pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
    pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
    pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
    pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
    pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
    pDisplay->SetMeasurement(CX_PERCENT);
    pDisplay->SetSizeDefaults();
    pDisplay->SetFontSize(8);
    pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
    pDisplay->SetRect(0.2, 0, 100, 97.0);
    pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
    pDisplay->GetStyle()->SetUseMaxScale(FALSE);
    pDisplay->GetStyle()->SetAlwaysShowZero(FALSE);

    m_Graph.AddComponent(pDisplay);

    // Add Label Component for X-Axis
    SRGraphLabel* pLabel = new SRGraphLabel;
    pLabel->SetMeasurement(SRGraphComponent::PERCENT);
    pLabel->SetLocationPoint(SRGraphLabel::TopCenter);
    pLabel->SetTextPosition(50.0, 93.5);
    pLabel->SetBlockMember(FALSE);
    pLabel->SetParent(&m_Graph);
    pLabel->SetLabelStandard(SRGraphLabel::Normal);
    pLabel->GetStyle()->SetColor(CXCLR_WHITE);
    pLabel->SetFontSize(9);
    pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
    m_Graph.AddComponent(pLabel);
    SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Period__sec_));

    // Add Label Component for Y-Axis
    pLabel = new SRGraphLabel;
    pLabel->SetMeasurement(SRGraphComponent::PERCENT);
    pLabel->SetLocationPoint(SRGraphLabel::MidCenter);
    pLabel->SetTextPosition(1.5, 50.0);
    pLabel->SetFontSize(9);
    pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
    pLabel->SetBlockMember(FALSE);
    pLabel->SetParent(&m_Graph);
    pLabel->SetLabelStandard(SRGraphLabel::Upward);
    pLabel->GetStyle()->SetColor(CXCLR_WHITE);
    m_Graph.AddComponent(pLabel);
    SetAxisLabelText(1, _LS(IDS_WG_CMD__ADDD__Spectral_Data));

    /////////////////////////////////////////////////////////////
    // Attach graph view to dialog's placeholder and connect graph to graphic view
    CWnd* wnd = GetDlgItem(IDC_ETC_FUNC_GRP);
    CRect rc;

    wnd->GetWindowRect(&rc);
    this->ScreenToClient(&rc);
    rc.left+=2;
    rc.top +=2;
    rc.right -= 2;
    rc.bottom -= 2;

    m_GraphView.DestroyWindow();
    m_GraphView.Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rc, this, IDC_ETC_FUNC_GRP);
    m_GraphView.m_pGraph = &m_Graph;

    m_GraphView.Invalidate();
}

void CETC_DESIGN_SPECT::NewSpectralData()
{
    m_pParamData->arSpecFuncData.RemoveAll();
    m_Graph.KillData();

    m_chkXLog.SetCheck(FALSE);
    m_chkYLog.SetCheck(FALSE);

    OnETCXlogChk();
    OnETCYlogChk();
}

void CETC_DESIGN_SPECT::SetValue(int nIndex, int nGroup, double x, double y)
{
    m_Graph.SetValue(nIndex, nGroup*2, x);
    m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
    m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
    m_Graph.SetValue(nIndex, nGroup*2+1, y);
}

void CETC_DESIGN_SPECT::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
    SRGraphLabel *pLabel=( SRGraphLabel * ) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
    pLabel->SetAnnotation(lbl);
}

void CETC_DESIGN_SPECT::SpectralDataChanged()
{
    // 1. Add handler to update graph
    T_SPFC_BASE data;
    int nSize = m_pParamData->arSpecFuncData.GetSize();

    memset(m_dMnMxXVal, 0, sizeof(m_dMnMxXVal));
    memset(m_dMnMxYVal, 0, sizeof(m_dMnMxYVal));

    m_Graph.KillData();
    if ( !nSize )return;

    for ( int i=0; i<nSize; i++ )
    {
        data = m_pParamData->arSpecFuncData[i];
        SetValue(i, 0, data.dblPeriod, data.dblValue);

        if ( i==0 )
        {
            m_dMnMxXVal[0] = data.dblPeriod;
            m_dMnMxXVal[1] = data.dblPeriod;
            m_dMnMxYVal[0] = data.dblValue;
            m_dMnMxYVal[1] = data.dblValue;
            continue;
        }

        if ( data.dblPeriod < m_dMnMxXVal[0] ) m_dMnMxXVal[0] = data.dblPeriod;
        if ( data.dblPeriod > m_dMnMxXVal[1] ) m_dMnMxXVal[1] = data.dblPeriod;

        if ( data.dblValue < m_dMnMxYVal[0] ) m_dMnMxYVal[0] = data.dblValue;
        if ( data.dblValue > m_dMnMxYVal[1] ) m_dMnMxYVal[1] = data.dblValue;
    }

    double dX = ( m_dMnMxXVal[1]-m_dMnMxXVal[0] )/20.;
    double dY = ( m_dMnMxYVal[1]-m_dMnMxYVal[0] )/20.;

    SRGraphDisplay	*pD=( SRGraphDisplay * ) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
    pD->SetMinRangeY(m_dMnMxYVal[0] - dY);
    pD->SetMinRangeX(0.01);
    pD->SetMaxRangeY(m_dMnMxYVal[1] + dY);
    pD->SetMaxRangeX(m_dMnMxXVal[1] + dX);
    pD->GetStyle()->SetScaleLimit(FALSE);
    pD->GetStyle()->SetUseMaxScale(TRUE);

    m_GraphView.Invalidate();
}

void CETC_DESIGN_SPECT::OnETCXlogChk()
{
    SRGraphDisplay *pD=( SRGraphDisplay * ) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
    pD->GetStyle()->SetLogX(m_chkXLog.GetCheck());

    m_GraphView.Invalidate();
}

void CETC_DESIGN_SPECT::OnETCYlogChk()
{
    SRGraphDisplay *pD=( SRGraphDisplay * ) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
    pD->GetStyle()->SetLog(m_chkYLog.GetCheck());

    m_GraphView.Invalidate();
}

void CETC_DESIGN_SPECT::OnETCImportBtn()
{
    CString strFilter = _LS(IDS_WG_CMD__ADDD__SGS_or_SPD_File____sgs____spd____);
    FILE* fp;
    CFileDialog	fd(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter.GetBuffer(0));

    if ( fd.DoModal() == IDOK )
    {
        if ( fd.GetFileExt() == _T("sgs") || fd.GetFileExt() == _T("SGS") ||
            fd.GetFileExt() == _T("spd") || fd.GetFileExt() == _T("SPD") )
        {
            if ( !( fp = _tfopen(fd.GetPathName().operator LPCTSTR(), _T("r")) ) )
            {
                AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_opening_f));
                return;
            }

            if ( fd.GetFileExt() == _T("sgs") || fd.GetFileExt() == _T("SGS") )
            {
                if ( !ReadSGSData(fp) )
                {
                    AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_reading_d));
                    return;
                }
            }
            else if ( fd.GetFileExt() == _T("spd") || fd.GetFileExt() == _T("SPD") )
            {
                if ( !ReadSPDData(fp) )
                {
                    AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_reading_d));
                    return;
                }
            }

            CMyChildDialog* dlg;
            m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
            CETC_GDS_UserPage *pUser = ( CETC_GDS_UserPage* ) dlg;
            pUser->Data2Dlg();
            SpectralDataChanged();
            UpdateData(FALSE);
        }
        else
        {
            MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_extension_));
            return;
        }
    }
}

void CETC_DESIGN_SPECT::OnETCScalingType()
{
    int nScaling = 0;
    CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlScaling, nScaling);

    GetDlgItem(IDC_ETC_SCLF_EDT)->EnableWindow(nScaling==0);
    GetDlgItem(IDC_ETC_MAXV_EDT)->EnableWindow(nScaling==1);
}

BOOL CETC_DESIGN_SPECT::ReadSGSData(FILE *fp)
{
    char	yytext[120];
    CString	str, msg;
    int		nGroup=-2, xIndex, yIndex, nNumOfDamping=1;
    BOOL bDampingLegend;
    CString strTitle[2];

    T_SPFC_BASE data;
    NewSpectralData();

    // 번역하지 마시오
    CString csCmdSGSw = _T("SGSW");
    CString csCmdTitle = _T("TITLE");
    CString csCmdXAxis = _T("X-AXIS");
    CString csCmdYAxis = _T("Y-AXIS");
    CString csCmdUnit = _T("UNIT&TYPE");
    CString csCmdFlag = _T("FLAGS");
    CString csCmdData = _T("DATA");
    CString csCmdDamping = _T("DAMPING");
    CString csCmdAccel = _T("ACCEL");
    CString csCmdVelo = _T("VELO");
    CString csCmdDisp = _T("DISP");
    CString csCmdGrav = _T("GRAV");
    CString csText;

    lex(fp, yytext);	// asterisk

    lex(fp, yytext);
    csText = yytext;
    csText.TrimRight();
    csText.MakeUpper();

    if ( csText != csCmdSGSw )
    {
        msg.Format(_LS(IDS_WG_CMD__ADDD__error__Invalid_Data_Format___));
        MessageBox(msg);
        return FALSE;
    }

    // Read Graph Title
    //
    int i=0, numOfTitles = 0;
    str.Empty();
    lex(fp, yytext);	// asterisk

    lex(fp, yytext);
    csText = yytext;
    csText.TrimRight();
    csText.MakeUpper();

    while ( csText == csCmdTitle )
    {
        lex(fp, yytext);		// comma
        lex(fp, yytext);

        if ( strcmp(yytext, "*") != 0 )
        {
            str += yytext;

            lex(fp, yytext);
            while ( strcmp(yytext, "*") != 0 )   // for in-Title comma
            {
                str = str +yytext+" ";
                lex(fp, yytext);
            }

            if ( i<2 )		    strTitle[i++]=str;
            str="";
        }

        lex(fp, yytext);
        csText = yytext;
        csText.TrimRight();
        csText.MakeUpper();
    }
    // End of Reading Graph Title

    // Read X-Axis Label
    //
    if ( csText == csCmdXAxis )
    {
        lex(fp, yytext);		// comma
        lex(fp, yytext);
        //SetAxisLabelText(0, yytext);
        lex(fp, yytext);	// asterisk
        lex(fp, yytext);
        csText = yytext;
        csText.TrimRight();
        csText.MakeUpper();
    }
    // Read Y-Axis Label
    //
    if ( csText == csCmdYAxis )
    {
        lex(fp, yytext);		// comma
        lex(fp, yytext);
        //SetAxisLabelText(1, yytext);
        lex(fp, yytext);	// asterisk
        lex(fp, yytext);
        csText = yytext;
        csText.TrimRight();
        csText.MakeUpper();
    }

    ////////////////////////////////////////////////////////////////////
    // UNIT & Type

    // Unit
    CString sUnit, sType;
    int nUnitRead, nDataType;
    T_UNIT_INDEX UnitIndexCurrent;
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexCurrent);

    if ( csText == csCmdUnit )
    {
        lex(fp, yytext);		// comma
        lex(fp, yytext);
        CString sUnit = yytext;
        sUnit.TrimRight();
        sUnit.MakeUpper();
        if ( sUnit != csCmdGrav )
        {
            if ( sUnit == _T("MM") ) nUnitRead = 0;
            else if ( sUnit == _T("CM") ) nUnitRead =1;
            else if ( sUnit == _T("M") ) nUnitRead = 2;
            else if ( sUnit == _T("INCH") || sUnit == _T("IN") || sUnit == _T("IN.") ) nUnitRead =3;
            else if ( sUnit == _T("FEET") || sUnit == _T("FT") || sUnit == _T("FT.") ) nUnitRead =4;
            else
            {
                AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_UNIT_type_));
                fclose(fp);
                return FALSE;
            }
        }
        else
        {
            nUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
            nDataType = 0; // normalized accel;
        }

        // Data type
        lex(fp, yytext);	// asterisk
        lex(fp, yytext);
        sType = yytext;
        sType.TrimRight();
        sType.MakeUpper();
        if ( sType == csCmdAccel && sUnit == csCmdGrav )      nDataType = 0; // normalized accel;
        else if ( sType == csCmdAccel && sUnit != csCmdGrav ) nDataType = 1; // accel;
        else if ( sType == csCmdVelo )                        nDataType = 2; // velocity;
        else if ( sType == csCmdDisp )                        nDataType = 3; // displacement;
        else
        {
            AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Data_Type_));
            fclose(fp);
            return FALSE;
        }

        lex(fp, yytext);	// asterisk

        lex(fp, yytext);
        csText = yytext;
        csText.TrimRight();
        csText.MakeUpper();
    }
    else // 단위및 타입정보가 없으면 디폴트 설정...
    {
        AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Data_Unit___Type_Information__));
        nUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
        nDataType = 0; // normalized accel;
    }
    m_nSpectralType = nDataType;
    CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlSpectral, m_nSpectralType);
    ChangeDataType(m_nSpectralType);

    //////////////////////////////////////////////////////////////
    T_UNIT_INDEX UnitIndexOld, UnitIndexNew;
    pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
    UnitIndexNew=UnitIndexOld;
    UnitIndexNew.nBase_Length=nUnitRead;

    pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew); // 읽은 단위계에서 현재 단위계로
    pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld); // conversion 을 하기 위해서...
    ////////////////////////////////////////////////////////////////

    // Read Graph Flag
    //
    if ( csText == csCmdFlag )
    {
        lex(fp, yytext);		// comma
        lex(fp, yytext);
        BOOL bTripartiteFlag = atof(yytext);
        if ( bTripartiteFlag )
        {
            m_chkXLog.SetCheck(TRUE);
            m_chkYLog.SetCheck(TRUE);
            SRGraphDisplay	*pD=( SRGraphDisplay * ) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
            pD->GetStyle()->SetLogX(TRUE);
            pD->GetStyle()->SetLog(TRUE);
        }

        lex(fp, yytext);		// comma
        lex(fp, yytext);
        bDampingLegend = atof(yytext);
    }
    else
    {
        AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format__FLAGS_not_f));
        fclose(fp);
        return FALSE;
    }

    // Read and Set Graph(Seismic) Data
    //
    lex(fp, yytext);	// asterisk

    lex(fp, yytext);
    csText = yytext;
    csText.TrimRight();
    csText.MakeUpper();

    //	while (csText == csCmdData)
    if ( csText == csCmdData )
        // 첫번째 데이터만을 보여줄 것임 ...!!
    {
        double	d;
        nGroup += 2;
        xIndex = yIndex = 0;

        if ( bDampingLegend )
        {
            lex(fp, yytext);	// asterisk

            lex(fp, yytext);
            csText = yytext;
            csText.TrimRight();
            csText.MakeUpper();

            if ( csText == csCmdDamping )
            {
                lex(fp, yytext);		// comma
                lex(fp, yytext);
                double temp = atof(yytext);
                if ( temp > 1.0 || temp < 0.0 )  temp = 0.05;
                CString strTemp;
                strTemp.Format(_T("%g"), temp);
                m_edtDampRatio.SetWindowText(strTemp);
                nNumOfDamping=nGroup/2+1;
            }
        }

        // Read X-Axis Value
        lex(fp, yytext);
        while ( strcmp(yytext, "*") != 0 )
        {
            // Set X-Axis Value
            if ( nGroup == 0 )   // only the first group of data can be imported.
                data.dblPeriod = atof(yytext);
            lex(fp, yytext);	// comma
            if ( strcmp(yytext, ",") != 0 )
            {
                AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
                fclose(fp);
                return FALSE;
            }

            // Read Y-Axis Value
            lex(fp, yytext);
            if ( ( d=atof(yytext) ) < 0. )
            {
                if ( m_chkYLog.GetCheck() )
                {
                    msg.Format(_LS(IDS_WG_CMD__ADDD__Warning__Negative_Value_Cannot_Be));
                    MessageBox(msg);
                    //((SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE))->GetStyle()->SetLog(FALSE);
                    m_chkYLog.SetCheck(FALSE); //LogFlagY = FALSE;
                }
            }
            // Set Y-Axis Value
            if ( nGroup == 0 )   // only the first group of data can be imported.
                data.dblValue = UnitCvt(atof(yytext));

            // Read X-Axis Value
            lex(fp, yytext);
            if ( ( d=atof(yytext) ) < 0. )
            {
                if ( m_chkYLog.GetCheck() )
                {
                    msg.Format(_LS(IDS_WG_CMD__ADDD__Warning__Negative_Value_Cannot_Be_MSG4));
                    ( ( SRGraphDisplay * ) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE) )->GetStyle()->SetLogX(FALSE);
                    m_chkYLog.SetCheck(FALSE);
                }
            }
            m_pParamData->arSpecFuncData.Add(data);
        }
        lex(fp, yytext);
        csText = yytext;
        csText.TrimRight();
        csText.MakeUpper();
    }

    if ( nNumOfDamping > 1 ) MessageBox(_LS(IDS_WG_CMD__ADDD__Warning___There_are_more_than_one));
    fclose(fp);

    return TRUE;
}

BOOL CETC_DESIGN_SPECT::ReadSPDData(FILE *fp)
{
    char yytext[80];

    T_SPFC_BASE data;
    NewSpectralData();

    // 번역하지 마시오
    CString csCmdUnit = _T("UNIT");
    CString csCmdData = _T("DATA");
    CString csCmdType = _T("TYPE");
    CString csCmdAccel = _T("ACCEL");
    CString csCmdVelo = _T("VELO");
    CString csCmdDisp = _T("DISP");
    CString csCmdGrav = _T("GRAV");
    CString csText;

    // Read and Set Spectrum data
    //
    try
    {
        ////////////////////////////////////////////////////////////////////
        // UNIT & Type

        // Unit
        CString sUnit, sType;
        int nUnitRead, nDataType;
        T_UNIT_INDEX UnitIndexCurrent;
        CDBDoc* pDoc = CDBDoc::GetDocPoint();
        pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexCurrent);

        lex(fp, yytext); // asterisk
        lex(fp, yytext);
        csText = yytext;
        csText.TrimRight();
        csText.MakeUpper();

        sUnit = _T("");

        if ( csText == csCmdUnit )
        {
            lex(fp, yytext);		// comma
            lex(fp, yytext);
            sUnit = yytext;
            sUnit.TrimRight();
            sUnit.MakeUpper();
            if ( sUnit != csCmdGrav )
            {
                if ( sUnit == _T("MM") ) nUnitRead = 0;
                else if ( sUnit == _T("CM") ) nUnitRead =1;
                else if ( sUnit == _T("M") ) nUnitRead = 2;
                else if ( sUnit == _T("INCH") || sUnit == _T("IN") || sUnit == _T("IN.") ) nUnitRead =3;
                else if ( sUnit == _T("FEET") || sUnit == _T("FT") || sUnit == _T("FT.") ) nUnitRead =4;
                else
                {
                    AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_UNIT_type_));
                    fclose(fp);
                    return FALSE;
                }
            }
            else
            {
                nUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
                nDataType = 0; // normalized accel;
            }

            lex(fp, yytext);		// asterisk
            lex(fp, yytext);
            csText = yytext;
            csText.TrimRight();
            csText.MakeUpper();
        }
        else
        {
            AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Data_Unit_Information___nReadi));
            nUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
            nDataType = 0; // normalized accel;
        }

        if ( csText == csCmdType )
        {
            // Data type
            lex(fp, yytext);	// comma
            lex(fp, yytext);
            sType = yytext;
            sType.TrimRight();
            sType.MakeUpper();

            if ( sType == csCmdAccel && sUnit == csCmdGrav )       nDataType = 0; // normalized accel;
            else if ( sType == csCmdAccel && sUnit != csCmdGrav ) nDataType = 1; // accel;
            else if ( sType == csCmdVelo )                        nDataType = 2; // velocity;
            else if ( sType == csCmdDisp )                        nDataType = 3; // displacement;
            else
            {
                AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Data_Type_));
                fclose(fp);
                return FALSE;
            }

            lex(fp, yytext);		// asterisk
            lex(fp, yytext);
            csText = yytext;
            csText.TrimRight();
            csText.MakeUpper();
        }
        else // 단위및 타입정보가 없으면 디폴트 설정...
        {
            AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Data_Type_Information___nReadi));
            nUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
            nDataType = 0; // normalized accel;
        }

        ChangeDataType(nDataType);

        //////////////////////////////////////////////////////////////
        T_UNIT_INDEX UnitIndexOld, UnitIndexNew;
        pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
        UnitIndexNew=UnitIndexOld;
        UnitIndexNew.nBase_Length=nUnitRead;

        pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew); // 읽은 단위계에서 현재 단위계로
        pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld); // conversion 을 하기 위해서...
          ////////////////////////////////////////////////////////////////

        if ( csText == csCmdData )
        {
            while ( lex(fp, yytext)==1 ) // Read X-Axis Value
            {
                // Set X-Axis Value
                data.dblPeriod = atof(yytext);
                lex(fp, yytext);	// comma
                if ( strcmp(yytext, ",") != 0 )
                {
                    AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
                    fclose(fp);
                    return FALSE;
                }

                lex(fp, yytext);

                // Set Y-Axis Value
                data.dblValue = UnitCvt(atof(yytext));
                m_pParamData->arSpecFuncData.Add(data);
            }
        }
        else
        {
            AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Data_or_Wrong_Format_));
        }
    }
    catch ( CFileException* e )  /// useless because it was not CFile;
    {
        e;  // not used
        fclose(fp);
        AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occured_while_reading_da));
        return FALSE;
    }

    fclose(fp);
    return TRUE;
}

double CETC_DESIGN_SPECT::UnitCvt(double dReadData)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    return  pDoc->m_pUnitCtrl->ConvertUnitLengthPrevious(dReadData);
}

int CETC_DESIGN_SPECT::lex(FILE *fp, char *yytext)
{
    int		c, index;

    index	= 0;
    c		= getc(fp);

    // Discard white characters
    while ( ( c==' ' ) || ( c=='\r' ) || ( c=='\t' ) || ( c=='\n' ) )
    {
        c = getc(fp);
    }

    if ( c == EOF ) return -1;

    // Handle(Discard) Comments
    while ( c == '*' )
    {
        c = getc(fp);
        if ( c == '*' )
        {
            c = getc(fp);
            while ( c != '\n' )
                c = getc(fp);
            c = getc(fp);
        }
        else
        {
            ungetc(c, fp);
            yytext[index++] = '*';
            yytext[index] = '\0';
            return 1;
        }
    }

    switch ( c )
    {
    case ',':
        yytext[index++] = c;
        yytext[index] = '\0';
        break;
    default:
        while ( ( c!='*' ) && ( c!=',' ) && ( c!='\r' ) && ( c!='\n' ) )
        {
            yytext[index++] = c;
            c = getc(fp);
            if ( c == EOF || index >=120 )
                return -1;
        }
        yytext[index] = '\0';
        if ( ( c=='*' ) || ( c==',' ) )
            ungetc(c, fp);
    }

    return 1;
}

void CETC_DESIGN_SPECT::ChangeDataType(int nType)
{
    switch ( nType )
    {
    case 0:
        m_edtMaxValue.SetUnitType(D_UNITSYS_NONE);
        m_untMaxValue.SetUnitType(D_UNITSYS_NONE);
        m_untMaxValue.SetWindowText(_T("g"));
        break;
    case 1:
        m_edtMaxValue.SetUnitType(D_UNITSYS_BASE_ACCELERATION);
        m_untMaxValue.SetUnitType(D_UNITSYS_BASE_ACCELERATION);
        break;
    case 2:
        m_edtMaxValue.SetUnitType(D_UNITSYS_BASE_VELOCITY);
        m_untMaxValue.SetUnitType(D_UNITSYS_BASE_VELOCITY);
        break;
    case 3:
        m_edtMaxValue.SetUnitType(D_UNITSYS_BASE_LENGTH);
        m_untMaxValue.SetUnitType(D_UNITSYS_BASE_LENGTH);
        break;
    default: ASSERT(0); break;
    }

    m_pParamData->nSpecFuncType = nType+1;

    CMyChildDialog* dlg;
    m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
    CETC_GDS_UserPage *pUser = ( CETC_GDS_UserPage* ) dlg;
    pUser->ChangeHeaderTitle();
}