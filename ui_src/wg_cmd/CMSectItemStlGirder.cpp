// CMSectItemStlGirder.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemStlGirder.h"
#include "CMSectSODDeckStiffenerDlg.h"
#include "CMSectRivetAngleDlg.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"

#include "CMSecPageStiffDlg.h"
#include "CMSectOffsetDlg.h"
#include "CMSectItemStlgView.h"
#include "CCMSectItemPSCDefine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_SECT_STLG_MCEL_NO_MIN 0
#define D_SECT_STLG_MCEL_NO_MAX 10

unsigned int CCMSectItemStlGirder::WM_AFTERINITDIALOG = RegisterWindowMessage(_T("WM_AFTERINITDIALOG"));

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemStlGirder dialog
CCMSectItemStlGirder::CCMSectItemStlGirder(CWnd* pParent /*=NULL*/)
	: CCMSectItemBase(CCMSectItemStlGirder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemStlGirder)
	m_bSymAutoCalc = FALSE;
	m_bShearDeform = FALSE;
	m_bWarpingEffect = FALSE;
	//}}AFX_DATA_INIT

	m_aOffsetName.RemoveAll();
	m_DataBox.Initialize();
	m_DataI.Initialize();
	m_DataMCell.Initialize();
	m_aRdoMCellLorR.RemoveAll();
	m_aRdoMCellFShape.RemoveAll();

	m_nLastType = D_SECT_TYPE_STLG_B;

	m_pStlgViewer= NULL;
}

void CCMSectItemStlGirder::DoDataExchange(CDataExchange* pDX)
{
	CCMSectItemBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemStlGirder)
	DDX_Control(pDX, IDC_CMD_SECT_ID_EDT,              m_edtID);
	DDX_Control(pDX, IDC_CMD_SECT_NAME_EDT,            m_edtName);
	DDX_Control(pDX, IDC_CMD_SECT_TYPE_CMB,            m_cmbSectType);
	DDX_Check  (pDX, IDC_CMD_SECT_SYM_AUTO_CALC_CHK,   m_bSymAutoCalc);
	DDX_Control(pDX, IDC_CMD_SECT_TOP_EDT,             m_edtTop);
	DDX_Control(pDX, IDC_CMD_SECT_TOP_UNT,             m_untTop);
	DDX_Control(pDX, IDC_CMD_SECT_BOT_EDT,             m_edtBot);
	DDX_Control(pDX, IDC_CMD_SECT_BOT_UNT,             m_untBot);
	DDX_Control(pDX, IDC_CMD_SECT_B1_EDT,              m_edtB1);
	DDX_Control(pDX, IDC_CMD_SECT_B2_EDT,              m_edtB2);
	DDX_Control(pDX, IDC_CMD_SECT_B3_EDT,              m_edtB3);
	DDX_Control(pDX, IDC_CMD_SECT_B4_EDT,              m_edtB4);
	DDX_Control(pDX, IDC_CMD_SECT_B5_EDT,              m_edtB5);
	DDX_Control(pDX, IDC_CMD_SECT_B6_EDT,              m_edtB6);
	DDX_Control(pDX, IDC_CMD_SECT_H_EDT,               m_edtH);
	DDX_Control(pDX, IDC_CMD_SECT_H_UNT,              m_untH);
	DDX_Control(pDX, IDC_CMD_SECT_T1_EDT,              m_edtt1);
	DDX_Control(pDX, IDC_CMD_SECT_T1_UNT,              m_untt1);
	DDX_Control(pDX, IDC_CMD_SECT_T2_EDT,              m_edtt2);
	DDX_Control(pDX, IDC_CMD_SECT_T2_UNT,              m_untt2);
	DDX_Control(pDX, IDC_CMD_SECT_TW1_EDT,             m_edttw1);
	DDX_Control(pDX, IDC_CMD_SECT_TW1_UNT,             m_unttw1);
	DDX_Control(pDX, IDC_CMD_SECT_TW2_EDT,             m_edttw2);
	DDX_Control(pDX, IDC_CMD_SECT_TW2_UNT,             m_unttw2);
	DDX_Check  (pDX, IDC_CMD_SHEAR_DEFORM_CHK,         m_bShearDeform);
	DDX_Check  (pDX, IDC_CMD_WARPING_EFFECT_CHK,       m_bWarpingEffect);
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT,              m_edtOffset);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_B1_EDT      , m_edtMCellB1);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_B2_EDT      , m_edtMCellB2);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_B3_EDT      , m_edtMCellB3);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_B4_EDT      , m_edtMCellB4);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_B5_EDT      , m_edtMCellB5);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_B6_EDT      , m_edtMCellB6);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_H_EDT       , m_edtMCellH);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_TF1_EDT     , m_edtMCellTf1);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_TW1_EDT     , m_edtMCellTw1);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_L_1_EDT , m_edtMCellAddL1);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_L_2_EDT , m_edtMCellAddL2);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_L_3_EDT , m_edtMCellAddL3);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_L_4_EDT , m_edtMCellAddL4);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_L_5_EDT , m_edtMCellAddL5);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_L_6_EDT , m_edtMCellAddL6);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_BR1_EDT     , m_edtMCellBr1);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_BR2_EDT     , m_edtMCellBr2);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_BR3_EDT     , m_edtMCellBr3);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_BR4_EDT     , m_edtMCellBr4);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_BR5_EDT     , m_edtMCellBr5);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_TF2_EDT     , m_edtMCellTf2);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_TW2_EDT     , m_edtMCellTw2);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_R_1_EDT , m_edtMCellAddR1);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_R_2_EDT , m_edtMCellAddR2);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_R_3_EDT , m_edtMCellAddR3);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_R_4_EDT , m_edtMCellAddR4);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_R_5_EDT , m_edtMCellAddR5);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_ADD_R_6_EDT , m_edtMCellAddR6);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_1_UNIT      , m_untMCellSize1);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_2_UNIT      , m_untMCellSize2);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_3_UNIT      , m_untMCellSize3);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_4_UNIT      , m_untMCellSize4);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_5_UNIT      , m_untMCellSize5);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_6_UNIT      , m_untMCellSize6);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_7_UNIT      , m_untMCellSize7);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_8_UNIT      , m_untMCellSize8);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_9_UNIT      , m_untMCellSize9);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_10_UNIT     , m_untMCellSize10);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_11_UNIT     , m_untMCellSize11);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_12_UNIT     , m_untMCellSize12);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_13_UNIT     , m_untMCellSize13);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_SYM_CHK     , m_chkMCellSym);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_NO_CELL_SPN , m_spnMCellNoCell);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_NO_CELL_EDT , m_edtMCellNoCell);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_J1_CHK      , m_chkMCellJ1);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_J2_CHK      , m_chkMCellJ2);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_JR1_CHK     , m_chkMCellJr1);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_JR2_CHK     , m_chkMCellJr2);

	DDX_Control(pDX, IDC_CMD_SECT_B_GUIDE_BMP, m_ImgViewer_B);
	DDX_Control(pDX, IDC_CMD_SECT_I_GUIDE_BMP, m_ImgViewer_I);

	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// Interfaces
/////////////////////////////////////////////////////////////////////////////

// 부모 Page에서 OK 버튼 눌러졌을 때 호출
BOOL CCMSectItemStlGirder::OnOKPublic()
{
	return SaveData();
}

// 부모 페이지에서 Apply 버튼 눌러졌을 때 호출
BOOL CCMSectItemStlGirder::OnApplyPublic()
{
	if (!SaveData()) return FALSE;
	if (!m_bModify)  // modify 면 그냥 두고 add면 재 초기화 
	{
		// ID만 수정한다.
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumSect(m_Key);
		ShowKey2Dlg();
	}
	else
	{
		m_OrgKey = m_Key;  // Change Origin Key
	}
	return TRUE;
}

// 강성 계산 결과를 보여준다.
void CCMSectItemStlGirder::ShowCalcResult()
{
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}

	CCMStiffDlg dlg(m_pDoc, &m_Data, NULL, this);
	dlg.DoModal();
}

void CCMSectItemStlGirder::DisplayOffsetPoint()
{
	SetOffsetData(&m_Data.SectBefore, &m_DataBox.SectBefore);
	SetOffsetData(&m_Data.SectBefore, &m_DataI.SectBefore);
	SetOffsetData(&m_Data.SectBefore, &m_DataMCell.SectBefore);
 
	m_wndSecViewStlGirderBox.Invalidate();	
	m_wndSecViewStlGirderI.Invalidate();	
	DrawViewDlgSection();
}

void CCMSectItemStlGirder::SetNewID(unsigned nNewID)
{
	m_Key = nNewID;
	ShowKey2Dlg();
}

/////////////////////////////////////////////////////////////////////////////
// Implementations
/////////////////////////////////////////////////////////////////////////////
void CCMSectItemStlGirder::InitCtrl()
{
	m_cmbSectType.ResetContent();
	
	CDlgUtil::CobxAddItem(m_cmbSectType, _LS(IDS_CMD_SECT_STEEL_GIRDER_BOX)   , D_SECT_TYPE_STLG_B);
	CDlgUtil::CobxAddItem(m_cmbSectType, _LS(IDS_CMD_SECT_STEEL_GIRDER_I)     , D_SECT_TYPE_STLG_I);

#if defined(_CIVIL_CH)
	CDlgUtil::CobxAddItem(m_cmbSectType, _LS(IDS_CMD_SECT_STEEL_GIRDER_MCELL) , D_SECT_TYPE_STLG_MCELL);
#else
	if (CTestEnvMgr::IsTestEnvST(_T("MCellTest"), _T("yes")))
	{
		CDlgUtil::CobxAddItem(m_cmbSectType, _LS(IDS_CMD_SECT_STEEL_GIRDER_MCELL) , D_SECT_TYPE_STLG_MCELL);
	}
#endif
	m_cmbSectType.SetCurSel(0);

	m_spnMCellNoCell.SetRange(D_SECT_STLG_MCEL_NO_MIN, D_SECT_STLG_MCEL_NO_MAX);

	m_aRdoMCellLorR.RemoveAll();
	m_aRdoMCellLorR.Add(IDC_CMD_SECT_MCELL_LEFT_RDO);
	m_aRdoMCellLorR.Add(IDC_CMD_SECT_MCELL_RIGHT_RDO);

	m_aRdoMCellFShape.RemoveAll();
	m_aRdoMCellFShape.Add(IDC_CMD_SECT_MCELL_GEN_RDO);
	m_aRdoMCellFShape.Add(IDC_CMD_SECT_MCELL_REBAR1_RDO);
	m_aRdoMCellFShape.Add(IDC_CMD_SECT_MCELL_REBAR2_RDO);
	m_aRdoMCellFShape.Add(IDC_CMD_SECT_MCELL_CLOSED_RDO);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoMCellFShape, 0);
}

void CCMSectItemStlGirder::InitUnit()
{
	m_edtTop.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTop.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtBot.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untBot.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtB1 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtB2 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtB3 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtB4 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtB5 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtB6 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtH  .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untH  .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtt1 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untt1 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtt2 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untt2 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edttw1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unttw1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edttw2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unttw2.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtID  .SetUnitType(D_UNITSYS_NONE);
	m_edtName.SetUnitType(D_UNITSYS_NONE);
	m_edtOffset.SetUnitType(D_UNITSYS_NONE);

	m_edtMCellB1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellB2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellB3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellB4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellB5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellB6.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellTf1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellTw1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddL1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddL2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddL3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddL4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddL5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddL6.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellBr1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellBr2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellBr3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellBr4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellBr5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellTf2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellTw2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddR1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddR2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddR3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddR4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddR5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMCellAddR6.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize6.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize7.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize8.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize9.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize10.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize11.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize12.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMCellSize13.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_edtMCellNoCell.SetUnitType(D_UNITSYS_NONE);
}

void CCMSectItemStlGirder::InitView()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_SECT_B_VIEW);
	ASSERT(pWnd);
	m_wndSecViewStlGirderBox.Init(pWnd);

	pWnd = GetDlgItem(IDC_CMD_SECT_I_VIEW);
	ASSERT(pWnd);
	m_wndSecViewStlGirderI.Init(pWnd);
}

void CCMSectItemStlGirder::InitImage()
{
	m_ImgViewer_B.SetImage(_T("SVG\\illustration\\Dialog\\SectStlGirder_Box.svg"));    // IDB_CMD_SEC_STL_GIRDER_BOX
	m_ImgViewer_I.SetImage(_T("SVG\\illustration\\Dialog\\SectStlGirder_I.svg"));      // IDB_CMD_SEC_STL_GIRDER_I
}

void CCMSectItemStlGirder::AlignControlDynamic()
{
	CRect rRef;
	CRect rMove;
	int nDistX, nDistY;

	CArray<UINT, UINT> aControls; 

	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);
	if(nCurType==D_SECT_TYPE_STLG_MCELL)
	{
		int nFShape;
		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoMCellFShape, nFShape);

		aControls.RemoveAll();
		aControls.Add(IDC_CMD_SECT_MCELL_H_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_TF1_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_TW1_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_1_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_2_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_3_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_4_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_5_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_6_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_TF2_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_TW2_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_1_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_2_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_3_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_4_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_5_EDT);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_6_EDT);
		aControls.Add( IDC_CMD_SECT_MCELL_H_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_TF1_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_TW1_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_1_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_2_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_3_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_4_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_5_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_6_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_TF2_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_TW2_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_1_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_2_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_3_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_4_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_5_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_6_STC);
		aControls.Add(IDC_CMD_SECT_MCELL_5_UNIT);
		aControls.Add(IDC_CMD_SECT_MCELL_6_UNIT);
		aControls.Add(IDC_CMD_SECT_MCELL_7_UNIT);
		aControls.Add(IDC_CMD_SECT_MCELL_8_UNIT);
		aControls.Add(IDC_CMD_SECT_MCELL_9_UNIT);
		aControls.Add(IDC_CMD_SECT_MCELL_10_UNIT);
		aControls.Add(IDC_CMD_SECT_MCELL_11_UNIT);
		aControls.Add(IDC_CMD_SECT_MCELL_12_UNIT);
		aControls.Add(IDC_CMD_SECT_MCELL_13_UNIT);

		if(nFShape == 3)
		{
			GetDlgItem(IDC_CMD_SECT_MCELL_B4_EDT)->GetWindowRect(rRef);
		}
		else
		{
			GetDlgItem(IDC_CMD_SECT_MCELL_B6_EDT)->GetWindowRect(rRef);
		}
		GetDlgItem(IDC_CMD_SECT_MCELL_H_EDT)->GetWindowRect(rMove);
		nDistX = rRef.left - rMove.left;
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(4);
		CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);

		aControls.RemoveAll();
		aControls.Add(IDC_CMD_MCELL_STIFFENER_BTN);
		if(nFShape == 0)
		{
			GetDlgItem(IDC_CMD_SECT_MCELL_TW1_EDT)->GetWindowRect(rRef);
		}
		else if(nFShape == 1)
		{
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_3_EDT)->GetWindowRect(rRef);
		}
		else if(nFShape == 2)
		{
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_6_EDT)->GetWindowRect(rRef);
		}
		else if(nFShape == 3)
		{
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_4_EDT)->GetWindowRect(rRef);
		}
		else ASSERT(0);    
		GetDlgItem(IDC_CMD_MCELL_STIFFENER_BTN)->GetWindowRect(rMove);    
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(6);
		CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);
	}
}

void CCMSectItemStlGirder::AlignControl()
{
	CRect rRef;
	CRect rMove;
	int nDistX, nDistY;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_CMD_SECT_I_GUIDE_BMP);
	
	GetDlgItem(IDC_CMD_SECT_B_GUIDE_BMP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SECT_I_GUIDE_BMP)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	nDistY = rRef.top  - rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);

	aControls.RemoveAll();
	aControls.Add(IDC_CMD_SECT_I_VIEW);	

	GetDlgItem(IDC_CMD_SECT_B_VIEW)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SECT_I_VIEW)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	nDistY = rRef.top  - rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);


	aControls.RemoveAll();
	aControls.Add(IDC_CMD_SECT_MCELL_B1_EDT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_B2_EDT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_B3_EDT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_B4_EDT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_B5_EDT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_B6_EDT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_H_EDT      );	
	aControls.Add(IDC_CMD_SECT_MCELL_TF1_EDT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_TW1_EDT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_1_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_2_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_3_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_4_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_5_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_6_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_BR1_EDT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_BR2_EDT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_BR3_EDT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_BR4_EDT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_BR5_EDT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_TF2_EDT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_TW2_EDT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_1_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_2_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_3_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_4_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_5_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_6_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_SIZE_GRP   );	
	aControls.Add(IDC_CMD_SECT_MCELL_B1_STC     );	
	aControls.Add(IDC_CMD_SECT_MCELL_B2_STC     );	
	aControls.Add(IDC_CMD_SECT_MCELL_B3_STC     );	
	aControls.Add(IDC_CMD_SECT_MCELL_B4_STC     );	
	aControls.Add(IDC_CMD_SECT_MCELL_B5_STC     );	
	aControls.Add(IDC_CMD_SECT_MCELL_B6_STC     );	
	aControls.Add(IDC_CMD_SECT_MCELL_H_STC      );	
	aControls.Add(IDC_CMD_SECT_MCELL_TF1_STC    );	
	aControls.Add(IDC_CMD_SECT_MCELL_TW1_STC    );	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_1_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_2_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_3_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_4_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_5_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_L_6_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_BR1_STC    );	
	aControls.Add(IDC_CMD_SECT_MCELL_BR2_STC    );	
	aControls.Add(IDC_CMD_SECT_MCELL_BR3_STC    );	
	aControls.Add(IDC_CMD_SECT_MCELL_BR4_STC    );	
	aControls.Add(IDC_CMD_SECT_MCELL_BR5_STC    );	
	aControls.Add(IDC_CMD_SECT_MCELL_TF2_STC    );	
	aControls.Add(IDC_CMD_SECT_MCELL_TW2_STC    );	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_1_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_2_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_3_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_4_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_5_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_ADD_R_6_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_1_UNIT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_2_UNIT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_3_UNIT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_4_UNIT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_5_UNIT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_6_UNIT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_7_UNIT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_8_UNIT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_9_UNIT     );	
	aControls.Add(IDC_CMD_SECT_MCELL_10_UNIT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_11_UNIT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_12_UNIT    );	
	aControls.Add(IDC_CMD_SECT_MCELL_13_UNIT    );	
	aControls.Add(IDC_CMD_MCELL_STIFFENER_BTN);	
	aControls.Add(IDC_CMD_SECT_MCELL_SYM_GRP);	
	aControls.Add(IDC_CMD_SECT_MCELL_SYM_CHK);	
	aControls.Add(IDC_CMD_SECT_MCELL_NO_CELL_STC);	
	aControls.Add(IDC_CMD_SECT_MCELL_NO_CELL_EDT);	
	aControls.Add(IDC_CMD_SECT_MCELL_NO_CELL_SPN);	
	aControls.Add(IDC_CMD_SECT_MCELL_LEFT_RDO);	
	aControls.Add(IDC_CMD_SECT_MCELL_RIGHT_RDO);	
	aControls.Add(IDC_CMD_SECT_MCELL_FSHAPE_GRP);	
	aControls.Add(IDC_CMD_SECT_MCELL_GEN_RDO);	
	aControls.Add(IDC_CMD_SECT_MCELL_REBAR1_RDO);	
	aControls.Add(IDC_CMD_SECT_MCELL_REBAR2_RDO);	
	aControls.Add(IDC_CMD_SECT_MCELL_CLOSED_RDO);	
	aControls.Add(IDC_CMD_SECT_MCELL_J1_CHK);	
	aControls.Add(IDC_CMD_SECT_MCELL_J2_CHK);	
	aControls.Add(IDC_CMD_SECT_MCELL_JR1_CHK);	
	aControls.Add(IDC_CMD_SECT_MCELL_JR2_CHK);	

	GetDlgItem(IDC_CMD_SECT_SIZE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SECT_MCELL_SIZE_GRP)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	nDistY = rRef.top  - rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);
}

void CCMSectItemStlGirder::ControlsShowHide()
{
	UpdateData(TRUE);

	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);

	int nFShape;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoMCellFShape, nFShape);

	// I, Box
	GetDlgItem(IDC_CMD_SECT_B_GUIDE_BMP       )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B);
	GetDlgItem(IDC_CMD_SECT_I_GUIDE_BMP       )->ShowWindow(nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_B_VIEW            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B);
	GetDlgItem(IDC_CMD_SECT_I_VIEW            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_SIZE_GRP          )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_SYM_AUTO_CALC_CHK )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_DIST_LEFT_GRP     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_TOP_STC           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_TOP_EDT           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_TOP_UNT           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_BOT_STC           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_BOT_EDT           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_BOT_UNT           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_B1_STC            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_B1_EDT            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_B2_STC            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_B2_EDT            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_B3_STC            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_B3_EDT            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_B4_STC            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_B4_EDT            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_B5_STC            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B);
	GetDlgItem(IDC_CMD_SECT_B5_EDT            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B);
	GetDlgItem(IDC_CMD_SECT_B6_STC            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B);
	GetDlgItem(IDC_CMD_SECT_B6_EDT            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B);
	GetDlgItem(IDC_CMD_SECT_H_STC             )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_H_EDT             )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_H_UNT             )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_T1_STC            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_T1_EDT            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_T1_UNT            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_T2_STC            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_T2_EDT            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_T2_UNT            )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_TW1_STC           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_TW1_EDT           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_TW1_UNT           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	GetDlgItem(IDC_CMD_SECT_TW2_STC           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B);
	GetDlgItem(IDC_CMD_SECT_TW2_EDT           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B);
	GetDlgItem(IDC_CMD_SECT_TW2_UNT           )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B);	
	GetDlgItem(IDC_CMD_STIFFENER_BTN          )->ShowWindow(nCurType==D_SECT_TYPE_STLG_B || nCurType==D_SECT_TYPE_STLG_I);
	
	//M-Cell
	GetDlgItem(IDC_CMD_SECT_MCELL_B1_EDT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_B2_EDT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_B3_EDT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_B4_EDT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_B5_EDT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 0 || nFShape == 1 || nFShape == 2));	
	GetDlgItem(IDC_CMD_SECT_MCELL_B6_EDT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 0 || nFShape == 1 || nFShape == 2));	
	GetDlgItem(IDC_CMD_SECT_MCELL_H_EDT       )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_TF1_EDT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_TW1_EDT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_1_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));	
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_2_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));	
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_3_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));	
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_4_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2 || nFShape == 3));	
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_5_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2));	
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_6_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2));	
	GetDlgItem(IDC_CMD_SECT_MCELL_BR1_EDT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_BR2_EDT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_BR3_EDT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_BR4_EDT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 0 || nFShape == 1 || nFShape == 2));	
	GetDlgItem(IDC_CMD_SECT_MCELL_BR5_EDT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 0 || nFShape == 1 || nFShape == 2));	
	GetDlgItem(IDC_CMD_SECT_MCELL_TF2_EDT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_TW2_EDT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_1_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_2_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_3_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_4_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_5_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_6_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2));		
	GetDlgItem(IDC_CMD_SECT_MCELL_SIZE_GRP    )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_B1_STC      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_B2_STC      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_B3_STC      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_B4_STC      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_B5_STC      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 0 || nFShape == 1 || nFShape == 2));	
	GetDlgItem(IDC_CMD_SECT_MCELL_B6_STC      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 0 || nFShape == 1 || nFShape == 2));	
	GetDlgItem(IDC_CMD_SECT_MCELL_H_STC       )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_TF1_STC     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_TW1_STC     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_1_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_2_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_3_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_4_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_5_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_6_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2));		
	GetDlgItem(IDC_CMD_SECT_MCELL_BR1_STC     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_BR2_STC     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_BR3_STC     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_BR4_STC     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 0 || nFShape == 1 || nFShape == 2));	
	GetDlgItem(IDC_CMD_SECT_MCELL_BR5_STC     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 0 || nFShape == 1 || nFShape == 2));	
	GetDlgItem(IDC_CMD_SECT_MCELL_TF2_STC     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_TW2_STC     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_1_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_2_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_3_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_4_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_5_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2));		
	GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_6_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2));		
	GetDlgItem(IDC_CMD_SECT_MCELL_1_UNIT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_2_UNIT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_3_UNIT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 0 || nFShape == 1 || nFShape == 2));			
	GetDlgItem(IDC_CMD_SECT_MCELL_4_UNIT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 0 || nFShape == 1 || nFShape == 2));			
	GetDlgItem(IDC_CMD_SECT_MCELL_5_UNIT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_6_UNIT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_7_UNIT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);		
	GetDlgItem(IDC_CMD_SECT_MCELL_8_UNIT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_9_UNIT      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_10_UNIT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 1 || nFShape == 2 || nFShape == 3));			
	GetDlgItem(IDC_CMD_SECT_MCELL_11_UNIT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2 || nFShape == 3));		
	GetDlgItem(IDC_CMD_SECT_MCELL_12_UNIT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2));		
	GetDlgItem(IDC_CMD_SECT_MCELL_13_UNIT     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL && (nFShape == 2));		
	GetDlgItem(IDC_CMD_MCELL_STIFFENER_BTN    )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);
	GetDlgItem(IDC_CMD_SECT_MCELL_SYM_GRP     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_SYM_CHK     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_NO_CELL_STC )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_NO_CELL_EDT )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_NO_CELL_SPN )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_LEFT_RDO    )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_RIGHT_RDO   )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_FSHAPE_GRP  )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_GEN_RDO     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_REBAR1_RDO  )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_REBAR2_RDO  )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_CLOSED_RDO  )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_J1_CHK      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_J2_CHK      )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_JR1_CHK     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);	
	GetDlgItem(IDC_CMD_SECT_MCELL_JR2_CHK     )->ShowWindow(nCurType==D_SECT_TYPE_STLG_MCELL);

	GetDlgItem(IDC_CMD_RIVET_ANGLE_BTN)->ShowWindow(nCurType == D_SECT_TYPE_STLG_I);
	
	BOOL b7thDOF = CSectDB::IsEnable7thDOF();
	GetDlgItem(IDC_CMD_WARPING_EFFECT_CHK)->ShowWindow(b7thDOF);  
}

void CCMSectItemStlGirder::CreateOrDestoryView()
{
	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);

	if(nCurType == D_SECT_TYPE_STLG_MCELL)
	{
		CreateSTLGView();
		m_pStlgViewer->ChangeBitmap();
	}
	else
	{
		DestroyView();
	}
}

void CCMSectItemStlGirder::EnableDisableControls()
{
	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);

	if(nCurType == D_SECT_TYPE_STLG_B || nCurType == D_SECT_TYPE_STLG_I)
	{
		GetDlgItem(IDC_CMD_SECT_TOP_EDT)->EnableWindow(!m_bSymAutoCalc);
		GetDlgItem(IDC_CMD_SECT_BOT_EDT)->EnableWindow(!m_bSymAutoCalc);

		GetDlgItem(IDC_CMD_SECT_B2_EDT)->EnableWindow ( nCurType == D_SECT_TYPE_STLG_B                     || (nCurType == D_SECT_TYPE_STLG_I && !m_bSymAutoCalc));
		GetDlgItem(IDC_CMD_SECT_B3_EDT)->EnableWindow ((nCurType == D_SECT_TYPE_STLG_B && !m_bSymAutoCalc) ||  nCurType == D_SECT_TYPE_STLG_I);
		GetDlgItem(IDC_CMD_SECT_B4_EDT)->EnableWindow ( nCurType == D_SECT_TYPE_STLG_B                     || (nCurType == D_SECT_TYPE_STLG_I && !m_bSymAutoCalc));
		GetDlgItem(IDC_CMD_SECT_B6_EDT)->EnableWindow ((nCurType == D_SECT_TYPE_STLG_B && !m_bSymAutoCalc) ||  nCurType == D_SECT_TYPE_STLG_I);
		GetDlgItem(IDC_CMD_SECT_TW2_EDT)->EnableWindow((nCurType == D_SECT_TYPE_STLG_B && !m_bSymAutoCalc) ||  nCurType == D_SECT_TYPE_STLG_I);
	}
	else if(nCurType == D_SECT_TYPE_STLG_MCELL)
	{
		int nNoOfCell = m_edtMCellNoCell.GetEditValue();    
		BOOL bSym = m_chkMCellSym.GetCheck();
		int nFShape;
		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoMCellFShape, nFShape);

		int nLorR;
		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoMCellLorR, nLorR);
		
		
		GetDlgItem(IDC_CMD_SECT_MCELL_SYM_CHK)->EnableWindow(nNoOfCell != 0);    
		GetDlgItem(IDC_CMD_SECT_MCELL_LEFT_RDO)->EnableWindow(nNoOfCell == 0);
		GetDlgItem(IDC_CMD_SECT_MCELL_RIGHT_RDO)->EnableWindow(nNoOfCell == 0);    

		GetDlgItem(IDC_CMD_SECT_MCELL_RIGHT_RDO)->EnableWindow(nNoOfCell == 0);
		GetDlgItem(IDC_CMD_SECT_MCELL_RIGHT_RDO)->EnableWindow(nNoOfCell == 0);
		GetDlgItem(IDC_CMD_SECT_MCELL_RIGHT_RDO)->EnableWindow(nNoOfCell == 0);
		GetDlgItem(IDC_CMD_SECT_MCELL_RIGHT_RDO)->EnableWindow(nNoOfCell == 0);

		//GetDlgItem(IDC_CMD_SECT_MCELL_J1_CHK)->EnableWindow(nFShape == 3 && ((nNoOfCell == 0 && nLorR == 0) || nNoOfCell != 0));
		GetDlgItem(IDC_CMD_SECT_MCELL_J1_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SECT_MCELL_J2_CHK)->EnableWindow(nFShape == 3 && ((nNoOfCell == 0 && nLorR == 0) || nNoOfCell != 0));
//    GetDlgItem(IDC_CMD_SECT_MCELL_JR1_CHK)->EnableWindow(nFShape == 3 && ((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell != 0 && !bSym)));    
		GetDlgItem(IDC_CMD_SECT_MCELL_JR1_CHK)->EnableWindow(FALSE);    
		GetDlgItem(IDC_CMD_SECT_MCELL_JR2_CHK)->EnableWindow(nFShape == 3 && ((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell != 0 && !bSym)));

		BOOL bJ1, bJ2, bJr1, bJr2;
		bJ1  = m_chkMCellJ2.IsWindowEnabled();
		bJ2  = m_chkMCellJ2.GetCheck()  && m_chkMCellJ2.IsWindowEnabled();
		bJr1 = m_chkMCellJr2.IsWindowEnabled();
		bJr2 = m_chkMCellJr2.GetCheck() && m_chkMCellJr2.IsWindowEnabled();

		// Size Edt
		GetDlgItem(IDC_CMD_SECT_MCELL_B1_EDT     )->EnableWindow((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0);    
		GetDlgItem(IDC_CMD_SECT_MCELL_B2_EDT     )->EnableWindow((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0);    
		GetDlgItem(IDC_CMD_SECT_MCELL_B3_EDT     )->EnableWindow(nFShape != 3 && ((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0));
		GetDlgItem(IDC_CMD_SECT_MCELL_B4_EDT     )->EnableWindow((nFShape != 3 && ((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) ) ||
																														 (nFShape == 3 && nNoOfCell > 2));
		GetDlgItem(IDC_CMD_SECT_MCELL_B5_EDT     )->EnableWindow(nFShape !=3 && nNoOfCell == 0 && nLorR == 0);
		GetDlgItem(IDC_CMD_SECT_MCELL_B6_EDT     )->EnableWindow(nNoOfCell > 2);    
		GetDlgItem(IDC_CMD_SECT_MCELL_BR1_EDT    )->EnableWindow((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym));    
		GetDlgItem(IDC_CMD_SECT_MCELL_BR2_EDT    )->EnableWindow((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym));    
		GetDlgItem(IDC_CMD_SECT_MCELL_BR3_EDT    )->EnableWindow(nFShape != 3 && ((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)));    
		GetDlgItem(IDC_CMD_SECT_MCELL_BR4_EDT    )->EnableWindow((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)); 
		GetDlgItem(IDC_CMD_SECT_MCELL_BR5_EDT    )->EnableWindow(nFShape !=3 && nNoOfCell == 0 && nLorR == 1);
		GetDlgItem(IDC_CMD_SECT_MCELL_TW2_EDT    )->EnableWindow(nNoOfCell > 1);
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_1_EDT)->EnableWindow(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && (nFShape != 3 || (nFShape == 3 && bJ1)));
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_2_EDT)->EnableWindow(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && (nFShape != 3 || (nFShape == 3 && bJ1)));
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_3_EDT)->EnableWindow(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && (nFShape != 3 || (nFShape == 3 && bJ2)));
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_4_EDT)->EnableWindow(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && (nFShape != 3 || (nFShape == 3 && bJ2)));
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_5_EDT)->EnableWindow((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0);    
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_6_EDT)->EnableWindow((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0);
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_1_EDT)->EnableWindow(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)) && (nFShape != 3 || (nFShape == 3 && bJr1)));      
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_2_EDT)->EnableWindow(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)) && (nFShape != 3 || (nFShape == 3 && bJr1)));      
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_3_EDT)->EnableWindow(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)) && (nFShape != 3 || (nFShape == 3 && bJr2)));      
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_4_EDT)->EnableWindow(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)) && (nFShape != 3 || (nFShape == 3 && bJr2)));      
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_5_EDT)->EnableWindow((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym));      
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_6_EDT)->EnableWindow((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym));    
		
		// Size Static
		GetDlgItem(IDC_CMD_SECT_MCELL_B1_STC     )->EnableWindow((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0);    
		GetDlgItem(IDC_CMD_SECT_MCELL_B2_STC     )->EnableWindow((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0);    
		GetDlgItem(IDC_CMD_SECT_MCELL_B3_STC     )->EnableWindow(nFShape != 3 && ((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0));
		GetDlgItem(IDC_CMD_SECT_MCELL_B4_STC     )->EnableWindow((nFShape != 3 && ((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) ) ||
																														(nFShape == 3 && nNoOfCell > 2));
		GetDlgItem(IDC_CMD_SECT_MCELL_B5_STC     )->EnableWindow(nFShape !=3 && nNoOfCell == 0 && nLorR == 0);
		GetDlgItem(IDC_CMD_SECT_MCELL_B6_STC     )->EnableWindow(nNoOfCell > 2);    
		GetDlgItem(IDC_CMD_SECT_MCELL_BR1_STC    )->EnableWindow((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym));    
		GetDlgItem(IDC_CMD_SECT_MCELL_BR2_STC    )->EnableWindow((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym));    
		GetDlgItem(IDC_CMD_SECT_MCELL_BR3_STC    )->EnableWindow(nFShape != 3 && ((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)));    
		GetDlgItem(IDC_CMD_SECT_MCELL_BR4_STC    )->EnableWindow((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)); 
		GetDlgItem(IDC_CMD_SECT_MCELL_BR5_STC    )->EnableWindow(nFShape !=3 && nNoOfCell == 0 && nLorR == 1);
		GetDlgItem(IDC_CMD_SECT_MCELL_TW2_STC    )->EnableWindow(nNoOfCell > 1);
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_1_STC)->EnableWindow(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && (nFShape != 3 || (nFShape == 3 && bJ1)));
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_2_STC)->EnableWindow(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && (nFShape != 3 || (nFShape == 3 && bJ1)));
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_3_STC)->EnableWindow(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && (nFShape != 3 || (nFShape == 3 && bJ2)));
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_4_STC)->EnableWindow(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && (nFShape != 3 || (nFShape == 3 && bJ2)));
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_5_STC)->EnableWindow((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0);    
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_6_STC)->EnableWindow((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0);
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_1_STC)->EnableWindow(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)) && (nFShape != 3 || (nFShape == 3 && bJr1)));  
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_2_STC)->EnableWindow(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)) && (nFShape != 3 || (nFShape == 3 && bJr1)));  
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_3_STC)->EnableWindow(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)) && (nFShape != 3 || (nFShape == 3 && bJr2)));  
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_4_STC)->EnableWindow(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym)) && (nFShape != 3 || (nFShape == 3 && bJr2)));  
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_5_STC)->EnableWindow((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym));      
		GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_6_STC)->EnableWindow((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !bSym));    

		RedrawWindow();
	}
}

void CCMSectItemStlGirder::ChangeText()
{
	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);

	if(nCurType == D_SECT_TYPE_STLG_B)
	{
		GetDlgItem(IDC_CMD_SECT_TW1_STC)->SetWindowText(_T("tw1"));
	}
	else if(nCurType == D_SECT_TYPE_STLG_I)
	{
		GetDlgItem(IDC_CMD_SECT_TW1_STC)->SetWindowText(_T("tw"));
	}
	else if(nCurType == D_SECT_TYPE_STLG_MCELL)
	{
		int nFShape;
		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoMCellFShape, nFShape);
		if(nFShape == 0)
		{
			// 아무거도 안함
		}
		else if(nFShape == 1)
		{
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_1_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RH));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_2_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RT));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_3_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RB));

			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_1_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RHR));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_2_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RTR));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_3_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RBR));   
		}
		else if(nFShape == 2)
		{
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_1_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RH1));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_2_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RH2));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_3_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RH3));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_4_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RB));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_5_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RT1));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_6_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RT2));

			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_1_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RHR1));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_2_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RHR2));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_3_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RHR3));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_4_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RBR));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_5_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RTR1));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_6_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_RTR2));
		}
		else if(nFShape == 3)
		{
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_1_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_JH1));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_2_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_JB1));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_3_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_JH2));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_L_4_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_JB2));

			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_1_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_JHR1));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_2_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_JBR1));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_3_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_JHR2));
			GetDlgItem(IDC_CMD_SECT_MCELL_ADD_R_4_STC)->SetWindowText(_LS(IDS_CMD_STLG_MCELL_JBR2));
		}
		else ASSERT(0);


	} 
	else
	{
		ASSERT(0);
	}
}

BOOL CCMSectItemStlGirder::SaveData()
{
	if (!Dlg2Data()) return FALSE;
	if (m_Data.SName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_section_name_));
		return FALSE;
	}

	CSectUtil::StlGSymmetricSectAutoCalc(m_Data, FALSE);

	if (!GetPeriAndStiff()) return FALSE;

	BOOL bReturn = FALSE;
	
	if (m_bModify)
	{
		if (m_bDesign) bReturn = m_pDoc->m_pDataCtrl->AddSecd(m_Key, m_Data, m_bVirtual);
		else           bReturn = m_pDoc->m_pDataCtrl->ModifySect(m_OrgKey, m_Key, m_Data);
	}
	else 
		bReturn = m_pDoc->m_pDataCtrl->AddSect(m_Key, m_Data);

	return bReturn;
}

BOOL CCMSectItemStlGirder::Dlg2Data()
{
	// Offset Data Backup
	int nOffsetPoint   = m_Data.SectBefore.nOffsetPoint;
	int nOffsetCenter  = m_Data.SectBefore.nOffsetCenter;
	int nUserOffsetRef = m_Data.SectBefore.nUserOffsetRef;
	int nHorzOffsetOpt = m_Data.SectBefore.nHorzOffsetOpt;
	int nVertOffsetOpt = m_Data.SectBefore.nVertOffsetOpt;
	double dUserDefOffsetY[2], dUserDefOffsetZ[2];
	for (int i=0; i<2; i++)
	{
		dUserDefOffsetY[i] = m_Data.SectBefore.dUserDefOffsetY[i];
		dUserDefOffsetZ[i] = m_Data.SectBefore.dUserDefOffsetZ[i];
	}
	
	///// 
	m_Data.Initialize();
	
	// ID
	CString csID;
	m_edtID.GetWindowText(csID);
	m_Key = (T_SECT_K)_tstol(csID);
		
	// Size
	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);
	m_Data.nStype = nCurType;
	if(nCurType == D_SECT_TYPE_STLG_B)
	{
		Dlg2Data_Box();
		m_Data = m_DataBox;
	}
	else if(nCurType == D_SECT_TYPE_STLG_I)
	{
		Dlg2Data_I();
		m_Data = m_DataI;
	}
	else if(nCurType == D_SECT_TYPE_STLG_MCELL)
	{
		Dlg2Data_MCell();
		m_Data = m_DataMCell;
	}
	else
	{
		ASSERT(0);
	}

	// Offset
	m_Data.SectBefore.nOffsetPoint   = nOffsetPoint;
	m_Data.SectBefore.nOffsetCenter  = nOffsetCenter;
	m_Data.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
	m_Data.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
	m_Data.SectBefore.nUserOffsetRef = nUserOffsetRef;
	for (int i=0; i<2; i++)
	{
		m_Data.SectBefore.dUserDefOffsetY[i] = dUserDefOffsetY[i];
		m_Data.SectBefore.dUserDefOffsetZ[i] = dUserDefOffsetZ[i];
	}
	
	return TRUE;
}

void CCMSectItemStlGirder::ShowKey2Dlg()
{
	if (m_edtID.GetSafeHwnd() == nullptr) return;

	CString strID;
	strID.Format(_T("%d"), m_Key);
	m_edtID.SetWindowText(strID);
}

void CCMSectItemStlGirder::GetSizeEdtArByCurState(std::vector<CEditUnit*>& aPtSizeEdt)
{
	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);

	aPtSizeEdt.clear();

	switch (nCurType)
	{
	case D_SECT_TYPE_STLG_B: 
		{
			aPtSizeEdt.push_back(&m_edtB1);    // [0]
			aPtSizeEdt.push_back(&m_edtB2);    // [1]
			aPtSizeEdt.push_back(&m_edtB3);    // [2]
			aPtSizeEdt.push_back(&m_edtB4);    // [3]
			aPtSizeEdt.push_back(&m_edtB5);    // [4]
			aPtSizeEdt.push_back(&m_edtB6);    // [5]
			aPtSizeEdt.push_back(&m_edtH );    // [6]
			aPtSizeEdt.push_back(&m_edtt1);    // [7]
			aPtSizeEdt.push_back(&m_edtt2);    // [8]
			aPtSizeEdt.push_back(&m_edttw1);   // [9]
			aPtSizeEdt.push_back(&m_edttw2);   // [10]
		}
		break;
	case D_SECT_TYPE_STLG_I: 
		{
			aPtSizeEdt.push_back(&m_edtB1);    // [0]
			aPtSizeEdt.push_back(&m_edtB2);    // [1]
			aPtSizeEdt.push_back(&m_edtB3);    // [2]
			aPtSizeEdt.push_back(&m_edtB4);    // [3]
			aPtSizeEdt.push_back(&m_edtH);     // [4]
			aPtSizeEdt.push_back(&m_edtt1);    // [5]
			aPtSizeEdt.push_back(&m_edtt2);    // [6]
			aPtSizeEdt.push_back(&m_edttw1);   // [7]
		}                               
		break;                          
	case D_SECT_TYPE_STLG_MCELL:      
		{
			int nFShape;
			CDlgUtil::CtrlRadioGetCheck(this, m_aRdoMCellFShape, nFShape);

			if(nFShape == 0)
			{
				aPtSizeEdt.push_back(&m_edtMCellB1   );   // [0]
				aPtSizeEdt.push_back(&m_edtMCellB2   );   // [1]
				aPtSizeEdt.push_back(&m_edtMCellB3   );   // [2]
				aPtSizeEdt.push_back(&m_edtMCellB4   );   // [3]
				aPtSizeEdt.push_back(&m_edtMCellB5   );   // [4]
				aPtSizeEdt.push_back(&m_edtMCellB6   );   // [5]
				aPtSizeEdt.push_back(&m_edtMCellBr1  );   // [6]
				aPtSizeEdt.push_back(&m_edtMCellBr2  );   // [7]
				aPtSizeEdt.push_back(&m_edtMCellBr3  );   // [8]
				aPtSizeEdt.push_back(&m_edtMCellBr4  );   // [9]
				aPtSizeEdt.push_back(&m_edtMCellBr5  );   // [10]
				aPtSizeEdt.push_back(&m_edtMCellH    );   // [11]
				aPtSizeEdt.push_back(&m_edtMCellTf1  );   // [12]
				aPtSizeEdt.push_back(&m_edtMCellTw1  );   // [13]     
				aPtSizeEdt.push_back(&m_edtMCellTf2  );   // [14]
				aPtSizeEdt.push_back(&m_edtMCellTw2  );   // [15]       
			}
			else if(nFShape == 1)
			{
				aPtSizeEdt.push_back(&m_edtMCellB1   );            // [0]
				aPtSizeEdt.push_back(&m_edtMCellB2   );            // [1]
				aPtSizeEdt.push_back(&m_edtMCellB3   );            // [2]
				aPtSizeEdt.push_back(&m_edtMCellB4   );            // [3]
				aPtSizeEdt.push_back(&m_edtMCellB5   );            // [4]
				aPtSizeEdt.push_back(&m_edtMCellB6   );            // [5]
				aPtSizeEdt.push_back(&m_edtMCellBr1  );            // [6]
				aPtSizeEdt.push_back(&m_edtMCellBr2  );            // [7]
				aPtSizeEdt.push_back(&m_edtMCellBr3  );            // [8]
				aPtSizeEdt.push_back(&m_edtMCellBr4  );            // [9]
				aPtSizeEdt.push_back(&m_edtMCellBr5  );            // [10]
				aPtSizeEdt.push_back(&m_edtMCellH    );            // [11]
				aPtSizeEdt.push_back(&m_edtMCellTf1  );            // [12]
				aPtSizeEdt.push_back(&m_edtMCellTw1  );            // [13]
				aPtSizeEdt.push_back(&m_edtMCellTf2  );            // [14]
				aPtSizeEdt.push_back(&m_edtMCellTw2  );            // [15]
				aPtSizeEdt.push_back(&m_edtMCellAddL1);  // rh     // [16]
				aPtSizeEdt.push_back(&m_edtMCellAddL2);  // rt     // [17]
				aPtSizeEdt.push_back(&m_edtMCellAddL3);  // rb     // [18]
				aPtSizeEdt.push_back(&m_edtMCellAddR1);  // rhr    // [19]
				aPtSizeEdt.push_back(&m_edtMCellAddR2);  // rtr    // [20]
				aPtSizeEdt.push_back(&m_edtMCellAddR3);  // rbr    // [21]
			}
			else if(nFShape == 2)
			{
				aPtSizeEdt.push_back(&m_edtMCellB1   );            // [0]
				aPtSizeEdt.push_back(&m_edtMCellB2   );            // [1]
				aPtSizeEdt.push_back(&m_edtMCellB3   );            // [2]
				aPtSizeEdt.push_back(&m_edtMCellB4   );            // [3]
				aPtSizeEdt.push_back(&m_edtMCellB5   );            // [4]
				aPtSizeEdt.push_back(&m_edtMCellB6   );            // [5]
				aPtSizeEdt.push_back(&m_edtMCellBr1  );            // [6]
				aPtSizeEdt.push_back(&m_edtMCellBr2  );            // [7]
				aPtSizeEdt.push_back(&m_edtMCellBr3  );            // [8]
				aPtSizeEdt.push_back(&m_edtMCellBr4  );            // [9]
				aPtSizeEdt.push_back(&m_edtMCellBr5  );            // [10]
				aPtSizeEdt.push_back(&m_edtMCellH    );            // [11]
				aPtSizeEdt.push_back(&m_edtMCellTf1  );            // [12]
				aPtSizeEdt.push_back(&m_edtMCellTw1  );            // [13]
				aPtSizeEdt.push_back(&m_edtMCellTf2  );            // [14]
				aPtSizeEdt.push_back(&m_edtMCellTw2  );            // [15]
				aPtSizeEdt.push_back(&m_edtMCellAddL1);  // rh1    // [16]
				aPtSizeEdt.push_back(&m_edtMCellAddL2);  // rh2    // [17]
				aPtSizeEdt.push_back(&m_edtMCellAddL3);  // rh3    // [18]
				aPtSizeEdt.push_back(&m_edtMCellAddL4);  // rb     // [19]
				aPtSizeEdt.push_back(&m_edtMCellAddL5);  // rt1    // [20]
				aPtSizeEdt.push_back(&m_edtMCellAddL6);  // rt2    // [21]
				aPtSizeEdt.push_back(&m_edtMCellAddR1);  // rhr1   // [22]
				aPtSizeEdt.push_back(&m_edtMCellAddR2);  // rhr2   // [23]
				aPtSizeEdt.push_back(&m_edtMCellAddR3);  // rhr3   // [24]
				aPtSizeEdt.push_back(&m_edtMCellAddR4);  // rbr    // [25]
				aPtSizeEdt.push_back(&m_edtMCellAddR5);  // rtr1   // [26]
				aPtSizeEdt.push_back(&m_edtMCellAddR6);  // rtr2   // [27]
			}
			else if(nFShape == 3)
			{
				aPtSizeEdt.push_back(&m_edtMCellB1   );            // [0]
				aPtSizeEdt.push_back(&m_edtMCellB2   );            // [1]
				aPtSizeEdt.push_back(&m_edtMCellB3   );            // [2]
				aPtSizeEdt.push_back(&m_edtMCellB4   );            // [3]
				aPtSizeEdt.push_back(&m_edtMCellBr1  );            // [4]
				aPtSizeEdt.push_back(&m_edtMCellBr2  );            // [5]
				aPtSizeEdt.push_back(&m_edtMCellBr3  );            // [6]
				aPtSizeEdt.push_back(&m_edtMCellH    );            // [7]
				aPtSizeEdt.push_back(&m_edtMCellTf1  );            // [8]
				aPtSizeEdt.push_back(&m_edtMCellTw1  );            // [9]
				aPtSizeEdt.push_back(&m_edtMCellTf2  );            // [10]
				aPtSizeEdt.push_back(&m_edtMCellTw2  );            // [11]
				aPtSizeEdt.push_back(&m_edtMCellAddL1); // jh1     // [12]
				aPtSizeEdt.push_back(&m_edtMCellAddL2); // jb1     // [13]
				aPtSizeEdt.push_back(&m_edtMCellAddL3); // jh2     // [14]
				aPtSizeEdt.push_back(&m_edtMCellAddL4); // jb2     // [15]
				aPtSizeEdt.push_back(&m_edtMCellAddR1); // jhr1    // [16]
				aPtSizeEdt.push_back(&m_edtMCellAddR2); // jbr1    // [17]
				aPtSizeEdt.push_back(&m_edtMCellAddR3); // jhr2    // [18]
				aPtSizeEdt.push_back(&m_edtMCellAddR4); // jbr2    // [19]
			}                                            
			else ASSERT(0);                               
		}
		break;
	default:
		ASSERT(0);
		break;
	}
}

BOOL CCMSectItemStlGirder::Dlg2Data_Box()
{
	UpdateData(TRUE);

	m_DataBox.SectBefore.Shape = D_SECT_SHAPE_STLG_B;
	
	// Name
	m_edtName.GetWindowText(m_DataBox.SName);
	m_DataBox.SName.TrimLeft(); m_DataBox.SName.TrimRight();

	// Symmetric Section Auto Calculation
	m_DataBox.SectBefore.SectI.bSymAutoCalc = m_bSymAutoCalc;

	// Consider Shear Deformation
	m_DataBox.SectBefore.bConsiderShearDeform = m_bShearDeform;
	m_DataBox.SectBefore.bConsiderWarpingEffect = m_bWarpingEffect;
	 
//  if(bTypeOnly) return TRUE;

	m_DataBox.SectAfter.SectI.Size[0] = m_edtTop.GetEditValue();
	m_DataBox.SectAfter.SectI.Size[1] = m_edtBot.GetEditValue();

	std::vector<CEditUnit*> aPtSizeEdt;
	GetSizeEdtArByCurState(aPtSizeEdt);
	for (int i = 0; i < aPtSizeEdt.size(); i++)
	{
		m_DataBox.SectBefore.SectI.Size[i]  = aPtSizeEdt[i]->GetEditValue();
	}

	return TRUE;
}

BOOL CCMSectItemStlGirder::Dlg2Data_I()
{
	UpdateData(TRUE);
	
	m_DataI.SectBefore.Shape = D_SECT_SHAPE_STLG_I;

	//if(bTypeOnly) return TRUE;

	// Name
	m_edtName.GetWindowText(m_DataI.SName);
	m_DataI.SName.TrimLeft(); m_DataI.SName.TrimRight();

	// Symmetric Section Auto Calculation
	m_DataI.SectBefore.SectI.bSymAutoCalc = m_bSymAutoCalc;

	// Consider Shear Deformation
	m_DataI.SectBefore.bConsiderShearDeform = m_bShearDeform;
	m_DataI.SectBefore.bConsiderWarpingEffect = m_bWarpingEffect;

	m_DataI.SectAfter.SectI.Size[0] = m_edtTop.GetEditValue();
	m_DataI.SectAfter.SectI.Size[1] = m_edtBot.GetEditValue();
	
	std::vector<CEditUnit*> aPtSizeEdt;
	GetSizeEdtArByCurState(aPtSizeEdt);
	for (int i = 0; i < aPtSizeEdt.size(); i++)
	{
		m_DataI.SectBefore.SectI.Size[i]  = aPtSizeEdt[i]->GetEditValue();
	}

	return TRUE;
}

BOOL CCMSectItemStlGirder::Dlg2Data_MCell()
{
	UpdateData(TRUE);

	m_DataMCell.SectBefore.Shape = D_SECT_SHAPE_STLG_MCELL;

	// Name
	m_edtName.GetWindowText(m_DataMCell.SName);
	m_DataMCell.SName.TrimLeft(); m_DataMCell.SName.TrimRight();

	// Consider Shear Deformation
	m_DataMCell.SectBefore.bConsiderShearDeform = m_bShearDeform;
	m_DataMCell.SectBefore.bConsiderWarpingEffect = m_bWarpingEffect;

	m_DataMCell.SectBefore.bSymmetric = m_chkMCellSym.GetCheck();

	m_DataMCell.SectBefore.nCellNum = (int)m_edtMCellNoCell.GetEditValue();
	CDlgUtil::CtrlRadioGetCheck(this,  m_aRdoMCellLorR, m_DataMCell.SectBefore.nCellType);
	CDlgUtil::CtrlRadioGetCheck(this,  m_aRdoMCellFShape, m_DataMCell.SectBefore.nFlangeShape);

	m_DataMCell.SectBefore.nJoint = 0;
	m_DataMCell.SectBefore.nJoint |= 0x00000001;
	if(m_chkMCellJ2.GetCheck())  m_DataMCell.SectBefore.nJoint |= 0x00000002;
	m_DataMCell.SectBefore.nJoint |= 0x00000004;
	if(m_chkMCellJr2.GetCheck()) m_DataMCell.SectBefore.nJoint |= 0x00000008;

	//if(bTypeOnly) return TRUE;

	std::vector<CEditUnit*> aPtSizeEdt;
	GetSizeEdtArByCurState(aPtSizeEdt);
	for (int i = 0; i < aPtSizeEdt.size(); i++)
	{
		m_DataMCell.SectBefore.SectI.Size[i]  = aPtSizeEdt[i]->GetEditValue();
	}

	return TRUE;
}

BOOL CCMSectItemStlGirder::GetPeriAndStiff()
{
	return CSectUtil::CalcSectData(m_Data);
}

void CCMSectItemStlGirder::Data2Dlg(BOOL bAfterSymmetric)
{
	if(bAfterSymmetric)
	{
		ASSERT(m_Data.nStype == D_SECT_TYPE_STLG_MCELL); // I Box 는 SymmetricSectAutoCalc() 함수에서 필요한 ctrl만 data2dlg를 해줌.. MCell 처럼 복잡한 경우만 bAfterSymmetric 를 TRUE 로 이 함수 부르도록 하자.
	}

	ShowKey2Dlg();
	m_edtName.SetWindowText(m_Data.SName);

	// Symmetric Section Auto Calculation
	m_bSymAutoCalc = m_Data.SectBefore.SectI.bSymAutoCalc;
	
	CDlgUtil::CobxSetCurSelItemData(m_cmbSectType, m_Data.nStype);
	m_nLastType = m_Data.nStype;

	if(m_Data.nStype == D_SECT_TYPE_STLG_B)
	{
		ASSERT(!bAfterSymmetric); // I,Box의 Symmetric 계산 후 Data2Dlg를 하는 코드는 SymmetricSectAutoCalc() 에 있음
		m_DataBox = m_Data;
		Data2Dlg_Box();
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_STLG_I)
	{
		ASSERT(!bAfterSymmetric); // I,Box의 Symmetric 계산 후 Data2Dlg를 하는 코드는 SymmetricSectAutoCalc() 에 있음
		m_DataI = m_Data;
		Data2Dlg_I();
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(m_Data.nStype == D_SECT_TYPE_STLG_MCELL)
	{
		if(!bAfterSymmetric)
		{
			m_DataMCell = m_Data;
		}
		Data2Dlg_MCell(bAfterSymmetric); // Box,I 는 
		DrawViewDlgSection();
	}
	else ASSERT(0);
	
	UpdateData(FALSE);
}

void CCMSectItemStlGirder::Data2Dlg_Box()
{
	m_edtTop.SetEditUnit(m_DataBox.SectAfter.SectI.Size[0]);
	m_edtBot.SetEditUnit(m_DataBox.SectAfter.SectI.Size[1]);

	std::vector<CEditUnit*> aPtSizeEdt;
	GetSizeEdtArByCurState(aPtSizeEdt);
	for (int i = 0; i < aPtSizeEdt.size(); i++)
	{
		aPtSizeEdt[i]->SetEditUnit(m_DataBox.SectBefore.SectI.Size[i]);
	}

	// Set Offset Point
	if (m_DataBox.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataBox.SectBefore.nOffsetPoint-1];
		m_edtOffset.SetWindowText(csOffset);
	}

	// Consider Shear Deformation
	m_bShearDeform = m_DataBox.SectBefore.bConsiderShearDeform;
	m_bWarpingEffect = m_DataBox.SectBefore.bConsiderWarpingEffect;

	UpdateData(FALSE);
}

void CCMSectItemStlGirder::Data2Dlg_I()
{
	m_edtTop.SetEditUnit(m_DataI.SectAfter.SectI.Size[0]);
	m_edtBot.SetEditUnit(m_DataI.SectAfter.SectI.Size[1]);
	
	std::vector<CEditUnit*> aPtSizeEdt;
	GetSizeEdtArByCurState(aPtSizeEdt);
	for (int i = 0; i < aPtSizeEdt.size(); i++)
	{
		aPtSizeEdt[i]->SetEditUnit(m_DataI.SectBefore.SectI.Size[i]);
	}
	
	// Set Offset Point
	if (m_DataI.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataI.SectBefore.nOffsetPoint-1];
		m_edtOffset.SetWindowText(csOffset);
	}

	// Consider Shear Deformation
	m_bShearDeform = m_DataI.SectBefore.bConsiderShearDeform;
	m_bWarpingEffect = m_DataI.SectBefore.bConsiderWarpingEffect;
	
	UpdateData(FALSE);
}

void CCMSectItemStlGirder::Data2Dlg_MCell(BOOL bAfterSymmetric)
{
	if(!bAfterSymmetric)
	{
		m_chkMCellSym.SetCheck(m_DataMCell.SectBefore.bSymmetric);
		m_edtMCellNoCell.SetEditUnit(m_DataMCell.SectBefore.nCellNum);
		CDlgUtil::CtrlRadioSetCheck(this,  m_aRdoMCellLorR, m_DataMCell.SectBefore.nCellType);
		CDlgUtil::CtrlRadioSetCheck(this,  m_aRdoMCellFShape, m_DataMCell.SectBefore.nFlangeShape);
	}  

	m_chkMCellJ1.SetCheck(TRUE);
	m_chkMCellJ2.SetCheck(FALSE);
	m_chkMCellJr1.SetCheck(TRUE);
	m_chkMCellJr2.SetCheck(FALSE);

	m_chkMCellJ1.SetCheck(TRUE);
	if(m_DataMCell.SectBefore.nJoint & 0x00000002) m_chkMCellJ2.SetCheck(TRUE);
	m_chkMCellJr1.SetCheck(TRUE);
	if(m_DataMCell.SectBefore.nJoint & 0x00000008) m_chkMCellJr2.SetCheck(TRUE);

	std::vector<CEditUnit*> aPtSizeEdt;
	GetSizeEdtArByCurState(aPtSizeEdt);
	for (int i = 0; i < aPtSizeEdt.size(); i++)
	{
		if(bAfterSymmetric)
		{
			if(!aPtSizeEdt[i]->IsWindowEnabled())
			{
				aPtSizeEdt[i]->SetEditUnit(m_DataMCell.SectBefore.SectI.Size[i]);
			}
		}
		else
		{
			aPtSizeEdt[i]->SetEditUnit(m_DataMCell.SectBefore.SectI.Size[i]);
		}    
	}

	// Set Offset Point
	if (m_DataMCell.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = m_aOffsetName[m_DataMCell.SectBefore.nOffsetPoint-1];
		m_edtOffset.SetWindowText(csOffset);
	}

	// Consider Shear Deformation
	m_bShearDeform = m_DataMCell.SectBefore.bConsiderShearDeform;
	m_bWarpingEffect = m_DataMCell.SectBefore.bConsiderWarpingEffect;

	UpdateData(FALSE);
}

void CCMSectItemStlGirder::ChangeSectType()
{
	if     (m_nLastType == D_SECT_TYPE_STLG_B) Dlg2Data_Box();
	else if(m_nLastType == D_SECT_TYPE_STLG_I) Dlg2Data_I();
	else if(m_nLastType == D_SECT_TYPE_STLG_MCELL) Dlg2Data_MCell();
	else ASSERT(0);

	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);
	
	if     (nCurType == D_SECT_TYPE_STLG_B) Data2Dlg_Box();
	else if(nCurType == D_SECT_TYPE_STLG_I) Data2Dlg_I();
	else if(nCurType == D_SECT_TYPE_STLG_MCELL) Data2Dlg_MCell(FALSE);
	else ASSERT(0);

	m_nLastType = nCurType;
}

void CCMSectItemStlGirder::SetOffsetData(T_SECT_SECTION_D* pData1, T_SECT_SECTION_D* pData2)
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

void CCMSectItemStlGirder::SymmetricSectAutoCalc()
{
	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);
	
	double dTop = 0, dBot = 0; // 계산되어 나오는 변수
	if(nCurType == D_SECT_TYPE_STLG_B)
	{
		CSectUtil::StlGSymmetricSectAutoCalc(m_DataBox, FALSE);

		m_edtTop.SetEditUnit(m_DataBox.SectAfter.SectI.Size[0]);
		m_edtBot.SetEditUnit(m_DataBox.SectAfter.SectI.Size[1]);
		m_edtB3.SetEditUnit(m_DataBox.SectBefore.SectI.Size[2]);
		m_edtB6.SetEditUnit(m_DataBox.SectBefore.SectI.Size[5]);
		m_edttw2.SetEditUnit(m_DataBox.SectBefore.SectI.Size[10]);
	}
	else if(nCurType == D_SECT_TYPE_STLG_I)
	{
		CSectUtil::StlGSymmetricSectAutoCalc(m_DataI, FALSE);

		m_edtTop.SetEditUnit(m_DataI.SectAfter.SectI.Size[0]);
		m_edtBot.SetEditUnit(m_DataI.SectAfter.SectI.Size[1]);
		m_edtB2.SetEditUnit(m_DataI.SectBefore.SectI.Size[1]);
		m_edtB4.SetEditUnit(m_DataI.SectBefore.SectI.Size[3]);
	}  
	else if(nCurType == D_SECT_TYPE_STLG_MCELL)
	{
		CSectUtil::StlGSymmetricSectAutoCalc(m_DataMCell, FALSE);
		// Data2Dlg는 이 함수 밖에서 부르자.. 적어도 최소한으로 불리도록 하기 위해서..
	}  
	else
	{
		ASSERT(0);
	}
}

void CCMSectItemStlGirder::DestroyView() 
{
	// Window가 있으면 없앤다.
	if(m_pStlgViewer != NULL)
	{
		m_pStlgViewer->DestroyWindow();
		//-------------------------------------------
		// m_pViewer가 CCMDlgBase를 상속받았으므로
		// 명시적인 delete을 하면 안된단다.. 이크~~~ 
		// delete m_pViewer;    
		m_pStlgViewer = NULL;    
	}
}

BOOL CCMSectItemStlGirder::DestroyWindow() 
{
	DestroyView();

	return CCMSectItemBase::DestroyWindow();
}

BEGIN_MESSAGE_MAP(CCMSectItemStlGirder, CCMSectItemBase)
	//{{AFX_MSG_MAP(CCMSectItemStlGirder)
	ON_REGISTERED_MESSAGE(CCMSectItemStlGirder::WM_AFTERINITDIALOG, OnAfterInitDialog)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_TYPE_CMB,        OnChangeSectTypeCmb)
	ON_BN_CLICKED(IDC_CMD_SECT_SYM_AUTO_CALC_CHK,  OnSymmetricAutoCalcBtn)
	ON_EN_CHANGE(IDC_CMD_SECT_TOP_EDT,             OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_BOT_EDT,             OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_B1_EDT,              OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_B2_EDT,              OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_B3_EDT,              OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_B4_EDT,              OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_B5_EDT,              OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_B6_EDT,              OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_H_EDT,               OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_T1_EDT,              OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_T2_EDT,              OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_TW1_EDT,             OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_TW2_EDT,             OnChangeSizeEdt)
	ON_BN_CLICKED(IDC_CMD_STIFFENER_BTN,           OnStiffenerBtn)
	ON_BN_CLICKED(IDC_CMD_MCELL_STIFFENER_BTN,     OnStiffenerBtn)  
	ON_BN_CLICKED(IDC_CMD_RIVET_ANGLE_BTN,         OnRivetAngleBtn)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN,       OnOffsetBtn)
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_SYM_CHK,      OnChangeCtrlConnectSize)  
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_SECT_MCELL_NO_CELL_SPN, OnSpin) 
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_B1_EDT     ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_B2_EDT     ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_B3_EDT     ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_B4_EDT     ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_B5_EDT     ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_B6_EDT     ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_H_EDT      ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_TF1_EDT    ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_TW1_EDT    ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_L_1_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_L_2_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_L_3_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_L_4_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_L_5_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_L_6_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_BR1_EDT    ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_BR2_EDT    ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_BR3_EDT    ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_BR4_EDT    ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_BR5_EDT    ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_TF2_EDT    ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_TW2_EDT    ,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_R_1_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_R_2_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_R_3_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_R_4_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_R_5_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_ADD_R_6_EDT,   OnChangeSizeEdt)
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_NO_CELL_EDT,   OnChangeCtrlConnectSize)
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_LEFT_RDO,     OnChangeCtrlConnectSize)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_RIGHT_RDO,    OnChangeCtrlConnectSize)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_GEN_RDO,      OnChangeFlangeShape)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_REBAR1_RDO,   OnChangeFlangeShape)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_REBAR2_RDO,   OnChangeFlangeShape)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_CLOSED_RDO,   OnChangeFlangeShape)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_J1_CHK,       OnChangeCtrlConnectSize)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_J2_CHK,       OnChangeCtrlConnectSize)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_JR1_CHK,      OnChangeCtrlConnectSize)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_JR2_CHK,      OnChangeCtrlConnectSize)  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemStlGirder message handlers

BOOL CCMSectItemStlGirder::OnInitDialog() 
{
	CCMSectItemBase::OnInitDialog();

	SetRedraw(FALSE);

	InitCtrl();
	InitUnit();
	InitView();
	InitImage();
	AlignControl();

	CSectUtil::GetOffsetNameList(m_aOffsetName);

	if (!m_bModify && !m_bInitDone)
	{
		// 시작 번호는 현재 번호에서 DB가 생성해 주는 다음 번호로 설정
		if(m_bWorkTab)
			m_Key = m_FixID;
		else
			m_Key = m_pDoc->m_pAttrCtrl->GetStartNumSect();

		m_Data.Initialize();
		m_Data.nStype = D_SECT_TYPE_STLG_B;    

		m_DataBox.Initialize();
		m_DataI.Initialize();
		m_DataMCell.Initialize();

		m_DataBox.nStype = D_SECT_TYPE_STLG_B;
		m_DataI.nStype = D_SECT_TYPE_STLG_I;
		m_DataMCell.nStype = D_SECT_TYPE_STLG_MCELL;

		m_DataBox.SectBefore.Shape = D_SECT_SHAPE_STLG_B;
		m_DataI.SectBefore.Shape = D_SECT_SHAPE_STLG_I;
		m_DataMCell.SectBefore.Shape = D_SECT_SHAPE_STLG_MCELL;
	}

	// J1과 Jr2은 무조건 On 시킴..  
	m_Data.SectBefore.nJoint |= 0x00000001; // J1
	m_Data.SectBefore.nJoint |= 0x00000004; // Jr2

	if(m_bModify)
	{
		if(!m_bInitDone)
		{
			m_DataBox.Initialize();
			m_DataI.Initialize();
			m_DataMCell.Initialize();
			
			m_DataBox.nStype = D_SECT_TYPE_STLG_B;
			m_DataI.nStype = D_SECT_TYPE_STLG_I;
			m_DataMCell.nStype = D_SECT_TYPE_STLG_MCELL;

			m_DataBox.SectBefore.Shape = D_SECT_SHAPE_STLG_B;
			m_DataI.SectBefore.Shape = D_SECT_SHAPE_STLG_I;
			m_DataMCell.SectBefore.Shape = D_SECT_SHAPE_STLG_MCELL;
		}
		
		if(m_Data.nStype == D_SECT_TYPE_STLG_B)
		{
			m_DataBox = m_Data;      
		}
		else if(m_Data.nStype == D_SECT_TYPE_STLG_I)
		{
			m_DataI = m_Data;
		}
		else if(m_Data.nStype == D_SECT_TYPE_STLG_MCELL)
		{
			m_DataMCell = m_Data;
		}
		else
		{
			ASSERT(0);
		}
		UpdateData(FALSE);
	}

	m_wndSecViewStlGirderBox.SetDataSource(&m_DataBox, FALSE);
	m_wndSecViewStlGirderI.SetDataSource(&m_DataI, FALSE);
	
	Data2Dlg(FALSE);

	// 초기화 되었음을 표시
	m_bInitDone = TRUE;

	if (m_bDesign) m_edtID.EnableWindow(FALSE);

	ChangeSectType();

	ControlsShowHide();
	EnableDisableControls();  
	// CreateOrDestoryView(); OnAfterInitDialog() 에서 부름

	ChangeText();
	
	AlignControlDynamic();

	this->PostMessage(WM_AFTERINITDIALOG);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemStlGirder::OnChangeSectTypeCmb()
{
	ChangeSectType();	
	ControlsShowHide();	
	EnableDisableControls();
	AlignControlDynamic();
	Dlg2Data();
	CreateOrDestoryView();
	ChangeText();
	
}

void CCMSectItemStlGirder::OnSymmetricAutoCalcBtn()
{
	UpdateData(TRUE);
	EnableDisableControls();

	Dlg2Data();

	SymmetricSectAutoCalc();

	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);
	if     (nCurType == D_SECT_TYPE_STLG_B) m_wndSecViewStlGirderBox.Invalidate();
	else if(nCurType == D_SECT_TYPE_STLG_I) m_wndSecViewStlGirderI.Invalidate();
	else if(nCurType == D_SECT_TYPE_STLG_MCELL) DrawViewDlgSection();
	else      		           ASSERT(0);
}

void CCMSectItemStlGirder::OnChangeFlangeShape()
{
	SetEdtValAuto();
	AlignControlDynamic();
	ControlsShowHide();
	EnableDisableControls();
	Dlg2Data();
	DrawViewDlgSection();
	ChangeText();
}

void CCMSectItemStlGirder::OnChangeCtrlConnectSize()
{
	SetEdtValAuto();
	AlignControlDynamic();
	ControlsShowHide();
	EnableDisableControls();
	Dlg2Data();
	if(m_chkMCellSym.GetCheck())
	{
		SymmetricSectAutoCalc();
		Data2Dlg(TRUE);
	}
	DrawViewDlgSection();
}

void CCMSectItemStlGirder::DrawViewDlgSection() 
{
	if(!IsViewWindow()) return;

	m_pStlgViewer->ChangeBitmap();
	m_pStlgViewer->RedrawSection();
}

void CCMSectItemStlGirder::OnChangeSizeEdt() 
{
	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);
	if(nCurType == D_SECT_TYPE_STLG_B)
	{
		Dlg2Data_Box();
		if(m_bSymAutoCalc)
		{
			SymmetricSectAutoCalc();
		}
		m_wndSecViewStlGirderBox.Invalidate();
	}
	else if(nCurType == D_SECT_TYPE_STLG_I)
	{
		Dlg2Data_I();
		if(m_bSymAutoCalc)
		{
			SymmetricSectAutoCalc();
		}
		m_wndSecViewStlGirderI.Invalidate();
	}
	else if(nCurType == D_SECT_TYPE_STLG_MCELL)
	{
		SetEdtValAuto();
		Dlg2Data_MCell();
		if(m_chkMCellSym.GetCheck())
		{
			SymmetricSectAutoCalc();
			Data2Dlg(TRUE);
		}
		DrawViewDlgSection();
	}
	else
	{
		ASSERT(0);
	}
}

void CCMSectItemStlGirder::OnStiffenerBtn() 
{
	CCMSectSODDeckStiffenerDlg dlg;

	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);

	dlg.m_bTapJ = FALSE;
		
	if     (nCurType == D_SECT_TYPE_STLG_B)     dlg.m_Data = m_DataBox; // Steel Box
	else if(nCurType == D_SECT_TYPE_STLG_I)     dlg.m_Data = m_DataI;   // Steel-I
	else if(nCurType == D_SECT_TYPE_STLG_MCELL) dlg.m_Data = m_DataMCell;   // Steel MCell	
	else ASSERT(0);
	
	if(dlg.DoModal() == IDOK)
	{
		if     (nCurType == D_SECT_TYPE_STLG_B)     m_DataBox   = dlg.m_Data; // Steel Box
		else if(nCurType == D_SECT_TYPE_STLG_I)     m_DataI     = dlg.m_Data; // Steel-I
		else if(nCurType == D_SECT_TYPE_STLG_MCELL) m_DataMCell = dlg.m_Data; // Steel MCell	
		else ASSERT(0);		
	}

	if     (nCurType == D_SECT_TYPE_STLG_B) m_wndSecViewStlGirderBox.SetDataSource(&m_DataBox);
	else if(nCurType == D_SECT_TYPE_STLG_I) m_wndSecViewStlGirderI  .SetDataSource(&m_DataI);  
	else if(nCurType == D_SECT_TYPE_STLG_MCELL) DrawViewDlgSection();
	else ASSERT(0);
	
	RedrawWindow();
}

void CCMSectItemStlGirder::OnRivetAngleBtn()
{
	// I 형만 지원한다.
	CCMSectRivetAngleDlg dlg;
	dlg.m_Data = m_DataI;

	if (dlg.DoModal() == IDOK)
	{
		m_DataI = dlg.m_Data;
	}

	RedrawWindow();
}

void CCMSectItemStlGirder::OnOffsetBtn() 
{
	// TODO: Add your control notification handler code here
	T_SECT_SECTION_D Old_Box, Old_I, Old_MCell;
	SetOffsetData(&m_DataBox.SectBefore,  &Old_Box);
	SetOffsetData(&m_DataI.SectBefore,    &Old_I);
	SetOffsetData(&m_DataMCell.SectBefore,&Old_MCell);
		
	// 일단 current data만 offset 정보를 얻어온 다음,
	// 나머지 data에도 setting
	DWORD nCurType;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSectType, nCurType);
	if     (nCurType == D_SECT_TYPE_STLG_B) SetOffsetData(&m_DataBox.SectBefore,       &m_Data.SectBefore);
	else if(nCurType == D_SECT_TYPE_STLG_I) SetOffsetData(&m_DataI.SectBefore,         &m_Data.SectBefore);
	else if(nCurType == D_SECT_TYPE_STLG_MCELL) SetOffsetData(&m_DataMCell.SectBefore, &m_Data.SectBefore);
	else ASSERT(0)  ;
	
	CCMSectOffsetDlg dlg(&m_Data.SectBefore, FALSE, this);
	if (dlg.DoModal() == IDOK)
	{
		SetOffsetData(&m_Data.SectBefore, &m_DataBox.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataI.SectBefore);
		SetOffsetData(&m_Data.SectBefore, &m_DataMCell.SectBefore);
		
		if (m_Data.SectBefore.nOffsetPoint >= 1)
		{
			CString csOffset = m_aOffsetName[m_Data.SectBefore.nOffsetPoint-1];
			m_edtOffset.SetWindowText(csOffset);
		}
	}	
	else
	{
		SetOffsetData(&Old_Box,   &m_DataBox.SectBefore);
		SetOffsetData(&Old_I,     &m_DataI.SectBefore);
		SetOffsetData(&Old_MCell, &m_DataMCell.SectBefore);
	}  
	
	m_wndSecViewStlGirderBox.Invalidate();	
	m_wndSecViewStlGirderI.Invalidate();	
	DrawViewDlgSection();
}

void CCMSectItemStlGirder::OnSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	pNMUpDown->iDelta = -pNMUpDown->iDelta;
	CDlgUtil::SetSpinValue(this, IDC_CMD_SECT_MCELL_NO_CELL_EDT, D_SECT_STLG_MCEL_NO_MIN, D_SECT_STLG_MCEL_NO_MAX, pNMHDR, pResult);

	*pResult = 0;
}

void CCMSectItemStlGirder::CreateSTLGView() 
{
	if(IsViewWindow()) return;

	m_pStlgViewer = new CCMSectItemStlgView();
	m_pStlgViewer->SetInitPos(D_INIT_POS_RT);
	m_pStlgViewer->Create(IDD_CMD_SECT_ITEM_STLG_VIEW, this);
	m_pStlgViewer->ShowWindow(SW_SHOW);
	m_pStlgViewer->SetDataSource(&m_DataMCell);
}

BOOL CCMSectItemStlGirder::IsViewWindow()
{ 
	if(m_pStlgViewer == NULL) return FALSE;
	HWND hWnd = m_pStlgViewer->GetSafeHwnd();
	if(!hWnd) return FALSE;
	if(!(::IsWindow(hWnd))) return FALSE;

	return TRUE;
}

void CCMSectItemStlGirder::SetEdtValAuto()
{
	int nFShape;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoMCellFShape, nFShape);

	int nCellNum = m_edtMCellNoCell.GetEditValue();    

	if((nFShape == 0 || nFShape == 1 || nFShape == 2) && nCellNum != 0)
	{
		double dB2 = m_edtMCellB2.GetEditValue();
		double dB3 = m_edtMCellB3.GetEditValue();
		m_edtMCellB5.SetEditUnit(max(dB2 - dB3, 0.0));

		double dBr2 = m_edtMCellBr2.GetEditValue();
		double dBr3 = m_edtMCellBr3.GetEditValue();
		m_edtMCellBr5.SetEditUnit(max(dBr2 - dBr3, 0.0));
	}
	else if(nFShape == 3)
	{
		double dB1 = m_edtMCellB1.GetEditValue();
		double dB2 = m_edtMCellB2.GetEditValue();
		m_edtMCellB3.SetEditUnit(max(dB1 - dB2, 0.0));

		double dBr1 = m_edtMCellBr1.GetEditValue();
		double dBr2 = m_edtMCellBr2.GetEditValue();
		m_edtMCellBr3.SetEditUnit(max(dBr1 - dBr2, 0.0));
	}
}

// 뷰의 위치문제로 인해 사용자 정의 메세지(OnInitDialog 직후에 날아옴)를 이용
LRESULT CCMSectItemStlGirder::OnAfterInitDialog(WPARAM wParam, LPARAM lParam)
{
	CreateOrDestoryView();
	DrawViewDlgSection();

	return 0L;
}