#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"

#include "CMWindItemCodeMgr.h"
#include "WindSeisAddLoadDlg.h"
#include "WindAddLoadKBC2015Dlg.h"
#include "WindSeisAddLoadGrid.h"    // List를 Grid로 변경 // PMS:XXXX-4183-20100525
#include "WindProfileDlg.h"
#include "WindProfileCHDlg.h"
#include "WindProfileKBC2015Dlg.h"
#include "WindProfileIS875GUSTDlg.h"
#include "WindProfileDPTDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\ServiceProvider.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_InitCtrl.h"//T_PREFERENCE
#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMWindItemDlg::CCMWindItemDlg(CWnd* pParent)
    : CDialogMove(CCMWindItemDlg::IDD, pParent)
{
    m_Key = 0;
    m_Data.Initialize();
    m_bModify = FALSE;
    m_KeyWAorWT = NULL;

    m_aCodeEnable.RemoveAll();

    m_pCodeMgr = NULL;
    m_pCodeMgr = new CCMWindItemCodeMgr(this);

    m_pDoc = CDBDoc::GetDocPoint();
    ASSERT(m_pDoc);

    m_pGridWnd = new CWindSeisAddLoadGrid();

    m_aMoveCtrlID.Add(IDC_CMD_DESCRIPTION_STC);
    m_aMoveCtrlID.Add(IDC_CMD_DESCRIPTION);
    m_aMoveCtrlID.Add(IDC_CMD_PLACE_HOLDER);
    m_aMoveCtrlID.Add(IDC_CMD_SCALE_GRB);
    m_aMoveCtrlID.Add(IDC_CMD_SCALE_X_STC);
    m_aMoveCtrlID.Add(IDC_CMD_SCALE_Y_STC);
    m_aMoveCtrlID.Add(IDC_CMD_SCALE_Z_STC);
    m_aMoveCtrlID.Add(IDC_CMD_SCALE_GX);
    m_aMoveCtrlID.Add(IDC_CMD_SCALE_GY);
    m_aMoveCtrlID.Add(IDC_CMD_SCALE_GR);
    m_aMoveCtrlID.Add(IDC_CMD_ADD_LOAD_STC);
    m_aMoveCtrlID.Add(IDC_CMD_GRID);
    m_aMoveCtrlID.Add(IDC_CMD_BTN_ADD);
    m_aMoveCtrlID.Add(IDC_CMD_BTN_PROFILE);
    m_aMoveCtrlID.Add(IDOK);
    m_aMoveCtrlID.Add(IDCANCEL);
    m_aMoveCtrlID.Add(IDC_CMD_APPLY);

    m_bSaveFoldedInfo = FALSE;
}

CCMWindItemDlg::~CCMWindItemDlg()
{
    if ( m_pCodeMgr != NULL )
    {
        delete m_pCodeMgr;
        m_pCodeMgr = NULL;
    }

    if ( m_pGridWnd )
    {
        delete m_pGridWnd;
        m_pGridWnd = NULL;
    }
}

void CCMWindItemDlg::SetParamData(T_WIND_K Key, T_WIND_D &Data, T_KEY KeyWAorWT/*=NULL*/)
{
    m_Key = m_OldKey = Key;
    m_Data = Data;
    m_bModify = TRUE;
    m_KeyWAorWT = KeyWAorWT;
}

void CCMWindItemDlg::AddAdditionalLoad(T_STOR_K StartStorKey, T_STOR_K EndStorKey, double dblX, double dblY, double dblRZ)
{
    // Story Key List를 얻는다.
    CArray<T_STOR_K, T_STOR_K> aKeyList;
    //m_pDoc->m_pAttrCtrl->GetStorKeyList(aKeyList);
    m_pDoc->m_pAttrCtrl->GetStorKeyListByConnect(aKeyList, &StartStorKey, &EndStorKey);
    int nCount = aKeyList.GetSize();
    if ( nCount <= 0 ) { ASSERT(0); return; }

    // Additional Wind Load Item 을 만든다.
    T_WIND_ADDITION item; item.Initialize();
    item.AddX = dblX;
    item.AddY = dblY;
    item.AddR = dblRZ;

    // Story List에서 시작 키의 Index를 찾는다.
    int i = 0;
    for ( i = 0; i < nCount; i++ )
        if ( aKeyList[i] == StartStorKey ) break;

    // 입력된 Wind Load List를 만든다.
    T_STOR_K key;
    CArray<T_WIND_ADDITION, T_WIND_ADDITION&> aAddition;
    do
    {
        key = aKeyList[i++];
        item.KeyStor = key;
        aAddition.Add(item);
    } while ( key != EndStorKey );

    // 새로 만들어진 List와 기존의 Wind Load List를 Merge한다.
    int nCount1 = aAddition.GetSize();
    m_pGridWnd->GetData(m_Data.arAddition);
    int nCount2 = m_Data.arAddition.GetSize();
    int n1, n2;
    CArray<T_WIND_ADDITION, T_WIND_ADDITION&> aNewAddition;
    for ( n1 = n2 = 0; n1 < nCount1 && n2 < nCount2; )
    {
        if ( aAddition[n1].KeyStor == m_Data.arAddition[n2].KeyStor )
        {
            aNewAddition.Add(aAddition[n1]);
            n1++; n2++;
        }
        else
        {
            T_STOR_D data1, data2;
            m_pDoc->m_pAttrCtrl->GetStor(aAddition[n1].KeyStor, data1);
            m_pDoc->m_pAttrCtrl->GetStor(m_Data.arAddition[n2].KeyStor, data2);
            if ( data1.dStoryLevel < data2.dStoryLevel )
                aNewAddition.Add(aAddition[n1++]);
            else aNewAddition.Add(m_Data.arAddition[n2++]);
        }
    }
    // 남은 부분 처리
    for ( ; n1 < nCount1; n1++ ) aNewAddition.Add(aAddition[n1]);
    for ( ; n2 < nCount2; n2++ ) aNewAddition.Add(m_Data.arAddition[n2]);

    // 새로 만들어진 것을 복사한다.
    m_Data.arAddition.RemoveAll();
    m_Data.arAddition.Copy(aNewAddition);

    // List를 갱신한다.
    MakeItemEx();
}

void CCMWindItemDlg::AddAdditionalLoad(T_STOR_K StartStorKey, T_STOR_K EndStorKey, double dblX, double dblY, double dblX2, double dblY2, double dblRZ, double dblRZ2)
{
    // Story Key List를 얻는다.
    CArray<T_STOR_K, T_STOR_K> aKeyList;
    m_pDoc->m_pAttrCtrl->GetStorKeyListByConnect(aKeyList, &StartStorKey, &EndStorKey);
    int nCount = aKeyList.GetSize();
    if ( nCount <= 0 ) { ASSERT(0); return; }

    // Additional Wind Load Item 을 만든다.
    T_WIND_ADDITION item; item.Initialize();
    item.AddX  = dblX;
    item.AddY  = dblY;
    item.AddX2 = dblX2;
    item.AddY2 = dblY2;
    item.AddR  = dblRZ;
    item.AddR2 = dblRZ2;

    // Story List에서 시작 키의 Index를 찾는다.
    int i = 0;
    for ( i = 0; i < nCount; i++ )
        if ( aKeyList[i] == StartStorKey ) break;

    // 입력된 Wind Load List를 만든다.
    T_STOR_K key;
    CArray<T_WIND_ADDITION, T_WIND_ADDITION&> aAddition;
    do
    {
        key = aKeyList[i++];
        item.KeyStor = key;
        aAddition.Add(item);
    } while ( key != EndStorKey );

    // 새로 만들어진 List와 기존의 Wind Load List를 Merge한다.
    int nCount1 = aAddition.GetSize();
    m_pGridWnd->GetData(m_Data.arAddition);
    int nCount2 = m_Data.arAddition.GetSize();
    int n1, n2;
    CArray<T_WIND_ADDITION, T_WIND_ADDITION&> aNewAddition;
    for ( n1 = n2 = 0; n1 < nCount1 && n2 < nCount2; )
    {
        if ( aAddition[n1].KeyStor == m_Data.arAddition[n2].KeyStor )
        {
            aNewAddition.Add(aAddition[n1]);
            n1++; n2++;
        }
        else
        {
            T_STOR_D data1, data2;
            m_pDoc->m_pAttrCtrl->GetStor(aAddition[n1].KeyStor, data1);
            m_pDoc->m_pAttrCtrl->GetStor(m_Data.arAddition[n2].KeyStor, data2);
            if ( data1.dStoryLevel < data2.dStoryLevel )
                aNewAddition.Add(aAddition[n1++]);
            else aNewAddition.Add(m_Data.arAddition[n2++]);
        }
    }
    // 남은 부분 처리
    for ( ; n1 < nCount1; n1++ ) aNewAddition.Add(aAddition[n1]);
    for ( ; n2 < nCount2; n2++ ) aNewAddition.Add(m_Data.arAddition[n2]);

    // 새로 만들어진 것을 복사한다.
    m_Data.arAddition.RemoveAll();
    m_Data.arAddition.Copy(aNewAddition);

    // List를 갱신한다.
    MakeItemEx();
}

void CCMWindItemDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogMove::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCMWindItemDlg)
    //DDX_Control(pDX, IDC_CMD_PLACE_HOLDER, m_wndHolder);
    DDX_Control(pDX, IDC_CMD_SCALE_GX, m_editScaleGX);
    DDX_Control(pDX, IDC_CMD_SCALE_GY, m_editScaleGY);
    DDX_Control(pDX, IDC_CMD_SCALE_GR, m_editScaleGR);
    DDX_Control(pDX, IDC_CMD_LOADCASE, m_cobxLoadCase);
    DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_editDiscription);
    DDX_Control(pDX, IDC_CMD_CODE_NAME, m_cobxCodeName);
	DDX_Control(pDX, IDC_CMD_NA_CMB, m_cmbNationalAnnex);
	DDX_Control(pDX, IDC_CMD_GRID, *m_pGridWnd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMWindItemDlg, CDialogMove)
    ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
    ON_BN_CLICKED(IDC_CMD_BTN_PROFILE, OnCmdBtnProfile)
    ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
    ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnCmdLoadcaseBtn)
    ON_CBN_SELCHANGE(IDC_CMD_CODE_NAME, OnSelchangeCmdCodeName)
    ON_CBN_SELCHANGE(IDC_CMD_NA_CMB, OnSelchangeCmdNationalAnnex)
	ON_BN_CLICKED(IDC_CMD_IMPORT_BTN, OnCmdBtnImport)
END_MESSAGE_MAP()

BOOL CCMWindItemDlg::OnInitDialog()
{
    CDialogMove::OnInitDialog();

    // TODO: Add extra initialization here
    m_cobxLoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
    CArray<T_STLD_K, T_STLD_K> aStldK; aStldK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
    T_STLD_D StldData;  StldData.Initialize();
    for ( int i=0; i<aStldK.GetSize(); i++ )
    {
        if ( !m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldData) ) { ASSERT(0); }
        if ( !CDBLib::IsStldATWL(StldData.LoadCaseType) ) continue;
        m_cobxLoadCase.RemoveCase(D_LOADCASE_STATIC, aStldK[i], TRUE);
    }

    InitCodeNameCobx();

    if ( !m_bModify )
    {
        m_Data.Initialize();
        m_Data.ScaleFactor_X = 1.0;
        m_Data.ScaleFactor_Y = 1.0;
        m_Data.ScaleFactor_R = 1.0;
        int nMovingType = CProduct::GetMovingType();
        if ( nMovingType == D_PRODUCT_MOVING_US || nMovingType == D_PRODUCT_MOVING_RUS )
        {
            if ( CProduct::IsIndia() )
                m_Data.nCodeType = IS_W_1987; // India(IS875-1987)
            else if ( CProduct::IsTaiwan() )
                m_Data.nCodeType = TAIWAN_W_1986; // Taiwan86
            else
                m_Data.nCodeType = IBC_W_2012; // IBC2012
        }
        else if ( nMovingType == D_PRODUCT_MOVING_JP )
        {
        #if defined(_IGEN)
            m_Data.nCodeType = JPN_W_2000;   // Japan(Arch 2000)
        #else
            m_Data.nCodeType = JP_W_2004;   // Japan(2004)
        #endif
        }
        else if ( nMovingType == D_PRODUCT_MOVING_CH )
            m_Data.nCodeType = IS_W_875_2015;    // China(2019)
        else
            m_Data.nCodeType = KDS_W_2022;

        ///////////////////////////////////////////////////////////////////////////////////
        //combo box init 프리퍼런스에 맞춰서.
        T_PREFERENCE rPref;
        rPref.Initialize();
        m_pDoc->m_pInitCtrl->GetPreference(rPref);
        int nIndex = m_cobxCodeName.FindString(-1, rPref.LoadCode.WLCode);
        if ( nIndex < 0 ) { ASSERT(0); nIndex=0; }
        m_cobxCodeName.SetCurSel(nIndex);

        //프리퍼런스에 맞는 데이터 Init
        m_Data.nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, m_cobxCodeName.GetCurSel());
        m_Data.CodeParam.Initialize(m_Data.nCodeType);

        BOOL bEnableNA = EnableNationalAnnex();
        if ( bEnableNA && m_Data.nCodeType==EURO_W_2005 && rPref.LoadCode.nWLNationalAnnex==1 )
        {
            m_Data.CodeParam.EURO2005.nTerrain = 11;
            m_Data.CodeParam.EURO2005.dVb = 20;
        }

        ///////////////////////////////////////////////////////////////////////////////////
        // [2011-05-04] Kim, Geun Young (Tel: 2042, gykim@midasit.com) : Default 선택
        CArray<T_STLD_K, T_STLD_K> aStldK;
        m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
        for ( int i=0; i<aStldK.GetSize(); i++ )
        {
            T_STLD_D StldD;
            if ( !m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldD) ) { ASSERT(0); continue; }
            if ( StldD.LoadCaseType == "W" )
            {
                m_Key = aStldK[i];
                break;
            }
        }
    }
    else
    {
        GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
    }

	T_MODL_D ModlD;
	BOOL bExistModl = m_pDoc->m_pAttrCtrl->GetModl(ModlD) && ModlD.aModule.GetSize() > 0;

#if defined (_MGEN_US) || defined(_DEBUG)
	GetDlgItem(IDC_CMD_IMPORT_BTN)->ShowWindow(SW_SHOW);
	if ( (m_Data.nCodeType == USER_W_TYPE || m_Data.nCodeType == RUS_W_2016) && !bExistModl)
		GetDlgItem(IDC_CMD_IMPORT_BTN)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_CMD_IMPORT_BTN)->EnableWindow(FALSE);
#else
	GetDlgItem(IDC_CMD_IMPORT_BTN)->ShowWindow(SW_HIDE);
#endif

    CRect rect;
    GetDlgItem(IDC_CMD_GRID)->GetWindowRect(&rect);
    m_pGridWnd->Initialize(this, TRUE, rect.Width(), ( m_Data.nCodeType==KBC_W_2016 || m_Data.nCodeType==KDS_W_2019 || m_Data.nCodeType == KDS_W_2022 ));

    CString strName, strNewName;
    GetDlgItem(IDC_CMD_ADD_LOAD_STC)->GetWindowText(strName);

    CString strLengthUnit, strForceUnit;
    CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, strLengthUnit);
    CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_FORCE, strForceUnit);

    strNewName.Format(_T("%s (%s:%s,%s)"), strName, _LS(IDS_WG_CMD__ADDD__Unit), strForceUnit, strLengthUnit);
    GetDlgItem(IDC_CMD_ADD_LOAD_STC)->SetWindowText(strNewName);
    Data2Dlg();

    if ( m_KeyWAorWT!=NULL )
    {
        //m_cobxLoadCase.ChangeSelect(D_LOADCASE_STATIC, m_KeyWAorWT);

        CDlgUtil::CtrlEnableDisable(this, IDC_CMD_LOADCASE, FALSE);
        CDlgUtil::CtrlEnableDisable(this, IDC_CMD_LOADCASE_BTN, FALSE);
        CDlgUtil::CtrlEnableDisable(this, IDC_CMD_CODE_NAME, FALSE);
        CDlgUtil::CtrlEnableDisable(this, IDC_CMD_DESCRIPTION, FALSE);

        CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_SCALE_GRB, FALSE, FALSE);
        CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_ADD_LOAD_STC, FALSE, FALSE);
        GetDlgItem(IDOK)->EnableWindow(FALSE);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindItemDlg::OnOK()
{
    // TODO: Add extra validation here
    if ( !AddToDB() ) { ASSERT(0); return; }

    m_bSaveFoldedInfo = TRUE;

    CDialogMove::OnOK();
}

BOOL CCMWindItemDlg::AddToDB()
{
    // TODO: Add your control notification handler code here
    if ( !Dlg2Data() ) return FALSE;

    BOOL IsIBChasAutoCalcD = FALSE;
    if ( ( m_Data.nCodeType == IBC_W_2000 && m_Data.CodeParam.IBC2000.bAutoCalc ) ||
        ( m_Data.nCodeType == IBC_W_2009 && m_Data.CodeParam.IBC2009.bAutoCalc ) ||
        ( m_Data.nCodeType == IBC_W_2012 && m_Data.CodeParam.IBC2012.bAutoCalc ) )
        IsIBChasAutoCalcD = TRUE;

    BOOL IsKBChasAutoCalcD     = ( m_Data.nCodeType == KBC_W_2009 && m_Data.CodeParam.KBC2009.bAutoCalc );
    BOOL IsKBC2016hasAutoCalcD = ( m_Data.nCodeType == KBC_W_2016 && m_Data.CodeParam.KBC2015.bAutoCalc );
    BOOL IsKDS2019hasAutoCalcD = ( m_Data.nCodeType == KDS_W_2019 && m_Data.CodeParam.KDS2019.bAutoCalc );
    BOOL IsKDS2021hasAutoCalcD = ( m_Data.nCodeType == KDS_W_2022 && m_Data.CodeParam.KDS2021.bAutoCalc );
    
    T_WNPS_D WnpsD;
    BOOL bExistWnps = m_pCodeMgr->GetWnpsData(WnpsD);
    if ( IsIBChasAutoCalcD || IsKBChasAutoCalcD || IsKBC2016hasAutoCalcD || IsKDS2019hasAutoCalcD || IsKDS2021hasAutoCalcD )
    {
        T_WNAT_D WnatD;
        m_pCodeMgr->GetWnatData(WnatD);
        BOOL bSuccess;
        if ( m_bModify )
        {
            bSuccess = m_pDoc->m_pDataCtrl->ModifyWind(m_OldKey, m_Key, m_Data, WnatD);
        }
        else
        {
            bSuccess = m_pDoc->m_pDataCtrl->AddWind(m_Key, m_Data, WnatD);
        }
        if ( !bSuccess ) return FALSE;
    }
	else if (bExistWnps)
	{
		BOOL bSuccess;
		if ( m_bModify )
		{
			bSuccess = m_pDoc->m_pDataCtrl->ModifyWind(m_OldKey, m_Key, m_Data, WnpsD);
		}
		else
		{
			bSuccess = m_pDoc->m_pDataCtrl->AddWind(m_Key, m_Data, WnpsD);
		}
		if ( !bSuccess ) return FALSE;
	}
    else
    {
        BOOL bSuccess;
        if ( m_bModify )
        {
            bSuccess = m_pDoc->m_pDataCtrl->ModifyWind(m_OldKey, m_Key, m_Data);
        }
        else
        {
            bSuccess = m_pDoc->m_pDataCtrl->AddWind(m_Key, m_Data);
        }
        if ( !bSuccess ) return FALSE;
    }
    return TRUE;
}

void CCMWindItemDlg::OnCancel()
{
    m_bSaveFoldedInfo = TRUE;

    CDialogMove::OnCancel();
}

void CCMWindItemDlg::OnCmdApply()
{
    AddToDB();
}

void CCMWindItemDlg::OnCmdBtnAdd()
{
    int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, m_cobxCodeName.GetCurSel());
    switch ( nCodeType )
    {
    case KBC_W_2016:
        {
            T_WIND_D data; data.Initialize();
            m_pCodeMgr->CodeDlg2Data(data);

            CWindAddLoadKBC2015Dlg dlg;
            dlg.SetMode(( void* ) this, data.CodeParam.KBC2015.bAcrossWind, data.CodeParam.KBC2015.bTorsionalWind);
            dlg.DoModal();
        }
        break;
    case KDS_W_2019:
        {
            T_WIND_D data; data.Initialize();
            m_pCodeMgr->CodeDlg2Data(data);

            CWindAddLoadKBC2015Dlg dlg;
            dlg.SetMode(( void* ) this, data.CodeParam.KDS2019.bAcrossWind, data.CodeParam.KDS2019.bTorsionalWind);
            dlg.DoModal();
        }
        break;
    case KDS_W_2022:
        {
            T_WIND_D data; data.Initialize();
            m_pCodeMgr->CodeDlg2Data(data);

            CWindAddLoadKBC2015Dlg dlg;
            dlg.SetMode(( void* ) this, data.CodeParam.KDS2021.bAcrossWind, data.CodeParam.KDS2021.bTorsionalWind);
            dlg.DoModal();
        }
        break;
    default:
        {
            CWindSeisAddLoadDlg dlg;
            dlg.SetMode(( void* ) this, nullptr, nullptr);
            dlg.DoModal();
        }
        break;
    }
}

void CCMWindItemDlg::OnCmdBtnProfile()
{
    if ( !Dlg2Data() ) return;

    if ( !m_pDoc->m_pEditData->CheckWindData(m_Data) ) return;

    T_WNAT_D* pWnatD = nullptr;
    T_WNAT_D WnatD;
    BOOL IsIBChasAutoCalcD = FALSE;
    if ( ( m_Data.nCodeType == IBC_W_2000 && m_Data.CodeParam.IBC2000.bAutoCalc ) ||
        ( m_Data.nCodeType == IBC_W_2009 && m_Data.CodeParam.IBC2009.bAutoCalc ) ||
        ( m_Data.nCodeType == IBC_W_2012 && m_Data.CodeParam.IBC2012.bAutoCalc ) )
        IsIBChasAutoCalcD = TRUE;
    BOOL IsKBChasAutoCalcD     = ( m_Data.nCodeType == KBC_W_2009 && m_Data.CodeParam.KBC2009.bAutoCalc );
    BOOL IsKBC2016hasAutoCalcD = ( m_Data.nCodeType == KBC_W_2016 && m_Data.CodeParam.KBC2015.bAutoCalc );
    BOOL IsKDS2019hasAutoCalcD = ( m_Data.nCodeType == KDS_W_2019 && m_Data.CodeParam.KDS2019.bAutoCalc );
    BOOL IsKDS2021hasAutoCalcD = ( m_Data.nCodeType == KDS_W_2022 && m_Data.CodeParam.KDS2021.bAutoCalc );

    if ( IsIBChasAutoCalcD || IsKBChasAutoCalcD || IsKBC2016hasAutoCalcD || IsKDS2019hasAutoCalcD || IsKDS2021hasAutoCalcD )
    {
        m_pCodeMgr->GetWnatData(WnatD);
        pWnatD = &WnatD;
    }

    switch ( m_Data.nCodeType )
    {
    case CH_W_2012:
    case CH_W_2021:
        {
            CWindProfileCHDlg dlg;
            dlg.InitWindProfile(m_Key, m_Data);
            dlg.DoModal();
        }
        break;
    case KBC_W_2016:
    case KDS_W_2019:
    case KDS_W_2022:
        {
            if ( m_Data.nCodeType == KDS_W_2022 && m_Data.CodeParam.KDS2021.bVortexShedding && m_Data.CodeParam.KDS2021.nMethod == 1 )
            {
                CWindProfileDlg dlg;
                dlg.InitWindProfile(m_Key, m_Data, pWnatD);
                dlg.DoModal();
            }
            else
            {
                CWindProfileKBC2015Dlg dlg;
                dlg.InitWindProfile(m_Key, m_Data, m_KeyWAorWT, pWnatD);
                dlg.DoModal();
            }
        }
        break;
    case IS_W_875_2015:
        {
            if ( m_Data.CodeParam.IS875_2015.nProcedure == 1 )
            {
                CWindProfileDlg dlg;
                dlg.InitWindProfile(m_Key, m_Data, pWnatD);
                dlg.DoModal();
            }
            else
            {
                CWindProfileIS875GUSTDlg dlg;
                dlg.InitWindProfile(m_Key, m_Data, pWnatD);
                dlg.DoModal();
            }
        }
        break;
    case DPT_W_2007:
        {
            if ( m_Data.CodeParam.DPT2007.unMethod == EN_DPT_WIND_METHOD_DETAIL )
            {
                CWindProfileDPTDlg dlg;
                dlg.InitWindProfile(m_Key, m_Data, m_KeyWAorWT, pWnatD);
                dlg.DoModal();
            }
            else
            {
                CWindProfileDlg dlg;
                dlg.InitWindProfile(m_Key, m_Data, pWnatD);
                dlg.DoModal();
            }
        }
        break;
    default:
        {
            CWindProfileDlg dlg;
            dlg.InitWindProfile(m_Key, m_Data, pWnatD);
            dlg.DoModal();
        }
        break;
    }
}

void CCMWindItemDlg::OnSelchangeCmdCodeName()
{
    int nOldCodeType = m_pCodeMgr->GetCurCodeType();

    int nSel = m_cobxCodeName.GetCurSel();
    if ( m_aCodeEnable[nSel]==FALSE )
    {
        CDlgUtil::CobxSetCurSelItemData(m_cobxCodeName, nOldCodeType);
        CString strCodeName = _T("");
        m_cobxCodeName.GetLBText(nSel, strCodeName);
        CString strErrMsg = _T("");
        strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), strCodeName);
        GSaveHistoryFormatNF(strErrMsg);

        return;
    }

    BOOL bEnableNA = EnableNationalAnnex();

    // TODO: Add your control notification handler code here
    int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, m_cobxCodeName.GetCurSel());
    if ( nOldCodeType!=nCodeType && ( nOldCodeType==EURO_W_2005 || nCodeType==EURO_W_2005 ) )
    {
        int nDistY = GetMoveDistNA();
        if ( bEnableNA )
        {
            nDistY *= ( -1 );
        }

        CRect RectDlg;
        GetWindowRect(&RectDlg);
        RectDlg.bottom += nDistY;
        ScreenToClient(RectDlg);
        SetWindowPos(NULL, RectDlg.left, RectDlg.top, RectDlg.Width(), RectDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);

        CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrlID, nDistY);

        RedrawWindow();
    }

    m_pCodeMgr->ChangeCodeDlg(nCodeType-1);

    BOOL bEnable = nCodeType==JP_W_2004; // Japan(2004)
    GetDlgItem(IDC_CMD_SCALE_GR)->EnableWindow(bEnable);
    if ( !bEnable )
    {
        m_editScaleGR.SetWindowText(_T("0"));
    }

    T_WIND_D data; data.Initialize();
    if ( nOldCodeType != nCodeType ) data.CodeParam.Initialize(nCodeType);

    m_pCodeMgr->CodeDlg2Data(data);
    if ( nCodeType == KBC_W_2016 || nCodeType == KDS_W_2019 )
    {
        T_WIND_KBC2015* pDataKBC2015 = nCodeType == KBC_W_2016 ? &data.CodeParam.KBC2015 : &data.CodeParam.KDS2019;
        m_pGridWnd->SetMode(( nCodeType == KBC_W_2016 || nCodeType == KDS_W_2019 ), pDataKBC2015->bAcrossWind, pDataKBC2015->bTorsionalWind);
    }
    else if ( nCodeType == KDS_W_2022 )
    {
        m_pGridWnd->SetMode(!data.CodeParam.KDS2021.bVortexShedding, data.CodeParam.KDS2021.bAcrossWind, data.CodeParam.KDS2021.bTorsionalWind);
    }
    else m_pGridWnd->SetMode(FALSE, FALSE, FALSE);

	T_MODL_D ModlD;
	BOOL bExistModl = m_pDoc->m_pAttrCtrl->GetModl(ModlD) && ModlD.aModule.GetSize() > 0;

#if defined (_MGEN_US) || defined(_DEBUG)
	if ((nCodeType == USER_W_TYPE || nCodeType == RUS_W_2016)&& !bExistModl)
		GetDlgItem(IDC_CMD_IMPORT_BTN)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_CMD_IMPORT_BTN)->EnableWindow(FALSE);
#endif
}

void CCMWindItemDlg::OnSelchangeCmdNationalAnnex()
{
    int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, m_cobxCodeName.GetCurSel());
    int nNA = m_cmbNationalAnnex.GetCurSel();
    m_pCodeMgr->ChangeNationalAnnex(nCodeType-1, nNA);
}

void CCMWindItemDlg::OnCmdLoadcaseBtn()
{
    AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));
}

void CCMWindItemDlg::AlignCtrl2Param(int nChildBottom)
{
    CRect rcHld;
    GetDlgItem(IDC_CMD_PLACE_HOLDER)->GetWindowRect(rcHld);
    rcHld.bottom = nChildBottom;
    ScreenToClient(rcHld);
    GetDlgItem(IDC_CMD_PLACE_HOLDER)->MoveWindow(rcHld);
    GetDlgItem(IDC_CMD_PLACE_HOLDER)->GetWindowRect(rcHld);

    CRect rcMove;
    GetDlgItem(IDC_CMD_SCALE_GRB)->GetWindowRect(rcMove);
    int nDistY = nChildBottom - rcMove.top + globalUtils.ScaleByDPI(7);
    CArray<UINT, UINT> aMoveCtrl;
    aMoveCtrl.Add(IDC_CMD_SCALE_GRB);
    aMoveCtrl.Add(IDC_CMD_SCALE_X_STC);
    aMoveCtrl.Add(IDC_CMD_SCALE_GX);
    aMoveCtrl.Add(IDC_CMD_SCALE_Y_STC);
    aMoveCtrl.Add(IDC_CMD_SCALE_GY);
    aMoveCtrl.Add(IDC_CMD_SCALE_Z_STC);
    aMoveCtrl.Add(IDC_CMD_SCALE_GR);
    aMoveCtrl.Add(IDC_CMD_ADD_LOAD_STC);
    aMoveCtrl.Add(IDC_CMD_GRID);
    aMoveCtrl.Add(IDC_CMD_BTN_ADD);
    aMoveCtrl.Add(IDC_CMD_BTN_PROFILE);
    aMoveCtrl.Add(IDOK);
    aMoveCtrl.Add(IDCANCEL);
    aMoveCtrl.Add(IDC_CMD_APPLY);
    CDlgUtil::CtrlMoveDistY(this, aMoveCtrl, nDistY);

    CRect rcDlg, rcBtn;
    GetWindowRect(rcDlg);
    GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rcBtn);

    rcDlg.bottom = rcBtn.bottom + globalUtils.ScaleByDPI(8);
    MoveWindow(rcDlg);

    Invalidate();
}

void CCMWindItemDlg::UpdateGrid(BOOL bWindKBC2015, BOOL bAcross, BOOL bTorsinal)
{
    if ( m_pGridWnd==NULL ) return;
    m_pGridWnd->SetMode(bWindKBC2015, bAcross, bTorsinal);
}

void CCMWindItemDlg::InitCodeNameCobx()
{
    m_cobxCodeName.ResetContent();

    CStringArray aCodeName;
    CArray<int, int> aCodeIndex;
    m_aCodeEnable.RemoveAll();
    int nNum = m_pCodeMgr->GetCodeNameList(aCodeName, aCodeIndex, &m_aCodeEnable);

    for ( int i = 0; i < nNum; i++ )
    {
        CDlgUtil::CobxAddItem(m_cobxCodeName, aCodeName[i], aCodeIndex[i]);
    }
    m_cobxCodeName.SetCurSel(0);

    m_cmbNationalAnnex.ResetContent();
    m_cmbNationalAnnex.AddString(_LS(IDS_PREF_RECOMMENDED));
    m_cmbNationalAnnex.AddString(_LS(IDS_PREF_SINGAPORE));
    m_cmbNationalAnnex.SetCurSel(0);
}

BOOL CCMWindItemDlg::EnableNationalAnnex()
{
    int nIndex = m_cobxCodeName.GetCurSel();
    CString strCurrentCode=_T("");
    m_cobxCodeName.GetLBText(nIndex, strCurrentCode);
    BOOL bEnable = CDBLib::Is_LoadCode_NationalAnnex(strCurrentCode);
    GetDlgItem(IDC_CMD_NA_STC)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_NA_CMB)->EnableWindow(bEnable);

    GetDlgItem(IDC_CMD_NA_STC)->ShowWindow(bEnable);
    GetDlgItem(IDC_CMD_NA_CMB)->ShowWindow(bEnable);

    return bEnable;
}

void CCMWindItemDlg::Data2Dlg()
{
    m_cobxLoadCase.ChangeSelect(D_LOADCASE_STATIC, m_Key);

    int nNum = m_cobxCodeName.GetCount();
    int i = 0;
    for ( i=0; i<nNum; i++ )
        if ( m_cobxCodeName.GetItemData(i) == m_Data.nCodeType ) break;
    ASSERT(i != nNum);
    m_cobxCodeName.SetCurSel(i);

    CString csVal;
    m_editDiscription.SetWindowText(m_Data.Description);
    csVal.Format(_T("%g"), m_Data.ScaleFactor_X);
    m_editScaleGX.SetWindowText(csVal);
    csVal.Format(_T("%g"), m_Data.ScaleFactor_Y);
    m_editScaleGY.SetWindowText(csVal);
    csVal.Format(_T("%g"), m_Data.ScaleFactor_R);
    m_editScaleGR.SetWindowText(csVal);

    BOOL bEnableNA = EnableNationalAnnex();
    BOOL bSingapore=FALSE;
    if ( bEnableNA )
    {
        if ( m_Data.nCodeType==EURO_W_2005 && m_Data.CodeParam.EURO2005.nTerrain>=10 )
        {
            m_cmbNationalAnnex.SetCurSel(1);
            bSingapore = TRUE;
        }
    }

    if ( bEnableNA==FALSE )
    {
        int nDistY = GetMoveDistNA();

        CRect RectDlg;
        GetWindowRect(&RectDlg);
        RectDlg.bottom += nDistY;
        ScreenToClient(RectDlg);
        SetWindowPos(NULL, RectDlg.left, RectDlg.top, RectDlg.Width(), RectDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);

        CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrlID, nDistY);
    }

    int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, i);
    m_pCodeMgr->Data2CodeDlg(m_Data, nCodeType - 1, m_bModify, bSingapore, m_KeyWAorWT, m_Key);

    BOOL bEnable = nCodeType==JP_W_2004; // Japan(2004)
    GetDlgItem(IDC_CMD_SCALE_GR)->EnableWindow(bEnable);
    if ( !bEnable )
    {
        m_editScaleGR.SetWindowText(_T("0"));
    }

    MakeItemEx();
}

int CCMWindItemDlg::GetMoveDistNA()
{
    CRect crNa;
    GetDlgItem(IDC_CMD_NA_CMB)->GetWindowRect(crNa);
    return ( -1.20 ) * crNa.Height() + 1;
}

BOOL CCMWindItemDlg::Dlg2Data()
{
    if ( m_KeyWAorWT!=NULL ) return TRUE;

    UINT nLoadCaseType;
    if ( !m_cobxLoadCase.GetSelectedLoad(nLoadCaseType, m_Key) )
    {
        AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
        return FALSE;
    }

    m_Data.Initialize();

    if ( !m_pCodeMgr->CodeDlg2Data(m_Data) ) return FALSE;

    int nIndex = m_cobxCodeName.GetCurSel();
    if ( nIndex == CB_ERR ) return FALSE;
    m_Data.nCodeType = m_cobxCodeName.GetItemData(nIndex);

    CString csVal;
    m_editDiscription.GetWindowText(m_Data.Description);
    m_editScaleGX.GetWindowText(csVal);
    m_Data.ScaleFactor_X = _ttof(csVal);
    m_editScaleGY.GetWindowText(csVal);
    m_Data.ScaleFactor_Y = _ttof(csVal);
    m_editScaleGR.GetWindowText(csVal);
    m_Data.ScaleFactor_R = _ttof(csVal);

	//Added by Maharshi on 10-22-2024
	if (m_Data.nCodeType == IS_W_875_2015 && m_Data.CodeParam.IS875_2015.nProcedure == 2)  //For IS875:2015 scale factors can only be a pair of zero and non-zero
	{
        double dScaleX = m_Data.ScaleFactor_X;
        double dScaleY = m_Data.ScaleFactor_Y;
		if (dgn::UQ0(dScaleX) && dgn::UQ0(dScaleY) || (dgn::EQ0(dScaleX) && dgn::EQ0(dScaleY)))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_IS875));
			return FALSE;
		}
	}

    m_pGridWnd->GetData(m_Data.arAddition);

    return TRUE;
}

void CCMWindItemDlg::MakeItemEx()
{
    m_pGridWnd->SetData(m_Data.arAddition);
    m_pGridWnd->MakeItemEx();
}

void CCMWindItemDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    HWND hWnd;
    if ( ( hWnd = GetSafeHwnd() ) == 0 || !IsWindow(hWnd) ) return;

    switch ( lHint )
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
    case D_UPDATE_UNIT:
        break;
    case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
        break;
    case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
        break;
    default:
        //ASSERT(FALSE);
        break;
    }
}

void CCMWindItemDlg::UpdateBuffer()
{
    CWaitCursor wait;

    CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
    int nCount = pViewBuff->GetCount();
    if ( nCount == 0 ) return;
    ASSERT(nCount == 1);

    T_UDRD_BUFFER buffer_ur;
    POSITION pos = pViewBuff->GetStartBuffer();

    T_STOR_K Key, KeyBak;
    T_STOR_D Data, DataBak;
    BOOL bStorChanged = FALSE;
	BOOL bDataChanged = FALSE;
	T_WIND_K WindK;
    while ( pos != NULL )
    {
        buffer_ur = pViewBuff->GetNextBuffer(pos);

        int nCmd = buffer_ur.nCmd;
        int nKey = buffer_ur.nKey;

        switch ( nCmd )
        {
        case UR_STOR_ADD:
            bStorChanged = TRUE;
            break;
        case UR_STOR_DEL:
            {
                m_pDoc->m_pViewBuff->GetStor(nKey, Key, Data);
                int nCount = m_Data.arAddition.GetSize();
                int i = 0;
                for ( i = 0; i < nCount; i++ )
                    if ( m_Data.arAddition[i].KeyStor == Key ) break;
                if ( i != nCount ) m_Data.arAddition.RemoveAt(i);

				if (m_Data.nCodeType == USER_W_TYPE || m_Data.nCodeType == RUS_W_2016)
				{				
					for (i = 0; i < m_Data.arUser.GetCount(); i++)
					{
						if (m_Data.arUser[i].KeyStor == Key)
						{
							m_Data.arUser.RemoveAt(i); 
							break;					
						}
					}
				}
            }
            bStorChanged = TRUE;
            break;
        case UR_STOR_MFD:
            {
                m_pDoc->m_pViewBuff->GetStor(nKey, KeyBak, DataBak);
            }
            bStorChanged = TRUE;
            break;
        case UR_STOR_MFS:
            {
                m_pDoc->m_pViewBuff->GetStor(nKey, Key, Data);
                int nCount = m_Data.arAddition.GetSize();
                int i = 0;
                for ( i = 0; i < nCount; i++ )
                    if ( m_Data.arAddition[i].KeyStor == Key ) break;
                if ( i != nCount ) m_Data.arAddition[i].KeyStor = KeyBak;

				if (m_Data.nCodeType == USER_W_TYPE || m_Data.nCodeType == RUS_W_2016)
				{
					for (i = 0; i < m_Data.arUser.GetCount(); i++)
					{
						if (m_Data.arUser[i].KeyStor == Key)
						{
							m_Data.arUser[i].KeyStor = KeyBak;
							break;
						}
					}
				}
            }
            bStorChanged = TRUE;
            break;
        case UR_WIND_ADD:
		case UR_WIND_MFS:
		case UR_WIND_MFD:
			{
				m_pDoc->m_pViewBuff->GetWind(nKey, WindK, m_Data);
				bDataChanged = TRUE;
			}
			break;
        default:
            break;
        }
    }

    if ( bStorChanged )
    {
        // while문에서는 Data 변경만 Update했다. 이 경우 Story 순서가 바뀌는 경우가
        // 발생할 수 도 있으므로 Sorting을 한다.
        CArray<T_WIND_ADDITION, T_WIND_ADDITION&> aAddition;
        CArray<T_STOR_K, T_STOR_K> aKeyList;
        m_pDoc->m_pAttrCtrl->GetStorKeyList(aKeyList);

        int nCount = aKeyList.GetSize();
        for ( int i = 0; i < nCount; i++ )
        {
            int nCount2 = m_Data.arAddition.GetSize();
            int j = 0;
            for ( j = 0; j < nCount2; j++ )
                if ( aKeyList[i] == m_Data.arAddition[j].KeyStor ) break;
            if ( j != nCount2 )
            {
                aAddition.Add(m_Data.arAddition[j]);
                m_Data.arAddition.RemoveAt(j);
            }
        }
        m_Data.arAddition.Copy(aAddition);
        // List를 갱신한다.
        MakeItemEx();
    }

	if (bStorChanged || bDataChanged)
	{
        if (m_Data.nCodeType == USER_W_TYPE)
        {
            m_pCodeMgr->Data2CodeDlg(m_Data, USER_W_TYPE - 1, TRUE, FALSE); 
        }
        else if (m_Data.nCodeType == RUS_W_2016)
        {
            m_pCodeMgr->Data2CodeDlg(m_Data, RUS_W_2016 - 1, TRUE, FALSE, NULL, m_Key);
        }
	}
}

void CCMWindItemDlg::GetWindData(T_WIND_D& WindD)
{
    Dlg2Data();
    WindD = m_Data;
}

bool CCMWindItemDlg::GetStoryData(T_CMD_WIND_STORY_D& rData)
{
    rData.Init();

    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return false; }

    CArray<T_STOR_K, T_STOR_K> aStorK;
    pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
    int nStorNum = aStorK.GetSize();
    if ( nStorNum < 1 ) { ASSERT(0); return false; }

    T_STOR_D StorD; StorD.Initialize();
    pDoc->m_pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);

    const double dDmLevel   = StorD.dStoryLevel * 2.0 / 3.0;
    const double dTol       = 1.e-10;
    const double dBaseLevel = pDoc->m_pAttrCtrl->GetGroundLevel();

    bool bFirst = true;
    bool bFindDmLevel = true;
    T_STOR_D StorD2; StorD2.Initialize();
    for ( int i = 0; i < nStorNum; ++i )
    {
        pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);

        if ( StorD2.dStoryLevel < dBaseLevel - ( 1.e-10 ) )
        {
            rData.dMaxDn4DB = max(rData.dMaxDn4DB, max(StorD2.WindWidthX, StorD2.WindWidthY));
            continue;
        }

        if ( bFindDmLevel )
        {
            if ( fabs(StorD2.dStoryLevel - dDmLevel) <= dTol )
            {
                rData.dMaxUp4Dm = max(StorD2.WindWidthX, StorD2.WindWidthY);
                bFindDmLevel = false;
            }
            else if ( StorD2.dStoryLevel > dDmLevel + dTol )
            {
                rData.dMaxUp4Dm = max(StorD2.WindWidthX, StorD2.WindWidthY);

                if ( i > 0 )
                {
                    T_STOR_D LowerStor;
                    pDoc->m_pAttrCtrl->GetStor(aStorK[i - 1], LowerStor);
                    rData.dMaxDn4Dm = max(LowerStor.WindWidthX, LowerStor.WindWidthY);
                }
                bFindDmLevel = false;
            }
        }

        if ( bFirst )
        {
            rData.dMinLx    = StorD2.WindWidthX;
            rData.dMinLy    = StorD2.WindWidthY;
            rData.dMaxUp4DB = max(StorD2.WindWidthX, StorD2.WindWidthY);
            bFirst = false;
            continue;
        }
        rData.dMinLx = min(rData.dMinLx, StorD2.WindWidthX);
        rData.dMinLy = min(rData.dMinLy, StorD2.WindWidthY);
    }

    if ( bFindDmLevel )
    {
        ASSERT(0);
        rData.dMaxDn4DB = max(rData.dMaxDn4DB, max(StorD2.WindWidthX, StorD2.WindWidthY));
    }

    for ( int i = 0; i < nStorNum; ++i )
    {
        pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);
        if ( StorD2.dStoryLevel > dBaseLevel ) break;
    }
    if ( StorD2.dStoryLevel > dBaseLevel )
    {
        rData.dLx = StorD2.WindWidthX;
        rData.dLy = StorD2.WindWidthY;
    }

    return true;
}

bool CCMWindItemDlg::GetStoryMass(double& rdMassX, double& rdMassY, double& rdMassXY, double& rdTotal)
{
    rdMassX  = 0.0;
    rdMassY  = 0.0;
    rdMassXY = 0.0;
    rdMassXY = 0.0;

    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return false; }

    CArray<T_STOR_K, T_STOR_K> aStorK;
    pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
    int nStor = aStorK.GetSize();
    if ( nStor == 0 ) return true;

    T_BLDC_D BldcD;
    if ( !pDoc->m_pAttrCtrl->GetBldc(BldcD) )
    {
        BldcD.Initialize();
    }

    double(*dDIAPMS)[3] = new double[nStor][3];
    double(*dDIAPXY)[2] = new double[nStor][2];
    double(*dRLSMSS)[2] = new double[nStor][2];

    CLateralLoad StorMass;
    BOOL bUseBedrockLevel = FALSE;
    StorMass.Make_DiaphramMassTableData(dDIAPMS, dDIAPXY, dRLSMSS, bUseBedrockLevel);

    const double dTol = 1.e-10;
    for ( int i = 0; i < nStor; ++i )
    {
        T_STOR_D StorD;
        if ( !pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD) ) { ASSERT(0); continue; }

        if ( StorD.dStoryLevel < BldcD.dBaseLevel - dTol ) continue;

        rdMassX += dDIAPMS[i][0] + dRLSMSS[i][0];
        rdMassY += dDIAPMS[i][1] + dRLSMSS[i][1];
    }
    rdMassXY = ( rdMassX + rdMassY ) / 2.0;
    rdTotal  = max(rdMassX, rdMassY);

    delete[] dDIAPMS;
    delete[] dDIAPXY;
    delete[] dRLSMSS;
    return true;
}
void CCMWindItemDlg::OnCmdBtnImport()
{
#ifndef _MGEN_US
	ASSERT(0);
#endif
	CString strFilter = _LS(IDS_CMD_EXCEL_XLSM_FILE_FILTER);

	TCHAR * szFilter = (TCHAR*)strFilter.GetBuffer();
	FILE* fp = nullptr;
	CFileDialog	fd(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if (fd.DoModal() == IDOK)
	{
		if (fd.GetFileExt() == _T("xlsm") || fd.GetFileExt() == _T("XLSM"))
		{
			CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
			ASSERT(pServiceHeadOffice);
			int nResult = pServiceHeadOffice->ReqService(_ULS(main), 2, nullptr);

            if (nResult && AddToDB()) {

                CString strServerURI = AfxGetApp()->GetProfileString(_T("CONNECTION"), _T("URI"));
                CString strMapiKey = AfxGetApp()->GetProfileString(_T("CONNECTION"), _T("Key"));
                CString strParam = _T("");
                strParam.Format(_T("start excel  \"%s\" /e/%s/%s/%d"), fd.GetPathName(), strServerURI, strMapiKey, m_Key);

                _wsystem(strParam);
            }
            IUsageCounter::Use(_T("WIND_IMP"), FALSE);
			return;
		}
	}
}