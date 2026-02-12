// SeisItemDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemDlg2.h"
#include <math.h>

#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"
#include "SeisProfieDlg.h"
#include "ETC_KSPeriodCalc.h"

#include "WindSeisAddLoadDlg.h"
#include "SeisJISCalcDlg.h"
#include "SeisIBCCalcDlg.h"
#include "SeisEuroCalcDlg.h"
#include "SeisChinaCalcDlg.h"
#include "SeisNBC95CalcDlg.h"
#include "SeisIS02PeriodCalcDlg.h"
#include "SeisTaiwan86PeriodDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemDlg2 dialog

CSeisItemDlg2::CSeisItemDlg2(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisItemDlg2::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	m_Key = 0;

	//{{AFX_DATA_INIT(CSeisItemDlg2)
	m_wndChAlpha1 = FALSE;
	m_wndChER = -1;
	m_dHn = 0.0;
	m_dBx = 1.0;
	m_dBy = 1.0;
	m_nTaiwan86Period = -1;
	//}}AFX_DATA_INIT
	m_aCtrlEtc.Add(IDC_CMD_FRAME1); // KOREA
	m_aCtrlEtc.Add(IDC_CMD_SF_TITLE);
	m_aCtrlEtc.Add(IDC_CMD_SF_EDIT);
	m_aCtrlEtc.Add(IDC_CMD_SF_COMBO);
	m_aCtrlEtc.Add(IDC_CMD_ZF_TITLE);
	m_aCtrlEtc.Add(IDC_CMD_ZF_EDIT);
	m_aCtrlEtc.Add(IDC_CMD_ZF_COMBO);
	m_aCtrlEtc.Add(IDC_CMD_IF_TITLE);
	m_aCtrlEtc.Add(IDC_CMD_IF_COMBO);
	m_aCtrlEtc.Add(IDC_CMD_FRAME2);
	m_aCtrlEtc.Add(IDC_CMD_XDIR_TITLE);
	m_aCtrlEtc.Add(IDC_CMD_YDIR_TITLE);
	m_aCtrlEtc.Add(IDC_CMD_PERIOD_A_TITLE);
	m_aCtrlEtc.Add(IDC_CMD_PERIOD_A_X_EDIT);
	m_aCtrlEtc.Add(IDC_CMD_PERIOD_A_Y_EDIT);
	m_aCtrlEtc.Add(IDC_CMD_PERIOD_C_TITLE);
	m_aCtrlEtc.Add(IDC_CMD_PERIOD_C_X_EDIT);
	m_aCtrlEtc.Add(IDC_CMD_PERIOD_C_Y_EDIT);
	m_aCtrlEtc.Add(IDC_CMD_RMF_TITLE);
	m_aCtrlEtc.Add(IDC_CMD_RMF_X_EDIT);
	m_aCtrlEtc.Add(IDC_CMD_RMF_Y_EDIT);
	m_aCtrlEtc.Add(IDC_CMD_BTN_CALC_PERIOD);

	m_aCtrlUbc97.Add(IDC_CMD_FRAME3);
	m_aCtrlUbc97.Add(IDC_CMD_ZF_TITLE2);
	m_aCtrlUbc97.Add(IDC_CMD_ZF_COMBO2);
	m_aCtrlUbc97.Add(IDC_CMD_IF_TITLE2);
	m_aCtrlUbc97.Add(IDC_CMD_IF_COMBO2);
	m_aCtrlUbc97.Add(IDC_CMD_SPT_TITLE2);
	m_aCtrlUbc97.Add(IDC_CMD_SPT_COMBO2);
	m_aCtrlUbc97.Add(IDC_CMD_SST_TITLE2);
	m_aCtrlUbc97.Add(IDC_CMD_SST_COMBO2);
	m_aCtrlUbc97.Add(IDC_CMD_CD_TITLE2);
	m_aCtrlUbc97.Add(IDC_CMD_CD_EDIT2);
	m_aCtrlUbc97.Add(IDC_CMD_CD_UNIT2);
	m_aCtrlUbc97.Add(IDC_CMD_FRAME4);
	m_aCtrlUbc97.Add(IDC_CMD_XDIR_TITLE2);
	m_aCtrlUbc97.Add(IDC_CMD_YDIR_TITLE2);
	m_aCtrlUbc97.Add(IDC_CMD_PERIOD_TITLE2);
	m_aCtrlUbc97.Add(IDC_CMD_PERIOD_X_EDIT2);
	m_aCtrlUbc97.Add(IDC_CMD_PERIOD_Y_EDIT2);
	m_aCtrlUbc97.Add(IDC_CMD_RMF_TITLE2);
	m_aCtrlUbc97.Add(IDC_CMD_RMF_X_EDIT2);
	m_aCtrlUbc97.Add(IDC_CMD_RMF_Y_EDIT2);

	m_aCtrlEccenX.Add(IDC_CMD_XDIR_POS);
	m_aCtrlEccenX.Add(IDC_CMD_XDIR_NEG);
	m_aCtrlEccenX.Add(IDC_CMD_XDIR_NONE);

	m_aCtrlEccenY.Add(IDC_CMD_YDIR_POS);
	m_aCtrlEccenY.Add(IDC_CMD_YDIR_NEG);
	m_aCtrlEccenY.Add(IDC_CMD_YDIR_NONE);

	m_aCtrlAtcEdit.Add(IDC_CMD_SF_EDIT); // For ATC3-06
	m_aCtrlAtcEdit.Add(IDC_CMD_ZF_EDIT); // For ATC3-06

	m_aCtrlSfZf.Add(IDC_CMD_SF_COMBO); // UBC 1991
	m_aCtrlSfZf.Add(IDC_CMD_ZF_COMBO); // UBC 1991

	m_aCtrlZone4.Add(IDC_CMD_SST_COMBO2); // UBC 1991
	m_aCtrlZone4.Add(IDC_CMD_CD_EDIT2); // UBC 1991

	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_TITLE4);
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_TITLE2);
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_TITLE3);
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_TITLE1);
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_TITLE5);
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_TITLE6); // 필요없음
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_TITLE7);
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_TITLE8); // 필요없음
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_TITLE9);
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_TITLE10);
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_ZONE_EDIT);
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_PERIOD_COBX);
	m_aCtrlJapan.Add(IDC_CMD_SLP_JPN_SHEAR_FACTOR);
	m_aCtrlJapan.Add(IDC_CMD_PERIOD_A_X_EDIT2); // 필요없음
	m_aCtrlJapan.Add(IDC_CMD_PERIOD_A_Y_EDIT2); // 필요없음
	m_aCtrlJapan.Add(IDC_CMD_PERIOD_C_X_EDIT2);
	m_aCtrlJapan.Add(IDC_CMD_PERIOD_C_Y_EDIT2);
	m_aCtrlJapan.Add(IDC_CMD_BTN_CALC_PERIOD2);
	m_aCtrlJapan.Add(IDC_CMD_RMF_X_EDIT3); // 필요없음
	m_aCtrlJapan.Add(IDC_CMD_RMF_Y_EDIT3); // 필요없음

	m_aCtrlIbc.Add(IDC_CMD_IBC_FRAME1);
	m_aCtrlIbc.Add(IDC_CMD_IBC_FRAME2);
	m_aCtrlIbc.Add(IDC_CMD_IBC_TITLE1);
	m_aCtrlIbc.Add(IDC_CMD_IBC_TITLE2);
	m_aCtrlIbc.Add(IDC_CMD_IBC_TITLE3);
	m_aCtrlIbc.Add(IDC_CMD_IBC_TITLE4);
	m_aCtrlIbc.Add(IDC_CMD_IBC_TITLE5);
	m_aCtrlIbc.Add(IDC_CMD_IBC_TITLE6);
	m_aCtrlIbc.Add(IDC_CMD_IBC_TITLE7);
	m_aCtrlIbc.Add(IDC_CMD_IBC_TITLE8);
	m_aCtrlIbc.Add(IDC_CMD_IBC_TITLE9);
	m_aCtrlIbc.Add(IDC_CMD_IBC_TITLE10);
	m_aCtrlIbc.Add(IDC_CMD_IBC_COMBO1);
	m_aCtrlIbc.Add(IDC_CMD_IBC_COMBO2);
	m_aCtrlIbc.Add(IDC_CMD_IBC_COMBO3);
	m_aCtrlIbc.Add(IDC_CMD_IBC_COMBO4);
	m_aCtrlIbc.Add(IDC_CMD_IBC_COMBO5);
	m_aCtrlIbc.Add(IDC_CMD_IBC_PERIOD_X_EDIT1);
	m_aCtrlIbc.Add(IDC_CMD_IBC_PERIOD_X_EDIT2);
	m_aCtrlIbc.Add(IDC_CMD_IBC_PERIOD_X_EDIT3);
	m_aCtrlIbc.Add(IDC_CMD_IBC_PERIOD_Y_EDIT1);
	m_aCtrlIbc.Add(IDC_CMD_IBC_PERIOD_Y_EDIT2);
	m_aCtrlIbc.Add(IDC_CMD_IBC_PERIOD_Y_EDIT3);
	m_aCtrlIbc.Add(IDC_CMD_IBC_BTN_CALC_PERIOD);

	m_aCtrlEuro1996.Add(IDC_CMD_EURO_FRAME1);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_FRAME2);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_SC_COMBO);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_Q0_COMBO);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_KR_COMBO);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_KD_COMBO);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_ALPHA_EDIT);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_KW_EDIT);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_PERIOD_X_EDIT);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_PERIOD_Y_EDIT);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_BTN_CALC_PERIOD);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_PERIOD_STATIC);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_SC_STATIC);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_Q0_STATIC);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_KR_STATIC);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_KD_STATIC);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_ALPHA_STATIC);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_KW_STATIC);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_XDIR_STATIC);
	m_aCtrlEuro1996.Add(IDC_CMD_EURO_YDIR_STATIC);

	m_aCtrlChina2002.Add(IDC_CMD_CHINA_FRAME1);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_SDC_STATIC);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_SDC_COMBO);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_SFI_STATIC);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_SFI_COMBO);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_SC_STATIC);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_SC_COMBO);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_DELTAN_STATIC);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_DELTAN_COMBO);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_XI_STATIC);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_XI_EDIT);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_ER_RDO1);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_ER_RDO2);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_ALPHA1_CHK);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_FRAME2);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_PERIOD_STATIC);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_XDIR_STATIC);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_YDIR_STATIC);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_PERIOD_X_EDIT);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_PERIOD_Y_EDIT);
	m_aCtrlChina2002.Add(IDC_CMD_CHINA_BTN_CALC_PERIOD);

	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_FRAME1);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_FRAME2);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_ZONE_RATIO_STATIC);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_ZONE_RATIO_CMB);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_ACCE_ZONE_STATIC);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_ACCE_ZONE_CMB);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_VELO_ZONE_STATIC);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_VELO_ZONE_CMB);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_IMPORTANCE_STATIC);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_IMPORTANCE_CMB);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_FOUNDATION_STATIC);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_FOUNDATION_CMB);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_T_ANAL_STATIC);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_T_CODE_STATIC);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_X_STATIC);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_Y_STATIC);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_TX_ANAL_EDIT);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_TY_ANAL_EDIT);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_TX_CODE_EDIT);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_TY_CODE_EDIT);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_T_CALC_BTN);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_R_STATIC);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_RX_CMB);
	m_aCtrlNbc1995.Add(IDC_CMD_NBC95_RY_CMB);

	m_aCtrlIs2002.Add(IDC_CMD_IS02_FRAME);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_FRAME2);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_ZONE_STATIC);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_ZONE_CMB);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_SOIL_STATIC);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_SOIL_CMB);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_IMPORTANCE_STATIC);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_IMPORTANCE_CMB);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_DAMPING_STATIC);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_DAMPING_EDIT);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_DAMPING_FACTOR_STATIC);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_DAMPING_FACTOR_EDIT);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_PERIOD_STATIC);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_X_STATIC);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_Y_STATIC);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_TX_EDIT);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_TY_EDIT);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_RESPONSE_STATIC);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_RX_CMB);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_RY_CMB);
	m_aCtrlIs2002.Add(IDC_CMD_IS02_T_CALC_BTN);

	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_FRAME);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_FRAME2);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_ZONE_STATIC);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_ZONE_CMB);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_SOIL_STATIC);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_SOIL_CMB);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_IMPORTANCE_STATIC);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_IMPORTANCE_CMB);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_ALPHA_STATIC);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_ALPHA_CMB);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_X_STATIC);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_Y_STATIC);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_PERIOD_RDO);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_PERIOD_RDO2);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_PERIOD_STATIC);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_TX_ANAL_EDIT);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_TY_ANAL_EDIT);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_TX_APPR_EDIT);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_TY_APPR_EDIT);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_TX_FINAL_EDIT);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_TY_FINAL_EDIT);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_RESPONSE_STATIC);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_RX_CMB);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_RY_CMB);
	m_aCtrlTaiwan86.Add(IDC_CMD_TAIWAN86_PERIOD_BTN);
	//	InitCalcKSPeriod();

	m_bSaveFoldedInfo = FALSE;
}


void CSeisItemDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisItemDlg2)
	DDX_Control(pDX, IDC_CMD_TAIWAN86_TY_FINAL_EDIT, m_wndTaiwan86TYFinal);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_TY_APPR_EDIT, m_wndTaiwan86TYAppr);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_TY_ANAL_EDIT, m_wndTaiwan86TYAnal);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_TX_FINAL_EDIT, m_wndTaiwan86TXFinal);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_TX_APPR_EDIT, m_wndTaiwan86TXAppr);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_TX_ANAL_EDIT, m_wndTaiwan86TXAnal);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_ZONE_CMB, m_wndTaiwan86ZoneCmb);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_SOIL_CMB, m_wndTaiwan86SoilCmb);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_RY_CMB, m_wndTaiwan86RYCmb);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_RX_CMB, m_wndTaiwan86RXCmb);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_IMPORTANCE_CMB, m_wndTaiwan86ImportanceCmb);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_ALPHA_CMB, m_wndTaiwan86AlphaCmb);
	DDX_Control(pDX, IDC_CMD_IS02_ZONE_CMB, m_wndIs02ZoneCmb);
	DDX_Control(pDX, IDC_CMD_IS02_SOIL_CMB, m_wndIs02SoilCmb);
	DDX_Control(pDX, IDC_CMD_IS02_IMPORTANCE_CMB, m_wndIs02ImportanceCmb);
	DDX_Control(pDX, IDC_CMD_IS02_DAMPING_EDIT, m_wndIs02Damping);
	DDX_Control(pDX, IDC_CMD_IS02_DAMPING_FACTOR_EDIT, m_wndIs02DampingFactor);
	DDX_Control(pDX, IDC_CMD_IS02_TX_EDIT, m_wndIs02Tx);
	DDX_Control(pDX, IDC_CMD_IS02_TY_EDIT, m_wndIs02Ty);
	DDX_Control(pDX, IDC_CMD_IS02_RX_CMB, m_wndIs02RxCmb);
	DDX_Control(pDX, IDC_CMD_IS02_RY_CMB, m_wndIs02RyCmb);
	DDX_Control(pDX, IDC_CMD_NBC95_ZONE_RATIO_CMB, m_wndNbc95ZoneRatio);
	DDX_Control(pDX, IDC_CMD_NBC95_ACCE_ZONE_CMB, m_wndNbc95AcceZone);
	DDX_Control(pDX, IDC_CMD_NBC95_VELO_ZONE_CMB, m_wndNbc95VeloZone);
	DDX_Control(pDX, IDC_CMD_NBC95_IMPORTANCE_CMB, m_wndNbc95Importance);
	DDX_Control(pDX, IDC_CMD_NBC95_FOUNDATION_CMB, m_wndNbc95Foundation);
	DDX_Control(pDX, IDC_CMD_NBC95_RX_CMB, m_wndNbc95Rx);
	DDX_Control(pDX, IDC_CMD_NBC95_RY_CMB, m_wndNbc95Ry);
	DDX_Control(pDX, IDC_CMD_NBC95_TX_ANAL_EDIT, m_wndNbc95TxAnal);
	DDX_Control(pDX, IDC_CMD_NBC95_TY_ANAL_EDIT, m_wndNbc95TyAnal);
	DDX_Control(pDX, IDC_CMD_NBC95_TX_CODE_EDIT, m_wndNbc95TxCode);
	DDX_Control(pDX, IDC_CMD_NBC95_TY_CODE_EDIT, m_wndNbc95TyCode);
	DDX_Control(pDX, IDC_CMD_CHINA_SDC_COMBO, m_wndChSDC);
	DDX_Control(pDX, IDC_CMD_CHINA_SFI_COMBO, m_wndChSFI);
	DDX_Control(pDX, IDC_CMD_CHINA_SC_COMBO, m_wndChSC);
	DDX_Control(pDX, IDC_CMD_CHINA_DELTAN_COMBO, m_wndChDeltaN);
	DDX_Control(pDX, IDC_CMD_CHINA_XI_EDIT, m_wndChDamping);
	DDX_Control(pDX, IDC_CMD_CHINA_PERIOD_Y_EDIT, m_wndChPeriodY);
	DDX_Control(pDX, IDC_CMD_CHINA_PERIOD_X_EDIT, m_wndChPeriodX);
	DDX_Control(pDX, IDC_CMD_EURO_PERIOD_Y_EDIT, m_wndEuroPeriodY);
	DDX_Control(pDX, IDC_CMD_EURO_PERIOD_X_EDIT, m_wndEuroPeriodX);
	DDX_Control(pDX, IDC_CMD_EURO_SC_COMBO, m_wndEuroSc);
	DDX_Control(pDX, IDC_CMD_EURO_Q0_COMBO, m_wndEuroQ0);
	DDX_Control(pDX, IDC_CMD_EURO_KR_COMBO, m_wndEuroKr);
	DDX_Control(pDX, IDC_CMD_EURO_KD_COMBO, m_wndEuroKd);
	DDX_Control(pDX, IDC_CMD_EURO_KW_EDIT, m_wndEuroKw);
	DDX_Control(pDX, IDC_CMD_EURO_ALPHA_EDIT, m_wndEuroAlpha);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_Y_EDIT2, m_wndIBCPeriodCoefY);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_Y_EDIT3, m_wndIBCPeriodCalcY);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_Y_EDIT1, m_wndIBCPeriodCodeY);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_X_EDIT2, m_wndIBCPeriodCoefX);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_X_EDIT3, m_wndIBCPeriodCalcX);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_X_EDIT1, m_wndIBCPeriodCodeX);
	DDX_Control(pDX, IDC_CMD_IBC_COMBO5, m_wndIBCCombo5);
	DDX_Control(pDX, IDC_CMD_IBC_COMBO4, m_wndIBCCombo4);
	DDX_Control(pDX, IDC_CMD_IBC_COMBO3, m_wndIBCCombo3);
	DDX_Control(pDX, IDC_CMD_IBC_COMBO2, m_wndIBCCombo2);
	DDX_Control(pDX, IDC_CMD_IBC_COMBO1, m_wndIBCCombo1);
	DDX_Control(pDX, IDC_CMD_SLP_JPN_ZONE_EDIT, m_wndZone);
	DDX_Control(pDX, IDC_CMD_SLP_JPN_SHEAR_FACTOR, m_wndShearFactor);
	DDX_Control(pDX, IDC_CMD_SLP_JPN_PERIOD_COBX, m_wndSPCombo);
	DDX_Control(pDX, IDC_CMD_RMF_Y_EDIT3, m_wndRmfYEdit3);
	DDX_Control(pDX, IDC_CMD_RMF_X_EDIT3, m_wndRmfXEdit3);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_Y_EDIT2, m_wndPeriodCY_JIS);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_X_EDIT2, m_wndPeriodCX_JIS);
	DDX_Control(pDX, IDC_CMD_PERIOD_A_Y_EDIT2, m_wndPeriodAY_JIS);
	DDX_Control(pDX, IDC_CMD_PERIOD_A_X_EDIT2, m_wndPeriodAX_JIS);
	DDX_Control(pDX, IDC_CMD_BTN_CALC_PERIOD, m_btnCalcPeriod);
	DDX_Control(pDX, IDC_CMD_APPLY, m_wndBtnApply);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_wndDescription);
	DDX_Control(pDX, IDC_CMD_SCALE_GY, m_wndScaleGY);
	DDX_Control(pDX, IDC_CMD_SCALE_GX, m_wndScaleGX);
	DDX_Control(pDX, IDC_CMD_LOADCASE, m_wndLoadCase);
	DDX_Control(pDX, IDC_CMD_RMF_Y_EDIT2, m_wndRmfYEdit2);
	DDX_Control(pDX, IDC_CMD_RMF_X_EDIT2, m_wndRmfXEdit2);
	DDX_Control(pDX, IDC_CMD_PERIOD_Y_EDIT2, m_wndPeriodY2);
	DDX_Control(pDX, IDC_CMD_PERIOD_X_EDIT2, m_wndPeriodX2);
	DDX_Control(pDX, IDC_CMD_CD_EDIT2, m_wndCdEdit2);
	DDX_Control(pDX, IDC_CMD_SST_COMBO2, m_wndSstCombo2);
	DDX_Control(pDX, IDC_CMD_IF_COMBO2, m_wndIfCombo2);
	DDX_Control(pDX, IDC_CMD_SPT_COMBO2, m_wndSptCombo2);
	DDX_Control(pDX, IDC_CMD_ZF_COMBO2, m_wndZfCombo2);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_Y_EDIT, m_wndPeriodCY);
	DDX_Control(pDX, IDC_CMD_PERIOD_C_X_EDIT, m_wndPeriodCX);
	DDX_Control(pDX, IDC_CMD_PERIOD_A_Y_EDIT, m_wndPeriodAY);
	DDX_Control(pDX, IDC_CMD_PERIOD_A_X_EDIT, m_wndPeriodAX);
	DDX_Control(pDX, IDC_CMD_RMF_Y_EDIT, m_wndRmfYEdit);
	DDX_Control(pDX, IDC_CMD_RMF_X_EDIT, m_wndRmfXEdit);
	DDX_Control(pDX, IDC_CMD_RMF_TITLE, m_wndRmfTitle);
	DDX_Control(pDX, IDC_CMD_RMF_TITLE2, m_wndRmfTitle2);
	DDX_Control(pDX, IDC_CMD_SF_COMBO, m_wndSfCombo);
	DDX_Control(pDX, IDC_CMD_SF_EDIT, m_wndSfEdit); // For ATC3-06
	DDX_Control(pDX, IDC_CMD_SF_TITLE, m_wndSfTitle);
	DDX_Control(pDX, IDC_CMD_IF_COMBO, m_wndIfCombo);
	DDX_Control(pDX, IDC_CMD_IF_TITLE, m_wndIfTitle);
	DDX_Control(pDX, IDC_CMD_ZF_COMBO, m_wndZfCombo); // For ATC3-06
	DDX_Control(pDX, IDC_CMD_ZF_EDIT, m_wndZfEdit);
	DDX_Control(pDX, IDC_CMD_ZF_TITLE, m_wndZfTitle);
	DDX_Control(pDX, IDC_CMD_CODE_NAME, m_wndCodeName);
	DDX_Check(pDX, IDC_CMD_CHINA_ALPHA1_CHK, m_wndChAlpha1);
	DDX_Radio(pDX, IDC_CMD_CHINA_ER_RDO1, m_wndChER);
	DDX_Radio(pDX, IDC_CMD_TAIWAN86_PERIOD_RDO, m_nTaiwan86Period);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisItemDlg2, CDialogMove)
	//{{AFX_MSG_MAP(CSeisItemDlg2)
	ON_CBN_SELCHANGE(IDC_CMD_CODE_NAME, OnChangeCodeName)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnCmdLoadcaseBtn)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_PROFILE, OnCmdBtnProfile)
	ON_BN_CLICKED(IDC_CMD_BTN_CALC_PERIOD, OnCmdBtnCalcPeriod)
	ON_CBN_SELCHANGE(IDC_CMD_ZF_COMBO2, OnSelchangeCmdZfCombo2)
	ON_BN_CLICKED(IDC_CMD_BTN_CALC_PERIOD2, OnCmdBtnCalcPeriod2)
	ON_BN_CLICKED(IDC_CMD_IBC_BTN_CALC_PERIOD, OnCmdIbcBtnCalcPeriod)
	ON_BN_CLICKED(IDC_CMD_EURO_BTN_CALC_PERIOD, OnCmdEuroBtnCalcPeriod)
	ON_BN_CLICKED(IDC_CMD_CHINA_BTN_CALC_PERIOD, OnCmdChinaBtnCalcPeriod)
	ON_BN_CLICKED(IDC_CMD_CHINA_ER_RDO1, OnCmdChinaErRdo)
	ON_BN_CLICKED(IDC_CMD_CHINA_ALPHA1_CHK, OnCmdChinaAlpha1Chk)
	ON_BN_CLICKED(IDC_CMD_NBC95_T_CALC_BTN, OnCmdNbc95TCalcBtn)
	ON_BN_CLICKED(IDC_CMD_CHINA_ER_RDO2, OnCmdChinaErRdo)
	ON_BN_CLICKED(IDC_CMD_IS02_T_CALC_BTN, OnCmdIs02CalcPeriodBtn)
	ON_BN_CLICKED(IDC_CMD_TAIWAN86_PERIOD_BTN, OnCmdTaiwan86CalcPeriodBtn)
	ON_BN_CLICKED(IDC_CMD_TAIWAN86_PERIOD_RDO, OnCmdTaiwan86PeriodRdo)
	ON_BN_CLICKED(IDC_CMD_TAIWAN86_PERIOD_RDO2, OnCmdTaiwan86PeriodRdo)
	ON_EN_CHANGE(IDC_CMD_IS02_DAMPING_EDIT, OnChangeCmdIs02DampingEdit)
	ON_EN_CHANGE(IDC_CMD_TAIWAN86_TX_ANAL_EDIT, OnChangeTaiwan86PeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TAIWAN86_TY_ANAL_EDIT, OnChangeTaiwan86PeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TAIWAN86_TX_APPR_EDIT, OnChangeTaiwan86PeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TAIWAN86_TY_APPR_EDIT, OnChangeTaiwan86PeriodEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------
// Interface

void CSeisItemDlg2::InitCalcKSPeriod()
{
	//{{AFX_DATA_INIT(CETC_KSPeriodCalc)
	m_dBx = 1.0;
	m_dBy = 1.0;
	//}}AFX_DATA_INIT

	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	T_STOR_D DataStory, DataStoryBase;

	// 주의 !
	// Seismic Load 에서는 m 단위로 변환하여 받아온다.
	// Wind Load 에서는 현재 단위로 받아온다.
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // M 기준
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);

	CArray<T_STOR_K,T_STOR_K> arKeyStory;
	pAttrCtrl->GetStorKeyList(arKeyStory); //Story Level로 정렬된 리스트
	int nNumOfStory = arKeyStory.GetSize();

	m_nNXCH2001 = m_nNYCH2001 = nNumOfStory - 1; // added by KYM
	m_nNXIBC2000 = m_nNYIBC2000 = nNumOfStory - 1; // added by KYM

	if (nNumOfStory > 0)
		pAttrCtrl->GetStor(arKeyStory[nNumOfStory - 1], DataStory);
	else
		DataStory.Initialize();

	double dBaseLevel = pAttrCtrl->GetBaseLevel();
	m_dHn = DataStory.dStoryLevel - dBaseLevel;

	if (!pAttrCtrl->GetCountStor()) m_dHn = 0;

	m_dHXKOR = m_dHYKOR = m_dHn;
	m_dHXEURO1996 = m_dHYEURO1996 = m_dHn;
	m_dHXCH2001 = m_dHYCH2001 = m_dHn;
	m_dHXJPN2000 = m_dHYJPN2000 = m_dHn;
	m_dHXIBC2000 = m_dHYIBC2000 = m_dHn;
	m_dHXNBC1995 = m_dHYNBC1995 = m_dHn;
	m_dHXIS2002 = m_dHYIS2002 = m_dHn;
	m_dHnXTaiwan86 = m_dHnYTaiwan86 = m_dHn;

	if (nNumOfStory > 0)
	{
		for (int i = 0; i < nNumOfStory; i++)
		{
			pAttrCtrl->GetStor(arKeyStory[i], DataStoryBase);
			if (DataStoryBase.dStoryLevel > dBaseLevel) break;
		}
		if (DataStoryBase.dStoryLevel > dBaseLevel)
		{
			m_dBx = DataStoryBase.WindWidthX;
			m_dBy = DataStoryBase.WindWidthY;
		}
		else
		{
			m_dBx = 1.0;
			m_dBy = 1.0;
		}
	}
	else
	{
		m_dBx = 1.0;
		m_dBy = 1.0;
	}

	m_dBXKOR1992 = m_dBx;
	m_dBYKOR1992 = m_dBy;
	m_dBXCH2001 = m_dBx;
	m_dBYCH2001 = m_dBy;
	m_dBXNBC1995 = m_dBx;
	m_dBYNBC1995 = m_dBy;
	m_dBXIS2002 = m_dBx;
	m_dBYIS2002 = m_dBy;

	// Acx , Acy 구하는 부분.		
	//  CalcForKS2000Ac(); 
	//////////////////////////////////////////////
	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}


BOOL CSeisItemDlg2::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// Member initialize(KOR1992, KOR2000)
	m_dHXKOR = 0.0;
	m_dHYKOR = 0.0;

	// Member initialize(KOR2000)
	m_dAcXKOR2000 = 0.0;
	m_dAcYKOR2000 = 0.0;
	m_nMtdXKOR2000 = 0;
	m_nMtdYKOR2000 = 0;

	// Member initialize(KOR1992)
	m_dBXKOR1992 = 0.0;
	m_dBYKOR1992 = 0.0;
	m_nMtdXKOR1992 = 0;
	m_nMtdYKOR1992 = 0;

	// Member initialize(IBC2000)
	m_dHXIBC2000 = 0.0;
	m_dHYIBC2000 = 0.0;
	m_nNXIBC2000 = 0;
	m_nNYIBC2000 = 0;
	m_nMtdXIBC2000 = 0;
	m_nMtdYIBC2000 = 0;

	// Member initialize(EURO1996)
	m_dHXEURO1996 = 0.0;
	m_dHYEURO1996 = 0.0;
	m_dAcXEURO1996 = 0.0;
	m_dAcYEURO1996 = 0.0;
	m_nDXEURO1996 = 0.0;
	m_nDYEURO1996 = 0.0;
	m_nMtdXEURO1996 = 0;
	m_nMtdYEURO1996 = 0;

	// Member initialize(CH2001)
	m_dHXCH2001 = 0.0;
	m_dHYCH2001 = 0.0;
	m_dBXCH2001 = 0.0;
	m_dBYCH2001 = 0.0;
	m_nNXCH2001 = 0;
	m_nNYCH2001 = 0;
	m_nMtdXCH2001 = 0;
	m_nMtdYCH2001 = 0;

	// Member initialize(JPN2000)
	m_dHXJPN2000 = 0.0;
	m_dHYJPN2000 = 0.0;
	m_dAXJPN2000 = 0;
	m_dAYJPN2000 = 0;

	// Member initialize(NBC1995)
	m_dHXNBC1995 = 0.0;
	m_dHYNBC1995 = 0.0;
	m_dBXNBC1995 = 0.0;
	m_dBYNBC1995 = 0.0;
	m_nNXNBC1995 = 0;
	m_nNYNBC1995 = 0;
	m_nMtdXNBC1995 = 0;
	m_nMtdYNBC1995 = 0;

	// Member initialize(IS2002)
	m_dHXIS2002 = 0.0;
	m_dHYIS2002 = 0.0;
	m_dBXIS2002 = 0.0;
	m_dBYIS2002 = 0.0;
	m_nMtdXIS2002 = 0;
	m_nMtdYIS2002 = 0;

	// Member initialize(Taiwan86)
	m_dHnXTaiwan86 = 0.0;
	m_dHnYTaiwan86 = 0.0;
	m_dAcXTaiwan86 = 0.0;
	m_dAcYTaiwan86 = 0.0;
	m_nMtdXTaiwan86 = 0;
	m_nMtdYTaiwan86 = 0;

	int nMovingType = CProduct::GetMovingType();

	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.ScaleFactor_X = 1.0;
		m_Data.ScaleFactor_Y = 1.0;

		if (nMovingType == D_PRODUCT_MOVING_US || nMovingType == D_PRODUCT_MOVING_RUS)
		{
			if (CProduct::IsIndia())
				m_Data.nCodeType = 11; // India(IS1893-2002)
			else if (CProduct::IsTaiwan())
				m_Data.nCodeType = 12; // Taiwan(Taiwan86)
			else
				m_Data.nCodeType = 7; // IBC2000(ASCE7-98)
		}
		else if (nMovingType == D_PRODUCT_MOVING_JP)
			m_Data.nCodeType = 6; // Japan(1994)
		else if (nMovingType == D_PRODUCT_MOVING_CH)
			m_Data.nCodeType = 9; // China(GB50011-2001)
		else
			m_Data.nCodeType = 5; // Korean(Arch. 2000)		
	}

	CString strCodeName[] = {
		_T("Korea(Arch.1992)"), _T("UBC(1991)"), _T("UBC(1997)"),
		_T("ATC3-06"), _T("Korea(Arch.2000)"), _T("Japan(Arch.2000)"),
		_T("IBC2000(ASCE7-98)"), _T("Eurocode-8(1996)"), _T("China(GB50011-2001)"),
		_T("NBC(1995)"), _T("IS1893(2002)"), _T("Taiwan(1999)")
	};

	CArray<int, int> aSeisSeq;
	aSeisSeq.RemoveAll();
	if (nMovingType == D_PRODUCT_MOVING_US || nMovingType == D_PRODUCT_MOVING_RUS)
	{
		if (CProduct::IsIndia())
		{
			aSeisSeq.Add(10); // IS1893(2002)    
			aSeisSeq.Add(7); // Eurocode-8(1996)      
		}
		else if (CProduct::IsTaiwan())
		{
			aSeisSeq.Add(11); // Taiwan(1986)    
			aSeisSeq.Add(8); // China(GB50011-2001)
			aSeisSeq.Add(6); // IBC2000         
			aSeisSeq.Add(2); // UBC(1997)       
			aSeisSeq.Add(1); // NBC(1995)  
			aSeisSeq.Add(3); // UBC(1991)      
			aSeisSeq.Add(9); // ATC3-06               
		}
		else // US original
		{
			aSeisSeq.Add(6); // IBC2000         
			aSeisSeq.Add(2); // UBC(1997)       
			aSeisSeq.Add(1); // UBC(1991)   
			aSeisSeq.Add(3); // ATC3-06         
			aSeisSeq.Add(9); // NBC(1995)       
			aSeisSeq.Add(7); // Eurocode-8(1996)
			aSeisSeq.Add(11); // Taiwan(1986)    
		}
	}
	else if (nMovingType == D_PRODUCT_MOVING_JP)
	{
		aSeisSeq.Add(5); // Japan(Arch.2000)
		aSeisSeq.Add(6); // IBC2000         
		aSeisSeq.Add(2); // UBC(1997)       
		aSeisSeq.Add(3); // NBC(1995)          
		aSeisSeq.Add(9); // ATC3-06            
		aSeisSeq.Add(7); // Eurocode-8(1996)   
		aSeisSeq.Add(8); // China(GB50011-2001)
		aSeisSeq.Add(4); // Korea(Arch.2000)         
	}
	else if (nMovingType == D_PRODUCT_MOVING_CH)
	{
		aSeisSeq.Add(8); // China(GB50011-2001)
		aSeisSeq.Add(6); // IBC2000            
		aSeisSeq.Add(2); // UBC(1997)          
		aSeisSeq.Add(1); // UBC(1991)          
		aSeisSeq.Add(3); // NBC(1995)          
		aSeisSeq.Add(9); // ATC3-06            
		aSeisSeq.Add(7); // Eurocode-8(1996)   
		aSeisSeq.Add(5); // Japan(Arch.2000)   
		aSeisSeq.Add(4); // Korea(Arch.2000)   
		aSeisSeq.Add(0); // Korea(Arch.1992)   
	}
	else
	{
		aSeisSeq.Add(4); // Korea(Arch.2000)     
		aSeisSeq.Add(0); // Korea(Arch.1992)
		aSeisSeq.Add(6); // IBC2000         
		aSeisSeq.Add(2); // UBC(1997)       
		aSeisSeq.Add(1); // NBC(1995)  
		aSeisSeq.Add(3); // UBC(1991)      
		aSeisSeq.Add(9); // ATC3-06         
		aSeisSeq.Add(7); // Eurocode-8(1996)      
		aSeisSeq.Add(8); // China(GB50011-2001)
		aSeisSeq.Add(5); // Japan(Arch.2000)
		aSeisSeq.Add(10); // IS1893(2002)
		aSeisSeq.Add(11); // Taiwan(1986)    
	}
	//  int nIndex;
	for (int i = 0; i < aSeisSeq.GetSize(); i++)
	{
		CDlgUtil::CobxAddItem(m_wndCodeName, strCodeName[aSeisSeq[i]], aSeisSeq[i] + 1); // 아래 방법도 동일
		//  nIndex = m_wndCodeName.AddString(strCodeName[nWindSeq[i]]); m_wndCodeName.SetItemData(nIndex, nWindSeq[i]+1);	
	}

	AlignControl();

	CString aInitKS[] = {
		_T("S2(1.2)"), _LS(IDS_WG_CMD__ADD2__Area2_0_12_), _T("1.2"), _T("0"), _T("0"), _T("0"), _T("0"), _T("6"),
		_T("6")
	};
	CString aInitKS2000[] = {
		_T("S2(1.2)"), _LS(IDS_WG_CMD__ADD2__Area1_0_11_), _T("1.2"), _T("0"), _T("0"), _T("0"), _T("0"), _T("6"),
		_T("6")
	};
	CString aInitUBC91[] = {
		_T("S1(1.0)"), _T("1 (0.075)"), _T("1.0"), _T("0"), _T("0"), _T("0"), _T("0"), _T("6"), _T("6")
	};
	CString aInitATC[] = {
		_T("0"), _T("0"), _T("0"), _T("0"), _T("0"), _T("0"), _T("0"), _T("0")
	};
	m_aKS2000.SetSize(9);
	m_aKS.SetSize(9);
	m_aUBC91.SetSize(9);
	m_aATC.SetSize(8);

	for (int i = 0; i < 9; i++)
	{
		m_aKS[i] = aInitKS[i];
		m_aKS2000[i] = aInitKS2000[i];
		m_aUBC91[i] = aInitUBC91[i];
	}
	for (int i = 0; i < 8; i++) m_aATC[i] = aInitATC[i];

	m_wndLoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);

	m_wndZfCombo2.AddString(_T("1 (0.075)"));
	m_wndZfCombo2.AddString(_T("2A(0.15)"));
	m_wndZfCombo2.AddString(_T("2B(0.20)"));
	m_wndZfCombo2.AddString(_T("3 (0.30)"));
	m_wndZfCombo2.AddString(_T("4 (0.40)"));

	m_wndSptCombo2.AddString(_T("Sa"));
	m_wndSptCombo2.AddString(_T("Sb"));
	m_wndSptCombo2.AddString(_T("Sc"));
	m_wndSptCombo2.AddString(_T("Sd"));
	m_wndSptCombo2.AddString(_T("Se"));
	m_wndSptCombo2.AddString(_T("Sf"));

	m_wndSstCombo2.AddString(_T("A"));
	m_wndSstCombo2.AddString(_T("B"));
	m_wndSstCombo2.AddString(_T("C"));

	m_wndSfCombo.AddString(_T("S1 (1.0)"));
	m_wndSfCombo.AddString(_T("S2 (1.2)"));
	m_wndSfCombo.AddString(_T("S3 (1.5)"));
	if (m_Data.nCodeType == 5) m_wndSfCombo.AddString(_T("S4 (2.0)"));

	m_wndIfCombo2.AddString(_T("1.0"));
	m_wndIfCombo2.AddString(_T("1.25"));

	if (m_Data.UBC1997.nSoilProfileType == 0) m_Data.UBC1997.nSoilProfileType = 1;
	if (m_Data.UBC1997.nSeismicSourceType == 0) m_Data.UBC1997.nSeismicSourceType = 1;
	m_wndZfCombo2.SetCurSel(0);
	m_wndSptCombo2.SetCurSel(m_Data.UBC1997.nSoilProfileType - 1);
	m_wndSstCombo2.SetCurSel(m_Data.UBC1997.nSeismicSourceType - 1);
	m_wndCdEdit2.SetWindowText(_T("10"));
	m_wndIfCombo2.SetWindowText(_T("1.0")); // for UBC1997 Code

	SetHeaderTitle();
	if (!m_bModify)
	{
		m_Data.KS1992.dSoilFactor = 1.2;
		m_Data.KS1992.dImportanceFactor = 1.2;
	}

	// JIS initialize //////////////////////////////////////
	int nTemp;
	if (m_Data.JP.dSoilFactor == 0.4) nTemp = 0;
	else if (m_Data.JP.dSoilFactor == 0.6) nTemp = 1;
	else if (m_Data.JP.dSoilFactor == 0.8) nTemp = 2;
	m_wndSPCombo.AddString(_T("0.4sec (I)"));
	m_wndSPCombo.AddString(_T("0.6sec (II)"));
	m_wndSPCombo.AddString(_T("0.8sec (III)"));
	m_wndSPCombo.SetCurSel(nTemp);
	m_wndZone.SetEditUnit(m_Data.JP.dZoneFactor);
	m_wndShearFactor.SetEditUnit(m_Data.JP.dStdShearFactor);
	////////////////////////////////////////////////////////

	// IBC initialize //////////////////////////////////////
	m_wndIBCCombo1.SetCurSel(0);
	m_wndIBCCombo2.SetCurSel(3);
	m_wndIBCCombo3.SetCurSel(0);
	m_wndIBCCombo4.SetCurSel(0);
	m_wndIBCCombo5.SetCurSel(0);

	// EURO1996 ////////////////////////////////////////////  added by KYM
	m_wndEuroSc.AddString(_T("A (1.0)"));
	m_wndEuroSc.AddString(_T("B (1.0)"));
	m_wndEuroSc.AddString(_T("C (0.9)"));
	m_wndEuroQ0.AddString(_T("5.0"));
	m_wndEuroQ0.AddString(_T("4.5"));
	m_wndEuroQ0.AddString(_T("4.0"));
	m_wndEuroQ0.AddString(_T("3.5"));
	m_wndEuroQ0.AddString(_T("2.0"));
	m_wndEuroKd.AddString(_LS(IDS_CMD0417__High_1_00_));
	m_wndEuroKd.AddString(_LS(IDS_CMD0417__Medium_0_75_));
	m_wndEuroKd.AddString(_LS(IDS_CMD0417__Low_0_50_));
	m_wndEuroKr.AddString(_LS(IDS_CMD0417__Regular_1_00_));
	m_wndEuroKr.AddString(_LS(IDS_CMD0417__Irregular_0_80_));

	m_wndEuroKw.SetUnitType(0);
	m_wndEuroAlpha.SetUnitType(0);
	m_wndEuroPeriodX.SetUnitType(0);
	m_wndEuroPeriodY.SetUnitType(0);

	// Eurocode-8 (1996) //////////////////////////////////////////
	m_wndEuroSc.SetCurSel(m_Data.EURO1996.nSoilClass);
	m_wndEuroQ0.SetCurSel(m_Data.EURO1996.nBasicBehavior);
	m_wndEuroKd.SetCurSel(m_Data.EURO1996.nDuctility);
	m_wndEuroKr.SetCurSel(m_Data.EURO1996.nRegularity);
	m_wndEuroKw.SetEditUnit(m_Data.EURO1996.dFailureModeFactor);
	m_wndEuroAlpha.SetEditUnit(m_Data.EURO1996.dAlphaFactor);
	m_wndEuroPeriodX.SetEditUnit(m_Data.EURO1996.dPeriod_X);
	m_wndEuroPeriodY.SetEditUnit(m_Data.EURO1996.dPeriod_Y);

	// China2002 //////////////////////////////////////////
	m_wndChSDC.AddString(_T("1"));
	m_wndChSDC.AddString(_T("2"));
	m_wndChSDC.AddString(_T("3"));
	m_wndChSFI.AddString(_T("6 (0.05g)"));
	m_wndChSFI.AddString(_T("7 (0.10g)"));
	m_wndChSFI.AddString(_T("7 (0.15g)"));
	m_wndChSFI.AddString(_T("8 (0.20g)"));
	m_wndChSFI.AddString(_T("8 (0.30g)"));
	m_wndChSFI.AddString(_T("9 (0.40g)"));
	m_wndChSC.AddString(_T("I"));
	m_wndChSC.AddString(_T("II"));
	m_wndChSC.AddString(_T("III"));
	m_wndChSC.AddString(_T("IV"));
	m_wndChDeltaN.AddString(_LS(IDS_WG_CMD__ADD2_DELTAN_1)); // RC or Steel Multistory
	m_wndChDeltaN.AddString(_LS(IDS_WG_CMD__ADD2_DELTAN_2)); // Interior Framed Multistory
	m_wndChDeltaN.AddString(_LS(IDS_WG_CMD__ADD2_DELTAN_3)); // Tall Steel Building(JGJ99-98)
	m_wndChDeltaN.AddString(_LS(IDS_WG_CMD__ADD2_DELTAN_4)); // Etc

	m_wndChSDC.SetCurSel(m_Data.CH2002.nSeisDgnCategory);
	m_wndChSFI.SetCurSel(m_Data.CH2002.nSeisFortification);
	m_wndChSC.SetCurSel(m_Data.CH2002.nSiteClass);
	m_wndChDeltaN.SetCurSel(m_Data.CH2002.nDeltaN);
	m_wndChDamping.SetEditUnit(m_Data.CH2002.dDamping);
	m_wndChER = m_Data.CH2002.nEarthResponse;
	m_wndChAlpha1 = m_Data.CH2002.bAlpha1;
	m_wndChPeriodX.SetEditUnit(m_Data.CH2002.dPeriod_X);
	m_wndChPeriodY.SetEditUnit(m_Data.CH2002.dPeriod_Y);

	// NBC1995 //////////////////////////////////////////
	CString aNbc95ZoneRatio[] = {_T("0.00"),_T("0.05"),_T("0.10"),_T("0.15"),_T("0.20"),_T("0.30"),_T("0.40")};
	CString aNbc95AcceVelo[] = {_T("0"),_T("1"),_T("2"),_T("3"),_T("4"),_T("5"),_T("6")};
	CString aNbc95Rfator[] = {_T("1.0"),_T("1.5"),_T("2.0"),_T("3.0"),_T("3.5"),_T("4.0")};
	CString aNbc95Ifator[] = {_T("1.0"),_T("1.3"),_T("1.5")};
	CString aNbc95Ffator[] = {_T("1.0"),_T("1.3"),_T("1.5"),_T("2.0")};
	for (int i = 0; i < 7; i++)
	{
		m_wndNbc95ZoneRatio.AddString(aNbc95ZoneRatio[i]);
		m_wndNbc95AcceZone.AddString(aNbc95AcceVelo[i]);
		m_wndNbc95VeloZone.AddString(aNbc95AcceVelo[i]);
	}
	for (int i = 0; i < 3; i++) m_wndNbc95Importance.AddString(aNbc95Ifator[i]);
	for (int i = 0; i < 4; i++) m_wndNbc95Foundation.AddString(aNbc95Ffator[i]);
	for (int i = 0; i < 6; i++)
	{
		m_wndNbc95Rx.AddString(aNbc95Rfator[i]);
		m_wndNbc95Ry.AddString(aNbc95Rfator[i]);
	}
	m_wndNbc95TxAnal.SetEditUnit(m_Data.NBC1995.dPeriodAnalysis_X);
	m_wndNbc95TyAnal.SetEditUnit(m_Data.NBC1995.dPeriodAnalysis_Y);
	m_wndNbc95TxCode.SetEditUnit(m_Data.NBC1995.dPeriodCode_X);
	m_wndNbc95TyCode.SetEditUnit(m_Data.NBC1995.dPeriodCode_Y);
	m_wndNbc95ZoneRatio.SetCurSel(1);
	m_wndNbc95AcceZone.SetCurSel(m_Data.NBC1995.nAccelerationZone);
	m_wndNbc95VeloZone.SetCurSel(m_Data.NBC1995.nVelocityZone);
	m_wndNbc95Importance.SetCurSel(0);
	m_wndNbc95Foundation.SetCurSel(0);
	m_wndNbc95Rx.SetCurSel(5);
	m_wndNbc95Ry.SetCurSel(5);

	// IS2002 //////////////////////////////////////////  
	CString aIs2002Zone[] = {_T("II (0.10)"),_T("III (0.16)"),_T("IV (0.24)"),_T("V (0.36)")};
	CString aIs2002Soil[] = {_T("I (Rock or Hard Soil)"),_T("II (Medium Soil)"),_T("III (Soft Soil)")};
	CString aIs2002Importance[] = {_T("1.0"),_T("1.5")};
	CString aIs2002Response[] = {_T("1.5"),_T("2.5"),_T("3.0"),_T("4.0"),_T("4.5"),_T("5.0")};
	for (int i = 0; i < 4; i++) m_wndIs02ZoneCmb.AddString(aIs2002Zone[i]);
	for (int i = 0; i < 3; i++) m_wndIs02SoilCmb.AddString(aIs2002Soil[i]);
	for (int i = 0; i < 2; i++) m_wndIs02ImportanceCmb.AddString(aIs2002Importance[i]);
	for (int i = 0; i < 6; i++) m_wndIs02RxCmb.AddString(aIs2002Response[i]);
	for (int i = 0; i < 6; i++) m_wndIs02RyCmb.AddString(aIs2002Response[i]);
	m_wndIs02ZoneCmb.SetCurSel(0);
	m_wndIs02SoilCmb.SetCurSel(0);
	m_wndIs02ImportanceCmb.SetCurSel(0);
	m_wndIs02RxCmb.SetCurSel(2);
	m_wndIs02RyCmb.SetCurSel(2);

	m_wndIs02Damping.SetEditUnit(m_Data.IS2002.dDamping);
	m_wndIs02DampingFactor.SetEditUnit(GetDampingFactorIS2002(m_Data.IS2002.dDamping));
	m_wndIs02Tx.SetEditUnit(m_Data.IS2002.dPeriod_X);
	m_wndIs02Ty.SetEditUnit(m_Data.IS2002.dPeriod_X);

	// Taiwan1986 //////////////////////////////////////////  
	m_nTaiwan86Period = m_Data.TAIWAN86.nPeriodMethod;
	CString aTaiwan86Zone[] = {_T("I (0.33g)"),_T("II (0.23g)")};
	CString aTaiwan86Soil[] = {_T("I (Stiff Soil)"),_T("II (Medium Soil)"),_T("III (Soft Soil)"),_T("Taipei Basin")};
	CString aTaiwan86Importance[] = {_T("1.0"),_T("1.25"),_T("1.5")};
	CString aTaiwan86Alpha[] = {_T("1.2"),_T("1.5")};
	CString aTaiwan86Response[] = {_T("1.6"),_T("2.4"),_T("2.8"),_T("3.2"),_T("3.6"),_T("4.0"),_T("4.4"),_T("4.8")};
	for (int i = 0; i < 2; i++) m_wndTaiwan86ZoneCmb.AddString(aTaiwan86Zone[i]);
	for (int i = 0; i < 4; i++) m_wndTaiwan86SoilCmb.AddString(aTaiwan86Soil[i]);
	for (int i = 0; i < 3; i++) m_wndTaiwan86ImportanceCmb.AddString(aTaiwan86Importance[i]);
	for (int i = 0; i < 2; i++) m_wndTaiwan86AlphaCmb.AddString(aTaiwan86Alpha[i]);
	for (int i = 0; i < 8; i++) m_wndTaiwan86RXCmb.AddString(aTaiwan86Response[i]);
	for (int i = 0; i < 8; i++) m_wndTaiwan86RYCmb.AddString(aTaiwan86Response[i]);
	m_wndTaiwan86ZoneCmb.SetCurSel(0);
	m_wndTaiwan86SoilCmb.SetCurSel(0);
	m_wndTaiwan86ImportanceCmb.SetCurSel(0);
	m_wndTaiwan86AlphaCmb.SetCurSel(0);
	m_wndTaiwan86RXCmb.SetCurSel(0);
	m_wndTaiwan86RYCmb.SetCurSel(0);

	m_wndTaiwan86TXAnal.SetEditUnit(m_Data.TAIWAN86.dPeriodAnalX);
	m_wndTaiwan86TYAnal.SetEditUnit(m_Data.TAIWAN86.dPeriodAnalY);
	m_wndTaiwan86TXAppr.SetEditUnit(m_Data.TAIWAN86.dPeriodApprX);
	m_wndTaiwan86TYAppr.SetEditUnit(m_Data.TAIWAN86.dPeriodApprY);
	m_wndTaiwan86TXFinal.SetEditUnit(m_Data.TAIWAN86.dPeriodFinalX);
	m_wndTaiwan86TYFinal.SetEditUnit(m_Data.TAIWAN86.dPeriodFinalY);

	m_wndTaiwan86TXFinal.SetReadOnly(TRUE);
	m_wndTaiwan86TYFinal.SetReadOnly(TRUE);
	////////////////////////////////////////////

	UpdateData(FALSE);

	Data2Dlg();

	InitCalcKSPeriod();

	if (m_bModify) m_wndBtnApply.EnableWindow(FALSE);
	UpdateData(FALSE); //1 2002.04.01 추가
	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemDlg2::AddAdditionalLoad(T_STOR_K StartStorKey, T_STOR_K EndStorKey, double dblX, double dblY)
{
	// Story Key List를 얻는다.
	CArray<T_STOR_K, T_STOR_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aKeyList);

	// Additional Seismic Load Item 을 만든다.
	T_SEIS_ADDITION item;
	item.AddX = dblX;
	item.AddY = dblY;

	// Story List에서 시작 키의 Index를 찾는다.
	int nCount = aKeyList.GetSize();
	int i = 0;
	for (i = 0; i < nCount; i++)
		if (aKeyList[i] == StartStorKey) break;

	// 입력된 Seismic Load List를 만든다.
	T_STOR_K key;
	CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&> aAddition;
	do
	{
		key = aKeyList[i++];
		item.KeyStor = key;
		aAddition.Add(item);
	}
	while (key != EndStorKey);

	// 새로 만들어진 List와 기존의 Seismic Load List를 Merge한다.
	int nCount1 = aAddition.GetSize();
	int nCount2 = m_Data.arAddition.GetSize();
	int n1, n2;
	CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&> aNewAddition;
	for (n1 = n2 = 0; n1 < nCount1 && n2 < nCount2;)
	{
		if (aAddition[n1].KeyStor == m_Data.arAddition[n2].KeyStor)
		{
			aNewAddition.Add(aAddition[n1]);
			n1++;
			n2++;
		}
		else
		{
			T_STOR_D data1, data2;
			m_pDoc->m_pAttrCtrl->GetStor(aAddition[n1].KeyStor, data1);
			m_pDoc->m_pAttrCtrl->GetStor(m_Data.arAddition[n2].KeyStor, data2);
			if (data1.dStoryLevel < data2.dStoryLevel)
				aNewAddition.Add(aAddition[n1++]);
			else aNewAddition.Add(m_Data.arAddition[n2++]);
		}
	}
	// 남은 부분 처리
	for (; n1 < nCount1; n1++) aNewAddition.Add(aAddition[n1]);
	for (; n2 < nCount2; n2++) aNewAddition.Add(m_Data.arAddition[n2]);

	// 새로 만들어진 것을 복사한다.
	m_Data.arAddition.RemoveAll();
	m_Data.arAddition.Copy(aNewAddition);

	// List를 갱신한다.
	MakeItemEx();
}

//-------------------------------------------------------------------------
// Implementation
void CSeisItemDlg2::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	UINT aFirstCtrlID[] = {
		IDC_CMD_FRAME3, IDC_CMD_SLP_JPN_TITLE4, IDC_CMD_IBC_FRAME1, IDC_CMD_EURO_FRAME1,
		IDC_CMD_CHINA_FRAME1, IDC_CMD_NBC95_FRAME1, IDC_CMD_IS02_FRAME, IDC_CMD_TAIWAN86_FRAME
	};
	CArray<UINT, UINT>* aCtrlSet[] = {
		&m_aCtrlUbc97, &m_aCtrlJapan, &m_aCtrlIbc, &m_aCtrlEuro1996, &m_aCtrlChina2002,
		&m_aCtrlNbc1995, &m_aCtrlIs2002, &m_aCtrlTaiwan86
	};

	GetDlgItem(IDC_CMD_FRAME1)->GetWindowRect(rRef);
	for (int i = 0; i < sizeof(aFirstCtrlID) / sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, *aCtrlSet[i], nDistY);
	}

	GetDlgItem(IDC_CMD_SF_COMBO)->GetWindowRect(rRef);
	GetDlgItem(m_aCtrlAtcEdit[0])->GetWindowRect(rToMove);
	//CDlgUtil::CtrlMoveDistXY(this, m_aCtrlAtcEdit, rRef.top-rToMove.top, rRef.left-rToMove.left);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlAtcEdit, rRef.top - rToMove.top);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(12);
	MoveWindow(r);
	MoveDlgWithinScreen();
}


void CSeisItemDlg2::CtrlShowHide()
{
	CDlgUtil::CtrlShowHide(this, m_aCtrlEtc, (m_Data.nCodeType != 3 && m_Data.nCodeType != 6 && m_Data.nCodeType != 7));
	CDlgUtil::CtrlShowHide(this, m_aCtrlUbc97, m_Data.nCodeType == 3);
	CDlgUtil::CtrlShowHide(this, m_aCtrlJapan, m_Data.nCodeType == 6);
	CDlgUtil::CtrlShowHide(this, m_aCtrlIbc, m_Data.nCodeType == 7);
	CDlgUtil::CtrlShowHide(this, m_aCtrlEuro1996, m_Data.nCodeType == 8); // added by KYM  
	CDlgUtil::CtrlShowHide(this, m_aCtrlChina2002, m_Data.nCodeType == 9); // added by KYM  
	CDlgUtil::CtrlShowHide(this, m_aCtrlNbc1995, m_Data.nCodeType == 10); // added by KYM  
	CDlgUtil::CtrlShowHide(this, m_aCtrlIs2002, m_Data.nCodeType == 11); // added by KYM  
	CDlgUtil::CtrlShowHide(this, m_aCtrlTaiwan86, m_Data.nCodeType == 12); // added by KYM  

	int nZFCode;
	if (m_Data.nCodeType != 3 && m_Data.nCodeType != 6 && m_Data.nCodeType != 7) // if not UBC 97
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlAtcEdit, m_Data.nCodeType == 4);
		CDlgUtil::CtrlShowHide(this, m_aCtrlSfZf, m_Data.nCodeType != 4);
	}

	switch (m_Data.nCodeType)
	{
	case 1: // Korean (Arch,1992)
	case 5: // Korean (Arch,2000)
		m_wndIfTitle.ShowWindow(SW_SHOW);
		m_wndIfCombo.ShowWindow(SW_SHOW);
		m_wndSfTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Soil_Profile_Type__));
		m_wndZfTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Earthquake_Area__));
		m_wndIfTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Importance_Factor__));
		m_wndRmfTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Response_Modification_Coeff___));
		m_btnCalcPeriod.ShowWindow(SW_SHOW);
		break;
	case 2: // UBC(1991)
		m_wndIfTitle.ShowWindow(SW_SHOW);
		m_wndIfCombo.ShowWindow(SW_SHOW);
		m_wndSfTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Soil_Profile_Type__));
		m_wndZfTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Seismic_Zone_Factor__));
		m_wndIfTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Importance_Factor__));
		m_wndRmfTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Ductility_Coefficient_Rw___));
		m_btnCalcPeriod.ShowWindow(SW_HIDE);
		break;
	case 3: // UBC(1997)
		nZFCode = GetZoneFactorCode(3, m_Data.UBC1997.dSeismicZoneFactor);
		m_wndRmfTitle2.SetWindowText(_LS(IDS_WG_CMD__ADDD__Ductility_Coefficient_R___));
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlZone4, nZFCode == 4);
		break;
	case 4: // ATC3-06
		m_wndIfTitle.ShowWindow(SW_HIDE);
		m_wndIfCombo.ShowWindow(SW_HIDE);
		m_wndSfTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Soil_Profile_Coefficient__));
		m_wndZfTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Effective_Peak_Velocity__));
		m_wndRmfTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Response_Modification_Factor__));
		m_btnCalcPeriod.ShowWindow(SW_HIDE);
		break;
	case 8: // Euro-8(1996)
	case 9: // China2002
	case 10: // NBC(1995) 
	case 11: // IS(2002) 
	case 12: // Taiwan(1986) 
		CDlgUtil::CtrlShowHide(this, m_aCtrlEtc, FALSE); // added by KYM : 초기 설정된 화면을 없앤다.
		CDlgUtil::CtrlShowHide(this, m_aCtrlSfZf, FALSE); // added by KYM : 초기 설정된 화면을 없앤다.
		CDlgUtil::CtrlShowHide(this, m_aCtrlAtcEdit, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlZone4, FALSE);
		break;
	}
}


void CSeisItemDlg2::Data2Dlg()
{
	/*
	if (m_Data.nCodeType < 1 || m_Data.nCodeType > 12) 
	{
		AfxMessageBox(_T("Error! Seismic Load Code"));
		return;
	}
	*/
	CString str;

	m_wndLoadCase.ChangeSelect(D_LOADCASE_STATIC, m_Key);

	CDlgUtil::CobxSetCurSelItemData(m_wndCodeName, m_Data.nCodeType); // v3
	m_wndDescription.SetWindowText(m_Data.Description);

	CtrlShowHide();

	int nTemp;
	int nSFCode, nZFCode;
	CString csVal;
	SetZoneFactorCombo(m_Data.nCodeType);
	SetImpoFactorCombo(m_Data.nCodeType);
	switch (m_Data.nCodeType)
	{
	case 1: // Korean (Arch,1992)
		nSFCode = GetSoilFactorCode(1, m_Data.KS1992.dSoilFactor);
		m_wndSfCombo.SetCurSel(nSFCode);
		nZFCode = GetZoneFactorCode(1, m_Data.KS1992.dZoneFactor);
		m_wndZfCombo.SetCurSel(nZFCode);

		csVal.Format(_T("%g"), m_Data.KS1992.dImportanceFactor); // 참고 (combo 박스의 drop down)
		m_wndIfCombo.SetWindowText(csVal); // 참고 

		m_wndPeriodAX.SetEditUnit(m_Data.KS1992.dPeriodAnalysis_X);
		m_wndPeriodAY.SetEditUnit(m_Data.KS1992.dPeriodAnalysis_Y);
		m_wndPeriodCX.SetEditUnit(m_Data.KS1992.dPeriodCode_X);
		m_wndPeriodCY.SetEditUnit(m_Data.KS1992.dPeriodCode_Y);
		m_wndRmfXEdit.SetEditUnit(m_Data.KS1992.dResponseModificationFactor_X);
		m_wndRmfYEdit.SetEditUnit(m_Data.KS1992.dResponseModificationFactor_Y);
		break;
	case 2: // UBC(1991)
		nSFCode = GetSoilFactorCode(2, m_Data.UBC1991.dSoilProfileFactor);
		m_wndSfCombo.SetCurSel(nSFCode);
		nZFCode = GetZoneFactorCode(2, m_Data.UBC1991.dZoneFactor);
		m_wndZfCombo.SetCurSel(nZFCode);

		csVal.Format(_T("%g"), m_Data.UBC1991.dImportanceFactor);
		m_wndIfCombo.SetWindowText(csVal);

		m_wndPeriodAX.SetEditUnit(m_Data.UBC1991.dPeriodAnalysis_X);
		m_wndPeriodAY.SetEditUnit(m_Data.UBC1991.dPeriodAnalysis_Y);
		m_wndPeriodCX.SetEditUnit(m_Data.UBC1991.dPeriodCode_X);
		m_wndPeriodCY.SetEditUnit(m_Data.UBC1991.dPeriodCode_Y);
		m_wndRmfXEdit.SetEditUnit(m_Data.UBC1991.dSystemCoefficient_X);
		m_wndRmfYEdit.SetEditUnit(m_Data.UBC1991.dSystemCoefficient_Y);
		break;
	case 3: // UBC(1997)
		m_wndSptCombo2.SetCurSel(m_Data.UBC1997.nSoilProfileType - 1);
		nZFCode = GetZoneFactorCode(3, m_Data.UBC1997.dSeismicZoneFactor);
		m_wndZfCombo2.SetCurSel(nZFCode);

		csVal.Format(_T("%g"), m_Data.UBC1997.dImportanceFactor);
		m_wndIfCombo2.SetWindowText(csVal);
		if (nZFCode == 4)
		{
			m_wndSstCombo2.SetCurSel(m_Data.UBC1997.nSeismicSourceType - 1);
			m_wndCdEdit2.SetEditUnit(m_Data.UBC1997.dCloseastDistance);
		}
		else
		{
			m_wndSstCombo2.SetCurSel(0);
			m_wndCdEdit2.SetEditUnit(_tstof(_T("10.0")));
		}
		m_wndPeriodX2.SetEditUnit(m_Data.UBC1997.dPeriod_X);
		m_wndPeriodY2.SetEditUnit(m_Data.UBC1997.dPeriod_Y);
		m_wndRmfXEdit2.SetEditUnit(m_Data.UBC1997.dResponseModificationFactor_X);
		m_wndRmfYEdit2.SetEditUnit(m_Data.UBC1997.dResponseModificationFactor_Y);
		break;
	case 4: // ATC3-06
		m_wndSfEdit.SetEditUnit(m_Data.ATC306.dSoilProfileCoefficient);
		m_wndZfEdit.SetEditUnit(m_Data.ATC306.dEffectivePeakVelocity);
		m_wndPeriodAX.SetEditUnit(m_Data.ATC306.dPeriodAnalysis_X);
		m_wndPeriodAY.SetEditUnit(m_Data.ATC306.dPeriodAnalysis_Y);
		m_wndPeriodCX.SetEditUnit(m_Data.ATC306.dPeriodCode_X);
		m_wndPeriodCY.SetEditUnit(m_Data.ATC306.dPeriodCode_Y);
		m_wndRmfXEdit.SetEditUnit(m_Data.ATC306.dResponseModificationFactor_X);
		m_wndRmfYEdit.SetEditUnit(m_Data.ATC306.dResponseModificationFactor_Y);
		break;
	case 5: // Korean (Arch,2000)
		nSFCode = GetSoilFactorCode(5, m_Data.KS2000.dSoilFactor);
		m_wndSfCombo.SetCurSel(nSFCode);
		nZFCode = GetZoneFactorCode(5, m_Data.KS2000.dZoneFactor);
		m_wndZfCombo.SetCurSel(nZFCode);

		csVal.Format(_T("%g"), m_Data.KS2000.dImportanceFactor);
		m_wndIfCombo.SetWindowText(csVal);

		m_wndPeriodAX.SetEditUnit(m_Data.KS2000.dPeriodAnalysis_X);
		m_wndPeriodAY.SetEditUnit(m_Data.KS2000.dPeriodAnalysis_Y);
		m_wndPeriodCX.SetEditUnit(m_Data.KS2000.dPeriodCode_X);
		m_wndPeriodCY.SetEditUnit(m_Data.KS2000.dPeriodCode_Y);
		m_wndRmfXEdit.SetEditUnit(m_Data.KS2000.dResponseModificationFactor_X);
		m_wndRmfYEdit.SetEditUnit(m_Data.KS2000.dResponseModificationFactor_Y);
		break;
	case 6:
		if (m_Data.JP.dSoilFactor == 0.4) nTemp = 0;
		else if (m_Data.JP.dSoilFactor == 0.6) nTemp = 1;
		else if (m_Data.JP.dSoilFactor == 0.8) nTemp = 2;
		m_wndSPCombo.SetCurSel(nTemp);
		m_wndZone.SetEditUnit(m_Data.JP.dZoneFactor);
		m_wndShearFactor.SetEditUnit(m_Data.JP.dStdShearFactor);
		m_wndPeriodCX_JIS.SetEditUnit(m_Data.JP.dPeriodCode_X);
		m_wndPeriodCY_JIS.SetEditUnit(m_Data.JP.dPeriodCode_Y);
		break;
	case 7:
		m_wndIBCCombo1.SetCurSel(m_Data.IBC2000.nSeismicDesignCategory);
		m_wndIBCCombo2.SetCurSel(m_Data.IBC2000.nSiteClass);
		str.Format(_T("%g"), m_Data.IBC2000.dMSRA1);
		m_wndIBCCombo4.SetWindowText(str);
		str.Format(_T("%g"), m_Data.IBC2000.dMSRAs);
		m_wndIBCCombo3.SetWindowText(str);
		str.Format(_T("%g"), m_Data.IBC2000.dImportanceFactor);
		m_wndIBCCombo5.SetWindowText(str);

		m_wndIBCPeriodCodeX.SetEditUnit(m_Data.IBC2000.dPeriodCode_X);
		m_wndIBCPeriodCodeY.SetEditUnit(m_Data.IBC2000.dPeriodCode_Y);
		m_wndIBCPeriodCalcX.SetEditUnit(m_Data.IBC2000.dPeriodCalculated_X);
		m_wndIBCPeriodCalcY.SetEditUnit(m_Data.IBC2000.dPeriodCalculated_Y);
		m_wndIBCPeriodCoefX.SetEditUnit(m_Data.IBC2000.dResponseModificationFactor_X);
		m_wndIBCPeriodCoefY.SetEditUnit(m_Data.IBC2000.dResponseModificationFactor_Y);
		break;
	case 8:
		m_wndEuroSc.SetCurSel(m_Data.EURO1996.nSoilClass);
		m_wndEuroQ0.SetCurSel(m_Data.EURO1996.nBasicBehavior);
		m_wndEuroKd.SetCurSel(m_Data.EURO1996.nDuctility);
		m_wndEuroKr.SetCurSel(m_Data.EURO1996.nRegularity);
		m_wndEuroKw.SetEditUnit(m_Data.EURO1996.dFailureModeFactor);
		m_wndEuroAlpha.SetEditUnit(m_Data.EURO1996.dAlphaFactor);
		m_wndEuroPeriodX.SetEditUnit(m_Data.EURO1996.dPeriod_X);
		m_wndEuroPeriodY.SetEditUnit(m_Data.EURO1996.dPeriod_Y);
		break;
	case 9: // China2002
		m_wndChSDC.SetCurSel(m_Data.CH2002.nSeisDgnCategory);
		m_wndChSFI.SetCurSel(m_Data.CH2002.nSeisFortification);
		m_wndChSC.SetCurSel(m_Data.CH2002.nSiteClass);
		m_wndChDeltaN.SetCurSel(m_Data.CH2002.nDeltaN);
		m_wndChDamping.SetEditUnit(m_Data.CH2002.dDamping);
		m_wndChER = m_Data.CH2002.nEarthResponse;
		m_wndChAlpha1 = m_Data.CH2002.bAlpha1;
		m_wndChPeriodX.SetEditUnit(m_Data.CH2002.dPeriod_X);
		m_wndChPeriodY.SetEditUnit(m_Data.CH2002.dPeriod_Y);
		break;
	case 10:
		csVal.Format(_T("%g"), m_Data.NBC1995.dZonalVelocityRatio);
		m_wndNbc95ZoneRatio.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.NBC1995.dImportanceFactor);
		m_wndNbc95Importance.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.NBC1995.dFoundationFactor);
		m_wndNbc95Foundation.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.NBC1995.dForceModificationFactor_X);
		m_wndNbc95Rx.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.NBC1995.dForceModificationFactor_Y);
		m_wndNbc95Ry.SetWindowText(csVal);
		m_wndNbc95AcceZone.SetCurSel(m_Data.NBC1995.nAccelerationZone);
		m_wndNbc95VeloZone.SetCurSel(m_Data.NBC1995.nVelocityZone);
		m_wndNbc95TxAnal.SetEditUnit(m_Data.NBC1995.dPeriodAnalysis_X);
		m_wndNbc95TyAnal.SetEditUnit(m_Data.NBC1995.dPeriodAnalysis_Y);
		m_wndNbc95TxCode.SetEditUnit(m_Data.NBC1995.dPeriodCode_X);
		m_wndNbc95TyCode.SetEditUnit(m_Data.NBC1995.dPeriodCode_Y);
		break;
	case 11: // IS2002
		m_wndIs02ZoneCmb.SetCurSel(m_Data.IS2002.nSeismicZone);
		m_wndIs02SoilCmb.SetCurSel(m_Data.IS2002.nSoilType);
		csVal.Format(_T("%g"), m_Data.IS2002.dImportanceFactor);
		m_wndIs02ImportanceCmb.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.IS2002.dResponseReductionFactor_X);
		m_wndIs02RxCmb.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.IS2002.dResponseReductionFactor_Y);
		m_wndIs02RyCmb.SetWindowText(csVal);
		m_wndIs02Damping.SetEditUnit(m_Data.IS2002.dDamping);
		m_wndIs02DampingFactor.SetEditUnit(GetDampingFactorIS2002(m_Data.IS2002.dDamping));
		m_wndIs02Tx.SetEditUnit(m_Data.IS2002.dPeriod_X);
		m_wndIs02Ty.SetEditUnit(m_Data.IS2002.dPeriod_Y);
		break;
	case 12: // Taiwan86
		m_nTaiwan86Period = m_Data.TAIWAN86.nPeriodMethod;
		m_wndTaiwan86ZoneCmb.SetCurSel(m_Data.TAIWAN86.nSeismicZone);
		m_wndTaiwan86SoilCmb.SetCurSel(m_Data.TAIWAN86.nSoilType);
		csVal.Format(_T("%g"), m_Data.TAIWAN86.dImportanceFactor);
		m_wndTaiwan86ImportanceCmb.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.TAIWAN86.dSeisMagnifyFactor);
		m_wndTaiwan86AlphaCmb.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.TAIWAN86.dResponseModifyFactorX);
		m_wndTaiwan86RXCmb.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.TAIWAN86.dResponseModifyFactorY);
		m_wndTaiwan86RYCmb.SetWindowText(csVal);
		m_wndTaiwan86TXAnal.SetEditUnit(m_Data.TAIWAN86.dPeriodAnalX);
		m_wndTaiwan86TYAnal.SetEditUnit(m_Data.TAIWAN86.dPeriodAnalY);
		m_wndTaiwan86TXAppr.SetEditUnit(m_Data.TAIWAN86.dPeriodApprX);
		m_wndTaiwan86TYAppr.SetEditUnit(m_Data.TAIWAN86.dPeriodApprY);
		m_wndTaiwan86TXFinal.SetEditUnit(m_Data.TAIWAN86.dPeriodFinalX);
		m_wndTaiwan86TYFinal.SetEditUnit(m_Data.TAIWAN86.dPeriodFinalY);
		m_wndTaiwan86TXAnal.EnableWindow(m_nTaiwan86Period == 0);
		m_wndTaiwan86TYAnal.EnableWindow(m_nTaiwan86Period == 0);
		break;
	}

	m_wndScaleGX.SetEditUnit(m_Data.ScaleFactor_X);
	m_wndScaleGY.SetEditUnit(m_Data.ScaleFactor_Y);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlEccenX, m_Data.nEccen_X);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlEccenY, m_Data.nEccen_Y);

	MakeItemEx();
	if (m_Data.arAddition.GetSize() > 0)
		m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

BOOL CSeisItemDlg2::Dlg2Data()
{
	UINT nLoadCaseType;
	if (!m_wndLoadCase.GetSelectedLoad(nLoadCaseType, m_Key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
		return FALSE;
	}

	// Addition Data 저장
	CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&> aAddition;
	aAddition.Copy(m_Data.arAddition);

	m_Data.Initialize();

	m_Data.nCodeType = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, m_wndCodeName.GetCurSel()); // v3
	m_wndDescription.GetWindowText(m_Data.Description);

	int nTemp;
	CString str;
	CString strError = _LS(IDS_CMD_HSFC_error); // Error
	CString strErrorR = strError + _T("! "); // Error!

	switch (m_Data.nCodeType)
	{
	case 1: // Korean (Arch, 1991)
		GetSoilFactorValue(1, m_wndSfCombo.GetCurSel(), m_Data.KS1992.dSoilFactor);
		GetZoneFactorValue(1, m_wndZfCombo.GetCurSel(), m_Data.KS1992.dZoneFactor);
		CFormulaEdit::GetEditValue(&m_wndIfCombo, m_Data.KS1992.dImportanceFactor); // 참고
		CFormulaEdit::GetEditValue(&m_wndPeriodAX, m_Data.KS1992.dPeriodAnalysis_X);
		CFormulaEdit::GetEditValue(&m_wndPeriodAY, m_Data.KS1992.dPeriodAnalysis_Y);
		CFormulaEdit::GetEditValue(&m_wndPeriodCX, m_Data.KS1992.dPeriodCode_X);
		CFormulaEdit::GetEditValue(&m_wndPeriodCY, m_Data.KS1992.dPeriodCode_Y);
		CFormulaEdit::GetEditValue(&m_wndRmfXEdit, m_Data.KS1992.dResponseModificationFactor_X);
		CFormulaEdit::GetEditValue(&m_wndRmfYEdit, m_Data.KS1992.dResponseModificationFactor_Y);
		if (m_Data.KS1992.dPeriodAnalysis_X < 0 || m_Data.KS1992.dPeriodAnalysis_Y < 0 ||
			m_Data.KS1992.dPeriodCode_X < 0 || m_Data.KS1992.dPeriodCode_Y < 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
			return FALSE;
		}
		break;
	case 2: // UBC(1991)
		GetSoilFactorValue(2, m_wndSfCombo.GetCurSel(), m_Data.UBC1991.dSoilProfileFactor);
		GetZoneFactorValue(2, m_wndZfCombo.GetCurSel(), m_Data.UBC1991.dZoneFactor);
		CFormulaEdit::GetEditValue(&m_wndIfCombo, m_Data.UBC1991.dImportanceFactor);
		CFormulaEdit::GetEditValue(&m_wndPeriodAX, m_Data.UBC1991.dPeriodAnalysis_X);
		CFormulaEdit::GetEditValue(&m_wndPeriodAY, m_Data.UBC1991.dPeriodAnalysis_Y);
		CFormulaEdit::GetEditValue(&m_wndPeriodCX, m_Data.UBC1991.dPeriodCode_X);
		CFormulaEdit::GetEditValue(&m_wndPeriodCY, m_Data.UBC1991.dPeriodCode_Y);
		CFormulaEdit::GetEditValue(&m_wndRmfXEdit, m_Data.UBC1991.dSystemCoefficient_X);
		CFormulaEdit::GetEditValue(&m_wndRmfYEdit, m_Data.UBC1991.dSystemCoefficient_Y);
		if (m_Data.UBC1991.dPeriodAnalysis_X < 0 || m_Data.UBC1991.dPeriodAnalysis_Y < 0 ||
			m_Data.UBC1991.dPeriodCode_X < 0 || m_Data.UBC1991.dPeriodCode_Y < 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
			return FALSE;
		}
		break;
	case 3: // UBC(1997)
		m_Data.UBC1997.nSoilProfileType = m_wndSptCombo2.GetCurSel() + 1;
		GetZoneFactorValue(3, m_wndZfCombo2.GetCurSel(), m_Data.UBC1997.dSeismicZoneFactor);
		if (m_wndZfCombo2.GetCurSel() == 4)
		{
			m_Data.UBC1997.nSeismicSourceType = m_wndSstCombo2.GetCurSel() + 1;
			CFormulaEdit::GetEditValue(&m_wndCdEdit2, m_Data.UBC1997.dCloseastDistance);
		}
		else
		{
			m_Data.UBC1997.nSeismicSourceType = 1;
			m_Data.UBC1997.dCloseastDistance = 10.0;
		}
		CFormulaEdit::GetEditValue(&m_wndIfCombo2, m_Data.UBC1997.dImportanceFactor);
		CFormulaEdit::GetEditValue(&m_wndPeriodX2, m_Data.UBC1997.dPeriod_X);
		CFormulaEdit::GetEditValue(&m_wndPeriodY2, m_Data.UBC1997.dPeriod_Y);
		CFormulaEdit::GetEditValue(&m_wndRmfXEdit2, m_Data.UBC1997.dResponseModificationFactor_X);
		CFormulaEdit::GetEditValue(&m_wndRmfYEdit2, m_Data.UBC1997.dResponseModificationFactor_Y);
		if (m_Data.UBC1997.dPeriod_X < 0 || m_Data.UBC1997.dPeriod_Y < 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
			return FALSE;
		}
		break;
	case 4: // ATC3-06
		CFormulaEdit::GetEditValue(&m_wndSfEdit, m_Data.ATC306.dSoilProfileCoefficient);
		CFormulaEdit::GetEditValue(&m_wndZfEdit, m_Data.ATC306.dEffectivePeakVelocity);
		CFormulaEdit::GetEditValue(&m_wndPeriodAX, m_Data.ATC306.dPeriodAnalysis_X);
		CFormulaEdit::GetEditValue(&m_wndPeriodAY, m_Data.ATC306.dPeriodAnalysis_Y);
		CFormulaEdit::GetEditValue(&m_wndPeriodCX, m_Data.ATC306.dPeriodCode_X);
		CFormulaEdit::GetEditValue(&m_wndPeriodCY, m_Data.ATC306.dPeriodCode_Y);
		CFormulaEdit::GetEditValue(&m_wndRmfXEdit, m_Data.ATC306.dResponseModificationFactor_X);
		CFormulaEdit::GetEditValue(&m_wndRmfYEdit, m_Data.ATC306.dResponseModificationFactor_Y);
		if (m_Data.ATC306.dPeriodAnalysis_X < 0 || m_Data.ATC306.dPeriodAnalysis_Y < 0 ||
			m_Data.ATC306.dPeriodCode_X < 0 || m_Data.ATC306.dPeriodCode_Y < 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
			return FALSE;
		}
		break;
	case 5: // Korean (Arch, 2000)
		GetSoilFactorValue(5, m_wndSfCombo.GetCurSel(), m_Data.KS2000.dSoilFactor);
		GetZoneFactorValue(5, m_wndZfCombo.GetCurSel(), m_Data.KS2000.dZoneFactor);
		CFormulaEdit::GetEditValue(&m_wndIfCombo, m_Data.KS2000.dImportanceFactor);
		CFormulaEdit::GetEditValue(&m_wndPeriodAX, m_Data.KS2000.dPeriodAnalysis_X);
		CFormulaEdit::GetEditValue(&m_wndPeriodAY, m_Data.KS2000.dPeriodAnalysis_Y);
		CFormulaEdit::GetEditValue(&m_wndPeriodCX, m_Data.KS2000.dPeriodCode_X);
		CFormulaEdit::GetEditValue(&m_wndPeriodCY, m_Data.KS2000.dPeriodCode_Y);
		CFormulaEdit::GetEditValue(&m_wndRmfXEdit, m_Data.KS2000.dResponseModificationFactor_X);
		CFormulaEdit::GetEditValue(&m_wndRmfYEdit, m_Data.KS2000.dResponseModificationFactor_Y);
		if (m_Data.KS2000.dPeriodAnalysis_X < 0 || m_Data.KS2000.dPeriodAnalysis_Y < 0 ||
			m_Data.KS2000.dPeriodCode_X < 0 || m_Data.KS2000.dPeriodCode_Y < 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
			return FALSE;
		}
		break;
	case 6: // Japan2000
		nTemp = m_wndSPCombo.GetCurSel();
		if (nTemp == 0) m_Data.JP.dSoilFactor = 0.4;
		else if (nTemp == 1) m_Data.JP.dSoilFactor = 0.6;
		else if (nTemp == 2) m_Data.JP.dSoilFactor = 0.8;
		CFormulaEdit::GetEditValue(&m_wndZone, m_Data.JP.dZoneFactor);
		CFormulaEdit::GetEditValue(&m_wndShearFactor, m_Data.JP.dStdShearFactor);
		CFormulaEdit::GetEditValue(&m_wndPeriodCX_JIS, m_Data.JP.dPeriodCode_X);
		CFormulaEdit::GetEditValue(&m_wndPeriodCY_JIS, m_Data.JP.dPeriodCode_Y);
		if (m_Data.JP.dPeriodCode_X < 0 || m_Data.JP.dPeriodCode_Y < 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
			return FALSE;
		}
		break;
	case 7: // IBC2000
		m_Data.IBC2000.nSeismicDesignCategory = m_wndIBCCombo1.GetCurSel();
		m_Data.IBC2000.nSiteClass = m_wndIBCCombo2.GetCurSel();
		m_wndIBCCombo4.GetWindowText(str);
		m_Data.IBC2000.dMSRA1 = _tstof(str);
		m_wndIBCCombo3.GetWindowText(str);
		m_Data.IBC2000.dMSRAs = _tstof(str);
		m_wndIBCCombo5.GetWindowText(str);
		m_Data.IBC2000.dImportanceFactor = _tstof(str);
		CFormulaEdit::GetEditValue(&m_wndIBCPeriodCodeX, m_Data.IBC2000.dPeriodCode_X);
		CFormulaEdit::GetEditValue(&m_wndIBCPeriodCodeY, m_Data.IBC2000.dPeriodCode_Y);
		CFormulaEdit::GetEditValue(&m_wndIBCPeriodCalcX, m_Data.IBC2000.dPeriodCalculated_X);
		CFormulaEdit::GetEditValue(&m_wndIBCPeriodCalcY, m_Data.IBC2000.dPeriodCalculated_Y);
		CFormulaEdit::GetEditValue(&m_wndIBCPeriodCoefX, m_Data.IBC2000.dResponseModificationFactor_X);
		CFormulaEdit::GetEditValue(&m_wndIBCPeriodCoefY, m_Data.IBC2000.dResponseModificationFactor_Y);
		if (m_Data.IBC2000.dPeriodCalculated_X < 0 || m_Data.IBC2000.dPeriodCalculated_Y < 0 ||
			m_Data.IBC2000.dPeriodCode_X < 0 || m_Data.IBC2000.dPeriodCode_Y < 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
			return FALSE;
		}
		if (m_Data.IBC2000.dResponseModificationFactor_X <= 0 || m_Data.IBC2000.dResponseModificationFactor_Y <= 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADDD__Response_Modification_Coeff___));
			return FALSE;
		}
		break;
	case 8: // Euro1992
		m_Data.EURO1996.nSoilClass = m_wndEuroSc.GetCurSel();
		m_Data.EURO1996.nBasicBehavior = m_wndEuroQ0.GetCurSel();
		m_Data.EURO1996.nDuctility = m_wndEuroKd.GetCurSel();
		m_Data.EURO1996.nRegularity = m_wndEuroKr.GetCurSel();
		m_Data.EURO1996.dFailureModeFactor = m_wndEuroKw.GetEditValue();
		m_Data.EURO1996.dAlphaFactor = m_wndEuroAlpha.GetEditValue();
		m_Data.EURO1996.dPeriod_X = m_wndEuroPeriodX.GetEditValue();
		m_Data.EURO1996.dPeriod_Y = m_wndEuroPeriodY.GetEditValue();
		if (m_Data.EURO1996.dPeriod_X < 0 || m_Data.EURO1996.dPeriod_Y < 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
			return FALSE;
		}
		break;
	case 9: // China2002
		m_Data.CH2002.nSeisDgnCategory = m_wndChSDC.GetCurSel();
		m_Data.CH2002.nSeisFortification = m_wndChSFI.GetCurSel();
		m_Data.CH2002.nSiteClass = m_wndChSC.GetCurSel();
		m_Data.CH2002.nDeltaN = m_wndChDeltaN.GetCurSel();
		m_Data.CH2002.dDamping = m_wndChDamping.GetEditValue();
		m_Data.CH2002.nEarthResponse = m_wndChER;
		m_Data.CH2002.bAlpha1 = m_wndChAlpha1;
		m_Data.CH2002.dPeriod_X = m_wndChPeriodX.GetEditValue();
		m_Data.CH2002.dPeriod_Y = m_wndChPeriodY.GetEditValue();
		if (m_Data.CH2002.dPeriod_X < 0 || m_Data.CH2002.dPeriod_Y < 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
			return FALSE;
		}
		break;
	case 10: // NBC1995
		m_wndNbc95ZoneRatio.GetWindowText(str);
		m_Data.NBC1995.dZonalVelocityRatio = _tstof(str);
		m_wndNbc95Importance.GetWindowText(str);
		m_Data.NBC1995.dImportanceFactor = _tstof(str);
		m_wndNbc95Foundation.GetWindowText(str);
		m_Data.NBC1995.dFoundationFactor = _tstof(str);
		m_wndNbc95Rx.GetWindowText(str);
		m_Data.NBC1995.dForceModificationFactor_X = _tstof(str);
		m_wndNbc95Ry.GetWindowText(str);
		m_Data.NBC1995.dForceModificationFactor_Y = _tstof(str);
		m_Data.NBC1995.nAccelerationZone = m_wndNbc95AcceZone.GetCurSel();
		m_Data.NBC1995.nVelocityZone = m_wndNbc95VeloZone.GetCurSel();
		m_Data.NBC1995.dPeriodAnalysis_X = m_wndNbc95TxAnal.GetEditValue();
		m_Data.NBC1995.dPeriodAnalysis_Y = m_wndNbc95TyAnal.GetEditValue();
		m_Data.NBC1995.dPeriodCode_X = m_wndNbc95TxCode.GetEditValue();
		m_Data.NBC1995.dPeriodCode_Y = m_wndNbc95TyCode.GetEditValue();
		if (m_Data.NBC1995.dPeriodAnalysis_X < 0 || m_Data.NBC1995.dPeriodAnalysis_Y < 0 ||
			m_Data.NBC1995.dPeriodCode_X < 0 || m_Data.NBC1995.dPeriodCode_Y < 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
			return FALSE;
		}
		if (m_Data.NBC1995.dForceModificationFactor_X <= 0 || m_Data.NBC1995.dForceModificationFactor_Y <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Response_Modification_Fact));
			return FALSE;
		}
		break;
	case 11: // IS2002
		m_wndIs02ImportanceCmb.GetWindowText(str);
		m_Data.IS2002.dImportanceFactor = _tstof(str);
		m_wndIs02RxCmb.GetWindowText(str);
		m_Data.IS2002.dResponseReductionFactor_X = _tstof(str);
		m_wndIs02RyCmb.GetWindowText(str);
		m_Data.IS2002.dResponseReductionFactor_Y = _tstof(str);
		m_Data.IS2002.nSeismicZone = m_wndIs02ZoneCmb.GetCurSel();
		m_Data.IS2002.nSoilType = m_wndIs02SoilCmb.GetCurSel();
		m_Data.IS2002.dDamping = m_wndIs02Damping.GetEditValue();
		m_Data.IS2002.dPeriod_X = m_wndIs02Tx.GetEditValue();
		m_Data.IS2002.dPeriod_Y = m_wndIs02Ty.GetEditValue();
		if (m_Data.IS2002.dPeriod_X < 0 || m_Data.IS2002.dPeriod_X < 0)
		{
			AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
			return FALSE;
		}
		if (m_Data.IS2002.dResponseReductionFactor_X <= 0 || m_Data.IS2002.dResponseReductionFactor_Y <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Response_Reduction_Fact));
			return FALSE;
		}
		break;
	case 12: // Taiwan86
		m_Data.TAIWAN86.nPeriodMethod = m_nTaiwan86Period;
		m_wndTaiwan86ImportanceCmb.GetWindowText(str);
		m_Data.TAIWAN86.dImportanceFactor = _tstof(str);
		m_wndTaiwan86AlphaCmb.GetWindowText(str);
		m_Data.TAIWAN86.dSeisMagnifyFactor = _tstof(str);
		m_wndTaiwan86RXCmb.GetWindowText(str);
		m_Data.TAIWAN86.dResponseModifyFactorX = _tstof(str);
		m_wndTaiwan86RYCmb.GetWindowText(str);
		m_Data.TAIWAN86.dResponseModifyFactorY = _tstof(str);
		m_Data.TAIWAN86.nSeismicZone = m_wndTaiwan86ZoneCmb.GetCurSel();
		m_Data.TAIWAN86.nSoilType = m_wndTaiwan86SoilCmb.GetCurSel();
		m_Data.TAIWAN86.dPeriodAnalX = m_wndTaiwan86TXAnal.GetEditValue();
		m_Data.TAIWAN86.dPeriodAnalY = m_wndTaiwan86TYAnal.GetEditValue();
		m_Data.TAIWAN86.dPeriodApprX = m_wndTaiwan86TXAppr.GetEditValue();
		m_Data.TAIWAN86.dPeriodApprY = m_wndTaiwan86TYAppr.GetEditValue();
		m_Data.TAIWAN86.dPeriodFinalX = m_wndTaiwan86TXFinal.GetEditValue();
		m_Data.TAIWAN86.dPeriodFinalY = m_wndTaiwan86TYFinal.GetEditValue();
		break;
	}

	m_Data.ScaleFactor_X = m_wndScaleGX.GetEditValue();
	m_Data.ScaleFactor_Y = m_wndScaleGY.GetEditValue();

	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlEccenX, nCheck);
	m_Data.nEccen_X = nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlEccenY, nCheck);
	m_Data.nEccen_Y = nCheck;

	// Addition Data 복사
	m_Data.arAddition.Copy(aAddition);

	return TRUE;
}


#define COLCOUNT 3

void CSeisItemDlg2::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Story), _LS(IDS_WG_CMD__ADD2__Add__X), _LS(IDS_WG_CMD__ADD2__Add__Y)};
	int nColWidth[COLCOUNT] = {75, 69, 69};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for (i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CSeisItemDlg2::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_Data.arAddition.GetSize();
	if (nItemCount == 0) return;

	for (int i = 0; i < nItemCount; i++)
	{
		InsertItem(i, m_Data.arAddition[i]);
	}
}

BOOL CSeisItemDlg2::InsertItem(int nPos, T_SEIS_ADDITION& data)
{
	CString str;

	for (int i = 0; i < COLCOUNT; i++)
	{
		str = DataToStr(i, data);
		if (i == 0) m_List.InsertItem(nPos, _T(""));
		m_List.SetItemText(nPos, i, str);
	}

	return TRUE;
}

BOOL CSeisItemDlg2::DeleteItem(int nPos)
{
	if (nPos < 0 || nPos >= m_List.GetItemCount()) return FALSE;
	m_List.DeleteItem(nPos);
	return TRUE;
}

BOOL CSeisItemDlg2::ModifyItem(int nPos, T_SEIS_ADDITION& data)
{
	int nItem = nPos;
	if (nItem != -1)
	{
		CString str;
		for (int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, data);
			m_List.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CSeisItemDlg2::DataToStr(int i, T_SEIS_ADDITION& data)
{
	CString str;

	if (i == 0)
	{
		T_STOR_D stor;
		m_pDoc->m_pAttrCtrl->GetStor(data.KeyStor, stor);
		str = stor.StoryName;
	}
	else if (i == 1) str.Format(_T("%g"), data.AddX);
	else if (i == 2) str.Format(_T("%g"), data.AddY);
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

int CSeisItemDlg2::GetSoilFactorCode(int nCodeType, double dblSF)
{
	switch (nCodeType)
	{
	case 1:
	case 2:
		if (dblSF == 1.0) return 0;
		if (dblSF == 1.2) return 1;
		if (dblSF == 1.5) return 2;
		break;
	case 3:
	case 4:
		break;
	case 5:
		if (dblSF == 1.0) return 0;
		if (dblSF == 1.2) return 1;
		if (dblSF == 1.5) return 2;
		if (dblSF == 2.0) return 3;
		break;
	}
	return 0;
}

int CSeisItemDlg2::GetZoneFactorCode(int nCodeType, double dblZF)
{
	switch (nCodeType)
	{
	case 1:
		if (dblZF == 0.08) return 0;
		if (dblZF == 0.12) return 1;
		break;
	case 2:
	case 3:
		if (dblZF == 0.075) return 0;
		if (dblZF == 0.15) return 1;
		if (dblZF == 0.2) return 2;
		if (dblZF == 0.3) return 3;
		if (dblZF == 0.4) return 4;
		break;
	case 4:
		break;
	case 5:
		if (dblZF == 0.11) return 0;
		if (dblZF == 0.07) return 1;
		break;
	}
	return 0;
}

void CSeisItemDlg2::GetSoilFactorValue(int nCodeType, int nCode, double& dblSF)
{
	switch (nCodeType)
	{
	case 1:
	case 2:
		if (nCode == 0) dblSF = 1.0;
		else if (nCode == 1) dblSF = 1.2;
		else if (nCode == 2) dblSF = 1.5;
		else if (nCode == 3) dblSF = 2.0;
		else dblSF = 1.0;
		break;
	case 3:
	case 4:
		break;
	case 5:
		if (nCode == 0) dblSF = 1.0;
		else if (nCode == 1) dblSF = 1.2;
		else if (nCode == 2) dblSF = 1.5;
		else if (nCode == 3) dblSF = 2.0;
		else dblSF = 1.0;
		break;
	}
}

void CSeisItemDlg2::GetZoneFactorValue(int nCodeType, int nCode, double& dblZF)
{
	switch (nCodeType)
	{
	case 1:
		if (nCode == 0) dblZF = 0.08;
		else if (nCode == 1) dblZF = 0.12;
		else dblZF = 0.08;
		break;
	case 2:
	case 3:
		if (nCode == 0) dblZF = 0.075;
		else if (nCode == 1) dblZF = 0.15;
		else if (nCode == 2) dblZF = 0.2;
		else if (nCode == 3) dblZF = 0.3;
		else if (nCode == 4) dblZF = 0.4;
		else dblZF = 0.075;
		break;
	case 4:
		break;
	case 5:
		if (nCode == 0) dblZF = 0.11;
		else if (nCode == 1) dblZF = 0.07;
		else dblZF = 0.11;
		break;
	}
}

void CSeisItemDlg2::SetZoneFactorCombo(int nCodeType)
{
	if (nCodeType == 1)
	{
		m_wndZfCombo.ResetContent();
		m_wndZfCombo.AddString(_LS(IDS_WG_CMD__ADD2__Area1_0_08_));
		m_wndZfCombo.AddString(_LS(IDS_WG_CMD__ADD2__Area2_0_12_));
	}
	else if (nCodeType == 2)
	{
		m_wndZfCombo.ResetContent();
		m_wndZfCombo.AddString(_T("1 (0.075)"));
		m_wndZfCombo.AddString(_T("2A(0.15)"));
		m_wndZfCombo.AddString(_T("2B(0.20)"));
		m_wndZfCombo.AddString(_T("3 (0.30)"));
		m_wndZfCombo.AddString(_T("4 (0.40)"));
	}
	else if (nCodeType == 5)
	{
		m_wndZfCombo.ResetContent();
		m_wndZfCombo.AddString(_LS(IDS_WG_CMD__ADD2__Area1_0_11_));
		m_wndZfCombo.AddString(_LS(IDS_WG_CMD__ADD2__Area2_0_07_));
	}
}

void CSeisItemDlg2::SetImpoFactorCombo(int nCodeType)
{
	if ((nCodeType == 1) || (nCodeType == 5))
	{
		m_wndIfCombo.ResetContent();
		m_wndIfCombo.AddString(_T("0.8"));
		m_wndIfCombo.AddString(_T("1.0"));
		m_wndIfCombo.AddString(_T("1.2"));
		m_wndIfCombo.AddString(_T("1.5"));
	}
	else if (nCodeType == 2)
	{
		m_wndIfCombo.ResetContent();
		m_wndIfCombo.AddString(_T("1.0"));
		m_wndIfCombo.AddString(_T("1.25"));
	}
}

void CSeisItemDlg2::SetSoilFactorCombo(int nCodeType)
{
	if ((nCodeType == 1) || (nCodeType == 2))
	{
		m_wndSfCombo.ResetContent();
		m_wndSfCombo.AddString(_T("S1(1.0)"));
		m_wndSfCombo.AddString(_T("S2(1.2)"));
		m_wndSfCombo.AddString(_T("S3(1.5)"));
	}
	else if (nCodeType == 5)
	{
		m_wndSfCombo.ResetContent();
		m_wndSfCombo.AddString(_T("S1(1.0)"));
		m_wndSfCombo.AddString(_T("S2(1.2)"));
		m_wndSfCombo.AddString(_T("S3(1.5)"));
		m_wndSfCombo.AddString(_T("S4(2.0)"));
	}
}

double CSeisItemDlg2::GetDampingFactorIS2002(double dDamping)
{
	double dDampingFactor = 0.;
	double aDamping[] = {0., 2., 5., 7., 10., 15., 20., 25., 30.};
	double aFactor[] = {3.20, 1.40, 1.00, 0.90, 0.80, 0.70, 0.60, 0.55, 0.50};

	if (dDamping < 0) dDamping = 0.;
	if (dDamping > 30) dDamping = 30.;
	int Index = 8;
	for (Index = 8; Index >= 0; Index--)
	{
		if (dDamping >= aDamping[Index]) break;
	}
	double dDifRatio = 0.;
	if (Index < 8)
	{
		dDifRatio = (dDamping - aDamping[Index]) / (aDamping[Index + 1] - aDamping[Index]);
		dDampingFactor = aFactor[Index] - (aFactor[Index] - aFactor[Index + 1]) * dDifRatio;
	}
	else dDampingFactor = 0.50;

	return dDampingFactor;
}


BOOL CSeisItemDlg2::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifySeis(m_OldKey, m_Key, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddSeis(m_Key, m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CSeisItemDlg2 message handlers


void CSeisItemDlg2::OnChangeCodeName()
{
	// TODO: Add your control notification handler code here	
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, m_wndCodeName.GetCurSel()); // v3
	if (m_Data.nCodeType == nCodeType) return;

	switch (m_Data.nCodeType)
	{
	case 1: // Korean (Arch, 1992)
		m_wndSfCombo.GetWindowText(m_aKS[0]);
		m_wndZfCombo.GetWindowText(m_aKS[1]);
		m_wndIfCombo.GetWindowText(m_aKS[2]);
		m_wndPeriodAX.GetWindowText(m_aKS[3]);
		m_wndPeriodAY.GetWindowText(m_aKS[4]);
		m_wndPeriodCX.GetWindowText(m_aKS[5]);
		m_wndPeriodCY.GetWindowText(m_aKS[6]);
		m_wndRmfXEdit.GetWindowText(m_aKS[7]);
		m_wndRmfYEdit.GetWindowText(m_aKS[8]);
		break;
	case 2: // UBC(1991)
		m_wndSfCombo.GetWindowText(m_aUBC91[0]);
		m_wndZfCombo.GetWindowText(m_aUBC91[1]);
		m_wndIfCombo.GetWindowText(m_aUBC91[2]);
		m_wndPeriodAX.GetWindowText(m_aUBC91[3]);
		m_wndPeriodAY.GetWindowText(m_aUBC91[4]);
		m_wndPeriodCX.GetWindowText(m_aUBC91[5]);
		m_wndPeriodCY.GetWindowText(m_aUBC91[6]);
		m_wndRmfXEdit.GetWindowText(m_aUBC91[7]);
		m_wndRmfYEdit.GetWindowText(m_aUBC91[8]);
		break;
	case 3: // UBC(1997)
		break;
	case 4: // ATC3-06
		m_wndSfEdit.GetWindowText(m_aATC[0]);
		m_wndZfEdit.GetWindowText(m_aATC[1]);
		m_wndPeriodAX.GetWindowText(m_aATC[2]);
		m_wndPeriodAY.GetWindowText(m_aATC[3]);
		m_wndPeriodCX.GetWindowText(m_aATC[4]);
		m_wndPeriodCY.GetWindowText(m_aATC[5]);
		m_wndRmfXEdit.GetWindowText(m_aATC[6]);
		m_wndRmfYEdit.GetWindowText(m_aATC[7]);
		break;
	case 5: // Korean (Arch, 2000)
		m_wndSfCombo.GetWindowText(m_aKS2000[0]);
		m_wndZfCombo.GetWindowText(m_aKS2000[1]);
		m_wndIfCombo.GetWindowText(m_aKS2000[2]);
		m_wndPeriodAX.GetWindowText(m_aKS2000[3]);
		m_wndPeriodAY.GetWindowText(m_aKS2000[4]);
		m_wndPeriodCX.GetWindowText(m_aKS2000[5]);
		m_wndPeriodCY.GetWindowText(m_aKS2000[6]);
		m_wndRmfXEdit.GetWindowText(m_aKS2000[7]);
		m_wndRmfYEdit.GetWindowText(m_aKS2000[8]);
		break;
	}

	m_Data.nCodeType = nCodeType;
	SetZoneFactorCombo(nCodeType);
	SetImpoFactorCombo(nCodeType);
	SetSoilFactorCombo(nCodeType);

	switch (m_Data.nCodeType)
	{
	case 1: // Korean (Arch, 1992)
		m_wndSfCombo.SelectString(-1, m_aKS[0]);
		m_wndZfCombo.SelectString(-1, m_aKS[1]);
		m_wndIfCombo.SetWindowText(m_aKS[2]);
		m_wndPeriodAX.SetWindowText(m_aKS[3]);
		m_wndPeriodAY.SetWindowText(m_aKS[4]);
		m_wndPeriodCX.SetWindowText(m_aKS[5]);
		m_wndPeriodCY.SetWindowText(m_aKS[6]);
		m_wndRmfXEdit.SetWindowText(m_aKS[7]);
		m_wndRmfYEdit.SetWindowText(m_aKS[8]);
		break;
	case 2: // UBC(1991)
		m_wndSfCombo.SelectString(-1, m_aUBC91[0]);
		m_wndZfCombo.SelectString(-1, m_aUBC91[1]);
		m_wndIfCombo.SetWindowText(m_aUBC91[2]);
		m_wndPeriodAX.SetWindowText(m_aUBC91[3]);
		m_wndPeriodAY.SetWindowText(m_aUBC91[4]);
		m_wndPeriodCX.SetWindowText(m_aUBC91[5]);
		m_wndPeriodCY.SetWindowText(m_aUBC91[6]);
		m_wndRmfXEdit.SetWindowText(m_aUBC91[7]);
		m_wndRmfYEdit.SetWindowText(m_aUBC91[8]);
		break;
	case 3: // UBC(1997)
		break;
	case 4: // ATC3-06
		m_wndSfEdit.SetWindowText(m_aATC[0]);
		m_wndZfEdit.SetWindowText(m_aATC[1]);
		m_wndPeriodAX.SetWindowText(m_aATC[2]);
		m_wndPeriodAY.SetWindowText(m_aATC[3]);
		m_wndPeriodCX.SetWindowText(m_aATC[4]);
		m_wndPeriodCY.SetWindowText(m_aATC[5]);
		m_wndRmfXEdit.SetWindowText(m_aATC[6]);
		m_wndRmfYEdit.SetWindowText(m_aATC[7]);
		break;
	case 5: // Korean (Arch, 2000)
		m_wndSfCombo.SelectString(-1, m_aKS2000[0]);
		m_wndZfCombo.SelectString(-1, m_aKS2000[1]);
		m_wndIfCombo.SetWindowText(m_aKS2000[2]);
		m_wndPeriodAX.SetWindowText(m_aKS2000[3]);
		m_wndPeriodAY.SetWindowText(m_aKS2000[4]);
		m_wndPeriodCX.SetWindowText(m_aKS2000[5]);
		m_wndPeriodCY.SetWindowText(m_aKS2000[6]);
		m_wndRmfXEdit.SetWindowText(m_aKS2000[7]);
		m_wndRmfYEdit.SetWindowText(m_aKS2000[8]);
		break;
	case 8: // Eurocode-8 (1996)		
	case 9: // China2002
	case 10: // NBC(1995)
	case 11: // IS(2002)
	case 12: // Taiwan(1986)
		break;
	}

	CtrlShowHide();

	if (nCodeType == 3) // if UBC 97
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlZone4, m_wndZfCombo2.GetCurSel() == 4);
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlAtcEdit, nCodeType == 4);
		CDlgUtil::CtrlShowHide(this, m_aCtrlSfZf,
		                       (nCodeType != 4 && nCodeType != 6 && nCodeType != 7 &&
			                       nCodeType != 8 && nCodeType != 9 && nCodeType != 10 &&
			                       nCodeType != 11 && nCodeType != 12)); // modified by KYM				
	}
}

void CSeisItemDlg2::OnOK()
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;

	m_bSaveFoldedInfo = TRUE;

	CDialogMove::OnOK();
}

void CSeisItemDlg2::OnCmdLoadcaseBtn()
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));
}

void CSeisItemDlg2::OnCmdApply()
{
	// TODO: Add your control notification handler code here
	if (!ApplyOrOK()) return;

	m_OldKey = m_Key;
}

void CSeisItemDlg2::OnCmdBtnAdd()
{
	// TODO: Add your control notification handler code here
	CWindSeisAddLoadDlg dlg;
	dlg.SetMode(nullptr, (void*)this, nullptr);
	dlg.DoModal();
}

void CSeisItemDlg2::OnCmdBtnDelete()
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_additional_se));
		return;
	}
	DeleteItem(iItem); // List에서 지운다.
	m_Data.arAddition.RemoveAt(iItem);

	if (iItem >= m_Data.arAddition.GetSize()) iItem--;
	if (iItem >= 0)
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CSeisItemDlg2::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
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
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START:
	case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CSeisItemDlg2::UpdateBuffer()
{
	CWaitCursor wait;

	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STOR_K Key, KeyBak;
	T_STOR_D Data, DataBak;
	BOOL bStorChanged = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_STOR_ADD):
			bStorChanged = TRUE;
			break;
		case(UR_STOR_DEL):
			{
				m_pDoc->m_pViewBuff->GetStor(nKey, Key, Data);
				int nCount = m_Data.arAddition.GetSize();
				int i = 0;
				for (i = 0; i < nCount; i++)
					if (m_Data.arAddition[i].KeyStor == Key) break;
				if (i != nCount) m_Data.arAddition.RemoveAt(i);
			}
			bStorChanged = TRUE;
			break;
		case(UR_STOR_MFD):
			{
				m_pDoc->m_pViewBuff->GetStor(nKey, KeyBak, DataBak);
			}
			bStorChanged = TRUE;
			break;
		case(UR_STOR_MFS):
			{
				m_pDoc->m_pViewBuff->GetStor(nKey, Key, Data);
				int nCount = m_Data.arAddition.GetSize();
				int i = 0;
				for (i = 0; i < nCount; i++)
					if (m_Data.arAddition[i].KeyStor == Key) break;
				if (i != nCount) m_Data.arAddition[i].KeyStor = KeyBak;
			}
			bStorChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if (bStorChanged)
	{
		// while문에서는 Data 변경만 Update했다. 이 경우 Story 순서가 바뀌는 경우가
		// 발생할 수 도 있으므로 Sorting을 한다.
		CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&> aAddition;
		CArray<T_STOR_K, T_STOR_K> aKeyList;
		m_pDoc->m_pAttrCtrl->GetStorKeyList(aKeyList);

		int nCount = aKeyList.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			int nCount2 = m_Data.arAddition.GetSize();
			int j = 0;
			for (j = 0; j < nCount2; j++)
				if (aKeyList[i] == m_Data.arAddition[j].KeyStor) break;
			if (j != nCount2)
			{
				aAddition.Add(m_Data.arAddition[j]);
				m_Data.arAddition.RemoveAt(j);
			}
		}
		m_Data.arAddition.Copy(aAddition);
		// List를 갱신한다.
		MakeItemEx();
	}
}


void CSeisItemDlg2::OnCmdBtnProfile()
{
	if (!Dlg2Data()) return;
	CSeisProfileDlg dlg;
	dlg.InitSeisProfile(m_Key, m_Data);
	dlg.DoModal();
}

void CSeisItemDlg2::OnSelchangeCmdZfCombo2()
{
	// TODO: Add your control notification handler code here
	int nZFCode = m_wndZfCombo2.GetCurSel();
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlZone4, nZFCode == 4);
}

void CSeisItemDlg2::OnCmdBtnCalcPeriod() // KOREAN 
{
	CETC_KSPeriodCalc dlg;
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	
	// KOR1992, KOR2000
	dlg.m_dHn_x = m_dHXKOR;
	dlg.m_dHn_y = m_dHYKOR;
	// KOR1992
	dlg.m_dBx = m_dBXKOR1992;
	dlg.m_dBy = m_dBYKOR1992;
	dlg.m_nXMethod = m_nMtdXKOR1992;
	dlg.m_nYMethod = m_nMtdYKOR1992;
	// KOR2000
	dlg.m_dAc_X = m_dAcXKOR2000;
	dlg.m_dAc_Y = m_dAcYKOR2000;
	dlg.m_nXMethod2000 = m_nMtdXKOR2000;
	dlg.m_nYMethod2000 = m_nMtdYKOR2000;

	dlg.m_nCodeType = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, m_wndCodeName.GetCurSel());

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%g"), dlg.m_dPeriod_x);
		cy.Format(_T("%g"), dlg.m_dPeriod_y);
		m_wndPeriodCX.SetWindowText(cx);
		m_wndPeriodCY.SetWindowText(cy);

		// KOR1992, KOR2000
		m_dHXKOR = dlg.m_dHn_x;
		m_dHYKOR = dlg.m_dHn_y;
		// KOR1992
		m_dBXKOR1992 = dlg.m_dBx;
		m_dBYKOR1992 = dlg.m_dBy;
		m_nMtdXKOR1992 = dlg.m_nXMethod;
		m_nMtdYKOR1992 = dlg.m_nYMethod;
		// KOR2000
		m_dAcXKOR2000 = dlg.m_dAc_X;
		m_dAcYKOR2000 = dlg.m_dAc_Y;
		m_nMtdXKOR2000 = dlg.m_nXMethod2000;
		m_nMtdYKOR2000 = dlg.m_nYMethod2000;
	}
}

void CSeisItemDlg2::OnCmdBtnCalcPeriod2() // JAPAN
{
	// TODO: Add your control notification handler code here
	double cx, cy;

	CSeisJISCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	added by KYM

	dlg.m_hx = m_dHXJPN2000;
	dlg.m_hy = m_dHYJPN2000;
	dlg.m_ax = m_dAXJPN2000;
	dlg.m_ay = m_dAYJPN2000;

	if (dlg.DoModal() == IDOK)
	{
		cx = dlg.m_hx * (0.02 + 0.01 * dlg.m_ax);
		cy = dlg.m_hy * (0.02 + 0.01 * dlg.m_ay);
		m_wndPeriodCX_JIS.SetEditUnit(cx);
		m_wndPeriodCY_JIS.SetEditUnit(cy);
		UpdateData(FALSE);

		m_dHXJPN2000 = dlg.m_hx;
		m_dHYJPN2000 = dlg.m_hy;
		m_dAXJPN2000 = dlg.m_ax;
		m_dAYJPN2000 = dlg.m_ay;
	}
}

void CSeisItemDlg2::OnCmdIbcBtnCalcPeriod()
{
	CSeisIBCCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	 added by KYM

	//	m_dHXIBC2000 = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_FT,
	//														0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, m_dHXIBC2000);
	//	m_dHYIBC2000 = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_FT,
	//														0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, m_dHYIBC2000);

	dlg.m_hnX = m_dHXIBC2000;
	dlg.m_hnY = m_dHYIBC2000;
	dlg.m_xrdo = m_nMtdXIBC2000;
	dlg.m_yrdo = m_nMtdYIBC2000;
	dlg.m_nNX = m_nNXIBC2000;
	dlg.m_nNY = m_nNYIBC2000;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%f"), dlg.m_dPeriod_X);
		cy.Format(_T("%f"), dlg.m_dPeriod_Y);
		m_wndIBCPeriodCodeX.SetWindowText(cx);
		m_wndIBCPeriodCodeY.SetWindowText(cy);
		m_dHXIBC2000 = dlg.m_hnX;
		m_dHYIBC2000 = dlg.m_hnY;
		m_nMtdXIBC2000 = dlg.m_xrdo;
		m_nMtdYIBC2000 = dlg.m_yrdo;
	}
}

void CSeisItemDlg2::OnCmdEuroBtnCalcPeriod()
{
	CSeisEuroCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	
	// 현재 dialog의 변수를 child dialog의 변수에 넘겨줌		
	dlg.m_dXHedit = m_dHXEURO1996;
	dlg.m_dYHedit = m_dHYEURO1996;
	dlg.m_dXAcedit = m_dAcXEURO1996;
	dlg.m_dYAcedit = m_dAcYEURO1996;
	dlg.m_dXDedit = m_nDXEURO1996;
	dlg.m_dYDedit = m_nDYEURO1996;
	dlg.m_nXMethod = m_nMtdXEURO1996;
	dlg.m_nYMethod = m_nMtdYEURO1996;

	int nSClass;
	double dTc;

	nSClass = m_wndEuroSc.GetCurSel() + 1;

	switch (nSClass)
	{
	case 1: dTc = 0.40;
		break;
	case 2: dTc = 0.60;
		break;
	case 3: dTc = 0.80;
		break;
	}

	if (dlg.DoModal() == IDOK)
	{
		double dP_x, dP_y;
		dP_x = dlg.m_dPeriod_x;
		dP_y = dlg.m_dPeriod_y;

		if (dP_x > 4 * dTc || dP_x > 2.0 || dP_y > 4 * dTc || dP_y > 2.0) // 기본진동수의 제한사항
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Restriction_of_fundamental_period_violated_));
		}

		CString cx, cy;
		cx.Format(_T("%f"), dlg.m_dPeriod_x);
		cy.Format(_T("%f"), dlg.m_dPeriod_y);
		m_wndEuroPeriodX.SetWindowText(cx);
		m_wndEuroPeriodY.SetWindowText(cy);
		// child dialog의 변수를 현재 dialog의 변수가 받음		
		m_dHXEURO1996 = dlg.m_dXHedit;
		m_dHYEURO1996 = dlg.m_dYHedit;
		m_dAcXEURO1996 = dlg.m_dXAcedit;
		m_dAcYEURO1996 = dlg.m_dYAcedit;
		m_nDXEURO1996 = dlg.m_dXDedit;
		m_nDYEURO1996 = dlg.m_dYDedit;
		m_nMtdXEURO1996 = dlg.m_nXMethod;
		m_nMtdYEURO1996 = dlg.m_nYMethod;
	}
}

void CSeisItemDlg2::OnCmdChinaBtnCalcPeriod()
{
	UpdateData();

	int nUnitChange = 1; // 주기계산 대화상자에서 wind load시에는 단위변환하고, 
	//                       seismic load시에는 단위변환않음
	CSeisChinaCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	

	dlg.m_nUnitChange = nUnitChange;
	dlg.m_dHX = m_dHXCH2001;
	dlg.m_dHY = m_dHYCH2001;
	dlg.m_dBX = m_dBXCH2001;
	dlg.m_dBY = m_dBYCH2001;
	dlg.m_nNX = m_nNXCH2001;
	dlg.m_nNY = m_nNYCH2001;
	dlg.m_nXMethod = m_nMtdXCH2001;
	dlg.m_nYMethod = m_nMtdYCH2001;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%g"), dlg.m_dPeriod_X);
		cy.Format(_T("%g"), dlg.m_dPeriod_Y);
		m_wndChPeriodX.SetWindowText(cx);
		m_wndChPeriodY.SetWindowText(cy);

		m_dHXCH2001 = dlg.m_dHX;
		m_dHYCH2001 = dlg.m_dHY;
		m_dBXCH2001 = dlg.m_dBX;
		m_dBYCH2001 = dlg.m_dBY;
		m_nNXCH2001 = dlg.m_nNX;
		m_nNYCH2001 = dlg.m_nNY;
		m_nMtdXCH2001 = dlg.m_nXMethod;
		m_nMtdYCH2001 = dlg.m_nYMethod;
	}
}


void CSeisItemDlg2::OnCmdNbc95TCalcBtn()
{
	UpdateData();

	CSeisNBC95CalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	

	dlg.m_dHX = m_dHXNBC1995;
	dlg.m_dHY = m_dHYNBC1995;
	dlg.m_dBX = m_dBXNBC1995;
	dlg.m_dBY = m_dBYNBC1995;
	dlg.m_nNX = m_nNXNBC1995;
	dlg.m_nNY = m_nNYNBC1995;
	dlg.m_nXMethod = m_nMtdXNBC1995;
	dlg.m_nYMethod = m_nMtdYNBC1995;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%g"), dlg.m_dPeriod_X);
		cy.Format(_T("%g"), dlg.m_dPeriod_Y);
		m_wndNbc95TxCode.SetWindowText(cx);
		m_wndNbc95TyCode.SetWindowText(cy);

		m_dHXNBC1995 = dlg.m_dHX;
		m_dHYNBC1995 = dlg.m_dHY;
		m_dBXNBC1995 = dlg.m_dBX;
		m_dBYNBC1995 = dlg.m_dBY;
		m_nNXNBC1995 = dlg.m_nNX;
		m_nNYNBC1995 = dlg.m_nNY;
		m_nMtdXNBC1995 = dlg.m_nXMethod;
		m_nMtdYNBC1995 = dlg.m_nYMethod;
	}
}

void CSeisItemDlg2::OnCmdChinaErRdo()
{
	m_Data.CH2002.nEarthResponse = m_wndChER;
	UpdateData();
}

void CSeisItemDlg2::OnCmdChinaAlpha1Chk()
{
	m_Data.CH2002.bAlpha1 = m_wndChAlpha1;
	UpdateData();
}

void CSeisItemDlg2::OnCmdIs02CalcPeriodBtn()
{
	UpdateData();

	CSeisIS02PeriodCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	

	dlg.m_dHX = m_dHXIS2002;
	dlg.m_dHY = m_dHYIS2002;
	dlg.m_dBX = m_dBXIS2002;
	dlg.m_dBY = m_dBYIS2002;
	dlg.m_nXMethod = m_nMtdXIS2002;
	dlg.m_nYMethod = m_nMtdYIS2002;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%g"), dlg.m_dPeriod_X);
		cy.Format(_T("%g"), dlg.m_dPeriod_Y);
		m_wndIs02Tx.SetWindowText(cx);
		m_wndIs02Ty.SetWindowText(cy);

		m_dHXIS2002 = dlg.m_dHX;
		m_dHYIS2002 = dlg.m_dHY;
		m_dBXIS2002 = dlg.m_dBX;
		m_dBYIS2002 = dlg.m_dBY;
		m_nMtdXIS2002 = dlg.m_nXMethod;
		m_nMtdYIS2002 = dlg.m_nYMethod;
	}
}

void CSeisItemDlg2::OnChangeCmdIs02DampingEdit()
{
	double dDamping = m_wndIs02Damping.GetEditValue();
	m_wndIs02DampingFactor.SetEditUnit(GetDampingFactorIS2002(dDamping));
}


void CSeisItemDlg2::OnCmdTaiwan86CalcPeriodBtn()
{
	CSeisTaiwan86PeriodDlg dlg(0, false);
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	

	dlg.m_nMethodX = m_nMtdXTaiwan86;
	dlg.m_nMethodY = m_nMtdYTaiwan86;
	dlg.m_dHnX = m_dHnXTaiwan86;
	dlg.m_dHnY = m_dHnYTaiwan86;
	dlg.m_dAcX = m_dAcXTaiwan86;
	dlg.m_dAcY = m_dAcYTaiwan86;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriodX);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriodY);
		m_wndTaiwan86TXAppr.SetEditUnit(_tstof(cx));
		m_wndTaiwan86TYAppr.SetEditUnit(_tstof(cy));

		m_nMtdXTaiwan86 = dlg.m_nMethodX;
		m_nMtdYTaiwan86 = dlg.m_nMethodY;
		m_dHnXTaiwan86 = dlg.m_dHnX;
		m_dHnYTaiwan86 = dlg.m_dHnY;
		m_dAcXTaiwan86 = dlg.m_dAcX;
		m_dAcYTaiwan86 = dlg.m_dAcY;
	}
}

void CSeisItemDlg2::OnCmdTaiwan86PeriodRdo()
{
	UpdateData(TRUE);
	m_wndTaiwan86TXAnal.EnableWindow(m_nTaiwan86Period == 0);
	m_wndTaiwan86TYAnal.EnableWindow(m_nTaiwan86Period == 0);
	OnChangeTaiwan86PeriodEdit();
	UpdateData(FALSE);
}

void CSeisItemDlg2::OnChangeTaiwan86PeriodEdit()
{
	double dTXAnal = m_wndTaiwan86TXAnal.GetEditValue();
	double dTXAppr = m_wndTaiwan86TXAppr.GetEditValue();
	double dTYAnal = m_wndTaiwan86TYAnal.GetEditValue();
	double dTYAppr = m_wndTaiwan86TYAppr.GetEditValue();

	double dTXFinal = m_pDoc->m_pAttrCtrl->CalcFundamentalPeriod(12, m_nTaiwan86Period, dTXAnal, dTXAppr);
	double dTYFinal = m_pDoc->m_pAttrCtrl->CalcFundamentalPeriod(12, m_nTaiwan86Period, dTYAnal, dTYAppr);

	CString str;
	str.Format(_T("%-10.4g"), dTXFinal);
	m_wndTaiwan86TXFinal.SetWindowText(str);
	str.Format(_T("%-10.4g"), dTYFinal);
	m_wndTaiwan86TYFinal.SetWindowText(str);
}
