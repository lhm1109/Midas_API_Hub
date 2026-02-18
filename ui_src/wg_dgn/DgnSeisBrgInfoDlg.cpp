//////////
// DgnSeisBrgInfoDlg.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisBrgInfoDlg.h"
#include "DgnSeisBrgInfoFile.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\PL_StrParser.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_cmd\CMMvcdDlg.h"
#include "..\wg_db\VehlDB.h"
#include "..\wg_db\VehlDef.h"
#include "..\wg_db\AttrCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// IRC
#define TYPENAME_Class_A                               _T("Class A")
#define TYPENAME_Class_B                               _T("Class B")
#define TYPENAME_Class_70R                             _T("Class 70R")
#define TYPENAME_Class_40R                             _T("Class 40R")
#define TYPENAME_Class_AA                              _T("Class AA")
#define TYPENAME_Footway                               _T("Footway")
#define TYPENAME_Fatigue                               _T("Fatigue Vehicle")

// IRS
#define TYPENAME_Broad_Gauge_1676mm                    _T("Broad Gauge-1676mm")
#define TYPENAME_Metre_Gauge_1000mm                    _T("Metre Gauge-1000mm")
#define TYPENAME_Narrow_Gauge_762mm                    _T("Narrow Gauge-762mm")
#define TYPENAME_Heavy_Mineral_Loadings                _T("Heavy Mineral Loadings")
#define TYPENAME_Footbridge_Footpath                   _T("Footbridge & Footpath")

// IRS - Broad Gauge-1676mm
#define SELNAME_Modified_B_G_Loading_1987_1            _T("Modified B.G. Loading 1987-1")
#define SELNAME_Modified_B_G_Loading_1987_2            _T("Modified B.G. Loading 1987-2")
#define SELNAME_B_G_Standard_Loading_1926_M_L          _T("B.G. Standard Loading 1926-M.L.")
#define SELNAME_B_G_Standard_Loading_1926_B_L          _T("B.G. Standard Loading 1926-B.L.")
#define SELNAME_Revised_B_G_Loading_1975_WG1_WG1       _T("Revised B.G. Loading 1975-WG1+WG1")
#define SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4A   _T("Revised B.G. Loading 1975-WAM4A+WAM4A")
#define SELNAME_Revised_B_G_Loading_1975_Bo_Bo_Bo_Bo   _T("Revised B.G. Loading 1975-Bo-Bo+Bo-Bo")
#define SELNAME_Revised_B_G_Loading_1975_WAM4A         _T("Revised B.G. Loading 1975-WAM4A")
#define SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4    _T("Revised B.G. Loading 1975-WAM4A+WAM4")
#define SELNAME_Revised_B_G_Loading_1975_WAM4A_WDM2    _T("Revised B.G. Loading 1975-WAM4A+WDM2")
#define SELNAME_25t_Loading_2008_Combination_1         _T("25t Loading-2008 Combination 1")
#define SELNAME_25t_Loading_2008_Combination_2         _T("25t Loading-2008 Combination 2")
#define SELNAME_25t_Loading_2008_Combination_3         _T("25t Loading-2008 Combination 3")
#define SELNAME_25t_Loading_2008_Combination_4         _T("25t Loading-2008 Combination 4")
#define SELNAME_25t_Loading_2008_Combination_5         _T("25t Loading-2008 Combination 5")
#define SELNAME_DFC_Loading_Combination_1              _T("DFC Loading Combination 1")
#define SELNAME_DFC_Loading_Combination_2              _T("DFC Loading Combination 2")
#define SELNAME_DFC_Loading_Combination_3              _T("DFC Loading Combination 3")
#define SELNAME_DFC_Loading_Combination_4              _T("DFC Loading Combination 4")
#define SELNAME_DFC_Loading_Combination_5              _T("DFC Loading Combination 5")

// IRS - Metre Gauge-1000mm
#define SELNAME_2_Co_Co_Locomotives                    _T("2 Co-Co Locomotives")
#define SELNAME_2_Bo_Bo_Locomotives                    _T("2 Bo-Bo Locomotives")
#define SELNAME_MGML_Loading_of_1929                   _T("MGML Loading of 1929")
#define SELNAME_M_L                                    _T("M.L.")
#define SELNAME_B_L                                    _T("B.L.")
#define SELNAME_C                                      _T("C.")

// IRS - Narrow Gauge-762mm
#define SELNAME_Class_H_B_B_or_Bo_Bo_Type              _T("Class H: B-B or Bo-Bo Type")
#define SELNAME_Class_H_C_C_or_Co_Co_Type              _T("Class H: C-C or Co-Co Type")
#define SELNAME_Class_H_Steam_Zf_1                     _T("Class H: Steam (Zf/1)")
#define SELNAME_Class_H_Diesel_Electric                _T("Class H: Diesel Electric")
#define SELNAME_Class_A_B_B_or_Bo_Bo_Type              _T("Class A: B-B or Bo-Bo Type")
#define SELNAME_Class_A_C_C_or_Co_Co_Type              _T("Class A: C-C or Co-Co Type")
#define SELNAME_Class_A_Diesel_Mech_Elec               _T("Class A: Diesel Mech./Elec.")
#define SELNAME_Class_A_Diesel_Mech_Elec_Articulated   _T("Class A: Diesel Mech./Elec.(Articulated)")
#define SELNAME_Class_A_DRG_No_CSO_C_873               _T("Class A: DRG No. CSO/C-873")
#define SELNAME_Class_B_B_B_or_Bo_Bo_Type              _T("Class B: B-B or Bo-Bo Type")
#define SELNAME_Class_B_Steam_Engine_Tank              _T("Class B: Steam Engine (Tank)")
#define SELNAME_Class_B_Steam_Engine_Tender            _T("Class B: Steam Engine (Tender)")
#define SELNAME_Class_B_Diesel_Electric                _T("Class B: Diesel Electric")

// IRS - Heavy Mineral Loadings
#define SELNAME_Train_Formation_No_1                   _T("Train Formation No.1")
#define SELNAME_Train_Formation_No_2                   _T("Train Formation No.2")
#define SELNAME_Train_Formation_No_3                   _T("Train Formation No.3")
#define SELNAME_Train_Formation_No_4                   _T("Train Formation No.4")
#define SELNAME_Train_Formation_No_5                   _T("Train Formation No.5")
#define SELNAME_Train_Formation_No_6                   _T("Train Formation No.6")
#define SELNAME_Train_Formation_No_7                   _T("Train Formation No.7")
#define SELNAME_Train_Formation_No_8                   _T("Train Formation No.8")
#define SELNAME_Train_Formation_No_9                   _T("Train Formation No.9")
#define SELNAME_Train_Formation_No_10                  _T("Train Formation No.10")
#define SELNAME_Train_Formation_No_11                  _T("Train Formation No.11")
#define SELNAME_Train_Formation_No_12                  _T("Train Formation No.12")
#define SELNAME_Train_Formation_No_13                  _T("Train Formation No.13")
#define SELNAME_Train_Formation_No_14                  _T("Train Formation No.14")
#define SELNAME_Train_Formation_No_15                  _T("Train Formation No.15")
#define SELNAME_Train_Formation_No_16                  _T("Train Formation No.16")
#define SELNAME_Train_Formation_No_17                  _T("Train Formation No.17")

// IRS - Footbridge & Footpath
#define SELNAME_Footbridge_Footpath                    _T("Footbridge & Footpath")

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisBrgInfoDlgdialog
CDgnSeisBrgInfoDlg::CDgnSeisBrgInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisBrgInfoDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnSeisBrgInfoDlg::~CDgnSeisBrgInfoDlg()
{

}

void CDgnSeisBrgInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisBrgInfoDlg)	
	DDX_Control(pDX, IDC_DGN_BRG_NAME_EDT, m_edtBrgName);
	DDX_Control(pDX, IDC_DGN_LOCATION_EDT, m_edtLocation);
	DDX_Control(pDX, IDC_DGN_MANAGER_EDT, m_edtManager);
	DDX_Control(pDX, IDC_DGN_LINENUM_EDT, m_edtstrLineNum);
	DDX_Control(pDX, IDC_DGN_ORG_LOC_EDT, m_edtOrgLoc);
	DDX_Control(pDX, IDC_DGN_BUILD_DATE_EDT, m_edtBuildYear);
	DDX_Control(pDX, IDC_DGN_TOTAL_LEN_EDT, m_edtTotalLen);
	DDX_Control(pDX, IDC_DGN_SPAN_EDT, m_edtSpan);
	DDX_Control(pDX, IDC_DGN_BRGWIDTH_EDT, m_edtBrgWidth);
	DDX_Control(pDX, IDC_DGN_UPPER_EDT, m_edtUpper);
	DDX_Control(pDX, IDC_DGN_ABUTMENT_TYPE_EDT, m_edtAbutmentType);
	DDX_Control(pDX, IDC_DGN_ABUTMENT_BASE_EDT, m_edtAbutmentBase);
	DDX_Control(pDX, IDC_DGN_PIER_TYPE_EDT, m_edtPierType);
	DDX_Control(pDX, IDC_DGN_PIER_BASE_EDT, m_edtPierBase);
	DDX_Control(pDX, IDC_DGN_SUPPORT_MATL_EDT, m_edtSupportMatl);
	DDX_Control(pDX, IDC_DGN_SUPPORT_CAPAABUT_EDT, m_edtSupportCapaAbut);
	DDX_Control(pDX, IDC_DGN_SUPPORT_CAPAPIER_EDT, m_edtSupportCapaPier);
	DDX_Control(pDX, IDC_DGN_PIER_SECT_EDT, m_edtPierSect);
	DDX_Control(pDX, IDC_DGN_TOTAL_LEN_UNT, m_untTotalLen);
	DDX_Control(pDX, IDC_DGN_SPAN_UNT, m_untSpan);
	DDX_Control(pDX, IDC_DGN_BRGWIDTH_UNT, m_untBrgWidth);
	DDX_Control(pDX, IDC_DGN_SUPPORT_CAPAABUT_UNT, m_untSupportCapaAbut);
	DDX_Control(pDX, IDC_DGN_SUPPORT_CAPAPIER_UNT, m_untSupportCapaPier);
	DDX_Control(pDX, IDC_DGN_SEISMIC_CLASS_CBX, m_cbxSeismicClass);
	DDX_Control(pDX, IDC_DGN_EARTHQUAKE_CBX, m_cbxEarthquake);
	DDX_Control(pDX, IDC_DGN_MV_CODE_CBX, m_cbxMvCode);
	DDX_Control(pDX, IDC_DGN_MVHL_STD_CBX, m_cbxMvhlStd);
	DDX_Control(pDX, IDC_DGN_MVHL_TYPE_CBX, m_cbxMvhlType);
	DDX_Control(pDX, IDC_DGN_MVHL_NAME_CBX, m_cbxMvhlName);
	DDX_Control(pDX, IDC_DGN_UPPER_CBX, m_cbxUpper);
	DDX_Control(pDX, IDC_DGN_ABUTMENT_TYPE_CBX, m_cbxAbutmentType);
	DDX_Control(pDX, IDC_DGN_ABUTMENT_BASE_CBX, m_cbxAbutmentBase);
	DDX_Control(pDX, IDC_DGN_PIER_TYPE_CBX, m_cbxPierType);
	DDX_Control(pDX, IDC_DGN_PIER_BASE_CBX, m_cbxPierBase);
	DDX_Control(pDX, IDC_DGN_SUPPORT_MATL_CBX, m_cbxSupportMatl);
	DDX_Control(pDX, IDC_DGN_PIER_SECT_CBX, m_cbxPierSect);
	DDX_Control(pDX, IDC_DGN_SPFCK_CBX, m_cbxSpfc);
	//DDX_Control(pDX, IDC_DGN_RATING_PARAM_STEEL_UNIT, m_TensSteelUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnSeisBrgInfoDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisBrgInfoDlg)
	ON_BN_CLICKED(IDC_DGN_OPEN_BTN, OnOpen)
	ON_BN_CLICKED(IDC_DGN_SAVE_ASBTN, OnSaveAs)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_CBN_SELCHANGE(IDC_DGN_MV_CODE_CBX, OnSelchangeCmdMvCode)
	ON_CBN_SELCHANGE(IDC_DGN_MVHL_STD_CBX, OnSelchangeCmdMvhlStd)
	ON_CBN_SELCHANGE(IDC_DGN_MVHL_TYPE_CBX, OnSelchangeCmdMvhlType)
	ON_CBN_SELCHANGE(IDC_DGN_UPPER_CBX, OnSelchangeForEnable)
	ON_CBN_SELCHANGE(IDC_DGN_ABUTMENT_TYPE_CBX, OnSelchangeForEnable)
	ON_CBN_SELCHANGE(IDC_DGN_ABUTMENT_BASE_CBX, OnSelchangeForEnable)
	ON_CBN_SELCHANGE(IDC_DGN_PIER_TYPE_CBX, OnSelchangeForEnable)
	ON_CBN_SELCHANGE(IDC_DGN_PIER_BASE_CBX, OnSelchangeForEnable)
	ON_CBN_SELCHANGE(IDC_DGN_SUPPORT_MATL_CBX, OnSelchangeForEnable)
	ON_CBN_SELCHANGE(IDC_DGN_PIER_SECT_CBX, OnSelchangeForEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDgnSeisBrgInfoDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	SetDefVal();
	SetControl();
	Data2Dlg();
	EnableCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisBrgInfoDlg::SetDefVal()
{
	m_Data.Initialize();
	if (!m_pDoc->m_pAttrCtrl2->GetSbri(m_Data))
	{
		T_MVCD_D MvcdD;
		MvcdD.Initialize();
		m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD);
		if (MvcdD.nCodeType == D_MOVE_CODE_NONE)
		{
			m_Data.nMvCode = D_MOVE_CODE_KOREA;
		}
		else
		{
			m_Data.nMvCode = MvcdD.nCodeType;
		}
		m_Data.nMvhlStd = D_MVHL_KOR_STND_LOAD;
		m_Data.strMvhlType = _T("DB-24");
	}
}

void CDgnSeisBrgInfoDlg::SetControl()
{
	m_edtBuildYear.SetUnitType(D_UNITSYS_NONE);
	m_edtTotalLen.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtBrgWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtSupportCapaAbut.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtSupportCapaPier.SetUnitType(D_UNITSYS_BASE_FORCE);

	m_untTotalLen.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSpan.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untBrgWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSupportCapaAbut.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untSupportCapaPier.SetUnitType(D_UNITSYS_BASE_FORCE);

	m_cbxSeismicClass.ResetContent();
	m_cbxSeismicClass.AddString(_LS(IDS_DGN_SEIS_SPECIAL));
	m_cbxSeismicClass.AddString(_T("1"));
	m_cbxSeismicClass.AddString(_T("2"));
	m_cbxSeismicClass.SetCurSel(0);

	m_cbxEarthquake.ResetContent();
	m_cbxEarthquake.AddString(_T("1"));
	m_cbxEarthquake.AddString(_T("2"));
	m_cbxEarthquake.SetCurSel(0);

	m_cbxUpper.ResetContent();
	m_cbxUpper.AddString(_LS(IDS_DGN_NONE));
	m_cbxUpper.AddString(_LS(IDS_DGN_SEIS_PSC_I_GIRDER));
	m_cbxUpper.AddString(_LS(IDS_DGN_SEIS_PSC_BOX_GIRDER));
	m_cbxUpper.AddString(_LS(IDS_DGN_SEIS_RC_SLAB));
	m_cbxUpper.AddString(_LS(IDS_DGN_SEIS_STEEL_I_GIRDER));
	m_cbxUpper.AddString(_LS(IDS_DGN_SEIS_STEEL_BOX_GIRDER));
	m_cbxUpper.SetCurSel(0);

	m_cbxAbutmentType.ResetContent();
	m_cbxAbutmentType.AddString(_LS(IDS_DGN_NONE));
	m_cbxAbutmentType.AddString(_LS(IDS_DGN_SEIS_REV_T));
	m_cbxAbutmentType.AddString(_LS(IDS_DGN_SEIS_GRAV));
	m_cbxAbutmentType.AddString(_LS(IDS_DGN_SEIS_SUB_WALL));
	m_cbxAbutmentType.AddString(_LS(IDS_DGN_SEIS_BOX));
	m_cbxAbutmentType.AddString(_LS(IDS_DGN_SEIS_RAMAN));
	m_cbxAbutmentType.AddString(_LS(IDS_DGN_SEIS_MID_CON));
	m_cbxAbutmentType.SetCurSel(0);

	m_cbxAbutmentBase.ResetContent();
	m_cbxAbutmentBase.AddString(_LS(IDS_DGN_NONE));
	m_cbxAbutmentBase.AddString(_LS(IDS_DGN_SEIS_BASE));
	m_cbxAbutmentBase.AddString(_LS(IDS_DGN_SEIS_MASS_BASE));
	m_cbxAbutmentBase.AddString(_LS(IDS_DGN_SEIS_PHC));
	m_cbxAbutmentBase.AddString(_LS(IDS_DGN_SEIS_STR));
	m_cbxAbutmentBase.AddString(_LS(IDS_DGN_SEIS_BASE_DT));
	m_cbxAbutmentBase.AddString(_LS(IDS_DGN_SEIS_WALL));
	m_cbxAbutmentBase.AddString(_LS(IDS_DGN_SEIS_ZOOM));
	m_cbxAbutmentBase.SetCurSel(0);

	m_cbxPierType.ResetContent();
	m_cbxPierType.AddString(_LS(IDS_DGN_NONE));
	m_cbxPierType.AddString(_LS(IDS_DGN_SEIS_T));
	m_cbxPierType.AddString(_LS(IDS_DGN_SEIS_DOOR));
	m_cbxPierType.AddString(_LS(IDS_DGN_SEIS_TORCH));
	m_cbxPierType.AddString(_LS(IDS_DGN_SEIS_RAMAN));
	m_cbxPierType.SetCurSel(0);

	m_cbxPierBase.ResetContent();
	m_cbxPierBase.AddString(_LS(IDS_DGN_NONE));
	m_cbxPierBase.AddString(_LS(IDS_DGN_SEIS_BASE));
	m_cbxPierBase.AddString(_LS(IDS_DGN_SEIS_MASS_BASE));
	m_cbxPierBase.AddString(_LS(IDS_DGN_SEIS_PHC));
	m_cbxPierBase.AddString(_LS(IDS_DGN_SEIS_STR));
	m_cbxPierBase.AddString(_LS(IDS_DGN_SEIS_BASE_DT));
	m_cbxPierBase.AddString(_LS(IDS_DGN_SEIS_WALL));
	m_cbxPierBase.AddString(_LS(IDS_DGN_SEIS_ZOOM));
	m_cbxPierBase.SetCurSel(0);


	m_cbxSupportMatl.ResetContent();
	m_cbxSupportMatl.AddString(_LS(IDS_DGN_NONE));
	m_cbxSupportMatl.AddString(_LS(IDS_DGN_SEIS_HIGH_STR));
	m_cbxSupportMatl.AddString(_LS(IDS_DGN_SEIS_ELAST_BOT));
	m_cbxSupportMatl.AddString(_LS(IDS_DGN_SEIS_POT_BOT));
	m_cbxSupportMatl.SetCurSel(0);

	m_cbxPierSect.ResetContent();
	m_cbxPierSect.AddString(_LS(IDS_DGN_NONE));
	m_cbxPierSect.AddString(_LS(IDS_DGN_SEIS_POT_CIRCLE));
	m_cbxPierSect.AddString(_LS(IDS_DGN_SEIS_POT_RECT));
	m_cbxPierSect.AddString(_LS(IDS_DGN_SEIS_POT_TRACK));
	m_cbxPierSect.SetCurSel(0);

	m_cbxSpfc.ResetContent();

	CDlgUtil::CobxAddItem(m_cbxSpfc, _LS(IDS_DGN__NONE), 0);

	CArray<T_SPFC_K, T_SPFC_K> aSpfcK;
	m_pDoc->m_pAttrCtrl->GetSpfcKeyList(aSpfcK);
	T_SPFC_D SpfcD;
	for (int i = 0; i < aSpfcK.GetSize(); i++)
	{
		SpfcD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetSpfc(aSpfcK[i], SpfcD)) ASSERT(0);
		CDlgUtil::CobxAddItem(m_cbxSpfc, SpfcD.SpecFuncName, aSpfcK[i]);
	}

	if (aSpfcK.GetSize() > 0) m_cbxSpfc.SetCurSel(1);
	else                      m_cbxSpfc.SetCurSel(0);	

	SetMvCodeCbx();
}

void CDgnSeisBrgInfoDlg::OnOk()
{
	if (!Dlg2Data())
	{
		return;
	}

	if (m_pDoc->m_pDataCtrl->AddSbri(m_Data))
	{
		CDialogMove::OnOK();
	}
}

BOOL CDgnSeisBrgInfoDlg::Dlg2Data()
{
	m_edtBrgName.GetWindowText(m_Data.strBrgName);
	m_edtLocation.GetWindowText(m_Data.strLocation);
	m_edtManager.GetWindowText(m_Data.strManager);
	m_edtstrLineNum.GetWindowText(m_Data.strLineNum);
	m_edtOrgLoc.GetWindowText(m_Data.strOrgLoc);
	m_Data.nBuildYear = m_edtBuildYear.GetEditValue();
	m_Data.dTotalLen = m_edtTotalLen.GetEditValue();
	CString strSpan;
	m_edtSpan.GetWindowText(strSpan);
	CStrParser::GetUEDistance(strSpan, m_Data.aSpan);
	m_Data.dBrgWidth = m_edtBrgWidth.GetEditValue();
	m_edtUpper.GetWindowText(m_Data.strUpper);
	m_edtAbutmentType.GetWindowText(m_Data.strAbutmentType);
	m_edtAbutmentBase.GetWindowText(m_Data.strAbutmentBase);
	m_edtPierType.GetWindowText(m_Data.strPierType);
	m_edtPierBase.GetWindowText(m_Data.strPierBase);
	m_edtSupportMatl.GetWindowText(m_Data.strSupportMatl);
	m_Data.dSupportCapaAbut = m_edtSupportCapaAbut.GetEditValue();
	m_Data.dSupportCapaPier = m_edtSupportCapaPier.GetEditValue();
	m_edtPierSect.GetWindowText(m_Data.strPierSect);

	m_Data.nSeismicClass = m_cbxSeismicClass.GetCurSel();
	m_Data.nEarthquake = m_cbxEarthquake.GetCurSel();
	m_Data.nMvCode = CDlgUtil::CobxGetCurSelItemData(m_cbxMvCode, m_cbxMvCode.GetCurSel());
	m_Data.nMvhlStd = CDlgUtil::CobxGetCurSelItemData(m_cbxMvhlStd, m_cbxMvhlStd.GetCurSel());
	m_cbxMvhlType.GetWindowText(m_Data.strMvhlType);
	m_cbxMvhlName.GetWindowText(m_Data.strMvhlName);
	m_Data.nUpper = m_cbxUpper.GetCurSel();
	m_Data.nAbutmentType = m_cbxAbutmentType.GetCurSel();
	m_Data.nAbutmentBase = m_cbxAbutmentBase.GetCurSel();
	m_Data.nPierType = m_cbxPierType.GetCurSel();
	m_Data.nPierBase = m_cbxPierBase.GetCurSel();
	m_Data.nSupportMatl = m_cbxSupportMatl.GetCurSel();
	m_Data.nPierSect = m_cbxPierSect.GetCurSel();
	DWORD nCurSelItem;
	CDlgUtil::CobxGetItemDataByCurSel(m_cbxSpfc, nCurSelItem);
	m_Data.SpfcK = nCurSelItem;

	BOOL bCheck = TRUE;
	int nReplace = 0;
	nReplace = m_Data.strBrgName     .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 교량명 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strLocation    .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 위치 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strManager     .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 관리주체 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strLineNum     .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 노선번호 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strOrgLoc      .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 소재지 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strMvhlType    .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 설계하중 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strMvhlName    .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 설계하중 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strUpper       .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 상부형식 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strAbutmentType.Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 교대형식 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strAbutmentBase.Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 교대기초 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strPierType    .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 교각형식 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strPierBase    .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 교각기초 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strSupportMatl .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 받침제원 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}
	nReplace = m_Data.strPierSect    .Replace(_T(","), _T("_"));  if (nReplace>0) { AfxMessageBox(_T("[오류] 교각단면 데이터에 쉼표( , )를 제거해 주세요.")); return FALSE;}

	return TRUE;
}

void CDgnSeisBrgInfoDlg::Data2Dlg()
{
	m_edtBrgName.SetWindowText(m_Data.strBrgName);
	m_edtLocation.SetWindowText(m_Data.strLocation);
	m_edtManager.SetWindowText(m_Data.strManager);
	m_edtstrLineNum.SetWindowText(m_Data.strLineNum);
	m_edtOrgLoc.SetWindowText(m_Data.strOrgLoc);
	m_edtBuildYear.SetEditUnit(m_Data.nBuildYear);
	m_edtTotalLen.SetEditUnit(m_Data.dTotalLen);
	CString strSpan;
	strSpan = CStrParser::ConvtDistArr2DistStr(m_Data.aSpan);
	m_edtSpan.SetWindowText(strSpan);
	m_edtBrgWidth.SetEditUnit(m_Data.dBrgWidth);
	m_edtUpper.SetWindowText(m_Data.strUpper);
	m_edtAbutmentType.SetWindowText(m_Data.strAbutmentType);
	m_edtAbutmentBase.SetWindowText(m_Data.strAbutmentBase);
	m_edtPierType.SetWindowText(m_Data.strPierType);
	m_edtPierBase.SetWindowText(m_Data.strPierBase);
	m_edtSupportMatl.SetWindowText(m_Data.strSupportMatl);
	m_edtSupportCapaAbut.SetEditUnit(m_Data.dSupportCapaAbut);
	m_edtSupportCapaPier.SetEditUnit(m_Data.dSupportCapaPier);
	m_edtPierSect.SetWindowText(m_Data.strPierSect);

	m_cbxSeismicClass.SetCurSel(m_Data.nSeismicClass);
	m_cbxEarthquake.SetCurSel(m_Data.nEarthquake);
	CDlgUtil::CobxSetCurSelItemData(m_cbxMvCode, m_Data.nMvCode);
	SetMvhlStdCbx();
	CDlgUtil::CobxSetCurSelItemData(m_cbxMvhlStd, m_Data.nMvhlStd);
	SetMvhlTypeCbx();
	CString strTmp;
	if (!m_Data.strMvhlType.IsEmpty())
	{
		int nCnt = m_cbxMvhlType.GetCount();
		for (int i = 0; i < nCnt; i++)
		{
			m_cbxMvhlType.GetLBText(i, strTmp);
			if (strTmp == m_Data.strMvhlType)
			{
				m_cbxMvhlType.SetCurSel(i);
				break;
			}
		}
	}
	SetMvhlNameCbx();
	if (!m_Data.strMvhlName.IsEmpty())
	{
		int nCnt = m_cbxMvhlName.GetCount();
		for (int i = 0; i < nCnt; i++)
		{
			m_cbxMvhlName.GetLBText(i, strTmp);
			if (strTmp == m_Data.strMvhlName)
			{
				m_cbxMvhlName.SetCurSel(i);
				break;
			}
		}
	}
	m_cbxUpper.SetCurSel(m_Data.nUpper);
	m_cbxAbutmentType.SetCurSel(m_Data.nAbutmentType);
	m_cbxAbutmentBase.SetCurSel(m_Data.nAbutmentBase);
	m_cbxPierType.SetCurSel(m_Data.nPierType);
	m_cbxPierBase.SetCurSel(m_Data.nPierBase);
	m_cbxSupportMatl.SetCurSel(m_Data.nSupportMatl);
	m_cbxPierSect.SetCurSel(m_Data.nPierSect);
	if (m_Data.SpfcK != NULL)
	{
		CDlgUtil::CobxSetCurSelItemData(m_cbxSpfc, m_Data.SpfcK);
	}
}

void CDgnSeisBrgInfoDlg::EnableCtrl()
{
	GetDlgItem(IDC_DGN_MVHL_NAME_CBX)->EnableWindow(m_cbxMvhlName.GetCount() > 0);
	GetDlgItem(IDC_DGN_UPPER_EDT)->EnableWindow(m_cbxUpper.GetCurSel() == 0);
	GetDlgItem(IDC_DGN_ABUTMENT_TYPE_EDT)->EnableWindow(m_cbxAbutmentType.GetCurSel() == 0);
	GetDlgItem(IDC_DGN_ABUTMENT_BASE_EDT)->EnableWindow(m_cbxAbutmentBase.GetCurSel() == 0);
	GetDlgItem(IDC_DGN_PIER_TYPE_EDT)->EnableWindow(m_cbxPierType.GetCurSel() == 0);
	GetDlgItem(IDC_DGN_PIER_BASE_EDT)->EnableWindow(m_cbxPierBase.GetCurSel() == 0);
	GetDlgItem(IDC_DGN_SUPPORT_MATL_EDT)->EnableWindow(m_cbxSupportMatl.GetCurSel() == 0);
	GetDlgItem(IDC_DGN_PIER_SECT_EDT)->EnableWindow(m_cbxPierSect.GetCurSel() == 0);
}

void CDgnSeisBrgInfoDlg::OnOpen()
{
	CDgnSeisBrgInfoFile File;
	File.ReadFromFile(m_Data);
	Data2Dlg();
}

void CDgnSeisBrgInfoDlg::OnSaveAs()
{
	Dlg2Data();
	CDgnSeisBrgInfoFile File;
	File.SaveToFile(m_Data);
}

void CDgnSeisBrgInfoDlg::OnCancel()
{

	CDialogMove::OnCancel();
}

void CDgnSeisBrgInfoDlg::OnSelchangeCmdMvCode()
{
	SetMvhlStdCbx();
	SetMvhlTypeCbx();
	SetMvhlNameCbx();
}

void CDgnSeisBrgInfoDlg::OnSelchangeCmdMvhlStd()
{
	SetMvhlTypeCbx();
	SetMvhlNameCbx();
}

void CDgnSeisBrgInfoDlg::OnSelchangeForEnable()
{
	EnableCtrl();
}

void CDgnSeisBrgInfoDlg::OnSelchangeCmdMvhlType()
{
	SetMvhlNameCbx();
}

void CDgnSeisBrgInfoDlg::SetMvCodeCbx()
{
	CCMMvcdDlg dlg;
	CStringArray      aCodeName;  aCodeName.RemoveAll();
	CArray<int, int>  aCodeData;  aCodeData.RemoveAll();
	CString strInitCodeName = _T("");
	dlg.GetCodeNameList(aCodeName);
	dlg.GetCodeDataList(aCodeData);
	strInitCodeName = dlg.GetInitCodeName();

	m_cbxMvCode.ResetContent();
	for (int i = 0; i < aCodeName.GetSize(); i++)
	{
		if (aCodeData[i] == D_MOVE_CODE_TRANS) continue;
		CDlgUtil::CobxAddItem(m_cbxMvCode, aCodeName[i], aCodeData[i]);
	}
	AdjustComboListBox(m_cbxMvCode);
}

void CDgnSeisBrgInfoDlg::SetMvhlStdCbx()
{
	int nCurMvCode;
	nCurMvCode = CDlgUtil::CobxGetCurSelItemData(m_cbxMvCode, m_cbxMvCode.GetCurSel());

	m_cbxMvhlStd.ResetContent();
	if (nCurMvCode == D_MOVE_CODE_NONE)
	{
		return;
	}

	if (nCurMvCode == D_MOVE_CODE_EURO_BS)
	{
		SetMvhlStdCbxEuroBS();
		return;
	}

	int nCodeSeq[57];
	int nNumIter = 15;
	switch (nCurMvCode)
	{
	case D_MOVE_CODE_AASHTO_STAN:
		nCodeSeq[0] = D_MVHL_AASHTO_STAND;
		nCodeSeq[1] = D_MVHL_AASHTO_LEGAL;
		nCodeSeq[2] = D_MVHL_CALTRANS_STND_2017;
		nCodeSeq[3] = D_MVHL_CALTRANS_STND;
		nCodeSeq[4] = D_MVHL_TURKEY;
		nCodeSeq[5] = D_MVHL_OTHERS;
		nNumIter = 6;
		break;
	case D_MOVE_CODE_AASHTO_LRFD:
		nCodeSeq[ 0] = D_MVHL_AASHTO_LRFD;
		nCodeSeq[ 1] = D_MVHL_AASHTO_STAND;
		nCodeSeq[ 2] = D_MVHL_AASHTO_LRFD_LEGAL;
		nCodeSeq[ 3] = D_MVHL_ALASKA;
		nCodeSeq[ 4] = D_MVHL_ALABAMA;
		nCodeSeq[ 5] = D_MVHL_ARIZONA;
		nCodeSeq[ 6] = D_MVHL_ARKANSAS;
		nCodeSeq[ 7] = D_MVHL_CALIFORNIA;
		nCodeSeq[ 8] = D_MVHL_COLORADO;
		nCodeSeq[ 9] = D_MVHL_CONNECTICUT;
		nCodeSeq[10] = D_MVHL_DISTRICT_OF_COLUMBIA;
		nCodeSeq[11] = D_MVHL_DELAWARE;
		nCodeSeq[12] = D_MVHL_FLORIDA;
		nCodeSeq[13] = D_MVHL_GEORGIA;
		nCodeSeq[14] = D_MVHL_HAWAII;
		nCodeSeq[15] = D_MVHL_AASHTO_IADOT;
		nCodeSeq[16] = D_MVHL_IDAHO;
		nCodeSeq[17] = D_MVHL_AASHTO_ILDOT;
		nCodeSeq[18] = D_MVHL_AASHTO_INDOT;
		nCodeSeq[19] = D_MVHL_KANSAS;
		nCodeSeq[20] = D_MVHL_KENTUCKY;
		nCodeSeq[21] = D_MVHL_AASHTO_LADOT;
		nCodeSeq[22] = D_MVHL_MASSACHUSETTS;
		nCodeSeq[23] = D_MVHL_MARYLAND;
		nCodeSeq[24] = D_MVHL_MAINE;
		nCodeSeq[25] = D_MVHL_MICHIGAN_NL;
		nCodeSeq[26] = D_MVHL_MICHIGAN_DL;
		nCodeSeq[27] = D_MVHL_MICHIGAN_SD;
		nCodeSeq[28] = D_MVHL_MINNESOTA;
		nCodeSeq[29] = D_MVHL_AASHTO_MODOT;
		nCodeSeq[30] = D_MVHL_MISSISSIPI;
		nCodeSeq[31] = D_MVHL_NORTHDAKOTA;
		nCodeSeq[32] = D_MVHL_NEBRASKA;
		nCodeSeq[33] = D_MVHL_NEVADA;
		nCodeSeq[34] = D_MVHL_NEWJERSEY;
		nCodeSeq[35] = D_MVHL_NEWMEXICO;
		nCodeSeq[36] = D_MVHL_NEWYORK;
		nCodeSeq[37] = D_MVHL_AASHTO_OHDOT;
		nCodeSeq[38] = D_MVHL_OKLAHOMA;
		nCodeSeq[39] = D_MVHL_OREGON;
		nCodeSeq[40] = D_MVHL_AASHTO_RIDOT;
		nCodeSeq[41] = D_MVHL_SOUTHCAROLINA;
		nCodeSeq[42] = D_MVHL_SOUTHDAKOTA;
		nCodeSeq[43] = D_MVHL_TENNESSEE;
		nCodeSeq[44] = D_MVHL_TEXAS;
		nCodeSeq[45] = D_MVHL_UTAH;
		nCodeSeq[46] = D_MVHL_AASHTO_VADOT;
		nCodeSeq[47] = D_MVHL_VERMONT;
		nCodeSeq[48] = D_MVHL_WASHINGTON;
		nCodeSeq[49] = D_MVHL_AASHTO_WIDOT;
		nCodeSeq[50] = D_MVHL_WESTVIRGINIA;
		nCodeSeq[51] = D_MVHL_FAST_ACT_EV_LOAD;
		nCodeSeq[52] = D_MVHL_CALTRANS_STND_2017;
		nCodeSeq[53] = D_MVHL_CALTRANS_STND;
		nCodeSeq[54] = D_MVHL_TURKEY;
		nCodeSeq[55] = D_MVHL_COLOMBIA;
		nCodeSeq[56] = D_MVHL_OTHERS;
		nNumIter = 57;
		break;
	case D_MOVE_CODE_CHINA:
		nCodeSeq[0] = D_MVHL_CH_JTGB01_2014;
		nCodeSeq[1] = D_MVHL_CH_JTGB01_2003;
		nCodeSeq[2] = D_MVHL_CH_JTJ001_97;
		nCodeSeq[3] = D_MVHL_CH_CJJ77_98;
		nCodeSeq[4] = D_MVHL_CH_CJJ11_2011;
		nCodeSeq[5] = D_MVHL_CH_CJJ11_2019;
		nCodeSeq[6] = D_MVHL_CH_TB100021_99;
		nCodeSeq[7] = D_MVHL_CH_TB10002_2017;
		nNumIter = 8;
		break;
	case D_MOVE_CODE_TAIWAN:
		nCodeSeq[0] = D_MVHL_ROC_TAIWAN;
		nNumIter = 1;
		break;
	case D_MOVE_CODE_JAPAN:
		nCodeSeq[0] = D_MVHL_AASHTO_STAND;
		nCodeSeq[1] = D_MVHL_AASHTO_LRFD;
		nCodeSeq[2] = D_MVHL_CALTRANS_STND_2017;
		nCodeSeq[3] = D_MVHL_CALTRANS_STND;
		nCodeSeq[4] = D_MVHL_KOR_STND_LOAD;
		nCodeSeq[5] = D_MVHL_KOR_STND_2005;
		nCodeSeq[6] = D_MVHL_KOR_RAIL_LOAD;
		nCodeSeq[7] = D_MVHL_KOR_RAIL_2004;
		nNumIter = 8;
		break;
	case D_MOVE_CODE_KOREA:
		nCodeSeq[0] = D_MVHL_KOR_STND_LOAD;
		nCodeSeq[1] = D_MVHL_KOR_STND_2005;
		nCodeSeq[2] = D_MVHL_KOR_RAIL_2012;
		nCodeSeq[3] = D_MVHL_KOR_RAIL_LOAD;
		nCodeSeq[4] = D_MVHL_KOR_RAIL_2004;
		nCodeSeq[5] = D_MVHL_AASHTO_STAND;
		nCodeSeq[6] = D_MVHL_AASHTO_LRFD;
		nCodeSeq[7] = D_MVHL_CALTRANS_STND_2017;
		nCodeSeq[8] = D_MVHL_CALTRANS_STND;
		nCodeSeq[9] = D_MVHL_OTHERS;
		nNumIter = 10;
		break;
	case D_MOVE_CODE_CANADA:
		nCodeSeq[0] = D_MVHL_CANCSA_S600;
		nNumIter = 1;
		break;
	case D_MOVE_CODE_NEWZEALAND:
		nCodeSeq[0] = D_MVHL_TRAFFIC_LOAD_NZ;
		nCodeSeq[1] = D_MVHL_GENERAL_ACCESS_NZ;
		nCodeSeq[2] = D_MVHL_50MAX_NZ;
		nCodeSeq[3] = D_MVHL_HPMV_NZ;
		nCodeSeq[4] = D_MVHL_DECK_EVALUATION;
		nCodeSeq[5] = D_MVHL_FATIGUE_NZ;
		nNumIter = 6;
		break;
	case D_MOVE_CODE_BRAZIL:
		nCodeSeq[0] = D_MVHL_NBR7188_ROADWAY;
		nCodeSeq[1] = D_MVHL_NBR7188_FOOTWAY;
		nCodeSeq[2] = D_MVHL_NBR7189_RAILWAY;
		nNumIter = 3;
		break;
	case D_MOVE_CODE_PENDOT:
		nCodeSeq[0] = D_MVHL_AASHTO_PENDOT;
		nCodeSeq[1] = D_MVHL_OTHERS;
		nNumIter = 2;
		break;
	case D_MOVE_CODE_BS:
		nCodeSeq[0] = D_MVHL_BS_5400;
		nCodeSeq[1] = D_MVHL_BS_BD3701;
		nCodeSeq[2] = D_MVHL_BS_BD8611;
		nNumIter = 3;
		break;
	case D_MOVE_CODE_KOREA_LRFD_2011:
		nCodeSeq[0] = D_MVHL_KSCE_LSD15;
		nNumIter = 1;
		break;
	case D_MOVE_CODE_AUSTRALIA:
		nCodeSeq[0] = D_MVHL_AUSTRALIA_AS_5100_2;
		nCodeSeq[1] = D_MVHL_AS_5100_2;
		nCodeSeq[2] = D_MVHL_AS_5100_2_RAIL;
		nCodeSeq[3] = D_MVHL_AS_5100_7_ROAD;
		nCodeSeq[4] = D_MVHL_AS_5100_7_HEAVY;
		nNumIter = 2;
		break;
	case D_MOVE_CODE_INDIA:
		nCodeSeq[0] = D_MVHL_IRC6_2000;
		nCodeSeq[1] = D_MVHL_IRS_BRIDGE_RULES;
		nNumIter = 2;
		break;
	case D_MOVE_CODE_POLAND:
		nCodeSeq[0] = D_MVHL_PN_85S_10030_ROAD_BRG;
		nCodeSeq[1] = D_MVHL_PN_85S_10030_WALKWALY_BRG;
		nCodeSeq[2] = D_MVHL_MILITARY_LOAD;
		nNumIter = 3;
		break;
	case D_MOVE_CODE_SOUTH_AFRICA:
		nCodeSeq[0] = D_MVHL_SOUTH_AFRICA_TMH7_1981;
		nNumIter = 1;
		break;
	case D_MOVE_CODE_RUSSIA:
		nCodeSeq[0] = D_MVHL_RUS_ROAD_BRG_AND_RAILBRG;
		nCodeSeq[1] = D_MVHL_RUS_ROAD_WALKWAY_AND_PBRG;
		nNumIter = 2;
		break;
	default: ASSERT(0); break;
	}

	for (int i = 0; i < nNumIter; i++)
	{
		CDlgUtil::CobxAddItem(m_cbxMvhlStd, CVehlDB::GetTypeNameAr()[nCodeSeq[i]], nCodeSeq[i]);
	}
	if (m_cbxMvhlStd.GetCount() > 0)
	{
		m_cbxMvhlStd.SetCurSel(0);
	}
	AdjustComboListBox(m_cbxMvhlStd);
}

void CDgnSeisBrgInfoDlg::SetMvhlStdCbxEuroBS()
{
	m_cbxMvhlStd.ResetContent();
	CDlgUtil::CobxAddItem(m_cbxMvhlStd, _LS(IDS_CMD_MVHL_EURO_BS_ROADBRIDGE), 0);//D_MVHL_EURO_BS_ROADBRIDGE); 대화상자에.. define 되어 있어.. ㅠㅠ
	CDlgUtil::CobxAddItem(m_cbxMvhlStd, _LS(IDS_CMD_MVHL_EURO_BS_FOOTBRIDGE), 1);//D_MVHL_EURO_BS_FOOTBRIDGE);
	CDlgUtil::CobxAddItem(m_cbxMvhlStd, _LS(IDS_CMD_MVHL_EURO_BS_ROADBRIDGE_FATIGUE), 2);//D_MVHL_EURO_BS_ROADBRIDGE_FATIGUE);
	CDlgUtil::CobxAddItem(m_cbxMvhlStd, _LS(IDS_CMD_MVHL_EURO_BS_TRAIL_LOAD), 3);//D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD);
	m_cbxMvhlStd.SetCurSel(0);
	AdjustComboListBox(m_cbxMvhlStd);
}

void CDgnSeisBrgInfoDlg::SetMvhlTypeCbx()
{
	int nCurMvCode;
	nCurMvCode = CDlgUtil::CobxGetCurSelItemData(m_cbxMvCode, m_cbxMvCode.GetCurSel());

	int m_nTypeIdx = CDlgUtil::CobxGetCurSelItemData(m_cbxMvhlStd, m_cbxMvhlStd.GetCurSel());
	if (nCurMvCode == D_MOVE_CODE_EURO_BS)
	{
		SetLoadTypeListEuroBS(m_nTypeIdx);
	}
	else
	{
		SetLoadTypeList(m_nTypeIdx);
	}
	AdjustComboListBox(m_cbxMvhlType);
}

void CDgnSeisBrgInfoDlg::SetLoadTypeListEuroBS(int nIndex)
{
	m_cbxMvhlType.ResetContent();

	if (nIndex < 0) return;

	if (nIndex == 0) // EN 1991-2:2003 - RoadBridge
	{
		m_cbxMvhlType.AddString(_T("Load Model 1"));
		m_cbxMvhlType.AddString(_T("Load Model 2"));
		m_cbxMvhlType.AddString(_T("Load Model 3"));
		m_cbxMvhlType.AddString(_T("Load Model 3 (UK NA)"));
		m_cbxMvhlType.AddString(_T("Load Model 4"));
	}
	else if (nIndex == 1) // EN 1991-2:2003 - Footway and FootBridge
	{
		m_cbxMvhlType.AddString(_T("Uniform load (Road bridge footway)"));
		m_cbxMvhlType.AddString(_T("Uniform load (Footbridge)"));
		m_cbxMvhlType.AddString(_T("Concentrated Load"));
		m_cbxMvhlType.AddString(_T("Uniform load (Road bridge footway) UK NA"));
	}
	else if (nIndex == 2) // EN 1991-2:2003 - RoadBridge Fatigue
	{
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 1"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 2 (280)"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 2 (360)"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 2 (630)"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 2 (560)"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 2 (610)"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 3 (One Vehicle)"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 3 (Two Vehicle)"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 4 (200)"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 4 (310)"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 4 (490)"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 4 (390)"));
		m_cbxMvhlType.AddString(_T("Fatigue Load Model 4 (450)"));
	}
	else if (nIndex == 3) // EN 1991-2:2003 - Rail Traffic Load
	{
		m_cbxMvhlType.AddString(_T("Load Model 71"));
		m_cbxMvhlType.AddString(_T("Load Model SW/0"));
		m_cbxMvhlType.AddString(_T("Load Model SW/2"));
		m_cbxMvhlType.AddString(_T("Unloaded Train"));
		m_cbxMvhlType.AddString(_T("HSLM A1 ~ HSLM A10"));
		m_cbxMvhlType.AddString(_T("HSLM B"));
	}
	else
	{
		ASSERT(0);
	}

	m_cbxMvhlType.SetCurSel(0);
}

void CDgnSeisBrgInfoDlg::SetLoadTypeList(int nIndex)
{
	m_cbxMvhlType.ResetContent();

	if (nIndex < 0) return;

	int nCurMvCode = CDlgUtil::CobxGetCurSelItemData(m_cbxMvCode, m_cbxMvCode.GetCurSel());


	CArray<CString, CString> aVehicleType;
	int i = 0;
	while (i < D_MAX_COUNT && CVehlDB::GetTypeListAr()[nIndex][i] != _T(""))
	{
		aVehicleType.Add(CVehlDB::GetTypeListAr()[nIndex][i++]);
	}

	if (aVehicleType.GetSize() == 0) return;

	if (nCurMvCode == D_MOVE_CODE_AASHTO_STAN && nIndex == D_MVHL_TURKEY)
	{
		for (i = 1; i < aVehicleType.GetSize(); i++)
		{
			m_cbxMvhlType.AddString(aVehicleType[i]);
		}

		m_cbxMvhlType.AddString(aVehicleType[0]);
	}
	else
	{
		for (i = 0; i < aVehicleType.GetSize(); i++)
		{
			m_cbxMvhlType.AddString(aVehicleType[i]);
		}
	}

	if (m_cbxMvhlType.GetCount() > 0)
	{
		m_cbxMvhlType.SetCurSel(0);
	}
}

void CDgnSeisBrgInfoDlg::SetMvhlNameCbx()
{
	m_cbxMvhlName.ResetContent();
	m_cbxMvhlName.EnableWindow(FALSE);

	int nCurMvCode = CDlgUtil::CobxGetCurSelItemData(m_cbxMvCode, m_cbxMvCode.GetCurSel());

	CString strMvhlType;
	m_cbxMvhlType.GetWindowText(strMvhlType);

	int nStndCode = CDlgUtil::CobxGetCurSelItemData(m_cbxMvhlStd, m_cbxMvhlStd.GetCurSel());

	if (nCurMvCode == D_MOVE_CODE_INDIA)
	{
		if (strMvhlType == TYPENAME_Broad_Gauge_1676mm)
		{
			m_cbxMvhlName.AddString(SELNAME_Modified_B_G_Loading_1987_1);
			m_cbxMvhlName.AddString(SELNAME_Modified_B_G_Loading_1987_2);
			m_cbxMvhlName.AddString(SELNAME_B_G_Standard_Loading_1926_M_L);
			m_cbxMvhlName.AddString(SELNAME_B_G_Standard_Loading_1926_B_L);
			m_cbxMvhlName.AddString(SELNAME_Revised_B_G_Loading_1975_WG1_WG1);
			m_cbxMvhlName.AddString(SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4A);
			m_cbxMvhlName.AddString(SELNAME_Revised_B_G_Loading_1975_Bo_Bo_Bo_Bo);
			m_cbxMvhlName.AddString(SELNAME_Revised_B_G_Loading_1975_WAM4A);
			m_cbxMvhlName.AddString(SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4);
			m_cbxMvhlName.AddString(SELNAME_Revised_B_G_Loading_1975_WAM4A_WDM2);
			m_cbxMvhlName.AddString(SELNAME_25t_Loading_2008_Combination_1);
			m_cbxMvhlName.AddString(SELNAME_25t_Loading_2008_Combination_2);
			m_cbxMvhlName.AddString(SELNAME_25t_Loading_2008_Combination_3);
			m_cbxMvhlName.AddString(SELNAME_25t_Loading_2008_Combination_4);
			m_cbxMvhlName.AddString(SELNAME_25t_Loading_2008_Combination_5);
			m_cbxMvhlName.AddString(SELNAME_DFC_Loading_Combination_1);
			m_cbxMvhlName.AddString(SELNAME_DFC_Loading_Combination_2);
			m_cbxMvhlName.AddString(SELNAME_DFC_Loading_Combination_3);
			m_cbxMvhlName.AddString(SELNAME_DFC_Loading_Combination_4);
			m_cbxMvhlName.AddString(SELNAME_DFC_Loading_Combination_5);
		}
		else if (strMvhlType == TYPENAME_Metre_Gauge_1000mm)
		{
			m_cbxMvhlName.AddString(SELNAME_2_Co_Co_Locomotives);
			m_cbxMvhlName.AddString(SELNAME_2_Bo_Bo_Locomotives);
			m_cbxMvhlName.AddString(SELNAME_MGML_Loading_of_1929);
			m_cbxMvhlName.AddString(SELNAME_M_L);
			m_cbxMvhlName.AddString(SELNAME_B_L);
			m_cbxMvhlName.AddString(SELNAME_C);
		}
		else if (strMvhlType == TYPENAME_Narrow_Gauge_762mm)
		{
			m_cbxMvhlName.AddString(SELNAME_Class_H_B_B_or_Bo_Bo_Type);
			m_cbxMvhlName.AddString(SELNAME_Class_H_C_C_or_Co_Co_Type);
			m_cbxMvhlName.AddString(SELNAME_Class_H_Steam_Zf_1);
			m_cbxMvhlName.AddString(SELNAME_Class_H_Diesel_Electric);
			m_cbxMvhlName.AddString(SELNAME_Class_A_B_B_or_Bo_Bo_Type);
			m_cbxMvhlName.AddString(SELNAME_Class_A_C_C_or_Co_Co_Type);
			m_cbxMvhlName.AddString(SELNAME_Class_A_Diesel_Mech_Elec);
			m_cbxMvhlName.AddString(SELNAME_Class_A_Diesel_Mech_Elec_Articulated);
			m_cbxMvhlName.AddString(SELNAME_Class_A_DRG_No_CSO_C_873);
			m_cbxMvhlName.AddString(SELNAME_Class_B_B_B_or_Bo_Bo_Type);
			m_cbxMvhlName.AddString(SELNAME_Class_B_Steam_Engine_Tank);
			m_cbxMvhlName.AddString(SELNAME_Class_B_Steam_Engine_Tender);
			m_cbxMvhlName.AddString(SELNAME_Class_B_Diesel_Electric);
		}
		else if (strMvhlType == TYPENAME_Heavy_Mineral_Loadings)
		{
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_1);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_2);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_3);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_4);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_5);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_6);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_7);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_8);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_9);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_10);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_11);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_12);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_13);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_14);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_15);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_16);
			m_cbxMvhlName.AddString(SELNAME_Train_Formation_No_17);
		}
		else if (strMvhlType == TYPENAME_Footbridge_Footpath)
		{
			m_cbxMvhlName.AddString(SELNAME_Footbridge_Footpath);
		}
		else
		{
			// Nothing
		}
	}
	else if (nCurMvCode == D_MOVE_CODE_POLAND)
	{
		if (nStndCode == D_MVHL_PN_85S_10030_ROAD_BRG)
		{
			m_cbxMvhlName.AddString(_T("Class A"));
			m_cbxMvhlName.AddString(_T("Class B"));
			m_cbxMvhlName.AddString(_T("Class C"));
			m_cbxMvhlName.AddString(_T("Class D"));
			m_cbxMvhlName.AddString(_T("Class E"));
		}
		else if (nStndCode == D_MVHL_MILITARY_LOAD)
		{
			// Tracked Vehicle 과 Wheeled Vehicle 이 같음
			m_cbxMvhlName.AddString(_T("Class 4"));
			m_cbxMvhlName.AddString(_T("Class 8"));
			m_cbxMvhlName.AddString(_T("Class 12"));
			m_cbxMvhlName.AddString(_T("Class 16"));
			m_cbxMvhlName.AddString(_T("Class 20"));
			m_cbxMvhlName.AddString(_T("Class 24"));
			m_cbxMvhlName.AddString(_T("Class 30"));
			m_cbxMvhlName.AddString(_T("Class 40"));
			m_cbxMvhlName.AddString(_T("Class 50"));
			m_cbxMvhlName.AddString(_T("Class 60"));
			m_cbxMvhlName.AddString(_T("Class 70"));
			m_cbxMvhlName.AddString(_T("Class 80"));
			m_cbxMvhlName.AddString(_T("Class 90"));
			m_cbxMvhlName.AddString(_T("Class 100"));
			m_cbxMvhlName.AddString(_T("Class 120"));
			m_cbxMvhlName.AddString(_T("Class 150"));
		}
	}
	else if (nCurMvCode == D_MOVE_CODE_RUSSIA)
	{
		if (strMvhlType == _T("Uniform Load"))
		{
			m_cbxMvhlName.AddString(_T("Pedestrian Bridge"));
			m_cbxMvhlName.AddString(_T("Bridge Walkway with AK"));
			m_cbxMvhlName.AddString(_T("Bridge Walkway without AK"));
		}
		else if (strMvhlType == _T("Uniform Load(w/o Other Loads)"))
		{
			m_cbxMvhlName.AddString(_T("Walkway for Railroad Bridge"));
			m_cbxMvhlName.AddString(_T("Walkway for Ballast-track Subway Bridge"));
			m_cbxMvhlName.AddString(_T("Walkway for Other Bridge"));
		}
		else if (strMvhlType == _T("Concentrated Load (w/o other loads)"))
		{
			m_cbxMvhlName.AddString(_T("Walkway for Town Bridge"));
			m_cbxMvhlName.AddString(_T("Walkway for Other Bridge"));
		}
		else
		{

		}
	}
	else if (nCurMvCode == D_MOVE_CODE_EURO_BS)
	{
		int nType = nStndCode - 1;

		if (nType == 0) // RoadBridge
		{
			if (strMvhlType == _T("Load Model 3"))
			{
				m_cbxMvhlName.AddString(_T("600/150"));
				m_cbxMvhlName.AddString(_T("900/150"));
				m_cbxMvhlName.AddString(_T("1200/150/200"));
				m_cbxMvhlName.AddString(_T("1500/150/200"));
				m_cbxMvhlName.AddString(_T("1800/150/200"));
				m_cbxMvhlName.AddString(_T("2400/200"));
				m_cbxMvhlName.AddString(_T("3000/200"));
				m_cbxMvhlName.AddString(_T("3600/200"));
			}
			else if (strMvhlType == _T("Load Model 3 (UK NA)"))
			{
				m_cbxMvhlName.AddString(_T("SV 80"));
				m_cbxMvhlName.AddString(_T("SV 100"));
				m_cbxMvhlName.AddString(_T("SV 196"));
				m_cbxMvhlName.AddString(_T("SOV 250"));
				m_cbxMvhlName.AddString(_T("SOV 350"));
				m_cbxMvhlName.AddString(_T("SOV 450"));
				m_cbxMvhlName.AddString(_T("SOV 600"));
			}
			else
			{
				// Nothing
			}
		}
		else if (nType == 3) // Rail Traffic Load
		{
			if (strMvhlType == _T("HSLM A1 ~ HSLM A10"))
			{
				m_cbxMvhlName.AddString(_T("A1"));
				m_cbxMvhlName.AddString(_T("A2"));
				m_cbxMvhlName.AddString(_T("A3"));
				m_cbxMvhlName.AddString(_T("A4"));
				m_cbxMvhlName.AddString(_T("A5"));
				m_cbxMvhlName.AddString(_T("A6"));
				m_cbxMvhlName.AddString(_T("A7"));
				m_cbxMvhlName.AddString(_T("A8"));
				m_cbxMvhlName.AddString(_T("A9"));
				m_cbxMvhlName.AddString(_T("A10"));
			}
			else
			{
				// Nothing
			}
		}
	}

	if (m_cbxMvhlName.GetCount() > 0)
	{
		m_cbxMvhlName.SetCurSel(0);
	}
	AdjustComboListBox(m_cbxMvhlName);

	EnableCtrl();
}

void CDgnSeisBrgInfoDlg::AdjustComboListBox(CComboBox& Cbx)
/*
콤보박스의 길이를 현재 사용되고 있는 리스트의 길이에
맞추어 변경한다.
*/
{
	CString str;
	CSize   sz;
	int     dx = 0;
	CDC*    pDC = Cbx.GetDC();
	for (int i = 0; i < Cbx.GetCount(); i++)
	{
		Cbx.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	Cbx.ReleaseDC(pDC);

	if (Cbx.GetDroppedWidth() < dx)
	{
		Cbx.SetDroppedWidth(dx);
		ASSERT(Cbx.GetDroppedWidth() == dx);
	}
}