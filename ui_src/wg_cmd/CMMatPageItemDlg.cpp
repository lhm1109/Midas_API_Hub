// CMMatPageItemDlg.cpp : implementation file
//
// Steel, Concrete, User는 Data1만 이용
// SRC는 Data1, Data2 모두 이용
// Steel, User Type은 steel 부분에 출력
// Concrete는 concrete 부분에 출력
// SRC는 Data1은 Steel에 Data2는 Concrete에 출력 되도록 프로그램
// 
// 2005. 5 무조건 섭씨로 DB에 넘겨주었으나 섭씨온도, 화씨온도 그대로 넘겨줌 (From 김영민과장님)

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMatPageItemDlg.h"
#include "CMDesignCodeListDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"

#include "..\wg_db\wg_db_EditData.h"

#include "..\wg_main\wg_mainres2.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CCM_TYPE_STEEL  _ULS(S)
#define CCM_TYPE_CONCR  _ULS(C)
#define CCM_TYPE_SRC    _ULS(SRC)
#define CCM_TYPE_USER   _ULS(U)
#define CCM_TYPE_ALUMNI _ULS(A)
#define CCM_TYPE_TIMBER _ULS(T)

#define CCM_STEEL       _LS(IDS_WG_CMD__ADDD__Steel)
#define CCM_CONCR       _LS(IDS_WG_CMD__ADDD__Concrete)
#define CCM_SRC         _LS(IDS_WG_CMD__ADDD_MATL_SRC)
#define CCM_USER        _LS(IDS_WG_CMD__ADDD__User_Defined)
#define CCM_ALUMINIUM   _LS(IDS_WG_CMD__ADDD__Aluminium)
#define CCM_TIMBER   _LS(IDS_WG_CMD__ADDD__Timber)
#if defined(_MGEN) || defined(_CIVIL)
	#define CCM_TYPE_STEEL_I  0
	#define CCM_TYPE_CONCR_I  1
	#define CCM_TYPE_SRC_I    2
	#define CCM_TYPE_ALUMI_I  3
	#define CCM_TYPE_TIMBER_I 4
	#define CCM_TYPE_USER_I   5
#elif defined(_TOWER)
	#define CCM_TYPE_STEEL_I  0
	#define CCM_TYPE_USER_I   1
	#define CCM_TYPE_CONCR_I  -1  // (Compile용)
	#define CCM_TYPE_SRC_I    -1  // (Compile용)
	#define CCM_TYPE_ALUMI_I  -1  // (Compile용)
#else
	#error _LS(IDS_CMD0417__Unknown_Product_Definition)
#endif

#define CCM_CODE_NONE   _LS(IDS_WG_CMD__ADDD__None)

/////////////////////////////////////////////////////////////////////////////
// CCMMatPageItemDlg dialog
#define CDialog CDialogMove
CCMMatPageItemDlg::CCMMatPageItemDlg(CDBDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMatPageItemDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_bModify = FALSE;
	//m_pBitmap = 0;  
	m_bWorkTab = false;

	//{{AFX_DATA_INIT(CCMMatPageItemDlg)
	m_wndMatlTypeRdo = 0;
	//}}AFX_DATA_INIT
	// Isotropic related entire control
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_FRAME);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_ELA_TEXT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_ELA_EDIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_ELA_UNIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_ELA_USER_CHK);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_POI_TEXT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_POI_EDIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_POI_UNIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_THR_TEXT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_THR_EDIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_THR_UNIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_DEN_TEXT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_DEN_EDIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_DEN_UNIT);  
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_MDEN_TEXT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_MDEN_EDIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_MDEN_UNIT);  
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_FRAME);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_ELA_TEXT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_ELA_EDIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_ELA_UNIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_POI_TEXT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_ELA_USER_CHK);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_POI_EDIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_POI_UNIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_THR_TEXT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_THR_EDIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_THR_UNIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_DEN_TEXT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_DEN_EDIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_DEN_UNIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_MDEN_TEXT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_MDEN_EDIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_MDEN_UNIT);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_THR_RD1);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_STL_THR_RD2);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_THR_RD1);
	m_aCtrlIso.Add(IDC_CMD_MATL_ISO_CON_THR_RD2);
 
	// Orthotropic related entire control
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_FRAME);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_LX_TITLE);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_LY_TITLE);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_LZ_TITLE);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_ELA_TEXT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_ELA_LX_EDIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_ELA_LY_EDIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_ELA_LZ_EDIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_ELA_UNIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_THR_TEXT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_THR_LX_EDIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_THR_LY_EDIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_THR_LZ_EDIT);  
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_THR_UNIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_LXY_TITLE);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_LXZ_TITLE);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_LYZ_TITLE);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_SHR_TEXT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_SHR_LXY_EDIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_SHR_LXZ_EDIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_SHR_LYZ_EDIT);  
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_SHR_UNIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_POI_TEXT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_POI_LXY_EDIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_POI_LXZ_EDIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_POI_LYZ_EDIT);  
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_POI_UNIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_DEN_TEXT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_DEN_EDIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_DEN_UNIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_MDEN_TEXT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_MDEN_EDIT);
	m_aCtrlOrt.Add(IDC_CMD_MATL_ORT_MDEN_UNIT);

	// Steel Isotropic control
	m_aCtrlIsoStlAnal.Add(IDC_CMD_MATL_ISO_STL_ELA_EDIT);
	m_aCtrlIsoStlAnal.Add(IDC_CMD_MATL_ISO_STL_POI_EDIT);
	m_aCtrlIsoStlAnal.Add(IDC_CMD_MATL_ISO_STL_THR_EDIT);
	m_aCtrlIsoStlAnal.Add(IDC_CMD_MATL_ISO_STL_DEN_EDIT);
	m_aCtrlIsoStlAnal.Add(IDC_CMD_MATL_ISO_STL_MDEN_EDIT);
//  m_aCtrlIsoStlAnal.Add(IDC_CMD_MATL_ISO_STL_MDEN_TEXT);

	// Concrete Isotropic control
	m_aCtrlIsoConAnal.Add(IDC_CMD_MATL_ISO_CON_ELA_EDIT);
	m_aCtrlIsoConAnal.Add(IDC_CMD_MATL_ISO_CON_POI_EDIT);
	m_aCtrlIsoConAnal.Add(IDC_CMD_MATL_ISO_CON_THR_EDIT);
	m_aCtrlIsoConAnal.Add(IDC_CMD_MATL_ISO_CON_DEN_EDIT);  
	m_aCtrlIsoConAnal.Add(IDC_CMD_MATL_ISO_CON_MDEN_EDIT);  
//  m_aCtrlIsoConAnal.Add(IDC_CMD_MATL_ISO_CON_MDEN_TEXT);

	// Orthotropic material data
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_ELA_LX_EDIT);
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_ELA_LY_EDIT);
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_ELA_LZ_EDIT);
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_THR_LX_EDIT);
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_THR_LY_EDIT);
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_THR_LZ_EDIT);  
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_SHR_LXY_EDIT);
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_SHR_LXZ_EDIT);
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_SHR_LYZ_EDIT);
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_POI_LXY_EDIT);
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_POI_LXZ_EDIT);
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_POI_LYZ_EDIT);      
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_DEN_EDIT);
	m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_MDEN_EDIT);
//  m_aCtrlOrtAnal.Add(IDC_CMD_MATL_ORT_MDEN_TEXT);
	
	// Thermal Transfer related control : tower인 경우 disable 시킨다.
	m_aCtrlThermal.Add(IDC_CMD_MP_ITEM_THER_FRAME);
	m_aCtrlThermal.Add(IDC_CMD_MP_ITEM_THER_SPHEAT_T);
	m_aCtrlThermal.Add(IDC_CMD_MP_ITEM_THER_SPHEAT_E);
	m_aCtrlThermal.Add(IDC_CMD_MP_ITEM_THER_SPHEAT_U);
	m_aCtrlThermal.Add(IDC_CMD_MP_ITEM_THER_HEATCO_T);
	m_aCtrlThermal.Add(IDC_CMD_MP_ITEM_THER_HEATCO_E);
	m_aCtrlThermal.Add(IDC_CMD_MP_ITEM_THER_HEATCO_U);
	
	// Isotropic, Orthotropic Radio Button
	m_aCtrlMatlTypeRdoIso.Add(IDC_CMD_MP_ITEM_ISO_RDO1);
	m_aCtrlMatlTypeRdoOrt.Add(IDC_CMD_MP_ITEM_ISO_RDO2);

	// Standard and DB control of Steel
	m_aCtrlStlCode.Add(IDC_CMD_MP_ITEM_CODE);
	m_aCtrlStlCode.Add(IDC_CMD_MP_ITEM_NAME_CBO);
	m_aCtrlStlCode.Add(IDC_CMD_MP_ITEM_PRODUCT_CBO);
	m_aCtrlStlCode.Add(IDC_CMD_MP_ITEM_ONLY_STRAND_CHK);
	
	// Standard and DB control of Concrete
	m_aCtrlConCode.Add(IDC_CMD_MP_ITEM_CODE2);
	m_aCtrlConCode.Add(IDC_CMD_MP_ITEM_CODE3);
	m_aCtrlConCode.Add(IDC_CMD_MP_ITEM_NAME_CBO2);

	// Thermal Option of Steel
	m_aCtrlThermalSteel.Add(IDC_CMD_MATL_ISO_STL_THR_RD2);
	m_aCtrlThermalSteel.Add(IDC_CMD_MATL_ISO_STL_THR_RD1);

	// Thermal Option of Concrete
	m_aCtrlThermalConc.Add(IDC_CMD_MATL_ISO_CON_THR_RD2);
	m_aCtrlThermalConc.Add(IDC_CMD_MATL_ISO_CON_THR_RD1);

	// Inelastic Material Properties for Fiber Model
	m_aCtrlInelasticMatl.Add(IDC_CMD_FRAME11);
	m_aCtrlInelasticMatl.Add(IDC_CMD_INELAS_CONC_STC);
	m_aCtrlInelasticMatl.Add(IDC_CMD_INELAS_CONC_CMB);
	m_aCtrlInelasticMatl.Add(IDC_CMD_INELAS_STL_STC);
	m_aCtrlInelasticMatl.Add(IDC_CMD_INELAS_STL_CMB);
	m_aCtrlInelasticMatl.Add(IDC_CMD_INELAS_BTN);
	m_aCtrlInelasticMatl.Add(IDC_CMD_INELAS_COVER_CONC_STC);
	m_aCtrlInelasticMatl.Add(IDC_CMD_INELAS_COVER_CONC_CMB);
}

void CCMMatPageItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMatPageItemDlg)
	DDX_Control(pDX, IDC_CMD_MATL_ORT_MDEN_TEXT, m_wndOrtMDenChk);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_MDEN_TEXT, m_wndIsoConMDenChk);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_MDEN_TEXT, m_wndIsoStlMDenChk);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_THER_SPHEAT_U, m_wndUnitSpHeat);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_THER_HEATCO_U, m_wndUnitHeatCo);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_THER_SPHEAT_E, m_wndSpHeat);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_THER_HEATCO_E, m_wndHeatCo);
	DDX_Control(pDX, IDC_WG_CMD_STATIC3, m_wndElasticityFrame);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_FRAME, m_wndIsoStlFrame);  
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_FRAME, m_wndIsoConFrame);  
	DDX_Control(pDX, IDC_CMD_FRAME, m_wndStlCodeFrame);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_FRAME, m_wndOrtFrame);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_BTN_APPLY, m_wndBtnApply);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_ELA_UNIT, m_wndIsoStlElaUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_POI_UNIT, m_wndIsoStlPoiUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_THR_UNIT, m_wndIsoStlThrUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_DEN_UNIT, m_wndIsoStlDenUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_MDEN_UNIT, m_wndIsoStlMDenUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_ELA_UNIT, m_wndIsoConElaUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_POI_UNIT, m_wndIsoConPoiUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_THR_UNIT, m_wndIsoConThrUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_DEN_UNIT, m_wndIsoConDenUnit);		
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_MDEN_UNIT, m_wndIsoConMDenUnit);		
	DDX_Control(pDX, IDC_CMD_MATL_ORT_ELA_UNIT, m_wndOrtElaUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_POI_UNIT, m_wndOrtPoiUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_THR_UNIT, m_wndOrtThrUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_SHR_UNIT, m_wndOrtShrUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_DEN_UNIT, m_wndOrtDenUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_MDEN_UNIT, m_wndOrtMDenUnit);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_ELA_EDIT, m_wndIsoStlEla);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_POI_EDIT, m_wndIsoStlPoi);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_THR_EDIT, m_wndIsoStlThr);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_DEN_EDIT, m_wndIsoStlDen);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_MDEN_EDIT, m_wndIsoStlMDen);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_ELA_EDIT, m_wndIsoConEla);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_POI_EDIT, m_wndIsoConPoi);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_THR_EDIT, m_wndIsoConThr);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_DEN_EDIT, m_wndIsoConDen);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_MDEN_EDIT, m_wndIsoConMDen);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_ELA_LX_EDIT,  m_wndOrtElaLx);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_ELA_LY_EDIT,  m_wndOrtElaLy);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_ELA_LZ_EDIT,  m_wndOrtElaLz);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_THR_LX_EDIT,  m_wndOrtThrLx);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_THR_LY_EDIT,  m_wndOrtThrLy);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_THR_LZ_EDIT,  m_wndOrtThrLz);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_POI_LXY_EDIT, m_wndOrtPoiLxy);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_POI_LXZ_EDIT, m_wndOrtPoiLxz);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_POI_LYZ_EDIT, m_wndOrtPoiLyz);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_SHR_LXY_EDIT, m_wndOrtShrLxy);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_SHR_LXZ_EDIT, m_wndOrtShrLxz);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_SHR_LYZ_EDIT, m_wndOrtShrLyz);
	DDX_Control(pDX, IDC_CMD_MATL_ORT_DEN_EDIT,     m_wndOrtDen);  
	DDX_Control(pDX, IDC_CMD_MATL_ORT_MDEN_EDIT,     m_wndOrtMDen);  
	DDX_Control(pDX, IDC_CMD_MP_ITEM_NAME_CBO2, m_wndConcrName);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_CODE2, m_wndConcrCode);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_CODE3, m_wndConcrSubCode);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_NAME_CBO, m_wndSteelName);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_ONLY_STRAND_CHK, m_wndOnlyStrandChk);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_PRODUCT_CBO, m_wndProductName);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_CODE, m_wndSteelCode);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_TYPE, m_wndType);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_ID, m_wndID);
	DDX_Control(pDX, IDC_CMD_PLASTIC_NAME_CMB, m_wndPlasticCmb);
	DDX_Radio(pDX, IDC_CMD_MP_ITEM_ISO_RDO1, m_wndMatlTypeRdo);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_DAMPING_RATIO_EDT, m_edtDampingRatio);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_STL_ELA_USER_CHK, m_wndIsoStlUserElaChk);
	DDX_Control(pDX, IDC_CMD_MATL_ISO_CON_ELA_USER_CHK, m_wndIsoConUserElaChk);
	DDX_Control(pDX, IDC_CMD_INELAS_CONC_CMB, m_cmbInelasConc);
	DDX_Control(pDX, IDC_CMD_INELAS_COVER_CONC_CMB, m_cmbInelasCoverConc);
	DDX_Control(pDX, IDC_CMD_INELAS_STL_CMB , m_cmbInelasStl);
	DDX_Control(pDX, IDC_CMD_PICTURE, m_ImgMatl);
	//}}AFX_DATA_MAP

}

BOOL CCMMatPageItemDlg::OnInitDialog() 
{
	SetRedraw(FALSE);

	MInitCombo initCombo;
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDefaultCode(); // 제일 먼저 호출되어야 한다.
	AlignControl();

#ifdef _MGEN_CH
	GetDlgItem(IDC_CMD_MP_ITEM_ONLY_STRAND_CHK)->ShowWindow(SW_HIDE);
#endif

	m_ImgMatl.SetIgnoreLargeView(TRUE);

#if defined(_CIVIL) || defined(_MGEN)
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlThermal, TRUE);
#elif defined(_TOWER)
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlThermal, FALSE);
#else
	#error Unknown_Product_Definition
#endif

//   //============================================================================================//
//   /** void CClassMatl::ConvertThermalCoefToFahr(T_MATL_D& rMatlD) 함수에서 
//       DB인 경우 Fahrenheit 온도 단위를 쓰면 무조건 Fahrenheit온도로 바꿔서 값을 주기 때문에
//       아래의 예외 처리 루틴을 추가 (원래는 무조건 Celsius 단위계로 주기로 되어 있음.)
//       2005.11.14 byhssim
//   */
//   // DB인 경우만 변환함
//   CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
//   if (m_Data.Data1.CodeName != "" && m_Data.Data1.CodeMatlName != "") 
//     m_Data.Data1.Analysis.Thermal = m_Data.Data1.Analysis.Thermal / pUnitCtrl->GetThermalTransCoeff(TRUE) ;     // Fahrenheit -> Celsius
//   if (m_Data.Type == "SRC" && m_Data.Data2.CodeName != "" && m_Data.Data2.CodeMatlName != 0)
//     m_Data.Data2.Analysis.Thermal = m_Data.Data2.Analysis.Thermal / pUnitCtrl->GetThermalTransCoeff(TRUE) ;     // Fahrenheit -> Celsius
//   //============================================================================================//

	if(!m_bModify) 
	{
		m_Key = m_pDoc->m_pAttrCtrl->GetStartNumMatl();
		m_Data.Initialize();

#if defined(_CH)  // 중국 버전은 무조건 Default로 Concrete이다.
		m_Data.Type = CCM_TYPE_CONCR;
		m_Data.Data1.CodeName = m_csDefConcrCode;
		m_Data.Data1.SubCodeName = m_csDefConcrSubCode;
		m_Data.Data2.CodeName = m_csDefConcrCode;
		m_Data.Data2.SubCodeName = m_csDefConcrSubCode;
#else
		m_Data.Type = CCM_TYPE_STEEL;
		m_Data.Data1.CodeName = m_csDefSteelCode;
		m_Data.Data2.CodeName = m_csDefConcrCode;
		m_Data.Data2.SubCodeName = m_csDefConcrSubCode;

		// MQC:9175-JWKWON-20141201-Initialize를 하고 나면 Damping Ratio가 0.05가 됨..
		m_Data.dDampingRatio = 0.02;
#endif

		// Thermal Option - 미국은 화씨(1), 나머지는 섭씨(0)를 Default로 한다.
#if defined(_US) || defined(_RUS)
		m_Data.nThermalUnit = 1;
#else
		m_Data.nThermalUnit = 0;
#endif
	}

	CheckCode();
	SetUnitType();
	SetTypeCombo();
	SetInelasticCombo();
	
	ChangeBitmap(GetTypeIndex(m_Data.Type));
	ShowDataToDlg();
	
	SetAutoChangeNameFlag(m_Data.Type, m_Data.Name, m_Data.Data1.SubCodeName);

	if(m_bWorkTab)
	{
		CString csValue;
		csValue.Format(_T("%d"), m_FixID);
		m_wndID.SetWindowText(csValue);
		UpdateData(FALSE);
	}

	if(m_Data.Data1.Analysis.nIsoOrtho == 0)      InitOrtAnalData();
	else if(m_Data.Data1.Analysis.nIsoOrtho == 1) InitIsoAnalData();
	
	if(m_bModify) m_wndBtnApply.EnableWindow(FALSE);

	EnableDisableUserEla();
	EnableShowOnlyStrand();

	OnCmdMatlIsoStlUserElaChk();
	OnCmdMatlIsoConUserElaChk();
	
//#if defined(_MATDB_EXPORT)
	//ExportMatlDB();
//#endif
	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Steel, Concrete Default Code를 멤버 변수에 설정한다.
void CCMMatPageItemDlg::GetDefaultCode()
{
	CArray<CString, CString&> aSteelCodeList, aConcrCodeList, aAluCodeList, aTimberCodeList;
	m_pDoc->m_pMatlDB->GetDesignCodeList(CString(CCM_TYPE_STEEL), aSteelCodeList);
	m_pDoc->m_pMatlDB->GetDesignCodeList(CString(CCM_TYPE_CONCR), aConcrCodeList);
	m_pDoc->m_pMatlDB->GetDesignCodeList(CString(CCM_TYPE_ALUMNI), aAluCodeList );
	m_pDoc->m_pMatlDB->GetDesignCodeList(CString(CCM_TYPE_TIMBER), aTimberCodeList);

	// Preference에서 초기값 읽어온다.
	T_PREFERENCE pref;
	m_pDoc->m_pInitCtrl->GetPreference(pref);
	m_csDefSteelCode = pref.Property.SteelMaterialDBName;
	m_csDefConcrCode = pref.Property.ConcreteMaterialDBName;
	m_csDefConcrSubCode = pref.Property.ConcreteMaterialCodeName;
	if (m_csDefSteelCode != _T("") && !IsExistInCodeList(aSteelCodeList, m_csDefSteelCode)) 
		m_csDefSteelCode = _T("");
	if (m_csDefConcrCode != _T("") && !IsExistInCodeList(aConcrCodeList, m_csDefConcrCode)) 
		m_csDefConcrCode = _T("");

	// Aluminum은 무조건 DB에서 선택
	if (aAluCodeList.GetSize() > 0) m_csDefAluminumCode = aAluCodeList[0];
	else m_csDefAluminumCode = _T("");

	if (m_csDefSteelCode != _T("")) 
	{
		m_csDefSteelCodeSRC = m_csDefSteelCode;
	}
	else  // SRC는 무조건 DB에서 선택
	{
		if (aSteelCodeList.GetSize() > 0) m_csDefSteelCodeSRC = aSteelCodeList[0];
		else m_csDefSteelCodeSRC = _T("");
	}
	if (m_csDefConcrCode != _T("")) 
	{
		m_csDefConcrCodeSRC = m_csDefConcrCode;
		m_csDefConcrSubCodeSRC = m_csDefConcrSubCode;
	}
	else
	{
		if (aConcrCodeList.GetSize() > 0) m_csDefConcrCodeSRC = aConcrCodeList[0];
		else m_csDefConcrCodeSRC = _T("");
	}

	// Timber는 무조건 DB에서 선택
	if (aTimberCodeList.GetSize() > 0) m_csDefTimberCode = aTimberCodeList[0];
	else m_csDefTimberCode = _T("");
}

void CCMMatPageItemDlg::AlignControl()
{
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_MP_ITEM_BTN_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(16);
	MoveWindow(r);

	// set FolderGroupBox
	//m_wndIsoStlFrame.InitControl(this, CCMMatPageItemDlg::IDD, IDC_CMD_MATL_ISO_STL_FRAME,TRUE,TRUE);
	m_wndIsoConFrame.InitControl(this, CCMMatPageItemDlg::IDD, IDC_CMD_MATL_ISO_CON_FRAME,TRUE,TRUE);

	// move Orthotropic matl type controls
	CRect rRef;
	CRect rToMove;
	int nDistY;	
	UINT aFirstCtrlID[] = {IDC_CMD_MATL_ORT_FRAME};
	CArray<UINT, UINT> *aCtrlSet[] = {&m_aCtrlOrt};

	GetDlgItem(IDC_CMD_MATL_ISO_STL_FRAME)->GetWindowRect(rRef);
	for(int i = 0; i < sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, *aCtrlSet[i], nDistY);
	}

	// show/hide controls

	// MNET:XXXX-HSSHIM-20131030. 알루미늄 재질을 국문 Gen에만 열어줌.
	// 알루미늄 재질을 중문 Gen에도 열어줌 JWKWON-2017-05-23
	BOOL bHideAluminum = TRUE;
#if defined(_MGEN_ORG) || defined(_MGEN_CH) || defined(_MGEN_US) || defined(_MGEN_RUS)
	bHideAluminum = FALSE;
#endif
	if (bHideAluminum)
	{
		CArray<UINT, UINT> aCtrl_AluminumOnly;  aCtrl_AluminumOnly.RemoveAll();
		aCtrl_AluminumOnly.Add(IDC_CMD_MP_ITEM_PRODUCT_STC);
		aCtrl_AluminumOnly.Add(IDC_CMD_MP_ITEM_PRODUCT_CBO);
		aCtrl_AluminumOnly.FreeExtra();

		CDlgUtil::CtrlShowHide(this, aCtrl_AluminumOnly, FALSE);
	}

#ifdef _MEC
	// Inelastic Material Properties Controls Hide
	CDlgUtil::CtrlShowHide(this, m_aCtrlInelasticMatl, FALSE);

	CArray<UINT, UINT> aCtrl;
	aCtrl.Add(IDC_CMD_MP_ITEM_THER_FRAME);
	aCtrl.Add(IDC_CMD_MP_ITEM_THER_SPHEAT_T);
	aCtrl.Add(IDC_CMD_MP_ITEM_THER_SPHEAT_E);
	aCtrl.Add(IDC_CMD_MP_ITEM_THER_SPHEAT_U);
	aCtrl.Add(IDC_CMD_MP_ITEM_THER_HEATCO_T);
	aCtrl.Add(IDC_CMD_MP_ITEM_THER_HEATCO_E);
	aCtrl.Add(IDC_CMD_MP_ITEM_THER_HEATCO_U);
	aCtrl.Add(IDC_CMD_MP_ITEM_DAMPING_RATIO_STC);
	aCtrl.Add(IDC_CMD_MP_ITEM_DAMPING_RATIO_EDT);
	aCtrl.Add(IDOK);
	aCtrl.Add(IDCANCEL);
	aCtrl.Add(IDC_CMD_MP_ITEM_BTN_APPLY);

	UINT aMecFirstCtrl[] = { IDC_CMD_MP_ITEM_THER_FRAME };
	CArray<UINT, UINT>* aTgtCtrl[] = { &aCtrl };
	GetDlgItem(IDC_CMD_FRAME11)->GetWindowRect(rRef);
	int nDlgY = 0;
	for (int i = 0; i < sizeof(aMecFirstCtrl) / sizeof(UINT); i++)
	{
		GetDlgItem(aMecFirstCtrl[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		if (i == 0) nDlgY = nDistY;
		CDlgUtil::CtrlMoveDistY(this, *aTgtCtrl[i], nDistY);
	}
	CRect rDlg;
	GetWindowRect(rDlg);
	rDlg.bottom += nDlgY;
	MoveWindow(rDlg);
#endif
}

BOOL CCMMatPageItemDlg::CheckCode()
{
	CArray<CString, CString&> aSteelCodeList, aConcrCodeList, aAluCodeList, aTimberCodeList;;
	m_pDoc->m_pMatlDB->GetDesignCodeList(CString(CCM_TYPE_STEEL), aSteelCodeList);
	m_pDoc->m_pMatlDB->GetDesignCodeList(CString(CCM_TYPE_CONCR), aConcrCodeList);
	m_pDoc->m_pMatlDB->GetDesignCodeList(CString(CCM_TYPE_ALUMNI), aAluCodeList );
	m_pDoc->m_pMatlDB->GetDesignCodeList(CString(CCM_TYPE_TIMBER), aTimberCodeList);

	if (m_Data.Type == CCM_TYPE_STEEL)
	{
		// steel에서 조사
		if (m_Data.Data1.CodeName == _T("")) return TRUE;
		if (!IsExistInCodeList(aSteelCodeList, m_Data.Data1.CodeName))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Steel_code__s__is_not_exi), m_Data.Data1.CodeName);
			AfxMessageBox(msg);
			m_Data.Data1.CodeName = _T("");
			return FALSE;
		}
	}
	else if (m_Data.Type == CCM_TYPE_CONCR)
	{
		// concrete에서 조사
		if (m_Data.Data1.CodeName == _T("")) return TRUE;
		if (!IsExistInCodeList(aConcrCodeList, m_Data.Data1.CodeName))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Concrete_code__s__is_not_), m_Data.Data1.CodeName);
			AfxMessageBox(msg);
			m_Data.Data1.CodeName = _T("");
			return FALSE;
		}
	}
	else if (m_Data.Type == CCM_TYPE_USER)
	{
		// steel, concrete 둘다 조사
		if (m_Data.Data1.CodeName == _T("")) return TRUE;
		if (!IsExistInCodeList(aSteelCodeList, m_Data.Data1.CodeName) &&
				!IsExistInCodeList(aConcrCodeList, m_Data.Data1.CodeName) &&
				!IsExistInCodeList(aAluCodeList  , m_Data.Data1.CodeName))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___User_code__s__is_not_exis), m_Data.Data1.CodeName);
			AfxMessageBox(msg);
			m_Data.Data1.CodeName = _T("");
			return FALSE;
		}
	}
	else if(m_Data.Type == CCM_TYPE_ALUMNI)
	{
		// aluminum에서 조사
		if (m_Data.Data1.CodeName == _T("")) return TRUE;
		if (!IsExistInCodeList(aAluCodeList, m_Data.Data1.CodeName))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Alum_code__s__is_not_exis), m_Data.Data1.CodeName);
			AfxMessageBox(msg);
			m_Data.Data1.CodeName = _T("");
			return FALSE;
		}
	}
	else if (m_Data.Type == CCM_TYPE_TIMBER)
	{
		// aluminum에서 조사
		if (m_Data.Data1.CodeName == _T("")) return TRUE;
		if (!IsExistInCodeList(aTimberCodeList, m_Data.Data1.CodeName))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Timber_code__s__is_not_exis), m_Data.Data1.CodeName);
			AfxMessageBox(msg);
			m_Data.Data1.CodeName = _T("");
			return FALSE;
		}
	}
	else if (m_Data.Type == CCM_TYPE_SRC)
	{
		// First는 steel, second는 concrete 조사
		if (m_Data.Data1.CodeName != _T("") && !IsExistInCodeList(aSteelCodeList, m_Data.Data1.CodeName))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Steel_code__s__is_not_exi), m_Data.Data1.CodeName);
			AfxMessageBox(msg);
			m_Data.Data1.CodeName = _T("");
			return FALSE;
		}
		if (m_Data.Data2.CodeName != _T("") && !IsExistInCodeList(aConcrCodeList, m_Data.Data2.CodeName))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Concrete_code__s__is_not_), m_Data.Data2.CodeName);
			AfxMessageBox(msg);
			m_Data.Data2.CodeName = _T("");
			return FALSE;
		}
	}
	return TRUE;
}

void CCMMatPageItemDlg::SetUnitType()
{
	m_wndIsoStlEla.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndIsoStlPoi.SetUnitType(CUnitCtrl::m_MATL_UNIT.Poisson);
	m_wndIsoStlThr.SetUnitType(CUnitCtrl::m_MATL_UNIT.Thermal);
	m_wndIsoStlDen.SetUnitType(CUnitCtrl::m_MATL_UNIT.Density);
	m_wndIsoStlMDen.SetUnitType(CUnitCtrl::m_MATL_UNIT.MassDensity);
	m_wndIsoConEla.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndIsoConPoi.SetUnitType(CUnitCtrl::m_MATL_UNIT.Poisson);
	m_wndIsoConThr.SetUnitType(CUnitCtrl::m_MATL_UNIT.Thermal);
	m_wndIsoConDen.SetUnitType(CUnitCtrl::m_MATL_UNIT.Density);
	m_wndIsoConMDen.SetUnitType(CUnitCtrl::m_MATL_UNIT.MassDensity);
	m_wndOrtElaLx.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndOrtElaLy.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndOrtElaLz.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndOrtThrLx.SetUnitType(CUnitCtrl::m_MATL_UNIT.Thermal);
	m_wndOrtThrLy.SetUnitType(CUnitCtrl::m_MATL_UNIT.Thermal);
	m_wndOrtThrLz.SetUnitType(CUnitCtrl::m_MATL_UNIT.Thermal);
	m_wndOrtPoiLxy.SetUnitType(CUnitCtrl::m_MATL_UNIT.Poisson);
	m_wndOrtPoiLxz.SetUnitType(CUnitCtrl::m_MATL_UNIT.Poisson);
	m_wndOrtPoiLyz.SetUnitType(CUnitCtrl::m_MATL_UNIT.Poisson);
	m_wndOrtShrLxy.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndOrtShrLxz.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndOrtShrLyz.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndOrtDen.SetUnitType(CUnitCtrl::m_MATL_UNIT.Density);
	m_wndOrtMDen.SetUnitType(CUnitCtrl::m_MATL_UNIT.MassDensity);

	m_wndIsoStlElaUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndIsoStlDenUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Density);
	m_wndIsoStlMDenUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.MassDensity);
	m_wndIsoStlPoiUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Poisson);
	m_wndIsoStlThrUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Thermal);

	m_wndIsoConElaUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndIsoConDenUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Density);
	m_wndIsoConMDenUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.MassDensity);
	m_wndIsoConPoiUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Poisson);
	m_wndIsoConThrUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Thermal);

	m_wndSpHeat.SetUnitType(CUnitCtrl::m_MATL_UNIT.dSpecificHeat);
	m_wndHeatCo.SetUnitType(CUnitCtrl::m_MATL_UNIT.dHeatConduction);
	m_wndUnitSpHeat.SetUnitType(CUnitCtrl::m_MATL_UNIT.dSpecificHeat);
	m_wndUnitHeatCo.SetUnitType(CUnitCtrl::m_MATL_UNIT.dHeatConduction);

	m_wndOrtElaUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndOrtPoiUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Poisson);
	m_wndOrtThrUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Thermal);
	m_wndOrtShrUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.ShearModulus);
	m_wndOrtDenUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.Density);
	m_wndOrtMDenUnit.SetUnitType(CUnitCtrl::m_MATL_UNIT.MassDensity);

	m_edtDampingRatio.SetUnitType(D_UNITSYS_NONE);
}

void CCMMatPageItemDlg::SetTypeCombo()
{
	// MNET:XXXX-HSSHIM-20131030. 알루미늄 재질을 국문 Gen에만 열어줌.
	// 알루미늄 재질을 중문 Gen에도 열어줌 JWKWON-2017-05-23
	BOOL bHideAluminum = TRUE;
#if defined(_MGEN_ORG) || defined(_MGEN_CH) || defined(_MGEN_US) || defined(_MGEN_RUS)
	bHideAluminum = FALSE;
#endif

	m_wndType.ResetContent();
#if defined(_MGEN) || defined(_CIVIL)
	CDlgUtil::CobxAddItem(m_wndType, CCM_STEEL    , CCM_TYPE_STEEL_I);
	CDlgUtil::CobxAddItem(m_wndType, CCM_CONCR    , CCM_TYPE_CONCR_I);
	CDlgUtil::CobxAddItem(m_wndType, CCM_SRC      , CCM_TYPE_SRC_I  );
	if (!bHideAluminum)
	{
		CDlgUtil::CobxAddItem(m_wndType, CCM_ALUMINIUM, CCM_TYPE_ALUMI_I);
		CDlgUtil::CobxAddItem(m_wndType, CCM_TIMBER, CCM_TYPE_TIMBER_I);
	}
	CDlgUtil::CobxAddItem(m_wndType, CCM_USER     , CCM_TYPE_USER_I );
//   m_wndType.AddString(CCM_STEEL    );
//   m_wndType.AddString(CCM_CONCR    );
//   m_wndType.AddString(CCM_SRC      );
//   m_wndType.AddString(CCM_ALUMINIUM);
//   m_wndType.AddString(CCM_USER     );
#elif defined(_TOWER)
	CDlgUtil::CobxAddItem(m_wndType, CCM_STEEL    , CCM_TYPE_STEEL_I);
	CDlgUtil::CobxAddItem(m_wndType, CCM_USER     , CCM_TYPE_USER_I );
//   m_wndType.AddString(CCM_STEEL);
//   m_wndType.AddString(CCM_USER);
#endif
	CDlgUtil::CobxSetCurSelItemData(m_wndType, CCM_TYPE_STEEL_I);
	//m_wndType.SetCurSel(0); // DEFAULT : steel
}

void CCMMatPageItemDlg::SetInelasticCombo()
{
	m_cmbInelasConc.ResetContent();
	m_cmbInelasCoverConc.ResetContent();
	m_cmbInelasStl.ResetContent();
	
	CDlgUtil::CobxAddItem(m_cmbInelasConc, _LS(IDS_WG_CMD__ADDD__None), 0);
	CDlgUtil::CobxAddItem(m_cmbInelasCoverConc, _LS(IDS_WG_CMD__ADDD__None), 0);
	CDlgUtil::CobxAddItem(m_cmbInelasStl , _LS(IDS_WG_CMD__ADDD__None), 0);
	
	T_FIMP_D FimpD;
	CArray<T_FIMP_K, T_FIMP_K> aFimpK;
	m_pDoc->m_pAttrCtrl->GetFimpKeyList(aFimpK);
	for (int i=0; i < aFimpK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetFimp(aFimpK[i], FimpD)) { ASSERT(0); continue; }
		if     (FimpD.nMaterialType == 0)
		{
			CDlgUtil::CobxAddItem(m_cmbInelasConc, FimpD.strName, aFimpK[i]);
			CDlgUtil::CobxAddItem(m_cmbInelasCoverConc, FimpD.strName, aFimpK[i]);
		}
		else if(FimpD.nMaterialType == 1)
		{
			CDlgUtil::CobxAddItem(m_cmbInelasStl , FimpD.strName, aFimpK[i]);
		}
		else if(FimpD.nMaterialType == 2)//rebar
		{
			CDlgUtil::CobxAddItem(m_cmbInelasStl , FimpD.strName, aFimpK[i]);
		}
	}
	CDlgUtil::CobxSetCurSelItemData(m_cmbInelasConc, 0);
	CDlgUtil::CobxSetCurSelItemData(m_cmbInelasCoverConc, 0);
	CDlgUtil::CobxSetCurSelItemData(m_cmbInelasStl , 0);
}

void CCMMatPageItemDlg::ChangeBitmap(int nBitmap)
{
#if defined(_TOWER)
	return;
#endif
	CString aImgPath_Matl[6] = {
	_T("SVG\\illustration\\Dialog\\cmd_matl1.svg"),
	_T("SVG\\illustration\\Dialog\\cmd_matl2.svg"),
	_T("SVG\\illustration\\Dialog\\cmd_matl3.svg"),
	_T("SVG\\illustration\\Dialog\\cmd_matl5.svg"),
	_T("SVG\\illustration\\Dialog\\cmd_matl6.svg"),
	_T("SVG\\illustration\\Dialog\\cmd_matl4.svg"),
	};

	// 	if (m_pBitmap != 0) delete m_pBitmap;
	// 	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_PICTURE);
	// 	m_pBitmap = new CBitmap;
	// 	m_pBitmap->LoadBitmap(aBitmapID[nBitmap]);
	// 	pImage->SetBitmap(HBITMAP(*m_pBitmap));

	m_ImgMatl.SetImage(aImgPath_Matl[nBitmap]);
}

void CCMMatPageItemDlg::SetInelasticMatlCombo()
{
	int nTypeIndex = GetTypeIndex(m_Data.Type);

	T_IMFM_D ImfmD;
	if(m_pDoc->m_pAttrCtrl2->GetImfm(m_Key, ImfmD))
	{
		if(nTypeIndex==CCM_TYPE_STEEL_I || nTypeIndex==CCM_TYPE_SRC_I)
		{
			CDlgUtil::CobxSetCurSelItemData(m_cmbInelasConc, ImfmD.ConcK);
			CDlgUtil::CobxSetCurSelItemData(m_cmbInelasCoverConc, ImfmD.Confined_ConcK);
			CDlgUtil::CobxSetCurSelItemData(m_cmbInelasStl , ImfmD.SteelK);
		}
		else if(nTypeIndex==CCM_TYPE_CONCR_I)
		{
			CDlgUtil::CobxSetCurSelItemData(m_cmbInelasConc, ImfmD.ConcK);
			CDlgUtil::CobxSetCurSelItemData(m_cmbInelasCoverConc, ImfmD.Confined_ConcK);
			CDlgUtil::CobxSetCurSelItemData(m_cmbInelasStl , ImfmD.RebarK);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
void CCMMatPageItemDlg::ShowDataToDlg()
{
	CString csValue;
	csValue.Format(_T("%d"), m_Key);   // T_MATL_K m_Key;
	m_wndID.SetWindowText(csValue); 
	m_wndName.SetWindowText(m_Data.Name);

	//m_wndType.SetCurSel(GetTypeIndex(m_Data.Type));
	CDlgUtil::CobxSetCurSelItemData(m_wndType, GetTypeIndex(m_Data.Type));
	
	// *^^*
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlThermalSteel, m_Data.nThermalUnit);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlThermalConc, m_Data.nThermalUnit);

	m_wndMatlTypeRdo = m_Data.Data1.Analysis.nIsoOrtho;

	CButton* IsoBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO1);  
	CButton* OrtBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO2);  
	if(m_wndMatlTypeRdo == 0)      IsoBtn->GetCheck();  
	else if(m_wndMatlTypeRdo == 1) OrtBtn->GetCheck();
	else ASSERT(0);

	// Change enable/disable state of ctrls, contents of combo, 
	// and init combo selection.
	ChangeDlgCtrls();  
	
	// *^^*
	m_wndIsoConMDenChk.SetCheck((int)m_Data.bUseMassDensity);
	m_wndIsoStlMDenChk.SetCheck((int)m_Data.bUseMassDensity);
	m_wndOrtMDenChk.SetCheck((int)m_Data.bUseMassDensity);
	EnableDisableMDensity(m_Data.bUseMassDensity);
	m_wndOrtMDen.EnableWindow(m_Data.bUseMassDensity);

	int nTypeIndex = GetTypeIndex(m_Data.Type);
	CString csSteelCode, csConcrCode;
	CString csProductCode;

	CComboBox* pCode = NULL;
	CComboBox* pSubCode = NULL;
	CComboBox* pName = NULL;

	if (nTypeIndex == CCM_TYPE_CONCR_I) // Concrete
	{
		pCode = &m_wndConcrCode; 
		pSubCode = &m_wndConcrSubCode;
		pName = &m_wndConcrName;
		csSteelCode = _T(""); 
		csConcrCode = m_Data.Data1.CodeName;    
	}
	else
	{
		pCode = &m_wndSteelCode; 
		pSubCode = NULL;
		pName = &m_wndSteelName;
		csSteelCode = m_Data.Data1.CodeName;
		if(nTypeIndex == CCM_TYPE_ALUMI_I) 
		{
			csProductCode = m_Data.Data1.SubCodeName;
		}
		if (nTypeIndex == CCM_TYPE_SRC_I) csConcrCode = m_Data.Data2.CodeName;
		else csConcrCode = _T("");  // Steel, User
	}
	// change contents of name combo
	SetNameCombo(&m_wndSteelName, m_Data.Type, csSteelCode);
	SetNameCombo(&m_wndConcrName, m_Data.Type, csConcrCode);
	SetProductCombo(&m_wndProductName, m_Data.Type, m_Data.Data1.CodeName, m_Data.Data1.CodeMatlName, csProductCode);
	SetInelasticMatlCombo();
	// Thermal

//   /** DB에서는 온도를 Celsius 단위로 저장하기 때문에 필요하면 Fahrenheit로 변환한다.  */
//   CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
//   if (pUnitCtrl->GetBaseTemperUnitType() == D_UNITSYS_TEMPER_INDEX_C)              // case Celsius
//   {
//     m_wndSpHeat.SetEditUnit(m_Data.dSpecificHeat);
//     m_wndHeatCo.SetEditUnit(m_Data.dHeatConduction);
//   }
//   else if (pUnitCtrl->GetBaseTemperUnitType() == D_UNITSYS_TEMPER_INDEX_F)         // case Fahrenheit 
//   {
//     m_wndSpHeat.SetEditUnit( m_Data.dSpecificHeat / pUnitCtrl->GetThermalTransCoeff(FALSE) );     // Celsius -> Fahrenheit
//     m_wndHeatCo.SetEditUnit(m_Data.dHeatConduction / pUnitCtrl->GetThermalTransCoeff(FALSE));     // Celsius -> Fahrenheit
//   }

	m_wndSpHeat.SetEditUnit(m_Data.dSpecificHeat);
	m_wndHeatCo.SetEditUnit(m_Data.dHeatConduction);

	// Plastic material
	m_wndPlasticCmb.ChangeSelect(m_Data.KeyEpmt);  

	// Data1
	if (m_Data.Data1.CodeName == _T("")) pCode->SetCurSel(0);
	else 
	{
		CString CodeName;
		CodeName = m_Data.Data1.CodeName;
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
		pCode->SelectString(-1, CodeName);
		if(pSubCode)
			pSubCode->SelectString(-1, m_Data.Data1.SubCodeName);

		CString CodeMatlName;
		CodeMatlName = m_Data.Data1.CodeMatlName;
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(m_Data.Data1.CodeName, CodeMatlName);
#endif
		int iIndex = pName->FindStringExact(-1, CodeMatlName);
		pName->SetCurSel(iIndex);
	}
	if (nTypeIndex == CCM_TYPE_CONCR_I) SetAnalData(CCM_TYPE_CONCR_I, m_Data.Data1.Analysis, m_Data.Data1.CodeName);
	else SetAnalData(CCM_TYPE_STEEL_I, m_Data.Data1.Analysis, m_Data.Data1.CodeName);

	m_edtDampingRatio.SetEditUnit(m_Data.dDampingRatio);

	if(nTypeIndex == CCM_TYPE_STEEL_I)
	{
		m_wndIsoStlUserElaChk.SetCheck((int)m_Data.Data1.Analysis.bUserElast);
	}
	else if(nTypeIndex == CCM_TYPE_CONCR_I)
	{
		m_wndIsoConUserElaChk.SetCheck((int)m_Data.Data1.Analysis.bUserElast);
	}
	else if(nTypeIndex == CCM_TYPE_SRC_I)
	{
		m_wndIsoStlUserElaChk.SetCheck((int)m_Data.Data1.Analysis.bUserElast);
		m_wndIsoConUserElaChk.SetCheck((int)m_Data.Data2.Analysis.bUserElast);
	}

	// user type이면
	if (nTypeIndex == CCM_TYPE_STEEL_I || nTypeIndex == CCM_TYPE_USER_I || nTypeIndex == CCM_TYPE_ALUMI_I || nTypeIndex == CCM_TYPE_TIMBER_I)
	{
		m_wndConcrCode.SetCurSel(0);
		SetAnalData(CCM_TYPE_CONCR_I, m_Data.Data2.Analysis, m_Data.Data2.CodeName);
		return; // SRC가 아니면 Data2가 출력될 필요 없다.  
	}
	else if (nTypeIndex == CCM_TYPE_CONCR_I)  // concrete이면 steel value 초기화
	{
		m_wndSteelCode.SetCurSel(0);
		SetAnalData(CCM_TYPE_STEEL_I, m_Data.Data2.Analysis, m_Data.Data2.CodeName);
		return; // SRC가 아니면 Data2가 출력될 필요 없다.
	}

	// Data2
	if (m_Data.Data2.CodeName == _T("")) m_wndConcrCode.SetCurSel(0);
	else 
	{
		CString CodeName, CodeMatlName;
		CodeName = m_Data.Data2.CodeName;
		CodeMatlName = m_Data.Data2.CodeMatlName;
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(m_Data.Data2.CodeName, CodeMatlName);
#endif

		m_wndConcrCode.SelectString(-1, CodeName);
		m_wndConcrSubCode.SelectString(-1, m_Data.Data2.SubCodeName);
		m_wndConcrName.SelectString(-1, CodeMatlName);
	}
	SetAnalData(CCM_TYPE_CONCR_I, m_Data.Data2.Analysis, m_Data.Data2.CodeName);
}

void CCMMatPageItemDlg::ChangeDlgCtrls()
{  
	BOOL bSrc = FALSE;
	int nTypeIndex = GetTypeIndex(m_Data.Type);
	CButton* IsoBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO1);  
	CButton* OrtBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO2);  
		
	SetFrameName(nTypeIndex);
	SetSteelCode(nTypeIndex);
	SetConcCode(nTypeIndex);
	
	CString csStlCode, csConcCode;
	m_wndSteelCode.GetWindowText(csStlCode);
	m_wndConcrCode.GetWindowText(csConcCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csConcCode);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csStlCode);
#endif
	m_wndMatlTypeRdo = m_Data.Data1.Analysis.nIsoOrtho;

	// material type(Isotropic, Orthotropic)에 따라 해당하는 전체 콘드롤을 보이거나 숨긴다.
	if (/*!m_wndIsoStlFrame.GetFoldState() &&*/ !m_wndIsoConFrame.GetFoldState())
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlIso, m_wndMatlTypeRdo == 0);
	}
	CDlgUtil::CtrlShowHide(this, m_aCtrlOrt, m_wndMatlTypeRdo == 1);
	EnableDisableUserEla();
	EnableShowOnlyStrand();

#if defined(_MGEN) && defined(_US)
	GetDlgItem(IDC_CMD_FRAME11)->SetWindowText(_LS(IDS_CMD_Inelastic_Fiber_And_ND));
#endif

	BOOL bMassStl, bMassCon;
	BOOL bThermalStl, bThermalCon;
	if(nTypeIndex == CCM_TYPE_STEEL_I) // Steel
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlStlCode, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlConCode, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoStlAnal, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoConAnal, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoIso, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoOrt, m_Data.Data1.CodeName == _T(""));
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlOrtAnal, m_Data.Data1.CodeName == _T(""));

		bThermalStl = (m_Data.Data1.CodeName != _T(""));
		bThermalCon = FALSE;
		bMassStl = TRUE;
		bMassCon = FALSE;
		m_wndProductName.EnableWindow(FALSE);

		CDlgUtil::CobxSetCurSelItemData(m_cmbInelasConc, 0);
		CDlgUtil::CobxSetCurSelItemData(m_cmbInelasCoverConc, 0);
		m_cmbInelasConc.EnableWindow(FALSE);
		m_cmbInelasCoverConc.EnableWindow(FALSE);
		m_cmbInelasStl.EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_INELAS_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_INELAS_STL_STC)->SetWindowText(_LS(IDS_CMD_MATL_STEEL));
	}
	else if(nTypeIndex == CCM_TYPE_USER_I) // User
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlStlCode, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlConCode, FALSE);    
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoStlAnal, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoConAnal, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoIso, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoOrt, m_Data.Data1.CodeName == _T(""));
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlOrtAnal, m_Data.Data1.CodeName == _T(""));

		bThermalStl = (m_Data.Data1.CodeName != _T(""));
		bThermalCon = FALSE;
		bMassStl = TRUE;
		bMassCon = FALSE;
		m_wndProductName.EnableWindow(FALSE);
		m_wndOnlyStrandChk.EnableWindow(FALSE);

		CDlgUtil::CobxSetCurSelItemData(m_cmbInelasConc, 0);
		CDlgUtil::CobxSetCurSelItemData(m_cmbInelasCoverConc, 0);
		CDlgUtil::CobxSetCurSelItemData(m_cmbInelasStl , 0);
		m_cmbInelasConc.EnableWindow(FALSE);
		m_cmbInelasCoverConc.EnableWindow(FALSE);
		m_cmbInelasStl.EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_INELAS_BTN)->EnableWindow(FALSE);
	}    
	else if(nTypeIndex == CCM_TYPE_ALUMI_I)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlStlCode, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlConCode, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoStlAnal, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoConAnal, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoIso, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoOrt, m_Data.Data1.CodeName == _T(""));
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlOrtAnal, m_Data.Data1.CodeName == _T(""));

		bThermalStl = (m_Data.Data1.CodeName != _T(""));
		bThermalCon = FALSE;
		bMassStl = TRUE;
		bMassCon = FALSE;
		m_wndProductName.EnableWindow(FALSE);
		m_wndOnlyStrandChk.EnableWindow(FALSE);

		CDlgUtil::CobxSetCurSelItemData(m_cmbInelasConc, 0);
		CDlgUtil::CobxSetCurSelItemData(m_cmbInelasCoverConc, 0);
		CDlgUtil::CobxSetCurSelItemData(m_cmbInelasStl , 0);
		m_cmbInelasConc.EnableWindow(FALSE);
		m_cmbInelasCoverConc.EnableWindow(FALSE);
		m_cmbInelasStl.EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_INELAS_BTN)->EnableWindow(FALSE);
	}
	else if(nTypeIndex == CCM_TYPE_CONCR_I) // Concrete
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlStlCode, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlConCode, TRUE);    
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoStlAnal, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoConAnal, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoIso, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoOrt, m_Data.Data1.CodeName == _T(""));
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlOrtAnal, m_Data.Data1.CodeName == _T(""));

		bThermalStl = FALSE;
		bThermalCon = (m_Data.Data1.CodeName != _T(""));
		bMassStl = FALSE;
		bMassCon = TRUE;
		m_wndProductName.EnableWindow(FALSE);

		m_cmbInelasConc.EnableWindow(TRUE);
		m_cmbInelasCoverConc.EnableWindow(TRUE);
		m_cmbInelasStl.EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_INELAS_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_INELAS_STL_STC)->SetWindowText(_LS(IDS_CMD_MATL_REBAR));
	}
	else if(nTypeIndex == CCM_TYPE_SRC_I) // SRC
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlStlCode, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlConCode, TRUE);    
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoStlAnal, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoConAnal, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoOrt, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlOrtAnal, FALSE);
		bSrc = TRUE;

		bThermalStl = bMassStl = (m_Data.Data1.CodeName != _T(""));
		bThermalCon = bMassCon = (m_Data.Data2.CodeName != _T(""));
		bMassStl = TRUE;
		bMassCon = TRUE;
		m_wndProductName.EnableWindow(FALSE);
		
		m_cmbInelasConc.EnableWindow(FALSE);
		m_cmbInelasCoverConc.EnableWindow(FALSE);
		m_cmbInelasStl.EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_INELAS_BTN)->EnableWindow(FALSE);
	}  
	else if (nTypeIndex == CCM_TYPE_TIMBER_I) // Timber
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlStlCode, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlConCode, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoStlAnal, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoConAnal, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoIso, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoOrt, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlOrtAnal, m_Data.Data1.CodeName == _T(""));

		bThermalStl = FALSE;
		bThermalCon = FALSE;
		bMassStl = FALSE;
		bMassCon = FALSE;
		m_wndProductName.EnableWindow(FALSE);

		CDlgUtil::CobxSetCurSelItemData(m_cmbInelasConc, 0);
		CDlgUtil::CobxSetCurSelItemData(m_cmbInelasCoverConc, 0);
		CDlgUtil::CobxSetCurSelItemData(m_cmbInelasStl, 0);
		m_cmbInelasConc.EnableWindow(FALSE);
		m_cmbInelasCoverConc.EnableWindow(FALSE);
		m_cmbInelasStl.EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_INELAS_BTN)->EnableWindow(FALSE);
		}
	else ASSERT(0);

	if((nTypeIndex == CCM_TYPE_STEEL_I && m_Data.Data1.CodeName != _T("")) || 
		 (nTypeIndex == CCM_TYPE_CONCR_I && m_Data.Data1.CodeName != _T("")) ||      
		 (nTypeIndex == CCM_TYPE_USER_I  && m_Data.Data1.CodeName != _T("")) ||
		 (nTypeIndex == CCM_TYPE_ALUMI_I && m_Data.Data1.CodeName != _T("")) ||
		 (nTypeIndex == CCM_TYPE_SRC_I) )
	{     
		IsoBtn->SetCheck(TRUE);
		OrtBtn->SetCheck(FALSE);    
		m_Data.Data1.Analysis.nIsoOrtho = 0;  
		m_wndPlasticCmb.EnableWindow(TRUE);
	}
	else if (nTypeIndex == CCM_TYPE_TIMBER_I)
	{
		IsoBtn->SetCheck(FALSE);
		OrtBtn->SetCheck(TRUE);
		m_Data.Data1.Analysis.nIsoOrtho = 1;
		m_wndPlasticCmb.EnableWindow(FALSE);
	}

	// *^^*
	m_wndIsoStlMDenChk.EnableWindow(bMassStl);
	m_wndIsoConMDenChk.EnableWindow(bMassCon);

	// User일때 섭씨, 화씨 없애기 - user일 때 섭씨로 Setting 한 후 Hide 시킨다.
	if (!bThermalStl) CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlThermalSteel, 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlThermalSteel, bThermalStl);
	if (!bThermalCon) CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlThermalConc, 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlThermalConc, bThermalCon);

	CDlgUtil::CtrlShowHide(this, m_aCtrlThermalSteel, bThermalStl);
	CDlgUtil::CtrlShowHide(this, m_aCtrlThermalConc, bThermalCon);

	// Concrete이면 data1은 concrete part에 출력
	// 이외에는 data1은 steel part에 출력
	CComboBox *pName1, *pName2;
	CArray<UINT, UINT> *paCtrlID1, *paCtrlID2;
	if(nTypeIndex == CCM_TYPE_CONCR_I)
	{
		pName1 = &m_wndConcrName;
		pName2 = &m_wndSteelName;
		paCtrlID1 = &m_aCtrlIsoConAnal;
		paCtrlID2 = &m_aCtrlIsoStlAnal;    
	}
	else 
	{
		pName1 = &m_wndSteelName;
		pName2 = &m_wndConcrName;
		paCtrlID1 = &m_aCtrlIsoStlAnal;
		paCtrlID2 = &m_aCtrlIsoConAnal;
	}

	// code 별 Name Enable/Disable
	if (m_Data.Data1.CodeName == _T(""))  pName1->EnableWindow(FALSE);
	else  CDlgUtil::CtrlEnableDisable(this, *paCtrlID1, FALSE); // Code 쓸 때는 Disable
	
	if (m_Data.Data2.CodeName == _T(""))  pName2->EnableWindow(FALSE);
	else  CDlgUtil::CtrlEnableDisable(this, *paCtrlID2, FALSE);

#ifdef _MGEN_CH
	GetDlgItem(IDC_CMD_MP_ITEM_ONLY_STRAND_CHK)->ShowWindow(SW_HIDE);
#endif

	UpdateData(FALSE);
}

int CCMMatPageItemDlg::GetTypeIndex(CString csType)
{
	if (csType == CCM_TYPE_STEEL) return CCM_TYPE_STEEL_I;
	if (csType == CCM_TYPE_CONCR) return CCM_TYPE_CONCR_I;
	if (csType == CCM_TYPE_SRC)   return CCM_TYPE_SRC_I;
	if (csType == CCM_TYPE_USER)   return CCM_TYPE_USER_I;
	if (csType == CCM_TYPE_ALUMNI)return CCM_TYPE_ALUMI_I;
	if (csType == CCM_TYPE_TIMBER)return CCM_TYPE_TIMBER_I;
	ASSERT(0);
	return 0;
}

CString CCMMatPageItemDlg::GetTypeCode(int nTypeIndex)
{
	if (nTypeIndex == CCM_TYPE_STEEL_I) return CCM_TYPE_STEEL;
	else if (nTypeIndex == CCM_TYPE_CONCR_I) return CCM_TYPE_CONCR;
	else if (nTypeIndex == CCM_TYPE_SRC_I)   return CCM_TYPE_SRC;
	else if (nTypeIndex == CCM_TYPE_USER_I)  return CCM_TYPE_USER;
	else if (nTypeIndex == CCM_TYPE_ALUMI_I) return CCM_TYPE_ALUMNI;
	else if (nTypeIndex == CCM_TYPE_TIMBER_I) return CCM_TYPE_TIMBER;
	else  ASSERT(0);
	
	return _T("");
}

void CCMMatPageItemDlg::SetFrameName(int nTypeIndex)
{
	CString strTitle = CCM_STEEL;
	CString strOrtTitle = CCM_STEEL;

	if(nTypeIndex == CCM_TYPE_CONCR_I) 
	{
		strOrtTitle = CCM_CONCR;
	}
	else if(nTypeIndex == CCM_TYPE_USER_I)
	{
		strTitle = CCM_USER;
		strOrtTitle = CCM_USER;
	}
	else if(nTypeIndex == CCM_TYPE_ALUMI_I)
	{
		strTitle = CCM_ALUMINIUM;
		strOrtTitle = CCM_ALUMINIUM;
	}
	else if (nTypeIndex == CCM_TYPE_TIMBER_I)
	{
		strTitle = CCM_TIMBER;
		strOrtTitle = CCM_TIMBER;

		GetDlgItem(IDC_CMD_MATL_ORT_LX_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_x));
		GetDlgItem(IDC_CMD_MATL_ORT_LY_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_y));
		GetDlgItem(IDC_CMD_MATL_ORT_LZ_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_z));
		GetDlgItem(IDC_CMD_MATL_ORT_LXY_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_xy));
		GetDlgItem(IDC_CMD_MATL_ORT_LXZ_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_xz));
		GetDlgItem(IDC_CMD_MATL_ORT_LYZ_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_yz));
	}

	if (nTypeIndex == CCM_TYPE_TIMBER_I)
	{
		GetDlgItem(IDC_CMD_MATL_ORT_LX_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_x));
		GetDlgItem(IDC_CMD_MATL_ORT_LY_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_y));
		GetDlgItem(IDC_CMD_MATL_ORT_LZ_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_z));
		GetDlgItem(IDC_CMD_MATL_ORT_LXY_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_xy));
		GetDlgItem(IDC_CMD_MATL_ORT_LXZ_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_xz));
		GetDlgItem(IDC_CMD_MATL_ORT_LYZ_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_Timber_yz));
	}
	else
	{
		GetDlgItem(IDC_CMD_MATL_ORT_LX_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_x));
		GetDlgItem(IDC_CMD_MATL_ORT_LY_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_y));
		GetDlgItem(IDC_CMD_MATL_ORT_LZ_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_z));
		GetDlgItem(IDC_CMD_MATL_ORT_LXY_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_xy));
		GetDlgItem(IDC_CMD_MATL_ORT_LXZ_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_xz));
		GetDlgItem(IDC_CMD_MATL_ORT_LYZ_TITLE)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Ortho_yz));
	}

	m_wndStlCodeFrame.SetWindowText(strTitle);
//   m_wndIsoStlFrame.SetWindowText(_T("    ") +strTitle); // m_wndIsoStlFrame.SetWindowText(strTitle);  // Folder Group Box
//   m_wndIsoStlFrame.Invalidate();    // 잔상문제 수정
	m_wndIsoStlFrame.SetWindowText(strTitle);
	m_wndOrtFrame.SetWindowText(strOrtTitle);
}

void CCMMatPageItemDlg::SetSteelCode(int nTypeIndex)  // steel or user defined
{
	// reset contents of ComboBox
	if (nTypeIndex == CCM_TYPE_STEEL_I) SetCode(&m_wndSteelCode, CCM_TYPE_STEEL);
	else if (nTypeIndex == CCM_TYPE_SRC_I) SetCode(&m_wndSteelCode, CCM_TYPE_STEEL, TRUE);
	else if (nTypeIndex == CCM_TYPE_USER_I) SetCode(&m_wndSteelCode, CCM_TYPE_USER);
	else if (nTypeIndex == CCM_TYPE_ALUMI_I) SetCode(&m_wndSteelCode, CCM_TYPE_ALUMNI);
	else if (nTypeIndex == CCM_TYPE_TIMBER_I) SetCode(&m_wndSteelCode, CCM_TYPE_TIMBER);

	else if (nTypeIndex == CCM_TYPE_CONCR_I) m_wndSteelCode.ResetContent();
}

void CCMMatPageItemDlg::SetConcCode(int nTypeIndex)  // steel or user defined
{
	// reset contents of ComboBox
	if (nTypeIndex == CCM_TYPE_CONCR_I) SetCode(&m_wndConcrCode, CCM_TYPE_CONCR);
	else if (nTypeIndex == CCM_TYPE_SRC_I) SetCode(&m_wndConcrCode, CCM_TYPE_CONCR, TRUE);
	else if (nTypeIndex == CCM_TYPE_USER_I) m_wndConcrCode.ResetContent();
	else if (nTypeIndex == CCM_TYPE_ALUMI_I) m_wndConcrCode.ResetContent();
	else if (nTypeIndex == CCM_TYPE_STEEL_I) m_wndConcrCode.ResetContent();
	else if (nTypeIndex == CCM_TYPE_TIMBER_I) m_wndConcrCode.ResetContent();
	else ASSERT(0);
}

void CCMMatPageItemDlg::SetProductCombo(CComboBox* pCombo, CString csType, CString csCodeName, CString csMatlName, CString csProd)
{
	pCombo->ResetContent();  
	int CheckSteelName = m_wndSteelName.GetCurSel();

	if(csType == CCM_TYPE_ALUMNI && CheckSteelName >= 0)
		pCombo->EnableWindow(csCodeName==MATLCODE_ALU_AA ? TRUE : FALSE);
	else if(csType == CCM_TYPE_ALUMNI && csProd != _T("") && csProd.IsEmpty() == FALSE)
		pCombo->EnableWindow(TRUE);
	else
		pCombo->EnableWindow(FALSE);
	
	if(csCodeName == _T(""))
	{
	  pCombo->EnableWindow(FALSE);
	  return;
	}

	CArray<CString, CString&> aProductList;
	
	if(csType == CCM_TYPE_ALUMNI || (csType == CCM_TYPE_USER && csCodeName== MATLCODE_ALU_AA))
	{
		m_pDoc->m_pMatlDB->GetAluminumProductList(csCodeName, csMatlName, aProductList);
	}
	
	if(pCombo->IsWindowEnabled() != TRUE && m_bModify != TRUE && !(csType == CCM_TYPE_USER && csCodeName== MATLCODE_ALU_AA)) return;

	for(int i = 0; i < aProductList.GetSize(); i++)
	{
		pCombo->AddString(aProductList[i]);
		if(aProductList[i] == csProd)
		{
			pCombo->SetCurSel(i);
		}
	}
	if(pCombo->GetCurSel() == -1 && pCombo->GetCount() > 0)
			pCombo->SetCurSel(0);
}

void CCMMatPageItemDlg::SetCode(CComboBox* pCombo, CString csType, BOOL bSRC)
{
	// reset contents of ComboBox
	pCombo->ResetContent();
	// get design code by type
	CArray<CString, CString&> aDesignCodeList;
	m_pDoc->m_pMatlDB->GetDesignCodeList(csType, aDesignCodeList);
	pCombo->AddString(CCM_CODE_NONE);  

	int nSize = aDesignCodeList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(aDesignCodeList[i]);
#endif
		pCombo->AddString(aDesignCodeList[i]);
	}
}

void CCMMatPageItemDlg::SetSubCode(CComboBox* pCombo, CString csType, CString csCode)
{
	if(csCode.IsEmpty()) return;

	pCombo->ResetContent();
	
	CString csMatlType = _T("");
	if (csType == CCM_TYPE_USER || csType == CCM_TYPE_SRC || csType == CCM_TYPE_ALUMNI)
		m_pDoc->m_pMatlDB->GetMatlType(csCode, csMatlType);
	else csMatlType = csType;

	BOOL bUseSubCode = FALSE;
	if(csMatlType==CCM_TYPE_CONCR) // GetConcreteSubNameList
	{
		CArray <CString, CString&> aNameList;
		int nSize = m_pDoc->m_pMatlDB->GetConcreteSubNameList(csCode, aNameList);
		for(int i=0; i<nSize; ++i)
		{
			pCombo->AddString(aNameList[i]);
		}
		if(nSize>0) bUseSubCode = TRUE;
	}
	if(!bUseSubCode)
	{
		m_Data.Data1.SubCodeName = m_Data.Data2.SubCodeName = _T("");
	}
}

void CCMMatPageItemDlg::SetNameCombo(CComboBox* pCombo, CString &csType, CString &csCode)
{
	// reset contents of ComboBox
	pCombo->ResetContent();
	// get material name list by material type
	// first get material type
	CString csMatlType;
	if (csType == CCM_TYPE_USER || csType == CCM_TYPE_SRC || csType == CCM_TYPE_ALUMNI)
		m_pDoc->m_pMatlDB->GetMatlType(csCode, csMatlType);
	else csMatlType = csType;

	BOOL bUseSubCode = FALSE;
	if(csType != CCM_TYPE_USER && csMatlType == CCM_TYPE_CONCR)
	{
		if(csCode==_LSX(KS01(RC)) || csCode==_LSX(KS01-Civil(RC))) bUseSubCode = TRUE;
		SetSubCode(&m_wndConcrSubCode, csType, csCode);
	}
	GetDlgItem(IDC_CMD_MP_ITEM_CODE3)->EnableWindow(bUseSubCode);

	CArray <CString, CString&> aNameList;
	// next get material name list
	if (csMatlType == CCM_TYPE_STEEL)  // steel
	{
		auto bOnlyStrand = m_wndOnlyStrandChk.GetCheck();
		if (bOnlyStrand)
		{
			m_pDoc->m_pMatlDB->GetStrandNameList(csCode, aNameList);
		}
		else
		{
			m_pDoc->m_pMatlDB->GetSteelNameList(csCode, aNameList);
		}
	}
	else if (csMatlType == CCM_TYPE_CONCR) // concrete
	{
		m_pDoc->m_pMatlDB->GetConcreteNameList(csCode, aNameList);
	}
	else if (csMatlType == CCM_TYPE_ALUMNI) // Aluminum
	{
		m_pDoc->m_pMatlDB->GetAluminumNameList(csCode, aNameList);
	}
	else if (csMatlType == CCM_TYPE_TIMBER) // Timber
	{
		m_pDoc->m_pMatlDB->GetTimberNameList(csCode, aNameList);
	}
	else return;

	int nSize = aNameList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(csCode, aNameList[i]);
#endif
		pCombo->AddString(aNameList[i]);
	}
}

void CCMMatPageItemDlg::GetSteelData(CString csCode, CString csName, int nThermalUnit, T_MATL_ANALYSIS &AnalData)
{
	T_MATL_STEEL SteelData;
	if (m_pDoc->m_pMatlDB->GetSteelData(csCode, csName, SteelData))
	{
		AnalData.Elast = SteelData.Elast;
		AnalData.Poisson = SteelData.Poisson;
		AnalData.Thermal = SteelData.Thermal;
		AnalData.Density = SteelData.Density;  
		AnalData.MassDensity = SteelData.MassDensity;  
		AnalData.Elast_y = 0.;
		AnalData.Poisson_y = 0.;
		AnalData.Thermal_y = 0.;
		AnalData.Elast_z = 0.;
		AnalData.Poisson_z = 0.;
		AnalData.Thermal_z = 0.;
		AnalData.ShearModulus = 0.;
		AnalData.ShearModulus_y = 0.;
		AnalData.ShearModulus_z = 0.;
		AnalData.nIsoOrtho = 0;

//     int nOption;
//     CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlThermalSteel, nOption);
//     if (nOption == 1) ConvertFahrCelsins(1, AnalData.Thermal);

		/** DB에서는 온도를 Celsius 단위로 저장하기 때문에 필요하면 Fahrenheit로 변환한다.  */
//     CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
//     if (pUnitCtrl->GetBaseTemperUnitType() == D_UNITSYS_TEMPER_INDEX_F)         // case Fahrenheit 
//     {
//       AnalData.Thermal = AnalData.Thermal / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
//       AnalData.Thermal_y = AnalData.Thermal_y / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
//       AnalData.Thermal_z = AnalData.Thermal_z / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
//     }
	}
	else AnalData.Initialize();
}

void CCMMatPageItemDlg::GetConcData(CString csCode, CString csSubCode, CString csName, int nThermalUnit, T_MATL_ANALYSIS &AnalData)
{
	T_MATL_CONCRETE concData;
	if (m_pDoc->m_pMatlDB->GetConcreteDataNew(csCode, csSubCode, csName, concData))
	{
		AnalData.Elast = concData.Elast;
		AnalData.Poisson = concData.Poisson;
		AnalData.Thermal = concData.Thermal;
		AnalData.Density = concData.Density;
		AnalData.MassDensity = concData.MassDensity;
		AnalData.Elast_y = 0.;
		AnalData.Poisson_y = 0.;
		AnalData.Thermal_y = 0.;
		AnalData.Elast_z = 0.;
		AnalData.Poisson_z = 0.;
		AnalData.Thermal_z = 0.;
		AnalData.ShearModulus = 0.;
		AnalData.ShearModulus_y = 0.;
		AnalData.ShearModulus_z = 0.;
		AnalData.nIsoOrtho = 0;

//     int nOption;
//     CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlThermalConc, nOption);
//     if (nOption == 1) ConvertFahrCelsins(1, AnalData.Thermal);

		/** DB에서는 온도를 Celsius 단위로 저장하기 때문에 필요하면 Fahrenheit로 변환한다.  */
//     CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
//     if (pUnitCtrl->GetBaseTemperUnitType() == D_UNITSYS_TEMPER_INDEX_F)         // case Fahrenheit 
//     {
//       AnalData.Thermal = AnalData.Thermal / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
//       AnalData.Thermal_y = AnalData.Thermal_y / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
//       AnalData.Thermal_z = AnalData.Thermal_z / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
//     }
	}
	else AnalData.Initialize();
}

void CCMMatPageItemDlg::GetAlumiData(CString csCode, CString csName, CString csProd, int nThermalUnit, T_MATL_ANALYSIS &AnalData)
{
	T_MATL_ALUMINUM AluminumData;
	if (m_pDoc->m_pMatlDB->GetAluminumData(csCode, csName, csProd, AluminumData))
	{
		AnalData.Elast = AluminumData.Elast;
		AnalData.Poisson = AluminumData.Poisson;
		AnalData.Thermal = AluminumData.Thermal;
		AnalData.Density = AluminumData.Density;
		AnalData.MassDensity = AluminumData.MassDensity;
		AnalData.Elast_y = 0.;
		AnalData.Poisson_y = 0.;
		AnalData.Thermal_y = 0.;
		AnalData.Elast_z = 0.;
		AnalData.Poisson_z = 0.;
		AnalData.Thermal_z = 0.;
		AnalData.ShearModulus = 0.;
		AnalData.ShearModulus_y = 0.;
		AnalData.ShearModulus_z = 0.;
		AnalData.nIsoOrtho = 0;
		
		//     int nOption;
		//     CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlThermalConc, nOption);
		//     if (nOption == 1) ConvertFahrCelsins(1, AnalData.Thermal);
		
		/** DB에서는 온도를 Celsius 단위로 저장하기 때문에 필요하면 Fahrenheit로 변환한다.  */
		//     CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
		//     if (pUnitCtrl->GetBaseTemperUnitType() == D_UNITSYS_TEMPER_INDEX_F)         // case Fahrenheit 
		//     {
		//       AnalData.Thermal = AnalData.Thermal / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
		//       AnalData.Thermal_y = AnalData.Thermal_y / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
		//       AnalData.Thermal_z = AnalData.Thermal_z / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
		//     }
	}
	else AnalData.Initialize();
}

void CCMMatPageItemDlg::GetTimberData(CString csCode, CString csName, int nThermalUnit, T_MATL_ANALYSIS& AnalData)
{
	T_MATL_TIMBER TimberData;
	if (m_pDoc->m_pMatlDB->GetTimberData(csCode, csName, TimberData))
	{
		AnalData.Elast = TimberData.Elast_x;
		AnalData.Poisson = TimberData.Poisson_x;
		AnalData.Thermal = TimberData.Thermal_x;
		AnalData.Density = TimberData.Density;
		AnalData.MassDensity = TimberData.MassDensity;
		AnalData.Elast_y = TimberData.Elast_y;
		AnalData.Poisson_y = TimberData.Poisson_y;
		AnalData.Thermal_y = TimberData.Thermal_y;
		AnalData.Elast_z = TimberData.Elast_z;
		AnalData.Poisson_z = TimberData.Poisson_z;
		AnalData.Thermal_z = TimberData.Thermal_z;
		AnalData.ShearModulus = TimberData.ShearModulus_x;
		AnalData.ShearModulus_y = TimberData.ShearModulus_y;
		AnalData.ShearModulus_z = TimberData.ShearModulus_z;
		AnalData.nIsoOrtho = 1;
	}
	else AnalData.Initialize();
}

void CCMMatPageItemDlg::SetAnalData(int nTypeIndex, T_MATL_ANALYSIS &AnalData, CString &csCodeName)
{
	CArray<UINT, UINT> *paCtrlID;  
	CEditUnit* pEdit;
	CString csValue;
	int nSize, i;
	CString aIsoFormat[] = {_T("%.4e"), _T("%g"), _T("%.4e"), _T("%.4g"), _T("%.4g")};  
	CString aOrtFormat[] = {_T("%.4e"), _T("%.4e"), _T("%.4e"),   // Elasitcity
													_T("%.4e"), _T("%.4e"), _T("%.4e"),   // Thermal
													_T("%.4e"), _T("%.4e"), _T("%.4e"),   // Shear Modulus
													_T("%g"),   _T("%g"),   _T("%g"),     // Posiion                          
													_T("%g"), _T("%g")};              // Weight Density, Mass Density

//   /** DB에서는 온도를 Celsius 단위로 저장하기 때문에 필요하면 Fahrenheit로 변환한다.  */
//   CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
//   double dAnalDataThermal = AnalData.Thermal;
//   double dAnalDataThermal_y = AnalData.Thermal_y;
//   double dAnalDataThermal_z = AnalData.Thermal_z;
//   if (pUnitCtrl->GetBaseTemperUnitType() == D_UNITSYS_TEMPER_INDEX_F)         // case Fahrenheit 
//   {
//     dAnalDataThermal = AnalData.Thermal / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
//     dAnalDataThermal_y = AnalData.Thermal_y / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
//     dAnalDataThermal_z = AnalData.Thermal_z / pUnitCtrl->GetThermalTransCoeff(FALSE);     // Celsius -> Fahrenheit
//   }

	double* pIsoValue[] = {&AnalData.Elast,   &AnalData.Poisson,   &AnalData.Thermal, &AnalData.Density, &AnalData.MassDensity};
	double* pOrtValue[] = {&AnalData.Elast,   &AnalData.Elast_y,   &AnalData.Elast_z, 
												 &AnalData.Thermal, &AnalData.Thermal_y, &AnalData.Thermal_z,
												 &AnalData.ShearModulus, &AnalData.ShearModulus_y, &AnalData.ShearModulus_z, 
												 &AnalData.Poisson, &AnalData.Poisson_y, &AnalData.Poisson_z,                         
												 &AnalData.Density, &AnalData.MassDensity}; 

//   double* pIsoValue[] = {&AnalData.Elast,   &AnalData.Poisson,   &dAnalDataThermal, &AnalData.Density, &AnalData.MassDensity};
//   double* pOrtValue[] = {&AnalData.Elast,   &AnalData.Elast_y,   &AnalData.Elast_z, 
//                          &dAnalDataThermal, &dAnalDataThermal_y, &dAnalDataThermal_z,
//                          &AnalData.ShearModulus, &AnalData.ShearModulus_y, &AnalData.ShearModulus_z, 
//                          &AnalData.Poisson, &AnalData.Poisson_y, &AnalData.Poisson_z,                         
//                          &AnalData.Density, &AnalData.MassDensity}; 


	if(AnalData.nIsoOrtho == 0)
	{
		//if (m_Data.nThermalUnit == 1 && csCodeName != "")  // 화씨일때 -> 화씨로 변환 후 보여줌 
		//  ConvertFahrCelsins(1, AnalData.Thermal);

		if(nTypeIndex == CCM_TYPE_STEEL_I)      paCtrlID = &m_aCtrlIsoStlAnal;
		else if(nTypeIndex == CCM_TYPE_CONCR_I) paCtrlID = &m_aCtrlIsoConAnal;
		else ASSERT(0);    

		nSize = paCtrlID->GetSize();
		for(i = 0; i < nSize; i++)
		{
			pEdit = (CEditUnit*)GetDlgItem(paCtrlID->GetAt(i));
			ASSERT(pEdit && pEdit->GetSafeHwnd() && IsWindow(pEdit->GetSafeHwnd()));
			//csValue.Format(aIsoFormat[i], *pIsoValue[i]);
			//pEdit->SetWindowText(csValue);
			pEdit->SetTextFormat(aIsoFormat[i]);
			pEdit->SetEditUnit(*pIsoValue[i]);
		}
	}
	else if(AnalData.nIsoOrtho == 1)
	{
		paCtrlID = &m_aCtrlOrtAnal;    
		nSize = paCtrlID->GetSize();
		for(i = 0; i < nSize; i++)
		{
			pEdit = (CEditUnit*)GetDlgItem(paCtrlID->GetAt(i));
			ASSERT(pEdit && pEdit->GetSafeHwnd() && IsWindow(pEdit->GetSafeHwnd()));
			//csValue.Format(aOrtFormat[i], *pOrtValue[i]);
			//pEdit->SetWindowText(csValue);
			pEdit->SetTextFormat(aOrtFormat[i]);
			pEdit->SetEditUnit(*pOrtValue[i]);
		}
	}    
}

void CCMMatPageItemDlg::SetTherData(const CString& csCodeName)
{
	double dSpecificHeat = 0.0;
	double dHeatConduction = 0.0;
	if(csCodeName.Left(2) == _T("GB"))
	{
		dSpecificHeat = 60.0 /m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_MATL_UNIT.dSpecificHeat);
		dHeatConduction = 0.0;
	}
	CEditUnit* pEdit;
	pEdit = (CEditUnit*)GetDlgItem(IDC_CMD_MP_ITEM_THER_SPHEAT_E);
	pEdit->SetTextFormat(_T("%g"));
	pEdit->SetEditUnit(dSpecificHeat);
	pEdit = (CEditUnit*)GetDlgItem(IDC_CMD_MP_ITEM_THER_HEATCO_E);
	pEdit->SetTextFormat(_T("%g"));
	pEdit->SetEditUnit(dHeatConduction);
}

void CCMMatPageItemDlg::InitIsoAnalData()
{
	CArray<UINT, UINT> *paCtrlStlID;
	CArray<UINT, UINT> *paCtrlConID;
	paCtrlStlID = &m_aCtrlIsoStlAnal;
	paCtrlConID = &m_aCtrlIsoConAnal;

	CString csValue;
	CString aFormat[] = {_T("%.4e"), _T("%g"), _T("%.4e"), _T("%.4g"), _T("%.4g")};  
	CEditUnit* pEdit;
	double Value[] = {0, 0, 0, 0, 0};
		
	int nSize = paCtrlStlID->GetSize();
		
	for(int i = 0; i < nSize; i++)  
	{
		pEdit = (CEditUnit*)GetDlgItem(paCtrlStlID->GetAt(i));
		ASSERT(pEdit && pEdit->GetSafeHwnd() && IsWindow(pEdit->GetSafeHwnd()));
		//csValue.Format(aFormat[i], Value[i]);
		//pEdit->SetWindowText(csValue);
		pEdit->SetTextFormat(aFormat[i]);
		pEdit->SetEditUnit(Value[i]);


		pEdit = (CEditUnit*)GetDlgItem(paCtrlConID->GetAt(i));
		ASSERT(pEdit && pEdit->GetSafeHwnd() && IsWindow(pEdit->GetSafeHwnd()));
		//csValue.Format(aFormat[i], Value[i]);
		//pEdit->SetWindowText(csValue);
		pEdit->SetTextFormat(aFormat[i]);
		pEdit->SetEditUnit(Value[i]);
	}
}

void CCMMatPageItemDlg::InitOrtAnalData()
{
	CArray<UINT, UINT> *paCtrlID;
	paCtrlID = &m_aCtrlOrtAnal;

	CString csValue;
	CString aFormat[] = {_T("%.4e"), _T("%.4e"), _T("%.4e"),   // Elasitcity
											 _T("%.4e"), _T("%.4e"), _T("%.4e"),   // Thermal
											 _T("%.4e"), _T("%.4e"), _T("%.4e"),   // Shear Modulus
											 _T("%g"),   _T("%g"),   _T("%g"),     // Posiion                       
											 _T("%g"), _T("%g")};              // Weight Density, Mass Density
	CEditUnit* pEdit;
	double Value[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		
	int nSize = paCtrlID->GetSize();
	for(int i = 0; i < nSize; i++)  
	{
		pEdit = (CEditUnit*)GetDlgItem(paCtrlID->GetAt(i));
		ASSERT(pEdit && pEdit->GetSafeHwnd() && IsWindow(pEdit->GetSafeHwnd()));
		//csValue.Format(aFormat[i], Value[i]);
		//pEdit->SetWindowText(csValue);
		pEdit->SetTextFormat(aFormat[i]);
		pEdit->SetEditUnit(Value[i]);

	}
}

BOOL CCMMatPageItemDlg::GetAnalData(int nTypeIndex, T_MATL_ANALYSIS &AnalData)
{
	CButton* IsoBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO1);  
	CButton* OrtBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO2);
	
	if(IsoBtn->GetCheck()) AnalData.nIsoOrtho = 0;  
	else if(OrtBtn->GetCheck()) AnalData.nIsoOrtho = 1;
	
	CArray<UINT, UINT> *paCtrlID;
	CEditUnit* pEdit;
	int nSize, i;
	double* pIsoValue[] = {&AnalData.Elast,   &AnalData.Poisson,   &AnalData.Thermal, &AnalData.Density, &AnalData.MassDensity};
	double* pOrtValue[] = {&AnalData.Elast,   &AnalData.Elast_y,   &AnalData.Elast_z, 
												 &AnalData.Thermal, &AnalData.Thermal_y, &AnalData.Thermal_z,
												 &AnalData.ShearModulus, &AnalData.ShearModulus_y, &AnalData.ShearModulus_z, 
												 &AnalData.Poisson, &AnalData.Poisson_y, &AnalData.Poisson_z,                         
												 &AnalData.Density, &AnalData.MassDensity};

	if(AnalData.nIsoOrtho == 0)
	{
		if(nTypeIndex == CCM_TYPE_STEEL_I)      paCtrlID = &m_aCtrlIsoStlAnal;
		else if(nTypeIndex == CCM_TYPE_CONCR_I) paCtrlID = &m_aCtrlIsoConAnal;
		else ASSERT(0);

		nSize = paCtrlID->GetSize();
		for(i = 0; i < nSize; i++)
		{
			pEdit = (CEditUnit*)GetDlgItem(paCtrlID->GetAt(i));
			ASSERT(pEdit && pEdit->GetSafeHwnd() && IsWindow(pEdit->GetSafeHwnd()));
			*pIsoValue[i] = pEdit->GetEditValue();
		}
	}  
	else if(AnalData.nIsoOrtho == 1) 
	{
		paCtrlID = &m_aCtrlOrtAnal; 
		nSize = paCtrlID->GetSize();
		for(i = 0; i < nSize; i++)
		{
			pEdit = (CEditUnit*)GetDlgItem(paCtrlID->GetAt(i));
			ASSERT(pEdit && pEdit->GetSafeHwnd() && IsWindow(pEdit->GetSafeHwnd()));
			*pOrtValue[i] = pEdit->GetEditValue();
		}    
	}  

//   /** DB에서는 온도를 Celsius 단위로 저장하기 때문에 필요하면 Fahrenheit로 변환한다.  */
//   CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
//   if (pUnitCtrl->GetBaseTemperUnitType() == D_UNITSYS_TEMPER_INDEX_F)         // case Fahrenheit 
//   {
//     AnalData.Thermal = AnalData.Thermal / pUnitCtrl->GetThermalTransCoeff(TRUE);                      // Fahrenheit -> Celsius
//     AnalData.Thermal_y = AnalData.Thermal_y / pUnitCtrl->GetThermalTransCoeff(TRUE);                      // Fahrenheit -> Celsius
//     AnalData.Thermal_z = AnalData.Thermal_z / pUnitCtrl->GetThermalTransCoeff(TRUE);                      // Fahrenheit -> Celsius
//   }

	return TRUE;
}

void CCMMatPageItemDlg::GetInelasticMatlCombo(const int nTypeIndex, T_KEY* pImfmK)
{
	DWORD nItemDataConc=0, nItemDataCoverConc = 0, nItemDataStl=0;

	CDlgUtil::CobxGetItemDataByCurSel(m_cmbInelasConc, nItemDataConc);
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbInelasCoverConc, nItemDataCoverConc);
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbInelasStl, nItemDataStl);

	switch ( nTypeIndex )
	{
	case CCM_TYPE_USER_I:
		break;
	case CCM_TYPE_ALUMI_I:
		break;
	case CCM_TYPE_TIMBER_I:
		break;
	case CCM_TYPE_STEEL_I:
		{
			pImfmK[1] = static_cast<T_IMFM_K>(nItemDataStl);
		}
		break;
	case CCM_TYPE_CONCR_I:
		{
			pImfmK[0] = static_cast<T_IMFM_K>(nItemDataConc);
			pImfmK[2] = static_cast<T_IMFM_K>(nItemDataCoverConc);
			pImfmK[3] = static_cast<T_IMFM_K>(nItemDataStl);
		}
		break;
	case CCM_TYPE_SRC_I:
		{
			pImfmK[0] = static_cast<T_IMFM_K>(nItemDataConc);
			pImfmK[1] = static_cast<T_IMFM_K>(nItemDataStl);
		}
		break;
	default: ASSERT(0); break;
	}
}
BOOL CCMMatPageItemDlg::Dlg2Data()
{
	int nID;
	if (!m_wndID.GetEditValue(nID))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_material_number_));
		return FALSE;
	}
	m_Key = (T_MATL_K)nID;
	CString csName;
	m_wndName.GetWindowText(csName);
	csName.TrimLeft(); csName.TrimRight();
	if (csName == _T("")) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Missing_material_name_));
		return FALSE;
	}
	m_Data.Name = csName;
	if (m_Data.Type == CCM_TYPE_SRC)
	{
		int nCheck=0;
		if (nCheck == 0) m_Data.bChangeSteel = TRUE;
		else m_Data.bChangeSteel = FALSE;
	}
	// Type, Code, Name is update when user changes them.

	// Get Analysis Data
	int nTypeIndex = GetTypeIndex(m_Data.Type);
	if (nTypeIndex != CCM_TYPE_CONCR_I) // steel, SRC, user, Aluminium
	{
		if (m_Data.Data1.CodeName == _T(""))  //code none
			GetAnalData(CCM_TYPE_STEEL_I, m_Data.Data1.Analysis);
		else 
			GetThermalData(CCM_TYPE_STEEL_I, m_Data.Data1.Analysis.Thermal);
		
		if (nTypeIndex == CCM_TYPE_SRC_I && m_Data.Data2.CodeName == _T(""))
			GetAnalData(CCM_TYPE_CONCR_I, m_Data.Data2.Analysis);
		else 
			GetThermalData(CCM_TYPE_CONCR_I, m_Data.Data2.Analysis.Thermal);
	}
	if (nTypeIndex == CCM_TYPE_CONCR_I)
	{
		if (m_Data.Data1.CodeName == _T(""))  // code none
			GetAnalData(CCM_TYPE_CONCR_I, m_Data.Data1.Analysis);
		else 
			GetThermalData(CCM_TYPE_CONCR_I, m_Data.Data1.Analysis.Thermal);
	}

	// *^^*
	if (m_Data.Data1.Analysis.nIsoOrtho == 0)
		m_Data.bUseMassDensity = (m_wndIsoStlMDenChk.GetCheck() == 1) ? TRUE : FALSE;
	else 
		m_Data.bUseMassDensity = (m_wndOrtMDenChk.GetCheck() == 1) ? TRUE : FALSE;
	

//   /** DB에서는 온도를 Celsius 단위로 저장하기 때문에 필요하면 Fahrenheit로 변환한다.  */
//   CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
//   if (pUnitCtrl->GetBaseTemperUnitType() == D_UNITSYS_TEMPER_INDEX_C)              // case Celsius
//   {
//     m_Data.dSpecificHeat = m_wndSpHeat.GetEditValue();
//     m_Data.dHeatConduction = m_wndHeatCo.GetEditValue();
//   }
//   else if (pUnitCtrl->GetBaseTemperUnitType() == D_UNITSYS_TEMPER_INDEX_F)         // case Fahrenheit 
//   {
//     m_Data.dSpecificHeat = m_wndSpHeat.GetEditValue() / pUnitCtrl->GetThermalTransCoeff(TRUE);       // Fahrenheit -> Celsius
//     m_Data.dHeatConduction = m_wndHeatCo.GetEditValue() / pUnitCtrl->GetThermalTransCoeff(TRUE);     // Fahrenheit -> Celsius
//   }

	m_Data.dSpecificHeat = m_wndSpHeat.GetEditValue();
	m_Data.dHeatConduction = m_wndHeatCo.GetEditValue();

 	if(m_Data.Data1.Analysis.nIsoOrtho == 0) m_wndPlasticCmb.GetSelectedEpmt(m_Data.KeyEpmt);
	else m_Data.KeyEpmt = 0;

	// *^^* 화씨일 때 -> 섭씨로 변환 후 저장 
	/*
	if (m_Data.Data1.Analysis.nIsoOrtho == 0 && m_Data.nThermalUnit == 1) 
	{
		if (m_Data.Data1.CodeName != _T("")) 
			ConvertFahrCelsins(0, m_Data.Data1.Analysis.Thermal);
		if (nTypeIndex == CCM_TYPE_SRC_I && m_Data.Data2.CodeName != _T(""))
			ConvertFahrCelsins(0, m_Data.Data2.Analysis.Thermal);
	}
	*/
	m_Data.dDampingRatio = m_edtDampingRatio.GetEditValue();

	if(nTypeIndex == CCM_TYPE_STEEL_I)
	{
		m_Data.Data1.Analysis.bUserElast = (m_wndIsoStlUserElaChk.GetCheck() == 1) ? TRUE : FALSE;
		if(m_Data.Data1.Analysis.bUserElast) m_Data.Data1.Analysis.Elast = m_wndIsoStlEla.GetEditValue();
	}
	else if(nTypeIndex == CCM_TYPE_CONCR_I)
	{
		m_Data.Data1.Analysis.bUserElast = (m_wndIsoConUserElaChk.GetCheck() == 1) ? TRUE : FALSE;
		if(m_Data.Data1.Analysis.bUserElast) m_Data.Data1.Analysis.Elast = m_wndIsoConEla.GetEditValue();
	}
	else if(nTypeIndex == CCM_TYPE_SRC_I)
	{
		m_Data.Data1.Analysis.bUserElast = (m_wndIsoStlUserElaChk.GetCheck() == 1) ? TRUE : FALSE;
		m_Data.Data2.Analysis.bUserElast = (m_wndIsoConUserElaChk.GetCheck() == 1) ? TRUE : FALSE;

		if(m_Data.Data1.Analysis.bUserElast) m_Data.Data1.Analysis.Elast = m_wndIsoStlEla.GetEditValue();
		if(m_Data.Data2.Analysis.bUserElast) m_Data.Data2.Analysis.Elast = m_wndIsoConEla.GetEditValue();
	}	

	return TRUE;
}

BOOL CCMMatPageItemDlg::GetThermalData(int nTypeIndex, double& dThermal)
{
	//CWnd* pWnd = NULL;
	CEditUnit* pEdit = NULL;
	CString csThermal;

			 if (nTypeIndex == CCM_TYPE_STEEL_I)    pEdit = &m_wndIsoStlThr; //pWnd = &m_wndIsoStlThr;
	else if (nTypeIndex == CCM_TYPE_CONCR_I)    pEdit = &m_wndIsoConThr; //pWnd = &m_wndIsoConThr;
	else ASSERT(0);
	
	//if (pWnd == NULL) return FALSE;
	//pWnd->GetWindowText(csThermal);
	//dThermal = _tstof(csThermal);
	if (pEdit == NULL) return FALSE;
	dThermal = pEdit->GetEditValue();

//   /** DB에서는 온도를 Celsius 단위로 저장하기 때문에 필요하면 Fahrenheit로 변환한다.  */
//   CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
//   if (pUnitCtrl->GetBaseTemperUnitType() == D_UNITSYS_TEMPER_INDEX_F)         // case Fahrenheit 
//   {
//     dThermal = dThermal / pUnitCtrl->GetThermalTransCoeff(TRUE);       // Fahrenheit -> Celsius
//   }

	return TRUE;
}

BOOL CCMMatPageItemDlg::IsExistInCodeList(CArray<CString, CString&> &aList, CString &csCode)
{
	int nSize = aList.GetSize();
	int i = 0;
	for ( i = 0; i < nSize; i++)
		if (aList[i] == csCode) break;
	if (i == nSize) return FALSE;
	return TRUE;
}


BEGIN_MESSAGE_MAP(CCMMatPageItemDlg, CDialog)
	//{{AFX_MSG_MAP(CCMMatPageItemDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MP_ITEM_TYPE, OnChangeType)
	ON_CBN_SELCHANGE(IDC_CMD_MP_ITEM_CODE, OnChangeSteelCode)
	ON_BN_CLICKED(IDC_CMD_MP_ITEM_ONLY_STRAND_CHK, OnCmdOnlyStrandChk)
	ON_CBN_SELCHANGE(IDC_CMD_MP_ITEM_CODE2, OnChangeConcrCode)
	ON_CBN_SELCHANGE(IDC_CMD_MP_ITEM_CODE3, OnChangeConcrSubCode)
	ON_CBN_SELCHANGE(IDC_CMD_MP_ITEM_NAME_CBO, OnChangeSteelName)
	ON_CBN_SELCHANGE(IDC_CMD_MP_ITEM_PRODUCT_CBO, OnChangeProductCode)
	ON_CBN_SELCHANGE(IDC_CMD_MP_ITEM_NAME_CBO2, OnChangeConcrName)
	ON_BN_CLICKED(IDC_CMD_MP_ITEM_BTN_APPLY, OnApply)
	ON_EN_CHANGE(IDC_CMD_MP_ITEM_NAME, OnChangeName)
	ON_EN_CHANGE(IDC_CMD_MP_ITEM_ID, OnChangeID)
	ON_BN_CLICKED(IDC_CMD_MP_ITEM_ISO_RDO1, OnMatlTypeChangeRdo)
	ON_BN_CLICKED(IDC_CMD_MATL_ISO_CON_THR_RD1, OnCmdMatlIsoConThrRd)
	ON_BN_CLICKED(IDC_CMD_MATL_ISO_STL_THR_RD1, OnCmdMatlIsoStlThrRd)
	ON_BN_CLICKED(IDC_CMD_MP_ITEM_ISO_RDO2, OnMatlTypeChangeRdo)
	ON_BN_CLICKED(IDC_CMD_MATL_ISO_CON_THR_RD2, OnCmdMatlIsoConThrRd)
	ON_BN_CLICKED(IDC_CMD_MATL_ISO_STL_THR_RD2, OnCmdMatlIsoStlThrRd)
	ON_BN_CLICKED(IDC_CMD_MATL_ISO_CON_MDEN_TEXT, OnCmdMatlIsoConMdenChk)
	ON_BN_CLICKED(IDC_CMD_MATL_ISO_STL_MDEN_TEXT, OnCmdMatlIsoStlMdenChk)
	ON_BN_CLICKED(IDC_CMD_MATL_ORT_MDEN_TEXT, OnCmdMatlOrtMdenChk)
	ON_BN_CLICKED(IDC_CMD_MATL_ISO_STL_ELA_USER_CHK, OnCmdMatlIsoStlUserElaChk)
	ON_BN_CLICKED(IDC_CMD_MATL_ISO_CON_ELA_USER_CHK, OnCmdMatlIsoConUserElaChk) 
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnBnClickedCmdMatlFoldButtonClicked)
	ON_BN_CLICKED(IDC_CMD_INELAS_BTN, OnInelasticBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMMatPageItemDlg message handlers
void CCMMatPageItemDlg::OnChangeType() 
{
	// TODO: Add your control notification handler code here  
	//int nTypeIndex = m_wndType.GetCurSel();
	DWORD dwType;
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_wndType, dwType))  dwType = -1;
	int nTypeIndex = dwType;
	m_Data.Type = GetTypeCode(nTypeIndex);
	m_Data.Data1.Initialize();
	m_Data.Data2.Initialize();

	if (nTypeIndex == 4)  // Timber
	{
		//m_wndIsoStlFrame.SetFoldState(FALSE);
		m_wndIsoConFrame.SetFoldState(FALSE);
	}

	CButton* IsoBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO1);  
	CButton* OrtBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO2);
	
	if(nTypeIndex == CCM_TYPE_ALUMI_I)
	{
	OrtBtn->SetCheck(FALSE);
	IsoBtn->SetCheck(TRUE);
	}
	else if (nTypeIndex == CCM_TYPE_TIMBER_I)
	{
		IsoBtn->SetCheck(FALSE);
		OrtBtn->SetCheck(TRUE);
	}
	
	if(nTypeIndex == CCM_TYPE_STEEL_I || nTypeIndex == CCM_TYPE_CONCR_I ||
		nTypeIndex == CCM_TYPE_USER_I  || nTypeIndex == CCM_TYPE_ALUMI_I ||
		nTypeIndex == CCM_TYPE_TIMBER_I)
	{
		if(IsoBtn->GetCheck())      
			m_Data.Data1.Analysis.nIsoOrtho = 0;
		else if(OrtBtn->GetCheck()) 
			m_Data.Data1.Analysis.nIsoOrtho = 1;
		else ASSERT(0);
	}  
	if (nTypeIndex == CCM_TYPE_STEEL_I)
	{
		m_Data.Data1.CodeName = m_csDefSteelCode;
		m_Data.Data2.CodeName = _T("");
		m_Data.dDampingRatio = 0.02;
	}
	else if (nTypeIndex == CCM_TYPE_CONCR_I)
	{
		m_Data.Data1.CodeName = m_csDefConcrCode;
		m_Data.Data1.SubCodeName = m_csDefConcrSubCode;
		m_Data.Data2.CodeName = _T("");
		m_Data.Data2.SubCodeName = _T("");
		m_Data.dDampingRatio = 0.05;
	}
	else if (nTypeIndex == CCM_TYPE_SRC_I)
	{
		m_Data.Data1.CodeName = m_csDefSteelCodeSRC;
		m_Data.Data1.SubCodeName = _T("");
		m_Data.Data2.CodeName = m_csDefConcrCodeSRC;
		m_Data.Data2.SubCodeName = m_csDefConcrSubCodeSRC;
		m_Data.dDampingRatio = 0.05;
	}
	else if (nTypeIndex == CCM_TYPE_USER_I)
	{
		m_Data.Data1.CodeName = _T("");
		m_Data.Data2.CodeName = _T("");
		m_Data.dDampingRatio = 0.0;
	}
	else if(nTypeIndex == CCM_TYPE_ALUMI_I)
	{
		m_Data.Data1.CodeName = m_csDefAluminumCode;
		m_Data.Data2.CodeName = _T("");
		m_Data.dDampingRatio = 0.0;
	}
	else if (nTypeIndex == CCM_TYPE_TIMBER_I)
	{
		m_Data.Data1.CodeName = m_csDefTimberCode;
		m_Data.Data2.CodeName = _T("");
		m_Data.dDampingRatio = 0.0;
	}
	else ASSERT(0);

	if (m_Data.Data1.Analysis.nIsoOrtho == 1 && nTypeIndex != CCM_TYPE_TIMBER_I)
		m_Data.Data1.CodeName = _T("");

	SetRedraw(FALSE);

	ChangeBitmap(nTypeIndex);
	ShowDataToDlg();

	EnableDisableUserEla();
	EnableShowOnlyStrand();

	OnCmdMatlIsoStlUserElaChk();
	OnCmdMatlIsoConUserElaChk();

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}


void CCMMatPageItemDlg::OnMatlTypeChangeRdo() 
{  
	UpdateData(TRUE);

	SetRedraw(FALSE);

	// Orthotropic 선택시 FolderGroupBox를 다시 펼쳐서 대화상자크기를 재조정한다.
	if(m_wndMatlTypeRdo == 1)  // Orthotropic
	{
		//m_wndIsoStlFrame.SetFoldState(FALSE);
		m_wndIsoConFrame.SetFoldState(FALSE);
	}
	
//  int nTypeIndex = m_wndType.GetCurSel();  
	CButton* IsoBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO1);  
	CButton* OrtBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO2);
	BOOL bIsoCheck = IsoBtn->GetCheck();
	BOOL bOrtCheck = OrtBtn->GetCheck();  
	
	if (/*!m_wndIsoStlFrame.GetFoldState() &&*/ !m_wndIsoConFrame.GetFoldState())
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlIso, bIsoCheck);
	}
	CDlgUtil::CtrlShowHide(this, m_aCtrlOrt, bOrtCheck);
	EnableDisableUserEla();
	EnableShowOnlyStrand();

	m_wndPlasticCmb.EnableWindow(bIsoCheck);  

	DWORD dwType;
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_wndType, dwType))  dwType = -1;
	int nTypeIndex = dwType;
	CString csCode;

	if (bIsoCheck)
	{
		BOOL bThermalStl, bThermalCon;
		//int nTypeIndex = m_wndType.GetCurSel();  
		
		switch(nTypeIndex)
		{
		case CCM_TYPE_USER_I:
			m_wndSteelCode.GetWindowText(csCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCode);
#endif
			bThermalStl = (csCode != CCM_CODE_NONE);
			bThermalCon = FALSE;
			break;
		case CCM_TYPE_ALUMI_I:
			m_wndSteelCode.GetWindowText(csCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCode);
#endif
			bThermalStl = (csCode != CCM_CODE_NONE);
			bThermalCon = FALSE;
			break;
		case CCM_TYPE_STEEL_I:
			m_wndSteelCode.GetWindowText(csCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCode);
#endif
			bThermalStl = (csCode != CCM_CODE_NONE);
			bThermalCon = FALSE;
			break;
		case CCM_TYPE_CONCR_I:
			m_wndConcrCode.GetWindowText(csCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCode);
#endif
			bThermalStl = FALSE;
			bThermalCon = (csCode != CCM_CODE_NONE);
			break;
		case CCM_TYPE_SRC_I:
			m_wndSteelCode.GetWindowText(csCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCode);
#endif
			bThermalStl = (csCode != CCM_CODE_NONE);
			m_wndConcrCode.GetWindowText(csCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCode);
#endif
			bThermalCon = (csCode != CCM_CODE_NONE);
			break;
		}
		if (!bThermalStl) CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlThermalSteel, 0);
		CDlgUtil::CtrlShowHide(this, m_aCtrlThermalSteel, bThermalStl);
		if (!bThermalCon) CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlThermalConc, 0);
		CDlgUtil::CtrlShowHide(this, m_aCtrlThermalConc, bThermalCon);
	}

	if (bIsoCheck)
	{
		int nCheck = m_wndIsoStlMDenChk.GetCheck();
		EnableDisableMDensity(nCheck == 1);
	}
	else 
	{
		int nCheck = m_wndOrtMDenChk.GetCheck();
		m_wndSteelCode.GetWindowText(csCode);
		if (csCode == CCM_CODE_NONE || nTypeIndex != CCM_TYPE_TIMBER_I)
		{
			m_wndOrtMDen.EnableWindow(nCheck == 1);
		}
	}

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

void CCMMatPageItemDlg::OnChangeProductCode()
{
	CString csProdctCode;
	//int nTypeIndex = m_wndType.GetCurSel();
	DWORD dwType;
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_wndType, dwType))  dwType = -1;
	int nTypeIndex = dwType;

	if(nTypeIndex != CCM_TYPE_ALUMI_I/* && nTypeIndex != CCM_TYPE_USER_I*/) 
	{
		ASSERT(0);
		return;
	}

	m_wndProductName.GetWindowText(csProdctCode);
	m_Data.Data1.SubCodeName = csProdctCode;

	GetAlumiData(m_Data.Data1.CodeName, m_Data.Data1.CodeMatlName, m_Data.Data1.SubCodeName, m_Data.nThermalUnit, m_Data.Data1.Analysis);
	
	// show analysis data to edit box
	SetAnalData(CCM_TYPE_STEEL_I, m_Data.Data1.Analysis, m_Data.Data1.CodeName);
}

void CCMMatPageItemDlg::OnChangeSteelCode() 
{
	// TODO: Add your control notification handler code here
	CButton* IsoBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO1);
	CButton* OrtBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO2);

	CString csCode;
	m_wndSteelCode.GetWindowText(csCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCode);
#endif

	//int nTypeIndex = m_wndType.GetCurSel();  
	DWORD dwType;
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_wndType, dwType))  dwType = -1;
	int nTypeIndex = dwType;
	
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoOrt, (csCode == CCM_CODE_NONE &&
		nTypeIndex != CCM_TYPE_SRC_I) || nTypeIndex == CCM_TYPE_TIMBER_I);
	if (nTypeIndex == CCM_TYPE_TIMBER_I)
	{
		IsoBtn->SetCheck(FALSE);
		OrtBtn->SetCheck(TRUE);
	}
	else if (csCode != CCM_CODE_NONE || nTypeIndex == CCM_TYPE_SRC_I)
	{
		IsoBtn->SetCheck(TRUE);
		OrtBtn->SetCheck(FALSE);
	}

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlOrtAnal, (csCode == CCM_CODE_NONE || nTypeIndex != CCM_TYPE_TIMBER_I));

	// Enable/Disable 상태 조정
	if (csCode == CCM_CODE_NONE) 
	{
		csCode = _T("");
		m_wndSteelName.EnableWindow(FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoStlAnal, TRUE);
		m_Data.Data1.CodeMatlName = _T("");  // Material Name은 초기화, Analysis Data는 유지
	}
	else
	{
		m_wndSteelName.EnableWindow(TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoStlAnal, FALSE);
		m_Data.Data1.Initialize();  // Analysis Data, Material Name 초기화

		if (nTypeIndex == CCM_TYPE_TIMBER_I)
		{
			m_Data.Data1.Analysis.nIsoOrtho = 1; // Timber Orthotropic property 초기화
		}
	}
	
	EnableShowOnlyStrand();
	
	// Name Combo List 조정
	m_Data.Data1.CodeName = csCode;
	SetNameCombo(&m_wndSteelName, m_Data.Type, m_Data.Data1.CodeName);
	SetAnalData(CCM_TYPE_STEEL_I, m_Data.Data1.Analysis, m_Data.Data1.CodeName);

	OnMatlTypeChangeRdo();
	SetProductCombo(&m_wndProductName, m_Data.Type, m_Data.Data1.CodeName, m_Data.Data1.CodeMatlName, m_Data.Data1.SubCodeName);

	EnableDisableUserEla();
	EnableShowOnlyStrand();
}

void CCMMatPageItemDlg::OnChangeConcrCode() 
{
	SetRedraw(FALSE);
	// TODO: Add your control notification handler code here
	CButton* IsoBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO1);
	CButton* OrtBtn = (CButton*)GetDlgItem(IDC_CMD_MP_ITEM_ISO_RDO2);
	CString csCode;
	m_wndConcrCode.GetWindowText(csCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCode);
#endif
	//int nTypeIndex = m_wndType.GetCurSel();  
	DWORD dwType;
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_wndType, dwType))  dwType = -1;
	int nTypeIndex = dwType;

	T_MATL_ALL* pData;
	if (m_Data.Type == CCM_TYPE_SRC) pData = &m_Data.Data2; // type == src
	else if (m_Data.Type == CCM_TYPE_CONCR) pData = &m_Data.Data1; // type = concrete
	else ASSERT(0); // Can't call this function when type is steel or user defined

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlMatlTypeRdoOrt, (csCode == CCM_CODE_NONE && nTypeIndex != CCM_TYPE_SRC_I));
	if (csCode != CCM_CODE_NONE || nTypeIndex == CCM_TYPE_SRC_I)
	{
		IsoBtn->SetCheck(TRUE);
		OrtBtn->SetCheck(FALSE);
	}

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlOrtAnal, (csCode == CCM_CODE_NONE || nTypeIndex != CCM_TYPE_SRC_I));

	// Enable/Disable 상태 조정
	if (csCode == CCM_CODE_NONE) 
	{
		csCode = _T("");
		m_wndConcrName.EnableWindow(FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoConAnal, TRUE);
		pData->CodeMatlName = _T("");  // Material Name은 초기화, Analysis Data는 유지
	}
	else
	{
		m_wndConcrName.EnableWindow(TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlIsoConAnal, FALSE);
		pData->Initialize();  // Analysis Data, Material Name 초기화
	}

	// Name Combo List 조정
	pData->CodeName = csCode;
	SetNameCombo(&m_wndConcrName, m_Data.Type, pData->CodeName);	
	SetAnalData(CCM_TYPE_CONCR_I, pData->Analysis, pData->CodeName);

	OnMatlTypeChangeRdo();

	EnableDisableUserEla();	
	EnableShowOnlyStrand();

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

void CCMMatPageItemDlg::OnChangeConcrSubCode()
{
	CString csSubCode;
	m_wndConcrSubCode.GetWindowText(csSubCode);

	T_MATL_ALL* pData;
	if (m_Data.Type == CCM_TYPE_SRC) pData = &m_Data.Data2; // type == src
	else if (m_Data.Type == CCM_TYPE_CONCR) pData = &m_Data.Data1; // type = concrete
	else ASSERT(0); // Can't call this function when type is steel or user defined

	pData->SubCodeName = csSubCode;

	CString csCode = pData->CodeName;
	CString csName = pData->CodeMatlName;
	
	// get analyis data
	GetConcData(csCode, csSubCode, csName, m_Data.nThermalUnit, pData->Analysis);

	// show analysis data to edit box
	SetAnalData(CCM_TYPE_CONCR_I, pData->Analysis, pData->CodeName);
}

void CCMMatPageItemDlg::OnChangeSteelName() 
{
	// TODO: Add your control notification handler code here
	CString csType, csCode, csSubCode, csName;

	m_wndSteelName.GetWindowText(csName);
	csCode = m_Data.Data1.CodeName;

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(csCode, csName);
#endif

	m_Data.Data1.CodeMatlName = csName;

	// decide code is steel or concrete
	if (m_Data.Type == CCM_TYPE_USER || m_Data.Type == CCM_TYPE_ALUMNI || m_Data.Type == CCM_TYPE_TIMBER) // steel or concrete
		m_pDoc->m_pMatlDB->GetMatlType(csCode, csType);
	else csType = CCM_TYPE_STEEL;

	//for auto Name
	CString ProductCode = m_Data.Data1.SubCodeName;
	SetProductCombo(&m_wndProductName, m_Data.Type, m_Data.Data1.CodeName, m_Data.Data1.CodeMatlName, ProductCode);  
	m_wndProductName.GetWindowText(ProductCode);
	m_Data.Data1.SubCodeName = ProductCode;
	
	csSubCode = m_Data.Data1.SubCodeName;
	// get analyis data
	if (csType == CCM_TYPE_STEEL) // steel
		GetSteelData(csCode, csName, m_Data.nThermalUnit, m_Data.Data1.Analysis);
	else if (csType == CCM_TYPE_CONCR) // concrete
		GetConcData(csCode, csSubCode, csName, m_Data.nThermalUnit, m_Data.Data1.Analysis);
	else if (csType == CCM_TYPE_ALUMNI) // Aluminum
		GetAlumiData(csCode, csName, csSubCode, m_Data.nThermalUnit, m_Data.Data1.Analysis);
	else if (csType == CCM_TYPE_TIMBER) // Timber
		GetTimberData(csCode, csName, m_Data.nThermalUnit, m_Data.Data1.Analysis);
	else ASSERT(0);	

	// show analysis data to edit box
	SetAnalData(CCM_TYPE_STEEL_I, m_Data.Data1.Analysis, m_Data.Data1.CodeName);
	SetTherData(csCode);//add by xuezc For Init SPHEAT of _T("GBXXX")
	// change material name
	//if (m_Data.Type == CCM_TYPE_SRC) m_Data.Name = csName + "/" + m_Data.Data2.CodeMatlName;
	//else m_Data.Name = csName;
	if (m_bAutoChangeName)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(csCode, csName);
#endif
		m_Data.Name = csName;
		m_wndName.SetWindowText(m_Data.Name);
	}


}

void CCMMatPageItemDlg::OnChangeConcrName() 
{
	// TODO: Add your control notification handler code here
	CString csType, csCode, csSubCode, csName;

	T_MATL_ALL* pData;
	if (m_Data.Type == CCM_TYPE_SRC) pData = &m_Data.Data2; // type == src
	else if (m_Data.Type == CCM_TYPE_CONCR) pData = &m_Data.Data1; // type = concrete
	else ASSERT(0); // Can't call this function when type is steel or user defined

	m_wndConcrName.GetWindowText(csName);
	csCode = pData->CodeName;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(csCode, csName);
#endif
	pData->CodeMatlName = csName;
	csSubCode = pData->SubCodeName;

	// decide code is steel or concrete
	csType = CCM_TYPE_CONCR;    // SRC  or Concrete -> Concrete

	// get analyis data
	GetConcData(csCode, csSubCode, csName, m_Data.nThermalUnit, pData->Analysis);

	// show analysis data to edit box
	SetAnalData(CCM_TYPE_CONCR_I, pData->Analysis, pData->CodeName);	

	// change material name
	if (m_bAutoChangeName)
	{
		m_Data.Name = csName;
		m_wndName.SetWindowText(m_Data.Name);
	}
}

void CCMMatPageItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;

	const int nTypeIndex = GetTypeIndex(m_Data.Type);
	T_IMFM_K ImfmK[4] = {0};
	GetInelasticMatlCombo(nTypeIndex, ImfmK);

	if (m_bModify)
	{
		if (!m_pDoc->m_pDataCtrl->ModifyMatl(m_OldKey, m_Key, m_Data, ImfmK)) return;
	}
	else 
	{
		if (!m_pDoc->m_pDataCtrl->AddMatl(m_Key, m_Data, ImfmK)) return;
	}
	CDialog::OnOK();
}

void CCMMatPageItemDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
	if (!Dlg2Data()) return;

	const int nTypeIndex = GetTypeIndex(m_Data.Type);
	T_IMFM_K ImfmK[4] = {0};
	GetInelasticMatlCombo(nTypeIndex, ImfmK);

	if (m_bModify)
	{
		if (!m_pDoc->m_pDataCtrl->ModifyMatl(m_OldKey, m_Key, m_Data, ImfmK)) return;
		m_OldKey = m_Key;
	}
	else 
	{
		if (!m_pDoc->m_pDataCtrl->AddMatl(m_Key, m_Data, ImfmK)) return;
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumMatl(m_Key);
	}

	// Initialize after apply
	if (!m_bModify)
	{
		CString csValue;
		csValue.Format(_T("%d"), m_Key);
		m_wndID.SetWindowText(csValue);
		m_Data.Name = _T("");
		m_wndName.SetWindowText(m_Data.Name);
	}
}


void CCMMatPageItemDlg::OnChangeName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString csName;
	m_wndName.GetWindowText(csName);
	m_Data.Name = csName;

	SetAutoChangeNameFlag(m_Data.Type, m_Data.Name, m_Data.Data1.SubCodeName);
}

void CCMMatPageItemDlg::OnChangeID() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int nID;
	if (m_wndID.GetEditValue(nID))
		m_Key = (T_MATL_K)nID;
}

void CCMMatPageItemDlg::SetAutoChangeNameFlag(CString csType, CString csName, CString csProd/*=_T("")*/)
{
	if (csName == _T("")) m_bAutoChangeName = TRUE;
	else
	{
		//Code 이름없거나 None이 아닌 경우만 고려
		CString csCodeNameC;
		CString csCodeNameS;
		CString csCodeNameA;
		// concrete
		BOOL bConNon= FALSE;
		m_wndConcrCode.GetWindowText(csCodeNameC);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCodeNameC);
#endif
		if (csCodeNameC == _T("") || csCodeNameC == CCM_CODE_NONE) bConNon= TRUE;
		// steel
		BOOL bStlNon= FALSE;
		m_wndSteelCode.GetWindowText(csCodeNameS);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCodeNameS);
#endif

		if (csCodeNameS == _T("") || csCodeNameS == CCM_CODE_NONE) bStlNon= TRUE;
		//Alu
		BOOL bAluNon= FALSE;
		m_wndSteelCode.GetWindowText(csCodeNameA);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCodeNameA);
#endif
		if (csCodeNameA == _T("") || csCodeNameA == CCM_CODE_NONE) bAluNon= TRUE;

		CString csMatlType;
		T_MATL_STEEL SteelData;
		T_MATL_CONCRETE ConcrData;
		T_MATL_ALUMINUM AlumiData;
		CArray<CString, CString&> aDesignCodeList;
		m_pDoc->m_pMatlDB->GetDesignCodeList(CString(CCM_TYPE_USER), aDesignCodeList);
		int nSize = aDesignCodeList.GetSize();
		int i = 0;
		for (i = 0; i < nSize; i++)
		{
			m_pDoc->m_pMatlDB->GetMatlType(aDesignCodeList[i], csMatlType);

			if (!bStlNon && csMatlType == CCM_TYPE_STEEL)
			{
				if (m_pDoc->m_pMatlDB->GetSteelData(aDesignCodeList[i], csName, SteelData))
				{
					m_Data.Data1.Analysis.Thermal = SteelData.Thermal;
					break;
				}
			}
			else if (!bConNon && csMatlType == CCM_TYPE_CONCR)
			{
				if (m_pDoc->m_pMatlDB->GetConcreteData(aDesignCodeList[i], csName, ConcrData))
				{
					m_Data.Data1.Analysis.Thermal = ConcrData.Thermal;
					break;
				}

			}
			else if (!bAluNon && csMatlType == CCM_TYPE_ALUMNI)
			{
				if (m_pDoc->m_pMatlDB->GetAluminumData(aDesignCodeList[i], csName, csProd, AlumiData))
				{
					m_Data.Data1.Analysis.Thermal = AlumiData.Thermal;
					break;
				}
			}
		}
		if (i == nSize) m_bAutoChangeName = FALSE;  // DB에 없으면
		else m_bAutoChangeName = TRUE; 
	}
}



void CCMMatPageItemDlg::EnableDisableMDensity(BOOL bEnable)
{
	BOOL bMDenStl, bMDenCon;
	int nTypeIndex = GetTypeIndex(m_Data.Type);
	if(nTypeIndex == CCM_TYPE_STEEL_I) // Steel
	{
		bMDenStl = (m_Data.Data1.CodeName == _T(""));
		bMDenCon = FALSE;
	}
	else if(nTypeIndex == CCM_TYPE_USER_I) // User
	{
		bMDenStl = (m_Data.Data1.CodeName == _T(""));
		bMDenCon = FALSE;
	}    
	else if(nTypeIndex == CCM_TYPE_ALUMI_I)
	{
		bMDenStl = (m_Data.Data1.CodeName == _T(""));
		bMDenCon = FALSE;
	}
	else if(nTypeIndex == CCM_TYPE_CONCR_I) // Concrete
	{
		bMDenStl = FALSE;
		bMDenCon = (m_Data.Data1.CodeName == _T(""));
	}
	else if(nTypeIndex == CCM_TYPE_SRC_I) // SRC
	{
		bMDenStl = (m_Data.Data1.CodeName == _T("")); 
		bMDenCon = (m_Data.Data2.CodeName == _T(""));
	}  
	else if (nTypeIndex == CCM_TYPE_TIMBER_I) // Timber
	{
		bMDenStl = (m_Data.Data1.CodeName == _T(""));
		bMDenCon = FALSE;
	}
	else ASSERT(0);

	if (bMDenStl) m_wndIsoStlMDen.EnableWindow(bEnable);
	if (bMDenCon) m_wndIsoConMDen.EnableWindow(bEnable);
}

void CCMMatPageItemDlg::OnCmdMatlIsoConMdenChk() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_wndIsoConMDenChk.GetCheck();
	m_wndIsoStlMDenChk.SetCheck(nCheck);
	EnableDisableMDensity(nCheck == 1);
	
	// 이전 버전과의 호환을 위해.. *^^*
	T_MATL_ALL* pData;
	if (m_Data.Type == CCM_TYPE_SRC) pData = &m_Data.Data2; // type == src
	else if (m_Data.Type == CCM_TYPE_CONCR) pData = &m_Data.Data1; // type = concrete
	else ASSERT(0); // Can't call this function when type is steel or user defined

	CString csName, csCode, csSubCode;
	csCode = pData->CodeName;
	m_wndConcrName.GetWindowText(csName);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(csCode, csName);
#endif
	pData->CodeMatlName = csName;
	csSubCode = pData->SubCodeName;

	GetConcData(csCode, csSubCode, csName, m_Data.nThermalUnit, pData->Analysis);

	CString csMDensity;
	//csMDensity.Format("%.4g", pData->Analysis.MassDensity);
	//m_wndIsoConMDen.SetWindowText(csMDensity);
	m_wndIsoConMDen.SetTextFormat(_T("%.4g"));
	m_wndIsoConMDen.SetEditUnit(pData->Analysis.MassDensity);
}

void CCMMatPageItemDlg::OnCmdMatlIsoStlMdenChk() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_wndIsoStlMDenChk.GetCheck();
	m_wndIsoConMDenChk.SetCheck(nCheck);
	EnableDisableMDensity(nCheck == 1);
	
	// 이전 버전과의 호환을 위해.. *^^*
	CString csName, csCode, csSubCode, csType;
	m_wndSteelName.GetWindowText(csName);
	csCode = m_Data.Data1.CodeName;

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(csCode, csName);
#endif

	m_Data.Data1.CodeMatlName = csName;
	csSubCode = m_Data.Data1.SubCodeName;

	if (m_Data.Type == CCM_TYPE_USER) // steel or concrete
		m_pDoc->m_pMatlDB->GetMatlType(csCode, csType);
	else if (m_Data.Type == CCM_TYPE_ALUMNI)
		m_pDoc->m_pMatlDB->GetMatlType(csCode, csType);
	else csType = CCM_TYPE_STEEL;
	if (csType.IsEmpty()) return;

	if (csType == CCM_TYPE_STEEL) // steel
		GetSteelData(csCode, csName, m_Data.nThermalUnit, m_Data.Data1.Analysis);
	else if (csType == CCM_TYPE_CONCR) // concrete
		GetConcData(csCode, csSubCode, csName, m_Data.nThermalUnit, m_Data.Data1.Analysis);
	else if (csType == CCM_TYPE_ALUMNI) // Aluminum
		GetAlumiData(csCode, csName, csSubCode, m_Data.nThermalUnit, m_Data.Data1.Analysis);
	else ASSERT(0);	

	CString csMDensity;
	//csMDensity.Format("%.4g", m_Data.Data1.Analysis.MassDensity);
	//m_wndIsoStlMDen.SetWindowText(csMDensity);
	m_wndIsoStlMDen.SetTextFormat(_T("%.4g"));
	m_wndIsoStlMDen.SetEditUnit(m_Data.Data1.Analysis.MassDensity);
}

void CCMMatPageItemDlg::OnCmdMatlOrtMdenChk()
{
	// TODO: Add your control notification handler code here
	int nCheck = m_wndOrtMDenChk.GetCheck();

	DWORD dwType;
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_wndType, dwType))  dwType = -1;
	int nTypeIndex = dwType;
	CString csCode;
	m_wndSteelCode.GetWindowText(csCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCode);
#endif

	if (csCode == CCM_CODE_NONE || nTypeIndex != CCM_TYPE_TIMBER_I)
	{
		m_wndOrtMDen.EnableWindow(nCheck == 1);
	}
}

// 두개가 동시에 작동해야 한다.
// 하나는 화씨, 하나는 섭씨가 될 수 없다.
void CCMMatPageItemDlg::OnCmdMatlIsoConThrRd() 
{
	// TODO: Add your control notification handler code here
	int nOption;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlThermalConc, nOption);
	if (m_Data.nThermalUnit == nOption) return; // 이전값과 같으면  return

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlThermalSteel, nOption);
	ChangeThermalUnit(nOption);
}

void CCMMatPageItemDlg::OnCmdMatlIsoStlThrRd() 
{
	// TODO: Add your control notification handler code here
	int nOption;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlThermalSteel, nOption);
	if (m_Data.nThermalUnit  == nOption) return; // 이전값과 같으면  return

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlThermalConc, nOption);
	ChangeThermalUnit(nOption);
}

void CCMMatPageItemDlg::ChangeThermalUnit(int nThermalUnit)
{
	//CWnd* pWnd;
	CEditUnit* pEdit = NULL;
	CString csCodeName;
	CString csThermalCoef;
	double dThermalCoef;

	// concrete
	m_wndConcrCode.GetWindowText(csCodeName);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCodeName);
#endif
	if (csCodeName != CCM_CODE_NONE)
	{
		//pWnd = GetDlgItem(IDC_CMD_MATL_ISO_CON_THR_EDIT);
		//if (pWnd == NULL) { ASSERT(0); return; }
		//pWnd->GetWindowText(csThermalCoef);
		// dThermalCoef = _tstof(csThermalCoef);
		pEdit = (CEditUnit*)GetDlgItem(IDC_CMD_MATL_ISO_CON_THR_EDIT);
		if (pEdit == NULL) { ASSERT(0); return; }
		dThermalCoef = pEdit->GetEditValue();
		ConvertFahrCelsins(nThermalUnit, dThermalCoef);
		//csThermalCoef.Format("%.4e", dThermalCoef);
		//pWnd->SetWindowText(csThermalCoef);
		pEdit->SetTextFormat(_T("%.4e"));
		pEdit->SetEditUnit(dThermalCoef);  
	}
	
	// steel
	m_wndSteelCode.GetWindowText(csCodeName);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCodeName);
#endif
	if (csCodeName != CCM_CODE_NONE)
	{
		//pWnd = GetDlgItem(IDC_CMD_MATL_ISO_STL_THR_EDIT);
		//if (pWnd == NULL) { ASSERT(0); return; }
		//pWnd->GetWindowText(csThermalCoef);
		//dThermalCoef = _tstof(csThermalCoef);
		pEdit = (CEditUnit*)GetDlgItem(IDC_CMD_MATL_ISO_STL_THR_EDIT);
		if (pEdit == NULL) { ASSERT(0); return; }
		dThermalCoef = pEdit->GetEditValue();
		ConvertFahrCelsins(nThermalUnit, dThermalCoef);
		//csThermalCoef.Format("%.4e", dThermalCoef);
		//pWnd->SetWindowText(csThermalCoef);
		pEdit->SetTextFormat(_T("%.4e"));
		pEdit->SetEditUnit(dThermalCoef); 
	}

	m_Data.nThermalUnit = nThermalUnit;
}

BOOL CCMMatPageItemDlg::ConvertFahrCelsins(int nThermalUnit, double &dThermalCoef)
{
	if (nThermalUnit == 0)         // 1/C = 1/F * 9/5 (화씨 -> 섭씨) 
	{
		dThermalCoef *= 9; 
		dThermalCoef /= 5;
	}
	else if (nThermalUnit == 1)    // 1/F = 1/C * 5/9 (섭씨 -> 화씨)
	{
		dThermalCoef *= 5; 
		dThermalCoef /= 9;
	}
	else return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Interface
/////////////////////////////////////////////////////////////////////////////
void CCMMatPageItemDlg::SetModifyData(T_MATL_K key, T_MATL_D& data)
{
	m_OldKey = key;
	m_Key = key;
	m_Data = data;
	m_bModify = TRUE;
}

void CCMMatPageItemDlg::SetMaterialID(UINT key)
{
	m_bWorkTab = true;
	m_FixID = key;
}

void CCMMatPageItemDlg::ExportMatlDB()
{
	// 단위계 맞추어서.. 출력할 것.
	CString StrPath, StrCode, StrSubCode, StrName;
	int nCodeCnt = m_wndSteelCode.GetCount();
	ofstream fout;
	
	string strData;

	const TCHAR* DIR_PATH_STEEL = _T("C:\\CMIDAS\\GENw\\CivilDebug\\MatlDB_TXT\\Steel\\");
	const TCHAR* DIR_PATH_CONCRETE = _T("C:\\CMIDAS\\GENw\\CivilDebug\\MatlDB_TXT\\Concrete\\");
	// 나중에 폴더 있는 검사는 코드 넣고 상대로 경로로 바꿀 것.
	const TCHAR* _DIR_PATH_STEEL = _T(".\\MatlDB_TXT\\Steel\\");
	const TCHAR* _DIR_PATH_CONCRETE = _T(".\\MatlDB_TXT\\Concrete\\");

	// Steel
	for (int idx = 0; idx < nCodeCnt; ++idx)
	{
		m_wndSteelCode.GetLBText(idx, StrCode);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(StrCode);
#endif

		StrPath.Format(_T("%s%s.txt"), DIR_PATH_STEEL, StrCode);
		
		fout.open(StrPath, ios_base::out);
		if(fout.is_open())
		{
			SetNameCombo(&m_wndSteelName, CCM_TYPE_STEEL, StrCode);
			
			fout << setw(20) << _T("Name") 
				<< setw(20) << _T("Elast") 
				<< setw(20) << _T("Poisson") 
				<< setw(20) << _T("Thermal")
				<< setw(20) << _T("Density") 
				<< setw(20) << _T("MassDensity") 
				<< setw(20) << _T("S_fu;") 
				<< setw(20) << _T("S_fy1") 
				<< setw(20) << _T("S_fy2")
				<< setw(20) << _T("S_fy3")
				<< setw(20) << _T("S_fy4")
				<< setw(20) << _T("S_fy5")
				<< setw(20) << _T("S_fy6")
				<< setw(20) << _T("S_aft")
				<< setw(20) << _T("S_aft2")
				<< setw(20) << _T("S_aft3")
				<< setw(20) << _T("S_fy")
				<< setw(20) << _T("S_afv")
				<< setw(20) << _T("S_afv2")
				<< setw(20) << _T("S_afv3")
					<< endl << endl << endl;

			int nNameCnt = m_wndSteelName.GetCount();
			for (int idxName = 0; idxName < nNameCnt; ++idxName)
			{
				m_wndSteelName.GetLBText(idxName, StrName);
#if defined(_RUS)
				m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(StrCode, StrName);
#endif
				T_MATL_STEEL Steeldata;
				if (m_pDoc->m_pMatlDB->GetSteelData(StrCode, StrName, Steeldata))
				{
					strData = CStringA(StrName);

					fout << setw(20) << strData
						<< setw(20) << Steeldata.Elast 
						<< setw(20) << Steeldata.Poisson
						<< setw(20) << Steeldata.Thermal
						<< setw(20) << Steeldata.Density 
						<< setw(20) << Steeldata.MassDensity 
						<< setw(20) << Steeldata.S_Fu
						<< setw(20) << Steeldata.S_Fy1 
						<< setw(20) << Steeldata.S_Fy2
						<< setw(20) << Steeldata.S_Fy3
						<< setw(20) << Steeldata.S_Fy4
						<< setw(20) << Steeldata.S_Fy5
						<< setw(20) << Steeldata.S_Fy6
						<< setw(20) << Steeldata.S_Aft
						<< setw(20) << Steeldata.S_Aft2
						<< setw(20) << Steeldata.S_Aft3
						<< setw(20) << Steeldata.S_Fy
						<< setw(20) << Steeldata.S_Afv
						<< setw(20) << Steeldata.S_Afv2
						<< setw(20) << Steeldata.S_Afv3;
				}
				fout << endl;
			}
			
		fout.close();
		}
	}

	// Conrete
	SetCode(&m_wndConcrCode, CCM_TYPE_CONCR);
	nCodeCnt = m_wndConcrCode.GetCount();
	for (int idx = 0; idx < nCodeCnt; ++idx)
	{
		m_wndConcrCode.GetLBText(idx, StrCode);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(StrCode);
#endif
		SetNameCombo(&m_wndConcrName, CCM_TYPE_CONCR, StrCode);

		int nSubCodeCnt = m_wndConcrSubCode.GetCount();
		if (nSubCodeCnt > 0)
		{
			for (int idxSub = 0; idxSub < nSubCodeCnt; ++idxSub)
			{
				m_wndConcrSubCode.GetLBText(idxSub, StrSubCode);
				StrPath.Format(_T("%s%s (%s).txt"), DIR_PATH_CONCRETE, StrCode, StrSubCode);
				fout.open(StrPath, ios_base::out);
				if(fout.is_open())
				{     
					fout << setw(20) << _T("Name") 
						<< setw(20) << _T("Elast") 
						<< setw(20) << _T("Poisson") 
						<< setw(20) << _T("Thermal")
						<< setw(20) << _T("Density") 
						<< setw(20) << _T("MassDensity") 
						<< setw(20) << _T("C_fc") 
						<< setw(20) << _T("C_fci") 
						<< endl << endl << endl;
					
					int nNameCnt = m_wndConcrName.GetCount();
					for (int idxName = 0; idxName < nNameCnt; ++idxName)
					{
						m_wndConcrName.GetLBText(idxName, StrName);
#if defined(_RUS)
						m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(StrCode, StrName);
#endif
						T_MATL_CONCRETE Concretedata;
						if (m_pDoc->m_pMatlDB->GetConcreteDataNew(StrCode, StrSubCode, StrName, Concretedata))
						{
							strData = CStringA(StrName);
							
							fout << setw(20) << strData
								<< setw(20) << Concretedata.Elast
								<< setw(20) << Concretedata.Poisson
								<< setw(20) << Concretedata.Thermal
								<< setw(20) << Concretedata.Density 
								<< setw(20) << Concretedata.MassDensity 
								<< setw(20) << Concretedata.C_fc
								<< setw(20) << Concretedata.C_fci ;
						}
						fout << endl;
					}
					
					fout.close();
			}
			}
		}
		else
		{
			StrPath.Format(_T("%s%s.txt"), DIR_PATH_CONCRETE, StrCode);
			fout.open(StrPath, ios_base::out);
			if(fout.is_open())
			{     
				fout << setw(20) << _T("Name") 
					<< setw(20) << _T("Elast") 
					<< setw(20) << _T("Poisson") 
					<< setw(20) << _T("Thermal")
					<< setw(20) << _T("Density") 
					<< setw(20) << _T("MassDensity") 
					<< setw(20) << _T("C_fc") 
					<< setw(20) << _T("C_fci") 
					<< endl << endl << endl;
				
				int nNameCnt = m_wndConcrName.GetCount();
				for (int idxname = 0; idxname < nNameCnt; ++idxname)
				{
					m_wndConcrName.GetLBText(idxname, StrName);
#if defined(_RUS)
					m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(StrCode, StrName);
#endif
					T_MATL_CONCRETE Concretedata;
					if (m_pDoc->m_pMatlDB->GetConcreteDataNew(StrCode, StrSubCode, StrName, Concretedata))
					{
						strData = CStringA(StrName);
						
						fout << setw(20) << strData
							<< setw(20) << Concretedata.Elast
							<< setw(20) << Concretedata.Poisson
							<< setw(20) << Concretedata.Thermal
							<< setw(20) << Concretedata.Density 
							<< setw(20) << Concretedata.MassDensity 
							<< setw(20) << Concretedata.C_fc
							<< setw(20) << Concretedata.C_fci;
					}
					fout << endl;
				}
				
				fout.close();
			}
		}
	}

	m_wndConcrCode.ResetContent();
}

void CCMMatPageItemDlg::EnableDisableUserEla() 
{
	CString csStlCode;
	CString csConCode;

	m_wndSteelCode.GetWindowText(csStlCode);
	m_wndConcrCode.GetWindowText(csConCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csStlCode);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csConCode);
#endif

	if(csStlCode == MATLCODE_STL_JTG3362_18) GetDlgItem(IDC_CMD_MATL_ISO_STL_ELA_USER_CHK)->ShowWindow(SW_SHOW);
	else                        GetDlgItem(IDC_CMD_MATL_ISO_STL_ELA_USER_CHK)->ShowWindow(SW_HIDE);

	if (csConCode == MATLCODE_CON_JTG3362_18) GetDlgItem(IDC_CMD_MATL_ISO_CON_ELA_USER_CHK)->ShowWindow(SW_SHOW);
	else                         GetDlgItem(IDC_CMD_MATL_ISO_CON_ELA_USER_CHK)->ShowWindow(SW_HIDE);
}

void CCMMatPageItemDlg::EnableShowOnlyStrand()
{
	m_wndOnlyStrandChk.SetCheck(FALSE);
	GetDlgItem(IDC_CMD_MP_ITEM_ONLY_STRAND_CHK)->ShowWindow(SW_HIDE);
	return;

	CString csCode;
	m_wndSteelCode.GetWindowText(csCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csCode);
#endif

	DWORD dwType;
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_wndType, dwType))  dwType = -1;
	int nTypeIndex = dwType;

	if (nTypeIndex == CCM_TYPE_STEEL_I)
	{
		if (csCode == MATLCODE_STL_ASTM ||
			csCode == MATLCODE_STL_EN05 ||
			csCode == MATLCODE_STL_GB50917_13 ||
			csCode == MATLCODE_STL_TB10092_17 ||
			csCode == MATLCODE_STL_JTJ023_85 ||			
			csCode == MATLCODE_STL_ASTM_A416 ||
			csCode == MATLCODE_STL_GB_T_5224 ||
			csCode == MATLCODE_STL_ETC ||
			csCode == MATLCODE_STL_KS_D_7002 ||
			csCode == MATLCODE_STL_EN_10138_3)
		{
			GetDlgItem(IDC_CMD_MP_ITEM_ONLY_STRAND_CHK)->EnableWindow(TRUE);
		}
		else
		{
			m_wndOnlyStrandChk.SetCheck(FALSE);
			GetDlgItem(IDC_CMD_MP_ITEM_ONLY_STRAND_CHK)->EnableWindow(FALSE);
		}
	}
	else
	{
		m_wndOnlyStrandChk.SetCheck(FALSE);
		GetDlgItem(IDC_CMD_MP_ITEM_ONLY_STRAND_CHK)->EnableWindow(FALSE);
	}
}

void CCMMatPageItemDlg::OnCmdMatlIsoStlUserElaChk() 
{
	CString csStlCode;
	m_wndSteelCode.GetWindowText(csStlCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csStlCode);
#endif

	// MQC 12036 : JTG3362-18(S) 일 때만 User Define 버튼에 의해 활성/비활성. 나머지 기준은 User Type이면 항상 활성화
	if(csStlCode == MATLCODE_STL_JTG3362_18)
	{
		BOOL bEnable = (m_wndIsoStlUserElaChk.GetCheck() == 1) ? TRUE : FALSE;
		GetDlgItem(IDC_CMD_MATL_ISO_STL_ELA_EDIT)->EnableWindow(bEnable);
	}
}

void CCMMatPageItemDlg::OnCmdMatlIsoConUserElaChk() 
{
	CString csConCode;
	m_wndConcrCode.GetWindowText(csConCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(csConCode);
#endif

	// "JTG3362-18(RC) 일 때만 User Define 버튼에 의해 활성/비활성. 나머지 기준은 User Type이면 항상 활성화
	if(csConCode == _T("JTG3362-18(RC)"))
	{
		BOOL bEnable = (m_wndIsoConUserElaChk.GetCheck() == 1) ? TRUE : FALSE;
		GetDlgItem(IDC_CMD_MATL_ISO_CON_ELA_EDIT)->EnableWindow(bEnable);
	}
}

void CCMMatPageItemDlg::OnCmdOnlyStrandChk()
{
	OnChangeSteelCode();
}

LRESULT CCMMatPageItemDlg::OnBnClickedCmdMatlFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	EnableDisableUserEla();
	EnableShowOnlyStrand();
	return 0L;
}

void CCMMatPageItemDlg::OnInelasticBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_FIBER_PROP,0),0L);
	SetInelasticCombo();
}