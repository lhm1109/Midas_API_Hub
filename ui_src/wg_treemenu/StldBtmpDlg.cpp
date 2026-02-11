// StldBtmpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldBtmpDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#include "StldBtmpDefCodeProvDlg.h"
#include "..\wg_cmd2\ExportFuncCMD2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_HPOS_Z1    0
#define D_HPOS_Z2    1
#define D_HPOS_Z3    2
/////////////////////////////////////////////////////////////////////////////
// CStldBtmpDlg dialog

CStldBtmpDlg::CStldBtmpDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldBtmpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldBtmpDlg)
	m_nDirection = 1;
	m_nMaterial = 0;
	m_nOption = 0;
	m_nRefPosition = 0;
	m_nType  = 0;
	m_nRef = 0;
	m_nBType = 1;
	m_nH1Type = 1;
	m_nH2Type = 1;
	//}}AFX_DATA_INIT
	m_pData = new T_BTMP_D;
 	m_pBitmap = 0;  
#if defined(_CH)    
	m_bOpenSlope = TRUE;
#else
	m_bOpenSlope = CProduct::GetTestEnvValue(_T("BtmpSlopeTest")) == _T("yes");
#endif
	
}

CStldBtmpDlg::~CStldBtmpDlg()
{
	delete m_pData;
	if (m_pBitmap) delete m_pBitmap;
}


void CStldBtmpDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldBtmpDlg)
	DDX_Control(pDX, IDC_TM_T0_TEMP_UNIT, m_wndT0Unit);
	DDX_Control(pDX, IDC_TM_T0_TEMP_EDIT, m_wndT0Edit);
	DDX_Control(pDX, IDC_TM_B_UNIT, m_wndBUnit);
	DDX_Control(pDX, IDC_TM_B_EDIT, m_wndBEdit);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_SelectLC);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_CobxLdgr);
	DDX_Control(pDX, IDC_TM_H2_UNIT, m_wndH2Unit);
	DDX_Control(pDX, IDC_TM_H2_EDIT, m_wndH2Edit);
	DDX_Control(pDX, IDC_TM_H1_UNIT, m_wndH1Unit);
	DDX_Control(pDX, IDC_TM_H1_EDIT, m_wndH1Edit);
	DDX_Control(pDX, IDC_TM_T2_EDIT, m_wndT2Edit);
	DDX_Control(pDX, IDC_TM_T2_UNIT, m_wndT2Unit);
	DDX_Control(pDX, IDC_TM_T1_UNIT, m_wndT1Unit);
	DDX_Control(pDX, IDC_TM_T1_EDIT, m_wndT1Edit);
	DDX_Control(pDX, IDC_TM_TERM_UNIT, m_wndTermUnit);
	DDX_Control(pDX, IDC_TM_ELST_UNIT, m_wndElstUnit);
	DDX_Control(pDX, IDC_TM_ELST_EDIT, m_wndElstEdit);
	DDX_Control(pDX, IDC_TM_TERM_EDIT, m_wndTermEdit);
	DDX_Control(pDX, IDC_TM_STEMP_LIST, m_wndSTempList);
	DDX_Control(pDX, IDC_TM_STEMP_BLANK_LIST, m_wndSTempBlankList);
	DDX_Radio  (pDX, IDC_TM_DIR_LY_RADIO, m_nDirection);
	DDX_Radio  (pDX, IDC_TM_REF_POS_RADIO, m_nRefPosition);
	DDX_Radio  (pDX, IDC_TM_ELEM_RADIO, m_nMaterial);
	DDX_Radio  (pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Radio  (pDX, IDC_TM_GENRAL_RDO, m_nType);
	DDX_Radio  (pDX, IDC_TM_TOP_RDO,  m_nRef);
	DDX_Radio  (pDX, IDC_TM_BSEC_RDO, m_nBType);
	DDX_Radio  (pDX, IDC_TM_H1Z_RDO,  m_nH1Type);
	DDX_Radio  (pDX, IDC_TM_H2Z_RDO,  m_nH2Type);
	DDX_Control(pDX, IDC_TM_H1Z_CMB, m_cmbH1Z);
	DDX_Control(pDX, IDC_TM_H2Z_CMB, m_cmbH2Z);
	DDX_Control(pDX, IDC_TM_APPLY_BY_CODE_CHK, m_chkApplyByCodeProv);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndDirPosPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldBtmpDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldBtmpDlg)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnTmBtnLc)
	ON_BN_CLICKED(IDC_TM_ELEM_RADIO, OnMaterialOption)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnTmOptChange)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_ADD_BUTTON, OnTmAddButton)
	ON_BN_CLICKED(IDC_TM_DELETE_BUTTON, OnTmDeleteButton)
	ON_BN_CLICKED(IDC_TM_MODIFY_BUTTON, OnTmModifyButton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_STEMP_LIST, OnItemchangedTmStempList)
	ON_BN_CLICKED(IDC_TM_T0_TEMP_BTN, OnTmT0TempBtn)
	ON_BN_CLICKED(IDC_TM_INPUT_RADIO, OnMaterialOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnTmOptChange)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnTmOptChange)
	ON_BN_CLICKED(IDC_TM_DIR_LZ_RADIO, OnTmDirRadio)
	ON_BN_CLICKED(IDC_TM_DIR_LY_RADIO, OnTmDirRadio)  
	ON_BN_CLICKED(IDC_TM_REF_POS_RADIO, OnTmRefPostRadio)
	ON_BN_CLICKED(IDC_TM_REF_POS_RADIO2, OnTmRefPostRadio)
	ON_BN_CLICKED(IDC_TM_REF_POS_RADIO3, OnTmRefPostRadio)
	ON_BN_CLICKED(IDC_TM_GENRAL_RDO, OnTmTypeOption)
	ON_BN_CLICKED(IDC_TM_PSC_RDO,    OnTmTypeOption)
	ON_BN_CLICKED(IDC_TM_BSEC_RDO, OnTmBType)
	ON_BN_CLICKED(IDC_TM_BUSR_RDO, OnTmBType)
	ON_BN_CLICKED(IDC_TM_H1Z_RDO,  OnTmH1Type)
	ON_BN_CLICKED(IDC_TM_H1U_RDO,  OnTmH1Type)
	ON_BN_CLICKED(IDC_TM_H2Z_RDO,  OnTmH2Type)
	ON_BN_CLICKED(IDC_TM_H2U_RDO,  OnTmH2Type)
	ON_BN_CLICKED(IDC_TM_APPLY_BY_CODE_BTN,  OnApplyByCode)  
	ON_BN_CLICKED(IDC_TM_APPLY_BY_CODE_CHK,  OnChkApplyByCode)  
	ON_BN_CLICKED(IDC_TM_SLOPE_OF_PSC_SECT_BTN,  OnSlopeOfPSCSect)    
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CStldBtmpDlg::GetH1Type()
{
	if(m_nType==0 || m_nH1Type==1) return 3;

	DWORD wItem = m_cmbH1Z.GetItemData(m_cmbH1Z.GetCurSel());
	return (int)wItem;
}

int CStldBtmpDlg::GetH2Type()
{
	if(m_nType==0 || m_nH2Type==1) return 3;
	
	DWORD wItem = m_cmbH2Z.GetItemData(m_cmbH2Z.GetCurSel());
	return (int)wItem;
}
/////////////////////////////////////////////////////////////////////////////
// CStldBtmpDlg message handlers

BOOL CStldBtmpDlg::OnInitDialog() 
{
	MInitCombo initCombo;
	SetRedraw(FALSE);

	CMenuBarChildDlg::OnInitDialog();
	
	m_pData->Initialize();

	m_SelectLC.SetLoadType(D_LOADCASE_STATIC,TRUE); // D_SELECTLOAD_????? 사용할것

	InitRcAr();

#if defined(_MGEN)
	GetDlgItem(IDC_TM_TYPE_FRM)  ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_GENRAL_RDO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_PSC_RDO)   ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_REF_STC)   ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_TOP_RDO)   ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_BOT_RDO)   ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_BSEC_RDO)  ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_BUSR_RDO)  ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_H1Z_RDO)   ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_H1Z_CMB)   ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_H1U_RDO)   ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_H2Z_RDO)   ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_H2Z_CMB)   ->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_H2U_RDO)   ->ShowWindow(SW_HIDE);
#endif

	m_cmbH1Z.ResetContent();
	m_cmbH1Z.SetItemData(m_cmbH1Z.AddString(_LSX(Z1)), D_HPOS_Z1);
	m_cmbH1Z.SetItemData(m_cmbH1Z.AddString(_LSX(Z2)), D_HPOS_Z2);
	m_cmbH1Z.SetItemData(m_cmbH1Z.AddString(_LSX(Z3)), D_HPOS_Z3);
	m_cmbH1Z.SetCurSel(0);

	m_cmbH2Z.ResetContent();
	m_cmbH2Z.SetItemData(m_cmbH2Z.AddString(_LSX(Z1)), D_HPOS_Z1);
	m_cmbH2Z.SetItemData(m_cmbH2Z.AddString(_LSX(Z2)), D_HPOS_Z2);
	m_cmbH2Z.SetItemData(m_cmbH2Z.AddString(_LSX(Z3)), D_HPOS_Z3);
	m_cmbH2Z.SetCurSel(1);

	MakeListHeader();
	MoveCtrls();
	MaterialCtrlMan();
	OnTmTypeOption();

	m_wndBUnit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dB);
	m_wndBEdit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dB);
	m_wndH2Unit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dH);
	m_wndH2Edit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dH);
	m_wndH1Unit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dH);
	m_wndH1Edit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dH);
	m_wndT2Edit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dT);
	m_wndT2Unit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dT);
	m_wndT1Unit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dT);
	m_wndT1Edit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dT);
	
	m_wndTermUnit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dThermal);
	m_wndTermEdit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dThermal);
	m_wndElstUnit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dElast  );
	m_wndElstEdit.SetUnitType(CUnitCtrl::m_BTMP_UNIT.dElast  );

	MakeListData();

	m_wndT0Unit.SetUnitType(CUnitCtrl::m_STMP_UNIT.dblSysTemp);
	m_wndT0Edit.SetReadOnly(TRUE);

	if (GetDlgItem(IDC_TM_EXECUTE))
		GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldBtmpDlg::MoveCtrls()
{
	CRect rectCtrl;
	GetDlgItem(IDC_TM_STEMP_LIST)->GetWindowRect(rectCtrl);
	ScreenToClient(rectCtrl);
	GetDlgItem(IDC_TM_STEMP_BLANK_LIST)->SetWindowPos(NULL, rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(), SWP_NOZORDER);

#if defined(_CIVIL)
	int nMarginY;
	if( !m_bOpenSlope )
	{     
		CRect rRef, rMove;
//#if defined (_RUS) // 러시아 리소스와 컨트럴 배치가 완전 달라서 코드가 다르게 들어감
//		GetDlgItem(IDC_TM_SLOPE_OF_PSC_SECT_STC)->GetWindowRect(rRef);
//		GetDlgItem(IDC_TM_TYPE_FRM)->GetWindowRect(rMove);
//		nMarginY = rMove.top - rRef.top;
//#else
		GetDlgItem(IDC_TM_SLOPE_OF_PSC_SECT_STC)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_PICTURE)->GetWindowRect(rMove);
		nMarginY = rMove.top - rRef.top;
//#endif
		CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrl4Slope, -nMarginY);
	}
#endif
}

void CStldBtmpDlg::InitRcAr()
{
	m_aRcEnabeByApplyCodeProv.RemoveAll();
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC2);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_DIR_LY_RADIO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_DIR_LZ_RADIO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC20);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_REF_POS_RADIO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_REF_POS_RADIO2);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_REF_POS_RADIO3);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC3);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC10);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_T0_TEMP_EDIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_T0_TEMP_UNIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_T0_TEMP_BTN);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC4);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_ELEM_RADIO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_INPUT_RADIO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC5);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_ELST_EDIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_ELST_UNIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC6);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_TERM_EDIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_TERM_UNIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_REF_STC);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_TOP_RDO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_BOT_RDO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC7);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_BSEC_RDO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_BUSR_RDO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_B_EDIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_B_UNIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC8);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_H1Z_RDO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_H1Z_CMB);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_H1U_RDO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_H1_EDIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_H1_UNIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC9);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_H2Z_RDO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_H2Z_CMB);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_H2U_RDO);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_H2_EDIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_H2_UNIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC14);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_T1_EDIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_T1_UNIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STATIC15);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_T2_EDIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_T2_UNIT);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_ADD_BUTTON);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_MODIFY_BUTTON);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_DELETE_BUTTON);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STEMP_LIST);
	m_aRcEnabeByApplyCodeProv.Add(IDC_TM_STEMP_BLANK_LIST);  

	m_aMoveCtrl4Rus.RemoveAll();
	m_aMoveCtrl4Rus.Add(IDC_TM_STATIC3);
	m_aMoveCtrl4Rus.Add(IDC_TM_STATIC10);
	m_aMoveCtrl4Rus.Add(IDC_TM_T0_TEMP_EDIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_T0_TEMP_UNIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_T0_TEMP_BTN);
	m_aMoveCtrl4Rus.Add(IDC_TM_STATIC4);
	m_aMoveCtrl4Rus.Add(IDC_TM_ELEM_RADIO);
	m_aMoveCtrl4Rus.Add(IDC_TM_INPUT_RADIO);
	m_aMoveCtrl4Rus.Add(IDC_TM_STATIC5);
	m_aMoveCtrl4Rus.Add(IDC_TM_ELST_EDIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_ELST_UNIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_STATIC6);
	m_aMoveCtrl4Rus.Add(IDC_TM_TERM_EDIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_TERM_UNIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_REF_STC);
	m_aMoveCtrl4Rus.Add(IDC_TM_TOP_RDO);
	m_aMoveCtrl4Rus.Add(IDC_TM_BOT_RDO);
	m_aMoveCtrl4Rus.Add(IDC_TM_STATIC7);
	m_aMoveCtrl4Rus.Add(IDC_TM_BSEC_RDO);
	m_aMoveCtrl4Rus.Add(IDC_TM_BUSR_RDO);
	m_aMoveCtrl4Rus.Add(IDC_TM_B_EDIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_B_UNIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_STATIC8);
	m_aMoveCtrl4Rus.Add(IDC_TM_H1Z_RDO);
	m_aMoveCtrl4Rus.Add(IDC_TM_H1Z_CMB);
	m_aMoveCtrl4Rus.Add(IDC_TM_H1U_RDO);
	m_aMoveCtrl4Rus.Add(IDC_TM_H1_EDIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_H1_UNIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_STATIC9);
	m_aMoveCtrl4Rus.Add(IDC_TM_H2Z_RDO);
	m_aMoveCtrl4Rus.Add(IDC_TM_H2Z_CMB);
	m_aMoveCtrl4Rus.Add(IDC_TM_H2U_RDO);
	m_aMoveCtrl4Rus.Add(IDC_TM_H2_EDIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_H2_UNIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_STATIC14);
	m_aMoveCtrl4Rus.Add(IDC_TM_T1_EDIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_T1_UNIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_STATIC15);
	m_aMoveCtrl4Rus.Add(IDC_TM_T2_EDIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_T2_UNIT);
	m_aMoveCtrl4Rus.Add(IDC_TM_ADD_BUTTON);
	m_aMoveCtrl4Rus.Add(IDC_TM_MODIFY_BUTTON);
	m_aMoveCtrl4Rus.Add(IDC_TM_DELETE_BUTTON);
	m_aMoveCtrl4Rus.Add(IDC_TM_STEMP_LIST);
	m_aMoveCtrl4Rus.Add(IDC_TM_STEMP_BLANK_LIST);  
	m_aMoveCtrl4Rus.Add(IDC_TM_EXECUTE);
	m_aMoveCtrl4Rus.Add(IDC_TM_CLOSE);  

	m_aMoveCtrl4Slope.RemoveAll();  
	m_aMoveCtrl4Slope.Copy(m_aMoveCtrl4Rus);
	m_aMoveCtrl4Slope.Add(IDC_TM_STATIC20);
	m_aMoveCtrl4Slope.Add(IDC_TM_REF_POS_RADIO);
	m_aMoveCtrl4Slope.Add(IDC_TM_REF_POS_RADIO2);
	m_aMoveCtrl4Slope.Add(IDC_TM_REF_POS_RADIO3);
	m_aMoveCtrl4Slope.Add(IDC_TM_STATIC2);
	m_aMoveCtrl4Slope.Add(IDC_TM_DIR_LY_RADIO);
	m_aMoveCtrl4Slope.Add(IDC_TM_DIR_LZ_RADIO);
	m_aMoveCtrl4Slope.Add(IDC_TM_PICTURE);
//#if defined (_RUS)
//	m_aMoveCtrl4Slope.Add(IDC_TM_TYPE_FRM);
//	m_aMoveCtrl4Slope.Add(IDC_TM_GENRAL_RDO);
//	m_aMoveCtrl4Slope.Add(IDC_TM_PSC_RDO);
//#endif
}

void CStldBtmpDlg::MakeListHeader()
{
	this->m_wndSTempList;
	CStringArray HTitles;
	CArray<float, float> HRatio;
	HTitles.Add(_T("No.")); HRatio.Add(0.18f);
#if defined(_CIVIL)
	HTitles.Add(_LS(IDS_IDD_TM_STLD_BTMP_IDC_TM_REF_STC)); HRatio.Add(0.20f);
#endif
	HTitles.Add(_T("B")  ); HRatio.Add(0.20f);
	HTitles.Add(_T("H1") ); HRatio.Add(0.20f);
	HTitles.Add(_T("H2") ); HRatio.Add(0.20f);

	CDlgUtil::_SetListCtrlHeader(&m_wndSTempList,HTitles,&HRatio,NULL);

	// ApplyByCodeProv CheckBox가 체크되었을때 빈 list로 보여지게 하기 위해서 list를 하나 더 만들어 show hide 함
	this->m_wndSTempBlankList;
	HTitles.RemoveAll();
	HRatio.RemoveAll();
	HTitles.Add(_T("No.")); HRatio.Add(0.18f);
#if defined(_CIVIL)
	HTitles.Add(_LS(IDS_IDD_TM_STLD_BTMP_IDC_TM_REF_STC)); HRatio.Add(0.20f);
#endif
	HTitles.Add(_T("B")  ); HRatio.Add(0.20f);
	HTitles.Add(_T("H1") ); HRatio.Add(0.20f);
	HTitles.Add(_T("H2") ); HRatio.Add(0.20f);

	CDlgUtil::_SetListCtrlHeader(&m_wndSTempBlankList,HTitles,&HRatio,NULL);
}

int CStldBtmpDlg::FindEndSectTemp()
{
	for(int i = 0;i < m_pData->SectTmp.GetSize(); i++)
	{
		if(m_pData->SectTmp[i].nMaterial == 0)
			return i;
	}
	
	return -1;  // Section Temperature Data 최대 개수 초과 
}

BOOL CStldBtmpDlg::GetSectTemp(T_BTMP_STMP * pSectTempD)
{
	pSectTempD->nMaterial = m_nMaterial +1;

	pSectTempD->nRef     = m_nRef;
	pSectTempD->nB       = (m_nType==0) ? 1 : m_nBType;
	pSectTempD->dB       = m_wndBEdit.GetEditValue();
	pSectTempD->nH1      = (m_nType==0) ? 3 : GetH1Type();
	pSectTempD->dH1      = m_wndH1Edit.GetEditValue();
	pSectTempD->nH2      = (m_nType==0) ? 3 : GetH2Type();
	pSectTempD->dH2      = m_wndH2Edit.GetEditValue();
	pSectTempD->dT1      = m_wndT1Edit.GetEditValue();
	pSectTempD->dT2      = m_wndT2Edit.GetEditValue();

	if(pSectTempD->nMaterial == 1) // Element 
	{
		pSectTempD->dElast   = 0;
	  pSectTempD->dThermal = 0;
	}
	else
	{
		pSectTempD->dElast   = m_wndElstEdit.GetEditValue();
	  pSectTempD->dThermal = m_wndTermEdit.GetEditValue();
	}

	return TRUE;
}

BOOL CStldBtmpDlg::ModifySecTemp()
{
	UpdateData(TRUE);
	CArray<int, int> arRet;
	CDlgUtil::GetSelectedListItemIndex(&m_wndSTempList, arRet);
	int nRet = arRet.GetSize();
	if(nRet == 0) return FALSE;
	
	for(int i = 0;i < nRet; i++)
	{
		T_BTMP_STMP * pStmp = (T_BTMP_STMP*) m_wndSTempList.GetItemData(arRet[i]);
		if(pStmp)
		{
			GetSectTemp(pStmp);
		}
		
	}
	MakeListData();   
	return TRUE;
}

BOOL CStldBtmpDlg::DeleteSecTemp()
{
	CArray<int, int> arRet;
	CDlgUtil::GetSelectedListItemIndex(&m_wndSTempList, arRet);
	int nRet = arRet.GetSize();
	if(nRet == 0) return FALSE;

	T_BTMP_STMP * pStmp;
	for(int i = 0; i < nRet; i++)
	{
		pStmp = (T_BTMP_STMP*) m_wndSTempList.GetItemData(arRet[i]);  
		if(pStmp)
			pStmp->Initialize();
	}

	for (int i = 0; i < m_pData->SectTmp.GetSize(); i++)
	{
		m_pData->SectTmp[i].Initialize();
	}  

	MakeListData();   
	return TRUE;
}

BOOL CStldBtmpDlg::AddSecTemp()
{
	UpdateData(TRUE);
	int nEndSecTemp = FindEndSectTemp();
	if( nEndSecTemp < 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_ERROR_BTMP_SIZE_LIMIT), D_BTMP_NUMSTMP); // 지정할 수 있는 Section Temperature Data개수는 %d개 입니다
		return FALSE;
	}

	//pBtmpD->SectTmp[nEndSecTemp];
	GetSectTemp(&(m_pData->SectTmp[nEndSecTemp]));
	MakeListData();
	return TRUE;
}


BOOL CStldBtmpDlg::GetParameter(T_BTMP_D * pBtmpD)
{
	UpdateData();

	unsigned int nLoadCaseType;
	if (!m_SelectLC.GetSelectedLoad(nLoadCaseType, pBtmpD->LoadCaseKey))
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Loadcase_is_not_selected_));
		return FALSE;
	}

	T_LDGR_K LdgrK;
	m_CobxLdgr.GetSelectedLdgr(LdgrK);
	pBtmpD->GroupKey     = LdgrK;
	pBtmpD->nDirection   = m_nDirection;
	pBtmpD->nRefPosition = m_nRefPosition;
	pBtmpD->nType = m_nType;
	if(m_nType==1)
	{
		pBtmpD->nDirection   = 1; // PSC일 경우 local z로 고정
		pBtmpD->nRefPosition = 1; // PSC일 경우 +End(Top)으로 고정
	}

	return TRUE;
}

void CStldBtmpDlg::MakeListData()
{
	m_wndSTempList.DeleteAllItems();
	CStringArray Contents;
	CString      StrTemp ;
	for(int i = 0;i < m_pData->SectTmp.GetSize(); i++)
	{
		if(m_pData->SectTmp[i].nMaterial == 0)
			break;
		Contents.RemoveAll();

		StrTemp.Format(_T("%d"),i+1);
		Contents.Add(StrTemp);
#if defined(_CIVIL)
		if(m_pData->nType==0)
		{
			if     (m_pData->nRefPosition==0) StrTemp = _T("C");
			else if(m_pData->nRefPosition==1) StrTemp = _T("T");
			else if(m_pData->nRefPosition==2) StrTemp = _T("B");
		}
		else
		{
		  StrTemp	= (m_pData->SectTmp[i].nRef==0) ? _T("T") : _T("B");
		}
		Contents.Add(StrTemp);
#endif
		if(m_pData->nType==0)
		{
			StrTemp.Format(_T("%g"),m_pData->SectTmp[i].dB);
			Contents.Add(StrTemp);
			StrTemp.Format(_T("%g"),m_pData->SectTmp[i].dH1);
			Contents.Add(StrTemp);
			StrTemp.Format(_T("%g"),m_pData->SectTmp[i].dH2);
			Contents.Add(StrTemp);
		}
		else
		{
			if(m_pData->SectTmp[i].nB==0) StrTemp = _T("S");
			else StrTemp.Format(_T("%g"),m_pData->SectTmp[i].dB);
			Contents.Add(StrTemp);
			if(m_pData->SectTmp[i].nH1==3) StrTemp.Format(_T("%g"),m_pData->SectTmp[i].dH1);
			else
			{
				if     (m_pData->SectTmp[i].nH1==0) StrTemp = _T("Z1");
				else if(m_pData->SectTmp[i].nH1==1) StrTemp = _T("Z2");
				else if(m_pData->SectTmp[i].nH1==2) StrTemp = _T("Z3");
				else ASSERT(0);
			}
			Contents.Add(StrTemp);
			if(m_pData->SectTmp[i].nH2==3) StrTemp.Format(_T("%g"),m_pData->SectTmp[i].dH2);
			else
			{
				if     (m_pData->SectTmp[i].nH2==0) StrTemp = _T("Z1");
				else if(m_pData->SectTmp[i].nH2==1) StrTemp = _T("Z2");
				else if(m_pData->SectTmp[i].nH2==2) StrTemp = _T("Z3");
				else ASSERT(0);
			}
			Contents.Add(StrTemp);
		}
		CDlgUtil::SetListItem(&m_wndSTempList,i,Contents,(DWORD_PTR)((m_pData->SectTmp.GetData())+i));
	}
}

void CStldBtmpDlg::SetShowHide()
{
	BOOL bCodeProv = GetChkApplyByCodeProv();

// 	CDlgUtil::CtrlShowHideByRect(this, IDC_TM_STATIC2,  m_nType==0, TRUE);
// 	CDlgUtil::CtrlShowHideByRect(this, IDC_TM_STATIC20, m_nType==0, TRUE);
	CArray<UINT, UINT> m_aSTATIC2;
	m_aSTATIC2.Add(IDC_TM_STATIC2);
	m_aSTATIC2.Add(IDC_TM_DIR_LY_RADIO);
	m_aSTATIC2.Add(IDC_TM_DIR_LZ_RADIO);
	CDlgUtil::CtrlShowHide(this, m_aSTATIC2, m_nType==0);

	CArray<UINT, UINT> m_aSTATIC20;
	m_aSTATIC20.Add(IDC_TM_STATIC20);
	m_aSTATIC20.Add(IDC_TM_REF_POS_RADIO);
	m_aSTATIC20.Add(IDC_TM_REF_POS_RADIO2);
	m_aSTATIC20.Add(IDC_TM_REF_POS_RADIO3);
	CDlgUtil::CtrlShowHide(this, m_aSTATIC20, m_nType==0);

	GetDlgItem(IDC_TM_STEMP_LIST)->ShowWindow(!bCodeProv);
	GetDlgItem(IDC_TM_STEMP_BLANK_LIST)->ShowWindow(bCodeProv);  

	GetDlgItem(IDC_TM_SLOPE_OF_PSC_SECT_STC)->ShowWindow(m_bOpenSlope);  
	GetDlgItem(IDC_TM_SLOPE_OF_PSC_SECT_BTN)->ShowWindow(m_bOpenSlope);


	CRect rRef, rMove;
	int nDistY;
	GetDlgItem(IDC_TM_STATIC2)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_STATIC3)->GetWindowRect(rMove);
	if (m_nType == 0)
	{
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
	}
	else
	{
		nDistY = rRef.top - rMove.top;
	}
	CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrl4Rus, nDistY);
}

BOOL CStldBtmpDlg::GetChkApplyByCodeProv()
{
	return m_chkApplyByCodeProv.GetCheck() && m_chkApplyByCodeProv.IsWindowEnabled();
}

void CStldBtmpDlg::SetEnable()
{ 
	UpdateData();

	if(CProduct::GetTestEnvValue(_T("BtmpRusCodeHTest")) != _T("yes"))
	{
		GetDlgItem(IDC_TM_APPLY_BY_CODE_CHK)->EnableWindow(m_nType==1); // m_chkApplyByCodeProv의 Enable여부에 따라서 Enable변경이 많아서 위에서 먼저 처리함..
	}
	
	BOOL bChkApplyByCodeProv = GetChkApplyByCodeProv();
	if(!bChkApplyByCodeProv)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aRcEnabeByApplyCodeProv, TRUE);  // 컨트럴 개수가 너무 많아서 따로 하는게 나은거 같음..
	}
	
	GetDlgItem(IDC_TM_REF_STC)->EnableWindow(m_nType==1 && (m_nOption != 2 && m_nType==1));
	GetDlgItem(IDC_TM_TOP_RDO)->EnableWindow(m_nType==1 && (m_nOption != 2 && m_nType==1));
	GetDlgItem(IDC_TM_BOT_RDO)->EnableWindow(m_nType==1 && (m_nOption != 2 && m_nType==1));
	GetDlgItem(IDC_TM_BSEC_RDO)->EnableWindow(m_nType==1 && (m_nOption != 2 && m_nType==1));
	GetDlgItem(IDC_TM_BUSR_RDO)->EnableWindow(m_nType==1 && (m_nOption != 2 && m_nType==1));
	GetDlgItem(IDC_TM_B_EDIT)  ->EnableWindow(m_nBType==1);
	GetDlgItem(IDC_TM_B_UNIT)  ->EnableWindow(m_nBType==1);
	GetDlgItem(IDC_TM_H1Z_RDO)->EnableWindow(m_nType==1 && (m_nOption != 2 && m_nType==1));
	GetDlgItem(IDC_TM_H1Z_CMB)->EnableWindow(m_nType==1 && m_nH1Type==0 && (m_nOption != 2 && m_nType==1));
	GetDlgItem(IDC_TM_H1U_RDO)->EnableWindow(m_nType==1 && (m_nOption != 2 && m_nType==1));
	GetDlgItem(IDC_TM_H1_EDIT)->EnableWindow(m_nH1Type==1);
	GetDlgItem(IDC_TM_H1_UNIT)->EnableWindow(m_nH1Type==1);
	GetDlgItem(IDC_TM_H2Z_RDO)->EnableWindow(m_nType==1 && (m_nOption != 2 && m_nType==1));
	GetDlgItem(IDC_TM_H2Z_CMB)->EnableWindow(m_nType==1 && m_nH2Type==0 && (m_nOption != 2 && m_nType==1));
	GetDlgItem(IDC_TM_H2U_RDO)->EnableWindow(m_nType==1 && (m_nOption != 2 && m_nType==1));
	GetDlgItem(IDC_TM_H2_EDIT)->EnableWindow(m_nH2Type==1);
	GetDlgItem(IDC_TM_H2_UNIT)->EnableWindow(m_nH2Type==1);  
	GetDlgItem(IDC_TM_ELST_EDIT)->EnableWindow(m_nMaterial == 1);
	GetDlgItem(IDC_TM_TERM_EDIT)->EnableWindow(m_nMaterial == 1);
	GetDlgItem(IDC_TM_GENRAL_RDO)->EnableWindow(m_nOption != 2);
	GetDlgItem(IDC_TM_PSC_RDO)->EnableWindow(m_nOption != 2);
	GetDlgItem(IDC_TM_DIR_LY_RADIO)->EnableWindow(m_nOption != 2);
	GetDlgItem(IDC_TM_DIR_LZ_RADIO)->EnableWindow(m_nOption != 2);
	GetDlgItem(IDC_TM_REF_POS_RADIO)->EnableWindow(m_nOption != 2);
	GetDlgItem(IDC_TM_REF_POS_RADIO2)->EnableWindow(m_nOption != 2);
	GetDlgItem(IDC_TM_REF_POS_RADIO3)->EnableWindow(m_nOption != 2);
	GetDlgItem(IDC_TM_APPLY_BY_CODE_BTN)->EnableWindow(bChkApplyByCodeProv);  

	GetDlgItem(IDC_TM_APPLY_BY_CODE_BTN)->EnableWindow(bChkApplyByCodeProv);  

	GetDlgItem(IDC_TM_SLOPE_OF_PSC_SECT_STC)->EnableWindow(m_nType == 1);
	GetDlgItem(IDC_TM_SLOPE_OF_PSC_SECT_BTN)->EnableWindow(m_nType == 1);

//#if defined(_CH) MQC A/B 5193-1 : 중국에서 열어달라고 해서 열어줌
//  GetDlgItem(IDC_TM_PSC_RDO)->EnableWindow(FALSE);
//#endif

	if(bChkApplyByCodeProv)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aRcEnabeByApplyCodeProv, FALSE); 
	}
}

void CStldBtmpDlg::MaterialCtrlMan()
{
	SetEnable();
}

void CStldBtmpDlg::OptionCtrlMan()
{  
	SetEnable();
	
	if(m_nOption != 2)
	{
		MaterialCtrlMan();
	}

}

void CStldBtmpDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMLoadGroupDlg::IDD);		
}

void CStldBtmpDlg::OnTmBtnLc() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));	
}

void CStldBtmpDlg::OnMaterialOption() 
{
	UpdateData();
	MaterialCtrlMan();
}

void CStldBtmpDlg::OnTmOptChange() 
{
	UpdateData();
	OptionCtrlMan();
}

void CStldBtmpDlg::OnTmExecute() 
{
	CArray<UINT, UINT> aSelKey;
	T_ELEM_D ElemData;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(aSelKey[i], ElemData);
		if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsFrameType(ElemData.eltyp))
			aSelKey.RemoveAt(i);
	}
	
	if (aSelKey.GetSize() == 0)
	{
		AfxMessageBox(_T("No selected frame element"));
		return;
	}

	BOOL bSuccess = FALSE;
	if(m_nOption == 0 || m_nOption == 1) //  Add/Replace
	{
		if(!GetParameter(m_pData))
		{
			return;
		}

		if(GetChkApplyByCodeProv())
		{
			if(!CStldBtmpDefCodeProvUtil::AddBtmpByCodeProv(*m_pData, m_DefCodeProvD, aSelKey, m_nOption == 1))
			{
				return;
			}
		}
		else
		{
			bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->AddBtmp(aSelKey, *m_pData, m_nOption == 1);
		}    
	}
	else // Delete
	{
		T_STLD_K KeyStld;
		T_LDGR_K KeyLdgr;
		unsigned int nLoadCaseType;
		m_SelectLC.GetSelectedLoad(nLoadCaseType,KeyStld);
		m_CobxLdgr.GetSelectedLdgr(KeyLdgr);
		bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->DelBtmp(aSelKey, KeyStld,KeyLdgr);
	}

	if (bSuccess) CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
}

void CStldBtmpDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldBtmpDlg::OnTmAddButton() 
{
	UpdateData();
	AddSecTemp();
}

void CStldBtmpDlg::OnTmDeleteButton() 
{
	UpdateData();
	DeleteSecTemp();
}

void CStldBtmpDlg::OnTmModifyButton() 
{
	UpdateData();
	ModifySecTemp();
}

BOOL CStldBtmpDlg::SetSectTemp(T_BTMP_STMP * pSectTempD)
{
	m_nMaterial  =  pSectTempD->nMaterial - 1;
	m_nBType     = (m_nType==0) ? 1 : pSectTempD->nB;
	m_nH1Type    = (m_nType==0) ? 1 : ((pSectTempD->nH1==3) ? 1 : 0);
	m_nH2Type    = (m_nType==0) ? 1 : ((pSectTempD->nH2==3) ? 1 : 0);
	UpdateData(FALSE);
	MaterialCtrlMan();
	OnTmBType();
	OnTmH1Type();
	OnTmH2Type();

	if(m_nType==1 && m_nH1Type==0) m_cmbH1Z.SetCurSel(pSectTempD->nH1);
	else                           m_cmbH1Z.SetCurSel(0);
	if(m_nType==1 && m_nH2Type==0) m_cmbH2Z.SetCurSel(pSectTempD->nH2);
	else                           m_cmbH2Z.SetCurSel(0);

	m_wndBEdit   .SetEditUnit(pSectTempD->dB        );
	m_wndH1Edit  .SetEditUnit(pSectTempD->dH1       );
	m_wndH2Edit  .SetEditUnit(pSectTempD->dH2       );
	m_wndT1Edit  .SetEditUnit(pSectTempD->dT1       );
	m_wndT2Edit  .SetEditUnit(pSectTempD->dT2       );
	m_wndElstEdit.SetEditUnit(pSectTempD->dElast    );
	m_wndTermEdit.SetEditUnit(pSectTempD->dThermal  );

	return TRUE;
}

void CStldBtmpDlg::OnItemchangedTmStempList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CArray<int, int> arRet;
	CDlgUtil::GetSelectedListItemIndex(&m_wndSTempList, arRet);
	int nRet = arRet.GetSize();
	if(nRet != 1) return;

	SetSectTemp((T_BTMP_STMP*)m_wndSTempList.GetItemData(arRet[0]));
}

void CStldBtmpDlg::OnTmT0TempBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_CONFIG,0));
}


void CStldBtmpDlg::DoUnitChange()
{
	for(int i = 0;i < m_pData->SectTmp.GetSize(); i++)
	{
		if(m_pData->SectTmp[i].nMaterial == 0)
			break;
		m_pData->SectTmp[i].dB  = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthPrevious(m_pData->SectTmp[i].dB);
		m_pData->SectTmp[i].dH1 = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthPrevious(m_pData->SectTmp[i].dH1);
		m_pData->SectTmp[i].dH2 = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthPrevious(m_pData->SectTmp[i].dH2);;
	}
	MakeListData();
}


void CStldBtmpDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
		HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		//UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		DoUnitChange();
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

void CStldBtmpDlg::UpdateBuffer()
{
}

void CStldBtmpDlg::OnTmDirRadio() 
{
	UpdateData(TRUE);
	m_pData->nDirection = m_nDirection;
	ChangeBitmap(m_nDirection, m_nRefPosition);	
	MakeListData();
}

void CStldBtmpDlg::OnTmRefPostRadio()
{
	UpdateData(TRUE);
	m_pData->nRefPosition = m_nRefPosition;
	ChangeBitmap(m_nDirection, m_nRefPosition);	
	MakeListData();
}

void CStldBtmpDlg::ChangeBitmap(int nDir, int nPos)
{ 
#if 0
	UINT aBitmapID[] = {IDB_TM_BTMP2, IDB_TM_BTMP, IDB_TM_BTMP5, IDB_TM_BTMP6, IDB_TM_BTMP3, IDB_TM_BTMP4, IDB_TM_BTMP_PSC };
	if(m_pBitmap != 0) delete m_pBitmap;

	int nIndex = 0;
	if(nDir==100 && nPos==100) nIndex = 6;
	else nIndex = nPos * 2 + nDir; 

	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_TM_PICTURE);
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[nIndex]);
	pImage->SetBitmap(HBITMAP(*m_pBitmap));
#else

	CString aDirPosPictures[] = {
		_T("SVG\\illustration\\Dialog\\tm_btmp2.svg"),
		_T("SVG\\illustration\\Dialog\\tm_btmp.svg"),
		_T("SVG\\illustration\\Dialog\\tm_btmp5.svg"),
		_T("SVG\\illustration\\Dialog\\tm_btmp6.svg"),
		_T("SVG\\illustration\\Dialog\\tm_btmp3.svg"),
		_T("SVG\\illustration\\Dialog\\tm_btmp4.svg"),
		_T("SVG\\illustration\\Dialog\\tm_btmp_psc.svg")
	};

	int nIndex = 0;
	if (nDir == 100 && nPos == 100) nIndex = 6;
	else nIndex = nPos * 2 + nDir;

	// svg이므로 psc 타입일 때 Window 크기를 바꿔줘야 한다. 
	m_wndDirPosPicture.SetImage(aDirPosPictures[nIndex]);
	
#endif
}

void CStldBtmpDlg::OnTmTypeOption()
{
	UpdateData(TRUE);

	m_pData->nType = m_nType;
	if(m_nType==0)
	{
		ChangeBitmap(m_nDirection, m_nRefPosition);
		m_nBType = m_nH1Type = m_nH2Type = 1; // user
		UpdateData(FALSE);
	}
	else
	{
		ChangeBitmap(100, 100);
	}

	SetShowHide();
	SetEnable();

	//CArray<UINT, UINT> aControls; aControls.RemoveAll();
	//aControls.Copy(m_aMoveCtrl4Rus);
	////aControls.Add -> 그룹 박스
	////aControls.Add -> 라디오
	////aControls.Add -> 라디오
	//
	//int nDistY = 0;
	//
	////GetDlgItem(Slope)->Getwindowrect()
	////GetDlgItem(그룹 박스)->Getwindowrect()
	//CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	
//#if defined (_RUS)
//	MoveCtrls();
//
//	int nMarginY;
//	if( m_nType == 0)
//	{ 
//		CRect rRef, rMove;
//		GetDlgItem(IDC_TM_STATIC20)->GetWindowRect(rRef);
//		GetDlgItem(IDC_TM_STATIC3)->GetWindowRect(rMove);
//		nMarginY = rMove.top - (rRef.bottom + globalUtils.ScaleByDPI(3));
//
//		CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrl4Rus, -nMarginY);
//	}
//	else if( m_nType == 1 )
//	{
//		CRect rRef, rMove;
//		GetDlgItem(IDC_TM_STATIC2)->GetWindowRect(rRef);
//		GetDlgItem(IDC_TM_STATIC3)->GetWindowRect(rMove);
//		nMarginY = rMove.top - rRef.top;
//
//		CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrl4Rus, -nMarginY);
//	}
//	
//#endif

	// initialize data
	for(int i=0; i<m_pData->SectTmp.GetSize(); ++i) m_pData->SectTmp[i].Initialize();
	MakeListData();

	RedrawWindow();
}

void CStldBtmpDlg::OnTmBType()
{
	SetEnable();
}

void CStldBtmpDlg::OnTmH1Type()
{
	SetEnable();
}

void CStldBtmpDlg::OnTmH2Type()
{
	SetEnable();
}

void CStldBtmpDlg::OnApplyByCode()
{
	CStldBtmpDefCodeProvDlg dlg;
	dlg.SetDlgData(m_DefCodeProvD);
	if(dlg.DoModal() == IDOK)
	{
		m_DefCodeProvD = dlg.GetGenData();
	}
}

void CStldBtmpDlg::OnChkApplyByCode()
{
	OnTmTypeOption();
// 	SetShowHide();
// 	SetEnable();
}

void CStldBtmpDlg::OnSlopeOfPSCSect()
{
	CMD2_DoModal(D_CMD2_SECTION_MANAGER_MAIN_DLG, NULL, 10);
}
