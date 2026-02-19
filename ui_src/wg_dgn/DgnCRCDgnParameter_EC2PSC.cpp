// DgnCRCDgnParameter_EC2PSC.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnCRCDgnParameter_EC2PSC.h"
//
#include "DgnCRCAnnexDgnParameterDlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCRCDgnParameter_EC2PSC dialog
using namespace dgn::def;

CDgnCRCDgnParameter_EC2PSC::CDgnCRCDgnParameter_EC2PSC(T_PSCD_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CDgnCRCDgnParameter_EC2PSC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCRCDgnParameter_EC2PSC)
	//}}AFX_DATA_INIT
	m_nPSType = 0;

	m_aBetaCalType.Add(IDC_EC2PSC_BETA_PARAMETER1);
	m_aBetaCalType.Add(IDC_EC2PSC_BETA_PARAMETER2);

	m_pParamData = pParamData;
}

CDgnCRCDgnParameter_EC2PSC::~CDgnCRCDgnParameter_EC2PSC()
{
}

void CDgnCRCDgnParameter_EC2PSC::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCRCDgnParameter_EC2PSC)  
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER2,   m_ChkOutputOpt_ULS1);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER3,   m_ChkOutputOpt_ULS2);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER4,   m_ChkOutputOpt_ULS3);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER14,  m_ChkOutputOpt_ULS4);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER6,   m_ChkOutputOpt_SLS1);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER7,   m_ChkOutputOpt_SLS2);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER8,   m_ChkOutputOpt_SLS3);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER9,   m_ChkOutputOpt_SLS4);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER10,  m_ChkOutputOpt_SLS5);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER11,  m_ChkOutputOpt_SLS6);
	//
	//DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER12, m_CmbExposureClass);
	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER16, m_CmbCementClass);
	//
	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER3,  m_RadioTndCalcMethod);
	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER4,  m_RadioTndCalcMethod2);
	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER6,  m_RadioShearMethod);
	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER7,  m_RadioShearMethod2);
	//
	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER9,  m_dStrutAngle);
	//
	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER14, m_ChkUserInputData);
	
	DDX_Radio  (pDX, IDC_EC2PSC_DESIGN_PARAMETER_PS_TYPE1_RDO, m_nPSType);

    DDX_Control(pDX, IDC_EC2PSC_NATIONAL_ANNEX_DG_EDT, m_EdtDg);
    DDX_Control(pDX, IDC_EC2PSC_NATIONAL_ANNEX_DG_UNT, m_UntDg);
    DDX_Control(pDX, IDC_EC2PSC_NATIONAL_ANNEX_KC_EDT, m_EdtKc);
    DDX_Control(pDX, IDC_EC2PSC_NATIONAL_ANNEX_PL_CMB, m_CmbPL);
    DDX_Control(pDX, IDC_EC2PSC_NATIONAL_ANNEX_SL_CMB, m_CmbSL);
    DDX_Control(pDX, IDC_EC2PSC_NATIONAL_ANNEX_STYPE_CMB, m_CmbSType);
    DDX_Control(pDX, IDC_EC2PSC_NATIONAL_ANNEX_BTYPE_CMB, m_CmbBType);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnCRCDgnParameter_EC2PSC, CMyChildDialog)
	//{{AFX_MSG_MAP(CDgnCRCDgnParameter_EC2PSC)
	ON_BN_CLICKED(IDC_EC2PSC_DESIGN_PARAMETER13,  OnClickModifyDgnParam)
	ON_BN_CLICKED(IDC_EC2PSC_PRINT_PARAMETER12,   OnClickSelectAllOutputOpt)
	ON_BN_CLICKED(IDC_EC2PSC_PRINT_PARAMETER13,   OnClickUnselectAllOutputOpt)
	ON_BN_CLICKED(IDC_EC2PSC_DESIGN_PARAMETER14,  OnCheckUserInputData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//------------------------------------------------------------------
// OnInitDialog
//------------------------------------------------------------------
BOOL CDgnCRCDgnParameter_EC2PSC::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	// Set ExposureClass Combo.
	//InitExposureClassComboSet();
	// Set CementClass Combo
	InitCementClassComboSet();

	// InitSetting!
	Initial_Data();

    SetAnnexCtrl();

	// v835 에서 막고 나가자
	GetDlgItem(IDC_EC2PSC_PRINT_PARAMETER14)->ShowWindow(SW_HIDE);	
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCRCDgnParameter_EC2PSC::Initial_Data()
{
	// Radio1 : ULS
	BOOL bFirst = FALSE;
	bFirst = m_pParamData->iTndnCalcMethod==0? TRUE : FALSE;
	m_RadioTndCalcMethod.SetCheck(bFirst);
	m_RadioTndCalcMethod2.SetCheck(!bFirst);

	// Radio2 : SLS
	bFirst = FALSE; bFirst = m_pParamData->iShearMethod==0? TRUE : FALSE;
	m_RadioShearMethod.SetCheck(bFirst);
	m_RadioShearMethod2.SetCheck(!bFirst);

	GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER7)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER11)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER12)->ShowWindow(SW_HIDE);
	
	// Beta 계산
	CDlgUtil::CtrlRadioSetCheck(this, m_aBetaCalType, m_pParamData->nBetaCalType); 

	// Edit : Strut Angle (Deg)
	m_dStrutAngle.SetUnitType(D_UNITSYS_NONE);

	// Default값 SET!!
	double dTheta = 0.0;
	dTheta = m_pParamData->dtheta;
	if(dTheta < 21.8 || dTheta > 45.0)  dTheta = 45.0;
	m_dStrutAngle.SetEditUnit(dTheta);  // Default 21.8 ~ 45.0 (Deg)

	// Cement Class를 추가하고, 변수는 기존에 있던 변수인 iCorrosiveCondition를 활용함.
    int iIndex = 0; iIndex = m_pParamData->iCorrosiveCondition;
	if(iIndex < 0 || iIndex > 2)  iIndex = 0;
	m_CmbCementClass.SetCurSel(iIndex);

	// CheckBox : Output Opt.
	m_ChkOutputOpt_ULS1.SetCheck(m_pParamData->bUltimateEuro[0]);
	m_ChkOutputOpt_ULS2.SetCheck(m_pParamData->bUltimateEuro[1]);
	m_ChkOutputOpt_ULS3.SetCheck(m_pParamData->bUltimateEuro[2]);
	m_ChkOutputOpt_ULS4.SetCheck(m_pParamData->bUltimateEuro[3]);

	m_ChkOutputOpt_SLS1.SetCheck(m_pParamData->bServiceabilityEuro[0]);
	m_ChkOutputOpt_SLS2.SetCheck(m_pParamData->bServiceabilityEuro[1]);
	m_ChkOutputOpt_SLS3.SetCheck(m_pParamData->bServiceabilityEuro[2]);
	m_ChkOutputOpt_SLS4.SetCheck(m_pParamData->bServiceabilityEuro[3]);
	m_ChkOutputOpt_SLS5.SetCheck(m_pParamData->bServiceabilityEuro[4]);
	m_ChkOutputOpt_SLS6.SetCheck(m_pParamData->bServiceabilityEuro[5]);
	//
	m_ChkUserInputData.SetCheck(m_pParamData->bUserInputData);
	//
	BOOL bChk = FALSE; bChk = m_pParamData->bUserInputData;
	GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER13)->EnableWindow(bChk);  // bUserInput인 경우 버튼을 활성화함.

	m_nPSType = m_pParamData->nPreStressStl;

	CDBDoc*	pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	BOOL bPscComposite = pDoc->m_pPostCtrl->IsPSCCompositeSect();	
	if( !bPscComposite )
	{
		GetDlgItem(IDC_EC2PSC_BETA_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EC2PSC_BETA_PARAMETER1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EC2PSC_BETA_PARAMETER2)->ShowWindow(SW_HIDE);
	}

    InitAnnexData();

	UpdateData(FALSE);
}

BOOL CDgnCRCDgnParameter_EC2PSC::Dlg2Data()
{
	UpdateData(TRUE);

	m_pParamData->iTndnCalcMethod         = m_RadioTndCalcMethod.GetCheck()? 0 : 1;
	m_pParamData->iShearMethod            = m_RadioShearMethod.GetCheck()? 0 : 1;
	m_pParamData->dtheta                  = m_dStrutAngle.GetEditValue();
	//m_pParamData->iExposureClass          = m_CmbExposureClass.GetCurSel();
	// Cement Class를 추가하고, 변수는 기존에 있던 변수인 iCorrosiveCondition를 활용함.
	m_pParamData->iCorrosiveCondition     = m_CmbCementClass.GetCurSel();
	m_pParamData->bUltimateEuro[0]        = m_ChkOutputOpt_ULS1.GetCheck();
	m_pParamData->bUltimateEuro[1]        = m_ChkOutputOpt_ULS2.GetCheck();
	m_pParamData->bUltimateEuro[2]        = m_ChkOutputOpt_ULS3.GetCheck();
	m_pParamData->bUltimateEuro[3]        = m_ChkOutputOpt_ULS4.GetCheck();
	m_pParamData->bServiceabilityEuro[0]  = m_ChkOutputOpt_SLS1.GetCheck();
	m_pParamData->bServiceabilityEuro[1]  = m_ChkOutputOpt_SLS2.GetCheck();
	m_pParamData->bServiceabilityEuro[2]  = m_ChkOutputOpt_SLS3.GetCheck();
	m_pParamData->bServiceabilityEuro[3]  = m_ChkOutputOpt_SLS4.GetCheck();
	m_pParamData->bServiceabilityEuro[4]  = m_ChkOutputOpt_SLS5.GetCheck();
	m_pParamData->bServiceabilityEuro[5]  = m_ChkOutputOpt_SLS6.GetCheck();
	//
	m_pParamData->bUserInputData          = m_ChkUserInputData.GetCheck();

	m_pParamData->nPreStressStl = m_nPSType;

	CDlgUtil::CtrlRadioGetCheck(this, m_aBetaCalType  , m_pParamData->nBetaCalType); 

    m_pParamData->dAggregateSize = m_EdtDg.GetEditValue();
    m_pParamData->dkc = m_EdtKc.GetEditValue();
    m_pParamData->enPL = static_cast<dgn::def::enProtectionLevelEU>(m_CmbPL.GetCurSel());
    m_pParamData->enSL = static_cast<dgn::def::enServiceLifeEU>(m_CmbSL.GetCurSel());
    m_pParamData->enSType = static_cast<dgn::def::enStructTypeEU>(m_CmbSType.GetCurSel());
    m_pParamData->enBType = static_cast<dgn::def::enBridgeTypeEU>(m_CmbBType.GetCurSel());
	return TRUE;
}

// Modify Annex DgnParam.
void CDgnCRCDgnParameter_EC2PSC::OnClickModifyDgnParam()
{
	CDgnCRCAnnexDgnParameterDlg dlg;
	dlg.m_pParamData  = m_pParamData;
	dlg.m_nAnnex      = m_nAnnex;
	if(dlg.DoModal()==IDOK)
	{
		m_pParamData = dlg.m_pParamData;
	}
}

// Select All Output Opt.
void CDgnCRCDgnParameter_EC2PSC::OnClickSelectAllOutputOpt()
{
	UpdateData(TRUE);
	// SET TRUE
	m_ChkOutputOpt_ULS1.SetCheck(TRUE);
	m_ChkOutputOpt_ULS2.SetCheck(TRUE);
	m_ChkOutputOpt_ULS3.SetCheck(TRUE);
	m_ChkOutputOpt_ULS4.SetCheck(TRUE);
	//
	m_ChkOutputOpt_SLS1.SetCheck(TRUE);
	m_ChkOutputOpt_SLS2.SetCheck(TRUE);
	m_ChkOutputOpt_SLS3.SetCheck(TRUE);
	m_ChkOutputOpt_SLS4.SetCheck(TRUE);
	m_ChkOutputOpt_SLS5.SetCheck(TRUE);
	m_ChkOutputOpt_SLS6.SetCheck(TRUE);

	UpdateData(FALSE);
}

// Unselect All Output Opt.
void CDgnCRCDgnParameter_EC2PSC::OnClickUnselectAllOutputOpt()
{
	UpdateData(TRUE);

	// SET TRUE
	m_ChkOutputOpt_ULS1.SetCheck(FALSE);
	m_ChkOutputOpt_ULS2.SetCheck(FALSE);
	m_ChkOutputOpt_ULS3.SetCheck(FALSE);
	m_ChkOutputOpt_ULS4.SetCheck(FALSE);
	//
	m_ChkOutputOpt_SLS1.SetCheck(FALSE);
	m_ChkOutputOpt_SLS2.SetCheck(FALSE);
	m_ChkOutputOpt_SLS3.SetCheck(FALSE);
	m_ChkOutputOpt_SLS4.SetCheck(FALSE);
	m_ChkOutputOpt_SLS5.SetCheck(FALSE);
	m_ChkOutputOpt_SLS6.SetCheck(FALSE);
	
	UpdateData(FALSE);
}

void CDgnCRCDgnParameter_EC2PSC::OnCheckUserInputData()
{
	BOOL bChk = FALSE;
	bChk = m_ChkUserInputData.GetCheck();

	GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER13)->EnableWindow(bChk);
}

void CDgnCRCDgnParameter_EC2PSC::InitCementClassComboSet()
{
	int iCountCombo = 0;
	iCountCombo = m_CmbCementClass.GetCount();
	if(iCountCombo > 0) m_CmbCementClass.ResetContent();

	CString asrtEC2ExpList[3] = {_LS(IDS_DGN_CRC_EC2PSC_CALSS_R),_LS(IDS_DGN_CRC_EC2PSC_CALSS_N),_LS(IDS_DGN_CRC_EC2PSC_CALSS_S)};
	int i=0;
	for(i=0; i<3; ++i)
	{
		m_CmbCementClass.AddString(asrtEC2ExpList[i]);
	}

	// Data가 있다면..Data에 의해서 최초 설정되도록 이후 Code추가!!
	int iIndex = 0; iIndex =m_pParamData->iCorrosiveCondition;
	if(iIndex < 0 || iIndex > 2)  iIndex = 0;
	m_CmbCementClass.SetCurSel(iIndex);
}

void CDgnCRCDgnParameter_EC2PSC::SetAnnexCtrl()
{
    InitAnnexCtrl();
    InitAnnexData();
    ShowAnnexCtrl();
    MoveWmaxCmbCtrl();
    ResizeOtherCtrl();

    return;
}

void CDgnCRCDgnParameter_EC2PSC::InitAnnexCtrl()
{
    m_EdtDg.SetUnitType(D_UNITSYS_BASE_LENGTH);
    m_UntDg.SetUnitType(D_UNITSYS_BASE_LENGTH);
    m_EdtKc.SetUnitType(D_UNITSYS_NONE);

    if ( !m_CmbPL.GetCount() )
    {
        m_CmbPL.AddString(_T("1"));
        m_CmbPL.AddString(_T("2"));
        m_CmbPL.AddString(_T("3"));
    }

    if ( !m_CmbSL.GetCount() )
    {
        m_CmbSL.AddString(_T("L100"));
        m_CmbSL.AddString(_T("L50"));
        m_CmbSL.AddString(_T("L20"));
    }

    if ( !m_CmbSType.GetCount() )
    {
        m_CmbSType.AddString(_LS(IDS_DGN_CRC_EC2PSC_ROAD_STRUCT));
        m_CmbSType.AddString(_LS(IDS_DGN_CRC_EC2PSC_RAILWAY_STRUCT));
        m_CmbSType.AddString(_LS(IDS_DGN_CRC_EC2PSC_OTHER_STRUCT));
        m_CmbSType.AddString(_LS(IDS_DGN_CRC_EC2PSC_PILE_COLM));
        m_CmbSType.AddString(_LS(IDS_DGN_CRC_EC2PSC_EARTH_STRUCT));
        m_CmbSType.AddString(_LS(IDS_DGN_CRC_EC2PSC_ANCHORAGE));
        CDlgUtil::CobxAdjustListBoxWidth(m_CmbSType);
    }

    if ( !m_CmbBType.GetCount() )
    {
        m_CmbBType.AddString(_LS(IDS_DGN_CRC_EC2PSC_ROAD_BRIDGE));
        m_CmbBType.AddString(_LS(IDS_DGN_CRC_EC2PSC_RAIL_BRIDGE));
        CDlgUtil::CobxAdjustListBoxWidth(m_CmbBType);
    }

    const auto strStc = [this] ()
    {
        switch ( m_nAnnex )
        {
        case CzechRepublic:
            return _LS(IDS_DGN_CRC_EC2PSC_PROTECT_LEVEL);
        case Sweden:
            return _LS(IDS_DGN_CRC_EC2PSC_SERVICE_LIFE);
        case Norway:
            return _LS(IDS_DGN_CRC_EC2PSC_COEFF_KC);
        case Luxembourg:
        case Belgium:
            return _LS(IDS_DGN_CRC_EC2PSC_SRUCTURE_TYPE);
        case Germany:
        case Denmark:
            return _LS(IDS_DGN_CRC_EC2PSC_BRIDGE_TYPE);
        default: return _LSX("");
        }
    } ();
    GetDlgItem(IDC_EC2PSC_NATIONAL_ANNEX_WMAX_STC)->SetWindowText(strStc);

    return;
}

void CDgnCRCDgnParameter_EC2PSC::InitAnnexData()
{
    m_EdtDg.SetEditUnit(m_pParamData->dAggregateSize);
    m_EdtKc.SetEditUnit(m_pParamData->dkc);
    m_CmbPL.SetCurSel(static_cast<int>(m_pParamData->enPL));
    m_CmbSL.SetCurSel(static_cast<int>(m_pParamData->enSL));
    m_CmbSType.SetCurSel(static_cast<int>(m_pParamData->enSType));
    m_CmbBType.SetCurSel(static_cast<int>(m_pParamData->enBType));

    return;
}

void CDgnCRCDgnParameter_EC2PSC::ShowAnnexCtrl() const
{
    const auto bDGKC = (m_nAnnex == Norway);
    GetDlgItem(IDC_EC2PSC_NATIONAL_ANNEX_DG_STC)->ShowWindow(bDGKC);
    GetDlgItem(IDC_EC2PSC_NATIONAL_ANNEX_DG_EDT)->ShowWindow(bDGKC);
    GetDlgItem(IDC_EC2PSC_NATIONAL_ANNEX_DG_UNT)->ShowWindow(bDGKC);
    GetDlgItem(IDC_EC2PSC_NATIONAL_ANNEX_KC_EDT)->ShowWindow(bDGKC);

    const auto bPL = (m_nAnnex == CzechRepublic);
    const auto bSL = (m_nAnnex == Sweden);
    const auto bSType = (m_nAnnex == Luxembourg || m_nAnnex == Belgium);
    const auto bBType = (m_nAnnex == Germany || m_nAnnex == Denmark);
    GetDlgItem(IDC_EC2PSC_NATIONAL_ANNEX_WMAX_STC)->ShowWindow(IsExistWmaxCtrl());
    GetDlgItem(IDC_EC2PSC_NATIONAL_ANNEX_PL_CMB)->ShowWindow(bPL);
    GetDlgItem(IDC_EC2PSC_NATIONAL_ANNEX_SL_CMB)->ShowWindow(bSL);
    GetDlgItem(IDC_EC2PSC_NATIONAL_ANNEX_STYPE_CMB)->ShowWindow(bSType);
    GetDlgItem(IDC_EC2PSC_NATIONAL_ANNEX_BTYPE_CMB)->ShowWindow(bBType);

    return;
}

void CDgnCRCDgnParameter_EC2PSC::MoveWmaxCmbCtrl() const
{
    if ( !IsExistWmaxCtrl() )
    {
        return;
    }

    const auto nCmbID = [this] ()
    {
        switch ( m_nAnnex )
        {
        case CzechRepublic:
            return IDC_EC2PSC_NATIONAL_ANNEX_PL_CMB;
        case Sweden:
            return IDC_EC2PSC_NATIONAL_ANNEX_SL_CMB;
        case Norway:
            return IDC_EC2PSC_NATIONAL_ANNEX_KC_EDT;
        case Luxembourg:
        case Belgium:
            return IDC_EC2PSC_NATIONAL_ANNEX_STYPE_CMB;
        case Germany:
        case Denmark:
            return IDC_EC2PSC_NATIONAL_ANNEX_BTYPE_CMB;
        default: ASSERT(0); return 0;
        }
    } ();

    CRect RectFix;
    GetDlgItem(IDC_EC2PSC_NATIONAL_ANNEX_PL_CMB)->GetWindowRect(&RectFix);

    CRect RectCmb;
    GetDlgItem(nCmbID)->GetWindowRect(&RectCmb);
    RectCmb.left = RectFix.left;

    ScreenToClient(RectCmb);
    GetDlgItem(nCmbID)->SetWindowPos(NULL, RectCmb.left, RectCmb.top, RectCmb.Width(), RectCmb.Height(), SWP_NOSIZE | SWP_NOZORDER);
    
    return;
}

bool CDgnCRCDgnParameter_EC2PSC::IsExistWmaxCtrl() const
{
    switch ( m_nAnnex )
    {
    case CzechRepublic:
    case Sweden:
    case Norway:
    case Luxembourg:
    case Belgium:
    case Germany:
    case Denmark:
        return true;
    default: return false;
    }
}

void CDgnCRCDgnParameter_EC2PSC::ResizeOtherCtrl() const
{
    const auto bDG = (m_nAnnex == Norway);
    const auto nThetaFixID = bDG ? IDC_EC2PSC_NATIONAL_ANNEX_DG_STC : IDC_EC2PSC_DESIGN_PARAMETER5;
    CRect RectThetaFix;
    GetDlgItem(nThetaFixID)->GetWindowRect(&RectThetaFix);
    const auto nThetaFixPos = bDG ? RectThetaFix.left : RectThetaFix.right;

    const auto nThetaID = IDC_EC2PSC_THETA_STC;
    CRect RectTheta;
    GetDlgItem(nThetaID)->GetWindowRect(&RectTheta);
    RectTheta.right = nThetaFixPos - 9;

    ScreenToClient(RectTheta);
    GetDlgItem(nThetaID)->SetWindowPos(NULL, RectTheta.left, RectTheta.top, RectTheta.Width(), RectTheta.Height(), SWP_NOZORDER);

    const auto bWmax = IsExistWmaxCtrl();
    const auto nCrackFixID = bWmax ? IDC_EC2PSC_NATIONAL_ANNEX_WMAX_STC : IDC_EC2PSC_DESIGN_PARAMETER15;
    CRect RectCrackFix;
    GetDlgItem(nCrackFixID)->GetWindowRect(&RectCrackFix);

    const auto nCrackID = IDC_EC2PSC_CRACK_CONTROL_GRP;
    CRect RectCrack;
    GetDlgItem(nCrackID)->GetWindowRect(&RectCrack);
    RectCrack.bottom = RectCrackFix.bottom + globalUtils.ScaleByDPI(8);

    ScreenToClient(RectCrack);
    GetDlgItem(nCrackID)->SetWindowPos(NULL, RectCrack.left, RectCrack.top, RectCrack.Width(), RectCrack.Height(), SWP_NOZORDER);

    std::vector<int> vnID;
    vnID.push_back(IDC_EC2PSC_BETA_PARAMETER1);
    vnID.push_back(IDC_EC2PSC_BETA_PARAMETER2);
    vnID.push_back(IDC_EC2PSC_BETA_STC);

    CRect RectBeta;
    GetDlgItem(IDC_EC2PSC_BETA_STC)->GetWindowRect(&RectBeta);
    
    ClientToScreen(RectCrack);
    for ( const auto nID : vnID )
    {
        CRect Rect;
        GetDlgItem(nID)->GetWindowRect(&Rect);
        Rect.top	-= RectBeta.top - RectCrack.bottom - globalUtils.ScaleByDPI(8);
        Rect.bottom -= RectBeta.top - RectCrack.bottom - globalUtils.ScaleByDPI(8);

        ScreenToClient(Rect);
        GetDlgItem(nID)->SetWindowPos(NULL, Rect.left, Rect.top, Rect.Width(), Rect.Height(), SWP_NOSIZE | SWP_NOZORDER);
    }

    return;
}