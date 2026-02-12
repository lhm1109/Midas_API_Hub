// TdmeItemDlg.cpp : implementation file
//

#include <math.h>
#include "stdafx.h"
#include "wg_cmd.h"
#include "TdmeItemDlg.h"
#include "CMRefWD_KDS2016.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "..\wg_main\wg_mainres2.h"

#include "..\wg_base\TestEnvMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTdmeItemDlg dialog
//  0                  1      2         3        4                  5       6        7                    8           9             10            22           24                   25                   26         27                  35
// _T("Korean Standard"), _T("ACI"), _T("CEB-FIP(1990)"),_T("Ohzagi") _T("Chinese Standard") _T("Japan") _T("Japan2") _T("INDIA(IRC:18-2000)") _T("European") _T("CEB-FIP(1978)")_T("KCI-USD12") _T("AS 3600-2009") _T("AS/RTA 5100.5-2011") _T("INDIA(IRC:112-2011)") _T("Russian") _T("CEB-FIP(1978)")_T("INDIA(IRC:112-2020)")
// CDBLib::GetHmtpCodeID, GetHmtpCodeName�� �ִ� ��ȣ�� Match ��ų ��
// �� ���� �ٲٸ� �ȵ�
// ���� �߰��Ǹ� CDBLib�� �ִ� �� �� �Լ��� �Բ� ������ ��
// Chinese Standard�� �־��ٰ� ������Բ��� ��� ���Ƶζ�� �ϼ���

// #if defined(_US)
// int nSeqTdme[] = {1, 2, 3, 7, 8, 9,              22, 24, 25, 26};
// #elif defined(_RUS)
// int nSeqTdme[] = {26, 1, 2, 3, 7, 8, 9,              22, 24, 25};
// #elif defined(_JP)
// int nSeqTdme[] = {3, 1, 2, 0, 5, 6, 7, 8, 9,     22, 24, 25, 26};
// #elif defined(_CH)
// int nSeqTdme[] = {1, 2, 3, 0, 5, 6, 7, 8, 9,     22, 24, 25, 26};
// #else
// int nSeqTdme[] = {10, 0, 1, 2, 3, 5, 6, 7, 8, 9, 22, 24, 25, 26};
// #endif 

CTdmeItemDlg::CTdmeItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CTdmeItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTdmeItemDlg)
	m_type = -1;
	m_csTypeName = _T("");
	m_nJpnCementType = 0;
	m_nJpnGraphType = 0;
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_bGridInit = FALSE;
	m_nRusCuringMeth = 0;
	m_nRusConcType = 0;

	m_aUserCtrl.Add(IDC_TDME_TABLE);

	m_aJPN2CodeRadioCtrl.Add(IDC_TDME_JPN2_CEMENTTYPE_RDO1);
	m_aJPN2CodeRadioCtrl.Add(IDC_TDME_JPN2_CEMENTTYPE_RDO2);

	m_aButtonCtrl.Add(IDC_TDME_SIN_REDRAW_GRAPH);
	m_aButtonCtrl.Add(IDC_CMD_DYGEN);
	m_aButtonCtrl.Add(IDOK);
	m_aButtonCtrl.Add(IDCANCEL);
}


void CTdmeItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTdmeItemDlg)
	DDX_Control(pDX, IDC_TDME_CH_S28UNIT, m_dCHCompStrengthUnit);
	DDX_Control(pDX, IDC_TDME_CH_S28EDIT, m_dCHCompStrength);
	DDX_Control(pDX, IDC_ETC_TDME_CHART_YLog, m_btnYLog);
	DDX_Control(pDX, IDC_ETC_TDME_CHART_XLog, m_btnXLog);
	DDX_Control(pDX, IDC_TDME_OHZ_S28UNIT, m_dOHZCompStrengthUnit);
	DDX_Control(pDX, IDC_TDME_OHZ_S28EDIT, m_dOHZCompStrength);
	DDX_Control(pDX, IDC_TDME_OHZ_COMBO, m_OHZCementType);
	DDX_Control(pDX, IDC_TDME_KS_S91UNIT, m_dKSDCompStrengthUnit);
	DDX_Control(pDX, IDC_TDME_KS_S91EDIT, m_dKSDCompStrength);
	DDX_Control(pDX, IDC_TDME_KS_FACTOR_BEDIT, m_dKSDFactorB);
	DDX_Control(pDX, IDC_TDME_KS_FACTOR_AEDIT, m_dKSDFactorA);
	DDX_Control(pDX, IDC_TDME_CEB_S28UNIT, m_dCEBCompStrengthUnit);
	DDX_Control(pDX, IDC_TDME_CEB_S28EDIT, m_dCEBCompStrength);
	DDX_Control(pDX, IDC_TDME_CEB_COMBO, m_CEBCementType);
	DDX_Control(pDX, IDC_TDME_BTN_ADD,   m_CEBAffreType);
	DDX_Control(pDX, IDC_TDME_ACI_S28UNIT, m_dACICompStrengthUnit);
	DDX_Control(pDX, IDC_TDME_ACI_S28EDIT, m_dACICompStrength);
	DDX_Control(pDX, IDC_TDME_ACI_FACTOR_BEDIT, m_dACIFactorB);
	DDX_Control(pDX, IDC_TDME_ACI_FACTOR_AEDIT, m_dACIFactorA);
	DDX_Control(pDX, IDC_TDME_SCALE, m_wndScaleFactor);
	DDX_Control(pDX, IDC_TDME_CODE_COMBO, m_CobxCode);
	DDX_Radio(pDX, IDC_TDME_TYPE_CODE, m_type);
	DDX_Text(pDX, IDC_TDME_FUNC_NAME, m_csTypeName);
	DDX_Control(pDX, IDC_TDME_JAPAN_S28EDIT, m_dJAPANCompStrength);
	DDX_Control(pDX, IDC_TDME_JAPAN_FACTOR_AEDIT, m_dJAPANFactorA);
	DDX_Control(pDX, IDC_TDME_JAPAN_FACTOR_BEDIT, m_dJAPANFactorB);
	DDX_Control(pDX, IDC_TDME_JAPAN_FACTOR_DEDIT, m_dJAPANFactorD);
	DDX_Control(pDX, IDC_TDME_JAPAN_S28UNIT, m_dJAPANCompStrengthUnit);
	DDX_Control(pDX, IDC_TDME_JAPAN_CONCRETE_CHK, m_btnConcreteData);
	DDX_Control(pDX, IDC_TDME_JAPAN_CEMENT_TYPE_CMB, m_JAPANCementType);
	DDX_Radio(pDX, IDC_TDME_JPN2_CEMENTTYPE_RDO1, m_nJpnCementType);
	DDX_Control(pDX, IDC_TDME_JPN2_E28EDIT, m_dJPN2CompStrength);
	DDX_Control(pDX, IDC_TDME_JPN2_E28UNIT, m_dJPN2CompStrengthUnit);
	DDX_Control(pDX, IDC_TDME_JAPAN_TENSILE, m_dJpnTensileStrength);
	DDX_Radio(pDX, IDC_CMD_TDME_GRAPHTYPE_RDO1, m_nJpnGraphType);
	DDX_Control(pDX, IDC_TDME_INDIA_S28EDIT, m_dINDIACompStrength);
	DDX_Control(pDX, IDC_TDME_INDIA_S28UNIT, m_dINDIACompStrengthUnit);
	DDX_Control(pDX, IDC_TDME_EUROPEAN_S28UNIT, m_dEUROPEANCompStrengthUnit);
	DDX_Control(pDX, IDC_TDME_EUROPEAN_S28EDIT, m_dEUROPEANCompStrength);
	DDX_Control(pDX, IDC_TDME_EUROPEAN_COMBO, m_EUROPEANCementType);
	DDX_Control(pDX, IDC_TDME_CEBFIP_1978_UNIT, m_dCEBFIP1978CompStrengthUnit);
	DDX_Control(pDX, IDC_TDME_CEBFIP_1978_EDIT, m_dCEBFIP1978CompStrength);
	DDX_Control(pDX, IDC_TDME_IND2011_S28UNIT, m_dIND2011CompStrengthUnit); // MNET:XXXX-JWKWON-20130909 : �ε� �ð����� ����
	DDX_Control(pDX, IDC_TDME_IND2011_S28EDIT, m_dIND2011CompStrength);
    DDX_Control(pDX, IDC_TDME_BTN_ADD2, m_dIND2011AffreType);
	DDX_Control(pDX, IDC_TDME_IND2011_COMBO, m_IND2011CementType);
	DDX_Control(pDX, IDC_TDME_RUS_CONC_B_EDT, m_edtRusConcB);
	DDX_Control(pDX, IDC_TDME_RUS_CONC_B_UNT, m_untRusConcB);
	DDX_Control(pDX, IDC_TDME_RUS_CEMENT_CMB, m_cmbRusCementType);
	DDX_Radio  (pDX, IDC_TDME_RUS_CMETH_RDO1, m_nRusCuringMeth);
	DDX_Radio  (pDX, IDC_TDME_RUS_CONC_TYPE_RDO1, m_nRusConcType);
	DDX_Control(pDX, IDC_TDME_RUS_MAX_SIZE_EDT, m_edtRusMaxSize);
	DDX_Control(pDX, IDC_TDME_RUS_MAX_SIZE_UNT, m_untRusMaxSize);
	DDX_Control(pDX, IDC_TDME_RUS_PZ_EDT, m_edtRusPz);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	//DDX_Control(pDX, IDC_CMD_TDME_KDS_2016_MATL_CMB, m_cmbMatl);
	DDX_Control(pDX, IDC_CMD_TDME_KDS_2016_DENSITY_EDT, m_edtDensity);

	DDX_Control(pDX, IDC_TDME_GILBERT_FCMI_EDT,        m_dGilbetCompStrength);
	DDX_Control(pDX, IDC_TDME_GILBERT_FCMI_UNT,        m_dGilbetCompStrengthUnit);
	DDX_Control(pDX, IDC_TDME_GILBERT_CEMENT_CMB,      m_GilbertCementType);
	DDX_Control(pDX, IDC_TDME_TABLE, m_wndGrid);
	DDX_Control(pDX, IDC_CMD_BITMAP, m_wndPicture);

	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		m_ParamData.strMatlName = m_csTypeName;
		m_ParamData.nMatlType = m_type+1;
		if (!m_wndScaleFactor.GetEditValue(m_ParamData.dScaleFactor))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_));
			pDX->Fail();
		}
//		m_ParamData.dSpecificHeat = m_SpecHeatEdit.GetEditValue();
//		m_ParamData.dHeatConduction = m_HeatCondEdit.GetEditValue();
		if (m_ParamData.nMatlType != 2) Dlg2Data();
	}
	else
	{
		if (!m_bGridInit) return;
		BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
		ROWCOL nCount = m_ParamData.aFuncData.GetSize();
		m_wndGrid.SetRowCount(nCount+1);
		m_wndGrid.SetCurrentCell(nCount+1, 1);

		m_wndGrid.LockUpdate(bOldLock);
		if (!bOldLock) m_wndGrid.Redraw();
	}
}

void CTdmeItemDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
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

void CTdmeItemDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	POSITION pos = pViewBuff->GetStartBuffer();
	T_UDRD_BUFFER buffer_ur;
	BOOL bMATL = FALSE;
	while(pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case UR_MATL_ADD:
		case UR_MATL_DEL:
		case UR_MATL_MFD:
		case UR_MATL_MFS:
			//InitMatlCombo();
			//OnTdmeMatlCmb();
			break;
		default:
			break;
		}
	}
}


BEGIN_MESSAGE_MAP(CTdmeItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CTdmeItemDlg)
	ON_BN_CLICKED(IDC_TDME_TYPE_CODE, OnTdmeSelChangeType)
	ON_CBN_SELCHANGE(IDC_TDME_CODE_COMBO, OnSelchangeTdmeCodeCombo)
	ON_BN_CLICKED(IDC_TDME_SIN_REDRAW_GRAPH, OnTdmeSinRedrawGraph)
	ON_BN_CLICKED(IDC_ETC_TDME_CHART_XLog, OnETCTDMECHARTXLog)
	ON_BN_CLICKED(IDC_ETC_TDME_CHART_YLog, OnETCTDMECHARTYLog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_TDME_JAPAN_CONCRETE_CHK, OnTdmeJPNConcreteChk)
	ON_CBN_SELCHANGE(IDC_TDME_JAPAN_CEMENT_TYPE_CMB, OnSelchangeJapanCementCombo)
	ON_BN_CLICKED(IDC_TDME_TYPE_USER, OnTdmeSelChangeType)
	ON_BN_CLICKED(IDC_CMD_TDME_GRAPHTYPE_RDO1, OnTdmeSinRedrawGraph)
	ON_BN_CLICKED(IDC_CMD_TDME_GRAPHTYPE_RDO2, OnTdmeSinRedrawGraph)
	ON_BN_CLICKED(IDC_CMD_TDME_GRAPHTYPE_RDO3, OnTdmeSinRedrawGraph)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	//ON_CBN_SELCHANGE(IDC_CMD_TDME_KDS_2016_MATL_CMB, OnTdmeMatlCmb)
	ON_BN_CLICKED(IDC_CMD_TDME_KDS_2016_MATL_BTN, OnTdmeMatlBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTdmeItemDlg message handlers
BOOL CTdmeItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(m_CurrUnit);

	CDlgUtil::GetCtrlIDByIncRect(this, m_aCodeCtrl          , IDC_TDME_FUNC_FRAME            , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aKSDCodeCtrl       , IDC_TDME_FUNC_KS_FRAME         , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aACICodeCtrl       , IDC_TDME_FUNC_ACI_FRAME        , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCEBCodeCtrl       , IDC_TDME_FUNC_CEB_FRAME        , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aOHZCodeCtrl       , IDC_TDME_FUNC_OHZ_FRAME        , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCHCodeCtrl        , IDC_TDME_FUNC_CH_FRAME         , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aJAPANCodeCtrl     , IDC_TDME_FUNC_JAPAN_FRAME      , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aJPN2CodeCtrl      , IDC_TDME_FUNC_JPN2_FRAME       , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aINDIACodeCtrl     , IDC_TDME_FUNC_INDIA_FRAME      , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aEUROPEANCodeCtrl  , IDC_TDME_FUNC_EUROPEAN_FRAME   , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCEBFIP1978CodeCtrl, IDC_TDME_FUNC_CEBFIP_1978_FRAME, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_arGraphtype        , IDC_CMD_TDME_JAPAN_TYPE        , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aIND2011CodeCtrl   , IDC_TDME_FUNC_IND2011_FRAME    , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aRussiaCodeCtrl    , IDC_TDME_FUNC_RUS_FRAME        , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aGilbertCtrl       , IDC_TDME_FUNC_GILBERT_FRAME    , TRUE);

	//m_aKDS2016Ctrl.Add(IDC_CMD_TDME_KDS_2016_MATL_STC);   //KDS2016, GILBERT AND RANZI
	//m_aKDS2016Ctrl.Add(IDC_CMD_TDME_KDS_2016_MATL_CMB);
	m_aKDS2016Ctrl.Add(IDC_CMD_TDME_KDS_2016_DENSITY_STC);
	m_aKDS2016Ctrl.Add(IDC_CMD_TDME_KDS_2016_DENSITY_EDT);
	m_aKDS2016Ctrl.Add(IDC_CMD_TDME_KDS_2016_DENSITY_UNT);			
	m_aKDS2016Ctrl.Add(IDC_CMD_TDME_KDS_2016_MATL_BTN);

	AlignControls();

	// init unit /////////////////////////////////////////////////
	m_dOHZCompStrengthUnit.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_dKSDCompStrengthUnit.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_dCEBCompStrengthUnit.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_dACICompStrengthUnit.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_dJAPANCompStrengthUnit.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_dJPN2CompStrengthUnit.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_dINDIACompStrengthUnit.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_dEUROPEANCompStrengthUnit.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_dCEBFIP1978CompStrengthUnit.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_dIND2011CompStrengthUnit.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_edtRusConcB.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_untRusConcB.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_dGilbetCompStrength.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_dGilbetCompStrengthUnit.SetUnitType(CUnitCtrl::m_TDME_UNIT.dCompStrength);
	m_edtRusMaxSize.SetUnitType(CUnitCtrl::m_TDME_UNIT.dMaxSize);
	m_untRusMaxSize.SetUnitType(CUnitCtrl::m_TDME_UNIT.dMaxSize);
	m_edtRusPz.SetUnitType(D_UNITSYS_NONE);
	m_edtDensity.SetUnitType(D_UNITSYS_NONE);
	//////////////////////////////////////////////////////////////
	

//   CString strPropType[] = {_LS(IDS_WG_CMD__ADDD__Korea_Standard),         _LS(IDS_WG_CMD__ADDD__ACI_STANDARD), 
//                            _LS(IDS_WG_CMD__ADDD__CEBFIP_STANDARD),        _LS(IDS_WG_CMD__ADDD__OHZAGI_STANDARD),
//                            _LS(IDS_CMD0417__Chinese_Standard),            _LS(IDS_WG_CMD__ADDD__JAPAN_HYDRATION_STANDARD),
//                            _LS(IDS_WG_CMD__ADDD__JAPAN_ELASTIC_STANDARD), _LS(IDS_WG_CMD__ADDD__INDIA_STANDARD),
//                            _LS(IDS_WG_CMD__ADDD__EUROPEAN),               _LS(IDS_WG_CMD__ADDD__CEBFIP_1978_STANDARD),
// 	                         _LS(IDS_WG_CMD__ADDD__KCI_USD12_STANDARD),     _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009),
//                            _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011)};
//   
//   int nSize = sizeof(nSeqTdme) / sizeof(int);
//   for(int i = 0; i < nSize; i++)
//   {
//     CDlgUtil::CobxAddItem(m_CobxCode, strPropType[nSeqTdme[i]], nSeqTdme[i]);
//   }

	// ���� �̸� ����
	SetComboCode();

	// Japan Cement Type ////////////////////////////////////////////
	m_JAPANCementType.AddString(_LS(IDS_CMD_TDME_JAPAN_CEMENT_TYPE_NORMAL_PORTLAND_CEMENT));
	m_JAPANCementType.AddString(_LS(IDS_CMD_TDME_JAPAN_CEMENT_TYPE_MODERATE_PORTLAND_CEMENT));
	m_JAPANCementType.AddString(_LS(IDS_CMD_TDME_JAPAN_CEMENT_TYPE_HIGH_EARLY_STRENGTH_PORTLAND_CEMENT));
	m_JAPANCementType.SetCurSel(0);

	// European Cement Type ////////////////////////////////////////
	m_EUROPEANCementType.AddString(_LS(IDS_CMD_TDME_EUROPEAN_CEMENT_TYPE_CLASS_R_020));
	m_EUROPEANCementType.AddString(_LS(IDS_CMD_TDME_EUROPEAN_CEMENT_TYPE_CLASS_N_025));
	m_EUROPEANCementType.AddString(_LS(IDS_CMD_TDME_EUROPEAN_CEMENT_TYPE_CLASS_S_038));
	m_EUROPEANCementType.SetCurSel(1);

	m_IND2011CementType.AddString(_LS(IDS_CMD_ETC_TDME_IRC_CEMENT_TYPE_SLOW));
	m_IND2011CementType.AddString(_LS(IDS_CMD_ETC_TDME_IRC_CEMENT_TYPE_NORMAL));
	m_IND2011CementType.AddString(_LS(IDS_CMD_ETC_TDME_IRC_CEMENT_TYPE_RAPID));
	m_IND2011CementType.SetCurSel(0);

	m_cmbRusCementType.AddString(_LS(IDS_CMD_TDME_RUSSIA_CEMENT_TYPE_NORMAL));
	m_cmbRusCementType.AddString(_LS(IDS_CMD_TDME_RUSSIA_CEMENT_TYPE_FAST_HARDENED));
	m_cmbRusCementType.AddString(_LS(IDS_CMD_TDME_RUSSIA_CEMENT_TYPE_SLAG));
	m_cmbRusCementType.AddString(_LS(IDS_CMD_TDME_RUSSIA_CEMENT_TYPE_POZZOLAN));
	m_cmbRusCementType.SetCurSel(0);

	CString str;
	CSize sz;
	int dx = 0;
	CDC* pDC = m_JAPANCementType.GetDC();
	for (int i=0; i<m_JAPANCementType.GetCount(); i++)
	{
		m_JAPANCementType.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_JAPANCementType.ReleaseDC(pDC);
	m_JAPANCementType.SetDroppedWidth(dx);

	OnTdmeJPNConcreteChk();

	//InitMatlCombo();
	/////////////////////////////////////////////////////////////////

	if(!m_bModify)
	{
		m_ParamData.Initialize();
		//m_ParamData.strCode = strPropType[nSeqTdme[0]];
		int ntest = m_CobxCode.GetCurSel();
		int nCode = CDlgUtil::CobxGetCurSelItemData(m_CobxCode, ntest);
		m_ParamData.strCode = CDBLib::GetHmtpCodeName(nCode);
		m_ParamData.dMaxSize = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthOut(0.02);	// Default 20mm

		//m_CEBCementType.SetCurSel(1);
		m_OHZCementType.SetCurSel(1);
		m_edtRusMaxSize.SetEditUnit(m_ParamData.dMaxSize);
		m_edtRusPz.SetEditUnit(m_ParamData.dPz);
	}

	Data2Dlg();

	InitGrid();
//  m_LogFlagX = FALSE;
//  m_LogFlagY = FALSE;
	InitChart();
	m_wndGrid.MakeItemEx();

	OnTdmeSelChangeType();
	//if(!m_bModify) OnTdmeMatlCmb();	

	GetDlgItem(IDC_TDME_FUNC_FRAME)->ShowWindow(FALSE);

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US �������� �ݿ�

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTdmeItemDlg::SetComboCode()
{
// 1~10���� (���� ��ȣ+1) �� �����Ѵ�.
// ȣ�� ������� ���ʹ� T_TDMT_COMMON nCode ��ȣ�� ������.

	m_CobxCode.ResetContent();

#if defined(_US)
	{
		//int nSeqTdme[] = {1, 2, 3, 7, 8, 9, 22, 24, 25, 26};
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ACI_STANDARD)        ,      2);  //1(����)
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP2010_STANDARD) ,     27);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP_STANDARD)     ,      3);  //2
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__OHZAGI_STANDARD)     ,      4);  //3
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__INDIA_STANDARD)      ,      8);  //7
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__EUROPEAN)            ,      9);  //8
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP_1978_STANDARD),     10);  //9
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2017),          TDMT_CODE_AUSTRALIA_2017);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2016),          TDMT_CODE_AUSTRALIA_2016);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011),          TDMT_CODE_AUSTRALIA_2011);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_3600_2018_AMD), TDMT_CODE_AUSTRALIA_2018_AMD_2021);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009),          TDMT_CODE_AUSTRALIA_2009);    
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2011)")                         ,  25);       // wg_mkor�� ���ڿ� ������ ��!!
        CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2020)")                         ,  35);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__RUSSIAN), TDMT_CODE_RUSSIAN);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__Gilbert_and_Ranzi_STANDARD) ,  34);	
#ifdef _CIVIL
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__JAPAN_HYDRATION_STANDARD),  6);  //5
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__JAPAN_ELASTIC_STANDARD)  ,  7);  //6
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KDS-2016")                                     , 33);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__KCI_USD12_STANDARD)      , 11);  //10(����)
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__Korea_Standard)          ,  1);  //0
#endif // _CIVIL
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ABNTNBR61182023), TDMT_CODE_ABNT_NBR_6118_2023);  //40
	}
#elif defined(_RUS)
	{
		//int nSeqTdme[] = {26, 1, 2, 3, 7, 8, 9, 22, 24, 25};
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__RUSSIAN), TDMT_CODE_RUSSIAN);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ACI_STANDARD)        ,      2);  //1(����)
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP2010_STANDARD) ,     27);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP_STANDARD)     ,      3);  //2
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__OHZAGI_STANDARD)     ,      4);  //3
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__INDIA_STANDARD)      ,      8);  //7
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__EUROPEAN)            ,      9);  //8
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP_1978_STANDARD),     10);  //9
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2017),          TDMT_CODE_AUSTRALIA_2017);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2016),          TDMT_CODE_AUSTRALIA_2016);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011),          TDMT_CODE_AUSTRALIA_2011);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_3600_2018_AMD), TDMT_CODE_AUSTRALIA_2018_AMD_2021);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009),          TDMT_CODE_AUSTRALIA_2009);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2011)")                         ,  25);       // wg_mkor�� ���ڿ� ������ ��!!
        CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2020)")                         ,  35);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__Gilbert_and_Ranzi_STANDARD) ,  34);
#ifdef _CIVIL
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__JAPAN_HYDRATION_STANDARD),  6);  //5
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__JAPAN_ELASTIC_STANDARD)  ,  7);  //6
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KDS-2016")                                     , 33);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__KCI_USD12_STANDARD)      , 11);  //10(����)
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__Korea_Standard)          ,  1);  //0
#endif // _CIVIL
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ABNTNBR61182023), TDMT_CODE_ABNT_NBR_6118_2023);  //40
		
	}
#elif defined(_JP)
	{
		//int nSeqTdme[] = {3, 1, 2, 0, 5, 6, 7, 8, 9,     22, 24, 25, 26}; 
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__OHZAGI_STANDARD)         ,  4);  //3(����)
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ACI_STANDARD)            ,  2);  //1
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP2010_STANDARD) ,     27);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP_STANDARD)         ,  3);  //2
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__Korea_Standard)          ,  1);  //0
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__JAPAN_HYDRATION_STANDARD),  6);  //5
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__JAPAN_ELASTIC_STANDARD)  ,  7);  //6
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__INDIA_STANDARD)          ,  8);  //7
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__EUROPEAN)                ,  9);  //8
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP_1978_STANDARD)    , 10);  //9
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2017),          TDMT_CODE_AUSTRALIA_2017);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2016),          TDMT_CODE_AUSTRALIA_2016);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011),          TDMT_CODE_AUSTRALIA_2011);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_3600_2018_AMD), TDMT_CODE_AUSTRALIA_2018_AMD_2021);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009),          TDMT_CODE_AUSTRALIA_2009);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2011)")                          , 25);       // wg_mkor�� ���ڿ� ������ ��!!
        CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2020)")                          , 35);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__RUSSIAN), TDMT_CODE_RUSSIAN);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__Gilbert_and_Ranzi_STANDARD)  , 34);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ABNTNBR61182023), TDMT_CODE_ABNT_NBR_6118_2023);  //40
	}
#elif defined(_CH)
	{
		// int nSeqTdme[] = {1, 2, 3, 0, 5, 6, 7, 8, 9,     22, 24, 25, 26};
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ACI_STANDARD)            ,  2);  //1(����)
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP2010_STANDARD) ,     27);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP_STANDARD)         ,  3);  //2
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__OHZAGI_STANDARD)         ,  4);  //3
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__Korea_Standard)          ,  1);  //0
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__JAPAN_HYDRATION_STANDARD),  6);  //5
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__JAPAN_ELASTIC_STANDARD)  ,  7);  //6
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__INDIA_STANDARD)          ,  8);  //7
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__EUROPEAN)                ,  9);  //8
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP_1978_STANDARD)    , 10);  //9
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2017),          TDMT_CODE_AUSTRALIA_2017);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2016),          TDMT_CODE_AUSTRALIA_2016);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011),          TDMT_CODE_AUSTRALIA_2011);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_3600_2018_AMD), TDMT_CODE_AUSTRALIA_2018_AMD_2021);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009),          TDMT_CODE_AUSTRALIA_2009);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2011)")                          , 25);       // wg_mkor�� ���ڿ� ������ ��!!
        CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2020)")                          , 35);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__RUSSIAN), TDMT_CODE_RUSSIAN);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ABNTNBR61182023), TDMT_CODE_ABNT_NBR_6118_2023);  //40
	}
#else
	{
	 // int nSeqTdme[] = {10, 0, 1, 2, 3, 5, 6, 7, 8, 9, 22, 24, 25, 26};
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KDS-2016")                                     , 33);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__KCI_USD12_STANDARD)      , 11);  //10(����)
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__Korea_Standard)          ,  1);  //0
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ACI_STANDARD)            ,  2);  //1
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP2010_STANDARD) ,     27);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP_STANDARD)         ,  3);  //2
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__OHZAGI_STANDARD)         ,  4);  //3
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__JAPAN_HYDRATION_STANDARD),  6);  //5
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__JAPAN_ELASTIC_STANDARD)  ,  7);  //6
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__INDIA_STANDARD)          ,  8);  //7
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__EUROPEAN)                ,  9);  //8
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__CEBFIP_1978_STANDARD)    , 10);  //9
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2017),          TDMT_CODE_AUSTRALIA_2017);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2016),          TDMT_CODE_AUSTRALIA_2016);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011),          TDMT_CODE_AUSTRALIA_2011);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_3600_2018_AMD), TDMT_CODE_AUSTRALIA_2018_AMD_2021);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009),          TDMT_CODE_AUSTRALIA_2009);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2011)")                          , 25);       // wg_mkor�� ���ڿ� ������ ��!!
        CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2020)")                          , 35);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__RUSSIAN), TDMT_CODE_RUSSIAN);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__Gilbert_and_Ranzi_STANDARD)  , 34);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ABNTNBR61182023), TDMT_CODE_ABNT_NBR_6118_2023);  //40
	}
#endif

	m_CobxCode.SetCurSel(0);
}

void CTdmeItemDlg::OnBtnDyGen()
{
	UpdateData(TRUE);
	T_TDME_K Key;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	Key = pDoc->m_pAttrCtrl->GetTdmeKey(m_ParamData.strMatlName);
	
	T_KEY_LIST aKey;
	pDoc->m_pAttrCtrl->GetUchtKeyList(aKey);
	
	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_TE_FUNC;  // Time Dependent Material Function.
	UchtD.nKey = Key;
	
	CString csName; csName.Empty();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if (nCount == 0) csName = _LS(IDS_CMD_DYNA_RPT_TDMCOMP_NAME); //_T("Time Dependent Material Comp Strength");
	else csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_TDMCOMP_NAME), nCount + 1);

	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if(dlg.DoModal() == IDOK)
	{
		UchtD.strName = csName;
	
		UchtD.nFuncType = m_type;
		UchtD.Data.Grph.nGraphType = m_nJpnGraphType;
		//CUnitCtrl::m_UCHT_UNIT.dDelay = CUnitCtrl::m_TDME_UNIT.dCompStrength;

		if(m_type == 0)
		{
			int nCode = CDlgUtil::CobxGetCurSelItemData(m_CobxCode, m_CobxCode.GetCurSel()); //2
			UchtD.Data.Grph.nKeyX  = nCode;
		 
			if(nCode == 1)  // korea standard
			{
				UchtD.dDelay = m_dKSDCompStrength.GetEditValue(); // S91
				UchtD.dMaxTemper = m_dKSDFactorA.GetEditValue();  // a
				UchtD.dMeanTemper = m_dKSDFactorB.GetEditValue(); // b
			}
			else if(nCode == 2) // ACI
			{
				UchtD.dDelay = m_dACICompStrength.GetEditValue(); // S28
				UchtD.dMaxTemper = m_dACIFactorA.GetEditValue();  // a
				UchtD.dMeanTemper = m_dACIFactorB.GetEditValue(); // b
			}
			else if (nCode == 3 || nCode == 11 || nCode == 27 || nCode == 28 || nCode == 33 || nCode == TDMT_CODE_ABNT_NBR_6118_2023) // CEB-FIP, KCI-USD12, KDS-2016, ABNT NBR 6118:2023
			{
				UchtD.dDelay = m_dCEBCompStrength.GetEditValue(); // S28
				UchtD.Data.Grph.nOctBandOption = m_CEBCementType.GetCurSel(); // nCement
			}
			else if(nCode == 4) // Ohzagi
			{
				UchtD.dDelay = m_dOHZCompStrength.GetEditValue(); // S28
		    UchtD.Data.Grph.nOctBandOption = m_OHZCementType.GetCurSel(); // nCement
			}
			else if(nCode == 5) // China
			{
				UchtD.dDelay = m_dCHCompStrength.GetEditValue();  // S28
			}
			else if(nCode == 6) //Japan
			{
				UchtD.dDelay = m_dJAPANCompStrength.GetEditValue(); // fck
				UchtD.dMaxTemper = m_dJAPANFactorA.GetEditValue();  // a
				UchtD.dMeanTemper = m_dJAPANFactorB.GetEditValue(); // b
				UchtD.dReactive = m_dJAPANFactorD.GetEditValue();   // d
				UchtD.dTemper = m_dJpnTensileStrength.GetEditValue(); // tensile
			}
			else if(nCode == 7) //Japan(Elastic Modulus)
			{
				int nSel;
				CDlgUtil::CtrlRadioGetCheck(this, m_aJPN2CodeRadioCtrl, nSel);
				UchtD.Data.Grph.nOctBandOption = nSel;
				UchtD.dDelay = m_dJPN2CompStrength.GetEditValue();  // fck
			}
			else if(nCode == 8) // INDIA standard
			{
				UchtD.dDelay = m_dINDIACompStrength.GetEditValue(); // S28
			}
			else if(nCode == 9) // European
			{
				UchtD.dDelay = m_dEUROPEANCompStrength.GetEditValue();  // S28
			  UchtD.Data.Grph.nOctBandOption = m_EUROPEANCementType.GetCurSel(); // nCement
			}
			else if(nCode == 10) // CEB-FIP(1978)
			{
				UchtD.dDelay = m_dCEBFIP1978CompStrength.GetEditValue();  // S28
			}
			else if(nCode == TDMT_CODE_AUSTRALIA_2009 || nCode == TDMT_CODE_AUSTRALIA_2011 || 
					nCode == TDMT_CODE_AUSTRALIA_2016 || nCode == TDMT_CODE_AUSTRALIA_2017 || // AS 3600-2009, AS/RTA 5100.5-2011, AS 5100.5-2016, AS 5100.5-2017 => INDIA standard �� ����.
				    nCode == TDMT_CODE_AUSTRALIA_2018_AMD_2021)
			{
				UchtD.dDelay = m_dINDIACompStrength.GetEditValue(); // S28
			}
			else if(nCode == 25) // (INDIA)IRC:112-2011
			{
				UchtD.dDelay = m_dIND2011CompStrength.GetEditValue();  // S28
				UchtD.Data.Grph.nOctBandOption = m_IND2011CementType.GetCurSel()+1; // nCement => 1���� �����̱� ������ Data ���� ������ 1�� ���Ѵ�...
			}
			else if(nCode == TDMT_CODE_RUSSIAN) // Russian
			{
				UchtD.dDelay = m_edtRusConcB.GetEditValue();  // S28
				UchtD.Data.Grph.nOctBandOption = m_cmbRusCementType.GetCurSel()+1; // nCement => 1���� �����̱� ������ Data ���� ������ 1�� ���Ѵ�...
			}
			else if (nCode == 34) // GILBERT AND RANZI
			{
				UchtD.dDelay = m_dGilbetCompStrength.GetEditValue(); // S28
				UchtD.Data.Grph.nOctBandOption = m_CEBCementType.GetCurSel() + 1; // nCement => 1���� �����̱� ������ Data ���� ������ 1�� ���Ѵ�...
			}
            else if (nCode == 35) // (INDIA)IRC:112-2011
            {
                UchtD.dDelay = m_dIND2011CompStrength.GetEditValue();  // S28
                UchtD.Data.Grph.nOctBandOption = m_IND2011CementType.GetCurSel() + 1; // nCement => 1���� �����̱� ������ Data ���� ������ 1�� ���Ѵ�...
            }
			else ASSERT(0);
		}

		UchtD.bLogX = m_btnXLog.GetCheck();
		UchtD.bLogY = m_btnYLog.GetCheck();
		if(!pDoc->m_pDataCtrl->AddUcht(UchtD)) { ASSERT(0); return; }
	}
	
}

void CTdmeItemDlg::Data2Dlg()
{
	CString str;
	m_csTypeName = m_ParamData.strMatlName;
	m_type = m_ParamData.nMatlType-1;
	str.Format(_T("%g"), m_ParamData.dScaleFactor);
	m_wndScaleFactor.SetWindowText(str);
	UpdateData(FALSE);

	if (m_ParamData.nMatlType == 2) return;  // USER TYPE

	int nID = CDBLib::GetHmtpCodeID(m_ParamData.strCode);

	CDlgUtil::CobxSetCurSelItemData(m_CobxCode, nID);     

	SetCementTypeSCmb(); // m_CobxCode ���� ���� �ݿ��ؾ� ��.

	switch(nID/*nSeqTdme[nNum]*/)  
	{
		case 1:
			m_dKSDCompStrength.SetEditUnit(m_ParamData.dCompStrength);
			m_dKSDFactorA.SetEditUnit(m_ParamData.dFactorA);
			m_dKSDFactorB.SetEditUnit(m_ParamData.dFactorB);
			break;
		case 2:
			m_dACICompStrength.SetEditUnit(m_ParamData.dCompStrength);
			m_dACIFactorA.SetEditUnit(m_ParamData.dFactorA);
			m_dACIFactorB.SetEditUnit(m_ParamData.dFactorB);
			break;
		case 3:
		case 11:
		case 27:
		case TDMT_CODE_ABNT_NBR_6118_2023:
			SetCementTypeSCmb();
			m_dCEBCompStrength.SetEditUnit(m_ParamData.dCompStrength);
			m_CEBCementType.SetCurSel(m_ParamData.nCementType-1);
			m_CEBAffreType.SetCurSel(m_ParamData.nAggreType);
			break;
		case 4:
			m_dOHZCompStrength.SetEditUnit(m_ParamData.dCompStrength);
			m_OHZCementType.SetCurSel(m_ParamData.nCementType-1);
			break;
		case 5:
			m_dCHCompStrengthUnit.SetUnitType(m_ParamData.dCompStrength);
			m_dCHCompStrength.SetEditUnit(m_ParamData.dCompStrength);
			break;
		case 6:
			m_dJAPANCompStrength.SetEditUnit(m_ParamData.dCompStrength);
			m_dJAPANFactorA.SetEditUnit(m_ParamData.dFactorA);
			m_dJAPANFactorB.SetEditUnit(m_ParamData.dFactorB);
			m_dJAPANFactorD.SetEditUnit(m_ParamData.dFactorD);
			m_btnConcreteData.SetCheck(m_ParamData.bUseConcData);
			m_JAPANCementType.SetCurSel(m_ParamData.nJpCementType);
			m_dJpnTensileStrength.SetEditUnit(m_ParamData.dTensileStrengthFactor);
			OnTdmeJPNConcreteChk();
			break;
		case 7:
			m_dJPN2CompStrength.SetEditUnit(m_ParamData.dCompStrength);
			CDlgUtil::CtrlRadioSetCheck(this, m_aJPN2CodeRadioCtrl, m_ParamData.nJpElaCementType);
			break;
		case 8:
		case TDMT_CODE_AUSTRALIA_2018_AMD_2021:
		case TDMT_CODE_AUSTRALIA_2017:
		case TDMT_CODE_AUSTRALIA_2016:
		case TDMT_CODE_AUSTRALIA_2011:
		case TDMT_CODE_AUSTRALIA_2009:
			m_dINDIACompStrength.SetEditUnit(m_ParamData.dCompStrength);
			break;
		case 9:
			m_dEUROPEANCompStrength.SetEditUnit(m_ParamData.dCompStrength);
			m_EUROPEANCementType.SetCurSel(m_ParamData.nCementType-1);
			break;
		case 10:
			m_dCEBFIP1978CompStrength.SetEditUnit(m_ParamData.dCompStrength);
			break;
		case 25:
        case 35:
			m_dIND2011CompStrength.SetEditUnit(m_ParamData.dCompStrength);
			m_IND2011CementType.SetCurSel(m_ParamData.nCementType-1); // �Է� �޾ƾ� ��
            m_dIND2011AffreType.SetCurSel(m_ParamData.nAggreType);
			break;
		case TDMT_CODE_RUSSIAN:
			m_edtRusConcB.SetEditUnit(m_ParamData.dCompStrength);	
			if(!m_bModify) m_cmbRusCementType.SetCurSel(0);
			else           m_cmbRusCementType.SetCurSel(m_ParamData.nCementType-1);
			m_nRusCuringMeth = m_ParamData.nCuringMeth;
			m_nRusConcType = m_ParamData.nConcType;
			m_edtRusMaxSize.SetEditUnit(m_ParamData.dMaxSize);
			m_edtRusPz.SetEditUnit(m_ParamData.dPz);
			break;
		case 33:
			SetCementTypeSCmb();
			m_dCEBCompStrength.SetEditUnit(m_ParamData.dCompStrength);
			m_CEBCementType.SetCurSel(m_ParamData.nCementType-1);
			m_CEBAffreType.SetCurSel(m_ParamData.nAggreType);
			m_edtDensity.SetEditUnit(m_ParamData.dDensity);
			break;
		case 34:
			SetCementTypeSCmb();
			m_dGilbetCompStrength.SetEditUnit(m_ParamData.dCompStrength);
			m_GilbertCementType.SetCurSel(m_ParamData.nCementType - 1);
			m_edtDensity.SetEditUnit(m_ParamData.dDensity);
			break;
		default:
			ASSERT(!_T("Not Exist Code - CTdmeItemDlg::Data2Dlg()"));
			break;

		UpdateData(FALSE);
	}
}

void CTdmeItemDlg::Dlg2Data()
{
	//m_CobxCode.GetWindowText(m_ParamData.strCode);
	int nNum = CDlgUtil::CobxGetCurSelItemData(m_CobxCode, m_CobxCode.GetCurSel()); //2
	m_ParamData.strCode = CDBLib::GetHmtpCodeName(nNum);

	switch(nNum)
	{
		case 1:
			m_ParamData.dCompStrength = m_dKSDCompStrength.GetEditValue();
			m_ParamData.dFactorA = m_dKSDFactorA.GetEditValue();
			m_ParamData.dFactorB = m_dKSDFactorB.GetEditValue();
		  break;
		case 2:
			m_ParamData.dCompStrength = m_dACICompStrength.GetEditValue();
			m_ParamData.dFactorA = m_dACIFactorA.GetEditValue();
			m_ParamData.dFactorB = m_dACIFactorB.GetEditValue();
			break;
		case 3:
		case 11:
		case 27:
		case TDMT_CODE_ABNT_NBR_6118_2023:
			m_ParamData.dCompStrength = m_dCEBCompStrength.GetEditValue();
			m_ParamData.nCementType = m_CEBCementType.GetCurSel() + 1;
			m_ParamData.nAggreType = m_CEBAffreType.GetCurSel();
			break;
		case 4:
			m_ParamData.dCompStrength = m_dOHZCompStrength.GetEditValue();
			m_ParamData.nCementType = m_OHZCementType.GetCurSel() + 1;
		  break;
		case 5:
			m_ParamData.dCompStrength = m_dCHCompStrength.GetEditValue();
			break;
		case 6:
			m_ParamData.dCompStrength = m_dJAPANCompStrength.GetEditValue();
			m_ParamData.dFactorA = m_dJAPANFactorA.GetEditValue();
			m_ParamData.dFactorB = m_dJAPANFactorB.GetEditValue();
			m_ParamData.dFactorD = m_dJAPANFactorD.GetEditValue();
			
			if (m_btnConcreteData.GetCheck())
			{
				m_ParamData.bUseConcData = TRUE;
				m_ParamData.nJpCementType = m_JAPANCementType.GetCurSel();
			}
			else
			{
				m_ParamData.bUseConcData = FALSE;
			}
			m_ParamData.dTensileStrengthFactor = m_dJpnTensileStrength.GetEditValue();
			break;
		case 7:
			m_ParamData.dCompStrength = m_dJPN2CompStrength.GetEditValue();
			CDlgUtil::CtrlRadioGetCheck(this, m_aJPN2CodeRadioCtrl, m_ParamData.nJpElaCementType);
			break;
		case 8:
			m_ParamData.dCompStrength = m_dINDIACompStrength.GetEditValue();
			break;
		case 9:
			m_ParamData.dCompStrength = m_dEUROPEANCompStrength.GetEditValue();
			m_ParamData.nCementType = m_EUROPEANCementType.GetCurSel() + 1;
			break;
		case 10:
			m_ParamData.dCompStrength = m_dCEBFIP1978CompStrength.GetEditValue();
			break;
		case TDMT_CODE_AUSTRALIA_2018_AMD_2021:
		case TDMT_CODE_AUSTRALIA_2017:
		case TDMT_CODE_AUSTRALIA_2016:
		case TDMT_CODE_AUSTRALIA_2011:
		case TDMT_CODE_AUSTRALIA_2009:
			m_ParamData.dCompStrength = m_dINDIACompStrength.GetEditValue();
			break;
		case 25:
        case 35:
			m_ParamData.dCompStrength = m_dIND2011CompStrength.GetEditValue();
			m_ParamData.nCementType = m_IND2011CementType.GetCurSel() + 1;  // �Է� �޾ƾ� ��.
            m_ParamData.nAggreType = m_dIND2011AffreType.GetCurSel();
			break;
		case TDMT_CODE_RUSSIAN:
			m_ParamData.dCompStrength = m_edtRusConcB.GetEditValue();
			m_ParamData.nCementType = m_cmbRusCementType.GetCurSel() + 1;
			m_ParamData.nCuringMeth = m_nRusCuringMeth;
			m_ParamData.nConcType = m_nRusConcType;
			m_ParamData.dMaxSize = m_edtRusMaxSize.GetEditValue();
			m_ParamData.dPz = m_edtRusPz.GetEditValue();
			break;
		case 33:
			m_ParamData.dCompStrength = m_dCEBCompStrength.GetEditValue();
			m_ParamData.nCementType = m_CEBCementType.GetCurSel() + 1;
			m_ParamData.dDensity = m_edtDensity.GetEditValue();
			break;
		case 34:
			m_ParamData.dCompStrength = m_dGilbetCompStrength.GetEditValue();
			m_ParamData.nCementType = m_GilbertCementType.GetCurSel() + 1;
			m_ParamData.dDensity = m_edtDensity.GetEditValue();
			break;
		default:
			ASSERT(!_T("Not Exist Code - CTdmeItemDlg::Dlg2Data()"));
			break;
	}
}

void CTdmeItemDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove;
	int nDistY, nDistX;

	// 1st move 
	GetDlgItem(IDC_FRAME)->GetWindowRect(rRef);
	GetDlgItem(IDC_TDME_FUNC_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCodeCtrl, nDistY);

	// 2nd move
	GetDlgItem(IDC_TDME_FUNC_FRAME)->GetWindowRect(rRef);
	GetDlgItem(IDC_TDME_FUNC_KS_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aKSDCodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_ACI_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aACICodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_CEB_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCEBCodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_OHZ_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aOHZCodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_CH_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCHCodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_JAPAN_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aJAPANCodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_JPN2_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aJPN2CodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_INDIA_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aINDIACodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_EUROPEAN_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aEUROPEANCodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_CEBFIP_1978_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCEBFIP1978CodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_IND2011_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aIND2011CodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_RUS_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aRussiaCodeCtrl, nDistY);
	GetDlgItem(IDC_TDME_FUNC_GILBERT_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aGilbertCtrl, nDistY);


	GetDlgItem(IDC_TDME_TABLE)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistXY(this, m_aUserCtrl, nDistX, nDistY);

// 	GetDlgItem(IDC_TDME_CEB_STATIC5)->GetWindowRect(rRef);
// 	GetDlgItem(IDC_CMD_TDME_KDS_2016_MATL_STC)->GetWindowRect(rToMove);
// 	nDistX = rRef.left - rToMove.left;
// 	GetDlgItem(IDC_CMD_TDME_KDS_2016_MATL_CMB)->GetWindowRect(rToMove);
// 	nDistY = rRef.top - rToMove.top;	
	GetDlgItem(IDC_TDME_CEB_STATIC5)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_TDME_KDS_2016_MATL_STC)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.bottom - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aKDS2016Ctrl, nDistX, nDistY);

	// Graph Type Frame
	GetDlgItem(IDC_TDME_FUNC_JAPAN_FRAME)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_TDME_JAPAN_TYPE)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.bottom;
	CDlgUtil::CtrlMoveDistY(this, m_arGraphtype, nDistY);
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(16);

	MoveWindow(r);
	GetDlgItem(IDC_FRAME)->ShowWindow(FALSE);
}

void CTdmeItemDlg::AlignControlsDensity(int nCode)
{
	BOOL bCEB, bGilbert;
	bCEB = bGilbert = FALSE;
	switch (nCode)
	{
	case 3:
	case 11:
	case 27:
	case 33:
	case TDMT_CODE_ABNT_NBR_6118_2023:
		bCEB = TRUE;
		break;
	case 34:
		bGilbert =TRUE;
		break;
	default:
		break;
	}

	if (!bCEB && !bGilbert) return;

	CRect rRef;
	CRect rToMove;
	int nDistY, nDistX;

	if (bCEB)
	{
// 		GetDlgItem(IDC_TDME_CEB_STATIC5)->GetWindowRect(rRef);
// 		GetDlgItem(IDC_CMD_TDME_KDS_2016_MATL_STC)->GetWindowRect(rToMove);
// 		nDistX = rRef.left - rToMove.left;
// 		GetDlgItem(IDC_CMD_TDME_KDS_2016_MATL_CMB)->GetWindowRect(rToMove);
// 		nDistY = rRef.top - rToMove.top;
		GetDlgItem(IDC_TDME_CEB_STATIC5)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_TDME_KDS_2016_DENSITY_STC)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top;

		CDlgUtil::CtrlMoveDistXY(this, m_aKDS2016Ctrl, nDistX, nDistY);
	}
	else if (bGilbert)
	{
// 		GetDlgItem(IDC_CMD_TDME_GILBERT_TEMP_STC)->GetWindowRect(rRef);
// 		GetDlgItem(IDC_CMD_TDME_KDS_2016_MATL_STC)->GetWindowRect(rToMove);
// 		nDistX = rRef.left - rToMove.left;
// 		GetDlgItem(IDC_CMD_TDME_KDS_2016_MATL_CMB)->GetWindowRect(rToMove);
// 		nDistY = rRef.top - rToMove.top;
		GetDlgItem(IDC_CMD_TDME_GILBERT_TEMP_STC)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_TDME_KDS_2016_DENSITY_STC)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top;

		CDlgUtil::CtrlMoveDistXY(this, m_aKDS2016Ctrl, nDistX, nDistY);
	}
	else
		ASSERT(0);
}

void CTdmeItemDlg::OnTdmeSelChangeType() 
{
	// TODO: Add your control notification handler code here
	int nPrevType = m_type;
	UpdateData();

	if (nPrevType == 1 && m_type != 1)  // user
	{
		if (!m_wndGrid.OnTerminate(D_TB_SAVE_ALWAYS))
			m_wndGrid.CancelEdit();
	}

	if(m_type == 0)
	{
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCodeCtrl, TRUE);
		CDlgUtil::CtrlShowHide(this, m_arGraphtype, FALSE);
		ShowHideCode(TRUE);
		m_wndScaleFactor.SetWindowText(_T("1.0"));
		m_wndScaleFactor.EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else if(m_type == 1)
	{
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCodeCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_arGraphtype, TRUE);
		ShowHideCode(FALSE);
		m_wndScaleFactor.EnableWindow(TRUE);
	}

	DrawChart();
	m_ParamData.nMatlType = m_type+1;
}

void CTdmeItemDlg::OnSelchangeTdmeCodeCombo() 
{
	// TODO: Add your control notification handler code here
	ShowHideCode(TRUE);
	SetCementTypeSCmb();
	DrawChart();
	OnPaint();
}

void CTdmeItemDlg::ShowHideCode(BOOL flag)
{
	int nNum;

	if(flag)
		nNum = CDlgUtil::CobxGetCurSelItemData(m_CobxCode, m_CobxCode.GetCurSel()); //2
	else
		nNum = 4;

	AlignControlsDensity(nNum);

	BOOL bKSD, bACI, bCEB, bOHZ, bCH, bJPN, bJPN2, bINDIA, bEUROPEAN, bCEB1978, bAustralia, bInd2011, bRUS, bGilbert;
	bKSD = bACI = bCEB = bOHZ = bCH = bJPN = bJPN2 = bINDIA = bEUROPEAN = bCEB1978 = bAustralia = bInd2011 = bRUS = bGilbert = FALSE;

	switch(nNum)
	{
		case 1:
			bKSD = flag;	bACI = bCEB = bOHZ = bCH = bJPN = bJPN2 = bINDIA = bEUROPEAN = bCEB1978 = bAustralia = bInd2011 = bRUS = bGilbert = FALSE;
			break;
		case 2:
			bACI = flag;	bKSD = bCEB = bOHZ = bCH = bJPN = bJPN2 = bINDIA = bEUROPEAN = bCEB1978 = bAustralia = bInd2011 = bRUS = bGilbert = FALSE;
			break;
		case 3:
		case 11:
		case 27:
		case 33:
		case TDMT_CODE_ABNT_NBR_6118_2023:
			bCEB = flag;	bKSD = bACI = bOHZ = bCH = bJPN = bJPN2 = bINDIA = bEUROPEAN = bCEB1978 = bAustralia = bInd2011 = bRUS = bGilbert = FALSE;
			break;
		case 4:
			bOHZ = flag;	bKSD = bACI = bCEB = bCH = bJPN = bJPN2 = bINDIA = bEUROPEAN = bCEB1978 = bAustralia = bInd2011 = bRUS = bGilbert = FALSE;
			break;
		case 5:
			bCH = flag;  bKSD = bACI = bCEB = bOHZ = bJPN = bJPN2 = bINDIA = bEUROPEAN = bCEB1978 = bAustralia = bInd2011 = bRUS = bGilbert = FALSE;
			break;
		case 6:
			bJPN = flag;  bKSD = bACI = bCEB = bOHZ = bCH = bJPN2 = bINDIA = bEUROPEAN = bCEB1978 = bAustralia = bInd2011 = bRUS = bGilbert = FALSE;
			break;
		case 7:
			bJPN2 = flag;  bKSD = bACI = bCEB = bOHZ = bCH = bJPN = bINDIA = bEUROPEAN = bCEB1978 = bAustralia = bInd2011 = bRUS = bGilbert = FALSE;
			break;
		case 8:
			bINDIA = flag; bKSD = bACI = bCEB = bOHZ = bCH = bJPN = bJPN2 = bEUROPEAN = bCEB1978 = bAustralia = bInd2011 = bRUS = bGilbert = FALSE;
			break;
		case 9:
			bEUROPEAN = flag;  bKSD = bACI = bCEB = bOHZ = bCH = bJPN = bJPN2 = bINDIA = bCEB1978 = bAustralia = bInd2011 = bRUS = bGilbert = FALSE;
			break;
		case 10:
			bCEB1978 = flag; bKSD = bACI = bCEB = bOHZ = bCH = bJPN = bJPN2 = bINDIA = bEUROPEAN = bAustralia = bInd2011 = bRUS = bGilbert = FALSE;
			break;
		case TDMT_CODE_AUSTRALIA_2018_AMD_2021:
		case TDMT_CODE_AUSTRALIA_2017:
		case TDMT_CODE_AUSTRALIA_2016:
		case TDMT_CODE_AUSTRALIA_2011:
		case TDMT_CODE_AUSTRALIA_2009:
			bAustralia = flag;  bKSD = bACI = bCEB = bOHZ = bCH = bJPN = bJPN2 = bINDIA = bEUROPEAN = bCEB1978 = bInd2011 = bRUS = bGilbert = FALSE;
			break;
		case 25:
        case 35:
			bInd2011 = flag;  bKSD = bACI = bCEB = bOHZ = bCH = bJPN = bJPN2 = bINDIA = bEUROPEAN = bCEB1978 = bAustralia = bRUS = bGilbert = FALSE;
			break;
		case TDMT_CODE_RUSSIAN:
			bRUS = flag;  bKSD = bACI = bCEB = bOHZ = bCH = bJPN = bJPN2 = bINDIA = bEUROPEAN = bCEB1978 = bAustralia = bInd2011 = bGilbert = FALSE;
			break;
		case 34:
			bGilbert = flag;  bKSD = bACI = bCEB = bOHZ = bCH = bJPN = bJPN2 = bINDIA = bEUROPEAN = bCEB1978 = bAustralia = bInd2011 = bRUS = FALSE;
			break;
		default:
			ASSERT(!_T("Not Exist Code - CTdmeItemDlg::ShowHideCode()"));
			break;
	}

	//Dialog Size ����
	UpdateData();
	CRect ref, target;
	GetDlgItem(IDC_TDME_SIN_REDRAW_GRAPH)->GetWindowRect(ref);
	if (bJPN || m_type==1)
		GetDlgItem(IDC_TDME_FUNC_JAPAN_FRAME)->GetWindowRect(target);
	else if(bRUS)
		GetDlgItem(IDC_TDME_FUNC_RUS_FRAME)->GetWindowRect(target);
	else
		GetDlgItem(IDC_TDME_FUNC_FRAME)->GetWindowRect(target);
	int dY = target.bottom - ref.top + globalUtils.ScaleByDPI(5);
	CDlgUtil::CtrlMoveDistY(this, m_aButtonCtrl, dY); //Move Button Ctrl

	CDlgUtil::CtrlShowHide(this, m_aKSDCodeCtrl, bKSD);
	CDlgUtil::CtrlShowHide(this, m_aACICodeCtrl, bACI);
	CDlgUtil::CtrlShowHide(this, m_aCEBCodeCtrl, bCEB);
	CDlgUtil::CtrlShowHide(this, m_aOHZCodeCtrl, bOHZ);
	CDlgUtil::CtrlShowHide(this, m_aCHCodeCtrl, bCH);
	CDlgUtil::CtrlShowHide(this, m_aJAPANCodeCtrl, bJPN);
	if (flag)
		CDlgUtil::CtrlShowHide(this, m_arGraphtype, bJPN);
	CDlgUtil::CtrlShowHide(this, m_aJPN2CodeCtrl, bJPN2);
	BOOL bShow = FALSE;
	if (nNum==8) bShow = bINDIA;
	else if(nNum== TDMT_CODE_AUSTRALIA_2009 || nNum== TDMT_CODE_AUSTRALIA_2011 || 
		    nNum== TDMT_CODE_AUSTRALIA_2016 || nNum== TDMT_CODE_AUSTRALIA_2017 || nNum== TDMT_CODE_AUSTRALIA_2018_AMD_2021) bShow = bAustralia;
	CDlgUtil::CtrlShowHide(this, m_aINDIACodeCtrl, bShow);
	CDlgUtil::CtrlShowHide(this, m_aEUROPEANCodeCtrl, bEUROPEAN);
	CDlgUtil::CtrlShowHide(this, m_aCEBFIP1978CodeCtrl, bCEB1978);
	CDlgUtil::CtrlShowHide(this, m_aIND2011CodeCtrl, bInd2011);
	CDlgUtil::CtrlShowHide(this, m_aRussiaCodeCtrl, bRUS);
	CDlgUtil::CtrlShowHide(this, m_aGilbertCtrl, bGilbert);
	CDlgUtil::CtrlShowHide(this, m_aKDS2016Ctrl, bCEB || bGilbert);

	if(nNum==3 || nNum==11 || nNum==33)
	{
		GetDlgItem(IDC_TDME_CEB_STATIC5)->ShowWindow(FALSE);
		m_CEBAffreType.ShowWindow(FALSE);
	}

	if(nNum==3 || nNum==11 || nNum==27 || nNum==TDMT_CODE_ABNT_NBR_6118_2023)
	{
		CDlgUtil::CtrlShowHide(this, m_aKDS2016Ctrl, FALSE);
	}

    if ( nNum == 35)
    {
        GetDlgItem(IDC_TDME_CEB_STATIC6)->ShowWindow(TRUE);
        m_dIND2011AffreType.ShowWindow(TRUE);
    }
	else
	{
		GetDlgItem(IDC_TDME_CEB_STATIC6)->ShowWindow(FALSE);
		m_dIND2011AffreType.ShowWindow(FALSE);
	}

	if(nNum == TDMT_CODE_ABNT_NBR_6118_2023)
	{
		GetDlgItem(IDC_TDME_CEB_STATIC2)->SetWindowText(_LS(IDS_CMD_TDME_ABNT_FCK));
	}
	else
	{
		GetDlgItem(IDC_TDME_CEB_STATIC2)->SetWindowText(_LS(IDS_CMD_TDME_CEB_FCM));
	}
//   if(nNum==25) // �����ϱ�..kyungsik
//   {
//     GetDlgItem(IDC_TDME_IND2011_STATIC)->SetWindowText(_T("Cube Mean Compressive Strength \(fcm=fck+10MPa)"));
//   }

	if(bGilbert)
	{
		GetDlgItem(IDC_CMD_TDME_GILBERT_TEMP_STC)->ShowWindow(FALSE);
	}
	GetDlgItem(IDC_TDME_FUNC_FRAME)->ShowWindow(FALSE);

	//Resize Dialog
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

int CTdmeItemDlg::SelChangeCodeCobx(CString str)
{
	int nNum = m_CobxCode.SelectString(-1, str);
	m_CobxCode.SetCurSel(nNum);

	return nNum;
}

T_TDME_D& CTdmeItemDlg::GetParamData()
{
	return m_ParamData;
}

void CTdmeItemDlg::SetParamData(T_TDME_D& data)
{
	m_bModify		= TRUE;
	m_ParamData = data;
	m_csOldName = m_ParamData.strMatlName;
}

void CTdmeItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData()) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess;

	if (m_bModify)
		bSuccess = pDoc->m_pDataCtrl->ModifyTdme(m_csOldName, m_ParamData);
	else
		bSuccess = pDoc->m_pDataCtrl->AddTdme(m_ParamData);

	if (!bSuccess) return;
	CDialogMove::OnOK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Grid
void CTdmeItemDlg::InitGrid()
{
	m_wndGrid.Initialize(&m_ParamData);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_bGridInit = TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
// Chart
void CTdmeItemDlg::InitChart()
{
	////////////////////////////////////////////////////////////////
	// Initialize m_Graph
	
	m_GraphView.SetDragable(FALSE); // set data-drag unable
	// Add Background component
	//
	SRGraphBackground	*pB=new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent((SRGraphComponent *) pB);
	
	// Add Display Component
	//

	SRGraphDisplay *pDisplay=new SRGraphDisplay;	
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX);
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	pDisplay->SetSizeDefaults();
	pDisplay->SetRect(0.0,0,100,97.0);
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(TRUE);
	
	
	// Logarithmic Axis Scaling
	//
//	pDisplay->GetStyle()->SetLogX(m_LogFlagX);
//	pDisplay->GetStyle()->SetLog(m_LogFlagY);

	//pDisplay->GetStyle()->SetShowXGrid(m_GridXFlag);
	//pDisplay->GetStyle()->SetShowYGrid(m_GridYFlag);
	

	m_Graph.AddComponent(pDisplay);

	// Add Label Component for X-Axis
	//
	SRGraphLabel	*pLabel=new SRGraphLabel;
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
	SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Time__day_));
	
	// Add Label Component for Y-Axis
	//
	pLabel=new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::MidCenter);
	pLabel->SetTextPosition(1.5, 50.0);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Upward);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	//pLabel->GetStyle()->SetColor(CXCLR_BACKGRND);
	m_Graph.AddComponent(pLabel);
//	SetAxisLabelText(1,_T("Tdme"));
	

	
	

	/////////////////////////////////////////////////////////////
	// Attatch graphview to dialog's placeholder and connect graph to graphview
	
	CWnd* wnd = GetDlgItem(IDC_CMD_PLACEHOLDER);
	CRect rc;

	wnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	rc.left+=2;
	rc.top+=2;
	rc.right -= 2;
	rc.bottom -= 2;

	m_GraphView.Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rc, this, IDC_CMD_PLACEHOLDER);
	m_GraphView.m_pGraph = &m_Graph;
	
	m_GraphView.Invalidate();
}

//void CTdmeItemDlg::InitMatlCombo()
//{
// 	CDBDoc* pDoc = CDBDoc::GetDocPoint();
// 
// 	m_cmbMatl.ResetContent();
// 	T_MATL_D MatlD; MatlD.Initialize();
// 	CArray<T_MATL_K, T_MATL_K> aMatlK;
// 	pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlK);
// 
// 	for(int i=0; i<aMatlK.GetSize(); i++)
// 	{
// 		if(!pDoc->m_pAttrCtrl->GetMatl(aMatlK[i], MatlD)) continue;
// 		if(MatlD.Type == _T("C") || MatlD.Type == _T("SRC"))
// 		{
// 			m_cmbMatl.SetItemData(m_cmbMatl.AddString(MatlD.Name), aMatlK[i]);
// 		}		
// 	}
// 
// 	m_cmbMatl.SetCurSel(0);
// 
// 	CDlgUtil::CobxAdjustListBoxWidth(m_cmbMatl);
//}

void CTdmeItemDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CTdmeItemDlg::OnDataChanged()
{
	// Add handler to update graph
	T_TDME_BASE data;
	int nSize = m_ParamData.aFuncData.GetSize();
	UpdateData();

	m_Graph.KillData();	
	for(int i=0; i<nSize; i++)
	{
		data = m_ParamData.aFuncData[i];
		if (m_nJpnGraphType == 0)
			SetValue(i, 0, data.dTime, data.dComp);
		else if (m_nJpnGraphType == 1)
			SetValue(i, 0, data.dTime, data.dTensile);
		else
			SetValue(i, 0, data.dTime, data.dElast);
	}
	m_GraphView.Invalidate();
}

void CTdmeItemDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
}

void CTdmeItemDlg::DrawChart()
{
	UpdateData();
/*
	if(m_type == 0)
	{
		GraphConstant();
	}
*/
	if(m_type == 0)
		GraphCode();
	else if(m_type == 1)
		GraphUser();
	
}

void CTdmeItemDlg::GraphConstant()
{
//	T_TDME_BASE data;

	m_Graph.KillData();
/*
	for(int i=0; i < 30; i++)
	{
		data.dTime = i;
		data.dValue = 0;
		SetValue(i, 0, data.dTime, data.dValue);
	}
*/
	m_GraphView.Invalidate();
}

void CTdmeItemDlg::GraphCode()
{
	UpdateData();
	T_TDME_BASE data;
	CArray<T_TDME_BASE, T_TDME_BASE> acode;

//2	int ncode = m_CobxCode.GetCurSel();
	int ncode = CDlgUtil::CobxGetCurSelItemData(m_CobxCode, m_CobxCode.GetCurSel()); //2

	if(ncode == 1) 	// korea standard
	{
		double S91 = m_dKSDCompStrength.GetEditValue();
		double a = m_dKSDFactorA.GetEditValue();
		double b = m_dKSDFactorB.GetEditValue();
		for(int i=0; i <= 91; i++)
		{
			if(a == 0 && (b == 0 || i == 0)) break;
			data.dTime = i;
			data.dElast = S91 * (i / (a + b*i));
			acode.Add(data);
		}
	}
	else if(ncode == 2)  // ACI
	{
		double S28 = m_dACICompStrength.GetEditValue();
		double a = m_dACIFactorA.GetEditValue();
		double b = m_dACIFactorB.GetEditValue();
		for(int i=0; i <= 30; i++)
		{
			if(a == 0 && (b == 0 || i == 0)) break;
			data.dTime = i;
			data.dElast = S28 * (i / (a + b*i));
			acode.Add(data);
		}
	}
	else if(ncode == 3 || ncode == 27)  // CEB-FIP
	{
		for(int i=0; i <= 30; i++)
		{
			double time;
			double a[3] = { 0.2, 0.25, 0.38 };
			double S28 = m_dCEBCompStrength.GetEditValue();
			int nCement = m_CEBCementType.GetCurSel();
			if(nCement < 0 || nCement > 2) break;
			if(i == 0) time = 0.1; // 0���� ������ ��������...
			else time = i;
			data.dTime = time;
			data.dElast = S28 * exp(a[nCement] * (1 - pow((28/time), 0.5)));
			acode.Add(data);
		}
	}
	else if (ncode == TDMT_CODE_ABNT_NBR_6118_2023)  // ABNT NBR 6118:2023
	{
		for (int i = 0; i <= 30; i++)
		{
			double time;
			double a[3] = { 0.38, 0.25, 0.2 };
			double S28 = m_dCEBCompStrength.GetEditValue();
			int nCement = m_CEBCementType.GetCurSel();
			if (nCement < 0 || nCement > 2) break;
			if (i == 0) time = 0.1;
			else time = i;
			data.dTime = time;
			data.dElast = S28 * exp(a[nCement] * (1 - pow((28 / time), 0.5)));
			acode.Add(data);
		}
	}
	else if (ncode == 34)  // GILBERT AND RANZI
	{
		for (int i = 0; i <= 30; i++)
		{
			double time;
			double s[2] = {0.38, 0.25};
			double FCMI = m_dGilbetCompStrength.GetEditValue();
			int nCement = m_GilbertCementType.GetCurSel();
			if (nCement < 0 || nCement > 1) break;

			if (i == 0) time = 0.1; // 0���� ������ ��������...
			else time = i;
			data.dTime = time;
			data.dElast = FCMI * exp(s[nCement] * (1 - pow((28 / time), 0.5)));
			acode.Add(data);
		}
	}
	else if(ncode == 4)  // Ohzagi
	{
		double S28 = m_dOHZCompStrength.GetEditValue();
		int nCement = m_OHZCementType.GetCurSel();
		for(int i=0; i <= 1000; i++)
		{
			if(nCement < 0 || nCement > 3) break;
			data.dTime = i;
			data.dElast = S28 * OhzagiY(nCement, i);
			acode.Add(data);
		}
	}
	else if(ncode == 5)  // China
	{
		double S28 = m_dCHCompStrength.GetEditValue();
		for(int i=3; i<=30; i++)
		{
			data.dTime = i;
			data.dElast = S28 * log10(i) / log10(28);
			acode.Add(data);
		}
	}
	else if (ncode == 6)    //Japan
	{
		double fck = m_dJAPANCompStrength.GetEditValue();
		double a = m_dJAPANFactorA.GetEditValue();
		double b = m_dJAPANFactorB.GetEditValue();
		double d = m_dJAPANFactorD.GetEditValue();
		double tensile = m_dJpnTensileStrength.GetEditValue();
		double fck_t;

		for(int i=0; i <= 30; i++)
		{
			if(a == 0 && (b == 0 || i == 0)) break;
			data.dTime = i;
			fck_t = fck * d * (i / (a + b*i));

			if (m_nJpnGraphType == 0)
			  data.dElast = fck_t;
			else if (m_nJpnGraphType == 1)
			{
				double dFactor = GetFactorForCode(m_CurrUnit.nBase_Force, m_CurrUnit.nBase_Length);
				data.dElast = tensile * sqrt(fck_t * dFactor);
				data.dElast /= dFactor;
			}
			else if (m_nJpnGraphType == 2)
			{
				double dFactor = GetFactorForCode(m_CurrUnit.nBase_Force, m_CurrUnit.nBase_Length);
				data.dElast = 4700. * sqrt(fck_t * dFactor);
				data.dElast /= dFactor;
			}
			acode.Add(data);
		}
	}
	else if (ncode == 7)    //Japan(Elastic Modulus)
	{
		double dTimeRapid[] = {1.52, 2., 4., 5., 10., 20., 28., 50., 80.35};
		//double dKeRapid[] = {0.7, 0.752, 0.836, 0.873, 0.939, 0.985, 1.0, 1.024, 1.039};
		double dKeRapid[] = {0.7, 0.752, 0.836, 0.873, 0.939, 0.985, 1.0, 1.0, 1.0};  // 28�� ���ĺ��ʹ� 1.0���� ����

		double dTimeNormal[] = {2., 4., 5., 7.938, 10., 20., 28., 50., 75.858};
		//double dKeNormal[] = {0.632, 0.764, 0.821, 0.884, 0.909, 0.976, 1., 1.036, 1.055};
		double dKeNormal[] = {0.632, 0.764, 0.821, 0.884, 0.909, 0.976, 1.0, 1.0, 1.0};  // 28�� ���ĺ��ʹ� 1.0���� ����
		int nLoop = sizeof(dTimeRapid)/sizeof(double);

		int nSel;
		CDlgUtil::CtrlRadioGetCheck(this, m_aJPN2CodeRadioCtrl, nSel);
		double fck = m_dJPN2CompStrength.GetEditValue();
		
		double* dTime;
		double* dVal;
		if (nSel == 0)
		{
			dTime = dTimeNormal;
			dVal = dKeNormal;
		}
		else
		{
			dTime = dTimeRapid;
			dVal = dKeRapid;
		}

		for (int i=0; i<nLoop; i++)
		{
			if (fck == 0) break;
			data.dTime = dTime[i];
			data.dElast = fck * dVal[i];
			acode.Add(data);
		}
	}
	else if(ncode == 8) 	// INDIA standard
	{
		double S28 = m_dINDIACompStrength.GetEditValue();
		for(int i=3; i <= 30; i++)
		{ 
			data.dTime = i;
			data.dElast = S28*(2.008-(1.2134/(pow(log10(i),0.5))));
			acode.Add(data);
		}
	}
	else if(ncode == 25 || ncode==35) 	// (INDIA) IRC : 112-2011
	{
		double dtime=0.0;
		double a[3] = { 0.38 ,0.25, 0.2};  // slow hardening, normal Portland, rapid harding
		double S28 = m_dIND2011CompStrength.GetEditValue();
		double dfcm = S28; // �Է¹޴� ���� Fcm���� �����.
		int nCement = m_IND2011CementType.GetCurSel(); 

		for(int i=0; i <= 30; i++)
		{ 
			dtime = i;
			if(i == 0) dtime = 0.1; // 0���� ������ ��������...
			data.dTime = dtime;
			data.dElast = dfcm * exp(a[nCement]*(1-pow((28/dtime),0.5)));
			acode.Add(data);
		}
	}
	else if(ncode == 9) // European
	{
		for(int i=0; i <= 30; i++)
		{
			double time;
			double a[3] = { 0.2, 0.25, 0.38 };
			double S28 = m_dEUROPEANCompStrength.GetEditValue();
			int nCement = m_EUROPEANCementType.GetCurSel();
			if(nCement < 0 || nCement > 2) break;
			if(i == 0) time = 0.1; // 0���� ������ ��������...
			else time = i;
			data.dTime = time;
			data.dElast = S28 * exp(a[nCement] * (1 - pow((28/time), 0.5)));
			acode.Add(data);
		}
	}
	else if(ncode == 10) // CEB-FIP(1978)
	{
		for(int i=2; i<=1000; i++)
		{
			double time;
			double S28 = m_dCEBFIP1978CompStrength.GetEditValue();
			
			time = i;

			data.dTime = time;

			//double Tmpvalue = log10(time);
			//data.dElast = (0.0062*pow(Tmpvalue,3) - 0.1263*pow(Tmpvalue,2) + 0.6541*Tmpvalue - 0.0158)*S28;
			double dTmp1 = time/(time+47);
			double dTmp2 = 1/2.45;
			data.dElast = pow(dTmp1, dTmp2)*S28;
			acode.Add(data);
		}
	}
	else if(ncode == 11 || ncode == 33)  // KCI-USD12, KDS-2016
	{
		for(int i=0; i <= 30; i++)
		{
			double time;
			double a[5] = { 0.35, 0.15, 0.25, 0.12, 0.40 };
			double S28 = m_dCEBCompStrength.GetEditValue();
			int nCement = m_CEBCementType.GetCurSel();
			if(nCement < 0 || nCement > 5) break;
			if(i == 0) time = 0.1; // 0���� ������ ��������...
			else time = i;
			data.dTime = time;
			data.dElast = S28 * exp(a[nCement] * (1 - pow((28/time), 0.5)));
			acode.Add(data);
		}
	}
	else if(ncode == TDMT_CODE_AUSTRALIA_2009 || ncode == TDMT_CODE_AUSTRALIA_2011 ||
		    ncode == TDMT_CODE_AUSTRALIA_2016 || ncode == TDMT_CODE_AUSTRALIA_2017 || ncode == TDMT_CODE_AUSTRALIA_2018_AMD_2021) 	// AS 3600-2009, AS/RTA 5100.5-2011, AS 5100.5-2016, AS 5100.5-2017 ȣ�ֽð����� ���� ���߸����� ����
	{ // INDIA standard�� ��ȭâ ����� ����.
		double S28 = m_dINDIACompStrength.GetEditValue();
		for(int i=0; i <= 1000; i++)
		{ 
			data.dTime = i;
			data.dElast = 1.451849874*pow(i,0.75)*S28/(pow(i,0.75)+5.5);
			acode.Add(data);
		}
	}
	else if(ncode == TDMT_CODE_RUSSIAN) // Russian
	{
		double dFactor = GetFactorForCode(m_CurrUnit.nBase_Force, m_CurrUnit.nBase_Length);

		double dB28 = m_edtRusConcB.GetEditValue() * dFactor; // B-class of concrete compressive strength

		double da = 0.0;
		double dc = 0.0;
		double dd = 0.0;

		int nCenmentType = m_cmbRusCementType.GetCurSel(); // 0:Normal, 1:Fast Hardened 2:Slag 3:Pozzolan
		
		if(nCenmentType == 0) // 0:Normal
		{
			da = 23.0;
			dc = 55.0;
			dd = 11.0;
		}
		else if(nCenmentType == 1) // 1:Fast Hardened 
		{
			da =  9.0;
			dc = 31.0;
			dd =  6.0;
		}
		else if(nCenmentType == 2) // 2:Slag
		{
			da = 39.0;
			dc = 62.0;
			dd = 15.0;
		}
		else if(nCenmentType == 3) // 3:Pozzolan
		{
			da = 35.0;
			dc = 93.0;
			dd =  8.0;
		}
		else 
		{
			ASSERT(0);
		}

		double dRbg = 0.0;
		double dRbn = 0.0;

		for(int i=0; i <= 30; i++)
		{ 
			dRbg = (1 + ((da/(dc+dB28))*((i-28)/(i+dd)))) * dB28;
			//dRbn = (0.77 - (0.001*dRbg))*dRbg;
		
			data.dTime = i;
			//data.dElast = dRbn/dFactor;
			data.dElast = dRbg/dFactor;
			acode.Add(data);
		}
	}
	else ASSERT(0);

	m_Graph.KillData();
	int nNum = acode.GetSize();
	for(int i=0; i < nNum; i++)
	{
		SetValue(i, 0, acode[i].dTime, acode[i].dElast);
	}
	m_GraphView.Invalidate();
}

double CTdmeItemDlg::GetFactorForCode(int iCurrUnitFor, int iCurrUnitLen)
{
	double dFactor, dCurrLenFactor, dCurrForFactor;
	int iCodeUnitFor = D_UNITSYS_FORCE_INDEX_N;
	int iCodeUnitLen = D_UNITSYS_LENGTH_INDEX_MM;

//   int iCurrUnitFor = m_CurrUnit.nBase_Force;
//   int iCurrUnitLen = m_CurrUnit.nBase_Length;

	if(iCurrUnitLen==D_UNITSYS_LENGTH_INDEX_MM)       dCurrLenFactor = 1.0;
	else if(iCurrUnitLen==D_UNITSYS_LENGTH_INDEX_CM)  dCurrLenFactor = 10.0;
	else if(iCurrUnitLen==D_UNITSYS_LENGTH_INDEX_M)   dCurrLenFactor = 1000.0;
	else if(iCurrUnitLen==D_UNITSYS_LENGTH_INDEX_FT)  dCurrLenFactor = 304.8;
	else if(iCurrUnitLen==D_UNITSYS_LENGTH_INDEX_IN)  dCurrLenFactor = 25.4;
	else  ASSERT(0);

	if(iCurrUnitFor==D_UNITSYS_FORCE_INDEX_N)         dCurrForFactor = 1.0;
	else if(iCurrUnitFor==D_UNITSYS_FORCE_INDEX_KN)   dCurrForFactor = 1000.0;
	else if(iCurrUnitFor==D_UNITSYS_FORCE_INDEX_KG)   dCurrForFactor = 9.80665;
	else if(iCurrUnitFor==D_UNITSYS_FORCE_INDEX_TON)  dCurrForFactor = 9806.65;
	else if(iCurrUnitFor==D_UNITSYS_FORCE_INDEX_LBF)  dCurrForFactor = 4.448223;
	else if(iCurrUnitFor==D_UNITSYS_FORCE_INDEX_KIP)  dCurrForFactor = 4448.223;
	else  ASSERT(0);

	dFactor = dCurrForFactor / (dCurrLenFactor * dCurrLenFactor);
	return dFactor;
}

void CTdmeItemDlg::GraphUser()
{
	OnDataChanged();
}

double CTdmeItemDlg::OhzagiY(int nCement, double m)
{
	double a=0.0, b=0.0, c=0.0;
	double retY=0.0, x=0.0;

	if(nCement == 0)
	{
		if(m <= 30) a = 0.024,  b = -0.028, c = 0.01;
		else				a = -0.005, b = 0.162,  c = -0.286;
		if(m <= 6.626) return 1e-10;
	}
	else if(nCement == 1)
	{
		if(m <= 30) a = 0.012, b = -0.028, c = 0.02;
		else				a = 0.000, b = 0.112,  c = -0.347;
		if(m <= 8.488) return 1e-10;
	}
	else if(nCement == 2)
	{
		if(m <= 30) a = 0.01,  b = -0.027, c = 0.02;
		else				a = 0.006, b = 0.014,  c = -0.077;
		if(m <= 9.477) return 1e-10;
	}
	else if(nCement == 3)
	{
		if(m <= 30) a = 0.012, b = -0.034, c = 0.028;
		else				a = 0.002, b = 0.074,  c = -0.251;
		if(m <= 9.456) return 1e-10;
	}

	x = 2.389 * log(m/3.5) - 1.0;
	retY = a*pow(x, 2) + b*x + c;

	return retY;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void CTdmeItemDlg::OnTdmeSinRedrawGraph() 
{
	// TODO: Add your control notification handler code here
	DrawChart();
}

void CTdmeItemDlg::OnETCTDMECHARTXLog() 
{
	// TODO: Add your control notification handler code here
	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLogX(m_btnXLog.GetCheck());
	
	if (m_btnXLog.GetCheck() == TRUE || m_btnYLog.GetCheck() == TRUE) 
	{
		pD->GetStyle()->SetScaleLimit(TRUE);
	  pD->SetMinRangeY(0.001);   
	  pD->SetMinRangeX(0.001);   
	}
	else 
	 	pD->GetStyle()->SetScaleLimit(FALSE);
	
	m_GraphView.Invalidate();
}

void CTdmeItemDlg::OnETCTDMECHARTYLog() 
{
	// TODO: Add your control notification handler code here
	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLog(m_btnYLog.GetCheck());

	if (m_btnXLog.GetCheck() == TRUE || m_btnYLog.GetCheck() == TRUE) 
	{
		pD->GetStyle()->SetScaleLimit(TRUE);
	  pD->SetMinRangeY(0.001);
		pD->SetMinRangeX(0.001);
	}
	else 
	 	pD->GetStyle()->SetScaleLimit(FALSE);

	m_GraphView.Invalidate();	
}

void CTdmeItemDlg::OnPaint() 
{
	CDialog::OnPaint();
	CClientDC dc(this); // device context for painting

	HWND hWnd = this->GetSafeHwnd();
	if (!hWnd || !(::IsWindow(hWnd))) return;

	UpdateData(TRUE);
	if (m_type == 1) return;  // USER TYPE

	int nSel = m_CobxCode.GetCurSel();
	if (nSel == CB_ERR) { ASSERT(0); return; }
	CString strSel;
	m_CobxCode.GetLBText(nSel, strSel);

	CString aBitmapPath[] = {
		_T("SVG\\Illustration\\Dialog\\etc_tdme_kor.svg"),        // 0      
		_T("SVG\\Illustration\\Dialog\\etc_tdme_aci_eq.svg"),	  // 1
		_T("SVG\\Illustration\\Dialog\\etc_tdme_cebfip_eq.svg"),  // 2
		_T("SVG\\Illustration\\Dialog\\etc_tdme_ohzagi_eq.svg"),  // 3
		_T("SVG\\Illustration\\Dialog\\etc_tdme_jpn_eqe.svg"),	  // 4
		_T("SVG\\Illustration\\Dialog\\etc_tdme_jpn2_eq.svg"),	  // 5
		_T("SVG\\Illustration\\Dialog\\etc_tdme_india_eq.svg"),	  // 6
		_T("SVG\\Illustration\\Dialog\\etc_tdme_cebfip_eq.svg"),  // 7
		_T("SVG\\Illustration\\Dialog\\etc_tdme_russia_eq.svg"),  // 8
		_T("SVG\\Illustration\\Dialog\\etc_tdme_gilbert.svg"),    // 9
		_T("SVG\\Illustration\\Dialog\\etc_tdme_abnt6118_eq.svg"),    // 10
	};

	int nBitmap = -1;
	if (strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__Korea_Standard)) == 0)
		nBitmap = 0;
	else if (strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__ACI_STANDARD)) == 0)
	{
		nBitmap = 1;
	}
	else if (
		strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__CEBFIP_STANDARD)) == 0 ||
		strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__KCI_USD12_STANDARD)) == 0 ||
		strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__CEBFIP2010_STANDARD)) == 0 ||
		strSel.CompareNoCase(_T("KDS-2016")) == 0
	)
	{
		nBitmap = 2;
	}
	else if (strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__OHZAGI_STANDARD)) == 0)
	{
		nBitmap = 3;
	}
	else if (strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__JAPAN_HYDRATION_STANDARD)) == 0)
	{
		nBitmap = 4;
	}
	else if (strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__JAPAN_ELASTIC_STANDARD)) == 0)
	{
		nBitmap = 5;
	}
	else if (strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__INDIA_STANDARD)) == 0)
	{
		nBitmap = 6;
	}
	else if (strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__EUROPEAN)) == 0)
	{
		nBitmap = 7;
	}
	else if (
		strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__RUSSIAN)) == 0
		//strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__CEBFIP_1978_STANDARD)) == 0
	)
	{
		nBitmap = 8;
	}
	else if (
		//strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009)) == 0 ||
		//strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011)) == 0 ||
		//strSel.CompareNoCase(_T("INDIA(IRC:112-2011)")) == 0 ||
		strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__Gilbert_and_Ranzi_STANDARD)) == 0
	)
	{
		nBitmap = 9;
	}
	else if (strSel.CompareNoCase(_LS(IDS_WG_CMD__ADDD__ABNTNBR61182023)) == 0)
	{
		nBitmap = 10;
	}

	if (nBitmap == -1)
	{
		m_wndPicture.ShowWindow(SW_HIDE);
		return;
	}

	m_wndPicture.SetImage(aBitmapPath[nBitmap]);
	GetDlgItem(IDC_CMD_BITMAP)->ShowWindow(TRUE);
	m_wndPicture.ShowWindow(SW_SHOW);
}

void CTdmeItemDlg::OnTdmeJPNConcreteChk()
{
	BOOL bEnalbe;
	if (m_btnConcreteData.GetCheck())
	{
		bEnalbe = TRUE;
		OnSelchangeJapanCementCombo();
	}
	else
	{
		bEnalbe = FALSE;
	}
	m_JAPANCementType.EnableWindow(bEnalbe);
	m_dJAPANFactorA.SetReadOnly(bEnalbe);
	m_dJAPANFactorB.SetReadOnly(bEnalbe);
	m_dJAPANFactorD.SetReadOnly(bEnalbe);
}

void CTdmeItemDlg::OnSelchangeJapanCementCombo()
{
	double a, b, d;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pEditData->CalcTdmeConcFactor(m_JAPANCementType.GetCurSel(), a, b, d);
	m_dJAPANFactorA.SetEditUnit(a);
	m_dJAPANFactorB.SetEditUnit(b);
	m_dJAPANFactorD.SetEditUnit(d);
}

void CTdmeItemDlg::SetCementTypeSCmb()
{
	int nCode = CDlgUtil::CobxGetCurSelItemData(m_CobxCode, m_CobxCode.GetCurSel());

	m_CEBCementType.ResetContent();
	if(nCode == 11 || nCode == 33)
	{
		m_CEBCementType.AddString(_LS(IDS_CMD_TDME_KOREAN_CEMENT_TYPE_N_R_MOIST_CURED_035));
		m_CEBCementType.AddString(_LS(IDS_CMD_TDME_KOREAN_CEMENT_TYPE_N_R_STEAM_CURED_015));
		m_CEBCementType.AddString(_LS(IDS_CMD_TDME_KOREAN_CEMENT_TYPE_RS_MOIST_CURED_025));
		m_CEBCementType.AddString(_LS(IDS_CMD_TDME_KOREAN_CEMENT_TYPE_RS_STEAM_CURED_012));
		m_CEBCementType.AddString(_STR(_T("SL : 0.40")));
		m_CEBCementType.SetCurSel(0);
	}
	else if(nCode == 27)
	{
		m_CEBCementType.AddString(_STR(_T("42.5 R,52.5 N,52.5 R : 0.20")));
		m_CEBCementType.AddString(_STR(_T("32.5 R,42.5 N        : 0.25")));
		m_CEBCementType.AddString(_STR(_T("32.5 N							 : 0.38")));
		m_CEBCementType.SetCurSel(0);

	}
	else if (nCode == TDMT_CODE_ABNT_NBR_6118_2023)
	{
		m_CEBCementType.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_CTYPE_CMB1));
		m_CEBCementType.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_CTYPE_CMB2));
		m_CEBCementType.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_CTYPE_CMB3));
		m_CEBCementType.SetCurSel(0);
	}
	else
	{
		m_CEBCementType.AddString(_STR(_T("RS : 0.2")));
		m_CEBCementType.AddString(_STR(_T("N, R : 0.25")));
		m_CEBCementType.AddString(_STR(_T("SL : 0.38")));
		m_CEBCementType.SetCurSel(1);
	}
	
	if(nCode==27)
	{
		GetDlgItem(IDC_TDME_CEB_STATIC5)->ShowWindow(TRUE);
		m_CEBAffreType.ShowWindow(TRUE);
		m_CEBAffreType.ResetContent();
		m_CEBAffreType.AddString(_LS(IDS_CMD_ETC_TDME_CEB_FIP_2010_BASALT));
		m_CEBAffreType.AddString(_LS(IDS_CMD_ETC_TDME_CEB_FIP_2010_QUARTZITE));
		m_CEBAffreType.AddString(_LS(IDS_CMD_ETC_TDME_CEB_FIP_2010_LIMESTONE));
		m_CEBAffreType.AddString(_LS(IDS_CMD_ETC_TDME_CEB_FIP_2010_SANDSTONE));
		m_CEBAffreType.SetCurSel(0);
	}     
	else if (nCode == TDMT_CODE_ABNT_NBR_6118_2023)
	{
		GetDlgItem(IDC_TDME_CEB_STATIC5)->ShowWindow(TRUE);
		m_CEBAffreType.ShowWindow(TRUE);
		m_CEBAffreType.ResetContent();
		m_CEBAffreType.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_ATYPE_CMB1));
		m_CEBAffreType.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_ATYPE_CMB2));
		m_CEBAffreType.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_ATYPE_CMB3));
		m_CEBAffreType.AddString(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_ATYPE_CMB4));
		m_CEBAffreType.SetCurSel(0);
	}
	else
	{
		GetDlgItem(IDC_TDME_CEB_STATIC5)->ShowWindow(FALSE);
		m_CEBAffreType.ShowWindow(FALSE);
	}

    if (nCode == 35)
    {
        GetDlgItem(IDC_TDME_CEB_STATIC6)->ShowWindow(TRUE);
        m_dIND2011AffreType.ShowWindow(TRUE);
        m_dIND2011AffreType.ResetContent();
        //m_dIND2011AffreType.AddString(_LS(IDS_CMD_ETC_HCRP_CEB_FIP_2010_BASALT));
        //m_dIND2011AffreType.AddString(_LS(IDS_CMD_ETC_HCRP_CEB_FIP_2010_QUARTZITE));
        //m_dIND2011AffreType.AddString(_LS(IDS_CMD_ETC_HCRP_CEB_FIP_2010_LIMESTONE));
        //m_dIND2011AffreType.AddString(_LS(IDS_CMD_ETC_HCRP_CEB_FIP_2010_SANDSTONE));

        m_dIND2011AffreType.AddString(_LS(IDS_CMD_ETC_TDME_CEB_FIP_2010_BASALT));
        m_dIND2011AffreType.AddString(_LS(IDS_CMD_ETC_TDME_CEB_FIP_2010_QUARTZITE));
        m_dIND2011AffreType.AddString(_LS(IDS_CMD_ETC_TDME_CEB_FIP_2010_LIMESTONE));
        m_dIND2011AffreType.AddString(_LS(IDS_CMD_ETC_TDME_CEB_FIP_2010_SANDSTONE));
        m_dIND2011AffreType.SetCurSel(0);

    }
    else
    {
        GetDlgItem(IDC_TDME_CEB_STATIC6)->ShowWindow(FALSE);
        m_dIND2011AffreType.ShowWindow(FALSE);
    }

	if (nCode == 34)
	{
		m_GilbertCementType.ResetContent();
		m_GilbertCementType.AddString(_LS(IDS_CMD_ETC_TDME_GILBERT_038));
		m_GilbertCementType.AddString(_LS(IDS_CMD_ETC_TDME_GILBERT_025));
		m_GilbertCementType.SetCurSel(0);
	}
}

//void CTdmeItemDlg::OnTdmeMatlCmb()
//{
// 	if(m_cmbMatl.GetCount() == 0) return;
// 
// 	double dDensity = 0.0;
// 
// 	T_MATL_K MatlK;
// 	MatlK = m_cmbMatl.GetItemData(m_cmbMatl.GetCurSel());
// 
// 	T_MATL_D MatlD; MatlD.Initialize();
// 	
// 	CDBDoc* pDoc = CDBDoc::GetDocPoint();
// 	if(!pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlD))
// 	{
// 		ASSERT(0);
// 		m_edtDensity.SetEditUnit(dDensity);
// 		return;
// 	}
// 
// 	if(MatlD.Type == _T("C"))
// 	{
// 		dDensity = MatlD.Data1.Analysis.Density;
// 	}
// 	else if(MatlD.Type == _T("SRC"))
// 	{
// 		dDensity = MatlD.Data2.Analysis.Density;
// 	}
// 	else
// 	{
// 		ASSERT(0);
// 	}
// 
// 	dDensity = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KG, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, dDensity);
// 	m_edtDensity.SetEditUnit(dDensity);
//}

void CTdmeItemDlg::OnTdmeMatlBtn()
{
	//AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PROP_MATERIAL,0));

	CCMDRefwdKDS2016Dlg Dlg;
	if (IDOK == Dlg.DoModal())
	{
		m_edtDensity.SetEditUnit(Dlg.m_dDensity);
	}
}