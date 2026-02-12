// CMSectItemCon.cpp : implementation file
//
// 2002. 10. 28  Modified by TAE - composite-I, Composite-T형 추가 
// 2003. 02. 14  Modified by TAE - Composite-I형이 PSC_Mid에서 PSC_I형으로 수정

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemCon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\Db_DllBaseManager.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\CMSectImportSPC.h"

#include "CMSecPageStiffDlg.h"
#include "CMConcrSteelMatlDlg.h"
#include "CMCompoMatDlg.h"
#include "CMSectItmePSCView.h"
#include "CCMSectItemPSCDefine.h"
#include "CMSectOffsetDlg.h"
#include "CMSelectSect.h"
#include "CMSectItemPSCImport.h"
#include "CMSectStiffenerDlg.h"
#include "CMSectSODDeckStiffenerDlg.h"

#ifdef _MGEN_CH
#include "..\wg_db\MITC_CommonTool.h"
#endif

#include <iostream>
#include <fstream>

#define D_SECT_CONSTRUCTION_DEF_SHAPE   D_SECT_SHAPE_REG_H

#define D_DEF_CALC_STIFF_OPT 1  // nCalcStiffOpt

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemCon dialog
#define   CDialog   CCMSectItemBase

CCMSectItemCon::CCMSectItemCon(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemCon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemCon)
	//}}AFX_DATA_INIT
#if defined(_MGEN)
	m_nCurType = 2;
#else
	m_nCurType = 1;
#endif
	m_pViewer = NULL;

	CSectUtil::GetOffsetNameList(m_aOffsetName);

	SetRcIDAr();

	m_PscValueSectK = 0;

	m_CurSectID = 0;
	m_aImportID.RemoveAll();
	m_aImportSect.RemoveAll();

	m_bReCalc = FALSE;
	m_bSymAutoCalc = FALSE;
}

void CCMSectItemCon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemCon)
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT, m_txtOffset);
	DDX_Control(pDX, IDC_CMD_SP_SECTYPE_COMBO, m_cboSecType);
	DDX_Control(pDX, IDC_CMD_SP_ID_UNIT9, m_unit9);
	DDX_Control(pDX, IDC_CMD_SP_ID_UNIT8, m_unit8);
	DDX_Control(pDX, IDC_CMD_SP_ID_UNIT7, m_unit7);
	DDX_Control(pDX, IDC_CMD_SP_ID_UNIT6, m_unit6);
	DDX_Control(pDX, IDC_CMD_SP_ID_UNIT5, m_unit5);
	DDX_Control(pDX, IDC_CMD_SP_ID_UNIT4, m_unit4);
	DDX_Control(pDX, IDC_CMD_SP_ID_UNIT3, m_unit3);
	DDX_Control(pDX, IDC_CMD_SP_ID_UNIT2, m_unit2);
	DDX_Control(pDX, IDC_CMD_SP_ID_UNIT12, m_unit12);
	DDX_Control(pDX, IDC_CMD_SP_ID_UNIT11, m_unit11);
	DDX_Control(pDX, IDC_CMD_SP_ID_UNIT1, m_unit1);
	DDX_Control(pDX, IDC_CMD_SP_ID_BF3_UNT, m_untBf3);
	DDX_Control(pDX, IDC_CMD_SP_ID_TW, m_tw);
	DDX_Control(pDX, IDC_CMD_SP_ID_TR2, m_Tr2);
	DDX_Control(pDX, IDC_CMD_SP_ID_TR1, m_Tr1);
	DDX_Control(pDX, IDC_CMD_SP_ID_TF2, m_Tf2);
	DDX_Control(pDX, IDC_CMD_SP_ID_TF1, m_Tf1);
	DDX_Control(pDX, IDC_CMD_SP_ID_TC, m_Tc);
	DDX_Control(pDX, IDC_CMD_SECT_TC_EDT, m_StlGirderTc);
	DDX_Control(pDX, IDC_CMD_SP_ID_SWIDTH, m_swidth);
	DDX_Control(pDX, IDC_CMD_SP_ID_HW, m_Hw);
	DDX_Control(pDX, IDC_CMD_SP_ID_HR2, m_Hr2);
	DDX_Control(pDX, IDC_CMD_SP_ID_HR1, m_Hr1);
	DDX_Control(pDX, IDC_CMD_SP_ID_HH, m_Hh);
	DDX_Control(pDX, IDC_CMD_SECT_HH_EDT, m_StlGirderHh);
	DDX_Control(pDX, IDC_CMD_SP_ID_ESEC, m_ESEC);
	DDX_Control(pDX, IDC_CMD_SP_ID_DSDC, m_DSDC);
	DDX_Control(pDX, IDC_CMD_SP_ID_PS, m_PS);
	DDX_Control(pDX, IDC_CMD_SP_ID_PC, m_PC);
	DDX_Control(pDX, IDC_CMD_SP_ID_TSTC, m_TSTC);
	DDX_Control(pDX, IDC_CMD_SP_ID_CTC, m_CTC);
	DDX_Control(pDX, IDC_CMD_SP_ID_BF2, m_Bf2);
	DDX_Control(pDX, IDC_CMD_SP_ID_BF3_EDT, m_Bf3);
	DDX_Control(pDX, IDC_CMD_SP_ID_TFP_EDT, m_tfp);
	DDX_Control(pDX, IDC_CMD_SP_ID_BC, m_Bc);
	DDX_Control(pDX, IDC_CMD_SECT_BC_EDT, m_StlGirderBc);
	DDX_Control(pDX, IDC_CMD_SP_ID_BF1, m_Bf1);
	DDX_Control(pDX, IDC_CMD_SP_ID_B2, m_B2);
	DDX_Control(pDX, IDC_CMD_SP_ID_B1, m_B1);
	//DDX_Control(pDX, IDC_CMD_SP_ID_GNUM_SPIN, m_gnum_spin);  
	//DDX_Control(pDX, IDC_CMD_SP_ID_N1_SPIN, m_N1_spin);
	//DDX_Control(pDX, IDC_CMD_SP_ID_N2_SPIN, m_N2_spin);
	DDX_Control(pDX, IDC_CMD_SP_ID_SNAME2, m_wndSecondName);
	DDX_Control(pDX, IDC_CMD_SP_ID_SNAME1, m_wndFirstName);
	DDX_Control(pDX, IDC_CMD_SP_ID_ID, m_wndID);
	DDX_Control(pDX, IDC_CMD_SP_ID_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_SP_ID_GNUM, m_gnum);
	DDX_Control(pDX, IDC_CMD_SP_ID_N1, m_N1);
	DDX_Control(pDX, IDC_CMD_SP_ID_N2, m_N2);
	DDX_Control(pDX, IDC_CMD_SHEAR_DEFORM, m_chkShearDeform);
	DDX_Control(pDX, IDC_CMD_WARPING_EFFECT_CHK, m_chkWarpingEffect);
	DDX_Control(pDX, IDC_CMD_HUMBLY_CHK, m_chkHumbly);
	DDX_Control(pDX, IDC_CMD_HUMBLY_CHK_BF, m_chkHumblyBefore);
	DDX_Control(pDX, IDC_CMD_HUMBLY_CHK_AF, m_chkHumblyAfter);
	DDX_Control(pDX, IDC_GENERAL_PSC_DESIGN_CHK, m_chkPSCDesign);
	DDX_Control(pDX, IDC_GENERAL_PSC_DESIGN_SHEAR_EDT, m_edtPSCShear);
	DDX_Control(pDX, IDC_GENERAL_PSC_DESIGN_SHEAR_UNT, m_untPSCShear);
	DDX_Control(pDX, IDC_CMD_MAT1_EDT, m_edtMat1); // MNET:2582 20061129 mylee
	DDX_Control(pDX, IDC_CMD_MAT2_EDT, m_edtMat2);
	DDX_Control(pDX, IDC_CMD_MAT_CHK, m_chkMat);
	DDX_Control(pDX, IDC_GENERAL_PARTSEL_CMB, m_cmbPart); // MNET:3022 20080103 mylee
	DDX_Control(pDX, IDC_GENERAL_PARTBEFORE_CMB, m_cmbBeforePart);
	DDX_Control(pDX, IDC_CMD_IMPORT_EDIT, m_editPath);
	DDX_Control(pDX, IDC_CMD_SECT_SG_EDT,  m_edtStlGirderSg);
	DDX_Control(pDX, IDC_CMD_SECT_TOP_EDT, m_edtStlGirderTop);  
	DDX_Control(pDX, IDC_CMD_SECT_BOT_EDT, m_edtStlGirderBot);
	DDX_Control(pDX, IDC_CMD_SECT_HH_UNT,  m_untStlGirderSlab);
	DDX_Control(pDX, IDC_CMD_SECT_BOT_UNT, m_untStlGirderBot);
	DDX_Control(pDX, IDC_CMD_SECT_B1_EDT,  m_edtStlGirderB1);
	DDX_Control(pDX, IDC_CMD_SECT_B2_EDT,  m_edtStlGirderB2);
	DDX_Control(pDX, IDC_CMD_SECT_B3_EDT,  m_edtStlGirderB3);
	DDX_Control(pDX, IDC_CMD_SECT_B3_UNT,  m_untStlGirderB3);
	DDX_Control(pDX, IDC_CMD_SECT_B4_EDT,  m_edtStlGirderB4);
	DDX_Control(pDX, IDC_CMD_SECT_B5_EDT,  m_edtStlGirderB5);
	DDX_Control(pDX, IDC_CMD_SECT_B6_EDT,  m_edtStlGirderB6);
	DDX_Control(pDX, IDC_CMD_SECT_B6_UNT,  m_untStlGirderB6);
	DDX_Control(pDX, IDC_CMD_SECT_H_EDT,   m_edtStlGirderH);  
	DDX_Control(pDX, IDC_CMD_SECT_T1_EDT,  m_edtStlGirdert1);  
	DDX_Control(pDX, IDC_CMD_SECT_T2_EDT,  m_edtStlGirdert2);
	DDX_Control(pDX, IDC_CMD_SECT_T2_UNT,  m_untStlGirdert2);
	DDX_Control(pDX, IDC_CMD_SECT_TW1_EDT, m_edtStlGirdertw1);  
	DDX_Control(pDX, IDC_CMD_SECT_TW2_EDT, m_edtStlGirdertw2);
	DDX_Control(pDX, IDC_CMD_SECT_BF1_UNT, m_untStlGirdertBf1);
	DDX_Control(pDX, IDC_CMD_SECT_BF2_UNT, m_untStlGirderBf2);
	DDX_Control(pDX, IDC_CMD_SECT_BF1_EDT, m_edtStlGirderbf1);  
	DDX_Control(pDX, IDC_CMD_SECT_BF2_EDT, m_edtStlGirderbf2);
	DDX_Control(pDX, IDC_CMD_SECT_TFP_EDT, m_edtStlGirdertfp);
	DDX_Check  (pDX, IDC_CMD_SECT_SYM_AUTO_CALC_CHK,   m_bSymAutoCalc);

	DDX_Control(pDX, IDC_CMD_SP_ID_TABLE1, m_wndGridFirst);
	DDX_Control(pDX, IDC_CMD_SP_ID_TABLE2, m_wndGridSecond);
	DDX_Control(pDX, IDC_CMD_SP_CI_TABLE, m_wndGridCI);
	DDX_Control(pDX, IDC_CMD_SP_CT_TABLE, m_wndGridCT);
	DDX_Control(pDX, IDC_CMD_SP_ID_GRID, m_wndGridGeneral);

	DDX_Control(pDX, IDC_CMD_SP_ID_PREVIEW3, m_ImgViewerPREVIEW3);
	DDX_Control(pDX, IDC_CMD_SP_ID_PREVIEW4, m_ImgViewerPREVIEW4);
	DDX_Control(pDX, IDC_CMD_SP_CI_PREVIEW1, m_ImgViewerCI_PREVIEW1);
	DDX_Control(pDX, IDC_CMD_SP_CT_PREVIEW1, m_ImgViewerCT_PREVIEW1);
	DDX_Control(pDX, IDC_CMD_PSC_PREVIEW   , m_ImgViewerPSC_PREVIEW);
	DDX_Control(pDX, IDC_CMD_SP_ID_PREVIEW7, m_ImgViewerPREVIEW7);
	DDX_Control(pDX, IDC_CMD_TUB_PREVIEW1  , m_ImgViewerTUB_PREVIEW1);
	DDX_Control(pDX, IDC_CMD_SECT_B_GUIDE_BMP, m_ImgViewerB_GUIDE_BMP);
	DDX_Control(pDX, IDC_CMD_SECT_I_GUIDE_BMP, m_ImgViewerI_GUIDE_BMP);
	DDX_Control(pDX, IDC_CMD_SECT_TUB_GUIDE_BMP, m_ImgViewerTUB_GUIDE_BMP);



	DDX_Control(pDX, IDC_CMD_SECT_ATYPE_COMBO, m_cbxAType);
	DDX_Control(pDX, IDC_CMD_SP_ID_BC3, m_Sg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSectItemCon, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemCon)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_SNAME1, OnChangeFirstName)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_SNAME2, OnChangeSecondName)
	ON_EN_CHANGE(IDC_CMD_SP_ID_B1, OnChangeCmdSpIdB1)
	ON_EN_CHANGE(IDC_CMD_SP_ID_B2, OnChangeCmdSpIdB2)
	ON_EN_CHANGE(IDC_CMD_SP_ID_BC, OnChangeCmdSpIdBc)
	ON_EN_CHANGE(IDC_CMD_SP_ID_BF1, OnChangeCmdSpIdBf1)
	ON_EN_CHANGE(IDC_CMD_SP_ID_BF2, OnChangeCmdSpIdBf2)
	ON_EN_CHANGE(IDC_CMD_SP_ID_BF3_EDT, OnChangeCmdSpIdBf3)
	ON_EN_CHANGE(IDC_CMD_SP_ID_TFP_EDT, OnChangeCmdSpIdtfp)
	ON_EN_CHANGE(IDC_CMD_SP_ID_HH, OnChangeCmdSpIdHh)
	ON_EN_CHANGE(IDC_CMD_SP_ID_HR1, OnChangeCmdSpIdHr1)
	ON_EN_CHANGE(IDC_CMD_SP_ID_HR2, OnChangeCmdSpIdHr2)
	ON_EN_CHANGE(IDC_CMD_SP_ID_HW, OnChangeCmdSpIdHw)
	ON_EN_CHANGE(IDC_CMD_SP_ID_TC, OnChangeCmdSpIdTc)
	ON_EN_CHANGE(IDC_CMD_SP_ID_TF1, OnChangeCmdSpIdTf1)
	ON_EN_CHANGE(IDC_CMD_SP_ID_TF2, OnChangeCmdSpIdTf2)
	ON_EN_CHANGE(IDC_CMD_SP_ID_TR1, OnChangeCmdSpIdTr1)
	ON_EN_CHANGE(IDC_CMD_SP_ID_TR2, OnChangeCmdSpIdTr2)
	ON_EN_CHANGE(IDC_CMD_SP_ID_TW, OnChangeCmdSpIdTw)
	ON_EN_CHANGE(IDC_CMD_SP_ID_N1, OnChangeCmdN1)
	ON_EN_CHANGE(IDC_CMD_SP_ID_N2, OnChangeCmdSpIdN2)
	ON_BN_CLICKED(IDC_MATERIAL_BTN, OnMaterialBtn)
	ON_CBN_SELCHANGE(IDC_CMD_SP_SECTYPE_COMBO, OnSelchangeCmdSpSectypeCombo)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_EN_CHANGE(IDC_CMD_SP_ID_SWIDTH, OnChangeCmdSpIdSwidth)
	ON_EN_CHANGE(IDC_CMD_SP_ID_ESEC, OnChangeCmdSpIdEsec)
	ON_EN_CHANGE(IDC_CMD_SP_ID_DSDC, OnChangeCmdSpIdDsdc)
	ON_EN_CHANGE(IDC_CMD_SP_ID_PS, OnChangeCmdSpIdPs)
	ON_EN_CHANGE(IDC_CMD_SP_ID_PC, OnChangeCmdSpIdPc)
	ON_EN_CHANGE(IDC_CMD_SP_ID_TSTC, OnChangeCmdSpIdTstc)
	ON_EN_CHANGE(IDC_CMD_SP_ID_CTC, OnChangeCmdSpIdCtc)
	ON_EN_CHANGE(IDC_CMD_SP_ID_GNUM, OnChangeCmdSpIdGnum)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_BN_CLICKED(IDC_CMD_MAT_CHK, OnCmdMultiModulusCheck)
	ON_BN_CLICKED(IDC_PSC_IMP_BTN, OnCmdPSCImportBtn)
	ON_BN_CLICKED(IDC_CMD_IMPORT_BTN, OnCmdImportBtn)
	ON_BN_CLICKED(IDC_CMD_CALC_SECT_PROP_BTN, OnCmdCalcSectPropBtn)
	ON_BN_CLICKED(IDC_CMD_SEL_BTN, OnCmdSelectBtn)
	ON_CBN_SELCHANGE(IDC_GENERAL_PARTSEL_CMB, OnSelchangeCmdPartSel)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_CHK, OnChkWarpingEffect)
	ON_BN_CLICKED(IDC_CMD_HUMBLY_CHK, OnChkhumbly)
	ON_BN_CLICKED(IDC_CMD_HUMBLY_CHK_BF, OnChkhumblyBefore)
	ON_BN_CLICKED(IDC_CMD_HUMBLY_CHK_AF, OnChkhumblyAfter)
	ON_BN_CLICKED(IDC_GENERAL_PSC_DESIGN_CHK, OnChkPSCDesign)
	ON_BN_CLICKED(IDC_CMD_STIFFENER_BTN, OnCmdStiffenerBtn)

	ON_EN_CHANGE(IDC_CMD_SECT_BC_EDT, OnChangeBc)
	ON_EN_CHANGE(IDC_CMD_SECT_TC_EDT, OnChangetc)
	ON_EN_CHANGE(IDC_CMD_SECT_HH_EDT, OnChangeHh)
	ON_BN_CLICKED(IDC_CMD_SECT_SYM_AUTO_CALC_CHK, OnSymmetricAutoCalcBtn)
	ON_EN_CHANGE(IDC_CMD_SECT_SG_EDT, OnChangeSg)
	ON_EN_CHANGE(IDC_CMD_SECT_TOP_EDT, OnChangeTop)
	ON_EN_CHANGE(IDC_CMD_SECT_BOT_EDT, OnChangeBot)
	ON_EN_CHANGE(IDC_CMD_SECT_B1_EDT, OnChangeB1)
	ON_EN_CHANGE(IDC_CMD_SECT_B2_EDT, OnChangeB2)
	ON_EN_CHANGE(IDC_CMD_SECT_B3_EDT, OnChangeB3)
	ON_EN_CHANGE(IDC_CMD_SECT_B4_EDT, OnChangeB4)
	ON_EN_CHANGE(IDC_CMD_SECT_B5_EDT, OnChangeB5)
	ON_EN_CHANGE(IDC_CMD_SECT_B6_EDT, OnChangeB6)
	ON_EN_CHANGE(IDC_CMD_SECT_H_EDT, OnChangeH)
	ON_EN_CHANGE(IDC_CMD_SECT_T1_EDT, OnChanget1)
	ON_EN_CHANGE(IDC_CMD_SECT_T2_EDT, OnChanget2)
	ON_EN_CHANGE(IDC_CMD_SECT_TW1_EDT, OnChangetw1)
	ON_EN_CHANGE(IDC_CMD_SECT_TW2_EDT, OnChangetw2)
	ON_EN_CHANGE(IDC_CMD_SECT_BF1_EDT, OnChangeBf1)
	ON_EN_CHANGE(IDC_CMD_SECT_BF2_EDT, OnChangeBf2)
	ON_EN_CHANGE(IDC_CMD_SECT_TFP_EDT, OnChangetfp)
	ON_BN_CLICKED(IDC_CMD_STIFFENER_BTN2, OnStiffenerBtn)
	ON_EN_CHANGE(IDC_CMD_SP_ID_BC3, OnChangeCmdSpIdSg)

	ON_CBN_SELCHANGE(IDC_CMD_SECT_ATYPE_COMBO, OnChangeSelAType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Interfaces
/////////////////////////////////////////////////////////////////////////////

// 부모 Page에서 OK 버튼 눌러졌을 때 호출
BOOL CCMSectItemCon::OnOKPublic()
{
	return SaveData();
}

// 부모 페이지에서 Apply 버튼 눌러졌을 때 호출
BOOL CCMSectItemCon::OnApplyPublic()
{
	if (!SaveData()) return FALSE;
	if (!m_bModify)  // modify 면 그냥 두고 add면 재 초기화 
	{
		// ID만 수정한다.
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumSect(m_Key);
		ShowKey2Dlg();
		m_wndFirstName.SetFocus();
	}
	else
	{
		m_OrgKey = m_Key;  // Change Origin Key
	}
	return TRUE;
}

// 강성 계산 결과를 보여준다.
void CCMSectItemCon::ShowCalcResult()
{
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}

	CCMStiffDlg dlg(m_pDoc, &m_Data, NULL, this);
	dlg.DoModal();
}

void CCMSectItemCon::DisplayOffsetPoint()
{
	if (m_Data.SectBefore.nOffsetCenter == 0) 
		OnCmdSectCentroidBtn();
	else
	{
		SetOffsetData(&m_Data.SectBefore, &m_DataBox.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataI.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataCI.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataCT.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataUser.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataPC.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataGeneral.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataTub.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataStlGirderBox.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataStlGirderI.SectBefore);  
		SetOffsetData(&m_Data.SectBefore, &m_DataStlGirderTub.SectBefore);
		m_wndSecViewFirst.Invalidate();	
		m_wndSecViewSecond.Invalidate();	
		m_wndSecViewBox.Invalidate();	
		m_wndSecViewI.Invalidate();	
		m_wndSecViewCI.Invalidate();	
		m_wndSecViewCT.Invalidate();	
		m_wndSecViewPC.Invalidate();
		m_wndSecViewGeneral.Invalidate();
		m_wndSecViewTub.Invalidate();
		m_wndSecViewStlGirderBox.Invalidate();	
		m_wndSecViewStlGirderI.Invalidate();	
		m_wndSecViewStlGirderTub.Invalidate();	
	}
}

void CCMSectItemCon::SetNewID(unsigned nNewID)
{
	m_Key = nNewID;
	ShowKey2Dlg();
}

/////////////////////////////////////////////////////////////////////////////
// Implementations
/////////////////////////////////////////////////////////////////////////////

void CCMSectItemCon::SetRcIDAr()
{
	m_aCtrlUser.RemoveAll();
	m_aCtrlUser.Add(IDC_STATIC1);
	m_aCtrlUser.Add(IDC_STATIC2);
	m_aCtrlUser.Add(IDC_BEFORE_GROUP);
	m_aCtrlUser.Add(IDC_AFTER_GROUP);
	m_aCtrlUser.Add(IDC_CMD_SP_ID_SNAME1);
	m_aCtrlUser.Add(IDC_CMD_SP_ID_SNAME2);
	m_aCtrlUser.Add(IDC_CMD_SP_ID_TABLE1);
	m_aCtrlUser.Add(IDC_CMD_SP_ID_TABLE2);

	m_PreViewUser.RemoveAll();
	m_PreViewUser.Add(IDC_CMD_SP_ID_PREVIEW1);
	m_PreViewUser.Add(IDC_CMD_SP_ID_PREVIEW2);

	m_aCtrlGirder.RemoveAll();
	m_aCtrlGirder.Add(IDC_BOX_GROUP2);
	m_aCtrlGirder.Add(IDC_STATIC9);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_HW);
	m_aCtrlGirder.Add(IDC_STATIC10);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_TW);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_UNIT6);
	m_aCtrlGirder.Add(IDC_STATIC11);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_B1);
	m_aCtrlGirder.Add(IDC_STATIC12);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_B2);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_UNIT7);
	m_aCtrlGirder.Add(IDC_STATIC13);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_BF1);
	m_aCtrlGirder.Add(IDC_STATIC14);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_BF2);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_UNIT8);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_BF3_STC);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_BF3_EDT);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_BF3_UNT);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_TFP_EDT);
	m_aCtrlGirder.Add(IDC_STATIC15);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_TF1);
	m_aCtrlGirder.Add(IDC_STATIC16);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_TFP_STC);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_TF2);
	m_aCtrlGirder.Add(IDC_CMD_SP_ID_UNIT9);
	m_aCtrlGirder.Add(IDC_CMD_STIFFENER_BTN);
	//   m_aCtrlGirder.Add(IDC_STATIC17);
	//   m_aCtrlGirder.Add(IDC_CMD_SP_ID_N1);
	//   m_aCtrlGirder.Add(IDC_CMD_SP_ID_N1_SPIN);
	//   m_aCtrlGirder.Add(IDC_STATIC18);
	//   m_aCtrlGirder.Add(IDC_CMD_SP_ID_N2);
	//   m_aCtrlGirder.Add(IDC_CMD_SP_ID_N2_SPIN);
	//   m_aCtrlGirder.Add(IDC_STATIC19);
	//   m_aCtrlGirder.Add(IDC_CMD_SP_ID_HR1);
	//   m_aCtrlGirder.Add(IDC_STATIC20);
	//   m_aCtrlGirder.Add(IDC_CMD_SP_ID_HR2);
	//   m_aCtrlGirder.Add(IDC_CMD_SP_ID_UNIT11);
	//   m_aCtrlGirder.Add(IDC_STATIC21);
	//   m_aCtrlGirder.Add(IDC_CMD_SP_ID_TR1);
	//   m_aCtrlGirder.Add(IDC_STATIC22);
	//   m_aCtrlGirder.Add(IDC_CMD_SP_ID_TR2);
	//   m_aCtrlGirder.Add(IDC_CMD_SP_ID_UNIT12);

	m_aCtrlMat.RemoveAll();
	m_aCtrlMat.Add(IDC_BOX_GROUP3);
	m_aCtrlMat.Add(IDC_MATERIAL_BTN);
	m_aCtrlMat.Add(IDC_STATIC23);
	m_aCtrlMat.Add(IDC_CMD_SP_ID_ESEC);
	m_aCtrlMat.Add(IDC_STATIC24);
	m_aCtrlMat.Add(IDC_CMD_SP_ID_DSDC);
	m_aCtrlMat.Add(IDC_STATIC25); // MNET:XXXX 20070307 mylee - poisson ratio add
	m_aCtrlMat.Add(IDC_CMD_SP_ID_PS);
	m_aCtrlMat.Add(IDC_STATIC26);
	m_aCtrlMat.Add(IDC_CMD_SP_ID_PC);
	m_aCtrlMat.Add(IDC_STATIC27);
	m_aCtrlMat.Add(IDC_CMD_SP_ID_TSTC);
	m_aCtrlMat.Add(IDC_CMD_MAT_CHK); // MNET:2582 20061129 mylee
	m_aCtrlMat.Add(IDC_CMD_MAT1_STC);
	m_aCtrlMat.Add(IDC_CMD_MAT2_STC);
	m_aCtrlMat.Add(IDC_CMD_MAT1_EDT);
	m_aCtrlMat.Add(IDC_CMD_MAT2_EDT);

	m_aCtrlMulti.RemoveAll();
	m_aCtrlMulti.Add(IDC_CMD_MAT_CHK); // MNET:2582 20061129 mylee
	m_aCtrlMulti.Add(IDC_CMD_MAT1_STC);
	m_aCtrlMulti.Add(IDC_CMD_MAT2_STC);
	m_aCtrlMulti.Add(IDC_CMD_MAT1_EDT);
	m_aCtrlMulti.Add(IDC_CMD_MAT2_EDT);

	m_aCtrlBox.RemoveAll();
	m_aCtrlBox.Add(IDC_STATIC3);
	m_aCtrlBox.Add(IDC_STATIC4);
	m_aCtrlBox.Add(IDC_STATIC5);
	m_aCtrlBox.Add(IDC_STATIC6);
	m_aCtrlBox.Add(IDC_STATIC7);
	m_aCtrlBox.Add(IDC_STATIC8);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_UNIT1);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_UNIT2);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_UNIT3);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_UNIT4);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_UNIT5);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_SWIDTH);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_GNUM);
	///m_aCtrlBox.Add(IDC_CMD_SP_ID_GNUM_SPIN);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_CTC);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_BC);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_TC);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_HH);
	m_aCtrlBox.Add(IDC_BOX_GROUP1);
	m_aCtrlBox.Add(IDC_STATIC28);
	m_aCtrlBox.Add(IDC_CMD_SP_ID_BC3);

	m_PreViewBox.RemoveAll();
	m_PreViewBox.Add(IDC_CMD_SP_ID_PREVIEW3);
	m_PreViewBox.Add(IDC_CMD_SP_ID_PREVIEW5);

	m_aCtrlI.RemoveAll();
	m_aCtrlI.Add(IDC_CMD_SP_ID_BF1);
	m_aCtrlI.Add(IDC_CMD_SP_ID_BF2);
	//   m_aCtrlI.Add(IDC_CMD_SP_ID_N1);
	//   m_aCtrlI.Add(IDC_CMD_SP_ID_N1_SPIN);
	//   m_aCtrlI.Add(IDC_CMD_SP_ID_N2);
	//   m_aCtrlI.Add(IDC_CMD_SP_ID_N2_SPIN);
	//   m_aCtrlI.Add(IDC_CMD_SP_ID_HR1);
	//   m_aCtrlI.Add(IDC_CMD_SP_ID_HR2);
	//   m_aCtrlI.Add(IDC_CMD_SP_ID_TR1);
	//   m_aCtrlI.Add(IDC_CMD_SP_ID_TR2);
	m_PreViewI.RemoveAll();
	m_PreViewI.Add(IDC_CMD_SP_ID_PREVIEW4);
	m_PreViewI.Add(IDC_CMD_SP_ID_PREVIEW6);

	m_aCtrlCI.RemoveAll();
	m_aCtrlCI.Add(IDC_BOX_CI_GROUP);
	m_aCtrlCI.Add(IDC_CMD_SP_CI_TABLE);

	m_PreViewCI.RemoveAll();
	m_PreViewCI.Add(IDC_CMD_SP_CI_PREVIEW1);
	m_PreViewCI.Add(IDC_CMD_SP_CI_PREVIEW2);

	m_aCtrlCT.RemoveAll();
	m_aCtrlCT.Add(IDC_BOX_CT_GROUP);
	m_aCtrlCT.Add(IDC_CMD_SP_CT_TABLE);

	m_PreViewCT.RemoveAll();
	m_PreViewCT.Add(IDC_CMD_SP_CT_PREVIEW1);
	m_PreViewCT.Add(IDC_CMD_SP_CT_PREVIEW2);

	m_aCtrlPC.RemoveAll();
	m_aCtrlPC.Add(IDC_PSC_FRM);
	m_aCtrlPC.Add(IDC_PSC_STC);
	m_aCtrlPC.Add(IDC_PSC_IMP_BTN);

	m_PreViewPC.RemoveAll();
	m_PreViewPC.Add(IDC_CMD_PSC_PREVIEW);
	m_PreViewPC.Add(IDC_CMD_PSC_PREVIEW1);

	m_aCtrlGeneral.RemoveAll();
	m_aCtrlGeneral.Add(IDC_CMD_GENERAL_STC); // MNET:3022 20080103 mylee
	m_aCtrlGeneral.Add(IDC_CMD_SP_ID_GRID);
	m_aCtrlGeneral.Add(IDC_GENERAL_PARTSEL_CMB);
	m_aCtrlGeneral.Add(IDC_GENERAL_PARTSEL_STC);
	m_aCtrlGeneral.Add(IDC_GENERAL_PSC_DESIGN_CHK);
	m_aCtrlGeneral.Add(IDC_GENERAL_PSC_DESIGN_SHEAR_STC);
	m_aCtrlGeneral.Add(IDC_GENERAL_PSC_DESIGN_SHEAR_EDT);
	m_aCtrlGeneral.Add(IDC_GENERAL_PSC_DESIGN_SHEAR_UNT);
	m_aCtrlGeneral.Add(IDC_GENERAL_PARTBEFORE_CMB);
	m_aCtrlGeneral.Add(IDC_GENERAL_PARTBEFORE_STC);

	m_PreViewGeneral.RemoveAll();
	m_PreViewGeneral.Add(IDC_CMD_SP_ID_PREVIEW7);
	m_PreViewGeneral.Add(IDC_CMD_SP_ID_PREVIEW8);
	m_PreViewGeneral.Add(IDC_CMD_IMPORT_BTN);
	m_PreViewGeneral.Add(IDC_CMD_CALC_SECT_PROP_BTN);
	m_PreViewGeneral.Add(IDC_CMD_FEM_RDO);
	m_PreViewGeneral.Add(IDC_CMD_EQU_RDO2);
	m_PreViewGeneral.Add(IDC_CMD_IMPORT_EDIT);
	m_PreViewGeneral.Add(IDC_CMD_SEL_BTN);

	m_aCtrlTub.RemoveAll();
	m_aCtrlTub.Add(IDC_CMD_SP_ID_BF3_EDT);
	m_aCtrlTub.Add(IDC_CMD_SP_ID_TFP_EDT);

	m_PreViewTub.RemoveAll();
	m_PreViewTub.Add(IDC_CMD_TUB_PREVIEW1);
	m_PreViewTub.Add(IDC_CMD_TUB_PREVIEW2);

	m_PreViewStlGirderBox.RemoveAll();
	m_PreViewStlGirderBox.Add(IDC_CMD_SECT_B_GUIDE_BMP);
	m_PreViewStlGirderBox.Add(IDC_CMD_SECT_B_VIEW);

	m_PreViewStlGirderI.RemoveAll();
	m_PreViewStlGirderI.Add(IDC_CMD_SECT_I_GUIDE_BMP);
	m_PreViewStlGirderI.Add(IDC_CMD_SECT_I_VIEW);

	m_PreViewStlGirderTub.RemoveAll();
	m_PreViewStlGirderTub.Add(IDC_CMD_SECT_TUB_GUIDE_BMP);
	m_PreViewStlGirderTub.Add(IDC_CMD_SECT_TUB_VIEW);  

	m_aCtrlOption.RemoveAll();
	m_aCtrlOption.Add(IDC_CMD_FEM_RDO); // MNET:3022 20080103 mylee
	m_aCtrlOption.Add(IDC_CMD_EQU_RDO2);

	m_aCtrlStlGirder.RemoveAll();
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_SLAB_GRP);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_BC_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_BC_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_TC_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_TC_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_HH_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_HH_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_HH_UNT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_SIZE_GRP);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_SYM_AUTO_CALC_CHK);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_DIST_LEFT_GRP);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_SG_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_SG_EDT);  
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_TOP_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_TOP_EDT);  
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_BOT_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_BOT_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_BOT_UNT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B1_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B1_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_H_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_H_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B3_UNT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B2_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B2_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_T1_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_T1_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B6_UNT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B3_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B3_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_T2_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_T2_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_T2_UNT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B4_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B4_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_TW1_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_TW1_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_BF1_UNT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B5_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B5_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_TW2_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_TW2_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_BF2_UNT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B6_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_B6_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_BF1_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_BF1_EDT);  
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_BF2_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_BF2_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_TFP_STC);
	m_aCtrlStlGirder.Add(IDC_CMD_SECT_TFP_EDT);
	m_aCtrlStlGirder.Add(IDC_CMD_STIFFENER_BTN2);

	m_aCtrSlabInfo.RemoveAll();
	m_aCtrSlabInfo.Add(IDC_STATIC3);
	m_aCtrSlabInfo.Add(IDC_CMD_SP_ID_SWIDTH);
	m_aCtrSlabInfo.Add(IDC_CMD_SP_ID_UNIT1);
	m_aCtrSlabInfo.Add(IDC_STATIC4);
	m_aCtrSlabInfo.Add(IDC_CMD_SP_ID_GNUM);
	//m_aCtrSlabInfo.Add(IDC_CMD_SP_ID_GNUM_SPIN);
	m_aCtrSlabInfo.Add(IDC_STATIC5);
	m_aCtrSlabInfo.Add(IDC_CMD_SP_ID_CTC);
	m_aCtrSlabInfo.Add(IDC_CMD_SP_ID_UNIT2);    

	m_aCtrSlabGROUP.RemoveAll();
	m_aCtrSlabGROUP.Add(IDC_BOX_GROUP1);  
	m_aCtrSlabGROUP.Add(IDC_STATIC6);  
	m_aCtrSlabGROUP.Add(IDC_CMD_SP_ID_BC);  
	m_aCtrSlabGROUP.Add(IDC_CMD_SP_ID_UNIT3);  
	m_aCtrSlabGROUP.Add(IDC_STATIC7);  
	m_aCtrSlabGROUP.Add(IDC_CMD_SP_ID_TC);  
	m_aCtrSlabGROUP.Add(IDC_CMD_SP_ID_UNIT4);  
	m_aCtrSlabGROUP.Add(IDC_STATIC8);  
	m_aCtrSlabGROUP.Add(IDC_CMD_SP_ID_HH);  
	m_aCtrSlabGROUP.Add(IDC_CMD_SP_ID_UNIT5);  

	m_aCtrlAType.RemoveAll();
	m_aCtrlAType.Add(IDC_WG_CMD_SECT_SYMMETRY);
	m_aCtrlAType.Add(IDC_CMD_SECT_ATYPE_COMBO);
}


BOOL CCMSectItemCon::SaveData()
{
	if (!Dlg2Data()) return FALSE;
	if (m_Data.SName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_section_name_));
		return FALSE;
	}

#ifdef _MGEN_CH
	if (m_nCurType == 7)
	{
		m_Data.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();
	}
#endif

	if (!GetPeriAndStiff()) return FALSE;

	BOOL bReturn = FALSE;
	BOOL bReturnSect = TRUE;
	BOOL bReturnSecp = TRUE;

#if defined(_CIVIL)
	BOOL bGrpdMod = FALSE;
	BOOL bStfnMod = FALSE;
	if(m_pDoc->m_pAttrCtrl2->ExistGrpdSsm(m_Key)) bGrpdMod = TRUE;
	if(m_pDoc->m_pAttrCtrl2->ExistStfnSsm(m_Key)) bStfnMod = TRUE;

	if(m_bModifyGrpd)
	{
		if(bGrpdMod) m_pDoc->m_pDataCtrl->ModifyGrpdSsm(m_Key, m_Key, m_GrpdSSMD);
		else         m_pDoc->m_pDataCtrl->AddGrpdSsm(m_Key, m_GrpdSSMD);
	}

	if(m_bModifyStfn)
	{
		if(bStfnMod) m_pDoc->m_pDataCtrl->ModifyStfnSsm(m_Key, m_Key, m_StfnSSMD);
		else         m_pDoc->m_pDataCtrl->AddStfnSsm(m_Key, m_StfnSSMD);
	}
#endif

	if (m_bModify)
	{
		if (m_bVirtual)
		{
			T_SECV_D SecvData;
			SecvData.ConvertToSecv(m_Data);
			bReturnSect = m_pDoc->m_pDataCtrl->ModifySecv(m_OrgKey, m_Key, SecvData);
		}
		else
		{
			if (m_bDesign) bReturnSect = m_pDoc->m_pDataCtrl->AddSecd(m_Key, m_Data, m_bVirtual);
			else           bReturnSect = m_pDoc->m_pDataCtrl->ModifySect(m_OrgKey, m_Key, m_Data);
		}
	}
	else 
		bReturnSect = m_pDoc->m_pDataCtrl->AddSect(m_Key, m_Data);

	if (m_bVirtual)
	{
		return bReturnSect;
	}

#if defined(_CIVIL)
	if(bReturnSect)
	{
		if(m_pDoc->m_pAttrCtrl2->ExistSecp(m_OrgKey))
		{
			bReturnSecp = m_pDoc->m_pDataCtrl->ModifySecp(m_OrgKey, m_Key, m_PSCDesign);
		}
		else
		{
			bReturnSecp = m_pDoc->m_pDataCtrl->AddSecp(m_Key, m_PSCDesign);
		}
	}
#endif

	if(bReturnSect && bReturnSecp) bReturn = TRUE;
	else                           bReturn = FALSE; 

	return bReturn;
}

void CCMSectItemCon::SaveBoxData()
{
	UpdateData();
	// write //////////////////////////////////////
	m_DataBox.SectBefore.Shape = D_SECT_SHAPE_COMPO_B;
	m_DataBox.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
	m_DataBox.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
	m_DataBox.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();

	m_DataBox.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
	m_DataBox.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
	m_DataBox.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();

	m_DataBox.SectBefore.SectI.Size[0] = m_Hw.GetEditValue();
	m_DataBox.SectBefore.SectI.Size[1] = m_tw.GetEditValue();
	m_DataBox.SectBefore.SectI.Size[2] = m_B1.GetEditValue();
	m_DataBox.SectBefore.SectI.Size[3] = m_Bf1.GetEditValue();
	m_DataBox.SectBefore.SectI.Size[4] = m_Tf1.GetEditValue();
	m_DataBox.SectBefore.SectI.Size[5] = m_B2.GetEditValue();
	m_DataBox.SectBefore.SectI.Size[6] = m_Bf2.GetEditValue();
	m_DataBox.SectBefore.SectI.Size[7] = m_Tf2.GetEditValue();

	m_DataBox.SectBefore.nStiffNum[0] = m_N1.GetEditValueInt();
	m_DataBox.SectBefore.nStiffNum[1] = m_N2.GetEditValueInt();
	m_DataBox.SectBefore.SectJ.Size[2] = m_Hr1.GetEditValue();
	m_DataBox.SectBefore.SectJ.Size[3] = m_Hr2.GetEditValue();
	m_DataBox.SectBefore.SectJ.Size[4] = m_Tr1.GetEditValue();
	m_DataBox.SectBefore.SectJ.Size[5] = m_Tr2.GetEditValue();

	m_DataBox.SectBefore.Matl_Elast     = m_ESEC.GetEditValue();
	m_DataBox.SectBefore.Matl_Density   = m_DSDC.GetEditValue();
	m_DataBox.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
	m_DataBox.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
	m_DataBox.SectBefore.Matl_Thermal   = m_TSTC.GetEditValue();

	m_DataBox.SectBefore.bMultipleElastic  = m_chkMat.GetCheck();
	m_DataBox.SectBefore.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataBox.SectBefore.dShrinkageESEC    = m_edtMat2.GetEditValue();
	// 원래는 before만, 만일을 위해 After까지
	m_DataBox.SectAfter.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataBox.SectAfter.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataBox.SectAfter.dShrinkageESEC    = m_edtMat2.GetEditValue();
}

void CCMSectItemCon::SaveIData()
{
	UpdateData();
	// write //////////////////////////////////////
	m_DataI.SectBefore.Shape = D_SECT_SHAPE_COMPO_I;
	m_DataI.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
	m_DataI.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
	m_DataI.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();

	m_DataI.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
	m_DataI.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
	m_DataI.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();

	m_DataI.SectBefore.SectI.Size[0] = m_Hw.GetEditValue();
	m_DataI.SectBefore.SectI.Size[1] = m_tw.GetEditValue();
	m_DataI.SectBefore.SectI.Size[2] = m_B1.GetEditValue();
	m_DataI.SectBefore.SectI.Size[3] = m_Tf1.GetEditValue();
	m_DataI.SectBefore.SectI.Size[4] = m_B2.GetEditValue();
	m_DataI.SectBefore.SectI.Size[5] = m_Tf2.GetEditValue();

	m_DataI.SectBefore.Matl_Elast     = m_ESEC.GetEditValue();
	m_DataI.SectBefore.Matl_Density   = m_DSDC.GetEditValue();
	m_DataI.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
	m_DataI.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
	m_DataI.SectBefore.Matl_Thermal   = m_TSTC.GetEditValue();

	m_DataI.SectBefore.bMultipleElastic  = m_chkMat.GetCheck();
	m_DataI.SectBefore.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataI.SectBefore.dShrinkageESEC    = m_edtMat2.GetEditValue();
	// 원래는 before만, 만일을 위해 After까지
	m_DataI.SectAfter.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataI.SectAfter.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataI.SectAfter.dShrinkageESEC    = m_edtMat2.GetEditValue();
}

void CCMSectItemCon::SaveCIData()
{
	UpdateData();
	m_DataCI.SectBefore.Shape = D_SECT_SHAPE_COMPO_CI;
	m_DataCI.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
	m_DataCI.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
	m_DataCI.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();

	m_DataCI.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
	m_DataCI.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
	m_DataCI.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();
	m_DataCI.SectAfter.SectJ.Size[3] = m_Sg.GetEditValue();

	m_wndGridCI.SaveData();

	m_DataCI.SectBefore.Matl_Elast     = m_ESEC.GetEditValue();
	m_DataCI.SectBefore.Matl_Density   = m_DSDC.GetEditValue();
	m_DataCI.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
	m_DataCI.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
	m_DataCI.SectBefore.Matl_Thermal   = m_TSTC.GetEditValue();

	m_DataCI.SectBefore.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataCI.SectBefore.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataCI.SectBefore.dShrinkageESEC    = m_edtMat2.GetEditValue();
	// 원래는 before만, 만일을 위해 After까지
	m_DataCI.SectAfter.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataCI.SectAfter.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataCI.SectAfter.dShrinkageESEC    = m_edtMat2.GetEditValue();
}

void CCMSectItemCon::SaveCTData()
{
	UpdateData();
	m_DataCT.SectBefore.Shape = D_SECT_SHAPE_COMPO_CT;
	m_DataCT.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
	m_DataCT.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
	m_DataCT.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();

	m_DataCT.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
	m_DataCT.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
	m_DataCT.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();
	m_DataCT.SectAfter.SectJ.Size[3] = m_Sg.GetEditValue();

	m_wndGridCT.SaveData();

	m_DataCT.SectBefore.Matl_Elast     = m_ESEC.GetEditValue();
	m_DataCT.SectBefore.Matl_Density   = m_DSDC.GetEditValue();
	m_DataCT.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
	m_DataCT.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
	m_DataCT.SectBefore.Matl_Thermal   = m_TSTC.GetEditValue();

	m_DataCT.SectBefore.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataCT.SectBefore.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataCT.SectBefore.dShrinkageESEC    = m_edtMat2.GetEditValue();
	// 원래는 before만, 만일을 위해 After까지
	m_DataCT.SectAfter.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataCT.SectAfter.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataCT.SectAfter.dShrinkageESEC    = m_edtMat2.GetEditValue();
}

void CCMSectItemCon::SavePCData()
{
	UpdateData();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	T_SECT_D PscValueSectD;
	if(m_PscValueSectK>0)
	{
		if(!pDoc->m_pAttrCtrl->GetSect(m_PscValueSectK, PscValueSectD)) { ASSERT(0); return; }

		// Offset
		int nOffsetPoint   = m_DataPC.SectBefore.nOffsetPoint;
		int nOffsetCenter  = m_DataPC.SectBefore.nOffsetCenter;
		int nHorzOffsetOpt = m_DataPC.SectBefore.nHorzOffsetOpt;
		int nVertOffsetOpt = m_DataPC.SectBefore.nVertOffsetOpt;
		int nUserOffsetRef = m_DataPC.SectBefore.nUserOffsetRef;
		double dUserDefOffsetY[2]={0.}, dUserDefOffsetZ[2]={0.,};
		memcpy(dUserDefOffsetY, m_DataPC.SectBefore.dUserDefOffsetY, sizeof(dUserDefOffsetY));
		memcpy(dUserDefOffsetZ, m_DataPC.SectBefore.dUserDefOffsetZ, sizeof(dUserDefOffsetZ));

		m_DataPC.SectBefore = PscValueSectD.SectBefore;

		//////////////////////////////////////////////////////////////////////////
		// Convert
		int nOutPoly = m_DataPC.SectBefore.SectI.aOuterPolygon.GetSize();
		double dMaxX = 0.0;
		double dMinX = 0.0;
		for(int nOuter=0 ; nOuter < nOutPoly ; ++nOuter)
		{
			CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>* paVertex = &m_DataPC.SectBefore.SectI.aOuterPolygon[nOuter].aVertex;
			for (int i = 0; i < paVertex->GetSize(); i++)
			{
				T_2COOR Coord;
				Coord.x = (*paVertex)[i].dX;
				if(dMaxX<Coord.x)  dMaxX = Coord.x;
				if(dMinX>Coord.x)  dMinX = Coord.x;
			}    
		}
		double dAdjX = dMaxX-(m_DataPC.SectBefore.SectI.Stiffness.Cyp);

		for(int nOuter=0 ; nOuter < nOutPoly ; ++nOuter)
		{
			//T_2COOR Coord;
			CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>* paVertex = &m_DataPC.SectBefore.SectI.aOuterPolygon[nOuter].aVertex;
			for (int i = 0; i < paVertex->GetSize(); i++)
			{
				m_DataPC.SectBefore.SectI.aOuterPolygon[nOuter].aVertex[i].dX -=dAdjX;
			}
		}

		int nInPoly = m_DataPC.SectBefore.SectI.aInnerPolygon.GetSize();
		for (int i = 0; i < nInPoly; i++)
		{
			CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>* paVertex = &m_DataPC.SectBefore.SectI.aInnerPolygon[i].aVertex;
			for (int j = 0; j < paVertex->GetSize(); j++)
			{
				m_DataPC.SectBefore.SectI.aInnerPolygon[i].aVertex[j].dX -=dAdjX;
			}
		}

		m_DataPC.SectBefore.SectI.Stiffness.dx1 -=dAdjX;
		m_DataPC.SectBefore.SectI.Stiffness.dx2 -=dAdjX;
		m_DataPC.SectBefore.SectI.Stiffness.dx3 -=dAdjX;
		m_DataPC.SectBefore.SectI.Stiffness.dx4 -=dAdjX;
		//////////////////////////////////////////////////////////////////////////


		// Offset
		m_DataPC.SectBefore.nOffsetPoint   = nOffsetPoint;
		m_DataPC.SectBefore.nOffsetCenter  = nOffsetCenter;
		m_DataPC.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
		m_DataPC.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
		m_DataPC.SectBefore.nUserOffsetRef = nUserOffsetRef;
		memcpy(m_DataPC.SectBefore.dUserDefOffsetY, dUserDefOffsetY, sizeof(dUserDefOffsetY));
		memcpy(m_DataPC.SectBefore.dUserDefOffsetZ, dUserDefOffsetZ, sizeof(dUserDefOffsetZ));


		// 전단관련 데이터들.
		for (int i = 0; i < 3; i++)
		{
			m_DataPC.SectBefore.bAutoShearCheckPos[0][i] = PscValueSectD.SectBefore.bAutoShearCheckPos[0][i];
			m_DataPC.SectBefore.dShearCheckPos[0][i]     = PscValueSectD.SectBefore.dShearCheckPos[0][i];
		}
		for (int i = 0; i < 3; i++)
		{
			m_DataPC.SectBefore.bAutoQy[0][i] = PscValueSectD.SectBefore.bAutoQy[0][i];
			m_DataPC.SectBefore.dQy[0][i]     = PscValueSectD.SectBefore.dQy[0][i];
		}
		m_DataPC.SectBefore.bAutoWebThk[0] = PscValueSectD.SectBefore.bAutoWebThk[0];
		m_DataPC.SectBefore.dWebThk[0]     = PscValueSectD.SectBefore.dWebThk[0];
		for (int i = 0; i < 3; i++)
		{
			m_DataPC.SectBefore.bAutoWebThkShear[0][i] = PscValueSectD.SectBefore.bAutoWebThkShear[0][i];
			m_DataPC.SectBefore.dWebThkShear[0][i]     = PscValueSectD.SectBefore.dWebThkShear[0][i];
		}
	}

	m_DataPC.nStype = D_SECT_TYPE_COMPO_PC;
	m_DataPC.SectBefore.Shape = D_SECT_SHAPE_COMPO_PC;

	m_DataPC.SectBefore.Matl_Elast     = m_ESEC.GetEditValue();
	m_DataPC.SectBefore.Matl_Density   = m_DSDC.GetEditValue();
	m_DataPC.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
	m_DataPC.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
	m_DataPC.SectBefore.Matl_Thermal   = m_TSTC.GetEditValue();

	m_DataPC.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
	m_DataPC.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
	m_DataPC.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();

	m_DataPC.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
	m_DataPC.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
	m_DataPC.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();
	m_DataPC.SectAfter.SectJ.Size[3] = m_Sg.GetEditValue();

	m_DataPC.SectBefore.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataPC.SectBefore.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataPC.SectBefore.dShrinkageESEC    = m_edtMat2.GetEditValue();
	// 원래는 before만, 만일을 위해 After까지
	m_DataPC.SectAfter.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataPC.SectAfter.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataPC.SectAfter.dShrinkageESEC    = m_edtMat2.GetEditValue();
}

void CCMSectItemCon::SaveGeneralData() // later
{
	// offset data backup
	int nOffsetPoint = m_DataGeneral.SectBefore.nOffsetPoint;
	int nOffsetCenter = m_DataGeneral.SectBefore.nOffsetCenter;
	int nUserOffsetRef = m_DataGeneral.SectBefore.nUserOffsetRef;
	int nHorzOffsetOpt = m_DataGeneral.SectBefore.nHorzOffsetOpt;
	int nVertOffsetOpt = m_DataGeneral.SectBefore.nVertOffsetOpt;
	double dUserDefOffsetY[2], dUserDefOffsetZ[2];
	for (int i=0; i<2; i++)
	{
		dUserDefOffsetY[i] = m_DataGeneral.SectBefore.dUserDefOffsetY[i];
		dUserDefOffsetZ[i] = m_DataGeneral.SectBefore.dUserDefOffsetZ[i];
	}

	// save size
	CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aOuterPolygon;
	CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aInnerPolygon;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	CArray<int, int> aLineLoop;
	CArray<T_GSEC_LSHAP,T_GSEC_LSHAP&> aShape;
	CArray<T_GSEC_PART, T_GSEC_PART&> aGeneralPart;

	int nBeforePart = m_DataGeneral.SectBefore.SectI.nBeforePart;
	aOuterPolygon.Copy(m_DataGeneral.SectBefore.SectI.aOuterPolygon);
	aInnerPolygon.Copy(m_DataGeneral.SectBefore.SectI.aInnerPolygon);
	aLine.Copy(m_DataGeneral.SectBefore.SectI.aLine);
	aLineLoop.Copy(m_DataGeneral.SectBefore.SectI.aLineLoop);
	aShape.Copy(m_DataGeneral.SectBefore.SectI.aShape);
	aGeneralPart.Copy(m_DataGeneral.SectBefore.SectI.aGeneralPart);

	T_SECT_SECTBASE_D SectBaseCopy;
	SectBaseCopy.Initialize();
	SectBaseCopy.Stiffness = m_DataGeneral.SectAfter.SectI.Stiffness;
	SectBaseCopy.PeriOut = m_DataGeneral.SectAfter.SectI.PeriOut;
	SectBaseCopy.PeriIn = m_DataGeneral.SectAfter.SectI.PeriIn;
	SectBaseCopy.Design.YBar = m_DataGeneral.SectAfter.SectI.Design.YBar;
	SectBaseCopy.Design.ZBar = m_DataGeneral.SectAfter.SectI.Design.ZBar;

	T_SECT_SECTBASE_D SectBaseCopyBefore;
	SectBaseCopyBefore.Initialize();
	SectBaseCopyBefore.Stiffness = m_DataGeneral.SectBefore.SectI.Stiffness;
	SectBaseCopyBefore.PeriOut = m_DataGeneral.SectBefore.SectI.PeriOut;
	SectBaseCopyBefore.PeriIn = m_DataGeneral.SectBefore.SectI.PeriIn;
	SectBaseCopyBefore.Design.YBar = m_DataGeneral.SectBefore.SectI.Design.YBar;
	SectBaseCopyBefore.Design.ZBar = m_DataGeneral.SectBefore.SectI.Design.ZBar;

	// save data
	m_DataGeneral.Initialize();
	m_DataGeneral.nStype = D_SECT_TYPE_COMPO_G;
	m_DataGeneral.SectBefore.Shape = D_SECT_SHAPE_COMPO_G;
	m_PSCDesign.Initialize();

	// ID
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = (T_SECT_K)_tstol(csID);

	// Name
	m_wndName.GetWindowText(m_DataGeneral.SName);
	m_DataGeneral.SName.TrimLeft(); m_DataGeneral.SName.TrimRight();

	// Shape
	//   int nShapeIndex = GetCurShapeIndex();
	//   if (nShapeIndex == LB_ERR) 
	//   {
	//     AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Shape_is_not_selected_));
	//     return FALSE;
	//   }
	//   m_DataGeneral.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
	//   if (m_DataGeneral.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT)
	//   {
	//     CString csCellNum;
	//     m_editCellNum.GetWindowText(csCellNum);
	//     m_DataGeneral.SectBefore.nCellShape = _ttoi(csCellNum);
	//   }

	// Built-Up
	//   if (m_wndBuiltUp.GetCheck() == 1) // later
	//     m_DataGeneral.SectBefore.SectI.BuiltUpFlag = 1;  // built-up
	//   else m_DataGeneral.SectBefore.SectI.BuiltUpFlag = 2; // rolled

	// Offset
	m_DataGeneral.SectBefore.nOffsetPoint = nOffsetPoint;
	m_DataGeneral.SectBefore.nOffsetCenter = nOffsetCenter;
	m_DataGeneral.SectBefore.nUserOffsetRef = nUserOffsetRef; // MQC:1493 20071130 mylee
	m_DataGeneral.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
	m_DataGeneral.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
	for (int i=0; i<2; i++)
	{
		m_DataGeneral.SectBefore.dUserDefOffsetY[i] = dUserDefOffsetY[i];
		m_DataGeneral.SectBefore.dUserDefOffsetZ[i] = dUserDefOffsetZ[i];
	}

	// consider shear deformation
	m_DataGeneral.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
	m_DataGeneral.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();
	m_DataGeneral.SectBefore.bHumblyChk = FALSE; // m_chkHumbly.GetCheck();

	// PSC Design
	m_PSCDesign.bCompPSC = m_chkPSCDesign.GetCheck();
	m_PSCDesign.dt1[0] = m_edtPSCShear.GetEditValue();

	// size, peri and stiffness - 잘못된 위치임. SectBaseCopy, SectBaseCopy에서 저장하도록 함
	//m_DataGeneral.SectBefore.SectI.Stiffness.WArea = m_DataGeneral.SectBefore.SectI.Stiffness.Area;

	// general section
	m_DataGeneral.SectAfter.SectI.Stiffness = SectBaseCopy.Stiffness;
	m_DataGeneral.SectAfter.SectI.PeriOut = SectBaseCopy.PeriOut;
	m_DataGeneral.SectAfter.SectI.PeriIn = SectBaseCopy.PeriIn;
	m_DataGeneral.SectAfter.SectI.Design.YBar = SectBaseCopy.Design.YBar;
	m_DataGeneral.SectAfter.SectI.Design.ZBar = SectBaseCopy.Design.ZBar;

	m_DataGeneral.SectBefore.SectI.Stiffness = SectBaseCopyBefore.Stiffness;
	m_DataGeneral.SectBefore.SectI.PeriOut = SectBaseCopyBefore.PeriOut;
	m_DataGeneral.SectBefore.SectI.PeriIn = SectBaseCopyBefore.PeriIn;
	m_DataGeneral.SectBefore.SectI.Design.YBar = SectBaseCopyBefore.Design.YBar;
	m_DataGeneral.SectBefore.SectI.Design.ZBar = SectBaseCopyBefore.Design.ZBar;

	m_DataGeneral.SectBefore.SectI.nBeforePart = nBeforePart;
	m_DataGeneral.SectBefore.SectI.aOuterPolygon.Copy(aOuterPolygon);
	m_DataGeneral.SectBefore.SectI.aInnerPolygon.Copy(aInnerPolygon);
	m_DataGeneral.SectBefore.SectI.aLine.Copy(aLine);
	m_DataGeneral.SectBefore.SectI.aLineLoop.Copy(aLineLoop);
	m_DataGeneral.SectBefore.SectI.aShape.Copy(aShape);
	m_DataGeneral.SectBefore.SectI.aGeneralPart.Copy(aGeneralPart);
	//CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, m_DataGeneral.SectBefore.nCalcStiffOpt);
	m_DataGeneral.SectBefore.nCalcStiffOpt = 1;

	// before part
	m_DataGeneral.SectBefore.SectI.nBeforePart = m_cmbBeforePart.GetCurSel()+1;

	// Before 에 해당하는 강성을 SectBefore 에 복사한다. (EditData 에도 해주는 부분 있음)
	int nTmp = m_DataGeneral.SectBefore.SectI.nBeforePart - 1;
	if(nTmp<aGeneralPart.GetSize())
	{
		m_DataGeneral.SectBefore.SectI.Stiffness = aGeneralPart[nTmp].StiffnessSum;
		m_DataGeneral.SectBefore.SectI.PeriOut = aGeneralPart[nTmp].PeriOutSum;
		m_DataGeneral.SectBefore.SectI.PeriIn = aGeneralPart[nTmp].PeriInSum;
		m_DataGeneral.SectBefore.SectI.Design.YBar = aGeneralPart[nTmp].YBarSum;
		m_DataGeneral.SectBefore.SectI.Design.ZBar = aGeneralPart[nTmp].ZBarSum;
	}
}

void CCMSectItemCon::SaveTubData()
{
	UpdateData();
	// write //////////////////////////////////////
	m_DataTub.SectBefore.Shape = D_SECT_SHAPE_COMPO_TUB;
	m_DataTub.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
	m_DataTub.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
	m_DataTub.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();

	m_DataTub.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
	m_DataTub.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
	m_DataTub.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();

	m_DataTub.SectBefore.SectI.Size[0] = m_Hw.GetEditValue();
	m_DataTub.SectBefore.SectI.Size[1] = m_tw.GetEditValue();
	m_DataTub.SectBefore.SectI.Size[2] = m_B1.GetEditValue();
	m_DataTub.SectBefore.SectI.Size[3] = m_Bf1.GetEditValue();
	m_DataTub.SectBefore.SectI.Size[4] = m_Tf1.GetEditValue();
	m_DataTub.SectBefore.SectI.Size[5] = m_B2.GetEditValue();
	m_DataTub.SectBefore.SectI.Size[6] = m_Bf2.GetEditValue();
	m_DataTub.SectBefore.SectI.Size[7] = m_Tf2.GetEditValue();
	m_DataTub.SectBefore.SectI.Size[8] = m_Bf3.GetEditValue();
	m_DataTub.SectBefore.SectI.Size[9] = m_tfp.GetEditValue();

	m_DataTub.SectBefore.nStiffNum[0] = m_N1.GetEditValueInt();
	m_DataTub.SectBefore.nStiffNum[1] = m_N2.GetEditValueInt();
	m_DataTub.SectBefore.SectJ.Size[2] = m_Hr1.GetEditValue();
	m_DataTub.SectBefore.SectJ.Size[3] = m_Hr2.GetEditValue();
	m_DataTub.SectBefore.SectJ.Size[4] = m_Tr1.GetEditValue();
	m_DataTub.SectBefore.SectJ.Size[5] = m_Tr2.GetEditValue();

	m_DataTub.SectBefore.Matl_Elast     = m_ESEC.GetEditValue();
	m_DataTub.SectBefore.Matl_Density   = m_DSDC.GetEditValue();
	m_DataTub.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
	m_DataTub.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
	m_DataTub.SectBefore.Matl_Thermal   = m_TSTC.GetEditValue();

	m_DataTub.SectBefore.bMultipleElastic  = m_chkMat.GetCheck();
	m_DataTub.SectBefore.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataTub.SectBefore.dShrinkageESEC    = m_edtMat2.GetEditValue();
	// 원래는 before만, 만일을 위해 After까지
	m_DataTub.SectAfter.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataTub.SectAfter.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataTub.SectAfter.dShrinkageESEC    = m_edtMat2.GetEditValue();
}

BOOL CCMSectItemCon::SaveStlGirderBoxData()
{
	UpdateData(TRUE);

	m_DataStlGirderBox.SectBefore.Shape = D_SECT_SHAPE_COMPO_STLG_B;

	m_DataStlGirderBox.SectAfter.SectJ.Size[0] = m_StlGirderBc.GetEditValue();
	m_DataStlGirderBox.SectAfter.SectJ.Size[1] = m_StlGirderTc.GetEditValue();
	m_DataStlGirderBox.SectAfter.SectJ.Size[2] = m_StlGirderHh.GetEditValue();

	m_DataStlGirderBox.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
	m_DataStlGirderBox.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
	m_DataStlGirderBox.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();

	m_DataStlGirderBox.SectBefore.SectI.Size[0]  = m_edtStlGirderB1.GetEditValue();
	m_DataStlGirderBox.SectBefore.SectI.Size[1]  = m_edtStlGirderB2.GetEditValue();
	m_DataStlGirderBox.SectBefore.SectI.Size[2]  = m_edtStlGirderB3.GetEditValue();
	m_DataStlGirderBox.SectBefore.SectI.Size[3]  = m_edtStlGirderB4.GetEditValue();
	m_DataStlGirderBox.SectBefore.SectI.Size[4]  = m_edtStlGirderB5.GetEditValue();
	m_DataStlGirderBox.SectBefore.SectI.Size[5]  = m_edtStlGirderB6.GetEditValue();
	m_DataStlGirderBox.SectBefore.SectI.Size[6]  = m_edtStlGirderH .GetEditValue();
	m_DataStlGirderBox.SectBefore.SectI.Size[7]  = m_edtStlGirdert1.GetEditValue();
	m_DataStlGirderBox.SectBefore.SectI.Size[8]  = m_edtStlGirdert2.GetEditValue();
	m_DataStlGirderBox.SectBefore.SectI.Size[9]  = m_edtStlGirdertw1.GetEditValue();
	m_DataStlGirderBox.SectBefore.SectI.Size[10] = m_edtStlGirdertw2.GetEditValue();

	m_DataStlGirderBox.SectBefore.Matl_Elast     = m_ESEC.GetEditValue();
	m_DataStlGirderBox.SectBefore.Matl_Density   = m_DSDC.GetEditValue();  
	m_DataStlGirderBox.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
	m_DataStlGirderBox.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
	m_DataStlGirderBox.SectBefore.Matl_Thermal   = m_TSTC.GetEditValue();

	m_DataStlGirderBox.SectBefore.bMultipleElastic  = m_chkMat.GetCheck();
	m_DataStlGirderBox.SectBefore.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataStlGirderBox.SectBefore.dShrinkageESEC    = m_edtMat2.GetEditValue();
	// 원래는 before만, 만일을 위해 After까지
	m_DataStlGirderBox.SectAfter.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataStlGirderBox.SectAfter.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataStlGirderBox.SectAfter.dShrinkageESEC    = m_edtMat2.GetEditValue();

	return TRUE;
}

BOOL CCMSectItemCon::SaveStlGirderIData()
{
	UpdateData(TRUE);

	m_DataStlGirderI.SectBefore.Shape = D_SECT_SHAPE_COMPO_STLG_I;

	m_DataStlGirderI.SectAfter.SectJ.Size[0] = m_StlGirderBc.GetEditValue();
	m_DataStlGirderI.SectAfter.SectJ.Size[1] = m_StlGirderTc.GetEditValue();
	m_DataStlGirderI.SectAfter.SectJ.Size[2] = m_StlGirderHh.GetEditValue();

	m_DataStlGirderI.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
	m_DataStlGirderI.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
	m_DataStlGirderI.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();

	m_DataStlGirderI.SectBefore.SectI.Size[0] = m_edtStlGirderB1.GetEditValue();
	m_DataStlGirderI.SectBefore.SectI.Size[1] = m_edtStlGirderB2.GetEditValue();
	m_DataStlGirderI.SectBefore.SectI.Size[2] = m_edtStlGirderB3.GetEditValue();
	m_DataStlGirderI.SectBefore.SectI.Size[3] = m_edtStlGirderB4.GetEditValue();
	m_DataStlGirderI.SectBefore.SectI.Size[4] = m_edtStlGirderH .GetEditValue();
	m_DataStlGirderI.SectBefore.SectI.Size[5] = m_edtStlGirdert1.GetEditValue();
	m_DataStlGirderI.SectBefore.SectI.Size[6] = m_edtStlGirdert2.GetEditValue();
	m_DataStlGirderI.SectBefore.SectI.Size[7] = m_edtStlGirdertw1.GetEditValue();

	m_DataStlGirderI.SectBefore.Matl_Elast     = m_ESEC.GetEditValue();
	m_DataStlGirderI.SectBefore.Matl_Density   = m_DSDC.GetEditValue();  
	m_DataStlGirderI.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
	m_DataStlGirderI.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
	m_DataStlGirderI.SectBefore.Matl_Thermal   = m_TSTC.GetEditValue();

	m_DataStlGirderI.SectBefore.bMultipleElastic  = m_chkMat.GetCheck();
	m_DataStlGirderI.SectBefore.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataStlGirderI.SectBefore.dShrinkageESEC    = m_edtMat2.GetEditValue();
	// 원래는 before만, 만일을 위해 After까지
	m_DataStlGirderI.SectAfter.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataStlGirderI.SectAfter.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataStlGirderI.SectAfter.dShrinkageESEC    = m_edtMat2.GetEditValue();

	return TRUE;
}


BOOL CCMSectItemCon::SaveStlGirderTubData()
{
	UpdateData(TRUE);

	m_DataStlGirderTub.SectBefore.Shape = D_SECT_SHAPE_COMPO_STLG_TUB;

	m_DataStlGirderTub.SectAfter.SectJ.Size[0] = m_StlGirderBc.GetEditValue();
	m_DataStlGirderTub.SectAfter.SectJ.Size[1] = m_StlGirderTc.GetEditValue();
	m_DataStlGirderTub.SectAfter.SectJ.Size[2] = m_StlGirderHh.GetEditValue();

	m_DataStlGirderTub.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
	m_DataStlGirderTub.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
	m_DataStlGirderTub.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();

	m_DataStlGirderTub.SectBefore.SectI.Size[0]  = m_edtStlGirderB1.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[1]  = m_edtStlGirderB2.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[2]  = m_edtStlGirderB3.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[3]  = m_edtStlGirderB4.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[4]  = m_edtStlGirderB5.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[5]  = m_edtStlGirderB6.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[6]  = m_edtStlGirderH .GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[7]  = m_edtStlGirdert1.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[8]  = m_edtStlGirdert2.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[9]  = m_edtStlGirdertw1.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[10] = m_edtStlGirdertw2.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[11] = m_edtStlGirderbf1.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[12] = m_edtStlGirderbf2.GetEditValue();
	m_DataStlGirderTub.SectBefore.SectI.Size[13] = m_edtStlGirdertfp.GetEditValue();

	m_DataStlGirderTub.SectBefore.Matl_Elast     = m_ESEC.GetEditValue();
	m_DataStlGirderTub.SectBefore.Matl_Density   = m_DSDC.GetEditValue();  
	m_DataStlGirderTub.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
	m_DataStlGirderTub.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
	m_DataStlGirderTub.SectBefore.Matl_Thermal   = m_TSTC.GetEditValue();

	m_DataStlGirderTub.SectBefore.bMultipleElastic  = m_chkMat.GetCheck();
	m_DataStlGirderTub.SectBefore.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataStlGirderTub.SectBefore.dShrinkageESEC    = m_edtMat2.GetEditValue();
	// 원래는 before만, 만일을 위해 After까지
	m_DataStlGirderTub.SectAfter.bMultipleElastic  = m_chkMat.GetCheck(); 
	m_DataStlGirderTub.SectAfter.dLongTermESEC     = m_edtMat1.GetEditValue();
	m_DataStlGirderTub.SectAfter.dShrinkageESEC    = m_edtMat2.GetEditValue();

	return TRUE;
}

BOOL CCMSectItemCon::Dlg2Data()
{
	// offset data backup
	int nOffsetPoint = m_Data.SectBefore.nOffsetPoint;
	int nOffsetCenter = m_Data.SectBefore.nOffsetCenter;
	int nUserOffsetRef = m_Data.SectBefore.nUserOffsetRef;
	int nHorzOffsetOpt = m_Data.SectBefore.nHorzOffsetOpt;
	int nVertOffsetOpt = m_Data.SectBefore.nVertOffsetOpt;
	double dUserDefOffsetY[2], dUserDefOffsetZ[2];
	for (int i=0; i<2; i++)
	{
		dUserDefOffsetY[i] = m_Data.SectBefore.dUserDefOffsetY[i];
		dUserDefOffsetZ[i] = m_Data.SectBefore.dUserDefOffsetZ[i];
	}

	// save data
	m_Data.Initialize();

	// ID
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = (T_SECT_K)_tstol(csID);

	// Size
	switch(m_nCurType)
	{
	case 1:
		m_Data.nStype = D_SECT_TYPE_COMPO_B;
		SaveBoxData();
		m_Data = m_DataBox;
		break;
	case 2:
		m_Data.nStype = D_SECT_TYPE_COMPO_I;
		SaveIData();
		m_Data = m_DataI;
		break;
	case 3:
		m_Data.nStype = D_SECT_TYPE_COMPO_CI;
		SaveCIData();
		m_Data = m_DataCI;
		break;
	case 4:
		m_Data.nStype = D_SECT_TYPE_COMPO_CT;
		SaveCTData();
		m_Data = m_DataCT;
		break;
	case 5:
		m_Data.nStype = D_SECT_TYPE_CONSTRUCTION;
		m_Data = m_DataUser;
		break;
	case 6:
		m_Data.nStype = D_SECT_TYPE_COMPO_PC;
		SavePCData();
		m_Data = m_DataPC;
		break;
	case 7:
		m_Data.nStype = D_SECT_TYPE_COMPO_G;
		// Save Grid Final
		m_wndGridGeneral.SaveData();
		SaveGeneralData();
		m_Data = m_DataGeneral;
#ifdef _MGEN_CH
		if (!CheckData())
		{
			return FALSE;
		}
#endif
		break;
	case 8:
		m_Data.nStype = D_SECT_TYPE_COMPO_TUB;
		SaveTubData();
		m_Data = m_DataTub;
		break;
	case 9:
		m_Data.nStype = D_SECT_TYPE_COMPO_STLG_B;
		SaveStlGirderBoxData();
		m_Data = m_DataStlGirderBox;
		break;
	case 10:
		m_Data.nStype = D_SECT_TYPE_COMPO_STLG_I;
		SaveStlGirderIData();
		m_Data = m_DataStlGirderI;
		break;
	case 11:
		m_Data.nStype = D_SECT_TYPE_COMPO_STLG_TUB;
		SaveStlGirderTubData();
		m_Data = m_DataStlGirderTub;
		break;
	default: ASSERT(0);
		break;
	}

	// Symmetric Section Auto Calculation
	m_Data.SectBefore.SectI.bSymAutoCalc = m_bSymAutoCalc;

	// consider shear deformation
	m_Data.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
	m_Data.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();
	m_Data.SectBefore.bHumblyChk = (m_chkHumbly.GetCheck() || m_chkHumblyBefore.GetCheck());
	m_Data.SectAfter.bHumblyChk = (m_chkHumbly.GetCheck() || m_chkHumblyAfter.GetCheck());

	// PSC Design
	m_PSCDesign.bCompPSC = m_chkPSCDesign.GetCheck();
	m_PSCDesign.dt1[0] = m_edtPSCShear.GetEditValue();

	// Offset
	m_Data.SectBefore.nOffsetPoint = nOffsetPoint;
	m_Data.SectBefore.nOffsetCenter = nOffsetCenter;
	m_Data.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
	m_Data.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
	m_Data.SectBefore.nUserOffsetRef = nUserOffsetRef;
	for (int i=0; i<2; i++)
	{
		m_Data.SectBefore.dUserDefOffsetY[i] = dUserDefOffsetY[i];
		m_Data.SectBefore.dUserDefOffsetZ[i] = dUserDefOffsetZ[i];
	}

	// Name
	m_wndName.GetWindowText(m_Data.SName);
	m_Data.SName.TrimLeft(); m_Data.SName.TrimRight();

	if(m_Data.nStype != D_SECT_TYPE_CONSTRUCTION)
	{
		// MQC 9509 by Hsshim. 
		if (CSectDB::IsEnable7thDOF())
		{
			// MQC Alpha/Beta 4632-59. HSSHIM-20130826. 7자유도 계산시 Slab Width로 계산하지 않음. bc(유효폭)으로 계산하기로 결정.
			// T_SECT_COMPO_CI::Set(const T_SECT_D& rSectD) 함수 참고로 만듬.
			m_Data.SectAfter.SectI.Size[0]     = m_Data.SectAfter.SectJ.Size[0];    // rSect.dWidth = rSect.Slab.Bc
			m_Data.SectAfter.SectI.BuiltUpFlag = 1;                                 // rSect.nNum   = 1
		}
	}

	//symmetrical type
	m_Data.nSectAType = m_cbxAType.GetCurSel();

	return TRUE;
}

BOOL CCMSectItemCon::GetPeriAndStiff()
{
	// by pig
	if(m_Data.nStype == D_SECT_TYPE_CONSTRUCTION)return TRUE;

	// hslee : Composite단면의 포와송비가 0.5 이상일 경우 pBeamdll에서 모든 결과값을 0.0으로 return 하기 때문에 이에 대한 에러 처리 추가  
	int nStype = m_Data.nStype == D_SECT_TYPE_TAPERED ? m_Data.SectBefore.nStype : m_Data.nStype;  
	if(nStype == D_SECT_TYPE_COMPO_B || nStype == D_SECT_TYPE_COMPO_I || nStype == D_SECT_TYPE_COMPO_TUB || 
		nStype == D_SECT_TYPE_COMPO_STLG_B || nStype == D_SECT_TYPE_COMPO_STLG_I || nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		if(m_Data.SectBefore.Matl_Poisson_S > 0.5 || m_Data.SectBefore.Matl_Poisson_C > 0.5)
		{  
			AfxMessageBox(_LS(IDS_DB_WARNING_STL_COMP_POISSON_RATIO));
		}
	}
	if(nStype == D_SECT_TYPE_COMPO_CI || nStype == D_SECT_TYPE_COMPO_CT || nStype == D_SECT_TYPE_COMPO_PC)
	{
		if(m_Data.SectBefore.Matl_Poisson_S > 0.5 || m_Data.SectBefore.Matl_Poisson_C > 0.5)
		{  
			AfxMessageBox(_LS(IDS_DB_WARNING_PSC_COMP_POISSON_RATIO));
		}
	}

	CSectUtil::Do7thDOFLoopForMeshSizeFind(m_Data);  
#ifdef _MGEN_CH
	if(m_Data.nStype == D_SECT_TYPE_COMPO_G && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_G)
	{
		if(CSectUtil::CalcSectData(m_Data))
		{
			m_Data.SectBefore.bConsiderWarpingEffect = TRUE;
			if(!MITC_CommonTool::Calc7thDOFByPSCValue(m_Data.SectBefore.SectI))
				return CSectUtil::CalcSectData(m_Data);
			return TRUE;
		}
	}
#endif
	return CSectUtil::CalcSectData(m_Data);
	//return TRUE;  // nothing to calculate
}

void CCMSectItemCon::InitSecTypeCombo()
{
	m_cboSecType.ResetContent();

#if defined(_MGEN)
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Steel_Box),      1);
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Steel_I),        2);
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Steel_TUB),      8);
#ifdef _CH
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Composite_G), 7);
#endif
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_User),           5);
#else
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Steel_Box_TYPE1),    1);
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Steel_I_TYPE1),      2);
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Steel_TUB_TYPE1),    8);
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Steel_Box_TYPE2),    9);
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Steel_I_TYPE2),     10);
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Steel_TUB_TYPE2),   11);
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Composite_CI),       3);
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Composite_CT),       4);
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Composite_PC),       6);
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_Composite_G),        7);  
	CDlgUtil::CobxAddItem(m_cboSecType, _LS(IDS_CMD_SECT_Comp_User),               5);
#endif

	CDlgUtil::CobxSetCurSelItemData(m_cboSecType, m_nCurType);
}

void CCMSectItemCon::InitControls()
{
	// set section type
	InitSecTypeCombo();   // 추가 

	// set section name list
	SetSectionNameList();

	// general composite
	SetGeneralPartList();
	GetDlgItem(IDC_CMD_SEL_BTN)->EnableWindow(FALSE);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE);
	m_editPath.SetReadOnly(TRUE);

	InitView();
	InitGrid();
	InitUnit();
	InitImage();

	//
	SetSectionATypeCombo();
	m_cbxAType.SetCurSel(0);

	if (CSectDB::IsEnable7thDOF())
	{
		// MQC Alpha/Beta 4632-59. HSSHIM-20130826. 7자유도 계산시 Slab Width로 계산하지 않음. bc(유효폭)으로 계산하기로 결정.
		GetDlgItem(IDC_STATIC3            )->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SP_ID_SWIDTH   )->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SP_ID_UNIT1    )->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC4            )->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SP_ID_GNUM     )->EnableWindow(FALSE);
		//GetDlgItem(IDC_CMD_SP_ID_GNUM_SPIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC5            )->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SP_ID_CTC      )->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SP_ID_UNIT2    )->EnableWindow(FALSE);
	}

	if (m_bVirtual)
	{
		GetDlgItem(IDC_CMD_SP_SECTYPE_COMBO  )->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HUMBLY_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HUMBLY_CHK_GRBOX)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HUMBLY_CHK_BF)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HUMBLY_CHK_AF)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_CHANGE_OFFSET_BTN )->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SHEAR_DEFORM      )->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_CHK)->EnableWindow(FALSE);
	}
}

void CCMSectItemCon::ShowKey2Dlg()
{
	if(m_wndID.GetSafeHwnd() == nullptr) return;

	CString csKey;
	csKey.Format(_T("%ld"), m_Key);
	m_wndID.SetWindowText(csKey);
}

void CCMSectItemCon::ShowData2Dlg()
{
	m_wndName.SetWindowText(m_Data.SName);

	if(m_Data.nStype == D_SECT_TYPE_CONSTRUCTION)
	{
		ShowUserData2Dlg();
		//m_wndSecViewFirst.SetCentroidFlag(FALSE);
		m_wndSecViewFirst.Invalidate();
		//m_wndSecViewSecond.SetCentroidFlag(FALSE);
		m_wndSecViewSecond.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_B)
	{
		ShowBoxData2Dlg();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_I)
	{
		ShowIData2Dlg();
		//m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_CI)
	{
		ShowCIData2Dlg();
		m_wndSecViewCI.SetCentroidFlag(FALSE);
		m_wndSecViewCI.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_CT)
	{
		ShowCTData2Dlg();
		m_wndSecViewCT.SetCentroidFlag(FALSE);
		m_wndSecViewCT.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_PC)
	{
		BOOL bHumblyShow = TRUE;
		ShowPCData2Dlg(bHumblyShow);
		m_wndSecViewPC.SetCentroidFlag(FALSE);
		m_wndSecViewPC.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_G)
	{
		ShowGeneralData2Dlg();
		m_wndSecViewGeneral.SetCentroidFlag(FALSE);
		m_wndSecViewGeneral.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_TUB)
	{
		ShowTubData2Dlg();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_B)
	{
		ShowStlGirderBoxData2Dlg();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_I)
	{
		ShowStlGirderIData2Dlg();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		ShowStlGirderTubData2Dlg();
		m_wndSecViewStlGirderTub.Invalidate();
	}
	else ASSERT(0);

	// Symmetric Section Auto Calculation
	m_bSymAutoCalc = m_Data.SectBefore.SectI.bSymAutoCalc;
	UpdateData(FALSE);
}

void CCMSectItemCon::PostShowData2Dlg()
{
	if(m_Data.nStype == D_SECT_TYPE_CONSTRUCTION)
	{

	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_B)
	{
		m_gnum.SetValue(m_DataBox.SectAfter.SectI.BuiltUpFlag);
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_I)
	{

	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_TUB)
	{
		m_gnum.SetValue(m_DataBox.SectAfter.SectI.BuiltUpFlag);
	}
}

void CCMSectItemCon::ShowUserData2Dlg()
{
	if(m_DataUser.SectBefore.SectI.SName.IsEmpty()==FALSE)
	{
		T_SECT_K Key = m_pDoc->m_pAttrCtrl->GetSectKey(m_DataUser.SectBefore.SectI.SName);
		if(Key!=NULL)
		{
			CString strName = _T("");
			strName.Format(_T("%d: %s"), Key, m_DataUser.SectBefore.SectI.SName);
			int nIndex = m_wndFirstName.FindString(0, strName);
			if(nIndex>=0)
			{
				m_wndFirstName.SetCurSel(nIndex);
			}
		}
	}

	if(m_DataUser.SectAfter.SectI.SName.IsEmpty()==FALSE)
	{
		T_SECT_K Key = m_pDoc->m_pAttrCtrl->GetSectKey(m_DataUser.SectAfter.SectI.SName);
		if(Key!=NULL)
		{
			CString strName = _T("");
			strName.Format(_T("%d: %s"), Key, m_DataUser.SectAfter.SectI.SName);
			int nIndex = m_wndSecondName.FindString(0, strName);
			if(nIndex>=0)
			{
				m_wndSecondName.SetCurSel(nIndex);
			}
		}
	}

	// before(first), after(second)
	m_wndGridFirst.InitSectionByData(&m_DataUser, FALSE);
	m_wndGridSecond.InitSectionByData(&m_DataUser, TRUE);

	// set offset point
	if (m_DataUser.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataUser.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_DataUser.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_DataUser.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// Humbly Check
	//m_chkHumbly.SetCheck((int)m_DataUser.SectBefore.bHumblyChk);
	 m_chkHumbly.ShowWindow(SW_HIDE);

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);
}

void CCMSectItemCon::ShowBoxData2Dlg()
{
	CString str;
	// write //////////////////////////////////////
	str.Format(_T("%g"), m_DataBox.SectAfter.SectI.Size[0]); m_swidth.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectAfter.SectI.Size[1]); m_CTC.SetWindowText(str);

	m_gnum.SetValue(m_DataBox.SectAfter.SectI.BuiltUpFlag);

	str.Format(_T("%g"), m_DataBox.SectAfter.SectJ.Size[0]); m_Bc.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectAfter.SectJ.Size[1]); m_Tc.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectAfter.SectJ.Size[2]); m_Hh.SetWindowText(str);

	str.Format(_T("%g"), m_DataBox.SectBefore.SectI.Size[0]); m_Hw.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectBefore.SectI.Size[1]); m_tw.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectBefore.SectI.Size[2]); m_B1.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectBefore.SectI.Size[3]); m_Bf1.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectBefore.SectI.Size[4]); m_Tf1.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectBefore.SectI.Size[5]); m_B2.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectBefore.SectI.Size[6]); m_Bf2.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectBefore.SectI.Size[7]); m_Tf2.SetWindowText(str);

	m_N1.SetValue(m_DataBox.SectBefore.nStiffNum[0]);//.nLytype;
	m_N2.SetValue(m_DataBox.SectBefore.nStiffNum[1]);//.nLztype;
	str.Format(_T("%g"), m_DataBox.SectBefore.SectJ.Size[2]); m_Hr1.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectBefore.SectJ.Size[3]); m_Hr2.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectBefore.SectJ.Size[4]); m_Tr1.SetWindowText(str);
	str.Format(_T("%g"), m_DataBox.SectBefore.SectJ.Size[5]); m_Tr2.SetWindowText(str);
	m_ESEC.SetEditUnit(m_DataBox.SectBefore.Matl_Elast);     //str.Format(_T("%g"), m_DataBox.SectBefore.Matl_Elast);   m_ESEC.SetWindowText(str);
	m_DSDC.SetEditUnit(m_DataBox.SectBefore.Matl_Density);   //str.Format(_T("%g"), m_DataBox.SectBefore.Matl_Density); m_DSDC.SetWindowText(str);
	m_PS  .SetEditUnit(m_DataBox.SectBefore.Matl_Poisson_S); //str.Format(_T("%g"), m_DataBox.SectBefore.Matl_Poisson_S); m_PS.SetWindowText(str);
	m_PC  .SetEditUnit(m_DataBox.SectBefore.Matl_Poisson_C); //str.Format(_T("%g"), m_DataBox.SectBefore.Matl_Poisson_C); m_PC.SetWindowText(str);
	m_TSTC.SetEditUnit(m_DataBox.SectBefore.Matl_Thermal);   //str.Format(_T("%g"), m_DataBox.SectBefore.Matl_Thermal); m_TSTC.SetWindowText(str);

	m_chkMat.SetCheck(m_DataBox.SectBefore.bMultipleElastic);
	m_edtMat1.SetEditUnit(m_DataBox.SectBefore.dLongTermESEC);  //str.Format(_T("%g"), m_DataBox.SectBefore.dLongTermESEC); m_edtMat1.SetWindowText(str);
	m_edtMat2.SetEditUnit(m_DataBox.SectBefore.dShrinkageESEC); //str.Format(_T("%g"), m_DataBox.SectBefore.dShrinkageESEC); m_edtMat2.SetWindowText(str);

	///////////////////////////////////////////////

	m_Bf3.SetWindowText(_T("0"));
	m_tfp.SetWindowText(_T("0"));

	// set offset point
	if (m_DataBox.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataBox.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_DataBox.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_DataBox.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// Humbly Check
	m_chkHumbly.SetCheck((int)m_DataBox.SectBefore.bHumblyChk);
	//m_chkHumbly.ShowWindow(SW_HIDE);

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);

	UpdateData(FALSE);
}

void CCMSectItemCon::ShowIData2Dlg()
{
	CString str;
	// write //////////////////////////////////////
	str.Format(_T("%g"), m_DataI.SectAfter.SectI.Size[0]); m_swidth.SetWindowText(str);
	str.Format(_T("%g"), m_DataI.SectAfter.SectI.Size[1]); m_CTC.SetWindowText(str);
	m_gnum.SetValue(m_DataI.SectAfter.SectI.BuiltUpFlag);

	str.Format(_T("%g"), m_DataI.SectAfter.SectJ.Size[0]); m_Bc.SetWindowText(str);
	str.Format(_T("%g"), m_DataI.SectAfter.SectJ.Size[1]); m_Tc.SetWindowText(str);
	str.Format(_T("%g"), m_DataI.SectAfter.SectJ.Size[2]); m_Hh.SetWindowText(str);

	str.Format(_T("%g"), m_DataI.SectBefore.SectI.Size[0]); m_Hw.SetWindowText(str);
	str.Format(_T("%g"), m_DataI.SectBefore.SectI.Size[1]); m_tw.SetWindowText(str);
	str.Format(_T("%g"), m_DataI.SectBefore.SectI.Size[2]); m_B1.SetWindowText(str);
	str.Format(_T("%g"), m_DataI.SectBefore.SectI.Size[3]); m_Tf1.SetWindowText(str);
	str.Format(_T("%g"), m_DataI.SectBefore.SectI.Size[4]); m_B2.SetWindowText(str);
	str.Format(_T("%g"), m_DataI.SectBefore.SectI.Size[5]); m_Tf2.SetWindowText(str);

	m_ESEC.SetEditUnit(m_DataI.SectBefore.Matl_Elast);     //str.Format(_T("%g"), m_DataI.SectBefore.Matl_Elast); m_ESEC.SetWindowText(str);
	m_DSDC.SetEditUnit(m_DataI.SectBefore.Matl_Density);   //str.Format(_T("%g"), m_DataI.SectBefore.Matl_Density); m_DSDC.SetWindowText(str);
	m_PS  .SetEditUnit(m_DataI.SectBefore.Matl_Poisson_S); //str.Format(_T("%g"), m_DataI.SectBefore.Matl_Poisson_S); m_PS.SetWindowText(str);
	m_PC  .SetEditUnit(m_DataI.SectBefore.Matl_Poisson_C); //str.Format(_T("%g"), m_DataI.SectBefore.Matl_Poisson_C); m_PC.SetWindowText(str);
	m_TSTC.SetEditUnit(m_DataI.SectBefore.Matl_Thermal);   //str.Format(_T("%g"), m_DataI.SectBefore.Matl_Thermal); m_TSTC.SetWindowText(str);

	m_chkMat.SetCheck(m_DataI.SectBefore.bMultipleElastic);
	m_edtMat1.SetEditUnit(m_DataI.SectBefore.dLongTermESEC);  //str.Format(_T("%g"), m_DataI.SectBefore.dLongTermESEC); m_edtMat1.SetWindowText(str);
	m_edtMat2.SetEditUnit(m_DataI.SectBefore.dShrinkageESEC); //str.Format(_T("%g"), m_DataI.SectBefore.dShrinkageESEC); m_edtMat2.SetWindowText(str);

	///////////////////////////////////////////////
	m_N1.SetValue(0);
	m_N2.SetValue(0);
	m_Bf1.SetWindowText(_T("0"));
	m_Bf2.SetWindowText(_T("0"));
	m_Bf3.SetWindowText(_T("0"));
	m_tfp.SetWindowText(_T("0"));
	m_Hr1.SetWindowText(_T("0"));
	m_Hr2.SetWindowText(_T("0"));
	m_Tr1.SetWindowText(_T("0"));
	m_Tr2.SetWindowText(_T("0"));

	// set offset point
	if (m_DataI.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataI.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_DataI.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_DataI.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// Humbly Check
	m_chkHumbly.SetCheck((int)m_DataI.SectBefore.bHumblyChk);
	//m_chkHumbly.ShowWindow(SW_HIDE);

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);

	UpdateData(FALSE);
}


void CCMSectItemCon::ShowCIData2Dlg()
{
	CString str;
	str.Format(_T("%g"), m_DataCI.SectAfter.SectI.Size[0]); m_swidth.SetWindowText(str);
	str.Format(_T("%g"), m_DataCI.SectAfter.SectI.Size[1]); m_CTC.SetWindowText(str);
	m_gnum.SetValue(m_DataCI.SectAfter.SectI.BuiltUpFlag);

	str.Format(_T("%g"), m_DataCI.SectAfter.SectJ.Size[0]); m_Bc.SetWindowText(str);
	str.Format(_T("%g"), m_DataCI.SectAfter.SectJ.Size[1]); m_Tc.SetWindowText(str);
	str.Format(_T("%g"), m_DataCI.SectAfter.SectJ.Size[2]); m_Hh.SetWindowText(str);
	str.Format(_T("%g"), m_DataCI.SectAfter.SectJ.Size[3]); m_Sg.SetWindowText(str);

	//  ShowData();

	m_ESEC.SetEditUnit(m_DataCI.SectBefore.Matl_Elast); 	//str.Format(_T("%g"), m_DataCI.SectBefore.Matl_Elast);   m_ESEC.SetWindowText(str);
	m_DSDC.SetEditUnit(m_DataCI.SectBefore.Matl_Density); 	//str.Format(_T("%g"), m_DataCI.SectBefore.Matl_Density); m_DSDC.SetWindowText(str);
	m_PS  .SetEditUnit(m_DataCI.SectBefore.Matl_Poisson_S); //str.Format(_T("%g"), m_DataCI.SectBefore.Matl_Poisson_S); m_PS.SetWindowText(str);
	m_PC  .SetEditUnit(m_DataCI.SectBefore.Matl_Poisson_C); //str.Format(_T("%g"), m_DataCI.SectBefore.Matl_Poisson_C); m_PC.SetWindowText(str);
	m_TSTC.SetEditUnit(m_DataCI.SectBefore.Matl_Thermal); 	//str.Format(_T("%g"), m_DataCI.SectBefore.Matl_Thermal); m_TSTC.SetWindowText(str);

	// set offset point
	if (m_DataCI.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataCI.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_DataCI.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_DataCI.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// Humbly Check
	m_chkHumbly.SetCheck((int)m_DataCI.SectBefore.bHumblyChk);
	//m_chkHumbly.ShowWindow(SW_HIDE);

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);

	m_chkMat.SetCheck(m_DataCI.SectBefore.bMultipleElastic);
	m_edtMat1.SetEditUnit(m_DataCI.SectBefore.dLongTermESEC);  //str.Format(_T("%g"), m_DataCI.SectBefore.dLongTermESEC); m_edtMat1.SetWindowText(str);
	m_edtMat2.SetEditUnit(m_DataCI.SectBefore.dShrinkageESEC); //str.Format(_T("%g"), m_DataCI.SectBefore.dShrinkageESEC); m_edtMat2.SetWindowText(str);
}

void CCMSectItemCon::ShowCTData2Dlg()
{
	CString str;
	str.Format(_T("%g"), m_DataCT.SectAfter.SectI.Size[0]); m_swidth.SetWindowText(str);
	str.Format(_T("%g"), m_DataCT.SectAfter.SectI.Size[1]); m_CTC.SetWindowText(str);
	m_gnum.SetValue(m_DataCT.SectAfter.SectI.BuiltUpFlag);

	str.Format(_T("%g"), m_DataCT.SectAfter.SectJ.Size[0]); m_Bc.SetWindowText(str);
	str.Format(_T("%g"), m_DataCT.SectAfter.SectJ.Size[1]); m_Tc.SetWindowText(str);
	str.Format(_T("%g"), m_DataCT.SectAfter.SectJ.Size[2]); m_Hh.SetWindowText(str);
	str.Format(_T("%g"), m_DataCT.SectAfter.SectJ.Size[3]); m_Sg.SetWindowText(str);

	
	//  ShowData();

	m_ESEC.SetEditUnit(m_DataCT.SectBefore.Matl_Elast); 	    //str.Format(_T("%g"), m_DataCT.SectBefore.Matl_Elast);   m_ESEC.SetWindowText(str);
	m_DSDC.SetEditUnit(m_DataCT.SectBefore.Matl_Density);   	//str.Format(_T("%g"), m_DataCT.SectBefore.Matl_Density); m_DSDC.SetWindowText(str);
	m_PS  .SetEditUnit(m_DataCT.SectBefore.Matl_Poisson_S); 	//str.Format(_T("%g"), m_DataCT.SectBefore.Matl_Poisson_S); m_PS.SetWindowText(str);
	m_PC  .SetEditUnit(m_DataCT.SectBefore.Matl_Poisson_C); 	//str.Format(_T("%g"), m_DataCT.SectBefore.Matl_Poisson_C); m_PC.SetWindowText(str);
	m_TSTC.SetEditUnit(m_DataCT.SectBefore.Matl_Thermal);   	//str.Format(_T("%g"), m_DataCT.SectBefore.Matl_Thermal); m_TSTC.SetWindowText(str);

	// set offset point
	if (m_DataCT.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataCT.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_DataCT.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_DataCT.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// Humbly Check
	m_chkHumbly.SetCheck((int)m_DataCT.SectBefore.bHumblyChk);
	//m_chkHumbly.ShowWindow(SW_HIDE);

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);

	m_chkMat.SetCheck(m_DataCT.SectBefore.bMultipleElastic);
	m_edtMat1.SetEditUnit(m_DataCT.SectBefore.dLongTermESEC);  	//str.Format(_T("%g"), m_DataCT.SectBefore.dLongTermESEC); m_edtMat1.SetWindowText(str);
	m_edtMat2.SetEditUnit(m_DataCT.SectBefore.dShrinkageESEC); 	//str.Format(_T("%g"), m_DataCT.SectBefore.dShrinkageESEC); m_edtMat2.SetWindowText(str);
}

void CCMSectItemCon::ShowPCData2Dlg(BOOL& bHumblyShow)
{
	CString str;
	str.Format(_T("%g"), m_DataPC.SectAfter.SectI.Size[0]); m_swidth.SetWindowText(str);
	str.Format(_T("%g"), m_DataPC.SectAfter.SectI.Size[1]); m_CTC.SetWindowText(str);
	m_gnum.SetValue(m_DataPC.SectAfter.SectI.BuiltUpFlag);

	str.Format(_T("%g"), m_DataPC.SectAfter.SectJ.Size[0]); m_Bc.SetWindowText(str);
	str.Format(_T("%g"), m_DataPC.SectAfter.SectJ.Size[1]); m_Tc.SetWindowText(str);
	str.Format(_T("%g"), m_DataPC.SectAfter.SectJ.Size[2]); m_Hh.SetWindowText(str);
	str.Format(_T("%g"), m_DataPC.SectAfter.SectJ.Size[3]); m_Sg.SetWindowText(str);

	m_ESEC.SetEditUnit(m_DataPC.SectBefore.Matl_Elast); 	//str.Format(_T("%g"), m_DataPC.SectBefore.Matl_Elast);   m_ESEC.SetWindowText(str);
	m_DSDC.SetEditUnit(m_DataPC.SectBefore.Matl_Density); 	//str.Format(_T("%g"), m_DataPC.SectBefore.Matl_Density); m_DSDC.SetWindowText(str);
	m_PS  .SetEditUnit(m_DataPC.SectBefore.Matl_Poisson_S); //str.Format(_T("%g"), m_DataPC.SectBefore.Matl_Poisson_S); m_PS.SetWindowText(str);
	m_PC  .SetEditUnit(m_DataPC.SectBefore.Matl_Poisson_C); //str.Format(_T("%g"), m_DataPC.SectBefore.Matl_Poisson_C); m_PC.SetWindowText(str);
	m_TSTC.SetEditUnit(m_DataPC.SectBefore.Matl_Thermal); 	//str.Format(_T("%g"), m_DataPC.SectBefore.Matl_Thermal); m_TSTC.SetWindowText(str);

	// set offset point
	if (m_DataPC.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataPC.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_DataPC.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_DataPC.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// Humbly Check
	m_chkHumbly.SetCheck((int)m_DataPC.SectBefore.bHumblyChk);

	if(CSectDB::IsClosedAftercomposite(m_DataPC))
	{
		bHumblyShow = FALSE;
	}

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);

	m_chkMat.SetCheck(m_DataPC.SectBefore.bMultipleElastic);
	m_edtMat1.SetEditUnit(m_DataPC.SectBefore.dLongTermESEC);  //str.Format(_T("%g"), m_DataPC.SectBefore.dLongTermESEC); m_edtMat1.SetWindowText(str);
	m_edtMat2.SetEditUnit(m_DataPC.SectBefore.dShrinkageESEC); //str.Format(_T("%g"), m_DataPC.SectBefore.dShrinkageESEC); m_edtMat2.SetWindowText(str);
}

void CCMSectItemCon::ShowGeneralData2Dlg() // later
{
	// name
	m_wndName.SetWindowText(m_Data.SName);

	// shape
	/*
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	SetCurShapeByIndex(nShapeIndex);
	*/
	// show/hide
	/*
	CDlgUtil::CtrlShowHide(this, m_aCtrlROct, m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_ROCT);
	CDlgUtil::CtrlShowHide(this, m_aCtrlGS, m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_GEN);
	if (m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_ROCT)
	{
	CString strCellNum;
	strCellNum.Format(_T("%d"), m_Data.SectBefore.nCellShape);
	m_editCellNum.SetWindowText(strCellNum);
	}
	*/

	// BuiltUp
	//SetBuiltUp();

	m_wndGridGeneral.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
#ifdef _MGEN_CH
	if (m_Data.bInputSec)
	{
		m_wndGridGeneral.SetWarpingEffect(TRUE);
	}
#endif
	// size and stiffness
	m_wndGridGeneral.ShowData();

	// offset
	CStringArray arOffsetNameList;
	CSectUtil::GetOffsetNameList(arOffsetNameList);
	if (m_DataGeneral.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = arOffsetNameList[m_DataGeneral.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_DataGeneral.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_DataGeneral.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// Humbly Check
	//m_chkHumbly.SetCheck((int)m_DataGeneral.SectBefore.bHumblyChk);
	m_chkHumbly.ShowWindow(SW_HIDE);

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);

	//CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, m_DataGeneral.SectBefore.nCalcStiffOpt);
	//CDlgUtil::CtrlShowHide(this, m_aCtrlGS, m_DataGeneral.SectBefore.Shape==D_SECT_SHAPE_REG_GEN);
	/*
	if (m_DataGeneral.nStype==D_SECT_TYPE_COMPO_G && m_DataGeneral.SectBefore.SectI.aLine.GetSize() > 0)
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, TRUE);
	else
	*/
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE); // 20080604 mylee - 무조건 FALSE

	// General Part List
	SetGeneralPartList();

	// Before Composite
	int nSel = m_DataGeneral.SectBefore.SectI.nBeforePart-1;
	if(nSel < 0) nSel = 0;
	m_cmbBeforePart.SetCurSel(nSel);

	//  int nPartSize = m_DataGeneral.SectBefore.SectI.aGeneralPart.GetSize();
	//   if(nPartSize==0)
	//   {
	//     m_wndGridGeneral.SetDataSource(&m_DataGeneral.SectBefore.SectI, D_SECT_TYPE_COMPO_G, &m_DataGeneral.SectBefore.nCellShape);
	//   }
	//   else
	//   {
	//     int nPart = nPartSize-1;
	//     m_wndGridGeneral.SetDataSource2(&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].StiffnessSum,
	//       &m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].PeriOutSum,
	//       &m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].PeriInSum,
	//       &m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].YBarSum,
	//       &m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].ZBarSum,
	//       &m_DataGeneral.SectBefore.SectI, D_SECT_TYPE_COMPO_G, &m_DataGeneral.SectBefore.nCellShape);
	//  }

	// guide bitmap / section preview
	//ChangeBitmap(); // Change guide bitmap
	m_wndSecViewGeneral.Invalidate();

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);
}

void CCMSectItemCon::ShowTubData2Dlg()
{
	CString str;
	// write //////////////////////////////////////
	str.Format(_T("%g"), m_DataTub.SectAfter.SectI.Size[0]); m_swidth.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectAfter.SectI.Size[1]); m_CTC.SetWindowText(str);

	m_gnum.SetValue(m_DataTub.SectAfter.SectI.BuiltUpFlag);

	str.Format(_T("%g"), m_DataTub.SectAfter.SectJ.Size[0]);
	m_Bc.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectAfter.SectJ.Size[1]); m_Tc.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectAfter.SectJ.Size[2]); m_Hh.SetWindowText(str);

	str.Format(_T("%g"), m_DataTub.SectBefore.SectI.Size[0]); m_Hw.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectI.Size[1]); m_tw.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectI.Size[2]); m_B1.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectI.Size[3]); m_Bf1.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectI.Size[4]); m_Tf1.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectI.Size[5]); m_B2.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectI.Size[6]); m_Bf2.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectI.Size[7]); m_Tf2.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectI.Size[8]); m_Bf3.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectI.Size[9]); m_tfp.SetWindowText(str);

	m_N1.SetValue(m_DataTub.SectBefore.nStiffNum[0]);//.nLytype;
	m_N2.SetValue(m_DataTub.SectBefore.nStiffNum[1]);//.nLztype;
	str.Format(_T("%g"), m_DataTub.SectBefore.SectJ.Size[2]); m_Hr1.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectJ.Size[3]); m_Hr2.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectJ.Size[4]); m_Tr1.SetWindowText(str);
	str.Format(_T("%g"), m_DataTub.SectBefore.SectJ.Size[5]); m_Tr2.SetWindowText(str);

	m_ESEC.SetEditUnit(m_DataTub.SectBefore.Matl_Elast); 	    //str.Format(_T("%g"), m_DataTub.SectBefore.Matl_Elast);   m_ESEC.SetWindowText(str);
	m_DSDC.SetEditUnit(m_DataTub.SectBefore.Matl_Density);    	//str.Format(_T("%g"), m_DataTub.SectBefore.Matl_Density); m_DSDC.SetWindowText(str);
	m_PS  .SetEditUnit(m_DataTub.SectBefore.Matl_Poisson_S); 	//str.Format(_T("%g"), m_DataTub.SectBefore.Matl_Poisson_S); m_PS.SetWindowText(str);
	m_PC  .SetEditUnit(m_DataTub.SectBefore.Matl_Poisson_C); 	//str.Format(_T("%g"), m_DataTub.SectBefore.Matl_Poisson_C); m_PC.SetWindowText(str);
	m_TSTC.SetEditUnit(m_DataTub.SectBefore.Matl_Thermal); 	    //str.Format(_T("%g"), m_DataTub.SectBefore.Matl_Thermal); m_TSTC.SetWindowText(str);

	m_chkMat.SetCheck(m_DataTub.SectBefore.bMultipleElastic);
	m_edtMat1.SetEditUnit(m_DataTub.SectBefore.dLongTermESEC); 	//str.Format(_T("%g"), m_DataTub.SectBefore.dLongTermESEC); m_edtMat1.SetWindowText(str);
	m_edtMat2.SetEditUnit(m_DataTub.SectBefore.dShrinkageESEC); 	//str.Format(_T("%g"), m_DataTub.SectBefore.dShrinkageESEC); m_edtMat2.SetWindowText(str);

	///////////////////////////////////////////////

	// set offset point
	if (m_DataTub.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataTub.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_DataTub.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_DataTub.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// Humbly Check
	m_chkHumblyBefore.SetCheck((int)m_DataTub.SectBefore.bHumblyChk);
	m_chkHumblyAfter.SetCheck((int)m_DataTub.SectAfter.bHumblyChk);
	//m_chkHumbly.ShowWindow(SW_HIDE);

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);

	UpdateData(FALSE);
}

void CCMSectItemCon::SetGeneralPartList()
{
	int nPartSize = m_DataGeneral.SectBefore.SectI.aGeneralPart.GetSize();
	int nIndex;
	CString str;
	CString strPART1 = _LS(IDS_WG_CMD_SECT_ITEM_PART1);

	m_cmbPart.ResetContent();
	m_cmbPart.SetRedraw(FALSE);
	nIndex = m_cmbPart.AddString(_LS(IDS_WG_CMD_SECT_ITEM_TOTAL));
	m_cmbPart.SetItemData(nIndex, nIndex);
	int i = 0;
	for (i = 0; i < nPartSize; ++i)
	{
		str.Format(_LS(IDS_WG_CMD_SECT_ITEM_PART_INT), i + 1);
		nIndex = m_cmbPart.AddString(str);
		m_cmbPart.SetItemData(nIndex, nIndex);
	}
	for(i=1 ; i<(nPartSize-1) ; ++i)
	{
		str = strPART1;
		CString strTmp;
		for(int j=0 ; j<i ; ++j)
		{
			strTmp.Format(_T("+%d"), j+2);
			str += strTmp;
		}
		nIndex = m_cmbPart.AddString(str);
		m_cmbPart.SetItemData(nIndex, nIndex);
	}
	m_cmbPart.SetCurSel(0);

	m_cmbBeforePart.ResetContent();
	m_cmbBeforePart.SetRedraw(FALSE);
	nIndex = m_cmbBeforePart.AddString(strPART1);
	m_cmbBeforePart.SetItemData(nIndex, nIndex);
	for(i=2 ; i<nPartSize ; ++i)
	{
		str = strPART1;
		CString strTmp;
		for(int j=2 ; j<=i ; ++j)
		{
			strTmp.Format(_T("+%d"), j);
			str += strTmp;
		}
		nIndex = m_cmbBeforePart.AddString(str);
		m_cmbBeforePart.SetItemData(nIndex, nIndex);
	}
	m_cmbBeforePart.SetCurSel(0);
	m_cmbPart.SetRedraw(TRUE);
	m_cmbBeforePart.SetRedraw(TRUE);
}


void CCMSectItemCon::SetSectionNameList()
{
	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSect();
	if(nItemCount == 0) return;

	m_wndFirstName.ResetContent();
	m_wndFirstName.SetRedraw(FALSE);
	m_wndSecondName.ResetContent();
	m_wndSecondName.SetRedraw(FALSE);
	if(nItemCount)
	{
		T_SECT_K Key;
		T_SECT_D Data;
		int nCount;
		POSITION pos;

		int* KeyBuf=new int[nItemCount];
		nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartSect();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextSect(pos, Key, Data);
			KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_SECT_K), CCompFunc::UINTAsc);

		int nIndex;
		CString csName;
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetSect(Key,Data);

			// Construction section은 component에서 제외
			if (Data.nStype == D_SECT_TYPE_CONSTRUCTION ||
				Data.nStype == D_SECT_TYPE_COMPO_B      ||
				Data.nStype == D_SECT_TYPE_COMPO_I      ||
				Data.nStype == D_SECT_TYPE_COMPO_TUB    ||
				Data.nStype == D_SECT_TYPE_COMPO_CI     ||
				Data.nStype == D_SECT_TYPE_COMPO_CT     ||
				Data.nStype == D_SECT_TYPE_COMPO_PC     || // MNET:3022 20080103 mylee
				Data.nStype == D_SECT_TYPE_COMPO_G      || // MNET:3022 20080103 mylee
				Data.nStype == D_SECT_TYPE_PSC           ) continue;
			if (Data.nStype == D_SECT_TYPE_TAPERED &&
				Data.SectBefore.nStype == D_SECT_TYPE_PSC) continue;
			if (Data.nStype == D_SECT_TYPE_TAPERED) // MNET:3022 20080103 mylee
			{
				if     (Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B)   continue;
				else if(Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I)   continue;
				else if(Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB) continue;
				else if(Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI)  continue;
				else if(Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT)  continue;
				else if(Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G)   continue;
			}

			if (Data.nStype == D_SECT_TYPE_REGULAR ||
				Data.nStype == D_SECT_TYPE_TAPERED  )
			{
				if (Data.SectBefore.Shape == D_SECT_SHAPE_REG_BSTF) continue;
			}

			csName.Format(_T("%d: %s"), Key, Data.SName);
			nIndex = m_wndFirstName.AddString(csName);
			m_wndFirstName.SetItemData(nIndex, (DWORD)Key);
			nIndex = m_wndSecondName.AddString(csName);
			m_wndSecondName.SetItemData(nIndex, (DWORD)Key);
		}
		delete []KeyBuf;
	}
	m_wndFirstName.SetRedraw(TRUE);
	m_wndSecondName.SetRedraw(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemCon message handlers

BOOL CCMSectItemCon::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetRedraw(FALSE);

	InitControls(); // 1번만 초기화 해주면 되는 것 들을 초기화 한다.

	SetTempMembValWithCbxAndView();

	ShowKey2Dlg();
	ShowData2Dlg();

	// 초기화 되었음을 표시
	m_bInitDone = TRUE;

	if (m_bDesign) m_wndID.EnableWindow(FALSE);

	// 컨트롤 조정
	AlignControls();
	AlignControls_7thDOF();

	SaveDynamicSizeCtrlPos();
	ChangeSectType();
	OnCmdMultiModulusCheck(); // added by lmy
	OnChkPSCDesign();

	InitSpinCtrl();

	//China Gen  modify by maxiao 2013-7-5
#if defined(_MGEN) && defined(_CH)
	GetDlgItem(IDC_CMD_SP_ID_GNUM)->SetWindowText(_T("1"));
	UpdateData(FALSE);
#endif

	HideNotUsedCtrl();

	UpdateData(FALSE);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	if (CTestEnvMgr::GetTestEnvST(_LSX(Test_PSCComp)) == _LSX(yes))
		m_Sg.ShowWindow(SW_SHOW);
	else
		m_Sg.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemCon::SaveDynamicSizeCtrlPos()
{
	GetDlgItem(IDC_BOX_GROUP3)->GetWindowRect(m_rectMatlGrpSize[0]); // SmallSize는 원래크기
	GetDlgItem(IDC_CMD_SECT_SIZE_GRP)->GetWindowRect(m_rectMatlGrpSize[1]); // BigSize는 IDC_CMD_SECT_SIZE_GRP와 x좌표만 맞춤
	m_rectMatlGrpSize[1].top = m_rectMatlGrpSize[0].top;
	m_rectMatlGrpSize[1].bottom = m_rectMatlGrpSize[0].bottom;
	for (int i = 0; i < 2; i++)
	{
		ScreenToClient(m_rectMatlGrpSize[i]);
	}
}

void CCMSectItemCon::SetTempMembValWithCbxAndView()
{
	if (!m_bModify && !m_bInitDone)
	{
		// 시작 번호는 현재 번호에서 DB가 생성해 주는 다음 번호로 설정
		if(m_bWorkTab)
			m_Key = m_FixID;
		else
			m_Key = m_pDoc->m_pAttrCtrl->GetStartNumSect();

		m_Data.Initialize();
		m_Data.nStype = D_SECT_TYPE_CONSTRUCTION;

		m_DataBox.Initialize();
		m_DataI.Initialize();
		m_DataCI.Initialize();
		m_DataCT.Initialize();
		m_DataUser.Initialize();
		m_DataPC.Initialize();
		m_DataGeneral.Initialize();
		m_DataTub.Initialize();
		m_DataStlGirderBox.Initialize();
		m_DataStlGirderI.Initialize();
		m_DataStlGirderTub.Initialize();
		m_PSCDesign.Initialize();
		m_PscValueSectK = 0;

		m_DataBox.nStype = D_SECT_TYPE_COMPO_B;
		m_DataI.nStype = D_SECT_TYPE_COMPO_I;
		m_DataCI.nStype = D_SECT_TYPE_COMPO_CI;
		m_DataCI.SectBefore.nCellType = 2;
		m_DataCI.SectBefore.nCellShape = 2;
		m_DataCT.nStype = D_SECT_TYPE_COMPO_CT;
		m_DataUser.nStype = D_SECT_TYPE_CONSTRUCTION;
		m_DataPC.nStype = D_SECT_TYPE_COMPO_PC;
		m_DataGeneral.nStype = D_SECT_TYPE_COMPO_G;
		m_DataTub.nStype = D_SECT_TYPE_COMPO_TUB;
		m_DataStlGirderBox.nStype = D_SECT_TYPE_COMPO_STLG_B;
		m_DataStlGirderI.nStype = D_SECT_TYPE_COMPO_STLG_I;
		m_DataStlGirderTub.nStype = D_SECT_TYPE_COMPO_STLG_TUB;
	}

	if(m_bModify)
	{
		if(!m_bInitDone)
		{
			m_DataBox.Initialize();
			m_DataI.Initialize();
			m_DataCI.Initialize();
			m_DataCT.Initialize();
			m_DataUser.Initialize();
			m_DataPC.Initialize();
			m_DataGeneral.Initialize();
			m_DataTub.Initialize();
			m_DataStlGirderBox.Initialize();
			m_DataStlGirderI.Initialize();
			m_DataStlGirderTub.Initialize();
			m_PSCDesign.Initialize();
			m_PscValueSectK = 0;

			m_DataBox.nStype = D_SECT_TYPE_COMPO_B;
			m_DataI.nStype = D_SECT_TYPE_COMPO_I;
			m_DataCI.nStype = D_SECT_TYPE_COMPO_CI;
			m_DataCI.SectBefore.nCellType = 2;
			m_DataCI.SectBefore.nCellShape = 2;
			m_DataCT.nStype = D_SECT_TYPE_COMPO_CT;
			m_DataUser.nStype = D_SECT_TYPE_CONSTRUCTION;
			m_DataPC.nStype = D_SECT_TYPE_COMPO_PC;
			m_DataGeneral.nStype = D_SECT_TYPE_COMPO_G;
			m_DataTub.nStype = D_SECT_TYPE_COMPO_TUB;
			m_DataStlGirderBox.nStype = D_SECT_TYPE_COMPO_STLG_B;
			m_DataStlGirderI.nStype = D_SECT_TYPE_COMPO_STLG_I;
			m_DataStlGirderTub.nStype = D_SECT_TYPE_COMPO_STLG_TUB;
		}

		if(m_Data.nStype == D_SECT_TYPE_COMPO_B)
		{
			m_DataBox = m_Data;
			CDlgUtil::CobxSetCurSelItemData(m_cboSecType, 1);
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_I)
		{
			m_DataI = m_Data;
			CDlgUtil::CobxSetCurSelItemData(m_cboSecType, 2);
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_CI)
		{
			m_DataCI = m_Data;
			CDlgUtil::CobxSetCurSelItemData(m_cboSecType, 3);
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_CT)
		{
			m_DataCT = m_Data;
			CDlgUtil::CobxSetCurSelItemData(m_cboSecType, 4);
		}
		else if(m_Data.nStype == D_SECT_TYPE_CONSTRUCTION)
		{
			m_DataUser = m_Data;
			CDlgUtil::CobxSetCurSelItemData(m_cboSecType, 5);
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_PC)
		{
			m_DataPC = m_Data;
			CDlgUtil::CobxSetCurSelItemData(m_cboSecType, 6);
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_G)
		{
			m_DataGeneral = m_Data;
			CDlgUtil::CobxSetCurSelItemData(m_cboSecType, 7);

			// PSC Design
			if(!m_pDoc->m_pAttrCtrl2->GetSecp(m_Key, m_PSCDesign)) m_PSCDesign.Initialize();
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_TUB)
		{
			m_DataTub = m_Data;
			CDlgUtil::CobxSetCurSelItemData(m_cboSecType, 8);
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_B)
		{
			m_DataStlGirderBox = m_Data;
			CDlgUtil::CobxSetCurSelItemData(m_cboSecType, 9);
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_I)
		{
			m_DataStlGirderI = m_Data;
			CDlgUtil::CobxSetCurSelItemData(m_cboSecType, 10);
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
		{
			m_DataStlGirderTub = m_Data;
			CDlgUtil::CobxSetCurSelItemData(m_cboSecType, 11);
		}
		UpdateData(FALSE);
	}

	m_wndSecViewBox.SetDataSource(&m_DataBox, FALSE);
	m_wndSecViewI.SetDataSource(&m_DataI, FALSE);
	m_wndSecViewCI.SetDataSource(&m_DataCI);
	m_wndSecViewCT.SetDataSource(&m_DataCT);
	m_wndSecViewPC.SetDataSource(&m_DataPC);
	m_wndSecViewGeneral.SetDataSource(&m_DataGeneral);
	m_wndSecViewTub.SetDataSource(&m_DataTub, FALSE);
	m_wndSecViewStlGirderBox.SetDataSource(&m_DataStlGirderBox, FALSE);
	m_wndSecViewStlGirderI.SetDataSource(&m_DataStlGirderI, FALSE);
	m_wndSecViewStlGirderTub.SetDataSource(&m_DataStlGirderTub, FALSE);


	m_wndSecViewFirst.SetDataSource(&m_DataUser, FALSE);
	m_wndSecViewSecond.SetDataSource(&m_DataUser, TRUE);
	m_wndGridCI.SetDataSource(&m_DataCI);
	m_wndGridCT.SetDataSource(&m_DataCT);



	m_wndGridGeneral.SetDataSource(&m_DataGeneral.SectAfter.SectI, &m_PSCDesign, D_SECT_TYPE_COMPO_G, &m_DataGeneral.SectAfter.nCellShape);
	//int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	int nShapeIndex = 21; // 해줘도 되고 안해줘도 됨(내부에서 자동 세팅)
	m_wndGridGeneral.ChangeShape(nShapeIndex);
}

void CCMSectItemCon::InitSpinCtrl()
{
	// spin
	m_gnum.SetRange(0, 10000);
	m_gnum.SetValue(m_Data.SectAfter.SectI.BuiltUpFlag); // girder number 관련 Edit Control Text 설정.
	m_gnum.SetInteger(TRUE);

	m_N1.SetRange(0, 10000);
	m_N1.SetValue(m_Data.SectBefore.nStiffNum[0]);//.nLytype);
	m_N1.SetInteger(TRUE);

	m_N2.SetRange(0, 10000);
	m_N2.SetValue(m_Data.SectBefore.nStiffNum[1]);//.nLztype);
	m_N2.SetInteger(TRUE);
}

void CCMSectItemCon::HideNotUsedCtrl()
{
	// Stiffener 관련 옵션 이제 사용 안한다!!
	GetDlgItem(IDC_STATIC17)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SP_ID_N1)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CMD_SP_ID_N1_SPIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC18)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SP_ID_N2)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CMD_SP_ID_N2_SPIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC19)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SP_ID_HR1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC20)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SP_ID_HR2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SP_ID_UNIT11)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC21)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SP_ID_TR1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC22)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SP_ID_TR2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SP_ID_UNIT12)->ShowWindow(SW_HIDE);
}

void CCMSectItemCon::AlignControls()
{
	CRect rRef;
	CRect rMove;
	int nDistX;

	// 일반 Ctrl들
	GetDlgItem(IDC_AFTER_GROUP)->GetWindowRect(rRef);
	GetDlgItem(IDC_BOX_GROUP2)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlBox, nDistX);
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlGirder, nDistX);
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlMat, nDistX);

	GetDlgItem(IDC_AFTER_GROUP)->GetWindowRect(rRef);
	GetDlgItem(IDC_BOX_CI_GROUP)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlCI, nDistX);

	GetDlgItem(IDC_AFTER_GROUP)->GetWindowRect(rRef);
	GetDlgItem(IDC_BOX_CT_GROUP)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlCT, nDistX);

	GetDlgItem(IDC_AFTER_GROUP)->GetWindowRect(rRef);
	GetDlgItem(IDC_PSC_FRM)    ->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlPC, nDistX);

	GetDlgItem(IDC_AFTER_GROUP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_GENERAL_STC)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlGeneral, nDistX);

	GetDlgItem(IDC_AFTER_GROUP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SECT_SLAB_GRP)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlStlGirder, nDistX);  

	// Preview와 GuideImage
	GetDlgItem(IDC_CMD_SP_ID_PREVIEW1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SP_ID_PREVIEW3)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_PreViewBox, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_PREVIEW1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SP_ID_PREVIEW4)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_PreViewI, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_PREVIEW1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SP_CI_PREVIEW1)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_PreViewCI, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_PREVIEW1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SP_CT_PREVIEW1)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_PreViewCT, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_PREVIEW1) ->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_PSC_PREVIEW1)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_PreViewPC, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_PREVIEW1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SP_ID_PREVIEW7)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_PreViewGeneral, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_PREVIEW1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_TUB_PREVIEW1)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_PreViewTub, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_PREVIEW1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SECT_B_VIEW)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_PreViewStlGirderBox, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_PREVIEW1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SECT_I_VIEW)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_PreViewStlGirderI, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_PREVIEW1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SECT_TUB_VIEW)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_PreViewStlGirderTub, nDistX);  

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_AFTER_GROUP);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.right = rectLast.right+1;
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);

}

void CCMSectItemCon::AlignControls_7thDOF()
{
	const double dSpaceBetweenControls = 4.0;

	// Slab width 입력 사용안함.
	if(CSectDB::IsEnable7thDOF())
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrSlabInfo, FALSE);

		CRect rRef;
		CRect rMove;
		// material
		GetDlgItem(IDC_CMD_SECT_SIZE_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_BOX_GROUP3)->GetWindowRect(rMove);
		const int nDistY = rRef.bottom + dSpaceBetweenControls - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlMat, nDistY);

		// Resize
		GetDlgItem(IDC_BOX_GROUP2)->GetWindowRect(rMove);
		const int nResizeY = rRef.bottom - rMove.bottom;
		std::list<UINT> lCtrlID;
		lCtrlID.push_back(IDC_BOX_GROUP2);
		lCtrlID.push_back(IDC_BOX_CI_GROUP);
		lCtrlID.push_back(IDC_CMD_SP_CI_TABLE);
		lCtrlID.push_back(IDC_BOX_CT_GROUP);
		lCtrlID.push_back(IDC_CMD_SP_CT_TABLE);
		lCtrlID.push_back(IDC_PSC_FRM);
		CDlgUtil::CtrlResizeBottom(this, lCtrlID, nResizeY, FALSE);
	}
	else
	{
		CRect rRef;
		CRect rMove;
		// < Steel Box(Type1) 일때>
		//슬래브폭, 보 수, 보 폭, RC슬래프 그룹BOX, 철골보 그룹BOX, 재질 그룹BOX Y방향 위치이동
		GetDlgItem(IDC_STATIC4)->GetWindowRect(rRef);
		GetDlgItem(IDC_STATIC3)->GetWindowRect(rMove);

		int nDistY = rRef.bottom - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlBox, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlGirder, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlMat, nDistY);

		// < 합셩-PC 일때>
		GetDlgItem(IDC_BOX_GROUP1)->GetWindowRect(rRef);
		GetDlgItem(IDC_PSC_FRM)->GetWindowRect(rMove);
		nDistY = (rRef.bottom + dSpaceBetweenControls) - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlPC, nDistY);

		//if(m_nCurType==3) // Composite-I  
		GetDlgItem(IDC_BOX_CI_GROUP)->GetWindowRect(rMove);
		nDistY = (rRef.bottom + dSpaceBetweenControls) - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlCI, nDistY);

		//if(m_nCurType==4) // Composite-T
		GetDlgItem(IDC_BOX_CT_GROUP)->GetWindowRect(rMove);
		nDistY = (rRef.bottom + dSpaceBetweenControls) - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlCT, nDistY);

		//전단 변형의 고려 체크박스 Y방향 위치이동
		CArray<UINT, UINT> m_aCtrlCheckBoxShearDeform;
		m_aCtrlCheckBoxShearDeform.Add(IDC_CMD_SHEAR_DEFORM);
		GetDlgItem(IDC_BOX_GROUP3)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_SHEAR_DEFORM)->GetWindowRect(rMove);
		nDistY = (rRef.bottom + dSpaceBetweenControls) - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlCheckBoxShearDeform, nDistY);
	}
}

void CCMSectItemCon::OnChangeFirstName() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndFirstName.GetCurSel();
	if (nIndex < 0) return;
	T_SECT_K Key = m_wndFirstName.GetItemData(nIndex);
	T_SECT_D Data;
	m_pDoc->m_pAttrCtrl->GetSect(Key, Data);

	// MQC:2427-JHYUN-20090210
	// DB/User tab 에서 단면 입력시 User Type 일 경우에
	// Section > Composite > Section Type User 에서 
	// Before Composite, After Composite 에 Section Name 이 출력되지 않는 문제
	// CSectUtil::AssignPartOfConSect 를 고치면 다른데 영향을 미치므로 일단 요 부분만 수정
	Data.SectBefore.SectI.SName = Data.SName;

	CSectUtil::AssignPartOfConSect(m_DataUser, Data, FALSE);
	//m_DataUser.SectBefore.nOffsetPoint = m_wndOffset.GetCurSel()+1;

	m_wndGridFirst.InitSectionByData(&m_DataUser, FALSE);
	//m_wndSecViewFirst.SetCentroidFlag(FALSE);
	m_wndSecViewFirst.Invalidate();	
}

void CCMSectItemCon::OnChangeSecondName() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndSecondName.GetCurSel();
	if (nIndex < 0) return;
	T_SECT_K Key = m_wndSecondName.GetItemData(nIndex);
	T_SECT_D Data;
	m_pDoc->m_pAttrCtrl->GetSect(Key, Data);

	// MQC:2427-JHYUN-20090210
	// DB/User tab 에서 단면 입력시 User Type 일 경우에
	// Section > Composite > Section Type User 에서 
	// Before Composite, After Composite 에 Section Name 이 출력되지 않는 문제
	// CSectUtil::AssignPartOfConSect 를 고치면 다른데 영향을 미치므로 일단 요 부분만 수정
	Data.SectBefore.SectI.SName = Data.SName;

	CSectUtil::AssignPartOfConSect(m_DataUser, Data, TRUE);
	//m_DataUser.SectAfter.nOffsetPoint = m_wndOffset.GetCurSel()+1;

	m_wndGridSecond.InitSectionByData(&m_DataUser, TRUE);
	//m_wndSecViewSecond.SetCentroidFlag(FALSE);
	m_wndSecViewSecond.Invalidate();		
}

void CCMSectItemCon::ChangeSectType()
{
	CDlgUtil::CobxGetItemDataByCurSel(m_cboSecType, m_nCurType);

	if     (m_nCurType == 1)  SaveBoxData(); // Steel Box
	else if(m_nCurType == 2)  SaveIData();   // Steel-I
	else if(m_nCurType == 3)  SaveCIData();  // Composite-I
	else if(m_nCurType == 4)  SaveCTData();  // Composite-T
	else if(m_nCurType == 6)  SavePCData();  // Composite-PSC
	else if(m_nCurType == 7)  SaveGeneralData();  // Composite General
	else if(m_nCurType == 8)  SaveTubData(); // Steel Tub
	else if(m_nCurType == 9)  SaveStlGirderBoxData(); // Comp Steel Girder Box
	else if(m_nCurType == 10) SaveStlGirderIData(); // Comp Steel Girder I
	else if(m_nCurType == 11) SaveStlGirderTubData(); // Comp Steel Girder Tub


	// MNET:2582 20061130 mylee
	if ( !(m_nCurType == 1 || m_nCurType == 2) )
	{
		m_chkMat.SetCheck(0);
		m_edtMat1.SetEditUnit(0);
		m_edtMat2.SetEditUnit(0);
	}
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlMulti, m_nCurType != 7); 
	OnCmdMultiModulusCheck();

	CButton* pBtnEsEg = (CButton*)GetDlgItem(IDC_STATIC23);
	CButton* pBtnDsDg = (CButton*)GetDlgItem(IDC_STATIC24);
	CButton* pBtnPs   = (CButton*)GetDlgItem(IDC_STATIC25);
	CButton* pBtnPc   = (CButton*)GetDlgItem(IDC_STATIC26);
	CButton* pBtnTsTg = (CButton*)GetDlgItem(IDC_STATIC27);
	CButton* pBtnCreep = (CButton*)GetDlgItem(IDC_CMD_MAT1_STC);
	CButton* pBtnShrinjkage = (CButton*)GetDlgItem(IDC_CMD_MAT2_STC);

	BOOL bCtrlBox=FALSE, bCtrlGirder=FALSE, bCtrlMat=FALSE, bCtrlI = FALSE, bCtrlCI=FALSE, bCtrlCT=FALSE, bCtrlUser=FALSE, bCtrlPC=FALSE, bCtrlTub=FALSE,
		bCtrlGeneral=FALSE, bStlGirderBox=FALSE, bStlGirderI=FALSE, bStlGirderTub=FALSE;
	BOOL bPrvBox=FALSE, bPrvI=FALSE, bPrvCI=FALSE, bPrvCT=FALSE, bPrvPC=FALSE, bPrvUser=FALSE, bPrvTub=FALSE,
		bPrvGeneral=FALSE, bPrevStlGirderBox=FALSE, bPrevStlGirderI=FALSE, bPrevStlGirderTub=FALSE;
	BOOL bBigMatlGrp = FALSE;
	BOOL bHumbly = FALSE;
	BOOL bHumblyBefore = FALSE;
	BOOL bHumblyAfter = FALSE;

	
	switch(m_nCurType)
	{
	case 1:   // Steel Box
		bCtrlBox    = TRUE;
		bCtrlGirder = TRUE;
		bCtrlMat    = TRUE;
		bPrvBox     = TRUE;
		bCtrlI      = TRUE;
		bCtrlTub    = FALSE;
		bHumbly     = TRUE;
		pBtnEsEg->SetWindowText(_T("Es / Ec"));
		pBtnDsDg->SetWindowText(_T("Ds / Dc"));
		pBtnPs->SetWindowText(_T("Ps"));
		pBtnPc->SetWindowText(_T("Pc"));
		pBtnTsTg->SetWindowText(_T("Ts / Tc"));
		pBtnCreep->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_CREEP));
		pBtnShrinjkage->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_SHRINKAGE));
		ShowBoxData2Dlg();
		break;
	case 2:   // Steel-I
		bCtrlBox    = TRUE;
		bCtrlGirder = TRUE;
		bCtrlMat    = TRUE;
		bPrvI       = TRUE;
		bCtrlI      = FALSE;
		bCtrlTub    = FALSE;
		bHumbly     = TRUE;
		pBtnEsEg->SetWindowText(_T("Es / Ec"));
		pBtnDsDg->SetWindowText(_T("Ds / Dc"));
		pBtnPs->SetWindowText(_T("Ps"));
		pBtnPc->SetWindowText(_T("Pc"));
		pBtnTsTg->SetWindowText(_T("Ts / Tc"));
		pBtnCreep->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_CREEP));
		pBtnShrinjkage->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_SHRINKAGE));
		ShowIData2Dlg();
		break;
	case 3:   // Composite-I
		bCtrlBox    = TRUE;
		bCtrlMat    = TRUE;
		bCtrlCI     = TRUE;
		bPrvCI      = TRUE;
		bHumbly     = TRUE;
		m_wndGridCI.ChangeShape(m_nCurType);  // 여기서 ShowData도 실행 
		pBtnEsEg->SetWindowText(_T("Egd/Esb"));
		pBtnDsDg->SetWindowText(_T("Dgd/Dsb"));
		pBtnPs->SetWindowText(_T("Pgd"));
		pBtnPc->SetWindowText(_T("Psb"));
		pBtnTsTg->SetWindowText(_T("Tgd/Tsb"));
		pBtnCreep->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_CONC_MULTI_CREEP));
		pBtnShrinjkage->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_CONC_MULTI_SHRINKAGE));
		ShowCIData2Dlg();
		break;
	case 4:   // Composite-T
		bCtrlBox    = TRUE;
		bCtrlMat    = TRUE;
		bCtrlCT     = TRUE;
		bPrvCT      = TRUE;
		bHumbly     = TRUE;
		m_wndGridCT.ChangeShape(m_nCurType);  // 여기서 ShowData도 실행 
		pBtnEsEg->SetWindowText(_T("Egd/Esb"));
		pBtnDsDg->SetWindowText(_T("Dgd/Dsb"));
		pBtnPs->SetWindowText(_T("Pgd"));
		pBtnPc->SetWindowText(_T("Psb"));
		pBtnTsTg->SetWindowText(_T("Tgd/Tsb"));
		pBtnCreep->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_CONC_MULTI_CREEP));
		pBtnShrinjkage->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_CONC_MULTI_SHRINKAGE));
		ShowCTData2Dlg();
		break;
	case 5:   // User
		bCtrlUser = TRUE;
		bPrvUser  = TRUE;
		ShowUserData2Dlg();
		break;
	case 6:   // Composite-PSC
		bCtrlBox    = TRUE;
		bCtrlMat    = TRUE;
		bCtrlPC     = TRUE;
		bPrvPC      = TRUE;
		bHumbly     = TRUE;
		pBtnEsEg->SetWindowText(_T("Egd/Esb"));
		pBtnDsDg->SetWindowText(_T("Dgd/Dsb"));
		pBtnPs->SetWindowText(_T("Pgd"));
		pBtnPc->SetWindowText(_T("Psb"));
		pBtnTsTg->SetWindowText(_T("Tgd/Tsb"));
		pBtnCreep->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_CONC_MULTI_CREEP));
		pBtnShrinjkage->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_CONC_MULTI_SHRINKAGE));
		ShowPCData2Dlg(bHumbly);
		break;
	case 7:   // General
		bCtrlGeneral = TRUE;
		bPrvGeneral  = TRUE;
		ShowGeneralData2Dlg();
		break;
	case 8:   // Steel Tub
		bCtrlBox    = TRUE;
		bCtrlGirder = TRUE;
		bCtrlMat    = TRUE;
		bPrvTub     = TRUE;
		bCtrlI      = TRUE;
		bCtrlTub    = TRUE;
		bHumbly     = TRUE;
		pBtnEsEg->SetWindowText(_T("Es / Ec"));
		pBtnDsDg->SetWindowText(_T("Ds / Dc"));
		pBtnPs->SetWindowText(_T("Ps"));
		pBtnPc->SetWindowText(_T("Pc"));
		pBtnTsTg->SetWindowText(_T("Ts / Tc"));
		pBtnCreep->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_CREEP));
		pBtnShrinjkage->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_SHRINKAGE));
		ShowTubData2Dlg();
		break;
	case 9:   // Composite Steel Girder Box     
		bCtrlMat          = TRUE;
		bStlGirderBox     = TRUE;
		bPrevStlGirderBox = TRUE;
		bBigMatlGrp       = TRUE;
		bHumbly			  = TRUE;
		pBtnEsEg->SetWindowText(_T("Es / Ec"));
		pBtnDsDg->SetWindowText(_T("Ds / Dc"));
		pBtnPs->SetWindowText(_T("Ps"));
		pBtnPc->SetWindowText(_T("Pc"));
		pBtnTsTg->SetWindowText(_T("Ts / Tc"));
		pBtnCreep->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_CREEP));
		pBtnShrinjkage->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_SHRINKAGE));
		ShowStlGirderBoxData2Dlg();
		break;
	case 10:   // Composite Steel Girder I     
		bCtrlMat        = TRUE;
		bStlGirderI     = TRUE;
		bPrevStlGirderI = TRUE;
		bBigMatlGrp       = TRUE;
		bHumbly			= TRUE;
		pBtnEsEg->SetWindowText(_T("Es / Ec"));
		pBtnDsDg->SetWindowText(_T("Ds / Dc"));
		pBtnPs->SetWindowText(_T("Ps"));
		pBtnPc->SetWindowText(_T("Pc"));
		pBtnTsTg->SetWindowText(_T("Ts / Tc"));
		pBtnCreep->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_CREEP));
		pBtnShrinjkage->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_SHRINKAGE));
		ShowStlGirderIData2Dlg();
		break;
	case 11:   // Composite Steel Girder Tub
		bCtrlMat          = TRUE;
		bStlGirderTub     = TRUE;
		bPrevStlGirderTub = TRUE;
		bBigMatlGrp       = TRUE;
		bHumbly			  = TRUE;
		pBtnEsEg->SetWindowText(_T("Es / Ec"));
		pBtnDsDg->SetWindowText(_T("Ds / Dc"));
		pBtnPs->SetWindowText(_T("Ps"));
		pBtnPc->SetWindowText(_T("Pc"));
		pBtnTsTg->SetWindowText(_T("Ts / Tc"));
		pBtnCreep->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_CREEP));
		pBtnShrinjkage->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_SHRINKAGE));
		ShowStlGirderTubData2Dlg();
		break;
	default: ASSERT(0); break;
	}
	CDlgUtil::CtrlShowHide(this, m_aCtrlBox,            bCtrlBox);
	CDlgUtil::CtrlShowHide(this, m_aCtrlGirder,         bCtrlGirder);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMat,            bCtrlMat);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCI,             bCtrlCI);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCT,             bCtrlCT);
	CDlgUtil::CtrlShowHide(this, m_aCtrlUser,           bCtrlUser);
	CDlgUtil::CtrlShowHide(this, m_aCtrlPC,             bCtrlPC);
	CDlgUtil::CtrlShowHide(this, m_aCtrlGeneral,        bCtrlGeneral);
	CDlgUtil::CtrlShowHide(this, m_aCtrlStlGirder,      bStlGirderBox || bStlGirderI || bStlGirderTub);

	CDlgUtil::CtrlShowHide(this, m_PreViewBox,          bPrvBox);
	CDlgUtil::CtrlShowHide(this, m_PreViewI,            bPrvI);
	CDlgUtil::CtrlShowHide(this, m_PreViewCI,           bPrvCI);
	CDlgUtil::CtrlShowHide(this, m_PreViewCT,           bPrvCT);
	CDlgUtil::CtrlShowHide(this, m_PreViewUser,         bPrvUser);
	CDlgUtil::CtrlShowHide(this, m_PreViewPC,           bPrvPC);
	CDlgUtil::CtrlShowHide(this, m_PreViewGeneral,      bPrvGeneral);
	CDlgUtil::CtrlShowHide(this, m_PreViewTub,          bPrvTub);
	CDlgUtil::CtrlShowHide(this, m_PreViewStlGirderBox, bPrevStlGirderBox);
	CDlgUtil::CtrlShowHide(this, m_PreViewStlGirderI,   bPrevStlGirderI);
	CDlgUtil::CtrlShowHide(this, m_PreViewStlGirderTub, bPrevStlGirderTub);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlI,         bCtrlI);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlTub,       bCtrlTub);

	BOOL bPSC = (m_nCurType == 3 || m_nCurType == 4);
	GetDlgItem(IDC_CMD_SECT_CENTROID_BTN)->EnableWindow(bPSC);

	GetDlgItem(IDC_CMD_SECT_CENTROID_BTN)->ShowWindow(!bPrvGeneral); // MNET:3022 20080103 mylee

	GetDlgItem(IDC_CMD_FEM_RDO)->ShowWindow(!bPrvGeneral);
	GetDlgItem(IDC_CMD_EQU_RDO2)->ShowWindow(!bPrvGeneral);
	GetDlgItem(IDC_CMD_CALC_SECT_PROP_BTN)->ShowWindow(bPrvGeneral);

	GetDlgItem(IDC_CMD_HUMBLY_CHK)->ShowWindow(bHumbly);
	if(!bHumbly) 
	{
		m_Data.SectBefore.bHumblyChk = FALSE;
		m_chkHumbly.SetCheck(0);
		m_chkHumblyBefore.SetCheck(0);
		m_chkHumblyAfter.SetCheck(0);
	}
	else
	{
		if ((m_nCurType == 8) || (m_nCurType == 11)) // Tub Section 에서만 Before/After Humbly 옵션 사용
		{
			GetDlgItem(IDC_CMD_HUMBLY_CHK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_HUMBLY_CHK_GRBOX)->ShowWindow(bHumbly);
			GetDlgItem(IDC_CMD_HUMBLY_CHK_BF)->ShowWindow(bHumbly);
			GetDlgItem(IDC_CMD_HUMBLY_CHK_BF)->EnableWindow(bHumbly);
			GetDlgItem(IDC_CMD_HUMBLY_CHK_AF)->ShowWindow(bHumbly);
			GetDlgItem(IDC_CMD_HUMBLY_CHK_AF)->EnableWindow(bHumbly);
		}
		else
		{
			GetDlgItem(IDC_CMD_HUMBLY_CHK)->ShowWindow(bHumbly);
			GetDlgItem(IDC_CMD_HUMBLY_CHK_GRBOX)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_HUMBLY_CHK_BF)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_HUMBLY_CHK_BF)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_HUMBLY_CHK_AF)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_HUMBLY_CHK_AF)->EnableWindow(FALSE);
		}
	}

	ChangeCtrlSize(bBigMatlGrp); // Type이 바뀔때 컨트럴 크기가 동적으로 변경될 필요가 있으면 파라메터에 Bool형 변수를 늘려서 bBigMatlGrp처럼 처리 하시오.

	if(bPSC) 
	{
		CreatePSCView();
		ChangeBitmap(m_nCurType);
	}
	else DestroyPSCView();

	EnableDisable7DOTbyStiffener();
	EnableDisableStlGirder();

	// Slab width 입력 사용안함.
	if(CSectDB::IsEnable7thDOF())
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrSlabInfo,  FALSE);
	}
	//////////////////////////////////////////////////////////////////////////
	//China Gen  modify by maxiao 2013-7-5
#if defined(_MGEN) && defined(_CH)
	GetDlgItem(IDC_CMD_SP_ID_SWIDTH)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SP_ID_GNUM)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC4)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC5)->ShowWindow(FALSE);
	//GetDlgItem(IDC_CMD_SP_ID_GNUM_SPIN)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SP_ID_UNIT1)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SP_ID_UNIT2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC8)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SP_ID_HH)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SP_ID_UNIT5)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SP_ID_CTC)->ShowWindow(FALSE);	
	UpdateData(FALSE);
#endif
#ifdef _MGEN_CH
	CDlgUtil::CtrlShowHide(this, m_aCtrlAType, m_nCurType == 7);
	m_cbxAType.SetCurSel(m_Data.nSectAType);
#else
	CDlgUtil::CtrlShowHide(this, m_aCtrlAType, FALSE);
#endif
	//////////////////////////////////////////////////////////////////////////
		if (m_nCurType == 4 || m_nCurType == 3 || m_nCurType == 6) {  // Added by Apoorva
			GetDlgItem(IDC_STATIC28)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_SP_ID_BC3)->ShowWindow(SW_SHOW);
		}
		else {
			GetDlgItem(IDC_STATIC28)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_SP_ID_BC3)->ShowWindow(SW_HIDE);
		}

		CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_BC);
		if (pWnd && CTestEnvMgr::GetTestEnvST(_LSX(Test_PSCComp)) == _LSX(yes))
		{
			CRect rc;
			pWnd->GetWindowRect(&rc);
			ScreenToClient(&rc);

			if (m_nCurType != 4 && m_nCurType != 3 && m_nCurType != 6)
			{
				int newWidth = 182;
				rc.right = rc.left + newWidth;   // Correct
			}
			else {
				int newWidth = 82;
				rc.right = rc.left + newWidth;
			}

			pWnd->MoveWindow(rc);
		}
		if (CTestEnvMgr::GetTestEnvST(_LSX(Test_PSCComp)) != _LSX(yes)) {
			m_Sg.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC28)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_SP_ID_BC3)->ShowWindow(SW_HIDE);
		}
}

void CCMSectItemCon::ChangeCtrlSize(BOOL bBigMatlGrp) 
{
	if(bBigMatlGrp)
	{
		GetDlgItem(IDC_BOX_GROUP3)->SetWindowPos(NULL, m_rectMatlGrpSize[1].left, m_rectMatlGrpSize[1].top, m_rectMatlGrpSize[1].Width(), m_rectMatlGrpSize[1].Height(), SWP_NOZORDER);
	}
	else
	{
		GetDlgItem(IDC_BOX_GROUP3)->SetWindowPos(NULL, m_rectMatlGrpSize[0].left, m_rectMatlGrpSize[0].top, m_rectMatlGrpSize[0].Width(), m_rectMatlGrpSize[0].Height(), SWP_NOZORDER);
	}

}

void CCMSectItemCon::OnSelchangeCmdSpSectypeCombo() 
{
	ChangeSectType();	
}

void CCMSectItemCon::OnSelchangeCmdPartSel() 
{
	int nPartSize = m_DataGeneral.SectBefore.SectI.aGeneralPart.GetSize();
	int nIndex = m_cmbPart.GetCurSel();

	if(!m_bReCalc) m_wndGridGeneral.SaveData();

	if(nIndex==0) // Total
	{
		//if(nPartSize==0)
		{
			m_wndGridGeneral.SetDataSource(&m_DataGeneral.SectAfter.SectI, &m_PSCDesign, D_SECT_TYPE_COMPO_G, &m_DataGeneral.SectAfter.nCellShape);
		}
		//     else
		//     {
		//       int nPart = nPartSize-1;
		//       m_wndGridGeneral.SetDataSource2(&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].StiffnessSum,
		//         &m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].PeriOutSum,
		//         &m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].PeriInSum,
		//         &m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].YBarSum,
		//         &m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].ZBarSum,
		//         &m_DataGeneral.SectBefore.SectI, D_SECT_TYPE_COMPO_G, &m_DataGeneral.SectBefore.nCellShape);
		//     }
	}
	else if(nIndex < nPartSize+1) // Part 1, Part 2...
	{
		int nPart = nIndex-1;
		m_wndGridGeneral.SetDataSource2(&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].Stiffness,
			&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].PeriOut,
			&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].PeriIn,
			&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].YBar,
			&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].ZBar,
			&m_DataGeneral.SectBefore.SectI, D_SECT_TYPE_COMPO_G, &m_DataGeneral.SectBefore.nCellShape);
	}
	else if(nIndex < (2*nPartSize-1) ) // Part 1+2, Part 1+2+3...
	{
		int nPart = nIndex-nPartSize;
		m_wndGridGeneral.SetDataSource2(&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].StiffnessSum,
			&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].PeriOutSum,
			&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].PeriInSum,
			&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].YBarSum,
			&m_DataGeneral.SectBefore.SectI.aGeneralPart[nPart].ZBarSum,
			&m_DataGeneral.SectBefore.SectI, D_SECT_TYPE_COMPO_G, &m_DataGeneral.SectBefore.nCellShape);
	}
	else
		ASSERT(0);

	m_wndGridGeneral.SetWarpingEffect(m_DataGeneral.SectBefore.bConsiderWarpingEffect);
#ifdef _MGEN_CH
	if (m_DataGeneral.nStype == D_SECT_TYPE_COMPO_G && m_DataGeneral.SectBefore.Shape == D_SECT_SHAPE_COMPO_G)
	{
		m_DataGeneral.nStype = D_SECT_TYPE_PSC;
		m_DataGeneral.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;
		BOOL bConsiderWarpingEffect = m_DataGeneral.SectBefore.bConsiderWarpingEffect;
		m_DataGeneral.SectBefore.bConsiderWarpingEffect = TRUE;
		double dTempSize[4];
		for (int i = 0; i < 4; i++)
		{
			dTempSize[i] = m_DataGeneral.SectBefore.SectI.Size[i];
			m_DataGeneral.SectBefore.SectI.Size[i] = 1.0;
		}
		CSectUtil::CalcSectData(m_DataGeneral, TRUE, FALSE, TRUE, TRUE);
		m_DataGeneral.nStype = D_SECT_TYPE_COMPO_G;
		m_DataGeneral.SectBefore.Shape = D_SECT_SHAPE_COMPO_G;
		for (int i = 0; i < 4; i++)
		{
			m_DataGeneral.SectBefore.SectI.Size[i] = dTempSize[i];
		}
		m_DataGeneral.SectBefore.bConsiderWarpingEffect = bConsiderWarpingEffect;
	}
#endif
	m_wndGridGeneral.ShowData();

	m_bReCalc = FALSE;
}

void CCMSectItemCon::OnChangeCmdSpIdB1() 
{  
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectI.Size[2] = m_B1.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectBefore.SectI.Size[2] = m_B1.GetEditValue();
		//m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectI.Size[2] = m_B1.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdB2() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectI.Size[5] = m_B2.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectBefore.SectI.Size[4] = m_B2.GetEditValue();
		//m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectI.Size[5] = m_B2.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdBc() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
		//m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 3)
	{
		m_DataCI.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
		m_wndSecViewCI.SetCentroidFlag(FALSE);
		m_wndSecViewCI.Invalidate();
	}
	else if(m_nCurType == 4)
	{
		m_DataCT.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
		m_wndSecViewCT.SetCentroidFlag(FALSE);
		m_wndSecViewCT.Invalidate();
	}
	else if(m_nCurType == 6)
	{
		m_DataPC.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
		m_wndSecViewPC.SetCentroidFlag(FALSE);
		m_wndSecViewPC.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
	else if(m_nCurType == 9)
	{
		m_DataTub.SectAfter.SectJ.Size[0] = m_StlGirderBc.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataTub.SectAfter.SectJ.Size[0] = m_StlGirderBc.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataTub.SectAfter.SectJ.Size[0] = m_StlGirderBc.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}

	//China Gen  modify by maxiao 2013-7-5
#if defined(_MGEN) && defined(_CH)
	CString s;
	m_Bc.GetWindowText(s);
	m_swidth.SetWindowText(s);
	OnChangeCmdSpIdSwidth();
#endif
}

void CCMSectItemCon::OnChangeCmdSpIdBf1() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectI.Size[3] = m_Bf1.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectI.Size[3] = m_Bf1.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdBf2() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectI.Size[6] = m_Bf2.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectI.Size[6] = m_Bf2.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdBf3() 
{
	if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectI.Size[8] = m_Bf3.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdtfp() 
{
	if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectI.Size[9] = m_tfp.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdHh() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();
		//m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 3)
	{
		m_DataCI.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();
		m_wndSecViewCI.SetCentroidFlag(FALSE);
		m_wndSecViewCI.Invalidate();
	}
	else if(m_nCurType == 4)
	{
		m_DataCT.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();
		m_wndSecViewCT.SetCentroidFlag(FALSE);
		m_wndSecViewCT.Invalidate();
	}
	else if(m_nCurType == 6)
	{
		m_DataPC.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();
		m_wndSecViewPC.SetCentroidFlag(FALSE);
		m_wndSecViewPC.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectAfter.SectJ.Size[2] = m_Hh.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
	else if(m_nCurType == 9)
	{
		m_DataTub.SectAfter.SectJ.Size[2] = m_StlGirderHh.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataTub.SectAfter.SectJ.Size[2] = m_StlGirderHh.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataTub.SectAfter.SectJ.Size[2] = m_StlGirderHh.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdHr1() 
{ 
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectJ.Size[2] = m_Hr1.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectJ.Size[2] = m_Hr1.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdHr2() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectJ.Size[3] = m_Hr2.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectJ.Size[3] = m_Hr2.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdHw() 
{  
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectI.Size[0] = m_Hw.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectBefore.SectI.Size[0] = m_Hw.GetEditValue();
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectI.Size[0] = m_Hw.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdTc() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
		//m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 3)
	{
		m_DataCI.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
		m_wndSecViewCI.SetCentroidFlag(FALSE);
		m_wndSecViewCI.Invalidate();
	}
	else if(m_nCurType == 4)
	{
		m_DataCT.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
		m_wndSecViewCT.SetCentroidFlag(FALSE);
		m_wndSecViewCT.Invalidate();
	}
	else if(m_nCurType == 6)
	{
		m_DataPC.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
		m_wndSecViewPC.SetCentroidFlag(FALSE);
		m_wndSecViewPC.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectAfter.SectJ.Size[1] = m_Tc.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
	else if(m_nCurType == 9)
	{
		m_DataTub.SectAfter.SectJ.Size[1] = m_StlGirderTc.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataTub.SectAfter.SectJ.Size[1] = m_StlGirderTc.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataTub.SectAfter.SectJ.Size[1] = m_StlGirderTc.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdTf1() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectI.Size[4] = m_Tf1.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectBefore.SectI.Size[3] = m_Tf1.GetEditValue();
		//m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectI.Size[4] = m_Tf1.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdTf2() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectI.Size[7] = m_Tf2.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectBefore.SectI.Size[5] = m_Tf2.GetEditValue();
		//m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectI.Size[7] = m_Tf2.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdTr1() 
{ 
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectJ.Size[4] = m_Tr1.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectJ.Size[4] = m_Tr1.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdTr2() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectJ.Size[5] = m_Tr2.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectJ.Size[5] = m_Tr2.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdTw() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.SectI.Size[1] = m_tw.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectBefore.SectI.Size[1] = m_tw.GetEditValue();
		//m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.SectI.Size[1] = m_tw.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdN1() 
{
	if (m_wndSecViewBox.GetSafeHwnd() == NULL || !IsWindow(m_wndSecViewBox.GetSafeHwnd())) return;
	UpdateData();
	m_DataBox.SectBefore.nStiffNum[0] = m_N1.GetEditValueInt();
	//m_wndSecViewBox.SetCentroidFlag(FALSE);
	m_wndSecViewBox.Invalidate();
}

void CCMSectItemCon::OnChangeCmdSpIdN2() 
{
	if (m_wndSecViewBox.GetSafeHwnd() == NULL || !IsWindow(m_wndSecViewBox.GetSafeHwnd())) return;
	UpdateData();
	m_DataBox.SectBefore.nStiffNum[1] = m_N2.GetEditValueInt();
	//m_wndSecViewBox.SetCentroidFlag(FALSE);
	m_wndSecViewBox.Invalidate();
}

void CCMSectItemCon::OnChangeCmdSpIdSwidth() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
		//m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
		//m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 3)
	{
		m_DataCI.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
		m_wndSecViewCI.SetCentroidFlag(FALSE);
		m_wndSecViewCI.Invalidate();
	}
	else if(m_nCurType == 4)
	{
		m_DataCT.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
		m_wndSecViewCT.SetCentroidFlag(FALSE);
		m_wndSecViewCT.Invalidate();
	}
	else if(m_nCurType == 6)
	{
		m_DataPC.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
		m_wndSecViewPC.SetCentroidFlag(FALSE);
		m_wndSecViewPC.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectAfter.SectI.Size[0] = m_swidth.GetEditValue();
		//m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdEsec()
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.Matl_Elast  = m_ESEC.GetEditValue();
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectBefore.Matl_Elast  = m_ESEC.GetEditValue();
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 3)
	{
		m_DataCI.SectBefore.Matl_Elast  = m_ESEC.GetEditValue();
		m_wndSecViewCI.Invalidate();
	}
	else if(m_nCurType == 4)
	{
		m_DataCT.SectBefore.Matl_Elast  = m_ESEC.GetEditValue();
		m_wndSecViewCT.Invalidate();
	}
	else if(m_nCurType == 6)
	{
		m_DataPC.SectBefore.Matl_Elast  = m_ESEC.GetEditValue();
		m_wndSecViewPC.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.Matl_Elast  = m_ESEC.GetEditValue();
		m_wndSecViewTub.Invalidate();
	}
	else if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.Matl_Elast = m_ESEC.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.Matl_Elast = m_ESEC.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.Matl_Elast = m_ESEC.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdDsdc()
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.Matl_Density  = m_DSDC.GetEditValue();
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectBefore.Matl_Density  = m_DSDC.GetEditValue();
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 3)
	{
		m_DataCI.SectBefore.Matl_Density  = m_DSDC.GetEditValue();
		m_wndSecViewCI.SetCentroidFlag(FALSE);
		m_wndSecViewCI.Invalidate();
	}
	else if(m_nCurType == 4)
	{
		m_DataCT.SectBefore.Matl_Density  = m_DSDC.GetEditValue();
		m_wndSecViewCT.SetCentroidFlag(FALSE);
		m_wndSecViewCT.Invalidate();
	}
	else if(m_nCurType == 6)
	{
		m_DataPC.SectBefore.Matl_Density  = m_DSDC.GetEditValue();
		m_wndSecViewPC.SetCentroidFlag(FALSE);
		m_wndSecViewPC.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.Matl_Density  = m_DSDC.GetEditValue();
		m_wndSecViewTub.Invalidate();
	}
	else if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.Matl_Density = m_DSDC.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.Matl_Density = m_DSDC.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.Matl_Density = m_DSDC.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdPs()
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.Matl_Poisson_S  = m_PS.GetEditValue();
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectBefore.Matl_Poisson_S  = m_PS.GetEditValue();
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 3)
	{
		m_DataCI.SectBefore.Matl_Poisson_S  = m_PS.GetEditValue();
		m_wndSecViewCI.Invalidate();
	}
	else if(m_nCurType == 4)
	{
		m_DataCT.SectBefore.Matl_Poisson_S  = m_PS.GetEditValue();
		m_wndSecViewCT.Invalidate();
	}
	else if(m_nCurType == 6)
	{
		m_DataPC.SectBefore.Matl_Poisson_S  = m_PS.GetEditValue();
		m_wndSecViewPC.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.Matl_Poisson_S  = m_PS.GetEditValue();
		m_wndSecViewTub.Invalidate();
	}
	else if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.Matl_Poisson_S = m_PS.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdPc()
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.Matl_Poisson_C  = m_PC.GetEditValue();
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectBefore.Matl_Poisson_C  = m_PC.GetEditValue();
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 3)
	{
		m_DataCI.SectBefore.Matl_Poisson_C  = m_PC.GetEditValue();
		m_wndSecViewCI.Invalidate();
	}
	else if(m_nCurType == 4)
	{
		m_DataCT.SectBefore.Matl_Poisson_C  = m_PC.GetEditValue();
		m_wndSecViewCT.Invalidate();
	}
	else if(m_nCurType == 6)
	{
		m_DataPC.SectBefore.Matl_Poisson_C  = m_PC.GetEditValue();
		m_wndSecViewPC.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.Matl_Poisson_C  = m_PC.GetEditValue();
		m_wndSecViewTub.Invalidate();
	}
	else if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.Matl_Poisson_C = m_PC.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdTstc()
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectBefore.Matl_Thermal  = m_TSTC.GetEditValue();
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectBefore.Matl_Thermal  = m_TSTC.GetEditValue();
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 3)
	{
		m_DataCI.SectBefore.Matl_Thermal  = m_TSTC.GetEditValue();
		m_wndSecViewCI.Invalidate();
	}
	else if(m_nCurType == 4)
	{
		m_DataCT.SectBefore.Matl_Thermal  = m_TSTC.GetEditValue();
		m_wndSecViewCT.Invalidate();
	}
	else if(m_nCurType == 6)
	{
		m_DataPC.SectBefore.Matl_Thermal  = m_TSTC.GetEditValue();
		m_wndSecViewPC.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectBefore.Matl_Thermal  = m_TSTC.GetEditValue();
		m_wndSecViewTub.Invalidate();
	}
	else if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.Matl_Thermal = m_TSTC.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.Matl_Thermal = m_TSTC.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.Matl_Thermal = m_TSTC.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdCtc() 
{
	if(m_nCurType == 1)
	{
		m_DataBox.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
		m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
		m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 3)
	{
		m_DataCI.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
		m_wndSecViewCI.SetCentroidFlag(FALSE);
		m_wndSecViewCI.Invalidate();
	}
	else if(m_nCurType == 4)
	{
		m_DataCT.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
		m_wndSecViewCT.SetCentroidFlag(FALSE);
		m_wndSecViewCT.Invalidate();
	}
	else if(m_nCurType == 6)
	{
		m_DataPC.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
		m_wndSecViewPC.SetCentroidFlag(FALSE);
		m_wndSecViewPC.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectAfter.SectI.Size[1] = m_CTC.GetEditValue();
		m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeCmdSpIdGnum() 
{
	if (m_wndSecViewBox.GetSafeHwnd() == NULL || !IsWindow(m_wndSecViewBox.GetSafeHwnd())) return;
	UpdateData(TRUE);

	if(m_nCurType == 1)
	{
		m_DataBox.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();
		m_wndSecViewBox.SetCentroidFlag(FALSE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_nCurType == 2)
	{
		m_DataI.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();
		m_wndSecViewI.SetCentroidFlag(FALSE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_nCurType == 3)
	{
		m_DataCI.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();
		m_wndSecViewCI.SetCentroidFlag(FALSE);
		m_wndSecViewCI.Invalidate();
	}
	else if(m_nCurType == 4)
	{
		m_DataCT.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();
		m_wndSecViewCT.SetCentroidFlag(FALSE);
		m_wndSecViewCT.Invalidate();
	}
	else if(m_nCurType == 6)
	{
		m_DataPC.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();
		m_wndSecViewPC.SetCentroidFlag(FALSE);
		m_wndSecViewPC.Invalidate();
	}
	else if(m_nCurType == 8)
	{
		m_DataTub.SectAfter.SectI.BuiltUpFlag = m_gnum.GetEditValueInt();
		m_wndSecViewTub.SetCentroidFlag(FALSE);
		m_wndSecViewTub.Invalidate();
	}
}

void CCMSectItemCon::OnMaterialBtn() 
{
	// TODO: Add your control notification handler code here
	CRect rect;
	GetDlgItem(IDC_MATERIAL_BTN)->GetWindowRect(rect);
	//rect.top += 20;
	//rect.left = rect.right;

	CStringArray aData;
	UINT aID[] = {IDC_CMD_SP_ID_ESEC, IDC_CMD_SP_ID_DSDC, IDC_CMD_SP_ID_PS, IDC_CMD_SP_ID_PC, IDC_CMD_SP_ID_TSTC};

	// save current data
	CString data;
	for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++)
	{
		GetDlgItem(aID[i])->GetWindowText(data);
		aData.Add(data);
	}

	if(m_nCurType == 1 || m_nCurType == 2  || m_nCurType == 8 ||
		m_nCurType == 9 || m_nCurType == 10 || m_nCurType == 11)
	{
		CCMConcrSteelMatlDlg dlg(m_pDoc, rect, IDD_CMD_SECT_ITEM_CON, this);
		if (dlg.DoModal() != IDOK)
		{
			// restore old data
			for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++)
				GetDlgItem(aID[i])->SetWindowText(aData[i]);
		}
	}
	else if(m_nCurType == 3 || m_nCurType == 4 || m_nCurType == 6 || m_nCurType == 7)
	{
		CCMCompoMatDlg dlg(m_pDoc, rect, IDD_CMD_SECT_ITEM_CON, this);
		if (dlg.DoModal() != IDOK)
		{
			// restore old data
			for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++)
				GetDlgItem(aID[i])->SetWindowText(aData[i]);
		}
	}
	else ASSERT(FALSE);
}

void CCMSectItemCon::OnChangeCmdSpIdSg()
{
	if (m_nCurType == 3)
	{
		/*m_DataCI.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();*/
		m_DataCI.SectAfter.SectJ.Size[3] = m_Sg.GetEditValue();
		m_wndSecViewCI.SetCentroidFlag(FALSE);
		m_wndSecViewCI.Invalidate();
	}
	else if (m_nCurType == 4)
	{
		/*m_DataCT.SectAfter.SectJ.Size[0] = m_Bc.GetEditValue();*/
		m_DataCT.SectAfter.SectJ.Size[3] = m_Sg.GetEditValue();
		m_wndSecViewCT.SetCentroidFlag(FALSE);
		m_wndSecViewCT.Invalidate();
	}
	else if (m_nCurType == 6)
	{
		m_DataPC.SectAfter.SectJ.Size[3] = m_Sg.GetEditValue();
		m_wndSecViewPC.SetCentroidFlag(FALSE);
		m_wndSecViewPC.Invalidate();
	}
	else {
		return;
	}
}

void CCMSectItemCon::ChangeMatlData(T_MATL_CONCRETE &ConcrData, T_MATL_STEEL &SteelData)
{
	if(m_nCurType == 3 || m_nCurType == 4) return;

	T_SECT_SECTION_D* pData=NULL;
	if(m_nCurType == 1)  pData = &(m_DataBox.SectBefore);
	else if(m_nCurType == 2) pData =  &(m_DataI.SectBefore);
	else if(m_nCurType == 5) pData =  &(m_DataUser.SectBefore);
	else if(m_nCurType == 8) pData =  &(m_DataTub.SectBefore);                                    
	else if(m_nCurType == 9) pData =  &(m_DataStlGirderBox.SectBefore);
	else if(m_nCurType == 10) pData = &(m_DataStlGirderI.SectBefore);
	else if(m_nCurType == 11) pData = &(m_DataStlGirderTub.SectBefore);
	else {ASSERT(0); return;}

	pData->Matl_Elast = SteelData.Elast/ConcrData.Elast;
	pData->Matl_Density = SteelData.Density/ConcrData.Density;
	pData->Matl_Poisson_S = SteelData.Poisson;
	pData->Matl_Poisson_C = ConcrData.Poisson;
	pData->Matl_Thermal = SteelData.Thermal/ConcrData.Thermal;

	CString str;
	m_ESEC.SetEditUnit(pData->Matl_Elast);      //str.Format(_T("%g"), pData->Matl_Elast); m_ESEC.SetWindowText(str);
	m_DSDC.SetEditUnit(pData->Matl_Density);   //str.Format(_T("%g"), pData->Matl_Density); m_DSDC.SetWindowText(str);
	m_PS  .SetEditUnit(pData->Matl_Poisson_S); //str.Format(_T("%g"), pData->Matl_Poisson_S); m_PS.SetWindowText(str);
	m_PC  .SetEditUnit(pData->Matl_Poisson_C); //str.Format(_T("%g"), pData->Matl_Poisson_C); m_PC.SetWindowText(str);
	m_TSTC.SetEditUnit(pData->Matl_Thermal);   //str.Format(_T("%g"), pData->Matl_Thermal); m_TSTC.SetWindowText(str);
	UpdateData(FALSE);
}

void CCMSectItemCon::ChangeMatlDataCICTPC(T_MATL_CONCRETE &ConcrDataSlab, T_MATL_CONCRETE &ConcrDataGirder)
{
	if(m_nCurType == 1 || m_nCurType == 2 || m_nCurType == 5 || m_nCurType == 8) return;

	T_SECT_SECTION_D* pData = NULL;
	if     (m_nCurType == 3) pData = &(m_DataCI.SectBefore);
	else if(m_nCurType == 4) pData = &(m_DataCT.SectBefore);
	else if(m_nCurType == 6) pData = &(m_DataPC.SectBefore);
	else {ASSERT(0); return;}

	// Steel->Girder, Concrete->Slab 로 간주하여 계산
	pData->Matl_Elast     = ConcrDataGirder.Elast/ConcrDataSlab.Elast;
	pData->Matl_Density   = ConcrDataGirder.Density/ConcrDataSlab.Density;
	pData->Matl_Poisson_S = ConcrDataGirder.Poisson; // MNET:XXXX 20070307 mylee 다시 살림
	pData->Matl_Poisson_C = ConcrDataSlab.Poisson;
	pData->Matl_Thermal   = ConcrDataGirder.Thermal/ConcrDataSlab.Thermal;

	CString str;
	m_ESEC.SetEditUnit(pData->Matl_Elast);    	//str.Format(_T("%g"), pData->Matl_Elast);     m_ESEC.SetWindowText(str);
	m_DSDC.SetEditUnit(pData->Matl_Density); 	//str.Format(_T("%g"), pData->Matl_Density);   m_DSDC.SetWindowText(str);
	m_PS  .SetEditUnit(pData->Matl_Poisson_S); 	//str.Format(_T("%g"), pData->Matl_Poisson_S); m_PS.SetWindowText(str);
	m_PC  .SetEditUnit(pData->Matl_Poisson_C); 	//str.Format(_T("%g"), pData->Matl_Poisson_C); m_PC.SetWindowText(str);
	m_TSTC.SetEditUnit(pData->Matl_Thermal); 	//str.Format(_T("%g"), pData->Matl_Thermal); m_TSTC.SetWindowText(str);
	UpdateData(FALSE);
}

///////////////////////////////////////////////////////////////////////////////////
//
// Viewer - Composite-I, Composite-T형에서 띄운다.

void CCMSectItemCon::CreatePSCView()
{
	// Window가 이미 있으면 만들지 않는다.
	if(IsViewWindow()) return;

	m_pViewer = new CCMSectItmePSCView();
	m_pViewer->SetInitPos(D_INIT_POS_RT);
	m_pViewer->Create(IDD_CMD_SECT_ITEM_PSC_VIEW, this);
	m_pViewer->ShowWindow(SW_SHOW);

	// View를 숨기고 다이얼로그 크기 조절 
	CWnd* pWnd;
	pWnd = m_pViewer->GetDlgItem(IDC_CMD_PSC_VIEW_STATIC); ASSERT(pWnd);
	pWnd->ShowWindow(SW_HIDE);
	pWnd = m_pViewer->GetDlgItem(IDC_CMD_VIEW_FRM); ASSERT(pWnd);
	pWnd->ShowWindow(SW_HIDE);

	CRect rectLast, r;
	pWnd = m_pViewer->GetDlgItem(IDC_CMD_PICTURE); ASSERT(pWnd);
	pWnd->GetWindowRect(rectLast);
	m_pViewer->GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);
	m_pViewer->MoveWindow(r);
}

void CCMSectItemCon::DestroyPSCView()
{
	// Window가 있으면 없앤다.
	if(m_pViewer == NULL) return;
	if(!IsViewWindow()) return;

	m_pViewer->DestroyWindow();
	m_pViewer = NULL;     
}

BOOL CCMSectItemCon::IsViewWindow()
{ 
	if(m_pViewer == NULL) return FALSE;
	HWND hWnd = m_pViewer->GetSafeHwnd();
	if(!hWnd) return FALSE;
	if(!(::IsWindow(hWnd))) return FALSE;

	return TRUE;
}

BOOL CCMSectItemCon::ChangeBitmap(int nType)
{
	if(!IsViewWindow()) return FALSE;

	int nIndex;
	if     (nType == 3) nIndex = __SECT_PSC_I__;
	else if(nType == 4) nIndex = __SECT_PSC_TEE__;
	else if(nType == 6) nIndex = __SECT_PSC_VALUE__;
	else ASSERT(0);

	return m_pViewer->ChangeBitmap(nIndex);  
}

BOOL CCMSectItemCon::DestroyWindow() 
{
	DestroyPSCView();

	return CDialog::DestroyWindow();
}

void CCMSectItemCon::OnCmdSectCentroidBtn() 
{
	// TODO: Add your control notification handler code here
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_Impossible_Display_Centroid));
		return;
	}

	if(m_Data.nStype == D_SECT_TYPE_CONSTRUCTION)
	{
		//m_wndSecViewFirst.SetCentroidFlag(TRUE);
		m_wndSecViewFirst.Invalidate();
		//m_wndSecViewSecond.SetCentroidFlag(TRUE);
		m_wndSecViewSecond.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_B)
	{
		m_DataBox = m_Data;
		//m_wndSecViewBox.SetCentroidFlag(TRUE);
		m_wndSecViewBox.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_I)
	{
		m_DataI = m_Data;
		//m_wndSecViewI.SetCentroidFlag(TRUE);
		m_wndSecViewI.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_CI)
	{
		m_DataCI = m_Data;
		m_wndSecViewCI.SetCentroidFlag(TRUE);
		m_wndSecViewCI.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_CT)
	{
		m_DataCT = m_Data;
		m_wndSecViewCT.SetCentroidFlag(TRUE);
		m_wndSecViewCT.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_PC)
	{
		m_DataPC = m_Data;
		m_wndSecViewPC.SetCentroidFlag(TRUE);
		m_wndSecViewPC.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_G)
	{
		m_DataGeneral = m_Data;
		//m_wndSecViewGeneral.SetCentroidFlag(TRUE);
		m_wndSecViewGeneral.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_TUB)
	{
		m_DataTub = m_Data;
		//m_wndSecViewTub.SetCentroidFlag(TRUE);
		m_wndSecViewTub.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_B)
	{
		m_DataStlGirderBox = m_Data;
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_I)
	{
		m_DataStlGirderI = m_Data;
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		m_DataStlGirderTub = m_Data;
		m_wndSecViewStlGirderTub.Invalidate();
	}
	else ASSERT(0);
}

void CCMSectItemCon::SetOffsetData(T_SECT_SECTION_D* pData1, T_SECT_SECTION_D* pData2)
{
	pData2->nOffsetPoint = pData1->nOffsetPoint;
	pData2->nOffsetCenter = pData1->nOffsetCenter;
	pData2->nVertOffsetOpt = pData1->nVertOffsetOpt;
	pData2->nHorzOffsetOpt = pData1->nHorzOffsetOpt;
	pData2->nUserOffsetRef = pData1->nUserOffsetRef;
	pData2->dUserDefOffsetY[0] = pData1->dUserDefOffsetY[0];
	pData2->dUserDefOffsetY[1] = pData1->dUserDefOffsetY[1];
	pData2->dUserDefOffsetZ[0] = pData1->dUserDefOffsetZ[0];
	pData2->dUserDefOffsetZ[1] = pData1->dUserDefOffsetZ[1];
}

void CCMSectItemCon::OnCmdChangeOffsetBtn() 
{
	// TODO: Add your control notification handler code here
	T_SECT_SECTION_D Old_Box, Old_I, Old_CI, Old_CT, Old_PC, Old_User, Old_General, Old_Tub, Old_StlGirderBox, Old_StlGirderI, Old_StlGirderTub;
	SetOffsetData(&m_DataBox.SectBefore,          &Old_Box);
	SetOffsetData(&m_DataI.SectBefore,            &Old_I);
	SetOffsetData(&m_DataCI.SectBefore,           &Old_CI);
	SetOffsetData(&m_DataCT.SectBefore,           &Old_CT);
	SetOffsetData(&m_DataPC.SectBefore,           &Old_PC);
	SetOffsetData(&m_DataGeneral.SectBefore,      &Old_General);
	SetOffsetData(&m_DataUser.SectBefore,         &Old_User);
	SetOffsetData(&m_DataTub.SectBefore,          &Old_Tub);
	SetOffsetData(&m_DataStlGirderBox.SectBefore, &Old_StlGirderBox);
	SetOffsetData(&m_DataStlGirderI.SectBefore,   &Old_StlGirderI);
	SetOffsetData(&m_DataStlGirderTub.SectBefore, &Old_StlGirderTub);

	// 일단 current data만 offset 정보를 얻어온 다음,
	// 나머지 data에도 setting
	switch(m_nCurType)
	{
	case 1: SetOffsetData(&m_DataBox.SectBefore,           &m_Data.SectBefore); break;
	case 2: SetOffsetData(&m_DataI.SectBefore,             &m_Data.SectBefore); break;
	case 3: SetOffsetData(&m_DataCI.SectBefore,            &m_Data.SectBefore); break;
	case 4: SetOffsetData(&m_DataCT.SectBefore,            &m_Data.SectBefore); break;
	case 5: SetOffsetData(&m_DataUser.SectBefore,          &m_Data.SectBefore); break;
	case 6: SetOffsetData(&m_DataPC.SectBefore,            &m_Data.SectBefore); break;
	case 7: SetOffsetData(&m_DataGeneral.SectBefore,       &m_Data.SectBefore); break;
	case 8: SetOffsetData(&m_DataTub.SectBefore,           &m_Data.SectBefore); break;
	case 9: SetOffsetData(&m_DataStlGirderBox.SectBefore,  &m_Data.SectBefore); break;
	case 10: SetOffsetData(&m_DataStlGirderI.SectBefore,   &m_Data.SectBefore); break;
	case 11: SetOffsetData(&m_DataStlGirderTub.SectBefore, &m_Data.SectBefore); break;
	default: ASSERT(0); break;
	}

	CCMSectOffsetDlg dlg(&m_Data.SectBefore, FALSE, this);
	if (dlg.DoModal() == IDOK)
	{
		SetOffsetData(&m_Data.SectBefore, &m_DataBox.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataI.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataCI.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataCT.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataUser.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataPC.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataGeneral.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataTub.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataStlGirderBox.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataStlGirderI.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataStlGirderTub.SectBefore);

		if (m_Data.SectBefore.nOffsetPoint >= 1)
		{
			CString csOffset = m_aOffsetName[m_Data.SectBefore.nOffsetPoint-1];
			m_txtOffset.SetWindowText(csOffset);
		}
	}	
	else
	{
		SetOffsetData(&Old_Box,          &m_DataBox.SectBefore);
		SetOffsetData(&Old_I,            &m_DataI.SectBefore);
		SetOffsetData(&Old_CI,           &m_DataCI.SectBefore);
		SetOffsetData(&Old_CT,           &m_DataCT.SectBefore);
		SetOffsetData(&Old_User,         &m_DataUser.SectBefore);
		SetOffsetData(&Old_PC,           &m_DataPC.SectBefore);
		SetOffsetData(&Old_General,      &m_DataGeneral.SectBefore);
		SetOffsetData(&Old_Tub,          &m_DataTub.SectBefore);
		SetOffsetData(&Old_StlGirderBox, &m_DataStlGirderBox.SectBefore);
		SetOffsetData(&Old_StlGirderI,   &m_DataStlGirderI.SectBefore);
		SetOffsetData(&Old_StlGirderTub, &m_DataStlGirderTub.SectBefore);
	}

	m_wndSecViewFirst.Invalidate();	
	m_wndSecViewSecond.Invalidate();	
	m_wndSecViewBox.Invalidate();	
	m_wndSecViewI.Invalidate();	
	m_wndSecViewCI.Invalidate();	
	m_wndSecViewCT.Invalidate();	
	m_wndSecViewPC.Invalidate();
	m_wndSecViewGeneral.Invalidate();
	m_wndSecViewTub.Invalidate();	  
	m_wndSecViewStlGirderBox.Invalidate();
	m_wndSecViewStlGirderI.Invalidate();
	m_wndSecViewStlGirderTub.Invalidate();
}

void CCMSectItemCon::OnCmdMultiModulusCheck() 
{
	int nCheck = m_chkMat.GetCheck();
	if (nCheck == 1) // Button Check
	{
		m_edtMat1.EnableWindow(TRUE);
		m_edtMat2.EnableWindow(TRUE);
	}
	else
	{
		m_edtMat1.SetEditUnit(0);
		m_edtMat2.SetEditUnit(0);
		m_edtMat1.EnableWindow(FALSE);
		m_edtMat2.EnableWindow(FALSE);
	}
}

void CCMSectItemCon::OnCmdPSCImportBtn()
{
	CCMSelectSect dlg;

	CRect Wnd, Ctrl;
	GetParent()->GetParent()->GetWindowRect(&Wnd);
	GetDlgItem(IDC_PSC_IMP_BTN)->GetWindowRect(&Ctrl);
	dlg.SetInitPos(D_INIT_POS_LT, Ctrl.left-Wnd.left, Ctrl.bottom-Wnd.top);

	CArray<CString, CString&> aSectShape;
	CString strShape = D_SECT_SHAPE_PSC_VALUE;
	aSectShape.Add(CString(D_SECT_SHAPE_PSC_VALUE));

	dlg.SetPSCShape(aSectShape);
	if(dlg.DoModal()==IDOK)
	{
		m_PscValueSectK = dlg.GetSelectedSectK();
		SavePCData();
		m_wndSecViewPC.SetCentroidFlag(FALSE);
		m_wndSecViewPC.Invalidate();
	}
}

void CCMSectItemCon::OnCmdImportBtn() 
{
	CString strSec;
	if (CProduct::IsRusLocal())
		strSec = _T("Section Files(*.sec)|*.sec|All File(*.*)|*.*||");
	else
		strSec = _T("MIDAS Section Files(*.sec)|*.sec|All File(*.*)|*.*||");

	// TODO: Add your control notification handler code here
	CString csFileName;
	CFileDialog fd(TRUE, _T(".sec"), NULL,
		OFN_FILEMUSTEXIST | OFN_LONGNAMES, 
		strSec, NULL);
	if (fd.DoModal() != IDOK) return;

	CWaitCursor wait;
	CCMSectImportSPC Importer;
	if (!Importer.Import(fd.GetPathName()))
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_CAN_NOT_IMPORT));
		return;
	}

	Importer.GetList(m_aImportID, m_aImportSect);

	T_GRPD_SSM_D GrpdSSMD; GrpdSSMD.Initialize();
	T_STFN_SSM_D StfnSSMD; StfnSSMD.Initialize();
	if(Importer.ConvertSectionManager(m_Data, GrpdSSMD, StfnSSMD))
	{
		m_bModifyGrpd = TRUE;
		m_bModifyStfn = TRUE;
		m_GrpdSSMD = GrpdSSMD;
		m_StfnSSMD = StfnSSMD;
	}

	m_editPath.SetWindowText(fd.GetPathName());
	m_editPath.SetFocus();
	m_editPath.SetSel(0, -1);

	GetDlgItem(IDC_CMD_SEL_BTN)->EnableWindow(TRUE);
	OnCmdSelectBtn();
	
#ifdef _MGEN_CH
	m_Data.nSectAType = GetSectionAType();
	m_cbxAType.SetCurSel(m_Data.nSectAType);
	m_wndGridGeneral.SetWarpingEffect(TRUE);
	m_Data.bInputSec = TRUE;
#else
	if (CSectDB::IsEnable7thDOF())
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		// [경고] SPC에서 import한 단면은 7자유도 단면 속성이 자동 계산되지 않습니다.
		pDoc->DisplayHistoryMessage(_LS(IDS_WG_CMD__ADDD__Warning_Cannot_auto_calc_7thDOF_because_from_SPC));
	}
#endif
}	

void CCMSectItemCon::OnCmdCalcSectPropBtn()    
{
	T_SECT_D SectD = m_DataGeneral;

	//
	BOOL bFirstBase = FALSE;
	for (int nGP = 0; nGP < SectD.SectBefore.SectI.aGeneralPart.GetSize(); nGP++)
	{
		if (!bFirstBase && SectD.SectBefore.SectI.aGeneralPart[nGP].bBaseMaterial) bFirstBase = TRUE;
		else if (bFirstBase) SectD.SectBefore.SectI.aGeneralPart[nGP].bBaseMaterial = FALSE;
	}
	//

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc->m_pSectDB->CalcSectCompo_G_ReCalc(SectD))
	{
		m_DataGeneral = SectD;
		m_bReCalc = TRUE;

		OnSelchangeCmdPartSel();
	}
}

void CCMSectItemCon::OnCmdSelectBtn() 
{
	// TODO: Add your control notification handler code here  
	CCMSectItemPSCImport dlg;
	dlg.SetCompositeMode(); // 20080223 mylee - Composite General 만 보이도록
	dlg.SetData(m_aImportID, m_aImportSect, m_CurSectID);
	if (dlg.DoModal() == IDOK)
	{
		// 주의 메시지 표시
		GSaveHistoryFormatNF(_LS(IDS_CMD_CON_WARNING_2PART));

		dlg.GetData(m_CurSectID, m_DataGeneral);

		// Qy, Qz 계산
		m_DataGeneral.SectBefore.bAutoWebThkShear[0][0] = 
			m_DataGeneral.SectBefore.bAutoWebThkShear[0][1] = 
			m_DataGeneral.SectBefore.bAutoWebThkShear[0][2] = 
			m_DataGeneral.SectBefore.bAutoWebThkShear[1][0] = 
			m_DataGeneral.SectBefore.bAutoWebThkShear[1][1] = 
			m_DataGeneral.SectBefore.bAutoWebThkShear[1][2] = TRUE; 

		double dQy=0., dQz=0.;
		//     if (CSectUtil::GetPscValueDataQ(&m_Data, dQy, dQz)) // later
		//     {
		//       m_Data.SectBefore.SectI.Stiffness.Qyb = dQy;
		//       m_Data.SectBefore.SectI.Stiffness.Qzb = dQz;
		//     }
		m_DataGeneral.SectBefore.bAutoWebThkShear[0][0] = 
			m_DataGeneral.SectBefore.bAutoWebThkShear[0][1] = 
			m_DataGeneral.SectBefore.bAutoWebThkShear[0][2] = 
			m_DataGeneral.SectBefore.bAutoWebThkShear[1][0] = 
			m_DataGeneral.SectBefore.bAutoWebThkShear[1][1] = 
			m_DataGeneral.SectBefore.bAutoWebThkShear[1][2] = FALSE; 

		m_DataGeneral.nStype = D_SECT_TYPE_COMPO_G;
		m_DataGeneral.SectBefore.Shape = D_SECT_SHAPE_COMPO_G;

		if(m_DataGeneral.SectBefore.SectI.aLine.GetSize()>0)
		{
			// Line Type은 강성 계산 (비틀림 값)
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE); // 20080604 mylee
			OnCmdFemRdo();
		}
		else
		{
			//CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, D_DEF_CALC_STIFF_OPT);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE);
			// size and stiffness
			m_wndGridGeneral.SetWarpingEffect(m_DataGeneral.SectBefore.bConsiderWarpingEffect);
			m_wndGridGeneral.ShowData();
			m_wndSecViewGeneral.Invalidate();	
		}
		SetGeneralPartList();
		//OnSelchangeCmdPartSel();

		T_SECT_D SectTmp = m_DataGeneral;
		if(!CSectUtil::IsMeshOK(SectTmp))
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Warning_mesh_NG_ImportFromSPC));
	}
}

void CCMSectItemCon::OnCmdFemRdo() 
{
	int nCalc;
	CDlgUtil::CtrlRadioGetCheck(this,m_aCtrlOption,nCalc);
	m_DataGeneral.SectBefore.nCalcStiffOpt=nCalc;                        
	if (m_DataGeneral.SectBefore.SectI.aLine.GetSize() > 0)
	{
		T_SECT_D SectD = m_DataGeneral;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		/*
		SectD.SectBefore.SectI.Stiffness.Initialize();
		if (pDoc->m_pSectDB->CalcSectData(SectD, TRUE))
		{
		m_Data.SectBefore.SectI.Stiffness.Rxx = SectD.SectBefore.SectI.Stiffness.Rxx;
		m_Data.SectBefore.SectI.Stiffness.Asy = SectD.SectBefore.SectI.Stiffness.Asy;
		m_Data.SectBefore.SectI.Stiffness.Asz = SectD.SectBefore.SectI.Stiffness.Asz;
		m_wndGridGeneral.ShowData();
		m_wndSecViewGeneral.Invalidate();	
		}
		*/
		m_wndGridGeneral.ShowData();
		m_wndSecViewGeneral.Invalidate();	
	}
}

void CCMSectItemCon::OnChkWarpingEffect()
{
#ifdef _MGEN_CH
	if (m_nCurType == 7)
	{
		return;
	}
#endif
	BOOL bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();
	m_Data.SectBefore.bConsiderWarpingEffect = bConsiderWarpingEffect;

	if (m_nCurType == 7)
	{
		m_wndGridGeneral.SetWarpingEffect(bConsiderWarpingEffect);
	}
}

void CCMSectItemCon::OnChkhumbly()
{
	BOOL bHumbly = m_chkHumbly.GetCheck();
	m_Data.SectBefore.bHumblyChk = bHumbly;
	m_Data.SectAfter.bHumblyChk = bHumbly;
}

void CCMSectItemCon::OnChkhumblyBefore()
{
	BOOL bHumbly = m_chkHumblyBefore.GetCheck();
	m_Data.SectBefore.bHumblyChk = bHumbly;
}

void CCMSectItemCon::OnChkhumblyAfter()
{
	BOOL bHumbly = m_chkHumblyAfter.GetCheck();
	m_Data.SectAfter.bHumblyChk = bHumbly;
}

void CCMSectItemCon::OnChkPSCDesign()
{
	//   BOOL bPSCDesign = m_chkPSCDesign.GetCheck();
	// 	
	//   if (m_nCurType == 7)
	//   {
	//     m_wndGridGeneral.SetPSCDesign(bPSCDesign);
	//   }
	BOOL bPSCDesign = m_chkPSCDesign.GetCheck();

	GetDlgItem(IDC_GENERAL_PSC_DESIGN_SHEAR_STC)->EnableWindow(bPSCDesign);
	GetDlgItem(IDC_GENERAL_PSC_DESIGN_SHEAR_EDT)->EnableWindow(bPSCDesign);
	GetDlgItem(IDC_GENERAL_PSC_DESIGN_SHEAR_UNT)->EnableWindow(bPSCDesign);
}

void CCMSectItemCon::OnCmdStiffenerBtn()
{
	CCMSectStiffenerDlg dlg;

	CDlgUtil::CobxGetItemDataByCurSel(m_cboSecType, m_nCurType);

	dlg.m_bTapJ = FALSE;

	if     (m_nCurType == 1) dlg.m_Data = m_DataBox; // Steel Box
	else if(m_nCurType == 2) dlg.m_Data = m_DataI;   // Steel-I
	else if(m_nCurType == 8) dlg.m_Data = m_DataTub; // Steel Tub
	else ASSERT(0);

	if(dlg.DoModal() == IDOK)
	{
		if     (m_nCurType == 1) m_DataBox = dlg.m_Data; // Steel Box
		else if(m_nCurType == 2) m_DataI   = dlg.m_Data; // Steel-I
		else if(m_nCurType == 8) m_DataTub = dlg.m_Data; // Steel Tub
		else ASSERT(0);		

		EnableDisable7DOTbyStiffener();
	}

	if     (m_nCurType == 1) m_wndSecViewBox.SetDataSource(&m_DataBox);
	else if(m_nCurType == 2) m_wndSecViewI  .SetDataSource(&m_DataI);
	else if(m_nCurType == 8) m_wndSecViewTub.SetDataSource(&m_DataTub);

	RedrawWindow();
}

void CCMSectItemCon::EnableDisableStlGirder()
{
	// m_nCurType : 9 = Comp Steel Girder Box, 10 = Comp Steel Girder I, 11 = Comp Steel Girder Tub

	GetDlgItem(IDC_CMD_SECT_SG_STC)->EnableWindow(!m_bSymAutoCalc);
	GetDlgItem(IDC_CMD_SECT_SG_EDT)->EnableWindow(!m_bSymAutoCalc);
	GetDlgItem(IDC_CMD_SECT_TOP_STC)->EnableWindow(!m_bSymAutoCalc);
	GetDlgItem(IDC_CMD_SECT_TOP_EDT)->EnableWindow(!m_bSymAutoCalc);
	GetDlgItem(IDC_CMD_SECT_BOT_STC)->EnableWindow(!m_bSymAutoCalc);
	GetDlgItem(IDC_CMD_SECT_BOT_EDT)->EnableWindow(!m_bSymAutoCalc);
	GetDlgItem(IDC_CMD_SECT_BOT_UNT)->EnableWindow(!m_bSymAutoCalc);
	GetDlgItem(IDC_CMD_SECT_B1_STC)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_SECT_B1_EDT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_SECT_B2_STC)->EnableWindow(m_nCurType == 9 || (m_nCurType == 10 && !m_bSymAutoCalc) || m_nCurType == 11);
	GetDlgItem(IDC_CMD_SECT_B2_EDT)->EnableWindow(m_nCurType == 9 || (m_nCurType == 10 && !m_bSymAutoCalc) || m_nCurType == 11);
	GetDlgItem(IDC_CMD_SECT_B3_STC)->EnableWindow((m_nCurType == 9 && !m_bSymAutoCalc) || m_nCurType == 10 || (m_nCurType == 11 && !m_bSymAutoCalc));
	GetDlgItem(IDC_CMD_SECT_B3_EDT)->EnableWindow((m_nCurType == 9 && !m_bSymAutoCalc) || m_nCurType == 10 || (m_nCurType == 11 && !m_bSymAutoCalc));
	GetDlgItem(IDC_CMD_SECT_B4_EDT)->EnableWindow(m_nCurType == 9 || (m_nCurType == 10 && !m_bSymAutoCalc) || m_nCurType == 11);
	GetDlgItem(IDC_CMD_SECT_B4_STC)->EnableWindow(m_nCurType == 9 || (m_nCurType == 10 && !m_bSymAutoCalc) || m_nCurType == 11);
	GetDlgItem(IDC_CMD_SECT_B5_STC)->EnableWindow(m_nCurType == 9 || m_nCurType == 11);
	GetDlgItem(IDC_CMD_SECT_B5_EDT)->EnableWindow(m_nCurType == 9 || m_nCurType == 11);
	GetDlgItem(IDC_CMD_SECT_B6_STC)->EnableWindow((m_nCurType == 9 && !m_bSymAutoCalc) || (m_nCurType == 11 && !m_bSymAutoCalc));
	GetDlgItem(IDC_CMD_SECT_B6_EDT)->EnableWindow((m_nCurType == 9 && !m_bSymAutoCalc) || (m_nCurType == 11 && !m_bSymAutoCalc));
	GetDlgItem(IDC_CMD_SECT_H_STC)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_SECT_H_EDT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_SECT_T1_STC)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_SECT_T1_EDT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_SECT_T2_STC)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_SECT_T2_EDT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_SECT_TW1_STC)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_SECT_TW1_EDT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_SECT_TW2_STC)->EnableWindow((m_nCurType == 9 && !m_bSymAutoCalc) || (m_nCurType == 11 && !m_bSymAutoCalc));
	GetDlgItem(IDC_CMD_SECT_TW2_EDT)->EnableWindow((m_nCurType == 9 && !m_bSymAutoCalc) || (m_nCurType == 11 && !m_bSymAutoCalc));
	GetDlgItem(IDC_CMD_SECT_BF1_STC)->EnableWindow(m_nCurType == 11);
	GetDlgItem(IDC_CMD_SECT_BF1_EDT)->EnableWindow(m_nCurType == 11);  
	GetDlgItem(IDC_CMD_SECT_BF2_STC)->EnableWindow(m_nCurType == 11 && !m_bSymAutoCalc);
	GetDlgItem(IDC_CMD_SECT_BF2_EDT)->EnableWindow(m_nCurType == 11 && !m_bSymAutoCalc);
	GetDlgItem(IDC_CMD_SECT_TFP_STC)->EnableWindow(m_nCurType == 11);
	GetDlgItem(IDC_CMD_SECT_TFP_EDT)->EnableWindow(m_nCurType == 11);

	if(m_nCurType == 9 || m_nCurType == 11)
	{
		GetDlgItem(IDC_CMD_SECT_TW1_STC)->SetWindowText(_T("tw1"));
	}
	else if(m_nCurType == 10)
	{
		GetDlgItem(IDC_CMD_SECT_TW1_STC)->SetWindowText(_T("tw"));
	}
}

void CCMSectItemCon::EnableDisable7DOTbyStiffener()
{
	if (m_bVirtual) return;

	BOOL bEnable7th = CSectDB::IsEnable7thDOF();

	if(bEnable7th)
	{
		m_chkWarpingEffect.EnableWindow(TRUE);		
	}
	else 
	{
		m_chkWarpingEffect.SetCheck(0);
		m_chkWarpingEffect.EnableWindow(FALSE);
	}
}

void CCMSectItemCon::OnChangeBc()
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectAfter.SectJ.Size[0] = m_StlGirderBc.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectAfter.SectJ.Size[0] = m_StlGirderBc.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectAfter.SectJ.Size[0] = m_StlGirderBc.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}

	if(m_bSymAutoCalc) OnSymmetricAutoCalcBtn();
}

void CCMSectItemCon::OnChangetc()
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectAfter.SectJ.Size[1] = m_StlGirderTc.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectAfter.SectJ.Size[1] = m_StlGirderTc.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectAfter.SectJ.Size[1] = m_StlGirderTc.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeHh()
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectAfter.SectJ.Size[2] = m_StlGirderHh.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectAfter.SectJ.Size[2] = m_StlGirderHh.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectAfter.SectJ.Size[2] = m_StlGirderHh.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnSymmetricAutoCalcBtn()
{
	UpdateData(TRUE);
	EnableDisableStlGirder();

	if(m_bSymAutoCalc)
	{
		SymmetricSectAutoCalc();
		Dlg2Data();

		if     (m_nCurType == 9) m_wndSecViewStlGirderBox.Invalidate();
		else if(m_nCurType == 10) m_wndSecViewStlGirderI.Invalidate();
		else if(m_nCurType == 11) m_wndSecViewStlGirderTub.Invalidate();
		else       		           ASSERT(0);
	}  
}


void CCMSectItemCon::SymmetricSectAutoCalc()
{
	double dBc = m_StlGirderBc.GetEditValue();
	double dB1 = m_edtStlGirderB1.GetEditValue();
	double dB2 = m_edtStlGirderB2.GetEditValue();
	double dB3 = m_edtStlGirderB3.GetEditValue();
	double dB4 = m_edtStlGirderB4.GetEditValue();
	double dB5 = m_edtStlGirderB5.GetEditValue();
	double dtw1 = m_edtStlGirdertw1.GetEditValue();
	double dbf1 = m_edtStlGirderbf1.GetEditValue();

	double dTop, dBot, dSg; // 계산되어 나오는 변수
	if(m_nCurType == 9)
	{
		CSectUtil::CompStlGSymmetricSectAutoCalc(D_SECT_TYPE_COMPO_STLG_B, dBc, dB1, dB2, dB3, dB4, dB5, dtw1, dbf1, FALSE, dSg, dTop, dBot, m_DataStlGirderBox);

		m_edtStlGirderSg.SetEditUnit(dSg);
		m_edtStlGirderTop.SetEditUnit(dTop);
		m_edtStlGirderBot.SetEditUnit(dBot);
		m_edtStlGirderB3.SetEditUnit(dB1);
		m_edtStlGirderB6.SetEditUnit(dB4);
		m_edtStlGirdertw2.SetEditUnit(dtw1);
	}
	else if(m_nCurType == 10)
	{
		CSectUtil::CompStlGSymmetricSectAutoCalc(D_SECT_TYPE_COMPO_STLG_I, dBc, dB1, dB2, dB3, dB4, dB5, dtw1, dbf1, FALSE, dSg, dTop, dBot, m_DataStlGirderI);

		m_edtStlGirderSg.SetEditUnit(dSg);
		m_edtStlGirderTop.SetEditUnit(dTop);
		m_edtStlGirderBot.SetEditUnit(dBot);
		m_edtStlGirderB2.SetEditUnit(dB1);
		m_edtStlGirderB4.SetEditUnit(dB3);
	}  
	else if(m_nCurType == 11)
	{
		CSectUtil::CompStlGSymmetricSectAutoCalc(D_SECT_TYPE_COMPO_STLG_TUB, dBc, dB1, dB2, dB3, dB4, dB5, dtw1, dbf1, FALSE, dSg, dTop, dBot, m_DataStlGirderBox);

		m_edtStlGirderSg.SetEditUnit(dSg);
		m_edtStlGirderTop.SetEditUnit(dTop);
		m_edtStlGirderBot.SetEditUnit(dBot);
		m_edtStlGirderB3.SetEditUnit(dB1);
		m_edtStlGirderB6.SetEditUnit(dB4);
		m_edtStlGirdertw2.SetEditUnit(dtw1);
		m_edtStlGirderbf2.SetEditUnit(dbf1);
	}  
}

void CCMSectItemCon::OnChangeSg() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeTop() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeBot() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeB1() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.SectI.Size[0] = m_edtStlGirderB1.GetEditValue();

		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		// Auto Calc되는 값
		m_DataStlGirderBox.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_DataStlGirderBox.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_DataStlGirderBox.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();
		m_DataStlGirderBox.SectBefore.SectI.Size[2]  = m_edtStlGirderB3.GetEditValue();

		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.SectI.Size[0] = m_edtStlGirderB1.GetEditValue();

		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		// Auto Calc되는 값
		m_DataStlGirderI.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_DataStlGirderI.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_DataStlGirderI.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();
		m_DataStlGirderI.SectBefore.SectI.Size[1]  = m_edtStlGirderB2.GetEditValue();

		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[0] = m_edtStlGirderB1.GetEditValue();

		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		// Auto Calc되는 값
		m_DataStlGirderTub.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_DataStlGirderTub.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_DataStlGirderTub.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();
		m_DataStlGirderTub.SectBefore.SectI.Size[2]  = m_edtStlGirderB3.GetEditValue();

		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeB2() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.SectI.Size[1] = m_edtStlGirderB2.GetEditValue();

		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		// Auto Calc되는 값
		m_DataStlGirderBox.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_DataStlGirderBox.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_DataStlGirderBox.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();

		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.SectI.Size[1] = m_edtStlGirderB2.GetEditValue();
		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[1] = m_edtStlGirderB2.GetEditValue();

		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		// Auto Calc되는 값
		m_DataStlGirderTub.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_DataStlGirderTub.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_DataStlGirderTub.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();

		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeB3() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.SectI.Size[2] = m_edtStlGirderB3.GetEditValue();
		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.SectI.Size[2] = m_edtStlGirderB3.GetEditValue();

		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		// Auto Calc되는 값
		m_DataStlGirderI.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_DataStlGirderI.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_DataStlGirderI.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();
		m_DataStlGirderI.SectBefore.SectI.Size[3] = m_edtStlGirderB4.GetEditValue();

		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[2] = m_edtStlGirderB3.GetEditValue();
		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeB4() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.SectI.Size[3] = m_edtStlGirderB4.GetEditValue();

		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		// Auto Calc되는 값
		m_DataStlGirderBox.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_DataStlGirderBox.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_DataStlGirderBox.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();
		m_DataStlGirderBox.SectBefore.SectI.Size[5]  = m_edtStlGirderB6.GetEditValue();

		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.SectI.Size[3] = m_edtStlGirderB4.GetEditValue();
		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[3] = m_edtStlGirderB4.GetEditValue();

		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		// Auto Calc되는 값
		m_DataStlGirderTub.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_DataStlGirderTub.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_DataStlGirderTub.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();
		m_DataStlGirderTub.SectBefore.SectI.Size[5]  = m_edtStlGirderB6.GetEditValue();

		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeB5() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.SectI.Size[4] = m_edtStlGirderB5.GetEditValue();

		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		// Auto Calc되는 값
		m_DataStlGirderBox.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_DataStlGirderBox.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_DataStlGirderBox.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();

		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[4] = m_edtStlGirderB5.GetEditValue();

		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		// Auto Calc되는 값
		m_DataStlGirderTub.SectAfter.SectI.Size[3] = m_edtStlGirderSg.GetEditValue();
		m_DataStlGirderTub.SectAfter.SectI.Size[4] = m_edtStlGirderTop.GetEditValue();
		m_DataStlGirderTub.SectAfter.SectI.Size[5] = m_edtStlGirderBot.GetEditValue();

		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeB6() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.SectI.Size[5] = m_edtStlGirderB6.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[5] = m_edtStlGirderB6.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeH() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.SectI.Size[6] = m_edtStlGirderH.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.SectI.Size[4] = m_edtStlGirderH.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[6] = m_edtStlGirderH.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChanget1() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.SectI.Size[7] = m_edtStlGirdert1.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.SectI.Size[5] = m_edtStlGirdert1.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[7] = m_edtStlGirdert1.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChanget2() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.SectI.Size[8] = m_edtStlGirdert2.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.SectI.Size[6] = m_edtStlGirdert2.GetEditValue();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[8] = m_edtStlGirdert2.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangetw1() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.SectI.Size[9] = m_edtStlGirdertw1.GetEditValue();
		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 10)
	{
		m_DataStlGirderI.SectBefore.SectI.Size[7] = m_edtStlGirdertw1.GetEditValue();
		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[9] = m_edtStlGirdertw1.GetEditValue();
		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangetw2() 
{
	if(m_nCurType == 9)
	{
		m_DataStlGirderBox.SectBefore.SectI.Size[10] = m_edtStlGirdertw2.GetEditValue();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[10] = m_edtStlGirdertw2.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeBf1() 
{
	if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[11] = m_edtStlGirderbf1.GetEditValue();

		if(m_bSymAutoCalc) SymmetricSectAutoCalc();
		// Auto Calc되는 값
		m_DataStlGirderTub.SectBefore.SectI.Size[12] = m_edtStlGirderbf2.GetEditValue();

		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangeBf2() 
{
	if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[12] = m_edtStlGirderbf2.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnChangetfp() 
{
	if(m_nCurType == 11)
	{
		m_DataStlGirderTub.SectBefore.SectI.Size[13] = m_edtStlGirdertfp.GetEditValue();
		m_wndSecViewStlGirderTub.Invalidate();
	}
}

void CCMSectItemCon::OnStiffenerBtn()
{
	CCMSectSODDeckStiffenerDlg dlg;

	CDlgUtil::CobxGetItemDataByCurSel(m_cboSecType, m_nCurType);

	dlg.m_bTapJ = FALSE;

	if     (m_nCurType == 9) dlg.m_Data  = m_DataStlGirderBox; // Steel Box
	else if(m_nCurType == 10) dlg.m_Data = m_DataStlGirderI;   // Steel-I
	else if(m_nCurType == 11) dlg.m_Data = m_DataStlGirderTub; // Steel-Tub
	else ASSERT(0);

	if(dlg.DoModal() == IDOK)
	{
		if     (m_nCurType == 9) m_DataStlGirderBox  = dlg.m_Data; // Steel Box
		else if(m_nCurType == 10) m_DataStlGirderI   = dlg.m_Data; // Steel-I
		else if(m_nCurType == 11) m_DataStlGirderTub = dlg.m_Data; // Steel-Tub
		else ASSERT(0);		
	}

	if     (m_nCurType == 9) m_wndSecViewStlGirderBox .SetDataSource(&m_DataStlGirderBox);
	else if(m_nCurType == 10) m_wndSecViewStlGirderI  .SetDataSource(&m_DataStlGirderI);
	else if(m_nCurType == 11) m_wndSecViewStlGirderTub.SetDataSource(&m_DataStlGirderTub);

	RedrawWindow();
}

void CCMSectItemCon::InitGrid()
{
	m_wndGridFirst.InitGrid(m_pDoc);
	m_wndGridSecond.InitGrid(m_pDoc);

	m_wndGridCI.InitGrid(&m_wndSecViewCI);
	m_wndGridCT.InitGrid(&m_wndSecViewCT);

	m_wndGridGeneral.InitGrid(&m_wndSecViewGeneral);
}

void CCMSectItemCon::InitView()
{
	// preview control
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW1);
	ASSERT(pWnd);
	m_wndSecViewFirst.Init(pWnd);

	pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW2);
	ASSERT(pWnd);
	m_wndSecViewSecond.Init(pWnd);

	pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW5);
	ASSERT(pWnd);
	m_wndSecViewBox.Init(pWnd);

	pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW6);
	ASSERT(pWnd);
	m_wndSecViewI.Init(pWnd);

	pWnd = GetDlgItem(IDC_CMD_SP_CI_PREVIEW2);
	ASSERT(pWnd);
	m_wndSecViewCI.Init(pWnd);

	pWnd = GetDlgItem(IDC_CMD_SP_CT_PREVIEW2);
	ASSERT(pWnd);
	m_wndSecViewCT.Init(pWnd);

	pWnd = GetDlgItem(IDC_CMD_PSC_PREVIEW1);
	ASSERT(pWnd);
	m_wndSecViewPC.Init(pWnd);

	pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW8);
	ASSERT(pWnd);
	m_wndSecViewGeneral.Init(pWnd);

	pWnd = GetDlgItem(IDC_CMD_TUB_PREVIEW2);
	ASSERT(pWnd);
	m_wndSecViewTub.Init(pWnd);

	pWnd = GetDlgItem(IDC_CMD_SECT_B_VIEW);
	ASSERT(pWnd);
	m_wndSecViewStlGirderBox.Init(pWnd);

	pWnd = GetDlgItem(IDC_CMD_SECT_I_VIEW);
	ASSERT(pWnd);
	m_wndSecViewStlGirderI.Init(pWnd);  

	pWnd = GetDlgItem(IDC_CMD_SECT_TUB_VIEW);
	ASSERT(pWnd);
	m_wndSecViewStlGirderTub.Init(pWnd);  
}

void CCMSectItemCon::InitImage()
{
	m_ImgViewerPREVIEW3     .SetImage(_T("SVG\\illustration\\Dialog\\SectComp01.svg"));            // IDB_CMD_SP_COMP_BMP01
	m_ImgViewerPREVIEW4     .SetImage(_T("SVG\\illustration\\Dialog\\SectComp02.svg"));            // IDB_CMD_SP_COMP_BMP02
	m_ImgViewerCI_PREVIEW1  .SetImage(_T("SVG\\illustration\\Dialog\\cmd_sp_ci.svg"));             // IDB_CMD_SP_COMP_BMP03
	m_ImgViewerCT_PREVIEW1  .SetImage(_T("SVG\\illustration\\Dialog\\cmd_sp_ct.svg"));             // IDB_CMD_SP_COMP_BMP04
	m_ImgViewerPSC_PREVIEW  .SetImage(_T("SVG\\illustration\\Dialog\\cmd_sp_ct.svg"));             // IDB_CMD_SP_COMP_BMP04
	m_ImgViewerPREVIEW7     .SetImage(_T("SVG\\illustration\\Dialog\\Sec25.svg"));                 // IDB_CMD_SP_ISGL_BMP25
	m_ImgViewerTUB_PREVIEW1 .SetImage(_T("SVG\\illustration\\Dialog\\SectComp03.svg"));            // IDB_CMD_SP_COMP_BMP05
	m_ImgViewerB_GUIDE_BMP  .SetImage(_T("SVG\\illustration\\Dialog\\SectCompStlGirder_Box.svg")); // IDB_CMD_SEC_COMP_STL_GIRDER_BOX
	m_ImgViewerI_GUIDE_BMP  .SetImage(_T("SVG\\illustration\\Dialog\\SectCompStlGirder_I.svg"));   // IDB_CMD_SEC_COMP_STL_GIRDER_I
	m_ImgViewerTUB_GUIDE_BMP.SetImage(_T("SVG\\illustration\\Dialog\\SectCompStlGirder_Tub.svg")); // IDB_CMD_SEC_COMP_STL_GIRDER_TUB

}

void CCMSectItemCon::InitUnit()
{
	m_unit9.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unit8.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unit7.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unit6.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unit5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unit4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unit3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unit12.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unit11.SetUnitType(D_UNITSYS_BASE_LENGTH);
	//  m_unit10.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unit1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untBf3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_tw.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tr2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tr1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tf2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tf1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_StlGirderTc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_swidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hw.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hr2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hr1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hh.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_StlGirderHh.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ESEC.SetUnitType(0); // fixed by mylee 20070307
	m_DSDC.SetUnitType(0);
	m_PS.SetUnitType(0); // MNET:XXXX 20070307 mylee
	m_PC.SetUnitType(0);
	m_TSTC.SetUnitType(0);
	m_CTC.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bf2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bf3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_tfp.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_StlGirderBc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bf1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_B2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_B1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMat1.SetUnitType(0); // MNET:2582 20061129 mylee
	m_edtMat2.SetUnitType(0);
	m_edtPSCShear.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPSCShear.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStlGirderSlab.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirderTop.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirderBot.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStlGirderBot.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirderB1 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirderB2 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirderB3 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirderB4 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirderB5 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirderB6 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirderH  .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStlGirderB3  .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirdert1 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStlGirderB6 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirdert2 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStlGirdert2 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirdertw1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStlGirdertBf1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirdertw2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStlGirderBf2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStlGirderSWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStlGirderCTC.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirderbf1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirderbf2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStlGirdertfp.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMSectItemCon::ShowStlGirderBoxData2Dlg()
{
	CString str;
	// write //////////////////////////////////////
	str.Format(_T("%g"), m_DataStlGirderBox.SectAfter.SectJ.Size[0]); m_StlGirderBc.SetWindowText(str);
	str.Format(_T("%g"), m_DataStlGirderBox.SectAfter.SectJ.Size[1]); m_StlGirderTc.SetWindowText(str);
	str.Format(_T("%g"), m_DataStlGirderBox.SectAfter.SectJ.Size[2]); m_StlGirderHh.SetWindowText(str);

	m_edtStlGirderSg.SetEditUnit(m_DataStlGirderBox.SectAfter.SectI.Size[3]);
	m_edtStlGirderTop.SetEditUnit(m_DataStlGirderBox.SectAfter.SectI.Size[4]);
	m_edtStlGirderBot.SetEditUnit(m_DataStlGirderBox.SectAfter.SectI.Size[5]);

	m_edtStlGirderB1.SetEditUnit( m_DataStlGirderBox.SectBefore.SectI.Size[0]);
	m_edtStlGirderB2.SetEditUnit( m_DataStlGirderBox.SectBefore.SectI.Size[1]);
	m_edtStlGirderB3.SetEditUnit( m_DataStlGirderBox.SectBefore.SectI.Size[2]);
	m_edtStlGirderB4.SetEditUnit( m_DataStlGirderBox.SectBefore.SectI.Size[3]);
	m_edtStlGirderB5.SetEditUnit( m_DataStlGirderBox.SectBefore.SectI.Size[4]);
	m_edtStlGirderB6.SetEditUnit( m_DataStlGirderBox.SectBefore.SectI.Size[5]);
	m_edtStlGirderH .SetEditUnit( m_DataStlGirderBox.SectBefore.SectI.Size[6]);
	m_edtStlGirdert1.SetEditUnit( m_DataStlGirderBox.SectBefore.SectI.Size[7]);
	m_edtStlGirdert2.SetEditUnit( m_DataStlGirderBox.SectBefore.SectI.Size[8]);
	m_edtStlGirdertw1.SetEditUnit(m_DataStlGirderBox.SectBefore.SectI.Size[9]);
	m_edtStlGirdertw2.SetEditUnit(m_DataStlGirderBox.SectBefore.SectI.Size[10]);

	m_ESEC.SetEditUnit(m_DataStlGirderBox.SectBefore.Matl_Elast);       // 	str.Format(_T("%g"), m_DataStlGirderBox.SectBefore.Matl_Elast);   m_ESEC.SetWindowText(str);
	m_DSDC.SetEditUnit(m_DataStlGirderBox.SectBefore.Matl_Density);     // 	str.Format(_T("%g"), m_DataStlGirderBox.SectBefore.Matl_Density); m_DSDC.SetWindowText(str);
	m_PS  .SetEditUnit(m_DataStlGirderBox.SectBefore.Matl_Poisson_S);   // 	str.Format(_T("%g"), m_DataStlGirderBox.SectBefore.Matl_Poisson_S); m_PS.SetWindowText(str);
	m_PC  .SetEditUnit(m_DataStlGirderBox.SectBefore.Matl_Poisson_C);   // 	str.Format(_T("%g"), m_DataStlGirderBox.SectBefore.Matl_Poisson_C); m_PC.SetWindowText(str);
	m_TSTC.SetEditUnit(m_DataStlGirderBox.SectBefore.Matl_Thermal);     // 	str.Format(_T("%g"), m_DataStlGirderBox.SectBefore.Matl_Thermal); m_TSTC.SetWindowText(str);

	m_chkMat.SetCheck(m_DataStlGirderBox.SectBefore.bMultipleElastic);
	m_edtMat1.SetEditUnit(m_DataStlGirderBox.SectBefore.dLongTermESEC); //	str.Format(_T("%g"), m_DataStlGirderBox.SectBefore.dLongTermESEC); m_edtMat1.SetWindowText(str);
	m_edtMat2.SetEditUnit(m_DataStlGirderBox.SectBefore.dShrinkageESEC);//	str.Format(_T("%g"), m_DataStlGirderBox.SectBefore.dShrinkageESEC); m_edtMat2.SetWindowText(str); 

	///////////////////////////////////////////////
	m_Bf3.SetWindowText(_T("0"));
	m_tfp.SetWindowText(_T("0"));

	// set offset point
	if (m_DataStlGirderBox.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataStlGirderBox.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_DataStlGirderBox.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_DataStlGirderBox.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// Humbly Check
	m_chkHumbly.SetCheck((int)m_DataStlGirderBox.SectBefore.bHumblyChk);
	//m_chkHumbly.ShowWindow(SW_HIDE);

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);

	UpdateData(FALSE);
}

void CCMSectItemCon::ShowStlGirderIData2Dlg()
{
	CString str;
	// write //////////////////////////////////////
	str.Format(_T("%g"), m_DataStlGirderI.SectAfter.SectJ.Size[0]); m_StlGirderBc.SetWindowText(str);
	str.Format(_T("%g"), m_DataStlGirderI.SectAfter.SectJ.Size[1]); m_StlGirderTc.SetWindowText(str);
	str.Format(_T("%g"), m_DataStlGirderI.SectAfter.SectJ.Size[2]); m_StlGirderHh.SetWindowText(str);

	m_edtStlGirderSg.SetEditUnit(m_DataStlGirderI.SectAfter.SectI.Size[3]);
	m_edtStlGirderTop.SetEditUnit(m_DataStlGirderI.SectAfter.SectI.Size[4]);
	m_edtStlGirderBot.SetEditUnit(m_DataStlGirderI.SectAfter.SectI.Size[5]);

	m_edtStlGirderB1.SetEditUnit( m_DataStlGirderI.SectBefore.SectI.Size[0]);
	m_edtStlGirderB2.SetEditUnit( m_DataStlGirderI.SectBefore.SectI.Size[1]);
	m_edtStlGirderB3.SetEditUnit( m_DataStlGirderI.SectBefore.SectI.Size[2]);
	m_edtStlGirderB4.SetEditUnit( m_DataStlGirderI.SectBefore.SectI.Size[3]);
	m_edtStlGirderH .SetEditUnit( m_DataStlGirderI.SectBefore.SectI.Size[4]);
	m_edtStlGirdert1.SetEditUnit( m_DataStlGirderI.SectBefore.SectI.Size[5]);
	m_edtStlGirdert2.SetEditUnit( m_DataStlGirderI.SectBefore.SectI.Size[6]);
	m_edtStlGirdertw1.SetEditUnit(m_DataStlGirderI.SectBefore.SectI.Size[7]);

	m_ESEC.SetEditUnit(m_DataStlGirderI.SectBefore.Matl_Elast);     // 	str.Format(_T("%g"), m_DataStlGirderI.SectBefore.Matl_Elast);   m_ESEC.SetWindowText(str);
	m_DSDC.SetEditUnit(m_DataStlGirderI.SectBefore.Matl_Density);   // 	str.Format(_T("%g"), m_DataStlGirderI.SectBefore.Matl_Density); m_DSDC.SetWindowText(str);
	m_PS  .SetEditUnit(m_DataStlGirderI.SectBefore.Matl_Poisson_S);	// 	str.Format(_T("%g"), m_DataStlGirderI.SectBefore.Matl_Poisson_S); m_PS.SetWindowText(str);
	m_PC  .SetEditUnit(m_DataStlGirderI.SectBefore.Matl_Poisson_C);	// 	str.Format(_T("%g"), m_DataStlGirderI.SectBefore.Matl_Poisson_C); m_PC.SetWindowText(str);
	m_TSTC.SetEditUnit(m_DataStlGirderI.SectBefore.Matl_Thermal); 	// 	str.Format(_T("%g"), m_DataStlGirderI.SectBefore.Matl_Thermal); m_TSTC.SetWindowText(str);

	m_chkMat.SetCheck(m_DataStlGirderI.SectBefore.bMultipleElastic);
	m_edtMat1.SetEditUnit(m_DataStlGirderI.SectBefore.dLongTermESEC);  // 	str.Format(_T("%g"), m_DataStlGirderI.SectBefore.dLongTermESEC); m_edtMat1.SetWindowText(str);
	m_edtMat2.SetEditUnit(m_DataStlGirderI.SectBefore.dShrinkageESEC); // 	str.Format(_T("%g"), m_DataStlGirderI.SectBefore.dShrinkageESEC); m_edtMat2.SetWindowText(str);

	///////////////////////////////////////////////
	m_Bf3.SetWindowText(_T("0"));
	m_tfp.SetWindowText(_T("0"));

	// set offset point
	if (m_DataStlGirderI.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataStlGirderI.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_DataStlGirderI.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_DataStlGirderI.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// Humbly Check
	m_chkHumbly.SetCheck((int)m_DataStlGirderI.SectBefore.bHumblyChk);
	//m_chkHumbly.ShowWindow(SW_HIDE);

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);

	UpdateData(FALSE);
}

void CCMSectItemCon::ShowStlGirderTubData2Dlg()
{
	CString str;
	// write //////////////////////////////////////
	str.Format(_T("%g"), m_DataStlGirderTub.SectAfter.SectJ.Size[0]); m_StlGirderBc.SetWindowText(str);
	str.Format(_T("%g"), m_DataStlGirderTub.SectAfter.SectJ.Size[1]); m_StlGirderTc.SetWindowText(str);
	str.Format(_T("%g"), m_DataStlGirderTub.SectAfter.SectJ.Size[2]); m_StlGirderHh.SetWindowText(str);

	m_edtStlGirderSg.SetEditUnit(m_DataStlGirderTub.SectAfter.SectI.Size[3]);
	m_edtStlGirderTop.SetEditUnit(m_DataStlGirderTub.SectAfter.SectI.Size[4]);
	m_edtStlGirderBot.SetEditUnit(m_DataStlGirderTub.SectAfter.SectI.Size[5]);

	m_edtStlGirderB1.SetEditUnit( m_DataStlGirderTub.SectBefore.SectI.Size[0]);
	m_edtStlGirderB2.SetEditUnit( m_DataStlGirderTub.SectBefore.SectI.Size[1]);
	m_edtStlGirderB3.SetEditUnit( m_DataStlGirderTub.SectBefore.SectI.Size[2]);
	m_edtStlGirderB4.SetEditUnit( m_DataStlGirderTub.SectBefore.SectI.Size[3]);
	m_edtStlGirderB5.SetEditUnit( m_DataStlGirderTub.SectBefore.SectI.Size[4]);
	m_edtStlGirderB6.SetEditUnit( m_DataStlGirderTub.SectBefore.SectI.Size[5]);
	m_edtStlGirderH .SetEditUnit( m_DataStlGirderTub.SectBefore.SectI.Size[6]);
	m_edtStlGirdert1.SetEditUnit( m_DataStlGirderTub.SectBefore.SectI.Size[7]);
	m_edtStlGirdert2.SetEditUnit( m_DataStlGirderTub.SectBefore.SectI.Size[8]);
	m_edtStlGirdertw1.SetEditUnit(m_DataStlGirderTub.SectBefore.SectI.Size[9]);
	m_edtStlGirdertw2.SetEditUnit(m_DataStlGirderTub.SectBefore.SectI.Size[10]);
	m_edtStlGirderbf1.SetEditUnit(m_DataStlGirderTub.SectBefore.SectI.Size[11]);
	m_edtStlGirderbf2.SetEditUnit(m_DataStlGirderTub.SectBefore.SectI.Size[12]);
	m_edtStlGirdertfp.SetEditUnit(m_DataStlGirderTub.SectBefore.SectI.Size[13]);

	m_ESEC.SetEditUnit(m_DataStlGirderTub.SectBefore.Matl_Elast) ; //str.Format(_T("%g"), m_DataStlGirderTub.SectBefore.Matl_Elast);   m_ESEC.SetWindowText(str);
	m_DSDC.SetEditUnit(m_DataStlGirderTub.SectBefore.Matl_Density) ; //str.Format(_T("%g"), m_DataStlGirderTub.SectBefore.Matl_Density); m_DSDC.SetWindowText(str);
	m_PS  .SetEditUnit(m_DataStlGirderTub.SectBefore.Matl_Poisson_S) ; //str.Format(_T("%g"), m_DataStlGirderTub.SectBefore.Matl_Poisson_S); m_PS.SetWindowText(str);
	m_PC  .SetEditUnit(m_DataStlGirderTub.SectBefore.Matl_Poisson_C) ; //str.Format(_T("%g"), m_DataStlGirderTub.SectBefore.Matl_Poisson_C); m_PC.SetWindowText(str);
	m_TSTC.SetEditUnit(m_DataStlGirderTub.SectBefore.Matl_Thermal) ; //str.Format(_T("%g"), m_DataStlGirderTub.SectBefore.Matl_Thermal); m_TSTC.SetWindowText(str);

	m_chkMat.SetCheck(m_DataStlGirderTub.SectBefore.bMultipleElastic);
	m_edtMat1.SetEditUnit(m_DataStlGirderTub.SectBefore.dLongTermESEC) ; //str.Format(_T("%g"), m_DataStlGirderTub.SectBefore.dLongTermESEC); m_edtMat1.SetWindowText(str);
	m_edtMat2.SetEditUnit(m_DataStlGirderTub.SectBefore.dShrinkageESEC) ; //str.Format(_T("%g"), m_DataStlGirderTub.SectBefore.dShrinkageESEC); m_edtMat2.SetWindowText(str);

	///////////////////////////////////////////////
	m_Bf3.SetWindowText(_T("0"));
	m_tfp.SetWindowText(_T("0"));

	// set offset point
	if (m_DataStlGirderTub.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataStlGirderTub.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_DataStlGirderTub.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_DataStlGirderTub.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// Humbly Check
	m_chkHumblyBefore.SetCheck((int)m_DataStlGirderTub.SectBefore.bHumblyChk);
	m_chkHumblyAfter.SetCheck((int)m_DataStlGirderTub.SectAfter.bHumblyChk);
	//m_chkHumbly.ShowWindow(SW_HIDE);

	// PSC Design
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear.SetEditUnit(m_PSCDesign.dt1[0]);

	UpdateData(FALSE);
}

void CCMSectItemCon::SetSectionATypeCombo()
{
	CString aItemName[] = {
		_LS(IDS_WG_CMD_Section_Symmetry_Type_Double), _LS(IDS_WG_CMD_Section_Symmetry_Type_Y),
		_LS(IDS_WG_CMD_Section_Symmetry_Type_Z), _LS(IDS_WG_CMD_Section_Symmetry_Type_Non) };

	m_cbxAType.ResetContent();

	for (int i = 0; i < 4; i++)
	{
		m_cbxAType.AddString(aItemName[i]);
	}
}


void CCMSectItemCon::OnChangeSelAType()
{
	int nSel = m_cbxAType.GetCurSel();
	m_wndGridGeneral.SetWarpingEffect(TRUE);
}

int  CCMSectItemCon::GetSectionAType()
{
	double dy1 = m_Data.SectBefore.SectI.Stiffness.dx1;
	double dy2 = m_Data.SectBefore.SectI.Stiffness.dx2;
	double dy3 = m_Data.SectBefore.SectI.Stiffness.dx3;
	double dy4 = m_Data.SectBefore.SectI.Stiffness.dx4;
	double dz1 = m_Data.SectBefore.SectI.Stiffness.dy1;
	double dz2 = m_Data.SectBefore.SectI.Stiffness.dy2;
	double dz3 = m_Data.SectBefore.SectI.Stiffness.dy3;
	double dz4 = m_Data.SectBefore.SectI.Stiffness.dy4;
	if (fabs(dy1 + dy2) < 1.0e-10 && fabs(dy3 + dy4) < 1.0e-10 && fabs(dz2 + dz3) < 1.0e-10 && fabs(dz1 + dz4) < 1.0e-10)
		return 0;
	else if ((fabs(dy1 + dy2) > 1.0e-10 || fabs(dy3 + dy4) > 1.0e-10) && fabs(dz2 + dz3) < 1.0e-10 && fabs(dz1 + dz4) < 1.0e-10)
		return 1;
	else if (fabs(dy1 + dy2) < 1.0e-10 && fabs(dy3 + dy4) < 1.0e-10 && (fabs(dz2 + dz3) > 1.0e-10 || fabs(dz1 + dz4) > 1.0e-10))
		return 2;
	else
		return 0;
}

BOOL CCMSectItemCon::CheckData()
{
	double dIw = m_Data.SectBefore.SectI.Stiffness.dIw;
	double dys_yc = m_Data.SectBefore.SectI.Stiffness.dYsy;
	double dzs_zc = m_Data.SectBefore.SectI.Stiffness.dYsz;
	if (m_Data.nSectAType == 1)
	{
		if (dIw == 0 || dys_yc == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD_Section_Symmetry_Type_Error1));
			return FALSE;
		}
			
	}
	else if (m_Data.nSectAType == 2)
	{
		if (dIw == 0 || dzs_zc == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD_Section_Symmetry_Type_Error2));
			return FALSE;
		}
	}
	else if (m_Data.nSectAType == 3)
	{
		if (dIw == 0 || dys_yc == 0 || dzs_zc == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD_Section_Symmetry_Type_Error3));
			return FALSE;
		}
	}

	return TRUE;
}