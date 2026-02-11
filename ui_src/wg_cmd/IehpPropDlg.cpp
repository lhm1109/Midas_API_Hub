// IehpPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IehpPropDlg.h"
#include "IehpPropDeformation.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DbToolPmcv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIehpPropDlg dialog


CIehpPropDlg::CIehpPropDlg(int nTabStyle, CWnd* pParent /*=NULL*/)
	: CDialogMove(CIehpPropDlg::IDD, pParent)
{
	m_pDoc = 0;
	//{{AFX_DATA_INIT(CIehpPropDlg)
	m_str1Plus = _T("");
	m_str1Mnus = _T("");
	m_str2Mnus = _T("");
	m_str2Plus = _T("");
	m_nUserAuto = 1;
	m_nSymmetry = 1;
	m_nStiffType = -1;
	m_nDeformDefineType = -1;
	m_nAlphaDelta = -1;
	m_nValueTypeSymmetry = 0;
	m_bAssignProperties = FALSE;
	//}}AFX_DATA_INIT

	// 사용되지 않음
	m_aIntractionNone.RemoveAll();
	m_aIntractionNone.Add(IDC_YIELD_USER_RDO);
	m_aIntractionNone.Add(IDC_YIELD_AUTO_RDO);
	m_aIntractionNone.Add(IDC_IEHP_CRACK_PLUS_EDIT);
	m_aIntractionNone.Add(IDC_IEHP_CRACK_MNUS_EDIT);
	m_aIntractionNone.Add(IDC_IEHP_CRACK_UNIT);
	m_aIntractionNone.Add(IDC_IEHP_YIELD_PLUS_EDIT);
	m_aIntractionNone.Add(IDC_IEHP_YIELD_MNUS_EDIT);
	m_aIntractionNone.Add(IDC_IEHP_YIELD_UNIT);
	m_aIntractionNone.Add(IDC_IEHP_ULTI_PLUS_EDIT);
	m_aIntractionNone.Add(IDC_IEHP_ULTI_MNUS_EDIT);
	m_aIntractionNone.Add(IDC_IEHP_ULTI_UNIT);
	m_aIntractionNone.Add(IDC_IEHP_FAIL_PLUS_EDIT);
	m_aIntractionNone.Add(IDC_IEHP_FAIL_MNUS_EDIT);
	m_aIntractionNone.Add(IDC_IEHP_FAIL_UNIT);

	m_aInitStiff.RemoveAll();
	m_aInitStiff.Add(IDC_STIFF_6EI_RDO);
	m_aInitStiff.Add(IDC_STIFF_3EI_RDO);
	m_aInitStiff.Add(IDC_STIFF_2EI_RDO);
	m_aInitStiff.Add(IDC_STIFF_USER_RDO);
	m_aInitStiff.Add(IDC_STIFF_USER_EDIT);
	m_aInitStiff.Add(IDC_STIFF_USER_UNIT);
	m_aInitStiff.Add(IDC_STIFF_ESTIF_RDO);
	m_aInitStiff.Add(IDC_STIFF_SCURV_RDO);    

	m_aCtrlP1.RemoveAll();
	m_aCtrlP1.Add(IDC_IEHP_STATIC_CRACK);
	m_aCtrlP1.Add(IDC_IEHP_CRACK_PLUS_EDIT);
	m_aCtrlP1.Add(IDC_IEHP_CRACK_MNUS_EDIT);
	m_aCtrlP1.Add(IDC_IEHP_CRACK_UNIT);
	
	m_aCtrlP2.RemoveAll();
	m_aCtrlP2.Add(IDC_IEHP_STATIC_FY);
	m_aCtrlP2.Add(IDC_IEHP_YIELD_PLUS_EDIT);
	m_aCtrlP2.Add(IDC_IEHP_YIELD_MNUS_EDIT);
	m_aCtrlP2.Add(IDC_IEHP_YIELD_UNIT);
		
	m_aCtrlP3.RemoveAll();
	m_aCtrlP3.Add(IDC_IEHP_STATIC_FU);
	m_aCtrlP3.Add(IDC_IEHP_ULTI_PLUS_EDIT);
	m_aCtrlP3.Add(IDC_IEHP_ULTI_MNUS_EDIT);
	m_aCtrlP3.Add(IDC_IEHP_ULTI_UNIT);

	m_aCtrlP4.RemoveAll();
	m_aCtrlP4.Add(IDC_IEHP_STATIC_FF);
	m_aCtrlP4.Add(IDC_IEHP_FAIL_PLUS_EDIT);
	m_aCtrlP4.Add(IDC_IEHP_FAIL_MNUS_EDIT);
	m_aCtrlP4.Add(IDC_IEHP_FAIL_UNIT);

	m_aCtrlD0.RemoveAll();
	m_aCtrlD0.Add(IDC_DISP_PLUS_STC);
	m_aCtrlD0.Add(IDC_DISP_MNUS_STC);

	m_aCtrlD1.RemoveAll();
	m_aCtrlD1.Add(IDC_IEHP_D1_STATIC);
	m_aCtrlD1.Add(IDC_IEHP_D1_PLUS_EDIT);
	m_aCtrlD1.Add(IDC_IEHP_D1_MNUS_EDIT);
	m_aCtrlD1.Add(IDC_IEHP_D1_UNIT);
	
	m_aCtrlD2.RemoveAll();
	m_aCtrlD2.Add(IDC_IEHP_D2_STATIC);
	m_aCtrlD2.Add(IDC_IEHP_D2_PLUS_EDIT);
	m_aCtrlD2.Add(IDC_IEHP_D2_MNUS_EDIT);
	m_aCtrlD2.Add(IDC_IEHP_D2_UNIT);
		
	m_aCtrlD3.RemoveAll();
	m_aCtrlD3.Add(IDC_IEHP_D3_STATIC);
	m_aCtrlD3.Add(IDC_IEHP_D3_PLUS_EDIT);
	m_aCtrlD3.Add(IDC_IEHP_D3_MNUS_EDIT);
	m_aCtrlD3.Add(IDC_IEHP_D3_UNIT);

	m_aCtrlD4.RemoveAll();
	m_aCtrlD4.Add(IDC_IEHP_D4_STATIC);
	m_aCtrlD4.Add(IDC_IEHP_D4_PLUS_EDIT);
	m_aCtrlD4.Add(IDC_IEHP_D4_MNUS_EDIT);
	m_aCtrlD4.Add(IDC_IEHP_D4_UNIT);

	m_aCtrlA0.RemoveAll();
	m_aCtrlA0.Add(IDC_STIFF_PLUS_STC);
	m_aCtrlA0.Add(IDC_STIFF_MNUS_STC);

	m_aCtrlA1.RemoveAll();
	m_aCtrlA1.Add(IDC_IEHP_STIFF_STATIC);
	m_aCtrlA1.Add(IDC_IEHP_STIF_1ST_PLUS_EDIT);
	m_aCtrlA1.Add(IDC_IEHP_STIF_1ST_MNUS_EDIT);
	
	m_aCtrlA2.RemoveAll();
	m_aCtrlA2.Add(IDC_CMD_ALPHA2_STATIC);
	m_aCtrlA2.Add(IDC_IEHP_STIF_2ND_PLUS_EDIT);
	m_aCtrlA2.Add(IDC_IEHP_STIF_2ND_MNUS_EDIT);
	
	m_aCtrlA3.RemoveAll();
	m_aCtrlA3.Add(IDC_CMD_ALPHA3_STATIC);
	m_aCtrlA3.Add(IDC_IEHP_STIF_3RD_PLUS_EDIT);
	m_aCtrlA3.Add(IDC_IEHP_STIF_3RD_MNUS_EDIT);
		
	m_aCtrlYield.RemoveAll();
	m_aCtrlYield.Add(IDC_IEHP_STATIC_FY);
	m_aCtrlYield.Add(IDC_IEHP_YIELD_PLUS_EDIT);
	m_aCtrlYield.Add(IDC_IEHP_YIELD_MNUS_EDIT);
	m_aCtrlYield.Add(IDC_IEHP_YIELD_UNIT);
	m_aCtrlYield.Add(IDC_CMD_ALPHA2_STATIC);
	m_aCtrlYield.Add(IDC_IEHP_STIF_2ND_PLUS_EDIT);
	m_aCtrlYield.Add(IDC_IEHP_STIF_2ND_MNUS_EDIT);  

	m_aCtrlUltmt.RemoveAll();
	m_aCtrlUltmt.Add(IDC_IEHP_STATIC_FU);
	m_aCtrlUltmt.Add(IDC_IEHP_ULTI_PLUS_EDIT);
	m_aCtrlUltmt.Add(IDC_IEHP_ULTI_MNUS_EDIT);
	m_aCtrlUltmt.Add(IDC_IEHP_ULTI_UNIT);
	m_aCtrlUltmt.Add(IDC_CMD_ALPHA3_STATIC);
	m_aCtrlUltmt.Add(IDC_IEHP_STIF_3RD_PLUS_EDIT);
	m_aCtrlUltmt.Add(IDC_IEHP_STIF_3RD_MNUS_EDIT);  

	m_aCtrlFail.RemoveAll();
	m_aCtrlFail.Add(IDC_IEHP_STATIC_FF);
	m_aCtrlFail.Add(IDC_IEHP_FAIL_PLUS_EDIT);
	m_aCtrlFail.Add(IDC_IEHP_FAIL_MNUS_EDIT);
	m_aCtrlFail.Add(IDC_IEHP_FAIL_UNIT);
	m_aCtrlFail.Add(IDC_IEHP_D4_STATIC);
	m_aCtrlFail.Add(IDC_IEHP_D4_PLUS_EDIT);
	m_aCtrlFail.Add(IDC_IEHP_D4_MNUS_EDIT);
	m_aCtrlFail.Add(IDC_IEHP_D4_UNIT);  
		
	m_aOK.RemoveAll();
	m_aOK.Add(IDOK);
	m_aOK.Add(IDCANCEL);

	m_aClough.RemoveAll();
	m_aClough.Add(IDC_IEHP_STATIC_CRACK);
	//m_aClough.Add(IDC_IEHP_CRACK_STATIC1);
	//m_aClough.Add(IDC_IEHP_CRACK_STATIC2);
	//m_aClough.Add(IDC_IEHP_CRACK_STATIC3);
	m_aClough.Add(IDC_IEHP_CRACK_PLUS_EDIT);
	m_aClough.Add(IDC_IEHP_CRACK_MNUS_EDIT);
	m_aClough.Add(IDC_IEHP_CRACK_UNIT);
	m_aClough.Add(IDC_IEHP_STIFF_STATIC);
	m_aClough.Add(IDC_IEHP_STIF_1ST_PLUS_EDIT);
	m_aClough.Add(IDC_IEHP_STIF_1ST_MNUS_EDIT);
	
	// 사용되지 않음
	m_aCtrlMinus.RemoveAll();
	m_aCtrlMinus.Add(IDC_IEHP_YIELD_MNUS_EDIT);
	m_aCtrlMinus.Add(IDC_IEHP_CRACK_MNUS_EDIT);
	m_aCtrlMinus.Add(IDC_IEHP_ULTI_MNUS_EDIT);
	m_aCtrlMinus.Add(IDC_IEHP_FAIL_MNUS_EDIT);
	m_aCtrlMinus.Add(IDC_IEHP_STIF_1ST_MNUS_EDIT);
	m_aCtrlMinus.Add(IDC_IEHP_STIF_2ND_MNUS_EDIT);
	m_aCtrlMinus.Add(IDC_IEHP_STIF_3RD_MNUS_EDIT);
	m_aCtrlMinus.Add(IDC_IEHP_D1_MNUS_EDIT);
	m_aCtrlMinus.Add(IDC_IEHP_D2_MNUS_EDIT);
	m_aCtrlMinus.Add(IDC_IEHP_D3_MNUS_EDIT);
	m_aCtrlMinus.Add(IDC_IEHP_D4_MNUS_EDIT);  
	m_aCtrlMinus.Add(IDC_IEHP_LEVEL_1MNUS);
	m_aCtrlMinus.Add(IDC_IEHP_LEVEL_2MNUS);
	m_aCtrlMinus.Add(IDC_IEHP_LEVEL_3MNUS);
	m_aCtrlMinus.Add(IDC_IEHP_LEVEL_4MNUS);
	m_aCtrlMinus.Add(IDC_IEHP_LEVEL_5MNUS);

	m_aAssignHinge.Add(IDC_TYPE_SYM_RDO2);
	m_aAssignHinge.Add(IDC_TYPE_ASYM_RDO2);
	m_aAssignHinge.Add(IDC_IEHP_ALPHA_DELTA_RDO);
	m_aAssignHinge.Add(IDC_IEHP_ALPHA_DELTA_RDO2);
	m_aAssignHinge.Add(IDC_TYPE_SYM_RDO);
	m_aAssignHinge.Add(IDC_TYPE_ASYM_RDO);
	m_aAssignHinge.Add(IDC_IEHP_STATIC_CRACK);
	m_aAssignHinge.Add(IDC_IEHP_STATIC_FY);
	m_aAssignHinge.Add(IDC_IEHP_STATIC_FU);
	m_aAssignHinge.Add(IDC_IEHP_STATIC_FF);
	m_aAssignHinge.Add(IDC_YIELD_PLUS_STC);
	m_aAssignHinge.Add(IDC_YIELD_MNUS_STC);
	m_aAssignHinge.Add(IDC_IEHP_CRACK_PLUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_YIELD_PLUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_ULTI_PLUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_FAIL_PLUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_CRACK_MNUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_YIELD_MNUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_ULTI_MNUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_FAIL_MNUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_STIF_1ST_PLUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_STIF_2ND_PLUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_STIF_3RD_PLUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_STIF_1ST_MNUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_STIF_2ND_MNUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_STIF_3RD_MNUS_EDIT);
	//m_aAssignHinge.Add(IDC_IEHP_STIF_EXPNT_EDIT);
	//m_aAssignHinge.Add(IDC_IEHP_STIF_LOOP_EDIT);
	//m_aAssignHinge.Add(IDC_IEHP_UNLD_STATIC);
	//m_aAssignHinge.Add(IDC_IEHP_EXPNT_STATIC);
	//m_aAssignHinge.Add(IDC_IEHP_LOOP_STATIC);
	m_aAssignHinge.Add(IDC_CMD_ALPHA3_STATIC);
	m_aAssignHinge.Add(IDC_CMD_ALPHA2_STATIC);
	m_aAssignHinge.Add(IDC_IEHP_STIFF_STATIC);
	m_aAssignHinge.Add(IDC_STIFF_PLUS_STC);
	m_aAssignHinge.Add(IDC_STIFF_MNUS_STC);
	m_aAssignHinge.Add(IDC_DEFORMATION_INDEX);
	m_aAssignHinge.Add(IDC_IEHP_INIT_STIFF_FRM);
	m_aAssignHinge.Add(IDC_STIFF_6EI_RDO);
	m_aAssignHinge.Add(IDC_STIFF_3EI_RDO);
	m_aAssignHinge.Add(IDC_STIFF_2EI_RDO);
	m_aAssignHinge.Add(IDC_STIFF_USER_RDO);
	m_aAssignHinge.Add(IDC_STIFF_ESTIF_RDO);
	m_aAssignHinge.Add(IDC_STIFF_SCURV_RDO);
	m_aAssignHinge.Add(IDC_STIFF_USER_UNIT);
	m_aAssignHinge.Add(IDC_STIFF_USER_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_D1_PLUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_D2_PLUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_D3_PLUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_D4_PLUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_D1_MNUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_D2_MNUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_D3_MNUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_D4_MNUS_EDIT);
	m_aAssignHinge.Add(IDC_IEHP_SLIP_T_EDT);
	m_aAssignHinge.Add(IDC_IEHP_SLIP_C_EDT);
	m_aAssignHinge.Add(IDC_YIELD_AUTO_RDO);
	m_aAssignHinge.Add(IDC_YIELD_USER_RDO);
	
	m_Data.Initialize();
	for(int i=0; i<7; i++)
	{
		m_Data.AllProp.PROP[i].KINEMA.Initialize();
		m_Data.AllProp.PROP[i].ORIGIN.Initialize();
		m_Data.AllProp.PROP[i].PEAK.Initialize();
		m_Data.AllProp.PROP[i].CLOUGH.Initialize();
		m_Data.AllProp.PROP[i].DEGRAD.Initialize();
		m_Data.AllProp.PROP[i].TAKEDA.Initialize();
		m_Data.AllProp.PROP[i].YLDSUR.Initialize();
		m_Data.AllProp.PROP[i].NORBIL.Initialize();
		m_Data.AllProp.PROP[i].ELABIL.Initialize();
		m_Data.AllProp.PROP[i].ELATRI.Initialize();
		m_Data.AllProp.PROP[i].ELATET.Initialize();

		m_Data.AllSubProp.PROP[i].KINEMA.Initialize();
		m_Data.AllSubProp.PROP[i].ORIGIN.Initialize();
		m_Data.AllSubProp.PROP[i].PEAK.Initialize();
		m_Data.AllSubProp.PROP[i].CLOUGH.Initialize();
		m_Data.AllSubProp.PROP[i].DEGRAD.Initialize();
		m_Data.AllSubProp.PROP[i].TAKEDA.Initialize();
		m_Data.AllSubProp.PROP[i].YLDSUR.Initialize();
		m_Data.AllSubProp.PROP[i].NORBIL.Initialize();
		m_Data.AllSubProp.PROP[i].ELABIL.Initialize();
		m_Data.AllSubProp.PROP[i].ELATRI.Initialize();
		m_Data.AllSubProp.PROP[i].ELATET.Initialize();
	}

	m_dZero = 10E-6;

	m_nSectType = -1;
	m_nTabStyle = nTabStyle;

	m_bDisableAutoCalc = FALSE;    // Auto 계산을 할 수 없는 경우
}


void CIehpPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpPropDlg)
	DDX_Control(pDX, IDC_HINGE_TAB1, m_Tab);
	DDX_Control(pDX, IDC_IEHP_D1_PLUS_EDIT, m_edtDisp1Plus);
	DDX_Control(pDX, IDC_IEHP_D1_MNUS_EDIT, m_edtDisp1Mnus);
	DDX_Control(pDX, IDC_IEHP_D2_PLUS_EDIT, m_edtDisp2Plus);
	DDX_Control(pDX, IDC_IEHP_D2_MNUS_EDIT, m_edtDisp2Mnus);
	DDX_Control(pDX, IDC_IEHP_D3_PLUS_EDIT, m_edtDisp3Plus);
	DDX_Control(pDX, IDC_IEHP_D3_MNUS_EDIT, m_edtDisp3Mnus);
	DDX_Control(pDX, IDC_IEHP_D4_PLUS_EDIT, m_edtDisp4Plus);
	DDX_Control(pDX, IDC_IEHP_D4_MNUS_EDIT, m_edtDisp4Mnus);
	DDX_Control(pDX, IDC_IEHP_ULTI_PLUS_EDIT, m_edtUltPlus);
	DDX_Control(pDX, IDC_IEHP_ULTI_MNUS_EDIT, m_edtUltMnus);
	DDX_Control(pDX, IDC_IEHP_FAIL_MNUS_EDIT, m_edtFraMnus);
	DDX_Control(pDX, IDC_IEHP_FAIL_PLUS_EDIT, m_edtFraPlus);
	DDX_Control(pDX, IDC_IEHP_CRACK_PLUS_EDIT, m_edtCrkPlus);
	DDX_Control(pDX, IDC_IEHP_CRACK_MNUS_EDIT, m_edtCrkMnus);
	DDX_Control(pDX, IDC_STIFF_USER_EDIT, m_edtStiffUser);
	DDX_Control(pDX, IDC_IEHP_YIELD_PLUS_EDIT, m_edtYldPlus);
	DDX_Control(pDX, IDC_IEHP_YIELD_MNUS_EDIT, m_edtYldMnus);
	DDX_Control(pDX, IDC_IEHP_STIF_EXPNT_EDIT, m_edtExponent);
	DDX_Control(pDX, IDC_IEHP_STIF_LOOP_EDIT,  m_edtLoop);
	DDX_Control(pDX, IDC_IEHP_STIF_PINCH_EDIT, m_edtPinch);
	DDX_Control(pDX, IDC_IEHP_SLIP_T_EDT, m_edtSlipT);
	DDX_Control(pDX, IDC_IEHP_SLIP_C_EDT, m_edtSlipC);

	DDX_Control(pDX, IDC_IEHP_ULTI_UNIT, m_untUltmt);
	DDX_Control(pDX, IDC_STIFF_USER_UNIT, m_untStiffUser);
	DDX_Control(pDX, IDC_IEHP_CRACK_UNIT, m_untCrack);
	DDX_Control(pDX, IDC_IEHP_YIELD_UNIT, m_untYield);
	DDX_Control(pDX, IDC_IEHP_FAIL_UNIT, m_untFract);
	DDX_Control(pDX, IDC_IEHP_D1_UNIT, m_untDisp1);
	DDX_Control(pDX, IDC_IEHP_D2_UNIT, m_untDisp2);
	DDX_Control(pDX, IDC_IEHP_D3_UNIT, m_untDisp3);
	DDX_Control(pDX, IDC_IEHP_D4_UNIT, m_untDisp4);
	DDX_Control(pDX, IDC_IEHP_STIF_EXPNT_UNIT, m_untExponent);
	DDX_Control(pDX, IDC_IEHP_STIF_LOOP_UNIT,  m_untLoop);
	DDX_Control(pDX, IDC_IEHP_SLIP_UNT,   m_untSlip);

	DDX_Text(pDX, IDC_IEHP_STIF_1ST_PLUS_EDIT, m_str1Plus);
	DDX_Text(pDX, IDC_IEHP_STIF_1ST_MNUS_EDIT, m_str1Mnus);
	DDX_Text(pDX, IDC_IEHP_STIF_2ND_MNUS_EDIT, m_str2Mnus);
	DDX_Text(pDX, IDC_IEHP_STIF_2ND_PLUS_EDIT, m_str2Plus);
	DDX_Text(pDX, IDC_IEHP_STIF_3RD_PLUS_EDIT, m_str3Plus);
	DDX_Text(pDX, IDC_IEHP_STIF_3RD_MNUS_EDIT, m_str3Mnus);
	
	DDX_Radio(pDX, IDC_TYPE_SYM_RDO2, m_nValueTypeSymmetry);
	DDX_Radio(pDX, IDC_YIELD_USER_RDO, m_nUserAuto);
	DDX_Radio(pDX, IDC_TYPE_SYM_RDO, m_nSymmetry);
	DDX_Radio(pDX, IDC_STIFF_6EI_RDO, m_nStiffType);
	DDX_Radio(pDX, IDC_IEHP_ALPHA_DELTA_RDO, m_nAlphaDelta);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIehpPropDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIehpPropDlg)
	ON_EN_CHANGE(IDC_IEHP_CRACK_PLUS_EDIT,    OnChangeCrackEdit)  
	ON_EN_CHANGE(IDC_IEHP_YIELD_PLUS_EDIT,    OnChangeYieldEdit)		
	ON_EN_CHANGE(IDC_IEHP_ULTI_PLUS_EDIT,     OnChangeUltiEdit)
	ON_EN_CHANGE(IDC_IEHP_FAIL_PLUS_EDIT,     OnChangeFailEdit)
	ON_EN_CHANGE(IDC_IEHP_CRACK_MNUS_EDIT,    OnChangeDrawData)
	ON_EN_CHANGE(IDC_IEHP_YIELD_MNUS_EDIT,    OnChangeDrawData)
	ON_EN_CHANGE(IDC_IEHP_ULTI_MNUS_EDIT,     OnChangeDrawData)  
	ON_EN_CHANGE(IDC_IEHP_FAIL_MNUS_EDIT,     OnChangeDrawData)
	ON_EN_CHANGE(IDC_IEHP_STIF_1ST_PLUS_EDIT, OnChangeStif1stEdit)
	ON_EN_CHANGE(IDC_IEHP_STIF_2ND_PLUS_EDIT, OnChangeStif2ndEdit)
	ON_EN_CHANGE(IDC_IEHP_STIF_3RD_PLUS_EDIT, OnChangeStif3rdEdit)
	ON_EN_CHANGE(IDC_IEHP_STIF_1ST_MNUS_EDIT, OnChangeDrawData)
	ON_EN_CHANGE(IDC_IEHP_STIF_2ND_MNUS_EDIT, OnChangeDrawData)
	ON_EN_CHANGE(IDC_IEHP_STIF_3RD_MNUS_EDIT, OnChangeDrawData)
	ON_EN_CHANGE(IDC_IEHP_D1_PLUS_EDIT,       OnChangeDisp1Edit)
	ON_EN_CHANGE(IDC_IEHP_D2_PLUS_EDIT,       OnChangeDisp2Edit)
	ON_EN_CHANGE(IDC_IEHP_D3_PLUS_EDIT,       OnChangeDisp3Edit)
	ON_EN_CHANGE(IDC_IEHP_D4_PLUS_EDIT,       OnChangeDisp4Edit)
	ON_EN_CHANGE(IDC_IEHP_D1_MNUS_EDIT,       OnChangeDrawData)
	ON_EN_CHANGE(IDC_IEHP_D2_MNUS_EDIT,       OnChangeDrawData)
	ON_EN_CHANGE(IDC_IEHP_D3_MNUS_EDIT,       OnChangeDrawData)
	ON_EN_CHANGE(IDC_IEHP_D4_MNUS_EDIT,       OnChangeDrawData)

	ON_EN_CHANGE(IDC_IEHP_SLIP_T_EDT,         OnChangeSlipInitGap)
	ON_EN_CHANGE(IDC_IEHP_SLIP_C_EDT,         OnChangeSlipInitGap)
	ON_BN_CLICKED(IDC_TYPE_SYM_RDO,           OnSymmetryTypeRdo)
	ON_BN_CLICKED(IDC_YIELD_AUTO_RDO,         OnUserAutoRdo)
	ON_BN_CLICKED(IDC_STIFF_6EI_RDO,          OnStiffTypeRdo)
	ON_BN_CLICKED(IDC_TYPE_ASYM_RDO,          OnSymmetryTypeRdo)
	ON_BN_CLICKED(IDC_YIELD_USER_RDO,         OnUserAutoRdo)
	ON_BN_CLICKED(IDC_STIFF_3EI_RDO,          OnStiffTypeRdo)
	ON_BN_CLICKED(IDC_STIFF_2EI_RDO,          OnStiffTypeRdo)
	ON_BN_CLICKED(IDC_STIFF_USER_RDO,         OnStiffTypeRdo)
	ON_BN_CLICKED(IDC_STIFF_ESTIF_RDO,        OnStiffTypeRdo)
	ON_BN_CLICKED(IDC_STIFF_SCURV_RDO,        OnStiffTypeRdo)  
	ON_BN_CLICKED(IDC_IEHP_ALPHA_DELTA_RDO,   OnAlphaDeltaRdo)
	ON_BN_CLICKED(IDC_IEHP_ALPHA_DELTA_RDO2,  OnAlphaDeltaRdo)


	ON_BN_CLICKED(IDC_DEFORMATION_INDEX,			OnDeformationIndex)
	ON_BN_CLICKED(IDC_TYPE_SYM_RDO2,					OnValueTypeSymmetry)
	ON_BN_CLICKED(IDC_TYPE_ASYM_RDO2,					OnValueTypeSymmetry)
	
	ON_NOTIFY(TCN_SELCHANGING, IDC_HINGE_TAB1, OnSelchangingTab)
	ON_NOTIFY(TCN_SELCHANGE  , IDC_HINGE_TAB1, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpPropDlg Data & Window handlers

void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD,T_IEHG_K IehgKey, T_IEHP_KINEMA data,T_IEHP_KINEMA subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].KINEMA = data;
	m_Data.AllSubProp.PROP[nD].KINEMA = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_ORIGIN data,T_IEHP_ORIGIN subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].ORIGIN = data;
	m_Data.AllSubProp.PROP[nD].ORIGIN = subdata;


	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD,T_IEHG_K IehgKey, T_IEHP_PEAK data,T_IEHP_PEAK   subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].PEAK = data;
	m_Data.AllSubProp.PROP[nD].PEAK = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_CLOUGH data,T_IEHP_CLOUGH subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].CLOUGH = data;
	m_Data.AllSubProp.PROP[nD].CLOUGH = subdata;


	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_DEGRAD data,T_IEHP_DEGRAD subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].DEGRAD = data;
	m_Data.AllSubProp.PROP[nD].DEGRAD = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_TAKEDA data,T_IEHP_TAKEDA subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].TAKEDA = data;
	m_Data.AllSubProp.PROP[nD].TAKEDA = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_NORBIL data,T_IEHP_NORBIL subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].NORBIL = data;
	m_Data.AllSubProp.PROP[nD].NORBIL = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_ELABIL data,T_IEHP_ELABIL subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].ELABIL = data;
	m_Data.AllSubProp.PROP[nD].ELABIL = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_ELATRI data,T_IEHP_ELATRI subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].ELATRI = data;
	m_Data.AllSubProp.PROP[nD].ELATRI = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_ELATET data,T_IEHP_ELATET subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].ELATET = data;
	m_Data.AllSubProp.PROP[nD].ELATET = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_SRCTET data,T_IEHP_SRCTET subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].SRCTET = data;
	m_Data.AllSubProp.PROP[nD].SRCTET = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_TAKTET data,T_IEHP_TAKTET subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].TAKTET = data;
	m_Data.AllSubProp.PROP[nD].TAKTET = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}
void CIehpPropDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_SLIP data,T_IEHP_SLIP subdata, int nFg1, int nFg2, int nSectType, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].SLIP = data;
	m_Data.AllSubProp.PROP[nD].SLIP = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}

/////////////////////////////////////////////////////////////////////////////////////////
void CIehpPropDlg::Data2Dlg()
{
	if (m_Tab.GetCurSel() == D_TAB_DEFALUT)
	{
		if (m_nTabStyle == J_LOCATION)
		{
			Data2TabDlg(D_TAB_SUB, m_Data.AllSubProp.PROP);
		}
		else
		{
			Data2TabDlg(D_TAB_DEFALUT, m_Data.AllProp.PROP);
		}
	}
	else if(m_Tab.GetCurSel()==D_TAB_SUB)
		Data2TabDlg(D_TAB_SUB    , m_Data.AllSubProp.PROP);
}

BOOL CIehpPropDlg::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg)
{
	if(bMsg) GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_NAME_VAR_ERROR), lpszData, lpszName, lpszVar);
	return FALSE;
}

#define CheckError(x, bMsg) CheckErrorNameDataVar(_LS(IDS_DB_DT_IEHP), m_Data.strName, x, bMsg)
BOOL CIehpPropDlg::CheckData(BOOL bMsg/*=TRUE*/)
{
	UpdateData(TRUE);
	double dV[8];   // Force or Moment
	double dD[8];   // Displacement or Rotation

	double dTol = 1.0e-9;
	BOOL bBilinearModel = FALSE;
	BOOL bTrilinearModel = FALSE;
	BOOL bTetralinearModel = FALSE;
	if(m_nModelType== 3 || m_nModelType==10 || m_nModelType==11 || // 3:Clough, 10:Normal Bilinear, 11:Elastic Bilinear 
		 m_nModelType==30 || m_nModelType==31 || m_nModelType==32)   // SLIP Bilinear
		bBilinearModel = TRUE;
	if(m_nModelType == 13 || m_nModelType == 14 || m_nModelType == 15 || m_nModelType == 92) // 13:Elastic Tetralinear, 14:Takeda Tetralinear, 15:Modified Takeda Tetralinear, 92:SRC Tetralinear
		bTetralinearModel = TRUE;
	if(!bBilinearModel && !bTetralinearModel) bTrilinearModel = TRUE;
		
	if(bBilinearModel)
	{
		if(m_edtCrkPlus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_YIELD_FORCE_POS1), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_YIELD_MOMENT_POS1), bMsg);
		}
		if(m_edtCrkMnus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_YIELD_FORCE_NEG1), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_YIELD_MOMENT_NEG1), bMsg);
		}  
		dV[0] = m_edtCrkPlus.GetEditValue(); dV[1] = m_edtCrkMnus.GetEditValue();
		dV[2] = m_edtCrkPlus.GetEditValue(); dV[3] = m_edtCrkMnus.GetEditValue();  
		dV[4] = m_edtCrkPlus.GetEditValue(); dV[5] = m_edtCrkMnus.GetEditValue();  
		dV[6] = m_edtCrkPlus.GetEditValue(); dV[7] = m_edtCrkMnus.GetEditValue();  

		if(m_nAlphaDelta == 1)
		{
			if(m_edtYldPlus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_YIELD_FORCE_POS), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_YIELD_MOMENT_POS), bMsg);
			}    
			if(m_edtYldMnus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_YIELD_FORCE_NEG), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_YIELD_MOMENT_NEG), bMsg);
			}
			dV[2] = m_edtYldPlus.GetEditValue(); dV[3] = m_edtYldMnus.GetEditValue();  
			dV[4] = m_edtYldPlus.GetEditValue(); dV[5] = m_edtYldMnus.GetEditValue();  
			dV[6] = m_edtYldPlus.GetEditValue(); dV[7] = m_edtYldMnus.GetEditValue();  
		}    
	}
	else if(bTrilinearModel)
	{
		// Crack Force or Moment
		if(m_edtCrkPlus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_CRACK_FORCE_POS), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_CRACK_MOMENT_POS), bMsg);
		}
		if(m_edtCrkMnus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_CRACK_FORCE_NEG), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_CRACK_MOMENT_NEG), bMsg);
		}        
		// Yield Force or Moment
		if(m_edtYldPlus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_YIELD_FORCE_POS), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_YIELD_MOMENT_POS), bMsg);
		}
		if(m_edtYldMnus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_YIELD_FORCE_NEG), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_YIELD_MOMENT_NEG), bMsg);
		}
		dV[0] = m_edtCrkPlus.GetEditValue(); dV[1] = m_edtCrkMnus.GetEditValue();
		dV[2] = m_edtYldPlus.GetEditValue(); dV[3] = m_edtYldMnus.GetEditValue();
		dV[4] = m_edtYldPlus.GetEditValue(); dV[5] = m_edtYldMnus.GetEditValue();    
		dV[6] = m_edtYldPlus.GetEditValue(); dV[7] = m_edtYldMnus.GetEditValue();    
		if(m_nAlphaDelta == 1)
		{
			// Ultimate Force or Moment
			if(m_edtUltPlus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_ULTIMATE_FORCE_POS), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_ULTIMATE_MOMENT_POS), bMsg);
			}
			if(m_edtUltMnus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_ULTIMATE_FORCE_NEG), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_ULTIMATE_MOMENT_NEG), bMsg);
			}
			dV[4] = m_edtUltPlus.GetEditValue(); dV[5] = m_edtUltMnus.GetEditValue();    
			dV[6] = m_edtUltPlus.GetEditValue(); dV[7] = m_edtUltMnus.GetEditValue();    
		}
	}
	else if(bTetralinearModel)
	{
		// Crack Force or Moment
		if(m_edtCrkPlus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_CRACK_FORCE_POS), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_CRACK_MOMENT_POS), bMsg);
		}
		if(m_edtCrkMnus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_CRACK_FORCE_NEG), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_CRACK_MOMENT_NEG), bMsg);
		}        
		// Yield Force or Moment
		if(m_edtYldPlus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_YIELD_FORCE_POS), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_YIELD_MOMENT_POS), bMsg);
		}
		if(m_edtYldMnus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_YIELD_FORCE_NEG), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_YIELD_MOMENT_NEG), bMsg);
		}
		// Ultimate Force or Moment
		if(m_edtUltPlus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_ULTIMATE_FORCE_POS), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_ULTIMATE_MOMENT_POS), bMsg);
		}
		if(m_edtUltMnus.GetEditValue()<=0.)
		{
			if(m_nDof<=2) return CheckError(_LS(IDS_DB_ULTIMATE_FORCE_NEG), bMsg);
			if(m_nDof>2)  return CheckError(_LS(IDS_DB_ULTIMATE_MOMENT_NEG), bMsg);
		}
		dV[0] = m_edtCrkPlus.GetEditValue(); dV[1] = m_edtCrkMnus.GetEditValue();
		dV[2] = m_edtYldPlus.GetEditValue(); dV[3] = m_edtYldMnus.GetEditValue();    
		dV[4] = m_edtUltPlus.GetEditValue(); dV[5] = m_edtUltMnus.GetEditValue();
		dV[6] = m_edtUltPlus.GetEditValue(); dV[7] = m_edtUltMnus.GetEditValue();
		if(m_nAlphaDelta == 1)
		{
			// Fracture Force or Moment
			if(m_edtFraPlus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_FRACTURE_FORCE_POS), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_FRACTURE_MOMENT_POS), bMsg);
			}
			if(m_edtFraMnus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_FRACTURE_FORCE_NEG), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_FRACTURE_MOMENT_NEG), bMsg);
			}      
			dV[6] = m_edtFraPlus.GetEditValue(); dV[7] = m_edtFraMnus.GetEditValue();    
		}
	}

	if(m_nAlphaDelta == 0)
	{
		if(bTrilinearModel || bTetralinearModel)
		{
			if(m_nDof<=2)
			{
				if     (dV[0] > dV[2]) return CheckError(_LS(IDS_DB_CRACK_FORCE_COMP_PLUS), bMsg);
				else if(dV[1] > dV[3]) return CheckError(_LS(IDS_DB_CRACK_FORCE_COMP_MNUS), bMsg);
			}
			else
			{
				if     (dV[0] > dV[2]) return CheckError(_LS(IDS_DB_CRACK_FORCE_COMP_PLUS), bMsg);
				else if(dV[1] > dV[3]) return CheckError(_LS(IDS_DB_CRACK_FORCE_COMP_MNUS), bMsg);
			}
		}
		if(bTetralinearModel)
		{
			if(m_nDof<=2)
			{
				if     (dV[2] > dV[4]) return CheckError(_LS(IDS_DB_YIELD_FORCE_COMP_PLUS), bMsg);
				else if(dV[3] > dV[5]) return CheckError(_LS(IDS_DB_YIELD_FORCE_COMP_MNUS), bMsg);
			}
			else
			{
				if     (dV[2] > dV[4]) return CheckError(_LS(IDS_DB_YIELD_FORCE_COMP_PLUS), bMsg);
				else if(dV[3] > dV[5]) return CheckError(_LS(IDS_DB_YIELD_FORCE_COMP_MNUS), bMsg);
			}
		}
	}
	else if(m_nAlphaDelta == 1)  
	{
		if(bTrilinearModel || bTetralinearModel || bTetralinearModel)
		{
			if(m_nDof<=2)
			{
				if     (dV[0] > dV[2]) return CheckError(_LS(IDS_DB_CRACK_FORCE_COMP_PLUS), bMsg);
				else if(dV[1] > dV[3]) return CheckError(_LS(IDS_DB_CRACK_FORCE_COMP_MNUS), bMsg);
			}
			else
			{
				if     (dV[0] > dV[2]) return CheckError(_LS(IDS_DB_CRACK_FORCE_COMP_PLUS), bMsg);
				else if(dV[1] > dV[3]) return CheckError(_LS(IDS_DB_CRACK_FORCE_COMP_MNUS), bMsg);
			}
		}
		if(bTrilinearModel || bTetralinearModel)
		{
			if(m_nDof<=2)
			{
				if     (dV[2] > dV[4]) return CheckError(_LS(IDS_DB_YIELD_FORCE_COMP_PLUS), bMsg);
				else if(dV[3] > dV[5]) return CheckError(_LS(IDS_DB_YIELD_FORCE_COMP_MNUS), bMsg);
			}
			else
			{
				if     (dV[2] > dV[4]) return CheckError(_LS(IDS_DB_YIELD_FORCE_COMP_PLUS), bMsg);
				else if(dV[3] > dV[5]) return CheckError(_LS(IDS_DB_YIELD_FORCE_COMP_MNUS), bMsg);
			}
		}
		if(bTetralinearModel)
		{
			if(m_nDof<=2)
			{
				if     (dV[4] < dV[6]) return CheckError(_LS(IDS_DB_ULTI_FORCE_COMP_PLUS), bMsg);
				else if(dV[5] < dV[7]) return CheckError(_LS(IDS_DB_ULTI_FORCE_COMP_MNUS), bMsg);
			}
			else
			{
				if     (dV[4] < dV[6]) return CheckError(_LS(IDS_DB_ULTI_FORCE_COMP_PLUS), bMsg);
				else if(dV[5] < dV[7]) return CheckError(_LS(IDS_DB_ULTI_FORCE_COMP_MNUS), bMsg);
			}
		}
	}

	// Stiffness reduction ratio
	if(m_nAlphaDelta == 0)   // Alpha
	{  
		dV[0] = _tstof(m_str1Plus); dV[1] = _tstof(m_str1Mnus);
		dV[2] = _tstof(m_str2Plus); dV[3] = _tstof(m_str2Mnus);
		dV[4] = _tstof(m_str3Plus); dV[5] = _tstof(m_str3Mnus);    
		if(m_nModelType == 0 || m_nModelType == 1 || m_nModelType == 2 || m_nModelType == 4 || m_nModelType == 5 || 
			 m_nModelType == 6 || m_nModelType == 7 || m_nModelType == 8 || m_nModelType == 9 || m_nModelType == D_IEHP_TAKS)
		{
			if(dV[0]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_POS), bMsg);
			if(dV[1]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_NEG), bMsg);
			if(dV[2]<0.) return CheckError(_LS(IDS_DB_2ND_STIF_REDU_RATIO_POS), bMsg);
			if(dV[3]<0.) return CheckError(_LS(IDS_DB_2ND_STIF_REDU_RATIO_NEG), bMsg);      
		}
		else if(m_nModelType == 3 || m_nModelType == 10)  // Clough(3), Normal Bilinear(10)
		{
			if(dV[0]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_POS), bMsg);
			if(dV[1]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_NEG), bMsg);
		}  
		else if(m_nModelType == 11)  // Elastic Bilinear
		{
			if(dV[0]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_POS), bMsg);
			if(dV[1]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_NEG), bMsg);
		}  
		else if(m_nModelType == 12)  // Elastic Trilinear
		{
			if(dV[0]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_POS), bMsg);
			if(dV[1]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_NEG), bMsg);
			if(dV[2]<0.) return CheckError(_LS(IDS_DB_2ND_STIF_REDU_RATIO_POS), bMsg);
			if(dV[3]<0.) return CheckError(_LS(IDS_DB_2ND_STIF_REDU_RATIO_NEG), bMsg);      
		}
		else if(m_nModelType == 13 || m_nModelType == 14 || m_nModelType == 15 || m_nModelType == 92)  // 13:Elastic Tetralinear, 14:Takeda Tetralinear, 15:Modified Takeda Tetralinear, 92:SRC Takeda Tetralinear
		{
			if(dV[0]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_POS), bMsg);
			if(dV[1]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_NEG), bMsg);
			if(dV[2]<0.) return CheckError(_LS(IDS_DB_2ND_STIF_REDU_RATIO_POS), bMsg);
			if(dV[3]<0.) return CheckError(_LS(IDS_DB_2ND_STIF_REDU_RATIO_NEG), bMsg);
			if(dV[4]<0.) return CheckError(_LS(IDS_DB_3RD_STIF_REDU_RATIO_POS), bMsg);
			if(dV[5]<0.) return CheckError(_LS(IDS_DB_3RD_STIF_REDU_RATIO_NEG), bMsg);      
		}
		else if(m_nModelType==30 || m_nModelType==31 || m_nModelType==32) // SLIP Bilinear
		{
			if(dV[0]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_POS), bMsg);
			if(dV[1]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_NEG), bMsg);
		}
		else if(m_nModelType==33 || m_nModelType==34 || m_nModelType==35) // SLIP Trilinear
		{
			if(dV[0]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_POS), bMsg);
			if(dV[1]<0.) return CheckError(_LS(IDS_DB_1ST_STIF_REDU_RATIO_NEG), bMsg);
			if(dV[2]<0.) return CheckError(_LS(IDS_DB_2ND_STIF_REDU_RATIO_POS), bMsg);
			if(dV[3]<0.) return CheckError(_LS(IDS_DB_2ND_STIF_REDU_RATIO_NEG), bMsg);      
		}
		else 
			ASSERT(FALSE);
	}
	else if(m_nAlphaDelta == 1)  // Displacement
	{
		if(bBilinearModel || bTrilinearModel || bTetralinearModel)
		{
			if(m_edtDisp1Plus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_1ST_YIELD_DISP), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_1ST_YIELD_ROTN), bMsg);
			}
			if(m_edtDisp1Mnus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_1ST_YIELD_DISP), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_1ST_YIELD_ROTN), bMsg);
			}  
			if(m_edtDisp2Plus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_2ND_YIELD_DISP), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_2ND_YIELD_ROTN), bMsg);
			}
			if(m_edtDisp2Mnus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_2ND_YIELD_DISP), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_2ND_YIELD_ROTN), bMsg);
			}  
			dD[0] = m_edtDisp1Plus.GetEditValue(); dD[1] = m_edtDisp1Mnus.GetEditValue();
			dD[2] = m_edtDisp2Plus.GetEditValue(); dD[3] = m_edtDisp2Mnus.GetEditValue();  
			dD[4] = m_edtDisp2Plus.GetEditValue(); dD[5] = m_edtDisp2Mnus.GetEditValue();  
			dD[6] = m_edtDisp2Plus.GetEditValue(); dD[7] = m_edtDisp2Mnus.GetEditValue();        
		}
		if(bTrilinearModel || bTetralinearModel)
		{
			if(m_edtDisp3Plus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_3RD_YIELD_DISP), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_3RD_YIELD_ROTN), bMsg);
			}
			if(m_edtDisp3Mnus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_3RD_YIELD_DISP), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_3RD_YIELD_ROTN), bMsg);
			}        
			dD[4] = m_edtDisp3Plus.GetEditValue(); dD[5] = m_edtDisp3Mnus.GetEditValue();  
			dD[6] = m_edtDisp3Plus.GetEditValue(); dD[7] = m_edtDisp3Mnus.GetEditValue();        
		}
		if(bTetralinearModel)
		{
			if(m_edtDisp4Plus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_4TH_YIELD_DISP), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_4TH_YIELD_ROTN), bMsg);
			}
			if(m_edtDisp4Mnus.GetEditValue()<=0.)
			{
				if(m_nDof<=2) return CheckError(_LS(IDS_DB_4TH_YIELD_DISP), bMsg);
				if(m_nDof>2)  return CheckError(_LS(IDS_DB_4TH_YIELD_ROTN), bMsg);
			}      
			dD[6] = m_edtDisp4Plus.GetEditValue(); dD[7] = m_edtDisp4Mnus.GetEditValue();        
		}

		if(bBilinearModel || bTrilinearModel || bTetralinearModel)
		{
			if(m_nDof<=2)
			{
				if(dD[0] > dD[2] || dD[1] > dD[3]) return CheckError(_LS(IDS_DB_YIELD_DISP_COMPARE1), bMsg);  // D2 >= D1
				if(dD[0] >= dD[2] && fabs(dV[0] - dV[2]) > dTol) return CheckError(_T("D2 > D1 (if P2 > P1)"), bMsg);
				if(dD[1] >= dD[3] && fabs(dV[1] - dV[3]) > dTol) return CheckError(_T("D2 > D1 (if P2 > P1)"), bMsg);      
			}
			else
			{
				if(dD[0] > dD[2] || dD[1] > dD[3]) return CheckError(_LS(IDS_DB_YIELD_ROTN_COMPARE1), bMsg);  // D2 >= D1
				if(dD[0] >= dD[2] && fabs(dV[0] - dV[2]) > dTol) return CheckError(_T("D2 > D1 (if P2 > P1)"), bMsg);
				if(dD[1] >= dD[3] && fabs(dV[1] - dV[3]) > dTol) return CheckError(_T("D2 > D1 (if P2 > P1)"), bMsg);      
			}      
		}
		if(bTrilinearModel || bTetralinearModel)
		{      
			if(m_nDof<=2)
			{
				if(dD[2] > dD[4] || dD[3] > dD[5]) return CheckError(_LS(IDS_DB_YIELD_DISP_COMPARE2), bMsg);  // D3 >= D2
				if(dD[2] >= dD[4] && fabs(dV[2] - dV[4]) > dTol) return CheckError(_T("D3 > D2 (if P3 > P2)"), bMsg);
				if(dD[3] >= dD[5] && fabs(dV[3] - dV[5]) > dTol) return CheckError(_T("D3 > D2 (if P3 > P2)"), bMsg);
			}
			else
			{
				if(dD[2] > dD[4] || dD[3] > dD[5]) return CheckError(_LS(IDS_DB_YIELD_ROTN_COMPARE2), bMsg);  // D3 >= D2
				if(dD[2] >= dD[4] && fabs(dV[2] - dV[4]) > dTol) return CheckError(_T("D3 > D2 (if P3 > P2)"), bMsg);
				if(dD[3] >= dD[5] && fabs(dV[3] - dV[5]) > dTol) return CheckError(_T("D3 > D2 (if P3 > P2)"), bMsg);
			}
		}
		if(bTetralinearModel)
		{
			if(m_nDof<=2)
			{
				if(dD[4] > dD[6] || dD[5] > dD[7]) return CheckError(_LS(IDS_DB_YIELD_DISP_COMPARE3), bMsg);  // D4 >= D3
				if(dD[4] >= dD[6] && fabs(dV[4] - dV[6]) > dTol) return CheckError(_T("D4 > D3 (if P4 < P3)"), bMsg);
				if(dD[5] >= dD[7] && fabs(dV[5] - dV[7]) > dTol) return CheckError(_T("D4 > D3 (if P4 < P3)"), bMsg);
			}
			else
			{
				if(dD[4] > dD[6] || dD[5] > dD[7]) return CheckError(_LS(IDS_DB_YIELD_ROTN_COMPARE3), bMsg);  // D4 >= D3
				if(dD[4] >= dD[6] && fabs(dV[4] - dV[6]) > dTol) return CheckError(_T("D4 > D3 (if P4 < P3)"), bMsg);
				if(dD[5] >= dD[7] && fabs(dV[5] - dV[7]) > dTol) return CheckError(_T("D4 > D3 (if P4 < P3)"), bMsg);
			}
		}
	}
	
	// Exponent
	double dE[1];  
	if(m_nModelType == 3 || m_nModelType == 5  || m_nModelType ==  6 || m_nModelType == 8 || 
		 m_nModelType == 9 || m_nModelType == 14 || m_nModelType == 15 || m_nModelType == D_IEHP_TAKS ||
		 m_nModelType == 92)
	{
		dE[0] = m_edtExponent.GetEditValue();
		if(dE[0]<0. || dE[0]>1.) return CheckError(_LS(IDS_DB_EXPO_UNLOAD_STIF_CALC), bMsg);
	}

	// Loop
	double dL[1];  
	if(m_nModelType == 5  || m_nModelType ==  6 || m_nModelType ==  8 || m_nModelType==9 ||
		 m_nModelType == 14 || m_nModelType == 15 || m_nModelType == D_IEHP_TAKS)
	{
		dL[0] = m_edtLoop.GetEditValue();
		if(dL[0]<=0. || dL[0]>1.) return CheckError(_LS(IDS_DB_INNER_LOOP_UNLOAD_STIF_FAC), bMsg);
	}

	if(m_nModelType == D_IEHP_TAKS)
	{
		double dPR = m_edtPinch.GetEditValue();
		if(dPR<0. || dPR>1.) return CheckError(_LS(IDS_DB_PINCHING_FACT), bMsg);
	}

	// Deformation Capacity
	for(int i=0; i<2; i++) for(int j=0; j<5; j++)
	{
		if(fabs(m_dDeformCapacity[i][j])<0.) return CheckError(_LS(IDS_DB_DEFORMATION_CAPACITY), bMsg);
		if(j>0) if(fabs(m_dDeformCapacity[i][j-1]) >= fabs(m_dDeformCapacity[i][j])) 
			return CheckError(_LS(IDS_DB_DEFORMATION_CAPACITY_COM), bMsg);
	}

	// Initial Gap
	if(m_nModelType==30 || m_nModelType==31 || m_nModelType==32 ||
		 m_nModelType==33 || m_nModelType==34 || m_nModelType==35)
	{
		double dGapP = m_edtSlipT.GetEditValue();
		double dGapM = m_edtSlipC.GetEditValue();
		if(m_nModelType!=32 && m_nModelType!=35 && dGapP<0.)
			return CheckError(_LS(IDS_DB_INIT_GAP_VALUE), bMsg);
		if(m_nModelType!=31 && m_nModelType!=34 && dGapM<0.)
			return CheckError(_LS(IDS_DB_INIT_GAP_VALUE), bMsg);
	}

	if (m_nModelType == D_IEHP_SLPB || m_nModelType == D_IEHP_SLBT || m_nModelType == D_IEHP_SLBC ||
		m_nModelType == D_IEHP_SLPT || m_nModelType == D_IEHP_SLTT || m_nModelType == D_IEHP_SLTC)
	{
		if (m_nAlphaDelta == 1)
		{
			double dGapT = m_edtSlipT.GetEditValue();
			double dGapC = m_edtSlipC.GetEditValue();

			if (m_nModelType != D_IEHP_SLBC && m_nModelType != D_IEHP_SLTC && (dGapT > dD[0]))
				return CheckError(_LS(IDS_DB_INIT_GAP_VALUE_RANGE_CHECK_T), bMsg);
			if (m_nModelType != D_IEHP_SLBT && m_nModelType != D_IEHP_SLTT && (dGapC > dD[1]))
				return CheckError(_LS(IDS_DB_INIT_GAP_VALUE_RANGE_CHECK_T), bMsg);
		}
	}

	return TRUE;
#undef CheckError
}

BOOL CIehpPropDlg::CheckAllData(BOOL bMsg)
{
	const auto nCurTab = m_Tab.GetCurSel();
	switch (nCurTab)
	{
		case D_TAB_DEFALUT:
			{
				TabDlg2Data(nCurTab, m_Data.AllProp.PROP);
			}
			break;
		case D_TAB_SUB:
			{
				TabDlg2Data(nCurTab, m_Data.AllSubProp.PROP);
			}
			break;
		default:
			{
				ASSERT(0); return FALSE;
			}
	}

	if (m_nTabStyle == I_J_LOCATION)
	{
		for ( auto i = 0; i < m_Tab.GetItemCount(); i++ )
		{
			SetTabData4CheckData(i);

			if ( !CheckData() )
			{
				m_Tab.SetCurSel(nCurTab);
				SetTabData4CheckData(nCurTab);

				return FALSE;
			}
		}

		m_Tab.SetCurSel(nCurTab);
		SetTabData4CheckData(nCurTab);
	}
	else
	{
		CheckData();
	}

	return TRUE;
}

void CIehpPropDlg::SetTabData4CheckData(const int nTabID)
{
	m_Tab.SetCurSel(nTabID);

	if (m_Tab.GetCurSel() == D_TAB_DEFALUT)
	{
		if (m_nTabStyle == J_LOCATION)
		{
			Data2TabDlg(D_TAB_SUB, m_Data.AllSubProp.PROP);
		}
		else
		{
			Data2TabDlg(D_TAB_DEFALUT, m_Data.AllProp.PROP);
		}
	}
	else if (m_Tab.GetCurSel() == D_TAB_SUB)
		Data2TabDlg(D_TAB_SUB, m_Data.AllSubProp.PROP);
}

BOOL CIehpPropDlg::Dlg2Data()
{
// 	UpdateData(TRUE);
// 	if(m_nUserAuto == 1 && m_bAssignProperties == FALSE && m_aBackupArr.GetSize() != 0)
// 		LoadDisplayBackupData();

	if (m_Tab.GetCurSel() == D_TAB_DEFALUT)
	{
		if (m_nTabStyle == J_LOCATION)
			return TabDlg2Data(D_TAB_SUB, m_Data.AllSubProp.PROP);
		else
			return TabDlg2Data(D_TAB_DEFALUT, m_Data.AllProp.PROP);
	}
	else if (m_Tab.GetCurSel() == D_TAB_SUB)
	{
		return TabDlg2Data(D_TAB_SUB, m_Data.AllSubProp.PROP);
	}
	else {ASSERT(FALSE); return FALSE;}

	
}
void CIehpPropDlg::SetUnitAndTitle()
{    
	if(m_nDof<3) // None & Dx, Dy, Dz
	{
		m_untCrack.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldForce);
		m_untYield.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldForce);
		m_untUltmt.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldForce);    
		m_untFract.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldForce);
		if(m_nHingeType == 1)  // Distributed
		{    
			m_untDisp1.SetUnitType(D_UNITSYS_NONE);
			m_untDisp2.SetUnitType(D_UNITSYS_NONE);
			m_untDisp3.SetUnitType(D_UNITSYS_NONE);
			m_untDisp4.SetUnitType(D_UNITSYS_NONE);
			m_untExponent.SetUnitType(D_UNITSYS_NONE);
			m_untLoop.SetUnitType(D_UNITSYS_NONE);
			m_untSlip.SetUnitType(D_UNITSYS_NONE);
		}
		// MNET:2815-GSJI-20070525 : Lumped + Fy,Fz 는 D_UNITSYS_NONE
		else if((m_nHingeType == 0 || m_nHingeType == 4) && (m_nDof==1 || m_nDof==2))
		{
			m_untDisp1.SetUnitType(D_UNITSYS_NONE);
			m_untDisp2.SetUnitType(D_UNITSYS_NONE);
			m_untDisp3.SetUnitType(D_UNITSYS_NONE);
			m_untDisp4.SetUnitType(D_UNITSYS_NONE);
			m_untExponent.SetUnitType(D_UNITSYS_NONE);
			m_untLoop.SetUnitType(D_UNITSYS_NONE);
			m_untSlip.SetUnitType(D_UNITSYS_NONE);
		}
		else
		{
			m_untDisp1.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldDisplacement);
			m_untDisp2.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldDisplacement);
			m_untDisp3.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldDisplacement);
			m_untDisp4.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldDisplacement);
			m_untExponent.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldDisplacement);
			m_untLoop.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldDisplacement);
			m_untSlip.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldDisplacement);
		}
		if(m_nHingeType == 0 || m_nHingeType == 4)  // Lumped
		{
			if (m_nDof == 0)
			{
				m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
				m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
			}
			else
			{
				m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
				m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
			}
		}
		else if (m_nHingeType == 1)  // Distributed
		{
			m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
		}
		else if (m_nHingeType == 2)  // Spring
		{
			m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
		}
		else if (m_nHingeType == 3)  // Truss
		{
			m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
		}
		else if (m_nHingeType == D_IEHP_HTYP_POINTSPRING)  // Point Spring
		{
			m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
		}
	}
	else if(m_nDof<6) // None & Rx, Ry, Rz
	{
		m_untCrack.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldMoment);
		m_untYield.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldMoment);    
		m_untUltmt.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldMoment);
		m_untFract.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldMoment);
		if(m_nHingeType == 1)  // Distributed
		{
			m_untDisp1.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);  
			m_untDisp2.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);  
			m_untDisp3.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);  
			m_untDisp4.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);  
			m_untExponent.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);
			m_untLoop.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);
			m_untSlip.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);
		}
		else // Lumped, Spring
		{
			m_untDisp1.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);  
			m_untDisp2.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);  
			m_untDisp3.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);  
			m_untDisp4.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);
			m_untExponent.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);
			m_untLoop.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);
			m_untSlip.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);
		}
		if (m_nHingeType == 0 || m_nHingeType == 4)  // Lumped
		{
			m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffMoment);
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffMoment);
		}
		else if (m_nHingeType == 1)  // Distributed
		{
			m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
		}
		else if (m_nHingeType == 2)  // Spring
		{
			m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffRoSpring);
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffRoSpring);
		}
		else if (m_nHingeType == D_IEHP_HTYP_POINTSPRING)  // Point Spring
		{
			m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffRoSpring);
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffRoSpring);
		}
		else ASSERT(0);
	}
	else if(m_nDof<9) // P-My-Mz & Rx, Ry, Rz
	{
		m_untCrack.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldMoment);
		m_untYield.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldMoment);    
		m_untUltmt.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldMoment);
		m_untFract.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldMoment);
		if(m_nHingeType == 1)  // Distributed
		{
			m_untDisp1.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);  
			m_untDisp2.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);  
			m_untDisp3.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);  
			m_untDisp4.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);  
			m_untExponent.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);
			m_untLoop.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);
			m_untSlip.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldUnitRotn);
		}
		else // Lumped, Spring
		{
			m_untDisp1.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);  
			m_untDisp2.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);  
			m_untDisp3.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);  
			m_untDisp4.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);  
			m_untExponent.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);
			m_untLoop.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);
			m_untSlip.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldRotation);
		}
		if (m_nHingeType == 0 || m_nHingeType == 4)  // Lumped
		{
			m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffMoment);
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffMoment);
		}
		else if (m_nHingeType == 1)  // Distributed
		{
			m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
		}
		else if (m_nHingeType == 2)  // Spring
		{
			m_edtStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffRoSpring);
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffRoSpring);
		}
		else if (m_nHingeType == D_IEHP_HTYP_POINTSPRING)
		{
			ASSERT(0);
		}
		else ASSERT(0);
	}  

	CString str; str.Empty();
	this->GetWindowText(str);
	if(m_nDefinition == 0)
	{
		if     (m_nModelType ==  0) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_KINEMA));             // Kinematic Hardening
		else if(m_nModelType ==  1) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_ORIGIN));             // Origin-oriented
		else if(m_nModelType ==  2) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_PEAK));               // Peak-oriented
		else if(m_nModelType ==  3) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_CLOUGH));             // Clough
		else if(m_nModelType ==  4) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_DEGRAD));             // Degrading Trilinear
		else if(m_nModelType ==  5) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_TAKEDA));             // Takeda
		else if(m_nModelType ==  6) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_MOD_TAKETA));         // Modified Takeda
		else if(m_nModelType ==  7) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_NONL_ELA_SYM));       // 비선형탄성형(대칭)
		else if(m_nModelType ==  8) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_NONL_INV_DIR));       // 비대칭역향형
		else if(m_nModelType ==  9) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_NONL_ELA_ASY));       // 비선형탄성형(비대칭)
		else if(m_nModelType == 10) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_NORBIL));             // Normal Bilinear
		else if(m_nModelType == 11) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_ELABIL));             // Elastic Bilinear
		else if(m_nModelType == 12) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_ELATRI));             // Elastic Trilinear
		else if(m_nModelType == 13) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_ELATET));             // Elastic Tetralinear
		else if(m_nModelType == 14) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_TAKTET));             // Takeda Tetralinear
		else if(m_nModelType == 15) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_MTATET));             // Modified Takeda Tetralinear
		else if(m_nModelType == 30) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_SLIP_BILINEAR));      // SLIP Bilinear
		else if(m_nModelType == 31) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_SLIP_BILINEAR_TENS)); // SLIP Bilinear/Tensile
		else if(m_nModelType == 32) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_SLIP_BILINEAR_COMP)); // SLIP Bilinear/Compress
		else if(m_nModelType == 33) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_SLIP_TRILINEAR));     // SLIP Trilinear
		else if(m_nModelType == 34) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_SLIP_TRILINEAR_TENS));// SLIP Trilinear/Tensile
		else if(m_nModelType == 35) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_SLIP_TRILINEAR_COMP));// SLIP Trilinear/Compress
		else if(m_nModelType == D_IEHP_TAKS) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_TAKS));       // Takeda Slip
		else if(m_nModelType == 92) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_SRCTET));             // SRC Tetralinear
	}
	UINT nPlus[] = { IDC_YIELD_PLUS_STC, IDC_STIFF_PLUS_STC,  IDC_DISP_PLUS_STC};
	UINT nMnus[] = { IDC_YIELD_MNUS_STC, IDC_STIFF_MNUS_STC,  IDC_DISP_MNUS_STC};
	if(m_nDof == 0) // Dx
	{
		for(int i = 0; i < sizeof(nPlus) / sizeof(UINT); i++)
		{
			GetDlgItem(nPlus[i])->SetWindowText(_T("(t)"));
			GetDlgItem(nMnus[i])->SetWindowText(_T("(c)"));
		}
	}
	else
	{
		for(int i = 0; i < sizeof(nPlus) / sizeof(UINT); i++)
		{
			GetDlgItem(nPlus[i])->SetWindowText(_T("(+)"));
			GetDlgItem(nMnus[i])->SetWindowText(_T("(-)"));
		}   
	}

	if(m_nModelType==30 || m_nModelType==31 || m_nModelType==32 ||
		 m_nModelType==33 || m_nModelType==34 || m_nModelType==35)
	{
		GetDlgItem(IDC_IEHP_UNLD_STATIC)->SetWindowText(_LS(IDS_CMD_IEHP_INIT_GAP));
	}
	else
	{
		GetDlgItem(IDC_IEHP_UNLD_STATIC)->SetWindowText(_LS(IDS_CMD_IEHP_STIFF_PARAM));
	}
	GetDlgItem(IDC_IEHP_STIF_LOOP_UNIT)->ShowWindow(FALSE);
}

void CIehpPropDlg::AlignCtrl()
{
	CRect rRef, rToMove;
	int nDistX, nDistY;

	// Align Yield Displacement
	UINT aFirstCtrlID[] = {IDC_IEHP_DISP_FRM};
	CArray<UINT, UINT> *aCtrlSet[] = {&m_aCtrlDelta};
	GetDlgItem(IDC_IEHP_ALPHA_FRM)->GetWindowRect(rRef);
	
	for(int i = 0; i < sizeof(aFirstCtrlID) / sizeof(UINT); ++i)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top  - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, *aCtrlSet[i], nDistX, nDistY);
	}

	// Align Slip Controls
	CArray<UINT, UINT> aCtrl;
	aCtrl.Add(IDC_IEHP_SLIP_T_STC);
	aCtrl.Add(IDC_IEHP_SLIP_T_EDT);
	aCtrl.Add(IDC_IEHP_SLIP_C_STC);
	aCtrl.Add(IDC_IEHP_SLIP_C_EDT);
	aCtrl.Add(IDC_IEHP_SLIP_UNT);
	GetDlgItem(IDC_IEHP_INIT_STIFF_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_IEHP_SLIP_T_STC)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);

	CWnd* pWnd[8];
	pWnd[0] = GetDlgItem(IDC_IEHP_UNLD_STATIC);
	pWnd[1] = GetDlgItem(IDC_IEHP_EXPNT_STATIC);
	pWnd[2] = GetDlgItem(IDC_IEHP_STIF_EXPNT_EDIT);
	pWnd[3] = GetDlgItem(IDC_IEHP_STIF_EXPNT_UNIT);
	pWnd[4] = GetDlgItem(IDC_IEHP_LOOP_STATIC);
	pWnd[5] = GetDlgItem(IDC_IEHP_STIF_LOOP_EDIT);
	pWnd[6] = GetDlgItem(IDC_IEHP_STIF_LOOP_UNIT);
	pWnd[7] = GetDlgItem(IDOK);
	for(int i=0; i<7; i++) 
	{
		pWnd[i]->EnableWindow(TRUE);
		pWnd[i]->ShowWindow(SW_SHOW);
	}

	GetDlgItem(IDC_IEHP_PINCH_STATIC)->EnableWindow(TRUE);
	GetDlgItem(IDC_IEHP_STIF_PINCH_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IEHP_PINCH_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_IEHP_STIF_PINCH_EDIT)->ShowWindow(SW_HIDE);

	BOOL bSlip = FALSE;
	BOOL bSRC = FALSE;
	if(m_nModelType == 3) // Clough
	{
		pWnd[3]->EnableWindow(FALSE);
		pWnd[4]->EnableWindow(FALSE);
		pWnd[5]->EnableWindow(FALSE);
		pWnd[6]->EnableWindow(FALSE);

		m_aAssignHinge.Add(IDC_IEHP_UNLD_STATIC);
		m_aAssignHinge.Add(IDC_IEHP_EXPNT_STATIC);
		m_aAssignHinge.Add(IDC_IEHP_STIF_EXPNT_EDIT);

		pWnd[3]->ShowWindow(SW_HIDE);
		pWnd[4]->ShowWindow(SW_HIDE);
		pWnd[5]->ShowWindow(SW_HIDE);
		pWnd[6]->ShowWindow(SW_HIDE);
	}
	else if(m_nModelType==5 || m_nModelType==6 || m_nModelType==8 || m_nModelType==9 || 
					m_nModelType==14 || m_nModelType==15 || m_nModelType == D_IEHP_TAKS)
	{ //  5:Takeda, 6:Modified Takeda, 7: 비대칭역향형, 8:비선형탄성형(비대칭), 9:비선형탄성형(비대칭)
		// 14:Takeda Tetralinear, 15:Modified Takeda Tetralinear 
		pWnd[3]->EnableWindow(FALSE);
		pWnd[5]->EnableWindow(!m_Data.bAddData4D2E);
		pWnd[6]->EnableWindow(FALSE);

		m_aAssignHinge.Add(IDC_IEHP_UNLD_STATIC);
		m_aAssignHinge.Add(IDC_IEHP_EXPNT_STATIC);
		m_aAssignHinge.Add(IDC_IEHP_STIF_EXPNT_EDIT);
		m_aAssignHinge.Add(IDC_IEHP_LOOP_STATIC);

		pWnd[3]->ShowWindow(SW_HIDE);
		pWnd[6]->ShowWindow(SW_HIDE);

		if(m_nModelType == D_IEHP_TAKS)
		{
			CString str; str.Empty();
			GetDlgItem(IDC_IEHP_UNLD_STATIC)->GetWindowText(str);
			GetDlgItem(IDC_IEHP_UNLD_STATIC)->SetWindowText(str + _T(" / ") + _LS(IDS_IEHP_HYSTERESIS_PARAM));

			GetDlgItem(IDC_IEHP_PINCH_STATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_IEHP_STIF_PINCH_EDIT)->ShowWindow(SW_SHOW);
		}
	}
	else if (m_nModelType == 92) //  SRC Takeda Tetralinear 
	{ 
		pWnd[3]->EnableWindow(FALSE);
		pWnd[4]->EnableWindow(FALSE);
		pWnd[5]->EnableWindow(FALSE);
		pWnd[6]->EnableWindow(FALSE);

		m_aAssignHinge.Add(IDC_IEHP_UNLD_STATIC);
		m_aAssignHinge.Add(IDC_IEHP_EXPNT_STATIC);
		m_aAssignHinge.Add(IDC_IEHP_STIF_EXPNT_EDIT);

		pWnd[3]->ShowWindow(SW_HIDE);
		pWnd[4]->ShowWindow(SW_HIDE);
		pWnd[5]->ShowWindow(SW_HIDE);
		pWnd[6]->ShowWindow(SW_HIDE);


		GetDlgItem(IDC_IEHP_EXPNT_STATIC)->GetWindowRect(rRef);
		GetDlgItem(IDC_IEHP_UNLD_STATIC)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.bottom + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlResizeBottom(this, IDC_IEHP_UNLD_STATIC, nDistY);

		GetDlgItem(IDC_IEHP_UNLD_STATIC)->GetWindowRect(rRef);
		GetDlgItem(IDC_HINGE_TAB1)->GetWindowRect(rToMove);
		rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(5);
		GetDlgItem(IDC_HINGE_TAB1)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_NOMOVE | SWP_NOZORDER);
		rRef = rToMove;
		GetDlgItem(IDOK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aOK, nDistY);

		// resize self size
		CRect rectLast;
		GetDlgItem(IDOK)->GetWindowRect(rectLast);
		CRect r;
		GetWindowRect(r);
		r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(10);
		MoveWindow(r);
	}
	else if(m_nModelType==30 || m_nModelType==33 || m_nModelType==31 ||
					m_nModelType==34 || m_nModelType==32 || m_nModelType==35)
	{
		for(int i=1; i<7; ++i)
		{
			pWnd[i]->EnableWindow(FALSE);
			pWnd[i]->ShowWindow(SW_HIDE);
		}
		bSlip = TRUE;

		m_aAssignHinge.Add(IDC_IEHP_UNLD_STATIC);
	}
	else
	{
		for(int i=0; i<7; i++)
		{
			pWnd[i]->EnableWindow(FALSE);
			pWnd[i]->ShowWindow(SW_HIDE);
		}

		GetDlgItem(IDC_IEHP_INIT_STIFF_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_HINGE_TAB1)->GetWindowRect(rToMove);
		rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(5);
		GetDlgItem(IDC_HINGE_TAB1)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_NOMOVE | SWP_NOZORDER);
		rRef = rToMove;
		GetDlgItem(IDOK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aOK, nDistY);

		// resize self size
		CRect rectLast;
		GetDlgItem(IDOK)->GetWindowRect(rectLast);
		CRect r;
		GetWindowRect(r);
		r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(10);
		MoveWindow(r);
	}

	CDlgUtil::CtrlShowHide(this, aCtrl, bSlip);
	if(bSlip)
	{
		BOOL bT = (m_nModelType==30 || m_nModelType==31 || m_nModelType==33 || m_nModelType==34);
		BOOL bC = (m_nModelType==30 || m_nModelType==32 || m_nModelType==33 || m_nModelType==35);
		GetDlgItem(IDC_IEHP_SLIP_T_EDT)->EnableWindow(bT);
		GetDlgItem(IDC_IEHP_SLIP_C_EDT)->EnableWindow(bC);
	}
	
}
/////////////////////////////////////////////////////////////////////////////
// CIehpPropDlg message handlers

BOOL CIehpPropDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CDialogMove::OnInitDialog();
	
	m_aCtrlAlpha.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlAlpha, IDC_IEHP_ALPHA_FRM, TRUE);
	m_aCtrlDelta.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlDelta, IDC_IEHP_DISP_FRM, TRUE);

	// MQC 4836-48. by Hsshim. Auto 계산할 수 없는 경우에 비활성화.
	m_bDisableAutoCalc = FALSE;    // Auto 계산을 할 수 없는 경우
	if (m_Data.nHingeType == 2) m_bDisableAutoCalc = TRUE;    // nHingeType;  // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가), 4=Wall(CRB)
	if (m_Data.nInteractionType != 0 && m_Data.nInteractionType != 3)  // nInteractionType;   // 0=None, 1=P-M, 2=P-M-M, 3=P-M Multi Curv
	{
		int nDof = m_nDof;
		if (nDof > 5) nDof = m_nDof-3;
		if (nDof == 0 || nDof == 4 || nDof == 5) m_bDisableAutoCalc = TRUE;
	}

	if (m_Data.nHingeType == D_IEHP_HTYP_POINTSPRING)
	{
		m_bDisableAutoCalc = TRUE;
	}

	if(m_bAssignProperties == TRUE && !m_bDisableAutoCalc) // Auto
	{
		if(m_ElemK < 1)	
		{
			ASSERT(0);
			return FALSE;		//Assign 시에는 해당 키값이 1이상이 무조건 들어와야함
		}
		m_pDoc->m_pAttrCtrl->CalcIehpPropValue(m_Data, m_ElemK, m_Data.AllProp.PROP, TRUE, FALSE, TRUE);
	}

	InitTabCtrl();
	InitGraph();
	SetUnitAndTitle();
	AlignCtrl();
	Data2Dlg();
	UpdateGraph();
	CtrlManager();
	OnUserAutoRdo();
	OnSymmetryTypeRdo();  

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIehpPropDlg::OnOK() 
{
	if (!CheckAllData()) return;

	if(!Dlg2Data()) return;

	int nTabCount = m_Tab.GetItemCount();
	if(nTabCount == 1)	
		m_bExistIJData = FALSE;
	else if(nTabCount == 2)
		m_bExistIJData = TRUE;

	if((m_nHingeType == 0 || m_nHingeType == 4) && m_nUserAuto == 1)
		TabDlg2Data(D_TAB_SUB    , m_Data.AllSubProp.PROP);

	int nDof = m_nDof;
	if (nDof > 5) nDof = nDof - 3;  // Interaction Type이 None이 아닌 경우
	m_pDoc->m_pAttrCtrl->CompareUserAutoData(m_nModelType, m_nUserAuto, m_nAlphaDelta, m_Data.AllProp.PROP[nDof], m_Data.AllSubProp.PROP[nDof]);

	CDialogMove::OnOK();
}

void CIehpPropDlg::OnChangeDisp1Edit() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
		UpdateData();
		if(m_nSymmetry == 0 || (m_nModelType==0 || m_nModelType==7))
			m_edtDisp1Mnus.SetEditUnit(m_edtDisp1Plus.GetEditValue());  
	}
	UpdateGraph();
}

void CIehpPropDlg::OnChangeDisp2Edit() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
		UpdateData();
		if(m_nSymmetry == 0 || (m_nModelType==0 || m_nModelType==7)) 
			m_edtDisp2Mnus.SetEditUnit(m_edtDisp2Plus.GetEditValue());
		
	}
	UpdateGraph();
}

void CIehpPropDlg::OnChangeDisp3Edit() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
		UpdateData();
		if(m_nSymmetry == 0 || (m_nModelType==0 || m_nModelType==7)) 
			m_edtDisp3Mnus.SetEditUnit(m_edtDisp3Plus.GetEditValue());
		
	}
	UpdateGraph();
}

void CIehpPropDlg::OnChangeDisp4Edit() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
		UpdateData();
		if(m_nSymmetry == 0 || (m_nModelType==0 || m_nModelType==7))
			m_edtDisp4Mnus.SetEditUnit(m_edtDisp4Plus.GetEditValue());
	}
	UpdateGraph();
}

void CIehpPropDlg::OnChangeFailEdit() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
		UpdateData();
		if(m_nSymmetry == 0 || (m_nAlphaDelta == 1 && (m_nModelType==0 || m_nModelType==7))) 
			m_edtFraMnus.SetEditUnit(m_edtFraPlus.GetEditValue());  
	}
	UpdateGraph();
}

void CIehpPropDlg::OnChangeUltiEdit() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
		UpdateData();
		if(m_nSymmetry == 0 || (m_nAlphaDelta == 1 && (m_nModelType==0 || m_nModelType==7))) 
			m_edtUltMnus.SetEditUnit(m_edtUltPlus.GetEditValue());
	}
	UpdateGraph();
}

void CIehpPropDlg::OnChangeYieldEdit() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
		UpdateData();
		if(m_nSymmetry == 0 || (m_nAlphaDelta == 1 && (m_nModelType==0 || m_nModelType==7))) 
			m_edtYldMnus.SetEditUnit(m_edtYldPlus.GetEditValue());

	}
	UpdateGraph();
}

void CIehpPropDlg::OnChangeCrackEdit() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
	  UpdateData();
	  if(m_nSymmetry == 0 || (m_nAlphaDelta == 1 && (m_nModelType==0 || m_nModelType==7))) 
			m_edtCrkMnus.SetEditUnit(m_edtCrkPlus.GetEditValue());

	}
	UpdateGraph();
}

void CIehpPropDlg::OnChangeStif1stEdit() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
	  UpdateData();
		if(m_nSymmetry == 0 || (m_nModelType==0 || m_nModelType==7)) 
			m_str1Mnus = m_str1Plus;
		UpdateData(FALSE);
	}

	UpdateGraph();
}

void CIehpPropDlg::OnChangeStif2ndEdit() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
	  UpdateData(TRUE);
		if(m_nSymmetry == 0 || (m_nModelType==0 || m_nModelType==7)) 
			m_str2Mnus = m_str2Plus;  
		UpdateData(FALSE);
	}

	UpdateGraph();
}

void CIehpPropDlg::OnChangeStif3rdEdit() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
	  UpdateData(TRUE);
		if(m_nSymmetry == 0 || (m_nModelType==0 || m_nModelType==7)) 
			m_str3Mnus = m_str3Plus;
		UpdateData(FALSE);
	}

	UpdateGraph();
}

void CIehpPropDlg::OnChangeDrawData() 
{
	// TODO: Add your control notification handler code here
	//UpdateChart();
	UpdateGraph();
}

void CIehpPropDlg::OnChangeSlipInitGap()
{
	// Slip 일 경우만 update
	if(m_nModelType==30 || m_nModelType==31 || m_nModelType==32 ||
		 m_nModelType==33 || m_nModelType==34 || m_nModelType==35)
	{
		double dGap_p = m_edtSlipT.GetEditValue();
		double dGap_m = m_edtSlipC.GetEditValue();
		if(dGap_p<0. || dGap_m<0.) return;
		UpdateGraph();
	}
}

void CIehpPropDlg::OnSymmetryTypeRdo() 
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
		if (m_nModelType == 30 || m_nModelType == 33) // 30: SLIP Bilinear   33: SLIP Trilinear
		{
			UpdateData();
			if (m_nSymmetry == 0)
			{
				m_edtSlipC.SetEditUnit(m_edtSlipT.GetEditValue());
			}
		}

		OnChangeYieldEdit();
		OnChangeCrackEdit();
		OnChangeUltiEdit();
		OnChangeFailEdit();
		OnChangeStif1stEdit();
		OnChangeStif2ndEdit();
		OnChangeStif3rdEdit();
		OnChangeDisp1Edit();
		OnChangeDisp2Edit();
		OnChangeDisp3Edit();
		OnChangeDisp4Edit();
	}

	OnChangeDrawData();  

	CtrlManager();

	UpdateData(FALSE);
}

void CIehpPropDlg::OnUserAutoRdo() 
{
	UpdateData(TRUE);  
	if(m_nUserAuto == 1) // Auto
	{
		// MQC:3442-Seungjun-20100621
		if(m_nSymmetry==0)
		{
			m_nSymmetry = 1; // auto는 항상 asymetric
			UpdateData(FALSE);
		}
	}
	
	OnChangeStif1stEdit();
	OnChangeStif2ndEdit();
	OnChangeStif3rdEdit();
	

	UpdateGraph();
	CtrlManager();
}

void CIehpPropDlg::OnStiffTypeRdo() 
{
	CtrlManager();  
}


//////////////////////////////////////////////////////////////////////////////////
// Graph

void CIehpPropDlg::InitGraph()
{
	CWnd* pGraph=GetDlgItem(IDC_PICTURE);
	ASSERT(pGraph);
	
	CRect rect;
	pGraph->GetClientRect(&rect);
	m_Draw.Create(pGraph, rect, NULL);
	m_Draw.FitToParent(rect);

}

// 이 함수 수정하면 CAnalCtrl::GetAlphafromDisplacement 도 함게 수정해야함.
void CIehpPropDlg::UpdateGraph()
{
	UpdateData();
	
	int nAlphaCalcType = 1;   // GetAlphaFromDeltaInIehp() 호출시 Bilinear(1), Trilinear(2), Tetralinear(3) 구분 인자
	double x=0., y=0.;  
	double dP1[2], dP2[2], dP3[2], dP4[2], dD1[2], dD2[2], dD3[2], dD4[2], dA1[2], dA2[2], dA3[2], dK[2], dGap[2];
	for(int i = 0; i < 2; i++)
	{
		dP1[i] = dP2[i] = dP3[i] = dP4[i] = 0.;  // Yield Strength
		dD1[i] = dD2[i] = dD3[i] = dD4[i] = 0.;  // Yield Displacement
		dA1[i] = dA2[i] = dA3[i] = 0.;           // Stiffness Reduction Ratio
		dGap[i] = 0.0;
	}
	
	if(m_nAlphaDelta == 1)  // P-Displacement 입력인 경우 Displacement로부터 Alpha값을 구하는 변환을 한다.
	{ 
		dD1[0] = m_edtDisp1Plus.GetEditValue(); 
		dD1[1] = m_edtDisp1Mnus.GetEditValue(); 
		dD2[0] = m_edtDisp2Plus.GetEditValue(); 
		dD2[1] = m_edtDisp2Mnus.GetEditValue(); 
		dD3[0] = m_edtDisp3Plus.GetEditValue(); 
		dD3[1] = m_edtDisp3Mnus.GetEditValue(); 
		dD4[0] = m_edtDisp4Plus.GetEditValue(); 
		dD4[1] = m_edtDisp4Mnus.GetEditValue(); 
	}

	if(m_nModelType==3  || m_nModelType==10 || m_nModelType==11 || // [Bilinear] clough, Normal Bilinear, Elastic Bilinear
		 m_nModelType==30 || m_nModelType==31 || m_nModelType==32)   // SLIP Bilinear
	{
		dP1[0] = m_edtCrkPlus.GetEditValue();
		dP1[1] = m_edtCrkMnus.GetEditValue();
		dA1[0] = _tstof(m_str1Plus);
		dA1[1] = _tstof(m_str1Mnus);
				
		if(m_nAlphaDelta == 1)  // P-Displacement 입력인 경우 Displacement로부터 Alpha값을 구하는 변환을 한다.
		{ 
			dP2[0] = m_edtYldPlus.GetEditValue();
			dP2[1] = m_edtYldMnus.GetEditValue();
			for(int i = 0; i < 2; i++)
			{
				if(dD1[i] <= 0. || dD2[i] <= 0.) return;
			}
			if(m_nModelType==30 || m_nModelType==31 || m_nModelType==32)
			{
				double dInitGapPositive = m_edtSlipT.GetEditValue();
				double dInitGapNegative = m_edtSlipC.GetEditValue();
				dGap[0] = (m_nModelType!=32) ? dInitGapPositive : 0.0;
				dGap[1] = (m_nModelType!=31) ? dInitGapNegative : 0.0;
			}
			nAlphaCalcType = 1;   // Bilinear
			if(!m_pDoc->m_pEditData->GetAlphaFromDeltaInIehp(nAlphaCalcType, m_nModelType, dP1, dP2, dP3, dP4, dD1, dD2, dD3, dD4, dA1, dA2, dA3, dK, dGap)) return;
		}        
		if(fabs(dA1[0]) < 0. || fabs(dA1[1]) < 0.) return;
	}
	else if(m_nModelType == 13 || m_nModelType == 14 || m_nModelType == 15 || m_nModelType == 92)  // [Tetralinear]
	{
		dP1[0] = m_edtCrkPlus.GetEditValue();
		dP1[1] = m_edtCrkMnus.GetEditValue();
		dP2[0] = m_edtYldPlus.GetEditValue();
		dP2[1] = m_edtYldMnus.GetEditValue();
		dP3[0] = m_edtUltPlus.GetEditValue();
		dP3[1] = m_edtUltMnus.GetEditValue();
		dA1[0] = _tstof(m_str1Plus);
		dA1[1] = _tstof(m_str1Mnus);
		dA2[0] = _tstof(m_str2Plus);
		dA2[1] = _tstof(m_str2Mnus);    
		dA3[0] = _tstof(m_str3Plus);
		dA3[1] = _tstof(m_str3Mnus);
		if(fabs(dP1[0])-fabs(dP2[0])>0. || fabs(dP1[1])-fabs(dP2[1])>0.) return;
		if(fabs(dP2[0])-fabs(dP3[0])>0. || fabs(dP2[1])-fabs(dP3[1])>0.) return;
		if(fabs(dP3[1])<=0. || fabs(dP3[0])<=0.) return;
		
		if(m_nAlphaDelta == 1)  // P-Displacement 입력인 경우 Displacement로부터 Alpha값을 구하는 변환을 한다.
		{ 
			dP4[0] = m_edtFraPlus.GetEditValue();
			dP4[1] = m_edtFraMnus.GetEditValue();
			for(int i = 0; i < 2; i++) 
			{
				if(dD1[i] <= 0. || dD2[i] <= 0. || dD3[i] <= 0. || dD4[i] <= 0.) return;
			}      
			nAlphaCalcType = 3;   // Tetralinear
			if(!m_pDoc->m_pEditData->GetAlphaFromDeltaInIehp(nAlphaCalcType, m_nModelType, dP1, dP2, dP3, dP4, dD1, dD2, dD3, dD4, dA1, dA2, dA3, dK, dGap)) return;
		}
		if(fabs(dA3[0])<0. || fabs(dA3[1])<0.) return;
	}
	else  // [Trilinear]
	{    
		dP1[0] = m_edtCrkPlus.GetEditValue();
		dP1[1] = m_edtCrkMnus.GetEditValue();
		dP2[0] = m_edtYldPlus.GetEditValue();
		dP2[1] = m_edtYldMnus.GetEditValue();
		dA1[0] = _tstof(m_str1Plus);
		dA1[1] = _tstof(m_str1Mnus);
		dA2[0] = _tstof(m_str2Plus);
		dA2[1] = _tstof(m_str2Mnus);    
		if(fabs(dP1[0])-fabs(dP2[0])>0. || fabs(dP1[1])-fabs(dP2[1])>0.) return;  
		if(fabs(dP2[1])<=0. || fabs(dP2[0])<=0.) return;    

		if(m_nAlphaDelta == 1)  // P-Displacement 입력인 경우 Displacement로부터 Alpha값을 구하는 변환을 한다.
		{ 
			dP3[0] = m_edtUltPlus.GetEditValue();
			dP3[1] = m_edtUltMnus.GetEditValue();
			for(int i = 0; i < 2; i++)
			{
				if(dD1[i] <= 0. || dD2[i] <= 0. || dD3[i] <= 0.) return;
			}
			if(m_nModelType==33 || m_nModelType==34 || m_nModelType==35)
			{
				double dInitGapPositive = m_edtSlipT.GetEditValue();
				double dInitGapNegative = m_edtSlipC.GetEditValue();
				dGap[0] = (m_nModelType!=35) ? dInitGapPositive : 0.0;
				dGap[1] = (m_nModelType!=34) ? dInitGapNegative : 0.0;
			}
			nAlphaCalcType = 2;   // Trilinear
			if(!m_pDoc->m_pEditData->GetAlphaFromDeltaInIehp(nAlphaCalcType, m_nModelType, dP1, dP2, dP3, dP4, dD1, dD2, dD3, dD4, dA1, dA2, dA3, dK, dGap)) return;
		}
		if(fabs(dA2[0])<0. || fabs(dA2[1])<0.) return;
	}
	if(m_nModelType==31 || m_nModelType==34) // 31:SLIP Bilinear/Tensile, 34:SLIP Trilinear/Tensile
	{
		dP1[1] = dP1[0];
		dP2[1] = dP2[0];
		dP3[1] = dP3[0];
		dA1[1] = dA1[0];
		dA2[1] = dA2[0];
	}
	if(m_nModelType==32 || m_nModelType==35) // 32:SLIP Bilinear/Compressive, 35:SLIP Trilinear/Compressive
	{
		dP1[0] = dP1[1];
		dP2[0] = dP2[1];
		dP3[0] = dP3[1];
		dA1[0] = dA1[1];
		dA2[0] = dA2[1];
	}

	double X1p=0., X2p=0., X3p=0., X4p=0., X1m=0., X2m=0., X3m=0., X4m=0.;
	double Am=0.;
	double FEp = 0., FEm = 0.;
	
	double dx[9], dy[9];
	for(int i=0; i<9; i++) dx[i] = dy[i] = 0.;
	double dOp=0., dOm=0.;

	if(m_nModelType == 3 || m_nModelType == 10 || m_nModelType == 11) // [Bilinear] Clough, Normal Bilinear, Elastic Bilinear
	{    
		if(fabs( (double)(dA1[0] <= 0.) )) dA1[0] = 1.0e-10;  
		if(fabs( (double)(dA1[1] <= 0.) )) dA1[1] = 1.0e-10;  
		if(fabs( dP1[0])<=0. || fabs(dP1[1])<=0.) return;
		if(fabs(dA1[0])<=0. || fabs(dA1[1])<=0.) return;
		
		// MQC 12222 : Yield displacement방식일 땐 그냥 입력받은 점 가지고 그래프 그림
		// Elastic Bilinear
		if(m_nAlphaDelta==1 && m_nModelType==11)
		{
			X1p = dD1[0];
			X2p = dD2[0];
			X1m = dD1[1];
			X2m = dD2[1];
		}
		else
		{
			X1p = dA1[0];
			X2p = 2. * X1p;
			dP2[0] = X1p*dP1[0] + dP1[0];    

			Am = dP1[1]/(dA1[1]*dP1[0])*X1p;
			X1m = Am * dA1[1];
			X2m = 2. * X1m;
			dP2[1] = X1m*dP1[1] + dP1[1];
		}
		
		if(fabs(dP2[0])<=0. || fabs(dP2[1])<=0.) return;

		double dMaxX1 = max(X1p, X1m);
		double dMaxX2 = max(X2p, X2m);
		double dMaxX  = max(dMaxX1, dMaxX2);
		double dMaxY1 = max(dP1[0], dP1[1]);
		double dMaxY2 = max(dP2[0], dP2[1]);
		double dMaxY  = max(dMaxY1, dMaxY2);
		if(fabs(dMaxX)<=0. || fabs(dMaxY)<=0.) return;
		dx[0] =  0.;            dy[0] =  0.;
		dx[1] =  2.*X1p/dMaxX;  dy[1] =  2.*dP1[0]/dMaxY;
		dx[2] =  2.*X2p/dMaxX;  dy[2] =  2.*dP2[0]/dMaxY;
		dx[3] = -2.*X1m/dMaxX;  dy[3] = -2.*dP1[1]/dMaxY;
		dx[4] = -2.*X2m/dMaxX;  dy[4] = -2.*dP2[1]/dMaxY;
		AdjustEndPoint(dx[4], dy[4], dx[3], dy[3], dx[1], dy[1], dx[2], dy[2]);
	}
	else if(m_nModelType == 13 || m_nModelType == 14 || m_nModelType == 15 || m_nModelType == 92)  // [Tetralinear]
	{
/**/    if(fabs((double)(dA1[0] <= 0.))) dA1[0] = 1.0e-10;  
/**/    if(fabs((double)(dA1[1] <= 0.))) dA1[1] = 1.0e-10;  
/**/    if(fabs((double)(dA2[0] <= 0.))) dA2[0] = 1.0e-10;  
/**/    if(fabs((double)(dA2[1] <= 0.))) dA2[1] = 1.0e-10;  
		if(fabs((double)(dA3[0] <= 0.))) dA3[0] = 1.0e-10;  
		if(fabs((double)(dA3[1] <= 0.))) dA3[1] = 1.0e-10;  
		if(fabs(dP1[0])<=0. || fabs(dP1[1])<=0.) return;
		if(fabs(dA1[0])<=0. || fabs(dA1[1])<=0.) return;
		if(fabs(dA2[0])<=0. || fabs(dA2[1])<=0.) return;
		if(fabs(dA3[0])<=0. || fabs(dA3[1])<=0.) return;

		// MQC 12222 : Yield displacement방식일 땐 그냥 입력받은 점 가지고 그래프 그림
		// Elastic Tetralinear
		if(m_nAlphaDelta==1 && m_nModelType==13)
		{
			X1p = dD1[0];
			X2p = dD2[0];
			X3p = dD3[0];
			X4p = dD4[0];
			X1m = dD1[1];
			X2m = dD2[1];
			X3m = dD3[1];
			X4m = dD4[1];
		}
		else
		{
			X1p = dA1[0] * dA2[0] * dA3[0];
			X2p = X1p + dA2[0] * dA3[0] * (dP2[0] / dP1[0] - 1.);
			X3p = X2p + dA1[0] * dA3[0] * (dP3[0] / dP1[0] - dP2[0] / dP1[0]);
			X4p = 1.5 * X3p;
			if(X1p <= 0.) return;    
			
			Am = dP1[1] / (dA1[1] * dA2[1] * dA3[1] * dP1[0]) * X1p;
			X1m = Am * dA1[1] * dA2[1] * dA3[1];
			X2m = X1m + Am * dA2[1] * dA3[1] * (dP2[1] / dP1[1] - 1.);
			X3m = X2m + Am * dA1[1] * dA3[1] * (dP3[1] / dP1[1] - dP2[1] / dP1[1]);
			X4m = 1.5 * X3m;
			if(X1m <= 0.) return;    
		}

		FEp = dP3[0] + dA3[0] * (X3p - X4p) * dP1[0] / X1p;
		FEm = dP3[1] + dA3[1] * (X3m - X4m) * dP1[1] / X1m;

		double dMaxX = max(max(max(X1p, X1m),max(X2p, X2m)), max(max(X3p, X3m),max(X4p, X4m)));
		double dMaxY = max(max(max(dP1[0], dP1[1]),max(dP2[0], dP2[1])), max(max(dP3[0], dP3[1]),max(FEp, FEm)));
		max(dP3[0], dP3[1]);
		if(fabs(dMaxX)<=0. || fabs(dMaxY)<=0.) return;
		dx[0] =  0.;            dy[0] =  0.;
		dx[1] =  2.*X1p/dMaxX;  dy[1] =  2.*dP1[0]/dMaxY;
		dx[2] =  2.*X2p/dMaxX;  dy[2] =  2.*dP2[0]/dMaxY;
		dx[3] =  2.*X3p/dMaxX;  dy[3] =  2.*dP3[0]/dMaxY;
		dx[4] =  2.*X4p/dMaxX;  dy[4] =  2.*FEp/dMaxY;
		dx[5] = -2.*X1m/dMaxX;  dy[5] = -2.*dP1[1]/dMaxY;
		dx[6] = -2.*X2m/dMaxX;  dy[6] = -2.*dP2[1]/dMaxY;
		dx[7] = -2.*X3m/dMaxX;  dy[7] = -2.*dP3[1]/dMaxY;
		dx[8] = -2.*X4m/dMaxX;  dy[8] = -2.*FEm/dMaxY;
		AdjustEndPoint(dx[8], dy[8], dx[7], dy[7], dx[3], dy[3], dx[4], dy[4]); 
	}
	else if(m_nModelType==30 || m_nModelType==31 || m_nModelType==32 || // SLIP Bilinear
					m_nModelType==33 || m_nModelType==34 || m_nModelType==35)   // SLIP Triliner
	{
		if(fabs((double)(dA1[0] <= 0.))) dA1[0] = 1.0e-10;  
		if(fabs((double)(dA1[1] <= 0.))) dA1[1] = 1.0e-10;  
		if(fabs((double)(dA2[0] <= 0.))) dA2[0] = 1.0e-10;  
		if(fabs((double)(dA2[1] <= 0.))) dA2[1] = 1.0e-10;  
		if(fabs(dP1[0])<=0. || fabs(dP1[1])<=0.) return;
		if(fabs(dA1[0])<=0. || fabs(dA1[1])<=0.) return;
		if(fabs(dA2[0])<=0. || fabs(dA2[1])<=0.) return;
		double dGap_p = m_edtSlipT.GetEditValue();
		double dGap_m = m_edtSlipC.GetEditValue();
		double dGap_p1 = dGap_p>0. ? (dGap_p/max(dGap_p, dGap_m)) : 0.;
		double dGap_m1 = dGap_m>0. ? (dGap_m/max(dGap_p, dGap_m)) : 0.;
		dGap_p = dGap_p1, dGap_m = dGap_m1;
		if(dGap_p<0. || dGap_m<0.) return;
		if(m_nModelType==30 || m_nModelType==31 || m_nModelType==32)
		{
			X1p = dA1[0];
			X2p = 2. * X1p;
			dP2[0] = dP1[0] * dA1[0] + dP1[0];

			Am = dP1[1]/(dA1[1]*dP1[0])*X1p;
			X1m = Am * dA1[1];
			X2m = 2. * X1m;
			dP2[1] = dP1[1] * dA1[1] + dP1[1];
			if(fabs(dP2[0])<=0. || fabs(dP2[1])<=0.) return;

			dGap_p *= X2p/5., dGap_m *= X2p/5.;
			X1p += dGap_p, X2p += dGap_p;
			X1m += dGap_m, X2m += dGap_m;

			double dMaxX = max(X2p, X2m);
			double dMaxY = max(dP2[0], dP2[1]);
			if(fabs(dMaxX)<=0. || fabs(dMaxY)<=0.) return;
			dx[0] =  0.;               dy[0] =  0.;
			dx[1] =  2.*dGap_p/dMaxX;  dy[1] =  0.;
			dx[2] =  2.*X1p/dMaxX;     dy[2] =  2.*dP1[0]/dMaxY;
			dx[3] =  2.*X2p/dMaxX;     dy[3] =  2.*dP2[0]/dMaxY;
			dx[4] = -2.*dGap_m/dMaxX;  dy[4] =  0.;
			dx[5] = -2.*X1m/dMaxX;     dy[5] = -2.*dP1[1]/dMaxY;
			dx[6] = -2.*X2m/dMaxX;     dy[6] = -2.*dP2[1]/dMaxY;
			if(m_nModelType==31) { dx[4] = dx[5] = dx[6] = -2.; dy[4] = dy[5] = dy[6] = 0.; }
			if(m_nModelType==32) { dx[1] = dx[2] = dx[3] =  2.; dy[1] = dy[2] = dy[3] = 0.; }

			AdjustEndPoint(dx[6], dy[6], dx[5], dy[5], dx[2], dy[2], dx[3], dy[3]);
			dGap_p *=  2./dMaxX;
			dGap_m *= -2./dMaxX;
			if(m_nModelType==30 || m_nModelType==31)
			{
				double dKE = fabs(dx[2]-dGap_p)<1.e-10 ? 1.:dy[2]/(dx[2]-dGap_p);
				dOp = dx[3] - dy[3]/dKE;
			}
			if(m_nModelType==30 || m_nModelType==32)
			{
				double dKE = fabs(dx[5]-dGap_m)<1.e-10 ? 1.:dy[5]/(dx[5]-dGap_m);
				dOm = dx[6] - dy[6]/dKE;
			}
		}
		else
		{
			X1p = dA1[0] * dA2[0];
			X2p = X1p + (dP2[0]/dP1[0]-1.)*dA2[0];
			X3p = 2. * X2p;
			dP3[0] = X2p/dA1[0] * dP1[0] + dP2[0];

			Am = (dP1[1]/(dA1[1]*dA2[1]*dP1[0]))*X1p;
			X1m = Am * dA1[1] * dA2[1];
			X2m = X1m + Am*(dP2[1]/dP1[1]-1.)*dA2[1];
			X3m = 2. * X2m;
			dP3[1] = X2m/(Am*dA1[1]) * dP1[1] + dP2[1];
			if(fabs(dP3[0])<=0. || fabs(dP3[1])<=0.) return;

			dGap_p *= X3p/5., dGap_m *= X3p/5.;
			X1p += dGap_p, X2p += dGap_p, X3p += dGap_p;
			X1m += dGap_m, X2m += dGap_m, X3m += dGap_m;

			double dMaxX = max(X3p, X3m);
			double dMaxY = max(dP3[0], dP3[1]);
			if(fabs(dMaxX)<=0. || fabs(dMaxY)<=0.) return;
			dx[0] =  0.;               dy[0] =  0.;
			dx[1] =  2.*dGap_p/dMaxX;  dy[1] =  0.;
			dx[2] =  2.*X1p/dMaxX;     dy[2] =  2.*dP1[0]/dMaxY;
			dx[3] =  2.*X2p/dMaxX;     dy[3] =  2.*dP2[0]/dMaxY;
			dx[4] =  2.*X3p/dMaxX;     dy[4] =  2.*dP3[0]/dMaxY;
			dx[5] = -2.*dGap_m/dMaxX;  dy[5] =  0.;
			dx[6] = -2.*X1m/dMaxX;     dy[6] = -2.*dP1[1]/dMaxY;
			dx[7] = -2.*X2m/dMaxX;     dy[7] = -2.*dP2[1]/dMaxY;
			dx[8] = -2.*X3m/dMaxX;     dy[8] = -2.*dP3[1]/dMaxY;
			if(m_nModelType==34) { dx[5] = dx[6] = dx[7] = dx[8] = -2.; dy[5] = dy[6] = dy[7] = dy[8] = 0.; }
			if(m_nModelType==35) { dx[1] = dx[2] = dx[3] = dx[4] =  2.; dy[1] = dy[2] = dy[3] = dy[4] = 0.; }
			AdjustEndPoint(dx[8], dy[8], dx[7], dy[7], dx[3], dy[3], dx[4], dy[4]); 

			dGap_p *=  2./dMaxX;
			dGap_m *= -2./dMaxX;
			double dKE = 0.;
			if(m_nModelType==33 || m_nModelType==34)
			{
				double dKE = fabs(dx[2]-dGap_p)<1.e-10 ? 1.:dy[2]/(dx[2]-dGap_p);
				dOp = dx[4] - dy[4]/dKE;
			}
			if(m_nModelType==33 || m_nModelType==35)
			{
				double dKE = fabs(dx[6]-dGap_m)<1.e-10 ? 1.:dy[6]/(dx[6]-dGap_m);
				dOm = dx[8] - dy[8]/dKE;
			}
		}
	}
	else  // [Trilinear]
	{
		if(fabs((double)(dA1[0] <= 0.))) dA1[0] = 1.0e-10;  
		if(fabs((double)(dA1[1] <= 0.))) dA1[1] = 1.0e-10;  
		if(fabs((double)(dA2[0] <= 0.))) dA2[0] = 1.0e-10;  
		if(fabs((double)(dA2[1] <= 0.))) dA2[1] = 1.0e-10;  
		if(fabs(dP1[0])<=0. || fabs(dP1[1])<=0.) return;
		if(fabs(dA1[0])<=0. || fabs(dA1[1])<=0.) return;
		if(fabs(dA2[0])<=0. || fabs(dA2[1])<=0.) return;

		// MQC 12222 : Yield displacement방식일 땐 그냥 입력받은 점 가지고 그래프 그림
		// Origin oriented, Peak oriented, Elastic Trilinear
		if(m_nAlphaDelta==1 && (m_nModelType==1 || m_nModelType==2 || m_nModelType==12))
		{
			X1p = dD1[0];
			X2p = dD2[0];
			X3p = dD3[0];
			X1m = dD1[1];
			X2m = dD2[1];
			X3m = dD3[1];
		}
		else
		{
			X1p = dA1[0] * dA2[0];
			X2p = X1p + (dP2[0]/dP1[0]-1.)*dA2[0];
			X3p = 2. * X2p;
			dP3[0] = X2p/dA1[0] * dP1[0] + dP2[0];

			Am = (dP1[1]/(dA1[1]*dA2[1]*dP1[0]))*X1p;
			X1m = Am * dA1[1] * dA2[1];
			X2m = X1m + Am*(dP2[1]/dP1[1]-1.)*dA2[1];
			X3m = 2. * X2m;
			dP3[1] = X2m/(Am*dA1[1]) * dP1[1] + dP2[1];
		}

		if(fabs(dP3[0])<=0. || fabs(dP3[1])<=0.) return;

		double dMaxX = max(max(X1p, X1m),max(max(X2p, X2m),max(X3p, X3m)));
		double dMaxY = max(max(dP1[0], dP1[1]), max(max(dP2[0], dP2[1]),max(dP3[0], dP3[1])));
		if(fabs(dMaxX)<=0. || fabs(dMaxY)<=0.) return;
		dx[0] =  0.;            dy[0] =  0.;
		dx[1] =  2.*X1p/dMaxX;  dy[1] =  2.*dP1[0]/dMaxY;
		dx[2] =  2.*X2p/dMaxX;  dy[2] =  2.*dP2[0]/dMaxY;
		dx[3] =  2.*X3p/dMaxX;  dy[3] =  2.*dP3[0]/dMaxY;
		dx[4] = -2.*X1m/dMaxX;  dy[4] = -2.*dP1[1]/dMaxY;
		dx[5] = -2.*X2m/dMaxX;  dy[5] = -2.*dP2[1]/dMaxY;
		dx[6] = -2.*X3m/dMaxX;  dy[6] = -2.*dP3[1]/dMaxY;
		AdjustEndPoint(dx[6], dy[6], dx[5], dy[5], dx[2], dy[2], dx[3], dy[3]); 
	}
		
	if(m_nModelType == 13 || m_nModelType == 14 || m_nModelType == 15 || m_nModelType == 92)  // [Tetralinear]
		m_Draw.SetDefaultData(-1, dx, dy, dOp, dOm);
	else if(m_nModelType == 3 || m_nModelType == 10 || m_nModelType == 11) // [Bilinear]
		m_Draw.SetDefaultData(1, dx, dy, dOp, dOm);
	else if(m_nModelType==30 || m_nModelType==31 || m_nModelType==32) // SLIP Bilinear
		m_Draw.SetDefaultData(11, dx, dy, dOp, dOm);
	else if(m_nModelType==33 || m_nModelType==34 || m_nModelType==35) // SLIP Trilinear
		m_Draw.SetDefaultData(12, dx, dy, dOp, dOm);
	else              
		m_Draw.SetDefaultData(0, dx, dy, dOp, dOm);
	
	m_Draw.DrawStrengthGraph();
}

// 그래프의 마지막 구간을 늘려 x또는 y 좌표가 2.0(1사분면) 또는 -2.0(3사분면)이 되도록 맞추는 함수
// x maximum과 minimum을 2.0과 -2.0으로 맞추거나,
// y maximum과 minimum을 2.0과 -2.0으로 맞춤
void CIehpPropDlg::AdjustEndPoint(double& xa0, double& ya0, double xa1, double ya1, 
																	double xb0, double yb0, double& xb1, double& yb1) 
{
	double dSlope, dXtmp, dYtmp;
	if(xa0 > -2.0 && ya0 > -2.0)
	{
		if(xa0 == xa1) return;
		dSlope = (ya1 - ya0) / (xa1 - xa0);
		dYtmp = dSlope * (-2.0 - xa0) + ya0;   // (-2.0, dYtmp)
		dXtmp = (-2.0 - ya0) / dSlope + xa0;   // (dXtmp, -2.0)
		if(dYtmp >= -2.0)
		{
			xa0 = -2.0;
			ya0 = dYtmp;
		}
		else if(dXtmp >= -2.0)
		{
			xa0 = dXtmp;
			ya0 = -2.0;
		}
	}
	else if(xb1 < 2.0 && yb1 < 2.0)
	{
		if(xb0 == xb1) return;
		dSlope = (yb1 - yb0) / (xb1 - xb0);
		dYtmp = dSlope * (2.0 - xb0) + yb0;   // (2.0, dYtmp)
		dXtmp = (2.0 - yb0) / dSlope + xb0;   // (dXtmp, 2.0)
		if(dYtmp <= 2.0)
		{
			xb1 = 2.0;
			yb1 = dYtmp;
		}
		else if(dXtmp <= 2.0)
		{
			xb1 = dXtmp;
			yb1 = 2.0;
		}
	}
}


void CIehpPropDlg::OnAlphaDeltaRdo() 
{
	CtrlManager();	
	OnChangeYieldEdit();
	OnChangeCrackEdit();
	OnChangeUltiEdit();
	OnChangeFailEdit();  
	UpdateGraph();
}

void CIehpPropDlg::CtrlManager()
{
	UpdateData(TRUE);

	if(!m_bAssignProperties)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge, TRUE);
	}

	// 6.5.0 까지
	// Lumped && User(Item Dlg) : User만 Enable
	// Lumped && Auto(Item Dlg) : My, Mz 인 경우만 전체 Enable
	// Lumped가 아닌 경우 : 전체 Disable
	int nDof = m_nDof;
	if(nDof > 5) nDof = nDof - 3;  // Interaction Type이 None이 아닌 경우
	int nCtrlType;   // Initial Stiffness의 Ctrl 설정 타입(A(1) ~ G(7)) : 7가지 존재
	int nModifiedStiffType, nModifiedUserAuto, nModifiedAlphaDelta;  

	if(!m_pDoc->m_pEditData->CheckIehpForcedSet(nDof, m_nStiffType, m_nAlphaDelta, 
																							m_nUserAuto, m_nHingeType, m_nIntrType, 1, 
																							nModifiedUserAuto, nModifiedAlphaDelta, nCtrlType, nModifiedStiffType))
	{    
		m_nUserAuto   = nModifiedUserAuto;
		m_nAlphaDelta = nModifiedAlphaDelta;
		m_nStiffType  = nModifiedStiffType;  // 맞지 않는 Stiffness Type radio option을 맞는 값으로 강제 설정
	}
	
	 BOOL bNotUseYieldSurface = (m_nIntrType == 0 || m_nIntrType == 3)? TRUE : FALSE;
//   BOOL bFyFzInterNone = FALSE;
//   if(m_nIntrType == 0 || nDof == 1 || nDof == 2) bFyFzInterNone = TRUE; 
	BOOL bFyFzInterNone = TRUE;     // 2014-07-14. by hsshim. MQC 4836-48. User Defined 타입에서 입력비활성화 되는 control 수정
	if (!bNotUseYieldSurface && (nDof == 0 || nDof == 4 || nDof == 5))  bFyFzInterNone = FALSE;

	BOOL bPMMTable = (m_nIntrType == 2 && m_nStiffType == 5 && !bFyFzInterNone);
	BOOL bPMMTakeda = (m_nIntrType == 2 && 
		((m_nModelType == D_IEHP_MTTE || m_nModelType == D_IEHP_MTAK) || (m_nModelType == D_IEHP_TTET || m_nModelType == D_IEHP_TAKE)));

	GetDlgItem(IDC_IEHP_ALPHA_DELTA_RDO2)->EnableWindow(m_nUserAuto == 0 && (bFyFzInterNone || bPMMTakeda)); // Fy, Fz는 PM, PMM인 경우에도 None처럼 작동(2004.12.29 지규선 요청)
	GetDlgItem(IDC_IEHP_ALPHA_DELTA_RDO)->EnableWindow(!bPMMTable || bPMMTakeda);

	GetDlgItem(IDC_STIFF_6EI_RDO)  ->EnableWindow(nCtrlType == 3);
	GetDlgItem(IDC_STIFF_3EI_RDO)  ->EnableWindow(nCtrlType == 3);
	GetDlgItem(IDC_STIFF_2EI_RDO)  ->EnableWindow(nCtrlType == 3);
	GetDlgItem(IDC_STIFF_USER_RDO) ->EnableWindow(nCtrlType == 1 || nCtrlType == 3 || nCtrlType == 4);
	GetDlgItem(IDC_STIFF_USER_EDIT)->EnableWindow((nCtrlType == 1 || nCtrlType == 3 || nCtrlType == 4) && m_nStiffType == 3);
	GetDlgItem(IDC_STIFF_USER_UNIT)->EnableWindow((nCtrlType == 1 || nCtrlType == 3 || nCtrlType == 4) && m_nStiffType == 3);
	GetDlgItem(IDC_STIFF_ESTIF_RDO)->EnableWindow(nCtrlType == 1 || nCtrlType == 2 || nCtrlType == 7);
	GetDlgItem(IDC_STIFF_SCURV_RDO)->EnableWindow(nCtrlType == 2 || nCtrlType == 5);
			
	GetDlgItem(IDC_IEHP_ALPHA_FRM)->ShowWindow(m_nAlphaDelta == 0);
	GetDlgItem(IDC_IEHP_DISP_FRM)->ShowWindow(m_nAlphaDelta == 1);
	
	BOOL bBilinearModel = m_pDoc->m_pAttrCtrl->IsIehpHysModel_Bilinear(m_nModelType);
	BOOL bTrilinearModel = m_pDoc->m_pAttrCtrl->IsIehpHysModel_Trilinear(m_nModelType);
	BOOL bTetralinearModel = m_pDoc->m_pAttrCtrl->IsIehpHysModel_Tetralinear(m_nModelType);
		
	if(m_nAlphaDelta == 0)  // P-Alpha
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlP1, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlP2, bTrilinearModel || bTetralinearModel);
		CDlgUtil::CtrlShowHide(this, m_aCtrlP3, bTetralinearModel);
		CDlgUtil::CtrlShowHide(this, m_aCtrlP4, FALSE);
	}
	else if(m_nAlphaDelta == 1)  // P-Delta
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlP1, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlP2, bBilinearModel || bTrilinearModel || bTetralinearModel);
		CDlgUtil::CtrlShowHide(this, m_aCtrlP3, bTrilinearModel || bTetralinearModel);
		CDlgUtil::CtrlShowHide(this, m_aCtrlP4, bTetralinearModel);
	}

	CDlgUtil::CtrlShowHide(this, m_aCtrlA0, m_nAlphaDelta==0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlA1, m_nAlphaDelta==0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlA2, m_nAlphaDelta == 0 && (bTrilinearModel || bTetralinearModel));
	CDlgUtil::CtrlShowHide(this, m_aCtrlA3, m_nAlphaDelta == 0 && bTetralinearModel);

	CDlgUtil::CtrlShowHide(this, m_aCtrlD0, m_nAlphaDelta == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlD1, m_nAlphaDelta == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlD2, m_nAlphaDelta == 1 && (bBilinearModel || bTrilinearModel || bTetralinearModel));
	CDlgUtil::CtrlShowHide(this, m_aCtrlD3, m_nAlphaDelta == 1 && (bTrilinearModel || bTetralinearModel));
	CDlgUtil::CtrlShowHide(this, m_aCtrlD4, m_nAlphaDelta == 1 && bTetralinearModel);

	if(bBilinearModel)
	{
		if(m_nDeformDefineType == 1) m_nDeformDefineType = 0;
		//GetDlgItem(IDC_IEHP_DFOM_RDO_Y)->EnableWindow(FALSE);  
	}

	BOOL bForcedSymmetry = FALSE;   // Asymmetry인 경우에도 Symmetry처럼 취급하는 경우(Kinematic Model)
	if(m_nModelType == 0 || m_nModelType == 7) bForcedSymmetry = TRUE;

	BOOL bTensOnly = (m_nModelType==31 || m_nModelType==34);
	BOOL bCompOnly = (m_nModelType==32 || m_nModelType==35);

	GetDlgItem(IDC_TYPE_SYM_RDO) ->EnableWindow(m_nUserAuto==0 && !bTensOnly && !bCompOnly);
	GetDlgItem(IDC_TYPE_ASYM_RDO)->EnableWindow(m_nUserAuto==0 && !bTensOnly && !bCompOnly);

	GetDlgItem(IDC_IEHP_D1_PLUS_EDIT)->EnableWindow(!bCompOnly);
	GetDlgItem(IDC_IEHP_D2_PLUS_EDIT)->EnableWindow(!bCompOnly);
	GetDlgItem(IDC_IEHP_D3_PLUS_EDIT)->EnableWindow(!bCompOnly);
	GetDlgItem(IDC_IEHP_D4_PLUS_EDIT)->EnableWindow(!bCompOnly);

	GetDlgItem(IDC_IEHP_STIF_1ST_PLUS_EDIT)->EnableWindow(!bCompOnly);
	GetDlgItem(IDC_IEHP_STIF_2ND_PLUS_EDIT)->EnableWindow(!bCompOnly);
	GetDlgItem(IDC_IEHP_STIF_3RD_PLUS_EDIT)->EnableWindow(!bCompOnly);

	GetDlgItem(IDC_IEHP_SLIP_T_EDT)->EnableWindow(!bCompOnly);

	if (bTensOnly)
	{
		GetDlgItem(IDC_IEHP_D1_MNUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_D2_MNUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_D3_MNUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_D4_MNUS_EDIT)->EnableWindow(FALSE);

		GetDlgItem(IDC_IEHP_STIF_1ST_MNUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_STIF_2ND_MNUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_STIF_3RD_MNUS_EDIT)->EnableWindow(FALSE);

		GetDlgItem(IDC_IEHP_SLIP_C_EDT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_IEHP_D1_MNUS_EDIT)->EnableWindow((m_nSymmetry == 1 && !bForcedSymmetry) || bCompOnly);
		GetDlgItem(IDC_IEHP_D2_MNUS_EDIT)->EnableWindow((m_nSymmetry == 1 && !bForcedSymmetry) || bCompOnly);
		GetDlgItem(IDC_IEHP_D3_MNUS_EDIT)->EnableWindow((m_nSymmetry == 1 && !bForcedSymmetry) || bCompOnly);
		GetDlgItem(IDC_IEHP_D4_MNUS_EDIT)->EnableWindow((m_nSymmetry == 1 && !bForcedSymmetry) || bCompOnly);

		GetDlgItem(IDC_IEHP_STIF_1ST_MNUS_EDIT)->EnableWindow((m_nSymmetry == 1 && !bForcedSymmetry) || bCompOnly);
		GetDlgItem(IDC_IEHP_STIF_2ND_MNUS_EDIT)->EnableWindow((m_nSymmetry == 1 && !bForcedSymmetry) || bCompOnly);
		GetDlgItem(IDC_IEHP_STIF_3RD_MNUS_EDIT)->EnableWindow((m_nSymmetry == 1 && !bForcedSymmetry) || bCompOnly);

		GetDlgItem(IDC_IEHP_SLIP_C_EDT)->EnableWindow((m_nSymmetry == 1 && !bForcedSymmetry) || bCompOnly);
	}

	//GetDlgItem(IDC_YIELD_USER_RDO)->EnableWindow(m_nIntrType == 0);  
	////MQC-Alpha&Beta 4827-7 전단성분의 내력 자동계산을 지원 = /* && (nDof != 1 && nDof != 2)*/
	//GetDlgItem(IDC_YIELD_AUTO_RDO)->EnableWindow(m_nIntrType == 0 && m_nHingeType != 2 /* && (nDof != 1 && nDof != 2)*/);	
	// MQC 4836-48. by Hsshim. Auto 계산할 수 없는 경우에 비활성화.
	GetDlgItem(IDC_YIELD_USER_RDO)->EnableWindow(!m_bDisableAutoCalc);  
	GetDlgItem(IDC_YIELD_AUTO_RDO)->EnableWindow(!m_bDisableAutoCalc);
		
	GetDlgItem(IDC_IEHP_CRACK_UNIT)->EnableWindow(bFyFzInterNone);
	GetDlgItem(IDC_IEHP_YIELD_UNIT)->EnableWindow(bFyFzInterNone);
	GetDlgItem(IDC_IEHP_ULTI_UNIT)->EnableWindow(bFyFzInterNone);
	GetDlgItem(IDC_IEHP_FAIL_UNIT)->EnableWindow(bFyFzInterNone);
		
	if(bTensOnly || bCompOnly)
	{
		if(m_nUserAuto==1) bTensOnly = bCompOnly = FALSE;
		GetDlgItem(IDC_IEHP_CRACK_PLUS_EDIT)->EnableWindow(bTensOnly);
		GetDlgItem(IDC_IEHP_CRACK_MNUS_EDIT)->EnableWindow(bCompOnly);
		GetDlgItem(IDC_IEHP_YIELD_PLUS_EDIT)->EnableWindow(bTensOnly);
		GetDlgItem(IDC_IEHP_YIELD_MNUS_EDIT)->EnableWindow(bCompOnly);
		GetDlgItem(IDC_IEHP_ULTI_PLUS_EDIT)->EnableWindow(bTensOnly);
		GetDlgItem(IDC_IEHP_ULTI_MNUS_EDIT)->EnableWindow(bCompOnly);
		GetDlgItem(IDC_IEHP_FAIL_PLUS_EDIT)->EnableWindow(bTensOnly);
		GetDlgItem(IDC_IEHP_FAIL_MNUS_EDIT)->EnableWindow(bCompOnly);
	}
	else
	{
		BOOL bPlus = (m_nUserAuto==0 && (bFyFzInterNone || (bPMMTakeda && m_nAlphaDelta == 1)));
		BOOL bMinus = (m_nUserAuto==0 && (bFyFzInterNone || (bPMMTakeda && m_nAlphaDelta == 1)) && m_nSymmetry==1 &&
									(m_nAlphaDelta == 0 || (m_nAlphaDelta == 1 && !bForcedSymmetry)));
																										 
		GetDlgItem(IDC_IEHP_CRACK_PLUS_EDIT)->EnableWindow(bPlus);
		GetDlgItem(IDC_IEHP_CRACK_MNUS_EDIT)->EnableWindow(bMinus);
		GetDlgItem(IDC_IEHP_YIELD_PLUS_EDIT)->EnableWindow(bPlus);
		GetDlgItem(IDC_IEHP_YIELD_MNUS_EDIT)->EnableWindow(bMinus);
		GetDlgItem(IDC_IEHP_ULTI_PLUS_EDIT)->EnableWindow(bPlus);
		GetDlgItem(IDC_IEHP_ULTI_MNUS_EDIT)->EnableWindow(bMinus);
		GetDlgItem(IDC_IEHP_FAIL_PLUS_EDIT)->EnableWindow(bPlus);
		GetDlgItem(IDC_IEHP_FAIL_MNUS_EDIT)->EnableWindow(bMinus);
	}
	
	if((m_nHingeType == 0 || m_nHingeType == 4) && m_nUserAuto == 0 && m_nTabStyle == I_J_LOCATION)
	{
		GetDlgItem(IDC_STATIC_IEND_JEND)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TYPE_SYM_RDO2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TYPE_ASYM_RDO2)->ShowWindow(SW_SHOW);
		if(m_nValueTypeSymmetry == -1)
		{
			m_nValueTypeSymmetry = 0;
			UpdateData(FALSE);
			ChangeTabCtrl(I_LOCATION);
		}
	}
	else
	{
		GetDlgItem(IDC_STATIC_IEND_JEND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TYPE_SYM_RDO2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TYPE_ASYM_RDO2)->ShowWindow(SW_HIDE);

		if((m_nHingeType == 0 || m_nHingeType == 4) && m_nUserAuto == 1 && m_nTabStyle == I_J_LOCATION && m_bAssignProperties == TRUE)
		{

		}
		else
		{
			m_nValueTypeSymmetry = -1;
			m_bExistIJData = FALSE;
			UpdateData(FALSE);
			ChangeTabCtrl(LOCATION_NONE);	
		}		
	}

	if (bPMMTable && !bPMMTakeda)
	{
		GetDlgItem(IDC_IEHP_D1_PLUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_D2_PLUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_D3_PLUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_D4_PLUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_D1_MNUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_D2_MNUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_D3_MNUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IEHP_D4_MNUS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_TYPE_SYM_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_TYPE_ASYM_RDO)->EnableWindow(FALSE);
	}

	if(m_bAssignProperties)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge, FALSE);
	}

	UpdateData(FALSE);
}

bool CIehpPropDlg::SyncDialogData()
{
	if (!CheckAllData()) return false;

	if (!Dlg2Data()) return false;

	int nTabCount = m_Tab.GetItemCount();
	if (nTabCount == 1)
		m_bExistIJData = FALSE;
	else if (nTabCount == 2)
		m_bExistIJData = TRUE;

	if ((m_nHingeType == 0 || m_nHingeType == 4) && m_nUserAuto == 1)
		TabDlg2Data(D_TAB_SUB, m_Data.AllSubProp.PROP);

	int nDof = m_nDof;
	if (nDof > 5) nDof = nDof - 3;  // Interaction Type이 None이 아닌 경우
	m_pDoc->m_pAttrCtrl->CompareUserAutoData(m_nModelType, m_nUserAuto, m_nAlphaDelta, m_Data.AllProp.PROP[nDof], m_Data.AllSubProp.PROP[nDof]);

	return true;
}

BOOL CIehpPropDlg::InitTabCtrl()
{
	m_Tab.DeleteAllItems();
	if(m_bExistIJData == TRUE)		
	{
		m_nValueTypeSymmetry = 1;
		UpdateData(FALSE);
	}
	if((m_Data.nHingeType == 0 || m_Data.nHingeType == 4) && m_nUserAuto == 1 && m_bAssignProperties == TRUE)
	{
		BOOL bEqual = CompareProp(m_Data.AllProp.PROP[m_nDof], m_Data.AllSubProp.PROP[m_nDof]);
		if(bEqual)		
		{
			m_nValueTypeSymmetry = 0;
			m_nSymmetry = 0;
		}
		else	
		{	
			m_nValueTypeSymmetry = 1;
			m_nSymmetry = 1;
		}

		UpdateData(FALSE);
	}
	
	ChangeTabCtrl(m_nTabStyle);
	m_Tab.SetCurSel(0);
	
 	return TRUE;
}

void CIehpPropDlg::ChangeTabCtrl(int m_nTabStyle)
{
	UpdateData(TRUE);

	CString aTitle[5]  = { _LS(IDS_RPS_PO_DOF_Properties),_LS(IDS_RPS_PO_DOF_PropertiesI), _LS(IDS_RPS_PO_DOF_PropertiesJ),
		_LS(IDS_RPS_PO_DOF_PropertiesIJ), _LS(IDS_RPS_PO_DOF_PropertiesCenter) }; 

	m_Tab.DeleteAllItems();

	switch (m_nTabStyle)
	{
	case LOCATION_NONE:
		m_Tab.InsertItem(0, aTitle[0]); 
		break;
	case I_LOCATION:
		m_Tab.InsertItem(0, aTitle[1]);
		break;
	case J_LOCATION:
		m_Tab.InsertItem(0, aTitle[2]);
		break;
	case I_J_LOCATION:
		if(m_nValueTypeSymmetry == 1)	//asymmetry
		{
			m_Tab.InsertItem(0, aTitle[1]);
			m_Tab.InsertItem(1, aTitle[2]);
		}
		else
			m_Tab.InsertItem(0, aTitle[0]); 
		
		break;
	case CENTER_LOCATION:
		m_Tab.InsertItem(0, aTitle[4]);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	Invalidate();
}

void CIehpPropDlg::Data2TabDlg(const int nTabID, T_IEHP_PROP* PROP)
{
	if(m_bAssignProperties)
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge, TRUE);
		
	
		int nD = m_nDof;
		if(m_nDof>5) nD = m_nDof-3;

		switch(m_nModelType)
		{
		case 0: case 7: // 0:Kinematic Hardening, 7:비선형탄성형(대칭)
			{
				m_nSymmetry = PROP[nD].KINEMA.nSymmetryType;
				m_nUserAuto = PROP[nD].KINEMA.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].KINEMA.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].KINEMA.dCrackForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].KINEMA.dCrackForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].KINEMA.dYieldForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].KINEMA.dYieldForce[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].KINEMA.dUltimateForce[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].KINEMA.dUltimateForce[1]);   
					m_edtDisp1Plus.SetEditUnit(PROP[nD].KINEMA.dYieldDisp1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].KINEMA.dYieldDisp1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].KINEMA.dYieldDisp2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].KINEMA.dYieldDisp2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].KINEMA.dYieldDisp3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].KINEMA.dYieldDisp3rd[1]);
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].KINEMA.dCrackMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].KINEMA.dCrackMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].KINEMA.dYieldMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].KINEMA.dYieldMoment[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].KINEMA.dUltimateMoment[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].KINEMA.dUltimateMoment[1]);					
					m_edtDisp1Plus.SetEditUnit(PROP[nD].KINEMA.dYieldRotn1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].KINEMA.dYieldRotn1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].KINEMA.dYieldRotn2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].KINEMA.dYieldRotn2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].KINEMA.dYieldRotn3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].KINEMA.dYieldRotn3rd[1]);
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].KINEMA.dStiffRatio1st[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].KINEMA.dStiffRatio1st[1]);
				m_str2Plus.Format(_T("%g"), PROP[nD].KINEMA.dStiffRatio2nd[0]);
				m_str2Mnus.Format(_T("%g"), PROP[nD].KINEMA.dStiffRatio2nd[1]);
				m_nStiffType = PROP[nD].KINEMA.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j] = PROP[nD].KINEMA.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].KINEMA.nDeformDefineType;
				break;
			}    
		case 1: // Origin-oriented
			{
				m_nSymmetry = PROP[nD].ORIGIN.nSymmetryType;
				m_nUserAuto = PROP[nD].ORIGIN.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].ORIGIN.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].ORIGIN.dCrackForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].ORIGIN.dCrackForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].ORIGIN.dYieldForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].ORIGIN.dYieldForce[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].ORIGIN.dUltimateForce[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].ORIGIN.dUltimateForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].ORIGIN.dYieldDisp1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].ORIGIN.dYieldDisp1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].ORIGIN.dYieldDisp2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].ORIGIN.dYieldDisp2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].ORIGIN.dYieldDisp3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].ORIGIN.dYieldDisp3rd[1]);
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].ORIGIN.dCrackMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].ORIGIN.dCrackMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].ORIGIN.dYieldMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].ORIGIN.dYieldMoment[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].ORIGIN.dUltimateMoment[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].ORIGIN.dUltimateMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].ORIGIN.dYieldRotn1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].ORIGIN.dYieldRotn1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].ORIGIN.dYieldRotn2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].ORIGIN.dYieldRotn2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].ORIGIN.dYieldRotn3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].ORIGIN.dYieldRotn3rd[1]);
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].ORIGIN.dStiffRatio1st[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].ORIGIN.dStiffRatio1st[1]);
				m_str2Plus.Format(_T("%g"), PROP[nD].ORIGIN.dStiffRatio2nd[0]);
				m_str2Mnus.Format(_T("%g"), PROP[nD].ORIGIN.dStiffRatio2nd[1]);
				m_nStiffType = PROP[nD].ORIGIN.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].ORIGIN.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].ORIGIN.nDeformDefineType;
				break;
			}
		case 2: // Peak-oriented
			{
				m_nSymmetry = PROP[nD].PEAK.nSymmetryType;
				m_nUserAuto = PROP[nD].PEAK.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].PEAK.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].PEAK.dCrackForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].PEAK.dCrackForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].PEAK.dYieldForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].PEAK.dYieldForce[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].PEAK.dUltimateForce[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].PEAK.dUltimateForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].PEAK.dYieldDisp1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].PEAK.dYieldDisp1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].PEAK.dYieldDisp2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].PEAK.dYieldDisp2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].PEAK.dYieldDisp3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].PEAK.dYieldDisp3rd[1]);
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].PEAK.dCrackMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].PEAK.dCrackMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].PEAK.dYieldMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].PEAK.dYieldMoment[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].PEAK.dUltimateMoment[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].PEAK.dUltimateMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].PEAK.dYieldRotn1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].PEAK.dYieldRotn1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].PEAK.dYieldRotn2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].PEAK.dYieldRotn2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].PEAK.dYieldRotn3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].PEAK.dYieldRotn3rd[1]);
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].PEAK.dStiffRatio1st[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].PEAK.dStiffRatio1st[1]);
				m_str2Plus.Format(_T("%g"), PROP[nD].PEAK.dStiffRatio2nd[0]);
				m_str2Mnus.Format(_T("%g"), PROP[nD].PEAK.dStiffRatio2nd[1]);
				m_nStiffType = PROP[nD].PEAK.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].PEAK.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].PEAK.nDeformDefineType;
				break;
			}
		case 3: // Clough
			{
				m_nSymmetry = PROP[nD].CLOUGH.nSymmetryType;
				m_nUserAuto = PROP[nD].CLOUGH.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].CLOUGH.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].CLOUGH.dYieldForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].CLOUGH.dYieldForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].CLOUGH.dUltimateForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].CLOUGH.dUltimateForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].CLOUGH.dYieldDisp2nd[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].CLOUGH.dYieldDisp2nd[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].CLOUGH.dYieldDisp3rd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].CLOUGH.dYieldDisp3rd[1]);        
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].CLOUGH.dYieldMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].CLOUGH.dYieldMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].CLOUGH.dUltimateMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].CLOUGH.dUltimateMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].CLOUGH.dYieldRotn2nd[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].CLOUGH.dYieldRotn2nd[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].CLOUGH.dYieldRotn3rd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].CLOUGH.dYieldRotn3rd[1]);        
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].CLOUGH.dStiffRatio2nd[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].CLOUGH.dStiffRatio2nd[1]);
				m_edtExponent.SetEditUnit(PROP[nD].CLOUGH.dUnloadStiffCalcExpo);
				m_nStiffType = PROP[nD].CLOUGH.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].CLOUGH.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].CLOUGH.nDeformDefineType;
				break;
			}
		case 4: // Degrading Trilinear
			{
				m_nSymmetry = PROP[nD].DEGRAD.nSymmetryType;
				m_nUserAuto = PROP[nD].DEGRAD.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].DEGRAD.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].DEGRAD.dCrackForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].DEGRAD.dCrackForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].DEGRAD.dYieldForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].DEGRAD.dYieldForce[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].DEGRAD.dUltimateForce[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].DEGRAD.dUltimateForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].DEGRAD.dYieldDisp1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].DEGRAD.dYieldDisp1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].DEGRAD.dYieldDisp2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].DEGRAD.dYieldDisp2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].DEGRAD.dYieldDisp3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].DEGRAD.dYieldDisp3rd[1]);
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].DEGRAD.dCrackMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].DEGRAD.dCrackMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].DEGRAD.dYieldMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].DEGRAD.dYieldMoment[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].DEGRAD.dUltimateMoment[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].DEGRAD.dUltimateMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].DEGRAD.dYieldRotn1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].DEGRAD.dYieldRotn1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].DEGRAD.dYieldRotn2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].DEGRAD.dYieldRotn2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].DEGRAD.dYieldRotn3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].DEGRAD.dYieldRotn3rd[1]);
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].DEGRAD.dStiffRatio1st[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].DEGRAD.dStiffRatio1st[1]);
				m_str2Plus.Format(_T("%g"), PROP[nD].DEGRAD.dStiffRatio2nd[0]);
				m_str2Mnus.Format(_T("%g"), PROP[nD].DEGRAD.dStiffRatio2nd[1]);
				m_nStiffType = PROP[nD].DEGRAD.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].DEGRAD.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].DEGRAD.nDeformDefineType;
				break;
			}
		case 5: case 6: case 8: case 9: case D_IEHP_TAKS:// 5: Takeda, 6: Modified Takeda, 8: 비대칭역향형, 9:비선형탄성형(비대칭), 74: Takeda Slip
			{
				m_nSymmetry = PROP[nD].TAKEDA.nSymmetryType;
				m_nUserAuto = PROP[nD].TAKEDA.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].TAKEDA.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].TAKEDA.dCrackForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].TAKEDA.dCrackForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].TAKEDA.dYieldForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].TAKEDA.dYieldForce[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].TAKEDA.dUltimateForce[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].TAKEDA.dUltimateForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].TAKEDA.dYieldDisp1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].TAKEDA.dYieldDisp1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].TAKEDA.dYieldDisp2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].TAKEDA.dYieldDisp2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].TAKEDA.dYieldDisp3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].TAKEDA.dYieldDisp3rd[1]);
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].TAKEDA.dCrackMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].TAKEDA.dCrackMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].TAKEDA.dYieldMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].TAKEDA.dYieldMoment[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].TAKEDA.dUltimateMoment[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].TAKEDA.dUltimateMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].TAKEDA.dYieldRotn1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].TAKEDA.dYieldRotn1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].TAKEDA.dYieldRotn2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].TAKEDA.dYieldRotn2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].TAKEDA.dYieldRotn3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].TAKEDA.dYieldRotn3rd[1]);
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].TAKEDA.dStiffRatio1st[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].TAKEDA.dStiffRatio1st[1]);
				m_str2Plus.Format(_T("%g"), PROP[nD].TAKEDA.dStiffRatio2nd[0]);
				m_str2Mnus.Format(_T("%g"), PROP[nD].TAKEDA.dStiffRatio2nd[1]);
				m_nStiffType = PROP[nD].TAKEDA.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				m_edtExponent.SetEditUnit(PROP[nD].TAKEDA.dUnloadStiffCalcExpo);
				m_edtLoop.SetEditUnit(PROP[nD].TAKEDA.dUnloadStiffReduFac);
				m_edtPinch.SetEditUnit(PROP[nD].TAKEDA.dPinchingRuleFac);

				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].TAKEDA.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].TAKEDA.nDeformDefineType;
				break;
			}
		case 10: // Normal Bilinear
			{
				m_nSymmetry = PROP[nD].NORBIL.nSymmetryType;
				m_nUserAuto = PROP[nD].NORBIL.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].NORBIL.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].NORBIL.dYieldForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].NORBIL.dYieldForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].NORBIL.dUltimateForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].NORBIL.dUltimateForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].NORBIL.dYieldDisp2nd[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].NORBIL.dYieldDisp2nd[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].NORBIL.dYieldDisp3rd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].NORBIL.dYieldDisp3rd[1]);        
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].NORBIL.dYieldMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].NORBIL.dYieldMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].NORBIL.dUltimateMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].NORBIL.dUltimateMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].NORBIL.dYieldRotn2nd[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].NORBIL.dYieldRotn2nd[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].NORBIL.dYieldRotn3rd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].NORBIL.dYieldRotn3rd[1]);        
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].NORBIL.dStiffRatio2nd[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].NORBIL.dStiffRatio2nd[1]);
				m_nStiffType = PROP[nD].NORBIL.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].NORBIL.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].NORBIL.nDeformDefineType;      
				break;
			}
		case 11: // Elastic Bilinear    
			{
				m_nSymmetry = PROP[nD].ELABIL.nSymmetryType;
				m_nUserAuto = PROP[nD].ELABIL.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].ELABIL.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].ELABIL.dYieldForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].ELABIL.dYieldForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].ELABIL.dUltimateForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].ELABIL.dUltimateForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].ELABIL.dYieldDisp2nd[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].ELABIL.dYieldDisp2nd[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].ELABIL.dYieldDisp3rd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].ELABIL.dYieldDisp3rd[1]);        
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].ELABIL.dYieldMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].ELABIL.dYieldMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].ELABIL.dUltimateMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].ELABIL.dUltimateMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].ELABIL.dYieldRotn2nd[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].ELABIL.dYieldRotn2nd[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].ELABIL.dYieldRotn3rd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].ELABIL.dYieldRotn3rd[1]);        
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].ELABIL.dStiffRatio2nd[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].ELABIL.dStiffRatio2nd[1]);
				m_nStiffType = PROP[nD].ELABIL.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].ELABIL.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].ELABIL.nDeformDefineType;
				break;
			}
		case 12: // Elastic Trilinear    
			{
				m_nSymmetry = PROP[nD].ELATRI.nSymmetryType;
				m_nUserAuto = PROP[nD].ELATRI.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].ELATRI.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].ELATRI.dCrackForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].ELATRI.dCrackForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].ELATRI.dYieldForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].ELATRI.dYieldForce[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].ELATRI.dUltimateForce[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].ELATRI.dUltimateForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].ELATRI.dYieldDisp1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].ELATRI.dYieldDisp1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].ELATRI.dYieldDisp2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].ELATRI.dYieldDisp2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].ELATRI.dYieldDisp3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].ELATRI.dYieldDisp3rd[1]);
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].ELATRI.dCrackMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].ELATRI.dCrackMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].ELATRI.dYieldMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].ELATRI.dYieldMoment[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].ELATRI.dUltimateMoment[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].ELATRI.dUltimateMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].ELATRI.dYieldRotn1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].ELATRI.dYieldRotn1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].ELATRI.dYieldRotn2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].ELATRI.dYieldRotn2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].ELATRI.dYieldRotn3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].ELATRI.dYieldRotn3rd[1]);
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].ELATRI.dStiffRatio1st[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].ELATRI.dStiffRatio1st[1]);
				m_str2Plus.Format(_T("%g"), PROP[nD].ELATRI.dStiffRatio2nd[0]);
				m_str2Mnus.Format(_T("%g"), PROP[nD].ELATRI.dStiffRatio2nd[1]);
				m_nStiffType = PROP[nD].ELATRI.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].ELATRI.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].ELATRI.nDeformDefineType;
				break;
			}
		case 13: // Elastic Tetralinear
			{
				m_nSymmetry = PROP[nD].ELATET.nSymmetryType;
				m_nUserAuto = PROP[nD].ELATET.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].ELATET.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].ELATET.dCrackForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].ELATET.dCrackForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].ELATET.dYieldForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].ELATET.dYieldForce[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].ELATET.dUltimateForce[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].ELATET.dUltimateForce[1]);
					m_edtFraPlus.SetEditUnit(PROP[nD].ELATET.dFractureForce[0]);
					m_edtFraMnus.SetEditUnit(PROP[nD].ELATET.dFractureForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].ELATET.dYieldDisp1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].ELATET.dYieldDisp1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].ELATET.dYieldDisp2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].ELATET.dYieldDisp2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].ELATET.dYieldDisp3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].ELATET.dYieldDisp3rd[1]);
					m_edtDisp4Plus.SetEditUnit(PROP[nD].ELATET.dYieldDisp4th[0]);
					m_edtDisp4Mnus.SetEditUnit(PROP[nD].ELATET.dYieldDisp4th[1]);
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].ELATET.dCrackMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].ELATET.dCrackMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].ELATET.dYieldMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].ELATET.dYieldMoment[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].ELATET.dUltimateMoment[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].ELATET.dUltimateMoment[1]);
					m_edtFraPlus.SetEditUnit(PROP[nD].ELATET.dFractureMoment[0]);
					m_edtFraMnus.SetEditUnit(PROP[nD].ELATET.dFractureMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].ELATET.dYieldRotn1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].ELATET.dYieldRotn1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].ELATET.dYieldRotn2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].ELATET.dYieldRotn2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].ELATET.dYieldRotn3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].ELATET.dYieldRotn3rd[1]);
					m_edtDisp4Plus.SetEditUnit(PROP[nD].ELATET.dYieldRotn4th[0]);
					m_edtDisp4Mnus.SetEditUnit(PROP[nD].ELATET.dYieldRotn4th[1]);
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].ELATET.dStiffRatio1st[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].ELATET.dStiffRatio1st[1]);
				m_str2Plus.Format(_T("%g"), PROP[nD].ELATET.dStiffRatio2nd[0]);
				m_str2Mnus.Format(_T("%g"), PROP[nD].ELATET.dStiffRatio2nd[1]);
				m_str3Plus.Format(_T("%g"), PROP[nD].ELATET.dStiffRatio3rd[0]);
				m_str3Mnus.Format(_T("%g"), PROP[nD].ELATET.dStiffRatio3rd[1]);
				m_nStiffType = PROP[nD].ELATET.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].ELATET.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].ELATET.nDeformDefineType;
				break;
			}    
		case 14: case 15: // 14:Takeda Tetralinear, 15:Modified Takeda Tetralinear
			{
				m_nSymmetry = PROP[nD].TAKTET.nSymmetryType;
				m_nUserAuto = PROP[nD].TAKTET.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].TAKTET.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].TAKTET.dCrackForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].TAKTET.dCrackForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].TAKTET.dYieldForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].TAKTET.dYieldForce[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].TAKTET.dUltimateForce[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].TAKTET.dUltimateForce[1]);
					m_edtFraPlus.SetEditUnit(PROP[nD].TAKTET.dFractureForce[0]);
					m_edtFraMnus.SetEditUnit(PROP[nD].TAKTET.dFractureForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].TAKTET.dYieldDisp1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].TAKTET.dYieldDisp1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].TAKTET.dYieldDisp2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].TAKTET.dYieldDisp2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].TAKTET.dYieldDisp3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].TAKTET.dYieldDisp3rd[1]);
					m_edtDisp4Plus.SetEditUnit(PROP[nD].TAKTET.dYieldDisp4th[0]);
					m_edtDisp4Mnus.SetEditUnit(PROP[nD].TAKTET.dYieldDisp4th[1]);
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].TAKTET.dCrackMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].TAKTET.dCrackMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].TAKTET.dYieldMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].TAKTET.dYieldMoment[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].TAKTET.dUltimateMoment[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].TAKTET.dUltimateMoment[1]);
					m_edtFraPlus.SetEditUnit(PROP[nD].TAKTET.dFractureMoment[0]);
					m_edtFraMnus.SetEditUnit(PROP[nD].TAKTET.dFractureMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].TAKTET.dYieldRotn1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].TAKTET.dYieldRotn1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].TAKTET.dYieldRotn2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].TAKTET.dYieldRotn2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].TAKTET.dYieldRotn3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].TAKTET.dYieldRotn3rd[1]);
					m_edtDisp4Plus.SetEditUnit(PROP[nD].TAKTET.dYieldRotn4th[0]);
					m_edtDisp4Mnus.SetEditUnit(PROP[nD].TAKTET.dYieldRotn4th[1]);
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].TAKTET.dStiffRatio1st[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].TAKTET.dStiffRatio1st[1]);
				m_str2Plus.Format(_T("%g"), PROP[nD].TAKTET.dStiffRatio2nd[0]);
				m_str2Mnus.Format(_T("%g"), PROP[nD].TAKTET.dStiffRatio2nd[1]);
				m_str3Plus.Format(_T("%g"), PROP[nD].TAKTET.dStiffRatio3rd[0]);
				m_str3Mnus.Format(_T("%g"), PROP[nD].TAKTET.dStiffRatio3rd[1]);
				m_nStiffType = PROP[nD].TAKTET.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				m_edtExponent.SetEditUnit(PROP[nD].TAKTET.dUnloadStiffCalcExpo);
				m_edtLoop.SetEditUnit(PROP[nD].TAKTET.dUnloadStiffReduFac);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].TAKTET.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].TAKTET.nDeformDefineType;
				break;
			} 
		case 92: // 92:SRC Tetralinear 
		{
			m_nSymmetry = PROP[nD].SRCTET.nSymmetryType;
			m_nUserAuto = PROP[nD].SRCTET.nYieldStrengthOpt;
			m_nAlphaDelta = PROP[nD].SRCTET.nPAlphaDelta;
			if (m_nDof < 3)
			{
				m_edtCrkPlus.SetEditUnit(PROP[nD].SRCTET.dCrackForce[0]);
				m_edtCrkMnus.SetEditUnit(PROP[nD].SRCTET.dCrackForce[1]);
				m_edtYldPlus.SetEditUnit(PROP[nD].SRCTET.dYieldForce[0]);
				m_edtYldMnus.SetEditUnit(PROP[nD].SRCTET.dYieldForce[1]);
				m_edtUltPlus.SetEditUnit(PROP[nD].SRCTET.dUltimateForce[0]);
				m_edtUltMnus.SetEditUnit(PROP[nD].SRCTET.dUltimateForce[1]);
				m_edtFraPlus.SetEditUnit(PROP[nD].SRCTET.dFractureForce[0]);
				m_edtFraMnus.SetEditUnit(PROP[nD].SRCTET.dFractureForce[1]);
				m_edtDisp1Plus.SetEditUnit(PROP[nD].SRCTET.dYieldDisp1st[0]);
				m_edtDisp1Mnus.SetEditUnit(PROP[nD].SRCTET.dYieldDisp1st[1]);
				m_edtDisp2Plus.SetEditUnit(PROP[nD].SRCTET.dYieldDisp2nd[0]);
				m_edtDisp2Mnus.SetEditUnit(PROP[nD].SRCTET.dYieldDisp2nd[1]);
				m_edtDisp3Plus.SetEditUnit(PROP[nD].SRCTET.dYieldDisp3rd[0]);
				m_edtDisp3Mnus.SetEditUnit(PROP[nD].SRCTET.dYieldDisp3rd[1]);
				m_edtDisp4Plus.SetEditUnit(PROP[nD].SRCTET.dYieldDisp4th[0]);
				m_edtDisp4Mnus.SetEditUnit(PROP[nD].SRCTET.dYieldDisp4th[1]);
			}
			else
			{
				m_edtCrkPlus.SetEditUnit(PROP[nD].SRCTET.dCrackMoment[0]);
				m_edtCrkMnus.SetEditUnit(PROP[nD].SRCTET.dCrackMoment[1]);
				m_edtYldPlus.SetEditUnit(PROP[nD].SRCTET.dYieldMoment[0]);
				m_edtYldMnus.SetEditUnit(PROP[nD].SRCTET.dYieldMoment[1]);
				m_edtUltPlus.SetEditUnit(PROP[nD].SRCTET.dUltimateMoment[0]);
				m_edtUltMnus.SetEditUnit(PROP[nD].SRCTET.dUltimateMoment[1]);
				m_edtFraPlus.SetEditUnit(PROP[nD].SRCTET.dFractureMoment[0]);
				m_edtFraMnus.SetEditUnit(PROP[nD].SRCTET.dFractureMoment[1]);
				m_edtDisp1Plus.SetEditUnit(PROP[nD].SRCTET.dYieldRotn1st[0]);
				m_edtDisp1Mnus.SetEditUnit(PROP[nD].SRCTET.dYieldRotn1st[1]);
				m_edtDisp2Plus.SetEditUnit(PROP[nD].SRCTET.dYieldRotn2nd[0]);
				m_edtDisp2Mnus.SetEditUnit(PROP[nD].SRCTET.dYieldRotn2nd[1]);
				m_edtDisp3Plus.SetEditUnit(PROP[nD].SRCTET.dYieldRotn3rd[0]);
				m_edtDisp3Mnus.SetEditUnit(PROP[nD].SRCTET.dYieldRotn3rd[1]);
				m_edtDisp4Plus.SetEditUnit(PROP[nD].SRCTET.dYieldRotn4th[0]);
				m_edtDisp4Mnus.SetEditUnit(PROP[nD].SRCTET.dYieldRotn4th[1]);
			}
			m_str1Plus.Format(_T("%g"), PROP[nD].SRCTET.dStiffRatio1st[0]);
			m_str1Mnus.Format(_T("%g"), PROP[nD].SRCTET.dStiffRatio1st[1]);
			m_str2Plus.Format(_T("%g"), PROP[nD].SRCTET.dStiffRatio2nd[0]);
			m_str2Mnus.Format(_T("%g"), PROP[nD].SRCTET.dStiffRatio2nd[1]);
			m_str3Plus.Format(_T("%g"), PROP[nD].SRCTET.dStiffRatio3rd[0]);
			m_str3Mnus.Format(_T("%g"), PROP[nD].SRCTET.dStiffRatio3rd[1]);
			m_nStiffType = PROP[nD].SRCTET.nInitStiffType;
			SetInitialStiffness4HingeType(nD, PROP);
			m_edtExponent.SetEditUnit(PROP[nD].SRCTET.dUnloadStiffCalcExpo);

			for (int i = 0; i < 2; i++) for (int j = 0; j < 5; j++)
				m_dDeformCapacity[i][j] = PROP[nD].SRCTET.dDeformCapacity[i][j];
			m_nDeformDefineType = PROP[nD].SRCTET.nDeformDefineType;
			break;
		}
			// 30:SLIP Bilinear,  31:SLIP Bilinear/Tensile,  32:SLIP Bilinear/Compressive
		case 30: case 31: case 32:
			{
				m_nSymmetry = PROP[nD].SLIP.nSymmetryType;
				m_nUserAuto = PROP[nD].SLIP.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].SLIP.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].SLIP.dYieldForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].SLIP.dYieldForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].SLIP.dUltimateForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].SLIP.dUltimateForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].SLIP.dYieldDisp2nd[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].SLIP.dYieldDisp2nd[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].SLIP.dYieldDisp3rd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].SLIP.dYieldDisp3rd[1]);        
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].SLIP.dYieldMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].SLIP.dYieldMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].SLIP.dUltimateMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].SLIP.dUltimateMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].SLIP.dYieldRotn2nd[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].SLIP.dYieldRotn2nd[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].SLIP.dYieldRotn3rd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].SLIP.dYieldRotn3rd[1]);        
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].SLIP.dStiffRatio2nd[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].SLIP.dStiffRatio2nd[1]);
				m_nStiffType = PROP[nD].SLIP.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				m_edtSlipT.SetEditUnit(PROP[nD].SLIP.dInitGapPositive);
				m_edtSlipC.SetEditUnit(PROP[nD].SLIP.dInitGapNegative);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].SLIP.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].SLIP.nDeformDefineType;      
				break;
			}
			// 33:SLIP Trilinear, 34:SLIP Trilinear/Tensile, 35:SLIP Trilinear/Compressive
		case 33: case 34: case 35:
			{
				m_nSymmetry = PROP[nD].SLIP.nSymmetryType;
				m_nUserAuto = PROP[nD].SLIP.nYieldStrengthOpt;
				m_nAlphaDelta = PROP[nD].SLIP.nPAlphaDelta;
				if(m_nDof<3)
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].SLIP.dCrackForce[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].SLIP.dCrackForce[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].SLIP.dYieldForce[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].SLIP.dYieldForce[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].SLIP.dUltimateForce[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].SLIP.dUltimateForce[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].SLIP.dYieldDisp1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].SLIP.dYieldDisp1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].SLIP.dYieldDisp2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].SLIP.dYieldDisp2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].SLIP.dYieldDisp3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].SLIP.dYieldDisp3rd[1]);
				}
				else
				{
					m_edtCrkPlus.SetEditUnit(PROP[nD].SLIP.dCrackMoment[0]);
					m_edtCrkMnus.SetEditUnit(PROP[nD].SLIP.dCrackMoment[1]);
					m_edtYldPlus.SetEditUnit(PROP[nD].SLIP.dYieldMoment[0]);
					m_edtYldMnus.SetEditUnit(PROP[nD].SLIP.dYieldMoment[1]);
					m_edtUltPlus.SetEditUnit(PROP[nD].SLIP.dUltimateMoment[0]);
					m_edtUltMnus.SetEditUnit(PROP[nD].SLIP.dUltimateMoment[1]);
					m_edtDisp1Plus.SetEditUnit(PROP[nD].SLIP.dYieldRotn1st[0]);
					m_edtDisp1Mnus.SetEditUnit(PROP[nD].SLIP.dYieldRotn1st[1]);
					m_edtDisp2Plus.SetEditUnit(PROP[nD].SLIP.dYieldRotn2nd[0]);
					m_edtDisp2Mnus.SetEditUnit(PROP[nD].SLIP.dYieldRotn2nd[1]);
					m_edtDisp3Plus.SetEditUnit(PROP[nD].SLIP.dYieldRotn3rd[0]);
					m_edtDisp3Mnus.SetEditUnit(PROP[nD].SLIP.dYieldRotn3rd[1]);
				}
				m_str1Plus.Format(_T("%g"), PROP[nD].SLIP.dStiffRatio1st[0]);
				m_str1Mnus.Format(_T("%g"), PROP[nD].SLIP.dStiffRatio1st[1]);
				m_str2Plus.Format(_T("%g"), PROP[nD].SLIP.dStiffRatio2nd[0]);
				m_str2Mnus.Format(_T("%g"), PROP[nD].SLIP.dStiffRatio2nd[1]);
				m_nStiffType = PROP[nD].SLIP.nInitStiffType;
				SetInitialStiffness4HingeType(nD, PROP);
				m_edtSlipT.SetEditUnit(PROP[nD].SLIP.dInitGapPositive);
				m_edtSlipC.SetEditUnit(PROP[nD].SLIP.dInitGapNegative);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_dDeformCapacity[i][j]=PROP[nD].SLIP.dDeformCapacity[i][j];
				m_nDeformDefineType = PROP[nD].SLIP.nDeformDefineType;
				break;
			} 
		}

		//if(m_nIntrType==0) m_nUserAuto=0;

		if (m_Data.nHingeType == D_IEHP_HTYP_POINTSPRING)
		{
			m_nUserAuto = FALSE;
		}

		//MQC-Alpha&Beta 4827-7 전단성분의 내력 자동계산을 지원
		//if(m_nDof == 1 || m_nDof == 2) m_nUserAuto = 0;  // Fy, Fz에 대해서는 AutoCalc 미지원으로 임시로 user 로 설정  
		UpdateData(FALSE);

		if(m_bAssignProperties)
			CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge, FALSE);
}
BOOL CIehpPropDlg::TabDlg2Data(const int nTabID, T_IEHP_PROP* PROP)
{
		if(!CheckData()) return FALSE;
		UpdateData(TRUE);
		int nD = m_nDof;
		if(m_nDof>5) nD = m_nDof-3;

		int nCompareType = m_nAlphaDelta;
		double dP1_org=0.0, dD1_org=0.0, dUser_org=0.0;
		double dP1_tgt=0.0, dD1_tgt=0.0, dUser_tgt=0.0;

		dP1_tgt = m_edtCrkPlus.GetEditValue();
		dD1_tgt = m_edtDisp1Plus.GetEditValue();
		dUser_tgt = m_edtStiffUser.GetEditValue();

		// MQC 12231 : Deformation capacity 값 초기화 되는 문제
		SaveDeformCapacity(nD, PROP);

		switch(m_nModelType)
		{
		case 0: case 7: // 0:Kinematic Hardening, 7:비선형탄성형(대칭)
			{
				PROP[nD].KINEMA.nSymmetryType = m_nSymmetry;
				PROP[nD].KINEMA.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].KINEMA.nPAlphaDelta = m_nAlphaDelta;

				dUser_org = PROP[nD].KINEMA.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].KINEMA.dCrackForce[0];  
					dD1_org = PROP[nD].KINEMA.dYieldDisp1st[0];
					PROP[nD].KINEMA.dCrackForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].KINEMA.dCrackForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].KINEMA.dYieldForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].KINEMA.dYieldForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].KINEMA.dUltimateForce[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].KINEMA.dUltimateForce[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].KINEMA.dYieldDisp1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].KINEMA.dYieldDisp1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].KINEMA.dYieldDisp2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].KINEMA.dYieldDisp2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].KINEMA.dYieldDisp3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].KINEMA.dYieldDisp3rd[1] = m_edtDisp3Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].KINEMA.dCrackMoment[0];  
					dD1_org = PROP[nD].KINEMA.dYieldRotn1st[0];
					PROP[nD].KINEMA.dCrackMoment[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].KINEMA.dCrackMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].KINEMA.dYieldMoment[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].KINEMA.dYieldMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].KINEMA.dUltimateMoment[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].KINEMA.dUltimateMoment[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].KINEMA.dYieldRotn1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].KINEMA.dYieldRotn1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].KINEMA.dYieldRotn2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].KINEMA.dYieldRotn2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].KINEMA.dYieldRotn3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].KINEMA.dYieldRotn3rd[1] = m_edtDisp3Mnus.GetEditValue();        
				}
				PROP[nD].KINEMA.dStiffRatio1st[0] = _tstof(m_str1Plus);
				PROP[nD].KINEMA.dStiffRatio1st[1] = _tstof(m_str1Mnus);
				PROP[nD].KINEMA.dStiffRatio2nd[0] = _tstof(m_str2Plus);
				PROP[nD].KINEMA.dStiffRatio2nd[1] = _tstof(m_str2Mnus);
				PROP[nD].KINEMA.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].KINEMA.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].KINEMA.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		case 1: // Origin-oriented
			{
				PROP[nD].ORIGIN.nSymmetryType = m_nSymmetry;
				PROP[nD].ORIGIN.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].ORIGIN.nPAlphaDelta = m_nAlphaDelta;

				dUser_org = PROP[nD].ORIGIN.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].ORIGIN.dCrackForce[0];  
					dD1_org = PROP[nD].ORIGIN.dYieldDisp1st[0];
					PROP[nD].ORIGIN.dCrackForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].ORIGIN.dCrackForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].ORIGIN.dYieldForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].ORIGIN.dYieldForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].ORIGIN.dUltimateForce[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].ORIGIN.dUltimateForce[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].ORIGIN.dYieldDisp1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].ORIGIN.dYieldDisp1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].ORIGIN.dYieldDisp2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].ORIGIN.dYieldDisp2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].ORIGIN.dYieldDisp3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].ORIGIN.dYieldDisp3rd[1] = m_edtDisp3Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].ORIGIN.dCrackMoment[0];  
					dD1_org = PROP[nD].ORIGIN.dYieldRotn1st[0];
					PROP[nD].ORIGIN.dCrackMoment[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].ORIGIN.dCrackMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].ORIGIN.dYieldMoment[0] = m_edtYldPlus.GetEditValue();;
					PROP[nD].ORIGIN.dYieldMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].ORIGIN.dUltimateMoment[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].ORIGIN.dUltimateMoment[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].ORIGIN.dYieldRotn1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].ORIGIN.dYieldRotn1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].ORIGIN.dYieldRotn2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].ORIGIN.dYieldRotn2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].ORIGIN.dYieldRotn3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].ORIGIN.dYieldRotn3rd[1] = m_edtDisp3Mnus.GetEditValue();        
				}
				PROP[nD].ORIGIN.dStiffRatio1st[0] = _tstof(m_str1Plus);
				PROP[nD].ORIGIN.dStiffRatio1st[1] = _tstof(m_str1Mnus);
				PROP[nD].ORIGIN.dStiffRatio2nd[0] = _tstof(m_str2Plus);
				PROP[nD].ORIGIN.dStiffRatio2nd[1] = _tstof(m_str2Mnus);
				PROP[nD].ORIGIN.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].ORIGIN.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].ORIGIN.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		case 2: // Peak-oriented
			{
				PROP[nD].PEAK.nSymmetryType = m_nSymmetry;
				PROP[nD].PEAK.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].PEAK.nPAlphaDelta = m_nAlphaDelta;

				dUser_org = PROP[nD].PEAK.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].PEAK.dCrackForce[0];  
					dD1_org = PROP[nD].PEAK.dYieldDisp1st[0];
					PROP[nD].PEAK.dCrackForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].PEAK.dCrackForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].PEAK.dYieldForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].PEAK.dYieldForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].PEAK.dUltimateForce[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].PEAK.dUltimateForce[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].PEAK.dYieldDisp1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].PEAK.dYieldDisp1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].PEAK.dYieldDisp2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].PEAK.dYieldDisp2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].PEAK.dYieldDisp3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].PEAK.dYieldDisp3rd[1] = m_edtDisp3Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].PEAK.dCrackMoment[0];  
					dD1_org = PROP[nD].PEAK.dYieldRotn1st[0];
					PROP[nD].PEAK.dCrackMoment[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].PEAK.dCrackMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].PEAK.dYieldMoment[0] = m_edtYldPlus.GetEditValue();;
					PROP[nD].PEAK.dYieldMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].PEAK.dUltimateMoment[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].PEAK.dUltimateMoment[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].PEAK.dYieldRotn1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].PEAK.dYieldRotn1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].PEAK.dYieldRotn2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].PEAK.dYieldRotn2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].PEAK.dYieldRotn3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].PEAK.dYieldRotn3rd[1] = m_edtDisp3Mnus.GetEditValue();        
				}
				PROP[nD].PEAK.dStiffRatio1st[0] = _tstof(m_str1Plus);
				PROP[nD].PEAK.dStiffRatio1st[1] = _tstof(m_str1Mnus);
				PROP[nD].PEAK.dStiffRatio2nd[0] = _tstof(m_str2Plus);
				PROP[nD].PEAK.dStiffRatio2nd[1] = _tstof(m_str2Mnus);
				PROP[nD].PEAK.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].PEAK.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].PEAK.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		case 3: // Clough
			{
				PROP[nD].CLOUGH.nSymmetryType = m_nSymmetry;
				PROP[nD].CLOUGH.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].CLOUGH.nPAlphaDelta = m_nAlphaDelta;

				dUser_org = PROP[nD].CLOUGH.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].CLOUGH.dYieldForce[0];  
					dD1_org = PROP[nD].CLOUGH.dYieldDisp2nd[0];
					PROP[nD].CLOUGH.dYieldForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].CLOUGH.dYieldForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].CLOUGH.dUltimateForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].CLOUGH.dUltimateForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].CLOUGH.dYieldDisp2nd[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].CLOUGH.dYieldDisp2nd[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].CLOUGH.dYieldDisp3rd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].CLOUGH.dYieldDisp3rd[1] = m_edtDisp2Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].CLOUGH.dYieldMoment[0];  
					dD1_org = PROP[nD].CLOUGH.dYieldRotn2nd[0];
					PROP[nD].CLOUGH.dYieldMoment[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].CLOUGH.dYieldMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].CLOUGH.dUltimateMoment[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].CLOUGH.dUltimateMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].CLOUGH.dYieldRotn2nd[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].CLOUGH.dYieldRotn2nd[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].CLOUGH.dYieldRotn3rd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].CLOUGH.dYieldRotn3rd[1] = m_edtDisp2Mnus.GetEditValue();        
				}
				PROP[nD].CLOUGH.dStiffRatio2nd[0] = _tstof(m_str1Plus);
				PROP[nD].CLOUGH.dStiffRatio2nd[1] = _tstof(m_str1Mnus);
				PROP[nD].CLOUGH.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				PROP[nD].CLOUGH.dUnloadStiffCalcExpo = m_edtExponent.GetEditValue();
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].CLOUGH.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].CLOUGH.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		case 4: // Degrading Trilinear
			{
				PROP[nD].DEGRAD.nSymmetryType = m_nSymmetry;
				PROP[nD].DEGRAD.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].DEGRAD.nPAlphaDelta = m_nAlphaDelta;
				dUser_org = PROP[nD].DEGRAD.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].DEGRAD.dCrackForce[0];  
					dD1_org = PROP[nD].DEGRAD.dYieldDisp1st[0];
					PROP[nD].DEGRAD.dCrackForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].DEGRAD.dCrackForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].DEGRAD.dYieldForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].DEGRAD.dYieldForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].DEGRAD.dUltimateForce[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].DEGRAD.dUltimateForce[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].DEGRAD.dYieldDisp1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].DEGRAD.dYieldDisp1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].DEGRAD.dYieldDisp2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].DEGRAD.dYieldDisp2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].DEGRAD.dYieldDisp3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].DEGRAD.dYieldDisp3rd[1] = m_edtDisp3Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].DEGRAD.dCrackMoment[0];  
					dD1_org = PROP[nD].DEGRAD.dYieldRotn1st[0];
					PROP[nD].DEGRAD.dCrackMoment[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].DEGRAD.dCrackMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].DEGRAD.dYieldMoment[0] = m_edtYldPlus.GetEditValue();;
					PROP[nD].DEGRAD.dYieldMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].DEGRAD.dUltimateMoment[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].DEGRAD.dUltimateMoment[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].DEGRAD.dYieldRotn1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].DEGRAD.dYieldRotn1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].DEGRAD.dYieldRotn2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].DEGRAD.dYieldRotn2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].DEGRAD.dYieldRotn3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].DEGRAD.dYieldRotn3rd[1] = m_edtDisp3Mnus.GetEditValue();        
				}
				PROP[nD].DEGRAD.dStiffRatio1st[0] = _tstof(m_str1Plus);
				PROP[nD].DEGRAD.dStiffRatio1st[1] = _tstof(m_str1Mnus);
				PROP[nD].DEGRAD.dStiffRatio2nd[0] = _tstof(m_str2Plus);
				PROP[nD].DEGRAD.dStiffRatio2nd[1] = _tstof(m_str2Mnus);
				PROP[nD].DEGRAD.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].DEGRAD.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].DEGRAD.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		case 5: case 6: case 8: case 9: case D_IEHP_TAKS:// 5: Takeda, 6 : Modified Takeda, 8: 비대칭역향형, 9:비선형탄성형(비대칭), 74: Takeda Slip
			{
				PROP[nD].TAKEDA.nSymmetryType = m_nSymmetry;
				PROP[nD].TAKEDA.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].TAKEDA.nPAlphaDelta = m_nAlphaDelta;

				dUser_org = PROP[nD].TAKEDA.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].TAKEDA.dCrackForce[0];  
					dD1_org = PROP[nD].TAKEDA.dYieldDisp1st[0];
					PROP[nD].TAKEDA.dCrackForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].TAKEDA.dCrackForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].TAKEDA.dYieldForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].TAKEDA.dYieldForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].TAKEDA.dUltimateForce[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].TAKEDA.dUltimateForce[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].TAKEDA.dYieldDisp1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].TAKEDA.dYieldDisp1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].TAKEDA.dYieldDisp2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].TAKEDA.dYieldDisp2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].TAKEDA.dYieldDisp3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].TAKEDA.dYieldDisp3rd[1] = m_edtDisp3Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].TAKEDA.dCrackMoment[0];  
					dD1_org = PROP[nD].TAKEDA.dYieldRotn1st[0];
					PROP[nD].TAKEDA.dCrackMoment[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].TAKEDA.dCrackMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].TAKEDA.dYieldMoment[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].TAKEDA.dYieldMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].TAKEDA.dUltimateMoment[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].TAKEDA.dUltimateMoment[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].TAKEDA.dYieldRotn1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].TAKEDA.dYieldRotn1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].TAKEDA.dYieldRotn2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].TAKEDA.dYieldRotn2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].TAKEDA.dYieldRotn3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].TAKEDA.dYieldRotn3rd[1] = m_edtDisp3Mnus.GetEditValue();        
				}
				PROP[nD].TAKEDA.dStiffRatio1st[0] = _tstof(m_str1Plus);
				PROP[nD].TAKEDA.dStiffRatio1st[1] = _tstof(m_str1Mnus);
				PROP[nD].TAKEDA.dStiffRatio2nd[0] = _tstof(m_str2Plus);
				PROP[nD].TAKEDA.dStiffRatio2nd[1] = _tstof(m_str2Mnus);
				PROP[nD].TAKEDA.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				PROP[nD].TAKEDA.dUnloadStiffCalcExpo = m_edtExponent.GetEditValue();
				PROP[nD].TAKEDA.dUnloadStiffReduFac = m_edtLoop.GetEditValue();
				PROP[nD].TAKEDA.dPinchingRuleFac = m_edtPinch.GetEditValue();
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].TAKEDA.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].TAKEDA.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		case 10: // Normal Bilinear
			{
				PROP[nD].NORBIL.nSymmetryType = m_nSymmetry;
				PROP[nD].NORBIL.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].NORBIL.nPAlphaDelta = m_nAlphaDelta;

				dUser_org = PROP[nD].NORBIL.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].NORBIL.dYieldForce[0];  
					dD1_org = PROP[nD].NORBIL.dYieldDisp2nd[0];
					PROP[nD].NORBIL.dYieldForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].NORBIL.dYieldForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].NORBIL.dUltimateForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].NORBIL.dUltimateForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].NORBIL.dYieldDisp2nd[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].NORBIL.dYieldDisp2nd[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].NORBIL.dYieldDisp3rd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].NORBIL.dYieldDisp3rd[1] = m_edtDisp2Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].NORBIL.dYieldMoment[0];  
					dD1_org = PROP[nD].NORBIL.dYieldRotn2nd[0];
					PROP[nD].NORBIL.dYieldMoment[0] = m_edtCrkPlus.GetEditValue();;
					PROP[nD].NORBIL.dYieldMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].NORBIL.dUltimateMoment[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].NORBIL.dUltimateMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].NORBIL.dYieldRotn2nd[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].NORBIL.dYieldRotn2nd[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].NORBIL.dYieldRotn3rd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].NORBIL.dYieldRotn3rd[1] = m_edtDisp2Mnus.GetEditValue();        
				}
				PROP[nD].NORBIL.dStiffRatio2nd[0] = _tstof(m_str1Plus);
				PROP[nD].NORBIL.dStiffRatio2nd[1] = _tstof(m_str1Mnus);
				PROP[nD].NORBIL.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].NORBIL.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].NORBIL.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		case 11: // Elastic Bilinear    
			{
				PROP[nD].ELABIL.nSymmetryType = m_nSymmetry;
				PROP[nD].ELABIL.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].ELABIL.nPAlphaDelta = m_nAlphaDelta;
				dUser_org = PROP[nD].ELABIL.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].ELABIL.dYieldForce[0];  
					dD1_org = PROP[nD].ELABIL.dYieldDisp2nd[0];
					PROP[nD].ELABIL.dYieldForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].ELABIL.dYieldForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].ELABIL.dUltimateForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].ELABIL.dUltimateForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].ELABIL.dYieldDisp2nd[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].ELABIL.dYieldDisp2nd[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].ELABIL.dYieldDisp3rd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].ELABIL.dYieldDisp3rd[1] = m_edtDisp2Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].ELABIL.dYieldMoment[0];  
					dD1_org = PROP[nD].ELABIL.dYieldRotn2nd[0];
					PROP[nD].ELABIL.dYieldMoment[0] = m_edtCrkPlus.GetEditValue();;
					PROP[nD].ELABIL.dYieldMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].ELABIL.dUltimateMoment[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].ELABIL.dUltimateMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].ELABIL.dYieldRotn2nd[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].ELABIL.dYieldRotn2nd[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].ELABIL.dYieldRotn3rd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].ELABIL.dYieldRotn3rd[1] = m_edtDisp2Mnus.GetEditValue();        
				}
				PROP[nD].ELABIL.dStiffRatio2nd[0] = _tstof(m_str1Plus);
				PROP[nD].ELABIL.dStiffRatio2nd[1] = _tstof(m_str1Mnus);
				PROP[nD].ELABIL.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].ELABIL.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].ELABIL.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		case 12: // Elastic Trilinear    
			{
				PROP[nD].ELATRI.nSymmetryType = m_nSymmetry;
				PROP[nD].ELATRI.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].ELATRI.nPAlphaDelta = m_nAlphaDelta;
				dUser_org = PROP[nD].ELATRI.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].ELATRI.dCrackForce[0];  
					dD1_org = PROP[nD].ELATRI.dYieldDisp1st[0];
					PROP[nD].ELATRI.dCrackForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].ELATRI.dCrackForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].ELATRI.dYieldForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].ELATRI.dYieldForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].ELATRI.dUltimateForce[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].ELATRI.dUltimateForce[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].ELATRI.dYieldDisp1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].ELATRI.dYieldDisp1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].ELATRI.dYieldDisp2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].ELATRI.dYieldDisp2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].ELATRI.dYieldDisp3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].ELATRI.dYieldDisp3rd[1] = m_edtDisp3Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].ELATRI.dCrackMoment[0];  
					dD1_org = PROP[nD].ELATRI.dYieldRotn1st[0];
					PROP[nD].ELATRI.dCrackMoment[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].ELATRI.dCrackMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].ELATRI.dYieldMoment[0] = m_edtYldPlus.GetEditValue();;
					PROP[nD].ELATRI.dYieldMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].ELATRI.dUltimateMoment[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].ELATRI.dUltimateMoment[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].ELATRI.dYieldRotn1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].ELATRI.dYieldRotn1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].ELATRI.dYieldRotn2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].ELATRI.dYieldRotn2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].ELATRI.dYieldRotn3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].ELATRI.dYieldRotn3rd[1] = m_edtDisp3Mnus.GetEditValue();        
				}
				PROP[nD].ELATRI.dStiffRatio1st[0] = _tstof(m_str1Plus);
				PROP[nD].ELATRI.dStiffRatio1st[1] = _tstof(m_str1Mnus);
				PROP[nD].ELATRI.dStiffRatio2nd[0] = _tstof(m_str2Plus);
				PROP[nD].ELATRI.dStiffRatio2nd[1] = _tstof(m_str2Mnus);
				PROP[nD].ELATRI.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].ELATRI.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].ELATRI.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		case 13: // Elastic Tetralinear
			{
				PROP[nD].ELATET.nSymmetryType = m_nSymmetry;
				PROP[nD].ELATET.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].ELATET.nPAlphaDelta = m_nAlphaDelta;
				dUser_org = PROP[nD].ELATET.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].ELATET.dCrackForce[0];  
					dD1_org = PROP[nD].ELATET.dYieldDisp1st[0];
					PROP[nD].ELATET.dCrackForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].ELATET.dCrackForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].ELATET.dYieldForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].ELATET.dYieldForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].ELATET.dUltimateForce[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].ELATET.dUltimateForce[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].ELATET.dFractureForce[0] = m_edtFraPlus.GetEditValue();
					PROP[nD].ELATET.dFractureForce[1] = m_edtFraMnus.GetEditValue();
					PROP[nD].ELATET.dYieldDisp1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].ELATET.dYieldDisp1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].ELATET.dYieldDisp2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].ELATET.dYieldDisp2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].ELATET.dYieldDisp3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].ELATET.dYieldDisp3rd[1] = m_edtDisp3Mnus.GetEditValue();
					PROP[nD].ELATET.dYieldDisp4th[0] = m_edtDisp4Plus.GetEditValue();
					PROP[nD].ELATET.dYieldDisp4th[1] = m_edtDisp4Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].ELATET.dCrackMoment[0];  
					dD1_org = PROP[nD].ELATET.dYieldRotn1st[0];
					PROP[nD].ELATET.dCrackMoment[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].ELATET.dCrackMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].ELATET.dYieldMoment[0] = m_edtYldPlus.GetEditValue();;
					PROP[nD].ELATET.dYieldMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].ELATET.dUltimateMoment[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].ELATET.dUltimateMoment[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].ELATET.dFractureMoment[0] = m_edtFraPlus.GetEditValue();
					PROP[nD].ELATET.dFractureMoment[1] = m_edtFraMnus.GetEditValue();
					PROP[nD].ELATET.dYieldRotn1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].ELATET.dYieldRotn1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].ELATET.dYieldRotn2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].ELATET.dYieldRotn2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].ELATET.dYieldRotn3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].ELATET.dYieldRotn3rd[1] = m_edtDisp3Mnus.GetEditValue();        
					PROP[nD].ELATET.dYieldRotn4th[0] = m_edtDisp4Plus.GetEditValue();
					PROP[nD].ELATET.dYieldRotn4th[1] = m_edtDisp4Mnus.GetEditValue();        
				}
				PROP[nD].ELATET.dStiffRatio1st[0] = _tstof(m_str1Plus);
				PROP[nD].ELATET.dStiffRatio1st[1] = _tstof(m_str1Mnus);
				PROP[nD].ELATET.dStiffRatio2nd[0] = _tstof(m_str2Plus);
				PROP[nD].ELATET.dStiffRatio2nd[1] = _tstof(m_str2Mnus);
				PROP[nD].ELATET.dStiffRatio3rd[0] = _tstof(m_str3Plus);
				PROP[nD].ELATET.dStiffRatio3rd[1] = _tstof(m_str3Mnus);
				PROP[nD].ELATET.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].ELATET.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].ELATET.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		case 14: case 15: // 14:Takeda Tetralinear, 15:Modified Takeda Tetralinear
			{
				PROP[nD].TAKTET.nSymmetryType = m_nSymmetry;
				PROP[nD].TAKTET.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].TAKTET.nPAlphaDelta = m_nAlphaDelta;
				dUser_org = PROP[nD].TAKTET.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].TAKTET.dCrackForce[0];  
					dD1_org = PROP[nD].TAKTET.dYieldDisp1st[0];
					PROP[nD].TAKTET.dCrackForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].TAKTET.dCrackForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].TAKTET.dYieldForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].TAKTET.dYieldForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].TAKTET.dUltimateForce[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].TAKTET.dUltimateForce[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].TAKTET.dFractureForce[0] = m_edtFraPlus.GetEditValue();
					PROP[nD].TAKTET.dFractureForce[1] = m_edtFraMnus.GetEditValue();
					PROP[nD].TAKTET.dYieldDisp1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].TAKTET.dYieldDisp1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].TAKTET.dYieldDisp2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].TAKTET.dYieldDisp2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].TAKTET.dYieldDisp3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].TAKTET.dYieldDisp3rd[1] = m_edtDisp3Mnus.GetEditValue();
					PROP[nD].TAKTET.dYieldDisp4th[0] = m_edtDisp4Plus.GetEditValue();
					PROP[nD].TAKTET.dYieldDisp4th[1] = m_edtDisp4Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].TAKTET.dCrackMoment[0];  
					dD1_org = PROP[nD].TAKTET.dYieldRotn1st[0];
					PROP[nD].TAKTET.dCrackMoment[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].TAKTET.dCrackMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].TAKTET.dYieldMoment[0] = m_edtYldPlus.GetEditValue();;
					PROP[nD].TAKTET.dYieldMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].TAKTET.dUltimateMoment[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].TAKTET.dUltimateMoment[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].TAKTET.dFractureMoment[0] = m_edtFraPlus.GetEditValue();
					PROP[nD].TAKTET.dFractureMoment[1] = m_edtFraMnus.GetEditValue();
					PROP[nD].TAKTET.dYieldRotn1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].TAKTET.dYieldRotn1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].TAKTET.dYieldRotn2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].TAKTET.dYieldRotn2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].TAKTET.dYieldRotn3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].TAKTET.dYieldRotn3rd[1] = m_edtDisp3Mnus.GetEditValue();        
					PROP[nD].TAKTET.dYieldRotn4th[0] = m_edtDisp4Plus.GetEditValue();
					PROP[nD].TAKTET.dYieldRotn4th[1] = m_edtDisp4Mnus.GetEditValue();              
				}
				PROP[nD].TAKTET.dStiffRatio1st[0] = _tstof(m_str1Plus);
				PROP[nD].TAKTET.dStiffRatio1st[1] = _tstof(m_str1Mnus);
				PROP[nD].TAKTET.dStiffRatio2nd[0] = _tstof(m_str2Plus);
				PROP[nD].TAKTET.dStiffRatio2nd[1] = _tstof(m_str2Mnus);
				PROP[nD].TAKTET.dStiffRatio3rd[0] = _tstof(m_str3Plus);
				PROP[nD].TAKTET.dStiffRatio3rd[1] = _tstof(m_str3Mnus);
				PROP[nD].TAKTET.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				PROP[nD].TAKTET.dUnloadStiffCalcExpo = m_edtExponent.GetEditValue();
				PROP[nD].TAKTET.dUnloadStiffReduFac = m_edtLoop.GetEditValue();
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].TAKTET.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].TAKTET.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		case 92: // 92:SRC Takeda Tetralinear 
		{
			PROP[nD].SRCTET.nSymmetryType = m_nSymmetry;
			PROP[nD].SRCTET.nYieldStrengthOpt = m_nUserAuto;
			PROP[nD].SRCTET.nPAlphaDelta = m_nAlphaDelta;
			dUser_org = PROP[nD].SRCTET.dInitStiffness;
			if (m_nDof < 3)
			{
				dP1_org = PROP[nD].SRCTET.dCrackForce[0];
				dD1_org = PROP[nD].SRCTET.dYieldDisp1st[0];
				PROP[nD].SRCTET.dCrackForce[0] = m_edtCrkPlus.GetEditValue();
				PROP[nD].SRCTET.dCrackForce[1] = m_edtCrkMnus.GetEditValue();
				PROP[nD].SRCTET.dYieldForce[0] = m_edtYldPlus.GetEditValue();
				PROP[nD].SRCTET.dYieldForce[1] = m_edtYldMnus.GetEditValue();
				PROP[nD].SRCTET.dUltimateForce[0] = m_edtUltPlus.GetEditValue();
				PROP[nD].SRCTET.dUltimateForce[1] = m_edtUltMnus.GetEditValue();
				PROP[nD].SRCTET.dFractureForce[0] = m_edtFraPlus.GetEditValue();
				PROP[nD].SRCTET.dFractureForce[1] = m_edtFraMnus.GetEditValue();
				PROP[nD].SRCTET.dYieldDisp1st[0] = m_edtDisp1Plus.GetEditValue();
				PROP[nD].SRCTET.dYieldDisp1st[1] = m_edtDisp1Mnus.GetEditValue();
				PROP[nD].SRCTET.dYieldDisp2nd[0] = m_edtDisp2Plus.GetEditValue();
				PROP[nD].SRCTET.dYieldDisp2nd[1] = m_edtDisp2Mnus.GetEditValue();
				PROP[nD].SRCTET.dYieldDisp3rd[0] = m_edtDisp3Plus.GetEditValue();
				PROP[nD].SRCTET.dYieldDisp3rd[1] = m_edtDisp3Mnus.GetEditValue();
				PROP[nD].SRCTET.dYieldDisp4th[0] = m_edtDisp4Plus.GetEditValue();
				PROP[nD].SRCTET.dYieldDisp4th[1] = m_edtDisp4Mnus.GetEditValue();
			}
			else
			{
				dP1_org = PROP[nD].SRCTET.dCrackMoment[0];
				dD1_org = PROP[nD].SRCTET.dYieldRotn1st[0];
				PROP[nD].SRCTET.dCrackMoment[0] = m_edtCrkPlus.GetEditValue();
				PROP[nD].SRCTET.dCrackMoment[1] = m_edtCrkMnus.GetEditValue();
				PROP[nD].SRCTET.dYieldMoment[0] = m_edtYldPlus.GetEditValue();;
				PROP[nD].SRCTET.dYieldMoment[1] = m_edtYldMnus.GetEditValue();
				PROP[nD].SRCTET.dUltimateMoment[0] = m_edtUltPlus.GetEditValue();
				PROP[nD].SRCTET.dUltimateMoment[1] = m_edtUltMnus.GetEditValue();
				PROP[nD].SRCTET.dFractureMoment[0] = m_edtFraPlus.GetEditValue();
				PROP[nD].SRCTET.dFractureMoment[1] = m_edtFraMnus.GetEditValue();
				PROP[nD].SRCTET.dYieldRotn1st[0] = m_edtDisp1Plus.GetEditValue();
				PROP[nD].SRCTET.dYieldRotn1st[1] = m_edtDisp1Mnus.GetEditValue();
				PROP[nD].SRCTET.dYieldRotn2nd[0] = m_edtDisp2Plus.GetEditValue();
				PROP[nD].SRCTET.dYieldRotn2nd[1] = m_edtDisp2Mnus.GetEditValue();
				PROP[nD].SRCTET.dYieldRotn3rd[0] = m_edtDisp3Plus.GetEditValue();
				PROP[nD].SRCTET.dYieldRotn3rd[1] = m_edtDisp3Mnus.GetEditValue();
				PROP[nD].SRCTET.dYieldRotn4th[0] = m_edtDisp4Plus.GetEditValue();
				PROP[nD].SRCTET.dYieldRotn4th[1] = m_edtDisp4Mnus.GetEditValue();
			}
			PROP[nD].SRCTET.dStiffRatio1st[0] = _tstof(m_str1Plus);
			PROP[nD].SRCTET.dStiffRatio1st[1] = _tstof(m_str1Mnus);
			PROP[nD].SRCTET.dStiffRatio2nd[0] = _tstof(m_str2Plus);
			PROP[nD].SRCTET.dStiffRatio2nd[1] = _tstof(m_str2Mnus);
			PROP[nD].SRCTET.dStiffRatio3rd[0] = _tstof(m_str3Plus);
			PROP[nD].SRCTET.dStiffRatio3rd[1] = _tstof(m_str3Mnus);
			PROP[nD].SRCTET.nInitStiffType = m_nStiffType;
			GetInitialStiffness4HingeType(nD, PROP);
			PROP[nD].SRCTET.dUnloadStiffCalcExpo = m_edtExponent.GetEditValue();

			for (int i = 0; i < 2; i++) for (int j = 0; j < 5; j++)
				PROP[nD].SRCTET.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
			PROP[nD].SRCTET.nDeformDefineType = m_nDeformDefineType;
			break;
		}
			// 30:SLIP Bilinear,  31:SLIP Bilinear/Tensile,  32:SLIP Bilinear/Compressive
		case 30: case 31: case 32:
			{
				PROP[nD].SLIP.nSymmetryType = m_nSymmetry;
				PROP[nD].SLIP.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].SLIP.nPAlphaDelta = m_nAlphaDelta;
				dUser_org = PROP[nD].KINEMA.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].SLIP.dYieldForce[0];  
					dD1_org = PROP[nD].SLIP.dYieldDisp2nd[0];
					PROP[nD].SLIP.dYieldForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].SLIP.dYieldForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].SLIP.dUltimateForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].SLIP.dUltimateForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].SLIP.dYieldDisp2nd[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].SLIP.dYieldDisp2nd[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].SLIP.dYieldDisp3rd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].SLIP.dYieldDisp3rd[1] = m_edtDisp2Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].SLIP.dYieldMoment[0];  
					dD1_org = PROP[nD].SLIP.dYieldRotn2nd[0];
					PROP[nD].SLIP.dYieldMoment[0] = m_edtCrkPlus.GetEditValue();;
					PROP[nD].SLIP.dYieldMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].SLIP.dUltimateMoment[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].SLIP.dUltimateMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].SLIP.dYieldRotn2nd[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].SLIP.dYieldRotn2nd[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].SLIP.dYieldRotn3rd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].SLIP.dYieldRotn3rd[1] = m_edtDisp2Mnus.GetEditValue();        
				}
				PROP[nD].SLIP.dStiffRatio2nd[0] = _tstof(m_str1Plus);
				PROP[nD].SLIP.dStiffRatio2nd[1] = _tstof(m_str1Mnus);
				PROP[nD].SLIP.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				PROP[nD].SLIP.dInitGapPositive = m_edtSlipT.GetEditValue();
				PROP[nD].SLIP.dInitGapNegative = m_edtSlipC.GetEditValue();
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].SLIP.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].SLIP.nDeformDefineType = m_nDeformDefineType;
				break;
			}
			// 33:SLIP Trilinear, 34:SLIP Trilinear/Tensile, 35:SLIP Trilinear/Compressive
		case 33: case 34: case 35:
			{
				PROP[nD].SLIP.nSymmetryType = m_nSymmetry;
				PROP[nD].SLIP.nYieldStrengthOpt = m_nUserAuto;
				PROP[nD].SLIP.nPAlphaDelta = m_nAlphaDelta;
				dUser_org = PROP[nD].SLIP.dInitStiffness;
				if(m_nDof<3)
				{
					dP1_org = PROP[nD].SLIP.dCrackForce[0];  
					dD1_org = PROP[nD].SLIP.dYieldDisp1st[0];
					PROP[nD].SLIP.dCrackForce[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].SLIP.dCrackForce[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].SLIP.dYieldForce[0] = m_edtYldPlus.GetEditValue();
					PROP[nD].SLIP.dYieldForce[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].SLIP.dUltimateForce[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].SLIP.dUltimateForce[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].SLIP.dYieldDisp1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].SLIP.dYieldDisp1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].SLIP.dYieldDisp2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].SLIP.dYieldDisp2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].SLIP.dYieldDisp3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].SLIP.dYieldDisp3rd[1] = m_edtDisp3Mnus.GetEditValue();
				}
				else
				{
					dP1_org = PROP[nD].SLIP.dCrackMoment[0];  
					dD1_org = PROP[nD].SLIP.dYieldRotn1st[0];
					PROP[nD].SLIP.dCrackMoment[0] = m_edtCrkPlus.GetEditValue();
					PROP[nD].SLIP.dCrackMoment[1] = m_edtCrkMnus.GetEditValue();
					PROP[nD].SLIP.dYieldMoment[0] = m_edtYldPlus.GetEditValue();;
					PROP[nD].SLIP.dYieldMoment[1] = m_edtYldMnus.GetEditValue();
					PROP[nD].SLIP.dUltimateMoment[0] = m_edtUltPlus.GetEditValue();
					PROP[nD].SLIP.dUltimateMoment[1] = m_edtUltMnus.GetEditValue();
					PROP[nD].SLIP.dYieldRotn1st[0] = m_edtDisp1Plus.GetEditValue();
					PROP[nD].SLIP.dYieldRotn1st[1] = m_edtDisp1Mnus.GetEditValue();
					PROP[nD].SLIP.dYieldRotn2nd[0] = m_edtDisp2Plus.GetEditValue();
					PROP[nD].SLIP.dYieldRotn2nd[1] = m_edtDisp2Mnus.GetEditValue();
					PROP[nD].SLIP.dYieldRotn3rd[0] = m_edtDisp3Plus.GetEditValue();
					PROP[nD].SLIP.dYieldRotn3rd[1] = m_edtDisp3Mnus.GetEditValue();        
				}
				PROP[nD].SLIP.dStiffRatio1st[0] = _tstof(m_str1Plus);
				PROP[nD].SLIP.dStiffRatio1st[1] = _tstof(m_str1Mnus);
				PROP[nD].SLIP.dStiffRatio2nd[0] = _tstof(m_str2Plus);
				PROP[nD].SLIP.dStiffRatio2nd[1] = _tstof(m_str2Mnus);
				PROP[nD].SLIP.nInitStiffType = m_nStiffType;
				GetInitialStiffness4HingeType(nD, PROP);
				PROP[nD].SLIP.dInitGapPositive = m_edtSlipT.GetEditValue();
				PROP[nD].SLIP.dInitGapNegative = m_edtSlipC.GetEditValue();
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					PROP[nD].SLIP.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				PROP[nD].SLIP.nDeformDefineType = m_nDeformDefineType;
				break;
			}
		}

		BOOL bWanningMsgOut=FALSE;
		double dCompTol = 1.0e-10; // MQC-5057 : gykim
		if(nCompareType==1)
		{
			if(CCompFunc::CompRealTol(dP1_org, dP1_tgt, dCompTol) != 0 || CCompFunc::CompRealTol(dD1_org, dD1_tgt, dCompTol) != 0)
			{
				bWanningMsgOut=TRUE;
			}
		}
		else if(nCompareType==0)
		{
			if(CCompFunc::CompRealTol(dUser_org, dUser_tgt, dCompTol) != 0) bWanningMsgOut=TRUE; 
		}

#if defined(_MGEN_JP) 
		if(bWanningMsgOut)
		{
			GSaveHistoryFormatNF(_LS(IDS_CMD_WANNING_NONLINEAR_INIT_STIFFNESS));
		}
#endif
		return TRUE;
}

void CIehpPropDlg::OnDeformationIndex()
{	
	int nTabID = m_Tab.GetCurSel();

	int nTabType = 0;
	if (nTabID == D_TAB_DEFALUT)
	{
		if (m_nTabStyle == J_LOCATION)
			nTabType = 1;
		else
			nTabType = 0;
	}
	else if (nTabID == D_TAB_SUB)
	{
		nTabType = 1;
	}

	if (nTabType == 0)
	{
		CIehpPropDeformation DeformationDlg(m_nSymmetry,m_nModelType,m_nDeformDefineType,m_nDof,m_Data.AllProp.PROP);
		if(DeformationDlg.DoModal() == IDOK)
		{
			m_nDeformDefineType = DeformationDlg.GetDeformDefineType();
		}
	}
	else if(nTabType == 1)
	{
		CIehpPropDeformation DeformationDlg(m_nSymmetry,m_nModelType,m_nDeformDefineType,m_nDof,m_Data.AllSubProp.PROP);
		if(DeformationDlg.DoModal() == IDOK)
		{
			m_nDeformDefineType = DeformationDlg.GetDeformDefineType();
		}
	}
}

void CIehpPropDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 다음탭으로 이동하기 직전 처리하는 부분
	int nTabID = m_Tab.GetCurSel();

	if(nTabID == D_TAB_DEFALUT)	
	{
		if(!TabDlg2Data(nTabID,m_Data.AllProp.PROP)) 
		{
			*pResult = 0;
		}
	}
	else if(nTabID == D_TAB_SUB)
	{
		if(!TabDlg2Data(nTabID,m_Data.AllSubProp.PROP)) 
		{
			*pResult = 1;
		}
	}

	int nDof = m_nDof;
	if (nDof > 5) nDof = nDof - 3;  // Interaction Type이 None이 아닌 경우
	m_pDoc->m_pAttrCtrl->CompareUserAutoData(m_nModelType, m_nUserAuto, m_nAlphaDelta, m_Data.AllProp.PROP[nDof], m_Data.AllSubProp.PROP[nDof]);
}

void CIehpPropDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 이동한 탭을 보여주기 전에 처리하는 부분
	Data2Dlg();
	UpdateGraph();
	CtrlManager();
	OnSymmetryTypeRdo();  

	*pResult = 0;  
}

void CIehpPropDlg::OnValueTypeSymmetry()
{
	ChangeTabCtrl(m_nTabStyle);
}

void CIehpPropDlg::SetControlStateData(BOOL bExistIJData, int nOptType)
{
	m_bExistIJData = bExistIJData;
	m_nUserAuto = nOptType;
}

BOOL CIehpPropDlg::CompareProp(T_IEHP_PROP MainProp, T_IEHP_PROP SubProp)
{
	switch(m_nModelType)
	{
	case 0: case 7: // 0:Kinematic Hardening, 7:비선형탄성형(대칭)
		if(memcmp(&MainProp.KINEMA, &SubProp.KINEMA, sizeof(MainProp.KINEMA))!=0) return FALSE;		
		break;
	case 1: // Origin-oriented
		if(memcmp(&MainProp.ORIGIN, &SubProp.ORIGIN, sizeof(MainProp.ORIGIN))!=0) return FALSE;		
		break;
	case 2: // Peak-oriented
		if(memcmp(&MainProp.PEAK, &SubProp.PEAK, sizeof(MainProp.PEAK))!=0) return FALSE;		
		break;
	case 3: // Clough
		if(memcmp(&MainProp.CLOUGH, &SubProp.CLOUGH, sizeof(MainProp.CLOUGH))!=0) return FALSE;		
		break;
	case 4: // Degrading Trilinear
		if(memcmp(&MainProp.DEGRAD, &SubProp.DEGRAD, sizeof(MainProp.DEGRAD))!=0) return FALSE;		
		break;
	case 5: case 6: case 8: case 9: case D_IEHP_TAKS: // 5: Takeda, 6: Modified Takeda, 8: 비대칭역향형, 9:비선형탄성형(비대칭), 74: Takeda Slip
		if(memcmp(&MainProp.TAKEDA, &SubProp.TAKEDA, sizeof(MainProp.TAKEDA))!=0) return FALSE;		
		break;
	case 10: // Normal Bilinear
		if(memcmp(&MainProp.NORBIL, &SubProp.NORBIL, sizeof(MainProp.NORBIL))!=0) return FALSE;		   
		break;
	case 11: // Elastic Bilinear    
		if(memcmp(&MainProp.ELABIL, &SubProp.ELABIL, sizeof(MainProp.ELABIL))!=0) return FALSE;		
		break;
	case 12: // Elastic Trilinear    
		if(memcmp(&MainProp.ELATRI, &SubProp.ELATRI, sizeof(MainProp.ELATRI))!=0) return FALSE;		
		break;
	case 13: // Elastic Tetralinear
		if(memcmp(&MainProp.ELATET, &SubProp.ELATET, sizeof(MainProp.ELATET))!=0) return FALSE;		
		break;
	case 14: case 15: // 14:Takeda Tetralinear, 15:Modified Takeda Tetralinear
		if(memcmp(&MainProp.TAKTET, &SubProp.TAKTET, sizeof(MainProp.TAKTET))!=0) return FALSE;		
		break;
	case 92: // 92:SRC Tetralinear
		if (memcmp(&MainProp.SRCTET, &SubProp.SRCTET, sizeof(MainProp.SRCTET)) != 0) return FALSE;
		break;
		// 30:SLIP Bilinear,  31:SLIP Bilinear/Tensile,  32:SLIP Bilinear/Compressive
	case 30: case 31: case 32: case 33: case 34: case 35:
		if(memcmp(&MainProp.SLIP, &SubProp.SLIP, sizeof(MainProp.SLIP))!=0) return FALSE;		
		break;
	}

	return TRUE;
}

void CIehpPropDlg::BackupDisplayData()
{
	UpdateData(TRUE);
	if(m_aBackupArr.GetSize() != 0)
	{
		ASSERT(FALSE);
		return;
	}

	m_aBackupArr.RemoveAll();

	m_aBackupArr.Add(m_edtDisp1Plus.GetEditValue());
	m_aBackupArr.Add(m_edtDisp1Mnus.GetEditValue());
	m_aBackupArr.Add(m_edtDisp2Plus.GetEditValue());
	m_aBackupArr.Add(m_edtDisp2Mnus.GetEditValue());
	m_aBackupArr.Add(m_edtDisp3Plus.GetEditValue());
	m_aBackupArr.Add(m_edtDisp3Mnus.GetEditValue());
	m_aBackupArr.Add(m_edtDisp4Plus.GetEditValue());
	m_aBackupArr.Add(m_edtDisp4Mnus.GetEditValue());
	m_aBackupArr.Add(m_edtFraPlus.GetEditValue());
	m_aBackupArr.Add(m_edtFraMnus.GetEditValue());
	m_aBackupArr.Add(m_edtUltPlus.GetEditValue());
	m_aBackupArr.Add(m_edtUltMnus.GetEditValue());
	m_aBackupArr.Add(m_edt3Plus.GetEditValue());
	m_aBackupArr.Add(m_edt3Mnus.GetEditValue());
	m_aBackupArr.Add(m_edtCrkPlus.GetEditValue());
	m_aBackupArr.Add(m_edtCrkMnus.GetEditValue());
	m_aBackupArr.Add(m_edtStiffUser.GetEditValue());
	m_aBackupArr.Add(m_edtYldPlus.GetEditValue());
	m_aBackupArr.Add(m_edtYldMnus.GetEditValue());
	m_aBackupArr.Add(m_edtExponent.GetEditValue());
	m_aBackupArr.Add(m_edtLoop.GetEditValue());
	m_aBackupArr.Add(m_edtSlipT.GetEditValue());
	m_aBackupArr.Add(m_edtSlipC.GetEditValue());

	m_edtDisp1Plus.SetEditUnit(0);
	m_edtDisp1Mnus.SetEditUnit(0);
	m_edtDisp2Plus.SetEditUnit(0);
	m_edtDisp2Mnus.SetEditUnit(0);
	m_edtDisp3Plus.SetEditUnit(0);
	m_edtDisp3Mnus.SetEditUnit(0);
	m_edtDisp4Plus.SetEditUnit(0);
	m_edtDisp4Mnus.SetEditUnit(0);
	m_edtFraPlus.SetEditUnit(0);
	m_edtFraMnus.SetEditUnit(0);
	m_edtUltPlus.SetEditUnit(0);
	m_edtUltMnus.SetEditUnit(0);
	m_edt3Plus.SetEditUnit(0);
	m_edt3Mnus.SetEditUnit(0);
	m_edtCrkPlus.SetEditUnit(0);
	m_edtCrkMnus.SetEditUnit(0);
	m_edtStiffUser.SetEditUnit(0);
	m_edtYldPlus.SetEditUnit(0);
	m_edtYldMnus.SetEditUnit(0);
	m_edtExponent.SetEditUnit(0);
	m_edtLoop.SetEditUnit(0);
	m_edtPinch.SetEditUnit(0);
	m_edtSlipT.SetEditUnit(0);
	m_edtSlipC.SetEditUnit(0);

	UpdateData(FALSE);
}

void CIehpPropDlg::LoadDisplayBackupData()
{
	if(m_aBackupArr.GetSize() == 0) 
	{
		ASSERT(FALSE);
		return;
	}
	UpdateData(TRUE);
	
	m_edtDisp1Plus.SetEditUnit(m_aBackupArr.GetAt(0));
	m_edtDisp1Mnus.SetEditUnit(m_aBackupArr.GetAt(1));
	m_edtDisp2Plus.SetEditUnit(m_aBackupArr.GetAt(2));
	m_edtDisp2Mnus.SetEditUnit(m_aBackupArr.GetAt(3));
	m_edtDisp3Plus.SetEditUnit(m_aBackupArr.GetAt(4));
	m_edtDisp3Mnus.SetEditUnit(m_aBackupArr.GetAt(5));
	m_edtDisp4Plus.SetEditUnit(m_aBackupArr.GetAt(6));
	m_edtDisp4Mnus.SetEditUnit(m_aBackupArr.GetAt(7));
	m_edtFraPlus.SetEditUnit(m_aBackupArr.GetAt(8));
	m_edtFraMnus.SetEditUnit(m_aBackupArr.GetAt(9));
	m_edtUltPlus.SetEditUnit(m_aBackupArr.GetAt(10));
	m_edtUltMnus.SetEditUnit(m_aBackupArr.GetAt(11));
	m_edt3Plus.SetEditUnit(m_aBackupArr.GetAt(12));
	m_edt3Mnus.SetEditUnit(m_aBackupArr.GetAt(13));
	m_edtCrkPlus.SetEditUnit(m_aBackupArr.GetAt(14));
	m_edtCrkMnus.SetEditUnit(m_aBackupArr.GetAt(15));
	m_edtStiffUser.SetEditUnit(m_aBackupArr.GetAt(16));
	m_edtYldPlus.SetEditUnit(m_aBackupArr.GetAt(17));
	m_edtYldMnus.SetEditUnit(m_aBackupArr.GetAt(18));
	m_edtExponent.SetEditUnit(m_aBackupArr.GetAt(19));
	m_edtLoop.SetEditUnit(m_aBackupArr.GetAt(20));
	m_edtSlipT.SetEditUnit(m_aBackupArr.GetAt(21));
	m_edtSlipC.SetEditUnit(m_aBackupArr.GetAt(22));

	m_aBackupArr.RemoveAll();
	UpdateData(FALSE);
}

void CIehpPropDlg::SaveDeformCapacity(int nD, T_IEHP_PROP* PROP)
{
	if(!PROP) { ASSERT(0); return; }

	switch(m_nModelType)
	{
	case 0: case 7: // 0:Kinematic Hardening, 7:비선형탄성형(대칭)
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].KINEMA.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].KINEMA.nDeformDefineType;
		break;
	case 1:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].ORIGIN.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].ORIGIN.nDeformDefineType;
		break;
	case 2:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].PEAK.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].PEAK.nDeformDefineType;
		break;
	case 3:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].CLOUGH.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].CLOUGH.nDeformDefineType;
		break;
	case 4:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].DEGRAD.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].DEGRAD.nDeformDefineType;
		break;
	case 5: case 6: case 8: case 9: // 5: Takeda, 6: Modified Takeda, 8: 비대칭역향형, 9:비선형탄성형(비대칭)
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].TAKEDA.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].TAKEDA.nDeformDefineType;
		break;
	case 10:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].NORBIL.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].NORBIL.nDeformDefineType;
		break;
	case 11:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].ELABIL.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].ELABIL.nDeformDefineType;
		break;
	case 12:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].ELATRI.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].ELATRI.nDeformDefineType;
		break;
	case 13:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].ELATET.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].ELATET.nDeformDefineType;
		break;
	case 14: case 15: // 14:Takeda Tetralinear, 15:Modified Takeda Tetralinear
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].TAKTET.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].TAKTET.nDeformDefineType;
		break;
	case 92: // 92:SRC Takeda Tetralinear 
		for (int i = 0; i < 2; i++) for (int j = 0; j < 5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].SRCTET.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].SRCTET.nDeformDefineType;
		break;
	case 30: case 31: case 32:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].SLIP.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].SLIP.nDeformDefineType;
		break;
	case 33: case 34: case 35:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_dDeformCapacity[i][j] = PROP[nD].SLIP.dDeformCapacity[i][j];
		m_nDeformDefineType = PROP[nD].SLIP.nDeformDefineType;
		break;
	}
}

void CIehpPropDlg::SetInitialStiffness4HingeType(const int nD, T_IEHP_PROP* PROP)
{
	double dInitialStiffness = 0.0;
	double dInitialStiffnessDist = 0.0;

	switch (m_nModelType)
	{
	case D_IEHP_KINE:
	case D_IEHP_NELS:
		{
			dInitialStiffness = PROP[nD].KINEMA.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].KINEMA.dInitStiffnessDist;
		}
		break;
	case D_IEHP_ORIG:
		{
			dInitialStiffness = PROP[nD].ORIGIN.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].ORIGIN.dInitStiffnessDist;
		}
		break;
	case D_IEHP_PICK:
		{
			dInitialStiffness = PROP[nD].PEAK.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].PEAK.dInitStiffnessDist;
		}
		break;
	case D_IEHP_CLOU:
		{
			dInitialStiffness = PROP[nD].CLOUGH.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].CLOUGH.dInitStiffnessDist;
		}
		break;
	case D_IEHP_DEGR:
		{
			dInitialStiffness = PROP[nD].DEGRAD.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].DEGRAD.dInitStiffnessDist;
		}
		break;
	case D_IEHP_TAKE:
	case D_IEHP_MTAK:
	case D_IEHP_ASYM:
	case D_IEHP_NELA:
	case D_IEHP_TAKS:
		{
			dInitialStiffness = PROP[nD].TAKEDA.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].TAKEDA.dInitStiffnessDist;
		}
		break;
	case D_IEHP_NBIL:
		{
			dInitialStiffness = PROP[nD].NORBIL.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].NORBIL.dInitStiffnessDist;
		}
		break;
	case D_IEHP_EBIL:
		{
			dInitialStiffness = PROP[nD].ELABIL.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].ELABIL.dInitStiffnessDist;
		}
		break;
	case D_IEHP_ETRI:
		{
			dInitialStiffness = PROP[nD].ELATRI.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].ELATRI.dInitStiffnessDist;
		}
		break;
	case D_IEHP_ETET:
		{
			dInitialStiffness = PROP[nD].ELATET.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].ELATET.dInitStiffnessDist;
		}
		break;
	case D_IEHP_TTET:
	case D_IEHP_MTTE:
		{
			dInitialStiffness = PROP[nD].TAKTET.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].TAKTET.dInitStiffnessDist;
		}
		break;
	case D_IEHP_SRCT:
	{
		dInitialStiffness = PROP[nD].SRCTET.dInitStiffness;
		dInitialStiffnessDist = PROP[nD].SRCTET.dInitStiffnessDist;
	}
		break;
	case D_IEHP_SLPB:
	case D_IEHP_SLBT:
	case D_IEHP_SLBC:
	case D_IEHP_SLPT:
	case D_IEHP_SLTT:
	case D_IEHP_SLTC:
		{
			dInitialStiffness = PROP[nD].SLIP.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].SLIP.dInitStiffnessDist;
		}
		break;
	case D_IEHP_AXIAL:
		{
			dInitialStiffness = PROP[nD].AXIAL.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].AXIAL.dInitStiffnessDist;
		}
		break;
	case D_IEHP_BUCK:
		{
			dInitialStiffness = PROP[nD].BUCKL.dInitStiffness;
			dInitialStiffnessDist = PROP[nD].BUCKL.dInitStiffnessDist;
		}
		break;
	case D_IEHP_ORGT:
	case D_IEHP_ORGP:
	case D_IEHP_FBIL:
	case D_IEHP_FINF:
	case D_IEHP_FTRI:
	case D_IEHP_LRBB:
	case D_IEHP_LRBT:
	case D_IEHP_DMPR:
	case D_IEHP_LRBH16:
	case D_IEHP_LRBH30:
	case D_IEHP_DMPRH16:
	case D_IEHP_DMPRH30:
	case D_IEHP_RO:
	case D_IEHP_HD:
	case D_IEHP_MLEL:
	case D_IEHP_MLKI:
	case D_IEHP_MLTA:
	case D_IEHP_MLPI:
	case D_IEHP_PMMC:
	default:
		{
			ASSERT(0);
		}
		break;
	}

	switch (m_nHingeType)
	{
	case D_IEHP_HTYP_BEAM_LUMP:
	case D_IEHP_HTYP_SPRING:
	case D_IEHP_HTYP_TRUSS:
	case D_IEHP_HTYP_WALL:
	case D_IEHP_HTYP_POINTSPRING:
		{
			m_edtStiffUser.SetEditUnit(dInitialStiffness);
		}
		break;
	case D_IEHP_HTYP_BEAM_DIST:
		{
			m_edtStiffUser.SetEditUnit(dInitialStiffnessDist);
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}

	return;
}

void CIehpPropDlg::GetInitialStiffness4HingeType(const int nD, T_IEHP_PROP* PROP)
{
	bool bDistributedModel = false;
	double dInitialStiffness = 0.0;
	double dInitialStiffnessDist = 0.0;

	switch (m_nHingeType)
	{
	case D_IEHP_HTYP_BEAM_LUMP:
	case D_IEHP_HTYP_SPRING:
	case D_IEHP_HTYP_TRUSS:
	case D_IEHP_HTYP_WALL:
	case D_IEHP_HTYP_POINTSPRING:
		{
			dInitialStiffness = m_edtStiffUser.GetEditValue();
		}
		break;
	case D_IEHP_HTYP_BEAM_DIST:
		{
			dInitialStiffnessDist = m_edtStiffUser.GetEditValue();
			bDistributedModel = true;
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}

	switch (m_nModelType)
	{
	case D_IEHP_KINE:
	case D_IEHP_NELS:
		{
			if (!bDistributedModel) PROP[nD].KINEMA.dInitStiffness = dInitialStiffness;
			else					PROP[nD].KINEMA.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_ORIG:
		{
			if (!bDistributedModel) PROP[nD].ORIGIN.dInitStiffness = dInitialStiffness;
			else					PROP[nD].ORIGIN.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_PICK:
		{
			if (!bDistributedModel) PROP[nD].PEAK.dInitStiffness = dInitialStiffness;
			else					PROP[nD].PEAK.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_CLOU:
		{
			if (!bDistributedModel) PROP[nD].CLOUGH.dInitStiffness = dInitialStiffness;
			else					PROP[nD].CLOUGH.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_DEGR:
		{
			if (!bDistributedModel) PROP[nD].DEGRAD.dInitStiffness = dInitialStiffness;
			else					PROP[nD].DEGRAD.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_TAKE:
	case D_IEHP_MTAK:
	case D_IEHP_ASYM:
	case D_IEHP_NELA:
	case D_IEHP_TAKS:
		{
			if (!bDistributedModel) PROP[nD].TAKEDA.dInitStiffness = dInitialStiffness;
			else					PROP[nD].TAKEDA.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_NBIL:
		{
			if (!bDistributedModel) PROP[nD].NORBIL.dInitStiffness = dInitialStiffness;
			else					PROP[nD].NORBIL.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_EBIL:
		{
			if (!bDistributedModel) PROP[nD].ELABIL.dInitStiffness = dInitialStiffness;
			else					PROP[nD].ELABIL.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_ETRI:
		{
			if (!bDistributedModel) PROP[nD].ELATRI.dInitStiffness = dInitialStiffness;
			else					PROP[nD].ELATRI.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_ETET:
		{
			if (!bDistributedModel) PROP[nD].ELATET.dInitStiffness = dInitialStiffness;
			else					PROP[nD].ELATET.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_TTET:
	case D_IEHP_MTTE:
		{
			if (!bDistributedModel) PROP[nD].TAKTET.dInitStiffness = dInitialStiffness;
			else					PROP[nD].TAKTET.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_SRCT:
		{
			if (!bDistributedModel) PROP[nD].SRCTET.dInitStiffness = dInitialStiffness;
			else					PROP[nD].SRCTET.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_SLPB:
	case D_IEHP_SLBT:
	case D_IEHP_SLBC:
	case D_IEHP_SLPT:
	case D_IEHP_SLTT:
	case D_IEHP_SLTC:
		{
			if (!bDistributedModel) PROP[nD].SLIP.dInitStiffness = dInitialStiffness;
			else					PROP[nD].SLIP.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_AXIAL:
		{
			if (!bDistributedModel) PROP[nD].AXIAL.dInitStiffness = dInitialStiffness;
			else					PROP[nD].AXIAL.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_BUCK:
		{
			if (!bDistributedModel) PROP[nD].BUCKL.dInitStiffness = dInitialStiffness;
			else					PROP[nD].BUCKL.dInitStiffnessDist = dInitialStiffnessDist;
		}
		break;
	case D_IEHP_ORGT:
	case D_IEHP_ORGP:
	case D_IEHP_FBIL:
	case D_IEHP_FINF:
	case D_IEHP_FTRI:
	case D_IEHP_LRBB:
	case D_IEHP_LRBT:
	case D_IEHP_DMPR:
	case D_IEHP_LRBH16:
	case D_IEHP_LRBH30:
	case D_IEHP_DMPRH16:
	case D_IEHP_DMPRH30:
	case D_IEHP_RO:
	case D_IEHP_HD:
	case D_IEHP_MLEL:
	case D_IEHP_MLKI:
	case D_IEHP_MLTA:
	case D_IEHP_MLPI:
	case D_IEHP_PMMC:
	default:
		{
			ASSERT(0);
		}
		break;
	}

	return;
}
