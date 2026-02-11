#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldEarthPressureSeisDlg.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\ExportFunc.h"
#include "..\wg_cmd\CMLoadGroupDlg.h"
#include "..\wg_cmd\CMLdarGrupDefDlg.h"
#include "..\wg_cmd\SoilPropListDlg.h"
#include "..\wg_cmd\SeismicLoadListDlg.h"
#include "..\wg_cmd\CMSeismicEarthPressureProfileDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
//#include "..\wg_base\PL_Parser.h"
#include "..\wg_db\ClosedPathFinder.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_QSort.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_gr\GDisplaySetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldEarthPressureStaticDlg dialog

#define ELEM_FRAME  1
#define ELEM_PLANAR 2

CStldEarthPressureSeisDlg::CStldEarthPressureSeisDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldEarthPressureSeisDlg::IDD)
{

	m_Data.Initialize();
	m_LagrK = 0;


	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitEpseOut(m_Data);

	m_nEPSEMethod = EN_EPSE_METHOD_EQUIVALENT;

	m_nSelectionType = SELECT_GRUP;
	m_bInitProfile = TRUE;
	
	m_OldPospKey = 0;
	m_OldPoslKey = 0;
	m_nOldLayerType = 0;
	m_dOldLayerLevel =0.0;
    m_dOldScaleFactor = 0.0;

	m_aCtrlOption.RemoveAll();
	m_aCtrlOption.Add(IDC_TM_EP_TYPE_ADD_RDO);
	m_aCtrlOption.Add(IDC_TM_EP_TYPE_DEL_RDO);

	m_aCtrlEPSLMethod.RemoveAll();
	m_aCtrlEPSLMethod.Add(IDC_TM_EP_TYPE_GRP);
	m_aCtrlEPSLMethod.Add(IDC_TM_EP_TYPE_EQU_RDO);
	m_aCtrlEPSLMethod.Add(IDC_TM_EP_TYPE_RDM_RDO);

	m_aCtrlEPSLMethodRdo.RemoveAll();
	m_aCtrlEPSLMethodRdo.Add(IDC_TM_EP_TYPE_EQU_RDO);
	m_aCtrlEPSLMethodRdo.Add(IDC_TM_EP_TYPE_RDM_RDO);

	m_aCtrlLayerParam.RemoveAll();
	m_aCtrlLayerParam.Add(IDC_TM_EP_LAYER_GRP);
	m_aCtrlLayerParam.Add(IDC_TM_EP_LAYER_SINGLE_RDO);
	m_aCtrlLayerParam.Add(IDC_TM_EP_LAYER_DOUBLE_RDO);
	m_aCtrlLayerParam.Add(IDC_TM_EP_LAYER_USER_RDO);
	m_aCtrlLayerParam.Add(IDC_WG_TREEMENU_STATIC6);
	m_aCtrlLayerParam.Add(IDC_TM_2ND_LAYER_EDIT);
	m_aCtrlLayerParam.Add(IDC_TM_2ND_LAYER_UNIT);

	m_aCtrlLayerMethod.RemoveAll();
	m_aCtrlLayerMethod.Add(IDC_TM_EP_LAYER_SINGLE_RDO);
	m_aCtrlLayerMethod.Add(IDC_TM_EP_LAYER_DOUBLE_RDO);
	m_aCtrlLayerMethod.Add(IDC_TM_EP_LAYER_USER_RDO);

	m_aCtrlDoubleCosine.RemoveAll();
	m_aCtrlDoubleCosine.Add(IDC_WG_TREEMENU_STATIC6);
	m_aCtrlDoubleCosine.Add(IDC_TM_2ND_LAYER_EDIT);
	m_aCtrlDoubleCosine.Add(IDC_TM_2ND_LAYER_UNIT);

	// Loading Type
	m_aCtrlLoadingType.RemoveAll();
	m_aCtrlLoadingType.Add(IDC_TM_EP_LOADINGTYPE_CMB);
	m_aCtrlLoadingType.Add(IDC_TM_EP_LOADINGWIDTH_EDT);
	m_aCtrlLoadingType.Add(IDC_TM_EP_LOADINGWIDTH_UNIT);
	m_aCtrlLoadingType.Add(IDC_TM_EP_LOADINGTYPE_GRP);
	m_aCtrlLoadingType.Add(IDC_TM_EP_LOADINGTYPE_STC);
	m_aCtrlLoadingType.Add(IDC_TM_EP_LOADINGWIDTH_STC);

	m_aCtrlAddDelete.RemoveAll();
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC0);
	m_aCtrlAddDelete.Add(IDC_TM_DIRECTION_COMBO);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC1);
	m_aCtrlAddDelete.Add(IDC_TM_ANGLE_EDT);
	//m_aCtrlAddDelete.Add(IDC_TM_ANGLE_SPIN);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC2);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC3);
	m_aCtrlAddDelete.Add(IDC_TM_PLANE_PT1_EDIT);
	m_aCtrlAddDelete.Add(IDC_TM_PLANE_PT1_UNIT);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC4);
	m_aCtrlAddDelete.Add(IDC_TM_SCALEFACTOR);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC5);
	m_aCtrlAddDelete.Add(IDC_TM_CODE_NAME);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_POSL_STC);
	m_aCtrlAddDelete.Add(IDC_TM_POSL_NAME_CMB);
	m_aCtrlAddDelete.Add(IDC_TM_POSL_BTN);
	m_aCtrlAddDelete.Add(IDC_TM_EP_LAYER_GRP);
	m_aCtrlAddDelete.Add(IDC_TM_EP_LAYER_SINGLE_RDO);
	m_aCtrlAddDelete.Add(IDC_TM_EP_LAYER_DOUBLE_RDO);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC6);
	m_aCtrlAddDelete.Add(IDC_TM_2ND_LAYER_EDIT);
	m_aCtrlAddDelete.Add(IDC_TM_2ND_LAYER_UNIT);
	m_aCtrlAddDelete.Add(IDC_TM_EP_LAYER_USER_RDO);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_POSP_STC);
	m_aCtrlAddDelete.Add(IDC_TM_POSP_NAME_CMB);
	m_aCtrlAddDelete.Add(IDC_TM_POSP_BTN);

	m_aCtrlLoadingGrHide.RemoveAll();
	m_aCtrlLoadingGrHide.Add(IDC_TM_TARGET_STATIC);
	m_aCtrlLoadingGrHide.Add(IDC_TM_TARGET_RDO1);
	m_aCtrlLoadingGrHide.Add(IDC_TM_TARGET_RDO2);
	m_aCtrlLoadingGrHide.Add(IDC_TM_LAGR_NAME_STATIC);
	m_aCtrlLoadingGrHide.Add(IDC_TM_LAGR_NAME);
	m_aCtrlLoadingGrHide.Add(IDC_TM_LAGR_BTN);

	m_aCtrlLoadingGrMove.RemoveAll();
	m_aCtrlLoadingGrMove.Add(IDC_TM_LAPL_ELEM_TYPE_GRB);
	m_aCtrlLoadingGrMove.Add(IDC_TM_LAPL_ELEM_TYPE1_RDO);
	m_aCtrlLoadingGrMove.Add(IDC_TM_LAPL_ELEM_TYPE2_RDO);
	m_aCtrlLoadingGrMove.Add(IDC_TM_ELEM_DEFINE_STATIC);
	m_aCtrlLoadingGrMove.Add(IDC_TM_ELEM_DEFINE);

	m_aLastCtrlData.RemoveAll();
	m_aLastCtrlData.Add(IDC_TM_BTN_PROFILE);
	m_aLastCtrlData.Add(IDC_TM_EXECUTE);
	m_aLastCtrlData.Add(IDC_TM_CLOSE);

// 	m_aCtrlData.RemoveAll();
// 	m_aCtrlData.Add(IDC_TM_EP_LAYER_GRP);
// 	m_aCtrlData.Add(IDC_TM_EP_LAYER_SINGLE_RDO);
// 	m_aCtrlData.Add(IDC_TM_EP_LAYER_DOUBLE_RDO);
// 	m_aCtrlData.Add(IDC_WG_TREEMENU_STATIC6);
// 	m_aCtrlData.Add(IDC_TM_2ND_LAYER_EDIT);
// 	m_aCtrlData.Add(IDC_TM_2ND_LAYER_UNIT);
// 	m_aCtrlData.Add(IDC_TM_EP_LAYER_USER_RDO);
// 	m_aCtrlData.Add(IDC_TM_EP_OPTION_GRP4);
// 	m_aCtrlData.Add(IDC_WG_TREEMENU_POSP_STC);
// 	m_aCtrlData.Add(IDC_TM_POSP_NAME_CMB);
// 	m_aCtrlData.Add(IDC_TM_POSP_BTN);
// 	m_aCtrlData.Add(IDC_TM_TARGET_STATIC);
// 	m_aCtrlData.Add(IDC_TM_TARGET_RDO1);
// 	m_aCtrlData.Add(IDC_TM_TARGET_RDO2);
// 	m_aCtrlData.Add(IDC_TM_LAGR_NAME_STATIC);
// 	m_aCtrlData.Add(IDC_TM_LAGR_NAME);
// 	m_aCtrlData.Add(IDC_TM_LAGR_BTN);
// 	m_aCtrlData.Add(IDC_TM_LAPL_ELEM_TYPE_GRB);
// 	m_aCtrlData.Add(IDC_TM_LAPL_ELEM_TYPE1_RDO);
// 	m_aCtrlData.Add(IDC_TM_LAPL_ELEM_TYPE2_RDO);
// 	m_aCtrlData.Add(IDC_TM_ELEM_DEFINE_STATIC);
// 	m_aCtrlData.Add(IDC_TM_ELEM_DEFINE);
// 	m_aCtrlData.Add(IDC_TM_BTN_PROFILE);
// 	m_aCtrlData.Add(IDC_TM_EXECUTE);
// 	m_aCtrlData.Add(IDC_TM_CLOSE);

#if defined(_CIVIL)
	m_bCivil = TRUE;
#else
	m_bCivil = FALSE;
#endif
}						 

CStldEarthPressureSeisDlg::~CStldEarthPressureSeisDlg()
{
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	dspOpt.m_Load.nTypeLaplLagr = 0;
	dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
	ExportDisplaySetting::SET(&dspOpt);
}

void CStldEarthPressureSeisDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TM_LOADCASE, m_cobxLoadCase);
	
	DDX_Control(pDX, IDC_TM_EP_LOADINGTYPE_CMB, m_cobxLodingType);
	DDX_Control(pDX, IDC_TM_DIRECTION_COMBO, m_cobxDirection);
	DDX_Control(pDX, IDC_TM_POSL_NAME_CMB, m_cobxSeisPsol);
	DDX_Control(pDX, IDC_TM_POSP_NAME_CMB, m_cobxSoilProp);
	DDX_Control(pDX, IDC_TM_CODE_NAME, m_cobxCodeName);
	DDX_Control(pDX, IDC_TM_LAGR_NAME, m_cobxLagr);
	DDX_Control(pDX, IDC_TM_ELEM_DEFINE, m_edElemList);
	DDX_Control(pDX, IDC_TM_ANGLE_EDT, m_edtAngle);
	DDX_Control(pDX, IDC_TM_SCALEFACTOR, m_edtScaleFactor);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_EDIT, m_wndCoordXYZ);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_UNIT, m_wndCoordUnit);
	//DDX_Radio(pDX, IDC_TM_EP_TYPE_EQU_RDO, m_nEPSEMethod);
	DDX_Control(pDX, IDC_TM_2ND_LAYER_EDIT, m_edt2ndLayer);
	DDX_Control(pDX, IDC_TM_2ND_LAYER_UNIT, m_2ndLayerUnit);

	DDX_Control(pDX, IDC_TM_EP_LOADINGWIDTH_EDT, m_edtWidth);
	DDX_Control(pDX, IDC_TM_EP_LOADINGWIDTH_UNIT, m_untWidth);
	
// 	DDX_Control(pDX, IDC_TM_WATER_EDIT, m_edtWaterLevel);
// 	DDX_Control(pDX, IDC_TM_WATER_UNIT, m_WaterLevelUnit);
}

BEGIN_MESSAGE_MAP(CStldEarthPressureSeisDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_LOADCASE_BTN, OnCmdBtnLoadCase)
	ON_BN_CLICKED(IDC_TM_TARGET_RDO1, OnCmdTargetChanged)
	ON_BN_CLICKED(IDC_TM_TARGET_RDO2, OnCmdTargetChanged)
	ON_BN_CLICKED(IDC_TM_LAGR_BTN, OnCmdBtnLoadAreaGroup)
	ON_BN_CLICKED(IDC_TM_POSL_BTN, OnCmdSeismicLoadBtn)
	ON_BN_CLICKED(IDC_TM_POSP_BTN, OnCmdSoilPropBtn)
	ON_BN_CLICKED(IDC_TM_EP_LAYER_SINGLE_RDO, OnCmdLayerParamRdo)
	ON_BN_CLICKED(IDC_TM_EP_LAYER_DOUBLE_RDO, OnCmdLayerParamRdo)
	ON_BN_CLICKED(IDC_TM_EP_LAYER_USER_RDO, OnCmdLayerParamRdo)
	ON_BN_CLICKED(IDC_TM_EP_TYPE_ADD_RDO, OnCmdOptionRdo)
	ON_BN_CLICKED(IDC_TM_EP_TYPE_DEL_RDO, OnCmdOptionRdo)
	ON_CBN_SELCHANGE(IDC_TM_POSL_NAME_CMB, OnSelChangePOSL)
	ON_CBN_SELCHANGE(IDC_TM_LAGR_NAME, OnSelChangeLagr)
	ON_CBN_SELCHANGE(IDC_TM_EP_LOADINGTYPE_CMB, OnSelChangeLoadingType)
	ON_CBN_SELCHANGE(IDC_TM_DIRECTION_COMBO, OnSelChangeDir)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ANGLE_SPIN, OnDeltaposCmdAngle)     //Span Num
	ON_BN_CLICKED(IDC_TM_BTN_PROFILE, OnCmdPressureProfile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldEarthPressureStaticDlg message handlers

void CStldEarthPressureSeisDlg::OnTmExecute()
{
	if (!Dlg2Data())
		return;

	int nOption = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);

	CArray<T_EPSE_D, T_EPSE_D&> DataSet;
	if (GetEpseAry(DataSet))
	{
		if (!CheckEpse(DataSet, TRUE))
			return;

		CArray<T_EPSE_K, T_EPSE_K> aEpseK;
		if (CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetEpskKeyListByData(DataSet, aEpseK) == 0) { ASSERT(0); return; }
		
		BOOL bResult = FALSE;
		if (nOption==0) // Add/replace
		{
			bResult = CDBDoc::GetDocPoint()->m_pDataCtrl->AddEpse(aEpseK, DataSet);
		}
		else
		{
			for (int i=aEpseK.GetSize()-1; i>-1; i--)
			{
				if (aEpseK[i] == 0) aEpseK.RemoveAt(i);
			}
			bResult = CDBDoc::GetDocPoint()->m_pDataCtrl->DelEpse(aEpseK);
		}
		
		if(bResult) CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	}

}

void CStldEarthPressureSeisDlg::OnTmClose()
{
	CloseDlg();
}


void CStldEarthPressureSeisDlg::OnCmdOptionRdo()
{
	int nOption = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlAddDelete, nOption==0);
	if(m_bCivil)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLoadingType, nOption == 0);
	}
	
	if (nOption == 0) // Add Mode
	{
		OnCmdTargetChanged();
		OnSelChangeDir();
		OnSelChangeLagr();
		OnSelChangePOSL();
	}
}
BOOL CStldEarthPressureSeisDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	//
	CDlgUtil::CtrlShowHide(this, m_aCtrlLoadingType, m_bCivil);
	CDlgUtil::CtrlShowHide     (this, m_aCtrlEPSLMethod, !m_bCivil);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlEPSLMethod, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aCtrlLoadingGrHide, !m_bCivil);
	GetDlgItem(IDC_TM_EP_LOADINGWIDTH_STC)->EnableWindow(FALSE/*nLoadingType == 0*/);
	GetDlgItem(IDC_TM_EP_LOADINGWIDTH_EDT)->EnableWindow(FALSE/*nLoadingType == 0*/);
	GetDlgItem(IDC_TM_EP_LOADINGWIDTH_UNIT)->EnableWindow(FALSE/*nLoadingType == 0*/);

	AlignControl();
	//

	CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, 0);
	
	InitCobxLoadingType();
	InitCobxDirection();
	InitCobxCodeName();

	m_edElemList.Connect(SC_ID_ELEM, &m_edElemList);
	m_cobxLoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
	m_edtAngle.SetRange(-360, 360);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);

	m_wndCoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCoordXYZ.SetAttUcsPos();
	m_wndCoordXYZ.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndCoordXYZ.SetNextLink(NULL);

	m_2ndLayerUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edt2ndLayer.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);

	GetElementType();

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	Data2Dlg();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStldEarthPressureSeisDlg::GetHolderRect(CRect* pRect)
{
	return FALSE;
}

void CStldEarthPressureSeisDlg::GetMoveCtrlHolderID(CArray<UINT, UINT>& caCtrls, UINT& uiHolderID)
{
	caCtrls.RemoveAll();
}

void CStldEarthPressureSeisDlg::OnCmdBtnLoadAreaGroup()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMLdarGrupDefDlg::IDD);
}

void CStldEarthPressureSeisDlg::OnCmdBtnLoadCase()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));
}

void CStldEarthPressureSeisDlg::InitCobxLoadingType()
{
	m_cobxLodingType.ResetContent();
	m_cobxLodingType.SetItemData(m_cobxLodingType.AddString(_LS(IDS_TM_EARTHP_Frame)) , 0);
	m_cobxLodingType.SetItemData(m_cobxLodingType.AddString(_LS(IDS_TM_EARTHP_Planar)), 1);
	m_cobxLodingType.SetCurSel(1); //Planar
}

void CStldEarthPressureSeisDlg::InitCobxDirection(BOOL bFrame)
{
	m_cobxDirection.ResetContent();
	if(bFrame)
	{
		m_cobxDirection.SetItemData(m_cobxDirection.AddString(_T("X-Y")), 0);
		m_cobxDirection.SetCurSel(0);
		return;
	}

	int nIndex;
	CStringArray strItem;
	strItem.Add(_T("X-Y"));
	strItem.Add(_T("Normal"));
	int nNum = strItem.GetSize();
	for (int i = 0; i < nNum; i++)
	{
		nIndex = m_cobxDirection.AddString(strItem[i]);
		m_cobxDirection.SetItemData(nIndex, i + 1);
	}
	m_cobxDirection.SetCurSel(0);
}

void CStldEarthPressureSeisDlg::InitCobxCodeName()
{
	m_cobxCodeName.ResetContent();

	if (m_bCivil)
	{
		m_cobxCodeName.SetItemData(m_cobxCodeName.AddString(_T("KDS(17-10-00:2018)")), KDS_E_2018 ); // D_SPFC_CODE_KSCE2019
	}
	else
	{
		m_cobxCodeName.SetItemData(m_cobxCodeName.AddString(_T("KDS(41-17-00:2019)")), KDS_E_2019 ); // D_SPFC_CODE_KDS2019
	}
	m_cobxCodeName.SetCurSel(0);
}

void CStldEarthPressureSeisDlg::OnDeltaposCmdAngle(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_TM_ANGLE_EDT, -180, 180, pNMHDR, pResult);

	*pResult = 0;
}

void CStldEarthPressureSeisDlg::OnCmdTargetChanged()
{
	m_nSelectionType = GetSelectionType();
	if (m_nSelectionType == SELECT_GRUP) //Group
	{
		GetDlgItem(IDC_TM_LAGR_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAGR_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE_GRB)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_ELEM_DEFINE)->EnableWindow(FALSE);
	}
	else if (m_nSelectionType == SELECT_ELEM) //Elements
	{
		GetDlgItem(IDC_TM_LAGR_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAGR_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE_GRB)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_ELEM_DEFINE)->EnableWindow(TRUE);
	}
	else ASSERT(0);

	OnSelChangeLagr();
}

int CStldEarthPressureSeisDlg::GetSelectionType()
{
	int nSelType = SELECT_GRUP;
	if (((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->GetCheck())
	{
		nSelType = SELECT_ELEM;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(FALSE);
	}
	return nSelType;
}

BOOL CStldEarthPressureSeisDlg::Data2Dlg(BOOL bWarning)
{
	//m_cobxLdgr.ChangeSelect(m_Data.KeyLdgr);
	if (m_Data.KeyStld)
		m_cobxLoadCase.ChangeSelect(D_LOADCASE_STATIC, m_Data.KeyStld);

	m_cobxLodingType.SetCurSel(m_Data.nLoadingType);
	m_edtWidth.SetEditUnit(m_Data.dWidth);

	if (m_Data.nDirection == 2) m_cobxDirection.SetCurSel(1);
	else                       m_cobxDirection.SetCurSel(0);

	CString strPos;
	strPos.Format(_T("%g, %g, %g"), m_Data.dInnerPnt[0], m_Data.dInnerPnt[1], m_Data.dInnerPnt[2]);
	m_wndCoordXYZ.SetWindowText(strPos);

	m_edtAngle.SetValue(m_Data.dAngle);
	m_edtScaleFactor.SetEditUnit(m_Data.dScaleFactor);
	if (m_Data.KeyPosp > 0)
		m_cobxSoilProp.ChangeSelect(m_Data.KeyPosp);
	if (m_Data.KeyPosl > 0)
		m_cobxSeisPsol.ChangeSelect(m_Data.KeyPosl);

// 	else
// 		m_Data.dWaterLevel;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLayerMethod, m_Data.nLayerMethod);
	m_edt2ndLayer.SetEditUnit(m_Data.dLayerLevel);

	if (m_Data.nSelection == SELECT_GRUP)
	{
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(FALSE);
		m_cobxLagr.ChangeSelect(m_LagrK);
	}
	else if (m_Data.nSelection == SELECT_ELEM)
	{
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(TRUE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SelectElem(NULL, m_Data.arKeysDefineElem, FALSE);
	}
	m_bInitProfile = TRUE;

	OnCmdTargetChanged();
	OnSelChangeDir();
	OnSelChangeLagr();
	OnSelChangePOSL();

	UpdateData(FALSE);
	return TRUE;
}

BOOL CStldEarthPressureSeisDlg::Dlg2Data(BOOL bWarning)
{
	UpdateData();
	//m_cobxLdgr.GetSelectedLdgr(m_Data.KeyLdgr);
	unsigned int nLoadCaseType;
	m_cobxLoadCase.GetSelectedLoad(nLoadCaseType, m_Data.KeyStld);
	if (nLoadCaseType != D_LOADCASE_STATIC || m_Data.KeyStld <= 0)
	{
		AfxMessageBox(_LS(IDS_TM_EARTHP_NO_SELECTED_STLD));
		return FALSE;
	}

	m_Data.nCodeType = m_cobxCodeName.GetItemData(m_cobxCodeName.GetCurSel());

	CString strPos;
	MyParser Parser;

	m_Data.nLoadingType = m_cobxLodingType.GetItemData(m_cobxLodingType.GetCurSel());
	m_Data.dWidth = m_edtWidth.GetEditValue();

	if(m_Data.nLoadingType == 0)
	{
		if (m_Data.dWidth <= 0.0)
		{
			AfxMessageBox(_LS(IDS_TM_EARTHP_LOADINGWIDTH_ZERO));
			return FALSE;
		}	
	}

	m_Data.nDirection = GetDirection();
	if (m_Data.nDirection == 1)
		m_edtAngle.GetEditValue(m_Data.dAngle);
	else if (m_Data.nDirection == 2)
	{
		m_wndCoordXYZ.GetWindowText(strPos);
		if (!Parser.ParsingPositionArg(strPos)) return FALSE;
		if (!Parser.GetPositionData(m_Data.dInnerPnt[0], m_Data.dInnerPnt[1], m_Data.dInnerPnt[2])) return FALSE;
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}

    m_Data.dScaleFactor = m_edtScaleFactor.GetEditValue();

	//m_Data.nEPSEMethod = m_nEPSEMethod;
	m_cobxSeisPsol.GetSelectedPosl(m_Data.KeyPosl);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLayerMethod, m_Data.nLayerMethod);
	m_Data.dLayerLevel = m_edt2ndLayer.GetEditValue();
	m_cobxSoilProp.GetSelectedPosp(m_Data.KeyPosp);

	if (((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->GetCheck())
	{
		m_Data.nSelection = SELECT_GRUP;
		m_Data.arKeysDefineElem.RemoveAll();
		m_cobxLagr.GetSelectedLagr(m_LagrK);
	}
	else if (((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->GetCheck())
	{
		m_Data.nSelection = SELECT_ELEM;
		m_LagrK = 0;
		// [PMS 5011-6] 요소 많이 선택 시 문자열->요소번호 파싱하다 프로그램 다운
		CArray<T_ELEM_K, T_ELEM_K> aElem;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		pDoc->m_pViewCtrl->GetAllSelectedElem(aElem);
		if (!aElem.IsEmpty())
		{
			int i, nSize;
			nSize = aElem.GetSize();
			m_Data.arKeysDefineElem.SetSize(nSize);
			for(i=0; i < nSize; i++)
			{
				m_Data.arKeysDefineElem[i] = aElem[i];
			}
		}
	}

	if (m_Data.nLayerMethod == EN_LAYER_METHOD_DOUBLE)
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();

		T_POSP_D PospD;
		if (!pDoc->m_pAttrCtrl->GetPosp(m_Data.KeyPosp, PospD)) { ASSERT(0); PospD.Initialize(); }
		if (m_Data.dLayerLevel > PospD.dGroundLevel || m_Data.dLayerLevel < PospD.dBedrockLevel)
		{
			AfxMessageBox(_LS(IDS_TM_ERROR_EARTH_PRESSURE_SEIS_2ND_LEVEL));
			return FALSE;
		}
	}

	// Profile 데이터를 새로 만들어야 하기 때문..
	if (m_Data.KeyPosl != m_OldPoslKey) m_bInitProfile = TRUE;
	else if(m_Data.KeyPosp != m_OldPospKey) m_bInitProfile = TRUE;
	else if(m_Data.nLayerMethod != m_nOldLayerType) m_bInitProfile = TRUE;
	else if(m_Data.dLayerLevel != m_dOldLayerLevel) m_bInitProfile = TRUE;
    else if ( m_Data.dScaleFactor != m_dOldScaleFactor ) m_bInitProfile = TRUE;
    
    m_OldPoslKey = m_Data.KeyPosl;
	m_OldPospKey = m_Data.KeyPosp;
	m_nOldLayerType = m_Data.nLayerMethod;
	m_dOldLayerLevel = m_Data.dLayerLevel;
    m_dOldScaleFactor = m_Data.dScaleFactor;

	return TRUE;
}

void CStldEarthPressureSeisDlg::OnSelChangeLagr()
{
	GDisplaySetting dspOpt;
	m_nSelectionType = GetSelectionType();
	if (m_nSelectionType == SELECT_GRUP) //Group
	{
		T_LAGR_K LagrK;
		m_cobxLagr.GetSelectedLagr(LagrK);
		ExportDisplaySetting::GET(&dspOpt);
		dspOpt.m_Load.nTypeLaplLagr = 2;
		dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
		dspOpt.m_Load.m_LoadingAreaKeys.Add((unsigned int)LagrK);
		ExportDisplaySetting::SET(&dspOpt);
	}
	else if (m_nSelectionType == SELECT_ELEM) //Elements
	{
		ExportDisplaySetting::GET(&dspOpt);
		dspOpt.m_Load.nTypeLaplLagr = 0;
		dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
		ExportDisplaySetting::SET(&dspOpt);
	}
}

void CStldEarthPressureSeisDlg::OnSelChangeLoadingType()
{
	int nLoadingType = m_cobxLodingType.GetItemData(m_cobxLodingType.GetCurSel()); // 0: Frame
	//
	InitCobxDirection(nLoadingType == 0);
	//
	GetDlgItem(IDC_TM_EP_LOADINGWIDTH_STC)->EnableWindow(nLoadingType==0);
	GetDlgItem(IDC_TM_EP_LOADINGWIDTH_EDT)->EnableWindow(nLoadingType==0);
	GetDlgItem(IDC_TM_EP_LOADINGWIDTH_UNIT)->EnableWindow(nLoadingType==0);

	// 
	((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO))->SetCheck(TRUE); // Elem
	((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO))->SetCheck(FALSE);
	GetDlgItem(IDC_TM_LAPL_ELEM_TYPE_GRB)->EnableWindow(nLoadingType !=0);
	GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO)->EnableWindow(nLoadingType!=0);
	GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO)->EnableWindow(nLoadingType!=0);
	//
	CString strName;
	if(nLoadingType == 0) strName = _T("Loading Element :"); 
	else                  strName = _T("Element Defining Loading Area :");
	GetDlgItem(IDC_TM_ELEM_DEFINE_STATIC)->SetWindowText(strName);
	//
	SelChangeDir();	
	//
}

void CStldEarthPressureSeisDlg::OnSelChangeDir()
{
	SelChangeDir();
}
void CStldEarthPressureSeisDlg::SelChangeDir()
{
	if (GetDirection() == 2) //Normal
	{
		GetDlgItem(IDC_TM_ANGLE_EDT)->EnableWindow(FALSE);
		//GetDlgItem(IDC_TM_ANGLE_SPIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_PLANE_PT1_EDIT)->EnableWindow(TRUE);
		m_wndCoordXYZ.SetFocus();
	}
	else //X-Y
	{
		GetDlgItem(IDC_TM_ANGLE_EDT)->EnableWindow(TRUE);
		//GetDlgItem(IDC_TM_ANGLE_SPIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_PLANE_PT1_EDIT)->EnableWindow(FALSE);
	}
}

int CStldEarthPressureSeisDlg::GetDirection()
{
	int nSel = m_cobxDirection.GetCurSel();
	if (nSel == 0)
		return 1; //X-Y
	else if (nSel == 1)
		return 2; //Normal

	m_cobxDirection.SetCurSel(0);
	return 1;
}
void CStldEarthPressureSeisDlg::OnSelChangePOSL()
{
	m_cobxSeisPsol.GetSelectedPosl(m_Data.KeyPosl);
	T_POSL_D PoslD;
	if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetPosl(m_Data.KeyPosl, PoslD)) PoslD.Initialize();
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlEPSLMethodRdo, PoslD.KDS2019.nEPSEMethod);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlLayerParam, PoslD.KDS2019.nEPSEMethod == EN_EPSE_METHOD_EQUIVALENT ? FALSE : TRUE);
	m_nEPSEMethod = PoslD.KDS2019.nEPSEMethod;

	OnCmdLayerParamRdo();
}

void CStldEarthPressureSeisDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
	case D_UPDATE_EXE_START:
	case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_UNIT:
		OnUnitChange();
		break;
	default:
		break;
	}
}

void CStldEarthPressureSeisDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_POSL_K CurPoslK = NULL;
	if(m_Data.KeyPosp !=0) m_cobxSeisPsol.GetSelectedPosl(CurPoslK);
	T_POSP_K CurPospK = NULL;
	if(m_Data.KeyPosl !=0) m_cobxSoilProp.GetSelectedPosp(CurPospK);

	T_POSP_K PospK = NULL;
	T_POSP_D PospD;
	T_POSL_K PoslK = NULL;
	T_POSL_D PoslD;

	BOOL bNeedUpdateProfile = FALSE;
	BOOL bModPosl = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_POSP_ADD):
			{
				pViewBuff->GetPosp(nKey, PospK, PospD);
				if(PospK != CurPospK) continue;
				bNeedUpdateProfile = TRUE;
			}
			break;
		case(UR_POSL_ADD):
			{
				bModPosl = TRUE;

				pViewBuff->GetPosl(nKey, PoslK, PoslD);
				if(PoslK != CurPoslK) continue;
				bNeedUpdateProfile = TRUE;
			}
			break;
		case(UR_POSL_MFD):
		case(UR_POSL_DEL):
			{
				bModPosl = TRUE;
			}
			break;
		default:
			break;
		}
	} // end of while

	if(bModPosl)
	{
		OnSelChangePOSL();
	}
	if(bNeedUpdateProfile)
	{
		m_bInitProfile = TRUE;
	}
}

void CStldEarthPressureSeisDlg::OnUnitChange()
{
	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitEpsePrevious(m_Data);
}

BOOL CStldEarthPressureSeisDlg::CheckEpse(CArray<T_EPSE_D, T_EPSE_D&>& rDataSet, BOOL bMsg)
{
	if (m_Data.nSelection != SELECT_GRUP && GetElementType() == ELEM_PLANAR)
	{
		for (int i = 0; i < rDataSet.GetSize(); i++)
		{
			CMap<T_ELEM_K, T_ELEM_K, T_PRES_D, T_PRES_D&> mapLoad;
			if (!CDBDoc::GetDocPoint()->m_pAttrCtrl2->CalcEarthPressure_Face(rDataSet[i], mapLoad, TRUE))
				return FALSE;
		}
	}
	else
	{
		if(m_Data.nLoadingType == 0)
		{
			CMap<T_ELEM_K, T_ELEM_K, T_BMLD_D, T_BMLD_D&> mapLoadX;
			CMap<T_ELEM_K, T_ELEM_K, T_BMLD_D, T_BMLD_D&> mapLoadY;
			CMap<T_ELEM_K, T_ELEM_K, T_BMLD_D, T_BMLD_D&> mapLoadZ;
			if (!CDBDoc::GetDocPoint()->m_pAttrCtrl2->CalcEarthPressure_Beam(rDataSet, mapLoadX, mapLoadY, mapLoadZ))
				return FALSE;
		}
		else
		{
			CMap<T_NODE_K, T_NODE_K, T_CNLD_D, T_CNLD_D&> mapLoad;
			if (!CDBDoc::GetDocPoint()->m_pAttrCtrl2->CalcEarthPressure_Nodal(rDataSet, mapLoad, NULL, TRUE))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CStldEarthPressureSeisDlg::GetEpseAry(CArray<T_EPSE_D, T_EPSE_D&>& rDataSet)
{
	int nSelection = m_Data.nSelection;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_LAPL_K, T_LAPL_K> KeyLapl;
	int i, nSize;

	int nElemType = GetElementType();
	if (m_Data.nSelection == SELECT_GRUP)
	{
		CArray<T_LAGR_K, T_LAGR_K> KeyLagr;
		KeyLagr.SetSize(1);
		KeyLagr[0] = m_LagrK;
		pDoc->m_pAttrCtrl2->GetKeyListLaplByLagr(KeyLagr, KeyLapl);

		nSize = KeyLapl.GetSize();
		rDataSet.SetSize(nSize);
		for(int i=0; i < nSize; i++)
		{
			m_Data.KeyLapl = KeyLapl[i];
			rDataSet[i] = m_Data;
		}
		if (nSize <= 0)
		{
			AfxMessageBox(_LS(IDS_TM_EARTHP_NO_LDPL));
			return FALSE;
		}
	}
	else
	{
		if (nElemType == ELEM_PLANAR)
		{
			T_ELEM_D ElemD; ElemD.Initialize();
			for (int i = m_Data.arKeysDefineElem.GetSize() - 1; i >= 0; i--)
			{
				if (!pDoc->m_pAttrCtrl->GetElem(m_Data.arKeysDefineElem[i], ElemD)) { ASSERT(0); return FALSE; }
				if (!pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp) && !pDoc->m_pAttrCtrl->IsPlstrs(ElemD.eltyp)) m_Data.arKeysDefineElem.RemoveAt(i);
			}

			if (m_Data.arKeysDefineElem.GetSize() < 1) return FALSE;

			T_EPSE_D EpseD;
			rDataSet.SetSize(m_Data.arKeysDefineElem.GetSize());
			for(int i=0; i < m_Data.arKeysDefineElem.GetSize(); i++)
			{
				EpseD = m_Data;
				EpseD.arKeysDefineElem.RemoveAll();
				EpseD.arKeysDefineElem.Add(m_Data.arKeysDefineElem[i]);
				rDataSet.SetAt(i, EpseD);
			}
		}
		else
		{
			if(m_Data.nLoadingType == 0)
			{
				CArray<T_MEMB_D, T_MEMB_D&> aDataMemb;
				GetMemberData(m_Data.arKeysDefineElem, aDataMemb);

				nSize = aDataMemb.GetSize();
				if(nSize==0) { ASSERT(0); return FALSE; }

				int nTotalSize = 0;
				for (int i = 0; i < nSize; i++)
				{
					nTotalSize += aDataMemb[i].aElemList.GetSize();
				}

				T_EPSE_D EpseD;
				rDataSet.SetSize(nTotalSize);
				int nIdx = -1;
				for (int i = 0; i < nSize; i++)
				{
					for (int j = 0; j < aDataMemb[i].aElemList.GetSize(); j++)
					{
						nIdx++;
						EpseD = m_Data;
						EpseD.arKeysDefineElem.RemoveAll();
						EpseD.arKeysDefineElem.Add(aDataMemb[i].aElemList[j]);
						rDataSet.SetAt(nIdx, EpseD);
					}
				}
			}
			else
 			{
				CArray<T_CPF_CYCLE_PATH3, T_CPF_CYCLE_PATH3&> aClosedPath;
				if (!IClosedPathFinder::Find4AutoGen(m_Data.arKeysDefineElem, aClosedPath) || aClosedPath.GetSize() <= 0)
				{
					ASSERT(0);
					return FALSE;
				}

				nSize = aClosedPath.GetSize();
				rDataSet.SetSize(nSize);
				for(int i=0; i < nSize; i++)
				{
					rDataSet[i] = m_Data;
					rDataSet[i].arKeysDefineElem.Copy(aClosedPath[i].aElemId);
					rDataSet[i].arKeysDefineNode.Copy(aClosedPath[i].aNodeId);
				}
 			}
// 			else
// 			{
// 				CWindLoadGeneratorCtrl myCtrl;
// 				if (!myCtrl.RunFindClosedArea(m_Data.arKeysDefineElem, TRUE))
// 					return FALSE;
// 
// 				CArray<UINT, UINT> arSurfKey;
// 				CArray<T_NODE_K, T_NODE_K> caNodeKey;
// 				CArray<T_ELEM_K, T_ELEM_K> caElemKey;
// 				nSize = myCtrl.GetSurfKeyList(arSurfKey);
// 
// 				rDataSet.SetSize(nSize);
// 				for(int i=0; i < nSize; i++)
// 				{
// 					rDataSet[i] = m_Data;
// 					if (!myCtrl.GetSurfInfo(arSurfKey[i], rDataSet[i].arKeysDefineNode, rDataSet[i].arKeysDefineElem))
// 					{
// 						ASSERT(0);
// 						return FALSE;
// 					}
// 				}
// 			}
			
		}
	}

	int nSizeEpseSet = rDataSet.GetSize();
	if (nSizeEpseSet <= 0) return FALSE;

	CLateralLoad laterLoad;
	for (i = nSizeEpseSet - 1; i >= 0; i--)
	{
		if (!laterLoad.MakeSeismicEarthPressure(rDataSet[i], m_bInitProfile))
		{
			rDataSet.RemoveAt(i); //일단 실패하는 상황을 만들면 안되지만. 실패하면 지우자.
		}
	}

	return TRUE;
}

// CDgnGenMembDlg::MemberDefine() 참고
void CStldEarthPressureSeisDlg::GetMemberData(const T_KEY_LIST& aSelFrmKey, CArray<T_MEMB_D, T_MEMB_D&>& aDataMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_MEMB_D DataMemb;
	T_ELEM_K KeyElem, KeyTemp;
	T_ELEM_D DataElem, DElemNext;
	T_NODE_K KeyNode;
	CArray<T_ELEM_K, T_ELEM_K> aConnectedElem;
	CArray<T_MEMB_K, T_MEMB_K> aKeyMembDel;                    // 최종적으로 삭제할 멤버의 리스트
	//CArray<T_MEMB_D, T_MEMB_D&> aDataMemb;                   // 최종적으로 선택된 멤버의 리스트
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapUsedElem;  // 한 번이라도 Member 지정에 사용된 요소의 map  
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K> mapUsedNode;  // Member의 Element List에 속하는 요소들의 절점의 map
	CMap<T_MEMB_K, T_MEMB_K, int, int> mapKeyMembDel;          // 기존 요소가 멤버의 구성요소일 때 삭제할 멤버의 map  
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapSelFrmKey; // 선택된 element list
	mapUsedElem.InitHashTable(HASHSIZEELEM);
	mapKeyMembDel.InitHashTable(HASHSIZEMEMB);
	mapUsedNode.InitHashTable(51);
	mapSelFrmKey.InitHashTable(HASHSIZEELEM);
	mapUsedElem.RemoveAll();
	mapKeyMembDel.RemoveAll();
	aKeyMembDel.RemoveAll();
	aDataMemb.RemoveAll();

	DataMemb.Initialize();
	DataElem.Initialize();
	DElemNext.Initialize();
	BOOL bAddMembFlag = FALSE; // 하나의 Member정의가 끝났음을 알리는 flag  
	BOOL bNextInclude = FALSE; // 현재 요소의 절점에 연결된 요소가 동일한 Member의 구성요소로 포함되었는지를 나타내는 flag  

	int nNumSelFrmKey = aSelFrmKey.GetSize();
	for (int i = 0; i < nNumSelFrmKey; i++)
	{
		mapSelFrmKey.SetAt(aSelFrmKey[i], aSelFrmKey[i]);
	}

	for (int i = 0; i < nNumSelFrmKey; i++)
	{
		DataMemb.Initialize();
		mapUsedNode.RemoveAll();   // 폐곡선을 막기 위해 쓰임
		KeyElem = aSelFrmKey.GetAt(i);
		if (!mapUsedElem.Lookup(KeyElem, KeyTemp)) // 기존에 한번이라도 Member로 지정되지 않은 요소이면
			DataMemb.aElemList.Add(KeyElem);        // 현재 지정할 Member의 요소 리스트에 추가한다.
		else continue; // 기존에 Member로 지정된 요소이면 현재 요소에 대한 Member 지정을 하지 않고 다음 요소에 대해 작업한다.

		if (pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem))
		{
			mapUsedNode.SetAt(DataElem.elnod[0], DataElem.elnod[0]);    // 기준 요소의 i노드를 사용된 노드 map에 추가한다.
			mapUsedNode.SetAt(DataElem.elnod[1], DataElem.elnod[1]);    // 기준 요소의 j노드를 사용된 노드 map에 추가한다.
			bNextInclude = TRUE;


			// Frame
			if (!pDoc->m_pAttrCtrl->IsFrameType(DataElem.eltyp))
			{
				int nRemove = DataMemb.aElemList.GetSize() - 1;
				DataMemb.aElemList.RemoveAt(nRemove);
				continue;
			}

			// 우선은 연직인 것들만 취급하자!
			T_NODE_D DataNodeI, DataNodeJ;
			VERIFY(pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[0], DataNodeI));
			VERIFY(pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[1], DataNodeJ));
			if (!pDoc->m_pAttrCtrl->IsVerticalFrame(DataNodeI, DataNodeJ))
			{
				int nRemove = DataMemb.aElemList.GetSize()-1;
				DataMemb.aElemList.RemoveAt(nRemove);
				continue;  
			}

			while (bNextInclude)  // j node 방향으로 연결된 요소에 대해 작업한다.
			{
				pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(DataElem.elnod[1], aConnectedElem);
				for (int k = 0; k < aConnectedElem.GetSize(); k++)
				{
					bNextInclude = FALSE;  // 시작할때는 FALSE
					if (mapUsedElem.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;      // 기존에 한번이라도 Member로 지정되지 않고
				    if (!mapSelFrmKey.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;    // 현재 요소가 선택된 프레임 요소 리스트에 속하고
					if (aConnectedElem.GetAt(k) == KeyElem) continue;                        // 현재 요소가 기준 요소와 다르고           
					if (!pDoc->m_pAttrCtrl->GetElem(aConnectedElem.GetAt(k), DElemNext)) continue;  // 현재 요소의 데이터가 정의되어 있고
					if (DataElem.elnod[1] != DElemNext.elnod[0]) continue;                   // 기준 요소와 현재 요소의 node i, j 순서가 동일하고          
					if (mapUsedNode.Lookup(DElemNext.elnod[1], KeyNode)) continue;           // 선택된 요소들이 폐곡선을 이루지 않아야 한다.  
					if (!pDoc->m_pAttrCtrl->CompareElemDirection(KeyElem, aConnectedElem.GetAt(k))) continue; // 기준요소와 현재 요소가 같은 방향

					DataMemb.aElemList.Add(aConnectedElem.GetAt(k));              // 위의 과정을 무사시 통과하면 Member의 구성요소가 된다.
					mapUsedNode.SetAt(DElemNext.elnod[1], DElemNext.elnod[1]);    // 현재 요소의 j노드를 사용된 노드 map에 추가한다.
					KeyElem = aConnectedElem.GetAt(k);                            // 현재 추가된 요소를 기준 요소로 설정
					DataElem.elnod[1] = DElemNext.elnod[1];                       // 현재 추가된 요소의 j절점이 새로운 기준요소의 i절점이 된다.          
					bNextInclude = TRUE;                                          // 새로운 기준요소의 j절점에 대해 작업을 계속함을 의미
					break;
				}
			}

			bNextInclude = TRUE;
			while (bNextInclude)   // i node 방향으로 연결된 요소에 대해 작업한다.
			{
				pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(DataElem.elnod[0], aConnectedElem);
				for (int k = 0; k < aConnectedElem.GetSize(); k++)
				{
					bNextInclude = FALSE;  // 시작할 때는 FALSE
					if (mapUsedElem.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;      // 기존에 한번이라도 Member로 지정되지 않고
				    if (!mapSelFrmKey.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;    // 현재 요소가 선택된 프레임 요소 리스트에 속하고
					if (aConnectedElem.GetAt(k) == KeyElem) continue;						 // 현재 요소가 기준 요소와 다르고 
					if (!pDoc->m_pAttrCtrl->GetElem(aConnectedElem.GetAt(k), DElemNext)) continue;  // 현재 요소의 데이터가 정의되어 있고
					if (DataElem.elnod[0] != DElemNext.elnod[1]) continue;					 // 기준 요소와 현재 요소의 node i, j 순서가 동일하고          
					if (mapUsedNode.Lookup(DElemNext.elnod[0], KeyNode)) continue;           // 선택된 요소들이 폐곡선을 이루지 않아야 한다.  
					if (!pDoc->m_pAttrCtrl->CompareElemDirection(KeyElem, aConnectedElem.GetAt(k))) continue; // 기준요소와 현재 요소가 같은 방향

					DataMemb.aElemList.Add(aConnectedElem.GetAt(k));              // 위의 과정을 무사시 통과하면 Member의 구성요소가 된다.
					mapUsedNode.SetAt(DElemNext.elnod[0], DElemNext.elnod[0]);    // 현재 요소의 j노드를 사용된 노드 map에 추가한다.
					KeyElem = aConnectedElem.GetAt(k);                            // 현재 추가된 요소를 기준 요소로 설정
					DataElem.elnod[0] = DElemNext.elnod[0];                       // 현재 추가된 요소의 i절점이 새로운 기준요소의 i절점이 된다.
					bNextInclude = TRUE;                                          // 새로운 기준요소의 i절점에 대해 작업을 계속함을 의미
					break;
				}
			}
		}

		if (DataMemb.aElemList.GetSize() >= 1)
		{
			for (int m = 0; m < DataMemb.aElemList.GetSize(); m++)
			{
				mapUsedElem.SetAt(DataMemb.aElemList[m], DataMemb.aElemList[m]); // 한번이라도 Member 지정에 사용된 요소의 map에 추가한다.
			}
			pDoc->m_pAttrCtrl->AlignElemListInOrder(DataMemb);
			aDataMemb.Add(DataMemb);  // 추가할 멤버를 모은다.
		}
	}
}

// 각 그룹별 직선 형태의 정렬된 요소만 들어옴.
void CStldEarthPressureSeisDlg::GetNodeListFromBottom(const CArray<T_ELEM_K, T_ELEM_K>& aElem, CArray<T_NODE_K, T_NODE_K>& aNode)
{
	if (aElem.GetSize() == 0) return;
	aNode.RemoveAll();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	const int NODENUM = 2;

	//
	CMap<UINT, UINT, int, int> mapNodeKCount;
	for (int j = 0; j < aElem.GetSize(); ++j)
	{
		T_ELEM_D ElemD; ElemD.Initialize();
		if (!pDoc->m_pAttrCtrl->GetElem(aElem[j], ElemD)) continue;
		for (int k = 0; k < NODENUM; ++k)
		{
			int nCount = 0;
			if(!mapNodeKCount.Lookup(ElemD.elnod[k], nCount))
			{
				mapNodeKCount.SetAt(ElemD.elnod[k], nCount+1);
			}
		}
	}

	// 
	int nCount;
	UINT NodeK;
	T_NODE_D NodeD;
	CArray<int, int> aSortK;
	CArray<double, double> aSortD;

	POSITION pos = mapNodeKCount.GetStartPosition();
	while (pos)
	{
		mapNodeKCount.GetNextAssoc(pos, NodeK, nCount);

		NodeD.Initialize();
		if (!pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD)) continue;

		aSortK.Add(NodeK);
		aSortD.Add(NodeD.z);
	}

	// Z값에 따라 오름차순 key 정렬
	int nStorKCnt = aSortK.GetSize();
	CQSort::QSortDblWithIndex(aSortK.GetData(), aSortD.GetData(), nStorKCnt);

	for(int i=0; i< aSortK.GetSize(); i++)
		aNode.Add(aSortK[i]);
}



void CStldEarthPressureSeisDlg::OnCmdSeismicLoadBtn()
{
	CSeismicLoadListDlg dlg;
	dlg.DoModal();
}
void CStldEarthPressureSeisDlg::OnCmdSoilPropBtn()
{
	CSoilPropListDlg dlg;
	dlg.DoModal();
}

void CStldEarthPressureSeisDlg::OnCmdLayerParamRdo()
{
	int nLayerMethod = EN_LAYER_METHOD_SINGLE;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLayerMethod, nLayerMethod);

	if (nLayerMethod == EN_LAYER_METHOD_SINGLE || nLayerMethod == EN_LAYER_METHOD_USER) m_edt2ndLayer.SetEditUnit(0.0);

	BOOL bEnable2ndLayer = nLayerMethod==EN_LAYER_METHOD_DOUBLE  &&  m_nEPSEMethod == EN_EPSE_METHOD_RESPONSE;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDoubleCosine, bEnable2ndLayer);
	m_bInitProfile = TRUE;
}
int CStldEarthPressureSeisDlg::GetElementType()
{
	int nElemType = ELEM_FRAME;
	if (((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO))->GetCheck())
		nElemType = ELEM_FRAME;
	else if (((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO))->GetCheck())
		nElemType = ELEM_PLANAR;
	else
	{
		nElemType = ELEM_FRAME;
		((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO))->SetCheck(FALSE);
	}
	return nElemType;
}


void CStldEarthPressureSeisDlg::OnCmdPressureProfile()
{
	if (!Dlg2Data())
		return;

	double dHeight = 0.0;
	BOOL bRetVal = CWindLoadGeneratorCtrl::GetModelHeight4EarthPressure(m_Data.KeyPosp, dHeight);
	if (dHeight <= 1E-5 || bRetVal == FALSE)
	{
		AfxMessageBox(_LS(IDS_TM_EARTHP_NO_NODEINFO));
		return;
	}
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pEditData->CheckEpseEquivalent(m_Data);

	CCMSeismicEarthPressureProfileDlg dlg;
	dlg.InitProfile(m_Data, m_bInitProfile);
	dlg.DoModal();
	
	m_bInitProfile = FALSE;
}


void CStldEarthPressureSeisDlg ::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;
	int nDistX = 0;

	if(m_bCivil)
	{
		// Loading Type
		GetDlgItem(IDC_TM_EP_OPTION_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_EP_LOADINGTYPE_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlLoadingType, nDistY);
	}

	// Direction, Angle...
	CArray<UINT, UINT> aControls02; aControls02.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, aControls02, IDC_TM_EP_OPTION_GRP2, TRUE);

	if(m_bCivil)
		GetDlgItem(IDC_TM_EP_LOADINGTYPE_GRP)->GetWindowRect(rRef);
	else
		GetDlgItem(IDC_TM_EP_OPTION_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_EP_OPTION_GRP2)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(1);
	CDlgUtil::CtrlMoveDistY(this, aControls02, nDistY);

	// Seismic Load code...
	CArray<UINT, UINT> aControls03; aControls03.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, aControls03, IDC_TM_EP_OPTION_GRP3, TRUE);

	GetDlgItem(IDC_TM_EP_OPTION_GRP2)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_EP_OPTION_GRP3)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(1);
	CDlgUtil::CtrlMoveDistY(this, aControls03, nDistY);

	// Method
	GetDlgItem(IDC_TM_EP_OPTION_GRP3)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_EP_TYPE_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlEPSLMethod, nDistY);

	// Layer
	CArray<UINT, UINT> aControlsLayer; aControlsLayer.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, aControlsLayer, IDC_TM_EP_LAYER_GRP, TRUE);

	GetDlgItem(IDC_TM_EP_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_EP_LAYER_GRP)->GetWindowRect(rToMove);
	if(m_bCivil)
		nDistY = rRef.top - rToMove.top;
	else
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
	CDlgUtil::CtrlMoveDistY(this, aControlsLayer, nDistY);

	// Parameter, Loading Area Group
	CArray<UINT, UINT> aControls04; aControls04.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, aControls04, IDC_TM_EP_OPTION_GRP4, TRUE);

	GetDlgItem(IDC_TM_EP_LAYER_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_EP_OPTION_GRP4)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(1);
	CDlgUtil::CtrlMoveDistY(this, aControls04, nDistY);

	if(m_bCivil)
	{
		GetDlgItem(IDC_TM_TARGET_STATIC)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE_GRB)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(2);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlLoadingGrMove, nDistY);

    	// Group resize
		CRect rectResize;
		GetDlgItem(IDC_TM_EP_OPTION_GRP4)->GetWindowRect(rectResize);
		GetDlgItem(IDC_TM_ELEM_DEFINE)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
		ScreenToClient(rectResize);
		GetDlgItem(IDC_TM_EP_OPTION_GRP4)->MoveWindow(rectResize);
	}

	// Button
	GetDlgItem(IDC_TM_EP_OPTION_GRP4)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_BTN_PROFILE)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aLastCtrlData, nDistY);
}