#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemDlgNew.h"
#include "SeisItemCodeMgr.h"
#include "WindSeisAddLoadDlg.h"
#include "WindSeisAddLoadGrid.h"
#include "SeisProfieDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\IUsageCounter.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\ServiceProvider.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSeisItemDlgNew::CSeisItemDlgNew(CWnd* pParent)
    : CDialogMove(CSeisItemDlgNew::IDD, pParent)
{
    m_Key = 0;
    m_Data.Initialize();
    m_bModify = FALSE;

    m_pCodeMgr = NULL;
    m_pCodeMgr = new CSeisItemCodeMgr(this);

    m_aCodeEnable.RemoveAll();

    m_pDoc = CDBDoc::GetDocPoint();
    ASSERT(m_pDoc);

    m_pGridWnd = new CWindSeisAddLoadGrid();

    m_aCtrlExRadio.Add(IDC_CMD_XDIR_POS);
    m_aCtrlExRadio.Add(IDC_CMD_XDIR_NEG);
    m_aCtrlExRadio.Add(IDC_CMD_XDIR_NONE);

    m_aCtrlEyRadio.Add(IDC_CMD_YDIR_POS);
    m_aCtrlEyRadio.Add(IDC_CMD_YDIR_NEG);
    m_aCtrlEyRadio.Add(IDC_CMD_YDIR_NONE);

    m_aMoveCtrlID.Add(IDC_CMD_DESCRIPTION_STC);
    m_aMoveCtrlID.Add(IDC_CMD_DESCRIPTION);
    m_aMoveCtrlID.Add(IDC_CMD_CHILD_FRAME);
    m_aMoveCtrlID.Add(IDC_CMD_SEISLOADFACTOR);
    m_aMoveCtrlID.Add(IDC_CMD_ACCIDENTAL_ECCENTRICITY);
    m_aMoveCtrlID.Add(IDC_CMD_TORSIONAL_AMPLIFICATION);
    m_aMoveCtrlID.Add(IDC_CMD_ADD_LOAD_STC);
    m_aMoveCtrlID.Add(IDC_CMD_BTN_PROFILE);
    m_aMoveCtrlID.Add(IDOK);
    m_aMoveCtrlID.Add(IDCANCEL);
    m_aMoveCtrlID.Add(IDC_CMD_APPLY);
    m_aMoveCtrlID.Add(IDC_CMD_XDIRECTION);
    m_aMoveCtrlID.Add(IDC_CMD_SCALE_GX);
    m_aMoveCtrlID.Add(IDC_CMD_YDIRECTION);
    m_aMoveCtrlID.Add(IDC_CMD_SCALE_GY);
    m_aMoveCtrlID.Add(IDC_CMD_ACCID_XDIRECTION);
    m_aMoveCtrlID.Add(IDC_CMD_ACCID_YDIRECTION);
    m_aMoveCtrlID.Add(IDC_CMD_ECCEN_TORSION_AMP_CHK);
    m_aMoveCtrlID.Add(IDC_CMD_BASIC_TORSION_AMP_CHK);
    m_aMoveCtrlID.Add(IDC_CMD_GRID);
    m_aMoveCtrlID.Add(IDC_CMD_BTN_ADD);
    m_aMoveCtrlID.Add(IDC_CMD_XDIR_POS);
    m_aMoveCtrlID.Add(IDC_CMD_XDIR_NEG);
    m_aMoveCtrlID.Add(IDC_CMD_XDIR_NONE);
    m_aMoveCtrlID.Add(IDC_CMD_YDIR_POS);
    m_aMoveCtrlID.Add(IDC_CMD_YDIR_NEG);
    m_aMoveCtrlID.Add(IDC_CMD_YDIR_NONE);
    m_aMoveCtrlID.Add(IDC_CMD_LIST);
    m_aMoveCtrlID.Add(IDC_CMD_BTN_DELETE);
    m_bSaveFoldedInfo = FALSE;
}

CSeisItemDlgNew::~CSeisItemDlgNew()
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

void CSeisItemDlgNew::SetParamData(T_SEIS_K Key, T_SEIS_D &ParamData)
{
    m_Key = m_OldKey = Key;
    m_Data = ParamData;
    m_bModify = TRUE;
}

void CSeisItemDlgNew::AddAdditionalLoad(T_STOR_K StartStorKey, T_STOR_K EndStorKey, double dblX, double dblY, double dblRZ)
{
    CArray<T_STOR_K, T_STOR_K> aKeyList;
    m_pDoc->m_pAttrCtrl->GetStorKeyListByConnect(aKeyList, &StartStorKey, &EndStorKey);

    // Additional Seismic Load Item 을 만든다.
    T_SEIS_ADDITION item;
    item.AddX = dblX;
    item.AddY = dblY;
    item.AddR = dblRZ;

    // Story List에서 시작 키의 Index를 찾는다.
    int nCount = aKeyList.GetSize();
    int i = 0;
    for ( i = 0; i < nCount; i++ )
        if ( aKeyList[i] == StartStorKey ) break;

    // 입력된 Seismic Load List를 만든다.
    T_STOR_K key;
    CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&> aAddition;
    do
    {
        key = aKeyList[i++];
        item.KeyStor = key;
        aAddition.Add(item);
    } while ( key != EndStorKey );

    // 새로 만들어진 List와 기존의 Seismic Load List를 Merge한다.
    int nCount1 = aAddition.GetSize();
    m_pGridWnd->GetData(m_Data.arAddition);
    int nCount2 = m_Data.arAddition.GetSize();
    int n1, n2;
    CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&> aNewAddition;
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

void CSeisItemDlgNew::DoDataExchange(CDataExchange* pDX)
{
    CDialogMove::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSeisItemDlgNew)
    DDX_Control(pDX, IDC_CMD_BASIC_TORSION_AMP_CHK, m_chkInherentTorsionAmplify);
    DDX_Control(pDX, IDC_CMD_ECCEN_TORSION_AMP_CHK, m_chkAccidentTorsionAmplify);
    DDX_Control(pDX, IDC_CMD_LOADCASE, m_wndLoadCase);
    DDX_Control(pDX, IDC_CMD_CODE_NAME, m_wndCodeName);
    DDX_Control(pDX, IDC_CMD_NA_CMB, m_cmbNationalAnnex);
    DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_wndDescription);
    DDX_Control(pDX, IDC_CMD_SCALE_GY, m_wndScaleGY);
    DDX_Control(pDX, IDC_CMD_SCALE_GX, m_wndScaleGX);
// 	DDX_Control(pDX, IDC_CMD_CHILD_FRAME, m_wndHolder);
	DDX_Control(pDX, IDC_CMD_GRID, *m_pGridWnd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisItemDlgNew, CDialogMove)
    //{{AFX_MSG_MAP(CSeisItemDlgNew)
    ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
    ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
    ON_BN_CLICKED(IDC_CMD_BTN_PROFILE, OnCmdBtnProfile)
    ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnCmdLoadcaseBtn)
    ON_CBN_SELCHANGE(IDC_CMD_CODE_NAME, OnSelchangeCmdCodeName)
    ON_CBN_SELCHANGE(IDC_CMD_NA_CMB, OnSelchangeCmdNationalAnnex)
	ON_BN_CLICKED(IDC_CMD_IMPORT_BTN, OnCmdBtnImport)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSeisItemDlgNew::OnInitDialog()
{
    CDialogMove::OnInitDialog();

    // TODO: Add extra initialization here
    m_wndLoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
    InitCodeNameCobx();
    // [SeisCode] 추가시 수정
    if ( !m_bModify )
    {
        m_Data = m_pDoc->m_pAttrCtrl->GetSeis_InitializedByNation();

        //combo box init 프리퍼런스에 맞춰서.
        T_PREFERENCE rPref;
        rPref.Initialize();
        m_pDoc->m_pInitCtrl->GetPreference(rPref);
        int nIndex = m_wndCodeName.FindString(-1, rPref.LoadCode.SLCode);
        if ( nIndex < 0 ) { ASSERT(0); nIndex=0; }
        m_wndCodeName.SetCurSel(nIndex);

        //프리퍼런스에 맞는 데이터 Init
        m_Data.nCodeType = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, m_wndCodeName.GetCurSel());

        BOOL bEnableNA = EnableNationalAnnex();
        if ( bEnableNA && m_Data.nCodeType==EURO_E_2004 && rPref.LoadCode.nSLNationalAnnex==1 )
        {
            m_Data.EURO2004.nGroundType = EN_GROUND_C;
            m_Data.EURO2004.dAg = 0.0178;
            m_Data.EURO2004.dImportance = 1.4;
        }

        // [2011-05-04] Kim, Geun Young (Tel: 2042, gykim@midasit.com) : Default 선택
        CArray<T_STLD_K, T_STLD_K> aStldK;
        m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
        for ( int i=0; i<aStldK.GetSize(); i++ )
        {
            T_STLD_D StldD;
            if ( !m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldD) ) { ASSERT(0); continue; }
            if ( StldD.LoadCaseType == _T("E") )
            {
                m_Key = aStldK[i];
                break;
            }
        }
    }
    else GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

#if defined (_MGEN_US) || defined(_DEBUG)
	GetDlgItem(IDC_CMD_IMPORT_BTN)->ShowWindow(SW_SHOW);
	if (m_Data.nCodeType == USER_E_TYPE)
		GetDlgItem(IDC_CMD_IMPORT_BTN)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_CMD_IMPORT_BTN)->EnableWindow(FALSE);
#else
	GetDlgItem(IDC_CMD_IMPORT_BTN)->ShowWindow(SW_HIDE);
#endif

    CRect rect;
    GetDlgItem(IDC_CMD_GRID)->GetWindowRect(&rect);
    m_pGridWnd->Initialize(this, TRUE, rect.Width());

    CString strName, strNewName;
    GetDlgItem(IDC_CMD_ADD_LOAD_STC)->GetWindowText(strName);

    CString strLengthUnit, strForceUnit;
    CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, strLengthUnit);
    CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_FORCE, strForceUnit);

    strNewName.Format(_T("%s (%s:%s,%s)"), strName, _LS(IDS_WG_CMD__ADDD__Unit), strForceUnit, strLengthUnit);
    GetDlgItem(IDC_CMD_ADD_LOAD_STC)->SetWindowText(strNewName);

    Data2Dlg();

    OnSelchangeCmdCodeName();

    return TRUE;
}

void CSeisItemDlgNew::OnOK()
{
    if ( !Dlg2Data() ) return;

    BOOL bSuccess;
    if ( m_bModify )
    {
        bSuccess = m_pDoc->m_pDataCtrl->ModifySeis(m_OldKey, m_Key, m_Data);
    }
    else
    {
        bSuccess = m_pDoc->m_pDataCtrl->AddSeis(m_Key, m_Data);
    }
    if ( !bSuccess ) return;

    if ( m_pDoc->m_pAttrCtrl->ExistPsll(m_Key) )
    {
        T_STLD_D DataStld;
        if ( m_pDoc->m_pAttrCtrl->GetStld(m_Key, DataStld) )
        {
            GSaveHistoryFormatNF(_LS(IDS_WG_CMD_WARNING_SEISMIC_DUPLICATE), DataStld.LoadCaseName);
        }
    }

    m_bSaveFoldedInfo = TRUE;
    CDialogMove::OnOK();
}

void CSeisItemDlgNew::OnCmdApply()
{
    AddToDB();

    if ( m_pDoc->m_pAttrCtrl->ExistPsll(m_Key) )
    {
        T_STLD_D DataStld;
        if ( m_pDoc->m_pAttrCtrl->GetStld(m_Key, DataStld) )
        {
            GSaveHistoryFormatNF(_LS(IDS_WG_CMD_WARNING_SEISMIC_DUPLICATE), DataStld.LoadCaseName);
        }
    }
}

void CSeisItemDlgNew::OnCancel()
{
    m_bSaveFoldedInfo = TRUE;
    CDialogMove::OnCancel();
}

void CSeisItemDlgNew::OnCmdBtnAdd()
{
    CWindSeisAddLoadDlg dlg;
    dlg.SetMode(nullptr, ( void* ) this, nullptr);
    dlg.DoModal();
}

void CSeisItemDlgNew::OnCmdBtnProfile()
{
    if ( !Dlg2Data() ) return;

    CSeisProfileDlg dlg;
    dlg.InitSeisProfile(m_Key, m_Data);
    dlg.DoModal();
}

void CSeisItemDlgNew::OnCmdLoadcaseBtn()
{
    AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));
}

void CSeisItemDlgNew::OnSelchangeCmdCodeName()
{
    int nOldCodeType = m_pCodeMgr->GetCurCodeType();

    int nSel = m_wndCodeName.GetCurSel();
    if ( m_aCodeEnable[nSel]==FALSE )
    {
        CDlgUtil::CobxSetCurSelItemData(m_wndCodeName, nOldCodeType);
        CString strCodeName = _T("");
        m_wndCodeName.GetLBText(nSel, strCodeName);
        CString strErrMsg = _T("");
        strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), strCodeName);
        GSaveHistoryFormatNF(strErrMsg);
        return;
    }

    BOOL bEnableNA = EnableNationalAnnex();

    int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, m_wndCodeName.GetCurSel());

    if ( nOldCodeType!=nCodeType && ( nOldCodeType==EURO_E_2004 || nCodeType==EURO_E_2004 ) )
    {
        int nDistY = GetMoveDistNA();
        if ( bEnableNA )
        {
            nDistY*=( -1 );
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

    BOOL bEnable_Accidental;  EnableCtrlOfTorsion_Accidental(nCodeType, bEnable_Accidental);
    BOOL bEnable_Inherent;  EnableCtrlOfTorsion_Inherent(nCodeType, bEnable_Inherent);

    if ( !bEnable_Accidental ) m_chkAccidentTorsionAmplify.SetCheck(FALSE);
    if ( !bEnable_Inherent ) m_chkInherentTorsionAmplify.SetCheck(FALSE);

    switch ( nCodeType )
    {
    case KBC_E_2009:
    case IBC_E_2009:
    case IBC_E_2012:
    case NSR_E_2010:
    case KBC_E_2016:
    case KDS_E_2019:
    case NSCP_E_2024:
        {
            m_chkInherentTorsionAmplify.SetCheck(FALSE);
            m_chkInherentTorsionAmplify.EnableWindow(FALSE);
        }
        break;
    default:
        {
            m_chkInherentTorsionAmplify.EnableWindow(TRUE);
        }
        break;
    }
    //EnableNationalAnnex();

#if defined (_MGEN_US) || defined(_DEBUG)
	if (nCodeType == USER_E_TYPE)
		GetDlgItem(IDC_CMD_IMPORT_BTN)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_CMD_IMPORT_BTN)->EnableWindow(FALSE);
#endif
	BOOL bExistData = m_pDoc->m_pAttrCtrl->ExistSeis(m_Key);
    if (!bExistData && !m_bModify) // 데이터가 없는 상태에서만 Default 값 변경
    {
		if (nCodeType == USER_E_TYPE || nOldCodeType == USER_E_TYPE)
		{
			m_Data.nEccen_X = (nCodeType == USER_E_TYPE) ? 2 : 0;
			m_Data.nEccen_Y = (nCodeType == USER_E_TYPE) ? 2 : 0;

			CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
			CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);
		}
    }
    
}

void CSeisItemDlgNew::OnSelchangeCmdNationalAnnex()
{
    int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, m_wndCodeName.GetCurSel());
    int nNA = CDlgUtil::CobxGetCurSelItemData(m_cmbNationalAnnex, m_cmbNationalAnnex.GetCurSel());
    m_pCodeMgr->ChangeNationalAnnex(nCodeType-1, nNA);
}

void CSeisItemDlgNew::AlignCtrl2Param(int nChildBottom)
{
    CRect rcHld;
    GetDlgItem(IDC_CMD_CHILD_FRAME)->GetWindowRect(rcHld);
    rcHld.bottom = nChildBottom;
    ScreenToClient(rcHld);
    GetDlgItem(IDC_CMD_CHILD_FRAME)->MoveWindow(rcHld);
    GetDlgItem(IDC_CMD_CHILD_FRAME)->GetWindowRect(rcHld);

    CRect rcMove;
    GetDlgItem(IDC_CMD_SEISLOADFACTOR)->GetWindowRect(rcMove);
    int nDistY = nChildBottom - rcMove.top + globalUtils.ScaleByDPI(7);

    CArray<UINT, UINT> aCtrlBelow;
    aCtrlBelow.Add(IDC_CMD_SEISLOADFACTOR);
    aCtrlBelow.Add(IDC_CMD_ACCIDENTAL_ECCENTRICITY);
    aCtrlBelow.Add(IDC_CMD_TORSIONAL_AMPLIFICATION);
    aCtrlBelow.Add(IDC_CMD_ADD_LOAD_STC);
    aCtrlBelow.Add(IDC_CMD_BTN_PROFILE);
    aCtrlBelow.Add(IDOK);
    aCtrlBelow.Add(IDCANCEL);
    aCtrlBelow.Add(IDC_CMD_APPLY);
    aCtrlBelow.Add(IDC_CMD_XDIRECTION);
    aCtrlBelow.Add(IDC_CMD_SCALE_GX);
    aCtrlBelow.Add(IDC_CMD_YDIRECTION);
    aCtrlBelow.Add(IDC_CMD_SCALE_GY);
    aCtrlBelow.Add(IDC_CMD_ACCID_XDIRECTION);
    aCtrlBelow.Add(IDC_CMD_ACCID_YDIRECTION);
    aCtrlBelow.Add(IDC_CMD_ECCEN_TORSION_AMP_CHK);
    aCtrlBelow.Add(IDC_CMD_BASIC_TORSION_AMP_CHK);
    aCtrlBelow.Add(IDC_CMD_GRID);
    aCtrlBelow.Add(IDC_CMD_BTN_ADD);
    aCtrlBelow.Add(IDC_CMD_XDIR_POS);
    aCtrlBelow.Add(IDC_CMD_XDIR_NEG);
    aCtrlBelow.Add(IDC_CMD_XDIR_NONE);
    aCtrlBelow.Add(IDC_CMD_YDIR_POS);
    aCtrlBelow.Add(IDC_CMD_YDIR_NEG);
    aCtrlBelow.Add(IDC_CMD_YDIR_NONE);
    aCtrlBelow.Add(IDC_CMD_LIST);
    aCtrlBelow.Add(IDC_CMD_BTN_DELETE);
    CDlgUtil::CtrlMoveDistY(this, aCtrlBelow, nDistY);

    CRect rcDlg, rcBtn;
    GetWindowRect(rcDlg);
    GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rcBtn);

    rcDlg.bottom = rcBtn.bottom + globalUtils.ScaleByDPI(8);
    MoveWindow(rcDlg);

    Invalidate();
}

void CSeisItemDlgNew::EnableCtrlOfTorsion_Accidental(const int nCodeType, BOOL &bEnable)
{
    // [중요] 여기가 수정되면 CEditData::CheckSeis 도 수정되어야 함.
    bEnable = FALSE;
    switch ( nCodeType )
    {
    case IBC_E_2000:	// IBC2000(ASCE7-98)
    case IBC_E_2009:	// IBC(2009)
    case IBC_E_2012:	// IBC(2012)
    case NSCP_E_2024:
    case KBC_E_2005:	// KBC(2005)
    case KBC_E_2009:	// KBC(2009)
    case KBC_E_2016:	// KBC(2016)
    case KDS_E_2019:	// KDS(2019)
    case NBC_E_1995:	// NBC1995
    case NSR_E_2010:	// NSR2010
    case TAIWAN_E_2006:	// TAIWAN(2006)
    case TAIWAN_E_2011:	// TAIWAN(2011)
    case TAIWAN_E_2022:	// TAIWAN(2011)
    case UBC_E_1997:	// UBC.1997
	case IS_E_2002:
	case IS_E_2016:
	case IS_E_2025:
	case USER_E_TYPE:
        bEnable = TRUE;
        break;
    default:
        break;
    }

    GetDlgItem(IDC_CMD_ECCEN_TORSION_AMP_CHK)->EnableWindow(bEnable);
}

void CSeisItemDlgNew::EnableCtrlOfTorsion_Inherent(const int nCodeType, BOOL &bEnable)
{
    // [중요] 여기가 수정되면 CEditData::CheckSeis 도 수정되어야 함.
    bEnable = FALSE;
    switch ( nCodeType )
    {
    case IBC_E_2000:	// IBC2000(ASCE7-98)
    case IBC_E_2009:	// IBC(2009)
    case IBC_E_2012:	// IBC(2012)
    case NSCP_E_2024:
    case KBC_E_2005:	// KBC(2005)
    case NBC_E_1995:	// NBC1995
    case NTC_E_2008:	// NTC2008
    case NTC_E_2012:	// NTC2012
    case NTC_E_2017:	// NTC2018
    case TAIWAN_E_2006:	// TAIWAN(2006)
    case TAIWAN_E_2011:	// TAIWAN(2011)
    case TAIWAN_E_2022:	// TAIWAN(2011)
    case UBC_E_1997:	// UBC.1997
	case IS_E_2002:
	case IS_E_2016:
	case IS_E_2025:
	case USER_E_TYPE:
        bEnable = TRUE;
        break;
    default:
        break;
    }

    GetDlgItem(IDC_CMD_BASIC_TORSION_AMP_CHK)->EnableWindow(bEnable);
}

void CSeisItemDlgNew::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

BOOL CSeisItemDlgNew::AddToDB()
{
	if (!Dlg2Data()) return FALSE;

	if (m_bModify)
	{
		return m_pDoc->m_pDataCtrl->ModifySeis(m_OldKey, m_Key, m_Data);
	}
	else
	{
		return m_pDoc->m_pDataCtrl->AddSeis(m_Key, m_Data);
	}
}
void CSeisItemDlgNew::UpdateBuffer()
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
	T_SEIS_K SeisK;
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

				if (m_Data.nCodeType == USER_E_TYPE)
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

				if (m_Data.nCodeType == USER_E_TYPE)
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
        case UR_SEIS_ADD:
		case UR_SEIS_MFS:
		case UR_SEIS_MFD:
			{
				m_pDoc->m_pViewBuff->GetSeis(nKey, SeisK, m_Data);
				bDataChanged = TRUE;
			}
			break;
        default:
            break;
        }
    } // end of while

    if ( bStorChanged )
    {
        // while문에서는 Data 변경만 Update했다. 이 경우 Story 순서가 바뀌는 경우가
        // 발생할 수 도 있으므로 Sorting을 한다.
        CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&> aAddition;
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
	if (m_Data.nCodeType == USER_E_TYPE && (bStorChanged || bDataChanged))
	{
		m_pCodeMgr->Data2CodeDlg(m_Data, USER_E_TYPE - 1, TRUE);
	}
}

void CSeisItemDlgNew::InitCodeNameCobx()
{
    m_wndCodeName.ResetContent();

    CStringArray aCodeName;
    CArray<int, int> aCodeIndex;
    m_aCodeEnable.RemoveAll();
    int nNum = m_pCodeMgr->GetCodeNameList(aCodeName, aCodeIndex, &m_aCodeEnable);

    for ( int i = 0; i < nNum; i++ )
    {
        CDlgUtil::CobxAddItem(m_wndCodeName, aCodeName[i], aCodeIndex[i]);
    }
    m_wndCodeName.SetCurSel(0);

    m_cmbNationalAnnex.ResetContent();
    CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_PREF_RECOMMENDED), EN_RECOMMENDED);
    CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_PREF_SINGAPORE), EN_SINGAPORE);
    CDlgUtil::CobxAddItem(m_cmbNationalAnnex, _LS(IDS_PREF_MALAYSIA), EN_MALESIA);

    m_cmbNationalAnnex.SetCurSel(0);

    AdjustComboListBox();
}

BOOL CSeisItemDlgNew::EnableNationalAnnex()
{
    int nIndex = m_wndCodeName.GetCurSel();
    CString strCurrentCode=_T("");
    m_wndCodeName.GetLBText(nIndex, strCurrentCode);
    BOOL bEnable = CDBLib::Is_LoadCode_NationalAnnex(strCurrentCode);
    GetDlgItem(IDC_CMD_NA_STC)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_NA_CMB)->EnableWindow(bEnable);

    GetDlgItem(IDC_CMD_NA_STC)->ShowWindow(bEnable);
    GetDlgItem(IDC_CMD_NA_CMB)->ShowWindow(bEnable);

    return bEnable;
}

void CSeisItemDlgNew::AdjustComboListBox()
{
    CString str;
    CSize   sz;
    int     dx=0;
    CDC*    pDC = m_wndCodeName.GetDC();
    for ( int i=0; i < m_wndCodeName.GetCount(); i++ )
    {
        m_wndCodeName.GetLBText(i, str);
        sz = pDC->GetTextExtent(str);

        if ( sz.cx > dx ) dx = sz.cx;
    }
    m_wndCodeName.ReleaseDC(pDC);

    if ( m_wndCodeName.GetDroppedWidth() < dx )
    {
        m_wndCodeName.SetDroppedWidth(dx);
        ASSERT(m_wndCodeName.GetDroppedWidth() == dx);
    }
}

void CSeisItemDlgNew::Data2Dlg()
{
    m_wndLoadCase.ChangeSelect(D_LOADCASE_STATIC, m_Key);

    int nNum = m_wndCodeName.GetCount();
    int i = 0;
    for ( i=0; i<nNum; i++ )
    {
        if ( m_wndCodeName.GetItemData(i) == m_Data.nCodeType ) break;
    }
    ASSERT(i != nNum);
    m_wndCodeName.SetCurSel(i);

    CString csVal;
    m_wndDescription.SetWindowText(m_Data.Description);
    csVal.Format(_T("%g"), m_Data.ScaleFactor_X);
    m_wndScaleGX.SetWindowText(csVal);
    csVal.Format(_T("%g"), m_Data.ScaleFactor_Y);
    m_wndScaleGY.SetWindowText(csVal);

    CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
    CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);

    m_chkAccidentTorsionAmplify.SetCheck(m_Data.bAccidentTorsionAmplify);
    m_chkInherentTorsionAmplify.SetCheck(m_Data.bInherentTorsionAmplify);

    BOOL bEnableNA = EnableNationalAnnex();
    
    if ( bEnableNA )
    {
        CDlgUtil::CobxSetCurSelItemData(m_cmbNationalAnnex, m_Data.EURO2004.nNationalAnnex);
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

    int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, i);
    m_pCodeMgr->Data2CodeDlg(m_Data, nCodeType - 1, m_bModify);

    BOOL bEnable;
    EnableCtrlOfTorsion_Accidental(nCodeType, bEnable); // bEnable은 여기에서는 사용하지 않음.
    EnableCtrlOfTorsion_Inherent(nCodeType, bEnable);   // bEnable은 여기에서는 사용하지 않음.

    MakeItemEx();
}

BOOL CSeisItemDlgNew::Dlg2Data()
{
    UINT nLoadCaseType;
    if ( !m_wndLoadCase.GetSelectedLoad(nLoadCaseType, m_Key) )
    {
        AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
        return FALSE;
    }

    m_Data.Initialize();

    if ( !m_pCodeMgr->CodeDlg2Data(m_Data) ) return FALSE;

    int nIndex = m_wndCodeName.GetCurSel();
    if ( nIndex == CB_ERR ) return FALSE;
    m_Data.nCodeType = m_wndCodeName.GetItemData(nIndex);
	BOOL bEnableNA = EnableNationalAnnex();

	if (bEnableNA)
	{
        m_Data.EURO2004.nNationalAnnex = (EN_SEIS_NA)CDlgUtil::CobxGetCurSelItemData(m_cmbNationalAnnex, m_cmbNationalAnnex.GetCurSel());
	}

    CString csVal;
    m_wndDescription.GetWindowText(m_Data.Description);
    m_wndScaleGX.GetWindowText(csVal);
    m_Data.ScaleFactor_X = _ttof(csVal);
    m_wndScaleGY.GetWindowText(csVal);
    m_Data.ScaleFactor_Y = _ttof(csVal);

    CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
    CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);

    m_Data.bAccidentTorsionAmplify = m_chkAccidentTorsionAmplify.GetCheck();
    m_Data.bInherentTorsionAmplify = m_chkInherentTorsionAmplify.GetCheck();

    m_pGridWnd->GetData(m_Data.arAddition);

    return TRUE;
}

void CSeisItemDlgNew::MakeItemEx()
{
    m_pGridWnd->SetData(m_Data.arAddition);
    m_pGridWnd->MakeItemEx();
}

int CSeisItemDlgNew::GetMoveDistNA()
{
    CRect rNa;
    GetDlgItem(IDC_CMD_NA_CMB)->GetWindowRect(rNa);
    return ( -1.2 ) * rNa.Height() + 2;
}

void CSeisItemDlgNew::OnCmdBtnImport()
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
			// 현재 다이얼로그와 DB 동기화를 위해 Apply 처리
			CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
			ASSERT(pServiceHeadOffice);
			int nResult = pServiceHeadOffice->ReqService(_ULS(main), 2, nullptr);
            if (nResult && AddToDB()) {

                CString strServerURI = AfxGetApp()->GetProfileString(_T("CONNECTION"), _T("URI"));
                CString strMapiKey = AfxGetApp()->GetProfileString(_T("CONNECTION"), _T("Key"));
                CString strParam = _T("");
                strParam.Format(_T("start excel \"%s\" /e/%s/%s/%d"), fd.GetPathName(), strServerURI, strMapiKey, m_Key);
                _wsystem(strParam);
            }
            IUsageCounter::Use(_T("SEIS_IMP"), FALSE);
			return;
		}
	}
}